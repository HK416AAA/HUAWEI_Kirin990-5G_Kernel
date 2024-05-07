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

#ifndef _DMS_DEV_H_
#define _DMS_DEV_H_

#include "vos.h"
#include "mdrv_maa.h"
#include "mdrv_vcom.h"
#include "dms_port_i.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define DMS_MAA_MEM_ALLOC(pid, length) mdrv_maa_alloc_lock((length), pid, MAA_ACESS)
#define DMS_MAA_MEM_FREE(pid, addr) mdrv_maa_free_lock(addr, pid)
#define DMS_INVALID_HANDLE (-1)

#if (FEATURE_AT_HSUART == FEATURE_ON)
#define DMS_HSUART_MAX_RCV_LEN      4096
#endif

enum DMS_DevState {
    DMS_DEV_NOT_READY,
    DMS_DEV_READY,

    DMS_DEV_STATUS_BUTT,
};
typedef VOS_UINT8 DMS_DevStateUint8;

#define DMS_DEV_ITEM(port_id, dev_type, dev_id, dev_ops)  \
    [port_id] = { \
    .handle = DMS_INVALID_HANDLE, \
    .portId = port_id, \
    .devId = dev_id, \
    .devType = dev_type, \
    .devStat = DMS_DEV_NOT_READY, \
    .portFuncTbl = VOS_NULL_PTR, \
    .ops = dev_ops,}

enum DMS_DevType {
    DMS_DEV_TYPE_COM,        /* ±íÊ¾APPVCOM/ACM/NCM CTRL/SOCK */
    DMS_DEV_TYPE_NCM_DATA,
    DMS_DEV_TYPE_ACM_MODEM,
    DMS_DEV_TYPE_UART,
#if (FEATURE_AT_HSUART == FEATURE_ON)
    DMS_DEV_TYPE_HSUART,
#endif

    DMS_DEV_TYPE_BUTT,
};
typedef VOS_UINT8 DMS_DevTypeUint8;

struct DMS_DevInfo {
    VOS_INT32                           handle;
    DMS_PortIdUint16                    portId;
    VOS_UINT16                          devId;
    DMS_DevStateUint8                   devStat;
    enum vcom_dev_type                  devType;
    VOS_UINT8                           reserv[6];

    const struct DMS_PortFuncTbl       *portFuncTbl;
    const struct DMS_DevOps            *ops;
};

struct DMS_DevOps {
    VOS_UINT32 (*Init)(struct DMS_DevInfo *dev);
    VOS_UINT32 (*Open)(struct DMS_DevInfo *dev);
    VOS_UINT32 (*Close)(struct DMS_DevInfo *dev);
    VOS_UINT32 (*SendDataSync)(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len);
    VOS_UINT32 (*SendDataAsync)(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len);
    VOS_UINT32 (*SendPrivData)(struct DMS_DevInfo *dev, IMM_Zc *data);
    VOS_UINT32 (*SendMscCmd)(struct DMS_DevInfo *dev, struct DMS_MscInfo *msc);
    VOS_UINT32 (*ChangeConnSpeed)(struct DMS_DevInfo *dev, struct DMS_ConnSpeed *connSpeed);
    VOS_UINT32 (*ChangeConnLinkState)(struct DMS_DevInfo *dev, VOS_UINT32 linkStatus);
    VOS_UINT32 (*ConfigFlowCtrl)(struct DMS_DevInfo *dev, VOS_UINT32 rtsFlag, VOS_UINT32 ctsFlag);
    VOS_UINT32 (*SetCharFrame)(struct DMS_DevInfo *dev, VOS_UINT32 udiDataLen, VOS_UINT32 udiStpLen, VOS_UINT32 udiParity);
    VOS_UINT32 (*SetBaud)(struct DMS_DevInfo *dev, VOS_UINT32 baudRate);
    VOS_UINT32 (*FlushTxData)(struct DMS_DevInfo *dev);
};

struct DMS_DevInfo* DMS_DEV_GetDevEntityByPortId(DMS_PortIdUint16 portId);
const struct DMS_PortFuncTbl* DMS_DEV_GetDevPortTblByPortId(DMS_PortIdUint16 portId);
VOS_INT32  DMS_DEV_GetDevHandlerByPortId(DMS_PortIdUint16 portId);
VOS_INT    DMS_COMM_CBCheck(DMS_PortIdUint16 portId);
VOS_VOID   DMS_DEV_RcvConnectStateInd(DMS_PortIdUint16 portId, VOS_UINT8 connState);

VOS_UINT32 DMS_COM_Init(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_COM_Open(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_COM_Close(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_COM_SendDataAsync(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len);
VOS_VOID   DMS_COM_RcvData(VOS_VOID *priv, struct vcom_rw_info *rwInfo);
VOS_UINT32 DMS_NCM_DataInit(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_NCM_ChangeConnSpeed(struct DMS_DevInfo *dev, struct DMS_ConnSpeed *connSpeed);
VOS_UINT32 DMS_NCM_ChangeConnLinkState(struct DMS_DevInfo *dev, VOS_UINT32 linkStatus);
VOS_UINT32 DMS_MDM_Open(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_MDM_Close(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_MDM_SendDataAsync(struct DMS_DevInfo *dev, VOS_UINT8 *dataBuff, VOS_UINT16 len);
VOS_UINT32 DMS_MDM_SendMscCmd(struct DMS_DevInfo *dev, struct DMS_MscInfo *msc);
VOS_UINT32 DMS_MDM_SendPrivData(struct DMS_DevInfo *dev, IMM_Zc *data);
VOS_UINT32 DMS_UART_SendDataSync(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len);
VOS_UINT32 DMS_UART_Open(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_UART_Close(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_UART_SendPrivData(struct DMS_DevInfo *dev, IMM_Zc *data);
#if (FEATURE_AT_HSUART == FEATURE_ON)
VOS_UINT32 DMS_HSUART_Open(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_HSUART_Close(struct DMS_DevInfo *dev);
VOS_UINT32 DMS_HSUART_SendDataAsync(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len);
VOS_UINT32 DMS_HSUART_SendMscCmd(struct DMS_DevInfo *dev, struct DMS_MscInfo *msc);
VOS_UINT32 DMS_HSUART_CfgFlowCtrl(struct DMS_DevInfo *dev, VOS_UINT32 rtsFlag, VOS_UINT32 ctsFlag);
VOS_UINT32 DMS_HSUART_SetCharFrame(struct DMS_DevInfo *dev, VOS_UINT32 udiDataLen, VOS_UINT32 udiStpLen, VOS_UINT32 udiParity);
VOS_UINT32 DMS_HSUART_SetBaud(struct DMS_DevInfo *dev, VOS_UINT32 baudRate);
VOS_UINT32 DMS_HSUART_SendPrivData(struct DMS_DevInfo *dev, IMM_Zc *data);
VOS_UINT32 DMS_HSUART_FlushTxData(struct DMS_DevInfo *dev);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

