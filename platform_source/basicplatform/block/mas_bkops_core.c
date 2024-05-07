/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: mas bkops core framework
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "[BKOPS]" fmt

#include <linux/bio.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/gfp.h>
#include <linux/mas_bkops_core.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/namei.h>
#include <linux/proc_fs.h>
#include <linux/reboot.h>
#include <linux/statfs.h>
#include <linux/suspend.h>
#ifdef CONFIG_MAS_UNISTORE_PRESERVE
#include <trace/iotrace.h>
#include "mas_blk.h"
#endif
#include "blk.h"
#include "mas_bkops_core_interface.h"

#define MAS_BKOPS_MODULE_NAME "mas_bkops"

#define bkops_ns_to_ms(ns) ((ns) / NSEC_PER_MSEC)

static struct workqueue_struct *bkops_workqueue_g;
static int mas_bkops_inited;
static char user_manual_gc;
static DEFINE_MUTEX(bkops_lock);
static bool bkops_enabled = true;

#ifdef CONFIG_MAS_DEBUG_FS
void mas_bkops_update_dur(struct bkops_stats *bkops_stats_p)
{
	u64 bkops_duration;
	u64 bkops_dur_msecs;

	if (!bkops_stats_p)
		return;

	bkops_duration = ktime_get_ns() - bkops_stats_p->bkops_start_time;
	if (bkops_duration > bkops_stats_p->max_bkops_duration)
		bkops_stats_p->max_bkops_duration = bkops_duration;

	bkops_dur_msecs = bkops_ns_to_ms(bkops_duration);
	if (bkops_dur_msecs < BKOPS_DUR_100MS)
		bkops_stats_p->bkops_dur[BKOPS_DUR_IDX_100MS]++;
	else if (bkops_dur_msecs < BKOPS_DUR_500MS)
		bkops_stats_p->bkops_dur[BKOPS_DUR_IDX_500MS]++;
	else if (bkops_dur_msecs < BKOPS_DUR_1000MS)
		bkops_stats_p->bkops_dur[BKOPS_DUR_IDX_1000MS]++;
	else if (bkops_dur_msecs < BKOPS_DUR_2000MS)
		bkops_stats_p->bkops_dur[BKOPS_DUR_IDX_2000MS]++;
	else if (bkops_dur_msecs < BKOPS_DUR_5000MS)
		bkops_stats_p->bkops_dur[BKOPS_DUR_IDX_5000MS]++;
	else
		bkops_stats_p->bkops_dur[BKOPS_DUR_IDX_FOR_AGES]++;

	if (!bkops_stats_p->bkops_stop_count)
		return;

	bkops_stats_p->bkops_avrg_exe_time =
		((bkops_stats_p->bkops_avrg_exe_time *
			(bkops_stats_p->bkops_stop_count - 1)) +
			bkops_duration) / bkops_stats_p->bkops_stop_count;
}

void mas_bkops_set_status_str(
	struct mas_bkops *bkops, u32 bkops_status_max,
	const char **bkops_status_str)
{
	if (!bkops || !bkops_status_str)
		return;

	bkops->bkops_stats.bkops_status_max = bkops_status_max;
	bkops->bkops_stats.bkops_status_str = bkops_status_str;
}
#endif /* CONFIG_MAS_DEBUG_FS */

static int mas_bkops_query(
	const struct mas_bkops *bkops, const u32 *bkops_status)
{
	int ret = 0;
	struct bkops_ops *bkops_ops_p = bkops->bkops_ops;
#ifdef CONFIG_MAS_DEBUG_FS
	u64 start_time, time_interval;
	struct bkops_stats *bkops_stats_p =
		(struct bkops_stats *)(&(bkops->bkops_stats));
#endif

	mutex_lock(&bkops_lock);
	if (!bkops_enabled)
		goto out;

#ifdef CONFIG_MAS_DEBUG_FS
	if (bkops->bkops_debug_ops.sim_bkops_query_delay) {
		pr_err("%s %d simulate bkops query delay %u ms\n", __func__,
			__LINE__, bkops->bkops_debug_ops.sim_bkops_query_delay);
		msleep(bkops->bkops_debug_ops.sim_bkops_query_delay);
	}

	start_time = ktime_get_ns();
#endif
	ret = bkops_ops_p->bkops_status_query(
		bkops->bkops_data, (u32 *)bkops_status);
	if (user_manual_gc && !(*bkops_status))
		user_manual_gc = 0;
#ifdef CONFIG_MAS_DEBUG_FS
	bkops_stats_p->bkops_actual_query_count++;
	if (!ret && *bkops_status < bkops_stats_p->bkops_status_max)
		bkops_stats_p->bkops_status[*bkops_status]++;

	time_interval = ktime_get_ns() - start_time;
	if (time_interval > bkops_stats_p->bkops_max_query_time)
		bkops_stats_p->bkops_max_query_time = time_interval;

	bkops_stats_p->bkops_avrg_query_time =
		((bkops_stats_p->bkops_avrg_query_time *
			 (bkops_stats_p->bkops_actual_query_count - 1)) +
			time_interval) /
		bkops_stats_p->bkops_actual_query_count;
#endif

out:
	mutex_unlock(&bkops_lock);
	return ret;
}

static int mas_bkops_start(const struct mas_bkops *bkops)
{
	int ret = 0;
	struct bkops_ops *bkops_ops_p = bkops->bkops_ops;
#ifdef CONFIG_MAS_DEBUG_FS
	static DEFINE_RATELIMIT_STATE(bkops_start_rs, (60 * HZ), 1);
	u64 start_time, stop_time, time_interval;
	struct bkops_stats *bkops_stats_p =
		(struct bkops_stats *)(&(bkops->bkops_stats));

	start_time = ktime_get_ns();
#endif

	mutex_lock(&bkops_lock);
	if (!bkops_enabled)
		goto out;

	ret = bkops_ops_p->bkops_start_stop(bkops->bkops_data, BKOPS_START);
#ifdef CONFIG_MAS_DEBUG_FS
	bkops_stats_p->bkops_start_count++;
	if (__ratelimit(&bkops_start_rs))
		pr_info("bkops_start_count: %u\n",
			bkops_stats_p->bkops_start_count);
	stop_time = ktime_get_ns();
	bkops_stats_p->bkops_start_time = stop_time;
	time_interval = stop_time - start_time;
	if (time_interval > bkops_stats_p->bkops_max_start_time)
		bkops_stats_p->bkops_max_start_time = time_interval;

	bkops_stats_p->bkops_avrg_start_time =
		((bkops_stats_p->bkops_avrg_start_time *
			 (bkops_stats_p->bkops_start_count - 1)) +
			time_interval) / bkops_stats_p->bkops_start_count;
#endif

out:
	mutex_unlock(&bkops_lock);
	return ret;
}

/*
 * BKOPS may be stopped by device driver directly(IOCTL or eMMC RPMB
 * access),so the statistic was moved into driver code accordingly.
 */
static void mas_bkops_stop(struct mas_bkops *bkops)
{
#ifdef CONFIG_MAS_DEBUG_FS
	if (bkops->bkops_debug_ops.skip_bkops_stop)
		return;
#endif

	mutex_lock(&bkops_lock);
	if (!bkops_enabled)
		goto out;

	bkops->bkops_ops->bkops_start_stop(bkops->bkops_data, BKOPS_STOP);
#ifdef CONFIG_MAS_DEBUG_FS
	bkops->bkops_stats.bkops_core_stop_count++;
#endif

out:
	mutex_unlock(&bkops_lock);
}

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
static void bkops_fs_work_end_states(
	struct mas_bkops *bkops, const u32 *bkops_status)
{
	if (blk_queue_query_unistore_enable(bkops->q) &&
		(bkops->dev_type == BKOPS_DEV_UFS_1861) && !(*bkops_status))
		clear_bit(BKOPS_FS_STARTED, &bkops->bkops_flag);
}
#endif

