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

#ifndef __MBB_CUST_API_H__
#define __MBB_CUST_API_H__
/*****************************************头文件********************************************/
#include "product_config.h"
#include "v_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* PLMN和CA对应关系结构体 */
typedef struct {
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
    VOS_CHAR  *caInfo; /* 当前PLMN的CA组合信息 */
} TAF_GlobalPlmnCaInfo;

typedef int (*HWLOCK_SIMLOCK_VERIFY)(const VOS_UINT8 *pwd);
typedef int (*SEC_DATA_INTEGRITY_CHECK)(VOS_VOID);
typedef int (*SEC_AUTH_START)(unsigned char *cipher_out, unsigned int *len_out,
                              unsigned char *random_data, unsigned int random_len);
typedef void (*SEC_GET_PUB_KEY_IDS)(unsigned int *ids);
typedef int (*SEC_AUTH_CONFIRM)(const unsigned char *cipher_in, unsigned int len_in);
typedef void (*SET_SAR_DOWN_STATUS)(unsigned int status);
typedef unsigned int (*GET_SAR_DOWN_STATUS)(void);
typedef VOS_BOOL (*TAF_MTA_CUST_NV_WRITE_REQ)(VOS_UINT32 id);
typedef TAF_NVIM_LteAttachProfileInfo* (*TAF_DSM_GET_CUST_ATTACH_PROFILE_TAB)(VOS_VOID);
typedef VOS_UINT8 (*TAF_DSM_GET_CUST_ATTACH_PROFILE_TAB_COUNT)(VOS_VOID);
typedef TAF_GlobalPlmnCaInfo* (*TAF_MTA_GET_GLOBAL_CACFG_TAB)(VOS_VOID);
typedef VOS_UINT32 (*TAF_MTA_GET_GLOBAL_CA_TAB_NUM)(VOS_VOID);
typedef VOS_BOOL (*TAF_JUDEG_NV_OPERATION_VALID)(VOS_VOID);
typedef VOS_UINT32 (*NAS_TELEMATIC_IS_NEED_SEND_SPEC_MSG)(ModemIdUint16 modemId, VOS_VOID *sndMsg);

typedef struct {
    HWLOCK_SIMLOCK_VERIFY hwlock_simlock_verify;
#if (FEATURE_MBB_CUST == FEATURE_ON)
    SEC_DATA_INTEGRITY_CHECK sec_data_integrity_check;
    SEC_AUTH_START sec_auth_start;
    SEC_GET_PUB_KEY_IDS sec_get_pub_key_ids;
    SEC_AUTH_CONFIRM sec_auth_confirm;
    SET_SAR_DOWN_STATUS set_sar_down_status;
    GET_SAR_DOWN_STATUS get_sar_down_status;
    TAF_MTA_CUST_NV_WRITE_REQ taf_mta_cust_nv_write_req;
    TAF_DSM_GET_CUST_ATTACH_PROFILE_TAB TAF_DSM_GetCustAttachProfileTable;
    TAF_DSM_GET_CUST_ATTACH_PROFILE_TAB_COUNT TAF_DSM_GetCustAttachProfileTableCount;
    TAF_MTA_GET_GLOBAL_CACFG_TAB TAF_MTA_GetGlobalCaCfgTbl;
    TAF_MTA_GET_GLOBAL_CA_TAB_NUM TAF_MTA_GetGlobalCaTblNum;
    TAF_JUDEG_NV_OPERATION_VALID TAF_JudgeNvOperationValid;
#endif
} TAF_MbbCustFuncCtx;

TAF_MbbCustFuncCtx* TAF_SDC_GetMbbCustFunTabAddr(VOS_VOID);
VOS_UINT32 TAF_RegMbbCustFunc(TAF_MbbCustFuncCtx funcTab);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __MBB_LOG_SERVICE_API_H__ */
