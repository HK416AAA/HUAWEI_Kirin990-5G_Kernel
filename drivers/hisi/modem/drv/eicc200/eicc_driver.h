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

#ifndef _EICC_DRIVER_H
#define _EICC_DRIVER_H

#include "eicc_platform.h"
#include "eicc_device.h"
/* offset register_define */
#define EICC_GLB_SECCTRL 0x0000  /* 全局安全配置寄存器 */
#define EICC_CORE_SECCTRL 0x0004 /* core安全配置寄存器 */

#define EICC_HW_VERSION_V200 0x32303061
#define EICC_OPIPE_SECCTRL0 0x0008 /* opipe安全配置寄存器 */
#define EICC_OPIPE_SECCTRL1 0x000C /* opipe安全配置寄存器 */
#define EICC_OPIPE_SECCTRL2 0x0010 /* opipe安全配置寄存器 */
#define EICC_OPIPE_SECCTRL3 0x0014 /* opipe安全配置寄存器 */
#define EICC_IPIPE_SECCTRL0 0x0018 /* ipipe安全配置寄存器 */
#define EICC_IPIPE_SECCTRL1 0x001C /* ipipe安全配置寄存器 */
#define EICC_IPIPE_SECCTRL2 0x0020 /* ipipe安全配置寄存器 */
#define EICC_IPIPE_SECCTRL3 0x0024 /* ipipe安全配置寄存器 */

#define EICC_HW_VERSION_V210 0x32313061
#define EICC_V210_OPIPE_SECCTRL0 0x000C /* opipe安全配置寄存器 */
#define EICC_V210_OPIPE_SECCTRL1 0x0010 /* opipe安全配置寄存器 */
#define EICC_V210_OPIPE_SECCTRL2 0x0014 /* opipe安全配置寄存器 */
#define EICC_V210_OPIPE_SECCTRL3 0x0018 /* opipe安全配置寄存器 */
#define EICC_V210_IPIPE_SECCTRL0 0x001C /* ipipe安全配置寄存器 */
#define EICC_V210_IPIPE_SECCTRL1 0x0020 /* ipipe安全配置寄存器 */
#define EICC_V210_IPIPE_SECCTRL2 0x0024 /* ipipe安全配置寄存器 */
#define EICC_V210_IPIPE_SECCTRL3 0x0028 /* ipipe安全配置寄存器 */

#define EICC_GLB_CTRL 0x0040         /* 全局控制寄存器 */
#define EICC_AXI_BUS_CFG 0x0044      /* EICC OS配置 */
#define EICC_TRANS_TIME_CNT 0x0048   /* 工作时间计数器值 */
#define EICC_TRANS_DATA_CNT 0x004C   /* 传输数据量计数器值 */
#define EICC_EICC_OPIPE_STAT0 0x0050 /* EICC状态寄存器 */
#define EICC_EICC_OPIPE_STAT1 0x0054 /* EICC状态寄存器 */
#define EICC_EICC_IPIPE_STAT0 0x0058 /* EICC状态寄存器 */
#define EICC_EICC_IPIPE_STAT1 0x005C /* EICC状态寄存器 */
#define EICC_GLB_OPIPE_INT0 0x0060   /* 全局OPIPE中断状态寄存器 */
#define EICC_GLB_OPIPE_INT1 0x0064   /* 全局OPIPE中断状态寄存器 */
#define EICC_GLB_IPIPE_INT0 0x0068   /* 全局IPIPE中断状态寄存器 */
#define EICC_GLB_IPIPE_INT1 0x006C   /* 全局IPIPE中断状态寄存器 */

#define EICC_CORE_OPIPE_INT_MASKSET0(cpu_id) (0x0080 + 0x40L * (cpu_id)) /* COREx OPIPE中断屏蔽设置寄存器 */
#define EICC_CORE_OPIPE_INT_MASKSET1(cpu_id) (0x0084 + 0x40L * (cpu_id)) /* COREx OPIPE中断屏蔽设置寄存器 */
#define EICC_CORE_IPIPE_INT_MASKSET0(cpu_id) (0x0088 + 0x40L * (cpu_id)) /* COREx IPIPE中断屏蔽设置寄存器 */
#define EICC_CORE_IPIPE_INT_MASKSET1(cpu_id) (0x008C + 0x40L * (cpu_id)) /* COREx IPIPE中断屏蔽设置寄存器 */

#define EICC_CORE_OPIPE_INT_MASKCLR0(cpu_id) (0x0090 + 0x40L * (cpu_id)) /* COREx OPIPE中断屏蔽清除寄存器 */
#define EICC_CORE_OPIPE_INT_MASKCLR1(cpu_id) (0x0094 + 0x40L * (cpu_id)) /* COREx OPIPE中断屏蔽清除寄存器 */
#define EICC_CORE_IPIPE_INT_MASKCLR0(cpu_id) (0x0098 + 0x40L * (cpu_id)) /* COREx IPIPE中断屏蔽清除寄存器 */
#define EICC_CORE_IPIPE_INT_MASKCLR1(cpu_id) (0x009C + 0x40L * (cpu_id)) /* COREx IPIPE中断屏蔽清除寄存器 */

#define EICC_CORE_OPIPE_INT_MASK0(cpu_id) (0x00A0 + 0x40L * (cpu_id)) /* COREx OPIPE中断屏蔽寄存器 */
#define EICC_CORE_OPIPE_INT_MASK1(cpu_id) (0x00A4 + 0x40L * (cpu_id)) /* COREx OPIPE中断屏蔽寄存器 */
#define EICC_CORE_IPIPE_INT_MASK0(cpu_id) (0x00A8 + 0x40L * (cpu_id)) /* COREx IPIPE中断屏蔽寄存器 */
#define EICC_CORE_IPIPE_INT_MASK1(cpu_id) (0x00AC + 0x40L * (cpu_id)) /* COREx IPIPE中断屏蔽寄存器 */

#define EICC_CORE_OPIPE_INT_STAT0(cpu_id) (0x00B0 + 0x40L * (cpu_id)) /* COREx OPIPE中断状态寄存器 */
#define EICC_CORE_OPIPE_INT_STAT1(cpu_id) (0x00B4 + 0x40L * (cpu_id)) /* COREx OPIPE中断状态寄存器 */
#define EICC_CORE_IPIPE_INT_STAT0(cpu_id) (0x00B8 + 0x40L * (cpu_id)) /* COREx IPIPE中断状态寄存器 */
#define EICC_CORE_IPIPE_INT_STAT1(cpu_id) (0x00BC + 0x40L * (cpu_id)) /* COREx IPIPE中断状态寄存器 */

#define EICC_BD_NEXT_POINTER_L(opipe_id) (0x1000 + 0x20L * (opipe_id)) /* BD_RAM中的next_pointer_l */
#define EICC_BD_NEXT_POINTER_H(opipe_id) (0x1004 + 0x20L * (opipe_id)) /* BD_RAM中的next_pointer_h */
#define EICC_BD_SRC_ADDR_L(opipe_id) (0x1008 + 0x20L * (opipe_id))     /* BD_RAM中的src_addr_l */
#define EICC_BD_SRC_ADDR_H(opipe_id) (0x100C + 0x20L * (opipe_id))     /* BD_RAM中的src_addr_h */
#define EICC_BD_DES_ADDR_L(opipe_id) (0x1010 + 0x20L * (opipe_id))     /* BD_RAM中的des_addr_l */
#define EICC_BD_DES_ADDR_H(opipe_id) (0x1014 + 0x20L * (opipe_id))     /* BD_RAM中的des_addr_h */
#define EICC_BD_MODIFY_MSK(opipe_id) (0x1018 + 0x20L * (opipe_id))     /* BD_RAM中的modify_msk */
#define EICC_BD_MODIFY_DATA(opipe_id) (0x101C + 0x20L * (opipe_id))    /* BD_RAM中的modify_data */

#define EICC_EICC_DBG0 0x3000    /* EICC debug寄存器0 */
#define EICC_EICC_DBG1 0x3004    /* EICC debug寄存器1 */
#define EICC_EICC_DBG2 0x3008    /* EICC debug寄存器2 */
#define EICC_EICC_VERSION 0x3FFC /* 版本指示寄存器 */

#define EICC_OPIPE_BASE_ADDR_L(opipe_id) (0x4000 + 0x80L * (opipe_id)) /* OPIPE起始地址寄存器低位 */
#define EICC_OPIPE_BASE_ADDR_H(opipe_id) (0x4004 + 0x80L * (opipe_id)) /* OPIPE起始地址寄存器高位 */
#define EICC_OPIPE_DEPTH(opipe_id) (0x4008 + 0x80L * (opipe_id))       /* OPIPE深度寄存器 */
#define EICC_OPIPE_WPTR(opipe_id) (0x400C + 0x80L * (opipe_id))        /* OPIPE写指针寄存器 */
#define EICC_OPIPE_RPTR(opipe_id) (0x4010 + 0x80L * (opipe_id))        /* OPIPE读指针寄存器 */
#define EICC_OPIPE_PTR_ADDR_L(opipe_id) (0x4018 + 0x80L * (opipe_id))  /* OPIPE指针更新地址寄存器低位 */
#define EICC_OPIPE_PTR_ADDR_H(opipe_id) (0x401C + 0x80L * (opipe_id))  /* OPIPE指针更新地址寄存器高位 */
#define EICC_OPIPE_INT_MASK(opipe_id) (0x4040 + 0x80L * (opipe_id))    /* OPIPE中断屏蔽寄存器 */
#define EICC_OPIPE_INT_STAT(opipe_id) (0x4044 + 0x80L * (opipe_id))    /* OPIPE中断状态寄存器 */
#define EICC_OPIPE_INT_RAW(opipe_id) (0x4048 + 0x80L * (opipe_id))     /* OPIPE中断原始状态寄存器 */
#define EICC_OPIPE_CTRL(opipe_id) (0x404C + 0x80L * (opipe_id))        /* OPIPE控制寄存器 */
#define EICC_OPIPE_STAT(opipe_id) (0x4050 + 0x80L * (opipe_id))        /* OPIPE状态寄存器 */
#define EICC_OPIPE_DBG0(opipe_id) (0x4054 + 0x80L * (opipe_id))        /* OPIPE debug寄存器0 */
#define EICC_OPIPE_DBG1(opipe_id) (0x4058 + 0x80L * (opipe_id))        /* OPIPE debug寄存器1 */
#define EICC_OPIPE_DBG2(opipe_id) (0x405C + 0x80L * (opipe_id))        /* OPIPE debug寄存器2 */
#define EICC_OPIPE_DBG3(opipe_id) (0x4060 + 0x80L * (opipe_id))        /* OPIPE debug寄存器3 */

#define EICC_IPIPE_BASE_ADDR_L(ipipe_id) (0x6000 + 0x80L * (ipipe_id)) /* IPIPE起始地址寄存器低位 */
#define EICC_IPIPE_BASE_ADDR_H(ipipe_id) (0x6004 + 0x80L * (ipipe_id)) /* IPIPE起始地址寄存器高位 */
#define EICC_IPIPE_DEPTH(ipipe_id) (0x6008 + 0x80L * (ipipe_id))       /* IPIPE深度寄存器 */
#define EICC_IPIPE_WPTR(ipipe_id) (0x600C + 0x80L * (ipipe_id))        /* IPIPE写指针寄存器 */
#define EICC_IPIPE_RPTR(ipipe_id) (0x6010 + 0x80L * (ipipe_id))        /* IPIPE读指针寄存器 */
#define EICC_IPIPE_REMOTE_CTRL(ipipe_id) (0x6014 + 0x80L * (ipipe_id)) /* IPIPE remote控制寄存器	 */
#define EICC_IPIPE_PTR_ADDR_L(ipipe_id) (0x6018 + 0x80L * (ipipe_id))  /* IPIPE指针更新地址寄存器低位 */
#define EICC_IPIPE_PTR_ADDR_H(ipipe_id) (0x601C + 0x80L * (ipipe_id))  /* IPIPE指针更新地址寄存器高位 */
#define EICC_IPIPE_CFG_ADDR_L(ipipe_id) (0x6020 + 0x80L * (ipipe_id))  /* IPIPE指针更新地址寄存器低位 */
#define EICC_IPIPE_CFG_ADDR_H(ipipe_id) (0x6024 + 0x80L * (ipipe_id))  /* IPIPE指针更新地址寄存器高位 */
#define EICC_IPIPE_INT_MASK(ipipe_id) (0x6040 + 0x80L * (ipipe_id))    /* IPIPE中断屏蔽寄存器 */
#define EICC_IPIPE_INT_STAT(ipipe_id) (0x6044 + 0x80L * (ipipe_id))    /* IPIPE中断状态寄存器 */
#define EICC_IPIPE_INT_RAW(ipipe_id) (0x6048 + 0x80L * (ipipe_id))     /* IPIPE中断原始状态寄存器 */
#define EICC_IPIPE_CTRL(ipipe_id) (0x604C + 0x80L * (ipipe_id))        /* IPIPE控制寄存器 */
#define EICC_IPIPE_STAT(ipipe_id) (0x6050 + 0x80L * (ipipe_id))        /* IPIPE状态寄存器 */
#define EICC_IPIPE_DBG0(ipipe_id) (0x6054 + 0x80L * (ipipe_id))        /* IPIPE debug寄存器0 */
#define EICC_IPIPE_DBG1(ipipe_id) (0x6058 + 0x80L * (ipipe_id))        /* IPIPE debug寄存器1 */
#define EICC_IPIPE_DBG2(ipipe_id) (0x605C + 0x80L * (ipipe_id))        /* IPIPE debug寄存器2 */
#define EICC_IPIPE_DBG3(ipipe_id) (0x6060 + 0x80L * (ipipe_id))        /* IPIPE debug寄存器3 */

#define SECREG_EN_MASK 0xFFFF0000UL
/* -------------------EICC全局安全控制----------------------------- */
#define GLB_SECCTRL_SECCTRL 0x10001

/* -------------------EICC全局控制----------------------------- */
#define GLB_CTRL_HACGIF_AUTOGATE (1 << 8) /* HAC_GIF模块时钟使能 */
#define GLB_CTRL_ROBF_AUTOGATE (1 << 7)   /* ROBF模块时钟使能 */
#define GLB_CTRL_ROBF_BYPASS (1 << 6)     /* ROBF模块bypass使能 */
#define GLB_CTRL_INT_MSK_SEL (1 << 5)     /* 控制v100继承的corex中断maskbit是否有效 */
#define GLB_CTRL_AXI_BUS (1 << 4)         /* AXI总线resp error控制位 */
#define GLB_CTRL_AHB_BUS (1 << 3)         /* AHB总线hresp返回error控制位 */
#define GLB_CTRL_SR_PUSH (1 << 2)         /* 低功耗流程推送控制 */
#define GLB_CTRL_CLK_AUTO (1 << 1)        /* 自动门控使能控制 */
#define GLB_CTRL_SRST (1 << 0)            /* EICC模块全局软复位控制 */

/* -------------------STAT BITS FIELD----------------------------- */
#define PIPE_STATUS_EMPTY (1 << 0)
#define PIPE_STATUS_FULL (1 << 1)
#define PIPE_STATUS_IDLE (1 << 2)

#define EICC_PIPE_CNT_PER_REG 32

int eicc_irqs_init(void);

void eicc_core_unsec(const struct eicc_device *pdev, u32 coreid);
void eicc_opipe_unsec(const struct eicc_device *pdev, u32 pipeid);
void eicc_ipipe_unsec(const struct eicc_device *pdev, u32 pipeid);

int ipipe_neg_check(const void *base_addr, u32 ipipe_id);
int ipipe_status_check(const void *base_addr, u32 ipipe_id);
int opipe_status_check(const void *base_addr, u32 opipe_id);

void eicc_opipe_startup(void *base_addr, struct opipe_config *opipe);
void eicc_ipipe_startup(void *base_addr, struct ipipe_config *ipipe);
void eicc_shadow_ipipe_startup(void *base_addr, struct ipipe_config *ipipe);

