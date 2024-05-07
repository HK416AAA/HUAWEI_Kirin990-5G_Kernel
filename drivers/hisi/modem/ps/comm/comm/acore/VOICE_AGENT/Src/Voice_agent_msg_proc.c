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
#include <linux/list.h>
#include "vos.h"
#include "PsTypeDef.h"
#include "Voice_agent_public.h"
#include "Voice_agent_msg_proc.h"
#include "Voice_agent_nv.h"
#include "mdrv_udi.h"
#include "Voice_agent_om.h"
#include "msp_diag_comm.h"
#include "bsp_icc.h"
#include "HifiManageCodecInterface.h"
#include "VoiceAgentHifiManageInterface.h"
#include "VoiceAgentVoiceInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/

#define THIS_FILE_ID                    PS_FILE_ID_DMS_CORE_C
#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))

/*****************************************************************************
  1 全局变量定义
*****************************************************************************/
struct list_head g_msgList = {0};
VOS_INT16 g_toolConnectStatus = 0;
/*****************************************************************************
  2 外部函数声明
*****************************************************************************/


/*****************************************************************************
  3 函数实现
*****************************************************************************/
VOS_VOID VOICE_AGENT_InitMsgList(VOS_VOID)
{
    INIT_LIST_HEAD(&g_msgList);
}
VOS_VOID VOICE_AGENT_MsgProcCB(char *pVirAddr, char *pPhyAddr, int size)
{
    if (pVirAddr != NULL) {
        VOS_CacheMemFree(pVirAddr);
    }
    if (size < 0) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_MsgProcCB send fail");
    } else {
        VOICEAGENT_INFO_LOG("VOICE_AGENT_MsgProcCB send succ");
    }
}
VOS_VOID VOICE_AGENT_SendMsgToApHifi(MsgBlock* pMsg)
{
    MsgBlock          *pHeader = pMsg;
    VOS_UINT_PTR      ulPhyAddr = VOS_NULL_PTR;
    VOS_UINT32        ulMsgLength = 0;
    HIFI_AGENT_MSG_PCIE_STRU * pMsgHead = NULL;
    ulMsgLength = sizeof(HIFI_AGENT_MSG_PCIE_STRU) + pHeader->ulLength;

    VOICEAGENT_INFO_LOG1("VOICE_AGENT_SendMsgToApHifi ID:", *((VOS_UINT16*)pMsg->value));
    pMsgHead = (HIFI_AGENT_MSG_PCIE_STRU*)VOS_CacheMemAllocDebug(ulMsgLength, (VOS_UINT32)0x00000000);
    if (NULL == pMsgHead)
    {
        VOICEAGENT_ERR_LOG("VOS_CacheMemAllocDebug ERROR");
        return;
    }

    pMsgHead->ulHifiMsgMagic = HIFI_PCIE_MSG_MAIGC_WORD;
    if (memcpy_s(&(pMsgHead->stMsg), (ulMsgLength - sizeof(pMsgHead->ulHifiMsgMagic)),
        pMsg, (VOS_MSG_HEAD_LENGTH + pHeader->ulLength)) != VOS_OK) {
        VOICEAGENT_INFO_LOG("stMsg memcpy fail");
    }

    if (VOS_OK != VOICE_AGENT_PcieSetWrite(VOICE_AGENT_MSG_DEV, (VOS_UINT8*)pMsgHead, ulPhyAddr, ulMsgLength, VOICE_AGENT_MsgProcCB))
    {
        VOS_CacheMemFree(pMsgHead);
        pMsgHead = NULL;
        VOICEAGENT_ERR_LOG("VOICE_AGENT_SendMsgToApHifi WriteDataAsyn ERROR");
    }
    return;
}
VOS_VOID VOICE_AGENT_DiagCnf(mdrv_diag_cfg_msg_head_s *pHeader)
{
    DIAG_CONN_HIFI_CNF_MSG_STRU *stCnfMsg;

    stCnfMsg = (DIAG_CONN_HIFI_CNF_MSG_STRU *)PS_ALLOC_MSG(ACPU_PID_VOICE_AGENT, sizeof(DIAG_CONN_HIFI_CNF_MSG_STRU) - VOS_MSG_HEAD_LENGTH);

    if (stCnfMsg == VOS_NULL_PTR)
    {
        VOICEAGENT_ERR_LOG("DiagAgentMsgProc:PS_ALLOC_MSG fail");
        return;
    }

    stCnfMsg->ulSenderPid   = ACPU_PID_VOICE_AGENT;
    stCnfMsg->ulReceiverPid = pHeader->ulSenderPid;
    stCnfMsg->ulMsgId     = pHeader->ulMsgId;
    stCnfMsg->ulLen = sizeof(stCnfMsg->ulSn) + sizeof(stCnfMsg->ulChannelId) + sizeof(stCnfMsg->ulResult);
    stCnfMsg->ulSn = *((VOS_UINT32 *)pHeader->pContext);
    stCnfMsg->ulChannelId = g_stDiagCtrlInfo.ulChannelId;
    stCnfMsg->ulResult = 0;

    if(VOS_OK != PS_SEND_MSG(ACPU_PID_VOICE_AGENT, stCnfMsg))
    {
        VOICEAGENT_ERR_LOG("DiagAgentMsgProc:PS_SEND_MSG fail");
        return;
    }

}


