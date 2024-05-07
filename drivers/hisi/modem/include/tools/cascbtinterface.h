

#ifndef __CASCBTINTERFACE_H__
#define __CASCBTINTERFACE_H__

/*
 * 1 其他头文件包含
 */
#include "vos.h"
#include "cproc1xcm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 2 宏定义
 */

/*
 * 3 枚举定义
 */

/*
 * 4 全局变量声明
 */

/*
 * 5 消息头定义
 */

/*
 * 6 消息定义
 */

/*
 * 7 STRUCT定义
 */

/*
 * 8 UNION定义
 */

/*
 * 9 OTHERS定义
 */

/*
 * 10 函数声明
 */
VOS_UINT32 CAS_1X_MAIN_CbtParseSyncInd(MsgBlock *pMsg, CAS_CPROC_1X_SET_TIMING_REQ_STRU *pstSetTimingReq);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of CasCbtInterface.h */

