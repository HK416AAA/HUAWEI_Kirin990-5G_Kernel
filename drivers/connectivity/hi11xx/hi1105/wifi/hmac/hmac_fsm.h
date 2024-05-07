

#ifndef __HMAC_FSM_H__
#define __HMAC_FSM_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "hmac_main.h"
#include "hmac_vap.h"
#include "hmac_rx_filter.h"
#include "hmac_config.h"

#if defined(WIN32)
#include "plat_pm_wlan.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_FSM_H
/* 2 �궨�� */
typedef uint32_t (*hmac_fsm_func)(hmac_vap_stru *pst_hmac_vap, void *p_param);

uint32_t hmac_config_sta_update_rates(mac_vap_stru *pst_mac_vap,
                                      mac_cfg_mode_param_stru *pst_cfg_mode,
                                      mac_bss_dscr_stru *pst_bss_dscr);

/* 3 ö�ٶ��� */
/* ״̬����������ö�� */
typedef enum {
    /* AP STA���е��������� */
    HMAC_FSM_INPUT_RX_MGMT,
    HMAC_FSM_INPUT_RX_DATA,
    HMAC_FSM_INPUT_TX_DATA,
    HMAC_FSM_INPUT_TIMER0_OUT,
    HMAC_FSM_INPUT_MISC, /* TBTT �쳣 �������������� */

    /* AP ���е��������� */
    HMAC_FSM_INPUT_START_REQ,

    /* STA���е��������� */
    HMAC_FSM_INPUT_SCAN_REQ,
    HMAC_FSM_INPUT_JOIN_REQ,
    HMAC_FSM_INPUT_AUTH_REQ,
    HMAC_FSM_INPUT_ASOC_REQ,

    HMAC_FSM_INPUT_LISTEN_REQ, /*  �������� */
    HMAC_FSM_INPUT_LISTEN_TIMEOUT,
    HMAC_FSM_INPUT_SCHED_SCAN_REQ, /* PNO����ɨ������ */

    HMAC_FSM_INPUT_ROAMING_START,
    HMAC_FSM_INPUT_ROAMING_STOP,

    HMAC_FSM_INPUT_TYPE_BUTT
} hmac_fsm_input_type_enum;

#define HMAC_FSM_AP_INPUT_TYPE_BUTT  (HMAC_FSM_INPUT_MISC + 1)
#define HMAC_FSM_STA_INPUT_TYPE_BUTT HMAC_FSM_INPUT_TYPE_BUTT
#define HMAC_SWITCH_STA_PSM_PERIOD   120000  // staut�򿪵͹��ĳ�ʱ��ʱ��ο�1101 120s
#define HMAC_SWITCH_STA_PSM_MAX_CNT  (HMAC_SWITCH_STA_PSM_PERIOD / WLAN_SLEEP_TIMER_PERIOD)

/* MISC�������͵������Ͷ��� */
typedef enum {
    HMAC_MISC_TBTT,
    HMAC_MISC_ERROR,
    HMAC_MISC_RADAR,

    HMAC_MISC_BUTT
} hmac_misc_input_enum;

/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* MISC�������ͣ���νṹ�嶨�� */
typedef struct {
    uint8_t en_type;
    uint8_t auc_resv[3]; // ����3�ֽ�
    void *p_data;
} hmac_misc_input_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
void hmac_fsm_init(void);
uint32_t hmac_fsm_call_func_ap(hmac_vap_stru *pst_hmac_vap,
                               hmac_fsm_input_type_enum en_input,
                               void *p_param);
uint32_t hmac_fsm_call_func_sta(hmac_vap_stru *pst_hmac_vap,
                                hmac_fsm_input_type_enum en_input,
                                void *p_param);
void hmac_fsm_change_state(hmac_vap_stru *pst_hmac_vap,
                           mac_vap_state_enum_uint8 en_vap_state);
void hmac_fsm_change_state_check(hmac_vap_stru *pst_hmac_vap,
                                 mac_vap_state_enum_uint8 en_comp_vap_state,
                                 mac_vap_state_enum_uint8 en_new_vap_state);

#endif /* end of hmac_fsm.h */