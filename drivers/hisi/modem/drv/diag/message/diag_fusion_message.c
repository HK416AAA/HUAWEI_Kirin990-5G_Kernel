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

#include "securec.h"
#include "osl_thread.h"
#include "osl_malloc.h"
#include "bsp_slice.h"
#include "bsp_diag_cmdid.h"
#include "diag_fusion_init.h"
#include "diag_fusion_conn.h"
#include "diag_fusion_cfg.h"
#include "diag_fusion_debug.h"
#include "diag_drv_msg.h"
#include "diag_service.h"
#include "ppm_port_switch.h"
#include "ppm_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define DIAG_MESSAGE_TASK_PRIO 79 /* 与SOCP任务优先级相同 */

diag_message_state_s g_diag_message_state;
diag_message_proc_s g_diag_msg_fn_table[DIAG_FRAME_MSG_TYPE_BUTT] = {
    { DIAG_FRAME_MSG_TYPE_RSV, NULL },   { DIAG_FRAME_MSG_TYPE_MSP, NULL },    { DIAG_FRAME_MSG_TYPE_PS, NULL },
    { DIAG_FRAME_MSG_TYPE_PHY, NULL },   { DIAG_FRAME_MSG_TYPE_BBP, NULL },    { DIAG_FRAME_MSG_TYPE_HSO, NULL },
    { DIAG_FRAME_MSG_TYPE_BSP, NULL },   { DIAG_FRAME_MSG_TYPE_EASYRF, NULL }, { DIAG_FRAME_MSG_TYPE_AP_BSP, NULL },
    { DIAG_FRAME_MSG_TYPE_AUDIO, NULL }, { DIAG_FRAME_MSG_TYPE_APP, NULL },
};

/*
 * Function Name: diag_message_init
 * Description  : HiTSP diag message初始化
 */
void diag_message_init(void)
{
    s32 ret;

    spin_lock_init(&g_diag_message_state.spin_lock);

    ret = diag_message_task_init();
    if (ret) {
        return;
    }

    diag_drv_msg_init();
    diag_srv_proc_func_reg((diag_srv_proc_cb)diag_dl_message_proc);
    diag_crit("diag fusion message init ok\n");

    return;
}

s32 diag_message_task_init(void)
{
    s32 ret;
    struct task_struct *task = NULL;

    task = kthread_create(diag_message_task, NULL, "diag_message_task");
    if (IS_ERR(task)) {
        diag_error("diag_message_task create fail\n");
        return BSP_ERROR;
    }

    g_diag_message_state.task = task;

    /* create msg queue */
    ret = mdrv_msg_qcreate(task->pid, MSG_QNUM_DIAG_APSS);
    if (ret) {
        diag_error("create queue fail, ret=%x\n", ret);
        return ret;
    }

    /* register mid for msg task */
    ret = mdrv_msg_register_mid(task->pid, DRV_MID_DIAG_APSS, MSG_QID_DIAG_APSS);
    if (ret) {
        diag_error("register msg mid fail, ret=%x\n", ret);
        return ret;
    }

    wake_up_process(task);

    return ERR_MSP_SUCCESS;
}

int diag_message_task(void *data)
{
    s32 ret;
    void *msg_get = NULL;
    msg_frame_s *msg_frame = NULL;

    for (;;) {
        ret = mdrv_msg_recv(MSG_QID_DIAG_APSS, &msg_get, 0xffffffff);
        if (ret) {
            diag_error("recv msg fail, ret=0x%x\n", ret);
            continue;
        }

        if (msg_get == NULL) {
            diag_error("msg recv null\n");
            continue;
        }

        msg_frame = (msg_frame_s *)msg_get;
        diag_ptr_record(DIAG_PTR_MESSAGE_RECV, msg_frame->src_id, msg_frame->data_len);

        switch (msg_frame->src_id) {
            case DRV_MID_DIAG_TSP:
                diag_tsp_msg_proc(msg_frame->diag_msg);
                break;

            default:
                diag_warning("unknown msg, src mid = 0x%x\n", msg_frame->src_id);
                break;
        }

        mdrv_msg_free(DRV_MID_DIAG_APSS, (void *)msg_frame);
    }

    return 0;
}

void diag_tsp_msg_proc(void *msg_data)
{
    diag_msg_info_s *diag_msg;

    diag_msg = (diag_msg_info_s *)msg_data;

    diag_ptr_record(DIAG_PTR_TSP_MESSAGE_RECV, diag_msg->msg_id, 0);

    switch (diag_msg->msg_id) {
        case DIAG_MSG_CONN_REQ:
            diag_conn_manager(diag_msg->data);
            break;

        case DIAG_MSG_DISCONN_REQ:
            diag_disconn_manager(diag_msg->data);
            break;

        case DIAG_MSG_CONN_AUTH_REQ:
            diag_auth_proc(diag_msg->data);
            break;

        case DIAG_MSG_MSP_REQ:
            diag_msp_msg_proc((diag_frame_head_s *)diag_msg->data);
            break;

        case DIAG_MSG_POWERON_LOG_QUERY:
            diag_poweron_log_state_send();
            break;

        case DIAG_MSG_RX_READY:
            mdrv_socp_send_data_manager(SOCP_CODER_DST_OM_IND, SOCP_DEST_DSM_ENABLE);
            break;
        case DIAG_MSG_PORT_COMMAND:
            ppm_port_commad_proc(diag_msg->data);
            break;

        default:
            diag_warning("receive msg id invalid, msg_id=0x%x, data=%p\n", diag_msg->msg_id, diag_msg->data);
            break;
    }

    return;
}

void diag_frame_msg_proc(u8 *msg_data)
{
    u32 ret = ERR_MSP_SUCCESS;
    u32 msg_type;
    diag_frame_head_s *msg = (diag_frame_head_s *)msg_data;
    diag_comm_cnf_s cnf_msg;
    diag_cnf_msg_s diag_msg;
    u32 auth_state;

    if (unlikely(msg == NULL)) {
        diag_error("diag msg is NULL\n");
        ret = ERR_MSP_PARA_NULL;
        goto DIAG_ERROR;
    }

    diag_ptr(EN_DIAG_PTR_MESSAGE_IN, 1, msg->cmd_id, msg->msg_len);

    if ((msg->msg_len < sizeof(diag_data_head_s)) || (msg->msg_len > DIAG_MEM_ALLOC_LEN_MAX)) {
        diag_error("msg len error, msg_len = 0x%x\n", msg->msg_len);
        ret = ERR_MSP_INVALID_LEN;
        goto DIAG_ERROR;
    }

    auth_state = diag_get_auth_state();
    if ((auth_state != DIAG_AUTH_TYPE_SUCCESS) && (msg->cmd_id != DIAG_CMD_HOST_CONNECT)) {
        diag_error("diag is not auth, cmdid = 0x%x\n", msg->cmd_id);
        ret = ERR_MSP_CONNECT_AUTH_FAIL;
        goto DIAG_ERROR;
    }

    msg_type = msg->cmdid_stru.pri_4b;
    if ((msg_type < DIAG_FRAME_MSG_TYPE_BUTT) && (g_diag_msg_fn_table[msg_type].diag_message_proc)) {
        g_diag_msg_fn_table[msg_type].diag_message_proc(msg);
        diag_ptr(EN_DIAG_PTR_MESSAGE_PROC, 1, msg->cmd_id, msg->msg_len);
    } else {
        diag_error("msg_type error, cmdid = 0x%x\n", msg->cmd_id);
        ret = ERR_MSP_INVALID_ID;
        goto DIAG_ERROR;
    }

    return;

DIAG_ERROR:
    cnf_msg.result = (s32)ret;
    diag_cnf_msg_fill(msg, &cnf_msg, &diag_msg);
    ret = diag_cnf_msg_report(&diag_msg);
    if (ret) {
        diag_error("report cnf msg fail,ret=0x%x\n", ret);
    }

    return;
}

u32 diag_dl_message_proc(void *msg_data)
{
    diag_msg_info_s *diag_msg = NULL;
    u32 ret;
    u32 len;
    errno_t err;

    diag_ptr_record(DIAG_PTR_DL_MESSAGE_IN, 1, DIAG_SERVICE_HEAD_VER(msg_data));

    if (DIAG_SERVICE_HEAD_VER(msg_data)) {
        len = ((diag_frame_head_s *)msg_data)->msg_len + sizeof(diag_frame_head_s) + sizeof(diag_msg_info_s);
        diag_msg = (diag_msg_info_s *)osl_malloc(len);
        if (diag_msg == NULL) {
            diag_error("malloc fail, len=0x%x", len);
            diag_ptr_record(DIAG_PTR_DL_MESSAGE_ERR, 1, len);
            return ERR_MSP_MALLOC_FAILUE;
        }

        diag_msg->msg_id = DIAG_MSG_CONFIG_TRANS;
        err = memcpy_s(diag_msg->data, sizeof(diag_frame_head_s) + ((diag_frame_head_s *)msg_data)->msg_len,
            (u8 *)msg_data, sizeof(diag_frame_head_s) + ((diag_frame_head_s *)msg_data)->msg_len);
        if (err) {
            diag_error("memcpy fail, ret=0x%x\n", err);
        }
    } else {
        len = ((diag_conn_frame_info_s *)msg_data)->msg_len + sizeof(diag_conn_frame_info_s) + sizeof(diag_msg_info_s);
        diag_msg = (diag_msg_info_s *)osl_malloc(len);
        if (diag_msg == NULL) {
            diag_error("malloc fail, len=0x%x", len);
            diag_ptr_record(DIAG_PTR_DL_MESSAGE_ERR, 2, len);
            return ERR_MSP_MALLOC_FAILUE;
        }

        diag_msg->msg_id = DIAG_MSG_CONN_REQ;
        err = memcpy_s(diag_msg->data, sizeof(diag_conn_frame_info_s) + ((diag_conn_frame_info_s *)msg_data)->msg_len,
            (u8 *)msg_data, sizeof(diag_conn_frame_info_s) + ((diag_conn_frame_info_s *)msg_data)->msg_len);
        if (err) {
            diag_error("memcpy fail, ret=0x%x\n", err);
        }
    }

    ret = diag_msg_send(diag_msg, len, DRV_MID_DIAG_APSS, DRV_MID_DIAG_TSP);
    if (ret) {
        diag_error("send msg to tsp diag fail, ret=0x%x, cmd_id=0x%x\n", ret, ((diag_frame_head_s *)msg_data)->cmd_id);
        diag_ptr_record(DIAG_PTR_DL_MESSAGE_ERR, 3, ret);
        osl_free(diag_msg);
        return ERR_MSP_DIAG_SEND_MSG_FAIL;
    }
    diag_ptr_record(DIAG_PTR_DL_MESSAGE_OK, len, DRV_MID_DIAG_TSP);
    osl_free(diag_msg);
    return ERR_MSP_SUCCESS;
}

s32 diag_msg_send(const void *data, u32 len, u32 send_mid, u32 recv_mid)
{
    msg_frame_s *msg = NULL;
    s32 ret;

    msg = (msg_frame_s *)mdrv_msg_alloc(send_mid, len + sizeof(msg_frame_s), MSG_FULL_LEN);
    if (msg == NULL) {
        diag_error("msg alloc fail,len=0x%lx\n", len + sizeof(msg_frame_s));
        return ERR_MSP_MALLOC_FAILUE;
    }

    MDRV_MSG_HEAD_FILL(msg, send_mid, recv_mid, len);

    ret = memcpy_s(msg->diag_msg, len, (u8 *)data, len);
    if (ret) {
        mdrv_msg_free(send_mid, msg);
        diag_error("memcpy_fail,ret=0x%x\n", ret);
        return ret;
    }

    ret = mdrv_msg_send(msg, 0);
    if (ret) {
        diag_error("msg send fail,ret=0x%x\n", ret);
        mdrv_msg_free(send_mid, msg);
        return ERR_MSP_DIAG_SEND_MSG_FAIL;
    }

    return ERR_MSP_SUCCESS;
}

s32 diag_msg_proc_cb_reg(u32 msg_type, diag_message_proc_fn diag_msg_proc_cb)
{
    if ((msg_type >= DIAG_FRAME_MSG_TYPE_BUTT) || (diag_msg_proc_cb == NULL)) {
        diag_error("diag_msg reg error, msp_type = 0x%x\n", msg_type);
        return ERR_MSP_DIAG_CB_NULL_ERR;
    }

    g_diag_msg_fn_table[msg_type].diag_message_proc = diag_msg_proc_cb;

    return ERR_MSP_SUCCESS;
}

void diag_cnf_msg_fill(diag_frame_head_s *msg_data, diag_comm_cnf_s *cnf_info, diag_cnf_msg_s *diag_msg)
{
    cnf_info->auid = ((diag_data_head_s *)msg_data->data)->auid;
    cnf_info->sn = ((diag_data_head_s *)msg_data->data)->sn;

    diag_msg->msg_id = msg_data->cmd_id;
    diag_msg->msg_len = sizeof(diag_comm_cnf_s);
    diag_msg->data = (u8 *)cnf_info;

    return;
}

s32 diag_cnf_msg_report(diag_cnf_msg_s *cnf_msg)
{
    diag_srv_head_s cnf_head;
    diag_msg_report_head_s diag_head;

    diag_srv_fill_head(&cnf_head);

    /* modem单独复位消息没有对应的auid，因此需要标识为IND */
    if (cnf_msg->msg_id != DIAG_CMD_MODEM_WILL_RESET) {
        cnf_head.frame_header.srv_head.mt_2b = DIAG_FRAME_MT_CNF;
    }

    cnf_head.frame_header.srv_head.msg_trans_id = g_diag_message_state.cnf_trans_id++;
    cnf_head.frame_header.cmd_id = cnf_msg->msg_id;
    cnf_head.frame_header.msg_len = cnf_msg->msg_len;

    diag_head.header_size = sizeof(cnf_head);
    diag_head.header = &cnf_head;
    diag_head.data_size = cnf_msg->msg_len;
    diag_head.data = cnf_msg->data;

    return diag_srv_pack_cnf_data(&diag_head);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
