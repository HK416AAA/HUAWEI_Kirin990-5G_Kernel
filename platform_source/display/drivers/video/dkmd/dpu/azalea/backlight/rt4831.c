/*
 * rt4831.c
 *
 * bias+backlight driver of RT4831
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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

#include "rt4831.h"
#include <linux/module.h>
#include <linux/leds.h>
#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/regmap.h>
#include <linux/semaphore.h>
#include "lcd_kit_bias.h"
#include "dpu_fb.h"
#ifdef CONFIG_LCDKIT_DRIVER
#include "lcdkit_panel.h"
#endif
#include "dpu_fb_defconfig.h"

struct class *rt4831_class;
struct rt4831_chip_data *rt4831_g_chip;
static bool rt4831_init_status;
static unsigned int g_bl_level_enhance_mode;
static struct rt4831_backlight_information g_bl_info;
static int g_force_resume_bl_flag = RESUME_IDLE;
static int g_resume_bl_duration; /* default not support auto resume */
static enum hrtimer_restart rt4831_bl_resume_hrtimer_fnc(
	struct hrtimer *timer);
static void rt4831_bl_resume_workqueue_handler(struct work_struct *work);
static int rt4831_fault_check_support;
static struct backlight_work_mode_reg_info g_bl_indo;
unsigned int rt4831_msg_level = 7;
extern int bl_lvl_map(int level);

module_param_named(debug_rt4831_msg_level, rt4831_msg_level, int, 0644);
MODULE_PARM_DESC(debug_rt4831_msg_level, "backlight rt4831 msg level");

static int rt4831_parse_dts(struct device_node *np)
{
	int ret;
	int i;

	if (np == NULL) {
		RT4831_ERR("np is NULL pointer\n");
		return -1;
	}

	for (i = 0; i < RT4831_RW_REG_MAX; i++) {
		ret = of_property_read_u32(np, rt4831_dts_string[i],
			&rt4831_reg[i]);
		if (ret < 0) {
			RT4831_ERR("get rt4831 dts config failed\n");
		} else {
			RT4831_INFO("get %s from dts value = 0x%x\n",
				rt4831_dts_string[i], rt4831_reg[i]);
		}
	}
	if (of_property_read_u32(np, "rt4831_check_fault_support",
		&rt4831_fault_check_support) < 0)
		RT4831_INFO("No need to detect fault flags!\n");

	return ret;
}

static int rt4831_config_write(struct rt4831_chip_data *pchip,
	unsigned int reg[], unsigned int val[], unsigned int size)
{
	int ret = 0;
	unsigned int i;

	if ((pchip == NULL) || (reg == NULL) || (val == NULL)) {
		RT4831_ERR("pchip or reg or val is NULL pointer\n");
		return -1;
	}

	for (i = 0; i < size; i++) {
		ret = regmap_write(pchip->regmap, reg[i], val[i]);
		if (ret < 0) {
			RT4831_ERR("write rt4831 bl config reg 0x%x failed\n",
				reg[i]);
			return ret;
		}
		RT4831_INFO("register 0x%x value = 0x%x\n", reg[i], val[i]);
	}

	return ret;
}

static int rt4831_config_read(struct rt4831_chip_data *pchip,
	unsigned int reg[], unsigned int val[], unsigned int size)
{
	int ret;
	unsigned int i;

	if ((pchip == NULL) || (reg == NULL) || (val == NULL)) {
		RT4831_ERR("pchip or reg or val is NULL pointer\n");
		return -1;
	}

	for (i = 0; i < size; i++) {
		ret = regmap_read(pchip->regmap, reg[i], &val[i]);
		if (ret < 0) {
			RT4831_ERR("read rt4831 bl config reg 0x%x failed",
				reg[i]);
			return ret;
		}
		RT4831_INFO("read 0x%x value = 0x%x\n", reg[i], val[i]);
	}

	return ret;
}

static void rt4831_bl_mode_reg_init(unsigned int reg[], unsigned int val[],
	unsigned int size)
{
	unsigned int i;
	unsigned int index;

	if ((reg == NULL) || (val == NULL)) {
		RT4831_ERR("reg or val is NULL pointer\n");
		return;
	}

	RT4831_INFO("in\n");
	memset(&g_bl_indo, 0, sizeof(struct backlight_work_mode_reg_info));

	for (i = 0; i < size; i++) {
		switch (reg[i]) {
		case REG_BL_CONFIG_1:
		case REG_BL_CONFIG_2:
		case REG_BL_OPTION_2:
			index = g_bl_indo.bl_mode_reg.index;
			if (index >= BL_MAX_CONFIG_REG_NUM)
				break;
			g_bl_indo.bl_mode_reg.reg_addr[index] = reg[i];
			g_bl_indo.bl_mode_reg.normal_reg_var[index] = val[i];
			if (reg[i] == REG_BL_CONFIG_1)
				g_bl_indo.bl_mode_reg.enhance_reg_var[index] =
					BL_OVP_29V;
			else if (reg[i] == REG_BL_CONFIG_2)
				g_bl_indo.bl_mode_reg.enhance_reg_var[index] =
					CURRENT_RAMP_5MS;
			else
				g_bl_indo.bl_mode_reg.enhance_reg_var[index] =
					BL_OCP_2;

			RT4831_INFO("reg:0x%x, normal=0x%x, enhance=0x%x\n",
				g_bl_indo.bl_mode_reg.reg_addr[index],
				g_bl_indo.bl_mode_reg.normal_reg_var[index],
				g_bl_indo.bl_mode_reg.enhance_reg_var[index]);

			g_bl_indo.bl_mode_reg.index++;
			break;
		case REG_BL_ENABLE:
			index = g_bl_indo.bl_enable_reg.index;
			if (index >= BL_MAX_CONFIG_REG_NUM)
				break;
			g_bl_indo.bl_enable_reg.reg_addr[index] = reg[i];
			g_bl_indo.bl_enable_reg.normal_reg_var[index] = val[i];
			g_bl_indo.bl_enable_reg.enhance_reg_var[index] =
				EN_4_SINK;

			RT4831_INFO("reg:0x%x, normal=0x%x, enhance=0x%x\n",
				g_bl_indo.bl_enable_reg.reg_addr[index],
				g_bl_indo.bl_enable_reg.normal_reg_var[index],
				g_bl_indo.bl_enable_reg.enhance_reg_var[index]);

			g_bl_indo.bl_enable_reg.index++;
			break;
		default:
			break;
		}
	}

	index =	g_bl_indo.bl_current_reg.index;
	g_bl_indo.bl_current_reg.reg_addr[index] = REG_BL_BRIGHTNESS_LSB;
	g_bl_indo.bl_current_reg.normal_reg_var[index] = LSB_10MA;
	g_bl_indo.bl_current_reg.enhance_reg_var[index] =
		g_bl_level_enhance_mode & LSB;

	RT4831_INFO("reg_addr:0x%x, normal_val=0x%x, enhance_val=0x%x\n",
		g_bl_indo.bl_current_reg.reg_addr[index],
		g_bl_indo.bl_current_reg.normal_reg_var[index],
		g_bl_indo.bl_current_reg.enhance_reg_var[index]);

	g_bl_indo.bl_current_reg.index++;
	index = g_bl_indo.bl_current_reg.index;
	g_bl_indo.bl_current_reg.reg_addr[index] = REG_BL_BRIGHTNESS_MSB;
	g_bl_indo.bl_current_reg.normal_reg_var[index] = MSB_10MA;
	g_bl_indo.bl_current_reg.enhance_reg_var[index] =
		g_bl_level_enhance_mode >> MSB;

	RT4831_INFO("reg_addr:0x%x, normal_val=0x%x, enhance_val=0x%x\n",
		g_bl_indo.bl_current_reg.reg_addr[index],
		g_bl_indo.bl_current_reg.normal_reg_var[index],
		g_bl_indo.bl_current_reg.enhance_reg_var[index]);

	g_bl_indo.bl_current_reg.index++;

	RT4831_INFO("success\n");
}

static int rt4831_parsr_default_bias(struct device_node *np)
{
	int ret;

	if (np == NULL) {
		RT4831_ERR("np is NULL pointer\n");
		return -1;
	}

	ret = of_property_read_u32(np, RT4831_PULL_DOWN_BOOST,
		&g_bl_info.rt4831_pull_down_boost);
	if (ret) {
		RT4831_ERR("rt4831 parse default pull down boost failed!\n");
		return ret;
	}

	ret = of_property_read_u32(np, RT4831_PULL_UP_BOOST,
		&g_bl_info.rt4831_pull_up_boost);
	if (ret) {
		RT4831_ERR("rt4831 parse default pull up boost failed!\n");
		return ret;
	}

	ret = of_property_read_u32(np, RT4831_ENABLE_VSP_VSP,
		&g_bl_info.rt4831_enable_vsp_vsn);
	if (ret) {
		RT4831_ERR("rt4831 parse default enable vsp vsn failed!\n");
		return ret;
	}

	return ret;
}

/* initialize chip */
static int rt4831_chip_init(struct rt4831_chip_data *pchip)
{
	int ret;
	struct device_node *np = NULL;
	unsigned int enable_reg = 0;

	RT4831_INFO("in!\n");

	if (pchip == NULL) {
		RT4831_ERR("pchip is NULL pointer\n");
		return -1;
	}

	np = of_find_compatible_node(NULL, NULL, DTS_COMP_RT4831);
	if (!np) {
		RT4831_ERR("NOT FOUND device node %s!\n", DTS_COMP_RT4831);
		return -1;
	}

	ret = rt4831_parse_dts(np);
	if (ret < 0) {
		RT4831_ERR("parse rt4831 dts failed");
		return ret;
	}

	ret = of_property_read_u32(np, "rt4831_bl_max_level",
		&g_bl_level_enhance_mode);
	if (ret)
		g_bl_level_enhance_mode = BL_MAX;
	RT4831_INFO("g_bl_level_enhance_mode = %d\n",
		g_bl_level_enhance_mode);

	ret = of_property_read_u32(np, RT4831_PULL_BOOST_SUPPORT,
		&g_bl_info.rt4831_pull_boost_support);
	if (ret)
		g_bl_info.rt4831_pull_boost_support = 0;
	if (g_bl_info.rt4831_pull_boost_support) {
		ret = rt4831_parsr_default_bias(np);
		if (ret) {
			RT4831_ERR("parse default bias failed!\n");
			g_bl_info.rt4831_pull_boost_support = 0;
		}
	}
	RT4831_INFO("rt4831_pull_boost_support = %d\n",
		g_bl_info.rt4831_pull_boost_support);
	ret = of_property_read_u32(np, RT4831_VTC_LINE_BOOST,
		&g_bl_info.rt4831_vtc_line_boost);
	if (ret)
		RT4831_ERR("rt4831 parse vtc line boost failed!\n");

	if (dpu_runmode_is_factory()) {
		ret = of_property_read_u32(np,
			RT4831_RUNNING_RESUME_BL_TIMMER,
			&g_resume_bl_duration);
		if (ret)
			g_resume_bl_duration = 0;
	} else {
		ret = of_property_read_u32(np, RT4831_UPDATE_RESUME_BL_TIMMER,
			&g_resume_bl_duration);
		if (ret)
			g_resume_bl_duration = 0;
	}

	if (g_resume_bl_duration > MAX_BL_RESUME_TIMMER)
		g_resume_bl_duration = MAX_BL_RESUME_TIMMER;

	RT4831_INFO("g_resume_bl_duration = %d\n", g_resume_bl_duration);

	ret = rt4831_config_write(pchip, rt4831_reg_addr,
		rt4831_reg, RT4831_RW_REG_MAX);
	if (ret < 0) {
		RT4831_ERR("rt4831 config register failed");
		return ret;
	}

	ret = rt4831_config_read(pchip, rt4831_reg_addr, rt4831_reg,
		RT4831_RW_REG_MAX);
	if (ret < 0) {
		RT4831_ERR("rt4831 config read failed");
		return ret;
	}

	rt4831_bl_mode_reg_init(rt4831_reg_addr, rt4831_reg,
		RT4831_RW_REG_MAX);
	pchip->bl_resume_wq = create_singlethread_workqueue("bl_resume");
	if (!pchip->bl_resume_wq)
		RT4831_ERR("create bl_resume_wq failed\n");

	INIT_WORK(&pchip->bl_resume_worker,
		rt4831_bl_resume_workqueue_handler);
	hrtimer_init(&pchip->bl_resume_hrtimer,
		CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	pchip->bl_resume_hrtimer.function = rt4831_bl_resume_hrtimer_fnc;

	if (g_fake_lcd_flag) {
		RT4831_INFO("is unknown lcd\n");
		/* BL disable when fake lcd on */
		ret = regmap_write(pchip->regmap, REG_BL_ENABLE, enable_reg);
		if (ret < 0)
			return ret;

		/* Bias supply off when fake lcd on */
		ret = regmap_update_bits(pchip->regmap,
			REG_DISPLAY_BIAS_CONFIG_1,
			MASK_LCM_EN, BIAS_SUPPLY_OFF);
		RT4831_INFO("Bias supply off when fake lcd on\n");
		if (ret < 0)
			return ret;
	}

	RT4831_INFO("ok!\n");

	return ret;
}
static int rt4831_check_ovp_error(void)
{
	int ret = -1;
#if defined(CONFIG_HUAWEI_DSM)
	unsigned int val = 0;
#endif
	struct i2c_client *client = NULL;

	if (!rt4831_init_status) {
		RT4831_ERR("rt4831_init fail, return\n");
		return ret;
	}

	client = rt4831_g_chip->client;

#if defined(CONFIG_HUAWEI_DSM)
	if (g_fake_lcd_flag != 0)
		return ret;

	ret = regmap_read(rt4831_g_chip->regmap, REG_FLAGS, &val);
	RT4831_INFO("regmap_read val %u\n", val);
	if (ret < 0) {
		dev_err(&client->dev,
			"fail : read chip reg REG_FAULT_FLAG error\n");
		return ret;
	}

	if (val == DEVICE_FAULT_OCCUR)
		return ret;

	ret = dsm_client_ocuppy(lcd_dclient);
	if (ret) {
		dev_err(&client->dev, "dsm_client_ocuppy fail: ret=%d\n", ret);
		return ret;
	}
	dev_err(&client->dev, "fail:FLAGS=%d\n", val);
	dsm_client_record(lcd_dclient, "FLAGS=%d\n", val);
	dsm_client_notify(lcd_dclient, DSM_LCD_OVP_ERROR_NO);
#endif

	return ret;
}

static void rt4831_get_bias_config(int vsp, int vsn, int *outvsp, int *outvsn)
{
	int i;
	int vol_size = ARRAY_SIZE(voltage_table);

	for (i = 0; i < vol_size; i++) {
		if (voltage_table[i].voltage == vsp) {
			*outvsp = voltage_table[i].value;
			break;
		}
	}
	if (i >= vol_size) {
		RT4831_ERR("not found vsn voltage, use default voltage\n");
		*outvsp = RT4831_VOL_55;
	}

	for (i = 0; i < vol_size; i++) {
		if (voltage_table[i].voltage == vsn) {
			*outvsn = voltage_table[i].value;
			break;
		}
	}
	if (i >= vol_size) {
		RT4831_ERR("not found vsn voltage, use default voltage\n");
		*outvsn = RT4831_VOL_55;
	}
	RT4831_INFO("vpos = 0x%x, vneg = 0x%x\n", *outvsp, *outvsn);
}

static int rt4831_set_ic_disable(void)
{
	int ret;

	if (!rt4831_g_chip)
		return -1;

	/* reset backlight ic */
	ret = regmap_write(rt4831_g_chip->regmap, REG_BL_ENABLE, BL_RESET);
	if (ret < 0)
		RT4831_ERR("regmap_write REG_BL_ENABLE fail\n");

	/* clean up bl val register */
	ret = regmap_update_bits(rt4831_g_chip->regmap,
			REG_BL_BRIGHTNESS_LSB, MASK_BL_LSB, BL_DISABLE);
	if (ret < 0)
		RT4831_ERR("regmap_update_bits REG_BL_BRIGHTNESS_LSB fail\n");

	ret = regmap_write(rt4831_g_chip->regmap, REG_BL_BRIGHTNESS_MSB,
			BL_DISABLE);
	if (ret < 0)
		RT4831_ERR("regmap_write REG_BL_BRIGHTNESS_MSB fail\n");
	if (!ret)
		RT4831_INFO("successful\n");
	return ret;
}

static bool rt4831_is_need_bias_disable(void)
{
	return false;
}

static int rt4831_set_vtc_bias(int vpos, int vneg, bool state)
{
	int ret = 0;
	int vsp;
	int vsn;

	if (vpos == 0 || vneg == 0) {
		RT4831_ERR("vtc line test vsp/vsn config error\n");
		return ret;
	}
	if (!rt4831_g_chip)
		return -1;

	rt4831_get_bias_config(vpos, vneg, &vsp, &vsn);
	ret = regmap_write(rt4831_g_chip->regmap, REG_VPOS_BIAS, vsp);
	if (ret < 0) {
		RT4831_ERR("write pull_down_vsp failed\n");
		return ret;
	}
	ret = regmap_write(rt4831_g_chip->regmap, REG_VNEG_BIAS, vsn);
	if (ret < 0) {
		RT4831_ERR("write pull_down_vsn failed\n");
		return ret;
	}

	if ((g_bl_info.rt4831_enable_vsp_vsn == 0) ||
		(g_bl_info.rt4831_vtc_line_boost == 0)) {
		RT4831_ERR("vtc line test bias boost or config error\n");
		return ret;
	}
	ret = regmap_write(rt4831_g_chip->regmap,
		REG_DISPLAY_BIAS_CONFIG_1,
		g_bl_info.rt4831_enable_vsp_vsn);
	if (ret < 0) {
		RT4831_ERR("write enable_vsp_vsn failed\n");
		return ret;
	}
	if (state == true) {
		ret = regmap_write(rt4831_g_chip->regmap,
			REG_LCM_BOOST_BIAS, g_bl_info.rt4831_vtc_line_boost);
		if (ret < 0) {
			RT4831_ERR("write vtc line boost failed\n");
			return ret;
		}
	}
	RT4831_INFO("is successful\n");
	return ret;
}

static int rt4831_set_bias_power_down(int vpos, int vneg)
{
	int ret = 0;
	int vsp;
	int vsn;

	if (!rt4831_g_chip)
		return -1;

	if (!g_bl_info.rt4831_pull_boost_support) {
		RT4831_INFO("No need to pull down BOOST!\n");
		return ret;
	}

	rt4831_get_bias_config(vpos, vneg, &vsp, &vsn);

	if (!rt4831_g_chip)
		return -1;

	ret = regmap_write(rt4831_g_chip->regmap, REG_VPOS_BIAS, vsp);
	if (ret < 0) {
		RT4831_ERR("write pull_down_vsp failed!\n");
		return ret;
	}

	ret = regmap_write(rt4831_g_chip->regmap, REG_VNEG_BIAS, vsn);
	if (ret < 0) {
		RT4831_ERR("write pull_down_vsn failed!\n");
		return ret;
	}

	ret = regmap_write(rt4831_g_chip->regmap,
		REG_DISPLAY_BIAS_CONFIG_1,
		g_bl_info.rt4831_enable_vsp_vsn);
	if (ret < 0) {
		RT4831_ERR("write enable_vsp_vsn failed!\n");
		return ret;
	}

	ret = regmap_write(rt4831_g_chip->regmap,
		REG_LCM_BOOST_BIAS, g_bl_info.rt4831_pull_down_boost);
	if (ret < 0) {
		RT4831_ERR("write enable_vsp_vsn failed!\n");
		return ret;
	}
	RT4831_INFO("lcd_kit_pull_boost_for_rt4831 is successful\n");

	return ret;
}

static int rt4831_set_bias(int vpos, int vneg)
{
	int ret;

	if ((vpos < 0) || (vneg < 0)) {
		RT4831_ERR("vpos or vneg is error\n");
		return -1;
	}

	if (!rt4831_g_chip)
		return -1;

	ret = rt4831_config_write(rt4831_g_chip, rt4831_reg_addr,
			rt4831_reg, RT4831_RW_REG_MAX);
	if (ret < 0) {
		RT4831_ERR("rt4831 config register failed");
		return ret;
	}
	RT4831_INFO("is successful\n");
	return ret;

}

static void rt4831_check_fault(struct rt4831_chip_data *pchip,
	int last_level, int level)
{
	unsigned int val = 0;
	int ret;
#if defined(CONFIG_HUAWEI_DSM)
	int i;
#endif

	RT4831_INFO("backlight check FAULT_FLAG!\n");

	ret = regmap_read(pchip->regmap, REG_FLAGS, &val);
	if (ret < 0) {
		RT4831_INFO("read lm36274 FAULT_FLAG failed!\n");
		return;
	}

#if defined(CONFIG_HUAWEI_DSM)
	for (i = 0; i < FLAG_CHECK_NUM; i++) {
		if (!(err_table[i].flag & val))
			continue;
		RT4831_ERR("last_bkl:%d, cur_bkl:%d\n FAULT_FLAG:0x%x!\n",
			last_level, level, err_table[i].flag);
		ret = dsm_client_ocuppy(lcd_dclient);
		if (ret) {
			RT4831_ERR("dsm_client_ocuppy fail: ret=%d!\n", ret);
			continue;
		}
		dsm_client_record(lcd_dclient,
			"rt4831 last_bkl:%d, cur_bkl:%d\n FAULT_FLAG:0x%x!\n",
			last_level, level, err_table[i].flag);
		dsm_client_notify(lcd_dclient, err_table[i].err_no);
	}
#endif
}

/*
 * rt4831_set_backlight_reg(): Set Backlight working mode
 *
 * @bl_level: value for backlight ,range from 0 to 2047
 *
 * A value of zero will be returned on success, a negative errno will
 * be returned in error cases.
 */
int rt4831_set_backlight_reg(uint32_t bl_level)
{
	ssize_t ret = -1;
	uint32_t level;
	int bl_msb;
	int bl_lsb;
	static int last_level = -1;
	static int enable_flag;
	static int disable_flag;

	if (!rt4831_init_status) {
		RT4831_ERR("init fail, return\n");
		return ret;
	}

	if (down_trylock(&(rt4831_g_chip->test_sem))) {
		RT4831_INFO("Now in test mode\n");
		return 0;
	}

#ifdef CONFIG_LCD_KIT_DRIVER
	bl_level = bl_lvl_map(bl_level);
#endif

	RT4831_INFO("bl_level = %u\n", bl_level);

	level = bl_level;
	if (level > BL_MAX)
		level = BL_MAX;

	if (g_fake_lcd_flag) {
		if (level > 0) {
			if (!enable_flag) {
				ret = regmap_write(rt4831_g_chip->regmap,
					REG_BL_ENABLE,
					rt4831_reg[RT4831_RW_REG_MAX - 1]);
				RT4831_INFO("REG_BL_ENABLE = 0x%x\n",
					rt4831_reg[RT4831_RW_REG_MAX - 1]);
				mdelay(RT4831_IC_DELAY); // ic need
			}
			enable_flag = 1;
			disable_flag = 0;
		} else {
			if (!disable_flag) {
				ret = regmap_write(rt4831_g_chip->regmap,
					REG_BL_ENABLE, BL_DISABLE);
				RT4831_INFO("REG_BL_ENABLE = 0x0\n");
				mdelay(RT4831_IC_DELAY); // ic need
			}
			disable_flag = 1;
			enable_flag = 0;
		}
	}

	/* 11-bit brightness code */
	bl_msb = level >> MSB;
	bl_lsb = level & LSB;

	if (((last_level == BL_MIN) && (rt4831_msg_level == LOG_LEVEL_INFO)) ||
		((level == BL_MIN) && (rt4831_msg_level == LOG_LEVEL_INFO)) ||
		(last_level == -1))
		RT4831_INFO("level = %d, bl_msb = %d, bl_lsb = %d\n", level,
			bl_msb, bl_lsb);

	RT4831_DEBUG("level = %d, bl_msb = %d, bl_lsb = %d\n", level, bl_msb,
		bl_lsb);

	ret = regmap_update_bits(rt4831_g_chip->regmap, REG_BL_BRIGHTNESS_LSB,
		MASK_BL_LSB, bl_lsb);
	if (ret < 0)
		goto i2c_error;

	ret = regmap_write(rt4831_g_chip->regmap, REG_BL_BRIGHTNESS_MSB,
		bl_msb);
	if (ret < 0)
		goto i2c_error;

	/* Judge power on or power off */
	if (rt4831_fault_check_support &&
		((last_level <= 0 && level != 0) ||
		(last_level > 0 && level == 0)))
		rt4831_check_fault(rt4831_g_chip, last_level, level);

	last_level = level;
	up(&(rt4831_g_chip->test_sem));
	RT4831_INFO("exit succ\n");
	return ret;

i2c_error:
	up(&(rt4831_g_chip->test_sem));
	dev_err(rt4831_g_chip->dev, "%s:i2c access fail to register\n",
		__func__);
	RT4831_INFO("exit fail\n");
	return ret;
}

/*
 * rt4831_set_reg(): Set rt4831 reg
 *
 * @bl_reg: which reg want to write
 * @bl_mask: which bits of reg want to change
 * @bl_val: what value want to write to the reg
 *
 * A value of zero will be returned on success, a negative errno will
 * be returned in error cases.
 */
ssize_t rt4831_set_reg(u8 bl_reg, u8 bl_mask, u8 bl_val)
{
	ssize_t ret = -1;
	u8 reg = bl_reg;
	u8 mask = bl_mask;
	u8 val = bl_val;

	if (!rt4831_init_status) {
		RT4831_ERR("init fail, return\n");
		return ret;
	}

	if (reg < REG_MAX) {
		RT4831_ERR("Invalid argument\n");
		return ret;
	}

	RT4831_INFO("reg=0x%x,mask=0x%x,val=0x%x\n", reg, mask, val);

	ret = regmap_update_bits(rt4831_g_chip->regmap, reg, mask, val);
	if (ret < 0) {
		RT4831_ERR("i2c access fail to register\n");
		return ret;
	}

	return ret;
}
EXPORT_SYMBOL(rt4831_set_reg);

static ssize_t rt4831_reg_bl_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct rt4831_chip_data *pchip = NULL;
	struct i2c_client *client = NULL;
	ssize_t ret;
	unsigned int bl_lsb = 0;
	unsigned int bl_msb = 0;
	int bl_level;

	if (!dev)
		return snprintf(buf, PAGE_SIZE, "dev is null\n");

	pchip = dev_get_drvdata(dev);
	if (!pchip)
		return snprintf(buf, PAGE_SIZE, "data is null\n");

	client = pchip->client;
	if (!client)
		return snprintf(buf, PAGE_SIZE, "client is null\n");

	ret = regmap_read(pchip->regmap, REG_BL_BRIGHTNESS_MSB, &bl_msb);
	if (ret < 0)
		return snprintf(buf, PAGE_SIZE, "RT4831 I2C read error\n");

	ret = regmap_read(pchip->regmap, REG_BL_BRIGHTNESS_LSB, &bl_lsb);
	if (ret < 0)
		return snprintf(buf, PAGE_SIZE, "RT4831 I2C read error\n");

	bl_level = (bl_msb << MSB) | bl_lsb;

	return snprintf(buf, PAGE_SIZE, "RT4831 bl_level:%d\n", bl_level);
}

static ssize_t rt4831_reg_bl_store(struct device *dev,
	struct device_attribute *dev_attr,
	const char *buf, size_t size)
{
	ssize_t ret;
	struct rt4831_chip_data *pchip = NULL;
	unsigned int bl_level = 0;
	unsigned int bl_msb;
	unsigned int bl_lsb;

	if (!dev)
		return snprintf((char *)buf, PAGE_SIZE, "dev is null\n");

	pchip = dev_get_drvdata(dev);
	if (!pchip)
		return snprintf((char *)buf, PAGE_SIZE, "data is null\n");

	ret = kstrtouint(buf, 10, &bl_level); // 10 means decimal
	if (ret)
		goto out_input;

	RT4831_INFO("buf=%s,state=%d\n", buf, bl_level);

	if (bl_level > BL_MAX)
		bl_level = BL_MAX;

	/* 11-bit brightness code */
	bl_msb = bl_level >> MSB;
	bl_lsb = bl_level & LSB;

	RT4831_INFO("bl_level = %d, bl_msb = %d, bl_lsb = %d\n", bl_level,
		bl_msb, bl_lsb);

	ret = regmap_update_bits(pchip->regmap, REG_BL_BRIGHTNESS_LSB,
		MASK_BL_LSB, bl_lsb);
	if (ret < 0)
		goto i2c_error;

	ret = regmap_write(pchip->regmap, REG_BL_BRIGHTNESS_MSB, bl_msb);
	if (ret < 0)
		goto i2c_error;

	return size;

i2c_error:
	dev_err(pchip->dev, "%s:i2c access fail to register\n", __func__);
	return snprintf((char *)buf, PAGE_SIZE,
		"%s: i2c access fail to register\n", __func__);

out_input:
	dev_err(pchip->dev, "%s:input conversion fail\n", __func__);
	return snprintf((char *)buf, PAGE_SIZE,
		"%s: input conversion fail\n", __func__);
}

static DEVICE_ATTR(reg_bl, 0644, rt4831_reg_bl_show,
	rt4831_reg_bl_store);

static ssize_t rt4831_reg_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct rt4831_chip_data *pchip = NULL;
	struct i2c_client *client = NULL;
	ssize_t ret;
	unsigned char val[REG_MAX] = { 0 };

	if (!dev)
		return snprintf(buf, PAGE_SIZE, "dev is null\n");

	pchip = dev_get_drvdata(dev);
	if (!pchip)
		return snprintf(buf, PAGE_SIZE, "data is null\n");

	client = pchip->client;
	if (!client)
		return snprintf(buf, PAGE_SIZE, "client is null\n");

	ret = regmap_bulk_read(pchip->regmap, REG_REVISION, &val[0], REG_MAX);
	if (ret < 0)
		goto i2c_error;

	return snprintf(buf, PAGE_SIZE, "Revision(0x01)= 0x%x\n"
		"\rBacklight Configuration1(0x02)= 0x%x\n"
		"\rBacklight Configuration2(0x03) = 0x%x\n"
		"\rBacklight Brightness LSB(0x04) = 0x%x\n"
		"\rBacklight Brightness MSB(0x05) = 0x%x\n"
		"\rBacklight Auto-Frequency Low(0x06) = 0x%x\n"
		"\rBacklight Auto-Frequency High(0x07) = 0x%x\n"
		"\rBacklight Enable(0x08) = 0x%x\n"
		"\rDisplay Bias Configuration 1(0x09)  = 0x%x\n"
		"\rDisplay Bias Configuration 2(0x0A)  = 0x%x\n"
		"\rDisplay Bias Configuration 3(0x0B) = 0x%x\n"
		"\rLCM Boost Bias Register(0x0C) = 0x%x\n"
		"\rVPOS Bias Register(0x0D) = 0x%x\n"
		"\rVNEG Bias Register(0x0E) = 0x%x\n"
		"\rFlags Register(0x0F) = 0x%x\n"
		"\rBacklight Option 1 Register(0x10) = 0x%x\n"
		"\rBacklight Option 2 Register(0x11) = 0x%x\n"
		"\rPWM-to-Digital Code Readback LSB(0x12) = 0x%x\n"
		"\rPWM-to-Digital Code Readback MSB(0x13) = 0x%x\n",
		val[0], val[1], val[2], val[3], val[4], val[5], val[6],
		val[7], val[8], val[9], val[10], val[11], val[12],
		val[13], val[14], val[15], val[16], val[17], val[18]);
		/* 0~18 means number of registers */

i2c_error:
	return snprintf(buf, PAGE_SIZE, "%s: i2c access fail to register\n",
		__func__);
}

static ssize_t rt4831_reg_store(struct device *dev,
	struct device_attribute *dev_attr, const char *buf, size_t size)
{
	ssize_t ret;
	struct rt4831_chip_data *pchip = NULL;
	unsigned int reg = 0;
	unsigned int mask = 0;
	unsigned int val = 0;

	if (!dev)
		return snprintf((char *)buf, PAGE_SIZE, "dev is null\n");

	pchip = dev_get_drvdata(dev);
	if (!pchip)
		return snprintf((char *)buf, PAGE_SIZE, "data is null\n");

	ret = sscanf(buf, "reg=0x%x, mask=0x%x, val=0x%x", &reg, &mask, &val);
	if (ret < 0) {
		pr_err("check your input!!!\n");
		goto out_input;
	}

	if (reg > REG_MAX) {
		pr_err("Invalid argument!!!\n");
		goto out_input;
	}

	RT4831_INFO("%s:reg=0x%x,mask=0x%x,val=0x%x\n", __func__, reg,
		mask, val);

	ret = regmap_update_bits(pchip->regmap, reg, mask, val);
	if (ret < 0)
		goto i2c_error;

	return size;

i2c_error:
	dev_err(pchip->dev, "%s:i2c access fail to register\n", __func__);
	return snprintf((char *)buf, PAGE_SIZE,
		"%s: i2c access fail to register\n", __func__);

out_input:
	dev_err(pchip->dev, "%s:input conversion fail\n", __func__);
	return snprintf((char *)buf, PAGE_SIZE, "%s: input conversion fail\n",
		__func__);
}

static DEVICE_ATTR(reg, 0644, rt4831_reg_show,
	rt4831_reg_store);

static enum hrtimer_restart rt4831_bl_resume_hrtimer_fnc(struct hrtimer *timer)
{
	struct rt4831_chip_data *bl_chip_ctrl = NULL;

	bl_chip_ctrl = container_of(timer, struct rt4831_chip_data,
		bl_resume_hrtimer);
	if (bl_chip_ctrl == NULL) {
		RT4831_INFO("bl_chip_ctrl is NULL, return\n");
		return HRTIMER_NORESTART;
	}

	if (bl_chip_ctrl->bl_resume_wq)
		queue_work(bl_chip_ctrl->bl_resume_wq,
			&(bl_chip_ctrl->bl_resume_worker));

	return HRTIMER_NORESTART;
}
static void rt4831_bl_resume_workqueue_handler(struct work_struct *work)
{
	ssize_t error;
	struct rt4831_chip_data *bl_chip_ctrl = NULL;

	RT4831_INFO("in\n");
	if (work == NULL) {
		RT4831_ERR("work is NULL, return\n");
		return;
	}
	bl_chip_ctrl = container_of(work, struct rt4831_chip_data,
		bl_resume_worker);
	if (bl_chip_ctrl == NULL) {
		RT4831_ERR("bl_chip_ctrl is NULL, return\n");
		return;
	}

	if (down_interruptible(&(rt4831_g_chip->test_sem))) {
		if (bl_chip_ctrl->bl_resume_wq)
			queue_work(bl_chip_ctrl->bl_resume_wq,
			&(bl_chip_ctrl->bl_resume_worker));
		RT4831_INFO("down_trylock get sem fail return\n");
		return;
	}

	if (g_force_resume_bl_flag != RESUME_2_SINK &&
		g_force_resume_bl_flag != RESUME_REMP_OVP_OCP) {
		RT4831_ERR("g_force_resume_bl_flag = 0x%x, return\n",
			g_force_resume_bl_flag);
		up(&(rt4831_g_chip->test_sem));
		return;
	}

	error = rt4831_check_ovp_error();
	if (error)
		RT4831_ERR("rt4831_check_ovp_error return error\n");

	if (g_force_resume_bl_flag == RESUME_2_SINK) {
		RT4831_INFO("resume RESUME_LINK\n");
		/* set 4 link to 2 link */
		error = rt4831_config_write(rt4831_g_chip,
			g_bl_indo.bl_enable_reg.reg_addr,
			g_bl_indo.bl_enable_reg.normal_reg_var,
			g_bl_indo.bl_enable_reg.index);
		if (error) {
			RT4831_INFO("resume 2 sink fail return\n");
			if (bl_chip_ctrl->bl_resume_wq)
				queue_work(bl_chip_ctrl->bl_resume_wq,
					&(bl_chip_ctrl->bl_resume_worker));
			up(&(rt4831_g_chip->test_sem));
			return;
		}

		error = rt4831_config_write(rt4831_g_chip,
			g_bl_indo.bl_current_reg.reg_addr,
			g_bl_indo.bl_current_reg.normal_reg_var,
			g_bl_indo.bl_current_reg.index);
		if (error)
			RT4831_ERR("set bl_current_reg fail\n");

		g_force_resume_bl_flag = RESUME_REMP_OVP_OCP;

		/* 6ms */
		mdelay(BL_LOWER_POW_DELAY);
	}

	if (g_force_resume_bl_flag == RESUME_REMP_OVP_OCP) {
		RT4831_INFO("resume RESUME_REMP_OVP_OCP\n");
		error = rt4831_config_write(rt4831_g_chip,
			g_bl_indo.bl_mode_reg.reg_addr,
			g_bl_indo.bl_mode_reg.normal_reg_var,
			g_bl_indo.bl_mode_reg.index);
		if (error) {
			RT4831_INFO("resume OVP fail return\n");
			if (bl_chip_ctrl->bl_resume_wq)
				queue_work(bl_chip_ctrl->bl_resume_wq,
					&(bl_chip_ctrl->bl_resume_worker));
			up(&(rt4831_g_chip->test_sem));
			return;
		}

		g_force_resume_bl_flag = RESUME_IDLE;
	}

	up(&(rt4831_g_chip->test_sem));
	RT4831_INFO("exit\n");
}
int rt4831_get_bl_resume_timmer(int *timmer)
{
	int ret = -1;

	if (timmer == NULL)
		return ret;

	*timmer = g_resume_bl_duration;
	ret = 0;
	RT4831_INFO("timmer %d\n", *timmer);
	return ret;
}

ssize_t rt4831_blkit_set_normal_work_mode(void)
{
	ssize_t error = -1;

	if (!rt4831_init_status) {
		RT4831_ERR("rt4831_init fail, return\n");
		return error;
	}

	RT4831_INFO("in\n");
	if (down_interruptible(&rt4831_g_chip->test_sem)) {
		RT4831_ERR("down_trylock fail return\n");
		return error;
	}

	error = rt4831_config_write(rt4831_g_chip,
		g_bl_indo.bl_current_reg.reg_addr,
		g_bl_indo.bl_current_reg.normal_reg_var,
		g_bl_indo.bl_current_reg.index);
	if (error)
		RT4831_ERR("set bl_current_reg fail\n");
	else
		mdelay(BL_LOWER_POW_DELAY);  // 6ms

	/* set 4  to 2 sink */
	error = rt4831_config_write(rt4831_g_chip,
		g_bl_indo.bl_enable_reg.reg_addr,
		g_bl_indo.bl_enable_reg.normal_reg_var,
		g_bl_indo.bl_enable_reg.index);
	if (error) {
		RT4831_ERR("set bl_enable_reg fail\n");
		g_force_resume_bl_flag = RESUME_2_SINK;
		goto out;
	}

	/* 6ms */
	mdelay(BL_LOWER_POW_DELAY);

	error = rt4831_check_ovp_error();
	if (error)
		RT4831_ERR("rt4831_check_ovp_error return error\n");

	g_force_resume_bl_flag = RESUME_REMP_OVP_OCP;
	error = rt4831_config_write(rt4831_g_chip,
		g_bl_indo.bl_mode_reg.reg_addr,
		g_bl_indo.bl_mode_reg.normal_reg_var,
		g_bl_indo.bl_mode_reg.index);
	if (error) {
		RT4831_ERR("set bl_mode_reg fail\n");
		goto out;
	}

	if (g_resume_bl_duration != 0)
		hrtimer_cancel(&rt4831_g_chip->bl_resume_hrtimer);

	g_force_resume_bl_flag = RESUME_IDLE;

	up(&(rt4831_g_chip->test_sem));
	RT4831_INFO("exit\n");
	return error;

out:
	up(&(rt4831_g_chip->test_sem));
	RT4831_INFO("failed\n");
	return error;
}

ssize_t rtblkit_set_enhance_work_mode(void)
{
	ssize_t error = -1;

	if (!rt4831_init_status) {
		RT4831_ERR("rt4831_init fail, return\n");
		return error;
	}

	RT4831_INFO("in\n");
	if (down_interruptible(&rt4831_g_chip->test_sem)) {
		RT4831_ERR("down_trylock fail return\n");
		return error;
	}

	g_force_resume_bl_flag = RESUME_IDLE;
	error = rt4831_config_write(rt4831_g_chip,
		g_bl_indo.bl_mode_reg.reg_addr,
		g_bl_indo.bl_mode_reg.enhance_reg_var,
		g_bl_indo.bl_mode_reg.index);
	if (error) {
		RT4831_ERR("set bl_mode_reg fail\n");
		goto out;
	}

	g_force_resume_bl_flag = RESUME_REMP_OVP_OCP;
	error = rt4831_config_write(rt4831_g_chip,
		g_bl_indo.bl_enable_reg.reg_addr,
		g_bl_indo.bl_enable_reg.enhance_reg_var,
		g_bl_indo.bl_enable_reg.index);
	if (error) {
		RT4831_ERR("set bl_enable_reg fail\n");
		goto out;
	}

	error = rt4831_check_ovp_error();
	if (error)
		RT4831_ERR("rt4831_check_ovp_error return error\n");

	g_force_resume_bl_flag = RESUME_2_SINK;
	error = rt4831_config_write(rt4831_g_chip,
		g_bl_indo.bl_current_reg.reg_addr,
		g_bl_indo.bl_current_reg.enhance_reg_var,
		g_bl_indo.bl_current_reg.index);
	if (error)
		RT4831_ERR("set bl_current_reg fail\n");

	if (g_resume_bl_duration != 0) {
		hrtimer_cancel(&rt4831_g_chip->bl_resume_hrtimer);
		hrtimer_start(&rt4831_g_chip->bl_resume_hrtimer,
			ktime_set((g_resume_bl_duration +
				PROTECT_BL_RESUME_TIMMER) / 1000,
				((g_resume_bl_duration +
				PROTECT_BL_RESUME_TIMMER) % 1000) * 1000000),
				HRTIMER_MODE_REL);
	}  /* the values of 1000 and 1000000 is to change time to second */

	up(&(rt4831_g_chip->test_sem));
	RT4831_INFO("exit\n");
	return error;

out:
	up(&(rt4831_g_chip->test_sem));
	return error;
}

static const struct regmap_config rt4831_regmap = {
	.reg_bits = 8,
	.val_bits = 8,
	.reg_stride = 1,
};

/* pointers to created device attributes */
static struct attribute *rt4831_attributes[] = {
	&dev_attr_reg_bl.attr,
	&dev_attr_reg.attr,
	NULL,
};

static const struct attribute_group rt4831_group = {
	.attrs = rt4831_attributes,
};

#ifdef CONFIG_LCD_KIT_DRIVER
#include "lcd_kit_bl.h"

static struct lcd_kit_bl_ops bl_ops = {
	.set_backlight = rt4831_set_backlight_reg,
};
static struct lcd_kit_bias_ops bias_ops = {
	.set_bias_voltage = rt4831_set_bias,
	.set_bias_power_down = rt4831_set_bias_power_down,
	.set_ic_disable = rt4831_set_ic_disable,
	.set_vtc_bias_voltage = rt4831_set_vtc_bias,
	.set_bias_is_need_disable = rt4831_is_need_bias_disable,
};
#endif

static int rt4831_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = NULL;
	struct rt4831_chip_data *pchip = NULL;
	int ret;
#if defined(CONFIG_HUAWEI_DSM)
	unsigned int val = 0;
#endif

	RT4831_INFO("in!\n");
	if (client == NULL) {
		RT4831_ERR("client is NULL pointer\n");
		return -1;
	}

	adapter = client->adapter;
	if (adapter == NULL) {
		RT4831_ERR("adapter is NULL pointer\n");
		return -1;
	}

	if (!i2c_check_functionality(adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "i2c functionality check fail\n");
		return -EOPNOTSUPP;
	}

	pchip = devm_kzalloc(&client->dev, sizeof(struct rt4831_chip_data),
		GFP_KERNEL);
	if (!pchip)
		return -ENOMEM;

#ifdef CONFIG_REGMAP_I2C
	pchip->regmap = devm_regmap_init_i2c(client, &rt4831_regmap);
	if (IS_ERR(pchip->regmap)) {
		ret = PTR_ERR(pchip->regmap);
		dev_err(&client->dev, "fail : allocate register map: %d\n",
			ret);
		goto err_out;
	}
#endif

	pchip->client = client;
	i2c_set_clientdata(client, pchip);

	sema_init(&(pchip->test_sem), 1);

	/* chip initialize */
	ret = rt4831_chip_init(pchip);
	if (ret < 0) {
		dev_err(&client->dev, "fail : chip init\n");
		goto err_out;
	}

#if defined(CONFIG_HUAWEI_DSM)
	/*
	 * HARDWARE check why ovp occur  OVP(0x2) occur if disable bl ic then
	 * read reg_flags register, so comment it
	 */
	if (g_fake_lcd_flag) {
		ret = regmap_read(pchip->regmap, REG_FLAGS, &val);
		if (ret < 0) {
			dev_err(&client->dev,
				"fail:read chip reg REG_FAULT_FLAG error\n");
			goto err_out;
		}

		if (val != DEVICE_FAULT_OCCUR) {
			ret = dsm_client_ocuppy(lcd_dclient);
			if (!ret) {
				dev_err(&client->dev, "fail:FLAG=%d\n", val);
				dsm_client_record(lcd_dclient,
					"FLAG=%d\n", val);
				dsm_client_notify(lcd_dclient,
					DSM_LCD_OVP_ERROR_NO);
			} else {
				dev_err(&client->dev,
					"dsm_client_ocuppy fail:  ret=%d\n",
					ret);
			}
		}
	}
#endif

	pchip->dev = device_create(rt4831_class, NULL, 0, "%s", client->name);
	if (IS_ERR(pchip->dev)) {
		/* Not fatal */
		RT4831_ERR("Unable to create device; errno = %ld\n",
			PTR_ERR(pchip->dev));
		pchip->dev = NULL;
	} else {
		dev_set_drvdata(pchip->dev, pchip);
		ret = sysfs_create_group(&pchip->dev->kobj, &rt4831_group);
		if (ret)
			goto err_sysfs;
	}

	rt4831_g_chip = pchip;

	RT4831_INFO("name: %s, address: (0x%x) ok!\n",
		client->name, client->addr);
#ifdef CONFIG_LCD_KIT_DRIVER
	lcd_kit_bl_register(&bl_ops);
	lcd_kit_bias_register(&bias_ops);
#endif
	rt4831_init_status = true;

	return ret;

err_sysfs:
	device_destroy(rt4831_class, 0);
err_out:
	devm_kfree(&client->dev, pchip);
	return ret;
}

static int rt4831_suspend(struct device *dev)
{
	int ret;

	/* store reg val before suspend */
	rt4831_config_read(rt4831_g_chip, rt4831_reg_addr, g_reg_val,
		RT4831_RW_REG_MAX);
	/* reset backlight ic */
	ret = regmap_write(rt4831_g_chip->regmap, REG_BL_ENABLE, BL_RESET);
	if (ret < 0)
		dev_err(rt4831_g_chip->dev,
			"%s:i2c access fail to register\n", __func__);

	/* clean up bl val register */
	ret = regmap_update_bits(rt4831_g_chip->regmap,
		REG_BL_BRIGHTNESS_LSB, MASK_BL_LSB, BL_DISABLE);
	if (ret < 0)
		dev_err(rt4831_g_chip->dev,
			"%s:i2c access fail to register\n", __func__);

	ret = regmap_write(rt4831_g_chip->regmap, REG_BL_BRIGHTNESS_MSB,
		BL_DISABLE);
	if (ret < 0)
		dev_err(rt4831_g_chip->dev,
			"%s:i2c access fail to register\n", __func__);

	return ret;
}

static int rt4831_resume(struct device *dev)
{
	int ret;

	rt4831_init_status = true;

	ret = rt4831_config_write(rt4831_g_chip, rt4831_reg_addr, g_reg_val,
		RT4831_RW_REG_MAX);
	if (ret < 0)
		dev_err(rt4831_g_chip->dev,
			"%s:i2c access fail to register\n", __func__);

	return ret;
}

static SIMPLE_DEV_PM_OPS(rt4831_pm_ops, rt4831_suspend, rt4831_resume);
#define RT4831_PM_OPS    (&rt4831_pm_ops)

static int rt4831_remove(struct i2c_client *client)
{
	struct rt4831_chip_data *pchip = NULL;

	if (client == NULL) {
		RT4831_ERR("client is NULL pointer\n");
		return -1;
	}
	pchip = i2c_get_clientdata(client);

	regmap_write(pchip->regmap, REG_BL_ENABLE, BL_DISABLE);

	sysfs_remove_group(&client->dev.kobj, &rt4831_group);

	if (g_resume_bl_duration != 0)
		hrtimer_cancel(&rt4831_g_chip->bl_resume_hrtimer);

	return 0;
}

static const struct i2c_device_id rt4831_id[] = {
	{ RT4831_NAME, 0 },
	{}
};

static const struct of_device_id rt4831_of_id_table[] = {
	{ .compatible = "rt,rt4831" },
	{},
};

MODULE_DEVICE_TABLE(i2c, rt4831_id);
static struct i2c_driver rt4831_i2c_driver = {
	.driver = {
		.name = "rt4831",
		.owner = THIS_MODULE,
		.of_match_table = rt4831_of_id_table,
#ifndef CONFIG_LCD_KIT_DRIVER
		.pm = RT4831_PM_OPS,
#endif
	},
	.probe = rt4831_probe,
	.remove = rt4831_remove,
	.id_table = rt4831_id,
};

static int __init rt4831_module_init(void)
{
	int ret;

	RT4831_INFO("in\n");

	rt4831_class = class_create(THIS_MODULE, "rt4831");
	if (IS_ERR(rt4831_class)) {
		ret = PTR_ERR(rt4831_class);
		RT4831_ERR("Unable to create rt4831 class; errno = %d\n", ret);
		rt4831_class = NULL;
		return ret;
	}

	ret = i2c_add_driver(&rt4831_i2c_driver);
	if (ret) {
		RT4831_ERR("Unable to register rt4831 driver\n");
		return ret;
	}

	RT4831_INFO("ok!\n");

	return ret;
}
late_initcall(rt4831_module_init);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Backlight driver for rt4831");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
