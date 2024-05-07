/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/kallsyms.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/wait.h>

#include <osl_types.h>
#include <osl_bio.h>
#include <bsp_shared_ddr.h>
#include <bsp_icc.h>
#include <bsp_modem_log.h>
#include <bsp_cshell.h>
#include <bsp_print.h>
#include "cshell_logger.h"
#include "securec.h"

#undef THIS_MODU
#define THIS_MODU mod_cshell_logger

#define CSHELL_TAG_BUFF_SIZE (8 * 1024)
#define LOGGER_LOG_CSHELL "cshell_log"
#define CSHELL_LOG_DEBUG(fmt, ...) (bsp_debug("%s " fmt, __FUNCTION__, ##__VA_ARGS__))
#define CSHELL_LOG_ERR(fmt, ...) (bsp_err("%s " fmt, __FUNCTION__, ##__VA_ARGS__))

static unsigned char g_buf_cshell_log[CSHELL_TAG_BUFF_SIZE];

/*
 * struct cshell_logger - cshell logger manager struct
 * This structure is allocated by writer
 */
struct cshell_logger {
    u32 init_flag;                /* module init flag */
    u32 threshold;                /* threshold to trigger file write from ring buffer to log record file */
    struct log_mem log_mem;       /* log memory to management ring buffer */
    struct log_usr_info log_info; /* log info to register to modem log */
};
static struct cshell_logger g_cshell_logger;

/*
 * cshell_log_writable_size_get - space left to write
 */
static inline u32 cshell_log_writable_size_get(u32 write, u32 read, u32 ring_buffer_size)
{
    return (read > write) ? (read - write) : (ring_buffer_size - write + read); /*lint !e834*/
}

/*
 * cshell_log_open - indicate that our memory(ring buffer) is ready
 */

int cshell_log_open(struct log_usr_info *usr_info)
{
    struct cshell_open_s flags;
    s32 ret;
    usr_info->mem = (struct log_mem *)&g_cshell_logger.log_mem;
    usr_info->ring_buf = (char *)g_buf_cshell_log;
    usr_info->mem_is_ok = 1;
    CSHELL_LOG_ERR("%s entry\n", __func__);
    flags.cshell_open_flags = (((u32)(((u32)0x1 << CSHELL_PMOM)) << 0x10) | ((u32)0x1 << CSHELL_PMOM));
#ifdef CONFIG_NR_CSHELL
    ret = bsp_icc_send((u32)ICC_CPU_NRCCPU, (u32)((ICC_CHN_NRIFC << 0x10) | NRIFC_RECV_FUNC_CSHELL), (u8 *)&flags,
                       (u32)sizeof(struct cshell_open_s));
#else
    ret = bsp_icc_send((u32)ICC_CPU_MODEM, (u32)((ICC_CHN_IFC << 0x10) | IFC_RECV_FUNC_CSHELL), (u8 *)&flags,
                       (u32)sizeof(struct cshell_open_s));
#endif
    if (ret != (int)sizeof(struct cshell_open_s)) {
        CSHELL_LOG_ERR("cshell_log_open failed,icc send fail,ret=0x%x\n", ret);
        return -1;
    }
    return 0;
}
/*
 * cshell_log_open - indicate that our memory(ring buffer) is ready
 */

int cshell_log_close(struct log_usr_info *usr_info)
{
    struct cshell_open_s flags;
    s32 ret;
    usr_info->mem_is_ok = 0;
    CSHELL_LOG_ERR("%s entry\n", __func__);
    flags.cshell_open_flags = (((u32)(((u32)0x1 << CSHELL_PMOM)) << 0x10) & 0xFFFF0000);
#ifdef CONFIG_NR_CSHELL
    ret = bsp_icc_send((u32)ICC_CPU_NRCCPU, (u32)((ICC_CHN_NRIFC << 0x10) | NRIFC_RECV_FUNC_CSHELL), (u8 *)&flags,
                       (u32)sizeof(struct cshell_open_s));
#else
    ret = bsp_icc_send((u32)ICC_CPU_MODEM, (u32)((ICC_CHN_IFC << 0x10) | IFC_RECV_FUNC_CSHELL), (u8 *)&flags,
                       (u32)sizeof(struct cshell_open_s));
#endif
    if (ret != (int)sizeof(struct cshell_open_s)) {
        CSHELL_LOG_ERR("cshell_log_close failed,icc send fail,ret=0x%x\n", ret);
        return -1;
    }
    return 0;
}

