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

#include <securec.h>
#include "diag_fusion_conn.h"
#include "bsp_diag.h"
#include <mdrv_diag.h>
#include "mdrv_timer.h"
#include "diag_drv_msg.h"
#include "bsp_diag_cmdid.h"
#include "bsp_nvim.h"
#include "bsp_version.h"
#include "bsp_applog.h"
#include "diag_fusion_message.h"
#include "diag_fusion_cfg.h"
#include "diag_fusion_debug.h"
#include "osl_malloc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

u32 g_diag_auth_state = DIAG_AUTH_TYPE_DEFAULT;

#define DIAG_CONN_STATE_FUN_NUM 32
mdrv_diag_conn_state_notify_fun g_conn_state_notify_func[DIAG_CONN_STATE_FUN_NUM];

unsigned int mdrv_diag_conn_state_notify_fun_reg(mdrv_diag_conn_state_notify_fun func)
{
    u32 index;

    for (index = 0; index < sizeof(g_conn_state_notify_func) / sizeof(mdrv_diag_conn_state_notify_fun); index++) {
        if (g_conn_state_notify_func[index] == NULL) {
            g_conn_state_notify_func[index] = func;
            return ERR_MSP_SUCCESS;
        }
    }

    diag_error("no space to register new func\n");
    return ERR_MSP_NOT_FREEE_SPACE;
}

void diag_notify_conn_state(unsigned int state)
{
    u32 index;

    for (index = 0; index < sizeof(g_conn_state_notify_func) / sizeof(mdrv_diag_conn_state_notify_fun); index++) {
        if (g_conn_state_notify_func[index] != NULL) {
            g_conn_state_notify_func[index](state);
        }
    }
}

void diag_show_conn_state_notify_cb(void)
{
    u32 index;

    for (index = 0; index < sizeof(g_conn_state_notify_func) / sizeof(mdrv_diag_conn_state_notify_fun); index++) {
        if (g_conn_state_notify_func[index] != NULL) {
            diag_crit("%d %pS\n", index, g_conn_state_notify_func[index]);
        }
    }

    return;
}

/* 端口断开时回调该接口通知DIAG断开连接 */
u32 diag_notify_port_disconn(void)
{
    diag_conn_frame_info_s *diag_msg;
    u32 timestamp[2];
    s32 ret;
    u32 len = sizeof(diag_conn_frame_info_s) + sizeof(diag_data_head_s);

    diag_msg = (diag_conn_frame_info_s *)osl_malloc(len);
    if (diag_msg == NULL) {
        return ERR_MSP_MALLOC_FAILUE;
    }
    (void)memset_s(diag_msg, len, 0, len);

    diag_msg->serv_head.sid_8b = DIAG_FRAME_MSP_SID_DIAG_SERVICE;
    diag_msg->serv_head.mt_2b = DIAG_FRAME_MT_REQ;

    diag_msg->serv_head.trans_id = 0;

    (void)mdrv_timer_get_accuracy_timestamp(&timestamp[1], &timestamp[0]);
    ret = memcpy_s((u8 *)diag_msg->serv_head.timestamp, sizeof(diag_msg->serv_head.timestamp), (u8 *)timestamp,
        sizeof(diag_msg->serv_head.timestamp));
    if (ret) {
        diag_error("memcpy_s fail, ret=0x%x\n", ret);
    }

    diag_msg->cmd_id = DIAG_CMD_HOST_DISCONNECT;
    diag_msg->msg_len = sizeof(diag_data_head_s);

    ret = (u32)diag_dl_message_proc(diag_msg);
    osl_free(diag_msg);
    return ret;
}

void diag_disconn_manager(u8 *disconn_msg)
{
    diag_frame_head_s *diag_head = (diag_frame_head_s *)disconn_msg;
    const bsp_version_info_s *version_info = bsp_get_version_info();

    diag_crit("receive tool disconnect cmd\n");

    diag_ptr_record(DIAG_PTR_DISCONNECT_IN, diag_head->cmd_id, diag_head->msg_len);

    diag_conn_reset();

    bsp_applog_disconn();

    diag_set_auth_state(DIAG_AUTH_TYPE_DEFAULT);

    diag_cfg_reset();

    diag_stop_mntn_timer();

    diag_reset_src_mntn_info();
    diag_reset_dst_mntn_info();

    diag_notify_conn_state(DIAG_STATE_DISCONN);

    if (version_info == NULL) {
        diag_error("get version info is null\n");
        return;
    }
    if (version_info->plat_type == PLAT_EMU) {
        mdrv_socp_set_ind_mode(SOCP_IND_MODE_DIRECT);
        diag_crit("EMU diag disconnect: socp set mode direct\n");
    } else {
        mdrv_socp_send_data_manager(SOCP_CODER_DST_OM_IND, SOCP_DEST_DSM_DISABLE);
    }

    diag_ptr_record(DIAG_PTR_DISCONNECT_OK, diag_head->cmd_id, diag_head->msg_len);
    return;
}

void diag_conn_manager(u8 *conn_msg)
{
    diag_conn_frame_info_s *diag_head;
    const bsp_version_info_s *version_info = bsp_get_version_info();
    s32 ret;

    diag_head = (diag_conn_frame_info_s *)conn_msg;
    diag_ptr_record(DIAG_PTR_CONNECT_IN, diag_head->cmd_id, diag_head->msg_len);

    diag_crit("receive tool conn cmd\n");

    diag_conn_reset();
    ret = diag_set_global_switch(DIAG_CFG_CONN_BIT, DIAG_CFG_SWT_OPEN);
    if (ret) {
        diag_error("set global switch fail\n");
    }

    diag_reset_src_mntn_info();
    diag_reset_dst_mntn_info();

    bsp_applog_conn();

    diag_notify_conn_state(DIAG_STATE_CONN);

    if (version_info == NULL) {
        diag_error("get version info is null\n");
        return;
    }
    if (version_info->plat_type == PLAT_EMU) {
        mdrv_socp_set_ind_mode(SOCP_IND_MODE_CYCLE);
        diag_open_all_switch();
        diag_crit("EMU diag connect: socp set mode cycle\n");
    }

    diag_ptr_record(DIAG_PTR_CONNECT_OK, diag_head->cmd_id, diag_head->msg_len);
    return;
}

void diag_auth_proc(const u8 *auth_msg)
{
    g_diag_auth_state = *auth_msg;

    diag_ptr_record(DIAG_PTR_AUTH_IN, 1, 1);

    if (g_diag_auth_state == DIAG_AUTH_TYPE_SUCCESS) {
        diag_crit("diag auth success\n");
    } else {
        diag_error("diag auth fail, state=0x%x\n", g_diag_auth_state);
        diag_ptr_record(DIAG_PTR_AUTH_FAIL, 1, 1);
    }

    diag_ptr_record(DIAG_PTR_AUTH_OK, 1, 1);
    return;
}

u32 diag_get_auth_state(void)
{
    return g_diag_auth_state;
}

void diag_set_auth_state(u32 state)
{
    g_diag_auth_state = state;
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
