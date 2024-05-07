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

#include "scm_fusion_common.h"

#include <securec.h>
#include "mdrv_socp.h"
#include "osl_malloc.h"
#include "bsp_socp.h"
#include "scm_fusion_chan_manager.h"
#include "scm_rate_ctrl.h"
#include "diag_port_manager.h"
#include "ppm_common.h"

#ifdef __cplusplus
extern "C" {
#endif

spinlock_t g_scm_spin_lock;

/*
 * 函 数 名: scm_uncache_mem_phy_to_virt
 * 功能描述: 根据输入的实地址，计算对应的虚地址
 * 输入参数: cur_phy_addr:  当前实地址
 * phy_start: 通道配置内存起始的实地址
 * virt_start:通道配置内存起始的虚地址
 * buf_len:    通道内存空间大小
 * 输出参数: 无
 * 返 回 值: NULL: 转换失败/other: 虚地址的值
 */
unsigned long *scm_uncache_mem_phy_to_virt(const u8 *cur_phy_addr, const u8 *phy_start, const u8 *virt_start,
    u32 buf_len)
{
    if (unlikely((cur_phy_addr < phy_start) || (cur_phy_addr >= (phy_start + buf_len)))) {
        diag_error("Phy to Virt error\n");

        return NULL;
    }

    return (unsigned long *)((cur_phy_addr - phy_start) + virt_start);
}
__init int scm_init(void)
{
    s32 ret;

    spin_lock_init(&g_scm_spin_lock);

    (void)scm_rate_ctrl_init();

    ret = scm_dst_chan_buff_init();
    if (ret) {
        return ret;
    }

    ret = scm_dst_chan_cfg_init();
    if (ret) {
        return ret;
    }

    scm_reg_cnf_coder_dst_send_func();
    scm_reg_ind_coder_dst_send_func();

    ret = scm_src_chan_buff_init();
    if (ret) {
        return ret;
    }

    ret = scm_src_chan_cfg_init();
    if (ret) {
        return ret;
    }

    diag_crit("[init]scm_init ok\n");
    return BSP_OK;
}

s32 scm_get_poweron_log_enable(void)
{
    socp_encdst_buf_log_cfg_s log_cfg;

    memset_s(&log_cfg, sizeof(log_cfg), 0, sizeof(log_cfg));

    if (bsp_socp_get_log_cfg(&log_cfg) == BSP_OK) {
        return ((log_cfg.buffer_size < POWERON_LOG_THRESHOLD) ? BSP_FALSE : BSP_TRUE);
    }

    return BSP_ERROR;
}

/*
 * 函 数 名: scm_socp_send_data_to_udi_succ
 * 功能描述: 把数据从SOCP通道的缓冲中发送到指定的端口执行成功
 * 输入参数: enChanID       目的通道号
 * phy_port      物理端口号
 * pstDebugInfo   可维可测信息结构指针
 * pulSendDataLen 预期发送的长度
 * 输出参数: pulSendDataLen 实际发送的长度
 * 返 回 值: void
 */
void scm_socp_send_data_to_udi_succ(u32 chan_id, unsigned int phy_port, mdrv_ppm_port_debug_info_s *debug_info,
    u32 *send_len)
{
    ppm_pseudo_sync_s *usb_ind_pseudo_sync = ppm_get_ind_pseudo_sync_info();
    ppm_pseudo_sync_s *usb_cfg_pseudo_sync = ppm_get_cfg_pseudo_sync_info();
    if ((chan_id == SOCP_CODER_DST_OM_CNF) && (phy_port == CPM_CFG_PORT)) {
        if ((usb_cfg_pseudo_sync->length == 0) || (*send_len != usb_cfg_pseudo_sync->length)) {
            debug_info->usb_send_cb_abnormal_num++;
            debug_info->usb_send_cb_abnormal_len += *send_len;
        }

        *send_len = usb_cfg_pseudo_sync->length;
    } else if ((chan_id == SOCP_CODER_DST_OM_IND) && (phy_port == CPM_IND_PORT)) {
        if ((usb_ind_pseudo_sync->length == 0) || (*send_len != usb_ind_pseudo_sync->length)) {
            debug_info->usb_send_cb_abnormal_num++;
            debug_info->usb_send_cb_abnormal_len += *send_len;
        }

        *send_len = usb_ind_pseudo_sync->length;
    }

    return;
}

#ifdef __cplusplus
}
#endif
