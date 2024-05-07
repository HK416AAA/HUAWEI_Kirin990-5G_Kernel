/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/reboot.h>
#include <linux/delay.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
#include <linux/platform_drivers/hisi_rproc.h>
#else
#include <linux/hisi/hisi_rproc.h>
#endif
#include <linux/hisi/hifidrvinterface.h>
#include <product_config.h>
#include <adrv.h>
#include <acore_nv_stru_drv.h>
#include <mdrv_vcom_agent.h>
#include <linux/gpio.h>
#include <linux/gpio-fan.h>
#include <linux/delay.h>
#include <bsp_dt.h>
#include <bsp_pcie.h>
#include <bsp_fiq.h>
#include <bsp_ipc.h>
#include <bsp_dump.h>
#include <bsp_dsp.h>
#include <bsp_hardtimer.h>
#include <bsp_reset.h>
#include <bsp_llt_gcov.h>
#include <bsp_power_state.h>
#include "reset_balong.h"
#include <mdrv_errno.h>
#include <securec.h>
#include <modem_boot.h>
#include "bsp_pmu_gpio.h"

/*lint --e{746, 732, 516, 958, 666} */
/* To make modem poweroff called only once when there are two rilds. */
static int g_modem_reset_power_off_flag = 0;
static int g_modem_reset_power_on_flag = 0;
spinlock_t g_modem_reset_power_spinlock;

static modem_reset_e g_modem_reset_fail_id = MODEM_RESET_DRV_ERR;
static unsigned int g_scbakdata13_offset = 0;
struct modem_reset_ctrl g_global_reset_ctrl = { 0 };
struct modem_reset_debug g_reset_debug = { 0 };
u32 g_modem_reset_gpio = 221; /* MODEM_POWERON */

#define reset_print_debug(fmt, ...)              \
    do {                                         \
        if (g_reset_debug.print_sw)              \
            reset_print_err(fmt, ##__VA_ARGS__); \
    } while (0)


/* 打点时间戳 */
static inline void cp_reset_timestamp_dump(enum RESET_DUMP_MOD_ID reset_dumpid)
{
    if (g_reset_debug.dump_state == (u32)RESET_DUMP_MAGIC)
        g_reset_debug.dump_info.entry_tbl[reset_dumpid] = bsp_get_slice_value();
}

uintptr_t get_scbakdata13(void)
{
    return (uintptr_t)(&g_scbakdata13_offset);
}

u32 modem_reset_fail_id_get(void)
{
    return (u32)g_modem_reset_fail_id;
}

void reset_notify_app(drv_reset_cb_moment_e stage)
{
    if ((stage == MDRV_RESET_CB_BEFORE) && (g_global_reset_ctrl.reset_action_status == RESET_IS_SUCC)) {
        bsp_set_modem_state(modem_state_e, 0);
        bsp_set_modem_state(modem_reset0_e, 0);
        bsp_set_modem_state(modem_reset1_e, 0);
        bsp_set_modem_state(modem_reset2_e, 0);
    } else if (stage == MDRV_RESET_CB_AFTER) {
        bsp_set_modem_state(modem_reset0_e, 1);
        bsp_set_modem_state(modem_reset1_e, 1);
        bsp_set_modem_state(modem_reset2_e, 1);
    }
}

/* 打点invoke调用name */
static void cp_reset_invoke_dump(char *cb_name, u32 len)
{
    u32 slice;
    u32 ret;

    if (g_reset_debug.dump_state == (u32)RESET_DUMP_MAGIC) {
        ret = strncpy_s(g_reset_debug.dump_info.invoke_addr, DRV_RESET_MODULE_NAME_LEN, cb_name,
            DRV_RESET_MODULE_NAME_LEN - 1);
        if (ret) {
            reset_print_err("strncpy fail(%d)\n", ret);
        }
        slice = bsp_get_slice_value();

        ret = memcpy_s((void *)(g_reset_debug.dump_info.invoke_addr + DRV_RESET_DUMP_SLICE_OFFSET), sizeof(u32),
            (void *)(&slice), sizeof(u32));
        if (ret) {
            reset_print_err("strncpy fail(%d)\n", ret);
        }
        if (g_reset_debug.dump_info.invoke_addr <
            (char *)((unsigned long)g_reset_debug.dump_info.base_addr + CP_RESET_DUMP_INVOKE_END))
            g_reset_debug.dump_info.invoke_addr += DRV_RESET_DUMP_MEMBER_SIZE;
    }
}
void reset_reboot_system(enum RESET_TYPE type)
{
    unsigned long flags = 0;
    spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
    g_global_reset_ctrl.modem_action = INVALID_MODEM_ACTION;
    g_global_reset_ctrl.reset_action_status = !RESET_IS_SUCC;
    spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);

    osl_sem_up(&(g_global_reset_ctrl.action_sem));
}

