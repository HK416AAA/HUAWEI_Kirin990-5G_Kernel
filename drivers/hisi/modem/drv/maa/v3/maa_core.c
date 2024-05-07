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

#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <asm/atomic.h>
#include <linux/netdevice.h>
#include <linux/mm.h>
#include <linux/vmstat.h>
#include <linux/gfp.h>
#include <linux/timer.h>
#include <asm/memory.h>
#include <linux/bitops.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_aximem.h>
#include <bsp_slice.h>
#include <bsp_dump.h>
#include <bsp_om_enum.h>
#include "bsp_maa.h"
#include "maa.h"
#include "mdrv_errno.h"
#include <asm-generic/bug.h>
#include <osl_types.h>
#include <bsp_sysctrl.h>
#include <bsp_reset.h>
#include <mdrv_sysboot.h>
#include <mdrv_memory_layout.h>
#include <linux/syscore_ops.h>
#include <linux/pm_wakeup.h>
#include <linux/mm.h>
#include "maa_def.h"
#include "maa_hal.h"

#define MAA_MEM_LINE_DEAULT 100
#define MAA_CURRETN_AVAIL_MEM ((unsigned int)si_mem_available() << (PAGE_SHIFT - 10))
#define MAA_TIMER_EXPIRES (msecs_to_jiffies(1000))
#define MAA_TWO_BURST_RESERVE (2 * MAA_BURST_LEN)
#define MAA_RELEASE_OPIPE_DEPTH (4 * MAA_BURST_LEN)
#define MAA_RELEASE_OPIPE_UTHRL (MAA_BURST_LEN >> 1)
#define MAA_RELEASE_OPIPE_DTHRL (MAA_BURST_LEN >> 2)
#define MAA_HEAD_ROM_SIZE (sizeof(struct maa_head_rom))

#ifdef CONFIG_MAA_V2
#define MAA_OPIPE2_LP_IPIPE 16
#else
#define MAA_OPIPE2_LP_IPIPE 12
#endif
#define MAA_LP_IPIPE_LEN (MAA_BURST_LEN - 1)
#define MAA_IPIPE_WPTR_PHY_SIZE 500
#define MAA_LP_IPIPE_WPTR_PHY_SIZE 100
#define MAA_TIMER_AP_OPIPE_EN_VALUE 0x7
#define MAA_RECLAIM_MIN_DEPTH 0x1000
#define MAA_RECLAIM_MAX_DIFF 0x40
#define MAA_OPIPE_NAME_MAX 64
#define MAA_BYTE_ALIGN(addr) ALIGN(((u32)(addr)), MAA_SKB_RESERVE_SIZE)
struct maa *g_maa_ctx;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
extern unsigned long totalram_pages;
#endif

unsigned int g_maa_once_flag = 0;

#define maa_err_once(fmt, ...)           \
    do {                                 \
        if (!g_maa_once_flag) {          \
            bsp_err("Warning once \n");  \
            bsp_err(fmt, ##__VA_ARGS__); \
            g_maa_once_flag = 1;         \
        }                                \
    } while (0)

#define maa_dbg(fmt, ...)                                 \
    do {                                                  \
        if (maa->dbg & MAA_DEBUG_MSG) {                   \
            bsp_err("<%s>" fmt, __func__, ##__VA_ARGS__); \
        }                                                 \
    } while (0)

#define maa_bug_on()                                                 \
    do {                                                             \
        dump_stack();                                                \
        if (maa->dbg & MAA_DEBUG_SYSTEM_ERR) {                       \
            system_error(DRV_ERRNO_MAA_ADDR_CHECK_FAIL, 0, 0, 0, 0); \
        }                                                            \
        if (maa->dbg & MAA_DEBUG_BUGON) {                            \
            BUG_ON(1);                                               \
        }                                                            \
    } while (0)

static const int g_level_size[MAA_OPIPE_MAX] = {
    MAA_OPIPE_LEVEL_0_SIZE, MAA_OPIPE_LEVEL_1_SIZE, MAA_OPIPE_LEVEL_2_SIZE, MAA_OPIPE_LEVEL_3_SIZE,
    MAA_OPIPE_LEVEL_4_SIZE, MAA_OPIPE_LEVEL_5_SIZE, MAA_OPIPE_LEVEL_INVALID
};
int maa_mdmreset_cb(drv_reset_cb_moment_e eparam, int userdata);

void maa_set_debug(int enable)
{
    g_maa_ctx->dbg = enable;
}

void maa_set_intr_threhold(int threhold)
{
    g_maa_ctx->intr_threhold = threhold;
}

void maa_set_timer_out(unsigned int timeout)
{
    g_maa_ctx->timer_out = timeout;
}

static inline int maa_check_addr_crc(unsigned long long addr)
{
    unsigned int bitcnt;

    bitcnt = __sw_hweight64(addr);
    if (bitcnt & 1) {
        maa_err("crc check fail,maa 0x%llx\n", addr);
        return -1;
    }
    return 0;
}

static inline int maa_check_double_free(struct maa *maa, unsigned long long addr)
{
    struct maa_map *map = NULL;
    maa_buf_t buf;
    struct maa_tab *tab = NULL;
    unsigned int idx;
    unsigned int level;

    buf.addr = addr;
    idx = buf.bits.idx;
    level = buf.bits.level;

    if (unlikely(!maa_opipe_is_enable(maa, level))) {
        maa_err("invalid maa, maa 0x%llx\n", addr);
        maa->status.unmap_invalid_level++;
        maa_bug_on();
        return -EFAULT;
    }

    map = &maa->opipes[level].map;
    tab = &map->tab[idx];
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", buf.addr);
        maa_bug_on();

        return -EFAULT;
    }

    if (unlikely(tab->magic != MAA_OWN || tab->maa_addr != addr)) {
        maa_err("magic %x, maa 0x%llx, oldmaa 0x%llx, manager 0x%llx\n", tab->magic, addr, tab->maa_addr,
            (unsigned long long)(uintptr_t)tab->manager);
        maa_bug_on();

        return -EFAULT;
    }

    return 0;
}

static int maa_check_addr(struct maa *maa, unsigned long long addr)
{
    int err;
    err = maa_check_addr_crc(addr);
    if (err) {
        return err;
    }
    err = maa_check_double_free(maa, addr);
    return err;
}

void maa_list_measure(unsigned int level)
{
    struct maa_map *map = &g_maa_ctx->opipes[level].map;
    struct maa_opipe *opipes = &g_maa_ctx->opipes[level];
    struct list_head *pos = NULL;
    unsigned int cnt = 0;
    unsigned long flags;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->free_list)
    {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("free_list cnt %u\n", cnt);
    cnt = 0;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->busy_list)
    {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("busy_list cnt %u\n", cnt);
    cnt = 0;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->reclaim_list)
    {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("reclaim_list cnt %u\n", cnt);
}
void maa_show_opipe_status(unsigned int level)
{
    struct maa *maa = g_maa_ctx;
    unsigned int i = level;

    bsp_err("map_invalid_level %u\n", maa->status.map_invalid_level);
    bsp_err("unmap_invalid_level %u\n", maa->status.unmap_invalid_level);
    bsp_err("opipe%u:skb_alloc_success  %u\n", i, maa->opipe_status[level].skb_alloc_success);
    bsp_err("opipe%u:skb_alloc_atomic  %u\n", i, maa->opipe_status[level].skb_alloc_atomic);
    bsp_err("opipe%u:skb_alloc_kernel  %u\n", i, maa->opipe_status[level].skb_alloc_kernel);
    bsp_err("opipe%u:skb_own_free  %u\n", i, maa->opipe_status[level].skb_own_free);
    bsp_err("opipe%u:map_own_success  %u\n", i, maa->opipe_status[level].map_own_success);
    bsp_err("opipe%u:map_own_fail  %u\n", i, maa->opipe_status[level].map_own_fail);
    bsp_err("opipe%u:alloc_map_own_fail  %u\n", i, maa->opipe_status[level].alloc_map_own_fail);
    bsp_err("opipe%u:free_list_empty  %u\n", i, maa->opipe_status[level].free_list_empty);
    bsp_err("opipe%u:skb_alloc_fail  %u\n", i, maa->opipe_status[level].skb_alloc_fail);
    bsp_err("opipe%u:unmap_own  %u\n", i, maa->opipe_status[level].unmap_own);
    bsp_err("opipe%u:opipe:map_fail_skb_free  %u\n", i, maa->opipe_status[level].map_fail_skb_free);
    bsp_err("opipe%u:map_tcp_ip_ok  %u\n", i, maa->opipe_status[level].map_tcp_ip_ok);
    bsp_err("opipe%u:map_tcp_ip_fail  %u\n", i, maa->opipe_status[level].map_tcp_ip_fail);
    bsp_err("opipe%u:unmap_tcp_ip  %u\n", i, maa->opipe_status[level].unmap_tcp_ip);
    bsp_err("opipe%u:unmap_test_tcp_ip  %u\n", i, maa->opipe_status[level].unmap_test_tcp_ip);
    bsp_err("opipe%u:opipe_full  %u\n", i, maa->opipe_status[level].opipe_full);
    bsp_err("opipe%u:opipe_dn  %u\n", i, maa->opipe_status[level].opipe_dn);
    bsp_err("opipe%u:ipipe_up  %u\n", i, maa->opipe_status[level].ipipe_up);
    bsp_err("opipe%u:delay_free_cnt %u\n", i, maa->opipes[i].delay_free_cnt);
    bsp_err("opipe%u:reclaim_cnt %u\n", i, maa->opipes[i].reclaim_cnt);
    bsp_err("opipe%u:reclaim_depth %u\n", i, maa->opipes[i].reclaim_depth);
    bsp_err("opipe%u:reclaim_clear %u\n", i, maa->opipes[i].reclaim_clear);
    bsp_err("opipe%u:skb_reclaim_free %u\n", i, maa->opipe_status[level].skb_reclaim_free);
    bsp_err("opipe%u:skb_tcp_ip_free %u\n", i, maa->opipe_status[level].skb_tcp_ip_free);
    bsp_err("opipe%u:busy %u\n", i, maa->opipes[i].map.busy);
    bsp_err("opipe%u:busy_depth %u\n", i, maa->opipes[i].busy_depth);
    bsp_err("opipe%u:max_alloc_cnt %u\n", i, maa->opipes[i].max_alloc_cnt);
    bsp_err("opipe%u:min_alloc_cnt %u\n", i, maa->opipes[i].min_alloc_cnt);
    bsp_err("opipe%u:free_success %u\n", i, maa->opipe_status[level].free);
    bsp_err("refill_succ: %u\n", maa->status.refill_succ);
    bsp_err("sche_cnt: %u\n", maa->status.sche_cnt);
    bsp_err("reclaim_succ: %u\n", maa->status.reclaim_succ);
    bsp_err("tab size: %u\n", maa->opipes[level].map.tab_max_size);
    maa_list_measure(level);
}

void maa_show_ipipe_status(void)
{
    struct maa *maa = g_maa_ctx;
    unsigned int i;
    struct maa_ipipe *ipipe = NULL;

    if (maa == NULL) {
        return;
    }

    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        ipipe = maa->cpu_ipipes + i;
        bsp_err("ipipe%u ipipe_empty %u\n", ipipe->idx, ipipe->debug.ipipe_empty);
        bsp_err("ipipe%u alloc_success %u\n", ipipe->idx, ipipe->debug.alloc);
    }
}

