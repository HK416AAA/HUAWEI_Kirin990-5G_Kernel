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
#include <linux/version.h>
#include <linux/device.h>
#include <linux/semaphore.h>
#include <linux/dma-mapping.h>
#include <linux/of_device.h>
#include <linux/kthread.h>
#include "bsp_eicc.h"
#include "msg_plat.h"
#include "msg_base.h"
#include "msg_mem.h"
#include "msg_core.h"
#include "msg_lite.h"
#include "msg_cmsg.h"

#define MSG_MAX_SIZE 0x4000
#define MSG_CHAN_RSV_SIZE 128
#define MSG_CHAN_BUFF_SIZE (MSG_MAX_SIZE * 2)

#define CMSG_SEND_TRY_WAIT_TIMES 200

struct msg_eicc_cfg {
    unsigned cid;
    unsigned smsg_sz;
    unsigned send_chn;
    unsigned sbuf_sz;
    unsigned recv_chn;
    unsigned rbuf_sz;
};

struct msg_eicc_cfg g_msg_eicc_cfg[] = {
    {MSG_CID_LPMCU, 256, EICC_CHN_SEND_AP2LPM_MDRV_MSG, 512, EICC_CHN_RECV_LPM2AP_MDRV_MSG, 512},
    {MSG_CID_TSP, MSG_MAX_SIZE - MSG_CHAN_RSV_SIZE, EICC_CHN_SEND_AP2TSP_MDRV_MSG, MSG_CHAN_BUFF_SIZE,
     EICC_CHN_RECV_TSP2AP_MDRV_MSG, MSG_CHAN_BUFF_SIZE},
};

#define MSG_CHN_STATUS_UNINIT 0
#define MSG_CHN_STATUS_INITING 1
#define MSG_CHN_STATUS_INITED 2

struct cmsg_dbg {
    u32 seq_err;
    u32 segmsg_err;
    u32 ioctl_err;
    u32 recv_err;
    u32 lenchk_err;
    u32 alloc_err;
    u32 pktfw_err;
    u32 longmsg_cnt;
};

struct cmsg_ctrl {
    unsigned cid;
    unsigned status;
    struct task_struct *worker;
    struct semaphore worker_sem;
    struct wakeup_source worker_wakelock;
    struct spinlock s_lock;
    eicc_chn_t s_handle;
    u32 s_max_pkt_len;
    eicc_chn_t r_handle;
    u16 s_seq;
    u16 r_seq;
    void *lmsg;
    u32 lmsg_off;
    struct cmsg_dbg dbg_info;
};

struct cmsg_ctrl g_cmsg_ctrl[MSG_CORE_MAX];
struct wakeup_source g_cmsg_wake_lock;

static struct platform_device g_cmsg_plat_device = {
    .name = "cmsg_plat",
};

static inline struct cmsg_ctrl *cmsg_get_ctrl(unsigned cid)
{
    if (cid > MSG_CORE_MAX) {
        return NULL;
    }
    return &g_cmsg_ctrl[cid];
}

static int recv_short_msg(struct cmsg_ctrl *ctrl, struct cmsg_hdr *phdr)
{
    int ret;
    void *msg = NULL;
    struct msg_head hdr;
    struct msg_flag *msg_flag = NULL;
    u32 send_flags = __MSG_NO_TS;
    struct cmsg_dbg *dbg = &ctrl->dbg_info;

    /* This may be obmited */
    if (phdr->len <= MDRV_MSG_HEAD_LEN) {
        dbg->lenchk_err++;
        msg_crit("recv_short_msg too small,phdr->len=%d\n", phdr->len);
        return MSG_ERR_EBADMSG;
    }
    ret = bsp_eicc_chn_recv(ctrl->r_handle, &hdr, sizeof(struct msg_head), EICC_RECV_FLAGS_DATAPEEK);
    if (ret != sizeof(struct msg_head)) {
        dbg->recv_err++;
        msg_crit("recv_short_msg too small,phdr->len=%d, ret = %d\n", phdr->len, ret);
        return MSG_ERR_EBADMSG;
    }

    /* 这个检查 确保eicc里的数据长度， 和msg自己认为的数据长度一致 */
    if (phdr->len != MDRV_MSG_HEAD_GET_LEN(&hdr) + sizeof(struct msg_head)) {
        dbg->lenchk_err++;
        msg_crit("recv_short_msg len chk fail,phdr->len=%d, msg len = %d\n", phdr->len, MDRV_MSG_HEAD_GET_LEN(&hdr));
        return MSG_ERR_EBADMSG;
    }

    msg = mdrv_msg_alloc(MDRV_MSG_HEAD_GET_SRCID(&hdr), phdr->len, MSG_FULL_LEN);
    if (msg == NULL) {
        dbg->alloc_err++;
        msg_crit("recv_short_msg alloc %d failed\n", phdr->len);
        return MSG_ERR_ENOMEM;
    }

    ret = bsp_eicc_chn_recv(ctrl->r_handle, msg, phdr->len, 0);
    if (ret != (int)phdr->len) {
        dbg->recv_err++;
        msg_crit("recv_short_msg recv err len=%d ret=%d\n", phdr->len, ret);
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(&hdr), msg);
        return MSG_ERR_EIO;
    }

    msg_flag = (struct msg_flag *)&(MSG_HEAD_GET_FLAG(msg));
    if (msg_flag->urgent) {
        send_flags |= MSG_UG;
    }

    ret = mdrv_msg_send(msg, send_flags);
    if (ret != 0) {
        dbg->pktfw_err++;
        msg_err("eicc crosscore forward failed ret=%d\n", ret);
        // 这里增加异步消息返回
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(&hdr), msg);
        return ret;
    }
    return 0;
}

static int recv_lite_msg(struct cmsg_ctrl *ctrl, struct cmsg_hdr *phdr)
{
    int ret;
    void *msg = NULL;
    struct cmsg_dbg *dbg = &ctrl->dbg_info;
    struct cmsg_lite_exthdr *lite_exthdr = NULL;
    struct msg_addr src_addr;
    struct msg_addr dst_addr;

    u32 alloc_len = phdr->len + sizeof(struct cmsg_lite_exthdr);
    msg = mdrv_msg_alloc(DRV_MID_MSG, alloc_len, MSG_FULL_LEN);
    if (msg == NULL) {
        dbg->alloc_err++;
        msg_crit("recv_lite_msg alloc %d failed\n", phdr->len);
        return MSG_ERR_ENOMEM;
    }
    ret = bsp_eicc_chn_recv(ctrl->r_handle, msg, phdr->len + sizeof(struct cmsg_lite_exthdr), 0);
    if (ret != (int)(phdr->len + sizeof(struct cmsg_lite_exthdr))) {
        dbg->recv_err++;
        mdrv_msg_free(DRV_MID_MSG, msg);
        msg_crit("recv_short_msg recv err len=%d ret=%d\n", phdr->len, ret);
        return MSG_ERR_EIO;
    }
    lite_exthdr = (struct cmsg_lite_exthdr *)msg;

    src_addr.core = (lite_exthdr->src >> 0x10);
    src_addr.chnid = (lite_exthdr->src & 0xFFFF);
    dst_addr.core = (lite_exthdr->dst >> 0x10);
    dst_addr.chnid = (lite_exthdr->dst & 0xFFFF);
    ret = msg_lite_callback(&src_addr, &dst_addr, (void *)(lite_exthdr + 1), phdr->len);
    if (ret != 0) {
        dbg->pktfw_err++;
        msg_err("msg fw failed 0x%x->0x%x ret=%d\n", (unsigned)lite_exthdr->src, (unsigned)lite_exthdr->dst, ret);
    }
    mdrv_msg_free(DRV_MID_MSG, msg);
    return ret;
}

static int recv_segbeg(struct cmsg_ctrl *ctrl, struct cmsg_hdr *phdr)
{
    int ret;
    void *msg = NULL;
    struct msg_head hdr;
    u32 msg_data_len;
    struct cmsg_dbg *dbg = &ctrl->dbg_info;

    dbg->longmsg_cnt++;
    if (ctrl->lmsg != NULL) {
        dbg->segmsg_err++;
        msg_err("recv_segbeg err\n");
        return MSG_ERR_EPROTO;
    }

    if (phdr->len <= MDRV_MSG_HEAD_LEN) {
        dbg->lenchk_err++;
        msg_crit("recv_segbeg too small,phdr->len=%d\n", phdr->len);
        return MSG_ERR_EBADMSG;
    }
    ret = bsp_eicc_chn_recv(ctrl->r_handle, &hdr, sizeof(struct msg_head), EICC_RECV_FLAGS_DATAPEEK);
    if (ret != MDRV_MSG_HEAD_LEN) {
        dbg->recv_err++;
        msg_crit("recv_segbeg too small,phdr->len=%d, ret = %d\n", phdr->len, ret);
        return MSG_ERR_EBADMSG;
    }

    msg_data_len = MDRV_MSG_HEAD_GET_LEN(&hdr);
    msg = mdrv_msg_alloc(MDRV_MSG_HEAD_GET_SRCID(&hdr), msg_data_len, MSG_DEFAULT);
    if (msg == NULL) {
        dbg->alloc_err++;
        msg_crit("recv_segbeg alloc %d failed\n", msg_data_len);
        return MSG_ERR_ENOMEM;
    }

    ret = bsp_eicc_chn_recv(ctrl->r_handle, msg, phdr->len, 0);
    if (ret != (int)phdr->len) {
        dbg->recv_err++;
        msg_crit("recv_segbeg recv err len=%d ret=%d\n", phdr->len, ret);
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(&hdr), msg);
        return MSG_ERR_EIO;
    }
    ctrl->lmsg = msg;
    ctrl->lmsg_off = phdr->len;
    return 0;
}
static int recv_segmsg(struct cmsg_ctrl *ctrl, struct cmsg_hdr *phdr)
{
    int ret;
    u32 msg_len;
    struct cmsg_dbg *dbg = &ctrl->dbg_info;

    if (ctrl->lmsg == NULL) {
        dbg->segmsg_err++;
        msg_err("recv_segmsg err\n");
        return MSG_ERR_EPROTO;
    }
    msg_len = MDRV_MSG_HEAD_GET_LEN(ctrl->lmsg) + MDRV_MSG_HEAD_LEN;
    if (phdr->len + ctrl->lmsg_off > msg_len) {
        dbg->lenchk_err++;
        msg_crit("recv_segmsg len overflow, phdr->len=%d\n", phdr->len);
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(ctrl->lmsg), ctrl->lmsg);
        ctrl->lmsg = NULL;
        ctrl->lmsg_off = 0;
        return MSG_ERR_EBADMSG;
    }
    ret = bsp_eicc_chn_recv(ctrl->r_handle, (u8 *)ctrl->lmsg + ctrl->lmsg_off, phdr->len, 0);
    if (ret != (int)phdr->len) {
        dbg->recv_err++;
        msg_crit("recv_segmsg recv err len=%d ret=%d\n", phdr->len, ret);
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(ctrl->lmsg), ctrl->lmsg);
        ctrl->lmsg = NULL;
        ctrl->lmsg_off = 0;
        return MSG_ERR_EIO;
    }
    ctrl->lmsg_off += phdr->len;
    return 0;
}
static int recv_segend(struct cmsg_ctrl *ctrl, struct cmsg_hdr *phdr)
{
    int ret;
    u32 msg_len;
    struct msg_flag *msg_flag = NULL;
    u32 send_flags = __MSG_NO_TS;
    struct cmsg_dbg *dbg = &ctrl->dbg_info;

    if (ctrl->lmsg == NULL) {
        dbg->segmsg_err++;
        msg_err("recv_segend err\n");
        return MSG_ERR_EBADMSG;
    }

    msg_len = MDRV_MSG_HEAD_GET_LEN(ctrl->lmsg) + MDRV_MSG_HEAD_LEN;
    if (phdr->len + ctrl->lmsg_off != msg_len) {
        dbg->lenchk_err++;
        msg_crit("recv_segend len chk fail, phdr->len=%d\n", phdr->len);
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(ctrl->lmsg), ctrl->lmsg);
        ctrl->lmsg = NULL;
        ctrl->lmsg_off = 0;
        return MSG_ERR_EBADMSG;
    }

    ret = bsp_eicc_chn_recv(ctrl->r_handle, (u8 *)ctrl->lmsg + ctrl->lmsg_off, phdr->len, 0);
    if (ret != (int)phdr->len) {
        dbg->recv_err++;
        msg_crit("recv_segend recv err len=%d ret=%d\n", phdr->len, ret);
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(ctrl->lmsg), ctrl->lmsg);
        ctrl->lmsg = NULL;
        ctrl->lmsg_off = 0;
        return MSG_ERR_EIO;
    }
    msg_flag = (struct msg_flag *)&(MSG_HEAD_GET_FLAG(ctrl->lmsg));
    if (msg_flag->urgent) {
        send_flags |= MSG_UG;
    }

    ret = mdrv_msg_send(ctrl->lmsg, send_flags);
    if (ret != 0) {
        dbg->pktfw_err++;
        msg_err("eicc crosscore forward failed ret=%d\n", ret);
        // 这里增加异步消息返回
        mdrv_msg_free(MDRV_MSG_HEAD_GET_SRCID(ctrl->lmsg), ctrl->lmsg);
        ctrl->lmsg = NULL;
        ctrl->lmsg_off = 0;
        return ret;
    }
    ctrl->lmsg = NULL;
    ctrl->lmsg_off = 0;
    return 0;
}

