
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
    VOS_UINT16      ratMode;        /* 具体含义参见LRRC_LPHY_RatTypeUint16 */
    VOS_UINT16      bandNo;         /*具体含义参见 LRRC_LPHY_BandIndUint16 */
    VOS_UINT8       antNum;
    VOS_UINT8       bandWidth;      /* 具体含义参见 LRRC_LPHY_BandWidthUint16 */
    VOS_UINT16      rsv;
    VOS_UINT32      freqKHz;
}LBT_BandInfo;

typedef struct {
    VOS_UINT16      duplexType;         /* 具体含义参见 LRRC_LPHY_LteModeTypeUint16 */
    VOS_UINT16      bsTxAntPort;        /* 具体含义参见 LRRC_LPHY_AntannaPortUint16 */
    VOS_UINT16      tddUpDownAssign;    /* 具体含义参见 LRRC_LPHY_SubframeAssignmentUint16 */
    VOS_UINT16      tddSpecSfPattern;   /* 具体含义参见 LRRC_LPHY_SpecialSubframePatternUint16 */
    VOS_UINT16      cellId;
    VOS_UINT16      crnti;
    VOS_UINT16      cpType;
    VOS_UINT16      dmrs;
    VOS_UINT16      pa;             /* 具体含义参见 LRRC_LPHY_PdschPaUint16 */
    VOS_UINT16      pb;             /* 具体含义参见 LRRC_LPHY_PdschPbUint16 */
    VOS_UINT16      phichResource;  /* 具体含义参见 LRRC_LPHY_PhichResourceUint16 */
    VOS_UINT16      phichDuration;  /* 具体含义参见 LRRC_LPHY_PhichDurationUint16 */
    VOS_UINT16      pdschTmMode;    /* 具体含义参见 LRRC_LPHY_PdschTmModeUint16 */
    VOS_UINT16      rsv;
}LBT_SyncCcCommPara;

typedef struct {
    LBT_BandInfo                bandInfo;
    LBT_SyncCcCommPara          commPara;
}LBT_SyncCcPara;

/* LTE CALL 任务的结构体 */
typedef struct {
    VOS_UINT16                  testMode;           /* 具体含义参见 NBT_TestModeUint16 */
    VOS_UINT16                  listenFlag;
    VOS_UINT8                   ulCcIndex;
    VOS_UINT8                   dlCcIndex;
    VOS_UINT8                   ulBandNum;          /* 上行Band个数 */
    VOS_UINT8                   dlBandNum;          /* 下行Band个数 */
    LBT_SyncCcPara          ulCcConfig[LBT_SYNC_MAX_UL_CC_NUM];      /* CC 上行同步参数 */
    LBT_SyncCcPara          dlCcConfig[LBT_SYNC_MAX_DL_CC_NUM];      /* CC 下行同步参数 */
}LBT_TASK_SyncPara;


/* 用途:用于配置上行功率 */
typedef struct {
    VOS_UINT16  OnOff;          /* 是否打开上行:1-打开，0-关闭 */
    VOS_UINT16  tpcOnOff;       /* 是否响应TPC，0-不响应，1-响应仪器TPC */
    VOS_INT16   targetPower;
    VOS_UINT16  mprDbbVal;
    VOS_UINT16  modeType;
    VOS_UINT16  rbPos;
    VOS_UINT16  rbNum;
    VOS_UINT16  channelType;
    VOS_UINT16  antMap;
    VOS_UINT16  tasMasStatus;
    /* 表示Tx测试是每个segment的持续时间，单位为ms，表示子帧个数
     * 如果值<=0，表示一直维持，这种配置应该只出现在慢速场景或者最后一个segment
     */
    VOS_INT16   duration;
    VOS_UINT16  rsv;
} LBT_TxSegPara;

/* LTE TX测试任务的结构体 */
/*lint -save -e43 */
typedef struct {
    VOS_UINT32 segmentCnt; /* 子段个数 */
    LBT_TxSegPara data[0]; /* 新编程规范 变长数据 0 */
} LBT_TASK_TxPara;
/*lint -restore */

/* LTE BLER测试任务的结构体 */
typedef struct {
    /* 任务的开始时间和测量的启动时间之间的偏移Slot
     * listmode场景下快速切换频点，因为仪表和UE的切换有时间差，信号稳定需要时间
     * 通过调整此参数可以保证在稳定状态下测量
     * 慢速测试时可以直接配置为0
     */
    VOS_UINT16 offsetSlot;
    VOS_UINT16 minSlot;     /* 测量最少的Slot */
    VOS_UINT16 maxSlot;     /* 测量最长的Slot */
    VOS_UINT16 antMap;      /* 指定测试天线 */
    VOS_UINT16 dlRatio;     /* 下行信号中没帧数据中的有效子帧个数 */
    VOS_UINT16 rsv;
} LBT_TASK_BlerPara;

/* 用途:LTE Bler结果上报结构 */
typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;
    VOS_UINT32 rxChan;
    VOS_UINT32 errorBlock[RX_ANT_NUM_MAX];  /* 误码block */
    VOS_UINT32 totalBlock[RX_ANT_NUM_MAX];  /* 总的block */
    VOS_INT32  rssi[RX_ANT_NUM_MAX];        /* 单位RX level 0.125dBm */
    VOS_INT32  snr[RX_ANT_NUM_MAX];         /* 单位RX Snr  0.125dBm */
    VOS_INT32  rscp[RX_ANT_NUM_MAX];        /* 单位RX Rscp  0.125dBm */
} LBT_TASK_BlerInd;

/* LTE RSSI测试任务的结构体 */
typedef struct {
    VOS_UINT16 offsetSlot;  /* 测量时偏移的Slot */
    VOS_UINT16 slotNum;
    VOS_UINT16 antMap;      /* 指定测试天线 */
    VOS_UINT16 rsv;
} LBT_TASK_RssiPara;

/* 用途:LTE Rssi结果上报结构 */
typedef struct {
    VOS_UINT16 band;
    VOS_UINT16 rsv;
    VOS_UINT32 rxChan;
    VOS_INT32  rssi[RX_ANT_NUM_MAX]; /* 单位RX level 0.125dBm */
} LBT_TASK_RssiInd;

/* LTE-V 单独配置功率参数的接口 */
typedef struct {
    VOS_UINT16  OnOff;          /* 是否打开上行:1-打开，0-关闭 */
    VOS_UINT16  tpcOnOff;       /* 是否响应TPC，0-不响应，1-响应仪器TPC */
    VOS_INT16   targetPower;    /* 单位1/8dBm */
    VOS_UINT16  mprDbbVal;      /* 单位1/8dBm */
    VOS_UINT16  antMap;         /* 只支持2T */
    VOS_UINT16  rsv;
} NBT_VPHY_TxPowerPara;



#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

