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
 * 版权所有（c）华为技术有限公司 2001-2011
 * 修改历史:
 * 1.日    期: 2008年5月3日
 *   修改内容: 创建文件
 */
#include "om_private.h"
#include "cbt_scm_soft_decode.h"
#include "cbt_ppm.h"
#include "pam_tag.h"
#if ((OSA_CPU_ACPU == VOS_OSA_CPU) && (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF))
#include "pam_app_om.h"
#else
#include "pam_om.h"
#endif

#ifndef MODEM_FUSION_VERSION
#include "msp.h"
#else
#include "mdrv_diag.h"
#endif

#include "mdrv_nvim.h"

#if (VOS_OS_VER == VOS_WIN32)
#include "ut_mem.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_APP_CBT_FUNC_C
#define THIS_MODU mod_pam_cbt

#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED)
#define CBT_WRITE_C1C2_REQ 0x1

/*
 * Description: C1\C2写请求结构体
*/
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;
    VOS_UINT16 c1;
    VOS_UINT16 c2;
}CBT_WriteC1C2Req;

#define CBT_WRITE_C1C2_CNF 0x2

/*
 * Description: C1\C2写CNF结构体
*/
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;
    VOS_UINT32 result;
}CBT_WriteC1C2Cnf;

VOS_UINT32 g_cbtSendErr = 0;


VOS_VOID CBT_AcpuSetC1C2ValueCnf(VOS_UINT32 result)
{
    CBT_WriteC1C2Cnf *msg = VOS_NULL_PTR;

    msg = (CBT_WriteC1C2Cnf *)VOS_AllocMsg(ACPU_PID_CBT,
        sizeof(CBT_WriteC1C2Cnf) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        return;
    }

    VOS_SET_RECEIVER_ID(msg, CCPU_PID_CBT);
    msg->msgName = CBT_WRITE_C1C2_CNF;
    msg->result = result;

    if (VOS_SendMsg(ACPU_PID_CBT, msg) != VOS_OK) {
        return;
    }

    return;
}


VOS_VOID CBT_AcpuSetC1C2Value(const CBT_WriteC1C2Req *msg)
{
    VOS_UINT32 result;

    if (VOS_GET_MSG_LEN(msg) < sizeof(CBT_WriteC1C2Req) - VOS_MSG_HEAD_LENGTH) {
        CBT_AcpuSetC1C2ValueCnf(VOS_ERR);
        return;
    }

    result = VOS_OK;
#if ((FEATURE_PHONE_USIM == FEATURE_ON) && (FEATURE_ON == FEATURE_SET_C1C2_VALUE))
    mdrv_info("Call pmu_dcxo_set, C1 = %d, C2 = %d.\r\n", msg->c1, msg->c2);
    result = pmu_dcxo_set(msg->c1, msg->c2);
#endif

    CBT_AcpuSetC1C2ValueCnf(result);
    return;
}


VOS_VOID CBT_AcpuSendData(CBT_EnDeMsg *msg)
{
    if (VOS_GET_MSG_LEN(msg) < sizeof(CBT_EnDeMsg) - VOS_MSG_HEAD_LENGTH) {
        return;
    }

    if (msg->dataLen > VOS_GET_MSG_LEN(msg) - ((sizeof(CBT_EnDeMsg) - VOS_MSG_HEAD_LENGTH))) {
        return;
    }

    if (CBTPPM_OamCbtPortDataSnd(msg->data, msg->dataLen) != VOS_OK) {
        g_cbtSendErr++;
    }
    return;
}


VOS_VOID CBT_AppMsgProc(MsgBlock *msg)
{
    VOS_UINT32 msgID;
    CBT_WriteC1C2Req *writeReq = VOS_NULL_PTR;

    /* 消息非空检测 */
    if (msg == VOS_NULL_PTR) {
        mdrv_err("<CBT_AppMsgProc> Msg Null!\r\n");
        return;
    }

    /* 判断是否为CBT模块的PID */
    if (VOS_GET_SENDER_ID(msg) != CCPU_PID_CBT) {
        mdrv_err("<CBT_AppMsgProc> please check PID!\r\n");
        return;
    }

    /* 消息长度检测,至少需要保证MsgName字段 */
    if (VOS_GET_MSG_LEN(msg) < sizeof(VOS_UINT32)) {
        mdrv_err("<CBT_AppMsgProc> Msg Length less 4Byte!\r\n");
        return;
    }

    writeReq = (CBT_WriteC1C2Req *)msg;
    msgID = writeReq->msgName;
    if (msgID == CBT_WRITE_C1C2_REQ){
        CBT_AcpuSetC1C2Value(writeReq);
    } else if(msgID == CBT_EN_DE_MSG) {
        CBT_AcpuSendData((CBT_EnDeMsg *)msg);
    }else {
        /* do nothing */
    }
    return;
}

/*
 * Description: ACPU CBT PID' initializtion function
 * History:
 *    Date: 2015-07-10
 *    Modification: Created function
 */
VOS_UINT32 CBT_AppPidInit(enum VOS_InitPhaseDefine ip)
{
    switch (ip) {
        case VOS_IP_RESTART:
            CBTPPM_OamCbtPortInit();
            break;

        default:
            break;
    }

    return VOS_OK;
}

/*
 * Description: OM' initializtion function
 * History:
 *    Date: 2008-03-20
 *    Modification: Created function
 */
VOS_UINT32 CBTAppFidInit(enum VOS_InitPhaseDefine ip)
{
    VOS_UINT32 rslt;

    switch (ip) {
        case VOS_IP_LOAD_CONFIG: {
            rslt = VOS_RegisterPIDInfo(ACPU_PID_CBT, (InitFunType)CBT_AppPidInit, (MsgFunType)CBT_AppMsgProc);
            if (rslt != VOS_OK) {
                return VOS_ERR;
            }

            rslt = VOS_RegisterMsgTaskPrio(ACPU_FID_CBT, VOS_PRIORITY_M2);
            if (rslt != VOS_OK) {
                return VOS_ERR;
            }

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
            if (VOS_RegisterFidTaskCoreBind(ACPU_FID_CBT, VOS_GetAllCoresMask()) != VOS_OK) {
                return VOS_ERR;
            }
#endif

            /* CBT自处理任务创建 */
            if (CBTSCM_SoftDecodeReqRcvTaskInit() != VOS_OK) {
                return VOS_ERR;
            }

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
            if (CBTPPM_SocketTaskInit() != VOS_OK) {
                return VOS_ERR;
            }
#endif

            break;
        }

        default:
            break;
    }

    return VOS_OK;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
