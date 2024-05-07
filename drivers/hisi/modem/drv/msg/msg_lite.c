/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
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
#include "msg_plat.h"
#include "msg_base.h"
#include "msg_mem.h"
#include "msg_core.h"
#include "msg_fixed.h"
#include "msg_cmsg.h"
#include "msg_lite.h"

static struct msg_chn_hdl g_msg_fd_info[MSG_CHN_CNT_MAX];

int msg_lite_callback(const struct msg_addr *src_addr, const struct msg_addr *dst_addr, void *buf, u32 len)
{
    if (src_addr == NULL || dst_addr == NULL || buf == NULL || len == 0) {
        return MSG_ERR_EINVAL;
    }

    if (dst_addr->chnid > MSG_CHN_CNT_MAX || g_msg_fd_info[dst_addr->chnid].state != MSG_CHN_OPEND) {
        return MSG_ERR_EHOSTUNREACH;
    }
    if ((g_msg_fd_info[dst_addr->chnid].core_mask & MSG_CORE_MASK(src_addr->core)) == 0) {
        return MSG_ERR_EHOSTUNREACH;
    }

    if (g_msg_fd_info[dst_addr->chnid].func == NULL) {
        return MSG_ERR_EHOSTUNREACH;
    }

    g_msg_fd_info[dst_addr->chnid].func(src_addr, buf, len);

    return 0;
}
void bsp_msgchn_attr_init(struct msgchn_attr *pattr)
{
    if (pattr == NULL) {
        return;
    }
    pattr->magic = MID_ATTR_INIT_MAGIC;
}

int bsp_msg_lite_open(struct msg_chn_hdl **ppchn_hdl, struct msgchn_attr *pattr)
{
    u32 chn_id;
    if (ppchn_hdl == NULL || pattr == NULL) {
        return MSG_ERR_EINVAL;
    }
    if (pattr->magic != MID_ATTR_INIT_MAGIC || pattr->chnid >= MSG_CHN_CNT_MAX) {
        return MSG_ERR_ENXIO;
    }

    chn_id = pattr->chnid;

    if (g_msg_fd_info[chn_id].state == MSG_CHN_OPEND) {
        return MSG_ERR_EEXIST;
    }
    g_msg_fd_info[chn_id].chnid = pattr->chnid;
    g_msg_fd_info[chn_id].core_mask = pattr->coremask;
    g_msg_fd_info[chn_id].func = pattr->lite_notify;
    g_msg_fd_info[chn_id].state = MSG_CHN_OPEND;

    *ppchn_hdl = &g_msg_fd_info[chn_id];

    return 0;
}

int bsp_msg_lite_sendto(struct msg_chn_hdl *chn_hdl, const struct msg_addr *dst, void *msg, unsigned len)
{
    int ret;
    struct msg_addr src_addr;
    if (chn_hdl == NULL || dst == NULL || msg == NULL || len == 0) {
        return MSG_ERR_EINVAL;
    }
    if (chn_hdl->state != MSG_CHN_OPEND) {
        return MSG_ERR_EIO;
    }
    src_addr.core = THIS_CORE;
    src_addr.chnid = chn_hdl->chnid;

    if (dst->core != THIS_CORE) {
        return msg_crosscore_send_lite(&src_addr, dst, msg, len);
    }

    // self loop
    ret = msg_lite_callback(&src_addr, dst, msg, len);
    if (ret < 0) {
        return ret;
    }
    return 0;
}
