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
#include "mdrv_udi.h"
#include "Voice_agent_public.h"


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
#define PCIE_DEV_NOT_OPEN (-13)
#define USELESS_NV_NUM 32


VOS_UINT32 g_uselessNv[USELESS_NV_NUM] = {30164, 30166, 30167, 30168, 30169, 30170, 30171,30172,
                                          30173, 30174, 30175, 30176, 30177, 30180, 30181,30182,
                                          30183, 30184, 30185, 30186, 30187, 30189, 30190,30191,
                                          30192, 30193, 30194, 30195, 30196, 30197, 30198,30199};

/*****************************************************************************
  3 外部函数声明
*****************************************************************************/


/*****************************************************************************
  3 函数实现
*****************************************************************************/

/*****************************************************************************
 函 数 名  : VOICE_AGENT_GET_NV_ADDR
 功能描述  : 封装HIFI获取NV数据的地址接口
 输入参数  :
 输出参数  : 无
 返 回 值  :

 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年09月15日
    修改内容   : 新生成函数
*****************************************************************************/

static VOS_VOID * VOICE_AGENT_GET_NV_ADDR(VOS_UINT32 ulModemId, VOS_UINT32 ulNvId)
{
    return mdrv_nv_get_addr(ulModemId, ulNvId);
}
/*****************************************************************************
 函 数 名  : VOICE_AGENT_SendNvToApHifi
 功能描述  : 调用PCIE将NV传输至AHIFI
 输入参数  :
 输出参数  : 无
 返 回 值  :

 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年02月14日
    修改内容   : 新生成函数
*****************************************************************************/
VOS_INT32 VOICE_AGENT_SendNvToApHifi(pcdev_write_done_cb_t pWriteCB)
{
    HIFI_MODEM_NV_DATA_STRU             *pstNvData = NULL;
    VOS_UINT32                          ulNvIndex = HIFI_MODEM_NV_BEGIN_ID;
    VOS_UINT32                          ulNvLength = 0;
    VOS_UINT8                          *pCurAddr  = NULL;
    VOS_UINT32                          ulModemId = 0;
    VOS_UINT32                          *pulNvAddr = NULL;
    VOS_INT32                           ret = 0;
    VOS_INT32                           i;
    bool                                uselessNvFlag = false;

    VOICEAGENT_INFO_LOG("VOICE_AGENT_SendNvToApHifi");
    pstNvData = (HIFI_MODEM_NV_DATA_STRU *)g_stHifDevInfo[VOICE_AGENT_NV_DEV].pNvVirAddr;
    if (NULL == pstNvData)
    {
        return VOS_ERR;
    }
    (void)memset_s(pstNvData, sizeof(HIFI_MODEM_NV_DATA_STRU), 0, sizeof(HIFI_MODEM_NV_DATA_STRU));
    pCurAddr = pstNvData->ulData;

    /* 调用底软的接口读取hifi 相关的NV */
    for (ulNvIndex = HIFI_MODEM_NV_BEGIN_ID; ulNvIndex < HIFI_MODEM_NV_END_ID; ulNvIndex++)
    {
        /*跳过暂时不使用的NV，减少遍历时间避免回复VC超时*/
        if (ulNvIndex >= g_uselessNv[0]) {
            for (i = 0; i < USELESS_NV_NUM; i++) {
                if (ulNvIndex == g_uselessNv[i]) {
                    uselessNvFlag = true;
                    break;
                }
            }
            if (uselessNvFlag) {
                uselessNvFlag = false;
                continue;
            }
        }

        if (0 != VOICE_AGENT_GET_NV_LENGTH(ulNvIndex, &ulNvLength))
        {
            VOICEAGENT_WARN_LOG1("VOICE_AGENT_Init get nv length error,nv id",ulNvIndex);
            continue;
        }

        pstNvData->stNvID[pstNvData->ulHifiNvNum].ulHifiModemNvId   = ulNvIndex;
        pstNvData->stNvID[pstNvData->ulHifiNvNum].ulNvDataLen       = ulNvLength;


        for (ulModemId = 0; ulModemId < 2; ulModemId ++)
        {
            pstNvData->stNvID[pstNvData->ulHifiNvNum].ulNvDataOffset[ulModemId] = pCurAddr - &pstNvData->ulData[0];
            pulNvAddr = VOICE_AGENT_GET_NV_ADDR(ulModemId,ulNvIndex);
            if (NULL != pulNvAddr)
            {
                (void)memcpy_s(pCurAddr, ulNvLength, pulNvAddr, ulNvLength);
                pCurAddr = pCurAddr + ulNvLength;
            }
        }
        pstNvData->ulHifiNvNum ++;

    }

    pstNvData->ulHifiNvMagic = B5000_NV_READY_MAIGC_WORD;

    ret = VOICE_AGENT_PcieSetWrite(VOICE_AGENT_NV_DEV, (VOS_UINT8*)pstNvData, (VOS_UINT8*)g_stHifDevInfo[VOICE_AGENT_NV_DEV].pNvPhyAddr, sizeof(HIFI_MODEM_NV_DATA_STRU), pWriteCB);

    return ret;

}
void VOICE_AGENT_EVENT_CB(pcdev_evt_e evt)
{
    HIFI_MODEM_NV_DATA_STRU *pstNvData = NULL;
    pstNvData = (HIFI_MODEM_NV_DATA_STRU *)g_stHifDevInfo[VOICE_AGENT_NV_DEV].pNvVirAddr;

    VOICEAGENT_ERR_LOG("VOICE_AGENT_EVENT_CB");

    if (VOS_OK != VOICE_AGENT_PcieSetWrite(VOICE_AGENT_NV_DEV, (VOS_UINT8*)pstNvData, (VOS_UINT8*)g_stHifDevInfo[VOICE_AGENT_NV_DEV].pNvPhyAddr, sizeof(HIFI_MODEM_NV_DATA_STRU), VOICE_AGENT_PcieWriteNvDataCB))
    {
        VOICEAGENT_ERR_LOG("VOICE_AGENT_EVENT_CB send NV fail");
    }

}

/*****************************************************************************
 函 数 名  : VOICE_AGENT_NV_PROC
 功能描述  : NV传输处理函数
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
VOS_INT32 VOICE_AGENT_NV_PROC(VOS_VOID)
{
    HIFI_MODEM_NV_DATA_STRU             *pstNvData = NULL;
    VOS_UINT_PTR                        ulPhyAddr  = VOS_NULL_PTR;
    VOS_INT32                           ulRet;
    VOS_INT32                           ulCtlRet;

    pstNvData = (HIFI_MODEM_NV_DATA_STRU*)VOS_UnCacheMemAllocDebug(sizeof(HIFI_MODEM_NV_DATA_STRU),VOS_UNCACHE_MEM_ADDR_ALIGN_4K,&ulPhyAddr,(VOS_UINT32)0xea000000);

    if (NULL == pstNvData)
    {
        return VOS_ERR;
    }
    g_stHifDevInfo[VOICE_AGENT_NV_DEV].pNvVirAddr = (VOS_UINT32 *)pstNvData;
    g_stHifDevInfo[VOICE_AGENT_NV_DEV].pNvPhyAddr = (VOS_UINT32 *)ulPhyAddr;

    ulRet = VOICE_AGENT_SendNvToApHifi(VOICE_AGENT_PcieWriteNvDataCB);

    if (ulRet == PCIE_DEV_NOT_OPEN) {
        ulCtlRet = mdrv_udi_ioctl(g_stHifDevInfo[VOICE_AGENT_NV_DEV].slVoiceagentHdl, VOICEAGENT_IOCTL_SET_EVT_CB, VOICE_AGENT_EVENT_CB);
        if (ulCtlRet != 0) {
            VOICEAGENT_ERR_LOG("VOICE_AGENT_EVENT_CB set fail");
        } else {
            VOICEAGENT_ERR_LOG("VOICE_AGENT_EVENT_CB set succ");
        }
        return VOS_ERR;
    }

    return ulRet;
}
#endif
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
