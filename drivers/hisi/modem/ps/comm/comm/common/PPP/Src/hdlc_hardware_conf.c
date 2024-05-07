/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#include "hdlc_hardware_conf.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_HDLC_HARDWARE_CONF_C

#if (FEATURE_PPP_HDLC_HARDWARE == FEATURE_ON)

/* HDLC配置信息 */
PPP_HDLC_HardConfigInfo g_pppHdlcConfig = {0};

/* 将PPP中封装解封装的物理地址转换为虚地址 */
VOS_UINT8* PPP_HDLC_HardBufPhy2Virt(VOS_UINT_PTR pAddr)
{
    VOS_UINT8 *addr = VOS_NULL_PTR;

    if (g_pppHdlcConfig.bufPhy2VirtFunc != VOS_NULL_PTR) {
        addr = (VOS_UINT8*)g_pppHdlcConfig.bufPhy2VirtFunc((VOS_UINT8*)pAddr, (VOS_UINT8*)g_pppHdlcConfig.pppPhyAddr,
            (VOS_UINT8*)g_pppHdlcConfig.pppVirtAddr, g_pppHdlcConfig.pppTotalBufLen);
    } else {
        addr = (VOS_UINT8*)pAddr;
    }

    return addr;
}

/* 将PPP中封装解封装的虚地址转换为物理地址 */
VOS_UINT8* PPP_HDLC_HardBufVirt2Phy(VOS_UINT_PTR vAddr)
{
    VOS_UINT8 *addr = VOS_NULL_PTR;

    if (g_pppHdlcConfig.bufVirt2PhyFunc != VOS_NULL_PTR) {
        addr = (VOS_UINT8*)g_pppHdlcConfig.bufVirt2PhyFunc((VOS_UINT8*)vAddr, (VOS_UINT8*)g_pppHdlcConfig.pppPhyAddr,
            (VOS_UINT8*)g_pppHdlcConfig.pppVirtAddr, g_pppHdlcConfig.pppTotalBufLen);
    } else {
        addr = (VOS_UINT8*)vAddr;
    }

    return addr;
}

/* 将PPP中封装解封装的物理地址转换为虚地址 */
VOS_UINT8* PPP_HDLC_HardMemPhy2Virt(VOS_UINT_PTR pAddr)
{
    VOS_UINT8 *addr = VOS_NULL_PTR;

    if (g_pppHdlcConfig.bufPhy2VirtFunc != VOS_NULL_PTR) {
        addr = (VOS_UINT8*)g_pppHdlcConfig.memPhy2VirtFunc(pAddr);
    } else {
        addr = (VOS_UINT8*)pAddr;
    }

    return addr;
}

/* 将PPP中封装解封装的虚地址转换为物理地址 */
VOS_UINT8* PPP_HDLC_HardMemVirt2Phy(VOS_UINT_PTR vAddr)
{
    VOS_UINT8 *addr = VOS_NULL_PTR;

    if (g_pppHdlcConfig.bufVirt2PhyFunc != VOS_NULL_PTR) {
        addr = (VOS_UINT8*)g_pppHdlcConfig.memVirt2PhyFunc(vAddr);
    } else {
        addr = (VOS_UINT8*)vAddr;
    }

    return addr;
}

/* IO地址转换函数 */
VOS_UINT_PTR PPP_HDLC_HardIOAddrCvt(VOS_UINT_PTR addr)
{
    VOS_UINT_PTR newAddr;

    if (g_pppHdlcConfig.ioAddrCvtFunc != VOS_NULL_PTR) {
        newAddr = (VOS_UINT_PTR)g_pppHdlcConfig.ioAddrCvtFunc(addr);
    } else {
        newAddr = (VOS_UINT_PTR)addr;
    }

    return newAddr;
}

/* 配置HDLC相关信息 */
VOS_UINT32 PPP_HdlcHardConfig(PPP_HDLC_HardConfigInfo *hdlcConfig)
{
    if (hdlcConfig == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* 如果HDLC使用内存长度为0，后续操作将无法进行 */
    if (hdlcConfig->pppTotalBufLen == 0) {
        return VOS_ERR;
    }

    /* 如果读写寄存器函数为空，HDLC硬件将无法正常工作 */
    if ((hdlcConfig->read32RegFunc == VOS_NULL_PTR) || (hdlcConfig->write32RegFunc == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    (VOS_VOID)memcpy_s(&g_pppHdlcConfig, sizeof(PPP_HDLC_HardConfigInfo), hdlcConfig, sizeof(PPP_HDLC_HardConfigInfo));

    return VOS_OK;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
