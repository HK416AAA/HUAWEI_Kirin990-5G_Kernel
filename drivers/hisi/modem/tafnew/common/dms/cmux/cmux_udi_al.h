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

#ifndef CMUX_UDI_AL_H
#define CMUX_UDI_AL_H

#ifdef WIN32
#include "hi_list.h"
#else
#include <linux/list.h>
#include "mdrv_hsuart.h"
#endif
#include "bsp_softtimer.h"
#include "mdrv_udi.h"
#include "cmux_cmd.h"
#include "cmux_drv_ifc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if (FEATURE_IOT_CMUX == FEATURE_ON)
#define MAX_CMUX_VPORT_NO    4
/* buffer define */
#define CMUX_READ_BUF_SIZE   1536
#define CMUX_READ_BUF_NUM    16
#define CMUX_WRITE_BUF_NUM   64
#define CMUX_SW_TIMER_LENGTH 900

#define CMUX_CMD_PLUS_COUNT_TWO   2
#define CMUX_CMD_PLUS_COUNT_THREE 3
#define CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_ONE   1
#define CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_TWO   2
#define CMUX_SWITCH_CMD_LENGTH_FOR_STAGE_THREE 3


typedef void (*cmux_free_cb_t)(char* pBuff);
typedef void (*cmux_read_cb_t)(unsigned char ucIndex);
typedef void (*cmux_msc_read_cb_t)(unsigned char ucIndex, void* pModemMsc);
typedef void (*cmux_msc_mode_read_cb_t)(unsigned char ucIndex);
typedef void (*cmux_switch_mode_cb_t)(unsigned char ucIndex);
typedef void (*cmux_close_cb_t)(void);
typedef void (*cmux_snc_mode_cb_t)(unsigned char ucIndex, unsigned char enable);


typedef struct tagCMUX_UDI_CALLBACKS_S {
    /* General Handlers */
    cmux_free_cb_t          free_cb;
    cmux_read_cb_t          read_cb;
    cmux_msc_read_cb_t      msc_read_cb;
    cmux_msc_mode_read_cb_t msc_mode_read_cb;
    cmux_switch_mode_cb_t   switch_mode_cb;
    cmux_snc_mode_cb_t      snc_mode_cb;
} cmux_udi_cbs;

typedef enum tagCMUX_MEM_STATE {
    CMUX_MEM_NOT_IN_LIST = 0,
    CMUX_MEM_IN_FREELIST = 1,
    CMUX_MEM_IN_DONELIST = 2
} cmux_mem_state;

/* CMUX 内存信息 */
typedef struct tagCMUX_MEM_NODE {
    struct list_head list_node;
    cmux_mem_state   state;
    unsigned int     valid_size;
    unsigned char*   mem_buf;
} cmux_mem_node;

/* CMUX数据队列结构体 */
typedef struct tagCMUX_TRANS_INFO_S {
    struct spinlock       list_lock;
    struct list_head      free_list;
    struct list_head      done_list;
    cmux_mem_node*        pmem_start; /* MemNode数组首地址 */
    cmux_mem_node*        pmem_end;
    cmux_mem_node*        pcur_pos; /* 当前使用节点 */
    unsigned int          node_size;
    unsigned int          node_num;
    volatile unsigned int done_cnt;
    /* statistics section */
    unsigned int total_bytes; /* 总计数 */
} cmux_trans_info;

/* cmux stat info */
typedef struct tagCMUX_STAT {
    unsigned int write_not_free_cnt;
    unsigned int lost_data_cnt;
    unsigned int syn_write_cnt;
    unsigned int burst_dma_done;
    unsigned int single_dma_done;
    unsigned int single_time_out;
    unsigned int alloc_mem_fail;
    unsigned int sleep_delay_cnt;
    unsigned int tx_node_empty;
    unsigned int tx_empty_dma;
    unsigned int tx_empty_int;
} cmux_stat;

typedef enum {
    CMUX_UDI_DLC_CTL = 0,
    CMUX_UDI_DLC_AT  = 1,
    CMUX_UDI_DLC_MDM = 2,
    CMUX_UDI_DLC_EXT = 3,
    CMUX_UDI_DLC_GPS = 4,
    CMUX_UDI_DLC_MAX = 0xff,
} CMUX_UDI_DLC_NUM;
typedef enum {
    NO_CMUX_STAGE,
    CMUX_STAGE_1,
    CMUX_STAGE_2,
    CMUX_STAGE_PLUS,
    CMUX_STAGE_3,
    CMUX_STAGE_MAX,
} cmux_stage;
typedef struct tagCMUX_CTX_S {
    enum udi_device_id  udi_device_id;
    int                 init;
    int                 open;
    volatile cmux_stage stage;
    cmux_trans_info     read_info;  /* 读信息 */
    cmux_trans_info     write_info; /* 写信息 */
    cmux_udi_cbs        cbs;
    unsigned char       DlcIndex;
    unsigned char reserved[3];
    cmux_stat           stat;

    struct softtimer_list cmux_state3_timer;
    struct softtimer_list cmux_state2_timer;
    struct softtimer_list cmux_stateplus_timer;
} cmux_ctx;

/* 异步数据收发结构 */
typedef struct tagAT_CMUX_WR_ASYNC_INFO {
    char*        pVirAddr;
    char*        pPhyAddr;
    unsigned int u32Size;
    unsigned char reserved[4];
    void*        pDrvPriv;
} at_cmux_wr_async_info;

int cmux_udi_init(void);
signed int cmux_udi_ioctl(void* pPrivate, unsigned int u32Cmd, void* pParam);
signed int cmux_udi_open(struct udi_open_param* param, int handle);
signed int cmux_udi_close(void* DevCtx);
void cmux_callback_func(unsigned int cbtype, const void* ptr, struct cmux_timer* ct);
void cmux_send_msc_status(unsigned char Index, cmux_modem_msc_stru* pModemMsc);
cmux_close_cb_t cmux_get_g_close_port_cb(void);
void cmux_set_g_close_port_cb(cmux_close_cb_t close_port_cb);
#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* CMUX_UDI_AL_H */
