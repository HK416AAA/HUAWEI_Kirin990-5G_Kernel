/*
 * tz_spi_notify.c
 *
 * exported funcs for spi interrupt actions
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "tz_spi_notify.h"
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/of_reserved_mem.h>
#include <linux/atomic.h>
#include <linux/interrupt.h>
#include <securec.h>
#include "teek_client_constants.h"
#include "tc_ns_client.h"
#include "tc_ns_log.h"
#include "tc_client_driver.h"
#include "gp_ops.h"
#include "mailbox_mempool.h"
#include "smc_smp.h"
#include "session_manager.h"
#include "tz_kthread_affinity.h"

#define DEFAULT_SPI_NUM 111

#define MAX_CALLBACK_COUNT 100
#define UUID_SIZE 16
struct teec_timer_property;

#ifdef DEF_ENG
static int g_timer_type;
#endif

enum timer_class_type {
	/* timer event using timer10 */
	TIMER_GENERIC,
	/* timer event using RTC */
	TIMER_RTC
};

struct teec_timer_property {
	unsigned int type;
	unsigned int timer_id;
	unsigned int timer_class;
	unsigned int reserved2;
};

struct notify_context_timer {
	unsigned int dev_file_id;
	unsigned char uuid[UUID_SIZE];
	unsigned int session_id;
	struct teec_timer_property property;
	uint32_t expire_time;
};


struct notify_context_wakeup {
	pid_t ca_thread_id;
};

struct notify_context_shadow {
	uint64_t target_tcb;
};

#ifdef CONFIG_TA_AFFINITY

#define AFF_BITS_SIZE 64

#define AFF_BITS_NUM ((CONFIG_TA_AFFINITY_CPU_NUMS % AFF_BITS_SIZE == 0) ? \
	(CONFIG_TA_AFFINITY_CPU_NUMS / AFF_BITS_SIZE) : \
	(CONFIG_TA_AFFINITY_CPU_NUMS / AFF_BITS_SIZE + 1))

#define aff_bits_mask(cpuid) \
	(1LLU << (cpuid - (cpuid / AFF_BITS_SIZE) * AFF_BITS_SIZE))

struct aff_bits_t {
	uint64_t aff_bits[AFF_BITS_NUM];
};

struct notify_context_set_affinity {
	pid_t ca_thread_id;
	struct aff_bits_t aff;
};

#endif

struct notify_context_stats {
	uint32_t send_s;
	uint32_t recv_s;
	uint32_t send_w;
	uint32_t recv_w;
#ifdef CONFIG_TA_AFFINITY
	uint32_t send_af;
	uint32_t recv_af;
#endif
	uint32_t missed;
};

union notify_context {
	struct notify_context_timer timer;
	struct notify_context_wakeup wakeup;
	struct notify_context_shadow shadow;
#ifdef CONFIG_TA_AFFINITY
	struct notify_context_set_affinity affinity;
#endif
	struct notify_context_stats  meta;
};

struct notify_data_entry {
	uint32_t entry_type : 31;
	uint32_t filled	 : 1;
	union notify_context context;
};

#ifdef CONFIG_BIG_SESSION

#define NOTIFY_DATA_ENTRY_COUNT \
	(((PAGE_SIZE * ((1U) << (CONFIG_NOTIFY_PAGE_ORDER))) \
		/ sizeof(struct notify_data_entry)) - 1)
#else
#define NOTIFY_DATA_ENTRY_COUNT \
	((PAGE_SIZE / sizeof(struct notify_data_entry)) - 1)
#endif

struct notify_data_struct {
	struct notify_data_entry entry[NOTIFY_DATA_ENTRY_COUNT];
	struct notify_data_entry meta;
};

static struct notify_data_struct *g_notify_data;
static struct notify_data_entry *g_notify_data_entry_shadow;
static spinlock_t g_notify_lock;

enum notify_data_type {
	NOTIFY_DATA_ENTRY_UNUSED,
	NOTIFY_DATA_ENTRY_TIMER,
	NOTIFY_DATA_ENTRY_RTC,
	NOTIFY_DATA_ENTRY_WAKEUP,
	NOTIFY_DATA_ENTRY_SHADOW,
	NOTIFY_DATA_ENTRY_FIQSHD,
	NOTIFY_DATA_ENTRY_SHADOW_EXIT,
#ifdef CONFIG_TA_AFFINITY
	NOTIFY_DATA_ENTRY_SET_AFFINITY,
#endif
	NOTIFY_DATA_ENTRY_MAX,
};

struct tc_ns_callback {
	unsigned char uuid[UUID_SIZE];
	struct mutex callback_lock;
	void (*callback_func)(void *);
	struct list_head head;
};

struct tc_ns_callback_list {
	unsigned int callback_count;
	struct mutex callback_list_lock;
	struct list_head callback_list;
};

static void tc_notify_fn(struct work_struct *dummy);
static struct tc_ns_callback_list g_ta_callback_func_list;
static DECLARE_WORK(tc_notify_work, tc_notify_fn);
static struct workqueue_struct *g_tz_spi_wq;

static void walk_callback_list(
	struct notify_context_timer *tc_notify_data_timer)
{
	struct tc_ns_callback *callback_func_t = NULL;

	mutex_lock(&g_ta_callback_func_list.callback_list_lock);
	list_for_each_entry(callback_func_t,
		&g_ta_callback_func_list.callback_list, head) {
		if (memcmp(callback_func_t->uuid, tc_notify_data_timer->uuid,
			UUID_SIZE))
			continue;

		if (tc_notify_data_timer->property.timer_class ==
			TIMER_RTC) {
			tlogd("start to call callback func\n");
			callback_func_t->callback_func(
				&(tc_notify_data_timer->property));
			tlogd("end to call callback func\n");
		} else if (tc_notify_data_timer->property.timer_class ==
			TIMER_GENERIC) {
			tlogd("timer60 no callback func\n");
		}
	}
	mutex_unlock(&g_ta_callback_func_list.callback_list_lock);
}

static int find_notify_sess(
	const struct notify_context_timer *tc_notify_data_timer,
	struct tc_ns_session **temp_ses, bool *enc_found)
{
	struct tc_ns_dev_file *temp_dev_file = NULL;
	struct tc_ns_dev_list *dev_list = NULL;
	struct tc_ns_service *temp_svc = NULL;

	dev_list = get_dev_list();
	if (!dev_list) {
		tloge("dev list is invalid\n");
		return -ENOENT;
	}

	mutex_lock(&dev_list->dev_lock);
	list_for_each_entry(temp_dev_file, &dev_list->dev_file_list, head) {
		tlogd("dev file id1 = %u, id2 = %u\n",
		      temp_dev_file->dev_file_id,
		      tc_notify_data_timer->dev_file_id);
		if (temp_dev_file->dev_file_id ==
			tc_notify_data_timer->dev_file_id) {
			mutex_lock(&temp_dev_file->service_lock);
			temp_svc =
				tc_find_service_in_dev(temp_dev_file,
					tc_notify_data_timer->uuid, UUID_LEN);
			mutex_unlock(&temp_dev_file->service_lock);
			if (!temp_svc)
				break;
			get_service_struct(temp_svc);
			mutex_lock(&temp_svc->session_lock);
			*temp_ses =
				tc_find_session_withowner(
					&temp_svc->session_list,
					tc_notify_data_timer->session_id,
					temp_dev_file);
			get_session_struct(*temp_ses);
			mutex_unlock(&temp_svc->session_lock);
			put_service_struct(temp_svc);
			temp_svc = NULL;
			if (*temp_ses) {
				tlogd("send cmd ses id %u\n",
					(*temp_ses)->session_id);
				*enc_found = true;
				break;
			}
			break;
		}
	}
	mutex_unlock(&dev_list->dev_lock);

	return 0;
}

static void tc_notify_timer_fn(struct notify_data_entry *notify_data_entry)
{
	struct tc_ns_session *temp_ses = NULL;
	bool enc_found = false;
	struct notify_context_timer *tc_notify_data_timer = NULL;

	tc_notify_data_timer = &(notify_data_entry->context.timer);
	notify_data_entry->filled = 0;
	tlogd("notify data timer type is 0x%x, timer ID is 0x%x\n",
	      tc_notify_data_timer->property.type,
	      tc_notify_data_timer->property.timer_id);
	walk_callback_list(tc_notify_data_timer);

	if (find_notify_sess(tc_notify_data_timer, &temp_ses, &enc_found))
		return;

	if (tc_notify_data_timer->property.timer_class == TIMER_GENERIC) {
		tlogd("timer60 wake up event\n");
		if (enc_found && temp_ses) {
			temp_ses->wait_data.send_wait_flag = 1;
			wake_up(&temp_ses->wait_data.send_cmd_wq);
			put_session_struct(temp_ses);
			temp_ses = NULL;
		}
	} else {
		tlogd("RTC do not need to wakeup\n");
	}
}

static noinline int get_notify_data_entry(struct notify_data_entry *copy)
{
	uint32_t i;
	int filled;
	int ret = -1;

	if (!copy || !g_notify_data) {
		tloge("bad parameters or notify data is NULL");
		return ret;
	}

	spin_lock(&g_notify_lock);
	/* TIMER and RTC use fix entry, skip them. */
	for (i = NOTIFY_DATA_ENTRY_UNUSED; i < NOTIFY_DATA_ENTRY_COUNT; i++) {
		struct notify_data_entry *e = &g_notify_data->entry[i];
		filled = e->filled;
		smp_mb();
		if (!filled)
			continue;
		switch (e->entry_type) {
		case NOTIFY_DATA_ENTRY_TIMER:
		case NOTIFY_DATA_ENTRY_RTC:
			break;
		case NOTIFY_DATA_ENTRY_SHADOW:
		case NOTIFY_DATA_ENTRY_SHADOW_EXIT:
		case NOTIFY_DATA_ENTRY_FIQSHD:
			g_notify_data->meta.context.meta.recv_s++;
			break;
		case NOTIFY_DATA_ENTRY_WAKEUP:
			g_notify_data->meta.context.meta.recv_w++;
			break;
#ifdef CONFIG_TA_AFFINITY
		case NOTIFY_DATA_ENTRY_SET_AFFINITY:
			g_notify_data->meta.context.meta.recv_af++;
			break;
#endif
		default:
			tloge("invalid notify type=%u\n", e->entry_type);
			goto exit;
		}
		if (memcpy_s(copy, sizeof(*copy), e, sizeof(*e)) != EOK) {
			tloge("memcpy entry failed\n");
			break;
		}
		smp_mb();
		e->filled = 0;
		ret = 0;
		break;
	}
exit:
	spin_unlock(&g_notify_lock);
	return ret;
}

static void tc_notify_wakeup_fn(const struct notify_data_entry *entry)
{
	const struct notify_context_wakeup *tc_notify_wakeup = NULL;

	tc_notify_wakeup = &(entry->context.wakeup);
	smc_wakeup_ca(tc_notify_wakeup->ca_thread_id);
	tlogd("notify data entry wakeup ca: %d\n",
		tc_notify_wakeup->ca_thread_id);
}

static void tc_notify_shadow_fn(const struct notify_data_entry *entry)
{
	const struct notify_context_shadow *tc_notify_shadow = NULL;

	tc_notify_shadow = &(entry->context.shadow);
	smc_queue_shadow_worker(tc_notify_shadow->target_tcb);
}

static void tc_notify_fiqshd_fn(const struct notify_data_entry *entry)
{
	const struct notify_context_shadow *tc_notify_shadow = NULL;

	if (!entry) {
		/* for NOTIFY_DATA_ENTRY_FIQSHD missed */
		fiq_shadow_work_func(0);
		return;
	}
	tc_notify_shadow = &(entry->context.shadow);
	fiq_shadow_work_func(tc_notify_shadow->target_tcb);
}

static void tc_notify_shadowexit_fn(const struct notify_data_entry *entry)
{
	const struct notify_context_wakeup *tc_notify_wakeup = NULL;

	tc_notify_wakeup = &(entry->context.wakeup);
	if (smc_shadow_exit(tc_notify_wakeup->ca_thread_id))
		tloge("shadow ca exit failed: %d\n",
			(int)tc_notify_wakeup->ca_thread_id);
}

#ifdef CONFIG_TA_AFFINITY
static void tc_notify_set_affinity(struct notify_data_entry *entry)
{
	struct notify_context_set_affinity *af_data = NULL;
	struct pending_entry *pe = NULL;

	af_data = &(entry->context.affinity);
	pe = find_pending_entry(af_data->ca_thread_id);
	if (pe != NULL) {
		struct cpumask mask;
		uint32_t i;

		cpumask_clear(&mask);
		for (i = 0; i < (uint32_t)NR_CPUS; i++) {
			struct aff_bits_t *aff = &af_data->aff;
			if (aff->aff_bits[i / AFF_BITS_SIZE] & aff_bits_mask(i))
				cpumask_set_cpu(i, &mask);
		}

		/*
		 * we don't set ca's cpumask here but in ca's own thread
		 * context after ca is wakeup in smc_send_func, or
		 * scheduler will set task's allow cpumask failure in that case.
		 */
		cpumask_copy(&pe->ta_mask, &mask);
		smc_wakeup_ca(af_data->ca_thread_id);
		tlogd("set affinity for ca thread id %u\n", af_data->ca_thread_id);
		put_pending_entry(pe);
	} else {
		tloge("invalid ca thread id %u for set affinity\n",
			af_data->ca_thread_id);
		/*
		 * if a TEE tcb without CA bind(CA is 0) cause a affinity set,
		 * the CA tid(current cpu context) may wrong
		 * (in tc_notify_fiqshd_fn, don't init_pending_entry,
		 * in this case, cannot find pending_entry),
		 * but we must set affinity for CA otherwise the TA can't run,
		 * so we wakeup all blocked CA.
		 */
		(void)smc_wakeup_broadcast();
	}
}
#endif

#define MISSED_COUNT 4
static void spi_broadcast_notifications(void)
{
	uint32_t missed;

	smp_mb();

	if (!g_notify_data) {
		tloge("notify data is NULL\n");
		return;
	}

	missed = __xchg(0, &g_notify_data->meta.context.meta.missed,
		MISSED_COUNT);
	if (!missed)
		return;
	if (missed & (1U << NOTIFY_DATA_ENTRY_WAKEUP)) {
		smc_wakeup_broadcast();
		missed &= ~(1U << NOTIFY_DATA_ENTRY_WAKEUP);
	}
	if (missed & (1U << NOTIFY_DATA_ENTRY_FIQSHD)) {
		tc_notify_fiqshd_fn(NULL);
		missed &= ~(1U << NOTIFY_DATA_ENTRY_FIQSHD);
	}
	if (missed)
		tloge("missed spi notification mask %x\n", missed);
}

static void tc_notify_fn(struct work_struct *dummy)
{
	struct notify_data_entry copy = {0};

	while (get_notify_data_entry(&copy) == 0) {
		switch (copy.entry_type) {
		case NOTIFY_DATA_ENTRY_TIMER:
		case NOTIFY_DATA_ENTRY_RTC:
			tc_notify_timer_fn(&copy);
			break;
		case NOTIFY_DATA_ENTRY_WAKEUP:
			tc_notify_wakeup_fn(&copy);
			break;
		case NOTIFY_DATA_ENTRY_SHADOW:
			tc_notify_shadow_fn(&copy);
			break;
		case NOTIFY_DATA_ENTRY_FIQSHD:
			tc_notify_fiqshd_fn(&copy);
			break;
		case NOTIFY_DATA_ENTRY_SHADOW_EXIT:
			tc_notify_shadowexit_fn(&copy);
			break;
#ifdef CONFIG_TA_AFFINITY
		case NOTIFY_DATA_ENTRY_SET_AFFINITY:
			tc_notify_set_affinity(&copy);
			break;
#endif
		default:
			tloge("invalid entry type = %u\n", copy.entry_type);
		}
		if (memset_s(&copy, sizeof(copy), 0, sizeof(copy)))
			tloge("memset copy failed\n");
	}
	spi_broadcast_notifications();
}

