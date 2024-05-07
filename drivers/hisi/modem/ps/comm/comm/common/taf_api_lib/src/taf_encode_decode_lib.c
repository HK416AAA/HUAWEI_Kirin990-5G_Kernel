
#include "taf_encode_decode_lib.h"
#include "taf_std_lib.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_TAF_ENCODE_DECODE_LIB_C

#if (OSA_CPU_CCPU == VOS_OSA_CPU)

MODULE_EXPORTED VOS_UINT32 TAF_COMMAPI_ConvertStrEncodingType(
    TAF_COMM_StrWithEncodingType *srcStr,
    TAF_COMM_EncodingTypeUint8 dstCoding,
    VOS_UINT32 dstBuffLen,
    TAF_COMM_StrWithEncodingType *dstStr)
{
    return TAF_STD_ConvertStrEncodingType(srcStr, dstCoding, dstBuffLen, dstStr);
}
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
