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

/* 用于加速访问device的数组，每个成员是一个指针，指向对应idx的device结构 */
struct eicc_device *g_eicc_devices_fastptr[EICC_DEVICE_NUM_MAX] = {0};
struct eicc_channel *g_eicc_channel_fastptr[EICC_USRCHN_NUM_MAX] = {0};
static u32 g_channel_fastptr_usedidx = 0;

struct eicc_device *eicc_device_get_fast(u32 devid)
{
    if (devid >= EICC_DEVICE_NUM_MAX) {
        return NULL;
    }
    return g_eicc_devices_fastptr[devid];
}

struct eicc_channel *eicc_channel_get_fast(u32 usr_chnid)
{
    u32 idx;
    for (idx = 0; idx < g_channel_fastptr_usedidx; idx++) {
        if (usr_chnid == g_eicc_channel_fastptr[idx]->user_id) {
            return g_eicc_channel_fastptr[idx];
        }
    }
    return NULL;
}

struct eicc_channel *eicc_channel_get_byidx(u32 idx)
{
    if (idx < EICC_USRCHN_NUM_MAX) {
        return g_eicc_channel_fastptr[idx];
    }
    return NULL;
}

int eicc_dev_chn_get(u32 phy_chn_id, struct eicc_device **ppdev, struct eicc_channel **ppchannel)
{
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    pdev = eicc_device_get_fast(GET_DEV_ID(phy_chn_id));
    if (pdev == NULL) {
        return -1;
    }
    if (GET_PIPE_ID(phy_chn_id) >= EICC_CHN_ID_MAX) {
        return -1;
    }
    pchannel = pdev->channels[GET_PIPE_ID(phy_chn_id)];
    if (pchannel == NULL) {
        return -1;
    }
    *ppdev = pdev;
    *ppchannel = pchannel;
    return 0;
}

int eicc_dev_chn_get_byid(u32 user_id, struct eicc_device **ppdev, struct eicc_channel **ppchannel)
{
    struct eicc_channel *pchannel_fake = NULL;
    if (ppdev == NULL || ppchannel == NULL) {
        return -1;
    }
    pchannel_fake = eicc_channel_get_fast(user_id);
    if (pchannel_fake == NULL) {
        return -1;
    }
    if (eicc_dev_chn_get(EICC_GET_PHYCHN_ID(pchannel_fake->ldrvchn_id), ppdev, ppchannel)) {
        return -1;
    }
    return 0;
}

int eicc_dev_chn_get_byidx(u32 chn_idx, struct eicc_device **ppdev, struct eicc_channel **ppchannel)
{
    struct eicc_channel *pchannel_fake = NULL;
    if (ppdev == NULL || ppchannel == NULL) {
        return -1;
    }
    pchannel_fake = eicc_channel_get_byidx(chn_idx);
    if (pchannel_fake == NULL) {
        return -1;
    }
    if (eicc_dev_chn_get(EICC_GET_PHYCHN_ID(pchannel_fake->ldrvchn_id), ppdev, ppchannel)) {
        return -1;
    }
    return 0;
}

static void eicc_glb_ctrl_init(struct eicc_device *dev)
{
/* 如果kenel unsec，那么一定要有安全os，而且全局和安全配置一定在安全os下做好了 */
#if EICC_FEATURE_KERNEL_INSEC
    u32 reg;
    void *base_addr = dev->base_va;

    writel(GLB_SECCTRL_SECCTRL, (void *)((char *)base_addr + EICC_GLB_SECCTRL));

    reg = readl((void *)((char *)base_addr + EICC_GLB_CTRL));
    reg = reg | GLB_CTRL_SR_PUSH | GLB_CTRL_CLK_AUTO;
    writel(reg, (void *)((char *)base_addr + EICC_GLB_CTRL));
#endif
}

int eicc_glbcfg_probe(struct eicc_device *pdev)
{
    if (pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_HOST || pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_GUEST) {
        eicc_glb_ctrl_init(pdev);
    }
    return 0;
}

