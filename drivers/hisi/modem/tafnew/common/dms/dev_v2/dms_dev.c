/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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

#include "dms_dev.h"

#include "securec.h"
#include "vos.h"
#include "imm_interface.h"
#include "taf_type_def.h"
#include "dms_dev_i.h"
#include "dms_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_DMS_DEV_C

VOS_INT DMS_COMM_CBCheck(DMS_PortIdUint16 portId)
{
    const struct DMS_PortFuncTbl* portTblInfo = VOS_NULL_PTR;
    VOS_INT32 handle;

    DMS_LOGI("enter.");

    if (portId >= DMS_PORT_BUTT) {
        DMS_LOGE("portId:%d is invalid.", portId);
        return VOS_ERROR;
    }

    handle = DMS_DEV_GetDevHandlerByPortId(portId);
    if (handle == DMS_INVALID_HANDLE) {
        DMS_PORT_LOGE(portId, "handle is null.");
        return VOS_ERROR;
    }

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(portId);
    if (portTblInfo == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "portTblInfo is not registered.");
        return VOS_ERROR;
    }

    return VOS_OK;
}

VOS_VOID *DMS_DEV_InitCtx(DMS_PortIdUint16 portId, const struct DMS_PortFuncTbl *portFuncTbl)
{
    struct DMS_DevInfo  *devInfo = VOS_NULL_PTR;

    DMS_PORT_LOGI(portId, "enter.");

    if (portFuncTbl == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "portFuncTbl null.");
        return VOS_NULL_PTR;
    }

    devInfo = DMS_DEV_GetDevEntityByPortId(portId);
    devInfo->portFuncTbl = portFuncTbl;

    return devInfo;
}

VOS_VOID DMS_DEV_RcvConnectStateInd(DMS_PortIdUint16 portId, VOS_UINT8 connState)
{
    const struct DMS_PortFuncTbl* portTblInfo = VOS_NULL_PTR;

    DMS_PORT_LOGI(portId, "enter.");

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(portId);
    if (portTblInfo == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "portTblInfo not registered.");
        return;
    }

    if (portTblInfo->RcvConnectInd == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "RcvConnectInd is not registered.");
        return;
    }

    portTblInfo->RcvConnectInd(portId, connState);
}

STATIC VOS_UINT32 DMS_DEV_Init(VOS_VOID *dev)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (devInfo == VOS_NULL_PTR) {
        DMS_LOGE("dev is null.");
        return VOS_ERR;
    }

    if (devInfo->handle != DMS_INVALID_HANDLE) {
        DMS_PORT_LOGE(devInfo->portId, "dev is inited.");
        return VOS_ERR;
    }

    if (devInfo->ops == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "ops is null.");
        return VOS_ERR;
    }

    if (devInfo->ops->Init == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->Init(devInfo);
}

VOS_VOID DMS_DEV_InitDev(VOS_VOID)
{
    struct DMS_DevInfo      *devInfo = VOS_NULL_PTR;

    DMS_LOGI("enter.");

    /* 新平台上电后INIT端口，INIT操作包括：1、获取设备句柄；2、注册设备ready的回调函数给底软
     * 设备ready时，通知DMS打开（使能）端口，即DMS注册（register）AT回调给底软
     * 设备not ready时，通知DMS关闭（去使能）端口，即DMS去注册（deregister）AT回调函数 */
    for (VOS_UINT32 i = 0; i < DMS_PORT_BUTT; i++) {
        devInfo = DMS_DEV_GetDevEntityByPortId((VOS_UINT8)i);
        DMS_DEV_Init(devInfo);
    }

    /* 开启维测定时器 */
    if (DMS_StartTimer(DMS_DEFAULT_ERR_RPT_TIME, TI_DMS_TIMER_MNTN_INFO) != VOS_OK) {
        DMS_LOGE("DMS_DEV_InitDev:ERROR:DMS_StartTimer!");
    }
}

STATIC VOS_UINT32 DMS_DEV_CommCheck(struct DMS_DevInfo *dev)
{
    DMS_LOGI("enter.");

    if (dev == VOS_NULL_PTR) {
        DMS_LOGE("dev is null.");
        return VOS_ERR;
    }

    if (dev->handle == DMS_INVALID_HANDLE) {
        DMS_PORT_LOGE(dev->portId, "dev handle is invalid.");
        return VOS_ERR;
    }

    if (dev->ops == VOS_NULL_PTR) {
        DMS_PORT_LOGE(dev->portId, "dev ops is null.");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_DEV_SendDataSync(VOS_VOID *dev, VOS_UINT8 *data, VOS_UINT16 len)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    /* 由上层保证下发的data和len不为空指针和0 */
    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->SendDataSync == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->SendDataSync(dev, data, len);
}

VOS_UINT32 DMS_DEV_SendDataAsync(VOS_VOID *dev, VOS_UINT8 *data, VOS_UINT16 len)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    /* 由上层保证下发的data和len不为空指针和0 */
    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->SendDataAsync == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->SendDataAsync(dev, data, len);
}

VOS_UINT32 DMS_DEV_SendPrivData(VOS_VOID *dev, IMM_Zc *immZc)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");
    /* 由上层保证下发的immZc不为空指针 */
    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        IMM_ZcFree(immZc);
        return VOS_ERR;
    }

    if (devInfo->ops->SendPrivData == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        IMM_ZcFree(immZc);
        return VOS_ERR;
    }

    return devInfo->ops->SendPrivData(dev, immZc);
}

VOS_UINT32 DMS_DEV_SendMscCmd(VOS_VOID *dev, struct DMS_MscInfo *msc)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->SendMscCmd == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->SendMscCmd(dev, msc);
}

VOS_UINT32 DMS_DEV_SetIpv6Dns(VOS_VOID *dev, struct DMS_NcmIpv6Dns *dns)
{
    return VOS_OK;
}

VOS_UINT32 DMS_DEV_ChangeFlowCtrl(VOS_VOID *dev, VOS_UINT32 enableFlg)
{
    return VOS_OK;
}

VOS_UINT32 DMS_DEV_ChangeConnSpeed(VOS_VOID *dev, struct DMS_ConnSpeed *connSpeed)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->ChangeConnSpeed == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->ChangeConnSpeed(dev, connSpeed);
}

VOS_UINT32 DMS_DEV_ChangeConnLinkState(VOS_VOID *dev, VOS_UINT32 linkStatus)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->ChangeConnLinkState == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->ChangeConnLinkState(dev, linkStatus);
}

VOS_UINT32 DMS_DEV_Open(VOS_VOID *dev)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->Open == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->Open(devInfo);
}

VOS_UINT32 DMS_DEV_Close(VOS_VOID *dev)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->Close == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->Close(dev);
}

VOS_INT32 DMS_DEV_GetHandle(VOS_VOID *dev)
{
    struct DMS_DevInfo *devInfo = dev;
    return devInfo->handle;
}

#if (FEATURE_AT_HSUART == FEATURE_ON)

VOS_UINT32 DMS_DEV_ConfigFlowCtrl(VOS_VOID *dev, VOS_UINT32 rtsFlag, VOS_UINT32 ctsFlag)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->ConfigFlowCtrl == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->ConfigFlowCtrl(dev, rtsFlag, ctsFlag);
}

VOS_UINT32 DMS_DEV_SetCharFrame(VOS_VOID *dev, VOS_UINT32 udiDataLen, VOS_UINT32 udiStpLen, VOS_UINT32 udiParity)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->SetCharFrame == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->SetCharFrame(dev, udiDataLen, udiStpLen, udiParity);
}

VOS_UINT32 DMS_DEV_SetBaud(VOS_VOID *dev, VOS_UINT32 baudRate)
{
    struct DMS_DevInfo *devInfo = dev;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->SetBaud == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    return devInfo->ops->SetBaud(dev, baudRate);
}

VOS_UINT32 DMS_DEV_SetACShell(VOS_VOID *dev, VOS_UINT32 mode)
{
    return VOS_OK;
}

VOS_UINT32 DMS_DEV_FlushTxData(VOS_VOID *dev)
{
    struct DMS_DevInfo *devInfo = dev;
    VOS_UINT32 ret;

    DMS_LOGI("enter.");

    if (DMS_DEV_CommCheck(devInfo) != VOS_OK) {
        return VOS_ERR;
    }

    if (devInfo->ops->FlushTxData == VOS_NULL_PTR) {
        DMS_PORT_LOGE(devInfo->portId, "dev ops func is null.");
        return VOS_ERR;
    }

    ret = devInfo->ops->FlushTxData(dev);

    return ret;
}
#endif

STATIC const struct DMS_DevOps g_dmsDevOps[] = {
    [DMS_DEV_TYPE_COM] = {
        .Init                   = DMS_COM_Init,
        .Open                   = DMS_COM_Open,
        .Close                  = DMS_COM_Close,
        .SendDataAsync          = DMS_COM_SendDataAsync,
    },

    [DMS_DEV_TYPE_NCM_DATA] = {
        .Init                   = DMS_NCM_DataInit,
        .ChangeConnSpeed        = DMS_NCM_ChangeConnSpeed,
        .ChangeConnLinkState    = DMS_NCM_ChangeConnLinkState,
    },

    [DMS_DEV_TYPE_ACM_MODEM] = {
        .Init                   = DMS_COM_Init,
        .Open                   = DMS_MDM_Open,
        .Close                  = DMS_MDM_Close,
        .SendDataAsync          = DMS_MDM_SendDataAsync,
        .SendMscCmd             = DMS_MDM_SendMscCmd,
        .SendPrivData           = DMS_MDM_SendPrivData,
    },

    [DMS_DEV_TYPE_UART] = {
        .Init                   = DMS_COM_Init,
        .Open                   = DMS_UART_Open,
        .Close                  = DMS_UART_Close,
        .SendDataSync           = DMS_UART_SendDataSync,
        .SendPrivData           = DMS_UART_SendPrivData,
    },

#if (FEATURE_AT_HSUART == FEATURE_ON)
    [DMS_DEV_TYPE_HSUART] = {
        .Init                   = DMS_COM_Init,
        .Open                   = DMS_HSUART_Open,
        .Close                  = DMS_HSUART_Close,
        .SendDataSync           = DMS_UART_SendDataSync,
        .SendDataAsync          = DMS_HSUART_SendDataAsync,
        .SendMscCmd             = DMS_HSUART_SendMscCmd,
        .ConfigFlowCtrl         = DMS_HSUART_CfgFlowCtrl,
        .SetCharFrame           = DMS_HSUART_SetCharFrame,
        .SetBaud                = DMS_HSUART_SetBaud,
        .SendPrivData           = DMS_HSUART_SendPrivData,
        .FlushTxData            = DMS_HSUART_FlushTxData,
    },
#endif
};

