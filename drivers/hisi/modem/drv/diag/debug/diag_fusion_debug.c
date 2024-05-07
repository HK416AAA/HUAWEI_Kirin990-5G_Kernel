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
#include <osl_thread.h>
#include <osl_types.h>
#include <osl_malloc.h>
#include <osl_sem.h>
#include "soc_socp_adapter.h"
#include "msg_id.h"
#include "bsp_socp.h"
#include "bsp_diag_cmdid.h"
#include "ppm_common.h"
#include "mdrv_socp.h"
#include "diag_fusion_debug.h"
#include "diag_fusion_init.h"
#include "diag_fusion_cfg.h"
#include "diag_fusion_message.h"
#include "diag_fusion_conn.h"

#ifdef __cplusplus
extern "C" {
#endif

diag_mntn_info_s g_diag_debug_info;
u32 g_diag_usb_send_start_slice;
diag_report_exc_info_s g_diag_report_exc_info[DIAG_EXC_PTR_BUTT];
diag_ptr_info_s g_diag_ptr_info;
char *g_diag_ptr_name[] = {
    "DIAG_PTR_START",
    "DIAG_PTR_MESSAGE_RECV",
    "DIAG_PTR_APSS_MESSAGE_RECV",
    "DIAG_PTR_UPHY_MESSAGE_RECV",
    "DIAG_PTR_4G_FRAME_MSG",
    "DIAG_PTR_5G_FRAME_MSG",
    "DIAG_PTR_MESSAGE_PROC",
    "DIAG_PTR_CONNECT_IN",
    "DIAG_PTR_CONNECT_OK",
    "DIAG_PTR_DISCONNECT_IN",
    "DIAG_PTR_DISCONNECT_OK",
    "DIAG_PTR_MSP_MSG_IN",
    "DIAG_PTR_MSP_MSG_OK",
    "DIAG_PTR_MSP_MSG_ERR",
    "DIAG_PTR_BSP_MSG_IN",
    "DIAG_PTR_BSP_MSG_OK",
    "DIAG_PTR_BSP_MSG_ERR",
    "DIAG_PTR_PS_MSG_IN",
    "DIAG_PTR_PS_MSG_OK",
    "DIAG_PTR_PS_MSG_ERR",
    "DIAG_PTR_UPHY_MSG_IN",
    "DIAG_PTR_UPHY_MSG_OK",
    "DIAG_PTR_UPHY_MSG_ERR",
    "DIAG_PTR_RFPHY_MSG_IN",
    "DIAG_PTR_RFPHY_MSG_OK",
    "DIAG_PTR_RFPHY_MSG_ERR",
    "DIAG_PTR_BBPDS_MSG_IN",
    "DIAG_PTR_BBPDS_MSG_OK",
    "DIAG_PTR_BBPDS_MSG_ERR",
    "DIAG_PTR_TIMESTAMP_CFG_IN",
    "DIAG_PTR_TIMESTAMP_CFG_OK",
    "DIAG_PTR_TIMESTAMP_CFG_ERR",
    "DIAG_PTR_GET_MODEM_NUM_IN",
    "DIAG_PTR_GET_MODEM_NUM_OK",
    "DIAG_PTR_GET_MODEM_NUM_ERR",
    "DIAG_PTR_GET_PID_TBL_IN",
    "DIAG_PTR_GET_PID_TBL_OK",
    "DIAG_PTR_GET_PID_TBL_ERR",
    "DIAG_PTR_MNTN_CFG_IN",
    "DIAG_PTR_MNTN_CFG_OK",
    "DIAG_PTR_MNTN_CFG_ERR",

    "DIAG_PTR_FLOW_CFG_IN",
    "DIAG_PTR_FLOW_CFG_ERR",
    "DIAG_PTR_FLOW_CFG_OK",

    "DIAG_PTR_AIR_CFG_IN",
    "DIAG_PTR_AIR_CFG_OK",
    "DIAG_PTR_AIR_CFG_ERR",
    "DIAG_PTR_TRACE_CFG_IN",
    "DIAG_PTR_TRACE_CFG_OK",
    "DIAG_PTR_TRACE_CFG_ERR",
    "DIAG_PTR_EVENT_CFG_IN",
    "DIAG_PTR_EVENT_CFG_OK",
    "DIAG_PTR_EVENT_CFG_ERR",
    "DIAG_PTR_PRINT_CFG_IN",
    "DIAG_PTR_PRINT_CFG_OK",
    "DIAG_PTR_PRINT_CFG_ERR",
    "DIAG_PTR_TRANS_CFG_IN",
    "DIAG_PTR_TRANS_CFG_OK",
    "DIAG_PTR_TRANS_CFG_ERR",
    "DIAG_PTR_USER_PLANE_CFG_IN",
    "DIAG_PTR_USER_PLANE_CFG_OK",
    "DIAG_PTR_USER_PLANE_CFG_ERR",
    "DIAG_PTR_SRV_PACK_IN",
    "DIAG_PTR_SRV_PACK_OK",
    "DIAG_PTR_SRV_PACK_ERR",
};

void diag_reset_src_mntn_info(void)
{
    (void)memset_s(&g_diag_debug_info.ps_chan_info, sizeof(g_diag_debug_info.ps_chan_info), 0,
        sizeof(g_diag_debug_info.ps_chan_info));

    return;
}

void diag_reset_dst_mntn_info(void)
{
    (void)memset_s(&g_diag_debug_info.dst_chan_info, sizeof(g_diag_debug_info.dst_chan_info), 0,
        sizeof(g_diag_debug_info.dst_chan_info));

    return;
}

void diag_mntn_report(void *para)
{
    struct softtimer_list *timer = NULL;
    s32 ret;

    ret = diag_get_mntn_switch();
    if (ret == ERR_MSP_SUCCESS) {
        diag_ps_src_mntn_report();
        diag_dst_mntn_report();
    } else {
        diag_warning("mntn switch close, ret=0x%x", ret);
    }

    timer = diag_get_timer_node(DIAG_TIMER_MNTN);
    if (timer == NULL) {
        diag_error("get diag timer fail\n");
        return;
    }

    bsp_softtimer_add(timer);
}
EXPORT_SYMBOL(diag_mntn_report);

void diag_ps_src_mntn_report(void)
{
    diag_ps_mntn_info_s ps_mntn;
    u32 ret;
    static u32 last_slice = 0;
    u32 current_slice;

    ps_mntn.trans_head.ulModule = MDRV_DIAG_GEN_MODULE_EX(DIAG_MODEM_0, DIAG_MODE_COMM, DIAG_MSG_TYPE_BSP);
    ps_mntn.trans_head.ulPid = DRV_MID_DIAG_APSS;
    ps_mntn.trans_head.ulMsgId = DIAG_DEBUG_AP_SRC_MNTN_CMDID;
    ps_mntn.trans_head.ulReserve = 0;
    ps_mntn.trans_head.ulLength = sizeof(diag_ps_mntn_info_s) - sizeof(mdrv_diag_trans_ind_s);

    ps_mntn.mntn_head.chan_id = SOCP_CODER_SRC_ACPU_IND;
    memcpy_s(ps_mntn.mntn_head.chan_name, sizeof(ps_mntn.mntn_head.chan_name), "ap_ps_src", sizeof("ap_ps_src"));

    current_slice = bsp_get_slice_value();
    ps_mntn.mntn_head.used_size = 0;
    ps_mntn.mntn_head.delta_time = DIAG_SLICE_DELTA(last_slice, current_slice);

    ret = (u32)memcpy_s(&ps_mntn.mntn_data, sizeof(ps_mntn_info_s), &g_diag_debug_info.ps_chan_info,
        sizeof(g_diag_debug_info.ps_chan_info));
    if (ret) {
        diag_error("memcpy_s fail,ret = 0x%x\n", ret);
        return;
    }

    ps_mntn.mntn_data.through_put = (((ps_mntn.mntn_data.package_len - ps_mntn.mntn_data.abandon_len) / 1024) * 32768) /
        ps_mntn.mntn_head.delta_time;

    ps_mntn.trans_head.pData = (void *)(&ps_mntn.mntn_head);
    ret = diag_report_trans((mdrv_diag_trans_ind_s *)&ps_mntn);
    if (ret == 0) {
        last_slice = current_slice;
        (void)memset_s(&g_diag_debug_info.ps_chan_info, sizeof(g_diag_debug_info.ps_chan_info), 0,
            sizeof(g_diag_debug_info.ps_chan_info));
    }

    return;
}

void diag_dst_mntn_report(void)
{
    diag_dst_mntn_info_s dst_mntn;
    socp_buffer_rw_s socp_buffer;
    static u32 last_slice = 0;
    u32 current_slice;
    u32 ret;

    dst_mntn.trans_head.ulModule = MDRV_DIAG_GEN_MODULE_EX(DIAG_MODEM_0, DIAG_MODE_COMM, DIAG_MSG_TYPE_BSP);
    dst_mntn.trans_head.ulPid = DRV_MID_DIAG_APSS;
    dst_mntn.trans_head.ulMsgId = DIAG_DEBUG_DST_MNTN_CMDID;
    dst_mntn.trans_head.ulReserve = 0;
    dst_mntn.trans_head.ulLength = sizeof(diag_dst_mntn_info_s) - sizeof(mdrv_diag_trans_ind_s);

    dst_mntn.mntn_head.chan_id = SOCP_CODER_DST_OM_IND;
    memcpy_s(dst_mntn.mntn_head.chan_name, sizeof(dst_mntn.mntn_head.chan_name), "ind_dst", sizeof("ind_dst"));

    (void)bsp_socp_get_read_buff(SOCP_CODER_DST_OM_IND, &socp_buffer);
    dst_mntn.mntn_head.used_size = socp_buffer.size + socp_buffer.rb_size;
    current_slice = bsp_get_slice_value();
    dst_mntn.mntn_head.delta_time = DIAG_SLICE_DELTA(last_slice, current_slice);

    diag_mntn_get_ind_dst_int_info(&g_diag_debug_info.dst_chan_info.socp_trf_times,
        &g_diag_debug_info.dst_chan_info.socp_thr_ovf_times);

    ret = (u32)memcpy_s(&dst_mntn.mntn_data, sizeof(dst_mntn_info_s), &g_diag_debug_info.dst_chan_info,
        sizeof(g_diag_debug_info.dst_chan_info));
    if (ret) {
        diag_error("memcpy_s fail,ret = 0x%x\n", ret);
        return;
    }

    /* 实际场景中不会出现，此判断是避免UBSAN检查报错 */
    if (((dst_mntn.mntn_head.delta_time * 1000) / 32768) == 0) {
        dst_mntn.mntn_data.thrput_phy = 0;
        dst_mntn.mntn_data.thrput_cb = 0;
    } else {
        dst_mntn.mntn_data.thrput_phy = ((g_diag_debug_info.dst_chan_info.delta_socp_len / 1024) * 1000) /
            ((dst_mntn.mntn_head.delta_time * 1000) / 32768);
        dst_mntn.mntn_data.thrput_cb = ((g_diag_debug_info.dst_chan_info.delta_usb_free_len / 1024) * 1000) /
            ((dst_mntn.mntn_head.delta_time * 1000) / 32768);
    }

    dst_mntn.trans_head.pData = (void *)(&dst_mntn.mntn_head);
    ret = diag_report_trans((mdrv_diag_trans_ind_s *)&dst_mntn);
    if (ret == 0) {
        last_slice = current_slice;
        (void)memset_s(&g_diag_debug_info.dst_chan_info, sizeof(g_diag_debug_info.dst_chan_info), 0,
            sizeof(g_diag_debug_info.dst_chan_info));
    }

    return;
}

void diag_deubg_socp_len(u32 send_len, u32 free_len)
{
    g_diag_debug_info.dst_chan_info.delta_socp_len = send_len;
    g_diag_debug_info.dst_chan_info.delta_socp_len = free_len;
}
EXPORT_SYMBOL(diag_deubg_socp_len);

void diag_start_mntn_timer(u32 period)
{
    struct softtimer_list *timer = NULL;

    timer = diag_get_timer_node(DIAG_TIMER_MNTN);
    if (timer == NULL) {
        diag_error("get diag timer fail\n");
        return;
    }
    bsp_softtimer_modify(timer, period);
    bsp_softtimer_add(timer);
    return;
}

void diag_stop_mntn_timer(void)
{
    struct softtimer_list *timer = NULL;

    timer = diag_get_timer_node(DIAG_TIMER_MNTN);
    if (timer == NULL) {
        diag_error("get diag timer fail\n");
        return;
    }

    (void)bsp_softtimer_delete(timer);

    return;
}

s32 diag_connect_test(void)
{
    diag_conn_frame_info_s diag_msg;

    memset_s(&diag_msg.serv_head, sizeof(diag_msg.serv_head), 0x01010002, sizeof(int));
    diag_msg.serv_head.trans_id = 0x12345678;
    diag_msg.cmd_id = 0x10065000;
    diag_msg.msg_len = 0x8;

    return diag_dl_message_proc(&diag_msg);
}

s32 diag_auth_test(u32 cmd)
{
    diag_conn_frame_info_s *diag_msg;
    diag_get_mdm_info_req_s *auth_msg;
    s32 ret;

    auth_msg = (diag_get_mdm_info_req_s *)osl_malloc(sizeof(diag_get_mdm_info_req_s));
    auth_msg->auid = 0x5a5a5a5a;
    auth_msg->sn = 0xcdcdcdcd;
    auth_msg->cmd = cmd;

    diag_msg = (diag_conn_frame_info_s *)osl_malloc(sizeof(diag_conn_frame_info_s) + sizeof(diag_get_mdm_info_req_s));

    memset_s(&diag_msg->serv_head, sizeof(diag_msg->serv_head), 0x01010002, sizeof(int));
    diag_msg->serv_head.trans_id = 0x12345678;
    diag_msg->cmd_id = 0x10065000;
    diag_msg->msg_len = sizeof(diag_get_mdm_info_req_s);
    memcpy_s(diag_msg->data, diag_msg->msg_len, auth_msg, sizeof(diag_get_mdm_info_req_s));

    ret = diag_dl_message_proc(diag_msg);

    osl_free(auth_msg);
    osl_free(diag_msg);
    return ret;
}

s32 diag_disconnect_test(void)
{
    diag_conn_frame_info_s diag_msg;

    memset_s(&diag_msg.serv_head, sizeof(diag_msg.serv_head), 0x01010002, sizeof(int));
    diag_msg.serv_head.trans_id = 0x12345678;
    diag_msg.cmd_id = 0x10065001;
    diag_msg.msg_len = 0x8;

    return diag_dl_message_proc(&diag_msg);
}

void diag_show_mntn_cfg(void)
{
    diag_cfg_info_s *diag_cfg;

    diag_cfg = diag_get_cfg_info();
    diag_crit("mntn_switch = 0x%x, period=0x%x\n", diag_cfg->mntn_cfg.mntn_switch, diag_cfg->mntn_cfg.period);

    return;
}

void diag_show_air_cfg(void)
{
    diag_cfg_info_s *diag_cfg;

    diag_cfg = diag_get_cfg_info();
    if ((diag_cfg->diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_AIR)) ==
        (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_AIR)) {
        diag_crit("air_switch is open, state=0x%x\n", diag_cfg->diag_cfg_state);
    } else {
        diag_crit("air_switch is close, state=0x%x\n", diag_cfg->diag_cfg_state);
    }

    return;
}

void diag_show_trace_cfg(u32 mid)
{
    diag_cfg_info_s *diag_cfg;

    diag_cfg = diag_get_cfg_info();

    if (DIAG_CFG_PS_MODULE_IS_VALID(mid)) {
        diag_crit("src_id = 0x%x, src_switch=0x%x\n", mid, diag_cfg->trace_src_ps_module_cfg[mid - PS_MID_START]);
        diag_crit("dst_id = 0x%x, dst_switch=0x%x\n", mid, diag_cfg->trace_dst_ps_module_cfg[mid - PS_MID_START]);
    } else if (DIAG_CFG_DRV_MODULE_IS_VALID(mid)) {
        diag_crit("src_id = 0x%x, src_switch=0x%x\n", mid, diag_cfg->trace_src_drv_module_cfg[mid - DRV_MID_START]);
        diag_crit("dst_id = 0x%x, dst_switch=0x%x\n", mid, diag_cfg->trace_dst_drv_module_cfg[mid - DRV_MID_START]);
    } else {
        diag_error("mid invalid\n");
    }

    return;
}

void diag_show_event_cfg(u32 mid)
{
    diag_cfg_info_s *diag_cfg;

    diag_cfg = diag_get_cfg_info();
    if ((diag_cfg->diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_EVT)) ==
        (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_EVT)) {
        diag_crit("event_main_switch is open, state=0x%x\n", diag_cfg->diag_cfg_state);
    } else {
        diag_crit("event_main_switch is close, state=0x%x\n", diag_cfg->diag_cfg_state);
        return;
    }

    if (DIAG_CFG_PS_MODULE_IS_VALID(mid)) {
        diag_crit("mid = 0x%x, mid_switch=0x%x\n", mid, diag_cfg->event_ps_module_cfg[mid - PS_MID_START]);
    } else {
        diag_crit("mid invalid, mid=0x%x\n", mid);
    }
    return;
}

void diag_show_drv_print_cfg(void)
{
    diag_cfg_info_s *diag_cfg;

    diag_cfg = diag_get_cfg_info();

    diag_crit("drv_print_level=0x%x\n", diag_cfg->drv_print_cfg.drv_print_level);
    return;
}

void diag_show_print_cfg(u32 mid)
{
    u32 print_level;

    print_level = mdrv_diag_get_log_level(mid);

    diag_crit("print_level = 0x%x\n", print_level);
    return;
}

void diag_show_trans_cfg(void)
{
    diag_cfg_info_s *diag_cfg;

    diag_cfg = diag_get_cfg_info();
    if ((diag_cfg->diag_cfg_state & (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_TRANS)) ==
        (DIAG_CFG_INIT | DIAG_CFG_CONN | DIAG_CFG_TRANS)) {
        diag_crit("trans_switch is open, state=0x%x\n", diag_cfg->diag_cfg_state);
    } else {
        diag_crit("trans_switch is close, state=0x%x\n", diag_cfg->diag_cfg_state);
    }

    return;
}

char *diag_show_report_exc_record(diag_report_exc_e type)
{
    return (char *)&g_diag_report_exc_info[type];
}

void diag_show_ptr_record(u32 num)
{
    u32 cur_ptr;
    u32 event;
    u32 i;
    cur_ptr = (g_diag_ptr_info.cur_ptr - num + DIAG_PTR_ELEMENT_MAX) % DIAG_PTR_ELEMENT_MAX;

    for (i = 0; i < num; i++) {
        event = g_diag_ptr_info.diag_ptr_element[cur_ptr].ptr_type;
        if (event != DIAG_PTR_START) {
            diag_crit("%-32s 0x%08x 0x%08x %08d\n", g_diag_ptr_name[event],
                g_diag_ptr_info.diag_ptr_element[cur_ptr].para1, g_diag_ptr_info.diag_ptr_element[cur_ptr].para2,
                g_diag_ptr_info.diag_ptr_element[cur_ptr].slice);
        }

        cur_ptr = (cur_ptr + 1) % DIAG_PTR_ELEMENT_MAX;
    }
    diag_crit("i = %d, show diag ptr over\n", i);
    return;
}

#define GEN_MODULE_ID(modem_id, modeid, level, pid) (((modem_id) << 24) | ((modeid) << 16) | ((level) << 12) | (pid))

u32 diag_log_test(u32 mid)
{
    u32 module_id = MDRV_DIAG_GEN_LOG_MODULE(1, 2, PS_LOG_LEVEL_INFO);
    u32 ret;

    ret = mdrv_diag_log_report(module_id, mid, __FILE__, __LINE__, "DIAG APSS = 0x%x, DIAG TSP=0x%x.\n",
        DRV_MID_DIAG_APSS, DRV_MID_DIAG_TSP);
    if (ret) {
        diag_error("ps log report test fail, ret=0x%x\n", ret);
    }

    ret = mdrv_diag_log_report(module_id, 0x8003, __FILE__, __LINE__, "DIAG APSS = 0x%x, DIAG TSP=0x%x.\n",
        DRV_MID_DIAG_APSS, DRV_MID_DIAG_TSP);
    if (ret) {
        diag_error("drv log report test fail, ret=0x%x\n", ret);
    }

    return ret;
}

u32 diag_trans_test(u32 mid)
{
    mdrv_diag_trans_ind_s trans_msg;
    char data[80] = "trans_report_test";

    trans_msg.ulModule = MDRV_DIAG_GEN_MODULE(1, 2);
    trans_msg.ulPid = mid;
    trans_msg.ulMsgId = 0x9753;

    trans_msg.ulLength = 80;
    trans_msg.pData = data;

    return mdrv_diag_trans_report(&trans_msg);
}

u32 diag_event_test(u32 mid)
{
    mdrv_diag_event_ind_s enevt_msg = { 0 };
    char data[16] = "event";

    enevt_msg.ulModule = MDRV_DIAG_GEN_MODULE(1, 2);
    enevt_msg.ulPid = mid;
    enevt_msg.ulEventId = 0x13579;

    enevt_msg.ulLength = 16;
    enevt_msg.pData = data;

    return mdrv_diag_event_report(&enevt_msg);
}

u32 diag_air_test(u32 mid)
{
    mdrv_diag_air_ind_s airmsg;
    u32 data = 0x1234567;
    u32 module_id = GEN_MODULE_ID(0, 1, 1, mid);

    airmsg.ulPid = mid;
    airmsg.ulDirection = 0x3;
    airmsg.ulLength = sizeof(data);
    airmsg.ulModule = module_id;
    airmsg.ulMsgId = 0x123;

    airmsg.ulLength = sizeof(data);
    airmsg.pData = &data;

    return mdrv_diag_air_report(&airmsg);
}

u32 diag_trace_test(u32 src_mid, u32 dst_mid)
{
    diag_trace_msg_s *trace_msg;
    char *data = "trace_test";
    u32 ret;

    trace_msg = (diag_trace_msg_s *)osl_malloc(sizeof(diag_trace_msg_s) + strlen(data) + 1);
    trace_msg->timestamp = 0x1234;
    trace_msg->src_id = src_mid;
    trace_msg->dst_id = dst_mid;
    trace_msg->msg_id = 0xcacacaca;
    trace_msg->data_len = strlen(data) + 1;

    memcpy_s(trace_msg->data, strlen(data) + 1, data, strlen(data) + 1);

    ret = mdrv_diag_trace_report(trace_msg);
    if (ret) {
        diag_error("trace test fail, ret=0x%x\n", ret);
    }

    osl_free(trace_msg);

    return ret;
}
void diag_api_test(u32 num, u32 src_mid, u32 dst_mid)
{
    u32 i;

    for (i = 0; i < num; i++) {
        diag_trans_test(src_mid);
        diag_event_test(src_mid);
        diag_air_test(src_mid);
        diag_log_test(src_mid);
        diag_trace_test(src_mid, dst_mid);

        diag_warning("test num: %d\n", i + 1);
    }

    return;
}

/* DIAG FLOW TEST START */
diag_flow_info_s g_diag_flow_info;
void diag_start_flow_test(void *flow_req)
{
    diag_cmd_flow_cfg_s *flow_data = (diag_cmd_flow_cfg_s *)flow_req;

    g_diag_flow_info.flow_timer = diag_get_timer_node(DIAG_TIMER_FLOW);
    if (g_diag_flow_info.flow_timer == NULL) {
        return;
    }

    if (g_diag_flow_info.diag_flow_timer_flag) {
        bsp_softtimer_delete(g_diag_flow_info.flow_timer);
        g_diag_flow_info.diag_flow_timer_flag = 0;
    }

    g_diag_flow_info.diag_flow_cycle = flow_data->pack_num;
    g_diag_flow_info.diag_flow_swt = flow_data->tsp_en;

    g_diag_flow_info.flow_timer->timeout = flow_data->tsp_peroid;
    bsp_softtimer_modify(g_diag_flow_info.flow_timer, flow_data->tsp_peroid);
    bsp_softtimer_add(g_diag_flow_info.flow_timer);
    g_diag_flow_info.diag_flow_timer_flag = 1;
}

void diag_stop_flow_test(void)
{
    g_diag_flow_info.diag_flow_swt = 0;
}

void diag_flow_report(void *para)
{
    g_diag_flow_info.diag_flow_timer_flag = 0;

    diag_api_test(g_diag_flow_info.diag_flow_cycle, 0x1, DRV_MID_DIAG_TSP);

    if (g_diag_flow_info.diag_flow_swt) {
        bsp_softtimer_add(g_diag_flow_info.flow_timer);
        g_diag_flow_info.diag_flow_timer_flag = 1;
    }

    return;
}

#ifdef __cplusplus
}
#endif