s32 reset_prepare(enum MODEM_ACTION action)
{
    unsigned long flags = 0;
    u32 current_action = (u32)action;
    u32 global_action;
    spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
    global_action = g_global_reset_ctrl.modem_action;
    reset_print_err("action(%d) is done, abundon action(%d)\n", global_action, action);
    if (current_action == global_action) {
        spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
        return RESET_OK;
    } else if ((global_action == (u32)MODEM_NORMAL) ||
        ((global_action == (u32)INVALID_MODEM_ACTION) && (current_action == (u32)MODEM_RESET)) ||
        ((global_action == (u32)MODEM_POWER_OFF) && (current_action == (u32)MODEM_POWER_ON))) {
        // 可进行reset
    } else {
        spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
        reset_print_err("action(%d) is done, abundon action(%d)\n", global_action, action);
        return RESET_ERROR;
    }
    g_reset_debug.main_stage = 0;
    g_global_reset_ctrl.modem_action = action;
    spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    __pm_stay_awake(g_global_reset_ctrl.wake_lock);
#else
    __pm_stay_awake(&(g_global_reset_ctrl.wake_lock));
#endif
    cp_reset_timestamp_dump(RESET_DUMP_PREPARE);
    g_reset_debug.main_stage++;
    reset_print_debug("(%d) wake_lock\n", g_reset_debug.main_stage);
    osl_sem_up(&(g_global_reset_ctrl.task_sem));

    return RESET_OK; /*lint !e454 */
}

s32 invoke_reset_cb(drv_reset_cb_moment_e stage)
{
    struct reset_cb_list *p = g_global_reset_ctrl.list_head;
    s32 ret;

    g_reset_debug.main_stage++;
    reset_print_debug("(%d) @reset %d\n", g_reset_debug.main_stage, (u32)stage);

    /* 根据回调函数优先级，调用回调函数 */
    while (p != NULL) {
        if (p->cb_info.cbfun == NULL) {
            p = p->next;
            continue;
        }

        reset_print_debug("%s callback invoked\n", p->cb_info.name);
        cp_reset_invoke_dump(p->cb_info.name, strlen(p->cb_info.name));
        ret = p->cb_info.cbfun(stage, p->cb_info.userdata);
        if (ret != RESET_OK) {
            if (p->cb_info.priolevel != DRV_RESET_CB_PIOR_IPF)
                g_modem_reset_fail_id = MODEM_RESET_NAS_CB_ERR;
            reset_print_err("fail to run cbfunc of %s, at stage%d return %d\n", p->cb_info.name, stage, ret);
            cp_reset_invoke_dump(p->cb_info.name, strlen(p->cb_info.name));
            return RESET_ERROR;
        }
        p = p->next;
    }

    return ret;
}

#define HISI_IPC_MSG_TIMEOUT 5000
static void hifi_reply_ok_process_before(u32 val)
{
    if (!val) {
        g_reset_debug.main_stage++;
        reset_print_debug("before (%d) has received the reply from hifi\n", g_reset_debug.main_stage);
    } else {
        g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
        reset_print_err("before unkown msg from hifi\n");
    }
    return;
}
s32 send_msg_to_hifi_before(void)
{
    s32 ret;
    u32 val = ~0x0;
    /* 消息ID */
    ret = bsp_reset_core_notify(BSP_HIFI, ID_AP_HIFI_CCPU_RESET_REQ, HISI_IPC_MSG_TIMEOUT, &val);
    switch (ret) {
        case BSP_RESET_NOTIFY_REPLY_OK:
            hifi_reply_ok_process_before(val);
            break;
        case BSP_RESET_NOTIFY_SEND_FAILED:
            g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
            reset_print_err("before send_msg_to_hifi=0x%x fail\n", ret);
            break;
        case BSP_RESET_NOTIFY_TIMEOUT:
            g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
            reset_print_err("before waiting the reply from hifi timeout(%d), but not reboot system!\n",
                RESET_WAIT_RESP_TIMEOUT);
            break;
        default:
            g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
            reset_print_err("before unexpect scenario\n");
            break;
    }
    return ret;
}

static void hifi_reply_ok_process_after(u32 val)
{
    if (!val) {
        g_reset_debug.main_stage++;
        reset_print_debug("after (%d) has received the reply from hifi\n", g_reset_debug.main_stage);
    } else {
        g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
        reset_print_err("after unkown msg from hifi\n");
    }
    return;
}
s32 send_msg_to_hifi_after(void)
{
    s32 ret;
    u32 val = ~0x0;
    /* 消息ID ID_AP_HIFI_CCPU_RESET_DONE_IND */
    ret = bsp_reset_core_notify(BSP_HIFI, ID_AP_HIFI_CCPU_RESET_DONE_IND, HISI_IPC_MSG_TIMEOUT, &val);
    switch (ret) {
        case BSP_RESET_NOTIFY_REPLY_OK:
            hifi_reply_ok_process_after(val);
            break;
        case BSP_RESET_NOTIFY_SEND_FAILED:
            g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
            reset_print_err("after send_msg_to_hifi=0x%x fail\n", ret);
            break;
        case BSP_RESET_NOTIFY_TIMEOUT:
            g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
            reset_print_err("after waiting the reply from hifi timeout(%d), but not reboot system!\n",
                RESET_WAIT_RESP_TIMEOUT);
            break;
        default:
            g_modem_reset_fail_id = MODEM_RESET_HIFI_CB_ERR;
            reset_print_err("after unexpect scenario\n");
            break;
    }
    return ret;
}

