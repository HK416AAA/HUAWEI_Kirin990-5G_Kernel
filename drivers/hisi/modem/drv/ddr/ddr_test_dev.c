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

#include <linux/device.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/debugfs.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/gpio.h>
#include <linux/timekeeping.h>
#include <bsp_hardtimer.h>
#include <bsp_pcie.h>
#include <bsp_slice.h>
#include <osl_thread.h>
#include "securec.h"
#include "modem_boot.h"
#include "ddr_test_dev.h"

unsigned int g_boot_flag = 0;
unsigned int g_reset_flag = 1;
unsigned int g_pcie_flag = 1;
extern int bsp_modem_pmu_reset(u32 delayMs);
extern void dump_mdmext_set_excflag(bool flag);
struct semaphore g_msi_ddr_test_pcie_semaphore;
struct semaphore g_msi_ddr_test_reset_semaphore;
struct ddr_test_param g_a_ddr_test_param;

int ddr_test_reset_b(void)
{
    int ret;

    dump_mdmext_set_excflag(true);

    ret = bsp_kernel_pcie_remove();
    if (ret) {
        printk("[ddr test]fail to remove pcie\n");
        return ret;
    }

    ret = bsp_modem_pmu_reset(PMU_RESET_VALUE);
    if (ret) {
        printk("[ddr test]reset pmu fail %x. \n", ret);
        return ret;
    }

    ret = bsp_modem_boot_reset();
    if (ret) {
        printk("[ddr test]fail to reset modem boot.\n");
        return ret;
    }

    ret = bsp_modem_boot_start();
    if (ret) {
        printk("[ddr test]modem boot start failed\n");
        return ret;
    }

    return ret;
}

static ssize_t ddr_test_read_dev(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    int ret = 0;

    ret = simple_read_from_buffer(user_buf, count, ppos, (void *)&g_boot_flag,
        sizeof(g_boot_flag)); /*lint !e571  (Warning -- Suspicious cast)*/
    if (ret == 0) {
        return -EFAULT;
    }
    printk(KERN_ERR "ddr test dev read %d\n", g_boot_flag);

    return ret;
}
unsigned int ddr_test_get_boot_flag(void)
{
    unsigned int tmp;
    tmp = g_boot_flag;
    g_boot_flag = 0;
    return tmp;
}
static ssize_t ddr_test_write_dev(struct file *file, const char __user *usr_buf, size_t count, loff_t *ppos)
{
    char buf[32] = {0};
    u32 *val = &g_a_ddr_test_param;
    int index = 0, param_pos = 0;

    memset_s(&g_a_ddr_test_param, sizeof(g_a_ddr_test_param), 0, sizeof(g_a_ddr_test_param));

    if (count >= sizeof(buf) || NULL == usr_buf) {
        printk(KERN_ERR "input size(%zu) >= buf size(%zu), usr_buf(%p)\n", count, sizeof(buf), usr_buf);
        return -EINVAL;
    }

    if (copy_from_user(&buf, usr_buf, count)) {
        printk(KERN_ERR "ddr test copy_from_user err\n");
        return -1;
    }

    while (index < count && (buf[index] < '0' || buf[index] > '9')) {
        ++index;
    }

    for (; index < count;) {
        if (buf[index] <= '9' && buf[index] >= '0') {
            *val *= 10;
            *val += (buf[index] - '0');
            ++param_pos;
            ++index;
        } else {
            printk(KERN_ERR "ddr test get val %d \n", *val);

            while (index < count && (buf[index] < '0' || buf[index] > '9')) {
                ++index;
            }

            ++val;
            param_pos = 0;
        }
    }

    printk(KERN_ERR "ddr test finally get praram [test_type]:%d, [sub_test_type]:%d, [profile]:%d, [bias]:%d.\n",
        g_a_ddr_test_param.test_type, g_a_ddr_test_param.sub_test_type, g_a_ddr_test_param.profile,
        g_a_ddr_test_param.bias);

    if (count > 2) {
        g_boot_flag = 2;
    } else {
        g_boot_flag = 1;
    }

    mdelay(500);
    up(&g_msi_ddr_test_reset_semaphore);

    return count;
}
int ddr_test_open_dev(struct inode *node, struct file *file)
{
    g_boot_flag = 0;
    return 0;
}

static struct file_operations g_ddr_test_fops = {
    .open = ddr_test_open_dev,
    .read = ddr_test_read_dev,
    .write = ddr_test_write_dev,
};

