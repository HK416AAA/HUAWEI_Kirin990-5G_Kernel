

#ifndef __NUPQUEUE_H__
#define __NUPQUEUE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ****************************************************************************
  1 其他头文件包含
**************************************************************************** */
#include "PsTypeDef.h"
#include "securec.h"


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/* ****************************************************************************
  2 宏定义
**************************************************************************** */
/* 模运算 */
#define NUP_QUEUE_MOD_ADD(x, y, z)    ((((x) + (y)) < (z))? ((x) + (y)):(((x) + (y)) - (z)))
#define NUP_QUEUE_MOD_SUB(x, y, z)    (((x) < (y))?(((x) + (z)) - (y)):((x) - (y)))

/* ****************************************************************************
  3 枚举定义
**************************************************************************** */

/* ****************************************************************************
  4 全局变量声明
**************************************************************************** */

/* ****************************************************************************
  5 消息头定义
**************************************************************************** */

/* ****************************************************************************
  6 消息定义
**************************************************************************** */

/* ****************************************************************************
  7 STRUCT定义
**************************************************************************** */
/* ****************************************************************************
结构名    : NUP_QUE_STRU
协议表格  :
ASN.1描述 :
结构说明  : NUP队列结构
**************************************************************************** */
typedef struct
{
    VOS_UINT32                          ulHead;                                 /* 头结点 */
    VOS_UINT32                          ulTail;                                 /* 尾结点 */
    VOS_UINT32                          ulMaxNum;                               /* 节点个数 */
    VOS_UINT32                          ulNodeSize;                             /* 节点大小 */
    VOS_UINT8                          *pucStartAddr;                           /* 队列起始地址 */
}NUP_QUE_STRU;

/* ****************************************************************************
  8 UNION定义
**************************************************************************** */

/* ****************************************************************************
  9 OTHERS定义
**************************************************************************** */

/* ****************************************************************************
  10 函数声明
**************************************************************************** */
extern VOS_UINT32 NUP_QueInit(NUP_QUE_STRU *pstQ, VOS_UINT32 ulMaxNum, VOS_UINT32 ulNodeSize, VOS_UINT8 *pucStartAddr);
extern VOS_UINT32 NUP_QueReset(NUP_QUE_STRU *pstQ);
extern VOS_UINT32 NUP_QueCnt(NUP_QUE_STRU *pstQ);
extern VOS_UINT32 NUP_QueIdleCnt(NUP_QUE_STRU *pstQ);
extern VOS_UINT32 NUP_QueEmpty(NUP_QUE_STRU *pstQ);
extern VOS_UINT32 NUP_QueFull(NUP_QUE_STRU *pstQ);
extern VOS_UINT32 NUP_EnQue(NUP_QUE_STRU *pstQ, VOS_UINT32 *pulNode, VOS_UINT32 ulNodeSize);
extern VOS_UINT32 NUP_BatchEnQue(NUP_QUE_STRU *pstQ, VOS_UINT8 *pData, VOS_UINT32 ulCpyNum);
extern VOS_UINT32 NUP_DeQue(NUP_QUE_STRU *pstQ, VOS_UINT32 *pulNode, VOS_UINT32 ulNodeSize);
extern VOS_UINT32 NUP_BatchDeQue(NUP_QUE_STRU *pstQ, VOS_UINT8 *pData, VOS_UINT32* pulCpyNum);
extern VOS_UINT32 NUP_QueFwdHead(NUP_QUE_STRU *pstQ);
extern VOS_VOID*  NUP_QuePeekHead(NUP_QUE_STRU *pstQ);
extern VOS_UINT32 NUP_QueFwdTail(NUP_QUE_STRU *pstQ);
extern VOS_VOID*  NUP_QuePeekTail(NUP_QUE_STRU *pstQ);
extern VOS_VOID*  NUP_QuePeekNode(NUP_QUE_STRU *pstQ, VOS_UINT32 ulOffset);
extern VOS_VOID*  NUP_QuePeekLastNode(NUP_QUE_STRU *pstQ);
extern VOS_VOID*  NUP_QuePeekNTail(NUP_QUE_STRU *pstQ, VOS_UINT32 ulOffset);
extern VOS_UINT32 NUP_QueFwdNTail(NUP_QUE_STRU *pstQ, VOS_UINT32 ulOffset);

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

