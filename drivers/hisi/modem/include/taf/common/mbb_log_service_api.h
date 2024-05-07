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
/*****************************************ͷ�ļ�********************************************/
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
/*****************************************�궨��********************************************/
/* ����/ע��/�ƶ��� */
#define LOG_SERVICE_FUNCTION_MOBILITY FEATURE_ON
/* ���� */
#define LOG_SERVICE_FUNCTION_PS_CALL FEATURE_ON
/* CS���� */
#define LOG_SERVICE_FUNCTION_CS_CALL FEATURE_ON
/* ����ҵ�� */
#define LOG_SERVICE_FUNCTION_DATA_SERVICE FEATURE_ON
/* SIM�� */
#define LOG_SERVICE_FUNCTION_SIM FEATURE_ON
/* ���� */
#define LOG_SERVICE_FUNCTION_SMS FEATURE_ON
/* VOLTE */
#define LOG_SERVICE_FUNCTION_IMS FEATURE_ON
/* ���� */
#define LOG_SERVICE_FUNCTION_OTHERS FEATURE_ON
/* ECALL */
#if (FEATURE_ECALL == FEATURE_ON)
#define LOG_SERVICE_FUNCTION_ECALL FEATURE_ON
#else
#define LOG_SERVICE_FUNCTION_ECALL FEATURE_OFF
#endif

/* ��������ATR���� */
#define LOG_SERVICE_MAX_ATR_LEN 80

/* APN��󳤶� */
#define LOG_SERVICE_MAX_APN_LEN 100

/* SIM��EHPLMN�������� */
#define LOG_SERVICE_EHPLMN_MAX_NUM 16

/* SYSCFGEX���뼼�������� */
#define LOG_SERVICE_MAX_RAT_NUM TAF_MMA_RAT_BUTT

#if ((FEATURE_LTE == FEATURE_ON) && (FEATURE_UE_MODE_NR == FEATURE_ON))
#define TAF_MLOG_MAX_RAT_NUM 4 /* ��ǰ֧�ֵĽ��뼼������ */
#elif ((FEATURE_ON == FEATURE_LTE) || (FEATURE_ON == FEATURE_UE_MODE_NR))
#define TAF_MLOG_MAX_RAT_NUM 3
#else
#define TAF_MLOG_MAX_RAT_NUM 2 /* ��ǰ֧�ֵ�GU���뼼������ */
#endif
#define TAF_MLOG_LTE_BAND_MAX_LENGTH 8

