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

#include "maa_hal.h"
#include "maa_def.h"
#include <bsp_print.h>

#define THIS_MODU mod_maa

#define maa_dbg_hal(fmt, ...)                             \
    do {                                                  \
        if (g_maa_ctx->dbg & MAA_DEBUG_MSG) {             \
            bsp_err("<%s>" fmt, __func__, ##__VA_ARGS__); \
        }                                                 \
    } while (0)

// 低功耗排空，送DDR，DDR满再送recycle
void __maa_release_pool_init(struct maa *maa)
{
    union maa_dbg_cnt_sel_u sel;
    struct maa_fifo *fifo = &maa->release.fifo;
    writel(fifo->phy & 0xffffffff, maa->regs + MAA_RECYCLE_POOL_BASE_ADDR_L(0));
    writel(fifo->phy >> 32, maa->regs + MAA_RECYCLE_POOL_BASE_ADDR_H(0)); // 32 high bits
    writel(fifo->depth - 1, maa->regs + MAA_RECYCLE_POOL_DEPTH(0));
    writel(fifo->uthrl, maa->regs + MAA_RECYCLE_POOL_UP_THRH(0));
    sel.val = 0;
    sel.bits.dbg_cnt_chn_sel = 0;
    sel.bits.dbg_cnt_cate_sel = MAA_DBG_CATE_SINGLE_RECYCLE;
    writel(sel.val, maa->regs + MAA_DBG_CNT_SEL(MAA_DBG_RECYCLE0));
}

void maa_ddr_buf_fifo_init(struct maa *maa, struct maa_fifo *fifo, unsigned int level)
{
    writel(fifo->phy & 0xffffffff, maa->regs + MAA_DDR_BUF_BASE_ADDR_L(level));
    writel(fifo->phy >> 32, maa->regs + MAA_DDR_BUF_BASE_ADDR_H(level)); // 32 high bits
    writel(fifo->depth - 1, maa->regs + MAA_DDR_BUF_DEPTH(level));
    writel(fifo->dthrl, maa->regs + MAA_DDR_BUF_DOWN_THRH(level));
}

void maa_fifo_init(struct maa *maa, struct maa_fifo *fifo, int level)
{
    union maa_rls_ch_ctrl_u ctrl;
    union maa_dbg_cnt_sel_u sel;

    writel(fifo->phy & 0xffffffff, maa->regs + MAA_RLS_CH_BASE_ADDR_L(level));
    writel(fifo->phy >> 32, maa->regs + MAA_RLS_CH_BASE_ADDR_H(level)); // 32 high bits
    writel(fifo->depth - 1, maa->regs + MAA_RLS_CH_DEPTH(level));
    writel(fifo->update_read_addr & 0xffffffff, maa->regs + MAA_RLS_CH_PTR_ADDR_L(level));
    writel((fifo->update_read_addr >> 32) & 0xffffffff, maa->regs + MAA_RLS_CH_PTR_ADDR_H(level)); // 32 high bits
    writel(fifo->dthrl, maa->regs + MAA_RLS_CH_THRH(level));
    ctrl.val = readl(maa->regs + MAA_RLS_CH_CTRL(level));
    ctrl.bits.rls_ch_des_sel = 0;
    ctrl.bits.rls_ch_level_sel = level;
    ctrl.bits.rls_ch_recycle_pool_sel = 0;
    ctrl.bits.rls_ch_big_release_pool_sel = 1;
    writel(ctrl.val, maa->regs + MAA_RLS_CH_CTRL(level));
    sel.val = 0;
    sel.bits.dbg_cnt_chn_sel = 0;
    sel.bits.dbg_cnt_cate_sel = MAA_DBG_CATE_SINGLE_RLS;
    writel(sel.val, maa->regs + MAA_DBG_CNT_SEL(MAA_DBG_BIGPOOL0 + level));
    writel(1, maa->regs + MAA_RLS_CH_EN(level));
}

void maa_force_fifo_full(struct maa *maa, struct maa_fifo *fifo, int level) {}

int maa_ipipe_init(struct maa *maa, struct maa_ipipe *ipipe, unsigned int idx)
{
    union maa_alloc_ch_ctrl_u ctrl;
    struct maa_fifo *fifo = &ipipe->fifo;
    writel(fifo->phy & 0xffffffff, maa->regs + MAA_ALLOC_CH_BASE_ADDR_L(idx));
    writel(fifo->phy >> 32, maa->regs + MAA_ALLOC_CH_BASE_ADDR_H(idx)); // 32 high bits
    writel(fifo->depth - 1, (void *)(maa->regs + MAA_ALLOC_CH_DEPTH(idx)));
    writel(fifo->update_write_addr & 0xffffffff, maa->regs + MAA_ALLOC_CH_PTR_ADDR_L(idx));
    writel((fifo->update_write_addr >> 32) & 0xffffffff, maa->regs + MAA_ALLOC_CH_PTR_ADDR_H(idx)); // 32 high bits
    ctrl.val = readl(maa->regs + MAA_ALLOC_CH_CTRL(idx));
    ctrl.bits.alloc_ch_level_sel = ipipe->level;
    ctrl.bits.alloc_ch_push_ptr_en = 1;
    writel(ctrl.val, maa->regs + MAA_ALLOC_CH_CTRL(idx));
    writel(1, (void *)(maa->regs + MAA_ALLOC_CH_EN(idx)));
    return 0;
}

void maa_interupt_init(struct maa *maa)
{
    bsp_err("maa->int_mask:%x", maa->int_mask);
    // 芯片是否支持burst，改两个writel,支持
    writel(maa->int_mask, (maa->regs + MAA_CORE0_BIG_POOL_INT_MASK(0)));
    writel(1, (maa->regs + MAA_CORE0_RECYCLE_POOL_INT_MASK));
    return;
}

void maa_ipipe_read_update(struct maa *maa, unsigned int rptr, unsigned int idx)
{
    writel(rptr, maa->regs + MAA_ALLOC_CH_RPTR(idx));
}

void maa_input(struct maa *maa, unsigned long long buf, struct maa_opipe *opipe)
{
    struct maa_fifo *fifo = &opipe->fifo;
    unsigned long long *base = (unsigned long long *)(uintptr_t)fifo->base;
    maa_dbg_hal("maa_input: %llx\n", buf);
    base[fifo->write] = buf;
    fifo->write = (fifo->write >= fifo->depth - 1) ? 0 : fifo->write + 1;
    writel(fifo->write, maa->regs + MAA_RLS_CH_WPTR(opipe->level));
    maa_dbg_hal("wptr %x\n", readl(maa->regs + MAA_RLS_CH_WPTR(opipe->level)));
    maa_dbg_hal("rptr %x\n", readl(maa->regs + MAA_RLS_CH_RPTR(opipe->level)));
}

unsigned int maa_hal_set_adq(struct maa *maa, unsigned int level, unsigned long long adqbase, unsigned int write,
    unsigned int depth, unsigned int idx)
{
    union maa_alloc_ch_ctrl_u ctrl;
    writel_relaxed((unsigned int)adqbase & 0xffffffffU, (void *)(maa->regs + MAA_ALLOC_CH_BASE_ADDR_L(idx)));
    writel_relaxed((unsigned int)(adqbase >> 32) & 0xff, (void *)(maa->regs + MAA_ALLOC_CH_BASE_ADDR_H(idx))); // 32 high bits
    writel_relaxed(depth - 1, (void *)(maa->regs + MAA_ALLOC_CH_DEPTH(idx)));
    writel(write & 0xffffffff, maa->regs + MAA_ALLOC_CH_PTR_ADDR_L(idx));
    writel(0, maa->regs + MAA_ALLOC_CH_PTR_ADDR_H(idx));
    maa_err("hw_idx:0x%x, base:0x%llx write:0x%x  depth:0x%x\n", idx, adqbase, write, depth);
    ctrl.val = readl(maa->regs + MAA_ALLOC_CH_CTRL(idx));
    ctrl.bits.alloc_ch_level_sel = level;
    ctrl.bits.alloc_ch_push_ptr_en = 1;
    writel(ctrl.val, maa->regs + MAA_ALLOC_CH_CTRL(idx));
    writel(1, (void *)(maa->regs + MAA_ALLOC_CH_EN(idx)));
    return maa->phy_regs + MAA_ALLOC_CH_RPTR(idx);
}

unsigned int maa_get_space(struct maa *maa, struct maa_opipe *opipe)
{
    unsigned int wptr, rptr;
    struct maa_fifo *fifo = &opipe->fifo;
    wptr = readl(maa->regs + MAA_RLS_CH_WPTR(opipe->level));
    rptr = readl(maa->regs + MAA_RLS_CH_RPTR(opipe->level));
    return (wptr >= rptr) ? fifo->depth - 1 + rptr - wptr : rptr - wptr - 1;
}

unsigned int maa_get_rls(struct maa *maa)
{
    return readl(maa->regs + MAA_RECYCLE_POOL_WPTR(0));
}

void maa_set_rls(struct maa *maa, unsigned int rptr)
{
    writel(rptr, maa->regs + MAA_RECYCLE_POOL_RPTR(0));
}

void maa_set_intr(struct maa *maa)
{
    writel(maa->int_mask, maa->regs + MAA_CORE0_BIG_POOL_INT_MASK(0));
    writel(1, maa->regs + MAA_CORE0_RECYCLE_POOL_INT_MASK);
}

void maa_top_intr(struct maa *maa)
{
    writel(0, maa->regs + MAA_CORE0_BIG_POOL_INT_MASK(0));
    writel(0, maa->regs + MAA_CORE0_RECYCLE_POOL_INT_MASK);
}

void maa_basic_cfg(struct maa *maa)
{
    writel(0xfff, maa->regs + MAA_AUTOCCLK);
    writel(1, maa->regs + MAA_DBG_CNT_CTRL);
}

int maa_hal_suspend(struct maa *maa)
{
    return 0;
}

int maa_hal_resume(struct maa *maa)
{
    return 0;
}

int maa_hal_prepare(struct maa *maa)
{
    return 0;
}

void maa_hal_complete(struct maa *maa) {}

void maa_detach(struct maa *maa) {}

void maa_fifo_reinit(struct maa *maa, struct maa_fifo *fifo, int level) {}

void maa_hds_transreport_hal(struct maa *maa, struct maa_debug_info *dbg_info) {}

int maa_hal_show_rls(unsigned int ch)
{
    struct maa *maa = g_maa_ctx;
    bsp_err("MAA_RLS_CH_BASE_ADDR_L:%#x\n", readl(maa->regs + MAA_RLS_CH_BASE_ADDR_L(ch)));
    bsp_err("MAA_RLS_CH_BASE_ADDR_H:%#x\n", readl(maa->regs + MAA_RLS_CH_BASE_ADDR_H(ch)));
    bsp_err("MAA_RLS_CH_DEPTH:%#x\n", readl(maa->regs + MAA_RLS_CH_DEPTH(ch)));
    bsp_err("MAA_RLS_CH_WPTR:%#x\n", readl(maa->regs + MAA_RLS_CH_WPTR(ch)));
    bsp_err("MAA_RLS_CH_RPTR:%#x\n", readl(maa->regs + MAA_RLS_CH_RPTR(ch)));
    bsp_err("MAA_RLS_CH_PTR_ADDR_L:%#x\n", readl(maa->regs + MAA_RLS_CH_PTR_ADDR_L(ch)));
    bsp_err("MAA_RLS_CH_PTR_ADDR_H:%#x\n", readl(maa->regs + MAA_RLS_CH_PTR_ADDR_H(ch)));
    bsp_err("MAA_RLS_CH_THRH:%#x\n", readl(maa->regs + MAA_RLS_CH_THRH(ch)));
    bsp_err("MAA_RLS_CH_CTRL:%#x\n", readl(maa->regs + MAA_RLS_CH_CTRL(ch)));
    bsp_err("MAA_RLS_CH_TIME_OUT_CFG:%#x\n", readl(maa->regs + MAA_RLS_CH_TIME_OUT_CFG(ch)));
    return 0;
}

int maa_hal_show_ddr(unsigned int ch)
{
    struct maa *maa = g_maa_ctx;
    bsp_err("MAA_DDR_BUF_BASE_ADDR_L:%#x\n", readl(maa->regs + MAA_DDR_BUF_BASE_ADDR_L(ch)));
    bsp_err("MAA_DDR_BUF_BASE_ADDR_H:%#x\n", readl(maa->regs + MAA_DDR_BUF_BASE_ADDR_H(ch)));
    bsp_err("MAA_DDR_BUF_DEPTH:%#x\n", readl(maa->regs + MAA_DDR_BUF_DEPTH(ch)));
    bsp_err("MAA_DDR_BUF_WPTR:%#x\n", readl(maa->regs + MAA_DDR_BUF_WPTR(ch)));
    bsp_err("MAA_DDR_BUF_RPTR:%#x\n", readl(maa->regs + MAA_DDR_BUF_RPTR(ch)));
    bsp_err("MAA_DDR_BUF_DOWN_THRH:%#x\n", readl(maa->regs + MAA_DDR_BUF_DOWN_THRH(ch)));
    bsp_err("MAA_DDR_BUF_WPTR_INTRL:%#x\n", readl(maa->regs + MAA_DDR_BUF_WPTR_INTRL(ch)));
    bsp_err("MAA_DDR_BUF_RPTR_INTRL:%#x\n", readl(maa->regs + MAA_DDR_BUF_RPTR_INTRL(ch)));
    return 0;
}

int maa_hal_show_allc(unsigned int ch)
{
    struct maa *maa = g_maa_ctx;
    bsp_err("MAA_ALLOC_CH_BASE_ADDR_L:%#x\n", readl(maa->regs + MAA_ALLOC_CH_BASE_ADDR_L(ch)));
    bsp_err("MAA_ALLOC_CH_BASE_ADDR_H:%#x\n", readl(maa->regs + MAA_ALLOC_CH_BASE_ADDR_H(ch)));
    bsp_err("MAA_ALLOC_CH_DEPTH:%#x\n", readl(maa->regs + MAA_ALLOC_CH_DEPTH(ch)));
    bsp_err("MAA_ALLOC_CH_WPTR:%#x\n", readl(maa->regs + MAA_ALLOC_CH_WPTR(ch)));
    bsp_err("MAA_ALLOC_CH_RPTR:%#x\n", readl(maa->regs + MAA_ALLOC_CH_RPTR(ch)));
    bsp_err("MAA_ALLOC_CH_PTR_ADDR_L:%#x\n", readl(maa->regs + MAA_ALLOC_CH_PTR_ADDR_L(ch)));
    bsp_err("MAA_ALLOC_CH_PTR_ADDR_H:%#x\n", readl(maa->regs + MAA_ALLOC_CH_PTR_ADDR_H(ch)));
    bsp_err("MAA_ALLOC_CH_CTRL:%#x\n", readl(maa->regs + MAA_ALLOC_CH_CTRL(ch)));
    bsp_err("MAA_ALLOC_CH_EN:%#x\n", readl(maa->regs + MAA_ALLOC_CH_EN(ch)));

    return 0;
}

int maa_hal_show_rec(unsigned int ch)
{
    struct maa *maa = g_maa_ctx;
    bsp_err("MAA_RECYCLE_POOL_BASE_ADDR_L:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_BASE_ADDR_L(ch)));
    bsp_err("MAA_RECYCLE_POOL_BASE_ADDR_H:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_BASE_ADDR_H(ch)));
    bsp_err("MAA_RECYCLE_POOL_DEPTH:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_DEPTH(ch)));
    bsp_err("MAA_RECYCLE_POOL_WPTR:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_WPTR(ch)));
    bsp_err("MAA_RECYCLE_POOL_RPTR:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_RPTR(ch)));
    bsp_err("MAA_RECYCLE_POOL_UP_THRH:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_UP_THRH(ch)));
    bsp_err("MAA_RECYCLE_POOL_WPTR_INTRL:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_WPTR_INTRL(ch)));

    return 0;
}

int maa_hal_show_intr(unsigned int ch)
{
    struct maa *maa = g_maa_ctx;
    bsp_err("MAA_CORE0_BIG_POOL_INT_MASK:%#x\n", readl(maa->regs + MAA_CORE0_BIG_POOL_INT_MASK(0)));
    bsp_err("MAA_CORE0_BIG_POOL_INT_MASK:%#x\n", readl(maa->regs + MAA_CORE0_BIG_POOL_INT_MASK(1)));
    bsp_err("MAA_CORE0_RECYCLE_POOL_INT_MASK:%#x\n", readl(maa->regs + MAA_CORE0_RECYCLE_POOL_INT_MASK));
    bsp_err("MAA_CORE1_BIG_POOL_INT_MASK:%#x\n", readl(maa->regs + MAA_CORE1_BIG_POOL_INT_MASK(0)));
    bsp_err("MAA_CORE1_BIG_POOL_INT_MASK:%#x\n", readl(maa->regs + MAA_CORE1_BIG_POOL_INT_MASK(1)));
    bsp_err("MAA_CORE1_RECYCLE_POOL_INT_MASK:%#x\n", readl(maa->regs + MAA_CORE1_RECYCLE_POOL_INT_MASK));
    bsp_err("MAA_BIG_POOL_INT_RAW:%#x\n", readl(maa->regs + MAA_BIG_POOL_INT_RAW(0)));
    bsp_err("MAA_BIG_POOL_INT_RAW:%#x\n", readl(maa->regs + MAA_BIG_POOL_INT_RAW(1)));
    bsp_err("MAA_RECYCLE_POOL_INT_RAW:%#x\n", readl(maa->regs + MAA_RECYCLE_POOL_INT_RAW));

    return 0;
}
