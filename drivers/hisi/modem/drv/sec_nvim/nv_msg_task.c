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
 */

#include <securec.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <osl_thread.h>
#include <mdrv_msg.h>
#include <msg_id.h>
#include "nv_msg.h"
#include "nv_comm.h"
#include "nv_debug.h"

#define THIS_MODU mod_nv

#define NV_CPMSG_QID_NUM 1
#define NV_CPMSG_QID 0

struct nv_tsk_msg {
    MDRV_MSG_HEAD
    nv_icc_msg_t icc_cnf;
};

u32 bsp_nvm_ccore_msg_cb(u32 result, u32 sn)
{
    s32 ret;
    struct nv_tsk_msg *msg = NULL;
    struct nv_global_ctrl_info_stru *nv_ctrl = nv_get_ctrl_info();

    msg = (struct nv_tsk_msg *)mdrv_msg_alloc(DRV_MID_NV_APSS, sizeof(nv_icc_msg_t), MSG_DEFAULT);
    if (msg == NULL) {
        nv_printf("nv cpmsg alloc failed\n");
        return BSP_ERR_NV_SEND_CPMSG_ERR;
    }

    MDRV_MSG_HEAD_FILL(msg, DRV_MID_NV_APSS, DRV_MID_NV_TSP, sizeof(nv_icc_msg_t));

    msg->icc_cnf.msg_type = NV_ICC_CNF;
    msg->icc_cnf.send_slice = bsp_get_slice_value();
    msg->icc_cnf.sn = sn;
    msg->icc_cnf.ret = result;

    ret = mdrv_msg_send(msg, MSG_DEFAULT);
    if (ret) {
        mdrv_msg_free(DRV_MID_NV_APSS, msg);
        nv_printf("nv cpmsg send failed, ret:%d\n", ret);
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    __pm_relax(nv_ctrl->wake_lock);
#else
    //lint -save -e455
    __pm_relax(&(nv_ctrl->wake_lock));
#endif
    g_msg_ctrl.icc_cb_reply++;
    return (u32)ret;
    //lint -restore
}

static int nv_cpmsg_thread(void *data)
{
    s32 ret;
    void *temp = NULL;
    void *msg_get = NULL;
    struct nv_tsk_msg *msg = NULL;
    nv_icc_msg_t *icc_cnf = NULL;
    struct nv_global_ctrl_info_stru *nv_ctrl = nv_get_ctrl_info();

    /* make pclint happy */
    temp = data;
    data = temp;

    for (;;) {
        ret = mdrv_msg_recv(NV_CPMSG_QID, &msg_get, MDRV_MSG_WAIT_FOREVER);
        if (ret) {
            nv_printf("nv cpmsg recv failed, ret:%d\n", ret);
            continue;
        }

        if (msg_get == NULL) {
            nv_printf("nv cpmsg recv NULL\n");
            continue;
        }

        g_msg_ctrl.icc_cb_count++;
        msg = (struct nv_tsk_msg *)msg_get;
        icc_cnf = &msg->icc_cnf;
        switch (icc_cnf->msg_type) {
            case NV_ICC_REQ_FLUSH:
            case NV_ICC_REQ_FLUSH_RWNV:
            case NV_ICC_REQ_LOAD_BACKUP:
            case NV_ICC_REQ_LOAD_CARRIER_RESUM:
            case NV_ICC_REQ_LOAD_CARRIER_CUST:
            case NV_ICC_REQ_UPDATE_DEFAULT:
            case NV_ICC_REQ_UPDATE_BACKUP:
            case NV_ICC_REQ_FACTORY_RESET:
            case NV_ICC_REQ_UPGRADE_FLUSH:
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
                __pm_stay_awake(nv_ctrl->wake_lock);
#else
                __pm_stay_awake(&(nv_ctrl->wake_lock));
#endif
                ret = (s32)nv_send_rmsg(icc_cnf->msg_type, icc_cnf->sn);
                break;
            case NV_ICC_REQ_FLUSH_RDWR_ASYNC:
                ret = (s32)nv_send_rmsg(icc_cnf->msg_type, icc_cnf->sn);
                break;
            default:
                nv_printf("invalid nv icc msg type %d \n", icc_cnf->msg_type);
        }

        if (ret) {
            nv_printf("hande cpmsg %d failed, ret:%d\n", icc_cnf->msg_type, ret);
        }
        nv_debug_record(NV_DEBUG_RECEIVE_ICC | (icc_cnf->msg_type << 0x10));

        mdrv_msg_free(DRV_MID_NV_APSS, (void *)msg);
    }

    return 0;
}

#define NV_CPMSG_TSK_PRIORITY 15
u32 nv_cpmsg_chan_init(void)
{
    s32 ret;
    struct task_struct *tsk = NULL;
    struct sched_param sch_para;

    sch_para.sched_priority = NV_CPMSG_TSK_PRIORITY;

    tsk = kthread_create(nv_cpmsg_thread, NULL, "nv_cpmsg");
    if (IS_ERR(tsk)) {
        nv_record("nv cpmsg task init failed\n");
        return NV_ERROR;
    }

    if (sched_setscheduler(tsk, SCHED_FIFO, &sch_para) != 0) {
        nv_printf("nv cpmsg task set priority failed\n");
        return NV_ERROR;
    }

    ret = mdrv_msg_qcreate(tsk->pid, NV_CPMSG_QID_NUM);
    if (ret) {
        nv_printf("nv cpmsg qid create failed, ret:%d\n", ret);
        return (u32)ret;
    }

    ret = mdrv_msg_register_mid(tsk->pid, DRV_MID_NV_APSS, NV_CPMSG_QID);
    if (ret) {
        nv_printf("nv cpmsg register mid failed, ret:%d\n", ret);
        return (u32)ret;
    }

    wake_up_process(tsk);
    return NV_OK;
}
