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

#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else
#include <linux/mutex.h>
#include <linux/skbuff.h>
extern struct mutex g_port_mutex_phy;
extern unsigned int mutex_flag_phy;
extern struct mutex g_port_mutex_send;
extern unsigned int mutex_flag_send;
extern struct mutex g_port_mutex_recv;
extern unsigned int mutex_flag_recv;
extern struct mutex g_port_mutex_recvdata;
extern unsigned int mutex_flag_recvdata;
#endif

#include "cmux_framer.h"
#include "cmux_tgt_osal.h"
#include "cmux_drv_ifc.h"
#include "securec.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#if (FEATURE_IOT_CMUX == FEATURE_ON)

/* Set the currently active DLC */
unsigned char g_cmux_active_dstask_dlc = 0;
/* Set the currently active DLC */
unsigned char g_cmux_last_active_dstask_dlc = 0;
unsigned char g_cmux_active_ppp_pstask_dlc  = 0;

struct cmux_callback g_callbk;

#define FEATURE_MEM_DEBUG

cmux_osal_timer_type g_cmux_tmr_container;

int g_timer_init = 0;

int g_dlc_tmr = -1;

unsigned int g_cmux_dbg_level = CMUX_DEBUG_ERR; /*lint !e18*/

int (*cmux_rxed_timer_expiry_g)(CMUXLIB* cm, unsigned char dlc);

#define CMUX_HEAP_SIZE (10 * 1024)

unsigned int g_cmux_alloc_count = 0;
unsigned int g_cmux_free_count  = 0;

unsigned int g_cmux_mem_usage = 0;


unsigned char cmux_get_active_ppp_pstask_dlc(void)
{
    return g_cmux_active_ppp_pstask_dlc;
}

void cmux_set_active_ppp_pstask_dlc(unsigned char dlc)
{
    g_cmux_active_ppp_pstask_dlc = dlc;
}

/* Description:    初始化超时定时器，注册回调(适配balong平台) */
void cmux_init_timer(struct delayed_work* cmux_timer, void (*timer_expiry_cb)(struct work_struct* w))
{
    INIT_DELAYED_WORK(cmux_timer, timer_expiry_cb);
}

/* Description:    启动超时定时器(适配balong平台) */
void cmux_timer_begin(struct delayed_work* cmux_timer, int time_val)
{
    schedule_delayed_work(cmux_timer, msecs_to_jiffies((unsigned int)time_val));
}

/* Description:    关闭超时定时器(适配balong平台) */
void cmux_timer_end(struct delayed_work* cmux_timer)
{
    cancel_delayed_work_sync(cmux_timer);
}

STATIC void cmuxtask_timer_cb(struct work_struct* w)
{
    cmux_osal_timer_type* con_ptr = container_of(to_delayed_work(w), cmux_osal_timer_type, timer); /*lint !e102 !e42*/

    if ((con_ptr->timer_cb != NULL) && (con_ptr->cm != NULL)) {
        (*con_ptr->timer_cb)(con_ptr->cm, (unsigned char)con_ptr->dlc_tmr);
    }
}

/*
 * Memory Related Porting
 * Following piece of code gets enabled only for target side
 */
int cmux_start_timer(CMUXLIB* cm, unsigned char dlc, struct cmux_timer* ct,
                     int (*cmux_rxed_timer_expiry)(CMUXLIB* cm, unsigned char dlc)) /*lint !e78*/
{                                                                                   /*lint !e527*/
    CMUX_FrTypeUint8 ftype = 0;
    unsigned int     timeout_cm = 0;

    CMUX_PRINT(CMUX_DEBUG_INFO, "should start timer\n");
    if ((cm == NULL) || (ct == NULL)) {
        return FAIL;
    }
    if ((dlc == 0) && (cm->dlc_db[0] != NULL) && (cm->dlc_db[0]->cmd_list)) {
        ftype = cm->dlc_db[0]->cmd_list->fr->f_type;
    }
    if ((ftype == CLD_CMD) || (ftype == FCON_CMD) || (ftype == FCOFF_CMD)) {
        timeout_cm                    = ct->res_timer_t2;
        g_cmux_tmr_container.timer_cb = cmux_rxed_timer_expiry;
        g_cmux_tmr_container.dlc_tmr  = (int)dlc;
        g_cmux_tmr_container.cm       = cm;
        if (!g_timer_init) {
            INIT_DELAYED_WORK(&g_cmux_tmr_container.timer, cmuxtask_timer_cb);
            g_timer_init = 1;
        }
        CMUX_PRINT(CMUX_DEBUG_INFO, "going to start timer\n");
        schedule_delayed_work(&g_cmux_tmr_container.timer, msecs_to_jiffies(timeout_cm));
        return 0;
    }

    if (cm->dlc_db[dlc] == NULL) {
        return FAIL;
    }
    cm->dlc_db[dlc]->dlc = dlc; /*lint !e63*/
    ct->retransmit_n2    = 0;   /*lint !e63*/
    return 0;
} /* cmux_start_timer */

void cmux_stop_timer(CMUXLIB* cm, unsigned char dlc) /*lint !e527*/
{
    if (cm->dlc_db[dlc] == NULL) {
        return;
    }
    cm->dlc_db[dlc]->dlc = dlc;
    CMUX_PRINT(CMUX_DEBUG_INFO, "going to cancel timer\n");
    cancel_delayed_work_sync(&g_cmux_tmr_container.timer);
}

/* used for allocating the buffer */
unsigned char* cmux_alloc(int size)
{
    void          *ptr  = NULL;
    unsigned char *buff = NULL;

    if (size < 0) {
        return FAIL;
    }

#ifdef WIN32
    ptr = (void*)malloc((unsigned int)size);
#else
    ptr = (void*)kmalloc((unsigned int)size, GFP_KERNEL);
#endif

    if (ptr == NULL) {
        return FAIL;
    }
    memset_s((void *)ptr, (unsigned int)size, 0, (unsigned int)size); /*lint !e506 !e522*/
    buff = (unsigned char*)ptr;
    return buff;
}

int cmux_free(unsigned char* buff)
{
    if (buff == NULL) {
        return FAIL;
    }
#ifdef WIN32
    free(buff);
#else
    kfree(buff);
#endif
    return PASS;
}
/* Description:  申请SKB内存(适配BALONG平台) */
unsigned char* cmux_alloc_skb(int size)
{
    struct sk_buff* pSkb;
    pSkb = dev_alloc_skb((unsigned int)size);
    if (pSkb == NULL) {
        return FAIL;
    }
    (void)memset_s((void *)pSkb->data, (size_t)size, 0, (size_t)size); /*lint !e506*/
    return (unsigned char*)pSkb;
}
/* Description:  将*src中的长度为size的数据放入到dest中(适配BALONG平台) */
void cmux_put_skb(unsigned char* dest, const unsigned char* src, unsigned int size)
{
    struct sk_buff* pSkb = (struct sk_buff*)dest;
    int             ret;

    if ((pSkb == NULL) || (src == NULL)) {
        return;
    }
    /* 修改SKB的长度 */
    skb_put(pSkb, size);
    /* 将源数据拷贝入SKB */
    if (size > 0) {
        ret = memcpy_s(pSkb->data, (size_t)pSkb->len, src, (size_t)size);
        CMUX_MEM_CHK_RTN_VAL(ret);
    }
}
/* Description:  将*src中的长度为size的数据放入到dest中(适配BALONG平台) */
unsigned char* cmux_get_skb(unsigned char* src)
{
    struct sk_buff* pSkb = (struct sk_buff*)src;
    return (unsigned char*)pSkb->data;
}

/* Description:  释放SKB内存(适配BALONG平台) */
void cmux_free_skb(unsigned char* buff)
{
    if (buff == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "buff is NULL!!\n");
        return;
    }

    kfree_skb((struct sk_buff*)buff);
}

/* Description:  设置CMUX log打印级别(适配BALONG平台) */
void cmux_debug_level(unsigned int level)
{
    g_cmux_dbg_level = level;
}
unsigned int cmux_get_debug_level(void)
{
    return g_cmux_dbg_level;
}

/* Description:  打印CMUX的raw数据的接口(适配BALONG平台) */
void cmux_hex_dump(cmux_debug_status level, unsigned char* buf, unsigned int len)
{
    unsigned int idx;

    if (level < (cmux_debug_status)cmux_get_debug_level()) {
        return;
    }

    CMUX_PRINT(CMUX_DEBUG_INFO, "cmux_hex_dump addr: %pK, len = %d\n", buf, len);
    for (idx = 0; idx < len; idx++) {
        if ((idx % HEX_NUMBER_LEN) == 0) {
            CMUX_PRINT(CMUX_DEBUG_INFO, "0x%04x : ", idx);
        }
        CMUX_PRINT(CMUX_DEBUG_INFO, "%02x ", ((unsigned char)buf[idx]));
        if ((idx % HEX_NUMBER_LEN) == (HEX_NUMBER_LEN - 1)) {
            CMUX_PRINT(CMUX_DEBUG_INFO, "\n");
        }
    }
    CMUX_PRINT(CMUX_DEBUG_INFO, "\n");
}
#endif
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */