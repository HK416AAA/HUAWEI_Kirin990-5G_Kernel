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

#ifndef __HIFI_MANAGE_MSG_PROC_H__
#define __HIFI_MANAGE_MSG_PROC_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include "vos.h"
#include "v_msg.h"
#include "ps_common_def.h"
#include "lps_common.h"
#include "mdrv.h"
#include "mdrv_udi.h"
#include "ps_tag.h"

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

#ifdef PS_ITT_PC_TEST
#define HIFI_MANAGE_INFO_LOG(String) \
                 PS_PRINTF_INFO(" %s\n",String)/*LPS_LOG(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_INFO, String)
 */
#define HIFI_MANAGE_INFO_LOG1(String, Para1) \
                 PS_PRINTF_INFO(" %s %d\n",String, (long)Para1)/*LPS_LOG1(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_INFO, String, (long)Para1 )
 */
#define HIFI_MANAGE_INFO_LOG2(String, Para1, Para2) \
                 PS_PRINTF_INFO(" %s %d %d\n",String, (long)Para1, (long)Para2)/*LPS_LOG2(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_INFO, String, (long)Para1, (long)Para2)
 */
#define HIFI_MANAGE_WARN_LOG(String) \
                 PS_PRINTF_INFO(" %s\n",String)/*LPS_LOG(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_WARNING, String)
 */
#define HIFI_MANAGE_WARN_LOG1(String, Para1) \
                     PS_PRINTF_INFO(" %s %d\n",String, (long)Para1)/*LPS_LOG(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_WARNING, String, (long)Para1 )
 */
#define HIFI_MANAGE_WARN_LOG2(String, Para1, Para2) \
                         PS_PRINTF_INFO(" %s %d\n",String, (long)Para1, (long)Para2)/*LPS_LOG(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_WARNING, String, (long)Para1, (long)Para2)
 */
#define HIFI_MANAGE_ERR_LOG(String) \
                 PS_PRINTF_INFO(" %s\n",String)/*LPS_LOG(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_ERROR, String)
 */
#define HIFI_MANAGE_ERR_LOG1(String, Para1) \
                     PS_PRINTF_INFO(" %s %d\n",String, (long)Para1)/*LPS_LOG(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_ERROR, String, (long)Para1 )
 */
#define HIFI_MANAGE_ERR_LOG2(String, Para1, Para2) \
                     PS_PRINTF_INFO(" %s %d %d\n",String, (long)Para1, (long)Para2)/*LPS_LOG(UE_MODULE_ESM_ID, VOS_NULL, PS_PRINT_ERROR, String, (long)Para1, (long)Para2)
 */

#else
/*打印转层间打开*/
#define HIFI_MANAGE_INFO_LOG(String) \
                 PS_PRINTF_INFO(" %s\n",String)
#define HIFI_MANAGE_INFO_LOG1(String, Para1) \
                 PS_PRINTF_INFO(" %s %d\n",String, (long)Para1)
#define HIFI_MANAGE_INFO_LOG2(String, Para1, Para2) \
                 PS_PRINTF_INFO(" %s %d\n",String, (long)Para1, (long)Para2)
#define HIFI_MANAGE_WARN_LOG(String) \
                 PS_PRINTF_WARNING(" %s\n",String)
#define HIFI_MANAGE_WARN_LOG1(String, Para1) \
                 PS_PRINTF_WARNING(" %s %d\n",String, (long)Para1)
#define HIFI_MANAGE_WARN_LOG2(String, Para1, Para2) \
                 PS_PRINTF_WARNING(" %s %d\n",String, (long)Para1, (long)Para2)
#define HIFI_MANAGE_ERR_LOG(String) \
                 PS_PRINTF_ERR(" %s\n",String)
#define HIFI_MANAGE_ERR_LOG1(String, Para1) \
                 PS_PRINTF_ERR(" %s %d\n",String, (long)Para1)
#define HIFI_MANAGE_ERR_LOG2(String, Para1, Para2) \
                 PS_PRINTF_ERR(" %s %d %d\n",String, (long)Para1, (long)Para2)
#endif

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT8 aucData[4];
}HIFI_MANAGE_OAS_MSG;

VOS_VOID HIFI_MANAGE_MsgProc(MsgBlock* pMsg);
VOS_INT32 HIFI_MANAGE_SendMsg(VOS_UINT32 receiverPid, const VOS_UINT8 *msgBuf, VOS_UINT32 msgLen);


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

#endif /* end of __HIFI_MANAGE_MSG_PROC_H__ */

