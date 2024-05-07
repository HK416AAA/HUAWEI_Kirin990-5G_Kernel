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

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "hifi_dump.h"
#include <linux/io.h>
#include <linux/delay.h>
#include <securec.h>
#include "Voice_agent_public.h"
#include "VoiceAgentVoiceInterface.h"
#include "bsp_dump.h"
#include "bsp_icc.h"
#include "bsp_slice.h"
#include "ps_common_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

#define THIS_FILE_ID                    PS_FILE_ID_DMS_CORE_C
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* HIFI�쳣MODID ��Χ HISI_MNTN_EXC_HIFI_START ~ HISI_MNTN_EXC_HIFI_END */
#define HISI_MNTN_SOCHIFI_EXC_MOD_ID 0x86000001

#define HIFI_DUMP_PROTECT_TIMELEN 2000
#define HIFI_DUMP_FNAME_LEN 128
#define HIFI_DUMP_LOG_SUCC 1
#define HIFI_DUMP_LOG_FAIL 2
#define HIFI_DUMP_MAGIC_NUM 0x5A5A5A5A


#if ((FEATURE_HIFI_DUMP == FEATURE_ON) && (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB)) && (defined(CONFIG_MODEM_FULL_DUMP)))
/* HIFI�쳣ʱʹ�ã�MBB HIFI��֧�ֶ�����λ���ܣ����쳣��Ϣ��ʱ���� */
struct rdr_exception_info_s g_hifiExcInfo =
{
    .e_modid            = HISI_MNTN_SOCHIFI_EXC_MOD_ID,
    .e_modid_end        = HISI_MNTN_SOCHIFI_EXC_MOD_ID,
    .e_process_priority = RDR_ERR,
    .e_reboot_priority  = RDR_REBOOT_WAIT,
    .e_notify_core_mask = RDR_HIFI,
    .e_reset_core_mask  = RDR_HIFI,
    .e_from_core        = RDR_HIFI,
    .e_reentrant        = (u32)RDR_REENTRANT_DISALLOW,
    .e_exce_type        = SOCHIFI_S_EXCEPTION,
    .e_upload_flag      = (u32)RDR_UPLOAD_YES,
    .e_from_module      = "sochifi",
    .e_desc             = "sochifi exc reset",
};

struct HifiDumpCtrlInfo g_hifiDumpCtrl;
struct HifiDumpRdrExcInfo g_hifiDumpRdrExc;

/*****************************************************************************
  3 �ⲿ��������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : HifiDumpSaveLog
 ��������  : ����HIFI��־��DDR
*****************************************************************************/
static VOS_VOID HifiDumpSaveLog(VOS_VOID)
{
    bsp_dump_save_file(g_hifiDumpRdrExc.logPath,"mbbhifi_dump.bin", g_hifiDumpCtrl.virtAddr, (VOS_VOID*)g_hifiDumpCtrl.phyAddr, g_hifiDumpCtrl.length);
}

/*****************************************************************************
 �� �� ��  : VoiceAgent_SendDumpLogReq
 ��������  : ���͵�����־�����MBB HIFI
*****************************************************************************/
static VOS_VOID VoiceAgent_SendDumpLogReq(VOS_VOID)
{
    struct VoiceAgent_Voice_DumpLogReq msg = {0};
    VOS_INT32 ret;

    VOICEAGENT_ERR_LOG("VoiceAgent_SendDumpLogReq enter");

    msg.ulSenderCpuId = VOS_CPU_ID_ACPU;
    msg.ulSenderPid = ACPU_PID_VOICE_AGENT;
    msg.ulReceiverCpuId = VOS_CPU_ID_MEDDSP;
    msg.ulReceiverPid = DSP_PID_VOICE_RT;
    msg.ulLength = sizeof(struct VoiceAgent_Voice_DumpLogReq) - VOS_MSG_HEAD_LENGTH;
    msg.msgId = ID_VOICEAGENT_VOICE_DUMP_LOG_REQ;
    msg.phyAddr = g_hifiDumpCtrl.phyAddr;
    msg.logLength = g_hifiDumpCtrl.length;

    ret = bsp_icc_send(ICC_CPU_HIFI, ACORE_HIFI_ICC_CHN_DUMP_FUNC, (VOS_UINT8*)&msg, sizeof(msg));
    if(ret != (VOS_INT32)(sizeof(msg))) {
        VOICEAGENT_ERR_LOG1("VoiceAgent_SendDumpLogReq icc send fail,ret=", ret);
    }
}

/*****************************************************************************
 �� �� ��  : AcoreHifiICCDumpFuncCallback
 ��������  : AcoreHifiICC���մ���ص�����
*****************************************************************************/
VOS_INT32 AcoreHifiICCDumpFuncCallback(VOS_UINT32 channelId , VOS_UINT32 len, VOS_VOID* context)
{
    struct Voice_VoiceAgent_DumpLogCnf msg = {0};
    VOS_INT32 ret;

    ret = bsp_icc_read(channelId, (VOS_UINT8*)&msg, sizeof(msg));
    if (ret != (VOS_INT32)len) {
        VOICEAGENT_ERR_LOG1("icc read fail,ret=", ret);
        return ret;
    }

    VOICEAGENT_ERR_LOG2("icc read ok,msgid,result", msg.msgId, msg.result);

    return 0;
}

/*****************************************************************************
 �� �� ��  : HifiDumpCallback
 ��������  : Dump�ص�����
*****************************************************************************/
VOS_VOID HifiDumpCallback(VOS_UINT32 modId, VOS_UINT32 eType,
                VOS_UINT64 coreId, VOS_CHAR* logPath, pfn_cb_dump_done fnDone)
{
    if (!g_hifiDumpCtrl.initState) {
        VOICEAGENT_ERR_LOG("HifiDumpCallback init state err!");
        return;
    }

    if (logPath == VOS_NULL_PTR || fnDone == VOS_NULL_PTR) {
        VOICEAGENT_ERR_LOG("HifiDumpCallback log path null!");
        return;
    }

    g_hifiDumpRdrExc.modId = modId;
    g_hifiDumpRdrExc.coreId = coreId;
    g_hifiDumpRdrExc.dumpDone = fnDone;
    g_hifiDumpRdrExc.logPath = logPath;

    osl_sem_up(&g_hifiDumpCtrl.taskSem);
}

/*****************************************************************************
 �� �� ��  : HifiDumpTask
 ��������  : Dump task
*****************************************************************************/
VOS_INT32 HifiDumpTask(VOS_VOID *data)
{
    while(true) {
        osl_sem_down(&g_hifiDumpCtrl.taskSem);

        HifiDumpSaveLog();

        g_hifiDumpRdrExc.dumpDone(g_hifiDumpRdrExc.modId, g_hifiDumpRdrExc.coreId);
        VOICEAGENT_ERR_LOG("hifi log save done");
        g_hifiDumpCtrl.dupmDoneFlag = true;
    }
}

/*****************************************************************************
 �� �� ��  : HifiResetCallback
 ��������  : ��λ�ص�������ע���rdr����HIFI���쳣ʱrdr���ص�����;
             MBB HIFI���쳣ʱ�ɵ�����ƴ�������Ҫע��
*****************************************************************************/
VOS_VOID HifiResetCallback(VOS_UINT32 modid, VOS_UINT32 etype, VOS_UINT64 coreid)
{
    VOICEAGENT_ERR_LOG("hifi reset!");
}

/*****************************************************************************
 �� �� ��  : HifiDumpInit
 ��������  : HIFI DUMP ��ʼ������
*****************************************************************************/
VOS_VOID HifiDumpInit(VOS_VOID)
{
    struct rdr_module_ops_pub socOps = {0};
    struct rdr_register_module_result socRst = {0};
    VOS_INT32 ret;

    VOICEAGENT_ERR_LOG("HifiDumpInit enter");

    memset_s(&g_hifiDumpCtrl, sizeof(g_hifiDumpCtrl), 0, sizeof(struct HifiDumpCtrlInfo));
    memset_s(&g_hifiDumpRdrExc, sizeof(g_hifiDumpRdrExc), 0, sizeof(struct HifiDumpRdrExcInfo));

    /* �ص�����ע�� */
    socOps.ops_dump = HifiDumpCallback;
    socOps.ops_reset = HifiResetCallback;
    if (rdr_register_module_ops(RDR_HIFI, &socOps, &socRst) != VOS_OK) {
        VOICEAGENT_ERR_LOG("HifiDumpInit register rdr fail!");
        return;
    }

    /* ��¼�����A�������ַ�Ϳռ� */
    g_hifiDumpCtrl.phyAddr = socRst.log_addr;
    g_hifiDumpCtrl.length = socRst.log_len;
    g_hifiDumpCtrl.virtAddr = ioremap_wc(socRst.log_addr, socRst.log_len);
    if (g_hifiDumpCtrl.virtAddr == VOS_NULL_PTR) {
        VOICEAGENT_ERR_LOG("HifiDumpInit remap fail!");
        return;
    }

    /* �����ź��� */
    osl_sem_init(0, &g_hifiDumpCtrl.taskSem);

    /* ���������߳� */
    if (osl_task_init("hifi_dump", 96, 0x1000, (OSL_TASK_FUNC)HifiDumpTask, NULL, &g_hifiDumpCtrl.taskId) != VOS_OK) {
        VOICEAGENT_ERR_LOG("HifiDumpInit osl task init fail!");
        return;
    }

    /* register hifi exception info */
    if (rdr_register_exception(&g_hifiExcInfo) == 0) {
        VOICEAGENT_ERR_LOG1("register rdr exception failed! exc_modid", g_hifiExcInfo.e_modid);
        return;
    }

    /* register icc callback */
    ret = bsp_icc_event_register(ACORE_HIFI_ICC_CHN_DUMP_FUNC, AcoreHifiICCDumpFuncCallback, VOS_NULL, VOS_NULL, VOS_NULL);
    if (ret) {
        VOICEAGENT_ERR_LOG1("register icc event fail! exc_modid", ret);
        return;
    }

    g_hifiDumpCtrl.initState = true;
    VOICEAGENT_ERR_LOG1("HifiDumpInit exit, len", g_hifiDumpCtrl.length);
}
#else
VOS_VOID HifiDumpInit(VOS_VOID)
{
    return;
}
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


