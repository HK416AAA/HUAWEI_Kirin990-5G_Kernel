/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: driver for a standalone usb phy chip
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef _CHIP_USB_PHY_H_
#define _CHIP_USB_PHY_H_

#include <linux/platform_drivers/usb/chip_usb.h>
#include <linux/platform_drivers/usb/tca.h>
#include <linux/phy/phy.h>
#include <linux/types.h>

struct chip_usb2_phy {
	struct phy *phy;
	enum chip_charger_type (*detect_charger_type)(
			struct chip_usb2_phy *usb2_phy);
	int (*set_dpdm_pulldown)(struct chip_usb2_phy *usb2_phy, bool pulldown);
	int (*set_vdp_src)(struct chip_usb2_phy *usb2_phy, bool enable);
};

struct chip_combophy {
	struct phy *phy;
	int (*set_mode)(struct chip_combophy *combophy,
			enum tcpc_mux_ctrl_type mode_type,
			enum typec_plug_orien_e typec_orien);
	ssize_t (*info_dump)(struct chip_combophy *combophy, char *buf,
			     size_t size);
	int (*register_debugfs)(struct chip_combophy *combophy,
				struct dentry *root);
	void (*regdump)(struct chip_combophy *combophy);
};

#endif /* _CHIP_USB_PHY_H_ */
