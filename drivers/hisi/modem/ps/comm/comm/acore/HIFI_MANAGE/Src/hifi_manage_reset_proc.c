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

#include "hifi_manage_reset_proc.h"
#include <linux/list.h>
#include <linux/delay.h>
#include <securec.h>
#include <of.h>
#include "vos.h"
#include "PsTypeDef.h"
#include "v_msg.h"
#include "hifi_manage_msg_proc.h"
#include "NasHifiManageInterface.h"
#include "HifiManageCodecInterface.h"
#include "VoiceAgentHifiManageInterface.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_om.h"
#endif
#include "mdrv_hifi_reset.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/

#define THIS_FILE_ID                    PS_FILE_ID_DMS_CORE_C

HIFI_MANAGE_WdgData g_hifiManageWatchdog = {0};
struct HIFI_MANAGE_ResetCallbackFuncNode *g_hifiManageResetData = VOS_NULL;
HIFI_MANAGE_TimerCtrl g_hifiManageTimerCtrl = {0};
HTIMER g_hifiManageLoadTimer = 0;

VOS_VOID HIFI_MANAGE_ResetInit(VOS_VOID)
{
    VOS_INT32 ret = 0;
    struct device_node *node = NULL;
    VOS_INT32 irq = 0;

    g_hifiManageTimerCtrl.exceNum = 0;

    g_hifiManageWatchdog.controlAddr = (VOS_UINT32 *)ioremap((unsigned long)HIFI_MANAGE_WDG_CONTROL_ADDR, (unsigned long)0x4);
    if (g_hifiManageWatchdog.controlAddr == VOS_NULL) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_ResetInit remap control failed");
        goto error;
    }

    g_hifiManageWatchdog.intclrAddr = (VOS_UINT32 *)ioremap((unsigned long)HIFI_MANAGE_WDG_INTCLR_ADDR, (unsigned long)0x4);
    if (g_hifiManageWatchdog.intclrAddr == VOS_NULL) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_ResetInit remap intclr failed");
        goto error;
    }

    g_hifiManageWatchdog.lockAddr = (VOS_UINT32 *)ioremap((unsigned long)HIFI_MANAGE_WDG_LOCK_ADDR, (unsigned long)0x4);
    if (g_hifiManageWatchdog.lockAddr == VOS_NULL) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_ResetInit remap lock failed");
        goto error;
    }
    /*调用底软的接口，注册HIFI镜像加载完成的回调函数*/
    ret = mdrv_hifi_reset_cb_register((hifi_cb_func)HIFI_MANAGE_LoadImgCompleteProc);
    if (ret != 0) {
        HIFI_MANAGE_WARN_LOG("register hifi reset cb func failed");
    }


    node = of_find_compatible_node(NULL, NULL, "hisilicon,watchdog_hifi");
    if (node == NULL) {
        HIFI_MANAGE_ERR_LOG("wdt hifi node not found!\n");
        return;
    }

    irq = (VOS_UINT32)irq_of_parse_and_map(node, 0);
    HIFI_MANAGE_INFO_LOG1("HIFI_MANAGE_ResetInit irq=", irq);
    /*注册狗中断处理函数,中断号117*/
    ret = request_irq(irq, HIFI_MANAGE_WatchdogIrqHandler, (unsigned long)0, "soc wdt handler", NULL);
    if (ret != VOS_OK) {
        HIFI_MANAGE_ERR_LOG1("HIFI_MANAGE_ResetInit wdg register cb failed", ret);
        goto error;
    }

    return;
error:
    if (g_hifiManageWatchdog.controlAddr != VOS_NULL) {
        iounmap(g_hifiManageWatchdog.controlAddr);
        g_hifiManageWatchdog.controlAddr = VOS_NULL;
    }
    if (g_hifiManageWatchdog.intclrAddr != VOS_NULL) {
        iounmap(g_hifiManageWatchdog.intclrAddr);
        g_hifiManageWatchdog.intclrAddr = VOS_NULL;
    }
    if (g_hifiManageWatchdog.lockAddr != VOS_NULL) {
        iounmap(g_hifiManageWatchdog.lockAddr);
        g_hifiManageWatchdog.lockAddr = VOS_NULL;
    }
    return;
}
irqreturn_t HIFI_MANAGE_WatchdogIrqHandler(int irq, void* data)
{
    HIFI_MANAGE_HifiExceptionMsg hifiExcepMsg = {0};
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_WatchdogIrqHandler enter");

    if (g_hifiManageWatchdog.lockAddr != VOS_NULL) {
        writel(HIFI_MANAGE_WDG_UNLOCK_NUM, g_hifiManageWatchdog.lockAddr);
    }
    if (g_hifiManageWatchdog.intclrAddr != VOS_NULL) {
        writel(HIFI_MANAGE_WDG_INTCLR_NUM, g_hifiManageWatchdog.intclrAddr);
    }
    if (g_hifiManageWatchdog.controlAddr != VOS_NULL) {
        writel(HIFI_MANAGE_WDG_CONTROL_DISABLE, g_hifiManageWatchdog.controlAddr);
    }
    if (g_hifiManageWatchdog.lockAddr != VOS_NULL) {
        writel(HIFI_MANAGE_WDG_LOCK_NUM, g_hifiManageWatchdog.lockAddr);
    }
    if (HIFI_MANAGE_EscapeTmrStart() != VOS_OK) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_WatchdogIrqHandler start timer fail");
    }
    g_hifiManageTimerCtrl.exceNum++;

    hifiExcepMsg.msgId = ID_HIFI_MANAGE_HIFI_EXCEPTION_IND;
    hifiExcepMsg.timeStamp = mdrv_timer_get_normal_timestamp();
    HIFI_MANAGE_SendMsg(ACPU_PID_HIFI_MANAGE, (const VOS_UINT8 *)&(hifiExcepMsg.msgId), sizeof(hifiExcepMsg) - VOS_MSG_HEAD_LENGTH);

    return IRQ_HANDLED;
}

VOS_VOID HIFI_MANAGE_HifiExceptionProc(MsgBlock* pMsg)
{
    VOS_UINT32 * hifiNmiAddr = VOS_NULL;

#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
    HIFI_MANAGE_VOICE_BHIFI_RESET_IND_MSG hifiResetMsg = {0};
    VOS_INT32 ret = 0;

    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_HifiExceptionProc enter");

    /*通知AHIFI发生BHIFI复位*/
    hifiResetMsg.usMsgName = ID_HIFI_MANAGE_BHIFI_RESET_IND;
    HIFI_MANAGE_SendMsg(ACPU_PID_VOICE_AGENT, (const VOS_UINT8 *)&(hifiResetMsg.usMsgName), sizeof(hifiResetMsg) - VOS_MSG_HEAD_LENGTH);

    /*通知NAS HIFI复位事件*/
    HIFI_MANAGE_RunCbFunc(HIFI_MANAGE_RESET_CALLCBFUN_RESET_BEFORE);

    /*触发NMI中断*/
    hifiNmiAddr = (VOS_UINT32 *)ioremap((unsigned long)HIFI_MANAGE_ACORE_HIFI_NMI, (unsigned long)0x4);
    if (hifiNmiAddr != VOS_NULL) {
        writel(HIFI_MANAGE_ACORE_HIFI_NMI_ENABLE, hifiNmiAddr);
        udelay(1);
        writel(HIFI_MANAGE_ACORE_HIFI_NMI_DISABLE, hifiNmiAddr);
        iounmap(hifiNmiAddr);
    }
    msleep(HIFI_MANAGE_NMI_WAIT_TIME_EXTRA_MODEM);

    /*保存日志*/

    /*通知底软加载镜像*/
    ret = mdrv_hifi_reset();
    if (ret != 0) {
        HIFI_MANAGE_ERR_LOG("drv reset fail");
        /*触发B5000复位*/
        mdrv_om_system_error(HIFI_MANAGE_HIFI_DSP_SYS_ERR, 0, 0, VOS_NULL, 0);
    }
    HIFI_MANAGE_LoadCheckTmrStart();

#else
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_HifiExceptionProc enter");

    /*触发NMI中断*/
    hifiNmiAddr = (VOS_UINT32 *)ioremap((unsigned long)HIFI_MANAGE_ACORE_HIFI_NMI, (unsigned long)0x4);
    if (hifiNmiAddr != VOS_NULL) {
        writel(HIFI_MANAGE_ACORE_HIFI_NMI_ENABLE, hifiNmiAddr);
        iounmap(hifiNmiAddr);
    }
    msleep(HIFI_MANAGE_NMI_WAIT_TIME_MBB);
    /*触发B5000复位*/
    mdrv_om_system_error(HIFI_MANAGE_HIFI_DSP_SYS_ERR, 0, 0, VOS_NULL, 0);

#endif
    return;
}
VOS_VOID HIFI_MANAGE_ToVoiceAgentLoadFinish(VOS_VOID)
{
    HIFI_MANAGE_HifiResetFinishMsg hifiResetMsg = {0};
    HIFI_MANAGE_INFO_LOG("HIFI_MANAGE_ToVoiceAgentLoadFinish enter");

    /*通知voiceagent BHIFI复位完成*/
    hifiResetMsg.msgId = ID_HIFI_MANAGE_TO_VOICE_AGENT_RESET_FINISHED;
    HIFI_MANAGE_SendMsg(ACPU_PID_VOICE_AGENT, (const VOS_UINT8 *)&(hifiResetMsg.msgId), sizeof(hifiResetMsg) - VOS_MSG_HEAD_LENGTH);

}

VOS_VOID HIFI_MANAGE_HifiImgLoadResultProc(MsgBlock* pMsg)
{
    HIFI_MANAGE_HifiImgLoadResultInd * hifiImgLoadResult = (HIFI_MANAGE_HifiImgLoadResultInd *)pMsg;

    if (g_hifiManageLoadTimer == VOS_NULL_PTR ||
        PS_STOP_REL_TIMER(&g_hifiManageLoadTimer) != VOS_OK) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_HifiImgLoadResultProc stop timer fail");
    }

    if (hifiImgLoadResult->loadResult == HIFI_MANAGE_HIFI_IMG_LOAD_OK) {
        /*发消息通知nas 复位完成*/
        HIFI_MANAGE_RunCbFunc(HIFI_MANAGE_RESET_CALLCBFUN_RESET_AFTER);
        HIFI_MANAGE_ToVoiceAgentLoadFinish();
    } else if (hifiImgLoadResult->loadResult == HIFI_MANAGE_HIFI_IMG_LOAD_FAILED) {
        /*触发B5000复位*/
        mdrv_om_system_error(HIFI_MANAGE_HIFI_DSP_SYS_ERR, 0, 0, VOS_NULL, 0);
    } else {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_HifiImgLoadResultProc result err");
    }
    return;
}

VOS_UINT32 HIFI_MANAGE_LoadImgCompleteProc(VOS_UINT32 procResult)
{
    HIFI_MANAGE_HifiImgLoadResultInd hifiImgLoadResult = {0};

    hifiImgLoadResult.msgId = ID_HIFI_MANAGE_HIFI_IMG_LOAD_COMPLETE_IND;
    hifiImgLoadResult.loadResult = procResult;
    HIFI_MANAGE_SendMsg(ACPU_PID_HIFI_MANAGE, (const VOS_UINT8 *)&(hifiImgLoadResult.msgId), sizeof(hifiImgLoadResult) - VOS_MSG_HEAD_LENGTH);
    return VOS_OK;
}
VOS_UINT32 HIFI_MANAGE_EscapeTmrStart(VOS_VOID)
{
    VOS_UINT32          rst;

    /* 已运行 */
    if (g_hifiManageTimerCtrl.timrer != VOS_NULL_PTR) {
        return VOS_OK;
    }

    g_hifiManageTimerCtrl.Period = HIFI_MANAGE_RST_MAX_TIME;
    rst = PS_START_REL_TIMER(&g_hifiManageTimerCtrl.timrer, ACPU_PID_HIFI_MANAGE, g_hifiManageTimerCtrl.Period, HIFI_MANAGE_TMR_ID_EXEC_PROC_TMR, 0,
                               VOS_RELTIMER_NOLOOP);

    if (rst != VOS_OK) {
        HIFI_MANAGE_ERR_LOG1("HIFI_MANAGE_EscapeProcTmrStart", rst);
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_VOID HIFI_MANAGE_LoadCheckTmrStart(VOS_VOID)
{
    VOS_UINT32          rst;

    /* 已运行 */
    if (g_hifiManageLoadTimer != VOS_NULL_PTR) {
        if (PS_STOP_REL_TIMER(&g_hifiManageLoadTimer) != VOS_OK) {
            HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_LoadCheckTmrStart stop fail");
            return;
        }
    }
    rst = PS_START_REL_TIMER(&g_hifiManageLoadTimer, ACPU_PID_HIFI_MANAGE, HIFI_MANAGE_LOAD_MAX_TIME, HIFI_MANAGE_TMR_ID_LOAD_CHECK_TMR, 0,
                               VOS_RELTIMER_NOLOOP);

    if (rst != VOS_OK) {
        HIFI_MANAGE_ERR_LOG1("HIFI_MANAGE_LoadCheckTmrStart fail", rst);
        return;
    }

    return;
}
VOS_VOID HIFI_MANAGE_LoadFailTmrProc(VOS_VOID)
{
    HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_LoadFailTmrProc");
    /*触发B5000复位*/
    mdrv_om_system_error(HIFI_MANAGE_HIFI_DSP_SYS_ERR, 0, 0, VOS_NULL, 0);

    return;
}

VOS_VOID HIFI_MANAGE_EscapeTmrTimeoutProc(VOS_VOID)
{
    if (g_hifiManageTimerCtrl.exceNum >= HIFI_MANAGE_MAX_EXEC_NUM) {
        HIFI_MANAGE_ERR_LOG1("HIFI_MANAGE_EscapeTmrTimeoutProc", g_hifiManageTimerCtrl.exceNum);
        mdrv_om_system_error(HIFI_MANAGE_HIFI_DSP_SYS_ERR, 0, 0, VOS_NULL, 0);
    } else {
        HIFI_MANAGE_ERR_LOG1("HIFI_MANAGE_EscapeTmrTimeoutProc", g_hifiManageTimerCtrl.exceNum);
        g_hifiManageTimerCtrl.exceNum = 0;
        g_hifiManageTimerCtrl.timrer = VOS_NULL_PTR;
    }

    return;
}

struct HIFI_MANAGE_ResetCallbackFuncNode *HIFI_MANAGE_ResetLinkInsert(struct HIFI_MANAGE_ResetCallbackFuncNode *link,
    struct HIFI_MANAGE_ResetCallbackFuncNode *unit)
{
    struct HIFI_MANAGE_ResetCallbackFuncNode *head = link;
    struct HIFI_MANAGE_ResetCallbackFuncNode *pose = link;
    struct HIFI_MANAGE_ResetCallbackFuncNode *tail = link;

    if (link == NULL || unit == NULL) {
        HIFI_MANAGE_ERR_LOG("HIFI_MANAGE_ResetLinkInsert input para err");
        return NULL;
    }

    while (pose != NULL) {
        if (pose->cbfuninfo.prioLevel > unit->cbfuninfo.prioLevel) {
            if (head == pose) {
                unit->pnext = pose;
                head = unit;
            } else {
                tail->pnext = unit;
                unit->pnext = pose;
            }
            break;
        }
        tail = pose;
        pose = pose->pnext;
    }

    if (pose == NULL) {
        tail->pnext = unit;
    }

    return head;
}
struct HIFI_MANAGE_ResetCallbackFuncNode *HIFI_MANAGE_ResetDoRegCbFunc(struct HIFI_MANAGE_ResetCallbackFuncNode *link,
    const char *name,
    HIFI_MANAGE_HifiResetCallbackFunc cbFun,
    int userData,
    int prioLevel)
{
    struct HIFI_MANAGE_ResetCallbackFuncNode  *head = link;
    struct HIFI_MANAGE_ResetCallbackFuncNode  *mgrUnit = NULL;

    if ((name == NULL)
        || (cbFun == NULL)
        || (prioLevel < HIFI_MANAGE_RESET_CBFUNC_PRIO_LEVEL_LOWT || prioLevel > HIFI_MANAGE_RESET_CBFUNC_PRIO_LEVEL_HIGH)) {
            HIFI_MANAGE_ERR_LOG1("fail in ccore reset regcb,fail, name, prio", prioLevel);
            return NULL;
    }

    mgrUnit = kmalloc(sizeof(*mgrUnit), GFP_KERNEL);
    if (mgrUnit != NULL) {
        (void)memset_s((void *)mgrUnit, (sizeof(*mgrUnit)), 0, (sizeof(*mgrUnit)));
        if (strncpy_s(mgrUnit->cbfuninfo.name, sizeof(mgrUnit->cbfuninfo.name), name, (unsigned long)HIFI_MANAGE_MODULE_NAME_LEN) != EOK) {
            HIFI_MANAGE_ERR_LOG("copy func name fail!");
        };
        mgrUnit->cbfuninfo.prioLevel = prioLevel;
        mgrUnit->cbfuninfo.userData = userData;
        mgrUnit->cbfuninfo.cbfun = cbFun;
    } else {
        HIFI_MANAGE_ERR_LOG("pmgr_unit malloc fail!");
        return NULL;
    }

    if (head == NULL) {
        head = mgrUnit;
    } else {
        head = HIFI_MANAGE_ResetLinkInsert(head, mgrUnit);
    }

    return head;
}

MODULE_EXPORTED int HIFI_MANAGE_RegResetCallbackFunc(const char * name, HIFI_MANAGE_HifiResetCallbackFunc cbFun, int userData, int prioLevel)
{
    g_hifiManageResetData = HIFI_MANAGE_ResetDoRegCbFunc(g_hifiManageResetData, name, cbFun, userData, prioLevel);
    HIFI_MANAGE_ERR_LOG("registered a cbfun for hifi reset");

    return 0;
}
int HIFI_MANAGE_DoRunCbFunc(const char *name, enum HIFI_MANAGE_ResetCallbackFuncMoment eparam)
{
    int  result = HIFI_MANAGE_SUCC;
    struct HIFI_MANAGE_ResetCallbackFuncNode  *head = g_hifiManageResetData;

    if (name == NULL) {
        HIFI_MANAGE_ERR_LOG("pname is null");
        return -EINVAL;
    }

    if (strncmp(name, HIFI_MANAGE_RESET_CBFUN_IGNORE_NAME, strlen(HIFI_MANAGE_RESET_CBFUN_IGNORE_NAME)) == 0) {
        while (head != NULL) {
            if (head->cbfuninfo.cbfun != NULL) {
                result = head->cbfuninfo.cbfun(eparam, head->cbfuninfo.userData);

                if (result !=HIFI_MANAGE_SUCC) {
                    HIFI_MANAGE_ERR_LOG("fail to run cbfun");
                    break;
                }
                HIFI_MANAGE_ERR_LOG("run cb function sucess");
            }
            head = head->pnext;
        }
    } else {
        while (head != NULL) {
            if (strncmp(name, head->cbfuninfo.name, strlen(head->cbfuninfo.name)) == 0
                && head->cbfuninfo.cbfun != NULL) {
                    result  = head->cbfuninfo.cbfun(eparam, head->cbfuninfo.userData);
                    HIFI_MANAGE_ERR_LOG("run cb function");
                    break;
            }
            head = head->pnext;
        }
    }

    if (result != HIFI_MANAGE_SUCC) {
        if (head != NULL) {
            HIFI_MANAGE_ERR_LOG("fail to run cbfun return");
        } else {
            HIFI_MANAGE_ERR_LOG("fail to run cbfun, but phead or pname is null");
        }
    }

    return result;
}

VOS_VOID HIFI_MANAGE_RunCbFunc(enum HIFI_MANAGE_ResetCallbackFuncMoment eparam)
{
    int  result = 0;

    if (eparam == HIFI_MANAGE_RESET_CALLCBFUN_RESET_BEFORE) {
        result = HIFI_MANAGE_DoRunCbFunc("NAS_AT", eparam);
        if (result != HIFI_MANAGE_SUCC) {
            HIFI_MANAGE_ERR_LOG("get callback fun fail");
        }
    } else {
        result = HIFI_MANAGE_DoRunCbFunc(HIFI_MANAGE_RESET_CBFUN_IGNORE_NAME, eparam);
        if (result != HIFI_MANAGE_SUCC) {
            HIFI_MANAGE_ERR_LOG("run callback fun fail");
        }
    }
    HIFI_MANAGE_INFO_LOG("end of run cb functions for hifi reset");
    return;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
