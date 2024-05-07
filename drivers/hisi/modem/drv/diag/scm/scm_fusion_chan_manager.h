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

#ifndef _SCM_FUSION_CHAN_MANAGER_H_
#define _SCM_FUSION_CHAN_MANAGER_H_

#include <osl_types.h>
#include <osl_spinlock.h>
#include <bsp_dump.h>
#include "bsp_diag.h"
#include "mdrv_diag.h"
#include "mdrv_socp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SCM_CODE_SRC_BD_NUM (4 * 1024)                                      /* 链式通道BD的个数 */
#define SCM_CODE_SRC_RD_NUM (4 * 1024)                                      /* 链式通道RD的个数 */
#define SCM_CODER_SRC_BDSIZE (sizeof(socp_rd_data_s) * SCM_CODE_SRC_BD_NUM) /* 链式通道的BD空间 */
#define SCM_CODER_SRC_RDSIZE (sizeof(socp_rd_data_s) * SCM_CODE_SRC_RD_NUM) /* 链式通道的RD空间 */

#define SCM_CODER_SRC_CNF_BUFFER_SIZE (1024 * 256)
#define SCM_CODER_SRC_IND_BUFFER_SIZE (1024 * 1024)
#define SCM_CODER_SRC_MAX_LEN (4 * 1024) /* 编码通道最大发送数据长度  */
#define SCM_CODER_SRC_RD_THRESHOLD 0     /* 编码源通道RD阈值 */

#define SCM_CODER_DST_CNF_SIZE (512 * 1024)             /* 命令回复编码目的通道空间大小 */
#define SCM_CODER_DST_IND_SIZE (1 * 1024 * 1024)        /* 主动上报编码目的通道空间大小 */
#define SCM_CODER_CNF_DST_THRESHOLD 256                 /* CNF编码目的通道阈值 */
#define SCM_CODER_IND_DST_THRESHOLD (64 * 1024)         /* IND编码目的通道阈值 */
#define SCM_CODER_DST_GTHRESHOLD ((4 * 1024) + 8 + 128) /* 编码源通道仲裁阈值 + DMA一次最小搬运字节 */
#define EN_NV_ID_POWER_ON_LOG_SWITCH 0xD33B

typedef enum {
    SCM_CHANNEL_UNINIT = 0, /* 未初始化 */
    SCM_CHANNEL_INIT_SUCC,  /* 初始化成功 */
    SCM_CHANNEL_MEM_FAIL,   /* 初始化申请内存错误 */
    SCM_CHANNEL_CFG_FAIL,   /* 初始化通道配置 */
    SCM_CHANNEL_START_FAIL, /* 通道开启错误 */
    SCM_CHANNEL_INIT_BUTT
} scm_chan_init_state_e;

typedef enum {
    SCM_SOCP_CNF_CHAN,
    SCM_SOCP_IND_PS_CHAN,
    SCM_SOCP_SRC_CHAN_BUTT
} scm_socp_src_chan_e;

typedef enum {
    SCM_SOCP_CNF_DST_CHAN,
    SCM_SOCP_IND_DST_CHAN,
    SCM_SOCP_DST_CHAN_BUTT
} scm_socp_dst_chan_e;

typedef void (*scm_coder_dst_read_cb)(u8 *virt_addr, u8 *phy_addr, u32 len);

typedef struct {
    unsigned int init_state;
    unsigned int chan_id;
    u32 buff_len;
    u32 threshold;
    u32 timeout_mode;
    u8 *dst_virt_buff;
    u8 *dst_phy_buff;
    scm_coder_dst_read_cb func; /* 编码目的通道数据处理回调函数 */
} scm_coder_dst_cfg_s;

typedef struct {
    unsigned int init_state;  /* 通道初始化状态，初始化后自动修改 */
    unsigned int chan_id;     /* 编码源通道ID，固定配置 */
    unsigned int dst_chan_id; /* 编码目的通道ID */
    unsigned int data_type;   /* 数据来源类型 */
    unsigned int chan_mode;   /* 通道类型 */
    unsigned int chan_prio;   /* 通道优先级 */
    unsigned int trans_id_en; /* SOCP Trans Id使能位 */
    unsigned int rptr_img_en; /* SOCP 指针镜像使能位 */
    u32 src_buff_len;         /* 编码源通道数据空间大小 */
    u32 rd_buff_len;          /* 编码源通道RD数据空间大小 */
    u8 *src_virt_buff;        /* 编码源通道数据空间内存指针 */
    u8 *src_phy_buff;
    u8 *rd_virt_buff; /* 编码源通道RD数据空间内存指针 */
    u8 *rd_phy_buff;
    u8 *rptr_img_phy_addr;  /* 编码源通道读指针镜像物理地址 */
    u8 *rptr_img_virt_addr; /* 编码源通道读指针镜像虚拟地址 */
} scm_coder_src_cfg_s;

s32 scm_cnf_dst_buff_init(void);
s32 scm_ind_dst_buff_init(void);
s32 scm_malloc_ind_dst_buff(void);
s32 scm_dst_chan_buff_init(void);
s32 scm_src_chan_buff_init(void);
s32 scm_src_chan_cfg_init(void);
unsigned long *scm_cnf_src_phy_to_virt(const u8 *phy_addr);
unsigned long *scm_ind_src_phy_to_virt(const u8 *phy_addr);
s32 scm_src_chan_cfg(scm_coder_src_cfg_s *chan_attr);
s32 scm_dst_chan_cfg(scm_coder_dst_cfg_s *chan_attr);
u32 scm_rls_cnf_dst_buff(u32 read_size);
u32 scm_rls_ind_dst_buff(u32 read_size);
s32 scm_rls_dst_buff(u32 chan_id, u32 read_size);
s32 scm_ind_cnf_cfg_init(void);
s32 scm_ind_dst_cfg_init(void);
s32 scm_dst_chan_cfg_init(void);
void scm_reg_cnf_coder_dst_send_func(void);
void scm_reg_ind_coder_dst_send_func(void);
int scm_cnf_dst_read_cb(unsigned int chan_id);
int scm_ind_dst_read_cb(unsigned int chan_id);
void scm_set_power_on_log(void);
u32 scm_ind_get_dst_buff_size(void);
void scm_send_cnf_data_to_udi(u8 *virt_addr, u8 *phy_addr, u32 len);
void scm_send_ind_data_to_udi(u8 *virt_addr, u8 *phy_addr, u32 len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
