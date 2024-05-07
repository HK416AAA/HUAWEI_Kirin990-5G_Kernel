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

#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/irqnr.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/gpio.h>
#include "osl_spinlock.h"
#include <osl_malloc.h>
#include <soc_interrupts.h>
#include <bsp_dump.h>
#include <bsp_vic.h>
#include "bsp_onoff.h"
#include "pmu_balong.h"
#include "bsp_pmu_gpio.h"
#include <bsp_slice.h>
#include <securec.h>

#undef THIS_MODU
#define THIS_MODU mod_pmu

struct pmu_gpio_ctrl g_pmu_gpio_ctrl[MAX_GPIO_ID];
struct pmu_irq *g_pmu_irqinfo = NULL;
u32 g_poweron_method = 0;
struct pmu_debug g_modem_early_debug[PMU_EARLY_INIT_END + 1] = {0};
u32 *g_modem_pmu_debug = NULL;

int pmu_gpio_init(void)
{
    struct device_node *node = NULL;
    u32 ret = OK;
    node = of_find_compatible_node(NULL, NULL, "hisilicon,cp_pmu_app");
    if (node == NULL) {
        printk("dts node not found!\n");
        return ERROR;
    }

    ret = (u32)of_property_read_u32(node, "poweron", &g_pmu_gpio_ctrl[POWERON].gpio_num);
    ret |= (u32)of_property_read_u32(node, "reset", &g_pmu_gpio_ctrl[RESET].gpio_num);
    ret |= (u32)of_property_read_u32(node, "irq", &g_pmu_gpio_ctrl[IRQ].gpio_num);
    ret |= (u32)of_property_read_u32(node, "poweron_method", &g_poweron_method);

    g_pmu_gpio_ctrl[POWERON].gpio_mask = UNMASKED;
    g_pmu_gpio_ctrl[RESET].gpio_mask = UNMASKED;
    g_pmu_gpio_ctrl[IRQ].gpio_mask = UNMASKED;

    if (ret) {
        printk("read gpio num from dts is failed,ret = %d!\n", ret);
        return ERROR;
    }

    return (int)ret;
}

void pmu_record_debug(u32 step)
{
    if (g_modem_pmu_debug != NULL && step < PMU_DEBUG_NUM_MAX) {
        g_modem_pmu_debug[step] = PMU_MAGIC_NUM;
    }
}

int modem_reset_gpio_init(void)
{
    int ret = 0;
    ret = gpio_request(g_pmu_gpio_ctrl[RESET].gpio_num, "modem_reset_gpio");
    if (ret) {
        pmu_print_err("modem_reset_gpio request failed, ret %d. \n", ret);
        return ERROR;
    }
    pmu_print_err("modem_reset_gpio num %d. \n", g_pmu_gpio_ctrl[RESET].gpio_num);
    return ret;
}

int modem_poweron_gpio_init(void)
{
    int ret;
    ret = gpio_request(g_pmu_gpio_ctrl[POWERON].gpio_num, "modem_poweron_gpio");
    if (ret) {
        pmu_print_err("modem_poweron_gpio request failed, ret %d. \n", ret);
        return ERROR;
    }
    pmu_print_err("modem_poweron_gpio num %d. \n", g_pmu_gpio_ctrl[POWERON].gpio_num);
    return ret;
}

int bsp_modem_pmu_irq_callback_register(unsigned int irq, void *routine, void *data)
{
    if (g_pmu_irqinfo == NULL) {
        return ERROR;
    }
    if (irq >= g_pmu_irqinfo->irq_nums) {
        pmu_print_err("pmic irq is error,please check irq=%d\n", (int)irq);
        return ERROR;
    }

    g_pmu_irqinfo->irq_handler[irq].irq_num = irq;
    g_pmu_irqinfo->irq_handler[irq].routine = routine;
    g_pmu_irqinfo->irq_handler[irq].data = data;

    return OK;
}

void bsp_modem_pmu_irq_mask(void)
{
    g_pmu_gpio_ctrl[IRQ].gpio_mask = MASKED;
}

void bsp_modem_pmu_irq_unmask(void)
{
    g_pmu_gpio_ctrl[IRQ].gpio_mask = UNMASKED;
}

static __inline__ void modem_pmu_irq_enable(void)
{
    enable_irq(g_pmu_irqinfo->irq);
}

static __inline__ void modem_pmu_irq_disable(void)
{
    disable_irq_nosync(g_pmu_irqinfo->irq);
}

irqreturn_t modem_pmu_gpio_irq_handler(int irq, void *data)
{
    struct pmu_irq *modem_pmu_irq = (struct pmu_irq *)data;
    unsigned int i;
    /* close int until int task finished */
    modem_pmu_irq->cnts++;
    if (g_pmu_gpio_ctrl[IRQ].gpio_mask == MASKED) {
        return IRQ_HANDLED;
    }
    modem_pmu_irq_disable();
    pmu_record_debug(PMU_IRQ_HANDLE);
    for (i = 0; i < modem_pmu_irq->irq_nums; i++) {
        if (modem_pmu_irq->irq_handler[i].routine != NULL) {
            modem_pmu_irq->irq_handler[i].routine(modem_pmu_irq->irq_handler[i].data);
        }
    }
    modem_pmu_irq_enable();
    return IRQ_HANDLED;
}

int modem_pmu_irq_gpio_setup(void)
{
    int ret;
    ret = gpio_request(g_pmu_gpio_ctrl[IRQ].gpio_num, "modem_irq_gpio");
    if (ret) {
        pmu_print_err("gpio request failed, ret %d. \n", ret);
        return ret;
    }
    pmu_print_err("modem_irq_gpio num %d. \n", g_pmu_gpio_ctrl[IRQ].gpio_num);
    ret = gpio_direction_input(g_pmu_gpio_ctrl[IRQ].gpio_num);
    return ret;
}

int modem_pmu_irq_gpio_init(void)
{
    int ret;
    pmu_record_debug(PMU_IRQ_MALLOC);

    ret = modem_pmu_irq_gpio_setup();
    if (ret) {
        pmu_print_err("gpio set failed, ret %d. \n", ret);
        return ERROR;
    }
    pmu_record_debug(PMU_IRQ_SETUP);

    ret = request_irq((g_pmu_irqinfo->irq), modem_pmu_gpio_irq_handler, IRQF_NO_SUSPEND | IRQF_TRIGGER_FALLING,
                      "modem_boot_gpio", g_pmu_irqinfo);
    if (ret) {
        pmu_print_err("unable to request MODEM PMU GPIO IRQ!\n");
        gpio_free(g_pmu_gpio_ctrl[IRQ].gpio_num);
        pmu_record_debug(PMU_IRQ_FAIL);
        return ERROR;
    }
    pmu_record_debug(PMU_IRQ_INIT);
    return ret;
}

int bsp_modem_boot_poweron(void)
{
    int ret = 0;
    ret = gpio_direction_output(g_pmu_gpio_ctrl[POWERON].gpio_num, 1);
    if (ret) {
        pmu_print_err("fail to set GPIO<%u> to 1. \n", g_pmu_gpio_ctrl[POWERON].gpio_num);
        return ret;
    }

    if (g_poweron_method == REVERSE_POWER_METHOD) {
        msleep(60);
        ret = gpio_direction_output(g_pmu_gpio_ctrl[POWERON].gpio_num, 0);
        if (ret) {
            pmu_print_err("[AP_MODEM_BOOT]fail to set GPIO<%u> to 0. \n", g_pmu_gpio_ctrl[POWERON].gpio_num);
            return ret;
        }
    }
    bsp_modem_pmu_irq_unmask();
    return ret;
}

int bsp_modem_boot_poweroff(void)
{
    int ret = 0;
    bsp_modem_pmu_irq_mask();
    if (g_poweron_method == POWER_METHOD) {
        ret = gpio_direction_output(g_pmu_gpio_ctrl[POWERON].gpio_num, 0);
        if (ret) {
            pmu_print_err("fail to power off modem. GPIO<%u>, ret = %d\n", g_pmu_gpio_ctrl[POWERON].gpio_num, ret);
            return ret;
        }
    }
    return ret;
}

int bsp_get_pmu_status(void)
{
    if (gpio_get_value(g_pmu_gpio_ctrl[IRQ].gpio_num)) {
        return OK;
    } else {
        return ERROR;
    }
}

