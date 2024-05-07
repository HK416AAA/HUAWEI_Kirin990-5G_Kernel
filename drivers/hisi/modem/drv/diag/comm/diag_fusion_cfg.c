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

/*
 * 1 Include HeadFile
 */
#include <securec.h>
#include "osl_malloc.h"
#include "bsp_diag.h"
#include "bsp_applog.h"
#include "bsp_version.h"
#include "diag_fusion_cfg.h"
#include "diag_fusion_debug.h"
#include "diag_fusion_message.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

diag_cfg_info_s g_diag_cfg_info;

void diag_conn_reset(void)
{
    g_diag_cfg_info.diag_cfg_state = DIAG_CFG_INIT;
}

void diag_open_all_switch(void)
{
    u32 index;

    g_diag_cfg_info.diag_cfg_state = DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_AIR | DIAG_CFG_EVT | DIAG_CFG_TRANS;

    g_diag_cfg_info.print_main_cfg = ((1 << (7 - PS_LOG_LEVEL_ERROR)) | (1 << (7 - PS_LOG_LEVEL_WARNING)) |
        (1 << (7 - PS_LOG_LEVEL_NORMAL)) | (1 << (7 - PS_LOG_LEVEL_INFO)));

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.event_ps_module_cfg) / sizeof(g_diag_cfg_info.event_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.event_ps_module_cfg[index] = DIAG_CFG_SWT_OPEN;
    }

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.trace_src_ps_module_cfg) / sizeof(g_diag_cfg_info.trace_src_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_src_ps_module_cfg[index] = DIAG_CFG_SWT_OPEN;
    }

    for (index = 0; index <
        (sizeof(g_diag_cfg_info.trace_src_drv_module_cfg) / sizeof(g_diag_cfg_info.trace_src_drv_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_src_drv_module_cfg[index] = DIAG_CFG_SWT_OPEN;
    }

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.trace_dst_ps_module_cfg) / sizeof(g_diag_cfg_info.trace_dst_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_dst_ps_module_cfg[index] = DIAG_CFG_SWT_OPEN;
    }

    for (index = 0; index <
        (sizeof(g_diag_cfg_info.trace_dst_drv_module_cfg) / sizeof(g_diag_cfg_info.trace_dst_drv_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_dst_drv_module_cfg[index] = DIAG_CFG_SWT_OPEN;
    }

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.print_ps_module_cfg) / sizeof(g_diag_cfg_info.print_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.print_ps_module_cfg[index] = DIAG_CFG_SWT_OPEN;
    }

    return;
}

void diag_cfg_reset(void)
{
    u32 index;
    errno_t err;

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.event_ps_module_cfg) / sizeof(g_diag_cfg_info.event_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.event_ps_module_cfg[index] = DIAG_CFG_SWT_OPEN;
    }

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.trace_src_ps_module_cfg) / sizeof(g_diag_cfg_info.trace_src_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_src_ps_module_cfg[index] = DIAG_CFG_SWT_CLOSE;
    }

    for (index = 0; index <
        (sizeof(g_diag_cfg_info.trace_src_drv_module_cfg) / sizeof(g_diag_cfg_info.trace_src_drv_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_src_drv_module_cfg[index] = DIAG_CFG_SWT_CLOSE;
    }

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.trace_dst_ps_module_cfg) / sizeof(g_diag_cfg_info.trace_dst_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_dst_ps_module_cfg[index] = DIAG_CFG_SWT_CLOSE;
    }

    for (index = 0; index <
        (sizeof(g_diag_cfg_info.trace_dst_drv_module_cfg) / sizeof(g_diag_cfg_info.trace_dst_drv_module_cfg[0]));
        index++) {
        g_diag_cfg_info.trace_dst_drv_module_cfg[index] = DIAG_CFG_SWT_CLOSE;
    }

    for (index = 0;
        index < (sizeof(g_diag_cfg_info.print_ps_module_cfg) / sizeof(g_diag_cfg_info.print_ps_module_cfg[0]));
        index++) {
        g_diag_cfg_info.print_ps_module_cfg[index] = DIAG_CFG_SWT_CLOSE;
    }

    g_diag_cfg_info.print_main_cfg = DIAG_CFG_PRINT_TOTAL_MODULE_SWT_NOT_USE;

    err = memset_s(&g_diag_cfg_info.filter_msg_cfg, sizeof(g_diag_cfg_info.filter_msg_cfg), 0,
        sizeof(g_diag_cfg_info.filter_msg_cfg));
    if (err) {
        diag_error("memset fail,ret=0x%x\n", err);
    }

    diag_report_reset();
}

s32 diag_set_global_switch(u32 switch_bit, u8 state)
{
    if (state == DIAG_CFG_SWT_OPEN) {
        g_diag_cfg_info.diag_cfg_state |= ((u32)1 << switch_bit);
    } else if (state == DIAG_CFG_SWT_CLOSE) {
        g_diag_cfg_info.diag_cfg_state &= ~((u32)1 << switch_bit);
    } else {
        return ERR_MSP_INVALID_PARAMETER;
    }
    return ERR_MSP_SUCCESS;
}

void diag_flow_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *flow_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_flow_cfg_s *flow_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;

    diag_ptr_record(DIAG_PTR_FLOW_CFG_IN, flow_cfg->cmd_id, flow_cfg->msg_len);

    flow_req = (diag_cmd_flow_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);

    if (flow_req->apss_en) {
        diag_start_flow_test(flow_req);
    } else {
        diag_stop_flow_test();
    }

    diag_cnf_msg_fill(flow_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }

    diag_ptr_record(DIAG_PTR_FLOW_CFG_OK, flow_req->apss_en, flow_req->pack_num);

    return;
}

void diag_mntn_cfg_proc(u8 *cmd_cfg)
{
    diag_frame_head_s *mntn_cfg = (diag_frame_head_s *)cmd_cfg;
    diag_cmd_mntn_cfg_s *mntn_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;
    diag_ptr_record(DIAG_PTR_MNTN_CFG_IN, mntn_cfg->cmd_id, mntn_cfg->msg_len);
    mntn_req = (diag_cmd_mntn_cfg_s *)(cmd_cfg + DIAG_HEAD_LEN);
    g_diag_cfg_info.mntn_cfg.mntn_switch = mntn_req->mntn_switch;
    if (mntn_req->mntn_switch == DIAG_CFG_SWT_OPEN) {
        g_diag_cfg_info.mntn_cfg.period = mntn_req->period;
        diag_start_mntn_timer(mntn_req->period);
    } else {
        diag_stop_mntn_timer();
    }

    diag_cnf_msg_fill(mntn_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }

    diag_ptr_record(DIAG_PTR_MNTN_CFG_OK, g_diag_cfg_info.mntn_cfg.mntn_switch, g_diag_cfg_info.mntn_cfg.period);
    return;
}

void diag_air_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *air_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_air_cfg_s *air_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    u32 air_switch;
    s32 ret;
    diag_ptr_record(DIAG_PTR_AIR_CFG_IN, air_cfg->cmd_id, air_cfg->msg_len);
    air_req = (diag_cmd_air_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    air_switch = DIAG_GET_CFG_SWT(air_req->air_switch);
    cnf_msg.result = diag_set_global_switch(DIAG_CFG_AIR_BIT, DIAG_GET_CFG_SWT(air_req->air_switch));

    diag_cnf_msg_fill((diag_frame_head_s *)cmd_msg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }
    /* 为兼容工具旧版本丢开机LOG问题，待工具发布一段时间后删除 */
    mdrv_socp_send_data_manager(SOCP_CODER_DST_OM_IND, SOCP_DEST_DSM_ENABLE);

    diag_ptr_record(DIAG_PTR_AIR_CFG_OK, air_cfg->cmd_id, air_cfg->msg_len);
    return;
}

