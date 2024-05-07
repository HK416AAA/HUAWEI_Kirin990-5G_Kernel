
#ifndef PC_UE_CAL_H
#define PC_UE_CAL_H

#include "cbt_socp_head.h"
#include "bbic_cal_shared.h"
#include "nbt_shared.h"
#include "lbt_shared.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#pragma pack(push, 4)

#define OM_MSG_CBT_SERVICE_ID 7
#define OM_MSG_CBT_SERVICE_SESSION_ID 1
#define PC_UE_MSG_MAX_LEN 65536

#define HIRF_PHY_A0A1_SAMPLE_NUM_MAX 128
#define HIRF_PHY_2R_ANT_NUM 2
#define HIRF_PHY_GSM_TX_SLOT_NUM 8

#ifndef NBT_MAX_UL_CC_NUM
#define NBT_MAX_UL_CC_NUM 2
#endif
#ifndef NBT_MAX_DL_CC_NUM
#define NBT_MAX_DL_CC_NUM 5
#endif
#ifndef RX_ANT_NUM_MAX
#define RX_ANT_NUM_MAX 8
#endif
#ifndef NBT_HF_MAX_BEAMNUM
#define NBT_HF_MAX_BEAMNUM 4
#endif

#ifndef NBT_TX_ANT_NUM_MAX
#define NBT_TX_ANT_NUM_MAX 4
#endif

enum NBT_TestMode {
    NBT_TESTMODE_CA = 0,
    NBT_TESTMODE_SC,
    NBT_TESTMODE_ENDC,
    NBT_TESTMODE_BUTT
};
typedef UINT16 NBT_TestModeUint16;

enum TEMP_LOGIC_CHAN_ENUM {
    TEMP_LOGIC_CHAN_XO_TEMP = 17, /* < 获取晶体温度 */
    TEMP_LOGIC_CHAN_BUTT
};
typedef UINT32 TEMP_LOGIC_CHAN_ENUM_UINT32;

enum BT_TASK_NAME_ENUM {
    BT_TASK_CALL = 0,
    BT_TASK_RX_BLER,
    BT_TASK_RX_RSSI,
    BT_TASK_TX,
    BT_TASK_RELEASE,
    BT_TASK_FR2_TX_BEAM_CFG,
    BT_TASK_FR2_HOLD,
    BT_TASK_TX_SAR_CFG,
    BT_TASK_BUTT
};
typedef UINT32 BT_TASK_NAME_ENUM_UINT32;



enum HIRF_MULTI_RAT_ENUM {
    HIRF_SA  = 0,
    HIRF_NSA = 1,
    HIRF_MULTI_RAT_BUTT
};
typedef UINT16 HIRF_MULTI_RAT_ENUM_UINT16;

/*
 * 结构说明: UECBT消息格式
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16            msgId;
    COMPONENT_ID_STRU     componentId;
    VOS_UINT32            msgLength;
    VOS_UINT8             para[0]; /* 消息内容 */
} UECBT_UniformMsg;

/*
 * 结构说明: UECBT消息格式
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16            msgId;
    COMPONENT_ID_STRU     componentId;
    VOS_UINT32            msgLength;
    VOS_UINT32            errorCode;
} UECBT_ResultIndMsg;

#define PC_UE_PAM_STRU_BEGIN

/*
 * Description: PC->UE建立连接REQ消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;
    UINT16                 usMsgId;
    OM_COMPONENT_MODE_STRU stCompMode;
    UINT32                 ulMsgLength;
    UINT32                 ulType;
    UINT32                 aulRsv[4];
} PC_UE_ESTABLISH_REQ_STRU;

/*
 * Name        : PC_UE_RELEASE_REQ_STRU
 * Description : PC->UE释放连接REQ消息
 */
typedef PC_UE_ESTABLISH_REQ_STRU PC_UE_RELEASE_REQ_STRU;

typedef struct {
    UINT16 usVVerNO;
    UINT16 usRverNO;
    UINT16 usCVerNO;
    UINT16 usBVerNO;
    UINT16 usSpcNO;
    UINT16 usCustomNOv;
    UINT32 ulProductNo;
    UINT8  aucBuildDate[12];
    UINT8  aucBuildTime[12];
} MSE_VER_INFO_S;

/*
 * Description: UE->PC建立连接CNF消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;
    UINT16                 usMsgId;
    OM_COMPONENT_MODE_STRU stCompMode;
    UINT32                 ulMsgLength;
    UINT32                 ulResult;
    UINT16                 usChipId;
    UINT16                 usRsv;
    MSE_VER_INFO_S         stswverinfo;
    UINT8                  aucReserve[212];
} PC_UE_ESTABLISH_CNF_STRU;

/*
 * Description: UE->PC释放连接CNF消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;
    UINT16                 usMsgId;
    OM_COMPONENT_MODE_STRU stCompMode;
    UINT32                 ulMsgLength;
    UINT32                 ulErrorCode;
} PC_UE_RELEASE_CNF_STRU;


/*
 * Description: PC->UE切换非信令REQ消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulMode;
} PC_UE_SET_FTM_MODE_REQ_STRU;

/*
 * Description: UE->PC切换非信令CNF消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
} PC_UE_SET_FTM_MODE_CNF_STRU;


typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;   /* 和原结构与原定义一致，消息头 */
    UINT16                 usMsgId;       /* 和原结构与原定义一致，消息ID */
    COMPONENT_ID_STRU      stComponentId; /* 和原结构与原定义一致，
                                             高4bit为处理的组件ID，LOADPHY需要填为1 */
    UINT32            ulDataLength;       /* 和原结构与原定义一致，参数的长度 */
    LOADPHY_PARA_STRU stLoadPhyPara;      /* LOADPHY参数 */
} PC_UECBT_LOAD_PHY_REQ_STRU;

/*
 * Description: UECBT -> PC LOADPHY CNF消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;   /* 和原结构与原定义一致，消息头 */
    UINT16                 usMsgId;       /* 和原结构与原定义一致，消息ID */
    COMPONENT_ID_STRU      stComponentId; /* 和原结构与原定义一致，
                                             高4bit为处理的组件ID，UECBT不做填写 */
    UINT32 ulDataLength;                  /* 和原结构与原定义一致，参数的长度 */
    UINT32 ulErrorCode;                   /* 结果: 0:SUCC
               1、2、3 ..... 各种失败情况 */
} UECBT_PC_LOAD_PHY_CNF_STRU;

/*
 * Description: HIRF->UE加载物理层REQ消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulLength;
    RAT_MODE_ENUM_UINT16       enRatMode;
    UINT8                      ucWorkMode; /* 0: 单模场景； 1: 双模场景(L+NR同时打开) */
    UINT8                      ucBusiness; /* 建议和物理层的定义统一 */
} HIRF_UE_LOAD_PHY_REQ_STRU;

/*
 * Description: UE->HIRF加载物理层Cnf消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulLength;
    UINT32                     ulErrorCode; /* 0表示成功，N(1,2,3...)表示不同错误 */
} HIRF_UE_LOAD_PHY_CNF_STRU;

/*
 * Description: PC->UE reload nv req
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU              msgHeader;
    UINT16                             msgId; /* 0x81b3 */
    COMPONENT_ID_STRU                  componentId;
    UINT32                             length;
    RAT_MODE_ENUM_UINT16               ratMode;
    BBA_CAL_RELOAD_NV_TYPE_ENUM_UINT16 type;
} PC_UE_ReloadNvStru;

/*
 * Description: UE->PC重新加载物理层CNF消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
} PC_UE_RELOAD_NV_CNF_STRU;

/*
 * Description: PC->UE查询温度接口，参数为逻辑通道
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU       stMsgHeader;
    UINT16                      usMsgId; /* 0x80C1 */
    COMPONENT_ID_STRU           stComponentId;
    UINT32                      ulLength;
    TEMP_LOGIC_CHAN_ENUM_UINT32 ulType; /* 逻辑通道 */
} PC_UE_QUERY_TEMP_LOGIC_CHAN_REQ_STRU;

/*
 * Description: UE->PC返回温度结果
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
    INT32                 lValue; /* 结果 */
} PC_UE_QUERY_TEMP_LOGIC_CHAN_IND_STRU;

