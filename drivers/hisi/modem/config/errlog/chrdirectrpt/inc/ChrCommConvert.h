
#ifndef _CHRCOMMCONVERT_H
#define _CHRCOMMCONVERT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#include  "product_config.h"
#include  "vos.h"
#ifdef MODEM_FUSION_VERSION
#include  "mdrv_chr.h"
#else
#include  "omerrorlog.h"
#endif

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

/* 结构体描述信息结构体 */
typedef struct
{
    VOS_UINT16          EventID;
    VOS_UINT16          StructSize;
} STRUCT_DESC;

extern const STRUCT_DESC * const g_AlarmStructDesc[];
extern const VOS_UINT32 g_AlarmStructDescCount;

/* 直通新接口定义，新增事件或XML结构改变时必须更新 */
extern   VOS_UINT8  aucProductName[20];
extern   VOS_UINT8  ucLogVersion;


extern VOS_VOID Modem_ErrLog_FillModemHeader(chr_modemlog_header_s *pstmodemHeader, VOS_UINT16 usSubEventID,
                                              VOS_UINT8 ucSubEventCause, VOS_BOOL enModemHidsLog);



extern VOS_INT32 Modem_ErrLog_ConvertEvent(VOS_UINT16 usEventID, VOS_UINT8 *pBuffer, VOS_UINT32 ulBufferSize,
    VOS_UINT32 *pBufferLen, const VOS_UINT8 *pStruct, VOS_UINT32 ulStructSize);


extern VOS_INT32 Modem_ErrLog_CalcEventSize(VOS_UINT16 usEventID);


extern VOS_INT32 Modem_ErrLog_ConvertAbsoluteEvent(VOS_UINT16 usEventID, VOS_UINT8 *pBuffer, VOS_UINT32 ulBufferSize,
    VOS_UINT32 *pBufferLen, const VOS_UINT8 *pStruct, VOS_UINT32 ulStructSize);


extern VOS_INT32 Modem_ErrLog_CalcAbsoluteEventSize(VOS_UINT16 usEventID);

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif
