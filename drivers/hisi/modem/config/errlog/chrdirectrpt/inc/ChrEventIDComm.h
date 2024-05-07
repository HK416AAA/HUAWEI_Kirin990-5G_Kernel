

#ifndef __ERRLOGCOMM_H__
#define __ERRLOGCOMM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
1 Include Headfile
*****************************************************************************/
#include  "product_config.h"
#include  "vos.h"

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

/*****************************************************************************
2 macro
*****************************************************************************/

/*DEFINE MODEM ALARMID BASE BEGIN*/
/* ֱͨ�¼�ö�ٷֶΣ���Ч��Χ 0~999����1000���� */
#define COMM_EventID_ENUM_BASE                              (0)
/* ֱͨ�¼���Ϊ�����¼���ǹ����¼��������¼�ͳһ���Σ��ǹ����¼���������䡣 */
/* ֱͨ�����¼���0~53����54���� */
#define COMM_RELATION_EVENTID_ENUM_BASE                     (COMM_EventID_ENUM_BASE+0)
/* ֱͨ�ǹ����¼���54~127����74���� */
#define COMM_ABSOLUTE_EVENTID_ENUM_BASE                     (COMM_EventID_ENUM_BASE+54)
/* ֱͨ�ǹ����¼�������ֶ���ʼ����, ����PS COM :10, TLPS:20(TLRRC:12, TL2:3, LNAS:5), GUC:24, PHY:20 */
/* �����ǹ���ֱͨЭ��ջ�����¼����¼�ID��ԭ�����¼���ȡ10����ԭ�ǹ����¼��������ά�ֲ��䡣 */
#define COMM_PSCOM_EVENTID_ENUM_BASE                        (COMM_ABSOLUTE_EVENTID_ENUM_BASE+0)
#define COMM_TLRRC_EVENTID_ENUM_BASE                        (COMM_ABSOLUTE_EVENTID_ENUM_BASE+10)
#define COMM_TL2_EVENTID_ENUM_BASE                          (COMM_ABSOLUTE_EVENTID_ENUM_BASE+22)
#define COMM_LNAS_EVENTID_ENUM_BASE                         (COMM_ABSOLUTE_EVENTID_ENUM_BASE+25)
#define COMM_GUC_EVENTID_ENUM_BASE                          (COMM_ABSOLUTE_EVENTID_ENUM_BASE+30)
#define COMM_PHY_EVENTID_ENUM_BASE                          (COMM_ABSOLUTE_EVENTID_ENUM_BASE+54)
#define COMM_NRRC_EVENTID_ENUM_BASE                         (COMM_ABSOLUTE_EVENTID_ENUM_BASE+74)

/* ����ģ���ֱͨ���¼�ö�ٷֶ� */
#define LNAS_SUBEVENTID_ENUM_BASE                           (1000)
#define GUCNAS_SUBEVENTID_ENUM_BASE                         (1250) /* GUC����ģ��ʹ�� */

#define GUNAS_SUBEVENTID_ENUM_BASE                          (1500) /* GUģ��ʹ�� */
#define CNAS_SUBEVENTID_ENUM_BASE                           (1750) /* Cģ��ʹ�� */
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
#define CAS_SUBEVENTID_ENUM_BASE                            (2000)
#endif
#define LRRC_SUBEVENTID_ENUM_BASE                           (2250)
#define LL2_SUBEVENTID_ENUM_BASE                            (2500)
#define LPHY_SUBEVENTID_ENUM_BASE                           (2750)

#define TRRC_SUBEVENTID_ENUM_BASE                           (3000)
#define TL2_SUBEVENTID_ENUM_BASE                            (3250)
#define TPHY_SUBEVENTID_ENUM_BASE                           (3500)

#define WRRC_SUBEVENTID_ENUM_BASE                           (3750)
#define WL2_SUBEVENTID_ENUM_BASE                            (4000)
#define WPHY_SUBEVENTID_ENUM_BASE                           (4250)

#define GAS_SUBEVENTID_ENUM_BASE                            (4500)
#define GL2_SUBEVENTID_ENUM_BASE                            (4750)
#define GPHY_SUBEVENTID_ENUM_BASE                           (5000)

#define CRRC_SUBEVENTID_ENUM_BASE                           (5250)
#define CL2_SUBEVENTID_ENUM_BASE                            (5500)
#define CPHY_SUBEVENTID_ENUM_BASE                           (5750)
#define NRRC_SUBEVENTID_ENUM_BASE                           (6000)

#define NRNAS_SUBEVENTID_ENUM_BASE                           6250

/*DEFINE MODEM ALARMID BASE END*/

