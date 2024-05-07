

#ifndef __OAM_EXT_IF_H__
#define __OAM_EXT_IF_H__

/* ����ͷ�ļ����� */
#include "platform_spec.h"
#include "oal_types.h"
#include "oal_net.h"
#include "oam_log.h"
#include "oam_wdk.h"
#include "oam_config.h"
#include "chr_user.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAM_EXT_IF_H

/* ��־�궨�� */
/* �ýӿ���Ҫ�ϸ���Ƶ��ã���Ҫ�����ж��ϰ벿��OAM OAL��ģ��Ƶ���������쳣��֧ */
#define oam_wifi_log_para_press(vap_id, feature_id, fileid, lev) \
    (((fileid) & 0xFFFF) | (((feature_id) & 0xFF) << 16) | (((vap_id) & 0xF) << 24) | (((lev) & 0xF) << 28))

#ifdef _PRE_WLAN_DFT_LOG

#define oam_io_printk(fmt, ...) \
    oam_log_console_printk(THIS_FILE_ID, (uint16_t)__LINE__, OAL_FUNC_NAME, fmt, ##__VA_ARGS__)

#define oam_info_log0(_uc_vap_id, _en_feature_id, fmt)                                                        \
    oam_log_print0(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_INFO), \
        (uint16_t)__LINE__, fmt)
#define oam_info_log1(_uc_vap_id, _en_feature_id, fmt, p1)                                                    \
    oam_log_print1(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_INFO), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1))
#define oam_info_log2(_uc_vap_id, _en_feature_id, fmt, p1, p2)                                                \
    oam_log_print2(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_INFO), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2))
#define oam_info_log3(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3)                                            \
    oam_log_print3(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_INFO), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2), (oal_uint)(p3))
#define oam_info_log4(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3, p4)                                        \
    oam_log_print4(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_INFO), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2), (oal_uint)(p3), (oal_uint)(p4))

#define oam_warning_log0(_uc_vap_id, _en_feature_id, fmt)                                                        \
    oam_log_print0(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_WARNING), \
        (uint16_t)__LINE__, fmt)
#define oam_warning_log1(_uc_vap_id, _en_feature_id, fmt, p1)                                                    \
    oam_log_print1(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_WARNING), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1))
#define oam_warning_log2(_uc_vap_id, _en_feature_id, fmt, p1, p2)                                                \
    oam_log_print2(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_WARNING), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2))
#define oam_warning_log3(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3)                                            \
    oam_log_print3(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_WARNING), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2), (oal_uint)(p3))
#define oam_warning_log4(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3, p4)                                        \
    oam_log_print4(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_WARNING), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2), (oal_uint)(p3), (oal_uint)(p4))

#define oam_error_log0(_uc_vap_id, _en_feature_id, fmt)                                                        \
    oam_log_print0(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_ERROR), \
        (uint16_t)__LINE__, fmt)
#define oam_error_log1(_uc_vap_id, _en_feature_id, fmt, p1)                                                    \
    oam_log_print1(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_ERROR), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1))
#define oam_error_log2(_uc_vap_id, _en_feature_id, fmt, p1, p2)                                                \
    oam_log_print2(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_ERROR), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2))
#define oam_error_log3(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3)                                            \
    oam_log_print3(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_ERROR), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2), (oal_uint)(p3))
#define oam_error_log4(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3, p4)                                        \
    oam_log_print4(oam_wifi_log_para_press(_uc_vap_id, _en_feature_id, THIS_FILE_ID, OAM_LOG_LEVEL_ERROR), \
        (uint16_t)__LINE__, fmt, (oal_uint)(p1), (oal_uint)(p2), (oal_uint)(p3), (oal_uint)(p4))

#else
#define oam_excp_record(_uc_vap_id, _excp_id)
#define oam_io_printk(fmt, ...)
#define oam_info_log0(_uc_vap_id, _en_feature_id, fmt)
#define oam_info_log1(_uc_vap_id, _en_feature_id, fmt, p1)
#define oam_info_log2(_uc_vap_id, _en_feature_id, fmt, p1, p2)
#define oam_info_log3(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3)
#define oam_info_log4(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3, p4)
#define oam_warning_log0(_uc_vap_id, _en_feature_id, fmt)
#define oam_warning_log1(_uc_vap_id, _en_feature_id, fmt, p1)
#define oam_warning_log2(_uc_vap_id, _en_feature_id, fmt, p1, p2)
#define oam_warning_log3(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3)
#define oam_warning_log4(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3, p4)
#define oam_error_log0(_uc_vap_id, _en_feature_id, fmt)
#define oam_error_log1(_uc_vap_id, _en_feature_id, fmt, p1)
#define oam_error_log2(_uc_vap_id, _en_feature_id, fmt, p1, p2)
#define oam_error_log3(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3)
#define oam_error_log4(_uc_vap_id, _en_feature_id, fmt, p1, p2, p3, p4)

#endif

#define OAM_DIFF_LOG0 oam_warning_log0
#define OAM_DIFF_LOG1 oam_warning_log1
#define OAM_DIFF_LOG2 oam_warning_log2
#define OAM_DIFF_LOG3 oam_warning_log3
#define OAM_DIFF_LOG4 oam_warning_log4

/* 2.1 TRACE��غ궨�� */
/* ͳ����Ϣ�궨�� */ /*lint -e506*/ /*lint -e774*/
#define oam_stat_dev_incr(_uc_dev_id, _member, _num)

/* Ϊ�˶������±���б���������Ҫ�������鳤��(��4������) */
#define oam_stat_dev_array_incr(_uc_dev_id, _member, _uc_array_index, _uc_array_max_size)

#define oam_stat_dev_update(_uc_dev_id, _member, _uc_q_id, _uc_index, _ul_val)

#define oam_stat_dev_get_val(_uc_dev_id, _member, _pul_val)

#define oam_stat_dev_set_val(_uc_dev_id, _member, _uc_val)

#define oam_stat_mgmt_incr(_uc_vap_id, _uc_mgmt_id, _member)

#define oam_stat_user_incr(_uc_user_id, _member, _num)

#define oam_stat_tid_incr(_uc_user_id, _uc_tid_num, _member, _num)

#define oam_tid_ampdu_stats_incr(_member, _cnt)

#define OAM_PRINT_FORMAT_LENGTH 256 /* ��ӡ��ʽ�ַ�������󳤶� */

/* ��־�ṹ����غ꣬����oam_log_info_stru */
#define OAM_FUNC_NAME_MAX_LEN       48   /* ����������󳤶� */
#define OAM_LOG_INFO_MAX_LEN        100  /* ��־��Ϣ��󳤶� */
#define OAM_LOG_INFO_IN_MEM_MAX_NUM 5000 /* oamģ����ౣ��5000��log��Ϣ�������Ժ��ͷ�Զ����� */
#define OAM_LOG_ENTRY_PARA_NUM      4

#define OAM_SEQ_TRACK_NUM 128

/* device����ĸ����жϴ��������� */
#define OAM_MAC_ERROR_TYPE_MAX_NUM 25
#define OAM_SOC_IRQ_MAX_NUM        5
#define OAM_PMU_BUCK_IRQ_MAX_NUM   5
#define OAM_MAC_SUB_IRQ_MAX_NUM    11

/* �ڴ����Ϣ�������һ����Առ�õĴ�С */
#define OAM_MEMBLOCK_INFO_STRU_LEN 8

/* oam_stats_device_subtype_irq_struǰ������Առ�ÿռ��С */
#define OAM_FIRST_THREE_MEMBER_LEN 4

/* ÿһ���¼��������¼��������ֵ */
#define OAM_MAX_EVENT_NUM_IN_EVENT_QUEUE 8

/* OAMģ������skbʱ��ͷ����β��Ϊsdt�ֱ�Ԥ��8����1���ֽ� */
#define OAM_RESERVE_SKB_HEADROOM_LEN 8
#define OAM_RESERVE_SKB_TAILROOM_LEN 1
#define OAM_RESERVE_SKB_LEN          (OAM_RESERVE_SKB_HEADROOM_LEN + OAM_RESERVE_SKB_TAILROOM_LEN)

/* �ַ�����0��β�ϱ���ʵ���ַ���������󳤶� */
#define OAM_REPORT_MAX_STRING_LEN (WLAN_SDT_NETBUF_MAX_PAYLOAD - 1) /* ��\0���� */

/* ����ά����غ궨�� */
#ifdef _PRE_WLAN_DFT_STAT
#define OAM_PHY_STAT_NODE_ENABLED_MAX_NUM 4
#define OAM_PHY_STAT_RX_PPDU_CNT          8
#define OAM_PHY_STAT_ITEM_MIN_IDX         1
#define OAM_PHY_STAT_ITEM_MAX_IDX         16
#define OAM_MACHW_STAT_RX_MEMBER_CNT      18
#define OAM_MACHW_STAT_TX_MEMBER_CNT      8
#define OAM_PSM_STAT_CNT                  10
#define OAM_UAPSD_STAT_CNT                11
#define OAM_TID_STAT_CNT                  14
#endif

#define OAM_OTA_RX_DSCR_TYPE OAM_OTA_TYPE_RX_DSCR_PILOT
#define OAM_OTA_TX_DSCR_TYPE OAM_OTA_TYPE_TX_DSCR_PILOT
#define MAC_ADDR_LEN 6

/* ö�ٶ��� */
/* data type */
typedef enum {
    OAM_DATA_TYPE_LOG = 0x1,
    OAM_DATA_TYPE_OTA = 0x2,
    OAM_DATA_TYPE_TRACE = 0x3,
    OAM_DATA_TYPE_EVENT = 0x4,
    OAM_DATA_TYPE_MEM_RW = 0x9,
    OAM_DATA_TYPE_REG_RW = 0x15,
    OAM_DATA_TYPE_CFG = 0x0b,
    OAM_DATA_TYPE_GVAR_RW = 0x27, /* global value read or write */
    OAM_DATA_TYPE_STRING = 0x28,  /* report string  */

    OAM_DATA_TYPE_DEVICE_LOG = 0x40,
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    OAM_DATA_TYPE_SAMPLE = 0x45,
#endif
    OAM_DATA_TYPE_DPD = 0x55,
    OAM_DATA_TYPE_BUTT
} oam_data_type_enum;
typedef uint8_t oam_data_type_enum_uint8;

/* SDT��������SDT APP�෢�����ݵ����� */
typedef enum {
    OAM_PRIMID_TYPE_PC_TO_DEV_DATA = 1,
    OAM_PRIMID_TYPE_DEV_ACK,
    OAM_PRIMID_TYPE_DEV_ACTIVE_UPLOAD,
    OAM_PRIMID_TYPE_OUTPUT_CONTENT,

    SDT_DRV_PRIMID_TYPE_BUTT
} oam_primid_type_enum;
typedef uint8_t oam_primid_type_enum_uint8;

