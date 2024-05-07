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
 * 1.日    期: 2014年5月25日
 *   修改内容: V8R1 OM_Optimize项目新增
 */
#include "cbt_scm_soft_decode.h"
#include "om_private.h"
#include "cbt_ppm.h"
#include "mdrv.h"
#include "pam_tag.h"
#if (VOS_OS_VER != VOS_WIN32)
#include <securec.h>
#endif

#if (VOS_OS_VER == VOS_WIN32)
#include "ut_mem.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_CBT_SCM_SOFT_DECODE_C
#define THIS_MODU mod_pam_cbt

#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED)
/*
 * SCM RCV 的任务栈大小
 */
#define SCM_CBT_REQ_TASK_STACK_SIZE 8096

/* 自旋锁，用来作CBT数据接收的临界资源保护 */
VOS_SPINLOCK g_cbtScmDataRcvSpinLock;

/* SCM数据接收数据缓冲区 */
VOS_CHAR g_cbtScmDataRcvBuffer[CBTSCM_DATA_RCV_PKT_SIZE];

/* SCM数据接收任务控制结构 */
CBTSCM_DataRcvCtrl g_cbtScmDataRcvTaskCtrlInfo;

CBTSCM_SoftdecodeInfo g_cbtScmSoftDecodeInfo;


/*
 * 功能描述: SCM软解码数据接收函数
 * 输入参数: buffer:数据内容
 *           len:数据长度
 * 修改历史:
 * 1. 日    期: 2014年5月21日
 *    修改内容: Creat Function
 */
VOS_UINT32 CBTSCM_SoftDecodeDataRcvProc(const VOS_UINT8 *buffer, VOS_UINT32 len)
{
    VOS_INT32 ret;

    if (len > (VOS_UINT32)OM_RingBufferFreeBytes(g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId)) {
        g_cbtScmSoftDecodeInfo.rbInfo.bufferNotEnough++;

        return VOS_ERR;
    }

    ret = OM_RingBufferPut(g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId, (VOS_CHAR *)buffer, (VOS_INT)len);
    if (len == (VOS_UINT32)ret) {
        if (VOS_SmV(g_cbtScmDataRcvTaskCtrlInfo.smId) == VOS_OK) {
            g_cbtScmSoftDecodeInfo.putInfo.dataLen += ret;
            g_cbtScmSoftDecodeInfo.putInfo.num++;

            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * 功能描述: GU CBT数据接收函数
 * 修改历史:
 * 1. 日    期: 2014年5月21日
 *    修改内容: Creat Function
 */
VOS_UINT32 CBTSCM_SoftDecodeDataRcv(VOS_UINT8 *buffer, VOS_UINT32 len)
{
    VOS_UINT32 rstl;
    VOS_ULONG  lockLevel;

    VOS_SpinLockIntLock(&g_cbtScmDataRcvSpinLock, lockLevel);

    rstl = CBTSCM_SoftDecodeDataRcvProc(buffer, len);

    VOS_SpinUnlockIntUnlock(&g_cbtScmDataRcvSpinLock, lockLevel);

    return rstl;
}


VOS_UINT32 CBTSCM_SoftDecodeAcpuRcvData(const VOS_UINT8 *data, VOS_UINT32 len)
{
    CBT_EnDeMsg *msg = VOS_NULL_PTR;

    msg = (CBT_EnDeMsg *)VOS_AllocMsg(ACPU_PID_CBT,
        sizeof(CBT_EnDeMsg) - VOS_MSG_HEAD_LENGTH + len);

    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    VOS_SET_RECEIVER_ID(msg, CCPU_PID_CBT);

    msg->msgName = CBT_EN_DE_MSG;
    msg->dataLen = len;

    (VOS_VOID)memcpy_s(msg->data, len, data, len);

    if (VOS_SendMsg(ACPU_PID_CBT, msg) != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}

/*
 * 功能描述: SCM软解码CBT数据接收任务
 * 修改历史:
 * 1. 日    期: 2014年5月21日
 *    修改内容: Creat Function
 */
VOS_VOID CBTSCM_SoftDecodeReqRcvSelfTask(VOS_UINT32 para1, VOS_UINT32 para2,
    VOS_UINT32 para3, VOS_UINT32 para4)
{
    VOS_INT32 ret, len, remainlen, readLen;
    VOS_UINT32 i, rslt, pktNum;
    VOS_ULONG lockLevel;

#if (VOS_OS_VER == VOS_WIN32)
    VOS_UINT32 j;
#endif

#if (VOS_OS_VER == VOS_WIN32)
    for (j = 0; j < 4; j++) {
#else
    for (;;) {
#endif

        if (VOS_SmP(g_cbtScmDataRcvTaskCtrlInfo.smId, 0) != VOS_OK) {
            continue;
        }

        len = OM_RingBufferNBytes(g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId);
        if (len <= 0) {
            continue;
        }

        pktNum = (VOS_UINT32)((len + CBTSCM_DATA_RCV_PKT_SIZE - 1) / CBTSCM_DATA_RCV_PKT_SIZE);

        remainlen = len;

        for (i = 0; i < pktNum; i++) {
            if (remainlen > CBTSCM_DATA_RCV_PKT_SIZE) {
                readLen = CBTSCM_DATA_RCV_PKT_SIZE;

                ret = OM_RingBufferGet(g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId, g_cbtScmDataRcvTaskCtrlInfo.buffer,
                    CBTSCM_DATA_RCV_PKT_SIZE);
            } else {
                readLen = remainlen;

                ret = OM_RingBufferGet(g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId, g_cbtScmDataRcvTaskCtrlInfo.buffer,
                    remainlen);
            }

            if (ret != readLen) {
                VOS_SpinLockIntLock(&g_cbtScmDataRcvSpinLock, lockLevel);

                OM_RingBufferFlush(g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId);

                VOS_SpinUnlockIntUnlock(&g_cbtScmDataRcvSpinLock, lockLevel);

                g_cbtScmSoftDecodeInfo.rbInfo.ringBufferFlush++;

                continue;
            }

            remainlen -= readLen;

            /* 调用HDLC解封装函数 */
            rslt = CBTSCM_SoftDecodeAcpuRcvData((VOS_UINT8 *)g_cbtScmDataRcvTaskCtrlInfo.buffer, (VOS_UINT32)readLen);
            if (rslt != VOS_OK) {
                mdrv_err("<CBTSCM_SoftDecodeReqRcvSelfTask> CBTSCM_SoftDecodeAcpuRcvData Fail\n");

                g_cbtScmSoftDecodeInfo.getInfo.dataLen += readLen;
            }
        }
    }
}

/*
 * 功能描述: SCM软解码CBT数据接收函数初始化
 * 修改历史:
 * 1. 日    期: 2014年5月21日
 *    修改内容: Creat Function
 */
VOS_UINT32 CBTSCM_SoftDecodeReqRcvTaskInit(VOS_VOID)
{
    VOS_UINT8 selfTaskIndex;

    /* 注册CBT数据接收自处理任务 */
    selfTaskIndex = VOS_RegisterSelfTaskPrio(ACPU_FID_CBT, (VOS_TaskEntryType)CBTSCM_SoftDecodeReqRcvSelfTask,
                                    SCM_DATA_RCV_SELFTASK_PRIO, SCM_CBT_REQ_TASK_STACK_SIZE);
    if (selfTaskIndex == VOS_NULL_BYTE) {
        return VOS_ERR;
    }

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
    if (VOS_RegisterSelfTaskCoreBind(ACPU_FID_CBT, selfTaskIndex, VOS_GetAllCoresMask()) != VOS_OK) {
        return VOS_ERR;
    }
#endif

    (VOS_VOID)memset_s(&g_cbtScmSoftDecodeInfo, sizeof(g_cbtScmSoftDecodeInfo), 0, sizeof(g_cbtScmSoftDecodeInfo));

    if (VOS_SmCCreate("CBT", 0, VOS_SEMA4_FIFO, &(g_cbtScmDataRcvTaskCtrlInfo.smId)) != VOS_OK) {
        mdrv_err("<CBTSCM_SoftDecodeReqRcvTaskInit> CBT semCCreate Fail");

        g_cbtScmSoftDecodeInfo.rbInfo.semCreatErr++;

        return VOS_ERR;
    }

    g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId = OM_RingBufferCreate(CBTSCM_DATA_RCV_BUFFER_SIZE);

    if (g_cbtScmDataRcvTaskCtrlInfo.rngOmRbufId == VOS_NULL_PTR) {
        mdrv_err("<CBTSCM_SoftDecodeReqRcvTaskInit> Creat CBT ringBuffer Fail");

        g_cbtScmSoftDecodeInfo.rbInfo.ringBufferCreatErr++;

        return VOS_ERR;
    }

    g_cbtScmDataRcvTaskCtrlInfo.buffer = &g_cbtScmDataRcvBuffer[0];

    VOS_SpinLockInit(&g_cbtScmDataRcvSpinLock);

    return VOS_OK;
}

VOS_VOID CBTSCM_SoftDecodeInfoShow(VOS_VOID)
{
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> SCM_SoftDecodeInfoShow start:\n");

    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Sem Creat Error=%d\n", g_cbtScmSoftDecodeInfo.rbInfo.semCreatErr);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Sem Give Error=%d\n", g_cbtScmSoftDecodeInfo.rbInfo.semGiveErr);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer Creat Error=%d\n",
               g_cbtScmSoftDecodeInfo.rbInfo.ringBufferCreatErr);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Task Id Error=%d\n", g_cbtScmSoftDecodeInfo.rbInfo.taskIdErr);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer not Enough=%d\n",
               g_cbtScmSoftDecodeInfo.rbInfo.bufferNotEnough);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer Flush=%d\n", g_cbtScmSoftDecodeInfo.rbInfo.ringBufferFlush);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer Put Error=%d\n",
               g_cbtScmSoftDecodeInfo.rbInfo.ringBufferPutErr);

    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer Put Success Times=%d\n", g_cbtScmSoftDecodeInfo.putInfo.num);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer Put Success Bytes=%d\n",
               g_cbtScmSoftDecodeInfo.putInfo.dataLen);

    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer Get Success Times=%d\n", g_cbtScmSoftDecodeInfo.getInfo.num);
    mdrv_debug("<CBTSCM_SoftDecodeInfoShow> Ring Buffer Get Success Bytes=%d\n",
               g_cbtScmSoftDecodeInfo.getInfo.dataLen);
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
