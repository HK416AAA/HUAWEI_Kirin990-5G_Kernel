
#ifndef BBIC_CAL_H
#define BBIC_CAL_H

#include "bbic_cal_shared.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#pragma pack(push, 4)

#define BBIC_CAL_COMM_STRU_BEGIN

/*
 * Description: Msg header struct, only with stComponentID
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
} BBIC_CAL_MSG_HDR_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT16                     uhwBusiness;
    UINT16                     uhwRsv;
} BBIC_CAL_BU_TYPE_STRU;

/*
 * Description: Msg header struct, only with stComponentID
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT8                  aucPara[4];
} BBIC_CAL_MSG_BLOCK_STRU;

#define BBIC_CAL_SELF_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                stComponentID;
    BBIC_CAL_COMM_SELFCAL_START_REQ_PARA_STRU stPara;
} BBIC_CAL_COMM_SELFCAL_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    BBIC_CAL_COMM_SELFCAL_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_COMM_SELFCAL_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_RX_IQ_START_REQ_PARA_STRU stPara;
} BBIC_CAL_RX_IQ_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RX_FIIQ_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_RX_FIIQ_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RX_FDIQ_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_RX_FDIQ_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    BBIC_CAL_TX_FILTER_START_REQ_PARA_STRU stPara;
} BBIC_CAL_TX_FILTER_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    BBIC_CAL_TX_FILTER_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_TX_FILTER_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_TX_IQ_START_REQ_PARA_STRU stPara;
} BBIC_CAL_TX_IQ_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    BBIC_CAL_TX_IQ_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_TX_IQ_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_TX_LO_START_REQ_PARA_STRU stPara;
} BBIC_CAL_TX_LO_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    BBIC_CAL_TX_LO_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_TX_LO_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    BBIC_CAL_IP2_START_REQ_PARA_STRU stPara;
} BBIC_CAL_IP2_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU        stComponentID;
    BBIC_CAL_IP2_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_IP2_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentID;
    BBIC_CAL_MRX_DELAY_IND_PATH_STRU   para;
} BBIC_CAL_MrxDelayResultInd;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_MRX_DCOC_START_REQ_PARA_STRU stPara;
} BBIC_CAL_MRX_DCOC_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    BBIC_CAL_MRX_DCOC_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_MRX_DCOC_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU  stComponentID;
	BBIC_CAL_RX_DCOC_START_REQ_PARA_STRU stPara;
} BBIC_CAL_RX_DCOC_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RX_DCOC_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_RX_DCOC_RESULT_IND_STRU;

#define BBIC_CAL_TRX_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_RX_START_REQ_PARA_STRU stPara;
} BBIC_CAL_RX_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    BBIC_CAL_RX_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_RX_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_TX_START_REQ_PARA_STRU stPara;
} BBIC_CAL_TX_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    BBIC_CAL_TX_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_TX_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    BBIC_CAL_VCC_START_REQ_PARA_STRU stPara;
} BBIC_CAL_PA_VCC_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    BBIC_CAL_PA_VCC_IND_RESULT_PARA_STRU stPara;
} BBIC_CAL_PA_VCC_RESULT_IND_STRU;

/*
 * Description: 单个Path校准的Mrx coor，按Pa 增益单个VCC进行返回，仅提取时上报该原始数据，包括IQ路数据
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_PA_VCC_MRXCOOR_IND_PARA_STRU stPara;
} BBIC_CAL_PA_VCC_PATH_MRXCOOR_IND_STRU;

/*
 * Description: 提取或校准时拟合样本对上报，按Path支持统一上报
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    BBIC_CAL_PA_VC_SAMPLES_IND_PARA_STRU stPara;
} BBIC_CAL_PA_VCC_EXTRACT_SAMPLES_IND_STRU;

/*
 * Description: 包括Apt/Apt Dpd/Et/Et Dpd compress, 温频补的提取
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    BBIC_CAL_PA_VCC_COMPRESS_REQ_PARA_STRU stPara;
} BBIC_CAL_PA_VCC_COMPRESS_START_REQ_STRU;

typedef BBIC_CAL_MSG_CNF_STRU BBIC_CAL_PA_VCC_COMPRESS_CNF_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    BBIC_CAL_PA_GAIN_DELTA_REQ_PARA_STRU stPara;
} BBIC_CAL_PA_GAIN_DELTA_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    BBIC_CAL_PA_GAIN_DELTA_IND_PARA_STRU stPara;
} BBIC_CAL_PA_GAIN_DELTA_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    BBIC_CAL_DPD_START_REQ_PARA_STRU stPara;
} BBIC_CAL_DPD_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU        stComponentID;
    BBIC_CAL_DPD_IND_RESULT_PARA_STRU stPara;
} BBIC_CAL_DPD_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_ET_DELAY_START_REQ_PARA_STRU stPara;
} BBIC_CAL_ET_DELAY_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    BBIC_CAL_ET_DELAY_K_IND_RESULT_PARA_STRU stPara;
} BBIC_CAL_ET_DELAY_K_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    BBIC_CAL_ET_DELAY_IND_RESULT_PARA_STRU stPara;
} BBIC_CAL_ET_DELAY_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU   stComponentID;
    BBIC_CAL_GE_TX_REQ_PARA_STRU stPara;
} BBIC_CAL_GE_TX_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU   stComponentID;
    BBIC_CAL_GE_TX_IND_PARA_STRU stPara;
} BBIC_CAL_GE_TX_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwErrorCode;
    VOS_UINT16                 uhwPathNum; /* < 有效的Path数量 */
    VOS_UINT16                 uhwRsv;
} BBIC_CAL_GE_TX_COMM_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_GE_RAMP_REQ_PARA_STRU stPara;
} BBIC_CAL_GETX_RAMP_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_GE_RAMP_IND_PARA_STRU stPara;
} BBIC_CAL_GETX_RAMP_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
} BBIC_CAL_TRX_START_STEP_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    BBIC_CAL_GE_RF_SETTING_REQ_PARA_STRU stPara;
} BBIC_CAL_GE_TX_RFSETTING_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    BBIC_CAL_GE_RF_SETTING_IND_PARA_STRU stPara;
} BBIC_CAL_GE_TX_RFSETTING_RESULT_IND_STRU;

#define BBIC_CAL_RF_DEBUG_TRX_STRU_BEGIN
/*
 * Description: 用于RFDEUG TX的内部消息，暂时不对外开放
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_RF_DEBUG_TX_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TX_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_RF_DEBUG_TX_CNF_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TX_CNF_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_RF_DEBUG_RX_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_RX_REQ_STRU;
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_RF_DEBUG_RX_CNF_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_RX_CNF_STRU;



typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                stComponentID;
    BBIC_CAL_RF_DEBUG_TX_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TX_RESULT_IND_STRU;


typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_RF_DEBUG_TRX_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TRX_REQ_STRU;
typedef BBIC_CAL_RF_DEBUG_TRX_REQ_STRU BBIC_CAL_RF_DEBUG_RM_REQ_STRU;

/*
 * Description: RFDEUG初始化
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
} BBIC_CAL_RF_DEBUG_INIT_REQ_STRU;

/*
 * Description: RM notify请求信息,不需要回复CNF，RM收到该消息，保存上下文
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
} BBIC_CAL_RF_DEBUG_RM_NOTIFY_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_RF_DEBUG_RM_CNF_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_RM_CNF_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    /* Need to nothing */
} BBIC_CAL_RF_DEBUG_RX_EVM_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    BBIC_CAL_RF_DEBUG_RX_EVM_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_RX_EVM_IND_STRU;

#define BBIC_CAL_RF_DEBUG_SELF_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    UINT8                      aucPara[4];
} BBIC_CAL_RF_DEBUG_SELF_UNIFORM_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_DEBUG_DCXO_CFIX_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_DCXO_CFIX_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    /* Need to nothing */
} BBIC_CAL_RF_DEBUG_DCXO_FE_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                  stComponentID;
    BBIC_CAL_RF_DEBUG_DCXO_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_DCXO_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    BBIC_CAL_RF_DEBUG_IP2_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_IP2_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    BBIC_CAL_RF_DEBUG_IP2_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_IP2_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    BBIC_CAL_RF_DEBUG_RX_DCOC_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_RX_DCOC_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    BBIC_CAL_RF_DEBUG_RX_DCOC_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_RX_DCOC_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    BBIC_CAL_RF_DEBUG_MRX_DCOC_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_MRX_DCOC_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    BBIC_CAL_RF_DEBUG_MRX_DCOC_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_MRX_DCOC_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_DEBUG_TX_IQ_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TX_IQ_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_DEBUG_TX_IQ_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TX_IQ_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_DEBUG_TX_LO_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TX_LO_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_DEBUG_TX_LO_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TX_LO_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_READ_REG_DATA_REQ_STRU stPara;
} BBIC_CAL_RF_DEBUG_READ_REG_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    VOS_UINT32                      uwResult;
    BBIC_CAL_READ_REG_DATA_IND_STRU stPara;
} BBIC_CAL_RF_DEBUG_READ_REG_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    BBIC_CAL_WRITE_REG_DATA_REQ_STRU stPara;
} BBIC_CAL_RF_DEBUG_WRITE_REG_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwResult;
} BBIC_CAL_RF_DEBUG_WRITE_REG_IND_STRU;


typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwResult;
} BBIC_CAL_RF_DEBUG_REGISTER_CNF_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwResult;
    VOS_UINT8                  aucPara[4];
} BBIC_CAL_RF_DEBUG_REGISTER_IND_STRU;


#define BBIC_CAL_CBT_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    RAT_MODE_ENUM_UINT16               uhwRatMode;
    BBA_CAL_RELOAD_NV_TYPE_ENUM_UINT16 rficType;
} BBIC_CAL_RELOAD_NV_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwErrorCode;
} BBIC_CAL_RELOAD_NV_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_NV_RESET_REQ_PARA_STRU stPara;
} BBIC_CAL_NV_RESET_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwErrorCode;
} BBIC_CAL_NV_RESET_CNF_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentID;
    UINT32                     count;
    UINT16                     data[0]; /* 需要写的nv数据，按照nvid,len,data排列 */
} BBICCAL_WriteNvReqStru;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentID;
    UINT32                     errorCode; /* 返回执行结果 */
    UINT32                     errorNvId; /* 返回出现错误的NVID */
} BBICCAL_WriteNvCnfStru;

#define BBIC_CAL_PHY_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_WPHY_CONFIG_PARA_STRU stPara;
} BBIC_CAL_WPHY_CONFIG_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_CPHY_CONFIG_PARA_STRU stPara;
} BBIC_CAL_CDMA_CONFIG_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_LPHY_CONFIG_PARA_STRU stPara;
} BBIC_CAL_LTE_CONFIG_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    BBIC_CAL_NB_RSSI_PARA_STRU stPara;
} BBIC_CAL_NB_RSSI_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_NB_RSSI_IND_PARA_STRU stPara;
} BBIC_CAL_NB_RSSI_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    BBIC_CAL_RFIC_SELFCAL_REQ_PARA_STRU stPara;
} BBIC_CAL_RFIC_SELFCAL_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    BBIC_CAL_RFIC_SELFCAL_IND_PARA_STRU stPara;
} BBIC_CAL_RFIC_SELFCAL_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     componentId;
    BBIC_CAL_RficHfRxAdcCalReqPara rxAdcPara;
} BBIC_CAL_RficHfRxAdcSelfCalReq;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     componentId;
    BBIC_CAL_RficHfRxAdcCalIndPara rxAdcInd;
} BBIC_CAL_RficHfRxAdcSelfCalInd;

#define BBIC_CAL_RF_DEBUG_HF_TRX_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_DEBUG_HF_RX_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_HF_RX_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    /* Need to nothing */
} BBIC_CAL_RF_DEBUG_HF_RSSI_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    BBIC_CAL_RF_DEBUG_HF_RX_RSSI_IND_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_HF_RX_RSSI_IND_STRU;

