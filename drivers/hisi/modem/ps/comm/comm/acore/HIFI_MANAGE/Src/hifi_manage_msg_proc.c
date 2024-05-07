/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
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

#include "hifi_manage_msg_proc.h"
#include <linux/list.h>
#include <securec.h>
#include "vos.h"
#include "v_msg.h"
#include "PsTypeDef.h"
#include "hifi_manage_reset_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_DMS_CORE_C

VOS_INT32 HIFI_MANAGE_SendMsg(VOS_UINT32 receiverPid, const VOS_UINT8 *msgBuf, VOS_UINT32 msgLen)
{
    HIFI_MANAGE_OAS_MSG * sendMsg = VOS_NULL;
    VOS_INT32 len = sizeof(HIFI_MANAGE_OAS_MSG) + msgLen;
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_SendMsg enter");

    sendMsg = (HIFI_MANAGE_OAS_MSG *)PS_ALLOC_MSG(ACPU_PID_HIFI_MANAGE, len);
    if (sendMsg == VOS_NULL_PTR) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_SendMsg msg alloc error");
        return VOS_ERR;
    }

    VOS_SET_SENDER_ID(sendMsg, ACPU_PID_HIFI_MANAGE);
    VOS_SET_RECEIVER_ID(sendMsg, receiverPid);

    if (memcpy_s(sendMsg->aucData, len - VOS_MSG_HEAD_LENGTH, msgBuf, msgLen) != VOS_OK) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_SendMsg msg cpy error");
        PS_FREE_MSG(ACPU_PID_HIFI_MANAGE, sendMsg);
        return VOS_ERR;
    }

    if (PS_SEND_MSG(ACPU_PID_HIFI_MANAGE, sendMsg) != VOS_OK) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_SendMsg msg send error");
        return VOS_ERR;
    }
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_SendMsg exit");

    return VOS_OK;
}

VOS_VOID HIFI_MANAGE_HifiManageMsgProc(MsgBlock* pMsg)
{
    HIFI_MANAGE_OAS_MSG *msgHead = (HIFI_MANAGE_OAS_MSG *)pMsg;

    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_HifiManageMsgProc:enter");

    switch (*((VOS_INT32*)msgHead->aucData)) {
        case ID_HIFI_MANAGE_HIFI_EXCEPTION_IND:
            HIFI_MANAGE_HifiExceptionProc(pMsg);
            break;
        case ID_HIFI_MANAGE_HIFI_IMG_LOAD_COMPLETE_IND:
            HIFI_MANAGE_HifiImgLoadResultProc(pMsg);
            break;
        default:
            break;
    }

    return;
}
VOS_VOID HIFI_MANAGE_TmrMsgProc(MsgBlock *pstMsg)
{
    REL_TimerMsgBlock *pstTmrMsg;

    pstTmrMsg = (REL_TimerMsgBlock *)pstMsg;

    switch (pstTmrMsg->name) {
        case HIFI_MANAGE_TMR_ID_EXEC_PROC_TMR:
            HIFI_MANAGE_EscapeTmrTimeoutProc();
            break;
        case HIFI_MANAGE_TMR_ID_LOAD_CHECK_TMR:
            HIFI_MANAGE_LoadFailTmrProc();
            break;
        default:
            HIFI_MANAGE_ERR_LOG1("HIFI_MANAGE_TmrMsgProc", pstTmrMsg->name);
            break;
    }

    return;
}

VOS_VOID HIFI_MANAGE_MsgProc(MsgBlock* pMsg)
{

    MsgBlock *msgHead = pMsg;
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_MsgProc enter");
    if (msgHead == VOS_NULL_PTR) {
        HIFI_MANAGE_ERR_LOG("Error:HIFI_MANAGE_MsgProc Parameter pMsg is NULL!");
        return;
    }

    switch(VOS_GET_SENDER_ID(msgHead)) {
        case ACPU_PID_HIFI_MANAGE:
            HIFI_MANAGE_HifiManageMsgProc(pMsg);
            break;
        case DOPRA_PID_TIMER:
            HIFI_MANAGE_TmrMsgProc(pMsg);
            break;

        default:
            break;
    }

    return;
}
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
