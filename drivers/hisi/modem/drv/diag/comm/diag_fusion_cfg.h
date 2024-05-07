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

#ifndef __DIAG_FUSION_CFG_H__
#define __DIAG_FUSION_CFG_H__

#include "osl_types.h"
#include "msg_id.h"
#include "mdrv_diag.h"
#include "diag_fusion_report.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(4)

#define DIAG_CFG_INIT (0x00000001)  /* DIAG已初始化 */
#define DIAG_CFG_CONN (0x00000002)  /* HSO已连接 */
#define DIAG_CFG_AIR (0x00000004)   /* 空口消息已打开 */
#define DIAG_CFG_EVT (0x00000008)   /* EVENT开关已打开 */
#define DIAG_CFG_TRANS (0x00000010) /* TRANS开关已打开 */
#define DIAG_CFG_USER (0x00000020)  /* USER_PLANE开关已打开 */

#define DIAG_CFG_POWERONLOG (0x00010000) /* 开机log开关已打开 */

#define DIAG_CFG_CAT_CFG_NUM (50)

#define DIAG_CMD_CFG_SWT_CLOSE (0x00000000)
#define DIAG_CMD_CFG_SWT_OPEN (0x80000000)

#define DIAG_CFG_PS_MID_NUM (OSA_SYS_MID_BUTT - PS_MID_START)
#define DIAG_CFG_PS_MODULE_IS_VALID(mid) ((mid < OSA_SYS_MID_BUTT) && (mid > PS_MID_START))
#define DIAG_CFG_PS_MODULE_IS_INVALID(mid) (mid >= OSA_SYS_MID_BUTT)

#define DIAG_CFG_DRV_MID_NUM (DRV_MID_END - DRV_MID_START)
#define DIAG_CFG_DRV_MODULE_IS_VALID(mid) ((mid < DRV_MID_END) && (mid > DRV_MID_START))
#define DIAG_CFG_DRV_MODULE_IS_INVALID(mid) (mid >= DRV_MID_END)

#define DIAG_CFG_PHY_MID_NUM (PHY_MID_END - PHY_MID_START)
#define DIAG_CFG_PHY_MODULE_IS_VALID(mid) ((mid < PHY_MID_END) && (mid > PHY_MID_START))
#define DIAG_CFG_PHY_MODULE_IS_INVALID(mid) (mid >= PHY_MID_END)
#define DIAG_CFG_MODULE_IS_INVALID(mid)         ((DIAG_CFG_PS_MODULE_IS_INVALID(mid)) && \
                                                 (DIAG_CFG_DRV_MODULE_IS_INVALID(mid)) && \
                                                 (DIAG_CFG_PHY_MODULE_IS_INVALID(mid))

#define DIAG_CFG_PRINT_TOTAL_MODULE_SWT_NOT_USE (0xFFFFFFFF)
#define DIAG_CFG_PRINT_TOTAL_MODULE (0xFFFFFFFF)

#define DIAG_CFG_FILTER_NUM_MAX (50)

/* diag初始化成功且HSO连接上 */
#define DIAG_IS_CONN_ON \
    ((g_diag_cfg_info.diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN)) == (DIAG_CFG_INIT | DIAG_CFG_CONN))
/* 允许LT 空口上报 */
#define DIAG_IS_AIR_ON                                                                    \
    ((g_diag_cfg_info.diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_AIR)) == \
        (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_AIR))
/* 允许事件上报 */
#define DIAG_IS_EVENT_ON                                                                  \
    ((g_diag_cfg_info.diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_EVT)) == \
        (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_EVT))
/* 允许透传Trans上报 */
#define DIAG_IS_TRANS_ON                                                                    \
    ((g_diag_cfg_info.diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_TRANS)) == \
        (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_TRANS))
/* 允许用户面user_plane上报 */
#define DIAG_IS_USER_ON                                                                    \
    ((g_diag_cfg_info.diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_USER)) == \
        (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_USER))
/* 允许开机(PowerOn)log上报 */
#define DIAG_IS_POLOG_ON \
    ((g_diag_cfg_info.diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_POWERONLOG)) == (DIAG_CFG_INIT | DIAG_CFG_POWERONLOG))

#define DIAG_GET_CFG_SWT(swt) ((DIAG_CMD_CFG_SWT_OPEN == swt) ? DIAG_CFG_SWT_OPEN : DIAG_CFG_SWT_CLOSE)

#define DIAG_EVENT_MID(value) (value & 0x7fffffff)
#define DIAG_EVENT_SWT(value) (value >> 31)
#define DIAG_PRINT_CFG_MAIN_SWT(value) (value >> 24)

typedef enum {
    DIAG_CFG_INIT_BIT = 0,
    DIAG_CFG_CONN_BIT,
    DIAG_CFG_AIR_BIT,
    DIAG_CFG_EVENT_BIT,
    DIAG_CFG_TRANS_BIT,
    DIAG_CFG_USER_PLANE_BIT,
    DIAG_CFG_POWERON_LOG_BIT = 0x10,
    DIAG_CFG_BIT_BUTT
} diag_cfg_bit_e;

typedef enum {
    DIAG_CFG_SWT_CLOSE = 0,
    DIAG_CFG_SWT_OPEN,
    DIAG_CFG_SWT_BUTT
} diag_cfg_switch_e;

typedef struct {
    u32 id; // 消息ID
    u32 filter_switch;
} filter_info_s;

typedef struct {
    u32 cat_id;
    u32 cfg_cnt;
    filter_info_s msg_filter_list[DIAG_CFG_FILTER_NUM_MAX];
} diag_log_filter_msg_s;

typedef struct {
    u16 mntn_switch;
    u16 period;
} diag_cmd_mntn_cfg_s;

typedef struct {
    u32 apss_peroid;
    u32 tsp_peroid;
    u32 apss_en;
    u32 tsp_en;
    u32 pack_num;
} diag_cmd_flow_cfg_s;

typedef struct {
    u32 air_switch; /* (0x00000000): 关；(0x80000000)：开 */
} diag_cmd_air_cfg_s;

typedef struct {
    u32 mid;
    u32 module_type;
    u32 trace_switch;
} diag_cmd_trace_cfg_s;

typedef struct {
    u32 event_switch;
    u32 count;
    u32 mid[0]; /* lint !e43 */
} diag_cmd_event_cfg_s;

typedef struct {
    u32 msg_id;
    u32 category;
    u32 filter_switch;
} diag_msg_filter_cfg_s;

typedef struct {
    u32 mid; /* mid = 0xffffffff表示所有模块的level开关 */
    u32 level_filter;
} diag_cmd_print_cfg_s;

typedef struct {
    u32 trans_switch;
} diag_cmd_trans_cfg_s;

typedef struct {
    u32 user_switch;
} diag_cmd_user_plane_cfg_s;

typedef struct {
    u32 enable;
    u32 level;
    u8 resv[8];
} diag_cmd_applog_cfg_s;

typedef struct {
    u32 drv_print_level;
} diag_cmd_drv_print_cfg_s;

typedef struct {
    u32 diag_cfg_state;
    u32 trace_src_ps_module_cfg[DIAG_CFG_PS_MID_NUM];
    u32 trace_src_drv_module_cfg[DIAG_CFG_DRV_MID_NUM];
    u32 trace_dst_ps_module_cfg[DIAG_CFG_PS_MID_NUM];
    u32 trace_dst_drv_module_cfg[DIAG_CFG_DRV_MID_NUM];
    u32 event_ps_module_cfg[DIAG_CFG_PS_MID_NUM];
    u32 print_ps_module_cfg[DIAG_CFG_PS_MID_NUM];
    u32 print_main_cfg;
    u32 power_on_log_profile;
    diag_log_filter_msg_s filter_msg_cfg;
    diag_cmd_mntn_cfg_s mntn_cfg;
    diag_cmd_drv_print_cfg_s drv_print_cfg;
} diag_cfg_info_s;

void diag_conn_reset(void);
void diag_open_all_switch(void);
void diag_cfg_reset(void);
s32 diag_set_global_switch(u32 switch_bit, u8 state);
void diag_mntn_cfg_proc(u8 *cmd_cfg);
void diag_flow_cfg_proc(u8 *cmd_msg);
void diag_air_cfg_proc(u8 *cmd_msg);
void diag_trace_cfg_proc(u8 *cmd_msg);
void diag_set_trace_switch(diag_cmd_trace_cfg_s *trace_req, u32 msg_len);
void diag_event_cfg_proc(u8 *cmd_msg);
void diag_set_event_switch(diag_cmd_event_cfg_s *event_req);
void diag_msg_filter_proc(u8 *cmd_msg);
s32 diag_set_msg_filter_switch(diag_msg_filter_cfg_s *filter_req, u32 msg_len);
void diag_drv_print_cfg_proc(u8 *cmd_msg);
void diag_print_cfg_proc(u8 *cmd_msg);
void diag_set_print_switch(diag_cmd_print_cfg_s *print_req, u32 msg_len);
void diag_set_poweronlog_profile(u32 level);
void diag_trans_cfg_proc(u8 *cmd_msg);
void diag_applog_cfg_proc(u8 *cmd_msg);
s32 diag_get_mntn_switch(void);
u32 diag_get_print_level(u32 mid, u32 level);
s32 diag_get_log_switch(u32 module_id, u32 mid);
s32 diag_get_print_power_on_log_cfg(u32 level);
s32 diag_get_drv_print_switch(u32 module_id);
s32 diag_get_trans_switch(void);
s32 diag_get_trace_switch(diag_trace_msg_s *trans_msg);
s32 diag_get_trace_cfg(u32 src_module, u32 dst_module, u32 msg_id);
s32 diag_get_msg_filter_cfg(u32 msg_id);
s32 diag_get_event_switch(mdrv_diag_event_ind_s *event_data);
diag_cfg_info_s *diag_get_cfg_info(void);
void diag_poweron_log_state_send(void);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
