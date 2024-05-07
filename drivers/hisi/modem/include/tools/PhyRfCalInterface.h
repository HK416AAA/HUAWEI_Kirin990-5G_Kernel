

/*
 * 1 其他头文件包含
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
 * 2 宏定义
 */

#define GPHY_RF_CAL_TX_ON                     0x5555 /* 老化测试中，使能发射 */
#define GPHY_RF_CAL_TX_OFF                    0xaaaa /* 老化测试中，停止发射 */

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

/* CDMA慢校接口中自校准MASK指示 */
#define MASK_CPHY_SLOW_SELF_CAL_ET            (1 << 10) /* ET LUT提取模式 */
#define CSDR_CAL_SLOW_SELF_CAL_DPD            0x500  /* DPD LUT提取模式 */

/* Mask code Used for PHY_RF_CalWTrxParaCfgReq */
#define MASK_W_TRX_CFG_TX_AFC                 (1 << 0)
#define MASK_W_TRX_CFG_TX_CFIX                (1 << 1)
#define MASK_W_TRX_CFG_TX_ARFCN               (1 << 2)
#define MASK_W_TRX_CFG_TX_ONOFF               (1 << 3)
#define MASK_W_TRX_CFG_TX_RFIC_MODE           (1 << 4)
#define MASK_W_TRX_CFG_TX_PAMODE              (1 << 5)
#define MASK_W_TRX_CFG_TX_POWER_PARA          (1 << 6)
#define MASK_W_TRX_CFG_TX_PA_VCC              (1 << 7)  /* APT ET调试时均是强配PA电压，共用一个掩码 */
#define MASK_W_TRX_CFG_PD_MRX_ONOFF           (1 << 8)  /* Hi6363时MRX档位配置和Mrx Corr能量上报共用该掩码 */
#define MASK_W_TRX_CFG_TX_UPA                 (1 << 9)
#define MASK_W_TRX_CFG_SELF_CAL               (1 << 10) /* ET/DPD提取 */
#define MASK_W_TRX_CFG_TX_C2FIX               (1 << 11)
#define MASK_W_TRX_CFG_TX_DPA                 (1 << 12)
#define MASK_W_TRX_CFG_TX_ET_TIMEDELAY        (1 << 13)
#define MASK_W_TRX_CFG_TX_DPD                 (1 << 14)
/* 如果只有Viq的掩码，表示输出恒定幅度，如果Viq和Step的掩码都打开，表示幅度从Viq步进，到最大幅度后，自动循环 */
#define MASK_W_TRX_CFG_TX_V_IQ                (1 << 15)
#define MASK_W_TRX_CFG_RX_ARFCN               (1 << 16)
#define MASK_W_TRX_CFG_RX_CROSS_ARFCN         (1 << 17)
#define MASK_W_TRX_CFG_RX_ONOFF               (1 << 18)
#define MASK_W_TRX_CFG_RX_ANTSEL              (1 << 19)
#define MASK_W_TRX_CFG_RX_AGC_GAIN            (1 << 20)
#define MASK_W_TRX_CFG_RX_EVM                 (1 << 21)
#define MASK_W_TRX_CFG_XO_SELFCAL             (1 << 22)
#define MASK_W_TRX_CFG_CLT_HVDAC              (1 << 23)
#define MASK_W_TRX_CFG_CLT_PATH_DELAY         (1 << 25) /* 需要提取path delay值并上报 */
#define MASK_W_TRX_CFG_CLT_REFLECTANCE_DETECT (1 << 26) /* 需要提取反射系数并上报 */
#define MASK_W_TRX_CFG_DPD_STEP_V             (1 << 27)
#define MASK_W_TRX_CFG_TX_ETDPD_COMP          (1 << 28) /* ETDPD温补/频补 */

/* ET校准的增益压缩点是否生效，如果掩码为1，则增益压缩点使用原语带的值，否则使用NV的值 */
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

/* 用于维测 */
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

#define G_RAMP_PWR_LEVEL_NUM                  7     /* 爬坡表数，最多7张表 */
#define G_RAMP_COEF_NUM                       16    /* 爬坡点数 */

/* W Rx AGC GAIN COUNT */
#define W_RX_NOBLOCK_AGC_GAIN_COUNT           8
#define W_RX_BLOCK_AGC_GAIN_COUNT             8

#define W_TRX_MAX_STEP_COUNT                  16
#define W_TRX_MAX_SWEEP_FRAME_COUNT           38    /* PC一次下发给UE侧数据包最大8K限制 */
/* DSP一次上报的最大个数，包括Rssi At1(0.125dBm)+Rssi At2(0.125dBm)+PD测量结果 */
#define W_TRX_MAX_REPORT_COUNT                1500

/* W模校准上报时成功失败标识 */
#define WPHY_TOOL_CAL_RESULT_RPT_SUCCESS      0

/* C模校准上报时成功失败标识 */
#define CPHY_TOOL_CAL_RESULT_RPT_SUCCESS      0

/* W模自校准的频段最大个数 */
#define SELF_CAL_BAND_ID_MAX_COUNT            10

#define SELF_CAL_ITEMS_MAX_COUNT              32

/* X模自校准的频段最大个数 */
#define CDMA_SELF_CAL_BAND_ID_MAX_COUNT       2

#define WG_BBP_RF_REG_WR_MAX_COUNT            8

/* X模寄存器读写个数 */
#define CSDR_BBP_RF_REG_WR_MAX_COUNT WG_BBP_RF_REG_WR_MAX_COUNT

/* G TRX CFG MARCO */
/* Mask code Used for RF_CAL_G_TX_PARA_CFG_REQ_STRU */
#define MASK_CAL_RF_G_TX_AFC                  (1 << 0) /* 上行单板频率控制值 */
#define MASK_CAL_RF_G_TX_ARFCN                (1 << 1) /* 上行频点是否生效 */
#define MASK_CAL_RF_G_TX_ONOFF                (1 << 2) /* 上行发射开关是否生效 */
#define MASK_CAL_RF_G_TX_DATA_PATTERN         (1 << 3) /* 上行的数据类型是否生效 */
#define MASK_CAL_RF_G_TX_CFIX                 (1 << 4) /* 上行Cfix是否生效 */
#define MASK_CAL_RF_G_TX_SLOT_CFG             (1 << 5) /* 上行的时隙配置是否生效 */
#define MASK_CAL_RF_G_TX_C2FIX                (1 << 6) /* 上行Cfix是否生效 */
#define MASK_CAL_RF_G_GET_CLT_RFT_VALUE       (1 << 7) /* Tuner反射系数提取 */
#define MASK_CAL_RF_G_GET_CLT_PATH_DELEAY     (1 << 8) /* Tuner Delay延迟提取 */
#define MASK_CAL_RF_G_GET_CLT_HVDAC           (1 << 9) /* 表示需要HVDAC配置有效 */

/* Mask code Used for RF_CAL_G_RX_PARA_CFG_REQ_STRU */
#define MASK_CAL_RF_G_RX_ARFCN                (1 << 0) /* 下发频点 */
#define MASK_CAL_RF_G_RX_MODE                 (1 << 1) /* 下发接收模式 */
#define MASK_CAL_RF_G_RX_AGCMODE              (1 << 2) /* 下发测量模式 */
#define MASK_CAL_RF_G_RX_AGCGAIN              (1 << 3) /* 下发AGC档位 */
#define MASK_CAL_RF_G_RX_BLOCKERMODE          (1 << 4) /* 下发AGC阻塞模式 */
#define MASK_CAL_RF_G_RX_ANTSEL               (1 << 5) /* 下发AGC天线选择 */

/* Mask code User for PHY_RF_CalGTxFastCalReq */
#define MASK_G_TX_FAST_CAL_REQ_TXMASK_APT     (1 << 0) /* APT使能 */
#define MASK_G_TX_FAST_CAL_REQ_TXMASK_MRX     (1 << 1) /* MRx上报使能 */

#define G_TX_PARA_MAX_SLOT_CNT_PER_FRAME      4

#define G_TX_MAX_STEP_COUNT                   7     /* 当前只支持4时隙 */
#define G_TRX_MAX_SWEEP_FRAME_COUNT           120   /* Hi6362只支持120帧 */
#define G_TMRX_MAX_SWEEP_FRAME_COUNT          400   /* Hi6363开始最多支持400帧 */
#define G_RX_MAX_STEP_COUNT                   7
#define G_RX_MAX_RSSI_COUNT (G_TRX_MAX_SWEEP_FRAME_COUNT * G_RX_MAX_STEP_COUNT * 2) /* 分集天线测量，上报数据*2 */
#define G_RX_AGC_GAIN_COUNT                   8
#define G_RX_MAX_CFG_REQ_DATA_COUNT           2     /* 慢速校准REQ消息修改为动态消息长度，支持2个时隙的动态调整 */

#define W_GAUGE_RESULT_SIZE                   13    /* 32K校准上报数据个数 */

#define GSM_TX_HD35_PCL_NUM                   3
#define GSM_TX_HD35_ARFCN_NUM                 3
#define GSM_TX_HD35_STEP_NUM                  5
#define GSM_TX_HD35_BAND_NUM                  4

#define GPHY_RX_IQ_MISMATCH_SLOT_NUM          64    /* 总采集64(4*16)个时隙数据，16帧，每帧4个时隙 */

#define W_RX_EVM_REPORT_ON                    1     /* EVM上报打开 */
#define W_RX_EVM_REPORT_OFF                   0     /* EVM上报关闭 */
#define W_RX_EVM_REPORT_DATA_COUNT            10240 /* EVM一次上报最大的数据,32bit */
#define COMM_SELF_CAL_TESTITEM_NUM            32    /* 通用自校准，装备一次下发支持的最大自校准项 */

/*
 * 消息头GSM 模式ID Componentid
 * lte          0x3000
 * tds          0x3100
 * gsm          0x3200
 * wcdma        0x3300
 * x            0x3400
 * 与模式无关   0x3f00
 */

#define GPHY_MSGHEAD_COMPONENT_ID             0x3200
#define WPHY_MSGHEAD_COMPONENT_ID             0x3300
#define CPHY_MSGHEAD_COMPONENT_ID             0x3400

#define W_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE    16000 /* W快速校准接口一次可以给DSP下发的最大数据包长度 */
#define C_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE    2000  /* C快速校准接口一次可以给DSP下发的最大数据包长度 */
/* DSP一次上报的最大个数，包括Rssi At1(0.125dBm)+Rssi At2(0.125dBm)+MRX测量结果,共4个字节 */
#define W_TRX_MAX_REPORT_COUNT_HI6363         4000

/* W DCXO 样本最大个数 */
#define DCXO_SAMPLE_MAX_NUM                   24

#define PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM       3

/*
 * 3 枚举定义
 */

/*
 * 枚举说明:RF CAL MSG ID ENUM
 */
enum PHY_TOOL_MsgId {
    /* WCDMA RF快速校准接口原语列表 */
    ID_TOOL_WPHY_CAL_TRX_FAST_CAL_REQ = 0x1240, /* _H2ASN_MsgChoice PHY_RF_CalWTrxFastCalReq */

    ID_TOOL_WPHY_CAL_PD_PARA_CFG_REQ = 0x1241, /* _H2ASN_MsgChoice PHY_RF_CAL_PD_PARA_CFG_REQ_STRU */

    /* WCDMA RF自校准接口原语列表 */
    ID_TOOL_WPHY_CAL_SELF_CAL_REQ = 0x1242, /* _H2ASN_MsgChoice PHY_RF_CalWSelfCalReq */

    /* WCDMA RF慢速校准接口原语列表 */
    ID_TOOL_WPHY_CAL_TRX_PARA_CFG_REQ = 0x1243, /* _H2ASN_MsgChoice PHY_RF_CalWTrxParaCfgReq */
    ID_TOOL_WPHY_CAL_RX_RSSI_MEAS_REQ = 0x1244, /* _H2ASN_MsgChoice PHY_RF_CalWRxRssiMeasReq */
    ID_WPHY_TOOL_GAUGE_REQ            = 0x1245, /* _H2ASN_MsgChoice WPHY_TOOL_GaugeReq */

    /* W寄存器读写接口 */
    ID_TOOL_WPHY_WR_RFIC_REG_REQ = 0x1246,
    ID_TOOL_WPHY_WR_BBP_REG_REQ  = 0x1247,

    /* WCDMA ET快速校准接口 */
    ID_TOOL_WPHY_CAL_ET_FAST_REQ = 0x1248,

    /* WCDMA ET Delay Freq Comp自校准接口原语 */
    /* _H2ASN_MsgChoice PHY_RF_CAL_ET_DELAY_SELF_CAL_REQ_STRU */
    ID_TOOL_WPHY_CAL_ET_DELAY_FREQ_COMP_SELF_CAL_REQ = 0x1249,

    /* DCXO自校准 */
    ID_TOOL_WPHY_CAL_XO_COFFICIENT_REQ = 0x124a,

    ID_TOOL_WPHY_CAL_DPD_LUT_SELF_CAL_REQ = 0x124b,

    /* DPD指定频点校准消息ID */
    ID_TOOL_WPHY_CAL_ASIGNED_CHAN_DPD_LUT_SELF_CAL_REQ = 0x124c,

    /* 非信令设置DPDT消息ID */
    ID_TOOL_WPHY_CAL_SET_DPDT_VALUE_REQ = 0x124d,

    ID_RRC_PHY_SWITCH_MOANT_IND = 0x1250, /* _H2ASN_MsgChoice RRC_PHY_SwitchMoantInd */

    /* W RF通用CNF */
    ID_WPHY_TOOL_CAL_RF_MSG_CNF = 0x21F0, /* _H2ASN_MsgChoice PHY_RF_CalWRfMsgCnf */

    ID_WPHY_TOOL_CAL_TX_POWER_DETECT_IND = 0x21F1, /* _H2ASN_MsgChoice PHY_RF_CalWRfTxPdInd */
    ID_WPHY_TOOL_GAUGE_CNF               = 0x21F2, /* _H2ASN_MsgChoice WPHY_TOOL_GaugeCnf */
    ID_WPHY_TOOL_GAUGE_IND               = 0x21F3, /* _H2ASN_MsgChoice WPHY_TOOL_GaugeInd */

    ID_WPHY_TOOL_CAL_TRX_FAST_CAL_IND = 0x21F4, /* _H2ASN_MsgChoice PHY_RF_CalWTrxFastCalInd */
    ID_WPHY_TOOL_CAL_SELF_CAL_IND     = 0x21F5, /* _H2ASN_MsgChoice PHY_RF_CalWSelfCalInd */

    ID_WPHY_TOOL_CAL_RX_RSSI_MEAS_IND = 0x21F6, /* _H2ASN_MsgChoice PHY_RF_CalWRxRssiMeasInd */

    ID_WPHY_TOOL_R_RFIC_REG_IND = 0x21F8,
    ID_WPHY_TOOL_R_BBP_REG_IND  = 0x21F9,

    /* W IP2自校准获取IM2上报接口 */
    ID_WPHY_TOOL_IP2_IM2_VALUE_IND = 0x21FA,
    ID_WPHY_TOOL_RX_EVM_MEAS_IND   = 0x21FB,

    /* 物理层通用数据上报 */
    ID_PHY_TOOL_COMM_DATA_IND = 0x21FD,

    /* 物理层可维可测数据上报 */
    ID_PHY_TOOL_MNTN_DATA_RPT          = 0x21FE,
    ID_WPHY_TOOL_CAL_XO_COFFICIENT_IND = 0x21FF, /* _H2ASN_MsgChoice PHY_RF_CalWRxRssiMeasInd */

    /* Path Delay上报 */
    ID_WPHY_TOOL_CAL_CLT_PATH_DELAY_IND = 0x2200,

    /* 反射系数上报 */
    ID_WPHY_TOOL_CAL_CLT_REFLECTANCE_IND = 0x2201,

    /* 慢速强配HVDAC消息上报 */
    ID_WPHY_TOOL_CAL_CLT_HVDAC_CFG_IND = 0x2202,

    /* 非信令设置DPDT CNF回复上报ID */
    ID_WPHY_TOOL_CAL_SET_DPDT_VALUE_CNF = 0x2203,

    // CDMA消息ID的定义，ID还需要重新定义
    /* CDMA RF快速校准接口原语列表 */
    ID_TOOL_CPHY_CAL_TRX_FAST_CAL_REQ = 0x2F00,

    ID_TOOL_CPHY_CAL_PD_PARA_CFG_REQ = 0x2F01,

    /* CDMA RF自校准接口原语列表 */
    ID_TOOL_CPHY_CAL_SELF_CAL_REQ = 0x2F02,

    /* CDMA RF慢速校准接口原语列表 */
    ID_TOOL_CPHY_CAL_TRX_PARA_CFG_REQ = 0x2F03,
    ID_TOOL_CPHY_CAL_RX_RSSI_MEAS_REQ = 0x2F04,

    /* CDMA RFIC和BBP读写接口原语列表 */
    ID_TOOL_CPHY_CAL_WR_RFIC_REG_REQ = 0x2F05,
    ID_TOOL_CPHY_CAL_WR_BBP_REG_REQ  = 0x2F06,

    /* CDMA ET LUT修正，快校消息 */
    ID_TOOL_CPHY_CAL_ET_LUT_FAST_CAL_REQ = 0x2F07,

    /* CDMA ET Delay (包括delay频补)自校 */
    ID_TOOL_CPHY_CAL_ET_DELAY_SELF_CAL_REQ = 0x2F08,

    /* CT和HIRF设置DPDT消息ID */
    ID_TOOL_CPHY_CAL_SET_DPDT_VALUE_REQ = 0x2F09,

    /* CDMA RF通用CNF */
    ID_CPHY_TOOL_CAL_RF_MSG_CNF = 0x2F10,

    /* CDMA通用数据上报,ET DPD delay 校准结果上报 */
    ID_CPHY_TOOL_COMM_DATA_IND = 0x2F11,

    ID_CPHY_TOOL_CAL_TX_POWER_DETECT_IND = 0x2F20,

    /* TRX快校 结果上报 */
    ID_CPHY_TOOL_CAL_TRX_FAST_CAL_IND = 0x2F21,

    /* 自校准 结果上报 */
    ID_CPHY_TOOL_CAL_SELF_CAL_IND = 0x2F22,

    ID_CPHY_TOOL_CAL_RX_RSSI_MEAS_IND = 0x2F23,

    /* CDMA RFIC和BBP读标记IND */
    ID_CPHY_TOOL_CAL_R_RFIC_REG_IND = 0x2F24,
    ID_CPHY_TOOL_CAL_R_BBP_REG_IND  = 0x2F25,

    /* pathdelay 提取上报 */
    ID_CPHY_TOOL_CAL_CLT_PATH_DELAY_IND = 0x2F26,

    /* hitune 反射系数提取上报 */
    ID_CPHY_TOOL_CAL_CLT_REFLECTANCE_IND = 0x2F27,

    ID_CPHY_TOOL_CAL_CLT_HVDAC_CFG_IND = 0x2F28,

    /* CT和HIRF配置DPDT值CNF上报 */
    ID_CPHY_TOOL_CAL_SET_DPDT_VALUE_CNF = 0x2F29,

    /* GSM RF快速校准接口原语列表 */
    ID_TOOL_GPHY_CAL_RX_FAST_CAL_REQ = 0x1740, /* _H2ASN_MsgChoice PHY_RF_CalGRxFastCalReq */
    ID_TOOL_GPHY_CAL_TX_FAST_CAL_REQ = 0x1741, /* _H2ASN_MsgChoice PHY_RF_CalGTxFastCalReq */

    /* GSM 自校准接口原语列表 */
    ID_TOOL_GPHY_CAL_SELF_CAL_REQ   = 0x1742, /* _H2ASN_MsgChoice PHY_RF_CAL_G_SELF_CAL_REQ_STRU */
    ID_TOOL_GPHY_CAL_RX_DCR_CAL_REQ = 0x1743, /* _H2ASN_MsgChoice PHY_RF_CalGRxDcrReq */

    /* GSM RF慢速校准接口原语列表 */
    ID_TOOL_GPHY_CAL_TX_PARA_CFG_REQ  = 0x1744, /* _H2ASN_MsgChoice PHY_RF_CalGTxParaCfgReq */
    ID_TOOL_GPHY_CAL_RX_PARA_CFG_REQ  = 0x1745, /* _H2ASN_MsgChoice PHY_RF_CalGRxParaCfgReq */
    ID_TOOL_GPHY_CAL_RX_RSSI_MEAS_REQ = 0x1746, /* _H2ASN_MsgChoice PHY_RF_CalGRxRssiMeasReq */

    ID_TOOL_GPHY_WR_RFIC_REG_REQ = 0x1747,
    ID_TOOL_GPHY_WR_BBP_REG_REQ  = 0x1748,

    /* GSM PA RAMP 配置接口 */
    ID_TOOL_GPHY_CAL_PA_RAMP_CFG_REQ = 0x1749,

    /* GSM TIMING 配置接口 */
    ID_TOOL_GPHY_CAL_TIMING_CFG_REQ     = 0x1750,
    ID_TOOL_GPHY_CAL_RX_IQ_MISMATCH_REQ = 0x1751,

    /* GSM HD3HD5校准原语 */
    ID_TOOL_GPHY_CAL_TX_HD3_HD5_REQ = 0x1752,

    ID_MPH_SWITCH_M1XO_IND = 0x1753,

    ID_TOOL_GPHY_SWITCH_MOANT_IND = 0x1754,

    /* Hi6363自校准原语,IND消息通过ID_GPHY_TOOL_COMM_DATA_IND上报 */
    ID_TOOL_GPHY_CAL_COMM_SELF_CAL_REQ = 0x1755, /* _H2ASN_MsgChoice  GPHY_RF_CalCommSelfCalReq */

    /* Hi6363 Tx快速校准原语 */
    ID_TOOL_GPHY_CAL_TMRX_FAST_CAL_REQ = 0x1756, /* _H2ASN_MsgChoice  PHY_RF_CalGeTmrxFastCalReq */

    ID_TOOL_GPHY_CAL_MRX_CORR_REQ = 0x1757, /* _H2ASN_MsgChoice  PHY_RF_CalGeTmrxFastCalReq */

    /* G RF通用CNF */
    ID_GPHY_TOOL_CAL_RF_MSG_CNF = 0x7120, /* _H2ASN_MsgChoice  PHY_RF_CAL_G_RF_MSG_CNF_STRU */

    ID_GPHY_TOOL_CAL_RX_FAST_CAL_IND = 0x7121, /* _H2ASN_MsgChoice  PHY_RF_CalGRxFastCalInd */

    ID_GPHY_TOOL_CAL_SELF_CAL_IND     = 0x7122, /* _H2ASN_MsgChoice  PHY_RF_CAL_G_SELF_CAL_IND_STRU */
    ID_GPHY_TOOL_CAL_RX_DCR_CAL_IND   = 0x7123, /* _H2ASN_MsgChoice  PHY_RF_CAL_G_RX_DCR_RPT_IND_STRU */
    ID_GPHY_TOOL_CAL_RX_RSSI_MEAS_IND = 0x7124, /* _H2ASN_MsgChoice  PHY_RF_CalGRxRssiMeasInd */

    ID_GPHY_TOOL_R_RFIC_REG_IND = 0x7126,
    ID_GPHY_TOOL_R_BBP_REG_IND  = 0x7127,

    ID_GPHY_TOOL_CAL_RX_IQ_MISMATCH_IND = 0x7128,

    ID_GPHY_TOOL_CAL_TX_HD3_HD5_IND = 0x7129,

    /* G模物理层通用数据上报接口 */
    ID_GPHY_TOOL_COMM_DATA_IND         = 0x7130,
    ID_GPHY_TOOL_CAL_TMRX_FAST_CAL_IND = 0x7131, /* _H2ASN_MsgChoice  PHY_RF_CalGeTmrxFastCalInd */

    ID_GPHY_TOOL_CAL_MRX_CORR_IND = 0x7132, /* _H2ASN_MsgChoice  PHY_RF_CalGMrxCorrInd */

    ID_GPHY_TOOL_CAL_CLT_RFT_IND = 0x7133,

    ID_GPHY_TOOL_CAL_CLT_HVDAC_CFG_IND = 0x7135,
};
typedef VOS_UINT16 PHY_TOOL_MsgIdUint16;

/*
 * 枚举说明:物理层自校准失败原因说明
 */
enum RF_CAL_SelfCalErrCode {
    RF_CAL_SELF_CAL_SUCCESS                         = 0,
    RF_CAL_SELF_CAL_FAIL                            = 1,
    RF_CAL_SELF_CAL_IP2_MEM_ALLOC_FAIL              = 2,    /* IP2校准时,申请uncache内存失败 */
    /* 找不到满足失败门限的点,若有些频点是好的,可能是双工隔离太差 */
    RF_CAL_SELF_CAL_IP2_GREATER_THAN_FAIL_THRESHOLD = 3,
    RF_CAL_SELF_CAL_IP2_SAMPLE_DATA_FAIL            = 4,    /* 采数失败,说明EDMA通道申请出了问题 */
    RF_CAL_SELF_CAL_IQMISMATCH_CORR_FAIL            = 5,    /* BBP相关运算失败 */
    RF_CAL_SELF_CAL_IQMISMATCH_OVER_CODE            = 6,    /* A,P,I,Q中至少有一个码字超出[-64,63] */
    RF_CAL_SELF_CAL_DCR_CORR_FAIL                   = 7,    /* DCR相关运算失败 */
    RF_CAL_SELF_CAL_DCR_ITERATIVE_FAIL              = 8,    /* 迭代三次仍然不满足门限要求，校准失败 */
    RF_CAL_SELF_CAL_LOLEAKAGE_MEM_ALLOC_FAIL        = 9,    /* 分配内存失败 */
    RF_CAL_SELF_CAL_LOLEAKAGE_SAMPLE_DATA_FAIL      = 10,   /* Lo Leakage采数失败 */
    RF_CAL_SELF_CAL_LOLEAKAGE_CORR_FAIL             = 11,   /* 相关运算失败 */
    RF_CAL_SELF_CAL_LOLEAKAGE_CALC_FAIL             = 12,   /* Lo Leakage计算失败 */
    RF_CAL_SELF_CAL_IQMISMATCH_SAMPLE_DATA_FAIL     = 13,   /* IQMISMATCH采数据错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP2_MCTMP_FAIL     = 14,   /* 计算TX IQMC_Tmp 和 MRXIQMC值错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP2_AP_FAIL        = 15,   /* 计算第一次A、P值错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_IR_FAIL        = 16,   /* 计算IR_Linear错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_ANGLE_FAIL     = 17,   /* 计算调整角度错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_AP_FAIL        = 18,   /* 计算第二次A、P值错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP4_IR_FAIL        = 19,   /* 计算IR1错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP4_AP_FAIL        = 20,   /* 计算第三次A、P值错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP5_IR_FAIL        = 21,   /* 计算IR2错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_5TIMES_FAIL          = 22,   /* 计算IR2错误 */
    RF_CAL_SELF_CAL_SYS_TIME_SYN_FAIL               = 23,   /* CFN/SFN同步失败 */
    RF_CAL_SELF_CAL_LOLEAKAGE_STEP1_SAMPLE_DATA_FAIL  = 24, /* LOLeakge Step1采数失败 */
    RF_CAL_SELF_CAL_LOLEAKAGE_STEP2_SAMPLE_DATA_FAIL  = 25, /* LOLeakge Step2采数失败 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP1_SAMPLE_DATA_FAIL = 26, /* IQ Step1采数据错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP2_SAMPLE_DATA_FAIL = 27, /* IQ Step2采数据错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP3_SAMPLE_DATA_FAIL = 28, /* IQ Step3采数据错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_STEP4_SAMPLE_DATA_FAIL = 29, /* IQ Step4采数据错误 */
    RF_CAL_SELF_CAL_ET_DPD_MEM_ALLOC_FAIL             = 30, /* DPD校准时,申请uncache内存失败 */
    RF_CAL_SELF_CAL_ET_DPD_DATA_SYNC_FAIL             = 31, /* 数据同步失败 */
    RF_CAL_SELF_CAL_ET_DPD_MISS_FIRST_POINT           = 32, /* 数据同步失败，没有采到单音突变 */
    RF_CAL_SELF_CAL_DPD_AMAM_CALC_FAIL                = 33, /* DPD AMAM计算错误 */
    RF_CAL_SELF_CAL_DPD_SAMPLE_DATA_FAIL              = 34, /* 采数据错误 */
    RF_CAL_SELF_CAL_DPD_CALC_AMPM_FAIL                = 35, /* 计算平均相位平均幅度失败 */
    RF_CAL_SELF_CAL_ET_SAMPLE_DATA_FAIL               = 36, /* ET采数据错误 */
    RF_CAL_SELF_CAL_ET_GET_COMPRESS_POINT_FAIL        = 37, /* 采数据错误 */
    RF_CAL_SELF_CAL_ET_TIME_DELAY_SAMPLE_DATA_FAIL    = 38, /* ET_TIME_DELAY采数据错误 */
    RF_CAL_SELF_CAL_IQMISMATCH_SYNC_FAIL              = 39, /* 获取同步位置失败 */
    RF_CAL_SELF_CAL_IQMISMATCH_SET_SAMPLE_TIME_FAIL   = 40, /* 查找定时采数起始时间失败 */
    RF_CAL_SELF_CAL_IQMISMATCH_DATA_TRANSFER_FAIL     = 41, /* 搬数失败,DMA或BBPMASTER */
    RF_CAL_SELF_CAL_ET_TIME_DELAY_CENTER_NV_ERROR     = 42, /* NV给的期望delay有风险，可能导致所有遍历点不经过最低点 */
    RF_CAL_SELF_CAL_IQMISMATCH_ANGLE_CALC_FAIL        = 43, /* c/a虚部为负，不满足+90度要求 */
    RF_CAL_SELF_CAL_ET_DELAY_ACLR_MEM_ALLOC_FAIL      = 44, /* Delay计算Aclr时申请内存失败 */
    RF_CAL_SELF_CAL_ET_DELAY_ACLR_CENTER_SQUARE_FULL  = 45, /* Delay计算Aclr时中心信号平方和溢出 */
    RF_CAL_SELF_CAL_DPD_FAST_SELF_CAL_MASK_FAIL       = 46, /* 工具下发DPD校准MASK位错误 */
    RF_CAL_SELF_CAL_IP2_DCR_FAIL                      = 47, /* IP2校准时去直流失败 */
    RF_CAL_SELF_CAL_MRX_DCOFFSET_LOST_ISR             = 48, /* MRX_DCOFFSET没收到中断 */
    RF_CAL_SELF_CAL_MRX_DCOFFSET_ITERATIVE_FAIL       = 49, /* MRX_DCOFFSET三次迭代失败 */
    RF_CAL_SELF_CAL_READ_HKADC_FAIL                   = 50, /* 查询PD电压失败 */
    RF_CAL_SELF_CAL_GET_PD_POWER_FAIL                 = 51, /* 查询PD电压失败 */
    RF_CAL_SELF_CAL_ET_PA_GAIN_FAIL                   = 52, /* PA Gain校准失败 */
    W_TX_SYMBOL_CAL_SAMPLE_TIMEOUT                    = 53, /* SYMBOL级TX校准Mrx 采数超时 */
    RF_CAL_SELF_CAL_CLT_MEM_ALLOC_FAIL                = 54, /* 闭环tuner校准内存申请失败 */
    RF_CAL_SELF_CAL_CLT_DETECT_DATA_FAIL              = 55, /* 闭环tuner校准采数失败 */
    RF_CAL_SELF_CAL_CLT_RSLT_MEAS_INVALID_FAIL        = 56, /* 闭环tuner第三方算法结果不可用 */
    RF_CAL_SELF_CAL_CLT_UNSUPPORT_FAIL                = 57, /* 不支持闭环 */
    RF_CAL_SELF_CAL_IQMISMATCH_TRANSIT_DATA_FAIL      = 1000, /* 搬数失败 */
    RF_CAL_SELF_CAL_BUTT
};
typedef VOS_UINT16 RF_CAL_SelfCalErrCodeUint16;

/*
 * 枚举说明:RF CAL W TRX FAST ENUM
 */
enum W_TX_RficMode {
    W_TX_RFIC_MODE_NORMAL = 0,
    W_TX_RFIC_MODE_VOICE,
    W_TX_RFIC_MODE_BUTT
};
typedef VOS_UINT16 W_TX_RficModeUint16;

/*
 * 枚举说明:RF CAL W TRX FAST ENUM
 */
enum W_TX_PowerCtrlModeType {
    W_TX_CAL_POWER_CTRL_POWER = 0,
    W_TX_CAL_POWER_CTRL_POWER_ATTEN,
    W_TX_CAL_POWER_CTRL_REGISTER,
    W_TX_CAL_POWER_CTRL_BUTT
};
typedef VOS_UINT16 W_TX_PowerCtrlModeUint16;

/*
 * 枚举说明:RF CAL W TRX FAST ENUM
 */

enum W_FAST_StepWidth {
    WIDTH_2MS  = 2,
    WIDTH_10MS = 10,
    WIDTH_20MS = 20,
    WIDTH_BUTT
};
typedef VOS_UINT16 W_FAST_StepWidthUint16;

/* W PA模式 */
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

/* W Rx 载波模式 0:单载波, 1:双载波 */
enum W_RX_CarrierMode {
    W_RX_CARRIER_MODE_SC = 0,
    W_RX_CARRIER_MODE_DC = 1,
};

/* W Rx BLOCK模式 0: No Block,   1:Block; */
enum W_RX_BlockMode {
    W_RX_WITHOUT_BLOCK = 0,
    W_RX_WITH_BLOCK,
};

/* W Rx 档位配置模式 0:档位自动调整 1:档位强配 */
enum W_RX_AgcCtrlMode {
    W_RX_AGC_CTRL_AUTO = 0,
    W_RX_AGC_CTRL_APPOINTED,
};

/*
 * 枚举说明:
 */
enum G_TX_ModulationType {
    MODE_GMSK = 0,
    MODE_8PSK = 1,
    MODE_CW,
    MODE_BUTT
};
typedef VOS_UINT16 G_TX_ModulationTypeUint16;

/*
 * 枚举说明:
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
 * 枚举说明:RF CAL C TRX FAST ENUM
 */

enum C_FAST_StepWidth {
    CSDR_2PCG_STEP_WIDTH_LENGTH = 2500,
    CSDR_4PCG_STEP_WIDTH_LENGTH = 5000,
    CSDR_8PCG_STEP_WIDTH_LENGTH = 10000,
    CSDR_PCG_STEP_WIDTH_LENGTH_BUTT
};
typedef VOS_UINT16 C_FAST_StepWidthUint16;

/*
 * 枚举说明:RF CAL TRX FAST STEP WIDTH ENUM
 */
enum HI6363_FAST_StepWidth {
    STEP_WIDTH_0US     = 0,     /* symbol级快速校准 */
    STEP_WIDTH_2000US  = 2000,  /* 2ms */
    STEP_WIDTH_2500US  = 2500,  /* 2.5ms,相当于C模的2个PCG */
    STEP_WIDTH_5000US  = 5000,  /* 5ms,相当于C模的4个PCG */
    STEP_WIDTH_10000US = 10000, /* 10ms,相当于C模的8个PCG */
    STEP_WIDTH_20000US = 20000, /* 20ms */
    STEP_WIDTH_BUTT
};
typedef VOS_UINT16 HI6363_FAST_StepWidthUint16;

/*
 * 枚举说明:W TX发射模式
 */
enum W_TX_ModMode {
    W_TX_MODU_MODE   = 0, /* 调制信号 */
    W_TX_SINGLE_MODE = 1, /* 单音信号 */
    W_TX_MOD_MODE_BUTT
};
typedef VOS_UINT16 W_TX_ModModeUint16;
/*
 * 4 消息头定义
 */

/*
 * 5 消息定义
 */

/*
 * 6 STRUCT定义
 */

/*
 * 结构说明: 接口头文件转定义MIPI命令控制结构体，与RF_NV_MIPIDEV_CMD_STRU
 */
typedef struct {
    VOS_UINT32 bitByteCnt : 5;     /* 0表示该CMMAND无效，取值0~4 */
    VOS_UINT32 bitMipiPortSel : 3; /* mipi port 0~7 */
    VOS_UINT32 bitSlaveID : 4;     /* slave id 0~15 */
    VOS_UINT32 bitRegAddr : 16;    /* reg addr 0~65535 */
    VOS_UINT32 bitRsv : 4;         /* 四字节对齐 */
} PHY_CAL_MipidevCmd;

/*
 * 结构说明: 接口头文件转定义MIPI数据结构体，与RF_NV_MIPIDEV_DATA_STRU结构一致
 */
typedef struct {
    VOS_UINT32 bitByte0 : 8; /* 第一个byte */
    VOS_UINT32 bitByte1 : 8; /* 第二个byte */
    VOS_UINT32 bitByte2 : 8; /* 第三个byte */
    VOS_UINT32 bitByte3 : 8; /* 第四个byte */
} PHY_CAL_MipidevData;

/*
 * 结构说明: 接口头文件转定义MIPI命令最小单元，结构与RF_NV_MIPIDEV_UNIT_STRU一致
 */
typedef struct {
    PHY_CAL_MipidevCmd  cmd;  /* 控制域 */
    PHY_CAL_MipidevData data; /* 数据域,一个MIPI占用2个UINT32 */
} PHY_CAL_MipidevUnit;

typedef struct {
    VOS_INT16 real; /* 复数实部 */
    VOS_INT16 img;  /* 复数虚部 */
} PHY_CAL_ComplexDataInt16;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件 ID */
} PHY_CAL_MsgDead;

/*
 * 结构说明: WCDMA RF慢速校准接口 -- UPA参数 ( 临时放在这里 )
 */
typedef struct {
    /* 下面的参数是发射UPA需要使用的 */
    VOS_UINT16 bec;
    VOS_UINT16 bed1;
    VOS_UINT16 bed2;
    VOS_UINT16 bc;
    VOS_UINT16 bd;
    VOS_UINT16 txSlotFormat; /* 调试模式:填入的是时隙格式 */
    VOS_UINT16 txChanNum;    /* 码道个数 */
} PHY_RF_CalWTxUpaPara;

/*
 * 结构说明: WCDMA RF慢速校准接口 -- DPA参数
 */
typedef struct {
    /* 下面的参数是发射DPA需要使用的 */
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
    VOS_INT16  afc;        /* AFC控制字 */
    VOS_UINT16 cfix  : 8;  /* 单DCXO c1 的控制字，取值范围为[0,255] */
    VOS_UINT16 c2fix : 8; /* 单DCXO C2 的控制字，取值范围为[0,7] */
} PHY_RF_CalAfcPara;

typedef union {
    VOS_INT16                     txPower10th;
    VOS_INT16                     txPowerAtten10th;
    PHY_RF_CalWTxRegCtrl          txRegCtrlPara;
} PHY_RF_CAL_W_TX_POWER_PARA_UNION;

/*
 * 结构说明: WCDMA RF快慢速校准接口Rx Agc Gain配置位域定义
 *           BIT[15] : 0:No Block， 1:Block;
 *           BIt[14] : 0:单载波，   1：双载波
 *           BIT[13] : 0:档位自动调整 1:档位强配
 *           BIT[2:0]: 0:第一档(增益最高的),依次类推
 */
typedef struct {
    VOS_UINT16 agcGainLvl : 3;
    VOS_UINT16 : 5;
    VOS_UINT16 : 5;
    VOS_UINT16 gainLvlMode : 1; /* 0 - 档位自动调整， 1- 强制档位 */
    VOS_UINT16 carrierMode : 1; /* 0 - 单载波， 1 - 双载波 */
    VOS_UINT16 blockMode : 1;   /* 0 - no block, 1 - block */
} W_RX_AgcGainCfg;

/*
 * 结构说明: WCDMA RF慢速校准接口
 */
typedef struct {
    VOS_UINT16 paVcc : 8;  /* PA Vcc 的电压控制字 */
    VOS_UINT16 paBias : 8; /* PA Bias电压控制字，MMMB PA时该数据有效，普通PA时WDSP直接丢弃该字段 */
} W_TX_PaParaCfg;

typedef struct {
    VOS_UINT16 pdDcOffset : 8; /* PD DCOFFSET的值,取值范围[0,31] */
    VOS_UINT16 pdVga : 8;      /* PD VGA的值,其中bit1表示衰减是否打开，Bit[6:4]表示VGA的取值 */
} W_TX_PdParaCfg;

typedef struct {
    VOS_UINT16 rficMode : 8; /* RFIC的工作模式，0:正常模式；1:Voice模式 */ /* W_TX_RficMode_UINT8 */
    VOS_UINT16 txPaMode : 8; /* 0:自动控制;1:高增益;2:中增益;3:低增益 */
} W_TX_RficPaMode;

/*lint -save -e959*/
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* 原语类型 */
    VOS_UINT16                  rsv;   /* 保留位   */

    VOS_UINT32 mask; /* 掩码位，根据bit位表示下面的字段是否有效，bit位的映射关系参见MASK_W_TRX_CFG_* */

    /* Afc Para Cfg */
    PHY_RF_CalAfcPara afcPara; /* AFC控制字 */

    /* Tx Para Cfg */
    VOS_UINT16 txBand;    /* W模BAND:1表示BAND1,2表示BAND2,依次类推 */
    VOS_UINT16 txChannel; /* W模TX频点号,如BAND1的9750 */
    /* 低8Bit定义 ---Tx发射 0:不使能,1:使能高8Bit定义 ---调制方式 0:正常调制,1:CW单音信号, 2:双音信号 */
    VOS_UINT16 txEnable;
    W_TX_RficPaMode                  txRficPaMode;
    /*
     * 功率控制模式，和unTxPowerPara使用同样的掩码。
     * 0：功率模式，此时unTxPowerPara.usTxPower10th有效；
     * 1：功率衰减模式，此时unTxPowerPara.usTxPowerAtten10th有效；
     * 2：寄存器控制方式，此时unTxPowerPara.stTxRegCtrlPara有效；
     */
    W_TX_PowerCtrlModeUint16         txPowerCtrlMode;
    /* 功率控制参数，和ucTxPowerCtrlMode使用同样的掩码 注意:该变量只用于RF6362，RF6363功率控制用unHi6363TxPowerPara */
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;

    /* Pa Volt Para Cfg */
    W_TX_PaParaCfg paParaCfg;
    /*
     * 自校准项目，其中高8bit：
     *  0 - 正常模式，或者从自校准模式回到正常模式
     *  1 - W IIP2自校准调试模式，此时工具保证下发的Tx channel = Rx Channel
     *  2 - W IQ Mismatch自校准模式，此时工具保证下发的Rx Channel = Tx Channel
     *  3 - G IQ Mismatch自校准模式，GSM模式下本来就只有1个信道号，此时请DSP自己选择
     *      Tx Band和Tx channel
     *      其中Tx Band含义： 0-GSM850,1-GSM900，2-DCS1800，3-PCS1900
     *  4 - W ET LUT自校准
     *  5 - W DPD LUT自校准
     * 其中低8bit含义：
     * W IIP2校准模式下，
     * 0 - 主集+分集
     * 1 - 主集
     * W/G IQ Mismatch校准模式下，
     * 0 - feedback
     * 1 - 直接发送到天线口
     *
     * 注意：在自校准模式下，其他掩码也应该能正常工作
     * 注意：如果从自校准模式回退到正常模式，DSP需要把RFIC和BBP配置回正常模式
     */
    VOS_UINT16 selfCal;

    /* Rx Para Cfg */
    VOS_UINT16 rxBand;         /* W模BAND:1表示BAND1,2表示BAND2,依次类推 */
    VOS_UINT16 rxChannel;      /* W模RX频点号,如BAND1的10700 */
    /*
     * 异频测量的Band。如果用户使能该字段*，则AGC表配置为usRxBand对应的AGC表，
     * 工作的Band和频点配置为usRxCrossBand和usRxCrossChannel
     */
    VOS_UINT16 rxCrossBand;
    VOS_UINT16 rxCrossChannel; /* 异频测量的频点 */

    VOS_UINT16 rxEnable;       /* 0:不使能,1:使能 */
    VOS_UINT16 rxAntSel;       /* 1:主集天线;2:分集天线 */
    /*
     * BIT[15] : 0:No Block， 1:Block;
     * BIt[14] : 0:单载波，   1：双载波
     * BIT[13] : 0:档位自动调整 1:档位强配
     * BIT[2:0]: 0:第一档(增益最高的),依次类推
     */
    W_RX_AgcGainCfg        rxAgcGain;
    VOS_UINT16             mrxGain;   /* MRX Gain档位 */

    /* 下面的参数是发射UPA需要使用的--为了不影响现有的AT命令,仅在该结构体后面增加相关变量 */
    VOS_UINT16                    upaEnable;  /* LMT发射UPA信号打开关闭.0:关闭;1:打开 */
    VOS_UINT16                    txCarrMode; /* TX载波模式.0:单载波;1:双载波 */
    PHY_RF_CalWTxUpaPara          primCarr;   /* 主载波相关的参数 */
    PHY_RF_CalWTxUpaPara          secondCarr; /* 辅载波相关的参数 */

    VOS_UINT16                    dpaEnable; /* LMT发射DPA信号打开关闭.0:关闭;1:打开 */
    VOS_UINT16                    rsv4;
    PHY_RF_CalWTxDpaPara          dpaPara; /* DPA相关的参数 */

    VOS_INT16 ip2IValue; /* IP2 I通道码字 */
    VOS_INT16 ip2QValue; /* IP2 Q通道码字 */

    VOS_UINT16 evmIndOnOff;      /* EVM数据上报是否使能，1表示打开上报，0表示关闭上报 */
    VOS_UINT16 enableReportData; /* EVM原始数据是否上报，1表示上报，0表示不上报 */
    /* ET delay 由 MASK_W_TRX_CFG_TX_ET_TIMEDELAY 控制 */
    VOS_INT16 signalDelay;  /* 信号通道只有整数延时寄存器配置  */
    VOS_INT16 envelopDelay; /* 包络通道延时,单位为1/1024sample */

    /* 激活哪一张dpd表，范围0~9 ，由 MASK_W_TRX_CFG_TX_DPD 控制 */
    VOS_INT16 dpdTableIndex;

    /*
     * IQ幅度配置，，由 MASK_W_TRX_CFG_TX_V_IQ 控制
     * 如果用户配置了需要遍历IQ，且使能 MASK_W_TRX_CFG_PD_ONOFF
     * 则DSP会遍历Viq，并找到增益压缩点，通过PD上报原语上报增益压缩点时的PD检测值
     * PD上报原语参见PHY_RF_CalWRfTxPdInd
     */
    VOS_INT16 viq;
    VOS_INT16 viqStep;

    /* 由MASK_W_TRX_CFG_TX_ET_COMP控制 */
    VOS_INT16  etPowerCompression; /* ET校准时的增益压缩点，单位0.1dB */
    VOS_UINT16 enableEtDpdComp;   /* BIT0:表示ET补偿使能，BIT1表示DPD补偿使能 */
    VOS_UINT16 rsv5;
    VOS_INT16  etFreqTempComp;  /* ET频补温补，单位0.1dB */
    VOS_INT16  dpdFreqTempComp; /* DPD频补温补，单位0.1dB */

#if (FEATURE_ON == FEATURE_HI6363)
    VOS_UINT16                              dpdStepV;
    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION hi6363TxPowerPara; /* RF6363 功率控制 */
#endif

    PHY_CAL_MipidevUnit cltHvdacMipi[PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM];
} PHY_RF_CalWTrxParaCfgReq;
/*lint -restore*/

/*
 * 结构说明: W PD参数配置原语
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* 原语类型 */
    VOS_UINT16                  rsv;   /* 保留位 */

    VOS_UINT16                  mask; /* refer to MASK_CAL_W_PD_* */

    /* PD Cfg */
    VOS_UINT16                  pdInputThrough; /* 0 - 关闭PD的输入信号， 1 - 打开PD的输入信号 */
    W_TX_PdParaCfg              txPdPara;       /* PD VGA参数 PD DC的参数 */
} PHY_RF_CalWPdParaCfgReq;

/*
 * 结构说明: W TRX 快慢速校准通用CNF
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件 ID */
    VOS_UINT16                  recMsgID;                /* 对应的下发Msg ID */
    VOS_UINT16                  errorCode;
} PHY_RF_CalWRfMsgCnf;

/*
 * 结构说明: W DCXO校准通用CNF
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;
    VOS_UINT16                  recMsgID; /* 对应的下发Msg ID */
    VOS_UINT16                  errorCode;
} PHY_RF_CalWRfDcxoCalMsgCnf;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件 ID */
    VOS_INT16                   pdHkadc;                  /*  Hi6362时，PdHkac为2字节 */
    VOS_INT16                   rsv;
    VOS_UINT32                  mrxCorr; /* Hi6363时，上报值为Mrx corr(4字节) */
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
    VOS_UINT16                      errBand; /* 存放校准失败的频段 */
    VOS_UINT16                      rsv;
    VOS_UINT16                      bandCnt; /* 存放DSP频段个数 */
    ANYBAND_W_DpdLutCalData         calData[SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCommDataDpdLutRpt;

/*
 * 结构说明: ET上报数据
 */
typedef struct {
    VOS_UINT32 bitMap; /* 校准MASK位 */
    VOS_UINT32 len;    /* 记录校准结构体长度,byte为单位 */
    VOS_INT16  pdHkadc;
    VOS_UINT16 compressSingleToneV;
    VOS_UINT32 compressMrxCorr; /* MRX电路检测的相关值 */
    VOS_UINT32 mrxCorr[64];     /* 64阶MRX值 */
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
    VOS_UINT16                       errBand;     /* 存放校准失败的频段 */
    VOS_UINT16                       selfCalMask; /* 存放DSP自校准项 */
    VOS_UINT16                       bandCnt;     /* 存放DSP频段个数 */
    ANYBAND_W_EtDelayCalData         calData[SELF_CAL_BAND_ID_MAX_COUNT];
    VOS_UINT32                       warningCode; /* 新增上报信息，装备不使用 */
} PHY_RF_CalCommDataEtDelayRpt;

/*
 * 结构说明: 通用数据上报结构头
 */
typedef struct {
    PHY_CAL_MsgDead        msgHead;
    VOS_UINT32             testItemsCnt; /* 内容指示，若包括某个内容,置上相应bit */

    /*
     * 后面跟上报的内容，包括各内容指示，各内容长度
     * VOS_UINT32 ulTest1BitMap; 第一组数据内容指示，若包括某个内容,置上相应bit
     * VOS_UINT32 ulTest1Len; 第一组数据byte个数，从ulTest1BitMap(包括)开始到ulTest2BitMap(不包括)结束
     * VOS_UINT16 usTest1Data[n]; 第一组数据 VOS_UINT32 ulTest2BitMap;
     * 第二组数据内容指示，若包括某个内容,置上相应bit VOS_UINT32 ulTest2Len;
     * 第二组数据byte个数，从ulTest2BitMap(包括)开始
     * VOS_UINT16 usTest2Data[n]; 第二组数据
     * .......... 后面类似
     */
} PHY_CAL_RfCommDataRptMsgHead;

/*
 * G模自校准配置原语
 */
typedef struct {
    VOS_UINT32 testItemsCnt;                                 /* 校准项个数 0-31 */
    /* 每个索引下只表示一个校准项，可能有重复的校准项，物理层按下发顺序全部上报 */
    VOS_UINT32 testItemsBitmap[COMM_SELF_CAL_TESTITEM_NUM];

    VOS_UINT32 bandMask;    /* 根据Bit位判断是否支持哪些Band，其中根据Msg ID判断是W band还是G Band */
    VOS_UINT32 bandExtMask; /* 扩展用，目前预留 */
} GPHY_RF_CalCommSelfCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16          msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    rsv;

    GPHY_RF_CalCommSelfCalReqData data;
} GPHY_RF_CalCommSelfCalReq;

/*
 * 结构说明: MRX DC 需要上报的直流码字
 */
typedef struct {
    VOS_INT16 dci;       /* 校准码子DCI */
    VOS_INT16 dcq;       /* 校准码子DCQ */
    VOS_INT16 reMainDci; /* 校准残余DCI */
    VOS_INT16 reMainDcq; /* 校准残余DCQ */
} GPHY_RF_MrxDcoffsetIq;

/*
 * G模自校准上报原语
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
    VOS_UINT16                           errBand; /* 存放校准失败的频段 */
    VOS_UINT16                           bandCnt; /* 存放DSP频段个数 */
    VOS_UINT16                           rsv;
    ANYBAND_G_MrxDcoffsetCalData         calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataMrxDcoffsetRpt;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;
    VOS_UINT16                  measNum;
    VOS_UINT16                  interval;
} PHY_RF_CalWRxRssiMeasReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件 ID */
    VOS_INT16                   rssi;
    VOS_UINT16                  agcState;
} PHY_RF_CalWRxRssiMeasInd;

/*
 * 结构说明: WCDMA RF Self Cal接口
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;
    VOS_UINT16                  selfCalMask; /* reference to MASK_SELF_CAL_* */
    VOS_UINT16                  modemIndex;  /* 0-modem0,1-modem1 */
    VOS_UINT32                  bandMask;    /* 根据Bit位判断是否支持哪些Band，其中根据Msg ID判断是W band还是G Band */
    VOS_UINT32                  bandExtMask; /* 扩展用，目前预留 */
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
    ANY_CARRIER_DcOffset carrier[2]; /* 其中0表示SC，1表示DC */
} ANY_ANT_AnyCarrierDcOffset;

typedef struct {
    ANY_ANT_AnyCarrierDcOffset ant[2]; /* 其中0表示AT1,1表示AT2 */
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
    ANY_CHAN_WRxIp2 chan[3]; /* 0：高信道，1：中信道，2：低信道 */
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
    ANY_CHAN_WRxImd2 chan[3]; /* 0：高信道，1：中信道，2：低信道 */
} RESULT_W_RxIp2;

typedef struct {
    VOS_INT16 dci;
    VOS_INT16 dcq;
    VOS_INT16 remainDci;
    VOS_INT16 remainDcq;
} NV_RX_DcoffsetIq;

typedef struct {
    NV_RX_DcoffsetIq vga[2][16]; /* 对应16个VGA的Dc值 */
} NV_W_RxDcoffsetIq;

/*
 * 结构说明: 0主集SC，1分集SC，2主集DC，3分集DC
 */
typedef struct {
    NV_RX_DcoffsetIq vga[4][16]; /* 对应16个VGA的Dc值 */
} NV_W_RxDcoffsetIqScdc;

typedef struct {
    VOS_UINT16                 bandId; /* 按MASK位上报 */
    VOS_UINT16                 rsv;
    NV_TX_IqMismatch           nvTxIqMismatch;
    RESULT_TX_IqMismatch       resultTxIqMismatch;
    NV_W_RxDcoffset            nvRxDcOffset;
    RESULT_W_RX_DCOFFSET_STRU  resultRxDcOffset;
    NV_W_RxIp2                 nvRxIp2;
    RESULT_W_RxIp2             resultRxIp2;
} ANY_BAND_WgSelfCalInd;

typedef struct {
    VOS_UINT16                 bandId; /* 按MASK位上报 */
    VOS_UINT16                 rsv;
    NV_TX_IqMismatch           nvTxIqMismatch;  /* TX IQ Mismatch NV项 */
    NV_TX_IqMismatch           nvMrxIqMismatch; /* MRX IQ Mismatch NV项 */
    RESULT_TX_IqMismatch       resultTxIqMismatch;
} ANY_BAND_GTxIqmismatchCalInd;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16                            errCode; /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                            errBand; /* 存放校准失败的频段 */
    VOS_UINT16                            bandCnt; /* 存放DSP频段个数 */
    VOS_UINT16                            rsv;
    ANY_BAND_GTxIqmismatchCalInd          calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataTxIqmismatchRpt;

typedef struct {
    PHY_TOOL_MsgIdUint16          msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    componentId;             /* 组件 ID */
    VOS_UINT16                    errCode;                 /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                    errBand;                 /* 存放校准失败的频段 */
    VOS_UINT16                    selfCalMask;             /* 存放DSP自校准项 */
    VOS_UINT16                    bandCnt;                 /* 存放DSP频段个数 */
    ANY_BAND_WgSelfCalInd         selfCalRlt[SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalWSelfCalInd;

typedef struct {
    PHY_TOOL_MsgIdUint16          msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    componentId; /* 组件 ID */

    VOS_UINT16                    errCode;     /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                    errBand;     /* 存放校准失败的频段 */
    VOS_UINT16                    selfCalMask; /* 存放DSP自校准项 */
    VOS_UINT16                    bandCnt;     /* 存放DSP频段个数 */
    ANY_BAND_WgSelfCalInd         selfCalRlt[CDMA_SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCSelfCalInd;

/*
 * Hi6363 W/C模自校准配置原语
 */
typedef struct {
    VOS_UINT32 testItemsCnt; /* 校准项个数，指示aulTestItemsBitmap中的有效个数，范围:大于0小于等于32 */
    /* 每个索引下只表示一个校准项，可能有重复的校准项，物理层按下发顺序全部上报，一个REQ只对应一次上报 */
    VOS_UINT32 testItemsBitmap[SELF_CAL_ITEMS_MAX_COUNT];
    VOS_UINT32 bandMask;    /* 根据Bit位判断是否支持哪些Band，其中根据Msg ID判断是W band还是G Band */
    VOS_UINT32 bandExtMask; /* 扩展用，目前预留 */
} PHY_RF_CalCommSelfCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16         msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   rsv;

    PHY_RF_CalCommSelfCalReqData data;
} PHY_RF_CalCommSelfCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16         msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   rsv0;

    VOS_UINT16 band;
    VOS_UINT16 rsv1;
    VOS_UINT32 txArfcn; /* W为100kHz为单位，C为kHz为单位 */
} PHY_RF_CalAsignedChanDpdSelfCalReq;

/*
 * Hi6363 W/C模自校准上报原语
 */
typedef struct {
    VOS_UINT32 bitMap; /* 指示校准类型(MASK)，用于工具解析 */
    VOS_UINT32 len;    /* 从usErrCode（含）到该校准项结果最后一个字节的byte个数 */

    VOS_UINT16 errCode; /* 校准失败后的error code,Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16 errBand; /* 存放校准失败的频段 */
    VOS_UINT16 bandCnt; /* 存放校准的频段个数 */
    VOS_UINT16 errFreq; /* 存放校准失败的频点 */
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
    NV_TX_IqMismatch       nvTxIqMismatch;  /* TX IQ Mismatch NV项 */
    NV_TX_IqMismatch       nvMrxIqMismatch; /* MRX IQ Mismatch NV项 */
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
    NV_W_RxIp2           nvIp2;     /* 校准结果 */
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
 * 结构说明: DPD LUT
 */
typedef struct {
    VOS_INT32 dpdCompI : 12; /* bit[0-11] : DPD补偿I路系数 */
    VOS_INT32 dpdCompQ : 12; /* bit[12-23]: DPD补偿Q路系数 */
    VOS_INT32 reserved : 8;  /* bit[24-31]: 保留 */
} NV_W_DpdCompPara;

typedef struct {
    VOS_UINT16              band;
    VOS_UINT16              rsv;
    VOS_UINT16              stepV[2];
    NV_W_DpdCompPara        dpdLut[2][128];
} ANYBAND_W_TxDpdLutData;

typedef struct {
    VOS_UINT16 etDelay;    /* Delay值 */
    VOS_INT16  etDelayAclr; /* Delay值对应的ACLR结果，单位0.125dBc */
} NV_W_TxEtDelayData;

typedef struct {
    /* 上报的个数，若为1表示不需要工具拟合搜索，表示PHY上报值即为校准结果 */
    VOS_UINT32                 lsRptCnt;
    VOS_UINT32                 lsFreq;          /* 绝对频率，W为100KHz,C为KHz */
    NV_W_TxEtDelayData         delayData[1]; /* PHY搜索delay的结果，真实个数为lsRptCnt */
} NV_W_TxEtDelayRpt;

typedef struct {
    VOS_UINT16 bandId;
    VOS_UINT16 freqNum;                        /* 校准的频点个数 */
    /* 找曲线最底点时，在上报值的最好点(ACLR最好)左右usSearchOffset范围按步长usSearchStep搜索 */
    VOS_UINT16 searchOffset;
    VOS_UINT16                searchStep;      /* 搜索步长 */
    NV_W_TxEtDelayRpt         etDelayData[1]; /* 校准或搜索结果上报，真实个数为usFreqNum */
} ANYBAND_W_TxEtDelayRsltRpt;

typedef struct {
    VOS_UINT16 etDelay;    /* Delay值 */
    VOS_INT16  etDelayAclr; /* Delay值对应的ACLR结果，单位0.125dBc */
} NV_CDMA_TxEtDelayData;

typedef struct {
    VOS_UINT32                    lsRptCnt; /* 上报的个数，若为1表示不需要工具拟合搜索，表示PHY上报值即为校准结果 */
    VOS_UINT32                    lsFreq;          /* 绝对频率，W为100KHz,C为KHz */
    NV_CDMA_TxEtDelayData         delayData[1]; /* PHY搜索delay的结果，真实个数为lsRptCnt */
} NV_CDMA_TxEtDelayRpt;

typedef struct {
    VOS_UINT16                   bandId;
    VOS_UINT16                   freqNum;         /* 校准的频点个数 */
    /* 找曲线最底点时，在上报值的最好点(ACLR最好)左右 usSearchOffset范围按步长usSearchStep搜索 */
    VOS_UINT16                   searchOffset;
    VOS_UINT16                   searchStep;      /* 搜索步长 */
    NV_CDMA_TxEtDelayRpt         etDelayData[1]; /* 校准或搜索结果上报，真实个数为usFreqNum */
} ANYBAND_CDMA_TxEtDelayRsltRpt;

/*
 * 结构说明: 表示RFIC和DBB不衰减的情况下，高中低UL4档增益的增益值(Hi6363使用)
 */
typedef struct {
    VOS_INT16 paGain[4]; /* 非VOICE模式下，表示RFIC和DBB不衰减的情况下，高中低3档增益的增益值，单位为0.1dB */
} UCOM_NV_WTxPaGainRpt;

typedef struct {
    VOS_UINT16                    bandId;
    VOS_UINT16                    reserved; /* 保留 */
    UCOM_NV_WTxPaGainRpt          etPaGain; /* 校准结果上报 */
} ANYBAND_W_TxEtPaGainRsltRpt;

/*
 * Hi6363 W/C模TRX快速校准配置原语
 */
typedef struct {
    /*
     * 功率控制模式，和unTxPowerPara使用同样的掩码。
     * 0：功率模式， 此时unTxPowerPara.usTxPower10th有效；
     * 1：功率衰减模式，此时unTxPowerPara.usTxPowerAtten10th有效；
     * 2：寄存器控制方式，此时unTxPowerPara.stTxRegCtrlPara有效；
     */
    VOS_UINT16 txPowerCtrlMode : 8;
    VOS_UINT16 txModMode : 8; /* 0：发射调制信号；1：发射单音信号； */
} W_TX_PowerCtrlMode;

typedef union {
    VOS_INT16             txEtDac;    /* ET PA VCC (mv)值 */
    W_TX_PaParaCfg        paParaCfg; /* APT PAVCC值 */
} PHY_RF_CAL_W_TX_PA_VCC_UNION;

typedef struct {
    VOS_UINT16 rficMode : 4; /* RFIC的工作模式，0:正常模式；1:Voice模式 */ /* W_TX_RficMode_UINT8 */
    VOS_UINT16 txPaMode : 4; /* 0:自动控制;1:高增益;2:中增益;3:低增益 */
    VOS_UINT16 mrxGain : 4;  /* MRX档位 */
    VOS_UINT16 dpdUsed : 4;  /* APT功率补偿校准，0:max PaVcc时候DPD配置为bypass，1:Pavcc码字变化时使能DPD模块 */
} W_TX_RficPaModeMrx;

/*
 * 结构说明: WCDMA RF快速校准单个Step的参数
 */
typedef struct {
    W_TX_RficPaModeMrx                      txRficPaModeMrx;
    PHY_RF_CAL_W_TX_PA_VCC_UNION            etAptParaCfg; /* ET or APT参数 */
    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION txPowerPara;
} EVERY_TX_MrxStepPara;

/*
 * 结构说明: WCDMA RF快速校准单个Step的参数
 */
typedef struct {
    /*
     * BIt[15] : 0: No Block,   1:Block;
     * BIT[14] : 0:单载波，     1:双载波
     * BIT[13] : 0:档位自动调整 1:档位强配
     * BIT[2:0]: 0:第一档(增益最高的)，依次类推
     */
    W_RX_AgcGainCfg rxAgcGain;
    VOS_UINT16      rsv;
} EVERY_RX_StepPara;

typedef struct {
    VOS_UINT32 len;               /* Chan包的字节长度，但不包括ulLen本身的字节长度 */
    VOS_UINT16 band;              /* 频段 */
    VOS_INT16  etPowerCompression; /* ET 等压缩量，单位0.1dB */
    VOS_UINT32 txArfcn;           /* W为100KHz单位，C为KHz单位 */
    /*
     * 总的STEP个数，包括需要配置前端的个数及空置个数，表示aulTxStepPattern有效bit个数，
     * 其中若对应bit为0，表示该2ms不需要配置
     */
    VOS_UINT16 txStepPatternCnt;
    VOS_UINT16 stepCnt;           /* 表示需要配置前端的个数，astStepData的长度个数 */

    VOS_UINT8  data[4];
    /*
     * aucData[]数据区格式如下：
     * VOS_UINT32 aulTxStepPattern[ceil(usTxStepPatternCnt/32)]; 数组长度 = usTxStepPatternCnt/32向上取整
     * EVERY_TX_MrxStepPara astStepData[n]; n = usStepCnt，
     * 也等于aulRxStepPattern中bit位为TRUE的总和, FALSE则物理层不配置前端
     */
} W_TX_MrxFastChann;

typedef W_TX_MrxFastChann C_TX_MRX_FAST_CHANN_STRU;

typedef struct {
    VOS_UINT32 len;              /* Chan包的字节长度，但不包括ulLen本身的字节长度 */
    VOS_UINT16 band;             /* 频段 */
    VOS_UINT16 rsv;              /* 保留 */
    VOS_UINT32 rxArfcn;          /* W为100KHz单位，C为KHz单位 */
    /*
     * 总的STEP个数，包括需要配置前端的个数及空置个数，表示aulRxStepPattern有效bit个数，
     * 其中若对应bit为0，表示该2ms不需要配置
     */
    VOS_UINT16 rxStepPatternCnt;
    VOS_UINT16 stepCnt;          /* 表示需要配置前端的个数，astStepData的长度个数 */
    VOS_UINT8  data[4];
    /*
     * aucData[]数据区格式如下：
     * VOS_UINT32 aulRxStepPattern[ceil(usRxStepPatternCnt/32)]; 数组长度 = usRxStepPatternCnt/32向上取整
     * EVERY_RX_StepPara     astStepData[n]; n = usStepCnt，
     * 也等于aulRxStepPattern中bit位为TRUE的总和, FALSE则物理层不配置前端
     */
} W_RX_FastChann;

typedef W_RX_FastChann C_RX_FAST_CHANN_STRU;

typedef struct {
    /* BIT[1]: APT是否生效 BIT[0]: MRX检测是否使能*/ /* Reference to MASK_W_TRX_FAST_CAL_REQ_TXMASK_* */
    VOS_UINT16                         txMask;
    W_TX_PowerCtrlMode                 txPowerCtrlMode;


    HI6363_FAST_StepWidthUint16        txStepWidthUs;  /* 单位us */
    HI6363_FAST_StepWidthUint16        rxStepWidthUs; /* 单位us */

    VOS_UINT16 txChannCnt;
    VOS_UINT16 rxChannCnt;

    VOS_UINT8  data[W_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE]; /* 物理层最大支持16k的消息包，数据区使用16000个字节 */
    /* 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，消息中实际长度为有效帧数 */
    /*
     * 使用时按照以下顺序存放数据，若同时有TRX则需要确保每个频点的TRX时长对齐
     * W_TX_MrxFastChann        astTxMrxChan[usTxChannCnt];
     * W_RX_FastChann            astRxMrxChan[usRxChannCnt];
     */
} W_TRX_MrxFastCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    W_TRX_MrxFastCalReqData     data;
} PHY_RF_CalWTrxMrxFastCalReqHi6363;

/*
 * 结构说明: ET 快速校准结果
 */
typedef struct {
    VOS_UINT32 paVccMv; /* 修正后的实际PA Vcc 的电压(mv) */
    VOS_UINT32 mrx;     /* MRX值 */
} W_TX_EtLutFastCalRslt;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;
    VOS_UINT16                  rssiCnt; /* RSSI测量值的个数 */
    VOS_UINT16                  mrxGainCorrCnt;

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    /* Rssi At1(0.125dBm) + Rssi At2(0.125dBm) + MRX相关值(4字节)) */
    VOS_INT16 asReportData[W_TRX_MAX_REPORT_COUNT_HI6363];

    VOS_INT32 errorCode; /* 新增，用于物理层标记错误信息，装备不使用 */
} PHY_RF_CalWTrxFastCalIndHi6363;

/*
 * 结构说明: WCDMA RF快速校准单个Step的参数
 */
typedef struct {
    /*
     * BIt[15] : 0: No Block,   1:Block;
     * BIT[14] : 0:单载波，     1:双载波
     * BIT[13] : 0:档位自动调整 1:档位强配
     * BIT[2:0]: 0:第一档(增益最高的)，依次类推
     */
    W_RX_AgcGainCfg                  rxAgcGain;
    W_TX_RficPaMode                  txRficPaMode;
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;
    W_TX_PaParaCfg                   paParaCfg;
    W_TX_PdParaCfg                   txPdPara; /* PD VGA参数 PD DC的参数 */
} ANY_STEP_Para;

/*
 * 结构说明: WCDMA RF快速校准接口
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
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    /*
     * BIT[3]: PD DCOFFSET是否生效
     * BIT[2]: PD VGA是否生效
     * BIT[1]: APT是否生效
     * BIT[0]: PD检测是否使能*/ /* Reference to MASK_W_TRX_FAST_CAL_REQ_TXMASK_*
    */
    VOS_UINT16                      txMask;
    /*
     * 功率控制模式，和unTxPowerPara使用同样的掩码。
     * 0：功率模式， 此时unTxPowerPara.usTxPower10th有效；
     * 1：功率衰减模式，此时unTxPowerPara.usTxPowerAtten10th有效；
     * 2：寄存器控制方式，此时unTxPowerPara.stTxRegCtrlPara有效；
     */
    W_TX_PowerCtrlModeUint16        txPowerCtrlMode;

    W_FAST_StepWidthUint16          stepWidthMs; /* Reference to W_FAST_StepWidth */
    VOS_UINT16                      rsv2;

    VOS_UINT16 txFrameCnt; /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */
    VOS_UINT16 rxFrameCnt; /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    W_TRX_FastFrame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalWTrxFastCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;     /* 原语类型 */
    VOS_UINT16                  component; /* 器件类型 */
    VOS_UINT32                  mask;      /* 下行Band */

    VOS_UINT16 rxBand;    /* 下行Band */
    VOS_UINT16 rsv;       /* 保留 */
    VOS_UINT32 rxChannel; /* 下行频点 */

    /*
     * BIT[15] : 0:No Block， 1:Block;
     * BIt[14] : 0:单载波，   1：双载波
     * BIT[13] : 0:档位自动调整 1:档位强配
     * BIT[2:0]: 0:第一档(增益最高的),依次类推
     */
    W_RX_AgcGainCfg rxAgcGain;
    VOS_UINT16 res;

    VOS_UINT16 c2CalEnable;  /* C2是否校准 */
    VOS_UINT16 c2Fix;        /* C2值 */
    VOS_UINT16 c2LowerLimit; /* C2校准下限 */
    VOS_UINT16 c2UpperLimit; /* C2校准上限 */

    VOS_UINT16 c1CalEnable; /* C1是否校准 */
    VOS_UINT16 c1Fix;       /* C1值 */

    W_TX_RficPaMode           txRficPaMode;
    /*
     * 功率控制模式，和unTxPowerPara使用同样的掩码。
     * 0：功率模式，此时unTxPowerPara.usTxPower10th有效；
     * 1：功率衰减模式，此时unTxPowerPara.usTxPowerAtten10th有效；
     * 2：寄存器控制方式，此时unTxPowerPara.stTxRegCtrlPara有效；
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;     /* 功率控制参数，和ucTxPowerCtrlMode使用同样的掩码 */

    VOS_UINT16 resData[32];
} PHY_RF_CalWDcxoSelfCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;     /* 原语类型 */
    VOS_UINT16                  component; /* 器件类型 */
    VOS_UINT32                  mask;      /* 下行Band */

    VOS_UINT16 rxBand;    /* 下行Band */
    VOS_UINT16 rsv;       /* 保留 */
    VOS_UINT32 rxChannel; /* 下行频点 */

    /*
     * BIT[15] : 0:No Block， 1:Block;
     * BIt[14] : 0:单载波，   1：双载波
     * BIT[13] : 0:档位自动调整 1:档位强配
     * BIT[2:0]: 0:第一档(增益最高的),依次类推
     */
    W_RX_AgcGainCfg rxAgcGain;
    VOS_UINT16 a0A1CalEnable;         /* 0:不校准 1:校准 */

    VOS_UINT16 c2CalEnable;  /* C2是否校准 */
    VOS_UINT16 c2Fix;        /* C2值 */
    VOS_UINT16 c2LowerLimit; /* C2校准下限 */
    VOS_UINT16 c2UpperLimit; /* C2校准上限 */

    VOS_UINT16 c1CalEnable; /* C1是否校准 */
    VOS_UINT16 c1Fix;       /* C1值 */

    W_TX_RficPaMode           txRficPaMode;
    /*
     * 功率控制模式，和unTxPowerPara使用同样的掩码。
     * 0：功率模式，此时unTxPowerPara.usTxPower10th有效；
     * 1：功率衰减模式，此时unTxPowerPara.usTxPowerAtten10th有效；
     * 2：寄存器控制方式，此时unTxPowerPara.stTxRegCtrlPara有效；
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;
    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION txPowerPara; /* 功率控制参数，和ucTxPowerCtrlMode使用同样的掩码 */

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
    VOS_UINT16                  componentId; /* 组件 ID */
    VOS_UINT16                  num;         /* 上报的第几次数据,从1开始 */
    VOS_UINT16                  evm;
    VOS_INT32                   freq;
    VOS_INT32                   initialPhase;
    VOS_UINT32                  reportDataCount;
    RAW_IQ_Data                 iqData[W_RX_EVM_REPORT_DATA_COUNT];
} PHY_RF_CalWRfRxEvmInd;

/*
 * 结构说明: WCDMA ET快速校准单个Step的参数
 */
typedef struct {
    VOS_UINT16                       rsv;
    W_TX_RficPaMode                  txRficPaMode;      /* RF及PA档位参数 */
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;       /* 功率参数，包含三种模式 */
    PHY_RF_CAL_W_TX_PA_VCC_UNION     paParaCfg;         /* PA电压控制参数 */
    VOS_INT16                        etPowerCompression; /* ET校准时的增益压缩点，单位0.1dB */
} ET_STEP_Para;

/*
 * 结构说明: WCDMA ET快速的帧结构
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
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    /*
     * BIT[1]：增益压缩点是否生效，如果增益压缩点生效，则每个电压下DSP需要遍历Viq，
     * 完成后找出压缩点对应的Viq，并上报压缩点对应的MRX值
     * BIT[0]: PD检测是否使能，如果PD检测使能，则找到压缩点后，需要重发一次，读取对应的PD值并上报；
     */
    /* Reference to PHY_RF_CalWEtFastReq_* */
    VOS_UINT16 mask;
    /*
     * 功率控制模式，和unTxPowerPara使用同样的掩码。
     * 0：功率模式， 此时unTxPowerPara.usTxPower10th有效；
     * 1：功率衰减模式，此时unTxPowerPara.usTxPowerAtten10th有效；
     * 2：寄存器控制方式，此时unTxPowerPara.stTxRegCtrlPara有效；
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;

    VOS_UINT16 rsv1;         /* 预留 */
    VOS_UINT16 calAlgorithm; /* 0 - 等压缩模式，1 - 等增益模式 */
    VOS_UINT16 txFrameCnt;   /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */
    VOS_UINT16 rsv2;         /* 预留 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    ET_FAST_Frame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalWEtFastReq;

typedef PHY_RF_CalWSelfCalReq PHY_RF_CAL_W_DPD_LUT_SELF_CAL_REQ_STRU;

/*
 * 结构说明:DCXO单个样本频偏温度对应值
 */
typedef struct {
    VOS_INT32 temp;    /* 温度,Boston温度单位0.001度 */
    VOS_INT32 freqPpm; /* 上面温度下对应的相对频偏，Boston定点化系数2^31/10^6 */
} PHY_RF_DcxoSample;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  componentId;
    VOS_UINT32                  errorCode;
    VOS_UINT16                  c2Fix;
    VOS_UINT16                  c1Fix;
#if (FEATURE_ON == FEATURE_HI6363)
    VOS_INT32               initTemp6363; /* Boston温度单位0.001度 */
    VOS_INT32               initFreq6363;
    COFFICIENT_DATA    a0;
    COFFICIENT_DATA    a1;
    COFFICIENT_DATA    a2;
    COFFICIENT_DATA    a3;
    VOS_INT32               dcxoInitTemp6363;                     /* 曲线计算初始温度,Boston温度单位0.001度 */
    VOS_INT32               beforeTempUp6363;                     /* a0、a1校准前温度,Boston温度单位0.001度 */
    VOS_INT32               afterTempUp6363;                      /* a0、a1校准后温度,Boston温度单位0.001度 */
    VOS_UINT16              rsv0;                             /* 四字节对齐保留 */
    VOS_UINT16              sampleNum;                        /* 样本个数 */
    PHY_RF_DcxoSample       dcxoSample[DCXO_SAMPLE_MAX_NUM]; /* 样本 */
#else
    VOS_INT16            initTemp; /* Chicago等老版本温度单位0.1度 */
    VOS_INT16            initFreq;
    COFFICIENT_DATA a0;
    COFFICIENT_DATA a1;
    COFFICIENT_DATA a2;
    COFFICIENT_DATA a3;
    VOS_INT16            tmax;       /* XO最大工作温度,Chicago等老版本温度单位0.1度 */
    VOS_INT16            tmin;       /* XO最小工作温度,Chicago等老版本温度单位0.1度 */
    VOS_INT16            dcxoInitTemp; /* 曲线计算初始温度,Chicago等老版本温度单位0.1度 */
    VOS_INT16            beforeTempUp; /* a0、a1校准前温度,Chicago等老版本温度单位0.1度 */
    VOS_INT16            afterTempUp;  /* a0、a1校准后温度,Chicago等老版本温度单位0.1度 */
    VOS_UINT16           rsv[15];
#endif
} PHY_RF_CalWRfDcxoSelfcalInd;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件 ID */
    VOS_UINT16                  rssiCnt;                 /* RSSI测量值的个数 */
    VOS_UINT16                  powerDetectorCnt;

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    VOS_INT16 asReportData[W_TRX_MAX_REPORT_COUNT]; /* Rssi At1(0.125dBm)+Rssi At2(0.125dBm)+PD测量结果 */
} PHY_RF_CalWTrxFastCalInd;

typedef struct {
    VOS_UINT16 rficGainCtrl; /* RF Gain的控制寄存器值，有RF Gain和BB Gain组合而成 */
    VOS_INT16  dbbAtten10th;  /* Dbb的衰减值，取值范围[-70,+10] */
} PHY_RF_CalGTxRegCtrl;

typedef struct {
    VOS_UINT32 rficGainCtrl : 24; /* RF Gain的控制寄存器值，有RF Gain和BB Gain组合而成 */
    VOS_INT32  dbbAtten10th : 8;  /* Dbb的衰减值，取值范围[-70,+10] */
} PHY_RF_CalGTxRegCtrl6363;

typedef union {
    VOS_UINT16                    txAgc;           /* 对应Tx Vpa控制模式 */
    VOS_INT16                     txPower10th;      /* 对应功率模式 */
    VOS_INT16                     txPowerAtten10th; /* 功率衰减模式 */
    PHY_RF_CalGTxRegCtrl          txRegCtrlPara;   /* 寄存器控制模式 */
#if FEATURE_HI6363
    PHY_RF_CalGTxRegCtrl6363      st6363TxRegCtrlPara; /* 6363寄存器控制模式 */
#endif
} PHY_RF_CAL_G_TX_POWER_PARA_UNION;

/*
 * 结构说明: G TX GSM RF慢速校准接口
 */
typedef struct {
    VOS_UINT16 txModType : 2;       /* 发射调制方式:0表示GMSK调制;1表示8PSK调制  G_TX_ModulationType_UINT8 */
    VOS_UINT16 txIsCwTone : 6;      /* 0 - 调制信号， 1- 单音信号 */
    /*
     * 发射控制方式：G_TX_PowerCtrlModeUint16
     * 0：GMSK电压控制,此方式下usTxVpa要配置；
     * 1：功率控制,此方式下usTxPower10th要配置；
     * 2：功率衰减模式，此方式下sTxPowerAtten10th要配置；
     * 3：寄存器控制模式，此方式下stTxRegCtrlPara要配置
     */
    VOS_UINT16 txPowerCtrlMode : 8;
} G_TX_ModeTypePowerCtrlCfg;

typedef struct {
    VOS_UINT16                         txMode; /* 0： burst,1:Continue */
    G_TX_ModeTypePowerCtrlCfg          txModTypePowerCtrl;
    PHY_RF_CAL_G_TX_POWER_PARA_UNION   txPowerPara;

    VOS_UINT16 paVcc;        /* EDGE PA Vcc控制电压，目前不支持 */
    VOS_UINT16 paVramp;      /* EDGE PA Vramp控制电压，在调制方式为8PSK，非功率控制模式下有效 */
    VOS_UINT16 paGainIndex;  /* linea pa增益索引( 0:auto,1:ultra high,2:high,3:mid,4:low ) */
    VOS_UINT16 mrxGainIndex; /* Mrx档位索引 */
} G_RF_SingleSlotCfg;

/*
 * 结构说明: 如果需要发单音信号，HiMS工具界面禁用usDataPattern，usSlotCnt只可选择1和8
 *           astSlotCfg[0]的ucTxIsCwTone配置为1
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    VOS_UINT32 mask; /* Reference to MASK_CAL_RF_G_TX_* */

    /* Afc Para Cfg */
    PHY_RF_CalAfcPara           afcPara; /* AFC控制字 */

    VOS_UINT16 freqNum;     /* (Band << 12) | Arfcn */
    VOS_UINT16 txEnable;    /* 发送使能控制:0x5555-使能发送;0xAAAA-停止发送;TSC 0; */
    VOS_UINT16 dataPattern; /* 0：All 0；1：All 1；2：Random */
    /*
     * 配置时隙个数，
     * 1~4 - 不需指定具体使用哪些时隙，DSP自动配置1~4时隙发射
     * 8 - 8时隙全开，使用第一个astSlotCfg的参数来配置
     */
    VOS_UINT16 slotCnt;
    G_RF_SingleSlotCfg          slotCfg[G_TX_PARA_MAX_SLOT_CNT_PER_FRAME];   /* 每个时隙的配置情况 */
    /* MASK_CAL_RF_G_GET_CLT_HVDAC 这个标志有效的时候配置 */
    PHY_CAL_MipidevUnit         hvDacValue[PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM];
} PHY_RF_CalGTxParaCfgReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;
} PHY_RF_CalGMrxCorrReq;

typedef struct {
    VOS_UINT16 cnt; /* 根据PHY_RF_CalGTxParaCfgReq_usSlotCnt决定上报的个数 */
    VOS_UINT16 rsv;
    VOS_UINT32 mrxCorr[8];
} PHY_RF_CalGMrxCorrIndData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    PHY_RF_CalGMrxCorrIndData   data;
} PHY_RF_CalGMrxCorrInd;

/*
 * 结构说明: 反射系数
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;
    VOS_UINT32                  errorCode;
    VOS_INT16                   reflectReal; /* 反射系数实部 */
    VOS_INT16                   reflectImag; /* 反射系数虚部 */
} PHY_RF_CalCltReflectanceInd;

/*
 * 结构说明: path delay
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  componentId;
    VOS_UINT32                  errorCode;
    VOS_UINT16                  fwdPathDelay; /* 前向 path delay */
    VOS_UINT16                  revPathDelay; /* 反向 path delay */
} PHY_RF_CalCltPathDelayInd;

/*
 * 结构说明 : GSM RF自校准接口启动G模自校准的Req接口与W模相同
 */
typedef PHY_RF_CalWSelfCalReq PHY_RF_CAL_G_SELF_CAL_REQ_STRU;

typedef PHY_RF_CalWSelfCalInd PHY_RF_CAL_G_SELF_CAL_IND_STRU;

/*
 * 结构说明: G Rx Rssi测量参数
 */
typedef struct {
    VOS_UINT16 mask;        /* Reference to MASK_CAL_RF_G_RX_* */
    VOS_UINT16 freqNum;     /* (Band << 12) | Arfcn */
    VOS_UINT16 rxMode;      /* 0:burst接收; 1:连续接收; */
    VOS_UINT16 aGCMode;     /* Fast AGC,Slow AGC */
    VOS_UINT16 aGCGain;     /* AGC档位：0-第一档…… */
    VOS_UINT16 blockerFlag; /* 0 - no blocker, 1- blocker */
    VOS_UINT16 rxAntSel;    /* 1:主集天线;2:分集天线;3:主分集双天线 */
    VOS_UINT16 eos;         /* End of slot，1表示后面还有1个slot，0表示最后一个slot */
} PHY_RF_CalGRxParaCfgReqData;

/*
 * 结构说明: G Rx Rssi测量结构体头，慢速校准REQ消息修改为动态消息长度，支持2个时隙的动态调整
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv1;

    PHY_RF_CalGRxParaCfgReqData data[G_RX_MAX_CFG_REQ_DATA_COUNT];
} PHY_RF_CalGRxParaCfgReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv1;
    VOS_UINT16                  measNum;
    VOS_UINT16                  rsv2;
} PHY_RF_CalGRxRssiMeasReq;

/*
 * 结构说明:G Rx Rssi测量，慢速校准Rssi上报消息修改为动态消息长度，按slot上报
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件 ID */

    /*
     * 后面跟上报的内容，分slot上报慢速校准结果，长度可变
     * VOS_INT16  sRSSIMain;  slot1主集Rssi值
     * VOS_INT16  sRSSIDiv;   slot1分集Rssi值
     * VOS_INT16  sRSSIMain;  slot2主集Rssi值
     * VOS_INT16  sRSSIDiv;   slot2分集Rssi值
     * .......... 后面类似
     */
} PHY_RF_CalGRxRssiMeasInd;

/*
 * 结构说明:上报主分集测量值
 */
typedef struct {
    VOS_INT16 rSSIMain; /* 主集Rssi值 */
    VOS_INT16 rSSIDiv;  /* 分集Rssi值 */
} GPHY_RF_CalSlotRpt;

#if FEATURE_HI6363
/*
 * 结构说明: GSM RF快速校准帧结构
 */
typedef struct {
    PHY_RF_CAL_G_TX_POWER_PARA_UNION txPowerPara;
    VOS_UINT16                       paVcc;
    VOS_UINT16                       paVramp;     /* EDGE Pa控制电压 */
    VOS_UINT16                       paGainIndex; /* linea pa增益索引( 0:auto,1:ultra high,2:high,3:mid,4:low ) */
    VOS_UINT16                       rsv;
} G_RF_TxStep;
#else
/*
 * 结构说明: GSM RF快速校准帧结构
 */
typedef struct {
    PHY_RF_CAL_G_TX_POWER_PARA_UNION txPowerPara;
    VOS_UINT16 paVcc;
    VOS_UINT16 paVramp;     /* EDGE Pa控制电压 */
    VOS_UINT16 paGainIndex; /* linea pa增益索引( 0:auto,1:ultra high,2:high,3:mid,4:low ) */
} G_RF_TxStep;
#endif

/*
 * 结构说明: GSM RF快速校准接口
 */
typedef struct {
    VOS_UINT16  channel;
    VOS_UINT16  stepPattern; /* 按Bit位表示当前时隙是否工作 */

    G_RF_TxStep stepValue[G_TX_MAX_STEP_COUNT]; /* 当前最大只支持四时隙 */
} G_RF_TxSequence;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    VOS_UINT16 txMask; /* Bit[0]:APT是否生效 用MASK位MASK_G_TX_FAST_CAL_REQ_TXMASK_APT表示 */

    G_TX_ModeTypePowerCtrlCfg   txModTypePowerCtrl;

    VOS_UINT16 frameCnt;
    VOS_UINT16 rsv2; /* 后续扩展预留 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    G_RF_TxSequence txSequence[G_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalGTxFastCalReq;


typedef struct {
    VOS_UINT16 channel;

    // 按照Pattern确定工作的slot，bit0表示slot0工作，bit1表示slot1工作，依次类推，最多支持8slot
    VOS_UINT16 stepPattern;
} PHY_RF_CalGeTmrxFastCalFrame;

/*
 * 结构说明: Step参数变化较频繁，预留一定保留字段，默认填0
 */
typedef struct {
    G_RF_TxStep       txCfg;
    VOS_UINT16        mrxGain;
    VOS_UINT16        rsv[3];
} PHY_RF_CalGeTmrxFastCalStep;

/*
 * 结构说明:长度<8168
 */
typedef struct {
    /*
     * Bit[0]:APT是否生效
     * Bit[1]:Mrx是否生效
     * Bit[2]:
     * 用MASK位MASK_G_TX_FAST_CAL_REQ_TXMASK_APT表示
     */
    VOS_UINT16 txMask;

    G_TX_ModeTypePowerCtrlCfg txModTypePowerCtrl;

    VOS_UINT16 stepWidth8th; /* 每个Step占用时间, 取值范围[1,8], 单位0.125slot */
    VOS_UINT16 rsv;

    VOS_UINT16 frameCnt;
    VOS_UINT16 stepCnt;

    VOS_UINT16 data[5100];
    /*
     * 数据区格式如下:
     * // 频点和掩码配置
     * typedef struct
     * {
     *     VOS_UINT16                          usChannel;
     *     // 按照Pattern确定工作的slot，bit0表示slot0工作，bit1表示slot1工作，依次类推，最多支持8slot
     *     VOS_UINT16                              usStepPattern;
     * }[m]; // m < 400, 如果Mrx上报打开，则m < 250, 因为上报结果是按照8时隙上报8个结果实现的，
     * 由PC侧判断当前结果对应的Pattern是True还是False决定取舍。
     * // step 功控参数
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
 * 结构说明:消息总长度<8192，除去vos头 + usMsgID + usRsv，stData的长度<8168
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    PHY_RF_CalGeTmrxFastCalReqData data;
} PHY_RF_CalGeTmrxFastCalReq;

typedef struct {
    // 每帧8个时隙上报结果，譬如如果stepwidth为8，表示1step = 1slot，则每帧上报8个结果，最多上报250帧
    // 如果stepwidth = 1，则表示1step = 0.125slot，则每帧上报64个结果，最多上报32帧
    // 2000为最大长度，实际上报为变长
    VOS_UINT16 stepCnt;
    VOS_UINT32 mrxCorr[2000];
} PHY_RF_CalGeTmrxFastCalIndData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    PHY_RF_CalGeTmrxFastCalIndData data;
} PHY_RF_CalGeTmrxFastCalInd;

/*
 * 结构说明 : G TRX 快慢速校准通用CNF
 */
typedef PHY_RF_CalWRfMsgCnf PHY_RF_CAL_G_RF_MSG_CNF_STRU;
typedef PHY_RF_CalWRfMsgCnf PHY_RF_CAL_WG_RF_MSG_CNF_STRU;

/*
 * 结构说明: GSM RX每个slot的参数配置，Hi6363开始支持半时隙测量
 *           其中H1表示前半时隙，H2表示后半时隙
 *           如果usStepWidth为2，表示每个step占1个slot，则只有H1有效
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
 * 结构说明: G Rx
 */
typedef struct {
    VOS_UINT16        channel;
    /*
     * bit[3:0]  AGC Gain档位, 目前取值范围0~7
     * bit4      0表示noblocker, 1表示blocker
     * bit[7:5]  rsv
     * bit[11:8]  AGC Gain档位, 目前取值范围0~7, 半时隙测量时有效
     * bit12      0表示noblocker, 1表示blocker
     * bit[15:13] rsv
     */
    G_RF_RxSlot       agcGain[G_RX_MAX_STEP_COUNT];
} G_RF_RxSequence;

/*
 * 结构说明: 下行快速校准参数
 */
typedef struct {
    VOS_UINT16 stepWidth : 8;  /* step宽度, 取值范围[1, 2], 单位0.5slot */
    VOS_UINT16 testScheme : 2; /* 0x01 单主集模式，0x10 单分集模式，0x11 主分集并行模式 */
    VOS_UINT16 rsv : 6;
} PHY_RF_CalRxFastPara;

typedef struct {
    VOS_UINT16                   frameCnt;   /* 测量帧数 */
    PHY_RF_CalRxFastPara         rxFastPara; /* 下行快速校准参数 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    G_RF_RxSequence              rxSequence[G_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalGRxFastCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    PHY_RF_CalGRxFastCalReqData data;
} PHY_RF_CalGRxFastCalReq;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;

    VOS_UINT16 rssiNum; /* 测量个数，最大为G_RX_MAX_RSSI_CNT个,不包括同步帧 */
    VOS_UINT16 rsv2;

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    /*
     * 如果是1时隙测量，每帧上报7个点，最大G_RX_MAX_RSSI_CNT个数据
     * 如果是半时隙测量，每帧上报14个点，最大2*G_RX_MAX_RSSI_CNT个数据
     */
    VOS_INT16 asRssiValue[G_RX_MAX_RSSI_COUNT];
} PHY_RF_CalGRxFastCalInd;

/*
 * 结构说明: G Rx DCR
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
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
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件ID */

    VOS_UINT16 errCode; /* Error code */
    VOS_UINT16 errBand; /* 存放校准失败的频段 */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv2;

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
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
    VOS_UINT16 errBand; /* 存放校准失败的频段 */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv;

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    ANYBAND_G_RxDcoffsetCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataRxDcoffsetRpt;

typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;

    VOS_UINT16 rfSettingNum;      /* Rf Setting 个数，最多3个 */
    VOS_UINT16 rfSettingIndex[3]; /* Rf Setting 索引(对应10组Rf Setting值)，high gain的Setting索引在前 */
} ANYBAND_G_TxRfSettingCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16 errCode; /* Error code */
    VOS_UINT16 errBand; /* 存放校准失败的频段 */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv;

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    ANYBAND_G_TxRfSettingCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataTxRfSettingRpt;

/*
 * 结构说明: GSM上/下爬坡表NV项参数
 */
typedef struct {
    VOS_UINT16 rampUp[G_RAMP_COEF_NUM];   /* 上坡表 */
    VOS_UINT16 rampDown[G_RAMP_COEF_NUM]; /* 下坡表 */
} NV_GSM_RampCoeff;

typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;

    NV_GSM_RampCoeff rampCoeff[G_RAMP_PWR_LEVEL_NUM]; /* 7张爬坡系数表 */
} ANYBAND_G_TxRampCoeffCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len;

    VOS_UINT16 errCode; /* Error code */
    VOS_UINT16 errBand; /* 存放校准失败的频段 */

    VOS_UINT16 rstCnt;
    VOS_UINT16 rsv;

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    ANYBAND_G_TxRampCoeffCalData calData[SELF_CAL_BAND_ID_MAX_COUNT];
} GPHY_RF_CalCommDataTxRampCoeffRpt;

/*
 * 结构说明: IQ Mismatch主分集天线的校准结果
 */
typedef struct {
    VOS_INT16 amp;   /* 幅度值 */
    VOS_INT16 phase; /* 相位值 */
} NV_G_RxIqMismatch;

/*
 * 结构说明: IQ Mismatch校准过程数据
 */
typedef struct {
    VOS_INT32 iSquare;    /* E(|I|^2) */
    VOS_INT32 qSquare;    /* E(|Q|^2) */
    VOS_INT32 iQMultiply; /* E(IQ) */
} RUN_DATA_GRxIqMismatch;

/*
 * 结构说明: G Rx IqMismatch校准结果
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* 消息原语 */
    VOS_UINT16                  componentId; /* 组件 ID */

    VOS_UINT16 errCode; /* 失败码 */
    VOS_UINT16 rsv2;

    NV_G_RxIqMismatch       nvRxIqMismatch;                                     /* 校准结果 */
    RUN_DATA_GRxIqMismatch  runDataRxIqMismatch[GPHY_RX_IQ_MISMATCH_SLOT_NUM]; /* 中间数据 */
} PHY_RF_CalGRxIqMismatchInd;

/*
 * 结构说明: G Rx IqMismatch校准结果
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* 消息原语 */
    VOS_UINT16                  componentId; /* 组件 ID */

    VOS_UINT16 errCode; /* 失败码 */
    VOS_UINT16 rsv2;

    NV_G_RxIqMismatch       ant1CalResult;                              /* 主集天线的校准结果 */
    NV_G_RxIqMismatch       ant2CalResult;                              /* 分集天线的校准结果 */
    RUN_DATA_GRxIqMismatch  ant1RunData[GPHY_RX_IQ_MISMATCH_SLOT_NUM]; /* 中间数据 */
    RUN_DATA_GRxIqMismatch  ant2RunData[GPHY_RX_IQ_MISMATCH_SLOT_NUM]; /* 中间数据 */
} PHY_RF_CalGRxIqmismatch6363Ind;

/*
 * 结构说明: G Rx IqMismatch校准命令
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* 消息原语 */
    VOS_UINT16                  rsv1;

    VOS_UINT16 bandId;
    VOS_UINT16 rxChannel;

    VOS_INT16  refIor10th;       /* 参考信号强度，单位0.1dBm */
    VOS_INT16  ampCorrThreshold; /* 校准通过门限 */
    VOS_INT16  phsCorrTHreshold;
    VOS_UINT16 testScheme; /* 0x1 单主集模式，0x2 单分集模式，0x3 主分集并行模式 */
} PHY_RF_CalGRxIqMismatchReq;

/*
 * 结构说明: G Tx HD3/HD5校准命令
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* 消息原语 */
    VOS_UINT16                  rsv1;

    VOS_UINT16 bandMask; /* Bit0:GSM850; Bit1:GSM900; Bit2:DCS; Bit3:PCS; */
    VOS_UINT16 rsv2;
} PHY_RF_CalGTxHd3Hd5Req;

/*
 * 结构说明: 单个功率等级的HD3/HD5的校准过程数据
 */
typedef struct {
    VOS_INT32 hD3Amp;
    VOS_INT32 asHD3Corr[GSM_TX_HD35_STEP_NUM];
    VOS_INT32 hD5Amp;
    VOS_INT32 asHD5Corr[GSM_TX_HD35_STEP_NUM];
} ANY_PCL_GTxHd3Hd5RunData;

/*
 * 结构说明: 单个频点的HD3/HD5的校准过程数据
 */
typedef struct {
    /* 分别存放Max_PCL、Max_PCL+1、Max_PCL+2三个功率等级的HD3/HD5校准过程数据 */
    ANY_PCL_GTxHd3Hd5RunData pclHd3Hd5RunData[GSM_TX_HD35_PCL_NUM];
} ANY_CHANN_GTxHd3Hd5RunData;

/*
 * 结构说明: 单个功率等级的HD3/HD5校准结果
 */
typedef struct {
    VOS_INT16 br; /* HD3校准值实部 */
    VOS_INT16 bi; /* HD3校准值虚部 */
    VOS_INT16 cr; /* HD5校准值实部 */
    VOS_INT16 ci; /* HD5校准值虚部 */
} ANY_PCL_GTxHd3Hd5;

/*
 * 结构说明: 单个频点的HD3/HD5校准结果
 */
typedef struct {
    /* 分别存放Max_PCL、Max_PCL+1、Max_PCL+2三个功率等级的HD3/HD5校准结果 */
    ANY_PCL_GTxHd3Hd5 pclHd3Hd5[GSM_TX_HD35_PCL_NUM];
} ANY_CHANN_GTxHd3Hd5;

/*
 * 结构说明: 单个频段的HD3/HD5校准结果和过程数据
 */
typedef struct {
    VOS_UINT16                  band; /* Band信息 */
    VOS_UINT16                  rsv;
    ANY_CHANN_GTxHd3Hd5         channHd3Hd5[GSM_TX_HD35_ARFCN_NUM]; /* 分别存放低、中、高三个频点的校准结果 */
    /* 分别存放低、中、高三个频点的校准过程数据 */
    ANY_CHANN_GTxHd3Hd5RunData  channHd3Hd5RunData[GSM_TX_HD35_ARFCN_NUM];
} ANY_BAND_GTxHd3Hd5Ind;

/*
 * 结构说明: G PHY上报HD3/HD5结果
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* 消息原语 */
    VOS_UINT16                  componentId; /* 组件 ID */

    VOS_UINT16 errCode; /* 失败码 用于PHY定位问题 */
    VOS_UINT16 errBand; /* 校准失败的频段 如果多个校准失败默认上报第一个失败的band */

    VOS_UINT16 bandCnt; /* 校准频段的个数 */
    VOS_UINT16 rsv2;

    ANY_BAND_GTxHd3Hd5Ind bandHd3Hd5[GSM_TX_HD35_BAND_NUM]; /* 最大支持G 4个频段 */
} PHY_RF_CalGTxHd3Hd5Ind;

/*
 * 结构说明: 用于32K测量任务的启动和停止
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;
    VOS_UINT32                  action; /* 1-启动32K测量;0-停止32K测量 */
} WPHY_TOOL_GaugeReq;

/*
 * 结构说明: 用于32K测量任务的启动和停止的原语回复
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;
    VOS_UINT32                  action; /* 1-启动32K测量;0-停止32K测量 */
} WPHY_TOOL_GaugeCnf;

/*
 * 结构说明: 用于32K测量结果的上报
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId; /* 组件 ID */
    VOS_UINT32                  data[W_GAUGE_RESULT_SIZE];
} WPHY_TOOL_GaugeInd;

typedef struct {
    VOS_UINT32 regAdd;
    VOS_UINT32 regValue;
} WR_REG;

/*
 * 结构说明: TOOL通过DSP读写RFIC的寄存器，要求信令/非信令下均有效
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    VOS_UINT8  writeReadFlag; /* 0:Read，此时需要回复CNF和IND */
    VOS_UINT8  channelNo;     /* 需要读取RF对应的通道号:0:表示通道0,1:表示通道1 */
    VOS_UINT16 regCnt;        /* 1~1024 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrRficRegReq;

/*
 * 结构说明: TOOL通过DSP读写RFIC的寄存器，要求信令/非信令下均有效
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId;             /* 组件 ID */
    VOS_UINT16                  writeReadFlag;           /* 0:Read */
    VOS_UINT16                  regCnt;                  /* 1~1024 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrRficRegInd;

/*
 * 结构说明: TOOL通过BBP读写RFIC的寄存器，要求信令/非信令下均有效
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    VOS_UINT16 writeReadFlag; /* 0:Read，此时需要回复CNF和IND 1:Write，此时只需要回复CNF */
    VOS_UINT16 regCnt;        /* 1~1024 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrBbpRegReq;

/*
 * 结构说明: TOOL通过DSP读写BBP的寄存器，要求信令/非信令下均有效
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId; /* 组件 ID */

    VOS_UINT16 writeReadFlag; /* 0:Read */
    VOS_UINT16 regCnt;        /* 1~1024 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    WR_REG regData[WG_BBP_RF_REG_WR_MAX_COUNT];
} WPHY_RF_WrRficBbpInd;

/*
 * 结构说明: TOOL通过DSP读写RFIC的寄存器，要求信令/非信令下均有效
 */
typedef WPHY_RF_WrRficRegReq GPHY_RF_WR_RFIC_REG_REQ_STRU;

/*
 * 结构说明: TOOL通过DSP读写RFIC的寄存器，要求信令/非信令下均有效
 */
typedef WPHY_RF_WrRficRegInd GPHY_RF_WR_RFIC_REG_IND_STRU;

/*
 * 结构说明: TOOL通过BBP读写RFIC的寄存器，要求信令/非信令下均有效
 */
typedef WPHY_RF_WrBbpRegReq GPHY_RF_WR_BBP_REG_REQ_STRU;

/*
 * 结构说明: TOOL通过DSP读写BBP的寄存器，要求信令/非信令下均有效
 */
typedef WPHY_RF_WrRficBbpInd       GPHY_RF_WR_RFIC_BBP_IND_STRU;
typedef PHY_RF_CalWRxRssiMeasReq   PHY_RF_CAL_C_RX_RSSI_MEAS_REQ_STRU;
typedef PHY_RF_CalWRfMsgCnf        PHY_RF_CAL_C_RF_MSG_CNF_STRU;
typedef PHY_RF_CalWRxRssiMeasInd   PHY_RF_CAL_C_RX_RSSI_MEAS_IND_STRU;
typedef PHY_RF_CalWRfTxPdInd       PHY_RF_CAL_C_RF_TX_PD_IND_STRU;
typedef W_TX_PdParaCfg             C_TX_PD_PARA_CFG_STRU;
typedef PHY_RF_CalWTxRegCtrl       CSDR_CAL_RF_TX_REG_CTRL_STRU;
typedef W_TX_RficPaMode            CSDR_TX_RFIC_PA_MODE_STRU;
/* DCR调试 */
typedef NV_W_RxDcoffset            PHY_RF_CAL_C_RX_DCOFFSET_STRU;
typedef PHY_RF_CalWSelfCalReq      PHY_RF_CAL_C_SELF_CAL_REQ_STRU;

