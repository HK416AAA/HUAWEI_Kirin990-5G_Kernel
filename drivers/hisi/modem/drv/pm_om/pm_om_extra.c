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
#include <linux/proc_fs.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/suspend.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/semaphore.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <osl_generic.h>
#include <osl_module.h>
#include <securec.h>
#include <bsp_icc.h>
#include <bsp_ipc.h>
#include <bsp_print.h>
#include <bsp_modem_log.h>
#include <mdrv_pm.h>
#include <bsp_pm_om.h>
#include <bsp_pcdev.h>
#include <mdrv_pcdev.h>
#include "pm_om_extra.h"
#include "pm_om_debug.h"

#undef THIS_MODU
#define THIS_MODU mod_pmom

static struct pm_om_data g_pm_om_data;

static unsigned int modem_log_poll(struct file *file, poll_table *wait)
{ /*lint --e{715} suppress pos not referenced*/
    pmom_pr_err("exit\n");
    return 0;
}

/*
 * modem_log_read - our log's read() method
 * 1) O_NONBLOCK works
 * 2) If there are no log to read, blocks until log is written to
 *  3) Will set errno to EINVAL if read
 */
static ssize_t modem_log_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{ /*lint --e{715} suppress pos not referenced*/
    int err = 0;
    unsigned long ret = 0;
    err = down_interruptible(&g_pm_om_data.pm_om_task_sem);
    if (err) {
        return 0;
    }
    ret = copy_to_user(buf, (void *)g_pm_om_data.data_buf, (unsigned long)g_pm_om_data.data_len);
    if (ret) {
        pmom_pr_err("ret = %ld", ret);
        return 0;
    }
    return (ssize_t)g_pm_om_data.data_len;
}

/*
 * modem_log_open - our log's open() method, not support open twice by same application
 */
static int modem_log_open(struct inode *inode, struct file *file)
{
    pmom_pr_err("entry\n");
    return 0;
}

/*
 * modem_log_release - our log's close() method
 */
static int modem_log_release(struct inode *inode, struct file *file)
{ /*lint --e{715} suppress inode not referenced*/
    pmom_pr_err("entry\n");
    return 0;
}

static const struct file_operations g_modem_log_fops = {
    .read = modem_log_read,
    .poll = modem_log_poll,
    .open = modem_log_open,
    .release = modem_log_release,
};

static ssize_t pm_stat_info_get(struct device *dev, struct device_attribute *attr, char *buf)
{ /*lint --e{715} suppress 'dev' & 'attr' not referenced*/
    int len;
    u64 sw_value;
    u32 tmp_low;
    u32 tmp_high;
    u32 sz = sizeof(int);
    sw_value = g_pm_om_data.sw_bit_value;
    tmp_low = (u32)(sw_value & (u32)DEFAULT_VALUE);
    tmp_high = (u32)((sw_value >> sz) & (u32)DEFAULT_VALUE);
    len = snprintf_s(buf, (size_t)PMOM_SHOW_BUFFER_LEN, (size_t)PMOM_SHOW_BUFFER_LEN - 1,
                     "0x%08x%08x\n", tmp_high, tmp_low);
    return len;
}

static int pm_check_and_get_buf(char p[], size_t dest_max, const char *buf, size_t count)
{
    int ret;
    if (count >= dest_max || buf == NULL) {
        pmom_pr_err("input buf larger than maximum value(200) or buf is NULL\n");
        return -EINVAL;
    }

    ret = memset_s(p, dest_max, 0, dest_max);
    if (ret != EOK) {
        pmom_pr_err("p memset ret = %d\n", ret);
        return ret;
    }
    ret = memcpy_s(p, dest_max, buf, count);
    if (ret != EOK) {
        pmom_pr_err("p memcpy ret = %d\n", ret);
        return ret;
    }
    return 0;
}

