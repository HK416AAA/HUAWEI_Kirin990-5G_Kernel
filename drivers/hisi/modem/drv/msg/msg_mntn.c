/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
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
#include "mdrv_diag.h"
#include "bsp_diag.h"

#include "msg_plat.h"
#include "msg_base.h"
#include "msg_mem.h"
#include "msg_core.h"
#include "msg_fixed.h"

#define MSG_MNTN_SEND_RECORD 0x1
#define MSG_MNTN_RECV_RECORD 0x2
#define MSG_MNTN_BUFF_RECORD 0x4
#define MSG_MNTN_DIAG_REPORT 0x10
#define MSG_MNTN_DEFAULT (MSG_MNTN_SEND_RECORD | MSG_MNTN_RECV_RECORD)

static uint32_t g_msg_mntn_ctrl = 0;

void msg_rec_setmntn(u32 val)
{
    g_msg_mntn_ctrl = val;
}

int msg_send_record(struct msg_pkt_info *info)
{
    struct msg_pkt_dbg *pkt_dbg = NULL;
    struct mid_unit_prv *prv_midctrl = NULL;
    uint32_t cur_cnt;
    if ((g_msg_mntn_ctrl & MSG_MNTN_SEND_RECORD) == 0) {
        return 0;
    }

    prv_midctrl = msg_get_private_midctrl(info->srcid);
    if (prv_midctrl == NULL || prv_midctrl->magic != MSG_MID_PRV_REG_MAGIC) {
        /* 溯源无效，无法记录 */
        return -1;
    }

    cur_cnt = prv_midctrl->record.send_cnt++;
    pkt_dbg = &prv_midctrl->record.send_rec[cur_cnt % MSG_RECORD_CNT];

    pkt_dbg->mid = info->dstid;
    pkt_dbg->timestamp = info->timestamp;
    pkt_dbg->len = info->len;
    return 0;
}

int msg_recv_record(struct msg_pkt_info *info)
{
    struct msg_pkt_dbg *pkt_dbg = NULL;
    struct mid_unit_prv *prv_midctrl = NULL;
    uint32_t cur_cnt;
    if ((g_msg_mntn_ctrl & MSG_MNTN_RECV_RECORD) == 0) {
        return 0;
    }

    prv_midctrl = msg_get_private_midctrl(info->dstid);
    if (prv_midctrl == NULL || prv_midctrl->magic != MSG_MID_PRV_REG_MAGIC) {
        /* 溯源无效，无法记录 */
        return -1;
    }
    cur_cnt = prv_midctrl->record.recv_cnt++;
    pkt_dbg = &prv_midctrl->record.recv_rec[cur_cnt % MSG_RECORD_CNT];

    pkt_dbg->mid = info->srcid;
    pkt_dbg->timestamp = info->timestamp;
    pkt_dbg->len = info->len;
    return 0;
}

void msg_diag_report(struct msg_pkt_info *info)
{
/* 单编版本 无上报 */
#if !defined(DRV_BUILD_SEPARATE)
    mdrv_diag_trans_ind_s trans_msg;
    if ((g_msg_mntn_ctrl & MSG_MNTN_DIAG_REPORT) == 0) {
        return;
    }

    trans_msg.ulModule = MDRV_DIAG_GEN_MODULE_EX(DIAG_MODEM_0, DIAG_MODE_COMM, DIAG_MSG_TYPE_MSP);
    trans_msg.ulPid = 0x8003;
    trans_msg.ulReserve = 0;
    trans_msg.ulMsgId = 0x2001;
    trans_msg.ulLength = sizeof(struct msg_pkt_info);
    trans_msg.pData = (void *)info;

    mdrv_diag_trans_report(&trans_msg);
#endif
    return;
}

void msg_buff_record(struct msg_pkt_info *info)
{
    if ((g_msg_mntn_ctrl & MSG_MNTN_BUFF_RECORD) == 0) {
        return;
    }
    /* do record */
    return;
}

int msg_mntn_init(void)
{
    /* 读取nv或者dts， 来默认初始化mntn开关 */
    msg_rec_setmntn(MSG_MNTN_DEFAULT);
    return 0;
}

static msg_chn_t g_msglite_mntn_hdl;

int msg_lite_mntn_send(u32 coreid, u32 chn_id, u32 len)
{
    int ret;

    void *buff = NULL;
    struct msg_addr dst;

    buff = mdrv_msg_alloc(DRV_MID_MSG, len, MSG_FULL_LEN);
    if (buff == NULL) {
        msg_always("mdrv_msg_alloc len = %d failed\n", (int)len);
        return -1;
    }

    dst.core = coreid;
    dst.chnid = chn_id;

    ret = bsp_msg_lite_sendto(g_msglite_mntn_hdl, &dst, buff, len);
    mdrv_msg_free(DRV_MID_MSG, buff);
    return ret;
}

static int msg_lite_mntn_proc(const struct msg_addr *src_info, void *buf, u32 len)
{
    msg_always("get msg from chnid[%d]  len = %d succcess\n", (int)src_info->chnid, (int)len);
    return 0;
}

int msg_lite_mntn_init(void)
{
    struct msgchn_attr lite_attr = { 0 };
    bsp_msgchn_attr_init(&lite_attr);
    lite_attr.chnid = MSG_CHN_LOOP0;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP) | MSG_CORE_MASK(MSG_CORE_APP) | MSG_CORE_MASK(MSG_CORE_LPM) |
        MSG_CORE_MASK(MSG_CORE_TEE);
    lite_attr.lite_notify = msg_lite_mntn_proc;
    if (bsp_msg_lite_open(&g_msglite_mntn_hdl, &lite_attr) == 0) {
        msg_always("bsp_msg_lite_open %d succcess\n", MSG_CHN_LOOP0);
    } else {
        msg_always("bsp_msg_lite_open %d fail\n", MSG_CHN_LOOP0);
    }
    return 0;
}

