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
#ifndef DPU_OVERLAY_UTILS_ARRAY_DSSV330_H
#define DPU_OVERLAY_UTILS_ARRAY_DSSV330_H

#if defined(CONFIG_DPU_FB_V320)
#include "../dpu_display_effect_dssv320.h"
#elif defined(CONFIG_DPU_FB_V330)
#include "../dpu_display_effect_dssv330.h"
#endif
#include "../dpu_smmu_base.h"

static uint32_t g_dss_module_base[DSS_CHN_MAX_DEFINE][MODULE_CHN_MAX] = {
	/* D0 */
	{
	MIF_CH0_OFFSET,  /* MODULE_MIF_CHN */
	AIF0_CH0_OFFSET,  /* MODULE_AIF0_CHN */
	AIF1_CH0_OFFSET,  /* MODULE_AIF1_CHN */
	MCTL_CTL_MUTEX_RCH0,  /* MODULE_MCTL_CHN_MUTEX */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH0_FLUSH_EN,  /* MODULE_MCTL_CHN_FLUSH_EN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH0_OV_OEN,  /* MODULE_MCTL_CHN_OV_OEN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH0_STARTY,  /* MODULE_MCTL_CHN_STARTY */
	DSS_MCTRL_SYS_OFFSET + MCTL_MOD0_DBG,  /* MODULE_MCTL_CHN_MOD_DBG */
	DSS_RCH_D0_DMA_OFFSET,  /* MODULE_DMA */
	DSS_RCH_D0_DFC_OFFSET,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	DSS_RCH_D0_CSC_OFFSET,  /* MODULE_CSC */
	},

	/* D1 */
	{
	MIF_CH1_OFFSET,  /* MODULE_MIF_CHN */
	AIF0_CH1_OFFSET,  /* MODULE_AIF0_CHN */
	AIF1_CH1_OFFSET,  /* MODULE_AIF1_CHN */
	MCTL_CTL_MUTEX_RCH1,  /* MODULE_MCTL_CHN_MUTEX */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH1_FLUSH_EN, /* MODULE_MCTL_CHN_FLUSH_EN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH1_OV_OEN,  /* MODULE_MCTL_CHN_OV_OEN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH1_STARTY,  /* MODULE_MCTL_CHN_STARTY */
	DSS_MCTRL_SYS_OFFSET + MCTL_MOD1_DBG,  /* MODULE_MCTL_CHN_MOD_DBG */
	DSS_RCH_D1_DMA_OFFSET,  /* MODULE_DMA */
	DSS_RCH_D1_DFC_OFFSET,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	DSS_RCH_D1_CSC_OFFSET,  /* MODULE_CSC */
	},

	/* V0 */
	{
	0,  /* MODULE_MIF_CHN */
	0,  /* MODULE_AIF0_CHN */
	0,  /* MODULE_AIF1_CHN */
	0,  /* MODULE_MCTL_CHN_MUTEX */
	0,  /* MODULE_MCTL_CHN_FLUSH_EN */
	0,  /* MODULE_MCTL_CHN_OV_OEN */
	0,  /* MODULE_MCTL_CHN_STARTY */
	0,  /* MODULE_MCTL_CHN_MOD_DBG */
	0,  /* MODULE_DMA */
	0,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	0,  /* MODULE_CSC */
	},

	/* G0 */
	{
	0,  /* MODULE_MIF_CHN */
	0,  /* MODULE_AIF0_CHN */
	0,  /* MODULE_AIF1_CHN */
	0,  /* MODULE_MCTL_CHN_MUTEX */
	0,  /* MODULE_MCTL_CHN_FLUSH_EN */
	0,  /* MODULE_MCTL_CHN_OV_OEN */
	0,  /* MODULE_MCTL_CHN_STARTY */
	0,  /* MODULE_MCTL_CHN_MOD_DBG */
	0,  /* MODULE_DMA */
	0,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	0,  /* MODULE_CSC */
	},

	/* V1 */
	{
	MIF_CH4_OFFSET,  /* MODULE_MIF_CHN */
	AIF0_CH4_OFFSET,  /* MODULE_AIF0_CHN */
	AIF1_CH4_OFFSET,  /* MODULE_AIF1_CHN */
	MCTL_CTL_MUTEX_RCH4,  /* MODULE_MCTL_CHN_MUTEX */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH4_FLUSH_EN,  /* MODULE_MCTL_CHN_FLUSH_EN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH4_OV_OEN,  /* MODULE_MCTL_CHN_OV_OEN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH4_STARTY,  /* MODULE_MCTL_CHN_STARTY */
	DSS_MCTRL_SYS_OFFSET + MCTL_MOD4_DBG,  /* MODULE_MCTL_CHN_MOD_DBG */
	DSS_RCH_VG1_DMA_OFFSET,  /* MODULE_DMA */
	DSS_RCH_VG1_DFC_OFFSET,  /* MODULE_DFC */
	DSS_RCH_VG1_SCL_OFFSET,  /* MODULE_SCL */
	DSS_RCH_VG1_SCL_LUT_OFFSET,  /* MODULE_SCL_LUT */
	DSS_RCH_VG1_ARSR_OFFSET,  /* MODULE_ARSR2P */
	DSS_RCH_VG1_ARSR_LUT_OFFSET,  /* MODULE_ARSR2P_LUT */
	DSS_RCH_VG1_POST_CLIP_OFFSET_ES,  /* MODULE_POST_CLIP_ES */
	DSS_RCH_VG1_POST_CLIP_OFFSET,  /* MODULE_POST_CLIP */
	DSS_RCH_VG1_PCSC_OFFSET,  /* MODULE_PCSC */
	DSS_RCH_VG1_CSC_OFFSET,  /* MODULE_CSC */
	},

	/* G1 */
	{
	MIF_CH5_OFFSET,  /* MODULE_MIF_CHN */
	AIF0_CH5_OFFSET,  /* MODULE_AIF0_CHN */
	AIF1_CH5_OFFSET,  /* MODULE_AIF1_CHN */
	MCTL_CTL_MUTEX_RCH5,  /* MODULE_MCTL_CHN_MUTEX */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH5_FLUSH_EN,  /* MODULE_MCTL_CHN_FLUSH_EN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH5_OV_OEN,  /* MODULE_MCTL_CHN_OV_OEN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH5_STARTY,  /* MODULE_MCTL_CHN_STARTY */
	DSS_MCTRL_SYS_OFFSET + MCTL_MOD5_DBG,  /* MODULE_MCTL_CHN_MOD_DBG */
	DSS_RCH_G1_DMA_OFFSET,  /* MODULE_DMA */
	DSS_RCH_G1_DFC_OFFSET,  /* MODULE_DFC */
	DSS_RCH_G1_SCL_OFFSET,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	DSS_RCH_G1_POST_CLIP_OFFSET_ES,  /* MODULE_POST_CLIP_ES */
	DSS_RCH_G1_POST_CLIP_OFFSET,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	DSS_RCH_G1_CSC_OFFSET,  /* MODULE_CSC */
	},

	/* D2 */
	{
	MIF_CH6_OFFSET,  /* MODULE_MIF_CHN */
	AIF0_CH6_OFFSET,  /* MODULE_AIF0_CHN */
	AIF1_CH6_OFFSET,  /* MODULE_AIF1_CHN */
	MCTL_CTL_MUTEX_RCH6,   /* MODULE_MCTL_CHN_MUTEX */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH6_FLUSH_EN,  /* MODULE_MCTL_CHN_FLUSH_EN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH6_OV_OEN,  /* MODULE_MCTL_CHN_OV_OEN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH6_STARTY,  /* MODULE_MCTL_CHN_STARTY */
	DSS_MCTRL_SYS_OFFSET + MCTL_MOD6_DBG,  /* MODULE_MCTL_CHN_MOD_DBG */
	DSS_RCH_D2_DMA_OFFSET,  /* MODULE_DMA */
	DSS_RCH_D2_DFC_OFFSET,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	DSS_RCH_D2_CSC_OFFSET,   /* MODULE_CSC */
	},

	/* D3 */
	{
	MIF_CH7_OFFSET,  /* MODULE_MIF_CHN */
	AIF0_CH7_OFFSET,  /* MODULE_AIF0_CHN */
	AIF1_CH7_OFFSET,  /* MODULE_AIF1_CHN */
	MCTL_CTL_MUTEX_RCH7,  /* MODULE_MCTL_CHN_MUTEX */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH7_FLUSH_EN,  /* MODULE_MCTL_CHN_FLUSH_EN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH7_OV_OEN,  /* MODULE_MCTL_CHN_OV_OEN */
	DSS_MCTRL_SYS_OFFSET + MCTL_RCH7_STARTY,  /* MODULE_MCTL_CHN_STARTY */
	DSS_MCTRL_SYS_OFFSET + MCTL_MOD7_DBG,  /* MODULE_MCTL_CHN_MOD_DBG */
	DSS_RCH_D3_DMA_OFFSET,  /* MODULE_DMA */
	DSS_RCH_D3_DFC_OFFSET,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	DSS_RCH_D3_CSC_OFFSET,  /* MODULE_CSC */
	},

	/* W0 */
	{
	MIF_CH8_OFFSET,  /* MODULE_MIF_CHN */
	AIF0_CH8_OFFSET,  /* MODULE_AIF0_CHN */
	AIF1_CH8_OFFSET,  /* MODULE_AIF1_CHN */
	MCTL_CTL_MUTEX_WCH0,  /* MODULE_MCTL_CHN_MUTEX */
	DSS_MCTRL_SYS_OFFSET + MCTL_WCH0_FLUSH_EN,   /* MODULE_MCTL_CHN_FLUSH_EN */
	DSS_MCTRL_SYS_OFFSET + MCTL_WCH0_OV_IEN,   /* MODULE_MCTL_CHN_OV_OEN */
	0,   /* MODULE_MCTL_CHN_STARTY */
	0,   /* MODULE_MCTL_CHN_MOD_DBG */
	DSS_WCH0_DMA_OFFSET,   /* MODULE_DMA */
	DSS_WCH0_DFC_OFFSET,   /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	DSS_WCH0_CSC_OFFSET,  /* MODULE_CSC */
	},

	/* W1 */
	{
	0,  /* MODULE_MIF_CHN */
	0,  /* MODULE_AIF0_CHN */
	0,  /* MODULE_AIF1_CHN */
	0,  /* MODULE_MCTL_CHN_MUTEX */
	0,  /* MODULE_MCTL_CHN_FLUSH_EN */
	0,  /* MODULE_MCTL_CHN_OV_OEN */
	0,  /* MODULE_MCTL_CHN_STARTY */
	0,  /* MODULE_MCTL_CHN_MOD_DBG */
	0,  /* MODULE_DMA */
	0,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0, /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	0,  /* MODULE_CSC */
	},

	/* V2 */
	{
	0,  /* MODULE_MIF_CHN */
	0,  /* MODULE_AIF0_CHN */
	0,  /* MODULE_AIF1_CHN */
	0,  /* MODULE_MCTL_CHN_MUTEX */
	0,  /* MODULE_MCTL_CHN_FLUSH_EN */
	0,  /* MODULE_MCTL_CHN_OV_OEN */
	0,  /* MODULE_MCTL_CHN_STARTY */
	0,  /* MODULE_MCTL_CHN_MOD_DBG */
	0,  /* MODULE_DMA */
	0,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	0,  /* MODULE_CSC */
	},

	/* W2 */
	{
	0,  /* MODULE_MIF_CHN */
	0,  /* MODULE_AIF0_CHN */
	0,  /* MODULE_AIF1_CHN */
	0,  /* MODULE_MCTL_CHN_MUTEX */
	0,  /* MODULE_MCTL_CHN_FLUSH_EN */
	0,  /* MODULE_MCTL_CHN_OV_OEN */
	0,  /* MODULE_MCTL_CHN_STARTY */
	0,  /* MODULE_MCTL_CHN_MOD_DBG */
	0,  /* MODULE_DMA */
	0,  /* MODULE_DFC */
	0,  /* MODULE_SCL */
	0,  /* MODULE_SCL_LUT */
	0,  /* MODULE_ARSR2P */
	0,  /* MODULE_ARSR2P_LUT */
	0,  /* MODULE_POST_CLIP_ES */
	0,  /* MODULE_POST_CLIP */
	0,  /* MODULE_PCSC */
	0,  /* MODULE_CSC */
	},
};

static uint32_t g_dss_module_ovl_base[DSS_MCTL_IDX_MAX][MODULE_OVL_MAX] = {
	{DSS_OVL0_OFFSET,
	DSS_MCTRL_CTL0_OFFSET},

	{0,
	0},

	{DSS_OVL2_OFFSET,
	DSS_MCTRL_CTL2_OFFSET},

	{0,
	0},

	{0,
	0},

	{0,
	0},
};

static uint32_t g_dss_module_cap[DSS_CHN_MAX_DEFINE][MODULE_CAP_MAX] = {
	/* D2 */
	{0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	/* D3 */
	{0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	/* V0 */
	{0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1},
	/* G0 */
	{0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	/* V1 */
	{0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
	/* G1 */
	{0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	/* D0 */
	{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	/* D1 */
	{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},

	/* W0 */
	{1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1},
	/* W1 */
	{1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1},

	/* V2 */
	{0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1},
	/* W2 */
	{1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1},
};

/* number of smrx idx for each channel */
static uint32_t g_dss_chn_sid_num[DSS_CHN_MAX_DEFINE] = {
	/* D0 D1 V0 G0 V1 G1 D2 D3 W0 W1 V2 W2 */
	4, 4, 0, 0, 4, 4, 1, 1, 3, 0, 0, 0
};

/* start idx of each channel
 * smrx_idx = g_dss_smmu_smrx_idx[chn_idx] + (0 ~ g_dss_chn_sid_num[chn_idx])
 */
static uint32_t g_dss_smmu_smrx_idx[DSS_CHN_MAX_DEFINE] = {
	/* D0 D1 V0 G0 V1 G1 D2 D3 W0 W1 V2 W2 */
	0, 4, 0, 0, 8, 12, 16, 17, 18, 0, 0, 0
};

#endif  /* DPU_OVERLAY_UTILS_ARRAY_DSSV330_H */
