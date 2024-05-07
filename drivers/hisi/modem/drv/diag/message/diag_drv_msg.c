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

#include "diag_fusion_message.h"
#include "diag_drv_msg.h"
#include "diag_fusion_conn.h"
#include "bsp_diag.h"
#include "diag_fusion_debug.h"
#include "diag_fusion_cfg.h"
#include "bsp_diag_cmdid.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

diag_drv_msg_proc_s g_diag_msp_proc_table[] = {
    /* log switch */
    { DIAG_CMD_LOG_CAT_PRINT, diag_print_cfg_proc },
    { DIAG_CMD_LOG_CAT_TRACE, diag_trace_cfg_proc },
    { DIAG_CMD_LOG_CAT_AIR, diag_air_cfg_proc },
    { DIAG_CMD_LOG_CAT_EVENT, diag_event_cfg_proc },
    { DIAG_CMD_LOG_CAT_MSG, diag_msg_filter_proc },
    { DIAG_CMD_LOG_TRANS, diag_trans_cfg_proc },
    /* mntn and debug cmd */
    { DIAG_CMD_LOG_MNTN, diag_mntn_cfg_proc },
    { DIAG_CMD_DEBUG_MSG, NULL },
    { DIAG_CMD_FLOW_TEST, diag_flow_cfg_proc },
    /* drv print cfg */
    { DIAG_CMD_DRV_PRINT_CFG, diag_drv_print_cfg_proc },
    /* applog cfg */
    { DIAG_CMD_APPLOG_CONFIG, diag_applog_cfg_proc },
};

void diag_drv_msg_init(void)
{
    diag_msg_proc_cb_reg(DIAG_FRAME_MSG_TYPE_MSP, diag_msp_msg_proc);

    return;
}

void diag_msp_msg_proc(diag_frame_head_s *msg_data)
{
    u32 i;
    diag_comm_cnf_s cnf_info;
    diag_cnf_msg_s diag_msg;

    if (msg_data == NULL) {
        diag_error("msg data is NULL\n");
        return;
    }

    diag_ptr_record(DIAG_PTR_MSP_MSG_IN, msg_data->cmd_id, msg_data->msg_len);

    for (i = 0; i < sizeof(g_diag_msp_proc_table) / sizeof(g_diag_msp_proc_table[0]); i++) {
        if ((msg_data->cmd_id == g_diag_msp_proc_table[i].cmd_id) && (g_diag_msp_proc_table[i].msg_proc_cb != NULL)) {
            g_diag_msp_proc_table[i].msg_proc_cb((u8 *)msg_data);
            diag_ptr_record(DIAG_PTR_MSP_MSG_OK, msg_data->cmd_id, msg_data->msg_len);
            return;
        }
    }

    diag_ptr_record(DIAG_PTR_MSP_MSG_ERR, msg_data->cmd_id, msg_data->msg_len);

    cnf_info.result = ERR_MSP_DIAG_INVALID_CMD;
    diag_cnf_msg_fill(msg_data, &cnf_info, &diag_msg);
    diag_cnf_msg_report(&diag_msg);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
