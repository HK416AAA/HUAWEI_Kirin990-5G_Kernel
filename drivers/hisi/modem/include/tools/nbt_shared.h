
#ifndef __NBT_SHARED_H__
#define __NBT_SHARED_H__

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#ifndef NBT_HF_MAX_ANTNUM
#define NBT_HF_MAX_ANTNUM             4
#endif
#ifndef NBT_HF_MAX_PASNUM
#define NBT_HF_MAX_PASNUM             2
#endif

enum NBT_TOOL_MsgTypeEnum {
    /* msg id for pc to ue */
    ID_TOOL_NBT_LISTMODE_REQ            = 0x1001,
    ID_TOOL_NBT_SLOT_CFG_REQ            = 0x1002,
    ID_TOOL_NBT_TRXTAS_REQ              = 0x109A, /**< @sa NBT_ISO_TrxTasReqStru */
    ID_TOOL_NBT_SRS_REQ                 = 0x109B, /**< @sa NBT_ISO_TxSrsReqStru */
    ID_TOOL_NBT_FR2_ANT_CFG_REQ         = 0x109C, /**< @sa NBT_ISO_NrFr2AntCfgReqStru */
    /* msg id for ue response */
    ID_NBT_TOOL_LISTMODE_CNF            = 0x2001,
    ID_TOOL_NBT_SLOT_CNF                = 0x2002,
    ID_NBT_TOOL_TRXTAS_CNF              = 0x209A, /**< @sa NBT_ISO_TrxTasIndStru */
    ID_NBT_TOOL_SRS_CNF                 = 0x209B, /**< @sa NBT_ISO_TxSrsIndStru */
    ID_NBT_TOOL_FR2_ANT_CFG_CNF         = 0x209C, /**< @sa NBT_ISO_NrFr2AntCfgCnfStru */
    ID_NBT_TOOL_RX_BLER_REPORT_IND      = 0x3001,
    ID_NBT_TOOL_RX_RSSI_REPORT_IND      = 0x3002,

};
typedef VOS_UINT16 NBT_TOOL_MsgTypeEnumU16;

/* ��;:NRBT������TRX TAS REQ���� */
typedef struct {
    VOS_UINT8  cmd; /* < 1 - ���ò�����3 - ��ѯ״̬ */
    VOS_UINT8  rsv[3];
    VOS_UINT32 reqValue; /* < bit[3:4] - UE����,bit[2] - TX������bit[0:1] - ����port�� */
} NBT_ISO_TrxTasReqParaStru;

/* ��;:NRBT��TRX TAS IND���� */
typedef struct {
    VOS_UINT32 errorCode; /* < < �����0��ʾ�ɹ���������ʾʧ�� */
    VOS_UINT32 indValue;  /* < bit[4:5] - UE����,bit[2:3] - TX0����port��,bit[0:1] - TX1����port�� */
} NBT_ISO_TrxTasIndParaStru;

/* NRBT��TDD SLOT CFG PARA�ṹ */
typedef struct {
    VOS_UINT16 upLinkSymbolEnable; /* xslot����symbol�Ƿ���ʹ�� */
    VOS_UINT8  tddUlDlPeriod;      /* period */
    VOS_UINT8  rsv;
    VOS_UINT16 downLinkSlotNum;   /* downlink slot num */
    VOS_UINT16 downLinkSymbolNum; /* downlink symbol num [0-14] */
    VOS_UINT16 upLinkSlotNum;     /* uplink slot num */
    VOS_UINT16 upLinkSymbolNum;   /* uplink symbol num [0-14] */
} NBT_ISO_TddBandSlotCfgPara;

typedef NBT_ISO_TrxTasReqParaStru NBT_ISO_TxSrsReqParaStru;
typedef NBT_ISO_TrxTasIndParaStru NBT_ISO_TxSrsIndParaStru;

/*
 * NRBT��ÿ�����ߵ�HF ANT CFG���ýṹ
 */
typedef struct {
    VOS_UINT32 initBeamIndex : 8; /* ��ʼbeam���� */
    VOS_UINT32 antArray      : 4; /* �������� */
    VOS_UINT32 antType       : 1; /* boardside/endfire */
    VOS_UINT32 polarType     : 1; /* horizontal/vertical */
    VOS_UINT32 enableBeamCfg : 1; /* beamʹ�ܣ�initBeamIndex��Ч */
    VOS_UINT32 rsv           : 17;/* ���� */
} NbtNrFr2AntCfg;

/*
 * NRBT��HF ANT CFG PARA�ṹ
 */
typedef struct {
    VOS_UINT16     ulAntNum;       /* �������߸��� */
    VOS_UINT16     dlAntNum;       /* �������߸��� */
    NbtNrFr2AntCfg ulCfg[NBT_HF_MAX_ANTNUM];
    NbtNrFr2AntCfg dlCfg[NBT_HF_MAX_ANTNUM];
    VOS_UINT16     ulChsStatus;      /* 0-chsֱͨ��1-chs���� */
    VOS_UINT16     dlChsStatus;      /* 0-chsֱͨ��1-chs���� */
    VOS_UINT8      ulPasStatus[NBT_HF_MAX_PASNUM];   /* 0-pasֱͨ��1-pas���� */
    VOS_UINT8      dlPasStatus[NBT_HF_MAX_PASNUM];   /* 0-pasֱͨ��1-pas���� */
} NbtNrFr2AntCfgReqPara;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

