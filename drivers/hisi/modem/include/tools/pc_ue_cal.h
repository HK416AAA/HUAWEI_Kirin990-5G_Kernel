
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
    TEMP_LOGIC_CHAN_XO_TEMP = 17, /* < ��ȡ�����¶� */
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
 * �ṹ˵��: UECBT��Ϣ��ʽ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16            msgId;
    COMPONENT_ID_STRU     componentId;
    VOS_UINT32            msgLength;
    VOS_UINT8             para[0]; /* ��Ϣ���� */
} UECBT_UniformMsg;

/*
 * �ṹ˵��: UECBT��Ϣ��ʽ
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
 * Description: PC->UE��������REQ��Ϣ
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
 * Description : PC->UE�ͷ�����REQ��Ϣ
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
 * Description: UE->PC��������CNF��Ϣ
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
 * Description: UE->PC�ͷ�����CNF��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;
    UINT16                 usMsgId;
    OM_COMPONENT_MODE_STRU stCompMode;
    UINT32                 ulMsgLength;
    UINT32                 ulErrorCode;
} PC_UE_RELEASE_CNF_STRU;


/*
 * Description: PC->UE�л�������REQ��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulMode;
} PC_UE_SET_FTM_MODE_REQ_STRU;

/*
 * Description: UE->PC�л�������CNF��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
} PC_UE_SET_FTM_MODE_CNF_STRU;


typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;   /* ��ԭ�ṹ��ԭ����һ�£���Ϣͷ */
    UINT16                 usMsgId;       /* ��ԭ�ṹ��ԭ����һ�£���ϢID */
    COMPONENT_ID_STRU      stComponentId; /* ��ԭ�ṹ��ԭ����һ�£�
                                             ��4bitΪ��������ID��LOADPHY��Ҫ��Ϊ1 */
    UINT32            ulDataLength;       /* ��ԭ�ṹ��ԭ����һ�£������ĳ��� */
    LOADPHY_PARA_STRU stLoadPhyPara;      /* LOADPHY���� */
} PC_UECBT_LOAD_PHY_REQ_STRU;

/*
 * Description: UECBT -> PC LOADPHY CNF��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU  stMsgHeader;   /* ��ԭ�ṹ��ԭ����һ�£���Ϣͷ */
    UINT16                 usMsgId;       /* ��ԭ�ṹ��ԭ����һ�£���ϢID */
    COMPONENT_ID_STRU      stComponentId; /* ��ԭ�ṹ��ԭ����һ�£�
                                             ��4bitΪ��������ID��UECBT������д */
    UINT32 ulDataLength;                  /* ��ԭ�ṹ��ԭ����һ�£������ĳ��� */
    UINT32 ulErrorCode;                   /* ���: 0:SUCC
               1��2��3 ..... ����ʧ����� */
} UECBT_PC_LOAD_PHY_CNF_STRU;

/*
 * Description: HIRF->UE���������REQ��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulLength;
    RAT_MODE_ENUM_UINT16       enRatMode;
    UINT8                      ucWorkMode; /* 0: ��ģ������ 1: ˫ģ����(L+NRͬʱ��) */
    UINT8                      ucBusiness; /* ����������Ķ���ͳһ */
} HIRF_UE_LOAD_PHY_REQ_STRU;

/*
 * Description: UE->HIRF���������Cnf��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulLength;
    UINT32                     ulErrorCode; /* 0��ʾ�ɹ���N(1,2,3...)��ʾ��ͬ���� */
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
 * Description: UE->PC���¼��������CNF��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
} PC_UE_RELOAD_NV_CNF_STRU;

/*
 * Description: PC->UE��ѯ�¶Ƚӿڣ�����Ϊ�߼�ͨ��
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU       stMsgHeader;
    UINT16                      usMsgId; /* 0x80C1 */
    COMPONENT_ID_STRU           stComponentId;
    UINT32                      ulLength;
    TEMP_LOGIC_CHAN_ENUM_UINT32 ulType; /* �߼�ͨ�� */
} PC_UE_QUERY_TEMP_LOGIC_CHAN_REQ_STRU;

/*
 * Description: UE->PC�����¶Ƚ��
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
    INT32                 lValue; /* ��� */
} PC_UE_QUERY_TEMP_LOGIC_CHAN_IND_STRU;

