/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef _EICC_DEVICE_H
#define _EICC_DEVICE_H

#include "eicc_platform.h"

#define EICC_IRQ_CNT 2
#define EICC_CPU_ID_MAX 16
#define EICC_CHN_ID_MAX 64
/* 每个寄存器存放16个PIPE的安全配置，需要4个寄存器来存放 */
#define EICC_PIPE_SEC_REGS_CNT (EICC_CHN_ID_MAX >> 4)
#define EICC_DFT_INVAILD_VALUE 0xFFFFFFFF

#define EICC_PIPE_ADDR_ALIGN 0x8
#define EICC_PIPE_ADDR_ALIGN_MASK (EICC_PIPE_ADDR_ALIGN - 1)
#define EICC_PIPE_PUSHPTR_ALIGN 0x4
#define EICC_PIPE_PUSHPTR_ALIGN_MASK (EICC_PIPE_PUSHPTR_ALIGN - 1)
#define EICC_PIPE_DEPTH_ALIGN 0x80
#define EICC_PIPE_DEPTH_ALIGN_MASK (EICC_PIPE_DEPTH_ALIGN - 1)
#define EICC_PIPE_DEPTH_MIN 0x80
#define EICC_PIPE_DEPTH_MAX 0xFF80

/* DMA操作默认对齐字节数 */
#define EICC_DMA_DFT_ALIGN 0x4
#define EICC_DMA_DFT_ALIGN_MASK (EICC_DMA_DFT_ALIGN - 1)

#define EICC_CHN_MIN_ALIGNBYTES 0x4
#define EICC_CHN_DEFAULT_ALIGNBYTES 0x4

#define EICC_HALT_QUERY_WAIT_TIMES 200

/* 芯片给出的值 */
#define EICC_OPIPE_STOP_WAIT 100
#define EICC_IPIPE_STOP_WAIT 300
#define EICC_IPIPE_IDLE_WAIT 300

struct eicc_descriptor {
    u32 msg_len : 16; /* [0-15] */
    u32 P_pos : 1;    /* [16]P */
    u32 S_pos : 1;    /* [17]S */
    u32 D_pos : 1;    /* [18]D */
    u32 M_pos : 1;    /* [19]M */
    u32 N_pos : 1;    /* [20]N */
    u32 C_pos : 1;    /* [21]C */
    u32 F_pos : 1;    /* [22]F */
    u32 V_pos : 1;    /* [23]V */
    u32 II_pos : 1;   /* [24]II */
    u32 OI_pos : 1;   /* [25]OI */
    u32 SI_pos : 1;   /* [26]SI */
    u32 DI_pos : 1;   /* [27]DI */
    u32 reserved : 4; /* [28-31] */
};

struct eicc_bd1_s {
    struct eicc_descriptor cfg;
} __attribute__((aligned(4)));

struct eicc_bd4_s {
    struct eicc_descriptor cfg;
    u32 src_addr_l;
    struct {
        u32 src_addr_h : 8; /* [0-7]-->next_ptr[32-39] */
        u32 reserved1 : 8;  /* [8-15] */
        u32 reserved2 : 8;  /* [16-23] */
        u32 des_addr_l : 8; /* [24-31]-->src_addr[0-7] */
    } ptr;
    u32 des_addr_h;
} __attribute__((aligned(4)));

struct eicc_bd7_s {
    struct eicc_descriptor cfg;
    u32 src_addr_l;
    struct {
        u32 src_addr_h : 8; /* [0-7]-->des_addr[32-39] */
        u32 reserved1 : 8;  /* [8-15] */
        u32 reserved2 : 8;  /* [16-23] */
        u32 des_addr_l : 8; /* [24-31]-->src_addr[0-7] */
    } ptr;
    u32 des_addr_h;
    struct {
        u32 des_offset : 16; /* [0-7]-->des_addr[32-39] */
        u32 src_offset : 16; /* [8-15] */
    } off;
    struct {
        u32 blockcnt : 16; /* [0-7]-->des_addr[32-39] */
        u32 reserved : 16; /* [8-15] */
    } blkdesc;
} __attribute__((aligned(4)));

/* 数据包头保持4字节对齐(芯片约束) */
struct eicc_packet {
    u32 user_id;   /* same with v100 */
    u16 len;       /* same with v100 */
    u16 task_id;   /* same with v100 */
    u32 timestamp; /* same with v100 该数据包发送时间戳 */
    u32 align_bytes;
    u32 ldrvchn_id; /* 维测，不做判据 */
    u32 rdrvchn_id; /* 维测，不做判据 */
    u32 recvtime1;  /* 该数据包接收时间戳 */
    u32 recvtime2;  /* 该数据包接收时间戳 */
};                  /* 驱动包头最大32字节对齐预留 */

struct eicc_send_packet {
    struct eicc_bd1_s bd_head;
    struct eicc_packet data_head;
};

enum EICC_INT_READ_LEV {
    EICC_INT_RD_UNDERFLOW = 0x1,
    EICC_INT_RD_OVERFLOW = 0x2,
    EICC_INT_RD_DATA_ARV = 0x4,
    EICC_INT_RD_ARV_TIMEOUT = 0x8,
    EICC_INT_RD_PIPE_WAKEUP = 0x10
};

enum EICC_INT_WRITE_LEV {
    EICC_INT_WR_UNDERFLOW = 0x1,
    EICC_INT_WR_OVERFLOW = 0x2,
    EICC_INT_WR_DATA_DONE = 0x4,
    EICC_INT_WR_PIPE_ERR = 0x8,
    EICC_INT_WR_TRANSCNT_FLIP = 0x10,
    EICC_INT_WR_ABORT = 0x20 /* 这个中断一定不要开，因为应用中强停通道可能停其他核的 */
};

/* 通道状态 */
enum eicc_chn_stat {
    EICC_CHN_INIT = 0x0001,         /* 初始化完的状态，以及调用close之后的状态 */
    EICC_CHN_OPEN = 0x0002,         /* 调用open后打开的状态 */
    EICC_CHN_OPENNOSR = 0x0003,     /* 某些特殊通道调用open后打开的状态 */
    EICC_CHN_OSUSPENDING = 0x0004,  /* 低功耗正在关闭opipe的状态 */
    EICC_CHN_OSUSPENDED = 0x0005,   /* 低功耗已经关闭opipe的状态 */
    EICC_CHN_ISUSPENDING = 0x0006,  /* 低功耗正在关闭ipipe的状态 */
    EICC_CHN_ISUSPENDED = 0x0007,   /* 低功耗已经关闭ipipe的状态 */
    EICC_CHN_SUSPENDED = 0x0008,    /* 低功耗已经完成empty检查，完成低功耗的状态 */
    EICC_CHN_DSSSUSPENDED = 0x0009, /* 某些特殊通道低功耗已经完成empty检查，完成低功耗的状态 */
    EICC_CHN_CLOSING = 0x000A,      /* 调用close的中间的状态 */
    EICC_CHN_ORESTTING = 0x000B,    /* 单独复位正在关闭opipe的状态 */
    EICC_CHN_ORESTTED = 0x000C,     /* 单独复位已经关闭opipe的状态 */
    EICC_CHN_IRESTTING = 0x000D,    /* 单独复位正在关闭ipipe的状态 */
    EICC_CHN_IRESTTED = 0x000E,     /* 单独复位已经关闭ipipe的状态 */
    EICC_CHN_RESTTED = 0x000F,      /* 单独复位已经完成关闭pipe的状态 */
};

struct compat_attr {
    u32 pa;    /* ringbuf物理地址，硬件有对齐要求 8Bytes */
    void *va;  /* ringbuf虚拟地址 */
    u32 depth; /* depth，硬件有对齐要求 128Bytes */
    u32 buf_sz;
    u32 align_bytes;
    u32 dym_flags;
    u32 flup;    /* 上水线配置 pipe_ctrl.union_struct.up_thrh */
    u32 fldn;    /* 下水线配置 pipe_ctrl.union_struct.dn_thrh */
    u32 int_lev; /* 中断配置 EICC_INT_WRITE_LEV for opipe and EICC_INT_READ_LEV for ipipe */
};

union glb_ctrl {
    u32 val;
    struct {
        u32 global_srst : 1; /* EICC模块全局软复位控制，自动清零。1：请求复位.0：不请求复位 */
        u32 clkauto_en : 1;  /* 自动门控使能控制位。0：不门控,1：EICC不使能，时钟关闭 */
        u32 push_en : 1;     /* 低功耗流程推送功能控制信位。0:不推送,1:推送 */
        u32 hresp_bypass : 1; /* AHB总线hresp返回error控制位。0：总线访问出错，返回error.1：总线访问出错，不返回error */
        u32 bus_err_bypass : 1; /* AXI总线resp error控制位。0：处理总线resp error.1：不处理总线resp error */
        u32 reserved : 26;      /* reserved */
        u32 eicc_ilde : 1;      /* eicc_idle */
    } union_stru;
};

struct eicc_glb_cfg {
    u32 glb_sec;
    u32 core_sec;
    u32 opipe_sec0;
    u32 opipe_sec1;
    u32 opipe_sec2;
    u32 opipe_sec3;
    u32 ipipe_sec0;
    u32 ipipe_sec1;
    u32 ipipe_sec2;
    u32 ipipe_sec3;
    union glb_ctrl ctrl_info;
    u32 os_cfg;
    u32 trans_stat;
    u32 trans_data_cnt;
};