static irqreturn_t tc_secure_notify(int irq, void *dev_id)
{
#define N_WORK  8
	int i;
	static struct work_struct tc_notify_works[N_WORK];
	static int init;

	if (!init) {
		for (i = 0; i < N_WORK; i++)
			INIT_WORK(&tc_notify_works[i], tc_notify_fn);
		init = 1;
	}
	for (i = 0; i < N_WORK; i++) {
		if (queue_work(g_tz_spi_wq, &tc_notify_works[i]))
			break;
	}
#undef N_WORK

	return IRQ_HANDLED;
}

int tc_ns_register_service_call_back_func(const char *uuid, void *func,
	const void *private_data)
{
	struct tc_ns_callback *callback_func = NULL;
	struct tc_ns_callback *new_callback = NULL;
	int ret = 0;

	if (!uuid || !func)
		return -EINVAL;

	(void)private_data;
	mutex_lock(&g_ta_callback_func_list.callback_list_lock);
	if (g_ta_callback_func_list.callback_count > MAX_CALLBACK_COUNT) {
		mutex_unlock(&g_ta_callback_func_list.callback_list_lock);
		tloge("callback_count is out\n");
		return -ENOMEM;
	}
	list_for_each_entry(callback_func,
		&g_ta_callback_func_list.callback_list, head) {
		if (!memcmp(callback_func->uuid, uuid, UUID_SIZE)) {
			callback_func->callback_func = (void (*)(void *))func;
			tlogd("succeed to find uuid ta_callback_func_list\n");
			goto find_callback;
		}
	}
	/* create a new callback struct if we couldn't find it in list */
	new_callback = kzalloc(sizeof(*new_callback), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)new_callback)) {
		tloge("kzalloc failed\n");
		ret = -ENOMEM;
		goto find_callback;
	}

	if (memcpy_s(new_callback->uuid, UUID_SIZE, uuid, UUID_SIZE)) {
		kfree(new_callback);
		new_callback = NULL;
		ret = -ENOMEM;
		goto find_callback;
	}
	g_ta_callback_func_list.callback_count++;
	tlogd("callback count is %u\n",
		g_ta_callback_func_list.callback_count);
	INIT_LIST_HEAD(&new_callback->head);
	new_callback->callback_func = (void (*)(void *))func;
	mutex_init(&new_callback->callback_lock);
	list_add_tail(&new_callback->head,
		&g_ta_callback_func_list.callback_list);
find_callback:
	mutex_unlock(&g_ta_callback_func_list.callback_list_lock);
	return ret;
}

int TC_NS_RegisterServiceCallbackFunc(const char *uuid, void *func,
	const void *private_data)
{
	const char *uuid_in = uuid;
	return tc_ns_register_service_call_back_func(uuid_in,
		func, private_data);
}
EXPORT_SYMBOL(TC_NS_RegisterServiceCallbackFunc);

#ifdef DEF_ENG

static void timer_callback_func(void *param)
{
	struct teec_timer_property *timer_property =
		(struct teec_timer_property *)param;
	tlogd("timer property type = %x, timer property timer id = %x\n",
		timer_property->type, timer_property->timer_id);
	g_timer_type = (int)timer_property->type;
}

static void tst_get_timer_type(int *type)
{
	*type = g_timer_type;
}

static void callback_demo_main(const char *uuid)
{
	int ret;

	tlogd("step into callback_demo_main\n");
	ret = tc_ns_register_service_call_back_func(uuid,
		timer_callback_func, NULL);
	if (ret)
		tloge("failed to tc_ns_register_service_call_back_func\n");
}

static int init_tst_test_context(struct tc_ns_client_context *client_context,
	const void *argp, int *cmd_id)
{
	struct tc_uuid secure_timer_uuid = {
		0x19b39980, 0x2487, 0x7b84,
		{0xf4, 0x1a, 0xbc, 0x89, 0x22, 0x62, 0xbb, 0x3d}
	};

	if (copy_from_user(client_context, argp, sizeof(*client_context))) {
		tloge("copy from user failed\n");
		return -ENOMEM;
	}

	if (!tc_user_param_valid(client_context, 0)) {
		tloge("param 0 is invalid\n");
		return -EFAULT;
	}

	/* a_addr contain the command id */
	if (copy_from_user(cmd_id,
		(void *)(uintptr_t)client_context->params[0].value.a_addr,
		sizeof(*cmd_id))) {
		tloge("copy from user failed:cmd_id\n");
		return -ENOMEM;
	}

	if (memcmp((char *)client_context->uuid, (char *)&secure_timer_uuid,
		sizeof(struct tc_uuid))) {
		tloge("request not from secure_timer\n");
		tloge("request uuid: %x %x %x %x\n",
			*(client_context->uuid + 0),
			*(client_context->uuid + 1),
			*(client_context->uuid + 2),
			*(client_context->uuid + 3));
		/* just wanna print the first four characters of uuid */
		return -EACCES;
	}

	return 0;
}

int tc_ns_tst_cmd(void *argp)
{
	struct tc_ns_client_context client_context;
	int ret;
	int cmd_id;
	int timer_type;

	if (!argp) {
		tloge("argp is NULL input buffer\n");
		return -EINVAL;
	}

	ret = init_tst_test_context(&client_context, argp, &cmd_id);
	if (ret)
		return ret;

	switch (cmd_id) {
	case TST_CMD_01:
		callback_demo_main((char *)client_context.uuid);
		break;
	case TST_CMD_02:
		tst_get_timer_type(&timer_type);
		if (!tc_user_param_valid(&client_context, (unsigned int)1)) {
			tloge("param 1 is invalid\n");
			ret = -EINVAL;
			return ret;
		}
		if (copy_to_user(
			(void *)(uintptr_t)
			client_context.params[1].value.a_addr,
			&timer_type, sizeof(timer_type))) {
			tloge("copy to user failed:timer_type\n");
			ret = -ENOMEM;
			return ret;
		}
		break;
	default:
		ret = -EINVAL;
		return ret;
	}
	if (copy_to_user(argp, &client_context, sizeof(client_context))) {
		tloge("copy to user failed:client context\n");
		ret = -ENOMEM;
		return ret;
	}
	return ret;
}

#else

int tc_ns_tst_cmd(void *argp)
{
	(void)argp;
	tloge("usr img do not support this cmd\n");
	return 0;
}

#endif

static int send_notify_cmd(unsigned int cmd_id)
{
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	int ret = 0;
	struct mb_cmd_pack *mb_pack = NULL;

	mb_pack = mailbox_alloc_cmd_pack();
	if (!mb_pack)
		return -ENOMEM;

	mb_pack->operation.paramtypes =
		TEE_PARAM_TYPE_VALUE_INPUT |
		TEE_PARAM_TYPE_VALUE_INPUT << TEE_PARAM_NUM;
	mb_pack->operation.params[0].value.a =
		virt_to_phys(g_notify_data);
	mb_pack->operation.params[0].value.b = (uint64_t)virt_to_phys(g_notify_data) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[1].value.a = SZ_4K;
	smc_cmd.cmd_type = CMD_TYPE_GLOBAL;
	smc_cmd.cmd_id = cmd_id;
	smc_cmd.operation_phys =
		virt_to_phys(&mb_pack->operation);
	smc_cmd.operation_h_phys = (uint64_t)virt_to_phys(&mb_pack->operation) >> ADDR_TRANS_NUM;

	if (tc_ns_smc(&smc_cmd)) {
		ret = -EPERM;
		tloge("register notify mem failed\n");
	}
	mailbox_free(mb_pack);

	return ret;
}

