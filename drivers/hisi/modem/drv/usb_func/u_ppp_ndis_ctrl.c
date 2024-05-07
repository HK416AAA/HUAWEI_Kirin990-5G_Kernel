/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <linux/export.h>
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/usb/balong/usb_nv.h>
#include <linux/kthread.h>
#include <securec.h>
#include <linux/usb/gadget.h>
#include <osl_types.h>
#include <mdrv_wan.h>
#include <mdrv_udi.h>
#include <mdrv_usb.h>
#include <bsp_print.h>
#include <bsp_espe.h>
#include <bsp_udi.h>
#include <bsp_maa.h>

#include "../../adrv/adrv.h"
#include "u_ppp_ndis_ctrl.h"

#define THIS_MODU mod_usb
#define USB_PPP_NDIS_MASK 64
struct u_ppp_ndis_ctx g_modem_stick_ctx;
static unsigned long long g_ppp_ndis_dma_mask = DMA_BIT_MASK(USB_PPP_NDIS_MASK);

struct u_ppp_ndis_ctx *usb_ppp_ndis_get_ctx(void)
{
    return &g_modem_stick_ctx;
}

static void usb_ppp_read_cb(void)
{
    int ret;
    struct acm_wr_async_info read_info;
    struct sk_buff *skb = NULL;
    struct sk_buff *skb_maa = NULL;
    struct rx_cb_map_s *cb = NULL;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;
    unsigned long long skb_maa_addr = 0;

    ret = mdrv_udi_ioctl(ctx->modem_handle, ACM_IOCTL_GET_RD_BUFF, &read_info);
    if (ret) {
        usb_err("modem get rd buf fail\n");
        return;
    }

    if (read_info.virt_addr == NULL || read_info.size == 0) {
        usb_err("virt_addr is null\n");
        return;
    }

    skb_maa = bsp_maa_alloc_skb(MAA_OPIPE_LEVEL_1_SIZE, &skb_maa_addr);
    if (skb_maa == NULL) {
        ctx->modem_stat.read_alloc_maa_fail++;
        goto RET;
    }

    skb = (struct sk_buff *)read_info.virt_addr;
    ret = memcpy_s((void *)skb_maa->data, MAA_OPIPE_LEVEL_1_SIZE, skb->data, skb->len);
    if (ret) {
        usb_err("memcpy_s fail:%d\n", ret);
        bsp_maa_kfree_skb(skb_maa);
        goto RET;
    }

    cb = (struct rx_cb_map_s *)skb_maa->cb;
    cb->userfield0 = USB_IOCTL_PPP_DATA;
    cb->userfield1 = skb->len;
    dma_map_single(&ctx->net->dev, (void *)skb_maa->data, skb->len, DMA_TO_DEVICE);
    ret = bsp_espe_config_td(ctx->spe_port, skb_maa_addr - ETH_HLEN, MAA_OPIPE_LEVEL_1_SIZE,
        (void *)skb_maa, ESPE_TD_KICK_PKT);
    if (ret) {
        usb_err("td config fail:%d\n", ret);
        bsp_maa_kfree_skb(skb_maa);
        goto RET;
    }
    ctx->modem_stat.read_succ++;

RET:
    ret = mdrv_udi_ioctl(ctx->modem_handle, ACM_IOCTL_RETURN_BUFF, &read_info);
    if (ret) {
        usb_err("modem ret read buf fail:%d\n", ret);
    }
    return;
}

/* 读取的msc怎么配置给spe */
static void usb_ppp_msc_read_cb(struct modem_msc_stru *read_msc)
{
    int ret = -1;
    struct sk_buff *skb = NULL;
    struct rx_cb_map_s *cb = NULL;
    unsigned long long skb_maa_addr = 0;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;

    if (read_msc == NULL) {
        usb_err("read_msc is null\n");
        return;
    }

    skb = bsp_maa_alloc_skb(MAA_OPIPE_LEVEL_1_SIZE, &skb_maa_addr);
    if (skb == NULL) {
        usb_err("alloc skb fail\n");
        return;
    }

    ret = memcpy_s((void *)skb->data, MAA_OPIPE_LEVEL_1_SIZE, read_msc, sizeof(*read_msc));
    if (ret) {
        usb_err("mem cpy fail:%d\n", ret);
        goto FAIL;
    }

    cb = (struct rx_cb_map_s *)skb->cb;
    cb->userfield0 = USB_IOCTL_PPP_MSC_READ;

    dma_map_single(&ctx->net->dev, (void *)skb->data, sizeof(*read_msc), DMA_TO_DEVICE);
    ret = bsp_espe_config_td(ctx->spe_port, skb_maa_addr - ETH_HLEN, MAA_OPIPE_LEVEL_1_SIZE,
        (void *)skb, ESPE_TD_KICK_PKT);
    if (ret) {
        usb_err("td config fail\n");
        goto FAIL;
    }

    ctx->modem_stat.read_msc_succ++;
    return;

FAIL:
    bsp_maa_free(skb_maa_addr);
}

static int usb_ppp_open(void)
{
    struct acm_read_buff_info read_buff_info;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;
    struct udi_open_param param;
    int handle;
    int ret = -1;

    param.devid = UDI_ACM_MODEM_ID;
    param.private = NULL;
    handle = mdrv_udi_open(&param);
    if (handle == UDI_INVALID_HANDLE) {
        usb_err("modem open fail\n");
        return ret;
    }
    ctx->modem_handle = handle;
    ctx->modem_stat.open++;

    ret = mdrv_udi_ioctl(handle, ACM_IOCTL_SET_READ_CB, usb_ppp_read_cb);
    if (ret) {
        usb_err("modem set read cb fail\n");
        return ret;
    }

    ret = mdrv_udi_ioctl(handle, ACM_IOCTL_WRITE_DO_COPY, NULL);
    if (ret) {
        usb_err("modem set do copy fail\n");
        return ret;
    }

    ret = mdrv_udi_ioctl(handle, ACM_MODEM_IOCTL_SET_MSC_READ_CB, usb_ppp_msc_read_cb);
    if (ret) {
        usb_err("modem set read msc cb fail\n");
        return ret;
    }

    read_buff_info.buff_size = USB_MODEM_UL_DATA_BUFF_SIZE;
    read_buff_info.buff_num = USB_MODEM_UL_DATA_BUFF_NUM;
    ret = mdrv_udi_ioctl(handle, ACM_IOCTL_RELLOC_READ_BUFF, &read_buff_info);
    if (ret) {
        usb_err("modem realloc buff fail\n");
        return ret;
    }

    return 0;
}

static int usb_ppp_close(void)
{
    int ret;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;

    ctx->modem_stat.close++;
    if (ctx->modem_handle == UDI_INVALID_HANDLE) {
        return 0;
    }

    ret = mdrv_udi_close(ctx->modem_handle);
    if (ret) {
        usb_err("modem close fail\n");
        return ret;
    }

    ctx->modem_handle = UDI_INVALID_HANDLE;
    return 0;
}

static void usb_enable_nortify_cb(unsigned int enum_done)
{
    int ret = -1;
    struct sk_buff *skb = NULL;
    struct rx_cb_map_s *cb = NULL;
    unsigned int *data = NULL;
    unsigned long long skb_maa_addr = 0;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;

    skb = bsp_maa_alloc_skb(MAA_OPIPE_LEVEL_1_SIZE, &skb_maa_addr);
    if (skb == NULL) {
        usb_err("alloc skb fail\n");
        return;
    }

    data = (unsigned int *)(skb->data);
    *data = enum_done;
    cb = (struct rx_cb_map_s *)skb->cb;
    cb->userfield0 = USB_IOCTL_USB_READY;
    dma_map_single(&ctx->net->dev, (void *)skb->data, sizeof(unsigned int), DMA_TO_DEVICE);
    ret = bsp_espe_config_td(ctx->spe_port, skb_maa_addr - ETH_HLEN, MAA_OPIPE_LEVEL_1_SIZE,
        (void *)skb, ESPE_TD_KICK_PKT);
    if (ret) {
        usb_err("td config fail\n");
        goto FAIL;
    }
    usb_info("usb enable nortify succ\n");
    return;

FAIL:
    bsp_maa_free(skb_maa_addr);
}

static void usb_ppp_msc_write(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb)
{
    int ret;
    struct modem_msc_stru *msc = NULL;

    dma_unmap_single(&ctx->net->dev, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);
    msc = (struct modem_msc_stru *)skb->data;
    ret = mdrv_udi_ioctl(ctx->modem_handle, ACM_MODEM_IOCTL_MSC_WRITE_CMD, msc);
    if (ret) {
        ctx->modem_stat.write_msc_fail++;
    } else {
        ctx->modem_stat.write_msc_succ++;
    }

    bsp_maa_kfree_skb(skb);
    return;
}

static void usb_ppp_data_write(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb, unsigned long long maa_org)
{
    int ret;
    struct acm_wr_async_info wr_info;
    struct sk_buff *skb_new = NULL;

    skb_new = bsp_maa_skb_unmap(maa_org);
    if (unlikely(skb_new == NULL)) {
        usb_err("maa unmap fail:0x%llx\n", maa_org);
        return;
    }

    skb_push(skb_new, ETH_HLEN);
    skb_put(skb_new, skb->len - ETH_HLEN);
    dma_unmap_single(&ctx->net->dev, virt_to_phys(skb_new->data), skb_new->len, DMA_FROM_DEVICE);
    wr_info.virt_addr = (char *)skb_new;
    ret = mdrv_udi_ioctl(ctx->modem_handle, ACM_IOCTL_WRITE_ASYNC, &wr_info);
    if (ret) {
        ctx->modem_stat.write_fail++;
        dev_kfree_skb_any(skb_new);
    } else {
        ctx->modem_stat.write_succ++;
    }
    return; /* write complete free by modem */
}

static void usb_ppp_ndis_ctrl_xmit(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb)
{
    struct stat_ctx *modem_stat = NULL;
    struct rx_cb_map_s *cb = NULL;
    unsigned int cmd;
    unsigned int dev_id = 0;
    unsigned long flags;

    modem_stat = &ctx->modem_stat;
    cb = (struct rx_cb_map_s *)skb->cb;
    cmd = cb->userfield0;

    switch (cmd) {
        case VCOM_IOCTL_PPP_OPEN:
            usb_err("ppp cmd:0x%x\n", cmd);
            spin_lock_irqsave(&ctx->ctx_lock, flags);
            ctx->ccore_spe_ok = 1;
            if (ctx->usb_enum) {
                usb_enable_nortify_cb(1);
            }
            spin_unlock_irqrestore(&ctx->ctx_lock, flags);
            break;

        case USB_IOCTL_PDUINFO_CFG:
            /* fall-through */
        case USB_IOCTL_SPEED_CHANGE:
            /* fall-through */
        case USB_IOCTL_CONNECT_NOTIFY:
            /* fall-through */
        case USB_IOCTL_SET_FLOW_CTRL:
            /* fall-through */
        case USB_IOCTL_NDIS_DATA:
            usb_ndis_ctrl_xmit(ctx, skb, cmd);
            return;

        case USB_IOCTL_PPP_MSC_WRITE:
            usb_err("ndis cmd:0x%x\n", cmd);
            usb_ppp_msc_write(ctx, skb);
            return;

        case USB_IOCTL_PPP_DATA:
            usb_ppp_data_write(ctx, skb, cb->maa_org);
            return;

        default:
            usb_err("dev_id:%d cmd invalide\n", dev_id);
            break;
    }

    bsp_maa_kfree_skb(skb);
    return;
}

void usb_ppp_ndis_xmit_wq(struct work_struct *work)
{
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;
    struct sk_buff *skb = NULL;
    unsigned long flags = 0;
    struct sk_buff_head tx_queue;

    skb_queue_head_init(&tx_queue);
    spin_lock_irqsave(&ctx->tx_lock, flags);
    skb_queue_splice_tail_init(&ctx->skb_head, &tx_queue);
    spin_unlock_irqrestore(&ctx->tx_lock, flags);

    while (1) {
        skb = __skb_dequeue(&tx_queue);
        if (unlikely(skb == NULL)) {
            break;
        }

        usb_ppp_ndis_ctrl_xmit(ctx, skb);
    }

    return;
}

void usb_ppp_ndis_init_workqueue(struct u_ppp_ndis_ctx *priv)
{
    INIT_WORK(&priv->work, usb_ppp_ndis_xmit_wq);
    priv->workqueue = alloc_ordered_workqueue("usb_ppp_ndis_tx", 0);
    if (priv->workqueue == NULL) {
        usb_err("workqueue alloc fail\n");
        return;
    }

    usb_err("workqueue alloc succ\n");
    return;
}

static void usb_ppp_ndis_ctrl_complete_rd(void *priv)
{
    struct u_ppp_ndis_ctx *ctx = (struct u_ppp_ndis_ctx *)priv;

    if (ctx == NULL) {
        return;
    }

    if (!skb_queue_empty(&ctx->skb_head)) {
        queue_work(ctx->workqueue, &ctx->work);
    }
    return;
}

/* stick ctrl maa addr must invalid cache */
static void usb_ppp_ndis_ctrl_finish_rd(struct sk_buff *skb, unsigned int len, void *param, unsigned int flags)
{
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;
    unsigned long flag;

    if (skb == NULL) {
        usb_err("skb is null\n");
        return;
    }

    spin_lock_irqsave(&ctx->tx_lock, flag);
    __skb_queue_tail(&ctx->skb_head, skb);
    spin_unlock_irqrestore(&ctx->tx_lock, flag);

    return;
}

int usb_alloc_espe_port(struct u_ppp_ndis_ctx *ctx)
{
    int port_num, err;
    struct espe_port_comm_attr port_attr;
    struct net_device *net = NULL;

    net = alloc_etherdev(sizeof(unsigned int));
    if (net == NULL) {
        usb_err("alloc net fail\n");
        return -ENOMEM;
    }

    err = memset_s(&port_attr, sizeof(port_attr), 0, sizeof(struct espe_port_comm_attr));
    if (err) {
        usb_err("memset_s err:%d\n", err);
        return err;
    }

    port_attr.enc_type = SPE_ENC_NONE;
    port_attr.td_depth = USB_SPE_TD_SIZE;
    port_attr.rd_depth = USB_SPE_RD_SIZE;
    port_attr.padding_enable = 0;
    port_attr.bypassport = SPE_CCORE_PPP_NDIS_CTRL_ID;
    port_attr.td_copy_en = 0;
    port_attr.bypassport_en = 1;
    port_attr.rsv_port_id = SPE_ACORE_PPP_NDIS_CTRL_ID;

    port_attr.ops.espe_finish_rd = usb_ppp_ndis_ctrl_finish_rd;
    port_attr.ops.espe_finish_td = NULL;
    port_attr.ops.espe_complete_rd = usb_ppp_ndis_ctrl_complete_rd;

    port_attr.ops.espe_netif_rx = NULL;
    port_attr.net = net;
    port_attr.priv = ctx;

    port_num = bsp_espe_alloc_port(&port_attr);
    if (port_num >= SPE_PORT_NUM || port_num < 0) {
        usb_err("spe port alloc failed\n");
        return -ENOMEM;
    }

    err = bsp_espe_enable_port(port_num);
    if (err) {
        usb_err("spe port enable failed\n");
        return -EINVAL;
    }

    arch_setup_dma_ops(&net->dev, 0, 0, NULL, 0);
    dma_set_mask_and_coherent(&net->dev, DMA_BIT_MASK(USB_PPP_NDIS_MASK));
    net->dev.dma_mask = &g_ppp_ndis_dma_mask;

    ctx->spe_port = port_num;
    ctx->net = net;
    return 0;
}

void usb_ppp_ndis_enable_cb(void)
{
    int ret;
    unsigned long flags;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;

    if (ctx->bypass) {
        ret = usb_ndis_ctrl_open();
        if (ret) {
            usb_info("stick ctrl open fail\n");
        } else {
            usb_info("stick ctrl open succ\n");
        }
    }

    ret = usb_ppp_open();
    if (ret) {
        usb_info("modem open fail\n");
    } else {
        usb_info("modem open succ\n");
    }

    spin_lock_irqsave(&ctx->ctx_lock, flags);
    ctx->usb_enum = 1;
    if (ctx->ccore_spe_ok) {
        usb_enable_nortify_cb(1);
    }
    spin_unlock_irqrestore(&ctx->ctx_lock, flags);
}

void usb_ppp_ndis_disable_cb(void)
{
    int ret;
    unsigned long flags;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;

    if (ctx->bypass) {
        ret = usb_ndis_ctrl_close();
        if (ret) {
            usb_info("stick ctrl close fail\n");
        } else {
            usb_info("stick ctrl close succ\n");
        }
    }

    ret = usb_ppp_close();
    if (ret) {
        usb_info("modem close fail\n");
    } else {
        usb_info("modem close succ\n");
    }

    spin_lock_irqsave(&ctx->ctx_lock, flags);
    ctx->usb_enum = 0;
    if (ctx->ccore_spe_ok) {
        usb_enable_nortify_cb(0);
    }
    spin_unlock_irqrestore(&ctx->ctx_lock, flags);
}

static int __init usb_ppp_ndis_init(void)
{
    int ret;
    struct u_ppp_ndis_ctx *ctx = &g_modem_stick_ctx;

    usb_err("init start\n");
    if (usb_ndis_support()) {
        ctx->bypass = 1;
    }

    ret = usb_alloc_espe_port(ctx);
    if (ret) {
        usb_err("init fail:%d\n", ret);
        return ret;
    }

    skb_queue_head_init(&ctx->skb_head);
    spin_lock_init(&ctx->tx_lock);
    spin_lock_init(&ctx->tx_lock);
    usb_ppp_ndis_init_workqueue(ctx);

    BSP_USB_RegUdiEnableCB(usb_ppp_ndis_enable_cb);
    BSP_USB_RegUdiDisableCB(usb_ppp_ndis_disable_cb);

    usb_err("init ok\n");
    return 0;
}

module_init(usb_ppp_ndis_init);

void usb_ppp_show(void)
{
    struct stat_ctx *modem_stat = &g_modem_stick_ctx.modem_stat;

    usb_err("open               : %u\n", modem_stat->open);
    usb_err("close              : %u\n", modem_stat->close);
    usb_err("write_succ         : %u\n", modem_stat->write_succ);
    usb_err("write_fail         : %u\n", modem_stat->write_fail);
    usb_err("write_msc_succ     : %u\n", modem_stat->write_msc_succ);
    usb_err("write_msc_fail     : %u\n", modem_stat->write_msc_fail);
    usb_err("write_msc_done     : %u\n", modem_stat->write_msc_done);
    usb_err("write_default      : %u\n", modem_stat->write_default);
    usb_err("read_succ          : %u\n", modem_stat->read_succ);
    usb_err("read_fail          : %u\n", modem_stat->read_fail);
    usb_err("read_free_skb      : %u\n", modem_stat->read_free_skb);
    usb_err("return_buff_succ   : %u\n", modem_stat->return_buff_succ);
    usb_err("return_buff_fail   : %u\n", modem_stat->return_buff_fail);
    usb_err("read_msc_succ      : %u\n", modem_stat->read_msc_succ);
    usb_err("read_msc_fail      : %u\n", modem_stat->read_msc_fail);
}