union opipe_ctrl {
    u32 val;
    struct {
        u32 local_en : 1; /* 1:pipe使能 0:pipe不使能 */
        u32 type : 1; /* PIPE类型。0：opipe与ipipe有关联(MSG通道);1：opipe与ipipe无关联（DMA通道） */
        u32 prior : 2; /* 高低优先级队列配置，支持4个优先级，0为高优先级，3为低优先级。 */
        u32 up_thrh : 2; /* PIPE上水线：00：一个数据块或则一个消息;01：1/2深度;10：3/4深度;11：7/8深度 */
        u32 dn_thrh : 2; /* PIPE下水线：00：一个数据块或则一个消息;01：1/8深度;10：1/4深度;11：1/2深度 */
        u32 reserved : 24;
    } union_stru;
};

union ipipe_ctrl {
    u32 val;
    struct {
        u32 local_en : 1; /* 1:pipe使能 0:pipe不使能 */
        u32 type : 2; /* PIPE类型。00：内部通道的ipipe;01：外部通道的实体ipipe;10：外部通道的影子ipipe;11：保留 */
        u32 prior : 2; /* 高低优先级队列配置，支持4个优先级，0为高优先级，3为低优先级。 */
        u32 up_thrh : 2; /* PIPE上水线：00：一个数据块或则一个消息;01：1/2深度;10：3/4深度;11：7/8深度 */
        u32 dn_thrh : 2; /* PIPE下水线：00：一个数据块或则一个消息;01：1/8深度;10：1/4深度;11：1/2深度 */
        u32 arv_timeout : 6; /* 数据到达超时：[5]：超时计时粒度，1表示按照ms计数，0表示按照us计数[4:0]：超时阈值，计数器计数到达阈值，上报数据到达超时中断，0表示不计时
                              */
        u32 reserved : 17;
    } union_stru;
};

union opipe_stat {
    u32 val;
    struct {
        u32 is_empty : 1; /* 1:空0:不空 */
        u32 is_full : 1;  /* 1:满0:不满 */
        u32 is_idle : 1;  /* 1:idle  0:busy  */
        u32 reserved : 29;
    } union_stru;
};

union ipipe_stat {
    u32 val;
    struct {
        u32 is_empty : 1; /* 1:空0:不空 */
        u32 is_full : 1;  /* 1:满0:不满 */
        u32 is_idle : 1;  /* 1:idle  0:busy  */
        u32 pt_stat : 2;  /* 00:不在低功耗状态  01:请求下电10:拒绝下电11:允许下电  */
        u32 reserved : 27;
    } union_stru;
};
/* clang-format off */
enum {
    EICC_NOT_PM_STAT = 0x0,
    EICC_PM_REQ_PD = 0x1,
    EICC_PM_REFUSE_PD = 0x2,
    EICC_PM_ALLOW_PD = 0x3
};
/* clang-format on */
struct opipe_config {
    u32 id; /* pipe编号  0-63 */
    void *ringbuf_va;
    eiccsoc_ptr_t ringbuf_pa;
    u32 depth;    /* 通道使用的buff大小， 128Bytes对齐 */
    u32 ram_size; /* 用户传入的buff大小 */
    u32 wptr;
    u32 rptr;
    u32 *ptr_vaddr;
    eiccsoc_ptr_t ptr_paddr;
    union opipe_ctrl opipe_uctrl;
    u32 int_level;
};

struct ipipe_config {
    u32 id;
    void *ringbuf_va;
    eiccsoc_ptr_t ringbuf_pa;
    u32 depth;
    u32 ram_size;
    u32 wptr;
    u32 rptr;
    u32 seek;
    u32 *ptr_vaddr;
    eiccsoc_ptr_t ptr_paddr;
    eiccsoc_ptr_t relv_pipe_paddr;
    union ipipe_ctrl ipipe_uctrl;
    u32 int_level;
};

/* used for eicc_channel.type */
#define EICC_CHN_TYPE_MSG_OUTSEND 0x0
#define EICC_CHN_TYPE_MSG_OUTRECV 0x1
#define EICC_CHN_TYPE_MSG_INRSEND 0x2
#define EICC_CHN_TYPE_MSG_INRRECV 0x3
#define EICC_CHN_TYPE_DMA 0x4

/* 这几个宏要与 mdrv_eicc.h 里面的MDRV_ATTR对应 */
#define EICC_DYM_SF_NOARRVEVENT (1UL << 0)
#define EICC_DYM_SF_ENOFLOWACT (1UL << 1)
#define EICC_DYM_SF_ENIFLOWACT (1UL << 2)
#define EICC_DYM_SF_NOWAKEUPACT (1UL << 3)
#define EICC_DYM_SF_DPMSRIGNORE (1UL << 4)
#define EICC_DYM_SF_DSSCTRLSR (1UL << 31)

/* 这种通道在单独复位中需要处理 */
#define EICC_CHN_SF_MDMRST_CARE (1UL << 29)
/* 远端通道为非安全通道 */
#define EICC_CHN_SF_REMOTE_UNSEC (1UL << 30)
/* 用于FPGA,EMU,ESL等场景，EICC总是上电的，所以不需要唤醒，可以直接数据收发 */
#define EICC_CHN_SF_REMOTE_READY (1UL << 31)

struct eicc_channel {
    u32 user_id;     /* 系统初始化时赋值 */
    u32 type;        /* 通道类型，系统初始化时赋值 ,不可改变 */
    u32 ldrvchn_id;  /* 通道类型，系统初始化时赋值 ,不可改变 */
    u32 rdrvchn_id;  /* 通道类型，系统初始化时赋值 ,不可改变 */
    u32 dym_flags;   /* 通道初始化的时候就确定的通道属性，在运行过程中不会改变 */
    u32 const_flags; /* 配置文件中决定的通道属性，系统初始化时赋值 ,不可改变 */
    u32 align_bytes; /* 消息头和消息体对齐方式，默认按照硬件约束4字节对齐,通道初始化确定，在运行过程中不会改变 */
    u32 state;       /* 通道状态: 标识发送通道是否可用 */
    u32 remote_awake; /* 通道睡眠状态标记 */
    u32 irq_ts;       /* 最后一次响应中断的时间 */
    spinlock_t lock;  /* 保护通道互斥的spin锁 */
    struct opipe_config *opipe_cfg;
    struct ipipe_config *ipipe_cfg;
    eicc_cbk cbk;  /* eicc事件发生时的回调函数 */
    void *cbk_arg; /* 回调时，透传的信息，用户需要保证cbk_arg的生命周期 */
};

/*lint -esym(528,isMsgSendChn) */
static inline int isMsgSendChn(struct eicc_channel *pchannel)
{
    return (pchannel->type == EICC_CHN_TYPE_MSG_OUTSEND || pchannel->type == EICC_CHN_TYPE_MSG_INRSEND);
}
/*lint -esym(528,isMsgRecvChn) */
static inline int isMsgRecvChn(struct eicc_channel *pchannel)
{
    return (pchannel->type == EICC_CHN_TYPE_MSG_OUTRECV || pchannel->type == EICC_CHN_TYPE_MSG_INRRECV);
}

/* 中断不合并， 收发各自合并-即二合一， 收和发都合并-即四合一 */
#define EICC_IRQ_TYPE_ORIGIN 0
#define EICC_IRQ_TYPE_OIMERG 1
#define EICC_IRQ_TYPE_ALMERG 2
struct irq_bundle {
    u32 cpu_id;               /* 用于读中断状态和判断通道是否归属自己 */
    u32 irq[EICC_IRQ_CNT];    /* 一组最多EICC_IRQ_CNT个中断 */
    struct eicc_device *pdev; /* 用于反向索引dev */
};

/* used for eicc_device.state */
#define EICC_DEV_STATE_WORKING 1
#define EICC_DEV_STATE_SLEEPING 2
/* used for eicc_device.ctrl_level */
#define EICC_DEV_CONTROL_LEVEL_IGNORE 0
#define EICC_DEV_CONTROL_LEVEL_IRQCLR 1
#define EICC_DEV_CONTROL_LEVEL_GUEST 2
#define EICC_DEV_CONTROL_LEVEL_HOST 3
struct eicc_device {
    u32 id; /* eicc设备在soc里面的编号 */
    u32 version_id;
    u32 ctrl_level;
    u32 pipepair_cnt;
    void *base_va;
    eiccsoc_ptr_t base_pa;
    u32 *push_ptr_va;
    eiccsoc_ptr_t push_ptr_pa;
    u32 state;
    u32 irq_type;
    struct irq_bundle *maps[EICC_CPU_ID_MAX]; /* 大部分设备只有一组，这样节省内存 */
    spinlock_t lock;
    u32 glb_ctrl;
    struct eicc_channel *channels[EICC_CHN_ID_MAX];
    void *of_node;
};

int eicc_devices_init(void);
struct eicc_device *eicc_device_get_fast(u32 devid);
struct eicc_channel *eicc_channel_get_fast(u32 usr_chnid);
struct eicc_channel *eicc_channel_get_byidx(u32 idx);
int eicc_dev_chn_get(u32 phy_chn_id, struct eicc_device **ppdev, struct eicc_channel **ppchannel);
int eicc_dev_chn_get_byid(u32 user_id, struct eicc_device **ppdev, struct eicc_channel **ppchannel);
int eicc_dev_chn_get_byidx(u32 chn_idx, struct eicc_device **ppdev, struct eicc_channel **ppchannel);

#endif