/**************************************ö�ٽṹ����*****************************************/
/* Э���¼����� */
typedef enum {
    LOG_SERVICE_EVT_NONE = 0,

#if (LOG_SERVICE_FUNCTION_MOBILITY == FEATURE_ON)
    /* ����/ע��/�ƶ����¼����� */
    LOG_SERVICE_EVT_MOBILITY_LMM_REG_REJ, /* LTEע�ᱻ�� */
    LOG_SERVICE_EVT_MOBILITY_LMM_REG_ACP, /* LTEע��ɹ� */
    LOG_SERVICE_EVT_MOBILITY_LMM_TAU_REJ, /* TAU���� */
    LOG_SERVICE_EVT_MOBILITY_LMM_TAU_ACP, /* TAU�ɹ� */
    LOG_SERVICE_EVT_MOBILITY_GMM_REG_REJ, /* GMMע�ᱻ�� */
    LOG_SERVICE_EVT_MOBILITY_GMM_REG_ACP, /* GMMע��ɹ� */
    LOG_SERVICE_EVT_MOBILITY_GMM_RAU_REJ, /* RAU���� */
    LOG_SERVICE_EVT_MOBILITY_GMM_RAU_ACP, /* RAU�ɹ� */
    LOG_SERVICE_EVT_MOBILITY_MM_LAU_REJ, /* LAU���� */
    LOG_SERVICE_EVT_MOBILITY_MM_LAU_ACP, /* LAU�ɹ� */
    LOG_SERVICE_EVT_MOBILITY_MT_DETACH, /* MT DETACH */
    LOG_SERVICE_EVT_MOBILITY_LOCAL_DETACH, /* ����ȥע�� */
    LOG_SERVICE_EVT_MOBILITY_SIGNAL_INFO, /* �ź���Ϣ */
    LOG_SERVICE_EVT_MOBILITY_CELL_FREQ_INFO, /* С��Ƶ����Ϣ */
    LOG_SERVICE_EVT_MOBILITY_CELL_LOC_INFO, /* С��λ����Ϣ */
    LOG_SERVICE_EVT_MOBILITY_SRV_STATE, /* ����״̬ */
    LOG_SERVICE_EVT_MOBILITY_PLMN_SEARCH_START, /* �������� */
    LOG_SERVICE_EVT_MOBILITY_SMC_FAILURE, /* SMCʧ�� */
    LOG_SERVICE_EVT_MOBILITY_NW_AUTH_REJ, /* �����Ȩ�ܾ� */
    LOG_SERVICE_EVT_MOBILITY_ENDC_STATE_CHANGE, /* ENDC״̬��� */
    LOG_SERVICE_EVT_MOBILITY_SYSCFGEX_SET, /* SYSCFGEX�������� */
    LOG_SERVICE_EVT_MOBILITY_RRC_STAT, /* RRC״̬�仯 */
    LOG_SERVICE_EVT_MOBILITY_RAT_CHANGE, /* ��ģ��ʽ�����仯 */
    LOG_SERVICE_EVT_MOBILITY_NET_SEL_OPT_CNF, /* �����Ż�����ִ�н���¼� */
    LOG_SERVICE_EVT_MOBILITY_NET_SEL_OPT_RPT, /* �����Ż�����״̬�ϱ��¼� */
    LOG_SERVICE_EVT_MOBILITY_LTEV_STATE_CHANGE, /* ��ǰͬ��״̬�����仯 */
    LOG_SERVICE_EVT_MOBILITY_LTEV_TYPE_CHANGE, /* ��ǰͬ��Դ�����仯 */
    LOG_SERVICE_EVT_MOBILITY_LTEV_LEVEL_CHANGE, /* ��ǰCBR��ǰӵ���ּ������仯 */
    LOG_SERVICE_EVT_MOBILITY_LTEV_MODE_INFO, /* VMODE ģʽ������Ϣ */
#endif

#if (LOG_SERVICE_FUNCTION_PS_CALL == FEATURE_ON)
    /* PS CALL�¼����� */
    LOG_SERVICE_EVT_PS_CALL_REJ, /* ���ű����¼� */
    LOG_SERVICE_EVT_PS_CALL_REQ_FAILURE, /* �������󷵻�ʧ�� */
    LOG_SERVICE_EVT_PS_CALL_REL_FAILURE, /* �Ͽ��������󷵻�ʧ�� */
    LOG_SERVICE_EVT_PS_CALL_EXP_PDP_DEACT, /* ����ʼ���Ĳ��ŶϿ��¼� */
    LOG_SERVICE_EVT_PS_CALL_SETUP_REQ, /* ���������¼� */
    LOG_SERVICE_EVT_PS_CALL_REL_REQ, /* �Ͽ����������¼� */
    LOG_SERVICE_EVT_PS_CALL_SETUP, /* ���ųɹ��¼� */
    LOG_SERVICE_EVT_PS_CALL_PDN_APN_AMBR, /* ����APN���ʴ�ӡ�¼� */
    LOG_SERVICE_EVT_PS_CALL_REL, /* �Ͽ����ųɹ��¼� */
    LOG_SERVICE_EVT_PS_CALL_MT_PDP_MODIFY, /* PDP������Modify�¼� */
    LOG_SERVICE_EVT_PS_CALL_EPS_BEARER_INFO, /* EPS������Ϣ */
#endif

#if (LOG_SERVICE_FUNCTION_CS_CALL == FEATURE_ON)
    /* CS CALL�¼����� */
    LOG_SERVICE_EVT_CS_CALL_CEND_CNF, /* ���йҶ�Cend�¼� */
    LOG_SERVICE_EVT_CS_CALL_ORIGIN_CALL_FAILURE, /* ���з����¼� */
    LOG_SERVICE_EVT_CS_CALL_REDIAL, /* �����ز��¼� */
    LOG_SERVICE_EVT_CS_CALL_SRVCC_CANCEL, /* ����SRVCCȡ���¼� */
    LOG_SERVICE_EVT_CS_CALL_ENCODE_INFO, /* ���б�����Ϣ */
    LOG_SERVICE_EVT_CS_CALL_INCOMING_CALL, /* ���к����¼� */
    LOG_SERVICE_EVT_CS_CALL_ORIGIN_CALL_REQ, /* ���з��������¼� */
#endif

#if (LOG_SERVICE_FUNCTION_DATA_SERVICE == FEATURE_ON)
    /* ����ҵ���¼����� */
    LOG_SERVICE_EVT_DATA_SERVICE_LMM_SER_REJ, /* LTE Service�����¼� */
    LOG_SERVICE_EVT_DATA_SERVICE_GMM_SER_REJ, /* GMM Service�����¼� */
#endif

#if (LOG_SERVICE_FUNCTION_SIM == FEATURE_ON)
    /* SIM���¼����� */
    LOG_SERVICE_EVT_SIM_CARD_INOUT, /* SIM������¼� */
    LOG_SERVICE_EVT_SIM_MANDATORY_FILE_MISS, /* ��ѡ�ļ�ʧ�� */
    LOG_SERVICE_EVT_SIM_GET_ATR_FAIL, /* ��ȡATRʧ�� */
    LOG_SERVICE_EVT_SIM_ATR_ERROR, /* ATR���� */
    LOG_SERVICE_EVT_SIM_SIMST, /* SIM��״̬�ϱ� */
    LOG_SERVICE_EVT_SIM_LOCK_STATE, /* SIMLOCK״̬�ϱ� */
    LOG_SERVICE_EVT_SIM_AID_TYPE, /* AID�ļ��е�Ӧ������ */
    LOG_SERVICE_EVT_SIM_CARD_STATE, /* PIN/PUK��״̬ */
    LOG_SERVICE_EVT_SIM_STATUS_CMD_ERROR, /* STATUS��ѯʧ�� */
    LOG_SERVICE_EVT_SIM_AUTH_REJ, /* SIM����Ȩʧ�� */
    LOG_SERVICE_EVT_SIM_EHPLMN_INFO, /* EHPLMN��Ϣ */
    LOG_SERVICE_EVT_SIM_MSISDN_INFO, /* MSISDN��Ϣ */
#endif

#if (LOG_SERVICE_FUNCTION_SMS == FEATURE_ON)
    /* �����¼����� */
    LOG_SERVICE_EVT_SMS_SEND_IN_CACHE, /* ���ű������¼� */
    LOG_SERVICE_EVT_SMS_MEM_STATUS, /* ���Ŵ洢����״̬�¼� */
    LOG_SERVICE_EVT_SMS_CP_ERROR, /* ���ŷ���ʱ�յ�cp-error�¼� */
    LOG_SERVICE_EVT_SMS_MM_ERROR, /* ����ҵ��������յ�MM�����¼� */
    LOG_SERVICE_EVT_SMS_MM_REL, /* ����ҵ��������յ�MM�ͷ��¼� */
    LOG_SERVICE_EVT_SMS_SEND_DOMAIN, /* ���ŷ��ͳ����� */
    LOG_SERVICE_EVT_SMS_SEND_START, /* ���ŷ��Ϳ�ʼ�¼� */
    LOG_SERVICE_EVT_SMS_SEND_FAILED, /* ���ŷ���ʧ���¼� */
    LOG_SERVICE_EVT_SMS_MM_EST_STATUS, /* ����MM�����¼� */
    LOG_SERVICE_EVT_SMS_RCV_START, /* ���ն��ſ�ʼ�¼� */
    LOG_SERVICE_EVT_SMS_SEND_SUC, /* ���ŷ��ͳɹ��¼� */
    LOG_SERVICE_EVT_SMS_CHECK_RESULT, /* FDN/MO CtrlУ���� */
    LOG_SERVICE_EVT_SMS_SEND_ABORT, /* �����ж��¼� */
#endif

#if (LOG_SERVICE_FUNCTION_IMS == FEATURE_ON)
    /* IMS�¼����� */
    LOG_SERVICE_EVT_IMS_PDN_ACT_REJ, /* IMS PDN������¼� */
    LOG_SERVICE_EVT_IMS_PDN_DEACT, /* IMS PDNȥ�����¼� */
    LOG_SERVICE_EVT_IMS_REG_STATUS, /* IMSע��״̬�¼� */
    LOG_SERVICE_EVT_IMS_SRVCC_STATUS, /* IMS SRVCC״̬�¼� */
    LOG_SERVICE_EVT_IMS_ENCODE_INFO, /* IMS���б�����Ϣ */
    LOG_SERVICE_EVT_IMS_INCOMING_CALL, /* IMS��������ָʾ */
#endif

#if (LOG_SERVICE_FUNCTION_OTHERS == FEATURE_ON)
    /* �����¼����� */
    LOG_SERVICE_EVT_OTHERS_TIMER_EXPIRED, /* Э�鶨ʱ����ʱ�¼� */
    LOG_SERVICE_EVT_OTHERS_COMMON_PRINT, /* ͨ�ô�ӡ�¼� */
    LOG_SERVICE_EVT_OTHERS_POWER_OFF, /* Э��ػ��¼� */
    LOG_SERVICE_EVT_OTHERS_POWER_ON, /* Э�鿪���¼� */
    LOG_SERVICE_EVT_OTHERS_ACORE_STAT_CHANGE, /* A�����߻���״̬�仯 */
#endif

#if (LOG_SERVICE_FUNCTION_ECALL == FEATURE_ON)
    /* ECALL�¼����� */
    LOG_SERVICE_EVT_ECALL_MSD_ABORT, /* ECALL MSD�����ж��¼�(IN-BAND) */
    LOG_SERVICE_EVT_ECALL_START_TYPE, /* ECALL���з��������¼� */
    LOG_SERVICE_EVT_ECALL_START_MODE, /* ECALL���з���ģʽ�¼� */
    LOG_SERVICE_EVT_ECALL_MSD_ALACK_IND, /* ECALL MSD������յ�ALACK�¼�(IN-BAND) */
    LOG_SERVICE_EVT_ECALL_TIMER_STATUS, /* ECALL TIMER״̬�¼�(T9,T10) */
    LOG_SERVICE_EVT_ECALL_SIP_STATUS_IND, /* ECALL SIP MSD����״̬�¼� */
    LOG_SERVICE_EVT_ECALL_INBAND_STATUS_IND, /* ECALL IN-BAND MSD����״̬�¼� */
    LOG_SERVICE_EVT_ECALL_ECL_SUPPORT_IND, /* ECALL ECL�Ƿ�֧���¼� */
    LOG_SERVICE_EVT_ECALL_MSD_IMSECLCFG_SET, /* ECALL ECLIMSCFG�����¼� */
#endif

    LOG_SERVICE_EVT_BUTT,
} LogServiceEventId;

/* ģ���ڲ����뼼��ö�ٶ��� */
typedef enum {
    LOG_SERVICE_RAT_NONE = 0, /* INVALID */
    LOG_SERVICE_RAT_GSM, /* GSM */
    LOG_SERVICE_RAT_WCDMA, /* WCDMA */
    LOG_SERVICE_RAT_LTE, /* LTE */
    LOG_SERVICE_RAT_TDS, /* TDS */
    LOG_SERVICE_RAT_NR, /* NR */

    LOG_SERVICE_RAT_BUTT,
} LogServiceRatType;

/* ģ���ڲ�����״̬ö�ٶ��� */
typedef enum {
    LOG_SERVICE_SRVSTAT_NONE = 0, /* �޷��� */
    LOG_SERVICE_SRVSTAT_LIMIT, /* ���Ʒ��� */
    LOG_SERVICE_SRVSTAT_NORMAL, /* �������� */

    LOG_SERVICE_SRVSTAT_BUTT,
} LogServiceSrvStat;

/* Э�鶨ʱ��ö�ٶ��� */
typedef enum {
    LOG_SERVICE_TIMER_NONE = 0,
    /* LMM */
    LOG_SERVICE_TIMER_T3410, /* ATTACH��ʱ�� */
    LOG_SERVICE_TIMER_T3417, /* Service Request��ʱ�� */
    LOG_SERVICE_TIMER_T3430, /* TAU��ʱ�� */
    LOG_SERVICE_TIMER_T3482, /* PDN���Ӷ�ʱ�� */
    /* GMM */
    LOG_SERVICE_TIMER_T3310, /* ATTACH��ʱ�� */
    LOG_SERVICE_TIMER_T3317, /* Service Request��ʱ�� */
    LOG_SERVICE_TIMER_T3330, /* RAU��ʱ�� */
    LOG_SERVICE_TIMER_T3380, /* PDP���ʱ�� */
    /* SMS */
    LOG_SERVICE_TIMER_TR1M, /* SMS MO */
    LOG_SERVICE_TIMER_TC1M_MO, /* SMS MO */
    LOG_SERVICE_TIMER_TC1M_MT, /* SMS MT */
    LOG_SERVICE_TIMER_TR2M, /* SMS MT */
    LOG_SERVICE_TIMER_RETRY_PERIOD, /* SMS�������ڶ�ʱ�� */
    LOG_SERVICE_TIMER_RETRY_INTERVAL, /* SMS���ͼ����ʱ�� */
    /* ECALL */
    LOG_SERVICE_TIMER_T9, /* ECALL T9��ʱ�� */
    LOG_SERVICE_TIMER_T10, /* ECALL T10��ʱ�� */

    LOG_SERVICE_TIMER_BUTT,
} LogServiceParamTimerId;