void diag_trace_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *trace_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_trace_cfg_s *trace_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;
    diag_ptr_record(DIAG_PTR_TRACE_CFG_IN, trace_cfg->cmd_id, trace_cfg->msg_len);
    trace_req = (diag_cmd_trace_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    diag_set_trace_switch(trace_req, (trace_cfg->msg_len - sizeof(diag_data_head_s)));

    cnf_msg.result = ERR_MSP_SUCCESS;
    diag_cnf_msg_fill(trace_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }

    diag_ptr_record(DIAG_PTR_TRACE_CFG_OK, trace_cfg->cmd_id, trace_cfg->msg_len);
    return;
}

void diag_set_trace_switch(diag_cmd_trace_cfg_s *trace_req, u32 msg_len)
{
    diag_cmd_trace_cfg_s *cmd_offset = NULL;
    u32 mid_offset;
    u32 i;

    for (i = 0; i < msg_len / sizeof(diag_cmd_trace_cfg_s); i++) {
        cmd_offset = trace_req + i;

        if (cmd_offset->module_type == 1) { /* dst moudle */
            if (DIAG_CFG_PS_MODULE_IS_VALID(cmd_offset->mid)) {
                mid_offset = cmd_offset->mid - PS_MID_START;
                g_diag_cfg_info.trace_dst_ps_module_cfg[mid_offset] = DIAG_GET_CFG_SWT(cmd_offset->trace_switch);
            } else if (DIAG_CFG_DRV_MODULE_IS_VALID(cmd_offset->mid)) {
                mid_offset = cmd_offset->mid - DRV_MID_START;
                g_diag_cfg_info.trace_dst_drv_module_cfg[mid_offset] = DIAG_GET_CFG_SWT(cmd_offset->trace_switch);
            }
        } else { /* src moudle */
            if (DIAG_CFG_PS_MODULE_IS_VALID(cmd_offset->mid)) {
                mid_offset = cmd_offset->mid - PS_MID_START;
                g_diag_cfg_info.trace_src_ps_module_cfg[mid_offset] = DIAG_GET_CFG_SWT(cmd_offset->trace_switch);
            } else if (DIAG_CFG_DRV_MODULE_IS_VALID(cmd_offset->mid)) {
                mid_offset = cmd_offset->mid - DRV_MID_START;
                g_diag_cfg_info.trace_src_drv_module_cfg[mid_offset] = DIAG_GET_CFG_SWT(cmd_offset->trace_switch);
            }
        }
    }

    return;
}

void diag_event_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *event_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_event_cfg_s *event_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;

    diag_ptr_record(DIAG_PTR_EVENT_CFG_IN, event_cfg->cmd_id, event_cfg->msg_len);

    event_req = (diag_cmd_event_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    diag_set_event_switch(event_req);

    cnf_msg.result = ERR_MSP_SUCCESS;
    diag_cnf_msg_fill(event_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }

    diag_ptr_record(DIAG_PTR_EVENT_CFG_OK, event_cfg->cmd_id, event_cfg->msg_len);
    return;
}

void diag_set_event_switch(diag_cmd_event_cfg_s *event_req)
{
    u32 real_mid;
    u32 mid_offset;
    u32 i;
    s32 ret;

    ret = diag_set_global_switch(DIAG_CFG_EVENT_BIT, event_req->event_switch);
    if (ret) {
        diag_error("get event switch fail, ret=0x%x\n", ret);
    }
    if (event_req->event_switch) {
        for (i = 0; i < event_req->count; i++) {
            real_mid = DIAG_EVENT_MID(event_req->mid[i]);
            if (DIAG_CFG_PS_MODULE_IS_VALID(real_mid)) {
                mid_offset = real_mid - PS_MID_START;
                g_diag_cfg_info.event_ps_module_cfg[mid_offset] = DIAG_EVENT_SWT(event_req->mid[i]);
            }
        }
    }
}

