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
 * 文件说明:CTTF使用的结构统一包含到该文件中。
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
/* 典型的RLP实体的个数 */
#define CTTF_HRPD_MNTN_DEFAULT_RLP_MAX_NUM 2

/* MFPA RLP实体的最大个数 */
#define CTTF_HRPD_MNTN_MF_RLP_MAX_NUM 6

/* EMPA RLP实体的最大个数 */
#define CTTF_HRPD_MNTN_EMF_RLP_MAX_NUM 6

/*
 * 根据CDMA 5.12s周期, 建议每1.28s上报一次。1.28s=64*20ms, 64作为数组下标,
 * （一次上报64帧的情况）, 内存可以接受
 *  该宏大小变化时，必须同步更改CTTF_1X_MAC_MNTN_BUF_MAX_SIZE
 */
#define CTTF_1X_MAC_MNTN_MAX_REPORT_NUM 64

/* 因为HIDS不支持动态添加、删除RLP实体, 所以讨论后决定按照协议最大值定义该宏大小变化时，必须同步更改CTTF_1X_RLP_MNTN_BUF_MAX_SIZE */
#define CTTF_1X_RLP_MNTN_MAX_NUM 6

/* 上报的MAC流最大个数，参考标杆设计 */
#define CTTF_HRPD_RTC3MAC_MNTN_FLOW_MAX_NUM 8

/*
 * 按照协议16slot为一个MAC Layer Packet, 每一个子帧记录一次。一个子帧为4个slot,
 * 则16个slot会产生16/4=4组数据一个slot为1.28ms，那么16个slot对应26.66ms。
 * 因为640ms上报一次消息，那么消息中一个MAC流会产生有24组数据（640=48*26.66ms）。
 * 一组数据记录4个子帧的信息，即1.28s中每个流上报24*4= 96组。
 */
#define CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_PACKET_PER_FLOW 96
#define CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_T2P_PER_FLOW 96
#define CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_HARQ 96

/* It's define the num of  subframe information instances */
#define CTTF_HRPD_MNTN_NUM_SUBFRAME_INFO_INSTANCES 3

/* rtc3支持的最大交织(进程)数 */
#define CTTF_HRPD_MNTN_RTC3MAC_INTERLACE_MAX_NUM 3

enum CTTF_MNTN_MSG_TYPE_ENUM {
    /* 1X的可维可测消息的编号以D4开头 */
    ID_OM_CTTF_1X_MNTN_CONFIG_REQ                    = 0xD400, /* 1X 配置开关请求, 用于下发配置 */
    ID_CTTF_OM_1X_MNTN_CONFIG_CNF                    = 0xD400, /* 1X 配置开关回复, 用于回复配置结果 */
    ID_CTTF_OM_1X_MNTN_FWD_LINK_IND                  = 0xD402, /* 1X Forward Link Info上报 */
    ID_CTTF_OM_1X_MNTN_REV_LINK_IND                  = 0xD403, /* 1X Reverse Link Info上报 */
    ID_CTTF_OM_1X_MNTN_RLP_STATISTICS_IND            = 0xD404, /* 1X RLP statistics上报 */
    ID_CTTF_OM_1X_MNTN_COMMON_CHANNEL_STATISTICS_IND = 0xD405, /* Common Channel statistics上报 */
    ID_OM_CTTF_1X_MNTN_TRACE_CONFIG_REQ              = 0xD406, /* 控制1x的trace配置开关请求 */

    /* EVDO的可维可测消息(含HRPD和eHRPD)的编号以D6开头 */
    ID_OM_CTTF_HRPD_MNTN_CONFIG_REQ                  = 0xD600, /* HRPD 配置开关请求, 用于下发配置 */
    ID_CTTF_OM_HRPD_MNTN_CONFIG_CNF                  = 0xD601, /* HRPD 配置开关回复, 用于回复配置结果 */
    ID_CTTF_OM_HRPD_MNTN_MODULE_PROTOCOL_STATE_IND   = 0xD602, /* HRPD Module Protocol State上报 */
    ID_CTTF_OM_HRPD_MNTN_MAC_PROTOCOL_STATISTICS_IND = 0xD603, /* HRPD MAC Protocol Statistics上报 */
    ID_CTTF_OM_HRPD_MNTN_RLP_STATISTICS_IND          = 0xD604, /* HRPD RLP statistics上报 */
    ID_CTTF_OM_HRPD_MNTN_MFPA_RLP_STATISTICS_IND     = 0xD605, /* MFPA RLP statistics上报 */
    ID_CTTF_OM_HRPD_MNTN_EMPA_RLP_STATISTICS_IND     = 0xD606, /* EMPA RLP statistics上报 */
    ID_CTTF_OM_HRPD_MNTN_REV_LINK_PACKET_INFO_IND    = 0xD607, /* HRPD Reverse Link Packet Info上报 */
    ID_CTTF_OM_HRPD_MNTN_REV_LINK_T2P_STATISTICS_IND = 0xD608, /* HRPD Reverse Link T2P statistics上报 */
    ID_CTTF_OM_HRPD_MNTN_HARQ_INFO_IND               = 0xD609, /* HRPD Harq Info上报 */
    ID_CTTF_OM_HRPD_MNTN_RTC3MAC_ACK_INFO_IND        = 0xD60A, /* HRPD RTC3MAC Info上报 */
    ID_CTTF_OM_HRPD_MNTN_TRACE_MSG_CONFIG_REQ        = 0xD60B, /* HRPD Trace Msg 配置消息 */

    CTTF_MNTN_MSG_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT16 CTTF_MNTN_MSG_TYPE_ENUM_UINT16;

enum CTTF_MNTN_RPT_CMD_ENUM {
    CTTF_MNTN_RPT_CMD_STOP     = 0x00, /*停止上报*/
    CTTF_MNTN_RPT_CMD_START    = 0x01  /*开始上报*/
};

typedef VOS_UINT8 CTTF_MNTN_RPT_CMD_ENUM_UINT8;

enum CTTF_MNTN_RLP_RPT_CMD_ENUM {
    CTTF_MNTN_RLP_RPT_CMD_STOP   = 0x00, /*停止上报*/
    CTTF_MNTN_RLP_RPT_CMD_START  = 0x01, /*开始上报*/
    CTTF_MNTN_RLP_RPT_CMD_CLEAR  = 0x02  /*清除*/
};
typedef VOS_UINT8 CTTF_MNTN_RLP_RPT_CMD_ENUM_UINT8;

/* hrpd上报结果 */
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
    CTTF_MNTN_COMM_RPT_CMD_STOP  = 0x00, /*停止上报*/
    CTTF_MNTN_COMM_RPT_CMD_START = 0x01, /*开始上报*/
    CTTF_MNTN_COMM_RPT_CMD_CLEAR = 0x02  /*清除*/
};
typedef VOS_UINT8 CTTF_MNTN_COMM_RPT_CMD_ENUM_UINT8;

