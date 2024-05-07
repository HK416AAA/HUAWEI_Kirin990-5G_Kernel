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
 * 版权所有 (c) 华为技术有限公司 2012-2019
 * 功能描述: 公共头文件, 定义VOS全系统的PID
 * 生成日期: 2011年7月26日
 */

#ifndef _VOS_PID_DEF_H
#define _VOS_PID_DEF_H

#include "product_config.h"
#include "v_typdef.h"
#include "msg_id.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define VOS_PID_DOPRAEND 5
#define VOS_PID_BUTT     OSA_SYS_MID_BUTT

#if (FEATURE_MULTI_MODEM == FEATURE_ON)

#if (3 == MULTI_MODEM_NUMBER) /* three modems */

enum ModemId {
    MODEM_ID_0 = 0,
    MODEM_ID_1,
    MODEM_ID_2,
    MODEM_ID_BUTT
};

#else /* two modems */

enum ModemId {
    MODEM_ID_0 = 0,
    MODEM_ID_1,
    MODEM_ID_BUTT,
    MODEM_ID_2
};

#endif

#else /* only one */

enum ModemId {
    MODEM_ID_0 = 0,
    MODEM_ID_BUTT,
    MODEM_ID_1,
    MODEM_ID_2
};

#endif

typedef VOS_UINT16 MODEM_ID_ENUM_UINT16;
typedef VOS_UINT16 ModemIdUint16;

/*
 * 功能说明: 接入模式定义
 * 注意；双卡双待的枚举值定义与协议栈和DRV协商为GULT;
 * 需要和DrvInterface.h中的PWC_COMM_MODE_E保持一致
 */
enum VOS_RatMode {
    VOS_RATMODE_GSM   = 0,
    VOS_RATMODE_WCDMA = 1,
    VOS_RATMODE_LTE   = 2,
    VOS_RATMODE_TDS   = 3,
    VOS_RATMODE_1X    = 4,
    VOS_RATMODE_HRPD  = 5,
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    VOS_RATMODE_NR = 6,
#endif
    VOS_RATMODE_BUTT
};
typedef VOS_UINT32 VOS_RATMODE_ENUM_UINT32;
typedef VOS_UINT32 VOS_RatModeUint32;

#if (FEATURE_VOICE_UP == FEATURE_ON)
/* 语音软件上移时, 使用ACPU的PID */
#define DSP_PID_VOICE_RT DSP_PID_VOICE_RT_ACPU
#define DSP_PID_VOICE DSP_PID_VOICE_ACPU
#else
/* 语音软件不上移时, 使用HIFI的PID */
#define DSP_PID_VOICE_RT DSP_PID_VOICE_RT_HIFI
#define DSP_PID_VOICE DSP_PID_VOICE_HIFI
#endif

/* MED HiFi的PID 定义，为保证其它组件编译通过，暂时保留MED_RT等PID */
#define DSP_PID_MED_RT DSP_PID_VOICE_RT
#define DSP_PID_MED DSP_PID_VOICE
#define DSP_PID_MED_OM DSP_PID_HIFI_OM

#if (VOS_WIN32 != VOS_OS_VER)
#define DSP_PID_STARTUP    UL1M_LPM_MSG_MID
#define I0_DSP_PID_GPHY    GPHY_MAIN_MSG_MID_0
#define I1_DSP_PID_GPHY    GPHY_MAIN_MSG_MID_1
#define I2_DSP_PID_GPHY    GPHY_MAIN_MSG_MID_2
#define I0_DSP_PID_WPHY    WPHY_MAIN_MSG_ENTRY_MID_0
#define I1_DSP_PID_WPHY    WPHY_MAIN_MSG_ENTRY_MID_1
#endif

#if defined (INSTANCE_1)
#define WUEPS_PID_USIM  I1_WUEPS_PID_USIM
#define MAPS_STK_PID    I1_MAPS_STK_PID
#define MAPS_PIH_PID    I1_MAPS_PIH_PID
#define MAPS_PB_PID     I1_MAPS_PB_PID
#define MAPS_EMAT_PID   I1_MAPS_EMAT_PID
#define WUEPS_PID_SLEEP I1_WUEPS_PID_SLEEP
#define WUEPS_PID_AWAKE I1_WUEPS_PID_AWAKE
#define UEPS_PID_DL     I1_UEPS_PID_DL
#define UEPS_PID_LL     I1_UEPS_PID_LL
#define UEPS_PID_SN     I1_UEPS_PID_SN
#define UEPS_PID_GRM    I1_UEPS_PID_GRM
#define WUEPS_PID_BMC   I1_WUEPS_PID_BMC
#define WUEPS_PID_PDCP  I1_WUEPS_PID_PDCP
#define WUEPS_PID_RLC   I1_WUEPS_PID_RLC
#define WUEPS_PID_MAC   I1_WUEPS_PID_MAC
#define WUEPS_PID_RLCDL I1_WUEPS_PID_RLCDL
#define WUEPS_PID_MACDL I1_WUEPS_PID_MACDL
#if (VOS_WIN32 == VOS_OS_VER)
#define DSP_PID_GPHY    I1_DSP_PID_GPHY
#define DSP_PID_WPHY    I1_DSP_PID_WPHY
#else
#define DSP_PID_GPHY    GPHY_MAIN_MSG_MID_1
#define DSP_PID_WPHY    WPHY_MAIN_MSG_ENTRY_MID_1
#endif
#define DSP_PID_GPHYLOW I1_DSP_PID_GPHYLOW
#define DSP_PID_SLEEP   I1_DSP_PID_SLEEP
#define DSP_PID_WAKE    I1_DSP_PID_WAKE
#define DSP_PID_IDLE    I1_DSP_PID_IDLE
#define DSP_PID_APM     UL1M_APM_MSG_MID_1
#define UEPS_PID_GAS    I1_UEPS_PID_GAS
#define WUEPS_PID_MM    I1_WUEPS_PID_MM
#define WUEPS_PID_MMC   I1_WUEPS_PID_MMC
#define UEPS_PID_REGM   I1_UEPS_PID_REGM
#define WUEPS_PID_GMM   I1_WUEPS_PID_GMM
#define WUEPS_PID_MMA   I1_WUEPS_PID_MMA
#define UEPS_PID_DSM    I1_UEPS_PID_DSM
#define WUEPS_PID_CC    I1_WUEPS_PID_CC
#define WUEPS_PID_SS    I1_WUEPS_PID_SS
#define WUEPS_PID_TC    I1_WUEPS_PID_TC
#define WUEPS_PID_SMS   I1_WUEPS_PID_SMS
#define WUEPS_PID_RABM  I1_WUEPS_PID_RABM
#define WUEPS_PID_SM    I1_WUEPS_PID_SM
#define WUEPS_PID_ADMIN I1_WUEPS_PID_ADMIN
#define WUEPS_PID_TAF   I1_WUEPS_PID_TAF
#define WUEPS_PID_VC    I1_WUEPS_PID_VC
#define UEPS_PID_MTA    I1_UEPS_PID_MTA
#define WUEPS_PID_DRV_AGENT   I1_WUEPS_PID_DRV_AGENT
#define UEPS_PID_XSMS   I1_UEPS_PID_XSMS
#define UEPS_PID_CCM    I1_UEPS_PID_CCM
#define UEPS_PID_GUCALL I1_UEPS_PID_GUCALL
#define UEPS_PID_XCALL  I1_UEPS_PID_XCALL
#define UEPS_PID_MSCC   I1_UEPS_PID_MSCC
#define WUEPS_PID_CSIMA I1_WUEPS_PID_CSIMA
#define UEPS_PID_XPDS   I1_UEPS_PID_XPDS
#define WUEPS_PID_LCS   I1_WUEPS_PID_LCS
#define PS_PID_NAS_COMM   I1_PS_PID_NAS_COMM
#define WUEPS_PID_WRR   I1_WUEPS_PID_WRR
#define WUEPS_PID_WCOM  I1_WUEPS_PID_WCOM

#define MSP_L4_L4A_PID  I1_MSP_L4_L4A_PID
#define MSP_LDSP_PID    I1_MSP_LDSP_PID
#define MSP_SYS_FTM_PID I1_MSP_SYS_FTM_PID

