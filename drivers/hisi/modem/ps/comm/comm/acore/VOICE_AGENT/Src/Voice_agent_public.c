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

#include "Voice_agent_public.h"
#include "vos.h"
#include "mdrv.h"
#include "mdrv_udi.h"
#include "Voice_agent_om.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

#define THIS_FILE_ID                    PS_FILE_ID_DMS_CORE_C
#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
#define PCIE_ASYNC_WRITE_NUM            400
VOICE_AGENT_DEV_INFO_STRU               g_stHifDevInfo[VOICE_AGENT_DEV_BUTT] =
{
    {UDI_PCDEV_AGENT_NV_ID, -1, NULL, NULL, 0},
    {UDI_PCDEV_AGENT_OM_ID, -1, NULL, NULL, 0},
    {UDI_PCDEV_AGENT_MSG_ID, -1, NULL, NULL, 0}
};

/*****************************************************************************
 �� �� ��  : VOICE_AGENT_WriteDataAsync
 ��������  : ��װHIFI�첽д���ݽӿ�
 �������  : pstDataAddr   - ����ָ��(�ɵ����߱�֤�ǿ�)
 �������  : ��
 �� �� ֵ  : AT_SUCCESS - �ɹ�
             AT_FAILURE - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��09��15��
    �޸�����   : �����ɺ���
*****************************************************************************/
static VOS_INT32 VOICE_AGENT_WriteDataAsync
(
    VOS_UINT8                         ucDevindex,
    VOS_UINT8                        *pstDataAddr,
    VOS_UINT8                        *pstPhyAddr,
    VOS_UINT32                        ulLen
)
{
    HIFI_WR_ASYNC_INFO_STRU             stCtlParam;
    VOS_INT32                           ulResult;
    VOS_UINT32                          i = 0;

    /* ���UDI�����Ч�� */
    if (VOICEAGENT_INVALID_HANDLE == g_stHifDevInfo[ucDevindex].slVoiceagentHdl)
    {
        VOICEAGENT_ERR_LOG("Invalid UDI handle!");
        return VOS_ERR;
    }
	if (NULL == pstDataAddr) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_WriteDataAsync pstDataAddr NULL");
        return VOS_ERR;
	}

    /* ��д�������ڴ��ַ */
    stCtlParam.pVirAddr = (VOS_CHAR *)pstDataAddr;
    stCtlParam.pPhyAddr = (VOS_CHAR *)pstPhyAddr;
    stCtlParam.u32Size  = ulLen;
    stCtlParam.pDrvPriv = VOS_NULL_PTR;

    /* �첽��ʽд�� */
    ulResult = mdrv_udi_ioctl(g_stHifDevInfo[ucDevindex].slVoiceagentHdl, VOICEAGENT_IOCTL_WRITE_ASYNC, &stCtlParam);
    while ((0 != ulResult) && (i <= PCIE_ASYNC_WRITE_NUM))
    {
        ulResult = mdrv_udi_ioctl(g_stHifDevInfo[ucDevindex].slVoiceagentHdl, VOICEAGENT_IOCTL_WRITE_ASYNC, &stCtlParam);
        i ++;
    }

    if (i != 0) {
        VOICEAGENT_ERR_LOG1("VOICE_AGENT_WriteDataAsync ulResult", ulResult);
        VOICEAGENT_ERR_LOG1("VOICE_AGENT_WriteDataAsync i", i);
    }

    return ulResult;
}
/*****************************************************************************
 �� �� ��  : VOICE_AGENT_PcieOpen
 ��������  : ���ڴ�PCIEʹ�õĶ˿��豸
 �������  : dev_index pcie_�豸����
 �������  : ��
 �� �� ֵ  : VOS_OK/VOS_ERR
 �޸���ʷ  :
   1.��    ��  : 2018��9��27��
     �޸�����  : Creat Function
*****************************************************************************/
VOS_INT32 VOICE_AGENT_PcieOpen
(
    VOS_UINT8                           ucDevindex
)
{
    struct udi_open_param                    stParam;
    VOICE_AGENT_HANDLE                  temp_lHandle = VOICEAGENT_INVALID_HANDLE;

    if (VOICE_AGENT_DEV_BUTT <= ucDevindex) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieOpen dev_index invaild");
        return VOS_ERR;
    }

    stParam.devid = g_stHifDevInfo[ucDevindex].enUdiDevId;
    temp_lHandle = mdrv_udi_open(&stParam);
    if (VOICEAGENT_INVALID_HANDLE == temp_lHandle)
    {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_Init OPEN ID ERROR");
        return VOS_ERR;
    }
    else
    {
        g_stHifDevInfo[ucDevindex].slVoiceagentHdl = temp_lHandle;
        return VOS_OK;
    }
}
/*****************************************************************************
 �� �� ��  : VOICE_AGENT_PcieSetWrite
 ��������  : PCIE���д����
 �������  : dev_index pcie_�豸����
             pstNvData NV����
             pWriteCB д��ɻص�
 �������  : ��
 �� �� ֵ  : VOS_OK/VOS_ERR
 �޸���ʷ  :
   1.��    ��  : 2018��9��27��
     �޸�����  : Creat Function
*****************************************************************************/
VOS_INT32 VOICE_AGENT_PcieSetWrite
(
    VOS_UINT8                           ucDevindex,
    VOS_UINT8                          *pstData,
    VOS_UINT8                          *pstPhyaddr,
    VOS_UINT32                          ulLen,
    pcdev_write_done_cb_t               pWriteCB
)
{
    VOS_INT32                           ulRet = 0;

    if (VOICE_AGENT_DEV_BUTT <= ucDevindex) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieSetWrite dev_index invaild");
        return VOS_ERR;
    }
	if (NULL == pstData) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieSetWrite pstData NULL");
        return VOS_ERR;
	}

    ulRet = mdrv_udi_ioctl(g_stHifDevInfo[ucDevindex].slVoiceagentHdl, VOICEAGENT_IOCTL_SET_WRITE_CB, pWriteCB);

    if (VOS_OK != ulRet)
    {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieSetWrite, ERROR, Set data read callback for modem failed!");
        return VOS_ERR;
    }

    ulRet = VOICE_AGENT_WriteDataAsync(ucDevindex, (VOS_UINT8 *)pstData, (VOS_UINT8 *)pstPhyaddr, ulLen);
    if (ulRet != VOS_OK)
    {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieSetWrite WriteDataAsyn ERROR");
        return ulRet;
    }
    return VOS_OK;
}
/*****************************************************************************
 �� �� ��  : VOICE_AGENT_PcieSetRead
 ��������  : PCIE��ɶ�����
 �������  : dev_index pcie_�豸����
             pReadCB ����ɻص�
             pReadBuff OM����buf
 �������  :
 �� �� ֵ  : VOS_OK/VOS_ERR
 �޸���ʷ  :
   1.��    ��  : 2018��9��27��
     �޸�����  : Creat Function
*****************************************************************************/
VOS_INT32 VOICE_AGENT_PcieSetRead
(
    VOS_UINT8                           ucDevindex,
    HIFI_READ_BUFF_INFO                *pReadBuff,
    VOS_VOID                           *pReadCB
)
{
    VOS_INT32                           ulRet = 0;

    if (VOICE_AGENT_DEV_BUTT <= ucDevindex) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieSetRead dev_index invaild");
        return VOS_ERR;
    }
	if (NULL == pReadBuff) {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieSetRead pReadBuff NULL");
        return VOS_ERR;
	}

    ulRet = mdrv_udi_ioctl(g_stHifDevInfo[ucDevindex].slVoiceagentHdl, VOICEAGENT_IOCTL_RELLOC_READ_BUFF, pReadBuff);

    /* ע��MODEM�豸�������ݽ��ջص� */
    ulRet = mdrv_udi_ioctl(g_stHifDevInfo[ucDevindex].slVoiceagentHdl, VOICEAGENT_IOCTL_SET_READ_CB, pReadCB);
    if (VOS_OK != ulRet)
    {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_Init, ERROR, Set data read callback for modem failed!");
        return VOS_ERR;
    }
    return VOS_OK;
}
/*****************************************************************************
 �� �� ��  : VOICE_AGENT_PcieReadDataCB
 ��������  : MODEM�豸���ݶ��ص���ע�������
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2018��10��10��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID VOICE_AGENT_PcieReadOmDataCB( VOS_VOID )
{
    VOS_UINT8                           ulResult;
    HIFI_WR_ASYNC_INFO_STRU             stCtlParam;

    ulResult = mdrv_udi_ioctl(g_stHifDevInfo[VOICE_AGENT_OM_DEV].slVoiceagentHdl, VOICEAGENT_IOCTL_GET_RD_BUFF, &stCtlParam);

    if ( VOS_OK != ulResult )
    {
        return ;
    }

    if (VOS_NULL_PTR == stCtlParam.pVirAddr)
    {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_PcieReadDataCB, WARNING, Data buffer error");
        return;
    }

    if (stCtlParam.u32Size > (VOICE_AGENT_LOG_DATA_NUM * VOICE_AGENT_LOG_BUFFER_LEN))
    {
        return;
    }

    if (memcpy_s(g_stDiagData, sizeof(g_stDiagData), stCtlParam.pVirAddr,stCtlParam.u32Size) != VOS_OK) {
        VOICEAGENT_INFO_LOG("g_stDiagData memcpy fail");
    }

    (VOS_VOID)VOICE_AGENT_DiagPackSend(stCtlParam.pVirAddr,stCtlParam.u32Size);

    ulResult = mdrv_udi_ioctl(g_stHifDevInfo[VOICE_AGENT_OM_DEV].slVoiceagentHdl, VOICEAGENT_IOCTL_RETURN_BUFF, &stCtlParam);
    if ( VOS_OK != ulResult )
    {
        return;
    }

    return;
}

/*****************************************************************************
 �� �� ��  : VOICE_AGENT_PcieReadDataCB
 ��������  : MODEM�豸���ݶ��ص���ע�������
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2018��10��10��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID VOICE_AGENT_PcieWriteNvDataCB(char *pVirAddr, char *pPhyAddr, int size)
{
    VOICEAGENT_INFO_LOG("VOICE_AGENT_PcieWriteNvDataCB");

    VOICE_AGENT_SendCnfMsgToOther(ID_AGENT_AGENT_LOOP_IND, ACPU_PID_VOICE_AGENT, 0);
}

/*****************************************************************************
 �� �� ��  : VOICE_AGENT_SendCnfMsgToVc
 ��������  : �ظ�VC����Ϣ
 �������  :
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ      :
  1.��    ��   : 2018��09��15��
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT32 VOICE_AGENT_SendCnfMsgToOther(VOS_UINT16 uhwMsgId, VOS_UINT32 uwReceiverPid, VOS_UINT32 uwRet)
{
    VCVOICE_OP_RSLT_STRU *stCnfMsg;

    stCnfMsg = (VCVOICE_OP_RSLT_STRU *)PS_ALLOC_MSG(ACPU_PID_VOICE_AGENT, sizeof(VCVOICE_OP_RSLT_STRU) - VOS_MSG_HEAD_LENGTH);

    if (stCnfMsg == VOS_NULL_PTR)
    {
        return VOS_ERR;
    }

    stCnfMsg->ulSenderPid   = ACPU_PID_VOICE_AGENT;
    stCnfMsg->ulReceiverPid = uwReceiverPid;
    stCnfMsg->usMsgName     = uhwMsgId;
    stCnfMsg->enExeRslt     = (VOS_UINT16)uwRet;


    if(VOS_OK != PS_SEND_MSG(ACPU_PID_VOICE_AGENT, stCnfMsg))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
#endif
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
