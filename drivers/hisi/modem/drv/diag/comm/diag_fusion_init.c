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

#include <securec.h>
#include <linux/module.h>
#include "bsp_softtimer.h"
#include "bsp_nvim.h"
#include "bsp_version.h"
#include "diag_port_manager.h"
#include "diag_service.h"
#include "diag_fusion_init.h"
#include "diag_fusion_report.h"
#include "diag_fusion_message.h"
#include "diag_fusion_cfg.h"
#include "diag_fusion_conn.h"
#include "ppm_common.h"
#include "ppm_port_switch.h"
#include "msp_service.h"
#include "scm_fusion_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct softtimer_list g_diag_timer[DIAG_TIMER_BUTT];

__init int diag_init(void)
{
    u32 ret;

    ppm_disconnect_cb_reg(diag_notify_port_disconn);

    ret = ppm_init_phy_port();
    if (ret) {
        diag_error("init ppm fail, ret:0x%x\n", ret);
        return ret;
    }

    ret = cpm_port_associate_init();
    if (ret) {
        diag_error("init cpm fail, ret:0x%x\n", ret);
        return ret;
    }

    ret = scm_init();
    if (ret) {
        diag_error("init scm fail, ret:0x%x\n", ret);
        return ret;
    }

    diag_srv_init();

    diag_report_init();

    msp_srv_init();

    diag_message_init();

    ret = (u32)diag_timer_init();
    if (ret) {
        diag_error("init timer fail, ret:0x%x\n", ret);
        return ret;
    }

    diag_set_global_switch(DIAG_CFG_INIT_BIT, DIAG_CFG_SWT_OPEN);

    diag_poweron_log_init();

    diag_crit("diag init ok\n");
    return ERR_MSP_SUCCESS;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(diag_init);
#endif

/* diag使用的timer统一在diag_timer_init中进行初始化 */
s32 diag_timer_init(void)
{
    s32 ret;

    ret = diag_mntn_timer_init();
    if (ret) {
        diag_error("create diag mntn timer fail, ret=0x%x\n", ret);
        return ret;
    }

    ret = diag_flow_timer_init();
    if (ret) {
        diag_error("create diag flow timer fail, ret=0x%x\n", ret);
    }

    return ERR_MSP_SUCCESS;
}

s32 diag_mntn_timer_init(void)
{
    struct softtimer_list *soft_timer = &g_diag_timer[DIAG_TIMER_MNTN];

    soft_timer->func = diag_mntn_report;
    soft_timer->para = NULL;
    soft_timer->timeout = 3000;
    soft_timer->wake_type = SOFTTIMER_NOWAKE;
    soft_timer->unit_type = TYPE_MS;

    return bsp_softtimer_create(soft_timer);
}

s32 diag_flow_timer_init(void)
{
    struct softtimer_list *soft_timer = &g_diag_timer[DIAG_TIMER_FLOW];

    soft_timer->func = diag_flow_report;
    soft_timer->para = NULL;
    soft_timer->timeout = 1000;
    soft_timer->wake_type = SOFTTIMER_WAKE;
    soft_timer->unit_type = TYPE_MS;

    return bsp_softtimer_create(soft_timer);
}

struct softtimer_list *diag_get_timer_node(diag_timer_e timer_enum)
{
    if (timer_enum >= DIAG_TIMER_BUTT) {
        return NULL;
    }

    return &g_diag_timer[timer_enum];
}

void diag_poweron_log_init(void)
{
    s32 poweron_log_enable;
    DRV_NV_POWER_ON_LOG_SWITCH_STRU poweron_log_nv;
#ifndef FEATURE_NVA_ON
    u32 ret;
#endif

    poweron_log_enable = scm_get_poweron_log_enable();
    if (poweron_log_enable == BSP_ERROR) {
        diag_error("get poweron log enable fail\n");
        return;
    }

    diag_conn_reset();

    if (poweron_log_enable == BSP_TRUE) {
        diag_set_global_switch(DIAG_CFG_POWERON_LOG_BIT, DIAG_CFG_SWT_OPEN);
#ifdef FEATURE_NVA_ON
        poweron_log_nv.cswACPUDiag = 1;
#else
        ret = bsp_nvm_read(NV_ID_DRV_DIAG_POWER_LOG, (u8 *)&poweron_log_nv, sizeof(poweron_log_nv));
        if (ret) {
            diag_error("nv read fail, ret=0x%x\n", ret);
            return;
        }
#endif
        diag_set_poweronlog_profile(poweron_log_nv.cswACPUDiag);
    } else {
        diag_set_global_switch(DIAG_CFG_POWERON_LOG_BIT, DIAG_CFG_SWT_CLOSE);
    }

    return;
}

#ifdef __cplusplus
}
#endif