static ssize_t pm_stat_info_set(u32 on_off, struct device *dev, struct device_attribute *attr, const char *buf,
                                size_t count)
{ /*lint --e{715} suppress 'dev' & 'attr' not referenced*/
    char p[MAX_BUFFER_SIZE];
    char *pointer = NULL;
    char *tempp = NULL;
    ssize_t mod_id;
    char *endp = NULL;

    int ret = pm_check_and_get_buf(p, MAX_BUFFER_SIZE, buf, count);
    if (ret != 0) {
        return (ssize_t)ret;
    }
    pointer = p;

    for (int i = 0; i < PMOM_MOD_ONOFF_BIT_MAX && ((tempp = strsep(&pointer, " ")) != NULL); i++) {
        if (*tempp == '\0') {
            continue;
        }
        mod_id = simple_strtol(tempp, &endp, PMOM_MOD_ID_N_BASE);
        if (tempp == endp) {
            pmom_pr_err("input includes Non-number parameter\n");
            return -EINVAL;
        }
        if (!(mod_id >= (ssize_t)PM_OM_MOD_ID_ENUM_MAX)) {
            if (on_off) {
                pm_om_log_mod_on(&g_pm_om_data.sw_bit_value, (u32)mod_id);
            } else {
                pm_om_log_mod_off(&g_pm_om_data.sw_bit_value, (u32)mod_id);
            }
        } else {
            pmom_pr_err("input number is invalid\n");
            return -EINVAL;
        }
    }
    if (on_off) {
        ret = bsp_icc_send(ICC_CPU_APLUSB, ICC_CHN_APLUSB_IFC << SHITF_SHORT | APLUSB_IFC_FUNC_PMOM_ON,
                           (u8 *)&g_pm_om_data.sw_bit_value, (u32)sizeof(g_pm_om_data.sw_bit_value));
    } else {
        ret = bsp_icc_send(ICC_CPU_APLUSB, ICC_CHN_APLUSB_IFC << SHITF_SHORT | APLUSB_IFC_FUNC_PMOM_OFF,
                           (u8 *)&g_pm_om_data.sw_bit_value, (u32)sizeof(g_pm_om_data.sw_bit_value));
    }
    if (ret != (int)sizeof(g_pm_om_data.sw_bit_value)) {
        pmom_pr_err("icc send fail! ret = %d.\n", ret);
        return ret;
    }
    return (ssize_t)count;
}
static ssize_t pm_stat_info_on_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{ /*lint --e{715} suppress 'dev' & 'attr' not referenced*/
    return pm_stat_info_set(1, dev, attr, buf, count);
}
static ssize_t pm_stat_info_off_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{ /*lint --e{715} suppress 'dev' & 'attr' not referenced*/
    return pm_stat_info_set(0, dev, attr, buf, count);
}

/*lint -save -e846 -e514 -e778 -e866 -e84 suppress DEVICE_ATTR warning*/
static DEVICE_ATTR(mod_on, 0600, pm_stat_info_get, pm_stat_info_on_set);
static DEVICE_ATTR(mod_off, 0600, pm_stat_info_get, pm_stat_info_off_set);
/*lint -restore +e846 +e514 +e778 +e866 +e84*/
static struct attribute *g_pm_stat_info_on_attributes[] = { &dev_attr_mod_on.attr, NULL };
/*lint -save -e785*/ /* suppress too few initializers for struct */
static const struct attribute_group g_pm_stat_info_on_group = {
    .attrs = g_pm_stat_info_on_attributes,
};

static struct attribute *g_pm_stat_info_off_attributes[] = { &dev_attr_mod_off.attr, NULL };

static const struct attribute_group g_pm_stat_info_off_group = {
    .attrs = g_pm_stat_info_off_attributes,
};

