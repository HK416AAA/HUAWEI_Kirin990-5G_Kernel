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

#ifndef __ADS_LTEV_H__
#define __ADS_LTEV_H__

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/skbuff.h>
#include "mdrv_wan.h"
#include "ltev_iface_i.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#if (defined(LLT_OS_VER))
#define STATIC
#else
#define STATIC static
#endif

#define LTEV_FRAG_QUE_MAX_LEN	10
#define LTEV_MAX_PKT_LEN	8188

#define LTEV_DF_MASK		0x03
#define LTEV_MF_MASK		0x00

#define LTEV_LL_RES_SPACE	ETH_HLEN

#define LTEV_ENABLE_IPF_COPY_SHIFT	9

/*
 * fragment queue flags
 *
 * @LTEV_FRAG_FIRST_IN: first fragment has arrived
 * @LTEV_FRAG_LAST_IN: final fragment has arrived
 * @LTEV_FRAG_COMPLETE: frag queue has been processed
 */
enum {
	LTEV_FRAG_FIRST_IN	= BIT(0),
	LTEV_FRAG_LAST_IN	= BIT(1),
	LTEV_FRAG_COMPLETE	= BIT(2),
};

union ltev_desc_u {
	union {
		struct {
			u8 last     :1;
			u8 first    :1;
			u8 seq      :3;
			u8 reserved :3;
		} frag_info;
		u8 frag_value;
	};
	unsigned int field;
};

#define LTEV_RX_SKB_MAP_CB(skb) \
	((struct rx_cb_map_s *)((skb)->cb))

#define LTEV_RX_DESC_CB(skb) \
	((union ltev_desc_u *)&(LTEV_RX_SKB_MAP_CB(skb)->userfield1))

#define LTEV_TX_SKB_MAP_CB(skb) \
	((struct wan_info_s *)((skb)->cb))

#define LTEV_TX_DESC_CB(skb) \
	((union ltev_desc_u *)&(LTEV_TX_SKB_MAP_CB(skb)->userfield1))

struct ltev_frag_queue_s {
	struct sk_buff_head head;
	unsigned int len;
	u8 seq_offset;
	u8 flags;
	u16 max_size;
};

struct ltev_iface_s {
	struct ltev_stats_s stats;

	struct ltev_frag_queue_s fq;
	u8 iface_id;
	u8 reserved[7];
};

/*lint -esym(528,ltev_is_fragment)*/
static inline bool ltev_is_fragment(const union ltev_desc_u *desc)
{
	return (desc->frag_value & LTEV_DF_MASK) != LTEV_DF_MASK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __ADS_LTEV_H__ */