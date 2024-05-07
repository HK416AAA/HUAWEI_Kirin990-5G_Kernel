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

#ifndef __HARDTIMER_HISI_H__
#define __HARDTIMER_HISI_H__

// 自研timer的寄存器定义
#define HITIMER_CLK_CTRL_OFFSET 0x0
#define HITIMER_INIT_OFFSET 0x4
#define HITIMER_CTRL_OFFSET 0x8
#define HITIMER_INTR_OFFSET 0xC
#define HITIMER_INTR_CLR_OFFSET 0x10
#define HITIMER_COUNT_VALUE 0x14
#define HITIMER_VERSION 0xF0
typedef union {
    struct {
        unsigned int timeren:1;
        unsigned int timersize:1;
        unsigned int oneshot:1;
        unsigned int timer_reload:1;
        unsigned int internable:1;
        unsigned int :24; // reserved
        unsigned int timermode:1;
        unsigned int timerpre:2;
    };
    unsigned int ctrl_val;
} ctrl_reg_u;

#define TIMER_LOAD_OFFSET HITIMER_INIT_OFFSET
#define TIMER_VALUE_OFFSET HITIMER_COUNT_VALUE
#define TIMER_CTRL_OFFSET HITIMER_CTRL_OFFSET
#define TIMER_INTCLR_OFFSET HITIMER_INTR_CLR_OFFSET
#define TIMER_INTRIS_OFFSET HITIMER_INTR_OFFSET  // 自研timer的原始中断和屏蔽后的中断在一个寄存器中
#define TIMER_INTMIS_OFFSET HITIMER_INTR_OFFSET
#define TIMER_BGLOAD_OFFSET HITIMER_INIT_OFFSET

#define IS_TIMER_ENABLE(value) ((value) & 0x1U)
#define TIMER_ENABLE_VAL(value) ((value) | 0x1U)
#define TIMER_DISABLE_VAL(value) ((value) & (~0x1U))
#define TIMER_INT_MASK_VAL(value) ((value) | 0x10U)
#define TIMER_INT_UNMASK_VAL(value) ((value) & (~0x10U))
#define DISABLE_AND_MASK_TIMER(value) ((value) & (~0x10U))
#define TIMER_ONCE_REG_VAL 0xEU
#define TIMER_PERIOD_REG_VAL 0xAU
#define TIMER_INT_ENABLE 0
#define TIMER_INT_DISABLE 1

#define TIMER_32K_CLK 32764
#define TIMER_CLK_32K_VALUE 0x4U
#define TIMER_CLK_19M_VALUE 0x6U


#endif

