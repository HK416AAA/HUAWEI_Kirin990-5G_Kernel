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

#ifndef __DIAG_FUSION_CONN_H__
#define __DIAG_FUSION_CONN_H__

#include "bsp_diag.h"
#include "mdrv_diag.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(4)

#define DIAG_GET_MODEM_INFO_BIT (1 << 1)
#define DIAG_VERIFY_SIGN_BIT (1 << 2)
#define DIAG_RECEIVE_READY_BIT (1 << 3)

typedef struct {
    u32 sid_8b : 8;
    u32 mdmid_3b : 3;
    u32 rsv_1b : 1;
    u32 ssid_4b : 4;
    u32 sessionid_8b : 8;
    u32 mt_2b : 2;
    u32 index_4b : 4;
    u32 eof_1b : 1;
    u32 ff_1b : 1;

    u32 trans_id;

    u32 timestamp[2];
} diag_conn_service_head_s;

typedef struct {
    /* 010: OM通道融合的版本        */
    /* 110: OM融合GU未融合的版本    */
    /* 100: OM完全融合的版本        */
    u32 drx_ctrl_flag : 1; /* DRX部分 */
    u32 port_flag : 1;     /* OM Port flag 0:old,1:new */
    u32 om_unify_flag : 1; /* OM融合 */
    u32 diag_5G_frame : 1;
    u32 auth_flag : 1; /* 是否进行安全校验 */
    u32 reserved : 27;
} diag_ver_flag_s;

#pragma pack(1)
/* 建连成功时的回复，工具根据该结构解析，不能随意修改顺序 */
typedef struct {
    u32 auid; /* 原AUID */
    u32 sn;   /* HSO分发，插件命令管理 */

    u32 result; /* 结果码 */
    u32 reserved0[7];
    char eng_imei[16]; /* for report IMEI for eng version */
    u32 chip_baseaddr;
    union {
        u32 unit_value;
        diag_ver_flag_s ver_flag;
    } diag_cfg; /* B135新增，标示低功耗特性版本:
                   1:低功耗版本；0：正常版本；0xFFFFFFFF:MSP读取NV项失败，HSO会认为连接不成功并给出提示，要求重新进行连接
                 */
    u32 lpd_mode;
    u8 reserved1[264];
    char product_info[64];
    char time_stamp[8];
    u32 cpu_freq;
    u32 usb_type;
} diag_conn_cnf_info_s;
#pragma pack()

typedef struct {
    diag_conn_service_head_s serv_head;
    u32 cmd_id;
    u32 msg_len;
    u8 data[0];
} diag_conn_frame_info_s;

/* 获取单板信息 */
typedef struct {
    u32 auid;
    u32 sn;
    u32 cmd; /* 命令:区分查询版本号、鉴权和连接 */
} diag_get_mdm_info_req_s;

u32 diag_notify_port_disconn(void);
void diag_notify_conn_state(unsigned int state);
void diag_show_conn_state_notify_cb(void);
s32 diag_get_imei(char imei[16]);
void diag_disconn_manager(u8 *disconn_msg);
void diag_conn_manager(u8 *conn_msg);
u32 diag_get_auth_state(void);
void diag_set_auth_state(u32 state);
void diag_auth_proc(const u8 *auth_msg);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