/*
 * cshell_log_read - indicate that our memory(ring buffer) is ready
 */
/*lint -e715*/
int cshell_log_read(struct log_usr_info *usr_info, char *buf, u32 count)
{
    u32 write_p = usr_info->mem->write;
    u32 read_p = usr_info->mem->read;

    /* new log message will be abandoned */
    if (cshell_log_writable_size_get(write_p, read_p, usr_info->mem->size) <= 200U) {
        usr_info->mem->read = usr_info->mem->write;
    }
    CSHELL_LOG_DEBUG("%s entry\n", __func__);
    return 0;
}

static s32 do_write_log(void *dest, u32 destMax, void *src, u32 count)
{
    errno_t err;
    err = memcpy_s(dest, destMax, src, count); /*lint !e747*/
    if (err != EOK) {
        return err;
    }
    return 0;
}
/*lint +e715*/
int a_cshell_write(int log_left, void *in_buff)
{
    struct ring_buffer rb;
    int ret;

    if (!g_cshell_logger.init_flag) {
        ret = -1;
        return ret;
    }

    rb.size = g_cshell_logger.log_mem.size;
    rb.read = g_cshell_logger.log_mem.read;
    rb.write = g_cshell_logger.log_mem.write;
    rb.buf = (char *)g_buf_cshell_log;
    log_left = min(log_left, (int)LOGGER_ENTRY_MAX_PAYLOAD);

    ret = bsp_ring_buffer_in(&rb, (void *)in_buff, log_left, (MEMCPY_FUNC)do_write_log); /*lint !e732*/
    g_cshell_logger.log_mem.write = rb.write;

    /* write length > threshold, try to trigger file write */
    if (bsp_ring_buffer_writen_size(&rb) > g_cshell_logger.threshold) {
        bsp_modem_log_fwrite_trigger(&g_cshell_logger.log_info);
    }

    return ret;
}
EXPORT_SYMBOL(a_cshell_write);

int cshell_logger_init(void)
{
    errno_t err;
    err = memset_s((void *)&g_cshell_logger, sizeof(g_cshell_logger), 0, sizeof(g_cshell_logger));
    if (err != EOK) {
        return err;
    }

    g_cshell_logger.log_mem.size = (u32)sizeof(g_buf_cshell_log);
    g_cshell_logger.log_mem.app_is_active = 0;

    g_cshell_logger.log_info.dev_name = LOGGER_LOG_CSHELL;
    g_cshell_logger.log_info.read_func = (USR_READ_FUNC)cshell_log_read;
    g_cshell_logger.log_info.open_func = (USR_OPEN_FUNC)cshell_log_open;
    g_cshell_logger.log_info.close_func = (USR_CLOSE_FUNC)cshell_log_close;
    g_cshell_logger.log_info.mem = (struct log_mem *)&g_cshell_logger.log_mem;
    g_cshell_logger.log_info.ring_buf = (char *)g_buf_cshell_log;
    g_cshell_logger.log_info.mem_is_ok = 1;
    if (bsp_modem_log_register(&g_cshell_logger.log_info)) {
        CSHELL_LOG_ERR("register to modem log fail\n");
        return -1;
    }

    g_cshell_logger.threshold = CSHELL_TAG_BUFF_SIZE / 0x2;
    g_cshell_logger.init_flag = 1;
    CSHELL_LOG_ERR("init ok\n");
    return 0;
}
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
/*lint -e528*/
module_init(cshell_logger_init);
#endif