s32 send_msg_to_hifi(drv_reset_cb_moment_e stage)
{
    s32 ret = 0;

    g_reset_debug.main_stage++;
    cp_reset_timestamp_dump(RESET_DUMP_HIFI_START);
    reset_print_debug("(%d) stage%d,ID_AP_HIFI_CCPU_RESET_REQ=%d\n", g_reset_debug.main_stage, (s32)stage,
        (s32)ID_AP_HIFI_CCPU_RESET_REQ);

    if (stage == MDRV_RESET_CB_BEFORE) {
        ret = send_msg_to_hifi_before();
    }
    if (stage == MDRV_RESET_CB_AFTER) {
        ret = send_msg_to_hifi_after();
    }
    /* 如果有必要，其他阶段也通知hifi */
    cp_reset_timestamp_dump(RESET_DUMP_HIFI_END);
    return ret;
}

#define RESET_MODEM_DONE_TIME_SLEEP 100
s32 wait_for_modem_reset_done(u32 timeout)
{
    g_reset_debug.main_stage++;
    reset_print_debug("(%d) waiting the reply from modem \n", g_reset_debug.main_stage);
    while (timeout--) {
        if (g_global_reset_ctrl.reset_wait_dump_state) {
            reset_print_err("Get msg from dump reset  end reset\n");
            return RESET_ERROR;
        } else if (g_global_reset_ctrl.reset_wait_modem_state) {
            g_reset_debug.main_stage++;
            reset_print_debug("(%d) has received the reply from modem \n", g_reset_debug.main_stage);
            return RESET_OK;
        }
        msleep(RESET_MODEM_DONE_TIME_SLEEP);
    }
    reset_print_err("Get response from modem reset timeout within %d\n", timeout);
    return RESET_ERROR;
}
u32 g_reset_pmu_time = 500;
void reset_set_time(u32 time)
{
    g_reset_pmu_time = time;
}

s32 do_power_off(u16 action)
{
    s32 ret;
    // 通知APP层复位开始
    reset_notify_app(MDRV_RESET_CB_BEFORE);
    g_global_reset_ctrl.boot_mode = 0x1;
    bsp_reset_set_dump_wait_state(0);
    cp_reset_timestamp_dump(RESET_DUMP_POWER_OFF);
    g_reset_debug.main_stage++;
    reset_print_debug("(%d) set boot mode:0x%x\n", g_reset_debug.main_stage, g_global_reset_ctrl.boot_mode);

    /* 通知hifi停止与modem交互并等待hifi处理完成 */
    ret = send_msg_to_hifi(MDRV_RESET_CB_BEFORE);
    if (ret) {
        reset_print_err("can not send msg to hifi go on\n");
    }
    /* 复位前各组件回调 */
    cp_reset_timestamp_dump(RESET_DUMP_CB_BEFORE_START);
    ret = invoke_reset_cb(MDRV_RESET_CB_BEFORE);
    if (ret < 0) {
        reset_reboot_system(RESET_TYPE_CB_INVOKE_BEFORE);
        return RESET_ERROR;
    }
    cp_reset_timestamp_dump(RESET_DUMP_CB_BEFORE_END);
    ccore_msg_switch_off(g_global_reset_ctrl.multicore_msg_switch, CCORE_STATUS);
    g_reset_debug.main_stage++;
    reset_print_err("(%d) before reset stage modem boot reset\n", g_reset_debug.main_stage);
    (void)bsp_modem_boot_reset();
    g_reset_debug.main_stage++;
    reset_print_err("(%d) before reset stage pcie remove start\n", g_reset_debug.main_stage);
    bsp_kernel_pcie_remove();
    reset_print_err("(%d) before reset stage pcie remove end\n", g_reset_debug.main_stage);
    /* set pmu to reset b5000 */
    g_reset_debug.main_stage++;
    reset_print_err("(%d) before reset rst pmu brfore\n", g_reset_debug.main_stage);
    (void)bsp_modem_pmu_reset(g_reset_pmu_time);
    bsp_reset_set_modem_wait_state(0);
    g_reset_debug.main_stage++;
    reset_print_err("(%d) before reset rst pmu after\n", g_reset_debug.main_stage);
    return RESET_OK;
}

s32 do_power_on_after(void)
{
    s32 ret;
    /* 复位后相关处理 */
    cp_reset_timestamp_dump(RESET_DUMP_CB_AFTER_START);
    ret = invoke_reset_cb(MDRV_RESET_CB_AFTER);
    if (ret < 0) {
        reset_reboot_system(RESET_TYPE_CB_INVOKE_AFTER);
        return RESET_ERROR;
    }
    cp_reset_timestamp_dump(RESET_DUMP_CB_AFTER_END);
    /* 通知hifi停止与modem交互并等待hifi处理完成 */
    ret = send_msg_to_hifi(MDRV_RESET_CB_AFTER);
    if (ret) {
        g_reset_debug.main_stage++;
        reset_print_debug("(%d) notify hifi after fail\n", g_reset_debug.main_stage);
    }
    g_reset_debug.main_stage++;
    reset_print_debug("(%d) after stage notify hifi after \n", g_reset_debug.main_stage);
    // 通知APP层复位完成
    reset_notify_app(MDRV_RESET_CB_AFTER);
    g_reset_debug.main_stage++;
    reset_print_debug("(%d) after stage notify app after \n", g_reset_debug.main_stage);
    return RESET_OK;
}

s32 do_power_on(u16 action)
{
    s32 i;
    s32 ret;

    /* C核和dsp镜像加载 */
    /* 如出现错误，重试3次，直至每次都错误，则复位系统 */
    cp_reset_timestamp_dump(RESET_DUMP_LOAD_IMAGE_START);
    g_reset_debug.main_stage++;
    reset_print_err("(%d) reseting stage image loading\n", g_reset_debug.main_stage);
    for (i = 0; i < RESET_RETRY_TIMES; i++) {
        ret = bsp_modem_boot_start();
        if (ret == 0) {
            break;
        } else {
            g_reset_debug.main_stage++;
            reset_print_err("(%d) reseting stage modem boot reset\n", g_reset_debug.main_stage);
            (void)bsp_modem_boot_reset();
            g_reset_debug.main_stage++;
            reset_print_err("(%d) reseting stage pcie remove start\n", g_reset_debug.main_stage);
            bsp_kernel_pcie_remove();
            g_reset_debug.main_stage++;
            reset_print_err("(%d) reseting stage pcie remove end\n", g_reset_debug.main_stage);
            (void)bsp_modem_pmu_reset(g_reset_pmu_time);
            reset_print_err("Retry %d times to load modem image also failed\n", i);
        }
    }
    if (ret < 0) {
        g_modem_reset_fail_id = MODEM_RESET_LOAD_SEC_IMAGE_ERR;
        g_reset_debug.main_stage++;
        reset_print_debug("(%d) reseting stage load image fail\n", g_reset_debug.main_stage);
        reset_reboot_system(RESET_TYPE_LOAD_MODEM_IMG_FAIL);
        return RESET_ERROR;
    }
    cp_reset_timestamp_dump(RESET_DUMP_LOAD_IMAGE_END);
    ccore_msg_switch_on(g_global_reset_ctrl.multicore_msg_switch, CCORE_STATUS);
    cp_reset_timestamp_dump(RESET_DUMP_CP_STARTUP);
    ret = wait_for_modem_reset_done(RESET_WAIT_MODEM_STARTUP_TIMEOUT);
    if (ret < 0) {
        cp_reset_timestamp_dump(RESET_DUMP_CP_STARTUP_TIMEOUT);
        reset_reboot_system(RESET_TYPE_WAIT_CCORE_RELAY_TIMEOUT);
        return RESET_ERROR;
    }

    if (do_power_on_after() != RESET_OK) {
        return RESET_ERROR;
    }

    return RESET_OK;
}

s32 do_reset(u16 action)
{
    s32 ret;

    ret = do_power_off(action);
    if (ret < 0) {
        return RESET_ERROR;
    }

    ret = do_power_on(action);
    if (ret < 0) {
        return RESET_ERROR;
    }

    return RESET_OK;
}

int modem_reset_task(void *arg)
{
    u16 action;
    unsigned long flags = 0;

    for (;;) {
        osl_sem_down(&(g_global_reset_ctrl.task_sem));
        spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
        action = (u16)g_global_reset_ctrl.modem_action;
        spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
        g_reset_debug.main_stage++;
        reset_print_debug("(%d)has taken task_sem, action=%d\n", g_reset_debug.main_stage, action);

        if (action == MODEM_POWER_OFF) {
            (void)do_power_off(action);
        } else if (action == MODEM_POWER_ON) {
            (void)do_power_on(action);
        } else if (action == MODEM_RESET) {
            (void)do_reset(action);
            g_global_reset_ctrl.reset_cnt++;
            reset_print_err("reset count: %d\n", g_global_reset_ctrl.reset_cnt);
        }
        spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
        if ((action == MODEM_POWER_ON || action == MODEM_RESET) && (action == g_global_reset_ctrl.modem_action)) {
            g_global_reset_ctrl.modem_action = MODEM_NORMAL;
            g_global_reset_ctrl.reset_state = RESET_IS_SUCC;
            g_global_reset_ctrl.reset_action_status = RESET_IS_SUCC;
        }
        spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);

        osl_sem_up(&(g_global_reset_ctrl.action_sem));
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        __pm_relax(g_global_reset_ctrl.wake_lock); /*lint !e455 */
#else
        __pm_relax(&(g_global_reset_ctrl.wake_lock)); /*lint !e455 */
#endif

        cp_reset_timestamp_dump(RESET_DUMP_END);
        g_global_reset_ctrl.exec_time = get_timer_slice_delta(g_global_reset_ctrl.exec_time, bsp_get_slice_value());
        reset_print_debug("execute done, elapse time %d\n", g_global_reset_ctrl.exec_time);
    }
}

struct reset_cb_list *sort_list_insert(struct reset_cb_list *list, struct reset_cb_list *node)
{
    struct reset_cb_list *head = list;
    struct reset_cb_list *curr = list;
    struct reset_cb_list *tail = list;

    if (list == NULL || node == NULL) {
        return NULL;
    }
    while (curr != NULL) {
        /* 由小到大, 按优先级插入 */
        if (curr->cb_info.priolevel > node->cb_info.priolevel) {
            if (head == curr) {
                node->next = curr;
                head = node;
            } else {
                tail->next = node;
                node->next = curr;
            }
            break;
        }
        tail = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        tail->next = node;
    }
    return head;
}

struct reset_cb_list *do_cb_func_register(struct reset_cb_list *list_head, const char *func_name, pdrv_reset_cbfun func,
    int user_data, int prior)
{
    struct reset_cb_list *cb_func_node = NULL;
    u32 name_len;

    if (func_name == NULL || func == NULL ||
        (prior < RESET_CBFUNC_PRIO_LEVEL_LOWT || prior > RESET_CBFUNC_PRIO_LEVEL_HIGH)) {
        reset_print_err("register fail, name:%s, cbfun=%lx, prio=%d\n", func_name, (uintptr_t)func, prior);
        return list_head;
    }

    cb_func_node = (struct reset_cb_list *)kmalloc(sizeof(struct reset_cb_list), GFP_KERNEL);
    if (cb_func_node != NULL) {
        name_len = (u32)min((u32)DRV_RESET_MODULE_NAME_LEN, (u32)strlen(func_name));

        memset_s((void *)cb_func_node, (sizeof(*cb_func_node)), 0, (sizeof(struct reset_cb_list)));

        memcpy_s((void *)cb_func_node->cb_info.name, (u32)min((u32)DRV_RESET_MODULE_NAME_LEN, (u32)strlen(func_name)),
            (void *)func_name, (int)name_len);
        cb_func_node->cb_info.priolevel = prior;
        cb_func_node->cb_info.userdata = user_data;
        cb_func_node->cb_info.cbfun = func;
    } else {
        reset_print_err("register malloc fail, name:%s, prio=%d\n", func_name, prior);
        return list_head;
    }

    if (list_head == NULL) {
        list_head = cb_func_node;
    } else {
        list_head = sort_list_insert(list_head, cb_func_node);
    }
    ++g_global_reset_ctrl.list_use_cnt;

    return list_head;
}

void modem_reset_do_work(struct work_struct *work)
{
    reset_print_debug("\n");
    g_global_reset_ctrl.exec_time = bsp_get_slice_value();
    if (g_global_reset_ctrl.in_suspend_state == 1) {
        if (!wait_for_completion_timeout(&(g_global_reset_ctrl.suspend_completion), HZ * RESET_ACTION_HZ_TIMEOUT)) {
            machine_restart("system halt"); /* 调systemError */
            return;
        }
    }
    if (reset_prepare(MODEM_RESET) != RESET_OK) {
        reset_print_err("reset prepare fail!\n");
    }
}

void modem_power_off_do_work(struct work_struct *work)
{
    reset_print_debug("\n");
    if (g_global_reset_ctrl.in_suspend_state == 1) {
        if (!wait_for_completion_timeout(&(g_global_reset_ctrl.suspend_completion), HZ * RESET_ACTION_HZ_TIMEOUT)) {
            machine_restart("system halt"); /* todo: 是否需要调用system_error */
            return;
        }
    }
    if (reset_prepare(MODEM_POWER_OFF) != RESET_OK) {
        reset_print_err("reset prepare fail!\n");
    }
}

void modem_power_on_do_work(struct work_struct *work)
{
    reset_print_debug("\n");
    if (g_global_reset_ctrl.in_suspend_state == 1) {
        if (!wait_for_completion_timeout(&(g_global_reset_ctrl.suspend_completion), HZ * RESET_ACTION_HZ_TIMEOUT)) {
            machine_restart("system halt");
            return;
        }
    }
    if (reset_prepare(MODEM_POWER_ON) != RESET_OK) {
        reset_print_err("reset prepare fail!\n");
    }
}

s32 reset_pm_notify(struct notifier_block *notify_block, unsigned long mode, void *reset_unused)
{
    switch (mode) {
        case PM_HIBERNATION_PREPARE:
        case PM_SUSPEND_PREPARE:
            g_global_reset_ctrl.in_suspend_state = 1;
            break;

        case PM_POST_SUSPEND:
        case PM_POST_HIBERNATION:
        case PM_POST_RESTORE:
            complete(&(g_global_reset_ctrl.suspend_completion));
            g_global_reset_ctrl.in_suspend_state = 0;
            break;
        default:
            break;
    }

    return 0;
}

void reset_feature_on(u32 sw)
{
    g_global_reset_ctrl.nv_config.is_feature_on = (sw ? 1 : 0);
}

void reset_ril_on(u32 sw)
{
    g_global_reset_ctrl.nv_config.is_connected_ril = (sw ? 1 : 0);
}

void reset_ctrl_debug_show(void)
{
    reset_print_err("in_suspend_state: 0x%x\n", g_global_reset_ctrl.in_suspend_state);
    reset_print_err("reset_cnt       : 0x%x\n", g_global_reset_ctrl.reset_cnt);
    reset_print_err("nv.feature_on   : 0x%x\n", g_global_reset_ctrl.nv_config.is_feature_on);
    reset_print_err("nv.ril_flag     : 0x%x\n", g_global_reset_ctrl.nv_config.is_connected_ril);
    reset_print_err("state           : 0x%x\n", g_global_reset_ctrl.state);
}

u32 bsp_reset_is_feature_on(void)
{
    return g_global_reset_ctrl.nv_config.is_feature_on;
}

u32 bsp_reset_is_connect_ril(void)
{
    return g_global_reset_ctrl.nv_config.is_connected_ril;
}

int bsp_cp_reset(void)
{
    unsigned long flags = 0;
    if (!bsp_reset_is_feature_on()) {
        reset_print_err("reset is not init\n");
        return -1;
    }
    spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
    g_global_reset_ctrl.reset_state = !RESET_IS_SUCC;
    spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
    cp_reset_timestamp_dump(RESET_DUMP_START);
    if (g_reset_debug.dump_state == (u32)RESET_DUMP_MAGIC) {
        g_reset_debug.dump_info.invoke_addr =
            (char *)((unsigned long)g_reset_debug.dump_info.base_addr + CP_RESET_DUMP_INVOKE_OFFSET);
    }
    reset_print_debug("\n");
    queue_work(g_global_reset_ctrl.reset_wq, &(g_global_reset_ctrl.work_reset));
    return 0;
}

