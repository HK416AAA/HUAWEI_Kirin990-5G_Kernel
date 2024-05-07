

/*
 * 1 其他头文件包含
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
 * 2 宏定义
 */

/*
 * 2.2 宏定义,W/G宏定义
 */

/* 打印携带参数个数 */
#define UPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM      5
#define UPHY_TOOL_INTER_REG_MAX_NUM           300

/* 可维可测的func type为4,intra msg类型需要找TOOL确认 */
#define UPHY_TOOL_MODEM2_FUNC_TYPE_OM_MSG     0x84
#define UPHY_TOOL_MODEM2_FUNC_TYPE_INTRA_MSG  0x94

#define UPHY_TOOL_MODEM1_FUNC_TYPE_OM_MSG     0x44
#define UPHY_TOOL_MODEM1_FUNC_TYPE_INTRA_MSG  0x54

#define UPHY_TOOL_MODEM0_FUNC_TYPE_OM_MSG     0x04
#define UPHY_TOOL_MODEM0_FUNC_TYPE_INTRA_MSG  0x1A

/* Log打印MIPI回读显示最大数 */
#define UPHY_TOOL_RDBACK_MIPI_NUM             2

/* 大小必须保持和phy_oam_interface.h中UPHY_OAM_SLEEP_INFO_NUM一致 */
#define UPHY_TOOL_SLEEP_INFO_NUM              16

/* 闭环调谐逼近轨迹记录最大值 */
#define UPHY_TOOL_CLT_TRACE_NUM               8

/*
 * 2.2 宏定义,GPHY宏定义
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

/* PS数传时下行最大的时隙个数 */
#define GPHY_PS_MAX_RX_TSN_NUM                5

/* PS数传时上行最大的时隙个数 */
#define GPHY_PS_MAX_TX_TSN_NUM                4

#define GPHY_TOOL_SLAVE_BSIC_MAX_NUM          8

/*
 * 原语GPHY_TOOL_FuncBitCtrlReq 中ulFuncBitMask每个bit位的含义;
 * 如果对应bit位置1:则对应功能失效;
 * 如果对应bit位置0:则恢复对应功能;
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

/* 可维可测数据块最大上报个数 */
#define GPHY_TOOL_RPT_BLK_MAX_NUM             10

/* 打印携带参数个数 */
#define GPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM      5   /* 错误上报时，界定的上报变量个数 */

/*
 * 2.3 宏定义,WPHY宏定义
 */

#define WPHY_TOOL_PA_SWITCH_COMP_SLOT         4   /* W pa切换补偿4个slot */
/* 打印携带参数个数 */
#define WPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM      5   /* 错误上报时，界定的上报变量个数 */

/* 激活小区最大个数 */
#define WPHY_MNTN_MAX_ACT_CELL_NUM            7   /* 激活集小区最大数目 */

/* fs2最大天线个数 */
#define WPHY_MNTN_FS2_MAX_ANT_NUM             4   /* fs2最大天线个数 */

/* 可维可测数据块最大上报个数 */
#define WPHY_TOOL_RPT_BLK_MAX_NUM             10

#define WPHY_MNTN_TRCH_NUM                    8

#define WPHY_MNTN_SLAVE_REPORTFREQ_NUM        6   /* 从模可谓可测上报频点个数 */
#define GPHY_TOOL_MA_FREQ_NUM                 64  /* 跳频序列的个数 */

#define GPHY_TOOL_BA_LIST_MAX_NUM             32  /* GSM BA LIST最大的个数 */

#define GPHY_TOOL_AMR_MAX_ACS_NUM             4   /* 最大激活集个数 */

#define GPHY_TOOL_RPT_MAX_LEN                 100

#define GPHY_TOOL_AMR_CODEC_MAX_NUM           8    /* AMR最大编码个数 */

/* 物理层每帧最大时隙个数 */
#define GPHY_TOOL_TN_NUM_IN_FRAME             8

#define GPHY_TOOL_MAX_UL_SLOT_NUM             4    /* Gsm上行最大的时隙个数 */

#define GPHY_TOOL_MAX_IDLE_FRAME_NUM          84   /* 专用态5S内空闲帧的最大数目 */

#define GPHY_TOOL_MAX_SYNC_TASK_NUM           5    /* 专用态空闲帧能做的最大同步任务个数 */

#define GPHY_TOOL_MAX_NCELL_NUM               32   /* 上报的邻区异系统最大数目 */

#define GPHY_TOOL_NCELL_RPT_PREDICT_TASK_INFO_NUM 5 /* 专用态打印的预测任务的个数 */

#define GPHY_TOOL_GBBPSHARE_MAX_MSG_PROC_NUM  4    /* bbp资源复用最大挂接原语的个数 */

/* G回读的内存地址最大规格 */
#define GPHY_MNTN_READ_ADDR_MAX_NUM           300

#define WPHY_MNTN_HSUPA_SUB_FRM_NUM           5    /* HSUPA子帧数 */

#define WPHY_MNTN_HSUPA_OM_CARERIER_NUM       1    /* UPA 可维可测使用，包括主副载波 */

#define WPHY_MNTN_HSDPA_OM_CARERIER_NUM       2

#define WPHY_MNTN_HSUPA_RPT_VALUE_MAX_NUM     100

#define WPHY_MNTN_RL_SCRAM_MAX_NUM            7

#define WPHY_MNTN_DEMOD_FINGER_MAX_NUM        29

#define WPHY_MNTN_EQ_FINGER_RPT_MAX_NUM       2

#define WPHY_MNTN_BLER_RPT_MAX_NUM            5

#define WPHY_MNTN_DPA_ERROR_CODE_RPT_MAX_NUM  2

#define WPHY_MNTN_MP_WIN_MAX_NUM              3

/* BCCH队列最大任务个数 */
#define WPHY_MNTN_BCCH_QUEUE_LENGTH           4

/* ademod队列最大任务个数 */
#define WPHY_MNTN_ADMOD_QUEUE_LENGTH          5

/* 跟踪均衡模板的帧数 */
#define WPHY_MNTN_CPC_DRX_PATTERN_FRAME_NUM   5

/* CPC DRX均衡控制的类型数 */
#define WPHY_MNTN_CPC_DRX_EQUA_CTRL_TYPE_NUM  2

/* CQI PATTERN个数 */
#define WPHY_MNTN_CPC_DRX_CQI_PATTERN_NUM     3

/* CPC DRX RF 控制类型数 */
#define WPHY_MNTN_CPC_DRX_RF_CTRL_TYPE_NUM    6

#define WPHY_MNTN_SLAVE_CS_SUCC_CELL_NUM      6

/* 最大IC小区个数 */
#define WPHY_MNTN_IC_MAX_CELL_NUM             2

/* DPA的P值数目+1，直接使用P值当下标 */
#define WPHY_MNTN_P_VALUE_NUM                 16

/* 各子帧上报RAM偏移 */
#define WPHY_MNTN_SUB_FRAME_ADDR_OFFSET       0x30

/* 子帧数 */
#define WPHY_MNTN_SUB_FRAME_NUM               5

#define WPHY_MNTN_MAX_CNT                     0xFFFFFFFF

#define WPHY_MNTN_MAX_UINT16_VALUE            0xFFFF

#define WPHY_MNTN_MAX_CARRIER_NUM             2

/* HSDSCH按照区间统计，最大分10个区间 */
#define WPHY_MNTN_MAX_REGION_NUM              10

/* W Rx FAST CAL 最大次数:SC/DC BLK/NBLK 最大8档，每档测试2个功率能量 */
#define WPHY_CAL_RX_FAST_CAL_MAX_VAILD_NUM    64

/* W Tx FAST CAL 最大次数 */
#define WPHY_CAL_TX_FAST_CAL_MAX_VAILD_NUM    450

/* HSUPA可维可测上报中激活集小区最大数量 */
#define WPHY_MNTN_HSUPA_ACTIVE_SET_MAX_NUM    6

/* WBBP 支持的最大天线个数 */
#define WPHY_MNTN_WBBP_MAX_ANT_NUM            2

#define WPHY_MNTN_INVALID_NUM                 0x3

/* 无效值 */
#define WPHY_MNTN_INVALID_VALUE               (-1)

/* WPHY每个帧中的时隙数 */
#define WPHY_MNTN_SLOT_NUM_BY_FRAME           15

/* 主小区Hsdpa SCCH的信道数量 */
#define WPHY_MAIN_CELL_HSDPA_SCCH_MAX_NUM     4

/* 十六位整数最大值，定义为非法值 */
#define WPHY_MNTN_UINT16_INVALID_NUM          0xFFFF

/* 寄存器默认边界值保护 */
#define WPHY_MNTN_REG_INVALID                 0x5A5A

/* HsScch No的无效值 */
#define WPHY_MNTN_INVALID_HSSCCH_NO           0xFF

/* ovsf的无效值 */
#define WPHY_MNTN_INVALID_OVSF                0xFF

/* 上报天线口RSSI参数值 */
#define WPHY_MNTN_RSSI_PARA                   0x400

/* 发射功率无效值 */
#define WPHY_MNTN_POWER_VAL_INVALID           0xFF

/* 天线1接收功率无效值 */
#define WPHY_MNTN_POWER_ANT1_VAL_INVALID      0xFF

/* 天线2接收功率无效值 */
#define WPHY_MNTN_POWER_ANT2_VAL_INVALID      0xFF

/* Hs Pdsch信道时间偏移无效值 */
#define WPHY_MNTN_DL_DPCCH_OFFSET_INVALID     0xFF

/* BBP_DBG 用 */
#define UPHY_TOOL_BBP_DBG_SAMPLE_REG_LIST_NUM 3
#define UPHY_TOOL_BBP_DBG_OTHER_REG_LIST_NUM  2

#define UPHY_TOOL_BBP_DBG_SAMPLE_REG_NUM      50

#define UPHY_TOOL_EASYRF_DBG_SAMPLE_REG_NUM   60
/*
 * 采数数据模块个数. 0:bbic dfe; 1:rfic0 data采数;2:rfic0 bus采数 3:rfic0 mrx采数
 * 4:rfic1 data采数;5:rfic1 bus采数 6:rfic1 mrx采数
 */
#define UPHY_TOOL_EASYRF_DBG_MODULE_NUM       7
/*
 * 采数每个模块对应列表个数 :0:配置列表;1:更新列表;2:使能列表;3:上报列表;
 * 4:查询前停止列表 5:停止前查询列表 6:查询后停止列表
 */
#define UPHY_TOOL_EASYRF_DBG_REG_LIST_NUM     7
/* EASYRF模块维测点个数 */
#define EASYRF_OM_MAX_LOG_POINTS              32

/* EASYRF寄存器回读个数 */
#define EASYRF_OM_MAX_REG_READ_NUM            8

#define UPHY_TOOL_MAX_SSI_READ_ALL_NUM        256 /* 全回读支持256个寄存器 */
#define UPHY_TOOL_MAX_SSI_READ_BACK_NUM       64  /* 最多支持64个寄存器的回读 */
#define UPHY_TOOL_MAX_MIPI_READ_BACK_NUM      64  /* 最多支持64个寄存器的回读 */

/* 回读的内存地址最大规格 */
#define WPHY_MNTN_READ_ADDR_MAX_NUM           250

#define GPHY_TOOL_NCELL_TIMER_INVALID_VALUE   0xFFFFFFFF /* 邻区c1、c2定时器用到的无效值 */

#define GPHY_TOOL_GSDR_SB_BURST_DATA_LEN      224

/* 快速校准维测回读AGC寄存器个数 */
#define WPHY_TOOL_FAST_CAL_READ_AGC_REG_NUM   2

/* 快速校准维测回读主集或分级LNA寄存器个数 */
#define WPHY_TOOL_FAST_CAL_READ_LNA_REG_NUM   4

/* 快速校准维测回读DCR次数 */
#define WPHY_TOOL_FAST_CAL_READ_DCR_TIMES     4

/* IQI 上报ID运算 */
#define WPHY_MNTN_CIQ_MAKE_ID(a, b, c, d) \
    (((VOS_UINT32)(a) << 24) | ((VOS_UINT32)(b) << 16) | ((VOS_UINT32)(c) << 8) | ((VOS_UINT32)(d)))

/* 最多支持的MODEM数目 */
#define RCM_TOOL_MNTN_MODEM_SUPPORT_NUM       (RCM_TOOL_MNTN_MODEM_ID_BUTT)

/* EASYRF配置维测信息环形队列长度 */
#define WPHY_TOOL_AEASYRF_MNTN_TASK_NUM       10

/* EASYRF配置缓存队列最大个数 */
#define WPHY_TOOL_RF_EASYRF_TASK_LIST_MAX_NUM 10

#define WPHY_TOOL_WBBP_MAX_ANT_NUM            2

/* EASYRF单次配置最大任务个数,根据任务配置组合任务最大并发任务个数确定 */
#define WPHY_TOOL_AEASYRF_CONFIG_TASK_MAX_NUM 7

/* WPHY采数维测32bit采数最大值 */
#define WPHY_TOOL_REPORT_SELF_CAL_SAMPLE_DATA_MAX_SIZE 4096

/* WPHY随机接入功率实网数据统计最大档位 */
#define WPHY_TOOL_MNTN_ACCESS_PWR_MAX_LEVEL   7

/* b5010 msgid有效位 */
#define WPHY_TOOL_MSGIDOFFSET                 0xFFF

/* easyrf the max num of mipi read */
#define EASYRF_OM_MAX_RD_MIPI_COUNT           5

/*
 * 3 枚举定义
 */
/*
 * 枚举说明: CLT DETECT FAIL原因
 */
enum WPHY_TOOL_CltDetectFailType {
    WPHY_TOOL_CLT_DETECT_FAIL_BBP_INVALID     = 0,
    WPHY_TOOL_CLT_DETECT_FAIL_NV_THLD_FAIL    = 1,
    WPHY_TOOL_CLT_DETECT_FAIL_PATH_DELAY_FAIL = 2,
    WPHY_TOOL_CLT_DETECT_FAIL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_CltDetectFailTypeUint16;

/*
 * 枚举说明: CLT状态
 */
enum WPHY_TOOL_CltStatusType {
    WPHY_TOOL_CLT_STATUS_CROSSMIPI = 0,
    WPHY_TOOL_CLT_STATUS_OL        = 1,
    WPHY_TOOL_CLT_STATUS_CL        = 2,
    WPHY_TOOL_CLT_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_CltStatusTypeUint16;

/*
 * 枚举说明: 接下来要做的邻区任务, bsic 搜索/bsic 验证/读邻区系统消息/异系统测量
 * 注意: 和GPHY_NCE_PREDICT_TASK_TYPE_ENUM保持一致，同步修改!!!
 */
enum GPHY_NCE_PredictTaskTypeElf {
    GPHY_E_NCE_PREDICT_TASK_INTERRAT_MEAS = 0, /* 接下来要做异系统测量 */
    GPHY_E_NCE_PREDICT_TASK_BCCH_READ     = 1, /* 接下来要做邻区系统消息读取 */
    GPHY_E_NCE_PREDICT_TASK_BSIC_VERIFY   = 2, /* 接下来要做邻区bsic check */
    GPHY_E_NCE_PREDICT_TASK_PHY_BSIC_SRCH = 3, /* 接下来要做物理层调度的邻区bsic srch */
    GPHY_E_NCE_PREDICT_TASK_PS_BSIC_SRCH  = 4, /* 接下来要做协议栈下发的邻区bsic srch */
    GPHY_E_NCE_PREDICT_TASK_BUTT
};
typedef VOS_UINT16 GPHY_NCE_PredictTaskTypeElfUint16;

/*
 * 枚举说明: 当前哪个模式在使用(可维可测使用)
 * 注意: 和GPHY_NCE_OCCASION_USE_MODE_ENUM保持一致，同步修改!!!
 */
enum GPHY_NCE_OccasionUseModeElf {
    GPHY_E_NCE_OCCASION_USE_GSM   = 0, /* GSM使用当前的沟 */
    GPHY_E_NCE_OCCASION_USE_WCDMA = 1, /* WCDMA使用当前的沟 */
    GPHY_E_NCE_OCCASION_USE_LTE   = 2, /* LTE使用当前的沟 */
    GPHY_E_NCE_OCCASION_USE_TDS   = 3, /* TD使用当前的沟 */
    GPHY_E_NCE_OCCASION_USE_BUTT
};
typedef VOS_UINT16 GPHY_NCE_OccasionUseModeElfUint16;

/*
 * 枚举说明: 分沟时中断类型(可维可测使用)
 */
enum GPHY_GAP_IntType {
    GPHY_GAP_INT0 = 0, /* 分沟时收到int0 */
    GPHY_GAP_INT1 = 1, /* 分沟时收到int1 */
    GPHY_GAP_INT2 = 2, /* 分沟时收到int2 */

    GPHY_GAP_BUTT
};
typedef VOS_UINT16 GPHY_GAP_IntTypeUint16;

/*
 * 枚举说明: 邻区同步标志
 * 注意: 和GPHY_NCE_NCELL_SYNC_ACTION_ENUM保持一致，同步修改!!!
 */
enum GPHY_NCE_NcellSyncActionElf {
    GPHY_E_NCE_NCELL_SYNC_BSIC_SEARCH = 0, /* 0:BSIC搜索 */
    GPHY_E_NCE_NCELL_SYNC_BSIC_VERIFY,     /* 1:BSIC验证 */
    GPHY_E_NCE_NCELL_SYNC_ACTION_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NcellSyncActionElfUint16;

/*
 * 枚举说明: BSIC搜索模式
 * 注意: 和GPHY_NCE_NCELL_BSIC_SEARCH_STATE_ENUM保持一致，同步修改!!!
 */
enum GPHY_NCE_NcellBsicSearchStateElf {
    GPHY_E_NCE_NCELL_BSIC_SEARCH_START = 0,         /* 0:BSIC搜索启动 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_SUCC,              /* 1:BSIC搜索成功 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_FAIL,              /* 2:BSIC搜索失败 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_ABNORMAL,          /* 3:BSIC搜索异常 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_OVER_HIGH_THD,     /* 4:BSIC搜索能量超过最大门限 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_NOT_IN_NCELL_LIST, /* 5:BSIC搜索过程中跳出邻区列表 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_FORCIBLY_STOP,     /* 6:BSIC搜索过程中强停bsic搜索任务 */
    GPHY_E_NCE_NCELL_BSIC_SEARCH_STATE_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NcellBsicSearchStateElfUint16;

/*
 * 枚举说明:
 * 注意: 和GPHY_NCE_NSTRONG_NCELL_ENUM_UINT16保持一致，同步修改!!!
 */
enum GPHY_NCE_NstrongNcellElf {
    GPHY_E_NCE_6_STRONG_NCELL      = 0, /* 0-属于 0- 5强小区 */
    GPHY_E_NCE_6TO10_STRONG_NCELL  = 1, /* 1-属于 6-10强小区 */
    GPHY_E_NCE_11TO15_STRONG_NCELL = 2, /* 2-属于11-15强小区 */
    GPHY_E_NCE_16TO31_STRONG_NCELL = 3, /* 3-属于16-31强小区 */
    GPHY_E_NCE_STRONG_NCELL_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NstrongNcellElfUint16;

/*
 * 枚举说明: BSIC验证模式
 * 注意: 和GPHY_NCE_NCELL_BSIC_VERIFY_STATE_ENUM保持一致，同步修改!!!
 */
enum GPHY_NCE_NcellBsicVerifyStateElf {
    GPHY_E_NCE_NCELL_BSIC_VERIFY_START = 0, /* 0:BSIC验证启动 */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_SUCC,      /* 1:BSIC验证成功 */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_FAIL,      /* 2:BSIC验证失败 */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_ABNORMAL,  /* 3:BSIC验证异常 */
    GPHY_E_NCE_NCELL_BSIC_VERIFY_STATE_BUTT
};
typedef VOS_UINT16 GPHY_NCE_NcellBsicVerifyStateElfUint16;

/*
 * 3.1 枚举定义,GPHY枚举定义
 */

/*
 * 3 枚举定义
 */
/*
 * UPHY回复CNF时,携带的处理结果枚举
 */
enum UPHY_TOOL_RtcDebug {
    UPHY_MNTN_RTC_DEBUG_CLOSE = 0,
    UPHY_MNTN_RTC_DEBUG_OPEN  = 1,

    UPHY_MNTN_RTC_DEBUG_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_RtcDebugUint16;

/*
 * UPHY<-->TOOL原语列表
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

    /* diag 寄存器结构配置及消息上报 */
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
    /* arm bba 消息ID 从0xFFC0--0xFFFF */

    ID_UPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnMsgIdUint16;

/*
 * EASYRF<-->TOOL原语列表
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
 * UPHY回复CNF时,携带的处理结果枚举
 */
enum UPHY_TOOL_MntnRslt {
    UPHY_MNTN_RSLT_SUCC = 0,
    UPHY_MNTN_RSLT_FAIL = 1,

    UPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnRsltUint16;

/*
 * 枚举说明: UPHY错误上报级别
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
 * RCM 枚举定义
 */

/*
 * 枚举说明: 记录Modem#0~#2的TAS Modem状态,维测上报
 *           含义同RCM_TAS_MODEM_STATE_ENUM_UINT16
 */
enum RCM_TOOL_MntnModemState {
    RCM_TOOL_MNTN_MODEM_STATE_FILGHT         = 0,  /* S0: 飞行模式 */
    RCM_TOOL_MNTN_MODEM_STATE_IDLE_HAPPY     = 1,  /* S1: 空闲HAPPY状态 */
    RCM_TOOL_MNTN_MODEM_STATE_PSONLY_GOOD    = 2,  /* S2: PS ONLY业务好讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_ACCESS_GOOD    = 3,  /* S3: 接入好讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_CS_BACK_GOOD   = 4,  /* S4: CS背景通话好讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_CS_ACTIVE_GOOD = 5,  /* S5: CS通话好讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_SEARCHING      = 6,  /* S6: 搜索状态 */
    RCM_TOOL_MNTN_MODEM_STATE_IDLE_UNHAPPY   = 7,  /* S7: 空闲UNHAPPY状态 */
    RCM_TOOL_MNTN_MODEM_STATE_PSONLY_POOR    = 8,  /* S8: PS ONLY业务弱讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_ACCESS_POOR    = 9,  /* S9: 接入弱讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_CS_BACK_POOR   = 10, /* S10: CS背景通话弱讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_CS_ACTIVE_POOR = 11, /* S11: CS通话弱讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_IDLE_GOOD      = 12, /* S12: 空闲态号讯号状态 */
    RCM_TOOL_MNTN_MODEM_STATE_LAB_MODE       = 13, /* S13: 实验室模式状态，包括环回模式和有线无线模式 */
    RCM_TOOL_MNTN_MODEM_STATE_TEST_MODE      = 14, /* S14: 测试模式状态，包括测试卡模式和AT算法模式 */
    RCM_TOOL_MNTN_MODEM_STATE_LOCK_ANT       = 15, /* S15: 锁天线模式 */
    RCM_TOOL_MNTN_MODEM_STATE_TAS_DISABLE    = 16, /* S16: tas不使能模式 */
    RCM_TOOL_MNTN_MODEM_STATE_BUTT
};
typedef VOS_UINT16 RCM_TOOL_MntnModemStateUint16;

/*
 * 枚举说明: 记录裁决后的内部TAS切换行动信息，维测上报
 *           含义同RCM_TAS_ACTION_ENUM_UINT16
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
 * 枚举说明: tool维测消息中专用的主集分集枚举定义,用于数组下标等应用;
 *          含义同RCM_MODEM_ID_ENUM_UINT16
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
 * GPHY<-->TOOL原语列表
 */
enum GPHY_TOOL_MntnMsgId {
    /* 下发REQ消息因为带VOS消息，上报的IND或CNF消息不带VOS消息头，应对REQ另外解析，这里指示为了ASN生成方便 */
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

    /* TAS状态上报请求 */
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

    /* 干扰特性 消息上报 */
    ID_GPHY_TOOL_INTERFE_TX_BLANKING_IND = 0xE04D,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G配置GTC把别一个modem的TX关闭 */
    ID_GPHY_TOOL_INTERFE_RX_BLANKING_IND = 0xE04E,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G配置GTC把别一个modem的RX恢复 */
    ID_GPHY_TOOL_INTERFE_STOP_G_TX_IND = 0xE04F,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G关闭自己的TX发送 */

    ID_GPHY_TOOL_POWER_CTRL_SET_CNF = 0xE050, /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */
    ID_GPHY_TOOL_CHANNEL_REPORT_IND = 0xE051, /* _H2ASN_MsgChoice  GPHY_TOOL_ChannelReportInd  */

    ID_GPHY_TOOL_MASTER_STOP_SLAVE_MEAS_IND = 0xE052,
    /* _H2ASN_MsgChoice  GPHY_TOOL_MasterStopSlaveMeasInd  */ /* 上报强停从模信息 */

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

    /* GmeasW GBGW分沟信息上报 */
    ID_GPHY_TOOL_GMEASW_SCHEDULE_INT7_IND = 0xE06B, /* _H2ASN_MsgChoice GPHY_GMEASW_SCHEDULE_INFO_STRU */
    ID_GPHY_TOOL_GBGW_SCHEDULE_INT7_IND   = 0xE06C, /* _H2ASN_MsgChoice GPHY_BGW_SCHEDULE_INFO_STRU */

    ID_GPHY_TOOL_MASTER_WAKE_INFO_IND = 0xE06D, /* _H2ASN_MsgChoice UPHY_GDRX_MASTER_MNTN_STRU */
    ID_GPHY_TOOL_SLAVE_WAKE_INFO_IND  = 0xE06E, /* _H2ASN_MsgChoice UPHY_GDRX_SLAVE_MNTN_STRU */
    ID_GPHY_TOOL_DCOFFSET_UPDATE_IND  = 0xE06F, /* _H2ASN_MsgChoice GPHY_TOOL_DcOffsetReport  */

    /* 发送给GSDR的消息 */
    ID_GPHY_TOOL_GSDR_SRCH_INIT_REQ = 0xE070,
    ID_GPHY_TOOL_GSDR_SRCH_PARA_REQ = 0xE071,
    ID_GPHY_TOOL_GSDR_SB_DEC_REQ    = 0xE072,

    /* G从模测量信息打印 */
    ID_GPHY_TOOL_SLAVE_MEAS_INFO_IND = 0xE073,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntDelayInd  */ /* 上报G从模测量信息 */

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

    /* GSM邻区相关信息上报 */
    ID_GPHY_TOOL_GSDR_DIFF_BASIC_CELL_IND = 0xE083,

    /* 邻区任务列表信息上报 */
    ID_GPHY_TOOL_NCELL_TASK_LIST_IND = 0xE084, /* _H2ASN_MsgChoice GPHY_TOOL_NcellTaskListIndRpt */

    /* SW-SAR消息上报 */
    ID_GPHY_TOOL_SW_SAR_INFO_IND = 0xE085, /* _H2ASN_MsgChoice ID_GPHY_TOOL_SW_SAR_INFO_IND */

    /* 时间平均SAR消息上报 */
    ID_GPHY_TOOL_TIME_AVG_SAR_PERIOD_IND  = 0xE097, /* _H2ASN_MsgChoice GPHY_TOOL_TimeAvgSarPeriodInd  */
    ID_GPHY_TOOL_TIME_AVG_SAR_DPR_ON_IND  = 0xE098, /* _H2ASN_MsgChoice GPHY_TOOL_TimeAvgSarDprStateInd  */
    ID_GPHY_TOOL_TIME_AVG_SAR_DPR_OFF_IND = 0xE099, /* _H2ASN_MsgChoice GPHY_TOOL_TimeAvgSarDprStateInd  */

    /* G从模SRVCC场景调度信息打印 */
    ID_GPHY_TOOL_SLAVE_BSIC_SYNC_RSLT_IND   = 0xE09A, /* _H2ASN_MsgChoice  GPHY_SLAVE_BSIC_SYNC_RSLT_STRU */
    ID_GPHY_TOOL_SLAVE_SRVCC_SCHDL_INFO_IND = 0xE09B,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntDelayInd  */ /* 上报G从模测量信息 */
    ID_GPHY_TOOL_INTERFE_STOP_G_BLANKING_IND = 0xE09C,
    /* _H2ASN_MsgChoice  COM_GPHY_INTERFE_INFO_STRU */ /* G关闭TX/RX操作 */
    ID_GPHY_TOOL_DT_STAT_CODEC_INFO_REQ = 0xE09D,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DtStatCodecReq  */ /* 该命令用于请求上报激活集中每个语音速率的使用率 */
    ID_GPHY_TOOL_DT_STAT_CODEC_INFO_IND = 0xE09E,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DtStatCodecInd  */ /* 上报激活集中每个语音速率的使用率上 */
    ID_GPHY_TOOL_REPORT_INFO_IND    = 0xE0F7,         /* _H2ASN_MsgChoice  GPHY_TOOL_ReportErrorInd  */
    ID_GPHY_TOOL_REPORT_WARNING_IND = 0xE0F8,         /* _H2ASN_MsgChoice  GPHY_TOOL_ReportErrorInd  */
    ID_GPHY_TOOL_REPORT_ERROR_IND   = 0xE0F9,         /* _H2ASN_MsgChoice  GPHY_TOOL_ReportErrorInd  */

    ID_TOOL_GPHY_BBP_RTC_REQ = 0xE0FA, /* _H2ASN_MsgChoice GPHY_TOOL_BbpRtcReq */
    ID_GPHY_TOOL_BBP_RTC_CNF = 0xE0FB, /* _H2ASN_MsgChoice GPHY_TOOL_CommonCnf */

    /* DSP-PROBE */
    ID_TOOL_GPHY_GSM_CELL_SNR_REQ = 0xE600,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GsmCellSnrReq  */ /* 该命令用于请求上报服务小区和6强邻区的信噪比，测量值 */
    ID_GPHY_TOOL_GSM_CELL_SNR_CNF = 0xE601,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_GSM_CELL_SNR_IND = 0xE602,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GsmCellSnrInd  */ /* 上报服务小区和6强邻区的信噪比，测量值 */

    ID_TOOL_GPHY_BA_LIST_MEAS_REQ = 0xE603,
    /* _H2ASN_MsgChoice  GPHY_TOOL_BaListMeasReq  */ /* 该命令用于请求上报服务区和32个邻区的信号强度，bsic值 */
    ID_GPHY_TOOL_BA_LIST_MEAS_CNF = 0xE604,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_BA_LIST_MEAS_IND = 0xE605,
    /* _H2ASN_MsgChoice  GPHY_TOOL_BaListMeasInd  */ /* 上报服务区和32个邻区的信号强度，bsic值 */

    ID_TOOL_GPHY_AMR_SETTING_REQ = 0xE606,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrSettingReq  */ /* 该命令用于请求上报AMR配置信息 */
    ID_GPHY_TOOL_AMR_SETTING_CNF = 0xE607,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_AMR_SETTING_IND = 0xE608,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrSettingInd  */ /* 上报AMR配置信息 */

    ID_TOOL_GPHY_AMR_CODEC_USAGE_REQ = 0XE609,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecUsageReq  */ /* 该命令用于请求上报激活集中每个语音速率的使用率 */
    ID_GPHY_TOOL_AMR_CODEC_USAGE_CNF = 0XE60A,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_AMR_CODEC_USAGE_IND = 0XE60B,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecUsageInd  */ /* 上报激活集中每个语音速率的使用率上 */

    ID_TOOL_GPHY_AMR_CODEC_STAT_REQ = 0xE60C,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecStatReq  */ /* 该命令用于请求各个编码速率的使用率 */
    ID_GPHY_TOOL_AMR_CODEC_STAT_CNF = 0xE60D,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_AMR_CODEC_STAT_IND = 0xE60E,
    /* _H2ASN_MsgChoice  GPHY_TOOL_AmrCodecStatInd  */ /* 上报各个编码速率的使用率 */

    ID_TOOL_GPHY_CS_PS_TX_INFO_REQ = 0xE60F,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CS_TX_INFO_REQ_STRU */ /* 该命令用于请求上报CS域上行信息 */
    ID_GPHY_TOOL_CS_PS_TX_INFO_CNF = 0xE610,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_CS_PS_TX_INFO_IND = 0xE611,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CS_TX_INFO_IND_STRU */ /* 上报CS域上行信息 */

    ID_TOOL_GPHY_CS_FER_REQ = 0xE612,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CsFerReq  */ /* 该命令请求上报CS域的FER */
    ID_GPHY_TOOL_CS_FER_CNF = 0xE613,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_CS_FER_IND = 0xE614,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CsFerInd  */ /* 上报CS域的FER */

    ID_TOOL_GPHY_GET_CURRENT_DSC_REQ = 0xE615,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GetCurrentDscReq  */ /* 该命令请求上报DSC计数值 */
    ID_GPHY_TOOL_GET_CURRENT_DSC_CNF = 0xE616,
    /* _H2ASN_MsgChoice  GPHY_TOOL_CommonCnf  */ /* 回复确认消息 */
    ID_GPHY_TOOL_GET_CURRENT_DSC_IND = 0xE617,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GetCurrentDscInd  */ /* 上报DSC计数值 */

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

    /* 双天线判决结果消息上报 */
    ID_GPHY_TOOL_DUAL_ANT_ESTIMATE_IND = 0xE640,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntEstimateInd  */ /* 上报主天线能量评估阶段判决信息 */
    ID_GPHY_TOOL_DUAL_ANT_COMPARE_IND = 0xE641,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntCompareInd  */ /* 上报主辅天线能量比较阶段每个调度状态的判决信息 */
    ID_GPHY_TOOL_DUAL_ANT_DELAY_IND = 0xE642,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntDelayInd  */ /* 上报天线切换迟滞信息 */
    ID_GPHY_TOOL_DUAL_ANT_CPMPARE_SUB_IND = 0xE643,
    /* _H2ASN_MsgChoice  GPHY_TOOL_DualAntCompareSubInd  */ /* 二阶并行算法每个上报周期主辅天线能量比较阶段判决信息 */
    ID_GPHY_TOOL_MAS_CHANGE_ANT_IND = 0xE644,
    /* _H2ASN_MsgChoice  GPHY_TOOL_MasChangeAntInd  */ /* MAS切换天线信息 */

    ID_TOOL_GPHY_GSDR_STATE_REPORT_REQ = 0xE650,
    /* _H2ASN_MsgChoice  TOOL_GPHY_GSDR_STATE_REQ_STRU */ /* 该命令用于请求上报服务小区和6强邻区的信噪比，测量值 */
    ID_GPHY_TOOL_GSDR_STATE_REPORT_CNF = 0xE651,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GSDR_STATE_CNF_STRU */ /* 回复确认消息 */
    ID_GPHY_TOOL_GSDR_STATE_REPORT_IND = 0xE652,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GsdrStateInd  */ /* 上报服务小区和6强邻区的信噪比，测量值 */

    ID_TOOL_GPHY_GSDR_FORCE_CTRL_REQ = 0xE653,
    /* _H2ASN_MsgChoice  TOOL_GPHY_GSDR_FORCE_CTRL_REQ_STRU */ /* 该命令用于请求上报服务小区和6强邻区的信噪比，测量值 */
    ID_GPHY_TOOL_GSDR_FORCE_CTRL_CNF = 0xE654,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GSDR_FORCE_CTRL_CNF_STRU */ /* 回复确认消息 */

    ID_TOOL_GPHY_GSDR_PARA_DUMP_REPORT_REQ = 0xE655,
    /* _H2ASN_MsgChoice  TOOL_GPHY_GSDR_STATE_REQ_STRU */ /* 该命令用于请求上报服务小区和6强邻区的信噪比，测量值 */
    ID_GPHY_TOOL_GSDR_PARA_DUMP_REPORT_CNF = 0xE656,
    /* _H2ASN_MsgChoice  GPHY_TOOL_GSDR_STATE_CNF_STRU */ /* 回复确认消息 */

    ID_GPHY_TOOL_RF_RESET_REPORT_IND = 0xE657,
    /* _H2ASN_MsgChoice  GPHY_TOOL_RfResetReport  */ /* 回复确认消息 */
    ID_GPHY_TOOL_GSM_PD_REPORT_IND           = 0xE658,
    ID_GPHY_TOOL_NCELL_RESEL_TIMER_STATE_IND = 0xE659,
    /* _H2ASN_MsgChoice  GPHY_TOOL_NcellReselTimerState  */ /* 该命令用于c1、c2重选定时器启停上报 */
    ID_GPHY_TOOL_NCELL_RESEL_TIMER_EXPIRED_IND = 0xE660,
    /* _H2ASN_MsgChoice  GPHY_TOOL_NcellReselTimerExpired  */ /* 该命令用于c1、c2重选定时器超时上报 */

    ID_GPHY_TOOL_GSDR_DUMP_SB_DATA_IND = 0xE661,

    ID_GPHY_TOOL_TIME_OUT_RPT_IND = 0xE662,
    /* _H2ASN_MsgChoice  GPHY_TOOL_TimeOutRptInd  */ /* 该命令用于负载过大异常时上报 */

    ID_GPHY_TOOL_INTERRAT_OPTIMIZATION_IND = 0xE663,
    /* _H2ASN_MsgChoice  GPHY_TOOL_InterratOptimizationInt  */ /* 异系统优化相关打印 */

    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_IN_MNTN_INFO = 0xE664,
    /* _H2ASN_MsgChoice  GL1C_SDR_ENC_ALG_IN_STRU */ /* 该命令用于编码前勾包 */
    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_IN_SUB_BLK_INFO = 0xE665,
    /* _H2ASN_MsgChoice  GSDR_ENCODE_SUB_BLK_RPT_STRU */ /* 该命令用于编码前子块勾包 */
    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_OUT_MNTN_INFO = 0xE666,
    /* _H2ASN_MsgChoice  GL1C_SDR_ENC_ALG_OUT_STRU */ /* 该命令用于编码后勾包 */
    ID_GPHY_TOOL_EVENT_GSDR_ENC_ALG_OUT_SUB_BLK_INFO = 0xE667,
    /* _H2ASN_MsgChoice  GSDR_ENCODE_SUB_BLK_RPT_STRU */ /* 该命令用于编码后子块勾包 */

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
    /* _H2ASN_MsgChoice GPHY_EASYRF_CONTEXT_RPT_STRU */ /* 该命令用于任务配置勾包 */
    ID_GPHY_TOOL_STRONGEST_NCELL_INFO_IND = 0xE676,
    /* _H2ASN_MsgChoice GPHY_TOOL_StrongestNcellInd  */ /* 该命令用于任务配置勾包 */
    ID_GPHY_TOOL_TX_POWER_LIMIT_INFO_IND = 0xE677,
    /* _H2ASN_MsgChoice GPHY_TOOL_TxPowerStatusInd  */ /* 该命令用于任务配置勾包 */
    ID_GPHY_TOOL_GSLAVE_INT0_TIMEOUT_IND = 0xE678, /* _H2ASN_MsgChoice GPHY_TOOL_GSLAVE_INT0_TIMEOUT_IND_STRU */
    ID_GPHY_TOOL_CLT_STATE_RPT_REQ       = 0xE679, /* _H2ASN_MsgChoice GPHY_TOOL_CLT_STATE_RPT_REQ_STRU */
    ID_GPHY_TOOL_RCM_TASK_NO_REL_RPT_IND = 0xE680, /* _H2ASN_MsgChoice GPHY_TOOL_RCM_TASK_NO_REL_IND_STRU  */

    ID_GPHY_TOOL_MEA_RPT_INFO_IND = 0xE681,
    /* _H2ASN_MsgChoice GPHY_ISR_GPHY_MEA_RPT_INFO_STRU */ /* 该命令用于测量结果勾包 */
    ID_GPHY_TOOL_SLAVE_MEA_RPT_INFO_IND = 0xE682,
    /* _H2ASN_MsgChoice GPHY_SLAVE_ISR_MEAS_RPT_INFO_STRU */ /* 该命令用于从模测量结果勾包 */
    ID_GPHY_TOOL_SLAVE_GTC_TASK_INFO_IND = 0xE683,
    /* _H2ASN_MsgChoice GPHY_SLAVE_GTC_CONTEXT_RPT_STRU */ /* 该命令用于从模GTC任务勾包 */
    ID_GPHY_TOOL_START_LMEAS_INFO_IND = 0xE684,
    /* _H2ASN_MsgChoice GPHY_TOOL_START_LMEAS_STRU */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_START_WMEAS_INFO_IND = 0xE685,
    /* _H2ASN_MsgChoice GPHY_TOOL_START_WMEAS_STRU */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_OCCASION_STOP_IND = 0xE686,
    /* _H2ASN_MsgChoice GPHY_TOOL_OCCASION_STOP_STRU */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_SUPPLEMENT_RF_TASK_INFO_IND = 0xE687,
    /* _H2ASN_MsgChoice GPHY_TOOL_SUPPLEMENT_RF_TASK_STRU */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_FIR2_FILTER_DCH_PARA_IND = 0xE688,
    /* _H2ASN_MsgChoice GPHY_TOOL_FIR2_FILTER_DCH_PARA_STRU */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_SLAVE_TASK_OP_RF_INVALID_RPT = 0xE689,
    /* _H2ASN_MsgChoice GPHY_SLAVE_TASK_OP_RF_INVALID_RPT_STRU */ /* 该命令用于从模在沟外操作射频的勾包 */
    ID_GPHY_TOOL_SLAVE_GAP_INFO_IND = 0xE68A,
    /* _H2ASN_MsgChoice GPHY_SLAVE_GAP_INFO_RPT_STRU */ /* 该命令用于从模任务信息的勾包 */
    ID_GPHY_TOOL_SLAVE_SB_FORECAST_INFO_IND = 0xE68B,
    /* _H2ASN_MsgChoice GPHY_SLAVE_SB_FORECAST_INFO_RPT_STRU */ /* 该命令用于从模sb任务定时信息的勾包 */
    ID_GPHY_TOOL_GTC_FN_RUNTIME_INFO_IND = 0xE68C,
    /* _H2ASN_MsgChoice GPHY_TOOL_GTC_FN_RUNTIME_INFO_IND_STRU */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_MODEM_STATUS_INFO_IND = 0xE68D,
    /* _H2ASN_MsgChoice GPHY_TOOL_MODEM_STATUS_INFO_STRU */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_CANCEL_TASK_CONTEXT_INFO = 0xE68E,
    /* _H2ASN_MsgChoice GPHY_EASYRF_CancelTaskContextRpt */ /* 该命令用于勾包 */
    ID_GPHY_TOOL_ANT_TASMAS_INFO_IND = 0xE69E,
    /* _H2ASN_MsgChoice GPHY_TOOL_ANT_TASMAS_INFO_STRU */ /* 该命令用于勾包 */

    ID_GPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnMsgIdUint16;

/*
 * GPHY回复CNF时,携带的处理结果枚举
 */
enum GPHY_TOOL_MntnRslt {
    GPHY_MNTN_RSLT_SUCC = 0,
    GPHY_MNTN_RSLT_FAIL = 1,

    GPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnRsltUint16;

/*
 * 结构说明: 操作bbp公共资源时，操作方式枚举
 */
enum GPHY_TOOL_BbpOperateType {
    GPHY_TOOL_BBP_WR = 0,
    GPHY_TOOL_BBP_RD = 1,

    GPHY_TOOL_BBP_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_BbpOperateTypeUint16;

/*
 * 枚举说明: 物理层状态枚举定义,用于上报
 */
enum GPHY_TOOL_PhyStates {
    GPHY_TOOL_PHY_NULL = 0,      /* 物理层处于NULL态 */
    GPHY_TOOL_PHY_SEARCHING_BCH, /* 物理层处于SearchingBCH态 */
    GPHY_TOOL_PHY_BCH,           /* 物理层处于BCH态 */
    GPHY_TOOL_PHY_TUNING_DCH,    /* 物理层处于TuningDCH态 */
    GPHY_TOOL_PHY_DCH,           /* 物理层处于DCH态 */
    GPHY_TOOL_PHY_SLAVE,         /* 物理层处理从模SLAVE态 */
    GPHY_TOOL_PHY_STATES_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_PhyStatesUint16;

/*
 * TOOL下发参数控制，主要是功率控制和BEP控制
 */
enum GPHY_TOOL_ParaCtrl {
    GPHY_TOOL_PARA_PWC_CTRL = 0x01, /* 下发控制功率参数 */
    GPHY_TOOL_PARA_BEP_CTRL = 0x02, /* 下发控制BEP参数 */

    GPHY_TOOL_PARA_CTRL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_ParaCtrlUint16;

/*
 * 枚举说明: GPHY错误上报级别
 */
enum GPHY_TOOL_ErrorRptLevel {
    GPHY_TOOL_ERROR_LEVEL_INFO    = 1,
    GPHY_TOOL_ERROR_LEVEL_WARNING = 2,
    GPHY_TOOL_ERROR_LEVEL_ERROR   = 3,

    GPHY_TOOL_ERROR_LEVEL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_ErrorRptLevelUint16;

/*
 * 枚举说明: 强停从模的原因
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
 * GPHY模块定义
 */

enum GPHY_TOOL_Module {
    GPHY_MODULE_DRV          = 0x5501, /* DRIVER模块名编号 */
    GPHY_MODULE_MEA          = 0x5502, /* 测量模块名编号 */
    GPHY_MODULE_NCE_TSK      = 0x5503, /* 邻区任务模块名编号 */
    GPHY_MODULE_NCE_RES      = 0x5504, /* 邻区重选模块名编号 */
    GPHY_MODULE_CHN_CCH      = 0x5505, /* 公共信道模块名编号 */
    GPHY_MODULE_CHN_CSDCH    = 0x5506, /* CS专用信道模块名编号 */
    GPHY_MODULE_CHN_PSDCH    = 0x5507, /* PS专用信道模块名编号 */
    GPHY_MODULE_CHN_AMRDTX   = 0x5508, /* 信道AMRDTX模块名编号 */
    GPHY_MODULE_COM          = 0x5509, /* 公共模块名编号 */
    GPHY_MODULE_COM_MAIL     = 0x550A, /* 公共邮箱模块名编号 */
    GPHY_MODULE_COM_TOA      = 0x550B, /* 公共TOA模块名编号 */
    GPHY_MODULE_DRX          = 0x550C, /* DRX模块名编号 */
    GPHY_MODULE_OM           = 0x550D, /* OM模块名编号 */
    GPHY_MODULE_WMEAS        = 0x550E, /* WMEAS模块名编号 */
    GPHY_MODULE_CAL          = 0x550F, /* CAL模块名编号 */
    GPHY_MODULE_LMEAS        = 0x5510, /* LTE测量模块名编号 */
    GPHY_MODULE_PD_INTERFACE = 0x5511, /* 下电部分代码接口函数模块 */
    GPHY_MODULE_TMEAS        = 0x5512, /* TD测量模块名编号 */
    GPHY_MODULE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_ModuleUint16;

/*
 * 3.2 枚举定义,WPHY枚举定义
 */

/*
 * WPHY<-->TOOL原语列表
 */

enum WPHY_TOOL_MntnMsgId {
    /* 下发REQ消息因为带VOS消息，上报的IND或CNF消息不带VOS消息头，应对REQ另外解析，这里指示为了ASN生成方便 */
    ID_TOOL_WPHY_REQ = 0x0, /* _H2ASN_MsgChoice TOOL_WPHY_REQ_STRU */

    /* 信令模式下实时修改PA增益切换点补偿的请求和回复原语 */
    ID_TOOL_WPHY_RF_PA_GAIN_SWITCH_COMP_REQ = 0xC801, /* _H2ASN_MsgChoice WPHY_TOOL_PaGainSwitchCompReq  */
    ID_WPHY_TOOL_RF_PA_GAIN_SWITCH_COMP_CNF = 0xC802, /* _H2ASN_MsgChoice WPHY_TOOL_PaGainSwitchCompCnf  */

    /* 信令模式下PD使能控制 */
    ID_TOOL_WPHY_PD_ENABLE_REQ = 0xC803, /* _H2ASN_MsgChoice WPHY_TOOL_PowDetEnableReq  */
    ID_WPHY_TOOL_PD_ENABLE_CNF = 0xC804, /* _H2ASN_MsgChoice WPHY_TOOL_PowDetEnableCnf  */

    /* 激活集小区上报 */
    ID_TOOL_WPHY_ACT_CELL_REQ = 0xF001, /* _H2ASN_MsgChoice WPHY_TOOL_ActCellReq  */
    ID_WPHY_TOOL_ACT_CELL_CNF = 0xF002, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_ACT_CELL_IND = 0xF003, /* _H2ASN_MsgChoice WPHY_TOOL_ActCellInfoInd  */

    /* 小区搜索列表上报 */
    ID_TOOL_WPHY_CELL_SEARCH_LIST_REQ = 0xF004, /* _H2ASN_MsgChoice WPHY_TOOL_CellSearchListReq   */
    ID_WPHY_TOOL_CELL_SEARCH_LIST_CNF = 0xF005, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_CELL_SEARCH_LIST_IND = 0xF006, /* _H2ASN_MsgChoice WPHY_TOOL_CellSearchListInd  */

    /* 功控开关控制 */
    ID_TOOL_WPHY_PWR_SWITCH_CTRL_REQ = 0xF00A, /* _H2ASN_MsgChoice WPHY_TOOL_PwrSwitchCtrlReq  */
    ID_WPHY_TOOL_PWR_SWITCH_CTRL_CNF = 0xF00B, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* 功控参数控制 */
    ID_TOOL_WPHY_PWR_PARA_SET_REQ = 0xF010, /* _H2ASN_MsgChoice WPHY_TOOL_PwrParaSetReq  */
    ID_WPHY_TOOL_PWR_PARA_SET_CNF = 0xF011, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* 误块率上报 */
    ID_TOOL_WPHY_BLER_REPORT_REQ = 0xF020, /* _H2ASN_MsgChoice WPHY_TOOL_BlerReportReq  */
    ID_WPHY_TOOL_BLER_REPORT_CNF = 0xF021, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_BLER_REPORT_IND = 0xF022, /* _H2ASN_MsgChoice WPHY_TOOL_BlerReportInd  */

    /* 事件上报 */
    ID_TOOL_WPHY_EVENT_REPORT_REQ = 0xF02A, /* _H2ASN_MsgChoice WPHY_TOOL_EventReportReq  */
    ID_WPHY_TOOL_EVENT_REPORT_CNF = 0xF02B, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* 事件上报:同失步上报 */
    ID_WPHY_TOOL_SYNC_STATUS_IND = 0xF02C, /* _H2ASN_MsgChoice WPHY_TOOL_SyncStatusInd  */

    /* 事件上报:AFC状态 */
    ID_WPHY_TOOL_VCTCXO_STATUS_IND = 0xF02E, /* _H2ASN_MsgChoice WPHY_TOOL_AfcStatusInd  */

    /* 事件上报:中断任务异常报告 */
    ID_WPHY_TOOL_INTERRUPT_EXCEPTIION_IND = 0xF030, /* _H2ASN_MsgChoice WPHY_TOOL_InterruptExceptiionInd  */

    /* 事件上报:L1状态上报 */
    ID_WPHY_TOOL_WCDMA_L1_STATUS_IND = 0xF031, /* _H2ASN_MsgChoice WPHY_TOOL_WCDMA_L1_STATUS_IND_STRU */

    /* DRX控制 */
    ID_TOOL_WPHY_DRX_SWITCH_REQ = 0xF032, /* _H2ASN_MsgChoice WPHY_TOOL_DrxSwitchReq  */
    ID_WPHY_TOOL_DRX_SWITCH_CNF = 0xF033, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* 门限设置 */
    ID_TOOL_WPHY_THRESHOLD_SET_REQ = 0xF034, /* _H2ASN_MsgChoice WPHY_TOOL_ThresholdSetReq  */
    ID_WPHY_TOOL_THRESHOLD_SET_CNF = 0xF035, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* 门限查询 */
    ID_TOOL_WPHY_THRESHOLD_QUERY_REQ = 0xF036, /* _H2ASN_MsgChoice WPHY_TOOL_ThresholdQueryReq  */
    ID_WPHY_TOOL_THRESHOLD_QUERY_IND = 0xF037, /* _H2ASN_MsgChoice WPHY_TOOL_ThresholdQueryInd  */

    /* 错误上报 */
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

    /* T313 状态上报 */
    ID_WPHY_TOOL_T313_STATUS_IND = 0xF042, /* _H2ASN_MsgChoice WPHY_TOOL_T313_STATUS_IND_STRU  */
    /* T312 状态上报 */
    ID_WPHY_TOOL_T312_STATUS_IND = 0xF043, /* _H2ASN_MsgChoice WPHY_TOOL_T312_STATUS_IND_STRU */

    /* PA状态上报 */
    ID_TOOL_WPHY_PA_STATUS_REPORT_REQ = 0xF046, /* _H2ASN_MsgChoice WPHY_TOOL_PaStatusReportReq  */
    ID_WPHY_TOOL_PA_STATUS_REPORT_CNF = 0xF047, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */
    ID_WPHY_TOOL_PA_STATUS_REPORT_IND = 0xF048, /* _H2ASN_MsgChoice WPHY_TOOL_PaStatusReportInd  */

    ID_TOOL_WPHY_INTRA_MSG_RPT_CTRL_REQ = 0xF049, /* _H2ASN_MsgChoice WPHY_TOOL_IntraMsgRptCtrlReq  */
    ID_WPHY_TOOL_INTRA_MSG_RPT_CTRL_CNF = 0xF04A, /* _H2ASN_MsgChoice WPHY_TOOL_CommonCnf  */

    /* 维测消息bitmap开关 */
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

    /* 前端可维可测 */
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

    /* 干扰特性新增 */
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
 * 枚举说明: 各种回复消息的结果值:成功或者失败等
 */
enum WPHY_TOOL_MntnRslt {
    WPHY_MNTN_RSLT_SUCC               = 0, /* 成功 */
    WPHY_MNTN_RSLT_FAIL               = 1, /* 失败 */
    WPHY_MNTN_RSLT_PERIOD_NOT_SUPPORT = 2, /* 配置周期不支持 */
    WPHY_MNTN_RSLT_PARA_INVALID       = 3, /* 设置参数无效 */
    WPHY_MNTN_RSLT_PARA_TYPE_OVERFLOW = 4, /* 设置参数类型不正确 */

    WPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRsltUint16;

/*
 * 枚举说明: 各种测量报告上报周期
 */
enum WPHY_TOOL_MntnReportType {
    WPHY_MNTN_REPORT_TYPE_NO_RPT          = 0, /* 不上报: */
    WPHY_MNTN_REPORT_TYPE_ONE_TIME        = 1, /* 上报1次: */
    WPHY_MNTN_REPORT_TYPE_PER_FRAME       = 2, /* 每帧上报: */
    WPHY_MNTN_REPORT_TYPE_ASSIGNED_PERIOD = 3, /* 指定周期:以帧为单位。如果选择此项，则必须填写下一个变量来指定周期。 */
    WPHY_MNTN_REPORT_TYPE_EVENT_TRIGER    = 4, /* 事件触发: */

    WPHY_MNTN_REPORT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnReportTypeUint16;

/*
 * 枚举说明: 开关控制的枚举值
 */
enum WPHY_TOOL_MntnPwrSwitchType {
    WPHY_MNTN_PWR_SWITCH_TYPE_UL_PC      = 0, /* 上行功率控制:固定上行发射东率 */
    WPHY_MNTN_PWR_SWITCH_TYPE_OLPC       = 1, /* 外环功控 */
    WPHY_MNTN_PWR_SWITCH_TYPE_WINUP      = 2, /* winup控制 */
    WPHY_MNTN_PWR_SWITCH_TYPE_WINDOWN    = 3, /* windown控制 */
    WPHY_MNTN_PWR_SWITCH_TYPE_FIX_DL_PWR = 4, /* 固定下行发射功率 */

    WPHY_MNTN_PWR_SWITCH_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnPwrSwitchTypeUint16;

/*
 * 枚举说明: 开关控制的枚举值
 */
enum WPHY_TOOL_MntnSwitchCtrl {
    WPHY_MNTN_SWITCH_CTRL_CLOSE = 0, /* 关闭 */
    WPHY_MNTN_SWITCH_CTRL_OPEN  = 1, /* 打开 */

    WPHY_MNTN_SWITCH_CTRL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSwitchCtrlUint16;

/*
 * 枚举说明: 开关控制的枚举值
 */
enum WPHY_TOOL_SyncStatus {
    WPHY_MNTN_SYNC_STATUS_FIX  = 0, /* 锁定 */
    WPHY_MNTN_SYNC_STATUS_LOST = 1, /* 失锁 */

    WPHY_MNTN_SYNC_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_SyncStatusUint16;

/*
 * 枚举说明: 开关控制的枚举值
 */
enum UPHY_TOOL_XoStatus {
    UPHY_MNTN_XO_STATUS_UNLOCKED = 0, /* 失锁 */
    UPHY_MNTN_XO_STATUS_LOCKED   = 1, /* 锁定 */

    UPHY_MNTN_XO_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_XoStatusUint16;

/*
 * 枚举说明: 功率参数类型，查询和设置公用
 */
enum WPHY_TOOL_MntnPwrPara {
    WPHY_MNTN_PWR_PARA_INIT_VALUE            = 0x00000000, /* Init_Value */
    WPHY_MNTN_PWR_PARA_SIR_TARGET            = 0x00000001, /* SirTarget */
    WPHY_MNTN_PWR_PARA_CURR_TRCH_ID_SELECTED = 0x00000002, /* 当前选择的TrchId */
    WPHY_MNTN_PWR_PARA_DL_TPC                = 0x00000004, /* DL TPC */
    WPHY_MNTN_PWR_PARA_CURR_SIR              = 0x00000008, /* 当前的SIR */
    WPHY_MNTN_PWR_PARA_CURR_BLER             = 0x00000010, /* 当前的BLER */
    WPHY_MNTN_PWR_PARA_UL_TPC                = 0x00000020, /* UL TPC */
    WPHY_MNTN_PWR_PARA_BD                    = 0x00000040, /* Bd */
    WPHY_MNTN_PWR_PARA_BEC                   = 0x00000080, /* Bec */
    WPHY_MNTN_PWR_PARA_BED                   = 0x00000100, /* Bed */
    WPHY_MNTN_PWR_PARA_BHS_NACK              = 0x00000200, /* BhsNack */
    WPHY_MNTN_PWR_PARA_BHS_ACK               = 0x00000400, /* BhsAck */
    WPHY_MNTN_PWR_PARA_BHS_CQI               = 0x00000800, /* BhsCQI */
    WPHY_MNTN_PWR_PARA_BHS_TFCI              = 0x00001000, /* TFCI */
    WPHY_MNTN_PWR_PARA_SLOT_FORMAT           = 0x00002000, /* SlotFormat */
    WPHY_MNTN_PWR_PARA_MAX_PWR               = 0x00004000, /* 最大发射功率 */
    WPHY_MNTN_PWR_PARA_MIN_PWR               = 0x00008000, /* 最小发射功率 */
    WPHY_MNTN_PWR_PARA_INIT_PWR              = 0x00010000, /* lint !e575 初始发射功率 */
    WPHY_MNTN_PWR_PARA_LIST_MODE_SET_PWR     = 0x00020000, /* lint !e575 LIST MODE下关闭内环功控直接设置的发射功率 */
    WPHY_MNTN_PWR_PARA_LIST_MODE_OPEN_ILPC   = 0x00040000, /* lint !e575 LIST MODE下重新打开内环功控 */
    WPHY_MNTN_PWR_PARA_DPCCH_MIN_PWR         = 0x00080000, /* DPCCH 最小发射功率 */
    WPHY_MNTN_PWR_PARA_DPCCH_MAX_PWR         = 0x00100000, /* DPCCH 最大发射功率 */
    WPHY_MNTN_PWR_PARA_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MntnPwrParaUint32;

/*
 * 枚举说明: 上报事件类型
 */
enum WPHY_TOOL_MntnEventType {
    WPHY_MNTN_EVENT_TYPE_AFC_STATUS  = 0x00000001, /* 6ms 任务超时 */
    WPHY_MNTN_EVENT_TYPE_SYNC_STATUS = 0x00000002, /* 9ms 任务超时 */
    WPHY_MNTN_EVENT_TYPE_L1_STATUS   = 0x00000004, /* L1状态 */
    WPHY_MNTN_EVENT_TYPE_INT_EXCEPT  = 0x00000008, /* 中断异常 */

    WPHY_MNTN_EVENT_TYPE_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MntnEventTypeUint32;

/*
 * 枚举说明: 中断异常类型
 */
enum WPHY_TOOL_MntnIntExcept {
    WPHY_MNTN_INT_EXCEPT_TYPE_6MS_TASK_EXPIRED = 0, /* 6ms 任务超时 */
    WPHY_MNTN_INT_EXCEPT_TYPE_9MS_TASK_EXPIRED = 1, /* 9ms 任务超时 */

    WPHY_MNTN_INT_EXCEPT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnIntExceptUint16;

/*
 * 枚举说明: 系统状态
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
 * 枚举说明: 可配置门限类型
 */
enum WPHY_TOOL_MntnThresholdType {
    /* 小区搜索门限 */
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

    /* 多径搜索门限 */
    WPHY_MNTN_THRESHOLD_TYPE_CORRE_LENTH_LOCK,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_LENTH_LOCK,
    WPHY_MNTN_THRESHOLD_TYPE_CORRE_LENTH_UNLOCK,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_LENTH_UNLOCK,

    /* 测量门限 */
    WPHY_MNTN_THRESHOLD_TYPE_MEAS_AVERAGE_NOISE,
    WPHY_MNTN_THRESHOLD_TYPE_MP_RELATIVE_FACTOR,

    WPHY_MNTN_THRESHOLD_TYPE_MEAS_TIMING_ECNO_THRESHOLD,
    WPHY_MNTN_THRESHOLD_TYPE_MEAS_TIMING_RSCP_THRESHOLD,

    WPHY_MNTN_THRESHOLD_TYPE_MAX_TIMING_MEAS_FRAME_NUMBER,

    /* 功控门限 */
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

    /* DRX功能门限 */
    WPHY_MNTN_THRESHOLD_TYPE_32K_CLOCK_MSR_WAIT_SFN_NUM,
    WPHY_MNTN_THRESHOLD_TYPE_SLEEP_LENTH,
    WPHY_MNTN_THRESHOLD_TYPE_TIME_ADJUST,

    WPHY_MNTN_THRESHOLD_TYPE_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MntnThresholdTypeUint32;

/*
 * 枚举说明: 开关控制的枚举值
 */
enum WPHY_TOOL_MntnPaStatus {
    WPHY_MNTN_PA_STATUS_CLOSE = 0, /* 关闭 */
    WPHY_MNTN_PA_STATUS_OPEN  = 1, /* 打开 */

    WPHY_MNTN_PA_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnPaStatusUint16;

/*
 * 枚举说明: WPHY可维可测固定抓取的几块数据枚举
 */
enum WPHY_TOOL_MntnRamRpt {
    WPHY_MNTN_RPT_HSUPA_SHARE_CTRL = 0, /* UPA控制 */
    WPHY_MNTN_RPT_DSP_MODE         = 1, /* DSP模式 */
    WPHY_MNTN_RPT_HSUPA_RPT        = 2,
    WPHY_MNTN_RPT_HSUPA_TTI        = 3,
    WPHY_MNTN_RPT_CPC_DRX_RPT      = 4,
    WPHY_MNTN_RPT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRamRptUint16;

/*
 * 枚举说明: WPHY错误上报级别
 */
enum WPHY_TOOL_MntnErrorRptLevel {
    WPHY_MNTN_ERROR_LEVEL_INFO    = 1,
    WPHY_MNTN_ERROR_LEVEL_WARNING = 2,
    WPHY_MNTN_ERROR_LEVEL_ERROR   = 3,

    WPHY_MNTN_ERROR_LEVEL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErrorRptLevelUint16;

/*
 * 枚举说明: 开关控制的枚举值
 */
enum UPHY_TOOL_MntnRptStatus {
    UPHY_MNTN_RPT_STATUS_CLOSE = 0, /* 关闭 */
    UPHY_MNTN_RPT_STATUS_OPEN  = 1, /* 打开 */
    UPHY_MNTN_RPT_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnRptStatusUint16;

#if (FEATURE_GSM_WHOLE_SDR == FEATURE_ON)
/*
 * 枚举说明: HIDS强控软基带开关控制的枚举值
 */
enum UPHY_TOOL_MntnCtrlStatus {
    UPHY_MNTN_CTRL_STATUS_NORMAL = 0, /* 默认不强制控制 */
    UPHY_MNTN_CTRL_STATUS_ISAIC  = 1, /* 强制打开ISAIC */
    UPHY_MNTN_CTRL_STATUS_DAIC   = 2, /* 强制打开DAIC */
    UPHY_MNTN_CTRL_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnCtrlStatusUint16;

#else
/*
 * 枚举说明: 开关控制的枚举值
 */
enum UPHY_TOOL_MntnCtrlStatus {
    UPHY_MNTN_CTRL_STATUS_NORMAL = 0, /* 默认功能不打开 */
    UPHY_MNTN_CTRL_STATUS_START  = 1, /* 强制打开ISAIC */
    UPHY_MNTN_CTRL_STATUS_STOP   = 2, /* 强制关闭SDR */
    UPHY_MNTN_CTRL_STATUS_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnCtrlStatusUint16;

#endif

/*
 * 枚举说明: 开关控制的枚举值
 */
enum WPHY_TOOL_MntnRptStatus {
    WPHY_MNTN_RPT_STATUS_CLOSE = 0, /* 关闭 */
    WPHY_MNTN_RPT_STATUS_OPEN  = 1, /* 打开 */

    WPHY_MNTN_RPT_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRptStatusUint16;

/*
 * 枚举说明: T313 Status
 * 枚举说明: T313定时器 状态
 */
enum WPHY_TOOL_MntnT313Status {
    WPHY_MNTN_T313_START   = 0,
    WPHY_MNTN_T313_STOP    = 1,
    WPHY_MNTN_T313_TIMEOUT = 2,

    WPHY_MNTN_T313_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT313StatusUint16;

/*
 * 枚举说明: T312 Status
 * 枚举说明: T312定时器 状态
 */
enum WPHY_TOOL_MntnT312Status {
    WPHY_MNTN_T312_START   = 0,
    WPHY_MNTN_T312_STOP    = 1,
    WPHY_MNTN_T312_TIMEOUT = 2,

    WPHY_MNTN_T312_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT312StatusUint16;

/*
 * 枚举说明: 强停LTE原因
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
 * 枚举说明: 从模状态
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
 * 枚举说明: 当前均衡工作状态
 * 枚举说明: 从模状态
 */
enum WPHY_TOOL_MntnCpcDrxEquaStatus {
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_ALWAYS_OPEN = 0, /* 均衡长开状态 */
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_DRX_OPEN,        /* 均衡打开状态 */
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_DRX_CLOSE,       /* 均衡关闭状态 */
    WPHY_MNTN_CPC_DRX_EQUA_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcDrxEquaStatusUint16;

/*
 * 枚举说明: 物理层枚举类型
 */
enum WPHY_TOOL_MntnBool {
    WPHY_MNTN_BOOL_FALSE = 0,
    WPHY_MNTN_BOOL_TRUE  = 1,
    WPHY_MNTN_BOOL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnBoolUint16;

/*
 * 枚举说明: 物理层枚举类型
 */
enum UPHY_TOOL_MntnBool {
    UPHY_MNTN_BOOL_FALSE = 0,
    UPHY_MNTN_BOOL_TRUE  = 1,
    UPHY_MNTN_BOOL_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnBoolUint16;

/*
 * 枚举说明: CQI 优先级类型定义
 */
enum WPHY_TOOL_MntnCpcDrxCqiPri {
    WPHY_MNTN_CPC_DRX_CQI_PRIORITY_0 = 0, /* CQI优先级为0 */
    WPHY_MNTN_CPC_DRX_CQI_PRIORITY_1,     /* CQI优先级为1 */
    WPHY_MNTN_CPC_DRX_CQI_PRIORITY_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcDrxCqiPriUint16;

/*
 * 枚举说明: 综合主辅载波的状态后的出的cycle状态的最大集合
 */
enum WPHY_TOOL_MntnCpcUldpcchCycleState {
    WPHY_MNTN_CPC_ULDPCCH_CYCLE1 = 0,
    WPHY_MNTN_CPC_ULDPCCH_CYCLE2,
    WPHY_MNTN_CPC_ULDPCCH_CYCLE1AND2,
    WPHY_MNTN_CPC_ULDPCCH_CYCLE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcUldpcchCycleStateUint16;

/*
 * 枚举说明:CPC DRX
 */
enum WPHY_TOOL_MntnCpcDrxRfStatus {
    WPHY_MNTN_CPC_DRX_RF_STATUS_ALWAYS_OPEN = 0, /* RF一直打开 */
    WPHY_MNTN_CPC_DRX_RF_STATUS_DRX_OPEN,        /* RF处于DRX的打开状态 */
    WPHY_MNTN_CPC_DRX_RF_STATUS_DRX_CLOSE,       /* RF处于DRX的关闭状态 */
    WPHY_MNTN_CPC_DRX_RF_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCpcDrxRfStatusUint16;

/*
 * 枚举说明: MAS状态枚举
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
 * 枚举说明: MAS 主被控模式枚举
 */
enum WPHY_TOOL_MntnCtrlMode {
    WPHY_MNTN_CTRL_MODE_ACTIVE  = 0,
    WPHY_MNTN_CTRL_MODE_PASSIVE = 1,
    WPHY_MNTN_CTRL_MODE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnCtrlModeUint16;

/*
 * 枚举说明: 天线TAS状态枚举
 */
enum WPHY_TOOL_MntnMasAnttasstatus {
    WPHY_MNTN_MAS_TASDPDT0 = 0, /* TAS直通态 */
    WPHY_MNTN_MAS_TASDPDT1 = 1, /* TAS交叉态 */
    WPHY_MNTN_MAS_TAS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasAnttasstatusUint16;

/*
 * 枚举说明: 天线MAS状态枚举
 */
enum WPHY_TOOL_MntnMasAntmasstatus {
    WPHY_MNTN_MAS_MASDPDT0 = 0, /* MAS默认态:天线在右边 */
    WPHY_MNTN_MAS_MASDPDT1 = 1, /* MAS非默认态:天线在左边 */
    WPHY_MNTN_MAS_MAS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasAntmasstatusUint16;

/*
 * 枚举说明: MAS ANTENNA枚举
 */
enum WPHY_TOOL_MntnMasAntenna {
    WPHY_MNTN_MAS_ANTENNA_SOURCE = 0,
    WPHY_MNTN_MAS_ANTENNA_DEST   = 1,
    WPHY_MNTN_MAS_ANTENNA_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnMasAntennaUint16;

/*
 * 枚举说明: 档位调整结果枚举
 */
enum WPHY_TOOL_MntnTxLevelAdjust {
    WPHY_MNTN_TX_LEVEL_ADJUST_UP           = 0, /* 升档 */
    WPHY_MNTN_TX_LEVEL_ADJUST_HOLD         = 1, /* 保持 */
    WPHY_MNTN_TX_LEVEL_ADJUST_DOWN         = 2, /* 降档 */
    WPHY_MNTN_TX_LEVEL_ADJUST_EXIT_SAR_TAS = 3, /* 退出SAR TAS */
    WPHY_MNTN_TX_LEVEL_ADJUST_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxLevelAdjustUint16;

/*
 * 枚举说明: TX_STATEII档位
 */
enum WPHY_TOOL_MntnTxStateiiLevelCfg {
    WPHY_MNTN_TX_STATEII_LEVEL_0 = 0, /* TX_STATEII档位0 */
    WPHY_MNTN_TX_STATEII_LEVEL_1 = 1, /* TX_STATEII档位1 */
    WPHY_MNTN_TX_STATEII_LEVEL_2 = 2, /* TX_STATEII档位2 */
    WPHY_MNTN_TX_STATEII_LEVEL_3 = 3, /* TX_STATEII档位3 */
    WPHY_MNTN_TX_STATEII_LEVEL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxStateiiLevelUint16;

/*
 * 枚举说明: TX POWER受限情况枚举
 */
enum WPHY_TOOL_MntnTxPower {
    WPHY_MNTN_TX_POWER_CURRENT_LIMITED   = 0, /* 当前档位受限 */
    WPHY_MNTN_TX_POWER_BACKOFF_LIMITED   = 1, /* 回退一档档位受限 */
    WPHY_MNTN_TX_POWER_MONITOR_LIMITED   = 2, /* MONITOR功率受限 */
    WPHY_MNTN_TX_POWER_MONITOR_UNLIMITED = 3, /* MONITOR功率不受限 */
    WPHY_MNTN_TX_POWER_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxPowerUint16;

/*
 * 枚举说明: MAS_TX_STATEII_SUB_STATE
 */
enum WPHY_TOOL_MntnTxStateiiSubStateCfg {
    WPHY_MNTN_TX_STATEII_SUB_STATE_BETTER_RX = 0, /* RX好天线判断子阶段 */
    WPHY_MNTN_TX_STATEII_SUB_STATE_T1        = 1, /* 主天线TX功率采集子阶段 */
    WPHY_MNTN_TX_STATEII_SUB_STATE_T2        = 2, /* 辅天线TX功率采集子阶段 */
    WPHY_MNTN_TX_STATEII_SUB_STATE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxStateiiSubStateUint16;

/*
 * 枚举说明: TX_SIGNAL_STATE
 */
enum WPHY_TOOL_MntnTxSignalState {
    WPHY_MNTN_TX_SIGNAL_STATE_GOOD = 0, /* TX_STATE GOOD */
    WPHY_MNTN_TX_SIGNAL_STATE_POOR = 1, /* TX_STATE POOR */
    WPHY_MNTN_TX_SIGNAL_STATE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnTxSignalStateUint16;

/*
 * 枚举说明: 当前rake多径运行的状态
 */
enum WPHY_TOOL_RakeMpStatus {
    WPHY_TOOL_RAKE_MP_STATUS_NONE = 0,   /* 多径搜索没有运行 */
    WPHY_TOOL_RAKE_MP_STATUS_RAKE,       /* 运行的是RAKE多径 */
    WPHY_TOOL_RAKE_MP_STATUS_RAKE_TYPE1, /* 运行的是RAKE TYPE1多径 */

    WPHY_TOOL_RAKE_MP_STATUS_RAKE_BUTT,
};
typedef VOS_UINT16 WPHY_TOOL_RakeMpStatusUint16;

/*
 * 枚举说明: 下行TPC命令字
 */
enum WPHY_TOOL_MntnDlTpcCmd {
    WPHY_MNTN_DL_TPC_CMD_HOLD = 0, /* 保持功率 */
    WPHY_MNTN_DL_TPC_CMD_UP   = 1, /* 升功率 */
    WPHY_MNTN_DL_TPC_CMD_DOWN = 3  /* 降功率 */
};
typedef VOS_UINT16 WPHY_TOOL_MntnDlTpcCmdUint16;

/*
 * 枚举说明: 下行TPC命令字
 */
enum WPHY_TOOL_MntnUlTpcCmd {
    WPHY_MNTN_UL_TPC_CMD_DOWN = 0, /* 降功率 */
    WPHY_MNTN_UL_TPC_CMD_UP   = 1  /* 升功率 */
};
typedef VOS_UINT16 WPHY_TOOL_MntnUlTpcCmdUint16;

/*
 * 枚举说明: 时隙测量任务
 */
enum GPHY_TOOL_TimeslotMeasType {
    GPHY_TOOL_TS_NO_MEAS = 0,                /* 0 当前时隙无测量任务 */
    GPHY_TOOL_TS_BA_MEAS,                    /* 1 当前时隙为BA测量 */
    GPHY_TOOL_TS_CELLSELECT_MEAS,            /* 2 当前时隙为小区选择测量 */
    GPHY_TOOL_TS_FULLLIST_MEAS,              /* 3 当前时隙为全频点测量 */
    GPHY_TOOL_TS_INTERFERE_MEAS,             /* 4 当前时隙为干扰测量 */
    GPHY_TOOL_TS_SCELL_BCCH_SLOW_GAIN_MEAS,  /* 5 服务区BCCH接收增益测量 */
    GPHY_TOOL_TS_SCELL_CCCH_SLOW_GAIN_MEAS,  /* 6 服务区CCCH接收增益测量 */
    GPHY_TOOL_TS_SCELL_PBCCH_SLOW_GAIN_MEAS, /* 7 服务区PBCCH接收增益测量 */
    GPHY_TOOL_TS_SCELL_PCCCH_SLOW_GAIN_MEAS, /* 8 服务区PCCCH接收增益测量 */
    GPHY_TOOL_TS_SCELL_CBCH_SLOW_GAIN_MEAS,  /* 9 服务区CBCH接收增益测量 */
    GPHY_TOOL_TS_SCELL_TCH_GAIN_MEAS,        /* 10 服务区CS专用态接收增益测量 */
    GPHY_TOOL_TS_SCELL_PDTCH_GAIN_MEAS,      /* 11 服务区PS专用态pdtch接收增益测量 */
    GPHY_TOOL_TS_SCELL_PTCCH_GAIN_MEAS,      /* 12 服务区PS专用态ptcch接收增益测量 */
    GPHY_TOOL_TS_SCELL_SYNC_SLOW_GAIN_MEAS,  /* 13 服务区同步慢速增益测量 */
    GPHY_TOOL_TS_SCELL_BCCH_FAST_GAIN_MEAS,  /* 14 服务区BCCH接收前快速增益获得测量 */
    GPHY_TOOL_TS_SCELL_CCCH_FAST_GAIN_MEAS,  /* 15 服务区CCCH接收前快速增益获得测量 */
    GPHY_TOOL_TS_SCELL_PBCCH_FAST_GAIN_MEAS, /* 16 服务区PBCCH接收前快速增益获得测量 */
    GPHY_TOOL_TS_SCELL_PCCCH_FAST_GAIN_MEAS, /* 17 服务区BCCH接收前快速增益获得测量 */
    GPHY_TOOL_TS_SCELL_CBCH_FAST_GAIN_MEAS,  /* 18 服务区CBCH接收前快速增益获得测量 */
    GPHY_TOOL_TS_SCELL_SYNC_FAST_GAIN_MEAS,  /* 19 服务区同步速增益测量 */
    GPHY_TOOL_TS_NCELL_BCCH_FAST_GAIN_MEAS,  /* 20 邻区接收前快速增益获得测量 */
    GPHY_TOOL_TS_NCELL_BCCH_SLOW_GAIN_MEAS,  /* 21 邻区接收慢速测量 */
    GPHY_TOOL_TS_NCELL_SYNC_FAST_GAIN_MEAS,  /* 22 邻区同步前快速增益获得测量 */
    GPHY_TOOL_TS_NCELL_SYNC_SLOW_GAIN_MEAS,  /* 23 邻区同步慢速测量 */
    GPHY_TOOL_TS_BA_MEAS_SBCCH_MEAS,         /* 24 存在PB时BA测量模块的服务区BCCH测量 */
    GPHY_TOOL_TS_CAL_MEAS,                   /* 25 校准测量 */
    GPHY_TOOL_TS_WCDMA_MEAS,                 /* 26 当前时隙为异系统测量 */
    GPHY_TOOL_TS_BG_TASK,                    /* 27 G背景搜索测量 */
    GPHY_TOOL_TS_DCXO_TASK,                  /* 28 DCXO RA delay期间的测量类型 */
    GPHY_TOOL_TS_DUAL_ANT_PRI_ANT_MEAS,      /* 29 双天线切换主天线测量 */
    GPHY_TOOL_TS_DUAL_ANT_SEC_ANT_MEAS,      /* 30 双天线切换辅天线测量 */
    GPHY_TOOL_TS_NOISE_TOOL_MEAS,            /* 31 噪声工具测量 */
    GPHY_TOOL_TS_PSRCH_PCH_FAST_GAIN_MEAS,   /* 32 并行搜寻呼接收前快速增益获得测量 */
    GPHY_TOOL_TS_PSRCH_PCH_SLOW_GAIN_MEAS,   /* 33 并行搜寻呼接收慢速测量 */

    GPHY_TOOL_TIMESLOT_MEAS_TYPE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_TimeslotMeasTypeUint16;

/*
 * DC维护类型枚举
 */
enum UPHY_TOOL_Dcoffset {
    UPHY_TOOL_DCOFFSET_UPDATE_INIT   = 0,
    UPHY_TOOL_DCOFFSET_UPDATE_REMAIN = 1,

    UPHY_TOOL_DCOFFSET_UPDATE_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_DcoffsetUint16;

/*
 * 枚举说明: 从模测量的小区搜索状态
 */
enum WPHY_TOOL_SlaveCsStatus {
    WPHY_TOOL_SLAVE_CS_STATUS_NONE = 0, /* 任务不存在 */
    WPHY_TOOL_SLAVE_CS_STATUS_READY,    /* 任务就绪 */
    WPHY_TOOL_SLAVE_CS_STATUS_RUNNING,  /* 任务正在运行 */
    WPHY_TOOL_SLAVE_CS_STATUS_FINISH
};
typedef VOS_UINT16 WPHY_TOOL_SlaveCsStatusUint16;

/*
 * 枚举说明: 从模测量的测量搜索状态
 */
enum WPHY_TOOL_SlaveMeasStatus {
    WPHY_TOOL_SLAVE_MEAS_STATUS_NONE = 0, /* 任务初始状态 */
    WPHY_TOOL_SLAVE_MEAS_STATUS_READY,    /* 任务就绪 */
    WPHY_TOOL_SLAVE_MEAS_STATUS_RUNNING,  /* 任务正在运行 */
    WPHY_TOOL_SLAVE_MEAS_STATUS_FINISH
};
typedef VOS_UINT16 WPHY_TOOL_SlaveMeasStatusUint16;

/*
 * 枚举说明: RF及前端工作场景枚举
 */
enum WPHY_TOOL_MntnRfOperScene {
    WPHY_TOOL_DRV_CS_AND_FM               = 0,  /* 小区搜索和频率测量 */
    WPHY_TOOL_DRV_IDLEPCH_MEAS            = 1,  /* 空闲模式与PCH状态下的异频测量 */
    WPHY_TOOL_DRV_DCH_MEAS                = 2,  /* DCH状态下的异频测量 */
    WPHY_TOOL_DRV_FACH_MEAS               = 3,  /* FACH状态下的异频测量 */
    WPHY_TOOL_DRV_INTER_FREQ_PCCPCH       = 4,  /* 异频广播解调 */
    WPHY_TOOL_DRV_CAMP_ON                 = 5,  /* 驻留 */
    WPHY_TOOL_DRV_HARD_HAND_OVER          = 6,  /* 异频硬切换 */
    WPHY_TOOL_DRV_RETURN_TO_INTRAFREQ     = 7,  /* 返回本频 */
    WPHY_TOOL_DRV_DRX_SLEEP_TO_AWAKE      = 8,  /* 进入唤醒状态 */
    WPHY_TOOL_DRV_DRX_AWAKE_TO_SLEEP      = 9,  /* 进入睡眠状态 */
    WPHY_TOOL_DRV_W_BG_INTERFREQ_RES      = 10, /* BG搜频点切换 */
    WPHY_TOOL_DRV_W_RX_DC_MODE_CHANGE     = 11, /* RX SC/DC状态切换标志 */
    WPHY_TOOL_DRV_W_TX_DC_MODE_CHANGE     = 12, /* TX SC/DC状态切换标志 */
    WPHY_TOOL_DSDS_CPICH_RESUME_RF        = 13, /* WDSDS CPICH态RF资源恢复 */
    WPHY_TOOL_DSDS_CONNECT_RESUME_RF      = 14, /* WDSDS FACH和DCH态RF资源恢复 */
    WPHY_TOOL_DSDS_W_RX_DC_MODE_RESUME_RF = 15, /* WDSDS DC RF资源恢复 */
    WPHY_TOOL_DSDS_PREEMPED_NO_RF         = 16, /* DSDS场景下资源被抢占后没有任何资源的时候可维可测用 */
    WPHY_TOOL_DRV_RF_DRX_LTE_MEAS         = 17,
    WPHY_TOOL_SLAVE_RF_MEAS_W             = 18, /* 从模测量，不需要拉IDLE_END */
    WPHY_TOOL_SLAVE_RF_BG_SEARCH          = 19, /* 背景搜和解广播 */
    WPHY_TOOL_CPICH_INTRA                 = 20, /* CPICH态时隙中断开射频 */

    WPHY_TOOL_DRV_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRfOperSceneUint16;

/*
 * 枚举说明: 通道号
 */
enum WPHY_TOOL_MntnChannel {
    WPHY_MNTN_MASTER_CHANNEL = 0, /* 降功率 */
    WPHY_MNTN_SLAVE_CHANNEL  = 1, /* 升功率 */
    WPHY_MNTN_CHANNEL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnChannelUint16;

/*
 * 枚举说明: 通道号
 */
enum GPHY_TOOL_MntnChannel {
    GPHY_MNTN_MASTER_CHANNEL = 0, /* 降功率 */
    GPHY_MNTN_SLAVE_CHANNEL  = 1, /* 升功率 */
    GPHY_MNTN_CHANNEL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnChannelUint16;

/*
 * 枚举说明: DCXO事件打印
 */
enum UPHY_TOOL_MntnDcxoEvent {
    UPHY_TOOL_MNTN_DCXO_1T_CELL_SEARCH = 0, /* W第一次搜网 */
    UPHY_TOOL_MNTN_DCXO_2T_CELL_SEARCH,     /* W第二次搜网 */
    UPHY_TOOL_MNTN_DCXO_3T_CELL_SEARCH,     /* W第三次搜网 */
    UPHY_TOOL_MNTN_DCXO_4T_CELL_SEARCH,     /* W第四次搜网 */
    UPHY_TOOL_MNTN_DCXO_5T_CELL_SEARCH,     /* W第五次搜网 */

    UPHY_TOOL_MNTN_DCXO_RD_PPM_FROM_OTHER_MODE, /* 从其它模获取频偏 */
    UPHY_TOOL_MNTN_DCXO_WR_PPM_TO_OTHER_MODE,   /* 频偏写给其它模 */

    UPHY_TOOL_MNTN_DCXO_DSDS_RD_PPM_FROM_OTHER_MODE, /* 从其它模获取频偏 */
    UPHY_TOOL_MNTN_DCXO_DSDS_WR_PPM_TO_OTHER_MODE,   /* 频偏写给其它模 */

    UPHY_TOOL_MNTN_DCXO_RD_RCM_TEMP_COMP,           /* 从CTU获取RCM写的温补值 */
    UPHY_TOOL_MNTN_DCXO_GET_MULTI_CELL_SEARCH_TIME, /* 计算当前搜网次数 */
    UPHY_TOOL_MNTN_DCXO_EVENT_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MntnDcxoEventUint16;

/*
 * 枚举说明: C模modemId枚举
 */
enum CSDR_ModeId {
    CSDR_MODE_ID_0 = 0, /* 卡0 */
    CSDR_MODE_ID_1 = 1, /* 卡1 */
    CSDR_MODE_ID_2 = 2, /* 卡2 */

    CSDR_MODE_ID_BUTT
};
typedef VOS_UINT16 CSDR_ModeIdUint16;

/*
 * 枚举说明: C模1X modemType枚举
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
 * 枚举说明: C模HRPD modemType枚举
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
 * 枚举说明: 上行增强随机接入控制状态
 */
enum WPHY_TOOL_MntnErachCtrlState {
    WPHY_MNTN_ERACH_CTRL_STATE_INVALID = 0,   /* UL ERACH未建立状态 */
    WPHY_MNTN_ERACH_CTRL_STATE_INIT,          /* UL ERACH初始态 */
    WPHY_MNTN_ERACH_CTRL_STATE_WAIT_AI,       /* UL ERACH等待AI中断态 */
    WPHY_MNTN_ERACH_CTRL_STATE_FORBID_SEND,   /* UL ERACH禁发数状态 */
    WPHY_MNTN_ERACH_CTRL_STATE_ALLOW_SEND,    /* UL ERACH可发数状态 */
    WPHY_MNTN_ERACH_CTRL_STATE_COLLISION_SUC, /* UL ERACH冲突解决成功后状态 */
    WPHY_MNTN_ERACH_CTRL_STATE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErachCtrlStateUint16;

/*
 * 枚举说明: 上行增强随机接入原语接收状态
 */
enum WPHY_TOOL_MntnErachPrimitiveStatus {
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_INVALID = 0,  /* 指示未收到上行增强随机接入原语,不包含MACI-CFG原语 */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_RLSETUP,      /* 指示收到 RRC_PHY_RL_SETUP_REQ(UL-EFACH) */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_RLMODIFY,     /* 指示收到 RRC_PHY_RL_MODIFY_REQ(UL-EFACH) */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_F2D_RLMODIFY, /* 指示收到 RRC_PHY_RL_MODIFY_REQ (FDPCH，F态->D态 */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_RLRELEASE,    /* 指示收到 RRC_PHY_RL_RELEASE_REQ(UL-EFACH) */
    WPHY_MNTN_ERACH_PRIMITIVE_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErachPrimitiveTypeUint16;

/*
 * 枚举说明: 逻辑信道类型
 */
enum WPHY_TOOL_MntnUlEfachLochType {
    WPHY_MNTN_UL_EFACH_LOCH_TYPE_CCCH      = 1, /* 上行增强FACH发送使用CCCH逻辑信道类型 */
    WPHY_MNTN_UL_EFACH_LOCH_TYPE_DCCH_DTCH = 2, /* 上行增强FACH发送使用DCCH逻辑信道类型 */

    WPHY_MNTN_UL_EFACH_LOCH_TYPE_BUTT /* 最后一个枚举值 */
};
typedef VOS_UINT16 WPHY_TOOL_MntnUlEfachLochTypeUint16;

/*
 * 枚举说明: PI结果
 */
enum WPHY_TOOL_MntnRachRslt {
    WPHY_MNTN_RACH_INVALID = 0,  /* 无效值 */
    WPHY_MNTN_RACH_OK,           /* bit[0]：接入成功 */
    WPHY_MNTN_RACH_NACK,         /* bit[1]：NACK */
    WPHY_MNTN_RACH_RETRANS_FAIL, /* bit[2]：达到重传次数而失败 */
    WPHY_MNTN_RACH_PWR_FAIL,     /* bit[3]：超过发射功率而失败 */
    WPHY_MNTN_RACH_RSLT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRachRsltUint16;

/*
 * 枚举说明: 上行增强随机接入原语接收状态
 */
enum WPHY_TOOL_MntnErachEaiType {
    WPHY_MNTN_ERACH_EAI_POSITIVE = 0, /* EAI结果为 +1 */
    WPHY_MNTN_ERACH_EAI_NEGATIVE,     /* EAI结果为 -1 */
    WPHY_MNTN_ERACH_EAI_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnErachEaiTypeUint16;

/*
 * 枚举说明: 上报GSDR判定开启/关闭状态
 */
enum GPHY_TOOL_MntnGsdrJudgeType {
    GPHY_MNTN_GSDR_JUDGE_DAIC_ON  = 0, /* 判断开启GSDR */
    GPHY_MNTN_GSDR_JUDGE_DAIC_OFF = 1, /* 判断关闭GSDR */
    GPHY_MNTN_GSDR_JUDGE_DAIC_FAST_ON,

    GPHY_MNTN_GSDR_JUDGE_OFF_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnGsdrJudgeTypeUint16;

/*
 * 枚举说明: WDSP RF Reset
 */
enum WDSP_MNTN_ErrorRptRfreset {
    WPHY_MNTN_RFRESET_DETECT_EVENT = 0,
    WPHY_MNTN_RFRESET_EXECTUE_EVENT,
    WPHY_MNTN_RFRESET_BUTT_EVENT
};
typedef VOS_UINT16 WPHY_TOOL_MntnRfresetEnentUint16;

/*
 * 枚举说明: 重选定时器类型
 */
enum GPHY_TOOL_NcellTimerId {
    GPHY_TOOL_NCELL_TIMER_C1 = 0, /* c1定时器 */
    GPHY_TOOL_NCELL_TIMER_C2,     /* c2定时器 */
    GPHY_TOOL_NCELL_TIMER_C2_ALL, /* 操作所有的邻区c2定时器 */

    GPHY_TOOL_NCELL_TIMER_ID_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_NcellTimerIdUint16;

/*
 * 枚举说明: c1、c2定时器启停状态
 */
enum GPHY_TOOL_NcellTimerState {
    GPHY_TOOL_NCELL_TIMER_START = 0, /* 定时器启动 */
    GPHY_TOOL_NCELL_TIMER_STOP,      /* 定时器停止 */

    GPHY_TOOL_NCELL_TIMER_STATE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_NcellTimerStateUnit16;
/*
 * 枚举说明: tool维测消息中专用的主集分集枚举定义,用于数组下标等应用;
 *          含义同UCOM_NV_PATH_MAIN_DIVERSITY_ENUM_UINT16
 */
enum GPHY_TOOL_MntnPathMainDiversity {
    GPHY_MNTN_PATH_MAIN      = 0, /* main path */
    GPHY_MNTN_PATH_DIVERSITY = 1, /* diversity path */
    GPHY_MNTN_PATH_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnPathMainDiversityUint16;

/*
 * 枚举说明: 采数与通道相关的寄存器内容补齐信息
 */
enum UPHY_TOOL_SampleRegComplete {
    UPHY_TOOL_SAMPLE_REG_CHN_UNRELATED = 0, /* 寄存器与通道无关 */
    UPHY_TOOL_SAMPLE_REG_DDR_VALUE,         /* ddr相关的寄存器值补齐 */
    UPHY_TOOL_SAMPLE_REG_BUS_DATA_CHN_ADDR, /* bus、data采数通道地址寄存器 */
    UPHY_TOOL_SAMPLE_REG_BUS_DATA_CHN_SIZE, /* bus、data采数通道size */
    UPHY_TOOL_SAMPLE_REG_BUFFER_INIT,       /* buffer 初始化 */
    UPHY_TOOL_SAMPLE_REG_DUMP_START_ADDR,   /* dump采数开始地址补齐 */
    UPHY_TOOL_SAMPLE_REG_DUMP_STOP_ADDR,    /* dump采数结束地址补齐 */

    UPHY_TOOL_SAMPLE_REG_COMPLETE_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_SampleRegCompleteUint16;

/*
 * 枚举说明: EDRX T321状态改变的原因
 */
enum WPHY_TOOL_MntnT321CtrlCause {
    WPHY_MNTN_T321_CTRL_PRIMITIVE      = 0, /* 原语原因 */
    WPHY_MNTN_T321_CTRL_EDCH_ALLOC     = 1, /* EDCH分配 */
    WPHY_MNTN_T321_CTRL_EDCH_REL       = 2, /* EDCH释放 */
    WPHY_MNTN_T321_CTRL_HSDSCH         = 3, /* HSDSCH数据接收 */
    WPHY_MNTN_T321_CTRL_EXPIRE         = 4, /* T321超时 */
    WPHY_MNTN_T321_CTRL_EXPIRE_RESTART = 5, /* T321超时后立刻重启，对应处理分支:超时时无fach资源且interrupt为true */
    WPHY_MNTN_T321_CTRL_RF_PREEMPT     = 6, /* rf资源抢占 */
    WPHY_MNTN_T321_CTRL_RF_RESUME      = 7, /* rf资源恢复 */
    WPHY_MNTN_T321_CTRL_ACCESS         = 8, /* 随机接入 */
    WPHY_MNTN_T321_CTRL_OTHER          = 9, /* 其他原因 */

    WPHY_MNTN_T321_CTRL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT321CtrlCauseUint16;

/*
 * 枚举说明: EDRX T321状态
 */
enum WPHY_TOOL_MntnT321CtrlStatus {
    WPHY_MNTN_T321_START  = 0, /* 原语原因 */
    WPHY_MNTN_T321_STOP   = 1, /* EDCH分配 */
    WPHY_MNTN_T321_EXPIRE = 2, /* EDCH释放 */

    WPHY_MNTN_T321_CTRL_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnT321CtrlStatusUint16;

/*
 * 枚举说明: GPHY负载异常场景
 */
enum GPHY_TOOL_MntnTimeOutType {
    GPHY_MNTN_TIME_OUT_TYPE_GTC_CONFIG       = 0, /* GTC配置任务超时 */
    GPHY_MNTN_TIME_OUT_TYPE_FN_CONFIG        = 1, /* FN配置任务超时 */
    GPHY_MNTN_TIME_OUT_TYPE_DRX_LATE         = 2, /* 唤醒的时间点QB已经无法满足即时翻页的要求 */
    GPHY_MNTN_TIME_OUT_TYPE_WAKE_SLICE_ERROR = 3, /* 唤醒时间错误 */

    GPHY_MNTN_TIME_OUT_TYPE_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MntnTimeOutTypeUint16;

/*
 * 枚举说明: WPHY负载异常场景
 */
enum WPHY_TOOL_MntnTimeOutType {
    WPHY_MNTN_TIME_OUT_LATE_SLOT12_TASK_EXPIRED  = 0, /* slot12负载异常 */
    WPHY_MNTN_TIME_OUT_TYPE_UPACC_TIME_EXCEPTION = 1, /* UPACC启动前调度时间异常 */
    WPHY_MNTN_TIME_OUT_TYPE_SLAVE_SCHEDULE       = 2, /* 从模调度时间超时 */
    WPHY_MNTN_TIME_OUT_TYPE_DELTA_SFN_ERR        = 3, /* 时间间隔超过门限 */
    WPHY_MNTN_TIME_OUT_TYPE_CLK_SWITCH_DELAY     = 4, /* 时钟切换延迟超时 */
    WPHY_MNTN_TIME_OUT_TYPE_PICH_SETUP_DELAY     = 5, /* PICH建立delay */

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
    WPHY_MNTN_CIQ_INFO_CNT_STOPPED = 0, /* 消息还没计数 */
    WPHY_MNTN_CIQ_INFO_CNT_RUNNING,
    WPHY_MNTN_CIQ_INFO_CNT_UNKNOWN
};
typedef VOS_UINT16 WPHY_MNTN_CiqInfoCntStatusUint16;

/*
 * 枚举说明: 从模测量的测量搜索状态
 */
enum WPHY_TOOL_MntnSlaveMeasFinishFlag {
    WPHY_MNTN_SLAVE_MEAS_FINISH_NONE        = 0, /* 初始无效状态 */
    WPHY_MNTN_SLAVE_MEAS_FINISH_SINGLE_FREQ = 1, /* 单频点测量完成 */
    WPHY_MNTN_SLAVE_MEAS_FINISH_ALL_FREQ    = 2, /* 全部频点测量完成 */
    WPHY_MNTN_SLAVE_MEAS_FINISH_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSlaveMeasFinishFlagUint16;

/*
 * 枚举说明: 记录当前唤醒类型
 */
enum WPHY_MNTN_WakeCntType {
    WPHY_MNTN_WAKE_CNT_TYPE_INIT = 0,          /* 睡眠 */
    WPHY_MNTN_WAKE_CNT_TYPE_MASTER_WAKE_NOMAL, /* 主模正常唤醒 */
    WPHY_MNTN_WAKE_CNT_TYPE_MASTER_WAKE_FORCE, /* 主模强制唤醒 */
    WPHY_MNTN_WAKE_CNT_TYPE_SLAVE_WAKE,        /* 从模唤醒 */
    WPHY_MNTN_WAKE_CNT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_MNTN_WakeCntTypeUint16;

/*
 * 枚举说明:从模小区搜索状态
 */
enum WPHY_TOOL_MntnSlaveCsStatus {
    WPHY_TOOL_MNTN_SLAVE_CS_INVALID         = 0x0,
    WPHY_TOOL_MNTN_SLAVE_CS_STEPAB_RUNNING  = 0x1, /* 从模两步法运行标记 */
    WPHY_TOOL_MNTN_SLAVE_CS_STEPAEB_RUNNING = 0x2, /* 从模step1+Eb运行标记 */
    WPHY_TOOL_MNTN_SLAVE_CS_STEP23_RUNNING  = 0x4, /* 从模stepEB或者step23运行标记 */
    WPHY_TOOL_MNTN_SLAVE_CS_FINISH          = 0x8, /* 从模小区搜索完成标记 */
    WPHY_TOOL_MNTN_SLAVE_CS_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MntnSlaveCsStatusUint16;

/*
 * 枚举说明: DCH态AFC RESET状态枚举
 */
enum WPHY_MNTN_DchAfcResetStatus {
    WPHY_MNTN_DCH_AFC_RESET_STATUS_CLOSE = 0, /* AFC RESET方案关闭 */
    WPHY_MNTN_DCH_AFC_RESET_STATUS_OPEN  = 1, /* AFC RESET方案打开 */
    WPHY_MNTN_DCH_AFC_RESET_STATUS_RESET = 2, /* 频偏较大场景执行AFC RESET */
    WPHY_MNTN_DCH_AFC_RESET_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_MNTN_DchAfcResetStatusUint16;

/* easyrf相关接口,后续考虑单独定义头文件 */
/*
 * 枚举说明: 采数动作类型
 */
enum BBA_DBG_Action {
    BBA_DBG_ACTION_START = 0x0000, /* 采数启动 */
    BBA_DBG_ACTION_QUERY = 0x0001, /* 采数查询 */
    BBA_DBG_ACTION_STOP  = 0x0002, /* 采数停止 */

    BBA_DBG_ACTION_BUTT
};
typedef VOS_UINT16 BBA_DBG_ActionUint16;

/*
 * 枚举说明: 工具配置的采数类型枚举
 */
enum EASYRF_DBG_CfgType {
    EASYRF_DBG_CFG_TYPE_BBIC_DFE  = 0x0000, /* BBIC DFE IQ采数 */
    EASYRF_DBG_CFG_TYPE_RFIC0_DBG = 0x0001, /* RFIC0 DBG采数 */
    EASYRF_DBG_CFG_TYPE_RFIC0_BUS = 0x0002, /* RFIC0 BUS采数 */
    EASYRF_DBG_CFG_TYPE_RFIC0_MRX = 0x0003, /* RFIC0 MRX采数 */
    EASYRF_DBG_CFG_TYPE_RFIC1_DBG = 0x0004, /* RFIC1 DBG采数 */
    EASYRF_DBG_CFG_TYPE_RFIC1_BUS = 0x0005, /* RFIC1 BUS采数 */
    EASYRF_DBG_CFG_TYPE_RFIC1_MRX = 0x0006, /* RFIC1 MRX采数 */

    EASYRF_DBG_CFG_TYPE_BUTT
};
typedef VOS_UINT16 EASYRF_DBG_CfgTypeUint16;

/*
 * 枚举说明:EASYRF DEBUG模块的采数各种配置
 */
enum EASYRF_DBG_SampleAction {
    EASYRF_DBG_SAMPLE_REG_CONFIG    = 0x0000, /* 对应配置寄存器列表 */
    EASYRF_DBG_SAMPLE_REG_UPDATE    = 0x0001, /* 对应更新寄存器列表 */
    EASYRF_DBG_SAMPLE_REG_ENABLE    = 0x0002, /* 对应使能寄存器列表 */
    EASYRF_DBG_SAMPLE_REG_REPORT    = 0x0003, /* 对应查询寄存器列表 */
    EASYRF_DBG_SAMPLE_REG_PRE_STOP  = 0x0004, /* 对应预停止,不需要查询就可以停止的寄存器列表 */
    EASYRF_DBG_SAMPLE_REG_QUERY     = 0x0005, /* 对应芯片提供的特殊查询状态寄存器,为特定状态才能停止后面的列表 */
    EASYRF_DBG_SAMPLE_REG_POST_STOP = 0x0006, /* 对应查询后才可以停止的寄存器列表 */

    EASYRF_DBG_SAMPLE_REG_BUTT
};
typedef VOS_UINT16 EASYRF_DBG_SampleActionUint16;

/*
 * 枚举说明:EASYRF 寄存器回读使能控制
 */
enum EASYRF_OM_Switch {
    EASYRF_OM_SWITCH_OFF = 0x0,
    EASYRF_OM_SWITCH_ON  = 0x1,
    EASYRF_OM_SWITCH_BUTT
};
typedef VOS_UINT16 EASYRF_OM_SwitchUint16;

/*
 * 枚举说明:EASYRF 寄存器回读TX/RX控制
 */
enum EASYRF_OM_ReadTrx {
    EASYRF_OM_READ_TX   = 0x0,
    EASYRF_OM_READ_RX   = 0x1,
    EASYRF_OM_READ_TRX  = 0x2,
    EASYRF_OM_READ_BUTT
};
typedef VOS_UINT16 EASYRF_OM_ReadTrxUint16;

/*
 * 枚举说明:PD统计模式
 */
enum EASYRF_PD_Mode {
    EASYRF_PD_MODE_POWER = 0, /* PD统计功率模式 */
    EASYRF_PD_MODE_GAIN  = 1, /* PD统计GAIN模式 */
    EASYRF_PD_MODE_BUTT
};
typedef VOS_UINT16 EASYRF_PD_ModeUint16;

/*
 * 枚举说明:EASYRF 寄存器回读TX/RX控制
 */
enum EASYRF_PD_OmSwitch {
    EASYRF_PD_OM_OFF = 0x0,
    EASYRF_PD_OM_ON  = 0x1,
    EASYRF_PD_OM_BUTT
};
typedef VOS_UINT16 EASYRF_PD_OmSwitchUint16;

/*
 * 枚举说明:EASYRF RATMODE
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
 * 枚举说明:PA档位模式
 */
enum EasyrfOmTxPaModeEnum {
    EASYRF_OM_TX_PA_MODE_HIGH     = 0, /* PA高档位 */
    EASYRF_OM_TX_PA_MODE_MID      = 1, /* PA中档位 */
    EASYRF_OM_TX_PA_MODE_LOW      = 2, /* PA低档位 */
    EASYRF_OM_TX_PA_MODE_ULTRALOW = 3, /* PA超低档位 */
    EASYRF_OM_TX_PA_MODE_BUTT
};
typedef VOS_UINT16 EasyrfOmTxPaModeEnumUint16;

/*
 * 枚举说明: EASYRF配置类型,紧凑定义,先定义所有的单个任务,组合任务放在后部。
 *           注:和驱动层函数通过索引关联,枚举中各任务的顺序不能随意调整
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

    /* 以下是组合任务 */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_START_AGC      = 13, /* 下行NV RX START+AGC SET组合配置,NV加载由底层判断 */
    /* 下行RX STOP+RX START+AGC SET组合配置,NV加载由底层判断 */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_STOP_START_AGC = 14,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_RESUME_AGC     = 15, /* 下行RX RESUME + AGC SET组合配置,不需要NV加载 */
    /* 下行RX STANDBY + RESUME + AGC SET组合配置,不需要NV加载 */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_STANDBY_RESUME_AGC = 16,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_TX_START_APC = 17, /* 上行TX START + APC SET组合配置,NV加载由底层判断 */
    /* 下行RX START + AGC SET+上行TX START+APC组合配置,NV加载由底层判断 */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_TX_START_AGC_APC = 18,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_TX_STOP = 19, /* 下行RX STOP + TX STOP组合配置,不需要加载NV */
    /* 下行RX STOP+TX STOP+RX START+TX START+AGC SET+APC SET组合配置,NV加载由底层判断 */
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_RX_TX_STOP_RX_TX_START_AGC_APC = 20,
    WPHY_TOOL_AEASYRF_CONFIG_TYPE_GROUP_TX_STOP_START_APC = 21, /* 上行TX STOP+TX START+APC组合 */

    WPHY_TOOL_AEASYRF_CONFIG_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_AeasyrfConfigTypeUint16;

/*
 * 枚举说明: 指示RX是打开还是关闭状态
 */
enum WPHY_TOOL_AeasyrfRxStatus {
    WPHY_TOOL_AEASYRF_RX_STATUS_OFF = 0, /* RX关闭状态 */
    WPHY_TOOL_AEASYRF_RX_STATUS_ON  = 1, /* RX打开状态 */

    WPHY_TOOL_AEASYRF_RX_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_AeasyrfRxStatusUint16;

/*
 * 枚举说明: 指示TX是打开还是关闭状态
 */
enum WPHY_TOOL_AeasyrfTxStatus {
    WPHY_TOOL_AEASYRF_TX_STATUS_OFF = 0, /* TX关闭状态 */
    WPHY_TOOL_AEASYRF_TX_STATUS_ON  = 1, /* TX打开状态 */

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
    /* 往下待接着添加 */

    WPHY_TOOL_NO_SIGNAL_MNTN_TYPE_BUTT = 1 << 13,
};
typedef VOS_UINT16 WPHY_TOOL_NoSignalMntnTypeUint16;

/*
 * 枚举说明: 滑窗SAR算法更新限制功率的原因
 */
enum WPHY_TOOL_SwSarUpdateReason {
    WPHY_TOOL_SW_NORMAL_UPDATE             = 0,  /* 滑窗SAR算法正常子周期更新(40ms或者40ms的倍数)更新 */
    WPHY_TOOL_SW_ANT_CHANGE_INHERIT        = 1,  /* 因天线发生切换导致滑窗sarlimitAvgMax发生变化，SAR算法继承 */
    WPHY_TOOL_SW_NO_ANT_CHANGE_RESTART     = 2,  /* 非天线切换(sensorcase、modemcase、天线连接状态)导致sarlimitAvgMax发生变化，滑窗SAR算法重启 */
    WPHY_TOOL_SW_FREQ_CHANGE_RESTART       = 3,  /* 非天线切换(频点或频段切换)导致sarlimitAvgMax发生变化，滑窗SAR算法重启 */
    WPHY_TOOL_SW_REDUCTION_CHANGE_RESTART  = 4,  /* 非天线切换(SAR功率回退等级)导致sarlimitAvgMax发生变化，滑窗SAR算法重启 */
    WPHY_TOOL_SW_RESTART_SET_W_MASTER      = 5,  /* 设置W为主模的时候,滑窗SAR算法重启 */
    WPHY_TOOL_SW_RESTART_RELEASE_ALL       = 6,  /* 在释放的地方,滑窗SAR算法重启， */
    WPHY_TOOL_SW_RESTART_RF_RESUME         = 7,  /* 在DCH/FACH态抢占恢复的地方,滑窗SAR算法重启， */
    WPHY_TOOL_SW_REASON_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_SwSarUpdateReasonUint16;

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
 * 6.0 STRUCT定义,WG公共定义
 */

/*
 * 枚举说明: 工具解析结构头
 */

typedef struct {
    VOS_UINT16 funcType; /* 可维可测是0x4,内部消息抓取是0x */
    VOS_UINT16 length;   /* 之后的长度,单位为byte */
    VOS_UINT32 sn;
    VOS_UINT32 timeSlice;
} UPHY_TOOL_TypeHeader;

/*
 * 枚举说明: 消息头结构定义
 */
typedef struct {
    VOS_UINT16 msgId;
    VOS_UINT16 reserved;
    VOS_UINT16 data[4];
} UPHY_TOOL_OmMsgHeader;

/*
 * 枚举说明: 消息头结构定义
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    VOS_UINT16 reserved;
} UPHY_TOOL_IntraMsgHeader;

/*
 * 结构说明: gu公共维测信息上报开关，后续可在这里加
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;            /* 保留字段 */
    UPHY_TOOL_MntnRptStatusUint16 uphySocpMntnStatus; /* 定时的SOCP周期维测上报使能标记 */
    VOS_UINT16                    uphySocpTimeMs;     /* 周期上报的可配置周期，单位ms，默认5s */
} UPHY_TOOL_MntnFuncSwitch;

/*
 * 6.1 STRUCT定义,UPHY定义
 */

/*
 * 结构说明: 通道号上报
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  toolsId;  /* 保留字段 */
    WPHY_TOOL_MntnChannelUint16 channel;  /* 默认每帧上报  */
    VOS_UINT16                  resurved; /* 保留字段 */
} WPHY_TOOL_ChannelReportInd;

/*
 * 结构说明: 通道号上报
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  toolsId;  /* 保留字段 */
    GPHY_TOOL_MntnChannelUint16 channel;  /* 默认每帧上报  */
    VOS_UINT16                  resurved; /* 保留字段 */
} GPHY_TOOL_ChannelReportInd;

/*
 * 结构说明: TOOL告诉GPHY,SDT连接状态
 */

/*
 * 结构说明: GPHY可维可测公共回复消息结构
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    UPHY_TOOL_MntnRsltUint16  result;  /* 接收原语的处理结果ID */
    VOS_UINT16                rsv;     /* 保留字段 */
} UPHY_TOOL_CommonCnf;

/*
 * 结构说明: 共享地址查询请求原语
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */     /* 原语类型 */
    VOS_UINT16 toolsId;   /* 保留字段 */
    VOS_UINT32 paraValue; /* 是否控制该参数 */
} UPHY_TOOL_QueryShareAddrReq;

/*
 * 结构说明: UPHY共享地址查询请求回复
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;   /* 保留字段 */
    VOS_UINT32                shareAddr; /* 共享首地址 */
} UPHY_TOOL_QueryShareAddrInd;

/*
 * 结构说明: TOOL告诉UPHY,SDT连接状态
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* 原语类型 */
    VOS_UINT16 toolsId; /* 保留字段 */
    VOS_UINT16 sdtStatus;
    VOS_UINT16 rsv;
} UPHY_TOOL_SdtLinkStatusReq;

/*
 * 结构说明: 错误上报
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;       /* 保留字段 */
    UPHY_TOOL_MntnRptLevelUint16 errorRptLevel; /* 控制上报错误级别 */
    VOS_UINT16                   rsv;
} UPHY_TOOL_ErrorReportReq;

/*
 * 结构说明: 错误上报
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                fileNo;  /* 文件名编码 */
    VOS_UINT16                lineNo;  /* 出错的行号 */
    VOS_UINT32                timeSlice;
    VOS_UINT32                logId;                                      /* PHY软件内统一编号LOG ID */
    VOS_UINT16                reserved;                                   /* 保留字段 */
    VOS_UINT16                varCnt;                                     /* 上报变量个数 */
    VOS_UINT32                varValue[UPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* 上报变量值 */
} UPHY_TOOL_ErrorReportInd;

/*
 * 结构说明: WPHY下异系统G寄存器列表下发
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* 原语类型 */
    VOS_UINT16 toolsId; /* 保留字段 */
    VOS_UINT16 rsv;     /* 控制上报错误级别 */
    VOS_UINT16 regNum;  /* 控制上报错误级别 */
    VOS_UINT32 regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_WphyInterGregReportReq;

/*
 * 结构说明: WPHY下异系统G寄存器收到回复
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    UPHY_TOOL_MntnRsltUint16  result;  /* 接收原语的处理结果ID */
    VOS_UINT16                regNum;  /* 控制上报错误级别 */
    VOS_UINT32                regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_WphyInterGregReportCnf;

/*
 * 结构说明: WPHY下异系统G寄存器值上报
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                rsv;     /* 控制上报错误级别 */
    VOS_UINT16                regNum;  /* 控制上报错误级别 */
    VOS_UINT32                regData[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_WphyInterGregReportInd;

/*
 * 结构说明: 停止WPHY下异系统G寄存器上报请求
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* 原语类型 */
    VOS_UINT16 toolsId; /* 保留字段 */
    VOS_UINT32 rsv;     /* 控制上报错误级别 */
} UPHY_TOOL_WphyInterGregStopReq;

/*
 * 结构说明: GPHY下异系统W寄存器列表下发
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                rsv;     /* 控制上报错误级别 */
    VOS_UINT16                regNum;  /* 控制上报错误级别 */
    VOS_UINT32                regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_GphyInterWregReportReq;

/*
 * 结构说明: GPHY下异系统W寄存器收到回复
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    UPHY_TOOL_MntnRsltUint16  result;  /* 接收原语的处理结果ID */
    VOS_UINT16                regNum;  /* 控制上报错误级别 */
    VOS_UINT32                regAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_GphyInterWregReportCnf;

/*
 * 结构说明: GPHY下异系统W寄存器值上报
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                rsv;     /* 控制上报错误级别 */
    VOS_UINT16                regNum;  /* 控制上报错误级别 */
    VOS_UINT32                regData[UPHY_TOOL_INTER_REG_MAX_NUM];
} UPHY_TOOL_GphyInterWregReportInd;

/*
 * 结构说明: 停止GPHY下异系统W寄存器上报请求
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */   /* 原语类型 */
    VOS_UINT16 toolsId; /* 保留字段 */
    VOS_UINT32 rsv;     /* 控制上报错误级别 */
} UPHY_TOOL_GphyInterWregStopReq;

/*
 * 结构说明: 通道号上报
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                       toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRfresetEnentUint16 abnormalEvent;
    VOS_UINT16                       abnormalRfChn;
    VOS_UINT16                       rfCheckType;
    VOS_UINT16                       abnEventCnt;
} WPHY_TOOL_RfErrorResetReport;

/*
 * 结构说明: 定时信息
 */
typedef struct {
    VOS_UINT16 sfn;  /* 系统定时SFN值 */
    VOS_UINT16 cfn;  /* 系统定时CFN值 */
    VOS_UINT16 slot; /* 系统定时SLOT值 */
    VOS_UINT16 chip; /* 系统定时CHIP值 */
#if (FEATURE_EASYRF == FEATURE_ON)
    VOS_UINT16 baseTimeSlot; /* 基准定时SLOT值 */
    VOS_UINT16 baseTimeChip; /* 基准定时CHIP值 */
    VOS_UINT32 sysCurrentMc; /* 定时MC值 */
#endif
} WPHY_TOOL_TimingInfo;

/*
 * 结构说明: WPHY DRX可维可测信息
 */
typedef struct {
    VOS_UINT32           forbidSleepReason; /* 禁止睡眠原因 */
    VOS_UINT32           sndSleepReqCnt;    /* WPHY 发送睡眠消息计数 */
    VOS_UINT32           sndSleepReqSlice;  /* WPHY 发送睡眠消息slice */
    VOS_UINT32           drxRcvSleepSlice;  /* DRX  收到睡眠消息slice */
    VOS_UINT32           drxSendSleepSlice; /* DRX  发送睡眠消息slice */
    VOS_UINT32           drxNeedWakeSlice;  /* DRX  预期唤醒slice */
    VOS_UINT32           drxRcvWakeSlice;   /* DRX  收到唤醒消息slice */
    VOS_UINT32           drxSendWakeSlice;  /* DRX  发送唤醒消息slice */
    VOS_UINT32           rcvWakeMsgCnt;     /* WPHY 接收到唤醒消息计数 */
    VOS_UINT32           rcvWakeMsgSlice;   /* WPHY 接收到唤醒消息slice */
    VOS_UINT32           dcStartSlice;      /* WPHY 启动DC稳定延时定时器slice */
    VOS_UINT32           dcEndSlice;        /* WPHY DC稳定定时器超时slice */
    VOS_UINT32           mpStartSlice;      /* WPHY 第一次多径搜索启动slice */
    VOS_UINT32           rcvMpSrchIntCnt;   /* WPHY 接收到多径搜索中断计数 */
    VOS_UINT32           rcvMpSrchIntSlice; /* WPHY 接收到多径搜索slice */
    VOS_UINT32           openSlotIntCnt;    /* WPHY 打开时隙中断计数 */
    VOS_UINT32           openSlotIntSlice;  /* WPHY 打开时隙中断slice */
    VOS_UINT32           pichSetupSlice;    /* WPHY PICH建立slice */
    VOS_UINT32           piIntSlice;        /* WPHY PI中断slice */
    WPHY_TOOL_TimingInfo piRealTime;        /* WPHY PI位置时间 */
    WPHY_TOOL_TimingInfo pichSetupTime;     /* WPHY PICH建立时间 */
} UPHY_TOOL_WphyDrxMntnInfo;

/*
 * 结构说明: 唤醒晚时的打点信息
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                rsv;       /* 保留字段 */
    VOS_UINT32                awakeTime; /* 需要唤醒的时间，W下为SFN，G下为Slice */
    VOS_UINT32                curTime;   /* 实际唤醒的时间，W下为SFN，G下为Slice */
    VOS_UINT32                awakeHWInfo[UPHY_TOOL_SLEEP_INFO_NUM]; /* OAM可维可测消息 */
    UPHY_TOOL_WphyDrxMntnInfo wphyDrxMntn;                           /* WPHY可维可测消息 */
} UPHY_TOOL_DrxLateInfoInd;

/*
 * 结构说明: 闭环调谐算法过程与结果可维可测上报
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;      /* 保留字段 */
    VOS_INT16                 gammaReal;    /* 反射系数量测结果的实部 */
    VOS_INT16                 gammaImag;    /* 反射系数量测结果的虚部 */
    VOS_UINT16                algState;     /* 算法状态 */
    VOS_UINT16                statusBitMap; /* 算法各流程状态位域标志 */
    VOS_UINT16                gridWinner;   /* 赢家格点，X在高8位，Y在低8位 */
    VOS_UINT16                gridTest;     /* 测试格点，X在高8位，Y在低8位 */
    VOS_UINT16                gridTrace[UPHY_TOOL_CLT_TRACE_NUM]; /* 逼近轨迹记录 */
    VOS_INT32                 fom;                                /* 当前FOM值 */
    VOS_INT32                 referenceFom;                       /* 参考FOM值 */
    VOS_UINT32                sliceSinceLastGoodFom;              /* 距离前次好的FOM的时间 */
    VOS_UINT32                sliceSinceLastHold;                 /* 距离前次保持收敛态的时间 */
    VOS_UINT32                sliceSinceLastFrozen;               /* 距离前次保持冻结态的时间 */
    VOS_UINT16                fomGoodThreshold;                   /* Good FOM门限 */
    VOS_UINT16                fomPoorThreshold;                   /* Poor FOM门限 */
    VOS_UINT16                fomHoldThreshold;                   /* Hold FOM门限 */
    VOS_UINT16                reserved;                           /* 预留 */
} RCM_TOOL_MntnCltAlgRpt;

/*
 * 6.2 STRUCT定义,GPHY定义
 */

/*
 * 结构说明: GPHY可维可测公共回复消息结构
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    GPHY_TOOL_MntnRsltUint16  result;  /* 接收原语的处理结果ID */
    VOS_UINT16                rsv;     /* 保留字段 */
} GPHY_TOOL_CommonCnf;

/*
 * 结构说明: GPHY GPRS信道的误码率上报请求
 */
typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;   /* 保留字段 */
    VOS_UINT16                blerFlag;  /* 是否上报BLER */
    VOS_UINT16                rptPeriod; /* BLER上报的周期,单位:块数 */
} GPHY_TOOL_BlerReportReq;

/*
 * 结构说明: GPHY上报BLER的结构和子结构
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
 * 结构说明: GPHY GPRS信道的SNR上报请求
 */
typedef struct {
    VOS_MSG_HEADER      /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16 toolsId; /* 保留字段 */
    VOS_UINT16 snrFlag; /* 是否上报SNR */
    VOS_UINT16 rsv;     /* 保留字段 */
} GPHY_TOOL_SnrReportReq;

/*
 * 结构说明: GPHY上报SNR的结构和子结构
 */

typedef struct {
    VOS_UINT16 rptValidNum;
    VOS_UINT16 rsv;
    VOS_INT16  snr[GPHY_PS_MAX_RX_TSN_NUM];
    VOS_UINT16 rsv2;
} GPHY_GPRS_SnrRecord;

/* 可维可测:SNR上报结构 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    VOS_UINT16                toolsId;
    GPHY_GPRS_SnrRecord       gprsSnrRecord;
} GPHY_TOOL_SnrReportInd;

/*
 * 结构说明: GPHY AFC积分值信息上报请求
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                afcFlag; /* 是否上报AFC值 */
    VOS_UINT16                rsv;     /* 保留字段 */
} GPHY_TOOL_AfcReportReq;

/*
 * 结构说明: GPHY上报AFC的结构
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    VOS_UINT16                toolsId;
    VOS_INT16                 afcOffset;
    VOS_UINT16                rsv;
} GPHY_TOOL_AfcReportInd;

/*
 * 结构说明: GPHY 数传下功控信息上报请求
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;     /* 保留字段 */
    VOS_UINT16                pwrCtrlFlag; /* 是否上报功率控制 */
    VOS_UINT16                rsv;         /* 保留字段 */
} GPHY_TOOL_PowerCtrlReportReq;

/*
 * 结构说明: GPHY上报功率控制参数的结构和子结构
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
 * 结构说明: GPHY 数传下指定功率发射设置请求
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;     /* 保留字段 */
    VOS_UINT16                pwrCtrlFlag; /* 是否按照指定功率发射 */
    VOS_UINT16                txPwr;       /* 指定功率,单位:0.1dB */
} GPHY_TOOL_PowerCtrlSetReq;

/*
 * 结构说明: GPHY上报EGPRS信道质量上报结构
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;     /* 保留字段 */
    VOS_UINT16                gmskMeanBep; /* 0-31,0xff为无效值 */
    VOS_UINT16                meanBep8psk; /* 0-31,0xff为无效值 */
    VOS_UINT16                gmskCvBep;   /* 0-7,0xff为无效值 */
    VOS_UINT16                cvBep8psk;   /* 0-7,0xff为无效值 */
} GPHY_TOOL_ChannelQualityInd;

/*
 * 结构说明: GPHY 可维可测功能控制下发的结构原语:用来控制功能设置请求
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;     /* 保留字段 */
    VOS_UINT32                funcBitMask; /* 对应Bit位置位,则控制该功能生效 */
} GPHY_TOOL_FuncBitCtrlReq;

/*
 * 结构说明: GPHY可维可测参数控制下发的结构原语:用来控制参数设置请求
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16               toolsId;     /* 保留字段 */
    GPHY_TOOL_ParaCtrlUint16 paraId;      /* 参数的ID,控制参数的列表 */
    VOS_UINT16               paraCtrlFlg; /* 是否控制该参数 */
    VOS_UINT32               paraValue1;  /* 控制参数的值 */
    VOS_UINT32               paraValue2;  /* 目前保留,便于扩展 */
} GPHY_TOOL_ParaControlReq;

/*
 * 结构说明: GPHY 上报前端动态信息上报请求
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                msgRptFlag; /* 指示消息监控的上报内容是否有变更 */
    VOS_UINT16                rsv;        /* 保留字段 */
} GPHY_TOOL_RfFrontStaticReq;

/*
 * 结构说明: RTC功能打开和关闭设置
 */
typedef struct {
    VOS_MSG_HEADER
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    UPHY_TOOL_RtcDebugUint16  rtcCtrl; /* RTC打开或者关闭 */
    VOS_UINT16                rsv;
} GPHY_TOOL_BbpRtcReq;

/*
 * 结构说明: GPHY上报前端动态信息上报
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    VOS_UINT16 rfTrcvOnState; /* RF总开关，1为打开，0为关闭 */
    VOS_UINT16 rfRxOnOrOff;   /* 04h[1:0] = 3为开 */
    VOS_UINT16 rxPathselect;  /* 04h[4:3] = 3为主分集都开，2开主集 */
    VOS_UINT16 abbRxConfig;   /* 7为主分集都开，3开主集 */

    VOS_UINT16 rfTxOnOrOff; /* 02h[1:0] = 3为开 */
    VOS_UINT16 rfTxShdn;    /* RF上行打开控制，0由BBP控制 */

    VOS_UINT16 gpioFuncOn; /* 硬件支持GPIO */
    VOS_UINT16 gpioPaMode; /* GPIO PA mode */

    VOS_UINT16 mipiFuncOn; /* 硬件支持MIPI */
    VOS_UINT16 mipiPaMode; /* MIPI PA mode */

    VOS_UINT16 aptFuncOn; /* 硬件支持APT */
    VOS_UINT16 aptPaMode; /* APT电压mode */
} GPHY_TOOL_RfFrontStaticInd;

/*
 * 结构说明: PHY 内部消息上报控制请求
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                msgRptFlag; /* 指示内部消息是否上报 */
    VOS_UINT16                rsv;        /* 保留字段 */
} GPHY_TOOL_IntraMsgRptCtrlReq;

/*
 * 结构说明: GPHY 语音下AMR信道编译码速率上报请求
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                amrRptFlag; /* 是否上报AMR信息 */
    VOS_UINT16                rsv;        /* 保留字段 */
} GPHY_TOOL_AmrReportReq;

/*
 * 结构说明: GPHY上报AMR信息参数
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                crcErrFlg;  /* 译码结果0-正确;1-错误 */
    VOS_UINT16                decodeMode; /* 下行编码方式 */
    VOS_UINT16                decodeRate; /* 下行编码速率 */
    VOS_UINT16                encodeMode; /* 上行编码方式 */
    VOS_UINT16                encodeRate; /* 上行编码速率 */
    VOS_UINT16                rsv;
} GPHY_TOOL_AmrReportInd;

/*
 * 结构说明: GPHY抓取非固定地址的数据
 */
typedef struct {
    VOS_UINT32 blkAddr; /* 抓取数据块的首地址 */
    VOS_UINT32 blkLen;  /* 抓取数据块的长度,单位:32bit */
} GPHY_TOOL_BlkReport;

typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                rptFlag;
    VOS_UINT16                rsv;
    GPHY_TOOL_BlkReport       rptBlk[GPHY_TOOL_RPT_BLK_MAX_NUM];
} GPHY_TOOL_BlkReportListReq;

/*
 * 结构说明: GPHY抓取非固定地址的数据
 */

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    GPHY_TOOL_BlkReport       rptBlk[GPHY_TOOL_RPT_BLK_MAX_NUM];
    VOS_UINT32                data[2];
} GPHY_TOOL_BlkReportListInd;

/*
 * 结构说明: GSM KEY RFIC REG Report
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
 * 结构说明: RX IQ IMBALANCE 维测消息结构
 */
typedef struct {
    VOS_UINT16 calBpFlag;     /* 估计是否打开的标记 0表示关闭，1表示打开。那么此次的上报的幅度相位值无效 */
    VOS_INT16  rptAm;         /* 本次上报的幅度值 */
    VOS_INT16  rptPhase;      /* 本次上报的相位 */
    VOS_INT16  currMainAm;    /* alpha滤波后的主集幅度值 */
    VOS_INT16  currMainPhase; /* alpha滤波后的主集相位值 */
    VOS_INT16  currDivAm;     /* alpha滤波后的分集幅度值 */
    VOS_INT16  currDivPhase;  /* alpha滤波后的分集相位值 */
    VOS_UINT16 rsv;           /* 保留 */
} GPHY_TOOL_RxIqImbalance;

/*
 * 结构说明: AGC档位 维测消息结构
 */
typedef struct {
    VOS_UINT16 meaFastFlag;     /* 是否为快速测量测量类型 */
    VOS_INT16  wbAgcRssi125dbm; /* 宽带能量 */
    VOS_UINT16 agcCurrGain;     /* 当前配置的档位 */
    VOS_UINT16 agcNextGain;     /* 下一次配置的档位 */
    VOS_UINT16 currBlockMode;   /* 当前blk 模式 */
    VOS_UINT16 nextBlockMode;   /* 估计出的下一次blk 模式 为0为noblk模式 为1为block模式  */
    VOS_UINT16 antPath;         /* 主集还是分集信息 */
    VOS_UINT16 rsv;             /* 保留 */
} GPHY_TOOL_AgcBlkNoblkMode;

/*
 * 结构说明: 单时隙接收信息的结构
 */
typedef struct {
    VOS_UINT16                       timeSlot;                             /* 上报测量中断时的时隙号 */
    VOS_UINT16                       reserved;                             /* 4字节对齐 */
    VOS_UINT32                       meaFn;                                /* 上报测量中断时的帧号 */
    VOS_UINT16                       meaArfcn;                             /* 上报测量的频点 */
    GPHY_TOOL_TimeslotMeasTypeUint16 tsMeasType;                           /* 测量类型 */
    VOS_UINT16                       nextAgcGain[GPHY_MNTN_PATH_BUTT];     /* AGC档位 */
    VOS_INT16                        asAgcRssi125dbm[GPHY_MNTN_PATH_BUTT]; /* 测量值:0.125dB */
    VOS_UINT32                drssi125dbm[GPHY_MNTN_PATH_BUTT]; /* DRSSI求对数运算前数值上报 0.125dbm -0x1000为负值 */
    VOS_UINT32                lsDcValue[GPHY_MNTN_PATH_BUTT];
    VOS_UINT32                grossCarrierEn;
    VOS_UINT32                grossInterfereEn;
    VOS_UINT32                valppCarrierEn;
    VOS_UINT32                valppInterfereEn;
    VOS_UINT16                nbPosAdj;
    VOS_UINT16                toaSelResult;
    GPHY_TOOL_RxIqImbalance   rxIqImbalance;
    GPHY_TOOL_AgcBlkNoblkMode agcBlkNoBlk[GPHY_MNTN_PATH_BUTT];
    VOS_UINT32                gspLineState;          /* 公共线控实际状态 */
    VOS_UINT32                extLineState;          /* EXT公共线控实际状态 */
    VOS_UINT32                crossMipiLineState;    /* Cross Mipi线控总输入状态 */
    VOS_UINT32                extCrossMipiLineState; /* EXT Cross Mipi线控总输入状态 */
} GPHY_TOOL_RcvBurstInfo;

/*
 * 结构说明: CTU通道模式信息
 */
typedef struct {
    VOS_UINT32 chnMasterModeRx; /* Channel master mode */
    VOS_UINT32 chnMasterModeTx; /* Channel master mode */
} GPHY_TOOL_CtuModeInfo;

/*
 * 结构说明: 单帧接收信息的结构
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT32                curFn;
    VOS_UINT32                bbpcommGsChSwAddr;      /* BBPCOMM_GS_CH_SW_ADDR  */
    VOS_UINT32                bbpcommPmuChSel0Addr;   /* BBPCOMM_PMU_CH_SEL0_ADDR */
    VOS_UINT32                bbpcommDbgReg20CpuAddr; /* BBPCOMM_DBG_REG20_CPU_ADDR */
    GPHY_TOOL_CtuModeInfo     ctuModeInfo;            /* CTU通道模式信息 */
    VOS_UINT16                vaildBurstNum;          /* 上报的有效时隙个数，为了匹配动态上报自动解析 */
    VOS_UINT16                reserved;               /* 保留位 */
    GPHY_TOOL_RcvBurstInfo    burstInfo[GPHY_TOOL_TN_NUM_IN_FRAME];
} GPHY_TOOL_RcvSingalInfoReq;

/*
 * 结构说明: G模出沟时上报分沟预处理、分沟时和出沟时的FN和QB信息
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;           /* 保留字段 */
    VOS_UINT32                fn[GPHY_GAP_BUTT]; /* 帧号 */
    VOS_UINT16                qb[GPHY_GAP_BUTT]; /* QB值 */
    VOS_UINT16                reserved1;         /* 保留字段 */
} GPHY_TOOL_GapOutTimeInfo;

/*
 * 结构说明: GPHY信息,告警,error上报结构
 */

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;   /* 保留字段 */
    VOS_UINT16                fileNo;    /* 文件名编码 */
    VOS_UINT16                lineNo;    /* 出错的行号 */
    VOS_UINT32                timeSlice;
    VOS_UINT32                logId;     /* PHY软件内统一编号LOG ID */
    VOS_UINT16                reserved;  /* 保留字段 */
    VOS_UINT16                varCnt;    /* 上报变量个数 */
    VOS_UINT32                varValue[GPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* 上报变量值 */
} GPHY_TOOL_ReportErrorInd;

/*
 * 结构说明: 空闲帧任务信息的结构
 */
typedef struct {
    VOS_UINT32                        taskFn;
    GPHY_NCE_PredictTaskTypeElfUint16 taskType;
    VOS_UINT16                        freqNum;                               /* 专用态IDLE帧做邻区任务的频点数目 */
    GPHY_NCE_OccasionUseModeElfUint16 occasionUseMode;                       /* 当前是哪个模式在使用 */
    VOS_UINT16                        freqInfo[GPHY_TOOL_MAX_SYNC_TASK_NUM]; /* 专用态IDLE帧最多可以做5个BSIC验证 */
} GPHY_TOOL_DchOccasionUseInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId; /* 保留字段 */
    VOS_UINT32                   curFn;
    VOS_UINT16                   timeOutCnt;
    VOS_UINT16                   occasionNum;
    GPHY_TOOL_DchOccasionUseInfo occasionUseInfo[GPHY_TOOL_MAX_IDLE_FRAME_NUM];
} GPHY_TOOL_DchOccasionUseRpt;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16              msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                             toolsId; /* 保留字段 */
    GPHY_NCE_NcellBsicSearchStateElfUint16 bsicSrchState;
    VOS_UINT16                             freq;
    VOS_UINT16                             rank;
    GPHY_NCE_NstrongNcellElfUint16         nStrongIndex;
    VOS_INT16                              rxlev125dbm;
    VOS_UINT16                             reserved;
} GPHY_TOOL_BsicSearchInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16              msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                             toolsId; /* 保留字段 */
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
 * 结构说明: GSM 邻区优先级信息
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
 * 结构说明: 异系统优先级信息
 */
typedef struct {
    VOS_UINT16 rank;
    VOS_UINT16 maxGapNum;      /* 最大可分配的沟的数目 */
    VOS_UINT16 assignedGapNum; /* 已分配的沟的数目 */
    VOS_UINT16 resv;
} GPHY_TOOL_InterratRankInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId; /* 保留字段 */
    GPHY_TOOL_InterratRankInfo interratRankInfo;
    GPHY_TOOL_NcellRankInfo    ncellRankInfo[GPHY_TOOL_MAX_NCELL_NUM];
} GPHY_TOOL_NcellInterratRankRpt;

/*
 * 结构说明: 在获取公共BBP资源操作权时的上报结构
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;   /* 保留字段 */
    VOS_UINT16                modemId;   /* 当前所处模式 */
    VOS_UINT16                resv;      /* 保留位 */
    VOS_UINT32                timeSlice; /* 当前时间戳 */
} GPHY_TOOL_BbpshareResSelectRpt;

/*
 * 结构说明: 对BBP公共资源操作时上报结构
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16      msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId;     /* 保留字段 */
    VOS_UINT16                     modemId;     /* 当前所处模式 */
    GPHY_TOOL_BbpOperateTypeUint16 operateType; /* 操作类型 */
    VOS_UINT32                     timeSlice;   /* 当前时间戳 */
    VOS_UINT32                     gbbpAddr;    /* 寄存器地址 */
    VOS_UINT32                     value;       /* 寄存器的值 */
} GPHY_TOOL_BbpshareRdWrError;

/*
 * 结构说明: 重选时c1、c2定时器启停状态查询
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16       msgId;           /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                      toolsId;         /* 保留字段 */
    GPHY_TOOL_NcellTimerIdUint16    ncellTimerId;    /* 定时器类型: c1、c2 */
    GPHY_TOOL_NcellTimerStateUnit16 ncellTimerState; /* 定时器状态: 启、停 */
    VOS_UINT32                      arfcn;           /* 定时器所在的频点。只在启动或停止一个c2定时器时生效 */
    VOS_UINT32                      countFnNum;      /* 记忆定时器需要计数多少帧。c2定时器全部停止时无效 */
    VOS_UINT32                      startFn;         /* 记忆定时器的起始帧号。c2定时器全部停止时无效 */
    VOS_UINT32                      passFnCnt;       /* 从起始帧号开始,已经经过了多少帧。c2定时器全部停止时无效 */
} GPHY_TOOL_NcellReselTimerState;

/*
 * 结构说明: 重选时c1、c2定时器启停状态查询
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16    msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;      /* 保留字段 */
    GPHY_TOOL_NcellTimerIdUint16 ncellTimerId; /* 定时器类型: c1、c2 */
    VOS_UINT16                   rsv;          /* 保留 */
} GPHY_TOOL_NcellReselTimerExpired;

typedef struct {
    VOS_UINT32 curFn;
    VOS_UINT16 curFnT2;
    VOS_UINT16 predictTask;
} GPHY_TOOL_NcellRecordPredictTask;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16        msgId;           /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                       toolsId;         /* 保留字段 */
    VOS_UINT16                       ncellBcchOccupy; /* 是否被邻区读系统广播消息占据 */
    VOS_UINT16                       srchRank;        /* 搜索频点的最高优先级 */
    VOS_UINT16                       interRatRank;    /* 异系统频点的最高优先级 */
    VOS_UINT16                       verifyRank;      /* 验证频点的最高优先级 */
    GPHY_TOOL_NcellRecordPredictTask recordPredictTask[GPHY_TOOL_NCELL_RPT_PREDICT_TASK_INFO_NUM];
} GPHY_TOOL_NcellPredictTaskRpt;

/*
 * 结构说明: 邻区验证和邻区搜索的任务信息，用于维测
 */
typedef struct {
    VOS_UINT16                        freqInfo;        /* 频点信息 */
    VOS_UINT16                        taskRemainCount; /* 当前频点任务存在的时长(寻呼周期的个数) */
    GPHY_NCE_NcellSyncActionElfUint16 ncellTaskType;   /* 标记是邻区搜索任务或验证任务 */
    VOS_UINT16                        gtcConfigState;  /* GTC任务配置的状态 */

    VOS_UINT32 fnOffset;    /* 验证--邻区和服务区的帧号偏差,模2715648 */
    VOS_UINT16 timeAlignmt; /* 验证--邻区和服务区的帧头偏差,模5000 */
    VOS_UINT16 reserved;    /* 保留位 */

    VOS_UINT16 bsicSrchReqOrgin; /* 搜索--邻区BSIC搜索任务来源: 协议栈请求或物理层内部调度 */
    VOS_UINT16 psSrchType;       /* 搜索--协议栈请求的bsic搜索类型: 背景搜或其它 */
    VOS_UINT32 initCfgNco;       /* 搜索--Nco 配置初始值 */
    VOS_UINT32 nceSycNcoValue;   /* 搜索--邻区频偏检测结果 */
} GPHY_TOOL_VerifySrchTaskInfo;

/*
 * 结构说明: 上报给工具的邻区任务结构体
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16    msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;        /* 保留字段 */
    VOS_UINT16                   ncellVerifyNum; /* 做验证任务的小区个数 */
    VOS_UINT16                   ncellSrchNum;   /* 做搜索任务的小区个数 */
    VOS_UINT16                   lastLockedFreq; /* 存储前一次保存的锁存频点 */
    VOS_UINT16                   reserved;       /* 保留位 */
    GPHY_TOOL_VerifySrchTaskInfo rptNcellTaskArray[GPHY_TOOL_MAX_NCELL_NUM]; /* 上报的ncell任务列表 */
} GPHY_TOOL_NcellTaskListIndRpt;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;      /* 保留字段 */
    VOS_UINT16                dummyPchFlag; /* 单帧解寻呼的检测结果 */
    VOS_UINT16                averageValue; /* 根据绝对值求和后的值取平均(根据算法链路扩大了16倍) */
    VOS_UINT32                fn;           /* 当前帧号 */
    VOS_INT32                 judgeDataC;   /* 计算出的中间值 */
    VOS_INT32                 judgeDataD;   /* 计算出的中间值 */
    VOS_INT32                 judgeDataE;   /* 计算出的中间值 */
    VOS_INT32                 judgeDataF;   /* 计算出的中间值 */
} GPHY_TOOL_ChnDummyPchRpt;

/*
 * 结构说明: SRVCC BSIC小区信息
 */
typedef struct {
    VOS_UINT16 arfcn;             /* 频点 */
    VOS_UINT16 rssiOrder;         /* 当前小区按照能量排序,在LRRC给出的列表中的能量排序. */
    VOS_UINT16 initSrchFailCnt;   /* 周期内初始确认失败次数. */
    VOS_UINT16 rrcReCfgFlag;      /* 失败小区如果LRRC重配 */
    VOS_UINT16 sbVerifiedFlag;    /* 在SB重确认周期内,小区是否已经完成验证标志 */
    VOS_UINT16 sbVerifyFailCount; /* 重确认小区重确认失败次数 */
    VOS_UINT16 bsicSucOrFailFlag; /* 同步是否成功 */
    VOS_UINT16 bsicCellType;      /* 同步类型 */
    VOS_UINT32 fbSearchLength;    /* FB搜索总长度 */
    VOS_UINT32 gSlaveOffsetFn;    /* 上一次搜索时得到的W/L/TD和G偏差的Fn */
    VOS_UINT16 gSlaveOffsetQb;    /* 上一次搜索时得到的W/L/TD和G偏差的Qb */
    VOS_UINT16 ncellToaOffset;    /* 相对于Timebase的偏差,由G FB搜索时提供该值,在SB搜索时使用 */
    VOS_UINT32 ncellFrameNum;     /* GSM 邻小区的帧号信息，邻区使用低16bitTDMA帧号低位0~26x51-1循环计数 */
} GPHY_TOOL_SrvccBsicRsltInfo;

typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;                   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                 /* 保留字段 */
    VOS_UINT16                gSlaveGapTask;           /* 一级任务:当前GAP分配的任务类型. */
    VOS_UINT16                gSlaveCfgType;           /* 二级任务:当前需要配置的类型. */
    VOS_UINT16                gRssiPeriodStartFlag;    /* GSM从模开始新一轮测量,测量完成后清除. */
    VOS_UINT16                srVccFbSrchIndex;   /* 执行初始确认的索引,在完成一个后索引置为无效.在下发任务时无效. */
    VOS_UINT16                periodFbCnt;        /* 周期性GAP周期性搜索FB次数 */
    VOS_UINT16                periodGapFlag;      /* 周期性GAP周期性搜索FB次数 */
    VOS_UINT16                srVccSbCellIndex;   /* 执行SB任务小区的索引,完成一个小区的SB验证后,设置为无效值 */
    VOS_UINT16                curBsicTaskType;    /* 当前是那种类型的任务优先. */
    VOS_UINT16                sbVerifyPeriorFlag; /* SB验证优先级是否高 */
    VOS_UINT16                offLineSaveIndex;   /* 离线译码当前存储的索引. */
    VOS_UINT16 offLineValidIndex; /* 有效索引的Index,存储多个间隔不一致,大于245ms的索引置为无效,无需在计算. */
    VOS_UINT16 bsicNum;    /* 原语下发的小区个数，初始确认重确认都在同一个表中，BSIC小区个数包括初始确认和重确认 */
    VOS_UINT16 calFreqNum; /* 配置RSSI测量小区个数 */
    VOS_UINT16 freqIndex;  /* RSSI小区索引 */
    VOS_UINT32 sbVerifyFbSrchTotalTime; /* 在SB验证期间,可能执行FB SRCH,在计算SB验证时间超时时,需要扣除SB SRCH的时间, */
    VOS_UINT32 sbVerifyFbSrchStartTime; /* 在SB验证期间,可能执行FB SRCH,FB SRCH起始时间点. */
    VOS_UINT32 sbPeriodStartSlice;      /* SB验证周期的起始时间点. */

    GPHY_TOOL_SrvccBsicRsltInfo cellBsicInfo[GPHY_TOOL_SLAVE_BSIC_MAX_NUM];
} GPHY_TOOL_SrvccScheduleInfo;

/*
 * 6.2 STRUCT定义,WPHY定义
 */
/*
 * 结构说明: W TX PA增益切换时功率补偿值
 */
typedef struct {
    VOS_INT16 h2MComp;  /* PA增益从高切到中的门限补偿,单位0.1db */
    VOS_INT16 m2HComp;  /* PA增益从中切到高的门限补偿,单位0.1db */
    VOS_INT16 m2LComp;  /* PA增益从中切到低的门限补偿,单位0.1db */
    VOS_INT16 l2MComp;  /* PA增益从低切到中的门限补偿,单位0.1db */
    VOS_INT16 l2UlComp; /* PA增益从低切到超的门限补偿,单位0.1db */
    VOS_INT16 uL2LComp; /* PA增益从超低切到低的门限补偿,单位0.1db */
} WPHY_TOOL_PaGainSwitchCompInfo;

/*
 * 结构说明: W TX 4个时隙的PA增益切换时功率补偿值
 */
typedef struct {
    WPHY_TOOL_PaGainSwitchCompInfo slotComp[WPHY_TOOL_PA_SWITCH_COMP_SLOT];
} WPHY_TOOL_TxPaGainSwitchComp;

/*
 * 结构说明: PA益切换补偿原语
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* Msg ID */
    VOS_UINT16 rsv1;  /* 保留 */

    WPHY_TOOL_TxPaGainSwitchComp paGainSwitchComp;
} WPHY_TOOL_PaGainSwitchCompReq;

/*
 * 结构说明: PA益切换补偿原语的结果
 */
typedef struct {
    VOS_INT16 h2M;  /* 高到中增益补偿 */
    VOS_INT16 m2H;  /* 中到高增益补偿 */
    VOS_INT16 m2L;  /* 中到低增益补偿 */
    VOS_INT16 l2M;  /* 低到中增益补偿 */
    VOS_INT16 l2Ul; /* 低到超低增益补偿 */
    VOS_INT16 uL2L; /* 超低到低增益补偿 */
} WPHY_TOOL_SigPaGainSwitchCompInd;

/*
 * 结构说明: PA益切换补偿原语的回复
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16                       transId;               /* 原语ID,固定值:0x5001 */
    VOS_UINT16                       reserved1;             /* 保留字段 */
    VOS_UINT16                       funcType;              /* 固定值:0x04 */
    VOS_UINT16                       length;                /* 从uwSn至结尾的长度 */
    VOS_UINT32                       sn;                    /* 固定值:0 */
    VOS_UINT32                       timeStamp;             /* 上报CNF或者SFN */
    VOS_UINT16                       primId;                /* WDSP_MNTN_MSG_ID_ENUM_UINT16,消息ID  */
    VOS_UINT16                       reserved2;
    VOS_UINT32                       result;                /* 消息内容起始地址 */
    WPHY_TOOL_SigPaGainSwitchCompInd rfPaGainSwitchCompInd; /* 当前增益补偿上报 */
} WPHY_TOOL_PaGainSwitchCompCnf;

/*
 * 结构说明: 该项是测试通过SDT实时打开关闭PD的函数的请求原语
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;   /* Msg ID */
    VOS_UINT16 rsv1;    /* 保留 */
    VOS_UINT32 pdEnFlg; /* PD使能标记，0:去使能，1:使能 */
} WPHY_TOOL_PowDetEnableReq;

/*
 * 结构说明: 该项是测试通过LMT实时打开关闭PD的函数的回复原语
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 transId;   /* 原语ID,固定值:0x5001 */
    VOS_UINT16 reserved1; /* 保留字段 */
    VOS_UINT16 funcType;  /* 固定值:0x04 */
    VOS_UINT16 length;    /* 从ulSn至结尾的长度 */
    VOS_UINT32 sn;        /* 固定值:0 */
    VOS_UINT32 timeStamp; /* 上报CNF或者SFN */
    VOS_UINT16 primId;    /* WDSP_MNTN_MSG_ID_ENUM_UINT16，消息ID  */
    VOS_UINT16 reserved2;
    VOS_UINT32 errCode;   /* 结果 */
} WPHY_TOOL_PowDetEnableCnf;

/*
 * 结构说明: WPHY可维可测公共回复消息结构
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRsltUint16  result;  /* 接收原语的处理结果ID */
    VOS_UINT16                rsv;     /* 保留字段 */
} WPHY_TOOL_CommonCnf;

/*
 * 结构说明: 激活集小区信息
 */
typedef struct {
    WPHY_TOOL_MntnReportTypeUint16 reportType;
    VOS_UINT16                     reportPeriod; /* 默认0，标识无效，当上报周期为指定周期时，该IE有效。 */
} WPHY_MNTN_ReportType;

typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */                /* 原语类型 */
    VOS_UINT16           toolsId;    /* 保留字段 */
    WPHY_MNTN_ReportType reportType; /* 默认每帧上报  */
} WPHY_TOOL_ActCellReq;

/*
 * 结构说明: 激活集小区信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_UINT16                scramble[WPHY_MNTN_MAX_ACT_CELL_NUM]; /* 激活集小区的扰码 */
    VOS_UINT16                servingCellScramble;                  /* 主小区扰码 */
} WPHY_TOOL_ActCellInfoInd;

/*
 * 结构说明: 小区搜索列表信息
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16           toolsId;    /* 保留字段 */
    WPHY_MNTN_ReportType reportType; /* 默认事件触发上报，每次小区搜索上报  */
} WPHY_TOOL_CellSearchListReq;

/*
 * 结构说明: 小区搜索列表信息
 */
typedef struct {
    VOS_UINT16 scramble;
    VOS_UINT16 energy;
    VOS_UINT32 phase;     /* 需要转换为Chip级别 */
    VOS_UINT16 lifeCycle; /* 生命周期 */
    VOS_UINT16 rsv;
} WPHY_MNTN_CsListCellInfo;

typedef struct {
    VOS_UINT16               freq;
    VOS_UINT16               cnt;
    WPHY_MNTN_CsListCellInfo cellInfo[32];
} WPHY_MNTN_CsListInfo;

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_UINT16                freqCnt;
    VOS_UINT16                rsv;
    WPHY_MNTN_CsListInfo      csList[4];
} WPHY_TOOL_CellSearchListInd;

/*
 * 结构说明: 功控功能控制，指定打开或关闭如下的功控功能:
 *             1.下行功控开关(如果此开关关闭，则标识所有的功控功能全部失效,
 *                包括内环功控，外环功控，Winup,Windown等，
 *                此时可以再通过功率设置参数来设置上行TPC,SirTarget等。)
 *             2.外环功控(如果此功能关闭，仅标识外环功能关闭，此时可以通过
 *                功率参数设置功能来设置SirTarget。)
 *             3.Windown
 *             4.Winup
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16         msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId; /* 保留字段 */
    WPHY_TOOL_MntnPwrSwitchTypeUint16 pCType;
    WPHY_TOOL_MntnSwitchCtrlUint16    status;
} WPHY_TOOL_PwrSwitchCtrlReq;

/*
 * 结构说明: 功率参数设置，以Mask标识有效标记
 *             1.SirTarget
 *             2.初始发射功率
 *             3.Hold上行发射功率(设置最大最小发射功率)
 *             4.设置指定发送的上行TPC.
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16   msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnPwrParaUint32 pwrParaMask;
    VOS_UINT32                  sirTarget; /* SirTarget */
    VOS_INT16                   initTxPwr; /* 初始功率 */
    VOS_INT16                   ulTpc;     /* 上行TPC */
    VOS_INT16                   maxTxPwr;  /* 最大发射功率 */
    VOS_INT16                   minTxPwr;  /* 最小发射功率 */
} WPHY_TOOL_PwrParaSetReq;

/*
 * 结构说明: 误块率上报
 */
typedef struct {
    VOS_MSG_HEADER                            /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;    /* 保留字段 */
    WPHY_MNTN_ReportType          reportType; /* 默认每帧上报  */
} WPHY_TOOL_BlerReportReq;

/*
 * 结构说明: 误块率上报功能
 *           误块率 = ulErrorBlocks/ulTotalBlocks
 */
typedef struct {
    VOS_UINT16 trchId;
    VOS_UINT16 tti;         /* 每个传输信道的TTI */
    VOS_UINT32 errorBlocks; /* 错块数  */
    VOS_UINT32 totalBlocks; /* 总块数 */
} WPHY_MNTN_TrchBler;

/*
 * 结构说明: 误块率上报功能
 *           每次上报的传输信道个数不固定，个数多少根据CNT确定。
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_UINT16                cnt;
    VOS_UINT16                rsv;
    WPHY_MNTN_TrchBler        trchBler[WPHY_MNTN_TRCH_NUM];
} WPHY_TOOL_BlerReportInd;

/*
 * 结构说明: 事件上报控制
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnEventTypeUint32 eventTypeMask;
} WPHY_TOOL_EventReportReq;

/*
 * 结构说明: W下同步失步状态上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId; /* 保留字段 */
    VOS_UINT16                 sfn;
    VOS_UINT16                 cfn;
    WPHY_TOOL_SyncStatusUint16 syncStatus;
    VOS_UINT16                 rsv;
} WPHY_TOOL_SyncStatusInd;

/*
 * 结构说明: AFC锁定
 * EVENT_VCTCXO_FREEZE ,EVENT_VCTCXO_UNFREEZE
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    UPHY_TOOL_XoStatusUint16  tcxoStatus;
    VOS_UINT16                rsv;
    VOS_UINT32                afcStatusValue;
} WPHY_TOOL_AfcStatusInd;

/*
 * 结构说明: 中断异常事件上报，
 *             1.在6ms任务结束时，读取9ms任务已经到达。
 *             2.在9ms任务结束时，读取SFN判断该任务是否已经跨帧。
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;

    WPHY_TOOL_MntnIntExceptUint16 intExceptType; /* 中断异常类型 */
    VOS_UINT16                    rsv;
} WPHY_TOOL_InterruptExceptiionInd;

/*
 * 结构说明: 上报L1状态
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;

    VOS_UINT32                    channelStatus; /* 信道状态 */
    WPHY_TOOL_MntnSysStatusUint16 sysStatus;     /* 系统状态 */
    VOS_UINT16                    rsv;
} WPHY_TOOL_WcdmaL1StatusInd;

/*
 * 结构说明: DRX功能打开和关闭设置
 */
typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16  msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId; /* 保留字段 */
    WPHY_TOOL_MntnSwitchCtrlUint16 drxCtrl;
    VOS_UINT16                     rsv;
} WPHY_TOOL_DrxSwitchReq;

/*
 * 结构说明: 门限设置
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16     msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId; /* 保留字段 */
    WPHY_TOOL_MntnThresholdTypeUint32 thresholdType;
    VOS_INT32                         value;
} WPHY_TOOL_ThresholdSetReq;

/*
 * 结构说明: 查询
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16     msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId; /* 保留字段 */
    WPHY_TOOL_MntnThresholdTypeUint32 thresholdType;
} WPHY_TOOL_ThresholdQueryReq;

/*
 * 结构说明: 查询
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId; /* 保留字段 */
    VOS_UINT16                        sfn;
    VOS_UINT16                        cfn;
    WPHY_TOOL_MntnThresholdTypeUint32 thresholdType;
    VOS_INT32                         value;
} WPHY_TOOL_ThresholdQueryInd;

/*
 * 结构说明: 错误上报
 */
typedef struct {
    VOS_MSG_HEADER                                   /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16     msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId;       /* 保留字段 */
    WPHY_TOOL_MntnErrorRptLevelUint16 errorRptLevel; /* 控制上报错误级别 */
    VOS_UINT16                        rsv;
} WPHY_TOOL_ErrorReportReq;

/*
 * 结构说明: 错误上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                fileNo;  /* 文件名编码 */
    VOS_UINT16                lineNo;  /* 出错的行号 */
    VOS_UINT32                timeSlice;
    VOS_UINT32                logId;                                      /* PHY软件内统一编号LOG ID */
    VOS_UINT16                reserved;                                   /* 保留字段 */
    VOS_UINT16                varCnt;                                     /* 上报变量个数 */
    VOS_UINT32                varValue[WPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* 上报变量值 */
} WPHY_TOOL_ErrorReportInd;

/*
 * 结构说明: RF前端参数上报
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    WPHY_MNTN_ReportType reportType; /* 默认每帧上报  */
} WPHY_TOOL_RfFrontEndParaReq;

/*
 * 结构说明: 上报TX Temp Comp,Freq Comp,AFC,LNA,HDET
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;        /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    VOS_INT16                 paHighTempComp; /* PA高增益时温度补偿 */
    VOS_INT16                 paMidTempComp;  /* PA中增益时温度补偿 */
    VOS_INT16                 paLowTempComp;  /* PA低增益时温度补偿 */

    VOS_INT16 paHighFreqComp; /* PA高增益时频率补偿 */
    VOS_INT16 paMidFreqComp;  /* PA中增益时频率补偿 */
    VOS_INT16 paLowFreqComp;  /* PA低增益时频率补偿 */

    VOS_UINT16 afcStatus;     /* AFC值 */
    VOS_UINT16 ant1AgcStatus; /* 天线1的AGC增益状态 */
    VOS_UINT16 ant2AgcStatus; /* 天线2的AGC增益状态 */
    VOS_UINT16 hdet;          /* HDET值 */
    VOS_UINT16 paStatus;      /* PA状态:上报的是PA的模式控制字 */
    VOS_UINT16 dbbAtten;      /* DBB衰减 */
    VOS_UINT16 rfAtten;       /* RF衰减,boston不使用 */
    VOS_UINT16 apcAptIndex;   /* APC和APT索引值上报，boston使用apc:0-7;apt:8-15 */
} WPHY_TOOL_RfFrontEndParaInd;

/*
 * 结构说明: WPHY的PI中断结果上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_INT16                 piResult;
    VOS_UINT16                piSoftValue;
    UPHY_TOOL_WphyDrxMntnInfo wphyDrxMntn; /* DRX可维可测信息 */
} WPHY_TOOL_PiIntResultInd;

/*
 * 结构说明: WPHY的AI中断结果上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;        /* 保留字段 */
    VOS_INT16                 aiResult;       /* AI结果,+1；-1；0 */
    VOS_UINT16                reTranNum;      /* 重传次数 */
    VOS_INT16                 initPower;      /* 初始功率,单位 0.1dBm */
    VOS_INT16                 currentPower;   /* 当前功率,单位 0.1dBm */
    VOS_INT16                 maxTxPower;     /* 最大发射功率,单位0.1dBm */
    VOS_INT16                 maxAiSoftValue; /* AICH软值最大值 */
} WPHY_TOOL_AiIntResultInd;

/*
 * 结构说明: T313定时器状态上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16      msgId;      /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId;    /* 保留字段 */
    WPHY_TOOL_MntnT313StatusUint16 t313Status; /* T313状态 */
    VOS_UINT16                     rsv;
} WPHY_TOOL_T313StatusInd;

/*
 * 结构说明: T312定时器状态上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16      msgId;      /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId;    /* 保留字段 */
    WPHY_TOOL_MntnT312StatusUint16 t312Status; /* T313状态 */
    VOS_UINT16                     rsv;
} WPHY_TOOL_T312StatusInd;

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT32                reserve; /* for coverity */
} WPHY_TOOL_SlaveCdrxStartInd;

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT32                reserve; /* for coverity */
} WPHY_TOOL_SlaveCdrxStopInd;

/*
 * 结构说明:多径消息结构
 */
typedef struct {
    VOS_UINT16 pathEnergy; /* 径能量 */
    VOS_UINT16 pathPos;    /* 径相位 */
} WPHY_TOOL_AmpPathInfo;

/*
 * 结构说明: W SLAVE CS信息
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16          intraStepbEn;     /* stepb是否使能 */
    WPHY_TOOL_MntnBoolUint16          stepbScramProMod; /* stepb是否无扰码，0表示带扰码 */
    WPHY_TOOL_MntnBoolUint16          stepbSaveMod;     /* stepb存数模式，0表示step1存数，可以离线做 */
    WPHY_TOOL_MntnSlaveCsStatusUint16 csStatus;         /* 小区搜索状态标志, 表示每个轮小区搜索完成  */
} WPHY_TOOL_WslaveCsInfo;

/*
 * 结构说明: W SLAVE MEAS FRAME PHASE信息
 */
typedef struct {
    VOS_UINT16 preFrameSlot;
    VOS_UINT16 preFrameChip;
    VOS_INT32  preSlaveWTimeOffset;
    VOS_INT32  curSlaveWTimeOffset;
    VOS_INT32  umtsSysBaseCntOff;
} WPHY_TOOL_MeasFramePhaseInfo;
/*
 * 结构说明: W SLAVE时主从模定时信息
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
 * 结构说明: W SLAVE MEAS信息
 */
typedef struct {
    VOS_UINT16                         measCellNum;
    VOS_UINT16                         measCellIndex;
    VOS_UINT16                         mpPurpose; /* 当前多径目的 */
    VOS_UINT16                         reserved;
    VOS_UINT32                         measStatus;
    VOS_UINT32                         measCsStatus;
    VOS_UINT32                         unitBitMap;          /* 多径的bitmap */
    VOS_UINT32                         mpResourceMap;       /* 当前多径资源情况 */
    WPHY_TOOL_AmpPathInfo              unitFirstFingerInfo; /* Strongest finger info */
    VOS_UINT32                         int0ConfigTime;      /* 短沟的预进沟调度时间，长沟无效 */
    WPHY_TOOL_MeasFramePhaseInfo       measFramePhaseInfo;
    WPHY_TOOL_UmtsLteGsmTimingInfoStru masterTimingInfo;
} WPHY_TOOL_WslaveMeasInfo;

/*
 * 结构说明: 单时钟下当前ppm，scpll配置值，温度值，AFC值
 */
typedef struct {
    VOS_INT32  ppmValue;         /* ppm */
    VOS_UINT32 pllFracValue;     /* Scpll 小数部分调整值 */
    VOS_UINT32 pllInterValue;    /* Scpll 整数部分调整值 */
    VOS_UINT32 afcValue4518;     /* AFC 4518 */
    VOS_INT16  temperatureValue; /* 温度 */
    VOS_UINT16 reserved;
} WPHY_TOOL_AfcInfo;

/*
 * 结构说明: W SLAVE 前端信息
 */
typedef struct {
    VOS_UINT16        currFreq;        /* CurrFreq */
    VOS_UINT16        currBand;        /* CurrBand */
    WPHY_TOOL_AfcInfo afcInfo;         /* 单时钟下当前ppm，scpll配置值，温度值，AFC值 */
    VOS_UINT32        feRssi;          /* RSSI 1414 */
    VOS_UINT32        feRssiDiv;       /* RSSI 1418 */
    VOS_UINT32        dcOffset0[8];    /* DC OFFSET 0 */
    VOS_UINT32        chnMasterModeRx; /* Channel master mode */
    VOS_UINT32        chnMasterModeTx; /* Channel master mode */
    VOS_UINT32        chnSlaveMode;    /* Channel slave mode */
    VOS_UINT32        chnSlaveChn[8];
    VOS_UINT32        ssiEn[2];         /* SSI EN */
    VOS_UINT32        rfPllStatus;      /* rfic只在测量完成时更新 */
    VOS_UINT32        crossMipiLineRpt; /* CrossMIpi线控信息 */
    VOS_UINT32        crossLnaLineRpt;  /* CrossLNa线控信息 */
    VOS_UINT16        tasAntStatusInfo; /* TasAntStatus */
    VOS_UINT16        wtcMode;          /* WTC模式 */
    VOS_UINT16        shareSel3gp;      /* 3GP SHARE SEL */
    VOS_UINT16        reserved;         /* 保留对齐 */
#if (FEATURE_ON == FEATURE_EASYRF)
    VOS_UINT32 clkGatingValue; /* B5000 配置的BBP 时钟门控值 */
#endif
} WPHY_TOOL_WslaveFeInfo;

/*
 * 结构说明: W SLAVE BBP enter exit gap 信息
 */
typedef struct {
    VOS_UINT32 from3F34To3F40[4]; /* enter exit gap, measure status */
    VOS_UINT32 materMeasWen;      /*  10E8 */
    VOS_UINT32 mipiSsiBusy;       /*  F80A34 */
} WPHY_TOOL_WbbpGapInfo;

#if (FEATURE_ON == FEATURE_EASYRF)
/*
 * 结构说明: WBBP INTRA/INTER状态可维可测信息上报
 */
typedef struct {
    VOS_UINT32 intraInterStatus;  /* 本异频状态机 */
    /* Intra/Inter Pos生效方式,为0,表示本异频pose需要经过帧头锁进去后才能生效,为1表示,到了pose点就可以生效 */
    VOS_UINT32 posValidType;
    VOS_UINT32 exitIntraPos;      /* 退出本频状态时刻 */
    VOS_UINT32 exitNonIntraPos;   /* 退出非本频状态时刻 */
    VOS_UINT32 backToIntraPos;    /* 回到本频状态时刻 */
    VOS_UINT32 enterInterPos;     /* 进入异频状态时刻 */
    VOS_UINT32 exitInterPos;      /* 退出异频状态时刻 */
    VOS_UINT32 backToNonInterPos; /* 回到非异频状态时刻 */
} WPHY_TOOL_WbbpIntraInterStatusInfo;
#endif

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16              msgId;                /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                             toolId;               /* tools id */
    WPHY_TOOL_MntnSlaveCurrentStatusUint16 currentStatus;
    VOS_UINT16                             currentFreqIndex;
    WPHY_TOOL_TimingInfo                   wslaveTimingInfo;     /* 定时上报信息(公共携带SFN CFN SLOT SLICE信息) */
    WPHY_TOOL_WslaveFeInfo                 wslaveFeInfo;         /* 前端信息 */
    WPHY_TOOL_WslaveMeasInfo               wslaveMeasInfo;       /* WslaveMeasInfo */
    WPHY_TOOL_WslaveCsInfo                 wslaveCsInfo;         /* WslaveCSInfo */
    WPHY_TOOL_WbbpGapInfo                  wbbpEnterExitGapInfo; /* EnterExitGapInfo */
#if (FEATURE_ON == FEATURE_EASYRF)
    WPHY_TOOL_WbbpIntraInterStatusInfo wbbpIntraInterStatus; /* BBP本异频状态信息 */
#endif
} WPHY_TOOL_SlaveMntnRptInd;

/*
 * 结构说明: PA状态上报请求
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */

    WPHY_MNTN_ReportType reportType; /* 默认每帧上报  */
} WPHY_TOOL_PaStatusReportReq;

/*
 * 结构说明: 唤醒LTE的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                sfn;        /* 唤醒时刻的sfn */
    VOS_UINT16                slot;       /* 唤醒时刻的slot */
    VOS_UINT32                wakeReason; /* 唤醒目的 */
} WPHY_TOOL_MasterWakeSlaveInd;

/*
 * 结构说明: 启动LTE测量的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;      /* 保留字段 */
    VOS_UINT16                sfn;          /* 启动时刻的sfn */
    VOS_UINT16                slot;         /* 启动时刻的slot */
    VOS_UINT32                startReason;  /* 启动原因 */
    VOS_UINT32                measTime;     /* 测量时间 us */
    VOS_UINT32                int0Int1Time; /* int0 int1间隔 us */
    VOS_UINT32                afcLockFlag;
    VOS_INT32                 afcValue;
} WPHY_TOOL_MasterStartLteMeasInd;

/*
 * 结构说明: 启动LTE测量的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;     /* 上报时刻的sfn */
    VOS_UINT16                slot;    /* 上报时刻的slot */
} WPHY_TOOL_MasterReportLteMeasInd;

/*
 * 结构说明: 强停LTE测量的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16                 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                                toolsId; /* 保留字段 */
    WPHY_TOOL_MntnMasterStopSlaveReasonUint16 reason;  /* 强停原因 */
    VOS_UINT16                                sfn;     /* 强停时刻的sfn */
    VOS_UINT16                                slot;    /* 强停时刻的slot */
    VOS_UINT16                                rsv;
} WPHY_TOOL_MasterStopSlaveMeasInd;

/*
 * 结构说明: 设置LTE测量标记的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                value;
    VOS_UINT16                rsv;
} WPHY_TOOL_SetLteMeasFlagInd;

/*
 * 结构说明: 配置的时刻
 */
typedef struct {
    VOS_UINT16 sfn;  /* 配置时刻的sfn */
    VOS_UINT16 cfn;  /* 配置时刻的cfn */
    VOS_UINT16 slot; /* 配置时刻的slot */
    VOS_UINT16 chip; /* 配置时刻的chip */
} WPHY_TOOL_CfgTimeInfo;

/*
 * 结构说明: 配置的时刻
 */
typedef struct {
    WPHY_TOOL_MntnRfOperSceneUint16 rfCfgOpSc;  /* RF配置的目的 */
    VOS_UINT16                      antMode;    /* RF天线模式 */
    VOS_UINT16                      intraBand;  /* 本频BAND */
    VOS_UINT16                      intraFreq;  /* 本频频点 */
    VOS_UINT16                      adjFreq;    /* 邻频频点 */
    VOS_UINT16                      interBand;  /* 异频BAND */
    VOS_UINT16                      interFreq;  /* 异频频点 */
    VOS_UINT16                      rxRficMode; /* RX RFIC模式 */
    VOS_UINT16                      txRficMode; /* TX RFIC模式 */
    VOS_UINT16                      reserve;
} WPHY_TOOL_RfCfgInfo;

/*
 * 结构说明: 设置LTE测量标记的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    WPHY_TOOL_CfgTimeInfo     cfgTime; /* RF配置时刻 */
    WPHY_TOOL_RfCfgInfo       cfgInfo; /* RF配置信息 */
} WPHY_TOOL_MasterRfConfigInd;

/*
 * 结构说明: FET帧级开关模板信息结构体
 */
typedef struct {
    VOS_UINT16 ustartFrameCfn; /* 模板的起始帧的CFN号 */
    /* 帧开关模板,bit0指示起始帧的开关,1为开,0为关;以此类推bitn指起始帧后第n帧,每个bit代表一帧的开关 */
    VOS_UINT16 patternBitMap;
} WPHY_TOOL_FetFramePatternInfo;

/*
 * 结构说明: FET打开信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;        /* 保留字段 */
    VOS_UINT16                fetEnable;      /* FET使能开关,开:UPHY_TRUE,关:UPHY_FALSE */
    VOS_UINT16                currDpchSf;     /* 保存当前使用的DPCH信道 */
    VOS_UINT16                ulTpcAlgorithm; /* 保存当前使用的功控算法 */
    VOS_UINT16                trchTti;        /* 语音传输信道TTI */
    VOS_UINT16                fetBlindStatus; /* FET SRB 盲检测开关，打开UPHY_TRUE,关闭UPHY_FALSE */
    VOS_UINT16 fetSrbCheckStatus; /* FET SRB 场景评估动作状态，已经检测过为:UPHY_TRUE,没检测过为:UPHY_FALSE */
    WPHY_TOOL_FetFramePatternInfo framePatternInfo; /* FET帧级开关模板信息 */
} WPHY_TOOL_FetOpenInd;

/*
 * 结构说明: FET关闭信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;   /* 保留字段 */
    VOS_UINT16                fetStatus; /* FET使能开关,开:UPHY_TRUE,关:UPHY_FALSE */
    VOS_UINT16                reserved;
    VOS_UINT32                fetCloseVote; /* FET开关投票,每个bit代表1票,为1指示投关的票,为0指示投开的票 */
    VOS_UINT32 fetRxOpenVote;               /* FET帧级下行接收开关投票,每个bit代表1票,为1指示投开的票,为0指示投关的票 */
    WPHY_TOOL_FetFramePatternInfo framePatternInfo; /* FET帧级开关模板信息 */
} WPHY_TOOL_FetCloseInd;

/*
 * 结构说明: CLT状态信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId;  /* 保留字段 */
    WPHY_TOOL_CltDetectFailTypeUint16 type;
    VOS_UINT16                        reserved; /* 保留字段 */
} WPHY_TOOL_CltDetectFailRptInd;

/*
 * 结构说明: CLT状态信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId; /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                 /* 保留字段 */
    VOS_UINT32                reserved;                /* 保留字段 */
} WPHY_TOOL_CltStatusRptInd;

/*
 * 结构说明: QPC打开上行链路的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                sfn;        /* 唤醒时刻的sfn */
    VOS_UINT16                slot;       /* 唤醒时刻的slot */
    VOS_UINT32                openReason; /* 打开原因 */
} WPHY_TOOL_QpcOpenUlInd;

/*
 * 结构说明: QPC关闭上行链路的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;     /* 保留字段 */
    VOS_UINT16                sfn;         /* 唤醒时刻的sfn */
    VOS_UINT16                slot;        /* 唤醒时刻的slot */
    VOS_UINT32                closeReason; /* 关闭原因 */
} WPHY_TOOL_QpcCloseUlInd;

/*
 * 结构说明: QPC当前RRC状态的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;  /* 保留字段 */
    VOS_UINT16                sfn;      /* 唤醒时刻的sfn */
    VOS_UINT16                slot;     /* 唤醒时刻的slot */
    VOS_UINT32                rrcState; /* RRC状态 */
} WPHY_TOOL_QpcRrcStateInd;

/*
 * 结构说明: QPC当前业务类型的指示 CS/PS
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;     /* 唤醒时刻的sfn */
    VOS_UINT16                slot;    /* 唤醒时刻的slot */
    VOS_UINT32                rbType;  /* RB TYPE */
} WPHY_TOOL_QpcRbTypeInd;

/*
 * 结构说明: TAS明切换评估阶段可维可测上报
 */
typedef struct {
    VOS_UINT16 nextState; /* 下一双天线状态 */
    VOS_INT16  rssi;      /* 主天线的RSSI,滤波后的结果,单位为0.125dbm */
    VOS_INT16  rscp;      /* 主天线的RSCP,滤波后的结果,单位为0.125dbm */
    VOS_UINT16 antStatus; /* 当前天线状态,0为主天线1为分集 */
} WPHY_TOOL_NonblindEstimateStateInd;

/*
 * 结构说明: TAS盲切换评估阶段可维可测上报
 */
typedef struct {
    VOS_UINT16 nextState;   /* 下一状态 */
    VOS_UINT16 rsv;         /* 保留字段 */
    VOS_INT16  deltaRscp;   /* T1时间内信号处于中间值前后能量差 */
    VOS_INT16  rscpAveCurr; /* 当前T1时间单个激活集小区能量,单位为0.125dbm */
    VOS_INT16  rscpAvePre;  /* 前一个T1时间单个激活集小区能量,单位为0.125dbm */
    VOS_UINT16 antStatus;   /* 当前天线状态,0为主天线1为分集 */
} WPHY_TOOL_BlindEstimateStateInd;

/*
 * 结构说明: TAS评估阶段可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16          msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                         toolsId;      /* 保留字段 */
    WPHY_TOOL_NonblindEstimateStateInd nonBlindInfo; /* 明切换评估状态信息 */
    WPHY_TOOL_BlindEstimateStateInd    blindInfo;    /* 盲切换评估状态信息 */
} WPHY_TOOL_TasEstimateStateInd;

/*
 * 结构说明: TAS明切换比较阶段可维可测上报
 */
typedef struct {
    VOS_UINT16 nextState; /* 下一双天线状态 */
    VOS_UINT16 phase;     /* 比较阶段下一阶段 */
    VOS_INT16  deltaRscp; /* 主分集天线滤波后的RSCP差值,单位为0.125dbm */
    VOS_UINT16 antStatus; /* 当前天线状态,0为主天线1为分集 */
} WPHY_TOOL_NonblindCompareStateInd;

/*
 * 结构说明: TAS盲切换比较阶段可维可测上报
 */
typedef struct {
    VOS_UINT16 nextState; /* 下一双天线状态 */
    VOS_UINT16 rsv;
    VOS_UINT16 phase;       /* 比较阶段下一阶段 */
    VOS_INT16  rscpAveCurr; /* 主分集天线滤波后的RSCP差值,单位为0.125dbm */
    VOS_INT16  rscpAvePre;  /* 主分集天线滤波后的RSCP差值,单位为0.125dbm */
    VOS_UINT16 antStatus;   /* 当前天线状态,0为主天线1为分集 */
} WPHY_TOOL_BlindCompareStateInd;

/*
 * 结构说明: TAS比较阶段可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId;      /* 保留字段 */
    WPHY_TOOL_NonblindCompareStateInd nonBlindInfo; /* 下一双天线状态 */
    WPHY_TOOL_BlindCompareStateInd    blindInfo;    /* 比较阶段下一阶段 */
} WPHY_TOOL_TasCompareStateInd;

/*
 * 结构说明: TAS迟滞阶段可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;   /* 保留字段 */
    VOS_UINT16                antStatus; /* 当前天线状态 */
    VOS_UINT16                rsv;       /* 四字节对齐 */
} WPHY_TOOL_MasFrozenStateInd;

/*
 * 结构说明: 从模W任务完成指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16               msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                              toolsId;        /* 保留字段 */
    WPHY_TOOL_MntnSlaveMeasFinishFlagUint16 measReportType; /* 完成上报的类型 */
    VOS_UINT16                              reportFreqNum;  /* 上报频点的个数 */
    VOS_UINT16                              freq[WPHY_MNTN_SLAVE_REPORTFREQ_NUM];
} WPHY_TOOL_SlaveWFinishInd;

/*
 * 结构说明: 从模W任务完成指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT32                rsv;
} WPHY_TOOL_SlaveGrssiFinishInd;

/*
 * 结构说明: 从模G BSIC任务完成指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT32                rsv;
} WPHY_TOOL_SlaveGbsicFinishInd;

/*
 * 结构说明: 从模调度超时指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                startChip;
    VOS_UINT16                startSlot;
    VOS_UINT16                endChip;
    VOS_UINT16                endSlot;
    VOS_UINT16                scheduleTime; /* 调度时间，单位us */
    VOS_UINT16                rsv;
} WPHY_TOOL_SlaveScheduleTimeoutInd;
/*
 * 结构说明: PA状态上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId; /* 保留字段 */
    VOS_UINT16                   sfn;
    VOS_UINT16                   cfn;
    WPHY_TOOL_MntnPaStatusUint16 paStatus;
    VOS_UINT16                   rsv;
} WPHY_TOOL_PaStatusReportInd;

/*
 * 结构说明: PHY 内部消息上报控制请求
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */      /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                msgRptFlag; /* 指示内部消息是否上报 */
    VOS_UINT16                rsv;        /* 保留字段 */
} WPHY_TOOL_IntraMsgRptCtrlReq;

/*
 * 结构说明: 静态查询前端状态请求原语
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
} WPHY_TOOL_RfFrontEndStaticRequireReq;

/*
 * 结构说明: 静态查询前端状态回复原语
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;         /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;       /* 保留字段 */
    VOS_UINT16                rfTrcvOnState; /* RF总开关，1为打开，0为关闭 */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT16 rfRxOnOrOff; /* 04h[1:0] = 3为开 */
#endif
    VOS_UINT16 rxPathselect; /* 04h[4:3] = 3为主分集都开，2开主集 */
    VOS_UINT16 wagcSwitch;   /* WBBP 主分集开关 */

    VOS_UINT16 abbRxConfig;           /* 3为主分集都开，1开主集 */
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:G模 2:W模 3:LTE模 4:TDS模 5:ET模式 6:APT模式 7:CA模式 */
    VOS_UINT16 abbTxCh01LineCtrlMode;
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:G模 2:W模 3:LTE模 4:TDS模 5:ET模式 6:APT模式 7:CA模式 */
    VOS_UINT16 abbTxCh23LineCtrlMode;
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:G模 2:W模 3:LTE模 4:TDS模 5:ET模式 6:APT模式 7:CA模式 */
    VOS_UINT16 abbRxCh01LineCtrlMode;
    /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:G模 2:W模 3:LTE模 4:TDS模 5:ET模式 6:APT模式 7:CA模式 */
    VOS_UINT16 abbRxCh23LineCtrlMode;
    VOS_UINT16 abbRxCh01Mode;    /* CH1:[15:8],CH0:[7:0] = 0:2G 1:3G_SC 2:4G 3: 3G_DC 4:TDS */
    VOS_UINT16 abbRxCh23Mode;    /* CH1:[15:8],CH0:[7:0] = 0:2G 1:3G_SC 2:4G 3: 3G_DC 4:TDS */
    VOS_UINT16 abbCh01VoiceData; /* CH1:[15:8],CH0:[7:0] = 1:voice mode 2: data mode */
    VOS_UINT16 abbCh23VoiceData; /* CH1:[15:8],CH0:[7:0] = 1:voice mode 2: data mode */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT16 rfTxOnOrOff; /* 02h[1:0] = 3为开 */
#endif
    VOS_UINT16 rfTxShdn;     /* RF上行打开控制，0由BBP控制 */
    VOS_UINT16 txPathselect; /* RFIC TX Patch */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT16 rfTxAgc; /* RFIC TXAGC */
#endif
    VOS_UINT16 cpcDrxFuncOn;   /* 当前工作在CPC DRX的标志 */
    VOS_UINT16 cpcDrxStartCfn; /* CPC DRX开始工作的CFN */
    VOS_UINT16 cpcSleepBitMap; /* 低16bit，1为醒来时隙，0为睡眠时隙 */

    VOS_UINT16 gpioFuncOn; /* 硬件支持GPIO */
    VOS_UINT16 gpioPaMode; /* GPIO PA mode */

    VOS_UINT16 mipiFuncOn; /* 硬件支持MIPI */
    VOS_UINT16 mipiPaMode; /* MIPI PA mode */

    VOS_UINT16 aptFuncOn; /* 硬件支持APT */
    VOS_UINT16 aptPaMode; /* APT电压mode */
    VOS_UINT16 aptVcc;    /* 保留 */
} WPHY_TOOL_RfFrontEndStaticRequireInd;

/*
 * 结构说明: 是否实时输出前端信息
 */
typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId; /* 保留字段 */
    WPHY_TOOL_MntnSwitchCtrlUint16 displayCtrl;
    VOS_UINT16                     reserved;
} WPHY_TOOL_RfFrontEndDisplayReq;

/*
 * 结构说明: RTC功能打开和关闭设置
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    UPHY_TOOL_RtcDebugUint16  rtcCtrl; /* RTC打开或者关闭 */
    VOS_UINT16                rsv;
} WPHY_TOOL_BbpRtcReq;

/*
 * 结构说明: WPHY抓取固定块画图的RAM
 */
typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRamRptUint16    rptRam;
    WPHY_TOOL_MntnRptStatusUint16 rptStatus;
} WPHY_TOOL_RamReportReq;

/*
 * 结构说明: WPHY抓取固定块画图的RAM
 */

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId; /* 保留字段 */
    VOS_UINT16                 sfn;
    VOS_UINT16                 cfn;
    WPHY_TOOL_MntnRamRptUint16 rptRam;
    VOS_UINT16                 rsv;
    VOS_UINT32                 data[2];
} WPHY_TOOL_RamReportInd;

/*
 * 结构说明: WPHY抓取非固定地址的数据
 */
typedef struct {
    VOS_UINT32 blkAddr; /* 抓取数据块的首地址 */
    VOS_UINT32 blkLen;  /* 抓取数据块的长度,单位:32bit */
} WPHY_TOOL_BlkReport;

typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rsv;
    WPHY_TOOL_BlkReport           rptBlk[WPHY_TOOL_RPT_BLK_MAX_NUM];
} WPHY_TOOL_BlkReportListReq;

/*
 * 结构说明: WPHY抓取非固定地址的数据
 */

typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;  /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                sfn;
    VOS_UINT16                cfn;
    WPHY_TOOL_BlkReport       rptBlk[WPHY_TOOL_RPT_BLK_MAX_NUM];
    VOS_UINT32                data[2];
} WPHY_TOOL_BlkReportListInd;

/*
 * 结构说明: 随机接入可维可测上报设置消息结构
 */
typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId;    /* 保留字段 */
    WPHY_TOOL_MntnSwitchCtrlUint16 raMntnCtrl; /* 随机接入可维可测打开或关闭 */
    VOS_UINT16                     reserve;    /* 保留字段 */
} WPHY_TOOL_RaMntnCtrlReq;

/*
 * 结构说明: ARM 0MS可维可测上报设置消息结构
 */
typedef struct {
    VOS_MSG_HEADER                                 /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId;        /* 保留字段 */
    WPHY_TOOL_MntnSwitchCtrlUint16 arm0msMntnCtrl; /* ARM 0MS打开或关闭 */
    VOS_UINT16                     reserve;        /* 保留字段 */
} WPHY_TOOL_Arm0msMntnCtrlReq;

/*
 * 结构说明: UPA子帧可维可测信息上报结构体
 */
typedef struct {
    VOS_UINT16 rptValue[WPHY_MNTN_HSUPA_RPT_VALUE_MAX_NUM][WPHY_MNTN_HSUPA_OM_CARERIER_NUM];
} WPHY_TOOL_UpaSubframeRptInfo;

/*
 * 结构说明: UPA可维可测信息上报结构体
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId; /* 保留字段 */
    VOS_UINT16                   tti;     /* TTI类型 */
    VOS_UINT16                   resvered;
    WPHY_TOOL_UpaSubframeRptInfo rptValue[WPHY_MNTN_HSUPA_SUB_FRM_NUM];
} WPHY_TOOL_UpaRptInfo;

/*
 * 结构说明: UPA可维可测信息上报结构体
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId; /* 保留字段 */
    VOS_UINT16                   tti;     /* TTI类型 */
    VOS_UINT16                   resvered;
    WPHY_TOOL_UpaSubframeRptInfo rptValue;
} WPHY_TOOL_UpaRptInfoTti10ms;

/*
 * 结构说明: 该命令用于请求上报服务小区和邻区的信噪比,测量值等信息
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_GsmCellSnrReq;

/*
 * 结构说明: 每个小区的信噪比,测量值等信息
 */
typedef struct {
    VOS_UINT16 freqInfo;    /* 频点信息上报，高12bit表示频段，0:850,1:900,2:1800,3:1900，低12bit表示频点号，0-1023。 */
    VOS_INT16  rxLev125dbm; /* 信号强度值，[-110,-15],单位为dbm */
    VOS_UINT16 cellSnr;     /* 信噪比，[0,99],单位为db */
    VOS_INT16  preCi;       /* 解调前主集天线信干比,单位为1dB */
    VOS_INT16  preCiDiv;    /* 解调前分集天线信干比,单位为1dB, 分集天线-24dB为无效值 */
    VOS_UINT16 resvered;    /* 保留字段 */
} GPHY_TOOL_GsmCellSnr;

/*
 * 结构说明: 用于上报服务小区和邻区的信噪比,测量值等信息
 */
typedef struct {
    VOS_UINT16           msgId;                           /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16           toolId;                          /* 保留字段 */
    VOS_UINT16           cellNum;                         /* 频点数 */
    GPHY_TOOL_GsmCellSnr cellInfo[GPHY_TOOL_MA_FREQ_NUM]; /* 小区信息 */
    VOS_UINT16           resvered;                        /* 保留字段 */
} GPHY_TOOL_GsmCellSnrInd;

/*
 * 结构说明: 请求上报BA LIST的信号强度，bsic信息
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_BaListMeasReq;

/*
 * 结构说明: 每个小区的信息
 */
typedef struct {
    VOS_UINT16 freqInfo;    /* 频点信息上报，高12bit表示频段，0:850,1:900,2:1800,3:1900，低12bit表示频点号，0-1023。 */
    VOS_INT16  rxLev125dbm; /* 信号强度值，[-110,-15],单位为0.125dbm */
    VOS_UINT16 bsic;        /* 基站标识码，0-77，无效0xffff */
    VOS_UINT16 resvered;    /* 保留字段 */
} GPHY_TOOL_CellMeas;

/*
 * 结构说明: 上报BA LIST的信号强度，bsic信息
 */
typedef struct {
    VOS_UINT16 msgId;         /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;        /* 保留字段 */
    /* 频点信息上报，高12bit表示频段，0:850,1:900,2:1800,3:1900，低12bit表示频点号，0-1023。 */
    VOS_UINT16 scellFreqInfo;
    VOS_INT16  scellRxLev125dbm;                                 /* 信号强度值，[-110,-15],单位为0.125dbm */
    VOS_UINT16         hoppingInd;                               /* 跳频指示,1:表示跳频,0表示不跳频 */
    VOS_UINT16         resvered1;                                /* 保留字段 */
    VOS_UINT16         cellNum;                                  /* 邻区的个数 */
    VOS_UINT16         resvered2;                                /* 保留字段 */
    GPHY_TOOL_CellMeas neighbourCell[GPHY_TOOL_BA_LIST_MAX_NUM]; /* 邻区的测量信息 */
} GPHY_TOOL_BaListMeasInd;

/*
 * 结构说明: 请求上报AMR配置信息
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_AmrSettingReq;

/*
 * 结构说明: 上报AMR配置信息
 */
typedef struct {
    VOS_UINT16 msgId;                               /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;                              /* 保留字段 */
    VOS_UINT16 acsNum;                              /* 激活集个数,范围为0-4 */
    VOS_UINT16 resvered;                            /* 保留字段 */
    VOS_UINT16 acs[GPHY_TOOL_AMR_MAX_ACS_NUM];      /* 激活集集合，范围0-7 */
    VOS_UINT16 hyst[GPHY_TOOL_AMR_MAX_ACS_NUM - 1]; /* 切换迟滞大小，范围0-15，单位为0.5db */
    VOS_UINT16 thd[GPHY_TOOL_AMR_MAX_ACS_NUM - 1];  /* 切换门限大小，范围为0-63，单位为0.5db */
} GPHY_TOOL_AmrSettingInd;

/*
 * 结构说明: 请求上报激活集中每个语音速率的使用率
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_AmrCodecUsageReq;

/*
 * 结构说明: 上报激活集中每个语音速率的使用率
 */
typedef struct {
    VOS_UINT16 msgId;                               /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;                              /* 保留字段 */
    VOS_UINT16 acsNum;                              /* 激活集个数,范围为0-4 */
    VOS_UINT16 resvered;                            /* 保留字段 */
    VOS_UINT16 acs[GPHY_TOOL_AMR_MAX_ACS_NUM];      /* 激活集集合，范围0-7 */
    VOS_UINT32 uplink[GPHY_TOOL_AMR_MAX_ACS_NUM];   /* 上行链路激活集的每个速率的块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 downlink[GPHY_TOOL_AMR_MAX_ACS_NUM]; /* 下行链路激活集的每个速率的块数，范围0-2^32-1，单位为块 */
} GPHY_TOOL_AmrCodecUsageInd;

/*
 * 结构说明: 路测请求上报AMR和非AMR不同速率的使用率
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_DtStatCodecReq;

/*
 * 结构说明: 路测上报AMR和非AMR不同速率的使用率
 */
typedef struct {
    VOS_UINT16 msgId;                   /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;                  /* 保留字段 */
    VOS_UINT32 efsDlBlockNum;           /* 下行EFS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 efsUlBlockNum;           /* 上行EFS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 fsDlBlockNum;            /* 下行FS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 fsUlBlockNum;            /* 上行FS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 hsDlBlockNum;            /* 下行HS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 hsUlBlockNum;            /* 上行HS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 afsDlBlockNum;           /* 下行AFS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 afsUlBlockNum;           /* 上行AFS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 ahsDlBlockNum;           /* 下行AHS译码块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 ahsUlBlockNum;           /* 上行AHS译码块数，范围0-2^32-1，单位为块 */
} GPHY_TOOL_DtStatCodecInd;

/*
 * 结构说明: 请求各个编码速率的使用率
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_AmrCodecStatReq;

/*
 * 结构说明: 每个编码速率的使用率
 */

typedef struct {
    VOS_UINT32 uplink[GPHY_TOOL_AMR_CODEC_MAX_NUM];   /* 上行链路激活集的每个速率的块数，范围0-2^32-1，单位为块 */
    VOS_UINT32 downlink[GPHY_TOOL_AMR_CODEC_MAX_NUM]; /* 下行链路激活集的每个速率的块数，范围0-2^32-1，单位为块 */
} GPHY_AMR_CodecAvg;

/*
 * 结构说明: 小区，呼叫的统计信息
 */
typedef struct {
    VOS_UINT16        msgId;  /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16        toolId; /* 保留字段 */
    GPHY_AMR_CodecAvg cell;   /* 小区的统计信息 */
    GPHY_AMR_CodecAvg call;   /* 每个呼叫的统计信息 */
} GPHY_TOOL_AmrCodecStatInd;

/*
 * 结构说明: 请求上报CS/PS域上行信息
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_CsPsTxInfoReq;

/*
 * 结构说明: 上报CS/PS域上行信息
 */
typedef struct {
    VOS_UINT16 msgId;      /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;     /* 保留字段 */
    VOS_UINT16 ta;         /* 时间提前量，范围为0-63，单位为符号 */
    VOS_UINT16 slotNumber; /* 时隙个数 */
    VOS_UINT16 txPower[4]; /* 上行发射功率，范围为0-99，单位为dbm */
    VOS_UINT16 pcl;        /* 基站指定的功率等级，范围为0-31 */
    VOS_UINT16 resvered;
} GPHY_TOOL_CsPsTxInfoInd;

/*
 * 结构说明: 请求上报CS FER信息
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_CsFerReq;

/*
 * 结构说明: 上报CS FER信息
 */
typedef struct {
    VOS_UINT16 msgId;          /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;         /* 保留字段 */
    VOS_UINT32 errorFrameFull; /* full块的错误帧数 */
    VOS_UINT32 totalFrameFull; /* full总帧数 */
    VOS_UINT32 errorFrameSub;  /* sub块的错误帧数 */
    VOS_UINT32 totalFrameSub;  /* sub总帧数 */
} GPHY_TOOL_CsFerInd;

/*
 * 结构说明: 请求上报下行链路计数值
 */
typedef struct {
    VOS_MSG_HEADER       /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* 保留字段 */
    VOS_UINT16 command;  /* 命令。0:停止，1：启动 */
    VOS_UINT16 resvered; /* 保留字段 */
} GPHY_TOOL_GetCurrentDscReq;

/*
 * 结构说明: 上报下行链路计数值
 */
typedef struct {
    VOS_UINT16 msgId;  /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId; /* 保留字段 */
    VOS_UINT16 dsc;    /* 下行链路计数值 */
    VOS_UINT16 resvered;
} GPHY_TOOL_GetCurrentDscInd;

/*
 * 结构说明: 主天线能量评估阶段判决结果上报
 */
typedef struct {
    VOS_UINT16 msgId;             /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;            /* tools id */
    VOS_UINT16 dualAntCurId;      /* 天线id */
    VOS_UINT16 dualAntNextState;  /* 下一个跳转阶段 */
    VOS_INT16  rxlevAvgSub125dbm; /* 主天线平均每个统计周期SUB块的能量值 0.125dbm */
    VOS_UINT16 reserved;          /* reserved  */
} GPHY_TOOL_DualAntEstimateInd;

/*
 * 结构说明: 当前和目标天线能量比较阶段判决结果上报，每个调度状态上报一次
 */
typedef struct {
    VOS_UINT16 msgId;                   /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;                  /* tools id */
    VOS_UINT16 roundNum;                /* round计数 */
    VOS_UINT16 compareFinishFlag;       /* 双天线比较是否结束，包括Ia Ib II IIa  */
    VOS_UINT16 dualAntCurState;         /* 当前的调度状态 */
    VOS_UINT16 dualAntNextState;        /* 下一个调度状态 */
    VOS_UINT16 antChangeFlag;           /* 当前state是否切天线标志 */
    VOS_UINT16 dualAntCurId;            /* 切换前的天线id */
    VOS_UINT16 dualAntSecId;            /* 当前比较的的天线id */
    VOS_UINT16 dualAntNextId;           /* 切换后的天线id */
    VOS_INT16  rxlevFilterPriAnt125dbm; /* 当前天线平均每个统计周期的能量滤波值 */
    VOS_INT16  rxlevFilterSecAnt125dbm; /* 目标天线平均每个统计周期的能量滤波值 */
    VOS_INT16  deltaRxlev125dbm;        /* 目标天线与当前天线平均每个统计周期的能量滤波差值 */
    VOS_UINT16 goodEnoughFlag;          /* 本次上报周期是否有足够好天线 */
} GPHY_TOOL_DualAntCompareInd;

/*
 * 结构说明: 当前和目标天线 能量差值的计算过程维测
 */
typedef struct {
    VOS_INT16  initDeltaRxlev125dbm;   /* 两根天线的初始能量差值 【目标天线-当前天线】，单位1/8 dB */
    VOS_INT16  dualAntImbalance125dbm; /* 【当前天线->目标天线】的imbalance值，单位1/8 dB */
    VOS_INT16  sarOffsetVaule;         /* 【当前天线->目标天线】的SarOffsetVaule，单位1/8 dB */
    VOS_UINT16 sarOffsetWeight;        /* SarOffset的权重，单位 % */
    VOS_INT16  finalDeltaRxlev125dbm;  /* 最终两根天线的能量差值 【目标天线-当前天线】，单位1/8 dB */
    VOS_UINT16 rsv;                    /* 保留位 */
} GPHY_TOOL_DualAntDeltaRxlev;

/*
 * 结构说明: 当前和目标天线能量比较阶段判决结果上报，改为二阶并行算法后每个上报周期都上报一次
 */
typedef struct {
    VOS_UINT16                  msgId;                   /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16                  toolId;                  /* tools id */
    VOS_UINT16                  countTotal;              /* 上报周期计数 */
    VOS_UINT16                  countHigh;               /* delta值大于高门限的上报周期计数  */
    VOS_UINT16                  countLow;                /* delta值小于低门限的上报周期计数 */
    VOS_UINT16                  dualAntCurId;            /* 当前天线id */
    VOS_UINT16                  dualAntSecId;            /* 目标天线id */
    VOS_INT16                   rxlevFilterPriAnt125dbm; /* 当前天线平均每个统计周期的能量滤波值 */
    VOS_INT16                   rxlevFilterSecAnt125dbm; /* 目标天线平均每个统计周期的能量滤波值 */
    VOS_INT16                   deltaRxlev125dbm;        /* 目标天线与当前天线平均每个统计周期的能量滤波差值 */
    GPHY_TOOL_DualAntDeltaRxlev dualAntDeltaRxlve;       /* 两天线的差值计算参数 */
} GPHY_TOOL_DualAntCompareSubInd;

/*
 * 结构说明: MAS切天线消息上报
 */
typedef struct {
    VOS_UINT16 msgId;            /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;           /* tools id */
    VOS_UINT16 masDpdtStatus;    /* 切换后MAS dpdt的状态 */
    VOS_UINT16 masChangeAntType; /* 切换类型: Probe or Switch */
} GPHY_TOOL_MasChangeAntInd;

/*
 * 结构说明: 天线切换停滞阶段判决结果上报
 */
typedef struct {
    VOS_UINT16 msgId;            /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;           /* tools id */
    VOS_UINT16 dualAntCurId;     /* 天线id */
    VOS_UINT16 dualAntNextState; /* 下一个跳转阶段 */
    VOS_UINT16 handAntEn;        /* 停滞阶段启动或结束标志 */
    VOS_UINT16 reserved;         /* reserved  */
} GPHY_TOOL_DualAntDelayInd;

/*
 * 结构说明: 主天线能量评估阶段判决结果上报
 */
typedef struct {
    VOS_UINT16 msgId;    /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;   /* TOOL ID，保留 */
    VOS_UINT16 sarDprEn; /* DPR状态 */
    VOS_UINT16 rsv;      /* 保留 */
} GPHY_TOOL_TimeAvgSarDprStateInd;

/*
 * 结构说明: 主天线能量评估阶段判决结果上报
 */
typedef struct {
    VOS_UINT16 msgId;                                        /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;                                       /* TOOL ID，保留 */
    VOS_INT16  gmskSarLimitedMaxPwr[GPHY_PS_MAX_TX_TSN_NUM]; /* GMSK的时间平均SAR最大限制功率，0.1 dBm */
    VOS_INT16  sarLimitedMaxPwr8psk[GPHY_PS_MAX_TX_TSN_NUM]; /* 8PSK的时间平均SAR最大限制功率，0.1 dBm */
    VOS_UINT16 dprState;                                     /* DPR状态 ON/OFF */
    VOS_UINT16 curPeirodAvgPwrIndex;                         /* 当前统计周期索引值,满400会清零 */
    VOS_INT32  curPeirodAvgPwr;                              /* 当前统计周期的平均功率辐射量，0.125 dBm */
    VOS_INT32  timeAvgSarAdjudgeSum;                         /* 当前判决周期平均功率总和,单位0.125dbm */
    VOS_INT32  averagePower;                                 /* 判决周期的平均功率辐射量，0.125 dBm */
    VOS_INT32  curPeirodAvgPwrLine;                          /* 当前统计周期的平均功率线性值 */
    VOS_INT32  adjudgeSumLine;                               /* 当前判决周期平均功率线性值总和 */
    VOS_INT32  averagePowerLine;                             /* 判决周期的平均功率线性值 */
    VOS_UINT32 curSlice;                                     /* 当前的slice */
} GPHY_TOOL_TimeAvgSarPeriodInd;

/*
 * 结构说明: RF RESET执行上报维测消息
 */
typedef struct {
    VOS_UINT16 msgId;                   /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;                  /* TOOL ID，保留 */
    VOS_UINT16 dsdsFlag;                /* DSDS标志 1-DSDS 0-DSDA */
    VOS_UINT16 dsdsVersion;             /* DSDS版本 1-DSDS2.0 0DSDS1.0 */
    VOS_UINT16 recordRfResetChannelId;  /* 记录的RF RESET通道 */
    VOS_UINT16 gsmRxRfSsiChannel;       /* 6362 Gsm Rx所在RFIC */
    VOS_UINT16 gsmRfConnection;         /* gsm Rf A/B通道配置 */
    VOS_UINT16 rsv;                     /* 保留 */
} GPHY_TOOL_RfResetReport;

/*
 * 结构说明: G主模强停从模时上报维测消息
 */
typedef struct {
    VOS_UINT16                                msgId;     /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16                                toolId;    /* TOOL ID，保留 */
    VOS_UINT32                                gapLength; /* 分沟时配置的沟长,单位Qb */
    VOS_UINT32                                curFn;     /* 当前的帧号 */
    VOS_UINT16                                curQb;     /* 当前的qb */
    GPHY_TOOL_MntnMasterStopSlaveReasonUint16 slaveTask; /* 从模类型 */
} GPHY_TOOL_MasterStopSlaveMeasInd;

/*
 * 结构说明: GSM DC Offset Report
 */
typedef struct {
    VOS_UINT16               msgId;        /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16               toolId;       /* tools id */
    VOS_UINT16               curChannelId; /* 当前通道  */
    UPHY_TOOL_DcoffsetUint16 updateType;
    VOS_UINT16               freqInfo;
    VOS_UINT16               gain;
    VOS_INT16                remainI;
    VOS_INT16                remainQ;

    VOS_INT16  initDcoci;   /* 初始值,RFIC控制值为0x80对应的I路DC值 */
    VOS_INT16  initDcocq;   /* 初始值,RFIC控制值为0x80对应的Q路DC值 */
    VOS_INT16  rfDci;       /* 校准后,RFIC I路DC控制值 */
    VOS_INT16  rfDcq;       /* 校准后,RFIC Q路DC控制值 */
    VOS_INT16  remainDcoci; /* 校准后,I路残余DC值 */
    VOS_INT16  remainDcocq; /* 校准后,Q路残余DC值 */
    VOS_UINT16 blockMode;   /* 当前的block模式 */
    VOS_UINT16 antPath;     /* 主分集天线 */
} GPHY_TOOL_DcOffsetReport;

/*
 * 结构说明: 单时钟下当前ppm，scpll配置值，温度值，AFC值
 */
typedef struct {
    VOS_INT32 ppmValue; /* ppm */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32 pllFracValue;  /* Scpll 小数部分调整值 */
    VOS_UINT32 pllInterValue; /* Scpll 整数部分调整值 */
#endif
    VOS_UINT32 afcValue;         /* AFC */
    VOS_INT16  temperatureValue; /* 温度 */
    VOS_UINT16 reserved;
} WPHY_TOOL_SingleXoInfo;

/*
 * 结构说明: 前端RSSI信息
 */
typedef struct {
    VOS_UINT32 ant1Rssi; /* RSSI 1414 */
    VOS_UINT32 ant2Rssi; /* RSSI 1414 */
} WPHY_TOOL_Rssi;

#if (FEATURE_ON == FEATURE_EASYRF)
/*
 * 结构说明: 前端状态信息
 */
typedef struct {
    WPHY_TOOL_AeasyrfRxStatusUint16 intraRxStatus; /* 为TRUE本频RX打开，否则本频RX关闭 */
    WPHY_TOOL_AeasyrfTxStatusUint16 intraTxStatus; /* 为TRUE本频TX打开，否则本频TX关闭 */
} WPHY_TOOL_RfStatus;
#else
/*
 * 结构说明: 前端状态信息
 */
typedef struct {
    VOS_UINT32 rxConfigMode;
    VOS_UINT32 rxPllMode;
    VOS_UINT32 txConfigMode;
    VOS_UINT32 txPllMode;
    VOS_UINT32 rxAgc; /* RxAgc状态 */
    VOS_UINT32 txAgc; /* TxAgc状态 */
} WPHY_TOOL_RfStatus;

/*
 * 结构说明: block状态信息
 */
typedef struct {
    VOS_UINT16 powerAfterRrc;
    VOS_UINT16 powerBeforeRrc;
    VOS_UINT16 dataValid;
    VOS_UINT16 reserved;
} WPHY_TOOL_BlockStateInfo;

#endif

/*
 * 结构说明: MIPI回读结果
 */
typedef struct {
    VOS_UINT32 data : 8;     /* 回读结果 */
    VOS_UINT32 mipiPort : 4; /* mipi port 0~7 */
    VOS_UINT32 slaveId : 4;  /* slave id 0~15 */
    VOS_UINT32 regAddr : 16; /* reg addr 0~65535 */
} UPHY_TOOL_MipiRdResult;

/*
 * 结构说明: TRANSVER ON配置地址信息
 */
typedef struct {
    VOS_UINT32 ctuLastSetTimestamp;   /* CTU通道最后一次改变时间 */
    VOS_UINT32 ctuSetCnt;             /* CTU通道总改变次数 */
    VOS_UINT32 ctuLastCleanTimestamp; /* CTU通道最后一次清空时间 */
    VOS_UINT32 ctuCleanCnt;           /* CTU通道总清空次数 */
} UPHY_TOOL_CtuChangeInfo;

/*
 * 结构说明: 前端信息
 */
typedef struct {
    /* SSI_EN开关/RSSI/AGC档位/DCOFFSET/AFC/BLOCK、NOBLOCK状态天线开关/ABB线控/TRCV_ON状态/单双天线状态 */
    WPHY_TOOL_SingleXoInfo singleXoInfo;      /* 单时钟下当前ppm，scpll配置值，温度值，AFC值 */
    WPHY_TOOL_Rssi         feRssi;            /* RSSI 1414 */
    WPHY_TOOL_RfStatus     rfStatus;          /* RF状态信息 */
    VOS_UINT32             wagcSwitch;        /* WAGC_SWITCH 1400 */
    VOS_UINT32             intMaskLteRcvW[2]; /* BBPCOMM_INT_MASK_LTE_RCV_W_ADDR */
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32 chnMasterModeRx; /* Channel master mode */
    VOS_UINT32 chnMasterModeTx; /* Channel master mode */
    VOS_UINT32 chnSlaveMode;
    VOS_UINT32 chnSlaveChn[8];

    UPHY_TOOL_CtuChangeInfo masterRxInfo; /* 主模CTU通道Rx变化信息 */
    UPHY_TOOL_CtuChangeInfo masterTxInfo; /* 主模CTU通道Tx变化信息 */
    UPHY_TOOL_CtuChangeInfo slaveInfo;    /* 从模CTU通道变化信息 */

    VOS_UINT16 estWorkMode;          /* 谐波消除模式 */
    VOS_UINT16 spurType;             /* 谐波类型 */
    VOS_UINT32 freqHarmDcStatus;     /* bit2:频点变化延迟标记 bit1:谐波去除使能 bit0:去DC估计使能 */
    VOS_UINT32 listenCtu;            /* 侦听通道寄存器信息 */
    VOS_UINT32 trafficCtu0;          /* 业务通道信息 */
    VOS_UINT32 trafficCtu1;          /* 业务通道信息 */
    VOS_UINT32 mipiState;            /* Mipi状态 */
    VOS_UINT32 rfRpt;                /* RF上报 */
    VOS_UINT32 crossMipiLineRpt;     /* CrossMIpi线控信息 */
    VOS_UINT32 crossLnaLineRpt;      /* CrossLNa线控信息 */
    VOS_UINT32 wphyDpdRpt;           /* DPD状态上报信息 */
    VOS_UINT32 wbbp2EtRpt;           /* ET状态上报信息 */
    VOS_UINT16 ctuRealAntStatus;     /* Ctu天线状态 */
    VOS_UINT16 increaseTxMaxPowerEn; /* 提升上行发射功率是否使能 */
#endif
#if (FEATURE_EASYRF == FEATURE_ON)
    VOS_UINT16 softTas   : 1;  /* 软件维护的TAS状态 */
    VOS_UINT16 softMas   : 1;  /* 软件维护的MAS状态 */
    VOS_UINT16 ctuTas    : 1;  /* CTU维护的TAS状态 */
    VOS_UINT16 ctuMas    : 1;  /* CTU维护的MAS状态 */
    VOS_UINT16 preferTas : 1;  /* 最优天线TAS状态 */
    VOS_UINT16 preferMas : 1;  /* 最优天线MAS状态 */
    VOS_UINT16 forceMap  : 2;  /* TAS/MAS强切标志位 */
    VOS_UINT16 tasNvSw   : 1;  /* TAS功能使能开关 */
    VOS_UINT16 fs2Sw     : 1;  /* FS2开关 */
    VOS_UINT16 bandType  : 2;  /* 频段类型 */
    VOS_UINT16 swSarSw   : 2;  /* 滑窗SAR开关 */
    VOS_UINT16 reserved  : 2;  /* 保留位 */
#else
    VOS_UINT16 antExchangeInd; /* 当前天线位置 */
#endif
    VOS_UINT16 ipcFlg;   /* 当前ipc资源锁 */
    VOS_UINT16 wtcMode;        /* WTC模式 */
    VOS_UINT16 shareSel3gp;
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32               ant1Dcr;
    VOS_UINT32               ant2Dcr;
    WPHY_TOOL_BlockStateInfo blockStateInfo[2]; /* block状态相关信息 */
#else
    VOS_UINT32 clkGatingValue;      /* B5000 配置的BBP 时钟门控值 */
#endif
} WPHY_TOOL_FeInfo;

/*
 * 结构说明: 低功耗相关信息
 */
typedef struct {
    /* RF/PA/BBP/ABB上下电状态，不睡原因 */
    VOS_UINT16 guBbpTopMemOffFlg;    /* GuBbp MEM是否下电判断 */
    VOS_UINT16 guBbpTopMemStatus;    /* 存储当前的上电状态 */
    VOS_UINT32 guBbpLpPurposeBitmap; /* TOP1B2B上下电操作目的,是因为TURBO还是DPA */
    VOS_UINT16 bbpMasterOffFlg;      /* BBP MASTER是否下电判断 */
    VOS_UINT16 bbpMasterStatus;      /* 存储当前BBP MASTER的上电状态 */
} WPHY_TOOL_LowpowerInfo;

/*
 * 结构说明: 多径搜索单元1相关信息
 */
typedef struct {
    VOS_UINT32 en;        /* 单元1使能信息 0xFD902020 */
    VOS_UINT32 mode;      /* 单元1模式 0xFD902024 */
    VOS_UINT32 scram;     /* 扰码 0xFD902028 */
    VOS_UINT32 cpichSync; /* 帧头信息 0xFD90202C */
    VOS_UINT32 winOffset; /* 窗位置 0xFD902030 */
} WPHY_TOOL_MultiputhUnit1Info;

/*
 * 结构说明: 多径搜索结果信息
 */
typedef struct {
    VOS_UINT32 fingerEnergy; /* 径能量 */
    VOS_UINT32 fingerPos;    /* 径相位 */
} WPHY_TOOL_FingerInfo;

/*
 * 结构说明: 单元0 1 2的 F0最强径信息
 */
typedef struct {
    WPHY_TOOL_FingerInfo ant1FingerInfo[WPHY_MNTN_MP_WIN_MAX_NUM];
    WPHY_TOOL_FingerInfo ant2FingerInfo;
} WPHY_TOOL_MpFingerInfo;

/*
 * 结构说明: 双天线 单元0 1 2的 F0最强径信息
 */
typedef struct {
    WPHY_TOOL_FingerInfo ant1FingerInfo[WPHY_MNTN_MP_WIN_MAX_NUM];
    WPHY_TOOL_FingerInfo ant2FingerInfo[WPHY_MNTN_MP_WIN_MAX_NUM];
} WPHY_TOOL_TwoAntMpFingerInfo;

/*
 * 结构说明: 解调径相关信息
 */
typedef struct {
    WPHY_TOOL_FingerInfo demodFingerInfo[WPHY_MNTN_DEMOD_FINGER_MAX_NUM];
    VOS_UINT32           fingerEn;
    VOS_UINT32           fingerAntInd;
} WPHY_TOOL_DemodFingerInfo;

/*
 * 结构说明: 主小区窗头尾信息
 */
typedef struct {
    VOS_UINT32 priWinTop; /* 窗头信息 0xFD9040C0 */
    VOS_UINT32 priWinEnd; /* 窗尾信息 0xFD9040C4 */
} WPHY_TOOL_PriWinInfo;

/*
 * 结构说明: 软窗头尾信息
 */
typedef struct {
    VOS_UINT32 softWinTop; /* 软窗头信息 0xFD9040C8 */
    VOS_UINT32 softWinEnd; /* 软窗尾信息 0xFD9040CC */
} WPHY_TOOL_SoftWinInfo;

/*
 * 结构说明: 链路扰码初相
 */
typedef struct {
    VOS_UINT16 rlScram[WPHY_MNTN_RL_SCRAM_MAX_NUM];
    VOS_UINT16 reserved;
} WPHY_TOOL_RlScramInfo;

/*
 * 结构说明: 测量调度状态
 */
typedef struct {
    VOS_UINT16 intraMeasureStatus;       /* 本频测量状态 */
    VOS_UINT16 interMeasureStatus;       /* 异频测量状态  */
    VOS_UINT16 interRatMeasureStatus;    /* 异系统测量状态 */
    VOS_UINT16 interRatbasicStatus;      /* 异系统BSIC验证状态 */
#if FEATURE_LTE                          /* _H2ASN_Skip */
    VOS_UINT16 interRatLteMeasureStatus; /* 异系统LTE测量状态 */
#endif /* FEATURE_LTE */                 /* _H2ASN_Skip */
    VOS_UINT16 reserved;
} WPHY_TOOL_MeasureCtrlInfo;

/*
 * 结构说明: 系统定时信息
 */
typedef struct {
    VOS_UINT32 sysTime;      /* 系统定时 */
    VOS_UINT32 cpichPathPos; /* 跟踪小区CPICH最早径在绝对时标上的位置 */
    VOS_UINT32 dpchPathPos;  /* 跟踪小区SCCPCH/DPCH最早径在绝对时标上的位置 */
} WPHY_TOOL_SysTimeInfo;

/*
 * 结构说明: 误码信息
 */
typedef struct {
    VOS_UINT32 errorBlocks; /* 错块数  */
    VOS_UINT32 totalBlocks; /* 总块数 */
} WPHY_TOOL_ErrorCodeInfo;

/*
 * 结构说明: R99下行信道误码统计
 */
typedef struct {
    WPHY_TOOL_ErrorCodeInfo blerInfo[WPHY_MNTN_BLER_RPT_MAX_NUM];
} WPHY_TOOL_R99BlerInfo;

/*
 * 结构说明: DPA译码误码统计
 */
typedef struct {
    WPHY_TOOL_ErrorCodeInfo errorCode[WPHY_MNTN_DPA_ERROR_CODE_RPT_MAX_NUM];
} WPHY_TOOL_DpaErrorCodeInfo;

/*
 * 结构说明: DPA 均衡解调径信息
 */
typedef struct {
    WPHY_TOOL_FingerInfo eqFingerInfo[WPHY_MNTN_EQ_FINGER_RPT_MAX_NUM];
} WPHY_TOOL_DpaEqFingerInfo;

/*
 * 结构说明: DPA 载波CQI信息
 */
typedef struct {
    VOS_UINT16 cqiInfo[WPHY_MNTN_MAX_CARRIER_NUM];
} WPHY_TOOL_DpaCqiInfo;

/*
 * 结构说明: 随机接入状态信息
 */
typedef struct {
    VOS_UINT16 accessStatus;
    VOS_UINT16 accessProcess;    /* 随机接入上报状态 */
    VOS_UINT16 avoidOccasionFlg; /* 随机接入避开occasion标记 */
    /* 0:正常起occasion；1:信令随机接入期间不起occasion；2:所有随机接入期间均不起occasion */
    VOS_UINT16 raOccasionType;
    VOS_INT16  prachInitRscp;    /* 随机接入计算初始功率使用的RSCP */
    VOS_UINT16 reserved;
} WPHY_TOOL_RaStatusInfo;

/*
 * 结构说明: PI可维可测信息
 */
typedef struct {
    VOS_UINT16 sfn;        /* PI SFN值 */
    VOS_UINT16 slot;       /* PI SLOT值 */
    VOS_UINT16 chip;       /* PI CHIP值 */
    VOS_UINT16 piAckNum;   /* 解到PI为ACK的个数 */
    VOS_UINT16 piTotalNum; /* 解到PI总个数 */
    VOS_UINT16 reserved;
} WPHY_TOOL_PiInfo;

/*
 * 结构说明: 上行发射功率信息
 */
typedef struct {
    VOS_INT16  txAvrgPwr;
    VOS_INT16  txMaxPwr;
    VOS_INT16  txMinPwr;
    VOS_INT16  filterDpcchPwr;
    VOS_INT16  allowUlMaxPower;
    VOS_UINT16 reserved;
#if (FEATURE_OFF == FEATURE_EASYRF)
    VOS_UINT32 mrxCorr;  /* BBP上报线性值 */
    VOS_INT16  mrxPower; /* 根据MRX线性值计算的功率，不包括模式补偿，结果有一点偏差 */
    VOS_INT16  paTemp;
#endif
} WPHY_TOOL_TxPowInfo;

/*
 * 结构说明: 上行发射功率信息
 */
typedef struct {
    VOS_UINT16 edrxActive; /* E-DRX状态有效下，T321超时后进入非连续接收状态指示 */
    VOS_UINT16 burstPhase; /* burst接收的当前阶段 */
} WPHY_TOOL_EdrxInfo;

/*
 * 结构说明: 当前帧TPC统计值
 */
typedef struct {
    VOS_UINT32 dlTpcS0To7;
    VOS_UINT32 dlTpcS8To15;
    VOS_UINT32 ulTpc;
} WPHY_TOOL_TpcInfo;

/*
 * 结构说明: 各信道功率因子
 */
typedef struct {
    VOS_UINT16 bc;
    VOS_UINT16 bd;
    VOS_UINT16 bhs;
    VOS_UINT16 reserved;
} WPHY_TOOL_GainFactor;

/*
 * 结构说明: 压模信息
 */
typedef struct {
    VOS_UINT32 dlGapPara;
    VOS_UINT32 ulGapPara;
} WPHY_TOOL_CmInfo;

/*
 * 结构说明: 时间测量信息
 */
typedef struct {
    VOS_UINT16 sfncfnMeasFlag;
    VOS_INT16  scramCodeNum;
} WPHY_TOOL_TimeMeaInfo;

/*
 * 结构说明: FET特性控制全局信息结构体
 */
typedef struct {
    VOS_UINT16 fetStatus;       /* FET使能开关,开:UPHY_TRUE,关:UPHY_FALSE */
    VOS_UINT16 startProtectCnt; /* 启动保护计数,单位帧 */
    VOS_UINT32 fetCloseVote;    /* FET开关投票,每个bit代表1票,为1指示投关的票,为0指示投开的票 */
    VOS_UINT32 fetRxOpenVote;   /* FET帧级下行接收开关投票,每个bit代表1票,为1指示投开的票,为0指示投关的票 */
    VOS_UINT32 decodeInfo;      /* FET提前译码信息上报 */
    WPHY_TOOL_FetFramePatternInfo framePatternInfo; /* FET帧级开关模板信息 */
} WPHY_TOOL_FetCtrlInfo;

/*
 * 结构说明: FET_SRB盲检测判断全局信息结构体
 */
typedef struct {
    VOS_UINT16 fetBlindStatus;    /* FET SRB 盲检测开关，打开UPHY_TRUE,关闭UPHY_FALSE */
    VOS_UINT16 fetSrbCheckStatus; /* FET SRB 场景评估动作状态，已经检测过为:UPHY_TRUE,没检测过为:UPHY_FALSE */
    VOS_UINT16 fetSrbExistStatus; /* 指示是否存在SRB包:UPHY_TRUE:存在，UPHY_FALSE:不存在 */
    VOS_UINT16 fetSrbWinCount;    /* 统计接受数据平滑框内帧数 */
    VOS_UINT16 fetDecodeSlotNum;  /* 启动译码时隙数 */
    VOS_UINT16 fetPwrLoopIndex;   /* 指示当前维护的功率数组中最旧帧信息位置，范围[0,15] */
    VOS_UINT32 po3Linear;         /* DPCCH和DPDCH之间PO偏差线性值 */
    VOS_UINT32 signalPwrAcc;      /* 统计平滑框内信号噪声 */
    VOS_UINT32 noisePwrAcc;       /* 统计平滑框内信号噪声 */
} WPHY_TOOL_FetSrbInfo;

/*
 * 结构说明: 控制译码同失步软判决相关信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    VOS_UINT16 dmodeSyncInd;         /* 当前帧解调同失步状态 */
    VOS_UINT16 winLen;               /* 解调译码软判决统计平滑框的长度 */
    VOS_UINT16 winCalLen;            /* 解调译码软判决统计平滑窗实际计算长度 */
    VOS_UINT16 winIndex;             /* 解调译码软判决统计平滑框维护系数 */
    VOS_UINT16 demSyncWinCount;      /* 解调译码软判决统计平滑框统计帧数计量 */
    VOS_UINT16 qin;                  /* 同步判决Qin门限 */
    VOS_UINT16 qout;                 /* 同步判决Qout门限 */
    VOS_UINT16 isRfValidFrameCnt;    /* 标识当前RF资源是否可用 */
    VOS_UINT32 curFrameSignalPwrAvg; /* 统计平滑框内当前帧信号噪声平均值 */
    VOS_UINT32 curFrameNoisePwrAvg;  /* 统计平滑框内当前帧信号噪声平均值 */
    VOS_UINT32 winSignalPwrAvg;      /* 统计平滑框内信号平均值 */
    VOS_UINT32 winNoisePwrAvg;       /* 统计平滑框内噪声平均值 */
} WPHY_TOOL_SoftDemSyncInfoRpt;

/*
 * 结构说明: 分集任务投票值，0表示无效，1表示开，2表示关
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
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
 * 结构说明: WPHY DSDS可维可测信息上报
 */
typedef struct {
    /*
     * bit0:当前帧slot0的资源情况
     * bit3:上一帧slot3的资源情况
     * bit6:上一帧slot6的资源情况
     * bit9:上一帧slot9的资源情况
     * bit12:上一帧slot12的资源情况
     */
    VOS_UINT16 dsdsResourceFlag;
    VOS_UINT16 dsdsOpenFlag;
    VOS_UINT16 currentChannel;
    VOS_UINT16 reserved;
} WPHY_TOOL_DsdsInfo;

/*
 * 结构说明: DAC值
 */
typedef struct {
    VOS_UINT32 bitByte0 : 8;
    VOS_UINT32 bitByte1 : 8;
    VOS_UINT32 bitByte2 : 8;
    VOS_UINT32 bitByte3 : 8;
} WPHY_TOOL_MipidevData;

/*
 * 结构说明: WPHY CLT可维可测信息上报
 */
typedef struct {
    VOS_UINT16            sn;            /* 当前的SN号 */
    VOS_UINT16            atTuneType;    /* AT命令指示的闭环调谐类型 */
    VOS_UINT16            cltFailBitmap; /* 当前闭环检测不能启动的原因WPHY_CLT_FAIL_ENUM_UINT16 */
    VOS_UINT16            cltState;      /* 当前闭环检测的消息收发状态 */
    VOS_UINT16            isCrossMipi;   /* 是否使用crossmipi的值 */
    VOS_UINT16            detectState;   /* 采数状态 */
    VOS_UINT16            detectCnt;     /* 采数间隔 */
    VOS_UINT16            clValideFlag;
    VOS_UINT16            hvDacMode;
    VOS_UINT16            clValidCnt; /* 配置CL之后开始计数,之后每帧slot0 +1 */
    WPHY_TOOL_MipidevData hvDacValue;
} WPHY_TOOL_CltInfo;

/*
 * 结构说明: WPHY各状态公共可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_TimingInfo         timingInfo;      /* 定时上报信息(公共携带SFN CFN SLOT SLICE信息) */
    VOS_UINT16                   currentFreq;     /* 当前主小区频点 */
    VOS_UINT16                   sysSramcode;     /* 当前主小区扰码 */
    WPHY_TOOL_RlScramInfo        rlScramInfo;     /* 链路0、6扰码初相 */
    WPHY_TOOL_DemodFingerInfo    demodFingerInfo; /* 所有有效解调径能量径位置 */
    VOS_UINT16                   mpPurpose;       /* 当前多径任务目的 */
    VOS_UINT16                   reserved;
    VOS_UINT32                   mpResourceMap;   /* 多径任务资源情况 */
    WPHY_TOOL_MultiputhUnit1Info unit1Info;       /* 多径搜索单元1相关信息 */
    WPHY_TOOL_MpFingerInfo       mpFingerInfo;    /* 单元0,1,2 f0多径能量及相位，通过多径中断获取，不另外读 */
    WPHY_TOOL_PriWinInfo         priWinInfo;      /* 主小区窗头尾 */
    WPHY_TOOL_MeasureCtrlInfo    measureCtrlInfo; /* 测量任务调度状态 */
    VOS_UINT16                   intraStatusFlg;  /* 当前同频小区搜索的标记 */
    VOS_UINT16                   interStatusFlg;  /* 当前异频小区搜索的标记 */
    /*
     * 小区搜索调度状态:bit0~1:当前同频三步法的类型;bit2~3:stepb搜索使能;
     * bit4:step1搜索使能。bit5:step1搜索到的时隙头是否用于本频Step23处理指示;
     * bit6:stepB的扰码处理模式.bit8:Step23搜索使能
     */
    VOS_UINT32                   csTaskStatus;
    WPHY_TOOL_R99BlerInfo r99BlerInfo;            /* 下行信道误码统计 */
    VOS_UINT32            demEn;                  /* 0-15bit为解调信道使能，16bit为传输信道使能 */
    WPHY_TOOL_DsdsInfo    dsdsInfo;
    VOS_UINT16            ant2State; /* 0--TX&RX off;1--TX open;2--RX open;3--TX&RX open */
#if (FEATURE_ON == FEATURE_EASYRF)
    /*
     *  bit[0:3]表示天线连接状态,0表示无线，1表示有线；
     *  bit[4:7]表示有无测试卡,0表示不是测试卡，1表示是测试卡
     *  bit[8:11]表示RF SUSPEND状态，0表示无效，1表示有效
     */
    VOS_UINT16 cardAntConnRfSuspendState;
#else
    /* bit[0:3]表示天线连接状态,0表示无线，1表示有线；bit[4:7]表示有无测试卡,0表示不是测试卡，1表示是测试卡 */
    VOS_UINT16 cardAndAntConnState;
#endif
    WPHY_TOOL_CltInfo cltInfo;
#if (FEATURE_ON == FEATURE_EASYRF)
    WPHY_TOOL_WbbpIntraInterStatusInfo wbbpIntraInterStatus; /* BBP本异频状态信息 */
#endif
    /* 主卡BBP共享MEM模式选择
     * 5'b00000:MST模式。
     * 5'b00001:GSM模式。
     * 5'b00010:LTE正常模式。
     * 5'b00100:UMTS模式。
     * 5'b01000:无效(原TDS模式）
     * 5'b10010:LTE EMTC模式。
     */
    VOS_UINT32 shareMemMode;
} WPHY_TOOL_CommBusinessInfo;

/*
 * 结构说明: IDLE态可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;            /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId;          /* 保留字段 */
    WPHY_TOOL_FeInfo           feInfo;           /* 前端信息 */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* 公共业务信息 */
} WPHY_TOOL_FreeMntnRpt;

/*
 * 结构说明: IDLE态可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;           /* 保留字段 */

    WPHY_TOOL_FeInfo           feInfo;           /* 前端信息 */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* 公共业务信息 */
    WPHY_TOOL_LowpowerInfo     lowPowerInfo;     /* 低功耗相关 */
    WPHY_TOOL_PiInfo           piInfo;
} WPHY_TOOL_IdleMntnRpt;

/*
 * 结构说明: SAR回退值可维可测信息上报
 */
typedef struct {
    VOS_INT16        sarBackOff;      /* SAR1-SAR4 回退值总和 = Sarsensor + Sarnormal + Sarspecial + Sarmodem */
    VOS_INT16        rseSarBackOff;   /* RSE回退值 = max(sarSensorRse,sarNormalRse)*/
} WPHY_TOOL_SarMntnRpt;


/*
 * 结构说明: FACH态可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    WPHY_TOOL_FeInfo           feInfo;           /* 前端信息 */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* 公共业务信息 */
    WPHY_TOOL_RaStatusInfo     raStatus;
    WPHY_TOOL_SysTimeInfo      sysTimeInfo;       /* 系统定时，最早径信息 */
    WPHY_TOOL_DpaErrorCodeInfo dpaErrorCode;      /* DPA译码误码统计 */
    WPHY_TOOL_DpaEqFingerInfo  dpaEqFinger;       /* 均衡解调径信息 */
    WPHY_TOOL_GainFactor       gainFactor;        /* 各信道功率因子 */
    WPHY_TOOL_EdrxInfo         edrx;              /* EDRX维测信息 */
    VOS_INT16                  sirTarget;         /* 外环功控值 */
    VOS_UINT16                 olpcCtrlMode;      /* 外环功控控制模式BLER或者BER */
    VOS_UINT32                 maxAndMinTxPow;    /* 最大最小功率值 0xFD90C0A4 */
    VOS_UINT32                 dpchSccpchDataPow; /* DPCH/SCCPCH信道子域能量值 */
    VOS_UINT32                 paStatus;          /* PA上下电状态 */
    VOS_UINT16                 pcUlChanEn;        /* bit由低到高分别表示上行E-DCH、DCH、RACH、HSUPA、功控使能信息 */
    VOS_UINT16                 compBypass;        /* Pd检测的值是否用于功率补偿 */
    VOS_INT16                  freqComp;
    VOS_INT16                  sarBackOff;              /* 非B5000的固定SAR回退值 */
    WPHY_TOOL_SarMntnRpt       allSarBackOff;           /* B5000的所有的SAR回退值 */
    VOS_UINT32                 pdDetectRslt;            /* 实际发射功率-理论发射功率，单位0.1dBm。 */
    VOS_UINT32                 nextOccasionMeasRatType; /* 下一帧occasion目的,值的意义对应VOS_RATMODE_ENUM_UINT32 */
} WPHY_TOOL_FachMntnRpt;

/*
 * 结构说明: DCH态可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    WPHY_TOOL_FeInfo           feInfo;           /* 前端信息 */
    WPHY_TOOL_CommBusinessInfo commBusinessInfo; /* 公共业务信息 */
    WPHY_TOOL_SoftWinInfo      softWinInfo;
    WPHY_TOOL_TxPowInfo        txPow;             /* 上行发射功率 */
    WPHY_TOOL_TpcInfo          tpcInfo;           /* 当前帧TPC统计值 */
    WPHY_TOOL_DpaErrorCodeInfo dpaErrorCode;      /* DPA译码误码统计 */
    WPHY_TOOL_DpaEqFingerInfo  dpaEqFinger;       /* 均衡解调径信息 */
    WPHY_TOOL_DpaCqiInfo       dpaCqiInfo;        /* DPA 载波CQI信息 */
    WPHY_TOOL_GainFactor       gainFactor;        /* 各信道功率因子 */
    WPHY_TOOL_SysTimeInfo      sysTimeInfo;       /* 系统定时，最早径信息 */
    WPHY_TOOL_CmInfo           cmInfo;            /* 压模情况 */
    WPHY_TOOL_TimeMeaInfo      timeMeaInfo;       /* 时间测量情况 */
    VOS_UINT32                 bbpDmodeSyncInd;   /* 当前帧解调同失步状态 */
    VOS_UINT16                 dmodeSyncInd;      /* 当前帧解调同失步状态 */
    VOS_UINT16                 frameSyncInd;      /* 当前帧同失步状态 */
    VOS_INT16                  sirTarget;         /* 外环功控值 */
    VOS_UINT16                 olpcCtrlMode;      /* 外环功控控制模式BLER或者BER */
    VOS_UINT32                 dpchSccpchDataPow; /* DPCH/SCCPCH信道子域能量值 */
    VOS_UINT32                 berError;          /* BER误码统计 */
    VOS_UINT32                 maxAndMinTxPow;    /* 最大最小功率值 0xFD90C0A4 */
    VOS_UINT32                 paStatus;          /* PA上下电状态 */
    VOS_UINT16                 pcUlChanEn;        /* bit由低到高分别表示上行E-DCH、DCH、RACH、HSUPA、功控使能信息 */
    VOS_UINT16                 compBypass;        /* Pd检测的功率值是否用于功率补偿 */
    VOS_INT16                  freqComp;
    VOS_INT16                  sarBackOff;     /* 非B5000的固定SAR回退值 */
    WPHY_TOOL_SarMntnRpt       allSarBackOff;  /* B5000的所有的SAR回退值 */
    VOS_UINT32                 pdDetectRslt;   /* 实际发射功率-理论发射功率，单位0.1dBm。 */
    WPHY_TOOL_FetCtrlInfo      fetCtrlInfo;    /* FET信息 */
    WPHY_TOOL_FetSrbInfo       wphyFetSrbCtrl; /* FET盲检测信息 */
} WPHY_TOOL_DchMntnRpt;

/*
 * 结构说明: SOCP流量控制可维可测周期上报结构
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    VOS_UINT32 channelId;    /* 源通道id */
    VOS_INT8   chanName[12]; /* 源通道名称 */
    VOS_UINT32 deltaTime;    /* 上报时间间隔 slice */
    VOS_UINT32 packageLen;   /* 上报时间段内上报的数据包总长度 byte（包括所有要写入的数据包：写入成功的，写入失败的） */
    VOS_UINT32 packageNum;   /* 上报时间段内上报的数据包总次数（包括所有要写入的数据包：写入成功的，写入失败的） */
    VOS_UINT32 abandonLen;   /* 上报时间段内被丢弃数据长度 byte */
    VOS_UINT32 abandonNum;   /* 上报时间段内被丢弃数据总次数 */
    VOS_UINT32 thrputEnc;    /* 上报周期内编码源吞吐率,byte/s，使用此项进行绘图 */
    VOS_UINT32 overFlow50Num; /* 上报周期内编码源buff占用超过50%的次数，使用此项进行绘图 */
    VOS_UINT32 overFlow80Num; /* 上报周期内编码源buff占用超过80%的次数，使用此项进行绘图 */
} WPHY_TOOL_SocpMntnInfo;

/*
 * 结构说明: 小区搜网实网信息统计
 */
typedef struct {
    VOS_UINT32 totalTime; /* 搜网总时长 */
    VOS_UINT32 totalCnt;  /* 搜网次数 */
} WPHY_TOOL_MntnCsInfo;

/*
 * 结构说明: 下行信号能量强度统计
 */
typedef struct {
    VOS_UINT32 weakSignalCnt;   /* W接收信号强度统计-弱信号 */
    VOS_UINT32 normalSignalCnt; /* W接收信号强度统计-中信号 */
    VOS_UINT32 strongSignalCnt; /* W接收信号强度统计-强信号 */
} WPHY_TOOL_MntnSignalInfo;

/*
 * 结构说明: W从模实网信息统计
 */
typedef struct {
    VOS_UINT32 slaveCellSearchCnt; /* W从模小区搜索总时长统计 */
    VOS_UINT32 slaveMeasCnt;       /* W从模测量总次数统计 */
    VOS_UINT32 slaveWorkTime;
} WPHY_TOOL_MntnSlaveInfo;

/*
 * 结构说明: FACH态实网信息统计
 */
typedef struct {
    VOS_UINT32 accessCntPerLevel[WPHY_TOOL_MNTN_ACCESS_PWR_MAX_LEVEL]; /* W随机接入次数统计 */
} WPHY_TOOL_MntnFachInfo;

/*
 * 结构说明: DRX态实网信息统计
 */
typedef struct {
    VOS_UINT32               totalTime;        /* DRX态总时间 */
    VOS_UINT32               totalCnt;         /* DRX态解PI总次数 */
    VOS_UINT32               totalWorkTime;    /* DRX态总工作时间 */
    VOS_UINT32               workShortTimeCnt; /* DRX态工作时间在指定范围内的周期次数，工作时间小于15ms */
    VOS_UINT32               workLongTimeCnt;  /* DRX态工作时间在指定范围内的周期次数 */
    WPHY_TOOL_MntnSignalInfo idleSignalInfo;   /* IDLE态接收信号强度统计  */
    WPHY_TOOL_MntnSignalInfo pchSignalInfo;    /* PCH态接收信号强度统计 */
    VOS_UINT32               forceWakeCnt;     /* DRX态强制唤醒次数 */
    VOS_UINT32               interRatMeasCnt;  /* DRX态异系统测量总次数 */

    VOS_UINT32 idleInterCellSearchCnt; /* IDLE态异频小区搜索总时长 */
    VOS_UINT32 idleInterMeasCnt;       /* IDLE态异频小区测量总次数 */
    VOS_UINT32 idleCbsTotalTime;       /* IDLE态公共广播块接收时长 */
    VOS_UINT32 idleSibRcvCnt;          /* IDLE态系统消息块分集关闭时接收次数 */
    VOS_UINT32 idleAnt2SibRcvCnt;      /* IDLE态系统消息块分集打开时接收次数 */
} WPHY_TOOL_MntnDrxInfo;

/*
 * 结构说明: 下行信号能量强度统计
 */
typedef struct {
    VOS_UINT32 ulWeakPowerCnt;    /* 发射功率统计 */
    VOS_UINT32 ulMiddle1PowerCnt; /* 发射功率统计 */
    VOS_UINT32 ulMiddle2PowerCnt; /* 发射功率统计 */
    VOS_UINT32 ulMiddle3PowerCnt; /* 发射功率统计 */
    VOS_UINT32 ulMiddle4PowerCnt; /* 发射功率统计 */
    VOS_UINT32 ulMiddle5PowerCnt; /* 发射功率统计 */
    VOS_UINT32 ulStrongPowerCnt;  /* 发射功率统计 */
} WPHY_MNTN_UlEnergyInfo;

/*
 * 结构说明: D态实网信息统计
 */
typedef struct {
    VOS_UINT32             csOnlyTotalTime;  /* CS业务总时长 */
    VOS_UINT32             psOnlyTotalTime;  /* PS业务总时长 */
    VOS_UINT32             csAndPsTotalTime; /* CS&PS业务总时长 */
    WPHY_MNTN_UlEnergyInfo psOnlyUlPower;    /* 只有PS业务发射功率统计 */
    WPHY_MNTN_UlEnergyInfo csUlPower;        /* CS业务发射功率统计，CS和PS同时存在的情况也归入CS任务统计 */

    VOS_UINT32 ant2CsTotalTime; /* W CS主分集接收总时间统计 单位为10ms个数 */
} WPHY_TOOL_MntnDchInfo;

/*
 * 结构说明: 实网信息统计
 */
typedef struct {
    VOS_UINT16              modeflg;            /* 模式标志位 */
    VOS_UINT16              length;             /* 有效数据段长度，不包含模式标志和长度标识 */
    WPHY_TOOL_MntnCsInfo    powerCostCsInfo;    /* 搜网信息 */
    WPHY_TOOL_MntnDrxInfo   powerCostDrxInfo;   /* IDLE态信息 */
    WPHY_TOOL_MntnFachInfo  powerCostFachInfo;  /* FACH态统计信息 */
    WPHY_TOOL_MntnDchInfo   powerCostDchInfo;   /* D态信息 */
    WPHY_TOOL_MntnSlaveInfo powerCostSlaveInfo; /* 从模统计信息 */
} WPHY_TOOL_MntnPowerCostInfo;

/*
 * 结构说明: 实网信息统计记录用
 */
typedef struct {
    VOS_UINT32                  stateStartSlice;   /* Idle态或free态cell_search起始时间 */
    VOS_UINT32                  wakeSlice;         /* Idle态唤醒 */
    VOS_UINT32                  csPsStartSlice;    /* CS/PS业务起始时间 */
    VOS_UINT16                  cellSearchFlg;     /* 处于cell_search_req下发到cell_search_ind上报之间的标记位 */
    VOS_UINT16                  idleFlg;           /* 处于IDLE态的标记位 */
    WPHY_MNTN_WakeCntTypeUint16 wakeStatus;        /* 唤醒类型的标记位 */
    VOS_UINT16                  backUpCsFlg;       /* 上一时刻的CS状态 */
    VOS_UINT16                  backUpPsFlg;       /* 上一时刻的PS状态 */
    WPHY_TOOL_MntnBoolUint16    cbsStartFlag;      /* CBS启动标记 */
    VOS_UINT32                  cbsStartSlice;     /* CBS启动时间 */
    VOS_UINT32                  idleMeasCntBackup; /* IDLE态第一轮异频小区测量启动前的测量总次数备份 */
} WPHY_TOOL_MntnRecordPowerCostInfo;

/*
 * 结构说明: TAS_STATUS report功能打开和关闭设置
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;  /* 保留字段 */
    VOS_UINT32                tasCtrl0; /* 控制字 */
    VOS_UINT32                tasCtrl1;
    VOS_UINT32                tasCtrl2;
    VOS_UINT32                tasCtrl3;
} UPHY_TOOL_TasStatusReportReq;

/*
 * 结构说明: TAS_STATUS report 上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;           /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolId;          /* 保留字段 */
    VOS_UINT16                band;            /* 当前频段  */
    VOS_UINT16                athorizeStatus;  /* TASMAS授权状态 */
    VOS_UINT16                dpdtStateSteady; /* 稳态天线位置 */
    VOS_UINT16                dpdtStateTemp;   /* 暂态天线位置 */
    VOS_INT16                 antRscp[4];      /* 天线0到3信号强度  */
} WPHY_TOOL_TasStatusReportInd;

/*
 * 结构说明: TAS_STATUS report 上报
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;         /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;       /* 保留字段 */
    VOS_UINT16                currentBand;   /* 当前切换频段 */
    VOS_UINT16                authorizeFlag; /* 最新TAS授权状态 TAS=0,MAS=0/TAS=1,MAS=0/TAS=0,MAS=1/TAS=1,MAS=1 */
    VOS_UINT16                wholeSwitchDpdtStatus;  /* 整轮切换后DPDT状态 */
    VOS_UINT16                singleSwitchDpdtStatus; /* 单次切换后DPDT状态 */
    VOS_INT16                 antRssi125dbm[4];       /* 天线0到3信号强度  */
} GPHY_TOOL_TasStatusReportInd;

/*
 * 结构说明: 配置的时刻
 */
typedef struct {
    VOS_UINT16 currentQueueType;     /* 当前任务是否为广播类任务,1:任务队列选择广播队列;0:任务队列选择A解调队列 */
    VOS_UINT16 currentTaskFreqIndex; /* 当前任务所属载波 */
    VOS_UINT16 queueIndex;           /* 当前任务对应任务队列的索引 */
    VOS_UINT16 band;                 /* 频段 */
    VOS_UINT16 freq;                 /* 频点 */
    VOS_UINT16 scramCode;            /* 扰码 */
    VOS_UINT16 tdIndicator;          /* 广播任务的分集类型 */
    VOS_UINT16 pccpchTaskType;       /* 广播任务类型 */
    VOS_UINT16 pccpchTaskStatus;     /* 广播任务状态 */
    VOS_UINT16 schdlPriority;        /* 调度优先级 */
} WPHY_TOOL_RakePccpchSchdlInfo;

/*
 * 结构说明: 设置LTE测量标记的指示
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_CfgTimeInfo         cfgTime; /* Pcpch配置时刻 */
    WPHY_TOOL_RakePccpchSchdlInfo cfgInfo; /* Pcccpch配置信息 */
} WPHY_TOOL_RakePccpchSchdlInd;

/*
 * 结构说明: 配置的时刻
 */
typedef struct {
    VOS_UINT16 band;             /* 频段 */
    VOS_UINT16 freq;             /* 频点 */
    VOS_UINT16 scramCode;        /* 扰码 */
    VOS_UINT16 tdIndicator;      /* 广播任务的分集类型 */
    VOS_UINT16 pccpchTaskType;   /* 广播任务类型 */
    VOS_UINT16 pccpchTaskStatus; /* 广播任务状态 */
    VOS_UINT16 schdlPriority;    /* 调度优先级 */
    VOS_UINT16 rsv;
} WPHY_TOOL_RakeBcchQueueInfo;

/*
 * 结构说明: BCH广播队列信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16   msgId;                                  /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  toolsId;                                /* 保留字段 */
    WPHY_TOOL_CfgTimeInfo       cfgTime;                                /* Pcpch配置时刻 */
    WPHY_TOOL_RakeBcchQueueInfo queueInfo[WPHY_MNTN_BCCH_QUEUE_LENGTH]; /* BCCH队列信息 */
} WPHY_TOOL_RakeBcchQueueInd;

/*
 * 结构说明: 配置的时刻
 */
typedef struct {
    VOS_UINT16 band;             /* 频段 */
    VOS_UINT16 freq;             /* 频点 */
    VOS_UINT16 scramCode;        /* 扰码 */
    VOS_UINT16 tdIndicator;      /* 广播任务的分集类型 */
    VOS_UINT16 pccpchTaskType;   /* 广播任务类型 */
    VOS_UINT16 pccpchTaskStatus; /* 广播任务状态 */
    VOS_UINT16 schdlPriority;    /* 调度优先级 */
    VOS_UINT16 rsv;
} WPHY_TOOL_RakeAdemodQueueInfo;

/*
 * 结构说明: ADEMOD队列信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16     msgId;                                   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;                                 /* 保留字段 */
    WPHY_TOOL_CfgTimeInfo         cfgTime;                                 /* ADEMOD配置时刻 */
    WPHY_TOOL_RakeAdemodQueueInfo queueInfo[WPHY_MNTN_ADMOD_QUEUE_LENGTH]; /* BCCH队列信息 */
} WPHY_TOOL_RakeAdemodQueueInd;

/*
 * 7 UNION定义
 */
/*
 * 联合体说明: WPHY可维可测信息上报
 */
typedef union {
    WPHY_TOOL_FreeMntnRpt freeMntnRpt; /* FREE态可维可测信息上报 */
    WPHY_TOOL_IdleMntnRpt idleMntnRpt; /* IDLE态可维可测信息上报 */
    WPHY_TOOL_FachMntnRpt fachMntnRpt; /* FACH态可维可测信息上报 */
    WPHY_TOOL_DchMntnRpt  dchMntnRpt;  /* DCH态可维可测信息上报 */
} WPHY_TOOL_MntnRpt;

/*
 * 结构说明: SAR DPR ON事件可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;  /* 保留字段 */
    WPHY_TOOL_MntnBoolUint16  sarDprEn; /* DPR状态 */
    VOS_UINT16                rsv;      /* 保留 */
} WPHY_TOOL_SarTimeAvgDprEventInd;

/*
 * 结构说明:时间平均 SAR参数可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                      /* _H2ASN_Skip */    /* 原语类型 */
    VOS_UINT16                toolsId;                    /* 保留字段 */
    WPHY_TOOL_MntnBoolUint16  timeAvgSarLimitPowerFlag;   /* 时间平均SAR限制功率标记 */
    VOS_INT16                 timeAvgSarMaxTxPowerLimit;  /* 时间平均SAR限制功率标记有效时最大限制功率,单位0.1dbm */
    VOS_INT16                 timeAvgSarStaticPrdPwrAvg;  /* 当前统计周期内的平均功率,单位0.125dbm */
    VOS_INT16                 timeAvgSarAdjudgePrdPwrAvg; /* 当前判决周期内的平均功率,单位0.125dbm */
    VOS_INT32                 timeAvgSarAdjudgeSum;       /* 当前判决周期平均功率总和,单位0.125dbm */
    VOS_UINT16                timeAvgSarStaticPrdIndex;   /* 当前统计周期个数满400会清零 */
    VOS_UINT16                reserved;
    VOS_UINT32                staticPrdPwrLineAvg;  /* 当前统计周期内的平均功率线性值 */
    VOS_UINT32                adjudgePrdPwrLineAvg; /* 当前判决周期内的平均功率线性值 */
    VOS_UINT32                adjudgeLineSum;       /* 当前判决周期平均功率总和线性值 */
} WPHY_TOOL_SarTimeAvgParaInfo;

/*
 * 结构说明: 合并后的均衡模板
 */
typedef struct {
    VOS_UINT16 startCfn;
    /* 从低到高共15位有效，每个BIT表示对应时隙的操作是打开(值为1)还是关闭(值为0) */
    VOS_UINT16 pattBitMap[WPHY_MNTN_CPC_DRX_PATTERN_FRAME_NUM];
} WPHY_TOOL_CpcDrxPatternInfo;

/*
 * 结构说明: HS-DPCCH信道CQI上报DRX控制静态参数信息
 */
typedef struct {
    VOS_UINT16 npre[WPHY_MNTN_HSDPA_OM_CARERIER_NUM]; /* 提前判断的子帧数 */
} WPHY_TOOL_CpcDrxCqiStaticInfo;

/*
 * 结构说明: edch dtx参数
 */
typedef struct {
    VOS_UINT16 ueDtxCyc1; /*  Units of subframes */
    VOS_UINT16 ueDtxCyc2; /*  Units of subframes */
    VOS_UINT16 macDtxCyc; /*  Units of subframes */
    VOS_UINT16 reserved;
} WPHY_TOOL_DtxEDchTti;

/*
 * 结构说明: HS-SCCH和HS_PDSCH信道接收控制信息
 */
typedef struct {
    VOS_UINT16 hsscchSchedSlot;             /* HS-SCCH和HS_PDSCH信道接收评估时隙 */
    VOS_UINT16 hsscchEquaRfSlotNum;         /* 均衡打开和RF打开的持续时间，以时隙为单位 */
    VOS_UINT16 hsscchInactiveEquaRfSlotCnt; /* 译码数据有效时，用于Inactivity_Threshold_for_UE_DRX_cycle的计数器 */
    VOS_UINT16 hsscchInactiveEquaRfSlotNum; /* 译码数据有效时，用于Inactivity_Threshold_for_UE_DRX_cycle的门限 */
} WPHY_TOOL_CpcDrxHsscchDrxCtrl;

/*
 * 结构说明: HS-SCCH和HS_PDSCH信道ACK发送控制信息
 */
typedef struct {
    VOS_UINT16 post;      /* 发送ACK时,可能需要发送POST */
    VOS_UINT16 rfSlotNum; /* HSPDSCH查询到译码时，到ACK发送之间，RF打开的持续时间，以时隙为单位 */
} WPHY_TOOL_CpcDrxHsdpcchAckCtrl;

/*
 * 结构说明: HS-DPCCH信道CQI上报DRX控制信息
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16         cqiSchedFlag;      /* TRUE:需要CQI DRX控制调度,否则不需要 */
    WPHY_TOOL_MntnCpcDrxCqiPriUint16 cqiDtxPri;         /* 综合考虑BBP和软件维护得出的CQI优先级 */
    WPHY_TOOL_CpcDrxCqiStaticInfo    cqiStaticNpreInfo; /* HS-DPCCH信道CQI上报DRX控制静态参数信息提前判断的子帧数 */
    /* CQI DRX调度控制的均衡PATTERN信息 */
    WPHY_TOOL_CpcDrxPatternInfo cqiEquaPattInfo[WPHY_MNTN_CPC_DRX_CQI_PATTERN_NUM];
    WPHY_TOOL_CpcDrxPatternInfo cqiRfPattern[WPHY_MNTN_CPC_DRX_CQI_PATTERN_NUM];    /* RF PATTERN */
} WPHY_TOOL_CpcDrxCqiCtrl;

/*
 * 结构说明: CPC DRX info
 */
typedef struct {
    WPHY_TOOL_MntnCpcDrxEquaStatusUint16 cpcEquaWorkState; /* 当前均衡工作状态 */
    VOS_UINT16                           reserve;
    WPHY_TOOL_CpcDrxPatternInfo          cpcEquaCombPatt;                                       /* 合并后的均衡模板 */
    WPHY_TOOL_CpcDrxPatternInfo          cpcEquaPattInfo[WPHY_MNTN_CPC_DRX_EQUA_CTRL_TYPE_NUM]; /* 均衡PATTERN信息 */
} WPHY_TOOL_CpcDrxEquaCtrl;

/*
 * 结构说明: CPC DRX info
 */
typedef struct {
    WPHY_TOOL_MntnCpcDrxRfStatusUint16 rfWorkState; /* 当前RF工作状态 */
    VOS_UINT16                         reserve;
    VOS_UINT32                         phySubTaskStaBitMap;                            /* 影响RF的子任务状态bitmap */
    WPHY_TOOL_CpcDrxPatternInfo        rfPattInfo[WPHY_MNTN_CPC_DRX_RF_CTRL_TYPE_NUM]; /* RF PATTERN信息 */
    WPHY_TOOL_CpcDrxPatternInfo        combRfPatt;                                     /* 合并后的RF模板 */
} WPHY_TOOL_CpcDrxRfCtrl;

/*
 * 结构说明: DCH态可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16                msgId;                 /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                               toolsId;               /* 保留字段 */
    WPHY_TOOL_MntnCpcUldpcchCycleStateUint16 ulDpcchPattCycleState; /* 综合主辅载波的状态后的出的cycle状态的最大集合 */
    WPHY_TOOL_MntnBoolUint16                 agchRgchSchedFlag;     /* 是否进行AGCH/RGCH调度标志, TRUE:要, FALSE:不要 */
    WPHY_TOOL_CpcDrxEquaCtrl                 equaCtrl;              /* 均衡模板 */
    WPHY_TOOL_CpcDrxCqiCtrl                  cqiDrxCtrl;            /* CQI DRX调度控制 */
    WPHY_TOOL_CpcDrxRfCtrl                   rfCtrl;                /* RF模板 */
    WPHY_TOOL_DtxEDchTti                     dtxEdchTti;            /* edch dtx参数 */
    WPHY_TOOL_CpcDrxPatternInfo              ulDpcchC1RfPatt;       /* UL Dpcch cycle1 RF PATTERN */
    WPHY_TOOL_CpcDrxPatternInfo              ulDpcchC2RfPatt;       /* UL Dpcch cycle2 RF PATTERN */
    WPHY_TOOL_CpcDrxHsscchDrxCtrl            hsscchDrxCtrl;         /* HsscchDrx控制结构 */
    WPHY_TOOL_CpcDrxHsdpcchAckCtrl           hsdpcchAckCtrl;        /* HsscchAck控制结构 */
    /* EDCH发送 一个载波上的 RF PATTERN，主辅载波的pattern分别刷新 */
    WPHY_TOOL_CpcDrxPatternInfo drxEdchCtrlRfPatt[WPHY_MNTN_HSUPA_OM_CARERIER_NUM];
} WPHY_TOOL_CpcMntnRpt;

/*
 * 结构说明: 小区信息上报
 */
typedef struct {
    VOS_INT16 scramCodeNum; /* 扰码 */
    VOS_INT16 freqInfo;     /* 频点 */
} WPHY_TOOL_IcCellInfo;

/*
 * 结构说明: 各个载波下IC小区信息
 */
typedef struct {
    VOS_UINT16           cellNum;                               /* 小区数量 */
    VOS_UINT16           reserved;                              /* 保留位 */
    WPHY_TOOL_IcCellInfo icCellInfo[WPHY_MNTN_IC_MAX_CELL_NUM]; /* 原语类型 */
} WPHY_TOOL_IcCellList;

/*
 * 结构说明: IC小区信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId; /* _H2ASN_Skip */               /* 原语类型 */
    VOS_UINT16                toolsId;                               /* 保留字段 */
    WPHY_TOOL_IcCellList      icCellList[WPHY_MNTN_MAX_CARRIER_NUM]; /* 原语类型 */
} WPHY_TOOL_IcCellReportInd;

/*
 * 结构说明: 各个载波下IC小区信息
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_IcCellReportReq;

/*
 * 结构说明: DTX信息上报
 */
typedef struct {
    VOS_UINT16 ulDtxStatus;          /* 原语类型,0--disable 1--enable */
    VOS_UINT16 mainCarrierEnable;    /* 原语类型,0--disable 1--enable */
    VOS_UINT16 secondCarrierEnable;  /* 原语类型,0--disable 1--enable */
    VOS_UINT16 mainCarrierActive;    /* 原语类型,0--Inactive 1--active */
    VOS_UINT16 secondCarrierActive;  /* 原语类型,0--Inactive 1--active */
    VOS_UINT16 mainCarrierOrder;     /* 原语类型,0--Inactive 1--active */
    VOS_UINT16 secondCarrierOrder;   /* 原语类型,0--Inactive 1--active */
    VOS_UINT16 mainCarrierPattern;   /* 原语类型 */
    VOS_UINT16 secondCarrierPattern; /* 原语类型 */
    VOS_UINT16 cqiDtxPriority;       /* 原语类型,0--low 1--high */
    VOS_UINT16 mainCarrierMacDtx;    /* 原语类型,0--continue 1--MAC DTX */
    VOS_UINT16 secondCarrierMacDtx;  /* 原语类型,0--continue 1--MAC DTX */
} WPHY_TOOL_UlDtxInfo;

/*
 * 结构说明: DRX信息上报
 */
typedef struct {
    VOS_UINT16 drxActiveStatus; /* 原语类型,0--Inactive 1--active */
    VOS_UINT16 reserved;        /* 保留为 */
} WPHY_TOOL_DlDrxInfo;

/*
 * 结构说明: CPC信息
 */
typedef struct {
    VOS_UINT16 enableStatus;       /* 原语类型,0--disable 1--enable */
    VOS_UINT16 activeStatus;       /* 原语类型,0--Inactive 1--active */
    VOS_UINT16 mainCarrierOrder;   /* 原语类型,0--Inactive 1--active */
    VOS_UINT16 secondCarrierOrder; /* 原语类型,0--Inactive 1--active */
} WPHY_TOOL_CpcInfo;

/*
 * 结构说明: RTT状态上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;        /* 保留字段 */
    WPHY_TOOL_UlDtxInfo       ulDtxInfo;      /* 原语类型 */
    WPHY_TOOL_DlDrxInfo       dlDRxInfo;      /* 原语类型 */
    WPHY_TOOL_CpcInfo         hsScchLessInfo; /* 原语类型 */
} WPHY_TOOL_RttStatusReportInd;

/*
 * 结构说明: 各个载波下IC小区信息
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_RttStatusReportReq;

/*
 * 结构说明: DPA信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                 /* 保留字段 */
    VOS_UINT16                enableStatus;            /* 原语类型,0--disable 1--enable */
    VOS_UINT16                activeStatus;            /* 原语类型,0--Inactive 1--active */
    VOS_UINT16                mainCarrierOrder;        /* 原语类型,0--Inactive 1--active */
    VOS_UINT16                secondCarrierOrder;      /* 原语类型,0--Inactive 1--active */
} WPHY_TOOL_DpaInfoInd;

/*
 * 结构说明: DPA 上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_DpaInfoReportReq;

/*
 * 结构说明: P值信息
 */
typedef struct {
    VOS_UINT32 pValueTotalCnt[WPHY_MNTN_P_VALUE_NUM]; /* 原语类型,0--disable 1--enable */
} WPHY_TOOL_PValueInfo;

/*
 * 结构说明: HS-SCCH译码信息
 */
typedef struct {
    VOS_UINT32           subFrameNum;    /* 原语类型,0--disable 1--enable */
    VOS_UINT32           hsScchAttempt;  /* 原语类型,0--Inactive 1--active */
    VOS_UINT32           hsScchSucc;     /* 原语类型,0--Inactive 1--active */
    VOS_UINT32           hsScchSuccRate; /* 原语类型,0--Inactive 1--active */
    WPHY_TOOL_PValueInfo pValueInfo;
} WPHY_TOOL_HsscchDecodeInfo;

/*
 * 结构说明: HS-SCCH译码信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId;                                     /* 保留字段 */
    WPHY_TOOL_HsscchDecodeInfo hsScchDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 原语类型 */
} WPHY_TOOL_HsscchStatisticReportInd;

/*
 * 结构说明: HSSCCH译码上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
} WPHY_TOOL_HsscchStatisticReportReq;

/*
 * 结构说明: HS-DSCH译码信息
 */
typedef struct {
    VOS_UINT32 qpskNum;          /* 原语类型,0--disable 1--enable */
    VOS_UINT32 qam16Num;         /* 原语类型,0--disable 1--enable */
    VOS_UINT32 qam64Num;         /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 support64QamFlag; /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 sbSub;            /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 sbPlus;           /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 firstSuccNum;     /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 secondSuccNum;    /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 thirdSuccNum;     /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 fourthSuccNum;    /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 fifthSuccNum;     /* 原语类型,0--Inactive 1--active */
    VOS_UINT32 sixthSuccNum;     /* 原语类型,0--Inactive 1--active */
} WPHY_TOOL_HsdschTbsInfo;

/*
 * 结构说明: HS-DSCH译码信息
 */
typedef struct {
    VOS_UINT16 tbsLowThredhold;  /* 原语类型,0--disable 1--enable */
    VOS_UINT16 tbsHighThredhold; /* 原语类型,0--disable 1--enable */
} WPHY_TOOL_HsdschTbsThrehold;

/*
 * 结构说明: HS-DSCH译码信息
 */
typedef struct {
    VOS_UINT16                  regionNum;                                  /* 原语类型,0--disable 1--enable */
    VOS_UINT16                  reserved;                                   /* 原语类型,0--disable 1--enable */
    WPHY_TOOL_HsdschTbsThrehold tbsThredholdInfo[WPHY_MNTN_MAX_REGION_NUM]; /* 原语类型,0--Inactive 1--active */
    WPHY_TOOL_HsdschTbsInfo     tbsInfo[WPHY_MNTN_MAX_REGION_NUM];          /* 原语类型,0--Inactive 1--active */
} WPHY_TOOL_HsdschDecodeInfo;

/*
 * 结构说明: HSDSCH译码上报请求
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId;                                     /* 保留字段 */
    WPHY_TOOL_HsdschDecodeInfo hsDschDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 原语类型 */
} WPHY_TOOL_HsdschStatisticReportInd;

/*
 * 结构说明: HSDSCH译码上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId; /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    reserved;
    VOS_UINT16                    regionNum; /* 配置为0-9，必须与ausThredhold中的数目匹配 */
    VOS_UINT16                    thredhold[WPHY_MNTN_MAX_REGION_NUM];
} WPHY_TOOL_HsdschStatisticReportReq;

/*
 * 结构说明: PILOT BER上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;                   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;                 /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;                 /* 上报允许标志 */
    VOS_UINT16                    rptPeriod;               /* 上报周期 */
} WPHY_TOOL_PilotBerReportReq;

/*
 * 结构说明: PILOT 16帧滑窗总比特和误比特信息
 */
typedef struct {
    VOS_UINT16 pilotTotalBitNum; /* Pilot总比特数，16帧滑窗 */
    VOS_UINT16 pilotErrorBitNum; /* Pilot误比特数，16帧滑窗 */
} WPHY_TOOL_PilotBerInfo;

/*
 * 结构说明: PILOT BER上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;      /* 保留字段 */
    WPHY_TOOL_PilotBerInfo    pilotBerInfo; /* 误码率 */  /* 原语类型 */
} WPHY_TOOL_PilotBerReportInd;

/*
 * 结构说明: 传输功控和信噪比上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_TpcAndSirReportReq;

/*
 * 结构说明: 传输功控和误码率上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;  /* 保留字段 */
    VOS_INT16                 dlTpcCnt; /* 传输功控 */ /* 原语类型 */
} WPHY_TOOL_TpcAndSirReportInd;

/*
 * 结构说明: 上行发射功率和主载波上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_PowerReportReq;

/*
 * 结构说明: 上行发射功率和主载波上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;        /* 保留字段 */
    VOS_INT16                 txAvrgPwr;      /* 平均传输功率 */
    VOS_INT16                 filterDpcchPwr; /* 滤波结果 */ /* 原语类型 */
} WPHY_TOOL_PowerReportInd;

/*
 * 结构说明: 主辅载波均衡调节可维可测统计上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsdpaLinkStatisticsReportReq;

/*
 * 结构说明: 主辅载波均衡调节可维可测统计上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    /* 第1行保存主载波每个子帧的CQI，第2行保存辅载波每个子帧的CQI */
    VOS_UINT16                cqiRpt[WPHY_MNTN_MAX_CARRIER_NUM][WPHY_MNTN_SUB_FRAME_NUM];
} WPHY_TOOL_HsdpaLinkStatisticsReportInd;

/*
 * 结构说明: 天线相关信息上报格式
 */
typedef struct {
    VOS_UINT16 antennaEnable; /* 天线前端开关。1，表示打开；0，表示关闭。W、TDS和C共用。 */
    /* BBP上报天线口RSSI值（双载波之和），0.125dB精度。说明：主要用于DRX流程。进入DRX睡眠时，软件从BBP读取寄存器获取。 */
    VOS_UINT16 dspRptRssiRf;
    /*
     * BBP上报用于控制RF增益的状态机。0，表示S1；1，表示S2；2，表示S3；3，表示S4；
     * 4，表示S5；5，表示S6；6，表示S7；7，表示S8。说明：主要用于DRX流程。进入DRX睡眠时，软件从BBP读取寄存器获取。
     */
    VOS_UINT16 dspRptAgcState;
} WPHY_DUAL_AntennaStatisticsReport;

/*
 * 结构说明: 双天线相关信息统计上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_DualAntennaStatisticsReportReq;

/*
 * 结构说明: 双天线相关信息统计上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId; /* 保留字段 */
    WPHY_DUAL_AntennaStatisticsReport dualAntennaStatistics[WPHY_MNTN_WBBP_MAX_ANT_NUM]; /* 主天线和辅天线 */
} WPHY_TOOL_DualAntennaStatisticsReportInd;

/*
 * 结构说明: HSUPA EDCH激活集信息上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsupaEdchActiveSetReportReq;

typedef struct {
    VOS_UINT16 rlScram;      /* HSUPA激活集小区主扰码初相 */
    /* 标识是否在RGCH服务的无线链路集中，0表示该RL不在RGCH服务RLS中，1表示该RL在RGCH服务RLS中 */
    VOS_UINT16 rgServRlFlag;
    /* HICH无线链路的无线链路集号，正常取值范围0~5，7为无效号，表示不属于任何一个HICH无线链路集。 */
    VOS_UINT16 tpcIndex;
    /* 标识小区是否E-DCH信道服务小区，0表示小区不是E-DCH的服务小区，1表示小区是E-DCH的服务小区 */
    VOS_UINT16 servingEdchFlag;
    VOS_UINT16 hsupaFlag; /* 标识小区是否支持HICH，0为不支持，1表示支持 */
} WPHY_HSUPA_EdchActiveSetInfoReport;

/*
 * 结构说明: HSUPA EDCH激活集信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16          msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                         toolsId; /* 保留字段 */
    WPHY_HSUPA_EdchActiveSetInfoReport activeSetInfo[WPHY_MNTN_HSUPA_ACTIVE_SET_MAX_NUM];
} WPHY_TOOL_HsupaEdchActiveSetReportInd;

/*
 * 结构说明: 小区载干比信息上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_RadioParaCellSnrReq;

/*
 * 结构说明: 载干比信息
 */
typedef struct {
    VOS_UINT32 signalPower; /* 信号功率 */
    VOS_UINT32 noisePower;  /* 噪声功率 */
} WPHY_MNTN_CellSnrInfo;

/*
 * 结构说明: 小区载干比信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                                    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                                  /* 保留字段 */
    WPHY_MNTN_CellSnrInfo     cellSnrInfo[WPHY_MNTN_SLOT_NUM_BY_FRAME]; /* DPCH 15个时隙载干比信息 */
    VOS_INT16                 sirTarget;                                /* 计算得到的SIR目标值 */
    VOS_INT16                 reserved;                                 /* 保留字段 */
} WPHY_TOOL_RadioParaCellSnrInd;

/*
 * 结构说明: DPA参数上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HspdschCfgReq;

/*
 * 结构说明: DPA参数的消息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;         /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;       /* 保留字段 */
    VOS_INT32                 dlDpchOffset;  /* DPCH相对于HSSCCH的时间偏移 */
    VOS_UINT16                harqProcesses; /* DPA支持的线程数 */
    VOS_UINT16                reserved;      /* 保留字段 */
} WPHY_TOOL_HspdschCfgInd;

/*
 * 结构说明: HS-SCCH参数上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsscchCfgReq;

/*
 * 结构说明: HS-SCCH参数
 */
typedef struct {
    VOS_UINT16 hsScchNo; /* 使用HS-SCCH编号表示其有效性 */
    VOS_UINT16 ovsf;     /* 主小区HS-SCCH的ovsf码号 */
} HS_SCCH_Configuration;

/*
 * 结构说明: HS-SCCH参数上报的消息结构
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                hsscchNum;
    VOS_UINT16                reserved;
    HS_SCCH_Configuration     hsscchValue[WPHY_MAIN_CELL_HSDPA_SCCH_MAX_NUM];
} WPHY_TOOL_HsscchCfgInd;

/*
 * 结构说明: HS-DPCCH信道配置参数上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsdpcchCfgReq;

/*
 * 结构说明: HS-DPCCH信道配置参数上报消息格式
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;           /* 保留字段 */
    VOS_UINT16                cqiCycle;          /* CQI发送周期 */
    VOS_UINT16                cqiRpt;            /* CQI连续发射次数，取值范围1~4 */
    VOS_UINT16                cqiAck;            /* ACK/NACK连续发射次数，取值范围1~4 */
    VOS_UINT16                hsDpcchChipOffset; /* HS-DPCCH第一个子帧帧头与上行DPCH帧头偏移chip数 */
    VOS_UINT16                poAck;             /* HS-DPCCH信道调制增益因子，最大值38 */
    VOS_UINT16                poNack;            /* HS-DPCCH信道调制增益因子，最大值38 */
    VOS_UINT16                poCqi;             /* HS-DPCCH信道调制增益因子，最大值38 */
    VOS_UINT16                reserved;          /* 保留字段 */
} WPHY_TOOL_HsdpcchCfgInd;

/*
 * 结构说明: HSDPA信道CQI消息上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsHsdpaCqiReq;

/*
 * 结构说明: HSDPA信道CQI消息上报格式
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                        /* 保留字段 */
    VOS_UINT16                carrierNum;                     /* 上报的载波数量 */
    VOS_UINT16                reserved;                       /* 保留字段 */
    VOS_UINT16                cqi[WPHY_MNTN_MAX_CARRIER_NUM]; /* channel quality indicator */
} WPHY_TOOL_HsHsdpaCqiInd;

/*
 * 结构说明: 主辅载波均衡调节可维可测统计上报的消息结构请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsdpaLinkStatReq;

/*
 * 结构说明: 主辅载波均衡调节可维可测统计上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                        /* 保留字段 */
    VOS_UINT32 schedDeltaRate[WPHY_MNTN_MAX_CARRIER_NUM];     /* MAC层调度的瞬时速率，周期内CRC OK的TBS总数 */
    VOS_UINT32 macLayerDeltaRate[WPHY_MNTN_MAX_CARRIER_NUM];  /* MAC层传输的瞬时速率，周期内CRC OK且ACK的TBS总数 */
    VOS_UINT16 hsscchSuccNum[WPHY_MNTN_MAX_CARRIER_NUM];      /* 周期内HS-SCCH Success子帧数 */
    VOS_UINT32 hsdpaDetTotalNum[WPHY_MNTN_MAX_CARRIER_NUM];   /* 周期内HS-SCCH检测帧总数 */
    VOS_UINT32 hsdpaCrcSuccNum[WPHY_MNTN_MAX_CARRIER_NUM];    /* 周期内HS-SCCH检测成功帧中CRC正确的帧数 */
    VOS_UINT32 totalBlocks[WPHY_MNTN_MAX_CARRIER_NUM];        /* 周期内初传+重传总块数 */
    VOS_UINT32 errorBlocks[WPHY_MNTN_MAX_CARRIER_NUM];        /* 周期内初传+重传误块数 */
    VOS_UINT32 hsdpaRetranTxTotal[WPHY_MNTN_MAX_CARRIER_NUM]; /* 周期内重传数 */
    VOS_UINT32 hsdpaInitTxTotal[WPHY_MNTN_MAX_CARRIER_NUM];   /* 周期内初传数 */
    VOS_UINT32 hsdpaRetranTxError[WPHY_MNTN_MAX_CARRIER_NUM]; /* 周期内重传误块数 */
    VOS_UINT32 hsdpaInitTxError[WPHY_MNTN_MAX_CARRIER_NUM];   /* 周期内初传误块数 */
    VOS_UINT16 hsdCodeChannelNum[WPHY_MNTN_MAX_CARRIER_NUM];  /* 周期内UE占用的码道数 */
    VOS_UINT16 hsdpaLessNum[WPHY_MNTN_MAX_CARRIER_NUM];       /* 周期内Less块数 */
    VOS_UINT16 hsdpaNonLessNum[WPHY_MNTN_MAX_CARRIER_NUM];    /* 周期内非Less块数 */
    WPHY_TOOL_MntnBoolUint16 lessActiveFlag[WPHY_MNTN_MAX_CARRIER_NUM]; /* 用于统计周期内Less激活的帧数 */
} WPHY_TOOL_HsdpaLinkStatInd;

/*
 * 结构说明: 上报HS-DSCH译码信息的请求消息结构
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsdpaDecodStatReq;

/*
 * 结构说明: 上报HS-DSCH译码信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId;                                     /* 保留字段 */
    WPHY_TOOL_HsscchDecodeInfo hsScchDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 原语类型 */
} WPHY_TOOL_HsdpaDecStatInd;

/*
 * 结构说明: 上报HS-DSCH译码信息请求的消息结构
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    reserved;
    VOS_UINT16                    regionNum; /* 配置为0-9，必须与ausThredhold中的数目匹配 */
    VOS_UINT16                    thredhold[WPHY_MNTN_MAX_REGION_NUM];
} WPHY_TOOL_HsdpaDecTbsReq;

/*
 * 结构说明: 上报HS-DSCH译码信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;                                       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId;                                     /* 保留字段 */
    WPHY_TOOL_HsdschDecodeInfo hsDschDecodeInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 原语类型 */
} WPHY_TOOL_HsdpaDecTbsInd;

/*
 * 结构说明: 上报HS-DSCH译码信息
 */
typedef struct {
    VOS_UINT32 schedDataRate;    /* 链路可调度传输的数据比特率 */
    VOS_UINT32 nonSchedDataRate; /* 链路不可调度传输的数据比特率 */
} WPHY_MNTN_HsupaLinkThroughputInfo;

/*
 * 结构说明: 上报路测功能控制
 */
typedef struct {
    WPHY_TOOL_MntnRptStatusUint16     reportFlag; /* _H2ASN_Skip */ /* 是否需要上报 */
    VOS_UINT16                        period;     /* _H2ASN_Skip */ /* 上报周期 */
    VOS_UINT16                        periodCnt;  /* _H2ASN_Skip */ /* 上报周期计数 */
    WPHY_TOOL_MntnBoolUint16          rptStatus;  /* _H2ASN_Skip */ /* 上报状态，TRUE表示在上报 */
    WPHY_MNTN_HsupaLinkThroughputInfo throughputInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 主辅载波upa链路吞吐量信息统计 */
} WPHY_MNTN_RptHsupaCtrlInfo;

/*
 * 结构说明: HSUPA链路吞吐量统计上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsupaLinkThroughputRptReq;

/*
 * 结构说明: HSUPA链路吞吐量统计上报的信息结构
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId; /* 保留字段 */
    VOS_UINT16                        carrierNum;
    VOS_UINT16                        reversed;
    WPHY_MNTN_HsupaLinkThroughputInfo throughputInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 主辅载波upa链路吞吐量信息统计 */
} WPHY_TOOL_HsupaLinkThroughputRptInd;

/*
 * 结构说明: HSUPA链路质量信息统计
 */
typedef struct {
    VOS_UINT16 sendDataCnt;      /* 总调度块数 */
    VOS_UINT16 sblerCnt;         /* 调度错误块数 */
    VOS_UINT16 firstSblerCnt;    /* 初传调度块总数 */
    VOS_UINT16 firstSendDataCnt; /* 初传调度错误块数 */
    VOS_UINT16 powerLimitedCnt;  /* power受限统计 */
    VOS_UINT16 sgLimitedCnt;     /* SG受限统计 */
    VOS_UINT16 bufferLimitedCnt; /* buffer受限统计 */
    VOS_UINT16 limitedTotalCnt;  /* 功率受限总块数 */
    VOS_UINT16 happyCnt;         /* happy统计 */
    VOS_UINT16 reserved;         /* 调度的总块数 */
} WPHY_MNTN_HsupaLinkQualityInfo;

/*
 * 结构说明: 上报路测功能控制信息
 */
typedef struct {
    WPHY_TOOL_MntnRptStatusUint16  reportFlag; /* _H2ASN_Skip */ /* 是否需要上报 */
    VOS_UINT16                     period;     /* _H2ASN_Skip */ /* 上报周期 */
    VOS_UINT16                     periodCnt;  /* _H2ASN_Skip */ /* 上报周期计数 */
    WPHY_TOOL_MntnBoolUint16       rptStatus;  /* _H2ASN_Skip */ /* 上报状态，TRUE表示在上报 */
    WPHY_MNTN_HsupaLinkQualityInfo qualityInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 主辅载波的upa链路质量信息 */
} WPHY_MNTN_RptHsupaLinkQualityCtrlInfo;

/*
 * 结构说明: HSUPA链路质量参数上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsupaLinkQualityReq;

/*
 * 结构说明: HSUPA链路质量参数上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16      msgId;                                  /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId;                                /* 保留字段 */
    WPHY_MNTN_HsupaLinkQualityInfo qualityInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 主辅载波的upa链路质量信息 */
} WPHY_TOOL_HsupaLinkQualityRptInd;

/*
 * 结构说明: HSUPA授权信息
 */
typedef struct {
    VOS_UINT32 agValue;    /* AG统计累加值 */
    VOS_UINT32 sgCnt;      /* SG统计值 */
    VOS_UINT32 sgValue;    /* SG统计值 */
    VOS_UINT16 agCnt;      /* AG计数, 计算AG平均值 */
    VOS_UINT16 rgUpCnt;    /* RG up统计 */
    VOS_UINT16 rgHoldCnt;  /* RG hold统计 */
    VOS_UINT16 rgDownCnt;  /* RG down统计 */
    VOS_UINT16 rgTotalCnt; /* RG 总数统计, 用于计算RG比率 */
    VOS_UINT16 totalCnt;   /* 总的统计次数 */
} WPHY_MNTN_HsupaGrantInfo;

/*
 * 结构说明: 上报upa grant路测功能控制
 */
typedef struct {
    WPHY_TOOL_MntnRptStatusUint16 reportFlag;                           /* _H2ASN_Skip */ /* 是否需要上报 */
    VOS_UINT16                    period;                               /* _H2ASN_Skip */ /* 上报周期 */
    VOS_UINT16                    periodCnt;                            /* _H2ASN_Skip */ /* 上报周期计数 */
    WPHY_TOOL_MntnBoolUint16      rptStatus;                            /* _H2ASN_Skip */ /* 上报状态，TRUE表示在上报 */
    WPHY_MNTN_HsupaGrantInfo      grantInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 主辅载波的授权消息统计 */
} WPHY_MNTN_RptHsupaGrantCtrlInfo;

/*
 * 结构说明: HSUPA GRANT统计上报请求消息结构
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_HsupaGrantRptReq;

/*
 * 结构说明: HSUPA GRANT统计上报消息结构
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                                /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                              /* 保留字段 */
    WPHY_MNTN_HsupaGrantInfo  grantInfo[WPHY_MNTN_MAX_CARRIER_NUM]; /* 主辅载波授权信息 */
} WPHY_TOOL_HsupaGrantRptInd;

/*
 * 结构说明: 传输功率上报请求消息结构
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_PwrCtrlPwrRptReq;

/*
 * 结构说明: 传输功率上报消息结构
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;      /* 保留字段 */
    VOS_INT16                 txPwrVal;     /* 传输功率 */
    VOS_INT16                 rxPwrAnt1Val; /* 天线1的接收功率 */
    VOS_INT16                 rxPwrAnt2Val; /* 天线2的接收功率 */
    VOS_UINT16                reserved;     /* 保留 */
} WPHY_TOOL_PwrCtrlPwrRptInd;

/*
 * 结构说明: 传输功率控制TPC上报消息结构
 */
typedef struct {
    WPHY_TOOL_MntnDlTpcCmdUint16 dlTpcCmd; /* 下行传输功率控制字 */
    WPHY_TOOL_MntnUlTpcCmdUint16 ulTpcCmd; /* 上行传输功率控制字 */
} WPHY_MNTN_TpcInfo;

/*
 * 结构说明: 传输功率控制TPC上报请求消息结构
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16     msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnRptStatusUint16 rptFlag;   /* 上报允许标志 */
    VOS_UINT16                    rptPeriod; /* 上报周期 */
} WPHY_TOOL_PwrCtrlTpcRptReq;

/*
 * 结构说明: 传输功率控制TPC上报消息结构
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                                /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                              /* 保留字段 */
    WPHY_MNTN_TpcInfo         tpcInfo[WPHY_MNTN_SLOT_NUM_BY_FRAME]; /* 传输功率控制信息 */
} WPHY_TOOL_PwrCtrlTpcRptInd;

/*
 * 结构说明: W RX校准轨迹打印, 一次所有流程为一个打印单元
 */
typedef struct {
    /* 配置 */
    VOS_UINT16 rxConfigTimeSfn;
    VOS_UINT16 rxConfigTimeSlot;
    VOS_UINT16 rxConfigTimeChip;
    VOS_UINT16 band;
    VOS_UINT16 freq;
    VOS_UINT16 agcCfg;

    /* 结果 */
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

    /* 外置LNA寄存器信息 */
    VOS_UINT16 mainLnaGain0[WPHY_TOOL_FAST_CAL_READ_LNA_REG_NUM]; /* MipiLnaGain0最多读取四个寄存器值 */
    VOS_UINT16 divLnaGain0[WPHY_TOOL_FAST_CAL_READ_LNA_REG_NUM];

    /* Rfic AGC寄存器信息 */
    VOS_UINT16 rficAgcInfo[WPHY_TOOL_FAST_CAL_READ_AGC_REG_NUM]; /* 0主集，1分集 */
} WPHY_TOOL_CalRxMntnUnit;

/*
 * 结构说明: W TX校准轨迹打印, 一次所有流程为一个打印单元
 */
typedef struct {
    VOS_UINT16 freq;     /* 频点 */
    VOS_INT16  power;    /* 功率 */
    VOS_UINT16 slot;     /* 时隙 */
    VOS_UINT16 chip;     /* chip */
    VOS_UINT32 rfRpt;    /* RF上报 */
    VOS_UINT32 paRpt;    /* PA上报 */
    VOS_UINT32 etAptRpt; /* ET配置上报 */
} WPHY_TOOL_CalTxMntnUnit;

/*
 * 结构说明: W RX校准轨迹打印, 一次所有流程为一个打印单元
 */
typedef struct {
    VOS_UINT16              rxValidNum; /* 记录的有效个数 */
    VOS_UINT16              reserved;   /* 保留 */
    WPHY_TOOL_CalRxMntnUnit wphyRxFastCalMntn[WPHY_CAL_RX_FAST_CAL_MAX_VAILD_NUM];
} WPHY_TOOL_CalRxFastMntnUnit;
/*
 * 结构说明: W RX校准轨迹打印, 一次所有流程为一个打印单元
 */
typedef struct {
    VOS_UINT16              txValidNum; /* 记录的有效个数 */
    VOS_UINT16              txFlag;     /* 保留 */
    WPHY_TOOL_CalTxMntnUnit wphyTxFastCalMntn[WPHY_CAL_TX_FAST_CAL_MAX_VAILD_NUM];
} WPHY_TOOL_CalTxFastMntnUnit;

/*
 * 结构说明: W RX校准轨迹打印, 一次所有流程为一个打印单元
 */
typedef struct {
    VOS_UINT16 mask;           /* mask */
    VOS_UINT16 reserved1;      /* 保留 */
    VOS_UINT32 txPllStatus;    /* txpll状态 */
    VOS_UINT32 txPll;          /* txpll状态 */
    VOS_UINT32 txConfig;       /* tx状态 */
    VOS_UINT32 rxPllStatus;    /* RxPll状态 */
    VOS_UINT32 rxPll;          /* RxPll状态 */
    VOS_UINT32 rxConfig;       /* RxPll状态 */
    VOS_UINT32 txETRpt;        /* ET上报 */
    VOS_UINT32 txRfRpt;        /* APC上报 */
    VOS_UINT32 txPaRpt;        /* PA上报 */
    VOS_UINT32 txPdRpt;        /* Pd上报 */
    VOS_UINT32 rxAagc;         /* AGC状态上报 */
    VOS_UINT32 rxAnt1Dc;       /* 天线1直流上报 */
    VOS_UINT32 rxAnt2Dc;       /* 天线2直流上报 */
    VOS_UINT32 rxAnt1RssiRpt;  /* 天线1能量上报 */
    VOS_UINT32 rxAnt2RssiRpt;  /* 天线2能量上报 */
    VOS_UINT32 ratModeFlag;    /* 当前模式指示寄存器上报，低16bit为卡1，高16bit为卡2 */
    VOS_UINT32 wtcModeSel;     /* wtc模式上报 */
    VOS_UINT32 wagcSwitch;     /* wagcSwitch上报 */
    VOS_UINT32 w3gppShareSel;  /* 3gpp主副卡共享资源占用指示上报 */
    VOS_UINT32 rffeMipiEn;     /* RFFE mipi客户端上报 */
    VOS_UINT32 rficMipiEn;     /* RFIC mipi客户端上报 */
    VOS_UINT32 rffeMipiStatus; /* RFFE mipi状态上报 */
    VOS_UINT32 rficMipiStatus; /* RFIC mipi状态上报 */
    VOS_UINT32 reserved2[2];   /* 保留 */
} WPHY_TOOL_AtTrxCalMntnUnit;

/*
 * 结构说明: AT Cal可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId; /* 保留字段 */
    WPHY_TOOL_AtTrxCalMntnUnit wphyAtTrxCalmntn;
} WPHY_TOOL_AtTrxCalMntnRpt;

/*
 * 结构说明: Rx Fast Cal可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16   msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  toolsId; /* 保留字段 */
    WPHY_TOOL_CalRxFastMntnUnit wphyToolRxFastCal;
} WPHY_TOOL_RxFastCalMntnRpt;

/*
 * 结构说明: Tx Fast Cal可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16   msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                  toolsId; /* 保留字段 */
    WPHY_TOOL_CalTxFastMntnUnit wphyToolTxFastCal;
} WPHY_TOOL_TxFastCalMntnRpt;

/*
 * 结构说明: MAS 天线TAS/MAS状态结构体
 */
typedef struct {
    WPHY_TOOL_MntnMasAnttasstatusUint16 tasStatus; /* 天线TAS状态 */
    WPHY_TOOL_MntnMasAntmasstatusUint16 masStatus; /* 天线MAS状态 */
} WPHY_TOOL_MasAntennaStatus;

/*
 * 结构说明: 记录盲切换状态测量信息
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16 sampleInd;     /* 样点指示( 0:表示第一个样点,1:表示非第一个样点 ) */
    VOS_UINT16               calCount;      /* 计算平均值累加次数 */
    VOS_UINT32               measSlice;     /* 第一次测量时的slice */
    VOS_UINT32               ecSumPre;      /* 激活集内小区径能量和 */
    VOS_UINT32               ecSumCurr;     /* 激活集内小区径能量和 */
    VOS_UINT32               ecAvePre;      /* 激活集内单小区平均径能量和 */
    VOS_UINT32               ecAveCurr;     /* 激活集内单小区平均径能量和 */
    VOS_INT16                gainRfPre;     /* 激活集内前一5ms的rf增益和误差 */
    VOS_INT16                gainRfCurr;    /* 激活集内当前5ms内rf增益和误差 */
    VOS_UINT32               ecAveAccPre;   /* 激活集内单小区平均值累加径能量和 */
    VOS_UINT32               ecAveAccCurr;  /* 激活集内单小区平均累加径能量和 */
    VOS_INT16                rscpRfAvePre;  /* T1时间内激活集小区天线口能量平均值 */
    VOS_INT16                rscpRfAveCurr; /* T1时间内激活集小区天线口能量平均值 */
} WPHY_TOOL_MasBlindStateMeasPara;

/*
 * 结构说明: MAS 天线明切能量信息结构体
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16 sampleInd; /* 样点指示( FALSE:表示第一个样点,TRUE:表示非第一个样点 ) */
    VOS_UINT16               reserve;   /* 4字节对齐 */
    VOS_UINT32               measSlice; /* 第一次测量时间点 */
    VOS_INT16                rscp;      /* 天线RSCP */
    VOS_INT16                rssi;      /* 天线RSSI */
} WPHY_TOOL_MasNonblindMeasPara;

/*
 * 结构说明: MAS算法天线资讯可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnBoolUint16        evaFinishFlag;  /* 当前State是哪个State */
    WPHY_TOOL_MntnBoolUint16        evaInherit;     /* 当前State当前天线评估能量是否可以继承 */
    WPHY_TOOL_MasAntennaStatus      antStatus;      /* 当前State当前天线TAS/MAS状态 */
    WPHY_TOOL_MasBlindStateMeasPara blindEnergy;    /* 当前State当前天线盲切采集的激活集所有小区能量信息 */
    WPHY_TOOL_MasNonblindMeasPara   nonBlindEnergy; /* 当前State当前天线明切采集的跟踪单小区能量信息 */
} WPHY_TOOL_MasAntInfoInd;

/*
 * 结构说明: MAS StateI 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;     /* 保留字段 */
    WPHY_TOOL_MntnBoolUint16     leaveStateI; /* 是否离开StateI */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;    /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState;   /* 下一State是哪个State */
    WPHY_TOOL_MntnBoolUint16     iaEnable;    /* Ia是否使能 */
    WPHY_TOOL_MntnBoolUint16     ibEnable;    /* Ib是否使能 */
    VOS_UINT16                   rsv;         /* 四字节对齐 */
    WPHY_TOOL_MasAntInfoInd      antInfo;     /* 当前天线信息 */
} WPHY_TOOL_MasMntnStateiInd;

/*
 * 结构说明: MAS StateIaIb 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;         /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;       /* 保留字段 */
    VOS_UINT16                   roundCount;    /* 当前比较轮数 */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;      /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState;     /* 下一State是哪个State */
    WPHY_TOOL_MntnBoolUint16     earlyExit;     /* 是否足够好而已可以提早冻结 */
    WPHY_TOOL_MntnBoolUint16     destAntBetter; /* 目标天线是否较好, 相当於是否切换天线 */
    WPHY_TOOL_MntnBoolUint16     iaEnable;      /* Ia是否使能 */
    WPHY_TOOL_MntnBoolUint16     ibEnable;      /* Ib是否使能 */
    VOS_UINT16                   rsv;           /* 四字节对齐 */
    WPHY_TOOL_MasAntInfoInd      srcAntInfo;    /* 源天线信息 */
    WPHY_TOOL_MasAntInfoInd      destAntInfo;   /* 目标天线信息 */
} WPHY_TOOL_MasMntnStateiaibInd;

/*
 * 结构说明: MAS StateII 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;         /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;       /* 保留字段 */
    VOS_UINT16                   roundCount;    /* 当前比较轮数 */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;      /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState;     /* 下一State是哪个State */
    WPHY_TOOL_MntnBoolUint16     earlyExit;     /* 是否足够好而已可以提早冻结 */
    WPHY_TOOL_MntnBoolUint16     destAntBetter; /* 目标天线是否较好, 相当於是否切换天线 */
    WPHY_TOOL_MntnBoolUint16     iaEnable;      /* Ia是否使能 */
    WPHY_TOOL_MntnBoolUint16     ibEnable;      /* Ib是否使能 */
    WPHY_TOOL_MntnBoolUint16     holdBlind;     /* 盲切维持标记 */
    WPHY_TOOL_MasAntInfoInd      srcAntInfo;    /* 源天线信息 */
    WPHY_TOOL_MasAntInfoInd      destAntInfo;   /* 目标天线信息 */
} WPHY_TOOL_MasMntnStateiiInd;

/*
 * 结构说明: MAS StateIII 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;  /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState; /* 下一State是哪个State */
} WPHY_TOOL_MasMntnStateiiiInd;

/*
 * 结构说明: MAS TX_STATEI 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;        /* 保留字段 */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;       /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState;      /* 下一State是哪个State */
    VOS_UINT16                   txValidSlotCnt; /* 有效的TX时隙个数计数 */
    /* 功率超过MaxPower - MPR - SarReduction - uhwTxMonitorPowerValue的TX时隙个数计数 */
    VOS_UINT16                   txBeyondMonitorSlotCnt;
    VOS_UINT32                   deltaTime;
} WPHY_TOOL_TasMntnTxStateiInd;

/*
 * 结构说明: MAS StateII RX好天线信息结构体
 */
typedef struct {
    WPHY_TOOL_MasAntennaStatus      antStatus;   /* 当前State当前天线TAS/MAS状态 */
    WPHY_TOOL_MasBlindStateMeasPara blindEnergy; /* 当前State当前天线盲切采集的激活集所有小区能量信息 */
} WPHY_TOOL_MntnTxStateiiRxAntInfo;

/*
 * 结构说明: MAS StateII RX好天线信息结构体
 */
typedef struct {
    /* astAntInfo[0]:源天线信息; astAntInfo[1]:目标天线信息 */
    WPHY_TOOL_MntnTxStateiiRxAntInfo antInfo[WPHY_MNTN_MAS_ANTENNA_BUTT];
    WPHY_TOOL_MntnMasAntennaUint16 betterAnt; /* 最终好天线是源天线还是目标天线 */
    VOS_UINT16                     rsv;       /* 四字节对齐 */
} WPHY_TOOL_MntnTxStateiiRxInfo;

/*
 * 结构说明: MAS_TX_STATEII档位结构
 */
typedef struct {
    VOS_UINT16 srcAntTimeLength;   /* 源(当前)天线时长，单位:ms */
    VOS_UINT16 destAntTimeLength;  /* 目标天线时长，单位:ms */
    VOS_INT16  srcAntTxPowerGain;  /* 源(当前)天线发射功率提升值，单位:0.1dB */
    VOS_INT16  destAntTxPowerGain; /* 目标天线发射功率提升值，单位:0.1dB */
} WPHY_TOOL_MntnTxStateiiLevelInfo;

/*
 * 结构说明: MAS_TX_STATEII档位结构
 */
typedef struct {
    VOS_UINT16                       levelNum;                                   /* 档位表有效档位个数 */
    VOS_UINT16                       rsv;                                        /* 4字节对齐 */
    WPHY_TOOL_MntnTxStateiiLevelInfo levelItem[WPHY_MNTN_TX_STATEII_LEVEL_BUTT]; /* 档位表 */
} WPHY_TOOL_MntnTxStateiiLevelTable;

/*
 * 结构说明: MAS StateII状态结构体
 */
typedef struct {
    VOS_UINT16                  txValidSlotCnt;         /* T1或者T2时间内有效的TX时隙个数计数 */
    VOS_UINT16                  txBeyondCurrSlotCnt;    /* 功率超过Current功率比较门限的TX时隙个数计数 */
    VOS_UINT16                  txBeyondBackOffSlotCnt; /* 功率超过BackOff功率比较门限的TX时隙个数计数 */
    VOS_UINT16                  txBeyondMonitorSlotCnt; /* 功率超过Monitor功率比较门限的TX时隙个数计数 */
    VOS_UINT32                  deltaTime;              /* 子阶段结束时TX能量采集总时长 */
    WPHY_TOOL_MntnTxPowerUint16 t1LimitInfo;            /* T1受限情况 */
    WPHY_TOOL_MntnTxPowerUint16 t2LimitInfo;            /* T2受限情况 */
} WPHY_TOOL_MntnTxStateiiSubStateInfo;

/*
 * 结构说明: MAS TX_STATEII RX好天线判断子阶段可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16             msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                            toolsId;      /* 保留字段 */
    WPHY_TOOL_MntnCtrlModeUint16          ctrlMode;     /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16          currState;    /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 currSubState; /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 nextSubState; /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiRxInfo         rxInfo;       /* 下行天线信息 */
} WPHY_TOOL_TasMntnTxStateiiRxSubInd;

/*
 * 结构说明: MAS TX_STATEII T1 子阶段可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16             msgId;            /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                            toolsId;          /* 保留字段 */
    WPHY_TOOL_MntnCtrlModeUint16          ctrlMode;         /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16          currState;        /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 currSubState;     /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 nextSubState;     /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiLevelUint16    currLevel;        /* 当前档位 */
    VOS_UINT16                            currAnt;          /* 当前阶段天线所在位置 */
    /* 最终档位表，结合原始NV表和两天线SAR回退之后得到的直通天线在前的档位表 */
    WPHY_TOOL_MntnTxStateiiLevelTable finalLevelTable;
    WPHY_TOOL_MntnTxStateiiSubStateInfo subStateT1Info;     /* T1子阶段信息 */
    VOS_INT16                           passThruSarBackOff; /* 直通天线SAR回退值 */
    VOS_INT16                           swapSarBackOff;     /* 交叉天线SAR回退值 */
} WPHY_TOOL_TasMntnTxStateiiT1SubInd;

/*
 * 结构说明: MAS TX_STATEII T2子阶段 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16             msgId;            /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                            toolsId;          /* 保留字段 */
    WPHY_TOOL_MntnCtrlModeUint16          ctrlMode;         /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16          currState;        /* 当前State是哪个State */
    WPHY_TOOL_MntnMasStateUint16          nextState;        /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 currSubState;     /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiSubStateUint16 nextSubState;     /* 当前State是哪个State */
    WPHY_TOOL_MntnTxStateiiLevelUint16    currLevel;        /* 当前档位 */
    WPHY_TOOL_MntnTxStateiiLevelUint16    nextLevel;        /* 下一档位 */
    VOS_UINT16                            rsv;              /* 保留字段 */
    /* 最终档位表，结合原始NV表和两天线SAR回退之后得到的直通天线在前的档位表 */
    WPHY_TOOL_MntnTxStateiiLevelTable finalLevelTable;
    WPHY_TOOL_MntnTxStateiiSubStateInfo subStateT2Info;     /* T2子阶段信息 */
    WPHY_TOOL_MntnTxLevelAdjustUint16   levelAdjustRslt;    /* 升降档判决结果 */
    VOS_UINT16                          currAnt;            /* 当前阶段天线所在位置 */
    VOS_INT16                           passThruSarBackOff; /* 直通天线SAR回退值 */
    VOS_INT16                           swapSarBackOff;     /* 交叉天线SAR回退值 */
} WPHY_TOOL_TasMntnTxSTtateiiT2SubInd;

/*
 * 结构说明: TAS TX_SIGNAL 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;                   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId;                 /* 保留字段 */
    WPHY_TOOL_MntnTxSignalStateUint16 txSignalState;           /* TX发射状态 */
    VOS_UINT16                        txValidSlotCount;        /* 有效的TX时隙个数计数 */
    VOS_UINT16 txBeyondPoorSlotCount; /* 功率超过MaxPower - MPR - SarReduction - uhwTxPoorPowerValue的TX时隙个数计数 */
    /* 功率小于MaxPower - MPR - SarReduction - uhwTxGoodPowerValue的TX时隙个数计数 */
    VOS_UINT16 txLessThanGoodSlotCount;
    VOS_UINT32 deltaTime;               /* 总的统计时长 */
} WPHY_TOOL_TasMntnTxSignalInd;

/*
 * 结构说明: Fs2 TAS TX_SIGNAL 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;                   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId;                 /* 保留字段 */
    WPHY_TOOL_MntnTxSignalStateUint16 txSignalState;           /* 四个TX发射状态 */
    VOS_UINT16                        txValidSlotCount;        /* 有效的TX时隙个数计数 */
    VOS_UINT16 txBeyondPoorSlotCount;     /* 采样周期内TX功率超过Poor门限的统计次数 */
    VOS_UINT16 txLessThanGoodSlotCount;   /* 采样周期内TX功率超过Good门限的统计次数 */
    VOS_UINT32 deltaTime;                 /* 总的统计时长 */
} WPHY_TOOL_Fs2TasMntnTxSignalInd;

/*
 * 结构说明: MAS 明切算法可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;            /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;          /* 保留字段 */
    VOS_INT16                 rscpSrc;          /* 主集天线测量结果 */
    VOS_INT16                 rscpDest;         /* 分集天线测量结果 */
    VOS_UINT16                countLow;         /* Low Count 计数 */
    VOS_UINT16                countHigh;        /* High Count 计数 */
    VOS_UINT16                countTotal;       /* Total Count 计数 */
    VOS_UINT16                rsv;              /* 四字节对齐 */
    WPHY_TOOL_MntnBoolUint16  dualAntEvaFinish; /* 双天线采样评估结束标记  */
    WPHY_TOOL_MntnBoolUint16  newAntBetter;     /* 新天线较好标记 */
    VOS_INT16                 dualAntImbalance;
    VOS_INT16                 sarOffsetVaule;
    VOS_UINT16                sarOffsetWeight;
    VOS_INT16                 finalRscpRfDelta;
} WPHY_TOOL_MasMntnNonblindinfoInd;

/*
 * 结构说明: MAS 左右旋 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16  msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId;   /* 保留字段 */
    WPHY_TOOL_MasAntennaStatus antStatus; /* 当前State当前天线TAS/MAS状态 */
} WPHY_TOOL_MasMntnMasSwitchInd;

/*
 * 结构说明: 滑窗SAR相关参数信息可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;                               /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                             /* 保留字段 */
    VOS_INT32                 sarLimitPowerMinDbm;                 /* 每个子窗允许的最小限制功率, 单位:0.1dbm */
    VOS_UINT32                sarLimitPowerMinMw;                  /* 每个子窗允许的最小限制功率, 单位:1/16mw */
    VOS_INT32                 sarLimitPowerMaxDbm;                 /* 每个子窗允许的最大发射功率, 单位:0.1dbm */
    VOS_UINT32                sarLimitPowerMaxMw;                  /* 每个子窗允许的最大发射功率, 单位:1/16mw */
    VOS_INT32                 sarPeriodPowerSumDbm;                /* 所有100个子窗SAR值的和 单位: 0.1dbm */
    VOS_UINT32                sarPeriodPowerSumMw;                 /* 所有100个子窗SAR值的和 单位:1/16mw */
    VOS_UINT32                sarSubWinLimitPowerMw;               /* 通过滑窗SAR计算出来的限制功率值 单位: 1/16mw */
    VOS_INT32                 sarSubWinLimitPowerDbm;              /* 通过滑窗SAR计算出来的限制功率值 单位: 0.1dbm */
    VOS_INT32                 sarAvgMaxPowerDbm;                   /* SAR最大平均功率；超过此值，意味着SAR超标,单位:0.1dbm */
    VOS_UINT32                sarAvgMaxPowerMw;                    /* SAR最大平均功率；超过此值，意味着SAR超标,单位:1/16mw */
    VOS_INT32                 preSarAvgMaxPowerDbm;                /* 上一帧的sarAvgMaxPower，用来和当前sarAvgMaxPower比较是否发生变化 单位:0.1dbm */
    VOS_UINT32                preSarAvgMaxPowerMw;                 /* 上一帧的sarAvgMaxPower，用来和当前sarAvgMaxPower比较是否发生变化 单位:1/16mw */
    VOS_INT32                 sarSubWinPowerSumDbm;                /* 滑窗SAR每个子窗内4帧发射功率(SAR值)累加和，单位:0.1dbm */
    VOS_UINT32                sarSubWinPowerSumMw;                 /* 滑窗SAR每个子窗内4帧发射功率(SAR值)累加和，单位:1/16mw */
    VOS_UINT16                sarSubWinFrameCount;                 /* 滑窗SAR每个子窗内帧数累加,累加到(4的倍数，由NV决定)后归零 */
    VOS_UINT16                sarSubWinFrameNum;                   /* 滑窗SAR每个子窗内帧数(4的倍数,具体由NV决定) */
    VOS_UINT16                sarLimitWinOffset;                   /* 记录当前子窗位置 */
    WPHY_TOOL_SwSarUpdateReasonUint16    sarUpdateReason;          /* 滑窗SAR算法更新原因 */
} WPHY_TOOL_SwSarCtrlMntnInfo;

/*
 * 结构说明: TAS5.0天线轮询评估结果状态结构体
 */
typedef struct {
    WPHY_TOOL_MasBlindStateMeasPara     antBlindEnergy; /* 盲切采集的激活集所有小区能量信息 */
    WPHY_TOOL_MntnBoolUint16            fs2SampleInd;   /* fs2样点指示( 0:表示第一个样点,1:表示非第一个样点 ) */
    WPHY_TOOL_MntnBoolUint16            evalOverFlag;   /* StateI rx tx都评估结束的标志 */
    WPHY_TOOL_MntnBoolUint16            txEvalOverFlag; /* StateI tx评估结束的标志 */
    VOS_UINT16                          rsv;
} WPHY_TOOL_AntEvaInfo;

/*
 * 结构说明: TAS5.0天线轮询评估结果状态结构体
 */
typedef struct {
    VOS_UINT16                          antStatus;      /* 实际天线状态 */
    VOS_UINT16                          fs2AntStatus;   /* fs2天线状态 */
    VOS_UINT16                          fs2TxAnt;       /* fs2 TX状态 */
    VOS_UINT16                          fs2PrxAnt;      /* fs2 PRX状态 */
    WPHY_TOOL_MntnTxSignalStateUint16   txSignalState;  /* tx评估结果: poor或good */
    VOS_UINT16                          rsv;
    WPHY_TOOL_AntEvaInfo                fs2antEva;
} WPHY_TOOL_MntnAntEvaInfo;

/*
 * 结构说明: FS2 MAS StateI 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;     /* 保留字段 */
    WPHY_TOOL_MntnBoolUint16     leaveStateI; /* 是否离开StateI */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;    /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState;   /* 下一State是哪个State */
    WPHY_TOOL_MntnBoolUint16     fs2AllEvalOverFlag; /* 所有天线均结束标志 */
    WPHY_TOOL_MntnAntEvaInfo     currAntEvaRslt;     /* 当前天线信息 */
} WPHY_TOOL_Fs2MasMntnStateiInd;

/*
 * 结构说明: MAS StateII 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;         /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;       /* 保留字段 */
    VOS_UINT16                   roundCount;    /* 当前比较轮数 */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;      /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState;     /* 下一State是哪个State */
    WPHY_TOOL_MntnBoolUint16     fs2AllEvalOverFlag; /* 所有天线均结束标志 */
    WPHY_TOOL_MntnAntEvaInfo     fs2AntEvaList[WPHY_MNTN_FS2_MAX_ANT_NUM]; /* 当前天线信息 */
} WPHY_TOOL_Fs2MasMntnStateiiInd;

/*
 * 结构说明: FS2 MAS StateIII 可维可测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;     /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId;   /* 保留字段 */
    WPHY_TOOL_MntnCtrlModeUint16 ctrlMode;  /* 控制模式 */
    WPHY_TOOL_MntnMasStateUint16 nextState; /* 下一State是哪个State */
} WPHY_TOOL_Fs2MasMntnStateiiiInd;

/*
 * 结构说明: WPHY 可维可测功能控制下发的结构原语:用来控制功能设置请求
 */
typedef struct {
    VOS_MSG_HEADER
    WPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;     /* 保留字段 */
    VOS_UINT32                funcBitMask; /* 对应Bit位置位,则控制该功能生效 */
} WPHY_TOOL_FuncBitControlReq;

/*
 * 结构说明: 状态查询回复CNF消息结构
 */
typedef struct {
    VOS_UINT32 addr;  /* 寄存器地址 */
    VOS_UINT32 value; /* 寄存器值 */
} UPHY_TOOL_BbpDbgRegRd;

/*
 * 结构说明: 状态查询回复CNF消息结构
 */
typedef struct {
    VOS_UINT32            regRdNum;                                  /* RegList个数 */
    UPHY_TOOL_BbpDbgRegRd regList[UPHY_TOOL_BBP_DBG_SAMPLE_REG_NUM]; /* REG_RD_STATUS_STRU */
} UPHY_TOOL_BbpDbgRegRdList;

/*
 * 结构说明: 状态查询回复CNF消息结构
 */
typedef struct {
    VOS_UINT32                omDrxSampleId; /* 默认值0 */
    VOS_UINT16                opid;          /* 默认值1 */
    VOS_UINT16                status;        /* 默认值0 */
    UPHY_TOOL_BbpDbgRegRdList drxRegData;    /* 一个 REG_CONFIG_LIST_STRU */
} UPHY_TOOL_BbpDbgRegRdReport;

/*
 * 结构说明: 工具寄存器配置消息上报结构
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT16                  msgId; /* _H2ASN_Skip */
    VOS_UINT16                  toolsId;
    UPHY_TOOL_BbpDbgRegRdReport sampleReg; /* UPHY_TOOL_BbpDbgRegRdReport  */
} UPHY_TOOL_BbpDbgRegRdReportInd;

/*
 * 结构说明: 工具寄存器配置消息结构
 */
typedef struct {
    VOS_UINT32 addr;  /* 寄存器地址 */
    VOS_UINT32 mask;  /* 掩码 */
    VOS_UINT32 value; /* 寄存器值 */
} UPHY_TOOL_BbpDbgRegWr;

/*
 * 结构说明: 新的采数地址，包括地址类型(dam, socp)和地址偏移
 */
typedef struct {
    VOS_UINT32 regAddr : 24;     /* 偏移地址 */
    VOS_UINT32 chalProcType : 4; /* 通道处理类型: 对应 UPHY_TOOL_SampleRegCompleteUint16  */
    VOS_UINT32 regType : 4;      /* 基地址类型: 对应 UPHY_TOOL_SAMPLE_BASE_ADDR_ENUM_UINT16 */
} UPHY_TOOL_NewBbpDbgRegAddr;

/*
 * 结构说明: 工具寄存器配置消息结构
 */
typedef struct {
    VOS_UINT32             regNum;  /* RegList个数 */
    UPHY_TOOL_BbpDbgRegWr *regList; /* 结构:REG_CONFIG_WR_STRU */
} UPHY_TOOL_BbpDbgRegWrReqList;

/*
 * 结构说明: 工具寄存器配置消息结构
 */
typedef struct {
    VOS_UINT32            regNum;                                    /* RegList个数 */
    UPHY_TOOL_BbpDbgRegWr regList[UPHY_TOOL_BBP_DBG_SAMPLE_REG_NUM]; /* 结构:REG_CONFIG_WR_STRU */
} UPHY_TOOL_BbpDbgRegWrList;

/*
 * 结构说明: 工具寄存器配置消息结构
 */
typedef struct {
    VOS_UINT32 omDrxSampleId;    /* 默认值0,取值分别为BUS/DATA */
    VOS_UINT16 opid;             /* 开始采数=4，停止采数=2，查询状态=1 */
    VOS_UINT16 powrDomainBitmap; /* 默认值0xFFFF */
    /* 1,2,3部门，采数寄存器，每组结构为REG_CONFIG_LIST_STRU */
    UPHY_TOOL_BbpDbgRegWrList drxSampleRegData[UPHY_TOOL_BBP_DBG_SAMPLE_REG_LIST_NUM];
    UPHY_TOOL_BbpDbgRegWrList drxReportRegData; /* 寄存器上报 */
    UPHY_TOOL_BbpDbgRegWrList drxStopRegData;   /* 停止寄存器 */
    UPHY_TOOL_BbpDbgRegWrList drxOtherRegData[UPHY_TOOL_BBP_DBG_OTHER_REG_LIST_NUM]; /* other ,暂时不用 */
} UPHY_TOOL_BbpDbgRegWrMsg;

/*
 * 结构说明: 工具寄存器配置消息结构
 */
typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
        VOS_UINT16           msgId; /* _H2ASN_Skip */
    VOS_UINT16               toolsId;
    UPHY_TOOL_BbpDbgRegWrMsg sampleReg;
} UPHY_TOOL_BbpDbgRegCfg;

/*
 * 结构说明: 记录modem_Id和主从模状态
 */
typedef struct {
    CSDR_ModeIdUint16        csdr1xModemId;     /* 1x所处的ModemId */
    CSDR_ModeIdUint16        csdrHrpdModemId;   /* HRPD所处的ModemId */
    CSDR_1X_ModeTypeUint16   csdr1xModemType;   /* 1x主从模状态 */
    CSDR_HRPD_ModeTypeUint16 csdrHrpdModemType; /* HRPD主从模状态 */
} UPHY_TOOL_CsdrModeState;

/*
 * 结构说明: 当前C模状态消息结构
 */
typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
        VOS_UINT16          msgId;          /* _H2ASN_Skip,原语类型 */
    VOS_UINT16              toolsId;        /* 保留字段 */
    UPHY_TOOL_CsdrModeState csdrModemState; /* C模的状态信息 */
} CPROC_UPHY_CsdrSetModeInfo;

/*
 * 结构说明: 工具寄存器配置消息上下文
 */
typedef struct {
    UPHY_TOOL_MntnBoolUint16 dataFlag; /* 采数寄存器的开关标志 */
    VOS_UINT16               rsv;
    /* 1,2,3部门，采数寄存器，每组结构为REG_CONFIG_LIST_STRU */
    UPHY_TOOL_BbpDbgRegWrList drxSampleRegData[UPHY_TOOL_BBP_DBG_SAMPLE_REG_LIST_NUM];
    UPHY_TOOL_BbpDbgRegWrList drxReportRegData;                                      /* 寄存器上报 */
    UPHY_TOOL_BbpDbgRegWrList drxStopRegData;                                        /* 停止寄存器 */
    UPHY_TOOL_BbpDbgRegWrList drxOtherRegData[UPHY_TOOL_BBP_DBG_OTHER_REG_LIST_NUM]; /* other ,暂时不用 */
} UPHY_TOOL_BbpDbgRegWrListInfo;

/*
 * 结构说明: 工具寄存器配置消息上报结构,仅用于可维可测，观测采数寄存器是否正确配置
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
        VOS_UINT16            msgId; /* _H2ASN_Skip */
    VOS_UINT16                toolsId;
    VOS_UINT32                omDrxSampleId;    /* 默认值0,取值分别为BUS/DATA */
    VOS_UINT16                opid;             /* 开始采数=4，停止采数=2，查询状态=1 */
    VOS_UINT16                powrDomainBitmap; /* 默认值0xFFFF */
    UPHY_TOOL_BbpDbgRegWrList drxSampleRegData; /* 1,2,3部门，采数寄存器，每组结构为REG_CONFIG_LIST_STRU */
} UPHY_TOOL_BbpDbgRegWrInd;

/*
 * 结构说明: 回读单个寄存器的配置
 */
typedef struct {
    VOS_UINT32 number;                                      /* 回读寄存器的个数 */
    VOS_UINT32 slaveAddr[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM]; /* 最多回读32个寄存器 */
    VOS_UINT16 chanNo[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM];    /* 0-3:表示CH0-CH3 */
    VOS_UINT16 regAddr[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM];   /* 地址 */
} UPHY_TOOL_MipiReadSingleReg;

/*
 * 结构说明: SSI回读的寄存器格式
 */
typedef struct {
    VOS_UINT32 number;                                   /* 回读寄存器的个数 */
    VOS_UINT16 chanNo[UPHY_TOOL_MAX_SSI_READ_BACK_NUM];  /* 地址 */
    VOS_UINT16 slaveId[UPHY_TOOL_MAX_SSI_READ_BACK_NUM]; /* Slave Id */
    VOS_UINT16 regAddr[UPHY_TOOL_MAX_SSI_READ_BACK_NUM]; /* 寄存器 */
} UPHY_TOOL_SsiReadSingleReg;

/*
 * 结构说明: SSI回读功能
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    rptSlot;       /* 回读时刻(默认时隙0) */
    VOS_UINT16                    cycleRptNum;   /* 指定每次周期分段回读的个数, 0-64,即周期回读不超过64个 */
    VOS_UINT32                    readType;      /* 0:指定寄存器和周期分段寄存器回读 1:全部(256个)寄存器回读 */
    VOS_UINT32                    chanNo;        /* 0-3:表示CH0-CH3。只支持1个通道的回读 */
    UPHY_TOOL_SsiReadSingleReg    readSingleReg; /* W回读周期10 ms */
} UPHY_TOOL_SsiReadFunction;

/*
 * 结构说明: MIPI回读功能
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 rptFlag;
    VOS_UINT16                    rptPeriod;
    VOS_UINT16                    rptSlot;       /* 回读时刻(默认时隙0) */
    VOS_UINT16                    reserved;
    VOS_UINT32                    readType;      /* 0:指定个数的寄存器回读,其他值预留 */
    UPHY_TOOL_MipiReadSingleReg   readSingleReg; /* W回读周期2ms,G回读周期2.3ms */
} UPHY_TOOL_MipiReadFunction;

/*
 * 结构说明: 请求回读SSI/MIPI寄存器
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16 msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;     /* 保留字段 */
    UPHY_TOOL_SsiReadFunction ssiFunction; /* SSI回读功能 */
} UPHY_TOOL_SsiReadRptReq;

/*
 * 结构说明: 请求回读SSI/MIPI寄存器
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16  msgId;        /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                 toolsId;      /* 保留字段 */
    UPHY_TOOL_MipiReadFunction mipiFunction; /* MIPI回读功能 */
} UPHY_TOOL_MipiReadRptReq;

/*
 * 结构说明: 上报回读SSI寄存器
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;                                       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                                     /* 保留字段 */
    VOS_UINT32                errorInd;                                    /* 回读超时指示 */
    VOS_UINT32                ssiNumber;                                   /* 回读寄存器的个数 */
    VOS_UINT32                ssiRegValue[UPHY_TOOL_MAX_SSI_READ_ALL_NUM]; /* SSI寄存器回读值 */
} UPHY_TOOL_SsiReadRptInd;

/*
 * 结构说明: 上报回读MIPI寄存器
 */
typedef struct {
    UPHY_TOOL_MntnMsgIdUint16 msgId;                                          /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;                                        /* 保留字段 */
    VOS_UINT32                errorInd;                                       /* 回读超时指示 */
    VOS_UINT32                mipiNumber;                                     /* 回读寄存器的个数 */
    VOS_UINT32                mipiRegValue[UPHY_TOOL_MAX_MIPI_READ_BACK_NUM]; /* MIPI寄存器回读值 */
} UPHY_TOOL_MipiReadRptInd;

/*
 * 结构说明: SSI回读上报控制
 */
typedef struct {
    VOS_UINT16                cycleReadIndex;  /* 周期读取索引 */
    VOS_UINT16                reserved;        /* 保留 */
    UPHY_TOOL_SsiReadFunction ssiReadFunction; /* 回读功能结构 */
} UPHY_TOOL_SsiReadCtrl;

/*
 * 结构说明: SSI/MIPI回读功能
 */
typedef struct {
    UPHY_TOOL_SsiReadCtrl      ssiReadCtrl;   /* SSI回读功能 */
    UPHY_TOOL_MipiReadFunction mipiFunction;  /* MIPI回读功能 */
    UPHY_TOOL_SsiReadRptInd    ssiReportInd;  /* SSI上报 */
    UPHY_TOOL_MipiReadRptInd   mipiReportInd; /* MIPI上报 */
} WPHY_TOOL_SsiMipiReadFunction;

/*
 * 结构说明: PHY 内部消息上报控制请求
 */
typedef struct {
    VOS_UINT32 addr;  /* 内存地址 */
    VOS_UINT32 value; /* 内存地址的值 */
} WPHY_TOOL_MemInfo;

/*
 * 结构说明: TOOL给物理层下发的读取内存请求
 */
typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
        WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16               toolsId;    /* 保留字段 */
    WPHY_TOOL_MntnBoolUint16 enableFlag; /* 使能标记，0表示去使能，1表示使能 */
    VOS_UINT16               readPeriod; /* 回读周期，单位为10ms */
    VOS_UINT16               addrNum;    /* 地址数目 */
    VOS_UINT16               reserved;
    VOS_UINT32               addrList[WPHY_MNTN_READ_ADDR_MAX_NUM]; /* 内存地址列表 */
} WPHY_TOOL_ReadMemReq;

/*
 * 结构说明: PHY 内部消息上报内存信息
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                addrNum; /* 地址数目 */
    VOS_UINT16                reserved;
    WPHY_TOOL_MemInfo         addrList[WPHY_MNTN_READ_ADDR_MAX_NUM]; /* 内存地址列表 */
} WPHY_TOOL_ReadMemInd;

/*
 * 结构说明: PHY 内部消息上报信息数据结构
 */
typedef struct {
    VOS_UINT32 addr;  /* 内存地址 */
    VOS_UINT32 value; /* 内存地址的值 */
} GPHY_TOOL_MemInfo;

/*
 * 结构说明: TOOL给物理层下发的读取内存请求
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    VOS_UINT16                command;    /* 命令。0:停止，1：启动 */
    VOS_UINT16                readPeriod; /* 回读周期，单位为10ms */
    VOS_UINT16                addrNum;    /* 地址数目 */
    VOS_UINT16                reserved;
    VOS_UINT32                addrList[GPHY_MNTN_READ_ADDR_MAX_NUM]; /* 内存地址列表 */
} GPHY_TOOL_ReadMemReq;

/*
 * 结构说明: PHY 内部消息上报内存信息
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                addrNum; /* 地址数目 */
    VOS_UINT16                reserved;
    GPHY_TOOL_MemInfo         addrList[GPHY_MNTN_READ_ADDR_MAX_NUM]; /* 内存地址列表 */
} GPHY_TOOL_ReadMemInd;

/*
 * 结构说明: WPHY各个系统状态可维可测上报控制
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 freeMntnRptStatus;  /* FREE态维测上报去使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 idleMntnRptStatus;  /* IDLE态维测上报去使能标记，包括PI打印等 */
    UPHY_TOOL_MntnRptStatusUint16 fachMntnRptStatus;  /* FACH态维测上报去使能标记，包括随机接入消息等 */
    UPHY_TOOL_MntnRptStatusUint16 dchMntnRptStatus;   /* DCH态维测上报去使能标记，包括随机接入消息等 */
    UPHY_TOOL_MntnRptStatusUint16 slaveMntnRptStatus; /* SLAVE维测上报去使能标记 */
    VOS_UINT16                    rserved;
} WPHY_TOOL_SysStateMntnRptCtrl;

/*
 * 结构说明: WPHY各个模块上报可维可测控制
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 csRptStatus;          /* 小区搜索维测上报使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 drxSchdlRptStatus;    /* FACH态维测上报使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 olpcRptStatus;        /* DCH态维测上报使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 cmRptStatus;          /* SLAVE维测上报使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 softDemSyncRptStatus; /* 解调同失步软化维测上报使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 abnormalRptStatus;    /* WPHY time out维测上报使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 ant2VoteStatus;       /* 分集双天线投票上报使能标记 */
    VOS_UINT16                    reserved;
} WPHY_TOOL_ModuleMntnRptCtrl;

/*
 * 结构说明: TOOL给物理层下发的读取内存请求
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    WPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */                                /* 原语类型 */
    VOS_UINT16                    toolsId;           /* 保留字段 */
    UPHY_TOOL_MntnRptStatusUint16 intraMsgRptStatus; /* 内部消息上报标记，1表示上报，0表示不上报 */
    VOS_UINT16                    reserved;
    /* 各个状态主动上报维测打印控制，内部变量0为不上报，1为主动上报，默认上报 */
    WPHY_TOOL_SysStateMntnRptCtrl sysStateRptStatusCtrl;
    /* 各个模块上报维测打印控制，内部变量1为主动上报，0为不上报，默认不上报 */
    WPHY_TOOL_ModuleMntnRptCtrl moduleRptStatusCtrl;
} WPHY_TOOL_MntnFuncSwtichReq;

/*
 * 结构说明: PHY DCXO信息上报
 */
typedef struct {
    VOS_MSG_HEADER
    UPHY_TOOL_MntnMsgIdUint16     msgId;           /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                    toolsId;         /* 保留字段 */
    UPHY_TOOL_MntnDcxoEventUint16 dcxoEventType;   /* 状态 */
    VOS_INT16                     xoTemp;          /* Xo温度,0.1度 */
    VOS_UINT16                    rcmPpmValidFlag; /* RCM曲线是否有效 */
    VOS_UINT16                    reserved;        /* 保留 */
    VOS_UINT32                    transValue;      /* 打印值 */
    VOS_INT32                     rcmPpm;          /* RCM写给PHY的值 */
    VOS_INT32                     wcdmaPpmMntn;    /* W ppm维护值 */
    VOS_INT32                     gsmPpmMntn;      /* G ppm维护值 */
    VOS_UINT32                    modem0RptValue;  /* PHY上报给RCM的值 */
    VOS_UINT32                    modem1RptValue;  /* PHY上报给RCM的值 */
    VOS_UINT32                    modem2RptValue;  /* PHY上报给RCM的值 */
} UPHY_TOOL_MntnDcxoStatus;

/*
 * 结构说明: 请求上报GSDR 运行状态信息
 */
typedef struct {
    VOS_MSG_HEADER      /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* _H2ASN_Skip */   /* 原语ID */
    VOS_UINT16 toolId;  /* 保留字段 */
    VOS_UINT16 command; /* 命令。0:停止，1：启动 */
    VOS_UINT16 reserved;
} GPHY_TOOL_GsdrStateReq;

/*
 * 结构说明: 上报GSDR 运行状态信息
 */
typedef struct {
    VOS_UINT16 msgId;     /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;    /* 保留字段 */
    VOS_UINT32 curFn;     /* 当前的帧号 */
    VOS_UINT16 gsdrState; /* GSDR当前运行的状态 */
    VOS_UINT16 reserved;
} GPHY_TOOL_GsdrStateInd;

/*
 * 结构说明: GPHY 强制启动GSDR
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                     toolsId;          /* 保留字段 */
    UPHY_TOOL_MntnCtrlStatusUint16 forceCtrlGsdrReq; /* 是否强制启动和关闭Gsdr，0:默认不打开；1:强制打开；2:强制关闭 */
    VOS_UINT16                     reserved;
} GPHY_TOOL_GsdrForcrCtrlReq;

/*
 * 结构说明: 上报gsdr不同类型维测信息
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId; /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;

    /*
     * 使用控制字ulSwitchCtrl，使能GSDR某些维测模块
     * FUNC_GSDR_NB_PARA_ENABLE_FLAG       GSDR_BIT0    --使能打印NB参数下发/上报功能
     * FUNC_GSDR_NB_DUMP_ENABLE_FLAG       GSDR_BIT1    --使能打印NB DUMP采数功能
     * FUNC_GSDR_SB_PARA_ENABLE_FLAG       GSDR_BIT2    --使能打印SB参数下发/上报功能
     * FUNC_GSDR_SB_DUMP_ENABLE_FLAG       GSDR_BIT3    --使能打印SB DUMP采数功能
     */
    VOS_UINT32 switchCtrl;
} UPHY_TOOL_GsdrMntnReq;

/*
 * 结构说明: ERACH下可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    WPHY_TOOL_MntnErachCtrlStateUint16 erachCtrlState; /* 控制状态指示 */
    /* 原语接收状态，不包含MACI-CFG原语，MACI-CFG原语状态直接到原语分发模块获取 */
    WPHY_TOOL_MntnErachPrimitiveTypeUint16 primitiveType;
    UPHY_TOOL_MntnBoolUint16            macINewCfgFlag;     /* MACI有新配置，true有效 */
    UPHY_TOOL_MntnBoolUint16            rgchExistFlag;      /* 是否存在RGCH信道 */
    UPHY_TOOL_MntnBoolUint16            implicitReleaseInd; /* 隐式释放标志，true有效 */
    UPHY_TOOL_MntnBoolUint16            explicitReleaseInd; /* 显式释放标志，true有效 */
    UPHY_TOOL_MntnBoolUint16            sgConfigInd;        /* 0:指示stSGConfig无效;1:指示stSGConfig有效 */
    WPHY_TOOL_MntnUlEfachLochTypeUint16 eFachLochType;      /* [0x1,0x2], 0x1:CCCH; 0x2:DCCH/DTCH */

    /* 用计数器实现Additional E-DCH transmission back off + Tdelay - 1定时器，启动后每TTI增加一次 */
    VOS_UINT16 tAddBackOffCount;
    /* 用计数器实现Maximum E-DCH resource allocation for CCCH定时器，启动后每TTI增加一次 */
    VOS_UINT16 tMaxAllocCcchCount;
    /* 用计数器实现Maximum period for collision resolution phase定时器，启动后每TTI增加一次 */
    VOS_UINT16 tMaxPrdCollisionCount;
    VOS_UINT16 tbCount;               /* 用计数器实现Tb定时器，启动后每TTI增加一次 */

    VOS_UINT16                       signatureIdx;
    WPHY_TOOL_MntnRachRsltUint16     rachRslt;       /* 随机接入结果枚举 */
    VOS_UINT16                       prachReTranNum; /* 重传次数 */
    VOS_UINT16                       aiSigNum;       /* AI签名序列 */
    VOS_UINT16                       eaiSigNum;      /* EAI签名序列 */
    WPHY_TOOL_MntnErachEaiTypeUint16 eaiHardValue;   /* EAI硬值 */
    VOS_INT16                        currSlotPower;  /* 当前最后一个preamble的发射功率 */
    VOS_INT16                        initPower;      /* 初始接入功率 */
    VOS_UINT16                       aiSlotNum;      /* AI接入时隙号 */

    VOS_INT16                sysChangeInd;        /* 小区重选/硬切换指示标志 */
    UPHY_TOOL_MntnBoolUint16 timeUpdateDelayFlag; /* ERACH下定时延迟更新标记 */
    VOS_INT16                fdpchChipOffset;     /* 只用于dpch，sccpch，pich */

    VOS_UINT32 bbpDmodeSyncInd; /* 当前帧解调同失步状态 */
    VOS_UINT16 dmodeSyncInd;    /* 当前帧解调同失步状态 */
    VOS_UINT16 frameSyncInd;    /* 当前帧同失步状态 */

    WPHY_TOOL_TxPowInfo txPow; /* 上行发射功率 */

    VOS_UINT32 erachChEn; /* Erach相关信道使能状态 */
} WPHY_TOOL_ErachMntnRpt;

/*
 * 结构说明: WTC模式可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    VOS_UINT16               wtcMode; /* WTC模式标识 */
    VOS_UINT16               modemId;
    WPHY_TOOL_MntnBoolUint16 bbpClkCloseFlag;
    WPHY_TOOL_MntnBoolUint16 masterModeFlag;
    WPHY_TOOL_MntnBoolUint16 tdWorkFlag;
    WPHY_TOOL_MntnBoolUint16 cdmaWorkFlag;
} WPHY_TOOL_WtcModeRpt;

/*
 * 结构说明: 上报gsdr判断是否开启/关闭的统计数据
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId;     /* 保留字段 */
    GPHY_TOOL_MntnGsdrJudgeTypeUint16 judgeType;   /* 判断启动和关闭Gsdr 1:判断是否打开 2:判断是否关闭 */
    VOS_UINT16                        rssiSum;     /* 满足RSSI门限的SUB个数  */
    VOS_UINT16                        rssiTotal;   /* 统计RSSI的SUB帧个数  */
    VOS_UINT16                        ciSum;       /* 满足CI门限的SUB个数  */
    VOS_UINT16                        ciTotal;     /* 统计CI的SUB帧个数 */
    VOS_UINT16                        subQualSum;  /* 满足QUAL门限的SUB个数  */
    VOS_UINT16                        fullQualSum; /* 满足QUAL门限的FULL个数  */
    VOS_UINT16                        rssiReason;  /* 统计CI的SUB帧个数 */
    VOS_UINT16                        ciReason;    /* 满足QUAL门限的SUB个数  */
    VOS_UINT16                        qualReason;  /* 满足QUAL门限的FULL个数  */
} GPHY_TOOL_GsdrJudgeInfoInd;

/*
 * 结构说明: 上报BCH态GSDR判断是否开启/关闭的统计数据
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */               /* 原语类型 */
    VOS_UINT16 toolsId;             /* 保留字段 */
    VOS_UINT16 curDecodeType;       /* 当前译码类型 */
    VOS_UINT16 expectDecodeType;    /* 期望译码类型 */
    VOS_INT16  mainAntRssiValue;    /* NB接收RSSI值  */
    VOS_INT16  divAntRssiValue;     /* NB双天线接收较小RSSI值  */
    VOS_UINT16 pber;                /* NB译码PBER值 */
    VOS_UINT16 awgnFlag;            /* NB译码场景检测AWGN */
    VOS_UINT16 saicToDaicRuleFlag;  /* 满足QUAL门限的SUB个数  */
    VOS_UINT16 isaicToDaicRuleFlag; /* 满足QUAL门限的FULL个数  */
    VOS_UINT16 isaicOffRuleFlag;    /* 统计CI的SUB帧个数 */
    VOS_UINT16 daicOffRuleFlag;     /* 满足QUAL门限的SUB个数  */
    VOS_INT16  bchDaicOnRssiThreshold;   /* BCH态下判决启动DACI的RSSI门限 */
    VOS_UINT16 bchDaicOnPberThreshold;   /* BCH态下判决启动DACI的PBER门限 */
    VOS_INT16  bchDaicOffRssiThreshold;  /* BCH态下判决关闭DACI的RSSI门限 */
    VOS_INT16  bchIsaicOffRssiThreshold; /* BCH态下判决关闭ISAIC的RSSI门限 */
} GPHY_TOOL_GsdrJudgeBchInfoInd;

/*
 * 结构说明: 用于MRX PD上报
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;             /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;           /* 保留字段 */
    VOS_INT16                 mrxGetPower;       /* 获取的功率0.1dB */
    VOS_UINT16                mrxPowerLinear;    /* 获取的功率,线性值 */
    VOS_INT16                 mrxGainValue;      /* mrx 默认增益 */
    VOS_INT16                 gmskMrxGainOffset; /* GMSK mrx 增益偏差 */
    VOS_INT16                 edgeMrxGainOffset; /* 8PSK mrx 增益偏差 */
    VOS_UINT16                phyState;          /* 物理层状态 */
} GPHY_MRX_PdReport;

/*
 * 结构说明: SB采数结构体
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    GPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16 toolsId; /* 保留字段 */
    VOS_UINT32 sbDumpDataEven[GPHY_TOOL_GSDR_SB_BURST_DATA_LEN];
    VOS_UINT32 sbDumpDataOdd[GPHY_TOOL_GSDR_SB_BURST_DATA_LEN];
} GPHY_TOOL_GsdrDumpSbDataInd;

/*
 * 结构说明: EDRX模块原语建立或释放EDRX
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    WPHY_TOOL_MntnT321CtrlStatusUint16 t321Status;   /* T321状态 */
    WPHY_TOOL_MntnT321CtrlCauseUint16  t321CtrlCuse; /* T321状态变化的原因 */
} WPHY_TOOL_EdrxMntnT321CtrlReq;

/*
 * 结构说明: EDRX模块原语建立或释放EDRX
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

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
    VOS_UINT16 reserve; /* 保留字段 */
} WPHY_TOOL_EdrxMntnRpt;

/*
 * 结构说明: G负载过大异常打印
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16       msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                      toolsId;     /* 保留字段 */
    GPHY_TOOL_MntnTimeOutTypeUint16 timeOutCase; /* 负载异常场景 */
    VOS_UINT16                      reserved;    /* 保留字段 */
} GPHY_TOOL_TimeOutRptInd;

/*
 * 结构说明: 异系统优化打印
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    VOS_UINT16 timerState; /* 定时器状态 */
    VOS_UINT16 reserved1;
    VOS_UINT32 countFnNum; /* 定时器需要多少帧 */
    VOS_UINT32 timeOutNum; /* 定时器超时次数 */
    VOS_UINT32 startFn;    /* 定时器启动帧号 */
    VOS_UINT32 passFnCnt;  /* 定时器从启动帧号开始已经经过了多少帧 */

    /* BIT0: LTE; BIT1: WCDMA; BIT2: TDS; 对应BIT位为1表示未完成当前一轮测量，为0表示不存在该模式或未完成 */
    VOS_UINT16 interratMeasFlg;
    /* BIT0: LTE; BIT1: WCDMA; BIT2: TDS; 对应BIT位为1表示向RCM申请了资源，为0表示未申请资源或未释放 */
    VOS_UINT16 requestedRfFlg;
    VOS_UINT16 allMeasState;     /* 所有异系统测量都完成了一轮。 PHY_TURE: 完成一轮; PHY_FALSE: 存在异系统测量未完成 */
    VOS_UINT16 measTimeOutCnt;   /* 完成一轮所有异系统测量所需要的4.5s周期个数 */
    VOS_UINT16 forcedRankEnable; /* 是否需要强制抬升异系统任务优先级。PHY_TRUE:需要; PHY_FALSE: 不需要 */
    VOS_UINT16 forcedRankGaps;   /* 连续强制抬升优先级次数 */
    VOS_UINT16 newGapsStart;     /* 给某一个异系统的分沟是否完成。PHY_TRUE: 分沟完成; PHY_FALSE: 需要继续分沟 */
    VOS_UINT16 lastInterratMeas; /* 上一次分沟的异系统, 1: LTE; 2:WCDMA; 4:TDS */
    VOS_UINT16 restartGapTime;   /* 重启一轮测量的间隔。单位: 帧。保护资源的申请和释放 */
    VOS_UINT16 reserved2;

    VOS_UINT32 phyCtuState; /* G写入CTU的主模状态 */
    VOS_UINT32 phyTaskFlag; /* 存在的task任务 */

    VOS_UINT16 bgLSlaveTaskState; /* L背景搜任务执行状态 */
    VOS_UINT16 bgWSlaveTaskState; /* W背景搜任务执行状态 */
    VOS_UINT16 lMeasRunState;     /* L测量状态 */
    VOS_UINT16 wMeasRunState;     /* W测量状态 */
} GPHY_TOOL_InterratOptimizationInt;

/*
 * 结构说明: W超时上报打印
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16       msgId;       /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                      toolsId;     /* 保留字段 */
    WPHY_TOOL_MntnTimeOutTypeUint16 timeOutCase; /* 负载异常场景 */
    VOS_UINT16                      reserved;    /* 保留字段 */
} WPHY_TOOL_TimeOutRptInd;

/*
 * 结构说明: D态大频偏场景下AFC Reset维测上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16         msgId;             /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                        toolsId;           /* 保留字段 */
    VOS_UINT16                        freqM;             /* dch态AFC RESET需要满足的大于门限的次数 */
    VOS_UINT16                        freqThresh;        /* dch态AFC RESET需要满足的频偏门限，单位KHz */
    VOS_INT32                         ppmThresh;         /* 由频偏门限转换成的ppm门限 */
    VOS_INT16                         temperatureTh;     /* dch态AFC RESET需要满足的温度门限，单位0.1摄氏度 */
    VOS_UINT16                        afcAboveThreshCnt; /* 大于门限的计数，达到uhwFreqM时可重置AFC */
    VOS_UINT16                        afcFrameCnt;       /* 帧数计数 */
    VOS_INT16                         oldRxFreq;         /* 当前频点不是原频点时，说明发生了切换 */
    VOS_INT32                         basePpm;           /* 需要AFC重置时，将AFC置为保存的base值 */
    VOS_INT32                         currentPpm;        /* 当前频率下Ppm的值 */
    WPHY_MNTN_DchAfcResetStatusUint16 afcResetStatus;    /* AFC RESET方案运行状态 */
    VOS_UINT16                        reserved;          /* 保留位 */
} WPHY_TOOL_DchAfcResetRpt;

/*
 * 结构说明: RCM DCXO可维可测上报控制
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 dcxoRptStatus;      /* DCXO维测上报去使能标记 */
    UPHY_TOOL_MntnRptStatusUint16 phySampleRptStatus; /* PHY样本点维测上报去使能标记 */
} RCM_TOOL_DcxoMntnRptCtrl;

/*
 * 结构说明: RCM SCHDL可维可测上报控制
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 schdlRptStatus; /* SCHDL维测上报去使能标记 */
    VOS_UINT16                    reserved;       /* 保留字段 */
} RCM_TOOL_SchdlMntnRptCtrl;

/*
 * 结构说明: RCM TAS可维可测上报控制
 */
typedef struct {
    UPHY_TOOL_MntnRptStatusUint16 tasRptStatus; /* TAS维测上报去使能标记 */
    VOS_UINT16                    reserved;     /* 保留字段 */
} RCM_TOOL_TasMntnRptCtrl;

/*
 * 结构说明: RCM 可维可测上报控制
 */
typedef struct {
    RCM_TOOL_DcxoMntnRptCtrl  dcxoMntnRptCtrl;  /* DCXO主动上报维测打印控制，内部变量0为不上报，1为主动上报，默认上报 */
    /* SCHDL主动上报维测打印控制，内部变量0为不上报，1为主动上报，默认上报 */
    RCM_TOOL_SchdlMntnRptCtrl schdlMntnRptCtrl;
    RCM_TOOL_TasMntnRptCtrl   tasMntnRptCtrl;   /* TAS主动上报维测打印控制，内部变量0为不上报，1为主动上报，默认上报 */
} RCM_TOOL_MntnRptCtrl;

/*
 * 结构说明: TOOL给RCM下发的控制消息，控制RCM DCXO维测消息上报的
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    UPHY_TOOL_MntnMsgIdUint16 msgId;
    /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16           toolsId; /* 保留字段 */
    RCM_TOOL_MntnRptCtrl rcmMntnRptCtrl;
} RCM_TOOL_MntnFuncSwtichReq;

/*
 * 结构说明: RCM TAS裁决结果上报结构
 */
typedef struct UPHY_MNTN_RCM_TAS_ARBITRATION_RSLT {
    RCM_TOOL_MntnModemStateUint16 modemState[RCM_TOOL_MNTN_MODEM_SUPPORT_NUM]; /* M0~M2的Modem状态 */
    RCM_TOOL_MntnActionUint16     tasAction;                                   /* TAS切换行动 */
} RCM_TOOL_MntnTasArbitrationRslt;

/* EASYRF 采数消息结构 */
/*
 * 结构说明: 工具寄存器配置消息结构
 */
typedef struct {
    VOS_UINT32            regNum;                                       /* RegList个数 */
    UPHY_TOOL_BbpDbgRegWr regList[UPHY_TOOL_EASYRF_DBG_SAMPLE_REG_NUM]; /* 结构:REG_CONFIG_WR_STRU */
} UPHY_TOOL_EasyrfDbgRegWrList;

/*
 * 结构说明: 状态查询回复CNF消息结构
 */
typedef struct {
    VOS_UINT32            regRdNum;                                     /* RegList个数 */
    UPHY_TOOL_BbpDbgRegRd regList[UPHY_TOOL_EASYRF_DBG_SAMPLE_REG_NUM]; /* REG_RD_STATUS_STRU */
} UPHY_TOOL_EasyrfDbgRegRdList;

/*
 * 结构说明: 状态查询回复CNF消息结构
 */
typedef struct {
    VOS_UINT32                   omDrxSampleId; /* 默认值0 */
    VOS_UINT16                   opid;          /* 默认值1 */
    VOS_UINT16                   status;        /* 默认值0 */
    UPHY_TOOL_EasyrfDbgRegRdList drxRegData;    /* 一个 REG_CONFIG_LIST_STRU */
} UPHY_TOOL_EasyrfDbgRegRdReport;

/*
 * 结构说明: 工具寄存器配置消息上报结构
 */
typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    VOS_UINT16                 msgId; /* _H2ASN_Skip */
    VOS_UINT16                     toolsId;
    UPHY_TOOL_EasyrfDbgRegRdReport sampleReg; /* UPHY_TOOL_EasyrfDbgRegRdReport  */
} UPHY_TOOL_EasyrfDbgRegRdReportInd;

/*
 * 结构说明: TOOL-RFM采数请求原语消息
 */
typedef struct {
    VOS_UINT32               sampleId;    /*  默认值0,取值分别为BUS/DATA */
    BBA_DBG_ActionUint16     debugAction; /*  采数动作:0:启动;1:查询;2、停止 */
    EASYRF_DBG_CfgTypeUint16 debugType;   /*  采数类型 */
    /*
     * 模块内多种配置表, 表维度UPHY_TOOL_EASYRF_DBG_REG_LIST_NUM，每个表的数据格式如下
     * |-regNum(VOS_UINT32)-|-regNum个配置数据(UPHY_TOOL_BbpDbgRegWr)-|
     */
    VOS_UINT32 drxSampleRegData[1];
} UPHY_TOOL_EasyrfDbgRegWrMsg;

/*
 * 结构说明: TOOL-RFM采数请求原语消息
 */
typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT16              msgId; /* _H2ASN_Skip */
    VOS_UINT16                  toolsId;
    UPHY_TOOL_EasyrfDbgRegWrMsg sampleReg;
} UPHY_TOOL_EasyrfDbgReq;

/*
 * 结构说明: TOOL-RFM采数请求回复原语消息
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    EASYRF_TOOL_MntnMsgIdUint16 msgID;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16               toolsId; /* 保留字段 */
    UPHY_TOOL_MntnRsltUint16 result;  /* 接收原语的处理结果ID */
    VOS_UINT16               rsv;     /* 保留字段 */
} UPHY_TOOL_EasyrfDbgCnf;

/*
 * 结构说明: PS信道质量上报请求
 */
typedef struct {
    VOS_MSG_HEADER
    GPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    UPHY_TOOL_MntnBoolUint16  enableFlag; /* 使能开关 */
    VOS_UINT16                reserved;   /* 保留字段 */
} TOOL_GPHY_PsChanqualReportReq;

/*
 * 结构说明: PS编码信息上报
 */
typedef struct {
    VOS_MSG_HEADER
    GPHY_TOOL_MntnMsgIdUint16 msgId;      /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId;    /* 保留字段 */
    UPHY_TOOL_MntnBoolUint16  enableFlag; /* 使能开关 */
    VOS_UINT16                reserved;   /* 保留字段 */
} TOOL_GPHY_PsUlRptReq;

/*
 * 结构说明: 打印维测点和级别
 */
typedef struct {
    VOS_UINT16 logPoint; /* *< Log point identifier */
    VOS_UINT16 logLevel; /* *< Log level setting */
} EASYRF_OM_LogPoint;

/*
 * 结构说明: 打印级别配置参数
 */
typedef struct {
    VOS_UINT8          numLogPoints; /* *< number of log points, 1..RFHAL_OM_MAX_LOG_POINTS */
    VOS_UINT8          reserved[3];
    EASYRF_OM_LogPoint logPoint[EASYRF_OM_MAX_LOG_POINTS];
    /* *< log point setting */
} EASYRF_LOG_LevelPara;

/*
 * 结构说明: 寄存器回读配置参数
 */
typedef struct {
    EASYRF_OM_SwitchUint16  switchOn; /* 0 去使能寄存器回读，1 使能寄存器回读 */
    EASYRF_OM_ReadTrxUint16 isTrx;    /* 0 回读TX, 1 回读RX, 2 tx/rx都回读 */
    VOS_UINT8               regNum;   /* 回读寄存器个数, 1->8 */
    VOS_UINT8               reserved[3];
    VOS_UINT32              regAddr[EASYRF_OM_MAX_REG_READ_NUM]; /* 回读的rfdsp寄存器地址 */
} EasyrfRegReadPara;

/*
 * 结构说明: PD OM配置参数
 */
typedef struct {
    EASYRF_OM_SwitchUint16   omSwitchFlag; /* 0 :PD OM原语开关关闭，1:PD OM原语开关打开 */
    EASYRF_PD_OmSwitchUint16 omEnFlag;     /* 0 PD OM关闭, 1 PD OM打开 */
    EASYRF_PD_ModeUint16     mode;         /* PD MODE控制打桩 */
    VOS_UINT16               rsvd;
} EasyrfPdOmReq;

/*
 * 结构说明: UL TIMING更新配置参数
 */
typedef struct {
    EASYRF_OM_SwitchUint16  omSwitchFlag;   /* 0 :TIMING原语开关关闭，1:TIMING原语开关打开 */
    EASYRF_OM_RatmodeUint16 ratMode;        /* RAT工作模式 */
    VOS_UINT16              bandExtendFlag; /* band扩展标志:0:不扩展，1:扩展 */
    VOS_UINT16              band;           /* band */
    VOS_UINT16              timingNvId;     /* NVID */
    VOS_UINT16              rsvd[3];        /* 为了64 bit对齐 */
} EasyrfUlTimingUpdateReq;

/*
 * 结构说明: 上行功率受限消息上报
 */
typedef struct {
    VOS_UINT16 msgId;              /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;             /* tools id */
    VOS_UINT16 txPowerLimitStatus; /* 上行功率受限状态 */
    VOS_UINT16 reserved;           /* 保留字段 */
} GPHY_TOOL_TxPowerStatusInd;

/*
 * 结构说明: 最强邻区解bsic失败消息上报
 */
typedef struct {
    VOS_UINT16 msgId;          /* _H2ASN_Skip */ /* 原语ID */
    VOS_UINT16 toolId;         /* tools id */
    VOS_UINT16 arfcn;          /* 频点 */
    VOS_UINT16 rxlev;          /* 能量 */
} GPHY_TOOL_StrongestNcellInd;

/*
 * 结构说明: TOOL-BBA打印级别指示原语消息
 */
typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16           toolsId; /* 保留字段 */
    EASYRF_LOG_LevelPara para;
} UPHY_TOOL_EasyrfLogLevelCfgReq;

/*
 * 结构说明: TOOL-BBA寄存器回读原语消息
 */
typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16        toolsId; /* 保留字段 */
    EasyrfRegReadPara regReadPara;
} UphyToolEasyrfRegReadReq;

/*
 * Name: RfhalOmAfeAlwaysOnParaStru
 * Description: request of afe&pll always open through HIDS.
 */
typedef struct {
    EASYRF_OM_SwitchUint16 switchOn;        /* 0:功能关闭，1:功能打开 */
    EASYRF_OM_SwitchUint16 isRxPllAlwaysOn; /* 0: pll不常开, 1: pll常开 */
    EASYRF_OM_SwitchUint16 isRxAfeAlwaysOn; /* 0: afe不常开, 1: afe常开 */
    EASYRF_OM_SwitchUint16 isTxPllAlwaysOn; /* 0: pll不常开, 1: pll常开 */
    EASYRF_OM_SwitchUint16 isTxAfeAlwaysOn; /* 0: afe不常开, 1: afe常开 */
    VOS_UINT16             reserved;
} EasyrfAfeAlwaysOnPara;

/*
 * 结构说明: TOOL->BBA RX AFE常开原语消息
 */
typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16            toolsId; /* 保留字段 */
    EasyrfAfeAlwaysOnPara afeAlwaysOnPara;
} UphyToolEasyrfAfeOnReq;

/*
 * 结构说明: 高频采数控制信息
 */
typedef struct {
    VOS_UINT8  iqDumpContinue;  /* 连续采数标志,1表示每次AGC原语自动采数,不需要再次置位,0表示只采一次 */
    VOS_UINT8  iqDumpChanSel;   /* DUMP采数通道选择 */
    VOS_UINT8  iqDumpNodeSel;   /* DUMP采数节点选择 */
    VOS_UINT8  iqDumpChanType;  /* DUMP采数时业务类型选择,主要服务于上行,用于区分PRACH/PUCCH/PUSCH不同信道 */
} EASYRF_OM_TrxDumpPara;

/*
 * 结构说明: 高频采数控制信息
 */
typedef struct {
    VOS_UINT16              txDumpEn;     /* Tx DUMP采数使能控制位 */
    VOS_UINT16              rxDumpEn;     /* Rx DUMP采数使能控制位 */
    EASYRF_OM_TrxDumpPara   iqRxDumpPara; /* RX DUMP采数控制参数 */
    EASYRF_OM_TrxDumpPara   iqTxDumpPara; /* TX DUMP采数控制参数 */
} EASYRF_OM_TrxDumpCtrlPara;

/*
 * 结构说明: 高频AFE/Pll常开控制参数
 */
typedef struct {
    VOS_UINT8 isAfeAlwaysOn; /* 6370AFE常开控制 */
    VOS_UINT8 isPllAlwaysOn; /* 6370PllBuffEn常开控制 */
    VOS_UINT8 rsv1;          /* rsv1 */
    VOS_UINT8 rsv2;          /* rsv2 */
} EASYRF_OM_TrxAnalogPara;

/*
 * 结构说明: 高频AFE/Pll常开控制
 */
typedef struct {
    VOS_UINT16               txOnEn;   /* TX AFE/PLL常开控制使能 */
    VOS_UINT16               rxOnEn;   /* RX AFE/PLL常开控制使能 */
    EASYRF_OM_TrxAnalogPara  rxOnPara; /* RX AFE/PLL常开控制参数 */
    EASYRF_OM_TrxAnalogPara  txOnPara; /* TX AFE/PLL常开控制参数 */
} EASYRF_OM_TrxAnaCtrlPara;

/*
 * 结构说明: 各模块Cycle统计结构
 */
typedef struct {
    EASYRF_OM_SwitchUint16    switchOn;
    VOS_UINT16                rsv;
    EASYRF_OM_TrxAnaCtrlPara  trxAnaCtrl;  /* 6370 模拟常开控制 */
    EASYRF_OM_TrxDumpCtrlPara dfeDumpCtrl; /* 6370 DFE采数控制 */
} EASYRF_OM_TrxStubCtrlPara;

/*
 * 结构说明: TOOL->BBA RX AFE常开原语消息
 */
typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16            toolsId; /* 保留字段 */
    EASYRF_OM_TrxStubCtrlPara hfTrxCtrl;
} UphyToolEasyrfHfTrxCtrlReq;

/*
 * Name: EasyrfTxApcParaStubStru
 * Description: tx apc code&pa mode stub through HIDS.
 */
typedef struct {
    EASYRF_OM_SwitchUint16 switchOn; /* 0:功能关闭，1:功能打开 */
    EASYRF_OM_SwitchUint16 stubMode; /* 0:功率打桩，1:code打桩 */
    /*
     * *< Tx transmit power, G/W/C:0.1dbm L/NR:1/8dbm. For NR,
     * if Power<=-100dbm, directly control minimum apc for saving rfdsp cycle
     */
    VOS_INT16                  txPower;
    EasyrfOmTxPaModeEnumUint16 paMode;  /* pa mode */
    VOS_UINT32                 apcCode; /* RFGAIN控制字 */
} EasyrfTxApcParaStub;

/*
 * 结构说明: TOOL->BBA TX apc打桩原语消息
 */
typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16          toolsId; /* 保留字段 */
    EasyrfTxApcParaStub txApcStubPara;
} UphyToolEasyrfTxApcStubReq;

/*
 * Name: EasyrfGpioStatusReadParaStru
 * Description: request of read rfdsp register(s) through HIDS.
 */
typedef struct {
    EASYRF_OM_SwitchUint16  switchOn; /* 0 去使能寄存器回读，1 使能寄存器回读 */
    EASYRF_OM_ReadTrxUint16 isTrx;    /* 0 回读TX, 1 回读RX, 2 tx/rx都回读 */
} EasyrfGpioStatusReadPara;

/*
 * 结构说明: TOOL->BBA gpio read原语消息
 */
typedef struct {
    VOS_MSG_HEADER                    /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16               toolsId; /* 保留字段 */
    EasyrfGpioStatusReadPara gpioReadPara;
} UphyToolEasyrfGpioReadReq;

/*
 * Name: EasyrfMipidevCmdStru
 * Description:
 */
typedef struct {
    VOS_UINT32 bitByteCnt : 5;     /* Byte Cnt,从1开始有效 */
    VOS_UINT32 bitMipiPortSel : 3; /* MIPI PORT选择,从0开始有效 */
    VOS_UINT32 bitSlaveId : 4;     /* Savle Id,0-15 */
    VOS_UINT32 bitRegAddr : 16;    /* 寄存器地址 */
    VOS_UINT32 bitRsv : 4;
} EasyrfMipidevCmd;

/*
 * Name: EasyrfRffeMipiReadDataStru
 * Description:
 */
typedef struct {
    VOS_UINT8        mipiNum;
    VOS_UINT8        isTrx;                            /* 0 回读TX, 1 回读RX, 2 tx/rx都回读 */
    VOS_UINT16       applyType;                        /* *< immediately or timing  */
    VOS_UINT32       applyTime;                        /* *< Time when data is valid, Unit: MC */
    EasyrfMipidevCmd cmd[EASYRF_OM_MAX_RD_MIPI_COUNT]; /* *< Port, Device, Address */
} EasyrfRffeMipiReadData;

/*
 * 结构说明: TOOL->BBA gpio read原语消息
 */
typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16             toolsId; /* 保留字段 */
    EasyrfRffeMipiReadData mipiReadPara;
} UphyToolEasyrfMipiReadReq;

/*
 * 结构说明: TOOL-BBA寄存器回读原语消息
 */
typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16    toolsId; /* 保留字段 */
    EasyrfPdOmReq omPara;
} UphyToolEasyrfPdOmReq;

/*
 * 结构说明: TOOL-BBA UL TIMING更新原语
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16              toolsId; /* 保留字段 */
    EasyrfUlTimingUpdateReq timingPara;
} UphyToolEasyrfUlTimingUpdateReq;

/*
 * 结构说明: TOOL-BBA UL TIMING更新原语
 */
typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16 toolsId;   /* 保留字段 */
    VOS_UINT16 isEnable;  /* 测试桩使能 */
    VOS_UINT16 reserved1;
    VOS_UINT16 resetInd;  /* 0 -- rfic0, 1 -- rfic1, 2 -- all */
    VOS_UINT16 period;    /* 触发EasyRF异常的周期, 单位是秒 */
    VOS_UINT32 reserved2;
} UphyToolEasyrfRfResetTestReq;

/*
 * 结构说明: TOOL-EASYRF源端统计上报配置请求消息
 */
typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT16 msgId;
    /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16 toolsId;   /* 保留字段 */
    VOS_UINT16 rptSwitch; /* 上报开关 */
    VOS_UINT16 period;    /* 上报周期 */
} UPHY_TOOL_EasyrfSrcReportCfgReq;

/*
 * 结构说明: G从模INT0配置超时指示原语消息
 */
typedef struct {
    GPHY_TOOL_MntnMsgIdUint16 msgId;              /* 原语类型 */ /* _H2ASN_Skip */
    VOS_UINT16                toolsId;            /* 保留字段 */
    VOS_UINT16                rsv;                /* 保留字段 */
    VOS_UINT16                timeBtwInt0AndInt1; /* int0到int1时间，us */
    VOS_UINT32                timeBeforeCfg;      /* 配置前所用时间，us */
} GPHY_TOOL_GSLAVE_INT0_TIMEOUT_IND_STRU;

/*
 * 结构说明: BBP硬件启动工作的时间信息
 */
typedef struct {
    VOS_UINT16 cfgTimingType; /* 类型 */
    VOS_UINT16 sfncfn;        /* 帧号,取值范围[0,255],仅在uhwImmediateActiveFlag=0时有效 */
    VOS_UINT16 slot;          /* 时隙号,取值范围[0,14],仅在uhwImmediateActiveFlag=0时有效 */
    VOS_UINT16 chip;          /* Chip */
    VOS_UINT16 period;        /* 4096 or 256 */
    VOS_UINT16 timeTypeMc;    /* 时间类型是否为MC,TRUE就是MC类型.从模的时候为MC不需要转两次,直接用MC */
    VOS_UINT32 rfCfgMc;       /* 从模下直接用MC */
} WPHY_TOOL_AeasyrfCfgTime;

/*
 * 结构说明: 获取U寄存器BBPCOMM_W_TIMING_DSP_UNION定时信息
 */
typedef struct {
    VOS_UINT16 sfn;  /* umts timing信息,等效w_sfn_dsp */
    VOS_UINT16 slot; /* umts timing信息,等效w_slot_cnt_dsp */
    VOS_UINT16 chip; /* umts timing信息,等效w_chip_cnt_dsp */
    VOS_UINT16 mfn;  /* umts timing信息,复帧号 */
} UPHY_TOOL_IratUmtsTimingInfo;

/*
 * 结构说明: EASYRF适配模块维测全局信息结构体
 */
typedef struct {
    UPHY_TOOL_MntnBoolUint16          cfgResult;       /* 本次配置结果 */
    WPHY_TOOL_AeasyrfConfigTypeUint16 easyRfCfgType;   /* 业务层下发的easyrf配置类型 */
    VOS_UINT32                        centerFreq;      /* 配置的频点 */
    VOS_UINT16                        band;            /* 配置的band */
    VOS_UINT16                        rfCfgErrorFlag;  /* 发生过射频配置异常的标记 */
    VOS_UINT32                        rfCfgErrorSlice; /* 记录最新一次异常start的slice */
    VOS_UINT16                        chanType;        /* 配置通道类型 */
    VOS_UINT16                        rxAntCfgMap;     /* RX打开天线配置:bit0:主集;bit1:分集 */
    VOS_UINT16                        activeMode;      /* 前端激活模式 */
    VOS_UINT16                        taskNum;         /* 分解后RFHAL接口任务个数 */
    WPHY_TOOL_AeasyrfCfgTime          cfgActiveTime;   /* 高层配置的激活时间 */
    UPHY_TOOL_IratUmtsTimingInfo      bbpWorkTime;     /* 反馈给业务层的BBP可以工作时间记录 */
    /* 分解的每个EASYRF任务配置对应指令的配置时刻 */ /* 配置的MC时刻 */
    VOS_UINT32 mcCfgTime[WPHY_TOOL_AEASYRF_CONFIG_TASK_MAX_NUM];
    VOS_UINT32 cfgStartTime;     /* easyrf前端配置开始时刻记录,单位:MC */
    VOS_UINT32 cfgFinishTime;    /* easyrf前端配置完成时刻记录,单位:MC */
    VOS_UINT32 currRxCenterFreq; /* 当前工作的Rx freq,中心频率单位kHz */
    VOS_UINT32 currTxCenterFreq; /* 当前工作的Tx freq,中心频率单位kHz */
    VOS_UINT16 currRxBand;       /* 当前工作的Rx band */
    VOS_UINT16 currTxBand;       /* 当前工作的Tx band */
} WPHY_TOOL_AeasyrfMntn;

/*
 * 结构说明: AEASYRF CTRL INFO维测
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */
    VOS_UINT16                modemId;
    VOS_UINT16                rsv;
    WPHY_TOOL_AeasyrfMntn     mntnInfo[WPHY_TOOL_AEASYRF_MNTN_TASK_NUM];
} WPHY_TOOL_AeasyrfCtrlInfo;

/*
 * 结构说明: EASYRF适配层和业务层的配置接口公共信息
 */
typedef struct {
    VOS_UINT16               modemId;          /* 配置对应的modem id  */
    VOS_UINT16               workBand;         /* 配置任务(rx stop除外)的band */
    VOS_UINT16               rsv;
    VOS_UINT16               activeMode;       /* 前端激活模式 */
    UPHY_TOOL_MntnBoolUint16 bandWidth10MFlag; /* 是否10M带宽 */
    UPHY_TOOL_MntnBoolUint16 dcScSwitchFlag;   /* 是否DS/SC切换,需要前端特殊处理 */
    VOS_UINT16               chanType;         /* 配置通道类型 */
    VOS_UINT16               rxAntNum;         /* RX支持天线个数,NV获取 */
    VOS_UINT16               rxAntCfgMap;      /* RX打开天线配置:bit0:主集;bit1:分集 */
    VOS_UINT16               rxWorkFreqNo;
    VOS_UINT32               rxWorkCenterFreq; /* RX工作频点(Rx Stop除外),单位kHz,除stop指令外的其它指令对应频点信息 */
    VOS_UINT32               txWorkCenterFreq; /* TX工作频点(Rx Stop除外),单位kHz,除stop指令外的其它指令对应频点信息 */
    WPHY_TOOL_AeasyrfCfgTime activeTime;       /* 激活时间配置 */
} WPHY_TOOL_AeasyrfConfigCommonInfo;

/*
 * 结构说明: EASYRF适配层和业务层的配置接口中AGC相关信息
 */
typedef struct {
    VOS_UINT16 agcGainIndex; /* *< Gain level index, Range:0-7 */
    VOS_UINT16 workType;     /* *< AGC Work Type */
} RFHAL_TOOL_RxAgcAntPath;

/*
 * 结构说明: EASYRF适配层和业务层的配置接口中AGC相关信息
 */
typedef struct {
    VOS_UINT16              agcAdjustMode;                       /* AGC调整模式 */
    VOS_UINT16              sysSyncState;                        /* 下行和网络的同步状态 */
    RFHAL_TOOL_RxAgcAntPath antInfo[WPHY_TOOL_WBBP_MAX_ANT_NUM]; /* AGC档位信息 */
    VOS_UINT16              configType;                          /* Set if analog gain is changed or unchanged */
    VOS_UINT16              rsv;
} WPHY_TOOL_AeasyrfConfigAgcInfo;

/*
 * 结构说明: EASYRF适配层和业务层的配置接口中APC相关信息
 */
typedef struct {
    VOS_UINT16 chanType;        /* 上行信道类型 */
    VOS_UINT16 rsv;
    VOS_UINT32 ulDpaDpchOffset; /* 上行HS-DPCCH信道和DPCH信道偏差,单位chip。不支持HS-DPCCH信道时配置为0 */
} WPHY_TOOL_AeasyrfConfigApcInfo;

/*
 * 结构说明: EASYRF适配层和业务层的配置接口总接口
 */
typedef struct {
    WPHY_TOOL_AeasyrfConfigTypeUint16 easyRfCfgType;       /* easyrf配置任务类型 */
    VOS_UINT16                        rsv;
    WPHY_TOOL_AeasyrfConfigCommonInfo easyRfCfgCommonPara; /* easyrf配置公共参数 */
    /* easyrf配置AGC接口特有参数,仅当调度类型为包含AGC任务的单任务和组合任务时有效 */
    WPHY_TOOL_AeasyrfConfigAgcInfo easyRfCfgAgcSpcPara;
    /* easyrf配置APC接口特有参数,仅当调度类型为包含APC任务的单任务和组合任务时有效 */
    WPHY_TOOL_AeasyrfConfigApcInfo easyRfCfgApcSpcPara;
} WPHY_TOOL_AeasyrfConfigInfo;

/*
 * 结构说明: EASYRF配置任务节点结构体
 */
typedef struct {
    UPHY_TOOL_MntnBoolUint16    nodeUsedFlag;
    VOS_UINT16                  rsv;
    WPHY_TOOL_AeasyrfConfigInfo rfCfgInfo;
} WPHY_TOOL_AeasyrfCfgTaskNode;

/*
 * 结构说明: AEASYRF CFG TASK LIST INFO维测
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16    msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                   toolsId; /* 保留字段 */
    VOS_UINT16                   listUsedDepth;
    VOS_UINT16                   rsv;
    WPHY_TOOL_AeasyrfCfgTaskNode rfCfgTaskNode[WPHY_TOOL_RF_EASYRF_TASK_LIST_MAX_NUM];
} WPHY_TOOL_AeasyrfCfgTaskListInfo;

/*
 * 结构说明: 校准过程中采数可维可测信息上报
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    WPHY_TOOL_NoSignalMntnTypeUint16 type; /* 记录校准维测类型 */
    VOS_UINT16                       reserved;

    VOS_UINT16 rptTimes; /* 此次上报数据的次数 */
    VOS_UINT16 dataNum;  /* 此次上报数据个数 */

    VOS_UINT16 band;     /* 频段 */
    VOS_UINT16 freqChan; /* 频点 */

    VOS_UINT32 userData1; /* 用户自己使用 */
    VOS_UINT32 userData2; /* 用户自己使用 */
    VOS_UINT32 userData3; /* 用户自己使用 */
    VOS_UINT32 userData4; /* 用户自己使用 */
    VOS_UINT32 data[WPHY_TOOL_REPORT_SELF_CAL_SAMPLE_DATA_MAX_SIZE];
} WPHY_TOOL_RptCalSampleDataMntn;

/*
 * 结构说明: U模前端非信令维测接口
 */
typedef struct {
    WPHY_TOOL_MntnMsgIdUint16 msgId;   /* _H2ASN_Skip */ /* 原语类型 */
    VOS_UINT16                toolsId; /* 保留字段 */

    WPHY_TOOL_NoSignalMntnTypeUint16 mntnType;
    VOS_UINT16                       reserved;

    /* bbp寄存器回读结果 */
    VOS_UINT32 bbpRegReadResult[UPHY_TOOL_RDBACK_MIPI_NUM];

    /* 快速校准过程中的MIPI回读结果 */
    UPHY_TOOL_MipiRdResult rficMipiRead[UPHY_TOOL_RDBACK_MIPI_NUM]; /* RFIC MIPI回读 */
    UPHY_TOOL_MipiRdResult rffeMipiRead[UPHY_TOOL_RDBACK_MIPI_NUM]; /* RFFE MIPI回读 */
} WPHY_TOOL_NoSignalMntnRpt;

/*
 * 8 OTHERS定义
 */

/*
 * 9 全局变量声明
 */

/*
 * 10 函数声明
 */

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of phytoolinterface.h */