static struct eicc_channel *eicc_chn_make(struct eicc_chn_dts *pchn_dts)
{
    struct eicc_channel *channel = NULL;
    channel = (struct eicc_channel *)eicc_malloc(sizeof(struct eicc_channel));
    if (channel == NULL) {
        eicc_print_error("eicc_chn_make failed!\n");
        return NULL;
    }
    (void)memset_s(channel, sizeof(struct eicc_channel), 0, sizeof(struct eicc_channel));
    spin_lock_init(&channel->lock);
    channel->user_id = pchn_dts->id;
    channel->type = pchn_dts->type;
    channel->ldrvchn_id = pchn_dts->ldrvchn_id;
    channel->rdrvchn_id = pchn_dts->rdrvchn_id;
    channel->dym_flags = 0;
    channel->const_flags = pchn_dts->const_flags;
    channel->remote_awake = 0;
    channel->state = (u32)EICC_CHN_INIT;
    return channel;
}

static int eicc_dev_of_map(struct eicc_device *pdev, struct eicc_device_dts *pdev_dts)
{
    pdev->base_pa = pdev_dts->reg_addr;
    if (pdev->ctrl_level != EICC_DEV_CONTROL_LEVEL_IGNORE) {
        pdev->base_va = eicc_ioremap(pdev_dts->reg_addr, pdev_dts->reg_size);
        if (pdev->base_va == NULL) {
            eicc_print_error("remap failed when eicc device init!\n");
            return -1;
        }
    }

    if (pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_HOST || pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_GUEST) {
        pdev->push_ptr_va = (u32 *)eicc_pushmem_alloc(sizeof(u32) * EICC_CHN_ID_MAX, &pdev->push_ptr_pa, sizeof(u32));
        if (pdev->push_ptr_va == NULL) {
            eicc_print_error("push mem alloc failed when eicc device init!\n");
            return -1;
        }
        (void)memset_s(pdev->push_ptr_va, sizeof(u32) * EICC_CHN_ID_MAX, 0, sizeof(u32) * EICC_CHN_ID_MAX);
    }

    return 0;
}
static int eicc_chn_dts_parse(device_node_s *p_chn_node, struct eicc_chn_dts *pchn_dts)
{
    u32 uret = 0;
    u32 chn_cfg[0x2] = {0};
    uret |= (u32)bsp_dt_property_read_u32_array(p_chn_node, "chn_id", &pchn_dts->id, 1);
    uret |= (u32)bsp_dt_property_read_u32_array(p_chn_node, "chn_type", &pchn_dts->type, 1);
    uret |= (u32)bsp_dt_property_read_u32_array(p_chn_node, "chn_cfg", chn_cfg, sizeof(chn_cfg) / sizeof(chn_cfg[0]));
    if (uret) {
        eicc_print_error("eicc_chn_probe failed!\n");
        return -1;
    }
    pchn_dts->ldrvchn_id = chn_cfg[0x0];
    pchn_dts->rdrvchn_id = chn_cfg[0x1];
    if (bsp_dt_property_read_u32_array(p_chn_node, "const_flags", &pchn_dts->const_flags, 1)) {
        pchn_dts->const_flags = 0;
    }
    return 0;
}

