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
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/memory.h>

#include "msg_plat.h"
#include "msg_base.h"
#include "msg_mem.h"
#include "msg_core.h"
#include "msg_fixed.h"
#include "msg_cmsg.h"

#include <mdrv_memory.h>

struct msg_ctrl g_msg_ctrl_stru;
struct mid_unit_prv g_mid_array[MSG_TOTAL_IDS_NUM];

/* 在本地查找mid */
struct mid_unit_prv *msg_get_private_midctrl(unsigned mid)
{
    unsigned gid = GET_MSG_GID(mid);
    unsigned sid = GET_MSG_SID(mid);
    struct mid_unit_prv *mid_ctrl = NULL;
    struct mid_unit_seg *mid_grpseg = NULL;

    if (gid >= MSG_GID_MAX) {
        msg_err("mid[%d] invaild\n", (int)mid);
        return NULL;
    }

    mid_grpseg = &g_msg_ctrl_stru.id_segd[gid];
    if (mid > g_msg_ctrl_stru.id_segd[gid].max_num || mid < g_msg_ctrl_stru.id_segd[gid].base_num) {
        msg_err("mid[%d] invaild\n", (int)mid);
        return NULL;
    }
    mid_ctrl = (struct mid_unit_prv *)mid_grpseg->mid_prv_base;
    return &mid_ctrl[sid];
}

struct mid_unit_sha *msg_get_shared_midctrl(unsigned mid)
{
    unsigned int gid = GET_MSG_GID(mid);
    unsigned int sid = GET_MSG_SID(mid);

    struct mid_unit_sha *mid_ctrl = NULL;
    struct mid_unit_seg *mid_grpseg = NULL;

    if (gid >= MSG_GID_MAX) {
        msg_err("mid[%d] invaild\n", (int)mid);
        return NULL;
    }
    if (!g_msg_ctrl_stru.xcore_support) {
        return NULL;
    }

    mid_grpseg = &g_msg_ctrl_stru.id_segd[gid];
    if (mid > g_msg_ctrl_stru.id_segd[gid].max_num || mid < g_msg_ctrl_stru.id_segd[gid].base_num) {
        msg_err("mid[%d] invaild\n", (int)mid);
        return NULL;
    }
    mid_ctrl = (struct mid_unit_sha *)mid_grpseg->mid_sha_base;
    return &mid_ctrl[sid];
}

/* 查找一个注册过的mid，优先查找本地，其次查找安全内存，再次查找非安全内存 */
static void msg_get_vaild_midctrl(unsigned mid, struct mid_unit_prv **ppprv_midctrl,
    struct mid_unit_sha **ppsha_midctrl)
{
    struct mid_unit_prv *pprv_midctrl = NULL;
    struct mid_unit_sha *psha_midctrl = NULL;
    pprv_midctrl = msg_get_private_midctrl(mid);
    if (pprv_midctrl != NULL && pprv_midctrl->magic == MSG_MID_PRV_REG_MAGIC) {
        *ppprv_midctrl = pprv_midctrl;
        return;
    }
    *ppprv_midctrl = NULL;
    psha_midctrl = msg_get_shared_midctrl(mid);
    if (psha_midctrl != NULL && psha_midctrl->magic == MSG_MID_SHA_REG_MAGIC) {
        *ppsha_midctrl = psha_midctrl;
        return;
    }
    *ppsha_midctrl = NULL;
    return;
}

/* 创建与task相关的task queue group，group中的queue的数目为 qid_num */
int mdrv_msg_qcreate(unsigned task_id, unsigned qid_num)
{
    int ret;
    struct task_struct *tsk = pid_task(find_vpid(task_id), PIDTYPE_PID);
    if (IS_ERR_OR_NULL(tsk)) {
        return MSG_ERR_EINVAL;
    }
    ret = msg_tqg_create(tsk, qid_num);
    if (ret) {
        msg_err("mdrv_msg_qcreate for task_id %d failed, ret = %d\n", task_id, ret);
    }
    return ret;
}