typedef enum {
    OAM_MODULE_ID_ALL,
    OAM_MODULE_ID_WAL,
    OAM_MODULE_ID_HMAC,
    OAM_MODULE_ID_DMAC,

    OAM_MODULE_ID_HAL,
    OAM_MODULE_ID_OAM,
    OAM_MODULE_ID_OAL,
    OAM_MODULE_ID_FRW,
    OAM_MODULE_ID_ALG,
    OAM_MODULE_ID_PLAT,
    OAM_MODULE_ID_SDT,

    OAM_MODULE_ID_BUTT
} oam_module_id_enum;
typedef uint16_t oam_module_id_enum_uint16;

/* ��־���� */
typedef enum {
    OAM_LOG_LEVEL_ERROR = 1, /* ERROR�����ӡ */
    OAM_LOG_LEVEL_WARNING,   /* WARNING�����ӡ */
    OAM_LOG_LEVEL_INFO,      /* INFO�����ӡ */

    OAM_LOG_LEVEL_BUTT
} oam_log_level_enum;
typedef uint8_t oam_log_level_enum_uint8;

#define OAM_LOG_DEFAULT_LEVEL OAM_LOG_LEVEL_WARNING

/* �����ļ������õ����� */
typedef enum {
    OAM_CFG_TYPE_MAX_ASOC_USER = 0,

    OAM_CFG_TYPE_BUTT
} oam_cfg_type_enum;
typedef uint16_t oam_cfg_type_enum_uint16;

/* ��ӡģʽ���� */
typedef enum {
    OAM_OUTPUT_TYPE_CONSOLE = 0, /* ����̨��� */
    OAM_OUTPUT_TYPE_SDT,         /* �����SDT */

    OAM_OUTPUT_TYPE_BUTT
} oam_output_type_enum;
typedef uint8_t oam_output_type_enum_uint8;

/* event�ϱ��¼���Ϣ���� */
typedef enum {
    OAM_EVENT_WID_QUERY,            /* ����������Ϣ����ѯ���� */
    OAM_EVENT_WID_WRITE,            /* ����������Ϣ��д���� */
    OAM_EVENT_INTERNAL,             /* WiTP�ڲ�����֮����¼���Ϣ */
    OAM_EVENT_VAP_STATE_CHANGE,     /* VAP״̬Ǩ���¼���Ϣ */
    OAM_EVENT_PAUSE_OR_RESUME_TID,  /* ��ͣ���߻ָ�tid */
    OAM_EVENT_PAUSE_OR_RESUME_USER, /* ��ͣ���߻ָ�user��Ϣ */
    OAM_EVENT_VAP_CHANGE_CHANNEL,   /* VAP�л��ŵ� */
    OAM_EVENT_USER_INFO_CHANGE,     /* �û�״̬��Ϣ����������Ϣ�仯 */
    OAM_EVENT_USER_TXRX_PARAM,      /* �û��շ����� */
    OAM_EVENT_REGISTER = 9,         /* �Ĵ��� */

    OAM_EVENT_TYPE_BUTT
} oam_event_type_enum;
typedef uint16_t oam_event_type_enum_uint16;

/* OTA�ϱ��¼����� */
typedef enum {
    /* Ĭ�Ϲر��ϱ� */
    OAM_OTA_TYPE_RX_DSCR,     /* ���������� */
    OAM_OTA_TYPE_TX_DSCR,     /* ���������� */
    OAM_OTA_TYPE_80211_FRAME, /* 80211֡(������beacon) */
    OAM_OTA_TYPE_BEACON,      /* beacon */
    OAM_OTA_TYPE_ETH_FRAME,   /* ��̫��֡ */
    OAM_OTA_TYPE_RX_DMAC_CB,
    OAM_OTA_TYPE_RX_HMAC_CB,
    OAM_OTA_TYPE_TX_CB,
    OAM_OTA_TYPE_TIMER_TRACK, /* ��ʱ������ɾ�����кź��ļ���׷�� ,�ѷ��� */

    /* Ĭ�ϴ��ϱ� */
    OAM_OTA_TYPE_IRQ,         /* �ж���Ϣ */
    OAM_OTA_TYPE_TIMER,       /* ������ʱ����Ϣ */
    OAM_OTA_TYPE_MEMPOOL,     /* ĳһ�ڴ�ؼ������ӳ�ʹ����Ϣ */
    OAM_OTA_TYPE_MEMBLOCK,    /* ĳһ�ڴ�ص������ڴ��ʹ����Ϣ�����ݣ��������������ڴ������� */
    OAM_OTA_TYPE_EVENT_QUEUE, /* ��ǰ���д����¼����¼������е��¼��������¼�ͷ��Ϣ */
    OAM_OTA_TYPE_MPDU_NUM,
    OAM_OTA_TYPE_PHY_STAT,          /* phy�շ���ͳ��ֵ */
    OAM_OTA_TYPE_MACHW_STAT,        /* mac�շ���ͳ��ֵ */
    OAM_OTA_TYPE_MGMT_STAT,         /* ����֡ͳ�� */
    OAM_OTA_TYPE_DBB_ENV_PARAM,     /* �տڻ�����ά����� */
    OAM_OTA_TYPE_USR_QUEUE_STAT,    /* �û�����ͳ����Ϣ */
    OAM_OTA_TYPE_VAP_STAT,          /* vap�������ͳ����Ϣ */
    OAM_OTA_TYPE_USER_THRPUT_PARAM, /* Ӱ���û�ʵʱ������ص�ͳ����Ϣ */
    OAM_OTA_TYPE_AMPDU_STAT,        /* ampduҵ������ͳ����Ϣ */
    OAM_OTA_TYPE_HARDWARE_INFO,     /* hal mac�����Ϣ */
    OAM_OTA_TYPE_USER_QUEUE_INFO,   /* �û�������Ϣ */

    /* VAP��USER�Ƚṹ�����Ϣ���ϱ������ṹ���ڴ� */
    OAM_OTA_TYPE_HMAC_VAP,
    OAM_OTA_TYPE_DMAC_VAP,
    OAM_OTA_TYPE_HMAC_USER,
    OAM_OTA_TYPE_MAC_USER,
    OAM_OTA_TYPE_DMAC_USER,

    /* hmac �� dmac vap�в��ֳ�Ա�Ĵ�С */
    OAM_OTA_TYPE_HMAC_VAP_MEMBER_SIZE,
    OAM_OTA_TYPE_DMAC_VAP_MEMBER_SIZE,

    /* ���ּ����ͳ����Ϣ,�������طŵ�һ��!!! */
    OAM_OTA_TYPE_DEV_STAT_INFO,
    OAM_OTA_TYPE_VAP_STAT_INFO,
    OAM_OTA_TYPE_USER_STAT_INFO,

    OAM_OTA_TYPE_PHY_BANK1_INFO, /* PHY�Ĵ��� BANK1 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK2_INFO, /* PHY�Ĵ��� BANK2 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK3_INFO, /* PHY�Ĵ��� BANK3 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK4_INFO, /* PHY�Ĵ��� BANK4 ��Ϣ */
    /* 03����PHY�Ĵ���5��6 */
    OAM_OTA_TYPE_PHY_BANK5_INFO, /* PHY�Ĵ��� BANK5 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK6_INFO, /* PHY�Ĵ��� BANK6 ��Ϣ */
    OAM_OTA_TYPE_PHY0_CTRL_INFO, /* PHY�Ĵ��� PHY0_CTRL ��Ϣ */
    OAM_OTA_TYPE_PHY1_CTRL_INFO, /* PHY�Ĵ��� PHY1_CTRL ��Ϣ */

    OAM_OTA_TYPE_MAC_BANK0_INFO, /* MAC�Ĵ��� BANK0 ��Ϣ */
    OAM_OTA_TYPE_MAC_BANK1_INFO, /* MAC�Ĵ��� BANK1 ��Ϣ */
    OAM_OTA_TYPE_MAC_BANK2_INFO, /* MAC�Ĵ��� BANK2 ��Ϣ */
    OAM_OTA_TYPE_MAC_BANK3_INFO, /* MAC�Ĵ��� BANK3 ��Ϣ�����ڼ��ֽ���Ϣ���ܶ�ȡ�� */
    OAM_OTA_TYPE_MAC_BANK4_INFO, /* MAC�Ĵ��� BANK4 ��Ϣ */

    OAM_OTA_TYPE_RF_REG_INFO,  /* RF�Ĵ��� */
    OAM_OTA_TYPE_SOC_REG_INFO, /* SOC�Ĵ��� */

    /* 03 MAC�Ĵ����б� */
    OAM_OTA_TYPE_MAC_CTRL0_BANK_INFO, /* MAC�Ĵ��� BANK0 ��Ϣ */
    OAM_OTA_TYPE_MAC_CTRL1_BANK_INFO, /* MAC�Ĵ��� BANK1 ��Ϣ */
    OAM_OTA_TYPE_MAC_CTRL2_BANK_INFO, /* MAC�Ĵ��� BANK2 ��Ϣ */
    OAM_OTA_TYPE_MAC_RD0_BANK_INFO,   /* MAC�Ĵ��� BANK3 ��Ϣ�����ڼ��ֽ���Ϣ���ܶ�ȡ�� */
    OAM_OTA_TYPE_MAC_RD1_BANK_INFO,   /* MAC�Ĵ��� BANK4 ��Ϣ */
    OAM_OTA_TYPE_MAC_LUT0_BANK_INFO,  /* MAC�Ĵ��� BANK5 ��Ϣ */
    OAM_OTA_TYPE_MAC_WLMAC_CTRL_INFO, /* MAC�Ĵ��� wlmac_ctrl ��Ϣ */
    OAM_OTA_TYPE_RF0_REG_INFO,        /* 03 RF0�Ĵ��� */
    OAM_OTA_TYPE_RF1_REG_INFO,        /* 03 RF1�Ĵ��� */

    /* 03 vectorά�������ϱ� */
    OAM_OTA_TYPE_RX_HDR,
    OAM_OTA_TYPE_RX_HDR_WORD0_INFO = OAM_OTA_TYPE_RX_HDR, /* trailer word0��Ϣ */
    OAM_OTA_TYPE_RX_HDR_WORD1_INFO, /* trailer word1��Ϣ */
    OAM_OTA_TYPE_RX_HDR_WORD2_INFO, /* trailer word2��Ϣ */
    OAM_OTA_TYPE_RX_HDR_WORD3_INFO, /* trailer word3��Ϣ */
    OAM_OTA_TYPE_RX_HDR_WORD4_INFO, /* trailer word4��Ϣ */
    OAM_OTA_TYPE_RX_HDR_WORD5_INFO, /* trailer word5��Ϣ */
    OAM_OTA_TYPE_RX_HDR_WORD6_INFO, /* trailer word6��Ϣ */
    OAM_OTA_TYPE_RX_HDR_WORD7_INFO, /* trailer word7��Ϣ */

    /* 03 trailerά�������ϱ� */
    OAM_OTA_TYPE_RX_TRLR,
    OAM_OTA_TYPE_RX_TRLR_WORD0_INFO = OAM_OTA_TYPE_RX_TRLR,  /* trailer word0��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD1_INFO,  /* trailer word1��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD2_INFO,  /* trailer word2��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD3_INFO,  /* trailer word3��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD4_INFO,  /* trailer word4��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD5_INFO,  /* trailer word5��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD6_INFO,  /* trailer word6��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD7_INFO,  /* trailer word7��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD8_INFO,  /* trailer word8��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD9_INFO,  /* trailer word9��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD10_INFO, /* trailer word10��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD11_INFO, /* trailer word11��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD12_INFO, /* trailer word12��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD13_INFO, /* trailer word13��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD14_INFO, /* trailer word14��Ϣ */
    OAM_OTA_TYPE_RX_TRLR_WORD15_INFO, /* trailer word15��Ϣ */

    /* 03����PHY�Ĵ���ABB_CALI_WL_CTRL */
    OAM_OTA_TYPE_ABB_CALI_WL_CTRL0_INFO, /* PHY�Ĵ��� ABB_CALI_WL_CTRL0 ��Ϣ */
    OAM_OTA_TYPE_ABB_CALI_WL_CTRL1_INFO, /* PHY�Ĵ��� ABB_CALI_WL_CTRL1 ��Ϣ */

    /* 03 SOCά�������ϱ� */
    OAM_OTA_TYPE_SOC_RF_W_C0_CTL_INFO, /* SOC�Ĵ��� RF_W_C0_CTL ��Ϣ */
    OAM_OTA_TYPE_SOC_RF_W_C1_CTL_INFO, /* SOC�Ĵ��� RF_W_C1_CTL ��Ϣ */
    OAM_OTA_TYPE_SOC_W_CTL_INFO,       /* SOC�Ĵ��� W_CTL ��Ϣ */
    OAM_OTA_TYPE_SOC_COEX_CTL_INFO,    /* SOC�Ĵ��� COEX_CTL ��Ϣ */
    OAM_OTA_TYPE_SOC_RESERVE1_INFO,    /* SOC�Ĵ��� Reserve1 ��Ϣ */
    OAM_OTA_TYPE_SOC_RESERVE2_INFO,    /* SOC�Ĵ��� Reserve2 ��Ϣ */
    OAM_OTA_TYPE_SOC_RESERVE3_INFO,    /* SOC�Ĵ��� Reserve3 ��Ϣ */
    OAM_OTA_TYPE_SOC_RESERVE4_INFO,    /* SOC�Ĵ��� Reserve4 ��Ϣ */

    /* 03PILOT�Ĵ��� */
    OAM_OTA_TYPE_PHY_BANK1_PILOT_INFO, /* PHY�Ĵ��� BANK1 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK2_PILOT_INFO, /* PHY�Ĵ��� BANK2 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK3_PILOT_INFO, /* PHY�Ĵ��� BANK3 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK4_PILOT_INFO, /* PHY�Ĵ��� BANK4 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK5_PILOT_INFO, /* PHY�Ĵ��� BANK5 ��Ϣ */
    OAM_OTA_TYPE_PHY_BANK6_PILOT_INFO, /* PHY�Ĵ��� BANK6 ��Ϣ */
    OAM_OTA_TYPE_PHY0_CTRL_PILOT_INFO, /* PHY�Ĵ��� PHY0_CTRL ��Ϣ */
    OAM_OTA_TYPE_PHY1_CTRL_PILOT_INFO, /* PHY�Ĵ��� PHY1_CTRL ��Ϣ */

    OAM_OTA_TYPE_MAC_CTRL0_BANK_PILOT_INFO, /* MAC�Ĵ��� BANK0 ��Ϣ */
    OAM_OTA_TYPE_MAC_CTRL1_BANK_PILOT_INFO, /* MAC�Ĵ��� BANK1 ��Ϣ */
    OAM_OTA_TYPE_MAC_CTRL2_BANK_PILOT_INFO, /* MAC�Ĵ��� BANK2 ��Ϣ */
    OAM_OTA_TYPE_MAC_CTRL3_BANK_INFO,       /* MAC�Ĵ��� BANK3 ��Ϣ ���ڼ��ֽ���Ϣ���ܶ�ȡ */
    OAM_OTA_TYPE_MAC_RD0_BANK_PILOT_INFO,   /* MAC�Ĵ��� BANK3 ��Ϣ�����ڼ��ֽ���Ϣ���ܶ�ȡ�� */
    OAM_OTA_TYPE_MAC_RD1_BANK_PILOT_INFO,   /* MAC�Ĵ��� BANK4 ��Ϣ */
    OAM_OTA_TYPE_MAC_LUT0_BANK_PILOT_INFO,  /* MAC�Ĵ��� BANK5 ��Ϣ */
    OAM_OTA_TYPE_MAC_WLMAC_CTRL_PILOT_INFO, /* MAC�Ĵ��� wlmac_ctrl ��Ϣ */

    OAM_OTA_TYPE_ABB_CALI_WL_CTRL0_PILOT_INFO, /* PHY�Ĵ��� ABB_CALI_WL_CTRL0 ��Ϣ */
    OAM_OTA_TYPE_ABB_CALI_WL_CTRL1_PILOT_INFO, /* PHY�Ĵ��� ABB_CALI_WL_CTRL1 ��Ϣ */

    OAM_OTA_TYPE_SOC_RF_W_C0_CTL_PILOT_INFO, /* SOC�Ĵ��� RF_W_C0_CTL ��Ϣ */
    OAM_OTA_TYPE_SOC_RF_W_C1_CTL_PILOT_INFO, /* SOC�Ĵ��� RF_W_C1_CTL ��Ϣ */
    OAM_OTA_TYPE_SOC_W_CTL_PILOT_INFO,       /* SOC�Ĵ��� W_CTL ��Ϣ */
    OAM_OTA_TYPE_SOC_COEX_CTL_PILOT_INFO,    /* SOC�Ĵ��� COEX_CTL ��Ϣ */

    OAM_OTA_TYPE_RX_DSCR_PILOT, /* ���������� */
    OAM_OTA_TYPE_TX_DSCR_PILOT, /* ���������� */

    /* 06 MSDU������ */
    OAM_OTA_TYPE_RX_MSDU_DSCR,                      /* ����MPDU������ */
    OAM_OTA_TYPE_RX_PPDU_DSCR,                      /* ����PPDU������ */
    OAM_OTA_TYPE_TX_PPDU_DSCR,                      /* ����PPDU������ */
    OAM_OTA_TYPE_TX_COMPLETE_PPDU_DSCR,             /* �������PPDU������ */
    OAM_OTA_TYPE_ALG_TX1_TYPE,                      /* ���㷨Ԥ���ϱ��ӿ� */
    OAM_OTA_TYPE_ALG_RX1_TYPE,                      /* ���㷨Ԥ���ϱ��ӿ� */

    OAM_OTA_TYPE_BA_INFO,                           /* ���ͺ�BA_INFO   */
    OAM_OTA_TYPE_TX_RING_INFO,
    OAM_OTA_TYPE_RX_RING_INFO,

    /* 06 PHY�Ĵ��� */
    OAM_OTA_TYPE_PHY_TIME_CRG_CTL_RB_INFO,         /* PHY�Ĵ��� ʱ��CRG */
    OAM_OTA_TYPE_PHY_PHY_GLB_REG_BANK_INFO,        /* PHY�Ĵ��� ȫ�ֿ��� */
    OAM_OTA_TYPE_PHY_CALI_TEST_REG_BANK_0_INFO,    /* PHY�Ĵ��� RFУ׼BANK0 */
    OAM_OTA_TYPE_PHY_CALI_TEST_REG_BANK_1_INFO_0,  /* PHY�Ĵ��� RFУ׼  BANK1 */
    OAM_OTA_TYPE_PHY_CALI_TEST_REG_BANK_1_INFO_1,  /* PHY�Ĵ��� RFУ׼  BANK1 */
    OAM_OTA_TYPE_PHY_RX_TIME_DOMAIN_REG_BANK_INFO, /* PHY�Ĵ��� ��AGC���RX����ʱ���� */
    OAM_OTA_TYPE_PHY_RX_AGC_REG_BANK_INFO,         /* PHY�Ĵ��� RX_AGC  */
    OAM_OTA_TYPE_PHY_TX_DFE_REG_BANK_INFO,         /* PHY�Ĵ��� TX����ʱ����  */
    OAM_OTA_TYPE_PHY_DPD_REG_BANK_INFO_0,          /* PHY�Ĵ��� DPD */
    OAM_OTA_TYPE_PHY_DPD_REG_BANK_INFO_1,          /* PHY�Ĵ��� DPD  */
    OAM_OTA_TYPE_PHY_DFX_REG_BANK_INFO,            /* PHY�Ĵ��� ά�����á��Ĵ����ϱ�  */
    OAM_OTA_TYPE_PHY_RF_CTRL_REG_BANK_0_INFO_0,    /* PHY�Ĵ��� RF/ABB��ؽӿ�����  */
    OAM_OTA_TYPE_PHY_RF_CTRL_REG_BANK_0_INFO_1,    /* PHY�Ĵ��� RF/ABB��ؽӿ�����  */
    OAM_OTA_TYPE_PHY_RF_CTRL_REG_BANK_1_INFO_0,    /* PHY�Ĵ��� RF/ABB��ؽӿ�����2ch��3ch  */
    OAM_OTA_TYPE_PHY_RF_CTRL_REG_BANK_1_INFO_1,    /* PHY�Ĵ��� RF/ABB��ؽӿ�����2ch��3ch  */
    OAM_OTA_TYPE_PHY_TPC_REG_BANK_INFO,            /* PHY�Ĵ��� TPC��λ����  */
    OAM_OTA_TYPE_PHY_DOTB_REG_BANK_INFO,           /* PHY�Ĵ��� 11B�������  */
    OAM_OTA_TYPE_PHY_RX_FREQ_DOMAIN_REG_BANK_INFO, /* PHY�Ĵ��� RX����Ƶ����  */
    OAM_OTA_TYPE_PHY_RX_CHN_DEC_IRC_REG_BANK_INFO,
    OAM_OTA_TYPE_PHY_RX_CHN_DEC_REG_BANK_INFO_0,   /* PHY�Ĵ��� ������ */
    OAM_OTA_TYPE_PHY_RX_CHN_DEC_REG_BANK_INFO_1,   /* PHY�Ĵ��� ������ */
    OAM_OTA_TYPE_PHY_RX_MIMO_DET_REG_BANK_INFO_0,  /* PHY�Ĵ��� ��� */
    OAM_OTA_TYPE_PHY_RX_MIMO_DET_REG_BANK_INFO_1,  /* PHY�Ĵ��� ��� */
    OAM_OTA_TYPE_PHY_RX_CHN_EST_REG_BANK_INFO,
    OAM_OTA_TYPE_PHY_TX_FREQ_REG_BANK_INFO_0,      /* PHY�Ĵ��� TX����Ƶ���� */
    OAM_OTA_TYPE_PHY_TX_FREQ_REG_BANK_INFO_1,      /* PHY�Ĵ��� TX����Ƶ���� */
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_0_0_INFO,     /* ABB_CALI 0ͨ�� */
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_0_1_INFO,
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_1_0_INFO,     /* ABB_CALI 1ͨ�� */
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_1_1_INFO,
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_2_0_INFO,     /* ABB_CALI 2ͨ�� */
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_2_1_INFO,
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_3_0_INFO,     /* ABB_CALI 3ͨ�� */
    OAM_OTA_TYPE_PHY_ABB_CALI_REG_BANK_3_1_INFO,
    OAM_OTA_TYPE_PHY_ABB_CALI_CRG_REG_BANK_INFO,   /* ABB_CALI CRG */

    /* 06 MAC�Ĵ��� */
    OAM_OTA_TYPE_MAC_CFG_HOST,                     /* MAC�Ĵ��� HOST���üĴ��� */
    OAM_OTA_TYPE_MAC_CFG_DMAC0,                    /* MAC�Ĵ��� DMAC���üĴ���0 */
    OAM_OTA_TYPE_MAC_CFG_DMAC1,                    /* MAC�Ĵ��� DMAC���üĴ���1 */
    OAM_OTA_TYPE_MAC_CFG_DMAC2,                    /* MAC�Ĵ��� DMAC���üĴ���2 */
    OAM_OTA_TYPE_MAC_CFG_DMAC3,                    /* MAC�Ĵ��� DMAC���üĴ���3 */
    OAM_OTA_TYPE_MAC_CFG_DMAC4,                    /* MAC�Ĵ��� DMAC���üĴ���4 */
    OAM_OTA_TYPE_MAC_CFG_DMAC5,                    /* MAC�Ĵ��� DMAC���üĴ���5 */
    OAM_OTA_TYPE_MAC_CFG_SMAC,                     /* MAC�Ĵ��� SMAC���üĴ��� */
    OAM_OTA_TYPE_MAC_RPT_HOST,                     /* MAC�Ĵ��� HOSTͳ�ƼĴ��� */
    OAM_OTA_TYPE_MAC_RPT_DMAC,                     /* MAC�Ĵ��� DMACͳ�ƼĴ��� */
    OAM_OTA_TYPE_MAC_PRT_SMAC,                     /* MAC�Ĵ��� SMACͳ�ƼĴ��� */

    /* 06 RF�Ĵ��� */
    OAM_OTA_TYPE_RF0_REG_INFO_0,                   /* 06 RF0 �Ĵ��� */
    OAM_OTA_TYPE_RF0_REG_INFO_1,                   /* 06 RF0 �Ĵ��� */
    OAM_OTA_TYPE_RF0_PLL_REG_INFO,                 /* 06 RF0 �Ĵ��� PLL */
    OAM_OTA_TYPE_RF0_ABB_REG_INFO,
    OAM_OTA_TYPE_RF1_REG_INFO_0,                   /* 06 RF1 �Ĵ��� */
    OAM_OTA_TYPE_RF1_REG_INFO_1,                   /* 06 RF1 �Ĵ��� */
    OAM_OTA_TYPE_RF1_PLL_REG_INFO,                 /* 06 RF1 �Ĵ��� PLL */
    OAM_OTA_TYPE_RF1_ABB_REG_INFO,
    OAM_OTA_TYPE_RF2_REG_INFO_0,                   /* 06 RF2 �Ĵ��� */
    OAM_OTA_TYPE_RF2_REG_INFO_1,                   /* 06 RF2 �Ĵ��� */
    OAM_OTA_TYPE_RF2_PLL_REG_INFO,                 /* 06 RF2 �Ĵ��� PLL */
    OAM_OTA_TYPE_RF2_ABB_REG_INFO,
    OAM_OTA_TYPE_RF3_REG_INFO_0,                   /* 06 RF3 �Ĵ��� */
    OAM_OTA_TYPE_RF3_REG_INFO_1,                   /* 06 RF3 �Ĵ��� */
    OAM_OTA_TYPE_RF3_PLL_REG_INFO,                 /* 06 RF3 �Ĵ��� PLL */
    OAM_OTA_TYPE_RF3_ABB_REG_INFO,

    /* 06 SOC�Ĵ��� */
    OAM_OTA_TYPE_SOC_GLB_CTL_0,
    OAM_OTA_TYPE_SOC_GLB_CTL_1,
    OAM_OTA_TYPE_SOC_PMU_CMU_CTL,
    OAM_OTA_TYPE_SOC_PMU2_TS_EF,
    OAM_OTA_TYPE_SOC_W_CTL,
    OAM_OTA_TYPE_SOC_WCPU_CTL,
    OAM_OTA_TYPE_SOC_WL_RF_ABB_C0,
    OAM_OTA_TYPE_SOC_WL_RF_ABB_C1,
    OAM_OTA_TYPE_SOC_WL_RF_ABB_C2,
    OAM_OTA_TYPE_SOC_WL_RF_ABB_C3,
    OAM_OTA_TYPE_SOC_WL_RF_ABB_COM,
    OAM_OTA_TYPE_SOC_PCIE_CTRL,
    OAM_OTA_TYPE_SOC_HOST_CTRL_0,
    OAM_OTA_TYPE_SOC_HOST_CTRL_1,
    OAM_OTA_TYPE_SOC_COEX_CTRL,

    OAM_OTA_TYPE_BUTT
} oam_ota_type_enum;
typedef uint8_t oam_ota_type_enum_uint8;

