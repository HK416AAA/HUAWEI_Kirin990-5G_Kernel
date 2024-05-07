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

#include "taf_type_def.h"
#include "dms_dev_i.h"
#include "dms_debug.h"
#include "dms_task.h"

#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_DMS_DEV_COMMON_C

STATIC VOS_VOID DMS_COM_RcvReadyCB(VOS_VOID *priv, VOS_INT32 ready)
{
    DMS_PortIdUint16 portId;

    DMS_LOGI("enter.");

    if (priv == VOS_NULL_PTR || ready < 0 || ready > 1) {
        DMS_LOGE("input para invalid, [priv=%d], [ready=%d]!", priv, ready);
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId >= DMS_PORT_BUTT) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    DMS_MNTN_IncStatsInfo(portId, DMS_READY_CB);
    DMS_PORT_LOGI(portId, "ready=%d.", ready);

    if (ready == 1) {
        DMS_DEV_RcvConnectStateInd(portId, DMS_CONNECT);
    } else {
        DMS_DEV_RcvConnectStateInd(portId, DMS_DISCONNECT);
    }
}

VOS_UINT32 DMS_COM_Init(struct DMS_DevInfo *dev)
{
    VOS_INT handle;

    DMS_PORT_LOGI(dev->portId, "enter.");

    handle = mdrv_vcom_open(dev->devType, dev->devId);
    if (handle < 0) {
        DMS_PORT_LOGE(dev->portId, "dev handle is invalid.");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_OPEN_ERR);
        return VOS_ERR;
    }

    dev->handle = handle;

    /* 设置对应打开设备的用户私有结构体，设置后通过priv参数带回到各回调函数中 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_USER_DATA, (VOS_VOID *)&dev->portId) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_USER_DATA failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_USER_DATA_ERR);
        return VOS_ERR;
    }

    /* 注册设备是否ready接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READY_CB, DMS_COM_RcvReadyCB) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READY_CB failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_REG_READY_CB_ERR);
        return VOS_ERR;
    }

    return VOS_OK;
}

STATIC struct vcom_rw_info *DMS_COM_GetWriteBuff(DMS_PortIdUint16 portId, struct vcom_rw_info *rwInfo,
    VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8 *senBuf = VOS_NULL_PTR;
    errno_t    memResult;

    if (len == 0) {
        DMS_PORT_LOGE(portId, "VOS_MemAlloc length error, len = %d.", len);
        return VOS_NULL_PTR;
    }

    senBuf = (VOS_UINT8 *)VOS_MemAlloc(PS_PID_DMS, DYNAMIC_MEM_PT, len);
    if (senBuf == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "VOS_MemAlloc fail.");
        return VOS_NULL_PTR;
    }

    memResult = memcpy_s(senBuf, len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    rwInfo->buf  = senBuf;
    rwInfo->size = len;

    return rwInfo;
}

VOS_VOID DMS_COM_RcvData(VOS_VOID *priv, struct vcom_rw_info *rwInfo)
{
    const struct DMS_PortFuncTbl *portTblInfo = VOS_NULL_PTR;
    struct DMS_DevInfo           *devInfo     = VOS_NULL_PTR;
    DMS_PortIdUint16              portId;

    DMS_LOGI("enter.");

    if (priv == VOS_NULL_PTR) {
        DMS_LOGE("priv is NULL!");
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (DMS_COMM_CBCheck(portId) != VOS_OK) {
        return;
    }

    DMS_MNTN_IncStatsInfo(portId, DMS_RD_CB);

    if (!(rwInfo != VOS_NULL_PTR && rwInfo->buf != VOS_NULL_PTR && rwInfo->size != 0)) {
        DMS_LOGE("rwInfo is invalid!");
        DMS_MNTN_IncStatsInfo(portId, DMS_RD_BUFF_ERR);
        return;
    }

    devInfo = DMS_DEV_GetDevEntityByPortId(portId);

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(portId);
    if (portTblInfo->RcvData == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "RcvData is not registered!");
        return;
    }

    /* 调用DMS port模块的回调函数 */
    (VOS_VOID)portTblInfo->RcvData(portId, (VOS_UINT8 *)rwInfo->buf, rwInfo->size);

    /* 数据处理完释放 */
    if (mdrv_vcom_ioctl(devInfo->handle, VCOM_IOCTL_CLEAR_READ_BUFF, (VOS_VOID *)rwInfo) != MDRV_OK) {
        DMS_MNTN_IncStatsInfo(portId, DMS_RETURN_BUFF_ERR);
        DMS_PORT_LOGE(portId, "VCOM_IOCTL_CLEAR_READ_BUFF failed!");
    }
}

STATIC VOS_VOID DMS_COM_RcvWriteDone(void *priv, struct vcom_rw_info *rwInfo)
{
    DMS_PortIdUint16 portId;

    DMS_LOGI("enter.");

    if (priv == VOS_NULL_PTR || rwInfo == VOS_NULL_PTR) {
        DMS_LOGE("input para invalid, [priv=%d], [rwInfo=%d]!", priv, rwInfo);
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId >= DMS_PORT_BUTT) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    DMS_MNTN_IncStatsInfo(portId, DMS_WRT_CB);

    /* modem端口走SPE通道，其他异步写端口走EICC通道，使用的是系统内存 */
    if (rwInfo->buf != VOS_NULL_PTR) {
        VOS_MemFree(PS_PID_DMS, rwInfo->buf);
        rwInfo->buf = VOS_NULL_PTR;
    }
}

VOS_UINT32 DMS_COM_Open(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为ready，端口可以正常发送数据 */
    dev->devStat = DMS_DEV_READY;

    /* 注册下行异步写回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_WRITE_CB, DMS_COM_RcvWriteDone) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_WRITE_CB failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_REG_WRT_CB_ERR);
        return VOS_ERR;
    }

    /* 注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, DMS_COM_RcvData) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_REG_RD_CB_ERR);
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_COM_Close(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为not ready，端口不能发送数据 */
    dev->devStat = DMS_DEV_NOT_READY;

    /* 去注册下行异步写回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_WRITE_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_WRITE_CB failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_REG_WRT_CB_ERR);
        return VOS_ERR;
    }

    /* 去注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_REG_RD_CB_ERR);
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_COM_SendDataAsync(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len)
{
    struct vcom_rw_info rwInfo;

    DMS_PORT_LOGI(dev->portId, "enter.");
    DMS_MNTN_IncStatsInfo(dev->portId, DMS_WRT_ASYNC);

    /* 设备ready才能发送数据 */
    if (dev->devStat == DMS_DEV_NOT_READY) {
        DMS_PORT_LOGE(dev->portId, "DEV not ready.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(&rwInfo, sizeof(rwInfo), 0x00, sizeof(rwInfo));

    if (DMS_COM_GetWriteBuff(dev->portId, &rwInfo, data, len) == VOS_NULL_PTR) {
        DMS_PORT_LOGE(dev->portId, "DMS_COM_GetWriteBuff fail!");
        return VOS_ERR;
    }

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_WRITE_ASYNC, (VOS_VOID *)&rwInfo) != VOS_OK) {
        DMS_PORT_LOGE(dev->portId, "write data async error.");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_WRT_ASYNC_ERR);
        DMS_COM_RcvWriteDone((VOS_VOID *)&dev->portId, &rwInfo);
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