int recv_long_msg(struct cmsg_ctrl *ctrl, struct cmsg_hdr *phdr)
{
    if (phdr->type == CMSG_TYPE_SEGBEG) {
        return recv_segbeg(ctrl, phdr);
    } else if (phdr->type == CMSG_TYPE_SEGMSG) {
        return recv_segmsg(ctrl, phdr);
    } else if (phdr->type == CMSG_TYPE_SEGEND) {
        return recv_segend(ctrl, phdr);
    } else {
        return -1;
    }
}

static int _msg_recv_process(struct cmsg_ctrl *ctrl, u32 pktlen)
{
    int ret;
    struct cmsg_hdr hdr;
    struct cmsg_hdr *phdr = &hdr;
    struct cmsg_dbg *dbg = &ctrl->dbg_info;
    u32 hdr_len;

    ret = bsp_eicc_chn_recv(ctrl->r_handle, phdr, sizeof(struct cmsg_hdr), 0);
    if (ret != (int)sizeof(struct cmsg_hdr)) {
        dbg->recv_err++;
        msg_err("msg_recv_process unexpected\n");
        /* skip this packet */
        return MSG_ERR_EBADMSG;
    }

    if (phdr->type == CMSG_TYPE_LITE) {
        hdr_len = sizeof(struct cmsg_hdr) + sizeof(struct cmsg_lite_exthdr);
    } else {
        hdr_len = sizeof(struct cmsg_hdr);
    }

    /* 重要：这个检查成立，就可以认为phdr->len是可信的，这样后面不用再检查phdr->len溢出等 */
    if (pktlen != phdr->len + hdr_len) {
        dbg->lenchk_err++;
        msg_crit("msg_recv_process len check failed\n");
        return MSG_ERR_EBADMSG;
    }
    if (phdr->seq != ctrl->r_seq) {
        dbg->seq_err++;
        msg_err("msg_recv_process seq check failed,resync\n");
        ctrl->r_seq = phdr->seq;
    }
    ctrl->r_seq++;
    if (phdr->type == CMSG_TYPE_SINGLE) {
        ret = recv_short_msg(ctrl, phdr);
        if (ret) {
            msg_print("recv short msg err\n");
            return -1;
        }
    } else if (phdr->type == CMSG_TYPE_LITE) {
        ret = recv_lite_msg(ctrl, phdr);
        if (ret) {
            msg_print("recv short msg err\n");
            return -1;
        }
    } else {
        ret = recv_long_msg(ctrl, phdr);
        if (ret) {
            msg_print("recv long msg err\n");
            return -1;
        }
    }
    return 0;
}

int msg_recv_process(struct cmsg_ctrl *ctrl)
{
    int ret;
    ioctl_nxtpkt pktinfo;
    struct cmsg_dbg *dbg = &ctrl->dbg_info;

    ret = bsp_eicc_chn_ioctl(ctrl->r_handle, EICC_IOCTL_CHN_NXTPKT_INF, &pktinfo, sizeof(pktinfo));
    if (ret) {
        dbg->ioctl_err++;
        msg_err("eicc ioctrl err\n");
        return -1;
    }

    if (pktinfo.len == 0) {
        /* 这属于正常情况,不要打印 */
        return MSG_ERR_EAGAIN;
    }

    if (pktinfo.len < sizeof(struct cmsg_hdr)) {
        dbg->lenchk_err++;
        msg_crit("msg_recv_process unexpected\n");
        /* skip this packet */
        return -1;
    }

    return _msg_recv_process(ctrl, pktinfo.len);
}

static int msg_eicc_recv_proc(void *arg)
{
    int ret;
    struct cmsg_ctrl *ctrl = (struct cmsg_ctrl *)arg;
    while (!kthread_should_stop()) {
        down(&ctrl->worker_sem);
        do {
            ret = msg_recv_process(ctrl);
        } while (ret == 0);
        __pm_relax(&ctrl->worker_wakelock);
    };
    return 0;
}

int msg_eicc_recv_cbk(eicc_event event, void *arg, const eicc_eventinfo *event_info)
{
    struct cmsg_ctrl *ctrl = NULL;
    msg_trace("msg_eicc_recv_cbk in\n");

    if (event == EICC_EVENT_DATA_ARRV) {
        ctrl = (struct cmsg_ctrl *)arg;
        __pm_stay_awake(&ctrl->worker_wakelock);
        up(&ctrl->worker_sem);
    } else {
        msg_err("eicc Something went wrong\n");
        return -1;
    }
    msg_trace("msg_eicc_recv_cbk out\n");
    return 0;
}

static int send_short_msg(struct cmsg_ctrl *ctrl, void *msg, unsigned msg_len)
{
    int ret;
    int try_times;

    struct cmsg_hdr hdr;
    struct cmsg_hdr *phdr = &hdr;

    eicc_blkx2_desc_t send_desc;

    phdr->type = CMSG_TYPE_SINGLE;
    phdr->rsv = 0;
    phdr->seq = ctrl->s_seq;
    phdr->flags = 0;
    phdr->len = msg_len;

    send_desc.cnt = 0x2;
    send_desc.datablk[0].len = sizeof(hdr);
    send_desc.datablk[0].buf = phdr;
    send_desc.datablk[1].len = msg_len;
    send_desc.datablk[1].buf = msg;

    for (try_times = 0; try_times < CMSG_SEND_TRY_WAIT_TIMES; try_times++) {
        ret = bsp_eicc_chn_blks_send(ctrl->s_handle, (eicc_blk_desc_t *)&send_desc, 0);
        if (ret == EICC_ERR_EAGAIN) {
            continue;
        }
        break;
    };

    if (ret < 0) {
        msg_err("msg send err, ret=%d\n", ret);
        return -1;
    }
    ctrl->s_seq++;
    if (ret != sizeof(hdr) + msg_len) {
        msg_err("msg send msg_len err,real send msg_len=%d, user send msg_len=%d\n", ret, (int)(sizeof(hdr) + msg_len));
        return -1;
    }
    return 0;
}

static int send_long_msg(struct cmsg_ctrl *ctrl, void *msg, unsigned len)
{
    int ret;
    int try_times, seg_no, seg_cnt;
    unsigned send_len, left_len, pkt_max_len;
    struct cmsg_hdr hdr;
    struct cmsg_hdr *phdr = &hdr;

    eicc_blkx2_desc_t send_desc;

    /* 每个跨核报文，都带了一个struct cmsg_hdr */
    pkt_max_len = ctrl->s_max_pkt_len - sizeof(struct cmsg_hdr);

    seg_cnt = (int)((len + (pkt_max_len - 1)) / pkt_max_len);

    send_desc.cnt = 0x2;
    send_desc.datablk[0].buf = phdr;
    send_desc.datablk[0].len = sizeof(struct cmsg_hdr);

    send_len = 0;
    left_len = len;
    for (seg_no = 0; seg_no < seg_cnt; seg_no++) {
        /* left 不可能为0 */
        if (seg_no == 0) {
            phdr->type = CMSG_TYPE_SEGBEG;
        } else if (seg_no == (seg_cnt - 1)) {
            phdr->type = CMSG_TYPE_SEGEND;
        } else {
            phdr->type = CMSG_TYPE_SEGMSG;
        }
        phdr->rsv = 0;
        phdr->seq = ctrl->s_seq;
        phdr->flags = 0;
        if (left_len >= pkt_max_len) {
            phdr->len = (u16)pkt_max_len;
        } else {
            /*  seg_no 一定为 seg_cnt-1 */
            phdr->len = (u16)left_len;
        }
        send_desc.datablk[1].buf = (void *)((char *)msg + send_len);
        send_desc.datablk[1].len = phdr->len;

        send_len += phdr->len;
        left_len -= phdr->len;

        for (try_times = 0; try_times < CMSG_SEND_TRY_WAIT_TIMES; try_times++) {
            ret = bsp_eicc_chn_blks_send(ctrl->s_handle, (eicc_blk_desc_t *)&send_desc, 0);
            if (ret == EICC_ERR_EAGAIN) {
                continue;
            }
            break;
        };
        if (ret < 0) {
            msg_err("msg send err, ret=%d\n", ret);
            return -1;
        }
        ctrl->s_seq++;
    }

    return 0;
}
static int send_lite_msg(struct cmsg_ctrl *ctrl, const struct msg_addr *src_addr, const struct msg_addr *dst_addr,
    void *buf, u32 len)
{
    int ret;
    int try_times;

    struct cmsg_hdr hdr;
    struct cmsg_lite_exthdr lite_exthdr;

    eicc_blkx3_desc_t send_desc;

    hdr.type = CMSG_TYPE_LITE;
    hdr.rsv = 0;
    hdr.seq = ctrl->s_seq;
    hdr.flags = 0;
    hdr.len = len;

    lite_exthdr.src = (src_addr->core << 0x10) | (src_addr->chnid);
    lite_exthdr.dst = (dst_addr->core << 0x10) | (dst_addr->chnid);

    send_desc.cnt = 0x3;
    send_desc.datablk[0].len = sizeof(hdr);
    send_desc.datablk[0].buf = &hdr;
    send_desc.datablk[1].len = sizeof(lite_exthdr);
    send_desc.datablk[1].buf = &lite_exthdr;
    send_desc.datablk[0x2].len = len;
    send_desc.datablk[0x2].buf = buf;

    for (try_times = 0; try_times < CMSG_SEND_TRY_WAIT_TIMES; try_times++) {
        ret = bsp_eicc_chn_blks_send(ctrl->s_handle, (eicc_blk_desc_t *)&send_desc, 0);
        if (ret == EICC_ERR_EAGAIN) {
            continue;
        }
        break;
    };

    if (ret < 0) {
        msg_err("msg send err, ret=%d\n", ret);
        return -1;
    }
    ctrl->s_seq++;
    if ((unsigned)ret != sizeof(hdr) + sizeof(lite_exthdr) + len) {
        msg_err("msg send msg_len err,real send msg_len=%d, user send msg_len=%d\n", (int)ret,
            (int)(sizeof(hdr) + len));
        return -1;
    }
    return 0;
}

int msg_crosscore_send(unsigned cid, void *msg)
{
    int ret;
    unsigned msg_len;
    unsigned cmsg_len;
    unsigned long lockflags;
    struct cmsg_ctrl *ctrl = NULL;

    ctrl = cmsg_get_ctrl(cid);
    if (ctrl == NULL || ctrl->status != MSG_CHN_STATUS_INITED) {
        return -1;
    }
    msg_len = MDRV_MSG_HEAD_LEN + MDRV_MSG_HEAD_GET_LEN(msg);
    cmsg_len = sizeof(struct cmsg_hdr) + msg_len;

    spin_lock_irqsave(&ctrl->s_lock, lockflags);
    if (cmsg_len <= ctrl->s_max_pkt_len) {
        ret = send_short_msg(ctrl, msg, msg_len);
    } else {
        ret = send_long_msg(ctrl, msg, msg_len);
    }
    spin_unlock_irqrestore(&ctrl->s_lock, lockflags);
    return ret;
}

int msg_crosscore_send_lite(const struct msg_addr *src_addr, const struct msg_addr *dst_addr, void *buf, u32 len)
{
    int ret;
    unsigned long lockflags;
    struct cmsg_ctrl *ctrl = NULL;
    ctrl = cmsg_get_ctrl(dst_addr->core);
    if (ctrl == NULL || ctrl->status != MSG_CHN_STATUS_INITED) {
        return -1;
    }
    spin_lock_irqsave(&ctrl->s_lock, lockflags);
    ret = send_lite_msg(ctrl, src_addr, dst_addr, buf, len);
    spin_unlock_irqrestore(&ctrl->s_lock, lockflags);
    return ret;
}

static void *msg_eicc_buf_alloc(size_t size, dma_addr_t *dma_handle)
{
    void *mem_addr = NULL;
    mem_addr = dma_alloc_coherent(&g_cmsg_plat_device.dev, size, dma_handle, GFP_KERNEL);
    if (mem_addr == NULL) {
        return NULL;
    }
    return mem_addr;
}

static void msg_eicc_buf_free(size_t size, void *cpu_addr, dma_addr_t dma_handle)
{
    dma_free_coherent(&g_cmsg_plat_device.dev, size, cpu_addr, dma_handle);
    return;
}

static int msg_eicc_schan_open(struct msg_eicc_cfg *eicc_cfg, struct cmsg_ctrl *ctrl)
{
    int ret;
    void *addr = NULL;
    dma_addr_t dma_addr;
    eicc_chn_attr_t attr;
    eicc_chn_attr_t *pattr = &attr;

    addr = msg_eicc_buf_alloc(eicc_cfg->sbuf_sz, &dma_addr);
    if (addr == NULL) {
        msg_err("msg dma mem alloc error\n");
        return -1;
    }

    ret = bsp_eicc_chn_attr_init(&attr);
    if (ret) {
        msg_err("chan attr init error\n");
        return -1;
    }

    pattr->chnid = eicc_cfg->send_chn;
    pattr->type = EICC_CHN_TYPE_SEND;
    pattr->pa = dma_addr;
    pattr->va = addr;
    pattr->size = eicc_cfg->sbuf_sz;
    pattr->cbk = NULL;
    pattr->cbk_arg = NULL;

    ret = bsp_eicc_chn_open(&ctrl->s_handle, &attr);
    if (ret) {
        msg_err("bsp_eicc_chn_open %d error ret=%x\n", pattr->chnid, ret);
        msg_eicc_buf_free(pattr->size, pattr->va, pattr->pa);
        return -1;
    }
    ctrl->s_max_pkt_len = eicc_cfg->smsg_sz;

    return 0;
}

static int msg_eicc_rchan_open(struct msg_eicc_cfg *eicc_cfg, struct cmsg_ctrl *ctrl)
{
    int ret;
    void *addr = NULL;
    dma_addr_t dma_addr;
    eicc_chn_attr_t attr;
    eicc_chn_attr_t *pattr = &attr;

    addr = msg_eicc_buf_alloc(eicc_cfg->rbuf_sz, &dma_addr);
    if (addr == NULL) {
        msg_err("msg dma mem alloc error\n");
        return -1;
    }

    ret = bsp_eicc_chn_attr_init(&attr);
    if (ret) {
        msg_err("chan attr init error\n");
        return -1;
    }

    pattr->chnid = eicc_cfg->recv_chn;
    pattr->type = EICC_CHN_TYPE_RECV;
    pattr->pa = dma_addr;
    pattr->va = addr;
    pattr->size = eicc_cfg->rbuf_sz;
    pattr->cbk = msg_eicc_recv_cbk;
    pattr->cbk_arg = ctrl;

    ret = bsp_eicc_chn_open(&ctrl->r_handle, &attr);
    if (ret) {
        msg_err("bsp_eicc_chn_open %d error ret=%x\n", pattr->chnid, ret);
        msg_eicc_buf_free(pattr->size, pattr->va, pattr->pa);
        return -1;
    }

    return 0;
}