#define PS_PID_ERRC     I1_PS_PID_ERRC
#define PS_PID_ERMM     I1_PS_PID_ERMM
#define PS_PID_OM       I1_PS_PID_OM
#define PS_PID_HPA      I1_PS_PID_HPA
#define PS_PID_MAC_UL   I1_PS_PID_MAC_UL
#define PS_PID_MAC_DL   I1_PS_PID_MAC_DL
#define PS_PID_RLC_UL   I1_PS_PID_RLC_UL
#define PS_PID_RLC_DL   I1_PS_PID_RLC_DL
#define PS_PID_PDCP_UL  I1_PS_PID_PDCP_UL
#define PS_PID_PDCP_DL  I1_PS_PID_PDCP_DL
#define PS_PID_LPP      I1_PS_PID_LPP
#define PS_PID_MM       I1_PS_PID_MM
#define PS_PID_ESM      I1_PS_PID_ESM
#define PS_PID_RABM     I1_PS_PID_RABM
#define PS_PID_TC       I1_PS_PID_TC
#define PS_PID_IP       I1_PS_PID_IP
#define PS_PID_IMSA     I1_PS_PID_IMSA
#define PS_PID_IMSVA    I1_PS_PID_IMSVA
#define PS_PID_SCC      I1_PS_PID_SCC
#define PS_PID_PTT      I1_PS_PID_PTT

#define DSP_PID_RTTSTUB     I1_DSP_PID_RTTSTUB
#define DSP_PID_RTTMATCH    I1_DSP_PID_RTTMATCH
#define DSP_PID_RTTPREEMPT  I1_DSP_PID_RTTPREEMPT
#define DSP_PID_RTTONLINE   I1_DSP_PID_RTTONLINE
#elif defined (INSTANCE_2)
#define WUEPS_PID_USIM  I2_WUEPS_PID_USIM
#define MAPS_STK_PID    I2_MAPS_STK_PID
#define MAPS_PIH_PID    I2_MAPS_PIH_PID
#define MAPS_PB_PID     I2_MAPS_PB_PID
#define MAPS_EMAT_PID   I2_MAPS_EMAT_PID
#define WUEPS_PID_SLEEP I2_WUEPS_PID_SLEEP
#define WUEPS_PID_AWAKE I2_WUEPS_PID_AWAKE
#define UEPS_PID_DL     I2_UEPS_PID_DL
#define UEPS_PID_LL     I2_UEPS_PID_LL
#define UEPS_PID_SN     I2_UEPS_PID_SN
#define UEPS_PID_GRM    I2_UEPS_PID_GRM
#if (VOS_WIN32 == VOS_OS_VER)
#define DSP_PID_GPHY    I2_DSP_PID_GPHY
#define DSP_PID_WPHY    I0_DSP_PID_WPHY
#else
#define DSP_PID_GPHY    GPHY_MAIN_MSG_MID_2
#define DSP_PID_WPHY    WPHY_MAIN_MSG_ENTRY_MID_0
#endif
#define DSP_PID_GPHYLOW I2_DSP_PID_GPHYLOW
#define DSP_PID_SLEEP   I2_DSP_PID_SLEEP
#define DSP_PID_WAKE    I2_DSP_PID_WAKE
#define DSP_PID_IDLE    I2_DSP_PID_IDLE
#define DSP_PID_APM     UL1M_APM_MSG_MID_2
#define UEPS_PID_GAS    I2_UEPS_PID_GAS
#define WUEPS_PID_MM    I2_WUEPS_PID_MM
#define WUEPS_PID_MMC   I2_WUEPS_PID_MMC
#define UEPS_PID_REGM   I2_UEPS_PID_REGM
#define WUEPS_PID_GMM   I2_WUEPS_PID_GMM
#define WUEPS_PID_MMA   I2_WUEPS_PID_MMA
#define UEPS_PID_DSM    I2_UEPS_PID_DSM
#define WUEPS_PID_CC    I2_WUEPS_PID_CC
#define WUEPS_PID_SS    I2_WUEPS_PID_SS
#define WUEPS_PID_TC    I2_WUEPS_PID_TC
#define WUEPS_PID_SMS   I2_WUEPS_PID_SMS
#define WUEPS_PID_RABM  I2_WUEPS_PID_RABM
#define WUEPS_PID_SM    I2_WUEPS_PID_SM
#define WUEPS_PID_ADMIN I2_WUEPS_PID_ADMIN
#define WUEPS_PID_TAF   I2_WUEPS_PID_TAF
#define WUEPS_PID_VC    I2_WUEPS_PID_VC
#define UEPS_PID_MTA    I2_UEPS_PID_MTA
#define WUEPS_PID_DRV_AGENT   I2_WUEPS_PID_DRV_AGENT
#define UEPS_PID_MSCC   I2_UEPS_PID_MSCC
#define UEPS_PID_XSMS   I2_UEPS_PID_XSMS
#define UEPS_PID_CCM    I2_UEPS_PID_CCM
#define UEPS_PID_GUCALL I2_UEPS_PID_GUCALL
#define UEPS_PID_XCALL  I2_UEPS_PID_XCALL
#define WUEPS_PID_CSIMA I2_WUEPS_PID_CSIMA
#define UEPS_PID_XPDS   I2_UEPS_PID_XPDS
#define WUEPS_PID_LCS   I2_WUEPS_PID_LCS
#define PS_PID_NAS_COMM   I2_PS_PID_NAS_COMM
#define WUEPS_PID_WRR   I0_WUEPS_PID_WRR
#define WUEPS_PID_WCOM  I0_WUEPS_PID_WCOM
#define WUEPS_PID_BMC   I0_WUEPS_PID_BMC
#define WUEPS_PID_PDCP  I0_WUEPS_PID_PDCP
#define WUEPS_PID_RLC   I0_WUEPS_PID_RLC
#define WUEPS_PID_MAC   I0_WUEPS_PID_MAC
#define WUEPS_PID_RLCDL I0_WUEPS_PID_RLCDL
#define WUEPS_PID_MACDL I0_WUEPS_PID_MACDL