/*
 * Description: 指定某个path的NV重新加载
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU           stMsgHeader;
    UINT16                          usMsgId;
    COMPONENT_ID_STRU               stComponentId;
    UINT32                          ulMsgLen;
    BBIC_CAL_NV_RESET_REQ_PARA_STRU stPara;
} PC_UE_NV_RESET_REQ_STRU;

/*
 * Description: UE->PC返回NV reset结果
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
} PC_UE_NV_RESET_CNF_STRU;

/*
 * Description: PC->UE写NV操作
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                count;
    UINT16                data[0];  // 需要写的nv数据，按照nvid,len,data排列
} PC_UE_NV_WRITE_REQ_STRU;

/*
 * 结构说明: 写NV结果
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU socpHeader;
    VOS_UINT16            msgId; /* 消息ID */
    COMPONENT_ID_STRU     componmentInfo;
    VOS_UINT32            msgLength; /* 从下一字节到消息尾的长度，单位btye */
    VOS_UINT32            errorCode; /* 返回执行结果 */
    VOS_UINT32            errorNvId; /* 返回出现错误的NVID */
} UE_PC_NV_WRITE_CNF_STRU;

/*
 * 结构说明: 写NV刷flash请求
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                count;
    UINT16                data[0];  // 需要写的nv数据，按照nvid,len,data排列
} PC_UE_WRITE_NV_FLASH_REQ_STRU;

/*
 * 结构说明: 写NV刷flash结果
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU socpHeader;
    VOS_UINT16            msgId; /* 消息ID */
    COMPONENT_ID_STRU     componmentInfo;
    VOS_UINT32            msgLength; /* 从下一字节到消息尾的长度，单位btye */
    VOS_UINT32            errorCode; /* 返回执行结果 */
    VOS_UINT32            errorNvId; /* 返回出现错误的NVID */
} UE_PC_WRITE_NV_FLASH_CNF_STRU;

#define BBIC_CAL_COMM_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT32                     ulMsgLen;
    BBIC_CAL_MSG_CNF_PARA_STRU stPara;
} PC_UE_CAL_MSG_CNF_STRU;

#define BBIC_CAL_SELF_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU  componentID;
    UINT32                      msgLen;
    BBIC_CAL_DcxoSelfCalPara    dcxoPara;
} PC_UE_DcxoSelfCalReq;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU  componentID;
    UINT32                      msgLen;
    BBIC_CAL_DcxoSelfCalRslt    dcxoInd;
} PC_UE_DcxoSelfCalInd;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU              stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    UINT32                             ulMsgLen;
    BBIC_CAL_RX_IQ_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_RX_IQ_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RX_FIIQ_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_RX_IQ_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RX_FDIQ_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_RX_FDIQ_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU              stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    UINT32                             ulMsgLen;
    BBIC_CAL_TX_IQ_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_TX_IQ_START_REQ_STRU;

/*
 * desc: 高频Rx ADC校准接口定义
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU          msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     componentId;
    UINT32                         msgLen;
    BBIC_CAL_RficHfRxAdcCalReqPara rxAdcPara;
} PC_UE_RficHfRxAdcSelfCalReq;

/*
 * desc: 高频TRx FDIQ校准接口定义
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU                  msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU             componentId;
    UINT32                                 msgLen;
    BBIC_CAL_HF_SELFCAL_FDIQ_REQ_PARA_STRU fdiqPara;
} PC_UE_RficHfTrxFdiqSelfCalReq;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_TX_IQ_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_TX_IQ_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU              stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    UINT32                             ulMsgLen;
    BBIC_CAL_TX_LO_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_TX_LO_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_TX_LO_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_TX_LO_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU            stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    UINT32                           ulMsgLen;
    BBIC_CAL_IP2_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_IP2_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU             stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU        stComponentID;
    UINT32                            ulMsgLen;
    BBIC_CAL_IP2_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_IP2_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_MRX_DCOC_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_MRX_DCOC_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                  stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    UINT32                                 ulMsgLen;
    BBIC_CAL_MRX_DCOC_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_MRX_DCOC_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    UINT32                               ulMsgLen;
    BBIC_CAL_RX_DCOC_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_RX_DCOC_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RX_DCOC_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_RX_DCOC_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    UINT32                               ulMsgLen;
    BBIC_CAL_GE_RF_SETTING_REQ_PARA_STRU stPara;
} PC_UE_CAL_GE_RF_SETTING_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    UINT32                               ulMsgLen;
    BBIC_CAL_GE_RF_SETTING_IND_PARA_STRU stPara;
} PC_UE_CAL_GE_RF_SETTING_IND_STRU;

#define BBIC_CAL_TRX_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU           stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    UINT32                          ulMsgLen;
    BBIC_CAL_RX_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_RX_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU            stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    UINT32                           ulMsgLen;
    BBIC_CAL_RX_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_RX_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU           stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    UINT32                          ulMsgLen;
    BBIC_CAL_TX_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_TX_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU            stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    UINT32                           ulMsgLen;
    BBIC_CAL_TX_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_TX_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU        stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU   stComponentID;
    UINT32                       ulMsgLen;
    BBIC_CAL_GE_TX_REQ_PARA_STRU stPara;
} PC_UE_CAL_GE_TX_APC_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU        stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU   stComponentID;
    UINT32                       ulMsgLen;
    BBIC_CAL_GE_TX_IND_PARA_STRU stPara;
} PC_UE_CAL_GE_TX_APC_RESULT_IND_STRU;

#define BBIC_CAL_RF_DEBUG_TRX_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU           stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    UINT32                          ulMsgLen;
    BBIC_CAL_RF_DEBUG_TRX_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_TRX_REQ_STRU;

/*
 * Description:Only for GSM TX
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU            stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    UINT32                           ulMsgLen;
    BBIC_CAL_RF_DEBUG_G_TX_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_GSM_TX_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                     stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU                stComponentID;
    UINT32                                    ulMsgLen;
    BBIC_CAL_RF_DEBUG_TX_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_TX_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                   stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    UINT32                                  ulMsgLen;
    BBIC_CAL_RF_DEBUG_GTX_MRX_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_GTX_MRX_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT32                     ulMsgLen;
    /* Need to nothing */
} PC_UE_CAL_RF_DEBUG_RSSI_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    UINT32                                     ulMsgLen;
    BBIC_CAL_RF_DEBUG_RX_RSSI_REPORT_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_RX_RSSI_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT32                     ulMsgLen;
    /* Need to nothing */
} PC_UE_CAL_RF_DEBUG_RX_EVM_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                  stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    UINT32                                 ulMsgLen;
    BBIC_CAL_RF_DEBUG_RX_EVM_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_RX_EVM_IND_STRU;

#define BBIC_CAL_RF_DEBUG_SELF_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_DEBUG_DCXO_CFIX_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_DCXO_CFIX_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT32                     ulMsgLen;
    UINT8                      aucPara[4];
} PC_UE_CAL_RF_DEBUG_UNIFORM_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                       stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU                  stComponentID;
    UINT32                                      ulMsgLen;
    BBIC_CAL_RF_DEBUG_DCXO_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_DCXO_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_RF_DEBUG_IP2_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_IP2_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    UINT32                                     ulMsgLen;
    BBIC_CAL_RF_DEBUG_IP2_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_IP2_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                   stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    UINT32                                  ulMsgLen;
    BBIC_CAL_RF_DEBUG_RX_DCOC_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_RX_DCOC_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                   stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    UINT32                                  ulMsgLen;
    BBIC_CAL_RF_DEBUG_RX_DCOC_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_RX_DCOC_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                    stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    UINT32                                   ulMsgLen;
    BBIC_CAL_RF_DEBUG_MRX_DCOC_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_MRX_DCOC_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                    stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    UINT32                                   ulMsgLen;
    BBIC_CAL_RF_DEBUG_MRX_DCOC_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_MRX_DCOC_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_DEBUG_TX_IQ_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_TX_IQ_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_DEBUG_TX_IQ_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_TX_IQ_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_DEBUG_TX_LO_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_TX_LO_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_DEBUG_TX_LO_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_TX_LO_RESULT_IND_STRU;

#define BBIC_CAL_PHY_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU          stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    UINT32                         ulMsgLen;
    BBIC_CAL_WPHY_CONFIG_PARA_STRU stPara;
} PC_UE_CAL_WPHY_CONFIG_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU          stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    UINT32                         ulMsgLen;
    BBIC_CAL_CPHY_CONFIG_PARA_STRU stPara;
} PC_UE_CAL_CDMA_CONFIG_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU          stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    UINT32                         ulMsgLen;
    BBIC_CAL_LPHY_CONFIG_PARA_STRU stPara;
} PC_UE_CAL_LTE_CONFIG_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT32                     ulMsgLen;
    BBIC_CAL_NB_RSSI_PARA_STRU stPara;
} PC_UE_CAL_NB_RSSI_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU          stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    UINT32                         ulMsgLen;
    BBIC_CAL_NB_RSSI_IND_PARA_STRU stPara;
} PC_UE_CAL_NB_RSSI_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_RFIC_SELFCAL_REQ_PARA_STRU stPara;
} PC_UE_CAL_RFIC_SELFCAL_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_RFIC_SELFCAL_IND_PARA_STRU stPara;
} PC_UE_CAL_RFIC_SELFCAL_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU          tMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    UINT32                         ulMsgLen;
    BBIC_CAL_GE_RAMP_REQ_PARA_STRU stPara;
} PC_UE_CAL_RAMP_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU          stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    UINT32                         ulMsgLen;
    BBIC_CAL_GE_RAMP_IND_PARA_STRU stPara;
} PC_UE_CAL_RAMP_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               tMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_RESULT_WRITE_REQ_PARA_STRU stPara;
} PC_UE_CAL_RESULT_WRITE_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_RESULT_WRITE_CNF_PARA_STRU stPara;
} PC_UE_CAL_RESULT_WRITE_CNF_STRU;

#define BBIC_CAL_RF_DEBUG_HF_TRX_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_DEBUG_HF_RX_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_HF_RX_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT32                     ulMsgLen;
    /* Need to nothing */
} PC_UE_CAL_RF_DEBUG_HF_RSSI_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    UINT32                                     ulMsgLen;
    BBIC_CAL_RF_DEBUG_HF_RX_RSSI_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_HF_RX_RSSI_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_DEBUG_HF_TX_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_DEBUG_HF_TX_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_HF_ANA_TEMP_REQ_PARA_STRU stPara;
} PC_UE_CAL_RF_HF_ANA_TEMP_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                 stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    UINT32                                ulMsgLen;
    BBIC_CAL_RF_HF_ANA_TEMP_IND_PARA_STRU stPara;
} PC_UE_CAL_RF_HF_ANA_TEMP_IND_STRU;

#define BBIC_CAL_HF_TRX_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU              stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    UINT32                             ulMsgLen;
    BBIC_CAL_HF_RX_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_HF_RX_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_HF_RX_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_HF_RX_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU              stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    UINT32                             ulMsgLen;
    BBIC_CAL_HF_TX_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_HF_TX_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU               stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    UINT32                              ulMsgLen;
    BBIC_CAL_HF_TX_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_HF_TX_RESULT_IND_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU componentID;
    UINT32                     msgLen;
    BBIC_CAL_HfDpdStartReqPara para;
} PCUE_CAL_HfDpdStartReq;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU      msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU componentID;
    UINT32                     msgLen;
    BBIC_CAL_HfDpdTriggerPara  para;
} PCUE_CAL_HfDpdFlashLutDdrReq;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU  componentID;
    UINT32                      msgLen;
    BBIC_CAL_HfDpdResultIndPara para;
} PCUE_CAL_HfDpdResultInd;

#define BBIC_CAL_HF_SELF_STRU_BEGIN

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                   stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    UINT32                                  ulMsgLen;
    BBIC_CAL_HF_RX_DCOC_START_REQ_PARA_STRU stPara;
} PC_UE_CAL_HF_RX_DCOC_START_REQ_STRU;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU                    stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    UINT32                                   ulMsgLen;
    BBIC_CAL_HF_RX_DCOC_RESULT_IND_PARA_STRU stPara;
} PC_UE_CAL_HF_RX_DCOC_RESULT_IND_STRU;

#define NR_BT_STRU_BEGIN

/* 用途:NRBT 总配置参数，长度和子段可配置 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     ulSegmengCnt; /* 字段个数 */
    UINT8                      aucData[0];   /* 变长，具体格式参见PC_UE_NRBT_SEGMENT_STRU */
} PC_UE_NRBT_TEST_REQ_STRU;

/* 用途:NRBT 总配置参数，长度和子段可配置 */
typedef struct {
    UINT32 ulTaskName;     /* 任务名称，BT_TASK_NAME_ENUM */
    UINT32 ulDuration;     /* 持续时间，单位ms */
    UINT32 ulTaskDataLen;  /* 任务长度 */
    UINT8  aucDataBuff[0]; /* 数据按照变长定义 */
} PC_UE_NRBT_SEGMENT_STRU;

/* 用途:NRBT CC配置参数 */
typedef struct {
    UINT16 usBandNo;    /* BAND信息 */
    UINT8  ucAntNum;    /* 天线配置数 */
    UINT8  ucBandWidth; /* BAND宽度 */
    UINT32 ulFreqKHz;   /* 频点信息 */
} PC_UE_NRBT_CC_INFO_STRU;

/* 用途:BT CALL时的配置参数 */
typedef struct {
    NBT_TestModeUint16      testMode;
    UINT8                   ulCcIndex;
    UINT8                   dlCcIndex;
    UINT16                  usUlBandNum;
    UINT16                  usDlBandNum;
    PC_UE_NRBT_CC_INFO_STRU astUlCCComfig[NBT_MAX_UL_CC_NUM]; /* 下行CC配置信息 */
    PC_UE_NRBT_CC_INFO_STRU astDlCCComfig[NBT_MAX_DL_CC_NUM]; /* 上行CC配置信息 */
} PC_UE_NRBT_CC_CFG_STRU;

/* 用途:BT CALL时的配置参数 */
typedef struct {
    PC_UE_NRBT_CC_CFG_STRU stCcCfg;         /* CC配置，当前只支持单CC */
    UINT16                 usSsbScs;        /* SSB子载波间隔，枚举定义 */
    UINT16                 usCommScs;       /* COMM子载波间隔，枚举定义 */
    INT16                  sRssi8th;        /* 下行信号强度，单位1/8dBm */
    INT16                  sDuration;       /* 持续时间，子帧个数，0表示立即切换，-1表示除非触发否则不切换 */
    UINT16                 usSlotFormat;    /* 上下行时隙格式 */
    UINT16                 usCpType;        /* Ncp/Ecp */
    UINT16                 usDmrs;          /* DMRS配置 */
    UINT16                 usSsbFreqOffset; /* SSB频点偏移量 */
} PC_UE_NRBT_CALL_CONFIG_STRU;

/* 用途:Rx BLER测试时的配置参数结构 */
typedef struct {
    UINT16 usOffsetSlot; /* 测量时偏移的Slot */
    UINT16 usMinSlot;    /* 测量最少的Slot */
    UINT16 usMaxSlot;    /* 测量最长的Slot */
    UINT16 usTestAntMap; /* 指定测试天线 */
    UINT16 usSlotNum;    /* 帧配比 */
    UINT16 usRsv;
} PC_UE_NRBT_RX_BLER_TEST_REQ_STRU;

/* 用途:Rx Bler结果上报结构 */
typedef struct {
    UINT16 usBandIndex;
    UINT16 usRsv;
    UINT32 ulRxChan;
    UINT32 ulErrBlock[RX_ANT_NUM_MAX];   /* 误码block */
    UINT32 ulTotalBlock[RX_ANT_NUM_MAX]; /* 总的block */
    INT32  lRxRssi[RX_ANT_NUM_MAX];      /* 单位RX level 0.125dBm */
    INT32  lRxSnr[RX_ANT_NUM_MAX];       /* 单位RX Snr  0.125dBm */
    INT32  lRxRscp[RX_ANT_NUM_MAX];      /* 单位RX Rscp  0.125dBm */
} PC_UE_NRBT_RX_BLER_REPORT_IND_STRU;

/* 用途:Rx RSSI测试时的配置参数结构 */
typedef struct {
    UINT16 usOffsetSlot; /* 测量时偏移的Slot */
    UINT16 usSlotNum;
    UINT16 usTestAntMap; /* 指定测试天线 */
    UINT16 usRsv;
} PC_UE_NRBT_RX_RSSI_TEST_REQ_STRU;

/* 用途:Rx Rssi结果上报结构 */
typedef struct {
    UINT16 usBandIndex;
    UINT16 usRsv;
    UINT32 ulRxChan;
    INT32  lRxRssi[RX_ANT_NUM_MAX]; /* 单位RX level 0.125dBm */
} PC_UE_NRBT_RX_RSSI_REPORT_IND_STRU;

/* 用途:Rx结果上报总结构 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     ulSegmentIndex;
    UINT8                      aucData[4];
} PC_UE_NRBT_TEST_IND_STRU;

/* 用途:TX总配置参数总结构 */
typedef struct {
    UINT32 ulSegmentCnt; /* 字段个数 */
    UINT8  aucData[0];   /* 数据变长 */
} PC_UE_NRBT_SET_TX_PWR_REQ_STRU;

