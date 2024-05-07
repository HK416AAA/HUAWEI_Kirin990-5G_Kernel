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

#include "socp_hal_common.h"
#include <of.h>
#include "bsp_print.h"
#include "bsp_socp.h"

#ifdef __cplusplus
extern "C" {
#endif

socp_hal_info_s g_socp_hal_info;

s32 socp_hal_init(void)
{
    g_socp_hal_info.dev = bsp_dt_find_compatible_node(NULL, NULL, "hisilicon,socp_balong_app");
    if (g_socp_hal_info.dev == NULL) {
        socp_error("socp dev find failed\n");
        return BSP_ERROR;
    }

    g_socp_hal_info.base_addr = bsp_dt_iomap(g_socp_hal_info.dev, 0);
    if (g_socp_hal_info.base_addr == NULL) {
        socp_error("socp base addr init failed\n");
        return BSP_ERROR;
    }

    g_socp_hal_info.socp_version = SOCP_REG_READ(SOCP_REG_SOCP_VERSION);

    socp_hal_global_reset();
    while (socp_hal_get_global_reset_state() != 0) {
        ;
    }

    return BSP_OK;
}

/* lint -save -e679 -e647 */
void socp_hal_unmask_header_error_int(void)
{
    /* 打开socp源通道包头错中断 */
    SOCP_REG_WRITE(SOCP_REG_APP_MASK1, 0);
}

void socp_hal_get_header_error_int_state(u32 *low_chan, u32 *high_chan)
{
    *low_chan = SOCP_REG_READ(SOCP_REG_APP_INTSTAT1);
}

void socp_hal_clear_header_error_raw_int(u32 low_chan, u32 high_chan)
{
    SOCP_REG_WRITE(SOCP_REG_ENC_RAWINT1, low_chan);
}

void socp_hal_set_header_error_int_mask(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_APP_MASK1, chan_id, 1, value);
}

void socp_hal_set_rate_ctrl_cfg(u32 value1, u32 value2)
{
    return;
}

void socp_hal_set_rate_ctrl_peroid(u32 value)
{
    return;
}

void socp_hal_set_dst_chan_trans_id_en(u32 chan_id, u32 value)
{
    return;
}

void socp_hal_set_trans_id_en(u32 chan_id, u32 value)
{
    return;
}

void socp_hal_set_rptr_img_addr(u32 chan_id, u32 addr_low, u32 addr_high)
{
    return;
}

void socp_hal_set_src_rd_timeout(u32 chan_id, u32 value)
{
    return;
}

void socp_hal_set_rate_ctrl_group(u32 chan_id, u32 value)
{
    return;
}

void socp_hal_clear_src_chan_cfg(u32 chan_id)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFCFG(chan_id), 0x0);
}

void socp_hal_clear_rate_ctrl_cnt(u32 chan_id)
{
    return;
}

void socp_hal_clear_src_rd_raw_int(u32 chan_id)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT3, chan_id, 1, 1);
}

u32 socp_hal_get_src_state(void)
{
    return (SOCP_REG_READ(SOCP_REG_ENCSTAT));
}

u32 socp_hal_get_single_src_state(u32 chan_id)
{
    return SOCP_REG_GETBITS(SOCP_REG_ENCSTAT, chan_id, 1);
}

s32 socp_hal_get_rate_ctrl_cnt(u32 chan_id)
{
    return 0;
}

void socp_hal_set_single_src_rd_timeout_int_mask(u32 chan_id, u32 value)
{
    return;
}

void socp_hal_clear_src_rd_timout_raw_int(u32 chan_id)
{
    return;
}

void socp_hal_set_single_src_rd_int_mask(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_APP_MASK3, chan_id, 1, value);
}

void socp_hal_set_rate_ctrl_threshold(u32 rate_limits)
{
    return;
}

void socp_hal_src_chan_init(u32 chan_id, socp_src_chan_cfg_s *src_attr)
{
    u32 buf_len, rd_buf_len;

    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 1, 2, (u32)src_attr->mode);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 4, 2, SOCP_REAL_CHAN_ID(src_attr->dest_chan_id));
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 8, 2, (u32)src_attr->priority);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 10, 1, (u32)src_attr->bypass_en);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 11, 1, (u32)src_attr->data_type_en);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 16, 8, (u32)src_attr->data_type);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 31, 1, (u32)src_attr->debug_en);
    buf_len = (uintptr_t)src_attr->coder_src_setbuf.input_end - (uintptr_t)src_attr->coder_src_setbuf.input_start + 1;
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFDEPTH(chan_id), buf_len);

    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFADDR_L(chan_id), (uintptr_t)src_attr->coder_src_setbuf.input_start);
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFADDR_H(chan_id), 0);

    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFWPTR(chan_id), 0);
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFRPTR(chan_id), 0);

    if (src_attr->mode == SOCP_ENCSRC_CHNMODE_LIST) {
        rd_buf_len = (uintptr_t)src_attr->coder_src_setbuf.rd_input_end -
            (uintptr_t)src_attr->coder_src_setbuf.rd_input_start + 1;

        SOCP_REG_SETBITS(SOCP_REG_ENCSRC_RDQCFG(chan_id), 0, 16, rd_buf_len);
        SOCP_REG_SETBITS(SOCP_REG_ENCSRC_RDQCFG(chan_id), 16, 16, 0);

        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQADDR_L(chan_id), (uintptr_t)src_attr->coder_src_setbuf.rd_input_start);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQADDR_H(chan_id), 0);

        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQWPTR(chan_id), 0);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQRPTR(chan_id), 0);
    }
}


/* lint -restore +e679 +e647 */
#ifdef __cplusplus
}
#endif