static int pm_info_probe(struct platform_device *pdev)
{
    int ret;
    ret = device_create_file(&(pdev->dev), &dev_attr_mod_on);
    if (ret) {
        pmom_pr_err("fail to creat balong modem mod on sysfs\n");
        return ret;
    }
    ret = device_create_file(&(pdev->dev), &dev_attr_mod_off);
    if (ret) {
        pmom_pr_err("fail to creat balong modem mod off sysfs\n");
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
    .probe      = pm_info_probe,
    .driver     = {
        .name     = "balong_modem_pm_info",
        .owner    = THIS_MODULE,
    },
};

int balong_modem_info_ctrl_init(void)
{
    int ret;

    ret = platform_device_register(&g_balong_modem_pm_info_device);
    if (ret) {
        pmom_pr_err("register balong modem info device fail\n");
        return ret;
    }

    ret = platform_driver_register(&g_balong_modem_pm_info_drv);
    if (ret) {
        pmom_pr_err("register balong modem info driver fail\n");
        platform_device_unregister(&g_balong_modem_pm_info_device);
    }

    return ret;
}
int pm_om_platform_init(void)
{
    int ret;
    const char *dev_name = "pmom";
    struct miscdevice *miscdev;
    miscdev = kzalloc(sizeof(struct miscdevice), GFP_KERNEL);
    if (miscdev == NULL) {
        pmom_pr_err("failed to kzalloc misc device!\n");
        return -1;
    }
    miscdev->fops = &g_modem_log_fops;
    miscdev->parent = NULL;
    miscdev->minor = MISC_DYNAMIC_MINOR;
    miscdev->name = kstrdup(dev_name, GFP_KERNEL);
    ret = misc_register(miscdev);
    if (unlikely(ret)) {                                      /*lint !e730: (Info -- Boolean argument to function)*/
        pmom_pr_err("failed to register misc device!\n"); /*lint !e429*/
        kfree(miscdev);
        return ret;
    }
    return 0;
}

void balong_modem_pm_info_exit(void)
{
    platform_driver_unregister(&g_balong_modem_pm_info_drv);
    platform_device_unregister(&g_balong_modem_pm_info_device);
}

static void pcdev_read_cb(void)
{
    int ret;
    pcdev_wr_async_info_s buf_info = { NULL, NULL, 0, NULL };
    ret = bsp_pcdev_ioctl(g_pm_om_data.pcdev_handler, PCDEV_IOCTL_GET_RD_BUFF, (void *)&buf_info);
    if (ret) {
        pmom_pr_err("pcdev ioctl error,ret:0x%x\n", ret);
        return;
    }
    g_pm_om_data.data_len = buf_info.size;
    ret = memcpy_s((void *)g_pm_om_data.data_buf, g_pm_om_data.data_len, buf_info.p_vaddr, buf_info.size);
    if (ret) {
        pmom_pr_err("g_pm_om_data memcpy  ret = %d\n", ret);
    }
    up(&g_pm_om_data.pm_om_task_sem);
    ret = bsp_pcdev_ioctl(g_pm_om_data.pcdev_handler, PCDEV_IOCTL_RETURN_BUFF, (void *)&buf_info);
    if (ret) {
        pmom_pr_err("pcdev ioctl error,ret:0x%x\n", ret);
        return;
    }
    return;
}

static void pcdev_port_init(void)
{
    int ret;
    pcdev_read_buf_info_s buf_info = {0};
    g_pm_om_data.pcdev_handler = bsp_pcdev_open(pcdev_pmom);
    if (IS_ERR_OR_NULL(g_pm_om_data.pcdev_handler)) {
        pmom_pr_err("pcdev port open failed\n");
        return;
    }
    buf_info.buf_size = CAT_BUFSIZ;
    ret = bsp_pcdev_ioctl(g_pm_om_data.pcdev_handler, PCDEV_IOCTL_RELLOC_READ_BUFF, (void *)&buf_info);
    if (ret) {
        pmom_pr_err("pcdev ioctl error,ret:0x%x\n", ret);
        return;
    }
    ret = bsp_pcdev_ioctl(g_pm_om_data.pcdev_handler, PCDEV_IOCTL_SET_READ_CB, pcdev_read_cb);
    if (ret) {
        pmom_pr_err("pcdev ioctl error,ret:0x%x\n", ret);
        return;
    }
}

int bsp_pm_om_init(void)
{
    int ret;
    ret = balong_modem_info_ctrl_init();
    if (ret) {
        pmom_pr_err("balong_modem_info_ctrl_init fail,ret=0x%x\n", ret);
        return ret;
    }
    ret = pm_om_platform_init();
    if (ret) {
        pmom_pr_err("pm_om_platform_init fail,ret=0x%x\n", ret);
        return ret;
    }
    if (memset_s((void *)&g_pm_om_data, sizeof(g_pm_om_data), 0, sizeof(g_pm_om_data))) {
        pmom_pr_err("g_pm_om_data memset fail\n");
        return -1;
    }
    g_pm_om_data.data_buf = kzalloc(CAT_BUFSIZ, GFP_KERNEL);
    if (g_pm_om_data.data_buf == NULL) {
        pmom_pr_err("data_buf malloc err");
        return -1;
    }
    sema_init(&g_pm_om_data.pm_om_task_sem, 0);
    ret = (int)mdrv_pcdev_reg_enumdonecb((pcdev_enum_done_cb_t)pcdev_port_init);
    if (ret) {
        pmom_pr_err("pcdev reg failed,ret = %d", ret);
        kfree(g_pm_om_data.data_buf);
        g_pm_om_data.data_buf = NULL;
        return ret;
    }

    pmom_pr_err("pm om init ok\n");
    return 0;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(bsp_pm_om_init); /*lint !e19 */
#endif