void maa_help(void)
{
    bsp_err("maa_show_opipe_status \n");
    bsp_err("maa_show_ipipe_status \n");
    bsp_err("maa_show_push_cnt \n");
}

void maa_hds_transreport(void)
{
    int i;
    struct maa *maa = g_maa_ctx;
    struct maa_debug_info *dbg_info = &maa->hds_debug_info;

    if (maa == NULL || dbg_info == NULL) {
        return;
    }

    maa_hds_transreport_hal(maa, dbg_info);
    dbg_info->work_resetting = maa->status.work_resetting;
    dbg_info->work_start = maa->status.work_start;
    dbg_info->work_end = maa->status.work_end;
    dbg_info->refill_succ = maa->status.refill_succ;
    dbg_info->sche_cnt = maa->status.sche_cnt;
    dbg_info->mem_protect = maa->status.mem_protect;
    dbg_info->map_invalid_level = maa->status.map_invalid_level;
    dbg_info->unmap_invalid_level = maa->status.unmap_invalid_level;
    dbg_info->fifo_timer_en = maa->fifo_timer_en;

    for (i = 0; i < MAA_OPIPE_LEVEL_INVALID; i++) {
        dbg_info->opipe_dbg[i].skb_alloc_success = maa->opipe_status[i].skb_alloc_success;
        dbg_info->opipe_dbg[i].skb_own_free = maa->opipe_status[i].skb_own_free;
        dbg_info->opipe_dbg[i].map_own_success = maa->opipe_status[i].map_own_success;
        dbg_info->opipe_dbg[i].map_own_fail = maa->opipe_status[i].map_own_fail;
        dbg_info->opipe_dbg[i].alloc_map_own_fail = maa->opipe_status[i].alloc_map_own_fail;
        dbg_info->opipe_dbg[i].free_list_empty = maa->opipe_status[i].free_list_empty;
        dbg_info->opipe_dbg[i].skb_alloc_fail = maa->opipe_status[i].skb_alloc_fail;
        dbg_info->opipe_dbg[i].unmap_own = maa->opipe_status[i].unmap_own;
        dbg_info->opipe_dbg[i].map_fail_skb_free = maa->opipe_status[i].map_fail_skb_free;
        dbg_info->opipe_dbg[i].map_tcp_ip_ok = maa->opipe_status[i].map_tcp_ip_ok;
        dbg_info->opipe_dbg[i].map_tcp_ip_fail = maa->opipe_status[i].map_tcp_ip_fail;
        dbg_info->opipe_dbg[i].unmap_tcp_ip = maa->opipe_status[i].unmap_tcp_ip;
        dbg_info->opipe_dbg[i].unmap_test_tcp_ip = maa->opipe_status[i].unmap_test_tcp_ip;
        dbg_info->opipe_dbg[i].delay_free_cnt = maa->opipes[i].delay_free_cnt;
        dbg_info->opipe_dbg[i].reclaim_cnt = maa->opipes[i].reclaim_cnt;
        dbg_info->opipe_dbg[i].max_alloc_cnt = maa->opipes[i].max_alloc_cnt;
        dbg_info->opipe_dbg[i].min_alloc_cnt = maa->opipes[i].min_alloc_cnt;
        dbg_info->opipe_dbg[i].free = maa->opipe_status[i].free;
        dbg_info->opipe_dbg[i].skb_reclaim_free = maa->opipe_status[i].skb_reclaim_free;
        dbg_info->opipe_dbg[i].reclaim_depth = maa->opipes[i].reclaim_depth;
    }
}

void maa_trans_report_register(void)
{
    struct maa *maa = g_maa_ctx;
    struct trans_report_info *info = &maa->report_info;
    int ret;

    info->ul_msg_id = TRANS_MSGID_MAA;
    info->data = (void *)&maa->hds_debug_info;
    info->size = sizeof(maa->hds_debug_info);
    info->func_cb = maa_hds_transreport;
    INIT_LIST_HEAD(&info->list);

    ret = bsp_trans_report_register(info);
    if (ret) {
        maa_err("trans report register fail\n");
    }
}

int maa_sys_mem_protect(struct maa *maa)
{
    long avail;
    if (maa->protect_thresh) {
        avail = MAA_CURRETN_AVAIL_MEM;
        if (avail < maa->protect_thresh) {
            maa->status.mem_protect++;
            return 0;
        }
    }
    return 1;
}

unsigned int maa_get_ipipe_idx(struct maa *maa, enum maa_ipipe_idx ipipe_id)
{
    unsigned int i;
    for (i = 0; i < maa->allc_para_size; i++) {
        if (maa->allc_para[i].ipipe_id == ipipe_id) {
            return maa->allc_para[i].idx;
        }
    }
    bsp_err("maa_get_ipipe_idx failed\n");
    return 0;
}