#define MSP_L4_L4A_PID  I0_MSP_L4_L4A_PID
#define MSP_LDSP_PID    I0_MSP_LDSP_PID
#define MSP_SYS_FTM_PID I0_MSP_SYS_FTM_PID

#define PS_PID_ERRC     I0_PS_PID_ERRC
#define PS_PID_ERMM     I0_PS_PID_ERMM
#define PS_PID_OM       I0_PS_PID_OM
#define PS_PID_HPA      I0_PS_PID_HPA
#define PS_PID_MAC_UL   I0_PS_PID_MAC_UL
#define PS_PID_MAC_DL   I0_PS_PID_MAC_DL
#define PS_PID_RLC_UL   I0_PS_PID_RLC_UL
#define PS_PID_RLC_DL   I0_PS_PID_RLC_DL
#define PS_PID_PDCP_UL  I0_PS_PID_PDCP_UL
#define PS_PID_PDCP_DL  I0_PS_PID_PDCP_DL
#define PS_PID_LPP      I0_PS_PID_LPP
#define PS_PID_MM       I0_PS_PID_MM
#define PS_PID_ESM      I0_PS_PID_ESM
#define PS_PID_RABM     I0_PS_PID_RABM
#define PS_PID_TC       I0_PS_PID_TC
#define PS_PID_IP       I0_PS_PID_IP
#define PS_PID_IMSA     I0_PS_PID_IMSA
#define PS_PID_IMSVA    I0_PS_PID_IMSVA
#define PS_PID_SCC      I0_PS_PID_SCC
#define PS_PID_PTT      I0_PS_PID_PTT

#else
#define WUEPS_PID_USIM  I0_WUEPS_PID_USIM
#define MAPS_STK_PID    I0_MAPS_STK_PID
#define MAPS_PIH_PID    I0_MAPS_PIH_PID
#define MAPS_PB_PID     I0_MAPS_PB_PID
#define MAPS_EMAT_PID   I0_MAPS_EMAT_PID
#define WUEPS_PID_SLEEP I0_WUEPS_PID_SLEEP
#define WUEPS_PID_AWAKE I0_WUEPS_PID_AWAKE
#define UEPS_PID_GAS    I0_UEPS_PID_GAS
#define UEPS_PID_DL     I0_UEPS_PID_DL
#define UEPS_PID_LL     I0_UEPS_PID_LL
#define UEPS_PID_SN     I0_UEPS_PID_SN
#define UEPS_PID_GRM    I0_UEPS_PID_GRM
#define WUEPS_PID_BMC   I0_WUEPS_PID_BMC
#define WUEPS_PID_PDCP  I0_WUEPS_PID_PDCP
#define WUEPS_PID_RLC   I0_WUEPS_PID_RLC
#define WUEPS_PID_MAC   I0_WUEPS_PID_MAC
#define WUEPS_PID_RLCDL I0_WUEPS_PID_RLCDL
#define WUEPS_PID_MACDL I0_WUEPS_PID_MACDL
#if (VOS_WIN32 == VOS_OS_VER)
#define DSP_PID_GPHY    I0_DSP_PID_GPHY
#define DSP_PID_WPHY    I0_DSP_PID_WPHY
#else
#define DSP_PID_GPHY    GPHY_MAIN_MSG_MID_0
#define DSP_PID_WPHY    WPHY_MAIN_MSG_ENTRY_MID_0
#endif
#define DSP_PID_GPHYLOW I0_DSP_PID_GPHYLOW
#define DSP_PID_SLEEP   I0_DSP_PID_SLEEP
#define DSP_PID_WAKE    I0_DSP_PID_WAKE
#define DSP_PID_IDLE    I0_DSP_PID_IDLE
#define DSP_PID_APM     UL1M_APM_MSG_MID_0
#define WUEPS_PID_MM    I0_WUEPS_PID_MM
#define WUEPS_PID_MMC   I0_WUEPS_PID_MMC
#define UEPS_PID_REGM   I0_UEPS_PID_REGM
#define WUEPS_PID_GMM   I0_WUEPS_PID_GMM
#define WUEPS_PID_MMA   I0_WUEPS_PID_MMA
#define UEPS_PID_DSM    I0_UEPS_PID_DSM
#define WUEPS_PID_CC    I0_WUEPS_PID_CC
#define WUEPS_PID_SS    I0_WUEPS_PID_SS
#define WUEPS_PID_TC    I0_WUEPS_PID_TC
#define WUEPS_PID_SMS   I0_WUEPS_PID_SMS
#define WUEPS_PID_RABM  I0_WUEPS_PID_RABM
#define WUEPS_PID_SM    I0_WUEPS_PID_SM
#define WUEPS_PID_ADMIN I0_WUEPS_PID_ADMIN
#define WUEPS_PID_TAF   I0_WUEPS_PID_TAF
#define WUEPS_PID_VC    I0_WUEPS_PID_VC
#define UEPS_PID_MTA    I0_UEPS_PID_MTA
#define WUEPS_PID_DRV_AGENT   I0_WUEPS_PID_DRV_AGENT
#define UEPS_PID_XSMS   I0_UEPS_PID_XSMS
#define UEPS_PID_CCM    I0_UEPS_PID_CCM
#define UEPS_PID_GUCALL I0_UEPS_PID_GUCALL
#define UEPS_PID_XCALL  I0_UEPS_PID_XCALL

#define UEPS_PID_MSCC   I0_UEPS_PID_MSCC
#define WUEPS_PID_CSIMA I0_WUEPS_PID_CSIMA

#define UEPS_PID_XPDS   I0_UEPS_PID_XPDS
#define WUEPS_PID_LCS   I0_WUEPS_PID_LCS
#define PS_PID_NAS_COMM   I0_PS_PID_NAS_COMM
#define WUEPS_PID_WRR   I0_WUEPS_PID_WRR
#define WUEPS_PID_WCOM  I0_WUEPS_PID_WCOM

#define MSP_L4_L4A_PID  I0_MSP_L4_L4A_PID
#define MSP_LDSP_PID    I0_MSP_LDSP_PID
#define MSP_SYS_FTM_PID I0_MSP_SYS_FTM_PID

