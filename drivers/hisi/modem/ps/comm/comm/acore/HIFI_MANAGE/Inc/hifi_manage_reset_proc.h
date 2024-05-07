/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
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

#ifndef __HIFI_MANAGE_RESET_PROC_H__
#define __HIFI_MANAGE_RESET_PROC_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include "vos.h"
#include "v_msg.h"
#include "NasHifiManageInterface.h"

/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
#pragma pack(*)    设置字节对齐方式
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif
#define HIFI_MANAGE_SUCC                       0
#define HIFI_MANAGE_FAIL                       1
#define HIFI_MANAGE_TRUE                       1
#define HIFI_MANAGE_FALSE                      0

#define HIFI_MANAGE_WDT_HIFI_INT  117

#define HIFI_MANAGE_WDG_BASE_ADDR         0xE4003000    /*A核上看到的hifi watchdog的基地址*/
#define HIFI_MANAGE_WDG_LOAD_ADDR         (HIFI_MANAGE_WDG_BASE_ADDR + 0x0000)   /*计数初始值寄存器*/
#define HIFI_MANAGE_WDG_VALUE_ADDR        (HIFI_MANAGE_WDG_BASE_ADDR + 0x0004)   /*计数当前值寄存器*/
#define HIFI_MANAGE_WDG_CONTROL_ADDR      (HIFI_MANAGE_WDG_BASE_ADDR + 0x0008)   /*watchdog的控制寄存器*/
#define HIFI_MANAGE_WDG_INTCLR_ADDR       (HIFI_MANAGE_WDG_BASE_ADDR + 0x000C)   /*中断清除寄存器*/
#define HIFI_MANAGE_WDG_RIS_ADDR          (HIFI_MANAGE_WDG_BASE_ADDR + 0x0010)   /*原始中断寄存器*/
#define HIFI_MANAGE_WDG_MIS_ADDR          (HIFI_MANAGE_WDG_BASE_ADDR + 0x0014)   /*屏蔽后中断寄存器*/
#define HIFI_MANAGE_WDG_LOCK_ADDR         (HIFI_MANAGE_WDG_BASE_ADDR + 0x0C00)   /*控制watchdog读写权限的寄存器*/

#define HIFI_MANAGE_WDG_UNLOCK_NUM        0x1ACCE551   /*打开寄存器的写权限*/
#define HIFI_MANAGE_WDG_LOCK_NUM          0x0          /*关闭寄存器的写权限*/
#define HIFI_MANAGE_WDG_CONTROL_DISABLE   0x0          /*计数器停止计数*/
#define HIFI_MANAGE_WDG_INTCLR_NUM        0x4455       /*清除watchdog中断，并重新计数*/

#define HIFI_MANAGE_SYSCTRL_PERI_BASE_ADDR 0xE4001000  /*sysctrl peri的基地址*/
#define HIFI_MANAGE_ACORE_HIFI_NMI         (HIFI_MANAGE_SYSCTRL_PERI_BASE_ADDR + 0x44c) /*SC_TOP_CTRL19 触发NMI中断*/
#define HIFI_MANAGE_ACORE_HIFI_NMI_ENABLE  0x1
#define HIFI_MANAGE_ACORE_HIFI_NMI_DISABLE  0x0


#define HIFI_MANAGE_TMR_ID_EXEC_PROC_TMR   0x0   /* 异常处理定时器 */
#define HIFI_MANAGE_TMR_ID_LOAD_CHECK_TMR   0x1   /* 异常处理定时器 */

#define HIFI_MANAGE_MAX_EXEC_NUM  3   /* 逃生处理的异常次数 */

#define HIFI_MANAGE_RST_MAX_TIME  5000 //(5 * 32768 / 1000)    //5m秒内3次异常，触发modem复位
#define HIFI_MANAGE_LOAD_MAX_TIME  10000   //底软加载完成监控时间

