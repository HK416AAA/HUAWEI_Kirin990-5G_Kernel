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

#include "hifi_manage_main.h"
#include <linux/kernel.h>
#include <linux/list.h>
#include "vos.h"
#include "PsTypeDef.h"
#include "hifi_manage_msg_proc.h"
#include "hifi_manage_reset_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_DMS_CORE_C

static VOS_VOID HIFI_MANAGE_Init(VOS_VOID)
{
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_Init enter");
    HIFI_MANAGE_ResetInit();
    return;
}
static VOS_UINT32 HIFI_MANAGE_PidInit(enum VOS_InitPhaseDefine enPhase)
{
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_PidInit enter");
    switch (enPhase) {
        case VOS_IP_INITIAL:
            HIFI_MANAGE_Init();
            break;
        default:
            break;
    }
    return VOS_OK;
}

VOS_UINT32 HIFI_MANAGE_FidInit(enum VOS_InitPhaseDefine ip)
{
    VOS_UINT32 retVal = 0;

    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_FidInit enter");
    switch (ip) {
        case VOS_IP_LOAD_CONFIG:
            retVal = VOS_RegisterPIDInfo(ACPU_PID_HIFI_MANAGE, (InitFunType)HIFI_MANAGE_PidInit, (MsgFunType)HIFI_MANAGE_MsgProc);
            if (retVal != VOS_OK) {
                HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_FidInit register pid error");
                return VOS_ERR;
            }
            retVal = VOS_RegisterTaskPrio(ACPU_FID_HIFI_MANAGE, VOS_PRIORITY_M5);
            if (retVal != VOS_OK) {
                HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_FidInit register pid prio error");
                return VOS_ERR;
            }
            break;

        default:
            break;
    }
    return VOS_OK;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


