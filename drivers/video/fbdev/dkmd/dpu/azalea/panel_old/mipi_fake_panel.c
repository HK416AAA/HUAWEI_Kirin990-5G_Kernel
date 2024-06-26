/* Copyright (c) 2008-2014, Hisilicon Tech. Co., Ltd. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 and
* only version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
* GNU General Public License for more details.
*
*/

#include "dpu_fb.h"
#include <linux/adc.h>

#define DTS_COMP_MIPI_FAKE_PANEL "hisilicon,mipi_fake_panel"

#define GPIO_VSP	"gpio_lcd_vsp"
#define GPIO_VSN	"gpio_lcd_vsn"
#define GPIO_LCD_BL_ENABLE_NAME	"gpio_lcd_bl_enable"

static int hkadc_buf = 0;
static uint32_t bl_enable_flag = 0;
static uint32_t gpio_lcd_bl_enable = 0;
struct ldo_ops {
	void(*dpu_vsp)(bool en);
	void(*dpu_vsn)(bool en);
	void(*dpu_vddio)(bool en);
	void(*dpu_vci)(bool en);
};

struct voltage_node {
	unsigned int vsn_gpio;
	unsigned int vsp_gpio;
};

static struct voltage_node volt = {0};
static struct ldo_ops ldo_en;

static struct gpio_desc fake_lcd_gpio_vsp_vsn_request_cmds[] = {
	/* AVDD_5.5V */
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_VSP, &volt.vsp_gpio, 0},
	/* AVEE_-5.5V */
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_VSN, &volt.vsn_gpio, 0},
};

static struct gpio_desc fake_lcd_gpio_bl_request_cmds[] = {
	/* backlight enable */
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_LCD_BL_ENABLE_NAME, &gpio_lcd_bl_enable, 0},
};

/*******************************************************************************
**
*/
static int mipi_fake_panel_set_fastboot(struct platform_device *pdev)
{
	struct dpu_fb_data_type *dpufd = NULL;

	if (pdev == NULL){
		DPU_FB_ERR("pdev is null\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	DPU_FB_DEBUG("fb%d, +.\n", dpufd->index);

	DPU_FB_DEBUG("fb%d, -.\n", dpufd->index);

	return 0;
}

static int mipi_fake_panel_on(struct platform_device *pdev)
{
	struct dpu_fb_data_type *dpufd = NULL;
	struct dpu_panel_info *pinfo = NULL;

	if (pdev == NULL){
		DPU_FB_ERR("pdev is null\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	DPU_FB_INFO("fb%d, +!\n", dpufd->index);

	pinfo = &(dpufd->panel_info);

	if (pinfo->lcd_init_step == LCD_INIT_POWER_ON) {
		pinfo->lcd_init_step = LCD_INIT_MIPI_LP_SEND_SEQUENCE;
	} else if (pinfo->lcd_init_step == LCD_INIT_MIPI_LP_SEND_SEQUENCE) {
		pinfo->lcd_init_step = LCD_INIT_MIPI_HS_SEND_SEQUENCE;
	} else if (pinfo->lcd_init_step == LCD_INIT_MIPI_HS_SEND_SEQUENCE) {
		;
	} else {
		DPU_FB_ERR("failed to init lcd!\n");
	}

	/* backlight on */
	//dpu_lcd_backlight_on(pdev);

	DPU_FB_INFO("fb%d, -!\n", dpufd->index);

	return 0;
}

static int mipi_fake_panel_off(struct platform_device *pdev)
{
	struct dpu_fb_data_type *dpufd = NULL;

	if (pdev == NULL){
		DPU_FB_ERR("pdev is null\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	DPU_FB_INFO("fb%d, +!\n", dpufd->index);
	/* backlight off */
	//dpu_lcd_backlight_off(pdev);

	DPU_FB_INFO("fb%d, -!\n", dpufd->index);

	return 0;
}

static int mipi_fake_panel_remove(struct platform_device *pdev)
{
	struct dpu_fb_data_type *dpufd = NULL;

	if (pdev == NULL){
		DPU_FB_ERR("pdev is null\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	/*if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}*/

	if (!dpufd)
		return 0;

	DPU_FB_DEBUG("fb%d, +.\n", dpufd->index);

	DPU_FB_DEBUG("fb%d, -.\n", dpufd->index);

	return 0;
}

static int mipi_fake_panel_set_backlight(struct platform_device *pdev, uint32_t bl_level)
{
	struct dpu_fb_data_type *dpufd = NULL;

	if (pdev == NULL){
		DPU_FB_ERR("pdev is null\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	DPU_FB_DEBUG("fb%d, bl_level=%d.\n", dpufd->index, bl_level);

	DPU_FB_DEBUG("fb%d, -.\n", dpufd->index);

	return 0;
}

static int mipi_fake_panel_bl_update(struct platform_device *pdev, uint32_t bl_level)
{
	int ret = 0;
	struct dpu_fb_data_type *dpufd = NULL;

	if (pdev == NULL){
		DPU_FB_ERR("pdev is null\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	if (bl_level > 0) {
		/*enable bl gpio*/
		if (bl_enable_flag)
			gpio_direction_output(gpio_lcd_bl_enable, 1);
		mdelay(2);
		/* backlight on */
		dpu_lcd_backlight_on(pdev);

		DPU_FB_INFO("set backlight to %d\n", bl_level);
		ret = dpu_blpwm_set_backlight(dpufd, bl_level);
	} else {
		ret = dpu_blpwm_set_backlight(dpufd, 0);
		/* backlight off */
		dpu_lcd_backlight_off(pdev);
		/*disable bl gpio*/
		if (bl_enable_flag)
			gpio_direction_output(gpio_lcd_bl_enable, 0);
	}

	return 0;
}

static ssize_t mipi_fake_panel_lcd_hkadc_debug_show(struct platform_device *pdev, char *buf)
{
	ssize_t ret = 0;
	struct dpu_fb_data_type *dpufd = NULL;

	if (!pdev || !buf){
		DPU_FB_ERR("pdev or buf is NULL\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	DPU_FB_DEBUG("fb%d, +.\n", dpufd->index);
	ret = snprintf(buf, PAGE_SIZE, "%d\n", hkadc_buf*4);
	DPU_FB_DEBUG("fb%d, -.\n", dpufd->index);

	return ret;
}

static ssize_t mipi_fake_panel_lcd_hkadc_debug_store(struct platform_device *pdev,
	const char *buf, size_t count)
{
	int ret = 0;
	int channel = 0;
	struct dpu_fb_data_type *dpufd = NULL;

	if (!pdev || !buf){
		DPU_FB_ERR("pdev or buf is NULL\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	DPU_FB_DEBUG("fb%d, +.\n", dpufd->index);

	ret = sscanf(buf, "%u", &channel);
	if(ret <= 0) {
		DPU_FB_ERR("Sscanf return invalid, ret = %d\n", ret);
		return count;
	}

	hkadc_buf = lpm_adc_get_value(channel);
	DPU_FB_INFO("channel[%d] value is %d\n", channel, hkadc_buf);

	DPU_FB_DEBUG("fb%d, -.\n", dpufd->index);

	return count;
}

static void dpu_vsp_enable(bool en)
{
	DPU_FB_INFO("vsp enable %d\n", en);

	if(en)
		gpio_direction_output(volt.vsp_gpio, 1);
	else
		gpio_direction_output(volt.vsp_gpio, 0);
}

static void dpu_vsn_enable(bool en)
{
	DPU_FB_INFO("vsn enable %d\n", en);

	if(en)
		gpio_direction_output(volt.vsn_gpio, 1);
	else
		gpio_direction_output(volt.vsn_gpio, 0);
}

static void dpu_vddio_enable(bool en)
{
	DPU_FB_INFO("obsolescent\n");

}

static void dpu_vci_enable(bool en)
{
	DPU_FB_INFO("obsolescent\n");

}

static ssize_t mipi_fake_panel_lcd_voltage_enable_store(struct platform_device *pdev,
	const char *buf, size_t count)
{
	char command[10] = {0};
	struct dpu_fb_data_type *dpufd = NULL;
	uint32_t bl_value = 0;
	char bl_value_buf[10] = {0};

	if (!pdev || !buf){
		DPU_FB_ERR("pdev or buf is NULL\n");
		return -1;
	}
	dpufd = platform_get_drvdata(pdev);
	if (dpufd == NULL){
		DPU_FB_ERR("dpufd is null\n");
		return -1;
	}

	DPU_FB_DEBUG("fb%d, +.\n", dpufd->index);

	if (strlen(buf) >= 10){
		DPU_FB_ERR("buf_overflow!\n");
		return count;
	}

	if (!sscanf(buf, "%s", command)) {
		DPU_FB_INFO("bad command(%s)\n", command);
		return count;
	}

	DPU_FB_INFO("command(%s)\n", command);
	if (!strncmp("vsp:", command, strlen("vsp:"))) {
		if('0' == command[strlen("vsp:")])
			ldo_en.dpu_vsp(false);
		else
			ldo_en.dpu_vsp(true);
	}

	if (!strncmp("vsn:", command, strlen("vsn:"))) {
		if('0' == command[strlen("vsn:")])
			ldo_en.dpu_vsn(false);
		else
			ldo_en.dpu_vsn(true);
	}

	if (!strncmp("vddio:", command, strlen("vddio:"))) {
		if('0' == command[strlen("vddio:")])
			ldo_en.dpu_vddio(false);
		else
			ldo_en.dpu_vddio(true);
	}

	if (!strncmp("vci:", command, strlen("vci:"))) {
		if('0' == command[strlen("vci:")])
			ldo_en.dpu_vci(false);
		else
			ldo_en.dpu_vci(true);
	}

	if (!strncmp("bl:", command, strlen("bl:"))) {
		if (((strlen(command) - strlen("bl:")) > 0) &&((strlen(command) - strlen("bl:")) < sizeof(bl_value_buf))) {
			memcpy(bl_value_buf, &command[strlen("bl:")], strlen(command)-strlen("bl:"));
			bl_value = simple_strtoul(bl_value_buf, NULL, 0);
			DPU_FB_INFO("bl_value_buf is %s, bl_value is %d\n", bl_value_buf, bl_value);
		} else {
			bl_value = 0;
		}

		mipi_fake_panel_bl_update(pdev, bl_value);
	}

	DPU_FB_DEBUG("fb%d, -.\n", dpufd->index);

	return count;
}

static struct dpu_panel_info mipi_fake_panel_info = {0};
static struct dpu_fb_panel_data g_panel_data = {
	.panel_info = &mipi_fake_panel_info,
	.set_fastboot = mipi_fake_panel_set_fastboot,
	.on = mipi_fake_panel_on,
	.off = mipi_fake_panel_off,
	.remove = mipi_fake_panel_remove,
	.set_backlight = mipi_fake_panel_set_backlight,
	.lcd_hkadc_debug_show = mipi_fake_panel_lcd_hkadc_debug_show,
	.lcd_hkadc_debug_store = mipi_fake_panel_lcd_hkadc_debug_store,
	.lcd_voltage_enable_store = mipi_fake_panel_lcd_voltage_enable_store,
};


/*******************************************************************************
**
*/
static int mipi_fake_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct dpu_panel_info *pinfo = NULL;
	struct device_node *np = NULL;
	const char *lcd_bl_ic_name = NULL;
	char lcd_bl_ic_name_buf[LCD_BL_IC_NAME_MAX];

	if (dpu_fb_device_probe_defer(PANEL_MIPI_VIDEO, BL_SET_BY_NONE)) {
		goto err_probe_defer;
	}

	DPU_FB_INFO("%s +.\n", DTS_COMP_MIPI_FAKE_PANEL);

	np = of_find_compatible_node(NULL, NULL, DTS_COMP_MIPI_FAKE_PANEL);
	if (!np) {
		DPU_FB_ERR("NOT FOUND device node %s!\n", DTS_COMP_MIPI_FAKE_PANEL);
		goto err_return;
	}

	ret = of_property_read_string_index(np, "lcd-bl-ic-name", 0, &lcd_bl_ic_name);
	if (ret != 0) {
		memcpy(lcd_bl_ic_name_buf, "INVALID", strlen("INVALID"));
	} else {
		if (strlen(lcd_bl_ic_name) >= LCD_BL_IC_NAME_MAX) {
			DPU_FB_ERR("The length of the lcd_bl_ic_name is greater than LCD_BL_IC_NAME_MAX\n");
			goto err_return;
		}
		memcpy(lcd_bl_ic_name_buf, lcd_bl_ic_name, strlen(lcd_bl_ic_name) + 1);
	}

	ret = of_property_read_u32(np, "bl_enable_flag", &bl_enable_flag);
	if (ret)
		bl_enable_flag = 1;

	DPU_FB_INFO("bl_enable_flag=%d\n", bl_enable_flag);

	if (!pdev) {
		DPU_FB_ERR("pdev is NULL\n");
		return 0;
	}

	pdev->id = 1;
	// init lcd panel info
	pinfo = g_panel_data.panel_info;
	memset(pinfo, 0, sizeof(struct dpu_panel_info));
	pinfo->xres = 1080;
	pinfo->yres = 1920;
	pinfo->width = 62;
	pinfo->height = 110;
	pinfo->type = PANEL_MIPI_VIDEO;
	pinfo->orientation = LCD_PORTRAIT;
	pinfo->bpp = LCD_RGB888;
	pinfo->bgr_fmt = LCD_RGB;
	pinfo->bl_set_type = BL_SET_BY_BLPWM;

	if (pinfo->bl_set_type == BL_SET_BY_BLPWM)
		pinfo->blpwm_input_ena = 0;

	if (!strncmp(lcd_bl_ic_name_buf, "LM36923YFFR", strlen("LM36923YFFR"))) {
		pinfo->bl_min = 63;
		pinfo->bl_max = 7992;
		pinfo->bl_default = 4000;
		pinfo->blpwm_precision_type = BLPWM_PRECISION_2048_TYPE;
		pinfo->bl_ic_ctrl_mode = REG_ONLY_MODE;
	} else {
#ifdef CONFIG_BACKLIGHT_10000
		pinfo->bl_min = 157;
		pinfo->bl_max = 9960;
		pinfo->bl_default = 4000;
		pinfo->blpwm_precision_type = BLPWM_PRECISION_10000_TYPE;
#else
		pinfo->bl_min = 1;
		pinfo->bl_max = 255;
		pinfo->bl_default = 102;
#endif
	}

	pinfo->frc_enable = 0;
	pinfo->esd_enable = 0;

	pinfo->ldi.h_back_porch = 40;
	pinfo->ldi.h_front_porch = 90;
	pinfo->ldi.h_pulse_width = 20;
	pinfo->ldi.v_back_porch = 28;
	pinfo->ldi.v_front_porch = 14;
	pinfo->ldi.v_pulse_width = 8;

	pinfo->mipi.lane_nums = DSI_4_LANES;
	pinfo->mipi.color_mode = DSI_24BITS_1;
	pinfo->mipi.vc = 0;
	pinfo->mipi.max_tx_esc_clk = 10 * 1000000;
	pinfo->mipi.burst_mode = 0;

	pinfo->mipi.dsi_bit_clk = 480;
	pinfo->mipi.dsi_bit_clk_upt = pinfo->mipi.dsi_bit_clk;

	pinfo->pxl_clk_rate = 146 * 1000000;

	volt.vsp_gpio = of_get_named_gpio(np, "gpios", 2);  // gpio_1_7
	volt.vsn_gpio = of_get_named_gpio(np, "gpios", 0);  //gpio_1_1

	ldo_en.dpu_vsp = dpu_vsp_enable;
	ldo_en.dpu_vsn = dpu_vsn_enable;
	ldo_en.dpu_vci = dpu_vci_enable;
	ldo_en.dpu_vddio = dpu_vddio_enable;

	if (bl_enable_flag) {
		gpio_lcd_bl_enable = of_get_named_gpio(np, "gpios", 1);
		gpio_cmds_tx(fake_lcd_gpio_bl_request_cmds, \
			ARRAY_SIZE(fake_lcd_gpio_bl_request_cmds));
	}
	DPU_FB_INFO("gpio_lcd_bl_enable is %d, bl_set_type is %d\n", gpio_lcd_bl_enable, pinfo->bl_set_type);
	/* lcd gpio request */
	gpio_cmds_tx(fake_lcd_gpio_vsp_vsn_request_cmds, \
		ARRAY_SIZE(fake_lcd_gpio_vsp_vsn_request_cmds));

	// alloc panel device data
	ret = platform_device_add_data(pdev, &g_panel_data,
		sizeof(struct dpu_fb_panel_data));
	if (ret) {
		DPU_FB_ERR("platform_device_add_data failed!\n");
		goto err_device_put;
	}

	dpu_fb_add_device(pdev);

	DPU_FB_DEBUG("-.\n");

	return 0;

err_device_put:
	platform_device_put(pdev);
err_return:
	return ret;
err_probe_defer:
	return -EPROBE_DEFER;
}

static const struct of_device_id dpu_panel_match_table[] = {
	{
		.compatible = DTS_COMP_MIPI_FAKE_PANEL,
		.data = NULL,
	},
	{},
};

MODULE_DEVICE_TABLE(of, dpu_panel_match_table);

static struct platform_driver this_driver = {
	.probe = mipi_fake_probe,
	.remove = NULL,
	.suspend = NULL,
	.resume = NULL,
	.shutdown = NULL,
	.driver = {
		.name = "mipi_fake_panel",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(dpu_panel_match_table),
	},
};

static int __init mipi_fake_panel_init(void)
{
	int ret = 0;
#ifndef CONFIG_X1_CARBU_EDITION_V10_ENABLE_MIPI_FAKE
// x1 car edition, others
#ifdef CONFIG_CDC_MULTI_DISPLAY
	// x1 car edition
	DPU_FB_INFO("disable mipi fake\n");
	return 0;
#endif
#endif
	// x1 hisi edition, others
	ret = platform_driver_register(&this_driver);
	if (ret) {
		DPU_FB_ERR("platform_driver_register failed, error=%d!\n", ret);
		return ret;
	}

	return ret;
}

module_init(mipi_fake_panel_init);
