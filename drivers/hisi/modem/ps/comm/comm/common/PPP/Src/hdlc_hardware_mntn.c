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

#include "hdlc_hardware_mntn.h"
#ifndef MODEM_FUSION_VERSION
#include "msp_diag_comm.h"
#else
#include "mdrv_diag.h"
#endif
#include "hdlc_hardware_conf.h"
#include "ttf_comm.h"
#include "gucttf_tag.h"
#include "ttf_util.h"
#include "securec.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_HDLC_HARDWARE_MNTN_C
#define THIS_MODU mod_ppp

#if (FEATURE_PPP_HDLC_HARDWARE == FEATURE_ON)

/* 统计信息 */
PPP_HDLC_HardDataProcStat g_pppHdlcHardStat = {0};

/* 可维可测等级配置 */
STATIC VOS_UINT32 g_hdlcMntnConfig = 0;

/* 保留清原始中断时的RAW_INT和STATUS值 */
HDLC_RegSaveInfo g_hdlcRegSaveInfo;

/* 获取可维可测等级 */
VOS_BOOL PPP_HDLC_HardMntnGetConfig(VOS_UINT32 mod)
{
    if ((g_hdlcMntnConfig & mod) == 0) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/* 基础勾包函数 */
VOS_VOID PPP_HDLC_HardMntnTraceMsg(HDLC_MNTN_TraceHead *head, HDLC_MNTN_EventTypeUint32 msgname, VOS_UINT32 dataLen)
{
    TTF_SET_MSG_SENDER_ID(head, PS_PID_PPP_HDLC);
    TTF_SET_MSG_RECEIVER_ID(head, PS_PID_PPP_HDLC);
    TTF_SET_MSG_LEN(head, dataLen - VOS_MSG_HEAD_LENGTH);

    head->msgname = msgname;

    mdrv_diag_trace_report(head);

    return;
}

/* 勾取单个数据 */
VOS_VOID PPP_HDLC_HardMntnTraceSingleData(VOS_UINT16 dataLen, const VOS_UINT8 *dataAddr,
    HDLC_MNTN_EventTypeUint32 eventType, VOS_UINT32 nodeIndex)
{
    VOS_UINT32          allocLen;
    HDLC_MNTN_NodeData *nodeData = VOS_NULL_PTR;
    VOS_UINT32          traceDataLen;

    traceDataLen = TTF_MIN(dataLen, HDLC_MNTN_ALLOC_MEM_MAX_SIZE);

    /* 消息长度等于消息结构大小加数据内容长度 */
    allocLen = traceDataLen + sizeof(HDLC_MNTN_NodeData);

    nodeData = (HDLC_MNTN_NodeData*)PS_MEM_ALLOC(PS_PID_PPP_HDLC, allocLen);

    if (nodeData == VOS_NULL_PTR) {
        PPP_HDLC_WARNING_LOG1("Alloc mem failed", eventType);
        return;
    }

    /* 用于标识这是一组输入链表中的第几个IP包 */
    nodeData->nodeIndex = (VOS_UINT16)nodeIndex;
    nodeData->len       = dataLen;

    if (traceDataLen > 0) {
        /*lint -e426*/
        TTF_SF_CHK(memcpy_s((VOS_UINT8*)(nodeData + 1), traceDataLen, dataAddr, traceDataLen));
        /*lint +e426*/
    }

    PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)nodeData, eventType, allocLen);
    PS_MEM_FREE(PS_PID_PPP_HDLC, nodeData);

    return;
}

#ifdef CONFIG_UART_SHELL
/* 设置可维可测等级 */
MODULE_EXPORTED VOS_UINT32 PPP_HDLC_HardMntnSetConfig(VOS_UINT32 config)
{
    g_hdlcMntnConfig = config;

    return g_hdlcMntnConfig;
}