/*LNAS �ڲ�ģ�����*/
#define LNAS_EMM_SUBEVENTID_ENUM_BASE          (LNAS_SUBEVENTID_ENUM_BASE)
#define LNAS_ESM_SUBEVENTID_ENUM_BASE          (LNAS_SUBEVENTID_ENUM_BASE+50)
#define LNAS_IMSA_SUBEVENTID_ENUM_BASE         (LNAS_SUBEVENTID_ENUM_BASE+100)
#define LNAS_LCS_SUBEVENTID_ENUM_BASE          (LNAS_SUBEVENTID_ENUM_BASE+150)
#define LNAS_LPP_SUBEVENTID_ENUM_BASE          (LNAS_SUBEVENTID_ENUM_BASE+170)


/*****************************************************************************
3 Massage Declare
*****************************************************************************/

/*****************************************************************************
 4 ENUM
*****************************************************************************/

/*******************************************************************************************************************
 --------------------------------------------------�¼�����------------------------------------------------------
*******************************************************************************************************************/

/*****************************************************************************
 ö����    : MODEM_COMM_ALARMID_ENUM_UINT8
 Э����  :
 ASN.1���� :
 ö��˵��  : �����¼��澯����ö�ٶ��壬��������:
            1�������¼��澯����ö�ٶ���
            2���ǹ����¼��澯����ö�����Ͷ���(�ǹ����¼�����������¼���
            ���¼��Ƕ���ǹ������¼��ļ���)

*****************************************************************************/
enum MODEM_COMM_EVENTID_ENUM
{
    /**************************************** ��������¼�EventID**********************************************/
    TDS_OOS_INFO_REPORT_FAULTID                        = COMM_RELATION_EVENTID_ENUM_BASE+0,



    /**************************************** ����ǹ����¼�EventID**********************************************/
    /* ����TLRRC�ǹ����¼�EventID */
    TRRC_STATISTIC_INFO_COLLECTION_EVENTID             = COMM_TLRRC_EVENTID_ENUM_BASE+0,
    LRRC_STATISTIC1_INFO_COLLECTION_EVENTID            = COMM_TLRRC_EVENTID_ENUM_BASE+1,
    LRRC_STATISTIC2_INFO_COLLECTION_EVENTID            = COMM_TLRRC_EVENTID_ENUM_BASE+2,
    LRRC_VOLTE_FAULT_AUTOAN_IND_EVENTID                = COMM_TLRRC_EVENTID_ENUM_BASE+3,
    LPS_VOLTE_STAT_IND_EVENTID                         = COMM_TLRRC_EVENTID_ENUM_BASE+4,

    /* ����TL�ǹ����¼�EventID */
    TMAC_STATISTIC_INFO_COLLECTION_EVENTID             = COMM_TL2_EVENTID_ENUM_BASE+0,
    LL2_STATISTIC_INFO_COLLECTION_EVENTID              = COMM_TL2_EVENTID_ENUM_BASE+1,

    /* ����LANS�ǹ����¼�EventID */
    LNAS_STATISTIC_INFO_COLLECTION_EVENTID             = COMM_LNAS_EVENTID_ENUM_BASE+0,

    /* ����GAS�ǹ����¼�EventID */
    GAS_STATISTIC_INFO_COLLECTION_EVENTID              = COMM_GUC_EVENTID_ENUM_BASE+0,

    /* ������Դ�˲�ǹ����¼�EventID */
    PS_RADIO_RESOURCE_CHECK_EVENTID                    = COMM_PSCOM_EVENTID_ENUM_BASE+1,
    /* ����WAS�ǹ����¼�EventID */
    WAS_STATISTIC_INFO_COLLECTION_EVENTID              = COMM_GUC_EVENTID_ENUM_BASE + 1,
    /* ����GUCNAS�ǹ����¼�EventID */
    GUCNAS_STATISTIC_INFO_COLLECTION_EVENTID            = COMM_GUC_EVENTID_ENUM_BASE + 2,

    GUNAS_STATISTIC_INFO_COLLECTION_EVENTID              = COMM_GUC_EVENTID_ENUM_BASE + 3,

    CNAS_STATISTIC_INFO_COLLECTION_EVENTID               = COMM_GUC_EVENTID_ENUM_BASE + 4,

    NRRC_STATISTIC_INFO_COLLECTION_EVENTID               = COMM_NRRC_EVENTID_ENUM_BASE + 0,

    /* ��������ǹ����¼�EventID�ڴ˶��� */

    COMM_ERRORLOG_EVENTID_TYPE_BUTT
};
typedef VOS_UINT8  MODEM_COMM_EVENTID_ENUM_UINT8;


/*******************************************************************************************************************
 --------------------------------------------------���¼�����------------------------------------------------------
*******************************************************************************************************************/
/*****************************************************************************
 ö����    : MODEM_GAS_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : GAS�澯�������¼�ö�ٶ���
*****************************************************************************/
enum MODEM_GAS_SUBEVENTID_ENUM
{
    GAS_NOT_SUPP_BSS_PAGING_COOR_CHR_RPT_STRU_ALARMID     = GAS_SUBEVENTID_ENUM_BASE+1,
    GAS_RADIO_RESOURCE_CHECK_CHR_RPT_STRU_ALARMID         = GAS_SUBEVENTID_ENUM_BASE+2,
    GAS_CUSTOM_C1_CELL_CHR_RPT_STRU_ALARMID               = GAS_SUBEVENTID_ENUM_BASE+3,
    GAS_ERR_LOG_EVT_NET_WORK_FEATURE_RPT_STRU_ALARMID     = GAS_SUBEVENTID_ENUM_BASE+4,
    GAS_ERR_LOG_EVT_GSM_CELL_CALL_TIMES_RPT_STRU_ALARMID  = GAS_SUBEVENTID_ENUM_BASE+5,

    GAS_AP_G2L_RESEL_HIT_STATISTICS_STRU_ALARMID          = GAS_SUBEVENTID_ENUM_BASE+6,

    GAS_AP_G2L_PING_PONG_RESTRAIN_RECORD_RPT_STRU_ALARMID = GAS_SUBEVENTID_ENUM_BASE+7,

    GAS_AP_GSM_AIR_MSG_ABNORMAL_STRU_ALARMID              = GAS_SUBEVENTID_ENUM_BASE+8,

    GAS_AP_GU_PING_PONG_RESTRAIN_RECORD_RPT_STRU_ALARMID  = GAS_SUBEVENTID_ENUM_BASE+9,

    GAS_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_GAS_SUBEVENTID_ENUM_UINT16;

/*****************************************************************************
 ö����    : MODEM_GUCNAS_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : GUCNAS���¼�ö�ٶ���
*****************************************************************************/
enum MODEM_GUCNAS_SUBEVENTID_ENUM
{
    GUCNAS_RADIO_RESOURCE_CHECK_CHR_RPT_STRU_ALARMID       = GUCNAS_SUBEVENTID_ENUM_BASE+1,
    
    GUNAS_SS_DECODE_EXCEPT_CHR_RPT_STRU_ALARMID           = GUNAS_SUBEVENTID_ENUM_BASE+2,
    GUNAS_SEARCHED_PLMN_INFO_CHR_RPT_STRU_ALARMID         = GUNAS_SUBEVENTID_ENUM_BASE+3,
    GUNAS_SILENT_SMS_INFO_CHR_RPT_STRU_ALARMID            = GUNAS_SUBEVENTID_ENUM_BASE+4,

    GUNAS_APS_PS_CALL_ENTITY_CHR_RPT_STRU_ALARMID         = GUNAS_SUBEVENTID_ENUM_BASE+5,
    GUNAS_SRVCC_TIFLAG_ERR_CHR_RPT_STRU_ALARMID           = GUNAS_SUBEVENTID_ENUM_BASE+6,
    GUNAS_RECEIVE_SMS_IN_ANTI_ATTACK_INFO_CHR_RPT_STRU_ALARMID = GUNAS_SUBEVENTID_ENUM_BASE+7,
    GUNAS_SMS_ANTI_ATTACK_INFO_CHR_RPT_STRU_ALARMID            = GUNAS_SUBEVENTID_ENUM_BASE+8,
    GUNAS_READ_6FAD_USIM_FILE_ABNORMAL_CHR_RPT_STRU_ALARMID = GUNAS_SUBEVENTID_ENUM_BASE+9,
    GUNAS_AUTH_TYPE_ADAPTATION_CHR_RPT_STRU_ALARMID = GUNAS_SUBEVENTID_ENUM_BASE+10,
    GUCNAS_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_GUCNAS_SUBEVENTID_ENUM_UINT16;



enum MODEM_NRNAS_SUBEVENTID_ENUM
{
    NRNAS_NRMM_DECODE_MSG_ERR_CHR_RPT_STRU_ALARMID     = NRNAS_SUBEVENTID_ENUM_BASE + 1,
    NRNAS_NRMM_ESCAPE_CHR_RPT_STRU_ALARMID             = NRNAS_SUBEVENTID_ENUM_BASE + 2,
    NRNAS_PCF_DECODE_MSG_ERR_CHR_RPT_STRU_ALARMID      = NRNAS_SUBEVENTID_ENUM_BASE + 3,
    NRNAS_NRSM_DECODE_MSG_ERR_CHR_RPT_STRU_ALARMID     = NRNAS_SUBEVENTID_ENUM_BASE + 4,
    NRNAS_NRSM_COMPATIBILITY_CHR_RPT_STRU_ALARMID      = NRNAS_SUBEVENTID_ENUM_BASE + 5,
    NRNAS_NRMM_REG_ABNORMAL_STRU_ALARMID               = NRNAS_SUBEVENTID_ENUM_BASE + 6,
    NRNAS_NRMM_COMPATIBILITY_CHR_RPT_STRU_ALARMID      = NRNAS_SUBEVENTID_ENUM_BASE + 7,
    NRNAS_NRSM_AIR_MSG_RETRANSMIT_CHR_RPT_STRU_ALARMID = NRNAS_SUBEVENTID_ENUM_BASE + 8,
    NRNAS_NRMM_REG_PROC_CHR_RPT_STRU_ALARMID           = NRNAS_SUBEVENTID_ENUM_BASE + 9,
    NRNAS_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16 MODEM_NRNAS_SUBEVENTID_ENUM_UINT16;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
/*****************************************************************************
 ö����    : MODEM_CAS_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : CAS���¼�ö�ٶ���
*****************************************************************************/
enum MODEM_CAS_SUBEVENTID_ENUM
{
    CAS_1X_RADIO_RESOURCE_CHECK_CHR_RPT_STRU_ALARMID       = CAS_SUBEVENTID_ENUM_BASE+1,
    CAS_HRPD_RADIO_RESOURCE_CHECK_CHR_RPT_STRU_ALARMID     = CAS_SUBEVENTID_ENUM_BASE+2,

    CAS_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_CAS_SUBEVENTID_ENUM_UINT16;
#endif
/*****************************************************************************
 ö����    : MODEM_WAS_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : WAS�澯�������¼�ö�ٶ���
*****************************************************************************/
enum MODEM_WAS_SUBEVENTID_ENUM
{
    WAS_RRC_ACCESS_LONG_CHR_RPT_STRU_ALARMID          = WRRC_SUBEVENTID_ENUM_BASE+1,
    WAS_RADIO_RESOURCE_CHECK_CHR_RPT_STRU_ALARMID     = WRRC_SUBEVENTID_ENUM_BASE+2,
    WAS_RRC_STATE_LAST_TIME_CHR_RPT_STRU_ALARMID      = WRRC_SUBEVENTID_ENUM_BASE+3,
    WAS_WOOS_SEARCH_SECTION_CHR_RPT_STRU_ALARMID      = WRRC_SUBEVENTID_ENUM_BASE+4,
    WAS_W2W_RESEL_SLOW_CHR_RPT_STRU_ALARMID           = WRRC_SUBEVENTID_ENUM_BASE+5,
    WAS_NET_WORK_FEATURE_CHR_RPT_STRU_ALARMID         = WRRC_SUBEVENTID_ENUM_BASE+6,
    WAS_DCH_TRIGGER_CU_CHR_RPT_STRU_ALARMID           = WRRC_SUBEVENTID_ENUM_BASE+7,
    WAS_CS_END_CESSION_NOTIFY_CHR_RPT_STRU_ALARMID    = WRRC_SUBEVENTID_ENUM_BASE+8,

    WAS_UL_PINGPONG_RESTRAIN_CHR_RPT_STRU_ALARMID     = WRRC_SUBEVENTID_ENUM_BASE+9,

    WAS_RNC_RELOC_FAIL_CHR_RPT_STRU_ALARMID           = WRRC_SUBEVENTID_ENUM_BASE+10,

    WAS_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_WAS_SUBEVENTID_ENUM_UINT16;

/*****************************************************************************
 ö����    : MODEM_TRRC_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : TRRC�澯�������¼�ö�ٶ���
*****************************************************************************/
enum MODEM_TRRC_SUBEVENTID_ENUM
{
    TRRC_OUT_OF_SYNC_CELL_UPDATE_INFO_STRU_ALARMID     = TRRC_SUBEVENTID_ENUM_BASE+1,
    TRRC_OOS_INFO_STRU_ALARMID                         = TRRC_SUBEVENTID_ENUM_BASE+2,
    TRRC_APP_COLLECT_TDS_FREQ_REPORT_STRU_ALARMID      = TRRC_SUBEVENTID_ENUM_BASE+3,

    TRRC_CS_CALL_EST_CNF_INFO_STRU_ALARMID             = TRRC_SUBEVENTID_ENUM_BASE+4,
    TRRC_CS_CALL_CONN_REQ_INFO_STRU_ALARMID            = TRRC_SUBEVENTID_ENUM_BASE+5,
    TRRC_CS_CALL_AIRMSG_INFO_STRU_ALARMID              = TRRC_SUBEVENTID_ENUM_BASE+6,
    TRRC_APP_CONFIG_PCH_CELL_INFO_REPORT_STRU_ALARMID  = TRRC_SUBEVENTID_ENUM_BASE+7,

    /*Add by lilin ��Դ�˲�CHR 2017-3-13 begin*************/
    TRRC_CHR_RRM_RESOURCE_CHECK_INFO_LIST_STRU_ALARMID = TRRC_SUBEVENTID_ENUM_BASE+8,
    /*Add by lilin ��Դ�˲�CHR 2017-3-13 end*************/

