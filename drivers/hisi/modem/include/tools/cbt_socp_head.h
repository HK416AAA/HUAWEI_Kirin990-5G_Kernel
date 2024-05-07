
#ifndef CBT_SOCP_HEAD_H
#define CBT_SOCP_HEAD_H

#include "vos.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#pragma pack(push, 4)


#define CBT_MSG_HEADER_LENGTH   16U
#define CBT_MSG_IDLEN_LENGTH    8U
#define CBT_MSG_HEAD_EX_LENGTH  (CBT_MSG_HEADER_LENGTH + CBT_MSG_IDLEN_LENGTH)

typedef enum {
    CBT_COMP_FUNC = 1,
    CBT_COMP_NOSIG,
    CBT_COMP_PHY,
    CBT_COMP_PS,
    CBT_COMP_TRANS,
    CBT_COMP_CAL,
    CBT_COMP_BUTT
} CBT_ComponetId;

/* ComponentTypeʹ�� */
typedef enum {
    CBT_MODE_LTE = 0,
    CBT_MODE_TDS,
    CBT_MODE_GSM,
    CBT_MODE_UMTS,
    CBT_MODE_CDMA,
    CBT_MODE_NR,
    CBT_MODE_LTEV2X = 7,
    CBT_MODE_BUTT,
    CBT_MODE_COMM   = 0xf,
} CBT_ComponentMode;

typedef enum {
    CBT_MT_REQ = 1,
    CBT_MT_CNF,
    CBT_MT_IND,
    CBT_MT_BUTT
} CBT_MsgType;

typedef enum {
    CBT_SSID_NOAPPOINT = 0,
    CBT_SSID_APP_CPU,
    CBT_SSID_MODEM_CPU,
    CBT_SSID_LTE_DSP,
    CBT_SSID_LTE_BBP,
    CBT_SSID_GU_DSP,
    CBT_SSID_HIFI,
    CBT_SSID_TDS_DSP,
    CBT_SSID_TDS_BBP,
    CBT_SSID_MCU,
    CBT_SSID_GPU,
    CBT_SSID_GU_BBP,
    CBT_SSID_IOM3,
    CBT_SSID_ISP,
    CBT_SSID_X_DSP,
    CBT_SSID_RESERVE,
    CBT_SSID_BUTT
} CBT_Ssid;

typedef struct {
    UINT8 modem : 3;  /* modem0: 0  modem1: 1 */ 
    UINT8 resv : 1;
    UINT8 ssid : 4;
} ModemSsid;

typedef struct {
    UINT8 msgType : 2;        /* ��Ϣ���ͣ�REQ: 1   CNF: 2   IND:3 */ 
    UINT8 fragmentIndex : 4;  /* ��Ϣ�ֶε�Ƭ������,0...15 */ 
    UINT8 eof : 1;            /* �ֶν�����ʶ��0���ֶ�δ������1���ֶν��� */ 
    UINT8 fragmentFlag : 1;   /* �Ƿ�ֶα�ʶ��0���ֶΣ�1�ֶ� */ 
} MsgFragmentInfo;

typedef struct {
    UINT8 ucRsv;
    UINT8 ucMode : 4;
    UINT8 ucCompId : 4;
} OM_COMPONENT_MODE_STRU;

/*
 * �������ʱ��������ݽṹ
 */ 
typedef struct {
    UINT32 ulTimestampL;
    UINT16 usTimestampH;
    UINT16 usRsv;
} TimeStamp;

typedef struct _OM_Msg_Socp_Head_Stru {
    UINT8 sid;  /* �̶�Ϊ7 */ 
    ModemSsid modemSsid;
    UINT8 sessionID;  /* �̶�Ϊ1 */ 
    MsgFragmentInfo msgSegment;
    UINT32 transId;
    TimeStamp timeStamp;
} OM_MSG_SOCP_HEAD_STRU;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif