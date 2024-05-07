/* Copyright (c) 2012-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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

#include "dpu_dpe_utils.h"
#include "dsc/dsc_algorithm_manager.h"
#include "dsc/utilities/dpu_debug.h"
#include <linux/sysctl.h>

static unsigned int g_comform_value;
static unsigned int g_acm_state;
static unsigned int g_gmp_state;
static unsigned int g_led_rg_csc_value[9];
static unsigned int g_is_led_rg_csc_set;
unsigned int g_led_rg_para1 = 7;
unsigned int g_led_rg_para2 = 30983;
#define GMP_CNT_COFE 4913  /* 17*17*17 */
#define XCC_CNT_COFE 12
#define COLOR_RECTIFY_DEFAULT 0x8000
#define CSC_VALUE_MIN_LEN 9
extern spinlock_t g_gmp_effect_lock;


//### from dpu_dpe_utils_dssv330.c
static uint32_t get_color_rectify_rgb(uint32_t color_rectify_rgb, uint8_t color_rectify_support)
{
	uint32_t rgb = COLOR_RECTIFY_DEFAULT;

	if (color_rectify_support && color_rectify_rgb && (color_rectify_rgb <= COLOR_RECTIFY_DEFAULT))
		rgb = color_rectify_rgb;

	return rgb;
}

static void xcc_set_coef(char __iomem *base_addr, struct dpu_panel_info *pinfo,
	struct rgb rectify_rgb)
{
	dpu_check_and_no_retval((!base_addr || !pinfo), ERR, "base_addr or pinfo is NULL\n");

	outp32(base_addr + LCP_XCC_COEF_00, pinfo->xcc_table[0]);
	outp32(base_addr + LCP_XCC_COEF_01, pinfo->xcc_table[1] * g_led_rg_csc_value[0] /
		COLOR_RECTIFY_DEFAULT * rectify_rgb.color_temp_rectify_r / COLOR_RECTIFY_DEFAULT);
	outp32(base_addr + LCP_XCC_COEF_02, pinfo->xcc_table[2]);
	outp32(base_addr + LCP_XCC_COEF_03, pinfo->xcc_table[3]);
	outp32(base_addr + LCP_XCC_COEF_10, pinfo->xcc_table[4]);
	outp32(base_addr + LCP_XCC_COEF_11, pinfo->xcc_table[5]);
	outp32(base_addr + LCP_XCC_COEF_12, pinfo->xcc_table[6] * g_led_rg_csc_value[4] /
		COLOR_RECTIFY_DEFAULT * rectify_rgb.color_temp_rectify_g / COLOR_RECTIFY_DEFAULT);
	outp32(base_addr + LCP_XCC_COEF_13, pinfo->xcc_table[7]);
	outp32(base_addr + LCP_XCC_COEF_20, pinfo->xcc_table[8]);
	outp32(base_addr + LCP_XCC_COEF_21, pinfo->xcc_table[9]);
	outp32(base_addr + LCP_XCC_COEF_22, pinfo->xcc_table[10]);
	outp32(base_addr + LCP_XCC_COEF_23, pinfo->xcc_table[11] * g_led_rg_csc_value[8] /
		COLOR_RECTIFY_DEFAULT * discount_coefficient(g_comform_value) /
		CHANGE_MAX * rectify_rgb.color_temp_rectify_b / COLOR_RECTIFY_DEFAULT);
}

static void dpe_init_xcc(struct dpu_fb_data_type *dpufd)
{
	char __iomem *xcc_base = NULL;
	struct dpu_panel_info *pinfo = NULL;
	struct rgb rectify_rgb;

	pinfo = &(dpufd->panel_info);
	xcc_base = dpufd->dss_base + DSS_DPP_XCC_OFFSET;

	rectify_rgb.color_temp_rectify_r = get_color_rectify_rgb(pinfo->color_temp_rectify_R,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_g = get_color_rectify_rgb(pinfo->color_temp_rectify_G,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_b = get_color_rectify_rgb(pinfo->color_temp_rectify_B,
		pinfo->color_temp_rectify_support);

	if (pinfo->xcc_support == 1) {
		/* XCC matrix */
		if (pinfo->xcc_table_len == GMP_CNT_COFE && pinfo->xcc_table) {
			xcc_set_coef(xcc_base, pinfo, rectify_rgb);
			/* enable xcc */
			set_reg(xcc_base + LCP_XCC_BYPASS_EN, 0x0, 1, 0);
		}
	}
}

static void set_lcp_param_gmp_table(struct lcp_info *lcp_param, char __iomem *gmp_lut_base)
{
	int i;
	const int STEP = 500;

	for (i = 0; i < GMP_CNT_COFE; i++) {
		set_reg(gmp_lut_base + i * 2 * 4, lcp_param->gmp_table_low32[i], 32, 0);
		set_reg(gmp_lut_base + i * 2 * 4 + 4, lcp_param->gmp_table_high4[i], 4, 0);
		if ((i % STEP) == 0)
			DPU_FB_INFO(
				"[effect]lcp_param gmp_table_low32[%d]=%d,gmp_table_high4[%d]=%d\n",
				i, lcp_param->gmp_table_low32[i], i, lcp_param->gmp_table_high4[i]);
	}
}

static void set_pinfo_gmp_lut_table(struct dpu_panel_info *pinfo, char __iomem *gmp_lut_base)
{
	int i;
	const int STEP = 500;

	for (i = 0; i < GMP_CNT_COFE; i++) {
		outp32(gmp_lut_base + i * 2 * 4, pinfo->gmp_lut_table_low32bit[i]);
		outp32(gmp_lut_base + i * 2 * 4 + 4, pinfo->gmp_lut_table_high4bit[i]);
		if ((i % STEP) == 0)
			DPU_FB_INFO("[effect] pinfo gmp_table_low32[%d]=%d,gmp_table_high4[%d]=%d\n",
				i, pinfo->gmp_lut_table_low32bit[i],
				i, pinfo->gmp_lut_table_high4bit[i]);
	}
}

static void gmp_set_lut(struct dpu_fb_data_type *dpufd)
{
	struct dpu_panel_info *pinfo = NULL;
	struct lcp_info *lcp_param = NULL;
	char __iomem *gmp_base = NULL;
	char __iomem *gmp_lut_base = NULL;
	uint32_t gmp_lut_sel;

	gmp_base = dpufd->dss_base + DSS_DPP_GMP_OFFSET;
	gmp_lut_base = dpufd->dss_base + DSS_DPP_GMP_LUT_OFFSET;

	pinfo = &(dpufd->panel_info);

	lcp_param = &(dpufd->effect_info[pinfo->disp_panel_id].lcp);
	dpu_check_and_no_retval(!lcp_param, ERR, "fb%d, lcp_param is NULL!\n", dpufd->index);

	if (dpufd->effect_gmp_update_flag && dpufd->effect_updated_flag[pinfo->disp_panel_id].gmp_effect_updated) {
		spin_lock(&g_gmp_effect_lock);
		if (lcp_param->gmp_table_low32 && lcp_param->gmp_table_high4)
			set_lcp_param_gmp_table(lcp_param, gmp_lut_base);

		spin_unlock(&g_gmp_effect_lock);

		gmp_lut_sel = (uint32_t)inp32(gmp_base + GMP_LUT_SEL);
		set_reg(gmp_base + GMP_LUT_SEL, (~(gmp_lut_sel & 0x1)) & 0x1, 1, 0);

		/* enable gmp */
		set_reg(gmp_base + GMP_EN, lcp_param->gmp_enable, 1, 0);
	} else {
		if ((pinfo->gmp_lut_table_len == GMP_CNT_COFE) &&
			(pinfo->gmp_lut_table_low32bit) && (pinfo->gmp_lut_table_high4bit)) {
			set_pinfo_gmp_lut_table(pinfo, gmp_lut_base);

			gmp_lut_sel = (uint32_t)inp32(gmp_base + GMP_LUT_SEL);
			set_reg(gmp_base + GMP_LUT_SEL, (~(gmp_lut_sel & 0x1)) & 0x1, 1, 0);
			/* not enable gmp */
			set_reg(gmp_base + GMP_EN, 0x0, 1, 0);
		}
	}

	DPU_FB_INFO("[effect] gmp enable=%d, lut_sel=%d, update_flag=%d, updated=%d, copy_status=%d\n",
		lcp_param->gmp_enable, gmp_lut_sel, dpufd->effect_gmp_update_flag,
		dpufd->effect_updated_flag[pinfo->disp_panel_id].gmp_effect_updated,
		dpufd->effect_gmp_copy_status);
}

static void gamma_set_lut(struct dpu_fb_data_type *dpufd)
{
	struct dpu_panel_info *pinfo = NULL;
	char __iomem *gamma_lut_base = NULL;
	uint32_t i;
	uint32_t index;

	if (dpufd == NULL)	{
		DPU_FB_ERR("init_degmma_xcc_gmp dpufd is NULL!\n");
		return;
	}

	pinfo = &(dpufd->panel_info);
	gamma_lut_base = dpufd->dss_base + DSS_DPP_GAMA_LUT_OFFSET;

	if (dpufb_use_dynamic_gamma(dpufd, gamma_lut_base) == 0) {
		if (pinfo->gamma_lut_table_len > 0 && pinfo->gamma_lut_table_R &&
			pinfo->gamma_lut_table_G && pinfo->gamma_lut_table_B) {
			for (i = 0; i < pinfo->gamma_lut_table_len / 2; i++) {
				index = i << 1;
				/* GAMA LUT */
				outp32(gamma_lut_base + (U_GAMA_R_COEF + i * 4),
					pinfo->gamma_lut_table_R[index] | (pinfo->gamma_lut_table_R[index + 1] << 16));
				outp32(gamma_lut_base + (U_GAMA_G_COEF + i * 4),
					pinfo->gamma_lut_table_G[index] | (pinfo->gamma_lut_table_G[index + 1] << 16));
				outp32(gamma_lut_base + (U_GAMA_B_COEF + i * 4),
					pinfo->gamma_lut_table_B[index] | (pinfo->gamma_lut_table_B[index + 1] << 16));
			}
			outp32(gamma_lut_base + U_GAMA_R_LAST_COEF,
				pinfo->gamma_lut_table_R[pinfo->gamma_lut_table_len - 1]);
			outp32(gamma_lut_base + U_GAMA_G_LAST_COEF,
				pinfo->gamma_lut_table_G[pinfo->gamma_lut_table_len - 1]);
			outp32(gamma_lut_base + U_GAMA_B_LAST_COEF,
				pinfo->gamma_lut_table_B[pinfo->gamma_lut_table_len - 1]);
		}
	}
}

static void degamma_set_lut(struct dpu_fb_data_type *dpufd)
{
	struct dpu_panel_info *pinfo = NULL;
	char __iomem *degamma_lut_base = NULL;
	uint32_t i;
	uint32_t index;

	if (dpufd == NULL)	{
		DPU_FB_ERR("init_degmma_xcc_gmp dpufd is NULL!\n");
		return;
	}

	pinfo = &(dpufd->panel_info);
	degamma_lut_base = dpufd->dss_base + DSS_DPP_DEGAMMA_LUT_OFFSET;

	if (!dpufb_use_dynamic_degamma(dpufd, degamma_lut_base)) {
		if (pinfo->igm_lut_table_len > 0 && pinfo->igm_lut_table_R &&
			pinfo->igm_lut_table_G && pinfo->igm_lut_table_B) {
			for (i = 0; i < pinfo->igm_lut_table_len / 2; i++) {
				index = i << 1;
				outp32(degamma_lut_base + (U_DEGAMA_R_COEF +  i * 4),
					pinfo->igm_lut_table_R[index] | (pinfo->igm_lut_table_R[index + 1] << 16));
				outp32(degamma_lut_base + (U_DEGAMA_G_COEF +  i * 4),
					pinfo->igm_lut_table_G[index] | (pinfo->igm_lut_table_G[index + 1] << 16));
				outp32(degamma_lut_base + (U_DEGAMA_B_COEF +  i * 4),
					pinfo->igm_lut_table_B[index] | (pinfo->igm_lut_table_B[index + 1] << 16));
			}
			outp32(degamma_lut_base + U_DEGAMA_R_LAST_COEF,
				pinfo->igm_lut_table_R[pinfo->igm_lut_table_len - 1]);
			outp32(degamma_lut_base + U_DEGAMA_G_LAST_COEF,
				pinfo->igm_lut_table_G[pinfo->igm_lut_table_len - 1]);
			outp32(degamma_lut_base + U_DEGAMA_B_LAST_COEF,
				pinfo->igm_lut_table_B[pinfo->igm_lut_table_len - 1]);
		}
	}
}

static void dpe_init_degamma(struct dpu_fb_data_type *dpufd)
{
	char __iomem *degamma_base = NULL;
	struct dpu_panel_info *pinfo = NULL;
	uint32_t degama_lut_sel;

	degamma_base = dpufd->dss_base + DSS_DPP_DEGAMMA_OFFSET;
	pinfo = &(dpufd->panel_info);

	if (pinfo->gamma_support == 1) {
		/* disable degamma */
		set_reg(degamma_base + DEGAMA_EN, 0x0, 1, 0);

		degamma_set_lut(dpufd);
		degama_lut_sel = (uint32_t)inp32(degamma_base + DEGAMA_LUT_SEL);
		set_reg(degamma_base + DEGAMA_LUT_SEL, (~(degama_lut_sel & 0x1)) & 0x1, 1, 0);

		/* enable degammab */
		set_reg(degamma_base + DEGAMA_EN, 0x1, 1, 0);
	} else {
		/* degama memory shutdown */
		outp32(degamma_base + DEGAMA_MEM_CTRL, 0x4);
	}
}

static void dpe_init_gmp(struct dpu_fb_data_type *dpufd)
{
	char __iomem *gmp_base = NULL;
	struct dpu_panel_info *pinfo = NULL;

	pinfo = &(dpufd->panel_info);
	gmp_base = dpufd->dss_base + DSS_DPP_GMP_OFFSET;
	if (pinfo->gmp_support == 1) {
		gmp_set_lut(dpufd);
		g_gmp_state = 1;
	} else {
		/* gmp memory shutdown */
		outp32(gmp_base + GMP_MEM_CTRL, 0x2);    /* only support deep sleep */
	}
}

static void dpe_init_gamma(struct dpu_fb_data_type *dpufd)
{
	char __iomem *gamma_base = NULL;
	struct dpu_panel_info *pinfo = NULL;
	uint32_t gama_lut_sel;

	pinfo = &(dpufd->panel_info);
	gamma_base = dpufd->dss_base + DSS_DPP_GAMA_OFFSET;
	if (pinfo->gamma_support == 1) {
		/* disable gamma */
		set_reg(gamma_base + GAMA_EN, 0x0, 1, 0);
		/* set gama lut */
		gamma_set_lut(dpufd);

		gama_lut_sel = (uint32_t)inp32(gamma_base + GAMA_LUT_SEL);
		set_reg(gamma_base + GAMA_LUT_SEL, (~(gama_lut_sel & 0x1)) & 0x1, 1, 0);

		/* enable gamma */
		set_reg(gamma_base + GAMA_EN, 0x1, 1, 0);
	} else {
		/* gama memory shutdown */
		outp32(gamma_base + GAMA_MEM_CTRL, 0x4);
	}
}

void init_igm_gmp_xcc_gm(struct dpu_fb_data_type *dpufd)
{
	if (!dpufd) {
		DPU_FB_ERR("init_degmma_xcc_gmp dpufd is NULL!\n");
		return;
	}

	dpufd->gmp_online_set_reg_count = 0;

	if (dpufd->index != PRIMARY_PANEL_IDX) {
		DPU_FB_ERR("fb%d, not support!\n", dpufd->index);
		return;
	}
	/* avoid the partial update */
	dpufd->display_effect_flag = 40;
	/* Degamma */
	dpe_init_degamma(dpufd);
	/* XCC */
	dpe_init_xcc(dpufd);
	/* GMP */
	dpe_init_gmp(dpufd);
	/* GAMMA & GAMMA pre */
	dpe_init_gamma(dpufd);
}
void dpe_store_ct_csc_value(struct dpu_fb_data_type *dpufd, unsigned int csc_value[], unsigned int len)
{
	struct dpu_panel_info *pinfo = NULL;

	if (!dpufd) {
		DPU_FB_ERR("dpufd is NULL.\n");
		return;
	}

	if (len < CSC_VALUE_MIN_LEN) {
		DPU_FB_ERR("csc_value len is too short.\n");
		return;
	}

	pinfo = &(dpufd->panel_info);

	if (pinfo->xcc_support == 0 || !(pinfo->xcc_table))
		return;

	pinfo->xcc_table[1] = csc_value[0];
	pinfo->xcc_table[2] = csc_value[1];
	pinfo->xcc_table[3] = csc_value[2];
	pinfo->xcc_table[5] = csc_value[3];
	pinfo->xcc_table[6] = csc_value[4];
	pinfo->xcc_table[7] = csc_value[5];
	pinfo->xcc_table[9] = csc_value[6];
	pinfo->xcc_table[10] = csc_value[7];
	pinfo->xcc_table[11] = csc_value[8];
}

void dpe_update_g_comform_discount(unsigned int value)
{
	g_comform_value = value;
	DPU_FB_INFO("g_comform_value = %d.\n", g_comform_value);
}

int dpe_set_ct_csc_value(struct dpu_fb_data_type *dpufd)
{
	struct dpu_panel_info *pinfo = NULL;
	char __iomem *lcp_base = NULL;
	/* color temp rectify rgb default valve */
	struct rgb rectify_rgb;

	dpu_check_and_return(!dpufd, -EINVAL, ERR, "dpufd is nullptr!\n");

	pinfo = &(dpufd->panel_info);

	if (dpufd->index == PRIMARY_PANEL_IDX) {
		lcp_base = dpufd->dss_base + DSS_DPP_LCP_OFFSET;
	} else {
		DPU_FB_ERR("fb%d, not support!\n", dpufd->index);
		return -1;
	}

	rectify_rgb.color_temp_rectify_r = get_color_rectify_rgb(pinfo->color_temp_rectify_R,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_g = get_color_rectify_rgb(pinfo->color_temp_rectify_G,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_b = get_color_rectify_rgb(pinfo->color_temp_rectify_B,
		pinfo->color_temp_rectify_support);

	/* XCC */
	if (pinfo->xcc_support == 1) {
		/* XCC matrix */
		if (pinfo->xcc_table_len > 0 && pinfo->xcc_table) {
			xcc_set_coef(lcp_base, pinfo, rectify_rgb);
			dpufd->color_temperature_flag = 2; /* display effect flag */
		}
	}

	return 0;
}

ssize_t dpe_show_ct_csc_value(struct dpu_fb_data_type *dpufd, char *buf)
{
	struct dpu_panel_info *pinfo = NULL;

	if (!dpufd) {
		DPU_FB_ERR("dpufd is NULL.\n");
		return -EINVAL;
	}

	pinfo = &(dpufd->panel_info);

	if (pinfo->xcc_support == 0 || !(pinfo->xcc_table))
		return 0;

	return snprintf(buf, PAGE_SIZE, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
		pinfo->xcc_table[1], pinfo->xcc_table[2], pinfo->xcc_table[3],
		pinfo->xcc_table[5], pinfo->xcc_table[6], pinfo->xcc_table[7],
		pinfo->xcc_table[9], pinfo->xcc_table[10], pinfo->xcc_table[11]);
}

int dpe_set_xcc_csc_value(struct dpu_fb_data_type *dpufd)
{
	void_unused(dpufd);
	return 0;
}

int dpe_set_comform_ct_csc_value(struct dpu_fb_data_type *dpufd)
{
	struct dpu_panel_info *pinfo = NULL;
	char __iomem *lcp_base = NULL;
	struct rgb rectify_rgb;

	if (!dpufd) {
		DPU_FB_ERR("dpufd is NULL.\n");
		return -EINVAL;
	}
	pinfo = &(dpufd->panel_info);

	if (dpufd->index != PRIMARY_PANEL_IDX) {
		DPU_FB_ERR("fb%d, not support!\n", dpufd->index);
		return -1;
	}

	lcp_base = dpufd->dss_base + DSS_DPP_LCP_OFFSET;

	rectify_rgb.color_temp_rectify_r = get_color_rectify_rgb(pinfo->color_temp_rectify_R,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_g = get_color_rectify_rgb(pinfo->color_temp_rectify_G,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_b = get_color_rectify_rgb(pinfo->color_temp_rectify_B,
		pinfo->color_temp_rectify_support);

	/* XCC */
	if (pinfo->xcc_support == 1) {
		/* XCC matrix */
		if (pinfo->xcc_table_len > 0 && pinfo->xcc_table)
			xcc_set_coef(lcp_base, pinfo, rectify_rgb);
	}

	return 0;
}

ssize_t dpe_show_comform_ct_csc_value(struct dpu_fb_data_type *dpufd, char *buf)
{
	struct dpu_panel_info *pinfo = NULL;

	dpu_check_and_return(!dpufd || !buf, -EINVAL, ERR, "dpufd or buf NUll pointer warning.\n");

	pinfo = &(dpufd->panel_info);
	if (pinfo->xcc_support == 0 || !(pinfo->xcc_table))
		return 0;

	return snprintf(buf, PAGE_SIZE, "%d,%d,%d,%d,%d,%d,%d,%d,%d,g_comform_value = %d.\n",
		pinfo->xcc_table[1], pinfo->xcc_table[2], pinfo->xcc_table[3],
		pinfo->xcc_table[5], pinfo->xcc_table[6], pinfo->xcc_table[7],
		pinfo->xcc_table[9], pinfo->xcc_table[10], pinfo->xcc_table[11],
		g_comform_value);
}

void dpe_init_led_rg_ct_csc_value(void)
{
	/* led rg csc default valve */
	g_led_rg_csc_value[0] = COLOR_RECTIFY_DEFAULT;
	g_led_rg_csc_value[1] = 0;
	g_led_rg_csc_value[2] = 0;
	g_led_rg_csc_value[3] = 0;
	g_led_rg_csc_value[4] = COLOR_RECTIFY_DEFAULT;
	g_led_rg_csc_value[5] = 0;
	g_led_rg_csc_value[6] = 0;
	g_led_rg_csc_value[7] = 0;
	g_led_rg_csc_value[8] = COLOR_RECTIFY_DEFAULT;
	g_is_led_rg_csc_set = 0;
}

void dpe_store_led_rg_ct_csc_value(unsigned int csc_value[], unsigned int len)
{
	if (len < CSC_VALUE_MIN_LEN) {
		DPU_FB_ERR("csc_value len is too short.\n");
		return;
	}

	g_led_rg_csc_value[0] = csc_value[0];
	g_led_rg_csc_value[1] = csc_value[1];
	g_led_rg_csc_value[2] = csc_value[2];
	g_led_rg_csc_value[3] = csc_value[3];
	g_led_rg_csc_value[4] = csc_value[4];
	g_led_rg_csc_value[5] = csc_value[5];
	g_led_rg_csc_value[6] = csc_value[6];
	g_led_rg_csc_value[7] = csc_value[7];
	g_led_rg_csc_value[8] = csc_value[8];
	g_is_led_rg_csc_set = 1;
}

int dpe_set_led_rg_ct_csc_value(struct dpu_fb_data_type *dpufd)
{
	struct dpu_panel_info *pinfo = NULL;
	char __iomem *lcp_base = NULL;
	struct rgb rectify_rgb;

	dpu_check_and_return(!dpufd, -EINVAL, ERR, "dpufd is nullptr!\n");

	pinfo = &(dpufd->panel_info);

	if (dpufd->index == PRIMARY_PANEL_IDX) {
		lcp_base = dpufd->dss_base + DSS_DPP_LCP_OFFSET;
	} else {
		DPU_FB_ERR("fb%d, not support!\n", dpufd->index);
		return -1;
	}

	rectify_rgb.color_temp_rectify_r = get_color_rectify_rgb(pinfo->color_temp_rectify_R,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_g = get_color_rectify_rgb(pinfo->color_temp_rectify_G,
		pinfo->color_temp_rectify_support);
	rectify_rgb.color_temp_rectify_b = get_color_rectify_rgb(pinfo->color_temp_rectify_B,
		pinfo->color_temp_rectify_support);

	/* XCC */
	if (g_is_led_rg_csc_set == 1 && pinfo->xcc_support == 1) {
		DPU_FB_DEBUG("real set color temperature: g_is_led_rg_csc_set = %d, R = 0x%x, G = 0x%x, B = 0x%x.\n",
			g_is_led_rg_csc_set, g_led_rg_csc_value[0],
			g_led_rg_csc_value[4], g_led_rg_csc_value[8]);
		/* XCC matrix */
		if (pinfo->xcc_table_len > 0 && pinfo->xcc_table)
			xcc_set_coef(lcp_base, pinfo, rectify_rgb);
	}

	return 0;
}

ssize_t dpe_show_led_rg_ct_csc_value(char *buf)
{
	if (!buf) {
		DPU_FB_ERR("buf, NUll pointer warning.\n");
		return 0;
	}

	return snprintf(buf, PAGE_SIZE, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
		g_led_rg_para1, g_led_rg_para2,
		g_led_rg_csc_value[0], g_led_rg_csc_value[1], g_led_rg_csc_value[2],
		g_led_rg_csc_value[3], g_led_rg_csc_value[4], g_led_rg_csc_value[5],
		g_led_rg_csc_value[6], g_led_rg_csc_value[7], g_led_rg_csc_value[8]);
}

ssize_t dpe_show_cinema_value(struct dpu_fb_data_type *dpufd, char *buf)
{
	dpu_check_and_return((!dpufd || !buf), 0, ERR, "dpufd or buf is NULL\n");

	return snprintf(buf, PAGE_SIZE, "gamma type is = %d.\n", dpufd->panel_info.gamma_type);
}

int dpe_set_cinema(struct dpu_fb_data_type *dpufd, unsigned int value)
{
	if (!dpufd) {
		DPU_FB_ERR("dpufd, NUll pointer warning.\n");
		return -1;
	}

	if (dpufd->panel_info.gamma_type == value) {
		DPU_FB_DEBUG("fb%d, cinema mode is already in %d!\n", dpufd->index, value);
		return 0;
	}

	dpufd->panel_info.gamma_type = value;

	return 0;
}

void dpe_update_g_acm_state(unsigned int value)
{
	void_unused(value);
}

void dpe_set_acm_state(struct dpu_fb_data_type *dpufd)
{
	void_unused(dpufd);
}

ssize_t dpe_show_acm_state(char *buf)
{
	ssize_t ret;

	if (!buf) {
		DPU_FB_ERR("NULL Pointer!\n");
		return 0;
	}

	ret = snprintf(buf, PAGE_SIZE, "g_acm_state = %d\n", g_acm_state);

	return ret;
}

void dpe_update_g_gmp_state(unsigned int value)
{
	void_unused(value);
}

void dpe_set_gmp_state(struct dpu_fb_data_type *dpufd)
{
	void_unused(dpufd);
}

ssize_t dpe_show_gmp_state(char *buf)
{
	ssize_t ret;

	if (!buf) {
		DPU_FB_ERR("NULL Pointer!\n");
		return 0;
	}

	ret = snprintf(buf, PAGE_SIZE, "g_gmp_state = %d.\n", g_gmp_state);

	return ret;
}

