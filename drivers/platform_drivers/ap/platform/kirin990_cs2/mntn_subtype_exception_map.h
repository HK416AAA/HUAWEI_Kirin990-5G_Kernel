#include "mntn_subtype_exception.h"
#include "mntn_public_interface.h"
#include "mntn_subtype_exception.h"
#include "mntn_public_interface.h"
#ifndef __AP_DDRC_SEC_SUBTYPE_MAP
#define __AP_DDRC_SEC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __MMC_EXCEPTION_SUBTYPE_MAP
#define __MMC_EXCEPTION_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __AP_PANIC_SUBTYPE_MAP
#define __AP_PANIC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __AP_BL31PANIC_SUBTYPE_MAP
#define __AP_BL31PANIC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __AP_VENDOR_PANIC_SUBTYPE_MAP
#define __AP_VENDOR_PANIC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __APWDT_HWEXC_SUBTYPE_MAP
#define __APWDT_HWEXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __APWDT_EXC_SUBTYPE_MAP
#define __APWDT_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __LPM3_EXC_SUBTYPE_MAP
#define __LPM3_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __MEM_REPAIR_EXC_SUBTYPE_MAP
#define __MEM_REPAIR_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __SCHARGER_EXC_SUBTYPE_MAP
#define __SCHARGER_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __PMU_EXC_SUBTYPE_MAP
#define __PMU_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __NPU_EXC_SUBTYPE_MAP
#define __NPU_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __CONN_EXC_SUBTYPE_MAP
#define __CONN_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __GENERAL_SEE_EXC_SUBTYPE_MAP
#define __GENERAL_SEE_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __IVP_EXC_SUBTYPE_MAP
#define __IVP_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __DSS_EXC_SUBTYPE_MAP
#define __DSS_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __ALOADER_EXC_SUBTYPE_MAP
#define __ALOADER_EXC_SUBTYPE_MAP(x,y,z) 
#endif
#ifndef __HM_EXC_SUBTYPE_MAP
#define __HM_EXC_SUBTYPE_MAP(x,y,z) 
#endif
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_UNKNOWN_MASTER)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_LPMCU)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IOMCU_M7)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_PCIE_1)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_PERF_STAT)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_MODEM)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_DJTAG_M)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IOMCU_DMA)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_UFS)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_SD)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_SDIO)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_CC712)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_FD_UL)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_DPC)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_USB31OTG)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_TOP_CSSYS)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_DMAC)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_ASP_HIFI)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_PCIE_0)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_ATGS)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_ASP_DMA)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_LATMON)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_ISP)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_DSS)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IPP_SUBSYS_JPGENC)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_MEDIA_COMMON_CMD)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_MEDIA_COMMON_RW)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_VENC)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_VDEC)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IVP)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IPP_SUBSYS_JPGDEC)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IPP_SUBSYS_FD)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IPP_SUBSYS_CPE)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IPP_SUBSYS_SLAM)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_NPU)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_FCM)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_GPU)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_IDI2AXI)
__AP_DDRC_SEC_SUBTYPE_MAP(AP_S_DDRC_SEC, PANIC, DMSS_CRYPTO_ENHANCE)
__MMC_EXCEPTION_SUBTYPE_MAP(MMC_S_EXCEPTION,HARDWARE_FAULT,MMC_EXCEPT_INIT_FAIL)
__MMC_EXCEPTION_SUBTYPE_MAP(MMC_S_EXCEPTION,HARDWARE_FAULT,MMC_EXCEPT_CMD_TIMEOUT)
__MMC_EXCEPTION_SUBTYPE_MAP(MMC_S_EXCEPTION,HARDWARE_FAULT,MMC_EXCEPT_COLDBOOT)
__MMC_EXCEPTION_SUBTYPE_MAP(MMC_S_EXCEPTION,HARDWARE_FAULT,MMC_UFSDEV_HARDERR)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_RESERVED)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_BC_PANIC)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_L3CACHE_ECC1)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_L3CACHE_ECC2)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_SOFTLOCKUP)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_OHARDLOCKUP)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_HARDLOCKUP)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_ISP)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_IVP)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_GPU)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,PANIC,HI_APPANIC_Storage)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,MODEM_REBOOTSYS,HI_APPANIC_MODEM)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU0_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU1_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU2_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU3_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU4_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU5_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU6_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU7_L1_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU0_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU1_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU2_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU3_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU4_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU5_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU6_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU7_L2_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU0_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU1_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU2_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU3_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU4_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU5_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU6_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU7_L1_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU0_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU1_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU2_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU3_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU4_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU5_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU6_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_CPU7_L2_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_L3_CE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_L3_UE)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_LB)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_PLL_UNLOCK)
__AP_PANIC_SUBTYPE_MAP(AP_S_PANIC,HARDWARE_FAULT,HI_APPANIC_EARLY_PANIC)
__AP_PANIC_SUBTYPE_MAP(AP_S_BL31_PANIC,PANIC,HI_APBL31PANIC_RESERVED)
__AP_PANIC_SUBTYPE_MAP(AP_S_BL31_PANIC,PANIC,HI_APBL31PANIC_ASSERT)
__AP_VENDOR_PANIC_SUBTYPE_MAP(AP_S_VENDOR_PANIC, PANIC, HI_APVNDPANIC_RESERVED)
__AP_VENDOR_PANIC_SUBTYPE_MAP(AP_S_VENDOR_PANIC, PANIC, HI_APVNDPANIC_CFI)
__APWDT_HWEXC_SUBTYPE_MAP(AP_S_AWDT,HWWATCHDOG,HI_APWDT_BL31)
__APWDT_HWEXC_SUBTYPE_MAP(AP_S_AWDT,HWWATCHDOG,HI_APWDT_LPM3)
__APWDT_HWEXC_SUBTYPE_MAP(AP_S_AWDT,HWWATCHDOG,HI_APWDT_APBL31LPM3)
__APWDT_HWEXC_SUBTYPE_MAP(AP_S_AWDT,HWWATCHDOG,HI_APWDT_AP)
__APWDT_HWEXC_SUBTYPE_MAP(AP_S_AWDT,HWWATCHDOG,HI_APWDT_BL31AP)
__APWDT_HWEXC_SUBTYPE_MAP(AP_S_AWDT,HWWATCHDOG,HI_APWDT_BL31LPM3)
__APWDT_EXC_SUBTYPE_MAP(AP_S_AWDT,HARDWARE_FAULT,HI_APWDT_HW)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_RESERVED)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_AP_WDT_LOCAL)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_AP_WDT_REMOTE)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_M3_WDT_LOCAL)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_M3_WDT_REMOTE)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_AP_SYS_PANIC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_M3_SYS_PANIC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_PMUSSI_PANIC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_CLK_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_REGULATOR_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_OCBC_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DMA_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_NOC_TIMEOUT)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_G3D_PWR_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_CPU_PWR_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_TSENSOR_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_CPUDVFS_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_GPUDVFS_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_MEMRP_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_NOC_BUS_IDLE_PEND)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_AHB_TIMEOUT)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_PERIFIX_UP_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_SR_STATUS_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PCSI_PERIVOLT_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_PANIC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_FATAL_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_SREF_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_OSC_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_TMON_LOW)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_TMON_HIGH)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_GATE_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_UCE0_EXC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_UCE1_EXC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_UCE2_EXC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_UCE3_EXC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_AREF_ALARM)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_RDTIMEOUT)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_PLLUNLOCK_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_RETRAIN_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_TMON_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_DFS_ENBP_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_DVALID_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_DFI_SEL_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_PLLUNLOCK_LP)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_UNKNOWN_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_UCE_EXC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_DDR_DFS_EXBP_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_DMSS_VOLT_NULL)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_CORE_VOLT_NULL)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_DMSS_VOLT_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_SCENE_ID_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_LATSTAT_SR_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_SC_LOCK)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_SLT_SC_LOCK)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_DDRC_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DDR_DMC_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_DMA_TIMEOUT)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_SUBPMU0_PANIC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_SUBPMU1_PANIC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,HARDWARE_FAULT,PSCI_SUBPMU2_PANIC)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,MODEM_TSENSOR_ERR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_MODEM_MEM_REPAIR)
__LPM3_EXC_SUBTYPE_MAP(LPM3_S_EXCEPTION,LPM3EXCEPTION,PSCI_MODEM_TIME_OUT)
__MEM_REPAIR_EXC_SUBTYPE_MAP(XLOADER_MEMORY_REPAIR, HARDWARE_FAULT, OMR_PASS_REBOOT)
__MEM_REPAIR_EXC_SUBTYPE_MAP(XLOADER_MEMORY_REPAIR, HARDWARE_FAULT, OMR_REPAIR_FAIL)
__MEM_REPAIR_EXC_SUBTYPE_MAP(XLOADER_MEMORY_REPAIR, HARDWARE_FAULT, OMR_RETEST_FAIL)
__SCHARGER_EXC_SUBTYPE_MAP(AP_S_SCHARGER, HARDWARE_FAULT, PMU_VSYS_OV)
__SCHARGER_EXC_SUBTYPE_MAP(AP_S_SCHARGER, HARDWARE_FAULT, PMU_VSYS_PWROFF_ABS)
__SCHARGER_EXC_SUBTYPE_MAP(AP_S_SCHARGER, HARDWARE_FAULT, PMU_VSYS_PWROFF_DEB)
__SCHARGER_EXC_SUBTYPE_MAP(AP_S_SCHARGER, HARDWARE_FAULT, PMU_CALI_VSYSPWROFF_DEB)
__SCHARGER_EXC_SUBTYPE_MAP(AP_S_SCHARGER, HARDWARE_FAULT, PMU_CALI_VSYSPWROFF_ABS)
__SCHARGER_EXC_SUBTYPE_MAP(AP_S_SCHARGER, HARDWARE_FAULT, PMU_CALI_VSYS_OV)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK00)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK01)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK10)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK1)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK2)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK3)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK4)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK5)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO3)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO2)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO1)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO0)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK6)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK7)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK8)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_BUCK9)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO8)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO7)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO6)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO4)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO23)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO22)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO21)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO18)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO17)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO15)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO14)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO32)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO30)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO29)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO28)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO27)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO26)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO25)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO24)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO41)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO40)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO38)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO37)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO36)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_PMUH)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_LDO34)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK00)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK10)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK1)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK2)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK3)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK4)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK5)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK6)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK7)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK8)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_BUCK9)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SCP_LDOBUFF)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, OCP_DISCHARGER)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, SYS_NRST_4S)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, PMUA_SHORT_F)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, PMUH_SHORT_F)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, VIN_LDOH_SHUTDOWN)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, VSYS_PWRONEXP_SHUTDOWN)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, CALI_PMUH_OCP)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, CALI_LDO26_OCP)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, CALI_BUCK3_SCP)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, CALI_BUCK3_OCP)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, CALI_PMUH_SHORT)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, CALI_PMUA_SHORT)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, RAMP_BUCK00)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, RAMP_BUCK10)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, RAMP_BUCK4)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, RAMP_BUCK5)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, RAMP_BUCK6)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, RAMP_LDO0)
__PMU_EXC_SUBTYPE_MAP(AP_S_PMU, HARDWARE_FAULT, RAMP_LDO40)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,AICORE_EXCP)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,AICORE_TIMEOUT)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,TS_RUNNING_EXCP)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,TS_RUNNING_TIMEOUT)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,TS_INIT_EXCP)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,AICPU_INIT_EXCP)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,AICPU_HEARTBEAT_EXCP)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,POWERUP_FAIL)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,POWERDOWN_FAIL)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,NPU_NOC_ERR)
__NPU_EXC_SUBTYPE_MAP(NPU_S_EXCEPTION,NPUEXCEPTION,SMMU_EXCP)
__CONN_EXC_SUBTYPE_MAP(WIFI_S_EXCEPTION,CONNEXCEPTION,CONN_WIFI_EXEC)
__CONN_EXC_SUBTYPE_MAP(WIFI_S_EXCEPTION,CONNEXCEPTION,CONN_WIFI_CHAN_EXEC)
__CONN_EXC_SUBTYPE_MAP(WIFI_S_EXCEPTION,CONNEXCEPTION,CONN_WIFI_WAKEUP_FAIL)
__CONN_EXC_SUBTYPE_MAP(BFGX_S_EXCEPTION,CONNEXCEPTION,CONN_BFGX_EXEC)
__CONN_EXC_SUBTYPE_MAP(BFGX_S_EXCEPTION,CONNEXCEPTION,CONN_BFGX_BEAT_TIMEOUT)
__CONN_EXC_SUBTYPE_MAP(BFGX_S_EXCEPTION,CONNEXCEPTION,CONN_BFGX_WAKEUP_FAIL)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SENSOR_CTRL)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SIC)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_MED_ROM)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_MED_RAM)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_OTPC)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_HARD)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_IPC_MAILBOX)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_MPU)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_BUS)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SSP)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SEC_EXTERN)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_WDG)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SYSALARM)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_NV_COUNTER)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SWP)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_COS)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_BB)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_MNTN_COS)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, NORMALBOOT, EXC_MNTN_COS_RESET)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_LIBC)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_NVM)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SECENG_TRNG)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SECENG_TRIM)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SECENG_SCE)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SECENG_RSA)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SECENG_SM2)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SECENG_KM)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SECENG_SCRAMBLING)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_DCS)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_BOTTOM)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, SIMULATE_EXC_RPMB_KO)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_ALARM0)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_ALARM1)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_AS2AP_IRQ)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_DS2AP_IRQ)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SENC2AP_IRQ)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SENC2AP_IRQ0)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_SENC2AP_IRQ1)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_LOCKUP)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_EH2H_SLV)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_TSENSOR1)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_RST)
__GENERAL_SEE_EXC_SUBTYPE_MAP(GENERAL_SEE_S_EXCEPTION, GENERAL_SEE_CRASH, EXC_UNKNOWN)
__IVP_EXC_SUBTYPE_MAP(IVP_S_EXCEPTION, IVPCRASH, IVP_NOC_ERR)
__IVP_EXC_SUBTYPE_MAP(IVP_S_EXCEPTION, IVPCRASH, IVP_SMMU_ERR)
__DSS_EXC_SUBTYPE_MAP(DSS_S_EXCEPTION, DSSCRASH, DSS_NOC_EXCEPTION)
__DSS_EXC_SUBTYPE_MAP(DSS_S_EXCEPTION, DSSCRASH, DSS_DDRC_EXCEPTION)
__ALOADER_EXC_SUBTYPE_MAP(ALOADER_PANIC, PANIC, ALOADER_PANIC_RESERVED)
__HM_EXC_SUBTYPE_MAP(HM_PANIC, PANIC, HM_PANIC_KERNEL)
__HM_EXC_SUBTYPE_MAP(HM_PANIC, PANIC, HM_PANIC_SYSMGR)
__HM_EXC_SUBTYPE_MAP(HM_PANIC, PANIC, HM_PANIC_INIT)
__HM_EXC_SUBTYPE_MAP(HM_PANIC, PANIC, HM_PANIC_HARDHDOG)
__HM_EXC_SUBTYPE_MAP(HM_PANIC, PANIC, HM_PANIC_LOWHDOG)
__HM_EXC_SUBTYPE_MAP(HM_PANIC, PANIC, HM_PANIC_HIGHDOG)
__HM_EXC_SUBTYPE_MAP(HM_PANIC, PANIC, HM_PANIC_VWATCHDOG)
