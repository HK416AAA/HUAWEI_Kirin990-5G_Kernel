/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <bsp_fiq.h>
#include <bsp_sysctrl.h>
#include <bsp_print.h>
#include <securec.h>
#undef THIS_MODU
#define THIS_MODU mod_fiq

struct fiq_ctrl {
    unsigned int fiq_init;
    void *sysctrl_base_addr;
    void *sysctrl_fiq_enable_reg;
    unsigned int sysctrl_fiq_enable_bit;
    void *sysctrl_fiq_status_reg;
    unsigned int sysctrl_fiq_status_bit;
    unsigned int chip_type;
    void *smem_base_addr;
    void *smem_fiq_status_addr;
    void *smem_fiq_clear_addr;
    void *smem_send_cnt_addr;
    void *smem_recive_cnt_addr;
};
struct fiq_ctrl g_fiq_ctrl;

static void send_fiq(fiq_num fiq_num_t)
{
    u32 regval;

    /* 更新FIQ的状态 */
    regval = readl((volatile const void *)g_fiq_ctrl.smem_fiq_status_addr);
    regval |= ((u32)0x1 << fiq_num_t);
    writel(regval, (volatile void *)g_fiq_ctrl.smem_fiq_status_addr);

    if (g_fiq_ctrl.chip_type) { /* mbb */
        writel((u32)0x1 << g_fiq_ctrl.sysctrl_fiq_enable_bit, (volatile void *)g_fiq_ctrl.sysctrl_fiq_enable_reg);
    } else { /* PHONE */
        regval = readl((volatile const void *)(g_fiq_ctrl.sysctrl_fiq_enable_reg));
        regval &= (~((u32)1 << 12));
        writel(regval, (volatile void *)g_fiq_ctrl.sysctrl_fiq_enable_reg);
    }
}
static void check_fiq_send(void)
{
    bsp_err("fiq status regval = 0x%x[bit %d]\n", readl((volatile const void *)(g_fiq_ctrl.sysctrl_fiq_status_reg)),
        g_fiq_ctrl.sysctrl_fiq_status_bit);
    if (readl((volatile const void *)g_fiq_ctrl.smem_send_cnt_addr) !=
        readl((volatile const void *)g_fiq_ctrl.smem_recive_cnt_addr)) {
        bsp_err("fiq send cnt != fiq receive cnt, lost modem fiq respond error\n");
    }
    bsp_err("fiq send cnt = %d,fiq receive cnt = %d\n", readl((volatile const void *)g_fiq_ctrl.smem_send_cnt_addr),
        readl((volatile const void *)g_fiq_ctrl.smem_recive_cnt_addr));

    if (readl((volatile const void *)g_fiq_ctrl.smem_fiq_clear_addr) !=
        readl((volatile const void *)g_fiq_ctrl.smem_fiq_status_addr)) {
        bsp_err("smem clear value != smem status value, modem fiq handler error\n");
    }
    bsp_err("smem clear value = 0x%x,smem status value = 0x%x\n",
        readl((volatile const void *)g_fiq_ctrl.smem_fiq_clear_addr),
        readl((volatile const void *)g_fiq_ctrl.smem_fiq_status_addr));

    writel(0x0, (volatile void *)g_fiq_ctrl.smem_fiq_clear_addr);
    writel(0x0, (volatile void *)g_fiq_ctrl.smem_fiq_status_addr);
}
int bsp_send_cp_fiq(fiq_num fiq_num_t)
{
    u32 tmp;
    if (!g_fiq_ctrl.fiq_init) {
        bsp_err("fiq no init,send_cp_fiq too early\n");
        return BSP_ERROR;
    }
    if (fiq_num_t >= FIQ_MAX) {
        bsp_err("fiq_num = %d error\n", fiq_num_t);
        return BSP_ERROR;
    }
    check_fiq_send();

    /* 更新中断处理次数 */
    tmp = readl((volatile const void *)g_fiq_ctrl.smem_send_cnt_addr);
    tmp += 1;
    writel(tmp, (volatile void *)g_fiq_ctrl.smem_send_cnt_addr);

    /* 发送FIQ */
    send_fiq(fiq_num_t);

    return BSP_OK;
}

int fiq_init(void)
{
    struct device_node *node = NULL;
    u32 tmp = 0;

    if (memset_s((void *)&g_fiq_ctrl, sizeof(struct fiq_ctrl), 0, sizeof(struct fiq_ctrl))) {
        bsp_err("memset fail\n");
        return -1;
    }

    g_fiq_ctrl.smem_base_addr = (void *)SHM_FIQ_BASE_ADDR;
    if (memset_s(g_fiq_ctrl.smem_base_addr, SHM_SIZE_CCORE_FIQ, 0, SHM_SIZE_CCORE_FIQ)) {
        bsp_err("memset fail\n");
        return -1;
    }

    node = of_find_compatible_node(NULL, NULL, "hisilicon,fiq");
    if (node == NULL) {
        bsp_err("get hisilicon,fiq fail!\n");
        return -1;
    }
    g_fiq_ctrl.sysctrl_base_addr = of_iomap(node, 0);
    if (g_fiq_ctrl.sysctrl_base_addr == NULL) {
        bsp_err("of iomap fail\n");
        return -1;
    }

    if (of_property_read_u32_array(node, "fiq_enable_offset", &tmp, 1)) {
        bsp_err("hisilicon,fiq fiq_enable_offset dts node not found!\n");
        return -1;
    }
    g_fiq_ctrl.sysctrl_fiq_enable_reg = g_fiq_ctrl.sysctrl_base_addr + tmp;

    if (of_property_read_u32_array(node, "fiq_enable_bit", &g_fiq_ctrl.sysctrl_fiq_enable_bit, 1)) {
        bsp_err("hisilicon,fiq fiq_enable_bit dts node not found!\n");
        return -1;
    }

    if (of_property_read_u32_array(node, "fiq_status_offset", &tmp, 1)) {
        bsp_err("hisilicon,fiq fiq_clear_reg dts node not found!\n");
        return -1;
    }
    g_fiq_ctrl.sysctrl_fiq_status_reg = g_fiq_ctrl.sysctrl_base_addr + tmp;

    if (of_property_read_u32_array(node, "fiq_status_bit", &g_fiq_ctrl.sysctrl_fiq_status_bit, 1)) {
        bsp_err("hisilicon,fiq fiq_clear_reg dts node not found!\n");
        return -1;
    }
    if (of_property_read_u32_array(node, "chip_type", &g_fiq_ctrl.chip_type, 1)) {
        bsp_err("hisilicon,fiq chip_type dts node not found!\n");
        return -1;
    }
    writel(0xFFFFFFFF, (volatile void *)SHM_FIQ_BARRIER);
    g_fiq_ctrl.smem_fiq_clear_addr = (void *)SHM_FIQ_CLEAR_ADDR;
    g_fiq_ctrl.smem_fiq_status_addr = (void *)SHM_FIQ_STATUS_ADDR;
    g_fiq_ctrl.smem_send_cnt_addr = (void *)SHM_FIQ_TOTAL_SEND_CNT;
    g_fiq_ctrl.smem_recive_cnt_addr = (void *)SHM_FIQ_TOTAL_RECIVE_CNT;

    g_fiq_ctrl.fiq_init = 0x1;
    bsp_err("init OK\n");
    return 0;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
arch_initcall(fiq_init);
#endif
EXPORT_SYMBOL(bsp_send_cp_fiq); /*lint !e19 */
