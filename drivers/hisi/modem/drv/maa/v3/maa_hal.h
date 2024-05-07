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

#ifndef __MAA_HAL_H__
#define __MAA_HAL_H__
#include "maa.h"
#include "bsp_maa.h"

void __maa_release_pool_init(struct maa *maa);
void maa_fifo_init(struct maa *maa, struct maa_fifo *fifo, int level);
void maa_force_fifo_full(struct maa *maa, struct maa_fifo *fifo, int level);
int maa_ipipe_init(struct maa *maa, struct maa_ipipe *ipipe, unsigned int idx);
void maa_interupt_init(struct maa *maa);
void maa_ipipe_read_update(struct maa *maa, unsigned int rptr, unsigned int idx);
void maa_input(struct maa *maa, unsigned long long buf, struct maa_opipe *opipe);
unsigned int maa_get_space(struct maa *maa, struct maa_opipe *opipe);
unsigned int maa_get_rls(struct maa *maa);
void maa_set_rls(struct maa *maa, unsigned int rptr);
void maa_set_intr(struct maa *maa);
void maa_top_intr(struct maa *maa);
void maa_basic_cfg(struct maa *maa);
int maa_hal_suspend(struct maa *maa);
int maa_hal_resume(struct maa *maa);
void maa_hal_complete(struct maa *maa);
int maa_hal_prepare(struct maa *maa);
void maa_detach(struct maa *maa);
void maa_fifo_reinit(struct maa *maa, struct maa_fifo *fifo, int level);
void maa_hds_transreport_hal(struct maa *maa, struct maa_debug_info *dbg_info);
void maa_ddr_buf_fifo_init(struct maa *maa, struct maa_fifo *fifo, unsigned int level);
unsigned int maa_hal_set_adq(struct maa *maa, unsigned int level, unsigned long long adqbase, unsigned int write,
    unsigned int depth, unsigned int ipipe);

#endif