int bsp_modem_pmu_reset(u32 delayMs)
{
    int ret = 0;
    bsp_modem_pmu_irq_mask();
    ret = gpio_direction_output(g_pmu_gpio_ctrl[RESET].gpio_num, 0);
    if (ret) {
        pmu_print_err("fail to set GPIO<%u> to 0. \n", g_pmu_gpio_ctrl[RESET].gpio_num);
        bsp_modem_pmu_irq_unmask();
        return ERROR;
    }
    mdelay(delayMs);
    ret = gpio_direction_output(g_pmu_gpio_ctrl[RESET].gpio_num, 1);
    if (ret) {
        pmu_print_err("fail to set GPIO<%u> to 1. \n", g_pmu_gpio_ctrl[RESET].gpio_num);
        bsp_modem_pmu_irq_unmask();
        return ERROR;
    }
    ret = gpio_direction_input(g_pmu_gpio_ctrl[RESET].gpio_num);
    bsp_modem_pmu_irq_unmask();
    return ret;
}
int bsp_modem_pmu_early_init(void)
{
    int ret = 0;
    pmu_print_err("early pmu init start!\n");
    g_modem_early_debug[PMU_EARLY_INIT_START].time_stamp = bsp_get_slice_value();
    ret = pmu_gpio_init();
    if (ret) {
        pmu_print_err("gpio init fail!\n");
        return ERROR;
    }

    g_pmu_irqinfo = (struct pmu_irq *)kmalloc(sizeof(struct pmu_irq), GFP_KERNEL);
    if (g_pmu_irqinfo == NULL) {
        pmu_print_err("irq_info to kmalloc!\n");
        return ERROR;
    }
    memset_s((void *)g_pmu_irqinfo, (sizeof(*g_pmu_irqinfo)), 0, (sizeof(struct pmu_irq)));

    g_pmu_irqinfo->irq = (unsigned int)gpio_to_irq((unsigned)(g_pmu_gpio_ctrl[IRQ].gpio_num));
    g_pmu_irqinfo->irq_nums = MAX_IRQ_ID;
    g_pmu_irqinfo->cnts = 0;
    g_modem_early_debug[PMU_EARLY_INIT_END].time_stamp = bsp_get_slice_value();
    pmu_print_err("early pmu init start ok!\n");
    return OK;
}
int bsp_modem_pmu_init(void)
{
    u32 ret;
    int i = 0;
    int pmu_debug_size = PMU_DEBUG_NUM_MAX * sizeof(struct pmu_debug);
    g_modem_pmu_debug = (u32 *)bsp_dump_register_field(DUMP_MODEMAP_PMU, "modem_pmu", pmu_debug_size, 0);
    if (g_modem_pmu_debug == NULL) {
        pmu_print_err("modem pmu request dump fail. \n");
    }

    for (; i <= PMU_EARLY_INIT_END; i++) {
        pmu_record_debug(i);
    }

    ret = (u32)modem_reset_gpio_init();
    pmu_record_debug(PMU_RESET_INT);
    ret |= (u32)modem_poweron_gpio_init();
    pmu_record_debug(PMU_POWERON_INIT);
    ret |= (u32)modem_pmu_irq_gpio_init();
    pmu_print_err("modem pmu init ok. \n");
    pmu_record_debug(PMU_POWERON_INIT);
    return (int)ret;
}

EXPORT_SYMBOL(bsp_modem_pmu_init);                  /*lint !e19 */
EXPORT_SYMBOL(bsp_modem_boot_poweron);              /*lint !e19 */
EXPORT_SYMBOL(bsp_modem_boot_poweroff);             /*lint !e19 */
EXPORT_SYMBOL(bsp_modem_pmu_reset);                 /*lint !e19 */
EXPORT_SYMBOL(bsp_modem_pmu_irq_callback_register); /*lint !e19 */
EXPORT_SYMBOL(bsp_modem_pmu_irq_mask);              /*lint !e19 */
EXPORT_SYMBOL(bsp_modem_pmu_irq_unmask);            /*lint !e19 */

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
arch_initcall(bsp_modem_pmu_early_init);
arch_initcall(bsp_modem_pmu_init); /*lint !e19*/
#endif
