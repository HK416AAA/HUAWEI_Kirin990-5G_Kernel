/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#ifndef _APPLOG_H
#define _APPLOG_H
#endif

#include <securec.h>
#include <linux/dma-mapping.h>
#include "osl_types.h"
#include "osl_sem.h"
#include "osl_spinlock.h"
#include "osl_thread.h"
#include <product_config.h>
#include "acore_nv_stru_drv.h"
#include "mdrv_socp.h"
#include "bsp_diag.h"
#include <bsp_socp.h>
#include "soc_socp_adapter.h"
#include "bsp_print.h"

#undef THIS_MODU
#define THIS_MODU mod_applog

#define APPLOG_OK 0
#define APPLOG_ERR -1
#define APPLOG_NULL (void *)0
#define APPLOG_SRC_BUF_LEN 0x10000
#define APPLOG_CHN_INIT 1   /* 通道已经分配 */
#define APPLOG_CHN_UNINIT 0 /* 通道没有分配 */
#define APPLOG_MAX_USER_BUFF_LEN 511
#define APPLOG_MAX_FILENAME_LINENO_LEN 9
#define APPLOG_MAX_HIDS_BUFF_LEN 520
#define APPLOG_HEAD_LEN 2

#define applog_crit(fmt, ...) printk(KERN_ERR "[%s]:" fmt, BSP_MOD(THIS_MODU), ##__VA_ARGS__)
#define applog_error(fmt, ...) \
    printk(KERN_ERR "[%s]:<%s %d>" fmt, BSP_MOD(THIS_MODU), __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define SOCP_CODER_SRC_APPLOG_IND SOCP_CODER_SRC_ACPU_USER_MODE_APP_IND

typedef struct {
    u8 level;
    u8 data[APPLOG_MAX_USER_BUFF_LEN];
} applog_data_s;


typedef struct {
    diag_socp_head_s socp_head;
    diag_frame_head_s diag_head;
    diag_print_ind_head_s print_head;
    u8 data[APPLOG_MAX_HIDS_BUFF_LEN];
} applog_send_info_s;

typedef struct {
    socp_data_type_e data_type;
    socp_data_type_en_e data_type_en;
    socp_enc_debug_en_e eEncDebugEn;
    socp_encsrc_chnmode_e mode;
    socp_chan_priority_e priority;
    u32 bypass_en;
    u32 SrcCh;
    u32 DstCh;
    u32 u32SrcBufLen;
} applog_src_cfg_s;

struct applog_ctrl_info {
    u32 u32ApplogSwitchOnOff;
    u32 u32ApplogInvalidParameter;
    u32 u32ApplogLevel;
    u32 u32ApplogLength;
    u32 u32ApplogReport;
    u32 u32ApplogSendData;
};

applog_src_cfg_s g_astAppLogCoderSrcCfg = { SOCP_DATA_TYPE_0,
    SOCP_DATA_TYPE_EN,
    SOCP_ENC_DEBUG_DIS,
    SOCP_ENCSRC_CHNMODE_CTSPACKET,
    SOCP_CHAN_PRIORITY_2,
    0,
    SOCP_CODER_SRC_APPLOG_IND,
    SOCP_CODER_DST_OM_IND,
    0x40000 };

s32 applog_src_chan_cfg(void);
s32 applog_send_data(u8 *data, u32 len);
s32 applog_report(applog_send_info_s *data, u32 len, u8 level);
void applog_fill_socp_head(diag_socp_head_s *socp_packet, u32 len);
void applog_diag_frame_head_init(diag_frame_head_s *diag_frame);
