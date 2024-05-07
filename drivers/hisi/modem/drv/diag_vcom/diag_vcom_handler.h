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

#ifndef __DIAG_VCOM_HANDLER_H__
#define __DIAG_VCOM_HANDLER_H__

#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/version.h>
#include "diag_vcom.h"
#include "diag_vcom_data.h"

#define DIAG_VCOM_DATA_SIZE (NLMSG_DEFAULT_SIZE - sizeof(diag_vcom_unsol_msg_s))
#define DIAG_VCOM_GET_ENTTIY(_ch) (&g_ppm_netlink_info[_ch])

#define DIAG_VCOM_MSG_OFFSETOF(_m) (offsetof(diag_vcom_msg_s, _m))
#define DIAG_VCOM_MSG_ARGS_SIZE(_m) (sizeof(((diag_vcom_msg_s *)0)->_m))
#define DIAG_VCOM_UNSOL_MSG_ARGS_SIZE(_m) (sizeof(((diag_vcom_unsol_msg_s *)0)->_m))

#define DIAG_VCOM_WAKELOCK_TIMEOUT 120

typedef struct {
    uint32_t msg_seq;
    uint32_t blk_num;
    uint32_t blk_seq;
    uint8_t mode;
    uint8_t reserved[3];
    uint32_t len;
    uint8_t *data;
} diag_vcom_om_data_desc_s;

typedef struct {
    uint32_t input_message;
    uint32_t input_errors;
    uint32_t tx_om_packets;
    uint32_t tx_om_errors;
    uint32_t rx_om_packets;
    uint32_t rx_om_errors;
    uint32_t rx_om_dropped;
    uint32_t alloc_errors;
    uint32_t put_errors;
    uint32_t unicast_errors;
    uint32_t unicast_message;
    volatile uint32_t wakelock_state;
} diag_vcom_stats_s;

typedef struct {
    struct sock *sk;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    struct wakeup_source *lock;
#else
    struct wakeup_source lock;
#endif
    struct diag_vcom_cb_ops cb_ops;
    diag_vcom_stats_s stats;
    uint32_t channel;
    uint32_t ready;
    uint32_t resp_pid;
    uint32_t portid;
} ppm_netlink_info_s;

int diag_vcom_channel_init(void);
void diag_vcom_channel_exit(void);
void diag_vcom_ctrl_msg_handler(struct sk_buff *skb);
void diag_vcom_data_msg_handler(struct sk_buff *skb);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
void diag_vcom_data_timer_handler(struct timer_list *timer);
#else
void diag_vcom_data_timer_handler(unsigned long para);
#endif
void diag_vcom_create_timer(void);
#endif /* __DIAG_VCOM_HANDLER_H__ */
