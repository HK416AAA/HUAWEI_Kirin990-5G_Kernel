

/*
 * 1 ����ͷ�ļ�����
 */
#ifndef __PHYTOOLINTERFACE_H__
#define __PHYTOOLINTERFACE_H__

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/*
 * 2 �궨��
 */

/*
 * 2.2 �궨��,W/G�궨��
 */

/* ��ӡЯ���������� */
#define UPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM      5
#define UPHY_TOOL_INTER_REG_MAX_NUM           300

/* ��ά�ɲ��func typeΪ4,intra msg������Ҫ��TOOLȷ�� */
#define UPHY_TOOL_MODEM2_FUNC_TYPE_OM_MSG     0x84
#define UPHY_TOOL_MODEM2_FUNC_TYPE_INTRA_MSG  0x94

#define UPHY_TOOL_MODEM1_FUNC_TYPE_OM_MSG     0x44
#define UPHY_TOOL_MODEM1_FUNC_TYPE_INTRA_MSG  0x54

#define UPHY_TOOL_MODEM0_FUNC_TYPE_OM_MSG     0x04
#define UPHY_TOOL_MODEM0_FUNC_TYPE_INTRA_MSG  0x1A

/* Log��ӡMIPI�ض���ʾ����� */
#define UPHY_TOOL_RDBACK_MIPI_NUM             2

/* ��С���뱣�ֺ�phy_oam_interface.h��UPHY_OAM_SLEEP_INFO_NUMһ�� */
#define UPHY_TOOL_SLEEP_INFO_NUM              16

/* �ջ���г�ƽ��켣��¼���ֵ */
#define UPHY_TOOL_CLT_TRACE_NUM               8

/*
 * 2.2 �궨��,GPHY�궨��
 */

#if defined(INSTANCE_1)
#define GPHY_RF_CAL_NO_SIG_FUNC_TYPE          0x48
#define WPHY_RF_CAL_NO_SIG_FUNC_TYPE          0x07
#elif defined(INSTANCE_2)
#define GPHY_RF_CAL_NO_SIG_FUNC_TYPE          0x88
#define WPHY_RF_CAL_NO_SIG_FUNC_TYPE          0x07
#else
#define GPHY_RF_CAL_NO_SIG_FUNC_TYPE          0x08
#define WPHY_RF_CAL_NO_SIG_FUNC_TYPE          0x07
#endif

#define CPHY_RF_CAL_NO_SIG_FUNC_TYPE          0x12

/* PS����ʱ��������ʱ϶���� */
#define GPHY_PS_MAX_RX_TSN_NUM                5

/* PS����ʱ��������ʱ϶���� */
#define GPHY_PS_MAX_TX_TSN_NUM                4

#define GPHY_TOOL_SLAVE_BSIC_MAX_NUM          8

/*
 * ԭ��GPHY_TOOL_FuncBitCtrlReq ��ulFuncBitMaskÿ��bitλ�ĺ���;
 * �����Ӧbitλ��1:���Ӧ����ʧЧ;
 * �����Ӧbitλ��0:��ָ���Ӧ����;
 */

#define GPHY_FUNC_DRX_USE_DISABLE_FLG         (BIT0)
#define GPHY_FUNC_DRX_SIMPLIFY_DISABLE_FLG    (BIT1)
#define GPHY_FUNC_BA_MEA_DISABLE_FLG          (BIT2)
#define GPHY_FUNC_INTERFERE_MEA_DISABLE_FLG   (BIT3)
#define GPHY_FUNC_C1_RESEL_DISABLE_FLG        (BIT4)
#define GPHY_FUNC_C2_RESEL_DISABLE_FLG        (BIT5)
#define GPHY_FUNC_NCELL_FBSB_DISABLE_FLG      (BIT6)
#define GPHY_FUNC_NCELL_SB_DISABLE_FLG        (BIT7)
#define GPHY_FUNC_NCELL_TASK_DISABLE_FLG      (BIT8)
#define GPHY_FUNC_RF_FRONT_DISPLAY_MNTN_FLG   (BIT9)

/* ��ά�ɲ����ݿ�����ϱ����� */
#define GPHY_TOOL_RPT_BLK_MAX_NUM             10

/* ��ӡЯ���������� */
#define GPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM      5   /* �����ϱ�ʱ���綨���ϱ��������� */

/*
 * 2.3 �궨��,WPHY�궨��
 */

#define WPHY_TOOL_PA_SWITCH_COMP_SLOT         4   /* W pa�л�����4��slot */
/* ��ӡЯ���������� */
#define WPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM      5   /* �����ϱ�ʱ���綨���ϱ��������� */

/* ����С�������� */
#define WPHY_MNTN_MAX_ACT_CELL_NUM            7   /* ���С�������Ŀ */

/* fs2������߸��� */
#define WPHY_MNTN_FS2_MAX_ANT_NUM             4   /* fs2������߸��� */

/* ��ά�ɲ����ݿ�����ϱ����� */
#define WPHY_TOOL_RPT_BLK_MAX_NUM             10

#define WPHY_MNTN_TRCH_NUM                    8

#define WPHY_MNTN_SLAVE_REPORTFREQ_NUM        6   /* ��ģ��ν�ɲ��ϱ�Ƶ����� */
#define GPHY_TOOL_MA_FREQ_NUM                 64  /* ��Ƶ���еĸ��� */

#define GPHY_TOOL_BA_LIST_MAX_NUM             32  /* GSM BA LIST���ĸ��� */

#define GPHY_TOOL_AMR_MAX_ACS_NUM             4   /* ��󼤻���� */

#define GPHY_TOOL_RPT_MAX_LEN                 100

#define GPHY_TOOL_AMR_CODEC_MAX_NUM           8    /* AMR��������� */

/* �����ÿ֡���ʱ϶���� */
#define GPHY_TOOL_TN_NUM_IN_FRAME             8

#define GPHY_TOOL_MAX_UL_SLOT_NUM             4    /* Gsm��������ʱ϶���� */

#define GPHY_TOOL_MAX_IDLE_FRAME_NUM          84   /* ר��̬5S�ڿ���֡�������Ŀ */

#define GPHY_TOOL_MAX_SYNC_TASK_NUM           5    /* ר��̬����֡���������ͬ��������� */

#define GPHY_TOOL_MAX_NCELL_NUM               32   /* �ϱ���������ϵͳ�����Ŀ */

#define GPHY_TOOL_NCELL_RPT_PREDICT_TASK_INFO_NUM 5 /* ר��̬��ӡ��Ԥ������ĸ��� */

#define GPHY_TOOL_GBBPSHARE_MAX_MSG_PROC_NUM  4    /* bbp��Դ�������ҽ�ԭ��ĸ��� */

/* G�ض����ڴ��ַ����� */
#define GPHY_MNTN_READ_ADDR_MAX_NUM           300

#define WPHY_MNTN_HSUPA_SUB_FRM_NUM           5    /* HSUPA��֡�� */

#define WPHY_MNTN_HSUPA_OM_CARERIER_NUM       1    /* UPA ��ά�ɲ�ʹ�ã����������ز� */

#define WPHY_MNTN_HSDPA_OM_CARERIER_NUM       2

#define WPHY_MNTN_HSUPA_RPT_VALUE_MAX_NUM     100

#define WPHY_MNTN_RL_SCRAM_MAX_NUM            7

#define WPHY_MNTN_DEMOD_FINGER_MAX_NUM        29

#define WPHY_MNTN_EQ_FINGER_RPT_MAX_NUM       2

#define WPHY_MNTN_BLER_RPT_MAX_NUM            5

#define WPHY_MNTN_DPA_ERROR_CODE_RPT_MAX_NUM  2

#define WPHY_MNTN_MP_WIN_MAX_NUM              3

/* BCCH�������������� */
#define WPHY_MNTN_BCCH_QUEUE_LENGTH           4

/* ademod�������������� */
#define WPHY_MNTN_ADMOD_QUEUE_LENGTH          5

/* ���پ���ģ���֡�� */
#define WPHY_MNTN_CPC_DRX_PATTERN_FRAME_NUM   5

/* CPC DRX������Ƶ������� */
#define WPHY_MNTN_CPC_DRX_EQUA_CTRL_TYPE_NUM  2

/* CQI PATTERN���� */
#define WPHY_MNTN_CPC_DRX_CQI_PATTERN_NUM     3

/* CPC DRX RF ���������� */
#define WPHY_MNTN_CPC_DRX_RF_CTRL_TYPE_NUM    6

#define WPHY_MNTN_SLAVE_CS_SUCC_CELL_NUM      6

/* ���ICС������ */
#define WPHY_MNTN_IC_MAX_CELL_NUM             2

/* DPA��Pֵ��Ŀ+1��ֱ��ʹ��Pֵ���±� */
#define WPHY_MNTN_P_VALUE_NUM                 16

/* ����֡�ϱ�RAMƫ�� */
#define WPHY_MNTN_SUB_FRAME_ADDR_OFFSET       0x30

/* ��֡�� */
#define WPHY_MNTN_SUB_FRAME_NUM               5

#define WPHY_MNTN_MAX_CNT                     0xFFFFFFFF

#define WPHY_MNTN_MAX_UINT16_VALUE            0xFFFF

#define WPHY_MNTN_MAX_CARRIER_NUM             2

/* HSDSCH��������ͳ�ƣ�����10������ */
#define WPHY_MNTN_MAX_REGION_NUM              10

/* W Rx FAST CAL ������:SC/DC BLK/NBLK ���8����ÿ������2���������� */
#define WPHY_CAL_RX_FAST_CAL_MAX_VAILD_NUM    64

/* W Tx FAST CAL ������ */
#define WPHY_CAL_TX_FAST_CAL_MAX_VAILD_NUM    450

/* HSUPA��ά�ɲ��ϱ��м��С��������� */
#define WPHY_MNTN_HSUPA_ACTIVE_SET_MAX_NUM    6

/* WBBP ֧�ֵ�������߸��� */
#define WPHY_MNTN_WBBP_MAX_ANT_NUM            2

#define WPHY_MNTN_INVALID_NUM                 0x3

/* ��Чֵ */
#define WPHY_MNTN_INVALID_VALUE               (-1)

/* WPHYÿ��֡�е�ʱ϶�� */
#define WPHY_MNTN_SLOT_NUM_BY_FRAME           15

/* ��С��Hsdpa SCCH���ŵ����� */
#define WPHY_MAIN_CELL_HSDPA_SCCH_MAX_NUM     4

/* ʮ��λ�������ֵ������Ϊ�Ƿ�ֵ */
#define WPHY_MNTN_UINT16_INVALID_NUM          0xFFFF

/* �Ĵ���Ĭ�ϱ߽�ֵ���� */
#define WPHY_MNTN_REG_INVALID                 0x5A5A

/* HsScch No����Чֵ */
#define WPHY_MNTN_INVALID_HSSCCH_NO           0xFF

/* ovsf����Чֵ */
#define WPHY_MNTN_INVALID_OVSF                0xFF

/* �ϱ����߿�RSSI����ֵ */
#define WPHY_MNTN_RSSI_PARA                   0x400

/* ���书����Чֵ */
#define WPHY_MNTN_POWER_VAL_INVALID           0xFF

/* ����1���չ�����Чֵ */
#define WPHY_MNTN_POWER_ANT1_VAL_INVALID      0xFF

/* ����2���չ�����Чֵ */
#define WPHY_MNTN_POWER_ANT2_VAL_INVALID      0xFF

/* Hs Pdsch�ŵ�ʱ��ƫ����Чֵ */
#define WPHY_MNTN_DL_DPCCH_OFFSET_INVALID     0xFF

/* BBP_DBG �� */
#define UPHY_TOOL_BBP_DBG_SAMPLE_REG_LIST_NUM 3
#define UPHY_TOOL_BBP_DBG_OTHER_REG_LIST_NUM  2

#define UPHY_TOOL_BBP_DBG_SAMPLE_REG_NUM      50

#define UPHY_TOOL_EASYRF_DBG_SAMPLE_REG_NUM   60
/*
 * ��������ģ�����. 0:bbic dfe; 1:rfic0 data����;2:rfic0 bus���� 3:rfic0 mrx����
 * 4:rfic1 data����;5:rfic1 bus���� 6:rfic1 mrx����
 */
#define UPHY_TOOL_EASYRF_DBG_MODULE_NUM       7
/*
 * ����ÿ��ģ���Ӧ�б���� :0:�����б�;1:�����б�;2:ʹ���б�;3:�ϱ��б�;
 * 4:��ѯǰֹͣ�б� 5:ֹͣǰ��ѯ�б� 6:��ѯ��ֹͣ�б�
 */
#define UPHY_TOOL_EASYRF_DBG_REG_LIST_NUM     7
/* EASYRFģ��ά������ */
#define EASYRF_OM_MAX_LOG_POINTS              32

/* EASYRF�Ĵ����ض����� */
#define EASYRF_OM_MAX_REG_READ_NUM            8

#define UPHY_TOOL_MAX_SSI_READ_ALL_NUM        256 /* ȫ�ض�֧��256���Ĵ��� */
#define UPHY_TOOL_MAX_SSI_READ_BACK_NUM       64  /* ���֧��64���Ĵ����Ļض� */
#define UPHY_TOOL_MAX_MIPI_READ_BACK_NUM      64  /* ���֧��64���Ĵ����Ļض� */

/* �ض����ڴ��ַ����� */
#define WPHY_MNTN_READ_ADDR_MAX_NUM           250

#define GPHY_TOOL_NCELL_TIMER_INVALID_VALUE   0xFFFFFFFF /* ����c1��c2��ʱ���õ�����Чֵ */

#define GPHY_TOOL_GSDR_SB_BURST_DATA_LEN      224

/* ����У׼ά��ض�AGC�Ĵ������� */
#define WPHY_TOOL_FAST_CAL_READ_AGC_REG_NUM   2

/* ����У׼ά��ض�������ּ�LNA�Ĵ������� */
#define WPHY_TOOL_FAST_CAL_READ_LNA_REG_NUM   4

/* ����У׼ά��ض�DCR���� */
#define WPHY_TOOL_FAST_CAL_READ_DCR_TIMES     4

/* IQI �ϱ�ID���� */
#define WPHY_MNTN_CIQ_MAKE_ID(a, b, c, d) \
    (((VOS_UINT32)(a) << 24) | ((VOS_UINT32)(b) << 16) | ((VOS_UINT32)(c) << 8) | ((VOS_UINT32)(d)))

/* ���֧�ֵ�MODEM��Ŀ */
#define RCM_TOOL_MNTN_MODEM_SUPPORT_NUM       (RCM_TOOL_MNTN_MODEM_ID_BUTT)

/* EASYRF����ά����Ϣ���ζ��г��� */
#define WPHY_TOOL_AEASYRF_MNTN_TASK_NUM       10

/* EASYRF���û������������ */
#define WPHY_TOOL_RF_EASYRF_TASK_LIST_MAX_NUM 10

#define WPHY_TOOL_WBBP_MAX_ANT_NUM            2

/* EASYRF������������������,���������������������󲢷��������ȷ�� */
#define WPHY_TOOL_AEASYRF_CONFIG_TASK_MAX_NUM 7

/* WPHY����ά��32bit�������ֵ */
#define WPHY_TOOL_REPORT_SELF_CAL_SAMPLE_DATA_MAX_SIZE 4096

/* WPHY������빦��ʵ������ͳ�����λ */
#define WPHY_TOOL_MNTN_ACCESS_PWR_MAX_LEVEL   7

/* b5010 msgid��Чλ */
#define WPHY_TOOL_MSGIDOFFSET                 0xFFF

/* easyrf the max num of mipi read */
#define EASYRF_OM_MAX_RD_MIPI_COUNT           5

/*
 * 3 ö�ٶ���
 */
/*
 * ö��˵��: CLT DETECT FAILԭ��
 */
enum WPHY_TOOL_CltDetectFailType {
    WPHY_TOOL_CLT_DETECT_FAIL_BBP_INVALID     = 0,
    WPHY_TOOL_CLT_DETECT_FAIL_NV_THLD_FAIL    = 1,
    WPHY_TOOL_CLT_DETECT_FAIL_PATH_DELAY_FAIL = 2,
    WPHY_TOOL_CLT_DETECT_FAIL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_CltDetectFailTypeUint16;

/*
 * ö��˵��: CLT״̬
 */
enum WPHY_TOOL_CltStatusType {
    WPHY_TOOL_CLT_STATUS_CROSSMIPI = 0,
    WPHY_TOOL_CLT_STATUS_OL        = 1,
    WPHY_TOOL_CLT_STATUS_CL        = 2,
    WPHY_TOOL_CLT_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_CltStatusTypeUint16;

/*
 * ö��˵��: ������Ҫ������������, bsic ����/bsic ��֤/������ϵͳ��Ϣ/��ϵͳ����
 * ע��: ��GPHY_NCE_PREDICT_TASK_TYPE_ENUM����һ�£�ͬ���޸�!!!
 */
enum GPHY_NCE_PredictTaskTypeElf {
    GPHY_E_NCE_PREDICT_TASK_INTERRAT_MEAS = 0, /* ������Ҫ����ϵͳ���� */
    GPHY_E_NCE_PREDICT_TASK_BCCH_READ     = 1, /* ������Ҫ������ϵͳ��Ϣ��ȡ */
    GPHY_E_NCE_PREDICT_TASK_BSIC_VERIFY   = 2, /* ������Ҫ������bsic check */
    GPHY_E_NCE_PREDICT_TASK_PHY_BSIC_SRCH = 3, /* ������Ҫ���������ȵ�����bsic srch */
    GPHY_E_NCE_PREDICT_TASK_PS_BSIC_SRCH  = 4, /* ������Ҫ��Э��ջ�·�������bsic srch */
    GPHY_E_NCE_PREDICT_TASK_BUTT
};
typedef VOS_UINT16 GPHY_NCE_PredictTaskTypeElfUint16;

/*
 * ö��˵��: ��ǰ�ĸ�ģʽ��ʹ��(��ά�ɲ�ʹ��)
 * ע��: ��GPHY_NCE_OCCASION_USE_MODE_ENUM����һ�£�ͬ���޸�!!!
 */
enum GPHY_NCE_OccasionUseModeElf {
    GPHY_E_NCE_OCCASION_USE_GSM   = 0, /* GSMʹ�õ�ǰ�Ĺ� */
    GPHY_E_NCE_OCCASION_USE_WCDMA = 1, /* WCDMAʹ�õ�ǰ�Ĺ� */
    GPHY_E_NCE_OCCASION_USE_LTE   = 2, /* LTEʹ�õ�ǰ�Ĺ� */
    GPHY_E_NCE_OCCASION_USE_TDS   = 3, /* TDʹ�õ�ǰ�Ĺ� */
    GPHY_E_NCE_OCCASION_USE_BUTT
};
typedef VOS_UINT16 GPHY_NCE_OccasionUseModeElfUint16;

/*
 * ö��˵��: �ֹ�ʱ�ж�����(��ά�ɲ�ʹ��)
 */
enum GPHY_GAP_IntType {
    GPHY_GAP_INT0 = 0, /* �ֹ�ʱ�յ�int0 */
    GPHY_GAP_INT1 = 1, /* �ֹ�ʱ�յ�int1 */
    GPHY_GAP_INT2 = 2, /* �ֹ�ʱ�յ�int2 */

    GPHY_GAP_BUTT
};
typedef VOS_UINT16 GPHY_GAP_IntTypeUint16;

/*
 * ö��˵��: ����ͬ����־
 * ע��: ��GPHY_NCE_NCELL_SYNC_ACTION_ENUM����һ�£�ͬ���޸�!!!
 */
enum GPHY_NCE_NcellSyncActionElf {
    GPHY_E_NCE_NCELL_SYNC_BSIC_SEARCH = 0, /* 0:BSIC���� */
    GPHY_E_NCE_NCELL_SYNC_BSIC_VERIFY,     /* 1:BSIC��֤ */
    GPHY_E_NCE_NCELL_SYNC_ACTION_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NcellSyncActionElfUint16;

/*
 * ö��˵��: BSIC����ģʽ
 * ע��: ��GPHY_NCE_NCELL_BSIC_SEARCH_STATE_ENUM����һ�£�ͬ���޸�!!!
 */
enum GPHY_NCE_NcellBsicSearchStateElf {
    GPHY_E_NCE_NCELL_BSIC_SEARCH_START = 0,         /* 0:BSIC�������� */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_SUCC,              /* 1:BSIC�����ɹ� */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_FAIL,              /* 2:BSIC����ʧ�� */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_ABNORMAL,          /* 3:BSIC�����쳣 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_OVER_HIGH_THD,     /* 4:BSIC������������������� */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_NOT_IN_NCELL_LIST, /* 5:BSIC�������������������б� */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_FORCIBLY_STOP,     /* 6:BSIC����������ǿͣbsic�������� */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_STATE_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NcellBsicSearchStateElfUint16;

/*
 * ö��˵��:
 * ע��: ��GPHY_NCE_NSTRONG_NCELL_ENUM_UINT16����һ�£�ͬ���޸�!!!
 */
enum GPHY_NCE_NstrongNcellElf {
    GPHY_E_NCE_6_STRONG_NCELL      = 0, /* 0-���� 0- 5ǿС�� */
    GPHY_E_NCE_6TO10_STRONG_NCELL  = 1, /* 1-���� 6-10ǿС�� */
    GPHY_E_NCE_11TO15_STRONG_NCELL = 2, /* 2-����11-15ǿС�� */
    GPHY_E_NCE_16TO31_STRONG_NCELL = 3, /* 3-����16-31ǿС�� */
    GPHY_E_NCE_STRONG_NCELL_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NstrongNcellElfUint16;

/*
 * ö��˵��: BSIC��֤ģʽ
 * ע��: ��GPHY_NCE_NCELL_BSIC_VERIFY_STATE_ENUM����һ�£�ͬ���޸�!!!
 */
enum GPHY_NCE_NcellBsicVerifyStateElf {
    GPHY_E_NCE_NCELL_BSIC_VERIFY_START = 0, /* 0:BSIC��֤���� */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_SUCC,      /* 1:BSIC��֤�ɹ� */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_FAIL,      /* 2:BSIC��֤ʧ�� */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_ABNORMAL,  /* 3:BSIC��֤�쳣 */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_STATE_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NcellBsicVerifyStateElfUint16;

/*
 * 3.1 ö�ٶ���,GPHYö�ٶ���
 */

/*
 * 3 ö�ٶ���
 */
/*
 * UPHY�ظ�CNFʱ,Я���Ĵ�����ö��
 */
enum UPHY_TOOL_RtcDebug {
    UPHY_MNTN_RTC_DEBUG_CLOSE = 0,
    UPHY_MNTN_RTC_DEBUG_OPEN  = 1,

    UPHY_MNTN_RTC_DEBUG_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_RtcDebugUint16;

/*
 * UPHY<-->TOOLԭ���б�
 */
enum UPHY_TOOL_MntnMsgId {
    ID_TOOL_UPHY_REQ = 0x0, /* _H2ASN_MsgChoice  TOOL_UPHY_REQ_STRU */

    ID_TOOL_UPHY_QUERY_SHARE_ADDR_REQ = 0xFE01, /* _H2ASN_MsgChoice UPHY_TOOL_QueryShareAddrReq  */
    ID_UPHY_TOOL_QUERY_SHARE_ADDR_IND = 0xFE02, /* _H2ASN_MsgChoice UPHY_TOOL_QueryShareAddrInd  */

    ID_TOOL_UPHY_SDT_LINK_STATUS_REQ = 0xFE03, /* _H2ASN_MsgChoice UPHY_TOOL_SdtLinkStatusReq  */
    ID_UPHY_TOOL_SDT_LINK_STATUS_CNF = 0xFE04, /* _H2ASN_MsgChoice UPHY_TOOL_CommonCnf  */

    ID_TOOL_UPHY_ERROR_REPORT_REQ = 0xFE05, /* _H2ASN_MsgChoice UPHY_TOOL_ErrorReportReq  */
    ID_UPHY_TOOL_ERROR_REPORT_CNF = 0xFE06, /* _H2ASN_MsgChoice UPHY_TOOL_CommonCnf  */

    ID_TOOL_UPHY_WPHY_BLER_RPT_REQ           = 0xFE07,
    ID_TOOL_UPHY_WPHY_INTRA_MSG_RPT_CTRL_REQ = 0xFE08,
    ID_TOOL_UPHY_GPHY_INTRA_MSG_RPT_CTRL_REQ = 0xFE09,

    ID_TOOL_UPHY_WPHY_INTER_GREG_REPORT_REQ = 0xFE0A, /* _H2ASN_MsgChoice UPHY_TOOL_WphyInterGregReportReq  */
    ID_UPHY_TOOL_WPHY_INTER_GREG_REPORT_CNF = 0xFE0B, /* _H2ASN_MsgChoice UPHY_TOOL_WphyInterGregReportCnf  */
    ID_UPHY_TOOL_WPHY_INTER_GREG_REPORT_IND = 0xFE0C, /* _H2ASN_MsgChoice UPHY_TOOL_WphyInterGregReportInd  */
    ID_TOOL_UPHY_WPHY_INTER_GREG_STOP_REQ   = 0xFE0D, /* _H2ASN_MsgChoice UPHY_TOOL_WphyInterGregStopReq  */
    ID_UPHY_TOOL_WPHY_INTER_GREG_STOP_CNF   = 0xFE0E, /* _H2ASN_MsgChoice UPHY_TOOL_CommonCnf  */

    ID_TOOL_UPHY_GPHY_INTER_WREG_REPORT_REQ = 0xFE0F, /* _H2ASN_MsgChoice UPHY_TOOL_GphyInterWregReportReq  */
    ID_UPHY_TOOL_GPHY_INTER_WREG_REPORT_CNF = 0xFE10, /* _H2ASN_MsgChoice UPHY_TOOL_GphyInterWregReportCnf  */
    ID_UPHY_TOOL_GPHY_INTER_WREG_REPORT_IND = 0xFE11, /* _H2ASN_MsgChoice UPHY_TOOL_GphyInterWregReportInd  */
    ID_TOOL_UPHY_GPHY_INTER_WREG_STOP_REQ   = 0xFE12, /* _H2ASN_MsgChoice UPHY_TOOL_GphyInterWregStopReq  */
    ID_UPHY_TOOL_GPHY_INTER_WREG_STOP_CNF   = 0xFE13, /* _H2ASN_MsgChoice UPHY_TOOL_CommonCnf  */

    ID_UPHY_TOOL_DRX_LATE_INFO_IND      = 0xFE14, /* _H2ASN_MsgChoice UPHY_TOOL_DrxLateInfoInd  */
    ID_UPHY_TOOL_KEY_INFORMATION_REPORT = 0xFE15, /* _H2ASN_MsgChoice UPHY_COM_KEY_INFORMATION_STRU */

    ID_UPHY_TOOL_UPHY_INIT_REPORT_IND = 0xFE18, /* _H2ASN_MsgChoice  ** */

    ID_RCM_TOOL_MNTN_MSG_SCHDL_MAIN_INFO = 0xFE19, /* _H2ASN_MsgChoice RCM_SCHDL_MAIN_PARA_STRU */
    ID_RCM_TOOL_MNTN_MSG_MSGLIST_INFO    = 0xFE1A, /* _H2ASN_MsgChoice RCM_PHY_MSG_LIST_STRU */
    ID_RCM_TOOL_MNTN_MSG_CHAINLIST_INFO  = 0xFE1B, /* _H2ASN_MsgChoice RCM_TASK_NODE_STRU */

    /* diag �Ĵ����ṹ���ü���Ϣ�ϱ� */
    ID_TOOL_COM_DIAG_PHY_SAMPLE_CFG_REQ     = 0xFE1C, /* _H2ASN_MsgChoice COM_TOOL_DIAG_PHY_SAMPLE_CFG_STRU */
    ID_TOOL_COM_DIAG_PHY_SAMPLE_IND         = 0xFE1D, /* _H2ASN_MsgChoice COM_TOOL_DIAG_PHY_SAMPLE_REG_IND_STRU */
    ID_TOOL_COM_DIAG_SAMPLE_REG_IND         = 0xFE1E, /* _H2ASN_MsgChoice COM_TOOL_DIAG_PHY_REG_IND_STRU */
    ID_TOOL_COM_DIAG_PHY_NEW_SAMPLE_CFG_REQ = 0xFE1F, /* _H2ASN_MsgChoice COM_TOOL_DIAG_PHY_SAMPLE_CFG_STRU */

    ID_TOOL_UPHY_SOCP_VOTE_WAKE_CNF = 0xFE20, /* _H2ASN_MsgChoice UPHY_DRV_SOCP_VOTE_WAKE_CNF_STRU */

    ID_TOOL_UPHY_SSI_READ_RPT_REQ = 0xFE21,
    ID_UPHY_TOOL_SSI_READ_RPT_CNF = 0xFE22,
    ID_UPHY_TOOL_SSI_READ_RPT_IND = 0xFE23,

    ID_TOOL_UPHY_MIPI_READ_RPT_REQ = 0xFE24,
    ID_UPHY_TOOL_MIPI_READ_RPT_CNF = 0xFE25,
    ID_UPHY_TOOL_MIPI_READ_RPT_IND = 0xFE26,

    ID_RCM_TOOL_MNTN_DCXO_EVENT_RPT        = 0xFE27,
    ID_RCM_TOOL_MNTN_DCXO_SAMPLE_ARRAY_RPT = 0xFE28,
    ID_RCM_TOOL_MNTN_DCXO_TEMP_COMP_INFO   = 0xFE29,

    ID_UPHY_TOOL_MNTN_DCXO_STATUS_RPT     = 0xFE2a,
    ID_RCM_TOOL_MNTN_IDLE_DCXO_EVENT_RPT  = 0xFE2b,
    ID_RCM_TOOL_MNTN_IDLE_DCXO_SAMPLE_RPT = 0xFE2c,

    ID_TOOL_UPHY_GSDR_PARA_DUMP_REPORT_REQ = 0xFE2d, /* _H2ASN_MsgChoice UPHY_TOOL_GsdrMntnReq  */
    ID_UPHY_TOOL_GSDR_PARA_DUMP_REPORT_CNF = 0xFE2e, /* _H2ASN_MsgChoice UPHY_TOOL_CommonCnf  */

    ID_TOOL_RCM_MNTN_FUNC_SWTICH_REQ = 0xFE2F,

    ID_RCM_TOOL_MNTN_TAS_ARBITRATION_RSLT_INFO = 0xFE30, /* _H2ASN_MsgChoice RCM_TOOL_MntnTasArbitrationRslt  */
    ID_UPHY_TOOL_MNTN_CLT_ALG_RPT              = 0xFE31, /* _H2ASN_MsgChoice RCM_TOOL_MntnCltAlgRpt  */
    ID_UPHY_TOOL_INFO_REPORT_IND               = 0xFEF7, /* _H2ASN_MsgChoice UPHY_TOOL_ErrorReportInd  */
    ID_UPHY_TOOL_WARNING_REPORT_IND            = 0xFEF8, /* _H2ASN_MsgChoice UPHY_TOOL_ErrorReportInd  */
    ID_UPHY_TOOL_ERROR_REPORT_IND              = 0xFEF9, /* _H2ASN_MsgChoice UPHY_TOOL_ErrorReportInd  */

    ID_UPHY_TOOL_TAS_STATUS_REPORT_REQ = 0xFEFA, /* _H2ASN_MsgChoice UPHY_TOOL_TasStatusReportReq  */
    ID_UPHY_TOOL_TAS_STATUS_REPORT_CNF = 0xFEFB, /* _H2ASN_MsgChoice UPHY_TOOL_CommonCnf  */
    ID_CPROC_UPHY_CSDR_SET_MODE_INFO   = 0xFEFF,

    ID_TOOL_COM_APP_PHY_SSI_DEBUG_CFG_REQ     = 0xFF00,
    ID_TOOL_COM_APP_PHY_NEW_SSI_DEBUG_CFG_REQ = 0xFF01,

    ID_TOOL_COM_APP_PHY_NEW_MIPI_DEBUG_CFG_REQ = 0xFF02,

    ID_UPHY_SDR_DEBUG_REPORT_IND   = 0xFF03, /* _H2ASN_MsgChoice  UPHY_TOOL_ErrorReportInd  */
    ID_UPHY_SDR_INFO_REPORT_IND    = 0xFF04, /* _H2ASN_MsgChoice  UPHY_TOOL_ErrorReportInd  */
    ID_UPHY_SDR_WARNING_REPORT_IND = 0xFF05, /* _H2ASN_MsgChoice  UPHY_TOOL_ErrorReportInd  */
    ID_UPHY_SDR_ERROR_REPORT_IND   = 0xFF06, /* _H2ASN_MsgChoice  UPHY_TOOL_ErrorReportInd  */

    ID_TOOL_UPHY_MNTN_FUNC_SWITCH_REQ = 0xFF07, /* _H2ASN_MsgChoice  UPHY_TOOL_MntnFuncSwitch  */
    /* arm bba ��ϢID ��0xFFC0--0xFFFF */

    ID_UPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnMsgIdUint16;

/*
 * EASYRF<-->TOOLԭ���б�
 */
enum EASYRF_TOOL_MntnMsgId {
    ID_TOOL_EASYRF_MNTN_DBG_CFG_REQ          = 0xF800,
    ID_EASYRF_TOOL_MNTN_DBG_CFG_CNF          = 0xF801,
    ID_EASYRF_TOOL_MNTN_DBG_REPORT_IND       = 0xF802,
    ID_TOOL_EASYRF_MNTN_LOG_LEVEL_CFG_REQ    = 0xF803,
    ID_TOOL_EASYRF_MNTN_DBG_CHAN_ADDR_INFO   = 0xF804,
    ID_TOOL_EASYRF_MNTN_SRC_REPORT_SWITCH    = 0xF805,
    ID_TOOL_EASYRF_MNTN_REG_READ_REQ         = 0xF806,
    ID_TOOL_EASYRF_MNTN_TRxAFE_ALWAYS_ON_REQ = 0xF807,
    ID_TOOL_EASYRF_MNTN_TXAPC_CODE_STUB_REQ  = 0xF808,
    ID_TOOL_EASYRF_MNTN_MIPI_READ_REQ        = 0xF809,
    ID_TOOL_EASYRF_MNTN_GPIO_READ_REQ        = 0xF80A,
    ID_TOOL_EASYRF_MNTN_PD_OM_REQ            = 0xF810,
    ID_TOOL_EASYRF_MNTN_UL_TIMING_UPDATE_REQ = 0xF811,
    ID_TOOL_EASYRF_MNTN_RFRESET_TEST_REQ     = 0xF812,
    ID_TOOL_EASYRF_MNTN_HF_TRX_CTRL_REQ      = 0xF813,
    ID_EASYRF_MNTN_MSG_ID_BUTT
};
typedef VOS_UINT16 EASYRF_TOOL_MntnMsgIdUint16;

/*
 * UPHY�ظ�CNFʱ,Я���Ĵ�����ö��
 */
enum UPHY_TOOL_MntnRslt {
    UPHY_MNTN_RSLT_SUCC = 0,
    UPHY_MNTN_RSLT_FAIL = 1,

    UPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnRsltUint16;

/*
 * ö��˵��: UPHY�����ϱ�����
 */
enum UPHY_TOOL_MntnRptLevel {
    UPHY_MNTN_RPT_LEVEL_DEBUG   = 0,
    UPHY_MNTN_RPT_LEVEL_INFO    = 1,
    UPHY_MNTN_RPT_LEVEL_WARNING = 2,
    UPHY_MNTN_RPT_LEVEL_ERROR   = 3,

    UPHY_MNTN_RPT_LEVEL_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnRptLevelUint16;

/*
 * RCM ö�ٶ���
 */

/*
 * ö��˵��: ��¼Modem#0~#2��TAS Modem״̬,ά���ϱ�
 *           ����ͬRCM_TAS_MODEM_STATE_ENUM_UINT16
 */
enum RCM_TOOL_MntnModemState {
    RCM_TOOL_MNTN_MODEM_STATE_FILGHT         = 0,  /* S0: ����ģʽ */
    RCM_TOOL_MNTN_MODEM_STATE_IDLE_HAPPY     = 1,  /* S1: ����HAPPY״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_PSONLY_GOOD    = 2,  /* S2: PS ONLYҵ���Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_ACCESS_GOOD    = 3,  /* S3: �����Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_CS_BACK_GOOD   = 4,  /* S4: CS����ͨ����Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_CS_ACTIVE_GOOD = 5,  /* S5: CSͨ����Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_SEARCHING      = 6,  /* S6: ����״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_IDLE_UNHAPPY   = 7,  /* S7: ����UNHAPPY״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_PSONLY_POOR    = 8,  /* S8: PS ONLYҵ����Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_ACCESS_POOR    = 9,  /* S9: ������Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_CS_BACK_POOR   = 10, /* S10: CS����ͨ����Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_CS_ACTIVE_POOR = 11, /* S11: CSͨ����Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_IDLE_GOOD      = 12, /* S12: ����̬��Ѷ��״̬ */
    RCM_TOOL_MNTN_MODEM_STATE_LAB_MODE       = 13, /* S13: ʵ����ģʽ״̬����������ģʽ����������ģʽ */
    RCM_TOOL_MNTN_MODEM_STATE_TEST_MODE      = 14, /* S14: ����ģʽ״̬���������Կ�ģʽ��AT�㷨ģʽ */
    RCM_TOOL_MNTN_MODEM_STATE_LOCK_ANT       = 15, /* S15: ������ģʽ */
    RCM_TOOL_MNTN_MODEM_STATE_TAS_DISABLE    = 16, /* S16: tas��ʹ��ģʽ */
    RCM_TOOL_MNTN_MODEM_STATE_BUTT
};
typedef VOS_UINT16 RCM_TOOL_MntnModemStateUint16;

/*
 * ö��˵��: ��¼�þ�����ڲ�TAS�л��ж���Ϣ��ά���ϱ�
 *           ����ͬRCM_TAS_ACTION_ENUM_UINT16
 */
enum RCM_TOOL_MntnAction {
    RCM_TOOL_MNTN_ACTION_START_M0     = 0x01, /* b0000 0001: A0a=C0+modem#0 */
    RCM_TOOL_MNTN_ACTION_START_M1     = 0x02, /* b0000 0010: A0b=C0+modem#1 */
    RCM_TOOL_MNTN_ACTION_START_M2     = 0x04, /* b0000 0100: A0c=C0+modem#2 */
    RCM_TOOL_MNTN_ACTION_START_M01    = 0x03, /* b0000 0011: A0ab=C0+modem#01 */
    RCM_TOOL_MNTN_ACTION_START_M02    = 0x05, /* b0000 0101: A0ac=C0+modem#02 */
    RCM_TOOL_MNTN_ACTION_START_M12    = 0x06, /* b0000 0110: A0bc=C0+modem#12 */
    RCM_TOOL_MNTN_ACTION_START_ALL    = 0x07, /* b0000 0111: A0abc=C0+modem#012 */
    RCM_TOOL_MNTN_ACTION_SETDPDT0_M0  = 0x11, /* b0001 0001: A1a=C1+modem#0 */
    RCM_TOOL_MNTN_ACTION_SETDPDT0_M1  = 0x12, /* b0001 0010: A1b=C1+modem#1 */
    RCM_TOOL_MNTN_ACTION_SETDPDT0_M2  = 0x14, /* b0001 0100: A1c=C1+modem#2 */
    RCM_TOOL_MNTN_ACTION_SETDPDT1_M0  = 0x21, /* b0010 0001: A2a=C2+modem#0 */
    RCM_TOOL_MNTN_ACTION_SETDPDT1_M1  = 0x22, /* b0010 0010: A2b=C2+modem#1 */
    RCM_TOOL_MNTN_ACTION_SETDPDT1_M2  = 0x24, /* b0010 0100: A2c=C2+modem#2 */
    RCM_TOOL_MNTN_ACTION_STOP_KEEP    = 0x30, /* b0011 0000: A3=C3+empty */
    RCM_TOOL_MNTN_ACTION_STOP_DEFAULT = 0x40, /* b0100 0000: A4=C4+empty */
    RCM_TOOL_MNTN_ACTION_BUTT         = 0xFF
};
typedef VOS_UINT16 RCM_TOOL_MntnActionUint16;

/*
 * ö��˵��: toolά����Ϣ��ר�õ������ּ�ö�ٶ���,���������±��Ӧ��;
 *          ����ͬRCM_MODEM_ID_ENUM_UINT16
 */
enum RCM_TOOL_MntnModemId {
    RCM_TOOL_MNTN_MODEM_0     = 0, /* MODEM0 */
    RCM_TOOL_MNTN_MODEM_1     = 1, /* MODEM1 */
    RCM_TOOL_MNTN_MODEM_2     = 2, /* MODEM2 */
    RCM_TOOL_MNTN_MODEM_1X    = 3, /* MODEM_1x */
    RCM_TOOL_MNTN_MODEM_OTHER = 4, /* MODEM_OHTER */
    RCM_TOOL_MNTN_MODEM_ID_BUTT
};
typedef VOS_UINT16 RCM_TOOL_MntnModemIdUint16;

/*
 * GPHY<-->TOOLԭ���б�
 */
enum GPHY_TOOL_MntnMsgId {
    /* �·�REQ��Ϣ��Ϊ��VOS��Ϣ���ϱ���IND��CNF��Ϣ����VOS��Ϣͷ��Ӧ��REQ�������������ָʾΪ��ASN���ɷ��� */
    ID_TOOL_GPHY_REQ = 0x0, /* _H2ASN_MsgChoice  TOOL_GPHY_REQ_STRU */

    ID_TOOL_GPHY_BLER_REPORT_REQ = 0xE001, /* _H2ASN_MsgChoice GPHY_TOOL_BlerReportReq  */
    ID_GPHY_TOOL_BLER_REPORT_CNF = 0xE010, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_BLER_REPORT_IND = 0xE011, /* _H2ASN_MsgChoice GPHY_TOOL_BlerReportInd  */

    ID_TOOL_GPHY_SNR_REPORT_REQ     = 0xE002, /* _H2ASN_MsgChoice GPHY_TOOL_SnrReportReq  */
    ID_TOOL_GPHY_POWER_CTRL_SET_REQ = 0xE005, /* _H2ASN_MsgChoice GPHY_TOOL_PowerCtrlSetReq  */
    ID_GPHY_TOOL_SNR_REPORT_CNF     = 0xE020, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_SNR_REPORT_IND     = 0xE021, /* _H2ASN_MsgChoice GPHY_TOOL_SnrReportInd  */

    ID_TOOL_GPHY_AFC_REPORT_REQ = 0xE003, /* _H2ASN_MsgChoice GPHY_TOOL_AfcReportReq  */
    ID_GPHY_TOOL_AFC_REPORT_CNF = 0xE030, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_AFC_REPORT_IND = 0xE031, /* _H2ASN_MsgChoice GPHY_TOOL_AfcReportInd  */

    ID_TOOL_GPHY_POWER_CTRL_REPORT_REQ = 0xE004, /* _H2ASN_MsgChoice GPHY_TOOL_PowerCtrlReportReq  */
    ID_GPHY_TOOL_POWER_CTRL_REPORT_CNF = 0xE040, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_POWER_CTRL_REPORT_IND = 0xE041, /* _H2ASN_MsgChoice GPHY_TOOL_PowerCtrlReportInd  */

    /* TAS״̬�ϱ����� */
    ID_GPHY_TOOL_REQUEST_TAS_STATUS_IND = 0xE043, /* _H2ASN_MsgChoice GPHY_TOOL_TasStatusReportInd */

    ID_GPHY_TOOL_PSRCH_NCELL_FB_SB_READ_REQ = 0xE044, /* _H2ASN_MsgChoice GPHY_TOOL_PSRCH_NCELL_FB_SB_READ_REQ_STRU */
    ID_GPHY_TOOL_PSRCH_NCELL_BCCH_READ_REQ  = 0xE045, /* _H2ASN_MsgChoice GPHY_TOOL_PSRCH_NCELL_BCCH_READ_REQ_STRU */

    ID_GPHY_TOOL_PSRCH_SEARCH_CELL_LIST_IND = 0xE046, /* _H2ASN_MsgChoice GPHY_TOOL_PSRCH_SEARCH_CELL_LIST_IND_STRU */
    ID_GPHY_TOOL_PSRCH_SEARCH_CAND_INFO_IND = 0xE047, /* _H2ASN_MsgChoice GPHY_TOOL_PSRCH_SEARCH_CAND_INFO_IND_STRU */
    ID_GPHY_TOOL_PSRCH_SEARCH_CELL_INFO_IND = 0xE048, /* _H2ASN_MsgChoice GPHY_TOOL_PSRCH_SEARCH_CELL_INFO_IND_STRU */

    ID_GPHY_TOOL_PSRCH_IDLE_CELL_LIST_IND = 0xE049, /* _H2ASN_MsgChoice GPHY_TOOL_PSRCH_IDLE_CELL_LIST_IND_STRU */

    ID_TOOL_GPHY_READ_MEM_REQ = 0xE04A, /* _H2ASN_MsgChoice GPHY_TOOL_ReadMemReq  */
    ID_GPHY_TOOL_READ_MEM_CNF = 0xE04B, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_READ_MEM_IND = 0xE04C, /* _H2ASN_MsgChoice GPHY_TOOL_ReadMemInd  */

    /* �������� ��Ϣ�ϱ� */
    ID_GPHY_TOOL_INTERFE_TX_BLANKING_IND = 0xE04D,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G����GTC�ѱ�һ��modem��TX�ر� */
    ID_GPHY_TOOL_INTERFE_RX_BLANKING_IND = 0xE04E,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G����GTC�ѱ�һ��modem��RX�ָ� */
    ID_GPHY_TOOL_INTERFE_STOP_G_TX_IND = 0xE04F,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G�ر��Լ���TX���� */

    ID_GPHY_TOOL_POWER_CTRL_SET_CNF = 0xE050, /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_CHANNEL_REPORT_IND = 0xE051, /* _H2ASN_MsgChoice  GPHY_TOOL_ChannelReportInd  */

    ID_GPHY_TOOL_MASTER_STOP_SLAVE_MEAS_IND = 0xE052,
    /* _H2ASN_MsgChoice  GPHY_TOOL_MasterStopSlaveMeasInd  */ /* �ϱ�ǿͣ��ģ��Ϣ */

    ID_GPHY_TOOL_CHANNEL_QUALITY_IND = 0xE080, /* _H2ASN_MsgChoice GPHY_TOOL_ChannelQualityInd  */

    ID_TOOL_GPHY_FUNC_BIT_CONTROL_REQ = 0xE00A, /* _H2ASN_MsgChoice GPHY_TOOL_FuncBitCtrlReq  */
    ID_GPHY_TOOL_FUNC_BIT_CONTROL_CNF = 0xE0A0, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */

    ID_TOOL_GPHY_PARA_CONTROL_REQ = 0xE00B, /* _H2ASN_MsgChoice GPHY_TOOL_ParaControlReq  */
    ID_GPHY_TOOL_PARA_CONTROL_CNF = 0xE0B0, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */

    ID_TOOL_GPHY_RF_FRONT_STATIC_REQ = 0xE052, /* _H2ASN_MsgChoice GPHY_TOOL_RfFrontStaticReq  */
    ID_GPHY_TOOL_RF_FRONT_STATIC_IND = 0xE053, /* _H2ASN_MsgChoice GPHY_TOOL_RfFrontStaticInd  */

    ID_TOOL_GPHY_INTRA_MSG_RPT_CTRL_REQ = 0xE054, /* _H2ASN_MsgChoice GPHY_TOOL_IntraMsgRptCtrlReq  */
    ID_GPHY_TOOL_INTRA_MSG_RPT_CTRL_CNF = 0xE055, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */

    ID_TOOL_GPHY_AMR_REPORT_REQ = 0xE057, /* _H2ASN_MsgChoice GPHY_TOOL_AmrReportReq  */
    ID_GPHY_TOOL_AMR_REPORT_CNF = 0xE058, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_AMR_REPORT_IND = 0xE059, /* _H2ASN_MsgChoice GPHY_TOOL_AmrReportInd  */

    ID_TOOL_GPHY_BLK_REPORT_LIST_REQ = 0xE05A, /* _H2ASN_MsgChoice GPHY_TOOL_BlkReportListReq  */
    ID_GPHY_TOOL_BLK_REPORT_LIST_CNF = 0xE05B, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_BLK_REPORT_LIST_IND = 0xE05C, /* _H2ASN_MsgChoice GPHY_TOOL_BlkReportListInd  */

    ID_GPHY_TOOL_GTC_TASK_INFO_IND   = 0xE061, /* _H2ASN_MsgChoice GPHY_GTC_CONTEXT_STRU */
    ID_GPHY_TOOL_RCV_SINGAL_INFO_IND = 0xE062, /* _H2ASN_MsgChoice GPHY_TOOL_RcvSingalInfoReq  */
    ID_GPHY_TOOL_CS_UL_INFO_IND      = 0xE064, /* _H2ASN_MsgChoice GSM_CHN_UL_INFO_RPT_STRU */
    ID_GPHY_TOOL_CS_DL_INFO_IND      = 0xE065, /* _H2ASN_MsgChoice COM_CS_DECODE_CTRL_INFO_STRU */

    ID_GPHY_TOOL_RF_RESOURCE_WAR_IND  = 0xE067, /* _H2ASN_MsgChoice GPHY_DRV_CPU_CFG_RF_FLG_STRU */
    ID_GPHY_TOOL_RF_DSDS_WAKE_REPORT  = 0xE068, /* _H2ASN_MsgChoice GPHY_DRV_CPU_CFG_RF_FLG_STRU */
    ID_GPHY_TOOL_RF_DSDS_SLEEP_REPORT = 0xE069, /* _H2ASN_MsgChoice GPHY_DRV_CPU_CFG_RF_FLG_STRU */
    ID_GPHY_TOOL_GRA_TASK_INFO_IND    = 0xE06A, /* _H2ASN_MsgChoice */

    /* GmeasW GBGW�ֹ���Ϣ�ϱ� */
    ID_GPHY_TOOL_GMEASW_SCHEDULE_INT7_IND = 0xE06B, /* _H2ASN_MsgChoice GPHY_GMEASW_SCHEDULE_INFO_STRU */
    ID_GPHY_TOOL_GBGW_SCHEDULE_INT7_IND   = 0xE06C, /* _H2ASN_MsgChoice GPHY_BGW_SCHEDULE_INFO_STRU */

    ID_GPHY_TOOL_MASTER_WAKE_INFO_IND = 0xE06D, /* _H2ASN_MsgChoice UPHY_GDRX_MASTER_MNTN_STRU */
    ID_GPHY_TOOL_SLAVE_WAKE_INFO_IND  = 0xE06E, /* _H2ASN_MsgChoice UPHY_GDRX_SLAVE_MNTN_STRU */
    ID_GPHY_TOOL_DCOFFSET_UPDATE_IND  = 0xE06F, /* _H2ASN_MsgChoice GPHY_TOOL_DcOffsetReport  */

    /* ���͸�GSDR����Ϣ */
    ID_GPHY_TOOL_GSDR_SRCH_INIT_REQ = 0xE070,
    ID_GPHY_TOOL_GSDR_SRCH_PARA_REQ = 0xE071,
    ID_GPHY_TOOL_GSDR_SB_DEC_REQ    = 0xE072,

    /* G��ģ������Ϣ��ӡ */
    ID_GPHY_TOOL_SLAVE_MEAS_INFO_IND = 0xE073,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntDelayInd  */ /* �ϱ�G��ģ������Ϣ */

    ID_GPHY_TOOL_DCH_OCCASION_USE_RPT_IND    = 0xE074,
    ID_GPHY_TOOL_NCELL_BSIC_SEARCH_STATE_IND = 0xE075,
    ID_GPHY_TOOL_NCELL_BSIC_VERIFY_STATE_IND = 0xE076,
    ID_GPHY_TOOL_NCELL_INTERRAT_RANK_IND     = 0xE077,
    ID_GPHY_TOOL_NCELL_PREDICT_TASK_IND      = 0xE078,
    ID_GPHY_TOOL_CHN_DUMMY_PCH_RPT_IND       = 0xE079,
    ID_GPHY_TOOL_NCELL_SB_IND                = 0xE07A,
    ID_GPHY_TOOL_FE_STATUS_INFO_IND          = 0xE07B, /* _H2ASN_MsgChoice GPHY_COM_FE_STATUS_INFO_STRU */
    ID_GPHY_TOOL_GAP_TIME_INFO_INT2_IND      = 0xE07C, /* _H2ASN_MsgChoice GPHY_TOOL_GapOutTimeInfo  */
    ID_GPHY_TOOL_GSDR_JUDGE_INFO_IND         = 0xE07D, /* _H2ASN_MsgChoice GPHY_TOOL_GSDR_JUGDE_INFO_IND_STRU */

    /* bbp share msgId */
    ID_GPHY_TOOL_BBP_SHARE_RES_SELECT_IND  = 0xE07E,
    ID_GPHY_TOOL_BBP_SHARE_RD_WR_ERROR_IND = 0xE07F,
    ID_GPHY_TOOL_BBP_SHARE_EVENT_IND       = 0xE081,
    ID_GPHY_TOOL_GSDR_JUDGE_BCH_INFO_IND   = 0xE082,

    /* GSM���������Ϣ�ϱ� */
    ID_GPHY_TOOL_GSDR_DIFF_BASIC_CELL_IND = 0xE083,

    /* ���������б���Ϣ�ϱ� */
    ID_GPHY_TOOL_NCELL_TASK_LIST_IND = 0xE084, /* _H2ASN_MsgChoice GPHY_TOOL_NcellTaskListIndRpt */

    /* SW-SAR��Ϣ�ϱ� */
    ID_GPHY_TOOL_SW_SAR_INFO_IND = 0xE085, /* _H2ASN_MsgChoice ID_GPHY_TOOL_SW_SAR_INFO_IND */

    /* ʱ��ƽ��SAR��Ϣ�ϱ� */
    ID_GPHY_TOOL_TIME_AVG_SAR_PERIOD_IND  = 0xE097, /* _H2ASN_MsgChoice GPHY_TOOL_TimeAvgSarPeriodInd  */
    ID_GPHY_TOOL_TIME_AVG_SAR_DPR_ON_IND  = 0xE098, /* _H2ASN_MsgChoice GPHY_TOOL_TimeAvgSarDprStateInd  */
    ID_GPHY_TOOL_TIME_AVG_SAR_DPR_OFF_IND = 0xE099, /* _H2ASN_MsgChoice GPHY_TOOL_TimeAvgSarDprStateInd  */

    /* G��ģSRVCC����������Ϣ��ӡ */
    ID_GPHY_TOOL_SLAVE_BSIC_SYNC_RSLT_IND   = 0xE09A, /* _H2ASN_MsgChoice  GPHY_SLAVE_BSIC_SYNC_RSLT_STRU */
    ID_GPHY_TOOL_SLAVE_SRVCC_SCHDL_INFO_IND = 0xE09B,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntDelayInd  */ /* �ϱ�G��ģ������Ϣ */
    ID_GPHY_TOOL_INTERFE_STOP_G_BLANKING_IND = 0xE09C,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G�ر�TX/RX���� */
    ID_GPHY_TOOL_DT_STAT_CODEC_INFO_REQ = 0xE09D,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DtStatCodecReq  */ /* ���������������ϱ������ÿ���������ʵ�ʹ���� */
    ID_GPHY_TOOL_DT_STAT_CODEC_INFO_IND = 0xE09E,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DtStatCodecInd  */ /* �ϱ������ÿ���������ʵ�ʹ������ */
    ID_GPHY_TOOL_REPORT_INFO_IND    = 0xE0F7,         /* _H2ASN_MsgChoice  GPHY_TOOL_ReportErrorInd  */
    ID_GPHY_TOOL_REPORT_WARNING_IND = 0xE0F8,         /* _H2ASN_MsgChoice  GPHY_TOOL_ReportErrorInd  */
    ID_GPHY_TOOL_REPORT_ERROR_IND   = 0xE0F9,         /* _H2ASN_MsgChoice  GPHY_TOOL_ReportErrorInd  */

    ID_TOOL_GPHY_BBP_RTC_REQ = 0xE0FA, /* _H2ASN_MsgChoice GPHY_TOOL_BbpRtcReq */
    ID_GPHY_TOOL_BBP_RTC_CNF = 0xE0FB, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf */

    /* DSP-PROBE */
    ID_TOOL_GPHY_GSM_CELL_SNR_REQ = 0xE600,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GsmCellSnrReq  */ /* ���������������ϱ�����С����6ǿ����������ȣ�����ֵ */
    ID_GPHY_TOOL_GSM_CELL_SNR_CNF = 0xE601,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_GSM_CELL_SNR_IND = 0xE602,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GsmCellSnrInd  */ /* �ϱ�����С����6ǿ����������ȣ�����ֵ */

    ID_TOOL_GPHY_BA_LIST_MEAS_REQ = 0xE603,
    /* _H2ASN_MsgChoice  GPHY_TOOL_BaListMeasReq  */ /* ���������������ϱ���������32���������ź�ǿ�ȣ�bsicֵ */
    ID_GPHY_TOOL_BA_LIST_MEAS_CNF = 0xE604,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_BA_LIST_MEAS_IND = 0xE605,
    /* _H2ASN_MsgChoice  GPHY_TOOL_BaListMeasInd  */ /* �ϱ���������32���������ź�ǿ�ȣ�bsicֵ */

    ID_TOOL_GPHY_AMR_SETTING_REQ = 0xE606,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrSettingReq  */ /* ���������������ϱ�AMR������Ϣ */
    ID_GPHY_TOOL_AMR_SETTING_CNF = 0xE607,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_AMR_SETTING_IND = 0xE608,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrSettingInd  */ /* �ϱ�AMR������Ϣ */

    ID_TOOL_GPHY_AMR_CODEC_USAGE_REQ = 0XE609,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecUsageReq  */ /* ���������������ϱ������ÿ���������ʵ�ʹ���� */
    ID_GPHY_TOOL_AMR_CODEC_USAGE_CNF = 0XE60A,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_AMR_CODEC_USAGE_IND = 0XE60B,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecUsageInd  */ /* �ϱ������ÿ���������ʵ�ʹ������ */

    ID_TOOL_GPHY_AMR_CODEC_STAT_REQ = 0xE60C,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecStatReq  */ /* ������������������������ʵ�ʹ���� */
    ID_GPHY_TOOL_AMR_CODEC_STAT_CNF = 0xE60D,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_AMR_CODEC_STAT_IND = 0xE60E,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecStatInd  */ /* �ϱ������������ʵ�ʹ���� */

    ID_TOOL_GPHY_CS_PS_TX_INFO_REQ = 0xE60F,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CS_TX_INFO_REQ_STRU */ /* ���������������ϱ�CS��������Ϣ */
    ID_GPHY_TOOL_CS_PS_TX_INFO_CNF = 0xE610,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_CS_PS_TX_INFO_IND = 0xE611,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CS_TX_INFO_IND_STRU */ /* �ϱ�CS��������Ϣ */

    ID_TOOL_GPHY_CS_FER_REQ = 0xE612,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CsFerReq  */ /* �����������ϱ�CS���FER */
    ID_GPHY_TOOL_CS_FER_CNF = 0xE613,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_CS_FER_IND = 0xE614,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CsFerInd  */ /* �ϱ�CS���FER */

    ID_TOOL_GPHY_GET_CURRENT_DSC_REQ = 0xE615,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GetCurrentDscReq  */ /* �����������ϱ�DSC����ֵ */
    ID_GPHY_TOOL_GET_CURRENT_DSC_CNF = 0xE616,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_GET_CURRENT_DSC_IND = 0xE617,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GetCurrentDscInd  */ /* �ϱ�DSC����ֵ */

    ID_GPHY_TOOL_MASTER_WAKE_LTE_IND        = 0xE620,
    ID_GPHY_TOOL_MASTER_START_LTE_MEAS_IND  = 0xE621,
    ID_GPHY_TOOL_MASTER_REPORT_LTE_MEAS_IND = 0xE622,
    ID_GPHY_TOOL_MASTER_STOP_LTE_MEAS_IND   = 0xE623,
    ID_GPHY_TOOL_SET_LTE_MEAS_FLAG_IND      = 0xE624,
    ID_GPHY_TOOL_CLEAR_LTE_MEAS_FLAG_IND    = 0xE625,

    ID_GPHY_TOOL_MASTER_WAKE_TDS_IND        = 0xE630,
    ID_GPHY_TOOL_MASTER_START_TDS_MEAS_IND  = 0xE631,
    ID_GPHY_TOOL_MASTER_REPORT_TDS_MEAS_IND = 0xE632,
    ID_GPHY_TOOL_MASTER_STOP_TDS_MEAS_IND   = 0xE633,
    ID_GPHY_TOOL_SET_TDS_MEAS_FLAG_IND      = 0xE634,
    ID_GPHY_TOOL_CLEAR_TDS_MEAS_FLAG_IND    = 0xE635,

    /* ˫�����о������Ϣ�ϱ� */
    ID_GPHY_TOOL_DUAL_ANT_ESTIMATE_IND = 0xE640,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntEstimateInd  */ /* �ϱ����������������׶��о���Ϣ */
    ID_GPHY_TOOL_DUAL_ANT_COMPARE_IND = 0xE641,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntCompareInd  */ /* �ϱ��������������ȽϽ׶�ÿ������״̬���о���Ϣ */
    ID_GPHY_TOOL_DUAL_ANT_DELAY_IND = 0xE642,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntDelayInd  */ /* �ϱ������л�������Ϣ */
    ID_GPHY_TOOL_DUAL_ANT_CPMPARE_SUB_IND = 0xE643,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntCompareSubInd  */ /* ���ײ����㷨ÿ���ϱ������������������ȽϽ׶��о���Ϣ */
    ID_GPHY_TOOL_MAS_CHANGE_ANT_IND = 0xE644,
    /* _H2ASN_MsgChoice  GPHY_TOOL_MasChangeAntInd  */ /* MAS�л�������Ϣ */

    ID_TOOL_GPHY_GSDR_STATE_REPORT_REQ = 0xE650,
    /* _H2ASN_MsgChoice  TOOL_GPHY_GSDR_STATE_REQ_STRU */ /* ���������������ϱ�����С����6ǿ����������ȣ�����ֵ */
    ID_GPHY_TOOL_GSDR_STATE_REPORT_CNF = 0xE651,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GSDR_STATE_CNF_STRU */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_GSDR_STATE_REPORT_IND = 0xE652,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GsdrStateInd  */ /* �ϱ�����С����6ǿ����������ȣ�����ֵ */

    ID_TOOL_GPHY_GSDR_FORCE_CTRL_REQ = 0xE653,
    /* _H2ASN_MsgChoice  TOOL_GPHY_GSDR_FORCE_CTRL_REQ_STRU */ /* ���������������ϱ�����С����6ǿ����������ȣ�����ֵ */
    ID_GPHY_TOOL_GSDR_FORCE_CTRL_CNF = 0xE654,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GSDR_FORCE_CTRL_CNF_STRU */ /* �ظ�ȷ����Ϣ */

    ID_TOOL_GPHY_GSDR_PARA_DUMP_REPORT_REQ = 0xE655,
    /* _H2ASN_MsgChoice  TOOL_GPHY_GSDR_STATE_REQ_STRU */ /* ���������������ϱ�����С����6ǿ����������ȣ�����ֵ */
    ID_GPHY_TOOL_GSDR_PARA_DUMP_REPORT_CNF = 0xE656,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GSDR_STATE_CNF_STRU */ /* �ظ�ȷ����Ϣ */

    ID_GPHY_TOOL_RF_RESET_REPORT_IND = 0xE657,
    /* _H2ASN_MsgChoice  GPHY_TOOL_RfResetReport  */ /* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_GSM_PD_REPORT_IND           = 0xE658,
    ID_GPHY_TOOL_NCELL_RESEL_TIMER_STATE_IND = 0xE659,
    /* _H2ASN_MsgChoice  GPHY_TOOL_NcellReselTimerState  */ /* ����������c1��c2��ѡ��ʱ����ͣ�ϱ� */
    ID_GPHY_TOOL_NCELL_RESEL_TIMER_EXPIRED_IND = 0xE660,
    /* _H2ASN_MsgChoice  GPHY_TOOL_NcellReselTimerExpired  */ /* ����������c1��c2��ѡ��ʱ����ʱ�ϱ� */

    ID_GPHY_TOOL_GSDR_DUMP_SB_DATA_IND = 0xE661,

    ID_GPHY_TOOL_TIME_OUT_RPT_IND = 0xE662,
    /* _H2ASN_MsgChoice  GPHY_TOOL_TimeOutRptInd  */ /* ���������ڸ��ع����쳣ʱ�ϱ� */

    ID_GPHY_TOOL_INTERRAT_OPTIMIZATION_IND = 0xE663,
    /* _H2ASN_MsgChoice  GPHY_TOOL_InterratOptimizationInt  */ /* ��ϵͳ�Ż���ش�ӡ */

    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_IN_MNTN_INFO = 0xE664,
    /* _H2ASN_MsgChoice  GL1C_SDR_ENC_ALG_IN_STRU */ /* ���������ڱ���ǰ���� */
    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_IN_SUB_BLK_INFO = 0xE665,
    /* _H2ASN_MsgChoice  GSDR_ENCODE_SUB_BLK_RPT_STRU */ /* ���������ڱ���ǰ�ӿ鹴�� */
    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_OUT_MNTN_INFO = 0xE666,
    /* _H2ASN_MsgChoice  GL1C_SDR_ENC_ALG_OUT_STRU */ /* ���������ڱ���󹴰� */
    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_OUT_SUB_BLK_INFO = 0xE667,
    /* _H2ASN_MsgChoice  GSDR_ENCODE_SUB_BLK_RPT_STRU */ /* ���������ڱ�����ӿ鹴�� */

    ID_GPHY_TOOL_CS_SACCH_TX_INFO_IND = 0xE668, /* _H2ASN_MsgChoice GSM_CHN_SACCH_UL_INFO_RPT_STRU */

    ID_GPHY_TOOL_GTC_STOP_INFO_IND            = 0xE669, /* _H2ASN_MsgChoice GSM_GRA_GTC_STOP_INFO_RPT_STRU */
    ID_GPHY_TOOL_CELL_SELECT_PREEMPT_INFO_IND = 0xE66A, /* _H2ASN_MsgChoice COM_CELL_SELECT_PREEMPT_INFO_STRU */
    ID_GPHY_TOOL_FULL_LIST_PREEMPT_INFO_IND   = 0xE66B, /* _H2ASN_MsgChoice COM_FULL_LIST_PREEMPT_INFO_STRU */

    ID_TOOL_GPHY_PS_CHANQUAL_REPORT_REQ = 0xE66C, /* _H2ASN_MsgChoice TOOL_GPHY_PsChanqualReportReq  */
    ID_GPHY_TOOL_PS_CHANQUAL_REPORT_CNF = 0xE66D,
    ID_GPHY_TOOL_PS_CHANQUAL_REPORT_IND = 0xE66E, /* _H2ASN_MsgChoice GSM_MEA_PS_CHANQUAL_REPORT_STRU */

    ID_TOOL_GPHY_PS_UL_RPT_REQ           = 0xE66F, /* _H2ASN_MsgChoice TOOL_GPHY_PsUlRptReq  */
    ID_GPHY_TOOL_PS_UL_RPT_CNF           = 0xE670,
    ID_GPHY_TOOL_PS_UL_ENCODE_INFO_IND   = 0xE671, /* _H2ASN_MsgChoice GPHY_TOOL_PS_ENC_BUF_INFO_STRU */
    ID_GPHY_TOOL_PS_UL_MOVE2BUF_INFO_IND = 0xE672, /* _H2ASN_MsgChoice GPHY_TOOL_PS_ENC_MOVE2BUF_INFO_STRU */
    ID_GPHY_TOOL_PS_UL_TXWORK_INFO_IND   = 0xE673, /* _H2ASN_MsgChoice GPHY_TOOL_PS_ENC_TXWORK_INFO_STRU */
    ID_GPHY_TOOL_PS_SRB_BUF_INFO_IND     = 0xE674, /* _H2ASN_MsgChoice GPHY_TOOL_PS_SRB_BUF_INFO_STRU */

    ID_GPHY_TOOL_EVENT_EASYRF_CONTEXT_INFO = 0xE675,
    /* _H2ASN_MsgChoice GPHY_EASYRF_CONTEXT_RPT_STRU */ /* �����������������ù��� */
    ID_GPHY_TOOL_STRONGEST_NCELL_INFO_IND = 0xE676,
    /* _H2ASN_MsgChoice GPHY_TOOL_StrongestNcellInd  */ /* �����������������ù��� */
    ID_GPHY_TOOL_TX_POWER_LIMIT_INFO_IND = 0xE677,
    /* _H2ASN_MsgChoice GPHY_TOOL_TxPowerStatusInd  */ /* �����������������ù��� */
    ID_GPHY_TOOL_GSLAVE_INT0_TIMEOUT_IND = 0xE678, /* _H2ASN_MsgChoice GPHY_TOOL_GSLAVE_INT0_TIMEOUT_IND_STRU */
    ID_GPHY_TOOL_CLT_STATE_RPT_REQ       = 0xE679, /* _H2ASN_MsgChoice GPHY_TOOL_CLT_STATE_RPT_REQ_STRU */
    ID_GPHY_TOOL_RCM_TASK_NO_REL_RPT_IND = 0xE680, /* _H2ASN_MsgChoice GPHY_TOOL_RCM_TASK_NO_REL_IND_STRU  */

    ID_GPHY_TOOL_MEA_RPT_INFO_IND = 0xE681,
    /* _H2ASN_MsgChoice GPHY_ISR_GPHY_MEA_RPT_INFO_STRU */ /* ���������ڲ���������� */
    ID_GPHY_TOOL_SLAVE_MEA_RPT_INFO_IND = 0xE682,
    /* _H2ASN_MsgChoice GPHY_SLAVE_ISR_MEAS_RPT_INFO_STRU */ /* ���������ڴ�ģ����������� */
    ID_GPHY_TOOL_SLAVE_GTC_TASK_INFO_IND = 0xE683,
    /* _H2ASN_MsgChoice GPHY_SLAVE_GTC_CONTEXT_RPT_STRU */ /* ���������ڴ�ģGTC���񹴰� */
    ID_GPHY_TOOL_START_LMEAS_INFO_IND = 0xE684,
    /* _H2ASN_MsgChoice GPHY_TOOL_START_LMEAS_STRU */ /* ���������ڹ��� */
    ID_GPHY_TOOL_START_WMEAS_INFO_IND = 0xE685,
    /* _H2ASN_MsgChoice GPHY_TOOL_START_WMEAS_STRU */ /* ���������ڹ��� */
    ID_GPHY_TOOL_OCCASION_STOP_IND = 0xE686,
    /* _H2ASN_MsgChoice GPHY_TOOL_OCCASION_STOP_STRU */ /* ���������ڹ��� */
    ID_GPHY_TOOL_SUPPLEMENT_RF_TASK_INFO_IND = 0xE687,
    /* _H2ASN_MsgChoice GPHY_TOOL_SUPPLEMENT_RF_TASK_STRU */ /* ���������ڹ��� */
    ID_GPHY_TOOL_FIR2_FILTER_DCH_PARA_IND = 0xE688,
    /* _H2ASN_MsgChoice GPHY_TOOL_FIR2_FILTER_DCH_PARA_STRU */ /* ���������ڹ��� */
    ID_GPHY_TOOL_SLAVE_TASK_OP_RF_INVALID_RPT = 0xE689,
    /* _H2ASN_MsgChoice GPHY_SLAVE_TASK_OP_RF_INVALID_RPT_STRU */ /* ���������ڴ�ģ�ڹ��������Ƶ�Ĺ��� */
    ID_GPHY_TOOL_SLAVE_GAP_INFO_IND = 0xE68A,
    /* _H2ASN_MsgChoice GPHY_SLAVE_GAP_INFO_RPT_STRU */ /* ���������ڴ�ģ������Ϣ�Ĺ��� */
    ID_GPHY_TOOL_SLAVE_SB_FORECAST_INFO_IND = 0xE68B,
    /* _H2ASN_MsgChoice GPHY_SLAVE_SB_FORECAST_INFO_RPT_STRU */ /* ���������ڴ�ģsb����ʱ��Ϣ�Ĺ��� */
    ID_GPHY_TOOL_GTC_FN_RUNTIME_INFO_IND = 0xE68C,
    /* _H2ASN_MsgChoice GPHY_TOOL_GTC_FN_RUNTIME_INFO_IND_STRU */ /* ���������ڹ��� */
    ID_GPHY_TOOL_MODEM_STATUS_INFO_IND = 0xE68D,
    /* _H2ASN_MsgChoice GPHY_TOOL_MODEM_STATUS_INFO_STRU */ /* ���������ڹ��� */
    ID_GPHY_TOOL_CANCEL_TASK_CONTEXT_INFO = 0xE68E,
    /* _H2ASN_MsgChoice GPHY_EASYRF_CancelTaskContextRpt */ /* ���������ڹ��� */
    ID_GPHY_TOOL_ANT_TASMAS_INFO_IND = 0xE69E,
    /* _H2ASN_MsgChoice GPHY_TOOL_ANT_TASMAS_INFO_STRU */ /* ���������ڹ��� */

    ID_GPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnMsgIdUint16;

/*
 * GPHY�ظ�CNFʱ,Я���Ĵ�����ö��
 */
enum GPHY_TOOL_MntnRslt {
    GPHY_MNTN_RSLT_SUCC = 0,
    GPHY_MNTN_RSLT_FAIL = 1,

    GPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnRsltUint16;

/*
 * �ṹ˵��: ����bbp������Դʱ��������ʽö��
 */
enum GPHY_TOOL_BbpOperateType {
    GPHY_TOOL_BBP_WR = 0,
    GPHY_TOOL_BBP_RD = 1,

    GPHY_TOOL_BBP_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_BbpOperateTypeUint16;

/*
 * ö��˵��: �����״̬ö�ٶ���,�����ϱ�
 */
enum GPHY_TOOL_PhyStates {
    GPHY_TOOL_PHY_NULL = 0,      /* ����㴦��NULL̬ */
    GPHY_TOOL_PHY_SEARCHING_BCH, /* ����㴦��SearchingBCH̬ */
    GPHY_TOOL_PHY_BCH,           /* ����㴦��BCH̬ */
    GPHY_TOOL_PHY_TUNING_DCH,    /* ����㴦��TuningDCH̬ */
    GPHY_TOOL_PHY_DCH,           /* ����㴦��DCH̬ */
    GPHY_TOOL_PHY_SLAVE,         /* ����㴦���ģSLAVE̬ */
    GPHY_TOOL_PHY_STATES_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_PhyStatesUint16;

/*
 * TOOL�·��������ƣ���Ҫ�ǹ��ʿ��ƺ�BEP����
 */
enum GPHY_TOOL_ParaCtrl {
    GPHY_TOOL_PARA_PWC_CTRL = 0x01, /* �·����ƹ��ʲ��� */
    GPHY_TOOL_PARA_BEP_CTRL = 0x02, /* �·�����BEP���� */

    GPHY_TOOL_PARA_CTRL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_ParaCtrlUint16;

/*
 * ö��˵��: GPHY�����ϱ�����
 */
enum GPHY_TOOL_ErrorRptLevel {
    GPHY_TOOL_ERROR_LEVEL_INFO    = 1,
    GPHY_TOOL_ERROR_LEVEL_WARNING = 2,
    GPHY_TOOL_ERROR_LEVEL_ERROR   = 3,

    GPHY_TOOL_ERROR_LEVEL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_ErrorRptLevelUint16;

/*
 * ö��˵��: ǿͣ��ģ��ԭ��
 */
enum GPHY_TOOL_MntnMasterStopSlaveReason {
    GPHY_TOOL_MNTN_MASTER_STOP_WCDMA_MEAS = 0,
    GPHY_TOOL_MNTN_MASTER_STOP_WCDMA_BG   = 1,
    GPHY_TOOL_MNTN_MASTER_STOP_LTE_MEAS   = 2,
    GPHY_TOOL_MNTN_MASTER_STOP_LTE_BG     = 3,

    GPHY_TOOL_MNTN_MASTER_STOP_SLAVE_REASION_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnMasterStopSlaveReasonUint16;

/*
 * GPHYģ�鶨��
 */

enum GPHY_TOOL_Module {
    GPHY_MODULE_DRV          = 0x5501, /* DRIVERģ������� */
    GPHY_MODULE_MEA          = 0x5502, /* ����ģ������� */
    GPHY_MODULE_NCE_TSK      = 0x5503, /* ��������ģ������� */
    GPHY_MODULE_NCE_RES      = 0x5504, /* ������ѡģ������� */
    GPHY_MODULE_CHN_CCH      = 0x5505, /* �����ŵ�ģ������� */
    GPHY_MODULE_CHN_CSDCH    = 0x5506, /* CSר���ŵ�ģ������� */
    GPHY_MODULE_CHN_PSDCH    = 0x5507, /* PSר���ŵ�ģ������� */
    GPHY_MODULE_CHN_AMRDTX   = 0x5508, /* �ŵ�AMRDTXģ������� */
    GPHY_MODULE_COM          = 0x5509, /* ����ģ������� */
    GPHY_MODULE_COM_MAIL     = 0x550A, /* ��������ģ������� */
    GPHY_MODULE_COM_TOA      = 0x550B, /* ����TOAģ������� */
    GPHY_MODULE_DRX          = 0x550C, /* DRXģ������� */
    GPHY_MODULE_OM           = 0x550D, /* OMģ������� */
    GPHY_MODULE_WMEAS        = 0x550E, /* WMEASģ������� */
    GPHY_MODULE_CAL          = 0x550F, /* CALģ������� */
    GPHY_MODULE_LMEAS        = 0x5510, /* LTE����ģ������� */
    GPHY_MODULE_PD_INTERFACE = 0x5511, /* �µ粿�ִ���ӿں���ģ�� */
    GPHY_MODULE_TMEAS        = 0x5512, /* TD����ģ������� */
    GPHY_MODULE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_ModuleUint16;

/*
 * 3.2 ö�ٶ���,WPHYö�ٶ���
 */

/*
 * WPHY<-->TOOLԭ���б�
 */

enum WPHY_TOOL_MntnMsgId {
    /* �·�REQ��Ϣ��Ϊ��VOS��Ϣ���ϱ���IND��CNF��Ϣ����VOS��Ϣͷ��Ӧ��REQ�������������ָʾΪ��ASN���ɷ��� */
    ID_TOOL_WPHY_REQ = 0x0, /* _H2ASN_MsgChoice TOOL_WPHY_REQ_STRU */

    /* ����ģʽ��ʵʱ�޸�PA�����л��㲹��������ͻظ�ԭ�� */
    ID_TOOL_WPHY_RF_PA_GAIN_SWITCH_COMP_REQ = 0xC801, /* _H2ASN_MsgChoice WPHY_TOOL_PaGainSwitchCompReq  */
    ID_WPHY_TOOL_RF_PA_GAIN_SWITCH_COMP_CNF = 0xC802, /* _H2ASN_MsgChoice WPHY_TOOL_PaGainSwitchCompCnf  */

    /* ����ģʽ��PDʹ�ܿ��� */
    ID_TOOL_WPHY_PD_ENABLE_REQ = 0xC803, /* _H2ASN_MsgChoice WPHY_TOOL_PowDetEnableReq  */
    ID_WPHY_TOOL_PD_ENABLE_CNF = 0xC804, /* _H2ASN_MsgChoice WPHY_TOOL_PowDetEnableCnf  */

    /* ���С���ϱ� */
    ID_TOOL_WPHY_ACT_CELL_REQ = 0xF001, /* _H2ASN_MsgChoice WPHY_TOOL_ActCellReq  */
    ID_WPHY_TOOL_ACT_CELL_CNF = 0xF002, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_ACT_CELL_IND = 0xF003, /* _H2ASN_MsgChoice WPHY_TOOL_ActCellInfoInd  */

    /* С�������б��ϱ� */
    ID_TOOL_WPHY_CELL_SEARCH_LIST_REQ = 0xF004, /* _H2ASN_MsgChoice WPHY_TOOL_CellSearchListReq   */
    ID_WPHY_TOOL_CELL_SEARCH_LIST_CNF = 0xF005, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_CELL_SEARCH_LIST_IND = 0xF006, /* _H2ASN_MsgChoice WPHY_TOOL_CellSearchListInd  */

    /* ���ؿ��ؿ��� */
    ID_TOOL_WPHY_PWR_SWITCH_CTRL_REQ = 0xF00A, /* _H2ASN_MsgChoice WPHY_TOOL_PwrSwitchCtrlReq  */
    ID_WPHY_TOOL_PWR_SWITCH_CTRL_CNF = 0xF00B, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* ���ز������� */
    ID_TOOL_WPHY_PWR_PARA_SET_REQ = 0xF010, /* _H2ASN_MsgChoice WPHY_TOOL_PwrParaSetReq  */
    ID_WPHY_TOOL_PWR_PARA_SET_CNF = 0xF011, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* ������ϱ� */
    ID_TOOL_WPHY_BLER_REPORT_REQ = 0xF020, /* _H2ASN_MsgChoice WPHY_TOOL_BlerReportReq  */
    ID_WPHY_TOOL_BLER_REPORT_CNF = 0xF021, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_BLER_REPORT_IND = 0xF022, /* _H2ASN_MsgChoice WPHY_TOOL_BlerReportInd  */

    /* �¼��ϱ� */
    ID_TOOL_WPHY_EVENT_REPORT_REQ = 0xF02A, /* _H2ASN_MsgChoice WPHY_TOOL_EventReportReq  */
    ID_WPHY_TOOL_EVENT_REPORT_CNF = 0xF02B, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* �¼��ϱ�:ͬʧ���ϱ� */
    ID_WPHY_TOOL_SYNC_STATUS_IND = 0xF02C, /* _H2ASN_MsgChoice WPHY_TOOL_SyncStatusInd  */

    /* �¼��ϱ�:AFC״̬ */
    ID_WPHY_TOOL_VCTCXO_STATUS_IND = 0xF02E, /* _H2ASN_MsgChoice WPHY_TOOL_AfcStatusInd  */

    /* �¼��ϱ�:�ж������쳣���� */
    ID_WPHY_TOOL_INTERRUPT_EXCEPTIION_IND = 0xF030, /* _H2ASN_MsgChoice WPHY_TOOL_InterruptExceptiionInd  */

    /* �¼��ϱ�:L1״̬�ϱ� */
    ID_WPHY_TOOL_WCDMA_L1_STATUS_IND = 0xF031, /* _H2ASN_MsgChoice WPHY_TOOL_WCDMA_L1_STATUS_IND_STRU */

    /* DRX���� */
    ID_TOOL_WPHY_DRX_SWITCH_REQ = 0xF032, /* _H2ASN_MsgChoice WPHY_TOOL_DrxSwitchReq  */
    ID_WPHY_TOOL_DRX_SWITCH_CNF = 0xF033, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* �������� */
    ID_TOOL_WPHY_THRESHOLD_SET_REQ = 0xF034, /* _H2ASN_MsgChoice WPHY_TOOL_ThresholdSetReq  */
    ID_WPHY_TOOL_THRESHOLD_SET_CNF = 0xF035, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* ���޲�ѯ */
    ID_TOOL_WPHY_THRESHOLD_QUERY_REQ = 0xF036, /* _H2ASN_MsgChoice WPHY_TOOL_ThresholdQueryReq  */
    ID_WPHY_TOOL_THRESHOLD_QUERY_IND = 0xF037, /* _H2ASN_MsgChoice WPHY_TOOL_ThresholdQueryInd  */

    /* �����ϱ� */
    ID_TOOL_WPHY_ERROR_REPORT_REQ   = 0xF038, /* _H2ASN_MsgChoice WPHY_TOOL_ErrorReportReq  */
    ID_WPHY_TOOL_ERROR_REPORT_CNF   = 0xF039, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_ERROR_REPORT_IND   = 0xF03A, /* _H2ASN_MsgChoice WPHY_TOOL_ErrorReportInd   */
    ID_WPHY_TOOL_WARNING_REPORT_IND = 0xF03B, /* _H2ASN_MsgChoice WPHY_TOOL_ErrorReportInd   */
    ID_WPHY_TOOL_INFO_REPORT_IND    = 0xF03C, /* _H2ASN_MsgChoice WPHY_TOOL_ErrorReportInd   */

    ID_TOOL_WPHY_RF_FRONT_END_PARA_REQ = 0xF03D, /* _H2ASN_MsgChoice WPHY_TOOL_RfFrontEndParaReq   */
    ID_WPHY_TOOL_RF_FRONT_END_PARA_CNF = 0xF03E, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf   */
    ID_WPHY_TOOL_RF_FRONT_END_PARA_IND = 0xF03F, /* _H2ASN_MsgChoice WPHY_TOOL_RfFrontEndParaInd   */

    ID_WPHY_TOOL_PI_INT_RESULT_IND = 0xF040, /* _H2ASN_MsgChoice WPHY_TOOL_PiIntResultInd   */
    ID_WPHY_TOOL_AI_INT_RESULT_IND = 0xF041, /* _H2ASN_MsgChoice WPHY_TOOL_AiIntResultInd   */

    /* T313 ״̬�ϱ� */
    ID_WPHY_TOOL_T313_STATUS_IND = 0xF042, /* _H2ASN_MsgChoice WPHY_TOOL_T313_STATUS_IND_STRU  */
    /* T312 ״̬�ϱ� */
    ID_WPHY_TOOL_T312_STATUS_IND = 0xF043, /* _H2ASN_MsgChoice WPHY_TOOL_T312_STATUS_IND_STRU */

    /* PA״̬�ϱ� */
    ID_TOOL_WPHY_PA_STATUS_REPORT_REQ = 0xF046, /* _H2ASN_MsgChoice WPHY_TOOL_PaStatusReportReq  */
    ID_WPHY_TOOL_PA_STATUS_REPORT_CNF = 0xF047, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_PA_STATUS_REPORT_IND = 0xF048, /* _H2ASN_MsgChoice WPHY_TOOL_PaStatusReportInd  */

    ID_TOOL_WPHY_INTRA_MSG_RPT_CTRL_REQ = 0xF049, /* _H2ASN_MsgChoice WPHY_TOOL_IntraMsgRptCtrlReq  */
    ID_WPHY_TOOL_INTRA_MSG_RPT_CTRL_CNF = 0xF04A, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* ά����Ϣbitmap���� */
    ID_WPHY_TOOL_FUNC_BIT_CONTROL_REQ = 0xF050, /* _H2ASN_MsgChoice  WPHY_TOOL_FuncBitControlReq  */
    ID_WPHY_TOOL_FUNC_BIT_CONTROL_CNF = 0xF051, /* _H2ASN_MsgChoice  WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_FET_OPEN_RPT_IND     = 0xF052, /* _H2ASN_MsgChoice WPHY_TOOL_FetOpenInd  */
    ID_WPHY_TOOL_FET_CLOSE_RPT_IND    = 0xF053, /* _H2ASN_MsgChoice WPHY_TOOL_FetCloseInd  */
    ID_WPHY_TOOL_FREE_MNTN_RPT_IND    = 0xF054, /* _H2ASN_MsgChoice WPHY_TOOL_FreeMntnRpt  */

    ID_WPHY_TOOL_SAR_TIME_AVG_DPR_ON_IND    = 0xF055, /* _H2ASN_MsgChoice WPHY_TOOL_SarTimeAvgDprEventInd  */
    ID_WPHY_TOOL_SAR_TIME_AVG_DPR_OFF_IND   = 0xF056, /* _H2ASN_MsgChoice WPHY_TOOL_SarTimeAvgDprEventInd  */
    ID_WPHY_TOOL_SAR_TIME_AVG_PARA_INFO_IND = 0xF057, /* _H2ASN_MsgChoice WPHY_TOOL_SarTimeAvgParaInfo  */

    ID_TOOL_WPHY_READ_MEM_REQ = 0xF058, /* _H2ASN_MsgChoice WPHY_TOOL_ReadMemReq  */
    ID_WPHY_TOOL_READ_MEM_CNF = 0xF059, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_READ_MEM_IND = 0xF05A, /* _H2ASN_MsgChoice WPHY_TOOL_ReadMemInd  */

    ID_TOOL_WPHY_MNTN_FUNC_SWTICH_REQ = 0xF05B, /* _H2ASN_MsgChoice WPHY_TOOL_MntnFuncSwtichReq  */

    ID_WPHY_TOOL_CLT_CROSSMIPI_RPT_IND   = 0xF05C, /* _H2ASN_MsgChoice WPHY_TOOL_CltStatusRptInd  */
    ID_WPHY_TOOL_CLT_OL_RPT_IND          = 0xF05D, /* _H2ASN_MsgChoice WPHY_TOOL_CltStatusRptInd  */
    ID_WPHY_TOOL_CLT_CL_RPT_IND          = 0xF05E, /* _H2ASN_MsgChoice WPHY_TOOL_CltStatusRptInd  */
    ID_WPHY_TOOL_CLT_DETECT_FAIL_RPT_IND = 0xF05F, /* _H2ASN_MsgChoice WPHY_TOOL_CltDetectFailRptInd  */

    /* ǰ�˿�ά�ɲ� */
    ID_TOOL_WPHY_RF_FRONT_STATIC_REQUIRE_REQ = 0xF070, /* _H2ASN_MsgChoice WPHY_TOOL_RfFrontEndStaticRequireReq   */
    ID_WPHY_TOOL_RF_FRONT_STATIC_REQUIRE_IND = 0xF071, /* _H2ASN_MsgChoice WPHY_TOOL_RfFrontEndStaticRequireInd   */

    ID_TOOL_WPHY_BBP_RTC_REQ = 0xF072, /* _H2ASN_MsgChoice WPHY_TOOL_BbpRtcReq   */
    ID_WPHY_TOOL_BBP_RTC_CNF = 0xF073, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf   */

    ID_TOOL_WPHY_RF_FRONT_DISPLAY_REQ = 0xF075, /* _H2ASN_MsgChoice WPHY_TOOL_RfFrontEndDisplayReq   */
    ID_WPHY_TOOL_RF_FRONT_DISPLAY_CNF = 0xF077, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    ID_TOOL_WPHY_RAM_REPORT_REQ = 0xF078, /* _H2ASN_MsgChoice WPHY_TOOL_RamReportReq   */
    ID_WPHY_TOOL_RAM_REPORT_CNF = 0xF079, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf   */
    ID_WPHY_TOOL_RAM_REPORT_IND = 0xF07A, /* _H2ASN_MsgChoice WPHY_TOOL_RamReportInd   */

    ID_TOOL_WPHY_BLK_REPORT_LIST_REQ = 0xF07B, /* _H2ASN_MsgChoice WPHY_TOOL_BlkReportListReq   */
    ID_WPHY_TOOL_BLK_REPORT_LIST_CNF = 0xF07C, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf   */
    ID_WPHY_TOOL_BLK_REPORT_LIST_IND = 0xF07D, /* _H2ASN_MsgChoice WPHY_TOOL_BlkReportListInd   */

    ID_TOOL_WPHY_RA_MNTN_CTRL_REQ = 0xF07E, /* _H2ASN_MsgChoice WPHY_TOOL_RaMntnCtrlReq  */
    ID_WPHY_TOOL_RA_MNTN_CTRL_CNF = 0xF07F, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf   */

    ID_TOOL_WPHY_ARM_0MS_MNTN_CTRL_REQ = 0xF080, /* _H2ASN_MsgChoice WPHY_TOOL_ARM_0MS_MNTN_CTRL_REQ_STRU */
    ID_WPHY_TOOL_ARM_0MS_MNTN_CTRL_CNF = 0xF081, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf   */

    ID_WPHY_TOOL_SLAVE_CDRX_START_IND       = 0xF082, /* _H2ASN_MsgChoice WPHY_TOOL_SlaveCdrxStartInd   */
    ID_WPHY_TOOL_SLAVE_CDRX_STOP_IND        = 0xF083, /* _H2ASN_MsgChoice WPHY_TOOL_SlaveCdrxStopInd   */
    ID_WPHY_TOOL_SLAVE_W_FINISH_IND         = 0xF084, /* _H2ASN_MsgChoice WPHY_TOOL_SlaveWFinishInd  */
    ID_WPHY_TOOL_SLAVE_GRSSI_FINISH_IND     = 0xF085, /* _H2ASN_MsgChoice WPHY_TOOL_SlaveGrssiFinishInd  */
    ID_WPHY_TOOL_SLAVE_GBSIC_FINISH_IND     = 0xF086, /* _H2ASN_MsgChoice WPHY_TOOL_SlaveGbsicFinishInd  */
    ID_WPHY_TOOL_SLAVE_SCHEDULE_TIMEOUT_IND = 0xF087, /* _H2ASN_MsgChoice WPHY_TOOL_SlaveScheduleTimeoutInd  */
    ID_WPHY_TOOL_SLAVE_MNTN_RPT_IND         = 0xF088, /* _H2ASN_MsgChoice WPHY_TOOL_SlaveMntnRptInd  */

    ID_WPHY_TOOL_MASTER_WAKE_SLAVE_IND      = 0xF090, /* _H2ASN_MsgChoice WPHY_TOOL_MasterWakeSlaveInd  */
    ID_WPHY_TOOL_MASTER_START_LTE_MEAS_IND  = 0xF091, /* _H2ASN_MsgChoice WPHY_TOOL_MasterStartLteMeasInd  */
    ID_WPHY_TOOL_MASTER_REPORT_LTE_MEAS_IND = 0xF092, /* _H2ASN_MsgChoice WPHY_TOOL_MasterReportLteMeasInd  */
    ID_WPHY_TOOL_MASTER_STOP_SLAVE_MEAS_IND = 0xF093, /* _H2ASN_MsgChoice WPHY_TOOL_MasterStopSlaveMeasInd  */
    ID_WPHY_TOOL_SET_LTE_MEAS_FLAG_IND      = 0xF094, /* _H2ASN_MsgChoice WPHY_TOOL_SetLteMeasFlagInd  */
    ID_WPHY_TOOL_MASTER_RF_CONFIG_IND       = 0xF095, /* _H2ASN_MsgChoice WPHY_TOOL_SetLteMeasFlagInd  */
    ID_WPHY_TOOL_UPA_INFO_RPT_IND           = 0xF096, /* _H2ASN_MsgChoice WPHY_TOOL_UpaRptInfo  */
    ID_WPHY_TOOL_MASTER_START_GSM_MEAS_IND  = 0xF097, /* _H2ASN_MsgChoice WPHY_TOOL_MasterStartLteMeasInd  */

    ID_WPHY_TOOL_QPC_OPEN_UL_IND   = 0xF0A0, /* _H2ASN_MsgChoice WPHY_TOOL_QpcOpenUlInd  */
    ID_WPHY_TOOL_QPC_CLOSE_UL_IND  = 0xF0A1, /* _H2ASN_MsgChoice WPHY_TOOL_QpcCloseUlInd  */
    ID_WPHY_TOOL_QPC_RRC_STATE_IND = 0xF0A2, /* _H2ASN_MsgChoice WPHY_TOOL_QpcRrcStateInd  */
    ID_WPHY_TOOL_QPC_RB_TYPE_IND   = 0xF0A3, /* _H2ASN_MsgChoice WPHY_TOOL_QpcRbTypeInd  */

    ID_WPHY_TOOL_MAS_MNTN_STATEI_IND    = 0xF0B0, /* _H2ASN_MsgChoice WPHY_TOOL_MasMntnStateiInd  */
    ID_WPHY_TOOL_MAS_MNTN_STATEIAIB_IND = 0xF0B1, /* _H2ASN_MsgChoice WPHY_TOOL_MasMntnStateiaibInd  */
    ID_WPHY_TOOL_MAS_MNTN_STATEII_IND   = 0xF0B2, /* _H2ASN_MsgChoice WPHY_TOOL_MasMntnStateiiInd  */

    ID_WPHY_TOOL_IDLE_MNTN_RPT_IND = 0xF0B3, /* _H2ASN_MsgChoice WPHY_TOOL_IdleMntnRpt  */
    ID_WPHY_TOOL_FACH_MNTN_RPT_IND = 0xF0B4, /* _H2ASN_MsgChoice WPHY_TOOL_FachMntnRpt  */
    ID_WPHY_TOOL_DCH_MNTN_RPT_IND  = 0xF0B5, /* _H2ASN_MsgChoice WPHY_TOOL_DchMntnRpt  */

    ID_WPHY_TOOL_TAS_STATUS_STATE_REPORT_IND = 0xF0B6, /* _H2ASN_MsgChoice WPHY_TOOL_TasStatusReportInd  */

    ID_WPHY_TOOL_PCCPCH_SCHEDULE_IND = 0xF0B7, /* _H2ASN_MsgChoice WPHY_TOOL_RakePccpchSchdlInd  */
    ID_WPHY_TOOL_BCCH_QUEUE_IND      = 0xF0B8, /* _H2ASN_MsgChoice WPHY_TOOL_RakeBcchQueueInd  */
    ID_WPHY_TOOL_ADEMOD_QUEUE_IND    = 0xF0B9, /* _H2ASN_MsgChoice WPHY_TOOL_RakeAdemodQueueInd  */
    ID_WPHY_TOOL_CPC_MNTN_RPT_IND    = 0xF0BA, /* _H2ASN_MsgChoice WPHY_TOOL_CpcMntnRpt  */

    ID_WPHY_TOOL_IC_CELL_LIST_RPT_REQ = 0xF0BB, /* _H2ASN_MsgChoice WPHY_TOOL_IcCellReportReq  */
    ID_WPHY_TOOL_IC_CELL_LIST_RPT_CNF = 0xF0BC, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_IC_CELL_LIST_RPT_IND = 0xF0BD, /* _H2ASN_MsgChoice WPHY_TOOL_IcCellReportInd  */

    ID_WPHY_TOOL_RTT_STATUS_RPT_REQ = 0xF0BE, /* _H2ASN_MsgChoice WPHY_TOOL_RttStatusReportReq  */
    ID_WPHY_TOOL_RTT_STATUS_RPT_CNF = 0xF0BF, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_RTT_STATUS_RPT_IND = 0xF0C0, /* _H2ASN_MsgChoice WPHY_TOOL_RttStatusReportInd  */

    ID_WPHY_TOOL_DPA_INFO_RPT_REQ = 0xF0C1, /* _H2ASN_MsgChoice WPHY_TOOL_DPA_INFO_REQ_STRU */
    ID_WPHY_TOOL_DPA_INFO_RPT_CNF = 0xF0C2, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_DPA_INFO_RPT_IND = 0xF0C3, /* _H2ASN_MsgChoice WPHY_TOOL_DpaInfoInd  */

    ID_WPHY_TOOL_HSSCCH_STATISTIC_RPT_REQ = 0xF0C4, /* _H2ASN_MsgChoice WPHY_TOOL_HsscchStatisticReportReq  */
    ID_WPHY_TOOL_HSSCCH_STATISTIC_RPT_CNF = 0xF0C5, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_HSSCCH_STATISTIC_RPT_IND = 0xF0C6, /* _H2ASN_MsgChoice WPHY_TOOL_HsscchStatisticReportInd  */

    ID_WPHY_TOOL_HSDSCH_STATISTIC_RPT_REQ = 0xF0C7, /* _H2ASN_MsgChoice WPHY_TOOL_HsdschStatisticReportReq  */
    ID_WPHY_TOOL_HSDSCH_STATISTIC_RPT_CNF = 0xF0C8, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_HSDSCH_STATISTIC_RPT_IND = 0xF0C9, /* _H2ASN_MsgChoice WPHY_TOOL_HsdschStatisticReportInd  */
    ID_WPHY_TOOL_RX_FAST_CAL_MNTN_RPT_IND = 0xF0CA, /* _H2ASN_MsgChoice WPHY_TOOL_RxFastCalMntnRpt  */
    ID_WPHY_TOOL_AT_TRX_CAL_MNTN_RPT_IND  = 0xF0CB, /* _H2ASN_MsgChoice WPHY_TOOL_AtTrxCalMntnRpt  */
    ID_WPHY_TOOL_TX_FAST_CAL_MNTN_RPT_IND = 0xF0CD, /* _H2ASN_MsgChoice WPHY_TOOL_TxFastCalMntnRpt  */

    ID_WPHY_TOOL_RPT_CAL_SAMPLE_DATA_IND = 0xF0CE, /* _H2ASN_MsgChoice WPHY_TOOL_RptCalSampleDataMntn  */
    ID_WPHY_TOOL_NO_SIGNAL_MNTN_IND      = 0xF0CF, /* _H2ASN_MsgChoice WPHY_TOOL_NoSignalMntnRpt  */

    ID_WPHY_TOOL_MAS_MNTN_STATEIII_IND     = 0xF0E1, /* _H2ASN_MsgChoice WPHY_TOOL_MasMntnStateiiiInd  */
    ID_WPHY_TOOL_MAS_MNTN_NONBLINDINFO_IND = 0xF0E2, /* _H2ASN_MsgChoice WPHY_TOOL_MasMntnNonblindinfoInd  */
    ID_WPHY_TOOL_MAS_MNTN_MAS_SWITCH_IND   = 0xF0E3, /* _H2ASN_MsgChoice WPHY_TOOL_MasMntnNonblindinfoInd  */

    ID_WPHY_TOOL_PILOT_BER_RPT_REQ = 0xF0E4, /* _H2ASN_MsgChoice WPHY_TOOL_PilotBerReportReq  */
    ID_WPHY_TOOL_PILOT_BER_RPT_CNF = 0xF0E5, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_PILOT_BER_RPT_IND = 0xF0E6, /* _H2ASN_MsgChoice WPHY_TOOL_PilotBerReportInd */

    ID_WPHY_TOOL_TPC_AND_SIR_RPT_REQ = 0xF0E7, /* _H2ASN_MsgChoice WPHY_TOOL_TpcAndSirReportReq  */
    ID_WPHY_TOOL_TPC_AND_SIR_RPT_CNF = 0xF0E8, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_TPC_AND_SIR_RPT_IND = 0xF0E9, /* _H2ASN_MsgChoice WPHY_TOOL_TpcAndSirReportInd  */

    ID_WPHY_TOOL_POWER_RPT_REQ = 0xF0EA, /* _H2ASN_MsgChoice WPHY_TOOL_PowerReportReq  */
    ID_WPHY_TOOL_POWER_RPT_CNF = 0xF0EB, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_POWER_RPT_IND = 0xF0EC, /* _H2ASN_MsgChoice WPHY_TOOL_PowerReportInd  */

    ID_WPHY_TOOL_HSDPA_LINK_STATISTICS_RPT_REQ = 0xF0ED, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaLinkStatisticsReportReq  */
    ID_WPHY_TOOL_HSDPA_LINK_STATISTICS_RPT_CNF = 0xF0EE, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_HSDPA_LINK_STATISTICS_RPT_IND = 0xF0EF, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaLinkStatisticsReportInd  */

    /* _H2ASN_MsgChoice WPHY_TOOL_DualAntennaStatisticsReportReq */
    ID_WPHY_TOOL_DUAL_ANTENNA_STATISTICS_RPT_REQ = 0xF0F0,
    ID_WPHY_TOOL_DUAL_ANTENNA_STATISTICS_RPT_CNF = 0xF0F1, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    /* _H2ASN_MsgChoice WPHY_TOOL_DualAntennaStatisticsReportInd */
    ID_WPHY_TOOL_DUAL_ANTENNA_STATISTICS_RPT_IND = 0xF0F2,

    ID_WPHY_TOOL_HSUPA_EDCH_ACTIVE_SET_RPT_REQ = 0xF0F3, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaEdchActiveSetReportReq  */
    ID_WPHY_TOOL_HSUPA_EDCH_ACTIVE_SET_RPT_CNF = 0xF0F4, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_HSUPA_EDCH_ACTIVE_SET_RPT_IND = 0xF0F5, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaEdchActiveSetReportInd  */

    /* ������������ */
    ID_WPHY_TOOL_INTERFE_TX_BLANKING_IND   = 0xF0F6,
    ID_WPHY_TOOL_CHANNEL_REPORT_IND        = 0xF0F7, /* _H2ASN_MsgChoice WPHY_TOOL_ChannelReportInd  */
    ID_WPHY_TOOL_ERACH_MNTN_RPT_IND        = 0xF0F8, /* _H2ASN_MsgChoice WPHY_TOOL_ErachMntnRpt  */
    ID_WPHY_TOOL_RF_ERROR_RESET_REPORT_IND = 0xF0F9, /* _H2ASN_MsgChoice WPHY_TOOL_RfErrorResetReport  */
    ID_WPHY_TOOL_EDRX_MNTN_T321_CTRL_IND   = 0xF0fa, /* _H2ASN_MsgChoice WPHY_TOOL_EDRX_MNTN_T321_CTRL_REQ_STRU */
    ID_WPHY_TOOL_EDRX_MNTN_RPT_IND         = 0xF0fb, /* _H2ASN_MsgChoice WPHY_TOOL_EdrxMntnRpt  */
    ID_WPHY_TOOL_ANT2_VOTE_LIST_RPT_IND    = 0xF0fc, /* _H2ASN_MsgChoice WPHY_TOOL_ANT2_VOTE_LIST_RPT_STRU */
    ID_WPHY_TOOL_DRIVE_TEST_RPT_CNF        = 0xF100, /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_DT_CNF_STRU */

    ID_WPHY_TOOL_RADIO_PARA_CELL_SNR_RPT_REQ = 0xF101, /* _H2ASN_MsgChoice WPHY_TOOL_RadioParaCellSnrReq  */
    ID_WPHY_TOOL_RADIO_PARA_CELL_SNR_RPT_IND = 0xF102, /* _H2ASN_MsgChoice WPHY_TOOL_RadioParaCellSnrInd  */

    ID_WPHY_TOOL_HSPDSCH_CFG_RPT_REQ = 0xF103, /* _H2ASN_MsgChoice WPHY_TOOL_HspdschCfgReq  */
    ID_WPHY_TOOL_HSPDSCH_CFG_RPT_IND = 0xF104, /* _H2ASN_MsgChoice WPHY_TOOL_HspdschCfgInd  */

    ID_WPHY_TOOL_HSSCCH_CFG_RPT_REQ = 0xF105, /* _H2ASN_MsgChoice WPHY_TOOL_HsscchCfgReq  */
    ID_WPHY_TOOL_HSSCCH_CFG_RPT_IND = 0xF106, /* _H2ASN_MsgChoice WPHY_TOOL_HsscchCfgInd  */

    ID_WPHY_TOOL_HSDPCCH_CFG_RPT_REQ = 0xF107, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpcchCfgReq  */
    ID_WPHY_TOOL_HSDPCCH_CFG_RPT_IND = 0xF108, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpcchCfgInd  */

    ID_WPHY_TOOL_HSDPA_CQI_RPT_REQ = 0xF109, /* _H2ASN_MsgChoice WPHY_TOOL_HsHsdpaCqiReq  */
    ID_WPHY_TOOL_HSDPA_CQI_RPT_IND = 0xF10A, /* _H2ASN_MsgChoice WPHY_TOOL_HsHsdpaCqiInd  */

    ID_WPHY_TOOL_HSDPA_LINK_STAT_RPT_REQ = 0xF10B, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaLinkStatReq  */
    ID_WPHY_TOOL_HSDPA_LINK_STAT_RPT_IND = 0xF10C, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaLinkStatInd  */

    ID_WPHY_TOOL_HSDPA_DECOD_STAT_RPT_REQ = 0xF10D, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaDecodStatReq  */
    ID_WPHY_TOOL_HSDPA_DECOD_STAT_RPT_IND = 0xF10E, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaDecStatInd  */

    ID_WPHY_TOOL_HSDPA_DECOD_TBS_RPT_REQ = 0xF10F, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaDecTbsReq  */
    ID_WPHY_TOOL_HSDPA_DECOD_TBS_RPT_IND = 0xF110, /* _H2ASN_MsgChoice WPHY_TOOL_HsdpaDecTbsInd  */

    ID_WPHY_TOOL_HSUPA_LINK_THROUGHPUT_RPT_REQ = 0xF111, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaLinkThroughputRptReq  */
    ID_WPHY_TOOL_HSUPA_LINK_THROUGHPUT_RPT_IND = 0xF112, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaLinkThroughputRptInd  */

    ID_WPHY_TOOL_HSUPA_LINK_QUALITY_RPT_REQ = 0xF113, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaLinkQualityReq  */
    ID_WPHY_TOOL_HSUPA_LINK_QUALITY_RPT_IND = 0xF114, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaLinkQualityRptInd  */

    ID_WPHY_TOOL_HSUPA_GRANT_RPT_REQ = 0xF115, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaGrantRptReq  */
    ID_WPHY_TOOL_HSUPA_GRANT_RPT_IND = 0xF116, /* _H2ASN_MsgChoice WPHY_TOOL_HsupaGrantRptInd  */

    ID_WPHY_TOOL_PWR_CTRL_PWR_RPT_REQ = 0xF117, /* _H2ASN_MsgChoice WPHY_TOOL_PwrCtrlPwrRptReq  */
    ID_WPHY_TOOL_PWR_CTRL_PWR_RPT_IND = 0xF118, /* _H2ASN_MsgChoice WPHY_TOOL_PwrCtrlPwrRptInd  */

    ID_WPHY_TOOL_PWR_CTRL_TPC_RPT_REQ = 0xF119, /* _H2ASN_MsgChoice WPHY_TOOL_PwrCtrlTpcRptReq  */
    ID_WPHY_TOOL_PWR_CTRL_TPC_RPT_IND = 0xF11A, /* _H2ASN_MsgChoice WPHY_TOOL_PwrCtrlTpcRptInd  */

    ID_WPHY_TOOL_TAS_MNTN_TX_STATEI_IND         = 0xF11B, /* _H2ASN_MsgChoice WPHY_TOOL_TasMntnTxStateiInd  */
    ID_WPHY_TOOL_TAS_MNTN_TX_STATEII_SUB_RX_IND = 0xF11C, /* _H2ASN_MsgChoice WPHY_TOOL_TasMntnTxStateiiRxSubInd  */
    /* _H2ASN_MsgChoice WPHY_TOOL_TAS_MNTN_TX_STATEII_T1_SUB_IND_STRU */
    ID_WPHY_TOOL_TAS_MNTN_TX_STATEII_SUB_T1_IND = 0xF11D,
    /* _H2ASN_MsgChoice WPHY_TOOL_TAS_MNTN_TX_STATEII_T2_SUB_IND_STRU */
    ID_WPHY_TOOL_TAS_MNTN_TX_STATEII_SUB_T2_IND = 0xF11E,
    ID_WPHY_TOOL_TAS_MNTN_TX_SIGNAL_IND = 0xF11F,         /* _H2ASN_MsgChoice WPHY_TOOL_TasMntnTxSignalInd  */

    ID_WPHY_TOOL_SOFT_DEM_SYNC_INFO_RPT_IND = 0xF120, /* _H2ASN_MsgChoice WPHY_TOOL_SoftDemSyncInfoRpt  */
    ID_WPHY_TOOL_TIME_OUT_RPT_IND           = 0xF121, /* _H2ASN_MsgChoice WPHY_TOOL_TimeOutRptInd  */
    ID_WPHY_TOOL_WTC_MODE_RPT_IND           = 0xF122, /* _H2ASN_MsgChoice WPHY_TOOL_WtcModeRpt  */
    ID_WPHY_TOOL_CIQ_CELL_INFO_RPT_IND      = 0xF123, /* _H2ASN_MsgChoice WPHY_TOOL_CiqCellMetricRptInfo  */
    ID_WPHY_TOOL_CIQ_RL_STATUS_INFO_RPT_IND = 0xF124, /* _H2ASN_MsgChoice WPHY_TOOL_CiqRlsMetricRptInfo  */
    ID_WPHY_TOOL_DCH_AFC_RESET_RPT_IND      = 0xF125, /* _H2ASN_MsgChoice WPHY_TOOL_DchAfcResetRpt  */

    ID_WPHY_TOOL_AEASYRF_CTRL_INFO_IND          = 0xF130, /* _H2ASN_MsgChoice WPHY_TOOL_AeasyrfCtrlInfo  */
    ID_WPHY_TOOL_AEASYRF_CFG_TASK_LIST_INFO_IND = 0xF131, /* _H2ASN_MsgChoice WPHY_TOOL_AeasyrfCfgTaskListInfo  */

    ID_WPHY_TOOL_SOCP_MNTN_INFO_IND = 0xF132, /* _H2ASN_MsgChoice UPHY_SOCP_MNTN_INFO_STRU */
    ID_WPHY_TOOL_SW_SAR_MNTN_INFO_IND = 0xF133, /* _H2ASN_MsgChoice WPHY_TOOL_SwSarCtrlMntnInfo */
    ID_WPHY_TOOL_FS2_MAS_MNTN_STATEI_IND    = 0xF134, /* _H2ASN_MsgChoice WPHY_TOOL_Fs2MasMntnStateiInd   */
    ID_WPHY_TOOL_FS2_MAS_MNTN_STATEII_IND   = 0xF135, /* _H2ASN_MsgChoice WPHY_TOOL_Fs2MasMntnStateiiInd  */
    ID_WPHY_TOOL_FS2_MAS_MNTN_STATEIII_IND  = 0xF136, /* _H2ASN_MsgChoice WPHY_TOOL_Fs2MasMntnStateiiiInd */
    ID_WPHY_TOOL_FS2_TAS_MNTN_TX_SIGNAL_IND = 0xF137, /* _H2ASN_MsgChoice WPHY_TOOL_Fs2TasMntnTxSignalInd  */

    ID_WPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMsgIdUint16;

/*
 * ö��˵��: ���ֻظ���Ϣ�Ľ��ֵ:�ɹ�����ʧ�ܵ�
 */
enum WPHY_TOOL_MntnRslt {
    WPHY_MNTN_RSLT_SUCC               = 0, /* �ɹ� */
    WPHY_MNTN_RSLT_FAIL               = 1, /* ʧ�� */
    WPHY_MNTN_RSLT_PERIOD_NOT_SUPPORT = 2, /* �������ڲ�֧�� */
    WPHY_MNTN_RSLT_PARA_INVALID       = 3, /* ���ò�����Ч */
    WPHY_MNTN_RSLT_PARA_TYPE_OVERFLOW = 4, /* ���ò������Ͳ���ȷ */

    WPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRsltUint16;

/*
 * ö��˵��: ���ֲ��������ϱ�����
 */
enum WPHY_TOOL_MntnReportType {
    WPHY_MNTN_REPORT_TYPE_NO_RPT          = 0, /* ���ϱ�: */
    WPHY_MNTN_REPORT_TYPE_ONE_TIME        = 1, /* �ϱ�1��: */
    WPHY_MNTN_REPORT_TYPE_PER_FRAME       = 2, /* ÿ֡�ϱ�: */
    WPHY_MNTN_REPORT_TYPE_ASSIGNED_PERIOD = 3, /* ָ������:��֡Ϊ��λ�����ѡ�����������д��һ��������ָ�����ڡ� */
    WPHY_MNTN_REPORT_TYPE_EVENT_TRIGER    = 4, /* �¼�����: */

    WPHY_MNTN_REPORT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnReportTypeUint16;

/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum WPHY_TOOL_MntnPwrSwitchType {
    WPHY_MNTN_PWR_SWITCH_TYPE_UL_PC      = 0, /* ���й��ʿ���:�̶����з��䶫�� */
    WPHY_MNTN_PWR_SWITCH_TYPE_OLPC       = 1, /* �⻷���� */
    WPHY_MNTN_PWR_SWITCH_TYPE_WINUP      = 2, /* winup���� */
    WPHY_MNTN_PWR_SWITCH_TYPE_WINDOWN    = 3, /* windown���� */
    WPHY_MNTN_PWR_SWITCH_TYPE_FIX_DL_PWR = 4, /* �̶����з��书�� */

    WPHY_MNTN_PWR_SWITCH_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnPwrSwitchTypeUint16;

/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum WPHY_TOOL_MntnSwitchCtrl {
    WPHY_MNTN_SWITCH_CTRL_CLOSE = 0, /* �ر� */
    WPHY_MNTN_SWITCH_CTRL_OPEN  = 1, /* �� */

    WPHY_MNTN_SWITCH_CTRL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSwitchCtrlUint16;

/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum WPHY_TOOL_SyncStatus {
    WPHY_MNTN_SYNC_STATUS_FIX  = 0, /* ���� */
    WPHY_MNTN_SYNC_STATUS_LOST = 1, /* ʧ�� */

    WPHY_MNTN_SYNC_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_SyncStatusUint16;

/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum UPHY_TOOL_XoStatus {
    UPHY_MNTN_XO_STATUS_UNLOCKED = 0, /* ʧ�� */
    UPHY_MNTN_XO_STATUS_LOCKED   = 1, /* ���� */

    UPHY_MNTN_XO_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_XoStatusUint16;

/*
 * ö��˵��: ���ʲ������ͣ���ѯ�����ù���
 */
enum WPHY_TOOL_MntnPwrPara {
    WPHY_MNTN_PWR_PARA_INIT_VALUE            = 0x00000000, /* Init_Value */
    WPHY_MNTN_PWR_PARA_SIR_TARGET            = 0x00000001, /* SirTarget */
    WPHY_MNTN_PWR_PARA_CURR_TRCH_ID_SELECTED = 0x00000002, /* ��ǰѡ���TrchId */
    WPHY_MNTN_PWR_PARA_DL_TPC                = 0x00000004, /* DL TPC */
    WPHY_MNTN_PWR_PARA_CURR_SIR              = 0x00000008, /* ��ǰ��SIR */
    WPHY_MNTN_PWR_PARA_CURR_BLER             = 0x00000010, /* ��ǰ��BLER */
    WPHY_MNTN_PWR_PARA_UL_TPC                = 0x00000020, /* UL TPC */
    WPHY_MNTN_PWR_PARA_BD                    = 0x00000040, /* Bd */
    WPHY_MNTN_PWR_PARA_BEC                   = 0x00000080, /* Bec */
    WPHY_MNTN_PWR_PARA_BED                   = 0x00000100, /* Bed */
    WPHY_MNTN_PWR_PARA_BHS_NACK              = 0x00000200, /* BhsNack */
    WPHY_MNTN_PWR_PARA_BHS_ACK               = 0x00000400, /* BhsAck */
    WPHY_MNTN_PWR_PARA_BHS_CQI               = 0x00000800, /* BhsCQI */
    WPHY_MNTN_PWR_PARA_BHS_TFCI              = 0x00001000, /* TFCI */
    WPHY_MNTN_PWR_PARA_SLOT_FORMAT           = 0x00002000, /* SlotFormat */
    WPHY_MNTN_PWR_PARA_MAX_PWR               = 0x00004000, /* ����书�� */
    WPHY_MNTN_PWR_PARA_MIN_PWR               = 0x00008000, /* ��С���书�� */
    WPHY_MNTN_PWR_PARA_INIT_PWR              = 0x00010000, /* lint !e575 ��ʼ���书�� */
    WPHY_MNTN_PWR_PARA_LIST_MODE_SET_PWR     = 0x00020000, /* lint !e575 LIST MODE�¹ر��ڻ�����ֱ�����õķ��书�� */
    WPHY_MNTN_PWR_PARA_LIST_MODE_OPEN_ILPC   = 0x00040000, /* lint !e575 LIST MODE�����´��ڻ����� */
    WPHY_MNTN_PWR_PARA_DPCCH_MIN_PWR         = 0x00080000, /* DPCCH ��С���书�� */
    WPHY_MNTN_PWR_PARA_DPCCH_MAX_PWR         = 0x00100000, /* DPCCH ����书�� */
    WPHY_MNTN_PWR_PARA_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MntnPwrParaUint32;

/*
 * ö��˵��: �ϱ��¼�����
 */
enum WPHY_TOOL_MntnEventType {
    WPHY_MNTN_EVENT_TYPE_AFC_STATUS  = 0x00000001, /* 6ms ����ʱ */
    WPHY_MNTN_EVENT_TYPE_SYNC_STATUS = 0x00000002, /* 9ms ����ʱ */
    WPHY_MNTN_EVENT_TYPE_L1_STATUS   = 0x00000004, /* L1״̬ */
    WPHY_MNTN_EVENT_TYPE_INT_EXCEPT  = 0x00000008, /* �ж��쳣 */

    WPHY_MNTN_EVENT_TYPE_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MntnEventTypeUint32;

/*
 * ö��˵��: �ж��쳣����
 */
enum WPHY_TOOL_MntnIntExcept {
    WPHY_MNTN_INT_EXCEPT_TYPE_6MS_TASK_EXPIRED = 0, /* 6ms ����ʱ */
    WPHY_MNTN_INT_EXCEPT_TYPE_9MS_TASK_EXPIRED = 1, /* 9ms ����ʱ */

    WPHY_MNTN_INT_EXCEPT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnIntExceptUint16;

/*
 * ö��˵��: ϵͳ״̬
 */
enum WPHY_TOOL_MntnSysStatus {
    WPHY_MNTN_SYS_STATUS_FREE  = 0,
    WPHY_MNTN_SYS_STATUS_CPICH = 1,
    WPHY_MNTN_SYS_STATUS_FACH  = 2,
    WPHY_MNTN_SYS_STATUS_DPCH  = 3,

    WPHY_MNTN_SYS_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSysStatusUint16;

/*
 * ö��˵��: ��������������
 */
enum WPHY_TOOL_MntnThresholdType {
    /* С���������� */
    WPHY_MNTN_THRESHOLD_TYPE_STEP1_NOISE = 0,
    WPHY_MNTN_THRESHOLD_TYPE_STEP2_NOISE,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NOISE_AFC_UNLOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NOISE_AFC_LOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEPB_NOISE_AFC_UNLOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEPB_NOISE_AFC_LOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEPEB_NOISE_AFC_UNLOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEPEB_NOISE_AFC_LOCK,

    WPHY_MNTN_THRESHOLD_TYPE_STEP1_FAIL_FOR_SPEC_FREQ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP1_FAIL_FOR_BAND,

    WPHY_MNTN_THRESHOLD_TYPE_STEP1_CORRELEN_FOR_INIT_CS,

    WPHY_MNTN_THRESHOLD_TYPE_STEP2_RSDEC_LEN,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_CORRELEN_AFC_UNLOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_CORRELEN_AFC_LOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NCORRELEN_AFC_UNLOCK,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NCORRELEN_AFC_LOCK,

    WPHY_MNTN_THRESHOLD_TYPE_STEPB_CORRELEN,

    /* �ྶ�������� */
    WPHY_MNTN_THRESHOLD_TYPE_CORRE_LENTH_LOCK,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_LENTH_LOCK,
    WPHY_MNTN_THRESHOLD_TYPE_CORRE_LENTH_UNLOCK,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_LENTH_UNLOCK,

    /* �������� */
    WPHY_MNTN_THRESHOLD_TYPE_MEAS_AVERAGE_NOISE,
    WPHY_MNTN_THRESHOLD_TYPE_MP_RELATIVE_FACTOR,

    WPHY_MNTN_THRESHOLD_TYPE_MEAS_TIMING_ECNO_THRESHOLD,
    WPHY_MNTN_THRESHOLD_TYPE_MEAS_TIMING_RSCP_THRESHOLD,

    WPHY_MNTN_THRESHOLD_TYPE_MAX_TIMING_MEAS_FRAME_NUMBER,

    /* �������� */
    WPHY_MNTN_THRESHOLD_TYPE_SIR_THRESHOLD_PILOT1,
    WPHY_MNTN_THRESHOLD_TYPE_SIR_THRESHOLD_PILOT2,

    WPHY_MNTN_THRESHOLD_TYPE_TPC_THRESHOLD_TTI1OR2,
    WPHY_MNTN_THRESHOLD_TYPE_TPC_THRESHOLD_TTI4,
    WPHY_MNTN_THRESHOLD_TYPE_TPC_THRESHOLD_TTI8,

    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_CPU_QIN,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_CPU_QOUT,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_SS_THRESHOLD,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_MRC_THRESHOLD,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_INT_THRESHOLD,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_ML_THRESHOLD,

    /* DRX�������� */
    WPHY_MNTN_THRESHOLD_TYPE_32K_CLOCK_MSR_WAIT_SFN_NUM,
    WPHY_MNTN_THRESHOLD_TYPE_SLEEP_LENTH,
    WPHY_MNTN_THRESHOLD_TYPE_TIME_ADJUST,

    WPHY_MNTN_THRESHOLD_TYPE_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MntnThresholdTypeUint32;

/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum WPHY_TOOL_MntnPaStatus {
    WPHY_MNTN_PA_STATUS_CLOSE = 0, /* �ر� */
    WPHY_MNTN_PA_STATUS_OPEN  = 1, /* �� */

    WPHY_MNTN_PA_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnPaStatusUint16;

/*
 * ö��˵��: WPHY��ά�ɲ�̶�ץȡ�ļ�������ö��
 */
enum WPHY_TOOL_MntnRamRpt {
    WPHY_MNTN_RPT_HSUPA_SHARE_CTRL = 0, /* UPA���� */
    WPHY_MNTN_RPT_DSP_MODE         = 1, /* DSPģʽ */
    WPHY_MNTN_RPT_HSUPA_RPT        = 2,
    WPHY_MNTN_RPT_HSUPA_TTI        = 3,
    WPHY_MNTN_RPT_CPC_DRX_RPT      = 4,
    WPHY_MNTN_RPT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRamRptUint16;

/*
 * ö��˵��: WPHY�����ϱ�����
 */
enum WPHY_TOOL_MntnErrorRptLevel {
    WPHY_MNTN_ERROR_LEVEL_INFO    = 1,
    WPHY_MNTN_ERROR_LEVEL_WARNING = 2,
    WPHY_MNTN_ERROR_LEVEL_ERROR   = 3,

    WPHY_MNTN_ERROR_LEVEL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErrorRptLevelUint16;

/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum UPHY_TOOL_MntnRptStatus {
    UPHY_MNTN_RPT_STATUS_CLOSE = 0, /* �ر� */
    UPHY_MNTN_RPT_STATUS_OPEN  = 1, /* �� */
    UPHY_MNTN_RPT_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnRptStatusUint16;

#if (FEATURE_GSM_WHOLE_SDR == FEATURE_ON)
/*
 * ö��˵��: HIDSǿ����������ؿ��Ƶ�ö��ֵ
 */
enum UPHY_TOOL_MntnCtrlStatus {
    UPHY_MNTN_CTRL_STATUS_NORMAL = 0, /* Ĭ�ϲ�ǿ�ƿ��� */
    UPHY_MNTN_CTRL_STATUS_ISAIC  = 1, /* ǿ�ƴ�ISAIC */
    UPHY_MNTN_CTRL_STATUS_DAIC   = 2, /* ǿ�ƴ�DAIC */
    UPHY_MNTN_CTRL_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnCtrlStatusUint16;

#else
/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum UPHY_TOOL_MntnCtrlStatus {
    UPHY_MNTN_CTRL_STATUS_NORMAL = 0, /* Ĭ�Ϲ��ܲ��� */
    UPHY_MNTN_CTRL_STATUS_START  = 1, /* ǿ�ƴ�ISAIC */
    UPHY_MNTN_CTRL_STATUS_STOP   = 2, /* ǿ�ƹر�SDR */
    UPHY_MNTN_CTRL_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnCtrlStatusUint16;

#endif

/*
 * ö��˵��: ���ؿ��Ƶ�ö��ֵ
 */
enum WPHY_TOOL_MntnRptStatus {
    WPHY_MNTN_RPT_STATUS_CLOSE = 0, /* �ر� */
    WPHY_MNTN_RPT_STATUS_OPEN  = 1, /* �� */

    WPHY_MNTN_RPT_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRptStatusUint16;

/*
 * ö��˵��: T313 Status
 * ö��˵��: T313��ʱ�� ״̬
 */
enum WPHY_TOOL_MntnT313Status {
    WPHY_MNTN_T313_START   = 0,
    WPHY_MNTN_T313_STOP    = 1,
    WPHY_MNTN_T313_TIMEOUT = 2,

    WPHY_MNTN_T313_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT313StatusUint16;

/*
 * ö��˵��: T312 Status
 * ö��˵��: T312��ʱ�� ״̬
 */
enum WPHY_TOOL_MntnT312Status {
    WPHY_MNTN_T312_START   = 0,
    WPHY_MNTN_T312_STOP    = 1,
    WPHY_MNTN_T312_TIMEOUT = 2,

    WPHY_MNTN_T312_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT312StatusUint16;

/*
 * ö��˵��: ǿͣLTEԭ��
 */
enum WPHY_TOOL_MntnMasterStopSlaveReason {
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_BG           = 0,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_MEAS_TIMEOUT = 1,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_BG_TIMEOUT   = 2,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_HIGH_TASK    = 3,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_MEAS         = 4,
    WPHY_TOOL_MNTN_MASTER_STOP_GSM_REASION_BG           = 5,
    WPHY_TOOL_MNTN_MASTER_STOP_GSM_REASION_MEAS_TIMEOUT = 6,
    WPHY_TOOL_MNTN_MASTER_STOP_GSM_REASION_BG_TIMEOUT   = 7,
    WPHY_TOOL_MNTN_MASTER_STOP_GSM_REASION_HIGH_TASK    = 8,
    WPHY_TOOL_MNTN_MASTER_STOP_GSM_REASION_MEAS         = 9,

    WPHY_TOOL_MNTN_MASTER_STOP_SLAVE_REASION_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasterStopSlaveReasonUint16;

/*
 * ö��˵��: ��ģ״̬
 */
enum WPHY_TOOL_MntnSlaveCurrentStatus {
    WPHY_MNTN_SLAVE_PRE         = 0,
    WPHY_MNTN_SLAVE_ENTERGAP    = 1,
    WPHY_MNTN_SLAVE_EXITRGAP    = 2,
    WPHY_MNTN_SLAVE_FREQCHANGE  = 3,
    WPHY_MNTN_SLAVE_MEAS_FINISH = 4,
    WPHY_MNTN_SLAVE_CS_FINISH   = 5,
    WPHY_MNTN_SLAVE_BG          = 6,
    WPHY_MNTN_SLAVE_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSlaveCurrentStatusUint16;

/*
 * ö��˵��: ��ǰ���⹤��״̬
 * ö��˵��: ��ģ״̬
 */
enum WPHY_TOOL_MntnCpcDrxEquaStatus {
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_ALWAYS_OPEN = 0, /* ���ⳤ��״̬ */
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_DRX_OPEN,        /* �����״̬ */
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_DRX_CLOSE,       /* ����ر�״̬ */
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcDrxEquaStatusUint16;

/*
 * ö��˵��: �����ö������
 */
enum WPHY_TOOL_MntnBool {
    WPHY_MNTN_BOOL_FALSE = 0,
    WPHY_MNTN_BOOL_TRUE  = 1,
    WPHY_MNTN_BOOL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnBoolUint16;

/*
 * ö��˵��: �����ö������
 */
enum UPHY_TOOL_MntnBool {
    UPHY_MNTN_BOOL_FALSE = 0,
    UPHY_MNTN_BOOL_TRUE  = 1,
    UPHY_MNTN_BOOL_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnBoolUint16;

/*
 * ö��˵��: CQI ���ȼ����Ͷ���
 */
enum WPHY_TOOL_MntnCpcDrxCqiPri {
    WPHY_MNTN_CPC_DRX_CQI_PRIORITY_0 = 0, /* CQI���ȼ�Ϊ0 */
    WPHY_MNTN_CPC_DRX_CQI_PRIORITY_1,     /* CQI���ȼ�Ϊ1 */
    WPHY_MNTN_CPC_DRX_CQI_PRIORITY_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcDrxCqiPriUint16;

/*
 * ö��˵��: �ۺ������ز���״̬��ĳ���cycle״̬����󼯺�
 */
enum WPHY_TOOL_MntnCpcUldpcchCycleState {
    WPHY_MNTN_CPC_ULDPCCH_CYCLE1 = 0,
    WPHY_MNTN_CPC_ULDPCCH_CYCLE2,
    WPHY_MNTN_CPC_ULDPCCH_CYCLE1AND2,
    WPHY_MNTN_CPC_ULDPCCH_CYCLE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcUldpcchCycleStateUint16;

/*
 * ö��˵��:CPC DRX
 */
enum WPHY_TOOL_MntnCpcDrxRfStatus {
    WPHY_MNTN_CPC_DRX_RF_STATUS_ALWAYS_OPEN = 0, /* RFһֱ�� */
    WPHY_MNTN_CPC_DRX_RF_STATUS_DRX_OPEN,        /* RF����DRX�Ĵ�״̬ */
    WPHY_MNTN_CPC_DRX_RF_STATUS_DRX_CLOSE,       /* RF����DRX�Ĺر�״̬ */
    WPHY_MNTN_CPC_DRX_RF_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcDrxRfStatusUint16;

/*
 * ö��˵��: MAS״̬ö��
 */
enum WPHY_TOOL_MntnMasState {
    WPHY_MNTN_MAS_STATEI      = 0,
    WPHY_MNTN_MAS_STATEIAIB   = 1,
    WPHY_MNTN_MAS_STATEII     = 2,
    WPHY_MNTN_MAS_STATEIII    = 3,
    WPHY_MNTN_MAS_TX_STATE_I  = 4,
    WPHY_MNTN_MAS_TX_STATE_II = 5,
    WPHY_MNTN_MAS_STATE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasStateUint16;

/*
 * ö��˵��: MAS ������ģʽö��
 */
enum WPHY_TOOL_MntnCtrlMode {
    WPHY_MNTN_CTRL_MODE_ACTIVE  = 0,
    WPHY_MNTN_CTRL_MODE_PASSIVE = 1,
    WPHY_MNTN_CTRL_MODE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCtrlModeUint16;

/*
 * ö��˵��: ����TAS״̬ö��
 */
enum WPHY_TOOL_MntnMasAnttasstatus {
    WPHY_MNTN_MAS_TASDPDT0 = 0, /* TASֱ̬ͨ */
    WPHY_MNTN_MAS_TASDPDT1 = 1, /* TAS����̬ */
    WPHY_MNTN_MAS_TAS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasAnttasstatusUint16;

/*
 * ö��˵��: ����MAS״̬ö��
 */
enum WPHY_TOOL_MntnMasAntmasstatus {
    WPHY_MNTN_MAS_MASDPDT0 = 0, /* MASĬ��̬:�������ұ� */
    WPHY_MNTN_MAS_MASDPDT1 = 1, /* MAS��Ĭ��̬:��������� */
    WPHY_MNTN_MAS_MAS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasAntmasstatusUint16;

/*
 * ö��˵��: MAS ANTENNAö��
 */
enum WPHY_TOOL_MntnMasAntenna {
    WPHY_MNTN_MAS_ANTENNA_SOURCE = 0,
    WPHY_MNTN_MAS_ANTENNA_DEST   = 1,
    WPHY_MNTN_MAS_ANTENNA_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasAntennaUint16;

/*
 * ö��˵��: ��λ�������ö��
 */
enum WPHY_TOOL_MntnTxLevelAdjust {
    WPHY_MNTN_TX_LEVEL_ADJUST_UP           = 0, /* ���� */
    WPHY_MNTN_TX_LEVEL_ADJUST_HOLD         = 1, /* ���� */
    WPHY_MNTN_TX_LEVEL_ADJUST_DOWN         = 2, /* ���� */
    WPHY_MNTN_TX_LEVEL_ADJUST_EXIT_SAR_TAS = 3, /* �˳�SAR TAS */
    WPHY_MNTN_TX_LEVEL_ADJUST_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxLevelAdjustUint16;

/*
 * ö��˵��: TX_STATEII��λ
 */
enum WPHY_TOOL_MntnTxStateiiLevelCfg {
    WPHY_MNTN_TX_STATEII_LEVEL_0 = 0, /* TX_STATEII��λ0 */
    WPHY_MNTN_TX_STATEII_LEVEL_1 = 1, /* TX_STATEII��λ1 */
    WPHY_MNTN_TX_STATEII_LEVEL_2 = 2, /* TX_STATEII��λ2 */
    WPHY_MNTN_TX_STATEII_LEVEL_3 = 3, /* TX_STATEII��λ3 */
    WPHY_MNTN_TX_STATEII_LEVEL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxStateiiLevelUint16;

/*
 * ö��˵��: TX POWER�������ö��
 */
enum WPHY_TOOL_MntnTxPower {
    WPHY_MNTN_TX_POWER_CURRENT_LIMITED   = 0, /* ��ǰ��λ���� */
    WPHY_MNTN_TX_POWER_BACKOFF_LIMITED   = 1, /* ����һ����λ���� */
    WPHY_MNTN_TX_POWER_MONITOR_LIMITED   = 2, /* MONITOR�������� */
    WPHY_MNTN_TX_POWER_MONITOR_UNLIMITED = 3, /* MONITOR���ʲ����� */
    WPHY_MNTN_TX_POWER_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxPowerUint16;

/*
 * ö��˵��: MAS_TX_STATEII_SUB_STATE
 */
enum WPHY_TOOL_MntnTxStateiiSubStateCfg {
    WPHY_MNTN_TX_STATEII_SUB_STATE_BETTER_RX = 0, /* RX�������ж��ӽ׶� */
    WPHY_MNTN_TX_STATEII_SUB_STATE_T1        = 1, /* ������TX���ʲɼ��ӽ׶� */
    WPHY_MNTN_TX_STATEII_SUB_STATE_T2        = 2, /* ������TX���ʲɼ��ӽ׶� */
    WPHY_MNTN_TX_STATEII_SUB_STATE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxStateiiSubStateUint16;

/*
 * ö��˵��: TX_SIGNAL_STATE
 */
enum WPHY_TOOL_MntnTxSignalState {
    WPHY_MNTN_TX_SIGNAL_STATE_GOOD = 0, /* TX_STATE GOOD */
    WPHY_MNTN_TX_SIGNAL_STATE_POOR = 1, /* TX_STATE POOR */
    WPHY_MNTN_TX_SIGNAL_STATE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxSignalStateUint16;

/*
 * ö��˵��: ��ǰrake�ྶ���е�״̬
 */
enum WPHY_TOOL_RakeMpStatus {
    WPHY_TOOL_RAKE_MP_STATUS_NONE = 0,   /* �ྶ����û������ */
    WPHY_TOOL_RAKE_MP_STATUS_RAKE,       /* ���е���RAKE�ྶ */
    WPHY_TOOL_RAKE_MP_STATUS_RAKE_TYPE1, /* ���е���RAKE TYPE1�ྶ */

    WPHY_TOOL_RAKE_MP_STATUS_RAKE_BUTT,
};
typedef VOS_UINT16 WPHY_TOOL_RakeMpStatusUint16;

/*
 * ö��˵��: ����TPC������
 */
enum WPHY_TOOL_MntnDlTpcCmd {
    WPHY_MNTN_DL_TPC_CMD_HOLD = 0, /* ���ֹ��� */
    WPHY_MNTN_DL_TPC_CMD_UP   = 1, /* ������ */
    WPHY_MNTN_DL_TPC_CMD_DOWN = 3  /* ������ */
};
typedef VOS_UINT16 WPHY_TOOL_MntnDlTpcCmdUint16;

/*
 * ö��˵��: ����TPC������
 */
enum WPHY_TOOL_MntnUlTpcCmd {
    WPHY_MNTN_UL_TPC_CMD_DOWN = 0, /* ������ */
    WPHY_MNTN_UL_TPC_CMD_UP   = 1  /* ������ */
};
typedef VOS_UINT16 WPHY_TOOL_MntnUlTpcCmdUint16;

/*
 * ö��˵��: ʱ϶��������
 */
enum GPHY_TOOL_TimeslotMeasType {
    GPHY_TOOL_TS_NO_MEAS = 0,                /* 0 ��ǰʱ϶�޲������� */
    GPHY_TOOL_TS_BA_MEAS,                    /* 1 ��ǰʱ϶ΪBA���� */
    GPHY_TOOL_TS_CELLSELECT_MEAS,            /* 2 ��ǰʱ϶ΪС��ѡ����� */
    GPHY_TOOL_TS_FULLLIST_MEAS,              /* 3 ��ǰʱ϶ΪȫƵ����� */
    GPHY_TOOL_TS_INTERFERE_MEAS,             /* 4 ��ǰʱ϶Ϊ���Ų��� */
    GPHY_TOOL_TS_SCELL_BCCH_SLOW_GAIN_MEAS,  /* 5 ������BCCH����������� */
    GPHY_TOOL_TS_SCELL_CCCH_SLOW_GAIN_MEAS,  /* 6 ������CCCH����������� */
    GPHY_TOOL_TS_SCELL_PBCCH_SLOW_GAIN_MEAS, /* 7 ������PBCCH����������� */
    GPHY_TOOL_TS_SCELL_PCCCH_SLOW_GAIN_MEAS, /* 8 ������PCCCH����������� */
    GPHY_TOOL_TS_SCELL_CBCH_SLOW_GAIN_MEAS,  /* 9 ������CBCH����������� */
    GPHY_TOOL_TS_SCELL_TCH_GAIN_MEAS,        /* 10 ������CSר��̬����������� */
    GPHY_TOOL_TS_SCELL_PDTCH_GAIN_MEAS,      /* 11 ������PSר��̬pdtch����������� */
    GPHY_TOOL_TS_SCELL_PTCCH_GAIN_MEAS,      /* 12 ������PSר��̬ptcch����������� */
    GPHY_TOOL_TS_SCELL_SYNC_SLOW_GAIN_MEAS,  /* 13 ������ͬ������������� */
    GPHY_TOOL_TS_SCELL_BCCH_FAST_GAIN_MEAS,  /* 14 ������BCCH����ǰ���������ò��� */
    GPHY_TOOL_TS_SCELL_CCCH_FAST_GAIN_MEAS,  /* 15 ������CCCH����ǰ���������ò��� */
    GPHY_TOOL_TS_SCELL_PBCCH_FAST_GAIN_MEAS, /* 16 ������PBCCH����ǰ���������ò��� */
    GPHY_TOOL_TS_SCELL_PCCCH_FAST_GAIN_MEAS, /* 17 ������BCCH����ǰ���������ò��� */
    GPHY_TOOL_TS_SCELL_CBCH_FAST_GAIN_MEAS,  /* 18 ������CBCH����ǰ���������ò��� */
    GPHY_TOOL_TS_SCELL_SYNC_FAST_GAIN_MEAS,  /* 19 ������ͬ����������� */
    GPHY_TOOL_TS_NCELL_BCCH_FAST_GAIN_MEAS,  /* 20 ��������ǰ���������ò��� */
    GPHY_TOOL_TS_NCELL_BCCH_SLOW_GAIN_MEAS,  /* 21 �����������ٲ��� */
    GPHY_TOOL_TS_NCELL_SYNC_FAST_GAIN_MEAS,  /* 22 ����ͬ��ǰ���������ò��� */
    GPHY_TOOL_TS_NCELL_SYNC_SLOW_GAIN_MEAS,  /* 23 ����ͬ�����ٲ��� */
    GPHY_TOOL_TS_BA_MEAS_SBCCH_MEAS,         /* 24 ����PBʱBA����ģ��ķ�����BCCH���� */
    GPHY_TOOL_TS_CAL_MEAS,                   /* 25 У׼���� */
    GPHY_TOOL_TS_WCDMA_MEAS,                 /* 26 ��ǰʱ϶Ϊ��ϵͳ���� */
    GPHY_TOOL_TS_BG_TASK,                    /* 27 G������������ */
    GPHY_TOOL_TS_DCXO_TASK,                  /* 28 DCXO RA delay�ڼ�Ĳ������� */
    GPHY_TOOL_TS_DUAL_ANT_PRI_ANT_MEAS,      /* 29 ˫�����л������߲��� */
    GPHY_TOOL_TS_DUAL_ANT_SEC_ANT_MEAS,      /* 30 ˫�����л������߲��� */
    GPHY_TOOL_TS_NOISE_TOOL_MEAS,            /* 31 �������߲��� */
    GPHY_TOOL_TS_PSRCH_PCH_FAST_GAIN_MEAS,   /* 32 ������Ѱ������ǰ���������ò��� */
    GPHY_TOOL_TS_PSRCH_PCH_SLOW_GAIN_MEAS,   /* 33 ������Ѱ���������ٲ��� */

    GPHY_TOOL_TIMESLOT_MEAS_TYPE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_TimeslotMeasTypeUint16;

/*
 * DCά������ö��
 */
enum UPHY_TOOL_Dcoffset {
    UPHY_TOOL_DCOFFSET_UPDATE_INIT   = 0,
    UPHY_TOOL_DCOFFSET_UPDATE_REMAIN = 1,

    UPHY_TOOL_DCOFFSET_UPDATE_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_DcoffsetUint16;

/*
 * ö��˵��: ��ģ������С������״̬
 */
enum WPHY_TOOL_SlaveCsStatus {
    WPHY_TOOL_SLAVE_CS_STATUS_NONE = 0, /* ���񲻴��� */
    WPHY_TOOL_SLAVE_CS_STATUS_READY,    /* ������� */
    WPHY_TOOL_SLAVE_CS_STATUS_RUNNING,  /* ������������ */
    WPHY_TOOL_SLAVE_CS_STATUS_FINISH
};
typedef VOS_UINT16 WPHY_TOOL_SlaveCsStatusUint16;

/*
 * ö��˵��: ��ģ�����Ĳ�������״̬
 */
enum WPHY_TOOL_SlaveMeasStatus {
    WPHY_TOOL_SLAVE_MEAS_STATUS_NONE = 0, /* �����ʼ״̬ */
    WPHY_TOOL_SLAVE_MEAS_STATUS_READY,    /* ������� */
    WPHY_TOOL_SLAVE_MEAS_STATUS_RUNNING,  /* ������������ */
    WPHY_TOOL_SLAVE_MEAS_STATUS_FINISH
};
typedef VOS_UINT16 WPHY_TOOL_SlaveMeasStatusUint16;

/*
 * ö��˵��: RF��ǰ�˹�������ö��
 */
enum WPHY_TOOL_MntnRfOperScene {
    WPHY_TOOL_DRV_CS_AND_FM               = 0,  /* С��������Ƶ�ʲ��� */
    WPHY_TOOL_DRV_IDLEPCH_MEAS            = 1,  /* ����ģʽ��PCH״̬�µ���Ƶ���� */
    WPHY_TOOL_DRV_DCH_MEAS                = 2,  /* DCH״̬�µ���Ƶ���� */
    WPHY_TOOL_DRV_FACH_MEAS               = 3,  /* FACH״̬�µ���Ƶ���� */
    WPHY_TOOL_DRV_INTER_FREQ_PCCPCH       = 4,  /* ��Ƶ�㲥��� */
    WPHY_TOOL_DRV_CAMP_ON                 = 5,  /* פ�� */
    WPHY_TOOL_DRV_HARD_HAND_OVER          = 6,  /* ��ƵӲ�л� */
    WPHY_TOOL_DRV_RETURN_TO_INTRAFREQ     = 7,  /* ���ر�Ƶ */
    WPHY_TOOL_DRV_DRX_SLEEP_TO_AWAKE      = 8,  /* ���뻽��״̬ */
    WPHY_TOOL_DRV_DRX_AWAKE_TO_SLEEP      = 9,  /* ����˯��״̬ */
    WPHY_TOOL_DRV_W_BG_INTERFREQ_RES      = 10, /* BG��Ƶ���л� */
    WPHY_TOOL_DRV_W_RX_DC_MODE_CHANGE     = 11, /* RX SC/DC״̬�л���־ */
    WPHY_TOOL_DRV_W_TX_DC_MODE_CHANGE     = 12, /* TX SC/DC״̬�л���־ */
    WPHY_TOOL_DSDS_CPICH_RESUME_RF        = 13, /* WDSDS CPICH̬RF��Դ�ָ� */
    WPHY_TOOL_DSDS_CONNECT_RESUME_RF      = 14, /* WDSDS FACH��DCH̬RF��Դ�ָ� */
    WPHY_TOOL_DSDS_W_RX_DC_MODE_RESUME_RF = 15, /* WDSDS DC RF��Դ�ָ� */
    WPHY_TOOL_DSDS_PREEMPED_NO_RF         = 16, /* DSDS��������Դ����ռ��û���κ���Դ��ʱ���ά�ɲ��� */
    WPHY_TOOL_DRV_RF_DRX_LTE_MEAS         = 17,
    WPHY_TOOL_SLAVE_RF_MEAS_W             = 18, /* ��ģ����������Ҫ��IDLE_END */
    WPHY_TOOL_SLAVE_RF_BG_SEARCH          = 19, /* �����Ѻͽ�㲥 */
    WPHY_TOOL_CPICH_INTRA                 = 20, /* CPICH̬ʱ϶�жϿ���Ƶ */

    WPHY_TOOL_DRV_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRfOperSceneUint16;

/*
 * ö��˵��: ͨ����
 */
enum WPHY_TOOL_MntnChannel {
    WPHY_MNTN_MASTER_CHANNEL = 0, /* ������ */
    WPHY_MNTN_SLAVE_CHANNEL  = 1, /* ������ */
    WPHY_MNTN_CHANNEL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnChannelUint16;

/*
 * ö��˵��: ͨ����
 */
enum GPHY_TOOL_MntnChannel {
    GPHY_MNTN_MASTER_CHANNEL = 0, /* ������ */
    GPHY_MNTN_SLAVE_CHANNEL  = 1, /* ������ */
    GPHY_MNTN_CHANNEL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnChannelUint16;

/*
 * ö��˵��: DCXO�¼���ӡ
 */
enum UPHY_TOOL_MntnDcxoEvent {
    UPHY_TOOL_MNTN_DCXO_1T_CELL_SEARCH = 0, /* W��һ������ */
    UPHY_TOOL_MNTN_DCXO_2T_CELL_SEARCH,     /* W�ڶ������� */
    UPHY_TOOL_MNTN_DCXO_3T_CELL_SEARCH,     /* W���������� */
    UPHY_TOOL_MNTN_DCXO_4T_CELL_SEARCH,     /* W���Ĵ����� */
    UPHY_TOOL_MNTN_DCXO_5T_CELL_SEARCH,     /* W��������� */

    UPHY_TOOL_MNTN_DCXO_RD_PPM_FROM_OTHER_MODE, /* ������ģ��ȡƵƫ */
    UPHY_TOOL_MNTN_DCXO_WR_PPM_TO_OTHER_MODE,   /* Ƶƫд������ģ */

    UPHY_TOOL_MNTN_DCXO_DSDS_RD_PPM_FROM_OTHER_MODE, /* ������ģ��ȡƵƫ */
    UPHY_TOOL_MNTN_DCXO_DSDS_WR_PPM_TO_OTHER_MODE,   /* Ƶƫд������ģ */

    UPHY_TOOL_MNTN_DCXO_RD_RCM_TEMP_COMP,           /* ��CTU��ȡRCMд���²�ֵ */
    UPHY_TOOL_MNTN_DCXO_GET_MULTI_CELL_SEARCH_TIME, /* ���㵱ǰ�������� */
    UPHY_TOOL_MNTN_DCXO_EVENT_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnDcxoEventUint16;

/*
 * ö��˵��: CģmodemIdö��
 */
enum CSDR_ModeId {
    CSDR_MODE_ID_0 = 0, /* ��0 */
    CSDR_MODE_ID_1 = 1, /* ��1 */
    CSDR_MODE_ID_2 = 2, /* ��2 */

    CSDR_MODE_ID_BUTT
};
typedef VOS_UINT16 CSDR_ModeIdUint16;

/*
 * ö��˵��: Cģ1X modemTypeö��
 */
enum CSDR_1X_ModeType {
    CSDR_1X_MODE_TYPE_SLAVE  = 0x0000,
    CSDR_1X_MODE_TYPE_MASTER = 0x0001,
    CSDR_1X_MODE_TYPE_CT     = 0x0002,
    CSDR_1X_MODE_TYPE_BT     = 0x0003,

    CSDR_1X_MODE_TYPE_BUTT
};
typedef VOS_UINT16 CSDR_1X_ModeTypeUint16;

/*
 * ö��˵��: CģHRPD modemTypeö��
 */
enum CSDR_HRPD_ModeType {
    CSDR_HRPD_MODE_TYPE_SLAVE              = 0x0000,
    CSDR_HRPD_MODE_TYPE_SLAVE_KEEP_TIMING  = 0x0001,
    CSDR_HRPD_MODE_TYPE_MASTER             = 0x0002,
    CSDR_HRPD_MODE_TYPE_MASTER_KEEP_TIMING = 0x0003,

    CSDR_HRPD_MODE_TYPE_BUTT
};
typedef VOS_UINT16 CSDR_HRPD_ModeTypeUint16;

/*
 * ö��˵��: ������ǿ����������״̬
 */
enum WPHY_TOOL_MntnErachCtrlState {
    WPHY_MNTN_ERACH_CTRL_STATE_INVALID = 0,   /* UL ERACHδ����״̬ */
    WPHY_MNTN_ERACH_CTRL_STATE_INIT,          /* UL ERACH��ʼ̬ */
    WPHY_MNTN_ERACH_CTRL_STATE_WAIT_AI,       /* UL ERACH�ȴ�AI�ж�̬ */
    WPHY_MNTN_ERACH_CTRL_STATE_FORBID_SEND,   /* UL ERACH������״̬ */
    WPHY_MNTN_ERACH_CTRL_STATE_ALLOW_SEND,    /* UL ERACH�ɷ���״̬ */
    WPHY_MNTN_ERACH_CTRL_STATE_COLLISION_SUC, /* UL ERACH��ͻ����ɹ���״̬ */
    WPHY_MNTN_ERACH_CTRL_STATE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErachCtrlStateUint16;

/*
 * ö��˵��: ������ǿ�������ԭ�����״̬
 */
enum WPHY_TOOL_MntnErachPrimitiveStatus {
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_INVALID = 0,  /* ָʾδ�յ�������ǿ�������ԭ��,������MACI-CFGԭ�� */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_RLSETUP,      /* ָʾ�յ� RRC_PHY_RL_SETUP_REQ(UL-EFACH) */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_RLMODIFY,     /* ָʾ�յ� RRC_PHY_RL_MODIFY_REQ(UL-EFACH) */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_F2D_RLMODIFY, /* ָʾ�յ� RRC_PHY_RL_MODIFY_REQ (FDPCH��F̬->D̬ */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_RLRELEASE,    /* ָʾ�յ� RRC_PHY_RL_RELEASE_REQ(UL-EFACH) */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErachPrimitiveTypeUint16;

/*
 * ö��˵��: �߼��ŵ�����
 */
enum WPHY_TOOL_MntnUlEfachLochType {
    WPHY_MNTN_UL_EFACH_LOCH_TYPE_CCCH      = 1, /* ������ǿFACH����ʹ��CCCH�߼��ŵ����� */
    WPHY_MNTN_UL_EFACH_LOCH_TYPE_DCCH_DTCH = 2, /* ������ǿFACH����ʹ��DCCH�߼��ŵ����� */

    WPHY_MNTN_UL_EFACH_LOCH_TYPE_BUTT /* ���һ��ö��ֵ */
};
typedef VOS_UINT16 WPHY_TOOL_MntnUlEfachLochTypeUint16;

/*
 * ö��˵��: PI���
 */
enum WPHY_TOOL_MntnRachRslt {
    WPHY_MNTN_RACH_INVALID = 0,  /* ��Чֵ */
    WPHY_MNTN_RACH_OK,           /* bit[0]������ɹ� */
    WPHY_MNTN_RACH_NACK,         /* bit[1]��NACK */
    WPHY_MNTN_RACH_RETRANS_FAIL, /* bit[2]���ﵽ�ش�������ʧ�� */
    WPHY_MNTN_RACH_PWR_FAIL,     /* bit[3]���������书�ʶ�ʧ�� */
    WPHY_MNTN_RACH_RSLT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRachRsltUint16;

/*
 * ö��˵��: ������ǿ�������ԭ�����״̬
 */
enum WPHY_TOOL_MntnErachEaiType {
    WPHY_MNTN_ERACH_EAI_POSITIVE = 0, /* EAI���Ϊ +1 */
    WPHY_MNTN_ERACH_EAI_NEGATIVE,     /* EAI���Ϊ -1 */
    WPHY_MNTN_ERACH_EAI_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErachEaiTypeUint16;

/*
 * ö��˵��: �ϱ�GSDR�ж�����/�ر�״̬
 */
enum GPHY_TOOL_MntnGsdrJudgeType {
    GPHY_MNTN_GSDR_JUDGE_DAIC_ON  = 0, /* �жϿ���GSDR */
    GPHY_MNTN_GSDR_JUDGE_DAIC_OFF = 1, /* �жϹر�GSDR */
    GPHY_MNTN_GSDR_JUDGE_DAIC_FAST_ON,

    GPHY_MNTN_GSDR_JUDGE_OFF_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnGsdrJudgeTypeUint16;

/*
 * ö��˵��: WDSP RF Reset
 */
enum WDSP_MNTN_ErrorRptRfreset {
    WPHY_MNTN_RFRESET_DETECT_EVENT = 0,
    WPHY_MNTN_RFRESET_EXECTUE_EVENT,
    WPHY_MNTN_RFRESET_BUTT_EVENT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRfresetEnentUint16;

/*
 * ö��˵��: ��ѡ��ʱ������
 */
enum GPHY_TOOL_NcellTimerId {
    GPHY_TOOL_NCELL_TIMER_C1 = 0, /* c1��ʱ�� */
    GPHY_TOOL_NCELL_TIMER_C2,     /* c2��ʱ�� */
    GPHY_TOOL_NCELL_TIMER_C2_ALL, /* �������е�����c2��ʱ�� */

    GPHY_TOOL_NCELL_TIMER_ID_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_NcellTimerIdUint16;

/*
 * ö��˵��: c1��c2��ʱ����ͣ״̬
 */
enum GPHY_TOOL_NcellTimerState {
    GPHY_TOOL_NCELL_TIMER_START = 0, /* ��ʱ������ */
    GPHY_TOOL_NCELL_TIMER_STOP,      /* ��ʱ��ֹͣ */

    GPHY_TOOL_NCELL_TIMER_STATE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_NcellTimerStateUnit16;
/*
 * ö��˵��: toolά����Ϣ��ר�õ������ּ�ö�ٶ���,���������±��Ӧ��;
 *          ����ͬUCOM_NV_PATH_MAIN_DIVERSITY_ENUM_UINT16
 */
enum GPHY_TOOL_MntnPathMainDiversity {
    GPHY_MNTN_PATH_MAIN      = 0, /* main path */
    GPHY_MNTN_PATH_DIVERSITY = 1, /* diversity path */
    GPHY_MNTN_PATH_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnPathMainDiversityUint16;

/*
 * ö��˵��: ������ͨ����صļĴ������ݲ�����Ϣ
 */
enum UPHY_TOOL_SampleRegComplete {
    UPHY_TOOL_SAMPLE_REG_CHN_UNRELATED = 0, /* �Ĵ�����ͨ���޹� */
    UPHY_TOOL_SAMPLE_REG_DDR_VALUE,         /* ddr��صļĴ���ֵ���� */
    UPHY_TOOL_SAMPLE_REG_BUS_DATA_CHN_ADDR, /* bus��data����ͨ����ַ�Ĵ��� */
    UPHY_TOOL_SAMPLE_REG_BUS_DATA_CHN_SIZE, /* bus��data����ͨ��size */
    UPHY_TOOL_SAMPLE_REG_BUFFER_INIT,       /* buffer ��ʼ�� */
    UPHY_TOOL_SAMPLE_REG_DUMP_START_ADDR,   /* dump������ʼ��ַ���� */
    UPHY_TOOL_SAMPLE_REG_DUMP_STOP_ADDR,    /* dump����������ַ���� */

    UPHY_TOOL_SAMPLE_REG_COMPLETE_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_SampleRegCompleteUint16;

/*
 * ö��˵��: EDRX T321״̬�ı��ԭ��
 */
enum WPHY_TOOL_MntnT321CtrlCause {
    WPHY_MNTN_T321_CTRL_PRIMITIVE      = 0, /* ԭ��ԭ�� */
    WPHY_MNTN_T321_CTRL_EDCH_ALLOC     = 1, /* EDCH���� */
    WPHY_MNTN_T321_CTRL_EDCH_REL       = 2, /* EDCH�ͷ� */
    WPHY_MNTN_T321_CTRL_HSDSCH         = 3, /* HSDSCH���ݽ��� */
    WPHY_MNTN_T321_CTRL_EXPIRE         = 4, /* T321��ʱ */
    WPHY_MNTN_T321_CTRL_EXPIRE_RESTART = 5, /* T321��ʱ��������������Ӧ�����֧:��ʱʱ��fach��Դ��interruptΪtrue */
    WPHY_MNTN_T321_CTRL_RF_PREEMPT     = 6, /* rf��Դ��ռ */
    WPHY_MNTN_T321_CTRL_RF_RESUME      = 7, /* rf��Դ�ָ� */
    WPHY_MNTN_T321_CTRL_ACCESS         = 8, /* ������� */
    WPHY_MNTN_T321_CTRL_OTHER          = 9, /* ����ԭ�� */

    WPHY_MNTN_T321_CTRL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT321CtrlCauseUint16;

/*
 * ö��˵��: EDRX T321״̬
 */
enum WPHY_TOOL_MntnT321CtrlStatus {
    WPHY_MNTN_T321_START  = 0, /* ԭ��ԭ�� */
    WPHY_MNTN_T321_STOP   = 1, /* EDCH���� */
    WPHY_MNTN_T321_EXPIRE = 2, /* EDCH�ͷ� */

    WPHY_MNTN_T321_CTRL_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT321CtrlStatusUint16;

/*
 * ö��˵��: GPHY�����쳣����
 */
enum GPHY_TOOL_MntnTimeOutType {
    GPHY_MNTN_TIME_OUT_TYPE_GTC_CONFIG       = 0, /* GTC��������ʱ */
    GPHY_MNTN_TIME_OUT_TYPE_FN_CONFIG        = 1, /* FN��������ʱ */
    GPHY_MNTN_TIME_OUT_TYPE_DRX_LATE         = 2, /* ���ѵ�ʱ���QB�Ѿ��޷����㼴ʱ��ҳ��Ҫ�� */
    GPHY_MNTN_TIME_OUT_TYPE_WAKE_SLICE_ERROR = 3, /* ����ʱ����� */

    GPHY_MNTN_TIME_OUT_TYPE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnTimeOutTypeUint16;

/*
 * ö��˵��: WPHY�����쳣����
 */
enum WPHY_TOOL_MntnTimeOutType {
    WPHY_MNTN_TIME_OUT_LATE_SLOT12_TASK_EXPIRED  = 0, /* slot12�����쳣 */
    WPHY_MNTN_TIME_OUT_TYPE_UPACC_TIME_EXCEPTION = 1, /* UPACC����ǰ����ʱ���쳣 */
    WPHY_MNTN_TIME_OUT_TYPE_SLAVE_SCHEDULE       = 2, /* ��ģ����ʱ�䳬ʱ */
    WPHY_MNTN_TIME_OUT_TYPE_DELTA_SFN_ERR        = 3, /* ʱ������������ */
    WPHY_MNTN_TIME_OUT_TYPE_CLK_SWITCH_DELAY     = 4, /* ʱ���л��ӳٳ�ʱ */
    WPHY_MNTN_TIME_OUT_TYPE_PICH_SETUP_DELAY     = 5, /* PICH����delay */

    WPHY_MNTN_TIME_OUT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTimeOutTypeUint16;

enum WPHY_CIQ_MetricId {
    WPHY_CIQ_METRIC_ID_RF60 = WPHY_MNTN_CIQ_MAKE_ID('R', 'F', '6', '0'),
    WPHY_CIQ_METRIC_ID_RF64 = WPHY_MNTN_CIQ_MAKE_ID('R', 'F', '6', '4'),
};
typedef VOS_UINT32 WPHY_IQ_MetricIdUint32;

enum WPHY_MNTN_CiqT313Status {
    WPHY_CIQ_T313_STATUS_STOPPED = 0,
    WPHY_CIQ_T313_STATUS_RUNNING,
    WPHY_CIQ_T313_STATUS_EXPIRED,
    WPHY_CIQ_T313_STATUS_UNKNOWN = 255
};
typedef VOS_UINT16 WPHY_MNTN_CiqT313StatusUint16;

enum WPHY_MNTN_CiqInfoCntStatus {
    WPHY_MNTN_CIQ_INFO_CNT_STOPPED = 0, /* ��Ϣ��û���� */
    WPHY_MNTN_CIQ_INFO_CNT_RUNNING,
    WPHY_MNTN_CIQ_INFO_CNT_UNKNOWN
};
typedef VOS_UINT16 WPHY_MNTN_CiqInfoCntStatusUint16;

/*
 * ö��˵��: ��ģ�����Ĳ�������״̬
 */
enum WPHY_TOOL_MntnSlaveMeasFinishFlag {
    WPHY_MNTN_SLAVE_MEAS_FINISH_NONE        = 0, /* ��ʼ��Ч״̬ */
    WPHY_MNTN_SLAVE_MEAS_FINISH_SINGLE_FREQ = 1, /* ��Ƶ�������� */
    WPHY_MNTN_SLAVE_MEAS_FINISH_ALL_FREQ    = 2, /* ȫ��Ƶ�������� */
    WPHY_MNTN_SLAVE_MEAS_FINISH_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSlaveMeasFinishFlagUint16;

/*
 * ö��˵��: ��¼��ǰ��������
 */
enum WPHY_MNTN_WakeCntType {
    WPHY_MNTN_WAKE_CNT_TYPE_INIT = 0,          /* ˯�� */
    WPHY_MNTN_WAKE_CNT_TYPE_MASTER_WAKE_NOMAL, /* ��ģ�������� */
    WPHY_MNTN_WAKE_CNT_TYPE_MASTER_WAKE_FORCE, /* ��ģǿ�ƻ��� */
    WPHY_MNTN_WAKE_CNT_TYPE_SLAVE_WAKE,        /* ��ģ���� */
    WPHY_MNTN_WAKE_CNT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_MNTN_WakeCntTypeUint16;

/*
 * ö��˵��:��ģС������״̬
 */
enum WPHY_TOOL_MntnSlaveCsStatus {
    WPHY_TOOL_MNTN_SLAVE_CS_INVALID         = 0x0,
    WPHY_TOOL_MNTN_SLAVE_CS_STEPAB_RUNNING  = 0x1, /* ��ģ���������б�� */
    WPHY_TOOL_MNTN_SLAVE_CS_STEPAEB_RUNNING = 0x2, /* ��ģstep1+Eb���б�� */
    WPHY_TOOL_MNTN_SLAVE_CS_STEP23_RUNNING  = 0x4, /* ��ģstepEB����step23���б�� */
    WPHY_TOOL_MNTN_SLAVE_CS_FINISH          = 0x8, /* ��ģС��������ɱ�� */
    WPHY_TOOL_MNTN_SLAVE_CS_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSlaveCsStatusUint16;

/*
 * ö��˵��: DCH̬AFC RESET״̬ö��
 */
enum WPHY_MNTN_DchAfcResetStatus {
    WPHY_MNTN_DCH_AFC_RESET_STATUS_CLOSE = 0, /* AFC RESET�����ر� */
    WPHY_MNTN_DCH_AFC_RESET_STATUS_OPEN  = 1, /* AFC RESET������ */
    WPHY_MNTN_DCH_AFC_RESET_STATUS_RESET = 2, /* Ƶƫ�ϴ󳡾�ִ��AFC RESET */
    WPHY_MNTN_DCH_AFC_RESET_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_MNTN_DchAfcResetStatusUint16;

/* easyrf��ؽӿ�,�������ǵ�������ͷ�ļ� */
/*
 * ö��˵��: ������������
 */
enum BBA_DBG_Action {
    BBA_DBG_ACTION_START = 0x0000, /* �������� */
    BBA_DBG_ACTION_QUERY = 0x0001, /* ������ѯ */
    BBA_DBG_ACTION_STOP  = 0x0002, /* ����ֹͣ */

    BBA_DBG_ACTION_BUTT
};
typedef VOS_UINT16 BBA_DBG_ActionUint16;

/*
 * ö��˵��: �������õĲ�������ö��
 */
enum EASYRF_DBG_CfgType {
    EASYRF_DBG_CFG_TYPE_BBIC_DFE  = 0x0000, /* BBIC DFE IQ���� */
    EASYRF_DBG_CFG_TYPE_RFIC0_DBG = 0x0001, /* RFIC0 DBG���� */
    EASYRF_DBG_CFG_TYPE_RFIC0_BUS = 0x0002, /* RFIC0 BUS���� */
    EASYRF_DBG_CFG_TYPE_RFIC0_MRX = 0x0003, /* RFIC0 MRX���� */
    EASYRF_DBG_CFG_TYPE_RFIC1_DBG = 0x0004, /* RFIC1 DBG���� */
    EASYRF_DBG_CFG_TYPE_RFIC1_BUS = 0x0005, /* RFIC1 BUS���� */
    EASYRF_DBG_CFG_TYPE_RFIC1_MRX = 0x0006, /* RFIC1 MRX���� */

    EASYRF_DBG_CFG_TYPE_BUTT
};
typedef VOS_UINT16 EASYRF_DBG_CfgTypeUint16;

/*
 * ö��˵��:EASYRF DEBUGģ��Ĳ�����������
 */
enum EASYRF_DBG_SampleAction {
    EASYRF_DBG_SAMPLE_REG_CONFIG    = 0x0000, /* ��Ӧ���üĴ����б� */
    EASYRF_DBG_SAMPLE_REG_UPDATE    = 0x0001, /* ��Ӧ���¼Ĵ����б� */
    EASYRF_DBG_SAMPLE_REG_ENABLE    = 0x0002, /* ��Ӧʹ�ܼĴ����б� */
    EASYRF_DBG_SAMPLE_REG_REPORT    = 0x0003, /* ��Ӧ��ѯ�Ĵ����б� */
    EASYRF_DBG_SAMPLE_REG_PRE_STOP  = 0x0004, /* ��ӦԤֹͣ,����Ҫ��ѯ�Ϳ���ֹͣ�ļĴ����б� */
    EASYRF_DBG_SAMPLE_REG_QUERY     = 0x0005, /* ��ӦоƬ�ṩ�������ѯ״̬�Ĵ���,Ϊ�ض�״̬����ֹͣ������б� */
    EASYRF_DBG_SAMPLE_REG_POST_STOP = 0x0006, /* ��Ӧ��ѯ��ſ���ֹͣ�ļĴ����б� */

    EASYRF_DBG_SAMPLE_REG_BUTT
};
typedef VOS_UINT16 EASYRF_DBG_SampleActionUint16;

/*
 * ö��˵��:EASYRF �Ĵ����ض�ʹ�ܿ���
 */
enum EASYRF_OM_Switch {
    EASYRF_OM_SWITCH_OFF = 0x0,
    EASYRF_OM_SWITCH_ON  = 0x1,
    EASYRF_OM_SWITCH_BUTT
};
typedef VOS_UINT16 EASYRF_OM_SwitchUint16;

/*
 * ö��˵��:EASYRF �Ĵ����ض�TX/RX����
 */
enum EASYRF_OM_ReadTrx {
    EASYRF_OM_READ_TX   = 0x0,
    EASYRF_OM_READ_RX   = 0x1,
    EASYRF_OM_READ_TRX  = 0x2,
    EASYRF_OM_READ_BUTT
};
typedef VOS_UINT16 EASYRF_OM_ReadTrxUint16;

/*
 * ö��˵��:PDͳ��ģʽ
 */
enum EASYRF_PD_Mode {
    EASYRF_PD_MODE_POWER = 0, /* PDͳ�ƹ���ģʽ */
    EASYRF_PD_MODE_GAIN  = 1, /* PDͳ��GAINģʽ */
    EASYRF_PD_MODE_BUTT
};
typedef VOS_UINT16 EASYRF_PD_ModeUint16;

/*
 * ö��˵��:EASYRF �Ĵ����ض�TX/RX����
 */
enum EASYRF_PD_OmSwitch {
    EASYRF_PD_OM_OFF = 0x0,
    EASYRF_PD_OM_ON  = 0x1,
    EASYRF_PD_OM_BUTT
};
typedef VOS_UINT16 EASYRF_PD_OmSwitchUint16;

/*
 * ö��˵��:EASYRF RATMODE
 */
enum EASYRF_OM_Ratmode {
    EASYRF_OM_RAT_MODE_GSM   = 0x0000,
    EASYRF_OM_RAT_MODE_WCDMA = 0x0001,
    EASYRF_OM_RAT_MODE_CDMA  = 0x0002,
    EASYRF_OM_RAT_MODE_LTE   = 0x0003,
    EASYRF_OM_RAT_MODE_NR    = 0x0004,
    EASYRF_OM_RAT_MODE_BUTT  = 0x0005
};
typedef VOS_UINT16 EASYRF_OM_RatmodeUint16;

/*
 * ö��˵��:PA��λģʽ
 */
enum EasyrfOmTxPaModeEnum {
    EASYRF_OM_TX_PA_MODE_HIGH     = 0, /* PA�ߵ�λ */
    EASYRF_OM_TX_PA_MODE_MID      = 1, /* PA�е�λ */
    EASYRF_OM_TX_PA_MODE_LOW      = 2, /* PA�͵�λ */
    EASYRF_OM_TX_PA_MODE_ULTRALOW = 3, /* PA���͵�λ */
    EASYRF_OM_TX_PA_MODE_BUTT
};
typedef VOS_UINT16 EasyrfOmTxPaModeEnumUint16;

/*
 * ö��˵��: EASYRF��������,���ն���,�ȶ������еĵ�������,���������ں󲿡�
 *           ע:�������㺯��ͨ����������,ö���и������˳�����������
 */
enum WPHY_TOOL_AeasyrfConfigType {
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_NV_LOAD           = 0,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_RX_START          = 1,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_RX_STOP           = 2,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_RX_STANDBY        = 3,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_RX_RESUME         = 4,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_RX_DIVANT_ENABLE  = 5,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_RX_DIVANT_DISABLE = 6,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_RX_AGC_CONFIG     = 7,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_TX_START          = 8,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_TX_STOP           = 9,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_TX_APC_CONFIG     = 10,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_TX_STANDBY        = 11,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_SINGLE_TX_RESUME         = 12,

    /* ������������� */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_START_AGC      = 13, /* ����NV RX START+AGC SET�������,NV�����ɵײ��ж� */
    /* ����RX STOP+RX START+AGC SET�������,NV�����ɵײ��ж� */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_STOP_START_AGC = 14,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_RESUME_AGC     = 15, /* ����RX RESUME + AGC SET�������,����ҪNV���� */
    /* ����RX STANDBY + RESUME + AGC SET�������,����ҪNV���� */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_STANDBY_RESUME_AGC = 16,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_TX_START_APC = 17, /* ����TX START + APC SET�������,NV�����ɵײ��ж� */
    /* ����RX START + AGC SET+����TX START+APC�������,NV�����ɵײ��ж� */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_TX_START_AGC_APC = 18,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_TX_STOP = 19, /* ����RX STOP + TX STOP�������,����Ҫ����NV */
    /* ����RX STOP+TX STOP+RX START+TX START+AGC SET+APC SET�������,NV�����ɵײ��ж� */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_TX_STOP_RX_TX_START_AGC_APC = 20,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_TX_STOP_START_APC = 21, /* ����TX STOP+TX START+APC��� */

    WPHY_TOOL_AEASYRF_CONFIG_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_AeasyrfConfigTypeUint16;

/*
 * ö��˵��: ָʾRX�Ǵ򿪻��ǹر�״̬
 */
enum WPHY_TOOL_AeasyrfRxStatus {
    WPHY_TOOL_AEASYRF_RX_STATUS_OFF = 0, /* RX�ر�״̬ */
    WPHY_TOOL_AEASYRF_RX_STATUS_ON  = 1, /* RX��״̬ */

    WPHY_TOOL_AEASYRF_RX_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_AeasyrfRxStatusUint16;

/*
 * ö��˵��: ָʾTX�Ǵ򿪻��ǹر�״̬
 */
enum WPHY_TOOL_AeasyrfTxStatus {
    WPHY_TOOL_AEASYRF_TX_STATUS_OFF = 0, /* TX�ر�״̬ */
    WPHY_TOOL_AEASYRF_TX_STATUS_ON  = 1, /* TX��״̬ */

    WPHY_TOOL_AEASYRF_TX_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_AeasyrfTxStatusUint16;

enum WPHY_TOOL_NoSignalMntnType {
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_IP2_CAL            = 1 << 0,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_IQ_MISMATCH        = 1 << 1,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_DPD_SELF_CAL       = 1 << 2,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_ET_DELAY_SELF_CAL  = 1 << 3,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_ET_DELAY_EXTRACT   = 1 << 4,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_ET_DELAY_FREQ_COMP = 1 << 5,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_RX_FAST_CAL        = 1 << 6,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_TX_FAST_CAL        = 1 << 7,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_ET_FAST_CAL        = 1 << 8,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_RX_DC_OFFSET_CAL   = 1 << 9,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_MRX_DC_OFFSET_CAL  = 1 << 10,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_ET_PA_GAIN_CAL     = 1 << 11,
    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_SLOW_CAL           = 1 << 12,
    /* ���´�������� */

    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_BUTT = 1 << 13,
};
typedef VOS_UINT16 WPHY_TOOL_NoSignalMntnTypeUint16;

/*
 * ö��˵��: ����SAR�㷨�������ƹ��ʵ�ԭ��
 */
enum WPHY_TOOL_SwSarUpdateReason {
    WPHY_TOOL_SW_NORMAL_UPDATE             = 0,  /* ����SAR�㷨���������ڸ���(40ms����40ms�ı���)���� */
    WPHY_TOOL_SW_ANT_CHANGE_INHERIT        = 1,  /* �����߷����л����»���sarlimitAvgMax�����仯��SAR�㷨�̳� */
    WPHY_TOOL_SW_NO_ANT_CHANGE_RESTART     = 2,  /* �������л�(sensorcase��modemcase����������״̬)����sarlimitAvgMax�����仯������SAR�㷨���� */
    WPHY_TOOL_SW_FREQ_CHANGE_RESTART       = 3,  /* �������л�(Ƶ���Ƶ���л�)����sarlimitAvgMax�����仯������SAR�㷨���� */
    WPHY_TOOL_SW_REDUCTION_CHANGE_RESTART  = 4,  /* �������л�(SAR���ʻ��˵ȼ�)����sarlimitAvgMax�����仯������SAR�㷨���� */
    WPHY_TOOL_SW_RESTART_SET_W_MASTER      = 5,  /* ����WΪ��ģ��ʱ��,����SAR�㷨���� */
    WPHY_TOOL_SW_RESTART_RELEASE_ALL       = 6,  /* ���ͷŵĵط�,����SAR�㷨������ */
    WPHY_TOOL_SW_RESTART_RF_RESUME         = 7,  /* ��DCH/FACH̬��ռ�ָ��ĵط�,����SAR�㷨������ */
    WPHY_TOOL_SW_REASON_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_SwSarUpdateReasonUint16;

/*
 * 4 ��Ϣͷ����
 */

/*
 * 5 ��Ϣ����
 */

/*
 * 6 STRUCT����
 */

/*
 * 6.0 STRUCT����,WG��������
 */

/*
 * ö��˵��: ���߽����ṹͷ
 */

typedef struct {
    VOS_UINT16 funcType; /* ��ά�ɲ���0x4,�ڲ���Ϣץȡ��0x */
    VOS_UINT16 length;   /* ֮��ĳ���,��λΪbyte */
    VOS_UINT32 sn;
    VOS_UINT32 timeSlice;
} UPHY_TOOL_TypeHeader;

/*
 * ö��˵��: ��Ϣͷ�ṹ����
 */
typedef struct {
    VOS_UINT16 msgId;
    VOS_UINT16 reserved;
    VOS_UINT16 data[4];
} UPHY_TOOL_OmMsgHeader;

/*
 * ö��˵��: ��Ϣͷ�ṹ����
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    VOS_UINT16 reserved;
} UPHY_TOOL_IntraMsgHeader;

/*
 * �ṹ˵��: gu����ά����Ϣ�ϱ����أ��������������
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;            /* �����ֶ� */
    UPHY_TOOL_MntnRptStatusUint16 uphySocpMntnStatus; /* ��ʱ��SOCP����ά���ϱ�ʹ�ܱ�� */
    VOS_UINT16                    uphySocpTimeMs;     /* �����ϱ��Ŀ��������ڣ���λms��Ĭ��5s */
} UPHY_TOOL_MntnFuncSwitch;

/*
 * 6.1 STRUCT����,UPHY����
 */

/*
 * �ṹ˵��: ͨ�����ϱ�
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  toolsId;  /* �����ֶ� */
    WPHY_TOOL_MntnChannelUint16 channel;  /* Ĭ��ÿ֡�ϱ�  */
    VOS_UINT16                  resurved; /* �����ֶ� */
} WPHY_TOOL_ChannelReportInd;

/*
 * �ṹ˵��: ͨ�����ϱ�
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  toolsId;  /* �����ֶ� */
    GPHY_TOOL_MntnChannelUint16 channel;  /* Ĭ��ÿ֡�ϱ�  */
    VOS_UINT16                  resurved; /* �����ֶ� */
} GPHY_TOOL_ChannelReportInd;

/*
 * �ṹ˵��: TOOL����GPHY,SDT����״̬
 */

/*
 * �ṹ˵��: GPHY��ά�ɲ⹫���ظ���Ϣ�ṹ
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    UPHY_TOOL_MntnRsltUint16  result;  /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                rsv;     /* �����ֶ� */
} UPHY_TOOL_CommonCnf;

/*
 * �ṹ˵��: �����ַ��ѯ����ԭ��
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */     /* ԭ������ */
    VOS_UINT16 toolsId;   /* �����ֶ� */
    VOS_UINT32 paraValue; /* �Ƿ���Ƹò��� */
} UPHY_TOOL_QueryShareAddrReq;

/*
 * �ṹ˵��: UPHY�����ַ��ѯ����ظ�
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;   /* �����ֶ� */
    VOS_UINT32                shareAddr; /* �����׵�ַ */
} UPHY_TOOL_QueryShareAddrInd;

/*
 * �ṹ˵��: TOOL����UPHY,SDT����״̬
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* ԭ������ */
    VOS_UINT16 toolsId; /* �����ֶ� */
    VOS_UINT16 sdtStatus;
    VOS_UINT16 rsv;
} UPHY_TOOL_SdtLinkStatusReq;

/*
 * �ṹ˵��: �����ϱ�
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;       /* �����ֶ� */
    UPHY_TOOL_MntnRptLevelUint16 errorRptLevel; /* �����ϱ����󼶱� */
    VOS_UINT16                   rsv;
} UPHY_TOOL_ErrorReportReq;

/*
 * �ṹ˵��: �����ϱ�
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                fileNo;  /* �ļ������� */
    VOS_UINT16                lineNo;  /* ������к� */
    VOS_UINT32                timeSlice;
    VOS_UINT32                logId;                                      /* PHY�����ͳһ���LOG ID */
    VOS_UINT16                reserved;                                   /* �����ֶ� */
    VOS_UINT16                varCnt;                                     /* �ϱ��������� */
    VOS_UINT32                varValue[UPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* �ϱ�����ֵ */
} UPHY_TOOL_ErrorReportInd;

/*
 * �ṹ˵��: WPHY����ϵͳG�Ĵ����б��·�
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* ԭ������ */
    VOS_UINT16 toolsId; /* �����ֶ� */
    VOS_UINT16 rsv;     /* �����ϱ����󼶱� */
    VOS_UINT16 regNum;  /* �����ϱ����󼶱� */
    VOS_UINT32 regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_WphyInterGregReportReq;

/*
 * �ṹ˵��: WPHY����ϵͳG�Ĵ����յ��ظ�
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    UPHY_TOOL_MntnRsltUint16  result;  /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                regNum;  /* �����ϱ����󼶱� */
    VOS_UINT32                regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_WphyInterGregReportCnf;

/*
 * �ṹ˵��: WPHY����ϵͳG�Ĵ���ֵ�ϱ�
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                rsv;     /* �����ϱ����󼶱� */
    VOS_UINT16                regNum;  /* �����ϱ����󼶱� */
    VOS_UINT32                regData[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_WphyInterGregReportInd;

/*
 * �ṹ˵��: ֹͣWPHY����ϵͳG�Ĵ����ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* ԭ������ */
    VOS_UINT16 toolsId; /* �����ֶ� */
    VOS_UINT32 rsv;     /* �����ϱ����󼶱� */
} UPHY_TOOL_WphyInterGregStopReq;

/*
 * �ṹ˵��: GPHY����ϵͳW�Ĵ����б��·�
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                rsv;     /* �����ϱ����󼶱� */
    VOS_UINT16                regNum;  /* �����ϱ����󼶱� */
    VOS_UINT32                regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_GphyInterWregReportReq;

/*
 * �ṹ˵��: GPHY����ϵͳW�Ĵ����յ��ظ�
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    UPHY_TOOL_MntnRsltUint16  result;  /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                regNum;  /* �����ϱ����󼶱� */
    VOS_UINT32                regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_GphyInterWregReportCnf;

/*
 * �ṹ˵��: GPHY����ϵͳW�Ĵ���ֵ�ϱ�
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                rsv;     /* �����ϱ����󼶱� */
    VOS_UINT16                regNum;  /* �����ϱ����󼶱� */
    VOS_UINT32                regData[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_GphyInterWregReportInd;

/*
 * �ṹ˵��: ֹͣGPHY����ϵͳW�Ĵ����ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* ԭ������ */
    VOS_UINT16 toolsId; /* �����ֶ� */
    VOS_UINT32 rsv;     /* �����ϱ����󼶱� */
} UPHY_TOOL_GphyInterWregStopReq;

/*
 * �ṹ˵��: ͨ�����ϱ�
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                       toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRfresetEnentUint16 abnormalEvent;
    VOS_UINT16                       abnormalRfChn;
    VOS_UINT16                       rfCheckType;
    VOS_UINT16                       abnEventCnt;
} WPHY_TOOL_RfErrorResetReport;

/*
 * �ṹ˵��: ��ʱ��Ϣ
 */
typedef struct {
    VOS_UINT16 sfn;  /* ϵͳ��ʱSFNֵ */
    VOS_UINT16 cfn;  /* ϵͳ��ʱCFNֵ */
    VOS_UINT16 slot; /* ϵͳ��ʱSLOTֵ */
    VOS_UINT16 chip; /* ϵͳ��ʱCHIPֵ */
#if (FEATURE_EASYRF == FEATURE_ON)
    VOS_UINT16 baseTimeSlot; /* ��׼��ʱSLOTֵ */
    VOS_UINT16 baseTimeChip; /* ��׼��ʱCHIPֵ */
    VOS_UINT32 sysCurrentMc; /* ��ʱMCֵ */
#endif
} WPHY_TOOL_TimingInfo;

/*
 * �ṹ˵��: WPHY DRX��ά�ɲ���Ϣ
 */
typedef struct {
    VOS_UINT32           forbidSleepReason; /* ��ֹ˯��ԭ�� */
    VOS_UINT32           sndSleepReqCnt;    /* WPHY ����˯����Ϣ���� */
    VOS_UINT32           sndSleepReqSlice;  /* WPHY ����˯����Ϣslice */
    VOS_UINT32           drxRcvSleepSlice;  /* DRX  �յ�˯����Ϣslice */
    VOS_UINT32           drxSendSleepSlice; /* DRX  ����˯����Ϣslice */
    VOS_UINT32           drxNeedWakeSlice;  /* DRX  Ԥ�ڻ���slice */
    VOS_UINT32           drxRcvWakeSlice;   /* DRX  �յ�������Ϣslice */
    VOS_UINT32           drxSendWakeSlice;  /* DRX  ���ͻ�����Ϣslice */
    VOS_UINT32           rcvWakeMsgCnt;     /* WPHY ���յ�������Ϣ���� */
    VOS_UINT32           rcvWakeMsgSlice;   /* WPHY ���յ�������Ϣslice */
    VOS_UINT32           dcStartSlice;      /* WPHY ����DC�ȶ���ʱ��ʱ��slice */
    VOS_UINT32           dcEndSlice;        /* WPHY DC�ȶ���ʱ����ʱslice */
    VOS_UINT32           mpStartSlice;      /* WPHY ��һ�ζྶ��������slice */
    VOS_UINT32           rcvMpSrchIntCnt;   /* WPHY ���յ��ྶ�����жϼ��� */
    VOS_UINT32           rcvMpSrchIntSlice; /* WPHY ���յ��ྶ����slice */
    VOS_UINT32           openSlotIntCnt;    /* WPHY ��ʱ϶�жϼ��� */
    VOS_UINT32           openSlotIntSlice;  /* WPHY ��ʱ϶�ж�slice */
    VOS_UINT32           pichSetupSlice;    /* WPHY PICH����slice */
    VOS_UINT32           piIntSlice;        /* WPHY PI�ж�slice */
    WPHY_TOOL_TimingInfo piRealTime;        /* WPHY PIλ��ʱ�� */
    WPHY_TOOL_TimingInfo pichSetupTime;     /* WPHY PICH����ʱ�� */
} UPHY_TOOL_WphyDrxMntnInfo;

/*
 * �ṹ˵��: ������ʱ�Ĵ����Ϣ
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                rsv;       /* �����ֶ� */
    VOS_UINT32                awakeTime; /* ��Ҫ���ѵ�ʱ�䣬W��ΪSFN��G��ΪSlice */
    VOS_UINT32                curTime;   /* ʵ�ʻ��ѵ�ʱ�䣬W��ΪSFN��G��ΪSlice */
    VOS_UINT32                awakeHWInfo[UPHY_TOOL_SLEEP_INFO_NUM]; /* OAM��ά�ɲ���Ϣ */
    UPHY_TOOL_WphyDrxMntnInfo wphyDrxMntn;                           /* WPHY��ά�ɲ���Ϣ */
} UPHY_TOOL_DrxLateInfoInd;

/*
 * �ṹ˵��: �ջ���г�㷨����������ά�ɲ��ϱ�
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;      /* �����ֶ� */
    VOS_INT16                 gammaReal;    /* ����ϵ����������ʵ�� */
    VOS_INT16                 gammaImag;    /* ����ϵ�����������鲿 */
    VOS_UINT16                algState;     /* �㷨״̬ */
    VOS_UINT16                statusBitMap; /* �㷨������״̬λ���־ */
    VOS_UINT16                gridWinner;   /* Ӯ�Ҹ�㣬X�ڸ�8λ��Y�ڵ�8λ */
    VOS_UINT16                gridTest;     /* ���Ը�㣬X�ڸ�8λ��Y�ڵ�8λ */
    VOS_UINT16                gridTrace[UPHY_TOOL_CLT_TRACE_NUM]; /* �ƽ��켣��¼ */
    VOS_INT32                 fom;                                /* ��ǰFOMֵ */
    VOS_INT32                 referenceFom;                       /* �ο�FOMֵ */
    VOS_UINT32                sliceSinceLastGoodFom;              /* ����ǰ�κõ�FOM��ʱ�� */
    VOS_UINT32                sliceSinceLastHold;                 /* ����ǰ�α�������̬��ʱ�� */
    VOS_UINT32                sliceSinceLastFrozen;               /* ����ǰ�α��ֶ���̬��ʱ�� */
    VOS_UINT16                fomGoodThreshold;                   /* Good FOM���� */
    VOS_UINT16                fomPoorThreshold;                   /* Poor FOM���� */
    VOS_UINT16                fomHoldThreshold;                   /* Hold FOM���� */
    VOS_UINT16                reserved;                           /* Ԥ�� */
} RCM_TOOL_MntnCltAlgRpt;

/*
 * 6.2 STRUCT����,GPHY����
 */

/*
 * �ṹ˵��: GPHY��ά�ɲ⹫���ظ���Ϣ�ṹ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    GPHY_TOOL_MntnRsltUint16  result;  /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                rsv;     /* �����ֶ� */
} GPHY_TOOL_CommonCnf;

/*
 * �ṹ˵��: GPHY GPRS�ŵ����������ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;   /* �����ֶ� */
    VOS_UINT16                blerFlag;  /* �Ƿ��ϱ�BLER */
    VOS_UINT16                rptPeriod; /* BLER�ϱ�������,��λ:���� */
} GPHY_TOOL_BlerReportReq;

/*
 * �ṹ˵��: GPHY�ϱ�BLER�Ľṹ���ӽṹ
 */

typedef struct {
    VOS_UINT32 blkCnt;
    VOS_UINT32 errBlkCnt;
} GPHY_BLER_Para;

typedef struct {
    VOS_UINT32     rcvBlkCnt;
    VOS_UINT32     dummyCnt;
    GPHY_BLER_Para blerPara[GPHY_PS_MAX_RX_TSN_NUM];
} GPHY_GPRS_BlerRecord;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    VOS_UINT16                toolsId;
    GPHY_GPRS_BlerRecord      gprsBlerRecord;
} GPHY_TOOL_BlerReportInd;

/*
 * �ṹ˵��: GPHY GPRS�ŵ���SNR�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER      /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16 toolsId; /* �����ֶ� */
    VOS_UINT16 snrFlag; /* �Ƿ��ϱ�SNR */
    VOS_UINT16 rsv;     /* �����ֶ� */
} GPHY_TOOL_SnrReportReq;

/*
 * �ṹ˵��: GPHY�ϱ�SNR�Ľṹ���ӽṹ
 */

typedef struct {
    VOS_UINT16 rptValidNum;
    VOS_UINT16 rsv;
    VOS_INT16  snr[GPHY_PS_MAX_RX_TSN_NUM];
    VOS_UINT16 rsv2;
} GPHY_GPRS_SnrRecord;

/* ��ά�ɲ�:SNR�ϱ��ṹ */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    VOS_UINT16                toolsId;
    GPHY_GPRS_SnrRecord       gprsSnrRecord;
} GPHY_TOOL_SnrReportInd;

/*
 * �ṹ˵��: GPHY AFC����ֵ��Ϣ�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                afcFlag; /* �Ƿ��ϱ�AFCֵ */
    VOS_UINT16                rsv;     /* �����ֶ� */
} GPHY_TOOL_AfcReportReq;

/*
 * �ṹ˵��: GPHY�ϱ�AFC�Ľṹ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    VOS_UINT16                toolsId;
    VOS_INT16                 afcOffset;
    VOS_UINT16                rsv;
} GPHY_TOOL_AfcReportInd;

/*
 * �ṹ˵��: GPHY �����¹�����Ϣ�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;     /* �����ֶ� */
    VOS_UINT16                pwrCtrlFlag; /* �Ƿ��ϱ����ʿ��� */
    VOS_UINT16                rsv;         /* �����ֶ� */
} GPHY_TOOL_PowerCtrlReportReq;

/*
 * �ṹ˵��: GPHY�ϱ����ʿ��Ʋ����Ľṹ���ӽṹ
 */

typedef struct {
    VOS_UINT16 txChannel;
    VOS_UINT16 txTn;
    VOS_INT16  txPwr;
    VOS_UINT16 gammaTn;
    VOS_UINT16 gammaBand;
    VOS_UINT16 alpha;
    VOS_UINT16 pMax;
    VOS_UINT16 rsv;
} GPHY_PWR_CtrlPara;

typedef struct {
    VOS_UINT16        txCnt;
    VOS_UINT16        rsv;
    GPHY_PWR_CtrlPara pwrCtrlPara[GPHY_PS_MAX_TX_TSN_NUM];
} GPHY_POWER_ControlRecord;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    VOS_UINT16                toolsId;
    GPHY_POWER_ControlRecord  pwrCtrlRecord;
} GPHY_TOOL_PowerCtrlReportInd;

/*
 * �ṹ˵��: GPHY ������ָ�����ʷ�����������
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;     /* �����ֶ� */
    VOS_UINT16                pwrCtrlFlag; /* �Ƿ���ָ�����ʷ��� */
    VOS_UINT16                txPwr;       /* ָ������,��λ:0.1dB */
} GPHY_TOOL_PowerCtrlSetReq;

/*
 * �ṹ˵��: GPHY�ϱ�EGPRS�ŵ������ϱ��ṹ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;     /* �����ֶ� */
    VOS_UINT16                gmskMeanBep; /* 0-31,0xffΪ��Чֵ */
    VOS_UINT16                meanBep8psk; /* 0-31,0xffΪ��Чֵ */
    VOS_UINT16                gmskCvBep;   /* 0-7,0xffΪ��Чֵ */
    VOS_UINT16                cvBep8psk;   /* 0-7,0xffΪ��Чֵ */
} GPHY_TOOL_ChannelQualityInd;

/*
 * �ṹ˵��: GPHY ��ά�ɲ⹦�ܿ����·��Ľṹԭ��:�������ƹ�����������
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;     /* �����ֶ� */
    VOS_UINT32                funcBitMask; /* ��ӦBitλ��λ,����Ƹù�����Ч */
} GPHY_TOOL_FuncBitCtrlReq;

/*
 * �ṹ˵��: GPHY��ά�ɲ���������·��Ľṹԭ��:�������Ʋ�����������
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16               toolsId;     /* �����ֶ� */
    GPHY_TOOL_ParaCtrlUint16 paraId;      /* ������ID,���Ʋ������б� */
    VOS_UINT16               paraCtrlFlg; /* �Ƿ���Ƹò��� */
    VOS_UINT32               paraValue1;  /* ���Ʋ�����ֵ */
    VOS_UINT32               paraValue2;  /* Ŀǰ����,������չ */
} GPHY_TOOL_ParaControlReq;

/*
 * �ṹ˵��: GPHY �ϱ�ǰ�˶�̬��Ϣ�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                msgRptFlag; /* ָʾ��Ϣ��ص��ϱ������Ƿ��б�� */
    VOS_UINT16                rsv;        /* �����ֶ� */
} GPHY_TOOL_RfFrontStaticReq;

/*
 * �ṹ˵��: RTC���ܴ򿪺͹ر�����
 */
typedef struct {
    VOS_MSG_HEADER
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    UPHY_TOOL_RtcDebugUint16  rtcCtrl; /* RTC�򿪻��߹ر� */
    VOS_UINT16                rsv;
} GPHY_TOOL_BbpRtcReq;

/*
 * �ṹ˵��: GPHY�ϱ�ǰ�˶�̬��Ϣ�ϱ�
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    VOS_UINT16 rfTrcvOnState; /* RF�ܿ��أ�1Ϊ�򿪣�0Ϊ�ر� */
    VOS_UINT16 rfRxOnOrOff;   /* 04h[1:0] = 3Ϊ�� */
    VOS_UINT16 rxPathselect;  /* 04h[4:3] = 3Ϊ���ּ�������2������ */
    VOS_UINT16 abbRxConfig;   /* 7Ϊ���ּ�������3������ */

    VOS_UINT16 rfTxOnOrOff; /* 02h[1:0] = 3Ϊ�� */
    VOS_UINT16 rfTxShdn;    /* RF���д򿪿��ƣ�0��BBP���� */

    VOS_UINT16 gpioFuncOn; /* Ӳ��֧��GPIO */
    VOS_UINT16 gpioPaMode; /* GPIO PA mode */

    VOS_UINT16 mipiFuncOn; /* Ӳ��֧��MIPI */
    VOS_UINT16 mipiPaMode; /* MIPI PA mode */

    VOS_UINT16 aptFuncOn; /* Ӳ��֧��APT */
    VOS_UINT16 aptPaMode; /* APT��ѹmode */
} GPHY_TOOL_RfFrontStaticInd;

/*
 * �ṹ˵��: PHY �ڲ���Ϣ�ϱ���������
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                msgRptFlag; /* ָʾ�ڲ���Ϣ�Ƿ��ϱ� */
    VOS_UINT16                rsv;        /* �����ֶ� */
} GPHY_TOOL_IntraMsgRptCtrlReq;

/*
 * �ṹ˵��: GPHY ������AMR�ŵ������������ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                amrRptFlag; /* �Ƿ��ϱ�AMR��Ϣ */
    VOS_UINT16                rsv;        /* �����ֶ� */
} GPHY_TOOL_AmrReportReq;

/*
 * �ṹ˵��: GPHY�ϱ�AMR��Ϣ����
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                crcErrFlg;  /* ������0-��ȷ;1-���� */
    VOS_UINT16                decodeMode; /* ���б��뷽ʽ */
    VOS_UINT16                decodeRate; /* ���б������� */
    VOS_UINT16                encodeMode; /* ���б��뷽ʽ */
    VOS_UINT16                encodeRate; /* ���б������� */
    VOS_UINT16                rsv;
} GPHY_TOOL_AmrReportInd;

/*
 * �ṹ˵��: GPHYץȡ�ǹ̶���ַ������
 */
typedef struct {
    VOS_UINT32 blkAddr; /* ץȡ���ݿ���׵�ַ */
    VOS_UINT32 blkLen;  /* ץȡ���ݿ�ĳ���,��λ:32bit */
} GPHY_TOOL_BlkReport;

typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                rptFlag;
    VOS_UINT16                rsv;
    GPHY_TOOL_BlkReport       rptBlk[GPHY_TOOL_RPT_BLK_MAX_NUM];
} GPHY_TOOL_BlkReportListReq;

/*
 * �ṹ˵��: GPHYץȡ�ǹ̶���ַ������
 */

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    GPHY_TOOL_BlkReport       rptBlk[GPHY_TOOL_RPT_BLK_MAX_NUM];
    VOS_UINT32                data[2];
} GPHY_TOOL_BlkReportListInd;

/*
 * �ṹ˵��: GSM KEY RFIC REG Report
 */
typedef struct {
    VOS_UINT32 tddOperate;
    VOS_UINT32 rx1Operate;
    VOS_UINT32 rx1Rsm;
    VOS_UINT32 txOpertate;
    VOS_UINT32 txRsm;
    VOS_UINT32 rx2Operate;
    VOS_UINT32 pllRx2Mode;
    VOS_UINT32 pllTxMode;
    VOS_UINT32 pllRx1Mode;
} GPHY_TOOL_RficKeyRegRpt;

/*
 * �ṹ˵��: RX IQ IMBALANCE ά����Ϣ�ṹ
 */
typedef struct {
    VOS_UINT16 calBpFlag;     /* �����Ƿ�򿪵ı�� 0��ʾ�رգ�1��ʾ�򿪡���ô�˴ε��ϱ��ķ�����λֵ��Ч */
    VOS_INT16  rptAm;         /* �����ϱ��ķ���ֵ */
    VOS_INT16  rptPhase;      /* �����ϱ�����λ */
    VOS_INT16  currMainAm;    /* alpha�˲������������ֵ */
    VOS_INT16  currMainPhase; /* alpha�˲����������λֵ */
    VOS_INT16  currDivAm;     /* alpha�˲���ķּ�����ֵ */
    VOS_INT16  currDivPhase;  /* alpha�˲���ķּ���λֵ */
    VOS_UINT16 rsv;           /* ���� */
} GPHY_TOOL_RxIqImbalance;

/*
 * �ṹ˵��: AGC��λ ά����Ϣ�ṹ
 */
typedef struct {
    VOS_UINT16 meaFastFlag;     /* �Ƿ�Ϊ���ٲ����������� */
    VOS_INT16  wbAgcRssi125dbm; /* ������� */
    VOS_UINT16 agcCurrGain;     /* ��ǰ���õĵ�λ */
    VOS_UINT16 agcNextGain;     /* ��һ�����õĵ�λ */
    VOS_UINT16 currBlockMode;   /* ��ǰblk ģʽ */
    VOS_UINT16 nextBlockMode;   /* ���Ƴ�����һ��blk ģʽ Ϊ0Ϊnoblkģʽ Ϊ1Ϊblockģʽ  */
    VOS_UINT16 antPath;         /* �������Ƿּ���Ϣ */
    VOS_UINT16 rsv;             /* ���� */
} GPHY_TOOL_AgcBlkNoblkMode;

/*
 * �ṹ˵��: ��ʱ϶������Ϣ�Ľṹ
 */
typedef struct {
    VOS_UINT16                       timeSlot;                             /* �ϱ������ж�ʱ��ʱ϶�� */
    VOS_UINT16                       reserved;                             /* 4�ֽڶ��� */
    VOS_UINT32                       meaFn;                                /* �ϱ������ж�ʱ��֡�� */
    VOS_UINT16                       meaArfcn;                             /* �ϱ�������Ƶ�� */
    GPHY_TOOL_TimeslotMeasTypeUint16 tsMeasType;                           /* �������� */
    VOS_UINT16                       nextAgcGain[GPHY_MNTN_PATH_BUTT];     /* AGC��λ */
    VOS_INT16                        asAgcRssi125dbm[GPHY_MNTN_PATH_BUTT]; /* ����ֵ:0.125dB */
    VOS_UINT32                drssi125dbm[GPHY_MNTN_PATH_BUTT]; /* DRSSI���������ǰ��ֵ�ϱ� 0.125dbm -0x1000Ϊ��ֵ */
    VOS_UINT32                lsDcValue[GPHY_MNTN_PATH_BUTT];
    VOS_UINT32                grossCarrierEn;
    VOS_UINT32                grossInterfereEn;
    VOS_UINT32                valppCarrierEn;
    VOS_UINT32                valppInterfereEn;
    VOS_UINT16                nbPosAdj;
    VOS_UINT16                toaSelResult;
    GPHY_TOOL_RxIqImbalance   rxIqImbalance;
    GPHY_TOOL_AgcBlkNoblkMode agcBlkNoBlk[GPHY_MNTN_PATH_BUTT];
    VOS_UINT32                gspLineState;          /* �����߿�ʵ��״̬ */
    VOS_UINT32                extLineState;          /* EXT�����߿�ʵ��״̬ */
    VOS_UINT32                crossMipiLineState;    /* Cross Mipi�߿�������״̬ */
    VOS_UINT32                extCrossMipiLineState; /* EXT Cross Mipi�߿�������״̬ */
} GPHY_TOOL_RcvBurstInfo;

/*
 * �ṹ˵��: CTUͨ��ģʽ��Ϣ
 */
typedef struct {
    VOS_UINT32 chnMasterModeRx; /* Channel master mode */
    VOS_UINT32 chnMasterModeTx; /* Channel master mode */
} GPHY_TOOL_CtuModeInfo;

/*
 * �ṹ˵��: ��֡������Ϣ�Ľṹ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT32                curFn;
    VOS_UINT32                bbpcommGsChSwAddr;      /* BBPCOMM_GS_CH_SW_ADDR  */
    VOS_UINT32                bbpcommPmuChSel0Addr;   /* BBPCOMM_PMU_CH_SEL0_ADDR */
    VOS_UINT32                bbpcommDbgReg20CpuAddr; /* BBPCOMM_DBG_REG20_CPU_ADDR */
    GPHY_TOOL_CtuModeInfo     ctuModeInfo;            /* CTUͨ��ģʽ��Ϣ */
    VOS_UINT16                vaildBurstNum;          /* �ϱ�����Чʱ϶������Ϊ��ƥ�䶯̬�ϱ��Զ����� */
    VOS_UINT16                reserved;               /* ����λ */
    GPHY_TOOL_RcvBurstInfo    burstInfo[GPHY_TOOL_TN_NUM_IN_FRAME];
} GPHY_TOOL_RcvSingalInfoReq;

/*
 * �ṹ˵��: Gģ����ʱ�ϱ��ֹ�Ԥ�����ֹ�ʱ�ͳ���ʱ��FN��QB��Ϣ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;           /* �����ֶ� */
    VOS_UINT32                fn[GPHY_GAP_BUTT]; /* ֡�� */
    VOS_UINT16                qb[GPHY_GAP_BUTT]; /* QBֵ */
    VOS_UINT16                reserved1;         /* �����ֶ� */
} GPHY_TOOL_GapOutTimeInfo;

/*
 * �ṹ˵��: GPHY��Ϣ,�澯,error�ϱ��ṹ
 */

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;   /* �����ֶ� */
    VOS_UINT16                fileNo;    /* �ļ������� */
    VOS_UINT16                lineNo;    /* ������к� */
    VOS_UINT32                timeSlice;
    VOS_UINT32                logId;     /* PHY�����ͳһ���LOG ID */
    VOS_UINT16                reserved;  /* �����ֶ� */
    VOS_UINT16                varCnt;    /* �ϱ��������� */
    VOS_UINT32                varValue[GPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* �ϱ�����ֵ */
} GPHY_TOOL_ReportErrorInd;

/*
 * �ṹ˵��: ����֡������Ϣ�Ľṹ
 */
typedef struct {
    VOS_UINT32                        taskFn;
    GPHY_NCE_PredictTaskTypeElfUint16 taskType;
    VOS_UINT16                        freqNum;                               /* ר��̬IDLE֡�����������Ƶ����Ŀ */
    GPHY_NCE_OccasionUseModeElfUint16 occasionUseMode;                       /* ��ǰ���ĸ�ģʽ��ʹ�� */
    VOS_UINT16                        freqInfo[GPHY_TOOL_MAX_SYNC_TASK_NUM]; /* ר��̬IDLE֡��������5��BSIC��֤ */
} GPHY_TOOL_DchOccasionUseInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId; /* �����ֶ� */
    VOS_UINT32                   curFn;
    VOS_UINT16                   timeOutCnt;
    VOS_UINT16                   occasionNum;
    GPHY_TOOL_DchOccasionUseInfo occasionUseInfo[GPHY_TOOL_MAX_IDLE_FRAME_NUM];
} GPHY_TOOL_DchOccasionUseRpt;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16              msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                             toolsId; /* �����ֶ� */
    GPHY_NCE_NcellBsicSearchStateElfUint16 bsicSrchState;
    VOS_UINT16                             freq;
    VOS_UINT16                             rank;
    GPHY_NCE_NstrongNcellElfUint16         nStrongIndex;
    VOS_INT16                              rxlev125dbm;
    VOS_UINT16                             reserved;
} GPHY_TOOL_BsicSearchInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16              msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                             toolsId; /* �����ֶ� */
    VOS_UINT32                             curFn;
    GPHY_NCE_NcellBsicVerifyStateElfUint16 bsicVerifyState;
    VOS_UINT16                             freq;
    VOS_UINT16                             rank;
    GPHY_NCE_NstrongNcellElfUint16         nStrongIndex;
    VOS_INT16                              rxlev125dbm;
    VOS_INT16                              dcOffsetI;
    VOS_UINT16                             agcGain[GPHY_MNTN_PATH_BUTT];
    VOS_UINT32                             snr;
    VOS_INT16                              dcOffsetQ;
    VOS_UINT16                             bsic;
    VOS_UINT16                             ncc;
    VOS_UINT16                             bcc;
    VOS_UINT16                             blockMode[GPHY_MNTN_PATH_BUTT];
} GPHY_TOOL_BsicVerifyInfo;

/*
 * �ṹ˵��: GSM �������ȼ���Ϣ
 */
typedef struct {
    VOS_UINT16 baListIndex;
    VOS_UINT16 arfcn;
    VOS_UINT16 band;
    VOS_UINT16 rank;
    VOS_INT16  rxlev125dbm;
    VOS_UINT16 nStrongIndex;
    VOS_UINT16 bsicKnownFlag;
    VOS_UINT16 bsic;
    VOS_UINT16 ncc;
    VOS_UINT16 bcc;
    VOS_UINT16 inMRFlag;
    VOS_UINT16 timeAlignmt;
    VOS_UINT32 fnOffset;
} GPHY_TOOL_NcellRankInfo;

/*
 * �ṹ˵��: ��ϵͳ���ȼ���Ϣ
 */
typedef struct {
    VOS_UINT16 rank;
    VOS_UINT16 maxGapNum;      /* ���ɷ���Ĺ�����Ŀ */
    VOS_UINT16 assignedGapNum; /* �ѷ���Ĺ�����Ŀ */
    VOS_UINT16 resv;
} GPHY_TOOL_InterratRankInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId; /* �����ֶ� */
    GPHY_TOOL_InterratRankInfo interratRankInfo;
    GPHY_TOOL_NcellRankInfo    ncellRankInfo[GPHY_TOOL_MAX_NCELL_NUM];
} GPHY_TOOL_NcellInterratRankRpt;

/*
 * �ṹ˵��: �ڻ�ȡ����BBP��Դ����Ȩʱ���ϱ��ṹ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;   /* �����ֶ� */
    VOS_UINT16                modemId;   /* ��ǰ����ģʽ */
    VOS_UINT16                resv;      /* ����λ */
    VOS_UINT32                timeSlice; /* ��ǰʱ��� */
} GPHY_TOOL_BbpshareResSelectRpt;

/*
 * �ṹ˵��: ��BBP������Դ����ʱ�ϱ��ṹ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16      msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId;     /* �����ֶ� */
    VOS_UINT16                     modemId;     /* ��ǰ����ģʽ */
    GPHY_TOOL_BbpOperateTypeUint16 operateType; /* �������� */
    VOS_UINT32                     timeSlice;   /* ��ǰʱ��� */
    VOS_UINT32                     gbbpAddr;    /* �Ĵ�����ַ */
    VOS_UINT32                     value;       /* �Ĵ�����ֵ */
} GPHY_TOOL_BbpshareRdWrError;

/*
 * �ṹ˵��: ��ѡʱc1��c2��ʱ����ͣ״̬��ѯ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16       msgId;           /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                      toolsId;         /* �����ֶ� */
    GPHY_TOOL_NcellTimerIdUint16    ncellTimerId;    /* ��ʱ������: c1��c2 */
    GPHY_TOOL_NcellTimerStateUnit16 ncellTimerState; /* ��ʱ��״̬: ����ͣ */
    VOS_UINT32                      arfcn;           /* ��ʱ�����ڵ�Ƶ�㡣ֻ��������ֹͣһ��c2��ʱ��ʱ��Ч */
    VOS_UINT32                      countFnNum;      /* ���䶨ʱ����Ҫ��������֡��c2��ʱ��ȫ��ֹͣʱ��Ч */
    VOS_UINT32                      startFn;         /* ���䶨ʱ������ʼ֡�š�c2��ʱ��ȫ��ֹͣʱ��Ч */
    VOS_UINT32                      passFnCnt;       /* ����ʼ֡�ſ�ʼ,�Ѿ������˶���֡��c2��ʱ��ȫ��ֹͣʱ��Ч */
} GPHY_TOOL_NcellReselTimerState;

/*
 * �ṹ˵��: ��ѡʱc1��c2��ʱ����ͣ״̬��ѯ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16    msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;      /* �����ֶ� */
    GPHY_TOOL_NcellTimerIdUint16 ncellTimerId; /* ��ʱ������: c1��c2 */
    VOS_UINT16                   rsv;          /* ���� */
} GPHY_TOOL_NcellReselTimerExpired;

typedef struct {
    VOS_UINT32 curFn;
    VOS_UINT16 curFnT2;
    VOS_UINT16 predictTask;
} GPHY_TOOL_NcellRecordPredictTask;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16        msgId;           /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                       toolsId;         /* �����ֶ� */
    VOS_UINT16                       ncellBcchOccupy; /* �Ƿ�������ϵͳ�㲥��Ϣռ�� */
    VOS_UINT16                       srchRank;        /* ����Ƶ���������ȼ� */
    VOS_UINT16                       interRatRank;    /* ��ϵͳƵ���������ȼ� */
    VOS_UINT16                       verifyRank;      /* ��֤Ƶ���������ȼ� */
    GPHY_TOOL_NcellRecordPredictTask recordPredictTask[GPHY_TOOL_NCELL_RPT_PREDICT_TASK_INFO_NUM];
} GPHY_TOOL_NcellPredictTaskRpt;

/*
 * �ṹ˵��: ������֤������������������Ϣ������ά��
 */
typedef struct {
    VOS_UINT16                        freqInfo;        /* Ƶ����Ϣ */
    VOS_UINT16                        taskRemainCount; /* ��ǰƵ��������ڵ�ʱ��(Ѱ�����ڵĸ���) */
    GPHY_NCE_NcellSyncActionElfUint16 ncellTaskType;   /* ��������������������֤���� */
    VOS_UINT16                        gtcConfigState;  /* GTC�������õ�״̬ */

    VOS_UINT32 fnOffset;    /* ��֤--�����ͷ�������֡��ƫ��,ģ2715648 */
    VOS_UINT16 timeAlignmt; /* ��֤--�����ͷ�������֡ͷƫ��,ģ5000 */
    VOS_UINT16 reserved;    /* ����λ */

    VOS_UINT16 bsicSrchReqOrgin; /* ����--����BSIC����������Դ: Э��ջ�����������ڲ����� */
    VOS_UINT16 psSrchType;       /* ����--Э��ջ�����bsic��������: �����ѻ����� */
    VOS_UINT32 initCfgNco;       /* ����--Nco ���ó�ʼֵ */
    VOS_UINT32 nceSycNcoValue;   /* ����--����Ƶƫ����� */
} GPHY_TOOL_VerifySrchTaskInfo;

/*
 * �ṹ˵��: �ϱ������ߵ���������ṹ��
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16    msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;        /* �����ֶ� */
    VOS_UINT16                   ncellVerifyNum; /* ����֤�����С������ */
    VOS_UINT16                   ncellSrchNum;   /* �����������С������ */
    VOS_UINT16                   lastLockedFreq; /* �洢ǰһ�α��������Ƶ�� */
    VOS_UINT16                   reserved;       /* ����λ */
    GPHY_TOOL_VerifySrchTaskInfo rptNcellTaskArray[GPHY_TOOL_MAX_NCELL_NUM]; /* �ϱ���ncell�����б� */
} GPHY_TOOL_NcellTaskListIndRpt;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;      /* �����ֶ� */
    VOS_UINT16                dummyPchFlag; /* ��֡��Ѱ���ļ���� */
    VOS_UINT16                averageValue; /* ���ݾ���ֵ��ͺ��ֵȡƽ��(�����㷨��·������16��) */
    VOS_UINT32                fn;           /* ��ǰ֡�� */
    VOS_INT32                 judgeDataC;   /* ��������м�ֵ */
    VOS_INT32                 judgeDataD;   /* ��������м�ֵ */
    VOS_INT32                 judgeDataE;   /* ��������м�ֵ */
    VOS_INT32                 judgeDataF;   /* ��������м�ֵ */
} GPHY_TOOL_ChnDummyPchRpt;

/*
 * �ṹ˵��: SRVCC BSICС����Ϣ
 */
typedef struct {
    VOS_UINT16 arfcn;             /* Ƶ�� */
    VOS_UINT16 rssiOrder;         /* ��ǰС��������������,��LRRC�������б��е���������. */
    VOS_UINT16 initSrchFailCnt;   /* �����ڳ�ʼȷ��ʧ�ܴ���. */
    VOS_UINT16 rrcReCfgFlag;      /* ʧ��С�����LRRC���� */
    VOS_UINT16 sbVerifiedFlag;    /* ��SB��ȷ��������,С���Ƿ��Ѿ������֤��־ */
    VOS_UINT16 sbVerifyFailCount; /* ��ȷ��С����ȷ��ʧ�ܴ��� */
    VOS_UINT16 bsicSucOrFailFlag; /* ͬ���Ƿ�ɹ� */
    VOS_UINT16 bsicCellType;      /* ͬ������ */
    VOS_UINT32 fbSearchLength;    /* FB�����ܳ��� */
    VOS_UINT32 gSlaveOffsetFn;    /* ��һ������ʱ�õ���W/L/TD��Gƫ���Fn */
    VOS_UINT16 gSlaveOffsetQb;    /* ��һ������ʱ�õ���W/L/TD��Gƫ���Qb */
    VOS_UINT16 ncellToaOffset;    /* �����Timebase��ƫ��,��G FB����ʱ�ṩ��ֵ,��SB����ʱʹ�� */
    VOS_UINT32 ncellFrameNum;     /* GSM ��С����֡����Ϣ������ʹ�õ�16bitTDMA֡�ŵ�λ0~26x51-1ѭ������ */
} GPHY_TOOL_SrvccBsicRsltInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;                   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                 /* �����ֶ� */
    VOS_UINT16                gSlaveGapTask;           /* һ������:��ǰGAP�������������. */
    VOS_UINT16                gSlaveCfgType;           /* ��������:��ǰ��Ҫ���õ�����. */
    VOS_UINT16                gRssiPeriodStartFlag;    /* GSM��ģ��ʼ��һ�ֲ���,������ɺ����. */
    VOS_UINT16                srVccFbSrchIndex;   /* ִ�г�ʼȷ�ϵ�����,�����һ����������Ϊ��Ч.���·�����ʱ��Ч. */
    VOS_UINT16                periodFbCnt;        /* ������GAP����������FB���� */
    VOS_UINT16                periodGapFlag;      /* ������GAP����������FB���� */
    VOS_UINT16                srVccSbCellIndex;   /* ִ��SB����С��������,���һ��С����SB��֤��,����Ϊ��Чֵ */
    VOS_UINT16                curBsicTaskType;    /* ��ǰ���������͵���������. */
    VOS_UINT16                sbVerifyPeriorFlag; /* SB��֤���ȼ��Ƿ�� */
    VOS_UINT16                offLineSaveIndex;   /* �������뵱ǰ�洢������. */
    VOS_UINT16 offLineValidIndex; /* ��Ч������Index,�洢��������һ��,����245ms��������Ϊ��Ч,�����ڼ���. */
    VOS_UINT16 bsicNum;    /* ԭ���·���С����������ʼȷ����ȷ�϶���ͬһ�����У�BSICС������������ʼȷ�Ϻ���ȷ�� */
    VOS_UINT16 calFreqNum; /* ����RSSI����С������ */
    VOS_UINT16 freqIndex;  /* RSSIС������ */
    VOS_UINT32 sbVerifyFbSrchTotalTime; /* ��SB��֤�ڼ�,����ִ��FB SRCH,�ڼ���SB��֤ʱ�䳬ʱʱ,��Ҫ�۳�SB SRCH��ʱ��, */
    VOS_UINT32 sbVerifyFbSrchStartTime; /* ��SB��֤�ڼ�,����ִ��FB SRCH,FB SRCH��ʼʱ���. */
    VOS_UINT32 sbPeriodStartSlice;      /* SB��֤���ڵ���ʼʱ���. */

    GPHY_TOOL_SrvccBsicRsltInfo cellBsicInfo[GPHY_TOOL_SLAVE_BSIC_MAX_NUM];
} GPHY_TOOL_SrvccScheduleInfo;

/*
 * 6.2 STRUCT����,WPHY����
 */
/*
 * �ṹ˵��: W TX PA�����л�ʱ���ʲ���ֵ
 */
typedef struct {
    VOS_INT16 h2MComp;  /* PA����Ӹ��е��е����޲���,��λ0.1db */
    VOS_INT16 m2HComp;  /* PA��������е��ߵ����޲���,��λ0.1db */
    VOS_INT16 m2LComp;  /* PA��������е��͵����޲���,��λ0.1db */
    VOS_INT16 l2MComp;  /* PA����ӵ��е��е����޲���,��λ0.1db */
    VOS_INT16 l2UlComp; /* PA����ӵ��е��������޲���,��λ0.1db */
    VOS_INT16 uL2LComp; /* PA����ӳ����е��͵����޲���,��λ0.1db */
} WPHY_TOOL_PaGainSwitchCompInfo;

/*
 * �ṹ˵��: W TX 4��ʱ϶��PA�����л�ʱ���ʲ���ֵ
 */
typedef struct {
    WPHY_TOOL_PaGainSwitchCompInfo slotComp[WPHY_TOOL_PA_SWITCH_COMP_SLOT];
} WPHY_TOOL_TxPaGainSwitchComp;

/*
 * �ṹ˵��: PA���л�����ԭ��
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* Msg ID */
    VOS_UINT16 rsv1;  /* ���� */

    WPHY_TOOL_TxPaGainSwitchComp paGainSwitchComp;
} WPHY_TOOL_PaGainSwitchCompReq;

/*
 * �ṹ˵��: PA���л�����ԭ��Ľ��
 */
typedef struct {
    VOS_INT16 h2M;  /* �ߵ������油�� */
    VOS_INT16 m2H;  /* �е������油�� */
    VOS_INT16 m2L;  /* �е������油�� */
    VOS_INT16 l2M;  /* �͵������油�� */
    VOS_INT16 l2Ul; /* �͵��������油�� */
    VOS_INT16 uL2L; /* ���͵������油�� */
} WPHY_TOOL_SigPaGainSwitchCompInd;

/*
 * �ṹ˵��: PA���л�����ԭ��Ļظ�
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16                       transId;               /* ԭ��ID,�̶�ֵ:0x5001 */
    VOS_UINT16                       reserved1;             /* �����ֶ� */
    VOS_UINT16                       funcType;              /* �̶�ֵ:0x04 */
    VOS_UINT16                       length;                /* ��uwSn����β�ĳ��� */
    VOS_UINT32                       sn;                    /* �̶�ֵ:0 */
    VOS_UINT32                       timeStamp;             /* �ϱ�CNF����SFN */
    VOS_UINT16                       primId;                /* WDSP_MNTN_MSG_ID_ENUM_UINT16,��ϢID  */
    VOS_UINT16                       reserved2;
    VOS_UINT32                       result;                /* ��Ϣ������ʼ��ַ */
    WPHY_TOOL_SigPaGainSwitchCompInd rfPaGainSwitchCompInd; /* ��ǰ���油���ϱ� */
} WPHY_TOOL_PaGainSwitchCompCnf;

/*
 * �ṹ˵��: �����ǲ���ͨ��SDTʵʱ�򿪹ر�PD�ĺ���������ԭ��
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;   /* Msg ID */
    VOS_UINT16 rsv1;    /* ���� */
    VOS_UINT32 pdEnFlg; /* PDʹ�ܱ�ǣ�0:ȥʹ�ܣ�1:ʹ�� */
} WPHY_TOOL_PowDetEnableReq;

/*
 * �ṹ˵��: �����ǲ���ͨ��LMTʵʱ�򿪹ر�PD�ĺ����Ļظ�ԭ��
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 transId;   /* ԭ��ID,�̶�ֵ:0x5001 */
    VOS_UINT16 reserved1; /* �����ֶ� */
    VOS_UINT16 funcType;  /* �̶�ֵ:0x04 */
    VOS_UINT16 length;    /* ��ulSn����β�ĳ��� */
    VOS_UINT32 sn;        /* �̶�ֵ:0 */
    VOS_UINT32 timeStamp; /* �ϱ�CNF����SFN */
    VOS_UINT16 primId;    /* WDSP_MNTN_MSG_ID_ENUM_UINT16����ϢID  */
    VOS_UINT16 reserved2;
    VOS_UINT32 errCode;   /* ��� */
} WPHY_TOOL_PowDetEnableCnf;

/*
 * �ṹ˵��: WPHY��ά�ɲ⹫���ظ���Ϣ�ṹ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRsltUint16  result;  /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                rsv;     /* �����ֶ� */
} WPHY_TOOL_CommonCnf;

/*
 * �ṹ˵��: ���С����Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnReportTypeUint16 reportType;
    VOS_UINT16                     reportPeriod; /* Ĭ��0����ʶ��Ч�����ϱ�����Ϊָ������ʱ����IE��Ч�� */
} WPHY_MNTN_ReportType;

typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */                /* ԭ������ */
    VOS_UINT16           toolsId;    /* �����ֶ� */
    WPHY_MNTN_ReportType reportType; /* Ĭ��ÿ֡�ϱ�  */
} WPHY_TOOL_ActCellReq;

/*
 * �ṹ˵��: ���С����Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_UINT16                scramble[WPHY_MNTN_MAX_ACT_CELL_NUM]; /* ���С�������� */
    VOS_UINT16                servingCellScramble;                  /* ��С������ */
} WPHY_TOOL_ActCellInfoInd;

/*
 * �ṹ˵��: С�������б���Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16           toolsId;    /* �����ֶ� */
    WPHY_MNTN_ReportType reportType; /* Ĭ���¼������ϱ���ÿ��С�������ϱ�  */
} WPHY_TOOL_CellSearchListReq;

/*
 * �ṹ˵��: С�������б���Ϣ
 */
typedef struct {
    VOS_UINT16 scramble;
    VOS_UINT16 energy;
    VOS_UINT32 phase;     /* ��Ҫת��ΪChip���� */
    VOS_UINT16 lifeCycle; /* �������� */
    VOS_UINT16 rsv;
} WPHY_MNTN_CsListCellInfo;

typedef struct {
    VOS_UINT16               freq;
    VOS_UINT16               cnt;
    WPHY_MNTN_CsListCellInfo cellInfo[32];
} WPHY_MNTN_CsListInfo;

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_UINT16                freqCnt;
    VOS_UINT16                rsv;
    WPHY_MNTN_CsListInfo      csList[4];
} WPHY_TOOL_CellSearchListInd;

/*
 * �ṹ˵��: ���ع��ܿ��ƣ�ָ���򿪻�ر����µĹ��ع���:
 *             1.���й��ؿ���(����˿��عرգ����ʶ���еĹ��ع���ȫ��ʧЧ,
 *                �����ڻ����أ��⻷���أ�Winup,Windown�ȣ�
 *                ��ʱ������ͨ���������ò�������������TPC,SirTarget�ȡ�)
 *             2.�⻷����(����˹��ܹرգ�����ʶ�⻷���ܹرգ���ʱ����ͨ��
 *                ���ʲ������ù���������SirTarget��)
 *             3.Windown
 *             4.Winup
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16         msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnPwrSwitchTypeUint16 pCType;
    WPHY_TOOL_MntnSwitchCtrlUint16    status;
} WPHY_TOOL_PwrSwitchCtrlReq;

/*
 * �ṹ˵��: ���ʲ������ã���Mask��ʶ��Ч���
 *             1.SirTarget
 *             2.��ʼ���书��
 *             3.Hold���з��书��(���������С���书��)
 *             4.����ָ�����͵�����TPC.
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16   msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnPwrParaUint32 pwrParaMask;
    VOS_UINT32                  sirTarget; /* SirTarget */
    VOS_INT16                   initTxPwr; /* ��ʼ���� */
    VOS_INT16                   ulTpc;     /* ����TPC */
    VOS_INT16                   maxTxPwr;  /* ����书�� */
    VOS_INT16                   minTxPwr;  /* ��С���书�� */
} WPHY_TOOL_PwrParaSetReq;

/*
 * �ṹ˵��: ������ϱ�
 */
typedef struct {
    VOS_MSG_HEADER                            /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;    /* �����ֶ� */
    WPHY_MNTN_ReportType          reportType; /* Ĭ��ÿ֡�ϱ�  */
} WPHY_TOOL_BlerReportReq;

/*
 * �ṹ˵��: ������ϱ�����
 *           ����� = ulErrorBlocks/ulTotalBlocks
 */
typedef struct {
    VOS_UINT16 trchId;
    VOS_UINT16 tti;         /* ÿ�������ŵ���TTI */
    VOS_UINT32 errorBlocks; /* �����  */
    VOS_UINT32 totalBlocks; /* �ܿ��� */
} WPHY_MNTN_TrchBler;

/*
 * �ṹ˵��: ������ϱ�����
 *           ÿ���ϱ��Ĵ����ŵ��������̶����������ٸ���CNTȷ����
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_UINT16                cnt;
    VOS_UINT16                rsv;
    WPHY_MNTN_TrchBler        trchBler[WPHY_MNTN_TRCH_NUM];
} WPHY_TOOL_BlerReportInd;

/*
 * �ṹ˵��: �¼��ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnEventTypeUint32 eventTypeMask;
} WPHY_TOOL_EventReportReq;

/*
 * �ṹ˵��: W��ͬ��ʧ��״̬�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId; /* �����ֶ� */
    VOS_UINT16                 sfn;
    VOS_UINT16                 cfn;
    WPHY_TOOL_SyncStatusUint16 syncStatus;
    VOS_UINT16                 rsv;
} WPHY_TOOL_SyncStatusInd;

/*
 * �ṹ˵��: AFC����
 * EVENT_VCTCXO_FREEZE ,EVENT_VCTCXO_UNFREEZE
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    UPHY_TOOL_XoStatusUint16  tcxoStatus;
    VOS_UINT16                rsv;
    VOS_UINT32                afcStatusValue;
} WPHY_TOOL_AfcStatusInd;

/*
 * �ṹ˵��: �ж��쳣�¼��ϱ���
 *             1.��6ms�������ʱ����ȡ9ms�����Ѿ����
 *             2.��9ms�������ʱ����ȡSFN�жϸ������Ƿ��Ѿ���֡��
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;

    WPHY_TOOL_MntnIntExceptUint16 intExceptType; /* �ж��쳣���� */
    VOS_UINT16                    rsv;
} WPHY_TOOL_InterruptExceptiionInd;

/*
 * �ṹ˵��: �ϱ�L1״̬
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;

    VOS_UINT32                    channelStatus; /* �ŵ�״̬ */
    WPHY_TOOL_MntnSysStatusUint16 sysStatus;     /* ϵͳ״̬ */
    VOS_UINT16                    rsv;
} WPHY_TOOL_WcdmaL1StatusInd;

/*
 * �ṹ˵��: DRX���ܴ򿪺͹ر�����
 */
typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16  msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnSwitchCtrlUint16 drxCtrl;
    VOS_UINT16                     rsv;
} WPHY_TOOL_DrxSwitchReq;

/*
 * �ṹ˵��: ��������
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16     msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnThresholdTypeUint32 thresholdType;
    VOS_INT32                         value;
} WPHY_TOOL_ThresholdSetReq;

/*
 * �ṹ˵��: ��ѯ
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16     msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnThresholdTypeUint32 thresholdType;
} WPHY_TOOL_ThresholdQueryReq;

/*
 * �ṹ˵��: ��ѯ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId; /* �����ֶ� */
    VOS_UINT16                        sfn;
    VOS_UINT16                        cfn;
    WPHY_TOOL_MntnThresholdTypeUint32 thresholdType;
    VOS_INT32                         value;
} WPHY_TOOL_ThresholdQueryInd;

/*
 * �ṹ˵��: �����ϱ�
 */
typedef struct {
    VOS_MSG_HEADER                                   /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16     msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId;       /* �����ֶ� */
    WPHY_TOOL_MntnErrorRptLevelUint16 errorRptLevel; /* �����ϱ����󼶱� */
    VOS_UINT16                        rsv;
} WPHY_TOOL_ErrorReportReq;

/*
 * �ṹ˵��: �����ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                fileNo;  /* �ļ������� */
    VOS_UINT16                lineNo;  /* ������к� */
    VOS_UINT32                timeSlice;
    VOS_UINT32                logId;                                      /* PHY�����ͳһ���LOG ID */
    VOS_UINT16                reserved;                                   /* �����ֶ� */
    VOS_UINT16                varCnt;                                     /* �ϱ��������� */
    VOS_UINT32                varValue[WPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* �ϱ�����ֵ */
} WPHY_TOOL_ErrorReportInd;

/*
 * �ṹ˵��: RFǰ�˲����ϱ�
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    WPHY_MNTN_ReportType reportType; /* Ĭ��ÿ֡�ϱ�  */
} WPHY_TOOL_RfFrontEndParaReq;

/*
 * �ṹ˵��: �ϱ�TX Temp Comp,Freq Comp,AFC,LNA,HDET
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;        /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_INT16                 paHighTempComp; /* PA������ʱ�¶Ȳ��� */
    VOS_INT16                 paMidTempComp;  /* PA������ʱ�¶Ȳ��� */
    VOS_INT16                 paLowTempComp;  /* PA������ʱ�¶Ȳ��� */

    VOS_INT16 paHighFreqComp; /* PA������ʱƵ�ʲ��� */
    VOS_INT16 paMidFreqComp;  /* PA������ʱƵ�ʲ��� */
    VOS_INT16 paLowFreqComp;  /* PA������ʱƵ�ʲ��� */

    VOS_UINT16 afcStatus;     /* AFCֵ */
    VOS_UINT16 ant1AgcStatus; /* ����1��AGC����״̬ */
    VOS_UINT16 ant2AgcStatus; /* ����2��AGC����״̬ */
    VOS_UINT16 hdet;          /* HDETֵ */
    VOS_UINT16 paStatus;      /* PA״̬:�ϱ�����PA��ģʽ������ */
    VOS_UINT16 dbbAtten;      /* DBB˥�� */
    VOS_UINT16 rfAtten;       /* RF˥��,boston��ʹ�� */
    VOS_UINT16 apcAptIndex;   /* APC��APT����ֵ�ϱ���bostonʹ��apc:0-7;apt:8-15 */
} WPHY_TOOL_RfFrontEndParaInd;

/*
 * �ṹ˵��: WPHY��PI�жϽ���ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_INT16                 piResult;
    VOS_UINT16                piSoftValue;
    UPHY_TOOL_WphyDrxMntnInfo wphyDrxMntn; /* DRX��ά�ɲ���Ϣ */
} WPHY_TOOL_PiIntResultInd;

/*
 * �ṹ˵��: WPHY��AI�жϽ���ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;        /* �����ֶ� */
    VOS_INT16                 aiResult;       /* AI���,+1��-1��0 */
    VOS_UINT16                reTranNum;      /* �ش����� */
    VOS_INT16                 initPower;      /* ��ʼ����,��λ 0.1dBm */
    VOS_INT16                 currentPower;   /* ��ǰ����,��λ 0.1dBm */
    VOS_INT16                 maxTxPower;     /* ����书��,��λ0.1dBm */
    VOS_INT16                 maxAiSoftValue; /* AICH��ֵ���ֵ */
} WPHY_TOOL_AiIntResultInd;

/*
 * �ṹ˵��: T313��ʱ��״̬�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16      msgId;      /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId;    /* �����ֶ� */
    WPHY_TOOL_MntnT313StatusUint16 t313Status; /* T313״̬ */
    VOS_UINT16                     rsv;
} WPHY_TOOL_T313StatusInd;

/*
 * �ṹ˵��: T312��ʱ��״̬�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16      msgId;      /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId;    /* �����ֶ� */
    WPHY_TOOL_MntnT312StatusUint16 t312Status; /* T313״̬ */
    VOS_UINT16                     rsv;
} WPHY_TOOL_T312StatusInd;

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT32                reserve; /* for coverity */
} WPHY_TOOL_SlaveCdrxStartInd;

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT32                reserve; /* for coverity */
} WPHY_TOOL_SlaveCdrxStopInd;

/*
 * �ṹ˵��:�ྶ��Ϣ�ṹ
 */
typedef struct {
    VOS_UINT16 pathEnergy; /* ������ */
    VOS_UINT16 pathPos;    /* ����λ */
} WPHY_TOOL_AmpPathInfo;

/*
 * �ṹ˵��: W SLAVE CS��Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16          intraStepbEn;     /* stepb�Ƿ�ʹ�� */
    WPHY_TOOL_MntnBoolUint16          stepbScramProMod; /* stepb�Ƿ������룬0��ʾ������ */
    WPHY_TOOL_MntnBoolUint16          stepbSaveMod;     /* stepb����ģʽ��0��ʾstep1���������������� */
    WPHY_TOOL_MntnSlaveCsStatusUint16 csStatus;         /* С������״̬��־, ��ʾÿ����С���������  */
} WPHY_TOOL_WslaveCsInfo;

/*
 * �ṹ˵��: W SLAVE MEAS FRAME PHASE��Ϣ
 */
typedef struct {
    VOS_UINT16 preFrameSlot;
    VOS_UINT16 preFrameChip;
    VOS_INT32  preSlaveWTimeOffset;
    VOS_INT32  curSlaveWTimeOffset;
    VOS_INT32  umtsSysBaseCntOff;
} WPHY_TOOL_MeasFramePhaseInfo;
/*
 * �ṹ˵��: W SLAVEʱ����ģ��ʱ��Ϣ
 */
typedef struct {
    VOS_UINT32 gsmFnCnt;
    VOS_UINT16 gsmQb;
    VOS_UINT16 lteSfn;
    VOS_UINT16 ltesubfrm;
    VOS_UINT16 ltetscnt;
    VOS_UINT16 umtsSlot;
    VOS_UINT16 umtsChip;
} WPHY_TOOL_UmtsLteGsmTimingInfoStru;
/*
 * �ṹ˵��: W SLAVE MEAS��Ϣ
 */
typedef struct {
    VOS_UINT16                         measCellNum;
    VOS_UINT16                         measCellIndex;
    VOS_UINT16                         mpPurpose; /* ��ǰ�ྶĿ�� */
    VOS_UINT16                         reserved;
    VOS_UINT32                         measStatus;
    VOS_UINT32                         measCsStatus;
    VOS_UINT32                         unitBitMap;          /* �ྶ��bitmap */
    VOS_UINT32                         mpResourceMap;       /* ��ǰ�ྶ��Դ��� */
    WPHY_TOOL_AmpPathInfo              unitFirstFingerInfo; /* Strongest finger info */
    VOS_UINT32                         int0ConfigTime;      /* �̹���Ԥ��������ʱ�䣬������Ч */
    WPHY_TOOL_MeasFramePhaseInfo       measFramePhaseInfo;
    WPHY_TOOL_UmtsLteGsmTimingInfoStru masterTimingInfo;
} WPHY_TOOL_WslaveMeasInfo;

/*
 * �ṹ˵��: ��ʱ���µ�ǰppm��scpll����ֵ���¶�ֵ��AFCֵ
 */
typedef struct {
    VOS_INT32  ppmValue;         /* ppm */
    VOS_UINT32 pllFracValue;     /* Scpll С�����ֵ���ֵ */
    VOS_UINT32 pllInterValue;    /* Scpll �������ֵ���ֵ */
    VOS_UINT32 afcValue4518;     /* AFC 4518 */
    VOS_INT16  temperatureValue; /* �¶� */
    VOS_UINT16 reserved;
} WPHY_TOOL_AfcInfo;

/*
 * �ṹ˵��: W SLAVE ǰ����Ϣ
 */
typedef struct {
    VOS_UINT16        currFreq;        /* CurrFreq */
    VOS_UINT16        currBand;        /* CurrBand */
    WPHY_TOOL_AfcInfo afcInfo;         /* ��ʱ���µ�ǰppm��scpll����ֵ���¶�ֵ��AFCֵ */
    VOS_UINT32        feRssi;          /* RSSI 1414 */
    VOS_UINT32        feRssiDiv;       /* RSSI 1418 */
    VOS_UINT32        dcOffset0[8];    /* DC OFFSET 0 */
    VOS_UINT32        chnMasterModeRx; /* Channel master mode */
    VOS_UINT32        chnMasterModeTx; /* Channel master mode */
    VOS_UINT32        chnSlaveMode;    /* Channel slave mode */
    VOS_UINT32        chnSlaveChn[8];
    VOS_UINT32        ssiEn[2];         /* SSI EN */
    VOS_UINT32        rfPllStatus;      /* rficֻ�ڲ������ʱ���� */
    VOS_UINT32        crossMipiLineRpt; /* CrossMIpi�߿���Ϣ */
    VOS_UINT32        crossLnaLineRpt;  /* CrossLNa�߿���Ϣ */
    VOS_UINT16        tasAntStatusInfo; /* TasAntStatus */
    VOS_UINT16        wtcMode;          /* WTCģʽ */
    VOS_UINT16        shareSel3gp;      /* 3GP SHARE SEL */
    VOS_UINT16        reserved;         /* �������� */
#if (FEATURE_ON == FEATURE_EASYRF)
    VOS_UINT32 clkGatingValue; /* B5000 ���õ�BBP ʱ���ſ�ֵ */
#endif
} WPHY_TOOL_WslaveFeInfo;

/*
 * �ṹ˵��: W SLAVE BBP enter exit gap ��Ϣ
 */
typedef struct {
    VOS_UINT32 from3F34To3F40[4]; /* enter exit gap, measure status */
    VOS_UINT32 materMeasWen;      /*  10E8 */
    VOS_UINT32 mipiSsiBusy;       /*  F80A34 */
} WPHY_TOOL_WbbpGapInfo;

#if (FEATURE_ON == FEATURE_EASYRF)
/*
 * �ṹ˵��: WBBP INTRA/INTER״̬��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    VOS_UINT32 intraInterStatus;  /* ����Ƶ״̬�� */
    /* Intra/Inter Pos��Ч��ʽ,Ϊ0,��ʾ����Ƶpose��Ҫ����֡ͷ����ȥ�������Ч,Ϊ1��ʾ,����pose��Ϳ�����Ч */
    VOS_UINT32 posValidType;
    VOS_UINT32 exitIntraPos;      /* �˳���Ƶ״̬ʱ�� */
    VOS_UINT32 exitNonIntraPos;   /* �˳��Ǳ�Ƶ״̬ʱ�� */
    VOS_UINT32 backToIntraPos;    /* �ص���Ƶ״̬ʱ�� */
    VOS_UINT32 enterInterPos;     /* ������Ƶ״̬ʱ�� */
    VOS_UINT32 exitInterPos;      /* �˳���Ƶ״̬ʱ�� */
    VOS_UINT32 backToNonInterPos; /* �ص�����Ƶ״̬ʱ�� */
} WPHY_TOOL_WbbpIntraInterStatusInfo;
#endif

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16              msgId;                /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                             toolId;               /* tools id */
    WPHY_TOOL_MntnSlaveCurrentStatusUint16 currentStatus;
    VOS_UINT16                             currentFreqIndex;
    WPHY_TOOL_TimingInfo                   wslaveTimingInfo;     /* ��ʱ�ϱ���Ϣ(����Я��SFN CFN SLOT SLICE��Ϣ) */
    WPHY_TOOL_WslaveFeInfo                 wslaveFeInfo;         /* ǰ����Ϣ */
    WPHY_TOOL_WslaveMeasInfo               wslaveMeasInfo;       /* WslaveMeasInfo */
    WPHY_TOOL_WslaveCsInfo                 wslaveCsInfo;         /* WslaveCSInfo */
    WPHY_TOOL_WbbpGapInfo                  wbbpEnterExitGapInfo; /* EnterExitGapInfo */
#if (FEATURE_ON == FEATURE_EASYRF)
    WPHY_TOOL_WbbpIntraInterStatusInfo wbbpIntraInterStatus; /* BBP����Ƶ״̬��Ϣ */
#endif
} WPHY_TOOL_SlaveMntnRptInd;

/*
 * �ṹ˵��: PA״̬�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */

    WPHY_MNTN_ReportType reportType; /* Ĭ��ÿ֡�ϱ�  */
} WPHY_TOOL_PaStatusReportReq;

/*
 * �ṹ˵��: ����LTE��ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                sfn;        /* ����ʱ�̵�sfn */
    VOS_UINT16                slot;       /* ����ʱ�̵�slot */
    VOS_UINT32                wakeReason; /* ����Ŀ�� */
} WPHY_TOOL_MasterWakeSlaveInd;

/*
 * �ṹ˵��: ����LTE������ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;      /* �����ֶ� */
    VOS_UINT16                sfn;          /* ����ʱ�̵�sfn */
    VOS_UINT16                slot;         /* ����ʱ�̵�slot */
    VOS_UINT32                startReason;  /* ����ԭ�� */
    VOS_UINT32                measTime;     /* ����ʱ�� us */
    VOS_UINT32                int0Int1Time; /* int0 int1��� us */
    VOS_UINT32                afcLockFlag;
    VOS_INT32                 afcValue;
} WPHY_TOOL_MasterStartLteMeasInd;

/*
 * �ṹ˵��: ����LTE������ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;     /* �ϱ�ʱ�̵�sfn */
    VOS_UINT16                slot;    /* �ϱ�ʱ�̵�slot */
} WPHY_TOOL_MasterReportLteMeasInd;

/*
 * �ṹ˵��: ǿͣLTE������ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16                 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnMasterStopSlaveReasonUint16 reason;  /* ǿͣԭ�� */
    VOS_UINT16                                sfn;     /* ǿͣʱ�̵�sfn */
    VOS_UINT16                                slot;    /* ǿͣʱ�̵�slot */
    VOS_UINT16                                rsv;
} WPHY_TOOL_MasterStopSlaveMeasInd;

/*
 * �ṹ˵��: ����LTE������ǵ�ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                value;
    VOS_UINT16                rsv;
} WPHY_TOOL_SetLteMeasFlagInd;

/*
 * �ṹ˵��: ���õ�ʱ��
 */
typedef struct {
    VOS_UINT16 sfn;  /* ����ʱ�̵�sfn */
    VOS_UINT16 cfn;  /* ����ʱ�̵�cfn */
    VOS_UINT16 slot; /* ����ʱ�̵�slot */
    VOS_UINT16 chip; /* ����ʱ�̵�chip */
} WPHY_TOOL_CfgTimeInfo;

/*
 * �ṹ˵��: ���õ�ʱ��
 */
typedef struct {
    WPHY_TOOL_MntnRfOperSceneUint16 rfCfgOpSc;  /* RF���õ�Ŀ�� */
    VOS_UINT16                      antMode;    /* RF����ģʽ */
    VOS_UINT16                      intraBand;  /* ��ƵBAND */
    VOS_UINT16                      intraFreq;  /* ��ƵƵ�� */
    VOS_UINT16                      adjFreq;    /* ��ƵƵ�� */
    VOS_UINT16                      interBand;  /* ��ƵBAND */
    VOS_UINT16                      interFreq;  /* ��ƵƵ�� */
    VOS_UINT16                      rxRficMode; /* RX RFICģʽ */
    VOS_UINT16                      txRficMode; /* TX RFICģʽ */
    VOS_UINT16                      reserve;
} WPHY_TOOL_RfCfgInfo;

/*
 * �ṹ˵��: ����LTE������ǵ�ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    WPHY_TOOL_CfgTimeInfo     cfgTime; /* RF����ʱ�� */
    WPHY_TOOL_RfCfgInfo       cfgInfo; /* RF������Ϣ */
} WPHY_TOOL_MasterRfConfigInd;

/*
 * �ṹ˵��: FET֡������ģ����Ϣ�ṹ��
 */
typedef struct {
    VOS_UINT16 ustartFrameCfn; /* ģ�����ʼ֡��CFN�� */
    /* ֡����ģ��,bit0ָʾ��ʼ֡�Ŀ���,1Ϊ��,0Ϊ��;�Դ�����bitnָ��ʼ֡���n֡,ÿ��bit����һ֡�Ŀ��� */
    VOS_UINT16 patternBitMap;
} WPHY_TOOL_FetFramePatternInfo;

/*
 * �ṹ˵��: FET����Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;        /* �����ֶ� */
    VOS_UINT16                fetEnable;      /* FETʹ�ܿ���,��:UPHY_TRUE,��:UPHY_FALSE */
    VOS_UINT16                currDpchSf;     /* ���浱ǰʹ�õ�DPCH�ŵ� */
    VOS_UINT16                ulTpcAlgorithm; /* ���浱ǰʹ�õĹ����㷨 */
    VOS_UINT16                trchTti;        /* ���������ŵ�TTI */
    VOS_UINT16                fetBlindStatus; /* FET SRB ä��⿪�أ���UPHY_TRUE,�ر�UPHY_FALSE */
    VOS_UINT16 fetSrbCheckStatus; /* FET SRB ������������״̬���Ѿ�����Ϊ:UPHY_TRUE,û����Ϊ:UPHY_FALSE */
    WPHY_TOOL_FetFramePatternInfo framePatternInfo; /* FET֡������ģ����Ϣ */
} WPHY_TOOL_FetOpenInd;

/*
 * �ṹ˵��: FET�ر���Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;   /* �����ֶ� */
    VOS_UINT16                fetStatus; /* FETʹ�ܿ���,��:UPHY_TRUE,��:UPHY_FALSE */
    VOS_UINT16                reserved;
    VOS_UINT32                fetCloseVote; /* FET����ͶƱ,ÿ��bit����1Ʊ,Ϊ1ָʾͶ�ص�Ʊ,Ϊ0ָʾͶ����Ʊ */
    VOS_UINT32 fetRxOpenVote;               /* FET֡�����н��տ���ͶƱ,ÿ��bit����1Ʊ,Ϊ1ָʾͶ����Ʊ,Ϊ0ָʾͶ�ص�Ʊ */
    WPHY_TOOL_FetFramePatternInfo framePatternInfo; /* FET֡������ģ����Ϣ */
} WPHY_TOOL_FetCloseInd;

/*
 * �ṹ˵��: CLT״̬��Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId;  /* �����ֶ� */
    WPHY_TOOL_CltDetectFailTypeUint16 type;
    VOS_UINT16                        reserved; /* �����ֶ� */
} WPHY_TOOL_CltDetectFailRptInd;

/*
 * �ṹ˵��: CLT״̬��Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                 /* �����ֶ� */
    VOS_UINT32                reserved;                /* �����ֶ� */
} WPHY_TOOL_CltStatusRptInd;

/*
 * �ṹ˵��: QPC��������·��ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                sfn;        /* ����ʱ�̵�sfn */
    VOS_UINT16                slot;       /* ����ʱ�̵�slot */
    VOS_UINT32                openReason; /* ��ԭ�� */
} WPHY_TOOL_QpcOpenUlInd;

/*
 * �ṹ˵��: QPC�ر�������·��ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;     /* �����ֶ� */
    VOS_UINT16                sfn;         /* ����ʱ�̵�sfn */
    VOS_UINT16                slot;        /* ����ʱ�̵�slot */
    VOS_UINT32                closeReason; /* �ر�ԭ�� */
} WPHY_TOOL_QpcCloseUlInd;

/*
 * �ṹ˵��: QPC��ǰRRC״̬��ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;  /* �����ֶ� */
    VOS_UINT16                sfn;      /* ����ʱ�̵�sfn */
    VOS_UINT16                slot;     /* ����ʱ�̵�slot */
    VOS_UINT32                rrcState; /* RRC״̬ */
} WPHY_TOOL_QpcRrcStateInd;

/*
 * �ṹ˵��: QPC��ǰҵ�����͵�ָʾ CS/PS
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;     /* ����ʱ�̵�sfn */
    VOS_UINT16                slot;    /* ����ʱ�̵�slot */
    VOS_UINT32                rbType;  /* RB TYPE */
} WPHY_TOOL_QpcRbTypeInd;

/*
 * �ṹ˵��: TAS���л������׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    VOS_UINT16 nextState; /* ��һ˫����״̬ */
    VOS_INT16  rssi;      /* �����ߵ�RSSI,�˲���Ľ��,��λΪ0.125dbm */
    VOS_INT16  rscp;      /* �����ߵ�RSCP,�˲���Ľ��,��λΪ0.125dbm */
    VOS_UINT16 antStatus; /* ��ǰ����״̬,0Ϊ������1Ϊ�ּ� */
} WPHY_TOOL_NonblindEstimateStateInd;

/*
 * �ṹ˵��: TASä�л������׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    VOS_UINT16 nextState;   /* ��һ״̬ */
    VOS_UINT16 rsv;         /* �����ֶ� */
    VOS_INT16  deltaRscp;   /* T1ʱ�����źŴ����м�ֵǰ�������� */
    VOS_INT16  rscpAveCurr; /* ��ǰT1ʱ�䵥�����С������,��λΪ0.125dbm */
    VOS_INT16  rscpAvePre;  /* ǰһ��T1ʱ�䵥�����С������,��λΪ0.125dbm */
    VOS_UINT16 antStatus;   /* ��ǰ����״̬,0Ϊ������1Ϊ�ּ� */
} WPHY_TOOL_BlindEstimateStateInd;

/*
 * �ṹ˵��: TAS�����׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16          msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                         toolsId;      /* �����ֶ� */
    WPHY_TOOL_NonblindEstimateStateInd nonBlindInfo; /* ���л�����״̬��Ϣ */
    WPHY_TOOL_BlindEstimateStateInd    blindInfo;    /* ä�л�����״̬��Ϣ */
} WPHY_TOOL_TasEstimateStateInd;

/*
 * �ṹ˵��: TAS���л��ȽϽ׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    VOS_UINT16 nextState; /* ��һ˫����״̬ */
    VOS_UINT16 phase;     /* �ȽϽ׶���һ�׶� */
    VOS_INT16  deltaRscp; /* ���ּ������˲����RSCP��ֵ,��λΪ0.125dbm */
    VOS_UINT16 antStatus; /* ��ǰ����״̬,0Ϊ������1Ϊ�ּ� */
} WPHY_TOOL_NonblindCompareStateInd;

/*
 * �ṹ˵��: TASä�л��ȽϽ׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    VOS_UINT16 nextState; /* ��һ˫����״̬ */
    VOS_UINT16 rsv;
    VOS_UINT16 phase;       /* �ȽϽ׶���һ�׶� */
    VOS_INT16  rscpAveCurr; /* ���ּ������˲����RSCP��ֵ,��λΪ0.125dbm */
    VOS_INT16  rscpAvePre;  /* ���ּ������˲����RSCP��ֵ,��λΪ0.125dbm */
    VOS_UINT16 antStatus;   /* ��ǰ����״̬,0Ϊ������1Ϊ�ּ� */
} WPHY_TOOL_BlindCompareStateInd;

/*
 * �ṹ˵��: TAS�ȽϽ׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId;      /* �����ֶ� */
    WPHY_TOOL_NonblindCompareStateInd nonBlindInfo; /* ��һ˫����״̬ */
    WPHY_TOOL_BlindCompareStateInd    blindInfo;    /* �ȽϽ׶���һ�׶� */
} WPHY_TOOL_TasCompareStateInd;

/*
 * �ṹ˵��: TAS���ͽ׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;   /* �����ֶ� */
    VOS_UINT16                antStatus; /* ��ǰ����״̬ */
    VOS_UINT16                rsv;       /* ���ֽڶ��� */
} WPHY_TOOL_MasFrozenStateInd;

/*
 * �ṹ˵��: ��ģW�������ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16               msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                              toolsId;        /* �����ֶ� */
    WPHY_TOOL_MntnSlaveMeasFinishFlagUint16 measReportType; /* ����ϱ������� */
    VOS_UINT16                              reportFreqNum;  /* �ϱ�Ƶ��ĸ��� */
    VOS_UINT16                              freq[WPHY_MNTN_SLAVE_REPORTFREQ_NUM];
} WPHY_TOOL_SlaveWFinishInd;

/*
 * �ṹ˵��: ��ģW�������ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT32                rsv;
} WPHY_TOOL_SlaveGrssiFinishInd;

/*
 * �ṹ˵��: ��ģG BSIC�������ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT32                rsv;
} WPHY_TOOL_SlaveGbsicFinishInd;

/*
 * �ṹ˵��: ��ģ���ȳ�ʱָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                startChip;
    VOS_UINT16                startSlot;
    VOS_UINT16                endChip;
    VOS_UINT16                endSlot;
    VOS_UINT16                scheduleTime; /* ����ʱ�䣬��λus */
    VOS_UINT16                rsv;
} WPHY_TOOL_SlaveScheduleTimeoutInd;
/*
 * �ṹ˵��: PA״̬�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId; /* �����ֶ� */
    VOS_UINT16                   sfn;
    VOS_UINT16                   cfn;
    WPHY_TOOL_MntnPaStatusUint16 paStatus;
    VOS_UINT16                   rsv;
} WPHY_TOOL_PaStatusReportInd;

/*
 * �ṹ˵��: PHY �ڲ���Ϣ�ϱ���������
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */      /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                msgRptFlag; /* ָʾ�ڲ���Ϣ�Ƿ��ϱ� */
    VOS_UINT16                rsv;        /* �����ֶ� */
} WPHY_TOOL_IntraMsgRptCtrlReq;

/*
 * �ṹ˵��: ��̬��ѯǰ��״̬����ԭ��
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
} WPHY_TOOL_RfFrontEndStaticRequireReq;

/*
 * �ṹ˵��: ��̬��ѯǰ��״̬�ظ�ԭ��
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;         /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;       /* �����ֶ� */
    VOS_UINT16                rfTrcvOnState; /* RF�ܿ��أ�1Ϊ�򿪣�0Ϊ�ر� */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT16 rfRxOnOrOff; /* 04h[1:0] = 3Ϊ�� */
#endif
    VOS_UINT16 rxPathselect; /* 04h[4:3] = 3Ϊ���ּ�������2������ */
    VOS_UINT16 wagcSwitch;   /* WBBP ���ּ����� */

    VOS_UINT16 abbRxConfig;           /* 3Ϊ���ּ�������1������ */
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:Gģ 2:Wģ 3:LTEģ 4:TDSģ 5:ETģʽ 6:APTģʽ 7:CAģʽ */
    VOS_UINT16 abbTxCh01LineCtrlMode;
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:Gģ 2:Wģ 3:LTEģ 4:TDSģ 5:ETģʽ 6:APTģʽ 7:CAģʽ */
    VOS_UINT16 abbTxCh23LineCtrlMode;
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:Gģ 2:Wģ 3:LTEģ 4:TDSģ 5:ETģʽ 6:APTģʽ 7:CAģʽ */
    VOS_UINT16 abbRxCh01LineCtrlMode;
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:Gģ 2:Wģ 3:LTEģ 4:TDSģ 5:ETģʽ 6:APTģʽ 7:CAģʽ */
    VOS_UINT16 abbRxCh23LineCtrlMode;
    VOS_UINT16 abbRxCh01Mode;    /* CH1:[15:8],CH0:[7:0] = 0:2G 1:3G_SC 2:4G 3: 3G_DC 4:TDS */
    VOS_UINT16 abbRxCh23Mode;    /* CH1:[15:8],CH0:[7:0] = 0:2G 1:3G_SC 2:4G 3: 3G_DC 4:TDS */
    VOS_UINT16 abbCh01VoiceData; /* CH1:[15:8],CH0:[7:0] = 1:voice mode 2: data mode */
    VOS_UINT16 abbCh23VoiceData; /* CH1:[15:8],CH0:[7:0] = 1:voice mode 2: data mode */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT16 rfTxOnOrOff; /* 02h[1:0] = 3Ϊ�� */
#endif
    VOS_UINT16 rfTxShdn;     /* RF���д򿪿��ƣ�0��BBP���� */
    VOS_UINT16 txPathselect; /* RFIC TX Patch */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT16 rfTxAgc; /* RFIC TXAGC */
#endif
    VOS_UINT16 cpcDrxFuncOn;   /* ��ǰ������CPC DRX�ı�־ */
    VOS_UINT16 cpcDrxStartCfn; /* CPC DRX��ʼ������CFN */
    VOS_UINT16 cpcSleepBitMap; /* ��16bit��1Ϊ����ʱ϶��0Ϊ˯��ʱ϶ */

    VOS_UINT16 gpioFuncOn; /* Ӳ��֧��GPIO */
    VOS_UINT16 gpioPaMode; /* GPIO PA mode */

    VOS_UINT16 mipiFuncOn; /* Ӳ��֧��MIPI */
    VOS_UINT16 mipiPaMode; /* MIPI PA mode */

    VOS_UINT16 aptFuncOn; /* Ӳ��֧��APT */
    VOS_UINT16 aptPaMode; /* APT��ѹmode */
    VOS_UINT16 aptVcc;    /* ���� */
} WPHY_TOOL_RfFrontEndStaticRequireInd;

/*
 * �ṹ˵��: �Ƿ�ʵʱ���ǰ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnSwitchCtrlUint16 displayCtrl;
    VOS_UINT16                     reserved;
} WPHY_TOOL_RfFrontEndDisplayReq;

/*
 * �ṹ˵��: RTC���ܴ򿪺͹ر�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    UPHY_TOOL_RtcDebugUint16  rtcCtrl; /* RTC�򿪻��߹ر� */
    VOS_UINT16                rsv;
} WPHY_TOOL_BbpRtcReq;

/*
 * �ṹ˵��: WPHYץȡ�̶��黭ͼ��RAM
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRamRptUint16    rptRam;
    WPHY_TOOL_MntnRptStatusUint16 rptStatus;
} WPHY_TOOL_RamReportReq;

/*
 * �ṹ˵��: WPHYץȡ�̶��黭ͼ��RAM
 */

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId; /* �����ֶ� */
    VOS_UINT16                 sfn;
    VOS_UINT16                 cfn;
    WPHY_TOOL_MntnRamRptUint16 rptRam;
    VOS_UINT16                 rsv;
    VOS_UINT32                 data[2];
} WPHY_TOOL_RamReportInd;

/*
 * �ṹ˵��: WPHYץȡ�ǹ̶���ַ������
 */
typedef struct {
    VOS_UINT32 blkAddr; /* ץȡ���ݿ���׵�ַ */
    VOS_UINT32 blkLen;  /* ץȡ���ݿ�ĳ���,��λ:32bit */
} WPHY_TOOL_BlkReport;

typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rsv;
    WPHY_TOOL_BlkReport           rptBlk[WPHY_TOOL_RPT_BLK_MAX_NUM];
} WPHY_TOOL_BlkReportListReq;

/*
 * �ṹ˵��: WPHYץȡ�ǹ̶���ַ������
 */

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;  /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    WPHY_TOOL_BlkReport       rptBlk[WPHY_TOOL_RPT_BLK_MAX_NUM];
    VOS_UINT32                data[2];
} WPHY_TOOL_BlkReportListInd;

/*
 * �ṹ˵��: ��������ά�ɲ��ϱ�������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId;    /* �����ֶ� */
    WPHY_TOOL_MntnSwitchCtrlUint16 raMntnCtrl; /* ��������ά�ɲ�򿪻�ر� */
    VOS_UINT16                     reserve;    /* �����ֶ� */
} WPHY_TOOL_RaMntnCtrlReq;

/*
 * �ṹ˵��: ARM 0MS��ά�ɲ��ϱ�������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER                                 /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId;        /* �����ֶ� */
    WPHY_TOOL_MntnSwitchCtrlUint16 arm0msMntnCtrl; /* ARM 0MS�򿪻�ر� */
    VOS_UINT16                     reserve;        /* �����ֶ� */
} WPHY_TOOL_Arm0msMntnCtrlReq;

/*
 * �ṹ˵��: UPA��֡��ά�ɲ���Ϣ�ϱ��ṹ��
 */
typedef struct {
    VOS_UINT16 rptValue[WPHY_MNTN_HSUPA_RPT_VALUE_MAX_NUM][WPHY_MNTN_HSUPA_OM_CARERIER_NUM];
} WPHY_TOOL_UpaSubframeRptInfo;

/*
 * �ṹ˵��: UPA��ά�ɲ���Ϣ�ϱ��ṹ��
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId; /* �����ֶ� */
    VOS_UINT16                   tti;     /* TTI���� */
    VOS_UINT16                   resvered;
    WPHY_TOOL_UpaSubframeRptInfo rptValue[WPHY_MNTN_HSUPA_SUB_FRM_NUM];
} WPHY_TOOL_UpaRptInfo;

/*
 * �ṹ˵��: UPA��ά�ɲ���Ϣ�ϱ��ṹ��
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId; /* �����ֶ� */
    VOS_UINT16                   tti;     /* TTI���� */
    VOS_UINT16                   resvered;
    WPHY_TOOL_UpaSubframeRptInfo rptValue;
} WPHY_TOOL_UpaRptInfoTti10ms;

/*
 * �ṹ˵��: ���������������ϱ�����С���������������,����ֵ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_GsmCellSnrReq;

/*
 * �ṹ˵��: ÿ��С���������,����ֵ����Ϣ
 */
typedef struct {
    VOS_UINT16 freqInfo;    /* Ƶ����Ϣ�ϱ�����12bit��ʾƵ�Σ�0:850,1:900,2:1800,3:1900����12bit��ʾƵ��ţ�0-1023�� */
    VOS_INT16  rxLev125dbm; /* �ź�ǿ��ֵ��[-110,-15],��λΪdbm */
    VOS_UINT16 cellSnr;     /* ����ȣ�[0,99],��λΪdb */
    VOS_INT16  preCi;       /* ���ǰ���������Ÿɱ�,��λΪ1dB */
    VOS_INT16  preCiDiv;    /* ���ǰ�ּ������Ÿɱ�,��λΪ1dB, �ּ�����-24dBΪ��Чֵ */
    VOS_UINT16 resvered;    /* �����ֶ� */
} GPHY_TOOL_GsmCellSnr;

/*
 * �ṹ˵��: �����ϱ�����С���������������,����ֵ����Ϣ
 */
typedef struct {
    VOS_UINT16           msgId;                           /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16           toolId;                          /* �����ֶ� */
    VOS_UINT16           cellNum;                         /* Ƶ���� */
    GPHY_TOOL_GsmCellSnr cellInfo[GPHY_TOOL_MA_FREQ_NUM]; /* С����Ϣ */
    VOS_UINT16           resvered;                        /* �����ֶ� */
} GPHY_TOOL_GsmCellSnrInd;

/*
 * �ṹ˵��: �����ϱ�BA LIST���ź�ǿ�ȣ�bsic��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_BaListMeasReq;

/*
 * �ṹ˵��: ÿ��С������Ϣ
 */
typedef struct {
    VOS_UINT16 freqInfo;    /* Ƶ����Ϣ�ϱ�����12bit��ʾƵ�Σ�0:850,1:900,2:1800,3:1900����12bit��ʾƵ��ţ�0-1023�� */
    VOS_INT16  rxLev125dbm; /* �ź�ǿ��ֵ��[-110,-15],��λΪ0.125dbm */
    VOS_UINT16 bsic;        /* ��վ��ʶ�룬0-77����Ч0xffff */
    VOS_UINT16 resvered;    /* �����ֶ� */
} GPHY_TOOL_CellMeas;

/*
 * �ṹ˵��: �ϱ�BA LIST���ź�ǿ�ȣ�bsic��Ϣ
 */
typedef struct {
    VOS_UINT16 msgId;         /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;        /* �����ֶ� */
    /* Ƶ����Ϣ�ϱ�����12bit��ʾƵ�Σ�0:850,1:900,2:1800,3:1900����12bit��ʾƵ��ţ�0-1023�� */
    VOS_UINT16 scellFreqInfo;
    VOS_INT16  scellRxLev125dbm;                                 /* �ź�ǿ��ֵ��[-110,-15],��λΪ0.125dbm */
    VOS_UINT16         hoppingInd;                               /* ��Ƶָʾ,1:��ʾ��Ƶ,0��ʾ����Ƶ */
    VOS_UINT16         resvered1;                                /* �����ֶ� */
    VOS_UINT16         cellNum;                                  /* �����ĸ��� */
    VOS_UINT16         resvered2;                                /* �����ֶ� */
    GPHY_TOOL_CellMeas neighbourCell[GPHY_TOOL_BA_LIST_MAX_NUM]; /* �����Ĳ�����Ϣ */
} GPHY_TOOL_BaListMeasInd;

/*
 * �ṹ˵��: �����ϱ�AMR������Ϣ
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_AmrSettingReq;

/*
 * �ṹ˵��: �ϱ�AMR������Ϣ
 */
typedef struct {
    VOS_UINT16 msgId;                               /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;                              /* �����ֶ� */
    VOS_UINT16 acsNum;                              /* �������,��ΧΪ0-4 */
    VOS_UINT16 resvered;                            /* �����ֶ� */
    VOS_UINT16 acs[GPHY_TOOL_AMR_MAX_ACS_NUM];      /* ������ϣ���Χ0-7 */
    VOS_UINT16 hyst[GPHY_TOOL_AMR_MAX_ACS_NUM - 1]; /* �л����ʹ�С����Χ0-15����λΪ0.5db */
    VOS_UINT16 thd[GPHY_TOOL_AMR_MAX_ACS_NUM - 1];  /* �л����޴�С����ΧΪ0-63����λΪ0.5db */
} GPHY_TOOL_AmrSettingInd;

/*
 * �ṹ˵��: �����ϱ������ÿ���������ʵ�ʹ����
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_AmrCodecUsageReq;

/*
 * �ṹ˵��: �ϱ������ÿ���������ʵ�ʹ����
 */
typedef struct {
    VOS_UINT16 msgId;                               /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;                              /* �����ֶ� */
    VOS_UINT16 acsNum;                              /* �������,��ΧΪ0-4 */
    VOS_UINT16 resvered;                            /* �����ֶ� */
    VOS_UINT16 acs[GPHY_TOOL_AMR_MAX_ACS_NUM];      /* ������ϣ���Χ0-7 */
    VOS_UINT32 uplink[GPHY_TOOL_AMR_MAX_ACS_NUM];   /* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 downlink[GPHY_TOOL_AMR_MAX_ACS_NUM]; /* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
} GPHY_TOOL_AmrCodecUsageInd;

/*
 * �ṹ˵��: ·�������ϱ�AMR�ͷ�AMR��ͬ���ʵ�ʹ����
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_DtStatCodecReq;

/*
 * �ṹ˵��: ·���ϱ�AMR�ͷ�AMR��ͬ���ʵ�ʹ����
 */
typedef struct {
    VOS_UINT16 msgId;                   /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;                  /* �����ֶ� */
    VOS_UINT32 efsDlBlockNum;           /* ����EFS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 efsUlBlockNum;           /* ����EFS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 fsDlBlockNum;            /* ����FS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 fsUlBlockNum;            /* ����FS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 hsDlBlockNum;            /* ����HS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 hsUlBlockNum;            /* ����HS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 afsDlBlockNum;           /* ����AFS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 afsUlBlockNum;           /* ����AFS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 ahsDlBlockNum;           /* ����AHS�����������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 ahsUlBlockNum;           /* ����AHS�����������Χ0-2^32-1����λΪ�� */
} GPHY_TOOL_DtStatCodecInd;

/*
 * �ṹ˵��: ��������������ʵ�ʹ����
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_AmrCodecStatReq;

/*
 * �ṹ˵��: ÿ���������ʵ�ʹ����
 */

typedef struct {
    VOS_UINT32 uplink[GPHY_TOOL_AMR_CODEC_MAX_NUM];   /* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
    VOS_UINT32 downlink[GPHY_TOOL_AMR_CODEC_MAX_NUM]; /* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
} GPHY_AMR_CodecAvg;

/*
 * �ṹ˵��: С�������е�ͳ����Ϣ
 */
typedef struct {
    VOS_UINT16        msgId;  /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16        toolId; /* �����ֶ� */
    GPHY_AMR_CodecAvg cell;   /* С����ͳ����Ϣ */
    GPHY_AMR_CodecAvg call;   /* ÿ�����е�ͳ����Ϣ */
} GPHY_TOOL_AmrCodecStatInd;

/*
 * �ṹ˵��: �����ϱ�CS/PS��������Ϣ
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_CsPsTxInfoReq;

/*
 * �ṹ˵��: �ϱ�CS/PS��������Ϣ
 */
typedef struct {
    VOS_UINT16 msgId;      /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;     /* �����ֶ� */
    VOS_UINT16 ta;         /* ʱ����ǰ������ΧΪ0-63����λΪ���� */
    VOS_UINT16 slotNumber; /* ʱ϶���� */
    VOS_UINT16 txPower[4]; /* ���з��书�ʣ���ΧΪ0-99����λΪdbm */
    VOS_UINT16 pcl;        /* ��վָ���Ĺ��ʵȼ�����ΧΪ0-31 */
    VOS_UINT16 resvered;
} GPHY_TOOL_CsPsTxInfoInd;

/*
 * �ṹ˵��: �����ϱ�CS FER��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_CsFerReq;

/*
 * �ṹ˵��: �ϱ�CS FER��Ϣ
 */
typedef struct {
    VOS_UINT16 msgId;          /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;         /* �����ֶ� */
    VOS_UINT32 errorFrameFull; /* full��Ĵ���֡�� */
    VOS_UINT32 totalFrameFull; /* full��֡�� */
    VOS_UINT32 errorFrameSub;  /* sub��Ĵ���֡�� */
    VOS_UINT32 totalFrameSub;  /* sub��֡�� */
} GPHY_TOOL_CsFerInd;

/*
 * �ṹ˵��: �����ϱ�������·����ֵ
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* �����ֶ� */
    VOS_UINT16 command;  /* ���0:ֹͣ��1������ */
    VOS_UINT16 resvered; /* �����ֶ� */
} GPHY_TOOL_GetCurrentDscReq;

/*
 * �ṹ˵��: �ϱ�������·����ֵ
 */
typedef struct {
    VOS_UINT16 msgId;  /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId; /* �����ֶ� */
    VOS_UINT16 dsc;    /* ������·����ֵ */
    VOS_UINT16 resvered;
} GPHY_TOOL_GetCurrentDscInd;

/*
 * �ṹ˵��: ���������������׶��о�����ϱ�
 */
typedef struct {
    VOS_UINT16 msgId;             /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;            /* tools id */
    VOS_UINT16 dualAntCurId;      /* ����id */
    VOS_UINT16 dualAntNextState;  /* ��һ����ת�׶� */
    VOS_INT16  rxlevAvgSub125dbm; /* ������ƽ��ÿ��ͳ������SUB�������ֵ 0.125dbm */
    VOS_UINT16 reserved;          /* reserved  */
} GPHY_TOOL_DualAntEstimateInd;

/*
 * �ṹ˵��: ��ǰ��Ŀ�����������ȽϽ׶��о�����ϱ���ÿ������״̬�ϱ�һ��
 */
typedef struct {
    VOS_UINT16 msgId;                   /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;                  /* tools id */
    VOS_UINT16 roundNum;                /* round���� */
    VOS_UINT16 compareFinishFlag;       /* ˫���߱Ƚ��Ƿ����������Ia Ib II IIa  */
    VOS_UINT16 dualAntCurState;         /* ��ǰ�ĵ���״̬ */
    VOS_UINT16 dualAntNextState;        /* ��һ������״̬ */
    VOS_UINT16 antChangeFlag;           /* ��ǰstate�Ƿ������߱�־ */
    VOS_UINT16 dualAntCurId;            /* �л�ǰ������id */
    VOS_UINT16 dualAntSecId;            /* ��ǰ�Ƚϵĵ�����id */
    VOS_UINT16 dualAntNextId;           /* �л��������id */
    VOS_INT16  rxlevFilterPriAnt125dbm; /* ��ǰ����ƽ��ÿ��ͳ�����ڵ������˲�ֵ */
    VOS_INT16  rxlevFilterSecAnt125dbm; /* Ŀ������ƽ��ÿ��ͳ�����ڵ������˲�ֵ */
    VOS_INT16  deltaRxlev125dbm;        /* Ŀ�������뵱ǰ����ƽ��ÿ��ͳ�����ڵ������˲���ֵ */
    VOS_UINT16 goodEnoughFlag;          /* �����ϱ������Ƿ����㹻������ */
} GPHY_TOOL_DualAntCompareInd;

/*
 * �ṹ˵��: ��ǰ��Ŀ������ ������ֵ�ļ������ά��
 */
typedef struct {
    VOS_INT16  initDeltaRxlev125dbm;   /* �������ߵĳ�ʼ������ֵ ��Ŀ������-��ǰ���ߡ�����λ1/8 dB */
    VOS_INT16  dualAntImbalance125dbm; /* ����ǰ����->Ŀ�����ߡ���imbalanceֵ����λ1/8 dB */
    VOS_INT16  sarOffsetVaule;         /* ����ǰ����->Ŀ�����ߡ���SarOffsetVaule����λ1/8 dB */
    VOS_UINT16 sarOffsetWeight;        /* SarOffset��Ȩ�أ���λ % */
    VOS_INT16  finalDeltaRxlev125dbm;  /* �����������ߵ�������ֵ ��Ŀ������-��ǰ���ߡ�����λ1/8 dB */
    VOS_UINT16 rsv;                    /* ����λ */
} GPHY_TOOL_DualAntDeltaRxlev;

/*
 * �ṹ˵��: ��ǰ��Ŀ�����������ȽϽ׶��о�����ϱ�����Ϊ���ײ����㷨��ÿ���ϱ����ڶ��ϱ�һ��
 */
typedef struct {
    VOS_UINT16                  msgId;                   /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16                  toolId;                  /* tools id */
    VOS_UINT16                  countTotal;              /* �ϱ����ڼ��� */
    VOS_UINT16                  countHigh;               /* deltaֵ���ڸ����޵��ϱ����ڼ���  */
    VOS_UINT16                  countLow;                /* deltaֵС�ڵ����޵��ϱ����ڼ��� */
    VOS_UINT16                  dualAntCurId;            /* ��ǰ����id */
    VOS_UINT16                  dualAntSecId;            /* Ŀ������id */
    VOS_INT16                   rxlevFilterPriAnt125dbm; /* ��ǰ����ƽ��ÿ��ͳ�����ڵ������˲�ֵ */
    VOS_INT16                   rxlevFilterSecAnt125dbm; /* Ŀ������ƽ��ÿ��ͳ�����ڵ������˲�ֵ */
    VOS_INT16                   deltaRxlev125dbm;        /* Ŀ�������뵱ǰ����ƽ��ÿ��ͳ�����ڵ������˲���ֵ */
    GPHY_TOOL_DualAntDeltaRxlev dualAntDeltaRxlve;       /* �����ߵĲ�ֵ������� */
} GPHY_TOOL_DualAntCompareSubInd;

/*
 * �ṹ˵��: MAS��������Ϣ�ϱ�
 */
typedef struct {
    VOS_UINT16 msgId;            /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;           /* tools id */
    VOS_UINT16 masDpdtStatus;    /* �л���MAS dpdt��״̬ */
    VOS_UINT16 masChangeAntType; /* �л�����: Probe or Switch */
} GPHY_TOOL_MasChangeAntInd;

/*
 * �ṹ˵��: �����л�ͣ�ͽ׶��о�����ϱ�
 */
typedef struct {
    VOS_UINT16 msgId;            /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;           /* tools id */
    VOS_UINT16 dualAntCurId;     /* ����id */
    VOS_UINT16 dualAntNextState; /* ��һ����ת�׶� */
    VOS_UINT16 handAntEn;        /* ͣ�ͽ׶������������־ */
    VOS_UINT16 reserved;         /* reserved  */
} GPHY_TOOL_DualAntDelayInd;

/*
 * �ṹ˵��: ���������������׶��о�����ϱ�
 */
typedef struct {
    VOS_UINT16 msgId;    /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;   /* TOOL ID������ */
    VOS_UINT16 sarDprEn; /* DPR״̬ */
    VOS_UINT16 rsv;      /* ���� */
} GPHY_TOOL_TimeAvgSarDprStateInd;

/*
 * �ṹ˵��: ���������������׶��о�����ϱ�
 */
typedef struct {
    VOS_UINT16 msgId;                                        /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;                                       /* TOOL ID������ */
    VOS_INT16  gmskSarLimitedMaxPwr[GPHY_PS_MAX_TX_TSN_NUM]; /* GMSK��ʱ��ƽ��SAR������ƹ��ʣ�0.1 dBm */
    VOS_INT16  sarLimitedMaxPwr8psk[GPHY_PS_MAX_TX_TSN_NUM]; /* 8PSK��ʱ��ƽ��SAR������ƹ��ʣ�0.1 dBm */
    VOS_UINT16 dprState;                                     /* DPR״̬ ON/OFF */
    VOS_UINT16 curPeirodAvgPwrIndex;                         /* ��ǰͳ����������ֵ,��400������ */
    VOS_INT32  curPeirodAvgPwr;                              /* ��ǰͳ�����ڵ�ƽ�����ʷ�������0.125 dBm */
    VOS_INT32  timeAvgSarAdjudgeSum;                         /* ��ǰ�о�����ƽ�������ܺ�,��λ0.125dbm */
    VOS_INT32  averagePower;                                 /* �о����ڵ�ƽ�����ʷ�������0.125 dBm */
    VOS_INT32  curPeirodAvgPwrLine;                          /* ��ǰͳ�����ڵ�ƽ����������ֵ */
    VOS_INT32  adjudgeSumLine;                               /* ��ǰ�о�����ƽ����������ֵ�ܺ� */
    VOS_INT32  averagePowerLine;                             /* �о����ڵ�ƽ����������ֵ */
    VOS_UINT32 curSlice;                                     /* ��ǰ��slice */
} GPHY_TOOL_TimeAvgSarPeriodInd;

/*
 * �ṹ˵��: RF RESETִ���ϱ�ά����Ϣ
 */
typedef struct {
    VOS_UINT16 msgId;                   /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;                  /* TOOL ID������ */
    VOS_UINT16 dsdsFlag;                /* DSDS��־ 1-DSDS 0-DSDA */
    VOS_UINT16 dsdsVersion;             /* DSDS�汾 1-DSDS2.0 0DSDS1.0 */
    VOS_UINT16 recordRfResetChannelId;  /* ��¼��RF RESETͨ�� */
    VOS_UINT16 gsmRxRfSsiChannel;       /* 6362 Gsm Rx����RFIC */
    VOS_UINT16 gsmRfConnection;         /* gsm Rf A/Bͨ������ */
    VOS_UINT16 rsv;                     /* ���� */
} GPHY_TOOL_RfResetReport;

/*
 * �ṹ˵��: G��ģǿͣ��ģʱ�ϱ�ά����Ϣ
 */
typedef struct {
    VOS_UINT16                                msgId;     /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16                                toolId;    /* TOOL ID������ */
    VOS_UINT32                                gapLength; /* �ֹ�ʱ���õĹ���,��λQb */
    VOS_UINT32                                curFn;     /* ��ǰ��֡�� */
    VOS_UINT16                                curQb;     /* ��ǰ��qb */
    GPHY_TOOL_MntnMasterStopSlaveReasonUint16 slaveTask; /* ��ģ���� */
} GPHY_TOOL_MasterStopSlaveMeasInd;

/*
 * �ṹ˵��: GSM DC Offset Report
 */
typedef struct {
    VOS_UINT16               msgId;        /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16               toolId;       /* tools id */
    VOS_UINT16               curChannelId; /* ��ǰͨ��  */
    UPHY_TOOL_DcoffsetUint16 updateType;
    VOS_UINT16               freqInfo;
    VOS_UINT16               gain;
    VOS_INT16                remainI;
    VOS_INT16                remainQ;

    VOS_INT16  initDcoci;   /* ��ʼֵ,RFIC����ֵΪ0x80��Ӧ��I·DCֵ */
    VOS_INT16  initDcocq;   /* ��ʼֵ,RFIC����ֵΪ0x80��Ӧ��Q·DCֵ */
    VOS_INT16  rfDci;       /* У׼��,RFIC I·DC����ֵ */
    VOS_INT16  rfDcq;       /* У׼��,RFIC Q·DC����ֵ */
    VOS_INT16  remainDcoci; /* У׼��,I·����DCֵ */
    VOS_INT16  remainDcocq; /* У׼��,Q·����DCֵ */
    VOS_UINT16 blockMode;   /* ��ǰ��blockģʽ */
    VOS_UINT16 antPath;     /* ���ּ����� */
} GPHY_TOOL_DcOffsetReport;

/*
 * �ṹ˵��: ��ʱ���µ�ǰppm��scpll����ֵ���¶�ֵ��AFCֵ
 */
typedef struct {
    VOS_INT32 ppmValue; /* ppm */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32 pllFracValue;  /* Scpll С�����ֵ���ֵ */
    VOS_UINT32 pllInterValue; /* Scpll �������ֵ���ֵ */
#endif
    VOS_UINT32 afcValue;         /* AFC */
    VOS_INT16  temperatureValue; /* �¶� */
    VOS_UINT16 reserved;
} WPHY_TOOL_SingleXoInfo;

/*
 * �ṹ˵��: ǰ��RSSI��Ϣ
 */
typedef struct {
    VOS_UINT32 ant1Rssi; /* RSSI 1414 */
    VOS_UINT32 ant2Rssi; /* RSSI 1414 */
} WPHY_TOOL_Rssi;

#if (FEATURE_ON == FEATURE_EASYRF)
/*
 * �ṹ˵��: ǰ��״̬��Ϣ
 */
typedef struct {
    WPHY_TOOL_AeasyrfRxStatusUint16 intraRxStatus; /* ΪTRUE��ƵRX�򿪣�����ƵRX�ر� */
    WPHY_TOOL_AeasyrfTxStatusUint16 intraTxStatus; /* ΪTRUE��ƵTX�򿪣�����ƵTX�ر� */
} WPHY_TOOL_RfStatus;
#else
/*
 * �ṹ˵��: ǰ��״̬��Ϣ
 */
typedef struct {
    VOS_UINT32 rxConfigMode;
    VOS_UINT32 rxPllMode;
    VOS_UINT32 txConfigMode;
    VOS_UINT32 txPllMode;
    VOS_UINT32 rxAgc; /* RxAgc״̬ */
    VOS_UINT32 txAgc; /* TxAgc״̬ */
} WPHY_TOOL_RfStatus;

/*
 * �ṹ˵��: block״̬��Ϣ
 */
typedef struct {
    VOS_UINT16 powerAfterRrc;
    VOS_UINT16 powerBeforeRrc;
    VOS_UINT16 dataValid;
    VOS_UINT16 reserved;
} WPHY_TOOL_BlockStateInfo;

#endif

/*
 * �ṹ˵��: MIPI�ض����
 */
typedef struct {
    VOS_UINT32 data : 8;     /* �ض���� */
    VOS_UINT32 mipiPort : 4; /* mipi port 0~7 */
    VOS_UINT32 slaveId : 4;  /* slave id 0~15 */
    VOS_UINT32 regAddr : 16; /* reg addr 0~65535 */
} UPHY_TOOL_MipiRdResult;

/*
 * �ṹ˵��: TRANSVER ON���õ�ַ��Ϣ
 */
typedef struct {
    VOS_UINT32 ctuLastSetTimestamp;   /* CTUͨ�����һ�θı�ʱ�� */
    VOS_UINT32 ctuSetCnt;             /* CTUͨ���ܸı���� */
    VOS_UINT32 ctuLastCleanTimestamp; /* CTUͨ�����һ�����ʱ�� */
    VOS_UINT32 ctuCleanCnt;           /* CTUͨ������մ��� */
} UPHY_TOOL_CtuChangeInfo;

/*
 * �ṹ˵��: ǰ����Ϣ
 */
typedef struct {
    /* SSI_EN����/RSSI/AGC��λ/DCOFFSET/AFC/BLOCK��NOBLOCK״̬���߿���/ABB�߿�/TRCV_ON״̬/��˫����״̬ */
    WPHY_TOOL_SingleXoInfo singleXoInfo;      /* ��ʱ���µ�ǰppm��scpll����ֵ���¶�ֵ��AFCֵ */
    WPHY_TOOL_Rssi         feRssi;            /* RSSI 1414 */
    WPHY_TOOL_RfStatus     rfStatus;          /* RF״̬��Ϣ */
    VOS_UINT32             wagcSwitch;        /* WAGC_SWITCH 1400 */
    VOS_UINT32             intMaskLteRcvW[2]; /* BBPCOMM_INT_MASK_LTE_RCV_W_ADDR */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32 chnMasterModeRx; /* Channel master mode */
    VOS_UINT32 chnMasterModeTx; /* Channel master mode */
    VOS_UINT32 chnSlaveMode;
    VOS_UINT32 chnSlaveChn[8];

    UPHY_TOOL_CtuChangeInfo masterRxInfo; /* ��ģCTUͨ��Rx�仯��Ϣ */
    UPHY_TOOL_CtuChangeInfo masterTxInfo; /* ��ģCTUͨ��Tx�仯��Ϣ */
    UPHY_TOOL_CtuChangeInfo slaveInfo;    /* ��ģCTUͨ���仯��Ϣ */

    VOS_UINT16 estWorkMode;          /* г������ģʽ */
    VOS_UINT16 spurType;             /* г������ */
    VOS_UINT32 freqHarmDcStatus;     /* bit2:Ƶ��仯�ӳٱ�� bit1:г��ȥ��ʹ�� bit0:ȥDC����ʹ�� */
    VOS_UINT32 listenCtu;            /* ����ͨ���Ĵ�����Ϣ */
    VOS_UINT32 trafficCtu0;          /* ҵ��ͨ����Ϣ */
    VOS_UINT32 trafficCtu1;          /* ҵ��ͨ����Ϣ */
    VOS_UINT32 mipiState;            /* Mipi״̬ */
    VOS_UINT32 rfRpt;                /* RF�ϱ� */
    VOS_UINT32 crossMipiLineRpt;     /* CrossMIpi�߿���Ϣ */
    VOS_UINT32 crossLnaLineRpt;      /* CrossLNa�߿���Ϣ */
    VOS_UINT32 wphyDpdRpt;           /* DPD״̬�ϱ���Ϣ */
    VOS_UINT32 wbbp2EtRpt;           /* ET״̬�ϱ���Ϣ */
    VOS_UINT16 ctuRealAntStatus;     /* Ctu����״̬ */
    VOS_UINT16 increaseTxMaxPowerEn; /* �������з��书���Ƿ�ʹ�� */
#endif
#if (FEATURE_EASYRF == FEATURE_ON)
    VOS_UINT16 softTas   : 1;  /* ���ά����TAS״̬ */
    VOS_UINT16 softMas   : 1;  /* ���ά����MAS״̬ */
    VOS_UINT16 ctuTas    : 1;  /* CTUά����TAS״̬ */
    VOS_UINT16 ctuMas    : 1;  /* CTUά����MAS״̬ */
    VOS_UINT16 preferTas : 1;  /* ��������TAS״̬ */
    VOS_UINT16 preferMas : 1;  /* ��������MAS״̬ */
    VOS_UINT16 forceMap  : 2;  /* TAS/MASǿ�б�־λ */
    VOS_UINT16 tasNvSw   : 1;  /* TAS����ʹ�ܿ��� */
    VOS_UINT16 fs2Sw     : 1;  /* FS2���� */
    VOS_UINT16 bandType  : 2;  /* Ƶ������ */
    VOS_UINT16 swSarSw   : 2;  /* ����SAR���� */
    VOS_UINT16 reserved  : 2;  /* ����λ */
#else
    VOS_UINT16 antExchangeInd; /* ��ǰ����λ�� */
#endif
    VOS_UINT16 ipcFlg;   /* ��ǰipc��Դ�� */
    VOS_UINT16 wtcMode;        /* WTCģʽ */
    VOS_UINT16 shareSel3gp;
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32               ant1Dcr;
    VOS_UINT32               ant2Dcr;
    WPHY_TOOL_BlockStateInfo blockStateInfo[2]; /* block״̬�����Ϣ */
#else
    VOS_UINT32 clkGatingValue;      /* B5000 ���õ�BBP ʱ���ſ�ֵ */
#endif
} WPHY_TOOL_FeInfo;

/*
 * �ṹ˵��: �͹��������Ϣ
 */
typedef struct {
    /* RF/PA/BBP/ABB���µ�״̬����˯ԭ�� */
    VOS_UINT16 guBbpTopMemOffFlg;    /* GuBbp MEM�Ƿ��µ��ж� */
    VOS_UINT16 guBbpTopMemStatus;    /* �洢��ǰ���ϵ�״̬ */
    VOS_UINT32 guBbpLpPurposeBitmap; /* TOP1B2B���µ����Ŀ��,����ΪTURBO����DPA */
    VOS_UINT16 bbpMasterOffFlg;      /* BBP MASTER�Ƿ��µ��ж� */
    VOS_UINT16 bbpMasterStatus;      /* �洢��ǰBBP MASTER���ϵ�״̬ */
} WPHY_TOOL_LowpowerInfo;

/*
 * �ṹ˵��: �ྶ������Ԫ1�����Ϣ
 */
typedef struct {
    VOS_UINT32 en;        /* ��Ԫ1ʹ����Ϣ 0xFD902020 */
    VOS_UINT32 mode;      /* ��Ԫ1ģʽ 0xFD902024 */
    VOS_UINT32 scram;     /* ���� 0xFD902028 */
    VOS_UINT32 cpichSync; /* ֡ͷ��Ϣ 0xFD90202C */
    VOS_UINT32 winOffset; /* ��λ�� 0xFD902030 */
} WPHY_TOOL_MultiputhUnit1Info;

/*
 * �ṹ˵��: �ྶ���������Ϣ
 */
typedef struct {
    VOS_UINT32 fingerEnergy; /* ������ */
    VOS_UINT32 fingerPos;    /* ����λ */
} WPHY_TOOL_FingerInfo;

/*
 * �ṹ˵��: ��Ԫ0 1 2�� F0��ǿ����Ϣ
 */
typedef struct {
    WPHY_TOOL_FingerInfo ant1FingerInfo[WPHY_MNTN_MP_WIN_MAX_NUM];
    WPHY_TOOL_FingerInfo ant2FingerInfo;
} WPHY_TOOL_MpFingerInfo;

/*
 * �ṹ˵��: ˫���� ��Ԫ0 1 2�� F0��ǿ����Ϣ
 */
typedef struct {
    WPHY_TOOL_FingerInfo ant1FingerInfo[WPHY_MNTN_MP_WIN_MAX_NUM];
    WPHY_TOOL_FingerInfo ant2FingerInfo[WPHY_MNTN_MP_WIN_MAX_NUM];
} WPHY_TOOL_TwoAntMpFingerInfo;

/*
 * �ṹ˵��: ����������Ϣ
 */
typedef struct {
    WPHY_TOOL_FingerInfo demodFingerInfo[WPHY_MNTN_DEMOD_FINGER_MAX_NUM];
    VOS_UINT32           fingerEn;
    VOS_UINT32           fingerAntInd;
} WPHY_TOOL_DemodFingerInfo;

/*
 * �ṹ˵��: ��С����ͷβ��Ϣ
 */
typedef struct {
    VOS_UINT32 priWinTop; /* ��ͷ��Ϣ 0xFD9040C0 */
    VOS_UINT32 priWinEnd; /* ��β��Ϣ 0xFD9040C4 */
} WPHY_TOOL_PriWinInfo;

/*
 * �ṹ˵��: ��ͷβ��Ϣ
 */
typedef struct {
    VOS_UINT32 softWinTop; /* ��ͷ��Ϣ 0xFD9040C8 */
    VOS_UINT32 softWinEnd; /* ��β��Ϣ 0xFD9040CC */
} WPHY_TOOL_SoftWinInfo;

/*
 * �ṹ˵��: ��·�������
 */
typedef struct {
    VOS_UINT16 rlScram[WPHY_MNTN_RL_SCRAM_MAX_NUM];
    VOS_UINT16 reserved;
} WPHY_TOOL_RlScramInfo;

/*
 * �ṹ˵��: ��������״̬
 */
typedef struct {
    VOS_UINT16 intraMeasureStatus;       /* ��Ƶ����״̬ */
    VOS_UINT16 interMeasureStatus;       /* ��Ƶ����״̬  */
    VOS_UINT16 interRatMeasureStatus;    /* ��ϵͳ����״̬ */
    VOS_UINT16 interRatbasicStatus;      /* ��ϵͳBSIC��֤״̬ */
#if FEATURE_LTE                          /* _H2ASN_Skip */
    VOS_UINT16 interRatLteMeasureStatus; /* ��ϵͳLTE����״̬ */
#endif /* FEATURE_LTE */                 /* _H2ASN_Skip */
    VOS_UINT16 reserved;
} WPHY_TOOL_MeasureCtrlInfo;

/*
 * �ṹ˵��: ϵͳ��ʱ��Ϣ
 */
typedef struct {
    VOS_UINT32 sysTime;      /* ϵͳ��ʱ */
    VOS_UINT32 cpichPathPos; /* ����С��CPICH���羶�ھ���ʱ���ϵ�λ�� */
    VOS_UINT32 dpchPathPos;  /* ����С��SCCPCH/DPCH���羶�ھ���ʱ���ϵ�λ�� */
} WPHY_TOOL_SysTimeInfo;

/*
 * �ṹ˵��: ������Ϣ
 */
typedef struct {
    VOS_UINT32 errorBlocks; /* �����  */
    VOS_UINT32 totalBlocks; /* �ܿ��� */
} WPHY_TOOL_ErrorCodeInfo;

/*
 * �ṹ˵��: R99�����ŵ�����ͳ��
 */
typedef struct {
    WPHY_TOOL_ErrorCodeInfo blerInfo[WPHY_MNTN_BLER_RPT_MAX_NUM];
} WPHY_TOOL_R99BlerInfo;

/*
 * �ṹ˵��: DPA��������ͳ��
 */
typedef struct {
    WPHY_TOOL_ErrorCodeInfo errorCode[WPHY_MNTN_DPA_ERROR_CODE_RPT_MAX_NUM];
} WPHY_TOOL_DpaErrorCodeInfo;

/*
 * �ṹ˵��: DPA ����������Ϣ
 */
typedef struct {
    WPHY_TOOL_FingerInfo eqFingerInfo[WPHY_MNTN_EQ_FINGER_RPT_MAX_NUM];
} WPHY_TOOL_DpaEqFingerInfo;

/*
 * �ṹ˵��: DPA �ز�CQI��Ϣ
 */
typedef struct {
    VOS_UINT16 cqiInfo[WPHY_MNTN_MAX_CARRIER_NUM];
} WPHY_TOOL_DpaCqiInfo;

/*
 * �ṹ˵��: �������״̬��Ϣ
 */
typedef struct {
    VOS_UINT16 accessStatus;
    VOS_UINT16 accessProcess;    /* ��������ϱ�״̬ */
    VOS_UINT16 avoidOccasionFlg; /* �������ܿ�occasion��� */
    /* 0:������occasion��1:������������ڼ䲻��occasion��2:������������ڼ������occasion */
    VOS_UINT16 raOccasionType;
    VOS_INT16  prachInitRscp;    /* �����������ʼ����ʹ�õ�RSCP */
    VOS_UINT16 reserved;
} WPHY_TOOL_RaStatusInfo;

/*
 * �ṹ˵��: PI��ά�ɲ���Ϣ
 */
typedef struct {
    VOS_UINT16 sfn;        /* PI SFNֵ */
    VOS_UINT16 slot;       /* PI SLOTֵ */
    VOS_UINT16 chip;       /* PI CHIPֵ */
    VOS_UINT16 piAckNum;   /* �⵽PIΪACK�ĸ��� */
    VOS_UINT16 piTotalNum; /* �⵽PI�ܸ��� */
    VOS_UINT16 reserved;
} WPHY_TOOL_PiInfo;

/*
 * �ṹ˵��: ���з��书����Ϣ
 */
typedef struct {
    VOS_INT16  txAvrgPwr;
    VOS_INT16  txMaxPwr;
    VOS_INT16  txMinPwr;
    VOS_INT16  filterDpcchPwr;
    VOS_INT16  allowUlMaxPower;
    VOS_UINT16 reserved;
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32 mrxCorr;  /* BBP�ϱ�����ֵ */
    VOS_INT16  mrxPower; /* ����MRX����ֵ����Ĺ��ʣ�������ģʽ�����������һ��ƫ�� */
    VOS_INT16  paTemp;
#endif
} WPHY_TOOL_TxPowInfo;

/*
 * �ṹ˵��: ���з��书����Ϣ
 */
typedef struct {
    VOS_UINT16 edrxActive; /* E-DRX״̬��Ч�£�T321��ʱ��������������״ָ̬ʾ */
    VOS_UINT16 burstPhase; /* burst���յĵ�ǰ�׶� */
} WPHY_TOOL_EdrxInfo;

/*
 * �ṹ˵��: ��ǰ֡TPCͳ��ֵ
 */
typedef struct {
    VOS_UINT32 dlTpcS0To7;
    VOS_UINT32 dlTpcS8To15;
    VOS_UINT32 ulTpc;
} WPHY_TOOL_TpcInfo;

/*
 * �ṹ˵��: ���ŵ���������
 */
typedef struct {
    VOS_UINT16 bc;
    VOS_UINT16 bd;
    VOS_UINT16 bhs;
    VOS_UINT16 reserved;
} WPHY_TOOL_GainFactor;

/*
 * �ṹ˵��: ѹģ��Ϣ
 */
typedef struct {
    VOS_UINT32 dlGapPara;
    VOS_UINT32 ulGapPara;
} WPHY_TOOL_CmInfo;

/*
 * �ṹ˵��: ʱ�������Ϣ
 */
typedef struct {
    VOS_UINT16 sfncfnMeasFlag;
    VOS_INT16  scramCodeNum;
} WPHY_TOOL_TimeMeaInfo;

/*
 * �ṹ˵��: FET���Կ���ȫ����Ϣ�ṹ��
 */
typedef struct {
    VOS_UINT16 fetStatus;       /* FETʹ�ܿ���,��:UPHY_TRUE,��:UPHY_FALSE */
    VOS_UINT16 startProtectCnt; /* ������������,��λ֡ */
    VOS_UINT32 fetCloseVote;    /* FET����ͶƱ,ÿ��bit����1Ʊ,Ϊ1ָʾͶ�ص�Ʊ,Ϊ0ָʾͶ����Ʊ */
    VOS_UINT32 fetRxOpenVote;   /* FET֡�����н��տ���ͶƱ,ÿ��bit����1Ʊ,Ϊ1ָʾͶ����Ʊ,Ϊ0ָʾͶ�ص�Ʊ */
    VOS_UINT32 decodeInfo;      /* FET��ǰ������Ϣ�ϱ� */
    WPHY_TOOL_FetFramePatternInfo framePatternInfo; /* FET֡������ģ����Ϣ */
} WPHY_TOOL_FetCtrlInfo;

/*
 * �ṹ˵��: FET_SRBä����ж�ȫ����Ϣ�ṹ��
 */
typedef struct {
    VOS_UINT16 fetBlindStatus;    /* FET SRB ä��⿪�أ���UPHY_TRUE,�ر�UPHY_FALSE */
    VOS_UINT16 fetSrbCheckStatus; /* FET SRB ������������״̬���Ѿ�����Ϊ:UPHY_TRUE,û����Ϊ:UPHY_FALSE */
    VOS_UINT16 fetSrbExistStatus; /* ָʾ�Ƿ����SRB��:UPHY_TRUE:���ڣ�UPHY_FALSE:������ */
    VOS_UINT16 fetSrbWinCount;    /* ͳ�ƽ�������ƽ������֡�� */
    VOS_UINT16 fetDecodeSlotNum;  /* ��������ʱ϶�� */
    VOS_UINT16 fetPwrLoopIndex;   /* ָʾ��ǰά���Ĺ������������֡��Ϣλ�ã���Χ[0,15] */
    VOS_UINT32 po3Linear;         /* DPCCH��DPDCH֮��POƫ������ֵ */
    VOS_UINT32 signalPwrAcc;      /* ͳ��ƽ�������ź����� */
    VOS_UINT32 noisePwrAcc;       /* ͳ��ƽ�������ź����� */
} WPHY_TOOL_FetSrbInfo;

/*
 * �ṹ˵��: ��������ͬʧ�����о������Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    VOS_UINT16 dmodeSyncInd;         /* ��ǰ֡���ͬʧ��״̬ */
    VOS_UINT16 winLen;               /* ����������о�ͳ��ƽ����ĳ��� */
    VOS_UINT16 winCalLen;            /* ����������о�ͳ��ƽ����ʵ�ʼ��㳤�� */
    VOS_UINT16 winIndex;             /* ����������о�ͳ��ƽ����ά��ϵ�� */
    VOS_UINT16 demSyncWinCount;      /* ����������о�ͳ��ƽ����ͳ��֡������ */
    VOS_UINT16 qin;                  /* ͬ���о�Qin���� */
    VOS_UINT16 qout;                 /* ͬ���о�Qout���� */
    VOS_UINT16 isRfValidFrameCnt;    /* ��ʶ��ǰRF��Դ�Ƿ���� */
    VOS_UINT32 curFrameSignalPwrAvg; /* ͳ��ƽ�����ڵ�ǰ֡�ź�����ƽ��ֵ */
    VOS_UINT32 curFrameNoisePwrAvg;  /* ͳ��ƽ�����ڵ�ǰ֡�ź�����ƽ��ֵ */
    VOS_UINT32 winSignalPwrAvg;      /* ͳ��ƽ�������ź�ƽ��ֵ */
    VOS_UINT32 winNoisePwrAvg;       /* ͳ��ƽ����������ƽ��ֵ */
} WPHY_TOOL_SoftDemSyncInfoRpt;

/*
 * �ṹ˵��: �ּ�����ͶƱֵ��0��ʾ��Ч��1��ʾ����2��ʾ��
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                usimVoteValue;
    VOS_UINT16                tasVoteValue;
    VOS_UINT16                piVoteValue;
    VOS_UINT16                cqiVoteValue;
    VOS_UINT16                csVoteValue;
    VOS_UINT16                psVoteValue;
    VOS_UINT16                rssiDiffVoteValue;
    VOS_UINT16                pccpchVoteValue;
    VOS_UINT16                ant2State;
    VOS_UINT16                currentState;
} WPHY_TOOL_Ant2VoteListRpt;

/*
 * �ṹ˵��: WPHY DSDS��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    /*
     * bit0:��ǰ֡slot0����Դ���
     * bit3:��һ֡slot3����Դ���
     * bit6:��һ֡slot6����Դ���
     * bit9:��һ֡slot9����Դ���
     * bit12:��һ֡slot12����Դ���
     */
    VOS_UINT16 dsdsResourceFlag;
    VOS_UINT16 dsdsOpenFlag;
    VOS_UINT16 currentChannel;
    VOS_UINT16 reserved;
} WPHY_TOOL_DsdsInfo;

/*
 * �ṹ˵��: DACֵ
 */
typedef struct {
    VOS_UINT32 bitByte0 : 8;
    VOS_UINT32 bitByte1 : 8;
    VOS_UINT32 bitByte2 : 8;
    VOS_UINT32 bitByte3 : 8;
} WPHY_TOOL_MipidevData;

/*
 * �ṹ˵��: WPHY CLT��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    VOS_UINT16            sn;            /* ��ǰ��SN�� */
    VOS_UINT16            atTuneType;    /* AT����ָʾ�ıջ���г���� */
    VOS_UINT16            cltFailBitmap; /* ��ǰ�ջ���ⲻ��������ԭ��WPHY_CLT_FAIL_ENUM_UINT16 */
    VOS_UINT16            cltState;      /* ��ǰ�ջ�������Ϣ�շ�״̬ */
    VOS_UINT16            isCrossMipi;   /* �Ƿ�ʹ��crossmipi��ֵ */
    VOS_UINT16            detectState;   /* ����״̬ */
    VOS_UINT16            detectCnt;     /* ������� */
    VOS_UINT16            clValideFlag;
    VOS_UINT16            hvDacMode;
    VOS_UINT16            clValidCnt; /* ����CL֮��ʼ����,֮��ÿ֡slot0 +1 */
    WPHY_TOOL_MipidevData hvDacValue;
} WPHY_TOOL_CltInfo;

/*
 * �ṹ˵��: WPHY��״̬������ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_TimingInfo         timingInfo;      /* ��ʱ�ϱ���Ϣ(����Я��SFN CFN SLOT SLICE��Ϣ) */
    VOS_UINT16                   currentFreq;     /* ��ǰ��С��Ƶ�� */
    VOS_UINT16                   sysSramcode;     /* ��ǰ��С������ */
    WPHY_TOOL_RlScramInfo        rlScramInfo;     /* ��·0��6������� */
    WPHY_TOOL_DemodFingerInfo    demodFingerInfo; /* ������Ч�����������λ�� */
    VOS_UINT16                   mpPurpose;       /* ��ǰ�ྶ����Ŀ�� */
    VOS_UINT16                   reserved;
    VOS_UINT32                   mpResourceMap;   /* �ྶ������Դ��� */
    WPHY_TOOL_MultiputhUnit1Info unit1Info;       /* �ྶ������Ԫ1�����Ϣ */
    WPHY_TOOL_MpFingerInfo       mpFingerInfo;    /* ��Ԫ0,1,2 f0�ྶ��������λ��ͨ���ྶ�жϻ�ȡ��������� */
    WPHY_TOOL_PriWinInfo         priWinInfo;      /* ��С����ͷβ */
    WPHY_TOOL_MeasureCtrlInfo    measureCtrlInfo; /* �����������״̬ */
    VOS_UINT16                   intraStatusFlg;  /* ��ǰͬƵС�������ı�� */
    VOS_UINT16                   interStatusFlg;  /* ��ǰ��ƵС�������ı�� */
    /*
     * С����������״̬:bit0~1:��ǰͬƵ������������;bit2~3:stepb����ʹ��;
     * bit4:step1����ʹ�ܡ�bit5:step1��������ʱ϶ͷ�Ƿ����ڱ�ƵStep23����ָʾ;
     * bit6:stepB�����봦��ģʽ.bit8:Step23����ʹ��
     */
    VOS_UINT32                   csTaskStatus;
    WPHY_TOOL_R99BlerInfo r99BlerInfo;            /* �����ŵ�����ͳ�� */
    VOS_UINT32            demEn;                  /* 0-15bitΪ����ŵ�ʹ�ܣ�16bitΪ�����ŵ�ʹ�� */
    WPHY_TOOL_DsdsInfo    dsdsInfo;
    VOS_UINT16            ant2State; /* 0--TX&RX off;1--TX open;2--RX open;3--TX&RX open */
#if (FEATURE_ON == FEATURE_EASYRF)
    /*
     *  bit[0:3]��ʾ��������״̬,0��ʾ���ߣ�1��ʾ���ߣ�
     *  bit[4:7]��ʾ���޲��Կ�,0��ʾ���ǲ��Կ���1��ʾ�ǲ��Կ�
     *  bit[8:11]��ʾRF SUSPEND״̬��0��ʾ��Ч��1��ʾ��Ч
     */
    VOS_UINT16 cardAntConnRfSuspendState;
#else
    /* bit[0:3]��ʾ��������״̬,0��ʾ���ߣ�1��ʾ���ߣ�bit[4:7]��ʾ���޲��Կ�,0��ʾ���ǲ��Կ���1��ʾ�ǲ��Կ� */
    VOS_UINT16 cardAndAntConnState;
#endif
    WPHY_TOOL_CltInfo cltInfo;
#if (FEATURE_ON == FEATURE_EASYRF)
    WPHY_TOOL_WbbpIntraInterStatusInfo wbbpIntraInterStatus; /* BBP����Ƶ״̬��Ϣ */
#endif
    /* ����BBP����MEMģʽѡ��
     * 5'b00000:MSTģʽ��
     * 5'b00001:GSMģʽ��
     * 5'b00010:LTE����ģʽ��
     * 5'b00100:UMTSģʽ��
     * 5'b01000:��Ч(ԭTDSģʽ��
     * 5'b10010:LTE EMTCģʽ��
     */
    VOS_UINT32 shareMemMode;
} WPHY_TOOL_CommBusinessInfo;

/*
 * �ṹ˵��: IDLE̬��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;            /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId;          /* �����ֶ� */
    WPHY_TOOL_FeInfo           feInfo;           /* ǰ����Ϣ */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* ����ҵ����Ϣ */
} WPHY_TOOL_FreeMntnRpt;

/*
 * �ṹ˵��: IDLE̬��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;           /* �����ֶ� */

    WPHY_TOOL_FeInfo           feInfo;           /* ǰ����Ϣ */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* ����ҵ����Ϣ */
    WPHY_TOOL_LowpowerInfo     lowPowerInfo;     /* �͹������ */
    WPHY_TOOL_PiInfo           piInfo;
} WPHY_TOOL_IdleMntnRpt;

/*
 * �ṹ˵��: SAR����ֵ��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    VOS_INT16        sarBackOff;      /* SAR1-SAR4 ����ֵ�ܺ� = Sarsensor + Sarnormal + Sarspecial + Sarmodem */
    VOS_INT16        rseSarBackOff;   /* RSE����ֵ = max(sarSensorRse,sarNormalRse)*/
} WPHY_TOOL_SarMntnRpt;


/*
 * �ṹ˵��: FACH̬��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    WPHY_TOOL_FeInfo           feInfo;           /* ǰ����Ϣ */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* ����ҵ����Ϣ */
    WPHY_TOOL_RaStatusInfo     raStatus;
    WPHY_TOOL_SysTimeInfo      sysTimeInfo;       /* ϵͳ��ʱ�����羶��Ϣ */
    WPHY_TOOL_DpaErrorCodeInfo dpaErrorCode;      /* DPA��������ͳ�� */
    WPHY_TOOL_DpaEqFingerInfo  dpaEqFinger;       /* ����������Ϣ */
    WPHY_TOOL_GainFactor       gainFactor;        /* ���ŵ��������� */
    WPHY_TOOL_EdrxInfo         edrx;              /* EDRXά����Ϣ */
    VOS_INT16                  sirTarget;         /* �⻷����ֵ */
    VOS_UINT16                 olpcCtrlMode;      /* �⻷���ؿ���ģʽBLER����BER */
    VOS_UINT32                 maxAndMinTxPow;    /* �����С����ֵ 0xFD90C0A4 */
    VOS_UINT32                 dpchSccpchDataPow; /* DPCH/SCCPCH�ŵ���������ֵ */
    VOS_UINT32                 paStatus;          /* PA���µ�״̬ */
    VOS_UINT16                 pcUlChanEn;        /* bit�ɵ͵��߷ֱ��ʾ����E-DCH��DCH��RACH��HSUPA������ʹ����Ϣ */
    VOS_UINT16                 compBypass;        /* Pd����ֵ�Ƿ����ڹ��ʲ��� */
    VOS_INT16                  freqComp;
    VOS_INT16                  sarBackOff;              /* ��B5000�Ĺ̶�SAR����ֵ */
    WPHY_TOOL_SarMntnRpt       allSarBackOff;           /* B5000�����е�SAR����ֵ */
    VOS_UINT32                 pdDetectRslt;            /* ʵ�ʷ��书��-���۷��书�ʣ���λ0.1dBm�� */
    VOS_UINT32                 nextOccasionMeasRatType; /* ��һ֡occasionĿ��,ֵ�������ӦVOS_RATMODE_ENUM_UINT32 */
} WPHY_TOOL_FachMntnRpt;

/*
 * �ṹ˵��: DCH̬��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    WPHY_TOOL_FeInfo           feInfo;           /* ǰ����Ϣ */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* ����ҵ����Ϣ */
    WPHY_TOOL_SoftWinInfo      softWinInfo;
    WPHY_TOOL_TxPowInfo        txPow;             /* ���з��书�� */
    WPHY_TOOL_TpcInfo          tpcInfo;           /* ��ǰ֡TPCͳ��ֵ */
    WPHY_TOOL_DpaErrorCodeInfo dpaErrorCode;      /* DPA��������ͳ�� */
    WPHY_TOOL_DpaEqFingerInfo  dpaEqFinger;       /* ����������Ϣ */
    WPHY_TOOL_DpaCqiInfo       dpaCqiInfo;        /* DPA �ز�CQI��Ϣ */
    WPHY_TOOL_GainFactor       gainFactor;        /* ���ŵ��������� */
    WPHY_TOOL_SysTimeInfo      sysTimeInfo;       /* ϵͳ��ʱ�����羶��Ϣ */
    WPHY_TOOL_CmInfo           cmInfo;            /* ѹģ��� */
    WPHY_TOOL_TimeMeaInfo      timeMeaInfo;       /* ʱ�������� */
    VOS_UINT32                 bbpDmodeSyncInd;   /* ��ǰ֡���ͬʧ��״̬ */
    VOS_UINT16                 dmodeSyncInd;      /* ��ǰ֡���ͬʧ��״̬ */
    VOS_UINT16                 frameSyncInd;      /* ��ǰ֡ͬʧ��״̬ */
    VOS_INT16                  sirTarget;         /* �⻷����ֵ */
    VOS_UINT16                 olpcCtrlMode;      /* �⻷���ؿ���ģʽBLER����BER */
    VOS_UINT32                 dpchSccpchDataPow; /* DPCH/SCCPCH�ŵ���������ֵ */
    VOS_UINT32                 berError;          /* BER����ͳ�� */
    VOS_UINT32                 maxAndMinTxPow;    /* �����С����ֵ 0xFD90C0A4 */
    VOS_UINT32                 paStatus;          /* PA���µ�״̬ */
    VOS_UINT16                 pcUlChanEn;        /* bit�ɵ͵��߷ֱ��ʾ����E-DCH��DCH��RACH��HSUPA������ʹ����Ϣ */
    VOS_UINT16                 compBypass;        /* Pd���Ĺ���ֵ�Ƿ����ڹ��ʲ��� */
    VOS_INT16                  freqComp;
    VOS_INT16                  sarBackOff;     /* ��B5000�Ĺ̶�SAR����ֵ */
    WPHY_TOOL_SarMntnRpt       allSarBackOff;  /* B5000�����е�SAR����ֵ */
    VOS_UINT32                 pdDetectRslt;   /* ʵ�ʷ��书��-���۷��书�ʣ���λ0.1dBm�� */
    WPHY_TOOL_FetCtrlInfo      fetCtrlInfo;    /* FET��Ϣ */
    WPHY_TOOL_FetSrbInfo       wphyFetSrbCtrl; /* FETä�����Ϣ */
} WPHY_TOOL_DchMntnRpt;

/*
 * �ṹ˵��: SOCP�������ƿ�ά�ɲ������ϱ��ṹ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    VOS_UINT32 channelId;    /* Դͨ��id */
    VOS_INT8   chanName[12]; /* Դͨ������ */
    VOS_UINT32 deltaTime;    /* �ϱ�ʱ���� slice */
    VOS_UINT32 packageLen;   /* �ϱ�ʱ������ϱ������ݰ��ܳ��� byte����������Ҫд������ݰ���д��ɹ��ģ�д��ʧ�ܵģ� */
    VOS_UINT32 packageNum;   /* �ϱ�ʱ������ϱ������ݰ��ܴ�������������Ҫд������ݰ���д��ɹ��ģ�д��ʧ�ܵģ� */
    VOS_UINT32 abandonLen;   /* �ϱ�ʱ����ڱ��������ݳ��� byte */
    VOS_UINT32 abandonNum;   /* �ϱ�ʱ����ڱ����������ܴ��� */
    VOS_UINT32 thrputEnc;    /* �ϱ������ڱ���Դ������,byte/s��ʹ�ô�����л�ͼ */
    VOS_UINT32 overFlow50Num; /* �ϱ������ڱ���Դbuffռ�ó���50%�Ĵ�����ʹ�ô�����л�ͼ */
    VOS_UINT32 overFlow80Num; /* �ϱ������ڱ���Դbuffռ�ó���80%�Ĵ�����ʹ�ô�����л�ͼ */
} WPHY_TOOL_SocpMntnInfo;

/*
 * �ṹ˵��: С������ʵ����Ϣͳ��
 */
typedef struct {
    VOS_UINT32 totalTime; /* ������ʱ�� */
    VOS_UINT32 totalCnt;  /* �������� */
} WPHY_TOOL_MntnCsInfo;

/*
 * �ṹ˵��: �����ź�����ǿ��ͳ��
 */
typedef struct {
    VOS_UINT32 weakSignalCnt;   /* W�����ź�ǿ��ͳ��-���ź� */
    VOS_UINT32 normalSignalCnt; /* W�����ź�ǿ��ͳ��-���ź� */
    VOS_UINT32 strongSignalCnt; /* W�����ź�ǿ��ͳ��-ǿ�ź� */
} WPHY_TOOL_MntnSignalInfo;

/*
 * �ṹ˵��: W��ģʵ����Ϣͳ��
 */
typedef struct {
    VOS_UINT32 slaveCellSearchCnt; /* W��ģС��������ʱ��ͳ�� */
    VOS_UINT32 slaveMeasCnt;       /* W��ģ�����ܴ���ͳ�� */
    VOS_UINT32 slaveWorkTime;
} WPHY_TOOL_MntnSlaveInfo;

/*
 * �ṹ˵��: FACH̬ʵ����Ϣͳ��
 */
typedef struct {
    VOS_UINT32 accessCntPerLevel[WPHY_TOOL_MNTN_ACCESS_PWR_MAX_LEVEL]; /* W����������ͳ�� */
} WPHY_TOOL_MntnFachInfo;

/*
 * �ṹ˵��: DRX̬ʵ����Ϣͳ��
 */
typedef struct {
    VOS_UINT32               totalTime;        /* DRX̬��ʱ�� */
    VOS_UINT32               totalCnt;         /* DRX̬��PI�ܴ��� */
    VOS_UINT32               totalWorkTime;    /* DRX̬�ܹ���ʱ�� */
    VOS_UINT32               workShortTimeCnt; /* DRX̬����ʱ����ָ����Χ�ڵ����ڴ���������ʱ��С��15ms */
    VOS_UINT32               workLongTimeCnt;  /* DRX̬����ʱ����ָ����Χ�ڵ����ڴ��� */
    WPHY_TOOL_MntnSignalInfo idleSignalInfo;   /* IDLE̬�����ź�ǿ��ͳ��  */
    WPHY_TOOL_MntnSignalInfo pchSignalInfo;    /* PCH̬�����ź�ǿ��ͳ�� */
    VOS_UINT32               forceWakeCnt;     /* DRX̬ǿ�ƻ��Ѵ��� */
    VOS_UINT32               interRatMeasCnt;  /* DRX̬��ϵͳ�����ܴ��� */

    VOS_UINT32 idleInterCellSearchCnt; /* IDLE̬��ƵС��������ʱ�� */
    VOS_UINT32 idleInterMeasCnt;       /* IDLE̬��ƵС�������ܴ��� */
    VOS_UINT32 idleCbsTotalTime;       /* IDLE̬�����㲥�����ʱ�� */
    VOS_UINT32 idleSibRcvCnt;          /* IDLE̬ϵͳ��Ϣ��ּ��ر�ʱ���մ��� */
    VOS_UINT32 idleAnt2SibRcvCnt;      /* IDLE̬ϵͳ��Ϣ��ּ���ʱ���մ��� */
} WPHY_TOOL_MntnDrxInfo;

/*
 * �ṹ˵��: �����ź�����ǿ��ͳ��
 */
typedef struct {
    VOS_UINT32 ulWeakPowerCnt;    /* ���书��ͳ�� */
    VOS_UINT32 ulMiddle1PowerCnt; /* ���书��ͳ�� */
    VOS_UINT32 ulMiddle2PowerCnt; /* ���书��ͳ�� */
    VOS_UINT32 ulMiddle3PowerCnt; /* ���书��ͳ�� */
    VOS_UINT32 ulMiddle4PowerCnt; /* ���书��ͳ�� */
    VOS_UINT32 ulMiddle5PowerCnt; /* ���书��ͳ�� */
    VOS_UINT32 ulStrongPowerCnt;  /* ���书��ͳ�� */
} WPHY_MNTN_UlEnergyInfo;

/*
 * �ṹ˵��: D̬ʵ����Ϣͳ��
 */
typedef struct {
    VOS_UINT32             csOnlyTotalTime;  /* CSҵ����ʱ�� */
    VOS_UINT32             psOnlyTotalTime;  /* PSҵ����ʱ�� */
    VOS_UINT32             csAndPsTotalTime; /* CS&PSҵ����ʱ�� */
    WPHY_MNTN_UlEnergyInfo psOnlyUlPower;    /* ֻ��PSҵ���书��ͳ�� */
    WPHY_MNTN_UlEnergyInfo csUlPower;        /* CSҵ���书��ͳ�ƣ�CS��PSͬʱ���ڵ����Ҳ����CS����ͳ�� */

    VOS_UINT32 ant2CsTotalTime; /* W CS���ּ�������ʱ��ͳ�� ��λΪ10ms���� */
} WPHY_TOOL_MntnDchInfo;

/*
 * �ṹ˵��: ʵ����Ϣͳ��
 */
typedef struct {
    VOS_UINT16              modeflg;            /* ģʽ��־λ */
    VOS_UINT16              length;             /* ��Ч���ݶγ��ȣ�������ģʽ��־�ͳ��ȱ�ʶ */
    WPHY_TOOL_MntnCsInfo    powerCostCsInfo;    /* ������Ϣ */
    WPHY_TOOL_MntnDrxInfo   powerCostDrxInfo;   /* IDLE̬��Ϣ */
    WPHY_TOOL_MntnFachInfo  powerCostFachInfo;  /* FACH̬ͳ����Ϣ */
    WPHY_TOOL_MntnDchInfo   powerCostDchInfo;   /* D̬��Ϣ */
    WPHY_TOOL_MntnSlaveInfo powerCostSlaveInfo; /* ��ģͳ����Ϣ */
} WPHY_TOOL_MntnPowerCostInfo;

/*
 * �ṹ˵��: ʵ����Ϣͳ�Ƽ�¼��
 */
typedef struct {
    VOS_UINT32                  stateStartSlice;   /* Idle̬��free̬cell_search��ʼʱ�� */
    VOS_UINT32                  wakeSlice;         /* Idle̬���� */
    VOS_UINT32                  csPsStartSlice;    /* CS/PSҵ����ʼʱ�� */
    VOS_UINT16                  cellSearchFlg;     /* ����cell_search_req�·���cell_search_ind�ϱ�֮��ı��λ */
    VOS_UINT16                  idleFlg;           /* ����IDLE̬�ı��λ */
    WPHY_MNTN_WakeCntTypeUint16 wakeStatus;        /* �������͵ı��λ */
    VOS_UINT16                  backUpCsFlg;       /* ��һʱ�̵�CS״̬ */
    VOS_UINT16                  backUpPsFlg;       /* ��һʱ�̵�PS״̬ */
    WPHY_TOOL_MntnBoolUint16    cbsStartFlag;      /* CBS������� */
    VOS_UINT32                  cbsStartSlice;     /* CBS����ʱ�� */
    VOS_UINT32                  idleMeasCntBackup; /* IDLE̬��һ����ƵС����������ǰ�Ĳ����ܴ������� */
} WPHY_TOOL_MntnRecordPowerCostInfo;

/*
 * �ṹ˵��: TAS_STATUS report���ܴ򿪺͹ر�����
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;  /* �����ֶ� */
    VOS_UINT32                tasCtrl0; /* ������ */
    VOS_UINT32                tasCtrl1;
    VOS_UINT32                tasCtrl2;
    VOS_UINT32                tasCtrl3;
} UPHY_TOOL_TasStatusReportReq;

/*
 * �ṹ˵��: TAS_STATUS report �ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;           /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolId;          /* �����ֶ� */
    VOS_UINT16                band;            /* ��ǰƵ��  */
    VOS_UINT16                athorizeStatus;  /* TASMAS��Ȩ״̬ */
    VOS_UINT16                dpdtStateSteady; /* ��̬����λ�� */
    VOS_UINT16                dpdtStateTemp;   /* ��̬����λ�� */
    VOS_INT16                 antRscp[4];      /* ����0��3�ź�ǿ��  */
} WPHY_TOOL_TasStatusReportInd;

/*
 * �ṹ˵��: TAS_STATUS report �ϱ�
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;         /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;       /* �����ֶ� */
    VOS_UINT16                currentBand;   /* ��ǰ�л�Ƶ�� */
    VOS_UINT16                authorizeFlag; /* ����TAS��Ȩ״̬ TAS=0,MAS=0/TAS=1,MAS=0/TAS=0,MAS=1/TAS=1,MAS=1 */
    VOS_UINT16                wholeSwitchDpdtStatus;  /* �����л���DPDT״̬ */
    VOS_UINT16                singleSwitchDpdtStatus; /* �����л���DPDT״̬ */
    VOS_INT16                 antRssi125dbm[4];       /* ����0��3�ź�ǿ��  */
} GPHY_TOOL_TasStatusReportInd;

/*
 * �ṹ˵��: ���õ�ʱ��
 */
typedef struct {
    VOS_UINT16 currentQueueType;     /* ��ǰ�����Ƿ�Ϊ�㲥������,1:�������ѡ��㲥����;0:�������ѡ��A������� */
    VOS_UINT16 currentTaskFreqIndex; /* ��ǰ���������ز� */
    VOS_UINT16 queueIndex;           /* ��ǰ�����Ӧ������е����� */
    VOS_UINT16 band;                 /* Ƶ�� */
    VOS_UINT16 freq;                 /* Ƶ�� */
    VOS_UINT16 scramCode;            /* ���� */
    VOS_UINT16 tdIndicator;          /* �㲥����ķּ����� */
    VOS_UINT16 pccpchTaskType;       /* �㲥�������� */
    VOS_UINT16 pccpchTaskStatus;     /* �㲥����״̬ */
    VOS_UINT16 schdlPriority;        /* �������ȼ� */
} WPHY_TOOL_RakePccpchSchdlInfo;

/*
 * �ṹ˵��: ����LTE������ǵ�ָʾ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_CfgTimeInfo         cfgTime; /* Pcpch����ʱ�� */
    WPHY_TOOL_RakePccpchSchdlInfo cfgInfo; /* Pcccpch������Ϣ */
} WPHY_TOOL_RakePccpchSchdlInd;

/*
 * �ṹ˵��: ���õ�ʱ��
 */
typedef struct {
    VOS_UINT16 band;             /* Ƶ�� */
    VOS_UINT16 freq;             /* Ƶ�� */
    VOS_UINT16 scramCode;        /* ���� */
    VOS_UINT16 tdIndicator;      /* �㲥����ķּ����� */
    VOS_UINT16 pccpchTaskType;   /* �㲥�������� */
    VOS_UINT16 pccpchTaskStatus; /* �㲥����״̬ */
    VOS_UINT16 schdlPriority;    /* �������ȼ� */
    VOS_UINT16 rsv;
} WPHY_TOOL_RakeBcchQueueInfo;

/*
 * �ṹ˵��: BCH�㲥������Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16   msgId;                                  /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  toolsId;                                /* �����ֶ� */
    WPHY_TOOL_CfgTimeInfo       cfgTime;                                /* Pcpch����ʱ�� */
    WPHY_TOOL_RakeBcchQueueInfo queueInfo[WPHY_MNTN_BCCH_QUEUE_LENGTH]; /* BCCH������Ϣ */
} WPHY_TOOL_RakeBcchQueueInd;

/*
 * �ṹ˵��: ���õ�ʱ��
 */
typedef struct {
    VOS_UINT16 band;             /* Ƶ�� */
    VOS_UINT16 freq;             /* Ƶ�� */
    VOS_UINT16 scramCode;        /* ���� */
    VOS_UINT16 tdIndicator;      /* �㲥����ķּ����� */
    VOS_UINT16 pccpchTaskType;   /* �㲥�������� */
    VOS_UINT16 pccpchTaskStatus; /* �㲥����״̬ */
    VOS_UINT16 schdlPriority;    /* �������ȼ� */
    VOS_UINT16 rsv;
} WPHY_TOOL_RakeAdemodQueueInfo;

/*
 * �ṹ˵��: ADEMOD������Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16     msgId;                                   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;                                 /* �����ֶ� */
    WPHY_TOOL_CfgTimeInfo         cfgTime;                                 /* ADEMOD����ʱ�� */
    WPHY_TOOL_RakeAdemodQueueInfo queueInfo[WPHY_MNTN_ADMOD_QUEUE_LENGTH]; /* BCCH������Ϣ */
} WPHY_TOOL_RakeAdemodQueueInd;

/*
 * 7 UNION����
 */
/*
 * ������˵��: WPHY��ά�ɲ���Ϣ�ϱ�
 */
typedef union {
    WPHY_TOOL_FreeMntnRpt freeMntnRpt; /* FREE̬��ά�ɲ���Ϣ�ϱ� */
    WPHY_TOOL_IdleMntnRpt idleMntnRpt; /* IDLE̬��ά�ɲ���Ϣ�ϱ� */
    WPHY_TOOL_FachMntnRpt fachMntnRpt; /* FACH̬��ά�ɲ���Ϣ�ϱ� */
    WPHY_TOOL_DchMntnRpt  dchMntnRpt;  /* DCH̬��ά�ɲ���Ϣ�ϱ� */
} WPHY_TOOL_MntnRpt;

/*
 * �ṹ˵��: SAR DPR ON�¼���ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;  /* �����ֶ� */
    WPHY_TOOL_MntnBoolUint16  sarDprEn; /* DPR״̬ */
    VOS_UINT16                rsv;      /* ���� */
} WPHY_TOOL_SarTimeAvgDprEventInd;

/*
 * �ṹ˵��:ʱ��ƽ�� SAR������ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                      /* _H2ASN_Skip */    /* ԭ������ */
    VOS_UINT16                toolsId;                    /* �����ֶ� */
    WPHY_TOOL_MntnBoolUint16  timeAvgSarLimitPowerFlag;   /* ʱ��ƽ��SAR���ƹ��ʱ�� */
    VOS_INT16                 timeAvgSarMaxTxPowerLimit;  /* ʱ��ƽ��SAR���ƹ��ʱ����Чʱ������ƹ���,��λ0.1dbm */
    VOS_INT16                 timeAvgSarStaticPrdPwrAvg;  /* ��ǰͳ�������ڵ�ƽ������,��λ0.125dbm */
    VOS_INT16                 timeAvgSarAdjudgePrdPwrAvg; /* ��ǰ�о������ڵ�ƽ������,��λ0.125dbm */
    VOS_INT32                 timeAvgSarAdjudgeSum;       /* ��ǰ�о�����ƽ�������ܺ�,��λ0.125dbm */
    VOS_UINT16                timeAvgSarStaticPrdIndex;   /* ��ǰͳ�����ڸ�����400������ */
    VOS_UINT16                reserved;
    VOS_UINT32                staticPrdPwrLineAvg;  /* ��ǰͳ�������ڵ�ƽ����������ֵ */
    VOS_UINT32                adjudgePrdPwrLineAvg; /* ��ǰ�о������ڵ�ƽ����������ֵ */
    VOS_UINT32                adjudgeLineSum;       /* ��ǰ�о�����ƽ�������ܺ�����ֵ */
} WPHY_TOOL_SarTimeAvgParaInfo;

/*
 * �ṹ˵��: �ϲ���ľ���ģ��
 */
typedef struct {
    VOS_UINT16 startCfn;
    /* �ӵ͵��߹�15λ��Ч��ÿ��BIT��ʾ��Ӧʱ϶�Ĳ����Ǵ�(ֵΪ1)���ǹر�(ֵΪ0) */
    VOS_UINT16 pattBitMap[WPHY_MNTN_CPC_DRX_PATTERN_FRAME_NUM];
} WPHY_TOOL_CpcDrxPatternInfo;

/*
 * �ṹ˵��: HS-DPCCH�ŵ�CQI�ϱ�DRX���ƾ�̬������Ϣ
 */
typedef struct {
    VOS_UINT16 npre[WPHY_MNTN_HSDPA_OM_CARERIER_NUM]; /* ��ǰ�жϵ���֡�� */
} WPHY_TOOL_CpcDrxCqiStaticInfo;

/*
 * �ṹ˵��: edch dtx����
 */
typedef struct {
    VOS_UINT16 ueDtxCyc1; /*  Units of subframes */
    VOS_UINT16 ueDtxCyc2; /*  Units of subframes */
    VOS_UINT16 macDtxCyc; /*  Units of subframes */
    VOS_UINT16 reserved;
} WPHY_TOOL_DtxEDchTti;

/*
 * �ṹ˵��: HS-SCCH��HS_PDSCH�ŵ����տ�����Ϣ
 */
typedef struct {
    VOS_UINT16 hsscchSchedSlot;             /* HS-SCCH��HS_PDSCH�ŵ���������ʱ϶ */
    VOS_UINT16 hsscchEquaRfSlotNum;         /* ����򿪺�RF�򿪵ĳ���ʱ�䣬��ʱ϶Ϊ��λ */
    VOS_UINT16 hsscchInactiveEquaRfSlotCnt; /* ����������Чʱ������Inactivity_Threshold_for_UE_DRX_cycle�ļ����� */
    VOS_UINT16 hsscchInactiveEquaRfSlotNum; /* ����������Чʱ������Inactivity_Threshold_for_UE_DRX_cycle������ */
} WPHY_TOOL_CpcDrxHsscchDrxCtrl;

/*
 * �ṹ˵��: HS-SCCH��HS_PDSCH�ŵ�ACK���Ϳ�����Ϣ
 */
typedef struct {
    VOS_UINT16 post;      /* ����ACKʱ,������Ҫ����POST */
    VOS_UINT16 rfSlotNum; /* HSPDSCH��ѯ������ʱ����ACK����֮�䣬RF�򿪵ĳ���ʱ�䣬��ʱ϶Ϊ��λ */
} WPHY_TOOL_CpcDrxHsdpcchAckCtrl;

/*
 * �ṹ˵��: HS-DPCCH�ŵ�CQI�ϱ�DRX������Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16         cqiSchedFlag;      /* TRUE:��ҪCQI DRX���Ƶ���,������Ҫ */
    WPHY_TOOL_MntnCpcDrxCqiPriUint16 cqiDtxPri;         /* �ۺϿ���BBP�����ά���ó���CQI���ȼ� */
    WPHY_TOOL_CpcDrxCqiStaticInfo    cqiStaticNpreInfo; /* HS-DPCCH�ŵ�CQI�ϱ�DRX���ƾ�̬������Ϣ��ǰ�жϵ���֡�� */
    /* CQI DRX���ȿ��Ƶľ���PATTERN��Ϣ */
    WPHY_TOOL_CpcDrxPatternInfo cqiEquaPattInfo[WPHY_MNTN_CPC_DRX_CQI_PATTERN_NUM];
    WPHY_TOOL_CpcDrxPatternInfo cqiRfPattern[WPHY_MNTN_CPC_DRX_CQI_PATTERN_NUM];    /* RF PATTERN */
} WPHY_TOOL_CpcDrxCqiCtrl;

/*
 * �ṹ˵��: CPC DRX info
 */
typedef struct {
    WPHY_TOOL_MntnCpcDrxEquaStatusUint16 cpcEquaWorkState; /* ��ǰ���⹤��״̬ */
    VOS_UINT16                           reserve;
    WPHY_TOOL_CpcDrxPatternInfo          cpcEquaCombPatt;                                       /* �ϲ���ľ���ģ�� */
    WPHY_TOOL_CpcDrxPatternInfo          cpcEquaPattInfo[WPHY_MNTN_CPC_DRX_EQUA_CTRL_TYPE_NUM]; /* ����PATTERN��Ϣ */
} WPHY_TOOL_CpcDrxEquaCtrl;

/*
 * �ṹ˵��: CPC DRX info
 */
typedef struct {
    WPHY_TOOL_MntnCpcDrxRfStatusUint16 rfWorkState; /* ��ǰRF����״̬ */
    VOS_UINT16                         reserve;
    VOS_UINT32                         phySubTaskStaBitMap;                            /* Ӱ��RF��������״̬bitmap */
    WPHY_TOOL_CpcDrxPatternInfo        rfPattInfo[WPHY_MNTN_CPC_DRX_RF_CTRL_TYPE_NUM]; /* RF PATTERN��Ϣ */
    WPHY_TOOL_CpcDrxPatternInfo        combRfPatt;                                     /* �ϲ����RFģ�� */
} WPHY_TOOL_CpcDrxRfCtrl;

/*
 * �ṹ˵��: DCH̬��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16                msgId;                 /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                               toolsId;               /* �����ֶ� */
    WPHY_TOOL_MntnCpcUldpcchCycleStateUint16 ulDpcchPattCycleState; /* �ۺ������ز���״̬��ĳ���cycle״̬����󼯺� */
    WPHY_TOOL_MntnBoolUint16                 agchRgchSchedFlag;     /* �Ƿ����AGCH/RGCH���ȱ�־, TRUE:Ҫ, FALSE:��Ҫ */
    WPHY_TOOL_CpcDrxEquaCtrl                 equaCtrl;              /* ����ģ�� */
    WPHY_TOOL_CpcDrxCqiCtrl                  cqiDrxCtrl;            /* CQI DRX���ȿ��� */
    WPHY_TOOL_CpcDrxRfCtrl                   rfCtrl;                /* RFģ�� */
    WPHY_TOOL_DtxEDchTti                     dtxEdchTti;            /* edch dtx���� */
    WPHY_TOOL_CpcDrxPatternInfo              ulDpcchC1RfPatt;       /* UL Dpcch cycle1 RF PATTERN */
    WPHY_TOOL_CpcDrxPatternInfo              ulDpcchC2RfPatt;       /* UL Dpcch cycle2 RF PATTERN */
    WPHY_TOOL_CpcDrxHsscchDrxCtrl            hsscchDrxCtrl;         /* HsscchDrx���ƽṹ */
    WPHY_TOOL_CpcDrxHsdpcchAckCtrl           hsdpcchAckCtrl;        /* HsscchAck���ƽṹ */
    /* EDCH���� һ���ز��ϵ� RF PATTERN�������ز���pattern�ֱ�ˢ�� */
    WPHY_TOOL_CpcDrxPatternInfo drxEdchCtrlRfPatt[WPHY_MNTN_HSUPA_OM_CARERIER_NUM];
} WPHY_TOOL_CpcMntnRpt;

/*
 * �ṹ˵��: С����Ϣ�ϱ�
 */
typedef struct {
    VOS_INT16 scramCodeNum; /* ���� */
    VOS_INT16 freqInfo;     /* Ƶ�� */
} WPHY_TOOL_IcCellInfo;

/*
 * �ṹ˵��: �����ز���ICС����Ϣ
 */
typedef struct {
    VOS_UINT16           cellNum;                               /* С������ */
    VOS_UINT16           reserved;                              /* ����λ */
    WPHY_TOOL_IcCellInfo icCellInfo[WPHY_MNTN_IC_MAX_CELL_NUM]; /* ԭ������ */
} WPHY_TOOL_IcCellList;

/*
 * �ṹ˵��: ICС����Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId; /* _H2ASN_Skip */               /* ԭ������ */
    VOS_UINT16                toolsId;                               /* �����ֶ� */
    WPHY_TOOL_IcCellList      icCellList[WPHY_MNTN_MAX_CARRIER_NUM]; /* ԭ������ */
} WPHY_TOOL_IcCellReportInd;

/*
 * �ṹ˵��: �����ز���ICС����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_IcCellReportReq;

/*
 * �ṹ˵��: DTX��Ϣ�ϱ�
 */
typedef struct {
    VOS_UINT16 ulDtxStatus;          /* ԭ������,0--disable 1--enable */
    VOS_UINT16 mainCarrierEnable;    /* ԭ������,0--disable 1--enable */
    VOS_UINT16 secondCarrierEnable;  /* ԭ������,0--disable 1--enable */
    VOS_UINT16 mainCarrierActive;    /* ԭ������,0--Inactive 1--active */
    VOS_UINT16 secondCarrierActive;  /* ԭ������,0--Inactive 1--active */
    VOS_UINT16 mainCarrierOrder;     /* ԭ������,0--Inactive 1--active */
    VOS_UINT16 secondCarrierOrder;   /* ԭ������,0--Inactive 1--active */
    VOS_UINT16 mainCarrierPattern;   /* ԭ������ */
    VOS_UINT16 secondCarrierPattern; /* ԭ������ */
    VOS_UINT16 cqiDtxPriority;       /* ԭ������,0--low 1--high */
    VOS_UINT16 mainCarrierMacDtx;    /* ԭ������,0--continue 1--MAC DTX */
    VOS_UINT16 secondCarrierMacDtx;  /* ԭ������,0--continue 1--MAC DTX */
} WPHY_TOOL_UlDtxInfo;

/*
 * �ṹ˵��: DRX��Ϣ�ϱ�
 */
typedef struct {
    VOS_UINT16 drxActiveStatus; /* ԭ������,0--Inactive 1--active */
    VOS_UINT16 reserved;        /* ����Ϊ */
} WPHY_TOOL_DlDrxInfo;

/*
 * �ṹ˵��: CPC��Ϣ
 */
typedef struct {
    VOS_UINT16 enableStatus;       /* ԭ������,0--disable 1--enable */
    VOS_UINT16 activeStatus;       /* ԭ������,0--Inactive 1--active */
    VOS_UINT16 mainCarrierOrder;   /* ԭ������,0--Inactive 1--active */
    VOS_UINT16 secondCarrierOrder; /* ԭ������,0--Inactive 1--active */
} WPHY_TOOL_CpcInfo;

/*
 * �ṹ˵��: RTT״̬�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;        /* �����ֶ� */
    WPHY_TOOL_UlDtxInfo       ulDtxInfo;      /* ԭ������ */
    WPHY_TOOL_DlDrxInfo       dlDRxInfo;      /* ԭ������ */
    WPHY_TOOL_CpcInfo         hsScchLessInfo; /* ԭ������ */
} WPHY_TOOL_RttStatusReportInd;

/*
 * �ṹ˵��: �����ز���ICС����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_RttStatusReportReq;

/*
 * �ṹ˵��: DPA��Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                 /* �����ֶ� */
    VOS_UINT16                enableStatus;            /* ԭ������,0--disable 1--enable */
    VOS_UINT16                activeStatus;            /* ԭ������,0--Inactive 1--active */
    VOS_UINT16                mainCarrierOrder;        /* ԭ������,0--Inactive 1--active */
    VOS_UINT16                secondCarrierOrder;      /* ԭ������,0--Inactive 1--active */
} WPHY_TOOL_DpaInfoInd;

/*
 * �ṹ˵��: DPA �ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_DpaInfoReportReq;

/*
 * �ṹ˵��: Pֵ��Ϣ
 */
typedef struct {
    VOS_UINT32 pValueTotalCnt[WPHY_MNTN_P_VALUE_NUM]; /* ԭ������,0--disable 1--enable */
} WPHY_TOOL_PValueInfo;

/*
 * �ṹ˵��: HS-SCCH������Ϣ
 */
typedef struct {
    VOS_UINT32           subFrameNum;    /* ԭ������,0--disable 1--enable */
    VOS_UINT32           hsScchAttempt;  /* ԭ������,0--Inactive 1--active */
    VOS_UINT32           hsScchSucc;     /* ԭ������,0--Inactive 1--active */
    VOS_UINT32           hsScchSuccRate; /* ԭ������,0--Inactive 1--active */
    WPHY_TOOL_PValueInfo pValueInfo;
} WPHY_TOOL_HsscchDecodeInfo;

/*
 * �ṹ˵��: HS-SCCH������Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId;                                     /* �����ֶ� */
    WPHY_TOOL_HsscchDecodeInfo hsScchDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* ԭ������ */
} WPHY_TOOL_HsscchStatisticReportInd;

/*
 * �ṹ˵��: HSSCCH�����ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_HsscchStatisticReportReq;

/*
 * �ṹ˵��: HS-DSCH������Ϣ
 */
typedef struct {
    VOS_UINT32 qpskNum;          /* ԭ������,0--disable 1--enable */
    VOS_UINT32 qam16Num;         /* ԭ������,0--disable 1--enable */
    VOS_UINT32 qam64Num;         /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 support64QamFlag; /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 sbSub;            /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 sbPlus;           /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 firstSuccNum;     /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 secondSuccNum;    /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 thirdSuccNum;     /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 fourthSuccNum;    /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 fifthSuccNum;     /* ԭ������,0--Inactive 1--active */
    VOS_UINT32 sixthSuccNum;     /* ԭ������,0--Inactive 1--active */
} WPHY_TOOL_HsdschTbsInfo;

/*
 * �ṹ˵��: HS-DSCH������Ϣ
 */
typedef struct {
    VOS_UINT16 tbsLowThredhold;  /* ԭ������,0--disable 1--enable */
    VOS_UINT16 tbsHighThredhold; /* ԭ������,0--disable 1--enable */
} WPHY_TOOL_HsdschTbsThrehold;

/*
 * �ṹ˵��: HS-DSCH������Ϣ
 */
typedef struct {
    VOS_UINT16                  regionNum;                                  /* ԭ������,0--disable 1--enable */
    VOS_UINT16                  reserved;                                   /* ԭ������,0--disable 1--enable */
    WPHY_TOOL_HsdschTbsThrehold tbsThredholdInfo[WPHY_MNTN_MAX_REGION_NUM]; /* ԭ������,0--Inactive 1--active */
    WPHY_TOOL_HsdschTbsInfo     tbsInfo[WPHY_MNTN_MAX_REGION_NUM];          /* ԭ������,0--Inactive 1--active */
} WPHY_TOOL_HsdschDecodeInfo;

/*
 * �ṹ˵��: HSDSCH�����ϱ�����
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId;                                     /* �����ֶ� */
    WPHY_TOOL_HsdschDecodeInfo hsDschDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* ԭ������ */
} WPHY_TOOL_HsdschStatisticReportInd;

/*
 * �ṹ˵��: HSDSCH�����ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId; /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    reserved;
    VOS_UINT16                    regionNum; /* ����Ϊ0-9��������ausThredhold�е���Ŀƥ�� */
    VOS_UINT16                    thredhold[WPHY_MNTN_MAX_REGION_NUM];
} WPHY_TOOL_HsdschStatisticReportReq;

/*
 * �ṹ˵��: PILOT BER�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;                   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;                 /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;                 /* �ϱ������־ */
    VOS_UINT16                    rptPeriod;               /* �ϱ����� */
} WPHY_TOOL_PilotBerReportReq;

/*
 * �ṹ˵��: PILOT 16֡�����ܱ��غ��������Ϣ
 */
typedef struct {
    VOS_UINT16 pilotTotalBitNum; /* Pilot�ܱ�������16֡���� */
    VOS_UINT16 pilotErrorBitNum; /* Pilot���������16֡���� */
} WPHY_TOOL_PilotBerInfo;

/*
 * �ṹ˵��: PILOT BER�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;      /* �����ֶ� */
    WPHY_TOOL_PilotBerInfo    pilotBerInfo; /* ������ */  /* ԭ������ */
} WPHY_TOOL_PilotBerReportInd;

/*
 * �ṹ˵��: ���书�غ�������ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_TpcAndSirReportReq;

/*
 * �ṹ˵��: ���书�غ��������ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;  /* �����ֶ� */
    VOS_INT16                 dlTpcCnt; /* ���书�� */ /* ԭ������ */
} WPHY_TOOL_TpcAndSirReportInd;

/*
 * �ṹ˵��: ���з��书�ʺ����ز��ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_PowerReportReq;

/*
 * �ṹ˵��: ���з��书�ʺ����ز��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;        /* �����ֶ� */
    VOS_INT16                 txAvrgPwr;      /* ƽ�����书�� */
    VOS_INT16                 filterDpcchPwr; /* �˲���� */ /* ԭ������ */
} WPHY_TOOL_PowerReportInd;

/*
 * �ṹ˵��: �����ز�������ڿ�ά�ɲ�ͳ���ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsdpaLinkStatisticsReportReq;

/*
 * �ṹ˵��: �����ز�������ڿ�ά�ɲ�ͳ���ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    /* ��1�б������ز�ÿ����֡��CQI����2�б��渨�ز�ÿ����֡��CQI */
    VOS_UINT16                cqiRpt[WPHY_MNTN_MAX_CARRIER_NUM][WPHY_MNTN_SUB_FRAME_NUM];
} WPHY_TOOL_HsdpaLinkStatisticsReportInd;

/*
 * �ṹ˵��: ���������Ϣ�ϱ���ʽ
 */
typedef struct {
    VOS_UINT16 antennaEnable; /* ����ǰ�˿��ء�1����ʾ�򿪣�0����ʾ�رա�W��TDS��C���á� */
    /* BBP�ϱ����߿�RSSIֵ��˫�ز�֮�ͣ���0.125dB���ȡ�˵������Ҫ����DRX���̡�����DRX˯��ʱ�������BBP��ȡ�Ĵ�����ȡ�� */
    VOS_UINT16 dspRptRssiRf;
    /*
     * BBP�ϱ����ڿ���RF�����״̬����0����ʾS1��1����ʾS2��2����ʾS3��3����ʾS4��
     * 4����ʾS5��5����ʾS6��6����ʾS7��7����ʾS8��˵������Ҫ����DRX���̡�����DRX˯��ʱ�������BBP��ȡ�Ĵ�����ȡ��
     */
    VOS_UINT16 dspRptAgcState;
} WPHY_DUAL_AntennaStatisticsReport;

/*
 * �ṹ˵��: ˫���������Ϣͳ���ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_DualAntennaStatisticsReportReq;

/*
 * �ṹ˵��: ˫���������Ϣͳ���ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId; /* �����ֶ� */
    WPHY_DUAL_AntennaStatisticsReport dualAntennaStatistics[WPHY_MNTN_WBBP_MAX_ANT_NUM]; /* �����ߺ͸����� */
} WPHY_TOOL_DualAntennaStatisticsReportInd;

/*
 * �ṹ˵��: HSUPA EDCH�����Ϣ�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsupaEdchActiveSetReportReq;

typedef struct {
    VOS_UINT16 rlScram;      /* HSUPA���С����������� */
    /* ��ʶ�Ƿ���RGCH�����������·���У�0��ʾ��RL����RGCH����RLS�У�1��ʾ��RL��RGCH����RLS�� */
    VOS_UINT16 rgServRlFlag;
    /* HICH������·��������·���ţ�����ȡֵ��Χ0~5��7Ϊ��Ч�ţ���ʾ�������κ�һ��HICH������·���� */
    VOS_UINT16 tpcIndex;
    /* ��ʶС���Ƿ�E-DCH�ŵ�����С����0��ʾС������E-DCH�ķ���С����1��ʾС����E-DCH�ķ���С�� */
    VOS_UINT16 servingEdchFlag;
    VOS_UINT16 hsupaFlag; /* ��ʶС���Ƿ�֧��HICH��0Ϊ��֧�֣�1��ʾ֧�� */
} WPHY_HSUPA_EdchActiveSetInfoReport;

/*
 * �ṹ˵��: HSUPA EDCH�����Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16          msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                         toolsId; /* �����ֶ� */
    WPHY_HSUPA_EdchActiveSetInfoReport activeSetInfo[WPHY_MNTN_HSUPA_ACTIVE_SET_MAX_NUM];
} WPHY_TOOL_HsupaEdchActiveSetReportInd;

/*
 * �ṹ˵��: С���ظɱ���Ϣ�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_RadioParaCellSnrReq;

/*
 * �ṹ˵��: �ظɱ���Ϣ
 */
typedef struct {
    VOS_UINT32 signalPower; /* �źŹ��� */
    VOS_UINT32 noisePower;  /* �������� */
} WPHY_MNTN_CellSnrInfo;

/*
 * �ṹ˵��: С���ظɱ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                                    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                                  /* �����ֶ� */
    WPHY_MNTN_CellSnrInfo     cellSnrInfo[WPHY_MNTN_SLOT_NUM_BY_FRAME]; /* DPCH 15��ʱ϶�ظɱ���Ϣ */
    VOS_INT16                 sirTarget;                                /* ����õ���SIRĿ��ֵ */
    VOS_INT16                 reserved;                                 /* �����ֶ� */
} WPHY_TOOL_RadioParaCellSnrInd;

/*
 * �ṹ˵��: DPA�����ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HspdschCfgReq;

/*
 * �ṹ˵��: DPA��������Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;         /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;       /* �����ֶ� */
    VOS_INT32                 dlDpchOffset;  /* DPCH�����HSSCCH��ʱ��ƫ�� */
    VOS_UINT16                harqProcesses; /* DPA֧�ֵ��߳��� */
    VOS_UINT16                reserved;      /* �����ֶ� */
} WPHY_TOOL_HspdschCfgInd;

/*
 * �ṹ˵��: HS-SCCH�����ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsscchCfgReq;

/*
 * �ṹ˵��: HS-SCCH����
 */
typedef struct {
    VOS_UINT16 hsScchNo; /* ʹ��HS-SCCH��ű�ʾ����Ч�� */
    VOS_UINT16 ovsf;     /* ��С��HS-SCCH��ovsf��� */
} HS_SCCH_Configuration;

/*
 * �ṹ˵��: HS-SCCH�����ϱ�����Ϣ�ṹ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                hsscchNum;
    VOS_UINT16                reserved;
    HS_SCCH_Configuration     hsscchValue[WPHY_MAIN_CELL_HSDPA_SCCH_MAX_NUM];
} WPHY_TOOL_HsscchCfgInd;

/*
 * �ṹ˵��: HS-DPCCH�ŵ����ò����ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsdpcchCfgReq;

/*
 * �ṹ˵��: HS-DPCCH�ŵ����ò����ϱ���Ϣ��ʽ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;           /* �����ֶ� */
    VOS_UINT16                cqiCycle;          /* CQI�������� */
    VOS_UINT16                cqiRpt;            /* CQI�������������ȡֵ��Χ1~4 */
    VOS_UINT16                cqiAck;            /* ACK/NACK�������������ȡֵ��Χ1~4 */
    VOS_UINT16                hsDpcchChipOffset; /* HS-DPCCH��һ����֡֡ͷ������DPCH֡ͷƫ��chip�� */
    VOS_UINT16                poAck;             /* HS-DPCCH�ŵ������������ӣ����ֵ38 */
    VOS_UINT16                poNack;            /* HS-DPCCH�ŵ������������ӣ����ֵ38 */
    VOS_UINT16                poCqi;             /* HS-DPCCH�ŵ������������ӣ����ֵ38 */
    VOS_UINT16                reserved;          /* �����ֶ� */
} WPHY_TOOL_HsdpcchCfgInd;

/*
 * �ṹ˵��: HSDPA�ŵ�CQI��Ϣ�ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsHsdpaCqiReq;

/*
 * �ṹ˵��: HSDPA�ŵ�CQI��Ϣ�ϱ���ʽ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                        /* �����ֶ� */
    VOS_UINT16                carrierNum;                     /* �ϱ����ز����� */
    VOS_UINT16                reserved;                       /* �����ֶ� */
    VOS_UINT16                cqi[WPHY_MNTN_MAX_CARRIER_NUM]; /* channel quality indicator */
} WPHY_TOOL_HsHsdpaCqiInd;

/*
 * �ṹ˵��: �����ز�������ڿ�ά�ɲ�ͳ���ϱ�����Ϣ�ṹ����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsdpaLinkStatReq;

/*
 * �ṹ˵��: �����ز�������ڿ�ά�ɲ�ͳ���ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                        /* �����ֶ� */
    VOS_UINT32 schedDeltaRate[WPHY_MNTN_MAX_CARRIER_NUM];     /* MAC����ȵ�˲ʱ���ʣ�������CRC OK��TBS���� */
    VOS_UINT32 macLayerDeltaRate[WPHY_MNTN_MAX_CARRIER_NUM];  /* MAC�㴫���˲ʱ���ʣ�������CRC OK��ACK��TBS���� */
    VOS_UINT16 hsscchSuccNum[WPHY_MNTN_MAX_CARRIER_NUM];      /* ������HS-SCCH Success��֡�� */
    VOS_UINT32 hsdpaDetTotalNum[WPHY_MNTN_MAX_CARRIER_NUM];   /* ������HS-SCCH���֡���� */
    VOS_UINT32 hsdpaCrcSuccNum[WPHY_MNTN_MAX_CARRIER_NUM];    /* ������HS-SCCH���ɹ�֡��CRC��ȷ��֡�� */
    VOS_UINT32 totalBlocks[WPHY_MNTN_MAX_CARRIER_NUM];        /* �����ڳ���+�ش��ܿ��� */
    VOS_UINT32 errorBlocks[WPHY_MNTN_MAX_CARRIER_NUM];        /* �����ڳ���+�ش������ */
    VOS_UINT32 hsdpaRetranTxTotal[WPHY_MNTN_MAX_CARRIER_NUM]; /* �������ش��� */
    VOS_UINT32 hsdpaInitTxTotal[WPHY_MNTN_MAX_CARRIER_NUM];   /* �����ڳ����� */
    VOS_UINT32 hsdpaRetranTxError[WPHY_MNTN_MAX_CARRIER_NUM]; /* �������ش������ */
    VOS_UINT32 hsdpaInitTxError[WPHY_MNTN_MAX_CARRIER_NUM];   /* �����ڳ�������� */
    VOS_UINT16 hsdCodeChannelNum[WPHY_MNTN_MAX_CARRIER_NUM];  /* ������UEռ�õ������ */
    VOS_UINT16 hsdpaLessNum[WPHY_MNTN_MAX_CARRIER_NUM];       /* ������Less���� */
    VOS_UINT16 hsdpaNonLessNum[WPHY_MNTN_MAX_CARRIER_NUM];    /* �����ڷ�Less���� */
    WPHY_TOOL_MntnBoolUint16 lessActiveFlag[WPHY_MNTN_MAX_CARRIER_NUM]; /* ����ͳ��������Less�����֡�� */
} WPHY_TOOL_HsdpaLinkStatInd;

/*
 * �ṹ˵��: �ϱ�HS-DSCH������Ϣ��������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsdpaDecodStatReq;

/*
 * �ṹ˵��: �ϱ�HS-DSCH������Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId;                                     /* �����ֶ� */
    WPHY_TOOL_HsscchDecodeInfo hsScchDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* ԭ������ */
} WPHY_TOOL_HsdpaDecStatInd;

/*
 * �ṹ˵��: �ϱ�HS-DSCH������Ϣ�������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    reserved;
    VOS_UINT16                    regionNum; /* ����Ϊ0-9��������ausThredhold�е���Ŀƥ�� */
    VOS_UINT16                    thredhold[WPHY_MNTN_MAX_REGION_NUM];
} WPHY_TOOL_HsdpaDecTbsReq;

/*
 * �ṹ˵��: �ϱ�HS-DSCH������Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId;                                     /* �����ֶ� */
    WPHY_TOOL_HsdschDecodeInfo hsDschDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* ԭ������ */
} WPHY_TOOL_HsdpaDecTbsInd;

/*
 * �ṹ˵��: �ϱ�HS-DSCH������Ϣ
 */
typedef struct {
    VOS_UINT32 schedDataRate;    /* ��·�ɵ��ȴ�������ݱ����� */
    VOS_UINT32 nonSchedDataRate; /* ��·���ɵ��ȴ�������ݱ����� */
} WPHY_MNTN_HsupaLinkThroughputInfo;

/*
 * �ṹ˵��: �ϱ�·�⹦�ܿ���
 */
typedef struct {
    WPHY_TOOL_MntnRptStatusUint16     reportFlag; /* _H2ASN_Skip */ /* �Ƿ���Ҫ�ϱ� */
    VOS_UINT16                        period;     /* _H2ASN_Skip */ /* �ϱ����� */
    VOS_UINT16                        periodCnt;  /* _H2ASN_Skip */ /* �ϱ����ڼ��� */
    WPHY_TOOL_MntnBoolUint16          rptStatus;  /* _H2ASN_Skip */ /* �ϱ�״̬��TRUE��ʾ���ϱ� */
    WPHY_MNTN_HsupaLinkThroughputInfo throughputInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* �����ز�upa��·��������Ϣͳ�� */
} WPHY_MNTN_RptHsupaCtrlInfo;

/*
 * �ṹ˵��: HSUPA��·������ͳ���ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsupaLinkThroughputRptReq;

/*
 * �ṹ˵��: HSUPA��·������ͳ���ϱ�����Ϣ�ṹ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId; /* �����ֶ� */
    VOS_UINT16                        carrierNum;
    VOS_UINT16                        reversed;
    WPHY_MNTN_HsupaLinkThroughputInfo throughputInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* �����ز�upa��·��������Ϣͳ�� */
} WPHY_TOOL_HsupaLinkThroughputRptInd;

/*
 * �ṹ˵��: HSUPA��·������Ϣͳ��
 */
typedef struct {
    VOS_UINT16 sendDataCnt;      /* �ܵ��ȿ��� */
    VOS_UINT16 sblerCnt;         /* ���ȴ������ */
    VOS_UINT16 firstSblerCnt;    /* �������ȿ����� */
    VOS_UINT16 firstSendDataCnt; /* �������ȴ������ */
    VOS_UINT16 powerLimitedCnt;  /* power����ͳ�� */
    VOS_UINT16 sgLimitedCnt;     /* SG����ͳ�� */
    VOS_UINT16 bufferLimitedCnt; /* buffer����ͳ�� */
    VOS_UINT16 limitedTotalCnt;  /* ���������ܿ��� */
    VOS_UINT16 happyCnt;         /* happyͳ�� */
    VOS_UINT16 reserved;         /* ���ȵ��ܿ��� */
} WPHY_MNTN_HsupaLinkQualityInfo;

/*
 * �ṹ˵��: �ϱ�·�⹦�ܿ�����Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnRptStatusUint16  reportFlag; /* _H2ASN_Skip */ /* �Ƿ���Ҫ�ϱ� */
    VOS_UINT16                     period;     /* _H2ASN_Skip */ /* �ϱ����� */
    VOS_UINT16                     periodCnt;  /* _H2ASN_Skip */ /* �ϱ����ڼ��� */
    WPHY_TOOL_MntnBoolUint16       rptStatus;  /* _H2ASN_Skip */ /* �ϱ�״̬��TRUE��ʾ���ϱ� */
    WPHY_MNTN_HsupaLinkQualityInfo qualityInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* �����ز���upa��·������Ϣ */
} WPHY_MNTN_RptHsupaLinkQualityCtrlInfo;

/*
 * �ṹ˵��: HSUPA��·���������ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsupaLinkQualityReq;

/*
 * �ṹ˵��: HSUPA��·���������ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16      msgId;                                  /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId;                                /* �����ֶ� */
    WPHY_MNTN_HsupaLinkQualityInfo qualityInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* �����ز���upa��·������Ϣ */
} WPHY_TOOL_HsupaLinkQualityRptInd;

/*
 * �ṹ˵��: HSUPA��Ȩ��Ϣ
 */
typedef struct {
    VOS_UINT32 agValue;    /* AGͳ���ۼ�ֵ */
    VOS_UINT32 sgCnt;      /* SGͳ��ֵ */
    VOS_UINT32 sgValue;    /* SGͳ��ֵ */
    VOS_UINT16 agCnt;      /* AG����, ����AGƽ��ֵ */
    VOS_UINT16 rgUpCnt;    /* RG upͳ�� */
    VOS_UINT16 rgHoldCnt;  /* RG holdͳ�� */
    VOS_UINT16 rgDownCnt;  /* RG downͳ�� */
    VOS_UINT16 rgTotalCnt; /* RG ����ͳ��, ���ڼ���RG���� */
    VOS_UINT16 totalCnt;   /* �ܵ�ͳ�ƴ��� */
} WPHY_MNTN_HsupaGrantInfo;

/*
 * �ṹ˵��: �ϱ�upa grant·�⹦�ܿ���
 */
typedef struct {
    WPHY_TOOL_MntnRptStatusUint16 reportFlag;                           /* _H2ASN_Skip */ /* �Ƿ���Ҫ�ϱ� */
    VOS_UINT16                    period;                               /* _H2ASN_Skip */ /* �ϱ����� */
    VOS_UINT16                    periodCnt;                            /* _H2ASN_Skip */ /* �ϱ����ڼ��� */
    WPHY_TOOL_MntnBoolUint16      rptStatus;                            /* _H2ASN_Skip */ /* �ϱ�״̬��TRUE��ʾ���ϱ� */
    WPHY_MNTN_HsupaGrantInfo      grantInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* �����ز�����Ȩ��Ϣͳ�� */
} WPHY_MNTN_RptHsupaGrantCtrlInfo;

/*
 * �ṹ˵��: HSUPA GRANTͳ���ϱ�������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_HsupaGrantRptReq;

/*
 * �ṹ˵��: HSUPA GRANTͳ���ϱ���Ϣ�ṹ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                                /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                              /* �����ֶ� */
    WPHY_MNTN_HsupaGrantInfo  grantInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* �����ز���Ȩ��Ϣ */
} WPHY_TOOL_HsupaGrantRptInd;

/*
 * �ṹ˵��: ���书���ϱ�������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_PwrCtrlPwrRptReq;

/*
 * �ṹ˵��: ���书���ϱ���Ϣ�ṹ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;      /* �����ֶ� */
    VOS_INT16                 txPwrVal;     /* ���书�� */
    VOS_INT16                 rxPwrAnt1Val; /* ����1�Ľ��չ��� */
    VOS_INT16                 rxPwrAnt2Val; /* ����2�Ľ��չ��� */
    VOS_UINT16                reserved;     /* ���� */
} WPHY_TOOL_PwrCtrlPwrRptInd;

/*
 * �ṹ˵��: ���书�ʿ���TPC�ϱ���Ϣ�ṹ
 */
typedef struct {
    WPHY_TOOL_MntnDlTpcCmdUint16 dlTpcCmd; /* ���д��书�ʿ����� */
    WPHY_TOOL_MntnUlTpcCmdUint16 ulTpcCmd; /* ���д��书�ʿ����� */
} WPHY_MNTN_TpcInfo;

/*
 * �ṹ˵��: ���书�ʿ���TPC�ϱ�������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* �ϱ������־ */
    VOS_UINT16                    rptPeriod; /* �ϱ����� */
} WPHY_TOOL_PwrCtrlTpcRptReq;

/*
 * �ṹ˵��: ���书�ʿ���TPC�ϱ���Ϣ�ṹ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                                /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                              /* �����ֶ� */
    WPHY_MNTN_TpcInfo         tpcInfo[WPHY_MNTN_SLOT_NUM_BY_FRAME]; /* ���书�ʿ�����Ϣ */
} WPHY_TOOL_PwrCtrlTpcRptInd;

/*
 * �ṹ˵��: W RXУ׼�켣��ӡ, һ����������Ϊһ����ӡ��Ԫ
 */
typedef struct {
    /* ���� */
    VOS_UINT16 rxConfigTimeSfn;
    VOS_UINT16 rxConfigTimeSlot;
    VOS_UINT16 rxConfigTimeChip;
    VOS_UINT16 band;
    VOS_UINT16 freq;
    VOS_UINT16 agcCfg;

    /* ��� */
    VOS_UINT16 rssiStartTimeSfn;
    VOS_UINT16 rssiStartTimeSlot;
    VOS_UINT16 rssiStartTimeChip;
    VOS_UINT16 rssiRsltTimeSfn;
    VOS_UINT16 rssiRsltTimeSlot;
    VOS_UINT16 rssiRsltTimeChip;
    VOS_INT32  rssiAnt1Rslt;
    VOS_INT32  rssiAnt2Rslt;
    VOS_INT16  rssiAnt1FirstRslt;
    VOS_INT16  rssiAnt2FirstRslt;
    VOS_INT16  rssiAnt1EndRslt;
    VOS_INT16  rssiAnt2EndRslt;

    VOS_UINT32 ant1Dcr[WPHY_TOOL_FAST_CAL_READ_DCR_TIMES];
    VOS_UINT32 ant2Dcr[WPHY_TOOL_FAST_CAL_READ_DCR_TIMES];

    /* ����LNA�Ĵ�����Ϣ */
    VOS_UINT16 mainLnaGain0[WPHY_TOOL_FAST_CAL_READ_LNA_REG_NUM]; /* MipiLnaGain0����ȡ�ĸ��Ĵ���ֵ */
    VOS_UINT16 divLnaGain0[WPHY_TOOL_FAST_CAL_READ_LNA_REG_NUM];

    /* Rfic AGC�Ĵ�����Ϣ */
    VOS_UINT16 rficAgcInfo[WPHY_TOOL_FAST_CAL_READ_AGC_REG_NUM]; /* 0������1�ּ� */
} WPHY_TOOL_CalRxMntnUnit;

/*
 * �ṹ˵��: W TXУ׼�켣��ӡ, һ����������Ϊһ����ӡ��Ԫ
 */
typedef struct {
    VOS_UINT16 freq;     /* Ƶ�� */
    VOS_INT16  power;    /* ���� */
    VOS_UINT16 slot;     /* ʱ϶ */
    VOS_UINT16 chip;     /* chip */
    VOS_UINT32 rfRpt;    /* RF�ϱ� */
    VOS_UINT32 paRpt;    /* PA�ϱ� */
    VOS_UINT32 etAptRpt; /* ET�����ϱ� */
} WPHY_TOOL_CalTxMntnUnit;

/*
 * �ṹ˵��: W RXУ׼�켣��ӡ, һ����������Ϊһ����ӡ��Ԫ
 */
typedef struct {
    VOS_UINT16              rxValidNum; /* ��¼����Ч���� */
    VOS_UINT16              reserved;   /* ���� */
    WPHY_TOOL_CalRxMntnUnit wphyRxFastCalMntn[WPHY_CAL_RX_FAST_CAL_MAX_VAILD_NUM];
} WPHY_TOOL_CalRxFastMntnUnit;
/*
 * �ṹ˵��: W RXУ׼�켣��ӡ, һ����������Ϊһ����ӡ��Ԫ
 */
typedef struct {
    VOS_UINT16              txValidNum; /* ��¼����Ч���� */
    VOS_UINT16              txFlag;     /* ���� */
    WPHY_TOOL_CalTxMntnUnit wphyTxFastCalMntn[WPHY_CAL_TX_FAST_CAL_MAX_VAILD_NUM];
} WPHY_TOOL_CalTxFastMntnUnit;

/*
 * �ṹ˵��: W RXУ׼�켣��ӡ, һ����������Ϊһ����ӡ��Ԫ
 */
typedef struct {
    VOS_UINT16 mask;           /* mask */
    VOS_UINT16 reserved1;      /* ���� */
    VOS_UINT32 txPllStatus;    /* txpll״̬ */
    VOS_UINT32 txPll;          /* txpll״̬ */
    VOS_UINT32 txConfig;       /* tx״̬ */
    VOS_UINT32 rxPllStatus;    /* RxPll״̬ */
    VOS_UINT32 rxPll;          /* RxPll״̬ */
    VOS_UINT32 rxConfig;       /* RxPll״̬ */
    VOS_UINT32 txETRpt;        /* ET�ϱ� */
    VOS_UINT32 txRfRpt;        /* APC�ϱ� */
    VOS_UINT32 txPaRpt;        /* PA�ϱ� */
    VOS_UINT32 txPdRpt;        /* Pd�ϱ� */
    VOS_UINT32 rxAagc;         /* AGC״̬�ϱ� */
    VOS_UINT32 rxAnt1Dc;       /* ����1ֱ���ϱ� */
    VOS_UINT32 rxAnt2Dc;       /* ����2ֱ���ϱ� */
    VOS_UINT32 rxAnt1RssiRpt;  /* ����1�����ϱ� */
    VOS_UINT32 rxAnt2RssiRpt;  /* ����2�����ϱ� */
    VOS_UINT32 ratModeFlag;    /* ��ǰģʽָʾ�Ĵ����ϱ�����16bitΪ��1����16bitΪ��2 */
    VOS_UINT32 wtcModeSel;     /* wtcģʽ�ϱ� */
    VOS_UINT32 wagcSwitch;     /* wagcSwitch�ϱ� */
    VOS_UINT32 w3gppShareSel;  /* 3gpp������������Դռ��ָʾ�ϱ� */
    VOS_UINT32 rffeMipiEn;     /* RFFE mipi�ͻ����ϱ� */
    VOS_UINT32 rficMipiEn;     /* RFIC mipi�ͻ����ϱ� */
    VOS_UINT32 rffeMipiStatus; /* RFFE mipi״̬�ϱ� */
    VOS_UINT32 rficMipiStatus; /* RFIC mipi״̬�ϱ� */
    VOS_UINT32 reserved2[2];   /* ���� */
} WPHY_TOOL_AtTrxCalMntnUnit;

/*
 * �ṹ˵��: AT Cal��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId; /* �����ֶ� */
    WPHY_TOOL_AtTrxCalMntnUnit wphyAtTrxCalmntn;
} WPHY_TOOL_AtTrxCalMntnRpt;

/*
 * �ṹ˵��: Rx Fast Cal��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16   msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  toolsId; /* �����ֶ� */
    WPHY_TOOL_CalRxFastMntnUnit wphyToolRxFastCal;
} WPHY_TOOL_RxFastCalMntnRpt;

/*
 * �ṹ˵��: Tx Fast Cal��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16   msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  toolsId; /* �����ֶ� */
    WPHY_TOOL_CalTxFastMntnUnit wphyToolTxFastCal;
} WPHY_TOOL_TxFastCalMntnRpt;

/*
 * �ṹ˵��: MAS ����TAS/MAS״̬�ṹ��
 */
typedef struct {
    WPHY_TOOL_MntnMasAnttasstatusUint16 tasStatus; /* ����TAS״̬ */
    WPHY_TOOL_MntnMasAntmasstatusUint16 masStatus; /* ����MAS״̬ */
} WPHY_TOOL_MasAntennaStatus;

/*
 * �ṹ˵��: ��¼ä�л�״̬������Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16 sampleInd;     /* ����ָʾ( 0:��ʾ��һ������,1:��ʾ�ǵ�һ������ ) */
    VOS_UINT16               calCount;      /* ����ƽ��ֵ�ۼӴ��� */
    VOS_UINT32               measSlice;     /* ��һ�β���ʱ��slice */
    VOS_UINT32               ecSumPre;      /* �����С���������� */
    VOS_UINT32               ecSumCurr;     /* �����С���������� */
    VOS_UINT32               ecAvePre;      /* ����ڵ�С��ƽ���������� */
    VOS_UINT32               ecAveCurr;     /* ����ڵ�С��ƽ���������� */
    VOS_INT16                gainRfPre;     /* �����ǰһ5ms��rf�������� */
    VOS_INT16                gainRfCurr;    /* ����ڵ�ǰ5ms��rf�������� */
    VOS_UINT32               ecAveAccPre;   /* ����ڵ�С��ƽ��ֵ�ۼӾ������� */
    VOS_UINT32               ecAveAccCurr;  /* ����ڵ�С��ƽ���ۼӾ������� */
    VOS_INT16                rscpRfAvePre;  /* T1ʱ���ڼ��С�����߿�����ƽ��ֵ */
    VOS_INT16                rscpRfAveCurr; /* T1ʱ���ڼ��С�����߿�����ƽ��ֵ */
} WPHY_TOOL_MasBlindStateMeasPara;

/*
 * �ṹ˵��: MAS ��������������Ϣ�ṹ��
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16 sampleInd; /* ����ָʾ( FALSE:��ʾ��һ������,TRUE:��ʾ�ǵ�һ������ ) */
    VOS_UINT16               reserve;   /* 4�ֽڶ��� */
    VOS_UINT32               measSlice; /* ��һ�β���ʱ��� */
    VOS_INT16                rscp;      /* ����RSCP */
    VOS_INT16                rssi;      /* ����RSSI */
} WPHY_TOOL_MasNonblindMeasPara;

/*
 * �ṹ˵��: MAS�㷨������Ѷ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16        evaFinishFlag;  /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnBoolUint16        evaInherit;     /* ��ǰState��ǰ�������������Ƿ���Լ̳� */
    WPHY_TOOL_MasAntennaStatus      antStatus;      /* ��ǰState��ǰ����TAS/MAS״̬ */
    WPHY_TOOL_MasBlindStateMeasPara blindEnergy;    /* ��ǰState��ǰ����ä�вɼ��ļ������С��������Ϣ */
    WPHY_TOOL_MasNonblindMeasPara   nonBlindEnergy; /* ��ǰState��ǰ�������вɼ��ĸ��ٵ�С��������Ϣ */
} WPHY_TOOL_MasAntInfoInd;

/*
 * �ṹ˵��: MAS StateI ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;     /* �����ֶ� */
    WPHY_TOOL_MntnBoolUint16     leaveStateI; /* �Ƿ��뿪StateI */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;    /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState;   /* ��һState���ĸ�State */
    WPHY_TOOL_MntnBoolUint16     iaEnable;    /* Ia�Ƿ�ʹ�� */
    WPHY_TOOL_MntnBoolUint16     ibEnable;    /* Ib�Ƿ�ʹ�� */
    VOS_UINT16                   rsv;         /* ���ֽڶ��� */
    WPHY_TOOL_MasAntInfoInd      antInfo;     /* ��ǰ������Ϣ */
} WPHY_TOOL_MasMntnStateiInd;

/*
 * �ṹ˵��: MAS StateIaIb ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;         /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;       /* �����ֶ� */
    VOS_UINT16                   roundCount;    /* ��ǰ�Ƚ����� */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;      /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState;     /* ��һState���ĸ�State */
    WPHY_TOOL_MntnBoolUint16     earlyExit;     /* �Ƿ��㹻�ö��ѿ������綳�� */
    WPHY_TOOL_MntnBoolUint16     destAntBetter; /* Ŀ�������Ƿ�Ϻ�, �൱��Ƿ��л����� */
    WPHY_TOOL_MntnBoolUint16     iaEnable;      /* Ia�Ƿ�ʹ�� */
    WPHY_TOOL_MntnBoolUint16     ibEnable;      /* Ib�Ƿ�ʹ�� */
    VOS_UINT16                   rsv;           /* ���ֽڶ��� */
    WPHY_TOOL_MasAntInfoInd      srcAntInfo;    /* Դ������Ϣ */
    WPHY_TOOL_MasAntInfoInd      destAntInfo;   /* Ŀ��������Ϣ */
} WPHY_TOOL_MasMntnStateiaibInd;

/*
 * �ṹ˵��: MAS StateII ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;         /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;       /* �����ֶ� */
    VOS_UINT16                   roundCount;    /* ��ǰ�Ƚ����� */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;      /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState;     /* ��һState���ĸ�State */
    WPHY_TOOL_MntnBoolUint16     earlyExit;     /* �Ƿ��㹻�ö��ѿ������綳�� */
    WPHY_TOOL_MntnBoolUint16     destAntBetter; /* Ŀ�������Ƿ�Ϻ�, �൱��Ƿ��л����� */
    WPHY_TOOL_MntnBoolUint16     iaEnable;      /* Ia�Ƿ�ʹ�� */
    WPHY_TOOL_MntnBoolUint16     ibEnable;      /* Ib�Ƿ�ʹ�� */
    WPHY_TOOL_MntnBoolUint16     holdBlind;     /* ä��ά�ֱ�� */
    WPHY_TOOL_MasAntInfoInd      srcAntInfo;    /* Դ������Ϣ */
    WPHY_TOOL_MasAntInfoInd      destAntInfo;   /* Ŀ��������Ϣ */
} WPHY_TOOL_MasMntnStateiiInd;

/*
 * �ṹ˵��: MAS StateIII ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;  /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState; /* ��һState���ĸ�State */
} WPHY_TOOL_MasMntnStateiiiInd;

/*
 * �ṹ˵��: MAS TX_STATEI ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;        /* �����ֶ� */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;       /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState;      /* ��һState���ĸ�State */
    VOS_UINT16                   txValidSlotCnt; /* ��Ч��TXʱ϶�������� */
    /* ���ʳ���MaxPower - MPR - SarReduction - uhwTxMonitorPowerValue��TXʱ϶�������� */
    VOS_UINT16                   txBeyondMonitorSlotCnt;
    VOS_UINT32                   deltaTime;
} WPHY_TOOL_TasMntnTxStateiInd;

/*
 * �ṹ˵��: MAS StateII RX��������Ϣ�ṹ��
 */
typedef struct {
    WPHY_TOOL_MasAntennaStatus      antStatus;   /* ��ǰState��ǰ����TAS/MAS״̬ */
    WPHY_TOOL_MasBlindStateMeasPara blindEnergy; /* ��ǰState��ǰ����ä�вɼ��ļ������С��������Ϣ */
} WPHY_TOOL_MntnTxStateiiRxAntInfo;

/*
 * �ṹ˵��: MAS StateII RX��������Ϣ�ṹ��
 */
typedef struct {
    /* astAntInfo[0]:Դ������Ϣ; astAntInfo[1]:Ŀ��������Ϣ */
    WPHY_TOOL_MntnTxStateiiRxAntInfo antInfo[WPHY_MNTN_MAS_ANTENNA_BUTT];
    WPHY_TOOL_MntnMasAntennaUint16 betterAnt; /* ���պ�������Դ���߻���Ŀ������ */
    VOS_UINT16                     rsv;       /* ���ֽڶ��� */
} WPHY_TOOL_MntnTxStateiiRxInfo;

/*
 * �ṹ˵��: MAS_TX_STATEII��λ�ṹ
 */
typedef struct {
    VOS_UINT16 srcAntTimeLength;   /* Դ(��ǰ)����ʱ������λ:ms */
    VOS_UINT16 destAntTimeLength;  /* Ŀ������ʱ������λ:ms */
    VOS_INT16  srcAntTxPowerGain;  /* Դ(��ǰ)���߷��书������ֵ����λ:0.1dB */
    VOS_INT16  destAntTxPowerGain; /* Ŀ�����߷��书������ֵ����λ:0.1dB */
} WPHY_TOOL_MntnTxStateiiLevelInfo;

/*
 * �ṹ˵��: MAS_TX_STATEII��λ�ṹ
 */
typedef struct {
    VOS_UINT16                       levelNum;                                   /* ��λ����Ч��λ���� */
    VOS_UINT16                       rsv;                                        /* 4�ֽڶ��� */
    WPHY_TOOL_MntnTxStateiiLevelInfo levelItem[WPHY_MNTN_TX_STATEII_LEVEL_BUTT]; /* ��λ�� */
} WPHY_TOOL_MntnTxStateiiLevelTable;

/*
 * �ṹ˵��: MAS StateII״̬�ṹ��
 */
typedef struct {
    VOS_UINT16                  txValidSlotCnt;         /* T1����T2ʱ������Ч��TXʱ϶�������� */
    VOS_UINT16                  txBeyondCurrSlotCnt;    /* ���ʳ���Current���ʱȽ����޵�TXʱ϶�������� */
    VOS_UINT16                  txBeyondBackOffSlotCnt; /* ���ʳ���BackOff���ʱȽ����޵�TXʱ϶�������� */
    VOS_UINT16                  txBeyondMonitorSlotCnt; /* ���ʳ���Monitor���ʱȽ����޵�TXʱ϶�������� */
    VOS_UINT32                  deltaTime;              /* �ӽ׶ν���ʱTX�����ɼ���ʱ�� */
    WPHY_TOOL_MntnTxPowerUint16 t1LimitInfo;            /* T1������� */
    WPHY_TOOL_MntnTxPowerUint16 t2LimitInfo;            /* T2������� */
} WPHY_TOOL_MntnTxStateiiSubStateInfo;

/*
 * �ṹ˵��: MAS TX_STATEII RX�������ж��ӽ׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16             msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                            toolsId;      /* �����ֶ� */
    WPHY_TOOL_MntnCtrlModeUint16          ctrlMode;     /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16          currState;    /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 currSubState; /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 nextSubState; /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiRxInfo         rxInfo;       /* ����������Ϣ */
} WPHY_TOOL_TasMntnTxStateiiRxSubInd;

/*
 * �ṹ˵��: MAS TX_STATEII T1 �ӽ׶ο�ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16             msgId;            /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                            toolsId;          /* �����ֶ� */
    WPHY_TOOL_MntnCtrlModeUint16          ctrlMode;         /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16          currState;        /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 currSubState;     /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 nextSubState;     /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiLevelUint16    currLevel;        /* ��ǰ��λ */
    VOS_UINT16                            currAnt;          /* ��ǰ�׶���������λ�� */
    /* ���յ�λ�����ԭʼNV���������SAR����֮��õ���ֱͨ������ǰ�ĵ�λ�� */
    WPHY_TOOL_MntnTxStateiiLevelTable finalLevelTable;
    WPHY_TOOL_MntnTxStateiiSubStateInfo subStateT1Info;     /* T1�ӽ׶���Ϣ */
    VOS_INT16                           passThruSarBackOff; /* ֱͨ����SAR����ֵ */
    VOS_INT16                           swapSarBackOff;     /* ��������SAR����ֵ */
} WPHY_TOOL_TasMntnTxStateiiT1SubInd;

/*
 * �ṹ˵��: MAS TX_STATEII T2�ӽ׶� ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16             msgId;            /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                            toolsId;          /* �����ֶ� */
    WPHY_TOOL_MntnCtrlModeUint16          ctrlMode;         /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16          currState;        /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnMasStateUint16          nextState;        /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 currSubState;     /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 nextSubState;     /* ��ǰState���ĸ�State */
    WPHY_TOOL_MntnTxStateiiLevelUint16    currLevel;        /* ��ǰ��λ */
    WPHY_TOOL_MntnTxStateiiLevelUint16    nextLevel;        /* ��һ��λ */
    VOS_UINT16                            rsv;              /* �����ֶ� */
    /* ���յ�λ�����ԭʼNV���������SAR����֮��õ���ֱͨ������ǰ�ĵ�λ�� */
    WPHY_TOOL_MntnTxStateiiLevelTable finalLevelTable;
    WPHY_TOOL_MntnTxStateiiSubStateInfo subStateT2Info;     /* T2�ӽ׶���Ϣ */
    WPHY_TOOL_MntnTxLevelAdjustUint16   levelAdjustRslt;    /* �������о���� */
    VOS_UINT16                          currAnt;            /* ��ǰ�׶���������λ�� */
    VOS_INT16                           passThruSarBackOff; /* ֱͨ����SAR����ֵ */
    VOS_INT16                           swapSarBackOff;     /* ��������SAR����ֵ */
} WPHY_TOOL_TasMntnTxSTtateiiT2SubInd;

/*
 * �ṹ˵��: TAS TX_SIGNAL ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;                   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId;                 /* �����ֶ� */
    WPHY_TOOL_MntnTxSignalStateUint16 txSignalState;           /* TX����״̬ */
    VOS_UINT16                        txValidSlotCount;        /* ��Ч��TXʱ϶�������� */
    VOS_UINT16 txBeyondPoorSlotCount; /* ���ʳ���MaxPower - MPR - SarReduction - uhwTxPoorPowerValue��TXʱ϶�������� */
    /* ����С��MaxPower - MPR - SarReduction - uhwTxGoodPowerValue��TXʱ϶�������� */
    VOS_UINT16 txLessThanGoodSlotCount;
    VOS_UINT32 deltaTime;               /* �ܵ�ͳ��ʱ�� */
} WPHY_TOOL_TasMntnTxSignalInd;

/*
 * �ṹ˵��: Fs2 TAS TX_SIGNAL ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;                   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId;                 /* �����ֶ� */
    WPHY_TOOL_MntnTxSignalStateUint16 txSignalState;           /* �ĸ�TX����״̬ */
    VOS_UINT16                        txValidSlotCount;        /* ��Ч��TXʱ϶�������� */
    VOS_UINT16 txBeyondPoorSlotCount;     /* ����������TX���ʳ���Poor���޵�ͳ�ƴ��� */
    VOS_UINT16 txLessThanGoodSlotCount;   /* ����������TX���ʳ���Good���޵�ͳ�ƴ��� */
    VOS_UINT32 deltaTime;                 /* �ܵ�ͳ��ʱ�� */
} WPHY_TOOL_Fs2TasMntnTxSignalInd;

/*
 * �ṹ˵��: MAS �����㷨��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;            /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;          /* �����ֶ� */
    VOS_INT16                 rscpSrc;          /* �������߲������ */
    VOS_INT16                 rscpDest;         /* �ּ����߲������ */
    VOS_UINT16                countLow;         /* Low Count ���� */
    VOS_UINT16                countHigh;        /* High Count ���� */
    VOS_UINT16                countTotal;       /* Total Count ���� */
    VOS_UINT16                rsv;              /* ���ֽڶ��� */
    WPHY_TOOL_MntnBoolUint16  dualAntEvaFinish; /* ˫���߲��������������  */
    WPHY_TOOL_MntnBoolUint16  newAntBetter;     /* �����߽Ϻñ�� */
    VOS_INT16                 dualAntImbalance;
    VOS_INT16                 sarOffsetVaule;
    VOS_UINT16                sarOffsetWeight;
    VOS_INT16                 finalRscpRfDelta;
} WPHY_TOOL_MasMntnNonblindinfoInd;

/*
 * �ṹ˵��: MAS ������ ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId;   /* �����ֶ� */
    WPHY_TOOL_MasAntennaStatus antStatus; /* ��ǰState��ǰ����TAS/MAS״̬ */
} WPHY_TOOL_MasMntnMasSwitchInd;

/*
 * �ṹ˵��: ����SAR��ز�����Ϣ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                               /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                             /* �����ֶ� */
    VOS_INT32                 sarLimitPowerMinDbm;                 /* ÿ���Ӵ��������С���ƹ���, ��λ:0.1dbm */
    VOS_UINT32                sarLimitPowerMinMw;                  /* ÿ���Ӵ��������С���ƹ���, ��λ:1/16mw */
    VOS_INT32                 sarLimitPowerMaxDbm;                 /* ÿ���Ӵ����������书��, ��λ:0.1dbm */
    VOS_UINT32                sarLimitPowerMaxMw;                  /* ÿ���Ӵ����������书��, ��λ:1/16mw */
    VOS_INT32                 sarPeriodPowerSumDbm;                /* ����100���Ӵ�SARֵ�ĺ� ��λ: 0.1dbm */
    VOS_UINT32                sarPeriodPowerSumMw;                 /* ����100���Ӵ�SARֵ�ĺ� ��λ:1/16mw */
    VOS_UINT32                sarSubWinLimitPowerMw;               /* ͨ������SAR������������ƹ���ֵ ��λ: 1/16mw */
    VOS_INT32                 sarSubWinLimitPowerDbm;              /* ͨ������SAR������������ƹ���ֵ ��λ: 0.1dbm */
    VOS_INT32                 sarAvgMaxPowerDbm;                   /* SAR���ƽ�����ʣ�������ֵ����ζ��SAR����,��λ:0.1dbm */
    VOS_UINT32                sarAvgMaxPowerMw;                    /* SAR���ƽ�����ʣ�������ֵ����ζ��SAR����,��λ:1/16mw */
    VOS_INT32                 preSarAvgMaxPowerDbm;                /* ��һ֡��sarAvgMaxPower�������͵�ǰsarAvgMaxPower�Ƚ��Ƿ����仯 ��λ:0.1dbm */
    VOS_UINT32                preSarAvgMaxPowerMw;                 /* ��һ֡��sarAvgMaxPower�������͵�ǰsarAvgMaxPower�Ƚ��Ƿ����仯 ��λ:1/16mw */
    VOS_INT32                 sarSubWinPowerSumDbm;                /* ����SARÿ���Ӵ���4֡���书��(SARֵ)�ۼӺͣ���λ:0.1dbm */
    VOS_UINT32                sarSubWinPowerSumMw;                 /* ����SARÿ���Ӵ���4֡���书��(SARֵ)�ۼӺͣ���λ:1/16mw */
    VOS_UINT16                sarSubWinFrameCount;                 /* ����SARÿ���Ӵ���֡���ۼ�,�ۼӵ�(4�ı�������NV����)����� */
    VOS_UINT16                sarSubWinFrameNum;                   /* ����SARÿ���Ӵ���֡��(4�ı���,������NV����) */
    VOS_UINT16                sarLimitWinOffset;                   /* ��¼��ǰ�Ӵ�λ�� */
    WPHY_TOOL_SwSarUpdateReasonUint16    sarUpdateReason;          /* ����SAR�㷨����ԭ�� */
} WPHY_TOOL_SwSarCtrlMntnInfo;

/*
 * �ṹ˵��: TAS5.0������ѯ�������״̬�ṹ��
 */
typedef struct {
    WPHY_TOOL_MasBlindStateMeasPara     antBlindEnergy; /* ä�вɼ��ļ������С��������Ϣ */
    WPHY_TOOL_MntnBoolUint16            fs2SampleInd;   /* fs2����ָʾ( 0:��ʾ��һ������,1:��ʾ�ǵ�һ������ ) */
    WPHY_TOOL_MntnBoolUint16            evalOverFlag;   /* StateI rx tx�����������ı�־ */
    WPHY_TOOL_MntnBoolUint16            txEvalOverFlag; /* StateI tx���������ı�־ */
    VOS_UINT16                          rsv;
} WPHY_TOOL_AntEvaInfo;

/*
 * �ṹ˵��: TAS5.0������ѯ�������״̬�ṹ��
 */
typedef struct {
    VOS_UINT16                          antStatus;      /* ʵ������״̬ */
    VOS_UINT16                          fs2AntStatus;   /* fs2����״̬ */
    VOS_UINT16                          fs2TxAnt;       /* fs2 TX״̬ */
    VOS_UINT16                          fs2PrxAnt;      /* fs2 PRX״̬ */
    WPHY_TOOL_MntnTxSignalStateUint16   txSignalState;  /* tx�������: poor��good */
    VOS_UINT16                          rsv;
    WPHY_TOOL_AntEvaInfo                fs2antEva;
} WPHY_TOOL_MntnAntEvaInfo;

/*
 * �ṹ˵��: FS2 MAS StateI ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;     /* �����ֶ� */
    WPHY_TOOL_MntnBoolUint16     leaveStateI; /* �Ƿ��뿪StateI */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;    /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState;   /* ��һState���ĸ�State */
    WPHY_TOOL_MntnBoolUint16     fs2AllEvalOverFlag; /* �������߾�������־ */
    WPHY_TOOL_MntnAntEvaInfo     currAntEvaRslt;     /* ��ǰ������Ϣ */
} WPHY_TOOL_Fs2MasMntnStateiInd;

/*
 * �ṹ˵��: MAS StateII ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;         /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;       /* �����ֶ� */
    VOS_UINT16                   roundCount;    /* ��ǰ�Ƚ����� */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;      /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState;     /* ��һState���ĸ�State */
    WPHY_TOOL_MntnBoolUint16     fs2AllEvalOverFlag; /* �������߾�������־ */
    WPHY_TOOL_MntnAntEvaInfo     fs2AntEvaList[WPHY_MNTN_FS2_MAX_ANT_NUM]; /* ��ǰ������Ϣ */
} WPHY_TOOL_Fs2MasMntnStateiiInd;

/*
 * �ṹ˵��: FS2 MAS StateIII ��ά�ɲ��ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;     /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId;   /* �����ֶ� */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;  /* ����ģʽ */
    WPHY_TOOL_MntnMasStateUint16 nextState; /* ��һState���ĸ�State */
} WPHY_TOOL_Fs2MasMntnStateiiiInd;

/*
 * �ṹ˵��: WPHY ��ά�ɲ⹦�ܿ����·��Ľṹԭ��:�������ƹ�����������
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;     /* �����ֶ� */
    VOS_UINT32                funcBitMask; /* ��ӦBitλ��λ,����Ƹù�����Ч */
} WPHY_TOOL_FuncBitControlReq;

/*
 * �ṹ˵��: ״̬��ѯ�ظ�CNF��Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32 addr;  /* �Ĵ�����ַ */
    VOS_UINT32 value; /* �Ĵ���ֵ */
} UPHY_TOOL_BbpDbgRegRd;

/*
 * �ṹ˵��: ״̬��ѯ�ظ�CNF��Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32            regRdNum;                                  /* RegList���� */
    UPHY_TOOL_BbpDbgRegRd regList[UPHY_TOOL_BBP_DBG_SAMPLE_REG_NUM]; /* REG_RD_STATUS_STRU */
} UPHY_TOOL_BbpDbgRegRdList;

/*
 * �ṹ˵��: ״̬��ѯ�ظ�CNF��Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32                omDrxSampleId; /* Ĭ��ֵ0 */
    VOS_UINT16                opid;          /* Ĭ��ֵ1 */
    VOS_UINT16                status;        /* Ĭ��ֵ0 */
    UPHY_TOOL_BbpDbgRegRdList drxRegData;    /* һ�� REG_CONFIG_LIST_STRU */
} UPHY_TOOL_BbpDbgRegRdReport;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ϱ��ṹ
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT16                  msgId; /* _H2ASN_Skip */
    VOS_UINT16                  toolsId;
    UPHY_TOOL_BbpDbgRegRdReport sampleReg; /* UPHY_TOOL_BbpDbgRegRdReport  */
} UPHY_TOOL_BbpDbgRegRdReportInd;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32 addr;  /* �Ĵ�����ַ */
    VOS_UINT32 mask;  /* ���� */
    VOS_UINT32 value; /* �Ĵ���ֵ */
} UPHY_TOOL_BbpDbgRegWr;

/*
 * �ṹ˵��: �µĲ�����ַ��������ַ����(dam, socp)�͵�ַƫ��
 */
typedef struct {
    VOS_UINT32 regAddr : 24;     /* ƫ�Ƶ�ַ */
    VOS_UINT32 chalProcType : 4; /* ͨ����������: ��Ӧ UPHY_TOOL_SampleRegCompleteUint16  */
    VOS_UINT32 regType : 4;      /* ����ַ����: ��Ӧ UPHY_TOOL_SAMPLE_BASE_ADDR_ENUM_UINT16 */
} UPHY_TOOL_NewBbpDbgRegAddr;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32             regNum;  /* RegList���� */
    UPHY_TOOL_BbpDbgRegWr *regList; /* �ṹ:REG_CONFIG_WR_STRU */
} UPHY_TOOL_BbpDbgRegWrReqList;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32            regNum;                                    /* RegList���� */
    UPHY_TOOL_BbpDbgRegWr regList[UPHY_TOOL_BBP_DBG_SAMPLE_REG_NUM]; /* �ṹ:REG_CONFIG_WR_STRU */
} UPHY_TOOL_BbpDbgRegWrList;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32 omDrxSampleId;    /* Ĭ��ֵ0,ȡֵ�ֱ�ΪBUS/DATA */
    VOS_UINT16 opid;             /* ��ʼ����=4��ֹͣ����=2����ѯ״̬=1 */
    VOS_UINT16 powrDomainBitmap; /* Ĭ��ֵ0xFFFF */
    /* 1,2,3���ţ������Ĵ�����ÿ��ṹΪREG_CONFIG_LIST_STRU */
    UPHY_TOOL_BbpDbgRegWrList drxSampleRegData[UPHY_TOOL_BBP_DBG_SAMPLE_REG_LIST_NUM];
    UPHY_TOOL_BbpDbgRegWrList drxReportRegData; /* �Ĵ����ϱ� */
    UPHY_TOOL_BbpDbgRegWrList drxStopRegData;   /* ֹͣ�Ĵ��� */
    UPHY_TOOL_BbpDbgRegWrList drxOtherRegData[UPHY_TOOL_BBP_DBG_OTHER_REG_LIST_NUM]; /* other ,��ʱ���� */
} UPHY_TOOL_BbpDbgRegWrMsg;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
        VOS_UINT16           msgId; /* _H2ASN_Skip */
    VOS_UINT16               toolsId;
    UPHY_TOOL_BbpDbgRegWrMsg sampleReg;
} UPHY_TOOL_BbpDbgRegCfg;

/*
 * �ṹ˵��: ��¼modem_Id������ģ״̬
 */
typedef struct {
    CSDR_ModeIdUint16        csdr1xModemId;     /* 1x������ModemId */
    CSDR_ModeIdUint16        csdrHrpdModemId;   /* HRPD������ModemId */
    CSDR_1X_ModeTypeUint16   csdr1xModemType;   /* 1x����ģ״̬ */
    CSDR_HRPD_ModeTypeUint16 csdrHrpdModemType; /* HRPD����ģ״̬ */
} UPHY_TOOL_CsdrModeState;

/*
 * �ṹ˵��: ��ǰCģ״̬��Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
        VOS_UINT16          msgId;          /* _H2ASN_Skip,ԭ������ */
    VOS_UINT16              toolsId;        /* �����ֶ� */
    UPHY_TOOL_CsdrModeState csdrModemState; /* Cģ��״̬��Ϣ */
} CPROC_UPHY_CsdrSetModeInfo;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ������
 */
typedef struct {
    UPHY_TOOL_MntnBoolUint16 dataFlag; /* �����Ĵ����Ŀ��ر�־ */
    VOS_UINT16               rsv;
    /* 1,2,3���ţ������Ĵ�����ÿ��ṹΪREG_CONFIG_LIST_STRU */
    UPHY_TOOL_BbpDbgRegWrList drxSampleRegData[UPHY_TOOL_BBP_DBG_SAMPLE_REG_LIST_NUM];
    UPHY_TOOL_BbpDbgRegWrList drxReportRegData;                                      /* �Ĵ����ϱ� */
    UPHY_TOOL_BbpDbgRegWrList drxStopRegData;                                        /* ֹͣ�Ĵ��� */
    UPHY_TOOL_BbpDbgRegWrList drxOtherRegData[UPHY_TOOL_BBP_DBG_OTHER_REG_LIST_NUM]; /* other ,��ʱ���� */
} UPHY_TOOL_BbpDbgRegWrListInfo;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ϱ��ṹ,�����ڿ�ά�ɲ⣬�۲�����Ĵ����Ƿ���ȷ����
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
        VOS_UINT16            msgId; /* _H2ASN_Skip */
    VOS_UINT16                toolsId;
    VOS_UINT32                omDrxSampleId;    /* Ĭ��ֵ0,ȡֵ�ֱ�ΪBUS/DATA */
    VOS_UINT16                opid;             /* ��ʼ����=4��ֹͣ����=2����ѯ״̬=1 */
    VOS_UINT16                powrDomainBitmap; /* Ĭ��ֵ0xFFFF */
    UPHY_TOOL_BbpDbgRegWrList drxSampleRegData; /* 1,2,3���ţ������Ĵ�����ÿ��ṹΪREG_CONFIG_LIST_STRU */
} UPHY_TOOL_BbpDbgRegWrInd;

/*
 * �ṹ˵��: �ض������Ĵ���������
 */
typedef struct {
    VOS_UINT32 number;                                      /* �ض��Ĵ����ĸ��� */
    VOS_UINT32 slaveAddr[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM]; /* ���ض�32���Ĵ��� */
    VOS_UINT16 chanNo[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM];    /* 0-3:��ʾCH0-CH3 */
    VOS_UINT16 regAddr[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM];   /* ��ַ */
} UPHY_TOOL_MipiReadSingleReg;

/*
 * �ṹ˵��: SSI�ض��ļĴ�����ʽ
 */
typedef struct {
    VOS_UINT32 number;                                   /* �ض��Ĵ����ĸ��� */
    VOS_UINT16 chanNo[UPHY_TOOL_MAX_SSI_READ_BACK_NUM];  /* ��ַ */
    VOS_UINT16 slaveId[UPHY_TOOL_MAX_SSI_READ_BACK_NUM]; /* Slave Id */
    VOS_UINT16 regAddr[UPHY_TOOL_MAX_SSI_READ_BACK_NUM]; /* �Ĵ��� */
} UPHY_TOOL_SsiReadSingleReg;

/*
 * �ṹ˵��: SSI�ض�����
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    rptSlot;       /* �ض�ʱ��(Ĭ��ʱ϶0) */
    VOS_UINT16                    cycleRptNum;   /* ָ��ÿ�����ڷֶλض��ĸ���, 0-64,�����ڻض�������64�� */
    VOS_UINT32                    readType;      /* 0:ָ���Ĵ��������ڷֶμĴ����ض� 1:ȫ��(256��)�Ĵ����ض� */
    VOS_UINT32                    chanNo;        /* 0-3:��ʾCH0-CH3��ֻ֧��1��ͨ���Ļض� */
    UPHY_TOOL_SsiReadSingleReg    readSingleReg; /* W�ض�����10 ms */
} UPHY_TOOL_SsiReadFunction;

/*
 * �ṹ˵��: MIPI�ض�����
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    rptSlot;       /* �ض�ʱ��(Ĭ��ʱ϶0) */
    VOS_UINT16                    reserved;
    VOS_UINT32                    readType;      /* 0:ָ�������ļĴ����ض�,����ֵԤ�� */
    UPHY_TOOL_MipiReadSingleReg   readSingleReg; /* W�ض�����2ms,G�ض�����2.3ms */
} UPHY_TOOL_MipiReadFunction;

/*
 * �ṹ˵��: ����ض�SSI/MIPI�Ĵ���
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;     /* �����ֶ� */
    UPHY_TOOL_SsiReadFunction ssiFunction; /* SSI�ض����� */
} UPHY_TOOL_SsiReadRptReq;

/*
 * �ṹ˵��: ����ض�SSI/MIPI�Ĵ���
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16  msgId;        /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                 toolsId;      /* �����ֶ� */
    UPHY_TOOL_MipiReadFunction mipiFunction; /* MIPI�ض����� */
} UPHY_TOOL_MipiReadRptReq;

/*
 * �ṹ˵��: �ϱ��ض�SSI�Ĵ���
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;                                       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                                     /* �����ֶ� */
    VOS_UINT32                errorInd;                                    /* �ض���ʱָʾ */
    VOS_UINT32                ssiNumber;                                   /* �ض��Ĵ����ĸ��� */
    VOS_UINT32                ssiRegValue[UPHY_TOOL_MAX_SSI_READ_ALL_NUM]; /* SSI�Ĵ����ض�ֵ */
} UPHY_TOOL_SsiReadRptInd;

/*
 * �ṹ˵��: �ϱ��ض�MIPI�Ĵ���
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;                                          /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;                                        /* �����ֶ� */
    VOS_UINT32                errorInd;                                       /* �ض���ʱָʾ */
    VOS_UINT32                mipiNumber;                                     /* �ض��Ĵ����ĸ��� */
    VOS_UINT32                mipiRegValue[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM]; /* MIPI�Ĵ����ض�ֵ */
} UPHY_TOOL_MipiReadRptInd;

/*
 * �ṹ˵��: SSI�ض��ϱ�����
 */
typedef struct {
    VOS_UINT16                cycleReadIndex;  /* ���ڶ�ȡ���� */
    VOS_UINT16                reserved;        /* ���� */
    UPHY_TOOL_SsiReadFunction ssiReadFunction; /* �ض����ܽṹ */
} UPHY_TOOL_SsiReadCtrl;

/*
 * �ṹ˵��: SSI/MIPI�ض�����
 */
typedef struct {
    UPHY_TOOL_SsiReadCtrl      ssiReadCtrl;   /* SSI�ض����� */
    UPHY_TOOL_MipiReadFunction mipiFunction;  /* MIPI�ض����� */
    UPHY_TOOL_SsiReadRptInd    ssiReportInd;  /* SSI�ϱ� */
    UPHY_TOOL_MipiReadRptInd   mipiReportInd; /* MIPI�ϱ� */
} WPHY_TOOL_SsiMipiReadFunction;

/*
 * �ṹ˵��: PHY �ڲ���Ϣ�ϱ���������
 */
typedef struct {
    VOS_UINT32 addr;  /* �ڴ��ַ */
    VOS_UINT32 value; /* �ڴ��ַ��ֵ */
} WPHY_TOOL_MemInfo;

/*
 * �ṹ˵��: TOOL��������·��Ķ�ȡ�ڴ�����
 */
typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16               toolsId;    /* �����ֶ� */
    WPHY_TOOL_MntnBoolUint16 enableFlag; /* ʹ�ܱ�ǣ�0��ʾȥʹ�ܣ�1��ʾʹ�� */
    VOS_UINT16               readPeriod; /* �ض����ڣ���λΪ10ms */
    VOS_UINT16               addrNum;    /* ��ַ��Ŀ */
    VOS_UINT16               reserved;
    VOS_UINT32               addrList[WPHY_MNTN_READ_ADDR_MAX_NUM]; /* �ڴ��ַ�б� */
} WPHY_TOOL_ReadMemReq;

/*
 * �ṹ˵��: PHY �ڲ���Ϣ�ϱ��ڴ���Ϣ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                addrNum; /* ��ַ��Ŀ */
    VOS_UINT16                reserved;
    WPHY_TOOL_MemInfo         addrList[WPHY_MNTN_READ_ADDR_MAX_NUM]; /* �ڴ��ַ�б� */
} WPHY_TOOL_ReadMemInd;

/*
 * �ṹ˵��: PHY �ڲ���Ϣ�ϱ���Ϣ���ݽṹ
 */
typedef struct {
    VOS_UINT32 addr;  /* �ڴ��ַ */
    VOS_UINT32 value; /* �ڴ��ַ��ֵ */
} GPHY_TOOL_MemInfo;

/*
 * �ṹ˵��: TOOL��������·��Ķ�ȡ�ڴ�����
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    VOS_UINT16                command;    /* ���0:ֹͣ��1������ */
    VOS_UINT16                readPeriod; /* �ض����ڣ���λΪ10ms */
    VOS_UINT16                addrNum;    /* ��ַ��Ŀ */
    VOS_UINT16                reserved;
    VOS_UINT32                addrList[GPHY_MNTN_READ_ADDR_MAX_NUM]; /* �ڴ��ַ�б� */
} GPHY_TOOL_ReadMemReq;

/*
 * �ṹ˵��: PHY �ڲ���Ϣ�ϱ��ڴ���Ϣ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                addrNum; /* ��ַ��Ŀ */
    VOS_UINT16                reserved;
    GPHY_TOOL_MemInfo         addrList[GPHY_MNTN_READ_ADDR_MAX_NUM]; /* �ڴ��ַ�б� */
} GPHY_TOOL_ReadMemInd;

/*
 * �ṹ˵��: WPHY����ϵͳ״̬��ά�ɲ��ϱ�����
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 freeMntnRptStatus;  /* FREE̬ά���ϱ�ȥʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 idleMntnRptStatus;  /* IDLE̬ά���ϱ�ȥʹ�ܱ�ǣ�����PI��ӡ�� */
    UPHY_TOOL_MntnRptStatusUint16 fachMntnRptStatus;  /* FACH̬ά���ϱ�ȥʹ�ܱ�ǣ��������������Ϣ�� */
    UPHY_TOOL_MntnRptStatusUint16 dchMntnRptStatus;   /* DCH̬ά���ϱ�ȥʹ�ܱ�ǣ��������������Ϣ�� */
    UPHY_TOOL_MntnRptStatusUint16 slaveMntnRptStatus; /* SLAVEά���ϱ�ȥʹ�ܱ�� */
    VOS_UINT16                    rserved;
} WPHY_TOOL_SysStateMntnRptCtrl;

/*
 * �ṹ˵��: WPHY����ģ���ϱ���ά�ɲ����
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 csRptStatus;          /* С������ά���ϱ�ʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 drxSchdlRptStatus;    /* FACH̬ά���ϱ�ʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 olpcRptStatus;        /* DCH̬ά���ϱ�ʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 cmRptStatus;          /* SLAVEά���ϱ�ʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 softDemSyncRptStatus; /* ���ͬʧ����ά���ϱ�ʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 abnormalRptStatus;    /* WPHY time outά���ϱ�ʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 ant2VoteStatus;       /* �ּ�˫����ͶƱ�ϱ�ʹ�ܱ�� */
    VOS_UINT16                    reserved;
} WPHY_TOOL_ModuleMntnRptCtrl;

/*
 * �ṹ˵��: TOOL��������·��Ķ�ȡ�ڴ�����
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */                                /* ԭ������ */
    VOS_UINT16                    toolsId;           /* �����ֶ� */
    UPHY_TOOL_MntnRptStatusUint16 intraMsgRptStatus; /* �ڲ���Ϣ�ϱ���ǣ�1��ʾ�ϱ���0��ʾ���ϱ� */
    VOS_UINT16                    reserved;
    /* ����״̬�����ϱ�ά���ӡ���ƣ��ڲ�����0Ϊ���ϱ���1Ϊ�����ϱ���Ĭ���ϱ� */
    WPHY_TOOL_SysStateMntnRptCtrl sysStateRptStatusCtrl;
    /* ����ģ���ϱ�ά���ӡ���ƣ��ڲ�����1Ϊ�����ϱ���0Ϊ���ϱ���Ĭ�ϲ��ϱ� */
    WPHY_TOOL_ModuleMntnRptCtrl moduleRptStatusCtrl;
} WPHY_TOOL_MntnFuncSwtichReq;

/*
 * �ṹ˵��: PHY DCXO��Ϣ�ϱ�
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16     msgId;           /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    toolsId;         /* �����ֶ� */
    UPHY_TOOL_MntnDcxoEventUint16 dcxoEventType;   /* ״̬ */
    VOS_INT16                     xoTemp;          /* Xo�¶�,0.1�� */
    VOS_UINT16                    rcmPpmValidFlag; /* RCM�����Ƿ���Ч */
    VOS_UINT16                    reserved;        /* ���� */
    VOS_UINT32                    transValue;      /* ��ӡֵ */
    VOS_INT32                     rcmPpm;          /* RCMд��PHY��ֵ */
    VOS_INT32                     wcdmaPpmMntn;    /* W ppmά��ֵ */
    VOS_INT32                     gsmPpmMntn;      /* G ppmά��ֵ */
    VOS_UINT32                    modem0RptValue;  /* PHY�ϱ���RCM��ֵ */
    VOS_UINT32                    modem1RptValue;  /* PHY�ϱ���RCM��ֵ */
    VOS_UINT32                    modem2RptValue;  /* PHY�ϱ���RCM��ֵ */
} UPHY_TOOL_MntnDcxoStatus;

/*
 * �ṹ˵��: �����ϱ�GSDR ����״̬��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER      /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */   /* ԭ��ID */
    VOS_UINT16 toolId;  /* �����ֶ� */
    VOS_UINT16 command; /* ���0:ֹͣ��1������ */
    VOS_UINT16 reserved;
} GPHY_TOOL_GsdrStateReq;

/*
 * �ṹ˵��: �ϱ�GSDR ����״̬��Ϣ
 */
typedef struct {
    VOS_UINT16 msgId;     /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;    /* �����ֶ� */
    VOS_UINT32 curFn;     /* ��ǰ��֡�� */
    VOS_UINT16 gsdrState; /* GSDR��ǰ���е�״̬ */
    VOS_UINT16 reserved;
} GPHY_TOOL_GsdrStateInd;

/*
 * �ṹ˵��: GPHY ǿ������GSDR
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                     toolsId;          /* �����ֶ� */
    UPHY_TOOL_MntnCtrlStatusUint16 forceCtrlGsdrReq; /* �Ƿ�ǿ�������͹ر�Gsdr��0:Ĭ�ϲ��򿪣�1:ǿ�ƴ򿪣�2:ǿ�ƹر� */
    VOS_UINT16                     reserved;
} GPHY_TOOL_GsdrForcrCtrlReq;

/*
 * �ṹ˵��: �ϱ�gsdr��ͬ����ά����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;

    /*
     * ʹ�ÿ�����ulSwitchCtrl��ʹ��GSDRĳЩά��ģ��
     * FUNC_GSDR_NB_PARA_ENABLE_FLAG       GSDR_BIT0    --ʹ�ܴ�ӡNB�����·�/�ϱ�����
     * FUNC_GSDR_NB_DUMP_ENABLE_FLAG       GSDR_BIT1    --ʹ�ܴ�ӡNB DUMP��������
     * FUNC_GSDR_SB_PARA_ENABLE_FLAG       GSDR_BIT2    --ʹ�ܴ�ӡSB�����·�/�ϱ�����
     * FUNC_GSDR_SB_DUMP_ENABLE_FLAG       GSDR_BIT3    --ʹ�ܴ�ӡSB DUMP��������
     */
    VOS_UINT32 switchCtrl;
} UPHY_TOOL_GsdrMntnReq;

/*
 * �ṹ˵��: ERACH�¿�ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    WPHY_TOOL_MntnErachCtrlStateUint16 erachCtrlState; /* ����״ָ̬ʾ */
    /* ԭ�����״̬��������MACI-CFGԭ�MACI-CFGԭ��״ֱ̬�ӵ�ԭ��ַ�ģ���ȡ */
    WPHY_TOOL_MntnErachPrimitiveTypeUint16 primitiveType;
    UPHY_TOOL_MntnBoolUint16            macINewCfgFlag;     /* MACI�������ã�true��Ч */
    UPHY_TOOL_MntnBoolUint16            rgchExistFlag;      /* �Ƿ����RGCH�ŵ� */
    UPHY_TOOL_MntnBoolUint16            implicitReleaseInd; /* ��ʽ�ͷű�־��true��Ч */
    UPHY_TOOL_MntnBoolUint16            explicitReleaseInd; /* ��ʽ�ͷű�־��true��Ч */
    UPHY_TOOL_MntnBoolUint16            sgConfigInd;        /* 0:ָʾstSGConfig��Ч;1:ָʾstSGConfig��Ч */
    WPHY_TOOL_MntnUlEfachLochTypeUint16 eFachLochType;      /* [0x1,0x2], 0x1:CCCH; 0x2:DCCH/DTCH */

    /* �ü�����ʵ��Additional E-DCH transmission back off + Tdelay - 1��ʱ����������ÿTTI����һ�� */
    VOS_UINT16 tAddBackOffCount;
    /* �ü�����ʵ��Maximum E-DCH resource allocation for CCCH��ʱ����������ÿTTI����һ�� */
    VOS_UINT16 tMaxAllocCcchCount;
    /* �ü�����ʵ��Maximum period for collision resolution phase��ʱ����������ÿTTI����һ�� */
    VOS_UINT16 tMaxPrdCollisionCount;
    VOS_UINT16 tbCount;               /* �ü�����ʵ��Tb��ʱ����������ÿTTI����һ�� */

    VOS_UINT16                       signatureIdx;
    WPHY_TOOL_MntnRachRsltUint16     rachRslt;       /* ���������ö�� */
    VOS_UINT16                       prachReTranNum; /* �ش����� */
    VOS_UINT16                       aiSigNum;       /* AIǩ������ */
    VOS_UINT16                       eaiSigNum;      /* EAIǩ������ */
    WPHY_TOOL_MntnErachEaiTypeUint16 eaiHardValue;   /* EAIӲֵ */
    VOS_INT16                        currSlotPower;  /* ��ǰ���һ��preamble�ķ��书�� */
    VOS_INT16                        initPower;      /* ��ʼ���빦�� */
    VOS_UINT16                       aiSlotNum;      /* AI����ʱ϶�� */

    VOS_INT16                sysChangeInd;        /* С����ѡ/Ӳ�л�ָʾ��־ */
    UPHY_TOOL_MntnBoolUint16 timeUpdateDelayFlag; /* ERACH�¶�ʱ�ӳٸ��±�� */
    VOS_INT16                fdpchChipOffset;     /* ֻ����dpch��sccpch��pich */

    VOS_UINT32 bbpDmodeSyncInd; /* ��ǰ֡���ͬʧ��״̬ */
    VOS_UINT16 dmodeSyncInd;    /* ��ǰ֡���ͬʧ��״̬ */
    VOS_UINT16 frameSyncInd;    /* ��ǰ֡ͬʧ��״̬ */

    WPHY_TOOL_TxPowInfo txPow; /* ���з��书�� */

    VOS_UINT32 erachChEn; /* Erach����ŵ�ʹ��״̬ */
} WPHY_TOOL_ErachMntnRpt;

/*
 * �ṹ˵��: WTCģʽ��ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    VOS_UINT16               wtcMode; /* WTCģʽ��ʶ */
    VOS_UINT16               modemId;
    WPHY_TOOL_MntnBoolUint16 bbpClkCloseFlag;
    WPHY_TOOL_MntnBoolUint16 masterModeFlag;
    WPHY_TOOL_MntnBoolUint16 tdWorkFlag;
    WPHY_TOOL_MntnBoolUint16 cdmaWorkFlag;
} WPHY_TOOL_WtcModeRpt;

/*
 * �ṹ˵��: �ϱ�gsdr�ж��Ƿ���/�رյ�ͳ������
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId;     /* �����ֶ� */
    GPHY_TOOL_MntnGsdrJudgeTypeUint16 judgeType;   /* �ж������͹ر�Gsdr 1:�ж��Ƿ�� 2:�ж��Ƿ�ر� */
    VOS_UINT16                        rssiSum;     /* ����RSSI���޵�SUB����  */
    VOS_UINT16                        rssiTotal;   /* ͳ��RSSI��SUB֡����  */
    VOS_UINT16                        ciSum;       /* ����CI���޵�SUB����  */
    VOS_UINT16                        ciTotal;     /* ͳ��CI��SUB֡���� */
    VOS_UINT16                        subQualSum;  /* ����QUAL���޵�SUB����  */
    VOS_UINT16                        fullQualSum; /* ����QUAL���޵�FULL����  */
    VOS_UINT16                        rssiReason;  /* ͳ��CI��SUB֡���� */
    VOS_UINT16                        ciReason;    /* ����QUAL���޵�SUB����  */
    VOS_UINT16                        qualReason;  /* ����QUAL���޵�FULL����  */
} GPHY_TOOL_GsdrJudgeInfoInd;

/*
 * �ṹ˵��: �ϱ�BCH̬GSDR�ж��Ƿ���/�رյ�ͳ������
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */               /* ԭ������ */
    VOS_UINT16 toolsId;             /* �����ֶ� */
    VOS_UINT16 curDecodeType;       /* ��ǰ�������� */
    VOS_UINT16 expectDecodeType;    /* ������������ */
    VOS_INT16  mainAntRssiValue;    /* NB����RSSIֵ  */
    VOS_INT16  divAntRssiValue;     /* NB˫���߽��ս�СRSSIֵ  */
    VOS_UINT16 pber;                /* NB����PBERֵ */
    VOS_UINT16 awgnFlag;            /* NB���볡�����AWGN */
    VOS_UINT16 saicToDaicRuleFlag;  /* ����QUAL���޵�SUB����  */
    VOS_UINT16 isaicToDaicRuleFlag; /* ����QUAL���޵�FULL����  */
    VOS_UINT16 isaicOffRuleFlag;    /* ͳ��CI��SUB֡���� */
    VOS_UINT16 daicOffRuleFlag;     /* ����QUAL���޵�SUB����  */
    VOS_INT16  bchDaicOnRssiThreshold;   /* BCH̬���о�����DACI��RSSI���� */
    VOS_UINT16 bchDaicOnPberThreshold;   /* BCH̬���о�����DACI��PBER���� */
    VOS_INT16  bchDaicOffRssiThreshold;  /* BCH̬���о��ر�DACI��RSSI���� */
    VOS_INT16  bchIsaicOffRssiThreshold; /* BCH̬���о��ر�ISAIC��RSSI���� */
} GPHY_TOOL_GsdrJudgeBchInfoInd;

/*
 * �ṹ˵��: ����MRX PD�ϱ�
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;           /* �����ֶ� */
    VOS_INT16                 mrxGetPower;       /* ��ȡ�Ĺ���0.1dB */
    VOS_UINT16                mrxPowerLinear;    /* ��ȡ�Ĺ���,����ֵ */
    VOS_INT16                 mrxGainValue;      /* mrx Ĭ������ */
    VOS_INT16                 gmskMrxGainOffset; /* GMSK mrx ����ƫ�� */
    VOS_INT16                 edgeMrxGainOffset; /* 8PSK mrx ����ƫ�� */
    VOS_UINT16                phyState;          /* �����״̬ */
} GPHY_MRX_PdReport;

/*
 * �ṹ˵��: SB�����ṹ��
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16 toolsId; /* �����ֶ� */
    VOS_UINT32 sbDumpDataEven[GPHY_TOOL_GSDR_SB_BURST_DATA_LEN];
    VOS_UINT32 sbDumpDataOdd[GPHY_TOOL_GSDR_SB_BURST_DATA_LEN];
} GPHY_TOOL_GsdrDumpSbDataInd;

/*
 * �ṹ˵��: EDRXģ��ԭ�ｨ�����ͷ�EDRX
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    WPHY_TOOL_MntnT321CtrlStatusUint16 t321Status;   /* T321״̬ */
    WPHY_TOOL_MntnT321CtrlCauseUint16  t321CtrlCuse; /* T321״̬�仯��ԭ�� */
} WPHY_TOOL_EdrxMntnT321CtrlReq;

/*
 * �ṹ˵��: EDRXģ��ԭ�ｨ�����ͷ�EDRX
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    VOS_UINT16 eraStatus;
    VOS_UINT16 r99RaStatus;
    VOS_UINT16 pccpchSib7Status;
    VOS_UINT16 pccpchOtherStatus;
    VOS_UINT16 intraTimeMeasStatus;
    VOS_UINT16 burstStatus;
    VOS_UINT16 intraMeasStatus;
    VOS_UINT16 gsmBsicStatus;
    VOS_UINT16 interMeasStatus;
    VOS_UINT16 gsmRssiStatus;
    VOS_UINT16 lteMeasStatus;
    VOS_UINT16 reserve; /* �����ֶ� */
} WPHY_TOOL_EdrxMntnRpt;

/*
 * �ṹ˵��: G���ع����쳣��ӡ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16       msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                      toolsId;     /* �����ֶ� */
    GPHY_TOOL_MntnTimeOutTypeUint16 timeOutCase; /* �����쳣���� */
    VOS_UINT16                      reserved;    /* �����ֶ� */
} GPHY_TOOL_TimeOutRptInd;

/*
 * �ṹ˵��: ��ϵͳ�Ż���ӡ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    VOS_UINT16 timerState; /* ��ʱ��״̬ */
    VOS_UINT16 reserved1;
    VOS_UINT32 countFnNum; /* ��ʱ����Ҫ����֡ */
    VOS_UINT32 timeOutNum; /* ��ʱ����ʱ���� */
    VOS_UINT32 startFn;    /* ��ʱ������֡�� */
    VOS_UINT32 passFnCnt;  /* ��ʱ��������֡�ſ�ʼ�Ѿ������˶���֡ */

    /* BIT0: LTE; BIT1: WCDMA; BIT2: TDS; ��ӦBITλΪ1��ʾδ��ɵ�ǰһ�ֲ�����Ϊ0��ʾ�����ڸ�ģʽ��δ��� */
    VOS_UINT16 interratMeasFlg;
    /* BIT0: LTE; BIT1: WCDMA; BIT2: TDS; ��ӦBITλΪ1��ʾ��RCM��������Դ��Ϊ0��ʾδ������Դ��δ�ͷ� */
    VOS_UINT16 requestedRfFlg;
    VOS_UINT16 allMeasState;     /* ������ϵͳ�����������һ�֡� PHY_TURE: ���һ��; PHY_FALSE: ������ϵͳ����δ��� */
    VOS_UINT16 measTimeOutCnt;   /* ���һ��������ϵͳ��������Ҫ��4.5s���ڸ��� */
    VOS_UINT16 forcedRankEnable; /* �Ƿ���Ҫǿ��̧����ϵͳ�������ȼ���PHY_TRUE:��Ҫ; PHY_FALSE: ����Ҫ */
    VOS_UINT16 forcedRankGaps;   /* ����ǿ��̧�����ȼ����� */
    VOS_UINT16 newGapsStart;     /* ��ĳһ����ϵͳ�ķֹ��Ƿ���ɡ�PHY_TRUE: �ֹ����; PHY_FALSE: ��Ҫ�����ֹ� */
    VOS_UINT16 lastInterratMeas; /* ��һ�ηֹ�����ϵͳ, 1: LTE; 2:WCDMA; 4:TDS */
    VOS_UINT16 restartGapTime;   /* ����һ�ֲ����ļ������λ: ֡��������Դ��������ͷ� */
    VOS_UINT16 reserved2;

    VOS_UINT32 phyCtuState; /* Gд��CTU����ģ״̬ */
    VOS_UINT32 phyTaskFlag; /* ���ڵ�task���� */

    VOS_UINT16 bgLSlaveTaskState; /* L����������ִ��״̬ */
    VOS_UINT16 bgWSlaveTaskState; /* W����������ִ��״̬ */
    VOS_UINT16 lMeasRunState;     /* L����״̬ */
    VOS_UINT16 wMeasRunState;     /* W����״̬ */
} GPHY_TOOL_InterratOptimizationInt;

/*
 * �ṹ˵��: W��ʱ�ϱ���ӡ
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16       msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                      toolsId;     /* �����ֶ� */
    WPHY_TOOL_MntnTimeOutTypeUint16 timeOutCase; /* �����쳣���� */
    VOS_UINT16                      reserved;    /* �����ֶ� */
} WPHY_TOOL_TimeOutRptInd;

/*
 * �ṹ˵��: D̬��Ƶƫ������AFC Resetά���ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;             /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                        toolsId;           /* �����ֶ� */
    VOS_UINT16                        freqM;             /* dch̬AFC RESET��Ҫ����Ĵ������޵Ĵ��� */
    VOS_UINT16                        freqThresh;        /* dch̬AFC RESET��Ҫ�����Ƶƫ���ޣ���λKHz */
    VOS_INT32                         ppmThresh;         /* ��Ƶƫ����ת���ɵ�ppm���� */
    VOS_INT16                         temperatureTh;     /* dch̬AFC RESET��Ҫ������¶����ޣ���λ0.1���϶� */
    VOS_UINT16                        afcAboveThreshCnt; /* �������޵ļ������ﵽuhwFreqMʱ������AFC */
    VOS_UINT16                        afcFrameCnt;       /* ֡������ */
    VOS_INT16                         oldRxFreq;         /* ��ǰƵ�㲻��ԭƵ��ʱ��˵���������л� */
    VOS_INT32                         basePpm;           /* ��ҪAFC����ʱ����AFC��Ϊ�����baseֵ */
    VOS_INT32                         currentPpm;        /* ��ǰƵ����Ppm��ֵ */
    WPHY_MNTN_DchAfcResetStatusUint16 afcResetStatus;    /* AFC RESET��������״̬ */
    VOS_UINT16                        reserved;          /* ����λ */
} WPHY_TOOL_DchAfcResetRpt;

/*
 * �ṹ˵��: RCM DCXO��ά�ɲ��ϱ�����
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 dcxoRptStatus;      /* DCXOά���ϱ�ȥʹ�ܱ�� */
    UPHY_TOOL_MntnRptStatusUint16 phySampleRptStatus; /* PHY������ά���ϱ�ȥʹ�ܱ�� */
} RCM_TOOL_DcxoMntnRptCtrl;

/*
 * �ṹ˵��: RCM SCHDL��ά�ɲ��ϱ�����
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 schdlRptStatus; /* SCHDLά���ϱ�ȥʹ�ܱ�� */
    VOS_UINT16                    reserved;       /* �����ֶ� */
} RCM_TOOL_SchdlMntnRptCtrl;

/*
 * �ṹ˵��: RCM TAS��ά�ɲ��ϱ�����
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 tasRptStatus; /* TASά���ϱ�ȥʹ�ܱ�� */
    VOS_UINT16                    reserved;     /* �����ֶ� */
} RCM_TOOL_TasMntnRptCtrl;

/*
 * �ṹ˵��: RCM ��ά�ɲ��ϱ�����
 */
typedef struct {
    RCM_TOOL_DcxoMntnRptCtrl  dcxoMntnRptCtrl;  /* DCXO�����ϱ�ά���ӡ���ƣ��ڲ�����0Ϊ���ϱ���1Ϊ�����ϱ���Ĭ���ϱ� */
    /* SCHDL�����ϱ�ά���ӡ���ƣ��ڲ�����0Ϊ���ϱ���1Ϊ�����ϱ���Ĭ���ϱ� */
    RCM_TOOL_SchdlMntnRptCtrl schdlMntnRptCtrl;
    RCM_TOOL_TasMntnRptCtrl   tasMntnRptCtrl;   /* TAS�����ϱ�ά���ӡ���ƣ��ڲ�����0Ϊ���ϱ���1Ϊ�����ϱ���Ĭ���ϱ� */
} RCM_TOOL_MntnRptCtrl;

/*
 * �ṹ˵��: TOOL��RCM�·��Ŀ�����Ϣ������RCM DCXOά����Ϣ�ϱ���
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16           toolsId; /* �����ֶ� */
    RCM_TOOL_MntnRptCtrl rcmMntnRptCtrl;
} RCM_TOOL_MntnFuncSwtichReq;

/*
 * �ṹ˵��: RCM TAS�þ�����ϱ��ṹ
 */
typedef struct UPHY_MNTN_RCM_TAS_ARBITRATION_RSLT {
    RCM_TOOL_MntnModemStateUint16 modemState[RCM_TOOL_MNTN_MODEM_SUPPORT_NUM]; /* M0~M2��Modem״̬ */
    RCM_TOOL_MntnActionUint16     tasAction;                                   /* TAS�л��ж� */
} RCM_TOOL_MntnTasArbitrationRslt;

/* EASYRF ������Ϣ�ṹ */
/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32            regNum;                                       /* RegList���� */
    UPHY_TOOL_BbpDbgRegWr regList[UPHY_TOOL_EASYRF_DBG_SAMPLE_REG_NUM]; /* �ṹ:REG_CONFIG_WR_STRU */
} UPHY_TOOL_EasyrfDbgRegWrList;

/*
 * �ṹ˵��: ״̬��ѯ�ظ�CNF��Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32            regRdNum;                                     /* RegList���� */
    UPHY_TOOL_BbpDbgRegRd regList[UPHY_TOOL_EASYRF_DBG_SAMPLE_REG_NUM]; /* REG_RD_STATUS_STRU */
} UPHY_TOOL_EasyrfDbgRegRdList;

/*
 * �ṹ˵��: ״̬��ѯ�ظ�CNF��Ϣ�ṹ
 */
typedef struct {
    VOS_UINT32                   omDrxSampleId; /* Ĭ��ֵ0 */
    VOS_UINT16                   opid;          /* Ĭ��ֵ1 */
    VOS_UINT16                   status;        /* Ĭ��ֵ0 */
    UPHY_TOOL_EasyrfDbgRegRdList drxRegData;    /* һ�� REG_CONFIG_LIST_STRU */
} UPHY_TOOL_EasyrfDbgRegRdReport;

/*
 * �ṹ˵��: ���߼Ĵ���������Ϣ�ϱ��ṹ
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    VOS_UINT16                 msgId; /* _H2ASN_Skip */
    VOS_UINT16                     toolsId;
    UPHY_TOOL_EasyrfDbgRegRdReport sampleReg; /* UPHY_TOOL_EasyrfDbgRegRdReport  */
} UPHY_TOOL_EasyrfDbgRegRdReportInd;

/*
 * �ṹ˵��: TOOL-RFM��������ԭ����Ϣ
 */
typedef struct {
    VOS_UINT32               sampleId;    /*  Ĭ��ֵ0,ȡֵ�ֱ�ΪBUS/DATA */
    BBA_DBG_ActionUint16     debugAction; /*  ��������:0:����;1:��ѯ;2��ֹͣ */
    EASYRF_DBG_CfgTypeUint16 debugType;   /*  �������� */
    /*
     * ģ���ڶ������ñ�, ��ά��UPHY_TOOL_EASYRF_DBG_REG_LIST_NUM��ÿ��������ݸ�ʽ����
     * |-regNum(VOS_UINT32)-|-regNum����������(UPHY_TOOL_BbpDbgRegWr)-|
     */
    VOS_UINT32 drxSampleRegData[1];
} UPHY_TOOL_EasyrfDbgRegWrMsg;

/*
 * �ṹ˵��: TOOL-RFM��������ԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT16              msgId; /* _H2ASN_Skip */
    VOS_UINT16                  toolsId;
    UPHY_TOOL_EasyrfDbgRegWrMsg sampleReg;
} UPHY_TOOL_EasyrfDbgReq;

/*
 * �ṹ˵��: TOOL-RFM��������ظ�ԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    EASYRF_TOOL_MntnMsgIdUint16 msgID;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16               toolsId; /* �����ֶ� */
    UPHY_TOOL_MntnRsltUint16 result;  /* ����ԭ��Ĵ�����ID */
    VOS_UINT16               rsv;     /* �����ֶ� */
} UPHY_TOOL_EasyrfDbgCnf;

/*
 * �ṹ˵��: PS�ŵ������ϱ�����
 */
typedef struct {
    VOS_MSG_HEADER
    GPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    UPHY_TOOL_MntnBoolUint16  enableFlag; /* ʹ�ܿ��� */
    VOS_UINT16                reserved;   /* �����ֶ� */
} TOOL_GPHY_PsChanqualReportReq;

/*
 * �ṹ˵��: PS������Ϣ�ϱ�
 */
typedef struct {
    VOS_MSG_HEADER
    GPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId;    /* �����ֶ� */
    UPHY_TOOL_MntnBoolUint16  enableFlag; /* ʹ�ܿ��� */
    VOS_UINT16                reserved;   /* �����ֶ� */
} TOOL_GPHY_PsUlRptReq;

/*
 * �ṹ˵��: ��ӡά���ͼ���
 */
typedef struct {
    VOS_UINT16 logPoint; /* *< Log point identifier */
    VOS_UINT16 logLevel; /* *< Log level setting */
} EASYRF_OM_LogPoint;

/*
 * �ṹ˵��: ��ӡ�������ò���
 */
typedef struct {
    VOS_UINT8          numLogPoints; /* *< number of log points, 1..RFHAL_OM_MAX_LOG_POINTS */
    VOS_UINT8          reserved[3];
    EASYRF_OM_LogPoint logPoint[EASYRF_OM_MAX_LOG_POINTS];
    /* *< log point setting */
} EASYRF_LOG_LevelPara;

/*
 * �ṹ˵��: �Ĵ����ض����ò���
 */
typedef struct {
    EASYRF_OM_SwitchUint16  switchOn; /* 0 ȥʹ�ܼĴ����ض���1 ʹ�ܼĴ����ض� */
    EASYRF_OM_ReadTrxUint16 isTrx;    /* 0 �ض�TX, 1 �ض�RX, 2 tx/rx���ض� */
    VOS_UINT8               regNum;   /* �ض��Ĵ�������, 1->8 */
    VOS_UINT8               reserved[3];
    VOS_UINT32              regAddr[EASYRF_OM_MAX_REG_READ_NUM]; /* �ض���rfdsp�Ĵ�����ַ */
} EasyrfRegReadPara;

/*
 * �ṹ˵��: PD OM���ò���
 */
typedef struct {
    EASYRF_OM_SwitchUint16   omSwitchFlag; /* 0 :PD OMԭ�￪�عرգ�1:PD OMԭ�￪�ش� */
    EASYRF_PD_OmSwitchUint16 omEnFlag;     /* 0 PD OM�ر�, 1 PD OM�� */
    EASYRF_PD_ModeUint16     mode;         /* PD MODE���ƴ�׮ */
    VOS_UINT16               rsvd;
} EasyrfPdOmReq;

/*
 * �ṹ˵��: UL TIMING�������ò���
 */
typedef struct {
    EASYRF_OM_SwitchUint16  omSwitchFlag;   /* 0 :TIMINGԭ�￪�عرգ�1:TIMINGԭ�￪�ش� */
    EASYRF_OM_RatmodeUint16 ratMode;        /* RAT����ģʽ */
    VOS_UINT16              bandExtendFlag; /* band��չ��־:0:����չ��1:��չ */
    VOS_UINT16              band;           /* band */
    VOS_UINT16              timingNvId;     /* NVID */
    VOS_UINT16              rsvd[3];        /* Ϊ��64 bit���� */
} EasyrfUlTimingUpdateReq;

/*
 * �ṹ˵��: ���й���������Ϣ�ϱ�
 */
typedef struct {
    VOS_UINT16 msgId;              /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;             /* tools id */
    VOS_UINT16 txPowerLimitStatus; /* ���й�������״̬ */
    VOS_UINT16 reserved;           /* �����ֶ� */
} GPHY_TOOL_TxPowerStatusInd;

/*
 * �ṹ˵��: ��ǿ������bsicʧ����Ϣ�ϱ�
 */
typedef struct {
    VOS_UINT16 msgId;          /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16 toolId;         /* tools id */
    VOS_UINT16 arfcn;          /* Ƶ�� */
    VOS_UINT16 rxlev;          /* ���� */
} GPHY_TOOL_StrongestNcellInd;

/*
 * �ṹ˵��: TOOL-BBA��ӡ����ָʾԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16           toolsId; /* �����ֶ� */
    EASYRF_LOG_LevelPara para;
} UPHY_TOOL_EasyrfLogLevelCfgReq;

/*
 * �ṹ˵��: TOOL-BBA�Ĵ����ض�ԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16        toolsId; /* �����ֶ� */
    EasyrfRegReadPara regReadPara;
} UphyToolEasyrfRegReadReq;

/*
 * Name: RfhalOmAfeAlwaysOnParaStru
 * Description: request of afe&pll always open through HIDS.
 */
typedef struct {
    EASYRF_OM_SwitchUint16 switchOn;        /* 0:���ܹرգ�1:���ܴ� */
    EASYRF_OM_SwitchUint16 isRxPllAlwaysOn; /* 0: pll������, 1: pll���� */
    EASYRF_OM_SwitchUint16 isRxAfeAlwaysOn; /* 0: afe������, 1: afe���� */
    EASYRF_OM_SwitchUint16 isTxPllAlwaysOn; /* 0: pll������, 1: pll���� */
    EASYRF_OM_SwitchUint16 isTxAfeAlwaysOn; /* 0: afe������, 1: afe���� */
    VOS_UINT16             reserved;
} EasyrfAfeAlwaysOnPara;

/*
 * �ṹ˵��: TOOL->BBA RX AFE����ԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16            toolsId; /* �����ֶ� */
    EasyrfAfeAlwaysOnPara afeAlwaysOnPara;
} UphyToolEasyrfAfeOnReq;

/*
 * �ṹ˵��: ��Ƶ����������Ϣ
 */
typedef struct {
    VOS_UINT8  iqDumpContinue;  /* ����������־,1��ʾÿ��AGCԭ���Զ�����,����Ҫ�ٴ���λ,0��ʾֻ��һ�� */
    VOS_UINT8  iqDumpChanSel;   /* DUMP����ͨ��ѡ�� */
    VOS_UINT8  iqDumpNodeSel;   /* DUMP�����ڵ�ѡ�� */
    VOS_UINT8  iqDumpChanType;  /* DUMP����ʱҵ������ѡ��,��Ҫ����������,��������PRACH/PUCCH/PUSCH��ͬ�ŵ� */
} EASYRF_OM_TrxDumpPara;

/*
 * �ṹ˵��: ��Ƶ����������Ϣ
 */
typedef struct {
    VOS_UINT16              txDumpEn;     /* Tx DUMP����ʹ�ܿ���λ */
    VOS_UINT16              rxDumpEn;     /* Rx DUMP����ʹ�ܿ���λ */
    EASYRF_OM_TrxDumpPara   iqRxDumpPara; /* RX DUMP�������Ʋ��� */
    EASYRF_OM_TrxDumpPara   iqTxDumpPara; /* TX DUMP�������Ʋ��� */
} EASYRF_OM_TrxDumpCtrlPara;

/*
 * �ṹ˵��: ��ƵAFE/Pll�������Ʋ���
 */
typedef struct {
    VOS_UINT8 isAfeAlwaysOn; /* 6370AFE�������� */
    VOS_UINT8 isPllAlwaysOn; /* 6370PllBuffEn�������� */
    VOS_UINT8 rsv1;          /* rsv1 */
    VOS_UINT8 rsv2;          /* rsv2 */
} EASYRF_OM_TrxAnalogPara;

/*
 * �ṹ˵��: ��ƵAFE/Pll��������
 */
typedef struct {
    VOS_UINT16               txOnEn;   /* TX AFE/PLL��������ʹ�� */
    VOS_UINT16               rxOnEn;   /* RX AFE/PLL��������ʹ�� */
    EASYRF_OM_TrxAnalogPara  rxOnPara; /* RX AFE/PLL�������Ʋ��� */
    EASYRF_OM_TrxAnalogPara  txOnPara; /* TX AFE/PLL�������Ʋ��� */
} EASYRF_OM_TrxAnaCtrlPara;

/*
 * �ṹ˵��: ��ģ��Cycleͳ�ƽṹ
 */
typedef struct {
    EASYRF_OM_SwitchUint16    switchOn;
    VOS_UINT16                rsv;
    EASYRF_OM_TrxAnaCtrlPara  trxAnaCtrl;  /* 6370 ģ�ⳣ������ */
    EASYRF_OM_TrxDumpCtrlPara dfeDumpCtrl; /* 6370 DFE�������� */
} EASYRF_OM_TrxStubCtrlPara;

/*
 * �ṹ˵��: TOOL->BBA RX AFE����ԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16            toolsId; /* �����ֶ� */
    EASYRF_OM_TrxStubCtrlPara hfTrxCtrl;
} UphyToolEasyrfHfTrxCtrlReq;

/*
 * Name: EasyrfTxApcParaStubStru
 * Description: tx apc code&pa mode stub through HIDS.
 */
typedef struct {
    EASYRF_OM_SwitchUint16 switchOn; /* 0:���ܹرգ�1:���ܴ� */
    EASYRF_OM_SwitchUint16 stubMode; /* 0:���ʴ�׮��1:code��׮ */
    /*
     * *< Tx transmit power, G/W/C:0.1dbm L/NR:1/8dbm. For NR,
     * if Power<=-100dbm, directly control minimum apc for saving rfdsp cycle
     */
    VOS_INT16                  txPower;
    EasyrfOmTxPaModeEnumUint16 paMode;  /* pa mode */
    VOS_UINT32                 apcCode; /* RFGAIN������ */
} EasyrfTxApcParaStub;

/*
 * �ṹ˵��: TOOL->BBA TX apc��׮ԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16          toolsId; /* �����ֶ� */
    EasyrfTxApcParaStub txApcStubPara;
} UphyToolEasyrfTxApcStubReq;

/*
 * Name: EasyrfGpioStatusReadParaStru
 * Description: request of read rfdsp register(s) through HIDS.
 */
typedef struct {
    EASYRF_OM_SwitchUint16  switchOn; /* 0 ȥʹ�ܼĴ����ض���1 ʹ�ܼĴ����ض� */
    EASYRF_OM_ReadTrxUint16 isTrx;    /* 0 �ض�TX, 1 �ض�RX, 2 tx/rx���ض� */
} EasyrfGpioStatusReadPara;

/*
 * �ṹ˵��: TOOL->BBA gpio readԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                    /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16               toolsId; /* �����ֶ� */
    EasyrfGpioStatusReadPara gpioReadPara;
} UphyToolEasyrfGpioReadReq;

/*
 * Name: EasyrfMipidevCmdStru
 * Description:
 */
typedef struct {
    VOS_UINT32 bitByteCnt : 5;     /* Byte Cnt,��1��ʼ��Ч */
    VOS_UINT32 bitMipiPortSel : 3; /* MIPI PORTѡ��,��0��ʼ��Ч */
    VOS_UINT32 bitSlaveId : 4;     /* Savle Id,0-15 */
    VOS_UINT32 bitRegAddr : 16;    /* �Ĵ�����ַ */
    VOS_UINT32 bitRsv : 4;
} EasyrfMipidevCmd;

/*
 * Name: EasyrfRffeMipiReadDataStru
 * Description:
 */
typedef struct {
    VOS_UINT8        mipiNum;
    VOS_UINT8        isTrx;                            /* 0 �ض�TX, 1 �ض�RX, 2 tx/rx���ض� */
    VOS_UINT16       applyType;                        /* *< immediately or timing  */
    VOS_UINT32       applyTime;                        /* *< Time when data is valid, Unit: MC */
    EasyrfMipidevCmd cmd[EASYRF_OM_MAX_RD_MIPI_COUNT]; /* *< Port, Device, Address */
} EasyrfRffeMipiReadData;

/*
 * �ṹ˵��: TOOL->BBA gpio readԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16             toolsId; /* �����ֶ� */
    EasyrfRffeMipiReadData mipiReadPara;
} UphyToolEasyrfMipiReadReq;

/*
 * �ṹ˵��: TOOL-BBA�Ĵ����ض�ԭ����Ϣ
 */
typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16    toolsId; /* �����ֶ� */
    EasyrfPdOmReq omPara;
} UphyToolEasyrfPdOmReq;

/*
 * �ṹ˵��: TOOL-BBA UL TIMING����ԭ��
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16              toolsId; /* �����ֶ� */
    EasyrfUlTimingUpdateReq timingPara;
} UphyToolEasyrfUlTimingUpdateReq;

/*
 * �ṹ˵��: TOOL-BBA UL TIMING����ԭ��
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16 toolsId;   /* �����ֶ� */
    VOS_UINT16 isEnable;  /* ����׮ʹ�� */
    VOS_UINT16 reserved1;
    VOS_UINT16 resetInd;  /* 0 -- rfic0, 1 -- rfic1, 2 -- all */
    VOS_UINT16 period;    /* ����EasyRF�쳣������, ��λ���� */
    VOS_UINT32 reserved2;
} UphyToolEasyrfRfResetTestReq;

/*
 * �ṹ˵��: TOOL-EASYRFԴ��ͳ���ϱ�����������Ϣ
 */
typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16 toolsId;   /* �����ֶ� */
    VOS_UINT16 rptSwitch; /* �ϱ����� */
    VOS_UINT16 period;    /* �ϱ����� */
} UPHY_TOOL_EasyrfSrcReportCfgReq;

/*
 * �ṹ˵��: G��ģINT0���ó�ʱָʾԭ����Ϣ
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;              /* ԭ������ */ /* _H2ASN_Skip */
    VOS_UINT16                toolsId;            /* �����ֶ� */
    VOS_UINT16                rsv;                /* �����ֶ� */
    VOS_UINT16                timeBtwInt0AndInt1; /* int0��int1ʱ�䣬us */
    VOS_UINT32                timeBeforeCfg;      /* ����ǰ����ʱ�䣬us */
} GPHY_TOOL_GSLAVE_INT0_TIMEOUT_IND_STRU;

/*
 * �ṹ˵��: BBPӲ������������ʱ����Ϣ
 */
typedef struct {
    VOS_UINT16 cfgTimingType; /* ���� */
    VOS_UINT16 sfncfn;        /* ֡��,ȡֵ��Χ[0,255],����uhwImmediateActiveFlag=0ʱ��Ч */
    VOS_UINT16 slot;          /* ʱ϶��,ȡֵ��Χ[0,14],����uhwImmediateActiveFlag=0ʱ��Ч */
    VOS_UINT16 chip;          /* Chip */
    VOS_UINT16 period;        /* 4096 or 256 */
    VOS_UINT16 timeTypeMc;    /* ʱ�������Ƿ�ΪMC,TRUE����MC����.��ģ��ʱ��ΪMC����Ҫת����,ֱ����MC */
    VOS_UINT32 rfCfgMc;       /* ��ģ��ֱ����MC */
} WPHY_TOOL_AeasyrfCfgTime;

/*
 * �ṹ˵��: ��ȡU�Ĵ���BBPCOMM_W_TIMING_DSP_UNION��ʱ��Ϣ
 */
typedef struct {
    VOS_UINT16 sfn;  /* umts timing��Ϣ,��Чw_sfn_dsp */
    VOS_UINT16 slot; /* umts timing��Ϣ,��Чw_slot_cnt_dsp */
    VOS_UINT16 chip; /* umts timing��Ϣ,��Чw_chip_cnt_dsp */
    VOS_UINT16 mfn;  /* umts timing��Ϣ,��֡�� */
} UPHY_TOOL_IratUmtsTimingInfo;

/*
 * �ṹ˵��: EASYRF����ģ��ά��ȫ����Ϣ�ṹ��
 */
typedef struct {
    UPHY_TOOL_MntnBoolUint16          cfgResult;       /* �������ý�� */
    WPHY_TOOL_AeasyrfConfigTypeUint16 easyRfCfgType;   /* ҵ����·���easyrf�������� */
    VOS_UINT32                        centerFreq;      /* ���õ�Ƶ�� */
    VOS_UINT16                        band;            /* ���õ�band */
    VOS_UINT16                        rfCfgErrorFlag;  /* ��������Ƶ�����쳣�ı�� */
    VOS_UINT32                        rfCfgErrorSlice; /* ��¼����һ���쳣start��slice */
    VOS_UINT16                        chanType;        /* ����ͨ������ */
    VOS_UINT16                        rxAntCfgMap;     /* RX����������:bit0:����;bit1:�ּ� */
    VOS_UINT16                        activeMode;      /* ǰ�˼���ģʽ */
    VOS_UINT16                        taskNum;         /* �ֽ��RFHAL�ӿ�������� */
    WPHY_TOOL_AeasyrfCfgTime          cfgActiveTime;   /* �߲����õļ���ʱ�� */
    UPHY_TOOL_IratUmtsTimingInfo      bbpWorkTime;     /* ������ҵ����BBP���Թ���ʱ���¼ */
    /* �ֽ��ÿ��EASYRF�������ö�Ӧָ�������ʱ�� */ /* ���õ�MCʱ�� */
    VOS_UINT32 mcCfgTime[WPHY_TOOL_AEASYRF_CONFIG_TASK_MAX_NUM];
    VOS_UINT32 cfgStartTime;     /* easyrfǰ�����ÿ�ʼʱ�̼�¼,��λ:MC */
    VOS_UINT32 cfgFinishTime;    /* easyrfǰ���������ʱ�̼�¼,��λ:MC */
    VOS_UINT32 currRxCenterFreq; /* ��ǰ������Rx freq,����Ƶ�ʵ�λkHz */
    VOS_UINT32 currTxCenterFreq; /* ��ǰ������Tx freq,����Ƶ�ʵ�λkHz */
    VOS_UINT16 currRxBand;       /* ��ǰ������Rx band */
    VOS_UINT16 currTxBand;       /* ��ǰ������Tx band */
} WPHY_TOOL_AeasyrfMntn;

/*
 * �ṹ˵��: AEASYRF CTRL INFOά��
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */
    VOS_UINT16                modemId;
    VOS_UINT16                rsv;
    WPHY_TOOL_AeasyrfMntn     mntnInfo[WPHY_TOOL_AEASYRF_MNTN_TASK_NUM];
} WPHY_TOOL_AeasyrfCtrlInfo;

/*
 * �ṹ˵��: EASYRF������ҵ�������ýӿڹ�����Ϣ
 */
typedef struct {
    VOS_UINT16               modemId;          /* ���ö�Ӧ��modem id  */
    VOS_UINT16               workBand;         /* ��������(rx stop����)��band */
    VOS_UINT16               rsv;
    VOS_UINT16               activeMode;       /* ǰ�˼���ģʽ */
    UPHY_TOOL_MntnBoolUint16 bandWidth10MFlag; /* �Ƿ�10M���� */
    UPHY_TOOL_MntnBoolUint16 dcScSwitchFlag;   /* �Ƿ�DS/SC�л�,��Ҫǰ�����⴦�� */
    VOS_UINT16               chanType;         /* ����ͨ������ */
    VOS_UINT16               rxAntNum;         /* RX֧�����߸���,NV��ȡ */
    VOS_UINT16               rxAntCfgMap;      /* RX����������:bit0:����;bit1:�ּ� */
    VOS_UINT16               rxWorkFreqNo;
    VOS_UINT32               rxWorkCenterFreq; /* RX����Ƶ��(Rx Stop����),��λkHz,��stopָ���������ָ���ӦƵ����Ϣ */
    VOS_UINT32               txWorkCenterFreq; /* TX����Ƶ��(Rx Stop����),��λkHz,��stopָ���������ָ���ӦƵ����Ϣ */
    WPHY_TOOL_AeasyrfCfgTime activeTime;       /* ����ʱ������ */
} WPHY_TOOL_AeasyrfConfigCommonInfo;

/*
 * �ṹ˵��: EASYRF������ҵ�������ýӿ���AGC�����Ϣ
 */
typedef struct {
    VOS_UINT16 agcGainIndex; /* *< Gain level index, Range:0-7 */
    VOS_UINT16 workType;     /* *< AGC Work Type */
} RFHAL_TOOL_RxAgcAntPath;

/*
 * �ṹ˵��: EASYRF������ҵ�������ýӿ���AGC�����Ϣ
 */
typedef struct {
    VOS_UINT16              agcAdjustMode;                       /* AGC����ģʽ */
    VOS_UINT16              sysSyncState;                        /* ���к������ͬ��״̬ */
    RFHAL_TOOL_RxAgcAntPath antInfo[WPHY_TOOL_WBBP_MAX_ANT_NUM]; /* AGC��λ��Ϣ */
    VOS_UINT16              configType;                          /* Set if analog gain is changed or unchanged */
    VOS_UINT16              rsv;
} WPHY_TOOL_AeasyrfConfigAgcInfo;

/*
 * �ṹ˵��: EASYRF������ҵ�������ýӿ���APC�����Ϣ
 */
typedef struct {
    VOS_UINT16 chanType;        /* �����ŵ����� */
    VOS_UINT16 rsv;
    VOS_UINT32 ulDpaDpchOffset; /* ����HS-DPCCH�ŵ���DPCH�ŵ�ƫ��,��λchip����֧��HS-DPCCH�ŵ�ʱ����Ϊ0 */
} WPHY_TOOL_AeasyrfConfigApcInfo;

/*
 * �ṹ˵��: EASYRF������ҵ�������ýӿ��ܽӿ�
 */
typedef struct {
    WPHY_TOOL_AeasyrfConfigTypeUint16 easyRfCfgType;       /* easyrf������������ */
    VOS_UINT16                        rsv;
    WPHY_TOOL_AeasyrfConfigCommonInfo easyRfCfgCommonPara; /* easyrf���ù������� */
    /* easyrf����AGC�ӿ����в���,������������Ϊ����AGC����ĵ�������������ʱ��Ч */
    WPHY_TOOL_AeasyrfConfigAgcInfo easyRfCfgAgcSpcPara;
    /* easyrf����APC�ӿ����в���,������������Ϊ����APC����ĵ�������������ʱ��Ч */
    WPHY_TOOL_AeasyrfConfigApcInfo easyRfCfgApcSpcPara;
} WPHY_TOOL_AeasyrfConfigInfo;

/*
 * �ṹ˵��: EASYRF��������ڵ�ṹ��
 */
typedef struct {
    UPHY_TOOL_MntnBoolUint16    nodeUsedFlag;
    VOS_UINT16                  rsv;
    WPHY_TOOL_AeasyrfConfigInfo rfCfgInfo;
} WPHY_TOOL_AeasyrfCfgTaskNode;

/*
 * �ṹ˵��: AEASYRF CFG TASK LIST INFOά��
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   toolsId; /* �����ֶ� */
    VOS_UINT16                   listUsedDepth;
    VOS_UINT16                   rsv;
    WPHY_TOOL_AeasyrfCfgTaskNode rfCfgTaskNode[WPHY_TOOL_RF_EASYRF_TASK_LIST_MAX_NUM];
} WPHY_TOOL_AeasyrfCfgTaskListInfo;

/*
 * �ṹ˵��: У׼�����в�����ά�ɲ���Ϣ�ϱ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    WPHY_TOOL_NoSignalMntnTypeUint16 type; /* ��¼У׼ά������ */
    VOS_UINT16                       reserved;

    VOS_UINT16 rptTimes; /* �˴��ϱ����ݵĴ��� */
    VOS_UINT16 dataNum;  /* �˴��ϱ����ݸ��� */

    VOS_UINT16 band;     /* Ƶ�� */
    VOS_UINT16 freqChan; /* Ƶ�� */

    VOS_UINT32 userData1; /* �û��Լ�ʹ�� */
    VOS_UINT32 userData2; /* �û��Լ�ʹ�� */
    VOS_UINT32 userData3; /* �û��Լ�ʹ�� */
    VOS_UINT32 userData4; /* �û��Լ�ʹ�� */
    VOS_UINT32 data[WPHY_TOOL_REPORT_SELF_CAL_SAMPLE_DATA_MAX_SIZE];
} WPHY_TOOL_RptCalSampleDataMntn;

/*
 * �ṹ˵��: Uģǰ�˷�����ά��ӿ�
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                toolsId; /* �����ֶ� */

    WPHY_TOOL_NoSignalMntnTypeUint16 mntnType;
    VOS_UINT16                       reserved;

    /* bbp�Ĵ����ض���� */
    VOS_UINT32 bbpRegReadResult[UPHY_TOOL_RDBACK_MIPI_NUM];

    /* ����У׼�����е�MIPI�ض���� */
    UPHY_TOOL_MipiRdResult rficMipiRead[UPHY_TOOL_RDBACK_MIPI_NUM]; /* RFIC MIPI�ض� */
    UPHY_TOOL_MipiRdResult rffeMipiRead[UPHY_TOOL_RDBACK_MIPI_NUM]; /* RFFE MIPI�ض� */
} WPHY_TOOL_NoSignalMntnRpt;

/*
 * 8 OTHERS����
 */

/*
 * 9 ȫ�ֱ�������
 */

/*
 * 10 ��������
 */

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of phytoolinterface.h */

