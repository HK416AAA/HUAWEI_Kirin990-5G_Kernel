/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include "mdrv_pcv.h"
#include "securec.h"
#include "ads_dev_i.h"
#include "ltev_iface.h"
#include "product_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

struct ltev_iface_s ltev_iface_ctx;

#if (FEATURE_PC5_DATA_CHANNEL == FEATURE_ON)

/*lint -save -e801*/
STATIC int ltev_fragment(struct ltev_iface_s *priv, struct sk_buff *skb,
			 unsigned int mtu,
			 int (*output)(struct ltev_iface_s *, struct sk_buff *))
{
	struct sk_buff *skb2 = NULL;
	union ltev_desc_u *desc = NULL;
	unsigned int left, len, offset;
	int ret = 0;
	u8 seq_id;

	left = skb->len;
	offset = 0;
	seq_id = 0;

	while (left > 0) {
		len = left;
		if (len > mtu)
			len = mtu;
		/* Align the next start on an eight byte boundary. */
		if (len < left)
			len &= ~7;

		/*
		 * Alloc new buffer.
		 */
		skb2 = netdev_alloc_skb_ip_align(NULL, len + LTEV_LL_RES_SPACE);
		if (!skb2) {
			ret = -ENOMEM;
			priv->stats.frag_alloc_errors++;
			goto err;
		}

		/*
		 * Set up data on packet.
		 */
		skb_reserve(skb2, LTEV_LL_RES_SPACE);
		skb_put(skb2, len);

		/*
		 * Copy data into the new buffer.
		 */
		skb_copy_from_linear_data_offset(skb, (int)offset, skb2->data, len);

		left -= len;

		desc = LTEV_TX_DESC_CB(skb2);
		desc->frag_info.first = (offset == 0) ? 1 : 0;
		desc->frag_info.last = (left == 0) ? 1 : 0;;
		desc->frag_info.seq = seq_id++;

		offset += len;

		ret = output(priv, skb2);
		if (ret) {
			priv->stats.frag_output_errors++;
			goto err;
		}
	}

	dev_consume_skb_any(skb);
	priv->stats.frag_oks++;
	return 0;

err:
	dev_kfree_skb_any(skb); /* 由操作系统接管 */
	priv->stats.frag_fails++;
	return ret;
}
/*lint -restore +e801*/

/*lint -save -e801*/
STATIC int ltev_output2(struct ltev_iface_s *priv, struct sk_buff *skb)
{
	int ret = 0;

	if (skb_headroom(skb) < LTEV_LL_RES_SPACE) {
		if (pskb_expand_head(skb, LTEV_LL_RES_SPACE, 0, GFP_ATOMIC)) {
			ret = -ENOMEM;
			priv->stats.tx_headroom_errors++;
			goto err_headromm;
		}
	}

	skb->protocol = 0;

#ifdef MODEM_FUSION_VERSION
	LTEV_TX_SKB_MAP_CB(skb)->userfield1 |= BIT(LTEV_ENABLE_IPF_COPY_SHIFT);
#endif

	ret = ads_iface_tx_ltev(priv->iface_id, skb);
	if (ret) {
		priv->stats.tx_link_errors++;
		goto err;
	}

	priv->stats.tx_packets++;
	return 0;

err_headromm:
	dev_kfree_skb_any(skb);
err:
	priv->stats.tx_dropped++;
	return ret;
}
/*lint -restore +e801*/

/*lint -save -e801*/
STATIC int ltev_output(struct ltev_iface_s *priv, struct sk_buff *skb)
{
	union ltev_desc_u *desc = NULL;
	unsigned int mtu = ETH_DATA_LEN;
	int ret = 0;

	priv->stats.tx_total_packets++;

	if (unlikely(skb->len == 0 || skb->len > LTEV_MAX_PKT_LEN)) {
		ret = -EINVAL;
		priv->stats.tx_length_errors++;
		goto err_free;
	}

	if (skb->len > mtu) {
		ret = ltev_fragment(priv, skb, mtu, ltev_output2);
		if (ret) {
			priv->stats.tx_big_dropped++;
			goto err;
		}

		priv->stats.tx_big_packets++;
		goto out;
	}

	desc = LTEV_TX_DESC_CB(skb);
	desc->frag_info.first = 1;
	desc->frag_info.last = 1;
	desc->frag_info.seq = 0;

	ret = ltev_output2(priv, skb);
	if (ret) {
		priv->stats.tx_norm_dropped++;
		goto err;
	}

	priv->stats.tx_norm_packets++;
out:
	return 0;

err_free:
	dev_kfree_skb_any(skb); /* 由操作系统接管 */
	priv->stats.tx_dropped++;
err:
	return ret;
}
/*lint -restore +e801*/

STATIC int ltev_xmit(struct sk_buff *skb)
{
	return ltev_output(&ltev_iface_ctx, skb);
}

STATIC void ltev_frag_flush(struct ltev_frag_queue_s *fq)
{
	struct ltev_iface_s *priv = container_of(fq, struct ltev_iface_s, fq);
	u32 num = skb_queue_len(&fq->head);

	__skb_queue_purge(&fq->head);
	fq->len = 0;
	fq->seq_offset = 0;
	fq->flags = 0;

	priv->stats.rx_frag_dropped += num;
}

/*lint -save -e801*/
STATIC struct sk_buff *ltev_frag_reasm(struct ltev_frag_queue_s *fq)
{
	struct ltev_iface_s *priv = container_of(fq, struct ltev_iface_s, fq);
	struct sk_buff *skb = NULL;
	struct sk_buff *skb2 = NULL;
	unsigned int offset = 0;
	u32 num = 0;

	/*
	 * Alloc new buffer.
	 */
	skb2 = netdev_alloc_skb_ip_align(NULL, fq->len + LTEV_LL_RES_SPACE);
	if (skb2 == NULL) {
		priv->stats.reasm_alloc_errors++;
		goto err;
	}
	/*
	 * Set up data on packet.
	 */
	skb_reserve(skb2, LTEV_LL_RES_SPACE);
	skb_put(skb2, fq->len);

	/*
	 * Copy data into the new buffer.
	 */
	while ((skb = __skb_dequeue(&fq->head))) {
		skb_copy_to_linear_data_offset(skb2, (int)offset, skb->data, skb->len);
		offset += skb->len;
		dev_kfree_skb_any(skb);
		num++;
	}

	fq->len = 0;
	fq->flags = 0;
	fq->seq_offset = 0;
	priv->stats.reasm_consume += num;
	priv->stats.reasm_oks++;
	return skb2;

err:
	ltev_frag_flush(fq);
	priv->stats.reasm_fails++;
	return NULL;
}
/*lint -restore +e801*/

/*lint -save -e801*/
STATIC int ltev_frag_queue(struct ltev_frag_queue_s *fq, struct sk_buff *skb)
{
	struct ltev_iface_s *priv = container_of(fq, struct ltev_iface_s, fq);
	union ltev_desc_u *desc = LTEV_RX_DESC_CB(skb);

again:
	if (fq->flags & LTEV_FRAG_COMPLETE) {
		ltev_frag_flush(fq);
		priv->stats.reasm_fails++;
	}

	/* Check if the sequence number of this fragment is expected. */
	if (fq->seq_offset != desc->frag_info.seq) {
		priv->stats.reasm_seq_errors++;
		goto err;
	}

	/* The first fragment? */
	if ((desc->frag_value & 0x02) == 0x02) {
		if (fq->flags) {
			fq->flags |= LTEV_FRAG_COMPLETE;
			goto again;
		}

		fq->flags |= LTEV_FRAG_FIRST_IN;
	}

	if (!(fq->flags & LTEV_FRAG_FIRST_IN)) {
		priv->stats.reasm_order_errors++;
		goto err;
	}

	/* The last fragment? */
	if ((desc->frag_value & 0x03) == 0x01)
		fq->flags |= LTEV_FRAG_LAST_IN;

	/* Check if queue reaches max size. */
	if (skb_queue_len(&fq->head) >= fq->max_size) {
		priv->stats.reasm_queue_errors++;
		goto err;
	}

	/* Insert fragment. */
	__skb_queue_tail(&fq->head, skb);
	fq->len += skb->len;
	fq->seq_offset++;

	return 0;

err:
	dev_kfree_skb_any(skb); /* 由操作系统接管 */
	ltev_frag_flush(fq);
	priv->stats.reasm_fails++;
	return -EINVAL;
}
/*lint -restore +e801*/

/*lint -save -e801*/
STATIC int ltev_frag_rcv(struct ltev_iface_s *priv, struct sk_buff *skb,
			 int (*input)(struct ltev_iface_s *, struct sk_buff *))
{
	struct ltev_frag_queue_s *fq = &priv->fq;
	struct sk_buff *skb2 = NULL;
	int ret = 0;

	priv->stats.rx_frag_packets++;

	ret = ltev_frag_queue(fq, skb);
	if (ret)
		goto err;

	if (fq->flags == (LTEV_FRAG_FIRST_IN | LTEV_FRAG_LAST_IN)) {
		skb2 = ltev_frag_reasm(fq);
		if (skb2 == NULL) {
			ret = -ENOMEM;
			goto err;
		}

		ret = input(priv, skb2);
		if (ret) {
			priv->stats.reasm_input_errors++;
			goto err;
		}
	}

	return 0;

err:
	return ret;
}
/*lint -restore +e801*/

STATIC int ltev_input2(struct ltev_iface_s *priv, struct sk_buff *skb)
{
	int ret;

	ret = mdrv_pcv_xmit(skb);
	if (ret)
		priv->stats.rx_link_errors++;

	return ret;
}

/*lint -save -e801*/
STATIC int ltev_input(struct ltev_iface_s *priv, struct sk_buff *skb)
{
	int ret = 0;

	priv->stats.rx_total_packets++;

	if (ltev_is_fragment(LTEV_RX_DESC_CB(skb)))
		return ltev_frag_rcv(priv, skb, ltev_input2);

	ret = ltev_input2(priv, skb);
	if (ret) {
		priv->stats.rx_norm_dropped++;
		goto err;
	}

	priv->stats.rx_norm_packets++;
	return 0;

err:
	return ret;
}
/*lint -restore +e801*/

STATIC int ltev_rcv_data(unsigned long user_data, struct sk_buff *skb)
{
	return ltev_input(&ltev_iface_ctx, skb);
}

void ltev_create(unsigned char iface_id)
{
	struct ltev_iface_s *priv = &ltev_iface_ctx;
	struct ads_iface_rx_handler_s rx_handler = {0};

	(void)memset_s(priv, sizeof(*priv), 0, sizeof(*priv));

	skb_queue_head_init(&priv->fq.head);
	priv->fq.max_size = LTEV_FRAG_QUE_MAX_LEN;
	priv->iface_id = iface_id;

	(void)memset_s(&rx_handler, sizeof(rx_handler), 0, sizeof(rx_handler));
	rx_handler.user_data = (uintptr_t)priv;
	rx_handler.rx_func = ltev_rcv_data;

	ads_iface_register_rx_handler(priv->iface_id, &rx_handler);

	mdrv_pcv_cb_register(ltev_xmit);
}

void ltev_show_stats(void)
{
	struct ltev_stats_s *stats = &ltev_iface_ctx.stats;

	pr_err("rx_total_packets               %10u\n", stats->rx_total_packets);
	pr_err("tx_total_packets               %10u\n", stats->tx_total_packets);

	pr_err("rx_norm_packets                %10u\n", stats->rx_norm_packets);
	pr_err("rx_frag_packets                %10u\n", stats->rx_frag_packets);
	pr_err("rx_packets                     %10u\n", stats->rx_packets);
	pr_err("rx_norm_dropped                %10u\n", stats->rx_norm_dropped);
	pr_err("rx_frag_dropped                %10u\n", stats->rx_frag_dropped);
	pr_err("rx_dropped                     %10u\n", stats->rx_dropped);

	pr_err("tx_norm_packets                %10u\n", stats->tx_norm_packets);
	pr_err("tx_big_packets                 %10u\n", stats->tx_big_packets);
	pr_err("tx_packets                     %10u\n", stats->tx_packets);
	pr_err("tx_norm_dropped                %10u\n", stats->tx_norm_dropped);
	pr_err("tx_big_dropped                 %10u\n", stats->tx_big_dropped);
	pr_err("tx_dropped                     %10u\n", stats->tx_dropped);

	pr_err("reasm_oks                      %10u\n", stats->reasm_oks);
	pr_err("reasm_fails                    %10u\n", stats->reasm_fails);
	pr_err("reasm_consume                  %10u\n", stats->reasm_consume);
	pr_err("reasm_seq_errors               %10u\n", stats->reasm_seq_errors);
	pr_err("reasm_order_errors             %10u\n", stats->reasm_order_errors);
	pr_err("reasm_queue_errors             %10u\n", stats->reasm_queue_errors);
	pr_err("reasm_alloc_errors             %10u\n", stats->reasm_alloc_errors);
	pr_err("reasm_input_errors             %10u\n", stats->reasm_input_errors);

	pr_err("frag_oks                       %10u\n", stats->frag_oks);
	pr_err("frag_fails                     %10u\n", stats->frag_oks);
	pr_err("frag_alloc_errors              %10u\n", stats->frag_alloc_errors);
	pr_err("frag_output_errors             %10u\n", stats->frag_output_errors);

	pr_err("rx_link_errors                 %10u\n", stats->rx_link_errors);

	pr_err("tx_length_errors               %10u\n", stats->tx_length_errors);
	pr_err("tx_headroom_errors             %10u\n", stats->tx_headroom_errors);
	pr_err("tx_link_errors                 %10u\n", stats->tx_link_errors);
}

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */