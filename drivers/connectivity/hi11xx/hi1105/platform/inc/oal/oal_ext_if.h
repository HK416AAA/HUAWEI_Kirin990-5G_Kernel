

#ifndef __OAL_EXT_IF_H__
#define __OAL_EXT_IF_H__

/* ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_util.h"
#include "oal_hardware.h"
#include "oal_schedule.h"
#include "oal_bus_if.h"
#include "oal_net.h"
#include "oal_list.h"
#include "oal_queue.h"
#include "oal_workqueue.h"
#include "arch/oal_ext_if.h"
#include "oal_thread.h"
#include "oal_aes.h"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_exception_rst.h"
#endif
#include "oal_fsm.h"

/* �궨�� */
#define basic_value_is_hex(_auc_str) (((_auc_str)[0] == '0') && (((_auc_str)[1] == 'x') || ((_auc_str)[1] == 'X')))
/* ö�ٶ��� */
typedef enum {
    OAL_TRACE_ENTER_FUNC,
    OAL_TRACE_EXIT_FUNC,

    OAL_TRACE_DIRECT_BUTT
} oal_trace_direction_enum;
typedef uint8_t oal_trace_direction_enum_uint8;

/* ������ģʽ */
typedef enum {
    CS_BLACKLIST_MODE_NONE,  /* �ر�         */
    CS_BLACKLIST_MODE_BLACK, /* ������       */
    CS_BLACKLIST_MODE_WHITE, /* ������       */

    CS_BLACKLIST_MODE_BUTT
} cs_blacklist_mode_enum;
typedef uint8_t cs_blacklist_mode_enum_uint8;

#ifdef _PRE_WLAN_REPORT_WIFI_ABNORMAL
// �ϱ������쳣״̬���������ϲ���
typedef enum {
    OAL_ABNORMAL_FRW_TIMER_BROKEN = 0,  // frw��ʱ������
    OAL_ABNORMAL_OTHER = 1,             // �����쳣

    OAL_ABNORMAL_BUTT
} oal_wifi_abnormal_reason_enum;

typedef enum {
    OAL_ACTION_RESTART_VAP = 0,  // ����vap���ϲ���δʵ��
    OAL_ACTION_REBOOT = 1,       // ��������

    OAL_ACTION_BUTT
} oal_product_action_enum;

#define oal_report_wifi_abnormal(_l_reason, _l_action, _p_arg, _l_size)
#endif

typedef enum {
    OAL_WIFI_STA_LEAVE = 0,  // STA �뿪
    OAL_WIFI_STA_JOIN = 1,   // STA ����

    OAL_WIFI_BUTT
} oal_wifi_sta_action_report_enum;
#define oal_wifi_report_sta_action(_ul_ifindex, _ul_eventID, _p_arg, _l_size)

#define OAL_WIFI_COMPILE_OPT_CNT 10
typedef struct {
    uint8_t feature_m2s_modem_is_open : 1;
    uint8_t feature_priv_closed_is_open : 1;
    uint8_t feature_hiex_is_open : 1;
    uint8_t rx_listen_ps_is_open : 1;
    uint8_t feature_11ax_is_open : 1;
    uint8_t feature_twt_is_open : 1;
    uint8_t feature_11ax_er_su_dcm_is_open : 1;
    uint8_t feature_11ax_uora_is_supported : 1;

    uint8_t feature_ftm_is_open : 1;
    uint8_t feature_psm_dfx_ext_is_open : 1;
    uint8_t feature_wow_opt_is_open : 1; // 06ʹ��
    uint8_t feature_dual_wlan_is_supported : 1;
    uint8_t feature_ht_self_cure_is_open : 1; /* 05��� ht sig_len ���� */
    uint8_t feature_slave_ax_is_support : 1;
    uint8_t longer_than_16_he_sigb_support : 1;
    uint8_t p2p_device_gc_use_one_vap : 1;

    uint8_t feature_hw_wapi : 1;    /* �Ƿ�֧��оƬ�ӽ���WAPI���� */
    uint8_t resv : 7;

    uint32_t max_p2p_group_num;     /* P2P GO/GC������� */
    uint32_t p2p_go_max_user_num;   /* P2P GO�������û��� */
    uint32_t max_asoc_user;         /* 1��chip֧�ֵ��������û��� */
    uint32_t max_active_user;       /* 1��chip֧�ֵ���󼤻��û��� */
    uint32_t max_user_limit;        /* board��Դ����û����������������鲥�û� */
    uint32_t invalid_user_id;       /* ��tx dscr��CB�ֶ�,����user idx����ͬʱ��Ӧ,��Чuser idȡȫboard����û�LIMIT */
    uint32_t max_tx_ba;             /* ֧�ֵĽ���tx ba �������� */
    uint32_t max_rx_ba;             /* ֧�ֵĽ���rx ba �������� */
    uint32_t other_bss_id;          /* hal other bss id */
    uint32_t max_tx_ampdu_num;      /* tx�ۺ������� */

    uint8_t max_rf_num;                 /* оƬ��������� */
    uint8_t feature_txq_ns_support : 1; /* ֧��������Ƭ���� */
    uint8_t feature_txbf_mib_study : 1; /* �Ƿ�ѧϰ�Զ�txbf���� */
    uint8_t trigger_su_beamforming_feedback : 1;
    uint8_t trigger_mu_partialbw_feedback : 1;
    uint8_t vht_max_mpdu_lenth : 2; /* 11acЭ��06���֧�ֵ�mpdu���� */
    uint8_t resv1 : 2;
    uint8_t bfee_support_ants;          /* ����֧����෢�������� */
    uint8_t bfer_sounding_dimensions;   /* ����soundingά�� */

    uint8_t vht_bfee_ntx_supports;      /* vht ֧�ֽ������� */
    uint8_t rx_stbc_160;                /* ����stbc���� */
    uint8_t num_different_channels;     /* оƬ֧�ֵ����ͬ�ŵ��� */
    uint8_t uc_resv[1];                 /* ���봦�� */
} oal_wlan_cfg_stru;

/* ȫ�ֱ������� */
#ifndef _PRE_LINUX_TEST
extern const oal_wlan_cfg_stru *g_wlan_spec_cfg;
#else
extern oal_wlan_cfg_stru *g_wlan_spec_cfg;
#endif
/* �������� */
extern int32_t oal_main_init(void);
extern void oal_main_exit(void);
extern uint8_t oal_chip_get_device_num(uint32_t ul_chip_ver);
extern uint8_t oal_board_get_service_vap_start_id(void);

#endif /* end of oal_ext_if.h */