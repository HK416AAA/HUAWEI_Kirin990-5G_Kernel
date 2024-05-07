/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#include "eicc_platform.h"
#include "eicc_dts.h"

#include "eicc_device.h"
#include "eicc_driver.h"
#include "eicc_core.h"
#include "eicc_pmsr.h"
#include "eicc_dump.h"
#include "eicc_reset.h"

#define EICC_MEM_ALIGN_DEFAULT (4)

static struct platform_device g_eicc_plat_device = {
    .name = "eicc_plat",
};

void *eicc_dma_alloc(u32 size, eiccsoc_ptr_t *pa, u32 align)
{
    void *mem_addr = NULL;
    u32 align_sz;

    if (align < EICC_MEM_ALIGN_DEFAULT) {
        align = EICC_MEM_ALIGN_DEFAULT;
    }
    align_sz = eicc_roundup(size, align);
    mem_addr = dma_alloc_coherent(&g_eicc_plat_device.dev, align_sz, pa, GFP_KERNEL);
    if (mem_addr == NULL) {
        eicc_print_error("eicc_dma_alloc failed\n");
        return NULL;
    }
    return mem_addr;
}

void eicc_dma_free(size_t size, void *vaddr, eiccsoc_ptr_t pa, u32 align)
{
    u32 align_sz;
    if (align < EICC_MEM_ALIGN_DEFAULT) {
        align = EICC_MEM_ALIGN_DEFAULT;
    }
    align_sz = eicc_roundup(size, align);
    dma_free_coherent(&g_eicc_plat_device.dev, align_sz, vaddr, pa);
}

void *eicc_pushmem_alloc(u32 size, eiccsoc_ptr_t *pa, u32 align)
{
    void *mem_addr = NULL;

    mem_addr = eicc_dma_alloc(size, pa, align);
    if (mem_addr == NULL) {
        eicc_print_error("pushmem dma_alloc_coherent failed\n");
        return NULL;
    }
    return mem_addr;
}

void eicc_pushmem_free(void *va)
{
    return;
}

void *eicc_ioremap(u32 pa, u32 size)
{
    return ioremap(pa, size);
}

/* ******************************************************************************
 * 低功耗相关处理
 * ***************************************************************************** */
int bsp_eicc_suspend(void)
{
    int ret;
    ret = eicc_chn_suspend();
    if (ret) {
        return ret;
    }
    ret = eicc_dev_suspend();
    if (ret) {
        eicc_chn_resume();
        return ret;
    }
    return ret;
}
int bsp_eicc_resume(void)
{
    eicc_dev_resume();
    eicc_chn_resume();
    return 0;
}

static int eicc_suspend_wrapper(struct device *dev)
{
    return bsp_eicc_suspend();
}
static int eicc_resume_wrapper(struct device *dev)
{
    return bsp_eicc_resume();
}

int bsp_eicc_chn_before_resetting(void)
{
    return eicc_chn_before_resetting();
}

int bsp_eicc_chn_after_resetting(void)
{
    return eicc_chn_after_resetting();
}

static int eicc_plat_probe(struct platform_device *pdev)
{
    int ret;
    of_dma_configure(&pdev->dev, NULL);
    dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));

    ret = eicc_devices_init();
    if (ret) {
        eicc_print_error("eicc_devices_init failed\n");
        return -1;
    }
    ret = eicc_irqs_init();
    if (ret) {
        eicc_print_error("eicc_irqs_init failed\n");
        return -1;
    }
    return 0;
}

static const struct dev_pm_ops g_eicc_pmops = {
    .suspend_noirq = eicc_suspend_wrapper,
    .resume_noirq = eicc_resume_wrapper,
    .prepare = NULL,
    .complete = NULL,
};
static struct platform_driver g_eicc_plat_driver = {
    .probe = eicc_plat_probe,
    .driver =
        {
            .name = "eicc_plat",
            .owner = THIS_MODULE,
            .pm = &g_eicc_pmops,
        },
};

/* EICC虽然有多个设备，但鉴于其关联特性，以及低功耗时间优化， 因此创建一个虚拟设备来统一处理所有eicc事务 */
int bsp_eicc_init(void)
{
    int ret;

    static int inited = 0;
    if (inited) {
        return 0;
    }
    if (!eicc_init_meet()) {
        eicc_print_always("eicc init skiped\n");
        return 0;
    }
    ret = platform_device_register(&g_eicc_plat_device);
    if (ret) {
        return ret;
    }
    ret = platform_driver_register(&g_eicc_plat_driver);
    if (ret) {
        platform_device_unregister(&g_eicc_plat_device);
        return ret;
    }
    inited = 1;
    eicc_print_always("eicc init ok\n");
    return ret;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
arch_initcall(bsp_eicc_init);
#endif

MODULE_DESCRIPTION("EICC Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei");
