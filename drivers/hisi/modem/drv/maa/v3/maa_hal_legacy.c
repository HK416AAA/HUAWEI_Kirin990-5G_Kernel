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

#include "maa_def_legacy.h"
#include "maa_hal.h"

#define MAA_LOWPHY_ADDR(addr) ((addr)&0xffffffff)
#define MAA_HIGHPHY_ADDR(addr) (((addr) >> 32) & 0xff)

void __maa_release_pool_init(struct maa *maa)
{
    struct maa_fifo *fifo = &maa->release.fifo;
    writel(MAA_LOWPHY_ADDR(fifo->phy), maa->regs + MAA_RLS_POOL_ADDR_L);
    writel(MAA_HIGHPHY_ADDR(fifo->phy), maa->regs + MAA_RLS_POOL_ADDR_H);
    writel(fifo->depth - 1, maa->regs + MAA_RLS_POOL_DEPTH);
    fifo->uthrl = 1;
    writel(fifo->uthrl, maa->regs + MAA_RLS_POOL_UP_THRH);
    writel(0, maa->regs + MAA_RLS_POOL_WPTR);
    writel(0, maa->regs + MAA_RLS_POOL_RPTR);
}

void maa_ddr_buf_fifo_init(struct maa *maa, struct maa_fifo *fifo, unsigned int level) {}

void maa_fifo_init(struct maa *maa, struct maa_fifo *fifo, int level)
{
    writel(MAA_LOWPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_L(level));
    writel(MAA_HIGHPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_H(level));
    writel(fifo->depth - 1, maa->regs + MAA_OPIPE_DEPTH(level));
    writel(0, maa->regs + MAA_OPIPE_WPTR(level));
    writel(0, maa->regs + MAA_OPIPE_RPTR(level));
    writel(fifo->uthrl, maa->regs + MAA_OPIPE_UP_THRH(level));
    writel(fifo->dthrl, maa->regs + MAA_OPIPE_DN_THRH(level));
    writel(1, maa->regs + MAA_OPIPE_CFG_DONE(level));
}

void maa_force_fifo_full(struct maa *maa, struct maa_fifo *fifo, int level)
{
    writel_relaxed(fifo->depth - 5, maa->regs + MAA_OPIPE_WPTR(level)); // reserve 5 to prevent filling opipe
}

int maa_ipipe_init(struct maa *maa, struct maa_ipipe *ipipe, unsigned int idx)
{
    struct maa_fifo *fifo = &ipipe->fifo;
    writel((unsigned int)MAA_LOWPHY_ADDR(fifo->phy), (void *)(maa->regs + MAA_IPIPE_BASE_ADDR_L(idx)));
    writel((unsigned int)MAA_HIGHPHY_ADDR(fifo->phy), (void *)(maa->regs + MAA_IPIPE_BASE_ADDR_H(idx)));
    writel(fifo->depth - 1, (void *)(maa->regs + MAA_IPIPE_DEPTH(idx)));
    writel(fifo->uthrl, (void *)(maa->regs + MAA_IPIPE_UP_THRH(idx)));
    writel(fifo->dthrl, (void *)(maa->regs + MAA_IPIPE_DN_THRH(idx)));
    writel((unsigned int)MAA_LOWPHY_ADDR(fifo->update_write_addr), (void *)(maa->regs + MAA_IPIPE_PTR_ADDR_L(idx)));
    writel((unsigned int)MAA_HIGHPHY_ADDR(fifo->update_write_addr), (void *)(maa->regs + MAA_IPIPE_PTR_ADDR_H(idx)));
    writel(0, (void *)(maa->regs + MAA_IPIPE_WPTR(idx)));
    writel(0, (void *)(maa->regs + MAA_IPIPE_RPTR(idx)));
    writel(1, (void *)(maa->regs + MAA_IPIPE_CTRL(idx)));
    return 0;
}

void maa_interupt_init(struct maa *maa)
{
    maa->int_mask |= MAA_RLS_FULL_WR_INT_MASK | MAA_SAFE_CHK_INT_MASK | MAA_DDR_RLS_INT_MASK;
    writel_relaxed(maa->int_mask, maa->regs + MAA_OPIPE_INT_RAW);
    writel_relaxed(maa->int_mask, maa->regs + MAA_OPIPE_INT_MSK);
    writel_relaxed(0, maa->regs + MAA_IPIPE_INT_MSK0);
    return;
}

void maa_ipipe_read_update(struct maa *maa, unsigned int rptr, unsigned int idx)
{
    writel_relaxed(rptr, (void *)(maa->regs + MAA_IPIPE_RPTR(idx)));
}

void maa_input(struct maa *maa, unsigned long long buf, struct maa_opipe *opipe)
{
    writeq(buf, maa->regs + MAA_ALLOC_ADDR_L(opipe->level, 0));
}

unsigned int maa_hal_set_adq(struct maa *maa, unsigned int level, unsigned long long adqbase, unsigned int write,
    unsigned int depth, unsigned int idx)
{
    unsigned int dthrl = depth - 2 * MAA_BURST_LEN; // reserve 2 * MAA_BURST_LEN as dthrl
    unsigned int uthrl = depth - MAA_BURST_LEN; // reserve

    writel_relaxed((unsigned int)adqbase & 0xffffffffU, (void *)(maa->regs + MAA_IPIPE_BASE_ADDR_L(idx)));
    writel_relaxed((unsigned int)(adqbase >> 32) & 0xff, (void *)(maa->regs + MAA_IPIPE_BASE_ADDR_H(idx))); // 32 high bits
    writel_relaxed(depth - 1, (void *)(maa->regs + MAA_IPIPE_DEPTH(idx)));
    writel_relaxed(0, (void *)(maa->regs + MAA_IPIPE_WPTR(idx)));
    writel_relaxed(0, (void *)(maa->regs + MAA_IPIPE_RPTR(idx)));
    writel_relaxed(uthrl, (void *)(maa->regs + MAA_IPIPE_UP_THRH(idx)));
    writel_relaxed(dthrl, (void *)(maa->regs + MAA_IPIPE_DN_THRH(idx)));
    writel_relaxed(write, (void *)(maa->regs + MAA_IPIPE_PTR_ADDR_L(idx)));
    writel_relaxed(0, (void *)(maa->regs + MAA_IPIPE_PTR_ADDR_H(idx)));
    writel(1, (void *)(maa->regs + MAA_IPIPE_CTRL(idx)));
    return maa->phy_regs + MAA_IPIPE_RPTR(idx);
    return 0;
}

unsigned int maa_get_space(struct maa *maa, struct maa_opipe *opipe)
{
    return readl(maa->regs + MAA_OPIPE_SPACE(opipe->level)) - 0xf;
}

unsigned int maa_get_rls(struct maa *maa)
{
    return readl_relaxed(maa->regs + MAA_RLS_POOL_WPTR);
}

void maa_set_rls(struct maa *maa, unsigned int rptr)
{
    writel_relaxed(rptr, maa->regs + MAA_RLS_POOL_RPTR);
}

void maa_set_intr(struct maa *maa)
{
    writel_relaxed(maa->int_mask, maa->regs + MAA_OPIPE_INT_MSK);
}

void maa_top_intr(struct maa *maa)
{
    unsigned int status;
    unsigned int raw_value;
    unsigned int mask;

    mask = MAA_RLS_FULL_WR_INT_MASK | MAA_SAFE_CHK_INT_MASK;
    status = readl(maa->regs + MAA_OPIPE_INT_STAT);
    if (unlikely(status & MAA_SAFE_CHK_INT_MASK)) {
        /* clear raw */
        status &= ~MAA_SAFE_CHK_INT_MASK;
        raw_value = readl(maa->regs + MAA_OPIPE_INT_RAW);
        writel(raw_value | MAA_SAFE_CHK_INT_MASK, maa->regs + MAA_OPIPE_INT_RAW);
        maa->status.safe_check_err++;
    }
    if (unlikely(status & MAA_RLS_FULL_WR_INT_MASK)) {
        /* clear raw */
        status &= ~MAA_RLS_FULL_WR_INT_MASK;
        raw_value = readl(maa->regs + MAA_OPIPE_INT_RAW);
        writel(raw_value | MAA_RLS_FULL_WR_INT_MASK, maa->regs + MAA_OPIPE_INT_RAW);
        maa->status.rls_full++;
    }

    writel(mask, maa->regs + MAA_OPIPE_INT_MSK);
}

void maa_basic_cfg(struct maa *maa)
{
    writel(maa->fifo_timer_en, maa->regs + MAA_TIMER_EN);
}

int maa_hal_suspend(struct maa *maa)
{
    unsigned int i = 0;

    if (maa == NULL) {
        return 0;
    }
    /* close timer_en and turn down opipe down waterline */
    writel(0, maa->regs + MAA_TIMER_EN);
    for (i = 0; i < MAA_OPIPE_ALLOC_LEVELS; i++) {
        writel(maa->opipes[i].fifo.dthrl / 4, maa->regs + MAA_OPIPE_DN_THRH(i)); // turn down dthrl to 1 / 4
    }
    writel(MAA_RELEASE_THRSH_LP, maa->regs + MAA_RLS_POOL_UP_THRH);

    writel_relaxed(maa->int_mask, maa->regs + MAA_OPIPE_INT_MSK);

    return 0;
}

int maa_hal_resume(struct maa *maa)
{
    unsigned int i = 0;

    /* restore maa timer_en and opipe down waterline */
    writel(maa->fifo_timer_en, maa->regs + MAA_TIMER_EN);
    for (i = 0; i < MAA_OPIPE_ALLOC_LEVELS; i++) {
        writel(maa->opipes[i].fifo.dthrl, maa->regs + MAA_OPIPE_DN_THRH(i));
    }
    writel(maa->release.fifo.uthrl, maa->regs + MAA_RLS_POOL_UP_THRH);

    return 0;
}

int maa_hal_prepare(struct maa *maa)
{
    unsigned int i;
    unsigned int idx;

    if (maa == NULL) {
        return 0;
    }

    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        idx = maa->cpu_ipipes[i].idx;
        writel(0, maa->regs + MAA_IPIPE_CTRL(idx));
    }
    return 0;
}

void maa_hal_complete(struct maa *maa)
{
    unsigned int i;
    unsigned int idx;

    if (maa == NULL) {
        return;
    }

    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        idx = maa->cpu_ipipes[i].idx;
        writel(1, maa->regs + MAA_IPIPE_CTRL(idx));
    }
}

void maa_detach(struct maa *maa)
{
    int i, idx;

    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_SPE_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_SPE_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_SPE_2K));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_SPE_2K));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_IPF_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_IPF_512));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_L(MAA_IPIPE_FOR_IPF_2K));
    writel_relaxed(0, maa->regs + MAA_IPIPE_PTR_ADDR_H(MAA_IPIPE_FOR_IPF_2K));

    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        idx = maa->cpu_ipipes[i].idx;
        writel(0, maa->regs + MAA_IPIPE_CTRL(idx));
    }
}

void maa_fifo_reinit(struct maa *maa, struct maa_fifo *fifo, int level)
{
    writel_relaxed(MAA_LOWPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_L(level));
    writel_relaxed(MAA_HIGHPHY_ADDR(fifo->phy), maa->regs + MAA_OPIPE_BASE_ADDR_H(level));
    writel_relaxed(fifo->depth - 1, maa->regs + MAA_OPIPE_DEPTH(level));
    writel_relaxed(0, maa->regs + MAA_OPIPE_WPTR(level));
    writel_relaxed(0, maa->regs + MAA_OPIPE_RPTR(level));
    writel_relaxed(fifo->uthrl, maa->regs + MAA_OPIPE_UP_THRH(level));
    writel_relaxed(fifo->dthrl, maa->regs + MAA_OPIPE_DN_THRH(level));
    writel(1, maa->regs + MAA_OPIPE_CFG_DONE(level));
    fifo->read = 0;
    fifo->write = 0;
}

void maa_hds_transreport_hal(struct maa *maa, struct maa_debug_info *dbg_info)
{
    int level;
    unsigned long flags;

    spin_lock_irqsave(&maa->reset_lock, flags);
    if (maa->resetting) {
        spin_unlock_irqrestore(&maa->reset_lock, flags);
        return;
    }

    dbg_info->irq_mask = readl(maa->regs + MAA_OPIPE_INT_MSK);
    dbg_info->irq_raw = readl(maa->regs + MAA_OPIPE_INT_RAW);

    for (level = 0; level < MAA_OPIPE_LEVEL_INVALID; level++) {
        dbg_info->opipe_dbg[level].wptr = readl(maa->regs + MAA_OPIPE_WPTR(level));
        dbg_info->opipe_dbg[level].rptr = readl(maa->regs + MAA_OPIPE_RPTR(level));
    }
    spin_unlock_irqrestore(&maa->reset_lock, flags);
}
