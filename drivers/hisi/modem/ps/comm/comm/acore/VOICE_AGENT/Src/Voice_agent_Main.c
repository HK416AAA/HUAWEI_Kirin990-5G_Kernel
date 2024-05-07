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
#include <linux/kernel.h>
#include <linux/list.h>
#include "vos.h"
#include "mdrv.h"
#include "mdrv_udi.h"
#include "Voice_agent_Main.h"
#include "PsTypeDef.h"
#include "Voice_agent_public.h"
#include "Voice_agent_msg_proc.h"
#include "hifi_dump.h"

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


/*****************************************************************************
  3 �ⲿ��������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
VOS_VOID VOICE_AGENT_PcieReadyCb(VOS_VOID)
{
    VOS_UINT8 ucDevindex = 0;
    VOICEAGENT_INFO_LOG("VOICE_AGENT_PcieReadyCb enter");
    /* ��PCIE��ͨ�� */
    for (ucDevindex = 0; ucDevindex < VOICE_AGENT_DEV_BUTT; ucDevindex++) {
        if(VOS_OK != VOICE_AGENT_PcieOpen(ucDevindex)) {
            VOICEAGENT_ERR_LOG1("PcieOpen fail, devindex = ", ucDevindex);
        }
    }

    if (VOS_OK != VOICE_AGENT_NV_PROC()) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_NV_PROC fail");
    }

    if (VOS_OK != VOICE_AGENT_OM_PROC()) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_OM_PROC fail");
    }

}
VOS_VOID VOICE_AGENT_PcieDisableCb(VOS_VOID)
{
    VOS_UINT8 ucDevindex = 0;
    VOICEAGENT_INFO_LOG("PCIE_DISABLE_CB enter");
    for (ucDevindex = 0; ucDevindex < VOICE_AGENT_DEV_BUTT; ucDevindex++) {
        mdrv_udi_close(g_stHifDevInfo[ucDevindex].slVoiceagentHdl);
    }
}
/*****************************************************************************
 �� �� ��  : HIFI_ACORE_PidInit
 ��������  : HIFI A AGENT PID��ʼ������
 �������  : enum VOS_InitPhaseDefine enPhase
 �������  : ��
 �� �� ֵ  : VOS_UINT32
             VOS_OK
             VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��09��10��
    �޸�����   : �����ɺ���
*****************************************************************************/
static VOS_VOID VOICE_AGENT_Init(VOS_VOID)
{
    VOICEAGENT_INFO_LOG("VOICE_AGENT_Init enter");

    mdrv_pcdev_reg_enumdonecb(VOICE_AGENT_PcieReadyCb);
    mdrv_pcdev_reg_disablecb(VOICE_AGENT_PcieDisableCb);
    VOICE_AGENT_InitMsgList();
    return;


}
#endif
/*****************************************************************************
 �� �� ��  : HIFI_ACORE_MsgProc
 ��������  : ATģ����Ϣ������ں���
 �������  : MsgBlock* pMsg
 �������  : ��
 �� �� ֵ  : TAF_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.Date        : 2018/10/10
    Modification: Created function

*****************************************************************************/
static VOS_VOID VOICE_AGENT_MsgProc(MsgBlock* pMsg)
{
#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
	MsgBlock		  *pHeader = VOS_NULL_PTR;

	/* ��ȡ��Ϣͷָ��*/
	pHeader = pMsg;

	switch(pHeader->ulSenderPid)
	{
		case UEPS_PID_MTC:
			VOICE_AGENT_MtcMsgProc(pMsg);
			break;
		case I0_PS_PID_IMSA:
		case I1_PS_PID_IMSA:
			VOICE_AGENT_ImsaMsgProc(pMsg);
			break;
        case MSP_PID_DIAG_AGENT:
            VOICE_AGENT_DiagAgentMsgProc(pMsg);
            break;
        case I0_WUEPS_PID_VC:
        case I1_WUEPS_PID_VC:
        case I2_WUEPS_PID_VC:
            VOICE_AGENT_VcMsgProc(pMsg);
            break;
        case ACPU_PID_VOICE_AGENT:
            VOICE_AGENT_LoopMsgProc(pMsg);
            break;
        case ACPU_PID_HIFI_MANAGE:
            VOICE_AGENT_HifiManageMsgProc(pMsg);
            break;
		default:
			break;
	}
#endif
	return;
}
/*****************************************************************************
 �� �� ��  : HIFI_ACORE_PidInit
 ��������  : HIFI A AGENT PID��ʼ������
 �������  : enum VOS_InitPhaseDefine enPhase
 �������  : ��
 �� �� ֵ  : VOS_UINT32
             VOS_OK
             VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��09��10��
    �޸�����   : �����ɺ���
*****************************************************************************/
static VOS_UINT32  VOICE_AGENT_PidInit(enum VOS_InitPhaseDefine enPhase)
{
	VOICEAGENT_INFO_LOG("VOICE_AGENT_PidInit enter");

    switch ( enPhase )
    {
        case VOS_IP_INITIAL:
#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
            VOICE_AGENT_Init();
            HifiDumpInit();
#endif
            break;

        default:
            break;
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : HIFI_AcoreFidInit
 ��������  : HIFI FID ��ʼ������
 �������  :

 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2018��9��06��
     �޸�����  : Creat Function
*****************************************************************************/
VOS_UINT32 VOICE_AGENT_FidInit(enum VOS_InitPhaseDefine ip)
{
    VOS_UINT32                          ulRelVal = 0;

    VOICEAGENT_INFO_LOG("VOICE_AGENT_FidInit enter");

    switch (ip)
    {
    case VOS_IP_LOAD_CONFIG:

        ulRelVal = VOS_RegisterPIDInfo(ACPU_PID_VOICE_AGENT, (InitFunType) VOICE_AGENT_PidInit, (MsgFunType) VOICE_AGENT_MsgProc);
        if (ulRelVal != VOS_OK)
        {
            VOICEAGENT_ERR_LOG("VOICE_AGENT_FidInit register pid error");
            return VOS_ERR;
        }

        ulRelVal = VOS_RegisterTaskPrio(ACPU_FID_VOICE_AGENT, VOS_PRIORITY_M5);
        if (ulRelVal != VOS_OK)
        {
            VOICEAGENT_ERR_LOG("VOICE_AGENT_FidInit register pid prio error");
            return VOS_ERR;
        }

        break;

    default:
        break;
    }

    return VOS_OK;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

