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

/*
 * 版权所有（c）华为技术有限公司 2015-2025
 * 功能描述: 为G200平台编译打桩，商用不需要
 * 修改历史:
 * 1.日    期: 2019年7月22日
 */

#include "vos.h"
#include "cbt_cpm_interface.h"
#include "nv_stru_pam.h"
#include "at_app_vcom_interface.h"
#include "ps_log_filter_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED) || defined(CBT_ENABLED)

VOS_UINT32 DMS_FidInit(enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 APP_NDIS_FidInit(enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 NFEXT_FidInit(enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 ADS_UL_FidInit(enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 ADS_DL_FidInit(enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 ADS_AGENT_FidInit(enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 PPP_FidInit(enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 TAF_AGENT_FidInit (enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

VOS_UINT32 USIMM_APP_FID_Init (enum VOS_InitPhaseDefine ip)
{
    return VOS_OK;
}

CBTCPM_SEND_FUNC AT_QuerySndFunc(AT_PHY_PortUint32 phyPort)
{
    return VOS_NULL_PTR;
}

VOS_VOID AT_RcvFuncReg(AT_PHY_PortUint32 phyPort, CBTCPM_RCV_FUNC rcvFunc)
{
    return;
}

VOS_UINT32  APP_VCOM_Send (APP_VCOM_DevIndexUint8 devIndex, VOS_UINT8 *data, VOS_UINT32 length)
{
    return VOS_OK;
}
VOS_UINT32 APP_VCOM_RegDataCallback(VOS_UINT8 devIndex, SEND_UL_AT_FUNC func)
{
    return VOS_OK;
}

VOS_UINT32 PS_OM_LayerMsgReplaceCBReg(VOS_UINT32 senderPid, PsOmLayerMsgReplaceCb func)
{
    return VOS_OK;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
