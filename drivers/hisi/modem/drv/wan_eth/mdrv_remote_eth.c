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

#include "mdrv_remote_eth.h"
#include "wan_eth_common.h"

#ifdef CONFIG_BALONG_WETH_DEV
#include "wan_eth_ep.h"
#include "mdrv_wan.h"
#include "bsp_espe.h"
#endif
#ifdef CONFIG_BALONG_WETH
#include "wan_eth_rc.h"
#endif

struct remote_eth_callback_s g_remote_eth_cb;
struct remote_eth_callback_s *weth_get_remote_eth_cb(void)
{
    return &g_remote_eth_cb;
}

int mdrv_remote_eth_callback_register(struct remote_eth_callback_s *cb)
{
    if (cb == NULL) {
        weth_pr_err("cb is null !\n");
        return -1;
    }

    g_remote_eth_cb.rx_data = cb->rx_data;
    g_remote_eth_cb.rx_complete = cb->rx_complete;

    return 0;
}

int mdrv_remote_eth_tx(struct sk_buff_head *skb_list)
{
    struct sk_buff *skb = NULL;
    struct sk_buff *cur_skb = NULL;
    struct weth_ep_ctx *pctx = weth_get_ep_ctx();
    struct espe_direct_fw_entry_ctx ctx = {0};
    struct rx_cb_map_s *cb_map = NULL;
    unsigned int devid;
    unsigned int packet_num = 0;
    int ret;

    if (unlikely(skb_list == NULL || skb_list->qlen == 0)) {
        pctx->tx_stats.remote_eth_skblist_null++;
        return -1;
    }

    while ((cur_skb = __skb_dequeue(skb_list))) {
        packet_num++;
        pctx->tx_stats.remote_eth_send++;
        devid = wan_get_devid(cur_skb);
        ctx.devid = devid;
        ctx.skb_cb_info = 1;
        ctx.add_wan_info = 0;
        ctx.l2_hdr_len = 0;
        ctx.maped = 1;

        bsp_espe_add_direct_fw_entry(cur_skb, &ctx);

        cb_map = (struct rx_cb_map_s *)&cur_skb->cb;
        cb_map->packet_info.bits.unmapped = WETH_EP_CPU_PKG;
        skb_onelink_tail(&skb, cur_skb);
    }

    ret = weth_ep_submit_tx(skb, packet_num, WETH_CPU_PORT_NO);
    if (ret) {
        pctx->tx_stats.remote_eth_fail++;
        pctx->tx_stats.remote_eth_drop += packet_num;
    }
    return 0;
}