/* 用途:用于配置上行功率 */
typedef struct {
    UINT16 usTxOnOff;  /* 是否打开上行:1-打开，0-关闭 */
    UINT16 usTpcOnOff; /* 是否响应TPC，0-不响应，1-响应仪器TPC */
    INT16  sPower;     /* Tpc Off下有效，强配的目标功率 */
    UINT16 mprVal;     /* MPR回退值 */
    UINT16 usModuType; /* 调制方式 */
    UINT16 usOfmdType; /* 符合格式，cp-ofdm或者dtf-s-ofdm */
    UINT16 usRbPos;
    UINT16 usRbNum;
    UINT16 numOfLayer; /* 流个数跟天线num一致 */
    UINT16 antNum;
    UINT16 tpmi;      /* 参考协议38.211 6.3.1.5 */
    INT16  sDuration; /* 持续时间，子帧个数，<=0表示一直维持 */
} PC_UE_NRBT_SET_TX_PWR_STRU;

/* 用途:hold task sub struct */
typedef struct {
    UINT32  holdStatus; /* hold状态,1 - hold，0 - cancel */
} PCUE_NRBT_TaskHold;

/* 用途:回复PC的NR BT的Cnf消息结构 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     ulSegmengIndex; /* 异常子段，无异常填0 */
    UINT32                     ulErrorCode;    /* 错误码，0-正常，若0则忽略ulSegmengIndex */
} PC_UE_NRBT_TEST_CNF_STRU;

/* 用途: NR FR2非信令综测控制TX beam */
typedef struct {
    UINT16    antBitmap;    /* 天线配置 */
    UINT16    rsv;
    UINT8     beamIndex[NBT_HF_MAX_BEAMNUM]; /* 每个天线对应的beam方向，紧缩方式存储 */
} PCUE_NRBT_TaskFr2TxAntCfg;

/* 用途:NRBT下配置TRX TAS参数，需要在进入D态后下发 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT8                      cmd; /* < 1 - 设置参数，3 - 查询状态 */
    UINT8                      rsv[3];
    UINT32                     trxTasValue; /* < bit[3:4] - UE能力,bit[2] - TX索引，bit[0:1] - 天线port号 */
} PCUE_NRBT_TrxTasReqStru;

/*
 * Description:只有当PCUE_NRBT_TrxTasReqStru的cmd为查询时，才会上报ind消息
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     errorCode; /* < < 结果，0表示成功，其他表示失败 */
    UINT32                     trxTasValue;
} PCUE_NRBT_TrxTasIndStru;

/* 用途:NRBT下强配SRS状态，需要在搜网之前下发，SRS强配后RX天线也会切换 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT8                      cmd; /* < 1 - 设置参数，3 - 查询状态 */
    UINT8                      rsv[3];
    UINT32                     srsValue; /* < bit[3:4] - UE能力, bit[2] - TX索引，bit[0:1] - 天线port号 */
} PCUE_NRBT_TxSrsReqStru;

/* 用途:只有PCUE_NRBT_TxSrsReqStru的cmd为查询时才有ind消息上报 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     errorCode; /* < < 结果，0表示成功，其他表示失败 */
    UINT32                     srsValue;
} PCUE_NRBT_TxSrsIndStru;

/*
 * 用途: NR FR2非信令综测需要提前配置ant状态
 */
typedef struct
{
    OM_MSG_SOCP_HEAD_STRU       header;
    BBIC_CAL_COMPONENT_ID_STRU  componentId;
    UINT32                      msgLen;  /* 下一字节到消息尾的长度，单位byte */
    NbtNrFr2AntCfgReqPara       data;
} PCUE_NRBT_Fr2AntCfgReq;

/*
 *用途: NR FR2非信令综测配置ant状态的response
 */
typedef struct
{
    OM_MSG_SOCP_HEAD_STRU       header;
    BBIC_CAL_COMPONENT_ID_STRU  componentId;
    UINT32                      msgLen;   /* 下一字节到消息尾的长度，单位byte */
    UINT32                      errorCode;
} PCUE_NRBT_Fr2AntCfgCnf;

/*
 * 用途: NR TX TAS测试双流双PORT的场景每T的功率回退值
 */
typedef struct
{
    UINT16                      txSarBackPower[NBT_TX_ANT_NUM_MAX]; /* TX降Sar场景每T的功率回退值,单位1/8db */
} PCUE_NRBT_TxSarCfgPara;

#define LTE_BT_STRU_BEGIN

typedef PC_UE_NRBT_TEST_REQ_STRU PC_UE_BtLnrTestReq;
typedef PC_UE_NRBT_SEGMENT_STRU PC_UE_BtLnrSegment;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