void bkops_idle_work_func(const struct work_struct *work)
{
	u32 bkops_status = 0;
	struct mas_bkops *bkops = container_of((struct delayed_work *)work,
		struct mas_bkops, bkops_idle_work);
	int ret;

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
	if (blk_queue_query_unistore_enable(bkops->q) &&
		test_and_clear_bit(BKOPS_CHK_STOP_STATE, &bkops->bkops_flag))
		return;
#endif

	ret = mas_bkops_query(bkops, &bkops_status);
	if (ret) {
		pr_err("mas_bkops_query failed!\n");
		return;
	}
	bkops->bkops_flag &= ~BKOPS_QUERY_NEEDED;

	/* record bkops status */
	bkops->bkops_status = bkops_status;

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
	bkops_fs_work_end_states(bkops, &bkops_status);
#endif

	if (bkops_status && !test_bit(BKOPS_START_STATUS, &bkops->bkops_flag)) {
		ret = mas_bkops_start(bkops);
		if (ret)
			pr_err("mas_bkops_start failed!\n");
		else
			set_bit(BKOPS_START_STATUS, &bkops->bkops_flag);
	}
}

static int bkops_strategy(struct mas_bkops *bkops)
{
	struct blk_dev_lld *lld = mas_blk_get_lld(bkops->q);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	struct timespec64 tp;
#else
	struct timespec tp;
#endif

	if (bkops->bkops_flag & BKOPS_CHK_TIME_INTERVAL) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
		ktime_get_boottime_ts64(&tp);
#else
		get_monotonic_boottime(&tp);
#endif
		if (unlikely(!bkops->last_bkops_query_time) ||
			unlikely(tp.tv_sec - bkops->last_bkops_query_time >=
				 bkops->bkops_check_interval)) {
			bkops->last_bkops_query_time = tp.tv_sec;
#ifdef CONFIG_MAS_DEBUG_FS
			bkops->bkops_stats.bkops_time_query_count++;
#endif
			return 1;
		}
	}

	if ((bkops->bkops_flag & BKOPS_CHK_ACCU_DISCARD) &&
		((lld->discard_len - bkops->last_discard_len) >=
			bkops->bkops_check_discard_len)) {
		bkops->last_discard_len = lld->discard_len;
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_discard_query_count++;
#endif
		return 1;
	}

	if ((bkops->bkops_flag & BKOPS_CHK_ACCU_WRITE) &&
		((lld->write_len - bkops->last_write_len) >=
			bkops->bkops_check_write_len)) {
		bkops->last_write_len = lld->write_len;
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_write_query_count++;
#endif
		return 1;
	}

	return 0;
}

static int bkops_should_query(struct mas_bkops *bkops)
{
#ifdef CONFIG_MAS_DEBUG_FS
	if (bkops->bkops_debug_ops.disable_bkops)
		goto query_not_need;

	if (bkops->bkops_debug_ops.bkops_force_query)
		goto query_needed;
#endif

	if (bkops->bkops_flag & BKOPS_QUERY_NEEDED)
		goto query_needed;

	if (bkops->en_bkops_retry && bkops->bkops_status) {
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_retry_count++;
#endif
		goto query_needed;
	}

	if (bkops_strategy(bkops))
		goto query_needed;

	if (user_manual_gc)
		goto query_needed;

#ifdef CONFIG_MAS_DEBUG_FS
query_not_need:
#endif
	bkops->bkops_flag &= ~BKOPS_QUERY_NEEDED;
	return 0;

query_needed:
	bkops->bkops_flag |= BKOPS_QUERY_NEEDED;
	return 1;
}

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
static enum blk_busyidle_callback_ret bkops_idle_notify_unistore(
	struct mas_bkops *bkops)
{
	if (!blk_queue_query_unistore_enable(bkops->q) &&
		!bkops_should_query(bkops))
		return BUSYIDLE_NO_IO;

	if (blk_queue_query_unistore_enable(bkops->q)) {
		if (bkops->dev_type == BKOPS_DEV_UFS_1861) {
			set_bit(BKOPS_IDLE_STATE, &bkops->bkops_flag);
			if (!test_bit(BKOPS_FS_STARTED, &bkops->bkops_flag))
				return BUSYIDLE_NO_IO;
		}
		if ((bkops->dev_type != BKOPS_DEV_UFS_1861) && !bkops_should_query(bkops))
			return BUSYIDLE_NO_IO;
		if (test_bit(BKOPS_ASYNC_WORK_STARTED, &bkops->bkops_flag))
			return BUSYIDLE_NO_IO;
	}

	set_bit(BKOPS_ASYNC_WORK_STARTED, &bkops->bkops_flag);
	clear_bit(BKOPS_CHK_STOP_STATE, &bkops->bkops_flag);
	queue_delayed_work(bkops_workqueue_g, &bkops->bkops_idle_work,
		msecs_to_jiffies(bkops->bkops_idle_delay_ms));
	return BUSYIDLE_IO_TRIGGERED;
}

