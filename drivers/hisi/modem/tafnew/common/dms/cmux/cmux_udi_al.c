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

#include <linux/spinlock.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include "cmux_udi_al.h"
#include "cmux_drv_ifc.h"
#include "cmux_tgt_osal.h"
#include "mdrv_udi.h"

#include "securec.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_IOT_CMUX == FEATURE_ON)
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

extern int bsp_udi_set_private(enum udi_device_id devId, void* pPrivate);
extern int bsp_udi_set_capability(enum udi_device_id devId, unsigned int u32Capability);
extern int bsp_udi_set_intf_table(enum udi_device_id devId, struct udi_drv_interface_table *pDrvInterface);
static cmux_ctx        g_cmux_ctx_arr[MAX_CMUX_VPORT_NO] = {
    {
        .udi_device_id = UDI_CMUX_AT_ID, /*lint !e65*/
        .DlcIndex      = CMUX_UDI_DLC_AT,
    },
    {
        .udi_device_id = UDI_CMUX_MDM_ID, /*lint !e65*/
        .DlcIndex      = CMUX_UDI_DLC_MDM,
    },
    {
        .udi_device_id = UDI_CMUX_EXT_ID, /*lint !e65*/
        .DlcIndex      = CMUX_UDI_DLC_EXT,
    },
    {
        .udi_device_id = UDI_CMUX_GPS_ID, /*lint !e65*/
        .DlcIndex      = CMUX_UDI_DLC_GPS,
    }
};

static struct udi_drv_interface_table g_cmux_udi = {
    .open_cb  = (udi_open_cb)cmux_udi_open,
    .close_cb = (udi_close_cb)cmux_udi_close,
    .ioctl_cb = (udi_ioctl_cb)cmux_udi_ioctl,
};

cmux_close_cb_t g_close_port_cb;

unsigned int      g_cmux_pluscount    = 0;
static const char g_cmux_switch_cmd[] = "+++";

/* 函数实现 */
extern int DMS_CMUX_CheckRawDataMode(unsigned char priIndex);

cmux_close_cb_t cmux_get_g_close_port_cb(void)
{
    return g_close_port_cb;
}
void cmux_set_g_close_port_cb(cmux_close_cb_t close_port_cb)
{
    g_close_port_cb = close_port_cb;
}

/* 功能描述: CMUX下模式切换第二阶段 */
void cmux_switch_stage_two(void *arg)
{
    cmux_ctx* ctx = (cmux_ctx*)arg;
    ctx->stage    = CMUX_STAGE_2;
}

/* 功能描述: CMUX下模式切换第三阶段 */
void cmux_switch_stage_three(void *arg)
{
    cmux_ctx* ctx = (cmux_ctx*)arg;

    ctx->stage = CMUX_STAGE_1;
    if (ctx->cbs.switch_mode_cb) {
        ctx->cbs.switch_mode_cb(ctx->DlcIndex);
    } else {
        CMUX_PRINT(CMUX_DEBUG_ERR, "switch_mode_cb is NULL!\r\n");
    }

    bsp_softtimer_add(&(ctx->cmux_state2_timer));
}

/* 功能描述: 进入 DETECT_STAGE_1 并起定时器switch_timer  --> switch_stage_two进入 DETECT_STAGE_2 */
void cmux_switch_stage_goback(cmux_ctx* ctx)
{
    /* 设置当前处于命令模式切换第一阶段，重新等1S空闲 */
    ctx->stage = CMUX_STAGE_1;
     bsp_softtimer_add(&(ctx->cmux_state2_timer));
}

/* 功能描述: CMUX下模式切换plus阶段 */
void cmux_switch_stage_plus(void *arg)
{
    cmux_ctx* ctx  = (cmux_ctx*)arg;
    g_cmux_pluscount = 0;
    cmux_switch_stage_goback(ctx);
}

int cmux_switch_mode_cmd_detect_stage_sec(cmux_ctx *ctx, const char *buff, int length)
{
    int result = FAIL;

    /* 判断是否+或者++ */
    if(((length == CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_ONE) || (length == CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_TWO)) &&
       (!memcmp((void*)g_cmux_switch_cmd, (void*)buff, (size_t)length)))
    {
        ctx->stage = CMUX_STAGE_PLUS;
        g_cmux_pluscount = (unsigned int)length;

        /* 起定时器，900ms之后，不再处于等待+号输入状态 */
        bsp_softtimer_add(&(ctx->cmux_stateplus_timer));
    }
    /* 判断是否+++ */
    else if((length == CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_THREE) &&
        (!memcmp((void*)g_cmux_switch_cmd, (void*)buff, (size_t)length)))
    {
        /* 起1S钟的超时SoftTimer，（在UART接收数据中断中，判断删除该SoftTimer，若该SoftTimer能触发超时中断处理，则表示后1S无数据下发）*/
        /* 设置当前处于命令模式切换第三阶段*/
        bsp_softtimer_add(&(ctx->cmux_state3_timer));

        /* 保持非UART透传模式下原代码逻辑，相当于在非透传模式下"+++"会被丢弃，只修改透传模式下的逻辑 */
        result = DMS_CMUX_CheckRawDataMode(ctx->DlcIndex);
    }
    else
    {
        cmux_switch_stage_goback(ctx);
    }

    return result;
}
void cmux_switch_mode_cmd_detect_stage_plus(cmux_ctx *ctx, const char *buff, int length)
{
    g_cmux_pluscount = g_cmux_pluscount + length;

    /* 判断是否+或者++ 且g_pluscount == 3 */
    if (((length == CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_ONE) || (length == CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_TWO)) &&
        (g_cmux_pluscount == CMUX_CMD_PLUS_COUNT_THREE) &&
        (!memcmp((void*)g_cmux_switch_cmd, (void*)buff, (size_t)length))) {
        g_cmux_pluscount = 0;

        /* 起1S钟的超时SoftTimer，（在UART接收数据中断中，判断删除该SoftTimer，若该SoftTimer能触发超时中断处理，则表示后1S无数据下发）*/
        /* 设置当前处于命令模式切换第三阶段 */
        bsp_softtimer_add(&(ctx->cmux_stateplus_timer));
    } else if (((length == CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_ONE) ||
                (length == CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_TWO)) && (g_cmux_pluscount == CMUX_CMD_PLUS_COUNT_TWO) &&
                (!memcmp((void*)g_cmux_switch_cmd, (void*)buff, (size_t)length))) {
        /* 判断是否第2个+号 */
        /* 刷新定时器，再等900ms */
         bsp_softtimer_add(&(ctx->cmux_state3_timer));
    } else {
        cmux_switch_stage_goback(ctx);
    }
}
/*****************************************************************************
* 函 数 名  : cmux_switch_mode_cmd_detect
* 功能描述  : CMUX下模式切换命令检测函数
* 输入参数  : ctx 上下文 pMemNode 数据节点指针
* 输出参数  : NA
* 返 回 值  : 成功/失败
*****************************************************************************/
int cmux_switch_mode_cmd_detect(cmux_ctx *ctx, const char *buff, int length)
{
    int result = FAIL;
    /* 入参合法性检查 */
    if (buff == NULL) {
        return FAIL;
    }

    if (ctx->stage == CMUX_STAGE_2) {
        result = cmux_switch_mode_cmd_detect_stage_sec(ctx, buff, length);
    } else if (ctx->stage == CMUX_STAGE_PLUS) {
        cmux_switch_mode_cmd_detect_stage_plus(ctx, buff, length);
    }

    return result;
}

/* 功能描述: CMUX下模式切换设置 */
void cmux_mode_switch_config(cmux_ctx* ctx, unsigned char* s32Arg)
{
    if (s32Arg == NULL) {
        bsp_softtimer_delete(&(ctx->cmux_state3_timer));
        bsp_softtimer_delete(&(ctx->cmux_state2_timer));
        bsp_softtimer_delete(&(ctx->cmux_stateplus_timer));
        ctx->stage              = NO_CMUX_STAGE;
        ctx->cbs.switch_mode_cb = NULL;
    } else {
        ctx->stage              = CMUX_STAGE_1;
        ctx->cbs.switch_mode_cb = (cmux_switch_mode_cb_t)s32Arg;
        bsp_softtimer_add(&(ctx->cmux_state2_timer));
    }
}

/*****************************************************************************
* 函 数 名  : cmux_timer_init
* 功能描述  : CMUX下的+++功能的timer初始化
* 输入参数  : ctx 上下文
* 输出参数  : NA
* 返 回 值  : PASS/FAIL
*****************************************************************************/
int cmux_timer_init(cmux_ctx *ctx)
{
    struct softtimer_list *statethree_timer = VOS_NULL_PTR;
    struct softtimer_list *statetwo_timer   = VOS_NULL_PTR;
    struct softtimer_list *stateplus_timer  = VOS_NULL_PTR;

    statethree_timer = &(ctx->cmux_state3_timer);
    statetwo_timer = &(ctx->cmux_state2_timer);
    stateplus_timer = &(ctx->cmux_stateplus_timer);

    statethree_timer->func = cmux_switch_stage_three;
    statethree_timer->para = (void *)ctx;
    statethree_timer->timeout = CMUX_SW_TIMER_LENGTH;
    statethree_timer->wake_type = SOFTTIMER_NOWAKE;

    statetwo_timer->func = cmux_switch_stage_two;
    statetwo_timer->para = (void *)ctx;
    statetwo_timer->timeout = CMUX_SW_TIMER_LENGTH;
    statetwo_timer->wake_type = SOFTTIMER_NOWAKE;

    stateplus_timer->func = cmux_switch_stage_plus;
    stateplus_timer->para = (void *)ctx;
    stateplus_timer->timeout = CMUX_SW_TIMER_LENGTH;
    stateplus_timer->wake_type = SOFTTIMER_NOWAKE;

    if(bsp_softtimer_create(statethree_timer) != OK)
    {
        CMUX_PRINT(CMUX_DEBUG_ERR, "statethree_timer create fail!\r\n");
    }

    if(bsp_softtimer_create(statetwo_timer) != OK)
    {
        CMUX_PRINT(CMUX_DEBUG_ERR, "statetwo_timer create fail!\r\n");
    }

    if(bsp_softtimer_create(stateplus_timer) != OK)
    {
        CMUX_PRINT(CMUX_DEBUG_ERR, "stateplus_timer create fail!\r\n");
    }

    return PASS;
}
/* Description:    获取cmux的ctx，根据index的值 */
STATIC cmux_ctx* cmux_get_ctx(unsigned int indexNum)
{
    int loop = 0;
    for (loop = 0; loop < MAX_CMUX_VPORT_NO; loop++) {
        if (indexNum == g_cmux_ctx_arr[loop].DlcIndex) {
            break;
        }
    }

    if (loop >= MAX_CMUX_VPORT_NO) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "CANNOT find ctx with index %d\n", indexNum);
        return NULL;
    }
    return &g_cmux_ctx_arr[loop];
}

/* Description: 初始化对应的cmux list */
int cmux_init_list(cmux_trans_info* trans_info)
{
    char*             pMemHead  = NULL;
    struct list_head* pListNode = NULL;
    unsigned int      idx       = 0;
    int               ret       = PASS;
    unsigned long     flags     = 0;

    if (trans_info == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "trans_info is NULL\n");
        ret = FAIL;
        return ret;
    }

    spin_lock_irqsave(&trans_info->list_lock, flags); /*lint !e18 !e830*/
    /* 初始化链表 */
    INIT_LIST_HEAD(&(trans_info->free_list));
    INIT_LIST_HEAD(&(trans_info->done_list));

    /* 初始化读信息 */
    pMemHead = cmux_alloc((int)(trans_info->node_num * sizeof(cmux_mem_node)));
    if (pMemHead) {
        trans_info->pmem_start  = (cmux_mem_node*)pMemHead;
        trans_info->pcur_pos    = NULL;
        trans_info->done_cnt    = 0;
        trans_info->total_bytes = 0;

        /* 初始化时,所有节点都在FreeList */
        for (idx = 0; idx < trans_info->node_num; idx++) {
            pListNode = &(trans_info->pmem_start[idx].list_node);
            list_add_tail(pListNode, &(trans_info->free_list));
            trans_info->pmem_start[idx].state      = CMUX_MEM_IN_FREELIST;
            trans_info->pmem_start[idx].valid_size = 0;
            trans_info->pmem_start[idx].mem_buf    = NULL;
        }
    } else {
        ret = FAIL;
    }
    spin_unlock_irqrestore(&trans_info->list_lock, flags);

    return ret;
}

/* Description:   初始化对应的cmux buffer */
int cmux_buf_init(cmux_ctx* ctx)
{
    int ret = PASS;

    if (ctx == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_ctx is NULL\n");
        ret = FAIL;
        return ret;
    }

    /*lint -e506*/
    /* 初始化读写初始值 */
    (void)memset_s((void*)&(ctx->read_info), sizeof(cmux_trans_info), 0, sizeof(cmux_trans_info));
    (void)memset_s((void*)&(ctx->write_info), sizeof(cmux_trans_info), 0, sizeof(cmux_trans_info));
    /*lint +e506*/
    /*lint -e133 -e43*/
    spin_lock_init(&(ctx->read_info.list_lock));
    spin_lock_init(&(ctx->write_info.list_lock));
    /*lint +e133 +e43*/
    ctx->read_info.node_num   = CMUX_READ_BUF_NUM;
    ctx->read_info.node_size  = CMUX_READ_BUF_SIZE;
    ctx->write_info.node_num  = CMUX_WRITE_BUF_NUM;
    ctx->write_info.node_size = 0;
    if (!cmux_init_list(&(ctx->read_info)) || !cmux_init_list(&(ctx->write_info))) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_init_list error\n");
        ret = FAIL;
    }

    return ret;
}
/* Description:   打开cmux udi虚拟设备 */
signed int cmux_udi_open(struct udi_open_param* param, int handle)
{ /*lint !e101*/
    cmux_ctx *ctx = NULL;

    if (param == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "param is NULL\n");
        return -1;
    }
#if defined(LLT_OS_VER)
    ctx = cmux_get_ctx((unsigned int)param->pPrivate);
#else
    ctx = cmux_get_ctx((unsigned int)(VOS_UINT_PTR)param->private);