/* HIDS工具, 要求每个枚举值都必须显示定义其值, 否则不能解析 */
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
    CTTF_1X_MAC_MNTN_RPT_PERIOD_1280 = 1280, /* 上报周期1280ms */
};
typedef VOS_UINT32 CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32;

/* hrpd上报周期 */
enum CTTF_HRPD_MNTN_RPT_PERIOD_ENUM {
    CTTF_HRPD_MNTN_RPT_PERIOD_1280 = 1280 /* 上报周期1280ms */
};

typedef VOS_UINT32 CTTF_HRPD_MNTN_RPT_PERIOD_ENUM_UINT32;

/* hrpd RTC 画图上报周期 */
enum CTTF_HRPD_MNTN_RTC_RPT_PERIOD_ENUM {
    CTTF_HRPD_MNTN_RTC_RPT_PERIOD_640 = 640 /* 上报周期640ms */
};
typedef VOS_UINT32 CTTF_HRPD_MNTN_RTC_RPT_PERIOD_ENUM_UINT32;

/* HRPD子协议类型 */
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

/* 传输模式 */
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

/* T2P工作模式 */
enum CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_ENUM {
    CTTF_HRPD_MNTN_RTC3MAC_MODE_NORMAL                = 0x00, /* 通常模式 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_PHY_FORBIDDEN         = 0x01, /* PHY禁止发送 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_SUPERVISION_FAILED    = 0x02, /* supervision failed之后，与网络失去联系 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_SILENCE_DURATION      = 0x03, /* 静默 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_TEST_LOOP             = 0x04, /* 测试模式 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_PREEMPT               = 0x05, /* 被抢占 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_RTC_SETUP_STATUS      = 0x06, /* RTC3处于setup状态 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_ATTR_PARA_CHANGE_NEGO = 0x07, /* 重协商改变协商参数 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_ATTR_PARA_CHANGE_GAUP = 0x08, /* GAUP改变协商参数 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_TCH_PARA_CHANGE       = 0x09, /* TCH参数改变 */
    CTTF_HRPD_MNTN_RTC3MAC_MODE_OTHER                 = 0x0A, /* 其它 */
    CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_BUTT             = 0x0B
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_ENUM_UINT8;

/* DCC和ECC的状态,按照工具的要求, DCC和ECC的状态要定义在一起 */
enum CTTF_HRPD_MNTN_CC_STATE_ENUM {
    CTTF_HRPD_MNTN_CC_INACTIVE_STATE = 0x0,
    CTTF_HRPD_MNTN_CC_ACTIVE_STATE   = 0x1,
    CTTF_HRPD_MNTN_CC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_CC_STATE_ENUM_UINT8;

/* SDAC和EAC的状态,按照工具的要求, DAC和EAC的状态要定义在一起 */
enum CTTF_HRPD_MNTN_AC_STATE_ENUM {
    CTTF_HRPD_MNTN_AC_INACTIVE_STATE = 0x0,
    CTTF_HRPD_MNTN_AC_ACTIVE_STATE   = 0x1,
    CTTF_HRPD_MNTN_AC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_AC_STATE_ENUM_UINT8;

/* Description: DFTC和EFTC的状态,按照工具的要求, DFTC和EFTC的状态要定义在一起 */
enum CTTF_HRPD_MNTN_FTC_STATE_ENUM {
    CTTF_HRPD_MNTN_FTC_INACTIVE_STATE      = 0x0,
    CTTF_HRPD_MNTN_FTC_VARIABLE_RATE_STATE = 0x1,
    CTTF_HRPD_MNTN_FTC_FIXED_RATE_STATE    = 0x2,
    CTTF_HRPD_MNTN_FTC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_FTC_STATE_ENUM_UINT8;

/* Description: DRTC和ST3 RTC的状态,按照工具的要求, DRTC和ST3 RTC的状态要定义在一起 */
enum CTTF_HRPD_MNTN_RTC_STATE_ENUM {
    CTTF_HRPD_MNTN_RTC_INACTIVE_STATE  = 0x0,
    CTTF_HRPD_MNTN_RTC_SETUP_STATE     = 0x1,
    CTTF_HRPD_MNTN_RTC_OPEN_STATE      = 0x2,
    CTTF_HRPD_MNTN_RTC_SUSPENDED_STATE = 0x3,
    CTTF_HRPD_MNTN_RTC_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_RTC_STATE_ENUM_UINT8;

/* Description: 安全层协议状态 */
enum CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM {
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_NO_STATE = 0x0,
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8;

/* Description: 流控层协议状态 */
enum CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_ENUM {
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_CLOSE_STATE = 0x0,
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_OPEN_STATE  = 0x1,
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_ENUM_UINT8;

/*
 * Description: 流应用层协议状态
 *              工具要求Default Packet Application, Multi-flow Packet Application,
 *              Enhanced Multi-flow Packet Application, Data Over Signalling,
 *              Test Application, 以及Enhanced Test Application定义在一起
 */
enum CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_ENUM {
    CTTF_HRPD_MNTN_STREAM_APP_RESERVATION_CLOSE_STATE = 0x0,
    CTTF_HRPD_MNTN_STREAM_APP_RESERVATION_STATE       = 0x1,
    CTTF_HRPD_MNTN_STREAM_APP_NO_STATE                = 0x2,
    CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_ENUM_UINT8;

/* Description: SNP协议状态 */
enum CTTF_HRPD_MNTN_SNP_UP_TX_STATE_ENUM {
    CTTF_HRPD_MNTN_SNP_UP_TX_ALLOW_STATE     = 0x0,
    CTTF_HRPD_MNTN_SNP_UP_TX_NOT_ALLOW_STATE = 0x1,
    CTTF_HRPD_MNTN_SNP_UP_TX_STATE_BUTT
};
typedef VOS_UINT8 CTTF_HRPD_MNTN_SNP_UP_TX_STATE_ENUM_UINT8;

/* Description: Default RTC 速率限制枚举 */
enum CTTF_HRPD_RTCMAC_MNTN_RATE_ENUM {
    CTTF_HRPD_RTCMAC_MNTN_RATE_NULL          = 0,
    CTTF_HRPD_RTCMAC_MNTN_RATE_9K6           = 1,
    CTTF_HRPD_RTCMAC_MNTN_RATE_19K2          = 2,
    CTTF_HRPD_RTCMAC_MNTN_RATE_38K4          = 3,
    CTTF_HRPD_RTCMAC_MNTN_RATE_76K8          = 4,
    CTTF_HRPD_RTCMAC_MNTN_RATE_153K6         = 5,
    CTTF_HRPD_RTCMAC_MNTN_RATE_NO_LIMITATION = 6, /* 表示对Rate无限制 */
    CTTF_HRPD_RTCMAC_MNTN_RATE_BUTT          = 7  /* 表示无效 */
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

/* 枚举说明: 物理层支持的PacketSize类型 */
enum CTTF_HRPD_MNTN_RTC3MAC_PAKETSIZE_INDEX_ENUM {
    CTTF_HRPD_MNTN_RTC3MAC_PS_0     = 0x00, /* PS大小为0bit<-->0Byte */
    CTTF_HRPD_MNTN_RTC3MAC_PS_128   = 0x01, /* PS大小为128bit<-->16BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_256   = 0x02, /* PS大小为256bit<-->32BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_512   = 0x03, /* PS大小为512bit<-->64BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_768   = 0x04, /* PS大小为768bit<-->96BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_1024  = 0x05, /* PS大小为1024bit<-->128BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_1536  = 0x06, /* PS大小为1536bit<-->192BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_2048  = 0x07, /* PS大小为2048bit<-->256BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_3072  = 0x08, /* PS大小为3072bit<-->384BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_4096  = 0x09, /* PS大小为4096bit<-->512BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_6144  = 0x0A, /* PS大小为6144bit<-->768BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_8192  = 0x0B, /* PS大小为8192bit<-->1024BYTE */
    CTTF_HRPD_MNTN_RTC3MAC_PS_12288 = 0x0C, /* PS大小为12288bit<-->1536BYTE */
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

/* 1X的可维可测消息 */
/*
 * 结构名: OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU
 * 结构说明: 1X下通用配置命令结构
 */
typedef struct {
    CTTF_MNTN_RPT_CMD_ENUM_UINT8             enCommand; /* 开始或停止上报 */
    VOS_UINT8                               aucReserve[3];
    CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* 上报周期, 单位: ms */
} OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU;

/* 结构说明: 1X下RLP配置命令结构 */
typedef struct {
    CTTF_MNTN_RLP_RPT_CMD_ENUM_UINT8         enCommand; /* 开始或停止上报或清除 */
    VOS_UINT8                               aucReserve[3];
    CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* 上报周期, 单位: ms */
} OM_CTTF_1X_MNTN_RLP_GENERIC_CONFIG_STRU;

/* 结构说明: 1X下通用配置命令结构 */
typedef struct {
    CTTF_MNTN_COMM_RPT_CMD_ENUM_UINT8        enCommand; /* 开始或停止上报或清除 */
    VOS_UINT8                               aucReserve[3];
    CTTF_1X_MAC_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* 上报周期, 单位: ms */
} OM_CTTF_1X_MNTN_COMM_GENERIC_CONFIG_STRU;

/* 结构说明: HRPD通用配置命令结构 */
typedef struct {
    CTTF_MNTN_COMM_RPT_CMD_ENUM_UINT8      enCommand; /* 开始或停止上报或清除 */
    VOS_UINT8                             aucReserve[3];
    CTTF_HRPD_MNTN_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* 上报周期, 单位: ms */
} OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU;

/* 结构说明: HRPD通用配置命令结构 */
typedef struct {
    CTTF_MNTN_RPT_CMD_ENUM_UINT8               enCommand; /* 开始或停止上报 */
    VOS_UINT8                                 aucReserve[3];
    CTTF_HRPD_MNTN_RTC_RPT_PERIOD_ENUM_UINT32 enRptPeriod; /* 上报周期, 单位: ms */
} OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU;

/* 对应消息:  ID_OM_CTTF_1X_MNTN_CONFIG_REQ */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stTransHdr;

    /* 根据CDMA 5.12s周期, 参考QXDM, 建议每1.28s上报一次 */
    OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU      stFwdLinkInfo;      /* 1X Forward Link Info, 上报周期: 1280ms */
    OM_CTTF_1X_MNTN_GENERIC_CONFIG_STRU      stRevLinkInfo;      /* 1X Reverse Link Info, 上报周期: 1280ms */
    OM_CTTF_1X_MNTN_RLP_GENERIC_CONFIG_STRU  stRlpStatistics;    /* 1X RLP statistics, 上报周期: 1280ms */
    OM_CTTF_1X_MNTN_COMM_GENERIC_CONFIG_STRU stCommonStatistics; /* Common Channel statistics, 上报周期: 1280ms */
} OM_CTTF_1X_MNTN_CONFIG_REQ_STRU;

/* 对应消息:  ID_OM_CTTF_HRPD_MNTN_CONFIG_REQ */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stTransHdr;

    /* 根据CDMA 5.12s周期, 建议每1.28s上报一次 */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stMacStatistics;  /* HRPD MAC Statistics, 上报周期: 1280ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stRlpStatistics;  /* HRPD RLP Statistics, 上报周期: 1280ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stMfpaStatistics; /* MFPA RLP Statistics, 上报周期: 1280ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stEmpaStatistics; /* EMPA RLP Statistics, 上报周期: 1280ms */
    OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU    stRtc3PktInfo;    /* RTC3 Packet Info, 上报周期: 640ms */
    OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU    stT2pStatistics;  /* T2P Statistics, 上报周期: 640ms */
    OM_CTTF_HRPD_MNTN_COMM_CONFIG_STRU    stRtc3HarqInfo;   /* RTC3 Harq Info, 上报周期: 640ms */
    OM_CTTF_HRPD_MNTN_GENERIC_CONFIG_STRU stRtc3AckInfo;    /* RTC3 ACK Info, 上报周期: 1280ms */
} OM_CTTF_HRPD_MNTN_CONFIG_REQ_STRU;

/* 对应消息:  ID_CTTF_OM_HRPD_MNTN_CONFIG_CNF */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stDiagHdr;

    PS_RSLT_CODE_ENUM_UINT32 enRslt;   /* 配置成功或失败 */
    VOS_UINT32               ulReason; /* 带具体失败的原因 */
} CTTF_OM_HRPD_MNTN_CONFIG_CNF_STRU;

/*
 * 对应消息:  ID_CTTF_OM_1X_MNTN_CONFIG_CNF
 * 说明: 注意OM融合后, HIDS对X模的配置开关消息处理, 不同于GU模
 *       上报的CNF消息
 */
typedef struct {
    DIAG_TRANS_MSG_HDR_STRU stDiagHdr;

    PS_RSLT_CODE_ENUM_UINT32 enRslt;   /* 配置成功或失败 */
    VOS_UINT32               ulReason; /* 带具体失败的原因 */
} CTTF_OM_1X_MNTN_CONFIG_CNF_STRU;

/*
 * 结构说明: 某Forward信道的统计值
 * 该结构体大小变化时，必须同步更改CTTF_1X_MAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    CTTF_1X_MAC_MNTN_RATE_ENUM_UINT8 enRate;         /* 无效值为BUTT */
    VOS_UINT8                        ucPercentage;   /* 成功解析的块百分比分子, [0..100], 无效值0xFF */
    VOS_UINT8                        aucReserved[2]; /* 保留字节, 按照无效值填写, 值为0 */
} CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU;

/*
 * 对应消息:  ID_CTTF_OM_1X_MNTN_FWD_LINK_IND
 * 说明:  主动上报消息, 在API调用时用指针指向上报内容即可
 * 结构说明: 各Forward信道的统计值
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU astFch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];  /* FCH统计值 */
    CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU astDcch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM]; /* DCCH统计值 */
    CTTF_1X_MAC_MNTN_FWD_LINK_PER_INFO_STRU astSch0[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM]; /* SCH统计值 */
} CTTF_OM_1X_MNTN_FWD_LINK_IND_STRU;

/* CTTF_OM_1X_MNTN_xxx_IND_STRU上报消息中头的长度，sizeof(usPrimId)+sizeof(usToolId) */
#define CTTF_OM_1X_MNTN_IND_HEAD_BYTE_SIZE 4

/* CTTF_OM_HRPD_MNTN_xxx_IND_STRU上报消息中头的长度，sizeof(usPrimId)+sizeof(usToolId) */
#define CTTF_OM_HRPD_MNTN_IND_HEAD_BYTE_SIZE 4

/*
 * 结构说明: 某Reverse信道的统计值
 * 该结构体大小变化时，必须同步更改CTTF_1X_MAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    CTTF_1X_MAC_MNTN_RATE_ENUM_UINT8 enRate;         /* 无效值为BUTT */
    VOS_UINT8                        aucReserved[3]; /* 保留字节, 按照无效值填写, 值为0 */
} CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU;

/*
 * 对应消息:  ID_CTTF_OM_1X_MNTN_REV_LINK_IND
 * 说明:  主动上报消息, 在API调用时用指针指向上报内容即可
 * 结构说明: 各Reverse信道的统计值
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU astFch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];
    CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU astDcch[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];
    CTTF_1X_MAC_MNTN_REV_LINK_PER_INFO_STRU astSch0[CTTF_1X_MAC_MNTN_MAX_REPORT_NUM];
} CTTF_OM_1X_MNTN_REV_LINK_IND_STRU;

/* 结构说明: 某RLP实体的REV方向统计值 */
typedef struct {
    VOS_UINT32 ulFrameTotalByteHigh; /* 发送的帧的总字节数（高32bit）, 2015年工具不支持64bit类型, 故分拆开上报。无效值: 0xFFFFFFFF */
    VOS_UINT32 ulFrameTotalByte;     /* 发送的帧的总字节数（低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByteHigh; /* 上层软件发送的总字节（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByte;     /* 上层软件发送的总字节（低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalFrameCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulIdleFrameCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulFchNewDataFrameCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulFchRetxFrameCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulSchNewDataFrameCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulSchRetxFrameCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakSingle;          /* 重传一次后接收成功的次数, 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakDouble;          /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakTriple;          /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbove;           /* 重传超过3次的次数, 无效值: 0xFFFFFFFF */
} CTTF_1X_RLP_MNTN_REV_STATISTICS_PER_ENTITY_STRU;

/* 结构说明: 某RLP实体的FWD方向统计值 */
typedef struct {
    VOS_UINT32 ulFrameTotalByteHigh; /* 接收的帧的总字节数（高32bit）, 2015年工具不支持64bit类型。无效值: 0xFFFFFFFF */
    VOS_UINT32 ulFrameTotalByte;     /* 接收的帧的总字节数（低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByteHigh; /* 上层软件接收的总字节（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulUpperTotalByte;     /* 上层软件总接收的总字节（低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalFrameCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulIdleFrameCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulFillFrameCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulBlankFrameCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNullFrameCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulFchNewDataFrameCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulFchRetxFrameCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulSchNewDataFrameCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulSchRetxFrameCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakCtrlFrameCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortTimes;      /* NAK abort次数, 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulInsufficentQualityFrameCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidFcsFrameCnt;         /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidTypeFieldFrameCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidCtlFieldFrameCnt;    /* 无效值: 0xFFFFFFFF */
} CTTF_1X_RLP_MNTN_FWD_STATISTICS_PER_ENTITY_STRU;

/*
 * 结构说明: 某RLP实体的统计值
 * 该结构体大小变化时，必须同步更改CTTF_1X_RLP_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT8  ucSrId;                  /* 区分RLP实体的唯一编号, 无效值: 0xFF */
    VOS_UINT8  ucMeasuredRtt;           /* 最近一次测量的RTT, 无效值: 0xFF */
    VOS_UINT8  aucReserved[2];          /* 保留字节, 按照无效值填写, 值为0 */
    VOS_UINT16 usBSTriggeredResetTimes; /* AT收到AN的复位RLP请求而复位RLP的次数，无效值: 0xFFFF */
    VOS_UINT16 usMSTriggeredResetTimes; /* AT主动复位RLP的次数，无效值: 0xFFFF */
    VOS_UINT32 ulCurrentBo;             /* 实时BO, 无效值: 0xFFFFFFFF */
    CTTF_1X_RLP_MNTN_REV_STATISTICS_PER_ENTITY_STRU stRevInfo;
    CTTF_1X_RLP_MNTN_FWD_STATISTICS_PER_ENTITY_STRU stFwdInfo;
} CTTF_1X_RLP_MNTN_STATISTICS_PER_ENTITY_STRU;

/*
 * 对应消息:  ID_CTTF_OM_1X_MNTN_RLP_STATISTICS_IND
 * 结构说明: 各RLP实体的统计值
 * 说明:  主动上报消息, 在API调用时用指针指向上报内容即可
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_RLP_MNTN_STATISTICS_PER_ENTITY_STRU astRlpEntity[CTTF_1X_RLP_MNTN_MAX_NUM];
} CTTF_OM_1X_MNTN_RLP_STATISTICS_IND_STRU;

/*
 * 结构说明: PCH的统计值
 * 该结构体大小变化时，必须同步更改CTTF_1X_LAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT32 ulTotalEncapPduFragCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ul96BitEncapPduFragCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ul192BitEncapPduFragCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalEncapPduCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulInvalidCrcEncapPduCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulChanLostCnt;           /* 无效值: 0xFFFFFFFF */
} CTTF_1X_MNTN_PCH_STATISTIC_STRU;

/*
 * 结构说明: ACH的统计值
 * 该结构体大小变化时，必须同步更改CTTF_1X_LAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT32 ulRequestTypeSduCnt;           /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulResponseTypeSduCnt;          /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe1Cnt;             /* 接入1次成功的次数, 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe2Cnt;             /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe3Cnt;             /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe4Cnt;             /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbe5Cnt;             /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAccessProbeAboveCnt;         /* 接入5次以上才成功的次数, 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulUnsucessfulAccessAttemptCnt; /* 接入attempt失败次数, 无效值: 0xFFFFFFFF */
} CTTF_1X_MNTN_ACH_STATISTIC_STRU;

/*
 * 结构说明: PAGE的统计值
 * 该结构体大小变化时，必须同步更改CTTF_1X_LAC_MNTN_BUF_MAX_SIZE
 */
typedef struct {
    VOS_UINT32 ulGpmMatchedCnt;          /* 匹配上的GPM，无效值: 0xFFFFFFFF */
    VOS_UINT32 ulSlotPageMatchedCnt;     /* Slot模式匹配的Page，无效值: 0xFFFFFFFF */
    VOS_UINT32 ulMismatchedPageCnt;      /* 所有的不匹配的Page，无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAddrMatchedOtherMsgCnt; /* 除了Page消息外，所有的地址匹配的消息数，无效值: 0xFFFFFFFF */
} CTTF_1X_MNTN_PAGE_MSG_STATISTIC_STRU;

/*
 * 对应消息:  ID_CTTF_OM_1X_MNTN_COMMON_CHANNEL_STATISTICS_IND
 * 说明:  主动上报消息, 在API调用时用指针指向上报内容即可
 * 结构说明: 1X公共信道的统计值
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_1X_MNTN_PCH_STATISTIC_STRU      stPch;
    CTTF_1X_MNTN_ACH_STATISTIC_STRU      stAch;
    CTTF_1X_MNTN_PAGE_MSG_STATISTIC_STRU stPage;

} CTTF_OM_1X_MNTN_COMMON_CHANNEL_STATISTICS_IND_STRU;

/* 结构说明: HRPD RTC3MAC 每个MAC Flow的包统计 */
typedef struct {
    VOS_UINT8  ucFlowId;            /* MAC流ID，无效值: 0xFF */
    VOS_UINT8  aucReserved[1];      /* 保留字节, 按照无效值填写, 值为0xFF */
    VOS_UINT16 usActualAllocation;  /* 单位: 字节，无效值: 0xFFFF */
    VOS_UINT16 usSuggestAllocation; /* 单位: 字节，无效值: 0xFFFF */
    VOS_UINT16 usRequiredQueueSize; /* 单位: 字节，无效值: 0xFFFF */
    VOS_UINT16 usQOutflow;          /* 单位: 字节，无效值: 0xFFFF */
    VOS_UINT8  aucReserved1[2];     /* 保留字节, 按照无效值填写, 值为0xFF */
} CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_PER_MAC_FLOW_STRU;

/*
 * 结构说明: HRPD RTC3MAC 包信息统计,PacketId定义为有符号数，可以初始化为-128，
 * 从-128，图行看起来更自然。从0xFFFF跳到正数(0...9),看起来不习惯。
 */
typedef struct {
    CTTF_HRPD_MAC_TRANS_MODE_ENUM_UINT8               enTransMode;       /* 无效值: BUTT */
    CTTF_HRPD_MNTN_RTC3MAC_PAKETSIZE_INDEX_ENUM_UINT8 enPktSize;         /* 无效值: BUTT */
    VOS_UINT16                                        usPaddingByteSize; /* 单位: 字节，无效值: 0xFFFF */
    VOS_INT8                                          cPacketId;         /* 无效值: -128,有效值0-9 */
    VOS_INT8                                          cTxT2pMax;         /* 单位:dB,无效值: -128 */
    VOS_INT8                                          cPotentialT2p;     /* 单位:dB, 无效值: -128 */
    VOS_UINT8                                         aucReserved[1];    /* 保留字节, 按照无效值填写, 值为0xFF */

    /* 各个MAC流的数据 */
    CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_PER_MAC_FLOW_STRU astMacFlow[CTTF_HRPD_RTC3MAC_MNTN_FLOW_MAX_NUM];
} CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_STRU;

/*
 * 结构说明: HRPD RTC3MAC 包信息统计上报
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_REV_LINK_PACKET_INFO_IND
 * 说明: 主动上报消息, 在API调用时用指针指向上报内容即可
 */
typedef struct {
    VOS_UINT16                              usPrimId;
    VOS_UINT16                              usToolId;
    CTTF_HRPD_MNTN_RTC3MAC_LINK_PACKET_STRU astLinkPktData[CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_PACKET_PER_FLOW];
} CTTF_OM_HRPD_MNTN_RTC3MAC_LINK_PACKET_REPORT_STRU;

/* 结构说明: HRPD RTC3MAC 每个MAC Flow的T2P信息统计 */
typedef struct {
    VOS_UINT8 ucFlowId;        /* MAC流ID，无效值: 0xFF */
    VOS_INT8  cQrab;           /* -1:为unloaded, 1:loaded, 无效值: -128 */
    VOS_INT8  cDeltaT2p;       /* 无效值: -128 */
    VOS_INT8  cBucketFactor;   /* 无效值: -128 */
    VOS_INT16 sT2pInflow;      /* 无效值: -32768 */
    VOS_INT16 sT2pOutflow;     /* 无效值: -32768 */
    VOS_INT16 sBucketLevel;    /* 无效值: -32768 */
    VOS_INT16 sBucketLevelSat; /* Saturation Bucket Level, 无效值: -32768 */
} CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_PER_MAC_FLOW_STRU;

/* 结构说明: HRPD RTC3MAC T2P信息统计 */
typedef struct {
    CTTF_HRPD_MNTN_RTC3MAC_WORK_MODE_ENUM_UINT8 enWorkMode; /* 无效值: BUTT */
    VOS_INT8                                    cQRABn;     /* -1:为unloaded, 1:loaded, 无效值: -128 */
    VOS_INT16                                   sFrab;      /* 无效值: -32768 */

    /* 各个MAC流的数据 */
    CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_PER_MAC_FLOW_STRU astMacFlow[CTTF_HRPD_RTC3MAC_MNTN_FLOW_MAX_NUM];
} CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_STRU;

/*
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_REV_LINK_T2P_STATISTICS_IND
 * 说明: 主动上报消息, 在API调用时用指针指向上报内容即可
 */
typedef struct {
    VOS_UINT16                           usPrimId;
    VOS_UINT16                           usToolId;
    CTTF_HRPD_MNTN_RTC3MAC_LINK_T2P_STRU astLinkT2pData[CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_T2P_PER_FLOW];
} CTTF_OM_HRPD_MNTN_RTC3MAC_LINK_T2P_REPORT_STRU;

/*
 * 结构说明: HRPD RTC3MAC Harq信息，ProcessId等定义为有符号数，可以初始化为-128，
 *           从-128到正数符号一般思维，图形看起来更自然。从0xFF跳到正数(0...9),
 *           看起来不习惯。
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

/* 结构说明: 一个子帧内(4slots)Harq信息 */
typedef struct {
    CTTF_HRPD_MNTN_RTC3MAC_HARQ_INFO_STRU astHarqInfo[CTTF_HRPD_MNTN_NUM_SUBFRAME_INFO_INSTANCES]; /* sub-frame
                                                                                                      information
                                                                                                      instances */
} CTTF_HRPD_MNTN_RTC3MAC_SUBFRAME_HARQ_STRU;

/*
 * 结构说明: HRPD RTC3MAC Harq信息统计上报
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_HARQ_INFO_IND
 * 说明: 主动上报消息, 在API调用时用指针指向上报内容即可
 */
typedef struct {
    VOS_UINT16                                usPrimId;
    VOS_UINT16                                usToolId;
    CTTF_HRPD_MNTN_RTC3MAC_SUBFRAME_HARQ_STRU astHarqData[CTTF_HRPD_RTC3MAC_MNTN_MAX_RECORD_HARQ];
} CTTF_OM_HRPD_MNTN_RTC3MAC_HARQ_REPORT_STRU;

/*
 * 协议表格:
 * ASN.1描述:
 * 结构说明: 板侧上报的发送方向的统计
 */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh; /* 发送的总字节数（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;     /* 发送的总字节数（低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;   /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;       /* （低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;  /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;      /* （低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTxNakRequestCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulIgnoredAnNakCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRlpFlushCnt;      /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_DEFAULT_RLP_REV_STATISTICS_STRU;

/*
 * 协议表格:
 * ASN.1描述:
 * 结构说明: 板侧上报的接收方向的统计
 */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh;  /* 接收的总字节数（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* 接收的总字节数（低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* （低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* （低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulDuplicateByteCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRxedNakRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortCnt;       /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_DEFAULT_RLP_FWD_STATISTICS_STRU;

/*
 * 协议表格:
 * ASN.1描述:
 * 结构说明: Default RLP每个实体的统计信息
 */
typedef struct {
    VOS_UINT8                                          ucStreamId;          /* 区分RLP实体的唯一编号, 无效值: 0xFF */
    VOS_UINT8                                          aucReserved[3];      /* 保留字节，按照无效值填写，值为0 */
    VOS_UINT32                                         ulAnResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                                         ulAtResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                                         ulRxAnResetAckCnt;   /* 无效值: 0xFFFFFFFF */
    CTTF_HRPD_MNTN_PER_DEFAULT_RLP_REV_STATISTICS_STRU stRevInfo;
    CTTF_HRPD_MNTN_PER_DEFAULT_RLP_FWD_STATISTICS_STRU stFwdInfo;
} CTTF_HRPD_MNTN_PER_DEFAULT_RLP_STATISTICS_STRU;

/*
 * 协议表格:
 * ASN.1描述:
 * 结构说明: Default RLP的统计信息
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_RLP_STATISTICS_IND
 */
typedef struct {
    VOS_UINT16                                     usPrimId;
    VOS_UINT16                                     usToolId;
    CTTF_HRPD_MNTN_PER_DEFAULT_RLP_STATISTICS_STRU astRlpEntity[CTTF_HRPD_MNTN_DEFAULT_RLP_MAX_NUM];
} CTTF_OM_HRPD_MNTN_DEFAULT_RLP_STATISTICS_STRU;

/*
 * 协议表格:
 * ASN.1描述:
 * 结构说明: MFPA板侧上报的发送方向的统计
 */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh;  /* 发送的总字节数（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* 发送的总字节数（低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* （低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* （低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTxNakRequestCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulIgnoredAnNakCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRlpFlushCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_MF_RLP_REV_STATISTICS_STRU;

/* 结构说明: MFPA板侧上报的接收方向的统计 */
typedef struct {
    VOS_UINT32 ulTotalByteCntHigh;  /* 接收的总字节数（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* （低32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulDuplicateByteCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRxedNakRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_MF_RLP_FWD_STATISTICS_STRU;

/* 结构说明: MFPA每个RLP实体的统计信息 */
typedef struct {
    VOS_UINT8                                     ucRlpFlowId;    /* 区分RLP实体的唯一编号, 无效值: 0xFF */
    VOS_UINT8                                     aucReserved[3]; /* 保留字节，按照无效值填写，值为0 */
    CTTF_HRPD_MNTN_PER_MF_RLP_REV_STATISTICS_STRU stRevInfo;
    CTTF_HRPD_MNTN_PER_MF_RLP_FWD_STATISTICS_STRU stFwdInfo;
} CTTF_HRPD_MNTN_PER_MF_RLP_STATISTICS_STRU;

/*
 * 结构说明: MFPA的可维可测信息
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_MFPA_RLP_STATISTICS_IND
 */
typedef struct {
    VOS_UINT16                                usPrimId;
    VOS_UINT16                                usToolId;
    CTTF_HRPD_MNTN_PER_MF_RLP_STATISTICS_STRU astRlpEntity[CTTF_HRPD_MNTN_MF_RLP_MAX_NUM];
} CTTF_OM_HRPD_MNTN_MF_RLP_STATISTICS_STRU;

/* 结构说明: EMPA板侧上报的发送方向的统计 */
typedef struct {
    VOS_UINT32 ulSelectRouteACnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulSelectRouteBCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCntHigh;  /* 发送的总字节数（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* （高32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* （高32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTxNakReportCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulIgnoreAnNakCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRlpFlushCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_EMF_RLP_REV_STATISTICS_STRU;

/* 结构说明: EMPA板侧上报的发送方向的统计 */
typedef struct {
    VOS_UINT32 ulSelectRouteACnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulSelectRouteBCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCntHigh;  /* 接收的总字节数（高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalByteCnt;      /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCntHigh;    /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewByteCnt;        /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCntHigh;   /* （高32bit）无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxByteCnt;       /* （低32bit） 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulDuplicateByteCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalPacketCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNewPacketCnt;      /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRetxPacketCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRxNakReportCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulNakAbortCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAnResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulAtResetRequestCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulRxedAnResetAckCnt; /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_MNTN_PER_EMF_RLP_FWD_STATISTICS_STRU;

/* 结构说明: EMPA 每个RLP实体的可维可测信息 */
typedef struct {
    VOS_UINT8                                   ucLinkFlowId;    /* 区分RLP实体的唯一编号, 无效值: 0xFF */
    CTTF_HRPD_PA_MNTN_FLOW_PROTOCOL_ENUM_UINT8  enFlowProtocol;  /* 不支持FlowProtocol时，按照无效值上报，值为0xFF */
    CTTF_HRPD_PA_MNTN_ROUTE_PROTOCOL_ENUM_UINT8 enRouteProtocol; /* 不支持RouteProtocol时，按照无效值上报，值为0xFF */
    VOS_UINT8                                   aucReserved[1];  /* 保留字节，按照无效值填写，值为0 */
    CTTF_HRPD_MNTN_PER_EMF_RLP_REV_STATISTICS_STRU stRevInfo;
    CTTF_HRPD_MNTN_PER_EMF_RLP_FWD_STATISTICS_STRU stFwdInfo;
} CTTF_HRPD_MNTN_PER_EMF_RLP_STATISTICS_STRU;

/*
 * 结构说明: EMPA的可维可测信息
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_EMPA_RLP_STATISTICS_IND
 */
typedef struct {
    VOS_UINT16                                 usPrimId;
    VOS_UINT16                                 usToolId;
    CTTF_HRPD_MNTN_PER_EMF_RLP_STATISTICS_STRU astRlpEntity[CTTF_HRPD_MNTN_EMF_RLP_MAX_NUM];
} CTTF_OM_HRPD_MNTN_EMF_RLP_STATISTICS_STRU;

/* 结构说明: Acmac可维可测信息统计 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;                  /* C.H1001中subprotocol的值，无效值0xFF */
    VOS_UINT8                              aucReserve[3];                  /* 保留字节，按照无效值填写，值为0xFF */
    VOS_UINT32                             ulSuccAttemptCnt;               /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulMaxProbeFailCnt;              /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulAdjacentProbeTimerExpiredCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulMailboxFailCnt;               /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulCprocFailCnt;                 /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulSndErrorIndCnt;               /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe1Cnt;     /* Access Probe 1成功次数, 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe2Cnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe3Cnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe4Cnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbe5Cnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulAccessProbeAboveCnt; /* 多于Access Probe 5才成功次数, 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;     /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt;    /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_ACMAC_MNTN_STATISTIC_STRU;

/* 结构说明: Ccmac可维可测信息统计 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;      /* C.H1001中subprotocol的值，无效值0xFF */
    VOS_UINT8                              aucReserve[3];      /* 保留字节，按照无效值填写，值为0xFF */
    VOS_UINT32                             ulPhyPkt128BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulMacLayerPktCnt;   /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_CCMAC_MNTN_STATISTIC_STRU;

/* 结构说明: RTCMAC可维可测信息统计 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;       /* C.H1001中subprotocol的值，无效值0xFF */
    CTTF_HRPD_RTCMAC_MNTN_RATE_ENUM_UINT8  enRateLimit;         /* 无效值0xFF  */
    VOS_UINT8                              aucReserve[2];       /* 保留字节，按照无效值填写，值为0xFF */
    VOS_UINT32                             ulPhyPkt128BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt768BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1536BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt2048BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt3072BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt4096BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt6144BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt8192BitCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt12288BitCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulSndErrorIndCnt;    /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_RTCMAC_MNTN_STATISTIC_STRU;

/* 结构说明: FTCMAC可维可测信息统计 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol;        /* C.H1001中subprotocol的值，无效值0xFF */
    VOS_UINT8                              aucReserve[3];        /* 保留字节，按照无效值填写，值为0xFF */
    VOS_UINT32                             ulPhyPkt128BitCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt256BitCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt512BitCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt1024BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt2048BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt3072BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt4096BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulPhyPkt5120BitCnt;   /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulTotPhyPktCnt;       /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulTotMacLayPktCnt;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulTotSingUserPktCnt;  /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulTotMultiUserPktCnt; /* 无效值: 0xFFFFFFFF */
    VOS_UINT32                             ulMaxSecLayPktNum;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotalMacPacketBytes;       /* MAC Packet Byte Size,不包含2bits MAC trailer；无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotMyAtMacPayloadBytes;    /* 无效值: 0xFFFFFFFF */
    VOS_UINT32 ulTotOtherAtMacPayloadBytes; /* 无效值: 0xFFFFFFFF */
} CTTF_HRPD_FTCMAC_MNTN_STATISTIC_STRU;

/*
 * 结构说明: HRPD MAC 信息统计上报
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_MAC_PROTOCOL_STATISTICS_IND
 * 说明: 主动上报消息, 在API调用时用指针指向上报内容即可
 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;

    CTTF_HRPD_ACMAC_MNTN_STATISTIC_STRU  stAcMac;
    CTTF_HRPD_CCMAC_MNTN_STATISTIC_STRU  stCcMac;
    CTTF_HRPD_RTCMAC_MNTN_STATISTIC_STRU stRtcMac;
    CTTF_HRPD_FTCMAC_MNTN_STATISTIC_STRU stFtcMac;
} CTTF_OM_HRPD_MAC_MNTN_STATISTIC_STRU;

/* Description: DCC和ECC协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_CC_STATE_ENUM_UINT8     enState;       /* 无效值: BUTT */
    VOS_UINT8                              aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_CC_MAC_PROTOCL_STATE_STRU;

/* Description: DAC和EAC协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_AC_STATE_ENUM_UINT8     enState;       /* 无效值: BUTT */
    VOS_UINT8                              aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_AC_MAC_PROTOCL_STATE_STRU;

/* Description: DFTC和EFTC协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_FTC_STATE_ENUM_UINT8    enState;       /* 无效值: BUTT */
    VOS_UINT8                              aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_FTC_MAC_PROTOCL_STATE_STRU;

/* Description: DRTC和ST3 RTC协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8 enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_RTC_STATE_ENUM_UINT8    enState;       /* 无效值: BUTT */
    VOS_UINT8                              aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_RTC_MAC_PROTOCL_STATE_STRU;

/* Description: 鉴权协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                  enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8 enState;       /* 无效值: BUTT */
    VOS_UINT8                                               aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_AUTHENTICATION_PROTOCL_STATE_STRU;

/*
 * Name: CTTF_HRPD_MNTN_MODULE_ENCRYPTION_PROTOCL_STATE_STRU
 * Description: 加密协议状态结构
 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                  enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8 enState;       /* 无效值: BUTT */
    VOS_UINT8                                               aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_ENCRYPTION_PROTOCL_STATE_STRU;

/* Description: 安全协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                  enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_SECURITY_LAYER_PROTOCOL_STATE_ENUM_UINT8 enState;       /* 无效值: BUTT */
    VOS_UINT8                                               aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_SECURITY_PROTOCL_STATE_STRU;

/* Description: 流控协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8                    enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_STREAM_FLOW_CTRL_PROTOCOL_STATE_ENUM_UINT8 enState;       /* 无效值: BUTT */
    VOS_UINT8                                                 aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_STREAM_FLOW_CTRL_PROTOCL_STATE_STRU;

/* Description: 流应用协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8              enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_STREAM_APP_PROTOCOL_STATE_ENUM_UINT8 enState;       /* 无效值: BUTT */
    VOS_UINT8                                           aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_STREAM_APP_PROTOCL_STATE_STRU;

/* Description: SNP协议状态结构 */
typedef struct {
    CTTF_HRPD_MNTN_SUB_PROTOCOL_ENUM_UINT8    enSubProtocol; /* 无效值: BUTT */
    CTTF_HRPD_MNTN_SNP_UP_TX_STATE_ENUM_UINT8 enState;       /* 无效值: BUTT */
    VOS_UINT8                                 aucReserve[2]; /* 保留字节，按照无效值填写，值为0 */
} CTTF_HRPD_MNTN_MODULE_SNP_UP_TX_STATE_STRU;

/*
 * Description: 模块协议状态结构
 * 对应消息: ID_CTTF_OM_HRPD_MNTN_MODULE_PROTOCOL_STATE_IND
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

/* 结构说明: RTC维测上报的ACK信息 */
typedef struct {
    VOS_UINT32 aulACKsNum[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* ulInterlace[interlacenumber]中数字之和,对应不同传输模式 */
    VOS_UINT32 aulPktNumOnInterlace[CTTF_HRPD_MNTN_RTC3MAC_INTERLACE_MAX_NUM][CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* 对应交织号收到的ACK个数，是累加值。对应不同的传输模式 */
} CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STRU;

/* 结构说明: RTC Reverse Link Metrics 统计信息 */
typedef struct {
    VOS_UINT32 ulGoodPacketNumSum; /* ulGoodPacketNum[CTTF_HRPD_RTC3MAC_TRANS_MODE_BUTT]求和 */
    VOS_UINT32 aulGoodPacketNum[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* 总发送成功的Packet个数，值包含ACK的个数 */
    VOS_UINT32 ulAvgPacketTerminationSum; /* ulAvgPacketTermination[CTTF_HRPD_RTC3MAC_TRANS_MODE_BUTT]求和 */
    VOS_UINT32 aulAvgPacketTermination[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* 以子帧为单位，求Packet平均发送的子帧数 */
    VOS_UINT32 ulTotlPktTxTimeSum; /* ulTotlPktTxTime[CTTF_HRPD_RTC3MAC_TRANS_MODE_BUTT]求和 */
    VOS_UINT32 aulTotlPktTxTime[CTTF_HRPD_MAC_TRANS_MODE_BUTT]; /* 数据包总的发送时间，以ms为单位 */
} CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STATIS_STRU;

/* 结构说明: RTC Reverse Link Metrics信息 */
typedef struct {
    VOS_UINT16 usPrimId;
    VOS_UINT16 usToolId;
    /* 每种PS类型对应的ACK info, PS0对应的位置不会用到，将该位置用来存放"所有PS类型的ACK info之和" */
    CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STRU astRtc3MacAckInfo[CTTF_HRPD_MNTN_RTC3MAC_PS_BUTT][CSDR_HARQ_MNTN_SUBPACKET_BUTT];
    /* 每种PS类型对应的ACK统计信息,PS0对应的位置不会用到，将该位置用来存放"所有PS类型对应的ACK统计信息之和" */
    CTTF_HRPD_MNTN_RTC3MAC_ACK_INFO_STATIS_STRU astRtc3MacAckInfoStatis[CTTF_HRPD_MNTN_RTC3MAC_PS_BUTT]; 
    VOS_UINT32 ulTotalParqNAKsNum;   /* 收到PARQ NAK的个数，累加值。不区分传输模式也不区分PS类型 */
    VOS_UINT32 ulTotalPacketSentNum; /* 总发送的Packet个数，包括ACK和PARQ中的NAK。不区分传输模式也不区分PS类型 */
    VOS_UINT32 ulTotalPer;           /* 表示中的误包率，改数值已经放大100倍，显示的时候以%的形式显示 */
} CTTF_OM_HRPD_MNTN_RTC3MAC_ACK_INFO_REPORT_STRU;


#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

