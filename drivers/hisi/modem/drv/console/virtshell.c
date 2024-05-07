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

#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <osl_malloc.h>
#include <bsp_icc.h>
#include <bsp_slice.h>
#include <bsp_print.h>
#include "uart_dev.h"
#include "ringbuffer.h"
#include "console.h"
#include "con_platform.h"
#include "virtshell.h"
#include "securec.h"

#undef THIS_MODU
#define THIS_MODU mod_cshell

struct virtshl_module_ctr g_virtshl_ctrl_info;
/*lint --e{64}*/ /* 64:list_for_each_entry */
int virtshl_ctrl_cmd_process(unsigned char *cmd, unsigned int size);
/*lint -e715*/
int virtshl_send_tsk_func(void *data)
{
    static unsigned char tmpbuf[VIRTSHL_MAX_PACKAGE_LEN] = {
        0,
    };
    unsigned char *outbuf = tmpbuf;
    int retval;
    int len = 0;

    while (!kthread_should_stop()) {
    wait_for_space:
        down(&g_virtshl_ctrl_info.send_tsk_sem);

        while (!list_empty(&g_virtshl_ctrl_info.slist)) {
            struct console *con = NULL;
            struct virtshl_icc_l16id *chn = NULL;
            unsigned long slist_lock_flg = 0;

            spin_lock_irqsave(&g_virtshl_ctrl_info.slist_lock, slist_lock_flg); /*lint !e550*/
            chn = list_first_entry(&g_virtshl_ctrl_info.slist, struct virtshl_icc_l16id, slist);
                /*lint !e826*/ /*lint !e838*/
            con = chn->con;    /*lint !e838*/

            list_del_init(&chn->slist);
            spin_unlock_irqrestore(&g_virtshl_ctrl_info.slist_lock, slist_lock_flg);

            /*lint -e506*/
            do {
                outbuf = tmpbuf;

                if (con == NULL)
                    break;
                else if (con->coreid != CON_CURCORE)
                    len = con->get(con, outbuf, VIRTSHL_MAX_PACKAGE_LEN);
                else
                    len = con->tx(con, outbuf, VIRTSHL_MAX_PACKAGE_LEN);

                if (len <= 0)
                    break;

                retval = bsp_icc_send(chn->target_coreid, chn->chn_id, outbuf, (unsigned int)len);
                if (retval < len) {
                    goto wait_for_space;
                } else {
                    /* everything is ok. */
                }
            } while (1);
            /*lint +e506*/
        }
    }

    return 0; /*lint !e527*/
} /*lint +e715*/

void virtshl_start_tx(struct console *con)
{
    struct virtshl_icc_l16id *chn = con->icchn;
    unsigned long slist_lock_flg = 0;

    spin_lock_irqsave(&g_virtshl_ctrl_info.slist_lock, slist_lock_flg); /*lint !e550*/
    if (list_empty_careful(&chn->slist))
        list_add_tail(&chn->slist, &g_virtshl_ctrl_info.slist);
    spin_unlock_irqrestore(&g_virtshl_ctrl_info.slist_lock, slist_lock_flg);

    up(&g_virtshl_ctrl_info.send_tsk_sem);
}

int virtshl_start_shell(struct console *con)
{
    struct virtshl_icc_l16id *chn = con->icchn;
    unsigned long slist_lock_flg = 0;

    if (chn == NULL)
        return -1;

    spin_lock_irqsave(&g_virtshl_ctrl_info.slist_lock, slist_lock_flg); /*lint !e550*/
    if (list_empty_careful(&chn->slist))
        list_add_tail(&chn->slist, &g_virtshl_ctrl_info.slist);
    spin_unlock_irqrestore(&g_virtshl_ctrl_info.slist_lock, slist_lock_flg);

    up(&g_virtshl_ctrl_info.send_tsk_sem);

    return 0;
}
EXPORT_SYMBOL(virtshl_start_shell);

/*lint -e715*/
int virtshl_recv_tsk_func(void *data)
{
    static unsigned char tmpbuf[VIRTSHL_MAX_PACKAGE_LEN] = {
        0,
    };

    while (!kthread_should_stop()) {
        down(&g_virtshl_ctrl_info.recv_tsk_sem);

        while (!list_empty(&g_virtshl_ctrl_info.rlist)) {
            struct console *con = NULL;
            struct virtshl_icc_l16id *chn = NULL;
            int icc_return_sz;
            unsigned long rlist_lock_flg = 0;

            spin_lock_irqsave(&g_virtshl_ctrl_info.rlist_lock, rlist_lock_flg); /*lint !e550*/
            chn = list_first_entry(&g_virtshl_ctrl_info.rlist, struct virtshl_icc_l16id, rlist);
                /*lint !e826*/ /*lint !e838*/
            con = chn->con;    /*lint !e838*/

            list_del_init(&chn->rlist);
            spin_unlock_irqrestore(&g_virtshl_ctrl_info.rlist_lock, rlist_lock_flg);

            while (1) {
                icc_return_sz = bsp_icc_read(chn->chn_id, tmpbuf, VIRTSHL_MAX_PACKAGE_LEN);
                if (icc_return_sz <= 0 || icc_return_sz > VIRTSHL_MAX_PACKAGE_LEN) {
                    /* TODO: some unknown error happens, take it */
                    break;
                }

                if (con == NULL) {
                    virtshl_ctrl_cmd_process(tmpbuf, (unsigned int)icc_return_sz);
                } else if (con->coreid != CON_CURCORE) {
                    con->put(con, tmpbuf, (unsigned int)icc_return_sz);
                    if (con->start_tx != NULL)
                        con->start_tx(con);
                } else {
                    (void)con->rx(con, tmpbuf, (unsigned int)icc_return_sz);
                }
            }
        }
    }

    return 0; /*lint !e527*/
} /*lint +e715*/
/*lint -e715*/
int virtshl_icc_readcb(unsigned int chn_id, unsigned int len, void *context)
{
    struct virtshl_icc_l16id *chn = (struct virtshl_icc_l16id *)context;
    unsigned long rlist_lock_flg = 0;

    spin_lock_irqsave(&g_virtshl_ctrl_info.rlist_lock, rlist_lock_flg); /*lint !e550*/
    if (list_empty_careful(&chn->rlist)) {
        list_add_tail(&chn->rlist, &g_virtshl_ctrl_info.rlist);
    }
    spin_unlock_irqrestore(&g_virtshl_ctrl_info.rlist_lock, rlist_lock_flg);

    up(&g_virtshl_ctrl_info.recv_tsk_sem);

    return 0;
} /*lint +e715*/

static unsigned int virtshl_get_nxt_id(struct virtshl_icc_h16id *hchn)
{
    unsigned int nxtid;

    nxtid = hchn->next_l16id;

    /* TODO: Think about using atomic integer */
    hchn->next_l16id++;
    if (hchn->next_l16id >= 20) /* 20 is the maxmum number of icc sub channel */
        hchn->next_l16id = 1;

    return nxtid;
}

/* cov_verified_start */
void virtshl_gen_con_name(char *namebuf, unsigned int coreid, unsigned int chnid)
{
    unsigned int i;
    char tmpch;

    for (i = 0; i < sizeof(unsigned int); i++) { /*lint !e838*/
        tmpch = (char)((coreid >> (i * 0x4)) & 0xF);
        *namebuf = tmpch <= 0x9 ? (tmpch + '0') : (tmpch - 0xA + 'A'); /*lint !e834*/
        namebuf++;
    }

    for (i = 0; i < (int)sizeof(unsigned int); i++) {
        tmpch = (char)((chnid >> (i * 0x4)) & 0xF);
        *namebuf = tmpch <= 0x9 ? (tmpch + '0') : (tmpch - 0xA + 'A'); /*lint !e834*/
        namebuf++;
    }

    *namebuf = 0;
}
/* cov_verified_stop */

static void virtshl_cmd_send(struct virtshl_ctrl_cmd *cmd, int type, int value)
{
    int ret;

    if (cmd == NULL) {
        return;
    }

    switch (type) {
        case VCMD_CONNECT:
            cmd->acknowledge = value ? VACK_FAIL : VACK_SUCC;
            cmd->cmd_type = VCMD_CONEC_ACK;
            cmd->target_coreid = cmd->local_coreid;
            cmd->local_coreid = CON_CURCORE;
            break;
        case VCMD_DISCONNECT:
            cmd->cmd_type = VCMD_DISCONEC_ACK;
            cmd->target_coreid = cmd->local_coreid;;
            cmd->local_coreid = CON_CURCORE;
            cmd->acknowledge = VACK_SUCC;
            break;
        default:
            bsp_err("virtshl cmd type err:%d", type);
            return;
    }
    do {
        ret = bsp_icc_send(cmd->target_coreid, ((cmd->entire_iccid & 0xFFFF0000) | 0), (unsigned char *)cmd,
            sizeof(struct virtshl_ctrl_cmd));
        if (ret != (int)sizeof(struct virtshl_ctrl_cmd)) {
            bsp_err("virtshl cmd->type:%d, bsp_icc_send fail:%d", type, ret);
            osl_task_delay(1);
        }
    } while (ret != (int)sizeof(struct virtshl_ctrl_cmd));
    return;
}

/*
 * Input must be a complete command.
 */
