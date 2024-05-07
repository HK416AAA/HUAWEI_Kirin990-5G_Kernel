
#ifndef __CHR_RELATIONEVENT_CFG_H__
#define __CHR_RELATIONEVENT_CFG_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"
#include "product_config.h"
#include "ChrEventIDComm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/
//pclint清理 xuziheng/306651 20180515
#ifndef FAULT_ID_CONTAIN_ALARM_ID_MAX_NUM
#define   FAULT_ID_CONTAIN_ALARM_ID_MAX_NUM   (256) /* ERRLOG 支持一个faultid关联256个alarm_id */
#endif

//pclint清理 xuziheng/306651 20180515
#ifndef ERR_LOG_PID_MAX_NUM
#define   ERR_LOG_PID_MAX_NUM                 (256) /* ERRLOG 支持256个PID */
#endif

/* 关联事件在此定义，以便工具生成关联事件XML */
#define RELATION_EVENT_DEFINITION

#define RELATIONEVENT1 \
{TDS_OOS_INFO_REPORT_FAULTID, TPS_PID_RRC, TRRC_OOS_INFO_STRU_ALARMID},\
{TDS_OOS_INFO_REPORT_FAULTID, TPS_PID_MAC, TLL2_OOS_INFO_STRU_ALARMID}


#undef RELATION_EVENT_DEFINITION

/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulSndPID;
    VOS_UINT16                          usAlarmID;
    VOS_UINT8                           ucReserved[2];
}ERRLOG_ALARM_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/

extern VOS_INT32 Modem_ErrLog_GetErrReptAddrAndSize(
           VOS_UINT32                          ulModemId,
           VOS_UINT_PTR                       *pulErrLogAddr,
           VOS_UINT32                         *pulsize
       );
extern VOS_INT32 Modem_ErrLog_GetPidAndAlarmId(
           VOS_UINT32                          ulModemId,
           VOS_UINT32                          ulFaultId,
           ERRLOG_ALARM_STRU                  *paustAlarmArray,
           VOS_UINT32                         *pulAlarmNum
       );



#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