int mdrv_msg_attr_init(struct mid_attr *attr)
{
    if (attr == NULL) {
        return MSG_ERR_EINVAL;
    }
    (void)memset_s(attr, sizeof(*attr), 0, sizeof(*attr));
    attr->magic = MID_ATTR_INIT_MAGIC;

    return 0;
}

int mdrv_msg_mid_reg(unsigned mid, struct mid_attr *attr)
{
    struct task_struct *tsk = NULL;
    struct msg_tskque_grp *grp = NULL;
    struct mid_unit_prv *prv_midctrl = NULL;
    struct mid_unit_sha *sha_midctrl = NULL;

    if (attr == NULL) {
        msg_err("null ptr\n");
        return MSG_ERR_EINVAL;
    }
    if (attr->magic != MID_ATTR_INIT_MAGIC) {
        return MSG_ERR_EINVAL;
    }

    tsk = pid_task(find_vpid(attr->task_id), PIDTYPE_PID);
    if (IS_ERR_OR_NULL(tsk)) {
        return MSG_ERR_ESRCH;
    }
    // check mid vaild
    prv_midctrl = msg_get_private_midctrl(mid);
    if (prv_midctrl == NULL) {
        msg_err("mid[%d] invaild\n", (int)mid);
        return MSG_ERR_ENXIO;
    }
    if (prv_midctrl->magic == MSG_MID_PRV_REG_MAGIC) {
        if ((attr->flags & MSG_MIDREG_FLAGS_FORCEBIND) == 0) {
            msg_err("mid[%d] has been registered @%d\n", (int)mid, (int)MSG_CURRENT_CORE_ID);
            return MSG_ERR_EEXIST;
        }
    }
    // check local tsk & qid vaild
    grp = msg_tqg_search(tsk);
    if (grp == NULL) {
        msg_err("mid[%d] task=%d invaild\n", (int)mid, (int)attr->task_id);
        return MSG_ERR_EINVAL;
    }
    if (attr->task_qid >= grp->q_num) {
        msg_err("mid[%d] register qid %d failed, max %d\n", (int)mid, (int)attr->task_qid, (int)grp->q_num);
        return MSG_ERR_ECHRNG;
    }
    prv_midctrl->mid = mid;
    prv_midctrl->tsk = tsk;
    prv_midctrl->qid = attr->task_qid;
    prv_midctrl->magic = MSG_MID_PRV_REG_MAGIC;

    sha_midctrl = msg_get_shared_midctrl(mid);
    if (sha_midctrl != NULL) {
        sha_midctrl->mid = mid;
        sha_midctrl->cid = MSG_CURRENT_CORE_ID;
        sha_midctrl->magic = MSG_MID_SHA_REG_MAGIC;
        sha_midctrl->reserved = 0;
    }

    return 0;
}

int mdrv_msg_register_mid(unsigned task_id, unsigned mid, unsigned qid)
{
    struct mid_attr attr;
    if (mdrv_msg_attr_init(&attr)) {
        return -1;
    }
    attr.task_id = task_id;
    attr.task_qid = qid;
    return mdrv_msg_mid_reg(mid, &attr);
}

static int msg_info_chkfill(void *msg, unsigned send_flags, struct msg_pkt_info *pinfo, u32 *pkmsg_flag)
{
    struct msg_flag *msg_flag = NULL;

    if (msg == NULL) {
        return MSG_ERR_EINVAL;
    }

    pinfo->point = MSG_PKT_POINT_SEND;
    pinfo->dstid = MDRV_MSG_HEAD_GET_DSTID(msg);
    pinfo->srcid = MDRV_MSG_HEAD_GET_SRCID(msg);

    pinfo->len = MDRV_MSG_HEAD_GET_LEN(msg);
    if (pinfo->len == 0) {
        msg_err("mid[%d] send to mid[%d] bad len\n", (int)pinfo->srcid, (int)pinfo->dstid);
        return MSG_ERR_EINVAL;
    }
    msg_flag = (struct msg_flag *)&(MSG_HEAD_GET_FLAG(msg));
    if (send_flags & MSG_UG) {
        msg_flag->urgent = 1;
        *pkmsg_flag = 1;
    }
    /* 用户没有说不让填时间戳，也没填时间戳 我们就填一个 */
    if ((send_flags & __MSG_NO_TS) == 0 && MDRV_MSG_HEAD_GET_TS(msg) == 0) {
        MDRV_MSG_HEAD_SET_TS(msg, 0x1234);
    }
    pinfo->timestamp = MDRV_MSG_HEAD_GET_TS(msg);
    return 0;
}


