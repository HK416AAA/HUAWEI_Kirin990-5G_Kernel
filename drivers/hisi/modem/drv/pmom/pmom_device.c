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
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/kthread.h>
#include <linux/sched/signal.h>
#include <bsp_print.h>
#include <securec.h>
#include "pmom_device.h"
#include "pmom_platform.h"

#undef THIS_MODU
#define THIS_MODU mod_pmom

static pmom_device_ctrl_s g_pmom_device_ctrl;
static pmom_platform_ctrl_s *g_pmom_platform_ctrl = NULL;

static ssize_t pmom_balong_modem_pm_info_get(struct device *dev, struct device_attribute *attr, char *buf)
{
    s32 len;
    pmom_device_ctrl_s *device_ctrl = &g_pmom_device_ctrl;

    len = snprintf_s(buf, PMOM_SHOW_SW_BITMAP_MAX_LEN, PMOM_SHOW_SW_BITMAP_MAX_LEN - 1, "0x%llx\n",
                     device_ctrl->mod_sw_bitmap);
    return len;
}

// set的内容为：mod_id mod_id mod_id ...
static ssize_t pmom_balong_modem_pm_info_set(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count)
{
    pmom_device_ctrl_s *device_ctrl = &g_pmom_device_ctrl;
    int i;
    char p[PMOM_PM_INFO_SET_BUF_SIZE] = { 0 };
    char *pointer = NULL;
    char *tempp = NULL;
    char *endp = NULL;
    ssize_t mod_id;
    int ret;
    u64 mod_sw_bitmap = 0;

    if (count >= PMOM_PM_INFO_SET_BUF_SIZE || buf == NULL) {
        bsp_err("pmom_balong_modem_pm_info_set input buf larger than maximum value(200) or buf is NULL!\n");
        return -EINVAL;
    }

    ret = memcpy_s(p, PMOM_PM_INFO_SET_BUF_SIZE, buf, count);
    if (ret) {
        bsp_err("pmom_balong_modem_pm_info_set memcpy_s failed, ret = %d!\n", ret);
        return -EINVAL;
    }
    pointer = p;

    tempp = strsep(&pointer, " ");
    for (i = 0; (i < PMOM_MOD_ID_BIT_MAX) && (tempp != NULL); i++) {
        if (*tempp == '\0') {
            continue;
        }
        mod_id = simple_strtol(tempp, &endp, PMOM_MOD_ID_N_BASE);
        if (tempp == endp) {
            bsp_err("pmom_balong_modem_pm_info_set input includes Non-number parameters!\n");
            return -EINVAL;
        }
        if (mod_id >= 0 && mod_id < PMOM_MOD_ID_BIT_MAX) {
            mod_sw_bitmap |= PMOM_BIT_64((u32)mod_id);
        } else {
            bsp_err("pmom_balong_modem_pm_info_set input number is invalid!\n");
            return -EINVAL;
        }
        tempp = strsep(&pointer, " ");
    }

    // 发消息通知c核上报pmom log
    device_ctrl->mod_sw_bitmap = mod_sw_bitmap;
    pmom_msg_to_ccore(PMOM_MSG_GATHER_LOG, &mod_sw_bitmap, sizeof(mod_sw_bitmap));

    return (ssize_t)count;
}

/*lint -save -e846 -e514 -e778 -e866 -e84 suppress DEVICE_ATTR warning*/
static DEVICE_ATTR(mod_on, 0600, pmom_balong_modem_pm_info_get, pmom_balong_modem_pm_info_set);

static s32 pmom_balong_modem_pm_info_probe(struct platform_device *pdev)
{
    s32 ret;
    ret = device_create_file(&(pdev->dev), &dev_attr_mod_on);
    if (ret != 0) {
        bsp_err("pmom_modem_pm_info_probe creat mod_on sysfs failed!\n");
        return ret;
    }

    return ret;
}

static struct platform_device g_balong_modem_pm_info_device = {
    .name = "balong_modem_pm_info",
    .id = 0,
    .dev = {
        .init_name = "balong_modem_pm_info",
    },
};

static struct platform_driver g_balong_modem_pm_info_drv = {
    .probe      = pmom_balong_modem_pm_info_probe,
    .driver     = {
        .name     = "balong_modem_pm_info",
        .owner    = THIS_MODULE,
    },
};

void pmom_create_balong_modem_pm_info(void)
{
    s32 ret;

    ret = platform_device_register(&g_balong_modem_pm_info_device);
    if (ret != 0) {
        bsp_err("pmom_create_modem_pm_info register device failed!\n");
        return;
    }

    ret = platform_driver_register(&g_balong_modem_pm_info_drv);
    if (ret != 0) {
        bsp_err("pmom_create_modem_pm_info register driver failed!\n");
        platform_device_unregister(&g_balong_modem_pm_info_device);
    }
}

static ssize_t pmom_device_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    s32 err;
    unsigned long ret;
    u32 len;

    bsp_info("%s entry\n", __func__);
    if (file == NULL) {
        return -ENOENT;
    }
    if (buf == NULL || g_pmom_platform_ctrl == NULL) {
        return -EFAULT;
    }

    err = down_interruptible(&g_pmom_platform_ctrl->pmom_task_sem);
    if (err) {
        bsp_err("(err) = %d, return ENOENT\n", err);
        return 0;
    }
    if (g_pmom_platform_ctrl->data_len == 0) {
        bsp_err("pmom_device_read data_len is 0!");
        return 0;
    }
    len = min(g_pmom_platform_ctrl->data_len, (u32)count);
    g_pmom_platform_ctrl->data_len = 0;
    ret = copy_to_user(buf, (void *)g_pmom_platform_ctrl->data_buf, (unsigned long)len);
    if (ret != 0) {
        bsp_err("pmom_device_read copy_to_user failed, ret = %ld!", ret);
        return 0;
    }
    return (ssize_t)len;
}

static const struct file_operations g_pmom_device_ops = {
    .read = pmom_device_read,
};

static s32 pmom_create_pmom_device(void)
{
    s32 ret;
    const char *dev_name = "pmom";
    struct miscdevice *pmom_device = &g_pmom_device_ctrl.pmom;

    pmom_device->minor = MISC_DYNAMIC_MINOR;
    pmom_device->name = kstrdup(dev_name, GFP_KERNEL);
    if (pmom_device->name == NULL) {
        ret = -ENOMEM;
        bsp_err("pmom_create_pmom_device failed, ret = %d!\n", ret);
        return ret;
    }

    pmom_device->fops = &g_pmom_device_ops;
    pmom_device->parent = NULL;

    ret = misc_register(pmom_device);
    if (ret != BSP_OK) {
        bsp_err("pmom_create_pmom_device register pmom failed, ret = %d!\n", ret);
        kfree(pmom_device->name);
        pmom_device->name = NULL;
        return ret;
    }
    return BSP_OK;
}

s32 pmom_device_init(void)
{
    s32 ret;
    ret = pmom_create_pmom_device();
    if (ret != BSP_OK) {
        return ret;
    }
    pmom_create_balong_modem_pm_info();
    g_pmom_platform_ctrl = pmom_get_platform_ctrl();
    return BSP_OK;
}