/* 打印统计信息 */
MODULE_EXPORTED VOS_VOID PPP_HDLC_HardMntnShowStatInfo(VOS_VOID)
{
    TTF_PRINT_DEBUG("HDLC Hardware STAT INFO Begin\n");

    TTF_PRINT_DEBUG("ulDefIpDataProcCnt            = %d\n", g_pppHdlcHardStat.defIpDataProcCnt);
    TTF_PRINT_DEBUG("ulDefPppDataProcCnt           = %d\n", g_pppHdlcHardStat.defPppDataProcCnt);
    TTF_PRINT_DEBUG("ulDefUncompleteCnt            = %d\n", g_pppHdlcHardStat.defUncompleteCnt);
    TTF_PRINT_DEBUG("ulDefWaitIntCnt               = %d\n", g_pppHdlcHardStat.defWaitIntCnt);
    TTF_PRINT_DEBUG("ulDefWaitQueryCnt             = %d\n", g_pppHdlcHardStat.defWaitQueryCnt);
    TTF_PRINT_DEBUG("ulDefIsrCnt                   = %d\n", g_pppHdlcHardStat.defIsrCnt);
    TTF_PRINT_DEBUG("ulDefLcpPauseCnt              = %d\n", g_pppHdlcHardStat.defLcpPauseCnt);
    TTF_PRINT_DEBUG("ulDefFullPauseCnt             = %d\n", g_pppHdlcHardStat.defFullPauseCnt);
    TTF_PRINT_DEBUG("ulDefInputDiscardCnt          = %d\n", g_pppHdlcHardStat.defInputDiscardCnt);

    TTF_PRINT_DEBUG("ulFrmIpDataProcCnt            = %d\n", g_pppHdlcHardStat.frmIpDataProcCnt);
    TTF_PRINT_DEBUG("ulFrmPppDataProcCnt           = %d\n", g_pppHdlcHardStat.frmPppDataProcCnt);
    TTF_PRINT_DEBUG("ulFrmWaitIntCnt               = %d\n", g_pppHdlcHardStat.frmWaitIntCnt);
    TTF_PRINT_DEBUG("ulFrmWaitQueryCnt             = %d\n", g_pppHdlcHardStat.frmWaitQueryCnt);
    TTF_PRINT_DEBUG("ulFrmIsrCnt                   = %d\n", g_pppHdlcHardStat.frmIsrCnt);
    TTF_PRINT_DEBUG("ulFrmAllocOutputMemFailCnt    = %d\n", g_pppHdlcHardStat.frmAllocOutputMemFailCnt);
    TTF_PRINT_DEBUG("ulFrmAllocFirstMemFailCnt     = %d\n", g_pppHdlcHardStat.frmAllocFirstMemFailCnt);
    TTF_PRINT_DEBUG("ulFrmOutputLinkFullCnt        = %d\n", g_pppHdlcHardStat.frmOutputLinkFullCnt);
    TTF_PRINT_DEBUG("ulFrmInputDiscardCnt          = %d\n", g_pppHdlcHardStat.frmInputDiscardCnt);

    TTF_PRINT_DEBUG("ulDefMaxInputCntOnce          = %d\n", g_pppHdlcHardStat.defMaxInputCntOnce);
    TTF_PRINT_DEBUG("ulDefMaxInputSizeOnce         = %d\n", g_pppHdlcHardStat.defMaxInputSizeOnce);
    TTF_PRINT_DEBUG("ulDefMaxValidCntOnce          = %d\n", g_pppHdlcHardStat.defMaxValidCntOnce);
    TTF_PRINT_DEBUG("ulDefMaxQueryCnt              = %d\n", g_pppHdlcHardStat.defMaxQueryCnt);

    TTF_PRINT_DEBUG("ulFrmMaxInputCntOnce          = %d\n", g_pppHdlcHardStat.frmMaxInputCntOnce);
    TTF_PRINT_DEBUG("ulFrmMaxInputSizeOnce         = %d\n", g_pppHdlcHardStat.frmMaxInputSizeOnce);
    TTF_PRINT_DEBUG("ulFrmMaxOutputCntOnce         = %d\n", g_pppHdlcHardStat.frmMaxOutputCntOnce);
    TTF_PRINT_DEBUG("ulFrmMaxOutputSizeOnce        = %d\n", g_pppHdlcHardStat.frmMaxOutputSizeOnce);
    TTF_PRINT_DEBUG("ulFrmMaxQueryCnt              = %d\n", g_pppHdlcHardStat.frmMaxQueryCnt);

    TTF_PRINT_DEBUG("ulMaxCntOnce                  = %d\n", g_pppHdlcHardStat.maxCntOnce);
    TTF_PRINT_DEBUG("ulHdlcHardProcCnt             = %d\n", g_pppHdlcHardStat.hdlcHardProcCnt);
    TTF_PRINT_DEBUG("ulContinueCnt                 = %d\n", g_pppHdlcHardStat.continueCnt);
    TTF_PRINT_DEBUG("usDefExpInfo                  = %d\n", g_pppHdlcHardStat.defExpInfo);
    TTF_PRINT_DEBUG("usFrmExpInfo                  = %d\n", g_pppHdlcHardStat.frmExpInfo);

    TTF_PRINT_DEBUG("ulForbidHdlcBugNoCpy          = %d\n", g_pppHdlcHardStat.forbidHdlcBugNoCpy);
    TTF_PRINT_DEBUG("ulForbidHdlcBugCpy            = %d\n", g_pppHdlcHardStat.forbidHdlcBugCpy);

    TTF_PRINT_DEBUG("HDLC Hardware STAT INFO End\n");

    return;
}

/* 帮助打印 */
MODULE_EXPORTED VOS_VOID PPP_HDLC_HardHelp(VOS_VOID)
{
    TTF_PRINT_DEBUG("PPP Debug Info\n");
    TTF_PRINT_DEBUG("PPP_HDLC_HardMntnShowStatInfo\n");
    TTF_PRINT_DEBUG("PPP_InputShowStatInfo\n");
    TTF_PRINT_DEBUG("PPP_HDLC_HardMntnSetConfig:\n");
    TTF_PRINT_DEBUG("1--para; 2--register; 4--data\n");
    TTF_PRINT_DEBUG("PPP_DriverHdlcHardShowDefReg\n");
    TTF_PRINT_DEBUG("PPP_DriverHdlcHardShowFrmReg\n");
    TTF_PRINT_DEBUG("PPP_DriverHdlcHardSetDefIntLimit\n");
    TTF_PRINT_DEBUG("PPP_DriverHdlcHradSetFrmIntLimit\n");
    TTF_PRINT_DEBUG("PPP_DriverHdlcHardShowConfigInfo\n");

    return;
}
#endif
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