static int msg_eicc_chan_init(struct msg_eicc_cfg *eicc_cfg)
{
    int ret;
    struct cmsg_ctrl *ctrl = NULL;
    char name[0x10];

    ctrl = cmsg_get_ctrl(eicc_cfg->cid);
    if (ctrl == NULL || ctrl->status != MSG_CHN_STATUS_UNINIT) {
        return -1;
    }
    ctrl->cid = eicc_cfg->cid;
    ctrl->status = MSG_CHN_STATUS_INITING;

    spin_lock_init(&ctrl->s_lock);
    sema_init(&ctrl->worker_sem, 0);
    (void)sprintf_s(name, sizeof(name), "msg_wl%02x", ctrl->cid);
    wakeup_source_init(&ctrl->worker_wakelock, name);
    ctrl->worker = kthread_run(msg_eicc_recv_proc, ctrl, "msg%d", ctrl->cid);
    if (ctrl->worker == NULL) {
        msg_err("kthread_run fail\n");
        return MSG_ERR_ESRCH;
    }

    ret = msg_eicc_schan_open(eicc_cfg, ctrl);
    if (ret) {
        return ret;
    }
    ret = msg_eicc_rchan_open(eicc_cfg, ctrl);
    if (ret) {
        return ret;
    }
    ctrl->status = MSG_CHN_STATUS_INITED;
    return 0;
}
static int cmsg_dummy_probe(struct platform_device *pdev)
{
    of_dma_configure(&pdev->dev, NULL);
    dma_set_mask_and_coherent(&pdev->dev, ~(0x1FULL)); // 32 bytes align
    return 0;
}
static struct platform_driver g_cmsg_plat_driver = {
    .probe = cmsg_dummy_probe,
    .driver =
        {
            .name = "cmsg_plat",
            .owner = THIS_MODULE,
        },
};

int msg_crosscore_init(void)
{
    int i;
    int ret;
    ret = platform_device_register(&g_cmsg_plat_device);
    if (ret) {
        return ret;
    }
    ret = platform_driver_register(&g_cmsg_plat_driver);
    if (ret) {
        platform_device_unregister(&g_cmsg_plat_device);
        return ret;
    }
    for (i = 0; i < sizeof(g_msg_eicc_cfg) / sizeof(g_msg_eicc_cfg[0]); i++) {
        ret = msg_eicc_chan_init(&g_msg_eicc_cfg[i]);
        if (ret) {
            msg_err("msg_eicc_chan_init %d error ret=%x\n", i, ret);
            return ret;
        }
    }
    return 0;
}
int msg_cmsg_info(unsigned cid)
{
    struct cmsg_dbg *dbg = NULL;

    struct cmsg_ctrl *ctrl = cmsg_get_ctrl(cid);
    if (ctrl == NULL) {
        msg_always("cid search null\n");
        return 0;
    }
    dbg = &ctrl->dbg_info;
    msg_always("seq_err=%d\n", dbg->seq_err);
    msg_always("segmsg_err=%d\n", dbg->segmsg_err);
    msg_always("ioctl_err=%d\n", dbg->ioctl_err);
    msg_always("recv_err=%d\n", dbg->recv_err);
    msg_always("lenchk_err=%d\n", dbg->lenchk_err);
    msg_always("alloc_err=%d\n", dbg->alloc_err);
    msg_always("pktfw_err=%d\n", dbg->pktfw_err);
    return 0;
}