int mdrv_msg_send(void *msg, unsigned send_flags)
{
    int ret ;
    struct msg_pkt_info info;
    struct msg_pkt_info *pinfo = &info;
    struct mid_unit_prv *prv_midctrl = NULL;
    struct mid_unit_sha *sha_midctrl = NULL;
    u32 kmsg_flag = 0;

    ret = msg_info_chkfill(msg, send_flags, pinfo, &kmsg_flag);
    if (ret) {
        return ret;
    }
    msg_get_vaild_midctrl(pinfo->dstid, &prv_midctrl, &sha_midctrl);
    if (prv_midctrl != NULL) {
        ret = msg_tskq_send(prv_midctrl->tsk, prv_midctrl->qid, msg, kmsg_flag);
        if (ret != 0) {
            msg_err("mid[%d] send to mid[%d] failed, tid=%d qid=%d, ret=%d\n", (int)pinfo->srcid, (int)pinfo->dstid,
                (int)prv_midctrl->tsk->pid, (int)prv_midctrl->qid, (int)ret);
            return ret;
        }
        msg_send_record(pinfo);
        msg_diag_report(pinfo);
    } else if (sha_midctrl != NULL) {
        ret = msg_crosscore_send(sha_midctrl->cid, msg);
        if (ret == 0) {
            mdrv_msg_free(pinfo->srcid, msg);
            msg_send_record(pinfo);
            msg_diag_report(pinfo);
        } else {
            msg_err("mid[%d] send to mid[%d] failed, cid=%d, ret=%d\n", (int)pinfo->srcid, (int)pinfo->dstid,
                (int)sha_midctrl->cid, (int)ret);
            ret = MSG_ERR_ENETUNREACH;
        }
    } else {
        msg_err("mid[%d] send to mid[%d] faild, dst not found\n", (int)pinfo->srcid, (int)pinfo->dstid);
        return MSG_ERR_EHOSTUNREACH;
    }
    return ret;
}

int mdrv_msg_recv(unsigned qid, void **msg, unsigned time_out)
{
    int ret;
    struct msg_pkt_info info;
    struct msg_pkt_info *pinfo = &info;

    if (msg == NULL) {
        msg_err("null ptr\n");
        return MSG_ERR_EINVAL;
    }

    ret = msg_tskq_recv(qid, msg, time_out);
    if (ret == 0) {
        pinfo->point = MSG_PKT_POINT_RECV;
        pinfo->dstid = MDRV_MSG_HEAD_GET_DSTID(*msg);
        pinfo->srcid = MDRV_MSG_HEAD_GET_SRCID(*msg);
        pinfo->len = MDRV_MSG_HEAD_GET_LEN(*msg);
        pinfo->timestamp = MDRV_MSG_HEAD_GET_TS(*msg);
        msg_recv_record(pinfo);
        msg_diag_report(pinfo);
        return 0;
    } else if (ret == MSG_ERR_ETIME) {
        return MDRV_MSG_ERR_QUEUE_EMPTY;
    } else {
        msg_err("recv from qid=%d fail ret=%d\n", qid, ret);
        return ret;
    }
}