#endif
    if (ctx != NULL) {
        ctx->open = TRUE;

        (void)bsp_udi_set_private(param->devid, (void*)ctx);
    } else {
#if defined(LLT_OS_VER)
        CMUX_PRINT(CMUX_DEBUG_ERR, "CANNOT find ctx of %d\n", (unsigned int)param->pPrivate);
#else
        CMUX_PRINT(CMUX_DEBUG_ERR, "CANNOT find ctx of %d\n", (unsigned int)(VOS_UINT_PTR)param->private);
#endif
        return -1;
    }
    return 0;
}
/* Description: 关闭cmux udi虚拟设备 */
signed int cmux_udi_close(void* DevCtx)
{
    cmux_ctx* pCmuxDevCtx = (cmux_ctx*)DevCtx;
    if (pCmuxDevCtx == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "DevCtx is NULL!\n");
        return -1;
    }
    pCmuxDevCtx->open = FALSE;

    return 0;
}
/* Description:   cmux异步写接口 */
signed int cmux_write_async(cmux_ctx* ctx, at_cmux_wr_async_info* pParam)
{
    unsigned char       *buff = NULL;
    unsigned char       *temp_buff = NULL;
    struct cmuxlib      *cmux = NULL;
    unsigned int         len;
    unsigned int         temp_len;
    unsigned int         send_len;
    unsigned int         total_len;
    unsigned char        dlc;
    int                  ret = 0;

    cmux = cmux_get_g_cmux();

    if ((ctx == NULL) || (pParam == NULL)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "InputParam err with ctx = %d pParam = %d\n", ctx, pParam);
        return -1;
    }

    if (cmux != NULL) {
        buff = cmux_get_skb(pParam->pVirAddr);
        len  = pParam->u32Size;
        dlc  = ctx->DlcIndex;

        if (buff == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_get_skb return NULL!\n");
            return -1;
        }

        CMUX_PRINT(CMUX_DEBUG_INFO, "cmux_hex_dump!\r\n");
        cmux_hex_dump(CMUX_DEBUG_INFO, buff, len);

        temp_len  = len;
        temp_buff = buff;
        total_len = 0;

        while (temp_len != 0) {
            send_len = (temp_len > MAX_SERIAL_PKT_LEN) ? MAX_SERIAL_PKT_LEN : temp_len;

            ret = cmux_send_dlc_data(cmux, temp_buff, send_len, dlc, CMUX_FRAGMENT_DATA | CMUX_FLUSH_DATA);
            if (ret != PASS) {
                CMUX_PRINT(CMUX_DEBUG_WARN, "cmux_send_dlc_data failed with len = %d\n", send_len);
                return -1;
            }
            temp_len -= send_len;
            total_len += send_len;
            temp_buff = buff + total_len;
        }

        if (ctx->cbs.free_cb) {
            ctx->cbs.free_cb((char*)pParam->pVirAddr);
        }
        return 0;
    } else {
        CMUX_PRINT(CMUX_DEBUG_INFO, "CMUX function is not on!\n");
        return -1;
    }
}
/******************************************************************************
Function:       cmux_check_write_signal_para_legality
Description:    cmux modem检查参数合法性
Input:          cmux_ctx *ctx, cmux_modem_msc_stru *pParam
Output:         NONE
Return:         signed int
Others:         NONE
******************************************************************************/
extern unsigned int cmux_hsuart_write_msc_cmd(cmux_modem_msc_stru *pstDceMsc);
int cmux_check_write_signal_para_legality(cmux_ctx *ctx, cmux_modem_msc_stru *pParam)
{
    cmux_modem_msc_stru *msc_stru = NULL;
    unsigned char        dlc;
    struct cmuxlib      *cm = cmux_get_g_cmux();
    int                  ret = PASS;

    if (cm == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "CMUX function is not on!\n");
        return FAIL;
    }
    if ((ctx == NULL) || (pParam == NULL)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "InputParam err with ctx = %d pParam = %d\n", ctx, pParam);
        return FAIL;
    }
    msc_stru = (cmux_modem_msc_stru*)pParam; /*lint !e565*/
    dlc      = ctx->DlcIndex;

    /* 设置硬件管脚 */
    ret = cmux_hsuart_write_msc_cmd(msc_stru);
    if (ret == FAIL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "HSUART Write Msc Cmd failed!\n");
    }

    if (cm->dlc_db[dlc] == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "DLC is not established!\n");
        return FAIL;
    }

    return PASS;
}
/******************************************************************************
Function:       cmux_dealwith_dcd_signal_change
Description:    cmux modem dcd管脚信号改变处理
Input:          cmux_ctx *ctx, cmux_modem_msc_stru *pParam
Output:         NONE
Return:         signed int
Others:         NONE
******************************************************************************/
void cmux_dealwith_dcd_signal_change(cmux_modem_msc_stru *msc_stru, struct cmuxlib *cm, unsigned char dlc, int *is_send_cmd)
{
    CMUX_PRINT(CMUX_DEBUG_INFO, "DCD change is %d!\n", msc_stru->OP_Dcd);
    if (msc_stru->OP_Dcd == 1) {
        if (cm->dlc_db[dlc]->msc.dv != msc_stru->ucDcd) {
            if (msc_stru->ucDcd == 1) {
                cm->dlc_db[dlc]->msc.dv = 1;
            } else {
                cm->dlc_db[dlc]->msc.dv = 0;
            }
            *is_send_cmd = TRUE;
        }
    }
}
/******************************************************************************
Function:       cmux_dealwith_dcd_signal_change
Description:    cmux modem dsr管脚信号改变处理
Input:          cmux_ctx *ctx, cmux_modem_msc_stru *pParam
Output:         NONE
Return:         signed int
Others:         NONE
******************************************************************************/
void cmux_dealwith_dsr_signal_change(cmux_modem_msc_stru *msc_stru, struct cmuxlib *cm, unsigned char dlc, int *is_send_cmd)
{
    CMUX_PRINT(CMUX_DEBUG_INFO, "DSR change is %d!\n", msc_stru->OP_Dsr);
    if (msc_stru->OP_Dsr == 1) {
        if (cm->dlc_db[dlc]->msc.rtc != msc_stru->ucDsr) {
            if (msc_stru->ucDsr == 1) {
                cm->dlc_db[dlc]->msc.rtc = 1;
            } else {
                cm->dlc_db[dlc]->msc.rtc = 0;
            }
            *is_send_cmd = TRUE;
        }
    }
}
/******************************************************************************
Function:       cmux_dealwith_dcd_signal_change
Description:    cmux modem ring管脚信号改变处理
Input:          cmux_ctx *ctx, cmux_modem_msc_stru *pParam
Output:         NONE
Return:         signed int
Others:         NONE
******************************************************************************/
void cmux_dealwith_ring_signal_change(cmux_modem_msc_stru *msc_stru, struct cmuxlib *cm, unsigned char dlc, int *is_send_cmd)
{
    CMUX_PRINT(CMUX_DEBUG_INFO, "RI change is %d!\n", msc_stru->OP_Ri);
    if (msc_stru->OP_Ri == 1) {
        if (msc_stru->ucRi == 1) {
            cm->dlc_db[dlc]->msc.ic = 1;
            *is_send_cmd            = TRUE;
        } else {
            /* RING变低电平时，无需上报 */
            cm->dlc_db[dlc]->msc.ic = 0;
        }
    }
}
/******************************************************************************
Function:       cmux_dealwith_dcd_signal_change
Description:    cmux modem cts管脚信号改变处理
Input:          cmux_ctx *ctx, cmux_modem_msc_stru *pParam
Output:         NONE
Return:         signed int
Others:         NONE
******************************************************************************/
void cmux_dealwith_cts_signal_change(cmux_modem_msc_stru *msc_stru, struct cmuxlib *cm)
{
    int ret;

    CMUX_PRINT(CMUX_DEBUG_INFO,"CTS change is %d!\n", msc_stru->OP_Cts);
    if (msc_stru->OP_Cts == 1) {
        if ( cm->flow_ctrl != msc_stru->ucCts ) {
            if (msc_stru->ucCts == 1) {
                CMUX_PRINT(CMUX_DEBUG_INFO,"going to send FCON\n");
                ret = cmux_send_fcon_fcoff(cm, 0, FCON_CMD);
                cm->flow_ctrl = 1;
            } else {
                CMUX_PRINT(CMUX_DEBUG_INFO, "going to send FCOFF\n");
                ret           = cmux_send_fcon_fcoff(cm, 0, FCOFF_CMD);
                cm->flow_ctrl = 0;
            }
        }
    }
}
/******************************************************************************
Function:       cmux_modem_write_signal
Description:    cmux modem状态异步写接口
Input:          cmux_ctx *ctx, cmux_modem_msc_stru *pParam
Output:         NONE
Return:         signed int
Others:         NONE
******************************************************************************/
signed int cmux_modem_write_signal(cmux_ctx *ctx, cmux_modem_msc_stru *pParam)
{
    cmux_modem_msc_stru *msc_stru = NULL;
    unsigned char        dlc;
    struct cmuxlib      *cm = cmux_get_g_cmux();
    int                  ret = PASS;
    int                  is_send_cmd = FALSE;

    ret = cmux_check_write_signal_para_legality(ctx, pParam);
    if (ret == FAIL) {
        return -1;
    }

    dlc = ctx->DlcIndex;

    msc_stru = (cmux_modem_msc_stru *)pParam;/*lint !e565*/
    /* 如果是RING管脚变化，DLC置为DLC0 */
    if (msc_stru->OP_Ri == 1) {
        cm->dlc_db[dlc]->msc.dlci = 0;
    } else {
        cm->dlc_db[dlc]->msc.dlci = dlc;
    }

    /* 当DCD信号有改变，为高电平时，msc的dv位置1，反之置0 */
    cmux_dealwith_dcd_signal_change(msc_stru, cm, dlc, &is_send_cmd);

    /* 当DSR信号有改变，为高电平时，msc的rtc位置1，反之置0 */
    cmux_dealwith_dsr_signal_change(msc_stru, cm, dlc, &is_send_cmd);

    /* 当RING信号有改变，为高电平时，msc的ic位置1，反之置0 */
    cmux_dealwith_ring_signal_change(msc_stru, cm, dlc, &is_send_cmd);

    /* 当CTS信号有改变，需要发送FCON/FCOFF命令*/
    cmux_dealwith_cts_signal_change(msc_stru, cm);

    /* 发送msc命令上报管脚状态 */
    if (is_send_cmd == TRUE) {
        ret = cmux_send_msc(cm, dlc, &(cm->dlc_db[dlc]->msc));
        if (ret == FAIL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "MSC is sent failed!\n");
        }
    }

    return 0;
}
/* Description:   将使用过的memnode重新放入freelist里面 */
void cmux_add_mem_to_freelist(cmux_trans_info* trans_info, cmux_mem_node* pMemNode)
{
    unsigned long flags;

    spin_lock_irqsave(&trans_info->list_lock, flags);

    list_add_tail(&pMemNode->list_node, &(trans_info->free_list));
    /* 去初始化pMemNode成员 */
    pMemNode->state      = CMUX_MEM_IN_FREELIST;
    pMemNode->valid_size = 0;
    pMemNode->mem_buf    = NULL;

    spin_unlock_irqrestore(&trans_info->list_lock, flags);
}
/* Description:   从freelist里面找到第一个可用的memnode */
cmux_mem_node* cmux_get_mem_from_freelist(cmux_trans_info* trans_info)
{
    struct list_head *pListNode = NULL;
    cmux_mem_node    *pMemNode  = NULL;
    unsigned long     flags     = 0;

    spin_lock_irqsave(&trans_info->list_lock, flags);

    if (list_empty(&(trans_info->free_list))) {
        pMemNode = NULL;
    } else {
        pListNode = trans_info->free_list.next;
        pMemNode  = list_entry(pListNode, cmux_mem_node, list_node);

        list_del_init(pListNode);
        pMemNode->state = CMUX_MEM_NOT_IN_LIST;
    }

    spin_unlock_irqrestore(&trans_info->list_lock, flags);

    return pMemNode;
}

/* Description:   将CMUX的数据保存在donelist里面等待AT获取 */
cmux_mem_node* cmux_get_read_node(cmux_ctx* ctx)
{
    cmux_mem_node* pMemNode;

    pMemNode = cmux_get_mem_from_freelist(&(ctx->read_info));

    if (pMemNode) {
        if (!pMemNode->mem_buf) {
            pMemNode->mem_buf = cmux_alloc_skb(ctx->read_info.node_size);
            if (pMemNode->mem_buf == NULL) {
                cmux_add_mem_to_freelist(&(ctx->read_info), pMemNode);
                pMemNode = NULL;
                ctx->stat.alloc_mem_fail++;
            }
        }
        ctx->read_info.pcur_pos = pMemNode;
    }

    return pMemNode;
}

/* Description:   将CMUX的数据保存在donelist里面等待AT获取 */
void cmux_add_mem_to_donelist(cmux_trans_info* trans_info, cmux_mem_node* pMemNode)
{
    unsigned long flags = 0;
    /* 入参合法性检查 */
    spin_lock_irqsave(&trans_info->list_lock, flags);

    list_add_tail(&pMemNode->list_node, &(trans_info->done_list));
    trans_info->done_cnt++;
    pMemNode->state = CMUX_MEM_IN_DONELIST;

    spin_unlock_irqrestore(&trans_info->list_lock, flags);
}

