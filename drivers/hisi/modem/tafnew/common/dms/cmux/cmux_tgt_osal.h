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

#ifndef CMUX_TGT_OSAL_H
#define CMUX_TGT_OSAL_H

#if (VOS_OS_VER != VOS_LINUX)
#include "Linuxstub.h"
#endif
#if (defined(LLT_OS_VER) && (LLT_OS_VER == LLT_WIN))
#include "skbuff.h"
#else
#ifdef WIN32
#include <windows.h>
#else
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#endif
#endif
#include "cmux_cmd.h"
#include "cmux_api_ifc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if (FEATURE_IOT_CMUX == FEATURE_ON)
#if (defined(LLT_OS_VER))
#define STATIC
#else
#define STATIC static
#endif

#define HEX_NUMBER_LEN 16

#define MUTEX_PHY_LOCK() mutex_lock(&g_port_mutex_phy);
#define MUTEX_PHY_UNLOCK() mutex_unlock(&g_port_mutex_phy);

#define MUTEX_RECV_LOCK() mutex_lock(&g_port_mutex_recv);
#define MUTEX_RECV_UNLOCK() mutex_unlock(&g_port_mutex_recv);

#define MUTEX_RECVDATA_LOCK() mutex_lock(&g_port_mutex_recvdata);
#define MUTEX_RECVDATA_UNLOCK() mutex_unlock(&g_port_mutex_recvdata);

#define MUTEX_SEND_LOCK() mutex_lock(&g_port_mutex_send);
#define MUTEX_SEND_UNLOCK() mutex_unlock(&g_port_mutex_send);



typedef enum {
    CMUX_DEBUG_INFO,
    CMUX_DEBUG_WARN,
    CMUX_DEBUG_ERR,
    CMUX_DEBUG_MAX
} cmux_debug_status;
typedef int (*timer_cb_t)(CMUXLIB* cm, unsigned char dlc);
typedef struct {
    CMUXLIB*            cm;
    int                 dlc_tmr;
    int                 rev;
    struct delayed_work timer;
    timer_cb_t          timer_cb;
} cmux_osal_timer_type;

#ifdef WIN32
#define CMUX_PRINT(level, fmt, ...) do {\
    pr_err("%s "fmt"\n",__FUNCTION__,__LINE__,##__VA_ARGS__);\
}while(0)
#else
#if (!defined(LLT_OS_VER))
#define CMUX_PRINT(level, fmt, arg0...) do {\
    if (level >= cmux_get_debug_level()) { \
        printk(KERN_ERR"%s,%d, "fmt"\n",__FUNCTION__,__LINE__,##arg0);\
    } \
}while(0)
#else
#define CMUX_PRINT(level, fmt, arg0...) do {\
    if (level >= cmux_get_debug_level()) { \
        printf("CMUX_PRINT_linux ");\
    } \
}while(0)
#endif
#endif

#ifdef WIN32
extern int cb_send_to_com(char* com_name, unsigned char* rawptr, unsigned short len);
#else
extern int cb_send_to_com(struct file* fp, unsigned char* rawptr, unsigned short len);
#endif

unsigned char* cmux_alloc(int size);
int cmux_free(unsigned char*);
unsigned char* cmux_alloc_skb(int size);
void cmux_put_skb(unsigned char* dest, const unsigned char* src, unsigned int size);
unsigned char* cmux_get_skb(unsigned char* src);
void cmux_hex_dump(cmux_debug_status level, unsigned char* buf, unsigned int len);
void cmux_free_skb(unsigned char* buff);
void cmux_stop_timer(struct cmuxlib*, unsigned char);
int cmux_start_timer(struct cmuxlib*, unsigned char, struct cmux_timer*,
                                int (*cmux_rxed_timer_expiry)(struct cmuxlib* cm, unsigned char dlc));

/* Description: 初始化超时定时器，注册回调(适配balong平台) */
void cmux_init_timer(struct delayed_work* cmux_timer, void (*timer_expiry_cb)(struct work_struct* w));

/* Description: 启动超时定时器(适配balong平台) */
void cmux_timer_begin(struct delayed_work* cmux_timer, int time_val);

/* Description: 关闭超时定时器(适配balong平台) */
void cmux_timer_end(struct delayed_work* cmux_timer);

unsigned char cmux_get_active_ppp_pstask_dlc(void);
void cmux_set_active_ppp_pstask_dlc(unsigned char dlc);
unsigned int cmux_get_debug_level(void);
void cmux_debug_level(unsigned int level);

#ifndef NULL
#define NULL 0
#endif

#ifdef WIN32
#else
#define printf pr_err
#endif

#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* CMUX_TGT_OSAL_H */
