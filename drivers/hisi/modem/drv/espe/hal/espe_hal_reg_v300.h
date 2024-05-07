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

#ifndef __SPE_REG_V300_H__
#define __SPE_REG_V300_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SPE_DBGEN 0x0004
#define RESERVED 0x001C
#define SPE_TIME 0x0030
#define SPE_IPF_BD_RD_IP_HEAD 0x0034
#define SPE_QOS_LMTTIME 0x00B0
#define SPE_TD_EMPTY_SEL 0x00B4

#define SPE_RDQ_FULL_INTC_STATUS 0x0274
#define SPE_RD_DONE_INTC_STATUS 0x0278
#define SPE_TD_DONE_INTC_STATUS 0x027C
#define SPE_ADQ_EMPTY_INTC_STATUS 0x0280
#define SPE_BUS_ERR_INTC_STATUS 0x0284
#define SPE_STICK_MODE 0x061c
#define SPE_IPF_UL_CNT 0x07AC
#define SPE_IPF_DL_CNT 0x07B0

#define SPE_ADQ_EN(x) (0x3078 + 4 * (x))

enum spe_tab_ctrl_type {
    SPE_TAB_CTRL_ADD_MAC_FILT = 0,
    SPE_TAB_CTRL_ADD_MAC_FW,
    SPE_TAB_CTRL_ADD_IPV4_FILT,
    SPE_TAB_CTRL_ADD_IPV6_FILT,
    SPE_TAB_CTRL_DEL_MAC_FILT,
    SPE_TAB_CTRL_DEL_MAC_FW,
    SPE_TAB_CTRL_DEL_IPV4_FILT,
    SPE_TAB_CTRL_DEL_IPV6_FILT,
    SPE_TAB_CTRL_DEL_HASH_CACHE_ENTRY,
    SPE_TAB_CTRL_ADD_QOS_MAC_LMT,
    SPE_TAB_CTRL_DEL_QOS_MAC_LMT,
    SPE_TAB_CTRL_BOTTOM
};

/* add by B5010 */
typedef union {
    struct {
        unsigned int stick_en : 1;
        unsigned int bypass_en : 1;
        unsigned int bypass_dport : 4;
        unsigned int reserved : 26;
    } bits;
    unsigned int u32;
} spe_stick_mode_t;

typedef union {
    struct {
        unsigned int qos_limit_period : 20;
        unsigned int qos_limit_en : 1;
        unsigned int reserved : 11;
    } bits;
    unsigned int u32;
} spe_qos_lmttime_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SPE_REG_V300_H__ */
