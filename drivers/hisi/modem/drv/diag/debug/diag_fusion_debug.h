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

#ifndef __DIAG_SYSTEM_DEBUG_H__
#define __DIAG_SYSTEM_DEBUG_H__

/*
 * 1 Include Headfile
 */
#include <osl_types.h>
#include <osl_sem.h>
#include "mdrv_diag.h"
#include "mdrv_print.h"
#include "bsp_dump.h"
#include "bsp_print.h"
#include "bsp_slice.h"
#include "bsp_socp.h"
#include "scm_fusion_chan_manager.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push)
#pragma pack(4)

#undef THIS_MODU
#define THIS_MODU mod_diag

#define diag_crit(fmt, ...) bsp_err(fmt, ##__VA_ARGS__)
#define diag_error(fmt, ...) bsp_err("<%s %d>" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define diag_warning(fmt, ...) bsp_wrn("<%s %d>" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define DIAG_SLICE_DELTA(begin, end) (((end) > (begin)) ? ((end) - (begin)) : ((0xFFFFFFFF - (begin)) + (end)))
#define DIAG_REPORT_EXC_RECORD_MAX 64
#define DIAG_PTR_ELEMENT_MAX 512

#define DIAG_PS_SRC_80_OVERFLOW_THR (SCM_CODER_SRC_IND_BUFFER_SIZE >> 3) /* 87.5%上溢阈值 */

typedef enum {
    DIAG_EXC_PTR_PHY_PRINT_REPORT,
    DIAG_EXC_PTR_PHY_PRINT_HASH,
    DIAG_EXC_PTR_PHY_MSG_REPORT,
    DIAG_EXC_TRANS_REPORT,
    DIAG_EXC_TRACE_REPORT,
    DIAG_EXC_EVENT_REPORT,
    DIAG_EXC_AIR_REPORT,
    DIAG_EXC_VOLTE_REPORT,
    DIAG_EXC_DT_REPORT,
    DIAG_EXC_USER_REPORT,
    DIAG_EXC_LOG_REPORT,
    DIAG_EXC_LOG_HASH,
    DIAG_EXC_PTR_BUTT
} diag_report_exc_e;

typedef enum {
    DIAG_PTR_START,
    DIAG_PTR_PPM_READ,
    DIAG_PTR_PPM_READ_ERR,
    DIAG_PTR_CPM_RECV,
    DIAG_PTR_CPM_ERR,
    DIAG_PTR_SCM_SOFT_DECODE,
    DIAG_PTR_SCM_SELF_TASK,
    DIAG_PTR_SCM_RCV,
    DIAG_PTR_SCM_ERR,
    DIAG_PTR_SOFT_DEC_OK,
    DIAG_PTR_SCM_DISPATCH,
    DIAG_PTR_MSP_SERVICE_IN,
    DIAG_PTR_MSP_SERVICE_OK,
    DIAG_PTR_MSP_SERVICE_ERR,
    DIAG_PTR_SERVICE_IN,
    DIAG_PTR_SERVICE_OK,
    DIAG_PTR_SERVICE_WAIT,
    DIAG_PTR_DIAG_SERVICE_ERR,
    DIAG_PTR_DL_MESSAGE_IN,
    DIAG_PTR_DL_MESSAGE_OK,
    DIAG_PTR_DL_MESSAGE_ERR,

    DIAG_PTR_MESSAGE_RECV,
    DIAG_PTR_TSP_MESSAGE_RECV,
    DIAG_PTR_4G_FRAME_MSG,
    DIAG_PTR_5G_FRAME_MSG,
    DIAG_PTR_MESSAGE_PROC,
    DIAG_PTR_CONNECT_IN,
    DIAG_PTR_CONNECT_OK,
    DIAG_PTR_CONNECT_ERR,
    DIAG_PTR_DISCONNECT_IN,
    DIAG_PTR_DISCONNECT_OK,

    DIAG_PTR_AUTH_IN,
    DIAG_PTR_AUTH_OK,
    DIAG_PTR_AUTH_FAIL,
    DIAG_PTR_MSP_MSG_IN,
    DIAG_PTR_MSP_MSG_OK,
    DIAG_PTR_MSP_MSG_ERR,

    DIAG_PTR_BSP_MSG_IN,
    DIAG_PTR_BSP_MSG_OK,
    DIAG_PTR_BSP_MSG_ERR,
    DIAG_PTR_MNTN_CFG_IN,
    DIAG_PTR_MNTN_CFG_OK,
    DIAG_PTR_MNTN_CFG_ERR,

    DIAG_PTR_FLOW_CFG_IN,
    DIAG_PTR_FLOW_CFG_ERR,
    DIAG_PTR_FLOW_CFG_OK,

    DIAG_PTR_AIR_CFG_IN,
    DIAG_PTR_AIR_CFG_OK,
    DIAG_PTR_AIR_CFG_ERR,
    DIAG_PTR_TRACE_CFG_IN,
    DIAG_PTR_TRACE_CFG_OK,
    DIAG_PTR_TRACE_CFG_ERR,
    DIAG_PTR_EVENT_CFG_IN,
    DIAG_PTR_EVENT_CFG_OK,
    DIAG_PTR_EVENT_CFG_ERR,
    DIAG_PTR_PRINT_CFG_IN,
    DIAG_PTR_PRINT_CFG_OK,
    DIAG_PTR_PRINT_CFG_ERR,
    DIAG_PTR_TRANS_CFG_IN,
    DIAG_PTR_TRANS_CFG_OK,
    DIAG_PTR_TRANS_CFG_ERR,
    DIAG_PTR_APPLOG_CFG_IN,
    DIAG_PTR_APPLOG_CFG_OK,
    DIAG_PTR_APPLOG_CFG_ERR,
    DIAG_PTR_SRV_PACK_IN,
    DIAG_PTR_SRV_PACK_OK,
    DIAG_PTR_SRV_PACK_ERR,

    DIAG_PTR_BUTT
} diag_ptr_e;

typedef enum {
    SOCP_DST_LOST_BRANCH,
    SOCP_DST_LOST_CPM_WR,
    SOCP_DST_LOST_CPM_CB,
    SOCP_DST_LOST_BUTT
} diag_socp_dst_lost_ind_e;

typedef struct {
    u32 chan_id;
    char chan_name[32];
    u32 used_size;
    u32 delta_time;
} diag_mntn_head_s;

typedef struct {
    u32 package_len;
    u32 abandon_len;
    u32 through_put;
    u32 overflow_80_num;
    u32 trace_num;
    u32 trace_fail_num;
    u32 event_num;
    u32 event_fail_num;
    u32 log_num;
    u32 log_fail_num;
    u32 air_num;
    u32 air_fail_num;
    u32 trans_num;
    u32 trans_fail_num;
} ps_mntn_info_s;

typedef struct {
    mdrv_diag_trans_ind_s trans_head;
    diag_mntn_head_s mntn_head;
    ps_mntn_info_s mntn_data;
} diag_ps_mntn_info_s;

typedef struct {
    unsigned int delta_lost_times;
    unsigned int delta_lost_len;
    unsigned int type_fail_num[SOCP_DST_LOST_BUTT];
    unsigned int type_fail_len[SOCP_DST_LOST_BUTT];
    unsigned int delta_socp_len;
    unsigned int delta_usb_len;
    unsigned int delta_usb_ok_len;
    unsigned int delta_usb_free_len;
    unsigned int delta_usb_fail_len;
    unsigned int delat_vcom_len;
    unsigned int delat_vcom_fail_len;
    unsigned int delat_vcom_succ_len;
    unsigned int delat_socket_len;
    unsigned int delat_socket_succ_len;
    unsigned int delat_socket_fail_len;
    unsigned int delat_socp_int_to_port_time;
    unsigned int delta_usb_send_time;
    unsigned int delta_vcom_send_time;
    unsigned int delta_socket_send_time;
    unsigned int thrput_phy;
    unsigned int thrput_cb;
    unsigned int socp_trf_times;
    unsigned int socp_thr_ovf_times;
    mdrv_ppm_usb_debug_info_s ppm_usb_info;
    unsigned int tool_reserved[12];
} dst_mntn_info_s;

typedef struct {
    mdrv_diag_trans_ind_s trans_head;
    diag_mntn_head_s mntn_head;
    dst_mntn_info_s mntn_data;
} diag_dst_mntn_info_s;

typedef struct {
} ind_dst_info_s;

typedef struct {
    ps_mntn_info_s ps_chan_info;
    dst_mntn_info_s dst_chan_info;
} diag_mntn_info_s;

typedef struct {
    u32 diag_flow_cycle;
    u32 diag_flow_swt;
    u32 diag_flow_timer_flag;
    struct softtimer_list *flow_timer;
} diag_flow_info_s;

typedef struct {
    u32 type;
    u32 para1;
    u32 para2;
    u32 result;
    u32 slice;
} diag_exc_s;

typedef struct {
    u32 cur_ptr;
    diag_exc_s diag_exc_element[DIAG_REPORT_EXC_RECORD_MAX];
} diag_report_exc_info_s;

typedef struct {
    diag_ptr_e ptr_type;
    u32 para1;
    u32 para2;
    u32 slice;
} diag_ptr_s;

typedef struct {
    u32 cur_ptr;
    diag_ptr_s diag_ptr_element[DIAG_PTR_ELEMENT_MAX];
} diag_ptr_info_s;

void diag_start_flow_test(void *flow_req);
void diag_stop_flow_test(void);
void diag_flow_report(void *para);
void diag_reset_src_mntn_info(void);
void diag_reset_dst_mntn_info(void);
void diag_mntn_report(void *para);
void diag_ps_src_mntn_report(void);
void diag_dst_mntn_report(void);
void diag_start_mntn_timer(u32 period);
void diag_stop_mntn_timer(void);

extern diag_mntn_info_s g_diag_debug_info;
extern u32 g_diag_usb_send_start_slice;
extern diag_report_exc_info_s g_diag_report_exc_info[DIAG_EXC_PTR_BUTT];
extern diag_ptr_info_s g_diag_ptr_info;

static inline void diag_system_debug_ind_dst_lost(u32 type, u32 len)
{
    g_diag_debug_info.dst_chan_info.delta_lost_times++;
    g_diag_debug_info.dst_chan_info.delta_lost_len += len;
    g_diag_debug_info.dst_chan_info.type_fail_num[type]++;
    g_diag_debug_info.dst_chan_info.type_fail_len[type] += len;
}

static inline void diag_system_debug_rev_socp_data(u32 len)
{
    g_diag_debug_info.dst_chan_info.delta_socp_len += len;
}

static inline void diag_system_debug_usb_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delta_usb_len += len;
}

static inline void diag_system_debug_usb_ok_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delta_usb_ok_len += len;
}

static inline void diag_system_debug_usb_free_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delta_usb_free_len += len;
}

static inline void diag_system_debug_usb_fail_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delta_usb_fail_len += len;
}

static inline void diag_system_debug_vcom_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delat_vcom_len += len;
}

static inline void diag_system_debug_vcom_fail_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delat_vcom_fail_len += len;
}

static inline void diag_system_debug_vcom_sucess_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delat_vcom_succ_len += len;
}

static inline void diag_system_debug_socket_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delat_socket_len += len;
}

static inline void diag_system_debug_socket_sucess_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delat_socket_succ_len += len;
}

static inline void diag_system_debug_socket_fail_len(u32 len)
{
    g_diag_debug_info.dst_chan_info.delat_socket_fail_len += len;
}

static inline void diag_system_debug_send_data_end(void)
{
    u32 curent_slice;
    u32 start_slice;
    u32 delta;

    curent_slice = bsp_get_slice_value();
    start_slice = bsp_get_socp_ind_dst_int_slice();
    delta = get_timer_slice_delta(start_slice, curent_slice);

    g_diag_debug_info.dst_chan_info.delat_socp_int_to_port_time += delta;
}

static inline void diag_system_debug_send_usb_end(void)
{
    u32 curent_slice;
    u32 start_slice;
    u32 delta;

    curent_slice = bsp_get_slice_value();
    start_slice = g_diag_usb_send_start_slice;

    delta = get_timer_slice_delta(start_slice, curent_slice);
    g_diag_debug_info.dst_chan_info.delta_usb_send_time += delta;
}

