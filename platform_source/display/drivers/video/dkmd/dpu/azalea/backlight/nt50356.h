/*
 * nt50356.h
 *
 * bias+backlight driver of NT50356
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
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#if defined(CONFIG_HUAWEI_DSM)
#include <dsm/dsm_pub.h>
extern struct dsm_client *lcd_dclient;
#endif

#ifndef __LINUX_NT50356_H
#define __LINUX_NT50356_H

#define NT50356_NAME "nt50356"
#define DTS_COMP_NT50356 "nt,nt50356"
#define NT50356_HIDDEN_REG_SUPPORT "nt50356_hidden_reg_support"
#define NT50356_ONLY_BIAS                "only_bias"
#define NT50356_RUNNING_RESUME_BL_TIMMER "nt50356_running_resume_bl_timmer"
#define NT50356_UPDATE_RESUME_BL_TIMMER "nt50356_update_resume_bl_timmer"
#define NT50356_PULL_BOOST_SUPPORT "nt50356_pull_boost_support"
#define NT50356_PULL_DOWN_BOOST "nt50356_pull_down_boost"
#define NT50356_ENABLE_VSP_VSP "nt50356_enable_vsp_vsn"
#define NT50356_PULL_UP_BOOST "nt50356_pull_up_boost"
#define NT50356_VTC_LINE_BOOST "nt50356_vtc_line_boost"
#define NT50356_VTC_END_BOOST "nt50356_vtc_end_boost"

#define MAX_RATE_NUM 9
/* base reg */
#define REG_REVISION 0x01
#define REG_BL_CONFIG_1 0x02
#define REG_BL_CONFIG_2 0x03
#define REG_BL_BRIGHTNESS_LSB 0x04
#define REG_BL_BRIGHTNESS_MSB 0x05
#define REG_AUTO_FREQ_LOW 0x06
#define REG_AUTO_FREQ_HIGH 0x07
#define REG_BL_ENABLE 0x08
#define REG_DISPLAY_BIAS_CONFIG_1 0x09
#define REG_DISPLAY_BIAS_CONFIG_2 0x0A
#define REG_DISPLAY_BIAS_CONFIG_3 0x0B
#define REG_LCM_BOOST_BIAS 0x0C
#define REG_VPOS_BIAS 0x0D
#define REG_VNEG_BIAS 0x0E
#define REG_FLAGS 0x0F
#define REG_BL_OPTION_1 0x10
#define REG_BL_OPTION_2 0x11
#define REG_BL_CURRENT_CONFIG 0x20
#define REG_MAX 0x21
#define REG_HIDDEN_ADDR 0x6A
#define REG_SET_SECURITYBIT_ADDR 0x50
#define REG_SET_SECURITYBIT_VAL 0x08
#define REG_CLEAR_SECURITYBIT_VAL 0x00
/* add config reg for nt50356 TSD bug */
#define NT50356_REG_CONFIG_A9 0xA9
#define NT50356_REG_CONFIG_E8 0xE8
#define NT50356_REG_CONFIG_E9 0xE9

#define NT50356_VOL_45 0x0A /* 4.5V */
#define NT50356_VOL_46 0x0C /* 4.6V */
#define NT50356_VOL_47 0x0E /* 4.7V */
#define NT50356_VOL_48 0x0f /* 4.8V */
#define NT50356_VOL_49 0x12 /* 4.9V */
#define NT50356_VOL_50 0x14 /* 5.0V */
#define NT50356_VOL_51 0x16 /* 5.1V */
#define NT50356_VOL_52 0x18 /* 5.2V */
#define NT50356_VOL_53 0x1A /* 5.3V */
#define NT50356_VOL_54 0x1C /* 5.4V */
#define NT50356_VOL_55 0x1E /* 5.5V */
#define NT50356_VOL_56 0x20 /* 5.6V */
#define NT50356_VOL_57 0x22 /* 5.7V */
#define NT50356_VOL_575 0x23 /* 5.75V */
#define NT50356_VOL_58 0x24 /* 5.8V */
#define NT50356_VOL_59 0x26 /* 5.9V */
#define NT50356_VOL_60 0x28 /* 6.0V */
#define NT50356_VOL_61 0x2A /* 6.1V */
#define NT50356_VOL_62 0x2C /* 6.2V */

/* mask code */
#define MASK_BL_LSB 0x07
#define MASK_LCM_EN 0xE0
#define MASK_SOFTWARE_RESET 0x80

/* update bit val */
#define DEVICE_FAULT_OCCUR 0
#define DEVICE_RESET 0x1

#define BL_MIN 0
#define BL_MAX 2047
#define MSB 3
#define LSB 0x07

#define MIN_BL_RESUME_TIMMER 1
#define MAX_BL_RESUME_TIMMER 400
#define PROTECT_BL_RESUME_TIMMER 28
#define NT50356_IC_DELAY 16

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#define LOG_LEVEL_INFO 8

#define NT50356_EMERG(msg, ...) \
	do { \
		if (nt50356_msg_level > 0) \
			pr_emerg("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)
#define NT50356_ALERT(msg, ...) \
	do { \
		if (nt50356_msg_level > 1) \
			pr_alert("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)
#define NT50356_CRIT(msg, ...) \
	do { \
		if (nt50356_msg_level > 2) \
			pr_crit("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)
#define NT50356_ERR(msg, ...) \
	do { \
		if (nt50356_msg_level > 3) \
			pr_err("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)
#define NT50356_WARNING(msg, ...) \
	do { \
		if (nt50356_msg_level > 4) \
			pr_warn("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)
#define NT50356_NOTICE(msg, ...) \
	do { \
		if (nt50356_msg_level > 5) \
			pr_notice("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)
#define NT50356_INFO(msg, ...) \
	do { \
		if (nt50356_msg_level > 6) \
			pr_info("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)
#define NT50356_DEBUG(msg, ...) \
	do { \
		if (nt50356_msg_level > 7) \
			pr_devel("[nt50356]%s: " msg, __func__, ##__VA_ARGS__); \
	} while (0)

struct nt50356_chip_data {
	struct device *dev;
	struct i2c_client *client;
	struct regmap *regmap;
	struct semaphore test_sem;
	struct work_struct bl_resume_worker;
	struct workqueue_struct *bl_resume_wq;
	struct hrtimer bl_resume_hrtimer;
};

#if defined(CONFIG_HUAWEI_DSM)
#define FLAG_CHECK_NUM 3

enum {
	OVP_FAULT_FLAG = 0x02,
	OCP_FAULT_FLAG = 0x01,
	TSD_FAULT_FLAG = 0x40,
};

struct nt50356_check_flag {
	u8 flag;
	int err_no;
};

static struct nt50356_check_flag err_table[] = {
	{OVP_FAULT_FLAG, DSM_LCD_OVP_ERROR_NO},
	{OCP_FAULT_FLAG, DSM_LCD_BACKLIGHT_OCP_ERROR_NO},
	{TSD_FAULT_FLAG, DSM_LCD_BACKLIGHT_TSD_ERROR_NO},
};
#endif

#define NT50356_RW_REG_MAX 17

struct nt50356_backlight_information {
	int nt50356_pull_boost_support;
	int nt50356_pull_down_boost;
	int nt50356_pull_up_boost;
	int nt50356_enable_vsp_vsn;
	int nt50356_vtc_line_boost;
	int nt50356_vtc_end_boost;
};

int nt50356_reg[NT50356_RW_REG_MAX] = { 0 };


static char *nt50356_dts_string[NT50356_RW_REG_MAX] = {
	"nt50356_bl_config_1",
	"nt50356_bl_config_2",
	"nt50356_auto_freq_low",
	"nt50356_auto_freq_high",
	"nt50356_reg_config_e8",
	"nt50356_reg_config_e9",
	"nt50356_reg_config_a9",
	"nt50356_display_bias_config_1",
	"nt50356_display_bias_config_2",
	"nt50356_display_bias_config_3",
	"nt50356_lcm_boost_bias",
	"nt50356_vpos_bias",
	"nt50356_vneg_bias",
	"nt50356_bl_option_1",
	"nt50356_bl_option_2",
	"nt50356_bl_current_config",
	"nt50356_bl_en",
};
struct nt50356_voltage {
	u32 voltage;
	int value;
};

static struct nt50356_voltage voltage_table[] = {
	{ 4500000, NT50356_VOL_45 },
	{ 4600000, NT50356_VOL_46 },
	{ 4700000, NT50356_VOL_47 },
	{ 4800000, NT50356_VOL_48 },
	{ 4900000, NT50356_VOL_49 },
	{ 5000000, NT50356_VOL_50 },
	{ 5100000, NT50356_VOL_51 },
	{ 5200000, NT50356_VOL_52 },
	{ 5300000, NT50356_VOL_53 },
	{ 5400000, NT50356_VOL_54 },
	{ 5500000, NT50356_VOL_55 },
	{ 5600000, NT50356_VOL_56 },
	{ 5700000, NT50356_VOL_57 },
	{ 5750000, NT50356_VOL_575 },
	{ 5800000, NT50356_VOL_58 },
	{ 5900000, NT50356_VOL_59 },
	{ 6000000, NT50356_VOL_60 },
	{ 6100000, NT50356_VOL_61 },
	{ 6200000, NT50356_VOL_62 },
};
static unsigned int nt50356_reg_addr[NT50356_RW_REG_MAX] = {
	REG_BL_CONFIG_1,
	REG_BL_CONFIG_2,
	REG_AUTO_FREQ_LOW,
	REG_AUTO_FREQ_HIGH,
	NT50356_REG_CONFIG_E8,
	NT50356_REG_CONFIG_E9,
	NT50356_REG_CONFIG_A9,
	REG_DISPLAY_BIAS_CONFIG_1,
	REG_DISPLAY_BIAS_CONFIG_2,
	REG_DISPLAY_BIAS_CONFIG_3,
	REG_LCM_BOOST_BIAS,
	REG_VPOS_BIAS,
	REG_VNEG_BIAS,
	REG_BL_OPTION_1,
	REG_BL_OPTION_2,
	REG_BL_CURRENT_CONFIG,
	REG_BL_ENABLE,
};

static unsigned int g_reg_val[NT50356_RW_REG_MAX] = { 0 };

enum bl_enable {
	EN_2_SINK = 0x15,
	EN_4_SINK = 0x1F,
	BL_RESET = 0x80,
	BL_DISABLE = 0x00,
};

enum lcm_en {
	NORMAL_MODE = 0x80,
	BIAS_SUPPLY_OFF = 0x00,
};

enum {
	BL_OVP_25V = 0x40,
	BL_OVP_29V = 0x60,
};

enum {
	CURRENT_RAMP_0US = 0x85,
	CURRENT_RAMP_5MS = 0xAD,
};

enum {
	LSB_MIN = 0x00,
	LSB_10MA = 0x05,
	LSB_30MA = 0x07,
};

enum {
	MSB_MIN = 0x00,
	MSB_10MA = 0xD2,
	MSB_30MA = 0xFF,
};

enum bl_option_2 {
	BL_OCP_1 = 0x35, /* 1.2A */
	BL_OCP_2 = 0x36, /* 1.5A */
	BL_OCP_3 = 0x37, /* 1.8A */
};

enum {
	BIAS_BOOST_TIME_0 = 0x00, /* 156ns */
	BIAS_BOOST_TIME_1 = 0x10, /* 181ns */
	BIAS_BOOST_TIME_2 = 0x20, /* 206ns */
	BIAS_BOOST_TIME_3 = 0x30, /* 231ns */
};

enum resume_type {
	RESUME_IDLE = 0x0,
	RESUME_2_SINK = 0x1,
	RESUME_REMP_OVP_OCP = 0x2,
};

#define BL_CONFIG_MODE_REG_NUM 3
#define BL_CONFIG_CURR_REG_NUM 2
#define BL_CONFIG_ENABLE_REG_NUM 1
#define BL_LOWER_POW_DELAY 6
#define BL_MAX_PREFLASHING_TIMER 800

/* bl_mode_config reg */
#define BL_MAX_CONFIG_REG_NUM 3

struct bl_config_reg {
	unsigned int index;
	unsigned int reg_addr[BL_MAX_CONFIG_REG_NUM];
	unsigned int normal_reg_var[BL_MAX_CONFIG_REG_NUM];
	unsigned int enhance_reg_var[BL_MAX_CONFIG_REG_NUM];
};

struct backlight_work_mode_reg_info {
	struct bl_config_reg bl_mode_reg;
	struct bl_config_reg bl_current_reg;
	struct bl_config_reg bl_enable_reg;
};

int nt50356_set_backlight_reg(uint32_t bl_level);
ssize_t nt50356_set_reg(u8 bl_reg, u8 bl_mask, u8 bl_val);

#endif /* __LINUX_NT50356_H */

