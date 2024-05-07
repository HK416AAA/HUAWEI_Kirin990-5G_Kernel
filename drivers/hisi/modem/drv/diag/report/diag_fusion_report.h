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

#ifndef __DIAG_FUSION_REPORT_H__
#define __DIAG_FUSION_REPORT_H__

/*
 * 1 Include Headfile
 */
#include "mdrv_diag.h"
#include "mdrv_msg.h"
#include "osl_types.h"
#include "osl_spinlock.h"
#include "stdarg.h"
#include "bsp_diag.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push)
#pragma pack(4)

#define DIAG_PRINT_LEN 256
#define DIAG_PRINT_PARAM_MAX_NUM (6)

#define DIAG_GET_MODEM_ID(id) (id >> 24)
#define DIAG_GET_MODE_ID(id) ((id & 0x000F0000) >> 16)
#define DIAG_GET_PRINT_LEVEL(id) ((id & 0x0000F000) >> 12)
#define DIAG_GET_GROUP_ID(id) ((id & 0x00000F00) >> 8)
#define DIAG_GET_MODULE_ID(id) (id & 0x00000FFF)

typedef enum {
    DIAG_LOG_PRINT,
    DIAG_LOG_AIR,
    DIAG_LOG_VoLTE,
    DIAG_LOG_TRACE,
    DIAG_LOG_TRANS,
    DIAG_LOG_USER,
    DIAG_LOG_EVENT,
    DIAG_LOG_DT,
    DIAG_LOG_BUTT
} diag_log_type_e;

typedef struct {
    u32 num;
    spinlock_t lock;
} diag_log_pkt_num_s;

typedef struct {
    u32 ind_ps_trans_id;                            /* ind ps chan统一排列的trans id */
    u32 cnf_trans_id;                               /* cnf chan统一排列的trans id */
    diag_log_pkt_num_s diag_log_num[DIAG_LOG_BUTT]; /* 按照LOG类型排列的NO号 */
} diag_report_info_s;

typedef struct {
    u32 timestamp_low;
    u32 timestamp_high;
} diag_ind_high_ts_s;

typedef struct {
    mdrv_diag_trans_ind_s trans_info;
    diag_ind_high_ts_s high_ts;
} diag_timestamp_ind_s;

typedef struct {
    MDRV_MSG_HEAD
    u32 msg_id;
    char data[0]; /*lint !e43 */
} diag_trace_msg_s;

void diag_report_init(void);
void diag_report_reset(void);
s32 diag_send_data(diag_msg_report_head_s *data, diag_log_type_e log_type);
s32 diag_report_drv_log(u32 module_id, u32 mid, const char *fmt, va_list arg);
char *diag_get_file_name_from_path(char *file_name);
s32 diag_report_trans(mdrv_diag_trans_ind_s *trans_msg);
s32 diag_report_trace(diag_trace_msg_s *trace_msg, u32 modem_id);
s32 diag_report_event(mdrv_diag_event_ind_s *event_msg);
s32 diag_report_air(mdrv_diag_air_ind_s *air_msg);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of diag_report.h */