/* Description:   将CMUX的数据保存在donelist里面等待AT获取 */
int cmux_data_rx_complete(cmux_ctx* ctx, const char* buff, int length)
{
    cmux_mem_node  *pMemNode = NULL;

    if (strlen(g_cmux_switch_cmd) >= (size_t)length) {
        if (cmux_switch_mode_cmd_detect(ctx, buff, length) != FAIL) {
            return FAIL;
        }
    } else {
        cmux_switch_stage_goback(ctx);
    }

    pMemNode = cmux_get_read_node(ctx);
    if (pMemNode == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "CANNOT get buf from freelist\n");
        return FAIL;
    }

    cmux_put_skb(pMemNode->mem_buf, buff, length);
    pMemNode->valid_size = length;
    cmux_add_mem_to_donelist(&ctx->read_info, pMemNode);
    return PASS;
}

/* Description:   cmux向AT/PPP/GPS发送数据的接口 */
void cmux_callback_func(unsigned int cbtype, const void* ptr, struct cmux_timer* ct)
{
    unsigned int  DlcIndex;
    struct frame* rxfr   = NULL;
    char*         pdata  = NULL;
    int           length = 0;
    cmux_ctx*     ctx    = NULL;

    CMUX_PRINT(CMUX_DEBUG_INFO, "rcved type %d \n", cbtype);
    if (cbtype == (TE_SENT_CB_TYPE | RX_COMPLETE_CB_TYPE)) {
        rxfr = (struct frame*)ptr;
        if (rxfr == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "FRAME PTR is NULL\n");
            return;
        }
        pdata = (char*)rxfr->uih_data;
        if (pdata == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "pdata is NULL\n");
            return;
        }

        length   = (int)rxfr->length;
        DlcIndex = rxfr->dlc;
        ctx      = cmux_get_ctx(DlcIndex);
        if (ctx == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_ctx is NULL\n");
            return;
        }

        (void)bsp_softtimer_delete(&(ctx->cmux_state3_timer));
        (void)bsp_softtimer_delete(&(ctx->cmux_state2_timer));
        (void)bsp_softtimer_delete(&(ctx->cmux_stateplus_timer));

        if (cmux_data_rx_complete(ctx, pdata, length) == FAIL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_data_rx_complete failed\n");
            return;
        }

        if (ctx->cbs.read_cb) {
            CMUX_PRINT(CMUX_DEBUG_INFO, "read_cb DlcIndex %d\n", ctx->DlcIndex);
            ctx->cbs.read_cb(ctx->DlcIndex);
        }
        return;
    } else if (cbtype == DLC_ESTABLISH_COMPLETE) {
        DlcIndex = (unsigned int)(uintptr_t)ptr;
        ctx      = cmux_get_ctx(DlcIndex);

        if (ctx == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "DLC_ESTABLISH_COMPLETE ctx NULL %d\n", DlcIndex);
            return;
        }

        if (ctx->cbs.msc_mode_read_cb) {
            CMUX_PRINT(CMUX_DEBUG_INFO, "msc_mode_read_cb DlcIndex %d\n", ctx->DlcIndex);
            ctx->cbs.msc_mode_read_cb(ctx->DlcIndex);
        }
        return;
    } else if (cbtype == AT_STATE_CHANGE_CB_TYPE) {
        struct snc_cmd* snc;
        unsigned char   enable;

        snc      = (struct snc_cmd*)ptr;
        DlcIndex = snc->dlci;
        enable   = snc->s1;

        ctx = cmux_get_ctx(DlcIndex);
        if (ctx == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "AT_STATE_CHANGE ctx NULL %d\n", DlcIndex);
            return;
        }

        if (ctx->cbs.snc_mode_cb) {
            CMUX_PRINT(CMUX_DEBUG_INFO, "snc_mode_cb DlcIndex=%d, enable=%d\n", ctx->DlcIndex, enable);
            ctx->cbs.snc_mode_cb(ctx->DlcIndex, enable);
        }
        return;
    } else {
        CMUX_PRINT(CMUX_DEBUG_INFO, "cbtype 0x%x non_support, return\n", cbtype);
        return;
    }
}

/* Description:    cmux向AT侧发送msc状态 */
void cmux_send_msc_status(unsigned char Index, cmux_modem_msc_stru* pModemMsc)
{
    cmux_ctx* ctx = NULL;

    if (pModemMsc == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "pModemMsc is NULL\n");
        return;
    }

    if (Index == CMUX_UDI_DLC_CTL) {
        /* 硬件DTR消息使用DLC 1来生成context承载 */
        ctx = cmux_get_ctx(CMUX_UDI_DLC_AT);
    } else {
        ctx = cmux_get_ctx(Index);
    }

    if (ctx == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_ctx is NULL\n");
        return;
    }

    /* 向AT侧上层发送设置管脚消息 */
    if (ctx->cbs.msc_read_cb) {
        ctx->cbs.msc_read_cb(Index, (unsigned char*)pModemMsc);
    }
}
/* Description:   返回cmux的donelist里面的buffer */
cmux_mem_node* cmux_get_mem_from_donelist(cmux_trans_info* trans_info)
{
    struct list_head *pListNode = NULL;
    cmux_mem_node    *pMemNode  = NULL;
    unsigned long     flags;

    spin_lock_irqsave(&trans_info->list_lock, flags);

    if (list_empty(&(trans_info->done_list))) {
        pMemNode = NULL;
    } else {
        pListNode = trans_info->done_list.next;
        pMemNode  = list_entry(pListNode, cmux_mem_node, list_node);
        list_del_init(pListNode);
        trans_info->done_cnt--;
        pMemNode->state = CMUX_MEM_NOT_IN_LIST;
    }

    spin_unlock_irqrestore(&trans_info->list_lock, flags);

    return pMemNode;
}
/* Description:   返回cmux的donelist里面的buffer */
int cmux_get_read_buffer(cmux_ctx* ctx, at_cmux_wr_async_info* pWRInfo)
{
    cmux_mem_node *pMemNode = NULL;

    /* unsigned long flags = 0; */
    if (ctx == NULL || pWRInfo == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_ctx:0x%pK   pWRInfo:0x%pK.\n", ctx, pWRInfo);
        return -1;
    }

    /* 获取完成的 buffer节点 */
    pMemNode = cmux_get_mem_from_donelist(&(ctx->read_info));

    if (pMemNode) {
        /* 数据所指向的数据 */
        pWRInfo->pVirAddr = (char*)pMemNode->mem_buf;
        /* 数据的大小 */
        pWRInfo->u32Size = (unsigned int)pMemNode->valid_size;
        /* 当前信息头指针 */
        pWRInfo->pDrvPriv = (void*)pMemNode->mem_buf;

        cmux_add_mem_to_freelist(&(ctx->read_info), pMemNode);

        pMemNode->mem_buf = cmux_alloc_skb(ctx->read_info.node_size);
    } else {
        CMUX_PRINT(CMUX_DEBUG_ERR, "no done mem node in list\n");
        return -1;
    }

    return 0;
}
/* Description:   释放AT层使用完毕的内存 */
int cmux_return_read_buffer(cmux_ctx* ctx, at_cmux_wr_async_info* pWRInfo)
{
    if ((ctx == NULL) || (pWRInfo == NULL)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_ctx:0x%pK   pWRInfo:0x%pK.\n", ctx, pWRInfo);
        return -1;
    }

    cmux_free_skb((unsigned char*)pWRInfo->pVirAddr);

    return 0;
}

signed int cmux_udi_ioctl_proc(unsigned char *pPrivate, unsigned int u32Cmd, unsigned char *pParam)
{
    cmux_ctx*  ctx = (cmux_ctx*)pPrivate;
    signed int ret = 0;

    if (ctx->DlcIndex > MAX_DLC) {
        return -1;
    }

    switch (u32Cmd) {
        /* 下行异步写接口 */
        case UDI_CMUX_IOCTL_WRITE_ASYNC: {
            ret = cmux_write_async(ctx, (at_cmux_wr_async_info*)pParam);
        } break;
        /* NAS注册下行数据buf 释放函数 */
        case UDI_CMUX_IOCTL_SET_FREE_CB: {
            ctx->cbs.free_cb = (cmux_free_cb_t)pParam;
        } break;
        /* 收到数据后调用此注册接口通知上层接收 */
        case UDI_CMUX_IOCTL_SET_READ_CB: {
            ctx->cbs.read_cb = (cmux_read_cb_t)pParam;
        } break;
        /* 上层调用此接口获得上行buf 地址，上层在我们的read回调函数中来实现的 */
        case UDI_CMUX_IOCTL_GET_RD_BUFF: {
            ret = cmux_get_read_buffer(ctx, (at_cmux_wr_async_info*)pParam);
        } break;
        /* 上层调用此接口释放上行buf */
        case UDI_CMUX_IOCTL_RETURN_BUFF: {
            ret = cmux_return_read_buffer(ctx, (at_cmux_wr_async_info*)pParam);
        } break;
        /* 上层调用此接口关闭多路复用模式 */
        case UDI_CMUX_IOCTL_CLOSE_PORT_CB: {
            cmux_set_g_close_port_cb((cmux_close_cb_t)pParam);
        } break;
        /* 写modem管脚信号，参数为pMODEM_MSC_STRU */
        case UDI_CMUX_IOCTL_MSC_WRITE_CMD: {
            ret = cmux_modem_write_signal(ctx, (cmux_modem_msc_stru*)(pParam));
        } break;
        /* 注册管脚变化通知回调参数为pMODEM_MSC_STRU 指向本地全局 */
        case UDI_CMUX_IOCTL_SET_MSC_READ_CB: {
            ctx->cbs.msc_read_cb = (cmux_msc_read_cb_t)pParam;
        } break;
        /* 注册管脚模式查询回调 */
        case UDI_CMUX_IOCTL_MSC_MODE_READ_CB: {
            ctx->cbs.msc_mode_read_cb = (cmux_msc_mode_read_cb_t)pParam;
        } break;
        /* +++切换命令模式回调，设置NULL关闭该功能 */
        case UDI_CMUX_IOCTL_SWITCH_MODE_CB: {
            cmux_mode_switch_config(ctx, pParam);
        } break;
        case UDI_CMUX_IOCTL_SNC_CB: {
            ctx->cbs.snc_mode_cb = (cmux_snc_mode_cb_t)pParam;
        } break;
        default:
            CMUX_PRINT(CMUX_DEBUG_ERR, "unknow s32Cmd: 0x%x.\n", u32Cmd);
            ret = -1;
            break;
    }

    return ret;
}

/******************************************************************************
Function:       cmux_udi_ioctl
Description:   处理cmux udi虚拟设备ioctl命令的函数。
Input:          void *DevCtx
Output:        NONE
Return:        signed int
Others:        NONE
******************************************************************************/
signed int cmux_udi_ioctl(void *pPrivate, unsigned int u32Cmd, void*pParam)
{
    cmux_ctx* ctx = (cmux_ctx*)pPrivate;
    signed int ret = 0;
    if (ctx == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_ctx is NULL!\n");
        ret = -1;
        return ret;
    }

    if (!ctx->open) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_ctx is not open!\n");
        ret = -1;
        return ret;
    }

    ret = cmux_udi_ioctl_proc(pPrivate, u32Cmd, pParam);

    return ret;
}

/******************************************************************************
Function:       cmux_udi_init
Description:    初始化cmux udi虚拟设备的函数。
Input:          void
Output:         NONE
Return:         Pass :成功
                Fail :失败
Others:         NONE
******************************************************************************/
int cmux_udi_init(void)
{
    int       loop = 0;
    cmux_ctx* ctx  = NULL;

    for (loop = 0; loop < MAX_CMUX_VPORT_NO; loop++) {
        ctx = &g_cmux_ctx_arr[loop];

        ctx->init = FALSE;

        /* UDI层的初始化 */
        (void)bsp_udi_set_capability(ctx->udi_device_id, 0);
        (void)bsp_udi_set_intf_table(ctx->udi_device_id, &g_cmux_udi);

        if (cmux_buf_init(ctx) != PASS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_buf_init is error!\n");
            return FAIL;
        }

        if (cmux_timer_init(ctx) != PASS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_timer_init is error!\n");
            return FAIL;
        }

        ctx->init = TRUE;
    }
    return PASS;
}
#if (!defined(LLT_OS_VER))
module_init(cmux_udi_init);
#endif
#endif/* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */