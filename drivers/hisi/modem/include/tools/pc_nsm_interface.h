
#ifndef __PC_NSM_INTERFACE_H__
#define __PC_NSM_INTERFACE_H__

#include "mdrv.h"

enum PC_NSM_MsgId {
    /* OM<->APP : Release link */
    APP_OM_RELEASE_REQ = 0x80b3,
    OM_APP_RELEASE_CNF = 0x80b4,

    /* 设置从信令下切换到非信令下 */
    APP_OM_SET_FTM_REQ = 0x80e5,
    OM_APP_SET_FTM_CNF = 0x80e6,

    /* OM<->APP : LOAD PHY */
    APP_OM_LOADPHY_REQ = 0x81b1,
    OM_APP_LOADPHY_CNF = 0x81b2,

    /* OM<->APP : 根据逻辑通道查询温度 */
    APP_OM_QUERY_TEMP_LOGIC_CHAN_REQ = 0x80c1,
    OM_APP_QUERY_TEMP_LOGIC_CHAN_IND = 0x80c2,

    APP_OM_READ_NV_REQ = 0x8021,
    OM_APP_READ_NV_IND = 0x8022,

    /* OM<->APP : Just for LMT. */
    APP_OM_WRITE_NV_REQ = 0x8023,
    OM_APP_WRITE_NV_CNF = 0x8024,

    APP_OM_NV_BACKUP_REQ = 0x8025,
    OM_APP_NV_BACKUP_CNF = 0x8026,

    /* OM<->APP : */
    APP_OM_C1C2_NV_WRITE_REQ = 0x8030,
    OM_APP_C1C2_NV_WRITE_CNF = 0x8031,


    /* OM<->APP :设置C核NV存储到Flash */
    APP_OM_NV_TO_FLASH_REQ = 0x802B,
    OM_APP_NV_TO_FLASH_CNF = 0x802C,

    /* 非CT场景下的写nv消息id */
    TOOL_UE_WRITE_NV_REQ = 0x9023,
    UE_TOOL_WRITE_NV_CNF = 0x9024,
    TOOL_UE_NV_WRITE_FLASH_REQ = 0x902D,
    UE_TOOL_NV_WRITE_FLASH_CNF = 0x902E,

    /* OM<->APP :设置C核一次性写NV存储Flash */
    APP_OM_NV_WRITE_FLASH_REQ = 0x802D,
    OM_APP_NV_WRITE_FLASH_CNF = 0x802E,

    /* OM<->APP : RELOAD NV */
    APP_OM_RELOADNV_REQ = 0x81b3,
    OM_APP_RELOADNV_CNF = 0x81b4,

    /* OM<->APP :LMT查询DSDA支持状态 */
    APP_OM_QUERY_MODEM_NUM_REQ = 0x8211,
    OM_APP_QUERY_MODEM_NUM_CNF = 0x8212,

    /* OM<->APP :通过物理通道查询PA的温度 */
    APP_OM_PA_TEMP_PHY_CHAN_REQ = 0x80c7,
    OM_APP_PA_TEMP_PHY_CHAN_IND = 0x80c8,

    /* OM<->APP : Restablish link */
    APP_OM_ESTABLISH_REQ = 0x80b1,
    OM_APP_ESTABLISH_CNF = 0x80b2,

};
typedef VOS_UINT16 PC_NSM_MsgIdUint16;

/* Query PA attribute */
enum {
    OM_W_PA_TEMP = 1,
    OM_G_PA_TEMP,
    OM_W_PLL_LOCK,
    OM_G_PLL_LOCK,
    OM_W_HKADC,
    OM_G_HKADC,
    OM_W_BBP_PLL_LOCK,
    OM_G_BBP_PLL_LOCK,
    OM_DSP_PLL_LOCK,
    OM_ARM_PLL_LOCK,
    OM_SIM_TEMP,
    OM_TCM_STATUS,
    OM_SDMMC_STATUS,
    OM_BATTER_VOLT,
    OM_BATTER_TEMP,
    OM_OLED_TEMP,
    OM_DCXO_TEMP,
    OM_DCXO_TEMP_LT,
    OM_OLED_BUTT
};

typedef struct {
    OM_MSG_SOCP_HEAD_STRU   msgHead;
    VOS_UINT16        msgId; /* 消息ID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;
    VOS_UINT16        c1Value; /* C1值 */
    VOS_UINT16        c2Value; /* C2值 */
} CBT_SetC1C2ValueReq;

typedef struct {
    VOS_UINT32 count;        /* 要读取的NV项个数 */
    VOS_UINT16 nvItemId[2]; /* 要读取的NV项ID数组，其中个数为ulCount */
} CBT_ReadNvReq;

typedef struct {
    VOS_UINT32 errorCode;        /* 返回执行结果 */
    VOS_UINT32 errNvId;          /* 返回出现错误的NVID */
    VOS_UINT32 count;            /* 返回的NV项个数 */
    VOS_UINT16 nvItemData[2]; /* 包括NVID值、NVID内容的长度、NVID的内容 */
} CBT_ReadNvInd;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU   msgHead;
    VOS_UINT16        msgId; /* 消息ID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;

    VOS_UINT32 errorCode; /* 返回执行结果 */
    VOS_UINT32 errNvId;   /* 返回出现错误的NVID */
    VOS_UINT32 count;     /* 返回的NV项个数 */
} CBT_ReadNvCnf;

/*
 * 结构说明: 存储NV到FLASH命令响应包结构
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHead;
    VOS_UINT16        msgId; /* 消息ID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;
    VOS_UINT32        errorCode; /* 返回执行结果 */
} CBT_NvToFlashCnf;

#define CBT_EST_IND_RSV_LEN 212

typedef struct {
    VOS_UINT32     result; /* 返回执行结果 */
    VOS_UINT16     chipId;
    VOS_UINT16     rsv;
    msw_ver_info_s swverInfo;
    VOS_UINT8      reserve[CBT_EST_IND_RSV_LEN];
} CBT_EstablishIndPara;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHeader;
    VOS_UINT16           msgId; /* 消息ID */
    COMPONENT_ID_STRU    compMode;
    VOS_UINT32           msgLength;
    CBT_EstablishIndPara msgData;
} CBT_EstablishInd;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHeader;
    VOS_UINT16        msgId; /* 消息ID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;
    VOS_UINT32        linkType; /* 0表示HDLC, 1表示usb */
    VOS_UINT32        rsv[4];
} CBT_EstablishReq;



#endif /* end of __PC_NSM_INTERFACE_H__ */

