/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef __TAF_COMMON_H__
#define __TAF_COMMON_H__

/* **************************************************************************** */
#include "vos.h"
#include "PsTypeDef.h"
#include "ps_common_def.h"
#include "mdrv.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_pm.h"
#endif
#include "ps_tag.h"
#include "v_id.h"
#include "product_config.h"
#include "taf_api.h"
#include "mdrv.h"
#include "errorlog.h"

/* **************************************************************************** */

/* **************************************************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* ======================================================== */
/* Log Print Module Define */
#ifndef THIS_MODU
#define THIS_MODU ps_nas
#endif

/* 标志无语音业务的编译开关，问题单A32D12736 */
#define TAF_FEATURE_NO_VOICE_SERVICE
/*
 * TAFM上报结构中option位段字节总个数，
 * 子模块可以从结构起始地址memset TAF_EVT_OP_LEN个字节达到赋初值目的
 */
#define TAF_EVT_OP_LEN 4

/* ----------TAFM公共接口---------- */
/* TAF各模块ID */
#ifndef SUBMOD_NULL
#define SUBMOD_NULL 0
#endif
#define TAF_CCA 1
#define TAF_APS 2
#define TAF_SMA 3
#define TAF_SSA 4
#define TAF_MMA 5
#define TAF_ALL_SUBMODULE 0xFF /* 所有子模块 */

/* 临时，V200要删除 */
#define TAF_TAFM 0
#define TAF_SPEECH 1
#define TAF_STATUS 2
#define TAF_MAX_SPEECH_TI 14

/* 打印级别，重新映射到OM定义的宏 */
#define INFO_PRINT PS_LOG_LEVEL_INFO
#define NORMAL_PRINT PS_LOG_LEVEL_NORMAL
#define WARNING_PRINT PS_LOG_LEVEL_WARNING
#define ERROR_PRINT PS_LOG_LEVEL_ERROR

#define TAF_PRINT(Module, PrintLevel, Str) TAF_LOG(Module, 0, PrintLevel, Str)

#define TAFM_PRINT(Str) TAF_LOG(TAF_TAFM, 0, ERROR_PRINT, Str)

#define TAFM_PRINT1(Str, Para1) TAF_LOG1(TAF_TAFM, 0, ERROR_PRINT, Str, Para1)

#define TAFM_PRINT2(Str, Para1, Para2) TAF_LOG2(TAF_TAFM, 0, ERROR_PRINT, Str, Para1, Para2)

/* 其他接口 */
/* 参数设置结果上报 */
VOS_VOID TAF_ParaSetResult(VOS_UINT16 clientId, VOS_UINT8 opId, TAF_PARA_SET_RESULT result, TAF_PARA_TYPE queryPara);

/* ***************************************************************************** */

#define SMA_INFO_LOG(String) TAF_LOG(WUEPS_PID_TAF, TAF_SMA, PS_LOG_LEVEL_INFO, String)
#define SMA_NORM_LOG(String) TAF_LOG(WUEPS_PID_TAF, TAF_SMA, PS_LOG_LEVEL_NORMAL, String)
#define SMA_WARN_LOG(String) TAF_LOG(WUEPS_PID_TAF, TAF_SMA, PS_LOG_LEVEL_WARNING, String)
#define SMA_ERR_LOG(String) TAF_LOG(WUEPS_PID_TAF, TAF_SMA, PS_LOG_LEVEL_ERROR, String)
#define SMA_LOG1(String, Para1) TAF_LOG1(WUEPS_PID_TAF, TAF_SMA, PS_LOG_LEVEL_INFO, String, Para1)
#define SMA_LOG2(String, Para1, Para2) TAF_LOG2(WUEPS_PID_TAF, TAF_SMA, PS_LOG_LEVEL_NORMAL, String, Para1, Para2)

/* 有返回值内存申请宏定义 */

#define TAF_COMM_BUILD_USIM_GET_FILE_INFO(pstGetfileInfo, enAppTypePara, usEfIdPara, ucRecordNumPara) do { \
    (pstGetfileInfo)->efId      = usEfIdPara;                                                     \
    (pstGetfileInfo)->recordNum = ucRecordNumPara;                                                \
    (pstGetfileInfo)->appType   = enAppTypePara;                                                  \
    (pstGetfileInfo)->rsv       = 0;                                                              \
} while (0)

#define TAF_COMM_BUILD_USIM_SET_FILE_INFO(pstSetfileInfo, enAppTypePara, usEfIdPara, ucRecordNumPara, ulEfLenPara, \
                                          pucEfPara)                                                               \
    do {                                                                                                           \
        (pstSetfileInfo)->efId      = usEfIdPara;                                                                  \
        (pstSetfileInfo)->recordNum = ucRecordNumPara;                                                             \
        (pstSetfileInfo)->appType   = enAppTypePara;                                                               \
        (pstSetfileInfo)->rsv       = 0;                                                                           \
        (pstSetfileInfo)->efLen     = ulEfLenPara;                                                                 \
        (pstSetfileInfo)->efContent = pucEfPara;                                                                   \
    } while (0)

/* ======================================================== */
#ifdef __cplusplus
}
#endif
/* **************************************************************************** */

/* ============================================================================ */
#endif /* __TAF_COMMON_H__ */
