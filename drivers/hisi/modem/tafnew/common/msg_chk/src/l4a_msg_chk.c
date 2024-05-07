/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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

#include "taf_type_def.h"
#include "taf_msg_chk_api.h"
#include "gen_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_L4A_MSG_CHK_C

STATIC TAF_ChkMsgLenNameMapTbl* L4A_GetChkAtMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 L4A_GetChkAtMsgLenTblSize(VOS_VOID);

/* MSP_L4_L4A_PID接收WUEPS_PID_AT的消息AT_MN_Msgtype */
static TAF_ChkMsgLenNameMapTbl g_l4aChkAtMsgLenTbl[] = {
    { ID_MSG_L4A_LWCLASHQRY_REQ, sizeof(L4A_READ_LWCLASH_REQ_STRU), VOS_NULL_PTR },
#ifndef MODEM_FUSION_VERSION
    { ID_MSG_L4A_ISMCOEXSET_REQ, sizeof(L4A_ISMCOEX_Req), VOS_NULL_PTR },
#endif
    { ID_MSG_L4A_RADVER_SET_REQ, sizeof(L4A_SET_RadverReq), VOS_NULL_PTR },
    { ID_MSG_L4A_CERSSI_REQ, sizeof(L4A_CSQ_InfoReq), VOS_NULL_PTR },
    { ID_MSG_L4A_CERSSI_INQ_REQ, sizeof(L4A_CSQ_InfoReq), VOS_NULL_PTR },
    { ID_MSG_L4A_CSQ_INFO_REQ, sizeof(L4A_CSQ_InfoReq), VOS_NULL_PTR },
    { ID_MSG_L4A_ANQUERY_INFO_REQ, sizeof(L4A_CSQ_InfoReq), VOS_NULL_PTR },
    { ID_MSG_L4A_CELL_INFO_REQ, sizeof(L4A_READ_CellInfoReq), VOS_NULL_PTR },
    { ID_MSG_L4A_CELL_ID_REQ, sizeof(L4A_READ_CELL_ID_REQ_STRU), VOS_NULL_PTR },
    { ID_MSG_L4A_FAST_DORM_REQ, sizeof(L4A_SET_FastDormReq), VOS_NULL_PTR },
    { ID_MSG_L4A_LW_THRESHOLD_REQ, sizeof(L4A_LW_ThreasholdCfgReq), VOS_NULL_PTR },
    { ID_MSG_L4A_LTE_TO_IDLE_REQ, sizeof(L4A_SET_LTE_TO_IDLE_REQ_STRU), VOS_NULL_PTR },
    { ID_MSG_L4A_LCACELLQRY_REQ, sizeof(L4A_READ_LCACELL_REQ_STRU), VOS_NULL_PTR },
    { ID_MSG_L4A_LCASCELLINFO_QRY_REQ, sizeof(L4A_READ_ScellInfoReq), VOS_NULL_PTR },
    { ID_MSG_L4A_IND_CFG_REQ, sizeof(L4A_IND_CfgReq), VOS_NULL_PTR }
};

static TAF_ChkMsgLenSndPidMapTbl g_l4aChkMsgLenTbl[] = {
    { WUEPS_PID_AT, L4A_GetChkAtMsgLenTblAddr, L4A_GetChkAtMsgLenTblSize },
};

/* MSP_L4_FID下的PID接收消息有效长度检查MAP */
static TAF_ChkMsgLenRcvPidMapTbl g_l4aFidChkMsgLenTbl[] = {
    TAF_MSG_CHECK_TBL_ITEM(I0_MSP_L4_L4A_PID, g_l4aChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_MSP_L4_L4A_PID, g_l4aChkMsgLenTbl),
};

STATIC TAF_ChkMsgLenNameMapTbl* L4A_GetChkAtMsgLenTblAddr(VOS_VOID)
{
    return g_l4aChkAtMsgLenTbl;
}

STATIC VOS_UINT32 L4A_GetChkAtMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_l4aChkAtMsgLenTbl);
}

VOS_UINT32 L4A_ChkMspL4FidRcvMsgLen(const MsgBlock *msg)
{
    if (VOS_GET_SENDER_ID(msg) == VOS_PID_TIMER) {
        return TAF_ChkTimerMsgLen(msg);
    }

    return TAF_ChkMsgLen(msg, g_l4aFidChkMsgLenTbl, TAF_GET_ARRAY_NUM(g_l4aFidChkMsgLenTbl));
}

VOS_VOID L4A_SortMspL4FidMsgLenChkTbl(VOS_VOID)
{
    TAF_SortChkMsgLenTblByMsgName(L4A_GetChkAtMsgLenTblAddr(), L4A_GetChkAtMsgLenTblSize());
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