struct pcie_trans_msg_info g_send_msg = { 0 };
struct pcie_trans_msg_info g_resp_msg = { 0 };
struct ddr_test_msi_log_snd_data g_ddr_send_data = { 0 };
struct ddr_test_msi_log_rcv_data g_ddr_resp_data = { 0 };
static s32 ddr_test_file_access(s8 *path, s32 mode)
{
    s32 ret;
    unsigned long old_fs;

    old_fs = get_fs();
    set_fs((unsigned long)KERNEL_DS);

    ret = sys_access(path, mode);

    set_fs(old_fs);

    return ret;
}

static s32 ddr_test_write_file(s8 *path, const char *buffer, const char *time, unsigned int time_lenth)
{
    s32 ret = -1;
    int fd;
    if (path == NULL) {
        return -1;
    }
    if (ddr_test_file_access((char *)DDR_TEST_LOG_PAR_PATH, 0) != 0) {
        ret = sys_mkdir(DDR_TEST_LOG_PAR_PATH, SYS_OPEN_MODE_1);
        if (0 != ret) {
            printk(KERN_ERR "[ap]sys_mkdir %s failed ret %d.\n", DDR_TEST_LOG_PAR_PATH, ret);
            return ret;
        }
    }
    if (ddr_test_file_access((char *)path, 0) != 0) {
        fd = sys_open(path, O_CREAT | O_RDWR, SYS_OPEN_MODE_0);
        if (fd < 0) {
            printk(KERN_ERR "[ap]ddr test, create %s failed fd %d\n", path, fd);
            return -1;
        }
    } else {
        fd = sys_open(path, O_RDWR, SYS_OPEN_MODE_0);
        if (fd < 0) {
            printk(KERN_ERR "[ap]ddr test, open %s failed fd %d\n", path, fd);
            return -1;
        }
    }

    ret = sys_lseek(fd, 0, SEEK_END);
    if (ret < 0) {
        printk(KERN_ERR "[ap]ddr test,seek file end failed %x!\n", ret);
        sys_close(fd);
        return ret;
    }

    (void)sys_write(fd, time, time_lenth);
    (void)sys_write(fd, buffer, (unsigned int)strlen(buffer));
    sys_close(fd);

    return ret;
}
void ddr_test_record(char *buffer)
{
    int ret = 0;
    mm_segment_t fs;
    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    struct timex txc = {
        0,
    };
    struct rtc_time tm = {
        0,
    };
    char time[LOG_BUF_LEN] = {0};

    do_gettimeofday(&(txc.time));
    rtc_time_to_tm(txc.time.tv_sec, &tm);
    snprintf_s(time, sizeof(time), (sizeof(time) - 1), "%d-%02d-%02d %02d:%02d:%02d:", tm.tm_year + LOG_YEAR_START,
        tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    ddr_test_write_file(DDR_TEST_LOG_PATH, buffer, time, (unsigned int)strlen(time));
    ddr_test_write_file(DDR_TEST_LOG_HIS_PATH, buffer, time, (unsigned int)strlen(time));

    set_fs(fs);
}

irqreturn_t ddr_test_msi_isr(int irq, void *dev_info)
{
    int ret;
    struct pcie_trans_msg_info g_resp_msg = {
        .msi_id = 0,
        .cmd_len = 0,
        .cmd_data_addr = (void *)&g_ddr_resp_data,
    };

    ret = bsp_pcie_rc_read_msg(&g_resp_msg);
    if (ret != 0) {
        printk(KERN_ERR "[ap]ddr test, pcie read msg err! ret %x\n", ret);
        return IRQ_HANDLED;
    }
    printk(KERN_ERR "[ap]ddr test, pcie read msg log type %x!\n", g_ddr_resp_data.op_type);
    printk(KERN_ERR "[ap]ddr test, pcie read msg log %s!\n", g_ddr_resp_data.log);
    bsp_pcie_rc_read_clear();
    up(&g_msi_ddr_test_pcie_semaphore);
    return IRQ_HANDLED;
}

int ddr_test_msi_int_task(void *data)
{
    struct pcie_trans_msg_info g_send_msg = { 0 };
    int ret = 0;

    do {
        (void)down_interruptible(&g_msi_ddr_test_pcie_semaphore);

        switch (g_ddr_resp_data.op_type) {
            case DDR_TEST_GET_TEST_PARAM:
                g_ddr_send_data.op_type = DDR_TEST_GET_TEST_PARAM;
                memcpy_s(&g_ddr_send_data.param, sizeof(g_ddr_send_data.param), &g_a_ddr_test_param,
                    sizeof(g_a_ddr_test_param));
                g_send_msg.msi_id = PCIE_MSI_ID_DDR_TEST;
                g_send_msg.cmd_len = sizeof(struct ddr_test_msi_log_snd_data);
                g_send_msg.cmd_data_addr = (void *)&g_ddr_send_data;
                ret = bsp_pcie_rc_send_msg(&g_send_msg);
                if (ret != 0) {
                    printk(KERN_ERR "[ap][ddr test]: fail to send ddr test param %d \n", g_ddr_send_data.param);
                    return -1;
                }
                break;
            case DDR_TEST_LOG:
                ddr_test_record((char *)(&g_ddr_resp_data.log));

                g_ddr_send_data.log_status = 1;
                g_ddr_send_data.op_type = DDR_TEST_LOG;
                g_send_msg.msi_id = PCIE_MSI_ID_DDR_TEST;
                g_send_msg.cmd_len = sizeof(struct ddr_test_msi_log_snd_data);
                g_send_msg.cmd_data_addr = (void *)&g_ddr_send_data;
                ret = bsp_pcie_rc_send_msg(&g_send_msg);
                if (ret != 0) {
                    printk(KERN_ERR "[ap][ddr test]: fail to send log_status, log status %d \n",
                        g_ddr_send_data.log_status);
                    return -1;
                }
                break;
            default:
                printk(KERN_ERR "[ap]ddr test, invalid op type: %x \n", g_ddr_resp_data.op_type);
                break;
        }
    } while (g_pcie_flag);

    return 0;
}
int ddr_test_reset_b_task(void *data)
{
    int ret = 0;
    do {
        (void)down_interruptible(&g_msi_ddr_test_reset_semaphore);
        printk(KERN_ERR "start reset modem\n");
        ret = ddr_test_reset_b();
        if (0 != ret) {
            printk(KERN_ERR "reset modem failed %x\n", ret);
        }
    } while (g_reset_flag);
}

int ddr_test_dev_init(void)
{
    int ret;
    OSL_TASK_ID taskid;
    int fd;
    mm_segment_t fs;
    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    struct dentry *debugfs_root;
    debugfs_root = debugfs_create_dir("ddr", NULL);
    if (!debugfs_root) {
        printk(KERN_ERR "create ddr debugfs dir failed\n");
        return 0;
    }
    debugfs_create_file("ddr_test", (S_IRUGO | S_IWUSR | S_IWGRP), debugfs_root, NULL, &g_ddr_test_fops);

    fd = sys_open(DDR_TEST_LOG_HIS_PATH, O_CREAT, SYS_OPEN_MODE_0);
    sys_close(fd);
    set_fs(fs);
    if (fd < 0) {
        printk(KERN_ERR "[ap]ddr test, create history log file failed fd %x\n", fd);
    }
    sema_init(&g_msi_ddr_test_pcie_semaphore, 0);
    sema_init(&g_msi_ddr_test_reset_semaphore, 0);

    ret = osl_task_init("dt_pcie_int", DDR_PCIE_PRIORITY, DDR_PCIE_TASK_STACK, (OSL_TASK_FUNC)ddr_test_msi_int_task,
        NULL, &taskid);
    if (ret) {
        printk(KERN_ERR "[ap][ddr test]fail to create task pcie int handler ret %x.\n", ret);
        return 0;
    }

    ret = osl_task_init("dt_reset_handler", DDR_PCIE_PRIORITY, DDR_PCIE_TASK_STACK,
        (OSL_TASK_FUNC)ddr_test_reset_b_task, NULL, &taskid);
    if (ret) {
        printk(KERN_ERR "[ap][ddr test]fail to create reset b task ret %x.\n", ret);
        return 0;
    }
    ret = bsp_pcie_receive_msi_request(PCIE_MSI_ID_DDR_TEST, ddr_test_msi_isr, "ddr_test_msg_msi", NULL);
    if (ret) {
        printk(KERN_ERR "[ap][ddr test]fail to init msi interrupt ret %x.\n", ret);
        return 0;
    }
    printk(KERN_ERR "ddr test dev init ok\n");
    return 0;
}
void ddr_test_dev_exit(void) {}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(ddr_test_dev_init);
module_exit(ddr_test_dev_exit);
#endif
