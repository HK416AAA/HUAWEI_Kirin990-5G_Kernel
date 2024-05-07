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

#ifndef __NAS_HIFIMANAGE_INTERFACE_H__
#define __NAS_HIFIMANAGE_INTERFACE_H__

/*****************************************************************************
  1 头文件包含
*****************************************************************************/

#include "vos.h"
#include "mdrv.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 宏定义
*****************************************************************************/


/*****************************************************************************
  3 枚举定义
*****************************************************************************/
enum HIFI_MANAGE_ResetCallbackFuncMoment {
    HIFI_MANAGE_RESET_CALLCBFUN_RESET_BEFORE,   /*Modem/HIFI 复位前*/
    HIFI_MANAGE_RESET_CALLCBFUN_RESET_AFTER,    /*Modem/HIFI 复位后*/
    HIFI_MANAGE_RESET_CALLCBFUN_RESETING,       /*Modem/HIFI 复位中*/
    HIFI_MANAGE_RESET_CALLCBFUN_MOEMENT_INVALID
};


/*****************************************************************************
  4 STRUCT定义
*****************************************************************************/
/*Record information of callback functions*/
typedef int (*HIFI_MANAGE_HifiResetCallbackFunc)(enum HIFI_MANAGE_ResetCallbackFuncMoment eparam, int userData);

#if (FEATURE_HIFI_MANAGE == FEATURE_ON)
extern int HIFI_MANAGE_RegResetCallbackFunc(const char * name, HIFI_MANAGE_HifiResetCallbackFunc cbFun, int userData, int prioLevel);
#else
static inline int HIFI_MANAGE_RegResetCallbackFunc(const char * name, HIFI_MANAGE_HifiResetCallbackFunc cbFun, int userData, int prioLevel)
{
    return 0;
}
#endif
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __NAS_HIFIMANAGE_INTERFACE_H__ */

