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
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/netlink.h>
#include <asm/uaccess.h>
#include <net/sock.h>

#include <device_event_dispatch_define.h>

#if (FEATURE_IOT_ATNLPROXY== FEATURE_ON)
/* 事 件定义 */
typedef enum _NL_MSG_EVENT {
	NL_EVENT_MIN = -1,        /* 事件边界值 */
	NL_CONNECT_TO_KERNEL = 0, /* 连接内核 */
	NL_NORMALRESPONSE = 1,    /* 正常的AT回复 */
	NL_EVENT_MAX              /* 事件边界值 */
} NL_MSG_EVENT;

static int atproxy_receive_from_app(void *buffer, int length)
{
	void *pVoid = NULL;
	int device_id;

	NL_DEVICE_EVENT *pEvent = (NL_DEVICE_EVENT *)buffer;
	if (pEvent == NULL) {
		return NETLINK_FALSE;
	}

	device_id = pEvent->device_id;

	/* 连接内核的netlink并注册AT */
	if (pEvent->event_code == NL_CONNECT_TO_KERNEL) {
		printk("atproxy_receive_from_app receive NL_CONNECT_TO_KERNEL\r\n");
		pVoid = (void *)pEvent->data;
		AtnlproxyRegisterToKernel(device_id, pVoid, pEvent->len);
	} else if (pEvent->event_code == NL_NORMALRESPONSE) {
		/* 所有的用户态的AT上报和AT应答都在这里处理 */
		AtnlproxyDispatchUspaceData(pEvent->data, pEvent->len);

	} else {
	}

	return NETLINK_TRUE;
}

static void atproxy_kernel_exit(void)
{
	int ret;
	/* unregister event dispatcher first */
	ret = at_device_event_handler_register(NL_DEVICE_ID_ATNLPROXT, NULL);
	if (ret != 0) {
		printk(KERN_ERR "atproxy_kernel_exit:unregister event handler failed\r\n");
	}

	return;
}

static int atproxy_kernel_init(void)
{
	/* register callback function to netlink module */
	if (at_device_event_handler_register(NL_DEVICE_ID_ATNLPROXT, atproxy_receive_from_app) != NETLINK_TRUE) {
		printk(KERN_ERR "atproxy_kernel_init:register event handler failed\r\n");
		return NETLINK_FALSE;
	}
	atnlproxy_init();
	return NETLINK_TRUE;
}

module_init(atproxy_kernel_init);
module_exit(atproxy_kernel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("balong.iot");
MODULE_DESCRIPTION("process monitor----report event when specified processes exit");

#endif

