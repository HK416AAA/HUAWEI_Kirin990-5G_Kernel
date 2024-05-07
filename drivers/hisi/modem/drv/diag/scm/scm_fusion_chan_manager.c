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

#include <linux/dma-mapping.h>
#include "scm_fusion_chan_manager.h"
#include "scm_fusion_common.h"
#include "ppm_common.h"
#include "diag_port_manager.h"
#include <bsp_shared_ddr.h>
#include <osl_malloc.h>
#include <securec.h>
#include <nv_id_drv.h>
#include <nv_stru_drv.h>
#include <bsp_nvim.h>
#include "bsp_socp.h"
#include "bsp_version.h"
#include "soc_socp_adapter.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern spinlock_t g_scm_spin_lock;
extern mdrv_ppm_chan_debug_info_s g_ppm_chan_info;
extern struct platform_device *g_socp_pdev;

scm_coder_src_cfg_s g_scm_src_chan_cfg[SCM_SOCP_SRC_CHAN_BUTT] = {
    { .init_state = SCM_CHANNEL_UNINIT,
      .chan_id = SOCP_CODER_SRC_ACPU_CNF,
      .dst_chan_id = SOCP_CODER_DST_OM_CNF,
      .data_type = SOCP_DATA_TYPE_0,
      .chan_mode = SOCP_ENCSRC_CHNMODE_CTSPACKET,
      .chan_prio = SOCP_CHAN_PRIORITY_3,
      .trans_id_en = SOCP_TRANS_ID_EN,
      .rptr_img_en = SOCP_PTR_IMG_DIS,
      .src_buff_len = SCM_CODER_SRC_CNF_BUFFER_SIZE,
      .rd_buff_len = SCM_CODER_SRC_RDSIZE,
      .src_virt_buff = NULL,
      .src_phy_buff = NULL,
      .rd_virt_buff = NULL,
      .rd_phy_buff = NULL,
      .rptr_img_phy_addr = NULL,
      .rptr_img_virt_addr = NULL },
    { .init_state = SCM_CHANNEL_UNINIT,
      .chan_id = SOCP_CODER_SRC_ACPU_IND,
      .dst_chan_id = SOCP_CODER_DST_OM_IND,
      .data_type = SOCP_DATA_TYPE_0,
      .chan_mode = SOCP_ENCSRC_CHNMODE_CTSPACKET,
      .chan_prio = SOCP_CHAN_PRIORITY_1,
      .trans_id_en = SOCP_TRANS_ID_EN,
      .rptr_img_en = SOCP_PTR_IMG_DIS,
      .src_buff_len = SCM_CODER_SRC_IND_BUFFER_SIZE,
      .rd_buff_len = SCM_CODER_SRC_RDSIZE,
      .src_virt_buff = NULL,
      .src_phy_buff = NULL,
      .rd_virt_buff = NULL,
      .rd_phy_buff = NULL,
      .rptr_img_phy_addr = NULL,
      .rptr_img_virt_addr = NULL },
};

scm_coder_dst_cfg_s g_scm_dst_chan_cfg[SCM_SOCP_DST_CHAN_BUTT] = {
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = SOCP_CODER_DST_OM_CNF,
        .buff_len = SCM_CODER_DST_CNF_SIZE,
        .threshold = SCM_CODER_CNF_DST_THRESHOLD,
        .timeout_mode = SOCP_TIMEOUT_TFR_SHORT,
        .dst_virt_buff = NULL,
        .dst_phy_buff = NULL,
        .func = NULL,
    },
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = SOCP_CODER_DST_OM_IND,
        .buff_len = SCM_CODER_DST_IND_SIZE,
        .threshold = SCM_CODER_IND_DST_THRESHOLD,
        .timeout_mode = SOCP_TIMEOUT_TFR_LONG,
        .dst_virt_buff = NULL,
        .dst_phy_buff = NULL,
        .func = NULL,
    },
};

s32 scm_cnf_dst_buff_init(void)
{
    u32 buf_len;

    /* CNF通道不需要做延迟写入 */
    buf_len = g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].buff_len;
    g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].dst_virt_buff = (u8 *)dma_alloc_coherent(&g_socp_pdev->dev, buf_len,
        (dma_addr_t *)&g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].dst_phy_buff, GFP_KERNEL);
    if (g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].dst_phy_buff == NULL) { /* 申请空间错误 */
        /* 记录通道初始化标记为内存申请异常 */
        g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].init_state = SCM_CHANNEL_MEM_FAIL;
        return ERR_MSP_MALLOC_FAILUE;
    }

    return BSP_OK;
}

s32 scm_malloc_ind_dst_buff(void)
{
    /* 申请编码目的空间 */
    g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].dst_virt_buff =
        (u8 *)dma_alloc_coherent(&g_socp_pdev->dev, g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].buff_len,
        (dma_addr_t *)&g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].dst_phy_buff, GFP_KERNEL);
    if (g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].dst_phy_buff == NULL) { /* 申请空间错误 */
        g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].init_state = SCM_CHANNEL_MEM_FAIL;
        return ERR_MSP_MALLOC_FAILUE; /* 返回错误 */
    }

    return BSP_OK;
}

s32 scm_ind_dst_buff_init(void)
{
    socp_encdst_buf_log_cfg_s log_cfg;
    s32 ret;

    ret = memset_s(&log_cfg, sizeof(log_cfg), 0, sizeof(log_cfg));
    if (ret) {
        diag_error("memset_s fail, ret=0x%x\n", ret);
    }

    ret = (s32)bsp_socp_get_log_cfg(&log_cfg);
    if (ret != 0) {
        return ret;
    }

    /* IND通道需要做延迟写入，BUFFER大小50M(默认值)，水线设置为水线设置为75%，内存在初始化已经申请过 */
    g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].dst_phy_buff = (u8 *)(uintptr_t)(log_cfg.phy_buffer_addr);
    g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].buff_len = log_cfg.buffer_size;
    g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].dst_virt_buff = log_cfg.vir_buffer;

    return BSP_OK;
}

s32 scm_dst_chan_buff_init(void)
{
    s32 ret;

    ret = scm_ind_dst_buff_init();
    if (ret) {
        diag_error("init ind dst buff fail(0x%x)\n", ret);
        return ret;
    }

    ret = scm_cnf_dst_buff_init();
    if (ret) {
        diag_error("init cnf dst buff fail(0x%x)\n", ret);
        return ret;
    }

    return BSP_OK;
}

s32 scm_src_chan_buff_init(void)
{
    u32 i;

    for (i = 0; i < SCM_SOCP_SRC_CHAN_BUTT; i++) {
        g_scm_src_chan_cfg[i].src_virt_buff = (u8 *)dma_alloc_coherent(&g_socp_pdev->dev,
            g_scm_src_chan_cfg[i].src_buff_len, (dma_addr_t *)&g_scm_src_chan_cfg[i].src_phy_buff, GFP_KERNEL);

        if (g_scm_src_chan_cfg[i].src_phy_buff == NULL) {
            diag_error("socp chan buffer alloc fail,chan_id=0x%x\n", g_scm_src_chan_cfg[i].chan_id);
            g_scm_src_chan_cfg[i].init_state = SCM_CHANNEL_MEM_FAIL;
            return BSP_ERROR;
        }

        g_scm_src_chan_cfg[i].init_state = SCM_CHANNEL_INIT_SUCC;
    }

    return BSP_OK;
}

s32 scm_src_chan_cfg_init(void)
{
    s32 ret;
    u32 i;

    for (i = 0; i < SCM_SOCP_SRC_CHAN_BUTT; i++) {
        ret = scm_src_chan_cfg(&g_scm_src_chan_cfg[i]);
        if (ret) {
            g_scm_src_chan_cfg[i].init_state = SCM_CHANNEL_CFG_FAIL; /* 记录通道初始化配置错误 */
            return ret;                                              /* 返回失败 */
        }

        ret = bsp_socp_start(g_scm_src_chan_cfg[i].chan_id);
        if (ret) {
            g_scm_src_chan_cfg[i].init_state = SCM_CHANNEL_START_FAIL; /* 记录通道开启错误 */
            return ret;                                                /* 返回失败 */
        }

        g_scm_src_chan_cfg[i].init_state = SCM_CHANNEL_INIT_SUCC; /* 记录通道初始化配置成功 */
    }

    return BSP_OK; /* 返回成功 */
}

unsigned long *scm_cnf_src_phy_to_virt(const u8 *phy_addr)
{
    return (unsigned long *)((phy_addr - g_scm_src_chan_cfg[SCM_SOCP_CNF_CHAN].src_phy_buff) +
        g_scm_src_chan_cfg[SCM_SOCP_CNF_CHAN].src_virt_buff);
}

unsigned long *scm_ind_src_phy_to_virt(const u8 *phy_addr)
{
    return (unsigned long *)((phy_addr - g_scm_src_chan_cfg[SCM_SOCP_IND_PS_CHAN].src_phy_buff) +
        g_scm_src_chan_cfg[SCM_SOCP_IND_PS_CHAN].src_virt_buff);
}

s32 scm_src_chan_cfg(scm_coder_src_cfg_s *chan_attr)
{
    socp_src_chan_cfg_s chan_info; /* 当前通道的属性信息 */
    s32 ret;

    chan_info.dest_chan_id = chan_attr->dst_chan_id;
    chan_info.data_type = chan_attr->data_type;
    chan_info.mode = chan_attr->chan_mode;
    chan_info.priority = chan_attr->chan_prio;
    chan_info.trans_id_en = chan_attr->trans_id_en;
    chan_info.rptr_img_en = chan_attr->rptr_img_en;
    chan_info.bypass_en = SOCP_HDLC_ENABLE;
    chan_info.data_type_en = SOCP_DATA_TYPE_EN;
    chan_info.debug_en = SOCP_ENC_DEBUG_DIS;

    chan_info.coder_src_setbuf.input_start = chan_attr->src_phy_buff;
    chan_info.coder_src_setbuf.input_end = (chan_attr->src_phy_buff + chan_attr->src_buff_len) - 1;
    chan_info.coder_src_setbuf.rd_input_start = chan_attr->rd_phy_buff;
    chan_info.coder_src_setbuf.rd_input_end = (chan_attr->rd_phy_buff + chan_attr->rd_buff_len) - 1;
    chan_info.coder_src_setbuf.rd_threshold = SCM_CODER_SRC_RD_THRESHOLD;

    if (chan_attr->rptr_img_en) {
        chan_attr->rptr_img_virt_addr = (u8 *)dma_alloc_coherent(&g_socp_pdev->dev, sizeof(unsigned long),
            (dma_addr_t *)&chan_attr->rptr_img_phy_addr, GFP_KERNEL);
        if (chan_attr->rptr_img_phy_addr == NULL) {
            diag_error("socp chan %d rptr image alloc fail\n", chan_attr->chan_id);
            return ERR_MSP_MALLOC_FAILUE;
        }

        chan_info.read_ptr_img_phy_addr = (uintptr_t)chan_attr->rptr_img_phy_addr;
        chan_info.read_ptr_img_vir_addr = (uintptr_t)chan_attr->rptr_img_virt_addr;
    }

    ret = bsp_socp_coder_set_src_chan(chan_attr->chan_id, &chan_info);
    if (ret) {
        diag_error("src chan cfg fail, chan_id=0x%x, ret=0x%x\n", chan_attr->chan_id, ret);

        return ret; /* 返回错误 */
    }

    diag_crit("socp chan %d init successfull\n", chan_attr->chan_id);
    return BSP_OK; /* 返回成功 */
}

s32 scm_dst_chan_cfg(scm_coder_dst_cfg_s *chan_attr)
{
    socp_dst_chan_cfg_s channel;
    s32 ret;

    /* 扩大编码目的通道1阈值门限 */
    channel.encdst_thrh = 2 * SCM_CODER_DST_GTHRESHOLD;
    channel.coder_dst_setbuf.output_start = chan_attr->dst_phy_buff;
    channel.coder_dst_setbuf.output_end = (chan_attr->dst_phy_buff + chan_attr->buff_len) - 1;
    channel.coder_dst_setbuf.threshold = chan_attr->threshold;
    channel.encdst_timeout_mode = chan_attr->timeout_mode;

    if ((ret = bsp_socp_coder_set_dst_chan(chan_attr->chan_id, &channel)) != BSP_OK) {
        diag_error("socp dst channel cfg fail, chan_id=0x%x\n", chan_attr->chan_id);
        chan_attr->init_state = SCM_CHANNEL_CFG_FAIL; /* 记录通道初始化配置错误 */
        return ret;                                   /* 返回失败 */
    }

    return ERR_MSP_SUCCESS;
}

