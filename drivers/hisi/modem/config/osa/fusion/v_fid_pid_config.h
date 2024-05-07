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
 * ��Ȩ���� (c) ��Ϊ�������޹�˾ 2012-2019
 * ��������: OSA PID Table
 * ��������: 2019-07-10
 */
#ifndef _V_FID_PID_CONFIG
#define _V_FID_PID_CONFIG

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "product_config.h"

extern VOS_UINT32 MOD_RegFidPidRSP(VOS_UINT32 pid, VOS_UINT32 fid, VOS_INT priority, MODEM_ID_ENUM_UINT16 modemId);

/*********************** INNER VARIABLES DEFINITION **************************/
/* PID��FID��Ӧ��ϵ�� */
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
BEGIN_DECLARE_PID_BELONG_TO_FID()
    ON_DECLARE(I0_WUEPS_PID_USIM,               I0_MAPS_USIMM_FID,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_MAPS_STK_PID,                 I0_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_MAPS_PIH_PID,                 I0_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_MAPS_PB_PID,                  I0_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_MAPS_EMAT_PID,                I0_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_0)
#if (FEATURE_UE_MODE_G == FEATURE_ON)
    ON_DECLARE(I0_UEPS_PID_LL,                  I0_UEPS_FID_SL,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_SN,                  I0_UEPS_FID_SL,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_GAS,                 I0_UEPS_FID_RR,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_GRM,                 I0_UEPS_FID_GRM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_CST,                    UEPS_FID_DC,            VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_DL,                  I0_UEPS_FID_DC,         VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
#if (FEATURE_UE_MODE_W == FEATURE_ON)
    ON_DECLARE(I0_WUEPS_PID_RLC,                I0_UEPS_FID_RM,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_MAC,                I0_UEPS_FID_RM,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_MACDL,              I0_UEPS_FID_RM_DL,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_RLCDL,              I0_UEPS_FID_RM_DL,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_BMC,                I0_UEPS_FID_PB,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_PDCP,               I0_UEPS_FID_PB,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_WRR,                I0_UEPS_FID_WRR,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_WCOM,               I0_UEPS_FID_WRR,        VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
    ON_DECLARE(I0_PS_PID_MM,                    I0_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_REGM,                I0_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_MMC,                I0_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_MM,                 I0_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_GMM,                I0_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_NAS_COMM,              I0_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_SMT,                   WUEPS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_CBS,                   WUEPS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_CC,                 I0_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_SS,                 I0_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_SMS,                I0_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_TC,                 I0_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_RABM,               I0_WUEPS_FID_RABMSM,    VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_SM,                 I0_WUEPS_FID_RABMSM,    VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEAPP_PID_MUX,                  WUEPS_FID_TAF,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_ADMIN,              I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_TAF,                I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_VC,                 I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_DRV_AGENT,          I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_MTA,                 I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_XSMS,                I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_MMA,                I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_DSM,                 I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_SAR,                   WUEPS_FID_SPY,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_SPY,                   WUEPS_FID_SPY,          VOS_PRIORITY_NULL,    MODEM_ID_0)
#ifdef PAM_BT_ENABLED
    ON_DECLARE(PAM_PID_NBT,                     PAM_FID_NBT,            VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
    ON_DECLARE(UEPS_PID_CDS,                    UEPS_FID_CDS,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_APP_OM,                 UEPS_FID_NUP_APP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_APP_NR_DT,           UEPS_FID_NUP_APP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I1_UEPS_PID_APP_NR_DT,           UEPS_FID_NUP_APP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_APP_LTE_DT,          UEPS_FID_NUP_APP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I1_UEPS_PID_APP_LTE_DT,          UEPS_FID_NUP_APP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_APP_NR_CHR,             UEPS_FID_NUP_APP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_APP_LTE_CHR,            UEPS_FID_NUP_APP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_NDCLIENT,               UEPS_FID_NDCLIENT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_DHCP,                   UEPS_FID_NDCLIENT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_IPS_CCORE,              UEPS_FID_IPS_CCORE,     VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_CSIMA,              I0_WUEPS_FID_CSIMA,     VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_CMMCA,                 WUEPS_FID_CMMCA,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CCPU_PID_CBT,                    CCPU_FID_CBT,           VOS_PRIORITY_NULL,    MODEM_ID_0)
#ifdef PAM_CT_ENABLED
    ON_DECLARE(CCPU_PID_PAM_MFG,                CCPU_FID_CBT,           VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
    ON_DECLARE(CCPU_PID_PAM_OM,                 CCPU_FID_PAM_OM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_REG,                   CCPU_FID_PAM_OM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_MTC,                    UEPS_FID_MTC,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_MSCC,                I0_UEPS_FID_MSCC,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(PS_PID_CSS,                      UEPS_FID_CSS,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_RRM,                    UEPS_FID_RRM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_XPDS,                I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_CCM,                 I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_GUCALL,              I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_XCALL,               I0_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_0)
#if ( FEATURE_MULTI_MODEM == FEATURE_ON )
    ON_DECLARE(I1_WUEPS_PID_USIM,               I1_MAPS_USIMM_FID,      VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_MAPS_STK_PID,                 I1_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_MAPS_PIH_PID,                 I1_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_MAPS_PB_PID,                  I1_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_MAPS_EMAT_PID,                I1_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_MM,                 I1_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_REGM,                I1_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_MMC,                I1_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_GMM,                I1_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_NAS_COMM,              I1_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_CC,                 I1_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_SS,                 I1_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_TC,                 I1_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_SMS,                I1_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_RABM,               I1_WUEPS_FID_RABMSM,    VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_SM,                 I1_WUEPS_FID_RABMSM,    VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_ADMIN,              I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_TAF,                I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_VC,                 I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_MTA,                 I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_DRV_AGENT,          I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_XSMS,                I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_MMA,                I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_DSM,                 I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
#if (FEATURE_UE_MODE_G == FEATURE_ON)
    ON_DECLARE(I1_UEPS_PID_GAS,                 I1_UEPS_FID_RR,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_LL,                  I1_UEPS_FID_SL,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_SN,                  I1_UEPS_FID_SL,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_DL,                  I1_UEPS_FID_DC,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_GRM,                 I1_UEPS_FID_GRM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
#endif
    ON_DECLARE(I1_UEPS_PID_MSCC,                I1_UEPS_FID_MSCC,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_CSIMA,              I1_WUEPS_FID_CSIMA,     VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_XPDS,                I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_LCS,                I1_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
#if ( FEATURE_MODEM1_SUPPORT_WCDMA == FEATURE_ON )
#if (FEATURE_UE_MODE_W == FEATURE_ON)
    ON_DECLARE(I1_WUEPS_PID_WRR,                I1_UEPS_FID_WRR,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_WCOM,               I1_UEPS_FID_WRR,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_RLC,                I1_UEPS_FID_RM,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_MAC,                I1_UEPS_FID_RM,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_MACDL,              I1_UEPS_FID_RM_DL,      VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_RLCDL,              I1_UEPS_FID_RM_DL,      VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_BMC,                I1_UEPS_FID_PB,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_WUEPS_PID_PDCP,               I1_UEPS_FID_PB,         VOS_PRIORITY_NULL,    MODEM_ID_1)
#endif
#endif

#if ( FEATURE_MODEM1_SUPPORT_LTE == FEATURE_ON)
    ON_DECLARE(I1_MSP_L4_L4A_PID,               I1_MSP_L4_FID,          VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_MSP_LDSP_PID,                 I1_MSP_L4_FID,          VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_MSP_SYS_FTM_PID,              I1_MSP_FTM_FID,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_ERRC,                  I1_PS_FID_RRC,          VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_ERMM,                  I1_PS_FID_RRC,          VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_ESM,                   I1_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_RABM,                  I1_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_TC,                    I1_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_IP,                    I1_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_PTT,                   I1_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_LPP,                   I1_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_OM,                    I1_PS_FID_OM,           VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_MAC_UL,             I0_UEPS_FID_NMACRLC_UL_RT, VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_RLC_UL,             I0_UEPS_FID_NMACRLC_UL_RT, VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_PDCP_UL,            I0_UEPS_FID_NPDCPRLC_UL,   VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_PDCP_DL,               I0_UEPS_FID_NPDCP_DL,   VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_RLC_DL,                I0_UEPS_FID_RLC_DL,     VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_MAC_DL,                I0_UEPS_FID_MAC_DL,     VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_IMSA,                  I1_PS_FID_IMSA,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_SCC,                   I1_PS_FID_IMSA,         VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_IMSVA,                 I1_PS_FID_IMSVA,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_PS_PID_MM,                    I1_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
#endif
    ON_DECLARE(I1_UEPS_PID_CCM,                 I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_GUCALL,              I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_XCALL,               I1_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_1)

    ON_DECLARE(I2_WUEPS_PID_USIM,               I2_MAPS_USIMM_FID,      VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_MAPS_STK_PID,                 I2_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_MAPS_PIH_PID,                 I2_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_MAPS_PB_PID,                  I2_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_MAPS_EMAT_PID,                I2_MAPS_SI_FID,         VOS_PRIORITY_NULL,    MODEM_ID_2)

#if ( 3 == MULTI_MODEM_NUMBER )/* three modems */
    ON_DECLARE(I2_WUEPS_PID_MM,                 I2_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_REGM,                I2_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_MMC,                I2_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_GMM,                I2_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_PS_PID_NAS_COMM,              I2_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_CC,                 I2_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_SS,                 I2_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_TC,                 I2_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_SMS,                I2_WUEPS_FID_CM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_RABM,               I2_WUEPS_FID_RABMSM,    VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_SM,                 I2_WUEPS_FID_RABMSM,    VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_ADMIN,              I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_TAF,                I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_VC,                 I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_MTA,                 I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_DRV_AGENT,          I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_XSMS,                I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_MMA,                I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_DSM,                 I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
#if (FEATURE_UE_MODE_G == FEATURE_ON)
    ON_DECLARE(I2_UEPS_PID_GAS,                 I2_UEPS_FID_RR,         VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_LL,                  I2_UEPS_FID_SL,         VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_SN,                  I2_UEPS_FID_SL,         VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_DL,                  I2_UEPS_FID_DC,         VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_GRM,                 I2_UEPS_FID_GRM,        VOS_PRIORITY_NULL,    MODEM_ID_2)
#endif
    ON_DECLARE(I2_UEPS_PID_MSCC,                I2_UEPS_FID_MSCC,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_CSIMA,              I2_WUEPS_FID_CSIMA,     VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_XPDS,                I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_WUEPS_PID_LCS,                I2_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_CCM,                 I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_GUCALL,              I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
    ON_DECLARE(I2_UEPS_PID_XCALL,               I2_WUEPS_FID_TAF,       VOS_PRIORITY_NULL,    MODEM_ID_2)
#endif
#endif

    ON_DECLARE(I0_MSP_L4_L4A_PID,               I0_MSP_L4_FID,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_MSP_LDSP_PID,                 I0_MSP_L4_FID,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_MSP_SYS_FTM_PID,              I0_MSP_FTM_FID,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_ERRC,                  I0_PS_FID_RRC,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_ERMM,                  I0_PS_FID_RRC,          VOS_PRIORITY_NULL,    MODEM_ID_0)
#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
    ON_DECLARE(TPS_PID_RRC,                     I0_PS_FID_RRC,          VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
    ON_DECLARE(I0_PS_PID_ESM,                   I0_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_RABM,                  I0_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_TC,                    I0_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_IP,                    I0_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_PTT,                   I0_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_LPP,                   I0_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_WUEPS_PID_LCS,                I0_PS_FID_CM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_OM,                    I0_PS_FID_OM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_MAC_UL,             I0_UEPS_FID_NMACRLC_UL_RT, VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_RLC_UL,             I0_UEPS_FID_NMACRLC_UL_RT, VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_PDCP_UL,            I0_UEPS_FID_NPDCPRLC_UL,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_IMSA,                  I0_PS_FID_IMSA,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_SCC,                   I0_PS_FID_IMSA,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_IMSVA,                 I0_PS_FID_IMSVA,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_PSUPMEM,                UEPS_FID_PSUPMEM,       VOS_PRIORITY_NULL,    MODEM_ID_0)

#if (FEATURE_ON == FEATURE_EDA_SUPPORT)
    ON_DECLARE(WUEPS_PID_EDA,                   PS_FID_EDA,             VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
    ON_DECLARE(HL1_PID_SNDCMD,                  HL1_FID_SNDCMD,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(TPS_PID_RLC,                     TPS_FID_MAC,            VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(TPS_PID_MAC,                     TPS_FID_MAC,            VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(TPS_PID_PDC,                     TPS_FID_PDC,            VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    ON_DECLARE(UEPS_PID_1XCASM,                 UEPS_FID_CAS1X,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_1XCMEAS,                UEPS_FID_CAS1X,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_1XCSRCH,                UEPS_FID_CAS1X,         VOS_PRIORITY_NULL,    MODEM_ID_0)

    ON_DECLARE(UEPS_PID_HALMP,                  UEPS_FID_CASHRPD,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_HSP,                    UEPS_FID_CASHRPD,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_HRUP,                   UEPS_FID_CASHRPD,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_HSCP,                   UEPS_FID_CASHRPD,       VOS_PRIORITY_NULL,    MODEM_ID_0)

    ON_DECLARE(MSPS_PID_1X_RLAC,                MSPS_FID_CTTF_1X_REV,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_1X_RMAC,                MSPS_FID_CTTF_1X_REV,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_1X_RRLP,                MSPS_FID_CTTF_1X_REV,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_1X_RTESTSO,             MSPS_FID_CTTF_1X_REV,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_1X_FLAC,                MSPS_FID_CTTF_1X_FWD,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_1X_FMAC,                MSPS_FID_CTTF_1X_FWD,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_1X_FRLP,                MSPS_FID_CTTF_1X_FWD,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_1X_FTESTSO,             MSPS_FID_CTTF_1X_FWD,   VOS_PRIORITY_NULL,    MODEM_ID_0)

    ON_DECLARE(CTTF_PID_HRPD_RSLP,              CTTF_FID_HRPD_REV,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_RPA,               CTTF_FID_HRPD_REV,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_RSPS,              CTTF_FID_HRPD_REV,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_RMAC,              CTTF_FID_HRPD_REV,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_FSLP,              CTTF_FID_HRPD_FWD,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_FPA,               CTTF_FID_HRPD_FWD,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_FSPS,              CTTF_FID_HRPD_FWD,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_FMAC,              CTTF_FID_HRPD_FWD,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CTTF_PID_HRPD_SIG,               CTTF_FID_HRPD_SIG,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_1X_CM,                 CPROC_FID_1X,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_1X_SM,                 CPROC_FID_1X,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_1X_CTAS,               CPROC_FID_1X,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_1X_DSP,                CPROC_FID_1X,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_EVDO,                  CPROC_FID_HRPD,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_HRPD_CM,               CPROC_FID_HRPD,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_HRPD_SM,               CPROC_FID_HRPD,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(CPROC_PID_RM,                    CPROC_FID_RM,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_XREG,                   UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_XCC,                    UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_XSD,                    UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_HSD,                    UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_HSM,                    UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_EHSM,                   UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_HLU,                    UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_HRM,                    UEPS_FID_CNAS,          VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    ON_DECLARE(MSPS_PID_PPPC,                   MSPS_FID_PPPC,          VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(MSPS_PID_MIP,                    MSPS_FID_PPPC,          VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if (FEATURE_ON == FEATURE_BASTET)
    ON_DECLARE(UEPS_PID_BASTET,                 UEPS_FID_BASTET,        VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if defined( FEATURE_NPHY_START)
    ON_DECLARE(PHY_PID_STARTUP,                 NPHY_FID_STARTUP,       VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
#if (VOS_WIN32 == VOS_OS_VER)
    ON_DECLARE(WUEPS_PID_ITTSTUB1,              WUEPS_FID_ITTSTUB,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_ITTSTUB2,              WUEPS_FID_ITTSTUB,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_WTTFTEST,              WUEPS_FID_ITTSTUB,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(WUEPS_PID_OAMTEST,               WUEPS_FID_ITTSTUB,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_PAM_OM,                 ACPU_FID_PAM_OM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
#if (FEATURE_ON == FEATURE_PC5_DATA_CHANNEL)
    ON_DECLARE(I0_PS_PID_VERRC,                 I0_PS_FID_VRRC,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VSYNC,                 I0_PS_FID_VRRC,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VMAC_UL,               I0_PS_FID_VRM_UL,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VRLC_UL,               I0_PS_FID_VRM_UL,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VPDCP_UL,              I0_PS_FID_VRM_UL,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VMAC_DL,               I0_PS_FID_VRM_DL,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VRLC_DL,               I0_PS_FID_VRM_DL,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VRBM_DL,               I0_PS_FID_VRM_DL,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VPDCP_DL,              I0_PS_FID_VRM_DL,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VNAS,                  I0_PS_FID_VNAS,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_VTC,                   I0_PS_FID_VNAS,         VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    ON_DECLARE(I0_UEPS_PID_NRRC,                I0_UEPS_FID_NRRC,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NRRM,                I0_UEPS_FID_NRRC,       VOS_PRIORITY_NULL,    MODEM_ID_0)
#if (FEATURE_MODEM1_SUPPORT_NR == FEATURE_ON)
    ON_DECLARE(I1_UEPS_PID_NRRC,                I1_UEPS_FID_NRRC,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_NRRM,                I1_UEPS_FID_NRRC,       VOS_PRIORITY_NULL,    MODEM_ID_1)
#endif

#endif

#ifdef MODEM_FUSION_VERSION
    ON_DECLARE(I0_UEPS_PID_NRMM,                I0_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NRSM,                I0_UEPS_FID_NRCM,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_PCF,                 I0_UEPS_FID_NRCM,       VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NREAP,               I0_UEPS_FID_NREAP,      VOS_PRIORITY_NULL,    MODEM_ID_0)
#if (FEATURE_MODEM1_SUPPORT_NR == FEATURE_ON)
    ON_DECLARE(I1_UEPS_PID_NRMM,                I1_WUEPS_FID_MM,        VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_NRSM,                I1_UEPS_FID_NRCM,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_PCF,                 I1_UEPS_FID_NRCM,       VOS_PRIORITY_NULL,    MODEM_ID_1)
    ON_DECLARE(I1_UEPS_PID_NREAP,               I1_UEPS_FID_NREAP,      VOS_PRIORITY_NULL,    MODEM_ID_1)
#endif
#endif

#ifdef PAM_CT_ENABLED
    ON_DECLARE(DSP_PID_BBA_CALC,                DSP_FID_BBA_CALC,               VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
    ON_DECLARE(DSP_PID_BBA_CAL,                 DSP_FID_BBA_CAL,                VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NPDCP_UL_RT,         I0_UEPS_FID_NMACRLC_UL_RT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NRLC_UL_RT,          I0_UEPS_FID_NMACRLC_UL_RT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NMAC_UL_RT,          I0_UEPS_FID_NMACRLC_UL_RT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_L2DS,                   I0_UEPS_FID_NMACRLC_UL_RT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_L2MA,                   I0_UEPS_FID_L2MA,               VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NRLC_UL_NRT,         I0_UEPS_FID_NPDCPRLC_UL,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NPDCP_UL_NRT,        I0_UEPS_FID_NPDCPRLC_UL,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NPDCP_DL,            I0_UEPS_FID_NPDCP_DL,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_PDCP_DL,               I0_UEPS_FID_NPDCP_DL,           VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NRLC_DL,             I0_UEPS_FID_RLC_DL,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_RLC_DL,                I0_UEPS_FID_RLC_DL,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_NMAC_DL,             I0_UEPS_FID_MAC_DL,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_PS_PID_MAC_DL,                I0_UEPS_FID_MAC_DL,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(I0_UEPS_PID_SDAP_UL,             UEPS_FID_CDS,                   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_L2LB,                   UEPS_FID_CDS,                   VOS_PRIORITY_NULL,    MODEM_ID_0)

    ON_DECLARE(MSPS_PID_PPPS,                   MSPS_FID_PPPS,                  VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_PSCOMM,                 UEPS_FID_PSCOMM,                VOS_PRIORITY_NULL,    MODEM_ID_0)

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON || VOS_OS_VER == VOS_WIN32)
    ON_DECLARE(WUEPS_PID_AT,                    PS_FID_DMS,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(PS_PID_DMS,                      PS_FID_DMS,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_TAFAGENT,               ACPU_FID_TAFAGENT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_PCSC,                   ACPU_FID_USIMM,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_PB,                     ACPU_FID_USIMM,         VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
    ON_DECLARE(PS_PID_APP_PPP,                  MSPS_FID_PPPS,              VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(PS_PID_APP_NDIS,                 PS_FID_APP_NDIS_PPP_DIPC,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_NDSERVER,               PS_FID_APP_NDIS_PPP_DIPC,   VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

END_DECLARE_PID_BELONG_TO_FID()

#endif

#if (OSA_CPU_ACPU == VOS_OSA_CPU)
BEGIN_DECLARE_PID_BELONG_TO_FID()
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    ON_DECLARE(PS_PID_APP_NDIS,                 PS_FID_APP_NDIS_PPP_DIPC,   VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(UEPS_PID_NDSERVER,               PS_FID_APP_NDIS_PPP_DIPC,   VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
    ON_DECLARE(ACPU_PID_RNIC,                   ACPU_FID_ADS_UL,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_NFEXT,                  ACPU_FID_NFEXT,         VOS_PRIORITY_NULL,    MODEM_ID_0)
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
    ON_DECLARE(ACPU_PID_ADS_UL,                 ACPU_FID_ADS_UL,        VOS_PRIORITY_NULL,    MODEM_ID_0)
#else
    ON_DECLARE(ACPU_PID_ADS_UL,                 ACPU_FID_ADS_UL,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_ADS_DL,                 ACPU_FID_ADS_DL,        VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif
    ON_DECLARE(ACPU_PID_PAM_OM,                 ACPU_FID_PAM_OM,        VOS_PRIORITY_NULL,    MODEM_ID_0)

#if (FEATURE_VOICE_UP == FEATURE_ON)
    ON_DECLARE(DSP_PID_VOICE_RT,                ACPU_FID_MED_RT,        VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(DSP_PID_VOICE,                   ACPU_FID_MED,           VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if (FEATURE_ON == FEATURE_BASTET)
    ON_DECLARE(ACPU_PID_BASTET_COMM,            ACPU_FID_BASTET_COMM,   VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if ((FEATURE_PPP == FEATURE_ON) && (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF))
    ON_DECLARE(PS_PID_APP_PPP,                  ACPU_FID_PPP,           VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
    ON_DECLARE(ACPU_PID_VOICE_AGENT,            ACPU_FID_VOICE_AGENT,   VOS_PRIORITY_NULL,      MODEM_ID_0)
#endif

#if (FEATURE_HIFI_MANAGE == FEATURE_ON)
    ON_DECLARE(ACPU_PID_HIFI_MANAGE,            ACPU_FID_HIFI_MANAGE,   VOS_PRIORITY_NULL,      MODEM_ID_0)
#endif

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    ON_DECLARE(WUEPS_PID_AT,                    PS_FID_DMS,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(PS_PID_DMS,                      PS_FID_DMS,             VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_TAFAGENT,               ACPU_FID_TAFAGENT,      VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_PCSC,                   ACPU_FID_USIMM,         VOS_PRIORITY_NULL,    MODEM_ID_0)
    ON_DECLARE(ACPU_PID_PB,                     ACPU_FID_USIMM,         VOS_PRIORITY_NULL,    MODEM_ID_0)
#endif

END_DECLARE_PID_BELONG_TO_FID()
#endif

#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */