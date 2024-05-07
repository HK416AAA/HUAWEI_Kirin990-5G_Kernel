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
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/irqdomain.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/personality.h>
#include <linux/stat.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/fuse.h>
#include <linux/statfs.h>
#include <linux/rcupdate.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/platform_device.h>
#include <bsp_pcie.h>
#include <bsp_reset.h>
#include "securec.h"
#include <bsp_slice.h>
#include <linux/time.h>
#include <linux/dma-mapping.h>
#include <linux/of_device.h>
#include <linux/syscore_ops.h>

#include <mdrv_errno.h>
#include <bsp_dump.h>

#define MAX_WRITE (256 * 1024)

#define PCIE_DMA_BUFFER_SIZE \
    (sizeof(struct fuse_in_header) + sizeof(struct fuse_write_in) + MAX_WRITE + sizeof(struct rfs_msg_head))
#define PCIE_BUFFER_SIZE 256

#define RFS_DEVICE_NAME "rfsd_dev"
#define RFS_DEVICE_CLASS "rfs_class"
#define RFS_FIRST_MINOR 0
#define RFS_DEVICES_NUMBER 1
#define RFS_OK 0
#define RFS_ERR (-1)
#define FUSE_RESET 2018

enum rfs_server_state {
    RFS_SERVER_INIT = 0,
    RFS_SERVER_RUNNING = 1,
    RFS_SERVER_RESUME = 2,
    RFS_SERVER_SUSPEND = 3,
    RFS_SERVER_RESET = 4,
    RFS_SERVER_RESET_DONE = 5,
    RFS_SERVER_PCIE_LINKDOWN = 6,
    RFS_SERVER_NUM
};

struct rfs_msg_head {
    unsigned int data_size;
};

struct rfsd_dev_t {
    struct cdev cdev;
    dev_t dev_no;
    struct class *rfs_class;
    struct semaphore r_sem;
    struct semaphore w_sem;
    struct semaphore reset_sem;
    enum rfs_server_state state;
    unsigned long bar_len;
    unsigned long bar_addr_phys;
    void *bar_addr_virt;

    void *buf_addr_virt;
    unsigned int pcie_receive_num;
    unsigned int pcie_sent_num;
    unsigned int dpm_state;
    unsigned int rfsd_init_flag;
    struct work_struct rfsd_reset_work;
    unsigned long dma_addr_phys;
    void *dma_addr_virt;
    unsigned long dma_buf_len;
    struct pcie_dma_transfer_info rc_transfer_info;
};
struct rfsd_dev_t g_rfs_server_dev;
struct platform_device *g_rfs_server_platform_pdev;
char g_rfs_name[0x8] = "rfs_msi";

struct rfs_server_fuse_type {
    unsigned int type;
    const char *ops_name;
};

#define FUSE_UNKNOWN_OP 0xffffffff
struct rfs_server_fuse_type g_rfs_server_ops_type[] = {
    { FUSE_LOOKUP, "handle_lookup" },
    { FUSE_FORGET, "handle_forget" },
    { FUSE_GETATTR, "handle_getattr" },
    { FUSE_SETATTR, "handle_setattr" },
    { FUSE_MKNOD, "handle_mknod" },
    { FUSE_MKDIR, "handle_mkdir" },
    { FUSE_UNLINK, "handle_unlink" },
    { FUSE_RMDIR, "handle_rmdir" },
    { FUSE_RENAME, "handle_rename" },
    { FUSE_OPEN, "handle_open" },
    { FUSE_READ, "handle_read" },
    { FUSE_WRITE, "handle_write" },
    { FUSE_STATFS, "handle_statfs" },
    { FUSE_RELEASE, "handle_release" },
    { FUSE_FSYNC, "handle_fsync" },
    { FUSE_FLUSH, "handle_flush" },
    { FUSE_OPENDIR, "handle_opendir" },
    { FUSE_READDIR, "handle_readdir" },
    { FUSE_RELEASEDIR, "handle_releasedir" },
    { FUSE_LOOKUP, "handle_reset" },
    { FUSE_INIT, "handle_init" },
    { FUSE_CANONICAL_PATH, "handle_canonical_path" },
    { FUSE_UNKNOWN_OP, "unknown_op" },
};

