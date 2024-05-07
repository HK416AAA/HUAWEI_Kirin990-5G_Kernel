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

#include "securec.h"
#include "taf_type_def.h"
#include "dms_dev.h"
#include "dms_dev_i.h"
#include "dms_debug.h"
#include "dms_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_DMS_DEV_UART_C

VOS_UINT32 DMS_UART_SendDataSync(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8 *senBuf = VOS_NULL_PTR;
    errno_t    memResult;

    DMS_PORT_LOGI(dev->portId, "enter.");
    DMS_MNTN_IncComStatsInfo(DMS_UART_WRT_SYNC);

    /* 设备ready才能发送数据 */
    if (dev->devStat == DMS_DEV_NOT_READY) {
        DMS_PORT_LOGE(dev->portId, "DEV not ready.");
        return VOS_ERR;
    }

    if (len == 0) {
        DMS_PORT_LOGE(dev->portId, "VOS_MemAlloc length error, len = %d.", len);
        return VOS_ERR;
    }

    senBuf = (VOS_UINT8 *)VOS_MemAlloc(PS_PID_DMS, DYNAMIC_MEM_PT, len);
    if (senBuf == VOS_NULL_PTR) {
        DMS_PORT_LOGE(dev->portId, "VOS_MemAlloc fail.");
        return VOS_ERR;
    }

    memResult = memcpy_s(senBuf, len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    if (mdrv_vcom_write(dev->handle, (VOS_VOID *)senBuf, len) < 0) { /* 返回值表示完成的字节数，小于零表示错误码 */
        DMS_MNTN_IncComStatsInfo(DMS_UART_WRT_SYNC_ERR);
        VOS_MemFree(PS_PID_DMS, senBuf);
        DMS_PORT_LOGE(dev->portId, "mdrv_vcom_write error.");
        return VOS_ERR;
    }

    VOS_MemFree(PS_PID_DMS, senBuf); /* mdrv_vcom_write为阻塞接口，数据发完后在DMS释放 */

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_UART_SendPrivData(struct DMS_DevInfo *dev, IMM_Zc *data)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    if (DMS_UART_SendDataSync(dev, IMM_ZcGetDataPtr(data), IMM_ZcGetUsedLen(data)) != VOS_OK) {
        DMS_PORT_LOGE(dev->portId, "DMS_UART_SendDataSync error.");
        IMM_ZcFree(data);
        return VOS_ERR;
    }

    IMM_ZcFree(data);
    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_UART_Open(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为ready，端口可以正常发送数据 */
    dev->devStat = DMS_DEV_READY;

    /* 注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, DMS_COM_RcvData) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_REG_RD_CB_ERR);
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_UART_Close(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为not ready，端口不能发送数据 */
    dev->devStat = DMS_DEV_NOT_READY;

    /* 去注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        DMS_MNTN_IncStatsInfo(dev->portId, DMS_REG_RD_CB_ERR);
        return VOS_ERR;
    }

    return VOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
