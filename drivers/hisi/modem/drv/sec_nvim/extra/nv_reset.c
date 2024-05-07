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

#include <product_config.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <drv_comm.h>
#include <osl_types.h>
#include <bsp_blk.h>
#include <bsp_nvim.h>
#include "nv_reset.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*lint -save -e438 -e715*/
#define NV_SEC_VRL_SIZE 0x1000

#define nv_printf(fmt, ...) (printk(KERN_ERR "<%s> " fmt, __FUNCTION__, ##__VA_ARGS__))

static u32 g_nv_debug = 0;

u32 nv_show_data(u32 *data)
{
    int i;

    if (g_nv_debug) {
        return NV_OK;
    }

    nv_printf("data:\n");
    for (i = 0; i < (sizeof(nv_dload_head) / sizeof(u32)); i++) {
        printk(KERN_ERR "0x%8x\n", *data);
        data++;
    }
    nv_printf("\ndata end\n");

    return NV_OK;
}
u32 nv_upgrade_set_partition_flag(const char *partition, bool flag)
{
    u32 ret;
    u32 flen;
    nv_dload_head phead;
    nv_dload_tail ptail;

    ret = bsp_blk_read(partition, (0 + NV_SEC_VRL_SIZE), &phead, (u32)sizeof(nv_dload_head));
    if (ret) {
        nv_printf("%s can't access, ret:0x%x\n", partition, ret);
        return ret;
    }

    if (phead.nv_bin.magic_num != NV_FILE_USING) {
        nv_printf("%s partition magic number:0x%x(!=0x%x) error\n", partition, phead.nv_bin.magic_num, NV_FILE_USING);
        nv_show_data((u32 *)&phead);
        return BSP_ERR_NV_NO_FILE;
    }

    flen = sizeof(nv_dload_head);
    flen += ((phead.nv_bin.magic_num == NV_FILE_USING) ? phead.nv_bin.len : 0);
    flen += ((phead.xnv_map.magic_num == NV_FILE_USING) ? phead.xnv_map.len : 0);
    flen += ((phead.xnv.magic_num == NV_FILE_USING) ? phead.xnv.len : 0);
    flen += ((phead.cust_map.magic_num == NV_FILE_USING) ? phead.cust_map.len : 0);
    flen += ((phead.cust.magic_num == NV_FILE_USING) ? phead.cust_map.len : 0);

    ret = bsp_blk_read(partition, (flen + NV_SEC_VRL_SIZE), &ptail, (u32)sizeof(nv_dload_tail));
    if (ret) {
        nv_printf("read %s tail failed, ret:0x%x\n", partition, ret);
        return BSP_ERR_NV_READ_DATA_FAIL;
    }

    if (flag) {
        ptail.upgrade_flag = NV_FILE_UPDATA;
    } else {
        ptail.upgrade_flag = NV_FILE_USING;
    }

    ret = bsp_blk_write(partition, (flen + NV_SEC_VRL_SIZE), &ptail, (u32)sizeof(nv_dload_tail));
    if (ret) {
        nv_printf("write %s tail failed, ret:0x%x\n", partition, ret);
        return BSP_ERR_NV_WRITE_DATA_FAIL;
    }

    return NV_OK;
}

u32 nv_upgrade_set_flag(bool flag)
{
    u32 ret;

    ret = nv_upgrade_set_partition_flag((s8 *)NV_DLOAD_SEC_NAME, flag);
    if (ret) {
        nv_printf("set %s upgrade flag failed\n", NV_DLOAD_SEC_NAME);
        return ret;
    }

    nv_printf("set %s upgrade flag success\n", NV_DLOAD_SEC_NAME);
    ret = nv_upgrade_set_partition_flag((s8 *)NV_DLOAD_CUST_SEC_NAME, flag);
    if ((ret != BSP_ERR_NV_NO_FILE) && (ret != NV_OK)) {
        nv_printf("set %s upgrade flag failed\n", NV_DLOAD_CUST_SEC_NAME);
        return ret;
    }

    nv_printf("nv upgrade flag set success\n");
    return NV_OK;
}

static ssize_t modem_nv_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    return (ssize_t)len;
}
static ssize_t modem_nv_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    u32 ret;

    ret = nv_upgrade_set_flag((bool)true);
    if (ret)
        return -EIO;

    nv_printf("modify upgrade flag success !\n");
    /*lint -save -e713*/
    return len;
    /*lint -restore*/
}
static const struct file_operations g_modem_nv = {
    .owner = THIS_MODULE, /*lint !e64*/
    .read = modem_nv_read,
    .write = modem_nv_write,
};
/*lint !e785*/
int modemNv_ProcInit(void)
{
    nv_printf("start create ModemNv node\n");

    if (proc_create("ModemNv", 0660, NULL, &g_modem_nv) == NULL) {
        return -1;
    }
    return 0;
}
/*lint -restore*/
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
/*lint -restore*/
module_init(modemNv_ProcInit);
#endif
/*lint -restore*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