void rfs_server_check_ops_type(const struct fuse_in_header *hdr)
{
    unsigned int i;
    struct rfs_server_fuse_type *ops_type = NULL;

    if (g_rfs_server_dev.dpm_state == 0) {
        return;
    }

    g_rfs_server_dev.dpm_state = 0;

    ops_type = g_rfs_server_ops_type;
    for (i = 0; i < (sizeof(g_rfs_server_ops_type) / sizeof(struct rfs_server_fuse_type)); i++) {
        if (ops_type->type == hdr->opcode) {
            break;
        }
        ops_type++;
    }
    if (i == (sizeof(g_rfs_server_ops_type) / sizeof(struct rfs_server_fuse_type))) {
        printk(KERN_ERR "[C SR][rfs server] can not find image of type id = %d,hdr->nodeid = %lld \n", hdr->opcode,
                hdr->nodeid);
        return;
    }

    if (bsp_is_pcie_first_user(PCIE_EP_MSI_RFS)) {
        printk(KERN_ERR "[C SR][rfs server] rfs first wake up ,rfsd: opcode = %s ,nodeid = %lld \n",
                ops_type->ops_name, hdr->nodeid);
    }
}

void rfs_server_pack_reset_msg(void)
{
    uint32_t len = 40;
    uint32_t opcode = FUSE_RESET;  // FUSE_RESET
    unsigned int datasize = 40;

    *(unsigned int *)g_rfs_server_dev.buf_addr_virt = datasize;
    *(uint32_t *)(g_rfs_server_dev.buf_addr_virt + sizeof(struct rfs_msg_head)) = len;
    *(uint32_t *)(g_rfs_server_dev.buf_addr_virt + sizeof(struct rfs_msg_head) + sizeof(uint32_t)) = opcode;
}

void rfs_server_dma_pack_reset_msg(void)
{
    uint32_t len = 40;
    uint32_t opcode = FUSE_RESET;  // FUSE_RESET
    unsigned int datasize = 40;

    *(unsigned int *)g_rfs_server_dev.dma_addr_virt = datasize;
    *(uint32_t *)(g_rfs_server_dev.dma_addr_virt + sizeof(struct rfs_msg_head)) = len;
    *(uint32_t *)(g_rfs_server_dev.dma_addr_virt + sizeof(struct rfs_msg_head) + sizeof(uint32_t)) = opcode;
}

int rfs_server_reset(drv_reset_cb_moment_e eparam, s32 usrdata)
{
    if (eparam == MDRV_RESET_CB_BEFORE) {
        printk(KERN_ERR "[rfs server] rfs_server_reset start\n");
        g_rfs_server_dev.state = RFS_SERVER_RESET;

        up(&g_rfs_server_dev.r_sem);
        while (down_interruptible(&g_rfs_server_dev.reset_sem)) {
            continue;
        }
        printk(KERN_ERR "[rfs server] rfs_server_reset end\n");
    }
    return 0;
}

void rfs_rerver_reset_work(struct work_struct *work)
{
    printk(KERN_ERR "[rfs server] rfs_rerver_reset_work done\n");
    system_error(DRV_ERROR_USER_RESET, 0, 0, NULL, 0);
}

int rfs_server_open(struct inode *inode, struct file *file)
{
    if (g_rfs_server_dev.rfsd_init_flag == 0) {
        g_rfs_server_dev.rfsd_init_flag = 1;
        printk(KERN_ERR "[rfs server] rfsd open dev/rfsd \n");
    } else if (g_rfs_server_dev.rfsd_init_flag == 1) {
        if (g_rfs_server_dev.state == RFS_SERVER_RESET_DONE) {
            printk(KERN_ERR "[rfs_server] rfsd restart during rfs reset !\n");
            up(&g_rfs_server_dev.reset_sem);
        }
        printk(KERN_ERR "[rfs server] rfsd restart ,reset modem\n");
        schedule_work(&g_rfs_server_dev.rfsd_reset_work);
    }
    return 0;
}

int rfs_server_pcie_receive(void *buf, unsigned long len)
{
    if (len > g_rfs_server_dev.bar_len) {
        printk(KERN_ERR "receive len is large than buf, len 0x%lx, bar len is 0x%lx\n", len, g_rfs_server_dev.bar_len);
        return RFS_ERR;
    }

    up(&g_rfs_server_dev.r_sem);

    return 0;
}

ssize_t rfs_server_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct rfs_msg_head *rfs_msg = NULL;
    int data_size;
    int err = 0;
    struct fuse_in_header *hdr = NULL;

    if (buf == NULL) {
        printk(KERN_ERR "[rfs server] usr buf is null\n");
        return RFS_ERR;
    }

    err = down_interruptible(&g_rfs_server_dev.r_sem);
    if (err) {
        return RFS_ERR;
    }

    /* rfs reset */
    if (g_rfs_server_dev.state > RFS_SERVER_RESET) {
        printk(KERN_ERR "[rfs server]  pcie linkdown drop the msg\n");
        return RFS_ERR;
    }

    if ((g_rfs_server_dev.state == RFS_SERVER_RESET) && (g_rfs_server_dev.r_sem.count == 0)) {
        rfs_server_dma_pack_reset_msg();
        printk(KERN_ERR "[rfs server] sent reset msg to rfsd\n");
        g_rfs_server_dev.state = RFS_SERVER_RESET_DONE;
    }

    rfs_msg = (struct rfs_msg_head *)g_rfs_server_dev.dma_addr_virt;
    if (count < rfs_msg->data_size) {
        printk(KERN_ERR "[rfs server] usr count is smaller , bar size is 0x%x, count is 0x%x\n", rfs_msg->data_size,
               (unsigned int)count);
        return RFS_ERR;
    }
    data_size = copy_to_user(buf, (char *)g_rfs_server_dev.dma_addr_virt + sizeof(struct rfs_msg_head),
                             rfs_msg->data_size);
    if (data_size) {
        printk(KERN_ERR "[rfs server] copy to user fail, request id 0x%lx\n",
               (unsigned long)(uintptr_t)g_rfs_server_dev.dma_addr_virt);
        return RFS_ERR;
    }

    hdr = (struct fuse_in_header *)((char *)g_rfs_server_dev.dma_addr_virt + sizeof(struct rfs_msg_head));

    rfs_server_check_ops_type(hdr);

    g_rfs_server_dev.pcie_receive_num++;

    return rfs_msg->data_size;
}

ssize_t rfs_server_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int ret = 0;
    struct rfs_msg_head *rfs_msg = (struct rfs_msg_head *)g_rfs_server_dev.dma_addr_virt;

    if (count > (g_rfs_server_dev.dma_buf_len - sizeof(struct rfs_msg_head))) {
        printk(KERN_ERR "[rfs server] dma usr count is larger , bar size is 0x%lx, count is 0x%x\n",
               (unsigned long)g_rfs_server_dev.dma_buf_len, (unsigned int)count);
        return RFS_ERR;
    }

    /* rfs reset */
    if (g_rfs_server_dev.state == RFS_SERVER_RESET_DONE) {
        printk(KERN_ERR "[rfs_server] rfs reset done success!\n");
        up(&g_rfs_server_dev.reset_sem);
        return count;
    } else if (g_rfs_server_dev.state == RFS_SERVER_RESET || g_rfs_server_dev.state == RFS_SERVER_PCIE_LINKDOWN) {
        printk(KERN_ERR "[rfs_server] rfs reset drop the msg!\n");
        return count;
    }

    if (copy_from_user(g_rfs_server_dev.dma_addr_virt + sizeof(struct rfs_msg_head), buf, count)) {
        printk(KERN_ERR "[rfs server] copy from user fail ,request id = 0x%lx\n",
               (unsigned long)(uintptr_t)g_rfs_server_dev.dma_addr_virt);
        return RFS_ERR;
    }
    writel(count, &rfs_msg->data_size);

    g_rfs_server_dev.rc_transfer_info.element.transfer_size = (count + sizeof(struct rfs_msg_head));
    ret = bsp_pcie_rc_dma_transfer(&(g_rfs_server_dev.rc_transfer_info));
    if (ret) {
        printk(KERN_ERR "[rfs server] dma:bsp_pcie_rc_send_msi faild ,ret = 0x%d\n", ret);
        return RFS_ERR;
    }

    g_rfs_server_dev.pcie_sent_num++;

    return count;
}

static int rfs_server_read_ep_addr(void)
{
    unsigned long *pcie_bar_addr = (unsigned long *)g_rfs_server_dev.bar_addr_virt;
    unsigned int ep_dma_addr_low;
    unsigned int ep_dma_addr_high;

    pcie_bar_addr++;
    ep_dma_addr_low = readl(pcie_bar_addr);
    ep_dma_addr_high = readl((unsigned long)(uintptr_t)pcie_bar_addr + sizeof(unsigned int));
    if (ep_dma_addr_low == 0 && ep_dma_addr_high == 0) {
        printk(KERN_ERR "[rfs server] read ep dma_addr_phys  fail\n");
        return -1;
    }
    g_rfs_server_dev.rc_transfer_info.element.dar_low = ep_dma_addr_low;
    g_rfs_server_dev.rc_transfer_info.element.dar_high = ep_dma_addr_high;

    return 0;
}
irqreturn_t rfs_server_pcie_msi_handler(int irq_num, void *data)
{
    /* get receive len */
    struct rfs_msg_head *rfs_msg = (struct rfs_msg_head *)g_rfs_server_dev.dma_addr_virt;

    if (rfs_msg->data_size > g_rfs_server_dev.dma_buf_len) {
        printk(KERN_ERR "[rfs server] dma:buf is large than buf, len 0x%x, dma buf len is 0x%lx\n", rfs_msg->data_size,
               g_rfs_server_dev.dma_buf_len);
        return IRQ_HANDLED;
    }

    up(&g_rfs_server_dev.r_sem);

    return IRQ_HANDLED;
}

int rfs_server_pcie_dma_mem_init(void)
{
    dma_addr_t rfs_pcie_dma_phy_addr = 0;
    void *rfs_pcie_dma_virt_addr = NULL;

    printk(KERN_ERR "[rfs client] rfs_server_pcie_dma_init start \n");

    /* malloc memory */
    rfs_pcie_dma_virt_addr = dma_alloc_coherent(&g_rfs_server_platform_pdev->dev, PCIE_DMA_BUFFER_SIZE,
                                                &rfs_pcie_dma_phy_addr, GFP_KERNEL);
    if (rfs_pcie_dma_virt_addr == NULL) {
        printk(KERN_ERR "[rfs client] dma allocation resource fail");
        return -1;
    }

    (void)memset_s(rfs_pcie_dma_virt_addr, PCIE_DMA_BUFFER_SIZE, 0, PCIE_DMA_BUFFER_SIZE);

    g_rfs_server_dev.dma_addr_phys = rfs_pcie_dma_phy_addr;
    g_rfs_server_dev.dma_addr_virt = rfs_pcie_dma_virt_addr;
    g_rfs_server_dev.dma_buf_len = PCIE_DMA_BUFFER_SIZE;

    g_rfs_server_dev.rc_transfer_info.direction = PCIE_DMA_DIRECTION_WRITE;
    g_rfs_server_dev.rc_transfer_info.element.sar_low = g_rfs_server_dev.dma_addr_phys & 0xFFFFFFFFUL;
    g_rfs_server_dev.rc_transfer_info.element.sar_high = g_rfs_server_dev.dma_addr_phys >> 0x20;
    g_rfs_server_dev.rc_transfer_info.element.transfer_size = 0;
    g_rfs_server_dev.rc_transfer_info.transfer_type = PCIE_DMA_NORMAL_MODE;
    g_rfs_server_dev.rc_transfer_info.callback_args = NULL;
    g_rfs_server_dev.rc_transfer_info.channel = PCIE_DMA_RFS;
    g_rfs_server_dev.rc_transfer_info.remote_int_enable = 0;

    printk(KERN_ERR "[rfs client] rfs_server_pcie_dma_init end \n");
    return 0;
}

static int rfs_server_send_mem_addr(void)
{
    unsigned long *pcie_bar_addr = (unsigned long *)g_rfs_server_dev.bar_addr_virt;
    unsigned int read_back_low;
    unsigned int read_back_high;

    writel(g_rfs_server_dev.dma_addr_phys & 0xFFFFFFFFUL, pcie_bar_addr);
    writel(g_rfs_server_dev.dma_addr_phys >> 0x20, (unsigned long)(uintptr_t)pcie_bar_addr + sizeof(unsigned int));
    read_back_low = readl(pcie_bar_addr);
    read_back_high = readl((unsigned long)(uintptr_t)pcie_bar_addr + sizeof(unsigned int));
    if ((read_back_low != (g_rfs_server_dev.dma_addr_phys & 0xFFFFFFFFUL))
        || (read_back_high != (g_rfs_server_dev.dma_addr_phys >> 0x20))) {
        printk(KERN_ERR "[rfs server] send dma_addr_phys to ep fail\n");
        return -1;
    }
    return 0;
}

int rfs_server_dma_debug_info(void)
{
    printk(KERN_ERR "[rfs server] debug_info begin\n");
    printk(KERN_ERR "[rfs server] debug_info : bar addr phy:0x%lx, bar virt addr:0x%lx\n",
           g_rfs_server_dev.bar_addr_phys, (unsigned long)(uintptr_t)g_rfs_server_dev.bar_addr_virt);
    printk(KERN_ERR "[rfs server] debug_info : buf_addr_virt:0x%lx\n",
          (unsigned long)(uintptr_t)g_rfs_server_dev.buf_addr_virt);
    printk(KERN_ERR "[rfs server] debug_info : pcie_bar_len = 0x%lx \n", g_rfs_server_dev.bar_len);
    printk(KERN_ERR "[rfs server] debug_info : pcie_sent_num = 0x%x, pcie_receive_num = 0x%x \n",
           g_rfs_server_dev.pcie_sent_num, g_rfs_server_dev.pcie_receive_num);
    printk(KERN_ERR "[rfs server] debug_info : r_sem.count = 0x%x \n", g_rfs_server_dev.r_sem.count);
    printk(KERN_ERR "[rfs server] debug_info : dma addr phy:0x%lx, dma virt addr:0x%lx\n",
           g_rfs_server_dev.dma_addr_phys, (unsigned long)(uintptr_t)g_rfs_server_dev.dma_addr_virt);
    printk(KERN_ERR "[rfs server] debug_info : dma_buf_len = 0x%lx \n", g_rfs_server_dev.dma_buf_len);
    printk(KERN_ERR "[rfs server] debug_info : ep dma addr high:0x%x, dma addr low:0x%x\n",
           g_rfs_server_dev.rc_transfer_info.element.dar_high, g_rfs_server_dev.rc_transfer_info.element.dar_low);
    printk(KERN_ERR "[rfs server] debug_info end\n");
    return 0;
}

int rfs_server_release(struct inode *inode, struct file *file)
{
    return 0;
}

static struct file_operations g_rfsd_dev_fops = {
    .open = rfs_server_open,
    .owner = THIS_MODULE,
    .read = rfs_server_read,
    .write = rfs_server_write,
    .release = rfs_server_release,
};

static void rfs_server_dev_free(void)
{
    struct cdev *cdev = &g_rfs_server_dev.cdev;

    cdev_del(cdev);
    unregister_chrdev_region(g_rfs_server_dev.dev_no, RFS_DEVICES_NUMBER);
    return;
}

static int rfs_server_dev_setup(void)
{
    int ret;
    struct cdev *cdev = &g_rfs_server_dev.cdev;

    ret = alloc_chrdev_region(&g_rfs_server_dev.dev_no, RFS_FIRST_MINOR, RFS_DEVICES_NUMBER, RFS_DEVICE_NAME);
    if (ret) {
        printk(KERN_ERR "[rfs server] alloc_chrdev_region failed\n");
    }

    cdev_init(cdev, &g_rfsd_dev_fops);

    cdev->owner = THIS_MODULE;
    cdev->ops = &g_rfsd_dev_fops;

    ret = cdev_add(cdev, g_rfs_server_dev.dev_no, 1);
    if (ret) {
        printk(KERN_ERR "[rfs server] add applog fail!\n");
        unregister_chrdev_region(g_rfs_server_dev.dev_no, RFS_DEVICES_NUMBER);
        return ret;
    }

    g_rfs_server_dev.rfs_class = class_create(THIS_MODULE, RFS_DEVICE_CLASS);
    if (IS_ERR(g_rfs_server_dev.rfs_class)) {
        printk(KERN_ERR "[rfs server] class create failed!\n");
        cdev_del(cdev);
        unregister_chrdev_region(g_rfs_server_dev.dev_no, RFS_DEVICES_NUMBER);
        return RFS_ERR;
    }

    device_create(g_rfs_server_dev.rfs_class, NULL, g_rfs_server_dev.dev_no, NULL, RFS_DEVICE_NAME);

    return 0;
}

int rfs_server_pcie_dev_init(void)
{
    int ret;

    /* request msi */
    ret = bsp_pcie_rc_msi_request(PCIE_EP_MSI_RFS, rfs_server_pcie_msi_handler, (const char *)g_rfs_name,
                                  NULL);
    if (ret) {
        printk(KERN_ERR "[rfs server] request msi fail! re = %d\n", ret);
        return ret;
    }

    /* get bar addr */
    g_rfs_server_dev.bar_addr_phys = bsp_pcie_rc_get_bar_addr(PCIE_BAR_RFS);
    if (!g_rfs_server_dev.bar_addr_phys) {
        printk(KERN_ERR "[rfs server] get bar addr fail!\n");
        return RFS_ERR;
    }

    /* get bar len */
    g_rfs_server_dev.bar_len = bsp_pcie_rc_get_bar_size(PCIE_BAR_RFS);

    /* get bar virt addr */
    g_rfs_server_dev.bar_addr_virt = ioremap(g_rfs_server_dev.bar_addr_phys, g_rfs_server_dev.bar_len + 1);
    if (g_rfs_server_dev.bar_addr_virt == NULL) {
        printk(KERN_ERR "[rfs server] bar addr ioremap fail!\n");
        return RFS_ERR;
    }
    ret = rfs_server_read_ep_addr();
    if (ret) {
        printk(KERN_ERR "[rfs server] read_ep_addr  fail\n");
        iounmap(g_rfs_server_dev.bar_addr_virt);
        return RFS_ERR;
    }

    ret = rfs_server_send_mem_addr();
    if (ret) {
        printk(KERN_ERR "[rfs server] pcie_dma_init fail!\n");
        iounmap(g_rfs_server_dev.bar_addr_virt);
        return RFS_ERR;
    }

    g_rfs_server_dev.pcie_sent_num = 0;
    g_rfs_server_dev.pcie_receive_num = 0;
    g_rfs_server_dev.state = RFS_SERVER_RUNNING;
    printk(KERN_ERR "[rfs server] rfs_server_pcie_dev_init done\n");

    return 0;
}

