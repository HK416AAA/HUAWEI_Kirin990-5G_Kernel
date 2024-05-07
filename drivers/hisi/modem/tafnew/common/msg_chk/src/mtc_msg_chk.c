/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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

#include "mtc_msg_chk.h"
#include "taf_msg_chk_api.h"
#include "mtc_nas_interface.h"
#include "mtc_rrc_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_MTC_MSG_CHK_C

#ifndef MODEM_FUSION_VERSION
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
/* UEPS_PID_MTC接收UEPS_PID_NRMM的消息长度检查 */
static TAF_ChkMsgLenNameMapTbl g_mtcChkNrmmMsgLenTbl[] = {
    { ID_NRMM_MTC_BEGIN_SESSION_NOTIFY, sizeof(NRMM_MTC_BeginSessionNotify), VOS_NULL_PTR },
    { ID_NRMM_MTC_END_SESSION_NOTIFY, sizeof(NRMM_MTC_EndSessionNotify), VOS_NULL_PTR }
};

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTC_GetChkNrmmMsgLenTblAddr(VOS_VOID)
{
    return g_mtcChkNrmmMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTC_GetChkNrmmMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtcChkNrmmMsgLenTbl);
}

/* UEPS_PID_MTC接收UEPS_PID_NRRC的消息长度检查 */
static TAF_ChkMsgLenNameMapTbl g_mtcChkNrrcMsgLenTbl[] = {
    { ID_RRC_MTC_NRRC_CELLINFO_IND, sizeof(RRC_MTC_NrrcCellinfoInd), VOS_NULL_PTR },
    { ID_RRC_MTC_NRRC_IMU_CONFIG_IND, sizeof(NRRC_MTC_ImuConfigInd), VOS_NULL_PTR },
    { ID_RRC_MTC_NRRC_IMU_FLUSH_IND, sizeof(NRRC_MTC_ImuFlushInd), VOS_NULL_PTR },
    { ID_RRC_MTC_NRRC_IMU_REPORT_START_IND, sizeof(NRRC_MTC_ImuReportStartInd), VOS_NULL_PTR },
    { ID_RRC_MTC_NRRC_IMU_REPORT_STOP_IND, sizeof(NRRC_MTC_ImuReportStopInd), VOS_NULL_PTR }
};

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTC_GetChkNrrcMsgLenTblAddr(VOS_VOID)
{
    return g_mtcChkNrrcMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTC_GetChkNrrcMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtcChkNrrcMsgLenTbl);
}

/* UEPS_PID_MTC接收消息长度检查 */
static TAF_ChkMsgLenSndPidMapTbl g_mtcChkMsgLenTbl[] = {
    { I0_UEPS_PID_NRMM, TAF_MTC_GetChkNrmmMsgLenTblAddr, TAF_MTC_GetChkNrmmMsgLenTblSize },
    { I1_UEPS_PID_NRMM, TAF_MTC_GetChkNrmmMsgLenTblAddr, TAF_MTC_GetChkNrmmMsgLenTblSize },
    { I0_UEPS_PID_NRRC, TAF_MTC_GetChkNrrcMsgLenTblAddr, TAF_MTC_GetChkNrrcMsgLenTblSize },
    { I1_UEPS_PID_NRRC, TAF_MTC_GetChkNrrcMsgLenTblAddr, TAF_MTC_GetChkNrrcMsgLenTblSize },
};

/* UEPS_PID_MTC接收消息有效长度检查MAP */
static TAF_ChkMsgLenRcvPidMapTbl g_tafMtcFidChkMsgLenTbl[] = {
    TAF_MSG_CHECK_TBL_ITEM(UEPS_PID_MTC, g_mtcChkMsgLenTbl),
};

VOS_VOID TAF_SortMtcFidMsgLenChkTbl(VOS_VOID)
{
    TAF_SortChkMsgLenTblByMsgName(g_mtcChkNrmmMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtcChkNrmmMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtcChkNrrcMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtcChkNrrcMsgLenTbl));
}

VOS_UINT32 TAF_ChkMtcFidRcvMsgLen(const MsgBlock *msg)
{
    return TAF_ChkMsgLen(msg, g_tafMtcFidChkMsgLenTbl, TAF_GET_ARRAY_NUM(g_tafMtcFidChkMsgLenTbl));
}
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