#ifdef FEATURE_NPHY_START
#define PHY_PID_STARTUP PHY_PID_STARTUP1
#else
#define PHY_PID_STARTUP NPHY_PID_STARTUP
#endif

#define PS_PID_ERRC     I0_PS_PID_ERRC
#define PS_PID_ERMM     I0_PS_PID_ERMM
#define PS_PID_OM       I0_PS_PID_OM
#define PS_PID_HPA      I0_PS_PID_HPA
#define PS_PID_MAC_UL   I0_PS_PID_MAC_UL
#define PS_PID_MAC_DL   I0_PS_PID_MAC_DL
#define PS_PID_RLC_UL   I0_PS_PID_RLC_UL
#define PS_PID_RLC_DL   I0_PS_PID_RLC_DL
#define PS_PID_PDCP_UL  I0_PS_PID_PDCP_UL
#define PS_PID_PDCP_DL  I0_PS_PID_PDCP_DL
#define PS_PID_LPP      I0_PS_PID_LPP
#define PS_PID_MM       I0_PS_PID_MM
#define PS_PID_ESM      I0_PS_PID_ESM
#define PS_PID_RABM     I0_PS_PID_RABM
#define PS_PID_TC       I0_PS_PID_TC
#define PS_PID_IP       I0_PS_PID_IP
#define PS_PID_IMSA     I0_PS_PID_IMSA
#define PS_PID_IMSVA    I0_PS_PID_IMSVA
#define PS_PID_SCC     I0_PS_PID_SCC
#define PS_PID_PTT      I0_PS_PID_PTT
#define UEPS_PID_NRMM   I0_UEPS_PID_NRMM
#define UEPS_PID_NRSM   I0_UEPS_PID_NRSM
#define UEPS_PID_PCF    I0_UEPS_PID_PCF
#define UEPS_PID_NREAP  I0_UEPS_PID_NREAP
#define PS_PID_NRTHROT  I0_PS_PID_NRTHROT

#define DSP_PID_RTTSTUB     I0_DSP_PID_RTTSTUB
#define DSP_PID_RTTMATCH    I0_DSP_PID_RTTMATCH
#define DSP_PID_RTTPREEMPT  I0_DSP_PID_RTTPREEMPT
#define DSP_PID_RTTONLINE   I0_DSP_PID_RTTONLINE
#endif

#define UPHY_PID_CSDR_1X_CM         DSP_PID_CSDR_1X_CM_AGENT
#define UPHY_PID_CSDR_1X_SM         DSP_PID_CSDR_1X_SM_AGENT
#define UPHY_PID_CSDR_HRPD_CM       DSP_PID_CSDR_HRPD_CM_AGENT
#define UPHY_PID_CSDR_HRPD_SM       DSP_PID_CSDR_HRPD_SM_AGENT
#define UPHY_PID_GSDR_HIGHPRI       DSP_PID_GSDR_HIGHPRI_AGENT

#if (FEATURE_ON == FEATURE_SUPPORT_5G_RTT_TIME_DOMAIN_BBIT)
#define UEPS_PID_NRRC               UEPS_PID_NRRC_0
#define UEPS_PID_NRRM               UEPS_PID_NRRM_0
#else
#define UEPS_PID_NRRC               I0_UEPS_PID_NRRC
#define UEPS_PID_NRRM               I0_UEPS_PID_NRRM
#endif

#ifndef UPHY_PID_HL1C_L1C_L
#define UPHY_PID_HL1C_L1C_L         UPHY_NRSCHED_HL1C_SYMBINT_MID_0
#endif

#define NPHY_PID_HL1C               UPHY_NRSCHED_HL1C_SYMBINT_MID_0

#define NPHY_I1_PID_HL1C            UPHY_NRSCHED_HL1C_SYMBINT_MID_1

#define LPHY_PID_HL1C               UPHY_LTE0SCHED_SYMBINT_HL1C_MID

#define LPHY_I1_PID_HL1C            UPHY_LTE1SCHED_SYMBINT_HL1C_MID

#define DSP_PID_RCM    UPHY_UL1M_RCM_M_MID

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _VOS_PID_DEF_H */