/*lint -e413*/
int virtshl_ctrl_cmd_process(unsigned char *cmd, unsigned int size)
{
    struct virtshl_ctrl_cmd *ctrl_cmd = (struct virtshl_ctrl_cmd *)cmd; /*lint !e826*/
    struct console *con = NULL;
    struct virtshl_icc_l16id *subchn = NULL;
    unsigned char *cmd_base = NULL, *log_base = NULL;
    int retval = 0;
    int h16id_index;

    if (memcmp(ctrl_cmd->cmd_magic, VIRTSHL_CMD_MAGIC, VIRTSHL_CMD_MAGIC_LEN)) /*lint !e747*/
        return -VIRTSHL_ERR_NOT_CTRLCMD;

    for (h16id_index = 0; h16id_index < VIRTSHL_MAX_H16ID_NUM; h16id_index++) { /*lint !e838*/
        if (g_virtshl_ctrl_info.h16ids[h16id_index].h16id == (ctrl_cmd->entire_iccid >> 0x10))
            break;
    }
    if (h16id_index >= VIRTSHL_MAX_H16ID_NUM) {
        return -1;
    }

    switch (ctrl_cmd->cmd_type) {
        /* cov_verified_start */
        case VCMD_CONNECT:
            con = (struct console *)osl_malloc(sizeof(struct console) + sizeof(struct virtshl_icc_l16id) +
                                               (1U << LOG_BUF_SHIFT) + (1U << CMD_BUF_SHIFT)); /*lint !e747*/
            if (con == NULL) {
                retval = -1;
                virtshl_cmd_send(ctrl_cmd, VCMD_CONNECT, retval);
                break;
            }
            subchn = (struct virtshl_icc_l16id *)(con + 1);
            cmd_base = (void *)(subchn + 1);
            log_base = cmd_base + (1U << CMD_BUF_SHIFT);

            virtshl_gen_con_name(con->name, ctrl_cmd->target_coreid, ctrl_cmd->entire_iccid);
            con->coreid = ctrl_cmd->target_coreid;
            con->start_tx = virtshl_start_tx;
            con->start_shell = shell_start;
            con->data = NULL;
            con->inflags = CON_INFLG_ICC;
            con->icchn = subchn;
            con_ring_init(&con->cmdbuf, cmd_base, (1U << CMD_BUF_SHIFT)); /*lint !e747*/
            con_ring_init(&con->logbuf, log_base, (1U << LOG_BUF_SHIFT)); /*lint !e747*/

            subchn->target_coreid = ctrl_cmd->local_coreid;
            subchn->owner_coreid = ctrl_cmd->local_coreid;
            subchn->chn_id = ctrl_cmd->entire_iccid;
            subchn->con = con;
            INIT_LIST_HEAD(&subchn->subchn_list);
            INIT_LIST_HEAD(&subchn->slist);
            INIT_LIST_HEAD(&subchn->rlist);

            retval = bsp_icc_event_register(subchn->chn_id, virtshl_icc_readcb, subchn, NULL, NULL);
            retval += con_register(con);
            if (retval) {
                retval = -1;
                osl_free(con);
                virtshl_cmd_send(ctrl_cmd, VCMD_CONNECT, retval);
                break;
            }

            list_add(&subchn->subchn_list, &g_virtshl_ctrl_info.h16ids[h16id_index].virtchn_list);
            virtshl_cmd_send(ctrl_cmd, VCMD_CONNECT, retval);
            break;

        case VCMD_DISCONNECT:
            list_for_each_entry(subchn, &g_virtshl_ctrl_info.h16ids[h16id_index].virtchn_list, subchn_list) /*lint !e826*/
            {
                if (subchn->owner_coreid == CON_CURCORE && subchn->chn_id == ctrl_cmd->entire_iccid)
                    break;
            }
            if (&subchn->subchn_list == &g_virtshl_ctrl_info.h16ids[h16id_index].virtchn_list) {
                retval = -1;
                break;
            }

            list_del_init(&subchn->subchn_list);

            (void)bsp_icc_event_unregister(ctrl_cmd->entire_iccid);

            con = subchn->con;
            con->icchn = NULL;
            subchn->con = NULL;

            if (con->predecessor != NULL) {
                con_enable(con->predecessor);
                ((struct uart_ldisc *)(con->predecessor->data))->bind_con = con->predecessor;
            }

            con_unregister(con);
            if (con->inflags & CON_INFLG_CONSTANT)
                osl_free(subchn);
            else
                osl_free(con);
            virtshl_cmd_send(ctrl_cmd, VCMD_DISCONNECT, retval);
            break;

            /* cov_verified_stop */
        case VCMD_CONEC_ACK:
            if (ctrl_cmd->acknowledge == VACK_FAIL) {
                list_for_each_entry(subchn, &g_virtshl_ctrl_info.h16ids[h16id_index].virtchn_list,
                                    subchn_list) /*lint !e826*/
                {
                    if (subchn->owner_coreid == CON_CURCORE && subchn->chn_id == ctrl_cmd->entire_iccid)
                        break;
                }
                if (&subchn->subchn_list == &g_virtshl_ctrl_info.h16ids[h16id_index].virtchn_list) {
                    retval = -1;
                    break;
                }

                list_del_init(&subchn->subchn_list);

                (void)bsp_icc_event_unregister(subchn->chn_id);

                con = subchn->con;
                con->icchn = NULL;
                subchn->con = NULL;
                con_unregister(con);

                if (con->predecessor != NULL)
                    con_enable(con->predecessor);

                if (con->inflags & CON_INFLG_CONSTANT)
                    osl_free(subchn);
                else
                    osl_free(con);
                break;
            }

            break;
        /* cov_verified_start */
        case VCMD_DISCONEC_ACK:
            list_for_each_entry(subchn, &g_virtshl_ctrl_info.h16ids[h16id_index].virtchn_list, subchn_list) /*lint !e826*/
            {
                if (ctrl_cmd->local_coreid == subchn->owner_coreid && subchn->chn_id == ctrl_cmd->entire_iccid)
                    break;
            }
            if (&subchn->subchn_list == &g_virtshl_ctrl_info.h16ids[h16id_index].virtchn_list) {
                retval = -1;
                break;
            }

            (void)bsp_icc_event_unregister(subchn->chn_id);
            list_del_init(&subchn->subchn_list);
            osl_free(subchn->con);

            break;
        /* cov_verified_stop */
        default:
            break;
    }

    return retval;
} /*lint !e715*/
/*lint +e413*/
EXPORT_SYMBOL(virtshl_ctrl_cmd_process);

unsigned int establish_icc_chn(unsigned int target_core, struct console *con)
{
    unsigned int hid, lid;
    int i;
    int retval;
    struct virtshl_icc_h16id *hchn = g_virtshl_ctrl_info.h16ids;
    struct virtshl_icc_l16id *lchn = NULL;
    struct virtshl_ctrl_cmd ctrl_cmd;
    errno_t err;
    /*
     * find an available sub channel.
     */
    for (i = 0; i < (int)g_virtshl_ctrl_info.h16id_num; i++) { /*lint !e838*/
        hchn = &g_virtshl_ctrl_info.h16ids[i];
        if (hchn->local_coreid == CON_CURCORE && target_core == hchn->target_coreid)
            break;
    }
    if (i >= (int)g_virtshl_ctrl_info.h16id_num) {
        printk(KERN_ERR "cshell establish find sub chan fail, i:%d > h16id_num:%d\n", i, g_virtshl_ctrl_info.h16id_num);
        return ~0;
    }

    hid = hchn->h16id;
    lid = virtshl_get_nxt_id(hchn);

    lchn = (struct virtshl_icc_l16id *)osl_malloc(sizeof(struct virtshl_icc_l16id)); /*lint !e747*/ /*lint !e838*/
    if (lchn == NULL) {
        printk(KERN_ERR "cshell establish icc osl_malloc fail\n");
        return ~0;
    }

    con->predecessor = NULL;
    con->icchn = lchn;

    lchn->target_coreid = target_core;
    lchn->owner_coreid = CON_CURCORE;
    lchn->chn_id = ((hid << 0x10) | lid);
    lchn->hid_idx = (unsigned int)i;
    lchn->con = con;
    INIT_LIST_HEAD(&lchn->subchn_list);
    INIT_LIST_HEAD(&lchn->slist);
    INIT_LIST_HEAD(&lchn->rlist);

    list_add(&lchn->subchn_list, &hchn->virtchn_list);

    if (bsp_icc_event_register(lchn->chn_id, virtshl_icc_readcb, lchn, NULL, NULL))
        return ~0;

    /*
     * Notify the target core to establish console.
     * And we must succeed to send out this message.
     */
    err = memcpy_s(ctrl_cmd.cmd_magic, VIRTSHL_CMD_MAGIC_LEN, VIRTSHL_CMD_MAGIC, VIRTSHL_CMD_MAGIC_LEN); /*lint !e747*/
    if (err != EOK) {
        return err;
    }
    ctrl_cmd.cmd_type = VCMD_CONNECT;
    ctrl_cmd.local_coreid = CON_CURCORE;
    ctrl_cmd.target_coreid = target_core;
    ctrl_cmd.entire_iccid = lchn->chn_id;
    ctrl_cmd.acknowledge = 0;

    do {
        retval = bsp_icc_send(ctrl_cmd.target_coreid, (hid << 0x10) | 0, (unsigned char *)&ctrl_cmd,
                              sizeof(ctrl_cmd)); /*lint !e747*/
        if (retval != (int)sizeof(ctrl_cmd)) {
            printk(KERN_ERR "cshell establish icc channel fail\n");
            break;
        }
    } while (retval != (int)sizeof(ctrl_cmd));

    return 0;
}
EXPORT_SYMBOL(establish_icc_chn);

#ifdef CONFIG_NR_CSHELL
unsigned int g_icc_h16id[VIRTSHL_MAX_H16ID_NUM] = { ICC_CHN_CSHELL, ICC_CHN_NRCCPU_APCPU_CSHELL };
unsigned int g_targetcore[VIRTSHL_MAX_H16ID_NUM] = { CON_CCORE, CON_NR_CCORE };
#else
unsigned int g_icc_h16id[VIRTSHL_MAX_H16ID_NUM] = { ICC_CHN_CSHELL };
unsigned int g_targetcore[VIRTSHL_MAX_H16ID_NUM] = { CON_CCORE };
#endif

int virtshl_init(void)
{
    int retval = 0;
    int i;
    unsigned int buf_size;
    void *buf_tail_nxt = NULL;
    struct virtshl_icc_l16id *ctrl_chn = NULL;

    INIT_LIST_HEAD(&g_virtshl_ctrl_info.slist);
    INIT_LIST_HEAD(&g_virtshl_ctrl_info.rlist);
    sema_init(&g_virtshl_ctrl_info.send_tsk_sem, 0);
    sema_init(&g_virtshl_ctrl_info.recv_tsk_sem, 0);
    spin_lock_init(&g_virtshl_ctrl_info.slist_lock);
    spin_lock_init(&g_virtshl_ctrl_info.rlist_lock);

    buf_size = sizeof(struct virtshl_icc_l16id); /*lint !e838*/
    buf_size += sizeof(struct virtshl_icc_h16id);
    buf_size *= VIRTSHL_MAX_H16ID_NUM;
    buf_tail_nxt = osl_malloc(buf_size); /*lint !e838*/
    if (buf_tail_nxt == NULL) {
        retval = -VIRTSHL_ERR_NO_MEM;
        goto no_mem_out;
    }
    g_virtshl_ctrl_info.h16ids = (struct virtshl_icc_h16id *)buf_tail_nxt;
    g_virtshl_ctrl_info.h16id_num = VIRTSHL_MAX_H16ID_NUM;
    ctrl_chn = (struct virtshl_icc_l16id *)(g_virtshl_ctrl_info.h16ids + VIRTSHL_MAX_H16ID_NUM); /*lint !e826*/

    for (i = 0; i < VIRTSHL_MAX_H16ID_NUM; i++) {
        g_virtshl_ctrl_info.h16ids[i].target_coreid = g_targetcore[i];
        g_virtshl_ctrl_info.h16ids[i].local_coreid = CON_CURCORE;
        g_virtshl_ctrl_info.h16ids[i].h16id = g_icc_h16id[i];
        g_virtshl_ctrl_info.h16ids[i].next_l16id = 1;
        INIT_LIST_HEAD(&g_virtshl_ctrl_info.h16ids[i].virtchn_list);

        ctrl_chn[i].target_coreid = g_virtshl_ctrl_info.h16ids[i].target_coreid;
        ctrl_chn[i].owner_coreid = CON_CURCORE;
        ctrl_chn[i].chn_id = (g_virtshl_ctrl_info.h16ids[i].h16id << 0x10) | 0;
        ctrl_chn[i].hid_idx = (unsigned int)i;
        ctrl_chn[i].con = NULL;
        INIT_LIST_HEAD(&ctrl_chn[i].subchn_list);
        INIT_LIST_HEAD(&ctrl_chn[i].slist);
        INIT_LIST_HEAD(&ctrl_chn[i].rlist);

        list_add(&ctrl_chn[i].subchn_list, &g_virtshl_ctrl_info.h16ids[i].virtchn_list);

        retval = bsp_icc_event_register(ctrl_chn[i].chn_id, virtshl_icc_readcb, &ctrl_chn[i], NULL, NULL);
        if (retval) {
            retval = -VIRTSHL_ERR_ICC_REG_EVENT;
            goto icc_cb_reg_fail;
        }
    }

    g_virtshl_ctrl_info.recv_tsk_id = kthread_run(virtshl_recv_tsk_func, NULL, "virtshl_rfunc");
    if (IS_ERR(g_virtshl_ctrl_info.recv_tsk_id))
        goto tsk_init_fail;
    g_virtshl_ctrl_info.send_tsk_id = kthread_run(virtshl_send_tsk_func, NULL, "virtshl_sfunc");
    if (IS_ERR(g_virtshl_ctrl_info.send_tsk_id))
        goto tsk_init_fail;

    return retval;

tsk_init_fail:
icc_cb_reg_fail:
    osl_free(g_virtshl_ctrl_info.h16ids);
no_mem_out:
    return retval;
}
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
/*lint -e528*/
module_init(virtshl_init);
#endif