/* ���ְ汾���޸���Ҫ��sdt����ͬ�� */
typedef enum {
    OAM_OTA_TYPE_CHIP_INVALID = 0,
    OAM_OTA_TYPE_1151_DEV = 1,
    OAM_OTA_TYPE_1102_HOST,
    OAM_OTA_TYPE_1151_HOST,
    OAM_OTA_TYPE_1102_DEV,
    OAM_OTA_TYPE_1103_HOST = 5,
    OAM_OTA_TYPE_1103_DEV,
    OAM_OTA_TYPE_1102A_HOST,
    OAM_OTA_TYPE_1102A_DEV,
    OAM_OTA_TYPE_1105_DEV = 11,
    OAM_OTA_TYPE_1105_HOST,
    OAM_OTA_TYPE_1106_DEV = 13,
    OAM_OTA_TYPE_1106_HOST = 14,
    OAM_OTA_TYPE_BISHENG_DEV = 17, /* ��SDT����һһ��Ӧ��15/16Ϊ1153��bisheng 17/18 */
    OAM_OTA_TYPE_BISHENG_HOST = 18,
    OAM_OTA_TYPE_SUZHE_DEV = 19,
    OAM_OTA_TYPE_SUZHE_HOST = 20,
    OAM_OTA_TYPE_1161_DEV = 21,
    OAM_OTA_TYPE_1161_HOST = 22,
} oam_ota_board_type_enum;

typedef uint8_t oam_ota_board_type_enum_uint8;

/* ��ʱ��׷������ */
typedef enum {
    OAM_TIMER_TRACK_TYPE_CREATE = 0,
    OAM_TIMER_TRACK_TYPE_DESTROY,
    OAM_TIMER_TRACK_TYPE_IMMEDIATE_DESTROY,

    OAM_TIMER_TRACK_TYPE_BUTT
} oam_timer_track_type_enum;
typedef uint8_t oam_timer_track_type_enum_uint8;

/* ���û��������ö�� */
/* ���û��������ݣ�֡���ݣ�CB�ֶΣ������� */
typedef enum {
    OAM_USER_TRACK_CONTENT_FRAME = 0,
    OAM_USER_TRACK_CONTENT_CB,
    OAM_USER_TRACK_CONTENT_DSCR,

    OAM_USER_TRACK_CONTENT_BUTT
} oam_user_track_content_enum;
typedef uint8_t oam_user_track_content_enum_uint8;

/* ���û�����֡����:���ݺ͹��� */
typedef enum {
    OAM_USER_TRACK_FRAME_TYPE_MGMT = 0,
    OAM_USER_TRACK_FRAME_TYPE_DATA,

    OAM_USER_TRACK_FRAME_TYPE_BUTT
} oam_user_track_frame_type_enum;
typedef uint8_t oam_user_track_frame_type_enum_uint8;

/* �����û���Ϣ�仯���ͣ�event������Ϣ��״̬�仯���� */
typedef enum {
    OAM_USER_INFO_CHANGE_TYPE_ASSOC_STATE = 0, /* �û�����״̬ */
    OAM_USER_INFO_CHANGE_TYPE_TX_PROTOCOL,     /* ��������֡ʹ�õ�Э��ģʽ */
    OAM_USER_INFO_CHANGE_TYPE_RX_PROTOCOL,     /* ��������֡ʹ�õ�Э��ģʽ */
    OAM_USER_INFO_CHANGE_TYPE_BUTT
} oam_user_info_change_type_enum;
typedef uint8_t oam_user_info_change_type_enum_uint8;

/* �����û��շ�����,event������Ϣ,������������ */
typedef enum {
    OAM_USER_TXRX_PARAM_TYPE_RSSI = 0,
    OAM_USER_TXRX_PARAM_TYPE_RATE,

    OAM_USER_TXRX_PARAM_TYPE_BUTT
} oam_user_txrx_param_type_enum;
typedef uint8_t oam_user_txrx_param_type_enum_uint8;

/* ͳ����Ϣ�ϱ����ͣ���Ϊdevice����vap����user���� */
typedef enum {
    OAM_STATS_TYPE_DEVICE,
    OAM_STATS_TYPE_VAP,
    OAM_STATS_TYPE_USER,

    OAM_STATS_TYPE_BUTT
} oam_stats_type_enum;
typedef uint8_t oam_stats_type_enum_uint8;

/* device����ͳ����Ϣ�ϱ������ͣ� */
typedef enum {
    OAM_STATS_DEVICE_SUBTYPE_IRQ,
    OAM_STATS_DEVICE_SUBTYPE_TIMER,
    OAM_STATS_DEVICE_SUBTYPE_MEMPOOL,
    OAM_STATS_DEVICE_SUBTYPE_EVENT_QUEUE,

    OAM_STATS_DEVICE_SUBTYPE_BUTT
} oam_stats_device_subtype_enum;
typedef uint8_t oam_stats_device_subtype_enum_uint8;

/* ALARM�ϱ��¼����� */
typedef enum {
    OAM_ALARM_TYPE_MEM_OVER_LEAK,
    OAM_ALARM_TYPE_PKT_RECV_LOST,
    OAM_ALARM_TYPE_PKT_SEND_LOST,

    OAM_ALARM_TYPE_BUTT
} oam_alarm_type_enum;
typedef uint16_t oam_alarm_type_enum_uint16;

/* �ڴ����Ϣ�ϱ����� */
typedef enum {
    OAM_MEMBLOCK_INFO_TYPE_UNIFORM,     /* ĳһ���ڴ�ص������ڴ����Ϣ */
    OAM_MEMBLOCK_INFO_TYPE_NON_UNIFORM, /* ҵ����������Ҫ�鿴�����������ڴ����Ϣ */

    OAM_MEMBLOCK_INFO_TYPE_BUTT
} oam_memblock_info_type_enum;
typedef uint8_t oam_memblock_info_type_enum_uint8;