static inline void diag_system_debug_send_usb_start(void)
{
    g_diag_usb_send_start_slice = bsp_get_slice_value();
}

static inline void diag_ps_package_record(u32 pack_len)
{
    g_diag_debug_info.ps_chan_info.package_len += pack_len;
}

static inline void diag_ps_ind_lost_record(u32 data_len)
{
    g_diag_debug_info.ps_chan_info.abandon_len += data_len;
}

static inline void diag_ps_buffer_overflow_record(u32 pack_len)
{
    if (pack_len < DIAG_PS_SRC_80_OVERFLOW_THR) {
        g_diag_debug_info.ps_chan_info.overflow_80_num++;
    }
}

static inline void diag_mntn_air_record(u32 result)
{
    g_diag_debug_info.ps_chan_info.air_num++;
    if (result != ERR_MSP_SUCCESS) {
        g_diag_debug_info.ps_chan_info.air_fail_num++;
    }

    return;
}

static inline void diag_mntn_trace_record(u32 result)
{
    g_diag_debug_info.ps_chan_info.trace_num++;
    if (result != ERR_MSP_SUCCESS) {
        g_diag_debug_info.ps_chan_info.trace_fail_num++;
    }

    return;
}

static inline void diag_mntn_event_record(u32 result)
{
    g_diag_debug_info.ps_chan_info.event_num++;
    if (result != ERR_MSP_SUCCESS) {
        g_diag_debug_info.ps_chan_info.event_fail_num++;
    }

    return;
}

static inline void diag_mntn_log_record(u32 result)
{
    g_diag_debug_info.ps_chan_info.log_num++;
    if (result != ERR_MSP_SUCCESS) {
        g_diag_debug_info.ps_chan_info.log_fail_num++;
    }

    return;
}

static inline void diag_mntn_trans_record(u32 result)
{
    g_diag_debug_info.ps_chan_info.trans_num++;
    if (result != ERR_MSP_SUCCESS) {
        g_diag_debug_info.ps_chan_info.trans_fail_num++;
    }

    return;
}
static inline void diag_report_exc_record(diag_report_exc_e type, u32 para1, u32 para2, u32 ret)
{
    u32 i = g_diag_report_exc_info[type].cur_ptr;

    g_diag_report_exc_info[type].diag_exc_element[i].type = type;
    g_diag_report_exc_info[type].diag_exc_element[i].para1 = para1;
    g_diag_report_exc_info[type].diag_exc_element[i].para2 = para2;
    g_diag_report_exc_info[type].diag_exc_element[i].result = ret;
    g_diag_report_exc_info[type].diag_exc_element[i].slice = bsp_get_slice_value();
    g_diag_report_exc_info[type].cur_ptr = (g_diag_report_exc_info[type].cur_ptr + 1) % DIAG_REPORT_EXC_RECORD_MAX;
}

static inline void diag_ptr_record(diag_ptr_e type, u32 para1, u32 para2)
{
    u32 i = g_diag_ptr_info.cur_ptr;

    g_diag_ptr_info.diag_ptr_element[i].ptr_type = type;
    g_diag_ptr_info.diag_ptr_element[i].para1 = para1;
    g_diag_ptr_info.diag_ptr_element[i].para2 = para2;
    g_diag_ptr_info.diag_ptr_element[i].slice = bsp_get_slice_value();
    g_diag_ptr_info.cur_ptr = (g_diag_ptr_info.cur_ptr + 1) % DIAG_PTR_ELEMENT_MAX;
    return;
}

/* 兼容非融合架构打点接口，后续归一后删除 */
static inline void diag_ptr(u32 type, u32 para_mark, u32 para1, u32 para2)
{
    diag_ptr_record(type, para1, para2);
}

static inline void diag_throughput_save(u32 chan, u32 bytes)
{
    return;
}

static inline void diag_mntn_get_ind_dst_int_info(u32 *trf_times, u32 *trf_overflow_times)
{
    bsp_socp_mntn_enc_dst_int_info(trf_times, trf_overflow_times);
}

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of diag_Debug.h */
