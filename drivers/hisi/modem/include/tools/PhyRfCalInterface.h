

/*
 * 1 ����ͷ�ļ�����
 */

#ifndef __PHYRFCALINTERFACE_H__
#define __PHYRFCALINTERFACE_H__
#include "uphy_config.h"
#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 2 �궨��
 */

#define GPHY_RF_CAL_TX_ON                     0x5555 /* �ϻ������У�ʹ�ܷ��� */
#define GPHY_RF_CAL_TX_OFF                    0xaaaa /* �ϻ������У�ֹͣ���� */

/* Mask code Used for PHY_RF_CalWSelfCalReq */
#define MASK_SELF_CAL_TX_IQ_MISMATCH          (1 << 0)
#define MASK_SELF_CAL_W_RX_DCR                (1 << 1)
#define MASK_SELF_CAL_W_RX_IP2                (1 << 2)
/* Mask code Used for PHY_RF_CAL_ET_DELAY_SELF_CAL_REQ_STRU */
#define MASK_SELF_CAL_W_ET_DELAY              (1 << 3)
#define MASK_SELF_CAL_W_ET_DELAY_FREQCOMP     (1 << 4)

/* Mask code Used for PHY_RF_CAL_W_DPD_LUT_SELF_CAL_REQ_STRU */
#define MASK_SELF_CAL_W_DPD_LUT               (1 << 5)

/* Mask code Used for PHY_RF_CalWSelfCalReq */
#define MASK_SELF_CAL_W_MRX_DCROFFSET         (1 << 6)

/* Mask code Used for PHY_RF_CalWSelfCalReq */
#define MASK_SELF_CAL_W_ET_DELAY_EXTRACT      (1 << 7)

/* Mask code Used for PHY_RF_CalWSelfCalReq */
#define MASK_SELF_CAL_W_ET_PA_GAIN            (1 << 8)

/* CDMA��У�ӿ�����У׼MASKָʾ */
#define MASK_CPHY_SLOW_SELF_CAL_ET            (1 << 10) /* ET LUT��ȡģʽ */
#define CSDR_CAL_SLOW_SELF_CAL_DPD            0x500  /* DPD LUT��ȡģʽ */

/* Mask code Used for PHY_RF_CalWTrxParaCfgReq */
#define MASK_W_TRX_CFG_TX_AFC                 (1 << 0)
#define MASK_W_TRX_CFG_TX_CFIX                (1 << 1)
#define MASK_W_TRX_CFG_TX_ARFCN               (1 << 2)
#define MASK_W_TRX_CFG_TX_ONOFF               (1 << 3)
#define MASK_W_TRX_CFG_TX_RFIC_MODE           (1 << 4)
#define MASK_W_TRX_CFG_TX_PAMODE              (1 << 5)
#define MASK_W_TRX_CFG_TX_POWER_PARA          (1 << 6)
#define MASK_W_TRX_CFG_TX_PA_VCC              (1 << 7)  /* APT ET����ʱ����ǿ��PA��ѹ������һ������ */
#define MASK_W_TRX_CFG_PD_MRX_ONOFF           (1 << 8)  /* Hi6363ʱMRX��λ���ú�Mrx Corr�����ϱ����ø����� */
#define MASK_W_TRX_CFG_TX_UPA                 (1 << 9)
#define MASK_W_TRX_CFG_SELF_CAL               (1 << 10) /* ET/DPD��ȡ */
#define MASK_W_TRX_CFG_TX_C2FIX               (1 << 11)
#define MASK_W_TRX_CFG_TX_DPA                 (1 << 12)
#define MASK_W_TRX_CFG_TX_ET_TIMEDELAY        (1 << 13)
#define MASK_W_TRX_CFG_TX_DPD                 (1 << 14)
/* ���ֻ��Viq�����룬��ʾ����㶨���ȣ����Viq��Step�����붼�򿪣���ʾ���ȴ�Viq�������������Ⱥ��Զ�ѭ�� */
#define MASK_W_TRX_CFG_TX_V_IQ                (1 << 15)
#define MASK_W_TRX_CFG_RX_ARFCN               (1 << 16)
#define MASK_W_TRX_CFG_RX_CROSS_ARFCN         (1 << 17)
#define MASK_W_TRX_CFG_RX_ONOFF               (1 << 18)
#define MASK_W_TRX_CFG_RX_ANTSEL              (1 << 19)
#define MASK_W_TRX_CFG_RX_AGC_GAIN            (1 << 20)
#define MASK_W_TRX_CFG_RX_EVM                 (1 << 21)
#define MASK_W_TRX_CFG_XO_SELFCAL             (1 << 22)
#define MASK_W_TRX_CFG_CLT_HVDAC              (1 << 23)
#define MASK_W_TRX_CFG_CLT_PATH_DELAY         (1 << 25) /* ��Ҫ��ȡpath delayֵ���ϱ� */
#define MASK_W_TRX_CFG_CLT_REFLECTANCE_DETECT (1 << 26) /* ��Ҫ��ȡ����ϵ�����ϱ� */
#define MASK_W_TRX_CFG_DPD_STEP_V             (1 << 27)
#define MASK_W_TRX_CFG_TX_ETDPD_COMP          (1 << 28) /* ETDPD�²�/Ƶ�� */

/* ETУ׼������ѹ�����Ƿ���Ч���������Ϊ1��������ѹ����ʹ��ԭ�����ֵ������ʹ��NV��ֵ */
#define MASK_W_TRX_CFG_TX_ET_COMP             (1 << 29)
#define MASK_W_TRX_CFG_TX_WAVE                (1 << 30)
#define MASK_W_TRX_CFG_TX_IQ_STEP             (((VOS_UINT32)1) << 31)

#define MASK_W_TRX_CFG_TX_APT MASK_W_TRX_CFG_TX_PA_VCC

/* Mask code Used for PHY_RF_CalWTrxFastCalReq_usTxMask */
#define MASK_W_TRX_FAST_CAL_REQ_TXMASK_HDET_MRX         (1 << 0)
#define MASK_W_TRX_FAST_CAL_REQ_TXMASK_APT              (1 << 1)
#define MASK_W_TRX_FAST_CAL_REQ_TXMASK_VGA              (1 << 2)
#define MASK_W_TRX_FAST_CAL_REQ_TXMASK_DCOFFSET         (1 << 3)
#define MASK_W_TRX_FAST_CAL_REQ_ET_LUT_APT_MODE         (1 << 4)
#define MASK_W_TRX_FAST_CAL_REQ_ET_LUT_ET_MODE          (1 << 5)
#define MASK_W_TRX_FAST_CAL_REQ_ET_GAIN_OFFSET_APT_MODE (1 << 6)
#define MASK_W_TRX_FAST_CAL_REQ_ET_GAIN_OFFSET_ET_MODE  (1 << 7)

#define MASK_W_TRX_FAST_CAL_ET_WORK                                                     \
    (MASK_W_TRX_FAST_CAL_REQ_ET_LUT_APT_MODE | MASK_W_TRX_FAST_CAL_REQ_ET_LUT_ET_MODE | \
     MASK_W_TRX_FAST_CAL_REQ_ET_GAIN_OFFSET_APT_MODE | MASK_W_TRX_FAST_CAL_REQ_ET_GAIN_OFFSET_ET_MODE)

#define MASK_W_TRX_FAST_CAL_ET_GAIN_OFFSET \
    (MASK_W_TRX_FAST_CAL_REQ_ET_GAIN_OFFSET_APT_MODE | MASK_W_TRX_FAST_CAL_REQ_ET_GAIN_OFFSET_ET_MODE)

/* Mask code Used for PHY_RF_CalWEtFastReq_usMask */
#define MASK_W_ET_FAST_REQ_MASK_HDET          (1 << 0)
#define MASK_W_ET_FAST_REQ_MASK_COMP          (1 << 1)

/* Mask code Used for GPHY_RF_CalCommSelfCalReqData_ulTestItemsBitMap */
#define MASK_G_SELF_CAL_TX_IQ_MISMATCH        (1 << 0)
#define MASK_G_SELF_CAL_MRX_DCOFFSET          (1 << 1)
#define MASK_G_SELF_CAL_RX_DCOFFSET           (1 << 2)
#define MASK_G_SELF_CAL_TX_RF_SETTING         (1 << 3)
#define MASK_G_SELF_CAL_TX_RAMP_COFF          (1 << 4)

/* ����ά�� */
#define MASK_G_SELF_CAL_DEBUG_DATA            (1 << 30)

/* Mask code Used for GPHY_CAL_RF_COMM_DATA_RPT_STRU_ulTestItemsBitMap */
#define MASK_GRF_COMM_DATA_RPT_TX_IQ_MISMATCH (1 << 0)
#define MASK_GRF_COMM_DATA_RPT_MRX_DCOFFSET   (1 << 1)
#define MASK_GRF_COMM_DATA_RPT_RX_DCOFFSET    (1 << 3)

/* Mask code Used for PHY_RF_CalWPdParaCfgReq */
#define MASK_W_PD_PARA_CFG_THROUGH            (1 << 0)
#define MASK_W_PD_PARA_CFG_DC_OFFSET          (1 << 1)
#define MASK_W_PD_PARA_CFG_VGA                (1 << 2)

/* Mask code Used for PHY_CAL_RfCommDataRptMsgHead_ulTestItemsBitMap */
#define MASK_RF_COMM_DATA_RPT_ET_LUT          (1 << 0)
#define MASK_RF_COMM_DATA_RPT_DPD_LUT         (1 << 1)
#define MASK_RF_COMM_DATA_RPT_ET_DELAY        (1 << 2)

#define G_RAMP_PWR_LEVEL_NUM                  7     /* ���±��������7�ű� */
#define G_RAMP_COEF_NUM                       16    /* ���µ��� */

/* W Rx AGC GAIN COUNT */
#define W_RX_NOBLOCK_AGC_GAIN_COUNT           8
#define W_RX_BLOCK_AGC_GAIN_COUNT             8

#define W_TRX_MAX_STEP_COUNT                  16
#define W_TRX_MAX_SWEEP_FRAME_COUNT           38    /* PCһ���·���UE�����ݰ����8K���� */
/* DSPһ���ϱ���������������Rssi At1(0.125dBm)+Rssi At2(0.125dBm)+PD������� */
#define W_TRX_MAX_REPORT_COUNT                1500

/* WģУ׼�ϱ�ʱ�ɹ�ʧ�ܱ�ʶ */
#define WPHY_TOOL_CAL_RESULT_RPT_SUCCESS      0

/* CģУ׼�ϱ�ʱ�ɹ�ʧ�ܱ�ʶ */
#define CPHY_TOOL_CAL_RESULT_RPT_SUCCESS      0

/* Wģ��У׼��Ƶ�������� */
#define SELF_CAL_BAND_ID_MAX_COUNT            10

#define SELF_CAL_ITEMS_MAX_COUNT              32

/* Xģ��У׼��Ƶ�������� */
#define CDMA_SELF_CAL_BAND_ID_MAX_COUNT       2

#define WG_BBP_RF_REG_WR_MAX_COUNT            8

/* Xģ�Ĵ�����д���� */
#define CSDR_BBP_RF_REG_WR_MAX_COUNT WG_BBP_RF_REG_WR_MAX_COUNT

/* G TRX CFG MARCO */
/* Mask code Used for RF_CAL_G_TX_PARA_CFG_REQ_STRU */
#define MASK_CAL_RF_G_TX_AFC                  (1 << 0) /* ���е���Ƶ�ʿ���ֵ */
#define MASK_CAL_RF_G_TX_ARFCN                (1 << 1) /* ����Ƶ���Ƿ���Ч */
#define MASK_CAL_RF_G_TX_ONOFF                (1 << 2) /* ���з��俪���Ƿ���Ч */
#define MASK_CAL_RF_G_TX_DATA_PATTERN         (1 << 3) /* ���е����������Ƿ���Ч */
#define MASK_CAL_RF_G_TX_CFIX                 (1 << 4) /* ����Cfix�Ƿ���Ч */
#define MASK_CAL_RF_G_TX_SLOT_CFG             (1 << 5) /* ���е�ʱ϶�����Ƿ���Ч */
#define MASK_CAL_RF_G_TX_C2FIX                (1 << 6) /* ����Cfix�Ƿ���Ч */
#define MASK_CAL_RF_G_GET_CLT_RFT_VALUE       (1 << 7) /* Tuner����ϵ����ȡ */
#define MASK_CAL_RF_G_GET_CLT_PATH_DELEAY     (1 << 8) /* Tuner Delay�ӳ���ȡ */
#define MASK_CAL_RF_G_GET_CLT_HVDAC           (1 << 9) /* ��ʾ��ҪHVDAC������Ч */

/* Mask code Used for RF_CAL_G_RX_PARA_CFG_REQ_STRU */
#define MASK_CAL_RF_G_RX_ARFCN                (1 << 0) /* �·�Ƶ�� */
#define MASK_CAL_RF_G_RX_MODE                 (1 << 1) /* �·�����ģʽ */
#define MASK_CAL_RF_G_RX_AGCMODE              (1 << 2) /* �·�����ģʽ */
#define MASK_CAL_RF_G_RX_AGCGAIN              (1 << 3) /* �·�AGC��λ */
#define MASK_CAL_RF_G_RX_BLOCKERMODE          (1 << 4) /* �·�AGC����ģʽ */
#define MASK_CAL_RF_G_RX_ANTSEL               (1 << 5) /* �·�AGC����ѡ�� */

/* Mask code User for PHY_RF_CalGTxFastCalReq */
#define MASK_G_TX_FAST_CAL_REQ_TXMASK_APT     (1 << 0) /* APTʹ�� */
#define MASK_G_TX_FAST_CAL_REQ_TXMASK_MRX     (1 << 1) /* MRx�ϱ�ʹ�� */

#define G_TX_PARA_MAX_SLOT_CNT_PER_FRAME      4

#define G_TX_MAX_STEP_COUNT                   7     /* ��ǰֻ֧��4ʱ϶ */
#define G_TRX_MAX_SWEEP_FRAME_COUNT           120   /* Hi6362ֻ֧��120֡ */
#define G_TMRX_MAX_SWEEP_FRAME_COUNT          400   /* Hi6363��ʼ���֧��400֡ */
#define G_RX_MAX_STEP_COUNT                   7
#define G_RX_MAX_RSSI_COUNT (G_TRX_MAX_SWEEP_FRAME_COUNT * G_RX_MAX_STEP_COUNT * 2) /* �ּ����߲������ϱ�����*2 */
#define G_RX_AGC_GAIN_COUNT                   8
#define G_RX_MAX_CFG_REQ_DATA_COUNT           2     /* ����У׼REQ��Ϣ�޸�Ϊ��̬��Ϣ���ȣ�֧��2��ʱ϶�Ķ�̬���� */

#define W_GAUGE_RESULT_SIZE                   13    /* 32KУ׼�ϱ����ݸ��� */

#define GSM_TX_HD35_PCL_NUM                   3
#define GSM_TX_HD35_ARFCN_NUM                 3
#define GSM_TX_HD35_STEP_NUM                  5
#define GSM_TX_HD35_BAND_NUM                  4

#define GPHY_RX_IQ_MISMATCH_SLOT_NUM          64    /* �ܲɼ�64(4*16)��ʱ϶���ݣ�16֡��ÿ֡4��ʱ϶ */

#define W_RX_EVM_REPORT_ON                    1     /* EVM�ϱ��� */
#define W_RX_EVM_REPORT_OFF                   0     /* EVM�ϱ��ر� */
#define W_RX_EVM_REPORT_DATA_COUNT            10240 /* EVMһ���ϱ���������,32bit */
#define COMM_SELF_CAL_TESTITEM_NUM            32    /* ͨ����У׼��װ��һ���·�֧�ֵ������У׼�� */

/*
 * ��ϢͷGSM ģʽID Componentid
 * lte          0x3000
 * tds          0x3100
 * gsm          0x3200
 * wcdma        0x3300
 * x            0x3400
 * ��ģʽ�޹�   0x3f00
 */

#define GPHY_MSGHEAD_COMPONENT_ID             0x3200
#define WPHY_MSGHEAD_COMPONENT_ID             0x3300
#define CPHY_MSGHEAD_COMPONENT_ID             0x3400

#define W_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE    16000 /* W����У׼�ӿ�һ�ο��Ը�DSP�·���������ݰ����� */
#define C_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE    2000  /* C����У׼�ӿ�һ�ο��Ը�DSP�·���������ݰ����� */
/* DSPһ���ϱ���������������Rssi At1(0.125dBm)+Rssi At2(0.125dBm)+MRX�������,��4���ֽ� */
#define W_TRX_MAX_REPORT_COUNT_HI6363         4000

/* W DCXO ���������� */
#define DCXO_SAMPLE_MAX_NUM                   24

#define PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM       3

/*
 * 3 ö�ٶ���
 */

/*
 * ö��˵��:RF CAL MSG ID ENUM
 */
enum PHY_TOOL_MsgId {
    /* WCDMA RF����У׼�ӿ�ԭ���б� */
    ID_TOOL_WPHY_CAL_TRX_FAST_CAL_REQ = 0x1240, /* _H2ASN_MsgChoice PHY_RF_CalWTrxFastCalReq */

    ID_TOOL_WPHY_CAL_PD_PARA_CFG_REQ = 0x1241, /* _H2ASN_MsgChoice PHY_RF_CAL_PD_PARA_CFG_REQ_STRU */

    /* WCDMA RF��У׼�ӿ�ԭ���б� */
    ID_TOOL_WPHY_CAL_SELF_CAL_REQ = 0x1242, /* _H2ASN_MsgChoice PHY_RF_CalWSelfCalReq */

    /* WCDMA RF����У׼�ӿ�ԭ���б� */
    ID_TOOL_WPHY_CAL_TRX_PARA_CFG_REQ = 0x1243, /* _H2ASN_MsgChoice PHY_RF_CalWTrxParaCfgReq */
    ID_TOOL_WPHY_CAL_RX_RSSI_MEAS_REQ = 0x1244, /* _H2ASN_MsgChoice PHY_RF_CalWRxRssiMeasReq */
    ID_WPHY_TOOL_GAUGE_REQ            = 0x1245, /* _H2ASN_MsgChoice WPHY_TOOL_GaugeReq */

    /* W�Ĵ�����д�ӿ� */
    ID_TOOL_WPHY_WR_RFIC_REG_REQ = 0x1246,
    ID_TOOL_WPHY_WR_BBP_REG_REQ  = 0x1247,

    /* WCDMA ET����У׼�ӿ� */
    ID_TOOL_WPHY_CAL_ET_FAST_REQ = 0x1248,

    /* WCDMA ET Delay Freq Comp��У׼�ӿ�ԭ�� */
    /* _H2ASN_MsgChoice PHY_RF_CAL_ET_DELAY_SELF_CAL_REQ_STRU */
    ID_TOOL_WPHY_CAL_ET_DELAY_FREQ_COMP_SELF_CAL_REQ = 0x1249,

    /* DCXO��У׼ */
    ID_TOOL_WPHY_CAL_XO_COFFICIENT_REQ = 0x124a,

    ID_TOOL_WPHY_CAL_DPD_LUT_SELF_CAL_REQ = 0x124b,

    /* DPDָ��Ƶ��У׼��ϢID */
    ID_TOOL_WPHY_CAL_ASIGNED_CHAN_DPD_LUT_SELF_CAL_REQ = 0x124c,

    /* ����������DPDT��ϢID */
    ID_TOOL_WPHY_CAL_SET_DPDT_VALUE_REQ = 0x124d,

    ID_RRC_PHY_SWITCH_MOANT_IND = 0x1250, /* _H2ASN_MsgChoice RRC_PHY_SwitchMoantInd */

    /* W RFͨ��CNF */
    ID_WPHY_TOOL_CAL_RF_MSG_CNF = 0x21F0, /* _H2ASN_MsgChoice PHY_RF_CalWRfMsgCnf */

    ID_WPHY_TOOL_CAL_TX_POWER_DETECT_IND = 0x21F1, /* _H2ASN_MsgChoice PHY_RF_CalWRfTxPdInd */
    ID_WPHY_TOOL_GAUGE_CNF               = 0x21F2, /* _H2ASN_MsgChoice WPHY_TOOL_GaugeCnf */
    ID_WPHY_TOOL_GAUGE_IND               = 0x21F3, /* _H2ASN_MsgChoice WPHY_TOOL_GaugeInd */

    ID_WPHY_TOOL_CAL_TRX_FAST_CAL_IND = 0x21F4, /* _H2ASN_MsgChoice PHY_RF_CalWTrxFastCalInd */
    ID_WPHY_TOOL_CAL_SELF_CAL_IND     = 0x21F5, /* _H2ASN_MsgChoice PHY_RF_CalWSelfCalInd */

    ID_WPHY_TOOL_CAL_RX_RSSI_MEAS_IND = 0x21F6, /* _H2ASN_MsgChoice PHY_RF_CalWRxRssiMeasInd */

    ID_WPHY_TOOL_R_RFIC_REG_IND = 0x21F8,
    ID_WPHY_TOOL_R_BBP_REG_IND  = 0x21F9,

    /* W IP2��У׼��ȡIM2�ϱ��ӿ� */
    ID_WPHY_TOOL_IP2_IM2_VALUE_IND = 0x21FA,
    ID_WPHY_TOOL_RX_EVM_MEAS_IND   = 0x21FB,

    /* �����ͨ�������ϱ� */
    ID_PHY_TOOL_COMM_DATA_IND = 0x21FD,

    /* ������ά�ɲ������ϱ� */
    ID_PHY_TOOL_MNTN_DATA_RPT          = 0x21FE,
    ID_WPHY_TOOL_CAL_XO_COFFICIENT_IND = 0x21FF, /* _H2ASN_MsgChoice PHY_RF_CalWRxRssiMeasInd */

    /* Path Delay�ϱ� */
    ID_WPHY_TOOL_CAL_CLT_PATH_DELAY_IND = 0x2200,

    /* ����ϵ���ϱ� */
    ID_WPHY_TOOL_CAL_CLT_REFLECTANCE_IND = 0x2201,

    /* ����ǿ��HVDAC��Ϣ�ϱ� */
    ID_WPHY_TOOL_CAL_CLT_HVDAC_CFG_IND = 0x2202,

    /* ����������DPDT CNF�ظ��ϱ�ID */
    ID_WPHY_TOOL_CAL_SET_DPDT_VALUE_CNF = 0x2203,

    // CDMA��ϢID�Ķ��壬ID����Ҫ���¶���
    /* CDMA RF����У׼�ӿ�ԭ���б� */
    ID_TOOL_CPHY_CAL_TRX_FAST_CAL_REQ = 0x2F00,

    ID_TOOL_CPHY_CAL_PD_PARA_CFG_REQ = 0x2F01,

    /* CDMA RF��У׼�ӿ�ԭ���б� */
    ID_TOOL_CPHY_CAL_SELF_CAL_REQ = 0x2F02,

    /* CDMA RF����У׼�ӿ�ԭ���б� */
    ID_TOOL_CPHY_CAL_TRX_PARA_CFG_REQ = 0x2F03,
    ID_TOOL_CPHY_CAL_RX_RSSI_MEAS_REQ = 0x2F04,

    /* CDMA RFIC��BBP��д�ӿ�ԭ���б� */
    ID_TOOL_CPHY_CAL_WR_RFIC_REG_REQ = 0x2F05,
    ID_TOOL_CPHY_CAL_WR_BBP_REG_REQ  = 0x2F06,

    /* CDMA ET LUT��������У��Ϣ */
    ID_TOOL_CPHY_CAL_ET_LUT_FAST_CAL_REQ = 0x2F07,

    /* CDMA ET Delay (����delayƵ��)��У */
    ID_TOOL_CPHY_CAL_ET_DELAY_SELF_CAL_REQ = 0x2F08,

    /* CT��HIRF����DPDT��ϢID */
    ID_TOOL_CPHY_CAL_SET_DPDT_VALUE_REQ = 0x2F09,

    /* CDMA RFͨ��CNF */
    ID_CPHY_TOOL_CAL_RF_MSG_CNF = 0x2F10,

    /* CDMAͨ�������ϱ�,ET DPD delay У׼����ϱ� */
    ID_CPHY_TOOL_COMM_DATA_IND = 0x2F11,

    ID_CPHY_TOOL_CAL_TX_POWER_DETECT_IND = 0x2F20,

    /* TRX��У ����ϱ� */
    ID_CPHY_TOOL_CAL_TRX_FAST_CAL_IND = 0x2F21,

    /* ��У׼ ����ϱ� */
    ID_CPHY_TOOL_CAL_SELF_CAL_IND = 0x2F22,

    ID_CPHY_TOOL_CAL_RX_RSSI_MEAS_IND = 0x2F23,

    /* CDMA RFIC��BBP�����IND */
    ID_CPHY_TOOL_CAL_R_RFIC_REG_IND = 0x2F24,
    ID_CPHY_TOOL_CAL_R_BBP_REG_IND  = 0x2F25,

    /* pathdelay ��ȡ�ϱ� */
    ID_CPHY_TOOL_CAL_CLT_PATH_DELAY_IND = 0x2F26,

    /* hitune ����ϵ����ȡ�ϱ� */
    ID_CPHY_TOOL_CAL_CLT_REFLECTANCE_IND = 0x2F27,

    ID_CPHY_TOOL_CAL_CLT_HVDAC_CFG_IND = 0x2F28,

    /* CT��HIRF����DPDTֵCNF�ϱ� */
    ID_CPHY_TOOL_CAL_SET_DPDT_VALUE_CNF = 0x2F29,

    /* GSM RF����У׼�ӿ�ԭ���б� */
    ID_TOOL_GPHY_CAL_RX_FAST_CAL_REQ = 0x1740, /* _H2ASN_MsgChoice PHY_RF_CalGRxFastCalReq */
    ID_TOOL_GPHY_CAL_TX_FAST_CAL_REQ = 0x1741, /* _H2ASN_MsgChoice PHY_RF_CalGTxFastCalReq */

    /* GSM ��У׼�ӿ�ԭ���б� */
    ID_TOOL_GPHY_CAL_SELF_CAL_REQ   = 0x1742, /* _H2ASN_MsgChoice PHY_RF_CAL_G_SELF_CAL_REQ_STRU */
    ID_TOOL_GPHY_CAL_RX_DCR_CAL_REQ = 0x1743, /* _H2ASN_MsgChoice PHY_RF_CalGRxDcrReq */

    /* GSM RF����У׼�ӿ�ԭ���б� */
    ID_TOOL_GPHY_CAL_TX_PARA_CFG_REQ  = 0x1744, /* _H2ASN_MsgChoice PHY_RF_CalGTxParaCfgReq */
    ID_TOOL_GPHY_CAL_RX_PARA_CFG_REQ  = 0x1745, /* _H2ASN_MsgChoice PHY_RF_CalGRxParaCfgReq */
    ID_TOOL_GPHY_CAL_RX_RSSI_MEAS_REQ = 0x1746, /* _H2ASN_MsgChoice PHY_RF_CalGRxRssiMeasReq */

    ID_TOOL_GPHY_WR_RFIC_REG_REQ = 0x1747,
    ID_TOOL_GPHY_WR_BBP_REG_REQ  = 0x1748,

    /* GSM PA RAMP ���ýӿ� */
    ID_TOOL_GPHY_CAL_PA_RAMP_CFG_REQ = 0x1749,

    /* GSM TIMING ���ýӿ� */
    ID_TOOL_GPHY_CAL_TIMING_CFG_REQ     = 0x1750,
    ID_TOOL_GPHY_CAL_RX_IQ_MISMATCH_REQ = 0x1751,

    /* GSM HD3HD5У׼ԭ�� */
    ID_TOOL_GPHY_CAL_TX_HD3_HD5_REQ = 0x1752,

    ID_MPH_SWITCH_M1XO_IND = 0x1753,

    ID_TOOL_GPHY_SWITCH_MOANT_IND = 0x1754,

    /* Hi6363��У׼ԭ��,IND��Ϣͨ��ID_GPHY_TOOL_COMM_DATA_IND�ϱ� */
    ID_TOOL_GPHY_CAL_COMM_SELF_CAL_REQ = 0x1755, /* _H2ASN_MsgChoice  GPHY_RF_CalCommSelfCalReq */

    /* Hi6363 Tx����У׼ԭ�� */
    ID_TOOL_GPHY_CAL_TMRX_FAST_CAL_REQ = 0x1756, /* _H2ASN_MsgChoice  PHY_RF_CalGeTmrxFastCalReq */

    ID_TOOL_GPHY_CAL_MRX_CORR_REQ = 0x1757, /* _H2ASN_MsgChoice  PHY_RF_CalGeTmrxFastCalReq */

    /* G RFͨ��CNF */
    ID_GPHY_TOOL_CAL_RF_MSG_CNF = 0x7120, /* _H2ASN_MsgChoice  PHY_RF_CAL_G_RF_MSG_CNF_STRU */

    ID_GPHY_TOOL_CAL_RX_FAST_CAL_IND = 0x7121, /* _H2ASN_MsgChoice  PHY_RF_CalGRxFastCalInd */

    ID_GPHY_TOOL_CAL_SELF_CAL_IND     = 0x7122, /* _H2ASN_MsgChoice  PHY_RF_CAL_G_SELF_CAL_IND_STRU */
    ID_GPHY_TOOL_CAL_RX_DCR_CAL_IND   = 0x7123, /* _H2ASN_MsgChoice  PHY_RF_CAL_G_RX_DCR_RPT_IND_STRU */
    ID_GPHY_TOOL_CAL_RX_RSSI_MEAS_IND = 0x7124, /* _H2ASN_MsgChoice  PHY_RF_CalGRxRssiMeasInd */

    ID_GPHY_TOOL_R_RFIC_REG_IND = 0x7126,
    ID_GPHY_TOOL_R_BBP_REG_IND  = 0x7127,

    ID_GPHY_TOOL_CAL_RX_IQ_MISMATCH_IND = 0x7128,

    ID_GPHY_TOOL_CAL_TX_HD3_HD5_IND = 0x7129,

    /* Gģ�����ͨ�������ϱ��ӿ� */
    ID_GPHY_TOOL_COMM_DATA_IND         = 0x7130,
    ID_GPHY_TOOL_CAL_TMRX_FAST_CAL_IND = 0x7131, /* _H2ASN_MsgChoice  PHY_RF_CalGeTmrxFastCalInd */

    ID_GPHY_TOOL_CAL_MRX_CORR_IND = 0x7132, /* _H2ASN_MsgChoice  PHY_RF_CalGMrxCorrInd */

    ID_GPHY_TOOL_CAL_CLT_RFT_IND = 0x7133,

    ID_GPHY_TOOL_CAL_CLT_HVDAC_CFG_IND = 0x7135,
};
typedef VOS_UINT16 PHY_TOOL_MsgIdUint16;

/*
 * ö��˵��:�������У׼ʧ��ԭ��˵��
 */
enum RF_CAL_SelfCalErrCode {
    RF_CAL_SELF_CAL_SUCCESS                         = 0,
    RF_CAL_SELF_CAL_FAIL                            = 1,
    RF_CAL_SELF_CAL_IP2_MEM_ALLOC_FAIL              = 2,    /* IP2У׼ʱ,����uncache�ڴ�ʧ�� */
    /* �Ҳ�������ʧ�����޵ĵ�,����ЩƵ���Ǻõ�,������˫������̫�� */
    RF_CAL_SELF_CAL_IP2_GREATER_THAN_FAIL_THRESHOLD = 3,
    RF_CAL_SELF_CAL_IP2_SAMPLE_DATA_FAIL            = 4,    /* ����ʧ��,˵��EDMAͨ������������� */
    RF_CAL_SELF_CAL_IQMISMATCH_CORR_FAIL            = 5,    /* BBP�������ʧ�� */
    RF_CAL_SELF_CAL_IQMISMATCH_OVER_CODE            = 6,    /* A,P,I,Q��������һ�����ֳ���[-64,63] */
    RF_CAL_SELF_CAL_DCR_CORR_FAIL                   = 7,    /* DCR�������ʧ�� */
    RF_CAL_SELF_CAL_DCR_ITERATIVE_FAIL              = 8,    /* ����������Ȼ����������Ҫ��У׼ʧ�� */
    RF_CAL_SELF_CAL_LOLEAKAGE_MEM_ALLOC_FAIL        = 9,    /* �����ڴ�ʧ�� */
    RF_CAL_SELF_CAL_LOLEAKAGE_SAMPLE_DATA_FAIL      = 10,   /* Lo Leakage����ʧ�� */
    RF_CAL_SELF_CAL_LOLEAKAGE_CORR_FAIL             = 11,   /* �������ʧ�� */
    RF_CAL_SELF_CAL_LOLEAKAGE_CALC_FAIL             = 12,   /* Lo Leakage����ʧ�� */
    RF_CAL_SELF_CAL_IQMISMATCH_SAMPLE_DATA_FAIL     = 13,   /* IQMISMATCH�����ݴ��� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP2_MCTMP_FAIL     = 14,   /* ����TX IQMC_Tmp �� MRXIQMCֵ���� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP2_AP_FAIL        = 15,   /* �����һ��A��Pֵ���� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_IR_FAIL        = 16,   /* ����IR_Linear���� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_ANGLE_FAIL     = 17,   /* ��������Ƕȴ��� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_AP_FAIL        = 18,   /* ����ڶ���A��Pֵ���� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP4_IR_FAIL        = 19,   /* ����IR1���� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP4_AP_FAIL        = 20,   /* ���������A��Pֵ���� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP5_IR_FAIL        = 21,   /* ����IR2���� */
    RF_CAL_SELF_CAL_IQMISMATCH_5TIMES_FAIL          = 22,   /* ����IR2���� */
    RF_CAL_SELF_CAL_SYS_TIME_SYN_FAIL               = 23,   /* CFN/SFNͬ��ʧ�� */
    RF_CAL_SELF_CAL_LOLEAKAGE_STEP1_SAMPLE_DATA_FAIL  = 24, /* LOLeakge Step1����ʧ�� */
    RF_CAL_SELF_CAL_LOLEAKAGE_STEP2_SAMPLE_DATA_FAIL  = 25, /* LOLeakge Step2����ʧ�� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP1_SAMPLE_DATA_FAIL = 26, /* IQ Step1�����ݴ��� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP2_SAMPLE_DATA_FAIL = 27, /* IQ Step2�����ݴ��� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_SAMPLE_DATA_FAIL = 28, /* IQ Step3�����ݴ��� */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP4_SAMPLE_DATA_FAIL = 29, /* IQ Step4�����ݴ��� */
    RF_CAL_SELF_CAL_ET_DPD_MEM_ALLOC_FAIL             = 30, /* DPDУ׼ʱ,����uncache�ڴ�ʧ�� */
    RF_CAL_SELF_CAL_ET_DPD_DATA_SYNC_FAIL             = 31, /* ����ͬ��ʧ�� */
    RF_CAL_SELF_CAL_ET_DPD_MISS_FIRST_POINT           = 32, /* ����ͬ��ʧ�ܣ�û�вɵ�����ͻ�� */
    RF_CAL_SELF_CAL_DPD_AMAM_CALC_FAIL                = 33, /* DPD AMAM������� */
    RF_CAL_SELF_CAL_DPD_SAMPLE_DATA_FAIL              = 34, /* �����ݴ��� */
    RF_CAL_SELF_CAL_DPD_CALC_AMPM_FAIL                = 35, /* ����ƽ����λƽ������ʧ�� */
    RF_CAL_SELF_CAL_ET_SAMPLE_DATA_FAIL               = 36, /* ET�����ݴ��� */
    RF_CAL_SELF_CAL_ET_GET_COMPRESS_POINT_FAIL        = 37, /* �����ݴ��� */
    RF_CAL_SELF_CAL_ET_TIME_DELAY_SAMPLE_DATA_FAIL    = 38, /* ET_TIME_DELAY�����ݴ��� */
    RF_CAL_SELF_CAL_IQMISMATCH_SYNC_FAIL              = 39, /* ��ȡͬ��λ��ʧ�� */
    RF_CAL_SELF_CAL_IQMISMATCH_SET_SAMPLE_TIME_FAIL   = 40, /* ���Ҷ�ʱ������ʼʱ��ʧ�� */
    RF_CAL_SELF_CAL_IQMISMATCH_DATA_TRANSFER_FAIL     = 41, /* ����ʧ��,DMA��BBPMASTER */
    RF_CAL_SELF_CAL_ET_TIME_DELAY_CENTER_NV_ERROR     = 42, /* NV��������delay�з��գ����ܵ������б����㲻������͵� */
    RF_CAL_SELF_CAL_IQMISMATCH_ANGLE_CALC_FAIL        = 43, /* c/a�鲿Ϊ����������+90��Ҫ�� */
    RF_CAL_SELF_CAL_ET_DELAY_ACLR_MEM_ALLOC_FAIL      = 44, /* Delay����Aclrʱ�����ڴ�ʧ�� */
    RF_CAL_SELF_CAL_ET_DELAY_ACLR_CENTER_SQUARE_FULL  = 45, /* Delay����Aclrʱ�����ź�ƽ������� */
    RF_CAL_SELF_CAL_DPD_FAST_SELF_CAL_MASK_FAIL       = 46, /* �����·�DPDУ׼MASKλ���� */
    RF_CAL_SELF_CAL_IP2_DCR_FAIL                      = 47, /* IP2У׼ʱȥֱ��ʧ�� */
    RF_CAL_SELF_CAL_MRX_DCOFFSET_LOST_ISR             = 48, /* MRX_DCOFFSETû�յ��ж� */
    RF_CAL_SELF_CAL_MRX_DCOFFSET_ITERATIVE_FAIL       = 49, /* MRX_DCOFFSET���ε���ʧ�� */
    RF_CAL_SELF_CAL_READ_HKADC_FAIL                   = 50, /* ��ѯPD��ѹʧ�� */
    RF_CAL_SELF_CAL_GET_PD_POWER_FAIL                 = 51, /* ��ѯPD��ѹʧ�� */
    RF_CAL_SELF_CAL_ET_PA_GAIN_FAIL                   = 52, /* PA GainУ׼ʧ�� */
    W_TX_SYMBOL_CAL_SAMPLE_TIMEOUT                    = 53, /* SYMBOL��TXУ׼Mrx ������ʱ */
    RF_CAL_SELF_CAL_CLT_MEM_ALLOC_FAIL                = 54, /* �ջ�tunerУ׼�ڴ�����ʧ�� */
    RF_CAL_SELF_CAL_CLT_DETECT_DATA_FAIL              = 55, /* �ջ�tunerУ׼����ʧ�� */
    RF_CAL_SELF_CAL_CLT_RSLT_MEAS_INVALID_FAIL        = 56, /* �ջ�tuner�������㷨��������� */
    RF_CAL_SELF_CAL_CLT_UNSUPPORT_FAIL                = 57, /* ��֧�ֱջ� */
    RF_CAL_SELF_CAL_IQMISMATCH_TRANSIT_DATA_FAIL      = 1000, /* ����ʧ�� */
    RF_CAL_SELF_CAL_BUTT
};
typedef VOS_UINT16 RF_CAL_SelfCalErrCodeUint16;

/*
 * ö��˵��:RF CAL W TRX FAST ENUM
 */
enum W_TX_RficMode {
    W_TX_RFIC_MODE_NORMAL = 0,
    W_TX_RFIC_MODE_VOICE,
    W_TX_RFIC_MODE_BUTT
};
typedef VOS_UINT16 W_TX_RficModeUint16;

/*
 * ö��˵��:RF CAL W TRX FAST ENUM
 */
enum W_TX_PowerCtrlModeType {
    W_TX_CAL_POWER_CTRL_POWER = 0,
    W_TX_CAL_POWER_CTRL_POWER_ATTEN,
    W_TX_CAL_POWER_CTRL_REGISTER,
    W_TX_CAL_POWER_CTRL_BUTT
};
typedef VOS_UINT16 W_TX_PowerCtrlModeUint16;

/*
 * ö��˵��:RF CAL W TRX FAST ENUM
 */

enum W_FAST_StepWidth {
    WIDTH_2MS  = 2,
    WIDTH_10MS = 10,
    WIDTH_20MS = 20,
    WIDTH_BUTT
};
typedef VOS_UINT16 W_FAST_StepWidthUint16;

/* W PAģʽ */
enum W_TX_PaMode {
    PA_MODE_AUTO = 0,
    PA_MODE_HIGH,
    PA_MODE_MID,
    PA_MODE_LOW,
    PA_MODE_BUTT
};

enum W_RX_Ant {
    W_RX_ANT1 = 1,  // main ant
    W_RX_ANT2 = 2,  // diversity ant
    W_RX_BUTT
};

/* W Rx �ز�ģʽ 0:���ز�, 1:˫�ز� */
enum W_RX_CarrierMode {
    W_RX_CARRIER_MODE_SC = 0,
    W_RX_CARRIER_MODE_DC = 1,
};

/* W Rx BLOCKģʽ 0: No Block,   1:Block; */
enum W_RX_BlockMode {
    W_RX_WITHOUT_BLOCK = 0,
    W_RX_WITH_BLOCK,
};

/* W Rx ��λ����ģʽ 0:��λ�Զ����� 1:��λǿ�� */
enum W_RX_AgcCtrlMode {
    W_RX_AGC_CTRL_AUTO = 0,
    W_RX_AGC_CTRL_APPOINTED,
};

/*
 * ö��˵��:
 */
enum G_TX_ModulationType {
    MODE_GMSK = 0,
    MODE_8PSK = 1,
    MODE_CW,
    MODE_BUTT
};
typedef VOS_UINT16 G_TX_ModulationTypeUint16;

/*
 * ö��˵��:
 */
enum G_TX_PowerCtrlMode {
    G_TX_CAL_POWER_CTRL_TX_AGC = 0,
    G_TX_CAL_POWER_CTRL_POWER,
    G_TX_CAL_POWER_CTRL_POWER_ATTEN,
    G_TX_CAL_POWER_CTRL_REGISTER,
    G_TX_CAL_POWER_CTRL_BUTT
};
typedef VOS_UINT16 G_TX_PowerCtrlModeUint16;

enum G_RX_WaveType {
    RX_BURST     = 0,
    RX_CONTINOUS = 1,
    RX_TYPE_BUTT
};

enum G_RX_MeasMode {
    AGC_SLOW = 0,
    AGC_FAST = 1,
    AGC_BUTT
};

/*
 * ö��˵��:RF CAL C TRX FAST ENUM
 */

enum C_FAST_StepWidth {
    CSDR_2PCG_STEP_WIDTH_LENGTH = 2500,
    CSDR_4PCG_STEP_WIDTH_LENGTH = 5000,
    CSDR_8PCG_STEP_WIDTH_LENGTH = 10000,
    CSDR_PCG_STEP_WIDTH_LENGTH_BUTT
};
typedef VOS_UINT16 C_FAST_StepWidthUint16;

/*
 * ö��˵��:RF CAL TRX FAST STEP WIDTH ENUM
 */
enum HI6363_FAST_StepWidth {
    STEP_WIDTH_0US     = 0,     /* symbol������У׼ */
    STEP_WIDTH_2000US  = 2000,  /* 2ms */
    STEP_WIDTH_2500US  = 2500,  /* 2.5ms,�൱��Cģ��2��PCG */
    STEP_WIDTH_5000US  = 5000,  /* 5ms,�൱��Cģ��4��PCG */
    STEP_WIDTH_10000US = 10000, /* 10ms,�൱��Cģ��8��PCG */
    STEP_WIDTH_20000US = 20000, /* 20ms */
    STEP_WIDTH_BUTT
};
typedef VOS_UINT16 HI6363_FAST_StepWidthUint16;

/*
 * ö��˵��:W TX����ģʽ
 */
enum W_TX_ModMode {
    W_TX_MODU_MODE   = 0, /* �����ź� */
    W_TX_SINGLE_MODE = 1, /* �����ź� */
    W_TX_MOD_MODE_BUTT
};
typedef VOS_UINT16 W_TX_ModModeUint16;
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
 * �ṹ˵��: �ӿ�ͷ�ļ�ת����MIPI������ƽṹ�壬��RF_NV_MIPIDEV_CMD_STRU
 */
typedef struct {
    VOS_UINT32 bitByteCnt : 5;     /* 0��ʾ��CMMAND��Ч��ȡֵ0~4 */
    VOS_UINT32 bitMipiPortSel : 3; /* mipi port 0~7 */
    VOS_UINT32 bitSlaveID : 4;     /* slave id 0~15 */
    VOS_UINT32 bitRegAddr : 16;    /* reg addr 0~65535 */
    VOS_UINT32 bitRsv : 4;         /* ���ֽڶ��� */
} PHY_CAL_MipidevCmd;

/*
 * �ṹ˵��: �ӿ�ͷ�ļ�ת����MIPI���ݽṹ�壬��RF_NV_MIPIDEV_DATA_STRU�ṹһ��
 */
typedef struct {
    VOS_UINT32 bitByte0 : 8; /* ��һ��byte */
    VOS_UINT32 bitByte1 : 8; /* �ڶ���byte */
    VOS_UINT32 bitByte2 : 8; /* ������byte */
    VOS_UINT32 bitByte3 : 8; /* ���ĸ�byte */
} PHY_CAL_MipidevData;

/*
 * �ṹ˵��: �ӿ�ͷ�ļ�ת����MIPI������С��Ԫ���ṹ��RF_NV_MIPIDEV_UNIT_STRUһ��
 */
typedef struct {
    PHY_CAL_MipidevCmd  cmd;  /* ������ */
    PHY_CAL_MipidevData data; /* ������,һ��MIPIռ��2��UINT32 */
} PHY_CAL_MipidevUnit;

typedef struct {
    VOS_INT16 real; /* ����ʵ�� */
    VOS_INT16 img;  /* �����鲿 */
} PHY_CAL_ComplexDataInt16;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ��� ID */
} PHY_CAL_MsgDead;

/*
 * �ṹ˵��: WCDMA RF����У׼�ӿ� -- UPA���� ( ��ʱ�������� )
 */
typedef struct {
    /* ����Ĳ����Ƿ���UPA��Ҫʹ�õ� */
    VOS_UINT16 bec;
    VOS_UINT16 bed1;
    VOS_UINT16 bed2;
    VOS_UINT16 bc;
    VOS_UINT16 bd;
    VOS_UINT16 txSlotFormat; /* ����ģʽ:�������ʱ϶��ʽ */
    VOS_UINT16 txChanNum;    /* ������� */
} PHY_RF_CalWTxUpaPara;

/*
 * �ṹ˵��: WCDMA RF����У׼�ӿ� -- DPA����
 */
typedef struct {
    /* ����Ĳ����Ƿ���DPA��Ҫʹ�õ� */
    VOS_UINT16 bc;
    VOS_UINT16 bd;
    VOS_UINT16 bhs;
    VOS_UINT16 rsv;
} PHY_RF_CalWTxDpaPara;

typedef struct {
    VOS_UINT16 rficGainCtrl;
    VOS_INT16  dbbAtten10th;
} PHY_RF_CalWTxRegCtrl;

typedef struct {
    VOS_UINT32 rficGainCtrl : 24;
    VOS_INT32  dbbAtten10th : 8;
} PHY_RF_CalWTxRegCtrlHi6363;

typedef union {
    VOS_INT16                            txPower10th;
    VOS_INT16                            txPowerAtten10th;
    PHY_RF_CalWTxRegCtrlHi6363           txRegCtrlPara;
} PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION;

typedef struct {
    VOS_INT16  afc;        /* AFC������ */
    VOS_UINT16 cfix  : 8;  /* ��DCXO c1 �Ŀ����֣�ȡֵ��ΧΪ[0,255] */
    VOS_UINT16 c2fix : 8; /* ��DCXO C2 �Ŀ����֣�ȡֵ��ΧΪ[0,7] */
} PHY_RF_CalAfcPara;

typedef union {
    VOS_INT16                     txPower10th;
    VOS_INT16                     txPowerAtten10th;
    PHY_RF_CalWTxRegCtrl          txRegCtrlPara;
} PHY_RF_CAL_W_TX_POWER_PARA_UNION;

/*
 * �ṹ˵��: WCDMA RF������У׼�ӿ�Rx Agc Gain����λ����
 *           BIT[15] : 0:No Block�� 1:Block;
 *           BIt[14] : 0:���ز���   1��˫�ز�
 *           BIT[13] : 0:��λ�Զ����� 1:��λǿ��
 *           BIT[2:0]: 0:��һ��(������ߵ�),��������
 */
typedef struct {
    VOS_UINT16 agcGainLvl : 3;
    VOS_UINT16 : 5;
    VOS_UINT16 : 5;
    VOS_UINT16 gainLvlMode : 1; /* 0 - ��λ�Զ������� 1- ǿ�Ƶ�λ */
    VOS_UINT16 carrierMode : 1; /* 0 - ���ز��� 1 - ˫�ز� */
    VOS_UINT16 blockMode : 1;   /* 0 - no block, 1 - block */
} W_RX_AgcGainCfg;

/*
 * �ṹ˵��: WCDMA RF����У׼�ӿ�
 */
typedef struct {
    VOS_UINT16 paVcc : 8;  /* PA Vcc �ĵ�ѹ������ */
    VOS_UINT16 paBias : 8; /* PA Bias��ѹ�����֣�MMMB PAʱ��������Ч����ͨPAʱWDSPֱ�Ӷ������ֶ� */
} W_TX_PaParaCfg;

typedef struct {
    VOS_UINT16 pdDcOffset : 8; /* PD DCOFFSET��ֵ,ȡֵ��Χ[0,31] */
    VOS_UINT16 pdVga : 8;      /* PD VGA��ֵ,����bit1��ʾ˥���Ƿ�򿪣�Bit[6:4]��ʾVGA��ȡֵ */
} W_TX_PdParaCfg;

typedef struct {
    VOS_UINT16 rficMode : 8; /* RFIC�Ĺ���ģʽ��0:����ģʽ��1:Voiceģʽ */ /* W_TX_RficMode_UINT8 */
    VOS_UINT16 txPaMode : 8; /* 0:�Զ�����;1:������;2:������;3:������ */
} W_TX_RficPaMode;

/*lint -save -e959*/
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* ԭ������ */
    VOS_UINT16                  rsv;   /* ����λ   */

    VOS_UINT32 mask; /* ����λ������bitλ��ʾ������ֶ��Ƿ���Ч��bitλ��ӳ���ϵ�μ�MASK_W_TRX_CFG_* */

    /* Afc Para Cfg */
    PHY_RF_CalAfcPara afcPara; /* AFC������ */

    /* Tx Para Cfg */
    VOS_UINT16 txBand;    /* WģBAND:1��ʾBAND1,2��ʾBAND2,�������� */
    VOS_UINT16 txChannel; /* WģTXƵ���,��BAND1��9750 */
    /* ��8Bit���� ---Tx���� 0:��ʹ��,1:ʹ�ܸ�8Bit���� ---���Ʒ�ʽ 0:��������,1:CW�����ź�, 2:˫���ź� */
    VOS_UINT16 txEnable;
    W_TX_RficPaMode                  txRficPaMode;
    /*
     * ���ʿ���ģʽ����unTxPowerParaʹ��ͬ�������롣
     * 0������ģʽ����ʱunTxPowerPara.usTxPower10th��Ч��
     * 1������˥��ģʽ����ʱunTxPowerPara.usTxPowerAtten10th��Ч��
     * 2���Ĵ������Ʒ�ʽ����ʱunTxPowerPara.stTxRegCtrlPara��Ч��
     */
    W_TX_PowerCtrlModeUint16         txPowerCtrlMode;
    /* ���ʿ��Ʋ�������ucTxPowerCtrlModeʹ��ͬ�������� ע��:�ñ���ֻ����RF6362��RF6363���ʿ�����unHi6363TxPowerPara */
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;

    /* Pa Volt Para Cfg */
    W_TX_PaParaCfg paParaCfg;
    /*
     * ��У׼��Ŀ�����и�8bit��
     *  0 - ����ģʽ�����ߴ���У׼ģʽ�ص�����ģʽ
     *  1 - W IIP2��У׼����ģʽ����ʱ���߱�֤�·���Tx channel = Rx Channel
     *  2 - W IQ Mismatch��У׼ģʽ����ʱ���߱�֤�·���Rx Channel = Tx Channel
     *  3 - G IQ Mismatch��У׼ģʽ��GSMģʽ�±�����ֻ��1���ŵ��ţ���ʱ��DSP�Լ�ѡ��
     *      Tx Band��Tx channel
     *      ����Tx Band���壺 0-GSM850,1-GSM900��2-DCS1800��3-PCS1900
     *  4 - W ET LUT��У׼
     *  5 - W DPD LUT��У׼
     * ���е�8bit���壺
     * W IIP2У׼ģʽ�£�
     * 0 - ����+�ּ�
     * 1 - ����
     * W/G IQ MismatchУ׼ģʽ�£�
     * 0 - feedback
     * 1 - ֱ�ӷ��͵����߿�
     *
     * ע�⣺����У׼ģʽ�£���������ҲӦ������������
     * ע�⣺�������У׼ģʽ���˵�����ģʽ��DSP��Ҫ��RFIC��BBP���û�����ģʽ
     */
    VOS_UINT16 selfCal;

    /* Rx Para Cfg */
    VOS_UINT16 rxBand;         /* WģBAND:1��ʾBAND1,2��ʾBAND2,�������� */
    VOS_UINT16 rxChannel;      /* WģRXƵ���,��BAND1��10700 */
    /*
     * ��Ƶ������Band������û�ʹ�ܸ��ֶ�*����AGC������ΪusRxBand��Ӧ��AGC��
     * ������Band��Ƶ������ΪusRxCrossBand��usRxCrossChannel
     */
    VOS_UINT16 rxCrossBand;
    VOS_UINT16 rxCrossChannel; /* ��Ƶ������Ƶ�� */

    VOS_UINT16 rxEnable;       /* 0:��ʹ��,1:ʹ�� */
    VOS_UINT16 rxAntSel;       /* 1:��������;2:�ּ����� */
    /*
     * BIT[15] : 0:No Block�� 1:Block;
     * BIt[14] : 0:���ز���   1��˫�ز�
     * BIT[13] : 0:��λ�Զ����� 1:��λǿ��
     * BIT[2:0]: 0:��һ��(������ߵ�),��������
     */
    W_RX_AgcGainCfg        rxAgcGain;
    VOS_UINT16             mrxGain;   /* MRX Gain��λ */

    /* ����Ĳ����Ƿ���UPA��Ҫʹ�õ�--Ϊ�˲�Ӱ�����е�AT����,���ڸýṹ�����������ر��� */
    VOS_UINT16                    upaEnable;  /* LMT����UPA�źŴ򿪹ر�.0:�ر�;1:�� */
    VOS_UINT16                    txCarrMode; /* TX�ز�ģʽ.0:���ز�;1:˫�ز� */
    PHY_RF_CalWTxUpaPara          primCarr;   /* ���ز���صĲ��� */
    PHY_RF_CalWTxUpaPara          secondCarr; /* ���ز���صĲ��� */

    VOS_UINT16                    dpaEnable; /* LMT����DPA�źŴ򿪹ر�.0:�ر�;1:�� */
    VOS_UINT16                    rsv4;
    PHY_RF_CalWTxDpaPara          dpaPara; /* DPA��صĲ��� */

    VOS_INT16 ip2IValue; /* IP2 Iͨ������ */
    VOS_INT16 ip2QValue; /* IP2 Qͨ������ */

    VOS_UINT16 evmIndOnOff;      /* EVM�����ϱ��Ƿ�ʹ�ܣ�1��ʾ���ϱ���0��ʾ�ر��ϱ� */
    VOS_UINT16 enableReportData; /* EVMԭʼ�����Ƿ��ϱ���1��ʾ�ϱ���0��ʾ���ϱ� */
    /* ET delay �� MASK_W_TRX_CFG_TX_ET_TIMEDELAY ���� */
    VOS_INT16 signalDelay;  /* �ź�ͨ��ֻ��������ʱ�Ĵ�������  */
    VOS_INT16 envelopDelay; /* ����ͨ����ʱ,��λΪ1/1024sample */

    /* ������һ��dpd����Χ0~9 ���� MASK_W_TRX_CFG_TX_DPD ���� */
    VOS_INT16 dpdTableIndex;

    /*
     * IQ�������ã����� MASK_W_TRX_CFG_TX_V_IQ ����
     * ����û���������Ҫ����IQ����ʹ�� MASK_W_TRX_CFG_PD_ONOFF
     * ��DSP�����Viq�����ҵ�����ѹ���㣬ͨ��PD�ϱ�ԭ���ϱ�����ѹ����ʱ��PD���ֵ
     * PD�ϱ�ԭ��μ�PHY_RF_CalWRfTxPdInd
     */
    VOS_INT16 viq;
    VOS_INT16 viqStep;

    /* ��MASK_W_TRX_CFG_TX_ET_COMP���� */
    VOS_INT16  etPowerCompression; /* ETУ׼ʱ������ѹ���㣬��λ0.1dB */
    VOS_UINT16 enableEtDpdComp;   /* BIT0:��ʾET����ʹ�ܣ�BIT1��ʾDPD����ʹ�� */
    VOS_UINT16 rsv5;
    VOS_INT16  etFreqTempComp;  /* ETƵ���²�����λ0.1dB */
    VOS_INT16  dpdFreqTempComp; /* DPDƵ���²�����λ0.1dB */

#if (FEATURE_ON == FEATURE_HI6363)
    VOS_UINT16                              dpdStepV;
    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION hi6363TxPowerPara; /* RF6363 ���ʿ��� */
#endif

    PHY_CAL_MipidevUnit cltHvdacMipi[PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM];
} PHY_RF_CalWTrxParaCfgReq;
/*lint -restore*/

/*
 * �ṹ˵��: W PD��������ԭ��
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* ԭ������ */
    VOS_UINT16                  rsv;   /* ����λ */

    VOS_UINT16                  mask; /* refer to MASK_CAL_W_PD_* */

    /* PD Cfg */
    VOS_UINT16                  pdInputThrough; /* 0 - �ر�PD�������źţ� 1 - ��PD�������ź� */
    W_TX_PdParaCfg              txPdPara;       /* PD VGA���� PD DC�Ĳ��� */
} PHY_RF_CalWPdParaCfgReq;

/*
 * �ṹ˵��: W TRX ������У׼ͨ��CNF
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ��� ID */
    VOS_UINT16                  recMsgID;                /* ��Ӧ���·�Msg ID */
    VOS_UINT16                  errorCode;
} PHY_RF_CalWRfMsgCnf;

/*
 * �ṹ˵��: W DCXOУ׼ͨ��CNF
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;
    VOS_UINT16                  recMsgID; /* ��Ӧ���·�Msg ID */
    VOS_UINT16                  errorCode;
} PHY_RF_CalWRfDcxoCalMsgCnf;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ��� ID */
    VOS_INT16                   pdHkadc;                  /*  Hi6362ʱ��PdHkacΪ2�ֽ� */
    VOS_INT16                   rsv;
    VOS_UINT32                  mrxCorr; /* Hi6363ʱ���ϱ�ֵΪMrx corr(4�ֽ�) */
} PHY_RF_CalWRfTxPdInd;

typedef struct {
    VOS_UINT16                      band;
    VOS_UINT16                      rsv;
    PHY_CAL_ComplexDataInt16        dpdLut[128];
} ANYBAND_W_DpdLutCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16                      errCode; /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                      errBand; /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16                      rsv;
    VOS_UINT16                      bandCnt; /* ���DSPƵ�θ��� */
    ANYBAND_W_DpdLutCalData         calData[SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCommDataDpdLutRpt;

/*
 * �ṹ˵��: ET�ϱ�����
 */
typedef struct {
    VOS_UINT32 bitMap; /* У׼MASKλ */
    VOS_UINT32 len;    /* ��¼У׼�ṹ�峤��,byteΪ��λ */
    VOS_INT16  pdHkadc;
    VOS_UINT16 compressSingleToneV;
    VOS_UINT32 compressMrxCorr; /* MRX��·�������ֵ */
    VOS_UINT32 mrxCorr[64];     /* 64��MRXֵ */
} PHY_RF_CalCommDataEtLutRpt;

typedef struct {
    VOS_INT16 asEtDelayFreqComp[8];
} NV_W_EtDelayFreqComp;

typedef struct {
    VOS_UINT16                   bandId;
    VOS_UINT16                   rsv;
    NV_W_EtDelayFreqComp         nvEtDelayFreqComp;
} ANYBAND_W_EtDelayCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16                       errCode;     /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                       errBand;     /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16                       selfCalMask; /* ���DSP��У׼�� */
    VOS_UINT16                       bandCnt;     /* ���DSPƵ�θ��� */
    ANYBAND_W_EtDelayCalData         calData[SELF_CAL_BAND_ID_MAX_COUNT];
    VOS_UINT32                       warningCode; /* �����ϱ���Ϣ��װ����ʹ�� */
} PHY_RF_CalCommDataEtDelayRpt;

/*
 * �ṹ˵��: ͨ�������ϱ��ṹͷ
 */
typedef struct {
    PHY_CAL_MsgDead        msgHead;
    VOS_UINT32             testItemsCnt; /* ����ָʾ��������ĳ������,������Ӧbit */

    /*
     * ������ϱ������ݣ�����������ָʾ�������ݳ���
     * VOS_UINT32 ulTest1BitMap; ��һ����������ָʾ��������ĳ������,������Ӧbit
     * VOS_UINT32 ulTest1Len; ��һ������byte��������ulTest1BitMap(����)��ʼ��ulTest2BitMap(������)����
     * VOS_UINT16 usTest1Data[n]; ��һ������ VOS_UINT32 ulTest2BitMap;
     * �ڶ�����������ָʾ��������ĳ������,������Ӧbit VOS_UINT32 ulTest2Len;
     * �ڶ�������byte��������ulTest2BitMap(����)��ʼ
     * VOS_UINT16 usTest2Data[n]; �ڶ�������
     * .......... ��������
     */
} PHY_CAL_RfCommDataRptMsgHead;

/*
 * Gģ��У׼����ԭ��
 */
typedef struct {
    VOS_UINT32 testItemsCnt;                                 /* У׼����� 0-31 */
    /* ÿ��������ֻ��ʾһ��У׼��������ظ���У׼�����㰴�·�˳��ȫ���ϱ� */
    VOS_UINT32 testItemsBitmap[COMM_SELF_CAL_TESTITEM_NUM];

    VOS_UINT32 bandMask;    /* ����Bitλ�ж��Ƿ�֧����ЩBand�����и���Msg ID�ж���W band����G Band */
    VOS_UINT32 bandExtMask; /* ��չ�ã�ĿǰԤ�� */
} GPHY_RF_CalCommSelfCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16          msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    rsv;

    GPHY_RF_CalCommSelfCalReqData data;
} GPHY_RF_CalCommSelfCalReq;

/*
 * �ṹ˵��: MRX DC ��Ҫ�ϱ���ֱ������
 */
typedef struct {
    VOS_INT16 dci;       /* У׼����DCI */
    VOS_INT16 dcq;       /* У׼����DCQ */
    VOS_INT16 reMainDci; /* У׼����DCI */
    VOS_INT16 reMainDcq; /* У׼����DCQ */
} GPHY_RF_MrxDcoffsetIq;

/*
 * Gģ��У׼�ϱ�ԭ��
 */
typedef struct {
    GPHY_RF_MrxDcoffsetIq mrxDc[5];
} NV_MRX_Dcoffset;

typedef struct {
    VOS_UINT16           bandId;
    VOS_UINT16           rsv;
    NV_MRX_Dcoffset      nvMrxDc;
} ANYBAND_G_MrxDcoffsetCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16                           errCode; /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                           errBand; /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16                           bandCnt; /* ���DSPƵ�θ��� */
    VOS_UINT16                           rsv;
    ANYBAND_G_MrxDcoffsetCalData         calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataMrxDcoffsetRpt;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;
    VOS_UINT16                  measNum;
    VOS_UINT16                  interval;
} PHY_RF_CalWRxRssiMeasReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ��� ID */
    VOS_INT16                   rssi;
    VOS_UINT16                  agcState;
} PHY_RF_CalWRxRssiMeasInd;

/*
 * �ṹ˵��: WCDMA RF Self Cal�ӿ�
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;
    VOS_UINT16                  selfCalMask; /* reference to MASK_SELF_CAL_* */
    VOS_UINT16                  modemIndex;  /* 0-modem0,1-modem1 */
    VOS_UINT32                  bandMask;    /* ����Bitλ�ж��Ƿ�֧����ЩBand�����и���Msg ID�ж���W band����G Band */
    VOS_UINT32                  bandExtMask; /* ��չ�ã�ĿǰԤ�� */
} PHY_RF_CalWSelfCalReq;

typedef struct {
    VOS_INT16 amp;
    VOS_INT16 phase;
    VOS_INT16 dci;
    VOS_INT16 dcq;
} NV_TX_IqMismatch;

typedef struct {
    VOS_INT16 ampCorr;
    VOS_INT16 dcCorr;
} RESULT_TX_IqMismatch;

typedef struct {
    VOS_INT16 i;
    VOS_INT16 q;
} RX_IQ;

typedef struct {
    RX_IQ noBlockDcOffset[W_RX_NOBLOCK_AGC_GAIN_COUNT];
    RX_IQ blockDcOffset[W_RX_BLOCK_AGC_GAIN_COUNT];
} ANY_CARRIER_DcOffset;

typedef struct {
    ANY_CARRIER_DcOffset carrier[2]; /* ����0��ʾSC��1��ʾDC */
} ANY_ANT_AnyCarrierDcOffset;

typedef struct {
    ANY_ANT_AnyCarrierDcOffset ant[2]; /* ����0��ʾAT1,1��ʾAT2 */
} NV_W_RxDcoffset;

typedef NV_W_RxDcoffset RESULT_W_RX_DCOFFSET_STRU;

typedef struct {
#if (FEATURE_ON == FEATURE_HI6363)
    RX_IQ           ip2Iq[2]; /* 0:SC   1:DC */
#else
    RX_IQ           divOn;
    RX_IQ           divOff;
#endif
} ANY_CHAN_WRxIp2;

typedef struct {
    ANY_CHAN_WRxIp2 chan[3]; /* 0�����ŵ���1�����ŵ���2�����ŵ� */
} NV_W_RxIp2;

typedef struct {
    VOS_INT16 imd2;
    VOS_INT16 rsv;
} W_RX_Imd2;

typedef struct {
#if (FEATURE_ON == FEATURE_HI6363)
    W_RX_Imd2       ip2Imd2[2]; /* 0:SC   1:DC */
#else
    W_RX_Imd2       divOn;
    W_RX_Imd2       divOff;
#endif
} ANY_CHAN_WRxImd2;

typedef struct {
    ANY_CHAN_WRxImd2 chan[3]; /* 0�����ŵ���1�����ŵ���2�����ŵ� */
} RESULT_W_RxIp2;

typedef struct {
    VOS_INT16 dci;
    VOS_INT16 dcq;
    VOS_INT16 remainDci;
    VOS_INT16 remainDcq;
} NV_RX_DcoffsetIq;

typedef struct {
    NV_RX_DcoffsetIq vga[2][16]; /* ��Ӧ16��VGA��Dcֵ */
} NV_W_RxDcoffsetIq;

/*
 * �ṹ˵��: 0����SC��1�ּ�SC��2����DC��3�ּ�DC
 */
typedef struct {
    NV_RX_DcoffsetIq vga[4][16]; /* ��Ӧ16��VGA��Dcֵ */
} NV_W_RxDcoffsetIqScdc;

typedef struct {
    VOS_UINT16                 bandId; /* ��MASKλ�ϱ� */
    VOS_UINT16                 rsv;
    NV_TX_IqMismatch           nvTxIqMismatch;
    RESULT_TX_IqMismatch       resultTxIqMismatch;
    NV_W_RxDcoffset            nvRxDcOffset;
    RESULT_W_RX_DCOFFSET_STRU  resultRxDcOffset;
    NV_W_RxIp2                 nvRxIp2;
    RESULT_W_RxIp2             resultRxIp2;
} ANY_BAND_WgSelfCalInd;

typedef struct {
    VOS_UINT16                 bandId; /* ��MASKλ�ϱ� */
    VOS_UINT16                 rsv;
    NV_TX_IqMismatch           nvTxIqMismatch;  /* TX IQ Mismatch NV�� */
    NV_TX_IqMismatch           nvMrxIqMismatch; /* MRX IQ Mismatch NV�� */
    RESULT_TX_IqMismatch       resultTxIqMismatch;
} ANY_BAND_GTxIqmismatchCalInd;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16                            errCode; /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                            errBand; /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16                            bandCnt; /* ���DSPƵ�θ��� */
    VOS_UINT16                            rsv;
    ANY_BAND_GTxIqmismatchCalInd          calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataTxIqmismatchRpt;

typedef struct {
    PHY_TOOL_MsgIdUint16          msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    componentId;             /* ��� ID */
    VOS_UINT16                    errCode;                 /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                    errBand;                 /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16                    selfCalMask;             /* ���DSP��У׼�� */
    VOS_UINT16                    bandCnt;                 /* ���DSPƵ�θ��� */
    ANY_BAND_WgSelfCalInd         selfCalRlt[SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalWSelfCalInd;

typedef struct {
    PHY_TOOL_MsgIdUint16          msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                    componentId; /* ��� ID */

    VOS_UINT16                    errCode;     /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                    errBand;     /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16                    selfCalMask; /* ���DSP��У׼�� */
    VOS_UINT16                    bandCnt;     /* ���DSPƵ�θ��� */
    ANY_BAND_WgSelfCalInd         selfCalRlt[CDMA_SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCSelfCalInd;

/*
 * Hi6363 W/Cģ��У׼����ԭ��
 */
typedef struct {
    VOS_UINT32 testItemsCnt; /* У׼�������ָʾaulTestItemsBitmap�е���Ч��������Χ:����0С�ڵ���32 */
    /* ÿ��������ֻ��ʾһ��У׼��������ظ���У׼�����㰴�·�˳��ȫ���ϱ���һ��REQֻ��Ӧһ���ϱ� */
    VOS_UINT32 testItemsBitmap[SELF_CAL_ITEMS_MAX_COUNT];
    VOS_UINT32 bandMask;    /* ����Bitλ�ж��Ƿ�֧����ЩBand�����и���Msg ID�ж���W band����G Band */
    VOS_UINT32 bandExtMask; /* ��չ�ã�ĿǰԤ�� */
} PHY_RF_CalCommSelfCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16         msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   rsv;

    PHY_RF_CalCommSelfCalReqData data;
} PHY_RF_CalCommSelfCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16         msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                   rsv0;

    VOS_UINT16 band;
    VOS_UINT16 rsv1;
    VOS_UINT32 txArfcn; /* WΪ100kHzΪ��λ��CΪkHzΪ��λ */
} PHY_RF_CalAsignedChanDpdSelfCalReq;

/*
 * Hi6363 W/Cģ��У׼�ϱ�ԭ��
 */
typedef struct {
    VOS_UINT32 bitMap; /* ָʾУ׼����(MASK)�����ڹ��߽��� */
    VOS_UINT32 len;    /* ��usErrCode����������У׼�������һ���ֽڵ�byte���� */

    VOS_UINT16 errCode; /* У׼ʧ�ܺ��error code,Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16 errBand; /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16 bandCnt; /* ���У׼��Ƶ�θ��� */
    VOS_UINT16 errFreq; /* ���У׼ʧ�ܵ�Ƶ�� */
} PHY_RF_SelfCalRptCommInfo;

typedef struct {
    VOS_INT16 dci;
    VOS_INT16 dcq;
    VOS_INT16 remainDci;
    VOS_INT16 remainDcq;
} WPHY_RF_MrxDcoffsetIq;

typedef struct {
    WPHY_RF_MrxDcoffsetIq mrxDc[5];
} NV_W_MrxDcoffset;

typedef struct {
    VOS_UINT16             bandId;
    VOS_UINT16             rsv;
    NV_W_MrxDcoffset       nvMrxDc;
} ANYBAND_W_MrxDcoffsetCalData;

typedef struct {
    PHY_RF_SelfCalRptCommInfo    rptCommInfo;
    ANYBAND_W_MrxDcoffsetCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} WPHY_RF_CalCommDataMrxDcoffsetRpt;

typedef struct {
    VOS_UINT16             bandId;
    VOS_UINT16             rsv;
    NV_TX_IqMismatch       nvTxIqMismatch;  /* TX IQ Mismatch NV�� */
    NV_TX_IqMismatch       nvMrxIqMismatch; /* MRX IQ Mismatch NV�� */
    VOS_UINT32             txIr;
} ANYBAND_W_TxMrxIqmismatchCalData;

typedef struct {
    PHY_RF_SelfCalRptCommInfo        rptCommInfo;
    ANYBAND_W_TxMrxIqmismatchCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} WPHY_RF_CalCommDataTxMrxIqmismatchRpt;

typedef struct {
    VOS_UINT16                    bandId;
    VOS_UINT16                    rsv;
    NV_W_RxDcoffsetIqScdc         nvDcOffset;
} ANYBAND_W_RxDcoffsetCalData;

typedef struct {
    PHY_RF_SelfCalRptCommInfo   rptCommInfo;
    ANYBAND_W_RxDcoffsetCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} WPHY_RF_CalCommDataRxDcoffsetRpt;

typedef struct {
    VOS_UINT16           bandId;
    VOS_UINT16           rsv;
    NV_W_RxIp2           nvIp2;     /* У׼��� */
    RESULT_W_RxIp2       resultIp2; /* IMD2 */
} ANYBAND_W_RxIp2CalData;

typedef struct {
    PHY_RF_SelfCalRptCommInfo  rptCommInfo;
    ANYBAND_W_RxIp2CalData     calData[SELF_CAL_BAND_ID_MAX_COUNT];
} WPHY_RF_CalCommDataRxIp2Rpt;

typedef struct {
    VOS_UINT16 bandId;
    VOS_UINT16 rsv;
    VOS_INT16  asEtDelayFreqComp[8];
} ANYBAND_W_TxEtDelayRpt;

typedef struct {
    PHY_RF_SelfCalRptCommInfo  rptCommInfo;
    ANYBAND_W_TxEtDelayRpt     calData[SELF_CAL_BAND_ID_MAX_COUNT];
} WPHY_RF_CalCommDataTxEtDelayRpt;

/*
 * �ṹ˵��: DPD LUT
 */
typedef struct {
    VOS_INT32 dpdCompI : 12; /* bit[0-11] : DPD����I·ϵ�� */
    VOS_INT32 dpdCompQ : 12; /* bit[12-23]: DPD����Q·ϵ�� */
    VOS_INT32 reserved : 8;  /* bit[24-31]: ���� */
} NV_W_DpdCompPara;

typedef struct {
    VOS_UINT16              band;
    VOS_UINT16              rsv;
    VOS_UINT16              stepV[2];
    NV_W_DpdCompPara        dpdLut[2][128];
} ANYBAND_W_TxDpdLutData;

typedef struct {
    VOS_UINT16 etDelay;    /* Delayֵ */
    VOS_INT16  etDelayAclr; /* Delayֵ��Ӧ��ACLR�������λ0.125dBc */
} NV_W_TxEtDelayData;

typedef struct {
    /* �ϱ��ĸ�������Ϊ1��ʾ����Ҫ���������������ʾPHY�ϱ�ֵ��ΪУ׼��� */
    VOS_UINT32                 lsRptCnt;
    VOS_UINT32                 lsFreq;          /* ����Ƶ�ʣ�WΪ100KHz,CΪKHz */
    NV_W_TxEtDelayData         delayData[1]; /* PHY����delay�Ľ������ʵ����ΪlsRptCnt */
} NV_W_TxEtDelayRpt;

typedef struct {
    VOS_UINT16 bandId;
    VOS_UINT16 freqNum;                        /* У׼��Ƶ����� */
    /* ��������׵�ʱ�����ϱ�ֵ����õ�(ACLR���)����usSearchOffset��Χ������usSearchStep���� */
    VOS_UINT16 searchOffset;
    VOS_UINT16                searchStep;      /* �������� */
    NV_W_TxEtDelayRpt         etDelayData[1]; /* У׼����������ϱ�����ʵ����ΪusFreqNum */
} ANYBAND_W_TxEtDelayRsltRpt;

typedef struct {
    VOS_UINT16 etDelay;    /* Delayֵ */
    VOS_INT16  etDelayAclr; /* Delayֵ��Ӧ��ACLR�������λ0.125dBc */
} NV_CDMA_TxEtDelayData;

typedef struct {
    VOS_UINT32                    lsRptCnt; /* �ϱ��ĸ�������Ϊ1��ʾ����Ҫ���������������ʾPHY�ϱ�ֵ��ΪУ׼��� */
    VOS_UINT32                    lsFreq;          /* ����Ƶ�ʣ�WΪ100KHz,CΪKHz */
    NV_CDMA_TxEtDelayData         delayData[1]; /* PHY����delay�Ľ������ʵ����ΪlsRptCnt */
} NV_CDMA_TxEtDelayRpt;

typedef struct {
    VOS_UINT16                   bandId;
    VOS_UINT16                   freqNum;         /* У׼��Ƶ����� */
    /* ��������׵�ʱ�����ϱ�ֵ����õ�(ACLR���)���� usSearchOffset��Χ������usSearchStep���� */
    VOS_UINT16                   searchOffset;
    VOS_UINT16                   searchStep;      /* �������� */
    NV_CDMA_TxEtDelayRpt         etDelayData[1]; /* У׼����������ϱ�����ʵ����ΪusFreqNum */
} ANYBAND_CDMA_TxEtDelayRsltRpt;

/*
 * �ṹ˵��: ��ʾRFIC��DBB��˥��������£����е�UL4�����������ֵ(Hi6363ʹ��)
 */
typedef struct {
    VOS_INT16 paGain[4]; /* ��VOICEģʽ�£���ʾRFIC��DBB��˥��������£����е�3�����������ֵ����λΪ0.1dB */
} UCOM_NV_WTxPaGainRpt;

typedef struct {
    VOS_UINT16                    bandId;
    VOS_UINT16                    reserved; /* ���� */
    UCOM_NV_WTxPaGainRpt          etPaGain; /* У׼����ϱ� */
} ANYBAND_W_TxEtPaGainRsltRpt;

/*
 * Hi6363 W/CģTRX����У׼����ԭ��
 */
typedef struct {
    /*
     * ���ʿ���ģʽ����unTxPowerParaʹ��ͬ�������롣
     * 0������ģʽ�� ��ʱunTxPowerPara.usTxPower10th��Ч��
     * 1������˥��ģʽ����ʱunTxPowerPara.usTxPowerAtten10th��Ч��
     * 2���Ĵ������Ʒ�ʽ����ʱunTxPowerPara.stTxRegCtrlPara��Ч��
     */
    VOS_UINT16 txPowerCtrlMode : 8;
    VOS_UINT16 txModMode : 8; /* 0����������źţ�1�����䵥���źţ� */
} W_TX_PowerCtrlMode;

typedef union {
    VOS_INT16             txEtDac;    /* ET PA VCC (mv)ֵ */
    W_TX_PaParaCfg        paParaCfg; /* APT PAVCCֵ */
} PHY_RF_CAL_W_TX_PA_VCC_UNION;

typedef struct {
    VOS_UINT16 rficMode : 4; /* RFIC�Ĺ���ģʽ��0:����ģʽ��1:Voiceģʽ */ /* W_TX_RficMode_UINT8 */
    VOS_UINT16 txPaMode : 4; /* 0:�Զ�����;1:������;2:������;3:������ */
    VOS_UINT16 mrxGain : 4;  /* MRX��λ */
    VOS_UINT16 dpdUsed : 4;  /* APT���ʲ���У׼��0:max PaVccʱ��DPD����Ϊbypass��1:Pavcc���ֱ仯ʱʹ��DPDģ�� */
} W_TX_RficPaModeMrx;

/*
 * �ṹ˵��: WCDMA RF����У׼����Step�Ĳ���
 */
typedef struct {
    W_TX_RficPaModeMrx                      txRficPaModeMrx;
    PHY_RF_CAL_W_TX_PA_VCC_UNION            etAptParaCfg; /* ET or APT���� */
    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION txPowerPara;
} EVERY_TX_MrxStepPara;

/*
 * �ṹ˵��: WCDMA RF����У׼����Step�Ĳ���
 */
typedef struct {
    /*
     * BIt[15] : 0: No Block,   1:Block;
     * BIT[14] : 0:���ز���     1:˫�ز�
     * BIT[13] : 0:��λ�Զ����� 1:��λǿ��
     * BIT[2:0]: 0:��һ��(������ߵ�)����������
     */
    W_RX_AgcGainCfg rxAgcGain;
    VOS_UINT16      rsv;
} EVERY_RX_StepPara;

typedef struct {
    VOS_UINT32 len;               /* Chan�����ֽڳ��ȣ���������ulLen������ֽڳ��� */
    VOS_UINT16 band;              /* Ƶ�� */
    VOS_INT16  etPowerCompression; /* ET ��ѹ��������λ0.1dB */
    VOS_UINT32 txArfcn;           /* WΪ100KHz��λ��CΪKHz��λ */
    /*
     * �ܵ�STEP������������Ҫ����ǰ�˵ĸ��������ø�������ʾaulTxStepPattern��Чbit������
     * ��������ӦbitΪ0����ʾ��2ms����Ҫ����
     */
    VOS_UINT16 txStepPatternCnt;
    VOS_UINT16 stepCnt;           /* ��ʾ��Ҫ����ǰ�˵ĸ�����astStepData�ĳ��ȸ��� */

    VOS_UINT8  data[4];
    /*
     * aucData[]��������ʽ���£�
     * VOS_UINT32 aulTxStepPattern[ceil(usTxStepPatternCnt/32)]; ���鳤�� = usTxStepPatternCnt/32����ȡ��
     * EVERY_TX_MrxStepPara astStepData[n]; n = usStepCnt��
     * Ҳ����aulRxStepPattern��bitλΪTRUE���ܺ�, FALSE������㲻����ǰ��
     */
} W_TX_MrxFastChann;

typedef W_TX_MrxFastChann C_TX_MRX_FAST_CHANN_STRU;

typedef struct {
    VOS_UINT32 len;              /* Chan�����ֽڳ��ȣ���������ulLen������ֽڳ��� */
    VOS_UINT16 band;             /* Ƶ�� */
    VOS_UINT16 rsv;              /* ���� */
    VOS_UINT32 rxArfcn;          /* WΪ100KHz��λ��CΪKHz��λ */
    /*
     * �ܵ�STEP������������Ҫ����ǰ�˵ĸ��������ø�������ʾaulRxStepPattern��Чbit������
     * ��������ӦbitΪ0����ʾ��2ms����Ҫ����
     */
    VOS_UINT16 rxStepPatternCnt;
    VOS_UINT16 stepCnt;          /* ��ʾ��Ҫ����ǰ�˵ĸ�����astStepData�ĳ��ȸ��� */
    VOS_UINT8  data[4];
    /*
     * aucData[]��������ʽ���£�
     * VOS_UINT32 aulRxStepPattern[ceil(usRxStepPatternCnt/32)]; ���鳤�� = usRxStepPatternCnt/32����ȡ��
     * EVERY_RX_StepPara     astStepData[n]; n = usStepCnt��
     * Ҳ����aulRxStepPattern��bitλΪTRUE���ܺ�, FALSE������㲻����ǰ��
     */
} W_RX_FastChann;

typedef W_RX_FastChann C_RX_FAST_CHANN_STRU;

typedef struct {
    /* BIT[1]: APT�Ƿ���Ч BIT[0]: MRX����Ƿ�ʹ��*/ /* Reference to MASK_W_TRX_FAST_CAL_REQ_TXMASK_* */
    VOS_UINT16                         txMask;
    W_TX_PowerCtrlMode                 txPowerCtrlMode;


    HI6363_FAST_StepWidthUint16        txStepWidthUs;  /* ��λus */
    HI6363_FAST_StepWidthUint16        rxStepWidthUs; /* ��λus */

    VOS_UINT16 txChannCnt;
    VOS_UINT16 rxChannCnt;

    VOS_UINT8  data[W_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE]; /* ��������֧��16k����Ϣ����������ʹ��16000���ֽ� */
    /* �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬��Ϣ��ʵ�ʳ���Ϊ��Ч֡�� */
    /*
     * ʹ��ʱ��������˳�������ݣ���ͬʱ��TRX����Ҫȷ��ÿ��Ƶ���TRXʱ������
     * W_TX_MrxFastChann        astTxMrxChan[usTxChannCnt];
     * W_RX_FastChann            astRxMrxChan[usRxChannCnt];
     */
} W_TRX_MrxFastCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    W_TRX_MrxFastCalReqData     data;
} PHY_RF_CalWTrxMrxFastCalReqHi6363;

/*
 * �ṹ˵��: ET ����У׼���
 */
typedef struct {
    VOS_UINT32 paVccMv; /* �������ʵ��PA Vcc �ĵ�ѹ(mv) */
    VOS_UINT32 mrx;     /* MRXֵ */
} W_TX_EtLutFastCalRslt;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;
    VOS_UINT16                  rssiCnt; /* RSSI����ֵ�ĸ��� */
    VOS_UINT16                  mrxGainCorrCnt;

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    /* Rssi At1(0.125dBm) + Rssi At2(0.125dBm) + MRX���ֵ(4�ֽ�)) */
    VOS_INT16 asReportData[W_TRX_MAX_REPORT_COUNT_HI6363];

    VOS_INT32 errorCode; /* ����������������Ǵ�����Ϣ��װ����ʹ�� */
} PHY_RF_CalWTrxFastCalIndHi6363;

/*
 * �ṹ˵��: WCDMA RF����У׼����Step�Ĳ���
 */
typedef struct {
    /*
     * BIt[15] : 0: No Block,   1:Block;
     * BIT[14] : 0:���ز���     1:˫�ز�
     * BIT[13] : 0:��λ�Զ����� 1:��λǿ��
     * BIT[2:0]: 0:��һ��(������ߵ�)����������
     */
    W_RX_AgcGainCfg                  rxAgcGain;
    W_TX_RficPaMode                  txRficPaMode;
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;
    W_TX_PaParaCfg                   paParaCfg;
    W_TX_PdParaCfg                   txPdPara; /* PD VGA���� PD DC�Ĳ��� */
} ANY_STEP_Para;

/*
 * �ṹ˵��: WCDMA RF����У׼�ӿ�
 */
typedef struct {
    VOS_UINT16         band;
    VOS_UINT16         stepCnt; /* [1, 16] */
    VOS_UINT16         txArfcn;
    VOS_UINT16         rxArfcn;
    VOS_UINT16         txStepPattern;
    VOS_UINT16         rxStepPattern;
    ANY_STEP_Para      stepData[W_TRX_MAX_STEP_COUNT];
} W_TRX_FastFrame;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    /*
     * BIT[3]: PD DCOFFSET�Ƿ���Ч
     * BIT[2]: PD VGA�Ƿ���Ч
     * BIT[1]: APT�Ƿ���Ч
     * BIT[0]: PD����Ƿ�ʹ��*/ /* Reference to MASK_W_TRX_FAST_CAL_REQ_TXMASK_*
    */
    VOS_UINT16                      txMask;
    /*
     * ���ʿ���ģʽ����unTxPowerParaʹ��ͬ�������롣
     * 0������ģʽ�� ��ʱunTxPowerPara.usTxPower10th��Ч��
     * 1������˥��ģʽ����ʱunTxPowerPara.usTxPowerAtten10th��Ч��
     * 2���Ĵ������Ʒ�ʽ����ʱunTxPowerPara.stTxRegCtrlPara��Ч��
     */
    W_TX_PowerCtrlModeUint16        txPowerCtrlMode;

    W_FAST_StepWidthUint16          stepWidthMs; /* Reference to W_FAST_StepWidth */
    VOS_UINT16                      rsv2;

    VOS_UINT16 txFrameCnt; /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */
    VOS_UINT16 rxFrameCnt; /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    W_TRX_FastFrame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalWTrxFastCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;     /* ԭ������ */
    VOS_UINT16                  component; /* �������� */
    VOS_UINT32                  mask;      /* ����Band */

    VOS_UINT16 rxBand;    /* ����Band */
    VOS_UINT16 rsv;       /* ���� */
    VOS_UINT32 rxChannel; /* ����Ƶ�� */

    /*
     * BIT[15] : 0:No Block�� 1:Block;
     * BIt[14] : 0:���ز���   1��˫�ز�
     * BIT[13] : 0:��λ�Զ����� 1:��λǿ��
     * BIT[2:0]: 0:��һ��(������ߵ�),��������
     */
    W_RX_AgcGainCfg rxAgcGain;
    VOS_UINT16 res;

    VOS_UINT16 c2CalEnable;  /* C2�Ƿ�У׼ */
    VOS_UINT16 c2Fix;        /* C2ֵ */
    VOS_UINT16 c2LowerLimit; /* C2У׼���� */
    VOS_UINT16 c2UpperLimit; /* C2У׼���� */

    VOS_UINT16 c1CalEnable; /* C1�Ƿ�У׼ */
    VOS_UINT16 c1Fix;       /* C1ֵ */

    W_TX_RficPaMode           txRficPaMode;
    /*
     * ���ʿ���ģʽ����unTxPowerParaʹ��ͬ�������롣
     * 0������ģʽ����ʱunTxPowerPara.usTxPower10th��Ч��
     * 1������˥��ģʽ����ʱunTxPowerPara.usTxPowerAtten10th��Ч��
     * 2���Ĵ������Ʒ�ʽ����ʱunTxPowerPara.stTxRegCtrlPara��Ч��
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;     /* ���ʿ��Ʋ�������ucTxPowerCtrlModeʹ��ͬ�������� */

    VOS_UINT16 resData[32];
} PHY_RF_CalWDcxoSelfCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;     /* ԭ������ */
    VOS_UINT16                  component; /* �������� */
    VOS_UINT32                  mask;      /* ����Band */

    VOS_UINT16 rxBand;    /* ����Band */
    VOS_UINT16 rsv;       /* ���� */
    VOS_UINT32 rxChannel; /* ����Ƶ�� */

    /*
     * BIT[15] : 0:No Block�� 1:Block;
     * BIt[14] : 0:���ز���   1��˫�ز�
     * BIT[13] : 0:��λ�Զ����� 1:��λǿ��
     * BIT[2:0]: 0:��һ��(������ߵ�),��������
     */
    W_RX_AgcGainCfg rxAgcGain;
    VOS_UINT16 a0A1CalEnable;         /* 0:��У׼ 1:У׼ */

    VOS_UINT16 c2CalEnable;  /* C2�Ƿ�У׼ */
    VOS_UINT16 c2Fix;        /* C2ֵ */
    VOS_UINT16 c2LowerLimit; /* C2У׼���� */
    VOS_UINT16 c2UpperLimit; /* C2У׼���� */

    VOS_UINT16 c1CalEnable; /* C1�Ƿ�У׼ */
    VOS_UINT16 c1Fix;       /* C1ֵ */

    W_TX_RficPaMode           txRficPaMode;
    /*
     * ���ʿ���ģʽ����unTxPowerParaʹ��ͬ�������롣
     * 0������ģʽ����ʱunTxPowerPara.usTxPower10th��Ч��
     * 1������˥��ģʽ����ʱunTxPowerPara.usTxPowerAtten10th��Ч��
     * 2���Ĵ������Ʒ�ʽ����ʱunTxPowerPara.stTxRegCtrlPara��Ч��
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;
    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION txPowerPara; /* ���ʿ��Ʋ�������ucTxPowerCtrlModeʹ��ͬ�������� */

    VOS_UINT16 ausRes[32];
} PHY_RF_CalWDcxoSelfCalReqHi6363;

typedef struct {
    VOS_UINT16 expOut;
    VOS_UINT16 rsv;
    VOS_UINT32 a1SigOut;
} COFFICIENT_DATA;

typedef struct {
    VOS_INT16 iData;
    VOS_INT16 qData;
} RAW_IQ_Data;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  componentId; /* ��� ID */
    VOS_UINT16                  num;         /* �ϱ��ĵڼ�������,��1��ʼ */
    VOS_UINT16                  evm;
    VOS_INT32                   freq;
    VOS_INT32                   initialPhase;
    VOS_UINT32                  reportDataCount;
    RAW_IQ_Data                 iqData[W_RX_EVM_REPORT_DATA_COUNT];
} PHY_RF_CalWRfRxEvmInd;

/*
 * �ṹ˵��: WCDMA ET����У׼����Step�Ĳ���
 */
typedef struct {
    VOS_UINT16                       rsv;
    W_TX_RficPaMode                  txRficPaMode;      /* RF��PA��λ���� */
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;       /* ���ʲ�������������ģʽ */
    PHY_RF_CAL_W_TX_PA_VCC_UNION     paParaCfg;         /* PA��ѹ���Ʋ��� */
    VOS_INT16                        etPowerCompression; /* ETУ׼ʱ������ѹ���㣬��λ0.1dB */
} ET_STEP_Para;

/*
 * �ṹ˵��: WCDMA ET���ٵ�֡�ṹ
 */
typedef struct {
    VOS_UINT16        band;
    VOS_UINT16        stepCnt; /* [1, 16] */
    VOS_UINT16        txArfcn;
    VOS_UINT16        rsv;
    VOS_UINT16        txStepPattern;
    VOS_UINT16        rsv2;
    ET_STEP_Para      stepData[W_TRX_MAX_STEP_COUNT];
} ET_FAST_Frame;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    /*
     * BIT[1]������ѹ�����Ƿ���Ч���������ѹ������Ч����ÿ����ѹ��DSP��Ҫ����Viq��
     * ��ɺ��ҳ�ѹ�����Ӧ��Viq�����ϱ�ѹ�����Ӧ��MRXֵ
     * BIT[0]: PD����Ƿ�ʹ�ܣ����PD���ʹ�ܣ����ҵ�ѹ�������Ҫ�ط�һ�Σ���ȡ��Ӧ��PDֵ���ϱ���
     */
    /* Reference to PHY_RF_CalWEtFastReq_* */
    VOS_UINT16 mask;
    /*
     * ���ʿ���ģʽ����unTxPowerParaʹ��ͬ�������롣
     * 0������ģʽ�� ��ʱunTxPowerPara.usTxPower10th��Ч��
     * 1������˥��ģʽ����ʱunTxPowerPara.usTxPowerAtten10th��Ч��
     * 2���Ĵ������Ʒ�ʽ����ʱunTxPowerPara.stTxRegCtrlPara��Ч��
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;

    VOS_UINT16 rsv1;         /* Ԥ�� */
    VOS_UINT16 calAlgorithm; /* 0 - ��ѹ��ģʽ��1 - ������ģʽ */
    VOS_UINT16 txFrameCnt;   /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */
    VOS_UINT16 rsv2;         /* Ԥ�� */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    ET_FAST_Frame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalWEtFastReq;

typedef PHY_RF_CalWSelfCalReq PHY_RF_CAL_W_DPD_LUT_SELF_CAL_REQ_STRU;

/*
 * �ṹ˵��:DCXO��������Ƶƫ�¶ȶ�Ӧֵ
 */
typedef struct {
    VOS_INT32 temp;    /* �¶�,Boston�¶ȵ�λ0.001�� */
    VOS_INT32 freqPpm; /* �����¶��¶�Ӧ�����Ƶƫ��Boston���㻯ϵ��2^31/10^6 */
} PHY_RF_DcxoSample;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  componentId;
    VOS_UINT32                  errorCode;
    VOS_UINT16                  c2Fix;
    VOS_UINT16                  c1Fix;
#if (FEATURE_ON == FEATURE_HI6363)
    VOS_INT32               initTemp6363; /* Boston�¶ȵ�λ0.001�� */
    VOS_INT32               initFreq6363;
    COFFICIENT_DATA    a0;
    COFFICIENT_DATA    a1;
    COFFICIENT_DATA    a2;
    COFFICIENT_DATA    a3;
    VOS_INT32               dcxoInitTemp6363;                     /* ���߼����ʼ�¶�,Boston�¶ȵ�λ0.001�� */
    VOS_INT32               beforeTempUp6363;                     /* a0��a1У׼ǰ�¶�,Boston�¶ȵ�λ0.001�� */
    VOS_INT32               afterTempUp6363;                      /* a0��a1У׼���¶�,Boston�¶ȵ�λ0.001�� */
    VOS_UINT16              rsv0;                             /* ���ֽڶ��뱣�� */
    VOS_UINT16              sampleNum;                        /* �������� */
    PHY_RF_DcxoSample       dcxoSample[DCXO_SAMPLE_MAX_NUM]; /* ���� */
#else
    VOS_INT16            initTemp; /* Chicago���ϰ汾�¶ȵ�λ0.1�� */
    VOS_INT16            initFreq;
    COFFICIENT_DATA a0;
    COFFICIENT_DATA a1;
    COFFICIENT_DATA a2;
    COFFICIENT_DATA a3;
    VOS_INT16            tmax;       /* XO������¶�,Chicago���ϰ汾�¶ȵ�λ0.1�� */
    VOS_INT16            tmin;       /* XO��С�����¶�,Chicago���ϰ汾�¶ȵ�λ0.1�� */
    VOS_INT16            dcxoInitTemp; /* ���߼����ʼ�¶�,Chicago���ϰ汾�¶ȵ�λ0.1�� */
    VOS_INT16            beforeTempUp; /* a0��a1У׼ǰ�¶�,Chicago���ϰ汾�¶ȵ�λ0.1�� */
    VOS_INT16            afterTempUp;  /* a0��a1У׼���¶�,Chicago���ϰ汾�¶ȵ�λ0.1�� */
    VOS_UINT16           rsv[15];
#endif
} PHY_RF_CalWRfDcxoSelfcalInd;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ��� ID */
    VOS_UINT16                  rssiCnt;                 /* RSSI����ֵ�ĸ��� */
    VOS_UINT16                  powerDetectorCnt;

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    VOS_INT16 asReportData[W_TRX_MAX_REPORT_COUNT]; /* Rssi At1(0.125dBm)+Rssi At2(0.125dBm)+PD������� */
} PHY_RF_CalWTrxFastCalInd;

typedef struct {
    VOS_UINT16 rficGainCtrl; /* RF Gain�Ŀ��ƼĴ���ֵ����RF Gain��BB Gain��϶��� */
    VOS_INT16  dbbAtten10th;  /* Dbb��˥��ֵ��ȡֵ��Χ[-70,+10] */
} PHY_RF_CalGTxRegCtrl;

typedef struct {
    VOS_UINT32 rficGainCtrl : 24; /* RF Gain�Ŀ��ƼĴ���ֵ����RF Gain��BB Gain��϶��� */
    VOS_INT32  dbbAtten10th : 8;  /* Dbb��˥��ֵ��ȡֵ��Χ[-70,+10] */
} PHY_RF_CalGTxRegCtrl6363;

typedef union {
    VOS_UINT16                    txAgc;           /* ��ӦTx Vpa����ģʽ */
    VOS_INT16                     txPower10th;      /* ��Ӧ����ģʽ */
    VOS_INT16                     txPowerAtten10th; /* ����˥��ģʽ */
    PHY_RF_CalGTxRegCtrl          txRegCtrlPara;   /* �Ĵ�������ģʽ */
#if FEATURE_HI6363
    PHY_RF_CalGTxRegCtrl6363      st6363TxRegCtrlPara; /* 6363�Ĵ�������ģʽ */
#endif
} PHY_RF_CAL_G_TX_POWER_PARA_UNION;

/*
 * �ṹ˵��: G TX GSM RF����У׼�ӿ�
 */
typedef struct {
    VOS_UINT16 txModType : 2;       /* ������Ʒ�ʽ:0��ʾGMSK����;1��ʾ8PSK����  G_TX_ModulationType_UINT8 */
    VOS_UINT16 txIsCwTone : 6;      /* 0 - �����źţ� 1- �����ź� */
    /*
     * ������Ʒ�ʽ��G_TX_PowerCtrlModeUint16
     * 0��GMSK��ѹ����,�˷�ʽ��usTxVpaҪ���ã�
     * 1�����ʿ���,�˷�ʽ��usTxPower10thҪ���ã�
     * 2������˥��ģʽ���˷�ʽ��sTxPowerAtten10thҪ���ã�
     * 3���Ĵ�������ģʽ���˷�ʽ��stTxRegCtrlParaҪ����
     */
    VOS_UINT16 txPowerCtrlMode : 8;
} G_TX_ModeTypePowerCtrlCfg;

typedef struct {
    VOS_UINT16                         txMode; /* 0�� burst,1:Continue */
    G_TX_ModeTypePowerCtrlCfg          txModTypePowerCtrl;
    PHY_RF_CAL_G_TX_POWER_PARA_UNION   txPowerPara;

    VOS_UINT16 paVcc;        /* EDGE PA Vcc���Ƶ�ѹ��Ŀǰ��֧�� */
    VOS_UINT16 paVramp;      /* EDGE PA Vramp���Ƶ�ѹ���ڵ��Ʒ�ʽΪ8PSK���ǹ��ʿ���ģʽ����Ч */
    VOS_UINT16 paGainIndex;  /* linea pa��������( 0:auto,1:ultra high,2:high,3:mid,4:low ) */
    VOS_UINT16 mrxGainIndex; /* Mrx��λ���� */
} G_RF_SingleSlotCfg;

/*
 * �ṹ˵��: �����Ҫ�������źţ�HiMS���߽������usDataPattern��usSlotCntֻ��ѡ��1��8
 *           astSlotCfg[0]��ucTxIsCwTone����Ϊ1
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    VOS_UINT32 mask; /* Reference to MASK_CAL_RF_G_TX_* */

    /* Afc Para Cfg */
    PHY_RF_CalAfcPara           afcPara; /* AFC������ */

    VOS_UINT16 freqNum;     /* (Band << 12) | Arfcn */
    VOS_UINT16 txEnable;    /* ����ʹ�ܿ���:0x5555-ʹ�ܷ���;0xAAAA-ֹͣ����;TSC 0; */
    VOS_UINT16 dataPattern; /* 0��All 0��1��All 1��2��Random */
    /*
     * ����ʱ϶������
     * 1~4 - ����ָ������ʹ����Щʱ϶��DSP�Զ�����1~4ʱ϶����
     * 8 - 8ʱ϶ȫ����ʹ�õ�һ��astSlotCfg�Ĳ���������
     */
    VOS_UINT16 slotCnt;
    G_RF_SingleSlotCfg          slotCfg[G_TX_PARA_MAX_SLOT_CNT_PER_FRAME];   /* ÿ��ʱ϶��������� */
    /* MASK_CAL_RF_G_GET_CLT_HVDAC �����־��Ч��ʱ������ */
    PHY_CAL_MipidevUnit         hvDacValue[PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM];
} PHY_RF_CalGTxParaCfgReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;
} PHY_RF_CalGMrxCorrReq;

typedef struct {
    VOS_UINT16 cnt; /* ����PHY_RF_CalGTxParaCfgReq_usSlotCnt�����ϱ��ĸ��� */
    VOS_UINT16 rsv;
    VOS_UINT32 mrxCorr[8];
} PHY_RF_CalGMrxCorrIndData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    PHY_RF_CalGMrxCorrIndData   data;
} PHY_RF_CalGMrxCorrInd;

/*
 * �ṹ˵��: ����ϵ��
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;
    VOS_UINT32                  errorCode;
    VOS_INT16                   reflectReal; /* ����ϵ��ʵ�� */
    VOS_INT16                   reflectImag; /* ����ϵ���鲿 */
} PHY_RF_CalCltReflectanceInd;

/*
 * �ṹ˵��: path delay
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  componentId;
    VOS_UINT32                  errorCode;
    VOS_UINT16                  fwdPathDelay; /* ǰ�� path delay */
    VOS_UINT16                  revPathDelay; /* ���� path delay */
} PHY_RF_CalCltPathDelayInd;

/*
 * �ṹ˵�� : GSM RF��У׼�ӿ�����Gģ��У׼��Req�ӿ���Wģ��ͬ
 */
typedef PHY_RF_CalWSelfCalReq PHY_RF_CAL_G_SELF_CAL_REQ_STRU;

typedef PHY_RF_CalWSelfCalInd PHY_RF_CAL_G_SELF_CAL_IND_STRU;

/*
 * �ṹ˵��: G Rx Rssi��������
 */
typedef struct {
    VOS_UINT16 mask;        /* Reference to MASK_CAL_RF_G_RX_* */
    VOS_UINT16 freqNum;     /* (Band << 12) | Arfcn */
    VOS_UINT16 rxMode;      /* 0:burst����; 1:��������; */
    VOS_UINT16 aGCMode;     /* Fast AGC,Slow AGC */
    VOS_UINT16 aGCGain;     /* AGC��λ��0-��һ������ */
    VOS_UINT16 blockerFlag; /* 0 - no blocker, 1- blocker */
    VOS_UINT16 rxAntSel;    /* 1:��������;2:�ּ�����;3:���ּ�˫���� */
    VOS_UINT16 eos;         /* End of slot��1��ʾ���滹��1��slot��0��ʾ���һ��slot */
} PHY_RF_CalGRxParaCfgReqData;

/*
 * �ṹ˵��: G Rx Rssi�����ṹ��ͷ������У׼REQ��Ϣ�޸�Ϊ��̬��Ϣ���ȣ�֧��2��ʱ϶�Ķ�̬����
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv1;

    PHY_RF_CalGRxParaCfgReqData data[G_RX_MAX_CFG_REQ_DATA_COUNT];
} PHY_RF_CalGRxParaCfgReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv1;
    VOS_UINT16                  measNum;
    VOS_UINT16                  rsv2;
} PHY_RF_CalGRxRssiMeasReq;

/*
 * �ṹ˵��:G Rx Rssi����������У׼Rssi�ϱ���Ϣ�޸�Ϊ��̬��Ϣ���ȣ���slot�ϱ�
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ��� ID */

    /*
     * ������ϱ������ݣ���slot�ϱ�����У׼��������ȿɱ�
     * VOS_INT16  sRSSIMain;  slot1����Rssiֵ
     * VOS_INT16  sRSSIDiv;   slot1�ּ�Rssiֵ
     * VOS_INT16  sRSSIMain;  slot2����Rssiֵ
     * VOS_INT16  sRSSIDiv;   slot2�ּ�Rssiֵ
     * .......... ��������
     */
} PHY_RF_CalGRxRssiMeasInd;

/*
 * �ṹ˵��:�ϱ����ּ�����ֵ
 */
typedef struct {
    VOS_INT16 rSSIMain; /* ����Rssiֵ */
    VOS_INT16 rSSIDiv;  /* �ּ�Rssiֵ */
} GPHY_RF_CalSlotRpt;

#if FEATURE_HI6363
/*
 * �ṹ˵��: GSM RF����У׼֡�ṹ
 */
typedef struct {
    PHY_RF_CAL_G_TX_POWER_PARA_UNION txPowerPara;
    VOS_UINT16                       paVcc;
    VOS_UINT16                       paVramp;     /* EDGE Pa���Ƶ�ѹ */
    VOS_UINT16                       paGainIndex; /* linea pa��������( 0:auto,1:ultra high,2:high,3:mid,4:low ) */
    VOS_UINT16                       rsv;
} G_RF_TxStep;
#else
/*
 * �ṹ˵��: GSM RF����У׼֡�ṹ
 */
typedef struct {
    PHY_RF_CAL_G_TX_POWER_PARA_UNION txPowerPara;
    VOS_UINT16 paVcc;
    VOS_UINT16 paVramp;     /* EDGE Pa���Ƶ�ѹ */
    VOS_UINT16 paGainIndex; /* linea pa��������( 0:auto,1:ultra high,2:high,3:mid,4:low ) */
} G_RF_TxStep;
#endif

/*
 * �ṹ˵��: GSM RF����У׼�ӿ�
 */
