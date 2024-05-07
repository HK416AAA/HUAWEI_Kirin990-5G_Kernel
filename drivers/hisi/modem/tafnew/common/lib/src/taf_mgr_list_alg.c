/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#include "taf_mgr_list_alg.h"
#include "ps_common_def.h"

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
#include "nas_multi_instance_api.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_TAF_MGR_LIST_ALG_C

VOS_UINT32 TAF_ListGetMgrListSize(const TAF_ListMgrInfo *mgrInfo)
{
    HI_LIST_S *me = VOS_NULL_PTR;
    VOS_UINT32 size = 0;

    msp_list_for_each(me, &(mgrInfo->listHead))
    {
        size++;
    }

    return size;
}

VOS_UINT32 TAF_ListIsMgrListEmpty(const TAF_ListMgrInfo *mgrInfo)
{
    const HI_LIST_S *listHead = VOS_NULL_PTR;

    listHead = &(mgrInfo->listHead);

    if (msp_list_empty(listHead) == VOS_TRUE) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_VOID* TAF_ListAllocMgrEntry(const VOS_UINT32 pid, const VOS_UINT32 length)
{
    VOS_VOID *entry = VOS_NULL_PTR;
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
    VOS_UINT32    realPid;
    ModemIdUint16 modemId;

    realPid = pid;
    modemId = NAS_MULTIINSTANCE_GetCurrInstanceModemId(pid);
    realPid = NAS_MULTIINSTANCE_GetSpecModemPid(modemId, realPid);

#ifdef DMT
    entry = (VOS_VOID *)PS_MEM_ALLOC_WITHOUT_LEAK_MONITOR(realPid, length);
#else
    entry = (VOS_VOID *)PS_MEM_ALLOC(realPid, length);  //lint !e516
#endif

#else

#ifdef DMT
    entry = (VOS_VOID *)PS_MEM_ALLOC_WITHOUT_LEAK_MONITOR(pid, length);
#else
    entry = (VOS_VOID *)PS_MEM_ALLOC(pid, length);  //lint !e516
#endif

#endif

    return entry;
}

VOS_VOID TAF_ListFreeMgrEntry(const VOS_UINT32 pid, VOS_VOID *entry)
{
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
    VOS_UINT32    realPid;
    ModemIdUint16 modemId;

    realPid = pid;
    modemId = NAS_MULTIINSTANCE_GetCurrInstanceModemId(pid);
    realPid = NAS_MULTIINSTANCE_GetSpecModemPid(modemId, realPid);
#endif

#if (OSA_CPU_CCPU == VOS_OSA_CPU)

#ifdef DMT
    PS_MEM_FREE_WITHOUT_LEAK_MONITOR(realPid, entry);
    entry = VOS_NULL_PTR;
#else
    PS_MEM_FREE(realPid, entry); //lint !e424 !e516
#endif

#else

#ifdef DMT
    PS_MEM_FREE_WITHOUT_LEAK_MONITOR(pid, entry);
    entry = VOS_NULL_PTR;
#else
    PS_MEM_FREE(pid, entry); //lint !e424 !e516
#endif

#endif
}

VOS_VOID TAF_ListAddMgrEntry(HI_LIST_S *list, TAF_ListMgrInfo *mgrInfo)
{
    msp_list_add(list, &(mgrInfo->listHead));
}

VOS_VOID TAF_ListDeleteMgrEntry(const VOS_UINT32 pid, HI_LIST_S *list, VOS_VOID *entry)
{
    msp_list_del(list);
    TAF_ListFreeMgrEntry(pid, entry);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
