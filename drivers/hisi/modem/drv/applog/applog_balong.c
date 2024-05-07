/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#include "applog_balong.h"
#include <linux/module.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <securec.h>
#include <product_config.h>
#include <of.h>
#include <osl_thread.h>
#include <osl_malloc.h>
#include <linux/cdev.h>
#include <linux/of_platform.h>
#include <bsp_slice.h>
#include <bsp_diag.h>

struct cdev g_applog_cdev;
static unsigned int g_applog_char_major;
static struct class *g_applog_class;
struct applog_ctrl_info g_applog_debug = { 0 };

s32 applog_open(struct inode *inode, struct file *file);
ssize_t applog_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
s32 applog_release(struct inode *inode, struct file *file);

static struct file_operations g_applog_fops = {
    .open = applog_open,
    .owner = THIS_MODULE,
    .write = applog_write,
    .release = applog_release,
};


#define APPLOGCHAR_DEVICE_NAME "applog"
#define APPLOGCHAR_DEVICE_CLASS "applog_class"

u32 g_applog_conn = false;
u32 g_applog_enable = false;
u32 g_applog_level = 0;
u32 g_applog_pkgnum = 0;
u32 g_applog_transId = 0;
u32 g_applog_init_state = APPLOG_CHN_UNINIT;
applog_data_s *g_applog_kbuf = NULL;
applog_send_info_s *g_applog_sendbuf = NULL;

u8 *g_socp_buffer_virt_addr;
u8 *g_socp_buffer_phy_addr;

extern struct platform_device *g_socp_pdev;

static u8 *applog_src_phy_to_virt(const u8 *phy_addr)
{
    return ((phy_addr - g_socp_buffer_phy_addr) + g_socp_buffer_virt_addr);
}

s32 bsp_applog_cfg(u32 enable, u32 level)
{
    if (enable) {
        g_applog_enable = true;
    } else {
        g_applog_enable = false;
    }

    if ((level <= 0) || (level > 4)) {
        applog_error("print level err %d\n", level);
        return APPLOG_ERR;
    }

    g_applog_level = level;

    return APPLOG_OK;
}

u32 bsp_applog_disconn(void)
{
    /* set disconnect */
    g_applog_conn = false;

    /* clear switch */
    g_applog_enable = false;

    /* stop socp channel */
    if (bsp_socp_stop(SOCP_CODER_SRC_APPLOG_IND)) {
        applog_error("socp start failed!\n");
        return APPLOG_ERR;
    }

    /* clear socp buffer */
    if (bsp_clear_socp_src_buffer(SOCP_CODER_SRC_APPLOG_IND)) {
        applog_error("socp start failed!\n");
        return APPLOG_ERR;
    }

    /* clear transid */
    g_applog_transId = 0;

    return APPLOG_OK;
}

u32 bsp_applog_conn(void)
{
    /* set local status connect */
    g_applog_conn = true;

    /* enable socp channel */
    if (APPLOG_OK != bsp_socp_start(SOCP_CODER_SRC_APPLOG_IND)) {
        applog_error("socp start failed!\n");
        return APPLOG_ERR;
    }

    /* return status to diag */
    return APPLOG_OK;
}

/*lint -save -e429 */
s32 applog_src_chan_cfg(void)
{
    socp_src_chan_cfg_s src_attr;

    g_socp_buffer_virt_addr = dma_alloc_coherent(&g_socp_pdev->dev, (size_t)APPLOG_SRC_BUF_LEN,
        (dma_addr_t *)&g_socp_buffer_phy_addr, GFP_KERNEL);
    if (g_socp_buffer_virt_addr == APPLOG_NULL) {
        applog_error("applog src chan malloc fail!\n");
        return APPLOG_ERR;
    }

    src_attr.data_type = SOCP_DATA_TYPE_0;
    src_attr.data_type_en = SOCP_DATA_TYPE_EN;
    src_attr.debug_en = SOCP_ENC_DEBUG_DIS;
    src_attr.mode = SOCP_ENCSRC_CHNMODE_CTSPACKET;
    src_attr.priority = SOCP_CHAN_PRIORITY_1;
#ifdef DIAG_SYSTEM_5G
    src_attr.trans_id_en = SOCP_TRANS_ID_EN;
#else
    src_attr.trans_id_en = SOCP_TRANS_ID_DIS;
#endif
    src_attr.rptr_img_en = SOCP_PTR_IMG_DIS;
    src_attr.bypass_en = 0;
    src_attr.dest_chan_id = SOCP_CODER_DST_OM_IND;

    src_attr.coder_src_setbuf.input_start = (u8 *)g_socp_buffer_phy_addr;
    src_attr.coder_src_setbuf.input_end = (u8 *)(g_socp_buffer_phy_addr + APPLOG_SRC_BUF_LEN - 1);

    if (APPLOG_OK != bsp_socp_coder_set_src_chan(SOCP_CODER_SRC_APPLOG_IND, &src_attr)) {
        applog_error("src channel set failed!\n");
        return APPLOG_ERR;
    }

    return APPLOG_OK;
}
/*lint -restore +e429 */
s32 applog_open(struct inode *inode, struct file *file)
{
    if (!g_applog_init_state) {
        return -1;
    }
    return 0;
}

s32 applog_release(struct inode *inode, struct file *file)
{
    return 0;
}

s32 applog_send_data(u8 *data, u32 len)
{
    s32 ret;
    socp_buffer_rw_s rw_buf = {
        NULL,
    };
    u32 temp_len = ALIGN_DDR_WITH_8BYTE(len);

    if (APPLOG_OK != bsp_socp_get_write_buff(SOCP_CODER_SRC_APPLOG_IND, &rw_buf)) {
        applog_error(" buffer write fail ,chan=0x%x len=0x%x\n", SOCP_CODER_SRC_APPLOG_IND, len);
        return -1;
    }

    if (rw_buf.size >= temp_len) {
        ret = memcpy_s((void *)applog_src_phy_to_virt(rw_buf.buffer), rw_buf.size, data, len);
        if (ret) {
            return APPLOG_ERR;
        }

        (void)bsp_socp_write_done(SOCP_CODER_SRC_APPLOG_IND, temp_len);
    } else if (rw_buf.size + rw_buf.rb_size >= temp_len) {
        ret = memcpy_s((void *)applog_src_phy_to_virt(rw_buf.buffer), rw_buf.size, data, rw_buf.size);
        if (ret) {
            return APPLOG_ERR;
        }
        ret = memcpy_s((void *)applog_src_phy_to_virt(rw_buf.rb_buffer), rw_buf.rb_size, data + rw_buf.size,
            (len - rw_buf.size));
        if (ret) {
            return APPLOG_ERR;
        }

        (void)bsp_socp_write_done(SOCP_CODER_SRC_APPLOG_IND, temp_len);
    } else {
        return APPLOG_ERR;
    }

    return APPLOG_OK;
}

s32 applog_report(applog_send_info_s *data, u32 len, u8 level)
{
    s32 ret = 0;
    u32 socp_packet_len = 0;
    u32 diag_packet_len = 0;
    u32 print_packet_len = 0;

    diag_print_ind_head_s *print_head = &data->print_head;
    diag_frame_head_s *diag_head = &data->diag_head;
    diag_socp_head_s *socp_head = &data->socp_head;

    print_packet_len = len + DIAG_PRINT_HEAD_SIZE;
    diag_packet_len = print_packet_len + DIAG_FRAME_HEAD_SIZE;
    socp_packet_len = diag_packet_len + DIAG_SOCP_HEAD_SIZE;

    /* fill print head */
    /* 1:error, 2:warning, 3:normal, 4:info */
    /* (0|ERROR|WARNING|NORMAL|INFO|0|0|0) */
    print_head->level = (0x80000000) >> level;
    print_head->module = 0xFFFFF;
    print_head->msg_num = g_applog_pkgnum;
    g_applog_pkgnum++;

    /* fill diag head */
    applog_diag_frame_head_init(diag_head);

    diag_head->cmdid_stru.cmdid_19b = 0;
    diag_head->cmdid_stru.sec_5b = DIAG_FRAME_MSG_PRINT;
    diag_head->cmdid_stru.mode_4b = DIAG_FRAME_MODE_COMM;
    diag_head->cmdid_stru.pri_4b = DIAG_FRAME_MSG_TYPE_APP;

    diag_head->srv_head.msg_trans_id = g_applog_transId;
    diag_head->msg_len = print_packet_len;

    /* fill socp head */
    applog_fill_socp_head(socp_head, diag_packet_len);

    ret = applog_send_data((u8 *)data, socp_packet_len);
    if (ret) {
        applog_error("send data error!\n");
        g_applog_debug.u32ApplogSendData++;
        return ret;
    }

    /* trans_id++, only if send data successfully */
    g_applog_transId++;

    return APPLOG_OK;
}

ssize_t applog_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    ssize_t ret;
    u32 len = 0;
    u8 level = 0;
    u32 para_len = 0;

    if ((0 == g_applog_conn) || (0 == g_applog_enable)) {
        g_applog_debug.u32ApplogSwitchOnOff++;
        return 0;
    }

    if ((NULL == buf) || (count <= APPLOG_HEAD_LEN)) {
        g_applog_debug.u32ApplogInvalidParameter++;
        return 0;
    }

    len = (count > sizeof(applog_data_s)) ? sizeof(applog_data_s) : count;

    if (copy_from_user(&level, buf, 1)) {
        applog_error("copy from user fail!\n");
        return 0;
    }

    level = level - '0';
    if ((level > 4) || (level > g_applog_level)) {
        g_applog_debug.u32ApplogLevel++;
        return len;
    }

    para_len = snprintf_s((char *)g_applog_sendbuf->data, APPLOG_MAX_HIDS_BUFF_LEN, APPLOG_MAX_HIDS_BUFF_LEN - 1,
        "%s[%d]", "applog", 0);
    if (para_len > APPLOG_MAX_FILENAME_LINENO_LEN) {
        g_applog_debug.u32ApplogLength++;
        return 0;
    }
    if (copy_from_user(g_applog_sendbuf->data + para_len, buf + 1, len - 1)) {
        applog_error("copy from user fail ,read_buf = %s\n", g_applog_sendbuf->data);
        return 0;
    }

    ret = applog_report(g_applog_sendbuf, len - 1 + para_len, level);
    if (ret) {
        applog_error("send data fail!\n");
        g_applog_debug.u32ApplogReport++;
        return 0;
    }


    return len;
}

s32 applog_setup_cdev(struct cdev *cdev, struct file_operations *fops)

{
    s32 ret;
    dev_t devno = MKDEV(g_applog_char_major, 0);

    cdev_init(cdev, fops);
    cdev->owner = THIS_MODULE;
    cdev->ops = fops;

    ret = cdev_add(cdev, devno, 1);
    if (ret) {
        applog_error("add applog fail!\n");
        return ret;
    }

    g_applog_class = class_create(THIS_MODULE, APPLOGCHAR_DEVICE_CLASS);
    if (IS_ERR(g_applog_class)) {
        applog_error("class create failed!\n");
        return APPLOG_ERR;
    }

    device_create(g_applog_class, NULL, devno, NULL, APPLOGCHAR_DEVICE_NAME);
    return APPLOG_OK;
}

void applog_fill_socp_head(diag_socp_head_s *socp_packet, u32 len)
{
    socp_packet->hisi_magic = DIAG_SOCP_HEAD_MAGIC;
    socp_packet->data_len = len;

    return;
}

