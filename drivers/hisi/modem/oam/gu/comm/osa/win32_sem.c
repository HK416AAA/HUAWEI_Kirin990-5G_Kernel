/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * 版权所有（c）华为技术有限公司 2012-2019
 * 功能描述: OSA win32操作系统上信号量功能实现
 * 生成日期: 2006年10月3日
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "vos_config.h"
#include "v_sem.h"
#include "stdlib.h"
#include "stdio.h"
#include "v_io.h"
#include "v_task.h"
#include "v_int.h"
#include "v_private.h"
#include "mdrv.h"
#include "pam_tag.h"

#if (VOS_WIN32 == VOS_OS_VER)

#ifdef LLT_OS_WIN

#define THIS_FILE_ID PS_FILE_ID_V_WIN32_SEM_C
#define THIS_MODU mod_pam_osa

typedef struct SEM_CONTROL_STRU {
    int flag;                       /* control block's state */
    struct SEM_CONTROL_STRU *semId; /* the ID return to User */
    int semType;                    /* type of SEM */
    int semFlags;
    int semInitCount;
    char name[VOS_MAX_SEM_NAME_LENGTH];
    HANDLE win32Handle; /* maped Win32 Handle */
} SEM_ControlBlock;

/* the number of queue's control block */
VOS_UINT32 g_vosSemCtrlBlkNumber;

/* the start address of queue's control block */
SEM_ControlBlock *g_vosSemCtrlBlk = VOS_NULL_PTR;

/* the Max usage of queue */
VOS_UINT32 g_vosSemMaxSemId;

#define VOS_SEM_CTRL_BUF_SIZE (sizeof(SEM_ControlBlock) * VOS_MAX_SEM_NUMBER)

VOS_CHAR g_vosSemCtrlBuf[VOS_SEM_CTRL_BUF_SIZE];

/*
 * Description: Init semaphore's control block
 */
VOS_VOID VOS_SemCtrlBlkInit(VOS_VOID)
{
    int i;

    g_vosSemCtrlBlkNumber = VOS_MAX_SEM_NUMBER;

    g_vosSemCtrlBlk = (SEM_ControlBlock *)g_vosSemCtrlBuf;

    for (i = 0; i < (int)g_vosSemCtrlBlkNumber; i++) {
        g_vosSemCtrlBlk[i].flag = VOS_SEM_CTRL_BLK_IDLE;
        g_vosSemCtrlBlk[i].semId = g_vosSemCtrlBlk + i;
    }

    g_vosSemMaxSemId = 0;

    return;
}

/*
 * Description: allocate a control block
 */
SEM_ControlBlock *VOS_SemCtrlBlkGet(VOS_VOID)
{
    VOS_UINT32 i;
    int intLockLevel;

    intLockLevel = VOS_SplIMP();

    for (i = 0; i < g_vosSemCtrlBlkNumber; i++) {
        if (g_vosSemCtrlBlk[i].flag == VOS_SEM_CTRL_BLK_IDLE) {
            g_vosSemCtrlBlk[i].flag = VOS_SEM_CTRL_BLK_BUSY;
            break;
        }
    }

    VOS_Splx(intLockLevel);

    if (i < g_vosSemCtrlBlkNumber) {
        /* record the max usage of SEM */
        if (i > g_vosSemMaxSemId) {
            g_vosSemMaxSemId = i;
        }

        return g_vosSemCtrlBlk + i;
    } else {
        mdrv_err("<VOS_GetSemCtrlBlk> no Idle.\n");

        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_FULL);

        return (VOS_MAX_SEM_ID_NULL);
    }
}

/*
 * Description: fress a block
 */
VOS_UINT32 VOS_SemCtrlBlkFree(SEM_ControlBlock *semAddress)
{
    int intLockLevel;

    if (semAddress == semAddress->semId) {
        if (semAddress->flag == VOS_SEM_CTRL_BLK_IDLE) {
            return (VOS_ERR);
        } else {
            intLockLevel = VOS_SplIMP();

            semAddress->flag = VOS_SEM_CTRL_BLK_IDLE;

            VOS_Splx(intLockLevel);
        }

        return (VOS_OK);
    } else {
        mdrv_err("<VOS_FreeSemCtrlBlk> free NULL Sem.\n");

        return (VOS_ERR);
    }
}

