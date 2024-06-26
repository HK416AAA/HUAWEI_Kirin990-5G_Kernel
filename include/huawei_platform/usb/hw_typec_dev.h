/*
 * hw_typec_dev.h
 *
 * header file for typec driver
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

#ifndef _HW_TYPEC_DEV_H_
#define _HW_TYPEC_DEV_H_

#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>

/* type-c protocol current when charging other devices */
enum typec_output_current {
	TYPEC_HOST_CURRENT_DEFAULT = 0,
	TYPEC_HOST_CURRENT_MID,
	TYPEC_HOST_CURRENT_HIGH,
};

/* detected type-c protocol current when as a slave and in charge */
enum typec_input_current {
	TYPEC_DEV_CURRENT_DEFAULT = 0,
	TYPEC_DEV_CURRENT_MID,
	TYPEC_DEV_CURRENT_HIGH,
	TYPEC_DEV_CURRENT_NOT_READY,
};

/* control port mode */
enum typec_host_port_mode {
	TYPEC_HOST_PORT_MODE_DFP = 0,
	TYPEC_HOST_PORT_MODE_UFP,
	TYPEC_HOST_PORT_MODE_DRP,
	TYPEC_HOST_PORT_MODE_DEFAULT,
};

/* detected port mode */
enum typec_device_port_mode {
	TYPEC_DEV_PORT_MODE_DFP = 0,
	TYPEC_DEV_PORT_MODE_UFP,
	TYPEC_DEV_PORT_MODE_POWERACC,
	TYPEC_DEV_PORT_MODE_DEBUGACC,
	TYPEC_DEV_PORT_MODE_AUDIOACC,
	TYPEC_DEV_PORT_MODE_NOT_READY,
};

/* type-c inserted plug orientation */
enum typec_cc_orient {
	TYPEC_ORIENT_DEFAULT = 0,
	TYPEC_ORIENT_CC1,
	TYPEC_ORIENT_CC2,
	TYPEC_ORIENT_NOT_READY,
};

enum typec_attach_status {
	TYPEC_DETACH = 0,
	TYPEC_ATTACH,
	TYPEC_CUR_CHANGE_FOR_FSC,
	TYPEC_ACC_MODE_CHANGE,
	TYPEC_STATUS_NOT_READY,
};

struct typec_device_ops {
	int (*clean_int_mask)(void);
	int (*detect_attachment_status)(void);
	int (*detect_cc_orientation)(void);
	int (*detect_input_current)(void);
	int (*detect_port_mode)(void);
	int (*ctrl_output_current)(int value);
	int (*ctrl_port_mode)(int value);
};

struct typec_device_status {
	int attach_status; /* attach or detach of usb plug */
	int cc_orient;     /* type-c inserted plug orientation */
	int input_current; /* detected type-c protocol current as a slave */
	int port_mode;     /* detected type-c port mode, host or slave */
};

/* 4 registers are enough now */
#define REGISTER_NUM     4

struct typec_device_info {
	int gpio_enb;
	int gpio_intb;
	int irq_intb;
	bool typec_trigger_otg;
	u8 reg[REGISTER_NUM];

	struct i2c_client *client;
	struct device *dev;
	struct module *owner;
	struct typec_device_ops *ops;
	struct work_struct g_intb_work;
	struct typec_device_status dev_st;
	struct mutex typec_lock;
	struct wakeup_source *wake_lock;
	bool sink_attached;

#ifdef CONFIG_DUAL_ROLE_USB_INTF
	struct dual_role_phy_instance *dual_role;
	struct dual_role_phy_desc *desc;
	struct delayed_work g_wdog_work;
	struct completion reverse_completion;
	bool trysnk_attempt;
	int reverse_state;
#endif /* CONFIG_DUAL_ROLE_USB_INTF */
};

#ifdef CONFIG_DUAL_ROLE_USB_INTF
#define REVERSE_ATTEMPT             1
#define REVERSE_COMPLETE            2
#define TRYSNK_TIMEOUT_MS           500
#define DUAL_ROLE_SET_MODE_WAIT_MS  1500
#define WAIT_VBUS_OFF_MS            50
#endif /* CONFIG_DUAL_ROLE_USB_INTF */

struct typec_device_info *typec_chip_register(
	struct typec_device_info *device_info,
	struct typec_device_ops *ops, struct module *owner);

#endif /* _HW_TYPEC_DEV_H_ */
