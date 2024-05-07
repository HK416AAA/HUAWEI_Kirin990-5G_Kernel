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

#include <net/xfrm.h>
#include <bsp_maa.h>
#include "../espe.h"
#include "../espe_core.h"
#include "../espe_dbg.h"

#define SPE_XFRM_DEBUG_ROW 0x200
#define SPE_XFRM_CHECK_IN 0xAAAAAAAA
#define SPE_XFRM_CHECK_MGAIC 0x55555555

static int g_spe_xfrm_enable = 1;

struct spe_xfrm_debug {
    int sport;
    int dport;
    struct sk_buff *dst;
    struct sk_buff *src;
    struct sec_path *sp;
    int spref;
    int update;
    int idx;
};

int spe_xfrm_set_enable(int enable)
{
    g_spe_xfrm_enable = enable;
    return g_spe_xfrm_enable;
}
static struct spe_xfrm_debug g_spe_xfrm_td[SPE_XFRM_DEBUG_ROW];
static struct spe_xfrm_debug g_spe_xfrm_rd[SPE_XFRM_DEBUG_ROW];
static struct spe_xfrm_debug g_spe_xfrm_free[SPE_XFRM_DEBUG_ROW];

void espe_xfrm_recv(struct sk_buff *dst, u32 dport)
{
    struct spe *spe = &g_espe;

    if (likely(dport != spe->cpuport.portno)) {
        return;
    }

    /*
     * distinguish original skb between and denc ipsec skb
     * we just care skb to cpuport,because skb->sp is useful only for tcp/ip
     * for update_only skb,we must make sure skb->sp->refcnt = 3, otherwise
     * will be crash
     * if update skb after td config.the skb alread free
     */
    if (!IS_ERR_OR_NULL(dst->sp)) {
        if (g_espe.msg_level & SPE_MSG_XFRM) {
            static int idx = 0;
            g_spe_xfrm_rd[idx].dport = dport;
            g_spe_xfrm_rd[idx].sport = SPE_XFRM_CHECK_MGAIC;
            g_spe_xfrm_rd[idx].dst = dst;
            g_spe_xfrm_rd[idx].sp = dst->sp;
            g_spe_xfrm_rd[idx].update = 0;
            g_spe_xfrm_rd[idx].idx = spe->ports[dport].stat.td_xfrm_rcv;
            idx++;
            idx = (idx == SPE_XFRM_DEBUG_ROW) ? 0 : idx;
        }
        spe->ports[dport].stat.td_xfrm_rcv++;
    } else {
        spe->ports[dport].stat.td_xfrm_rcv_fail++;
    }
}

static int __espe_xfrm_xmit(struct spe *spe, struct sk_buff *skb)
{
    static int idx = 0;
    unsigned long long maa_addr = 0;
    unsigned int l2_hdr_len = 0;
    int ret;

    skb_push(skb, l2_hdr_len);
    if (g_espe.msg_level & SPE_MSG_XFRM) {
        g_spe_xfrm_td[idx].sport = skb->dev->spe_portno;
        g_spe_xfrm_td[idx].src = skb;
        g_spe_xfrm_td[idx].dport = SPE_XFRM_CHECK_MGAIC;
        g_spe_xfrm_td[idx].sp = skb->sp;
        g_spe_xfrm_td[idx].idx = spe->ports[skb->dev->spe_portno].stat.td_xfrm_config;
        idx++;
        idx = (idx == SPE_XFRM_DEBUG_ROW) ? 0 : idx;
    }
    spe->ports[skb->dev->spe_portno].stat.td_xfrm_config++;

    if (g_spe_xfrm_enable) {
        /* call bsp_espe_config_td */
        maa_addr = bsp_maa_skb_map(skb, l2_hdr_len);
        if (maa_addr) {
            ret = bsp_espe_config_td(skb->dev->spe_portno, maa_addr - l2_hdr_len, skb->len, (void *)skb,
                                     ESPE_TD_KICK_PKT);
            if (ret) {
                bsp_maa_skb_unmap(maa_addr);
                dma_unmap_single(spe->dev, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);
            }
        } else {
            SPE_ERR_ONCE("skb map failed\n");
            spe->ports[skb->dev->spe_portno].stat.td_xfrm_config_fail++;
            skb_pull(skb, l2_hdr_len);
            return -EAGAIN;
        }
    }

    if (ret) {
        if (g_espe.msg_level & SPE_MSG_XFRM) {
            SPE_ERR("%s xfrm td config failed \n", __func__);
        }
        spe->ports[skb->dev->spe_portno].stat.td_xfrm_config_fail++;
        skb_pull(skb, ETH_HLEN);
        return ret;
    }
    return 0;
}

int espe_xfrm_xmit(void *input_skb)
{
    struct spe *spe = &g_espe;
    struct sk_buff *skb = (struct sk_buff *)input_skb;
    struct ethhdr *eth_hdr = NULL;
    int ret = -EAGAIN;

    if (unlikely(skb == NULL || skb->dev == NULL || skb->data == NULL)) {
        return ret;
    }

    eth_hdr = (struct ethhdr *)skb->data;
    if (eth_hdr->h_proto == ETH_P_IP || eth_hdr->h_proto == ETH_P_IPV6) {
        l2_hdr_len = ETH_HLEN;
    } else {
        return ret;
    }

    if (skb->dev->spe_portno == spe->ipfport.portno) {
        return ret;
    }

    ret = __espe_xfrm_xmit(spe, skb);
    if (ret) {
        return ret;
    }

    spe->xfrm_dfs_cnt++;
    return ret;
}

int espe_xfrm_free_sp(struct sk_buff *skb, u32 dport)
{
    struct spe *spe = &g_espe;
    /* just ipsec skb */
    if (!skb || IS_ERR_OR_NULL(skb->sp) || !skb->dev) {
        return -EAGAIN;
    }

    /* espe_xfrm_recv secpath_put,for update_only skb,not set sp */
    if (g_espe.msg_level & SPE_MSG_XFRM) {
        static int idx = 0;
        g_spe_xfrm_free[idx].dport = dport;
        g_spe_xfrm_free[idx].sport = SPE_XFRM_CHECK_MGAIC;
        g_spe_xfrm_free[idx].src = skb;
        g_spe_xfrm_free[idx].sp = skb->sp;
        g_spe_xfrm_free[idx].idx = spe->ports[skb->dev->spe_portno].stat.td_xfrm_free_skb;
        g_spe_xfrm_free[idx].update = 0;
        idx++;
        idx = (idx == SPE_XFRM_DEBUG_ROW) ? 0 : idx;
    }
    spe->ports[skb->dev->spe_portno].stat.td_xfrm_free_skb++;
    if (dport != spe->cpuport.portno) {
        secpath_put(skb->sp);
    }

    return 0;
}
