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
#include "msp_at.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_FTM_MSG_CHK_TBL_C

STATIC TAF_ChkMsgLenNameMapTbl* FTM_GetChkAtMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 FTM_GetChkAtMsgLenTblSize(VOS_VOID);
VOS_UINT32 FTM_ChkSetLtCmdReqMsgLen(const MSG_Header *msgHeader);

static TAF_ChkMsgLenNameMapTbl g_ftmChkAtMsgLenTbl[] = {
    { ID_MSG_SYM_SET_TMODE_REQ, (sizeof(AT_FW_DataMsg) + sizeof(SYM_SET_TmodeReq)), VOS_NULL_PTR},
    { ID_MSG_FTM_SET_TSELRF_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_SetTselrfReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_SET_FCHAN_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_SetFchanReq)),  VOS_NULL_PTR },
    { ID_MSG_FTM_RD_FCHAN_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_RdFchanReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_SET_FWAVE_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_SetFwaveReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_SET_TXON_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_SetTxonReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_RD_TXON_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_RdTxonReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_SET_RXON_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_SetRxonReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_RD_RXON_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_RdRxonReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_SET_AAGC_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_SetAagcReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_RD_AAGC_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_RdAagcReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_FRSSI_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_FRSSI_Req)), VOS_NULL_PTR },
    { ID_MSG_FTM_SET_LTCOMMCMD_REQ, 0, FTM_ChkSetLtCmdReqMsgLen },
    { ID_MSG_FTM_RD_LTCOMMCMD_REQ,  (sizeof(AT_FW_DataMsg) + sizeof(FTM_RdNopraraReq)), VOS_NULL_PTR },
    { ID_MSG_FTM_RD_FPLLSTATUS_REQ, (sizeof(AT_FW_DataMsg) + sizeof(FTM_RdFpllstatusReq)), VOS_NULL_PTR }
};

static TAF_ChkMsgLenSndPidMapTbl g_ftmAgentChkAtMsgLenTbl[] = {
    { WUEPS_PID_AT, FTM_GetChkAtMsgLenTblAddr, FTM_GetChkAtMsgLenTblSize },
};

/* 列表MSP_FTM_FID下各PID接收消息长度检查处理映射表: 第一列:消息接收PID，第二列:指定接收PID的消息长度检查表 */
static TAF_ChkMsgLenRcvPidMapTbl g_ftmFidChkMsgLenTbl[] = {
    TAF_MSG_CHECK_TBL_ITEM(I0_MSP_SYS_FTM_PID, g_ftmAgentChkAtMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_MSP_SYS_FTM_PID, g_ftmAgentChkAtMsgLenTbl),
};

VOS_UINT32 FTM_ChkSetLtCmdReqMsgLen(const MSG_Header *msgHeader)
{
    const AT_FW_DataMsg       *atFwDataMsg = VOS_NULL_PTR;
    const FTM_SetLtcommcmdReq *ltCmdSetReq = VOS_NULL_PTR;
    VOS_UINT32                 fixedPartLength;

    fixedPartLength = sizeof(AT_FW_DataMsg);
    if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < fixedPartLength) {
        return VOS_FALSE;
    }

    atFwDataMsg = (const AT_FW_DataMsg *)msgHeader;
    fixedPartLength += sizeof(FTM_SetLtcommcmdReq);
    if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < fixedPartLength) {
        return VOS_FALSE;
    }

    ltCmdSetReq      = (const FTM_SetLtcommcmdReq *)atFwDataMsg->context;
    fixedPartLength += ltCmdSetReq->dataLen;
    if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < fixedPartLength) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

STATIC TAF_ChkMsgLenNameMapTbl* FTM_GetChkAtMsgLenTblAddr(VOS_VOID)
{
    return g_ftmChkAtMsgLenTbl;
}

STATIC VOS_UINT32 FTM_GetChkAtMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_ftmChkAtMsgLenTbl);
}

VOS_UINT32 FTM_ChkFtmFidRcvMsgLen(const MsgBlock *msg)
{
    return TAF_ChkMsgLen(msg, g_ftmFidChkMsgLenTbl, TAF_GET_ARRAY_NUM(g_ftmFidChkMsgLenTbl));
}

VOS_VOID FTM_SortFtmFidChkMsgLenTbl(VOS_VOID)
{
    TAF_SortChkMsgLenTblByMsgName(g_ftmChkAtMsgLenTbl, TAF_GET_ARRAY_NUM(g_ftmChkAtMsgLenTbl));
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

