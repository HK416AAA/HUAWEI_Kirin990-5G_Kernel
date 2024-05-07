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

#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/jhash.h>
#include <linux/kernel.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netdevice.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#include <net/ip.h>

#include <securec.h>
#include <mdrv_wan.h>
#include <bsp_espe.h>
#include <bsp_maa.h>
#include "../espe_ip_entry.h"
#include "../espe_dbg.h"
#include "espe_direct_fw.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define ESPE_DFW_DL_TO_LAN 0x444c414e
#define ESPE_DFW_UL_TO_WAN 0x5557414e

#ifdef ESPE_RECORD_WAN
struct wan_dev_info_s *espe_direct_fw_find_wan_dev(struct net_device *dev)
{
    struct wan_dev_entry *entry = NULL;
    struct spe *spe = &g_espe;

    list_for_each_entry(entry, &spe->wanport.wan_dev_list, list)
    {
        if (entry->info.dev == dev) {
            return &entry->info;
        }
    }
    return NULL;
}

int espe_direct_fw_record_wandev(struct wan_dev_info_s *wan_dev_info)
{
    struct spe *spe = &g_espe;
    struct wan_dev_entry *ptr = NULL;
    int ret;

    if (!spe->direct_fw.dfw_flags.enable) {
        return 0;
    }

    if (wan_dev_info == NULL) {
        return -EINVAL;
    }

    list_for_each_entry(ptr, &spe->direct_fw.wan_dev_list, list)
    {
        if (ptr->info.dev == wan_dev_info->dev) {
            ret = memcpy_s(&ptr->info, sizeof(ptr->info), wan_dev_info, sizeof(*wan_dev_info));
            if (ret) {
                SPE_ERR("memset_s failed\n");
            }
            return 0;
        }
    }

    ptr = kmalloc(sizeof(struct wan_dev_entry), GFP_KERNEL);
    if (ptr == NULL) {
        return -ENOMEM;
    }

    ret = memcpy_s(&ptr->info, sizeof(ptr->info), wan_dev_info, sizeof(*wan_dev_info));
    if (ret) {
        SPE_ERR("memset_s failed\n");
    }
    list_add(&ptr->list, &spe->direct_fw.wan_dev_list);

    return 0;
}

int spe_wan_dev_dump(void)
{
    struct spe *spe = &g_espe;
    struct wan_dev_entry *ptr = NULL;

    list_for_each_entry(ptr, &spe->direct_fw.wan_dev_list, list)
    {
        if (ptr != NULL) {
            if ((ptr->info.dev != NULL) && (ptr->info.dev->name != NULL)) {
                bsp_err("dev_name   %s\n", ptr->info.dev->name);
            } else {
                bsp_err("dev_name   NULL! \n");
            }
            bsp_err("ipv4 info  \n");
            bsp_err("pdu_session_id   %u\n", ptr->info.v4.info.pdu_session_id);
            bsp_err("fc_head   %u\n", ptr->info.v4.info.fc_head);
            bsp_err("modem_id   %u\n", ptr->info.v4.info.modem_id);
            bsp_err("higi_pri_flag   %u\n", ptr->info.v4.info.higi_pri_flag);
            bsp_err("parse_en   %u\n", ptr->info.v4.info.parse_en);
            bsp_err("userfield0   %x\n", ptr->info.v4.userfield0);
            bsp_err("userfield1   %x\n", ptr->info.v4.userfield1);
            bsp_err("userfield2   %x\n", ptr->info.v4.userfield2);

            bsp_err("ipv6 info  \n");
            bsp_err("pdu_session_id   %u\n", ptr->info.v6.info.pdu_session_id);
            bsp_err("fc_head   %u\n", ptr->info.v6.info.fc_head);
            bsp_err("modem_id   %u\n", ptr->info.v6.info.modem_id);
            bsp_err("higi_pri_flag   %u\n", ptr->info.v6.info.higi_pri_flag);
            bsp_err("parse_en   %u\n", ptr->info.v6.info.parse_en);
            bsp_err("userfield0   %x\n", ptr->info.v6.userfield0);
            bsp_err("userfield1   %x\n", ptr->info.v6.userfield1);
            bsp_err("userfield2   %x\n", ptr->info.v6.userfield2);
        } else {
            bsp_err("wan dev NULL!\n");
        }
    }
    return 0;
}

void espe_direct_fw_write_wan_info(struct spe *spe, struct wan_dev_info_s *wan_dev_info, struct espe_ip_fw_entry *entry)
{
    struct wan_info_s *wan_info = NULL;

    if (entry->iptype) {
        wan_info = &wan_dev_info->v6;
    } else {
        wan_info = &wan_dev_info->v4;
    }

    entry->pdu_ssid = wan_info->info.pdu_session_id;
    entry->fc_head = wan_info->info.fc_head;
    entry->modem_id = wan_info->info.modem_id;
    entry->htab_usrfield_lower16 = (unsigned short)(wan_info->userfield0);
    entry->htab_usrfield_higher16 = (unsigned short)(wan_info->userfield0 >> 16);
    entry->priv->wan_dev = wan_dev_info->dev;

    if (spe->msg_level & SPE_MSG_TUPLE) {
        SPE_ERR("pdu_ssid 0x%x fc_head 0x%x modem_id 0x%x userfield0 0x%x \n", wan_info->info.pdu_session_id,
                wan_info->info.fc_head, wan_info->info.modem_id, wan_info->userfield0);
    }
}

int espe_direct_fw_record_add_waninfo(struct espe_ip_fw_entry *entry)
{
    struct wan_dev_info_s *wan_info;

    wan_info = espe_direct_fw_find_wan_dev(NULL);
    if (wan_info != NULL) {
        entry->priv->dst_net = ESPE_DFW_UL_TO_WAN;
        espe_direct_fw_write_wan_info(spe, wan_info, entry);
    }

    spe->direct_fw.no_wan_info++;
    return -EINVAL;
}

#else
int espe_direct_fw_record_add_waninfo(struct espe_ip_fw_entry *entry)
{
    return 0;
}
int espe_direct_fw_record_wandev(struct wan_dev_info_s *wan_dev_info)
{
    return 0;
}
#endif

int espe_make_ipv4_tuple(struct sk_buff *skb, struct nf_conntrack_tuple *tuple, unsigned int l3_proto)
{
    struct spe *spe = &g_espe;
    struct iphdr *iph = NULL;
    struct udphdr *udph = NULL;
    struct tcphdr *tcph = NULL;

    iph = ip_hdr(skb);
    if (iph->ihl != 5) {
        spe->direct_fw.iph_len_err++;
        return -EINVAL;
    }

    if (ip_is_fragment(iph)) {
        spe->direct_fw.iph_frag++;
        return -EINVAL;
    }
    skb->transport_header = skb->network_header + iph->ihl * 4;

    tuple->dst.protonum = l3_proto;
    tuple->dst.dir = IP_CT_DIR_ORIGINAL;
    tuple->dst.u3.ip = iph->daddr;
    tuple->src.u3.ip = iph->saddr;
    tuple->src.l3num = AF_INET;  // AF_INET=2 as ipv4, AF_INET6=10 as ipv6

    if (l3_proto == IPPROTO_TCP) {
        tcph = tcp_hdr(skb);
        tuple->src.u.tcp.port = tcph->source;
        tuple->dst.u.tcp.port = tcph->dest;
    } else if (l3_proto == IPPROTO_UDP) {
        udph = udp_hdr(skb);
        tuple->src.u.udp.port = udph->source;
        tuple->dst.u.udp.port = udph->dest;
    } else {
        spe->direct_fw.ipproto_err++;
        return -EINVAL;
    }
    return 0;
}

