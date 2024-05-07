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

#ifndef __VOICEAGENTVOICEINTERFACE_H__
#define __VOICEAGENTVOICEINTERFACE_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include  "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/* 消息ID定义 */
enum VOICEAGENT_VOICE_MSG_ID_ENUM
{
    ID_VOICEAGENT_VOICE_DUMP_LOG_REQ = 0x9459,   /* 导出HIFI日志请求 */
    ID_VOICEAGENT_VOICE_TOOL_CONNECT_STATUS = 0x9463,

    ID_VOICE_VOICEAGENT_DUMP_LOG_CNF = 0X4940,   /* 响应导出HIFI日志请求 */
    VOICEAGENT_VOICE_MSG_ID_BUTT
};
typedef VOS_UINT16 VOICEAGENT_VOICE_MSG_ID_ENUM_UINT16;

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/

/*****************************************************************************
  5 消息头定义
*****************************************************************************/
struct VoiceAgent_Voice_Msg {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    VOS_UINT16 reserved;
};

/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/
/* DUMP HIFI请求结构体 */
struct VoiceAgent_Voice_DumpLogReq {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    VOS_UINT16 reserved;
    VOS_UINT64 phyAddr;    /* modem A核分配的物理地址*/
    VOS_UINT32 logLength;  /* modem A核分配的日志长度*/
};

/* DUMP HIFI响应结构体 */
struct Voice_VoiceAgent_DumpLogCnf {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    VOS_UINT16 result;    /* dumplog结果，0表示成功，其它失败 */
    VOS_UINT32 reserved;
};
struct Voice_Agent_Diag_Link_msg {
    VOS_MSG_HEADER
    VOS_UINT32              ulMsgId;           /* 消息ID */
    VOS_UINT32              ulLen;             /* 消息长度 */
    VOS_UINT32              ulSn;
    VOS_UINT32              status;
    VOS_UINT32              reversed;
};

/*****************************************************************************
  7 消息定义
*****************************************************************************/

/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  10 函数声明
*****************************************************************************/


#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of VoiceAgentVoiceInterface.h */