/*lint -esym(528,eicc_dev_glbctrl_set) */
static inline void eicc_dev_glbctrl_set(void *base_addr, u32 glb_cfg)
{
    writel(glb_cfg, (void *)((char *)base_addr + EICC_GLB_CTRL));
}

/*lint -esym(528,eicc_dev_glbctrl_get) */
static inline u32 eicc_dev_glbctrl_get(const void *base_addr)
{
    return readl((void *)((char *)base_addr + EICC_GLB_CTRL));
}

/*lint -esym(528,eicc_opipe_is_en) */
static inline u32 eicc_opipe_is_en(const void *base_addr, u32 pipe_id)
{
    u32 status;
    status = readl((void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
    return (status & 1);
}

/*lint -esym(528,eicc_opipe_local_en) */
static inline void eicc_opipe_local_en(void *base_addr, u32 pipe_id)
{
    u32 status;

    status = readl((void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
    writel(status | 1, (void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
}

/*lint -esym(528,eicc_opipe_local_dis) */
static inline void eicc_opipe_local_dis(void *base_addr, u32 pipe_id)
{
    u32 status;

    status = readl((void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
    writel(status & (~1), (void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
}

/*lint -esym(528,eicc_opipe_abort_set) */
static inline void eicc_opipe_abort_set(void *base_addr, u32 pipe_id)
{
    u32 status;

    status = readl((void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
    writel(status | 0x100, (void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
}

/*lint -esym(528,eicc_opipe_abort_clr) */
static inline void eicc_opipe_abort_clr(void *base_addr, u32 pipe_id)
{
    u32 status;

    status = readl((void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
    writel(status & (~0x100), (void *)((char *)base_addr + EICC_OPIPE_CTRL(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_is_en) */
static inline u32 eicc_ipipe_is_en(const void *base_addr, u32 pipe_id)
{
    u32 status;
    status = readl((void *)((char *)base_addr + EICC_IPIPE_CTRL(pipe_id)));
    return (status & 1);
}

/*lint -esym(528,eicc_ipipe_local_en) */
static inline void eicc_ipipe_local_en(void *base_addr, u32 pipe_id)
{
    u32 status;

    status = readl((void *)((char *)base_addr + EICC_IPIPE_CTRL(pipe_id)));
    writel(status | 1, (void *)((char *)base_addr + EICC_IPIPE_CTRL(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_local_dis) */
static inline void eicc_ipipe_local_dis(void *base_addr, u32 pipe_id)
{
    u32 status;

    status = readl((void *)((char *)base_addr + EICC_IPIPE_CTRL(pipe_id)));
    writel(status & (~1), (void *)((char *)base_addr + EICC_IPIPE_CTRL(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_remote_status) */
static inline u32 eicc_ipipe_remote_status(const void *base_addr, u32 pipe_id)
{
    u32 status;
    status = readl((void *)((char *)base_addr + EICC_IPIPE_REMOTE_CTRL(pipe_id)));
    return status;
}

/*lint -esym(528,eicc_ipipe_remote_en) */
static inline void eicc_ipipe_remote_en(void *base_addr, u32 pipe_id)
{
    writel(1, (void *)((char *)base_addr + EICC_IPIPE_REMOTE_CTRL(pipe_id)));
}

/*lint -esym(528,eicc_opipe_coreint_en) */
static inline void eicc_opipe_coreint_en(void *base_addr, u32 pipe_id, u32 cpu_id)
{
    if (pipe_id < EICC_PIPE_CNT_PER_REG) {
        writel(1UL << pipe_id, (void *)((char *)base_addr + EICC_CORE_OPIPE_INT_MASKSET0(cpu_id)));
    } else {
        writel(1UL << (pipe_id - EICC_PIPE_CNT_PER_REG),
            (void *)((char *)base_addr + EICC_CORE_OPIPE_INT_MASKSET1(cpu_id)));
    }
}

/*lint -esym(528,eicc_opipe_coreint_dis) */
static inline void eicc_opipe_coreint_dis(void *base_addr, u32 pipe_id, u32 cpu_id)
{
    if (pipe_id < EICC_PIPE_CNT_PER_REG) {
        writel(1UL << pipe_id, (void *)((char *)base_addr + EICC_CORE_OPIPE_INT_MASKCLR0(cpu_id)));
    } else {
        writel(1UL << (pipe_id - EICC_PIPE_CNT_PER_REG),
            (void *)((char *)base_addr + EICC_CORE_OPIPE_INT_MASKCLR1(cpu_id)));
    }
}

/*lint -esym(528,eicc_ipipe_coreint_en) */
static inline void eicc_ipipe_coreint_en(void *base_addr, u32 pipe_id, u32 cpu_id)
{
    if (pipe_id < EICC_PIPE_CNT_PER_REG) {
        writel(1UL << pipe_id, (void *)((char *)base_addr + EICC_CORE_IPIPE_INT_MASKSET0(cpu_id)));
    } else {
        writel(1UL << (pipe_id - EICC_PIPE_CNT_PER_REG),
            (void *)((char *)base_addr + EICC_CORE_IPIPE_INT_MASKSET1(cpu_id)));
    }
}

/*lint -esym(528,eicc_ipipe_coreint_dis) */
static inline void eicc_ipipe_coreint_dis(void *base_addr, u32 pipe_id, u32 cpu_id)
{
    if (pipe_id < EICC_PIPE_CNT_PER_REG) {
        writel(1UL << pipe_id, (void *)((char *)base_addr + EICC_CORE_IPIPE_INT_MASKCLR0(cpu_id)));
    } else {
        writel(1UL << (pipe_id - EICC_PIPE_CNT_PER_REG),
            (void *)((char *)base_addr + EICC_CORE_IPIPE_INT_MASKCLR1(cpu_id)));
    }
}

/*lint -esym(528,eicc_opipe_stat_isbusy) */
static inline int eicc_opipe_stat_isbusy(const void *base_addr, u32 pipe_id)
{
    u32 status;

    if (pipe_id < EICC_PIPE_CNT_PER_REG) {
        status = readl((void *)((char *)base_addr + EICC_EICC_OPIPE_STAT0));
        status &= (1UL << pipe_id);
        if (!status) {
            return 1;
        }
    } else {
        status = readl((void *)((char *)base_addr + EICC_EICC_OPIPE_STAT1));
        status &= (1UL << (pipe_id - EICC_PIPE_CNT_PER_REG));
        if (!status) {
            return 1;
        }
    }
    return 0;
}

/*lint -esym(528,eicc_ipipe_stat_isbusy) */
static inline int eicc_ipipe_stat_isbusy(const void *base_addr, u32 pipe_id)
{
    u32 status;

    if (pipe_id < EICC_PIPE_CNT_PER_REG) {
        status = readl((void *)((char *)base_addr + EICC_EICC_IPIPE_STAT0));
        status &= (1UL << pipe_id);
        if (!status) {
            return 1;
        }
    } else {
        status = readl((void *)((char *)base_addr + EICC_EICC_IPIPE_STAT1));
        status &= (1UL << (pipe_id - EICC_PIPE_CNT_PER_REG));
        if (!status) {
            return 1;
        }
    }
    return 0;
}

/*lint -esym(528,eicc_opipe_devint_en) */
static inline u32 eicc_opipe_devint_en(void *base_addr, u32 pipe_id, u32 level)
{
    u32 reg;

    reg = readl((void *)((char *)base_addr + EICC_OPIPE_INT_MASK(pipe_id)));
    writel(reg | level, (void *)((char *)base_addr + EICC_OPIPE_INT_MASK(pipe_id)));

    return (reg | level);
}

/*lint -esym(528,eicc_opipe_devint_dis) */
static inline u32 eicc_opipe_devint_dis(void *base_addr, u32 pipe_id, u32 level)
{
    u32 reg;

    reg = readl((void *)((char *)base_addr + EICC_OPIPE_INT_MASK(pipe_id)));
    reg &= (~level);
    writel(reg, (void *)((char *)base_addr + EICC_OPIPE_INT_MASK(pipe_id)));

    return reg;
}

/*lint -esym(528,eicc_opipe_devint_stat) */
static inline u32 eicc_opipe_devint_stat(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_OPIPE_INT_STAT(pipe_id)));
}

/*lint -esym(528,eicc_opipe_devint_raw) */
static inline u32 eicc_opipe_devint_raw(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_OPIPE_INT_RAW(pipe_id)));
}

/*lint -esym(528,eicc_opipe_devint_clr) */
static inline void eicc_opipe_devint_clr(void *base_addr, u32 pipe_id, u32 level)
{
    writel(level, (void *)((char *)base_addr + EICC_OPIPE_INT_RAW(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_devint_en) */
static inline u32 eicc_ipipe_devint_en(void *base_addr, u32 pipe_id, u32 level)
{
    u32 reg;

    reg = readl((void *)((char *)base_addr + EICC_IPIPE_INT_MASK(pipe_id)));
    writel(reg | level, (void *)((char *)base_addr + EICC_IPIPE_INT_MASK(pipe_id)));

    return (reg | level);
}

/*lint -esym(528,eicc_ipipe_devint_dis) */
static inline u32 eicc_ipipe_devint_dis(void *base_addr, u32 pipe_id, u32 level)
{
    u32 reg;

    reg = readl((void *)((char *)base_addr + EICC_IPIPE_INT_MASK(pipe_id)));
    reg &= (~level);
    writel(reg, (void *)((char *)base_addr + EICC_IPIPE_INT_MASK(pipe_id)));

    return reg;
}

/*lint -esym(528,eicc_ipipe_devint_stat) */
static inline u32 eicc_ipipe_devint_stat(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_IPIPE_INT_STAT(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_devint_raw) */
static inline u32 eicc_ipipe_devint_raw(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_IPIPE_INT_RAW(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_devint_clr) */
static inline void eicc_ipipe_devint_clr(void *base_addr, u32 pipe_id, u32 level)
{
    writel(level, (void *)((char *)base_addr + EICC_IPIPE_INT_RAW(pipe_id)));
}

/*lint -esym(528,eicc_opipe_wptr_update) */
static inline void eicc_opipe_wptr_update(void *base_addr, u32 pipe_id, u32 wptr)
{
    writel(wptr, (void *)((char *)base_addr + EICC_OPIPE_WPTR(pipe_id)));
}

/*lint -esym(528,eicc_opipe_rptr_update) */
static inline void eicc_opipe_rptr_update(void *base_addr, u32 pipe_id, u32 rptr)
{
    writel(rptr, (void *)((char *)base_addr + EICC_OPIPE_RPTR(pipe_id)));
}

/*lint -esym(528,eicc_opipe_wptr_get) */
static inline u32 eicc_opipe_wptr_get(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_OPIPE_WPTR(pipe_id)));
}

/*lint -esym(528,eicc_opipe_rptr_get) */
static inline u32 eicc_opipe_rptr_get(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_OPIPE_RPTR(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_wptr_update) */
static inline void eicc_ipipe_wptr_update(void *base_addr, u32 pipe_id, u32 wptr)
{
    writel(wptr & 0xFFFF, (void *)((char *)base_addr + EICC_IPIPE_WPTR(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_rptr_update) */
static inline void eicc_ipipe_rptr_update(void *base_addr, u32 pipe_id, u32 rptr)
{
    writel(rptr, (void *)((char *)base_addr + EICC_IPIPE_RPTR(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_wptr_get) */
static inline u32 eicc_ipipe_wptr_get(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_IPIPE_WPTR(pipe_id))) & 0xFFFF;
}

/*lint -esym(528,eicc_opipe_local_status) */
static inline u32 eicc_opipe_local_status(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_OPIPE_STAT(pipe_id)));
}

/*lint -esym(528,eicc_ipipe_local_status) */
static inline u32 eicc_ipipe_local_status(const void *base_addr, u32 pipe_id)
{
    return readl((void *)((char *)base_addr + EICC_IPIPE_STAT(pipe_id)));
}

#endif /* EICC_DRIVER_H */
