

#ifndef _AT_ATP_PROC_H_
#define _AT_ATP_PROC_H_

#include "vos.h"
#include "at_atp_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

VOS_VOID AT_ProcAtpMsg(VOS_VOID* buffer, VOS_UINT32 len);

VOS_VOID AT_InitAtpPort(VOS_VOID);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
