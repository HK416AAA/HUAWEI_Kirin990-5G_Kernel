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
#include "imm_interface.h"
#include "dms_dev_i.h"
#include "dms_debug.h"

#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_DMS_DEV_HSUART_C

#if (FEATURE_AT_HSUART == FEATURE_ON)

STATIC IMM_Zc *DMS_HSUART_SysTransMaa(DMS_PortIdUint16 portId, struct vcom_rw_info *rwInfo)
{
    struct DMS_DevInfo  *devInfo = VOS_NULL_PTR;
    IMM_Zc              *immZc = VOS_NULL_PTR;
    VOS_UINT64           maaAddr = 0;
    errno_t              memResult;
    VOS_UINT32           size = rwInfo->size;

    devInfo = DMS_DEV_GetDevEntityByPortId(portId);

    /* HSUART端口，底软提供的内存为系统内存，需要申请MAA内存，并封装一个TTF头，并释放系统内存 */
    maaAddr = DMS_MAA_MEM_ALLOC(PS_PID_DMS, size);
    if (maaAddr == 0) {
        DMS_PORT_LOGE(portId, "DMS_MAA_MEM_ALLOC fail!");
        return VOS_NULL_PTR;
    }

    memResult = memcpy_s((VOS_VOID *)(VOS_UINT_PTR)(maaAddr & 0xFFFFFFFF), size, rwInfo->buf, size);
    TAF_MEM_CHK_RTN_VAL(memResult, size, size);

    immZc = IMM_ZcStaticAllocWithMaa((VOS_UINT16)size, maaAddr);
    if (immZc == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "IMM_ZcStaticAllocWithMaa fail!");
        DMS_MAA_MEM_FREE(PS_PID_DMS, maaAddr);
        return VOS_NULL_PTR;
    }

    /* 释放系统内存 */
    if (mdrv_vcom_ioctl(devInfo->handle, VCOM_IOCTL_CLEAR_READ_BUFF, (VOS_VOID *)rwInfo) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_RETURN_BUFF_ERR);
        DMS_PORT_LOGE(portId, "VCOM_IOCTL_CLEAR_READ_BUFF failed!");
    }

    return immZc;
}

STATIC struct vcom_rw_info *DMS_HSUART_SysTransSys(DMS_PortIdUint16 portId, struct vcom_rw_info *rwInfo,
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

STATIC struct vcom_rw_info *DMS_HSUART_MaaTransSys(DMS_PortIdUint16 portId,
    struct vcom_rw_info *rwInfo, IMM_Zc *immZc)
{
    VOS_UINT8  *senBuf = VOS_NULL_PTR;
    VOS_UINT8  *zcData = VOS_NULL_PTR;
    errno_t     memResult;
    VOS_UINT32  len;

    zcData = IMM_ZcGetDataPtr(immZc);
    len = IMM_ZcGetUsedLen(immZc);

    /* HSUART端口走EICC通道，先申请系统内存并拷贝MAA数据，然后释放TTF内存并将系统内存提供给底软 */
    senBuf = (VOS_UINT8  *)VOS_MemAlloc(PS_PID_DMS, DYNAMIC_MEM_PT, len);
    if (senBuf == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "VOS_MemAlloc fail.");
        IMM_ZcFree(immZc);
        return VOS_NULL_PTR;
    }

    memResult = memcpy_s(senBuf, len, zcData, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    rwInfo->buf = senBuf;
    rwInfo->size = len;

    IMM_ZcFree(immZc);

    return rwInfo;
}

STATIC VOS_VOID DMS_HSUART_RcvData(VOS_VOID *priv, struct vcom_rw_info *rwInfo)
{
    const struct DMS_PortFuncTbl *portTblInfo = VOS_NULL_PTR;
    IMM_Zc                       *immZc       = VOS_NULL_PTR;
    DMS_PortIdUint16              portId;

    DMS_LOGI("enter.");
    DMS_MNTN_IncComStatsInfo(DMS_HSUART_RD_CB);

    if (priv == VOS_NULL_PTR) {
        DMS_LOGE("priv is NULL!");
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_HSUART) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    if (DMS_COMM_CBCheck(portId) != VOS_OK) {
        return;
    }

    if (!(rwInfo != VOS_NULL_PTR && rwInfo->buf != VOS_NULL_PTR && rwInfo->size != 0)) {
        DMS_LOGE("rwInfo is invalid!");
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_RD_BUFF_ERR);
        return;
    }

    if (rwInfo->size > DMS_HSUART_MAX_RCV_LEN) {
        DMS_LOGE("rwInfo size is too large, %d, !", rwInfo->size);
        return;
    }

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(portId);
    if (portTblInfo->RcvPrivData == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "RcvData is not registered!");
        return;
    }

    /* 调用DMS port模块的回调函数 */
    immZc = DMS_HSUART_SysTransMaa(portId, rwInfo);
    if (immZc == VOS_NULL_PTR) {
        DMS_PORT_LOGE(portId, "DMS_HSUART_SysTransMaa fail!");
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_RD_ADDR_INVALID);
        return;
    }

    (VOS_VOID)portTblInfo->RcvPrivData(portId, immZc);
}

STATIC VOS_VOID DMS_HSUART_RcvWriteDone(void *priv, struct vcom_rw_info *rwInfo)
{
    DMS_PortIdUint16 portId;

    DMS_LOGI("enter.");

    DMS_MNTN_IncComStatsInfo(DMS_HSUART_FREE_CB);

    if (priv == VOS_NULL_PTR || rwInfo == VOS_NULL_PTR) {
        DMS_LOGE("input para invalid, [priv=%d], [rwInfo=%d]!", priv, rwInfo);
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_HSUART) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    /* hsuart异步写端口走EICC通道，使用的是系统内存 */
    if (rwInfo->buf != VOS_NULL_PTR) {
        VOS_MemFree(PS_PID_DMS, rwInfo->buf);
        rwInfo->buf = VOS_NULL_PTR;
    }
}

STATIC VOS_VOID DMS_HSUART_RcvMscCmd(void *priv, struct vcom_msc_info *vcomMsc)
{
    const struct DMS_PortFuncTbl *portTblInfo = VOS_NULL_PTR;
    struct DMS_MscInfo            dmsMsc;
    DMS_PortIdUint16              portId;

    DMS_LOGI("enter.");
    DMS_MNTN_IncComStatsInfo(DMS_HSUART_RD_MSC);

    if (priv == VOS_NULL_PTR || vcomMsc == VOS_NULL_PTR) {
        DMS_LOGE("input para invalid, [priv=%d], [vcomMsc=%d]!", priv, vcomMsc);
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_RD_MSC_NULL);
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_HSUART) {
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

STATIC VOS_VOID DMS_HSUART_RcvEscapeSequence(void *priv)
{
    const struct DMS_PortFuncTbl *portTblInfo = VOS_NULL_PTR;
    DMS_PortIdUint16              portId;

    DMS_LOGI("enter.");
    DMS_MNTN_IncComStatsInfo(DMS_HSUART_SWITCH_MODE);

    if (priv == VOS_NULL_PTR ) {
        DMS_LOGE("input para invalid, priv is NULL!");
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_HSUART) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    if (DMS_COMM_CBCheck(DMS_PORT_HSUART) != VOS_OK) {
        return;
    }

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(DMS_PORT_HSUART);
    if (portTblInfo->RcvEscapeSequence == VOS_NULL_PTR) {
        DMS_LOGE("RcvEscapeSequence is not registered.");
        return;
    }

    portTblInfo->RcvEscapeSequence(DMS_PORT_HSUART);
}

STATIC VOS_VOID DMS_HSUART_RcvWaterMarkDetectInd(void *priv, VOS_INT32 isHigh)
{
    const struct DMS_PortFuncTbl *portTblInfo = VOS_NULL_PTR;
    DMS_WaterLevelUint32          waterLevel = DMS_LEVEL_LOW;
    DMS_PortIdUint16              portId;

    DMS_LOGI("enter.");
    DMS_MNTN_IncComStatsInfo(DMS_HSUART_WATER_CB);

    if (priv == VOS_NULL_PTR ) {
        DMS_LOGE("input para invalid, priv is NULL!");
        return;
    }

    if (isHigh != 0 && isHigh != 1) {
        DMS_LOGE("level abnormal: %d.", isHigh);
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_WATER_LEVEL_ERR);
        return;
    }

    portId = *(DMS_PortIdUint16 *)priv;
    if (portId != DMS_PORT_HSUART) {
        DMS_LOGE("portId is uncorrect, %d!", portId);
        return;
    }

    if (DMS_COMM_CBCheck(DMS_PORT_HSUART) != VOS_OK) {
        return;
    }

    portTblInfo = DMS_DEV_GetDevPortTblByPortId(DMS_PORT_HSUART);
    if (portTblInfo->RcvWaterMarkInd == VOS_NULL_PTR) {
        DMS_LOGE("RcvWaterMarkInd is not registered.");
        return;
    }

    if (isHigh == 1) {
        waterLevel = DMS_LEVEL_HIGH;
    }

    portTblInfo->RcvWaterMarkInd(DMS_PORT_HSUART, waterLevel);
}

VOS_UINT32 DMS_HSUART_Open(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为ready，端口可以正常发送数据 */
    dev->devStat = DMS_DEV_READY;

    /* 注册下行异步写回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_WRITE_CB, DMS_HSUART_RcvWriteDone) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_FREE_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_WRITE_CB failed!");
        return VOS_ERR;
    }

    /* 注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, DMS_HSUART_RcvData) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_RD_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        return VOS_ERR;
    }

    /* 注册管脚信号读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_MSC_READ_CB, DMS_HSUART_RcvMscCmd) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_RD_MSC_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_MSC_READ_CB failed!");
        return VOS_ERR;
    }

    /* 注册"+++"命令检测回调 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_ESCAP_SEQ_DETECT, DMS_HSUART_RcvEscapeSequence) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_SWITCH_MODE_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_ESCAP_SEQ_DETECT failed.\r");
        return VOS_ERR;
    }

    /* 注册HSUART端口水线监测回调 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_WATER_MARK_DETECT, DMS_HSUART_RcvWaterMarkDetectInd) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_WATER_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_WATER_MARK_DETECT failed.\r");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_Close(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 端口状态更新为not ready，端口不能发送数据 */
    dev->devStat = DMS_DEV_NOT_READY;

    /* 去注册下行异步写回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_WRITE_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_FREE_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_WRITE_CB failed!");
        return VOS_ERR;
    }

    /* 去注册上行读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_READ_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_RD_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_READ_CB failed!");
        return VOS_ERR;
    }

    /* 去注册管脚信号读回调接口 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_MSC_READ_CB, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_RD_MSC_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_MSC_READ_CB failed!");
        return VOS_ERR;
    }

    /* 去注册"+++"命令检测回调 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_ESCAP_SEQ_DETECT, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_SWITCH_MODE_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_ESCAP_SEQ_DETECT failed.\r");
        return VOS_ERR;
    }

    /* 去注册HSUART端口水线监测回调 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_WATER_MARK_DETECT, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_REG_WATER_CB_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_WATER_MARK_DETECT failed.\r");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_SendDataAsync(struct DMS_DevInfo *dev, VOS_UINT8 *data, VOS_UINT16 len)
{
    struct vcom_rw_info rwInfo;

    DMS_PORT_LOGI(dev->portId, "enter.");
    DMS_MNTN_IncComStatsInfo(DMS_HSUART_WRT_ASYNC);

    /* 设备ready才能发送数据 */
    if (dev->devStat == DMS_DEV_NOT_READY) {
        DMS_PORT_LOGE(dev->portId, "DEV not ready.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(&rwInfo, sizeof(rwInfo), 0x00, sizeof(rwInfo));

    if (DMS_HSUART_SysTransSys(dev->portId, &rwInfo, data, len) == VOS_NULL_PTR) {
        DMS_PORT_LOGE(dev->portId, "DMS_HSUART_SysTransSys fail!");
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_ALLOC_BUF_ERR);
        return VOS_ERR;
    }

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_WRITE_ASYNC, (VOS_VOID *)&rwInfo) != VOS_OK) {
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_WRITE_ASYNC error.");
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_WRT_ASYNC_ERR);
        DMS_HSUART_RcvWriteDone((VOS_VOID *)&dev->portId, &rwInfo);
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_SendPrivData(struct DMS_DevInfo *dev, IMM_Zc *data)
{
    struct vcom_rw_info rwInfo;

    DMS_PORT_LOGI(dev->portId, "enter.");
    DMS_MNTN_IncComStatsInfo(DMS_HSUART_WRT_ASYNC);

    /* 设备ready才能发送数据 */
    if (dev->devStat == DMS_DEV_NOT_READY) {
        DMS_PORT_LOGE(dev->portId, "DEV not ready.");
        IMM_ZcFree(data);
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(&rwInfo, sizeof(rwInfo), 0x00, sizeof(rwInfo));

    if (DMS_HSUART_MaaTransSys(dev->portId, &rwInfo, data) == VOS_NULL_PTR) {
        DMS_PORT_LOGE(dev->portId, "DMS_HSUART_MaaTransSys fail!");
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_ALLOC_BUF_ERR);
        return VOS_ERR;
    }

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_WRITE_ASYNC, (VOS_VOID *)&rwInfo) != VOS_OK) {
        DMS_PORT_LOGE(dev->portId, "write data async error.");
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_WRT_ASYNC_ERR);
        DMS_HSUART_RcvWriteDone((VOS_VOID *)&dev->portId, &rwInfo);
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_SendMscCmd(struct DMS_DevInfo *dev, struct DMS_MscInfo *dmsMsc)
{
    struct vcom_msc_info vcomMsc;

    DMS_PORT_LOGI(dev->portId, "enter.");

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
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_WRT_MSC_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_MSC_WRITE_CMD error.");
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_CfgFlowCtrl(struct DMS_DevInfo *dev, VOS_UINT32 rtsFlag, VOS_UINT32 ctsFlag)
{
    struct vcom_flow_ctrl flowCtrlValue;

    DMS_PORT_LOGI(dev->portId, "enter.");

    (VOS_VOID)memset_s(&flowCtrlValue, sizeof(flowCtrlValue), 0x00, sizeof(flowCtrlValue));

    flowCtrlValue.rtsen = (VOS_UINT16)rtsFlag;
    flowCtrlValue.ctsen = (VOS_UINT16)ctsFlag;

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_FLOW_CONTROL, (VOS_VOID *)&flowCtrlValue) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_FLOW_CONTRL_ERR);
        DMS_PORT_LOGE(dev->portId, "VCOM_IOCTL_SET_FLOW_CONTROL error.");
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_SetCharFrame(struct DMS_DevInfo *dev, VOS_UINT32 udiDataLen, VOS_UINT32 udiStpLen,
    VOS_UINT32 udiParity)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 调用DRV函数设置串口数据位长度 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_WLEN, (VOS_VOID *)&udiDataLen) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_SET_WLEN_ERR);
        DMS_PORT_LOGE(dev->portId, "set wlen error.");
        return VOS_ERR;
    }

    /* 调用DRV函数设置串口停止位长度 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_STP, (VOS_VOID *)&udiStpLen) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_SET_STP_ERR);
        DMS_PORT_LOGE(dev->portId, "set stp error.");
        return VOS_ERR;
    }

    /* 调用DRV函数设置串口的校验位 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_EPS, (VOS_VOID *)&udiParity) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_SET_EPS_ERR);
        DMS_PORT_LOGE(dev->portId, "set eps error.");
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_SetBaud(struct DMS_DevInfo *dev, VOS_UINT32 baudRate)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    /* 调用DRV函数设置串口的波特率 */
    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_SET_BAUD, (VOS_VOID *)&baudRate) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_SET_BAUD_ERR);
        DMS_PORT_LOGE(dev->portId, "set baud rate error.");
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

VOS_UINT32 DMS_HSUART_FlushTxData(struct DMS_DevInfo *dev)
{
    DMS_PORT_LOGI(dev->portId, "enter.");

    if (mdrv_vcom_ioctl(dev->handle, VCOM_IOCTL_CLEAR_WRITE_BUFF, VOS_NULL_PTR) != MDRV_OK) {
        DMS_MNTN_IncComStatsInfo(DMS_HSUART_FLUSH_BUFF_ERR);
        DMS_PORT_LOGE(dev->portId, "Release buffer failed.");
        return VOS_ERR;
    }

    DMS_PORT_LOGI(dev->portId, "success.");
    return VOS_OK;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