/*
 * Description: To create a counting semaphore;
 */
VOS_UINT32 VOS_SmCCreate(const VOS_CHAR *smName, VOS_UINT32 smInit, VOS_UINT32 flags, VOS_SEM *smID)
{
    SEM_ControlBlock *iSemId = VOS_NULL_PTR;
    HANDLE semId;
    VOS_UINT32 len;
    VOS_UINT32 i;

    if (smID == VOS_NULL_PTR) {
        VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_INVALID_SMID);
        return (VOS_ERRNO_SEMA4_CCREATE_INVALID_SMID);
    }

    iSemId = VOS_SemCtrlBlkGet();
    if (iSemId == VOS_MAX_SEM_ID_NULL) {
        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
        return (VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
    }

    if (smInit == 0xFFFFFFFF) {
        semId = CreateSemaphore(NULL, (VOS_INT32)1, 1, NULL);
    } else {
        semId = CreateSemaphore(NULL, (VOS_INT32)smInit, 0x400000, NULL);
    }

    if (semId == NULL) {
        VOS_SemCtrlBlkFree(iSemId);
        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
        return (VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
    } else {
        *smID = (VOS_SEM)iSemId;
        len = VOS_StrNLen(smName, VOS_SEM_NAME_PARA_LENGTH);
        if (len > VOS_NULL) {
            for (i = 0; i < len; i++) {
                iSemId->name[i] = smName[i];
            }
            iSemId->name[len] = '\0';
        } else {
            iSemId->name[0] = '\0';
        }

        if (smInit == 0xFFFFFFFF) {
            iSemId->semType = VOS_SEM_TYPE_MUTEX;
        } else {
            iSemId->semType = VOS_SEM_TYPE_COUNT;
        }

        iSemId->semFlags = (int)flags;
        iSemId->semInitCount = (int)smInit;
        iSemId->win32Handle = semId;

        return (VOS_OK);
    }
}

/*
 * Description: To create a Mutex semaphore, can be null
 */
VOS_UINT32 VOS_SmMCreate(const VOS_CHAR *smName, VOS_UINT32 flags, VOS_SEM *smId)
{
    return (VOS_SmCCreate(smName, 0xFFFFFFFF, flags, smId));
}

/*
 * Description: To create a counting semaphore;
 */
VOS_UINT32 VOS_SmCreate(const VOS_CHAR *smName, VOS_UINT32 smInit, VOS_UINT32 flags, VOS_SEM *smId)
{
    return (VOS_SmCCreate(smName, smInit, flags, smId));
}

/*
 * Description: To create a Binary semaphore;
 */
VOS_UINT32 VOS_SmBCreate(const VOS_CHAR *smName, VOS_UINT32 smInit, VOS_UINT32 flags, VOS_SEM *smId)
{
    SEM_ControlBlock *iSemId = VOS_NULL_PTR;
    HANDLE semId;
    VOS_INT32 initState;
    VOS_UINT32 len;
    VOS_UINT32 i;

    if (smInit >= 1) {
        initState = 1;
    } else {
        initState = 0;
    }

    iSemId = VOS_SemCtrlBlkGet();
    if (iSemId == VOS_MAX_SEM_ID_NULL) {
        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
        return (VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
    }

    semId = CreateSemaphore(NULL, initState, (VOS_INT32)1, NULL);
    if (semId == NULL) {
        VOS_SemCtrlBlkFree(iSemId);
        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
        return (VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);
    } else {
        *smId = (VOS_SEM)iSemId;
        len = VOS_StrNLen(smName, VOS_SEM_NAME_PARA_LENGTH);
        if (len > VOS_NULL) {
            for (i = 0; i < len; i++) {
                iSemId->name[i] = smName[i];
            }
            iSemId->name[len] = '\0';
        } else {
            iSemId->name[0] = '\0';
        }

        iSemId->semType = VOS_SEM_TYPE_BINARY;
        iSemId->semFlags = (int)flags;
        iSemId->semInitCount = (int)smInit;
        iSemId->win32Handle = semId;

        return (VOS_OK);
    }
}

/*
 * Description: to delete the exiting semaphore according to the ulSmID
 */
VOS_UINT32 VOS_SmDelete(VOS_SEM smId)
{
    SEM_ControlBlock *tempPtr = VOS_NULL_PTR;

    if (smId == VOS_NULL) {
        VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_INVALID_SMID);
        return (VOS_ERRNO_SEMA4_CCREATE_INVALID_SMID);
    }

    tempPtr = (SEM_ControlBlock *)smId;

    if (tempPtr == tempPtr->semId) {
        if (tempPtr->flag == VOS_SEM_CTRL_BLK_IDLE) {
            (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_OBJTFULL);

            return (VOS_ERRNO_SEMA4_CCREATE_INVALID_SMID);
        }

        if (CloseHandle(tempPtr->win32Handle) != 0) {
            return VOS_SemCtrlBlkFree(tempPtr);
        } else {
            (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_OSALCREATEERR);
            return (VOS_ERRNO_SEMA4_CCREATE_OSALCREATEERR);
        }
    } else {
        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_SEMA4_CCREATE_INVALID_SMID);
        return (VOS_ERRNO_SEMA4_CCREATE_INVALID_SMID);
    }
}

/*
 * Description: Lock the resource for synchronization, if the resource is none
 * then block, otherwise the number of the resource --
 */
VOS_UINT32 VOS_SmP(VOS_SEM smId, VOS_UINT32 timeOutInMillSec)
{
    SEM_ControlBlock *tempPtr = VOS_NULL_PTR;
    VOS_UINT32 timeOut;
    VOS_UINT32 ulerror;

    if (smId == VOS_NULL) {
        return (VOS_ERRNO_SEMA4_P_IDERR);
    }

    tempPtr = (SEM_ControlBlock *)smId;

    if (tempPtr != tempPtr->semId) {
        return (VOS_ERRNO_SEMA4_P_IDERR);
    }

    if (tempPtr->flag == VOS_SEM_CTRL_BLK_IDLE) {
        return (VOS_ERRNO_SEMA4_P_NOTACTIVE);
    }

    if (timeOutInMillSec == 0) {
        timeOut = INFINITE;
    } else {
        timeOut = timeOutInMillSec;
    }

    ulerror = WaitForSingleObject(tempPtr->win32Handle, timeOut);
    if (ulerror == WAIT_OBJECT_0) {
        return VOS_OK;
    }

    if (ulerror == WAIT_TIMEOUT) {
        return VOS_ERRNO_SEMA4_P_TIMEOUT;
    }

    return VOS_ERRNO_SEMA4_P_CANOTP;
}

/*
 * Description: Release/Unlock the sema4 that has locked a resource
 */
VOS_UINT32 VOS_SmV(VOS_SEM smId)
{
    SEM_ControlBlock *tempPtr = VOS_NULL_PTR;
    VOS_UINT32 ulerror;

    if (smId == VOS_NULL) {
        return (VOS_ERRNO_SEMA4_V_IDERR);
    }

    tempPtr = (SEM_ControlBlock *)smId;

    if (tempPtr == tempPtr->semId) {
        if (tempPtr->flag == VOS_SEM_CTRL_BLK_IDLE) {
            return (VOS_ERRNO_SEMA4_V_NOTACTIVE);
        }

        ulerror = (VOS_UINT32)ReleaseSemaphore(tempPtr->win32Handle, 1, NULL);
        if (ulerror == 0) {
            return (VOS_ERRNO_SEMA4_V_NOTACTIVE);
        } else {
            return VOS_OK;
        }
    } else {
        return (VOS_ERRNO_SEMA4_V_IDERR);
    }
}

/*
 * Description: to lock the resource asynchronous
 */
VOS_UINT32 VOS_Sm_AsyP(VOS_SEM smId)
{
    return (VOS_SmP(smId, 1));
}

/*
 * Description: print the usage info of Sem
 */
VOS_VOID VOS_show_sem_info(VOS_VOID)
{
    mdrv_debug("<VOS_show_sem_info> Max be used sem is %x.\n", g_vosSemMaxSemId);
}

#endif

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
