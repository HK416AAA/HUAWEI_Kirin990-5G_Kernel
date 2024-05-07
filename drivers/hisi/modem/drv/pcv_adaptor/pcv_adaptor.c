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

#include <linux/skbuff.h>
#include <mdrv_pc5_adaptor.h>
#include <mdrv_pcv.h>

struct g_pcv_ctx_s {
    mdrv_pc5_rx_cb cb2high;
    mdrv_rx_cb  cb2ps;
    unsigned long ul_tx;
    unsigned long ul_fin;
    unsigned long ul_drop;
    unsigned long dl_tx;
    unsigned long dl_fin;
    unsigned long dl_drop;
};

struct g_pcv_ctx_s g_pcv_ctx = {0};

/*
  xmit skb to modem
  return 0-suc, others-fail
*/
int mdrv_pc5_tx(struct sk_buff* skb)
{
    if (skb == NULL)
        return -1;
    g_pcv_ctx.ul_tx++;
    if (g_pcv_ctx.cb2ps) {
        g_pcv_ctx.cb2ps(skb);
        g_pcv_ctx.ul_fin++;
    } else {
        consume_skb(skb);
        g_pcv_ctx.ul_drop++;
        return -1;
    }
    return 0;
}


/*
  register callback for rx.
  return 0-suc, others-fail
*/
int mdrv_pc5_cb_register(mdrv_pc5_rx_cb cb)
{
    if (!cb)
        return -1;
    g_pcv_ctx.cb2high = cb;
    return 0;
}

/**
 * mdrv_pcv_cb_register - callback register
 * @cb: ul rx callback
 * if failed return non-zero
 */
int mdrv_pcv_cb_register(mdrv_rx_cb cb)
{
    if (!cb)
        return -1;
    g_pcv_ctx.cb2ps = cb;
    return 0;
}

/**
 * mdrv_pcv_xmit - transmit a PC5 skbuff to PC5 interface layer
 * @skb: skbuff send to PC5
 * if failed return non-zero
 */
int mdrv_pcv_xmit(struct sk_buff* skb)
{
    g_pcv_ctx.dl_tx++;
    if (skb == NULL)
        return -1;
    if (g_pcv_ctx.cb2high) {
        g_pcv_ctx.cb2high(skb);
        g_pcv_ctx.dl_fin++;
    } else {
        consume_skb(skb);
        g_pcv_ctx.dl_drop++;
        return -1;
    }
    return 0;
}

