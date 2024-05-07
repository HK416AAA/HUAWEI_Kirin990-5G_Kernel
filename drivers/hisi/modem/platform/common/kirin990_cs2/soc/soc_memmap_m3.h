#ifndef __SOC_MEMMAP_M3_H__
#define __SOC_MEMMAP_M3_H__ 
#ifdef __cplusplus
extern "C" {
#endif
#include "soc_memmap_comm.h"
#define LPMCU_DDR_PHY_TO_VIRT(phy_addr) ((phy_addr) + 0x20000000)
#define LPMCU_DDR_VIRT_TO_PHY(virt_addr) ((virt_addr) - 0x20000000)
#ifdef __cplusplus
}
#endif
#endif
