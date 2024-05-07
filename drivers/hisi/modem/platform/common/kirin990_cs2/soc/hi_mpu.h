#ifndef __HI_MPU_H__
#define __HI_MPU_H__ 
#include <product_config.h>
#include "hi_efuse.h"
#ifdef MODEM_SANITIZER
#define MPU_MDM_EXE_SIZE (DDR_MCORE_SIZE)
#define MPU_MDM_NR_EXE_SIZE (DDR_MCORE_NR_SIZE)
#define DDR_ATTRIBUTE (MPU_READ_WRTER | MPU_NOEXE | MPU_MEM_NOCACHEABLE | MPU_SHAREABLE)
#define sanitizer_map(index) \
do { \
    (void)mpu_region_cfg(index++, MDM_SANTIZIER_MEM_ADDR, MPU_REGION_64M, MPU_READ_WRTER | MPU_EXE | MPU_MEM_CACHEABLE | MPU_SHAREABLE); \
    (void)mpu_region_cfg(index++, MDM_SANTIZIER_NR_MEM_ADDR, MPU_REGION_8M, MPU_READ_WRTER | MPU_EXE | MPU_MEM_CACHEABLE | MPU_SHAREABLE); \
} while (0)
#else
#define MPU_MDM_EXE_SIZE (DDR_MCORE_SIZE - DDR_MCORE_UNCACHE_SIZE)
#define MPU_MDM_NR_EXE_SIZE (DDR_MCORE_NR_SIZE - DDR_MCORE_NR_UNCACHE_SIZE)
#define DDR_ATTRIBUTE (MPU_STRONGLY_ORDER | MPU_NOEXE)
#define sanitizer_map(index) 
#endif
#define SHARE_DDR_SIZE SZ_32M
#ifdef CONFIG_NXDEP_MPU
extern u32 num_of_region_unused(void);
extern int ioremap_readonly(void* phy_addr, unsigned int len, void* virt_addr);
#endif
static inline void text_protect_cfg(void)
{
#ifdef CONFIG_NXDEP_MPU
    int ret = 0;
    unsigned long virt_addr = 0;
    #if defined(__OS_LRCCPU__) && defined(CONFIG_CCPU_HAS_TCM)
    if(num_of_region_unused() > 0) {
        ret = ioremap_readonly((void *)CCPU_ITCM_ADDR, CCPU_ITCM_SIZE, (void *)&virt_addr);
        if(ret == BSP_ERROR) {
            osl_printf("ITCM text protect failed 3\n");
        }
    }
    #elif defined(__OS_NRCCPU__) && defined(CONFIG_NRCCPU_HAS_TCM)
    if(num_of_region_unused() > 0) {
        ret = ioremap_readonly((void *)NRCCPU_ITCM_ADDR, NRCCPU_ITCM_SIZE, (void *)&virt_addr);
        if(ret == BSP_ERROR) {
            osl_printf("NR ITCM text protect failed 3\n");
        }
    }
    #endif
#endif
}
#if (((BL31_BASE_ADDR + BL31_SHM_OFFSET) & 0x000000ff) + 0x20) > 0x100
#error "offset of BL31 is incorrect for mpu."
#endif
#define MPU_BL31_EFUSE_ADDR ((BL31_BASE_ADDR + BL31_SHM_OFFSET) & 0xffffff00)
#define platform_mpu_init(index) \
do { \
    (void)mpu_region_cfg(index++, 0x00000000, MPU_REGION_4G, DDR_ATTRIBUTE);\
    (void)mpu_region_cfg(index++, CCPU_ITCM_ADDR, MPU_REGION_32K, MPU_READ_WRTER | MPU_EXE | MPU_MEM_NOCACHEABLE);\
    (void)mpu_region_cfg(index++, CCPU_DTCM_ADDR, MPU_REGION_16K, MPU_READ_WRTER | MPU_NOEXE | MPU_MEM_NOCACHEABLE);\
    (void)mpu_region_cfg(index++, 0xE0000000, MPU_REGION_512M, MPU_READ_WRTER | MPU_NOEXE | MPU_DEVICE | MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, CCPU_LLRAM_BASE_ADDR, MPU_REGION_512K, MPU_READ_WRTER | MPU_NOEXE | MPU_MEM_NOCACHEABLE | MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, HI_DSP_SUBSYSTEM_BASE_ADDR, MPU_REGION_16M, MPU_READ_WRTER | MPU_NOEXE | MPU_STRONGLY_ORDER | MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, (HI_DSP_SUBSYSTEM_BASE_ADDR + SZ_16M) , MPU_REGION_4M , MPU_READ_WRTER | MPU_NOEXE | MPU_STRONGLY_ORDER | MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, 0xF0000000 , MPU_REGION_64M, MPU_READ_WRTER | MPU_NOEXE | MPU_STRONGLY_ORDER | MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, DDR_MEM_ADDR, MPU_REGION_512M, MPU_READ_WRTER | MPU_NOEXE | MPU_MEM_NOCACHEABLE | MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, MPU_BL31_EFUSE_ADDR, MPU_REGION_256B, MPU_READ_ONlY | MPU_NOEXE | MPU_MEM_NOCACHEABLE | MPU_SHAREABLE);\
    sanitizer_map(index);\
} while (0)
#define platform_mpu_init_nrccpu(index) \
do { \
    (void)mpu_region_cfg(index++, 0x00000000 , MPU_REGION_4G , DDR_ATTRIBUTE);\
    (void)mpu_region_cfg(index++, NRCCPU_ITCM_ADDR , MPU_REGION_16K , MPU_READ_WRTER | MPU_EXE | MPU_MEM_NOCACHEABLE| MPU_NOSHAREABLE);\
    (void)mpu_region_cfg(index++, NRCCPU_DTCM_ADDR , MPU_REGION_64K , MPU_READ_WRTER | MPU_NOEXE | MPU_MEM_NOCACHEABLE| MPU_NOSHAREABLE);\
    (void)mpu_region_cfg(index++, 0xE0000000 , MPU_REGION_512M, MPU_READ_WRTER | MPU_NOEXE | MPU_DEVICE | MPU_SHAREABLE );\
    (void)mpu_region_cfg(index++, NRCCPU_LLRAM_BASE_ADDR , MPU_REGION_1M , MPU_READ_WRTER | MPU_NOEXE | MPU_MEM_NOCACHEABLE| MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, HI_DSP_SUBSYSTEM_BASE_ADDR , MPU_REGION_16M , MPU_READ_WRTER | MPU_NOEXE | MPU_STRONGLY_ORDER | MPU_SHAREABLE );\
    (void)mpu_region_cfg(index++, (HI_DSP_SUBSYSTEM_BASE_ADDR + SZ_16M) , MPU_REGION_4M , MPU_READ_WRTER | MPU_NOEXE | MPU_STRONGLY_ORDER | MPU_SHAREABLE );\
    (void)mpu_region_cfg(index++, 0xF0000000 , MPU_REGION_64M , MPU_READ_WRTER | MPU_NOEXE | MPU_STRONGLY_ORDER | MPU_SHAREABLE );\
    (void)mpu_region_cfg(index++, DDR_MEM_ADDR, MPU_REGION_512M, MPU_READ_WRTER | MPU_NOEXE | MPU_MEM_NOCACHEABLE | MPU_SHAREABLE);\
    (void)mpu_region_cfg(index++, MPU_BL31_EFUSE_ADDR, MPU_REGION_256B, MPU_READ_ONlY | MPU_NOEXE | MPU_MEM_NOCACHEABLE | MPU_SHAREABLE);\
    sanitizer_map(index);\
} while (0)
#endif