void diag_msg_filter_proc(u8 *cmd_msg)
{
    diag_frame_head_s *filter_cfg = (diag_frame_head_s *)cmd_msg;
    diag_msg_filter_cfg_s *filter_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;

    filter_req = (diag_msg_filter_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    diag_set_msg_filter_switch(filter_req, (filter_cfg->msg_len - sizeof(diag_data_head_s)));

    cnf_msg.result = ERR_MSP_SUCCESS;
    diag_cnf_msg_fill(filter_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }

    return;
}

s32 diag_set_msg_filter_switch(diag_msg_filter_cfg_s *filter_req, u32 msg_len)
{
    return 0;
}

void diag_trans_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *trans_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_trans_cfg_s *trans_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;

    diag_ptr_record(DIAG_PTR_TRANS_CFG_IN, trans_cfg->cmd_id, trans_cfg->msg_len);

    trans_req = (diag_cmd_trans_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    cnf_msg.result = diag_set_global_switch(DIAG_CFG_TRANS_BIT, trans_req->trans_switch);

    diag_cnf_msg_fill(trans_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }
    diag_ptr_record(DIAG_PTR_TRANS_CFG_OK, trans_cfg->cmd_id, trans_cfg->msg_len);
    return;
}

void diag_applog_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *applog_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_applog_cfg_s *applog_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;

    diag_ptr_record(DIAG_PTR_APPLOG_CFG_IN, applog_cfg->cmd_id, applog_cfg->msg_len);

    applog_req = (diag_cmd_applog_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    cnf_msg.result = bsp_applog_cfg(applog_req->enable, applog_req->level);

    diag_cnf_msg_fill(applog_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }
    diag_ptr_record(DIAG_PTR_APPLOG_CFG_OK, applog_cfg->cmd_id, applog_cfg->msg_len);
    return;
}


void diag_drv_print_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *print_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_drv_print_cfg_s *print_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;

    print_req = (diag_cmd_drv_print_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    g_diag_cfg_info.drv_print_cfg.drv_print_level = print_req->drv_print_level;

    cnf_msg.result = ERR_MSP_SUCCESS;
    diag_cnf_msg_fill(print_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }

    return;
}

void diag_print_cfg_proc(u8 *cmd_msg)
{
    diag_frame_head_s *print_cfg = (diag_frame_head_s *)cmd_msg;
    diag_cmd_print_cfg_s *print_req = NULL;
    diag_cnf_msg_s diag_msg;
    diag_comm_cnf_s cnf_msg;
    s32 ret;
    diag_ptr_record(DIAG_PTR_PRINT_CFG_IN, print_cfg->cmd_id, print_cfg->msg_len);

    print_req = (diag_cmd_print_cfg_s *)(cmd_msg + DIAG_HEAD_LEN);
    diag_set_print_switch(print_req, (print_cfg->msg_len - sizeof(diag_data_head_s)));

    cnf_msg.result = ERR_MSP_SUCCESS;
    diag_cnf_msg_fill(print_cfg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("diag cfg msg cnf fail, ret=0x%x\n", ret);
    }

    diag_ptr_record(DIAG_PTR_PRINT_CFG_OK, print_cfg->cmd_id, print_cfg->msg_len);
    return;
}

void diag_set_print_switch(diag_cmd_print_cfg_s *print_req, u32 msg_len)
{
    u32 i;
    u32 mid_offset;
    s32 ret;
    if (print_req->mid == DIAG_CFG_PRINT_TOTAL_MODULE) {
        ret = memset_s(g_diag_cfg_info.print_ps_module_cfg, sizeof(g_diag_cfg_info.print_ps_module_cfg), 0,
            sizeof(g_diag_cfg_info.print_ps_module_cfg));
        if (ret) {
            diag_error("clear print_module_cfg fail, ret=0x%x\n", ret);
        }

        g_diag_cfg_info.print_main_cfg = DIAG_PRINT_CFG_MAIN_SWT(print_req->level_filter);
    } else {
        g_diag_cfg_info.print_main_cfg = DIAG_CFG_PRINT_TOTAL_MODULE_SWT_NOT_USE;

        for (i = 0; i < msg_len / sizeof(diag_cmd_print_cfg_s); i++) {
            if (DIAG_CFG_PS_MODULE_IS_VALID((print_req + i)->mid)) {
                mid_offset = (print_req + i)->mid - PS_MID_START;
                g_diag_cfg_info.print_ps_module_cfg[mid_offset] =
                    DIAG_PRINT_CFG_MAIN_SWT((print_req + i)->level_filter);
            }
        }
    }

    return;
}

void diag_set_poweronlog_profile(u32 level)
{
    g_diag_cfg_info.power_on_log_profile = level;
    return;
}

s32 diag_get_mntn_switch(void)
{
    if (!DIAG_IS_POLOG_ON) {
        if (!DIAG_IS_CONN_ON) {
            return ERR_MSP_NO_INITILIZATION;
        }
    }

    return ERR_MSP_SUCCESS;
}

s32 diag_get_drv_print_switch(u32 module_id)
{
    if (!DIAG_IS_POLOG_ON) {
        if (!DIAG_IS_CONN_ON) {
            return ERR_MSP_NO_INITILIZATION;
        }

        if ((g_diag_cfg_info.drv_print_cfg.drv_print_level == PS_LOG_LEVEL_OFF) ||
            (g_diag_cfg_info.drv_print_cfg.drv_print_level < DIAG_GET_PRINT_LEVEL(module_id))) {
            return ERR_MSP_CFG_LOG_NOT_ALLOW;
        }

        return ERR_MSP_SUCCESS;
    } else {
        return diag_get_print_power_on_log_cfg(DIAG_GET_PRINT_LEVEL(module_id));
    }
}

/*
 * Function Name: diag_get_print_level
 * Description: 工具的LEVEL值转换成MSP本地存储的LEVEL值
 * TOOL ->             MSP ->     PS
 * 0x40000000对应0x40  对应 1        (ERROR);
 * 0x20000000对应0x20  对应 2        (WARNING);
 * 0x10000000对应0x10  对应 3        (NORMAL);
 * 0x08000000对应0x08  对应 4        (INFO)
 */
u32 diag_get_print_level(u32 mid, u32 level)
{
    u32 level_filter;

    if ((level == PS_LOG_LEVEL_OFF) || (level >= PS_LOG_LEVEL_BUTT)) {
        return ERR_MSP_CFG_LOG_NOT_ALLOW;
    }

    level_filter = 1 << (7 - level);
    if (g_diag_cfg_info.print_main_cfg != DIAG_CFG_PRINT_TOTAL_MODULE_SWT_NOT_USE) {
        if (level_filter & g_diag_cfg_info.print_main_cfg) {
            return ERR_MSP_SUCCESS;
        }
    }

    if (DIAG_CFG_PS_MODULE_IS_VALID(mid)) {
        if (level_filter & g_diag_cfg_info.print_ps_module_cfg[mid - MSG_PS_MID_START]) {
            return ERR_MSP_SUCCESS;
        }
    }

    return ERR_MSP_CFG_LOG_NOT_ALLOW;
}

s32 diag_get_log_switch(u32 module_id, u32 mid)
{
    if (!DIAG_IS_POLOG_ON) {
        if (!DIAG_IS_CONN_ON) {
            return ERR_MSP_NO_INITILIZATION;
        }
        return diag_get_print_level(mid, DIAG_GET_PRINT_LEVEL(module_id));
    } else {
        return diag_get_print_power_on_log_cfg(DIAG_GET_PRINT_LEVEL(module_id));
    }
}

s32 diag_get_print_power_on_log_cfg(u32 level)
{
    if (((g_diag_cfg_info.power_on_log_profile == DIAGLOG_POWER_LOG_PROFILE_SIMPLE) && (level <= PS_LOG_LEVEL_ERROR)) ||
        ((g_diag_cfg_info.power_on_log_profile == DIAGLOG_POWER_LOG_PROFILE_NORAML) &&
        (level <= PS_LOG_LEVEL_NORMAL)) ||
        ((g_diag_cfg_info.power_on_log_profile == DIAGLOG_POWER_LOG_PROFILE_WHOLE) && (level <= PS_LOG_LEVEL_INFO))) {
        return ERR_MSP_SUCCESS;
    } else {
        return ERR_MSP_CFG_LOG_NOT_ALLOW;
    }
}

s32 diag_get_trans_switch(void)
{
    if (!DIAG_IS_POLOG_ON) {
        if (!DIAG_IS_TRANS_ON) {
            return ERR_MSP_CFG_LOG_NOT_ALLOW;
        }
    } else {
        if (g_diag_cfg_info.power_on_log_profile <= DIAGLOG_POWER_LOG_PROFILE_SIMPLE) {
            return ERR_MSP_CFG_LOG_NOT_ALLOW;
        }
    }

    return ERR_MSP_SUCCESS;
}

s32 diag_get_trace_switch(diag_trace_msg_s *trans_msg)
{
    s32 ret;

    if (!DIAG_IS_POLOG_ON) {
        /* 检查DIAG是否初始化且HSO是否连接上 */
        if (!DIAG_IS_CONN_ON) {
            return ERR_MSP_NO_INITILIZATION;
        }

        /* 检查是否允许层间消息上报 */
        ret = diag_get_trace_cfg(trans_msg->src_id, trans_msg->dst_id, trans_msg->msg_id);
        if (ret) {
            return ret;
        }
    }

    return ERR_MSP_SUCCESS;
}

/*
 * Function Name: diag_get_trace_cfg
 * Description: 获取层间消息开关状态
 */
s32 diag_get_trace_cfg(u32 src_module, u32 dst_module, u32 msg_id)
{
    u32 src_switch = 1;
    u32 dst_switch = 1;
    /*lint -save -e568 -e685 -e661*/
    if (DIAG_CFG_PS_MODULE_IS_VALID(src_module)) {
        src_switch = g_diag_cfg_info.trace_src_ps_module_cfg[src_module - PS_MID_START];
    } else if (DIAG_CFG_DRV_MODULE_IS_VALID(src_module)) {
        src_switch = g_diag_cfg_info.trace_src_drv_module_cfg[src_module - DRV_MID_START];
    }

    if (DIAG_CFG_PS_MODULE_IS_VALID(dst_module)) {
        src_switch = g_diag_cfg_info.trace_dst_ps_module_cfg[dst_module - PS_MID_START];
    } else if (DIAG_CFG_DRV_MODULE_IS_VALID(dst_module)) {
        src_switch = g_diag_cfg_info.trace_dst_drv_module_cfg[dst_module - DRV_MID_START];
    }

    if (src_switch || dst_switch) {
        return ERR_MSP_SUCCESS;
    }

    diag_error("src_id=0x%x, dst_id=0x%x\n", src_module, dst_module);
    return ERR_MSP_CFG_LOG_NOT_ALLOW;
    /*lint -restore +e568 +e685 +e661*/
}

/*
 * Function Name: diag_get_msg_filter_cfg
 * Description: 获取层间消息过滤开关
 * Input: VOS_UINT32 ulCatId
 *        VOS_UINT32 ulMsgId
 */
s32 diag_get_msg_filter_cfg(u32 msg_id)
{
    filter_info_s *item_cfg = NULL;
    u32 i;

    for (i = 0; i < g_diag_cfg_info.filter_msg_cfg.cfg_cnt; i++) {
        item_cfg = (g_diag_cfg_info.filter_msg_cfg.msg_filter_list + i);

        if (item_cfg->id == msg_id) {
            if (item_cfg->filter_switch == DIAG_CFG_SWT_CLOSE) {
                return ERR_MSP_CFG_LOG_NOT_ALLOW;
            } else if (item_cfg->filter_switch == DIAG_CFG_SWT_OPEN) {
                return ERR_MSP_SUCCESS;
            } else {
                return ERR_MSP_DIAG_MSG_CFG_NOT_SET;
            }
        }
    }

    return ERR_MSP_DIAG_MSG_CFG_NOT_SET;
}

s32 diag_get_event_switch(mdrv_diag_event_ind_s *event_data)
{
    if (!DIAG_IS_POLOG_ON) {
        if (!DIAG_IS_EVENT_ON) {
            return ERR_MSP_NO_INITILIZATION;
        }

        if (DIAG_CFG_PS_MODULE_IS_VALID(event_data->ulPid)) {
            if (g_diag_cfg_info.event_ps_module_cfg[event_data->ulPid - PS_MID_START] == DIAG_CFG_SWT_OPEN) {
                return ERR_MSP_SUCCESS;
            }
        }
        return ERR_MSP_CFG_LOG_NOT_ALLOW;
    }

    return ERR_MSP_SUCCESS;
}

diag_cfg_info_s *diag_get_cfg_info(void)
{
    return &g_diag_cfg_info;
}

void diag_poweron_log_state_send(void)
{
    diag_msg_info_s *diag_msg = NULL;
    u32 poweron_log_state, len;
    s32 ret;

    diag_error("recv poweronlog state query\n");

    len = sizeof(diag_msg_info_s) + sizeof(poweron_log_state);
    diag_msg = (diag_msg_info_s *)osl_malloc(len);
    if (diag_msg == NULL) {
        diag_error("osl_malloc fail\n");
        return;
    }

    diag_msg->msg_id = DIAG_MSG_POWERON_LOG_QUERY;
    if (mdrv_diag_get_conn_state() == DIAG_STATE_POWERON_LOG) {
        poweron_log_state = BSP_TRUE;
    } else {
        poweron_log_state = BSP_FALSE;
    }

    ret = memcpy_s(diag_msg->data, sizeof(u32), &poweron_log_state, sizeof(u32));
    if (ret) {
        diag_error("memcpy_s fail, ret=0x%x\n", ret);
        osl_free(diag_msg);
        return;
    }

    (void)diag_msg_send(diag_msg, len, DRV_MID_DIAG_APSS, DRV_MID_DIAG_TSP);
    osl_free(diag_msg);
}

/*
 * 函 数 名: mdrv_diag_allow_air_msg_report
 * 功能描述: 提供给协议栈,用于检测当前是否允许上报AirMsg
 * 输入参数: VOS_VOID
 * 返回值: ERR_MSP_SUCCESS = 0 当前允许上报空口消息
 *        ERR_MSP_CFG_LOG_NOT_ALLOW = 203 当前不允许上报空口消息
 */
u32 mdrv_diag_allow_air_msg_report(void)
{
    if (!DIAG_IS_CONN_ON) {
        return ERR_MSP_CFG_LOG_NOT_ALLOW;
    } else {
        return ERR_MSP_SUCCESS;
    }
}

/*
 * 函 数 名: mdrv_diag_get_log_level
 * 功能描述: 提供给协议栈,用于查询当前PRINT LOG上报级别
 * 输入参数: mid:查询的mid
 * 返回值:   mdrv_diag_log_level_e 打印级别
 */
u32 mdrv_diag_get_log_level(u32 mid)
{
    u8 level;

    if ((mid >= PS_MID_START) && (mid < OSA_SYS_MID_BUTT)) {
        if (g_diag_cfg_info.print_main_cfg == DIAG_CFG_PRINT_TOTAL_MODULE_SWT_NOT_USE) {
            level = g_diag_cfg_info.print_ps_module_cfg[mid - PS_MID_START];
        } else {
            level = (u8)g_diag_cfg_info.print_main_cfg;
        }

        /* level中存储的值(0|ERROR|WARNING|NORMAL|INFO|0|0|0) bit 6-3 分别表示ERROR-INFO */
        if (level & 0x08) {
            return PS_LOG_LEVEL_INFO;
        } else if (level & 0x10) {
            return PS_LOG_LEVEL_NORMAL;
        } else if (level & 0x20) {
            return PS_LOG_LEVEL_WARNING;
        } else if (level & 0x40) {
            return PS_LOG_LEVEL_ERROR;
        } else {
            return PS_LOG_LEVEL_OFF;
        }
    }

    return PS_LOG_LEVEL_OFF;
}

unsigned int mdrv_diag_get_conn_state(void)
{
    if (DIAG_IS_POLOG_ON) {
        return DIAG_STATE_POWERON_LOG;
    } else if (DIAG_IS_CONN_ON) {
        return DIAG_STATE_CONN;
    } else {
        return DIAG_STATE_DISCONN;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
