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
#include <product_config.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <mdrv_memory.h>
#include <bsp_nvim.h>
#include <param_cfg_to_sec.h>
#include <osl_types.h>
#include "nv_comm.h"
#include "nv_msg.h"
#include "nv_partition.h"

struct nvm_info g_nvm_info = { 0 };

struct nvm_info *nv_get_nvminfo(void)
{
    return &g_nvm_info;
}

u64 nv_get_mem_addr(void)
{
    return g_nvm_info.dtaddr;
}

u64 nv_get_mbnaddr(void)
{
    return g_nvm_info.mbnaddr;
}

struct nv_ctrl_info *nv_get_ctrl_info(void)
{
    u64 dtaddr = g_nvm_info.dtaddr;
    struct nv_ctrl_info *ctrl_info = NULL;

    if (dtaddr == 0) {
        nv_printf("nv mem init err\n");
        return NULL;
    }

    ctrl_info = (struct nv_ctrl_info *)(uintptr_t)dtaddr;
    if (ctrl_info->magicnum != NV_HEAD_MAGICNUM) {
        nv_printf("nv mem data invalid");
        return NULL;
    }

    return ctrl_info;
}

struct section_info *nv_get_sninfo(void)
{
    u64 dtaddr = g_nvm_info.dtaddr;
    struct nv_ctrl_info *ctrl_info = nv_get_ctrl_info();
    struct section_info *sninfo = NULL;

    if (ctrl_info == NULL) {
        return NULL;
    }

    sninfo = (struct section_info *)(uintptr_t)(dtaddr + ctrl_info->snidxoff);

    return sninfo;
}

struct item_info *nv_get_item_info(void)
{
    u64 dtaddr = g_nvm_info.dtaddr;
    struct nv_ctrl_info *ctrl_info = nv_get_ctrl_info();
    struct item_info *item_info = NULL;

    if (ctrl_info == NULL) {
        return NULL;
    }

    item_info = (struct item_info *)(uintptr_t)(dtaddr + ctrl_info->itmidxoff);

    return item_info;
}

int nv_msg_task(void *parm)
{
    u32 ret, sync_flag;
    struct nv_msg_data *msg_data = NULL;
    struct nv_msg_element *msg_element = NULL;
    UNUSED(parm);

    while (1) {
        osl_sem_down(&g_nvm_info.task_sem);

        sync_flag = 0;
        msg_element = nv_get_msg_element();
        if (msg_element == NULL) {
            continue;
        }

        msg_data = msg_element->msg_data;

        nv_debug_printf(NV_INFO, "msg task handle sn:0x%x, msg type:0x%x\n", msg_data->sn, msg_data->msg_type);
        switch (msg_data->msg_type) {
            case NV_ICC_REQ_FLUSH:
                sync_flag = 1;
                ret = nv_flush_data(msg_data->msg);
                break;
            case NV_ICC_REQ_FLUSH_RWNV:
                sync_flag = 1;
                ret = nv_flush_rwbuf(msg_data->msg);
                break;
            case NV_ICC_REQ_LOAD_BACKUP:
                sync_flag = 1;
                ret = nv_reload_flush();
                break;
            case NV_ICC_REQ_UPDATE_DEFAULT:
                sync_flag = 1;
                ret = nv_update_factory();
                break;
            case NV_ICC_REQ_UPDATE_BACKUP:
                sync_flag = 1;
                ret = nv_update_backup();
                break;
            case NV_ICC_REQ_FACTORY_RESET:
                sync_flag = 1;
                ret = nv_factory_reset();
                break;
            case NV_ICC_REQ_DATA_WRITEBACK:
                sync_flag = 1;
                ret = nv_upgrade_flush();
                break;
            case NV_ICC_REQ_READ_BLK:
                ret = nv_load_block(msg_data);
                break;
            default:
                nv_printf("msg type %d invalid\n", msg_data->msg_type);
                break;
        }

        msg_data->ret = ret;
        if (ret) {
            nv_printf("nv msg handle fail\n");
        }

        if (sync_flag) {
            nv_cpmsg_cb(msg_data);
        }

        nv_debug_printf(NV_DBG, "sn:0x%x, msg type:0x%x handle end\n", msg_data->sn, msg_data->msg_type);
        nv_free_msgelement(msg_element);
    }

    return NV_OK;
}

u32 nv_mem_init(struct nvm_info *nvminfo)
{
    unsigned long paddr;

    nvminfo->dtaddr = (uintptr_t)mdrv_mem_share_get("nsroshm_nv", &paddr, (unsigned int *)&nvminfo->dtsize, SHM_NSRO);
    if (nvminfo->dtaddr == 0) {
        nv_printf("get sec nv mem err\n");
        return NV_ERROR;
    }
    nvminfo->dtaddr += (u64)NV_GLOBAL_CTRL_SIZE; /* hifi在手机上传入地址有1K偏移 */
    nvminfo->dtsize -= (u64)NV_GLOBAL_CTRL_SIZE;

    nvminfo->mbnaddr = nvminfo->dtaddr + nvminfo->dtsize - NV_MBN_SIZE;
    nvminfo->mbnsize = NV_MBN_SIZE;
    return NV_OK;
}
#define NV_MSG_TASK_PRIORITY 15
#define NV_MSG_TASK_STACK_SIZE 2048
int bsp_nvm_init(void)
{
    u32 ret;
    struct nvm_info *nvminfo = &g_nvm_info;

    nv_debug(NV_FUN_KERNEL_INIT, 0, 0, 0, 0);
    nv_printf("nv init start\n");

    (void)memset_s(nvminfo, sizeof(struct nvm_info), 0, sizeof(struct nvm_info));
    nvminfo->nvm_init_state = NVM_INIT_START;
    ret = nv_mem_init(nvminfo);
    if (ret != NV_OK) {
        nv_debug(NV_FUN_KERNEL_INIT, 0x1, 0, 0, 0);
        nv_printf("nv mem init err\n");
        return ret;
    }

    osl_sem_init(0, &nvminfo->task_sem);
    wakeup_source_init(&nvminfo->wakelock, "nv_wakelock");

    ret = nv_debug_init();
    if (ret != NV_OK) {
        nv_debug(NV_FUN_KERNEL_INIT, 0x2, 0, 0, 0);
        nv_printf("nv debug init fail\n");
    }

    ret = nv_nvmfs_init();
    if (ret != NV_OK) {
        nv_debug(NV_FUN_KERNEL_INIT, 0x3, 0, 0, 0);
        nv_printf("nv file system init fail\n");
        goto nv_init_fail;
    }

    ret = (u32)osl_task_init("nv_msg", NV_MSG_TASK_PRIORITY, NV_MSG_TASK_STACK_SIZE,
        (OSL_TASK_FUNC)nv_msg_task, NULL, (OSL_TASK_ID *)&nvminfo->task_id);
    if (ret) {
        nv_debug(NV_FUN_KERNEL_INIT, 0x4, 0, 0, 0);
        nv_printf("nv icc task init fail\n");
        goto nv_init_fail;
    }

    ret = nv_msg_init();
    if (ret != NV_OK) {
        nv_debug(NV_FUN_KERNEL_INIT, 0x5, 0, 0, 0);
        nv_printf("nv msg init fail\n");
        goto nv_init_fail;
    }

    nvminfo->nvm_init_state = NVM_INIT_SUCCESS;

    nv_printf("nv init success\n");
    return NV_OK;

nv_init_fail:
    nvminfo->nvm_init_state = NVM_INIT_FAILED;
    return (int)ret;
}

#ifdef BSP_CONFIG_PHONE_TYPE
static ssize_t modem_nv_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    return (ssize_t)len;
}

static ssize_t modem_nv_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    u32 ret;

    ret = nv_set_upgrade_flag();
    if (ret) {
        nv_printf("set nv upgrade flag err, ret:0x%x\n", ret);
        return -EIO;
    }

    nv_printf("modify upgrade flag success !\n");
    return len;
}

static const struct file_operations g_modem_nv = {
    .owner = THIS_MODULE, /*lint !e64*/
    .read = modem_nv_read,
    .write = modem_nv_write,
};
#endif

int nv_proc_init(void)
{
#ifdef BSP_CONFIG_PHONE_TYPE
    if (proc_create("ModemNv", 0660, NULL, &g_modem_nv) == NULL) {
        return -1;
    }
#endif

    return 0;
}

static int modem_nv_probe(struct platform_device *dev)
{
    int ret;

    ret = bsp_nvm_init();
    if (ret) {
        nv_printf("nv init err\n");
        return ret;
    }

    ret = nv_proc_init();
    if (ret) {
        nv_printf("nv proc file init err\n");
        return ret;
    }

    return ret;
}

int modem_nv_init(void)
{
    int ret;
    struct platform_device *dev = NULL;

    if (g_nvm_info.nvm_init_state != 0) {
        show_stack(current, NULL);
    }
    ret = nv_init_dev();
    if (ret) {
        return ret;
    }
    ret = modem_nv_probe(dev);

    return ret;
}

#if (FEATURE_OFF == FEATURE_DELAY_MODEM_INIT)
module_init(modem_nv_init);
module_exit(modem_nv_exit);
#endif
