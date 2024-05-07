

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include  "ChrCommConvert.h"


/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
/* 直通新接口定义，新增事件或XML结构改变时必须更新 */
VOS_UINT8  aucProductName[20] = {"COMMON#104"};
VOS_UINT8  ucLogVersion       = 58;


/*****************************************************************************
  3 函数实现
*****************************************************************************/


MODULE_EXPORTED VOS_VOID Modem_ErrLog_FillModemHeader(chr_modemlog_header_s *pstmodemHeader, VOS_UINT16 usSubEventID,
                                              VOS_UINT8 ucSubEventCause, VOS_BOOL enModemHidsLog)
{

    if (VOS_NULL_PTR == pstmodemHeader)
    {
        return;
    }

    /* memcpy函数整个，为避免LLT_OS平台库依赖问题，使用直接字符拷贝操作 */
    VOS_UINT32 i=0;
    for(i=0; i<20; ++i){
        pstmodemHeader->product_name[i] = aucProductName[i];
    }

    pstmodemHeader->sub_eventid    = usSubEventID;
    pstmodemHeader->en_modem_hids_log  = enModemHidsLog;
    pstmodemHeader->log_version    = ucLogVersion;
    pstmodemHeader->sub_event_cause = ucSubEventCause;
}


const STRUCT_DESC * GetStructDescByAlarmID(VOS_UINT16 usAlarmID)
{
    VOS_UINT32 i = 0;
    const STRUCT_DESC *pStructDesc = VOS_NULL_PTR;
    for (i = 0; i < g_AlarmStructDescCount; ++i)
    {
        pStructDesc = g_AlarmStructDesc[i];
        if (VOS_NULL_PTR != pStructDesc && usAlarmID == pStructDesc->EventID)
        {
            return pStructDesc;
        }
    }

    return VOS_NULL_PTR;
}


MODULE_EXPORTED VOS_INT32 Modem_ErrLog_ConvertEvent(VOS_UINT16 usAlarmID, VOS_UINT8 *pBuffer, VOS_UINT32 ulBufferSize,
    VOS_UINT32 *pBufferLen, const VOS_UINT8 *pStruct, VOS_UINT32 ulStructSize)
{
    return VOS_ERR;
}


MODULE_EXPORTED VOS_INT32 Modem_ErrLog_CalcEventSize(VOS_UINT16 usAlarmID)
{
    return 0;
}


MODULE_EXPORTED VOS_INT32 Modem_ErrLog_ConvertAbsoluteEvent(VOS_UINT16 usAlarmID, VOS_UINT8 *pBuffer, VOS_UINT32 ulBufferSize,
    VOS_UINT32 *pBufferLen, const VOS_UINT8 *pStruct, VOS_UINT32 ulStructSize)
{
    if (VOS_NULL_PTR == pBuffer || VOS_NULL_PTR == pStruct || NULL == pBufferLen || ulBufferSize < ulStructSize)
    {
        return VOS_ERR;
    }

    /* memcpy函数整个，为避免LLT_OS平台库依赖问题，使用直接字符拷贝操作 */
    VOS_UINT32 i=0;
    for(i=0; i<ulStructSize; ++i){
        pBuffer[i] = pStruct[i];
    }

    *pBufferLen = ulStructSize;
    return VOS_OK;
}


MODULE_EXPORTED VOS_INT32 Modem_ErrLog_CalcAbsoluteEventSize(VOS_UINT16 usAlarmID)
{
    const STRUCT_DESC *pStructDesc = GetStructDescByAlarmID(usAlarmID);

    if (VOS_NULL_PTR == pStructDesc)
    {
        return 0;
    }

    return pStructDesc->StructSize;
}




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


