#ifndef _BSP_SMMU_H
#define _BSP_SMMU_H
#include <osl_types.h>

typedef enum {
    SMMU_PCIE0 = 0,
    SMMU_PCIE1 = 1,
} smmu_dev_id;

#ifdef CONFIG_HISI_SMMU_V3
int bsp_smmu_power_on(smmu_dev_id smmuid);
int bsp_smmu_power_off(smmu_dev_id smmuid);

/* Bypass only can be used when SMMU power on*/
int bsp_smmu_bypass_enable(smmu_dev_id smmuid);
int bsp_smmu_bypass_disable(smmu_dev_id smmuid);
#else
static inline int bsp_smmu_power_on(smmu_dev_id smmuid)
{
    UNUSED(smmuid);
    return 0;
}
static inline int bsp_smmu_power_off(smmu_dev_id smmuid)
{
    UNUSED(smmuid);
    return 0;
}
static inline int bsp_smmu_bypass_enable(smmu_dev_id smmuid)
{
    UNUSED(smmuid);
    return 0;
}
static inline int bsp_smmu_bypass_disable(smmu_dev_id smmuid)
{
    UNUSED(smmuid);
    return 0;
}
#endif

#endif
