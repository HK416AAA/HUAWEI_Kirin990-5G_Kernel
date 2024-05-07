/** @file
 * Copyright (c) 2020-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "hdmirx_link_edid.h"
#include <hdmirx_common.h>

#define BYTES_WRITE_EACH_TIME 4
#define BITS_PER_BYTES 8
#define BYTES_PER_BLOCK 128

#define HISI_EDID_TEST
#define ONLY_1080P_30FPS
#ifdef HISI_EDID_TEST
#ifdef ONLY_1080P_30FPS

static bool g_edid21_flag = false;

static const uint8_t g_edid_hdmi_20[] = {
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x58, 0xAE, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,
	0x33, 0x18, 0x01, 0x03, 0x81, 0x00, 0x00, 0x78, 0x0A, 0xEE, 0x9D, 0xA3, 0x54, 0x4C, 0x99, 0x26,
	0x0F, 0x47, 0x4A, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x27,

	0x02, 0x03, 0x30, 0x74, 0x41, 0x22, 0x32, 0x09, 0x7F, 0x01, 0x15, 0x07, 0x50, 0x57, 0x07, 0x00,
	0x3D, 0x07, 0x50, 0x5F, 0x7E, 0x01, 0x67, 0x7E, 0x01, 0x83, 0x01, 0x00, 0x00, 0xE2, 0x00, 0xCF,
	0xE3, 0x05, 0xD8, 0x00, 0xEB, 0x01, 0x46, 0xD0, 0x00, 0x43, 0x0C, 0x43, 0x98, 0x99, 0xEE, 0xD3,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C,
};
#else
static const uint8_t g_edid_hdmi_20[] = {
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x58, 0xAE, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,
	0x33, 0x18, 0x01, 0x03, 0x81, 0x00, 0x00, 0x78, 0x0A, 0xEE, 0x9D, 0xA3, 0x54, 0x4C, 0x99, 0x26,
	0x0F, 0x47, 0x4A, 0xBF, 0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
	0x45, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x66, 0x21, 0x50, 0xB0, 0x51, 0x00, 0x1B, 0x30,
	0x40, 0x70, 0x36, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x41,
	0x42, 0x20, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x30, 0x4C, 0x0F, 0x55, 0x3C, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xC9,

	0x02, 0x03, 0x63, 0x74, 0x57, 0x61, 0x60, 0x66, 0x65, 0x02, 0x04, 0x11, 0x13, 0x20, 0x22, 0x90,
	0x21, 0x1F, 0x06, 0x15, 0x03, 0x12, 0x16, 0x05, 0x14, 0x5D, 0x5E, 0x5F, 0x32, 0x09, 0x7F, 0x01,
	0x15, 0x07, 0x50, 0x57, 0x07, 0x00, 0x3D, 0x07, 0x50, 0x5F, 0x7E, 0x01, 0x67, 0x7E, 0x01, 0x83,
	0x01, 0x00, 0x00, 0x6D, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x3C, 0x20, 0x00, 0x60, 0x01, 0x02,
	0x03, 0x67, 0xD8, 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x03, 0xE2, 0x00, 0xCF, 0xE3, 0x05, 0xD8, 0x00,
	0xE2, 0x0F, 0x0F, 0xE3, 0x06, 0x0D, 0x01, 0xEB, 0x01, 0x46, 0xD0, 0x00, 0x43, 0x0C, 0x43, 0x98,
	0x99, 0xEE, 0xD3, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4,
	0x8E, 0x21, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9B
};
#endif
#else
static const uint8_t g_edid_hdmi_20[] = {
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x06, 0xB3, 0x8B, 0x27, 0x01, 0x01, 0x01, 0x01,
	0x2F, 0x1E, 0x01, 0x03, 0x80, 0x3C, 0x22, 0x78, 0x3A, 0x0B, 0x10, 0xB0, 0x4C, 0x45, 0xA8, 0x26,
	0x0A, 0x50, 0x54, 0xBF, 0xCF, 0x00, 0x71, 0x4F, 0x81, 0xC0, 0x81, 0x40, 0x81, 0x80, 0xD1, 0xC0,
	0xD1, 0xFC, 0x95, 0x00, 0xB3, 0x00, 0x08, 0xE8, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
	0x8A, 0x00, 0x54, 0x4F, 0x21, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x30, 0x78, 0x1E,
	0x87, 0x3C, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x52,
	0x4F, 0x47, 0x20, 0x58, 0x47, 0x32, 0x37, 0x55, 0x51, 0x0A, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFF,
	0x00, 0x4C, 0x42, 0x4C, 0x4D, 0x51, 0x53, 0x31, 0x38, 0x33, 0x33, 0x33, 0x33, 0x20, 0x01, 0xAF,

	0x02, 0x03, 0x52, 0xF2, 0x55, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x01, 0x02, 0x03, 0x11, 0x12, 0x13,
	0x04, 0x0E, 0x0F, 0x1D, 0x1E, 0x14, 0x05, 0x1F, 0x10, 0x3F, 0xE2, 0x00, 0xD5, 0xE3, 0x05, 0xC0,
	0x00, 0x23, 0x09, 0x07, 0x07, 0x83, 0x01, 0x00, 0x00, 0xE2, 0x0F, 0x18, 0x6D, 0x03, 0x0C, 0x00,
	0x20, 0x00, 0x38, 0x3C, 0x20, 0x00, 0x60, 0x01, 0x02, 0x03, 0x67, 0xD8, 0x5D, 0xC4, 0x01, 0x78,
	0x88, 0x01, 0xE6, 0x06, 0x05, 0x01, 0x70, 0x70, 0x07, 0x68, 0x1A, 0x00, 0x00, 0x01, 0x0B, 0x30,
	0x78, 0xFE, 0x56, 0x5E, 0x00, 0xA0, 0xA0, 0xA0, 0x29, 0x50, 0x30, 0x20, 0x35, 0x00, 0x54, 0x4F,
	0x21, 0x00, 0x00, 0x1A, 0x6F, 0xC2, 0x00, 0xA0, 0xA0, 0xA0, 0x55, 0x50, 0x30, 0x20, 0x35, 0x00,
	0x54, 0x4F, 0x21, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC4
};
#endif

static const uint8_t g_edid_hdmi_21[] = {

	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x04, 0x43, 0x01, 0x20, 0x00, 0x0E, 0x00, 0x00,
	0x0B, 0x1E, 0x01, 0x03, 0x80, 0x66, 0x39, 0x78, 0x0A, 0xA8, 0x33, 0xAB, 0x50, 0x45, 0xA5, 0x27,
	0x0D, 0x48, 0x48, 0x21, 0x08, 0x00, 0x71, 0x4F, 0x81, 0xC0, 0x81, 0x00, 0x81, 0x80, 0x95, 0x00,
	0xA9, 0xC0, 0xB3, 0x00, 0xD1, 0xC0, 0x08, 0xE8, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
	0x8A, 0x00, 0x50, 0x1D, 0x74, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x18, 0x78, 0x0F,
	0xFF, 0x77, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x44,
	0x44, 0x46, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFF,
	0x00, 0x44, 0x44, 0x46, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x9B,

	0x02, 0x03, 0x59, 0xF0, 0x5C, 0x61, 0x10, 0x1F, 0x04, 0x13, 0x05, 0x14, 0x21, 0x22, 0x5E, 0x5F,
	0x60, 0x65, 0x66, 0x64, 0x03, 0x12, 0x75, 0x76, 0xDA, 0xDB, 0xC3, 0xC4, 0xC6, 0xC7, 0xC8, 0xC9,
	0x01, 0x29, 0x09, 0x07, 0x07, 0x15, 0x07, 0x50, 0x57, 0x07, 0x00, 0x83, 0x01, 0x00, 0x00, 0xE2,
	0x00, 0xCF, 0xE3, 0x05, 0xDB, 0x01, 0x6E, 0x03, 0x0C, 0x00, 0x40, 0x00, 0xB8, 0x3C, 0x28, 0x00,
	0x80, 0x01, 0x02, 0x03, 0x04, 0x6D, 0xD8, 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x13, 0x00, 0x00, 0x00,
	0x88, 0x13, 0x3F, 0xE5, 0x0F, 0x01, 0x38, 0xFE, 0x07, 0x56, 0x5E, 0x00, 0xA0, 0xA0, 0xA0, 0x29,
	0x50, 0x30, 0x20, 0x35, 0x00, 0x50, 0x1D, 0x74, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B
};

int hdmirx_link_load_edid(struct hdmirx_ctrl_st *hdmirx, const uint8_t *edid, unsigned int length)
{
	unsigned int cnt = length / BYTES_WRITE_EACH_TIME;
	unsigned int each_cnt = 0;
	unsigned int offset  = 0;
	unsigned int reg_value;
	unsigned int i;

	if ((length == 0) || (length % BYTES_PER_BLOCK))
		return -1;

	disp_pr_info("[hdmirx] edid length is %d, cnt %d", length, cnt);

	for (i = 0; i < cnt; i++) {
		offset = i * BYTES_WRITE_EACH_TIME;
		reg_value = 0;
		each_cnt = 0;
		while (each_cnt < BYTES_WRITE_EACH_TIME) {
			reg_value = reg_value | ((*(edid + offset + each_cnt)) << (each_cnt * BITS_PER_BYTES));
			each_cnt++;
		}

		disp_pr_info("addr 0x%x write edid 0x%x", hdmirx->hdmirx_aon_base + 0x2800 + offset, reg_value);

		set_reg(hdmirx->hdmirx_aon_base + 0x2800 + offset, reg_value, 32, 0);
	}

	return 0;
}

int hdmirx_link_edid_init(struct hdmirx_ctrl_st *hdmirx)
{
	// close DDC
	int ret;
	set_reg(hdmirx->hdmirx_aon_base + 0x10A0, 0, 32, 0);

	if (g_edid21_flag)
		ret = hdmirx_link_load_edid(hdmirx, g_edid_hdmi_21, sizeof(g_edid_hdmi_21));
	else
		ret = hdmirx_link_load_edid(hdmirx, g_edid_hdmi_20, sizeof(g_edid_hdmi_20));

	// open DDC
	set_reg(hdmirx->hdmirx_aon_base + 0x10A0, 1, 32, 0);

	return ret;
}

void hdmirx_edid_select(uint32_t version)
{
	disp_pr_info("[hdmirx]hdmi version select\n");
	if (version == 0)
		g_edid21_flag = false;
	else
		g_edid21_flag = true;
}
EXPORT_SYMBOL_GPL(hdmirx_edid_select);