void bsp_modem_power_off(void)
{
    reset_print_debug("\n");
    if (bsp_reset_is_feature_on()) {
        queue_work(g_global_reset_ctrl.reset_wq, &(g_global_reset_ctrl.work_power_off));
    } else {
        reset_print_err("reset fearute is off\n");
    }
}

void bsp_modem_power_on(void)
{
    reset_print_debug("\n");
    if (bsp_reset_is_feature_on()) {
        queue_work(g_global_reset_ctrl.reset_wq, &(g_global_reset_ctrl.work_power_on));
    } else {
        reset_print_err("reset fearute is off\n");
    }
}

s32 bsp_reset_cb_func_register(const char *name, pdrv_reset_cbfun func, int user_data, int prior)
{
    u32 use_cnt;
    struct reset_cb_list *head = g_global_reset_ctrl.list_head;

    use_cnt = g_global_reset_ctrl.list_use_cnt;
    g_global_reset_ctrl.list_head = do_cb_func_register(head, name, func, user_data, prior);
    if (use_cnt == g_global_reset_ctrl.list_use_cnt) {
        return RESET_ERROR;
    }

    return RESET_OK;
}

struct modem_reset_ctrl *bsp_reset_control_get(void)
{
    return &g_global_reset_ctrl;
}

s32 bsp_reset_ccpu_status_get(void)
{
    enum RESET_MULTICORE_CHANNEL_STATUS channel_status = CCORE_STATUS;

    if (bsp_reset_is_feature_on() == 0) {
        return 1;
    } else {
        return (g_global_reset_ctrl.multicore_msg_switch & channel_status) ? 1 : 0;
    }
}

u32 bsp_reset_is_successful(u32 timeout_ms)
{
    u32 action;
    unsigned long flags = 0;
    UNUSED(timeout_ms);
    spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
    action = g_global_reset_ctrl.reset_state;
    spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
    return action;
}

void bsp_reset_set_dump_wait_state(u32 state)
{
    unsigned long flags = 0;
    reset_print_debug(" set state %d\n", state);
    spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
    g_global_reset_ctrl.reset_wait_dump_state = state;
    spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
}
void bsp_reset_set_modem_wait_state(u32 state)
{
    unsigned long flags = 0;
    reset_print_debug(" set state %d\n", state);
    spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
    g_global_reset_ctrl.reset_wait_modem_state = state;
    spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
}
irqreturn_t reset_pcie_isr(int irq, void *data)
{
    reset_print_debug("\n");
    if (g_global_reset_ctrl.modem_action != MODEM_NORMAL)
        bsp_reset_set_modem_wait_state(1);

    bsp_modem_boot_set_modemready_state();
    return IRQ_HANDLED;
}

void modem_reset_state_mangement(long state, unsigned long lock_flag)
{
    if (state == BALONG_MODEM_RESET) { /* 切卡 */
        spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
        reset_print_err("modem reset %d\n", BALONG_MODEM_RESET);
        system_error(DRV_ERRNO_RESET_SIM_SWITCH, 0, 0, NULL, 0);
        return;
    } else if (state == BALONG_MODEM_OFF) {
        /* To make modem poweroff called only once when there are two rilds. */
        if (g_modem_reset_power_off_flag) {
            bsp_err("Balong_power: modem power off has been called! \n");
            spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
            return;
        }
        bsp_modem_power_off();
        reset_print_err("modem power off %d\n", BALONG_MODEM_OFF);
        g_modem_reset_power_off_flag = 1;
        spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
        return;
    } else if (state == BALONG_MODEM_ON) {
        if ((g_modem_reset_power_off_flag) && (!g_modem_reset_power_on_flag)) {
            bsp_modem_power_on();
            g_modem_reset_power_on_flag = 1;
            reset_print_err("modem power on %d\n", BALONG_MODEM_ON);
        } else {
            reset_print_err("modem now is power on!\n");
        }
        spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
        return;
    } else if (state == BALONG_MODEM_RILD_SYS_ERR) {
        reset_print_err("modem reset using system_error by rild %d\n", BALONG_MODEM_RILD_SYS_ERR);
        spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
        system_error(NAS_REBOOT_MOD_ID_RILD, 0, 0, NULL, 0);
        return;
    } else if (state == BALONG_MODEM_3RD_SYS_ERR) {
        reset_print_err("modem reset using system_error by 3rd modem %d\n", DRV_ERRNO_RESET_3RD_MODEM);
        spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
        system_error(DRV_ERRNO_RESET_3RD_MODEM, 0, 0, NULL, 0);
        return;
    } else {
        spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
        reset_print_err("Balong_power : invalid code to balong power !!!!\n");
        return;
    }
    spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
}

void modem_reset_set_state(long state)
{
    unsigned long lock_flag = 0;
    spin_lock_irqsave(&g_modem_reset_power_spinlock, lock_flag); /*lint !e550*/
    if ((g_modem_reset_power_off_flag) && (!g_modem_reset_power_on_flag) && (state != BALONG_MODEM_ON)) {
        reset_print_err("modem has been power off,please power on,don't reset!\n");
        spin_unlock_irqrestore(&g_modem_reset_power_spinlock, lock_flag);
        return;
    }
    modem_reset_state_mangement(state, lock_flag);
}

