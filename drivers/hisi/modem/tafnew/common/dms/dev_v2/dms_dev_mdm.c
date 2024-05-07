/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
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

#include "dms_dev_i.h"
#include "dms_debug.h"
#include "taf_type_def.h"
#include "imm_interface.h"

#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_DMS_DEV_MDM_C

STATIC struct vcom_rw_info *DMS_MDM_SysTransMaa(DMS_PortIdUint16 portId, struct vcom_rw_info *rwInfo,
    VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT64 maaAddr = 0;
    errno_t    memResult;

    maaAddr = DMS_MAA_MEM_ALLOC(PS_PID_DMS, len);
    if (maaAddr == 0) {
        DMS_PORT_LOGE(portId, "DMS_MAA_MEM_ALLOC fail!");
        return VOS_NULL_PTR;
    }

    memResult = memcpy_s((VOS_VOID *)(VOS_UINT_PTR)(maaAddr & 0xFFFFFFFF), len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    rwInfo->maa_addr = maaAddr;
    rwInfo->size = len;

    return rwInfo;
}

STATIC VOS_VOID DMS_MDM_RcvData(VOS_VOID *priv, struct vcom_rw_info *rwInfo)
{
    const struct DMS_PortFuncTbl *portTblInfo = VOS_NULL_PTR;
    IMM_Zc                       *immZc       = VOS_NULL_PTR;
    DMS_PortIdUint16              portId;

    DMS_LOGI("enter.");
    DMS_MNTN_IncComStatsInfo(DMS_MDM_RD_CB);

    if (priv == VOS_NULL_PTR) {
        DMS_LOGE("priv is NULL!");
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_MODEM) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    if (DMS_COMM_CBCheck(portId) != VOS_OK) {
        return;
    }

    if (!(rwInfo != VOS_NULL_PTR && rwInfo->maa_addr != 0 && rwInfo->size != 0)) {
        DMS_LOGE("rwInfo is invalid!");
        DMS_MNTN_IncComStatsInfo(DMS_MDM_RD_BUFF_ERR);
        return;
    }

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(portId);
    if (portTblInfo->RcvPrivData == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "RcvPrivData is not registered!");
        return;
    }

    /* MODEM端口，底软提供的内存为MAA内存，需要封装一个TTF头 */
    immZc = IMM_ZcStaticAllocWithMaa((VOS_UINT16)rwInfo->size, rwInfo->maa_addr);
    if (immZc == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "IMM_ZcStaticAllocWithMaa fail!");
        return;
    }

    /* 调用DMS port模块的回调函数 */
    (VOS_VOID)portTblInfo->RcvPrivData(portId, immZc);
}

STATIC VOS_VOID DMS_MDM_RcvWriteDone(void *priv, struct vcom_rw_info *rwInfo)
{
    DMS_PortIdUint16 portId;

    DMS_LOGI("enter.");
    DMS_MNTN_IncComStatsInfo(DMS_MDM_FREE_CB);

    if (priv == VOS_NULL_PTR || rwInfo == VOS_NULL_PTR) {
        DMS_LOGE("input para invalid, [priv=%d], [rwInfo=%d]!", priv, rwInfo);
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_MODEM) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    /* modem端口走SPE通道，下行异步写内存使用的是MAA内存 */
    if (rwInfo->userdata != VOS_NULL_PTR) { /* 如果userData非空，即需要释放TTF头，否则只需要释放MAA内存 */
        DMS_MNTN_IncComStatsInfo(DMS_MDM_FREE_CB_PPP);
        IMM_Zc* mem = (IMM_Zc *)rwInfo->userdata;
        IMM_ZcFree(mem);
        rwInfo->userdata = VOS_NULL_PTR;
    } else {
        if (rwInfo->maa_addr != 0) {
            DMS_MNTN_IncComStatsInfo(DMS_MDM_FREE_CB_AT);
            DMS_MAA_MEM_FREE(PS_PID_DMS, rwInfo->maa_addr);
            rwInfo->maa_addr = 0;
        }
    }
}

STATIC VOS_VOID DMS_MDM_RcvMscCmd(void *priv, struct vcom_msc_info *vcomMsc)
{
    const struct DMS_PortFuncTbl *portTblInfo = VOS_NULL_PTR;
    struct DMS_MscInfo            dmsMsc;
    DMS_PortIdUint16              portId;

    DMS_LOGI("enter.");

    if (priv == VOS_NULL_PTR || vcomMsc == VOS_NULL_PTR) {
        DMS_LOGE("input para invalid, [priv=%d], [vcomMsc=%d]!", priv, vcomMsc);
        DMS_MNTN_IncComStatsInfo(DMS_MDM_RD_MSC_NULL);
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_MODEM) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    if (DMS_COMM_CBCheck(portId) != VOS_OK) {
        return;
    }

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(portId);
    if (portTblInfo->RcvMscInd == VOS_NULL_PTR) {
        DMS_LOGE("RcvMscInd is not registered.");
        return;
    }

    (VOS_VOID)memset_s(&dmsMsc, sizeof(dmsMsc), 0x00, sizeof(dmsMsc));
    dmsMsc.dtrFlag = vcomMsc->op_dtr;
    dmsMsc.dtr = vcomMsc->dtr;

    portTblInfo->RcvMscInd(portId, &dmsMsc);
}

VOS_UINT32 DMS_MDM_Open(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为ready，端口可以正常发送数据 */
    dev->devStat = DMS_DEV_READY;

    /* 注册下行异步写回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_WRITE_CB, DMS_MDM_RcvWriteDone) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_MDM_REG_FREE_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_WRITE_CB failed!");
        return VOS_ERR;
    }

    /* 注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, DMS_MDM_RcvData) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_MDM_REG_RD_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        return VOS_ERR;
    }

    /* 注册管脚信号读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_MSC_READ_CB, DMS_MDM_RcvMscCmd) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_MDM_REG_RD_MSC_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_MSC_READ_CB failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_MDM_Close(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为not ready，端口不能发送数据 */
    dev->devStat = DMS_DEV_NOT_READY;

    /* 去注册下行异步写回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_WRITE_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_MDM_REG_FREE_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_WRITE_CB failed!");
        return VOS_ERR;
    }

    /* 去注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_MDM_REG_RD_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        return VOS_ERR;
    }

    /* 去注册管脚信号读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_MSC_READ_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_MDM_REG_RD_MSC_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_MSC_READ_CB failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_MDM_SendDataAsync(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len)
{
    struct vcom_rw_info rwInfo;

    DMS_PORT_LOGI(dev->portId, "enter.");
    DMS_MNTN_IncComStatsInfo(DMS_MDM_WRT_ASYNC);

    /* 设备ready才能发送数据 */
    if (dev->devStat == DMS_DEV_NOT_READY) {
        DMS_PORT_LOGE(dev->portId, "DEV not ready.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(&rwInfo, sizeof(rwInfo), 0x00, sizeof(rwInfo));

    if (DMS_MDM_SysTransMaa(dev->portId, &rwInfo, data, len) == VOS_NULL_PTR) {
        DMS_PORT_LOGE(dev->portId, "DMS_MDM_SysTransMaa fail!");
        DMS_MNTN_IncComStatsInfo(DMS_MDM_WRT_BUFF_ERR);
        return VOS_ERR;
    }

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_WRITE_ASYNC, (VOS_VOID *)&rwInfo) != VOS_OK) {
        DMS_PORT_LOGE(dev->portId, "write data async error.");
        DMS_MNTN_IncComStatsInfo(DMS_MDM_WRT_MSC_ERR);
        DMS_MDM_RcvWriteDone((VOS_VOID *)&dev->portId, &rwInfo);
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_MDM_SendPrivData(struct DMS_DevInfo *dev, IMM_Zc *data)
{
    struct vcom_rw_info  rwInfo;

    DMS_PORT_LOGI(dev->portId, "enter.");
    DMS_MNTN_IncComStatsInfo(DMS_MDM_WRT_ASYNC);

    /* 设备ready才能发送数据 */
    if (dev->devStat == DMS_DEV_NOT_READY) {
        DMS_PORT_LOGE(dev->portId, "DEV not ready.");
        IMM_ZcFree(data);
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(&rwInfo, sizeof(rwInfo), 0x00, sizeof(rwInfo));

    /* MODEM端口走SPE通道，直接提取内存中的MAA数据并提供给底软 */
    rwInfo.userdata = (VOS_UINT8 *)data;
    rwInfo.maa_addr = (VOS_UINT64)(VOS_UINT_PTR)IMM_ZcGetDataPtr(data);
    rwInfo.size = IMM_ZcGetUsedLen(data);

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_WRITE_ASYNC, (VOS_VOID *)&rwInfo) != VOS_OK) {
        DMS_PORT_LOGE(dev->portId, "write data async error.");
        DMS_MNTN_IncComStatsInfo(DMS_MDM_WRT_MSC_ERR);
        DMS_MDM_RcvWriteDone((VOS_VOID *)&dev->portId, &rwInfo);
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_MDM_SendMscCmd(struct DMS_DevInfo *dev, struct DMS_MscInfo *dmsMsc)
{
    struct vcom_msc_info vcomMsc;

    DMS_PORT_LOGI(dev->portId, "enter.");
    DMS_MNTN_IncComStatsInfo(DMS_MDM_WRT_MSC);

    /* 设备ready才能发送数据 */
    if (dev->devStat == DMS_DEV_NOT_READY) {
        DMS_PORT_LOGE(dev->portId, "DEV not ready.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(&vcomMsc, sizeof(vcomMsc), 0x00, sizeof(vcomMsc));
    vcomMsc.op_dsr = dmsMsc->dsrFlag;
    vcomMsc.op_cts = dmsMsc->ctsFlag;
    vcomMsc.op_dcd = dmsMsc->dcdFlag;
    vcomMsc.op_ri  = dmsMsc->riFlag;
    vcomMsc.dsr = dmsMsc->dsr;
    vcomMsc.cts = dmsMsc->cts;
    vcomMsc.dcd = dmsMsc->dcd;
    vcomMsc.ri  = dmsMsc->ri;

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_MSC_WRITE_CMD, (VOS_VOID *)&vcomMsc) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_MDM_WRT_MSC_ERR);
        DMS_PORT_LOGE(dev->portId, "set msc error.");
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