int eicc_chns_probe(struct eicc_device *pdev)
{
    u32 ldrvchn_id;
    struct eicc_channel *channel = NULL;

    struct eicc_chn_dts chn_dts;
    struct eicc_chn_dts *pchn_dts = &chn_dts;

    device_node_s temp_node;
    device_node_s *pnode = NULL;
    device_node_s *p_chn_node = &temp_node; /* for reduced dts version */

    (void)memset_s(p_chn_node, sizeof(device_node_s), 0, sizeof(device_node_s));
    if (pdev == NULL || pdev->of_node == NULL) {
        return -1;
    }

    pnode = (device_node_s *)pdev->of_node;
    bsp_dt_for_each_child_of_node(pnode, p_chn_node)
    {
        if (bsp_dt_property_read_u32_array(p_chn_node, "chn_id", &pchn_dts->id, 1)) {
            /* not a channel,may be other type; */
            continue;
        }
        if (eicc_chn_dts_parse(p_chn_node, pchn_dts)) {
            return -1;
        }
        ldrvchn_id = pchn_dts->ldrvchn_id;
        if (EICC_GET_CORE_ID(ldrvchn_id) >= EICC_CPU_ID_MAX || pdev->maps[EICC_GET_CORE_ID(ldrvchn_id)] == NULL) {
            eicc_print_error("chn_id 0x%x detected not design to this!\n", pchn_dts->id);
            continue;
        }
        if (EICC_GET_DEV_ID(ldrvchn_id) != pdev->id || EICC_GET_PIPE_ID(ldrvchn_id) >= pdev->pipepair_cnt ||
            pdev->channels[EICC_GET_PIPE_ID(ldrvchn_id)] != NULL) {
            eicc_print_error("chn_id 0x%x detected err!\n", pchn_dts->id);
            return -1;
        }
        if (g_channel_fastptr_usedidx >= EICC_USRCHN_NUM_MAX) {
            eicc_print_error("eicc_chn_probe too much!\n");
            return -1;
        }

        channel = eicc_chn_make(pchn_dts);
        if (channel == NULL) {
            return -1;
        }
        pdev->channels[EICC_GET_PIPE_ID(channel->ldrvchn_id)] = channel;
        eicc_print_trace("chan init ok,chan_id=0x%x,drvcnn_id=0x%x\n", (unsigned)channel->user_id,
            (unsigned)channel->ldrvchn_id);
        g_eicc_channel_fastptr[g_channel_fastptr_usedidx] = channel;
        g_channel_fastptr_usedidx++;
    }

    return 0;
}
static int eicc_dev_irq_parse(device_node_s *p_irq_node, struct eicc_device_dts *pdev_dts)
{
    u32 cpu_id;
    struct irq_bundle *maps = NULL;
    if (bsp_dt_property_read_u32_array(p_irq_node, "cpu_id", &cpu_id, 1)) {
        /* not a irq bundle ,may be other type; */
        return 0;
    }

    if (cpu_id >= EICC_CPU_ID_MAX) {
        eicc_print_error("dts irq_bundle[%d] outrange error!\n", cpu_id);
        return -1;
    }
    if (pdev_dts->maps[cpu_id] != NULL) {
        eicc_print_error("dts irq_bundle[%d] dup error!\n", cpu_id);
        return -1;
    }
    maps = (struct irq_bundle *)eicc_malloc(sizeof(struct irq_bundle));
    if (maps == NULL) {
        eicc_print_error("eicc_devdts_parse malloc error!\n");
        return -1;
    }
    (void)memset_s(maps, sizeof(struct irq_bundle), 0, sizeof(struct irq_bundle));
    maps->cpu_id = cpu_id;

    if (pdev_dts->ctrl_level == EICC_DEV_CONTROL_LEVEL_HOST || pdev_dts->ctrl_level == EICC_DEV_CONTROL_LEVEL_GUEST) {
        maps->irq[0] = bsp_dt_irq_parse_and_map(p_irq_node, 0);
        maps->irq[1] = bsp_dt_irq_parse_and_map(p_irq_node, 1);
    } else if (pdev_dts->ctrl_level == EICC_DEV_CONTROL_LEVEL_IRQCLR) {
        maps->irq[0] = bsp_dt_irq_parse_and_map(p_irq_node, 0);
        maps->irq[1] = 0xFFFFFFFF;
    } else {
        maps->irq[0] = 0xFFFFFFFF;
        maps->irq[1] = 0xFFFFFFFF;
    }
    pdev_dts->maps[cpu_id] = maps;
    return 0;
}
static int eicc_dev_dts_parse(device_node_s *p_dev_node, struct eicc_device_dts *pdev_dts)
{
    u32 uret = 0;
    u32 array[0x2] = {0};

    device_node_s temp_node;
    device_node_s *p_irq_node = &temp_node; /* for reduced dts version */

    uret |= (u32)bsp_dt_property_read_u32_array(p_dev_node, "dev_id", &pdev_dts->dev_id, 1);
    uret |= (u32)bsp_dt_property_read_u32_array(p_dev_node, "reg", array, sizeof(array) / sizeof(array[0]));
    uret |= (u32)bsp_dt_property_read_u32_array(p_dev_node, "pipepair_cnt", &pdev_dts->pipepair_cnt, 1);
    uret |= (u32)bsp_dt_property_read_u32_array(p_dev_node, "ctrl_level", &pdev_dts->ctrl_level, 1);
    uret |= (u32)bsp_dt_property_read_u32_array(p_dev_node, "irq_type", &pdev_dts->irq_type, 1);
    if (uret) {
        eicc_print_error("dts node error!\n");
        return -1;
    }
    pdev_dts->reg_addr = array[0x0];
    pdev_dts->reg_size = array[0x1];

    /* IRQ MAPS PARSE */
    (void)memset_s(p_irq_node, sizeof(device_node_s), 0, sizeof(device_node_s));
    bsp_dt_for_each_child_of_node(p_dev_node, p_irq_node)
    {
        if (eicc_dev_irq_parse(p_irq_node, pdev_dts)) {
            return -1;
        }
    }

    return 0;
}

/* pdev->version_id and pdev->of_node be set by eicc_devices_init */
int eicc_dev_probe(struct eicc_device *pdev)
{
    u32 devid;
    struct eicc_device_dts dev_dts;
    struct eicc_device_dts *pdev_dts = &dev_dts;
    device_node_s *p_dev_node = NULL;
    if (pdev == NULL || pdev->of_node == NULL) {
        return -1;
    }
    p_dev_node = (device_node_s *)pdev->of_node;
    (void)memset_s(&dev_dts, sizeof(struct eicc_device_dts), 0, sizeof(struct eicc_device_dts));
    if (eicc_dev_dts_parse(p_dev_node, pdev_dts)) {
        return -1;
    }
    devid = pdev_dts->dev_id;
    if (devid >= EICC_DEVICE_NUM_MAX || g_eicc_devices_fastptr[devid]) {
        eicc_print_error("dts node devid error or dup!\n");
        return -1;
    }
    pdev->id = devid;
    pdev->ctrl_level = pdev_dts->ctrl_level;
    pdev->pipepair_cnt = pdev_dts->pipepair_cnt;

    pdev->irq_type = pdev_dts->irq_type;
    if (memcpy_s(pdev->maps, sizeof(pdev->maps), pdev_dts->maps, sizeof(pdev_dts->maps))) {
        eicc_print_error("dts node maps size not match!\n");
        return -1;
    }

    spin_lock_init(&pdev->lock);

    if (eicc_dev_of_map(pdev, pdev_dts)) {
        return -1;
    }

    g_eicc_devices_fastptr[devid] = pdev;
    return 0;
}

int eicc_probe(struct eicc_device *pdev)
{
    if (eicc_dev_probe(pdev)) {
        return -1;
    };

    if (eicc_chns_probe(pdev)) {
        return -1;
    };

    if (eicc_glbcfg_probe(pdev)) {
        return -1;
    };
    pdev->state = EICC_DEV_STATE_WORKING;
    return 0;
}

int eicc_devices_init(void)
{
    int ret;
    u32 version_id;
    struct eicc_device *device = NULL;

    device_node_s temp_node;
    device_node_s *parent_node = NULL;
    device_node_s *child_node = &temp_node; /* for reduced dts version */

    parent_node = bsp_dt_find_compatible_node(NULL, NULL, "hisilicon,eicc_balong_device");
    if (parent_node == NULL) {
        eicc_print_error("eicc dts not found!\n");
        return 0;
    }
    eicc_print_always("eicc dts found!\n");

    (void)memset_s(child_node, sizeof(device_node_s), 0, sizeof(device_node_s));
    bsp_dt_for_each_child_of_node(parent_node, child_node)
    {
        ret = bsp_dt_property_read_u32_array(child_node, "version_id", &version_id, 1);
        if (ret) {
            eicc_print_error("dts node error!\n");
            return -1;
        }
        device = (struct eicc_device *)eicc_malloc(sizeof(struct eicc_device));
        if (device == NULL) {
            eicc_print_error("malloc failed when eicc device init!\n");
            return -1;
        }
        (void)memset_s(device, sizeof(struct eicc_device), 0, sizeof(struct eicc_device));
        device->version_id = version_id;
        device->of_node = child_node;
        ret = eicc_probe(device);
        if (ret) {
            eicc_print_error("eicc_probe failed\n");
            return ret;
        }
    }
    eicc_print_always("eicc_devices_init completed ok!\n");
    return 0;
}