typedef struct {
    VOS_UINT16  channel;
    VOS_UINT16  stepPattern; /* ��Bitλ��ʾ��ǰʱ϶�Ƿ��� */

    G_RF_TxStep stepValue[G_TX_MAX_STEP_COUNT]; /* ��ǰ���ֻ֧����ʱ϶ */
} G_RF_TxSequence;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    VOS_UINT16 txMask; /* Bit[0]:APT�Ƿ���Ч ��MASKλMASK_G_TX_FAST_CAL_REQ_TXMASK_APT��ʾ */

    G_TX_ModeTypePowerCtrlCfg   txModTypePowerCtrl;

    VOS_UINT16 frameCnt;
    VOS_UINT16 rsv2; /* ������չԤ�� */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    G_RF_TxSequence txSequence[G_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalGTxFastCalReq;


typedef struct {
    VOS_UINT16 channel;

    // ����Patternȷ��������slot��bit0��ʾslot0������bit1��ʾslot1�������������ƣ����֧��8slot
    VOS_UINT16 stepPattern;
} PHY_RF_CalGeTmrxFastCalFrame;

/*
 * �ṹ˵��: Step�����仯��Ƶ����Ԥ��һ�������ֶΣ�Ĭ����0
 */
typedef struct {
    G_RF_TxStep       txCfg;
    VOS_UINT16        mrxGain;
    VOS_UINT16        rsv[3];
} PHY_RF_CalGeTmrxFastCalStep;

/*
 * �ṹ˵��:����<8168
 */
typedef struct {
    /*
     * Bit[0]:APT�Ƿ���Ч
     * Bit[1]:Mrx�Ƿ���Ч
     * Bit[2]:
     * ��MASKλMASK_G_TX_FAST_CAL_REQ_TXMASK_APT��ʾ
     */
    VOS_UINT16 txMask;

    G_TX_ModeTypePowerCtrlCfg txModTypePowerCtrl;

    VOS_UINT16 stepWidth8th; /* ÿ��Stepռ��ʱ��, ȡֵ��Χ[1,8], ��λ0.125slot */
    VOS_UINT16 rsv;

    VOS_UINT16 frameCnt;
    VOS_UINT16 stepCnt;

    VOS_UINT16 data[5100];
    /*
     * ��������ʽ����:
     * // Ƶ�����������
     * typedef struct
     * {
     *     VOS_UINT16                          usChannel;
     *     // ����Patternȷ��������slot��bit0��ʾslot0������bit1��ʾslot1�������������ƣ����֧��8slot
     *     VOS_UINT16                              usStepPattern;
     * }[m]; // m < 400, ���Mrx�ϱ��򿪣���m < 250, ��Ϊ�ϱ�����ǰ���8ʱ϶�ϱ�8�����ʵ�ֵģ�
     * ��PC���жϵ�ǰ�����Ӧ��Pattern��True����False����ȡ�ᡣ
     * // step ���ز���
     * typedef struct
     * {
     *     <***************************************************>
     *     PHY_RF_CAL_G_TX_POWER_PARA_UNION    unTxPowerPara;
     *     VOS_UINT16                              usPaVcc;
     *     VOS_UINT16                              usPaVramp;
     *     VOS_UINT16                              usPaGainIndex;
     *     VOS_UINT16                              usRsv1;
     *     <***************************************************>
     *     VOS_UINT16                              usMrxGain;
     *     VOS_UINT16                              usRsv2[3];
     * }[n]; // n < 400
     */
} PHY_RF_CalGeTmrxFastCalReqData;

/*
 * �ṹ˵��:��Ϣ�ܳ���<8192����ȥvosͷ + usMsgID + usRsv��stData�ĳ���<8168
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    PHY_RF_CalGeTmrxFastCalReqData data;
} PHY_RF_CalGeTmrxFastCalReq;

typedef struct {
    // ÿ֡8��ʱ϶�ϱ������Ʃ�����stepwidthΪ8����ʾ1step = 1slot����ÿ֡�ϱ�8�����������ϱ�250֡
    // ���stepwidth = 1�����ʾ1step = 0.125slot����ÿ֡�ϱ�64�����������ϱ�32֡
    // 2000Ϊ��󳤶ȣ�ʵ���ϱ�Ϊ�䳤
    VOS_UINT16 stepCnt;
    VOS_UINT32 mrxCorr[2000];
} PHY_RF_CalGeTmrxFastCalIndData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    PHY_RF_CalGeTmrxFastCalIndData data;
} PHY_RF_CalGeTmrxFastCalInd;

/*
 * �ṹ˵�� : G TRX ������У׼ͨ��CNF
 */
typedef PHY_RF_CalWRfMsgCnf PHY_RF_CAL_G_RF_MSG_CNF_STRU;
typedef PHY_RF_CalWRfMsgCnf PHY_RF_CAL_WG_RF_MSG_CNF_STRU;

/*
 * �ṹ˵��: GSM RXÿ��slot�Ĳ������ã�Hi6363��ʼ֧�ְ�ʱ϶����
 *           ����H1��ʾǰ��ʱ϶��H2��ʾ���ʱ϶
 *           ���usStepWidthΪ2����ʾÿ��stepռ1��slot����ֻ��H1��Ч
 */
typedef struct {
    VOS_UINT16 h1SlotAgcGainIndex : 4;
    VOS_UINT16 h1SlotBlockerFlag : 1;
    VOS_UINT16 rsv : 3;
    VOS_UINT16 h2SlotAgcGainIndex : 4;
    VOS_UINT16 h2SlotBlockerFlag : 1;
    VOS_UINT16 rsv2 : 3;
} G_RF_RxSlot;

/*
 * �ṹ˵��: G Rx
 */
typedef struct {
    VOS_UINT16        channel;
    /*
     * bit[3:0]  AGC Gain��λ, Ŀǰȡֵ��Χ0~7
     * bit4      0��ʾnoblocker, 1��ʾblocker
     * bit[7:5]  rsv
     * bit[11:8]  AGC Gain��λ, Ŀǰȡֵ��Χ0~7, ��ʱ϶����ʱ��Ч
     * bit12      0��ʾnoblocker, 1��ʾblocker
     * bit[15:13] rsv
     */
    G_RF_RxSlot       agcGain[G_RX_MAX_STEP_COUNT];
} G_RF_RxSequence;

/*
 * �ṹ˵��: ���п���У׼����
 */
typedef struct {
    VOS_UINT16 stepWidth : 8;  /* step���, ȡֵ��Χ[1, 2], ��λ0.5slot */
    VOS_UINT16 testScheme : 2; /* 0x01 ������ģʽ��0x10 ���ּ�ģʽ��0x11 ���ּ�����ģʽ */
    VOS_UINT16 rsv : 6;
} PHY_RF_CalRxFastPara;

typedef struct {
    VOS_UINT16                   frameCnt;   /* ����֡�� */
    PHY_RF_CalRxFastPara         rxFastPara; /* ���п���У׼���� */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    G_RF_RxSequence              rxSequence[G_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalGRxFastCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    PHY_RF_CalGRxFastCalReqData data;
} PHY_RF_CalGRxFastCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;

    VOS_UINT16 rssiNum; /* �������������ΪG_RX_MAX_RSSI_CNT��,������ͬ��֡ */
    VOS_UINT16 rsv2;

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    /*
     * �����1ʱ϶������ÿ֡�ϱ�7���㣬���G_RX_MAX_RSSI_CNT������
     * ����ǰ�ʱ϶������ÿ֡�ϱ�14���㣬���2*G_RX_MAX_RSSI_CNT������
     */
    VOS_INT16 asRssiValue[G_RX_MAX_RSSI_COUNT];
} PHY_RF_CalGRxFastCalInd;

/*
 * �ṹ˵��: G Rx DCR
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv1;

    VOS_UINT16 bandMask; /* Bit0:GSM850; Bit1:GSM900; Bit2:DCS; Bit3:PCS; */
    VOS_UINT16 rsv2;
} PHY_RF_CalGRxDcrReq;

typedef struct {
    VOS_INT16 initDcocI;
    VOS_INT16 initDcocQ;
    VOS_INT16 dcI;
    VOS_INT16 dcQ;
    VOS_INT16 remainDcocI;
    VOS_INT16 remainDcocQ;
} G_RX_Dcr;

typedef struct {
    G_RX_Dcr gain0[16];
} NV_G_RxDcoffsetGain0;

typedef struct {
    NV_G_RxDcoffsetGain0   gain0Dc;
    G_RX_Dcr               otherGainDc[G_RX_AGC_GAIN_COUNT - 1];
    NV_G_RxDcoffsetGain0   crossBandGain0Dc;
    G_RX_Dcr               crossBandOtherGainDc[G_RX_AGC_GAIN_COUNT - 1];
} NV_G_RxDcoffset;

typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 blockerFlag; /* 0 - no blocker, 1 - blocker */

    NV_G_RxDcoffset nvRxDcr;
} ANY_BAND_GRxDcoffsetInd;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ���ID */

    VOS_UINT16 errCode; /* Error code */
    VOS_UINT16 errBand; /* ���У׼ʧ�ܵ�Ƶ�� */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv2;

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    ANY_BAND_GRxDcoffsetInd rst[10];
} PHY_RF_CalGRxDcrInd;

typedef struct {
    VOS_UINT16 dciCode;
    VOS_UINT16 dcqCode;
    VOS_INT16  remainDigitalDci;
    VOS_INT16  remainDigitalDcq;
} NV_RX_DcocCompValue;

typedef struct {
    NV_RX_DcocCompValue dcocComp[16];
} NV_G_RxDcocCalResult;

typedef struct {
    NV_G_RxDcocCalResult ant1CalResult;
    NV_G_RxDcocCalResult ant2CalResult;
} NV_GSM_RxPath0DcocCalResult;

typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;

    NV_GSM_RxPath0DcocCalResult nvRxDcr;
} ANYBAND_G_RxDcoffsetCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16 errCode; /* Error code */
    VOS_UINT16 errBand; /* ���У׼ʧ�ܵ�Ƶ�� */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv;

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    ANYBAND_G_RxDcoffsetCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataRxDcoffsetRpt;

typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;

    VOS_UINT16 rfSettingNum;      /* Rf Setting ���������3�� */
    VOS_UINT16 rfSettingIndex[3]; /* Rf Setting ����(��Ӧ10��Rf Settingֵ)��high gain��Setting������ǰ */
} ANYBAND_G_TxRfSettingCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16 errCode; /* Error code */
    VOS_UINT16 errBand; /* ���У׼ʧ�ܵ�Ƶ�� */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv;

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    ANYBAND_G_TxRfSettingCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataTxRfSettingRpt;

/*
 * �ṹ˵��: GSM��/�����±�NV�����
 */
typedef struct {
    VOS_UINT16 rampUp[G_RAMP_COEF_NUM];   /* ���±� */
    VOS_UINT16 rampDown[G_RAMP_COEF_NUM]; /* ���±� */
} NV_GSM_RampCoeff;

typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;

    NV_GSM_RampCoeff rampCoeff[G_RAMP_PWR_LEVEL_NUM]; /* 7������ϵ���� */
} ANYBAND_G_TxRampCoeffCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16 errCode; /* Error code */
    VOS_UINT16 errBand; /* ���У׼ʧ�ܵ�Ƶ�� */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv;

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    ANYBAND_G_TxRampCoeffCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataTxRampCoeffRpt;

/*
 * �ṹ˵��: IQ Mismatch���ּ����ߵ�У׼���
 */
typedef struct {
    VOS_INT16 amp;   /* ����ֵ */
    VOS_INT16 phase; /* ��λֵ */
} NV_G_RxIqMismatch;

/*
 * �ṹ˵��: IQ MismatchУ׼��������
 */
typedef struct {
    VOS_INT32 iSquare;    /* E(|I|^2) */
    VOS_INT32 qSquare;    /* E(|Q|^2) */
    VOS_INT32 iQMultiply; /* E(IQ) */
} RUN_DATA_GRxIqMismatch;

/*
 * �ṹ˵��: G Rx IqMismatchУ׼���
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* ��Ϣԭ�� */
    VOS_UINT16                  componentId; /* ��� ID */

    VOS_UINT16 errCode; /* ʧ���� */
    VOS_UINT16 rsv2;

    NV_G_RxIqMismatch       nvRxIqMismatch;                                     /* У׼��� */
    RUN_DATA_GRxIqMismatch  runDataRxIqMismatch[GPHY_RX_IQ_MISMATCH_SLOT_NUM]; /* �м����� */
} PHY_RF_CalGRxIqMismatchInd;

/*
 * �ṹ˵��: G Rx IqMismatchУ׼���
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* ��Ϣԭ�� */
    VOS_UINT16                  componentId; /* ��� ID */

    VOS_UINT16 errCode; /* ʧ���� */
    VOS_UINT16 rsv2;

    NV_G_RxIqMismatch       ant1CalResult;                              /* �������ߵ�У׼��� */
    NV_G_RxIqMismatch       ant2CalResult;                              /* �ּ����ߵ�У׼��� */
    RUN_DATA_GRxIqMismatch  ant1RunData[GPHY_RX_IQ_MISMATCH_SLOT_NUM]; /* �м����� */
    RUN_DATA_GRxIqMismatch  ant2RunData[GPHY_RX_IQ_MISMATCH_SLOT_NUM]; /* �м����� */
} PHY_RF_CalGRxIqmismatch6363Ind;

/*
 * �ṹ˵��: G Rx IqMismatchУ׼����
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* ��Ϣԭ�� */
    VOS_UINT16                  rsv1;

    VOS_UINT16 bandId;
    VOS_UINT16 rxChannel;

    VOS_INT16  refIor10th;       /* �ο��ź�ǿ�ȣ���λ0.1dBm */
    VOS_INT16  ampCorrThreshold; /* У׼ͨ������ */
    VOS_INT16  phsCorrTHreshold;
    VOS_UINT16 testScheme; /* 0x1 ������ģʽ��0x2 ���ּ�ģʽ��0x3 ���ּ�����ģʽ */
} PHY_RF_CalGRxIqMismatchReq;

/*
 * �ṹ˵��: G Tx HD3/HD5У׼����
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* ��Ϣԭ�� */
    VOS_UINT16                  rsv1;

    VOS_UINT16 bandMask; /* Bit0:GSM850; Bit1:GSM900; Bit2:DCS; Bit3:PCS; */
    VOS_UINT16 rsv2;
} PHY_RF_CalGTxHd3Hd5Req;

/*
 * �ṹ˵��: �������ʵȼ���HD3/HD5��У׼��������
 */
typedef struct {
    VOS_INT32 hD3Amp;
    VOS_INT32 asHD3Corr[GSM_TX_HD35_STEP_NUM];
    VOS_INT32 hD5Amp;
    VOS_INT32 asHD5Corr[GSM_TX_HD35_STEP_NUM];
} ANY_PCL_GTxHd3Hd5RunData;

/*
 * �ṹ˵��: ����Ƶ���HD3/HD5��У׼��������
 */
typedef struct {
    /* �ֱ���Max_PCL��Max_PCL+1��Max_PCL+2�������ʵȼ���HD3/HD5У׼�������� */
    ANY_PCL_GTxHd3Hd5RunData pclHd3Hd5RunData[GSM_TX_HD35_PCL_NUM];
} ANY_CHANN_GTxHd3Hd5RunData;

/*
 * �ṹ˵��: �������ʵȼ���HD3/HD5У׼���
 */
typedef struct {
    VOS_INT16 br; /* HD3У׼ֵʵ�� */
    VOS_INT16 bi; /* HD3У׼ֵ�鲿 */
    VOS_INT16 cr; /* HD5У׼ֵʵ�� */
    VOS_INT16 ci; /* HD5У׼ֵ�鲿 */
} ANY_PCL_GTxHd3Hd5;

/*
 * �ṹ˵��: ����Ƶ���HD3/HD5У׼���
 */
typedef struct {
    /* �ֱ���Max_PCL��Max_PCL+1��Max_PCL+2�������ʵȼ���HD3/HD5У׼��� */
    ANY_PCL_GTxHd3Hd5 pclHd3Hd5[GSM_TX_HD35_PCL_NUM];
} ANY_CHANN_GTxHd3Hd5;

/*
 * �ṹ˵��: ����Ƶ�ε�HD3/HD5У׼����͹�������
 */
typedef struct {
    VOS_UINT16                  band; /* Band��Ϣ */
    VOS_UINT16                  rsv;
    ANY_CHANN_GTxHd3Hd5         channHd3Hd5[GSM_TX_HD35_ARFCN_NUM]; /* �ֱ��ŵ͡��С�������Ƶ���У׼��� */
    /* �ֱ��ŵ͡��С�������Ƶ���У׼�������� */
    ANY_CHANN_GTxHd3Hd5RunData  channHd3Hd5RunData[GSM_TX_HD35_ARFCN_NUM];
} ANY_BAND_GTxHd3Hd5Ind;

/*
 * �ṹ˵��: G PHY�ϱ�HD3/HD5���
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* ��Ϣԭ�� */
    VOS_UINT16                  componentId; /* ��� ID */

    VOS_UINT16 errCode; /* ʧ���� ����PHY��λ���� */
    VOS_UINT16 errBand; /* У׼ʧ�ܵ�Ƶ�� ������У׼ʧ��Ĭ���ϱ���һ��ʧ�ܵ�band */

    VOS_UINT16 bandCnt; /* У׼Ƶ�εĸ��� */
    VOS_UINT16 rsv2;

    ANY_BAND_GTxHd3Hd5Ind bandHd3Hd5[GSM_TX_HD35_BAND_NUM]; /* ���֧��G 4��Ƶ�� */
} PHY_RF_CalGTxHd3Hd5Ind;

/*
 * �ṹ˵��: ����32K���������������ֹͣ
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;
    VOS_UINT32                  action; /* 1-����32K����;0-ֹͣ32K���� */
} WPHY_TOOL_GaugeReq;

/*
 * �ṹ˵��: ����32K���������������ֹͣ��ԭ��ظ�
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;
    VOS_UINT32                  action; /* 1-����32K����;0-ֹͣ32K���� */
} WPHY_TOOL_GaugeCnf;

/*
 * �ṹ˵��: ����32K����������ϱ�
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId; /* ��� ID */
    VOS_UINT32                  data[W_GAUGE_RESULT_SIZE];
} WPHY_TOOL_GaugeInd;

typedef struct {
    VOS_UINT32 regAdd;
    VOS_UINT32 regValue;
} WR_REG;

/*
 * �ṹ˵��: TOOLͨ��DSP��дRFIC�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    VOS_UINT8  writeReadFlag; /* 0:Read����ʱ��Ҫ�ظ�CNF��IND */
    VOS_UINT8  channelNo;     /* ��Ҫ��ȡRF��Ӧ��ͨ����:0:��ʾͨ��0,1:��ʾͨ��1 */
    VOS_UINT16 regCnt;        /* 1~1024 */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrRficRegReq;

/*
 * �ṹ˵��: TOOLͨ��DSP��дRFIC�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId;             /* ��� ID */
    VOS_UINT16                  writeReadFlag;           /* 0:Read */
    VOS_UINT16                  regCnt;                  /* 1~1024 */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrRficRegInd;

/*
 * �ṹ˵��: TOOLͨ��BBP��дRFIC�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    VOS_UINT16 writeReadFlag; /* 0:Read����ʱ��Ҫ�ظ�CNF��IND 1:Write����ʱֻ��Ҫ�ظ�CNF */
    VOS_UINT16 regCnt;        /* 1~1024 */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrBbpRegReq;

/*
 * �ṹ˵��: TOOLͨ��DSP��дBBP�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId; /* ��� ID */

    VOS_UINT16 writeReadFlag; /* 0:Read */
    VOS_UINT16 regCnt;        /* 1~1024 */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrRficBbpInd;

/*
 * �ṹ˵��: TOOLͨ��DSP��дRFIC�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef WPHY_RF_WrRficRegReq GPHY_RF_WR_RFIC_REG_REQ_STRU;

/*
 * �ṹ˵��: TOOLͨ��DSP��дRFIC�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef WPHY_RF_WrRficRegInd GPHY_RF_WR_RFIC_REG_IND_STRU;

/*
 * �ṹ˵��: TOOLͨ��BBP��дRFIC�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef WPHY_RF_WrBbpRegReq GPHY_RF_WR_BBP_REG_REQ_STRU;

/*
 * �ṹ˵��: TOOLͨ��DSP��дBBP�ļĴ�����Ҫ������/�������¾���Ч
 */
typedef WPHY_RF_WrRficBbpInd       GPHY_RF_WR_RFIC_BBP_IND_STRU;
typedef PHY_RF_CalWRxRssiMeasReq   PHY_RF_CAL_C_RX_RSSI_MEAS_REQ_STRU;
typedef PHY_RF_CalWRfMsgCnf        PHY_RF_CAL_C_RF_MSG_CNF_STRU;
typedef PHY_RF_CalWRxRssiMeasInd   PHY_RF_CAL_C_RX_RSSI_MEAS_IND_STRU;
typedef PHY_RF_CalWRfTxPdInd       PHY_RF_CAL_C_RF_TX_PD_IND_STRU;
typedef W_TX_PdParaCfg             C_TX_PD_PARA_CFG_STRU;
typedef PHY_RF_CalWTxRegCtrl       CSDR_CAL_RF_TX_REG_CTRL_STRU;
typedef W_TX_RficPaMode            CSDR_TX_RFIC_PA_MODE_STRU;
/* DCR���� */
typedef NV_W_RxDcoffset            PHY_RF_CAL_C_RX_DCOFFSET_STRU;
typedef PHY_RF_CalWSelfCalReq      PHY_RF_CAL_C_SELF_CAL_REQ_STRU;

/* RFIC��BBP��д */
typedef WPHY_RF_WrRficRegReq PHY_RF_CAL_C_WR_RFIC_REG_REQ_STRU;
typedef WPHY_RF_WrBbpRegReq  PHY_RF_CAL_C_WR_BBP_REG_REQ_STRU;
typedef WPHY_RF_WrRficRegInd PHY_RF_CAL_C_WR_RFIC_REG_IND_STRU;
typedef WPHY_RF_WrRficBbpInd PHY_RF_CAL_C_WR_BBP_REG_IND_STRU;

typedef ANYBAND_W_MrxDcoffsetCalData      ANYBAND_C_MRX_DCOFFSET_CAL_DATA_STRU;
typedef ANYBAND_W_TxMrxIqmismatchCalData  ANYBAND_C_TX_MRX_IQMISMATCH_CAL_DATA_STRU;
typedef ANYBAND_W_DpdLutCalData           ANYBAND_C_DPD_LUT_CAL_DATA_STRU;
typedef ANYBAND_W_TxEtPaGainRsltRpt       ANYBAND_C_TX_ET_PA_GAIN_RSLT_RPT_STRU;

typedef WPHY_RF_MrxDcoffsetIq CSDR_RF_MRX_DCOFFSET_IQ_STRU;
typedef NV_W_EtDelayFreqComp  NV_C_ET_DELAY_FREQ_COMP_STRU;

typedef struct {
    NV_RX_DcoffsetIq vga[2][16]; /* ��Ӧ16��VGA��Dcֵ */
} NV_C_RxDcoffsetIq;

typedef struct {
    VOS_UINT16               bandId;
    VOS_UINT16               rsv;
    NV_C_RxDcoffsetIq        nvDcOffset;
} ANYBAND_C_RxDcoffsetCalData;

typedef struct {
    RX_IQ divOn;
    RX_IQ divOff;
} ANY_CHAN_CRxIp2;

typedef struct {
    ANY_CHAN_CRxIp2 chan[3]; /* 0�����ŵ���1�����ŵ���2�����ŵ� */
} NV_C_RxIp2;

typedef struct {
    VOS_INT16 imd2;
    VOS_INT16 rsv;
} C_RX_Imd2;

typedef C_RX_Imd2 CSDR_RX_IMD2_STRU; /* ���IMD2ֵ */

typedef struct {
    C_RX_Imd2 divOn;
    C_RX_Imd2 divOff;
} ANY_CHAN_CRxImd2;

typedef struct {
    ANY_CHAN_CRxImd2 chan[3]; /* 0�����ŵ���1�����ŵ���2�����ŵ� */
} RESULT_C_RxIp2;

typedef struct {
    VOS_UINT16           bandId;
    VOS_UINT16           rsv;
    NV_C_RxIp2           nvIp2;     /* У׼��� */
    RESULT_C_RxIp2       resultIp2; /* IMD2 */
} ANYBAND_C_RxIp2CalData;

typedef NV_W_RxDcoffset            NV_C_RX_DCOFFSET_STRU;
typedef RESULT_W_RX_DCOFFSET_STRU  RESULT_C_RX_DCOFFSET_STRU;

typedef struct {
    VOS_UINT16                 bandId; /* ��MASKλ�ϱ� */
    VOS_UINT16                 rsv;
    NV_TX_IqMismatch           nvTxIqMismatch;
    RESULT_TX_IqMismatch       resultTxIqMismatch;
    NV_C_RX_DCOFFSET_STRU      nvRxDcOffset;
    RESULT_C_RX_DCOFFSET_STRU  resultRxDcOffset;
    NV_C_RxIp2                 nvRxIp2;
    RESULT_C_RxIp2             resultRxIp2;
} ANY_BAND_CSelfCalInd;

/*
 * �ṹ˵��: Cģ����У׼��Ϣ�ӿڲ���
 */
typedef struct {
    VOS_UINT16 msgId; /* ԭ������ */
    VOS_UINT16 rsv1;  /* ����λ   */

    VOS_UINT32 mask; /* ����W����У׼�ӿڶ��� */

    /* Afc Para Cfg */
    PHY_RF_CalAfcPara afcPara; /* AFC������ */

    /* Tx Para Cfg */
    VOS_UINT16 txBand;
    VOS_UINT16 mrxGain;  /* MRX Gain��λ */
    VOS_UINT32 txFreq;   /* Freq, KHZ��λ */
    VOS_UINT16 txEnable; /* ��8Bit���� ---Tx���� 0:��ʹ��,1:ʹ�� ��8Bit���� ---���Ʒ�ʽ 0:��������,1:CW�����ź� */
    W_TX_RficPaMode           txRficPaMode;
    W_TX_PowerCtrlModeUint16  txPowerCtrlMode; /* ����W����У׼�ӿڶ��� */
    VOS_UINT16                       rsv2;
    /* ����W����У׼�ӿڶ��壬ע��:RF6362�ã�RF6363�ñ���unHi6363TxPowerPara */
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;

    /* Pa Volt Para Cfg */
    W_TX_PaParaCfg        paParaCfg;
    VOS_UINT16            selfCal;

    /* Rx Para Cfg */
    VOS_UINT16 rxBand;
    VOS_UINT16 rsv3;
    VOS_UINT32 rxFreq; /* Freq, KHZ��λ */

    VOS_UINT16 rxEnable; /* ����W����У׼�ӿڶ��� */
    VOS_UINT16 rxAntSel; /* ����W����У׼�ӿڶ��� */

    W_RX_AgcGainCfg        rxAgcGain; /* ����W����У׼�ӿڶ��� */
    VOS_UINT16             rsv4;

    VOS_INT16 ip2IValue; /* IP2 Iͨ������ */
    VOS_INT16 ip2QValue; /* IP2 Qͨ������ */

    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION hi6363TxPowerPara; /* RF6363���ʿ��ƽṹ�� */

    /* ET delay �� MASK_W_TRX_CFG_TX_ET_TIMEDELAY ���� */
    VOS_INT16 signalDelay;  /* �ź�ͨ��ֻ��������ʱ�Ĵ�������  */
    VOS_INT16 envelopDelay; /* ����ͨ����ʱ,��λΪ1/1024sample */

    /* ������һ��dpd����Χ0~9 ���� MASK_W_TRX_CFG_TX_DPD ���� */
    VOS_INT16 dpdTableIndex;

    /*
     * IQ�������ã����� MASK_W_TRX_CFG_TX_V_IQ ����
     * ����û���������Ҫ����IQ����ʹ�� MASK_W_TRX_CFG_PD_ONOFF
     * ��DSP�����Viq�����ҵ�����ѹ���㣬ͨ��PD�ϱ�ԭ���ϱ�����ѹ����ʱ��PD���ֵ
     * PD�ϱ�ԭ��μ�PHY_RF_CalWRfTxPdInd
     */
    VOS_INT16 viq;
    VOS_INT16 viqStep;

    /* ��MASK_W_TRX_CFG_TX_ET_COMP���� */
    VOS_INT16  etPowerCompression; /* ETУ׼ʱ������ѹ���㣬��λ0.1dB */

    /* ET DPD�²�Ƶ�� */
    VOS_UINT16 enableEtDpdComp; /* BIT0:��ʾET����ʹ�ܣ�BIT1��ʾDPD����ʹ�� */
    VOS_UINT16 rsv6;
    VOS_INT16  etFreqTempComp;  /* ETƵ���²�����λ0.1dB */
    VOS_INT16  dpdFreqTempComp; /* DPDƵ���²�����λ0.1dB */

    PHY_CAL_MipidevUnit cltHvdacMipi[PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM];
} PHY_RF_CalCTrxParaCfgReq;

/*
 * �ṹ˵��: Cģ����У׼��Ϣ�ӿ�FRAME����
 */
typedef struct {
    VOS_UINT16         band;          /* Ƶ��ֻ������ȷ��ʹ����һ��Bc��Nv��,У׼Ƶ�����������õ�FreqΪ׼ */
    VOS_UINT16         stepCnt;       /* [1, 16] */
    VOS_UINT32         txFreq;        /* ��������Freq, KHz��λ */
    VOS_UINT32         rxFreq;        /* ��������Freq, KHz��λ */
    VOS_UINT16         txStepPattern; /* ����W����У׼�ӿڶ��� */
    VOS_UINT16         rxStepPattern; /* ����W����У׼�ӿڶ��� */
    ANY_STEP_Para      stepData[W_TRX_MAX_STEP_COUNT]; /* ����W����У׼�ӿڶ��� */
} C_TRX_FastFrame;

/*
 * �ṹ˵��: Cģ����У׼��Ϣ�ӿڲ���
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  rsv;

    VOS_UINT16                  txMask;          /* ����W����У׼�ӿڶ��� */
    W_TX_PowerCtrlModeUint16    txPowerCtrlMode; /* ����W����У׼�ӿڶ��� */
    /* ��װ��Լ����֧��2��4��8��pcg����Ӧ��stepwidth��2500,5000,10000 */
    C_FAST_StepWidthUint16      stepWidthMs;
    VOS_UINT16 rsv2;

    VOS_UINT16 txFrameCnt; /* ����W����У׼�ӿڶ��� */
    VOS_UINT16 rxFrameCnt; /* ����W����У׼�ӿڶ��� */

    C_TRX_FastFrame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalCTrxFastCalReq;

/*
 * �ṹ˵��: 6363��,Cģ����У׼��Ϣ�ӿڲ���
 */
typedef struct {
    /*
     * BIT[1]: APT�Ƿ���Ч
     * BIT[0]: MRX����Ƿ�ʹ��
     */
    /* Reference to MASK_W_TRX_FAST_CAL_REQ_TXMASK_* */
    VOS_UINT16                         txMask;
    W_TX_PowerCtrlMode                 txPowerCtrlMode;
    /* ��λus,TRXͬʱ���ڵ�ʱ����ҪTX��RX���ȱ���һ�£����ֻ��TX��ʱ����TX�ĳ���Ϊ�� */
    HI6363_FAST_StepWidthUint16        txStepWidthUs;
    VOS_UINT16                         rsv; /* ����  */

    VOS_UINT16 txChannCnt;
    VOS_UINT16 rxChannCnt;

    VOS_UINT8 data[C_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE]; /* ��������֧��8k����Ϣ����������ʹ��8000���ֽ� */
    /* �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬��Ϣ��ʵ�ʳ���Ϊ��Ч֡�� */
    /*
     * ʹ��ʱ��������˳�������ݣ���ͬʱ��TRX����Ҫȷ��ÿ��Ƶ���TRXʱ������
     * C_TX_MRX_FAST_CHANN_STRU        astTxMrxChan[usTxChannCnt];
     * C_RX_FAST_CHANN_STRU            astRxMrxChan[usRxChannCnt];
     */
} C_TRX_MrxFastCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    C_TRX_MrxFastCalReqData     data;
} PHY_RF_CalCTrxMrxFastCalReqHi6363;

typedef struct CSDR_CAL_RecvMsg_tag {
    VOS_MSG_HEADER
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  rsv;
    VOS_UINT32 *                msgBody;
} CSDR_CAL_RecvMsg;

/*
 * �ṹ˵��: �ڷ�����BTʱ��������Ϣ֪ͨGPHY�л�UEģʽ������ȫ��ͨ���ԡ�
 *           0:modem1������tcxo1 1:modem1������tcxo0��
 *           ��Ϣ��Ҫ��LOAD PHY֮�����á�
 */
typedef struct {
    VOS_MSG_HEADER                            /* ģ��Э��ջ����Ϣ�����VOSͷ */
        PHY_TOOL_MsgIdUint16        msgId;  /* ԭ������ */
    VOS_UINT16                      toolId; /* ����λ */
    VOS_UINT16                      ueMode; /* 0:modem1������tcxo1 1:modem1������tcxo0 */
    VOS_UINT16                      resvered;
} MPH_SWITCH_M1XoInd;

/*
 * �ṹ˵��: �ڷ�����BTʱ��������Ϣ֪ͨWPHY�л�UEģʽ������ȫ��ͨ���ԡ�
 *           0:w��Ƶ�ι����������� 1:W B8�����ڸ�����.
 */
typedef struct {
    VOS_MSG_HEADER                            /* ģ��Э��ջ����Ϣ�����VOSͷ */
        PHY_TOOL_MsgIdUint16        msgId;  /* ԭ������ */
    VOS_UINT16                      toolId; /* ����λ */
    VOS_UINT16                      ueMode; /* 0:w��Ƶ�ι����������� 1:W B8�����ڸ����� */
    VOS_UINT16                      resvered;
} RRC_PHY_SwitchMoantInd;

/*
 * �ṹ˵��: CDMA ET���ٵ�֡�ṹ
 */
typedef struct {
    VOS_UINT16        band;     /* �·�bandö��ֵ */
    VOS_UINT16        stepCnt;  /* [1, 16] */
    VOS_UINT32        uTxFreqKhz; /* liyong, w:Ƶ�㣬C��Ƶ��Khz */
    VOS_UINT16        txStepPattern;
    VOS_UINT16        rsv;
    ET_STEP_Para      stepData[W_TRX_MAX_STEP_COUNT];
} CDMA_ET_FastFrame;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  rsv;

    /*
     * BIT[1]������ѹ�����Ƿ���Ч���������ѹ������Ч��
     * ��ÿ����ѹ��DSP��Ҫ����Viq����ɺ��ҳ�ѹ�����Ӧ��Viq��
     * ���ϱ�ѹ�����Ӧ��MRXֵ
     * BIT[0]: PD����Ƿ�ʹ�ܣ����PD���ʹ�ܣ����ҵ�ѹ�����
     * ��Ҫ�ط�һ�Σ���ȡ��Ӧ��PDֵ���ϱ���
     */
    VOS_UINT16 mask;

    /* Reference to PHY_RF_CalWEtFastReq_* */
    /*
     * ���ʿ���ģʽ����unTxPowerParaʹ��ͬ�������롣
     * 0������ģʽ�� ��ʱunTxPowerPara.usTxPower10th��Ч��
     * 1������˥��ģʽ����ʱunTxPowerPara.usTxPowerAtten10th��Ч��
     * 2���Ĵ������Ʒ�ʽ����ʱunTxPowerPara.stTxRegCtrlPara��Ч��
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;

    VOS_UINT16 rsv1;         /* Ԥ�� */
    VOS_UINT16 calAlgorithm; /* 0 - ��ѹ��ģʽ��1 - ������ģʽ */
    VOS_UINT16 txFrameCnt;   /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */
    VOS_UINT16 rsv2;         /* Ԥ�� */

    /*
     * �ӿ��ĵ��а�����󳤶ȶ��壬���㲿��ֱ��ʹ�øýṹ�����ڴ棬
     * ��Ϣ��ʵ�ʳ���Ϊ��Ч֡��
     */
    CDMA_ET_FastFrame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalCsdrEtFastReq6362;

/*
 * �ṹ˵��: ET ����У׼���
 */
typedef struct {
    VOS_UINT32 paVccMv; /* �������ʵ��PA Vcc �ĵ�ѹ(mv) */
    VOS_UINT32 mrx;     /* MRXֵ */
} C_TX_EtLutFastCalRslt;

typedef struct {
    VOS_UINT16                      band;
    VOS_UINT16                      rsv;
    PHY_CAL_ComplexDataInt16        dpdLut[128];
} ANYBAND_CDMA_DpdLutCalData;

/*
 * �ṹ˵��: DPD LUTУ׼���
 */
typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len; /* ��¼У׼�ṹ�峤��,byteΪ��λ */

    VOS_UINT16                         errCode; /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                         errBand; /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16                         rsv;
    VOS_UINT16                         bandCnt; /* ���DSPƵ�θ��� */
    ANYBAND_CDMA_DpdLutCalData         calData[CDMA_SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCdmaDpdLutRpt;

/*
 * �ṹ˵��: ET�ϱ�����  for chicago
 */
typedef struct {
    VOS_UINT32 bitMap; /* У׼MASKλ */
    VOS_UINT32 len;    /* ��¼У׼�ṹ�峤��,byteΪ��λ */
    VOS_INT16  pdHkadc;
    VOS_UINT16 compressSingleToneV;
    VOS_UINT32 compressMrxCorr; /* MRX��·�������ֵ */
    VOS_UINT32 mrxCorr[64];     /* 64��MRXֵ */
} PHY_RF_CalCdmaEtLutRpt;

typedef struct {
    VOS_UINT16 bandId;
    VOS_UINT16 rsv;
    VOS_INT16  asEtDelayFreqComp[8]; /* delayУ׼����ϱ� */
} ANYBAND_CDMA_EtDelayCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len; /* ��¼ʵ��У׼�������,byteΪ��λ */

    VOS_UINT16 errCode;     /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16 errBand;     /* ���У׼ʧ�ܵ�Ƶ�� */
    VOS_UINT16 selfCalMask; /* ���DSP��У׼�� */
    VOS_UINT16 bandCnt;     /* ���DSPƵ�θ��� */

    ANYBAND_CDMA_EtDelayCalData calData[CDMA_SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCdmaEtDelayRpt;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId; /* ��� ID */
    VOS_INT32                   errorCode;   /* ������ */

    VOS_INT16 reflectReal;  /* ����ϵ��ʵ�� */
    VOS_INT16 reflectImage; /* ����ϵ���鲿 */
} PHY_RF_CalCCltReflectanceInd;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                  componentId; /* ��� ID */
    VOS_INT32                   errorCode;   /* ������ */

    VOS_UINT16 fwdPathDelay; /* ǰ��delay */
    VOS_UINT16 revPathDelay; /* ����delay */
} PHY_RF_CalCCltPathDelayInd;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of PhyRfCalInterface.h */

