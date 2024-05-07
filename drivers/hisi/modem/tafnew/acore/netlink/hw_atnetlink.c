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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <asm/uaccess.h>
#include <net/sock.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <device_event_dispatch_define.h>
#include "securec.h"

#if (FEATURE_IOT_ATNLPROXY== FEATURE_ON)
struct workqueue_struct *g_at_netlink_wq = NULL;  // 事件上报用工作队列

struct sock *g_at_device_event_sock;
u32 g_at_us_pid = (unsigned int)(-1);

struct delayed_work g_at_device_event_send_work;

#define AT_DELAY_FOR_NL_SEND msecs_to_jiffies(1)

// Device event buffer, to avoid blocking by socket
struct list_head g_at_device_event_queue;
spinlock_t g_at_queue_lock;

struct at_device_event_node {
	struct list_head list;
	int len;
	char data[0];
} at_device_event_node;

/* Limit the event buffer queue length,
 * Otherwise, this queue may consuming out all of memory
 */
int g_at_event_queue_len = 0;
#define AT_MAX_EVENT_QUEUE_LEN 300

/* Device callback
 * Used to dispatch message from userspace
 */
device_event_dispatch g_at_device_event_handler[NL_DEVICE_ID_MAX_ID] = {0};

/**
 *
 * Register call back
 *
 */
int at_device_event_handler_register(NL_DEVICE_ID id, device_event_dispatch dispatcher)
{
	if (((int)id < 0) || (id >= NL_DEVICE_ID_MAX_ID) || (dispatcher == NULL)) {
		return NETLINK_FALSE;
	}

	g_at_device_event_handler[id] = dispatcher;

	return NETLINK_TRUE;
}

EXPORT_SYMBOL_GPL(at_device_event_handler_register);

/**
 *
 * Construct a buffer node
 *
 */
static struct at_device_event_node *at_device_event_node_alloc(void * data, int len)
{
	struct at_device_event_node *node = NULL;
	int ret = 0;

	unsigned long flags;

	spin_lock_irqsave(&g_at_queue_lock, flags);
	if (g_at_event_queue_len >= AT_MAX_EVENT_QUEUE_LEN) {
		spin_unlock_irqrestore(&g_at_queue_lock, flags);
		return NULL;
	}
	spin_unlock_irqrestore(&g_at_queue_lock, flags);

	node = (struct at_device_event_node *)kmalloc((len + sizeof(struct at_device_event_node)), GFP_ATOMIC);
	if (node == NULL) {
		return NULL;
	}

	node->len = len;
	if (len > 0) {
		ret = memcpy_s(node->data, node->len, data, len);
		if (ret) {
			printk("at_device_event_node_alloc: memcpy error\n");
		}
	}

	spin_lock_irqsave(&g_at_queue_lock, flags);
	g_at_event_queue_len++;
	spin_unlock_irqrestore(&g_at_queue_lock, flags);

	return node;
}

/* Destruct a buffer node */
static inline void at_device_event_node_free(struct at_device_event_node *node)
{
	g_at_event_queue_len--;

	kfree(node);
}

/* Put a device event into event buffer */
static inline void at_device_event_enqueue(struct at_device_event_node *node)
{
	unsigned long flags;

	spin_lock_irqsave(&g_at_queue_lock, flags);
	list_add_tail(&node->list, &g_at_device_event_queue);
	spin_unlock_irqrestore(&g_at_queue_lock, flags);
}

/**
 *
 * Get a device event from event buffer
 * Notice: list should have at least one entry!
 *
 */
static struct at_device_event_node *at_device_event_dequeue(void)
{
	struct at_device_event_node *node = NULL;
	unsigned long flags;

	spin_lock_irqsave(&g_at_queue_lock, flags);
	node = list_first_entry(&g_at_device_event_queue, struct at_device_event_node, list);
	if (node == NULL) {
		spin_unlock_irqrestore(&g_at_queue_lock, flags);
		return NULL;
	}

	list_del(&node->list);
	spin_unlock_irqrestore(&g_at_queue_lock, flags);

	return node;
}

/**
 *
 * Check whether the device event buffer queue is empty
 *
 */
static inline bool at_device_event_queue_empty(void)
{
	unsigned long flags;
	bool ret = 0;

	spin_lock_irqsave(&g_at_queue_lock, flags);
	ret = list_empty(&g_at_device_event_queue);
	spin_unlock_irqrestore(&g_at_queue_lock, flags);

	return ret;
}

/**
 *
 * Process the message sent from user space
 *
 */
static void at_device_event_input(struct sk_buff *__skb)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh = NULL;
	NL_DEVICE_EVENT *msg_body = NULL;
	device_event_dispatch dispatcher = NULL;

	skb = skb_get(__skb);
	if (skb->len < NLMSG_SPACE(0)) {
		return;
	}

	nlh = nlmsg_hdr(skb);

	// Store the user space daemon pid
	g_at_us_pid = nlh->nlmsg_pid;

	queue_delayed_work(g_at_netlink_wq, &g_at_device_event_send_work, AT_DELAY_FOR_NL_SEND);

	// Dispatch message to other device driver ...
	msg_body = NLMSG_DATA(nlh);

	if ((msg_body->device_id < 0) || (msg_body->device_id >= NL_DEVICE_ID_MAX_ID)) {
		return;
	}

	dispatcher = g_at_device_event_handler[msg_body->device_id];
	if (likely(dispatcher != NULL)) {
		dispatcher(msg_body, msg_body->len + sizeof(NL_DEVICE_EVENT));
	} else {
		printk(KERN_ERR "at_device_event_input: event handler is NOT registered\n");
	}

	kfree_skb(skb);

	return;
}

extern int netlink_unicast(struct sock *ssk, struct sk_buff *skb,
	u32 pid, int nonblock);
/**
 *
 * Send message to user space
 *
 */
static void at_device_event_send(struct work_struct *w)
{
	struct at_device_event_node *node = NULL;
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh = NULL;
	int size;
	int ret;

	while (!at_device_event_queue_empty()) {
		printk("at_device_event_send: queue is NOT empty\n");

		// Get one entry from event buffer queue
		node = at_device_event_dequeue();
		if (node == NULL) {
			break;
		}

		/*
		 * node->len: size of (node->data), is event message length
		 * size: size of (skb)
		 * size = sizeof(*nlh) + align pad + node->len + aligh pad
		 *
		 * alloc skb here
		 * But, NOT release skb here
		 *
		 */
		size = NLMSG_SPACE(node->len);
		skb = alloc_skb((unsigned int)size, GFP_ATOMIC);
		if (skb == NULL) {
			printk(KERN_ALERT "at_device_event_send: alloc skb failed\n");
			return;
		}

		// Use "size - sizeof(*nlh)" here (incluing align pads)
		nlh = nlmsg_put(skb, 0, 0, 0, size - sizeof(*nlh), 0);

		NETLINK_CB(skb).portid = 0;
		NETLINK_CB(skb).dst_group = 0;
		if (node->len > 0) {
			memcpy_s(NLMSG_DATA(nlh), nlh->nlmsg_len - NLMSG_HDRLEN, node->data, node->len);
		}

		// Release this event entry
		at_device_event_node_free(node);

		// Send message to user space
		ret = netlink_unicast(g_at_device_event_sock, skb, g_at_us_pid, 0);
		if (ret < 0) {
			return;
		}
	}
}

/**
 *
 * Device driver use this interface to report an event
 * NOTICE: This interface may be called in interrupt or in process context
 *
 */
int at_device_event_report(void *data, int len)
{
	struct at_device_event_node *node = NULL;

	if (unlikely(g_at_device_event_sock == NULL)) {
		return NETLINK_FALSE;
	}

	node = at_device_event_node_alloc(data, len);
	if (node == NULL) {
		return NETLINK_FALSE;
	}

	at_device_event_enqueue(node);

	/*
	 * If device driver report event before daemon was started,
	 * Only put event into buffer queue.
	 */
	if (likely(-1 != g_at_us_pid)) {
		/* 将工作任务加入到工作队列中 */
		queue_delayed_work(g_at_netlink_wq, &g_at_device_event_send_work, AT_DELAY_FOR_NL_SEND);
	}

	return NETLINK_TRUE;
}
void test_at_device_event_report(void)
{
	char *data = "haha";
	at_device_event_report(data, (int)strlen(data));
}
EXPORT_SYMBOL_GPL(at_device_event_report);

static int __init at_device_event_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.groups = 32,
		.input = at_device_event_input,
	};
	/* 创建netlink用工作队列 */
	g_at_netlink_wq = create_workqueue("AT_NET_LINK_WQ");
	if (g_at_netlink_wq == NULL) {
		printk(KERN_ALERT "kernel create NET_LINK_WQ workqueue failed.\n");
		return -EIO;
	}

	INIT_LIST_HEAD(&g_at_device_event_queue);
	INIT_DELAYED_WORK(&g_at_device_event_send_work, at_device_event_send);
	spin_lock_init(&g_at_queue_lock);

	// Create a socket
	g_at_device_event_sock = netlink_kernel_create(&init_net, NETLINK_ATNLPROXY, &cfg);
	if (!g_at_device_event_sock) {
		printk(KERN_ALERT "kernel create netlink failed.\n");
		return -EIO;
	}

	return NETLINK_TRUE;
}

static void __exit at_device_event_exit(void)
{
	cancel_delayed_work_sync(&g_at_device_event_send_work);

	/* 清空工作队列并释放资源 */
	flush_workqueue(g_at_netlink_wq);
	destroy_workqueue(g_at_netlink_wq);
	g_at_netlink_wq = NULL;

	netlink_kernel_release(g_at_device_event_sock);
}

subsys_initcall(at_device_event_init);
module_exit(at_device_event_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("balong.iot");
MODULE_DESCRIPTION("Netlink module for kernel<->userspace messge.");

#endif