/*
 * Description: ָ��ĳ��path��NV���¼���
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU           stMsgHeader;
    UINT16                          usMsgId;
    COMPONENT_ID_STRU               stComponentId;
    UINT32                          ulMsgLen;
    BBIC_CAL_NV_RESET_REQ_PARA_STRU stPara;
} PC_UE_NV_RESET_REQ_STRU;

/*
 * Description: UE->PC����NV reset���
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                ulErrorCode;
} PC_UE_NV_RESET_CNF_STRU;

/*
 * Description: PC->UEдNV����
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                count;
    UINT16                data[0];  // ��Ҫд��nv���ݣ�����nvid,len,data����
} PC_UE_NV_WRITE_REQ_STRU;

/*
 * �ṹ˵��: дNV���
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU socpHeader;
    VOS_UINT16            msgId; /* ��ϢID */
    COMPONENT_ID_STRU     componmentInfo;
    VOS_UINT32            msgLength; /* ����һ�ֽڵ���Ϣβ�ĳ��ȣ���λbtye */
    VOS_UINT32            errorCode; /* ����ִ�н�� */
    VOS_UINT32            errorNvId; /* ���س��ִ����NVID */
} UE_PC_NV_WRITE_CNF_STRU;

/*
 * �ṹ˵��: дNVˢflash����
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU stMsgHeader;
    UINT16                usMsgId;
    COMPONENT_ID_STRU     stComponentId;
    UINT32                ulLength;
    UINT32                count;
    UINT16                data[0];  // ��Ҫд��nv���ݣ�����nvid,len,data����
} PC_UE_WRITE_NV_FLASH_REQ_STRU;

/*
 * �ṹ˵��: дNVˢflash���
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU socpHeader;
    VOS_UINT16            msgId; /* ��ϢID */
    COMPONENT_ID_STRU     componmentInfo;
    VOS_UINT32            msgLength; /* ����һ�ֽڵ���Ϣβ�ĳ��ȣ���λbtye */
    VOS_UINT32            errorCode; /* ����ִ�н�� */
    VOS_UINT32            errorNvId; /* ���س��ִ����NVID */
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
 * desc: ��ƵRx ADCУ׼�ӿڶ���
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU          msgHeader;
    BBIC_CAL_COMPONENT_ID_STRU     componentId;
    UINT32                         msgLen;
    BBIC_CAL_RficHfRxAdcCalReqPara rxAdcPara;
} PC_UE_RficHfRxAdcSelfCalReq;

/*
 * desc: ��ƵTRx FDIQУ׼�ӿڶ���
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

/* ��;:NRBT �����ò��������Ⱥ��Ӷο����� */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     ulSegmengCnt; /* �ֶθ��� */
    UINT8                      aucData[0];   /* �䳤�������ʽ�μ�PC_UE_NRBT_SEGMENT_STRU */
} PC_UE_NRBT_TEST_REQ_STRU;

/* ��;:NRBT �����ò��������Ⱥ��Ӷο����� */
typedef struct {
    UINT32 ulTaskName;     /* �������ƣ�BT_TASK_NAME_ENUM */
    UINT32 ulDuration;     /* ����ʱ�䣬��λms */
    UINT32 ulTaskDataLen;  /* ���񳤶� */
    UINT8  aucDataBuff[0]; /* ���ݰ��ձ䳤���� */
} PC_UE_NRBT_SEGMENT_STRU;

/* ��;:NRBT CC���ò��� */
typedef struct {
    UINT16 usBandNo;    /* BAND��Ϣ */
    UINT8  ucAntNum;    /* ���������� */
    UINT8  ucBandWidth; /* BAND��� */
    UINT32 ulFreqKHz;   /* Ƶ����Ϣ */
} PC_UE_NRBT_CC_INFO_STRU;

/* ��;:BT CALLʱ�����ò��� */
typedef struct {
    NBT_TestModeUint16      testMode;
    UINT8                   ulCcIndex;
    UINT8                   dlCcIndex;
    UINT16                  usUlBandNum;
    UINT16                  usDlBandNum;
    PC_UE_NRBT_CC_INFO_STRU astUlCCComfig[NBT_MAX_UL_CC_NUM]; /* ����CC������Ϣ */
    PC_UE_NRBT_CC_INFO_STRU astDlCCComfig[NBT_MAX_DL_CC_NUM]; /* ����CC������Ϣ */
} PC_UE_NRBT_CC_CFG_STRU;

