/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: misctrl for USB
 * Create: 2019-6-16
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/bitops.h>

#include "dwc3-chip.h"
#include "chip_usb3_misctrl.h"
#include "chip_usb3_31phy.h"
#include "common.h"

static void __iomem *misc_base_addr;
static void __iomem *sc_misc_base_addr;

void init_misc_ctrl_addr(void __iomem *base)
{
	pr_info("[USB.DBG]:misc base:%pK\n", base);
	misc_base_addr = base;
}

void init_sc_misc_ctrl_addr(void __iomem *base)
{
	pr_info("[USB.DBG]:sc misc base:%pK\n", base);
	sc_misc_base_addr = base;
}

void usb3_misc_reg_writel(u32 val, unsigned long int offset)
{
	usb3_rw_reg_writel(val, misc_base_addr, offset);
}

u32 usb3_misc_reg_readl(unsigned long int offset)
{
	return usb3_rw_reg_readl(misc_base_addr, offset);
}

void usb3_misc_reg_setbit(u32 bit, unsigned long int offset)
{
	usb3_rw_reg_setbit(bit, misc_base_addr, offset);
}

void usb3_misc_reg_clrbit(u32 bit, unsigned long int offset)
{
	usb3_rw_reg_clrbit(bit, misc_base_addr, offset);
}

void usb3_misc_reg_setvalue(u32 val, unsigned long int offset)
{
	usb3_rw_reg_setvalue(val, misc_base_addr, offset);
}

void usb3_misc_reg_clrvalue(u32 val, unsigned long int offset)
{
	usb3_rw_reg_clrvalue(val, misc_base_addr, offset);
}

u32 usb3_sc_misc_reg_readl(unsigned long int offset)
{
	return usb3_rw_reg_readl(sc_misc_base_addr, offset);
}

void usb3_sc_misc_reg_writel(u32 val, unsigned long int offset)
{
	usb3_rw_reg_writel(val, sc_misc_base_addr, offset);
}

void usb3_sc_misc_reg_setbit(u32 bit, unsigned long int offset)
{
	usb3_rw_reg_setbit(bit, sc_misc_base_addr, offset);
}

void usb3_sc_misc_reg_clrbit(u32 bit, unsigned long int offset)
{
	usb3_rw_reg_clrbit(bit, sc_misc_base_addr, offset);
}

void usb3_sc_misc_reg_setvalue(u32 val, unsigned long int offset)
{
	usb3_rw_reg_setvalue(val, sc_misc_base_addr, offset);
}

void usb3_sc_misc_reg_clrvalue(u32 val, unsigned long int offset)
{
	usb3_rw_reg_clrvalue(val, sc_misc_base_addr, offset);
}

static unsigned int misc_usecount;
static DEFINE_MUTEX(misc_ctrl_mutex);

static char *misc_ctrl_status_string(unsigned int misc_ctrl_status)
{
	char *s = NULL;

	if (misc_ctrl_status == (1 << MICS_CTRL_USB))
		s = "MICS_CTRL_USB";
	else if (misc_ctrl_status == (1 << MICS_CTRL_COMBOPHY))
		s = "MICS_CTRL_COMBOPHY";
	else if (misc_ctrl_status == ((1 << MICS_CTRL_USB) | (1 << MICS_CTRL_COMBOPHY)))
		s = "MICS_CTRL_USB_COMBOPHY";
	else if (misc_ctrl_status == 0)
		s = "MICS_CTRL_NONE";
	else
		s = "ilegal misc_ctrl_status";
	return s;
}

static char *misc_ctrl_type_string(enum misc_ctrl_type type)
{
	char *s = NULL;

	if (type == MICS_CTRL_USB)
		s = "MICS_CTRL_USB";
	else if (type == MICS_CTRL_COMBOPHY)
		s = "MICS_CTRL_COMBOPHY";
	else
		s = "ilegal misc_ctrl_type";
	return s;
}

int dwc3_misc_ctrl_get(enum misc_ctrl_type type)
{
	struct chip_usb_combophy *combophy = NULL;
	int ret;

	pr_debug("+ misc_usecount [%s] type[%s]\n",
			misc_ctrl_status_string(misc_usecount),
			misc_ctrl_type_string(type));

	if ((type != MICS_CTRL_USB) && (type != MICS_CTRL_COMBOPHY)) {
		pr_err("[USBERR] type[%d] is not correct\n", type);
		return -EINVAL;
	}

	combophy = usb3_get_combophy_phandle();
	if (!combophy) {
		pr_err("[USBERR] combophy module init error\n");
		return -EINVAL;
	}

	mutex_lock(&misc_ctrl_mutex);
	if (misc_usecount == 0) {
		pr_debug("it will be going to  release miscctrl\n");

		ret = usb3_open_misc_ctrl_clk();
		if (ret) {
			mutex_unlock(&misc_ctrl_mutex);
			usb_err("misc ctrl clk enable err\n");
			return -EPERM;
		}

		/* make sure Misc-ctrl at reset status */
		combophy_reset_misc_ctrl(combophy);
		udelay(100); /* delay 100us */

		/* dis-reset usb misc ctrl module */
		combophy_unreset_misc_ctrl(combophy);

		misc_usecount = 1 << (unsigned int)type;
	} else {
		pr_debug("%s has got, just return!\n",
				misc_ctrl_status_string(misc_usecount));
		misc_usecount =  misc_usecount | (1 << (unsigned int)type);
	}
	mutex_unlock(&misc_ctrl_mutex);
	pr_debug("-misc_usecount[%s]\n", misc_ctrl_status_string(misc_usecount));
	return 0;
}

void dwc3_misc_ctrl_put(enum misc_ctrl_type type)
{
	struct chip_usb_combophy *combophy = NULL;

	pr_debug("+ misc_usecount [%s] type[%s]\n",
			misc_ctrl_status_string(misc_usecount),
			misc_ctrl_type_string(type));

	if ((type != MICS_CTRL_USB) && (type != MICS_CTRL_COMBOPHY)) {
		pr_err("[USB.DBG] type[%d] is not correct\n", type);
		return;
	}

	combophy = usb3_get_combophy_phandle();
	if (!combophy) {
		pr_err("[USBERR] combophy module init error\n");
		return;
	}

	mutex_lock(&misc_ctrl_mutex);
	misc_usecount = misc_usecount & (~(unsigned int)(1 << (unsigned int)type));
	if (misc_usecount == 0) {
		pr_debug("it will be going to reset miscctrl\n");
		/* reset usb misc ctrl module */
		combophy_reset_misc_ctrl(combophy);

		usb3_close_misc_ctrl_clk();
	} else {
		pr_debug("%s has got, just return!\n",
				misc_ctrl_status_string(misc_usecount));
	}
	mutex_unlock(&misc_ctrl_mutex);
	pr_debug("-misc_usecount[%s]\n", misc_ctrl_status_string(misc_usecount));
}
