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

#define EICC_CHECK_LEVEL_IMPOSSIBLE

void eicc_opipe_enable(struct eicc_device *pdev, struct eicc_channel *pchannel)
{
#if EICC_FEATURE_KERNEL_INSEC
    eicc_core_unsec(pdev, EICC_GET_CORE_ID(pchannel->ldrvchn_id));
    eicc_opipe_unsec(pdev, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
#endif
    eicc_opipe_startup(pdev->base_va, pchannel->opipe_cfg);
    eicc_opipe_devint_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), pchannel->opipe_cfg->int_level);
    eicc_opipe_coreint_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id),
        EICC_GET_CORE_ID(pchannel->ldrvchn_id));
    eicc_opipe_local_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
}

static int eicc_opipe_open(struct eicc_device *pdev, struct eicc_channel *pchannel, struct compat_attr *pcompat_attr)
{
    if (pchannel->opipe_cfg == NULL) {
        pchannel->opipe_cfg = eicc_malloc(sizeof(struct opipe_config));
        if (pchannel->opipe_cfg == NULL) {
            return -1;
        }
    }

    (void)memset_s(pchannel->opipe_cfg, sizeof(struct opipe_config), 0, sizeof(struct opipe_config));

    pchannel->opipe_cfg->id = EICC_GET_PIPE_ID(pchannel->ldrvchn_id);
    pchannel->opipe_cfg->ringbuf_va = pcompat_attr->va;
    pchannel->opipe_cfg->ringbuf_pa = pcompat_attr->pa;
    pchannel->opipe_cfg->depth = pcompat_attr->depth;
    pchannel->opipe_cfg->ram_size = pcompat_attr->buf_sz;

    pchannel->opipe_cfg->ptr_vaddr = &pdev->push_ptr_va[EICC_GET_PIPE_ID(pchannel->ldrvchn_id)];
    *pchannel->opipe_cfg->ptr_vaddr = pchannel->opipe_cfg->rptr;
    pchannel->opipe_cfg->ptr_paddr = pdev->push_ptr_pa + sizeof(u32) * EICC_GET_PIPE_ID(pchannel->ldrvchn_id);

    if (isMsgSendChn(pchannel)) {
        pchannel->opipe_cfg->opipe_uctrl.union_stru.prior = 1;
        pchannel->opipe_cfg->opipe_uctrl.union_stru.type = 0;
    } else {
        pchannel->opipe_cfg->opipe_uctrl.union_stru.prior = 3;
        pchannel->opipe_cfg->opipe_uctrl.union_stru.type = 1;
    }
    /* wakeup中断的几个条件 1，remote en为0 2，opipe不空且(达到上水线或者达到超时) */
    pchannel->opipe_cfg->opipe_uctrl.union_stru.up_thrh = 0;
    pchannel->opipe_cfg->opipe_uctrl.union_stru.dn_thrh = 0;
    pchannel->opipe_cfg->int_level = EICC_INT_WR_PIPE_ERR;

    eicc_opipe_enable(pdev, pchannel);
    return 0;
}

void eicc_ipipe_enable(struct eicc_device *pdev, struct eicc_channel *pchannel)
{
#if EICC_FEATURE_KERNEL_INSEC
    eicc_core_unsec(pdev, EICC_GET_CORE_ID(pchannel->ldrvchn_id));
    eicc_ipipe_unsec(pdev, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
#endif
    eicc_ipipe_startup(pdev->base_va, pchannel->ipipe_cfg);
    eicc_ipipe_devint_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), pchannel->ipipe_cfg->int_level);
    eicc_ipipe_coreint_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id),
        EICC_GET_CORE_ID(pchannel->ldrvchn_id));

    eicc_ipipe_local_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));

    if (pchannel->const_flags & EICC_CHN_SF_REMOTE_READY) {
        pchannel->remote_awake = 1;
    }

    if (pchannel->type == EICC_CHN_TYPE_MSG_OUTRECV && pchannel->remote_awake) {
        eicc_ipipe_remote_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        pchannel->remote_awake = 0;
    }
    if (pchannel->type == EICC_CHN_TYPE_MSG_INRRECV) {
        eicc_ipipe_devint_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), EICC_INT_RD_PIPE_WAKEUP);
    }
}

