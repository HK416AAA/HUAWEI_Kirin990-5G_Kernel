/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#include <bsp_print.h>
#include <osl_malloc.h>
#include "pmom_platform.h"
#include <securec.h>

#undef THIS_MODU
#define THIS_MODU mod_pmom

static pmom_platform_ctrl_s g_pmom_platform_ctrl;
static pmom_msg_pkt_record_s g_pmom_msg_recv_record;
static pmom_msg_pkt_record_s g_pmom_msg_send_record;

pmom_platform_ctrl_s *pmom_get_platform_ctrl(void)
{
    return &g_pmom_platform_ctrl;
}

static void pmom_record_msg_pkt(pmom_msg_pkt_s *msg_pkt, pmom_msg_pkt_record_s *msg_pkt_record)
{
    s32 ret;
    ret = memcpy_s(&msg_pkt_record->msg_pkt[msg_pkt_record->index], sizeof(pmom_msg_pkt_s),
                   msg_pkt, sizeof(pmom_msg_pkt_s));
    if (ret != EOK) {
        bsp_err("pmom_record_msg_pkt memcpy_s failed!\n");
        return;
    }
    msg_pkt_record->index++;
    if (msg_pkt_record->index >= PMOM_MSG_PKT_RECORD_NUM) {
        msg_pkt_record->index = 0;
    }
}

// 接口可以发送的msg格式如下
// 1. msg_head cmd log_len log_buf （带log内容的msg）
// 2. msg_head cmd 0 （只带cmd，不带log内容的msg）
void pmom_msg_to_ccore(pmom_msg_cmd_e cmd, void *log_buf, u32 log_len)
{
    s32 ret;
    pmom_msg_pkt_s *msg = NULL;
    u32 src_mid = DRV_MID_PMOM_APSS;
    u32 dest_mid = DRV_MID_PMOM_TSP;
    u32 data_len = sizeof(pmom_msg_content_s) + log_len;

    if (log_buf == NULL && log_len != 0) {
        bsp_err("pmom_msg_to_ccore params error!\n");
        return;
    }

    /* 申请mem，使用MSG_FULL_LEN标记申请时，长度包含了MSG头的长度 */
    msg = (pmom_msg_pkt_s *)mdrv_msg_alloc(src_mid, sizeof(pmom_msg_pkt_s) + log_len, MSG_FULL_LEN);
    if (msg == NULL) {
        bsp_err("pmom_msg_to_ccore mdrv_msg_alloc failed!\n");
        return;
    }
    /* 不要直接对msg中的头字段操作，要用接口，fill时候的长度不包含MSG头长度 */
    MDRV_MSG_HEAD_FILL(msg, src_mid, dest_mid, data_len);

    /* 填写消息中的用户内容 */
    msg->content.cmd = cmd;
    msg->content.log_len = log_len;
    if (log_buf != NULL) {
        ret = memcpy_s(msg->content.log_data, msg->content.log_len, log_buf, log_len);
        if (ret != EOK) {
            bsp_err("pmom_msg_to_ccore memcpy_s log_data failed!\n");
            mdrv_msg_free(src_mid, msg);
            return;
        }
    }

    pmom_record_msg_pkt(msg, &g_pmom_msg_send_record);
    ret = mdrv_msg_send(msg, MSG_DEFAULT); /* 一般使用MSG_DEFAULT默认标记发送就可以 */
    if (ret != 0) {
        mdrv_msg_free(src_mid, msg);
        bsp_err("pmom_msg_to_ccore mdrv_msg_send failed!\n");
        return;
    }
    return;
}

static void pmom_msg_to_buf(char *cmdmsg_data, u32 cmdmsg_len)
{
    pmom_platform_ctrl_s *ctrl = &g_pmom_platform_ctrl;
    s32 ret;
    if (cmdmsg_data == NULL || cmdmsg_len == 0) {
        bsp_err("pmom_msg_to_buf cmdmsg_data is null or cmdmsg_len is 0!\n");
        return;
    }

    if (cmdmsg_len >= PMOM_BUF_SIZE) {
        bsp_err("pmom_msg_to_buf cmdmsg_len is too large!\n");
        return;
    }

    (void)memset_s(ctrl->data_buf, PMOM_BUF_SIZE, 0, PMOM_BUF_SIZE);
    ret = memcpy_s(ctrl->data_buf, PMOM_BUF_SIZE, cmdmsg_data, cmdmsg_len);
    if (ret != EOK) {
        bsp_err("pmom_msg_to_buf memcpy_s failed!\n");
        return;
    }
    ctrl->data_len = cmdmsg_len;
    up(&ctrl->pmom_task_sem);
}

static s32 pmom_msg_workthread(void *arg)
{
    s32 ret;
    void *msg = NULL;
    pmom_msg_pkt_s *pkt = NULL;
    pmom_msg_content_s *cmdmsg = NULL;
    u32 my_msg_len;
    u32 src_id;

    for (;;) {
        ret = mdrv_msg_recv(0, &msg, MDRV_MSG_WAIT_FOREVER);
        if (ret) {
            bsp_err("pmom_msg_workthread mdrv_msg_recv failed!\n");
            continue;
        }
        if (msg == NULL) {
            bsp_err("pmom_msg_workthread mdrv_msg_recv null!\n");
            continue;
        }
        src_id = MDRV_MSG_HEAD_GET_SRCID(msg);
        if (src_id != DRV_MID_PMOM_TSP) {
            bsp_err("pmom_msg_workthread src_id(%u) err!\n", src_id);
            mdrv_msg_free(DRV_MID_PMOM_APSS, msg);
            msg = NULL;
            continue;
        }
        pkt = (pmom_msg_pkt_s *)msg;
        pmom_record_msg_pkt(pkt, &g_pmom_msg_recv_record);
        cmdmsg = &pkt->content;
        my_msg_len = MDRV_MSG_HEAD_GET_LEN(msg);
        if (my_msg_len != (sizeof(pmom_msg_content_s) + cmdmsg->log_len)) {
            bsp_err("pmom_msg_workthread recv unexpected msg!\n");
            mdrv_msg_free(DRV_MID_PMOM_APSS, msg);
            msg = NULL;
            continue;
        }
        switch (cmdmsg->cmd) {
            case PMOM_MSG_PROC_LOG:
                pmom_msg_to_buf(cmdmsg->log_data, cmdmsg->log_len);
                break;
            default:
                bsp_err("pmom_msg_workthread recv unexpected cmd(%u) msg!\n", cmdmsg->cmd);
        }

        mdrv_msg_free(DRV_MID_PMOM_APSS, msg);
        msg = NULL;
    }

    return BSP_OK;
}

static s32 pmom_msg_init(void)
{
    s32 ret;
    pmom_platform_ctrl_s *ctrl = &g_pmom_platform_ctrl;

    ctrl->tsk = kthread_create(pmom_msg_workthread, &ctrl->tsk, "pmom_msg_ap");
    if (IS_ERR(ctrl->tsk)) {
        bsp_err("pmom_msg_init kthread_create pmom_msg_acpu failed!\n");
        return BSP_ERROR;
    }
    ret = mdrv_msg_qcreate(ctrl->tsk->pid, 1);
    if (ret != 0) {
        bsp_err("pmom_msg_init mdrv_msg_qcreate pmom_msg_acpu failed!\n");
        return ret;
    }

    ret = mdrv_msg_register_mid(ctrl->tsk->pid, DRV_MID_PMOM_APSS, 0);
    if (ret != 0) {
        bsp_err("pmom_msg_init mdrv_msg_register_mid DRV_MID_PMOM_APSS failed!\n");
        return ret;
    }
    (void)wake_up_process(ctrl->tsk);

    return BSP_OK;
}

s32 pmom_platform_init(void)
{
    s32 ret;
    pmom_platform_ctrl_s *ctrl = &g_pmom_platform_ctrl;

    sema_init(&ctrl->pmom_task_sem, 0);
    ctrl->data_buf = (char *)osl_malloc(PMOM_BUF_SIZE);
    if (ctrl->data_buf == NULL) {
        bsp_err("pmom_platform_init osl_malloc data_buf failed!\n");
        return BSP_ERROR;
    }
    (void)memset_s(ctrl->data_buf, PMOM_BUF_SIZE, 0, PMOM_BUF_SIZE);
    ret = pmom_msg_init();
    if (ret != BSP_OK) {
        osl_free(ctrl->data_buf);
        ctrl->data_buf = NULL;
        return ret;
    }

    return BSP_OK;
}

static void pmom_msg_print_record(pmom_msg_pkt_record_s *msg_pkt_record)
{
    u32 i, j, count;
    s32 safe_len;
    char msg_print[PMOM_MSG_PRINT_MAX_LEN] = { 0 };

    bsp_err("current index = %u\n", msg_pkt_record->index);
    for (i = 0; i < PMOM_MSG_PKT_RECORD_NUM; i++) {
        count = 0;
        safe_len = sprintf_s(msg_print, PMOM_MSG_PRINT_MAX_LEN, "%02u msg_head: ", i);
        if (safe_len == PMOM_SPRINTF_S_ERR) {
            bsp_err("msg_head sprintf_s failed!\n");
            return;
        }
        count = count + (u32)safe_len;
        for (j = 0; j < sizeof(struct msg_head); j++) {
            safe_len = sprintf_s(msg_print + count, PMOM_MSG_PRINT_MAX_LEN - count,
                                 "%02X ", ((uint8_t *)(&msg_pkt_record->msg_pkt[i].hdr))[j]);
            if (safe_len == PMOM_SPRINTF_S_ERR) {
                bsp_err("hdr sprintf_s failed!\n");
                return;
            }
            count = count + (u32)safe_len;
        }
        safe_len = sprintf_s(msg_print + count, PMOM_MSG_PRINT_MAX_LEN - count,
                             "\n           content: ");
        if (safe_len == PMOM_SPRINTF_S_ERR) {
            bsp_err("content sprintf_s failed!\n");
            return;
        }
        count = count + (u32)safe_len;
        for (j = 0; j < sizeof(pmom_msg_content_s); j++) {
            safe_len = sprintf_s(msg_print + count, PMOM_MSG_PRINT_MAX_LEN - count,
                                 "%02X ", ((uint8_t *)(&msg_pkt_record->msg_pkt[i].content))[j]);
            if (safe_len == PMOM_SPRINTF_S_ERR) {
                bsp_err("msg_pkt.content sprintf_s failed!\n");
                return;
            }
            count = count + (u32)safe_len;
        }
        bsp_err("%s\n", msg_print);
    }
}

void pmom_msg_print(void)
{
    bsp_err("msg_send_record:\n");
    pmom_msg_print_record(&g_pmom_msg_send_record);
    bsp_err("msg_recv_record:\n");
    pmom_msg_print_record(&g_pmom_msg_recv_record);
}