static void bkops_busy_notify_unistore(
	struct mas_bkops *bkops)
{
	if (blk_queue_query_unistore_enable(bkops->q) &&
		(bkops->dev_type == BKOPS_DEV_UFS_1861))
		clear_bit(BKOPS_IDLE_STATE, &bkops->bkops_flag);
}
#endif

enum blk_busyidle_callback_ret bkops_io_busyidle_notify_handler(
	const struct blk_busyidle_event_node *event_node,
	enum blk_idle_notify_state state)
{
	enum blk_busyidle_callback_ret ret = BUSYIDLE_NO_IO;
#ifdef CONFIG_MAS_DEBUG_FS
	bool cancel_ret = false;
#endif
	struct mas_bkops *bkops =
		(struct mas_bkops *)(event_node->param_data);

	switch (state) {
	case BLK_IDLE_NOTIFY:
#ifdef CONFIG_MAS_UNISTORE_PRESERVE
		ret = bkops_idle_notify_unistore(bkops);
#else
		if (!bkops_should_query(bkops))
			break;
		set_bit(BKOPS_ASYNC_WORK_STARTED, &bkops->bkops_flag);
		queue_delayed_work(bkops_workqueue_g, &bkops->bkops_idle_work,
			msecs_to_jiffies(bkops->bkops_idle_delay_ms));
		ret = BUSYIDLE_IO_TRIGGERED;
#endif
		break;
	case BLK_BUSY_NOTIFY:
#ifdef CONFIG_MAS_UNISTORE_PRESERVE
		bkops_busy_notify_unistore(bkops);
#endif
		if (!test_and_clear_bit(
			    BKOPS_ASYNC_WORK_STARTED, &bkops->bkops_flag))
			break;

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
		clear_bit(BKOPS_CHK_STOP_STATE, &bkops->bkops_flag);
#endif
#ifndef CONFIG_MAS_DEBUG_FS
		cancel_delayed_work_sync(&bkops->bkops_idle_work);
#else
		cancel_ret = cancel_delayed_work_sync(&bkops->bkops_idle_work);
		if (cancel_ret)
			bkops->bkops_stats.bkops_idle_work_canceled_count++;
		else
			bkops->bkops_stats.bkops_idle_work_waited_count++;
#endif
		if (test_bit(BKOPS_START_STATUS, &bkops->bkops_flag)) {
			mas_bkops_stop(bkops);
			clear_bit(BKOPS_START_STATUS, &bkops->bkops_flag);
		}
		ret = BUSYIDLE_NO_IO;
		break;
#ifdef CONFIG_MAS_UNISTORE_PRESERVE
	case BLK_FG_BUSY_NOTIFY:
	case BLK_FG_IDLE_NOTIFY:
		ret = BUSYIDLE_NO_IO;
		break;
#endif
	default:
		pr_err_ratelimited("%s unknown state: %d\n", __func__, state);
		ret = BUSYIDLE_NO_IO;
	}

	return ret;
}

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
static int mas_get_bkops(struct block_device *bdev, struct mas_bkops **bkops)
{
	struct blk_dev_lld *lld = NULL;

	if (unlikely(!bdev))
		return BKOPS_INPUT_ERR;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	lld = mas_blk_get_lld(bdev->bd_disk->queue);
#else
	lld = mas_blk_get_lld(bdev->bd_queue);
#endif
	if (!lld)
		return BKOPS_INPUT_ERR;

	*bkops = lld->bkops;

	if (!(lld->features & BLK_LLD_UFS_UNISTORE_EN))
		return BKOPS_DEV_NOT_SUPPORT;

	if (unlikely(!(*bkops)))
		return BKOPS_FUNC_NOT_SUPPORT;
	if ((*bkops)->dev_type != BKOPS_DEV_UFS_1861)
		return BKOPS_DEV_NOT_SUPPORT;

	return 0;
}

static void mas_bkops_work_cancel(struct mas_bkops *bkops)
{
	if (test_and_clear_bit(BKOPS_ASYNC_WORK_STARTED, &bkops->bkops_flag)) {
		clear_bit(BKOPS_CHK_STOP_STATE, &bkops->bkops_flag);
#ifdef CONFIG_MAS_DEBUG_FS
		if (cancel_delayed_work_sync(&bkops->bkops_idle_work))
			bkops->bkops_stats.bkops_idle_work_canceled_count++;
		else
			bkops->bkops_stats.bkops_idle_work_waited_count++;
#else
		cancel_delayed_work_sync(&bkops->bkops_idle_work);
#endif
	}
}

int mas_bkops_work_query(struct block_device *bdev)
{
	struct mas_bkops *bkops = NULL;
	u32 bkops_status = 0;
	int ret;

	io_trace_unistore_count(UNISTORE_BKOPS_FS_QUERY_CNT, 1);

	ret = mas_get_bkops(bdev, &bkops);
	if (ret)
		return ret;

#ifdef CONFIG_MAS_DEBUG_FS
	bkops->bkops_stats.bkops_fs_work_query_count++;
#endif
	if (!test_bit(BKOPS_IDLE_STATE, &bkops->bkops_flag)) {
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_query_not_idle_count++;
#endif
		return BKOPS_STATE_NOT_IDLE;
	}

	if (test_bit(BKOPS_START_STATUS, &bkops->bkops_flag)) {
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_query_already_start_count++;
#endif
		return BKOPS_ALREADY_START;
	}

	mas_bkops_work_cancel(bkops);
	ret = mas_bkops_query(bkops, &bkops_status);
	if (ret) {
		pr_err("%s: mas_bkops_query failed, ret = %d!\n", __func__, ret);
		return BKOPS_QUERY_ERR;
	}

	if (!bkops_status) {
		clear_bit(BKOPS_FS_STARTED, &bkops->bkops_flag);
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_end_count++;
		bkops->bkops_stats.bkops_fs_work_query_no_need_count++;
#endif
		return BKOPS_NO_NEED_START;
	}

	return BKOPS_NEED_START;
}

static int mas_bkops_work_start_ret(struct mas_bkops *bkops, int ret)
{
	if (ret) {
		pr_err("%s: mas_bkops_start failed, ret = %d!\n", __func__, ret);
		return BKOPS_START_ERR;
	} else {
		set_bit(BKOPS_FS_STARTED, &bkops->bkops_flag);
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_start_idle_count++;
#endif
		set_bit(BKOPS_START_STATUS, &bkops->bkops_flag);
	}

	return BKOPS_START_SUC;
}

int mas_bkops_work_start(struct block_device *bdev)
{
	struct mas_bkops *bkops = NULL;
	u32 bkops_status = 0;
	int ret;

	io_trace_unistore_count(UNISTORE_BKOPS_FS_START_CNT, 1);

	ret = mas_get_bkops(bdev, &bkops);
	if (ret)
		return ret;

	if (!test_bit(BKOPS_IDLE_STATE, &bkops->bkops_flag)) {
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_start_not_idle_count++;
#endif
		return BKOPS_STATE_NOT_IDLE;
	}

	if (test_bit(BKOPS_START_STATUS, &bkops->bkops_flag)) {
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_start_already_start_count++;
#endif
		return BKOPS_ALREADY_START;
	}

	mas_bkops_work_cancel(bkops);
	ret = mas_bkops_query(bkops, &bkops_status);
	if (ret) {
		pr_err("%s: mas_bkops_query failed, ret = %d!\n", __func__, ret);
		return BKOPS_QUERY_ERR;
	}

	if (!bkops_status) {
		clear_bit(BKOPS_FS_STARTED, &bkops->bkops_flag);
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_end_count++;
		bkops->bkops_stats.bkops_fs_work_start_no_need_count++;
#endif
		return BKOPS_NO_NEED_START;
	}

	ret = mas_bkops_start(bkops);
	return mas_bkops_work_start_ret(bkops, ret);
}

void mas_bkops_work_stop(struct block_device *bdev)
{
	struct mas_bkops *bkops = NULL;
	int ret;

	io_trace_unistore_count(UNISTORE_BKOPS_FS_STOP_CNT, 1);

	ret = mas_get_bkops(bdev, &bkops);
	if (ret)
		return;

	if (!test_and_clear_bit(BKOPS_FS_STARTED, &bkops->bkops_flag)) {
#ifdef CONFIG_MAS_DEBUG_FS
		bkops->bkops_stats.bkops_fs_work_stop_not_start_count++;
#endif
		return;
	}
#ifdef CONFIG_MAS_DEBUG_FS
	bkops->bkops_stats.bkops_fs_work_stop_count++;
#endif
	if (test_and_clear_bit(
		    BKOPS_ASYNC_WORK_STARTED, &bkops->bkops_flag))
		set_bit(BKOPS_CHK_STOP_STATE, &bkops->bkops_flag);

	if (test_bit(BKOPS_START_STATUS, &bkops->bkops_flag)) {
		mas_bkops_stop(bkops);
		clear_bit(BKOPS_START_STATUS, &bkops->bkops_flag);
	}
}
#endif /* CONFIG_MAS_UNISTORE_PRESERVE */

static inline void bkops_disable(void)
{
	mutex_lock(&bkops_lock);
	bkops_enabled = false;
	mutex_unlock(&bkops_lock);
}

static inline void bkops_enable(void)
{
	mutex_lock(&bkops_lock);
	bkops_enabled = true;
	mutex_unlock(&bkops_lock);
}

int bkops_pm_callback(
	const struct notifier_block *self, unsigned long action,
	const void *ptr)
{
	switch (action) {
	case PM_SUSPEND_PREPARE:
	case PM_HIBERNATION_PREPARE:
		bkops_disable();
		break;

	case PM_POST_SUSPEND:
	case PM_POST_HIBERNATION:
		bkops_enable();
		break;

	default:
		return NOTIFY_DONE;
	}

	return NOTIFY_OK;
}

int bkops_notify_reboot(
	const struct notifier_block *self, unsigned long code,
	const void *no_use)
{
	bkops_disable();
	return NOTIFY_DONE;
}

static struct notifier_block bkops_reboot_notifier = {
	.notifier_call = __cfi_bkops_notify_reboot,
	.next = NULL,
	.priority = INT_MAX, /* before any real devices */
};

static struct notifier_block bkops_pm_notifier = {
	.notifier_call = __cfi_bkops_pm_callback,
	.next = NULL,
	.priority = 0,
};

static int mas_bkops_init(void)
{
	if (mas_bkops_inited)
		return 0;

	bkops_workqueue_g = alloc_workqueue(
		"bkops_workqueue", WQ_MEM_RECLAIM | WQ_HIGHPRI, 0);
	if (bkops_workqueue_g) {
		register_pm_notifier(&bkops_pm_notifier);
		register_reboot_notifier(&bkops_reboot_notifier);
		mas_bkops_inited = 1;
		return 0;
	}

	return -EINVAL;
}

static void mas_bkops_exit(void)
{
	if (!bkops_workqueue_g)
		return;

	mas_bkops_inited = 0;
	unregister_reboot_notifier(&bkops_reboot_notifier);
	unregister_pm_notifier(&bkops_pm_notifier);
	destroy_workqueue(bkops_workqueue_g);
	bkops_workqueue_g = NULL;
}

struct mas_bkops *mas_bkops_alloc(void)
{
	struct mas_bkops *bkops =
		kzalloc(sizeof(struct mas_bkops), GFP_KERNEL);

	if (!bkops) {
		pr_err("Failed to alloc bkops_func\n");
		return NULL;
	}

	INIT_DELAYED_WORK(&bkops->bkops_idle_work, __ufs_bkops_idle_work_func);
	bkops->bkops_idle_delay_ms = BKOPS_DEF_IDLE_DELAY;
	bkops->bkops_check_interval = BKOPS_DEF_CHECK_INTERVAL;
	bkops->bkops_check_discard_len = BKOPS_DEF_DISCARD_LEN;
	bkops->bkops_check_write_len = BKOPS_DEF_WRITE_LEN;

	return bkops;
}

int mas_bkops_manual_gc_proc_show(struct seq_file *m, const void *v)
{
	seq_printf(m, "%d\n", user_manual_gc);

	return 0;
}

int mas_bkops_manual_gc_proc_open(
	const struct inode *p_inode, const struct file *p_file)
{
	return single_open((struct file *)p_file,
		__cfi_mas_bkops_manual_gc_proc_show, NULL);
}

