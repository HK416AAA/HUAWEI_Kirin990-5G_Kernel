/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2021. All rights reserved.
 * Description: HHEE log driver in kernel
 * Date: 2017/2/1
 */

#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/list.h>
#include <linux/completion.h>
#include <linux/workqueue.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <securec.h>
#include <asm/page.h>
#include <linux/string.h>
#include "hhee.h"

static int hhee_logging_enabled;
struct circular_buffer *normal_cb;
struct circular_buffer *crash_cb;
struct circular_buffer *monitor_cb;

/*
 * Initialize the shared buffer for HHEE logging.
 * The shared buffer is allocated in DMA memory to get uncached memory
 * since TLK directly writes to the physical address of the shared buffer.
 * The structure is declared in DMA memory too since it's members will
 * also be updated by the TLK directly to their physical addresses.
 */
static int circ_buf_init(struct circular_buffer *cir_buf)
{
	struct circular_buffer *tmp_cb = (struct circular_buffer *)(uintptr_t)cir_buf->virt_log_addr;

	if (cir_buf->inited == 0) {
		if (memset_s(cir_buf, sizeof(struct circular_buffer), 0,
			     sizeof(struct circular_buffer)) != EOK) {
			pr_err("[%s] memset_s error\n", __func__);
			return -EFAULT;
		}
		cir_buf->virt_log_addr = (uint64_t)(uintptr_t)tmp_cb;
		cir_buf->start = tmp_cb->start;
		cir_buf->inited = 1;
	}

	cir_buf->size = tmp_cb->size;
	cir_buf->end = tmp_cb->end;
	cir_buf->overflow = tmp_cb->overflow;
	cir_buf->buf = (char *)(tmp_cb);

	return 0;
}

/*
 * Copy the contents of the circular buffer into a char buffer in order.
 * This helps to treat the buffer like a string and use it to tokenize it
 * into lines, tag and display it.
 */
static ssize_t circ_buf_copy(struct circular_buffer *incb, char *text)
{
	size_t len = 0;
	unsigned long tmpsize;

	if (incb->end == incb->start)
		return 0;

	tmpsize = (unsigned long)abs((long)(incb->end - incb->start));
	if (tmpsize > incb->size) {
		pr_err("%s: cbuf pointers corrupted\n", __func__);
		return -EINVAL;
	}

	if (incb->end > incb->start) {
		len += incb->end - incb->start;
		if (memcpy_s(text, incb->end - incb->start, incb->buf + incb->start,
			     incb->end - incb->start) != EOK) {
			pr_err("%s: memcpy_s error\n", __func__);
			return -EFAULT;
		}
	} else if (incb->start > incb->end) {
		if (memcpy_s(text, incb->size - incb->start, incb->buf + incb->start,
			     incb->size - incb->start) != EOK) {
			pr_err("%s: memcpy_s error\n", __func__);
			return -EFAULT;
		}
		len += incb->size - incb->start;
		if (memcpy_s(text + incb->size - incb->start, incb->end, incb->buf,
			     incb->end) != EOK) {
			pr_err("%s: memcpy_s error\n", __func__);
			return -EFAULT;
		}
		len += incb->end;
	}

	return (ssize_t)len;
}