/*****************************************************************************
 函 数 名  : VOICE_AGENT_ImsaMsgProc
 功能描述  : 处理Imsa的消息
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2018年09月15日
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID VOICE_AGENT_ImsaMsgProc(MsgBlock* pMsg)
{
    VOICE_AGENT_SendMsgToApHifi(pMsg);
}
/*****************************************************************************
 函 数 名  : VOICE_AGENT_DiagAgentMsgProc
 功能描述  : 处理DiagAgent的消息
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2018年09月15日
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID VOICE_AGENT_DiagAgentMsgProc(MsgBlock* pMsg)
{
    mdrv_diag_cfg_msg_head_s *pHeader = (mdrv_diag_cfg_msg_head_s *)pMsg;

    VOICEAGENT_INFO_LOG("DiagAgentMsgProc:enter");

    if (pHeader->ulMsgId == ID_MSG_DIAG_CMD_CONNECT_REQ) {
        if (GetSocpStatus() == VOICE_AGENT_SOCP_CFG_DONE)
        {
            VOICE_AGENT_SrcChanStart();
        }
        g_toolConnectStatus = 1;
        VOICE_AGENT_DiagCnf(pHeader);

    } else if (pHeader->ulMsgId == ID_MSG_DIAG_CMD_DISCONNECT_REQ) {
        VOICE_AGENT_SrcChanStop();
        g_toolConnectStatus = 0;
        VOICE_AGENT_DiagCnf(pHeader);
    }

    VOICE_AGENT_SendMsgToApHifi(pMsg);

}
/*****************************************************************************
 函 数 名  : VOICE_AGENT_MtcMsgProc
 功能描述  : 处理VC的消息
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2018年09月15日
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID VOICE_AGENT_MtcMsgProc(MsgBlock* pMsg)
{
    VOICE_AGENT_SendMsgToApHifi(pMsg);
}
static VOS_VOID VOICE_AGENT_SendToolsStatusToHifi(VOS_VOID)
{
    struct Voice_Agent_Diag_Link_msg msg = {0};
    VOS_INT32 ret;

    VOICEAGENT_ERR_LOG("VOICE_AGENT_SendToolsStatusToHifi enter");

    msg.ulSenderCpuId = VOS_CPU_ID_ACPU;
    msg.ulSenderPid = ACPU_PID_HIFI_MANAGE;
    msg.ulReceiverCpuId = VOS_CPU_ID_MEDDSP;
    msg.ulReceiverPid = DSP_PID_HIFI_OM;
    msg.ulLength = sizeof(struct Voice_Agent_Diag_Link_msg) - VOS_MSG_HEAD_LENGTH;
    msg.ulMsgId = ID_VOICEAGENT_VOICE_TOOL_CONNECT_STATUS;
    msg.status = g_toolConnectStatus;

    ret = bsp_icc_send(ICC_CPU_HIFI, ACORE_HIFI_ICC_CHN_DUMP_FUNC, (VOS_UINT8*)&msg, sizeof(msg));
    if(ret != (VOS_INT32)(sizeof(msg))) {
        VOICEAGENT_ERR_LOG1("VOICE_AGENT_SendToolsStatusToHifi icc send fail,ret=", ret);
    }
}

VOS_VOID VOICE_AGENT_HifiManageMsgProc(MsgBlock* pMsg)
{
    switch(*(VOS_UINT32*)(pMsg->value)) {
    case ID_HIFI_MANAGE_TO_VOICE_AGENT_RESET_FINISHED:
        VOICE_AGENT_SendToolsStatusToHifi();
        break;
    case ID_HIFI_MANAGE_BHIFI_RESET_IND:
        VOICE_AGENT_SendMsgToApHifi(pMsg);
        break;
    default:
        VOICEAGENT_INFO_LOG("VOICE_AGENT_HifiManageMsgProc:default");
        break;
    }

}

/*****************************************************************************
 函 数 名  : VOICE_AGENT_VcMsgProc
 功能描述  : 处理VC的消息
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2018年09月15日
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID VOICE_AGENT_VcMsgProc(MsgBlock* pMsg)
{
    MsgBlock *pHeader = pMsg;
    HIFI_AGENT_MSG_NODE_STRU *msgNode = NULL;
    VCVOICE_NV_REFRESH_IND_STRU *nvMsg = NULL;
    if (*(VOS_UINT16*)(pHeader->value) == ID_VC_VOICE_NV_REFRESH_IND) {
        nvMsg = (VCVOICE_NV_REFRESH_IND_STRU *)pMsg;
        if (nvMsg->ucRefreshReason) {
            VOICE_AGENT_SendCnfMsgToOther(ID_VOICE_VC_NV_REFRESH_RSP, nvMsg->ulSenderPid, VCVOICE_EXECUTE_RSLT_PARA_ERR);
            VOICEAGENT_ERR_LOG("VCVOICE_EXECUTE_RSLT_PARA_ERR");
            return;
        }

        msgNode = (HIFI_AGENT_MSG_NODE_STRU*)VOS_CacheMemAllocDebug(sizeof(HIFI_AGENT_MSG_NODE_STRU), (VOS_UINT32)0x00000000);
        if (NULL == msgNode)
        {
            VOICEAGENT_ERR_LOG("VOS_CacheMemAllocDebug ERROR");
            return;
        }

        if (list_empty(&g_msgList)) {
            if (VOICE_AGENT_SendNvToApHifi(VOICE_AGENT_PcieWriteNvDataCB) == VOS_OK) {
                //add to list tail//
                VOICEAGENT_INFO_LOG("SET PCIE add to list tail");
                if (memcpy_s(&(msgNode->msg), sizeof(HIFI_AGENT_MSG_NODE_STRU),
                                    pMsg, (VOS_MSG_HEAD_LENGTH + pHeader->ulLength)) != VOS_OK) {
                    VOICEAGENT_INFO_LOG("msgNode->msg memcpy fail");
                }
                list_add_tail(&(msgNode->msg_node),&g_msgList);
            } else {
                VOS_CacheMemFree(msgNode);
                VOICE_AGENT_SendCnfMsgToOther(ID_VOICE_VC_NV_REFRESH_RSP, nvMsg->ulSenderPid, VCVOICE_EXECUTE_RSLT_FAIL);
                return;
            }
        } else {
            VOICEAGENT_INFO_LOG("ONLY add to list tail");
            //add to list tail//
            if (memcpy_s(&(msgNode->msg), sizeof(HIFI_AGENT_MSG_NODE_STRU),
                                pMsg, (VOS_MSG_HEAD_LENGTH + pHeader->ulLength)) != VOS_OK) {
                VOICEAGENT_INFO_LOG("msgNode->msg memcpy fail");
            }
            list_add_tail(&(msgNode->msg_node),&g_msgList);
        }

    }
}
VOS_VOID VOICE_AGENT_LoopMsgProc(MsgBlock* pMsg)
{
    HIFI_AGENT_MSG_NODE_STRU *msgNode = NULL;
        // list_empty
        if (!list_empty(&g_msgList)) {
            VOICEAGENT_INFO_LOG("list first node NOT empty");
            msgNode = list_entry(g_msgList.next, HIFI_AGENT_MSG_NODE_STRU, msg_node);
            VOICE_AGENT_SendCnfMsgToOther(ID_VOICE_VC_NV_REFRESH_RSP, msgNode->msg.ulSenderPid, VCVOICE_EXECUTE_RSLT_SUCC);
            list_del(&msgNode->msg_node);
            VOS_CacheMemFree(msgNode);
        }
        while (!list_empty(&g_msgList)) {
            VOICEAGENT_ERR_LOG("list ELSE node NOT empty");
            if (VOICE_AGENT_SendNvToApHifi(VOICE_AGENT_PcieWriteNvDataCB) == VOS_OK) {
                VOICEAGENT_INFO_LOG("SendNvToApHifi succ break");
                break;
            } else {
                msgNode = list_entry(g_msgList.next, HIFI_AGENT_MSG_NODE_STRU, msg_node);
                VOICEAGENT_INFO_LOG("send RSP");
                VOICE_AGENT_SendCnfMsgToOther(ID_VOICE_VC_NV_REFRESH_RSP, msgNode->msg.ulSenderPid, VCVOICE_EXECUTE_RSLT_FAIL);
                list_del(&msgNode->msg_node);
                VOS_CacheMemFree(msgNode);
            }
        }
        VOICEAGENT_INFO_LOG("list while done");
}
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