/* ��;:BT CALLʱ�����ò��� */
typedef struct {
    PC_UE_NRBT_CC_CFG_STRU stCcCfg;         /* CC���ã���ǰֻ֧�ֵ�CC */
    UINT16                 usSsbScs;        /* SSB���ز������ö�ٶ��� */
    UINT16                 usCommScs;       /* COMM���ز������ö�ٶ��� */
    INT16                  sRssi8th;        /* �����ź�ǿ�ȣ���λ1/8dBm */
    INT16                  sDuration;       /* ����ʱ�䣬��֡������0��ʾ�����л���-1��ʾ���Ǵ��������л� */
    UINT16                 usSlotFormat;    /* ������ʱ϶��ʽ */
    UINT16                 usCpType;        /* Ncp/Ecp */
    UINT16                 usDmrs;          /* DMRS���� */
    UINT16                 usSsbFreqOffset; /* SSBƵ��ƫ���� */
} PC_UE_NRBT_CALL_CONFIG_STRU;

/* ��;:Rx BLER����ʱ�����ò����ṹ */
typedef struct {
    UINT16 usOffsetSlot; /* ����ʱƫ�Ƶ�Slot */
    UINT16 usMinSlot;    /* �������ٵ�Slot */
    UINT16 usMaxSlot;    /* �������Slot */
    UINT16 usTestAntMap; /* ָ���������� */
    UINT16 usSlotNum;    /* ֡��� */
    UINT16 usRsv;
} PC_UE_NRBT_RX_BLER_TEST_REQ_STRU;

/* ��;:Rx Bler����ϱ��ṹ */
typedef struct {
    UINT16 usBandIndex;
    UINT16 usRsv;
    UINT32 ulRxChan;
    UINT32 ulErrBlock[RX_ANT_NUM_MAX];   /* ����block */
    UINT32 ulTotalBlock[RX_ANT_NUM_MAX]; /* �ܵ�block */
    INT32  lRxRssi[RX_ANT_NUM_MAX];      /* ��λRX level 0.125dBm */
    INT32  lRxSnr[RX_ANT_NUM_MAX];       /* ��λRX Snr  0.125dBm */
    INT32  lRxRscp[RX_ANT_NUM_MAX];      /* ��λRX Rscp  0.125dBm */
} PC_UE_NRBT_RX_BLER_REPORT_IND_STRU;

/* ��;:Rx RSSI����ʱ�����ò����ṹ */
typedef struct {
    UINT16 usOffsetSlot; /* ����ʱƫ�Ƶ�Slot */
    UINT16 usSlotNum;
    UINT16 usTestAntMap; /* ָ���������� */
    UINT16 usRsv;
} PC_UE_NRBT_RX_RSSI_TEST_REQ_STRU;

/* ��;:Rx Rssi����ϱ��ṹ */
typedef struct {
    UINT16 usBandIndex;
    UINT16 usRsv;
    UINT32 ulRxChan;
    INT32  lRxRssi[RX_ANT_NUM_MAX]; /* ��λRX level 0.125dBm */
} PC_UE_NRBT_RX_RSSI_REPORT_IND_STRU;

/* ��;:Rx����ϱ��ܽṹ */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     ulSegmentIndex;
    UINT8                      aucData[4];
} PC_UE_NRBT_TEST_IND_STRU;

/* ��;:TX�����ò����ܽṹ */
typedef struct {
    UINT32 ulSegmentCnt; /* �ֶθ��� */
    UINT8  aucData[0];   /* ���ݱ䳤 */
} PC_UE_NRBT_SET_TX_PWR_REQ_STRU;

/* ��;:�����������й��� */
typedef struct {
    UINT16 usTxOnOff;  /* �Ƿ������:1-�򿪣�0-�ر� */
    UINT16 usTpcOnOff; /* �Ƿ���ӦTPC��0-����Ӧ��1-��Ӧ����TPC */
    INT16  sPower;     /* Tpc Off����Ч��ǿ���Ŀ�깦�� */
    UINT16 mprVal;     /* MPR����ֵ */
    UINT16 usModuType; /* ���Ʒ�ʽ */
    UINT16 usOfmdType; /* ���ϸ�ʽ��cp-ofdm����dtf-s-ofdm */
    UINT16 usRbPos;
    UINT16 usRbNum;
    UINT16 numOfLayer; /* ������������numһ�� */
    UINT16 antNum;
    UINT16 tpmi;      /* �ο�Э��38.211 6.3.1.5 */
    INT16  sDuration; /* ����ʱ�䣬��֡������<=0��ʾһֱά�� */
} PC_UE_NRBT_SET_TX_PWR_STRU;

