/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#ifndef __AT_CDS_INTERFACE_H__
#define __AT_CDS_INTERFACE_H__

#include "vos.h"
#include "taf_type_def.h"
#include "PsTypeDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define ID_MBB_AT_CDS_MSG_ID_BASECODE   0x1000    /*避免后续HISI添加消息*/
/*
*枚举说明  : AT与CDS的消息定义
*/
enum AT_CDS_MSG_ID_ENUM
{
    /* AT发给CDS的消息枚举 */
    ID_AT_CDS_SET_IPFILTER_SWITCH_REQ = ID_MBB_AT_CDS_MSG_ID_BASECODE + 1,
    ID_AT_CDS_QRY_IPFILTER_SWITCH_REQ = ID_MBB_AT_CDS_MSG_ID_BASECODE + 2,

    /* CDS发给AT的消息枚举 */
    ID_CDS_AT_SET_IPFILTER_SWITCH_CNF = ID_MBB_AT_CDS_MSG_ID_BASECODE + 101,
    ID_CDS_AT_QRY_IPFILTER_SWITCH_CNF = ID_MBB_AT_CDS_MSG_ID_BASECODE + 102,

    ID_CDS_AT_MSG_ID_ENUM_BUTT
};

typedef VOS_UINT32 AT_CDS_MSG_ID_ENUM_UINT32;


/*
*结构说明  : ID_AT_CDS_SET_IPFILTER_SWITCH_REQ 消息发送结构体
*/
typedef struct {
    VOS_MSG_HEADER
    AT_CDS_MSG_ID_ENUM_UINT32           msgId;
    VOS_UINT16                          clientId;
    VOS_UINT8                           opId;
    VOS_UINT8                           reserved;
    VOS_UINT32                          ipFilterSwitch;
} AT_CDS_SetIpFilterSwitchReqStru;

/*
*结构说明  : ID_CDS_AT_SET_IPFILTER_SWITCH_CNF 消息发送结构体
*/
typedef struct {
    VOS_MSG_HEADER
    AT_CDS_MSG_ID_ENUM_UINT32           msgId;
    VOS_UINT16                          clientId;
    VOS_UINT8                           opId;
    VOS_UINT8                           reserved;
    VOS_UINT32                          result;
} CDS_AT_SetIpFilterSwitchCnfStru;

/*
*结构说明  : ID_AT_CDS_QRY_IPFILTER_SWITCH_REQ 消息发送结构体
*/
typedef struct {
    VOS_MSG_HEADER
    AT_CDS_MSG_ID_ENUM_UINT32           msgId;
    VOS_UINT16                          clientId;
    VOS_UINT8                           opId;
    VOS_UINT8                           reserved;
} AT_CDS_QryIpFilterSwitchReqStru;

/*
*结构说明  : ID_AT_CDS_QRY_IPFILTER_SWITCH_CNF 消息发送结构体
*/
typedef struct {
    VOS_MSG_HEADER
    AT_CDS_MSG_ID_ENUM_UINT32           msgId;
    VOS_UINT16                          clientId;
    VOS_UINT8                           opId;
    VOS_UINT8                           reserved;
    VOS_UINT32                          result;
    VOS_UINT32                          ipFilterSwitch;
} CDS_AT_QryIpFilterSwitchCnfStru;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