ssize_t hhee_copy_logs(char __user *buf, size_t count, loff_t *offp,
		       int logtype)
{
	ssize_t len;
	char *buffer = NULL;
	struct circular_buffer *incb = NULL;

	if (!hhee_logging_enabled)
		return 0;

	if (NORMAL_LOG == logtype)
		incb = normal_cb;
	else if (CRASH_LOG == logtype)
		incb = crash_cb;
	else if (MONITOR_LOG == logtype)
		incb = monitor_cb;
	else {
		pr_err("logtype err\n");
		return -EINVAL;
	}

	circ_buf_init(incb);

	buffer = (char *)kzalloc(incb->size, GFP_KERNEL);
	if (!buffer) {
		pr_err("kzalloc failed\n");
		return -ENOMEM;
	}

	/* This detects if the buffer proved to be too small to hold the data.
	 * If buffer is not large enough, it overwrites it's oldest data,
	 * This warning serves to alert the user to possibly use a bigger buffer
	 */
	if (incb->overflow == 1) {
		pr_info("\n[HHEE] buffer overwritten.\n\n");
		incb->overflow = 0;
	}

	if ((len = circ_buf_copy(incb, buffer)) < 0) {
		kfree(buffer);
		return -EFAULT;
	}
	incb->buf[incb->end] = '\0';

	/* read offset clear */
	*offp = 0;
	len = simple_read_from_buffer(buf, count, offp, buffer,
				      (unsigned long)len);
	if (len < 0) {
		pr_err("hhee log simple_read_from_buffer err\n");
		kfree(buffer);
		return -EFAULT;
	}

	if (incb->start + len < incb->size)
		incb->start = incb->start + (size_t)len;
	else
		incb->start = (unsigned long)(incb->start + (size_t)len - incb->size);

	kfree(buffer);
	return len;
}

static int cb_init(uint64_t inlog_addr, uint64_t inlog_size,
		   struct circular_buffer **incb, unsigned int logtype)
{
	uint64_t log_addr;
	struct circular_buffer *tmp_cb = NULL;

	log_addr = (uint64_t)(uintptr_t)ioremap_cache(inlog_addr, inlog_size);
	if (!log_addr) {
		pr_err("fail to get virtal addr of hhee\n");
		return -EINVAL;
	}

	tmp_cb = (struct circular_buffer *)kzalloc(
		sizeof(struct circular_buffer), GFP_KERNEL);
	if (!tmp_cb) {
		iounmap((void __iomem *)(uintptr_t)log_addr);
		pr_err("%s: no memory avaiable for circular buffer struct\n",
		       __func__);
		return -ENOMEM;
	}

	tmp_cb->virt_log_addr = log_addr;
	tmp_cb->virt_log_size = inlog_size;
	tmp_cb->logtype = logtype;
	*incb = tmp_cb;
	return 0;
}

static void cb_deinit(struct circular_buffer *incb)
{
	if (incb && incb->virt_log_addr)
		iounmap((void __iomem *)(uintptr_t)incb->virt_log_addr);
	if (incb)
		kfree(incb);
}

/*
 * Call function to initialize circular buffer.
 * An HVC is made to send the virtual address of the structure to
 * HHEE
 */
int hhee_logger_init(void)
{
	struct arm_smccc_res ret_res;
	int ret;

	pr_info("hhee logger init\n");
	/* get logging information */
	/* normal buffer */
	ret_res = hhee_fn_hvc((unsigned long)HHEE_LOGBUF_INFO, 0ul, 0ul, 0ul);
	ret = cb_init(ret_res.a0, ret_res.a1, &normal_cb, NORMAL_LOG);
	if (ret)
		goto outfail1;

	/* crash buffer */
	ret_res = hhee_fn_hvc((unsigned long)HHEE_CRASHLOG_INFO, 0ul, 0ul, 0ul);
	ret = cb_init(ret_res.a0, ret_res.a1, &crash_cb, CRASH_LOG);
	if (ret)
		goto outfail2;

	/* monitor buffer */
	ret_res = hhee_fn_hvc((unsigned long)HHEE_PMFBUFLOG_INFO, 0ul, 0ul, 0ul);
	ret = cb_init(ret_res.a0, ret_res.a1, &monitor_cb, MONITOR_LOG);
	if (ret)
		goto outfail3;

	hhee_logging_enabled = 1;

	pr_info("HHEE logger init done\n");
	return 0;
outfail3:
	cb_deinit(crash_cb);
outfail2:
	cb_deinit(normal_cb);
outfail1:
	pr_info("HHEE log init failed!\n");
	return -EINVAL;
}