void applog_diag_frame_head_init(diag_frame_head_s *diag_frame)
{
    s32 ret;

#ifdef DIAG_SYSTEM_5G
    u32 timestamp = 0;

    diag_frame->srv_head.sid_4b = DIAG_FRAME_MSP_SID_DIAG_SERVICE;
    diag_frame->srv_head.ver_4b = DIAG_FRAME_MSP_VER_5G;
    diag_frame->srv_head.mdmid_3b = 0;
    diag_frame->srv_head.rsv_5b = 0;
    diag_frame->srv_head.ssid_8b = DIAG_FRAME_SSID_APP_CPU;
    diag_frame->srv_head.mt_2b = DIAG_FRAME_MT_IND;
    diag_frame->srv_head.index_4b = 0;
    diag_frame->srv_head.eof_1b = 0;
    diag_frame->srv_head.ff_1b = 0;
    diag_frame->srv_head.msg_trans_id = 0;

    timestamp = bsp_get_slice_value();
    ret =
        memcpy_s(diag_frame->srv_head.timestamp, sizeof(diag_frame->srv_head.timestamp), &timestamp, sizeof(timestamp));
    if (ret) {
        applog_error("copy timestamp failed\n");
        return;
    }
#else
    u64 timestamp = 0;

    diag_frame->srv_head.sid_8b = DIAG_FRAME_MSP_SID_DIAG_SERVICE;
    diag_frame->srv_head.mdmid_3b = 0;
    diag_frame->srv_head.rsv_1b = 0;
    diag_frame->srv_head.ssid_4b = DIAG_FRAME_SSID_APP_CPU;
    diag_frame->srv_head.sessionid_8b = MSP_SERVICE_SESSION_ID;
    diag_frame->srv_head.mt_2b = DIAG_FRAME_MT_IND;
    diag_frame->srv_head.index_4b = 0;
    diag_frame->srv_head.eof_1b = 0;
    diag_frame->srv_head.ff_1b = 0;
    diag_frame->srv_head.msg_trans_id = 0;

    bsp_slice_getcurtime(&timestamp);
    ret =
        memcpy_s(diag_frame->srv_head.timestamp, sizeof(diag_frame->srv_head.timestamp), &timestamp, sizeof(timestamp));
    if (ret) {
        applog_error("copy timestamp failed\n");
        return;
    }
#endif
    diag_frame->cmdid_stru.cmdid_19b = 0;
    diag_frame->cmdid_stru.sec_5b = 0;
    diag_frame->cmdid_stru.mode_4b = 0;
    diag_frame->cmdid_stru.pri_4b = 0;
    diag_frame->msg_len = 0;
    return;
}

s32 applog_init(void)
{
    s32 ret;
    dev_t dev = 0;

    applog_crit("[init]start!\n");

    /* 动态获取设备号 */
    ret = alloc_chrdev_region(&dev, 0, 1, APPLOGCHAR_DEVICE_NAME);
    if (ret) {
        applog_error("applog chrdev failed alloc :%d\n", g_applog_char_major);
        return ret;
    }
    g_applog_char_major = MAJOR(dev);

    /* 创建字符驱动设备 */
    ret = applog_setup_cdev(&g_applog_cdev, &g_applog_fops);
    if (ret) {
        applog_error("setup chrdev failed !\n");
        return ret;
    }

    ret = applog_src_chan_cfg();
    if (ret) {
        applog_error("src chan fail !\n");
    }

    /* 申请数据buffer */
    g_applog_sendbuf = (applog_send_info_s *)osl_malloc(sizeof(applog_send_info_s));
    if (g_applog_sendbuf == NULL) {
        applog_error("kbuf malloc fail!\n");
        return APPLOG_ERR;
    }

    memset_s(g_applog_sendbuf, sizeof(applog_send_info_s), 0, sizeof(applog_send_info_s));

    g_applog_init_state = APPLOG_CHN_INIT;
    applog_crit("[init]ok!\n");
    return APPLOG_OK;
}

void applog_exit(void)
{
    osl_free(g_applog_sendbuf);
    cdev_del(&(g_applog_cdev));
    class_destroy(g_applog_class);
    unregister_chrdev_region(MKDEV(g_applog_char_major, 0), 1);
}

u32 applog_show_debug(void)
{
    applog_crit("switch status: 0x%x\n", g_applog_debug.u32ApplogSwitchOnOff);
    applog_crit(" invalid parameter: 0x%d\n", g_applog_debug.u32ApplogInvalidParameter);
    applog_crit("print level error: 0x%x\n", g_applog_debug.u32ApplogLevel);
    applog_crit("data length error: 0x%x\n", g_applog_debug.u32ApplogLength);
    applog_crit("data report: 0x%x\n", g_applog_debug.u32ApplogReport);
    applog_crit("socp data send: 0x%x\n", g_applog_debug.u32ApplogSendData);
    return APPLOG_OK;
}
EXPORT_SYMBOL(applog_show_debug);
module_init(applog_init);
module_exit(applog_exit);