static int rfs_server_pcie_free(void)
{
    u32 ret;

    g_rfs_server_dev.state = RFS_SERVER_PCIE_LINKDOWN;

    if (g_rfs_server_dev.bar_addr_virt != NULL) {
        iounmap(g_rfs_server_dev.bar_addr_virt);
        g_rfs_server_dev.bar_addr_virt = 0;
        g_rfs_server_dev.bar_addr_phys = 0;
        g_rfs_server_dev.bar_len = 0;
    }

    ret = bsp_pcie_rc_msi_free(PCIE_EP_MSI_RFS);
    if (ret) {
        printk(KERN_ERR "[rfs server] pcie_rc_msi_free fail! ret = %d\n", ret);
    } else {
        printk(KERN_ERR "[rfs server] pcie_free success \n");
    }
    return ret;
}

static void rfs_server_pcie_linkdown(void)
{
    g_rfs_server_dev.state = RFS_SERVER_PCIE_LINKDOWN;
    printk(KERN_ERR "[rfs server] pcie_linkdown \n");
}

int rfs_server_pcie_callback(u32 usr_id, u32 cb_id, void *callback_args)
{
    int ret = 0;

    if (cb_id == PCIE_RC_CB_ENUM_DONE) {
        ret = rfs_server_pcie_dev_init();
    } else if (cb_id == PCIE_RC_CB_EXIT) {
        ret = rfs_server_pcie_free();
    } else if (cb_id == PCIE_RC_CB_LINKDOWN) {
        rfs_server_pcie_linkdown();
    }
    return ret;
}

int rfs_server_debug_info(void)
{
    printk(KERN_ERR "[rfs server] debug_info begin\n");
    printk(KERN_ERR "[rfs server] debug_info : bar addr phy:0x%lx, bar virt addr:0x%lx\n",
           g_rfs_server_dev.bar_addr_phys, (unsigned long)(uintptr_t)g_rfs_server_dev.bar_addr_virt);
    printk(KERN_ERR "[rfs server] debug_info : buf_addr_virt:0x%lx\n",
          (unsigned long)(uintptr_t)g_rfs_server_dev.buf_addr_virt);
    printk(KERN_ERR "[rfs server] debug_info : pcie_bar_len = 0x%lx \n", g_rfs_server_dev.bar_len);
    printk(KERN_ERR "[rfs server] debug_info : pcie_sent_num = 0x%x, pcie_receive_num = 0x%x \n",
           g_rfs_server_dev.pcie_sent_num, g_rfs_server_dev.pcie_receive_num);
    printk(KERN_ERR "[rfs server] debug_info : r_sem.count = 0x%x \n", g_rfs_server_dev.r_sem.count);
    printk(KERN_ERR "[rfs server] debug_info end\n");
    return 0;
}

static int rfs_server_mem_init(void)
{
    u32 ret;
    /* alloc mem */
    g_rfs_server_dev.buf_addr_virt = kzalloc(PCIE_BUFFER_SIZE, GFP_KERNEL);
    if (g_rfs_server_dev.buf_addr_virt == NULL) {
        printk(KERN_ERR "[rfs server] kzalloc buf fail!\n");
        return RFS_ERR;
    }

    ret = rfs_server_pcie_dma_mem_init();
    if (ret) {
        printk(KERN_ERR "[rfs server] pcie_dma_mem_init fail!\n");
        kfree(g_rfs_server_dev.buf_addr_virt);
        return ret;
    }
    return 0;
}