unsigned int maa_get_ipipe_level(struct maa *maa, enum maa_ipipe_idx ipipe_id)
{
    unsigned int i;
    for (i = 0; i < maa->allc_para_size; i++) {
        if (maa->allc_para[i].ipipe_id == ipipe_id) {
            return maa->allc_para[i].level;
        }
    }
    bsp_err("maa_get_ipipe_level failed\n");
    return 0;
}

unsigned int maa_set_adqbase(struct maa *maa, unsigned long long adqbase, unsigned int write, unsigned int depth,
    unsigned int id)
{
    unsigned int ipipe;
    unsigned int level;

    ipipe = maa_get_ipipe_idx(maa, id);
    level = maa_get_ipipe_level(maa, id);

    if (level == 0) {
        return 0;
    }

    return maa_hal_set_adq(maa, level, adqbase, write, depth, ipipe);
}

static int maa_opipe2_lp_ipipe_init(struct maa *maa)
{
    unsigned int idx = MAA_OPIPE2_LP_IPIPE;
    struct maa_fifo *fifo = NULL;
    struct maa_ipipe maa_lp_ipipe;
    int ret;

    ret = (int)memset_s(&maa_lp_ipipe, sizeof(struct maa_ipipe), 0, sizeof(struct maa_ipipe));
    if (ret) {
        maa_err("memset_s fail\n");
    }

    fifo = &maa_lp_ipipe.fifo;

    fifo->depth = MAA_LP_IPIPE_LEN;

    fifo->uthrl = fifo->depth * 3 / 4; // set uthrl as 3 / 4 depth
    fifo->dthrl = fifo->depth / 2; // set dthrl as 1 / 2 depth
    fifo->update_write_addr = maa->ipipe_write_ptr_phy + MAA_IPIPE_WPTR_PHY_SIZE;
    fifo->phy = maa->ipipe_write_ptr_phy + MAA_IPIPE_WPTR_PHY_SIZE + MAA_LP_IPIPE_WPTR_PHY_SIZE;
    fifo->read = 0;
    fifo->write = 0;
    maa_ipipe_init(maa, &maa_lp_ipipe, idx);

    return 0;
}

static int maa_cpu_ipipe_config(struct maa *maa, unsigned int i)
{
    unsigned int idx;
    struct maa_fifo *fifo = NULL;
    idx = maa->para->iparamter.ipipeidx[i];
    if (idx == MAA_IPIPE_FOR_ACPU_FREE) {
        maa->free_ipipe = &maa->cpu_ipipes[i];
    }
    if (!maa->para->iparamter.depth[i]) {
        maa_err("ipipe %u depth is zero\n", idx);
        return -1;
    }
    spin_lock_init(&maa->cpu_ipipes[i].lock);
    maa->cpu_ipipes[i].idx = idx;
    maa->cpu_ipipes[i].level = MAA_OPIPE_LEVEL_1;
    if (MAA_OPIPE_LEVEL_INVALID != g_level_size[maa->cpu_ipipes[i].level]) {
        maa_err("ipipe %u,level %u,using\n", idx, maa->cpu_ipipes[i].level);
    }
    if (maa->cpu_using_ipipe[maa->cpu_ipipes[i].level] == maa->max_cpu_ipipes) {
        maa->cpu_using_ipipe[maa->cpu_ipipes[i].level] = i;
    }

    fifo = &maa->cpu_ipipes[i].fifo;
    fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev,
        maa->para->iparamter.depth[i] * sizeof(maa_buf_t), &fifo->phy, GFP_KERNEL);
    if (!fifo->base) {
        maa_err("ipipe %u base is zero\n", idx);
        return -1;
    }
    fifo->depth = maa->para->iparamter.depth[i];
    fifo->size = g_level_size[maa->cpu_ipipes[i].level];
    fifo->uthrl = fifo->depth * 3 / 4; // set uthrl as 3 / 4 depth
    fifo->dthrl = fifo->depth / 2; // set dthrl as 1 / 2 depth
    fifo->update_write_addr = maa->ipipe_write_ptr_phy + i * 4;
    fifo->update_write_virt = (unsigned long long)(uintptr_t)maa->ipipe_write_ptr_virt + i * 4;
    maa_ipipe_init(maa, &maa->cpu_ipipes[i], idx);
    return 0;
}

static int maa_cpu_ipipe_init(struct maa *maa)
{
    unsigned int i;

    if (!maa->para->iparamter.cnt) {
        maa_err("maa->para->iparamter.cnt = 0\n");
        return 0;
    }
    maa->max_cpu_ipipes = maa->para->iparamter.cnt;
    maa->cpu_ipipes = (struct maa_ipipe *)kzalloc(maa->max_cpu_ipipes * sizeof(struct maa_ipipe), GFP_KERNEL);
    if (maa->cpu_ipipes == NULL) {
        maa_err("cpu_ipipes alloc\n ");
        return 0;
    }
    maa->ipipe_write_ptr_virt = (unsigned int *)dma_alloc_coherent(maa->dev,
        MAA_IPIPE_WPTR_PHY_SIZE + MAA_LP_IPIPE_WPTR_PHY_SIZE + MAA_LP_IPIPE_LEN * sizeof(maa_buf_t),
        &maa->ipipe_write_ptr_phy, GFP_KERNEL);
    if (maa->ipipe_write_ptr_virt == NULL) {
        maa_err("g_maa_ipipe_write_ptr = 0\n ");
        goto fail1;
    }

    for (i = 0; i < MAA_OPIPE_ALLOC_LEVELS; i++) {
        maa->cpu_using_ipipe[i] = maa->max_cpu_ipipes;
    }
    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        maa_cpu_ipipe_config(maa, i);
    }

    return 0;
fail1:

    kfree(maa->cpu_ipipes);
    return 0;
}
unsigned long long maa_ipipe_alloc(unsigned int size)
{
    unsigned int level;
    unsigned long flags;
    unsigned int ipipeidx;
    unsigned int cpu_ipipe_idx;
    unsigned int write;
    maa_buf_t buf;
    struct maa_ipipe *ipipe = NULL;
    struct maa *maa = g_maa_ctx;

    level = maa_len_to_levels(size);
    if (level == MAA_OPIPE_LEVEL_INVALID) {
        return 0;
    }

    cpu_ipipe_idx = maa->cpu_using_ipipe[level];
    if (cpu_ipipe_idx >= maa->max_cpu_ipipes) {
        return 0;
    }

    ipipe = maa->cpu_ipipes + cpu_ipipe_idx;
    if (!ipipe->fifo.update_write_virt || !ipipe->fifo.base) {
        return 0;
    }

    ipipeidx = ipipe->idx;

    spin_lock_irqsave(&ipipe->lock, flags);
    write = *(unsigned int *)((uintptr_t)ipipe->fifo.update_write_virt);
    ipipe->fifo.write = write;

    if (ipipe->fifo.write == ipipe->fifo.read) {
        spin_unlock_irqrestore(&ipipe->lock, flags);
        ipipe->debug.ipipe_empty++;
        return 0;
    }
    buf = *((maa_buf_t *)(uintptr_t)ipipe->fifo.base + ipipe->fifo.read);
    ipipe->fifo.read++;
    ipipe->debug.alloc++;
    if (ipipe->fifo.read == ipipe->fifo.depth) {
        ipipe->fifo.read = 0;
    }

    maa_ipipe_read_update(maa, ipipe->fifo.read, ipipeidx);
    spin_unlock_irqrestore(&ipipe->lock, flags);

    return buf.addr;
}

int maa_freelist_is_empty(struct maa *maa, unsigned int level)
{
    struct maa_map *map = NULL;

    if (unlikely(!maa_opipe_is_enable(maa, level))) {
        maa_err("invalid level\n");
        maa->status.map_invalid_level++;
        return -1;
    }

    map = &maa->opipes[level].map;

    if (list_empty(&map->free_list)) {
        maa->opipe_status[level].free_list_empty++;
        return 1;
    }

    return 0;
}

unsigned long long maa_map_single(struct maa *maa, unsigned long long phy, void *manager, int from, unsigned int level)
{
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    maa_buf_t buf;
    unsigned long flags;
    unsigned int bitcnt;

    if (unlikely(!maa_opipe_is_enable(maa, level))) {
        maa_err("invalid level\n");
        maa->status.map_invalid_level++;
        return 0;
    }
    map = &maa->opipes[level].map;
    spin_lock_irqsave(&maa->opipes[level].lock, flags);
    if (unlikely(list_empty(&map->free_list))) {
        maa->opipe_status[level].free_list_empty++;
        spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
        return 0;
    }
    buf.addr = 0;
    tab = list_first_entry(&map->free_list, struct maa_tab, list);
    map->free--;
    tab->manager = manager;
    tab->magic = MAA_OWN;
    buf.bits.phyaddr = phy & MAA_PHY_ADDR_MASK;
    buf.bits.idx = tab->idx;
    buf.bits.from = from;
    buf.bits.level = level;
    buf.bits.crc = 0;

    bitcnt = __sw_hweight64(buf.addr);
    if (bitcnt & 1) {
        buf.bits.crc = 1;
    }
    tab->maa_addr = buf.addr;
    map->busy++;
    list_move_tail(&tab->list, &map->busy_list);
    maa->opipes[level].busy_depth++;
    spin_unlock_irqrestore(&maa->opipes[level].lock, flags);

    return buf.addr;
}

struct sk_buff *maa_unmap_tcpip_skb(struct maa *maa, maa_buf_t buf)
{
    struct maa_opipe *opipe = NULL;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    unsigned int level = buf.bits.level;
    unsigned int idx = buf.bits.idx;
    unsigned long flags = 0;

    opipe = &maa->opipes[level];
    map = &opipe->map;
    spin_lock_irqsave(&opipe->lock, flags);

    tab = &map->tab[idx];
    skb = (struct sk_buff *)tab->manager;
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (unsigned long long)((uintptr_t)skb) + 1;

    list_move_tail(&tab->list, &map->free_list);
    maa->opipes[level].busy_depth--;
    map->busy--;
    map->free++;
    map->curr = tab->idx;
    spin_unlock_irqrestore(&opipe->lock, flags);

    maa->opipe_status[level].unmap_tcp_ip++;
    if (map->cache) {
        if (skb->len) {
            dma_unmap_single(maa->dev, (dma_addr_t)virt_to_phys(skb->data), skb->len, DMA_TO_DEVICE);
        } else {
            maa_err("skb->len is zero\n");
        }
    }

    return skb;
}
struct sk_buff *__maa_unmap_tcpip_skb(struct maa *maa, maa_buf_t buf)
{
    struct maa_opipe *opipe = NULL;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    unsigned int level = buf.bits.level;
    unsigned int idx = buf.bits.idx;
    opipe = &maa->opipes[level];
    map = &opipe->map;
    tab = &map->tab[idx];
    skb = (struct sk_buff *)tab->manager;
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (unsigned long long)((uintptr_t)skb) + 1;
    list_move_tail(&tab->list, &map->free_list);
    maa->opipes[level].busy_depth--;
    map->busy--;
    map->free++;
    map->curr = tab->idx;
    maa->opipe_status[level].unmap_tcp_ip++;
    if (map->cache) {
        if (skb->len) {
            dma_unmap_single(maa->dev, (dma_addr_t)virt_to_phys(skb->data), skb->len, DMA_TO_DEVICE);
        } else {
            maa_err("skb->len is zero\n");
        }
    }
    return skb;
}

struct sk_buff * maa_unmap_tab(struct maa *maa, maa_buf_t buf, maa_buf_t *p)
{
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;

    map = &maa->opipes[buf.bits.level].map;
    tab = &map->tab[buf.bits.idx];

    if (unlikely(tab->magic != MAA_OWN)) {
        maa->opipe_status[buf.bits.level].unmap_own_fail++;
        WARN_ON_ONCE(1);
        maa_err_once("unmapaddr 0x%llx, keepaddr 0x%llx,level %u,idx %u\n",
            buf.addr, tab->maa_addr, buf.bits.level, buf.bits.idx);
        return NULL;
    }

    skb = (struct sk_buff *)tab->manager;
    p->addr = tab->maa_addr;
    tab->manager = (void *)(uintptr_t)(buf.addr + 1);
    tab->magic = ~MAA_OWN;
    tab->maa_addr = (uintptr_t)skb + 1;
    list_move_tail(&tab->list, &map->free_list);
    map->busy--;
    map->free++;
    map->curr = tab->idx;

    return skb;
}


static struct sk_buff *__maa_unmap_skb(struct maa *maa, maa_buf_t buf)
{
    struct maa_map *map = NULL;
    struct sk_buff *skb = NULL;
    struct maa_head_rom *rom = NULL;
    maa_buf_t true_addr;

    if (unlikely(!maa_opipe_is_enable(maa, buf.bits.level))) {
        maa_err("invalid  level maa 0x%llx\n", buf.addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }
    map = &maa->opipes[buf.bits.level].map;
    if (unlikely(buf.bits.idx < MAA_TABEL_START || buf.bits.idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", buf.addr);
        maa_bug_on();
        return NULL;
    }

    skb = maa_unmap_tab(maa, buf, &true_addr);
    if (skb == NULL) {
        maa_bug_on();
        return NULL;
    }

    if (true_addr.bits.from && map->cache) {
        dma_unmap_single(g_maa_ctx->dev, (dma_addr_t)true_addr.bits.phyaddr - MAA_SKB_RESERVE_SIZE, MAA_SKB_RESERVE_SIZE,
            DMA_FROM_DEVICE);
        rom = (struct maa_head_rom *)phys_to_virt(true_addr.bits.phyaddr - MAA_SKB_RESERVE_SIZE);
        rom->check = ~MAA_OWN;
        rom->dcheck = ~MAA_OWN;
        rom->maa_addr = 0;
        (void)dma_map_single(maa->dev, rom, MAA_SKB_RESERVE_SIZE, DMA_TO_DEVICE);
        if (rom->check == MAA_OWN) {
            maa_bug_on();
        }
        maa->opipe_status[true_addr.bits.level].unmap_cache_flush++;
    }
    maa->opipe_status[true_addr.bits.level].unmap_own++;
    return skb;
}

struct sk_buff *maa_unmap_skb(struct maa *maa, maa_buf_t buf, unsigned long long *orig)
{
    struct maa_opipe *opipe = NULL;
    struct maa_map *map = NULL;
    struct sk_buff *skb = NULL;
    struct maa_head_rom *rom = NULL;
    unsigned long flags;
    maa_buf_t true_addr;

    if (unlikely(!maa_opipe_is_enable(maa, buf.bits.level))) {
        maa_err("invalid  level maa 0x%llx\n", buf.addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }
    opipe = &maa->opipes[buf.bits.level];
    map = &opipe->map;
    if (unlikely(buf.bits.idx < MAA_TABEL_START || buf.bits.idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", buf.addr);
        maa_bug_on();
        return NULL;
    }

    spin_lock_irqsave(&opipe->lock, flags);
    skb = maa_unmap_tab(maa, buf, &true_addr);
    spin_unlock_irqrestore(&opipe->lock, flags);
    if (skb == NULL) {
        maa_bug_on();
        return NULL;
    }

    if (orig) {
        *orig = true_addr.addr;
    } else {
        if (true_addr.addr != buf.addr) {
            maa_bug_on();
            return NULL;
        }
    }

    if (true_addr.bits.from && map->cache) {
        dma_unmap_single(g_maa_ctx->dev, true_addr.bits.phyaddr - MAA_SKB_RESERVE_SIZE, MAA_SKB_RESERVE_SIZE,
            DMA_FROM_DEVICE);
        rom = (struct maa_head_rom *)phys_to_virt(true_addr.bits.phyaddr - MAA_SKB_RESERVE_SIZE);
        rom->check = ~MAA_OWN;
        rom->dcheck = ~MAA_OWN;
        rom->maa_addr = 0;
        (void)dma_map_single(maa->dev, rom, MAA_SKB_RESERVE_SIZE, DMA_TO_DEVICE);
        if (rom->check == MAA_OWN) {
            maa_bug_on();
        }
        maa->opipe_status[buf.bits.level].unmap_cache_flush++;
    }
    maa->opipe_status[buf.bits.level].unmap_own++;
    if (orig) {
        *orig = true_addr.addr;
    }

    return skb;
}

int maa_mem_check(struct maa *maa)
{
    unsigned long cnt;
    cnt = global_zone_page_state(NR_FREE_PAGES);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    return cnt > (totalram_pages() / maa->mem_line);
#else
    return cnt > (totalram_pages / maa->mem_line);
#endif
}

unsigned long maa_alloc_page(struct maa *maa, struct maa_opipe *opipe, maa_buf_t *p_addr, unsigned int level)
{
    unsigned long page;
    u64 phy;
    u64 addr;

    if (maa_freelist_is_empty(maa, level)) {
        maa->opipe_status[level].map_own_fail++;
        return 0;
    }

    page = __get_free_page(GFP_KERNEL);
    if (unlikely(!page)) {
        maa->opipe_status[level].skb_alloc_fail++;
        maa->need_schedule |= BIT(level);
        return 0;
    }
    maa->opipe_status[level].skb_alloc_success++;

    phy = virt_to_phys((void *)(uintptr_t)page);
    addr = maa_map_single(maa, phy, (void *)(uintptr_t)page, MAA_SKB_FROM_OWN, level);
    p_addr->addr = addr;
    if (unlikely(!addr)) {
        free_page(page);
        maa->opipe_status[level].map_own_fail++;
        maa->opipe_status[level].map_fail_skb_free++;
        return 0;
    }
    if (opipe->map.cache) {
        (void)dma_map_single(maa->dev, (void *)(uintptr_t)page, PAGE_SIZE, DMA_FROM_DEVICE);
    }
    maa->opipe_status[level].map_own_success++;
    return page;
}

struct sk_buff *maa_alloc_skb(struct maa *maa, struct maa_opipe *opipe, maa_buf_t *p_addr, gfp_t gfp)
{
    struct sk_buff *skb = NULL;
    struct maa_head_rom *rom = NULL;
    u64 addr;
    u64 phy;
    unsigned int len = g_level_size[opipe->level];
    unsigned int level = opipe->level;

    if (maa_freelist_is_empty(maa, level)) {
        maa->opipe_status[level].map_own_fail++;
        return 0;
    }

    skb = alloc_skb(len + MAA_SKB_RESERVE_SIZE, gfp);
    if (unlikely(skb == NULL)) {
        maa->opipe_status[level].skb_alloc_fail++;
        maa_err("skb == NULL\n");
        return NULL;
    }

    maa->opipe_status[level].skb_alloc_success++;
    if (gfp == GFP_ATOMIC) {
        maa->opipe_status[level].skb_alloc_atomic++;
    }
    if (gfp == GFP_KERNEL) {
        maa->opipe_status[level].skb_alloc_kernel++;
    }
    phy = virt_to_phys(skb->data);
    addr = maa_map_single(maa, phy + MAA_SKB_RESERVE_SIZE, skb, MAA_SKB_FROM_OWN, level);
    p_addr->addr = addr;
    if (unlikely(!addr)) {
        dev_kfree_skb_any(skb);
        maa->opipe_status[level].alloc_map_own_fail++;
        maa->opipe_status[level].map_fail_skb_free++;
        maa_err("maa_map_single failed\n");
        return NULL;
    }
    rom = (struct maa_head_rom *)(skb->data);
    rom->maa_addr = addr;
    rom->dcheck = MAA_OWN;
    rom->check = MAA_OWN;
    if (opipe->map.cache) {
        (void)dma_map_single(maa->dev, skb->data, MAA_SKB_RESERVE_SIZE, DMA_TO_DEVICE);
        (void)dma_map_single(maa->dev, skb->data, len + MAA_SKB_RESERVE_SIZE, DMA_FROM_DEVICE);
    }
    skb_reserve(skb, MAA_SKB_RESERVE_SIZE);
    maa->opipe_status[level].map_own_success++;
    return skb;
}

unsigned long long maa_alloc_reserve_mem(struct maa *maa, struct maa_opipe *opipe, unsigned int size,
    unsigned int level)
{
    unsigned long long addr;
    unsigned long flags = 0;

    if (maa_freelist_is_empty(maa, level)) {
        maa->opipe_status[level].map_own_fail++;
        return 0;
    }

    spin_lock_irqsave(&maa->reserve_mem.lock, flags);
    maa->reserve_mem.cur = MAA_BYTE_ALIGN(maa->reserve_mem.cur);
    if (maa->reserve_mem.cur + size > maa->reserve_mem.end) {
        maa->opipe_status[level].skb_alloc_fail++;
        spin_unlock_irqrestore(&maa->reserve_mem.lock, flags);
        return 0;
    }
    addr = (unsigned long long)maa->reserve_mem.cur;
    maa->reserve_mem.cur += size;
    spin_unlock_irqrestore(&maa->reserve_mem.lock, flags);
    maa->opipe_status[level].skb_alloc_success++;
    addr = maa_map_single(maa, addr, (void *)(uintptr_t)addr, MAA_SKB_FROM_OWN, level);
    if (unlikely(!addr)) {
        maa->opipe_status[level].alloc_map_own_fail++;
        maa->opipe_status[level].map_fail_skb_free++;
        return 0;
    }
    maa->opipe_status[level].map_own_success++;
    return addr;
}

int maa_free_buf(struct maa *maa, maa_buf_t buf)
{
    int err;
    unsigned int level;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct sk_buff *skb = NULL;
    unsigned long flags;
    err = maa_check_addr(maa, buf.addr);
    if (err) {
        maa_err("maa check add error\n");
        maa_bug_on();
        return err;
    }
    level = buf.bits.level;
    map = &maa->opipes[level].map;
    tab = &map->tab[buf.bits.idx];
    if (buf.bits.from == MAA_SKB_FROM_OWN) {
        spin_lock_irqsave(&maa->opipes[level].lock, flags);
        list_move_tail(&tab->list, &map->reclaim_list);
        maa->opipes[level].busy_depth--;
        maa->opipes[level].reclaim_depth++;
        spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
        maa->opipes[level].delay_free_cnt++;
    } else if (buf.bits.from == MAA_SKB_FROM_TCP_IP) {
        skb = maa_unmap_tcpip_skb(maa, buf);
        if (skb != NULL) {
            dev_kfree_skb_any(skb);
            maa->opipe_status[level].skb_tcp_ip_free++;
        }
    } else {
        maa_err("maa check add error, 0x%llx\n", buf.addr);
    }
    return err;
}

void maa_ipipe_free(struct maa *maa, unsigned long long ptr)
{
    maa_buf_t buf;
    buf.addr = ptr;
    maa_free_buf(maa, buf);
}

static int maa_fill_from_alloc(struct maa *maa, struct maa_opipe *opipe, struct maa_fifo *fifo)
{
    maa_buf_t mbuf;
    unsigned long page;
    struct sk_buff *skb = NULL;
    if (opipe->from == MAA_CP_MEM) {
        mbuf.addr = maa_alloc_reserve_mem(maa, opipe, fifo->size, opipe->level);
        if (mbuf.addr == 0) {
            return -1;
        }
    } else {
        if (!maa_sys_mem_protect(maa)) {
            return -1;
        }
        if (MAA_OPIPE_LEVEL_2 != opipe->level) {
            skb = maa_alloc_skb(maa, opipe, &mbuf, GFP_KERNEL);
            if (unlikely(skb == NULL)) {
                return -1;
            }
        } else {
            page = maa_alloc_page(maa, opipe, &mbuf, opipe->level);
            if (unlikely(!page)) {
                return -1;
            }
        }
    }
    maa_input(maa, mbuf.addr, opipe);
    return 0;
}

unsigned int maa_fill_fifo(struct maa *maa, struct maa_fifo *fifo, unsigned int cnt)
{
    unsigned long flag;
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    unsigned int i = 0;
    struct maa_opipe *opipe = container_of(fifo, struct maa_opipe, fifo);
    map = &opipe->map;

    if (!list_empty(&map->reclaim_list)) {
        spin_lock_irqsave(&opipe->lock, flag);
        for (; i < cnt; i++) {
            if (!list_empty(&map->reclaim_list)) {
                tab = list_first_entry(&map->reclaim_list, struct maa_tab, list);
                list_move_tail(&tab->list, &map->busy_list);
                opipe->busy_depth++;
                opipe->reclaim_depth--;
                maa_input(maa, tab->maa_addr, opipe);
                opipe->reclaim_cnt++;
            } else {
                break;
            }
        }
        spin_unlock_irqrestore(&opipe->lock, flag);
    }
    for (; i < cnt; i++) {
        if(!maa_fill_from_alloc(maa, opipe, fifo)) {
            break;
        }
    }

    return !(i == cnt);
}

extern void maa_release_func(struct maa *maa);

void maa_clear_reclaim(struct maa *maa, unsigned int level)
{
    struct maa_map *map = NULL;
    struct maa_tab *tab = NULL;
    struct maa_tab *tmp = NULL;
    struct maa_opipe *opipe = NULL;
    struct sk_buff *skb = NULL;

    opipe = &maa->opipes[level];
    map = &opipe->map;

    if (opipe->reclaim_depth > MAA_RECLAIM_MIN_DEPTH) {
        list_for_each_entry_safe(tab, tmp, &map->reclaim_list, list)
        {
            skb = __maa_unmap_skb(maa, (maa_buf_t)tab->maa_addr);
            if (skb != NULL) {
                dev_kfree_skb_any(skb);
                maa->opipe_status[level].skb_reclaim_free++;
            }
            opipe->reclaim_depth--;
            if (opipe->reclaim_depth <= MAA_RECLAIM_MIN_DEPTH) {
                break;
            }
        }
        opipe->reclaim_clear++;
    }
}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
void maa_bigpool_timer(struct timer_list *t)
{
    struct maa *maa = from_timer(maa, t, timer);
#else
void maa_bigpool_timer(unsigned long data)
{
    struct maa *maa = (struct maa *)(uintptr_t)data;
#endif
    int i;
    unsigned long flags;
    unsigned int diff;

    spin_lock_irqsave(&maa->reset_lock, flags);
    if (maa->resetting) {
        spin_unlock_irqrestore(&maa->reset_lock, flags);
        mod_timer(&maa->timer, jiffies + MAA_TIMER_EXPIRES);
        maa->timer_cnt++;
        maa_err("maa is resetting\n");
        return;
    }
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    for (i = 0; i < MAA_OPIPE_LEVEL_2; i++) {
        spin_lock_irqsave(&maa->opipes[i].lock, flags);
        if (maa->opipes[i].reclaim_depth >= maa->opipes[i].last_reclaim_depth) {
            diff = maa->opipes[i].reclaim_depth - maa->opipes[i].last_reclaim_depth;
            if (diff < MAA_RECLAIM_MAX_DIFF && (maa->dbg & MAA_DEBUG_TIMER_EN)) {
                maa_clear_reclaim(maa, i);
            }
        }
        maa->opipes[i].last_reclaim_depth = maa->opipes[i].reclaim_depth;
        spin_unlock_irqrestore(&maa->opipes[i].lock, flags);
    }

    mod_timer(&maa->timer, jiffies + MAA_TIMER_EXPIRES);
    maa->timer_cnt++;

    return;
}

int maa_reclaim_fullfill(struct maa *maa)
{
    int ret = 0;

    if (maa->fullfill_count) {
        ret = maa_fill_fifo(maa, &maa->opipes[maa->fullfill_idx].fifo, maa->fullfill_count);
        bsp_err("fullfill:%d\n", ret);
    }

    maa_release_func(maa);

    return ret;
}

struct sk_buff *_maa_to_skb(struct maa *maa, unsigned long long addr)
{
    struct sk_buff *skb = NULL;
    struct maa_map *map = NULL;
    maa_buf_t buf;
    struct maa_tab *tab = NULL;
    unsigned int idx;
    unsigned int level;

    buf.addr = addr;
    idx = buf.bits.idx;
    level = buf.bits.level;

    if (unlikely(!maa_opipe_is_enable(maa, level))) {
        maa_err("invalid level maa 0x%llx\n", addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }

    map = &maa->opipes[level].map;
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa_err("invalid idx maa 0x%llx\n", addr);
        return NULL;
    }

    tab = &map->tab[idx];

    if (unlikely(tab->magic != MAA_OWN || tab->maa_addr != addr)) {
        maa_err("magic %x, addr 0x%llx, oldaddr 0x%llx, skb 0x%llx\n", tab->magic, addr, tab->maa_addr,
            (unsigned long long)(uintptr_t)tab->manager);
        return NULL;
    }
    skb = (struct sk_buff *)tab->manager;

    return skb;
}

void __maa_release_func(struct maa *maa, struct maa_release_poll *release, unsigned int cnt)
{
    maa_buf_t buf;
    unsigned int i;

    for (i = 0; i < cnt; i++) {
        buf = release->local_release[i];
        maa_free_buf(maa, buf);
    }
}

static inline unsigned int maa_get_release_busy_num(unsigned int w, unsigned r, unsigned int depth)
{
    return ((w >= r) ? (w - r) : (depth - (r - w)));
}

void maa_release_func(struct maa *maa)
{
    struct maa_release_poll *release = &maa->release;
    maa_buf_t *base = (maa_buf_t *)(uintptr_t)release->fifo.base;
    unsigned int max_free_cnt = 0;
    int ret;
    unsigned int rsl_busy_cnt;
    unsigned int cnt = 0;
    unsigned int tail_num;

    release->fifo.write = maa_get_rls(maa);

    while (release->fifo.write != release->fifo.read) {
        rsl_busy_cnt = maa_get_release_busy_num(release->fifo.write, release->fifo.read, release->fifo.depth);
        if (rsl_busy_cnt > MAA_BURST_LEN) {
            cnt = MAA_BURST_LEN;
        } else {
            cnt = rsl_busy_cnt;
        }
        if (release->fifo.read + cnt <= release->fifo.depth) {
            ret = memcpy_s(release->local_release, sizeof(release->local_release), base + release->fifo.read,
                cnt * sizeof(maa_buf_t));
        } else {
            /* copy two space */
            tail_num = release->fifo.depth - release->fifo.read;
            ret = memcpy_s(release->local_release, sizeof(release->local_release), base + release->fifo.read,
                tail_num * sizeof(maa_buf_t));
            ret = memcpy_s(release->local_release + tail_num,
                sizeof(release->local_release) - tail_num * sizeof(maa_buf_t), base,
                (cnt - tail_num) * sizeof(maa_buf_t));
        }
        if (ret) {
            release->mcp_fail++;
        }
        max_free_cnt += cnt;
        __maa_release_func(maa, release, cnt);
        release->fifo.read += cnt;
        if (release->fifo.read >= release->fifo.depth) {
            release->fifo.read -= release->fifo.depth;
        }
    }
    if (release->max_free_cnt < max_free_cnt) {
        release->max_free_cnt = max_free_cnt;
    }
    if (release->min_free_cnt > max_free_cnt) {
        release->min_free_cnt = max_free_cnt;
    }

    maa_set_rls(maa, release->fifo.read);
}

irqreturn_t maa_interrupt(int irq, void *dev_id)
{
    struct maa *maa = (struct maa *)dev_id;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        __pm_stay_awake(maa->wake_lock);
#else
        __pm_stay_awake(&(maa->wake_lock));
#endif
    maa_top_intr(maa);

    queue_work(g_maa_ctx->workqueue, &g_maa_ctx->fill_work);

    return IRQ_HANDLED;
}

void maa_refill_bigpool_work(struct work_struct *work)
{
    unsigned int i;
    unsigned int total;
    struct maa *maa = NULL;
    unsigned long flags = 0;
    int ret = 0;

    maa = container_of(work, struct maa, fill_work);
    maa->status.work_start = bsp_get_slice_value();
    spin_lock_irqsave(&maa->reset_lock, flags);
    if (maa->resetting) {
        maa->status.work_resetting++;
        maa->status.work_end = bsp_get_slice_value();
        spin_unlock_irqrestore(&maa->reset_lock, flags);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        __pm_relax(maa->wake_lock);
#else
        __pm_relax(&(maa->wake_lock));
#endif
        maa_err("maa is resetting\n");
        return;
    }
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    maa_release_func(maa);

    for (i = 0; i <= maa->max_opipes; i++) {
        if (!maa_opipe_is_enable(maa, i)) {
            continue;
        }
        total = maa_get_space(maa, &maa->opipes[i]);
        if (total) {
            ret |= maa_fill_fifo(maa, &maa->opipes[i].fifo, total);
        }
    }

    if (ret) {
        queue_work(g_maa_ctx->workqueue, &g_maa_ctx->fill_work);
        maa->status.sche_cnt++;
    } else {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        __pm_relax(maa->wake_lock);
#else
        __pm_relax(&(maa->wake_lock));
#endif
        maa_set_intr(maa); // maa_enable_intr
        maa->status.refill_succ++;
    }
    maa->status.work_end = bsp_get_slice_value();
}

static unsigned int maa_set_dthrl(struct maa *maa, int level)
{
    struct maa_fifo *fifo = &maa->opipes[level].fifo;

    if (maa->para->oparamter.dthr_config) {
        return maa->para->oparamter.dthr[level];
    }
    switch (level) {
        case MAA_OPIPE_LEVEL_0:
        case MAA_OPIPE_LEVEL_1:
            return 3 * (fifo->depth >> 2); // dthrl set to 3 / 4 depth
        case MAA_OPIPE_LEVEL_4:
        case MAA_OPIPE_LEVEL_5:
            return fifo->depth >> 2; // dthrl set to 1 / 2 depth
        default:
            return fifo->depth - MAA_TWO_BURST_RESERVE - 1;
    }
}

int maa_ddr_buf_init(struct maa *maa, unsigned int level)
{
    struct maa_fifo *fifo = &maa->ddr_buf[level].fifo;

    fifo->depth = maa->para->ddr_buf_depth[level];
    if (fifo->depth == 0) {
        return 0;
    }
    fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev, fifo->depth * sizeof(maa_buf_t),
        &fifo->phy, GFP_KERNEL);
    if (!fifo->base) {
        maa_err("no ddr buf alloc\n");
        return -1;
    }
    fifo->dthrl = fifo->depth - MAA_BURST_LEN - 1;

    maa_ddr_buf_fifo_init(maa, fifo, level);

    return 0;
}

int maa_create_tab(struct maa *maa, struct maa_map *map)
{
    unsigned int i;
    struct maa_tab *tmp = NULL;
    map->tab = (struct maa_tab *)kzalloc(sizeof(struct maa_tab) * map->tab_max_size, GFP_KERNEL);
    if (map->tab == NULL) {
        maa_err("no map tab alloc\n");
        return -ENOMEM;
    }

    for (i = MAA_TABEL_START; i < map->tab_max_size; i++) {
        tmp = map->tab + i;
        tmp->idx = i;
        map->total++;
        map->free++;
        list_add_tail(&tmp->list, &map->free_list);
    }

    return 0;
}

int maa_opipe_init(struct maa *maa, int level)
{
    int ret;
    struct maa_map *map = &maa->opipes[level].map;
    struct maa_fifo *fifo = &maa->opipes[level].fifo;
    INIT_LIST_HEAD(&map->free_list);
    INIT_LIST_HEAD(&map->busy_list);
    INIT_LIST_HEAD(&map->reclaim_list);
    spin_lock_init(&maa->opipes[level].lock);

    map->free = 0;
    map->busy = 0;
    map->total = 0;
    fifo->depth = maa->para->oparamter.depth[level];
    maa->opipes[level].from = maa->para->oparamter.from[level];
    maa->opipes[level].level = level;

    map->tab_max_size = maa->para->oparamter.tab_size[level];
    ret = maa_create_tab(maa, map);
    if (ret) {
        return ret;
    }
    map->cache = 1;

    fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev, fifo->depth * sizeof(maa_buf_t),
        &fifo->phy, GFP_KERNEL);
    if (!fifo->base) {
        maa_err("dma_alloc_coherent failed\n");
        goto no_mem;
    }
    fifo->update_read_virt = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev, sizeof(unsigned int),
        &fifo->update_read_addr, GFP_KERNEL);
    if (!fifo->update_read_virt) {
        maa_err("dma_alloc_coherent failed\n");
        goto no_mem;
    }
    fifo->size = g_level_size[level];
    fifo->uthrl = fifo->depth - MAA_BURST_LEN - 1;
    fifo->dthrl = maa_set_dthrl(maa, level);
    fifo->free = fifo->depth - fifo->dthrl;
    maa_fifo_init(maa, fifo, level); // hal

    if (!maa->para->oparamter.disable_irq[level]) {
        maa->int_mask |= 1 << (unsigned int)level;
    }
    maa->opipes[level].status = MAA_OPIPE_STATUS_INIT;
    return 0;

no_mem:
    if (map->tab != NULL) {
        kfree(map->tab);
    }
    if (!maa->para->oparamter.base[level] && fifo->base) {
        dma_free_coherent(maa->dev, fifo->depth * sizeof(maa_buf_t), (void *)(uintptr_t)fifo->base, fifo->phy);
    }
    return -ENOMEM;
}

struct sk_buff *maa_to_skb(struct maa *maa, unsigned long long addr, unsigned long long *orig)
{
    struct sk_buff *skb = NULL;
    unsigned long flags;
    struct maa_map *map = NULL;
    maa_buf_t buf;
    struct maa_tab *tab = NULL;
    unsigned int idx;
    unsigned int level;

    buf.addr = addr;
    idx = buf.bits.idx;
    level = buf.bits.level;

    if (unlikely(!maa_opipe_is_enable(maa, level))) {
        maa_err("invalid level maa 0x%llx\n", addr);
        maa->status.unmap_invalid_level++;
        return NULL;
    }

    map = &maa->opipes[level].map;
    if (unlikely(idx < MAA_TABEL_START || idx > (map->tab_max_size - 1))) {
        maa->status.unmap_invalid_idx++;
        maa_err("invalid idx maa 0x%llx\n", addr);
        return NULL;
    }
    spin_lock_irqsave(&maa->opipes[level].lock, flags);
    tab = &map->tab[idx];
    skb = (struct sk_buff *)tab->manager;
    *orig = tab->maa_addr;
    if (unlikely(tab->magic != MAA_OWN || !tab->maa_addr)) {
        maa->opipe_status[level].unmap_own_fail++;
        WARN_ON_ONCE(1);
        maa_err_once("unmapaddr 0x%llx, keepaddr 0x%llx,level %u,idx %u\n", buf.addr, tab->maa_addr, level, idx);
        spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
        return NULL;
    }
    spin_unlock_irqrestore(&maa->opipes[level].lock, flags);
    return skb;
}

int maa_release_opipe_init(struct maa *maa)
{
    unsigned int level = maa->opipe_free_level;
    struct maa_fifo *fifo = &maa->opipes[level].fifo;

    fifo->depth = MAA_RELEASE_OPIPE_DEPTH;
    fifo->uthrl = MAA_RELEASE_OPIPE_UTHRL; /* up th */
    fifo->dthrl = MAA_RELEASE_OPIPE_DTHRL; /* up th */
    fifo->read = 0;
    fifo->write = 0;
    /* should use aximem */
    if (!fifo->base) {
        fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev, fifo->depth * sizeof(maa_buf_t),
            &fifo->phy, GFP_KERNEL);
    }

    if (!fifo->base) {
        maa_err("release opipe is null \n");
        return -ENOMEM;
    }
    maa_fifo_init(maa, fifo, level);
    maa_force_fifo_full(maa, fifo, level);
    return 0;
}

int maa_release_pool_init(struct maa *maa)
{
    struct maa_fifo *fifo = &maa->release.fifo;

    fifo->base = (unsigned long long)(uintptr_t)dma_alloc_coherent(maa->dev,
        (fifo->depth + MAA_TWO_BURST_RESERVE) * sizeof(maa_buf_t), &fifo->phy, GFP_KERNEL);
    if (!fifo->base) {
        maa_err("release fifo is null \n");
        return -ENOMEM;
    }

    __maa_release_pool_init(maa); // hal

    return 0;
}

void maa_global_init(struct maa *maa)
{
    maa->fifo_timer_en = MAA_TIMER_AP_OPIPE_EN_VALUE | BIT(maa->opipe_free_level);
    maa->int_mask = 0;
}

void maa_reserve_mem_init(struct maa *maa)
{
#ifdef DDR_MAA_MDM_ADDR
    unsigned long maa_addr;
    unsigned int maa_size;
#endif

    spin_lock_init(&maa->reserve_mem.lock);
#ifdef DDR_MAA_MDM_ADDR
    maa_addr = mdrv_mem_region_get("mdm_maa_ddr", &maa_size);
    maa->reserve_mem.start = maa_addr;
#else
    maa->reserve_mem.start = 0;
#endif
    maa->reserve_mem.cur = maa->reserve_mem.start;
#ifdef DDR_MAA_MDM_SIZE
    maa->reserve_mem.size = maa_size;
#else
    maa->reserve_mem.size = 0;
#endif
    maa->reserve_mem.end = maa->reserve_mem.start + maa->reserve_mem.size;
}

static void maa_dump_hook(void)
{
    unsigned int *reg = NULL;
    unsigned int i, j;
    unsigned int num = 0;
    struct maa *maa = g_maa_ctx;

    if (maa == NULL) {
        return;
    }

    for (i = 0; i < MAA_BACK_REGS_NUM && (maa != NULL); i++) {
        for (j = 0; j < maa->back[i].num; j++) {
            reg = maa->dump_base + num + j;
            *reg = readl((void *)(maa->back[i].start + j * sizeof(unsigned int)));
        }
        num += maa->back[i].num;
    }
}

static void maa_dump_init(struct maa *maa)
{
    int ret;

    maa->dump_base = (unsigned int *)bsp_dump_register_field(DUMP_MODEMAP_MAA, "MAA", MAA_DUMP_SIZE, 0);
    if (maa->dump_base == NULL) {
        maa_err("dump mem alloc fail\n");
        return;
    }
    ret = (int)memset_s(maa->dump_base, MAA_DUMP_SIZE, 0, MAA_DUMP_SIZE);
    if (ret) {
        maa_err("dump memset_s fail\n");
    }

    ret = bsp_dump_register_hook("maa", maa_dump_hook);
    if (ret == BSP_ERROR) {
        maa_err("register om fail\n");
    }
}

int maa_core_probe(struct maa *maa)
{
    int ret;
    unsigned int level;
    g_maa_ctx = maa;

    BUILD_BUG_ON((SKB_DATA_ALIGN(MAA_HEAD_ROM_SIZE) > NET_SKB_PAD));

    maa_global_init(maa);

    maa_reserve_mem_init(maa);

    maa_basic_cfg(maa);

    ret = maa_release_pool_init(maa);
    if (ret) {
        return -1;
    }

    /* ddr buf init must before opipe init */
    for (level = 0; level <= maa->max_opipes; level++) {
        if (!maa_opipe_is_enable(maa, level)) {
            continue;
        }
        ret = maa_ddr_buf_init(maa, level);
        if (ret) {
            maa_err("maa_ddr_buf_init failed\n");
            return -1;
        }
    }
    for (level = 0; level <= maa->max_opipes; level++) {
        if (!maa_opipe_is_enable(maa, level)) {
            continue;
        }
        ret = maa_opipe_init(maa, level);
        if (ret) {
            maa_err("maa_opipe_init failed\n");
            return -1;
        }
    }

    maa_cpu_ipipe_init(maa);
    /* use level5 for directly free up packet */
    if (maa->opipe_for_free) {
        maa_release_opipe_init(maa);
    }

    maa_reclaim_fullfill(maa);
    maa_err("maa_interupt_init\n");
    maa_interupt_init(maa);

    spin_lock_init(&maa->reset_lock);
    bsp_err("bsp_reset_cb_func_register\n");
    if (bsp_reset_cb_func_register("MAA_BALONG", maa_mdmreset_cb, 0, DRV_RESET_CB_PIOR_MAA)) {
        bsp_err("set modem reset call back func failed\n");
    }

    maa_dump_init(maa);

    maa_trans_report_register();

    maa_err("[init] ok\n");

    return 0;
}

void __maa_set_backup_regs(struct maa *maa, unsigned int idx, unsigned int start, unsigned int end)
{
    unsigned int num;
    if (maa == NULL) {
        maa_err("maa is null\n");
        return;
    }

    num = maa->back_regs_num;

    if (idx < MAA_BACK_REGS_NUM && (maa != NULL)) {
        num += maa->back[idx].num;
        if (num < MAA_DUMP_REG_NUM) {
            maa->back[idx].start = (char *)maa->regs + start;
            maa->back[idx].num = (end - start) / sizeof(unsigned int) + 1;
            maa->back_regs_num = num;
        }
    }
}

int maa_suspend(struct device *dev)
{
    return maa_hal_suspend(g_maa_ctx);
}
EXPORT_SYMBOL(maa_suspend);
int maa_resume(struct device *dev)
{
    return maa_hal_resume(g_maa_ctx);
}
EXPORT_SYMBOL(maa_resume);
int maa_prepare(struct device *dev)
{
    return maa_hal_prepare(g_maa_ctx);
}
void maa_complete(struct device *dev)
{
    maa_hal_complete(g_maa_ctx);
}

int maa_deinit(void)
{
    struct maa *maa = g_maa_ctx;
    unsigned long flags = 0;

    maa_detach(maa);
    disable_irq(maa->irq);

    spin_lock_irqsave(&maa->reset_lock, flags);
    maa->resetting = 1;
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    flush_workqueue(maa->workqueue);
    mod_timer(&maa->timer, jiffies + MAA_TIMER_EXPIRES);

    maa->deinit_cnt++;
    return 0;
}

int maa_interrupt_reinit(void)
{
    struct maa *maa = g_maa_ctx;
    maa_interupt_init(maa);
    enable_irq(maa->irq);
    return 0;
}

int bsp_maa_reinit(void)
{
    int i;
    struct maa *maa = g_maa_ctx;
    unsigned long flags;
    struct maa_tab *pos = NULL;
    struct maa_tab *n = NULL;
    struct sk_buff *skb = NULL;
    maa_buf_t buf;

    for (i = 0; i <= maa->max_opipes; i++) {
        if (!maa_opipe_is_enable(maa, i)) {
            continue;
        }
        spin_lock_irqsave(&maa->opipes[i].lock, flags);
        list_splice_init(&maa->opipes[i].map.busy_list, &maa->opipes[i].map.reclaim_list);
        maa->opipes[i].reclaim_depth += maa->opipes[i].busy_depth;
        maa->opipes[i].delay_free_cnt += maa->opipes[i].busy_depth;
        maa->opipes[i].busy_depth = 0;
        list_for_each_entry_safe(pos, n, &maa->opipes[i].map.reclaim_list, list) {
            buf.addr = pos->maa_addr;
            if (buf.bits.from == MAA_SKB_FROM_TCP_IP) {
                skb = __maa_unmap_tcpip_skb(maa, buf);
                dev_kfree_skb_any(skb);
                maa->opipes[i].reclaim_depth--;
            }
        }
        spin_unlock_irqrestore(&maa->opipes[i].lock, flags);
        maa_fifo_reinit(maa, &maa->opipes[i].fifo, i);
    }

    for (i = 0; i < maa->max_cpu_ipipes; i++) {
        if (!maa->para->iparamter.depth[i]) {
            continue;
        }
        maa_ipipe_init(maa, &maa->cpu_ipipes[i], maa->para->iparamter.ipipeidx[i]);
    }
    maa_opipe2_lp_ipipe_init(maa);
    if (maa->opipe_for_free) {
        maa_release_opipe_init(maa);
    }
    __maa_release_pool_init(maa);
    maa->release.fifo.read = 0;
    maa->release.fifo.write = 0;
    spin_lock_irqsave(&maa->reset_lock, flags);
    maa->resetting = 0;
    spin_unlock_irqrestore(&maa->reset_lock, flags);

    maa_interrupt_reinit();

    maa->reinit_cnt++;
    return 0;
}

int maa_mdmreset_cb(drv_reset_cb_moment_e eparam, int userdata)
{
    if (eparam == MDRV_RESET_CB_BEFORE) {
        maa_deinit();
    }
    return 0;
}

void maa_mdmreset_status(void)
{
    struct maa *maa = g_maa_ctx;
    bsp_err("timer cnt:%d\n", maa->timer_cnt);
    bsp_err("resetting:%d\n", maa->resetting);
    bsp_err("deinit_cnt:%d\n", maa->deinit_cnt);
    bsp_err("reinit_cnt:%d\n", maa->reinit_cnt);
}
