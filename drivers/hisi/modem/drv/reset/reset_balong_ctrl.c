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

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/reboot.h>
#include <linux/delay.h>
#include <product_config.h>
#include <bsp_dt.h>
#include <bsp_reset.h>
#include <bsp_pcie.h>
#include <bsp_ipc.h>
#include "reset_balong.h"
#include <securec.h>

struct modem_reset_ctrl g_modem_reset_reboot_ctrl = { 0 };
struct modem_reset_debug g_reset_debug = { 0 };
static unsigned int g_scbakdata13_offset = 0;

#define reset_print_debug(fmt, ...)              \
    do {                                         \
        if (g_reset_debug.print_sw)              \
            reset_print_err(fmt, ##__VA_ARGS__); \
    } while (0)

void reset_ipc_isr_lrreboot(u32 data)
{
    unsigned long flags = 0;
    reset_print_debug("lr boot ok\n");
    spin_lock_irqsave(&g_modem_reset_reboot_ctrl.action_lock, flags);
    g_modem_reset_reboot_ctrl.modem_lrreboot_state = 1;
    spin_unlock_irqrestore(&g_modem_reset_reboot_ctrl.action_lock, flags);
    osl_sem_up(&(g_modem_reset_reboot_ctrl.wait_ccore_reset_ok_sem));
}
void reset_ipc_isr_nrreboot(u32 data)
{
    unsigned long flags = 0;
    reset_print_debug("nr boot ok\n");
    spin_lock_irqsave(&g_modem_reset_reboot_ctrl.action_lock, flags);
    g_modem_reset_reboot_ctrl.modem_nrreboot_state = 1;
    spin_unlock_irqrestore(&g_modem_reset_reboot_ctrl.action_lock, flags);
    osl_sem_up(&(g_modem_reset_reboot_ctrl.wait_ccore_reset_ok_sem));
}

int modem_reset_reboot_task(void *arg)
{
    unsigned long flags = 0;
    int ret;

    for (;;) {
        osl_sem_down(&(g_modem_reset_reboot_ctrl.wait_ccore_reset_ok_sem));
        if (!(g_modem_reset_reboot_ctrl.modem_lrreboot_state && g_modem_reset_reboot_ctrl.modem_nrreboot_state)) {
            continue;
        }

        if (bsp_reset_ccore_is_reboot()) {
            spin_lock_irqsave(&g_modem_reset_reboot_ctrl.action_lock, flags);
            g_modem_reset_reboot_ctrl.modem_lrreboot_state = 0;
            g_modem_reset_reboot_ctrl.modem_nrreboot_state = 0;
            spin_unlock_irqrestore(&g_modem_reset_reboot_ctrl.action_lock, flags);
            /* 发送PCIE中断给A侧 */
            ret = bsp_pcie_ep_vote_lock(PCIE_USER_RESET, 0x1);
            if (ret) {
                reset_print_err("reset pcie vote lock fail %d\n", ret);
            }
            bsp_pcie_ep_send_msi(PCIE_EP_MSI_RESET);
            bsp_pcie_ep_vote_unlock(PCIE_USER_RESET);
            bsp_reset_bootflag_set(CCORE_BOOT_NORMAL);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
            __pm_relax(g_modem_reset_reboot_ctrl.wake_lock);
#else
            __pm_relax(&(g_modem_reset_reboot_ctrl.wake_lock));
#endif
            reset_print_debug("receive modem start ok,send irq to reset\n");
        }

        reset_print_debug("execute done, elapse time %d\n", g_modem_reset_reboot_ctrl.exec_time);
    }
}

s32 bsp_reset_ccpu_status_get(void)
{
    return 1;
}
int bsp_cp_reset(void)
{
    reset_print_err("b.a no reset\n");
    return 0;
}

void bsp_modem_power_off(void)
{
    reset_print_err("[reset]: <%s> is stub\n", __FUNCTION__);
    return;
}


u32 bsp_reset_is_feature_on(void)
{
    return 0;
}

u32 bsp_reset_is_successful(u32 timeout_ms)
{
    reset_print_err("b.a no reset success\n");
    return 0;
}

s32 bsp_reset_cb_func_register(const char *name, pdrv_reset_cbfun func, int user_data, int prior)
{
    return RESET_OK;
}

uintptr_t get_scbakdata13(void)
{
    return ((uintptr_t)bsp_sysctrl_addr_byindex(SYSCTRL_AO) + g_scbakdata13_offset);
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

int __init bsp_reset_init(void)
{
    if (RESET_OK != reset_of_node_init()) {
        reset_print_err("reset_of_node_init fail!\n");
        return RESET_ERROR;
    }
    (void)memset_s(&(g_modem_reset_reboot_ctrl), sizeof(struct modem_reset_ctrl), 0, sizeof(g_modem_reset_reboot_ctrl));

    (void)memset_s(&g_reset_debug, sizeof(struct modem_reset_debug), 0, sizeof(g_reset_debug));

    osl_sem_init(0, &g_modem_reset_reboot_ctrl.wait_ccore_reset_ok_sem);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    g_modem_reset_reboot_ctrl.wake_lock = wakeup_source_register(NULL, "modem_reset wake");
    if (g_modem_reset_reboot_ctrl.wake_lock == NULL) {
        reset_print_err("wakeup source register fail!\n");
        return RESET_ERROR;
    }
#else
    wakeup_source_init(&g_modem_reset_reboot_ctrl.wake_lock, "modem_reset wake");
#endif
    spin_lock_init(&g_modem_reset_reboot_ctrl.action_lock);
    g_modem_reset_reboot_ctrl.task = kthread_run(modem_reset_reboot_task, NULL, "modem_reset_reboot");
    if (g_modem_reset_reboot_ctrl.task == NULL) {
        reset_print_err("create modem_reset thread fail!\n");
        return RESET_ERROR;
    }
    if (bsp_reset_ccore_is_reboot()) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        __pm_stay_awake(g_modem_reset_reboot_ctrl.wake_lock);
#else
        __pm_stay_awake(&(g_modem_reset_reboot_ctrl.wake_lock));
#endif
    }
    g_modem_reset_reboot_ctrl.modem_lrreboot_state = 0;
    g_modem_reset_reboot_ctrl.modem_nrreboot_state = 0;
    g_modem_reset_reboot_ctrl.ipc_recv_irq_reboot = IPC_ACPU_INT_SRC_CCPU_RESET_SUCC;
    g_modem_reset_reboot_ctrl.ipc_recv_irq_nrreboot = IPC_ACPU_INT_SRC_NRCCPU_RESET_SUCC;
    if (bsp_ipc_int_connect(g_modem_reset_reboot_ctrl.ipc_recv_irq_reboot, (voidfuncptr)reset_ipc_isr_lrreboot, 0)) {
        reset_print_err("connect reboot ipc fail!\n");
        return RESET_ERROR;
    }
    if (bsp_ipc_int_enable(g_modem_reset_reboot_ctrl.ipc_recv_irq_reboot)) {
        reset_print_err("enable reboot ipc fail!\n");
        return RESET_ERROR;
    }
    if (bsp_ipc_int_connect(g_modem_reset_reboot_ctrl.ipc_recv_irq_nrreboot, (voidfuncptr)reset_ipc_isr_nrreboot, 0)) {
        reset_print_err("connect nrreboot ipc fail!\n");
        return RESET_ERROR;
    }
    if (bsp_ipc_int_enable(g_modem_reset_reboot_ctrl.ipc_recv_irq_nrreboot)) {
        reset_print_err("enable nrreboot ipc fail!\n");
        return RESET_ERROR;
    }
    g_modem_reset_reboot_ctrl.state = 1;

    reset_print_err("ok\n");

    return 0;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(bsp_reset_init); /*lint !e19*/
#endif
