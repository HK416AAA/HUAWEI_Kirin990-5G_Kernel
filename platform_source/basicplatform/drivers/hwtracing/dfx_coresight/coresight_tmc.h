/*
 * coresight-tmc.h
 *
 * Coresight Traffic Message Channel module
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <linux/types.h>

#ifndef _CORESIGHT_CORESIGHT_TMC_H
#define _CORESIGHT_CORESIGHT_TMC_H

#define TMC_ETR  "etr"
#define TMC_ETB  "etb"
#define TMC_ETF  "etf"

#define MAX_TMC_DEV 5
#define BYTES_PER_WORD          4

#define     HEADER_SIZE         (48)
#define     TMC_MAGIC_START     "TMCDUMP"
#define     TMC_MAGIC_SIZE      (sizeof(TMC_MAGIC_START))
#define     TMC_TIME_LEN        sizeof(u64)
#define     TMC_NAME_LEN        (HEADER_SIZE-TMC_MAGIC_SIZE-4-TMC_TIME_LEN)
#define     BUFF_LEN            (0x80)

struct tcm_dump_head {
	char magic[TMC_MAGIC_SIZE];
	u64 ts_nsec;
	u32 data_size;
	char name[TMC_NAME_LEN];
};
#ifdef CONFIG_DFX_CORESIGHT_TRACE
extern void set_etr_trace_info(u64 paddr, u32 size, u32 offset);
#else
static inline void set_etr_trace_info(u64 paddr, u32 size, u32 offset){}
#endif

#endif