static int rfs_server_dev_init(void)
{
    int ret;
    struct pcie_callback_info pcie_rfs_callback_info = {0};
    printk(KERN_ERR "[rfs server] init begin\n");

    /* sem int */
    sema_init(&g_rfs_server_dev.r_sem, 0);
    sema_init(&g_rfs_server_dev.w_sem, 0);
    sema_init(&g_rfs_server_dev.reset_sem, 0);

    /* cdev init */
    ret = rfs_server_dev_setup();
    if (ret) {
        printk(KERN_ERR "[rfs server] setup dev fail! ret = %d\n", ret);
        return ret;
    }

    /* register pcie callback */
    pcie_rfs_callback_info.callback = rfs_server_pcie_callback;
    pcie_rfs_callback_info.callback_args = NULL;
    ret = bsp_pcie_rc_cb_register(PCIE_USER_RFS, &pcie_rfs_callback_info);
    if (ret) {
        printk(KERN_ERR "[rfs server] pcie cb register fail! ret = %d\n", ret);
        rfs_server_dev_free();
        return ret;
    }

    /* register reset callback */
    ret = bsp_reset_cb_func_register("rfs", (pdrv_reset_cbfun)rfs_server_reset, 0, DRV_RESET_CB_PIOR_RFILE);
    if (ret) {
        printk(KERN_ERR "[rfs server] reset cb register fail! ret = %d\n", ret);
        rfs_server_dev_free();
        return ret;
    }

    /* mem init */
    ret = rfs_server_mem_init();
    if (ret) {
        printk(KERN_ERR "[rfs server] mem_init fail! ret = %d\n", ret);
        rfs_server_dev_free();
        return ret;
    }

    INIT_WORK(&g_rfs_server_dev.rfsd_reset_work, rfs_rerver_reset_work);

    g_rfs_server_dev.state = RFS_SERVER_INIT;

    printk(KERN_ERR "[rfs server] init end \n");
    return ret;
}

static void rfs_server_set_dpm_flag(void)
{
    g_rfs_server_dev.dpm_state = 1;
    printk(KERN_ERR "[rfs server] [SR] set dpm_state.\n");
}

static struct syscore_ops g_rfs_server_syscore_ops = {
    .resume = rfs_server_set_dpm_flag,
};

int rfs_server_probe(struct platform_device *pdev)
{
    int ret;

    printk(KERN_ERR "[rfs server] [init] start\n");

    of_dma_configure(&pdev->dev, NULL);
    dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(0x40));
    g_rfs_server_platform_pdev = pdev;

    register_syscore_ops(&g_rfs_server_syscore_ops);

    ret = rfs_server_dev_init();
    if (ret) {
        printk(KERN_ERR "[rfs server] rfs_server_dev_init fail\n");
        return ret;
    }

    printk(KERN_ERR "[rfs server] [init] ok.\n");

    return 0;
}

static struct platform_driver g_rfs_server_drv = {
    .probe      = rfs_server_probe,
    .driver     = {
        .name     = "rfs_server",
        .owner    = THIS_MODULE,
    },
};

static struct platform_device g_rfs_server_device = {
    .name = "rfs_server",
    .id = -1,
    .dev = {
        .init_name = "rfs_server",
    },
};

int rfs_server_init(void)
{
    int ret;

    ret = platform_device_register(&g_rfs_server_device);
    if (ret) {
        bsp_err(KERN_ERR "[rfs server] register rfs_server device failed\n");
        return -1;
    }

    ret = platform_driver_register(&g_rfs_server_drv);
    if (ret) {
        printk(KERN_ERR "[rfs server] register rfs_server driver failed\n");
        platform_device_unregister(&g_rfs_server_device);
        return -1;
    }

    return 0;
}

void rfs_server_exit(void)
{
    platform_driver_unregister(&g_rfs_server_drv);
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(rfs_server_init);
module_exit(rfs_server_exit);
#endif

MODULE_DESCRIPTION("HIS Balong rfs server");
MODULE_LICENSE("GPL");
