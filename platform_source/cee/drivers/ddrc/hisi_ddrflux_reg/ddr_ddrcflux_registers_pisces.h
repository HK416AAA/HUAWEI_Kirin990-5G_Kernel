/*
 * ddr_ddrcflux_registers_pisces.h
 *
 * ddrcflux registers message for pisces
 *
 * Copyright (c) 2012-2021 Huawei Technologies Co., Ltd.
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
#ifndef __DDR_DDRCFLUX_REGISTERS_PISCES_H__
#define __DDR_DDRCFLUX_REGISTERS_PISCES_H__

#define PLATFORM_LIST \
	{ .compatible = "pisces-ddrc-flux", }, \
	{ },

#define DDRC_PLATFORM_NAME	PISCES
#define DDRC_IRQ_CPU_CORE_NUM	8
#define DDRC_DMC_NUM	2
#define DDRC_ASI_PORT_NUM	17
#define DDRC_QOSB_NUM	0

#define DDR_QOSB

#define ddr_reg_dmc_addr(m, n) DDR_REG_DMC((m << 1) + n)

/* timer5 control info */
#define TIMER_SCTRL_SCPEREN1_GT_CLOCK \
	(BIT(SOC_SCTRL_SCPEREN1_gt_pclk_timer5_START) | BIT(SOC_SCTRL_SCPEREN1_gt_clk_timer5_START))
#define TIMER_LOAD_OFFSET	0x20
#define TIMER_VALUE_OFFSET	0x24
#define TIMER_CTRL_OFFSET	0x28
#define TIMER_INTCLR_OFFSET	0x2c
#define TIMER_RIS_OFFSET	0x30
#define TIMER_BGLOAD_OFFSET	0x38
#define TIMER_CTRL_32BIT	BIT(1)
#define TIMER_CTRL_IE	BIT(5)
#define TIMER_CTRL_PERIODIC	BIT(6)
#define TIMER_CTRL_ENABLE	BIT(7)
#define BW_TIMER_DEFAULT_LOAD	0xFFFFFFFF
#define MAX_DATA_OF_32BIT	0xFFFFFFFF

#define DDR_FLUX_DMSS_ASI_DFX_OSTD \
	ddr_flux(DMSS, (0x70c + 0x800 * 0), 1 * 4, "ASI_DFX_OSTD_0,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 1), 1 * 4, "ASI_DFX_OSTD_1,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 2), 1 * 4, "ASI_DFX_OSTD_2,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 3), 1 * 4, "ASI_DFX_OSTD_3,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 4), 1 * 4, "ASI_DFX_OSTD_4,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 5), 1 * 4, "ASI_DFX_OSTD_5,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 6), 1 * 4, "ASI_DFX_OSTD_6,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 7), 1 * 4, "ASI_DFX_OSTD_7,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 8), 1 * 4, "ASI_DFX_OSTD_8,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 9), 1 * 4, "ASI_DFX_OSTD_9,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 10), 1 * 4, "ASI_DFX_OSTD_10,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 11), 1 * 4, "ASI_DFX_OSTD_11,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 12), 1 * 4, "ASI_DFX_OSTD_12,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 13), 1 * 4, "ASI_DFX_OSTD_13,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 14), 1 * 4, "ASI_DFX_OSTD_14,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 15), 1 * 4, "ASI_DFX_OSTD_15,"), \
	ddr_flux(DMSS, (0x70c + 0x800 * 16), 1 * 4, "ASI_DFX_OSTD_16,"),

#define DDR_FLUX_DMSS_ASI_STAT_FLUX_WR \
	ddr_flux(DMSS, (0x218 + 0x800 * 0), 1 * 4, "ASI_STAT_FLUX_WR_0,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 1), 1 * 4, "ASI_STAT_FLUX_WR_1,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 2), 1 * 4, "ASI_STAT_FLUX_WR_2,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 3), 1 * 4, "ASI_STAT_FLUX_WR_3,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 4), 1 * 4, "ASI_STAT_FLUX_WR_4,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 5), 1 * 4, "ASI_STAT_FLUX_WR_5,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 6), 1 * 4, "ASI_STAT_FLUX_WR_6,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 7), 1 * 4, "ASI_STAT_FLUX_WR_7,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 8), 1 * 4, "ASI_STAT_FLUX_WR_8,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 9), 1 * 4, "ASI_STAT_FLUX_WR_9,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 10), 1 * 4, "ASI_STAT_FLUX_WR_10,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 11), 1 * 4, "ASI_STAT_FLUX_WR_11,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 12), 1 * 4, "ASI_STAT_FLUX_WR_12,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 13), 1 * 4, "ASI_STAT_FLUX_WR_13,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 14), 1 * 4, "ASI_STAT_FLUX_WR_14,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 15), 1 * 4, "ASI_STAT_FLUX_WR_15,"), \
	ddr_flux(DMSS, (0x218 + 0x800 * 16), 1 * 4, "ASI_STAT_FLUX_WR_16,"),

#define DDR_FLUX_DMSS_ASI_STAT_FLUX_RD \
	ddr_flux(DMSS, (0x21c + 0x800 * 0), 1 * 4, "ASI_STAT_FLUX_RD_0,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 1), 1 * 4, "ASI_STAT_FLUX_RD_1,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 2), 1 * 4, "ASI_STAT_FLUX_RD_2,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 3), 1 * 4, "ASI_STAT_FLUX_RD_3,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 4), 1 * 4, "ASI_STAT_FLUX_RD_4,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 5), 1 * 4, "ASI_STAT_FLUX_RD_5,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 6), 1 * 4, "ASI_STAT_FLUX_RD_6,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 7), 1 * 4, "ASI_STAT_FLUX_RD_7,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 8), 1 * 4, "ASI_STAT_FLUX_RD_8,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 9), 1 * 4, "ASI_STAT_FLUX_RD_9,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 10), 1 * 4, "ASI_STAT_FLUX_RD_10,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 11), 1 * 4, "ASI_STAT_FLUX_RD_11,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 12), 1 * 4, "ASI_STAT_FLUX_RD_12,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 13), 1 * 4, "ASI_STAT_FLUX_RD_13,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 14), 1 * 4, "ASI_STAT_FLUX_RD_14,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 15), 1 * 4, "ASI_STAT_FLUX_RD_15,"), \
	ddr_flux(DMSS, (0x21c + 0x800 * 16), 1 * 4, "ASI_STAT_FLUX_RD_16,"),

#define DDR_FLUX_DMC \
	ddr_flux(DMC00, 0x940, 1 * 4, "DDRC_HIS_FLUX_WR_00,"), \
	ddr_flux(DMC00, 0x944, 1 * 4, "DDRC_HIS_FLUX_RD_00,"), \
	ddr_flux(DMC00, 0x948, 1 * 4, "DDRC_HIS_FLUX_WCMD_00,"), \
	ddr_flux(DMC00, 0x94C, 1 * 4, "DDRC_HIS_FLUX_RCMD_00,"), \
	ddr_flux(DMC00, 0x950, 1 * 4, "DDRC_HIS_FLUXID_WR_00,"), \
	ddr_flux(DMC00, 0x954, 1 * 4, "DDRC_HIS_FLUXID_RD_00,"), \
	ddr_flux(DMC00, 0x958, 1 * 4, "DDRC_HIS_FLUXID_WCMD_00,"), \
	ddr_flux(DMC00, 0x95C, 1 * 4, "DDRC_HIS_FLUXID_RCMD_00,"), \
	ddr_flux(DMC00, 0x960, 1 * 4, "DDRC_HIS_WLATCNT0_00,"), \
	ddr_flux(DMC00, 0x964, 1 * 4, "DDRC_HIS_WLATCNT1_00,"), \
	ddr_flux(DMC00, 0x968, 1 * 4, "DDRC_HIS_RLATCNT0_00,"), \
	ddr_flux(DMC00, 0x96C, 1 * 4, "DDRC_HIS_RLATCNT1_00,"), \
	ddr_flux(DMC00, 0x970, 1 * 4, "DDRC_HIS_INHERE_RLAT_CNT_00,"), \
	ddr_flux(DMC00, 0x974, 1 * 4, "DDRC_HIS_CMD_SUM_00,"), \
	ddr_flux(DMC00, 0x978, 1 * 4, "DDRC_HIS_DAT_SUM_00,"), \
	ddr_flux(DMC00, 0x97C, 1 * 4, "DDRC_HIS_PRE_CMD_00,"), \
	ddr_flux(DMC00, 0x980, 1 * 4, "DDRC_HIS_ACT_CMD_00,"), \
	ddr_flux(DMC00, 0x984, 1 * 4, "DDRC_HIS_BNK_CHG_00,"), \
	ddr_flux(DMC00, 0x988, 1 * 4, "DDRC_HIS_RNK_CHG_00,"), \
	ddr_flux(DMC00, 0x98C, 1 * 4, "DDRC_HIS_RW_CHG_00,"), \
	ddr_flux(DMC01, 0x940, 1 * 4, "DDRC_HIS_FLUX_WR_01,"), \
	ddr_flux(DMC01, 0x944, 1 * 4, "DDRC_HIS_FLUX_RD_01,"), \
	ddr_flux(DMC01, 0x948, 1 * 4, "DDRC_HIS_FLUX_WCMD_01,"), \
	ddr_flux(DMC01, 0x94C, 1 * 4, "DDRC_HIS_FLUX_RCMD_01,"), \
	ddr_flux(DMC01, 0x950, 1 * 4, "DDRC_HIS_FLUXID_WR_01,"), \
	ddr_flux(DMC01, 0x954, 1 * 4, "DDRC_HIS_FLUXID_RD_01,"), \
	ddr_flux(DMC01, 0x958, 1 * 4, "DDRC_HIS_FLUXID_WCMD_01,"), \
	ddr_flux(DMC01, 0x95C, 1 * 4, "DDRC_HIS_FLUXID_RCMD_01,"), \
	ddr_flux(DMC01, 0x960, 1 * 4, "DDRC_HIS_WLATCNT0_01,"), \
	ddr_flux(DMC01, 0x964, 1 * 4, "DDRC_HIS_WLATCNT1_01,"), \
	ddr_flux(DMC01, 0x968, 1 * 4, "DDRC_HIS_RLATCNT0_01,"), \
	ddr_flux(DMC01, 0x96C, 1 * 4, "DDRC_HIS_RLATCNT1_01,"), \
	ddr_flux(DMC01, 0x970, 1 * 4, "DDRC_HIS_INHERE_RLAT_CNT_01,"), \
	ddr_flux(DMC01, 0x974, 1 * 4, "DDRC_HIS_CMD_SUM_01,"), \
	ddr_flux(DMC01, 0x978, 1 * 4, "DDRC_HIS_DAT_SUM_01,"), \
	ddr_flux(DMC01, 0x97C, 1 * 4, "DDRC_HIS_PRE_CMD_01,"), \
	ddr_flux(DMC01, 0x980, 1 * 4, "DDRC_HIS_ACT_CMD_01,"), \
	ddr_flux(DMC01, 0x984, 1 * 4, "DDRC_HIS_BNK_CHG_01,"), \
	ddr_flux(DMC01, 0x988, 1 * 4, "DDRC_HIS_RNK_CHG_01,"), \
	ddr_flux(DMC01, 0x98C, 1 * 4, "DDRC_HIS_RW_CHG_01\n"),

#endif
