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

#ifndef PMOM_PLATFORM_H
#define PMOM_PLATFORM_H

#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <msg_id.h>
#include <bsp_msg.h>
#include <osl_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PMOM_MSG_PKT_RECORD_NUM 10
#define PMOM_MSG_PRINT_MAX_LEN 500
#define PMOM_SPRINTF_S_ERR (-1)
#define PMOM_BUF_SIZE 10240U

typedef enum {
    // CCORE->ACORE
    PMOM_MSG_PROC_LOG,
    // ACORE->CCORE
    PMOM_MSG_GATHER_LOG,
    PMOM_MSG_CMD_MAX,
} pmom_msg_cmd_e;

typedef struct {
    pmom_msg_cmd_e cmd;
    u32 log_len;
    u8 log_data[0]; // 变长数组，必须放在最后
} pmom_msg_content_s;

typedef struct {
    struct msg_head hdr;
    pmom_msg_content_s content;
} pmom_msg_pkt_s;

typedef struct {
    pmom_msg_pkt_s msg_pkt[PMOM_MSG_PKT_RECORD_NUM];
    u32 index;
} pmom_msg_pkt_record_s;

typedef struct {
    struct task_struct *tsk;
    struct semaphore pmom_task_sem;
    char *data_buf;
    u32 data_len;
} pmom_platform_ctrl_s;

s32 pmom_platform_init(void);
void pmom_msg_to_ccore(pmom_msg_cmd_e cmd, void *log_buf, u32 log_len);
pmom_platform_ctrl_s *pmom_get_platform_ctrl(void);

#ifdef __cplusplus
}
#endif

#endif