ssize_t mas_bkops_manual_gc_proc_write(const struct file *p_file,
	const char __user *userbuf, size_t count, const loff_t *ppos)
{
	char buf;

	if (!count || !userbuf)
		return -EINVAL;

	if (copy_from_user(&buf, userbuf, sizeof(char)))
		return -EFAULT;

	if (buf != '0' && buf != '1') {
		pr_err("%s %d: input error.\n", __func__, __LINE__);
		return -EINVAL;
	}

	user_manual_gc = buf - '0';

	return (ssize_t)count;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
static const struct proc_ops mas_bkops_manual_gc_fops = {
	.proc_open = __cfi_mas_bkops_manual_gc_proc_open,
	.proc_read = seq_read,
	.proc_write = __cfi_mas_bkops_manual_gc_proc_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};
#else
static const struct file_operations mas_bkops_manual_gc_fops = {
	.open = __cfi_mas_bkops_manual_gc_proc_open,
	.read = seq_read,
	.write = __cfi_mas_bkops_manual_gc_proc_write,
	.llseek = seq_lseek,
	.release = single_release,
};
#endif

int mas_bkops_status_proc_show(struct seq_file *m, const void *v)
{
	u32 status = 0;
	int ret;

	ret = mas_bkops_query(m->private, &status);
	if (ret) {
		pr_err("%s: mas_bkops_query failed, ret = %d!\n", __func__, ret);
		return ret;
	}

	seq_printf(m, "%u\n", status);

	return 0;
}

int mas_bkops_status_proc_open(
	const struct inode *p_inode, const struct file *p_file)
{
	return single_open((struct file *)p_file,
		__cfi_mas_bkops_status_proc_show, PDE_DATA(p_inode));
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
static const struct proc_ops mas_bkops_status_fops = {
	.proc_open = __cfi_mas_bkops_status_proc_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};
#else
static const struct file_operations mas_bkops_status_fops = {
	.open = __cfi_mas_bkops_status_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};
#endif

static void mas_bkops_manual_gc_proc_init(const struct mas_bkops *bkops)
{
	struct proc_dir_entry *dir = NULL;

	dir = proc_mkdir("block", NULL);
	if (!dir) {
		pr_err("%s %d: proc mk error.\n", __func__, __LINE__);
		return;
	}

	proc_create("manual_gc", 0440 | 0220, dir, &mas_bkops_manual_gc_fops);
	proc_create_data(
		"status", 0440, dir, &mas_bkops_status_fops, (void *)bkops);
}

int mas_bkops_enable(
	struct request_queue *q, struct mas_bkops *bkops, struct dentry *pdir)
{
	struct blk_busyidle_event_node *event_node = NULL;
	struct blk_dev_lld *blk_lld = NULL;

	if (!q || !bkops || !pdir)
		return -EINVAL;

	event_node = &(bkops->busyidle_event_node);
	blk_lld = mas_blk_get_lld(q);
	if (unlikely(!(blk_lld->features & BLK_LLD_BUSYIDLE_SUPPORT))) {
		pr_err("Busy Idle was not enabled!\n");
		return -EINVAL;
	}

	if (bkops->dev_type == BKOPS_DEV_NONE ||
		bkops->dev_type >= BKOPS_DEV_TYPE_MAX) {
		pr_err("bkops dev type not specified!\n");
		return -EINVAL;
	}

	if (!bkops->bkops_ops) {
		pr_err("bkops_ops is NULL!\n");
		return -EINVAL;
	}

	if (mas_bkops_init()) {
		pr_err("%s %d mas_bkops_init failed!\n", __func__,
			__LINE__);
		return -EINVAL;
	}

	if (mas_bkops_add_debugfs(bkops, pdir))
		pr_err("Failed to create mmc bkops debugfs!\n");

	bkops->q = q;
	strncpy(event_node->subscriber, MAS_BKOPS_MODULE_NAME,
		SUBSCRIBER_NAME_LEN);
	event_node->subscriber[SUBSCRIBER_NAME_LEN - 1] = '\0';
	event_node->busyidle_callback = __cfi_bkops_io_busyidle_notify_handler;
	event_node->param_data = bkops;
	if (blk_lld_busyidle_event_subscribe(blk_lld, event_node))
		goto bkops_exit;

	mas_bkops_manual_gc_proc_init(bkops);

	return 0;

bkops_exit:
	mas_bkops_exit();
	return -EINVAL;
}