static int config_spi_context(struct device *class_dev, struct device_node *np)
{
	unsigned int irq = DEFAULT_SPI_NUM;
	int ret;

#ifndef CONFIG_ACPI
	if (!np) {
		tloge("device node not found\n");
		return -EINVAL;
	}
#endif

	/* Map IRQ 0 from the OF interrupts list */
#ifdef CONFIG_ACPI
	irq = (unsigned int)get_acpi_tz_irq();
#else
	irq = irq_of_parse_and_map(np, 0);
#endif
	ret = devm_request_irq(class_dev, irq, tc_secure_notify,
		IRQF_NO_SUSPEND, TC_NS_CLIENT_DEV, NULL);
	if (ret < 0) {
		tloge("device irq %u request failed %d", irq, ret);
		return ret;
	}

	g_ta_callback_func_list.callback_count = 0;
	INIT_LIST_HEAD(&g_ta_callback_func_list.callback_list);
	mutex_init(&g_ta_callback_func_list.callback_list_lock);

	return 0;
}

int tz_spi_init(struct device *class_dev, struct device_node *np)
{
	int ret;

	if (!class_dev) /* here np can be NULL */
		return -EINVAL;

	spin_lock_init(&g_notify_lock);
	g_tz_spi_wq = alloc_workqueue("g_tz_spi_wq",
		WQ_UNBOUND | WQ_HIGHPRI, TZ_WQ_MAX_ACTIVE);
	if (!g_tz_spi_wq) {
		tloge("it failed to create workqueue g_tz_spi_wq\n");
		return -ENOMEM;
	}
	tz_workqueue_bind_mask(g_tz_spi_wq, WQ_HIGHPRI);

	ret = config_spi_context(class_dev, np);
	if (ret)
		goto clean;

	if (!g_notify_data) {
#ifdef CONFIG_BIG_SESSION
		/* we should map at least 3 pages for 100 sessions, 2^2 > 3 */
		g_notify_data = (struct notify_data_struct *)
			(uintptr_t)__get_free_pages(
			GFP_KERNEL | __GFP_ZERO, CONFIG_NOTIFY_PAGE_ORDER);
#else
		g_notify_data = (struct notify_data_struct *)
			(uintptr_t)__get_free_page(GFP_KERNEL | __GFP_ZERO);
#endif
		if (!g_notify_data) {
			tloge("get free page failed for notification data\n");
			ret = -ENOMEM;
			goto clean;
		}

		ret = send_notify_cmd(GLOBAL_CMD_ID_REGISTER_NOTIFY_MEMORY);
		if (ret) {
			tloge("shared memory failed ret is 0x%x\n", ret);
			ret = -EFAULT;
			free_page((unsigned long)(uintptr_t)g_notify_data);
			g_notify_data = NULL;
			goto clean;
		}

		g_notify_data_entry_shadow =
			&g_notify_data->entry[NOTIFY_DATA_ENTRY_SHADOW - 1];
		tlogd("target is: %llx\n",
		      g_notify_data_entry_shadow->context.shadow.target_tcb);
	}

	return 0;
clean:
	tz_spi_exit();
	return ret;
}

void tz_spi_exit(void)
{
	if (g_notify_data) {
		if (send_notify_cmd(GLOBAL_CMD_ID_UNREGISTER_NOTIFY_MEMORY))
			tloge("unregister notify data mem failed\n");
#ifdef CONFIG_BIG_SESSION
		free_pages((unsigned long)(uintptr_t)g_notify_data,
			CONFIG_NOTIFY_PAGE_ORDER);
#else
		free_page((unsigned long)(uintptr_t)g_notify_data);
#endif
		g_notify_data = NULL;
	}

	if (g_tz_spi_wq) {
		flush_workqueue(g_tz_spi_wq);
		destroy_workqueue(g_tz_spi_wq);
		g_tz_spi_wq = NULL;
	}
}
