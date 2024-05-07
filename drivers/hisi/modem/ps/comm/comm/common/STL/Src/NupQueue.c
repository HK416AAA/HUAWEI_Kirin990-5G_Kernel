

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ****************************************************************************
  1 头文件包含
**************************************************************************** */
#include "NupQueue.h"
#define   THIS_FILE_ID   PS_FILE_ID_NUP_QUEUE_C

/* ****************************************************************************
  2 全局变量定义
**************************************************************************** */

/* ****************************************************************************
  3 函数实现
**************************************************************************** */

MODULE_EXPORTED VOS_UINT32 NUP_QueInit(NUP_QUE_STRU *pstQ, VOS_UINT32 ulMaxNum,
                           VOS_UINT32 ulNodeSize, VOS_UINT8 *pucStartAddr)
{
    /* 参数检查 */
    if ((VOS_NULL_PTR == pstQ)
        || (VOS_NULL_PTR == pucStartAddr)
        || (ulMaxNum < 2)
        || (VOS_NULL_PTR != pstQ->pucStartAddr))
    {
        return PS_FAIL;
    }

    pstQ->pucStartAddr  = pucStartAddr;
    pstQ->ulMaxNum      = ulMaxNum;
    pstQ->ulNodeSize    = ulNodeSize;
    pstQ->ulHead        = 0;
    pstQ->ulTail        = 0;

    return PS_SUCC;
}


MODULE_EXPORTED VOS_UINT32 NUP_QueReset(NUP_QUE_STRU *pstQ)
{
    /* 参数检查 */
    if ((VOS_NULL_PTR == pstQ)
        || (VOS_NULL_PTR == pstQ->pucStartAddr))
    {
        return PS_FAIL;
    }

    pstQ->ulHead = 0;
    pstQ->ulTail = 0;

    return PS_SUCC;
}


MODULE_EXPORTED VOS_UINT32 NUP_QueCnt(NUP_QUE_STRU *pstQ)
{
    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return 0;
    }

    return NUP_QUEUE_MOD_SUB(pstQ->ulTail, pstQ->ulHead, pstQ->ulMaxNum);
}


MODULE_EXPORTED VOS_UINT32 NUP_QueIdleCnt(NUP_QUE_STRU *pstQ)
{
    VOS_UINT32  ulCnt;

    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return 0;
    }

    ulCnt = NUP_QUEUE_MOD_SUB(pstQ->ulTail, pstQ->ulHead, pstQ->ulMaxNum);

    return (pstQ->ulMaxNum - 1 - ulCnt);
}


MODULE_EXPORTED VOS_UINT32 NUP_QueEmpty(NUP_QUE_STRU *pstQ)
{
    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return PS_TRUE;
    }

    if (pstQ->ulHead == pstQ->ulTail)
    {
        return PS_TRUE;
    }

    return PS_FALSE;
}


VOS_UINT32 NUP_QueFull(NUP_QUE_STRU *pstQ)
{
    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return PS_TRUE;
    }

    if (pstQ->ulHead == NUP_QUEUE_MOD_ADD(pstQ->ulTail, 1, pstQ->ulMaxNum))
    {
        return PS_TRUE;
    }

    return PS_FALSE;
}


VOS_UINT32 NUP_EnQue(NUP_QUE_STRU *pstQ, VOS_UINT32 *pulNode, VOS_UINT32 ulNodeSize)
{
    VOS_UINT32  ulTail;
    VOS_UINT32 *pDstAddr = VOS_NULL_PTR;
    VOS_UINT32 *pSrcAddr = VOS_NULL_PTR;
    VOS_UINT32  ulWriteNum;
    VOS_UINT32  ulWriteCnt;

    /* 参数检查 */
    if ((VOS_NULL_PTR == pstQ)
        || (VOS_NULL_PTR == pulNode)
        || (VOS_NULL_PTR == pstQ->pucStartAddr))
    {
        return PS_FAIL;
    }

    ulTail = NUP_QUEUE_MOD_ADD(pstQ->ulTail, 1, pstQ->ulMaxNum);
    if (ulTail == pstQ->ulHead)
    {
        return PS_FAIL;
    }

    pSrcAddr     = pulNode;
    pDstAddr     = (VOS_UINT32 *)(VOS_UINT_PTR)((VOS_UINT32)(VOS_UINT_PTR)pstQ->pucStartAddr + pstQ->ulTail * pstQ->ulNodeSize);
    ulWriteNum   = ulNodeSize >> 2;
    for (ulWriteCnt = 0; ulWriteCnt < ulWriteNum; ulWriteCnt++)
    {
        *(pDstAddr + ulWriteCnt) = *(pSrcAddr + ulWriteCnt);
    }

#if (VOS_RTOSCK == VOS_OS_VER)
#ifdef MODEM_FUSION_VERSION

    __asm__ __volatile__("sync");

#else

#ifdef __OS_RTOSCK_SMP__

        /* :
        从逻辑角度,上下句有依赖关系；
        从流水线角度,上下文没有依赖,顺序不固定；
        增加dmb操作 */
        __asm__ __volatile__("dmb": : :"memory");
#endif

#endif
#endif

    pstQ->ulTail = ulTail;

    return PS_SUCC;
}


MODULE_EXPORTED VOS_UINT32 NUP_BatchEnQue(NUP_QUE_STRU *pstQ, VOS_UINT8 *pData, VOS_UINT32 ulCpyNum)
{
    VOS_UINT32  ulCnt;
    VOS_UINT8  *pDstAddr = VOS_NULL_PTR;
    VOS_UINT32  ulTail;
    VOS_UINT32  ulFirstCpyNum;

    /* 参数检查 */
    if ((VOS_NULL_PTR == pstQ)
        || (VOS_NULL_PTR == pData)
        || (0 == ulCpyNum))
    {
        return PS_FAIL;
    }

    ulCnt = NUP_QUEUE_MOD_SUB(pstQ->ulTail, pstQ->ulHead, pstQ->ulMaxNum);
    if (ulCpyNum > (pstQ->ulMaxNum - 1 - ulCnt))
    {
        return PS_FAIL;
    }

    pDstAddr = pstQ->pucStartAddr + pstQ->ulNodeSize * pstQ->ulTail;
    ulTail   = NUP_QUEUE_MOD_ADD(pstQ->ulTail, ulCpyNum, pstQ->ulMaxNum);
    if (ulTail > pstQ->ulTail)
    {
        if(memcpy_s(pDstAddr, ulCpyNum*pstQ->ulNodeSize, pData,
                    ulCpyNum*pstQ->ulNodeSize) != EOK)
        {
            return PS_FAIL;
        }
    }
    else
    {
        ulFirstCpyNum = NUP_QUEUE_MOD_SUB(0, pstQ->ulTail, pstQ->ulMaxNum);
        if(memcpy_s(pDstAddr, ulCpyNum*pstQ->ulNodeSize, pData,
                    ulFirstCpyNum*pstQ->ulNodeSize) != EOK)
        {
            return PS_FAIL;
        }
        if (0 != ulTail)
        {
            if(memcpy_s(pstQ->pucStartAddr, ulCpyNum*pstQ->ulNodeSize,
                        pData+ulFirstCpyNum*pstQ->ulNodeSize,
                        (ulCpyNum-ulFirstCpyNum)*pstQ->ulNodeSize) != EOK)
            {
                return PS_FAIL;
            }
        }
    }


#if (VOS_RTOSCK == VOS_OS_VER)
#ifdef MODEM_FUSION_VERSION

    __asm__ __volatile__("sync");

#else

#ifdef __OS_RTOSCK_SMP__

        /* :
        从逻辑角度,上下句有依赖关系；
        从流水线角度,上下文没有依赖,顺序不固定；
        增加dmb操作 */
        __asm__ __volatile__("dmb": : :"memory");
#endif

#endif
#endif

    pstQ->ulTail = ulTail;

    return PS_SUCC;
}


VOS_UINT32 NUP_DeQue(NUP_QUE_STRU *pstQ, VOS_UINT32 *pulNode, VOS_UINT32 ulNodeSize)
{
    VOS_UINT32  ulHead;
    VOS_UINT32 *pDstAddr = VOS_NULL_PTR;
    VOS_UINT32 *pSrcAddr = VOS_NULL_PTR;
    VOS_UINT32  ulWriteNum;
    VOS_UINT32  ulWriteCnt;

    /* 参数检查 */
    if ((VOS_NULL_PTR == pstQ)
        || (VOS_NULL_PTR == pulNode)
        || (VOS_NULL_PTR == pstQ->pucStartAddr)
        || (0 != (ulNodeSize & 0x3))
        || (ulNodeSize != pstQ->ulNodeSize))
    {
        return PS_FAIL;
    }

    if (pstQ->ulHead == pstQ->ulTail)
    {
        return PS_FAIL;
    }

    ulHead     = NUP_QUEUE_MOD_ADD(pstQ->ulHead, 1, pstQ->ulMaxNum);
    pSrcAddr     = (VOS_UINT32 *)(VOS_UINT_PTR)((VOS_UINT32)(VOS_UINT_PTR)pstQ->pucStartAddr + pstQ->ulHead * pstQ->ulNodeSize);
    pDstAddr     = pulNode;
    ulWriteNum   = ulNodeSize >> 2;
    for (ulWriteCnt = 0; ulWriteCnt < ulWriteNum; ulWriteCnt++)
    {
        *(pDstAddr + ulWriteCnt) = *(pSrcAddr + ulWriteCnt);
    }
    pstQ->ulHead = ulHead;

    return PS_SUCC;
}


MODULE_EXPORTED VOS_UINT32 NUP_BatchDeQue(NUP_QUE_STRU *pstQ, VOS_UINT8 *pData, VOS_UINT32* pulCpyNum)
{
    VOS_UINT32  ulCnt;
    VOS_UINT32  ulCpyNum;
    VOS_UINT8  *pSrcAddr = VOS_NULL_PTR;
    VOS_UINT32  ulHead;
    VOS_UINT32  ulFirstCpyNum;

    if ((VOS_NULL_PTR == pstQ)
        || (VOS_NULL_PTR == pData)
        || (VOS_NULL_PTR == pulCpyNum)
        || (0 == *pulCpyNum))
    {
        return PS_FAIL;
    }

    ulCnt = NUP_QUEUE_MOD_SUB(pstQ->ulTail, pstQ->ulHead, pstQ->ulMaxNum);
    if (0 == ulCnt)
    {
        *pulCpyNum = 0;
        return PS_SUCC;
    }

    ulCpyNum = (*pulCpyNum >= ulCnt)?ulCnt:*pulCpyNum;
    pSrcAddr = pstQ->pucStartAddr + pstQ->ulNodeSize * pstQ->ulHead;
    ulHead   = NUP_QUEUE_MOD_ADD(pstQ->ulHead, ulCpyNum, pstQ->ulMaxNum);

    if (ulHead > pstQ->ulHead)
    {
        if (memcpy_s(pData, ulCpyNum*pstQ->ulNodeSize, pSrcAddr,
                     ulCpyNum*pstQ->ulNodeSize) != EOK)
        {
            return PS_FAIL;
        }
    }
    else
    {
        ulFirstCpyNum = NUP_QUEUE_MOD_SUB(0, pstQ->ulHead, pstQ->ulMaxNum);
        if (memcpy_s(pData, ulCpyNum*pstQ->ulNodeSize, pSrcAddr,
                     ulFirstCpyNum*pstQ->ulNodeSize) != EOK)
        {
            return PS_FAIL;
        }

        if (0 != ulHead)
        {
            if (memcpy_s(pData+ulFirstCpyNum*pstQ->ulNodeSize,
                         ulCpyNum*pstQ->ulNodeSize, pstQ->pucStartAddr,
                         (ulCpyNum-ulFirstCpyNum)*pstQ->ulNodeSize) != EOK)
            {
                return PS_FAIL;
            }
        }
    }

    pstQ->ulHead = ulHead;
    *pulCpyNum   = ulCpyNum;

    return PS_SUCC;
}


MODULE_EXPORTED VOS_UINT32 NUP_QueFwdHead(NUP_QUE_STRU *pstQ)
{
    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return PS_FAIL;
    }

    if (pstQ->ulHead == pstQ->ulTail)
    {
        return PS_FAIL;
    }

    pstQ->ulHead = NUP_QUEUE_MOD_ADD(pstQ->ulHead, 1, pstQ->ulMaxNum);

    return PS_SUCC;
}


MODULE_EXPORTED VOS_VOID* NUP_QuePeekHead(NUP_QUE_STRU *pstQ)
{
    if (VOS_NULL_PTR == pstQ)
    {
        return VOS_NULL_PTR;
    }

    if (pstQ->ulHead == pstQ->ulTail)
    {
        return VOS_NULL_PTR;
    }

    return (VOS_VOID*)(pstQ->pucStartAddr + pstQ->ulNodeSize * pstQ->ulHead);
}


VOS_UINT32 NUP_QueFwdTail(NUP_QUE_STRU *pstQ)
{
    VOS_UINT32  ulTail;

    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return PS_FAIL;
    }

    ulTail = NUP_QUEUE_MOD_ADD(pstQ->ulTail, 1, pstQ->ulMaxNum);
    if (ulTail == pstQ->ulHead)
    {
        return PS_FAIL;
    }

#if (VOS_RTOSCK == VOS_OS_VER)
#ifdef MODEM_FUSION_VERSION

    __asm__ __volatile__("sync");

