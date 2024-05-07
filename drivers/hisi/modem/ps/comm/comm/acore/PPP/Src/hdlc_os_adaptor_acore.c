
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

#include "hdlc_os_adaptor_interface.h"
#include "ppp_public.h"
#include "vos.h"
#include "linux_interface.h"
#include <linux/io.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <linux/device.h>

/* 一个页表最小长度 */
#define PPP_HDLC_MEM_MAP_SIZE 4096

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0))
struct device g_hdlcDev = {0};
#ifdef CONFIG_ARM64
VOS_UINT64 g_hdlcDmaMask = 0xffffffffffffffffULL;
#else
VOS_UINT64 g_hdlcDmaMask = 0xffffffffULL;
#endif
#endif

struct device* g_hdlcDevRef;

__STATIC_ VOS_VOID PPP_HDLC_HardWrite32Reg(VOS_UINT_PTR addr, VOS_UINT32 value)
{
    writel(value, (volatile unsigned int*)addr);
}

__STATIC_ VOS_UINT32 PPP_HDLC_HardRead32Reg(VOS_UINT_PTR addr)
{
    VOS_UINT32 value = readl((volatile unsigned int*)addr);
    return value;
}

/* IO地址映射函数 */
__STATIC_ VOS_UINT_PTR PPP_IoAddressMap(VOS_UINT_PTR addr)
{
    VOS_UINT_PTR ioAddr;
    ioAddr = (VOS_UINT_PTR)ioremap(addr, PPP_HDLC_MEM_MAP_SIZE);
    return ioAddr;
}

VOS_VOID PPP_HDLC_CacheInvalid(VOS_VOID* data, VOS_UINT32 len)
{
    dma_map_single(g_hdlcDevRef, data, len, DMA_FROM_DEVICE);
}

VOS_VOID PPP_HDLC_CacheFlush(VOS_VOID* data, VOS_UINT32 len)
{
    dma_map_single(g_hdlcDevRef, data, len, DMA_TO_DEVICE);
}

VOS_UINT32 PPP_HDLC_GetAlignBufLen(VOS_UINT32 addr)
{
    return PAGE_ALIGN(addr + PAGE_SIZE);
}

VOS_VOID PPP_HDLC_FillIoConfig(PPP_HDLC_HardConfigInfo* cfg)
{
    cfg->write32RegFunc  = PPP_HDLC_HardWrite32Reg;
    cfg->read32RegFunc   = PPP_HDLC_HardRead32Reg;

    cfg->memPhy2VirtFunc = (PPP_HDLC_HardMemCvtFunc)phys_to_virt;
    cfg->memVirt2PhyFunc = (PPP_HDLC_HardMemCvtFunc)virt_to_phys;
    cfg->bufPhy2VirtFunc = VOS_UncacheMemPhyToVirt;
    cfg->bufVirt2PhyFunc = VOS_UncacheMemVirtToPhy;
    cfg->ioAddrCvtFunc   = PPP_IoAddressMap;
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0))
/* 初始化HDLC平台设备 */
VOS_INT32 PPP_HDLC_PlatDevProbe(struct platform_device *dev)
{
    g_hdlcDevRef = &(dev->dev);
    dma_set_mask_and_coherent(g_hdlcDevRef, DMA_BIT_MASK(64));  //lint !e598 !e648

    return 0;
}

/* HDLC平台设备删除 */
VOS_INT32 PPP_HDLC_PlatDevRemove(struct platform_device *dev)
{
    return 0;
}

static const struct of_device_id g_hdlcPlatDevOfMatch[] = {
    {
        .compatible = "hisilicon,hisi-hdlc",
    },
    {},
};

static struct platform_driver g_hdlcPlatDevDriver = {
    .probe  = PPP_HDLC_PlatDevProbe,
    .remove = PPP_HDLC_PlatDevRemove,
    .driver = {
        .name = "hisi-hdlc",
        .of_match_table = of_match_ptr(g_hdlcPlatDevOfMatch),
    },
};

#endif

VOS_VOID PPP_HDLC_HardwareDeviceReg(VOS_VOID)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0))
    g_hdlcDev.dma_mask = &g_hdlcDmaMask;
    g_hdlcDevRef   = &g_hdlcDev;
#else
    platform_driver_register(&g_hdlcPlatDevDriver);
#endif
}

/* acore linux系统不需要主动注册映射关系 */
VOS_VOID PPP_HDLC_RegVirPhyAddrMap(VOS_UINT_PTR ioBaseAddr)
{
}

