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
 * 功能描述: OSA win32操作系统上任务功能实现
 * 生成日期: 2006年10月3日
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "vos_config.h"
#include "v_typdef.h"
#include "v_task.h"
#include "stdlib.h"
#include "stdio.h"
#include "v_io.h"
#include "v_int.h"
#include "vos_id.h"
#include "v_sem.h"
#include "v_private.h"
#include "mdrv.h"
#include "pam_tag.h"

#if (VOS_WIN32 == VOS_OS_VER)

#define THIS_FILE_ID PS_FILE_ID_V_WIN32_TASK_C
#define THIS_MODU mod_pam_osa

#ifdef LLT_OS_WIN

typedef struct {
    int                 flag;
    int                 tid;      /* task ID */
    VOS_TaskEntryType function; /* the entry of task */
    VOS_UINT32          priority;
    VOS_UINT32          stackSize;
    VOS_UINT32          args[VOS_TARG_NUM]; /* the argument of the entry */
    HANDLE              win32Handle;        /* maped win32 handle */
    VOS_UINT32          win32ThreadId;    /* maped win32 thread id */
    VOS_CHAR            name[VOS_MAX_LENGTH_TASK_NAME];
    VOS_UINT32          eventIsCreate; /* event create or not */
    VOS_UINT32          events;        /* current event */
    VOS_UINT32          expects;       /* expected event */
    VOS_UINT32          receives;      /* have been received event */
    VOS_UINT32          flags;         /* event mode */
    VOS_UINT32          fid;           /* belong to a FID or not */
} VOS_TaskControlBlock;

/* the number of task's control block */
VOS_UINT32 g_vosTaskCtrlBlkNumber;

/* the start address of task's control block */
VOS_TaskControlBlock *g_vosTaskCtrlBlk = VOS_NULL_PTR;

#define VOS_TASK_CTRL_BUF_SIZE (sizeof(VOS_TaskControlBlock) * VOS_MAX_TASK_NUMBER)

VOS_CHAR g_vosTaskCtrlBuf[VOS_TASK_CTRL_BUF_SIZE];

/*
 * Description: Init task's control block
 */
VOS_VOID VOS_TaskCtrlBlkInit(VOS_VOID)
{
    int i;

    g_vosTaskCtrlBlkNumber = VOS_MAX_TASK_NUMBER;

    g_vosTaskCtrlBlk = (VOS_TaskControlBlock *)g_vosTaskCtrlBuf;

    for (i = 0; i < (int)g_vosTaskCtrlBlkNumber; i++) {
        g_vosTaskCtrlBlk[i].flag            = VOS_TASK_CTRL_BLK_IDLE;
        g_vosTaskCtrlBlk[i].tid             = i;
        g_vosTaskCtrlBlk[i].win32Handle     = VOS_NULL_PTR;
        g_vosTaskCtrlBlk[i].eventIsCreate = VOS_FALSE;
        g_vosTaskCtrlBlk[i].events        = 0;
        g_vosTaskCtrlBlk[i].expects       = 0;
        g_vosTaskCtrlBlk[i].receives      = 0;
        g_vosTaskCtrlBlk[i].flags         = VOS_EVENT_NOWAIT;
        g_vosTaskCtrlBlk[i].fid           = 0xffffffff;
    }
    return;
}

/*
 * Description: allocate a block
 */
VOS_UINT32 VOS_TaskCtrlBlkGet(VOS_VOID)
{
    VOS_UINT32 i;
    int        intLockLevel;

    intLockLevel = VOS_SplIMP();

    for (i = 0; i < g_vosTaskCtrlBlkNumber; i++) {
        if (g_vosTaskCtrlBlk[i].flag == VOS_TASK_CTRL_BLK_IDLE) {
            g_vosTaskCtrlBlk[i].flag = VOS_TASK_CTRL_BLK_BUSY;

            break;
        }
    }

    VOS_Splx(intLockLevel);

    if (i < g_vosTaskCtrlBlkNumber) {
        return i;
    } else {
        mdrv_err("<VOS_TaskCtrlBlkGet> allocate task control block fail.\n");

        return (VOS_TASK_CTRL_BLK_NULL);
    }
}

/*
 * Description: free a block
 */
VOS_UINT32 VOS_TaskCtrlBlkFree(VOS_UINT32 tid)
{
    int intLockLevel;

    if (tid < g_vosTaskCtrlBlkNumber) {
        if (g_vosTaskCtrlBlk[tid].flag == VOS_TASK_CTRL_BLK_IDLE) {
            mdrv_err("<VOS_TaskCtrlBlkFree> free Idle Task.\n");

            return VOS_ERR;
        } else {
            intLockLevel              = VOS_SplIMP();
            g_vosTaskCtrlBlk[tid].flag = VOS_TASK_CTRL_BLK_IDLE;
            VOS_Splx(intLockLevel);

            return VOS_OK;
        }
    } else {
        mdrv_err("<VOS_TaskCtrlBlkFree> Tid exceed TaskCtrlBlkNumber.\n");

        return VOS_ERR;
    }
}

/*
 * Description: Map WIN32 priority to OSAL priority
 */
VOS_UINT32 VOS_WIN32PriMap(VOS_UINT32 taskPriority)
{
    taskPriority >>= 5;
    switch (taskPriority) {
        case 0:
            taskPriority = (VOS_UINT32)THREAD_PRIORITY_IDLE;
            break;

        case 1:
            taskPriority = (VOS_UINT32)THREAD_PRIORITY_LOWEST;
            break;

        case 2:
            taskPriority = (VOS_UINT32)THREAD_PRIORITY_BELOW_NORMAL;
            break;

        case 3:
            taskPriority = (VOS_UINT32)THREAD_PRIORITY_NORMAL;
            break;

        case 4:
            taskPriority = (VOS_UINT32)THREAD_PRIORITY_ABOVE_NORMAL;
            break;

        case 5:
            taskPriority = (VOS_UINT32)THREAD_PRIORITY_HIGHEST;
            break;

        case 6:
        case 7:
        default:
            taskPriority = (VOS_UINT32)THREAD_PRIORITY_TIME_CRITICAL;
            break;
    }
    return taskPriority;
}

/*
 * Description: Get current task's ID
 */

VOS_VOID VOS_Win32TaskEntry(VOS_VOID *arg)
{
    VOS_TaskControlBlock *temp = VOS_NULL_PTR;
    VOS_UINT32              para1;
    VOS_UINT32              para2;
    VOS_UINT32              para3;
    VOS_UINT32              para4;

    temp = (VOS_TaskControlBlock *)arg;

    para1 = temp->args[0];
    para2 = temp->args[1];
    para3 = temp->args[2];
    para4 = temp->args[3];

    temp->function(para1, para2, para3, para4);
}

/*
 * Description: create task with default task mode:
 */
VOS_UINT32 VOS_CreateTaskOnly(const VOS_CHAR *puchName, VOS_UINT32 *taskID, VOS_TaskEntryType pfnFunc,
                              VOS_UINT32 priority, VOS_UINT32 stackSize, const VOS_UINT32 aulArgs[VOS_TARG_NUM])
{
    int        i;
    VOS_UINT32 iTid;
    HANDLE     temp;
    VOS_CHAR   defaultName[8];
    VOS_UCHAR  value;
    VOS_UINT32 osTaskPriority;

    if (taskID == VOS_NULL_PTR) {
        VOS_SetErrorNo(VOS_ERRNO_TASK_CREATE_INPUTTIDISNULL);
        return( VOS_ERRNO_TASK_CREATE_INPUTTIDISNULL );
    }

    if (pfnFunc == VOS_NULL_PTR) {
        VOS_SetErrorNo(VOS_ERRNO_TASK_CREATE_INPUTENTRYISNULL);
        return( VOS_ERRNO_TASK_CREATE_INPUTENTRYISNULL );
    }

    iTid = VOS_TaskCtrlBlkGet();
    if (iTid == (VOS_UINT32)VOS_TASK_CTRL_BLK_NULL) {
        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_TASK_CREATE_NOFREETCB);
        return (VOS_ERRNO_TASK_CREATE_NOFREETCB);
    }

    /* caller not use name asign a default name */
    if (VOS_NULL_PTR == puchName) {
        defaultName[0] = 't';
        defaultName[1] = 'i';
        defaultName[2] = 'd';
        defaultName[3] = (VOS_CHAR)(48 + iTid / 100);
        value           = (VOS_UCHAR)(iTid % 100);
        defaultName[4] = (VOS_CHAR)(48 + value / 10);
        defaultName[5] = (VOS_CHAR)(48 + value % 10);
        defaultName[6] = '\0';
    }

    *taskID = iTid;

    if (VOS_NULL_PTR != puchName) {
        if (memcpy_s(g_vosTaskCtrlBlk[iTid].name, VOS_MAX_LENGTH_TASK_NAME, puchName,
                     VOS_StrNLen(puchName, VOS_MAX_LENGTH_TASK_NAME)) != EOK) {
            mdrv_om_system_error(VOS_REBOOT_MEMCPY_MEM, (VOS_INT)VOS_StrNLen(puchName, VOS_MAX_LENGTH_TASK_NAME),
                                 (VOS_INT)((THIS_FILE_ID << 16) | __LINE__), 0, 0);
        }

        g_vosTaskCtrlBlk[iTid].name[VOS_MAX_LENGTH_TASK_NAME - 1] = '\0';
    } else {
        g_vosTaskCtrlBlk[iTid].name[0] = defaultName[0];
        g_vosTaskCtrlBlk[iTid].name[1] = defaultName[1];
        g_vosTaskCtrlBlk[iTid].name[2] = defaultName[2];
        g_vosTaskCtrlBlk[iTid].name[3] = defaultName[3];
        g_vosTaskCtrlBlk[iTid].name[4] = defaultName[4];
        g_vosTaskCtrlBlk[iTid].name[5] = defaultName[5];
        g_vosTaskCtrlBlk[iTid].name[6] = defaultName[6];
    }

    g_vosTaskCtrlBlk[iTid].function  = pfnFunc;
    g_vosTaskCtrlBlk[iTid].priority  = priority;
    g_vosTaskCtrlBlk[iTid].stackSize = stackSize;

    for (i = 0; i < VOS_TARG_NUM; i++) {
        g_vosTaskCtrlBlk[iTid].args[i] = aulArgs[i];
    }

    temp = CreateThread(VOS_NULL_PTR, stackSize, (LPTHREAD_START_ROUTINE)VOS_Win32TaskEntry,
                           (VOS_VOID *) &(g_vosTaskCtrlBlk[iTid]), CREATE_SUSPENDED,
                           &(g_vosTaskCtrlBlk[iTid].win32ThreadId));
    if (VOS_NULL_PTR == temp) {
        VOS_TaskCtrlBlkFree(iTid);
        (VOS_VOID)VOS_SetErrorNo(VOS_ERRNO_TASK_CREATE_OSALCREATEFAIL);
        return (VOS_ERRNO_TASK_CREATE_OSALCREATEFAIL);
    }

    g_vosTaskCtrlBlk[iTid].win32Handle = temp;

    osTaskPriority = VOS_WIN32PriMap(priority);
    if (VOS_NULL == SetThreadPriority(temp, (VOS_INT)osTaskPriority)) {
        return VOS_ERR;
    }

    return VOS_OK;
}

/*
 * Description: create task with default task mode:
 */
VOS_UINT32 VOS_CreateTask(const VOS_CHAR *puchName, VOS_UINT32 *taskID, VOS_TaskEntryType pfnFunc,
    VOS_UINT32 priority, VOS_UINT32 stackSize, const VOS_UINT32 aulArgs[VOS_TARG_NUM])
{
    VOS_UINT32 result;
    VOS_UINT32 win32Handle;

    if (taskID == VOS_NULL_PTR) {
        VOS_SetErrorNo(VOS_ERRNO_TASK_CREATE_INPUTTIDISNULL);
        return( VOS_ERRNO_TASK_CREATE_INPUTTIDISNULL );
    }

    if (pfnFunc == VOS_NULL_PTR) {
        VOS_SetErrorNo(VOS_ERRNO_TASK_CREATE_INPUTENTRYISNULL);
        return( VOS_ERRNO_TASK_CREATE_INPUTENTRYISNULL );
    }

    result = VOS_CreateTaskOnly(puchName, taskID, pfnFunc, priority, stackSize, aulArgs);
    if (VOS_OK != result) {
        mdrv_err("<VOS_CreateTask>: CreateTaskOnly Fail\n");
        return result;
    }

    win32Handle = g_vosTaskCtrlBlk[*taskID].win32Handle;

    result = ResumeThread(win32Handle);
    if (VOS_NULL_DWORD == result) {
        mdrv_err("<VOS_CreateTask> ResumeThread Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}

#ifdef __OS_RTOSCK_SMP__
/*
 * Description: CoreBind a task
 */
VOS_UINT32 VOS_CoreBindTask(VOS_UINT32 taskID, VOS_UINT32 coreMask)
{
    return VOS_OK;
}
#endif

/*
 * Description: Suspend a task
 */
VOS_UINT32 VOS_SuspendTask(VOS_UINT32 taskID)
{
    HANDLE temp;

    if (taskID >= g_vosTaskCtrlBlkNumber) {
        return (VOS_ERR);
    } else {
        temp = g_vosTaskCtrlBlk[taskID].win32Handle;
        if (VOS_NULL_PTR == temp) {
            return (VOS_ERR);
        }
    }

    if (VOS_NULL_DWORD == SuspendThread(temp)) {
        mdrv_err("<VOS_SuspendTask> taskSuspend Fail.\n");
        return (VOS_ERR);
    }

    return VOS_OK;
}

/*
 * Description: Resume a task
 */
VOS_UINT32 VOS_ResumeTask(VOS_UINT32 taskID)
{
    HANDLE temp;

    if (taskID >= g_vosTaskCtrlBlkNumber) {
        return (VOS_ERR);
    } else {
        temp = g_vosTaskCtrlBlk[taskID].win32Handle;
        if (VOS_NULL_PTR == temp) {
            return (VOS_ERR);
        }
    }

    if (VOS_NULL_DWORD == ResumeThread(temp)) {
        mdrv_err("<VOS_ResumeTask> taskSuspend fail.\n");
        return (VOS_ERR);
    }

    return VOS_OK;
}

/*
 * Description: Let the task sleep specified millseconds, only tick precision
 */
VOS_UINT32 VOS_TaskDelay(VOS_UINT32 millSecs)
{
    Sleep(millSecs);
    return VOS_OK;
}

/*
 * Description: forbid task attemper.
 */
VOS_UINT32 VOS_TaskLock(VOS_VOID)
{
    VOS_SplIMP();

    return VOS_OK;
}

/*
 * Description: permit task attemper.
 */
VOS_UINT32 VOS_TaskUnlock(VOS_VOID)
{
    VOS_Splx(0);

    return VOS_OK;
}

/*
 * Description: Delete a task, on some platform this operation is prohibited.
 */
VOS_UINT32 VOS_DeleteTask(VOS_UINT32 taskID)
{
    return (VOS_OK);
}

/*
 * Description: Get current task's ID
 */
VOS_UINT32 VOS_GetCurrentTaskID(VOS_VOID)
{
    VOS_UINT32 osid;
    int        i;
    int        intLockLevel;

    osid = (VOS_UINT32)GetCurrentThreadId();
    if (VOS_NULL_LONG == osid) {
        return (VOS_NULL_LONG);
    }

    intLockLevel = VOS_SplIMP();

    for (i = 0; i < (VOS_INT)g_vosTaskCtrlBlkNumber; i++) {
        if (VOS_TASK_CTRL_BLK_BUSY == g_vosTaskCtrlBlk[i].flag) {
            if (osid == g_vosTaskCtrlBlk[i].win32ThreadId) {
                VOS_Splx(intLockLevel);

                return (VOS_UINT32)i;
            }
        }
    }

    VOS_Splx(intLockLevel);

    mdrv_err("<VOS_GetCurrentTaskID> Current Task ID not found.\n");

    return (VOS_NULL_LONG);
}

/*
 * Description: Get current FID
 */
VOS_UINT32 VOS_GetCurrentFID(VOS_VOID)
{
    VOS_UINT32 osid;
    int        i;
    int        intLockLevel;

    osid = (VOS_UINT32)GetCurrentThreadId();
    if (VOS_NULL_LONG == osid) {
        return (VOS_NULL_LONG);
    }

    intLockLevel = VOS_SplIMP();

    for (i = 0; i < (VOS_INT)g_vosTaskCtrlBlkNumber; i++) {
        if (VOS_TASK_CTRL_BLK_BUSY == g_vosTaskCtrlBlk[i].flag) {
            if (osid == g_vosTaskCtrlBlk[i].win32ThreadId) {
                VOS_Splx(intLockLevel);

                return g_vosTaskCtrlBlk[i].fid;
            }
        }
    }

    VOS_Splx(intLockLevel);

    mdrv_err("<VOS_GetCurrentTaskID> Current FID not found.\n");

    return (VOS_NULL_LONG);
}

/*
 * Description:Create resource for the Event.
 */
VOS_UINT32 VOS_CreateEvent(VOS_UINT32 taskID)
{
    if (taskID >= g_vosTaskCtrlBlkNumber) {
        return VOS_ERR;
    }

    if (VOS_TASK_CTRL_BLK_IDLE == g_vosTaskCtrlBlk[taskID].flag) {
        return VOS_ERR;
    }

    g_vosTaskCtrlBlk[taskID].eventIsCreate = VOS_TRUE;
    g_vosTaskCtrlBlk[taskID].events        = 0;
    g_vosTaskCtrlBlk[taskID].expects       = 0;
    g_vosTaskCtrlBlk[taskID].receives      = 0;
    g_vosTaskCtrlBlk[taskID].flags         = VOS_EVENT_NOWAIT;

    return VOS_OK;
}

/*
 * Description: checke a event exists or not
 */
VOS_UINT32 VOS_CheckEvent(VOS_UINT32 taskID)
{
    if (taskID >= g_vosTaskCtrlBlkNumber) {
        return VOS_ERR;
    }

    if (VOS_TASK_CTRL_BLK_IDLE == g_vosTaskCtrlBlk[taskID].flag) {
        return VOS_ERR;
    }

    if (VOS_FALSE == g_vosTaskCtrlBlk[taskID].eventIsCreate) {
        return VOS_ERR;
    }

    if (0xffffffff == g_vosTaskCtrlBlk[taskID].fid) {
        return VOS_ERR;
    }

    return VOS_OK;
}

/*
 * Description: record corresponding of FID&TID
 */
VOS_UINT32 VOS_RecordFIDTIDInfo(VOS_UINT32 fid, VOS_UINT32 tid)
{
    g_vosTaskCtrlBlk[tid].fid = fid;

    return VOS_OK;
}

/*
 * Description:Send Event to the TaskID.
 */
VOS_UINT32 VOS_EventWrite(VOS_UINT32 taskID, VOS_UINT32 events)
{
    /* the functin should be full later */
    int        intLockLevel;
    VOS_UINT32 tempQueue;
    VOS_SEM    tempSem;

    if (VOS_OK != VOS_CheckEvent(taskID)) {
        mdrv_err("<VOS_EventWrite> EVENT not exist.\n");
        return VOS_ERR;
    }

    intLockLevel = VOS_SplIMP();

    g_vosTaskCtrlBlk[taskID].events |= events;

    VOS_Splx(intLockLevel);

    tempQueue = VOS_GetQueueIDFromFid(g_vosTaskCtrlBlk[taskID].fid);
    if (0xffffffff == tempQueue) {
        return VOS_ERR;
    }

    tempSem = VOS_GetSemIDFromQueue(tempQueue);
    if (0xffffffff == tempSem) {
        return VOS_ERR;
    }

    if (VOS_OK != VOS_SmV(tempSem)) {
        mdrv_err("<VOS_EventWrite> VOS_SmV fail.\n");

        return VOS_ERR;
    } else {
        return VOS_OK;
    }
}

/*
 * Description:Read event
 */
VOS_UINT32 VOS_EventRead(VOS_UINT32 events, VOS_UINT32 flags, VOS_UINT32 timeOutInMillSec,
                         VOS_UINT32 *retEvents)
{
    /* the functin should be full later */
    VOS_UINT32 taskSelf; /* Self task ID */
    VOS_UINT32 tempQueue;
    VOS_SEM    tempSem;
    int        intLockLevel;
    VOS_UINT32 tempEvent;

    if (retEvents == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    taskSelf = VOS_GetCurrentTaskID();
    if (0xffffffff == taskSelf) {
        return VOS_ERR;
    }

    if (VOS_EVENT_NOWAIT & flags) {
        mdrv_err("<VOS_EventRead> OSA don't support VOS_EVENT_NOWAIT.\n");

        VOS_SuspendTask(taskSelf);

        return VOS_ERR;
    }

    if (!(VOS_EVENT_ANY & flags)) {
        mdrv_err("<VOS_EventRead> OSA don't support VOS_EVENT_ALL.\n");

        VOS_SuspendTask(taskSelf);

        return VOS_ERR;
    }

    if (0 == events) {
        mdrv_err("<VOS_EventRead> OSA don't support event = 0.\n");

        VOS_SuspendTask(taskSelf);

        return VOS_ERR;
    }

    if (VOS_OK != VOS_CheckEvent(taskSelf)) {
        mdrv_err("<VOS_EventRead> VOS_EventRead EVENT not exist.\n");

        VOS_SuspendTask(taskSelf);

        return VOS_ERR;
    }

    tempQueue = VOS_GetQueueIDFromFid(g_vosTaskCtrlBlk[taskSelf].fid);
    if (0xffffffff == tempQueue) {
        return VOS_ERR;
    }

    tempSem = VOS_GetSemIDFromQueue(tempQueue);
    if (0xffffffff == tempSem) {
        return VOS_ERR;
    }

    if (VOS_OK != VOS_SmP(tempSem, timeOutInMillSec)) {
        mdrv_err("<VOS_EventRead> VOS_SmP Fail\n");
        return VOS_ERR;
    }

    intLockLevel = VOS_SplIMP();

    tempEvent = g_vosTaskCtrlBlk[taskSelf].events & events;

    if (tempEvent) {
        g_vosTaskCtrlBlk[taskSelf].events &= ~tempEvent;
        *retEvents = tempEvent;
    } else {
        *retEvents = VOS_MSG_SYNC_EVENT;
    }

    VOS_Splx(intLockLevel);

    return VOS_OK;
}

/*
 * Description: get the queue ID of a TId
 */
VOS_UINT32 VOS_GetQueueIDFromTID(VOS_UINT32 tid)
{
    if (tid >= g_vosTaskCtrlBlkNumber) {
        mdrv_err("<VOS_GetQueueIDFromTID> TID invaild.\n");

        return 0xffffffff;
    }

    if (0xffffffff == g_vosTaskCtrlBlk[tid].fid) {
        mdrv_err("<VOS_GetQueueIDFromTID> FID invaild.\n");

        return 0xffffffff;
    }

    return VOS_GetQueueIDFromFid(g_vosTaskCtrlBlk[tid].fid);
}

#endif

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
