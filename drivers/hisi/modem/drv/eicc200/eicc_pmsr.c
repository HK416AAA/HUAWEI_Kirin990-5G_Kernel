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

#define EICC_PM_DBG_STEPS_CNT 4

struct eicc_pm_dbg {
    u32 s_times;                        /* 成功的睡眠次数 */
    u32 r_times;                        /* 成功的唤醒次数 */
    u32 sr_stat[EICC_PM_DBG_STEPS_CNT]; /* 一次睡眠唤醒的记录 */
    u32 chn_id;                         /* 最新睡眠失败通道 */
    u32 last_step;                      /* 最新睡眠失败通道失败位置 */
};

struct eicc_pm_dbg g_eicc_pm_dbg;

void eicc_pm_disable_all_opipe(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_OPEN) {
            continue;
        }

        if (isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_OSUSPENDING;
            continue;
        }
        eicc_opipe_local_dis(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        pchannel->state = EICC_CHN_OSUSPENDING;
    };
    return;
}

int eicc_pm_checkpd_all_opipe(void)
{
    int ret;
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_pm_dbg *eicc_pm_dbg = &g_eicc_pm_dbg;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_OSUSPENDING) {
            continue;
        }
        if (isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_OSUSPENDED;
            continue;
        }

        ret = opipe_status_check(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        if (ret) {
            if (eicc_pm_dbg->chn_id != pchannel->user_id) {
                eicc_print_error("channel %d pm opipe ret %d\n", pchannel->user_id, ret);
            }
            eicc_pm_dbg->chn_id = pchannel->user_id;
            eicc_pm_dbg->last_step = (u32)ret;
            return ret;
        }
        pchannel->state = EICC_CHN_OSUSPENDED;
    };
    return 0;
}

void eicc_pm_disable_all_ipipe(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_OSUSPENDED) {
            continue;
        }
        if (!isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_ISUSPENDING;
            continue;
        }
        eicc_ipipe_local_dis(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        pchannel->state = EICC_CHN_ISUSPENDING;
    };
}

int eicc_pm_checkpd_all_ipipe(void)
{
    int ret;
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_pm_dbg *eicc_pm_dbg = &g_eicc_pm_dbg;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_ISUSPENDING) {
            continue;
        }
        if (!isMsgRecvChn(pchannel)) {
            pchannel->state = EICC_CHN_ISUSPENDED;
            continue;
        }
        ret = ipipe_neg_check(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        if (ret == 0) {
            ret = ipipe_status_check(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        }
        if (ret) {
            if (eicc_pm_dbg->chn_id != pchannel->user_id) {
                eicc_print_error("channel %d pm ipipe ret %d\n", pchannel->user_id, ret);
            }
            eicc_pm_dbg->chn_id = pchannel->user_id;
            eicc_pm_dbg->last_step = (u32)ret;
            return ret;
        }
        pchannel->state = EICC_CHN_ISUSPENDED;
    };
    return 0;
}

int eicc_pm_check_pipe_empty(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    union ipipe_stat i_state = { 0 };
    union opipe_stat o_state = { 0 };
    struct eicc_pm_dbg *eicc_pm_dbg = &g_eicc_pm_dbg;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != (u32)EICC_CHN_ISUSPENDED) {
            continue;
        }
        if (isMsgRecvChn(pchannel)) {
            i_state.val = eicc_ipipe_local_status(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
            if (!i_state.union_stru.is_empty) {
                eicc_pm_dbg->chn_id = pchannel->user_id;
                eicc_pm_dbg->last_step = EICC_ERR_PM_IPIPE_NOT_EMPTY;
                return -1;
            }
        } else {
            o_state.val = eicc_opipe_local_status(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
            if (!o_state.union_stru.is_empty) {
                eicc_pm_dbg->chn_id = pchannel->user_id;
                eicc_pm_dbg->last_step = EICC_ERR_PM_OPIPE_NOT_EMPTY;
                return -1;
            }
        }
        pchannel->state = EICC_CHN_SUSPENDED;
    };
    return 0;
}

static void eicc_pm_recovery(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state == EICC_CHN_INIT || pchannel->state == EICC_CHN_OPEN ||
            pchannel->state == EICC_CHN_OPENNOSR) {
            continue;
        }
        if (isMsgRecvChn(pchannel)) {
            eicc_ipipe_local_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
            pchannel->state = (u32)EICC_CHN_OPEN;
        } else if (pchannel->type == EICC_CHN_TYPE_MSG_OUTSEND) {
            eicc_opipe_local_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
            pchannel->state = (u32)EICC_CHN_OPEN;
        } else {
            eicc_opipe_local_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
            pchannel->state = (u32)EICC_CHN_OPEN;
        }
    };
    return;
}

static void eicc_pm_chn_save_config(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_SUSPENDED) {
            continue;
        }
        if (pchannel->opipe_cfg != NULL) {
            pchannel->opipe_cfg->rptr = eicc_opipe_rptr_get(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        }
        if (pchannel->ipipe_cfg != NULL && pchannel->type != EICC_CHN_TYPE_MSG_OUTSEND) {
            pchannel->ipipe_cfg->wptr = eicc_ipipe_wptr_get(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        }
    };
    return;
}

int eicc_chn_suspend(void)
{
    int ret;
    struct eicc_pm_dbg *eicc_pm_dbg = &g_eicc_pm_dbg;

    eicc_pm_dbg->sr_stat[0x0]++;

    eicc_pm_disable_all_opipe();
    ret = eicc_pm_checkpd_all_opipe();
    if (ret) {
        eicc_pm_recovery();
        eicc_pm_dbg->sr_stat[0x1]++;
        return ret;
    }

    eicc_pm_disable_all_ipipe();
    ret = eicc_pm_checkpd_all_ipipe();
    if (ret) {
        eicc_pm_recovery();
        eicc_pm_dbg->sr_stat[0x2]++;
        return ret;
    }

    ret = eicc_pm_check_pipe_empty();
    if (ret) {
        eicc_pm_recovery();
        eicc_pm_dbg->sr_stat[0x3]++;
        return ret;
    }

    eicc_pm_chn_save_config();
    eicc_pm_dbg->s_times++;
    return 0;
}

void eicc_chn_resume(void)
{
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_pm_dbg *eicc_pm_dbg = &g_eicc_pm_dbg;

    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (pchannel->state != EICC_CHN_SUSPENDED) {
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
    eicc_pm_dbg->r_times++;

    eicc_pm_dbg->sr_stat[0x0] = 0;
    eicc_pm_dbg->sr_stat[0x1] = 0;
    eicc_pm_dbg->sr_stat[0x2] = 0;
    eicc_pm_dbg->sr_stat[0x3] = 0;
    return;
}

/* dev suspend 失败的时候，由pmsrhook负责进行eicc_chn_resume来恢复通道 */
int eicc_dev_suspend(void)
{
    u32 idx;
    struct eicc_device *pdev = NULL;
    for (idx = 0; idx < EICC_DEVICE_NUM_MAX; idx++) {
        pdev = eicc_device_get_fast(idx);
        if (pdev == NULL) {
            continue;
        }
        if (pdev->ctrl_level != EICC_DEV_CONTROL_LEVEL_HOST) {
            continue;
        }
    }

    return 0;
}

void eicc_dev_resume(void)
{
    u32 idx;
    struct eicc_device *pdev = NULL;
    for (idx = 0; idx < EICC_DEVICE_NUM_MAX; idx++) {
        pdev = eicc_device_get_fast(idx);
        if (pdev == NULL) {
            continue;
        }
        /* restore dev configuration */
        if (pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_HOST) {
        }
    }

    return;
}

u32 eicc_pmsr_dump_save(u8 *buf, u32 len)
{
    u32 used_len = sizeof(g_eicc_pm_dbg);
    if (len < used_len) {
        return 0;
    }
    if (memcpy_s(buf, len, &g_eicc_pm_dbg, sizeof(g_eicc_pm_dbg))) {
        return 0;
    }
    return used_len;
}