struct DMS_DevInfo g_dmsDevInfo[] =  {
    DMS_DEV_ITEM(DMS_PORT_APP,      VCOM_DEV_APP,      VCOM_APP_0,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP1,     VCOM_DEV_APP,      VCOM_APP_1,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
#if (FEATURE_VCOM_EXT == FEATURE_ON)
    DMS_DEV_ITEM(DMS_PORT_APP2,     VCOM_DEV_APP,      VCOM_APP_2,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP3,     VCOM_DEV_APP,      VCOM_APP_3,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP4,     VCOM_DEV_APP,      VCOM_APP_4,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP5,     VCOM_DEV_APP,      VCOM_APP_5,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP6,     VCOM_DEV_APP,      VCOM_APP_6,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP7,     VCOM_DEV_APP,      VCOM_APP_7,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP8,     VCOM_DEV_APP,      VCOM_APP_8,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP9,     VCOM_DEV_APP,      VCOM_APP_9,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP10,    VCOM_DEV_APP,      VCOM_APP_10,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP11,    VCOM_DEV_APP,      VCOM_APP_11,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP12,    VCOM_DEV_APP,      VCOM_APP_12,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP13,    VCOM_DEV_APP,      VCOM_APP_13,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP14,    VCOM_DEV_APP,      VCOM_APP_14,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP15,    VCOM_DEV_APP,      VCOM_APP_15,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP16,    VCOM_DEV_APP,      VCOM_APP_16,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP17,    VCOM_DEV_APP,      VCOM_APP_17,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP18,    VCOM_DEV_APP,      VCOM_APP_18,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP19,    VCOM_DEV_APP,      VCOM_APP_19,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP20,    VCOM_DEV_APP,      VCOM_APP_20,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP21,    VCOM_DEV_APP,      VCOM_APP_21,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP22,    VCOM_DEV_APP,      VCOM_APP_22,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP23,    VCOM_DEV_APP,      VCOM_APP_23,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP24,    VCOM_DEV_APP,      VCOM_APP_24,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP25,    VCOM_DEV_APP,      VCOM_APP_25,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP26,    VCOM_DEV_APP,      VCOM_APP_26,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP27,    VCOM_DEV_APP,      VCOM_APP_27,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP28,    VCOM_DEV_APP,      VCOM_APP_28,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP29,    VCOM_DEV_APP,      VCOM_APP_29,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP30,    VCOM_DEV_APP,      VCOM_APP_30,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP31,    VCOM_DEV_APP,      VCOM_APP_31,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP32,    VCOM_DEV_APP,      VCOM_APP_32,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP33,    VCOM_DEV_APP,      VCOM_APP_33,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP34,    VCOM_DEV_APP,      VCOM_APP_34,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP35,    VCOM_DEV_APP,      VCOM_APP_35,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP36,    VCOM_DEV_APP,      VCOM_APP_36,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP37,    VCOM_DEV_APP,      VCOM_APP_37,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP38,    VCOM_DEV_APP,      VCOM_APP_38,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP39,    VCOM_DEV_APP,      VCOM_APP_39,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP40,    VCOM_DEV_APP,      VCOM_APP_40,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP41,    VCOM_DEV_APP,      VCOM_APP_41,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP42,    VCOM_DEV_APP,      VCOM_APP_42,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP43,    VCOM_DEV_APP,      VCOM_APP_43,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP44,    VCOM_DEV_APP,      VCOM_APP_44,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP45,    VCOM_DEV_APP,      VCOM_APP_45,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP46,    VCOM_DEV_APP,      VCOM_APP_46,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP47,    VCOM_DEV_APP,      VCOM_APP_47,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP48,    VCOM_DEV_APP,      VCOM_APP_48,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP49,    VCOM_DEV_APP,      VCOM_APP_49,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP50,    VCOM_DEV_APP,      VCOM_APP_50,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP51,    VCOM_DEV_APP,      VCOM_APP_51,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_APP52,    VCOM_DEV_APP,      VCOM_APP_52,   &g_dmsDevOps[DMS_DEV_TYPE_COM]),
#endif

    DMS_DEV_ITEM(DMS_PORT_PCUI,     VCOM_DEV_ACM,      VCOM_ACM_AT,      &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_CTRL,     VCOM_DEV_ACM,      VCOM_ACM_CTRL,    &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_PCUI2,    VCOM_DEV_ACM,      VCOM_ACM_SKYTONE, &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_NCM_CTRL, VCOM_DEV_NCM_CTRL, VCOM_NCM_CTRL_ID, &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_UART,     VCOM_DEV_UART,     VCOM_UART_ID,     &g_dmsDevOps[DMS_DEV_TYPE_UART]),
    DMS_DEV_ITEM(DMS_PORT_SOCK,     VCOM_DEV_SOCK,     VCOM_SOCK_ID,     &g_dmsDevOps[DMS_DEV_TYPE_COM]),
    DMS_DEV_ITEM(DMS_PORT_MODEM,    VCOM_DEV_PPP,      VCOM_PPP_ID,      &g_dmsDevOps[DMS_DEV_TYPE_ACM_MODEM]),

#if (FEATURE_AT_HSUART == FEATURE_ON)
    DMS_DEV_ITEM(DMS_PORT_HSUART,   VCOM_DEV_HSUART,   VCOM_HSUART_ID,   &g_dmsDevOps[DMS_DEV_TYPE_HSUART]),
#endif

    DMS_DEV_ITEM(DMS_PORT_NCM_DATA, VCOM_DEV_NCM_DATA, VCOM_NCM_DATA_ID, &g_dmsDevOps[DMS_DEV_TYPE_NCM_DATA]),
#if (FEATURE_AT_PROXY == FEATURE_ON)
    DMS_DEV_ITEM(DMS_PORT_ATP,      VCOM_DEV_APP,      VCOM_ATP_ID,      &g_dmsDevOps[DMS_DEV_TYPE_COM]),
#endif
};

struct DMS_DevInfo* DMS_DEV_GetDevEntityByPortId(DMS_PortIdUint16 portId)
{
    return &g_dmsDevInfo[portId];
}

const struct DMS_PortFuncTbl* DMS_DEV_GetDevPortTblByPortId(DMS_PortIdUint16 portId)
{
    return g_dmsDevInfo[portId].portFuncTbl;
}

VOS_INT32 DMS_DEV_GetDevHandlerByPortId(DMS_PortIdUint16 portId)
{
    return g_dmsDevInfo[portId].handle;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