/* ��;:hold task sub struct */
typedef struct {
    UINT32  holdStatus; /* hold״̬,1 - hold��0 - cancel */
} PCUE_NRBT_TaskHold;

/* ��;:�ظ�PC��NR BT��Cnf��Ϣ�ṹ */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     ulSegmengIndex; /* �쳣�ӶΣ����쳣��0 */
    UINT32                     ulErrorCode;    /* �����룬0-��������0�����ulSegmengIndex */
} PC_UE_NRBT_TEST_CNF_STRU;

/* ��;: NR FR2�������۲����TX beam */
typedef struct {
    UINT16    antBitmap;    /* �������� */
    UINT16    rsv;
    UINT8     beamIndex[NBT_HF_MAX_BEAMNUM]; /* ÿ�����߶�Ӧ��beam���򣬽�����ʽ�洢 */
} PCUE_NRBT_TaskFr2TxAntCfg;

/* ��;:NRBT������TRX TAS��������Ҫ�ڽ���D̬���·� */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT8                      cmd; /* < 1 - ���ò�����3 - ��ѯ״̬ */
    UINT8                      rsv[3];
    UINT32                     trxTasValue; /* < bit[3:4] - UE����,bit[2] - TX������bit[0:1] - ����port�� */
} PCUE_NRBT_TrxTasReqStru;

/*
 * Description:ֻ�е�PCUE_NRBT_TrxTasReqStru��cmdΪ��ѯʱ���Ż��ϱ�ind��Ϣ
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     errorCode; /* < < �����0��ʾ�ɹ���������ʾʧ�� */
    UINT32                     trxTasValue;
} PCUE_NRBT_TrxTasIndStru;

/* ��;:NRBT��ǿ��SRS״̬����Ҫ������֮ǰ�·���SRSǿ���RX����Ҳ���л� */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT8                      cmd; /* < 1 - ���ò�����3 - ��ѯ״̬ */
    UINT8                      rsv[3];
    UINT32                     srsValue; /* < bit[3:4] - UE����, bit[2] - TX������bit[0:1] - ����port�� */
} PCUE_NRBT_TxSrsReqStru;

/* ��;:ֻ��PCUE_NRBT_TxSrsReqStru��cmdΪ��ѯʱ����ind��Ϣ�ϱ� */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU      stMsgHeader;
    BBIC_CAL_COMPONENT_ID_STRU stComponentId;
    UINT32                     ulMsgLen;
    UINT32                     errorCode; /* < < �����0��ʾ�ɹ���������ʾʧ�� */
    UINT32                     srsValue;
} PCUE_NRBT_TxSrsIndStru;

/*
 * ��;: NR FR2�������۲���Ҫ��ǰ����ant״̬
 */
typedef struct
{
    OM_MSG_SOCP_HEAD_STRU       header;
    BBIC_CAL_COMPONENT_ID_STRU  componentId;
    UINT32                      msgLen;  /* ��һ�ֽڵ���Ϣβ�ĳ��ȣ���λbyte */
    NbtNrFr2AntCfgReqPara       data;
} PCUE_NRBT_Fr2AntCfgReq;

/*
 *��;: NR FR2�������۲�����ant״̬��response
 */
typedef struct
{
    OM_MSG_SOCP_HEAD_STRU       header;
    BBIC_CAL_COMPONENT_ID_STRU  componentId;
    UINT32                      msgLen;   /* ��һ�ֽڵ���Ϣβ�ĳ��ȣ���λbyte */
    UINT32                      errorCode;
} PCUE_NRBT_Fr2AntCfgCnf;

/*
 * ��;: NR TX TAS����˫��˫PORT�ĳ���ÿT�Ĺ��ʻ���ֵ
 */
typedef struct
{
    UINT16                      txSarBackPower[NBT_TX_ANT_NUM_MAX]; /* TX��Sar����ÿT�Ĺ��ʻ���ֵ,��λ1/8db */
} PCUE_NRBT_TxSarCfgPara;

#define LTE_BT_STRU_BEGIN

typedef PC_UE_NRBT_TEST_REQ_STRU PC_UE_BtLnrTestReq;
typedef PC_UE_NRBT_SEGMENT_STRU PC_UE_BtLnrSegment;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

