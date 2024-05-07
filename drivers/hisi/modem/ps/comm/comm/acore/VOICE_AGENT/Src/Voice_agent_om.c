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

#include "Voice_agent_nv.h"
#include "mdrv.h"
#include "mdrv_udi.h"
#include "msp_diag_comm.h"
#include "Voice_agent_om.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/

#define THIS_FILE_ID                    PS_FILE_ID_DMS_CORE_C

#if (defined(CONFIG_HISI_BALONG_EXTRA_MODEM_MBB))
/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
VOICE_AGENT_DIAG_CTRL_INFO              g_stDiagCtrlInfo = {49, VOS_FALSE};
VOICE_AGENT_DIAG_DATA_INFO              g_stDiagData[VOICE_AGENT_LOG_DATA_NUM];
VOS_UINT16 g_socpCfgDone = VOICE_AGENT_SOCP_CFG_UNDONE;

/*****************************************************************************
  3 外部函数声明
*****************************************************************************/

/*****************************************************************************
  3 函数实现
*****************************************************************************/
VOS_UINT16 GetSocpStatus(){
    return g_socpCfgDone;
}
VOS_VOID SetSocpStatus(VOS_UINT16 uwpara){
    g_socpCfgDone = uwpara;
}

static VOS_INT32 VOICE_AGENT_SocpFormatCheck(VOICE_AGENT_DIAG_DATA_INFO *pst)
{
    VOS_UINT8          isSocpHeadError = 0;
    VOS_UINT32         socpLenSum = 0;
    VOS_UINT16         usLen = 0;
    VOS_UINT32         ulsocp_head = 0;
    VOS_UINT8          *pDataCheck = NULL;

	if (NULL == pst)
	{
		return VOS_ERR;
	}
	pDataCheck = pst->aucData;
	while (socpLenSum < pst->ulDataSize)
	{
		ulsocp_head = ((VOS_UINT32)(*pDataCheck) ) | ((VOS_UINT32)(*(pDataCheck+1)) << 8) | ((VOS_UINT32)(*(pDataCheck+2)) << 16)  | ((VOS_UINT32)(*(pDataCheck+3)) << 24);
		if ( VOICE_AGENT_SOCP_HEAD_MAGIC_NUM != ulsocp_head)
		{
			isSocpHeadError = 1;
			break;
		}
		pDataCheck = pDataCheck +4;
		usLen = ((VOS_UINT16)(*(pDataCheck)) ) | ((VOS_UINT16)(*(pDataCheck+1)) << 8);
		usLen = ((usLen-1)/8 + 1) *8;
		pDataCheck = pDataCheck + 4 + usLen;
		socpLenSum = socpLenSum + usLen +8;
	}

	if ((socpLenSum != pst->ulDataSize) | isSocpHeadError)
	{
		isSocpHeadError = 0;
		return VOS_ERR;
	}
	return VOS_OK;
}

/*****************************************************************************
* 函 数 名  : VOICE_AGENT_SrcChanCfg
*
* 功能描述  : 维测LOG数据送入socp源通道配置
*
* 输入参数  : 无
*
* 输出参数  : 无
*
* 返 回 值  : 成功与否标识码

 修改历史   :
 日    期   : 2018年10月8日
 修改内容   :
*****************************************************************************/
static VOS_INT32 VOICE_AGENT_SrcChanCfg(VOS_VOID)
{
    socp_src_chan_cfg_s                EncSrcAttr;
    VOS_UINT8                          *pVirAddr=0;
    VOS_UINT_PTR                        ulPhyAddr  = VOS_NULL_PTR;

    pVirAddr = (VOS_UINT8*)LPS_CacheDmaMalloc(VOICE_AGENT_LOG_SRC_BUF_LEN,VOS_UNCACHE_MEM_ADDR_ALIGN_4K,&ulPhyAddr,(VOS_UINT32)0xea000000);

    if(VOS_NULL_PTR == pVirAddr)
    {
        VOICEAGENT_ERR_LOG("applog src chan malloc fail!");
        LPS_CacheDmaFree(pVirAddr,VOICE_AGENT_LOG_SRC_BUF_LEN);
        return VOS_ERR;
    }

    if (memcpy_s(pVirAddr, VOICE_AGENT_LOG_SRC_BUF_LEN, &g_stDiagCtrlInfo, sizeof(g_stDiagCtrlInfo)) != VOS_OK) {
        VOICEAGENT_INFO_LOG("pVirAddr memcpy fail!");
    }

    g_stDiagCtrlInfo.pChannelVirAddr = pVirAddr;
    g_stDiagCtrlInfo.pChannelPhyAddr = ulPhyAddr;

    EncSrcAttr.data_type = SOCP_DATA_TYPE_0;
    EncSrcAttr.data_type_en = SOCP_DATA_TYPE_EN;
    EncSrcAttr.debug_en = SOCP_ENC_DEBUG_DIS;
    EncSrcAttr.mode = SOCP_ENCSRC_CHNMODE_CTSPACKET;
    EncSrcAttr.priority = SOCP_CHAN_PRIORITY_1;
    EncSrcAttr.trans_id_en = SOCP_TRANS_ID_EN;
    EncSrcAttr.bypass_en = 0;
    EncSrcAttr.dest_chan_id = SOCP_CODER_DST_OM_IND;
    EncSrcAttr.rptr_img_en = 0;

    EncSrcAttr.coder_src_setbuf.input_start = (u8 *)ulPhyAddr;
    EncSrcAttr.coder_src_setbuf.input_end = (u8 *)(ulPhyAddr +VOICE_AGENT_LOG_SRC_BUF_LEN - 1);

    if(VOS_OK != mdrv_socp_corder_set_src_chan(49, &EncSrcAttr))
    {
        VOICEAGENT_ERR_LOG("src channel set failed!");
        LPS_CacheDmaFree(pVirAddr,VOICE_AGENT_LOG_SRC_BUF_LEN);
        return VOS_ERR;
    }

    return VOS_OK;
}
/*****************************************************************************
* 函 数 名  : VOICE_AGENT_SrcChanStop
*
* 功能描述  : 维测LOG数据关闭socp 通道
*
* 输入参数  : 无
*
* 输出参数  : 无
*
* 返 回 值  : 成功与否标识码

 修改历史   :
 日    期   : 2018年10月8日
 修改内容   :
*****************************************************************************/
VOS_VOID VOICE_AGENT_SrcChanStop(VOS_VOID)
{
    VOS_UINT32                          ulRet;

    ulRet = mdrv_socp_stop(g_stDiagCtrlInfo.ulChannelId);
    if(ulRet)
    {
       VOICEAGENT_ERR_LOG("stop socp fail");
       return;
    }
    VOICEAGENT_INFO_LOG1("SrcChanStop:ulRet = ",ulRet);

    ulRet = mdrv_clear_socp_buff(g_stDiagCtrlInfo.ulChannelId);
    if(ulRet)
    {
        VOICEAGENT_ERR_LOG("clear socp buff fail");
        return;
    }

    VOICEAGENT_INFO_LOG1("SrcChanStopclear:ulRet = ",ulRet);

}

/*****************************************************************************
* 函 数 名  : VOICE_AGENT_SrcChanStart
*
* 功能描述  : 维测LOG数据打开socp源通道
*
* 输入参数  : 无
*
* 输出参数  : 无
*
* 返 回 值  : 成功与否标识码

 修改历史   :
 日    期   : 2018年10月8日
 修改内容   :
*****************************************************************************/
VOS_UINT32 VOICE_AGENT_SrcChanStart(VOS_VOID)
{
    VOS_UINT32                          ulRet;

    /* if power on log */
    if(g_stDiagCtrlInfo.ulChannelStatus)
    {
        ulRet = mdrv_socp_stop(g_stDiagCtrlInfo.ulChannelId);
        if(ulRet)
        {
            VOICEAGENT_ERR_LOG("stop socp fail");
            LPS_CacheDmaFree(g_stDiagCtrlInfo.pChannelVirAddr,VOICE_AGENT_LOG_SRC_BUF_LEN);
            return VOS_FALSE;
        }

        ulRet = mdrv_clear_socp_buff(g_stDiagCtrlInfo.ulChannelId);
        if(ulRet)
        {
            VOICEAGENT_ERR_LOG("clear socp buff fail");
            LPS_CacheDmaFree(g_stDiagCtrlInfo.pChannelVirAddr,VOICE_AGENT_LOG_SRC_BUF_LEN);
            return VOS_FALSE;
        }
    }

    /* 使能socp编码源通道 */
    ulRet = mdrv_socp_start(g_stDiagCtrlInfo.ulChannelId);
    if(ulRet)
    {
        VOICEAGENT_ERR_LOG("clear socp buff fail");
        LPS_CacheDmaFree(g_stDiagCtrlInfo.pChannelVirAddr,VOICE_AGENT_LOG_SRC_BUF_LEN);
        return VOS_FALSE;
    }
    VOICEAGENT_INFO_LOG1("SrcChanStart:ulRet =",ulRet);
}
static VOS_UINT_PTR VOICE_AGENT_ChannelPhytoVirt(VOS_UINT8 * phyAddr)
{
    if(phyAddr < g_stDiagCtrlInfo.pChannelPhyAddr)
    {
        return NULL;
    }
    return (VOS_UINT_PTR)((phyAddr - g_stDiagCtrlInfo.pChannelPhyAddr) + g_stDiagCtrlInfo.pChannelVirAddr);
}

/*****************************************************************************
* 函 数 名      :VOICE_AGENT_DiagPackWrite
* 功能描述  : 写入数据，更新写指针
* 输入参数  :
* 输出参数  : 无
* 返 回 值     : 无
*****************************************************************************/
static VOS_VOID VOICE_AGENT_DiagPackWrite(socp_buffer_rw_s *pRWBuffer, const void * pPayload, u32 u32DataLen)
{
    VOS_UINT32                          ulTempLen;
    VOS_UINT32                          ulTempLen1;

    if(0 == u32DataLen)
    {
        VOICEAGENT_ERR_LOG("DataLen is invaild");
        return;
    }

    if (NULL == pRWBuffer)
    {
        VOICEAGENT_ERR_LOG("pRWBuffer is NULL");
        return;
    }

    if(pRWBuffer->size > u32DataLen)
    {
        if (memcpy_s(((u8*)pRWBuffer->buffer), pRWBuffer->size, pPayload, u32DataLen) != VOS_OK) {
            VOICEAGENT_INFO_LOG("pRWBuffer->pBuffer memcpy_s fail");
        }
    }
    else
    {
        if((pRWBuffer->buffer) && (pRWBuffer->size))
        {
            ulTempLen = pRWBuffer->size;
            if (memcpy_s(((u8*)pRWBuffer->buffer), pRWBuffer->size, pPayload, ulTempLen) != VOS_OK) {
                VOICEAGENT_INFO_LOG("pRWBuffer->pBuffer memcpy_s fail");
            }
        }
        else
        {
            ulTempLen = 0;
        }

        ulTempLen1 = u32DataLen - pRWBuffer->size;
        if(ulTempLen1)
        {
            if (memcpy_s(pRWBuffer->rb_buffer, pRWBuffer->rb_size, ((u8 *)pPayload + ulTempLen) ,ulTempLen1) != VOS_OK) {
                VOICEAGENT_INFO_LOG("pRWBuffer->pBuffer memcpy_s fail");
            }
        }
    }

    return;
}


/*****************************************************************************
 Function Name   : diag_SrvcPackFirst
 Description     : 不分包时的封装，或分包时，第一包的封装
*****************************************************************************/
VOS_UINT32 VOICE_AGENT_DiagPackSend(VOS_CHAR *pData, VOS_UINT32 ulDataSize)
{
    socp_buffer_rw_s                    stSocpBuf = {NULL, NULL, 0, 0};
    VOS_UINT32                          ulRet = ERR_MSP_FAILURE;
    VOS_UINT8                           ucPcakNum = 0;
    VOS_UINT8		                    i = 0;
	VOICE_AGENT_DIAG_DATA_INFO          *pCheckData = NULL;


	if (0 == ulDataSize%VOICE_AGENT_LOG_BUFFER_LEN)
	{
		ucPcakNum = ulDataSize/VOICE_AGENT_LOG_BUFFER_LEN;
	}
	else
	{
		ucPcakNum = ulDataSize/VOICE_AGENT_LOG_BUFFER_LEN + 1;
	}

	for (i = 0; i < ucPcakNum;i++)
	{
		if ((g_stDiagData[i].ulDataSize > VOICE_AGENT_LOG_DATA_LEN) | (g_stDiagData[i].ulDataSize == 0))
		{
			continue;
		}

		if(mdrv_socp_get_write_buff(g_stDiagCtrlInfo.ulChannelId, &stSocpBuf))
	    {
	    	VOICEAGENT_ERR_LOG("mdrv_socp_get_write_buff fail");
	        return VOS_FALSE;/* 返回失败 */
	    }
	    if((stSocpBuf.size + stSocpBuf.rb_size) >= (g_stDiagData[i].ulDataSize))
	    {
	        stSocpBuf.buffer = (char *)VOICE_AGENT_ChannelPhytoVirt((u8*)stSocpBuf.buffer);
	        stSocpBuf.rb_buffer = (char *)VOICE_AGENT_ChannelPhytoVirt((u8*)stSocpBuf.rb_buffer);
	    }
	    else
	    {
	        continue;
	    }

        if((NULL == stSocpBuf.buffer) && (NULL == stSocpBuf.rb_buffer))
        {
            continue;
        }
        pCheckData = &g_stDiagData[i];
		if(VOS_ERR == VOICE_AGENT_SocpFormatCheck(pCheckData))
		{
            VOICEAGENT_ERR_LOG("SocpFormatCheck ERROR");
			continue;
		}

        VOICE_AGENT_DiagPackWrite(&stSocpBuf, g_stDiagData[i].aucData, g_stDiagData[i].ulDataSize);
        ulRet = (u32)mdrv_socp_write_done(g_stDiagCtrlInfo.ulChannelId, g_stDiagData[i].ulDataSize);
	    if(ulRet)
	    {
	        continue;
	    }


    }
    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : VOICE_AGENT_OM_PROC
 功能描述  : OM传输处理函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : VOS_OK - 成功
             VOS_ERR - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年09月15日
    修改内容   : 新生成函数
*****************************************************************************/
VOS_INT32 VOICE_AGENT_OM_PROC(VOS_VOID)
{
    HIFI_READ_BUFF_INFO                 stReadBuff;
    VOICEAGENT_INFO_LOG("OM_PROC enter");
    if (VOS_OK == VOICE_AGENT_SrcChanCfg())
    {
        SetSocpStatus(VOICE_AGENT_SOCP_CFG_DONE);
    }

    stReadBuff.u32BuffSize = (64 * 1024 );
    if (VOS_OK != VOICE_AGENT_PcieSetRead(VOICE_AGENT_OM_DEV, &stReadBuff, VOICE_AGENT_PcieReadOmDataCB)) {
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
