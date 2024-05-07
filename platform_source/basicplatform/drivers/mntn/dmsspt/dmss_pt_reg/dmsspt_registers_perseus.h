/*
 * Copyright @ Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: dmsspt's platform register macro
 * Create: 2020-3-10
 */

#ifndef _DMSSPT_REGISTERS_PERSEUS_H_
#define _DMSSPT_REGISTERS_PERSEUS_H_

#define PLATFORM_LIST \
	{.compatible = "hisilicon,perseus-dmsspt",}, \
	{},\

#define DDRC_DMI_NUM	2

#define DDRC_FILTER_SRC_PLATFORM

/* pattern trace related registers */
#define SOC_DMSS_GLB_INT_STATUS_STR	SOC_DMSS_GLB_INT_STATUS1_UNION
#define SOC_DMSS_GLB_INT_CLEAR_STR	SOC_DMSS_GLB_INT_CLEAR1_UNION
#define SOC_DMSS_GLB_INT_STATUS_PTR	SOC_DMSS_GLB_INT_STATUS1_ADDR
#define SOC_DMSS_GLB_INT_CLEAR_PTR	SOC_DMSS_GLB_INT_CLEAR1_ADDR

#define DMSS_GLB_INT_STATUS_PTR(base) \
	((volatile SOC_DMSS_GLB_INT_STATUS_STR *)SOC_DMSS_GLB_INT_STATUS_PTR(base))
#define DMSS_GLB_INT_CLEAR_PTR(base) \
	((volatile SOC_DMSS_GLB_INT_CLEAR_STR *)SOC_DMSS_GLB_INT_CLEAR_PTR(base))
#define DMSS_GLB_ADDR_INTLV_PTR(base) \
	((volatile SOC_DMSS_GLB_ADDR_INTLV_UNION *)SOC_DMSS_GLB_ADDR_INTLV_ADDR(base))
#define DMSS_GLB_TRACE_CTRL0_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_CTRL0_UNION *)SOC_DMSS_GLB_TRACE_CTRL0_ADDR(base))
#define DMSS_GLB_TRACE_CTRL1_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_CTRL1_UNION *)SOC_DMSS_GLB_TRACE_CTRL1_ADDR(base))
#define DMSS_GLB_TRACE_CTRL2_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_CTRL2_UNION *)SOC_DMSS_GLB_TRACE_CTRL2_ADDR(base))
#define DMSS_GLB_TRACE_REC0_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_REC0_UNION *)SOC_DMSS_GLB_TRACE_REC0_ADDR(base))
#define DMSS_GLB_TRACE_REC1_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_REC1_UNION *)SOC_DMSS_GLB_TRACE_REC1_ADDR(base))
#define DMSS_GLB_TRACE_FILTER0_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_FILTER0_UNION *)SOC_DMSS_GLB_TRACE_FILTER0_ADDR(base))
#define DMSS_GLB_TRACE_FILTER1_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_FILTER1_UNION *)SOC_DMSS_GLB_TRACE_FILTER1_ADDR(base))
#define DMSS_GLB_TRACE_FILTER2_PTR(base, mid_grps) \
	((volatile SOC_DMSS_GLB_TRACE_FILTER2_UNION *)SOC_DMSS_GLB_TRACE_FILTER2_ADDR(base, mid_grps))
#define DMSS_GLB_TRACE_FREQ_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_FREQ_UNION *)SOC_DMSS_GLB_TRACE_FREQ_ADDR(base))
#define DMSS_GLB_TRACE_INT_EN_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_INT_EN_UNION *)SOC_DMSS_GLB_TRACE_INT_EN_ADDR(base))
#define DMSS_GLB_TRACE_LOCK_DOWN_PTR(base) \
	((volatile SOC_DMSS_GLB_TRACE_LOCK_DOWN_UNION *)SOC_DMSS_GLB_TRACE_LOCK_DOWN_ADDR(base))
#define DMSS_GLB_TRACE_STAT0_PTR(base, dmis) \
	((volatile SOC_DMSS_GLB_TRACE_STAT0_UNION *)SOC_DMSS_GLB_TRACE_STAT0_ADDR(base, dmis))
#define DMSS_GLB_TRACE_STAT1_PTR(base, dmis) \
	((volatile SOC_DMSS_GLB_TRACE_STAT1_UNION *)SOC_DMSS_GLB_TRACE_STAT1_ADDR(base, dmis))
#define DMSS_GLB_TRACE_STAT2_PTR(base, dmis) \
	((volatile SOC_DMSS_GLB_TRACE_STAT2_UNION *)SOC_DMSS_GLB_TRACE_STAT2_ADDR(base, dmis))

#define INTLV_GRAN_MAX_VAL	0x3
#define INTLV_GRAN_START_VALUE0

#define CPU_ASI_NUM	6
#define DDR_SIZE_16G	0x400000000ULL
#define DDR_SIZE_16G512M	0x420000000ULL
#define DDR_SIZE_32G	0x800000000ULL
#define DDR_SIZE_32G512M	0x820000000ULL
#define ADDR_SHIFT_MODE_1_START_ADDR	DDR_SIZE_16G
#define ADDR_SHIFT_MODE_1_END_ADDR	DDR_SIZE_16G512M
#define ADDR_SHIFT_MODE_2_START_ADDR	DDR_SIZE_32G
#define ADDR_SHIFT_MODE_2_END_ADDR	DDR_SIZE_32G512M

#endif