/* SIM���β�״̬���� */
typedef enum {
    LOG_SERVICE_SIM_REMOVE = 0, /* SIM���γ��¼� */
    LOG_SERVICE_SIM_INSERT, /* SIM�������¼� */
} LogServiceParamSimOperation;

/* SIMLOCK״̬���� */
typedef enum {
    LOG_SERVICE_SIMLOCK_UNLOCKED = 0, /* �Ϸ��� */
    LOG_SERVICE_SIMLOCK_LOCKED, /* �Ƿ��� */
} LogServiceParamSimlockState;

/* SIM���ϵ��ѹ�������� */
typedef enum {
    LOG_SERVICE_SIM_VOLTAGE_1P8V = 1, /* 1.8V */
    LOG_SERVICE_SIM_VOLTAGE_3P0V, /* 3.0V */
    LOG_SERVICE_SIM_VOLTAGE_5P0V, /* 5.0V */
} LogServiceParamSimVoltage;

/* SIM����ѡ�ļ��б� */
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

/* ����SIM����AID״̬ */
typedef enum {
    LOG_SERVICE_SIM_AID_USIM = 0x01, /* USIM���� */
    LOG_SERVICE_SIM_AID_ISIM = 0x02, /* ISIM���� */
    LOG_SERVICE_SIM_AID_CSIM = 0x04, /* CSIM���� */
} LogServiceParamAidInfo;

/* SIM��״̬ */
typedef enum {
    LOG_SERVICE_SIM_CARD_PIN = 0x00, /* PIN������ */
    LOG_SERVICE_SIM_CARD_PUK, /* PUK������ */
    LOG_SERVICE_SIM_CARD_ABSENT, /* SIM������λ */
    LOG_SERVICE_SIM_CARD_AVAILABLE, /* SIM������ */
    LOG_SERVICE_SIM_CARD_UNAVAILABLE, /* SIM�������� */
} LogServiceParamCardState;

/* ���ŷ������� */
typedef enum {
    LOG_SERVICE_SMS_DIR_MO = 0, /* ���Ͷ��� */
    LOG_SERVICE_SMS_DIR_MT, /* ���ն��� */
} LogServiceSmsDirection;

/* IP���Ͷ��� */
typedef enum {
    LOG_SERVICE_IP_TYPE_V4 = 1, /* IPV4 */
    LOG_SERVICE_IP_TYPE_V6, /* IPV6 */
    LOG_SERVICE_IP_TYPE_V4V6, /* IPV4V6 */
    LOG_SERVICE_IP_TYPE_PPP, /* PPP */
} LogServiceIpType;

/* ���ż�Ȩ���Ͷ��� */
typedef enum {
    LOG_SERVICE_AUTH_TYPE_NONE = 0, /* �޼�Ȩ */
    LOG_SERVICE_AUTH_TYPE_PAP, /* PAP */
    LOG_SERVICE_AUTH_TYPE_CHAP, /* CHAP */
    LOG_SERVICE_AUTH_TYPE_AUTO, /* AUTO */
} LogServiceAuthType;

/* ���Ŵ洢�������� */
typedef enum {
    LOG_SERVICE_SMS_MEM_APP = 0, /* ���Ŵ洢��Ӧ�� */
    LOG_SERVICE_SMS_MEM_SIM, /* ���Ŵ洢��SIM�� */
} LogServiceSmsMemType;

/* ���Ŵ洢����״̬���� */
typedef enum {
    LOG_SERVICE_SMS_MEM_NOT_FULL = 0, /* ���Ŵ洢δ�� */
    LOG_SERVICE_SMS_MEM_FULL, /* ���Ŵ洢�� */
} LogServiceSmsMemDStatus;

/* ����MO/FDN���� */
typedef enum {
    LOG_SERVICE_SMS_CHECK_FDN = 0, /* FDNУ�� */
    LOG_SERVICE_SMS_CHECK_MO_CTRL, /* MO Control */
} LogServiceSmsCheckType;

/* ���з���ʧ�ܴ������� */
typedef enum {
    LOG_SERVICE_VOICE_ORG_BY_EMC = 0, /* �������� */
    LOG_SERVICE_VOICE_ORG_BY_PH_MODE, /* �������� */
} LogServiceVoiceOrgType;

/* ����״̬ */
typedef enum {
    LOG_SERVICE_SMS_STATUS_SEND_INIT = 0, /* ��ʼ�� */
    LOG_SERVICE_SMS_STATUS_SEND_START, /* ��ʼ������ */
    LOG_SERVICE_SMS_STATUS_SEND_SUCCESS, /* ���ŷ��ͳɹ� */
    LOG_SERVICE_SMS_STATUS_SEND_FAILED, /* ���ŷ���ʧ�� */
    LOG_SERVICE_SMS_STATUS_RECEIVE_START, /* ��ʼ���ն��� */
} LogServiceSmsStatus;

/* ���ų������� */
typedef enum {
    LOG_SERVICE_SMS_DOMAIN_CS = 0, /* ������CS�� */
    LOG_SERVICE_SMS_DOMAIN_PS, /* ������PS�� */
    LOG_SERVICE_SMS_DOMAIN_BUTT,
} LogServiceSmsDomain;

/* SIM��״̬������ͬSIMST�ϱ� */
typedef enum {
    LOG_SERVICE_SIM_STAT_INVALID = 0, /* ��Ч״̬ */
    LOG_SERVICE_SIM_STAT_VALID = 1, /* ����Ч */
    LOG_SERVICE_SIM_STAT_ONLY_PS_VALID = 2, /* ֻ��PS��Ч */
    LOG_SERVICE_SIM_STAT_ONLY_CS_VALID = 3, /* ֻ��CS��Ч */
    LOG_SERVICE_SIM_STAT_PS_CS_INVALID = 4, /* CS/PS����Ч */
    LOG_SERVICE_SIM_STAT_ROMSIM = 240, /* ROMSIM */
    LOG_SERVICE_SIM_STAT_ABSENT = 255, /* SIM������λ */
} LogServiceSimState;

/* GMM Service��������ö�� */
typedef enum {
    LOG_SERVICE_GMM_SER_TYPE_SIGNALING = 0, /* �������� */
    LOG_SERVICE_GMM_SER_TYPE_DATA, /* �������� */
    LOG_SERVICE_GMM_SER_TYPE_PAGING_RESPONSE, /* Ѱ����Ӧ */
    LOG_SERVICE_GMM_SER_TYPE_RESERVED, /* ���� */
} LogServiceGmmServiceReqType;

/* RRC״̬ö�ٶ��� */
typedef enum {
    LOG_SERVICE_RRC_STAT_IDLE = 0, /* RRC����̬ */
    LOG_SERVICE_RRC_STAT_CONN, /* RRC����̬ */
    LOG_SERVICE_RRC_STAT_INACT, /* RRC INACTIVE̬ */
    LOG_SERVICE_RRC_STAT_BUTT, /* RRC��Ч״̬ */
} LogServiceRrcStat;

/* ��ʱ��״̬ö�ٶ��� */
typedef enum {
    LOG_SERVICE_TIMER_STAT_RUNNING = 0, /* ��ʱ������ */
    LOG_SERVICE_TIMER_STAT_STOP, /* ��ʱ��ֹͣ */
    LOG_SERVICE_TIMER_STAT_BUTT, /* ��ʱ����Ч״̬ */
} LogServiceTimerStat;

/*
 * �ṹ˵��: MLOGϵͳģʽö��
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

/**************************************���ݽṹ����*****************************************/
/* LTEС����Ϣ�ṹ���� */
typedef struct {
    LogServiceUInt cellId; /* С��ID */
    LogServiceUInt frequency; /* С��Ƶ�� */
    LogServiceUShort tac; /* С��TAC */
    LogServiceUShort bandwidth; /* С������ */
    LogServiceUChar band; /* ��ǰƵ�� */
    LogServiceUChar rsv[3]; /* ����λ */
} LogServiceLteCellInfo;

/* NRС������Ϣ������� */
typedef LogServiceLteCellInfo LogServiceNrCellInfo;

/* PLMN�ṹ���� */
typedef struct {
    LogServiceUInt mcc; /* MCC */
    LogServiceUInt mnc; /* MNC */
} LogServicePlmnId;

/* LTE�ź����ݽṹ */
typedef struct {
    LogServiceShort rsrp; /* RSRP */
    LogServiceShort rsrq; /* RSRQ */
    LogServiceShort rssi; /* RSSI */
    LogServiceShort sinr; /* SINR */
} LogServiceLteSignalInfo;

/* WCDMA�ź����ݽṹ */
typedef struct {
    LogServiceShort rscp; /* RSCP */
    LogServiceShort ecio; /* ECIO */
    LogServiceShort rssi; /* RSSI */
    LogServiceShort rsv; /* ���� */
} LogServiceWcdmaSignalInfo;

/* 5G�ź����ݽṹ */
typedef struct {
    LogServiceShort rsrp; /* RSRP */
    LogServiceShort rsrq; /* RSRQ */
    LogServiceShort sinr; /* SINR */
    LogServiceShort rsv; /* ���� */
} LogServiceNrSignalInfo;

/* �źŲ����ṹ */
typedef struct {
    LogServiceRatType rat; /* ���뼼�� */

    union {
        LogServiceLteSignalInfo lteSigInfo; /* LTE���ź����� */
        LogServiceWcdmaSignalInfo wcdmaSigInfo; /* WCDMA���ź����� */
        LogServiceNrSignalInfo nrSigInfo; /* 5G���ź����� */
    } u;
} LogServiceParamSignalInfo;

/* С��Ƶ�κ�Ƶ��ṹ���� */
typedef struct {
    LogServiceRatType rat; /* ������ʽ */
    LogServiceUInt frequency; /* С��Ƶ�� */
    LogServiceUShort bandwidth; /* С������ */
    LogServiceUChar band; /* ��ǰƵ�� */
    LogServiceUChar rsv; /* ����λ */
} LogServiceParamFreqInfo;

