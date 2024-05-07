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

#ifndef _SOCP_HAL_H
#define _SOCP_HAL_H

#include <linux/device.h>
#include "osl_types.h"
#include "osl_bio.h"
#include "bsp_dt.h"
#include "drv_comm.h"


#ifdef __cplusplus
extern "C" {
#endif
/* SOCP全局控制寄存器组 */
#define SOCP_REG_GBLRST 0x0
#define SOCP_REG_ENCRST 0x4
#define SOCP_REG_ENCSTAT 0xc
#define SOCP_REG_CLKCTRL 0x14
#define SOCP_REG_INTTIMEOUT 0x24


/* SOCP安全控制寄存器组 */
#define SOCP_REG_ENCSRC_SEC_CTRL_L 0x30

/* 中断寄存器组 */
#define SOCP_REG_GBL_INTSTAT 0x50
#define SOCP_REG_ENC_CORE0_MASK0 0x54
#define SOCP_REG_ENC_RAWINT0 0x58
#define SOCP_REG_ENC_CORE0_INT0 0x5c

#define SOCP_REG_APP_MASK1 0x60
#define SOCP_REG_ENC_RAWINT1 0x68
#define SOCP_REG_APP_INTSTAT1 0x6c
#define SOCP_REG_ENC_CORE0_MASK2 0x74
#define SOCP_REG_ENC_RAWINT2 0x78
#define SOCP_REG_ENC_CORE0_INTSTAT2 0x7c
#define SOCP_REG_APP_MASK3 0x80
#define SOCP_REG_ENC_RAWINT3 0x88
#define SOCP_REG_APP_INTSTAT3 0x8c

/* 编码源通道buffer寄存器组 */
#define SOCP_REG_ENCSRC_BUFWPTR(m) (0x100 + (m) * 0x40)
#define SOCP_REG_ENCSRC_BUFRPTR(m) (0x104 + (m) * 0x40)
#define SOCP_REG_ENCSRC_BUFADDR_L(m) (0x108 + (m) * 0x40)
#define SOCP_REG_ENCSRC_BUFADDR_H(m) (0x10c + (m) * 0x40)
#define SOCP_REG_ENCSRC_BUFDEPTH(m) (0x110 + (m) * 0x40)
#define SOCP_REG_ENCSRC_BUFCFG(m) (0x114 + (m) * 0x40)
#define SOCP_REG_ENCSRC_RDQWPTR(m) (0x118 + (m) * 0x40)
#define SOCP_REG_ENCSRC_RDQRPTR(m) (0x11c + (m) * 0x40)
#define SOCP_REG_ENCSRC_RDQADDR_L(m) (0x120 + (m) * 0x40)
#define SOCP_REG_ENCSRC_RDQADDR_H(m) (0x124 + (m) * 0x40)
#define SOCP_REG_ENCSRC_RDQCFG(m) (0x128 + (m) * 0x40)
#define SOCP_REG_ENCSRC_RDQ_WPTR_ADDR_L(m) (0x134 + (m) * 0x40)
#define SOCP_REG_ENCSRC_RDQ_WPTR_ADDR_H(m) (0x138 + (m) * 0x40)

/* 编码目的通道buffer寄存器 */
#define SOCP_REG_ENCDEST_BUFWPTR(n) (0x900 + (n) * 0x20)
#define SOCP_REG_ENCDEST_BUFRPTR(n) (0x904 + (n) * 0x20)
#define SOCP_REG_ENCDEST_BUFADDR_L(n) (0x908 + (n) * 0x20)
#define SOCP_REG_ENCDEST_BUFADDR_H(n) (0x90c + (n) * 0x20)
#define SOCP_REG_ENCDEST_BUFDEPTH(n) (0x910 + (n) * 0x20)
#define SOCP_REG_ENCDEST_BUFTHRH(n) (0x914 + (n) * 0x20)
#define SOCP_REG_ENCDEST_BUFTHRESHOLD(n) (0x918 + (n) * 0x20)
#define SOCP_REG_ENCDEST_BUFMODE_CFG(n) (0x91c + (n) * 0x20)

/* 版本寄存器 */
#define SOCP_REG_SOCP_VERSION 0xffc

/* 通道最大值定义 */
#define SOCP_MAX_ENCSRC_CHN 0x20
#define SOCP_MAX_ENCDST_CHN 0x08

#define SOCP_APP_ENC_TFRINT_MASK (1 << 15)    /* 编码core0 通道传输全局中断 */
#define SOCP_CORE0_ENC_MODESWT_MASK (1 << 6)  /* 编码core0 编码目的buffer模式切换完成 */
#define SOCP_APP_ENC_FLAGINT_MASK (1 << 14)   /* 编码core0 包头检测错误中断 */
#define SOCP_APP_ENC_OUTOVFINT_MASK (1 << 12) /* 编码core0 buffer溢出全局中断状态 */
#define SOCP_ENC_DST_BUFF_THRESHOLD_OVF_MASK 0x007f007f
#define SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN 16

#define IS_ENC_SRC_HEADER_ERROR_INT(state) ((state)&SOCP_APP_ENC_FLAGINT_MASK)
#define IS_ENC_DST_TFR_INT(state) ((state)&SOCP_APP_ENC_TFRINT_MASK)
#define IS_ENC_DST_OVERFLOW_INT(state) ((state)&SOCP_APP_ENC_OUTOVFINT_MASK)
#define IS_ENC_DST_MODE_SWT_INT(state) ((state)&SOCP_CORE0_ENC_MODESWT_MASK)
#define IS_ENC_DST_THRESHOLD_OVER_FLOW(state) ((state)&SOCP_ENC_DST_BUFF_THRESHOLD_OVF_MASK)
#define IS_IND_DST_THRD_OVF_RAW_INT(reg) (((reg) >> 16) & 2)

typedef struct {
    device_node_s *dev;
    u8 *base_addr;
    u32 socp_version;
} socp_hal_info_s;

#ifdef __cplusplus
}
#endif

#endif