int espe_make_ipv6_tuple(struct sk_buff *skb, struct nf_conntrack_tuple *tuple, unsigned int l3_proto)
{
    struct spe *spe = &g_espe;
    struct ipv6hdr *hdr = NULL;
    struct udphdr *udph = NULL;
    struct tcphdr *tcph = NULL;
    int ret;

    hdr = ipv6_hdr(skb);

    tuple->dst.protonum = l3_proto;
    tuple->dst.dir = IP_CT_DIR_ORIGINAL;

    ret = memcpy_s(tuple->src.u3.ip6, sizeof(tuple->src.u3.ip6), &hdr->saddr, sizeof(hdr->saddr));
    if (ret) {
        SPE_ERR_ONCE("memcpy ipv6 src addr fail \n");
        return -EINVAL;
    }
    ret = memcpy_s(tuple->dst.u3.ip6, sizeof(tuple->dst.u3.ip6), &hdr->daddr, sizeof(hdr->daddr));
    if (ret) {
        SPE_ERR_ONCE("memcpy ipv6 dst addr fail \n");
        return -EINVAL;
    }

    tuple->src.l3num = AF_INET6;  // AF_INET=2 as ipv4, AF_INET6=10 as ipv6

    if (l3_proto == IPPROTO_TCP) {
        skb->transport_header = skb->network_header + sizeof(*hdr);
        tcph = tcp_hdr(skb);
        tuple->src.u.tcp.port = tcph->source;
        tuple->dst.u.tcp.port = tcph->dest;
    } else if (l3_proto == IPPROTO_UDP) {
        skb->transport_header = skb->network_header + sizeof(*hdr);
        udph = udp_hdr(skb);
        tuple->src.u.udp.port = udph->source;
        tuple->dst.u.udp.port = udph->dest;
    } else {
        spe->direct_fw.ipproto_err++;
        return -EINVAL;
    }

    return 0;
}

static int espe_direct_fw_add_tuple(struct sk_buff *skb, struct espe_ip_fw_entry *entry,
                                    union packet_info_u *packet_info)
{
    struct nf_conntrack_tuple *tuple = &(entry->tuple);
    int ret;

    if ((packet_info->bits.ip_proto == AF_INET) &&
        ((packet_info->bits.l4_proto == IPPROTO_TCP) || (packet_info->bits.l4_proto == IPPROTO_UDP))) {
        ret = espe_make_ipv4_tuple(skb, tuple, packet_info->bits.l4_proto);
        if (ret) {
            return -EINVAL;
        }
        entry->iptype = 0;
    } else if ((packet_info->bits.ip_proto == AF_INET6) &&
               ((packet_info->bits.l4_proto == IPPROTO_TCP) || (packet_info->bits.l4_proto == IPPROTO_UDP))) {
        ret = espe_make_ipv6_tuple(skb, tuple, packet_info->bits.l4_proto);
        if (ret) {
            return -EINVAL;
        }
        entry->iptype = 1;
    } else {
        return -EINVAL;
    }
    entry->action = SPE_NF_NAT_MANIP_NONE;
    entry->nat_src_ip = 0;
    entry->nat_src_port = 0;
    entry->nat_dst_ip = 0;
    entry->nat_dst_port = 0;

    return ret;
}

unsigned int espe_entry_get_port(struct spe *spe, struct espe_ip_fw_entry *entry)
{
    unsigned int i;
    unsigned int most_idle_port = 0;
    unsigned int cur_port;
    unsigned int busy_level = 0;
    unsigned int cur_busy_level;
    struct espe_direct_fw_ctx *dir_fw_ctx = NULL;

    if (spe->direct_fw.port_cnt == 0) {
        espe_bug(spe);
        return spe->cpuport.portno;
    }

    for (i = 0; i < spe->direct_fw.port_cnt; i++) {
        cur_port = spe->direct_fw.port_nums[i];
        dir_fw_ctx = &spe->ports[cur_port].ctrl.direct_fw_ctx;
        if (spe->ports[cur_port].ctrl.port_flags.enable) {
            cur_busy_level = dir_fw_ctx->port_weight / dir_fw_ctx->stream_cnt;
            if (busy_level < cur_busy_level) {
                busy_level = cur_busy_level;
                most_idle_port = cur_port;
            }
        }
    }

    spe->ports[most_idle_port].ctrl.direct_fw_ctx.stream_cnt++;
    return most_idle_port;
}

void espe_get_ipf_mac_addr(struct spe *spe)
{
    unsigned char *mac = NULL;
    unsigned int mac_lo;
    unsigned int mac_hi;
    int ret;

    mac = spe->ipfport.ipf_eth_head.h_dest;
    mac_lo = spe_readl_relaxed(spe->regs, SPE_GLB_DMAC_ADDR_L);
    mac_hi = spe_readl_relaxed(spe->regs, SPE_GLB_DMAC_ADDR_H);

    ret = memcpy_s(mac, sizeof(mac_lo), &mac_lo, sizeof(mac_lo));
    if (ret != EOK) {
        SPE_ERR("<espe_get_ipf_mac> memcpy_s fail!\n");
    }
    ret = memcpy_s(mac + sizeof(mac_lo), ESPE_MAC_ADDR_LEFT_LEN, &mac_hi, ESPE_MAC_ADDR_LEFT_LEN);
    if (ret != EOK) {
        SPE_ERR("<espe_get_ipf_mac> memcpy_s fail!\n");
    }

    mac = spe->ipfport.ipf_eth_head.h_source;
    mac_lo = spe_readl_relaxed(spe->regs, SPE_GLB_SMAC_ADDR_L);
    mac_hi = spe_readl_relaxed(spe->regs, SPE_GLB_SMAC_ADDR_H);

    ret = memcpy_s(mac, sizeof(mac_lo), &mac_lo, sizeof(mac_lo));
    if (ret != EOK) {
        SPE_ERR("<espe_get_ipf_mac> memcpy_s fail!\n");
    }
    ret = memcpy_s(mac + sizeof(mac_lo), ESPE_MAC_ADDR_LEFT_LEN, &mac_hi, ESPE_MAC_ADDR_LEFT_LEN);
    if (ret != EOK) {
        SPE_ERR("<espe_get_ipf_mac> memcpy_s fail!\n");
    }

    return;
}

void espe_set_mac_addr(struct spe *spe, struct espe_ip_fw_entry *entry)
{
    unsigned char *mac_hd = NULL;
    int ret;
    static unsigned int flag = 0;

    if (unlikely(flag == 0) && (spe->spe_version == ESPE_VER_V300)) {
        espe_get_ipf_mac_addr(spe);
        flag = 1;
    }

    mac_hd = (char *)(&spe->ipfport.ipf_eth_head);
    ret = memcpy_s(entry->shost, sizeof(entry->shost), mac_hd + ETH_ALEN, ETH_ALEN);
    if (ret != EOK) {
        SPE_ERR("espe_set_mac_addr, memcpy_s fail \n");
    }
    ret = memcpy_s(entry->dhost, sizeof(entry->dhost), mac_hd, ETH_ALEN);
    if (ret != EOK) {
        SPE_ERR("espe_set_mac_addr, memcpy_s fail \n");
    }
    return;
}

static void espe_set_direct_fw_entry(struct spe *spe, struct espe_direct_fw_entry_ctx *ctx,
                                     struct espe_ip_fw_entry *entry, unsigned int hash, unsigned int hkey)
{
    // write mac here
    espe_set_mac_addr(spe, entry);

    entry->vid = 0;
    entry->usb_net_id = ctx->net_id;
    entry->priv->dir = 0;
    entry->portno = espe_entry_get_port(spe, entry);
    entry->timestamp = spe_readl(spe->regs, SPE_TIME);
    entry->priv->hash = hash;
    entry->priv->hkey = hkey;
    if (ctx->add_wan_info) {
        espe_direct_fw_record_add_waninfo(entry);
    } else {
        entry->priv->dst_net = ESPE_DFW_DL_TO_LAN;
        entry->htab_usrfield_higher16 = (unsigned short)reciprocal_scale(hash, 0xffff);
        entry->htab_usrfield_lower16 = (unsigned short)(ctx->devid);
    }
    entry->action = 0;  // noaction

    return;
}

int espe_add_direct_fw_entry(struct spe *spe, struct sk_buff *skb, unsigned long long ip_phy_addr,
                             union packet_info_u *packet_info, struct espe_direct_fw_entry_ctx *ctx)
{
    struct espe_ip_fw_entry *entry = NULL;
    unsigned int hkey;
    unsigned int hash;
    unsigned long long ip_phy_addr_local = ip_phy_addr;
    unsigned long flags;
    int ret = 0;

    if (ctx->maped) {
        dma_unmap_single(spe->dev, ip_phy_addr_local, L1_CACHE_BYTES, DMA_FROM_DEVICE);
    }

    spin_lock_irqsave(&spe->ipfw.lock, flags);
    entry = ip_fw_node_get(spe);
    if (entry == NULL) {
        spe->direct_fw.ip_fw_node_get_fail++;
        goto release;
    }

    ret = espe_direct_fw_add_tuple(skb, entry, packet_info);
    if (ret) {
        spe->direct_fw.add_tuple_fail++;
        goto release;
    }

    /* get hash */
    hash = spe_ipfw_get_hash(&entry->tuple, spe->ipfw.hzone);
    hkey = reciprocal_scale(hash, spe->ipfw.hlist_size);
    if (ip_fw_entry_exist(spe, &(entry->tuple), hkey)) {
        spe->direct_fw.entry_exist++;
        goto release;
    }
    spin_unlock_irqrestore(&spe->ipfw.lock, flags);

    espe_set_direct_fw_entry(spe, ctx, entry, hash, hkey);

    ip_fw_list_add(spe, entry, hkey);

    spin_lock_irqsave(&spe->ipfw.lock, flags);
    list_add(&entry->priv->wan_list, &spe->ipfw.wan_entry_list); /* add entry after first node */
    spe->direct_fw.entry_cnt++;
    spin_unlock_irqrestore(&spe->ipfw.lock, flags);
    if (ctx->maped) {
        ip_phy_addr_local = dma_map_single(spe->dev, skb->data, L1_CACHE_BYTES, DMA_FROM_DEVICE);
    }

    return 0;

release:
    if (entry != NULL) {
        ip_fw_node_put(spe, entry, false);
    }
    spin_unlock_irqrestore(&spe->ipfw.lock, flags);
    if (ctx->maped) {
        ip_phy_addr_local = dma_map_single(spe->dev, skb->data, L1_CACHE_BYTES, DMA_FROM_DEVICE);
    }
    return -EINVAL;;
}

int espe_get_packet_info(struct sk_buff *skb, union packet_info_u *pkt_info)
{
    const struct ethhdr *eth;

    eth = (struct ethhdr *)skb->data;

    if (eth->h_proto == htons(ETH_P_IP)) {
        pkt_info->bits.ip_proto = AF_INET;
        pkt_info->bits.l4_proto = ip_hdr(skb)->protocol;
    } else if (eth->h_proto == htons(ETH_P_IPV6)) {
        pkt_info->bits.ip_proto = AF_INET6;
        pkt_info->bits.l4_proto = ipv6_hdr(skb)->nexthdr;
    } else {
        return -EINVAL;
    }

    return 0;
}

void bsp_espe_add_direct_fw_entry(struct sk_buff *skb, struct espe_direct_fw_entry_ctx *ctx)
{
    int ret;
    unsigned long long ip_phy_addr = 0;
    struct rx_cb_map_s *cb = NULL;
    struct spe *spe = &g_espe;
    union packet_info_u *packet_info = NULL;
    union packet_info_u pkt_info;

    spe->direct_fw.entry_add++;

    if (!spe->direct_fw.dfw_flags.enable) {
        spe->direct_fw.entry_add_dfw_disabled++;
        return;
    }

    if (spe->direct_fw.entry_cnt >= ESPE_DIERCT_FW_ENTRY_MAX) {
        spe->direct_fw.entry_add_max++;
        return;
    }

    if (skb == NULL) {
        spe->direct_fw.entry_add_skb_null++;
        return;
    }

    if (ctx->skb_cb_info) {
        skb->network_header = skb_headroom(skb) + ctx->l2_hdr_len;
        cb = (struct rx_cb_map_s *)skb->cb;
        if (cb->packet_info.u32 == 0) {
            spe->direct_fw.not_tcp_udp++;
            return;
        }
        packet_info = &cb->packet_info;
        ip_phy_addr = bsp_maa_to_phy(cb->maa_l2 + cb->packet_info.bits.l2_hdr_offeset);
        if (ip_phy_addr == 0) {
            spe->direct_fw.entry_add_l3maa_null++;
            return;
        }
    } else {
        skb->network_header = skb_headroom(skb) + ctx->l2_hdr_len;
        ret = espe_get_packet_info(skb, &pkt_info);
        if (unlikely(ret)) {
            spe->direct_fw.no_packet_info++;
            return;
        }
        packet_info = &pkt_info;
        ip_phy_addr = virt_to_phys(skb->data + ctx->l2_hdr_len);
    }

    ret = espe_add_direct_fw_entry(spe, skb, ip_phy_addr, packet_info, ctx);
    if (ret) {
        spe->direct_fw.entry_add_pkt_err_total++;
    }
    return;
}

