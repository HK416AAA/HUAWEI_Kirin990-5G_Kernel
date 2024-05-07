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

/*
 * 1. Other files included
 */
#include <net/sock.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <securec.h>
#include "product_config.h"
#ifdef BSP_CONFIG_PHONE_TYPE
#include <linux/mtd/hisi_nve_interface.h>
#endif
#include <securec.h>
#include "mdrv.h"
#include "mdrv_diag_system.h"
#include "bsp_diag.h"
#include "bsp_socp.h"
#include "diag_vcom_handler.h"
#include "diag_vcom_debug.h"
#include "bsp_softtimer.h"

struct timer_list g_ppm_netlink_timer;

static struct netlink_kernel_cfg g_ppm_netlink_ctrl_cfg = {
    .input = diag_vcom_ctrl_msg_handler,
};

static struct netlink_kernel_cfg g_ppm_netlink_data_cfg = {
    .input = diag_vcom_data_msg_handler,
};

static ppm_netlink_info_s g_ppm_netlink_info[DIAG_VCOM_CHAN_MAX] = {
    {
        .channel = DIAG_VCOM_CHAN_CTRL,
    },
    {
        .channel = DIAG_VCOM_CHAN_DATA,
    },
};

/*
 * Prototype: diag_vcom_create_timer
 * Description: create diag vcom timer
 */
void diag_vcom_create_timer(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        timer_setup(&g_ppm_netlink_timer, diag_vcom_data_timer_handler, 0);
#else
    init_timer(&g_ppm_netlink_timer);
    g_ppm_netlink_timer.function = diag_vcom_data_timer_handler;
    g_ppm_netlink_timer.data = (unsigned long)(uintptr_t)DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_DATA);
#endif
    DIAG_VCOM_LOGE("create diag_vcom_data_timer success\n");

    return;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
void diag_vcom_data_timer_handler(struct timer_list *timer)
{
#else
void diag_vcom_data_timer_handler(unsigned long para)
{
#endif
    ppm_netlink_info_s *entity = DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_DATA);

    DIAG_VCOM_LOGE(" enter diag_vcom_data_timer_handler\n");

    if (entity->stats.wakelock_state) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        __pm_relax(entity->lock); //lint !e455
#else
        __pm_relax(&entity->lock); //lint !e455
#endif
        entity->stats.wakelock_state = 0;
        DIAG_VCOM_LOGE("diag_vcom_data_timer_handler: state = 0x%x\n", entity->stats.wakelock_state);
    }
}

/*
 * Prototype: diag_vcom_channel_init
 * Description: Register netlink protocol handler and open socket.
 */
int diag_vcom_channel_init(void)
{
    struct sock *sk1 = NULL;
    struct sock *sk2 = NULL;
    ppm_netlink_info_s *entity = NULL;

    sk1 = netlink_kernel_create(&init_net, NETLINK_HW_LOGCAT_EXT, &g_ppm_netlink_ctrl_cfg);
    if (sk1 == NULL) {
        DIAG_VCOM_LOGE("Cannot create diag ctrl socket\n");
        return -ENOMEM;
    }

    sk2 = netlink_kernel_create(&init_net, NETLINK_HW_LOGCAT, &g_ppm_netlink_data_cfg);
    if (sk2 == NULL) {
        DIAG_VCOM_LOGE("Cannot to create diag data socket\n");
        netlink_kernel_release(sk1);
        sk1 = NULL;
        return -ENOMEM;
    }

    entity = DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_CTRL);
    entity->sk = sk1;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    entity->lock = wakeup_source_register(NULL, "diag_ctrl_wakelock");
    if (entity->lock == NULL) {
        DIAG_VCOM_LOGE("diag ctrl netlink wakeup lock register err\n");
    }
#else
    wakeup_source_init(&entity->lock, "diag_ctrl_wakelock");
#endif
    entity = DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_DATA);
    entity->sk = sk2;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    entity->lock = wakeup_source_register(NULL, "diag_data_wakelock");
    if (entity->lock == NULL) {
        DIAG_VCOM_LOGE("diag data netlink wakeup lock register err\n");
    }
#else
    wakeup_source_init(&entity->lock, "diag_data_wakelock");
#endif
    diag_vcom_create_timer(); /* diag_data_wakelock release in 2 mins */

    return 0;
}

/*
 * Prototype: diag_vcom_channel_exit
 * Description: Release netlink resources.
 */
void diag_vcom_channel_exit(void)
{
    ppm_netlink_info_s *entity = NULL;

    entity = DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_CTRL);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
    wakeup_source_trash(&entity->lock);
#endif
    netlink_kernel_release(entity->sk);

    entity = DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_DATA);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
    wakeup_source_trash(&entity->lock);
#endif
    netlink_kernel_release(entity->sk);
}

/*
 * Prototype: diag_vcom_register_ops
 * Description: Register channel operations.
 */
int diag_vcom_register_ops(uint32_t channel, struct diag_vcom_cb_ops *ops)
{
    ppm_netlink_info_s *entity = NULL;

    if (channel >= DIAG_VCOM_CHAN_MAX) {
        DIAG_VCOM_LOGE("Register channel[%d] ops failed\n", channel);
        return DIAG_VCOM_ERROR;
    }

    entity = DIAG_VCOM_GET_ENTTIY(channel);
    entity->cb_ops = *ops;
    entity->ready = true;

    DIAG_VCOM_LOGH("Register channel[%d] ops successfully\n", channel);
    return DIAG_VCOM_OK;
}

/*
 * Prototype: diag_vcom_send_resp_msg
 * Description: Send netlink response message.
 */
static int diag_vcom_send_resp_msg(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg, uint32_t msg_len)
{
    struct sk_buff *resp_skb = NULL;
    struct nlmsghdr *resp_nlh = NULL;
    diag_vcom_msg_s *resp_msg = NULL;
    size_t payload;
    int ret;

    DIAG_VCOM_LOGI("pid=%d, msg_type=%d, op=%d, code[len]=%d\n", entity->resp_pid, msg->message_type, msg->op,
        msg->return_code);

    payload = NLMSG_ALIGN(msg_len);

    resp_skb = nlmsg_new(payload, GFP_KERNEL);
    if (resp_skb == NULL) {
        DIAG_VCOM_LOGE("Alloc netlink message failed\n");
        entity->stats.alloc_errors++;
        return DIAG_VCOM_ERROR;
    }

    resp_nlh = nlmsg_put(resp_skb, 0, 0, NLMSG_DONE, (int)payload, 0);
    if (resp_nlh == NULL) {
        DIAG_VCOM_LOGE("Put netlink message failed\n");
        kfree_skb(resp_skb);
        resp_skb = NULL;
        entity->stats.put_errors++;
        return DIAG_VCOM_ERROR;
    }

    resp_msg = (diag_vcom_msg_s *)nlmsg_data(resp_nlh);
    if (memcpy_s(resp_msg, msg_len, msg, msg_len) != EOK) {
        DIAG_VCOM_LOGE("memcpy_s failed\n");
        kfree_skb(resp_skb);
        resp_skb = NULL;
        return DIAG_VCOM_ERROR;
    }

    ret = nlmsg_unicast(entity->sk, resp_skb, entity->resp_pid);
    if (ret < 0) {
        DIAG_VCOM_LOGE("Send netlink message failed, pid=%d\n", entity->resp_pid);
        entity->stats.unicast_errors++;
        return DIAG_VCOM_ERROR;
    }

    entity->stats.unicast_message++;
    return DIAG_VCOM_OK;
}

/*
 * Prototype: diag_vcom_send_om_cmd_msg
 * Description: Send netlink om command response.
 */
static int diag_vcom_send_om_cmd_msg(ppm_netlink_info_s *entity, uint8_t *data, uint32_t len)
{
    struct sk_buff *resp_skb = NULL;
    struct nlmsghdr *resp_nlh = NULL;
    diag_vcom_msg_s *resp_msg = NULL;
    size_t payload;
    int ret;

    DIAG_VCOM_LOGI("pid=%d, len=%d", entity->portid, len);

    payload = NLMSG_ALIGN(sizeof(diag_vcom_msg_s) + len);

    resp_skb = nlmsg_new(payload, GFP_KERNEL);
    if (resp_skb == NULL) {
        DIAG_VCOM_LOGE("Alloc netlink message failed\n");
        entity->stats.alloc_errors++;
        return DIAG_VCOM_ERROR;
    }

    resp_nlh = nlmsg_put(resp_skb, 0, 0, NLMSG_DONE, (int)payload, 0);
    if (resp_nlh == NULL) {
        DIAG_VCOM_LOGE("Put netlink message failed\n");
        kfree_skb(resp_skb);
        resp_skb = NULL;
        entity->stats.put_errors++;
        return DIAG_VCOM_ERROR;
    }

    resp_msg = (diag_vcom_msg_s *)nlmsg_data(resp_nlh);
    resp_msg->message_type = DIAG_VCOM_REQUEST_SEND_OM_COMMAND;
    resp_msg->op = DIAG_VCOM_MSG_RETURN_DATA;
    resp_msg->args_length = DIAG_VCOM_MSG_ARGS_SIZE(om_command) + len;

    resp_msg->om_command.length = len;
    ret = memcpy_s(resp_msg->om_command.data, resp_msg->om_command.length, data, len);
    if (ret != EOK) {
        DIAG_VCOM_LOGE("memory copy failed, ret=%d\n", ret);
        kfree_skb(resp_skb);
        entity->stats.put_errors++;
        return ret;
    }

    ret = nlmsg_unicast(entity->sk, resp_skb, entity->portid);
    if (ret < 0) {
        DIAG_VCOM_LOGE("Send netlink message failed, ret=%d\n", ret);
        entity->stats.unicast_errors++;
        return DIAG_VCOM_ERROR;
    }

    entity->stats.unicast_message++;
    return DIAG_VCOM_OK;
}

/*
 * Prototype: diag_vcom_send_om_data_msg
 * Description: Send netlink om data message.
 */
static int diag_vcom_send_om_data_msg(ppm_netlink_info_s *entity, diag_vcom_om_data_desc_s *desc)
{
    struct sk_buff *unsol_skb = NULL;
    struct nlmsghdr *unsol_nlh = NULL;
    diag_vcom_unsol_msg_s *unsol_msg = NULL;
    size_t payload;
    int ret;

    DIAG_VCOM_LOGD("pid=%d, len=%d, mode=%d\n", entity->portid, desc->len, desc->mode);

    payload = NLMSG_ALIGN(sizeof(diag_vcom_unsol_msg_s) + desc->len);

    unsol_skb = nlmsg_new(payload, GFP_KERNEL);
    if (unsol_skb == NULL) {
        DIAG_VCOM_LOGE("Alloc netlink message failed\n");
        entity->stats.alloc_errors++;
        return DIAG_VCOM_ERROR;
    }

    unsol_nlh = nlmsg_put(unsol_skb, 0, 0, NLMSG_DONE, (int)payload, 0);
    if (unsol_nlh == NULL) {
        DIAG_VCOM_LOGE("Put netlink message failed\n");
        kfree_skb(unsol_skb);
        entity->stats.put_errors++;
        return DIAG_VCOM_ERROR;
    }

    unsol_msg = (diag_vcom_unsol_msg_s *)nlmsg_data(unsol_nlh);
    unsol_msg->message_type = DIAG_VCOM_UNSOL_REPORT_OM_DATA;
    unsol_msg->op = DIAG_VCOM_MSG_RETURN_DATA;
    unsol_msg->args_length = DIAG_VCOM_UNSOL_MSG_ARGS_SIZE(om_data) + desc->len;

    unsol_msg->om_data.msg_seq = desc->msg_seq;
    unsol_msg->om_data.blk_num = desc->blk_num;
    unsol_msg->om_data.blk_seq = desc->blk_seq;
    unsol_msg->om_data.mode = desc->mode;
    unsol_msg->om_data.length = desc->len;
    ret = memcpy_s(unsol_msg->om_data.data, unsol_msg->om_data.length, desc->data, desc->len);
    if (ret != EOK) {
        DIAG_VCOM_LOGE("memory copy failed, ret=%d\n", ret);
        kfree_skb(unsol_skb);
        entity->stats.put_errors++;
        return ret;
    }

    ret = netlink_unicast(entity->sk, unsol_skb, entity->portid, 0);
    if (ret < 0) {
        DIAG_VCOM_LOGD("Send netlink message failed, ret=%d\n", ret);
        entity->stats.unicast_errors++;
        return DIAG_VCOM_ERROR;
    }

    entity->stats.unicast_message++;
    return DIAG_VCOM_OK;
}

static void diag_vcom_wake_lock(ppm_netlink_info_s *entity)
{
    mod_timer(&g_ppm_netlink_timer, jiffies + DIAG_VCOM_WAKELOCK_TIMEOUT * HZ);
    entity->stats.wakelock_state = 1;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    __pm_stay_awake(entity->lock);
#else
    __pm_stay_awake(&entity->lock);
#endif
    return;
}

static void diag_vcom_wake_unlock(ppm_netlink_info_s *entity)
{
    if (entity->stats.wakelock_state) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
        __pm_relax(entity->lock); //lint !e455
#else
        __pm_relax(&entity->lock); //lint !e455
#endif
        del_timer(&g_ppm_netlink_timer);

        entity->stats.wakelock_state = 0;
    }

    return;
}

/*
 * Prototype: diag_vcom_fragment_om_data
 * Description: Fragment om data.
 */
static int diag_vcom_fragment_om_data(ppm_netlink_info_s *entity, uint8_t *data, uint32_t len, uint8_t mode)
{
    diag_vcom_om_data_desc_s om_data_desc;
    uint32_t blk_num;
    uint32_t tail_blk_size;
    uint32_t i;

    DIAG_VCOM_LOGD("len=%d, mode=%d\n", len, mode);

    blk_num = len / DIAG_VCOM_DATA_SIZE;
    tail_blk_size = len % DIAG_VCOM_DATA_SIZE;

    memset_s(&om_data_desc, sizeof(om_data_desc), 0, sizeof(om_data_desc));
    om_data_desc.blk_num = blk_num + ((tail_blk_size) ? 1 : 0);
    om_data_desc.mode = mode;
    om_data_desc.data = data;
    om_data_desc.len = DIAG_VCOM_DATA_SIZE;

    diag_vcom_wake_lock(entity);

    for (i = 0; i < blk_num; i++) {
        diag_vcom_send_om_data_msg(entity, &om_data_desc);
        om_data_desc.data += DIAG_VCOM_DATA_SIZE;
        om_data_desc.blk_seq++;
    }

    if (tail_blk_size) {
        om_data_desc.len = tail_blk_size;
        diag_vcom_send_om_data_msg(entity, &om_data_desc);
    }

    diag_vcom_wake_unlock(entity);

    return DIAG_VCOM_OK;
}

/*
 * Prototype: diag_vcom_report_om_data
 * Description: Report om data through netlink message.
 */
int diag_vcom_report_om_data(uint32_t channel, uint8_t *data, uint32_t len, uint8_t mode)
{
    ppm_netlink_info_s *entity = NULL;

    DIAG_VCOM_LOGD("channel=%d, len=%d, mode=%d\n", channel, len, mode);

    if (unlikely(channel >= DIAG_VCOM_CHAN_MAX)) {
        DIAG_VCOM_LOGE("Channel[%d] is invalid\n", channel);
        return DIAG_VCOM_ERROR;
    }

    entity = DIAG_VCOM_GET_ENTTIY(channel);

    if (unlikely(!data || !len)) {
        DIAG_VCOM_LOGE("Data or len is invalid\n");
        entity->stats.rx_om_errors++;
        return DIAG_VCOM_ERROR;
    }

    if (!entity->portid) {
        DIAG_VCOM_LOGD("Channel[%d] is not opened\n", channel);
        entity->stats.rx_om_dropped++;
        return DIAG_VCOM_ERROR;
    }

    entity->stats.rx_om_packets++;

    if (channel == DIAG_VCOM_CHAN_CTRL)
        return diag_vcom_send_om_cmd_msg(entity, data, len);
    else
        return diag_vcom_fragment_om_data(entity, data, len, mode);
}

/*
 * Prototype: diag_vcom_associate_port
 * Description: Associate netlink channel.
 */
static void diag_vcom_associate_port(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    ppm_netlink_info_s *ent = NULL;
    diag_vcom_msg_s resp_msg;
    uint32_t i;

    DIAG_VCOM_LOGI("pid=%d,type=0x%x\n", msg->port.pid, msg->message_type);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    if (!msg->port.pid) {
        resp_msg.return_code = DIAG_VCOM_E_INCORRECT_PARAMETERS;
    } else if (!entity->ready) {
        resp_msg.return_code = DIAG_VCOM_E_NOT_READY;
    } else {
        for (i = 0; i < DIAG_VCOM_CHAN_MAX; i++) {
            ent = DIAG_VCOM_GET_ENTTIY(i);
            ent->portid = msg->port.pid;

            if (ent->cb_ops.event_cb)
                ent->cb_ops.event_cb(ent->channel, DIAG_VCOM_EVT_CHAN_OPEN);
        }

        resp_msg.return_code = DIAG_VCOM_E_SUCCESS;
    }

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_unassociate_port
 * Description: Unassociate netlink channel.
 */
static void diag_vcom_unassociate_port(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    ppm_netlink_info_s *ent = NULL;
    diag_vcom_msg_s resp_msg;
    uint32_t i;

    DIAG_VCOM_LOGI("pid=%d.type=0x%x\n", msg->port.pid, msg->message_type);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    for (i = 0; i < DIAG_VCOM_CHAN_MAX; i++) {
        ent = DIAG_VCOM_GET_ENTTIY(i);
        ent->portid = 0;

        if (ent->cb_ops.event_cb)
            ent->cb_ops.event_cb(ent->channel, DIAG_VCOM_EVT_CHAN_CLOSE);
    }

    resp_msg.return_code = DIAG_VCOM_E_SUCCESS;
    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_set_log_port_config
 * Description: Set log port.
 */
static void diag_vcom_set_log_port_config(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;
    uint32_t log_port;

    DIAG_VCOM_LOGI("port=%d,type=0x%x\n", msg->log_port_config.port, msg->message_type);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    log_port = (msg->log_port_config.port == DIAG_VCOM_LOG_PORT_USB) ? DIAG_VCOM_LOG_PORT_USB : DIAG_VCOM_LOG_PORT_VCOM;

    if (bsp_diag_set_log_port(log_port) != 0)
        resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
    else
        resp_msg.return_code = DIAG_VCOM_E_SUCCESS;

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_get_log_port_config
 * Description: Get current log port.
 */
static void diag_vcom_get_log_port_config(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;
    uint32_t log_port = DIAG_VCOM_LOG_PORT_USB;

    DIAG_VCOM_LOGI("port=%d,type=0x%x\n", msg->log_port_config.port, msg->message_type);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    if (bsp_diag_get_log_port(&log_port) != 0) {
        DIAG_VCOM_LOGE("Query log port failed\n");
        resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
    } else {
        resp_msg.op = DIAG_VCOM_MSG_RETURN_DATA;
        resp_msg.args_length = DIAG_VCOM_MSG_ARGS_SIZE(log_port_config);
        resp_msg.log_port_config.port =
            (log_port == DIAG_VCOM_LOG_PORT_USB) ? DIAG_VCOM_LOG_PORT_USB : DIAG_VCOM_LOG_PORT_VCOM;
    }

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_set_log_config
 * Description: Set log mode.
 */
static void diag_vcom_set_log_config(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;
    uint32_t log_port = DIAG_VCOM_LOG_PORT_USB;
    socp_ind_mode_e mode;

    DIAG_VCOM_LOGH("mode=%d,type=0x%x\n", msg->log_config.mode, msg->message_type);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;
    resp_msg.return_code = DIAG_VCOM_E_SUCCESS;

    if (bsp_diag_get_log_port(&log_port) != 0) {
        DIAG_VCOM_LOGE("Query log port failed\n");
        resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
    } else {
        if (log_port == DIAG_VCOM_LOG_PORT_USB) {
            DIAG_VCOM_LOGE("Current log port is USB\n");
            resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
        } else {
            switch (msg->log_config.mode) {
                case DIAG_VCOM_LOG_MODE_DIRECT:
                case DIAG_VCOM_LOG_MODE_DELAY:
                case DIAG_VCOM_LOG_MODE_CYCLE:
                    mode = (socp_ind_mode_e)msg->log_config.mode;
                    if (mdrv_socp_set_cfg_ind_mode(mode) != 0) {
                        DIAG_VCOM_LOGE("Set ind mode failed\n");
                        resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
                    }
                    break;

                default:
                    resp_msg.return_code = DIAG_VCOM_E_INCORRECT_PARAMETERS;
                    break;
            }
        }
    }

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_get_log_config
 * Description: Get log mode.
 */
static void diag_vcom_get_log_config(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;
    uint32_t log_port = DIAG_VCOM_LOG_PORT_USB;
    uint32_t mode = 0;

    DIAG_VCOM_LOGI("mode=%d,type=0x%x\n", msg->log_config.mode, msg->message_type);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    if (bsp_diag_get_log_port(&log_port) != 0) {
        DIAG_VCOM_LOGE("Query log port failed\n");
        resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
    } else {
        DIAG_VCOM_LOGH("Current log port=%d\n", log_port);

        if (log_port == DIAG_VCOM_LOG_PORT_USB) {
            mode = bsp_socp_get_ind_chan_mode();
        } else {
#ifdef CONFIG_DEFLATE
            DIAG_VCOM_LOGI("deflate feature on\n");
            mode = bsp_deflate_get_ind_chan_mode();
#else
            DIAG_VCOM_LOGI("deflate feature off\n");
            mode = bsp_socp_get_ind_chan_mode();
#endif
        }

        resp_msg.op = DIAG_VCOM_MSG_RETURN_DATA;
        resp_msg.args_length = DIAG_VCOM_MSG_ARGS_SIZE(log_config);
        resp_msg.log_config.mode = mode;
    }

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_set_log_nve_config
 * Description: Set log nve config.
 */
#ifdef BSP_CONFIG_PHONE_TYPE
static void diag_vcom_set_log_nve_config(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;
    struct hisi_nve_info_user nve_config;

    DIAG_VCOM_LOGI("NVE feature on, enable=%d\n", msg->log_nve_config.enable);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    memset_s(&nve_config, sizeof(nve_config), 0, sizeof(nve_config));
    nve_config.nv_number = 285;
    nve_config.valid_size = 4;
    nve_config.nv_operation = NV_READ;

    if (hisi_nve_direct_access(&nve_config) != 0) {
        DIAG_VCOM_LOGE("Read NVE failed\n");
        resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
    } else {
        nve_config.nv_operation = NV_WRITE;
        nve_config.nv_data[0] = (msg->log_nve_config.enable == true) ? 0x31 : 0x30;

        if (hisi_nve_direct_access(&nve_config) != 0) {
            DIAG_VCOM_LOGE("Write NVE failed\n");
            resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
        } else {
            resp_msg.return_code = DIAG_VCOM_E_SUCCESS;
        }
    }

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}
#else
static void diag_vcom_set_log_nve_config(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;

    DIAG_VCOM_LOGI("NVE feature off, enable=%d\n", msg->log_nve_config.enable);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;
    resp_msg.return_code = DIAG_VCOM_E_SUCCESS;

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}
#endif

/*
 * Prototype: diag_vcom_get_log_nve_config
 * Description: Get log nve config.
 */
static void diag_vcom_get_log_nve_config(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;
    socp_encdst_buf_log_cfg_s log_cfg;

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    memset_s(&log_cfg, sizeof(log_cfg), 0, sizeof(log_cfg));

    if (mdrv_socp_get_sd_logcfg(&log_cfg) != 0) {
        resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
    } else {
        resp_msg.op = DIAG_VCOM_MSG_RETURN_DATA;
        resp_msg.args_length = DIAG_VCOM_MSG_ARGS_SIZE(log_nve_config);
        resp_msg.log_nve_config.enable = (log_cfg.log_on_flag == SOCP_DST_CHAN_DELAY) ? true : false;
    }

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_send_om_cmd_request
 * Description: Send om command to diag.
 */
static void diag_vcom_send_om_cmd_request(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;
    uint32_t ret, real_len;

    DIAG_VCOM_LOGI("length=%d,type=0x%x\n", msg->om_command.length, msg->message_type);

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;

    real_len = msg->args_length - DIAG_VCOM_MSG_ARGS_SIZE(om_command);
    if (real_len < msg->om_command.length) {
        resp_msg.return_code = DIAG_VCOM_E_INCORRECT_PARAMETERS;
        diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
        return;
    }

    if (entity->cb_ops.data_rx_cb) {
        ret = entity->cb_ops.data_rx_cb(entity->channel, msg->om_command.data, msg->om_command.length);
        if (ret) {
            entity->stats.tx_om_errors++;
            resp_msg.return_code = DIAG_VCOM_E_GENERIC_FAILURE;
            diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
        } else {
            entity->stats.tx_om_packets++;
        }
    }
}

/*
 * Prototype: diag_vcom_process_unknown_msg
 * Description: Process unknown message.
 */
static void diag_vcom_process_unknown_msg(ppm_netlink_info_s *entity, diag_vcom_msg_s *msg)
{
    diag_vcom_msg_s resp_msg;

    DIAG_VCOM_LOGH("Unknow message\n");

    memset_s(&resp_msg, sizeof(resp_msg), 0, sizeof(resp_msg));
    resp_msg.message_type = msg->message_type;
    resp_msg.op = DIAG_VCOM_MSG_RETURN_CODE;
    resp_msg.return_code = DIAG_VCOM_E_UNKNOWN_MESSAGE;

    diag_vcom_send_resp_msg(entity, &resp_msg, sizeof(resp_msg));
}

/*
 * Prototype: diag_vcom_validate_msg
 * Description: Validate request message.
 */
static int diag_vcom_validate_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = NULL;
    diag_vcom_msg_s *msg = NULL;
    uint32_t real_len;

    nlh = nlmsg_hdr(skb);
    if (!NLMSG_OK(nlh, skb->len)) {
        DIAG_VCOM_LOGE("Skb len %d is invalid\n", skb->len);
        return DIAG_VCOM_ERROR;
    }

    if (nlmsg_len(nlh) < (int)sizeof(diag_vcom_msg_s)) {
        DIAG_VCOM_LOGE("Netlink message len %d is invalid\n", nlh->nlmsg_len);
        return DIAG_VCOM_ERROR;
    }

    msg = (diag_vcom_msg_s *)nlmsg_data(nlh);
    if (msg->op != DIAG_VCOM_MSG_COMMAND) {
        DIAG_VCOM_LOGE("Message[%d] op %d is invalid\n", msg->message_type, msg->op);
        return DIAG_VCOM_ERROR;
    }

    real_len = (uint32_t)(nlmsg_len(nlh) - DIAG_VCOM_MSG_OFFSETOF(args_start));
    if (real_len < msg->args_length) {
        DIAG_VCOM_LOGE("Message[%d] args len %d is invalid\n", msg->message_type, msg->args_length);
        return DIAG_VCOM_ERROR;
    }

    return DIAG_VCOM_OK;
}

/*
 * Prototype: diag_vcom_ctrl_msg_handler
 * Description: Netlink message handle function.
 */
void diag_vcom_ctrl_msg_handler(struct sk_buff *skb)
{
    ppm_netlink_info_s *entity = DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_CTRL);
    struct nlmsghdr *nlh = NULL;
    diag_vcom_msg_s *msg = NULL;

    if (diag_vcom_validate_msg(skb) != DIAG_VCOM_OK) {
        DIAG_VCOM_LOGE("Message is invalid\n");
        entity->stats.input_errors++;
        return;
    }

    entity->stats.input_message++;

    nlh = nlmsg_hdr(skb);
    msg = (diag_vcom_msg_s *)nlmsg_data(nlh);

    DIAG_VCOM_LOGI("Message[%d] op=%d, arg_len=%d, nlmsg_pid=%d, nlmsg_len=%d\n", msg->message_type, msg->op,
        msg->args_length, nlh->nlmsg_pid, nlh->nlmsg_len);

    entity->resp_pid = nlh->nlmsg_pid;

    switch (msg->message_type) {
        case DIAG_VCOM_REQUEST_ASSOCIATE_PORT:
            diag_vcom_associate_port(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_UNASSOCIATE_PORT:
            diag_vcom_unassociate_port(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_SET_LOG_PORT_CONFIG:
            diag_vcom_set_log_port_config(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_GET_LOG_PORT_CONFIG:
            diag_vcom_get_log_port_config(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_SET_LOG_CONFIG:
            diag_vcom_set_log_config(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_GET_LOG_CONFIG:
            diag_vcom_get_log_config(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_SET_LOG_NVE_CONFIG:
            diag_vcom_set_log_nve_config(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_GET_LOG_NVE_CONFIG:
            diag_vcom_get_log_nve_config(entity, msg);
            break;

        case DIAG_VCOM_REQUEST_SEND_OM_COMMAND:
            diag_vcom_send_om_cmd_request(entity, msg);
            break;

        default:
            diag_vcom_process_unknown_msg(entity, msg);
            break;
    }

    DIAG_VCOM_LOGH("Message[%d] is processed\n", msg->message_type);
}

/*
 * Prototype: diag_vcom_data_msg_handler
 * Description: Netlink message handle function.
 */
void diag_vcom_data_msg_handler(struct sk_buff *skb)
{
    ppm_netlink_info_s *entity = DIAG_VCOM_GET_ENTTIY(DIAG_VCOM_CHAN_DATA);
    struct nlmsghdr *nlh = NULL;
    diag_vcom_msg_s *msg = NULL;

    if (diag_vcom_validate_msg(skb) != DIAG_VCOM_OK) {
        DIAG_VCOM_LOGE("Drop invalid message\n");
        entity->stats.input_errors++;
        return;
    }

    entity->stats.input_message++;

    nlh = nlmsg_hdr(skb);
    msg = (diag_vcom_msg_s *)nlmsg_data(nlh);

    DIAG_VCOM_LOGI("Message: pid=%d, len=%d, msg_type=%d, op=%d, arg_len=%d\n", nlh->nlmsg_pid, nlh->nlmsg_len,
        msg->message_type, msg->op, msg->args_length);

    entity->resp_pid = nlh->nlmsg_pid;

    diag_vcom_process_unknown_msg(entity, msg);

    DIAG_VCOM_LOGH("Message[%d] is processed\n", msg->message_type);
}

/*
 * Prototype: diag_vcom_show_stats
 * Description: Show diag vcom statistics.
 */
void diag_vcom_show_stats(void)
{
    diag_vcom_stats_s *stats = NULL;
    uint32_t i;

    for (i = 0; i < DIAG_VCOM_CHAN_MAX; i++) {
        stats = &g_ppm_netlink_info[i].stats;
        DIAG_VCOM_LOGH("[CH:%d] input_message:   %10u\n", i, stats->input_message);
        DIAG_VCOM_LOGH("[CH:%d] input_errors:%10u\n", i, stats->input_errors);
        DIAG_VCOM_LOGH("[CH:%d] tx_om_packets:   %10u\n", i, stats->tx_om_packets);
        DIAG_VCOM_LOGH("[CH:%d] tx_om_errors:    %10u\n", i, stats->tx_om_errors);
        DIAG_VCOM_LOGH("[CH:%d] rx_om_packets:   %10u\n", i, stats->rx_om_packets);
        DIAG_VCOM_LOGH("[CH:%d] rx_om_errors:    %10u\n", i, stats->rx_om_errors);
        DIAG_VCOM_LOGH("[CH:%d] rx_om_dropped:   %10u\n", i, stats->rx_om_dropped);
        DIAG_VCOM_LOGH("[CH:%d] alloc_errors:    %10u\n", i, stats->alloc_errors);
        DIAG_VCOM_LOGH("[CH:%d] put_errors:      %10u\n", i, stats->put_errors);
        DIAG_VCOM_LOGH("[CH:%d] unicast_errors:  %10u\n", i, stats->unicast_errors);
        DIAG_VCOM_LOGH("[CH:%d] unicast_message: %10u\n", i, stats->unicast_message);
    }
}
EXPORT_SYMBOL(diag_vcom_show_stats);
