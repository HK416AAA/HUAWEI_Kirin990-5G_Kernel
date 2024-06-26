/*
 * hisi_pcie_idle_sleep.h
 *
 * PCIe idle sleep vote interfaces for EP
 *
 * Copyright (c) 2016-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef _PCIE_IDLE_SLEEP_H_
#define _PCIE_IDLE_SLEEP_H_

#include <platform_include/basicplatform/linux/pcie-kport-api.h>

#if defined(CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP)
static inline void kirin_pcie_refclk_device_vote(u32 ep_type, u32 rc_id, u32 vote)
{
	pcie_kport_refclk_device_vote(ep_type, rc_id, vote);
}
#else
static inline void pcie_kport_refclk_device_vote(u32 ep_type, u32 rc_id, u32 vote)
{
}
static inline void kirin_pcie_refclk_device_vote(u32 ep_type, u32 rc_id, u32 vote)
{
}
#endif /* CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP */

#endif /* _PCIE_IDLE_SLEEP_H_ */
