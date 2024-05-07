/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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
#include "eicc_platform.h"
#include "eicc_dts.h"
#include "eicc_device.h"
#include "eicc_driver.h"
#include "eicc_core.h"
#include "eicc_pmsr.h"
#include "eicc_dump.h"
#include "eicc_reset.h"
#include "bsp_sec_call.h"


#define EICC_RESET_DBG_STEPS_CNT 4
#define EICC_BEFORE_RESET_CMD 0
#define EICC_AFTER_RESET_CMD 1


struct eicc_reset_dbg {
    u32 before_reset_times;                   /* before reset的成功次数 */
    u32 after_reset_times;                    /* after reset的成功次数 */
    u32 reset_stat[EICC_RESET_DBG_STEPS_CNT]; /* 一次单独复位的记录 */
    u32 chn_id;                               /* 最新reset失败通道 */
    u32 last_step;                            /* 最新reset失败通道失败位置 */
};

struct eicc_reset_dbg g_eicc_reset_dbg;

void eicc_rst_disable_all_sendtomdm_chn_opipe(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_OPEN) {
            continue;
        }

        if ((pchannel->const_flags & EICC_CHN_SF_MDMRST_CARE) == 0) {
            continue;
        }

        if (isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_ORESTTING;
            continue;
        }
        eicc_opipe_local_dis(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        pchannel->state = EICC_CHN_ORESTTING;
    }
    return;
}

int eicc_reset_check_sendtomdm_opipe(void)
{
    int ret;
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_reset_dbg *eicc_reset_dbg = &g_eicc_reset_dbg;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_ORESTTING) {
            continue;
        }
        if (isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_ORESTTED;
            continue;
        }

        ret = opipe_status_check(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        if (ret) {
            if (eicc_reset_dbg->chn_id != pchannel->user_id) {
                eicc_print_error("channel %d reset opipe ret %d\n", pchannel->user_id, ret);
            }
            eicc_reset_dbg->chn_id = pchannel->user_id;
            eicc_reset_dbg->last_step = (u32)ret;
            return ret;
        }
        pchannel->state = EICC_CHN_ORESTTED;
    };
    return 0;
}

void eicc_rst_disable_all_mdm_chn_ipipe(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_ORESTTED) {
            continue;
        }
        if (!isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_IRESTTING;
            continue;
        }
        eicc_ipipe_local_dis(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        pchannel->state = EICC_CHN_IRESTTING;
    };
}


int eicc_reset_check_all_recvmdm_ipipe(void)
{
    int ret;
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_reset_dbg *eicc_reset_dbg = &g_eicc_reset_dbg;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_IRESTTING) {
            continue;
        }
        if (!isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_IRESTTED;
            continue;
        }
        ret = ipipe_neg_check(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        if (ret == 0) {
            ret = ipipe_status_check(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        }
        if (ret) {
            if (eicc_reset_dbg->chn_id != pchannel->user_id) {
                eicc_print_error("channel %d pm ipipe ret %d\n", pchannel->user_id, ret);
            }
            eicc_reset_dbg->chn_id = pchannel->user_id;
            eicc_reset_dbg->last_step = (u32)ret;
            return ret;
        }
        pchannel->state = EICC_CHN_IRESTTED;
    };
    return 0;
}

void eicc_rst_opipe_rptr_config(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_IRESTTED) {
            continue;
        }
        if (isMsgSendChn(pchannel)) {
            pchannel->opipe_cfg->rptr = pchannel->opipe_cfg->wptr;
            eicc_opipe_rptr_update(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), pchannel->opipe_cfg->rptr);
            *(volatile u32 *)(pchannel->opipe_cfg->ptr_vaddr) = pchannel->opipe_cfg->rptr;
        }
        if (isMsgRecvChn(pchannel)) {
            pchannel->ipipe_cfg->wptr = pchannel->ipipe_cfg->rptr;
            eicc_ipipe_wptr_update(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), pchannel->ipipe_cfg->wptr);
            *(volatile u32 *)(pchannel->ipipe_cfg->ptr_vaddr) = pchannel->ipipe_cfg->wptr;
        }
        pchannel->state = EICC_CHN_RESTTED;
    };
    return;
}


int eicc_chn_before_resetting(void)
{
    int ret;
    struct eicc_reset_dbg *eicc_reset_dbg = &g_eicc_reset_dbg;

    eicc_reset_dbg->reset_stat[0x0]++;

    eicc_rst_disable_all_sendtomdm_chn_opipe();
    ret = eicc_reset_check_sendtomdm_opipe();
    if (ret) {
        eicc_print_error("reset check send to modem opipe status failed, ret %d\n", ret);
        eicc_reset_dbg->reset_stat[0x1]++;
        return ret;
    }

    eicc_rst_disable_all_mdm_chn_ipipe();

    ret = eicc_reset_check_all_recvmdm_ipipe();
    if (ret) {
        eicc_print_error("reset check recv form modem ipipe status failed, ret %d\n", ret);
        eicc_reset_dbg->reset_stat[0x2]++;
        return ret;
    }

    /* use sec_call to teeos close ipipo */
    ret = bsp_sec_call(FUNC_MDRV_EICC_CAOPTS, EICC_BEFORE_RESET_CMD);
    if (ret != 0) {
        eicc_print_error("before reset bsp_sec_call return error,ret is %d.\n", ret);
        eicc_reset_dbg->reset_stat[0x3]++;
        return ret;
    }

    eicc_rst_opipe_rptr_config();

    eicc_reset_dbg->before_reset_times++;
    return 0;
}

int eicc_chn_after_resetting(void)
{
    u32 idx = 0;
    int ret, i;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_reset_dbg *eicc_reset_dbg = &g_eicc_reset_dbg;

    /* use sec_call to teeos open ipipo */

    ret = bsp_sec_call(FUNC_MDRV_EICC_CAOPTS, EICC_AFTER_RESET_CMD);
    if (ret != 0) {
        eicc_print_error("after reset bsp_sec_call return error,ret is %d.\n", ret);
        return ret;
    }

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_RESTTED) {
            continue;
        }
        if (isMsgRecvChn(pchannel)) {
            eicc_ipipe_enable(pdev, pchannel);
        } else if (pchannel->type == EICC_CHN_TYPE_MSG_OUTSEND) {
            eicc_opipe_enable(pdev, pchannel);
        } else {
            eicc_opipe_enable(pdev, pchannel);
        }
        pchannel->state = EICC_CHN_OPEN;
    };
    eicc_reset_dbg->after_reset_times++;

    for (i = 0; i < EICC_RESET_DBG_STEPS_CNT; i++) {
        eicc_reset_dbg->reset_stat[i] = 0;
    }
    return 0;
}