    TRRC_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_TRRC_SUBEVENTID_ENUM_UINT16;

/*****************************************************************************
 ö����    : MODEM_LNAS_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : LNAS�澯�������¼�ö�ٶ���
*****************************************************************************/
enum MODEM_LNAS_SUBEVENTID_ENUM
{
    LNAS_EMM_ERR_INFO_DSDS_RRM_RF_CHECKIN_STRU_ALARMID         = LNAS_EMM_SUBEVENTID_ENUM_BASE+1,
    LNAS_EMM_ERR_INFO_LTE_FAKE_NODEB_STRU_ALARMID              = LNAS_EMM_SUBEVENTID_ENUM_BASE+2,
    LNAS_EMM_ERR_INFO_LOAD_BALANCE_STRU_ALARMID                = LNAS_EMM_SUBEVENTID_ENUM_BASE+3,
    LNAS_EMM_ERR_INFO_DSDS_RF_RECOVER_RETRY_STRU_ALARMID       = LNAS_EMM_SUBEVENTID_ENUM_BASE+4,
    LNAS_EMM_ERR_INFO_GRADUAL_FORBIDDEN_OPTIMIZE_STRU_ALARMID  = LNAS_EMM_SUBEVENTID_ENUM_BASE+5,
    LNAS_EMM_ERR_INFO_VOLTE_WITH_TA_CHANGE_STRU_ALARMID        = LNAS_EMM_SUBEVENTID_ENUM_BASE+6,
    LNAS_EMM_ERR_INFO_NO_RAU_WITH_TAU_STRU_ALARMID             = LNAS_EMM_SUBEVENTID_ENUM_BASE+7,
    LNAS_EMM_ERR_INFO_IMS_BEARER_WITH_CN_MOD_CONFLICT_STRU_ALARMID  = LNAS_EMM_SUBEVENTID_ENUM_BASE+8,
    LNAS_EMM_ERR_INFO_3440_TIMEOUT_DO_NOT_REL_STRU_ALARMID          = LNAS_EMM_SUBEVENTID_ENUM_BASE+9,
    LNAS_EMM_ERR_INFO_CNMSG_DECODE_FAIL_STRU_ALARMID                = LNAS_EMM_SUBEVENTID_ENUM_BASE+10,
    LNAS_EMM_ERR_INFO_SMC_FAIL_STRU_ALARMID                         = LNAS_EMM_SUBEVENTID_ENUM_BASE+11,
    LNAS_EMM_ERR_INFO_LOCAL_DETACH_STRU_ALARMID                     = LNAS_EMM_SUBEVENTID_ENUM_BASE+12,
    LNAS_EMM_ERR_INFO_T3402_DEACTIVED_STRU_ALARMID                  = LNAS_EMM_SUBEVENTID_ENUM_BASE+13,
    LNAS_EMM_ERR_INFO_ATT_FAIL_ESM_NEED_DISABLE_LTE_STRU_ALARMID    = LNAS_EMM_SUBEVENTID_ENUM_BASE+14,
    LNAS_EMM_ERR_INFO_MMC_REL_DRGINIT_STRU_ALARMID                  = LNAS_EMM_SUBEVENTID_ENUM_BASE+15,
    LNAS_EMM_ERR_INFO_MO_BAR_ATTAMPT_STRU_ALARMID                   = LNAS_EMM_SUBEVENTID_ENUM_BASE+16,
    LNAS_ESM_ERR_INFO_SWITCH_APN_STRU_ALARMID                       = LNAS_EMM_SUBEVENTID_ENUM_BASE+17,
    LNAS_ESM_ERR_INFO_CNMSG_DECODE_FAIL_STRU_ALARMID                = LNAS_EMM_SUBEVENTID_ENUM_BASE+18,
    LNAS_IMSA_ERR_INFO_EMC_TCALL_TIMEOUT_STRU_ALARMID               = LNAS_EMM_SUBEVENTID_ENUM_BASE+19,
    LNAS_IMSA_ERR_INFO_DSDS_WAIT_BEAR_REL_TIMEOUT_STRU_ALARMID      = LNAS_EMM_SUBEVENTID_ENUM_BASE+20,
    LNAS_IMSA_ERR_INFO_DSDS_SMS_RF_OCCUPY_STRU_ALARMID              = LNAS_EMM_SUBEVENTID_ENUM_BASE+21,
    LNAS_IMSA_ERR_INFO_WAIT_RETRY_START_SWITCH_STRU_ALARMID         = LNAS_EMM_SUBEVENTID_ENUM_BASE+22,
    LNAS_IMSA_ERR_INFO_HANDOVER_INITIAL_PDN_CONN_STRU_ALARMID       = LNAS_EMM_SUBEVENTID_ENUM_BASE+23,
    LNAS_IMSA_ERR_INFO_REG_REJ_WAIT_RECOVERY_STRU_ALARMID           = LNAS_EMM_SUBEVENTID_ENUM_BASE+24,
    LNAS_EMM_ERR_INFO_DETACH_NO_RF_STRU_ALARMID                     = LNAS_EMM_SUBEVENTID_ENUM_BASE+25,
    LNAS_EMM_ERR_INFO_PERIODIC_TAU_WITH_CAUSE_STRU_ALARMID          = LNAS_EMM_SUBEVENTID_ENUM_BASE+26,
    LNAS_EMM_ERR_INFO_LANZHOU_CSFB_OPTIMIZE_STRU_ALARMID            = LNAS_EMM_SUBEVENTID_ENUM_BASE+27,
    LNAS_EMM_ERR_INFO_START_3402_ADD_FORB_LIST_STRU_ALARMID         = LNAS_EMM_SUBEVENTID_ENUM_BASE+28,
    LNAS_EMM_ERR_INFO_IMPROVE_CALL_SUCC_RATIO_STRU_ALARMID          = LNAS_EMM_SUBEVENTID_ENUM_BASE+29,
    LNAS_EMM_ERR_INFO_TAU_EST_WITH_CELL_SEARCHING_OPT_STRU_ALARMID  = LNAS_EMM_SUBEVENTID_ENUM_BASE+30,
    LNAS_EMM_ERR_INFO_AUTH_REJ_OPT_STRU_ALARMID                     = LNAS_EMM_SUBEVENTID_ENUM_BASE+31,
    LNAS_EMM_ERR_INFO_TAU_WITH_CONN_REL_DO_NOT_WAIT_3411_TIMEOUT_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE+32,
    LNAS_ESM_ERR_INFO_ESM_MSG_RETRANSMITE_STRU_ALARMID              = LNAS_EMM_SUBEVENTID_ENUM_BASE+33,
    LNAS_EMM_ERR_INFO_EMC_DATA_REQ_TAU_STRU_ALARMID                 = LNAS_EMM_SUBEVENTID_ENUM_BASE+34,
    LNAS_EMM_ERR_INFO_EMC_BERER_CN_REJ_ANYCELL_SRCH_STRU_ALARMID    = LNAS_EMM_SUBEVENTID_ENUM_BASE+35,
    LNAS_OM_ERRLOG_CSS_BAND_LIST_INFO_STRU_ALARMID                  = LNAS_EMM_SUBEVENTID_ENUM_BASE+36,
    LNAS_EMM_ERR_INFO_CSFB_TAU_RCV_MT_DETACH_STRU_ALARMID           = LNAS_EMM_SUBEVENTID_ENUM_BASE+37,
    LNAS_EMM_ERR_INFO_MT_DETACH_OPTIMIZE_STRU_ALARMID               = LNAS_EMM_SUBEVENTID_ENUM_BASE+38,
    LNAS_EMM_ERR_INFO_MT_SERVICE_RETRY_STRU_ALARMID                 = LNAS_EMM_SUBEVENTID_ENUM_BASE+39,
    LNAS_EMM_ERR_INFO_DSDS_NO_RF_TAU_PUNISH_STRU_ALARMID            = LNAS_EMM_SUBEVENTID_ENUM_BASE+40,
    LNAS_EMM_ERR_INFO_DSDS_BUFFER_NO_RF_TAU_REQ_STRU_ALARMID        = LNAS_EMM_SUBEVENTID_ENUM_BASE+41,
    LNAS_EMM_ERR_INFO_RF_OCCUPY_MODEM1_TAU_STRU_ALARMID             = LNAS_EMM_SUBEVENTID_ENUM_BASE+42,
    LNAS_EMM_ERR_INFO_REATTACH_PUNISH_CHR_STRU_ALARMID           = LNAS_EMM_SUBEVENTID_ENUM_BASE+43,
    LNAS_EMM_ERR_INFO_DEEP_GAME_CHR_INFO_STRU_ALARMID           = LNAS_EMM_SUBEVENTID_ENUM_BASE+44,
    LNAS_EMM_ERR_INFO_ATTACH_TAU_FAIL_MAXTIMES_CHR_STRU_ALARMID             = LNAS_EMM_SUBEVENTID_ENUM_BASE+45,
    LNAS_ESM_ERR_INFO_ESM65_CHR_INFO_STRU_ALARMID                           = LNAS_EMM_SUBEVENTID_ENUM_BASE+46,
    LNAS_EMM_ERR_INFO_ESCAPE_CHR_INFO_STRU_ALARMID                           = LNAS_EMM_SUBEVENTID_ENUM_BASE+47,
    LNAS_ESM_ERR_INFO_LCSLPP_LEN_CHR_INFO_STRU_ALARMID                       = LNAS_EMM_SUBEVENTID_ENUM_BASE+48,
    LNAS_ESM_ERR_INFO_AIR_COMPATI_CHR_INFO_STRU_ALARMID                      = LNAS_EMM_SUBEVENTID_ENUM_BASE+49,
    LNAS_ERABM_ERR_INFO_BEAR_ACT_WITHOUT_DRB_INFO_STRU_ALARMID               = LNAS_EMM_SUBEVENTID_ENUM_BASE+50,
    LNAS_EMM_ERR_INFO_HO_WITHIN_TAU_PROCESS_INFO_STRU_ALARMID                = LNAS_EMM_SUBEVENTID_ENUM_BASE+51,
    LNAS_LMM_OM_DL_NASCOUNT_ERR_CHR_STRU_ALARMID                             = LNAS_EMM_SUBEVENTID_ENUM_BASE+52,
    LNAS_ESM_ERR_INFO_BEARER_STATUS_INCONSISTENT_STRU_ALARMID                = LNAS_EMM_SUBEVENTID_ENUM_BASE + 53,
    LNAS_EMM_ERR_INFO_REPEATEDLY_CONN_RECOVER_TAU_EFFECT_STRU_ALARMID        = LNAS_EMM_SUBEVENTID_ENUM_BASE+54,
    LNAS_IMSA_ERR_INFO_RELEASE_SIGNAL_BEAR_STRU_ALARMID    = LNAS_EMM_SUBEVENTID_ENUM_BASE+55,
    LNAS_EMM_ERR_INFO_ESCAPE_DESCEND_CAP_INFO_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE+56,
    LNAS_EMM_ERR_INFO_ROBUST_OPTIMIZATION_STATISTICS_INFO_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE+57,
    LNAS_ESM_ERR_INFO_CHANGE_NULL_APN_OPT_STRU_ALARMID        = LNAS_EMM_SUBEVENTID_ENUM_BASE + 58,
    LNAS_EMM_ERR_INFO_LRRC_SMC_FAIL_STRU_ALARMID              = LNAS_EMM_SUBEVENTID_ENUM_BASE + 59,
    LNAS_ESM_ERR_INFO_5G_ESCAPE_DESCEND_CAP_INFO_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE + 60,
    LNAS_EMM_ERR_INFO_COMPATIBILITY_STRU_ALARMID              = LNAS_EMM_SUBEVENTID_ENUM_BASE + 61,
    LNAS_ESM_ERR_INFO_ESM_STATUS_NOT_EQ_LMM_STRU_ALARMID      = LNAS_EMM_SUBEVENTID_ENUM_BASE + 62,
    LNAS_EMM_ERR_INFO_LTE_FAKE_NODEB_OPTIMIZATION_STATISTICS_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE + 63,
    LNAS_EMM_ERR_INFO_INITIATE_TAU_OPTIMIZATION_STATISTICS_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE + 64,
    LNAS_IMSA_ERR_INFO_NR_EST_FAIL_EVENT_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE + 65,
    LNAS_IMSA_ERR_INFO_IMPLICIT_DETACH_EMC_REDIAL_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE + 66,
    LNAS_IMSA_ERR_INFO_MEDIA_PDP_LOST_EVENT_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE + 67,
    LNAS_EMM_ERR_INFO_N2L_HO_ABNORMAL_REL_STATISTICS_STRU_ALARMID = LNAS_EMM_SUBEVENTID_ENUM_BASE + 78,
    LNAS_ERR_INFO_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_LNAS_SUBEVENTID_ENUM_UINT16;

/*****************************************************************************
 ö����    : MODEM_TRRC_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : TRRC�澯�������¼�ö�ٶ���
*****************************************************************************/
enum MODEM_LRRC_SUBEVENTID_ENUM
{
    LPS_OM_VOLTE_FAULT_AUTOAN_INFO_STRU_ALARMID     = LRRC_SUBEVENTID_ENUM_BASE+1,
    LPS_OM_VOLTE_GENERAL_STAT_INFO_STRU_ALARMID      = LRRC_SUBEVENTID_ENUM_BASE+2,
    LRRC_OM_UE_CAP_FALL_REPORT_STRU_ALARMID          = LRRC_SUBEVENTID_ENUM_BASE+3,
    LRRC_OM_USER_PLANE_ERR_REL_REPORT_STRU_ALARMID   = LRRC_SUBEVENTID_ENUM_BASE+4,
    LRRC_CHR_RRM_RES_CHECK_INFO_LIST_STRU_ALARMID     = LRRC_SUBEVENTID_ENUM_BASE+5,
    LRRC_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_LRRC_SUBEVENTID_ENUM_UINT16;

/*****************************************************************************
 ö����    : MODEM_TLL2_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : TLL2�澯�������¼�ö�ٶ���
*****************************************************************************/
enum MODEM_TLL2_SUBEVENTID_ENUM
{
    TLL2_OOS_INFO_STRU_ALARMID                        = TL2_SUBEVENTID_ENUM_BASE+1,
    /*niuxiufan preamble modify begin */
    LMAC_OM_PREAMBLE_SPEC_NOT_MATCH_INFO_STRU_ALARMID,         /*Preamble��ƥ��ֱͨCHR */
    /*niuxiufan preamble modify end */

