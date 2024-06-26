/*
 * npu_atf_subsys.c
 *
 * about npu atf subsys
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
#include <asm/compiler.h>
#include <linux/compiler.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/platform_drivers/npu_pm.h>
#include <linux/arm-smccc.h>
#include "npu_atf_subsys.h"
#include "npu_log.h"

noinline int atfd_service_npu_smc(u64 _service_id, u64 _cmd, u64 _arg1,
	u64 _arg2)
{
	struct arm_smccc_res res = {0};
	arm_smccc_smc(_service_id, _cmd, _arg1, _arg2, 0, 0, 0, 0, &res);
	return (int)res.a0;
}

int npuatf_start_secmod(u64 work_mode, u64 canary)
{
	int ret;

	ret = atfd_service_npu_smc(NPU_START_SECMODE, 0, work_mode, canary);
	return ret;
}

int npuatf_enable_secmod(u64 secure_mode)
{
	int ret;

	npu_drv_debug("npu_pm_power_on start\n");
	npu_drv_boot_time_tag("start npu_pm_power_on\n");  // based on sw interface for npu subsys/npu bus poweron
#ifndef NPU_POWER_UP_SELF_CONTROL
	ret = npu_pm_power_on();
	if (ret) {
		npu_drv_err("pm power on failed ,ret = %d\n", ret);
		return -1;
	}
#endif
	npu_drv_boot_time_tag("start atfd_service_npu_smc\n");
	ret = atfd_service_npu_smc(NPU_ENABLE_SECMODE, secure_mode,
		NPUIP_NPU_SUBSYS, NPU_FLAGS_POWER_ON);
	if (ret) {
		npu_drv_err("NPU_ENABLE_SECMODE failed ,ret = %d\n", ret);
		return -1;
	}

	return ret;
}

int npuatf_powerup_aicore(u64 secure_mode, u32 aic_flag)
{
	int ret;

	npu_drv_debug("npu_pm_power_on aicore start\n");
	npu_drv_boot_time_tag("start atfd_service_npu_smc\n");
	ret = atfd_service_npu_smc(NPU_ENABLE_SECMODE, secure_mode,
		NPUIP_AICORE, aic_flag);
	if (ret) {
		npu_drv_err("NPU_ENABLE_SECMODE failed ,ret = %d\n", ret);
		return -1;
	}

	return ret;
}

int npu_init_sdma_tbu(u64 secure_mode, u32 flag)
{
	int ret;

	npu_drv_boot_time_tag("npu init sdma tbu start");
	ret = atfd_service_npu_smc(NPU_ENABLE_SECMODE, secure_mode,
		NPUIP_SDMA, flag);
	if (ret) {
		npu_drv_err("NPU_ENABLE_SECMODE init sdma failed ,ret = %d\n", ret);
		return -1;
	}

	return ret;
}

int npuatf_power_down_ts_secreg(u32 work_mode)
{
	int ret;

	ret = atfd_service_npu_smc(NPU_POWER_DOWN_TS_SEC_REG, work_mode, 0, 0);
	return ret;
}

int npuatf_power_down(u32 mode)
{
	int ret;
	// power down npu cpu
	ret = atfd_service_npu_smc(NPU_CPU_POWER_DOWN_SECMODE, mode,
		NPUIP_NPU_SUBSYS, NPU_FLAGS_POWER_OFF);
	if (ret != 0)
		npu_drv_err("npu_smc or power_off fail NPU_CPU_POWER_DOWN_SECMODE ,ret: %d\n",
			ret);

	return ret;
}
int npuatf_power_down_aicore(u64 secure_mode, u32 aic_flag)
{
	int ret;

	npu_drv_debug("npu_power_down aicore start\n");
	ret = atfd_service_npu_smc(NPU_CPU_POWER_DOWN_SECMODE, secure_mode,
		NPUIP_AICORE, aic_flag);
	if (ret) {
		npu_drv_err("NPU_CPU_POWER_DOWN_SECMODE failed ,ret = %d\n", ret);
		return -1;
	}

	return ret;
}

int npuatf_power_down_tcu(void)
{
	int ret;

	npu_drv_info("power down tcu start\n");
	ret = atfd_service_npu_smc(NPU_CPU_POWER_DOWN_SECMODE, 0,
		NPUIP_TCU, 0);
	if (ret) {
		npu_drv_err("power down tcu failed ,ret = %d\n", ret);
		return -1;
	}

	return ret;
}

int atf_query_gic0_state(u64 cmd)
{
	int ret;

	ret = atfd_service_npu_smc(GIC_CFG_CHECK_SECMODE, cmd, 0, 0);
	return ret;
}

int acpu_gic0_online_ready(u64 cmd)
{
	int ret;

	ret = atfd_service_npu_smc(GIC_ONLINE_READY_SECMODE, cmd, 0, 0);
	return ret;
}

int npuatf_enable_tbu(u64 cmd)
{
	int ret;

	npu_drv_debug("start\n");
	ret = atfd_service_npu_smc(NPU_POWER_UP_SMMU_TBU, cmd, 0, 0);
	if (ret) {
		npu_drv_err("failed ,ret = %d\n", ret);
		return -1;
	}
	return ret;
}

int npuatf_disable_tbu(u64 cmd, u32 aic_flag)
{
	int ret;

	npu_drv_debug("start\n");
	ret = atfd_service_npu_smc(NPU_POWER_DOWN_SMMU_TBU, cmd, 0, aic_flag);
	if (ret) {
		npu_drv_err("atfd service smc failed ,ret = %d\n", ret);
		return -1;
	}
	return ret;
}

int npuatf_switch_hwts_aicore_pool(u64 aic_pool_switch_info)
{
	int ret;

	npu_drv_debug("start\n");
	ret = atfd_service_npu_smc(NPU_SWITCH_HWTS_AICORE_POOL,
		0, NPUIP_NONE, aic_pool_switch_info);
	if (ret) {
		npu_drv_err("npu atf switch hwts aicore pool failed, ret = %d\n", ret);
		return -1;
	}
	return ret;
}

int npuatf_lowpower_ops(u64 lowpower_dev, u64 lowpower_flag)
{
	int ret;

	ret = atfd_service_npu_smc(NPU_LOWERPOWER_OPS, 0,
		lowpower_dev, lowpower_flag);
	if (ret != 0) {
		npu_drv_err("atfd service smc failed ,ret = %d\n", ret);
		return -1;
	}
	return ret;
}

int npuatf_aicore_pmu_set()
{
	int ret;

	ret = atfd_service_npu_smc(NPU_PROFILING_SETTING, 0, 0, 0);
	if (ret != 0) {
		npu_drv_err("atfd service smc failed ,ret = %d\n", ret);
		return -1;
	}
	return ret;
}
