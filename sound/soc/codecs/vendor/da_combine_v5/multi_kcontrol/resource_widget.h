/*
 * resource_widget.h -- da combine v5 codec driver
 *
 * Copyright (c) 2018 Huawei Technologies Co., Ltd.
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

#ifndef __DA_COMBINE_V5_RESOURCE_WIDGET_H__
#define __DA_COMBINE_V5_RESOURCE_WIDGET_H__

#include "linux/platform_drivers/da_combine/da_combine_v5_type.h"

int da_combine_v5_add_resource_widgets(struct snd_soc_component *codec, bool single_kcontrol);
int da_combine_v5_resmgr_init(struct da_combine_v5_platform_data *platform_data);
void da_combine_v5_supply_pll_init(struct snd_soc_component *codec);
void request_dp_clk(struct snd_soc_component *codec, bool enable);
#ifdef CONFIG_HIGH_RESISTANCE_HS_DET
void request_cp_dp_clk(struct snd_soc_component *codec);
void release_cp_dp_clk(struct snd_soc_component *codec);
#endif
int enable_hsmicbias(struct snd_soc_component *codec);
int s2_rx_power_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *kcontrol, int event);
void dp_clk_enable(struct snd_soc_component *codec);
int enable_supply(struct snd_soc_component *codec);
int enable_ibias(struct snd_soc_component *codec);
int pll48k_turn_on(struct snd_soc_component *codec);
#endif

