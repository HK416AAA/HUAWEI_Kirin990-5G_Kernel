/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: pcie_nic
 * Author: zhangsanmao
 * Create: 2018-11-30
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
#include "wan_eth_spe.h"
#include "wan_eth_common.h"
#include "bsp_espe.h"
#include "mdrv_wan.h"
#include "mdrv_remote_eth.h"
#include "wan_eth_maa.h"

#define MAX_PORT_NUM 7
#define IPV4V6_MASK 0x3
#define IPV4_MASK 0x1
#define IPV6_MASK 0x2
#define BYPASS_IFACEID_MAX 32
#define MAX_ESPE_PORT_NO 16
#define WETH_PE_THRU 3
#define WETH_MP_THRU 5

struct weth_pport_ctx {
    struct net_device *ndev;
    int weth_port_no;
    int spe_port;
    struct sk_buff *skb_queue_tail;
    int skb_queue_len;

    unsigned int skb_null;
    unsigned int pport_rx_packets;
    unsigned int skb_queue_tail_null;
    unsigned int invalid_port;
    unsigned int send_fail;

    spinlock_t rx_lock;
    spinlock_t tx_lock;
};

struct weth_spe_ctx {
    weth_spe_tx_cb_t tx_cb;
    unsigned int rx_total_packets;
    unsigned int cb_null;
};

struct weth_spe_ctx g_sctx;
struct weth_pport_ctx *g_pport_ctx[MAX_PORT_NUM];

STATIC netdev_tx_t weth_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
    return NETDEV_TX_OK;
}

static const struct net_device_ops g_weth_ops = {
    .ndo_start_xmit = weth_start_xmit,
};

void weth_spe_register_tx_cb(weth_spe_tx_cb_t cb)
{
    g_sctx.tx_cb = cb;
}

static unsigned int weth_spe_get_protocol(struct rx_cb_map_s *map_info)
{
    FUNCTION_START;

    if (!map_info->ipf_result.bits.ip_type) {
        return (unsigned int)htons(ETH_P_IP);
    } else {
        return (unsigned int)htons(ETH_P_IPV6);
    }
}

STATIC void weth_spe_finish_rd(struct sk_buff *skb, unsigned int len, void *param, unsigned int flag)
{
    unsigned int protocol;
    unsigned int dev_id;
    struct rx_cb_map_s *map_info = NULL;
    struct weth_pport_ctx *pport_ctx = (struct weth_pport_ctx *)param;
    unsigned long flags;
    struct weth_spe_ctx *sctx = &g_sctx;

    FUNCTION_START;
    sctx->rx_total_packets++;

    if (unlikely(skb == NULL)) {
        pport_ctx->skb_null++;
        return;
    }

    map_info = (struct rx_cb_map_s *)&skb->cb;

    protocol = weth_spe_get_protocol(map_info);
    dev_id = wan_get_devid(skb);

    map_info->userfield0 = protocol;
    map_info->userfield0 = (map_info->userfield0 << 16) | dev_id;
    map_info->packet_info.bits.unmapped = WETH_EP_SPE_PKG;
    pport_ctx->pport_rx_packets++;

    skb_pull(skb, ETH_HLEN);
    spin_lock_irqsave(&pport_ctx->tx_lock, flags);
    skb_onelink_tail(&pport_ctx->skb_queue_tail, skb);
    pport_ctx->skb_queue_len++;
    spin_unlock_irqrestore(&pport_ctx->tx_lock, flags);

    return;
}

STATIC void weth_spe_rx_complete(void *param)
{
    struct sk_buff *skb_queue_tail = NULL;
    int skb_queue_len;
    struct weth_pport_ctx *pport_ctx = NULL;
    unsigned long flags;
    int ret;

    FUNCTION_START;
    pport_ctx = (struct weth_pport_ctx *)param;

    spin_lock_irqsave(&pport_ctx->tx_lock, flags);
    skb_queue_tail = pport_ctx->skb_queue_tail;
    pport_ctx->skb_queue_tail = NULL;

    skb_queue_len = pport_ctx->skb_queue_len;
    pport_ctx->skb_queue_len = 0;
    spin_unlock_irqrestore(&pport_ctx->tx_lock, flags);

    if (unlikely(skb_queue_tail == NULL)) {
        pport_ctx->skb_queue_tail_null++;
        return;
    }

    if (unlikely(pport_ctx->weth_port_no < 0 || pport_ctx->weth_port_no >= MAX_PORT_NUM)) {
        pport_ctx->invalid_port++;
        __weth_ep_free_skb_list(skb_queue_tail);
        return;
    }

    if (g_sctx.tx_cb != NULL) {
        ret = g_sctx.tx_cb(skb_queue_tail, skb_queue_len, pport_ctx->weth_port_no);
        if (ret) {
            pport_ctx->send_fail++;
        }
    } else {
        g_sctx.cb_null++;
        __weth_ep_free_skb_list(skb_queue_tail);
    }

    return;
}

int weth_spe_get_debug_info(struct weth_debug_info_s *debug_info)
{
    struct weth_spe_ctx *sctx = &g_sctx;
    int i;

    if (g_pport_ctx == NULL) {
        return -1;
    }

    debug_info->spe_rx_total = sctx->rx_total_packets;
    for (i = 0; i < MAX_PORT_NUM; i++) {
        debug_info->spe_info[i].pport_rx_packets = g_pport_ctx[i]->pport_rx_packets;
    }

    return 0;
}

int weth_spe_alloc_port(struct espe_port_comm_attr *weth_attr)
{
    int port_no, i;
    struct weth_pport_ctx *pport_ctx = NULL;

    for (i = 0; i < MAX_PORT_NUM; i++) {
        if (i < WETH_PE_THRU) {
            weth_attr->enc_type = SPE_ENC_DIRECT_FW_PE;
        } else if (i < WETH_MP_THRU) {
            weth_attr->enc_type = SPE_ENC_DIRECT_FW_MP;
        } else {
            weth_attr->enc_type = SPE_ENC_DIRECT_FW_HP;
        }

        pport_ctx = (struct weth_pport_ctx *)kzalloc(sizeof(struct weth_pport_ctx), GFP_KERNEL);
        if (pport_ctx == NULL) {
            weth_pr_err("spe pport_ctx alloc fail\n");
            return -1;
        }

        pport_ctx->ndev = alloc_etherdev(sizeof(struct weth_pport_ctx));
        if (pport_ctx->ndev == NULL) {
            weth_pr_err("alloc_etherdev faild!\n");
            return -1;
        }

        dev_alloc_name(pport_ctx->ndev, "weth_ep%d");
        pport_ctx->ndev->netdev_ops = &g_weth_ops;

        if (register_netdev(pport_ctx->ndev)) {
            weth_pr_err("register_netdev fail\n");
            return -1;
        }

        spin_lock_init(&pport_ctx->tx_lock);
        spin_lock_init(&pport_ctx->rx_lock);

        weth_attr->net = pport_ctx->ndev;
        weth_attr->priv = pport_ctx;
        port_no = bsp_espe_alloc_port(weth_attr);
        if (port_no < 0) {
            weth_pr_err("spe port alloc fail\n");
            return -1;
        }

        pport_ctx->weth_port_no = i;
        pport_ctx->spe_port = port_no;
        g_pport_ctx[i] = pport_ctx;
        (void)bsp_espe_enable_port(port_no);

        pport_ctx = NULL;
    }

    return 0;
}

int weth_spe_init(void)
{
    struct espe_port_comm_attr weth_attr = {0};
    int ret;

    FUNCTION_START;

    weth_attr.td_depth = WETH_SPE_TD_DEPTH;
    weth_attr.rd_depth = WETH_SPE_RD_DEPTH;
    weth_attr.bypassport = 0;
    weth_attr.bypassport_en = 0;
    weth_attr.ops.espe_finish_rd = weth_spe_finish_rd;
    weth_attr.ops.espe_complete_rd = weth_spe_rx_complete;
    weth_attr.ops.espe_finish_td = NULL;
    weth_attr.ops.get_wan_info = NULL;
    weth_attr.ops.espe_netif_rx = NULL;

    ret = weth_spe_alloc_port(&weth_attr);
    if (ret != 0) {
        goto out1;
    }

    return 0;

out1:
    for (i = 0; i < MAX_PORT_NUM; i++) {
        if (g_pport_ctx[i]) {
            kfree(g_pport_ctx[i]);
        }
    }

    return -1;
}

void weth_spe_show_status(void)
{
    int i;
    struct weth_spe_ctx *sctx = &g_sctx;

    weth_pr_err("total rx_packets: %d\n", sctx->rx_total_packets);
    weth_pr_err("cb_null: %d\n", sctx->cb_null);

    for (i = 0; i < MAX_PORT_NUM; i++) {
        struct weth_pport_ctx *pport_ctx = g_pport_ctx[i];

        weth_pr_err("port %d skb_null: %d\n", i, pport_ctx->skb_null);
        weth_pr_err("port %d rx_packets: %d\n", i, pport_ctx->pport_rx_packets);
        weth_pr_err("port %d skb_queue_tail_null: %d\n", i, pport_ctx->skb_queue_tail_null);
        weth_pr_err("port %d invalid_port: %d\n\n", i, pport_ctx->invalid_port);
        weth_pr_err("port %d send_fail: %d\n\n", i, pport_ctx->send_fail);
    }
}

module_init(weth_spe_init);