/* NAS���̱��ܲ����ṹ���� */
typedef struct {
    LogServiceUChar origCause; /* ԭʼԭ��ֵ */
    LogServiceUChar cause; /* �ܾ�ԭ��ֵ */
    LogServiceUChar rsv[2]; /* ���� */
} LogServiceParamRejInfo;

/* LTE ENDC��Ϣ�����ṹ���� */
typedef struct {
    LogServiceUChar restrictDcnr; /* �Ƿ�����DCNR */
    LogServiceUChar iwkN26; /* �Ƿ�֧��N26�ӿ� */
    LogServiceUChar upperLayerInd; /* SIB2�Ƿ�Я��upperLayerIndication */
    LogServiceUChar rsv; /* ���� */
} LogServiceParamEndcInfo;

/* SYSCFGEX��ʽ�����ṹ�嶨�� */
typedef struct {
    LogServiceUChar ratNum; /* ���뼼������ */
    LogServiceUChar ratOrder[LOG_SERVICE_MAX_RAT_NUM]; /* ���뼼���б� */
    LogServiceUChar rsv; /* ���� */
} LogServiceParamSyscfgexRatInfo;

/* SYSCFGEXƵ�β����ṹ�嶨�� */
typedef struct {
    LogServiceUInt bandLow; /* ��32Ƶ�� */
    LogServiceUInt bandHigh; /* ��32Ƶ�� */
} LogServiceParamSyscfgexBandInfo;

/* SYSCFGEX�����ṹ�嶨�� */
typedef struct {
    LogServiceUChar roam; /* ����״̬ */
    LogServiceUChar srvDomain; /* ������ */
    LogServiceUChar rsv1; /* ���� */
    LogServiceUChar rsv2; /* ���� */
    LogServiceParamSyscfgexRatInfo rat; /* ���뼼�� */
    LogServiceParamSyscfgexBandInfo guBand; /* GUƵ����Ϣ */
    LogServiceParamSyscfgexBandInfo lteBand; /* LTEƵ����Ϣ */
} LogServiceParamSyscfgexInfo;

/* MT DETACH�����ṹ���� */
typedef struct {
    LogServiceRatType rat; /* ��ǰ��ʽ */
    LogServiceUShort origCause; /* Detachԭʼԭ�� */
    LogServiceUShort cause; /* Detachԭ�� */
    LogServiceUChar type; /* Detach���� */
    LogServiceUChar rsv[3]; /* ���� */
} LogServiceParamMtDetachInfo;

/* EHPLMN�ṹ���� */
typedef struct {
    LogServiceUChar eHPlmnNum; /* EHPLMN�ĸ��� */
    LogServiceUChar rsv[3]; /* Ԥ��λ */
    LogServicePlmnId eHPlmnList[LOG_SERVICE_EHPLMN_MAX_NUM];
} LogServiceParamEHPlmnInfo;

/* MSISDN�ṹ���� */
typedef struct {
    LogServiceUChar msisdnLen; /* MSISDN�ĳ��� */
    LogServiceUChar msisdn[6]; /* MSISDN��ǰ5λ��1λ������ */
    LogServiceUChar rsv[1]; /* 1λԤ��λ */
} LogServiceParamMsisdnInfo;

/* SIM��״̬�������� */
typedef struct {
    LogServiceUChar simState; /* SIM��״̬ */
    LogServiceUChar lockState; /* SIM����״̬ */
    LogServiceUChar rsv[2]; /* ���� */
} LogServiceParamSimStateInfo;

/* SIM��ATR��Ϣ */
typedef struct {
    LogServiceUInt atrLen; /* ATR���� */
    LogServiceUChar atr[LOG_SERVICE_MAX_ATR_LEN]; /* ATR���� */
} LogServiceParamSimAtrInfo;

/* ���Ų����ṹ */
typedef struct {
    LogServiceUChar cid; /* CID */
    LogServiceUChar usrCid; /* User CID, ��ͨƽ̨���漰 */
    LogServiceUChar apn[LOG_SERVICE_MAX_APN_LEN]; /* APN���� */
    LogServiceUChar qci; /* QCI */
    LogServiceIpType ipType; /* �������� */
    LogServiceAuthType authType; /* ��Ȩ���� */
    LogServiceUInt dlGBR; /* ���֤������ */
    LogServiceUInt dlMBR; /* ���������� */
    LogServiceUInt errCause; /* ������ */
} LogServiceParamDialInfo;

/* ����������Ϣ */
typedef struct {
    LogServiceUInt dlApnAmbr; /* ��������ֵ,��λKbps */
    LogServiceUInt ulApnAmbr; /* ��������ֵ,��λKbps */
} LogServiceParamApnAmbrInfo;

/* EPS���ؼ�����Ϣ */
typedef struct {
    LogServiceUChar bearerType; /* ��ӦEPS�������� */
    LogServiceUChar pdpType; /* ��ӦEPS����IP���� */
    LogServiceUChar rsv[2]; /* Ԥ��λ */
    LogServiceUInt epsBearerId; /* ��ӦEPS����ID */
    LogServiceUInt linkedEpsBearerId; /* ��˳��ع�����EPS����ID */
    LogServiceUChar apn[LOG_SERVICE_MAX_APN_LEN];
} LogServiceParamEpsBearerInfo;