static int eicc_ipipe_open(struct eicc_device *pdev, struct eicc_channel *pchannel, struct compat_attr *pcompat_attr)
{
    struct eicc_device *p_relev_dev = NULL;

    if (pchannel->ipipe_cfg == NULL) {
        pchannel->ipipe_cfg = eicc_malloc(sizeof(struct ipipe_config));
        if (pchannel->ipipe_cfg == NULL) {
            return -1;
        }
    }

    (void)memset_s(pchannel->ipipe_cfg, sizeof(struct ipipe_config), 0, sizeof(struct ipipe_config));

    pchannel->ipipe_cfg->id = EICC_GET_PIPE_ID(pchannel->ldrvchn_id);
    pchannel->ipipe_cfg->ringbuf_va = pcompat_attr->va;
    pchannel->ipipe_cfg->ringbuf_pa = pcompat_attr->pa;
    pchannel->ipipe_cfg->depth = pcompat_attr->depth;
    pchannel->ipipe_cfg->ram_size = pcompat_attr->buf_sz;

    pchannel->ipipe_cfg->ptr_vaddr = &pdev->push_ptr_va[EICC_GET_PIPE_ID(pchannel->ldrvchn_id)];
    *pchannel->ipipe_cfg->ptr_vaddr = pchannel->ipipe_cfg->wptr;
    pchannel->ipipe_cfg->ptr_paddr = pdev->push_ptr_pa + sizeof(u32) * EICC_GET_PIPE_ID(pchannel->ldrvchn_id);

    pchannel->ipipe_cfg->ipipe_uctrl.union_stru.prior = 1;
    if (pchannel->type == EICC_CHN_TYPE_MSG_OUTRECV) {
        pchannel->ipipe_cfg->ipipe_uctrl.union_stru.type = 1;
        p_relev_dev = eicc_device_get_fast(EICC_GET_DEV_ID(pchannel->rdrvchn_id));
        if (p_relev_dev == NULL) {
            /* impossible */
            eicc_print_error("eicc_device_get_fast relev device fail\n");
            return -1;
        }
        pchannel->ipipe_cfg->relv_pipe_paddr =
            p_relev_dev->base_pa + EICC_IPIPE_BASE_ADDR_L(EICC_GET_PIPE_ID(pchannel->rdrvchn_id));
    } else {
        pchannel->ipipe_cfg->ipipe_uctrl.union_stru.type = 0;
    }

    if ((pchannel->dym_flags & EICC_DYM_SF_NOARRVEVENT) == 0) {
        pchannel->ipipe_cfg->int_level = EICC_INT_RD_DATA_ARV;
    }

    eicc_ipipe_enable(pdev, pchannel);
    return 0;
}

void eicc_shadow_ipipe_enable(struct eicc_device *pdev, struct eicc_channel *pchannel)
{
#if EICC_FEATURE_KERNEL_INSEC
    eicc_shadow_ipipe_startup(pdev->base_va, pchannel->ipipe_cfg);
    /* 打开唤醒中断 */
    if ((pchannel->const_flags & EICC_CHN_SF_REMOTE_READY) == 0) {
        eicc_ipipe_devint_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), EICC_INT_RD_PIPE_WAKEUP);
        eicc_ipipe_coreint_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id),
            EICC_GET_RCORE_ID(pchannel->ldrvchn_id));
    }
    eicc_ipipe_local_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
#endif
}

static int eicc_shadow_ipipe_open(struct eicc_device *pdev, struct eicc_channel *pchannel)
{
    struct eicc_device *p_relev_dev = NULL;

    if (pchannel->ipipe_cfg == NULL) {
        pchannel->ipipe_cfg = eicc_malloc(sizeof(struct ipipe_config));
        if (pchannel->ipipe_cfg == NULL) {
            return -1;
        }
    }

    (void)memset_s(pchannel->ipipe_cfg, sizeof(struct ipipe_config), 0, sizeof(struct ipipe_config));

    pchannel->ipipe_cfg->id = EICC_GET_PIPE_ID(pchannel->ldrvchn_id);

    p_relev_dev = eicc_device_get_fast(EICC_GET_DEV_ID(pchannel->rdrvchn_id));
    if (p_relev_dev == NULL) {
        /* impossible */
        eicc_print_error("eicc_device_get_fast relev device fail\n");
        return -1;
    }

    pchannel->ipipe_cfg->relv_pipe_paddr =
        p_relev_dev->base_pa + EICC_IPIPE_BASE_ADDR_L(EICC_GET_PIPE_ID(pchannel->rdrvchn_id));

    pchannel->ipipe_cfg->ipipe_uctrl.union_stru.prior = 1;
    pchannel->ipipe_cfg->ipipe_uctrl.union_stru.type = 0x2;
    /* wakeup中断的几个条件 1，remote en为0 2，opipe不空且(达到上水线或者达到超时) */
    pchannel->ipipe_cfg->ipipe_uctrl.union_stru.arv_timeout = 1;
    eicc_shadow_ipipe_enable(pdev, pchannel);
    return 0;
}

static int eicc_channel_open(struct eicc_device *pdev, struct eicc_channel *pchannel, struct compat_attr *pcompat_attr)
{
    int ret = 0;
    if (isMsgRecvChn(pchannel)) {
        ret = eicc_ipipe_open(pdev, pchannel, pcompat_attr);
    } else if (pchannel->type == EICC_CHN_TYPE_MSG_OUTSEND) {
        if (eicc_shadow_ipipe_open(pdev, pchannel) == 0) {
            ret = eicc_opipe_open(pdev, pchannel, pcompat_attr);
        }
    } else if (pchannel->type == EICC_CHN_TYPE_MSG_INRSEND || pchannel->type == EICC_CHN_TYPE_DMA) {
        ret = eicc_opipe_open(pdev, pchannel, pcompat_attr);
    } else {
        ret = -1;
    }

    return ret;
}