    TLL2_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_TLL2_SUBEVENTID_ENUM_UINT16;

/*****************************************************************************
 ö����    : MODEM_CL2_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : CL2���¼�ö�ٶ���
*****************************************************************************/
enum MODEM_CL2_SUBEVENTID_ENUM
{
    CTTF_RADIO_RESOURCE_CHECK_CHR_DIRECT_RPT_STRU_ALARMID   = CL2_SUBEVENTID_ENUM_BASE+1,

    CTTF_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_CL2_SUBEVENTID_ENUM_UINT16;


/*****************************************************************************
 ö����    : MODEM_NRRC_SUBEVENTID_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : NRRC���¼�ö�ٶ���
*****************************************************************************/
enum MODEM_NRRC_SUBEVENTID_ENUM
{
    NRRC_CHR_DIRECT_OTA_CHECK_FAIL_STRU_ALARMID             = NRRC_SUBEVENTID_ENUM_BASE+1,
    NRRC_CHR_DIRECT_OUT_OF_SYNC_STRU_ALARMID                = NRRC_SUBEVENTID_ENUM_BASE+2,
    NRRC_CHR_DIRECT_RA_FAIL_STRU_ALARMID                    = NRRC_SUBEVENTID_ENUM_BASE+3,
    NRRC_CHR_DIRECT_RADIO_RES_CHECK_STRU_ALARMID            = NRRC_SUBEVENTID_ENUM_BASE+4,
    NRRC_CHR_DIRECT_BAND_CNT_OVER_LMT_STRU_ALARMID          = NRRC_SUBEVENTID_ENUM_BASE+5,
    NRRC_CHR_DIRECT_NW_PARA_ABNORMAL_STRU_ALARMID           = NRRC_SUBEVENTID_ENUM_BASE+6,
    NRRC_CHR_DIRECT_EXTREME_SCENE_STRU_ALARMID              = NRRC_SUBEVENTID_ENUM_BASE+7,
    NRRC_CHR_DIRECT_HPS_RPT_STRU_ALARMID                    = NRRC_SUBEVENTID_ENUM_BASE+8,
    NRRC_CHR_DIRECT_BAD_CELL_STRU_ALARMID                   = NRRC_SUBEVENTID_ENUM_BASE+9,
    NRRC_CHR_DIRECT_UNEXP_MSG_STRU_ALARMID                  = NRRC_SUBEVENTID_ENUM_BASE+10,
    NRRC_CHR_DIRECT_UECAP_ENQ_ABNORMAL_STRU_ALARMID         = NRRC_SUBEVENTID_ENUM_BASE+11,
    NRRC_CHR_DIRECT_ASN_ENCODE_FAIL_STRU_ALARMID            = NRRC_SUBEVENTID_ENUM_BASE+12,
    NRRC_CHR_DIRECT_UECAP_PROC_ERROR_STRU_ALARMID             = NRRC_SUBEVENTID_ENUM_BASE+13,
    NRRC_CHR_DIRECT_OBTAION_UECAP_SRC_DATA_ERROR_STRU_ALARMID = NRRC_SUBEVENTID_ENUM_BASE+14,
    NRRC_CHR_DIRECT_UECAP_NW_ACT_ABNORMAL_STRU_ALARMID        = NRRC_SUBEVENTID_ENUM_BASE+15,
    NRRC_CHR_DIRECT_CONTINUAL_REEST_STRU_ALARMID            = NRRC_SUBEVENTID_ENUM_BASE+16,
    NRRC_CHR_DIRECT_N2L_EPSFB_STRU_ALARMID                  = NRRC_SUBEVENTID_ENUM_BASE+17,
    NRRC_CHR_DIRECT_L2N_REDIR_FAIL_STRU_ALARMID             = NRRC_SUBEVENTID_ENUM_BASE+18,
    NRRC_ERRORLOG_ALARMID_BUTT
};
typedef VOS_UINT16  MODEM_NRRC_SUBEVENTID_ENUM_UINT16;

/*****************************************************************************
 5 STRUCT
*****************************************************************************/

/*****************************************************************************
6 UNION
*****************************************************************************/

/*****************************************************************************
7 Extern Global Variable
*****************************************************************************/

/*****************************************************************************
8 Fuction Extern
*****************************************************************************/

/*****************************************************************************
9 OTHERS
*****************************************************************************/


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#ifdef __cplusplus
  #if __cplusplus
      }
  #endif
#endif

#endif /* end of ErrLogComm.h */
