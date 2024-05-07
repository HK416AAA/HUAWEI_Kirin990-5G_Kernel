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

#ifndef __RESET_BALONG_H__
#define __RESET_BALONG_H__

#ifdef __cplusplus /* __cplusplus */
extern "C" {
#endif /* __cplusplus */

#include <linux/printk.h>
#include <linux/suspend.h>
#include <linux/workqueue.h>
#include <linux/completion.h>
#include <bsp_print.h>

#include <osl_sem.h>
#include <osl_spinlock.h>
#include <bsp_print.h>
#include <bsp_pmu_gpio.h>

/* ************************* 宏和枚举定义 ************************* */
#define pmu_print_err(fmt, ...) (bsp_err("[%s] " fmt, __FUNCTION__, ##__VA_ARGS__))

#ifndef OK
#define OK 0
#endif

#ifndef ERROR
#define ERROR (-1)
#endif

#ifndef MASKED
#define MASKED (1)
#endif
#ifndef UNMASKED
#define UNMASKED (0)
#endif

#define PMU_MAGIC_NUM 0xAEAEAEAE

// GPIO 上拉下拉 与 PMU 的HOLD 管脚处的上拉下拉相反
#define REVERSE_POWER_METHOD 1
// GPIO 上拉下拉 与 PMU 的HOLD 管脚处的上拉下拉相同
#define POWER_METHOD 0

typedef void (*pmu_irq_cb)(void *);

typedef enum {
    PMU_EARLY_INIT_START,
    PMU_EARLY_INIT_END,
    PMU_RESET_INT,
    PMU_POWERON_INIT,
    PMU_IRQ_MALLOC,
    PMU_IRQ_SETUP,
    PMU_IRQ_FAIL,
    PMU_IRQ_INIT,
    PMU_IRQ_HANDLE,
    PMU_INIT_END,
    PMU_DEBUG_NUM_MAX
} pmu_debug_step_e;
/* 结构体定义 */

struct pmu_stamp {
    unsigned irq_num;
};

struct pmu_gpio_ctrl {
    u32 gpio_num;
    u32 gpio_mask;
};

struct pmu_irq_handle {
    u32 irq_num;
    pmu_irq_cb routine;
    void *data;
    u32 cnt;
};

struct pmu_irq {
    u32 irq;
    u32 cnts;
    u32 irq_nums;
    struct resource *res;
    spinlock_t irq_lock;
    struct pmu_irq_handle irq_handler[MAX_IRQ_ID];
};

struct pmu_debug {
    u32 time_stamp;
};
#endif
