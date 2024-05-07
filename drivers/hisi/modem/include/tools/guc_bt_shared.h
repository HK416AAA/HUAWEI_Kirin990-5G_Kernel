
#ifndef GUC_BT_SHARED_H
#define GUC_BT_SHARED_H

#include "vos.h"
#include "bbic_cal_comm_ext.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#pragma pack(push, 4)

#define OM_BER_DATA_MAX_SIZE 1024
#define LISTMODE_MSG_ID_MAX_NUM 50
#define CBT_CHAN_MAX_SEGMENT_COUNT 10

/*
 * 功能描述: NOSIG和装备的消息ID
 */
typedef enum {
    NBT_GUC_TOOL_MSG_ID_BEGIN = 0x1600,

    APP_OM_NON_SIG_BT_TRAN_REQ = 0x1601,  /* PC_NBT_TransMsgReq */
    OM_APP_NON_SIG_BT_TRAN_CNF = 0x1602,  /* UECBT_ResultIndMsg */

    APP_OM_W_SYNC_STATUS_REQ = 0x1603,  /* UECBT_UniformMsg */
    OM_APP_W_SYNC_STATUS_CNF = 0x1604,  /* UECBT_ResultIndMsg */

    APP_OM_GE_DL_STATUS_REQ = 0x1605,   /* PC_NBT_GeDlStatusReq */
    OM_APP_GE_DL_STATUS_CNF = 0x1606,   /* UECBT_ResultIndMsg */

    APP_OM_EDGE_BLER_REQ = 0x1607,  /* UECBT_UniformMsg */
    OM_APP_EDGE_BLER_CNF = 0x1608,  /* UECBT_ResultIndMsg */

    APP_OM_W_BER_DATA_REQ = 0x1609, /* PC_NBT_WcBerReq */
    OM_APP_W_BER_DATA_CNF = 0x160a, /* UECBT_ResultIndMsg */
    OM_APP_W_BER_DATA_IND = 0x160b, /* SOCPP Header + NBT_WBerIndPara */

    APP_OM_LISTMODE_BT_GETMSG_REQ = 0x1611,
    OM_APP_LISTMODE_BT_GETMSG_CNF = 0x1612, /* SOCPP Header + NBT_GetMsgCnfPara */

    APP_OM_LISTMODE_BT_TRAN_REQ = 0x1613, /* SOCPP Header + NBT_GuListmodeData */
    OM_APP_LISTMODE_BT_TRAN_CNF = 0x1614, /* UECBT_ResultIndMsg */

    APP_OM_LISTMODE_BT_TEST_REQ = 0x1615, /* NBT_GuListModeProcReq */
    OM_APP_LISTMODE_BT_TEST_CNF = 0x1616, /* UECBT_ResultIndMsg */

    OM_APP_LISTMODE_BT_BER_IND = 0x1617, /* SOCP Header + NBT_ListModeBerIndPara */
    OM_APP_LISTMODE_BT_RSSI_IND = 0x1618, /* SOCP Header + NBT_ListModeRssiIndPara */

    APP_OM_BT_W_TX_SET_POWER_REQ = 0x1619, /* NBT_WSetPowerReq */
    OM_APP_BT_W_TX_SET_POWER_CNF = 0x1620, /* UECBT_ResultIndMsg */

    APP_OM_BT_WG_RSSI_REQ = 0x1621, /* PC_NBT_WgGetRssiReq */
    OM_APP_BT_WG_RSSI_CNF = 0x1622, /* SOCP Header + NBT_RssiCnfPara */

    APP_OM_G_BER_DATA_REQ = 0x1624, /* PC_NBT_WcBerReq */
    OM_APP_G_BER_DATA_CNF = 0x1625, /* UECBT_ResultIndMsg */
    OM_APP_G_BER_DATA_IND = 0x1626, /* SOCP Header + NBT_GBerIndPara */

    APP_OM_NON_SIG_BT_C_SYNC_REQ = 0x1701, /* UECBT_UniformMsg */
    OM_APP_NON_SIG_BT_C_SYNC_CNF = 0x1702, /* UECBT_ResultIndMsg */
    APP_OM_NON_SIG_BT_C_FER_REQ = 0x1703, /* PC_NBT_WcBerReq */
    OM_APP_NON_SIG_BT_C_FER_CNF = 0x1704, /* SOCP Header + NBT_CFerCnfPara */

    APP_OM_BT_C_TX_SET_POWER_REQ = 0x1705, /* SOCP Header + NBT_CSetPowerReqPara */
    OM_APP_BT_C_TX_SET_POWER_CNF = 0x1706, /* UECBT_ResultIndMsg */

    NBT_GUC_TOOL_MSG_ID_END,

    NBT_GUC_TOOL_MSG_ID_BUTT
} NBT_GUC_ToolMsgId;

typedef struct {
    VOS_UINT16 berValue[2];
} NBT_GeDlStatusPara;

/*
 * 功能说明: GUC BT 消息透传内容
 */
typedef struct {
    VOS_UINT16 mode;
    VOS_UINT16 rsv;
} NBT_WgGetRssiReqPara;

typedef struct {
    VOS_UINT16 modeType; /* 1:GSM 2: EDGE */
    VOS_UINT16 rsv;
} NBT_GeDlStatusReqPara;

/*
 * 功能说明: GUC BT 消息透传内容
 */
typedef struct {
    VOS_UINT16 enableReport;
    VOS_UINT16 msgCnt;
    VOS_UINT8 data[0];
} NBT_TransMsgReqPara;

/*
 * 功能说明: W模C模 BT Ber测试请求
 */
typedef struct {
    VOS_UINT16 frameNum;
    VOS_UINT16 rsv;
} NBT_WcBerReqPara;

/*
 * 功能说明: W模 BT 配置功率请求
 */
typedef struct {
    VOS_INT32 power10th;
} NBT_WSetPowerReqPara;

/*
 * 功能说明: W模 BT 功率配置参数
 */
typedef union {
    VOS_UINT16 gsmPcl;    /* Gsm配置的PCL等级 */
    VOS_INT16 wPower10th; /* Wcdma配置的期望power */
} NBT_GuPowerCtrl;

/*
 * 功能说明: listmode 一个segment的数据结构
 */
typedef struct {
    VOS_UINT16 tpcEn;
    VOS_UINT16 rsv;
    NBT_GuPowerCtrl wgPowerCtrl;
    VOS_UINT16 txFramCnt;
} NBT_ListModeSegment;

/*
 * 功能说明: listmode 一个信道的数据结构
 */
typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 enableRssiReport : 1; /* 0 不使能，1 使能 */
    VOS_UINT16 offSetFrames : 7;     /* 单位: Frame */
    VOS_UINT16 rssiReportFrames : 8; /* 单位: Frame */
    VOS_UINT16 txChan;
    VOS_UINT16 rxChan;
    VOS_UINT16 durationFrameCnt;
    VOS_UINT16 reportFrameCnt;
    VOS_UINT16 segmentCount;
    VOS_INT16  cbtCallRxLevel;
    NBT_ListModeSegment segment[CBT_CHAN_MAX_SEGMENT_COUNT];
} NBT_ListModeChanPara;

/*
 * 功能说明: listmode开始测试req消息参数
 */
typedef struct {
    VOS_UINT16 enableReport;
    VOS_UINT16 mode;
    VOS_UINT16 chanCount;
    VOS_UINT16 cbtCallTimeOut;
    VOS_UINT8 data[0];
} NBT_ListModePara;

/*
 * 功能说明: listmode配置消息内容消息
 */
typedef struct {
    VOS_UINT16 mode;
    VOS_UINT16 rsv;
    VOS_UINT32 msgCount;
    VOS_UINT8 data[0];
} NBT_GuListmodeData;

/*
 * 功能说明: listmode配置消息内容消息
 */
typedef struct {
    VOS_UINT16 enableTpc;
    VOS_INT16 dedicatePower; /* 单位 0.125 dBm */
} NBT_CSetPowerReqPara;

/*
 * 结构说明: Ber上报消息内容
 */
typedef struct {
    VOS_UINT16 totalFrameNum;
    VOS_UINT16 curFrameNum;
    VOS_UINT16 band;
    VOS_UINT16 rxChan;
    VOS_UINT16 crcData;
    VOS_UINT16 listModeSegIndex; /* list mode segment index, start by 1 */
    VOS_UINT32 dataLen;          /* 数据长度,单位:字节, 在数值上是4的整数倍,范围[0..WTTFPHY_MAX_PHY_DATA_REQ_LEN-1] */
    VOS_UINT8 data[OM_BER_DATA_MAX_SIZE]; /* 上行数据块 */
} NBT_ListModeBerIndPara;

