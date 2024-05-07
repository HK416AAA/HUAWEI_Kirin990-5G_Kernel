
#ifndef __PC_NSM_INTERFACE_H__
#define __PC_NSM_INTERFACE_H__

#include "mdrv.h"

enum PC_NSM_MsgId {
    /* OM<->APP : Release link */
    APP_OM_RELEASE_REQ = 0x80b3,
    OM_APP_RELEASE_CNF = 0x80b4,

    /* ���ô��������л����������� */
    APP_OM_SET_FTM_REQ = 0x80e5,
    OM_APP_SET_FTM_CNF = 0x80e6,

    /* OM<->APP : LOAD PHY */
    APP_OM_LOADPHY_REQ = 0x81b1,
    OM_APP_LOADPHY_CNF = 0x81b2,

    /* OM<->APP : �����߼�ͨ����ѯ�¶� */
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


    /* OM<->APP :����C��NV�洢��Flash */
    APP_OM_NV_TO_FLASH_REQ = 0x802B,
    OM_APP_NV_TO_FLASH_CNF = 0x802C,

    /* ��CT�����µ�дnv��Ϣid */
    TOOL_UE_WRITE_NV_REQ = 0x9023,
    UE_TOOL_WRITE_NV_CNF = 0x9024,
    TOOL_UE_NV_WRITE_FLASH_REQ = 0x902D,
    UE_TOOL_NV_WRITE_FLASH_CNF = 0x902E,

    /* OM<->APP :����C��һ����дNV�洢Flash */
    APP_OM_NV_WRITE_FLASH_REQ = 0x802D,
    OM_APP_NV_WRITE_FLASH_CNF = 0x802E,

    /* OM<->APP : RELOAD NV */
    APP_OM_RELOADNV_REQ = 0x81b3,
    OM_APP_RELOADNV_CNF = 0x81b4,

    /* OM<->APP :LMT��ѯDSDA֧��״̬ */
    APP_OM_QUERY_MODEM_NUM_REQ = 0x8211,
    OM_APP_QUERY_MODEM_NUM_CNF = 0x8212,

    /* OM<->APP :ͨ������ͨ����ѯPA���¶� */
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
    VOS_UINT16        msgId; /* ��ϢID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;
    VOS_UINT16        c1Value; /* C1ֵ */
    VOS_UINT16        c2Value; /* C2ֵ */
} CBT_SetC1C2ValueReq;

typedef struct {
    VOS_UINT32 count;        /* Ҫ��ȡ��NV����� */
    VOS_UINT16 nvItemId[2]; /* Ҫ��ȡ��NV��ID���飬���и���ΪulCount */
} CBT_ReadNvReq;

typedef struct {
    VOS_UINT32 errorCode;        /* ����ִ�н�� */
    VOS_UINT32 errNvId;          /* ���س��ִ����NVID */
    VOS_UINT32 count;            /* ���ص�NV����� */
    VOS_UINT16 nvItemData[2]; /* ����NVIDֵ��NVID���ݵĳ��ȡ�NVID������ */
} CBT_ReadNvInd;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU   msgHead;
    VOS_UINT16        msgId; /* ��ϢID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;

    VOS_UINT32 errorCode; /* ����ִ�н�� */
    VOS_UINT32 errNvId;   /* ���س��ִ����NVID */
    VOS_UINT32 count;     /* ���ص�NV����� */
} CBT_ReadNvCnf;

/*
 * �ṹ˵��: �洢NV��FLASH������Ӧ���ṹ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHead;
    VOS_UINT16        msgId; /* ��ϢID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;
    VOS_UINT32        errorCode; /* ����ִ�н�� */
} CBT_NvToFlashCnf;

#define CBT_EST_IND_RSV_LEN 212

typedef struct {
    VOS_UINT32     result; /* ����ִ�н�� */
    VOS_UINT16     chipId;
    VOS_UINT16     rsv;
    msw_ver_info_s swverInfo;
    VOS_UINT8      reserve[CBT_EST_IND_RSV_LEN];
} CBT_EstablishIndPara;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHeader;
    VOS_UINT16           msgId; /* ��ϢID */
    COMPONENT_ID_STRU    compMode;
    VOS_UINT32           msgLength;
    CBT_EstablishIndPara msgData;
} CBT_EstablishInd;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU       msgHeader;
    VOS_UINT16        msgId; /* ��ϢID */
    COMPONENT_ID_STRU compMode;
    VOS_UINT32        msgLength;
    VOS_UINT32        linkType; /* 0��ʾHDLC, 1��ʾusb */
    VOS_UINT32        rsv[4];
} CBT_EstablishReq;



#endif /* end of __PC_NSM_INTERFACE_H__ */