void espe_del_direct_fw_entry(struct spe *spe, struct espe_ip_fw_entry *entry)
{
    unsigned int hkey;

    hkey = entry->priv->hkey;
    espe_ipfw_list_del_entry(spe, entry, hkey);
    spe->ports[entry->portno].ctrl.direct_fw_ctx.stream_cnt--;
    ip_fw_node_put(spe, entry, false);
    spe->stat.ipfw_del++;
    return;
}

void bsp_espe_del_direct_fw_dev(unsigned int devid)
{
    struct spe *spe = &g_espe;
    struct espe_ip_fw_entry *entry = NULL;
    struct espe_ip_fw_entry_priv *entry_priv = NULL;
    struct espe_ip_fw_entry_priv *n = NULL;
    unsigned long flags = 0;
    int i = 0;

    if (!spe->direct_fw.dfw_flags.enable) {
        return;
    }

    spin_lock_irqsave(&spe->ipfw.lock, flags);

    list_for_each_entry_safe(entry_priv, n, &spe->ipfw.wan_entry_list, wan_list)
    {
        entry = entry_priv->entry;
        if (((entry_priv->dst_net == ESPE_DFW_DL_TO_LAN) && (entry->htab_usrfield_lower16 == devid)) ||
            (entry_priv->dst_net == ESPE_DFW_UL_TO_WAN)) {
            espe_del_direct_fw_entry(spe, entry);
        } else {
            i++;
        }
    }
    spe->direct_fw.entry_cnt = i;
    spin_unlock_irqrestore(&spe->ipfw.lock, flags);
}

void espe_clean_ipfw_entry_cache(struct spe *spe)
{
    unsigned int tab_clean;
    // clean tab_ctrl;
    spe_writel(spe->regs, SPE_TAB_CLR, 0);

    tab_clean = BIT(4);  // clean all hash cache
    spe_writel(spe->regs, SPE_TAB_CLR, tab_clean);
}

void espe_updata_direct_fw_entry(struct spe *spe)
{
    struct espe_ip_fw_entry *entry = NULL;
    struct espe_ip_fw_entry_priv *entry_priv = NULL;
    struct espe_ip_fw_entry_priv *n = NULL;
    unsigned int spe_time;
    unsigned int entry_unuse_time = 0;
    unsigned long flags = 0;
    int i = 0;

    spe_time = spe_readl(spe->regs, SPE_TIME); /* 0-15bit valid */

    spin_lock_irqsave(&spe->ipfw.lock, flags);

    list_for_each_entry_safe(entry_priv, n, &spe->ipfw.wan_entry_list, wan_list)
    {
        entry = entry_priv->entry;
        if (spe_time >= entry->timestamp) {
            entry_unuse_time = spe_time - entry->timestamp;
        } else {
            entry_unuse_time = SPE_TIME_MAX - entry->timestamp + spe_time;
        }

        // process time out entry;
        if (entry_unuse_time > SPE_S_TO_TIMER_CNT(spe->direct_fw.direct_fw_time_interval)) {
            espe_del_direct_fw_entry(spe, entry);
        } else {
            i++;
        }
    }
    spe->direct_fw.entry_cnt = i;
    spin_unlock_irqrestore(&spe->ipfw.lock, flags);
}

static void espe_direct_fw_ctrl_timer(unsigned long arg)
{
    struct spe *spe = (struct spe *)(uintptr_t)arg;

    espe_updata_direct_fw_entry(spe);

    mod_timer(&spe->direct_fw.direct_fw_timer, (jiffies + msecs_to_jiffies(spe->direct_fw.direct_fw_time_interval)));
}

void espe_direct_fw_init(struct spe *spe)
{
    int ret;

    ret = memset_s(&spe->direct_fw, sizeof(spe->direct_fw), 0, sizeof(struct spe_direct_fw_ctrl));
    if (ret) {
        SPE_ERR("espe_direct_fw_init set empty failed\n");
        espe_bug(spe);
        return;
    }

    INIT_LIST_HEAD(&spe->direct_fw.wan_dev_list);
    spe->direct_fw.direct_fw_time_interval = ESPE_DIRECT_FW_AGEING_TIME;

    init_timer(&spe->direct_fw.direct_fw_timer);
    spe->direct_fw.direct_fw_timer.function = espe_direct_fw_ctrl_timer;
    spe->direct_fw.direct_fw_timer.data = (unsigned long)(uintptr_t)spe;
    spe->direct_fw.direct_fw_timer.expires = jiffies + msecs_to_jiffies(spe->direct_fw.direct_fw_time_interval);

    return;
}

void espe_direct_fw_status(void)
{
    int i;
    struct spe *spe = &g_espe;
    unsigned int cur_port;
    SPE_ERR("direct_fw_en: %u\n", spe->direct_fw.dfw_flags.enable);
    SPE_ERR("port_cnt: %u\n", spe->direct_fw.port_cnt);
    for (i = 0; i < spe->direct_fw.port_cnt; i++) {
        cur_port = spe->direct_fw.port_nums[i];
        if (spe->ports[cur_port].ctrl.port_flags.alloced) {
            SPE_ERR("port_nums: %u\n", cur_port);
            SPE_ERR("port_no: %u\n", spe->ports[cur_port].ctrl.direct_fw_ctx.port_no);
            SPE_ERR("port_weight: %u\n", spe->ports[cur_port].ctrl.direct_fw_ctx.port_weight);
            SPE_ERR("stream_cnt: %u\n", spe->ports[cur_port].ctrl.direct_fw_ctx.stream_cnt);
        }
    }

    SPE_ERR("entry_add: %u\n", spe->direct_fw.entry_add);
    SPE_ERR("entry_add_max: %u\n", spe->direct_fw.entry_add_max);
    SPE_ERR("entry_add_dfw_disabled: %u\n", spe->direct_fw.entry_add_dfw_disabled);
    SPE_ERR("entry_add_skb_null: %u\n", spe->direct_fw.entry_add_skb_null);
    SPE_ERR("entry_add_l3maa_null: %u\n", spe->direct_fw.entry_add_l3maa_null);
    SPE_ERR("not_tcp_udp: %u\n", spe->direct_fw.not_tcp_udp);
    SPE_ERR("entry_add_pkt_err_total: %u\n", spe->direct_fw.entry_add_pkt_err_total);
    SPE_ERR("entry_add_success: %u\n", spe->direct_fw.entry_add_success);

    SPE_ERR("ipfw_node_get_fail: %u\n", spe->direct_fw.ipfw_node_get_fail);
    SPE_ERR("ip_fw_node_get_fail: %u\n", spe->direct_fw.ip_fw_node_get_fail);
    SPE_ERR("iph_len_err: %u\n", spe->direct_fw.iph_len_err);
    SPE_ERR("iph_frag: %u\n", spe->direct_fw.iph_frag);
    SPE_ERR("ipproto_err: %u\n", spe->direct_fw.ipproto_err);
    SPE_ERR("add_tuple_fail: %u\n", spe->direct_fw.add_tuple_fail);
    SPE_ERR("entry_exist: %u\n", spe->direct_fw.entry_exist);
    SPE_ERR("no_wan_info: %u\n", spe->direct_fw.no_wan_info);
    SPE_ERR("no_wan_info: %u\n", spe->direct_fw.no_packet_info);
}

MODULE_LICENSE("GPL");

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