/* RFIC和BBP读写 */
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
    NV_RX_DcoffsetIq vga[2][16]; /* 对应16个VGA的Dc值 */
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
    ANY_CHAN_CRxIp2 chan[3]; /* 0：高信道，1：中信道，2：低信道 */
} NV_C_RxIp2;

typedef struct {
    VOS_INT16 imd2;
    VOS_INT16 rsv;
} C_RX_Imd2;

typedef C_RX_Imd2 CSDR_RX_IMD2_STRU; /* 存放IMD2值 */

typedef struct {
    C_RX_Imd2 divOn;
    C_RX_Imd2 divOff;
} ANY_CHAN_CRxImd2;

typedef struct {
    ANY_CHAN_CRxImd2 chan[3]; /* 0：高信道，1：中信道，2：低信道 */
} RESULT_C_RxIp2;

typedef struct {
    VOS_UINT16           bandId;
    VOS_UINT16           rsv;
    NV_C_RxIp2           nvIp2;     /* 校准结果 */
    RESULT_C_RxIp2       resultIp2; /* IMD2 */
} ANYBAND_C_RxIp2CalData;

typedef NV_W_RxDcoffset            NV_C_RX_DCOFFSET_STRU;
typedef RESULT_W_RX_DCOFFSET_STRU  RESULT_C_RX_DCOFFSET_STRU;

typedef struct {
    VOS_UINT16                 bandId; /* 按MASK位上报 */
    VOS_UINT16                 rsv;
    NV_TX_IqMismatch           nvTxIqMismatch;
    RESULT_TX_IqMismatch       resultTxIqMismatch;
    NV_C_RX_DCOFFSET_STRU      nvRxDcOffset;
    RESULT_C_RX_DCOFFSET_STRU  resultRxDcOffset;
    NV_C_RxIp2                 nvRxIp2;
    RESULT_C_RxIp2             resultRxIp2;
} ANY_BAND_CSelfCalInd;

/*
 * 结构说明: C模慢速校准消息接口参数
 */
typedef struct {
    VOS_UINT16 msgId; /* 原语类型 */
    VOS_UINT16 rsv1;  /* 保留位   */

    VOS_UINT32 mask; /* 复用W慢速校准接口定义 */

    /* Afc Para Cfg */
    PHY_RF_CalAfcPara afcPara; /* AFC控制字 */

    /* Tx Para Cfg */
    VOS_UINT16 txBand;
    VOS_UINT16 mrxGain;  /* MRX Gain档位 */
    VOS_UINT32 txFreq;   /* Freq, KHZ单位 */
    VOS_UINT16 txEnable; /* 低8Bit定义 ---Tx发射 0:不使能,1:使能 高8Bit定义 ---调制方式 0:正常调制,1:CW单音信号 */
    W_TX_RficPaMode           txRficPaMode;
    W_TX_PowerCtrlModeUint16  txPowerCtrlMode; /* 复用W慢速校准接口定义 */
    VOS_UINT16                       rsv2;
    /* 复用W慢速校准接口定义，注意:RF6362用，RF6363用变量unHi6363TxPowerPara */
    PHY_RF_CAL_W_TX_POWER_PARA_UNION txPowerPara;

    /* Pa Volt Para Cfg */
    W_TX_PaParaCfg        paParaCfg;
    VOS_UINT16            selfCal;

    /* Rx Para Cfg */
    VOS_UINT16 rxBand;
    VOS_UINT16 rsv3;
    VOS_UINT32 rxFreq; /* Freq, KHZ单位 */

    VOS_UINT16 rxEnable; /* 复用W慢速校准接口定义 */
    VOS_UINT16 rxAntSel; /* 复用W慢速校准接口定义 */

    W_RX_AgcGainCfg        rxAgcGain; /* 复用W慢速校准接口定义 */
    VOS_UINT16             rsv4;

    VOS_INT16 ip2IValue; /* IP2 I通道码字 */
    VOS_INT16 ip2QValue; /* IP2 Q通道码字 */

    PHY_RF_CAL_W_TX_POWER_PARA_HI6363_UNION hi6363TxPowerPara; /* RF6363功率控制结构体 */

    /* ET delay 由 MASK_W_TRX_CFG_TX_ET_TIMEDELAY 控制 */
    VOS_INT16 signalDelay;  /* 信号通道只有整数延时寄存器配置  */
    VOS_INT16 envelopDelay; /* 包络通道延时,单位为1/1024sample */

    /* 激活哪一张dpd表，范围0~9 ，由 MASK_W_TRX_CFG_TX_DPD 控制 */
    VOS_INT16 dpdTableIndex;

    /*
     * IQ幅度配置，，由 MASK_W_TRX_CFG_TX_V_IQ 控制
     * 如果用户配置了需要遍历IQ，且使能 MASK_W_TRX_CFG_PD_ONOFF
     * 则DSP会遍历Viq，并找到增益压缩点，通过PD上报原语上报增益压缩点时的PD检测值
     * PD上报原语参见PHY_RF_CalWRfTxPdInd
     */
    VOS_INT16 viq;
    VOS_INT16 viqStep;

    /* 由MASK_W_TRX_CFG_TX_ET_COMP控制 */
    VOS_INT16  etPowerCompression; /* ET校准时的增益压缩点，单位0.1dB */

    /* ET DPD温补频补 */
    VOS_UINT16 enableEtDpdComp; /* BIT0:表示ET补偿使能，BIT1表示DPD补偿使能 */
    VOS_UINT16 rsv6;
    VOS_INT16  etFreqTempComp;  /* ET频补温补，单位0.1dB */
    VOS_INT16  dpdFreqTempComp; /* DPD频补温补，单位0.1dB */

    PHY_CAL_MipidevUnit cltHvdacMipi[PHY_TRX_CLT_HVDAC_MIPI_UNIT_NUM];
} PHY_RF_CalCTrxParaCfgReq;

/*
 * 结构说明: C模快速校准消息接口FRAME参数
 */
typedef struct {
    VOS_UINT16         band;          /* 频段只是用于确定使用哪一套Bc的Nv项,校准频率以下面配置的Freq为准 */
    VOS_UINT16         stepCnt;       /* [1, 16] */
    VOS_UINT32         txFreq;        /* 单板上行Freq, KHz单位 */
    VOS_UINT32         rxFreq;        /* 单板下行Freq, KHz单位 */
    VOS_UINT16         txStepPattern; /* 复用W快速校准接口定义 */
    VOS_UINT16         rxStepPattern; /* 复用W快速校准接口定义 */
    ANY_STEP_Para      stepData[W_TRX_MAX_STEP_COUNT]; /* 复用W快速校准接口定义 */
} C_TRX_FastFrame;

/*
 * 结构说明: C模快速校准消息接口参数
 */
typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;
    VOS_UINT16                  rsv;

    VOS_UINT16                  txMask;          /* 复用W快速校准接口定义 */
    W_TX_PowerCtrlModeUint16    txPowerCtrlMode; /* 复用W快速校准接口定义 */
    /* 和装备约定仅支持2、4、8个pcg，对应的stepwidth是2500,5000,10000 */
    C_FAST_StepWidthUint16      stepWidthMs;
    VOS_UINT16 rsv2;

    VOS_UINT16 txFrameCnt; /* 复用W快速校准接口定义 */
    VOS_UINT16 rxFrameCnt; /* 复用W快速校准接口定义 */

    C_TRX_FastFrame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalCTrxFastCalReq;

/*
 * 结构说明: 6363上,C模快速校准消息接口参数
 */
typedef struct {
    /*
     * BIT[1]: APT是否生效
     * BIT[0]: MRX检测是否使能
     */
    /* Reference to MASK_W_TRX_FAST_CAL_REQ_TXMASK_* */
    VOS_UINT16                         txMask;
    W_TX_PowerCtrlMode                 txPowerCtrlMode;
    /* 单位us,TRX同时存在的时候，需要TX和RX长度保持一致，如果只有TX的时候，以TX的长度为主 */
    HI6363_FAST_StepWidthUint16        txStepWidthUs;
    VOS_UINT16                         rsv; /* 保留  */

    VOS_UINT16 txChannCnt;
    VOS_UINT16 rxChannCnt;

    VOS_UINT8 data[C_HI6363_TRX_FAST_CAL_REQ_MAX_SIZE]; /* 物理层最大支持8k的消息包，数据区使用8000个字节 */
    /* 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，消息中实际长度为有效帧数 */
    /*
     * 使用时按照以下顺序存放数据，若同时有TRX则需要确保每个频点的TRX时长对齐
     * C_TX_MRX_FAST_CHANN_STRU        astTxMrxChan[usTxChannCnt];
     * C_RX_FAST_CHANN_STRU            astRxMrxChan[usRxChannCnt];
     */
} C_TRX_MrxFastCalReqData;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
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
 * 结构说明: 在非信令BT时，发送消息通知GPHY切换UE模式。用于全网通特性。
 *           0:modem1工作在tcxo1 1:modem1工作在tcxo0。
 *           消息需要在LOAD PHY之后配置。
 */
typedef struct {
    VOS_MSG_HEADER                            /* 模拟协议栈的消息，添加VOS头 */
        PHY_TOOL_MsgIdUint16        msgId;  /* 原语类型 */
    VOS_UINT16                      toolId; /* 保留位 */
    VOS_UINT16                      ueMode; /* 0:modem1工作在tcxo1 1:modem1工作在tcxo0 */
    VOS_UINT16                      resvered;
} MPH_SWITCH_M1XoInd;

/*
 * 结构说明: 在非信令BT时，发送消息通知WPHY切换UE模式。用于全网通特性。
 *           0:w各频段工作在主天线 1:W B8工作在副天线.
 */
typedef struct {
    VOS_MSG_HEADER                            /* 模拟协议栈的消息，添加VOS头 */
        PHY_TOOL_MsgIdUint16        msgId;  /* 原语类型 */
    VOS_UINT16                      toolId; /* 保留位 */
    VOS_UINT16                      ueMode; /* 0:w各频段工作在主天线 1:W B8工作在副天线 */
    VOS_UINT16                      resvered;
} RRC_PHY_SwitchMoantInd;

/*
 * 结构说明: CDMA ET快速的帧结构
 */
typedef struct {
    VOS_UINT16        band;     /* 下发band枚举值 */
    VOS_UINT16        stepCnt;  /* [1, 16] */
    VOS_UINT32        uTxFreqKhz; /* liyong, w:频点，C是频率Khz */
    VOS_UINT16        txStepPattern;
    VOS_UINT16        rsv;
    ET_STEP_Para      stepData[W_TRX_MAX_STEP_COUNT];
} CDMA_ET_FastFrame;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  rsv;

    /*
     * BIT[1]：增益压缩点是否生效，如果增益压缩点生效，
     * 则每个电压下DSP需要遍历Viq，完成后找出压缩点对应的Viq，
     * 并上报压缩点对应的MRX值
     * BIT[0]: PD检测是否使能，如果PD检测使能，则找到压缩点后，
     * 需要重发一次，读取对应的PD值并上报；
     */
    VOS_UINT16 mask;

    /* Reference to PHY_RF_CalWEtFastReq_* */
    /*
     * 功率控制模式，和unTxPowerPara使用同样的掩码。
     * 0：功率模式， 此时unTxPowerPara.usTxPower10th有效；
     * 1：功率衰减模式，此时unTxPowerPara.usTxPowerAtten10th有效；
     * 2：寄存器控制方式，此时unTxPowerPara.stTxRegCtrlPara有效；
     */
    W_TX_PowerCtrlModeUint16 txPowerCtrlMode;

    VOS_UINT16 rsv1;         /* 预留 */
    VOS_UINT16 calAlgorithm; /* 0 - 等压缩模式，1 - 等增益模式 */
    VOS_UINT16 txFrameCnt;   /* [1, W_TRX_MAX_SWEEP_FRAME_CNT] */
    VOS_UINT16 rsv2;         /* 预留 */

    /*
     * 接口文档中按照最大长度定义，方便部件直接使用该结构申请内存，
     * 消息中实际长度为有效帧数
     */
    CDMA_ET_FastFrame sweepFrame[W_TRX_MAX_SWEEP_FRAME_COUNT];
} PHY_RF_CalCsdrEtFastReq6362;

/*
 * 结构说明: ET 快速校准结果
 */
typedef struct {
    VOS_UINT32 paVccMv; /* 修正后的实际PA Vcc 的电压(mv) */
    VOS_UINT32 mrx;     /* MRX值 */
} C_TX_EtLutFastCalRslt;

typedef struct {
    VOS_UINT16                      band;
    VOS_UINT16                      rsv;
    PHY_CAL_ComplexDataInt16        dpdLut[128];
} ANYBAND_CDMA_DpdLutCalData;

/*
 * 结构说明: DPD LUT校准结果
 */
typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len; /* 记录校准结构体长度,byte为单位 */

    VOS_UINT16                         errCode; /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16                         errBand; /* 存放校准失败的频段 */
    VOS_UINT16                         rsv;
    VOS_UINT16                         bandCnt; /* 存放DSP频段个数 */
    ANYBAND_CDMA_DpdLutCalData         calData[CDMA_SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCdmaDpdLutRpt;

/*
 * 结构说明: ET上报数据  for chicago
 */
typedef struct {
    VOS_UINT32 bitMap; /* 校准MASK位 */
    VOS_UINT32 len;    /* 记录校准结构体长度,byte为单位 */
    VOS_INT16  pdHkadc;
    VOS_UINT16 compressSingleToneV;
    VOS_UINT32 compressMrxCorr; /* MRX电路检测的相关值 */
    VOS_UINT32 mrxCorr[64];     /* 64阶MRX值 */
} PHY_RF_CalCdmaEtLutRpt;

typedef struct {
    VOS_UINT16 bandId;
    VOS_UINT16 rsv;
    VOS_INT16  asEtDelayFreqComp[8]; /* delay校准结果上报 */
} ANYBAND_CDMA_EtDelayCalData;

typedef struct {
    VOS_UINT32 bitMap;
    VOS_UINT32 len; /* 记录实际校准结果长度,byte为单位 */

    VOS_UINT16 errCode;     /* Reference to RF_CAL_SELF_CAL_* */
    VOS_UINT16 errBand;     /* 存放校准失败的频段 */
    VOS_UINT16 selfCalMask; /* 存放DSP自校准项 */
    VOS_UINT16 bandCnt;     /* 存放DSP频段个数 */

    ANYBAND_CDMA_EtDelayCalData calData[CDMA_SELF_CAL_BAND_ID_MAX_COUNT];
} PHY_RF_CalCdmaEtDelayRpt;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId; /* 组件 ID */
    VOS_INT32                   errorCode;   /* 错误码 */

    VOS_INT16 reflectReal;  /* 反射系数实部 */
    VOS_INT16 reflectImage; /* 反射系数虚部 */
} PHY_RF_CalCCltReflectanceInd;

typedef struct {
    PHY_TOOL_MsgIdUint16        msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  componentId; /* 组件 ID */
    VOS_INT32                   errorCode;   /* 错误码 */

    VOS_UINT16 fwdPathDelay; /* 前向delay */
    VOS_UINT16 revPathDelay; /* 反向delay */
} PHY_RF_CalCCltPathDelayInd;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of PhyRfCalInterface.h */