#define HIFI_MANAGE_HIFI_DSP_SYS_ERR   0xAFFFF001       //hifi触发modem复位的modid

#define HIFI_MANAGE_NMI_WAIT_TIME_EXTRA_MODEM 100     //等待100ms
#define HIFI_MANAGE_NMI_WAIT_TIME_MBB 200     //等待200ms


#define HIFI_MANAGE_RESET_CBFUNC_PRIO_LEVEL_LOWT    0
#define HIFI_MANAGE_RESET_CBFUNC_PRIO_LEVEL_HIGH    49

#define HIFI_MANAGE_MODULE_NAME_LEN                 9  /* exclude '\0' */
#define HIFI_MANAGE_RESET_CBFUN_IGNORE_NAME         "NONAME"

enum HIFI_MANAGE_HifiManageToSelfMsg {
    ID_HIFI_MANAGE_HIFI_EXCEPTION_IND = 0x0001,
    ID_HIFI_MANAGE_HIFI_IMG_LOAD_COMPLETE_IND = 0x0002,
    ID_HIFI_MANAGE_MSG_BUTT
};
typedef VOS_UINT32 HIFI_MANAGE_HifiManageToSelfMsgUint32;

enum HIFI_MANAGE_HifiImgLoadResult {
    HIFI_MANAGE_HIFI_IMG_LOAD_OK = 0x00,
    HIFI_MANAGE_HIFI_IMG_LOAD_FAILED = 0x01,

    HIFI_MANAGE_HIFI_IMG_LOAD_BUTT
};
typedef VOS_UINT8 HIFI_MANAGE_HifiImgLoadResultUint8;

typedef struct {
    VOS_UINT32 * controlAddr;
    VOS_UINT32 * lockAddr;
    VOS_UINT32 * intclrAddr;
} HIFI_MANAGE_WdgData;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT32 timeStamp;
} HIFI_MANAGE_HifiExceptionMsg;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT32 loadResult;
} HIFI_MANAGE_HifiImgLoadResultInd;

typedef struct {
    HTIMER     timrer;          /* 定时器 */
    VOS_UINT32 Period;        /* 统计周期,单位ms */
    VOS_UINT32 exceNum;      /*异常次数 */
} HIFI_MANAGE_TimerCtrl;

struct HIFI_MANAGE_ResetCallbackFuncData {
    char name[HIFI_MANAGE_MODULE_NAME_LEN + 1];
    int prioLevel;
    HIFI_MANAGE_HifiResetCallbackFunc cbfun;
    int userData;
};

/*Link list unit of managing all callback functions*/
struct HIFI_MANAGE_ResetCallbackFuncNode {
    struct HIFI_MANAGE_ResetCallbackFuncData cbfuninfo;
    struct HIFI_MANAGE_ResetCallbackFuncNode *pnext;
};

VOS_VOID HIFI_MANAGE_ResetInit(VOS_VOID);
VOS_VOID HIFI_MANAGE_HifiExceptionProc(MsgBlock* pMsg);
VOS_VOID HIFI_MANAGE_HifiImgLoadResultProc(MsgBlock* pMsg);
irqreturn_t HIFI_MANAGE_WatchdogIrqHandler(int irq, void* data);
VOS_UINT32 HIFI_MANAGE_LoadImgCompleteProc(VOS_UINT32 procResult);
VOS_VOID HIFI_MANAGE_RunCbFunc(enum HIFI_MANAGE_ResetCallbackFuncMoment eparam);
VOS_UINT32 HIFI_MANAGE_EscapeTmrStart(VOS_VOID);
VOS_VOID HIFI_MANAGE_EscapeTmrTimeoutProc(VOS_VOID);
VOS_VOID HIFI_MANAGE_LoadFailTmrProc(VOS_VOID);
VOS_VOID HIFI_MANAGE_LoadCheckTmrStart(VOS_VOID);


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

#endif /* end of __HIFI_MANAGE_RESET_PROC_H__ */