/* 目前实现无锁保护，可以用于低功耗场景 */
int eicc_channel_halt(struct eicc_device *pdev, struct eicc_channel *pchannel)
{
    int query_times;
    eicc_opipe_abort_set(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
    for (query_times = 0; query_times < EICC_HALT_QUERY_WAIT_TIMES; query_times++) {
        if (EICC_INT_WR_ABORT & eicc_opipe_devint_raw(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id))) {
            break;
        }
    }
    if (query_times >= EICC_HALT_QUERY_WAIT_TIMES) {
        return -1;
    }
    eicc_opipe_devint_clr(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), EICC_INT_WR_ABORT);
    eicc_opipe_local_dis(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
    eicc_opipe_abort_clr(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
    pchannel->opipe_cfg->wptr = 0;
    eicc_opipe_wptr_update(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), pchannel->opipe_cfg->wptr);
    pchannel->opipe_cfg->rptr = 0;
    eicc_opipe_rptr_update(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), pchannel->opipe_cfg->rptr);
    *(volatile u32 *)(pchannel->opipe_cfg->ptr_vaddr) = 0;

    for (query_times = 0; query_times < EICC_HALT_QUERY_WAIT_TIMES; query_times++) {
        if (0x1 & eicc_opipe_local_status(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id))) {
            break;
        }
    }
    if (query_times >= EICC_HALT_QUERY_WAIT_TIMES) {
        return -1;
    }
    /* 如果还要重发数据，那要在en前回复rptr和wptr */
    eicc_opipe_local_en(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));

    return 0;
}

/* opipe中还有多少剩余空间 */
static inline u32 opipe_free_space_get(struct eicc_channel *pchannel)
{
    u32 w_off = pchannel->opipe_cfg->wptr;
    u32 r_off = *(volatile u32 *)(pchannel->opipe_cfg->ptr_vaddr);
    u32 bufsz = pchannel->opipe_cfg->depth;
    eicc_print_trace("w_off : 0x%x  r_off : 0x%x\n", (unsigned)w_off, (unsigned)r_off);
    return (w_off >= r_off) ? (bufsz - w_off + r_off) : (r_off - w_off);
}

/* ipipe中还有多少数据未读取 */
static inline u32 ipipe_used_space_get(struct eicc_channel *pchannel)
{
    u32 r_off = pchannel->ipipe_cfg->rptr;
    u32 w_off = *(volatile u32 *)(pchannel->ipipe_cfg->ptr_vaddr);
    u32 bufsz = pchannel->ipipe_cfg->depth;
    eicc_print_trace("w_off : 0x%x  r_off : 0x%x\n", (unsigned)w_off, (unsigned)r_off);
    return (w_off >= r_off) ? (w_off - r_off) : (bufsz + w_off - r_off);
}

static int opipe_put_to_ringbuf(const struct eicc_channel *pchannel, const u8 *data, u32 data_len, u32 *pw_off)
{
    u8 *ringbuf = (u8 *)pchannel->opipe_cfg->ringbuf_va;
    u32 w_off = *(pw_off);
    u32 r_off = *(volatile u32 *)(pchannel->opipe_cfg->ptr_vaddr);
    u32 bufsz = pchannel->opipe_cfg->depth;

    u32 tail_sz, wrapped_sz;

#ifdef EICC_CHECK_LEVEL_IMPOSSIBLE
    if (bufsz < w_off || bufsz < r_off) {
        /* big error; */
        return -1;
    }
#endif
    /* w_off,r_off never be same as bufsz */
    if (w_off >= r_off) {
        tail_sz = bufsz - w_off;
        wrapped_sz = r_off; /* may be zero */
    } else {
        tail_sz = r_off - w_off;
        wrapped_sz = 0;
    }

    /*
     * 长度不能等于剩余空间，否则写指针会更新为和读指针相同的值,造成芯片认为buf为空
     */
    if (tail_sz + wrapped_sz <= data_len) {
        return -1;
    }

    if (tail_sz >= data_len) {
        if (memcpy_s(ringbuf + w_off, tail_sz, data, data_len)) {
            return -1;
        }
        w_off = (w_off + data_len == bufsz) ? 0 : (w_off + data_len);
    } else {
        if (memcpy_s(ringbuf + w_off, tail_sz, data, tail_sz)) {
            return -1;
        }
        if (memcpy_s(ringbuf, wrapped_sz, data + tail_sz, data_len - tail_sz)) { /*lint !e613*/
            return -1;
        }
        w_off = data_len - tail_sz;
    }
    *(pw_off) = w_off;
    return 0;
}

static int ipipe_get_from_ringbuf(const struct eicc_channel *pchannel, u8 *data, u32 data_len, u32 *pr_off)
{
    u8 *ringbuf = (u8 *)pchannel->ipipe_cfg->ringbuf_va;
    u32 r_off = *(pr_off);
    u32 w_off = *(volatile u32 *)(pchannel->ipipe_cfg->ptr_vaddr);
    u32 bufsz = pchannel->ipipe_cfg->depth;

    u32 tail_sz, wrapped_sz;
#ifdef EICC_CHECK_LEVEL_IMPOSSIBLE
    if (bufsz < w_off || bufsz < r_off) {
        /* big error; */
        return -1;
    }
#endif
    /* w_off,r_off never be same as bufsz */
    if (w_off >= r_off) {
        tail_sz = w_off - r_off; /* may be zero */
        wrapped_sz = 0;
    } else {
        tail_sz = bufsz - r_off;
        wrapped_sz = w_off; /* may be zero */
    }
#ifdef EICC_CHECK_LEVEL_IMPOSSIBLE
    /* big error should be detected by this function caller */
    if (tail_sz + wrapped_sz < data_len) {
        return -1;
    }
#endif
    if (tail_sz >= data_len) {
        if (memcpy_s(data, data_len, ringbuf + r_off, data_len)) {
            return -1;
        }
        r_off = (r_off + data_len == bufsz) ? 0 : (r_off + data_len);
    } else {
        if (memcpy_s(data, data_len, ringbuf + r_off, tail_sz)) {
            return -1;
        }
        if (memcpy_s(data + tail_sz, data_len - tail_sz, ringbuf, data_len - tail_sz)) {
            return -1;
        }
        r_off = data_len - tail_sz;
    }
    *(pr_off) = r_off;
    return 0;
}

/* 这个函数依赖eicc buff本身四字节对齐，buff内的使用至少4字节对齐 */
static void ipipe_pkt_ts_set(struct eicc_channel *pchannel)
{
    u8 *ringbuf = NULL;
    u32 r_off, w_off, bufsz, avail_space;

    u32 *ts = NULL;
    u32 ts_off;

    ringbuf = (u8 *)pchannel->ipipe_cfg->ringbuf_va;
    r_off = pchannel->ipipe_cfg->rptr;
    w_off = *(volatile u32 *)(pchannel->ipipe_cfg->ptr_vaddr);
    bufsz = pchannel->ipipe_cfg->depth;
    avail_space = (w_off >= r_off) ? (w_off - r_off) : (bufsz + w_off - r_off);
    if (sizeof(struct eicc_packet) > avail_space) {
        eicc_print_error("ipipe pkt is too small, avail_space = 0x%x\n", (unsigned)avail_space);
        return;
    }
    ts_off = offsetof(struct eicc_packet, recvtime1);
    if (r_off + ts_off >= bufsz) {
        ts = (u32 *)(ringbuf + r_off + ts_off - bufsz);
    } else {
        ts = (u32 *)(ringbuf + r_off + ts_off);
    }
    *ts = pchannel->irq_ts;
    ts_off = offsetof(struct eicc_packet, recvtime2);
    if (r_off + ts_off >= bufsz) {
        ts = (u32 *)(ringbuf + r_off + ts_off - bufsz);
    } else {
        ts = (u32 *)(ringbuf + r_off + ts_off);
    }
    *ts = (*ts) + 1;
    return;
}

int opipe_align_fixed(const struct eicc_channel *pchannel, u32 fixed_len, u32 *pw_off)
{
    u32 w_off = *(pw_off);
    if (w_off + fixed_len >= pchannel->opipe_cfg->depth) {
        w_off = w_off + fixed_len - pchannel->opipe_cfg->depth;
    } else {
        w_off = w_off + fixed_len;
    }
    *pw_off = w_off;
    return 0;
}

int ipipe_align_fixed(const struct eicc_channel *pchannel, u32 fixed_len, u32 *pr_off)
{
    u32 r_off = *(pr_off);
    if (r_off + fixed_len >= pchannel->ipipe_cfg->depth) {
        r_off = r_off + fixed_len - pchannel->ipipe_cfg->depth;
    } else {
        r_off = r_off + fixed_len;
    }
    *pr_off = r_off;
    return 0;
}

int bsp_eicc_chn_attr_init(eicc_chn_attr_t *pattr)
{
    if (pattr == NULL) {
        eicc_print_error("bad param @%d\n", __LINE__);
        return -1;
    }
    pattr->chnid = (u32)EICC_CHN_INVAILD;
    pattr->type = EICC_CHN_TYPE_INVAILD;
    pattr->va = NULL;
    pattr->pa = 0;
    pattr->size = 0;
    pattr->cbk = NULL;
    pattr->cbk_arg = NULL;
    pattr->flags = 0;

    return 0;
}
static int eicc_chn_attr_check(struct eicc_channel *pchannel, eicc_chn_attr_t *pattr, struct compat_attr *pcompat_attr)
{
    if (pattr->chnid == (u32)EICC_CHN_INVAILD) {
        eicc_print_error("invaild pattr->chnid\n");
        return -1;
    }
    if (isMsgSendChn(pchannel)) {
        if (pattr->type != EICC_CHN_TYPE_SEND) {
            return -1;
        }
    } else if (isMsgRecvChn(pchannel)) {
        if (pattr->type != EICC_CHN_TYPE_RECV) {
            return -1;
        }
    } else {
        if (pattr->type != EICC_CHN_TYPE_SEND) {
            return -1;
        }
    }

    if (pattr->size <= sizeof(struct eicc_packet)) {
        return -1;
    }
    if ((pattr->pa & EICC_PIPE_ADDR_ALIGN_MASK) || (pattr->size & EICC_PIPE_DEPTH_ALIGN_MASK)) {
        eicc_print_error("eicc attr align check,pattr->va is 0x%x, pattr->pa is 0x%x pattr->size is 0x%x\n",
            (unsigned)(uintptr_t)pattr->va, (unsigned)pattr->pa, (unsigned)pattr->size);
        return -1;
    }
    pcompat_attr->align_bytes = EICC_CHN_DEFAULT_ALIGNBYTES;
    pcompat_attr->pa = pattr->pa;
    pcompat_attr->va = pattr->va;
    pcompat_attr->depth = pattr->size;
    pcompat_attr->buf_sz = pattr->size;
    pcompat_attr->dym_flags = pattr->flags;
    return 0;
}

