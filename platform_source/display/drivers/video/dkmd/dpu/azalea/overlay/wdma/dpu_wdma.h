/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef DPU_WDMA_H
#define DPU_WDMA_H

#include "../../dpu_fb.h"

void dpu_wdma_init(const char __iomem *wdma_base, dss_wdma_t *s_wdma);
void dpu_wdma_set_reg(struct dpu_fb_data_type *dpufd,
	char __iomem *wdma_base, dss_wdma_t *s_wdma);

int dpu_wdma_config(struct dpu_fb_data_type *dpufd,
	dss_wb_layer_t *layer, dss_rect_t aligned_rect, dss_rect_t *ov_block_rect, bool last_block);

#endif /* DPU_WDMA_H */