/* ���Ž��յ�MM���͵�L/G MM_ERROR_INDʱ�����Ϣ�ṹ */
typedef struct {
    LogServiceSmsDirection smsDir; /* ���ŷ��� */
    LogServiceUInt error; /* MM������ */
    LogServiceUChar smsMr; /* MRֵ */
    LogServiceUChar smrStatus; /* SMR״̬ */
    LogServiceUChar rsv[2]; /* ���� */
} LogServiceParamMmError;

/* ���Ŵ洢���������Ϣ */
typedef struct {
    LogServiceSmsMemType memType; /* �洢���� */
    LogServiceSmsMemDStatus memStatus; /* �洢״̬ */
} LogServiceParamSmsMem;

/* Mo Ctrl/Fdn Ctrl�����Ϣ */
typedef struct {
    LogServiceSmsCheckType type; /* ����У������ */
    LogServiceUInt cause; /* У��ʧ��ԭ��ֵ */
} LogServiceParamCheckResult;

/* ����ͨ�����뷽ʽ�����Ϣ�ṹ */
typedef struct {
    LogServiceUChar codecType;
    LogServiceUChar voiceDomain;
    LogServiceUChar rsv1;
    LogServiceUChar rsv2;
} LogServiceParamVoiceEncode;

/* IMS�������뷽ʽ�����Ϣ�ṹ */
typedef struct {
    LogServiceUChar codecType;
    LogServiceUChar voiceDomain;
    LogServiceUChar rsv1;
    LogServiceUChar rsv2;
} LogServiceParamImsEncode;

/* ���йҶ������Ϣ�ṹ */
typedef struct {
    LogServiceUInt preCallTime; /* ��һ�κ��е�ʱ�� */
    LogServiceUInt cause; /* ���йҶ�ԭ��ֵ */
} LogServiceParamVoiceCendInfo;

/* ���з���ʧ�������Ϣ�ṹ */
typedef struct {
    LogServiceVoiceOrgType orgCallType; /* �ն�ʼ�����е����� */
    LogServiceUInt cause; /* ����ʧ�ܵ�ԭ��ֵ */
} LogServiceParamVoiceOrgFailure;

/* ����״̬��Ϣ */
typedef struct {
    LogServiceUChar mr; /* MR */
    LogServiceUChar mmEstStatus; /* MM����״̬ */
    LogServiceUChar rsv[2]; /* ���� */
    LogServiceSmsStatus smsStatus; /* ����״̬ */
    LogServiceSmsDomain smsDomain; /* ������ */
    LogServiceUInt error; /* ������Ϣ */
} LogServiceParamSmsStatus;

/* ���Ž��յ�mm���͵�MMSMS_REL_INDʱ�����Ϣ�ṹ */
typedef struct {
    LogServiceSmsDirection smsDir; /* ���ŷ��� */
    LogServiceUChar mr; /* ����MR */
    LogServiceUChar smcStatus; /* SMC״̬ */
    LogServiceUChar rsv[2]; /* ���� */
    LogServiceUInt relCause; /* MM�ͷ�ԭ�� */
} LogServiceParamMmRelInfo;

/* LENDC��Ϣ���� */
typedef struct {
    LogServiceUInt endcAvailableFlag;
    LogServiceUInt endcPlmnAvailableFlag;
    LogServiceUInt endcEpcRestrictedFlag;
    LogServiceUInt nrPscellFlag;
} LogServiceParamEndcState;

/* �����Ż�������Ϣ�����Ϣ�ṹ */
typedef struct {
    LogServiceUChar rat; /* ��ʽ��Ϣ */
    LogServiceUChar band; /* Ƶ�κ� */
    LogServiceUChar rsv[2]; /* ���� */
    LogServiceUInt scene; /* ���� */
    LogServiceUInt operate; /* ���� */
} LogServiceParamNetSelOptInfo;

/* ECALL�����Ϣ�����Ϣ�ṹ */
typedef struct {
    LogServiceUChar callType; /* ͨ������ */
    LogServiceUChar ecallOpMode; /* MSD����ģʽ */
} LogServiceParamEcallStartInfo;

/* ECALL��ʱ�������Ϣ�ṹ */
typedef struct {
    LogServiceParamTimerId eCallTimerId;
    LogServiceUChar ecallTimerStatus;
    LogServiceUChar rsv[3]; /* ���� */
} LogServiceParamEcallTimerInfo;

/* ECALL MSD״̬�����Ϣ�ṹ */
typedef struct {
    LogServiceUChar ecallTransStatus;
    LogServiceUChar rsv1; /* ���� */
    LogServiceUInt ecallTransFailCause;
} LogServiceParamEcallStatusInfo;

typedef struct {
    LogServiceUInt vsyncCurrentState; /* ��ǰͬ��״̬ */
    LogServiceUInt vsyncOrigType; /* ԭʼͬ��Դ */
    LogServiceUInt vsyncCurrentType; /* ��ǰͬ��Դ */
    LogServiceUInt cbrCurrentLvl; /* CBR��ǰӵ���ּ� */
    LogServiceUInt ltevMode; /* VMODE ģʽ����ֵ */
} LogServiceParamLtevInfo;

/* �ⲿ����ͳһʹ�����½ṹ�Ĺ�������� */
typedef union {
    /* �ƶ����¼��Ĳ����ṹ */
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

    /* �����¼��Ĳ����ṹ */
    LogServiceParamDialInfo paramDialInfo;
    LogServiceParamEpsBearerInfo paramEpsBearerInfo;
    LogServiceParamApnAmbrInfo paramApnAmbrInfo;
    /* CS�����¼��Ĳ����ṹ */
    LogServiceUInt paramCsCallId;
    LogServiceUChar paramRedailCause;
    LogServiceParamVoiceEncode paramVoiceEncode;
    LogServiceParamVoiceCendInfo paramVoiceCendInfo;
    LogServiceParamVoiceOrgFailure paramVoiceOrgFailure;

    /* ����ҵ���¼��Ĳ����ṹ */
    LogServiceParamRejInfo paramServiceRejInfo;

    /* SIM�¼��Ĳ����ṹ */
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

    /* SMS�¼��Ĳ����ṹ */
    LogServiceParamSmsMem paramSmsMemInfo;
    LogServiceParamMmError paramSmsError;
    LogServiceParamCheckResult paramSmsCheckResult;
    LogServiceParamSmsStatus paramSmsStatus;
    LogServiceParamMmRelInfo paramMmRelInfo;

    /* IMS�¼��Ĳ����ṹ */
    LogServiceUShort paramImsCallId;
    LogServiceUShort paramSipStatus;
    LogServiceUShort paramVoiceDomain;
    LogServiceUInt paramSrvccStatus;
    LogServiceUInt paramImsPdnErrCause;
    LogServiceUInt paramSrvccCancelCause;
    LogServiceParamImsEncode paramImsEncode;

    /* �����¼��Ĳ����ṹ */
    LogServiceUChar paramPowerMode;
    LogServiceParamTimerId paramTimerId;
    LogServiceUInt paramApSleepStat;

    /* ECALL�¼��Ĳ����ṹ */
    LogServiceParamEcallStartInfo paramEcallStartInfo;
    LogServiceUChar ecallAlackValue;
    LogServiceParamEcallTimerInfo paramEcallTimerInfo;
    LogServiceParamEcallStatusInfo paramMsdStatusInfo;
    LogServiceUChar nwEclSupportFlag;
    LogServiceUChar ecallDomainMode;
} LogServiceParamInfo;

/* ģ�������Ľṹ���� */
typedef struct {
    LogServiceRatType rat; /* ��ǰ������ʽ */
    LogServiceSrvStat srvStat; /* ��ǰ����״̬ */
    LogServiceRrcStat rrcStat; /* ��ǰRRC״̬ */
    LogServiceUInt mrdcFlag; /* MRDC״̬��ʶ */
    LogServiceNrCellInfo nrCellInfo; /* NR����С����Ϣ������ */
    LogServiceLteCellInfo lteCellInfo; /* L����С����Ϣ������ */
    LogServiceUChar apn[LOG_SERVICE_MAX_APN_LEN]; /* ��ǰ����APN */
    LogServicePlmnId simPlmn; /* SIM����HPLMN */
    LogServiceUInt eHPlmnPrintFlag; /* ��ʾ��ǰEHPLMN�Ƿ��ӡ */
    LogServiceParamSmsStatus smsStatus; /* ����״̬������� */
    LogServiceParamSyscfgexInfo syscfgexInfo; /* SYSCFGEX�������� */
    LogServiceUInt apSleepStat; /* AP����״̬��0:Sleep��1:Wake Up */
    LogServiceUInt powerOffStat; /* ��ǰ�Ƿ��ڹػ�״̬ */
} LogServiceContext;

/* ��MLOG�ṩ��RAT list�ṹ����Ϣ */
typedef struct {
    VOS_UINT8                 ratNum;
    TAF_MLOG_NetRatTypeUint8  ratPrio[TAF_MLOG_MAX_RAT_NUM];
#if ((FEATURE_LTE == FEATURE_ON) && (FEATURE_UE_MODE_NR == FEATURE_ON))
    VOS_UINT8 rsv1[3];
#elif ((FEATURE_OFF == FEATURE_LTE) && (FEATURE_OFF == FEATURE_UE_MODE_NR))
    VOS_UINT8 rsv2;
#endif
} TAF_MLOG_RatPrio;

/* ��MOLG�ṩ��RAT list�ṹ����Ϣ */
typedef struct {
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
} TAF_MLOG_PlmnId;

/* ��MOLG�ṩ��LTE Band֧�ֵĽṹ����Ϣ */
typedef struct {
    TAF_UINT32 bandInfo[TAF_MLOG_LTE_BAND_MAX_LENGTH]; /* ��8��U32�ֶα�ʶLTEƵ�� */
} TAF_MLOG_SetLtePrefBandInfo;

/* ��MOLG�ṩ����Ϣ�ṹ�� */
typedef struct {
    VOS_UINT8                   plmnSelMode;
    VOS_UINT8                   reserve[3];
    TAF_MLOG_RatPrio            prioRatList;        /* ���뼼���Լ����ȼ� */
    TAF_MLOG_PlmnId             campPlmn;
    TAF_MLOG_SetLtePrefBandInfo lteBandInfo;
    TAF_MLOG_PlmnId             hplmn;
} TAF_MlogEventCtx;

/**************************************����ӿڶ���*****************************************/
VOS_VOID TAF_GetMlogEventCtx(TAF_MlogEventCtx  *mlogEvent);
/* Э���¼��ռ������� */
LogServiceVoid NO_OPTIMIZE_WEAK LOG_SERVICE_EvtCollect(LogServiceEventId event, LogServiceParamInfo *param);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
/* NRSMЭ���¼��ռ������� */
LogServiceUInt NO_OPTIMIZE_WEAK LOG_SERVICE_NrsmEvtCollect(LogServiceUInt eventType, struct MsgCB *msg);
/* NRMMЭ���¼��ռ������� */
LogServiceUInt NO_OPTIMIZE_WEAK LOG_SERVICE_NrmmEvtCollect(LogServiceUInt eventType, struct MsgCB *msg);
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __MBB_LOG_SERVICE_API_H__ */
