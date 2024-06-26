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

#ifndef __VOICEAGENTMSGPROC_H__
#define __VOICEAGENTMSGPROC_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/



#define THIS_MODU ps_modem_voice


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
#pragma pack(*)    设置字节对齐方式
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/
#define HIFI_PCIE_MSG_MAIGC_WORD        0x5a5b5c5d

/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

/*****************************************************************************
  4 Enum
*****************************************************************************/

/*****************************************************************************
  4 Struct
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT8                           aucData[4];
}HIFI_AGENT_MSG_OSA_STRU;

typedef struct
{
    VOS_UINT32                          ulHifiMsgMagic;
    HIFI_AGENT_MSG_OSA_STRU             stMsg;
}HIFI_AGENT_MSG_PCIE_STRU;

typedef struct
{
    VCVOICE_NV_REFRESH_IND_STRU msg;
    struct list_head msg_node;
}HIFI_AGENT_MSG_NODE_STRU;

typedef struct{
    VOS_MSG_HEADER
    VOS_UINT32              ulMsgId;           /* 消息ID */
    VOS_UINT32              ulLen;             /* 消息长度 */
    VOS_UINT32              ulSn;
    VOS_UINT32              ulChannelId;        /* 通道ID */
    VOS_UINT32              ulResult;           /*  处理结果 0成功, 0x5C5C5C5C通道未分配, 0x5A5A5A5A通道未使用, 其他值失败*/
}DIAG_CONN_HIFI_CNF_MSG_STRU;

/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/
#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
extern VOS_VOID VOICE_AGENT_InitMsgList(VOS_VOID);
extern VOS_VOID VOICE_AGENT_DiagAgentMsgProc(MsgBlock* pMsg);
extern VOS_VOID VOICE_AGENT_ImsaMsgProc(MsgBlock* pMsg);
extern VOS_VOID VOICE_AGENT_MtcMsgProc(MsgBlock* pMsg);
extern VOS_VOID VOICE_AGENT_VcMsgProc(MsgBlock* pMsg);
extern VOS_VOID VOICE_AGENT_LoopMsgProc(MsgBlock* pMsg);
extern VOS_VOID VOICE_AGENT_HifiManageMsgProc(MsgBlock* pMsg);
#endif
/*****************************************************************************
  9 OTHERS
****************************************************************************/

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif


#ifdef __cplusplus
    #if __cplusplus
            }
    #endif
#endif

#endif /* end of __VOICEAGENTMSGPROC_H__ */