#ifndef MODEM_FUSION_VERSION
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    RFDEBUG_HFRX_PM2mPreSetReqPara             stPara;  /* M2M校准中phase对齐REQ消息 */
} RFDEBUG_HFRX_PM2mPreSetReq;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    RFDEBUG_HFRX_PM2mPreSetIndPara             stPara;
} RFDEBUG_HFRX_PM2mPreSetInd;
#endif

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_DEBUG_HF_TX_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_HF_TX_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_HF_ANA_TEMP_REQ_PARA_STRU stPara;
} BBIC_CAL_RF_HF_ANA_TEMP_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_RF_HF_ANA_TEMP_IND_PARA_STRU stPara;
} BBIC_CAL_RF_HF_ANA_TEMP_IND_STRU;

#define BBIC_CAL_HF_TRX_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_HF_RX_START_REQ_PARA_STRU stPara;
} BBIC_CAL_HF_RX_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    BBIC_CAL_HF_RX_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_HF_RX_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU         stComponentID;
    BBIC_CAL_HF_TX_START_REQ_PARA_STRU stPara;
} BBIC_CAL_HF_TX_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU          stComponentID;
    BBIC_CAL_HF_TX_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_HF_TX_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentID;
    BBIC_CAL_HfDpdStartReqPara para;
} BBIC_CAL_HfDpdStartReq;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU  componentID;
    BBIC_CAL_HfDpdResultIndPara para;
} BBIC_CAL_HfDpdResultInd;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentID;
    BBIC_CAL_HfDpdTriggerPara  para;
} BBIC_CAL_HfDpdTriggerReq;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU   componentID;
    BBIC_CAL_HfDpdTriggerCnfPara para;
} BBIC_CAL_HfDpdTriggerCnf;

#define BBIC_CAL_HF_SELF_STRU_BEGIN

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    BBIC_CAL_HF_RX_DCOC_START_REQ_PARA_STRU stPara;
} BBIC_CAL_HF_RX_DCOC_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    BBIC_CAL_HF_RX_DCOC_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_HF_RX_DCOC_RESULT_IND_STRU;


/*
 * desc : dcxo pc req msg
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentId;
    BBIC_CAL_DcxoSelfCalPara dcxoPara;
} BBIC_CAL_DcxoSelfCalReq;

/*
 * desc : DCXO ind pc msg
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentId;
    BBIC_CAL_DcxoSelfCalRslt dcxoInd;
} BBIC_CAL_DcxoSelfCalInd;


/*
 * desc : DCXO dcoc req msg
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentId;
    BBIC_CAL_DcxoSelfCalDcocReqPara dcocPara;
} BBIC_CAL_DcxoDcocCalReq;

/*
 * desc: DCXO dcoc confirm msg
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU componentId;
    UINT32 errCode;
} BBIC_CAL_DcxoDcocCalCfm;

/*
 * Description: 高频FDIQ校准请求消息结构体
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    BBIC_CAL_HF_SELFCAL_FDIQ_REQ_PARA_STRU stPara;
} BBIC_CAL_HF_SELFCAL_FDIQ_REQ_STRU;

/*
 * Description: 高频FDIQ校准上报消息结构体
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU           stComponentID;
    BBIC_CAL_HF_SELFCAL_IF_IND_PARA_STRU stPara;
} BBIC_CAL_HF_TRX_FDIQ_RESULT_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    BBIC_CAL_HF_RX_FIIQ_START_REQ_PARA_STRU stPara;
} BBIC_CAL_HF_RX_FIIQ_START_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU               stComponentID;
    BBIC_CAL_HF_RX_FIIQ_RESULT_IND_PARA_STRU stPara;
} BBIC_CAL_HF_RX_FIIQ_RESULT_IND_STRU;

/* For Hids */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU            stComponentID;
    BBIC_CAL_DPD_START_REQ_PARA_HIDS_STRU stPara;
} BBIC_CAL_DPD_START_REQ_HIDS_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU             stComponentID;
    BBIC_CAL_DPD_IND_RESULT_PARA_HIDS_STRU stPara;
} BBIC_CAL_DPD_RESULT_IND_HIDS_STRU;

/*
 * Description: SRS Switching
 */
typedef struct
{
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_SRS_REQ_PARA_STRU  stPara;
} BBIC_CAL_SRS_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU    stComponentID;
    BBIC_CAL_SRS_CNF_PARA_STRU    stPara;
} BBIC_CAL_SRS_CNF_STRU;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