#else

#ifdef __OS_RTOSCK_SMP__

        /* :
        从逻辑角度,上下句有依赖关系；
        从流水线角度,上下文没有依赖,顺序不固定；
        增加dmb操作 */
        __asm__ __volatile__("dmb": : :"memory");
#endif

#endif
#endif

    pstQ->ulTail = ulTail;

    return PS_SUCC;
}

VOS_UINT32 NUP_QueFwdNTail(NUP_QUE_STRU *pstQ, VOS_UINT32 ulOffset)
{
    VOS_UINT32  ulTail;
    VOS_UINT32  ulCnt;

    /* 参数检查 */
    if (pstQ == VOS_NULL_PTR)
    {
        return PS_FAIL;
    }

    ulCnt = NUP_QueIdleCnt(pstQ);
    if (ulOffset > ulCnt)
    {
        return PS_FAIL;
    }

    ulTail = NUP_QUEUE_MOD_ADD(pstQ->ulTail, ulOffset, pstQ->ulMaxNum);

#if (VOS_RTOSCK == VOS_OS_VER)
#ifdef MODEM_FUSION_VERSION

    __asm__ __volatile__("sync");

#else

#ifdef __OS_RTOSCK_SMP__

        /* :
        从逻辑角度,上下句有依赖关系；
        从流水线角度,上下文没有依赖,顺序不固定；
        增加dmb操作 */
        __asm__ __volatile__("dmb": : :"memory");
#endif

#endif
#endif

    pstQ->ulTail = ulTail;

    return PS_SUCC;
}



VOS_VOID* NUP_QuePeekTail(NUP_QUE_STRU *pstQ)
{
    VOS_UINT32  ulTail;

    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return VOS_NULL_PTR;
    }

    ulTail = NUP_QUEUE_MOD_ADD(pstQ->ulTail, 1, pstQ->ulMaxNum);
    if (ulTail == pstQ->ulHead)
    {
        return VOS_NULL_PTR;
    }

    return (VOS_VOID *)(pstQ->pucStartAddr + pstQ->ulNodeSize * pstQ->ulTail);
}

VOS_VOID* NUP_QuePeekNTail(NUP_QUE_STRU *pstQ, VOS_UINT32 ulOffset)
{
    VOS_UINT32  ulTail;
    VOS_UINT32  ulCnt;

    /* 参数检查 */
    if (pstQ == VOS_NULL_PTR)
    {
        return VOS_NULL_PTR;
    }

    ulCnt = NUP_QueIdleCnt(pstQ);
    if (ulOffset >= ulCnt)
    {
        return VOS_NULL_PTR;
    }

    ulTail = NUP_QUEUE_MOD_ADD(pstQ->ulTail, ulOffset, pstQ->ulMaxNum);

    return (VOS_VOID *)(pstQ->pucStartAddr + pstQ->ulNodeSize * ulTail);
}


VOS_VOID* NUP_QuePeekNode(NUP_QUE_STRU *pstQ, VOS_UINT32 ulOffset)
{
    VOS_UINT32 ulNode;
    VOS_UINT32 ulCnt;

    if (VOS_NULL_PTR == pstQ)
    {
        return VOS_NULL_PTR;
    }

    ulCnt = NUP_QUEUE_MOD_SUB(pstQ->ulTail, pstQ->ulHead, pstQ->ulMaxNum);
    if ((0 == ulCnt) || (ulOffset >= ulCnt))
    {
        return VOS_NULL_PTR;
    }

    ulNode = NUP_QUEUE_MOD_ADD(pstQ->ulHead, ulOffset, pstQ->ulMaxNum);

    return (VOS_VOID *)(pstQ->pucStartAddr + pstQ->ulNodeSize * ulNode);
}



VOS_VOID* NUP_QuePeekLastNode(NUP_QUE_STRU *pstQ)
{
    VOS_UINT32  ulCnt;
    VOS_UINT32  ulLastNode;

    /* 参数检查 */
    if (VOS_NULL_PTR == pstQ)
    {
        return VOS_NULL_PTR;
    }

    ulCnt = NUP_QUEUE_MOD_SUB(pstQ->ulTail, pstQ->ulHead, pstQ->ulMaxNum);
    if (0 == ulCnt)
    {
        return VOS_NULL_PTR;
    }

    ulLastNode = NUP_QUEUE_MOD_ADD(pstQ->ulHead, ulCnt - 1, pstQ->ulMaxNum);

    return (VOS_VOID *)(pstQ->pucStartAddr + pstQ->ulNodeSize * ulLastNode);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