/* STURCT�ϱ��¼����� */
typedef enum {
    OAM_STRUCT_TYPE_DEVICE,
    OAM_STRUCT_TYPE_HMAC_VAP,
    OAM_STRUCT_TYPE_MAC_VAP,
    OAM_STRUCT_TYPE_DMAC_VAP,
    OAM_STRUCT_TYPE_HMAC_USER,
    OAM_STRUCT_TYPE_MAC_USER,
    OAM_STRUCT_TYPE_DMAC_USER,
    OAM_STRUCT_TYPE_TID,

    OAM_STRUCT_TYPE_BUTT
} oam_struct_type_enum;

/* SDT��ӡbeacon֡��beacon֡�����������Ŀ�������,����beacon֡�Ĵ�ӡ�շ����̶����� */
typedef enum {
    OAM_SDT_PRINT_BEACON_RXDSCR_TYPE_BEACON = 1, /* ��ӡbeacon֡������ӡbeacon֡�Ľ��������� */
    OAM_SDT_PRINT_BEACON_RXDSCR_TYPE_RXDSCR,     /* ��ӡbeacon֡�Ľ���������������ӡbeacon֡ */
    OAM_SDT_PRINT_BEACON_RXDSCR_TYPE_BOTH,       /* ��ӡbeacon֡�Լ����������� */

    OAM_SDT_PRINT_BEACON_RXDSCR_TYPE_BUTT
} oam_sdt_print_beacon_rxdscr_type_enum;
typedef uint8_t oam_sdt_print_beacon_rxdscr_type_enum_uint8;


/* ����֡�����ڽ������̻��Ƿ������̵�ö�� */
typedef enum {
    OAM_OTA_FRAME_DIRECTION_TYPE_TX = 0,
    OAM_OTA_FRAME_DIRECTION_TYPE_RX,

    OAM_OTA_FRAME_DIRECTION_TYPE_BUTT
} oam_ota_frame_direction_type_enum;
typedef uint8_t oam_ota_frame_direction_type_enum_uint8;

typedef enum {
    OAM_NL_CMD_SDT = 0, /* SDTģ�� */
    OAM_NL_CMD_HUT,     /* HUTģ�� */
    OAM_NL_CMD_ALG,     /* ALGģ�� */
    OAM_NL_CMD_ACS,     /* ACSģ�� */
    OAM_NL_CMD_DAQ,
    OAM_NL_CMD_REG,  /* �Ĵ�����ȡģ�� */
    OAM_NL_CMD_PSTA, /* proxysta */

    OAM_NL_CMD_BUTT
} oam_nl_cmd_enum;
typedef uint8_t oam_nl_cmd_enum_uint8;

/* ���Ժ����д��gst_oam_feature_list */
typedef enum {
    OAM_SF_SCAN = 0,
    OAM_SF_AUTH,
    OAM_SF_ASSOC,
    OAM_SF_FRAME_FILTER,
    OAM_SF_WMM,

    OAM_SF_DFS = 5,
    OAM_SF_NETWORK_MEASURE,
    OAM_SF_ENTERPRISE_VO,
    OAM_SF_HOTSPOTROAM,
    OAM_SF_NETWROK_ANNOUNCE,

    OAM_SF_NETWORK_MGMT = 10,
    OAM_SF_NETWORK_PWS,
    OAM_SF_PROXYARP,
    OAM_SF_TDLS,
    OAM_SF_CALIBRATE,

    OAM_SF_EQUIP_TEST = 15,
    OAM_SF_CRYPTO,
    OAM_SF_WPA,
    OAM_SF_WEP,
    OAM_SF_WPS,

    OAM_SF_PMF = 20,
    OAM_SF_WAPI,
    OAM_SF_BA,
    OAM_SF_AMPDU,
    OAM_SF_AMSDU,

    OAM_SF_STABILITY = 25,
    OAM_SF_TCP_OPT,
    OAM_SF_ACS,
    OAM_SF_AUTORATE,
    OAM_SF_TXBF,

    OAM_SF_DYN_RECV = 30, /* dynamin recv */
    OAM_SF_VIVO,          /* video_opt voice_opt */
    OAM_SF_MULTI_USER,
    OAM_SF_MULTI_TRAFFIC,
    OAM_SF_ANTI_INTF,

    OAM_SF_EDCA = 35,
    OAM_SF_SMART_ANTENNA,
    OAM_SF_TPC,
    OAM_SF_TX_CHAIN,
    OAM_SF_RSSI,

    OAM_SF_WOW = 40,
    OAM_SF_GREEN_AP,
    OAM_SF_PWR, /* psm uapsd fastmode */
    OAM_SF_SMPS,
    OAM_SF_TXOP,

    OAM_SF_WIFI_BEACON = 45,
    OAM_SF_KA_AP, /* keep alive ap */
    OAM_SF_MULTI_VAP,
    OAM_SF_2040, /* 20m+40m coex */
    OAM_SF_DBAC,

    OAM_SF_PROXYSTA = 50,
    OAM_SF_UM,  /* user managment */
    OAM_SF_P2P, /* P2P ���� */
    OAM_SF_M2U,
    OAM_SF_IRQ, /* top half */

    OAM_SF_TX = 55,
    OAM_SF_RX,
    OAM_SF_DUG_COEX,
    OAM_SF_CFG, /* wal dmac config���� */
    OAM_SF_FRW, /* frw�� */

    OAM_SF_KEEPALIVE = 60,
    OAM_SF_COEX,
    OAM_SF_HS20, /* HotSpot 2.0���� */
    OAM_SF_MWO_DET,
    OAM_SF_CCA_OPT,

    OAM_SF_ROAM = 65, /* roam module, #ifdef _PRE_WLAN_FEATURE_ROAM */
    OAM_SF_DFT,
    OAM_SF_DFR,
    OAM_SF_RRM,
    OAM_SF_VOWIFI,

    OAM_SF_OPMODE = 70,
    OAM_SF_M2S,
    OAM_SF_DBDC,
    OAM_SF_HILINK,
    OAM_SF_WDS, /* WDS ���� */

    OAM_SF_WMMAC = 75,
    OAM_SF_USER_EXTEND,
    OAM_SF_PKT_CAP, /* ץ������ */
    OAM_SF_SOFT_CRYPTO,

    OAM_SF_CAR,  /* ��������  */
    OAM_SF_11AX, /* 11AX ���� */
    OAM_SF_CSA,
    OAM_SF_CONN, /* ONT��־ */
    OAM_SF_CHAN, /* ONT��־ */
    OAM_SF_CUSTOM,

    OAM_SF_QOS,
    OAM_SF_INI,
#ifdef _PRE_WLAN_FEATURE_HIEX
    OAM_SF_HIEX,
#else
    OAM_SF_RESERVE5,
#endif
    OAM_SF_RESERVE6,
    OAM_SF_RESERVE7,
    OAM_SF_RESERVE8,
    OAM_SF_RESERVE9,
    OAM_SF_ANY, /*
                 * rifs protection shortgi frag datarate countrycode
                 * coustom_security startup_time lsig monitor wds
                 * hidessid
                 */
#ifdef _PRE_WLAN_FEATURE_FTM
    OAM_SF_FTM,
#endif
    OAM_SF_WATERFILLING,
    OAM_SF_SAE,

#ifdef _PRE_WLAN_CHBA_MGMT
    OAM_SF_CHBA,
#endif
    OAM_SF_AUDIO = 95,
    OAM_SF_SCH,
    OAM_SF_TRX,
    OAM_SF_DSP,

    OAM_SOFTWARE_FEATURE_BUTT
} oam_feature_enum;
typedef uint8_t oam_feature_enum_uint8;

typedef enum {
    OAM_RATELIMIT_TYPE_PRINTK = 0,
    OAM_RATELIMIT_TYPE_LOG,
    OAM_RATELIMIT_TYPE_DSCR,
    OAM_RATELIMIT_TYPE_FRAME_WLAN,
    OAM_RATELIMIT_TYPE_FRAME_ETH,
    OAM_RATELIMIT_TYPE_CB,
    OAM_RATELIMIT_TYPE_BUTT
} oam_ratelimit_type_enum;
typedef uint8_t oam_ratelimit_type_enum_uint8;

#define OAM_EXCP_STATS_TIMEOUT 5000
/* ��ö�����Ͳ�����Ƶ������,ֻ�������쳣ԭ����ȷ�ķ�֧,�����ϱ��쳣������־ */
typedef enum {
    OAM_HAL_MAC_ERROR_PARA_CFG_ERR = 0,            /* ���������������쳣,����AMPDU�������ò�ƥ��,AMPDU��MPDU���ȳ���,sub msdu num���� */
    OAM_HAL_MAC_ERROR_RXBUFF_LEN_TOO_SMALL = 1,    /* ���շ�AMSDU֡������RxBuff��С�쳣 */
    OAM_HAL_MAC_ERROR_BA_ENTRY_NOT_FOUND = 2,      /* δ�ҵ�BA�Ự�����쳣0 */
    OAM_HAL_MAC_ERROR_PHY_TRLR_TIME_OUT = 3,       /* PHY_RX_TRAILER��ʱ */
    OAM_HAL_MAC_ERROR_PHY_RX_FIFO_OVERRUN = 4,     /* PHY_RX_FIFO��д�쳣 */
    OAM_HAL_MAC_ERROR_TX_DATAFLOW_BREAK = 5,       /* ����֡���ݶ��� */
    OAM_HAL_MAC_ERROR_RX_FSM_ST_TIMEOUT = 6,       /* RX_FSM״̬����ʱ */
    OAM_HAL_MAC_ERROR_TX_FSM_ST_TIMEOUT = 7,       /* TX_FSM״̬����ʱ */
    OAM_HAL_MAC_ERROR_RX_HANDLER_ST_TIMEOUT = 8,   /* RX_HANDLER״̬����ʱ */
    OAM_HAL_MAC_ERROR_TX_HANDLER_ST_TIMEOUT = 9,   /* TX_HANDLER״̬����ʱ */
    OAM_HAL_MAC_ERROR_TX_INTR_FIFO_OVERRUN = 10,   /* TX �ж�FIFO��д */
    OAM_HAL_MAC_ERROR_RX_INTR_FIFO_OVERRUN = 11,   /* RX�ж� FIFO��д */
    OAM_HAL_MAC_ERROR_HIRX_INTR_FIFO_OVERRUN = 12, /* HIRX�ж�FIFO��д */
    OAM_HAL_MAC_ERROR_RX_Q_EMPTY = 13,             /* ���յ���ͨ���ȼ�֡����ʱRX BUFFERָ��Ϊ�� */
    OAM_HAL_MAC_ERROR_HIRX_Q_EMPTY = 14,           /* ���յ������ȼ�֡����ʱHI RX BUFFERָ��Ϊ�� */
    OAM_HAL_MAC_ERROR_BUS_RLEN_ERR = 15,           /* ���߶����󳤶�Ϊ0�쳣 */
    OAM_HAL_MAC_ERROR_BUS_RADDR_ERR = 16,          /* ���߶������ַ��Ч�쳣 */
    OAM_HAL_MAC_ERROR_BUS_WLEN_ERR = 17,           /* ����д���󳤶�Ϊ0�쳣 */
    OAM_HAL_MAC_ERROR_BUS_WADDR_ERR = 18,          /* ����д�����ַ��Ч�쳣 */
    OAM_HAL_MAC_ERROR_TX_ACBK_Q_OVERRUN = 19,      /* tx acbk����fifo��д */
    OAM_HAL_MAC_ERROR_TX_ACBE_Q_OVERRUN = 20,      /* tx acbe����fifo��д */
    OAM_HAL_MAC_ERROR_TX_ACVI_Q_OVERRUN = 21,      /* tx acvi����fifo��д */
    OAM_HAL_MAC_ERROR_TX_ACVO_Q_OVERRUN = 22,      /* tx acv0����fifo��д */
    OAM_HAL_MAC_ERROR_TX_HIPRI_Q_OVERRUN = 23,     /* tx hipri����fifo��д */
    OAM_HAL_MAC_ERROR_MATRIX_CALC_TIMEOUT = 24,    /* matrix���㳬ʱ */
    OAM_HAL_MAC_ERROR_CCA_TIMEOUT = 25,            /* cca��ʱ */
    OAM_HAL_MAC_ERROR_DCOL_DATA_OVERLAP = 26,      /* ����overlap�澯 */
    OAM_HAL_MAC_ERROR_BEACON_MISS = 27,            /* ��������beaconʧ�� */
    OAM_HAL_MAC_ERROR_UNKOWN_28 = 28,
    OAM_HAL_MAC_ERROR_UNKOWN_29 = 29,
    OAM_HAL_MAC_ERROR_UNKOWN_30 = 30,
    OAM_HAL_MAC_ERROR_UNKOWN_31 = 31,

    OAM_HAL_SOC_ERROR_BUCK_OCP = 32, /* PMU BUCK�����ж� */
    OAM_HAL_SOC_ERROR_BUCK_SCP,      /* PMU BUCK��·�ж� */
    OAM_HAL_SOC_ERROR_OCP_RFLDO1,    /* PMU RFLDO1�����ж� */
    OAM_HAL_SOC_ERROR_OCP_RFLDO2,    /* PMU RFLDO2�����ж� */
    OAM_HAL_SOC_ERROR_OCP_CLDO,      /* PMU CLDO�����ж� */
    OAM_HAL_SOC_ERROR_RF_OVER_TEMP,  /* RF�����ж� */
    OAM_HAL_SOC_ERROR_CMU_UNLOCK,    /* CMU PLLʧ���ж� */
    OAM_HAL_SOC_ERROR_PCIE_SLV_ERR,

    OAM_EXCP_TYPE_BUTT
} oam_excp_type_enum;
typedef uint8_t oam_excp_type_enum_uint8;

typedef enum {
    OAM_EXCP_STATUS_INIT = 0,  /* ��ʼ״̬:�ϱ���ɺ��л�����ʼ״̬ */
    OAM_EXCP_STATUS_REFRESHED, /* �и��£����ϱ��� */
    OAM_EXCP_STATUS_BUTT
} oam_excp_status_enum;
typedef uint8_t oam_excp_status_enum_uint8;

/* STRUCT���� */
/* SDT�ϴ��ӿڵ�ע�ắ������ */
typedef int32_t (*oam_sdt_up_func)(oal_netbuf_stru *pst_netbuf);
typedef int32_t (*oam_sdt_report_data_func)(oal_netbuf_stru *pst_netbuf,
                                            oam_data_type_enum_uint8 en_type,
                                            oam_primid_type_enum_uint8 en_prim);
typedef int32_t (*oam_sdt_status_func)(void);

/* ����ģ�����SDT���Ӻ��� �ýṹ�� */
typedef struct {
    int32_t (*p_sdt_report_data_func)(oal_netbuf_stru *pst_netbuf, oam_data_type_enum_uint8 en_type,
                                      oam_primid_type_enum_uint8 en_prim);
    int32_t (*p_sdt_get_wq_len_func)(void);
} oam_sdt_func_hook_stru;

/* �ⲿ�������� */
extern oam_sdt_func_hook_stru g_oam_sdt_func_hook;

/* ����ģ�����WAL���Ӻ��� �ýṹ�� */
typedef struct {
    int32_t (*p_wal_recv_mem_data_func)(uint8_t *puc_buf, uint16_t us_len);
    int32_t (*p_wal_recv_reg_data_func)(uint8_t *puc_buf, uint16_t us_len);
    int32_t (*p_wal_recv_cfg_data_func)(uint8_t *puc_buf, uint16_t us_len);
    int32_t (*p_wal_recv_global_var_func)(uint8_t *puc_buf, uint16_t us_len);
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    int32_t (*p_wal_recv_sample_data_func)(uint8_t *puc_buf, uint16_t us_len);
#endif
} oam_wal_func_hook_stru;

extern oam_wal_func_hook_stru g_oam_wal_func_hook;

/* VAPID ��־����ṹ�� */
typedef struct {
    oam_feature_enum_uint8 en_feature_id;
    oam_log_level_enum_uint8 en_log_level;
    oal_switch_enum_uint8 en_switch;
    uint8_t uc_resv;
} oam_cfg_log_level_stru;

typedef struct {
    uint8_t bit_vap_id : 4;
    uint8_t bit_log_level : 4;
} oam_vap_log_level_stru;

/* ��־��Ϣ�ṹ�� */
typedef struct {
    oam_vap_log_level_stru st_vap_log_level; /* vapid + loglevel */
    oam_feature_enum_uint8 en_feature_id;    /* ����id */
    uint16_t us_resv;
    uint16_t us_file_id;  /* �ļ��� */
    uint16_t us_line_num; /* �к� */
    uint32_t ul_tick;     /* ʱ���������ms */
    int32_t al_param[OAM_LOG_ENTRY_PARA_NUM];
} oam_log_info_stru;

/* OTAͷ����Ϣ */
typedef struct {
    uint32_t ul_tick;                  /* ʱ���������ms */
    oam_ota_type_enum_uint8 en_ota_type; /* OTA���� */
    uint8_t uc_frame_hdr_len;          /* �����֡������֡ͷ���ȣ�������������ͣ����ֶ�û���� */
    uint16_t us_ota_data_len;          /* OTA���ݳ��ȣ������֡������֡ͷ��֡����ܳ��� */
    uint8_t auc_user_macaddr[MAC_ADDR_LEN];
    oam_ota_frame_direction_type_enum_uint8 en_frame_direction; /* ����֡�����ڷ������̻��ǽ������̣�ֻ��ota_type��֡��ʱ������ֶβ������� */
    uint8_t auc_resv[1];                                      /* ���ֲ�Ʒ */
} oam_ota_hdr_stru;

/* OTA���ݽṹ�� */
typedef struct {
    oam_ota_hdr_stru st_ota_hdr; /* OTAͷ����Ϣ */
    uint8_t auc_ota_data[4];   /* OTA���ݹ̶�4���ֽ� */
} oam_ota_stru;

/* ��Ϣ�ϱ���SDT��װ�Ľṹ�� */
/* EVENT��Ϣ����󳤶� */
#define OAM_EVENT_INFO_MAX_LEN 48

/* �ڴ��ʹ����Ϣ��ѯ */
typedef struct {
    uint8_t uc_mem_pool_id;
    uint8_t uc_subpool_id;
    uint8_t auc_resv[2];
    uint16_t us_mem_pool_total_cnt;
    uint16_t us_mem_pool_used_cnt;
    uint16_t us_subpool_total_cnt;
    uint16_t us_subpool_free_cnt;
} oam_stats_mempool_stru;

/* �ڴ��ʹ����Ϣ��������׼�ڴ����Ϣ��������ĳһ�������ڴ���Ϣ */
typedef struct {
    uint8_t uc_pool_id; /* ��������������ڴ� */
    uint8_t uc_subpool_id;
    uint8_t uc_user_cnt; /* ���ڴ������ü��� */
    uint8_t auc_resv[1];
    uint32_t ul_file_id;        /* ������ڴ����ļ��� */
    uint32_t ul_alloc_line_num; /* ������ڴ����к� */
} oam_memblock_info_stru;

/* �¼�����ʹ�������Ϣ�ṹ�� */
typedef struct {
    uint8_t en_type;      /* �¼����� */
    uint8_t uc_sub_type;  /* �¼������� */
    uint16_t us_length;   /* �¼����� */
    uint8_t en_pipeline;  /* �¼��ֶκ� */
    uint8_t uc_chip_id;   /* оƬID */
    uint8_t uc_device_id; /* �豸ID */
    uint8_t uc_vap_id;    /* VAP ID */
} oam_event_hdr_info_stru;

/*lint -e717*/
#define oam_sdt_stat_incr(_cnt)   \
    do {                          \
        g_sdt_stat_info._cnt++; \
    } while (0)

enum oam_filter_type_enum {
    OAM_FLT_PASS = 0,
    OAM_FLT_DROP,
    OAM_FLT_BUTT,
};

typedef struct {
    uint32_t ul_nlk_sd_cnt;       /* �ϱ���Ϣ����:��ʧ�ܸ��� */
    uint32_t ul_nlk_sd_fail;      /* ����ʧ��ͳ�� */
    uint32_t ul_wq_len;           /* ��������Ϣ���� */
    uint32_t ul_filter_cnt;       /* ������Ϣ���� */
    oal_bool_enum en_filter_switch; /* ���˿���״̬ */
    uint8_t uc_recv[3];
} oam_sdt_stat_info_stru;

extern oam_sdt_stat_info_stru g_sdt_stat_info;

/* ���������ļ������Ϣ�ṹ */
typedef struct {
    oam_cfg_type_enum_uint16 en_cfg_type;
    int8_t *pc_section; /* ������������section�ַ��� */
    int8_t *pc_key;     /* �������������ļ��е��ַ�����ʾ */
    int32_t l_val;      /* �������ֵ */
} oam_cfg_data_stru;

/* �쳣ͳ�ƽṹ��:��ul_reported_cnt��ul_record_cnt��ͬʱ */
typedef struct {
    uint32_t en_status;     /* ÿ���쳣�����Ƿ�ˢ�� */
    uint32_t ul_record_cnt; /* ��¼��һ�θ��µĴ��� */
} oam_excp_record_stru;

/* oam�쳣ͳ�� */
typedef struct {
    uint32_t en_status; /* VAP�����쳣ͳ���Ƿ���ˢ�� */
    oam_excp_record_stru ast_excp_record_tbl[OAM_EXCP_TYPE_BUTT];
} oam_exception_ctx_stru;

