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

static void usb_ndis_stat_chg_cb(enum ncm_ioctrl_connect_stus status, void *buff)
{
    int ret;
    struct sk_buff *skb = NULL;
    struct rx_cb_map_s *cb = NULL;
    unsigned int *data = NULL;
    unsigned long long skb_maa_addr = 0;
    struct u_ppp_ndis_ctx *ctx = usb_ppp_ndis_get_ctx();

    skb = bsp_maa_alloc_skb(MAA_OPIPE_LEVEL_1_SIZE, &skb_maa_addr);
    if (skb == NULL) {
        usb_err("alloc skb fail\n");
        return;
    }

    data = (unsigned int *)(skb->data);
    *data = (unsigned int)status;

    cb = (struct rx_cb_map_s *)skb->cb;
    cb->userfield0 = USB_IOCTL_STAT_CHG;
    dma_map_single(&ctx->net->dev, (void *)skb->data, sizeof(unsigned int), DMA_TO_DEVICE);
    ret = bsp_espe_config_td(ctx->spe_port, skb_maa_addr - ETH_HLEN, MAA_OPIPE_LEVEL_1_SIZE,
        (void *)skb, ESPE_TD_KICK_PKT);
    if (ret) {
        usb_err("td config fail\n");
        bsp_maa_free(skb_maa_addr);
    }

    return;
}

int usb_ndis_ctrl_open(void)
{
    struct udi_open_param param;
    struct u_ppp_ndis_ctx *ctx = usb_ppp_ndis_get_ctx();
    int handle;
    int ret = -1;

    ctx->stick_stat.open++;
    param.devid = UDI_NCM_NDIS_ID;
    param.private = NULL;
    handle = mdrv_udi_open(&param);
    if (handle == UDI_INVALID_HANDLE) {
        usb_err("stick open fail\n");
        return ret;
    }
    ctx->stick_handle = handle;

    ret = mdrv_udi_ioctl(handle, NCM_IOCTL_REG_CONNECT_STUS_CHG_FUNC, usb_ndis_stat_chg_cb);
    if (ret) {
        usb_err("stick set write cb fail:%d\n", ret);
    }

    return ret;
}

int usb_ndis_ctrl_close(void)
{
    int ret;
    struct u_ppp_ndis_ctx *ctx = usb_ppp_ndis_get_ctx();

    ctx->stick_stat.close++;
    if (ctx->stick_handle == UDI_INVALID_HANDLE) {
        return 0;
    }

    ret = mdrv_udi_close(ctx->stick_handle);
    if (ret) {
        usb_err("stick ctrl close fail\n");
        return ret;
    }

    ctx->stick_handle = UDI_INVALID_HANDLE;
    return 0;
}

int usb_ndis_support(void)
{
    bsp_usb_nv_init();
    return bsp_usb_is_stick_mode();
}

void usb_ndis_ctrl_write(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb, unsigned int cmd)
{
    int ret;

    usb_err("ndis cmd:0x%x\n", cmd);
    dma_unmap_single(&ctx->net->dev, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);
    ret = mdrv_udi_ioctl(ctx->stick_handle, cmd, (void *)skb->data);
    if (ret) {
        ctx->stick_stat.write_fail++;
    } else {
        ctx->stick_stat.write_succ++;
    }

    bsp_maa_kfree_skb(skb);
    return;
}

void usb_ndis_data_write(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb, unsigned long long maa_org)
{
    int ret;
    struct sk_buff *skb_new = NULL;

    skb_new = bsp_maa_skb_unmap(maa_org);
    if (skb_new == NULL) {
        usb_err("unmap fail\n");
        return;
    }

    skb_push(skb_new, ETH_HLEN);
    skb_put(skb_new, skb->len - ETH_HLEN);
    ret = mdrv_udi_write(ctx->stick_handle, (void *)skb_new, 0);
    if (ret) {
        ctx->stick_stat.write_fail++;
        dev_kfree_skb_any(skb_new);
    } else {
        ctx->stick_stat.write_succ++;
    }
    return;
}

void usb_ndis_ctrl_xmit(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb, unsigned int cmd)
{
    struct stat_ctx *stick_stat = NULL;
    struct rx_cb_map_s *cb = NULL;
    unsigned int dev_id = 0;
    void *cmd_para = NULL;

    stick_stat = &ctx->stick_stat;
    cb = (struct rx_cb_map_s *)skb->cb;
    cmd_para = (void *)skb->data;

    switch (cmd) {
        case USB_IOCTL_PDUINFO_CFG:
            cmd = NCM_IOCTL_SET_PDUSESSION;
            usb_ndis_ctrl_write(ctx, skb, cmd);
            return;

        case USB_IOCTL_SPEED_CHANGE:
            cmd = NCM_IOCTL_CONNECTION_SPEED_CHANGE_NOTIF;
            usb_ndis_ctrl_write(ctx, skb, cmd);
            return;

        case USB_IOCTL_CONNECT_NOTIFY:
            cmd = NCM_IOCTL_NETWORK_CONNECTION_NOTIF;
            usb_ndis_ctrl_write(ctx, skb, cmd);
            return;

        case USB_IOCTL_SET_FLOW_CTRL:
            cmd = NCM_IOCTL_FLOW_CTRL_NOTIF;
            usb_ndis_ctrl_write(ctx, skb, cmd);
            return;

        case USB_IOCTL_NDIS_DATA:
            usb_ndis_data_write(ctx, skb, cb->maa_org);
            return;

        default:
            usb_err("dev_id:%d cmd invalide\n", dev_id);
            break;
    }

    return;
}

void usb_ndis_show(void)
{
    struct u_ppp_ndis_ctx *ctx = usb_ppp_ndis_get_ctx();
    struct stat_ctx *stick_stat = &ctx->stick_stat;

    usb_err("open               : %u\n", stick_stat->open);
    usb_err("close              : %u\n", stick_stat->close);
    usb_err("write_succ         : %u\n", stick_stat->write_succ);
    usb_err("write_fail         : %u\n", stick_stat->write_fail);
    usb_err("write_msc_succ     : %u\n", stick_stat->write_msc_succ);
    usb_err("write_msc_fail     : %u\n", stick_stat->write_msc_fail);
    usb_err("write_msc_done     : %u\n", stick_stat->write_msc_done);
    usb_err("write_default      : %u\n", stick_stat->write_default);
    usb_err("read_succ          : %u\n", stick_stat->read_succ);
    usb_err("read_fail          : %u\n", stick_stat->read_fail);
    usb_err("read_free_skb      : %u\n", stick_stat->read_free_skb);
    usb_err("return_buff_succ   : %u\n", stick_stat->return_buff_succ);
    usb_err("return_buff_fail   : %u\n", stick_stat->return_buff_fail);
    usb_err("read_msc_succ      : %u\n", stick_stat->read_msc_succ);
    usb_err("read_msc_fail      : %u\n", stick_stat->read_msc_fail);
}