s32 scm_cnf_dst_cfg_init(void)
{
    const bsp_version_info_s *version_info = NULL;
    s32 ret;

    version_info = bsp_get_version_info();
    if (version_info == NULL) {
        diag_error("get version info is null\n");
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (version_info->plat_type == PLAT_HYBRID) {
        g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].threshold = 32;
    }

    ret = scm_dst_chan_cfg(&g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN]);
    if (ret) {
        return ret;
    }

    bsp_socp_encdst_dsm_init(g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].chan_id, SOCP_DEST_DSM_ENABLE);

    g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].init_state = SCM_CHANNEL_INIT_SUCC; /* 记录通道初始化配置错误 */

    (void)bsp_socp_register_read_cb(g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].chan_id,
        (socp_read_cb)scm_cnf_dst_read_cb);

    (void)socp_dst_channel_enable(g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].chan_id);

    return ERR_MSP_SUCCESS;
}

s32 scm_ind_dst_cfg_init(void)
{
    const bsp_version_info_s *version_info = NULL;
    s32 ret;

    version_info = bsp_get_version_info();
    if (version_info == NULL) {
        diag_error("get version info is null\n");
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (version_info->plat_type == PLAT_HYBRID) {
        g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].threshold = 512;
    }

    ret = scm_dst_chan_cfg(&g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN]);
    if (ret) {
        return ret;
    }

    (void)bsp_socp_register_read_cb(g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].chan_id,
        (socp_read_cb)scm_ind_dst_read_cb);

    (void)socp_dst_channel_enable(g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].chan_id);

    g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].init_state = SCM_CHANNEL_INIT_SUCC; /* 记录通道初始化配置错误 */

    return ERR_MSP_SUCCESS;
}

s32 scm_dst_chan_cfg_init(void)
{
    s32 ret;

    ret = scm_ind_dst_cfg_init();
    if (ret) {
        return ret;
    }
    ret = scm_cnf_dst_cfg_init();
    if (ret) {
        return ret;
    }

    return BSP_OK;
}

u32 scm_rls_cnf_dst_buff(u32 read_size)
{
    return scm_rls_dst_buff(g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].chan_id, read_size);
}

u32 scm_rls_ind_dst_buff(u32 read_size)
{
    return (u32)scm_rls_dst_buff(g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].chan_id, read_size);
}

s32 scm_rls_dst_buff(u32 chan_id, u32 read_size)
{
    socp_buffer_rw_s buffer;
    u32 data_len;

    if (read_size == 0) { /* 释放通道所有数据 */
        if (bsp_socp_get_read_buff(chan_id, &buffer) != BSP_OK) {
            diag_error("Get Read Buffer is Error\n");
            return ERR_MSP_FAILURE;
        }

        data_len = buffer.size + buffer.rb_size;

        if (chan_id == g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].chan_id) {
            diag_system_debug_ind_dst_lost(EN_DIAG_DST_LOST_CPMCB, data_len);
        }
    } else {
        data_len = read_size;
    }

    if (bsp_socp_read_data_done(chan_id, data_len) != BSP_OK) {
        diag_error("Read Data Done is Error\n");

        return ERR_MSP_FAILURE;
    }
    return BSP_OK;
}

void scm_reg_cnf_coder_dst_send_func(void)
{
    g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].func = (scm_coder_dst_read_cb)scm_send_cnf_data_to_udi;

    diag_crit("socp cnf chan reg send func success\n");
}

void scm_reg_ind_coder_dst_send_func(void)
{
    g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].func = (scm_coder_dst_read_cb)scm_send_ind_data_to_udi;
    diag_crit("socp ind dst chan reg send func success\n");
}

void scm_send_cnf_data_to_udi(u8 *virt_addr, u8 *phy_addr, u32 len)
{
    u32 send_result;
    u32 ret = ERR_MSP_FAILURE;
    unsigned int phy_port;
    u32 send_len;
    bool send_succ_flag = false;
    bool usb_send_flag = false;
    mdrv_ppm_port_debug_info_s *debug_info = NULL;
    unsigned int logic_port;
    unsigned int chan_id;

    chan_id = g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].chan_id;
    debug_info = &g_ppm_chan_info.cnf_debug_info;
    logic_port = CPM_OM_CFG_COMM;

    /* 参数检查 */
    if ((len == 0) || (virt_addr == NULL)) {
        debug_info->usb_send_cb_abnormal_num++;
        return;
    }

    ppm_get_send_data_len(chan_id, len, &send_len, &phy_port);

    send_result = cpm_com_send(logic_port, virt_addr, phy_addr, send_len);
    if (send_result == CPM_SEND_ERR) { /* 当前通道已经发送失败，调用SOCP通道无数据搬运 */
        debug_info->usb_send_err_num++;
        debug_info->usb_send_err_len += send_len;
    } else if (send_result == CPM_SEND_FUNC_NULL) { /* 当前通道异常，扔掉所有数据 */
        debug_info->ppm_discard_num++;
        debug_info->ppm_discard_len += len;
    } else if (send_result == CPM_SEND_PARA_ERR) { /* 发送数据获取实地址异常 */
        debug_info->ppm_get_virt_err++;
        debug_info->ppm_get_virt_send_len += len;
    } else if (send_result == CPM_SEND_AYNC) {
        send_succ_flag = true;
        usb_send_flag = true;
        ret = BSP_OK;
    } else if (send_result == CPM_SEND_OK) {
        scm_socp_send_data_to_udi_succ(chan_id, phy_port, debug_info, &send_len);

        send_succ_flag = true;
    } else {
        diag_error("cpm_com_send return Error(0x%x)\n", (s32)send_result);
    }

    if (usb_send_flag != true) {
        ret = scm_rls_cnf_dst_buff(send_len);
        if (ret != BSP_OK) {
            debug_info->socp_readdone_err_num++;
            debug_info->socp_readdone_err_len += send_len;

            diag_error("SCM_RlsDestBuf return Error(0x%x)\n", (s32)ret);
        }
    }

    if ((ret == BSP_OK) && (send_succ_flag == true)) {
        debug_info->usb_send_num++;
    }

    return;
}

void scm_send_ind_data_to_udi(u8 *virt_addr, u8 *phy_addr, u32 len)
{
    u32 send_result;
    u32 ret = ERR_MSP_FAILURE;
    unsigned int phy_port;
    u32 send_len;
    bool send_succ_flag = false;
    bool usb_send_flag = false;
    mdrv_ppm_port_debug_info_s *debug_info = NULL;
    unsigned int logic_port;
    socp_coder_dst_e chan_id;

    chan_id = g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].chan_id;
    debug_info = &g_ppm_chan_info.ind_debug_info;
    logic_port = CPM_OM_IND_COMM;

    /* 参数检查 */
    if ((len == 0) || (virt_addr == NULL)) {
        debug_info->usb_send_cb_abnormal_num++;
        return;
    }

    ppm_get_send_data_len(chan_id, len, &send_len, &phy_port);

    diag_system_debug_rev_socp_data(send_len);
    diag_throughput_save(EN_DIAG_THRPUT_DATA_CHN_PHY, send_len);
    send_result = cpm_com_send(logic_port, virt_addr, phy_addr, send_len);
    if (send_result == CPM_SEND_ERR) { /* 当前通道已经发送失败，调用SOCP通道无数据搬运 */
        debug_info->usb_send_err_num++;
        debug_info->usb_send_err_len += send_len;
        diag_system_debug_ind_dst_lost(EN_DIAG_DST_LOST_CPMWR, send_len);
    } else if (send_result == CPM_SEND_FUNC_NULL) { /* 当前通道异常，扔掉所有数据 */
        debug_info->ppm_discard_num++;
        debug_info->ppm_discard_len += len;
        diag_system_debug_ind_dst_lost(EN_DIAG_DST_LOST_CPMWR, send_len);
    } else if (send_result == CPM_SEND_PARA_ERR) { /* 发送数据获取实地址异常 */
        debug_info->ppm_get_virt_err++;
        debug_info->ppm_get_virt_send_len += len;
        diag_system_debug_ind_dst_lost(EN_DIAG_DST_LOST_CPMWR, send_len);
    } else if (send_result == CPM_SEND_AYNC) { // 增加cpm错误码
        send_succ_flag = true;
        usb_send_flag = true;
        ret = BSP_OK;
    } else if (send_result == CPM_SEND_OK) {
        send_succ_flag = true;
    } else {
        debug_info->ppm_discard_num++;
        debug_info->ppm_discard_len += len;
    }

    if (usb_send_flag != true) {
        ret = scm_rls_ind_dst_buff(send_len);
        if (ret != BSP_OK) {
            debug_info->socp_readdone_err_num++;
            debug_info->socp_readdone_err_len += send_len;

            diag_error("SCM_RlsDestBuf return Error 0x%x\n", (s32)ret);
        }
    }

    if ((ret == BSP_OK) && (send_succ_flag == true)) {
        debug_info->usb_send_num++;
        debug_info->usb_send_real_len += send_len;
    }

    return;
}

int scm_cnf_dst_read_cb(unsigned int chan_id)
{
    socp_buffer_rw_s buffer;
    unsigned long *virt_addr = NULL;
    u32 dst_chan;
    s32 ret;

    dst_chan = g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].chan_id;

    ret = bsp_socp_get_read_buff(dst_chan, &buffer);
    if (unlikely(ret != BSP_OK)) {
        diag_error("Get Read Buffer is Error\n");
        return ERR_MSP_INVALID_PARAMETER;
    }

    /* 开机log功能，IND通道上报函数为空，使log缓存在本地 */
    if (g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].func == NULL) {
        diag_error("cnf dst channel is null\n");
        return ERR_MSP_SUCCESS;
    }

    if (unlikely((buffer.size + buffer.rb_size) == 0) || (buffer.buffer == NULL)) {
        bsp_socp_read_data_done(dst_chan, buffer.size + buffer.rb_size); /* 清空数据 */
        diag_error("Get RD error\n");                                    /* 记录Log */
        return ERR_MSP_SUCCESS;
    }

    if (buffer.size == 0) {
        return ERR_MSP_SUCCESS;
    }

    /* 发送数据 */
    virt_addr = scm_uncache_mem_phy_to_virt((u8 *)buffer.buffer, g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].dst_phy_buff,
        g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].dst_virt_buff, g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].buff_len);
    if (virt_addr == NULL) {
        bsp_socp_read_data_done(dst_chan, buffer.size + buffer.rb_size); /* 清空数据 */
        diag_error("stBuffer.buffer==NULL\n");
        return ERR_MSP_MALLOC_FAILUE;
    }

    g_scm_dst_chan_cfg[SCM_SOCP_CNF_DST_CHAN].func((u8 *)virt_addr, (u8 *)buffer.buffer, (u32)buffer.size);

    return ERR_MSP_SUCCESS;
}

/*
 * 函 数 名: scm_ind_dst_read_cb
 * 功能描述: 处理编码目的通道的数据
 * 输入参数: ulDstChID 目的通道ID
 */
int scm_ind_dst_read_cb(unsigned int chan_id)
{
    socp_buffer_rw_s buffer;
    unsigned long *virt_addr = NULL;
    u32 dst_chan;

    dst_chan = g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].chan_id;

    if (bsp_socp_get_read_buff(dst_chan, &buffer) != BSP_OK) {
        diag_error("Get Read Buffer is Error\n"); /* 记录Log */
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (((buffer.size + buffer.rb_size) == 0) || (buffer.buffer == NULL)) {
        bsp_socp_read_data_done(dst_chan, buffer.size + buffer.rb_size); /* 清空数据 */
        diag_system_debug_ind_dst_lost(EN_DIAG_DST_LOST_BRANCH, buffer.size + buffer.rb_size);
        diag_error("Get RD error\n");
        return ERR_MSP_SUCCESS;
    }

    if (buffer.size == 0) {
        return ERR_MSP_SUCCESS;
    }

    /* 发送数据 */
    virt_addr = scm_uncache_mem_phy_to_virt((u8 *)buffer.buffer, g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].dst_phy_buff,
        g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].dst_virt_buff, g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].buff_len);
    if (virt_addr == NULL) {
        bsp_socp_read_data_done(dst_chan, buffer.size + buffer.rb_size); /* 清空数据 */
        diag_system_debug_ind_dst_lost(EN_DIAG_DST_LOST_BRANCH, buffer.size + buffer.rb_size);
        diag_error("stBuffer.pBuffe==NULL\n");
        return ERR_MSP_MALLOC_FAILUE;
    }

    g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].func((u8 *)virt_addr, (u8 *)buffer.buffer, (u32)buffer.size);

    return ERR_MSP_SUCCESS;
}

u32 scm_ind_get_dst_buff_size(void)
{
    return g_scm_dst_chan_cfg[SCM_SOCP_IND_DST_CHAN].buff_len;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
