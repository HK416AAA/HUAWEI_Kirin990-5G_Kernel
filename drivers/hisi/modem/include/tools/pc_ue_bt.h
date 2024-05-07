
#ifndef PE_UE_BT_H
#define PE_UE_BT_H

#include "vos.h"
#include "cbt_socp_head.h"
#include "guc_bt_shared.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#pragma pack(push, 4)

/*
 * 功能说明: W模G模BT 获取Rssi请求
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16 primId;
    COMPONENT_ID_STRU compMode;
    VOS_UINT32 msgLength;
    NBT_WgGetRssiReqPara para;
} PC_NBT_WgGetRssiReq;

/*
 * 功能说明: GUC BT 消息透传请求
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16 primId;
    COMPONENT_ID_STRU compMode;
    VOS_UINT32 msgLength;
    NBT_TransMsgReqPara para;
} PC_NBT_TransMsgReq;

/*
 * 功能说明: W模C模 BT Ber测试请求
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16 primId;
    COMPONENT_ID_STRU compMode;
    VOS_UINT32 msgLength;
    NBT_WcBerReqPara para;
} PC_NBT_WcBerReq;

/*
 * 功能说明: W模 BT 配置功率请求
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16 primId;
    COMPONENT_ID_STRU compMode;
    VOS_UINT32 msgLength;
    NBT_WSetPowerReqPara para;
} PC_NBT_WSetPowerReq;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16 primId;
    COMPONENT_ID_STRU compMode;
    VOS_UINT32 msgLength;
    NBT_EdgeBlerCnfPara para;
} PC_NBT_EdgeBlerCnf;

typedef struct {
    OM_MSG_SOCP_HEAD_STRU msgHeader;
    VOS_UINT16 primId;
    COMPONENT_ID_STRU compMode;
    VOS_UINT32 msgLength;
    NBT_GeDlStatusReqPara para;
} PC_NBT_GeDlStatusReq;

/*
 * 结构说明: GSM上报下行质量消息结构
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU header;
    VOS_UINT16 primId;
    VOS_UINT16 toolId;
    VOS_UINT32 msgLength;
    NBT_GeDlStatusPara para;
} PC_NBT_GeDlStatus;

/*
 * 结构说明: GSM上报下行质量消息结构
 */
typedef struct {
    OM_MSG_SOCP_HEAD_STRU header;
    VOS_UINT16 primId;
    VOS_UINT16 toolId;
    VOS_UINT32 msgLength;
    NBT_CSetPowerReqPara para;
} PC_NBT_CSetPowerReq;


#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

