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
 * �ļ�˵��:CTTFʹ�õĽṹͳһ���������ļ��С�
 */

#ifndef CTTF_OAM_INTERFACE_H
#define CTTF_OAM_INTERFACE_H

#include "vos.h"
#include "ttf_link_interface.h"
#include "PsTypeDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)
/* ���͵�RLPʵ��ĸ��� */
#define CTTF_HRPD_MNTN_DEFAULT_RLP_MAX_NUM 2

/* MFPA RLPʵ��������� */
#define CTTF_HRPD_MNTN_MF_RLP_MAX_NUM 6

/* EMPA RLPʵ��������� */
#define CTTF_HRPD_MNTN_EMF_RLP_MAX_NUM 6

/*
 * ����CDMA 5.12s����, ����ÿ1.28s�ϱ�һ�Ρ�1.28s=64*20ms, 64��Ϊ�����±�,
 * ��һ���ϱ�64֡�������, �ڴ���Խ���
 *  �ú��С�仯ʱ������ͬ������CTTF_1X_MAC_MNTN_BUF_MAX_SIZE
 */
#define CTTF_1X_MAC_MNTN_MAX_REPORT_NUM 64

/* ��ΪHIDS��֧�ֶ�̬��ӡ�ɾ��RLPʵ��, �������ۺ��������Э�����ֵ����ú��С�仯ʱ������ͬ������CTTF_1X_RLP_MNTN_BUF_MAX_SIZE */
#define CTTF_1X_RLP_MNTN_MAX_NUM 6

/* �ϱ���MAC�����������ο������� */
#define CTTF_HRPD_RTC3MAC_MNTN_FLOW_MAX_NUM 8

/*
 * ����Э��16slotΪһ��MAC Layer Packet, ÿһ����֡��¼һ�Ρ�һ����֡Ϊ4��slot,
 * ��16��slot�����16/4=4������һ��slotΪ1.28ms����ô16��slot��Ӧ26.66ms��
 * ��Ϊ640ms�ϱ�һ����Ϣ����ô��Ϣ��һ��MAC���������24�����ݣ�640=48*26.66ms����
 * һ�����ݼ�¼4����֡����Ϣ����1.28s��ÿ�����ϱ�24*4= 96�顣
 */
#define CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_PACKET_PER_FLOW 96
#define CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_T2P_PER_FLOW 96
#define CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_HARQ 96

/* It's define the num of  subframe information instances */
#define CTTF_HRPD_MNTN_NUM_SUBFRAME_INFO_INSTANCES 3

/* rtc3֧�ֵ����֯(����)�� */
#define CTTF_HRPD_MNTN_RTC3MAC_INTERLACE_MAX_NUM 3

enum CTTF_MNTN_MSG_TYPE_ENUM {
    /* 1X�Ŀ�ά�ɲ���Ϣ�ı����D4��ͷ */
    ID_OM_CTTF_1X_MNTN_CONFIG_REQ                    = 0xD400, /* 1X ���ÿ�������, �����·����� */
    ID_CTTF_OM_1X_MNTN_CONFIG_CNF                    = 0xD400, /* 1X ���ÿ��ػظ�, ���ڻظ����ý�� */
    ID_CTTF_OM_1X_MNTN_FWD_LINK_IND                  = 0xD402, /* 1X Forward Link Info�ϱ� */
    ID_CTTF_OM_1X_MNTN_REV_LINK_IND                  = 0xD403, /* 1X Reverse Link Info�ϱ� */
    ID_CTTF_OM_1X_MNTN_RLP_STATISTICS_IND            = 0xD404, /* 1X RLP statistics�ϱ� */
    ID_CTTF_OM_1X_MNTN_COMMON_CHANNEL_STATISTICS_IND = 0xD405, /* Common Channel statistics�ϱ� */
    ID_OM_CTTF_1X_MNTN_TRACE_CONFIG_REQ              = 0xD406, /* ����1x��trace���ÿ������� */

    /* EVDO�Ŀ�ά�ɲ���Ϣ(��HRPD��eHRPD)�ı����D6��ͷ */
    ID_OM_CTTF_HRPD_MNTN_CONFIG_REQ                  = 0xD600, /* HRPD ���ÿ�������, �����·����� */
    ID_CTTF_OM_HRPD_MNTN_CONFIG_CNF                  = 0xD601, /* HRPD ���ÿ��ػظ�, ���ڻظ����ý�� */
    ID_CTTF_OM_HRPD_MNTN_MODULE_PROTOCOL_STATE_IND   = 0xD602, /* HRPD Module Protocol State�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_MAC_PROTOCOL_STATISTICS_IND = 0xD603, /* HRPD MAC Protocol Statistics�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_RLP_STATISTICS_IND          = 0xD604, /* HRPD RLP statistics�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_MFPA_RLP_STATISTICS_IND     = 0xD605, /* MFPA RLP statistics�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_EMPA_RLP_STATISTICS_IND     = 0xD606, /* EMPA RLP statistics�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_REV_LINK_PACKET_INFO_IND    = 0xD607, /* HRPD Reverse Link Packet Info�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_REV_LINK_T2P_STATISTICS_IND = 0xD608, /* HRPD Reverse Link T2P statistics�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_HARQ_INFO_IND               = 0xD609, /* HRPD Harq Info�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_RTC3MAC_ACK_INFO_IND        = 0xD60A, /* HRPD RTC3MAC Info�ϱ� */
    ID_CTTF_OM_HRPD_MNTN_TRACE_MSG_CONFIG_REQ        = 0xD60B, /* HRPD Trace Msg ������Ϣ */

    CTTF_MNTN_MSG_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT16 CTTF_MNTN_MSG_TYPE_ENUM_UINT16;

enum CTTF_MNTN_RPT_CMD_ENUM {
    CTTF_MNTN_RPT_CMD_STOP     = 0x00, /*ֹͣ�ϱ�*/
    CTTF_MNTN_RPT_CMD_START    = 0x01  /*��ʼ�ϱ�*/
};

typedef VOS_UINT8 CTTF_MNTN_RPT_CMD_ENUM_UINT8;

enum CTTF_MNTN_RLP_RPT_CMD_ENUM {
    CTTF_MNTN_RLP_RPT_CMD_STOP   = 0x00, /*ֹͣ�ϱ�*/
    CTTF_MNTN_RLP_RPT_CMD_START  = 0x01, /*��ʼ�ϱ�*/
    CTTF_MNTN_RLP_RPT_CMD_CLEAR  = 0x02  /*���*/
};
typedef VOS_UINT8 CTTF_MNTN_RLP_RPT_CMD_ENUM_UINT8;

/* hrpd�ϱ���� */
enum CTTF_HRPD_MNTN_RESULT_TYPE_ENUM {
    CTTF_HRPD_MNTN_RESULT_OK               = 0,
    CTTF_HRPD_MNTN_RESULT_START_TIMER_FAIL = 1,
    CTTF_HRPD_MNTN_RESULT_ALLOC_MEM_FAIL   = 2,
    CTTF_HRPD_MNTN_RESULT_ALLOC_MSG_FAIL   = 3,
    CTTF_HRPD_MNTN_RESULT_INPUT_PARAM_ERR  = 4,
    CTTF_HRPD_MNTN_RESULT_LOCAL_PARAM_ERR  = 5
};
typedef VOS_UINT32 CTTF_HRPD_MNTN_RESULT_TYPE_ENUM_UINT32;

enum CTTF_MNTN_COMM_RPT_CMD_ENUM {
    CTTF_MNTN_COMM_RPT_CMD_STOP  = 0x00, /*ֹͣ�ϱ�*/
    CTTF_MNTN_COMM_RPT_CMD_START = 0x01, /*��ʼ�ϱ�*/
    CTTF_MNTN_COMM_RPT_CMD_CLEAR = 0x02  /*���*/
};
typedef VOS_UINT8 CTTF_MNTN_COMM_RPT_CMD_ENUM_UINT8;

/* HIDS����, Ҫ��ÿ��ö��ֵ��������ʾ������ֵ, �����ܽ��� */
enum CTTF_1X_MAC_MNTN_RATE_ENUM {
    CTTF_1X_MAC_MNTN_RATE_BLANK   = 0,
    CTTF_1X_MAC_MNTN_RATE_EIGHTH  = 1,
    CTTF_1X_MAC_MNTN_RATE_QUARTER = 2,
    CTTF_1X_MAC_MNTN_RATE_HALF    = 3,
    CTTF_1X_MAC_MNTN_RATE_FULL    = 4,
    CTTF_1X_MAC_MNTN_RATE_2X      = 5,
    CTTF_1X_MAC_MNTN_RATE_4X      = 6,
    CTTF_1X_MAC_MNTN_RATE_8X      = 7,
    CTTF_1X_MAC_MNTN_RATE_16X     = 8,
    CTTF_1X_MAC_MNTN_RATE_32X     = 9,
    CTTF_1X_MAC_MNTN_RATE_BUTT    = 10
};
typedef VOS_UINT8 CTTF_1X_MAC_MNTN_RATE_ENUM_UINT8;

enum CTTF_1X_MAC_MNTN_RESULT_TYPE_ENUM {
    CTTF_1X_MAC_MNTN_RESULT_OK               = 0,
    CTTF_1X_MAC_MNTN_RESULT_START_TIMER_FAIL = 1,
    CTTF_1X_MAC_MNTN_RESULT_ALLOC_MEM_FAIL   = 2,
    CTTF_1X_MAC_MNTN_RESULT_ALLOC_MSG_FAIL   = 3,
    CTTF_1X_MAC_MNTN_RESULT_INPUT_PARAM_ERR  = 4,
    CTTF_1X_MAC_MNTN_RESULT_LOCAL_PARAM_ERR  = 5
};
typedef VOS_UINT32 CTTF_1X_MAC_MNTN_RESULT_TYPE_ENUM_UINT32;

enum CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM {
    CTTF_1X_MAC_MNTN_RPT_PERIOD_1280 = 1280, /* �ϱ�����1280ms */
};
typedef VOS_UINT32 CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32;

/* hrpd�ϱ����� */
enum CTTF_HRPD_MNTN_RPT_PERIOD_ENUM {
    CTTF_HRPD_MNTN_RPT_PERIOD_1280 = 1280 /* �ϱ�����1280ms */
};

typedef VOS_UINT32 CTTF_HRPD_MNTN_RPT_PERIOD_ENUM_UINT32;

/* hrpd RTC ��ͼ�ϱ����� */
enum CTTF_HRPD_MNTN_RTC_RPT_PERIOD_ENUM {
    CTTF_HRPD_MNTN_RTC_RPT_PERIOD_640 = 640 /* �ϱ�����640ms */
};
typedef VOS_UINT32 CTTF_HRPD_MNTN_RTC_RPT_PERIOD_ENUM_UINT32;

/* HRPD��Э������ */
enum CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM {
    CTTF_HRPD_MNTN_DEFAULT_CC_MAC_ENUM                        = 0x00,
    CTTF_HRPD_MNTN_ENHANCE_CC_MAC_ENUM                        = 0x01,
    CTTF_HRPD_MNTN_DEFAULT_AC_MAC_ENUM                        = 0x02,
    CTTF_HRPD_MNTN_ENHANCE_AC_MAC_ENUM                        = 0x03,
    CTTF_HRPD_MNTN_DEFAULT_FTC_MAC_ENUM                       = 0x04,
    CTTF_HRPD_MNTN_ENHANCE_FTC_MAC_ENUM                       = 0x05,
    CTTF_HRPD_MNTN_DEFAULT_RTC_MAC_ENUM                       = 0x06,
    CTTF_HRPD_MNTN_SUBTYPE3_RTC_MAC_ENUM                      = 0x07,
    CTTF_HRPD_MNTN_DEFAULT_AUTHENTICATION_ENUM                = 0x08,
    CTTF_HRPD_MNTN_SHA1_AUTHENTICATION_ENUM                   = 0x09,
    CTTF_HRPD_MNTN_DEFAULT_ENCRYPTION_ENUM                    = 0x0a,
    CTTF_HRPD_MNTN_AES_ENCRYPTION_ENUM                        = 0x0b,
    CTTF_HRPD_MNTN_DEFAULT_SECURITY_ENUM                      = 0x0c,
    CTTF_HRPD_MNTN_GENERIC_SECURITY_ENUM                      = 0x0d,
    CTTF_HRPD_MNTN_DEFAULT_STREAM_ENUM                        = 0x0e,
    CTTF_HRPD_MNTN_DEFAULT_SIGNALING_APP_ENUM                 = 0x0f,
    CTTF_HRPD_MNTN_DEFAULT_PACKET_APP_RN_ENUM                 = 0x10,
    CTTF_HRPD_MNTN_DEFAULT_PACKET_APP_SN_ENUM                 = 0x11,
    CTTF_HRPD_MNTN_TEST_APP_ENUM                              = 0x12,
    CTTF_HRPD_MNTN_MULTI_FLOW_PACKET_APP_RN_ENUM              = 0x13,
    CTTF_HRPD_MNTN_MULTI_FLOW_PACKET_APP_SN_ENUM              = 0x14,
    CTTF_HRPD_MNTN_3G1X_CIRCUIT_SERVICE_NOTIFICATION_APP_ENUM = 0x15,
    CTTF_HRPD_MNTN_RESERVED_APP_ENUM                          = 0x16,
    CTTF_HRPD_MNTN_ENHANCED_MULTI_FLOW_PACKET_APP_RN_ENUM     = 0x17,
    CTTF_HRPD_MNTN_ENHANCED_MULTI_FLOW_PACKET_APP_SN_ENUM     = 0x18,
    CTTF_HRPD_MNTN_ENHANCED_TEST_APP_ENUM                     = 0x19,
    CTTF_HRPD_MNTN_STREAM_NOT_USED_ENUM                       = 0x1a,
    CTTF_HRPD_MNTN_NO_SUB_PROTOCOL_ENUM                       = 0x1b,
    CTTF_HRPD_MNTN_SUB_PROTOCOL_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8;

/* ����ģʽ */
enum CTTF_HRPD_MAC_TRANS_MODE_ENUM {
    CTTF_HRPD_MAC_TRANS_MODE_HIGH_CAPACITY = 0x00,
    CTTF_HRPD_MAC_TRANS_MODE_LOW_LATENCY   = 0x01,
    CTTF_HRPD_MAC_TRANS_MODE_BUTT          = 0x02
};
typedef VOS_UINT8 CTTF_HRPD_MAC_TRANS_MODE_ENUM_UINT8;

/* It's used to define subpacketID, it means how many times the packet is sent. */
enum CTTF_HrpdMntnSubPacketType {
    CSDR_HARQ_MNTN_SUBPACKET_FIRST_TIME  = 0x0000,
    CSDR_HARQ_MNTN_SUBPACKET_SECOND_TIME = 0x0001,
    CSDR_HARQ_MNTN_SUBPACKET_THIRD_TIME  = 0x0002,
    CSDR_HARQ_MNTN_SUBPACKET_FOURTH_TIME = 0x0003,
    CSDR_HARQ_MNTN_SUBPACKET_BUTT        = 0x0004
};
typedef VOS_UINT16 CTTF_HrpdMntnSubPacketTypeUint8;

/* T2P����ģʽ */
enum CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_ENUM {
    CTTF_HRPD_MNTN_RTC3MAC_MODE_NORMAL                = 0x00, /* ͨ��ģʽ */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_PHY_FORBIDDEN         = 0x01, /* PHY��ֹ���� */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_SUPERVISION_FAILED    = 0x02, /* supervision failed֮��������ʧȥ��ϵ */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_SILENCE_DURATION      = 0x03, /* ��Ĭ */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_TEST_LOOP             = 0x04, /* ����ģʽ */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_PREEMPT               = 0x05, /* ����ռ */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_RTC_SETUP_STATUS      = 0x06, /* RTC3����setup״̬ */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_ATTR_PARA_CHANGE_NEGO = 0x07, /* ��Э�̸ı�Э�̲��� */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_ATTR_PARA_CHANGE_GAUP = 0x08, /* GAUP�ı�Э�̲��� */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_TCH_PARA_CHANGE       = 0x09, /* TCH�����ı� */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_OTHER                 = 0x0A, /* ���� */
    CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_BUTT             = 0x0B
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_ENUM_UINT8;

/* DCC��ECC��״̬,���չ��ߵ�Ҫ��, DCC��ECC��״̬Ҫ������һ�� */
enum CTTF_HRPD_MNTN_CC_STATE_ENUM {
    CTTF_HRPD_MNTN_CC_INACTIVE_STATE = 0x0,
    CTTF_HRPD_MNTN_CC_ACTIVE_STATE   = 0x1,
    CTTF_HRPD_MNTN_CC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_CC_STATE_ENUM_UINT8;

/* SDAC��EAC��״̬,���չ��ߵ�Ҫ��, DAC��EAC��״̬Ҫ������һ�� */
enum CTTF_HRPD_MNTN_AC_STATE_ENUM {
    CTTF_HRPD_MNTN_AC_INACTIVE_STATE = 0x0,
    CTTF_HRPD_MNTN_AC_ACTIVE_STATE   = 0x1,
    CTTF_HRPD_MNTN_AC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_AC_STATE_ENUM_UINT8;

/* Description: DFTC��EFTC��״̬,���չ��ߵ�Ҫ��, DFTC��EFTC��״̬Ҫ������һ�� */
enum CTTF_HRPD_MNTN_FTC_STATE_ENUM {
    CTTF_HRPD_MNTN_FTC_INACTIVE_STATE      = 0x0,
    CTTF_HRPD_MNTN_FTC_VARIABLE_RATE_STATE = 0x1,
    CTTF_HRPD_MNTN_FTC_FIXED_RATE_STATE    = 0x2,
    CTTF_HRPD_MNTN_FTC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_FTC_STATE_ENUM_UINT8;

/* Description: DRTC��ST3 RTC��״̬,���չ��ߵ�Ҫ��, DRTC��ST3 RTC��״̬Ҫ������һ�� */
enum CTTF_HRPD_MNTN_RTC_STATE_ENUM {
    CTTF_HRPD_MNTN_RTC_INACTIVE_STATE  = 0x0,
    CTTF_HRPD_MNTN_RTC_SETUP_STATE     = 0x1,
    CTTF_HRPD_MNTN_RTC_OPEN_STATE      = 0x2,
    CTTF_HRPD_MNTN_RTC_SUSPENDED_STATE = 0x3,
    CTTF_HRPD_MNTN_RTC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_RTC_STATE_ENUM_UINT8;

/* Description: ��ȫ��Э��״̬ */
enum CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM {
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_NO_STATE = 0x0,
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8;

/* Description: ���ز�Э��״̬ */
enum CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_ENUM {
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_CLOSE_STATE = 0x0,
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_OPEN_STATE  = 0x1,
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_ENUM_UINT8;

/*
 * Description: ��Ӧ�ò�Э��״̬
 *              ����Ҫ��Default Packet Application, Multi-flow Packet Application,
 *              Enhanced Multi-flow Packet Application, Data Over Signalling,
 *              Test Application, �Լ�Enhanced Test Application������һ��
 */
enum CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_ENUM {
    CTTF_HRPD_MNTN_STREAM_APP_RESERVATION_CLOSE_STATE = 0x0,
    CTTF_HRPD_MNTN_STREAM_APP_RESERVATION_STATE       = 0x1,
    CTTF_HRPD_MNTN_STREAM_APP_NO_STATE                = 0x2,
    CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_ENUM_UINT8;

/* Description: SNPЭ��״̬ */
enum CTTF_HRPD_MNTN_SNP_UP_TX_STATE_ENUM {
    CTTF_HRPD_MNTN_SNP_UP_TX_ALLOW_STATE     = 0x0,
    CTTF_HRPD_MNTN_SNP_UP_TX_NOT_ALLOW_STATE = 0x1,
    CTTF_HRPD_MNTN_SNP_UP_TX_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_SNP_UP_TX_STATE_ENUM_UINT8;

/* Description: Default RTC ��������ö�� */
enum CTTF_HRPD_RTCMAC_MNTN_RATE_ENUM {
    CTTF_HRPD_RTCMAC_MNTN_RATE_NULL          = 0,
    CTTF_HRPD_RTCMAC_MNTN_RATE_9K6           = 1,
    CTTF_HRPD_RTCMAC_MNTN_RATE_19K2          = 2,
    CTTF_HRPD_RTCMAC_MNTN_RATE_38K4          = 3,
    CTTF_HRPD_RTCMAC_MNTN_RATE_76K8          = 4,
    CTTF_HRPD_RTCMAC_MNTN_RATE_153K6         = 5,
    CTTF_HRPD_RTCMAC_MNTN_RATE_NO_LIMITATION = 6, /* ��ʾ��Rate������ */
    CTTF_HRPD_RTCMAC_MNTN_RATE_BUTT          = 7  /* ��ʾ��Ч */
};
typedef VOS_UINT8 CTTF_HRPD_RTCMAC_MNTN_RATE_ENUM_UINT8;


/*
 * Description: Flow Protocol Identifier
 *              C.R1001-H_v1.0_Administration of Parameter Value Assignments for cdma2000
 *              Spread Spectrum Standards Table 11.3-1
 */
enum CTTF_HRPD_PA_MNTN_FLOW_PROTOCOL_ENUM {
    CTTF_HRPD_PA_MNTN_FLOW_PROTOCOL_NULL                         = 0x00,
    CTTF_HRPD_PA_MNTN_HDLC_LIKE_FRMAING_WITH_PPP                 = 0x01,
    CTTF_HRPD_PA_MNTN_IPV4                                       = 0x02,
    CTTF_HRPD_PA_MNTN_IPV6                                       = 0x03,
    CTTF_HRPD_PA_MNTN_ROHC                                       = 0x04,
    CTTF_HRPD_PA_MNTN_IPV4_AND_IPV6                              = 0x05,
    CTTF_HRPD_PA_MNTN_HDLC_LIKE_FRMAING_WITH_ALTPPP              = 0x06,
    CTTF_HRPD_PA_MNTN_HDLC_LIKE_FRMAING_SUPPORTING_MULTIPLE_PDNS = 0x07,
    CTTF_HRPD_PA_MNTN_PDN_MUX_FOR_IP_PDNS                        = 0x08,
    CTTF_HRPD_PA_MNTN_BCMCS_OVER_UNICAST_WITH_HDLC_LIKE_FRAMING  = 0x09,
    CTTF_HRPD_PA_MNTN_BCMCS_OVER_UNICAST_WITH_SEG_BASED_FRAMING  = 0x0A,
    CTTF_HRPD_PA_MNTN_FLOW_PROTOCOL_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_PA_MNTN_FLOW_PROTOCOL_ENUM_UINT8;

/*
 * Description: Route Protocol Identifier
 *              C.R1001-H_v1.0_Administration of Parameter Value Assignments for cdma2000
 *              Spread Spectrum Standards Table 11.3-1
 */
enum CTTF_HRPD_PA_MNTN_ROUTE_PROTOCOL_ENUM {
    CTTF_HRPD_PA_MNTN_ROUTE_PROTOCOL_NULL = 0x00,
    CTTF_HRPD_PA_MNTN_ROUTE_PROTOCOL_ROHC = 0x04,
    CTTF_HRPD_PA_MNTN_ROUTE_PROTOCOL_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_PA_MNTN_ROUTE_PROTOCOL_ENUM_UINT8;

/* ö��˵��: �����֧�ֵ�PacketSize���� */
enum CTTF_HRPD_MNTN_RTC3MAC_PAKETSIZE_INDEX_ENUM {
    CTTF_HRPD_MNTN_RTC3MAC_PS_0     = 0x00, /* PS��СΪ0bit<-->0Byte */
    CTTF_HRPD_MNTN_RTC3MAC_PS_128   = 0x01, /* PS��СΪ128bit<-->16BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_256   = 0x02, /* PS��СΪ256bit<-->32BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_512   = 0x03, /* PS��СΪ512bit<-->64BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_768   = 0x04, /* PS��СΪ768bit<-->96BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_1024  = 0x05, /* PS��СΪ1024bit<-->128BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_1536  = 0x06, /* PS��СΪ1536bit<-->192BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_2048  = 0x07, /* PS��СΪ2048bit<-->256BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_3072  = 0x08, /* PS��СΪ3072bit<-->384BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_4096  = 0x09, /* PS��СΪ4096bit<-->512BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_6144  = 0x0A, /* PS��СΪ6144bit<-->768BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_8192  = 0x0B, /* PS��СΪ8192bit<-->1024BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_12288 = 0x0C, /* PS��СΪ12288bit<-->1536BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_BUTT  = 0x0D
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_RTC3MAC_PAKETSIZE_INDEX_ENUM_UINT8;


typedef struct {
    VOS_MSG_HEADER

    VOS_UINT32  ulMsgId;

    VOS_UINT16  usOriginalId;
    VOS_UINT16  usTerminalId;
    VOS_UINT32  ulTimeStamp;
    VOS_UINT32  ulSN;
} DIAG_TRANS_MSG_HDR_STRU;

/* 1X�Ŀ�ά�ɲ���Ϣ */
/*
 * �ṹ��: OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU
 * �ṹ˵��: 1X��ͨ����������ṹ
 */
typedef struct {
    CTTF_MNTN_RPT_CMD_ENUM_UINT8             enCommand; /* ��ʼ��ֹͣ�ϱ� */
    VOS_UINT8                               aucReserve[3];
    CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* �ϱ�����, ��λ: ms */
} OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU;

/* �ṹ˵��: 1X��RLP��������ṹ */
typedef struct {
    CTTF_MNTN_RLP_RPT_CMD_ENUM_UINT8         enCommand; /* ��ʼ��ֹͣ�ϱ������ */
    VOS_UINT8                               aucReserve[3];
    CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* �ϱ�����, ��λ: ms */
} OM_CTTF_1X_MNTN_RLP_GENERIC_CONFIG_STRU;

/* �ṹ˵��: 1X��ͨ����������ṹ */
typedef struct {
    CTTF_MNTN_COMM_RPT_CMD_ENUM_UINT8        enCommand; /* ��ʼ��ֹͣ�ϱ������ */
    VOS_UINT8                               aucReserve[3];
    CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* �ϱ�����, ��λ: ms */
} OM_CTTF_1X_MNTN_COMM_GENERIC_CONFIG_STRU;

/* �ṹ˵��: HRPDͨ����������ṹ */
typedef struct {
    CTTF_MNTN_COMM_RPT_CMD_ENUM_UINT8      enCommand; /* ��ʼ��ֹͣ�ϱ������ */
    VOS_UINT8                             aucReserve[3];
    CTTF_HRPD_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* �ϱ�����, ��λ: ms */
} OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU;

/* �ṹ˵��: HRPDͨ����������ṹ */
typedef struct {
    CTTF_MNTN_RPT_CMD_ENUM_UINT8               enCommand; /* ��ʼ��ֹͣ�ϱ� */
    VOS_UINT8                                 aucReserve[3];
    CTTF_HRPD_MNTN_RTC_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* �ϱ�����, ��λ: ms */
} OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU;

/* ��Ӧ��Ϣ:  ID_OM_CTTF_1X_MNTN_CONFIG_REQ */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stTransHdr;

    /* ����CDMA 5.12s����, �ο�QXDM, ����ÿ1.28s�ϱ�һ�� */
    OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU      stFwdLinkInfo;      /* 1X Forward Link Info, �ϱ�����: 1280ms */
    OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU      stRevLinkInfo;      /* 1X Reverse Link Info, �ϱ�����: 1280ms */
    OM_CTTF_1X_MNTN_RLP_GENERIC_CONFIG_STRU  stRlpStatistics;    /* 1X RLP statistics, �ϱ�����: 1280ms */
    OM_CTTF_1X_MNTN_COMM_GENERIC_CONFIG_STRU stCommonStatistics; /* Common Channel statistics, �ϱ�����: 1280ms */
} OM_CTTF_1X_MNTN_CONFIG_REQ_STRU;

/* ��Ӧ��Ϣ:  ID_OM_CTTF_HRPD_MNTN_CONFIG_REQ */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stTransHdr;

    /* ����CDMA 5.12s����, ����ÿ1.28s�ϱ�һ�� */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stMacStatistics;  /* HRPD MAC Statistics, �ϱ�����: 1280ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stRlpStatistics;  /* HRPD RLP Statistics, �ϱ�����: 1280ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stMfpaStatistics; /* MFPA RLP Statistics, �ϱ�����: 1280ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stEmpaStatistics; /* EMPA RLP Statistics, �ϱ�����: 1280ms */
    OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU    stRtc3PktInfo;    /* RTC3 Packet Info, �ϱ�����: 640ms */
    OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU    stT2pStatistics;  /* T2P Statistics, �ϱ�����: 640ms */
    OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU    stRtc3HarqInfo;   /* RTC3 Harq Info, �ϱ�����: 640ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stRtc3AckInfo;    /* RTC3 ACK Info, �ϱ�����: 1280ms */
} OM_CTTF_HRPD_MNTN_CONFIG_REQ_STRU;

/* ��Ӧ��Ϣ:  ID_CTTF_OM_HRPD_MNTN_CONFIG_CNF */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stDiagHdr;

    PS_RSLT_CODE_ENUM_UINT32 enRslt;   /* ���óɹ���ʧ�� */
    VOS_UINT32               ulReason; /* ������ʧ�ܵ�ԭ�� */
} CTTF_OM_HRPD_MNTN_CONFIG_CNF_STRU;

/*
 * ��Ӧ��Ϣ:  ID_CTTF_OM_1X_MNTN_CONFIG_CNF
 * ˵��: ע��OM�ںϺ�, HIDS��Xģ�����ÿ�����Ϣ����, ��ͬ��GUģ
 *       �ϱ���CNF��Ϣ
 */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stDiagHdr;

    PS_RSLT_CODE_ENUM_UINT32 enRslt;   /* ���óɹ���ʧ�� */
    VOS_UINT32               ulReason; /* ������ʧ�ܵ�ԭ�� */
} CTTF_OM_1X_MNTN_CONFIG_CNF_STRU;

/*
 * �ṹ˵��: ĳForward�ŵ���ͳ��ֵ
 * �ýṹ���С�仯ʱ������ͬ������CTTF_1X_MAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    CTTF_1X_MAC_MNTN_RATE_ENUM_UINT8 enRate;         /* ��ЧֵΪBUTT */
    VOS_UINT8                        ucPercentage;   /* �ɹ������Ŀ�ٷֱȷ���, [0..100], ��Чֵ0xFF */
    VOS_UINT8                        aucReserved[2]; /* �����ֽ�, ������Чֵ��д, ֵΪ0 */
} CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU;

/*
 * ��Ӧ��Ϣ:  ID_CTTF_OM_1X_MNTN_FWD_LINK_IND
 * ˵��:  �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 * �ṹ˵��: ��Forward�ŵ���ͳ��ֵ
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU astFch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];  /* FCHͳ��ֵ */
    CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU astDcch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM]; /* DCCHͳ��ֵ */
    CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU astSch0[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM]; /* SCHͳ��ֵ */
} CTTF_OM_1X_MNTN_FWD_LINK_IND_STRU;

/* CTTF_OM_1X_MNTN_xxx_IND_STRU�ϱ���Ϣ��ͷ�ĳ��ȣ�sizeof(usPrimId)+sizeof(usToolId) */
#define CTTF_OM_1X_MNTN_IND_HEAD_BYTE_SIZE 4

/* CTTF_OM_HRPD_MNTN_xxx_IND_STRU�ϱ���Ϣ��ͷ�ĳ��ȣ�sizeof(usPrimId)+sizeof(usToolId) */
#define CTTF_OM_HRPD_MNTN_IND_HEAD_BYTE_SIZE 4

/*
 * �ṹ˵��: ĳReverse�ŵ���ͳ��ֵ
 * �ýṹ���С�仯ʱ������ͬ������CTTF_1X_MAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    CTTF_1X_MAC_MNTN_RATE_ENUM_UINT8 enRate;         /* ��ЧֵΪBUTT */
    VOS_UINT8                        aucReserved[3]; /* �����ֽ�, ������Чֵ��д, ֵΪ0 */
} CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU;

/*
 * ��Ӧ��Ϣ:  ID_CTTF_OM_1X_MNTN_REV_LINK_IND
 * ˵��:  �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 * �ṹ˵��: ��Reverse�ŵ���ͳ��ֵ
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU astFch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];
    CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU astDcch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];
    CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU astSch0[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];
} CTTF_OM_1X_MNTN_REV_LINK_IND_STRU;

/* �ṹ˵��: ĳRLPʵ���REV����ͳ��ֵ */
typedef struct {
    VOS_UINT32 ulFrameTotalByteHigh; /* ���͵�֡�����ֽ�������32bit��, 2015�깤�߲�֧��64bit����, �ʷֲ��ϱ�����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulFrameTotalByte;     /* ���͵�֡�����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByteHigh; /* �ϲ�������͵����ֽڣ���32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByte;     /* �ϲ�������͵����ֽڣ���32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalFrameCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulIdleFrameCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulFchNewDataFrameCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulFchRetxFrameCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulSchNewDataFrameCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulSchRetxFrameCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakSingle;          /* �ش�һ�κ���ճɹ��Ĵ���, ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakDouble;          /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakTriple;          /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbove;           /* �ش�����3�εĴ���, ��Чֵ: 0xFFFFFFFF */
} CTTF_1X_RLP_MNTN_REV_STATISTICS_PER_ENTITY_STRU;

/* �ṹ˵��: ĳRLPʵ���FWD����ͳ��ֵ */
typedef struct {
    VOS_UINT32 ulFrameTotalByteHigh; /* ���յ�֡�����ֽ�������32bit��, 2015�깤�߲�֧��64bit���͡���Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulFrameTotalByte;     /* ���յ�֡�����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByteHigh; /* �ϲ�������յ����ֽڣ���32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByte;     /* �ϲ�����ܽ��յ����ֽڣ���32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalFrameCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulIdleFrameCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulFillFrameCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulBlankFrameCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNullFrameCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulFchNewDataFrameCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulFchRetxFrameCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulSchNewDataFrameCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulSchRetxFrameCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakCtrlFrameCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortTimes;      /* NAK abort����, ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulInsufficentQualityFrameCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidFcsFrameCnt;         /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidTypeFieldFrameCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidCtlFieldFrameCnt;    /* ��Чֵ: 0xFFFFFFFF */
} CTTF_1X_RLP_MNTN_FWD_STATISTICS_PER_ENTITY_STRU;

/*
 * �ṹ˵��: ĳRLPʵ���ͳ��ֵ
 * �ýṹ���С�仯ʱ������ͬ������CTTF_1X_RLP_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT8  ucSrId;                  /* ����RLPʵ���Ψһ���, ��Чֵ: 0xFF */
    VOS_UINT8  ucMeasuredRtt;           /* ���һ�β�����RTT, ��Чֵ: 0xFF */
    VOS_UINT8  aucReserved[2];          /* �����ֽ�, ������Чֵ��д, ֵΪ0 */
    VOS_UINT16 usBSTriggeredResetTimes; /* AT�յ�AN�ĸ�λRLP�������λRLP�Ĵ�������Чֵ: 0xFFFF */
    VOS_UINT16 usMSTriggeredResetTimes; /* AT������λRLP�Ĵ�������Чֵ: 0xFFFF */
    VOS_UINT32 ulCurrentBo;             /* ʵʱBO, ��Чֵ: 0xFFFFFFFF */
    CTTF_1X_RLP_MNTN_REV_STATISTICS_PER_ENTITY_STRU stRevInfo;
    CTTF_1X_RLP_MNTN_FWD_STATISTICS_PER_ENTITY_STRU stFwdInfo;
} CTTF_1X_RLP_MNTN_STATISTICS_PER_ENTITY_STRU;

/*
 * ��Ӧ��Ϣ:  ID_CTTF_OM_1X_MNTN_RLP_STATISTICS_IND
 * �ṹ˵��: ��RLPʵ���ͳ��ֵ
 * ˵��:  �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_RLP_MNTN_STATISTICS_PER_ENTITY_STRU astRlpEntity[CTTF_1X_RLP_MNTN_MAX_NUM];
} CTTF_OM_1X_MNTN_RLP_STATISTICS_IND_STRU;

/*
 * �ṹ˵��: PCH��ͳ��ֵ
 * �ýṹ���С�仯ʱ������ͬ������CTTF_1X_LAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT32 ulTotalEncapPduFragCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ul96BitEncapPduFragCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ul192BitEncapPduFragCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalEncapPduCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidCrcEncapPduCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulChanLostCnt;           /* ��Чֵ: 0xFFFFFFFF */
} CTTF_1X_MNTN_PCH_STATISTIC_STRU;

/*
 * �ṹ˵��: ACH��ͳ��ֵ
 * �ýṹ���С�仯ʱ������ͬ������CTTF_1X_LAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT32 ulRequestTypeSduCnt;           /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulResponseTypeSduCnt;          /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe1Cnt;             /* ����1�γɹ��Ĵ���, ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe2Cnt;             /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe3Cnt;             /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe4Cnt;             /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe5Cnt;             /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbeAboveCnt;         /* ����5�����ϲųɹ��Ĵ���, ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulUnsucessfulAccessAttemptCnt; /* ����attemptʧ�ܴ���, ��Чֵ: 0xFFFFFFFF */
} CTTF_1X_MNTN_ACH_STATISTIC_STRU;

/*
 * �ṹ˵��: PAGE��ͳ��ֵ
 * �ýṹ���С�仯ʱ������ͬ������CTTF_1X_LAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT32 ulGpmMatchedCnt;          /* ƥ���ϵ�GPM����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulSlotPageMatchedCnt;     /* Slotģʽƥ���Page����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulMismatchedPageCnt;      /* ���еĲ�ƥ���Page����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAddrMatchedOtherMsgCnt; /* ����Page��Ϣ�⣬���еĵ�ַƥ�����Ϣ������Чֵ: 0xFFFFFFFF */
} CTTF_1X_MNTN_PAGE_MSG_STATISTIC_STRU;

/*
 * ��Ӧ��Ϣ:  ID_CTTF_OM_1X_MNTN_COMMON_CHANNEL_STATISTICS_IND
 * ˵��:  �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 * �ṹ˵��: 1X�����ŵ���ͳ��ֵ
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_MNTN_PCH_STATISTIC_STRU      stPch;
    CTTF_1X_MNTN_ACH_STATISTIC_STRU      stAch;
    CTTF_1X_MNTN_PAGE_MSG_STATISTIC_STRU stPage;

} CTTF_OM_1X_MNTN_COMMON_CHANNEL_STATISTICS_IND_STRU;

/* �ṹ˵��: HRPD RTC3MAC ÿ��MAC Flow�İ�ͳ�� */
typedef struct {
    VOS_UINT8  ucFlowId;            /* MAC��ID����Чֵ: 0xFF */
    VOS_UINT8  aucReserved[1];      /* �����ֽ�, ������Чֵ��д, ֵΪ0xFF */
    VOS_UINT16 usActualAllocation;  /* ��λ: �ֽڣ���Чֵ: 0xFFFF */
    VOS_UINT16 usSuggestAllocation; /* ��λ: �ֽڣ���Чֵ: 0xFFFF */
    VOS_UINT16 usRequiredQueueSize; /* ��λ: �ֽڣ���Чֵ: 0xFFFF */
    VOS_UINT16 usQOutflow;          /* ��λ: �ֽڣ���Чֵ: 0xFFFF */
    VOS_UINT8  aucReserved1[2];     /* �����ֽ�, ������Чֵ��д, ֵΪ0xFF */
} CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_PER_MAC_FLOW_STRU;

/*
 * �ṹ˵��: HRPD RTC3MAC ����Ϣͳ��,PacketId����Ϊ�з����������Գ�ʼ��Ϊ-128��
 * ��-128��ͼ�п���������Ȼ����0xFFFF��������(0...9),��������ϰ�ߡ�
 */
typedef struct {
    CTTF_HRPD_MAC_TRANS_MODE_ENUM_UINT8               enTransMode;       /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_RTC3MAC_PAKETSIZE_INDEX_ENUM_UINT8 enPktSize;         /* ��Чֵ: BUTT */
    VOS_UINT16                                        usPaddingByteSize; /* ��λ: �ֽڣ���Чֵ: 0xFFFF */
    VOS_INT8                                          cPacketId;         /* ��Чֵ: -128,��Чֵ0-9 */
    VOS_INT8                                          cTxT2pMax;         /* ��λ:dB,��Чֵ: -128 */
    VOS_INT8                                          cPotentialT2p;     /* ��λ:dB, ��Чֵ: -128 */
    VOS_UINT8                                         aucReserved[1];    /* �����ֽ�, ������Чֵ��д, ֵΪ0xFF */

    /* ����MAC�������� */
    CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_PER_MAC_FLOW_STRU astMacFlow[CTTF_HRPD_RTC3MAC_MNTN_FLOW_MAX_NUM];
} CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_STRU;

/*
 * �ṹ˵��: HRPD RTC3MAC ����Ϣͳ���ϱ�
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_REV_LINK_PACKET_INFO_IND
 * ˵��: �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 */
typedef struct {
    VOS_UINT16                              usPrimId;
    VOS_UINT16                              usToolId;
    CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_STRU astLinkPktData[CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_PACKET_PER_FLOW];
} CTTF_OM_HRPD_MNTN_RTC3MAC_LINK_PACKET_REPORT_STRU;

/* �ṹ˵��: HRPD RTC3MAC ÿ��MAC Flow��T2P��Ϣͳ�� */
typedef struct {
    VOS_UINT8 ucFlowId;        /* MAC��ID����Чֵ: 0xFF */
    VOS_INT8  cQrab;           /* -1:Ϊunloaded, 1:loaded, ��Чֵ: -128 */
    VOS_INT8  cDeltaT2p;       /* ��Чֵ: -128 */
    VOS_INT8  cBucketFactor;   /* ��Чֵ: -128 */
    VOS_INT16 sT2pInflow;      /* ��Чֵ: -32768 */
    VOS_INT16 sT2pOutflow;     /* ��Чֵ: -32768 */
    VOS_INT16 sBucketLevel;    /* ��Чֵ: -32768 */
    VOS_INT16 sBucketLevelSat; /* Saturation Bucket Level, ��Чֵ: -32768 */
} CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_PER_MAC_FLOW_STRU;

/* �ṹ˵��: HRPD RTC3MAC T2P��Ϣͳ�� */
typedef struct {
    CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_ENUM_UINT8 enWorkMode; /* ��Чֵ: BUTT */
    VOS_INT8                                    cQRABn;     /* -1:Ϊunloaded, 1:loaded, ��Чֵ: -128 */
    VOS_INT16                                   sFrab;      /* ��Чֵ: -32768 */

    /* ����MAC�������� */
    CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_PER_MAC_FLOW_STRU astMacFlow[CTTF_HRPD_RTC3MAC_MNTN_FLOW_MAX_NUM];
} CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_STRU;

/*
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_REV_LINK_T2P_STATISTICS_IND
 * ˵��: �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 */
typedef struct {
    VOS_UINT16                           usPrimId;
    VOS_UINT16                           usToolId;
    CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_STRU astLinkT2pData[CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_T2P_PER_FLOW];
} CTTF_OM_HRPD_MNTN_RTC3MAC_LINK_T2P_REPORT_STRU;

/*
 * �ṹ˵��: HRPD RTC3MAC Harq��Ϣ��ProcessId�ȶ���Ϊ�з����������Գ�ʼ��Ϊ-128��
 *           ��-128����������һ��˼ά��ͼ�ο���������Ȼ����0xFF��������(0...9),
 *           ��������ϰ�ߡ�
 */
typedef struct {
    VOS_INT8 cProcessId;   /* Invalid value:-128, the processer id of the next sub frame, value range is 0~2 */
    VOS_INT8 cSubPacketId; /* Invalid value:-128, 0:first time,1:second time,2:third time,3:fouth time */
    /*
     * Invalid value:-128, it indicates which type data will
     * CSDR_HRPD_SUBFRAME_DATA_TYPE_ENUM_UINT16 end. Possible values are : 0:no data to send, 1:new
     * data, 2:retransmission data, 3:silence duration
     */
    VOS_INT8 cDataType;
    VOS_INT8 cPacketId;    /* Invalid value:-128, value range is 0~9 */
} CTTF_HRPD_MNTN_RTC3MAC_HARQ_INFO_STRU;

/* �ṹ˵��: һ����֡��(4slots)Harq��Ϣ */
typedef struct {
    CTTF_HRPD_MNTN_RTC3MAC_HARQ_INFO_STRU astHarqInfo[CTTF_HRPD_MNTN_NUM_SUBFRAME_INFO_INSTANCES]; /* sub-frame
                                                                                                      information
                                                                                                      instances */
} CTTF_HRPD_MNTN_RTC3MAC_SUBFRAME_HARQ_STRU;

/*
 * �ṹ˵��: HRPD RTC3MAC Harq��Ϣͳ���ϱ�
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_HARQ_INFO_IND
 * ˵��: �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 */
typedef struct {
    VOS_UINT16                                usPrimId;
    VOS_UINT16                                usToolId;
    CTTF_HRPD_MNTN_RTC3MAC_SUBFRAME_HARQ_STRU astHarqData[CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_HARQ];
} CTTF_OM_HRPD_MNTN_RTC3MAC_HARQ_REPORT_STRU;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ����ϱ��ķ��ͷ����ͳ��
 */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh; /* ���͵����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;     /* ���͵����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;   /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;       /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;  /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;      /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTxNakRequestCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulIgnoredAnNakCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRlpFlushCnt;      /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_DEFAULT_RLP_REV_STATISTICS_STRU;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ����ϱ��Ľ��շ����ͳ��
 */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh;  /* ���յ����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* ���յ����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulDuplicateByteCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRxedNakRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortCnt;       /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_DEFAULT_RLP_FWD_STATISTICS_STRU;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: Default RLPÿ��ʵ���ͳ����Ϣ
 */
typedef struct {
    VOS_UINT8                                          ucStreamId;          /* ����RLPʵ���Ψһ���, ��Чֵ: 0xFF */
    VOS_UINT8                                          aucReserved[3];      /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
    VOS_UINT32                                         ulAnResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                                         ulAtResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                                         ulRxAnResetAckCnt;   /* ��Чֵ: 0xFFFFFFFF */
    CTTF_HRPD_MNTN_PER_DEFAULT_RLP_REV_STATISTICS_STRU stRevInfo;
    CTTF_HRPD_MNTN_PER_DEFAULT_RLP_FWD_STATISTICS_STRU stFwdInfo;
} CTTF_HRPD_MNTN_PER_DEFAULT_RLP_STATISTICS_STRU;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: Default RLP��ͳ����Ϣ
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_RLP_STATISTICS_IND
 */
typedef struct {
    VOS_UINT16                                     usPrimId;
    VOS_UINT16                                     usToolId;
    CTTF_HRPD_MNTN_PER_DEFAULT_RLP_STATISTICS_STRU astRlpEntity[CTTF_HRPD_MNTN_DEFAULT_RLP_MAX_NUM];
} CTTF_OM_HRPD_MNTN_DEFAULT_RLP_STATISTICS_STRU;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: MFPA����ϱ��ķ��ͷ����ͳ��
 */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh;  /* ���͵����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* ���͵����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTxNakRequestCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulIgnoredAnNakCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRlpFlushCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_MF_RLP_REV_STATISTICS_STRU;

/* �ṹ˵��: MFPA����ϱ��Ľ��շ����ͳ�� */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh;  /* ���յ����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulDuplicateByteCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRxedNakRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_MF_RLP_FWD_STATISTICS_STRU;

/* �ṹ˵��: MFPAÿ��RLPʵ���ͳ����Ϣ */
typedef struct {
    VOS_UINT8                                     ucRlpFlowId;    /* ����RLPʵ���Ψһ���, ��Чֵ: 0xFF */
    VOS_UINT8                                     aucReserved[3]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
    CTTF_HRPD_MNTN_PER_MF_RLP_REV_STATISTICS_STRU stRevInfo;
    CTTF_HRPD_MNTN_PER_MF_RLP_FWD_STATISTICS_STRU stFwdInfo;
} CTTF_HRPD_MNTN_PER_MF_RLP_STATISTICS_STRU;

/*
 * �ṹ˵��: MFPA�Ŀ�ά�ɲ���Ϣ
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_MFPA_RLP_STATISTICS_IND
 */
typedef struct {
    VOS_UINT16                                usPrimId;
    VOS_UINT16                                usToolId;
    CTTF_HRPD_MNTN_PER_MF_RLP_STATISTICS_STRU astRlpEntity[CTTF_HRPD_MNTN_MF_RLP_MAX_NUM];
} CTTF_OM_HRPD_MNTN_MF_RLP_STATISTICS_STRU;

/* �ṹ˵��: EMPA����ϱ��ķ��ͷ����ͳ�� */
typedef struct {
    VOS_UINT32 ulSelectRouteACnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulSelectRouteBCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCntHigh;  /* ���͵����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTxNakReportCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulIgnoreAnNakCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRlpFlushCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_EMF_RLP_REV_STATISTICS_STRU;

/* �ṹ˵��: EMPA����ϱ��ķ��ͷ����ͳ�� */
typedef struct {
    VOS_UINT32 ulSelectRouteACnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulSelectRouteBCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCntHigh;  /* ���յ����ֽ�������32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* ����32bit����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* ����32bit�� ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulDuplicateByteCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRxNakReportCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_EMF_RLP_FWD_STATISTICS_STRU;

/* �ṹ˵��: EMPA ÿ��RLPʵ��Ŀ�ά�ɲ���Ϣ */
typedef struct {
    VOS_UINT8                                   ucLinkFlowId;    /* ����RLPʵ���Ψһ���, ��Чֵ: 0xFF */
    CTTF_HRPD_PA_MNTN_FLOW_PROTOCOL_ENUM_UINT8  enFlowProtocol;  /* ��֧��FlowProtocolʱ��������Чֵ�ϱ���ֵΪ0xFF */
    CTTF_HRPD_PA_MNTN_ROUTE_PROTOCOL_ENUM_UINT8 enRouteProtocol; /* ��֧��RouteProtocolʱ��������Чֵ�ϱ���ֵΪ0xFF */
    VOS_UINT8                                   aucReserved[1];  /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
    CTTF_HRPD_MNTN_PER_EMF_RLP_REV_STATISTICS_STRU stRevInfo;
    CTTF_HRPD_MNTN_PER_EMF_RLP_FWD_STATISTICS_STRU stFwdInfo;
} CTTF_HRPD_MNTN_PER_EMF_RLP_STATISTICS_STRU;

/*
 * �ṹ˵��: EMPA�Ŀ�ά�ɲ���Ϣ
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_EMPA_RLP_STATISTICS_IND
 */
typedef struct {
    VOS_UINT16                                 usPrimId;
    VOS_UINT16                                 usToolId;
    CTTF_HRPD_MNTN_PER_EMF_RLP_STATISTICS_STRU astRlpEntity[CTTF_HRPD_MNTN_EMF_RLP_MAX_NUM];
} CTTF_OM_HRPD_MNTN_EMF_RLP_STATISTICS_STRU;

/* �ṹ˵��: Acmac��ά�ɲ���Ϣͳ�� */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;                  /* C.H1001��subprotocol��ֵ����Чֵ0xFF */
    VOS_UINT8                              aucReserve[3];                  /* �����ֽڣ�������Чֵ��д��ֵΪ0xFF */
    VOS_UINT32                             ulSuccAttemptCnt;               /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulMaxProbeFailCnt;              /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulAdjacentProbeTimerExpiredCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulMailboxFailCnt;               /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulCprocFailCnt;                 /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulSndErrorIndCnt;               /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe1Cnt;     /* Access Probe 1�ɹ�����, ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe2Cnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe3Cnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe4Cnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe5Cnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbeAboveCnt; /* ����Access Probe 5�ųɹ�����, ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;     /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt;    /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_ACMAC_MNTN_STATISTIC_STRU;

/* �ṹ˵��: Ccmac��ά�ɲ���Ϣͳ�� */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;      /* C.H1001��subprotocol��ֵ����Чֵ0xFF */
    VOS_UINT8                              aucReserve[3];      /* �����ֽڣ�������Чֵ��д��ֵΪ0xFF */
    VOS_UINT32                             ulPhyPkt128BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulMacLayerPktCnt;   /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_CCMAC_MNTN_STATISTIC_STRU;

/* �ṹ˵��: RTCMAC��ά�ɲ���Ϣͳ�� */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;       /* C.H1001��subprotocol��ֵ����Чֵ0xFF */
    CTTF_HRPD_RTCMAC_MNTN_RATE_ENUM_UINT8  enRateLimit;         /* ��Чֵ0xFF  */
    VOS_UINT8                              aucReserve[2];       /* �����ֽڣ�������Чֵ��д��ֵΪ0xFF */
    VOS_UINT32                             ulPhyPkt128BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt768BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1536BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt2048BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt3072BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt4096BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt6144BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt8192BitCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt12288BitCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulSndErrorIndCnt;    /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_RTCMAC_MNTN_STATISTIC_STRU;

/* �ṹ˵��: FTCMAC��ά�ɲ���Ϣͳ�� */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;        /* C.H1001��subprotocol��ֵ����Чֵ0xFF */
    VOS_UINT8                              aucReserve[3];        /* �����ֽڣ�������Чֵ��д��ֵΪ0xFF */
    VOS_UINT32                             ulPhyPkt128BitCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt2048BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt3072BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt4096BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt5120BitCnt;   /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulTotPhyPktCnt;       /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulTotMacLayPktCnt;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulTotSingUserPktCnt;  /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulTotMultiUserPktCnt; /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32                             ulMaxSecLayPktNum;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotalMacPacketBytes;       /* MAC Packet Byte Size,������2bits MAC trailer����Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotMyAtMacPayloadBytes;    /* ��Чֵ: 0xFFFFFFFF */
    VOS_UINT32 ulTotOtherAtMacPayloadBytes; /* ��Чֵ: 0xFFFFFFFF */
} CTTF_HRPD_FTCMAC_MNTN_STATISTIC_STRU;

/*
 * �ṹ˵��: HRPD MAC ��Ϣͳ���ϱ�
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_MAC_PROTOCOL_STATISTICS_IND
 * ˵��: �����ϱ���Ϣ, ��API����ʱ��ָ��ָ���ϱ����ݼ���
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_HRPD_ACMAC_MNTN_STATISTIC_STRU  stAcMac;
    CTTF_HRPD_CCMAC_MNTN_STATISTIC_STRU  stCcMac;
    CTTF_HRPD_RTCMAC_MNTN_STATISTIC_STRU stRtcMac;
    CTTF_HRPD_FTCMAC_MNTN_STATISTIC_STRU stFtcMac;
} CTTF_OM_HRPD_MAC_MNTN_STATISTIC_STRU;

/* Description: DCC��ECCЭ��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_CC_STATE_ENUM_UINT8     enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                              aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_CC_MAC_PROTOCL_STATE_STRU;

/* Description: DAC��EACЭ��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_AC_STATE_ENUM_UINT8     enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                              aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_AC_MAC_PROTOCL_STATE_STRU;

/* Description: DFTC��EFTCЭ��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_FTC_STATE_ENUM_UINT8    enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                              aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_FTC_MAC_PROTOCL_STATE_STRU;

/* Description: DRTC��ST3 RTCЭ��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_RTC_STATE_ENUM_UINT8    enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                              aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_RTC_MAC_PROTOCL_STATE_STRU;

/* Description: ��ȨЭ��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                  enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8 enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                                               aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_AUTHENTICATION_PROTOCL_STATE_STRU;

/*
 * Name: CTTF_HRPD_MNTN_MODULE_ENCRYPTION_PROTOCL_STATE_STRU
 * Description: ����Э��״̬�ṹ
 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                  enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8 enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                                               aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_ENCRYPTION_PROTOCL_STATE_STRU;

/* Description: ��ȫЭ��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                  enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8 enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                                               aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_SECURITY_PROTOCL_STATE_STRU;

/* Description: ����Э��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                    enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_ENUM_UINT8 enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                                                 aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_STREAM_FLOW_CTRL_PROTOCL_STATE_STRU;

/* Description: ��Ӧ��Э��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8              enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_ENUM_UINT8 enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                                           aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_STREAM_APP_PROTOCL_STATE_STRU;

/* Description: SNPЭ��״̬�ṹ */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8    enSubProtocol; /* ��Чֵ: BUTT */
    CTTF_HRPD_MNTN_SNP_UP_TX_STATE_ENUM_UINT8 enState;       /* ��Чֵ: BUTT */
    VOS_UINT8                                 aucReserve[2]; /* �����ֽڣ�������Чֵ��д��ֵΪ0 */
} CTTF_HRPD_MNTN_MODULE_SNP_UP_TX_STATE_STRU;

/*
 * Description: ģ��Э��״̬�ṹ
 * ��Ӧ��Ϣ: ID_CTTF_OM_HRPD_MNTN_MODULE_PROTOCOL_STATE_IND
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_HRPD_MNTN_MODULE_CC_MAC_PROTOCL_STATE_STRU           stCcMac;
    CTTF_HRPD_MNTN_MODULE_AC_MAC_PROTOCL_STATE_STRU           stAcMac;
    CTTF_HRPD_MNTN_MODULE_FTC_MAC_PROTOCL_STATE_STRU          stFtcMac;
    CTTF_HRPD_MNTN_MODULE_RTC_MAC_PROTOCL_STATE_STRU          stRtcMac;
    CTTF_HRPD_MNTN_MODULE_AUTHENTICATION_PROTOCL_STATE_STRU   stAuth;
    CTTF_HRPD_MNTN_MODULE_ENCRYPTION_PROTOCL_STATE_STRU       stEncryption;
    CTTF_HRPD_MNTN_MODULE_SECURITY_PROTOCL_STATE_STRU         stSecurity;
    CTTF_HRPD_MNTN_MODULE_STREAM_FLOW_CTRL_PROTOCL_STATE_STRU stStream0;
    CTTF_HRPD_MNTN_MODULE_STREAM_FLOW_CTRL_PROTOCL_STATE_STRU stStream1;
    CTTF_HRPD_MNTN_MODULE_STREAM_FLOW_CTRL_PROTOCL_STATE_STRU stStream2;
    CTTF_HRPD_MNTN_MODULE_STREAM_FLOW_CTRL_PROTOCL_STATE_STRU stStream3;
    CTTF_HRPD_MNTN_MODULE_STREAM_APP_PROTOCL_STATE_STRU       stStream0App;
    CTTF_HRPD_MNTN_MODULE_STREAM_APP_PROTOCL_STATE_STRU       stStream1App;
    CTTF_HRPD_MNTN_MODULE_STREAM_APP_PROTOCL_STATE_STRU       stStream2App;
    CTTF_HRPD_MNTN_MODULE_STREAM_APP_PROTOCL_STATE_STRU       stStream3App;
    CTTF_HRPD_MNTN_MODULE_SNP_UP_TX_STATE_STRU                stSnpUplinkTx;
} CTTF_OM_HRPD_MNTN_MODULE_PROTOCOL_STATE_STRU;

/* �ṹ˵��: RTCά���ϱ���ACK��Ϣ */
typedef struct {
    VOS_UINT32 aulACKsNum[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* ulInterlace[interlacenumber]������֮��,��Ӧ��ͬ����ģʽ */
    VOS_UINT32 aulPktNumOnInterlace[CTTF_HRPD_MNTN_RTC3MAC_INTERLACE_MAX_NUM][CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* ��Ӧ��֯���յ���ACK���������ۼ�ֵ����Ӧ��ͬ�Ĵ���ģʽ */
} CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STRU;

/* �ṹ˵��: RTC Reverse Link Metrics ͳ����Ϣ */
typedef struct {
    VOS_UINT32 ulGoodPacketNumSum; /* ulGoodPacketNum[CTTF_HRPD_RTC3MAC_TRANS_MODE_BUTT]��� */
    VOS_UINT32 aulGoodPacketNum[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* �ܷ��ͳɹ���Packet������ֵ����ACK�ĸ��� */
    VOS_UINT32 ulAvgPacketTerminationSum; /* ulAvgPacketTermination[CTTF_HRPD_RTC3MAC_TRANS_MODE_BUTT]��� */
    VOS_UINT32 aulAvgPacketTermination[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* ����֡Ϊ��λ����Packetƽ�����͵���֡�� */
    VOS_UINT32 ulTotlPktTxTimeSum; /* ulTotlPktTxTime[CTTF_HRPD_RTC3MAC_TRANS_MODE_BUTT]��� */
    VOS_UINT32 aulTotlPktTxTime[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* ���ݰ��ܵķ���ʱ�䣬��msΪ��λ */
} CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STATIS_STRU;

/* �ṹ˵��: RTC Reverse Link Metrics��Ϣ */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;
    /* ÿ��PS���Ͷ�Ӧ��ACK info, PS0��Ӧ��λ�ò����õ�������λ���������"����PS���͵�ACK info֮��" */
    CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STRU astRtc3MacAckInfo[CTTF_HRPD_MNTN_RTC3MAC_PS_BUTT][CSDR_HARQ_MNTN_SUBPACKET_BUTT];
    /* ÿ��PS���Ͷ�Ӧ��ACKͳ����Ϣ,PS0��Ӧ��λ�ò����õ�������λ���������"����PS���Ͷ�Ӧ��ACKͳ����Ϣ֮��" */
    CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STATIS_STRU astRtc3MacAckInfoStatis[CTTF_HRPD_MNTN_RTC3MAC_PS_BUTT]; 
    VOS_UINT32 ulTotalParqNAKsNum;   /* �յ�PARQ NAK�ĸ������ۼ�ֵ�������ִ���ģʽҲ������PS���� */
    VOS_UINT32 ulTotalPacketSentNum; /* �ܷ��͵�Packet����������ACK��PARQ�е�NAK�������ִ���ģʽҲ������PS���� */
    VOS_UINT32 ulTotalPer;           /* ��ʾ�е�����ʣ�����ֵ�Ѿ��Ŵ�100������ʾ��ʱ����%����ʽ��ʾ */
} CTTF_OM_HRPD_MNTN_RTC3MAC_ACK_INFO_REPORT_STRU;


#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