int bsp_eicc_chn_open(eicc_chn_t *pchn_hdl, eicc_chn_attr_t *pattr)
{
    eicc_chn_t chn_hdl;
    unsigned long flags;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_channel *pchannel_fake = NULL;
    struct compat_attr compat_attr;

    if (pchn_hdl == NULL || pattr == NULL) {
        eicc_print_error("bad param @%d\n", __LINE__);
        return EICC_ERR_EINVAL;
    }
    eicc_print_trace("bsp_eicc_chn_open pattr->chnid=0x%x\n", pattr->chnid);
    /* get device and channel */
    pchannel_fake = eicc_channel_get_fast(pattr->chnid);
    if (pchannel_fake == NULL) {
        eicc_print_error("attr->usr_chnid %d is not defined,check dts please\n", pattr->chnid);
        return EICC_ERR_ENXIO;
    }

    chn_hdl = pchannel_fake->ldrvchn_id;
    if (eicc_dev_chn_get(EICC_GET_PHYCHN_ID(chn_hdl), &pdev, &pchannel)) {
        eicc_print_error("eicc_dev_chn_get failed\n");
        return EICC_ERR_ENXIO;
    }

    spin_lock_irqsave(&pchannel->lock, flags);
    if (pchannel->state != (u32)EICC_CHN_INIT) {
        spin_unlock_irqrestore(&pchannel->lock, flags);
        eicc_print_error("bad state @%d\n", __LINE__);
        return EICC_ERR_EBADFD;
    }

    if (eicc_chn_attr_check(pchannel, pattr, &compat_attr)) {
        spin_unlock_irqrestore(&pchannel->lock, flags);
        eicc_print_error("bad param @%d\n", __LINE__);
        return -1;
    }
    pchannel->align_bytes = compat_attr.align_bytes;
    pchannel->dym_flags = compat_attr.dym_flags;
    pchannel->cbk = pattr->cbk; /* can be null */
    pchannel->cbk_arg = pattr->cbk_arg;

    if (eicc_channel_open(pdev, pchannel, &compat_attr)) {
        spin_unlock_irqrestore(&pchannel->lock, flags);
        eicc_print_error("eicc_channel_open failed @%d\n", __LINE__);
        return -1;
    }

    pchannel->state = (u32)EICC_CHN_OPEN;
    spin_unlock_irqrestore(&pchannel->lock, flags);

    *(pchn_hdl) = chn_hdl;
    return 0;
}

static int eicc_fill_hdr(struct eicc_channel *pchannel, struct eicc_send_packet *ppacket,
    struct eicc_send_helper *phelper)
{
    u32 blkidx;
    u32 head_len;
    u32 total_msglen = 0;

    eicc_blk_desc_t *pblkdesc = (eicc_blk_desc_t *)phelper->send_desc;
    for (blkidx = 0; blkidx < pblkdesc->cnt; blkidx++) {
        if (pblkdesc->datablk[blkidx].buf == NULL || pblkdesc->datablk[blkidx].len == 0) { /* 不支持空消息发送 */
            eicc_print_error("bad param @%d\n", __LINE__);
            return EICC_ERR_EINVAL;
        }
        total_msglen = total_msglen + pblkdesc->datablk[blkidx].len;
    }
    if (total_msglen == 0 || total_msglen > 0xFFFFUL) {
        eicc_print_error("bad param @%d\n", __LINE__);
        return EICC_ERR_EINVAL;
    }

    /* 这步有点多余，我们的头一定32bytes对齐 */
    head_len = eicc_roundup(sizeof(struct eicc_packet), (pchannel->align_bytes));
    ppacket->bd_head.cfg.msg_len = head_len + eicc_roundup(total_msglen, pchannel->align_bytes);
    if (phelper->send_flags & EICC_SEND_FLAGS_NOARRVNOTIFY) {
        ppacket->bd_head.cfg.II_pos = 0;
    } else {
        ppacket->bd_head.cfg.II_pos = 1;
    }
    ppacket->data_head.user_id = pchannel->user_id;
    ppacket->data_head.len = (u16)total_msglen;
    ppacket->data_head.timestamp = eicc_get_slice_value();

    ppacket->data_head.align_bytes = pchannel->align_bytes;
    ppacket->data_head.ldrvchn_id = pchannel->ldrvchn_id;
    ppacket->data_head.rdrvchn_id = pchannel->rdrvchn_id;

    phelper->head_len = head_len;
    phelper->total_msglen = total_msglen;
    phelper->write_len = ppacket->bd_head.cfg.msg_len + sizeof(struct eicc_bd1_s);

    return 0;
}

static int eicc_write_packet(struct eicc_device *pdev, struct eicc_channel *pchannel, struct eicc_send_packet *ppacket,
    struct eicc_send_helper *phelper)
{
    u32 blkidx;
    u32 total_msglen, write_len, space_left;
    u32 wptr;
    eicc_blk_desc_t *pblkdesc = (eicc_blk_desc_t *)phelper->send_desc;

    total_msglen = phelper->total_msglen;
    write_len = phelper->write_len;
    space_left = opipe_free_space_get(pchannel);
    eicc_print_trace("space left : 0x%x\n", (unsigned)space_left);
    if (write_len >= space_left) {
        eicc_print_trace("opipe nospace,write len: 0x%x, space left: 0x%x, send fail\n", write_len, space_left);
        return EICC_ERR_EAGAIN;
    }

    wptr = pchannel->opipe_cfg->wptr;
    if (opipe_put_to_ringbuf(pchannel, (u8 *)ppacket, sizeof(*ppacket), &wptr)) {
        eicc_print_error("opipe_put_to_ringbuf failed @%d\n", __LINE__);
        return EICC_ERR_EIO;
    }
    opipe_align_fixed(pchannel, 0, &wptr);

    for (blkidx = 0; blkidx < pblkdesc->cnt; blkidx++) {
        if (opipe_put_to_ringbuf(pchannel, pblkdesc->datablk[blkidx].buf, pblkdesc->datablk[blkidx].len, &wptr)) {
            eicc_print_error("opipe_put_to_ringbuf failed @%d\n", __LINE__);
            return EICC_ERR_EIO;
        }
    }
    opipe_align_fixed(pchannel, eicc_roundup(total_msglen, pchannel->align_bytes) - total_msglen, &wptr);

    pchannel->opipe_cfg->wptr = wptr;
    eicc_opipe_wptr_update(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id), pchannel->opipe_cfg->wptr);
    eicc_print_trace("eicc_opipe_wptr_update wptr: 0x%x\n,rptr:0x%x ", (unsigned)wptr,
        (unsigned)*(volatile u32 *)(pchannel->opipe_cfg->ptr_vaddr));
    return 0;
}

int bsp_eicc_chn_blks_send(eicc_chn_t chn_hdl, eicc_blk_desc_t *pblkdesc, u32 send_flags)
{
    int ret;
    unsigned long flags;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_send_packet packet = { { { 0 } }, { 0 } };
    struct eicc_send_helper helper;
    struct eicc_send_helper *phelper = &helper;
    if (pblkdesc == NULL || eicc_dev_chn_get(EICC_GET_PHYCHN_ID(chn_hdl), &pdev, &pchannel)) {
        eicc_print_error("invaild param for chn_hdl 0x%x\n", (unsigned)chn_hdl);
        return EICC_ERR_EINVAL;
    }
    if (pchannel->state != (u32)EICC_CHN_OPEN || !isMsgSendChn(pchannel)) {
        eicc_print_error("bad chn_hdl 0x%x\n", (unsigned)chn_hdl);
        return EICC_ERR_EBADFD;
    }
    phelper->send_desc = pblkdesc;
    phelper->send_flags = send_flags;
    if (eicc_fill_hdr(pchannel, &packet, phelper)) {
        return EICC_ERR_EINVAL;
    }

    spin_lock_irqsave(&pchannel->lock, flags);
    ret = eicc_write_packet(pdev, pchannel, &packet, phelper);
    spin_unlock_irqrestore(&pchannel->lock, flags);
    if (ret) {
        return ret;
    }
    return (int)packet.data_head.len;
}

int bsp_eicc_chn_send(eicc_chn_t chn_hdl, void *buf, unsigned len, u32 send_flags)
{
    eicc_blkx2_desc_t send_desc;
    send_desc.cnt = 1;
    send_desc.datablk[0].buf = buf;
    send_desc.datablk[0].len = len;
    return bsp_eicc_chn_blks_send(chn_hdl, (eicc_blk_desc_t *)&send_desc, send_flags);
}

static int eicc_read_hdr(struct eicc_channel *pchannel, struct eicc_packet *ppkthdr, struct eicc_recv_helper *phelper)
{
    u32 used_len;
    used_len = ipipe_used_space_get(pchannel);
    eicc_print_trace("ipipe used length 0x%x\n", (unsigned)used_len);
    if (used_len == 0) {
        phelper->used_len = used_len;
        return 0;
    }
#ifdef EICC_CHECK_LEVEL_IMPOSSIBLE
    if (used_len < sizeof(struct eicc_packet)) {
        /* big error */
        return -1;
    }
#endif
    phelper->rptr = pchannel->ipipe_cfg->rptr;
    if (ipipe_get_from_ringbuf(pchannel, (u8 *)ppkthdr, sizeof(*ppkthdr), &phelper->rptr)) {
        eicc_print_error("ipipe_get_from_ringbuf failed @%d\n", __LINE__);
        return -1;
    }
#ifdef EICC_CHECK_LEVEL_IMPOSSIBLE
    if (ppkthdr->align_bytes != pchannel->align_bytes) {
        eicc_print_error("ipipe_get_from_ringbuf failed @%d\n", __LINE__);
        return -1;
    }
    if (used_len < eicc_roundup(ppkthdr->len, pchannel->align_bytes) +
        eicc_roundup(sizeof(struct eicc_packet), pchannel->align_bytes)) {
        eicc_print_error("ipipe_get_from_ringbuf failed @%d\n", __LINE__);
        return -1;
    }
#endif
    phelper->used_len = used_len;
    return 0;
}

int eicc_read_payload(struct eicc_device *pdev, struct eicc_channel *pchannel, struct eicc_packet *ppkthdr,
    struct eicc_recv_helper *phelper)
{
    int ret = 0;
    u32 ret_len = 0;
#ifdef EICC_CHECK_LEVEL_IMPOSSIBLE
    if (phelper->payloadskip_len >= ppkthdr->len) {
        eicc_print_error("payloadskip_len check failed @%d\n", __LINE__);
        return -1;
    }
#endif
    if (phelper->len < ppkthdr->len - phelper->payloadskip_len) {
        ret = ipipe_get_from_ringbuf(pchannel, (u8 *)phelper->buf, phelper->len, &phelper->seek);
        if (ret == 0) {
            ret_len = phelper->len;
            if ((phelper->recv_flags & EICC_RECV_FLAGS_DATAPEEK) == 0) {
                pchannel->ipipe_cfg->seek = phelper->seek;
            }
        } else {
            eicc_print_error("ipipe_get_from_ringbuf failed @%d\n", __LINE__);
        }
    } else {
        ret = ipipe_get_from_ringbuf(pchannel, (u8 *)phelper->buf, ppkthdr->len - phelper->payloadskip_len,
            &phelper->seek);
        if (ret == 0) {
            ret_len = ppkthdr->len - phelper->payloadskip_len;
            if ((phelper->recv_flags & EICC_RECV_FLAGS_DATAPEEK) == 0) {
                ipipe_align_fixed(pchannel, eicc_roundup(ppkthdr->len, pchannel->align_bytes) - ppkthdr->len,
                    &phelper->seek);
                pchannel->ipipe_cfg->seek = phelper->seek;
                pchannel->ipipe_cfg->rptr = phelper->seek; /* round check */
                eicc_ipipe_rptr_update(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id),
                    pchannel->ipipe_cfg->rptr);
                eicc_print_trace("eicc_ipipe_rptr_update rptr: 0x%x , wptr: 0x%x\n",
                    (unsigned)pchannel->ipipe_cfg->rptr, (unsigned)*(volatile u32 *)(pchannel->ipipe_cfg->ptr_vaddr));
            }
        } else {
            eicc_print_error("ipipe_get_from_ringbuf failed @%d\n", __LINE__);
        }
    }

    if (ret == 0) {
        return (int)ret_len;
    }
    return -1;
}

int bsp_eicc_chn_recv(eicc_chn_t chn_hdl, void *buf, unsigned len, u32 recv_flags)
{
    int ret;
    unsigned long flags;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    struct eicc_packet pkthdr = { 0 };
    struct eicc_recv_helper helper;
    struct eicc_recv_helper *phelper = &helper;

    if (buf == NULL || len == 0) {
        eicc_print_error("bad param for chn_hdl 0x%x\n", (unsigned)chn_hdl);
        return -1;
    }
    if (eicc_dev_chn_get(EICC_GET_PHYCHN_ID(chn_hdl), &pdev, &pchannel)) {
        eicc_print_error("invaild chn_hdl 0x%x\n", (unsigned)chn_hdl);
        return -1;
    }
    if (pchannel->state != (u32)EICC_CHN_OPEN || !isMsgRecvChn(pchannel)) {
        eicc_print_error("bad chn_hdl 0x%x\n", (unsigned)chn_hdl);
        return -1;
    }
    phelper->buf = buf;
    phelper->len = len;
    phelper->recv_flags = recv_flags;
    spin_lock_irqsave(&pchannel->lock, flags);

    ret = eicc_read_hdr(pchannel, &pkthdr, phelper);
    if (ret) {
        spin_unlock_irqrestore(&pchannel->lock, flags);
        return ret;
    }
    if (phelper->used_len == 0) { /* empty */
        spin_unlock_irqrestore(&pchannel->lock, flags);
        return 0;
    }
    /* 如果这俩相同，则可以认为是第一次读，seek要跳过eicc头 */
    if (pchannel->ipipe_cfg->seek == pchannel->ipipe_cfg->rptr) {
        phelper->seek = phelper->rptr;
        phelper->payloadskip_len = 0;
    } else {
        phelper->seek = pchannel->ipipe_cfg->seek;
        if (phelper->seek > phelper->rptr) {
            phelper->payloadskip_len = phelper->seek - phelper->rptr;
        } else {
            phelper->payloadskip_len = pchannel->ipipe_cfg->depth - phelper->rptr + phelper->seek;
        }
    }

    ret = eicc_read_payload(pdev, pchannel, &pkthdr, phelper);

    spin_unlock_irqrestore(&pchannel->lock, flags);
    return ret;
}

int bsp_eicc_chn_ioctl(eicc_chn_t chn_hdl, unsigned req, void *arg, u32 size)
{
    unsigned long lockflags;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    u32 rptr;
    struct eicc_packet pkthdr = { 0 };
    union opipe_stat o_state;
    ioctl_nxtpkt *pktinfo = (ioctl_nxtpkt *)arg;
    struct eicc_chn_status *status = (struct eicc_chn_status *)arg;

    if (eicc_dev_chn_get(EICC_GET_PHYCHN_ID(chn_hdl), &pdev, &pchannel)) {
        eicc_print_error("invaild chn_hdl 0x%x\n", (unsigned)chn_hdl);
        return -1;
    }
    if (req == EICC_IOCTL_CHN_NXTPKT_INF) {
        if (pktinfo == NULL || size < sizeof(*pktinfo) || !isMsgRecvChn(pchannel)) {
            return -1;
        }
        spin_lock_irqsave(&pchannel->lock, lockflags);
        rptr = pchannel->ipipe_cfg->rptr;
        if (ipipe_get_from_ringbuf(pchannel, (u8 *)&pkthdr, sizeof(pkthdr), &rptr)) {
            pktinfo->len = 0;
            pktinfo->pkthdl = EICC_DFT_INVAILD_VALUE; /* wptr和rptr不可能到这里 */
        } else {
            pktinfo->len = pkthdr.len;
            pktinfo->pkthdl = pchannel->ipipe_cfg->rptr;
        }
        spin_unlock_irqrestore(&pchannel->lock, lockflags);
        return 0;
    } else if (req == EICC_IOCTL_CHN_STATUS_INF) {
        if (status == NULL || size < sizeof(*status) || isMsgRecvChn(pchannel)) {
            return -1;
        }
        o_state.val = eicc_opipe_local_status(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        if (o_state.union_stru.is_empty && o_state.union_stru.is_idle) {
            spin_lock_irqsave(&pchannel->lock, lockflags);
            if (eicc_opipe_wptr_get(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id)) ==
                eicc_opipe_rptr_get(pdev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id))) {
                status->busy = 0;
            } else {
                status->busy = 1;
            }

            spin_unlock_irqrestore(&pchannel->lock, lockflags);
            return 0;
        } else {
            status->busy = 1;
            return 0;
        }
    }
    return -1;
}

static int eicc_data_arrive_event_notify(eicc_event event, struct eicc_channel *pchannel)
{
    u32 rptr;
    eicc_eventinfo eventinfo;

    eventinfo.data_arrv.chnid = pchannel->user_id;
    eventinfo.data_arrv.chn_hdl = (eicc_chn_t)pchannel->ldrvchn_id;

    while (ipipe_used_space_get(pchannel)) {
        rptr = pchannel->ipipe_cfg->rptr;
        ipipe_pkt_ts_set(pchannel);
        if (pchannel->cbk) {
            pchannel->cbk(event, pchannel->cbk_arg, &eventinfo);
        }
        if (pchannel->ipipe_cfg->rptr == rptr) {
            break;
        }
    }

    return 0;
}

static int eicc_send_flowup_event_notify(eicc_event event, struct eicc_channel *pchannel)
{
    eicc_print_crit("eicc_send_flowup_event_notify %d pchannel->user_id 0x%x\n", (int)event,
        (unsigned)pchannel->user_id);
    return 0;
}

static int eicc_fusion_normal_event_notify(u32 inttype, struct eicc_device *pdev, struct eicc_channel *pchannel)
{
    int ret = -1;
    switch (inttype) {
        case EICC_INT_RD_DATA_ARV:
            ret = eicc_data_arrive_event_notify(EICC_EVENT_DATA_ARRV, pchannel);
            break;
        case EICC_INT_WR_OVERFLOW:
            ret = eicc_send_flowup_event_notify(EICC_EVENT_SEND_FLOWUP, pchannel);
            break;
        default:
            eicc_print_crit("inttype 0x%x @dev[%d] is not impl now\n", (unsigned)inttype, (int)pdev->id);
            break;
    }
    return ret;
}
int eicc_normal_event_notify(u32 inttype, struct eicc_device *pdev, u32 pipe_id)
{
    int ret = 0;
    struct eicc_channel *pchannel = NULL;
    if (pipe_id >= EICC_CHN_ID_MAX) {
        eicc_print_error("invaild pipe_id 0x%x\n", (unsigned)pipe_id);
        /* big error */
        return -1;
    }
    pchannel = pdev->channels[pipe_id];
    if (pchannel == NULL) {
        eicc_print_error("no channel relev to pipe_id 0x%x\n", (unsigned)pipe_id);
        return -1;
    }
    pchannel->irq_ts = eicc_get_slice_value();
    ret = eicc_fusion_normal_event_notify(inttype, pdev, pchannel);

    return ret;
}

int eicc_wakeup_event_notify(struct eicc_device *pldev, u32 ipipe_id)
{
    unsigned long flags;
    struct eicc_channel *pchannel = NULL;
    if (ipipe_id >= EICC_CHN_ID_MAX) {
        eicc_print_error("invaild ipipe_id 0x%x\n", (unsigned)ipipe_id);
        /* big error */
        return -1;
    }
    pchannel = pldev->channels[ipipe_id];
    if (pchannel == NULL) {
        eicc_print_error("no channel relev to ipipe_id 0x%x\n", (unsigned)ipipe_id);
        return -1;
    }

    pchannel->irq_ts = eicc_get_slice_value();
    spin_lock_irqsave(&pchannel->lock, flags);
    if (pchannel->state == EICC_CHN_OPEN) {
        eicc_print_trace("ipipe_id 0x%x remote en\n", (unsigned)ipipe_id);
        eicc_ipipe_local_en(pldev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        eicc_ipipe_remote_en(pldev->base_va, EICC_GET_PIPE_ID(pchannel->ldrvchn_id));
        pchannel->remote_awake = 0;
    } else {
        eicc_print_trace("ipipe_id 0x%x remote flag\n", (unsigned)ipipe_id);
        pchannel->remote_awake = 1;
    }
    spin_unlock_irqrestore(&pchannel->lock, flags);
    return 0;
}
