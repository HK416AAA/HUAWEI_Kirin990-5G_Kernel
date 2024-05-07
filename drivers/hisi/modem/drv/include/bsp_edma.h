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
#ifndef _BSP_EDMA_H_
#define _BSP_EDMA_H_

#include "soc_interrupts.h"
#include "osl_types.h"
#include "bsp_memmap.h"
#include "drv_edma_enum.h"
#include "mdrv_edma.h"
#include "bsp_print.h"
/* p531 联调 使用 gu内部逻辑 edma 挂vic */
#ifdef CONFIG_MODULE_VIC
#include "bsp_vic.h"
#endif

#include <linux/types.h>
#include "hi_edma.h"

#ifdef __cplusplus /* __cplusplus */
extern "C" {
#endif /* __cplusplus */

typedef dma_addr_t edma_addr_t;

typedef dma_addr_t edma_addr_t;

#define BALONG_DMA_INT_DONE 1
#define BALONG_DMA_INT_LLT_DONE 2
#define BALONG_DMA_INT_CONFIG_ERR 4
#define BALONG_DMA_INT_TRANSFER_ERR 8
#define BALONG_DMA_INT_READ_ERR 16

#define BALONG_DMA_P2M 1
#define BALONG_DMA_M2P 2
#define BALONG_DMA_M2M 3

#define EDMA_CHN_FREE 1
#define EDMA_CHN_BUSY 0

#define EDMA_TRANS_WIDTH_8 0x0
#define EDMA_TRANS_WIDTH_16 0x1
#define EDMA_TRANS_WIDTH_32 0x2
#define EDMA_TRANS_WIDTH_64 0x3

#define EDMA_BUR_LEN_1 0x0
#define EDMA_BUR_LEN_2 0x1
#define EDMA_BUR_LEN_3 0x2
#define EDMA_BUR_LEN_4 0x3
#define EDMA_BUR_LEN_5 0x4
#define EDMA_BUR_LEN_6 0x5
#define EDMA_BUR_LEN_7 0x6
#define EDMA_BUR_LEN_8 0x7
#define EDMA_BUR_LEN_9 0x8
#define EDMA_BUR_LEN_10 0x9
#define EDMA_BUR_LEN_11 0xa
#define EDMA_BUR_LEN_12 0xb
#define EDMA_BUR_LEN_13 0xc
#define EDMA_BUR_LEN_14 0xd
#define EDMA_BUR_LEN_15 0xe
#define EDMA_BUR_LEN_16 0xf


/* EDMAC_BASIC_CONFIG?BALONG_DMA_SET_LLI?BALONG_DMA_SET_CONFIG ?? */
/* config------Bit 31 */
#define EDMAC_TRANSFER_CONFIG_SOUR_INC (0X80000000)
/* Bit 30 */
#define EDMAC_TRANSFER_CONFIG_DEST_INC (0X40000000)
#define EDMAC_TRANSFER_CONFIG_BOTH_INC (0XC0000000)

/* Bit 27-24 */
#define EDMAC_TRANSFER_CONFIG_SOUR_BURST_LENGTH_MASK (0xF000000)
#define EDMAC_TRANSFER_CONFIG_SOUR_BURST_LENGTH(_len) ((unsigned int)((_len) << 24))
/* Bit 23-20 */
#define EDMAC_TRANSFER_CONFIG_DEST_BURST_LENGTH_MASK (0xF00000)
#define EDMAC_TRANSFER_CONFIG_DEST_BURST_LENGTH(_len) ((unsigned int)((_len) << 20))

/* Bit18-16 */
#define EDMAC_TRANSFER_CONFIG_SOUR_WIDTH_MASK (0x70000)
#define EDMAC_TRANSFER_CONFIG_SOUR_WIDTH(_len) ((unsigned int)((_len) << 16))
/* Bit14-12 */
#define EDMAC_TRANSFER_CONFIG_DEST_WIDTH_MASK (0x7000)
#define EDMAC_TRANSFER_CONFIG_DEST_WIDTH(_len) ((unsigned int)((_len) << 12))

/* Bit9-4 */
#define EDMAC_TRANSFER_CONFIG_REQUEST(_ulReg) ((_ulReg) << 4)
/* Bit3-2 */
#define EDMAC_TRANSFER_CONFIG_FLOW_DMAC(_len) ((unsigned int)((_len) << 2))

#define EDMAC_TRANSFER_CONFIG_INT_TC_ENABLE (0x2)
#define EDMAC_TRANSFER_CONFIG_INT_TC_DISABLE (0x0)

#define EDMAC_TRANSFER_CONFIG_CHANNEL_ENABLE (0x1)
#define EDMAC_TRANSFER_CONFIG_CHANNEL_DISABLE (0x0)
#define EDMAC_NEXT_LLI_ENABLE 0x2 /* Bit 1 */
/* Bit 15 */
#define EDMAC_TRANSFER_CONFIG_EXIT_ADD_MODE_A_SYNC (0UL)
#define EDMAC_TRANSFER_CONFIG_EXIT_ADD_MODE_AB_SYNC (0x00008000)

#define P2M_CONFIG (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_PRF_DMA) | EDMAC_TRANSFER_CONFIG_DEST_INC)
#define M2P_CONFIG (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_PRF_DMA) | EDMAC_TRANSFER_CONFIG_SOUR_INC)
#define M2M_CONFIG \
    (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_MEM_DMA) | EDMAC_TRANSFER_CONFIG_SOUR_INC | EDMAC_TRANSFER_CONFIG_DEST_INC)

/*
 * old version BALONG_DMA_SET_LLI
 * new name same as bsp. should bsp.h
 */
#define EDMAC_MAKE_LLI_ADDR(_p) (u32)((u32)(_p)&0xFFFFFFE0)

typedef enum edma_trans_type_e {
    MEM_MEM_DMA = 0x00,
    MEM_PRF_DMA = 0x01,
    MEM_PRF_PRF = 0x10
} EDMA_TRANS_TYPE_E;

typedef struct _BALONG_DMA_CB {
    volatile unsigned int lli;
    volatile unsigned int bindx;
    volatile unsigned int cindx;
    volatile unsigned int cnt1;
    volatile unsigned int cnt0;
    volatile unsigned int src_addr;
    volatile unsigned int des_addr;
    volatile unsigned int config;
} edma_cb_s __attribute__((aligned(32)));

typedef void (*channel_isr)(unsigned long channel_arg, unsigned int int_status); /* v9r1 */

#define mdrv_edma_basic_config(burst_width, burst_len)                                                         \
    (EDMAC_TRANSFER_CONFIG_SOUR_BURST_LENGTH(burst_len) | EDMAC_TRANSFER_CONFIG_DEST_BURST_LENGTH(burst_len) | \
        EDMAC_TRANSFER_CONFIG_SOUR_WIDTH(burst_width) | EDMAC_TRANSFER_CONFIG_DEST_WIDTH(burst_width))
#define EDMAC_BASIC_CONFIG(burst_width, burst_len) mdrv_edma_basic_config(burst_width, burst_len)

#define mdrv_edma_set_lli(addr, last) ((last) ? 0 : (EDMAC_MAKE_LLI_ADDR(addr) | EDMAC_NEXT_LLI_ENABLE))

/* 链式传输时的节点信息 */
struct edma_cb {
    volatile unsigned int lli; /* 指向下个LLI */
    volatile unsigned int bindx;
    volatile unsigned int cindx;
    volatile unsigned int cnt1;
    volatile unsigned int cnt0;     /* 块传输或者LLI传输的每个节点数据长度 <= 65535字节 */
    volatile unsigned int src_addr; /* 物理地址 */
    volatile unsigned int des_addr; /* 物理地址 */
    volatile unsigned int config;
} __attribute__((aligned(32)));

#define EDMA_CX_LLI (0x800)
#define EDMA_CX_CONFIG (0x81C)

/* 错误码定义 */
#ifndef OK
#define OK 0
#endif
#ifndef ERROR
#define ERROR (-1)
#endif

/* 错误码定义 */
#define DMAC_SUCCESS 0
#define DMA_FAIL (-1)

#define DMA_ERROR_BASE -100
#define DMA_CHANNEL_INVALID                     (DMA_ERROR_BASE - 1)
#define DMA_TRXFERSIZE_INVALID                  (DMA_ERROR_BASE - 2)
#define DMA_SOURCE_ADDRESS_INVALID              (DMA_ERROR_BASE - 3)
#define DMA_DESTINATION_ADDRESS_INVALID         (DMA_ERROR_BASE - 4)
#define DMA_MEMORY_ADDRESS_INVALID              (DMA_ERROR_BASE - 5)
#define DMA_PERIPHERAL_ID_INVALID               (DMA_ERROR_BASE - 6)
#define DMA_DIRECTION_ERROR                     (DMA_ERROR_BASE - 7)
#define DMA_TRXFER_ERROR                        (DMA_ERROR_BASE - 8)
#define DMA_LLIHEAD_ERROR                       (DMA_ERROR_BASE - 9)
#define DMA_SWIDTH_ERROR                        (DMA_ERROR_BASE - 0xa)
#define DMA_LLI_ADDRESS_INVALID                 (DMA_ERROR_BASE - 0xb)
#define DMA_TRANS_CONTROL_INVALID               (DMA_ERROR_BASE - 0xc)
#define DMA_MEMORY_ALLOCATE_ERROR               (DMA_ERROR_BASE - 0xd)
#define DMA_NOT_FINISHED                        (DMA_ERROR_BASE - 0xe)
#define DMA_CONFIG_ERROR                        (DMA_ERROR_BASE - 0xf)


/* 错误码定义 */
#define EDMA_SUCCESS DMAC_SUCCESS
#define EDMA_FAIL DMA_FAIL

#define EDMA_ERROR_BASE                     DMA_ERROR_BASE
#define EDMA_CHANNEL_INVALID                DMA_CHANNEL_INVALID
#define EDMA_TRXFERSIZE_INVALID             DMA_TRXFERSIZE_INVALID
#define EDMA_SOURCE_ADDRESS_INVALID         DMA_SOURCE_ADDRESS_INVALID
#define EDMA_DESTINATION_ADDRESS_INVALID    DMA_DESTINATION_ADDRESS_INVALID
#define EDMA_MEMORY_ADDRESS_INVALID         DMA_MEMORY_ADDRESS_INVALID
#define EDMA_PERIPHERAL_ID_INVALID          DMA_PERIPHERAL_ID_INVALID
#define EDMA_DIRECTION_ERROR                DMA_DIRECTION_ERROR
#define EDMA_TRXFER_ERROR                   DMA_TRXFER_ERROR
#define EDMA_LLIHEAD_ERROR                  DMA_LLIHEAD_ERROR
#define EDMA_SWIDTH_ERROR                   DMA_SWIDTH_ERROR
#define EDMA_LLI_ADDRESS_INVALID            DMA_LLI_ADDRESS_INVALID
#define EDMA_TRANS_CONTROL_INVALID          DMA_TRANS_CONTROL_INVALID
#define EDMA_MEMORY_ALLOCATE_ERROR          DMA_MEMORY_ALLOCATE_ERROR
#define EDMA_NOT_FINISHED                   DMA_NOT_FINISHED
#define EDMA_CONFIG_ERROR                   DMA_CONFIG_ERROR

/*
 * 函数void (*channel_isr)(u32 channel_arg, u32 int_status)的参数int_status、
 * 函数s32 bsp_edma_channel_init (enum edma_req_id req,  channel_isr pFunc,
 * u32 channel_arg, u32 int_flag)的参数int_flag 为以下几种中断类型，可组合
 */
#define EDMA_INT_DONE BALONG_DMA_INT_DONE                 /* EDMA传输完成中断 */
#define EDMA_INT_LLT_DONE BALONG_DMA_INT_LLT_DONE         /* 链式EDMA节点传输完成中断 */
#define EDMA_INT_CONFIG_ERR BALONG_DMA_INT_CONFIG_ERR     /* EDMA配置错误导致的中断 */
#define EDMA_INT_TRANSFER_ERR BALONG_DMA_INT_TRANSFER_ERR /* EDMA传输错误导致的中断 */
#define EDMA_INT_READ_ERR BALONG_DMA_INT_READ_ERR         /* EDMA链表读错误导致的中断 */
#define EDMA_INT_ALL 0x1F

/* EDMAC传输方向定义 */
#define EDMA_P2M BALONG_DMA_P2M
#define EDMA_M2P BALONG_DMA_M2P
#define EDMA_M2M BALONG_DMA_M2M

#define EDMA_GET_DEST_ADDR 0
#define EDMA_GET_SOUR_ADDR 1

#define EDMA_DATA_TIMEOUT 10

/* EDMAC流控制与传输类型 */
#define MEM_MEM_EDMA MEM_MEM_DMA /* 内存到内存，DMA流控 */
#define MEM_PRF_EDMA MEM_PRF_DMA /* 内存与外设，DMA流控 */

#define EDMAC_MAX_CHANNEL (16)

#define EDMAC_DT_NUM (16)
/* ACPU: 0, CCPU: 1 */
#ifdef INT_LVL_EDMAC0

#define EDMAC_DT_ARM (0)
#define INT_LVL_EDMAC INT_LVL_EDMAC0
#define INT_LVL_EDMAC_CH4 INT_LVL_EDMAC_CH4_0

#elif (defined INT_LVL_EDMA_MDM1) || (defined INT_LVL_EDMA_MDM0)

#define EDMAC_DT_ARM (0)
#define INT_LVL_EDMAC INT_LVL_EDMA_MDM0
#define INT_LVL_EDMAC_CH4 INT_LVL_EDMAC_SOC_0

#endif

#define EDMA_CH16_ID 0
#define EDMA_CH4_ID 1
#define EDMA_NUMBER 2

#define EDMA_ID(channel_id) ((channel_id < EDMA_CH16_NUM) ? EDMA_CH16_ID : EDMA_CH4_ID)
#define EDMA_ID_CHAN_ID(channel_id) ((channel_id < EDMA_CH16_NUM) ? channel_id : (channel_id - EDMA_CH16_NUM))

/*
 * EDMA 对应的具体位   供EDMA 寄存器配置宏
 * EDMAC_BASIC_CONFIG、EDMA_SET_LLI、EDMA_SET_CONFIG 使用
 */
/* Bit3-2 */
#define EDMAC_TRANSFER_CONFIG_FLOW_EDMAC(_len) EDMAC_TRANSFER_CONFIG_FLOW_DMAC(_len)

/* addr:物理地址 */
#define EDMA_SET_LLI(addr, last) mdrv_edma_set_lli(addr, last)

#define BALONG_DMA_SET_CONFIG(req, direction, burst_width, burst_len)                                     \
    (EDMAC_BASIC_CONFIG(burst_width, burst_len) | EDMAC_TRANSFER_CONFIG_REQUEST(req) |                    \
        EDMAC_TRANSFER_CONFIG_INT_TC_ENABLE | EDMAC_TRANSFER_CONFIG_CHANNEL_ENABLE /*lint -save -e506*/ | \
        ((direction == BALONG_DMA_M2M) ? M2M_CONFIG :                                                     \
                                         ((direction == BALONG_DMA_P2M) ? P2M_CONFIG : M2P_CONFIG))) /*lint -restore*/

#define EDMA_SET_CONFIG(req, direction, burst_width, burst_len) \
    BALONG_DMA_SET_CONFIG(req, direction, burst_width, burst_len)

#define EDMA_CHANNEL_DISABLE EDMAC_TRANSFER_CONFIG_CHANNEL_DISABLE
#define EDMA_CHANNEL_ENABLE EDMAC_TRANSFER_CONFIG_CHANNEL_ENABLE
#define EDMA_NEXT_LLI_ENABLE EDMAC_NEXT_LLI_ENABLE
// reserved reg num calculate
#define EDMA_CX_RESERVED_REG_NUM (8)
#define EDMA_INT_RESERVED_REG_NUM ((0x40 - 0x28) / 4 - 1)
#define EDMA_INT_GLOBAL_RESERVED_REG_NUM ((0x600 - 0x400) / 4)
#define EDMA_GLOBAL_RESERVED_REG_NUM ((0x660 - 0x620) / 4 - 1)
#define EDMA_GLOBAL_RESERVED_REG_NUM_2 ((0x688 - 0x674) / 4 - 1)
#define EDMA_GLOBAL_CX_RESERVED_REG_NUM ((0x0700 - 0x698) / 4 - 1)

struct chan_int_service {
    channel_isr chan_isr;
    unsigned long chan_arg;
    u32 int_status;
};

struct EDMAC_FREE_NODE_HEADER_STRU {
    u32 ulInitFlag;
    u32 ulFirstFreeNode;
};

struct EDMA_LLI_ALLOC_ADDRESS_STRU {
    void *s_alloc_virt_address;
    void *s_alloc_phys_address;
};

struct EDMA_SIMPLE_LLI_STRU {
    u32 ulSourAddr;
    u32 ulDestAddr;
    u32 ulLength;
    u32 ulConfig;
};

struct EDMAC_TRANSFER_CONFIG_STRU {
    struct edma_cb lli_node_info;
    volatile u32 ulPad10[EDMA_CX_RESERVED_REG_NUM];
};

struct EDMAC_CURR_STATUS_REG_STRU {
    volatile u32 ulCxCurrCnt1;
    volatile u32 ulCxCurrCnt0;
    volatile u32 ulCxCurrSrcAddr;
    volatile u32 ulCxCurrDesAddr;
};

/* EDMA 寄存器结构体 */
typedef struct {
    /* 0x0000--- */
    volatile u32 ulIntState;
    /* 0x0004--- */
    volatile u32 ulIntTC1;
    /* 0x0008--- */
    volatile u32 ulIntTC2;
    /* 0x000C--- */
    volatile u32 ulIntErr1;
    /* 0x0010--- */
    volatile u32 ulIntErr2;
    /* 0x0014--- */
    volatile u32 ulIntErr3;
    /* 0x0018--- */
    volatile u32 ulIntTC1Mask;
    /* 0x001C--- */
    volatile u32 ulIntTC2Mask;
    /* 0x0020--- */
    volatile u32 ulIntErr1Mask;
    /* 0x0024--- */
    volatile u32 ulIntErr2Mask;
    /* 0x0028--- */
    volatile u32 ulIntErr3Mask;
    volatile u32 ulPad[EDMA_INT_RESERVED_REG_NUM];
} EDMAC_CPU_REG_STRU;

struct edma_reg_struct {
    volatile EDMAC_CPU_REG_STRU stCpuXReg[EDMAC_DT_NUM];

    volatile u32 ulPad0[EDMA_INT_GLOBAL_RESERVED_REG_NUM]; // reserved reg
    /* 0x0600-- */
    volatile u32 ulIntTC1Raw;
    volatile u32 ulPad1;
    /* 0x0608-- */
    volatile u32 ulIntTC2Raw;
    volatile u32 ulPad2;
    /* 0x0610-- */
    volatile u32 ulIntERR1Raw;
    volatile u32 ulPad3;
    /* 0x0618-- */
    volatile u32 ulIntERR2Raw;
    volatile u32 ulPad4;
    /* 0x0620-- */
    volatile u32 ulIntERR3Raw;
    volatile u32 ulPad5[EDMA_GLOBAL_RESERVED_REG_NUM];
    /* 0x0660-- */
    volatile u32 ulSingleReq;
    volatile u32 ulLastSingleReq;
    volatile u32 ulBurstReq;
    volatile u32 ulLastBurstReq;
    volatile u32 ulFlushReq;
    volatile u32 ulLastFlushReq;
    volatile u32 ulPad6[EDMA_GLOBAL_RESERVED_REG_NUM_2];
    /* 0x0688-- */
    volatile u32 ulChannelPrioritySet;
    volatile u32 ulPad7;
    /* 0x0690-- */
    volatile u32 ulChannelState;
    volatile u32 ulPad8;
    /* 0x0698 -- */
    volatile u32 ulDmaCtrl;
    volatile u32 ulPad9[EDMA_GLOBAL_CX_RESERVED_REG_NUM];
    /* 0x0700 -- */
    volatile struct EDMAC_CURR_STATUS_REG_STRU stCurrStatusReg[EDMAC_MAX_CHANNEL];
    /* 0x0800 -- */
    volatile struct EDMAC_TRANSFER_CONFIG_STRU stTransferConfig[EDMAC_MAX_CHANNEL];
};

#ifndef EDMAC_MAKE_LLI_ADDR
/* for acore,cause c_mdrv defined */
#define EDMAC_MAKE_LLI_ADDR(_p) (u32)((u32)(_p) & 0xFFFFFFE0)
#endif

#define EDMAC_CHANNEL_CB(x) (g_edma_drv_info.edma_reg_str->stTransferConfig[x].lli_node_info)

#ifdef CONFIG_EDMA_SUPPORT

s32 bsp_edma_channel_init(enum edma_req_id request, channel_isr pFunc, unsigned long channel_arg, u32 int_flag);
s32 bsp_edma_current_transfer_address(u32 channel_id);
u64 bsp_edma_current_cnt(u32 channel_id);
s32 bsp_edma_channel_stop(u32 channel_id);
s32 bsp_edma_channel_is_idle(u32 channel_id);
s32 bsp_edma_chanmap_is_idle(u32 channel_map);
void bsp_edma_channel_obtainable(void);
s32 bsp_edma_channel_set_config(u32 channel_id, u32 direction, u32 burst_width, u32 burst_len);
s32 bsp_edma_channel_dest_set_config(u32 channel_id, u32 dest_width, u32 dest_len);
s32 bsp_edma_channel_start(u32 channel_id, u32 src_addr, u32 des_addr, u32 len);
s32 bsp_edma_channel_2vec_start(u32 channel_id, u32 src_addr, u32 des_addr, u32 len, u32 size_align);
s32 bsp_edma_channel_async_start(u32 channel_id, u32 src_addr, u32 des_addr, u32 len);
struct edma_cb *bsp_edma_channel_get_lli_addr(u32 channel_id);
s32 bsp_edma_channel_lli_start(u32 channel_id);
s32 bsp_edma_channel_lli_async_start(u32 channel_id);
s32 bsp_edma_channel_free(u32 channel_id);
void edma_test_start(u32 edma_test_chan_id, u32 src, u32 dest, u32 size, u32 config);

/* v7r1 bus stress */
typedef struct tagAXI_DMA_TASK_HANDLE_S {
    unsigned int u32SrcAddr;
    unsigned int u32DstAddr;
    unsigned int ulLength;
    unsigned int ulChIdx;
} AXI_DMA_TASK_HANDLE_S;

s32 EDMA_NormTaskLaunch(AXI_DMA_TASK_HANDLE_S *pstDMAHandle);
s32 EDMA_2VecTaskLaunch(AXI_DMA_TASK_HANDLE_S *pstDMAHandle);
s32 EDMA_QueryCHNLState(u32 u32ChIdx);
s32 EDMA_QueryCHNsLState(u32 u32ChIdxMap);

#ifdef CONFIG_MPERF
struct edma_chan_perf {
    u32 chan_id;
    u32 size;
    u32 time_total;
};
s32 bsp_edma_get_perf_info(struct edma_chan_perf *chan_perf);
void edma_perf_enable(void);
void edma_perf_disable(void);
#endif
#endif

#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif /* End of DMADRV_H */