/* VAP��־������������־���� */
typedef struct {
    oam_log_level_enum_uint8 aen_vap_log_level[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];  /* VAP��־���أ�INFO����ر�ʱ�ָ� */
    /* ������־���� */
    oam_log_level_enum_uint8 aen_feature_log_level[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT][OAM_SOFTWARE_FEATURE_BUTT];
} oam_vap_log_stru;

/* ��־ģ��ȫ�ֽṹ�� */
typedef struct {
    oal_switch_enum_uint8 en_global_log_switch;               /* ��־ȫ�ֿ��� */
    oam_vap_log_stru st_vap_log_info;                         /* VAP��־������������־���� */
} oam_log_ctx_stru;

/* ota����ö��, ע�⣬�¼ӿ���ʱ������ں��� */
typedef enum {
    OAM_OTA_SWITCH_BEACON = 0, /* �˴��������շ����� */
    OAM_OTA_SWITCH_RX_PPDU_DSCR = 1,
    OAM_OTA_SWITCH_RX_MSDU_DSCR = 2,
    OAM_OTA_SWITCH_TX_PPDU_DSCR = 3,
    OAM_OTA_SWITCH_TX_COMPLETE_PPDU_DSCR = 4,
    OAM_OTA_SWITCH_RX_80211_FRAME = 5,
    OAM_OTA_SWITCH_TX_80211_FRAME = 6,
    OAM_OTA_SWITCH_RX_CB = 7,
    OAM_OTA_SWITCH_TX_CB = 8,
    OAM_OTA_SWITCH_RX_ETH_FRAME = 9,
    OAM_OTA_SWITCH_TX_ETH_FRAME = 10,
    OAM_OTA_SWITCH_ALG_RX = 11,
    OAM_OTA_SWITCH_ALG_TX = 12,
    OAM_OTA_SWITCH_BUTT,
} oam_ota_switch_enum;

/* oam��otaģ�飬����ĳһ��ota�Ŀ��ƿ��� */
typedef struct {
    oal_switch_enum_uint8 ota_switch[OAM_OTA_SWITCH_BUTT];

    /* ����ota_switch_mib��Ϊota�ܿ��أ�ֻҪ��һ��OTA�������򲻽�ota�ϱ����̣���������mips */
    /* ota_switch_mib�и���bit�Ķ�����ο�oam_ota_switch_enum����������: */
    /* |      ...       | TX_COMPLETE_PPDU_DSCR | TX_PPDU_DSCR | RX_MSDU_DSCR | RX_PPDU_DSCR | BEACON | */
    /* | bit15 - bit5   |          bit4         |     bit3     |     bit2     |      bit1    |  bit0  | */
    uint32_t golble_switch;
} oam_ota_ctx_stru;

/* ����OTA�ϱ�������Ҫ�Ĳ��� */
typedef struct {
    uint8_t ota_type;
    uint8_t ota_switch;
} oam_ota_switch_param_stru;

/* 80211֡�ϱ����ƽṹ */
typedef struct {
    uint8_t bit_content : 1,
               bit_cb : 1,
               bit_dscr : 1,
               bit_ba_info : 1,
               bit_msdu_dscr : 1,
               bit_mu_dscr : 1,
               bit_himit_dscr : 1,
               bit_rsv : 1;
} oam_80211_frame_ctx_stru;

typedef union {
    uint8_t value;
    oam_80211_frame_ctx_stru frame_ctx;
} oam_80211_frame_ctx_union;
/* ���û����ٿ��ƿ��ؽṹ */
typedef struct {
    /* 80211�㲥\�鲥����֡�ϱ����ƽṹ */
    oam_80211_frame_ctx_union ast_80211_mcast_mgmt_ctx[OAM_OTA_FRAME_DIRECTION_TYPE_BUTT];
    /* 80211�㲥\�鲥����֡�ϱ����ƽṹ */
    oam_80211_frame_ctx_union ast_80211_mcast_data_ctx[OAM_OTA_FRAME_DIRECTION_TYPE_BUTT];
    /* ����֡���أ���ͬ�û����ؿ��Բ�ͬ */
    oam_80211_frame_ctx_union ast_80211_ucast_mgmt_ctx[WLAN_USER_MAX_USER_LIMIT][OAM_OTA_FRAME_DIRECTION_TYPE_BUTT];
    /* ����֡���أ���ͬ�û����ؿ��Բ�ͬ */
    oam_80211_frame_ctx_union ast_80211_ucast_data_ctx[WLAN_USER_MAX_USER_LIMIT][OAM_OTA_FRAME_DIRECTION_TYPE_BUTT];
    /* probe request��probe response̫�࣬����������� */
    oam_80211_frame_ctx_union aen_80211_probe_switch[OAM_OTA_FRAME_DIRECTION_TYPE_BUTT];
    /* 802.3����֡�ϱ����ƽṹ */
    oal_switch_enum_uint8 aen_eth_data_ctx[WLAN_USER_MAX_USER_LIMIT][OAM_OTA_FRAME_DIRECTION_TYPE_BUTT];
    oal_switch_enum_uint8 en_tx_mcast_dhcp_arp_switch; /* ����֡�ϱ����� */
    /* ����֡���̴�ӡ�ܿ��أ�ֻҪ��һ���û������ݿ��ش򿪣��ܿ��ؾͿ�������رգ���������mips */
    oal_switch_enum_uint8 aen_data_global_switch[OAM_OTA_FRAME_DIRECTION_TYPE_BUTT];
    oal_switch_enum_uint8 aen_user_info_change_switch[WLAN_USER_MAX_USER_LIMIT][OAM_USER_INFO_CHANGE_TYPE_BUTT];
} oam_user_track_ctx_stru;

#define OAM_FILE_PATH_LENGTH 256 /* ���ڱ���LOG�ļ���ַ���ַ�����󳤶�,·���Ϊ255�ֽڣ������Ҫ��һ�ֽڴ��\0 */

typedef struct {
    uint8_t auc_resv[3];
    oam_output_type_enum_uint8 en_output_type;   /* �����ʽ */

    /* LOGģ����������� */
    oam_log_ctx_stru st_log_ctx;

    /* ĳһ��OTA��Ϣ�Ŀ��ƽṹ */
    oam_ota_ctx_stru ast_ota_ctx;

    oam_exception_ctx_stru st_exception_ctx[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];

    /* ���û����ٹ��������� */
    oam_user_track_ctx_stru st_user_track_ctx;
} oam_mng_ctx_stru;

/* ͳ��������ά����صĽṹ���� */
#ifdef _PRE_WLAN_DFT_STAT

/* ��Ӧ�տڻ�����æ�̶ȵĲ����ṹ����PHY�Ĳ���Ҳ��MAC�Ĳ��� */
typedef struct {
    uint16_t us_mac_ch_stats_period_us; /* mac�����ŵ�����ʱ��ͳ�Ƶ����ڣ�20ms��20000us */
    uint16_t us_mac_pri20_free_time_us; /* ͳ����������20M�ŵ�����ʱ�䣬us */
    uint16_t us_mac_pri40_free_time_us; /* ͳ����������40M�ŵ�����ʱ�䣬us */
    uint16_t us_mac_pri80_free_time_us; /* ͳ����������80M�ŵ�����ʱ��,us */
    uint16_t us_mac_ch_rx_time_us;      /* ͳ�������ڽ���ռ�õ�ʱ�䣬us */
    uint16_t us_mac_ch_tx_time_us;      /* ͳ�������ڷ���ռ�õ�ʱ�䣬us */

    uint8_t uc_phy_ch_estimate_time_ms; /* phy�����ŵ�����ʱ���ʵ����ڣ�ms */
    int8_t c_phy_pri20_idle_power_dbm;  /* ͳ����������20M�ŵ�����ʱ�Ĺ��ʣ�һֱ�����оͲ�ͳ�� */
    int8_t c_phy_pri40_idle_power_dbm;  /* ͳ����������40M�ŵ�����ʱ�Ĺ��ʣ�һֱ�����оͲ�ͳ�� */
    int8_t c_phy_pri80_idle_power_dbm;  /* ͳ����������80M�ŵ�����ʱ�Ĺ��ʣ�һֱ�����оͲ�ͳ�� */

    uint32_t aul_beacon_miss_max_num[WLAN_SERVICE_AP_MAX_NUM_PER_DEVICE]; /* mac��¼��ÿ��AP BEACON MISS��ʷ���ֵ */
    uint32_t non_directed_frame_num;
} oam_stats_dbb_env_param_stru;

/* phy���շ�����Ŀͳ��ֵ�������û�ָ����Ҫ��4������phyһֱͳ�Ƶ�8������Э��ģʽ��ͳ��ֵ */
typedef struct {
    uint32_t aul_user_requested_stat_cnt[OAM_PHY_STAT_NODE_ENABLED_MAX_NUM];

    /* phyͳ�ƵĽ��յ�11b,ht,vht,legecy ofdm֡��ȷ�ʹ���ĸ��� */
    uint32_t aul_phy_stat_rx_ppdu_cnt[OAM_PHY_STAT_RX_PPDU_CNT];
} oam_stats_phy_stat_stru;

/* macͳ���շ�����Ŀ */
typedef struct {
    /* rxͳ�� */
    uint32_t ul_machw_rx_ampdu_count;
    uint32_t ul_machw_rx_passed_mpdu_in_ampdu_cnt;
    uint32_t ul_machw_rx_failed_mpdu_in_ampdu_cnt;
    uint32_t ul_machw_rx_octects_in_ampdu;
    uint32_t ul_machw_rx_delimit_fail_count;
    uint32_t ul_machw_rx_dup_mpdu_cnt;
    uint32_t ul_machw_rx_passed_mpdu_cnt;
    uint32_t ul_machw_rx_failed_mpdu_cnt;
    uint32_t ul_machw_rx_bcn_cnt;
    uint32_t ul_machw_rx_phy_err_mac_passed_cnt;
    uint32_t ul_machw_rx_phy_longer_err_cnt;
    uint32_t ul_machw_rx_phy_shorter_err_cnt;
    uint32_t ul_machw_rx_filtered_cnt;

    /* txͳ�� */
    uint32_t ul_machw_tx_hi_pri_mpdu_cnt;
    uint32_t ul_machw_tx_normal_pri_mpdu_cnt;
    uint32_t ul_machw_tx_ampdu_count;
    uint32_t ul_machw_tx_mpdu_inampdu_count;
    uint32_t ul_machw_tx_octects_in_ampdu;
    uint32_t ul_machw_tx_bcn_count;
    uint32_t ul_machw_tx_normal_pri_retry_cnt;
    uint32_t ul_machw_tx_hi_pri_retry_cnt;
} oam_stats_machw_stat_stru;

/* vap����ͳ����Ϣ�ṹ�����������ֽ�����ƽ������(�ֽ���/�ϱ�����) */
typedef struct {
    uint32_t ul_lan_tx_pkts;  /* ��̫�������İ���Ŀ */
    uint32_t ul_lan_tx_bytes; /* ��̫���������ֽ�����������̫��ͷ */
    uint32_t ul_drv_tx_pkts;  /* ����������Ŀ������Ӳ������Ŀ */
    uint32_t ul_drv_tx_bytes; /* ���������ֽ���������80211ͷβ */
    uint32_t ul_hw_tx_pkts;   /* ��������ж��ϱ����ͳɹ��ĸ��� */
    uint32_t ul_hw_tx_bytes;  /* ��������ж��ϱ����ͳɹ��ֽ��� */

    uint32_t ul_drv_rx_pkts;  /* �������հ���Ŀ */
    uint32_t ul_drv_rx_bytes; /* ���������ֽ�����������80211ͷβ */
    uint32_t ul_lan_rx_pkts;  /* ����������̫������Ŀ */
    uint32_t ul_lan_rx_bytes; /* ����������̫���ֽ�������������̫��ͷ */
    uint32_t ul_app_rx_pkts;  /* ����������APP����Ŀ */
    uint32_t ul_app_rx_bytes; /* ����������APP�ֽ�������������̫��ͷ */

    /* ƽ������ ��λ: kbps */
    uint32_t ul_lan_tx_rate; /* lan����ƽ������ */
    uint32_t ul_drv_tx_rate; /* ��������ƽ������ */
    uint32_t ul_hw_tx_rate;  /* ��������ϱ��ɹ��ķ���ƽ������ */
    uint32_t ul_drv_rx_rate; /* �����հ�ƽ������ */
    uint32_t ul_lan_rx_rate; /* lan�հ�ƽ������ */
} oam_stats_vap_stat_stru;
#endif

/* ��������    OAMģ������ṩ�Ľӿ� */
extern oam_mng_ctx_stru g_oam_mng_ctx;

extern int32_t oam_main_init(void);
extern void oam_main_exit(void);
extern uint32_t oam_print(const char *pc_string);
extern uint32_t oam_set_output_type(oam_output_type_enum_uint8 en_output_type);
extern uint32_t oam_get_output_type(oam_output_type_enum_uint8 *pen_output_type);

void oam_log_set_global_switch(oal_switch_enum_uint8 en_log_switch);
extern uint32_t oam_log_set_feature_level(uint8_t uc_vap_id,
                                          oam_feature_enum_uint8 en_feature_id,
                                          oam_log_level_enum_uint8 en_log_level);
extern uint32_t oam_get_feature_id(uint8_t *puc_feature_name,
                                   oam_feature_enum_uint8 *puc_feature_id);
extern uint32_t oam_show_feature_list(void);
void oam_log_set_vap_level(uint8_t uc_vap_id, oam_log_level_enum_uint8 en_log_level);
extern uint32_t oam_log_print0(uint32_t ul_para,
                               uint16_t us_line_num,
                               int8_t *pc_string);

extern uint32_t oam_log_print1(uint32_t ul_para,
                               uint16_t us_line_num,
                               int8_t *pc_string,
                               oal_uint l_param1);

extern uint32_t oam_log_print2(uint32_t ul_para,
                               uint16_t us_line_num,
                               int8_t *pc_string,
                               oal_uint l_param1,
                               oal_uint l_param2);

extern uint32_t oam_log_print3(uint32_t ul_para,
                               uint16_t us_line_num,
                               int8_t *pc_string,
                               oal_uint l_param1,
                               oal_uint l_param2,
                               oal_uint l_param3);

extern uint32_t oam_log_print4(uint32_t ul_para,
                               uint16_t us_line_num,
                               int8_t *pc_string,
                               oal_uint l_param1,
                               oal_uint l_param2,
                               oal_uint l_param3,
                               oal_uint l_param4);

extern uint32_t oam_log_console_printk(uint16_t us_file_no,
                                       uint16_t us_line_num,
                                       const int8_t *pc_func_name,
                                       const int8_t *pc_fmt, ...);
extern uint32_t oam_ota_report(uint8_t *puc_param_one_addr,
                               uint16_t us_param_one_len,
                               uint8_t *puc_param_two_addr,
                               uint16_t us_param_two_len,
                               oam_ota_type_enum_uint8 en_ota_type);
void oam_ota_set_switch(uint8_t ota_switch_id, uint8_t ota_switch);
uint32_t oam_ota_get_global_switch(void);
oal_switch_enum_uint8 oam_ota_get_switch(uint8_t ota_switch_id);
extern uint32_t oam_report_eth_frame(uint8_t *puc_user_mac_addr,
                                     uint8_t *puc_eth_frame_hdr_addr,
                                     uint16_t us_eth_frame_len,
                                     oam_ota_frame_direction_type_enum_uint8 en_eth_frame_direction);
extern uint32_t oam_report_dscr(uint8_t *puc_user_macaddr,
                                uint8_t *puc_dscr_addr,
                                uint16_t us_dscr_len,
                                oam_ota_type_enum_uint8 en_ota_type);
extern void oam_report_backtrace(void);
extern uint32_t oam_report_80211_mcast_get_switch(oam_ota_frame_direction_type_enum_uint8 en_mcast_direction,
                                                  oam_user_track_frame_type_enum_uint8 en_frame_type,
                                                  oam_80211_frame_ctx_union *oam_frame_report_un);
extern uint32_t oam_report_80211_ucast_get_switch(oam_ota_frame_direction_type_enum_uint8 en_ucast_direction,
                                                  oam_user_track_frame_type_enum_uint8 en_frame_type,
                                                  oam_80211_frame_ctx_union *oam_frame_report_un,
                                                  uint16_t us_user_idx);
extern uint32_t oam_report_80211_probe_get_switch(oam_ota_frame_direction_type_enum_uint8 en_probereq_direction,
                                                  oam_80211_frame_ctx_union *oam_frame_report_un);
extern uint32_t oam_report_80211_probe_set_switch(oam_ota_frame_direction_type_enum_uint8 en_probe_direction,
                                                  oam_80211_frame_ctx_union *oam_switch_param);
extern uint32_t oam_report_eth_frame_get_switch(uint16_t us_user_idx,
                                                oam_ota_frame_direction_type_enum_uint8 en_eth_direction,
                                                oal_switch_enum_uint8 *pen_eth_switch);
extern uint32_t oam_report_netbuf_cb(uint8_t *user_mac_addr,
                                     uint8_t *netbuf_cb,
                                     uint16_t cb_len,
                                     oam_ota_type_enum_uint8 en_ota_type);
extern uint32_t oam_report_dft_params(uint8_t *puc_user_mac_addr,
                                      uint8_t *puc_param,
                                      uint16_t us_param_len,
                                      oam_ota_type_enum_uint8 en_type);
void oam_report_set_all_switch(oal_switch_enum_uint8 en_switch);
extern uint32_t oam_report_80211_ucast_set_switch(oam_ota_frame_direction_type_enum_uint8 en_ucast_direction,
                                                  oam_user_track_frame_type_enum_uint8 en_frame_type,
                                                  oam_80211_frame_ctx_union *oam_switch_param,
                                                  uint16_t us_user_idx);
extern uint32_t oam_report_eth_frame_set_switch(uint16_t us_user_idx,
                                                oal_switch_enum_uint8 en_switch,
                                                oam_ota_frame_direction_type_enum_uint8 en_eth_direction);
extern uint32_t oam_report_80211_mcast_set_switch(oam_ota_frame_direction_type_enum_uint8 en_mcast_direction,
                                                  oam_user_track_frame_type_enum_uint8 en_frame_type,
                                                  oam_80211_frame_ctx_union *oam_frame_report);
extern uint32_t oam_report_dhcp_arp_set_switch(oal_switch_enum_uint8 en_switch);
extern oal_switch_enum_uint8 oam_report_dhcp_arp_get_switch(void);
extern oal_switch_enum_uint8 oam_report_data_get_global_switch(oam_ota_frame_direction_type_enum_uint8 en_direction);


/* SDTģ�����ӿ� */
extern void oam_sdt_func_fook_register(oam_sdt_func_hook_stru *pfun_st_oam_sdt_hook);
extern void oam_sdt_func_fook_unregister(void);
extern void oam_wal_func_fook_register(oam_wal_func_hook_stru *pfun_st_oam_wal_hook);
extern void oam_wal_func_fook_unregister(void);
extern uint32_t oam_report_data2sdt(oal_netbuf_stru *pst_netbuf,
                                    oam_data_type_enum_uint8 en_type,
                                    oam_primid_type_enum_uint8 en_prim);
extern oal_netbuf_stru *oam_alloc_data2sdt(uint16_t us_data_len);

/* STATISTICSģ�����ӿ� */
extern uint32_t oam_stats_report_timer_info_to_sdt(uint8_t *puc_timer_addr,
                                                   uint8_t uc_timer_len);
extern uint32_t oam_stats_report_event_queue_info_to_sdt(uint8_t *puc_event_queue_addr,
                                                         uint16_t us_event_queue_info_len);
extern void oam_stats_clear_stat_info(void);
extern uint32_t oam_stats_report_usr_info(uint16_t us_usr_id);
extern uint32_t oam_stats_report_mempool_info_to_sdt(uint8_t uc_pool_id,
                                                     uint16_t us_pool_total_cnt,
                                                     uint16_t us_pool_used_cnt,
                                                     uint8_t uc_subpool_id,
                                                     uint16_t us_subpool_total_cnt,
                                                     uint16_t us_subpool_free_cnt);
extern uint32_t oam_stats_report_memblock_info_to_sdt(uint8_t *puc_origin_data,
                                                      uint8_t uc_user_cnt,
                                                      uint8_t uc_pool_id,
                                                      uint8_t uc_subpool_id,
                                                      uint16_t us_len,
                                                      uint32_t ul_file_id,
                                                      uint32_t ul_alloc_line_num);
extern void oam_stats_report_irq_info_to_sdt(uint8_t *puc_irq_info_addr, uint16_t us_irq_info_len);

/* �������ļ���صĺ������� */
extern int32_t oam_cfg_get_item_by_id(oam_cfg_type_enum_uint16 en_cfg_type);

OAL_STATIC OAL_INLINE oal_switch_enum_uint8 oam_get_log_switch(uint32_t ul_para)
{
    uint8_t uc_vap_id = (ul_para >> 24) & 0xf; /* delocalize 24 */
    oam_feature_enum_uint8 en_feature_id = (ul_para >> 16) & 0xff; /* delocalize 16 */
    oam_log_level_enum_uint8 en_log_level = (ul_para >> 28) & 0xf; /* delocalize 28 */

    /* ��ִ�����ܽǶȿ��ǣ���ǰ�ж����Կ��أ��󲿷�info��־�����ڵ�һ�������ֱ���˳� */
    if (g_oam_mng_ctx.st_log_ctx.en_global_log_switch == OAL_SWITCH_OFF) {
        return OAL_SWITCH_OFF;
    } else if (en_log_level > g_oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_vap_log_level[uc_vap_id]) {
        return OAL_SWITCH_OFF;
    } else if (en_log_level >
               g_oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_feature_log_level[uc_vap_id][en_feature_id]) {
        return OAL_SWITCH_OFF;
    }

    return OAL_SWITCH_ON;
}

#endif /* end of oam_ext_if.h */