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

#ifndef __MBB_LOG_SERVICE_API_H__
#define __MBB_LOG_SERVICE_API_H__
/*****************************************头文件********************************************/
#include "mbb_log_service_types.h"
#include "product_config.h"
#include "taf_app_mma.h"
#include "v_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
/*****************************************宏定义********************************************/
/* 搜网/注册/移动性 */
#define LOG_SERVICE_FUNCTION_MOBILITY FEATURE_ON
/* 拨号 */
#define LOG_SERVICE_FUNCTION_PS_CALL FEATURE_ON
/* CS语音 */
#define LOG_SERVICE_FUNCTION_CS_CALL FEATURE_ON
/* 数据业务 */
#define LOG_SERVICE_FUNCTION_DATA_SERVICE FEATURE_ON
/* SIM卡 */
#define LOG_SERVICE_FUNCTION_SIM FEATURE_ON
/* 短信 */
#define LOG_SERVICE_FUNCTION_SMS FEATURE_ON
/* VOLTE */
#define LOG_SERVICE_FUNCTION_IMS FEATURE_ON
/* 其他 */
#define LOG_SERVICE_FUNCTION_OTHERS FEATURE_ON
/* ECALL */
#if (FEATURE_ECALL == FEATURE_ON)
#define LOG_SERVICE_FUNCTION_ECALL FEATURE_ON
#else
#define LOG_SERVICE_FUNCTION_ECALL FEATURE_OFF
#endif

/* 定义最大的ATR长度 */
#define LOG_SERVICE_MAX_ATR_LEN 80

/* APN最大长度 */
#define LOG_SERVICE_MAX_APN_LEN 100

/* SIM卡EHPLMN的最大个数 */
#define LOG_SERVICE_EHPLMN_MAX_NUM 16

/* SYSCFGEX接入技术最大个数 */
#define LOG_SERVICE_MAX_RAT_NUM TAF_MMA_RAT_BUTT

#if ((FEATURE_LTE == FEATURE_ON) && (FEATURE_UE_MODE_NR == FEATURE_ON))
#define TAF_MLOG_MAX_RAT_NUM 4 /* 当前支持的接入技术个数 */
#elif ((FEATURE_ON == FEATURE_LTE) || (FEATURE_ON == FEATURE_UE_MODE_NR))
#define TAF_MLOG_MAX_RAT_NUM 3
#else
#define TAF_MLOG_MAX_RAT_NUM 2 /* 当前支持的GU接入技术个数 */
#endif
#define TAF_MLOG_LTE_BAND_MAX_LENGTH 8

/**************************************枚举结构定义*****************************************/
/* 协议事件定义 */
typedef enum {
    LOG_SERVICE_EVT_NONE = 0,

#if (LOG_SERVICE_FUNCTION_MOBILITY == FEATURE_ON)
    /* 搜网/注册/移动性事件定义 */
    LOG_SERVICE_EVT_MOBILITY_LMM_REG_REJ, /* LTE注册被拒 */
    LOG_SERVICE_EVT_MOBILITY_LMM_REG_ACP, /* LTE注册成功 */
    LOG_SERVICE_EVT_MOBILITY_LMM_TAU_REJ, /* TAU被拒 */
    LOG_SERVICE_EVT_MOBILITY_LMM_TAU_ACP, /* TAU成功 */
    LOG_SERVICE_EVT_MOBILITY_GMM_REG_REJ, /* GMM注册被拒 */
    LOG_SERVICE_EVT_MOBILITY_GMM_REG_ACP, /* GMM注册成功 */
    LOG_SERVICE_EVT_MOBILITY_GMM_RAU_REJ, /* RAU被拒 */
    LOG_SERVICE_EVT_MOBILITY_GMM_RAU_ACP, /* RAU成功 */
    LOG_SERVICE_EVT_MOBILITY_MM_LAU_REJ, /* LAU被拒 */
    LOG_SERVICE_EVT_MOBILITY_MM_LAU_ACP, /* LAU成功 */
    LOG_SERVICE_EVT_MOBILITY_MT_DETACH, /* MT DETACH */
    LOG_SERVICE_EVT_MOBILITY_LOCAL_DETACH, /* 本地去注册 */
    LOG_SERVICE_EVT_MOBILITY_SIGNAL_INFO, /* 信号信息 */
    LOG_SERVICE_EVT_MOBILITY_CELL_FREQ_INFO, /* 小区频率信息 */
    LOG_SERVICE_EVT_MOBILITY_CELL_LOC_INFO, /* 小区位置信息 */
    LOG_SERVICE_EVT_MOBILITY_SRV_STATE, /* 服务状态 */
    LOG_SERVICE_EVT_MOBILITY_PLMN_SEARCH_START, /* 搜网场景 */
    LOG_SERVICE_EVT_MOBILITY_SMC_FAILURE, /* SMC失败 */
    LOG_SERVICE_EVT_MOBILITY_NW_AUTH_REJ, /* 网络鉴权拒绝 */
    LOG_SERVICE_EVT_MOBILITY_ENDC_STATE_CHANGE, /* ENDC状态变更 */
    LOG_SERVICE_EVT_MOBILITY_SYSCFGEX_SET, /* SYSCFGEX参数设置 */
    LOG_SERVICE_EVT_MOBILITY_RRC_STAT, /* RRC状态变化 */
    LOG_SERVICE_EVT_MOBILITY_RAT_CHANGE, /* 主模制式发生变化 */
    LOG_SERVICE_EVT_MOBILITY_NET_SEL_OPT_CNF, /* 搜网优化流程执行结果事件 */
    LOG_SERVICE_EVT_MOBILITY_NET_SEL_OPT_RPT, /* 搜网优化流程状态上报事件 */
    LOG_SERVICE_EVT_MOBILITY_LTEV_STATE_CHANGE, /* 当前同步状态发生变化 */
    LOG_SERVICE_EVT_MOBILITY_LTEV_TYPE_CHANGE, /* 当前同步源发生变化 */
    LOG_SERVICE_EVT_MOBILITY_LTEV_LEVEL_CHANGE, /* 当前CBR当前拥塞分级发生变化 */
    LOG_SERVICE_EVT_MOBILITY_LTEV_MODE_INFO, /* VMODE 模式设置信息 */
#endif

#if (LOG_SERVICE_FUNCTION_PS_CALL == FEATURE_ON)
    /* PS CALL事件定义 */
    LOG_SERVICE_EVT_PS_CALL_REJ, /* 拨号被拒事件 */
    LOG_SERVICE_EVT_PS_CALL_REQ_FAILURE, /* 拨号请求返回失败 */
    LOG_SERVICE_EVT_PS_CALL_REL_FAILURE, /* 断开拨号请求返回失败 */
    LOG_SERVICE_EVT_PS_CALL_EXP_PDP_DEACT, /* 网络始发的拨号断开事件 */
    LOG_SERVICE_EVT_PS_CALL_SETUP_REQ, /* 拨号请求事件 */
    LOG_SERVICE_EVT_PS_CALL_REL_REQ, /* 断开拨号请求事件 */
    LOG_SERVICE_EVT_PS_CALL_SETUP, /* 拨号成功事件 */
    LOG_SERVICE_EVT_PS_CALL_PDN_APN_AMBR, /* 开户APN速率打印事件 */
    LOG_SERVICE_EVT_PS_CALL_REL, /* 断开拨号成功事件 */
    LOG_SERVICE_EVT_PS_CALL_MT_PDP_MODIFY, /* PDP上下文Modify事件 */
    LOG_SERVICE_EVT_PS_CALL_EPS_BEARER_INFO, /* EPS承载信息 */
#endif

#if (LOG_SERVICE_FUNCTION_CS_CALL == FEATURE_ON)
    /* CS CALL事件定义 */
    LOG_SERVICE_EVT_CS_CALL_CEND_CNF, /* 呼叫挂断Cend事件 */
    LOG_SERVICE_EVT_CS_CALL_ORIGIN_CALL_FAILURE, /* 呼叫发起事件 */
    LOG_SERVICE_EVT_CS_CALL_REDIAL, /* 呼叫重拨事件 */
    LOG_SERVICE_EVT_CS_CALL_SRVCC_CANCEL, /* 呼叫SRVCC取消事件 */
    LOG_SERVICE_EVT_CS_CALL_ENCODE_INFO, /* 呼叫编码信息 */
    LOG_SERVICE_EVT_CS_CALL_INCOMING_CALL, /* 呼叫呼入事件 */
    LOG_SERVICE_EVT_CS_CALL_ORIGIN_CALL_REQ, /* 呼叫发起请求事件 */
#endif

#if (LOG_SERVICE_FUNCTION_DATA_SERVICE == FEATURE_ON)
    /* 数据业务事件定义 */
    LOG_SERVICE_EVT_DATA_SERVICE_LMM_SER_REJ, /* LTE Service被拒事件 */
    LOG_SERVICE_EVT_DATA_SERVICE_GMM_SER_REJ, /* GMM Service被拒事件 */
#endif

#if (LOG_SERVICE_FUNCTION_SIM == FEATURE_ON)
    /* SIM卡事件定义 */
    LOG_SERVICE_EVT_SIM_CARD_INOUT, /* SIM卡插拔事件 */
    LOG_SERVICE_EVT_SIM_MANDATORY_FILE_MISS, /* 必选文件失败 */
    LOG_SERVICE_EVT_SIM_GET_ATR_FAIL, /* 获取ATR失败 */
    LOG_SERVICE_EVT_SIM_ATR_ERROR, /* ATR错误 */
    LOG_SERVICE_EVT_SIM_SIMST, /* SIM卡状态上报 */
    LOG_SERVICE_EVT_SIM_LOCK_STATE, /* SIMLOCK状态上报 */
    LOG_SERVICE_EVT_SIM_AID_TYPE, /* AID文件中的应用类型 */
    LOG_SERVICE_EVT_SIM_CARD_STATE, /* PIN/PUK锁状态 */
    LOG_SERVICE_EVT_SIM_STATUS_CMD_ERROR, /* STATUS轮询失败 */
    LOG_SERVICE_EVT_SIM_AUTH_REJ, /* SIM卡鉴权失败 */
    LOG_SERVICE_EVT_SIM_EHPLMN_INFO, /* EHPLMN信息 */
    LOG_SERVICE_EVT_SIM_MSISDN_INFO, /* MSISDN信息 */
#endif

#if (LOG_SERVICE_FUNCTION_SMS == FEATURE_ON)
    /* 短信事件定义 */
    LOG_SERVICE_EVT_SMS_SEND_IN_CACHE, /* 短信被缓存事件 */
    LOG_SERVICE_EVT_SMS_MEM_STATUS, /* 短信存储介质状态事件 */
    LOG_SERVICE_EVT_SMS_CP_ERROR, /* 短信发送时收到cp-error事件 */
    LOG_SERVICE_EVT_SMS_MM_ERROR, /* 短信业务过程中收到MM错误事件 */
    LOG_SERVICE_EVT_SMS_MM_REL, /* 短信业务过程中收到MM释放事件 */
    LOG_SERVICE_EVT_SMS_SEND_DOMAIN, /* 短信发送承载域 */
    LOG_SERVICE_EVT_SMS_SEND_START, /* 短信发送开始事件 */
    LOG_SERVICE_EVT_SMS_SEND_FAILED, /* 短信发送失败事件 */
    LOG_SERVICE_EVT_SMS_MM_EST_STATUS, /* 短信MM建链事件 */
    LOG_SERVICE_EVT_SMS_RCV_START, /* 接收短信开始事件 */
    LOG_SERVICE_EVT_SMS_SEND_SUC, /* 短信发送成功事件 */
    LOG_SERVICE_EVT_SMS_CHECK_RESULT, /* FDN/MO Ctrl校验结果 */
    LOG_SERVICE_EVT_SMS_SEND_ABORT, /* 短信中断事件 */
#endif

#if (LOG_SERVICE_FUNCTION_IMS == FEATURE_ON)
    /* IMS事件定义 */
    LOG_SERVICE_EVT_IMS_PDN_ACT_REJ, /* IMS PDN激活被拒事件 */
    LOG_SERVICE_EVT_IMS_PDN_DEACT, /* IMS PDN去激活事件 */
    LOG_SERVICE_EVT_IMS_REG_STATUS, /* IMS注册状态事件 */
    LOG_SERVICE_EVT_IMS_SRVCC_STATUS, /* IMS SRVCC状态事件 */
    LOG_SERVICE_EVT_IMS_ENCODE_INFO, /* IMS呼叫编码信息 */
    LOG_SERVICE_EVT_IMS_INCOMING_CALL, /* IMS语音来电指示 */
#endif

#if (LOG_SERVICE_FUNCTION_OTHERS == FEATURE_ON)
    /* 其他事件定义 */
    LOG_SERVICE_EVT_OTHERS_TIMER_EXPIRED, /* 协议定时器超时事件 */
    LOG_SERVICE_EVT_OTHERS_COMMON_PRINT, /* 通用打印事件 */
    LOG_SERVICE_EVT_OTHERS_POWER_OFF, /* 协议关机事件 */
    LOG_SERVICE_EVT_OTHERS_POWER_ON, /* 协议开机事件 */
    LOG_SERVICE_EVT_OTHERS_ACORE_STAT_CHANGE, /* A核休眠唤醒状态变化 */
#endif

#if (LOG_SERVICE_FUNCTION_ECALL == FEATURE_ON)
    /* ECALL事件定义 */
    LOG_SERVICE_EVT_ECALL_MSD_ABORT, /* ECALL MSD传输中断事件(IN-BAND) */
    LOG_SERVICE_EVT_ECALL_START_TYPE, /* ECALL呼叫发起类型事件 */
    LOG_SERVICE_EVT_ECALL_START_MODE, /* ECALL呼叫发起模式事件 */
    LOG_SERVICE_EVT_ECALL_MSD_ALACK_IND, /* ECALL MSD传输接收到ALACK事件(IN-BAND) */
    LOG_SERVICE_EVT_ECALL_TIMER_STATUS, /* ECALL TIMER状态事件(T9,T10) */
    LOG_SERVICE_EVT_ECALL_SIP_STATUS_IND, /* ECALL SIP MSD传输状态事件 */
    LOG_SERVICE_EVT_ECALL_INBAND_STATUS_IND, /* ECALL IN-BAND MSD传输状态事件 */
    LOG_SERVICE_EVT_ECALL_ECL_SUPPORT_IND, /* ECALL ECL是否支持事件 */
    LOG_SERVICE_EVT_ECALL_MSD_IMSECLCFG_SET, /* ECALL ECLIMSCFG设置事件 */
#endif

    LOG_SERVICE_EVT_BUTT,
} LogServiceEventId;

/* 模块内部接入技术枚举定义 */
typedef enum {
    LOG_SERVICE_RAT_NONE = 0, /* INVALID */
    LOG_SERVICE_RAT_GSM, /* GSM */
    LOG_SERVICE_RAT_WCDMA, /* WCDMA */
    LOG_SERVICE_RAT_LTE, /* LTE */
    LOG_SERVICE_RAT_TDS, /* TDS */
    LOG_SERVICE_RAT_NR, /* NR */

    LOG_SERVICE_RAT_BUTT,
} LogServiceRatType;

/* 模块内部服务状态枚举定义 */
typedef enum {
    LOG_SERVICE_SRVSTAT_NONE = 0, /* 无服务 */
    LOG_SERVICE_SRVSTAT_LIMIT, /* 限制服务 */
    LOG_SERVICE_SRVSTAT_NORMAL, /* 正常服务 */

    LOG_SERVICE_SRVSTAT_BUTT,
} LogServiceSrvStat;

/* 协议定时器枚举定义 */
typedef enum {
    LOG_SERVICE_TIMER_NONE = 0,
    /* LMM */
    LOG_SERVICE_TIMER_T3410, /* ATTACH定时器 */
    LOG_SERVICE_TIMER_T3417, /* Service Request定时器 */
    LOG_SERVICE_TIMER_T3430, /* TAU定时器 */
    LOG_SERVICE_TIMER_T3482, /* PDN连接定时器 */
    /* GMM */
    LOG_SERVICE_TIMER_T3310, /* ATTACH定时器 */
    LOG_SERVICE_TIMER_T3317, /* Service Request定时器 */
    LOG_SERVICE_TIMER_T3330, /* RAU定时器 */
    LOG_SERVICE_TIMER_T3380, /* PDP激活定时器 */
    /* SMS */
    LOG_SERVICE_TIMER_TR1M, /* SMS MO */
    LOG_SERVICE_TIMER_TC1M_MO, /* SMS MO */
    LOG_SERVICE_TIMER_TC1M_MT, /* SMS MT */
    LOG_SERVICE_TIMER_TR2M, /* SMS MT */
    LOG_SERVICE_TIMER_RETRY_PERIOD, /* SMS发送周期定时器 */
    LOG_SERVICE_TIMER_RETRY_INTERVAL, /* SMS发送间隔定时器 */
    /* ECALL */
    LOG_SERVICE_TIMER_T9, /* ECALL T9定时器 */
    LOG_SERVICE_TIMER_T10, /* ECALL T10定时器 */

    LOG_SERVICE_TIMER_BUTT,
} LogServiceParamTimerId;

/* SIM卡拔插状态定义 */
typedef enum {
    LOG_SERVICE_SIM_REMOVE = 0, /* SIM卡拔出事件 */
    LOG_SERVICE_SIM_INSERT, /* SIM卡插入事件 */
} LogServiceParamSimOperation;

/* SIMLOCK状态定义 */
typedef enum {
    LOG_SERVICE_SIMLOCK_UNLOCKED = 0, /* 合法卡 */
    LOG_SERVICE_SIMLOCK_LOCKED, /* 非法卡 */
} LogServiceParamSimlockState;

/* SIM卡上电电压参数定义 */
typedef enum {
    LOG_SERVICE_SIM_VOLTAGE_1P8V = 1, /* 1.8V */
    LOG_SERVICE_SIM_VOLTAGE_3P0V, /* 3.0V */
    LOG_SERVICE_SIM_VOLTAGE_5P0V, /* 5.0V */
} LogServiceParamSimVoltage;

/* SIM卡必选文件列表 */
typedef enum {
    LOG_SERVICE_SIM_MFILE_MF = 0x3F00, /* MF */
    LOG_SERVICE_SIM_MFILE_IMSI = 0x6F07, /* IMSI */
    LOG_SERVICE_SIM_MFILE_ICCID = 0x2FE2, /* ICCID */
    LOG_SERVICE_SIM_MFILE_DIR = 0x2F00, /* DIR */
    LOG_SERVICE_SIM_MFILE_ADF = 0x7F00, /* ADF */
    LOG_SERVICE_SIM_MFILE_UST = 0x6F38, /* UST */
    LOG_SERVICE_SIM_MFILE_AD = 0x6FAD, /* AD */
    LOG_SERVICE_SIM_MFILE_DFGSM = 0x7F20, /* DFGSM */
} LogServiceParamMandatoryFileId;

/* 定义SIM卡的AID状态 */
typedef enum {
    LOG_SERVICE_SIM_AID_USIM = 0x01, /* USIM存在 */
    LOG_SERVICE_SIM_AID_ISIM = 0x02, /* ISIM存在 */
    LOG_SERVICE_SIM_AID_CSIM = 0x04, /* CSIM存在 */
} LogServiceParamAidInfo;

/* SIM卡状态 */
typedef enum {
    LOG_SERVICE_SIM_CARD_PIN = 0x00, /* PIN码锁定 */
    LOG_SERVICE_SIM_CARD_PUK, /* PUK码锁定 */
    LOG_SERVICE_SIM_CARD_ABSENT, /* SIM卡不在位 */
    LOG_SERVICE_SIM_CARD_AVAILABLE, /* SIM卡可用 */
    LOG_SERVICE_SIM_CARD_UNAVAILABLE, /* SIM卡不可用 */
} LogServiceParamCardState;

/* 短信发起类型 */
typedef enum {
    LOG_SERVICE_SMS_DIR_MO = 0, /* 发送短信 */
    LOG_SERVICE_SMS_DIR_MT, /* 接收短信 */
} LogServiceSmsDirection;

/* IP类型定义 */
typedef enum {
    LOG_SERVICE_IP_TYPE_V4 = 1, /* IPV4 */
    LOG_SERVICE_IP_TYPE_V6, /* IPV6 */
    LOG_SERVICE_IP_TYPE_V4V6, /* IPV4V6 */
    LOG_SERVICE_IP_TYPE_PPP, /* PPP */
} LogServiceIpType;

/* 拨号鉴权类型定义 */
typedef enum {
    LOG_SERVICE_AUTH_TYPE_NONE = 0, /* 无鉴权 */
    LOG_SERVICE_AUTH_TYPE_PAP, /* PAP */
    LOG_SERVICE_AUTH_TYPE_CHAP, /* CHAP */
    LOG_SERVICE_AUTH_TYPE_AUTO, /* AUTO */
} LogServiceAuthType;

/* 短信存储介质类型 */
typedef enum {
    LOG_SERVICE_SMS_MEM_APP = 0, /* 短信存储在应用 */
    LOG_SERVICE_SMS_MEM_SIM, /* 短信存储在SIM卡 */
} LogServiceSmsMemType;

/* 短信存储介质状态类型 */
typedef enum {
    LOG_SERVICE_SMS_MEM_NOT_FULL = 0, /* 短信存储未满 */
    LOG_SERVICE_SMS_MEM_FULL, /* 短信存储满 */
} LogServiceSmsMemDStatus;

/* 短信MO/FDN类型 */
typedef enum {
    LOG_SERVICE_SMS_CHECK_FDN = 0, /* FDN校验 */
    LOG_SERVICE_SMS_CHECK_MO_CTRL, /* MO Control */
} LogServiceSmsCheckType;

/* 呼叫发起失败错误类型 */
typedef enum {
    LOG_SERVICE_VOICE_ORG_BY_EMC = 0, /* 紧急呼叫 */
    LOG_SERVICE_VOICE_ORG_BY_PH_MODE, /* 正常呼叫 */
} LogServiceVoiceOrgType;

/* 短信状态 */
typedef enum {
    LOG_SERVICE_SMS_STATUS_SEND_INIT = 0, /* 初始化 */
    LOG_SERVICE_SMS_STATUS_SEND_START, /* 开始发短信 */
    LOG_SERVICE_SMS_STATUS_SEND_SUCCESS, /* 短信发送成功 */
    LOG_SERVICE_SMS_STATUS_SEND_FAILED, /* 短信发送失败 */
    LOG_SERVICE_SMS_STATUS_RECEIVE_START, /* 开始接收短信 */
} LogServiceSmsStatus;

/* 短信承载类型 */
typedef enum {
    LOG_SERVICE_SMS_DOMAIN_CS = 0, /* 短信走CS域 */
    LOG_SERVICE_SMS_DOMAIN_PS, /* 短信走PS域 */
    LOG_SERVICE_SMS_DOMAIN_BUTT,
} LogServiceSmsDomain;

/* SIM卡状态，定义同SIMST上报 */
typedef enum {
    LOG_SERVICE_SIM_STAT_INVALID = 0, /* 无效状态 */
    LOG_SERVICE_SIM_STAT_VALID = 1, /* 卡有效 */
    LOG_SERVICE_SIM_STAT_ONLY_PS_VALID = 2, /* 只有PS有效 */
    LOG_SERVICE_SIM_STAT_ONLY_CS_VALID = 3, /* 只有CS有效 */
    LOG_SERVICE_SIM_STAT_PS_CS_INVALID = 4, /* CS/PS都无效 */
    LOG_SERVICE_SIM_STAT_ROMSIM = 240, /* ROMSIM */
    LOG_SERVICE_SIM_STAT_ABSENT = 255, /* SIM卡不在位 */
} LogServiceSimState;

/* GMM Service请求类型枚举 */
typedef enum {
    LOG_SERVICE_GMM_SER_TYPE_SIGNALING = 0, /* 信令请求 */
    LOG_SERVICE_GMM_SER_TYPE_DATA, /* 数据请求 */
    LOG_SERVICE_GMM_SER_TYPE_PAGING_RESPONSE, /* 寻呼响应 */
    LOG_SERVICE_GMM_SER_TYPE_RESERVED, /* 其他 */
} LogServiceGmmServiceReqType;

/* RRC状态枚举定义 */
typedef enum {
    LOG_SERVICE_RRC_STAT_IDLE = 0, /* RRC空闲态 */
    LOG_SERVICE_RRC_STAT_CONN, /* RRC连接态 */
    LOG_SERVICE_RRC_STAT_INACT, /* RRC INACTIVE态 */
    LOG_SERVICE_RRC_STAT_BUTT, /* RRC无效状态 */
} LogServiceRrcStat;

/* 定时器状态枚举定义 */
typedef enum {
    LOG_SERVICE_TIMER_STAT_RUNNING = 0, /* 定时器启动 */
    LOG_SERVICE_TIMER_STAT_STOP, /* 定时器停止 */
    LOG_SERVICE_TIMER_STAT_BUTT, /* 定时器无效状态 */
} LogServiceTimerStat;

/*
 * 结构说明: MLOG系统模式枚举
 */
enum TAF_MLOG_NetRatType {
    TAF_MLOG_NET_RAT_TYPE_GSM,
    TAF_MLOG_NET_RAT_TYPE_WCDMA,
    TAF_MLOG_NET_RAT_TYPE_LTE,

    TAF_MLOG_NET_RAT_TYPE_CDMA_1X,
    TAF_MLOG_NET_RAT_TYPE_EVDO,

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    TAF_MLOG_NET_RAT_TYPE_NR,
#endif

    TAF_MLOG_NET_RAT_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MLOG_NetRatTypeUint8;

/**************************************数据结构定义*****************************************/
/* LTE小区信息结构定义 */
typedef struct {
    LogServiceUInt cellId; /* 小区ID */
    LogServiceUInt frequency; /* 小区频点 */
    LogServiceUShort tac; /* 小区TAC */
    LogServiceUShort bandwidth; /* 小区带宽 */
    LogServiceUChar band; /* 当前频段 */
    LogServiceUChar rsv[3]; /* 保留位 */
} LogServiceLteCellInfo;

/* NR小区的信息结果定义 */
typedef LogServiceLteCellInfo LogServiceNrCellInfo;

/* PLMN结构定义 */
typedef struct {
    LogServiceUInt mcc; /* MCC */
    LogServiceUInt mnc; /* MNC */
} LogServicePlmnId;

/* LTE信号数据结构 */
typedef struct {
    LogServiceShort rsrp; /* RSRP */
    LogServiceShort rsrq; /* RSRQ */
    LogServiceShort rssi; /* RSSI */
    LogServiceShort sinr; /* SINR */
} LogServiceLteSignalInfo;

/* WCDMA信号数据结构 */
typedef struct {
    LogServiceShort rscp; /* RSCP */
    LogServiceShort ecio; /* ECIO */
    LogServiceShort rssi; /* RSSI */
    LogServiceShort rsv; /* 保留 */
} LogServiceWcdmaSignalInfo;

/* 5G信号数据结构 */
typedef struct {
    LogServiceShort rsrp; /* RSRP */
    LogServiceShort rsrq; /* RSRQ */
    LogServiceShort sinr; /* SINR */
    LogServiceShort rsv; /* 保留 */
} LogServiceNrSignalInfo;

/* 信号参数结构 */
typedef struct {
    LogServiceRatType rat; /* 接入技术 */

    union {
        LogServiceLteSignalInfo lteSigInfo; /* LTE的信号内容 */
        LogServiceWcdmaSignalInfo wcdmaSigInfo; /* WCDMA的信号内容 */
        LogServiceNrSignalInfo nrSigInfo; /* 5G的信号内容 */
    } u;
} LogServiceParamSignalInfo;

/* 小区频段和频点结构定义 */
typedef struct {
    LogServiceRatType rat; /* 网络制式 */
    LogServiceUInt frequency; /* 小区频点 */
    LogServiceUShort bandwidth; /* 小区带宽 */
    LogServiceUChar band; /* 当前频段 */
    LogServiceUChar rsv; /* 保留位 */
} LogServiceParamFreqInfo;

/* NAS过程被拒参数结构定义 */
typedef struct {
    LogServiceUChar origCause; /* 原始原因值 */
    LogServiceUChar cause; /* 拒绝原因值 */
    LogServiceUChar rsv[2]; /* 保留 */
} LogServiceParamRejInfo;

/* LTE ENDC信息参数结构定义 */
typedef struct {
    LogServiceUChar restrictDcnr; /* 是否限制DCNR */
    LogServiceUChar iwkN26; /* 是否支持N26接口 */
    LogServiceUChar upperLayerInd; /* SIB2是否携带upperLayerIndication */
    LogServiceUChar rsv; /* 保留 */
} LogServiceParamEndcInfo;

/* SYSCFGEX制式参数结构体定义 */
typedef struct {
    LogServiceUChar ratNum; /* 接入技术个数 */
    LogServiceUChar ratOrder[LOG_SERVICE_MAX_RAT_NUM]; /* 接入技术列表 */
    LogServiceUChar rsv; /* 保留 */
} LogServiceParamSyscfgexRatInfo;

/* SYSCFGEX频段参数结构体定义 */
typedef struct {
    LogServiceUInt bandLow; /* 低32频段 */
    LogServiceUInt bandHigh; /* 高32频段 */
} LogServiceParamSyscfgexBandInfo;

/* SYSCFGEX参数结构体定义 */
typedef struct {
    LogServiceUChar roam; /* 漫游状态 */
    LogServiceUChar srvDomain; /* 服务域 */
    LogServiceUChar rsv1; /* 保留 */
    LogServiceUChar rsv2; /* 保留 */
    LogServiceParamSyscfgexRatInfo rat; /* 接入技术 */
    LogServiceParamSyscfgexBandInfo guBand; /* GU频段信息 */
    LogServiceParamSyscfgexBandInfo lteBand; /* LTE频段信息 */
} LogServiceParamSyscfgexInfo;

/* MT DETACH参数结构定义 */
typedef struct {
    LogServiceRatType rat; /* 当前制式 */
    LogServiceUShort origCause; /* Detach原始原因 */
    LogServiceUShort cause; /* Detach原因 */
    LogServiceUChar type; /* Detach类型 */
    LogServiceUChar rsv[3]; /* 保留 */
} LogServiceParamMtDetachInfo;

/* EHPLMN结构定义 */
typedef struct {
    LogServiceUChar eHPlmnNum; /* EHPLMN的个数 */
    LogServiceUChar rsv[3]; /* 预留位 */
    LogServicePlmnId eHPlmnList[LOG_SERVICE_EHPLMN_MAX_NUM];
} LogServiceParamEHPlmnInfo;

/* MSISDN结构定义 */
typedef struct {
    LogServiceUChar msisdnLen; /* MSISDN的长度 */
    LogServiceUChar msisdn[6]; /* MSISDN的前5位和1位结束符 */
    LogServiceUChar rsv[1]; /* 1位预留位 */
} LogServiceParamMsisdnInfo;

/* SIM卡状态参数定义 */
typedef struct {
    LogServiceUChar simState; /* SIM卡状态 */
    LogServiceUChar lockState; /* SIM卡锁状态 */
    LogServiceUChar rsv[2]; /* 保留 */
} LogServiceParamSimStateInfo;

/* SIM卡ATR信息 */
typedef struct {
    LogServiceUInt atrLen; /* ATR长度 */
    LogServiceUChar atr[LOG_SERVICE_MAX_ATR_LEN]; /* ATR内容 */
} LogServiceParamSimAtrInfo;

/* 拨号参数结构 */
typedef struct {
    LogServiceUChar cid; /* CID */
    LogServiceUChar usrCid; /* User CID, 高通平台不涉及 */
    LogServiceUChar apn[LOG_SERVICE_MAX_APN_LEN]; /* APN内容 */
    LogServiceUChar qci; /* QCI */
    LogServiceIpType ipType; /* 拨号类型 */
    LogServiceAuthType authType; /* 鉴权类型 */
    LogServiceUInt dlGBR; /* 最大保证比特率 */
    LogServiceUInt dlMBR; /* 最大传输比特率 */
    LogServiceUInt errCause; /* 错误码 */
} LogServiceParamDialInfo;

/* 开户速率信息 */
typedef struct {
    LogServiceUInt dlApnAmbr; /* 下行速率值,单位Kbps */
    LogServiceUInt ulApnAmbr; /* 上行速率值,单位Kbps */
} LogServiceParamApnAmbrInfo;

/* EPS承载激活信息 */
typedef struct {
    LogServiceUChar bearerType; /* 对应EPS承载类型 */
    LogServiceUChar pdpType; /* 对应EPS承载IP类型 */
    LogServiceUChar rsv[2]; /* 预留位 */
    LogServiceUInt epsBearerId; /* 对应EPS承载ID */
    LogServiceUInt linkedEpsBearerId; /* 与此承载关联的EPS承载ID */
    LogServiceUChar apn[LOG_SERVICE_MAX_APN_LEN];
} LogServiceParamEpsBearerInfo;

/* 短信接收到MM发送的L/G MM_ERROR_IND时相关信息结构 */
typedef struct {
    LogServiceSmsDirection smsDir; /* 短信方向 */
    LogServiceUInt error; /* MM错误码 */
    LogServiceUChar smsMr; /* MR值 */
    LogServiceUChar smrStatus; /* SMR状态 */
    LogServiceUChar rsv[2]; /* 保留 */
} LogServiceParamMmError;

/* 短信存储介质相关信息 */
typedef struct {
    LogServiceSmsMemType memType; /* 存储介质 */
    LogServiceSmsMemDStatus memStatus; /* 存储状态 */
} LogServiceParamSmsMem;

/* Mo Ctrl/Fdn Ctrl相关信息 */
typedef struct {
    LogServiceSmsCheckType type; /* 短信校验类型 */
    LogServiceUInt cause; /* 校验失败原因值 */
} LogServiceParamCheckResult;

/* 呼叫通话编码方式相关信息结构 */
typedef struct {
    LogServiceUChar codecType;
    LogServiceUChar voiceDomain;
    LogServiceUChar rsv1;
    LogServiceUChar rsv2;
} LogServiceParamVoiceEncode;

/* IMS语音编码方式相关信息结构 */
typedef struct {
    LogServiceUChar codecType;
    LogServiceUChar voiceDomain;
    LogServiceUChar rsv1;
    LogServiceUChar rsv2;
} LogServiceParamImsEncode;

/* 呼叫挂断相关信息结构 */
typedef struct {
    LogServiceUInt preCallTime; /* 上一次呼叫的时长 */
    LogServiceUInt cause; /* 呼叫挂断原因值 */
} LogServiceParamVoiceCendInfo;

/* 呼叫发起失败相关信息结构 */
typedef struct {
    LogServiceVoiceOrgType orgCallType; /* 终端始发呼叫的类型 */
    LogServiceUInt cause; /* 呼叫失败的原因值 */
} LogServiceParamVoiceOrgFailure;

/* 短信状态信息 */
typedef struct {
    LogServiceUChar mr; /* MR */
    LogServiceUChar mmEstStatus; /* MM建链状态 */
    LogServiceUChar rsv[2]; /* 保留 */
    LogServiceSmsStatus smsStatus; /* 短信状态 */
    LogServiceSmsDomain smsDomain; /* 短信域 */
    LogServiceUInt error; /* 错误信息 */
} LogServiceParamSmsStatus;

/* 短信接收到mm发送的MMSMS_REL_IND时相关信息结构 */
typedef struct {
    LogServiceSmsDirection smsDir; /* 短信方向 */
    LogServiceUChar mr; /* 短信MR */
    LogServiceUChar smcStatus; /* SMC状态 */
    LogServiceUChar rsv[2]; /* 保留 */
    LogServiceUInt relCause; /* MM释放原因 */
} LogServiceParamMmRelInfo;

/* LENDC消息内容 */
typedef struct {
    LogServiceUInt endcAvailableFlag;
    LogServiceUInt endcPlmnAvailableFlag;
    LogServiceUInt endcEpcRestrictedFlag;
    LogServiceUInt nrPscellFlag;
} LogServiceParamEndcState;

/* 搜网优化方案消息相关信息结构 */
typedef struct {
    LogServiceUChar rat; /* 制式信息 */
    LogServiceUChar band; /* 频段号 */
    LogServiceUChar rsv[2]; /* 保留 */
    LogServiceUInt scene; /* 场景 */
    LogServiceUInt operate; /* 动作 */
} LogServiceParamNetSelOptInfo;

/* ECALL起呼消息相关信息结构 */
typedef struct {
    LogServiceUChar callType; /* 通话类型 */
    LogServiceUChar ecallOpMode; /* MSD传输模式 */
} LogServiceParamEcallStartInfo;

/* ECALL定时器相关信息结构 */
typedef struct {
    LogServiceParamTimerId eCallTimerId;
    LogServiceUChar ecallTimerStatus;
    LogServiceUChar rsv[3]; /* 保留 */
} LogServiceParamEcallTimerInfo;

/* ECALL MSD状态相关信息结构 */
typedef struct {
    LogServiceUChar ecallTransStatus;
    LogServiceUChar rsv1; /* 保留 */
    LogServiceUInt ecallTransFailCause;
} LogServiceParamEcallStatusInfo;

typedef struct {
    LogServiceUInt vsyncCurrentState; /* 当前同步状态 */
    LogServiceUInt vsyncOrigType; /* 原始同步源 */
    LogServiceUInt vsyncCurrentType; /* 当前同步源 */
    LogServiceUInt cbrCurrentLvl; /* CBR当前拥塞分级 */
    LogServiceUInt ltevMode; /* VMODE 模式设置值 */
} LogServiceParamLtevInfo;

/* 外部调用统一使用如下结构的共用体参数 */
typedef union {
    /* 移动性事件的参数结构 */
    LogServiceSrvStat paramSrvState;
    LogServiceParamFreqInfo paramFreqInfo;
    LogServiceParamRejInfo paramNasRejInfo;
    LogServiceParamSignalInfo paramSignalInfo;
    LogServiceParamMtDetachInfo paramMtDetachInfo;
    LogServiceParamEndcInfo paramEndcCapInfo;
    LogServiceParamSyscfgexInfo paramSyscfgexInfo;
    LogServiceUInt paramNwAuthRej;
    LogServiceUInt paramLocalDetach;
    LogServiceUInt paramSmcFailCause;
    LogServiceParamEndcState paramEndcState;
    LogServiceRrcStat paramRrcStat;
    LogServiceUInt paramRat;
    LogServiceParamNetSelOptInfo paramNetSelOptInfo;
    LogServiceParamLtevInfo paramLtevInfo;

    /* 拨号事件的参数结构 */
    LogServiceParamDialInfo paramDialInfo;
    LogServiceParamEpsBearerInfo paramEpsBearerInfo;
    LogServiceParamApnAmbrInfo paramApnAmbrInfo;
    /* CS语音事件的参数结构 */
    LogServiceUInt paramCsCallId;
    LogServiceUChar paramRedailCause;
    LogServiceParamVoiceEncode paramVoiceEncode;
    LogServiceParamVoiceCendInfo paramVoiceCendInfo;
    LogServiceParamVoiceOrgFailure paramVoiceOrgFailure;

    /* 数据业务事件的参数结构 */
    LogServiceParamRejInfo paramServiceRejInfo;

    /* SIM事件的参数结构 */
    LogServiceParamSimStateInfo paramSimStateInfo;
    LogServiceParamSimOperation paramSimOperation;
    LogServiceParamSimlockState paramSimLockState;
    LogServiceParamMandatoryFileId paramMissMandatoryFile;
    LogServiceParamSimAtrInfo paramSimAtrInfo;
    LogServiceParamAidInfo paramSimAidInfo;
    LogServiceParamCardState paramCardState;
    LogServiceUInt paramSimAuthFailure;
    LogServiceParamEHPlmnInfo paramEHPlmnInfo;
    LogServiceParamMsisdnInfo paramMsisdnInfo;

    /* SMS事件的参数结构 */
    LogServiceParamSmsMem paramSmsMemInfo;
    LogServiceParamMmError paramSmsError;
    LogServiceParamCheckResult paramSmsCheckResult;
    LogServiceParamSmsStatus paramSmsStatus;
    LogServiceParamMmRelInfo paramMmRelInfo;

    /* IMS事件的参数结构 */
    LogServiceUShort paramImsCallId;
    LogServiceUShort paramSipStatus;
    LogServiceUShort paramVoiceDomain;
    LogServiceUInt paramSrvccStatus;
    LogServiceUInt paramImsPdnErrCause;
    LogServiceUInt paramSrvccCancelCause;
    LogServiceParamImsEncode paramImsEncode;

    /* 其他事件的参数结构 */
    LogServiceUChar paramPowerMode;
    LogServiceParamTimerId paramTimerId;
    LogServiceUInt paramApSleepStat;

    /* ECALL事件的参数结构 */
    LogServiceParamEcallStartInfo paramEcallStartInfo;
    LogServiceUChar ecallAlackValue;
    LogServiceParamEcallTimerInfo paramEcallTimerInfo;
    LogServiceParamEcallStatusInfo paramMsdStatusInfo;
    LogServiceUChar nwEclSupportFlag;
    LogServiceUChar ecallDomainMode;
} LogServiceParamInfo;

/* 模块上下文结构定义 */
typedef struct {
    LogServiceRatType rat; /* 当前网络制式 */
    LogServiceSrvStat srvStat; /* 当前服务状态 */
    LogServiceRrcStat rrcStat; /* 当前RRC状态 */
    LogServiceUInt mrdcFlag; /* MRDC状态标识 */
    LogServiceNrCellInfo nrCellInfo; /* NR服务小区信息上下文 */
    LogServiceLteCellInfo lteCellInfo; /* L服务小区信息上下文 */
    LogServiceUChar apn[LOG_SERVICE_MAX_APN_LEN]; /* 当前拨号APN */
    LogServicePlmnId simPlmn; /* SIM卡的HPLMN */
    LogServiceUInt eHPlmnPrintFlag; /* 表示当前EHPLMN是否打印 */
    LogServiceParamSmsStatus smsStatus; /* 短信状态相关内容 */
    LogServiceParamSyscfgexInfo syscfgexInfo; /* SYSCFGEX设置内容 */
    LogServiceUInt apSleepStat; /* AP休眠状态，0:Sleep，1:Wake Up */
    LogServiceUInt powerOffStat; /* 当前是否处于关机状态 */
} LogServiceContext;

/* 给MLOG提供的RAT list结构体信息 */
typedef struct {
    VOS_UINT8                 ratNum;
    TAF_MLOG_NetRatTypeUint8  ratPrio[TAF_MLOG_MAX_RAT_NUM];
#if ((FEATURE_LTE == FEATURE_ON) && (FEATURE_UE_MODE_NR == FEATURE_ON))
    VOS_UINT8 rsv1[3];
#elif ((FEATURE_OFF == FEATURE_LTE) && (FEATURE_OFF == FEATURE_UE_MODE_NR))
    VOS_UINT8 rsv2;
#endif
} TAF_MLOG_RatPrio;

/* 给MOLG提供的RAT list结构体信息 */
typedef struct {
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
} TAF_MLOG_PlmnId;

/* 给MOLG提供的LTE Band支持的结构体信息 */
typedef struct {
    TAF_UINT32 bandInfo[TAF_MLOG_LTE_BAND_MAX_LENGTH]; /* 共8个U32字段标识LTE频段 */
} TAF_MLOG_SetLtePrefBandInfo;

/* 给MOLG提供的信息结构体 */
typedef struct {
    VOS_UINT8                   plmnSelMode;
    VOS_UINT8                   reserve[3];
    TAF_MLOG_RatPrio            prioRatList;        /* 接入技术以及优先级 */
    TAF_MLOG_PlmnId             campPlmn;
    TAF_MLOG_SetLtePrefBandInfo lteBandInfo;
    TAF_MLOG_PlmnId             hplmn;
} TAF_MlogEventCtx;

/**************************************对外接口定义*****************************************/
VOS_VOID TAF_GetMlogEventCtx(TAF_MlogEventCtx  *mlogEvent);
/* 协议事件收集处理函数 */
LogServiceVoid NO_OPTIMIZE_WEAK LOG_SERVICE_EvtCollect(LogServiceEventId event, LogServiceParamInfo *param);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
/* NRSM协议事件收集处理函数 */
LogServiceUInt NO_OPTIMIZE_WEAK LOG_SERVICE_NrsmEvtCollect(LogServiceUInt eventType, struct MsgCB *msg);
/* NRMM协议事件收集处理函数 */
LogServiceUInt NO_OPTIMIZE_WEAK LOG_SERVICE_NrmmEvtCollect(LogServiceUInt eventType, struct MsgCB *msg);
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __MBB_LOG_SERVICE_API_H__ */