int mdrv_msg_num_get(unsigned task_id, unsigned qid)
{
    struct task_struct *tsk = NULL;
    tsk = pid_task(find_vpid(task_id), PIDTYPE_PID);
    if (IS_ERR_OR_NULL(tsk)) {
        return MSG_ERR_EINVAL;
    }
    return msg_tskq_get_num(tsk, qid);
}

int mdrv_event_send(unsigned task_id, unsigned event)
{
    struct task_struct *tsk = NULL;
    tsk = pid_task(find_vpid(task_id), PIDTYPE_PID);
    if (event == 0 || IS_ERR_OR_NULL(tsk)) {
        return MSG_ERR_EINVAL;
    }
    return msg_evt_send(tsk, event);
}
int mdrv_event_recv(unsigned event, unsigned opt, unsigned time_out, unsigned *recv_evt)
{
    return msg_evt_recv(event, opt, time_out, recv_evt);
}

int mdrv_event_vrecv(unsigned event, unsigned opt, unsigned time_out, unsigned *recv_evt)
{
    int ret;
    if (event == 0 || recv_evt == NULL) {
        return MSG_ERR_EINVAL;
    }
    if (opt != MDRV_EVENT_ANY) {
        return MSG_ERR_EINVAL;
    }
    ret = msg_evt_vrecv(event, opt, time_out, recv_evt);
    if (ret == 0 && *recv_evt == 0) {
        *recv_evt = PS_OSA_RESERVED_EVENT;
    }
    return ret;
}
/*lint -save -e762*/
int mdrv_msg_vsend(void *msg, unsigned int flag) __attribute__((alias("mdrv_msg_send")));
int mdrv_event_vsend(unsigned int taskId, unsigned int event) __attribute__((alias("mdrv_event_send")));
/*lint -restore*/
static int msg_unit_seg_init(struct msg_ctrl *ctrl)
{
    struct mid_unit_seg *mid_seg = NULL;

    mid_seg = &(ctrl->id_segd[MSG_GID_PS]);
    mid_seg->base_num = PS_MID_START;
    mid_seg->max_num = PS_MID_END;
    mid_seg->total_num = PS_MID_TOTAL_NUM;
    mid_seg->mid_prv_base = ctrl->mid_prv_base;

    if (ctrl->xcore_support) {
        mid_seg->mid_sha_base = ctrl->mid_sha_base;
    }

    mid_seg = &(ctrl->id_segd[MSG_GID_DRV]);
    mid_seg->base_num = DRV_MID_START;
    mid_seg->max_num = DRV_MID_END;
    mid_seg->total_num = DRV_MID_TOTAL_NUM;
    mid_seg->mid_prv_base = (void *)((struct mid_unit_prv *)ctrl->mid_prv_base + PS_MID_TOTAL_NUM);
    if (ctrl->xcore_support) {
        mid_seg->mid_sha_base = (void *)((struct mid_unit_sha *)ctrl->mid_sha_base + PS_MID_TOTAL_NUM);
    }

    mid_seg = &(ctrl->id_segd[MSG_GID_PHY]);
    mid_seg->base_num = PHY_MID_START;
    mid_seg->max_num = PHY_MID_END;
    mid_seg->total_num = PHY_MID_TOTAL_NUM;
    mid_seg->mid_prv_base = (void *)((struct mid_unit_prv *)ctrl->mid_prv_base + PS_MID_TOTAL_NUM + DRV_MID_TOTAL_NUM);
    if (ctrl->xcore_support) {
        mid_seg->mid_sha_base =
            (void *)((struct mid_unit_sha *)ctrl->mid_sha_base + PS_MID_TOTAL_NUM + DRV_MID_TOTAL_NUM);
    }
    return 0;
}

