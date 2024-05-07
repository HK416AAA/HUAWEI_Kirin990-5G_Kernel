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

#ifndef __DIAG_FUSION_MESSAGE_H__
#define __DIAG_FUSION_MESSAGE_H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include "osl_types.h"
#include "osl_sem.h"
#include "msg_id.h"
#include "mdrv_msg.h"
#include "bsp_diag.h"
#include "osl_spinlock.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(4)

#define MSG_QID_DIAG_APSS 0
#define MSG_QNUM_DIAG_APSS 4

typedef struct {
    struct task_struct *task;
    spinlock_t spin_lock;
    u32 cnf_trans_id;
} diag_message_state_s;

typedef struct {
    MDRV_MSG_HEAD
    u8 diag_msg[0];
} msg_frame_s;

typedef struct {
    u32 msg_id;
    u8 data[0]; //lint !e43
} diag_msg_info_s;

typedef void (*diag_message_proc_fn)(diag_frame_head_s *data);
typedef struct {
    u32 msg_type;
    diag_message_proc_fn diag_message_proc;
} diag_message_proc_s;

typedef struct {
    u32 msg_id;
    u32 msg_len;
    u8 *data;
} diag_cnf_msg_s;

void diag_message_init(void);
s32 diag_message_task_init(void);
int diag_message_task(void *data);
s32 diag_msg_send(const void *data, u32 len, u32 send_mid, u32 recv_mid);
void diag_cnf_msg_fill(diag_frame_head_s *msg_data, diag_comm_cnf_s *cnf_info, diag_cnf_msg_s *diag_msg);
s32 diag_cnf_msg_report(diag_cnf_msg_s *cnf_msg);
void diag_frame_msg_proc(u8 *msg_data);
void diag_tsp_msg_proc(void *msg_data);
s32 diag_msg_proc_cb_reg(u32 msg_type, diag_message_proc_fn diag_msg_proc_cb);
s32 diag_cnf_msg_report(diag_cnf_msg_s *cnf_msg);
u32 diag_dl_message_proc(void *msg_data);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
