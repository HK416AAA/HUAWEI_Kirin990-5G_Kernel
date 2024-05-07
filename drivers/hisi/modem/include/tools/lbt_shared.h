
#ifndef __LBT_SHARED_H__
#define __LBT_SHARED_H__

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#ifndef RX_ANT_NUM_MAX
#define RX_ANT_NUM_MAX 8
#endif
#define LBT_SYNC_MAX_UL_CC_NUM 2
#define LBT_SYNC_MAX_DL_CC_NUM 5

typedef struct {
    VOS_UINT16      ratMode;        /* ���庬��μ�LRRC_LPHY_RatTypeUint16 */
    VOS_UINT16      bandNo;         /*���庬��μ� LRRC_LPHY_BandIndUint16 */
    VOS_UINT8       antNum;
    VOS_UINT8       bandWidth;      /* ���庬��μ� LRRC_LPHY_BandWidthUint16 */
    VOS_UINT16      rsv;
    VOS_UINT32      freqKHz;
}LBT_BandInfo;

typedef struct {
    VOS_UINT16      duplexType;         /* ���庬��μ� LRRC_LPHY_LteModeTypeUint16 */
    VOS_UINT16      bsTxAntPort;        /* ���庬��μ� LRRC_LPHY_AntannaPortUint16 */
    VOS_UINT16      tddUpDownAssign;    /* ���庬��μ� LRRC_LPHY_SubframeAssignmentUint16 */
    VOS_UINT16      tddSpecSfPattern;   /* ���庬��μ� LRRC_LPHY_SpecialSubframePatternUint16 */
    VOS_UINT16      cellId;
    VOS_UINT16      crnti;
    VOS_UINT16      cpType;
    VOS_UINT16      dmrs;
    VOS_UINT16      pa;             /* ���庬��μ� LRRC_LPHY_PdschPaUint16 */
    VOS_UINT16      pb;             /* ���庬��μ� LRRC_LPHY_PdschPbUint16 */
    VOS_UINT16      phichResource;  /* ���庬��μ� LRRC_LPHY_PhichResourceUint16 */
    VOS_UINT16      phichDuration;  /* ���庬��μ� LRRC_LPHY_PhichDurationUint16 */
    VOS_UINT16      pdschTmMode;    /* ���庬��μ� LRRC_LPHY_PdschTmModeUint16 */
    VOS_UINT16      rsv;
}LBT_SyncCcCommPara;

typedef struct {
    LBT_BandInfo                bandInfo;
    LBT_SyncCcCommPara          commPara;
}LBT_SyncCcPara;

/* LTE CALL ����Ľṹ�� */
typedef struct {
    VOS_UINT16                  testMode;           /* ���庬��μ� NBT_TestModeUint16 */
    VOS_UINT16                  listenFlag;
    VOS_UINT8                   ulCcIndex;
    VOS_UINT8                   dlCcIndex;
    VOS_UINT8                   ulBandNum;          /* ����Band���� */
    VOS_UINT8                   dlBandNum;          /* ����Band���� */
    LBT_SyncCcPara          ulCcConfig[LBT_SYNC_MAX_UL_CC_NUM];      /* CC ����ͬ������ */
    LBT_SyncCcPara          dlCcConfig[LBT_SYNC_MAX_DL_CC_NUM];      /* CC ����ͬ������ */
}LBT_TASK_SyncPara;


/* ��;:�����������й��� */
typedef struct {
    VOS_UINT16  OnOff;          /* �Ƿ������:1-�򿪣�0-�ر� */
    VOS_UINT16  tpcOnOff;       /* �Ƿ���ӦTPC��0-����Ӧ��1-��Ӧ����TPC */
    VOS_INT16   targetPower;
    VOS_UINT16  mprDbbVal;
    VOS_UINT16  modeType;
    VOS_UINT16  rbPos;
    VOS_UINT16  rbNum;
    VOS_UINT16  channelType;
    VOS_UINT16  antMap;
    VOS_UINT16  tasMasStatus;
    /* ��ʾTx������ÿ��segment�ĳ���ʱ�䣬��λΪms����ʾ��֡����
     * ���ֵ<=0����ʾһֱά�֣���������Ӧ��ֻ���������ٳ����������һ��segment
     */
    VOS_INT16   duration;
    VOS_UINT16  rsv;
} LBT_TxSegPara;

/* LTE TX��������Ľṹ�� */
/*lint -save -e43 */
typedef struct {
    VOS_UINT32 segmentCnt; /* �Ӷθ��� */
    LBT_TxSegPara data[0]; /* �±�̹淶 �䳤���� 0 */
} LBT_TASK_TxPara;
/*lint -restore */

/* LTE BLER��������Ľṹ�� */
typedef struct {
    /* ����Ŀ�ʼʱ��Ͳ���������ʱ��֮���ƫ��Slot
     * listmode�����¿����л�Ƶ�㣬��Ϊ�Ǳ��UE���л���ʱ���ź��ȶ���Ҫʱ��
     * ͨ�������˲������Ա�֤���ȶ�״̬�²���
     * ���ٲ���ʱ����ֱ������Ϊ0
     */
    VOS_UINT16 offsetSlot;
    VOS_UINT16 minSlot;     /* �������ٵ�Slot */
    VOS_UINT16 maxSlot;     /* �������Slot */
    VOS_UINT16 antMap;      /* ָ���������� */
    VOS_UINT16 dlRatio;     /* �����ź���û֡�����е���Ч��֡���� */
    VOS_UINT16 rsv;
} LBT_TASK_BlerPara;

/* ��;:LTE Bler����ϱ��ṹ */
typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;
    VOS_UINT32 rxChan;
    VOS_UINT32 errorBlock[RX_ANT_NUM_MAX];  /* ����block */
    VOS_UINT32 totalBlock[RX_ANT_NUM_MAX];  /* �ܵ�block */
    VOS_INT32  rssi[RX_ANT_NUM_MAX];        /* ��λRX level 0.125dBm */
    VOS_INT32  snr[RX_ANT_NUM_MAX];         /* ��λRX Snr  0.125dBm */
    VOS_INT32  rscp[RX_ANT_NUM_MAX];        /* ��λRX Rscp  0.125dBm */
} LBT_TASK_BlerInd;

/* LTE RSSI��������Ľṹ�� */
typedef struct {
    VOS_UINT16 offsetSlot;  /* ����ʱƫ�Ƶ�Slot */
    VOS_UINT16 slotNum;
    VOS_UINT16 antMap;      /* ָ���������� */
    VOS_UINT16 rsv;
} LBT_TASK_RssiPara;

/* ��;:LTE Rssi����ϱ��ṹ */
typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;
    VOS_UINT32 rxChan;
    VOS_INT32  rssi[RX_ANT_NUM_MAX]; /* ��λRX level 0.125dBm */
} LBT_TASK_RssiInd;

/* LTE-V �������ù��ʲ����Ľӿ� */
typedef struct {
    VOS_UINT16  OnOff;          /* �Ƿ������:1-�򿪣�0-�ر� */
    VOS_UINT16  tpcOnOff;       /* �Ƿ���ӦTPC��0-����Ӧ��1-��Ӧ����TPC */
    VOS_INT16   targetPower;    /* ��λ1/8dBm */
    VOS_UINT16  mprDbbVal;      /* ��λ1/8dBm */
    VOS_UINT16  antMap;         /* ֻ֧��2T */
    VOS_UINT16  rsv;
} NBT_VPHY_TxPowerPara;



#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

