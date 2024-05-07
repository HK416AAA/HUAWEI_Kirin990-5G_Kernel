/** @file
 * Copyright (c) 2020-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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
#ifndef HISI_DISP_ISR_OFFLINE_H
#define HISI_DISP_ISR_OFFLINE_H

#include <linux/types.h>
#include <linux/of.h>
#include "hisi_isr_utils.h"


void hisi_offline_isr_init(struct hisi_disp_isr *isr, const uint32_t irq_no,
	uint32_t *irq_state, uint32_t state_count, const char *irq_name, void *parent);

#endif /* HISI_DISP_ISR_OFFLINE_H */