#define RESET_STATE_NUM_TWO_PLACE 3
#define RESET_STATE_NUM_MIN 1
#define RESET_STATE_SINGLE_PLACE 2
#define RESET_STATE_PLACE_FACTOR 10
int modem_reset_set_by_rild(char *buf, int count)
{
    long state;

    if (count > RESET_STATE_NUM_TWO_PLACE || count <= RESET_STATE_NUM_MIN) {
        bsp_err("buf len err: %d\n", (int)count);
        return -EINVAL;
    }
    if (count == RESET_STATE_SINGLE_PLACE && (!isdigit(buf[0]))) {
        bsp_err("count = 2,buf err: %c\n", buf[0]);
        return -EINVAL;
    }
    if (count == RESET_STATE_NUM_TWO_PLACE && (!isdigit(buf[0]) || (!isdigit(buf[1])))) {
        bsp_err("count = 3,buf err: %c%c\n", buf[0], buf[1]);
        return -EINVAL;
    }
    if (count == RESET_STATE_SINGLE_PLACE) {
        bsp_info("Power set to %c\n", buf[0]);
    } else if (count == RESET_STATE_NUM_TWO_PLACE) {
        bsp_info("Power set to %c%c\n", buf[0], buf[1]);
    }

    state = (count == RESET_STATE_SINGLE_PLACE) ? (buf[0] - '0') :
                                                  (((buf[0] - '0') * RESET_STATE_PLACE_FACTOR) + (buf[1] - '0'));
    reset_print_err("count = %lu\n", (unsigned long)count);

    if (!bsp_reset_is_connect_ril()) {
        reset_print_err("modem reset not to be connected to ril\n");
        return count;
    }
    modem_reset_set_state(state);
    return count;
}
int reset_register_irq_for_wait_modem_boot(u32 usr_id, u32 cb_id, void *callback_args)
{
    s32 ret;
    if (cb_id == PCIE_RC_CB_ENUM_DONE) {
        ret = bsp_pcie_rc_msi_request(PCIE_EP_MSI_RESET, reset_pcie_isr, "reset_pcie_irq", NULL);
        if (ret) {
            reset_print_err("request reset pcie irq error: %d\n", ret);
            return -1;
        }
        reset_print_err("pcie request reset irq ok\n");
    }
    if (cb_id == PCIE_RC_CB_EXIT) {
        bsp_pcie_rc_msi_free(PCIE_EP_MSI_RESET);
    }
    return 0;
}

int reset_pcie_init(void)
{
    int ret;
    struct pcie_callback_info pcie_reset_callback_info = { 0 };
    pcie_reset_callback_info.callback = reset_register_irq_for_wait_modem_boot;
    pcie_reset_callback_info.callback_args = NULL;
    ret = bsp_pcie_rc_cb_register(PCIE_USER_RESET, &pcie_reset_callback_info);
    reset_print_err("pcie reset cb register ok\n");
    return ret;
}

int __init reset_of_node_init(void)
{
    device_node_s *node = NULL;
    int ret = RESET_OK;

    node = bsp_dt_find_compatible_node(NULL, NULL, "hisilicon,cp_reset_app");
    if (node == NULL) {
        reset_print_err("dts node not found!\n");
        return RESET_ERROR;
    }

    ret = bsp_dt_property_read_u32(node, "scbakdata13", &g_scbakdata13_offset);
    if (ret) {
        reset_print_err("read scbakdata13 from dts is failed,ret = %d!\n", ret);
        return RESET_ERROR;
    }

    return RESET_OK;
}

void bsp_reset_var_init(void)
{
    unsigned long flags = 0;
    memset_s(&g_reset_debug, sizeof(struct modem_reset_debug), 0, sizeof(g_reset_debug));
    osl_sem_init(0, &g_global_reset_ctrl.action_sem);
    osl_sem_init(0, &g_global_reset_ctrl.task_sem);
    osl_sem_init(0, &g_global_reset_ctrl.wait_ccore_reset_ok_sem);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    g_global_reset_ctrl.wake_lock = wakeup_source_register(NULL, "modem_reset wake");
    if (g_global_reset_ctrl.wake_lock == NULL) {
        reset_print_err("wakeup source register fail!\n");
        return RESET_ERROR;
    }
#else
    wakeup_source_init(&g_global_reset_ctrl.wake_lock, "modem_reset wake");
#endif
    spin_lock_init(&g_global_reset_ctrl.action_lock);
    spin_lock_irqsave(&g_global_reset_ctrl.action_lock, flags);
    g_global_reset_ctrl.modem_action = MODEM_NORMAL;
    g_global_reset_ctrl.reset_state = 0;
    g_global_reset_ctrl.reset_action_status = RESET_IS_SUCC;
    spin_unlock_irqrestore(&g_global_reset_ctrl.action_lock, flags);
    g_global_reset_ctrl.multicore_msg_switch = 1;
}

int bsp_reset_task_init(void)
{
    g_global_reset_ctrl.task = kthread_run(modem_reset_task, NULL, "modem_reset");
    if (g_global_reset_ctrl.task == NULL) {
        reset_print_err("create modem_reset thread fail!\n");
        return RESET_ERROR;
    }
    init_completion(&(g_global_reset_ctrl.suspend_completion));
    g_global_reset_ctrl.pm_notify.notifier_call = reset_pm_notify;
    register_pm_notifier(&g_global_reset_ctrl.pm_notify);
    g_global_reset_ctrl.reset_wq = create_singlethread_workqueue("reset_wq");
    if (g_global_reset_ctrl.reset_wq == NULL) {
        reset_print_err("create reset wq fail!\n");
        system_error(DRV_ERRNO_MODEM_RST_FAIL, 0, 0, NULL, 0);
    }
    INIT_WORK(&(g_global_reset_ctrl.work_reset), modem_reset_do_work);
    INIT_WORK(&(g_global_reset_ctrl.work_power_off), modem_power_off_do_work);
    INIT_WORK(&(g_global_reset_ctrl.work_power_on), modem_power_on_do_work);
    return RESET_OK;
}

void bsp_reset_dump_init(void)
{
    g_reset_debug.dump_info.base_addr =
        (void *)bsp_dump_register_field(DUMP_MODEMAP_CPRESET, "CP_RESET", CP_RESET_DUMP_SIZE, 0);
    if (g_reset_debug.dump_info.base_addr == NULL) {
        reset_print_err("get dump addr error,reset init continue.\n");
    } else {
        g_reset_debug.dump_info.file_head =
            (struct reset_dump_header *)g_reset_debug.dump_info
                .base_addr; /*lint !e826: (Info -- Suspicious pointer-to-pointer conversion (area too small))*/
        g_reset_debug.dump_info.file_head->magic = (u32)RESET_DUMP_MAGIC;
        g_reset_debug.dump_info.file_head->len = CP_RESET_DUMP_SIZE;
        g_reset_debug.dump_info.file_head->version = 1;
        g_reset_debug.dump_info.file_head->nv_config = g_global_reset_ctrl.nv_config;

        g_reset_debug.dump_info.entry_tbl =
            (u32 *)((unsigned long)g_reset_debug.dump_info.base_addr + (unsigned long)sizeof(struct reset_dump_header));
        g_reset_debug.dump_info.invoke_addr =
            (char *)((unsigned long)g_reset_debug.dump_info.base_addr + CP_RESET_DUMP_INVOKE_OFFSET);
        g_reset_debug.dump_info.host_addr =
            (char *)((unsigned long)g_reset_debug.dump_info.base_addr + CP_RESET_DUMP_HOST_OFFSET);

        if ((RESET_DUMP_MOD_ID_MAX * sizeof(int) > CP_RESET_DUMP_INVOKE_OFFSET) || ((STAMP_RESET_FIQ_OUT_COUNT -
            STAMP_RESET_BASE_ADDR + 1) > (CP_RESET_DUMP_SIZE - CP_RESET_DUMP_HOST_OFFSET))) { /*lint !e587 */
            reset_print_err("dump base info too oversize, please check.\n");
            g_reset_debug.dump_state = 0;
        } else {
            g_reset_debug.dump_state = (u32)RESET_DUMP_MAGIC;
        }
    }
}

int __init bsp_reset_init(void)
{
    reset_ril_on(0);
    reset_feature_on(0);
    if (RESET_OK != reset_of_node_init()) {
        reset_print_err("reset_of_node_init fail!\n");
        return RESET_ERROR;
    }
    if (reset_pcie_init()) { // 注册PCIE终端用于接收B.ACPU 复位完成消息
        reset_print_err("register pcie int cb error\n");
    }
    bsp_reset_var_init();
    if (bsp_reset_task_init() != RESET_OK) {
        return RESET_ERROR;
    }
    bsp_reset_dump_init();
    spin_lock_init(&g_modem_reset_power_spinlock);
    (void)bsp_modem_state_cb_register(modem_reset_set_by_rild);
    g_reset_debug.print_sw = 1;
    g_global_reset_ctrl.state = 1;
    reset_feature_on(g_global_reset_ctrl.state);
    reset_ril_on(g_global_reset_ctrl.state);
    reset_print_err("ok\n");
    return 0;
}

EXPORT_SYMBOL(bsp_reset_init);            /*lint !e19 */
EXPORT_SYMBOL(bsp_cp_reset);              /*lint !e19 */
EXPORT_SYMBOL(bsp_reset_is_successful);   /*lint !e19 */
EXPORT_SYMBOL(modem_reset_fail_id_get);   /*lint !e19 */
EXPORT_SYMBOL(reset_reboot_system);       /*lint !e19 */
EXPORT_SYMBOL(bsp_reset_control_get);     /*lint !e19 */
EXPORT_SYMBOL(bsp_reset_ccore_is_reboot); /*lint !e19 */
EXPORT_SYMBOL(reset_pm_notify);           /*lint !e19 */
EXPORT_SYMBOL(reset_feature_on);          /*lint !e19 */
EXPORT_SYMBOL(reset_ril_on);              /*lint !e19 */
EXPORT_SYMBOL(reset_ctrl_debug_show);     /*lint !e19 */
EXPORT_SYMBOL(bsp_reset_is_connect_ril);  /*lint !e19 */
EXPORT_SYMBOL(bsp_reset_is_feature_on);   /*lint !e19 */
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(bsp_reset_init); /*lint !e19*/
#endif