/*
 * 功能说明: 上报给PC侧list mode需要的msgid回复消息内容
 */
typedef struct {
    VOS_UINT32 result;
    VOS_UINT32 count;
    VOS_UINT32 msgId[LISTMODE_MSG_ID_MAX_NUM];
} NBT_GetMsgCnfPara;

/*
 * 功能说明: 上报给PC侧的RSSI CNF内容
 */
typedef struct {
    VOS_UINT32 errorCode;
    VOS_INT32 rxLevel; /* 上报的RX level 0.125dBm */
} NBT_RssiCnfPara;

/*
 * 功能说明: 上报给PC侧的W BER IND内容
 */
typedef struct {
    VOS_UINT16 totalFrameNum;
    VOS_UINT16 curFrameNum;
    VOS_UINT32 dataLen; /* 数据长度,单位:字节,在数值上是4的整数倍,范围[0..WTTFPHY_MAX_PHY_DATA_REQ_LEN-1] */
    VOS_UINT8 data[OM_BER_DATA_MAX_SIZE]; /* 上行数据块 */
} NBT_WBerIndPara;

/*
 * 结构说明: C模 Fer上报消息内容
 */
typedef struct {
    VOS_UINT32 errorCode;
    VOS_UINT16 totalFrameNum;
    VOS_UINT16 badFrameNum;
    VOS_UINT32 totalBitsNum;
    VOS_UINT32 errorBitsNum;
} NBT_CFerCnfPara;

/*
 * 结构说明: ListMode Rssi上报内容
 */
typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rxChan;
    VOS_UINT16 listModeSegIndex; /* list mode segment index, start by 1 */
    VOS_UINT16 rsv;
    VOS_INT32 rxLevel; /* 上报的RX level 0.125dBm */
    VOS_INT32 rscp;    /* 上报的Rscp 0.125dBm */
} NBT_ListModeRssiIndPara;

/*
 * 结构说明: G模 Ber上报消息结构
 */
typedef struct {
    VOS_UINT16 totalFrameNum;
    VOS_UINT16 curFrameNum;
    VOS_UINT16 crcData;
    VOS_UINT16 dataLen; /* 数据长度,单位:字节, 在数值上是4的整数倍, 范围[0..WTTFPHY_MAX_PHY_DATA_REQ_LEN-1] */
    VOS_UINT8 data[OM_BER_DATA_MAX_SIZE]; /* 上行数据块 */
} NBT_GBerIndPara;

typedef struct {
    VOS_UINT32 firstErrNum;
    VOS_UINT32 firstTotalNum;
    VOS_UINT32 secondErrNum;
    VOS_UINT32 secondTotalNum;
} NBT_EdgeBlerCnfPara;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_TransMsgReqPara para;
} NBT_TransMsgReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
} NBT_UniformMsg;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    VOS_UINT32 errCode;
} NBT_ErrCodeInd;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_GeDlStatusReqPara para;
} NBT_GeDlStatusReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_WcBerReqPara para;
} NBT_WcBerReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_WSetPowerReqPara para;
} NBT_WSetPowerReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_WgGetRssiReqPara para;
} NBT_WgGetRssiReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_CSetPowerReqPara para;
} NBT_CSetPowerReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    VOS_UINT16 mode;
    VOS_UINT16 rsv;
    VOS_UINT32 msgCount;
    VOS_UINT8 data[0];
} NBT_ListmodeContextReqPara;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    VOS_UINT16 enableReport;
    VOS_UINT16 mode;
    VOS_UINT16 chanCount;
    VOS_UINT16 cbtCallTimeOut;
    NBT_ListModeChanPara chanPara[10];
} NBT_ListModeTestReq;

/*
 * 功能说明: 上报给PC侧的W BER IND内容
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_WBerIndPara para;
} NBT_WBerDataInd;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_GetMsgCnfPara para;
} NBT_GetMsgCnf;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_ListModeBerIndPara para;
} NBT_ListModeBerInd;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_ListModeRssiIndPara para;
} NBT_ListModeRssiInd;

/*
 * 功能说明: 上报给PC侧的RSSI CNF内容
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_RssiCnfPara para;
} NBT_RssiCnf;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_GBerIndPara para;
} NBT_GBerInd;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 msgId;
    COMPONENT_ID_STRU componentId;
    NBT_CFerCnfPara para;
} NBT_CFerCnf;


#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