int msg_core_init(void)
{
    int ret;
    unsigned phy_size;
    phy_addr_t phy_addr;
    struct msg_ctrl *ctrl = NULL;

    ctrl = &g_msg_ctrl_stru;
    ctrl->mid_prv_base = (void *)g_mid_array;
    ctrl->mid_sha_base = mdrv_mem_share_get("usshm_msg_mid", &phy_addr, &phy_size, SHM_UNSEC);
    if (ctrl->mid_sha_base == NULL) {
        msg_err("error: share mem is not support!!\n");
    } else {
        if (MSG_TOTAL_IDS_NUM * sizeof(struct mid_unit_sha) > phy_size) {
            msg_err("error: share mem is not enough!!\n");
        } else {
            ctrl->xcore_support = 1;
            msg_always("phy_addr = 0x%llx, phy_size = 0x%x\n", (long long)phy_addr, phy_size);
            msg_always("ctrl->mid_sha_base = %p!\n", ctrl->mid_sha_base);
        }
    }

    ret = msg_unit_seg_init(ctrl);
    if (ret != 0) {
        return -1;
    }
    return ret;
}


int bsp_msg_init(void)
{
    int ret;
    ret = msg_base_init();
    if (ret) {
        msg_err("msg_base_init failed\n");
        return -1;
    }

    ret = msg_core_init();
    if (ret) {
        msg_err("msg_core_init failed!\n");
        return ret;
    }

    ret = msg_crosscore_init();
    if (ret) {
        msg_err("msg_crosscore_init failed\n");
        return -1;
    }
    msg_mntn_init();

    return 0;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
subsys_initcall(bsp_msg_init);
#endif

int msg_ctrl_info(void)
{
    int i;
    struct msg_ctrl *ctrl = &g_msg_ctrl_stru;
    struct mid_unit_seg *mid_grpseg = NULL;

    msg_always("ctrl->mid_prv_base = %p\n", ctrl->mid_prv_base);
    msg_always("ctrl->mid_sha_base = %p\n", ctrl->mid_sha_base);
    msg_always("ctrl->xcore_support = %d\n", ctrl->xcore_support);
    mid_grpseg = ctrl->id_segd;
    for (i = 0; i < MSG_GID_MAX; i++) {
        msg_always("gid = %d\n", i);
        msg_always("mid_grpseg->mid_prv_base = %p\n", mid_grpseg[i].mid_prv_base);
        msg_always("mid_grpseg->mid_sha_base = %p\n", mid_grpseg[i].mid_sha_base);
        msg_always("ctrl->base_num = %d\n", mid_grpseg[i].base_num);
        msg_always("ctrl->max_num = %d\n", mid_grpseg[i].max_num);
        msg_always("ctrl->total_num = %d\n", mid_grpseg[i].total_num);
    }
    return 0;
}

int msg_mid_info(unsigned mid)
{
    struct mid_unit_prv *prv_midctrl = NULL;
    struct mid_unit_sha *sha_midctrl = NULL;

    prv_midctrl = msg_get_private_midctrl(mid);
    if (prv_midctrl != NULL) {
        msg_always("magic: 0x%x\n", (unsigned)prv_midctrl->magic);
        msg_always("lc_mask: 0x%x\n", (unsigned)prv_midctrl->lc_mask);
        msg_always("flags: 0x%x\n", (unsigned)prv_midctrl->flags);
        if (IS_ERR_OR_NULL(prv_midctrl->tsk)) {
            msg_always("tid: invaild\n");
        } else {
            msg_always("tid: 0x%x\n", (unsigned)prv_midctrl->tsk->pid);
        }
        msg_always("qid: 0x%x\n", (unsigned)prv_midctrl->qid);
        msg_always("record.recv_cnt: 0x%x\n", (unsigned)prv_midctrl->record.recv_cnt);
        msg_always("record.send_cnt: 0x%x\n", (unsigned)prv_midctrl->record.send_cnt);
    }
    sha_midctrl = msg_get_shared_midctrl(mid);
    if (sha_midctrl != NULL) {
        msg_always("magic: 0x%x\n", (unsigned)sha_midctrl->magic);
        msg_always("cid: 0x%x\n", (unsigned)sha_midctrl->cid);
    }

    return 0;
}
