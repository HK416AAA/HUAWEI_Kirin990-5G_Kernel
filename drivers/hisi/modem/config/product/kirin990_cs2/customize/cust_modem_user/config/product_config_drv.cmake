# MD5: f433e508666e75bd5693575a38493f14
set(CFG_ENABLE_BUILD_VARS YES) 

set(CFG_FEATURE_UPGRADE_TL YES) 

set(CFG_PRODUCT_NAME "kirin990_cs2") 

set(CFG_PRODUCT_CFG_CHIP_SOLUTION_NAME "Balong") 

set(CFG_CCPU_OS RTOSCK_SMP) 

set(CFG_HCC_VERSION 7.3) 

set(CFG_PLATFORM kirin990_cs2) 

set(CFG_PLATFORM_HISI_BALONG kirin990_cs2) 

set(CFG_CONFIG_VERSION_STUB NO) 

set(CFG_HW_VERSION_STUB 0x36F5FC42) 

set(CFG_BSP_CONFIG_EDA NO) 

if(${CFG_EMU_TYPE_ESL} MATCHES "FEATURE_ON")
set(CFG_CONFIG_UART_ESL YES) 

set(CFG_FEATURE_SVLSOCKET NO) 

set(CFG_BURN_ESL YES) 

set(CFG_BSP_CONFIG_EMU_MCORE_DTB NO) 

set(CFG_CONFIG_NRBBP YES) 

set(CFG_CONFIG_BALONG_L2CACHE NO) 

set(CFG_CONFIG_BALONG_MODEM_RESET_CTRL NO) 

else()
set(CFG_CONFIG_BALONG_L2CACHE YES) 

set(CFG_EMU_EDMA_LOAD_HAC NO) 

set(CFG_MINI_XLOADER NO) 

set(CFG_MINI_HIBOOT NO) 

set(CFG_BSP_CONFIG_EMU_NO_USB NO) 

set(CFG_CONFIG_BBP YES) 

set(CFG_CONFIG_NRBBP YES) 

set(CFG_BSP_CONFIG_EMU_MCORE_DTB NO) 

set(CFG_FEATURE_CPU_PRIVATE_SLICE YES) 

set(CFG_FEATURE_CPU_PRIVATE_SLICE_HRT YES) 

endif()
set(CFG_BSP_CONFIG_EMU NO) 

set(CFG_BSP_CONFIG_HI3650 YES) 

set(CFG_BSP_CONFIG_PHONE_TYPE YES) 

set(CFG_BSP_CONFIG_HI3660 YES) 

set(CFG_BSP_CONFIG_BOARD_ASIC YES) 

set(CFG_FULLSTACK_NR_LLRAM_ADDR DDR_FULLSTACK_MEM_ADDR) 

set(CFG_FULLSTACK_HAC_LLRAM_ADDR (FULLSTACK_NR_LLRAM_ADDR  +  NRCCPU_LLRAM_BASE_SIZE)) 

set(CFG_FULLSTACK_WFI_ADDR (FULLSTACK_HAC_LLRAM_ADDR  +  HAC_LLRAM_SIZE)) 

set(CFG_FULLSTACK_WFI_SIZE 0x10000) 

set(CFG_BSP_CONFIG_BOARD_SFT NO) 

set(CFG_BSP_HAS_SEC_FEATURE NO) 

set(CFG_CONFIG_MALLOC_UNIFIED YES) 

if(${CFG_EMU_TYPE_EMU} MATCHES "FEATURE_ON")
set(CFG_CONFIG_EICC_PHY_EMU YES) 

endif()
set(CFG_CONFIG_SMART_SYSTEM_MODEM NO) 

set(CFG_CONFIG_HIBOOT_DEBUG NO) 

set(CFG_CONFIG_OF YES) 

set(CFG_FEATURE_BALONG_MODEM_ATE NO) 

set(CFG_TLPHY_IMAGE_LOAD_ADDR_ATE 0x87000000) 

set(CFG_TLPHY_IMAGE_LOAD_SIZE_ATE 0x400000) 

set(CFG_CONFIG_RINGBUFFER_STUB NO) 

set(CFG_CONFIG_CONSOLE_STUB NO) 

set(CFG_CCPU_CORE_NUM 2) 

set(CFG_CCPU_LLRAM_BASE_ADDR 0xE0800000) 

set(CFG_CCPU_LLRAM_BASE_SIZE 0x30000) 

set(CFG_CCPU_SRAM_SIZE 0x3000) 

set(CFG_LL2_LLRAM_BASE_ADDR (CCPU_LLRAM_BASE_ADDR  +  CCPU_SRAM_SIZE)) 

set(CFG_LL2_LLRAM_SIZE 0x28000) 

set(CFG_CONFIG_CCPU_HAS_LLRAM YES) 

set(CFG_CCPU_LLRAM_ADDR (LL2_LLRAM_BASE_ADDR  +  LL2_LLRAM_SIZE)) 

set(CFG_CCPU_LLRAM_SIZE (CCPU_LLRAM_BASE_SIZE - CCPU_SRAM_SIZE - LL2_LLRAM_SIZE)) 

set(CFG_HI_SRAM_MEM_ADDR CCPU_LLRAM_BASE_ADDR) 

set(CFG_HI_SRAM_SIZE CCPU_SRAM_SIZE) 

set(CFG_DRV_SRAM_ADDR (HI_SRAM_MEM_ADDR)) 

set(CFG_DRV_SRAM_SIZE 0x2000) 

set(CFG_CPHY_SRAM_ADDR ((DRV_SRAM_ADDR)  +  (DRV_SRAM_SIZE))) 

set(CFG_CPHY_SRAM_SIZE 0x800) 

set(CFG_CPHY_LPC_SRAM_ADDR ( CPHY_SRAM_ADDR )) 

set(CFG_CPHY_LPC_SRAM_SIZE 0x40) 

set(CFG_CPHY_1X_DATA_MBX_SRAM_ADDR ( (CPHY_LPC_SRAM_ADDR)  +  (CPHY_LPC_SRAM_SIZE) )) 

set(CFG_CPHY_1X_DATA_MBX_SRAM_SIZE 0x40) 

set(CFG_CPHY_HRPD_DATA_MBX_SRAM_ADDR ( (CPHY_1X_DATA_MBX_SRAM_ADDR)  +  (CPHY_1X_DATA_MBX_SRAM_SIZE) )) 

set(CFG_CPHY_HRPD_DATA_MBX_SRAM_SIZE 0x40) 

set(CFG_GPHY_SRAM_ADDR ((CPHY_SRAM_ADDR)  +  (CPHY_SRAM_SIZE))) 

set(CFG_GPHY_SRAM_SIZE 0x40) 

set(CFG_CONFIG_CCPU_HAS_TCM YES) 

set(CFG_CCPU_ITCM_ADDR 0x0) 

set(CFG_CCPU_ITCM_SIZE 0x8000) 

set(CFG_CCPU_ITCM_SIZE_CFG (0x6u<<0x2)) 

set(CFG_CCPU_DTCM_ADDR (CCPU_ITCM_ADDR  +  CCPU_ITCM_SIZE)) 

set(CFG_CCPU_DTCM_SIZE 0x4000) 

set(CFG_CCPU_DTCM_SIZE_CFG (0x5u<<0x2)) 

set(CFG_CONFIG_HAS_HAC YES) 

set(CFG_HAC_OS RTOSCK_SMP) 

set(CFG_HAC_COMPILER HCC) 

set(CFG_CONFIG_HAC_ARCH "Cortex-R8") 

set(CFG_HAC_CORE_NUM 2) 

set(CFG_HAC_TEXT_START_ADDR HAC_LLRAM_ADDR) 

set(CFG_HAC_LLRAM_ADDR 0xF4C00000) 

set(CFG_HAC_LLRAM_SIZE 0x180000) 

set(CFG_HAC_LLRAM_UNCACHED_SIZE 0xB0000) 

set(CFG_HAC_LLRAM_CACHED_SIZE (HAC_LLRAM_SIZE - HAC_LLRAM_UNCACHED_SIZE)) 

set(CFG_CONFIG_HAC_HAS_TCM YES) 

set(CFG_HAC_ITCM_SIZE 0x8000) 

set(CFG_HAC_ITCM_SIZE_CFG (0x6u<<0x2)) 

set(CFG_HAC_DTCM_SIZE 0x20000) 

set(CFG_HAC_DTCM_SIZE_CFG (0x8u<<0x2)) 

set(CFG_HAC_TCM_START_ADDR 0xFE000000) 

set(CFG_HAC_ITCM0_ADDR HAC_TCM_START_ADDR) 

set(CFG_HAC_ITCM1_ADDR (HAC_ITCM0_ADDR  +  HAC_ITCM_SIZE)) 

set(CFG_HAC_DTCM_ADDR 0xFE020000) 

set(CFG_HAC_DTCM0_ADDR HAC_DTCM_ADDR) 

set(CFG_HAC_DTCM1_ADDR HAC_DTCM_ADDR) 

set(CFG_HAC_THUMB_OPTIMIZE YES) 

set(CFG_NRCCPU_TEMP_CACHE_ADDR 0x1F000000) 

set(CFG_NRCCPU_TEMP_CACHE_SIZE 0x00200000) 

set(CFG_NRCCPU_TEMP_UNCACHE_ADDR (NRCCPU_TEMP_CACHE_ADDR + NRCCPU_TEMP_CACHE_SIZE)) 

set(CFG_NRCCPU_TEMP_UNCACHE_SIZE 0x00200000) 

set(CFG_CONFIG_HAS_NRCCPU YES) 

set(CFG_NRCCPU_OS RTOSCK_SMP) 

set(CFG_NRCCPU_COMPILER HCC) 

set(CFG_CONFIG_NRCCPU_ARCH "Cortex-R8") 

set(CFG_NRCCPU_CORE_NUM 2) 

set(CFG_CONFIG_NRCCPU_HAS_LLRAM YES) 

set(CFG_NRCCPU_LLRAM_BASE_ADDR 0xF5400000) 

set(CFG_NRCCPU_LLRAM_BASE_SIZE 0x000A0000) 

set(CFG_NRCCPU_SRAM_SIZE 0x2800) 

set(CFG_NRCCPU_LLRAM_ADDR (NRCCPU_LLRAM_BASE_ADDR  +  NRCCPU_SRAM_SIZE)) 

set(CFG_NRCCPU_LLRAM_SIZE (NRCCPU_LLRAM_BASE_SIZE - NRCCPU_SRAM_SIZE)) 

set(CFG_CONFIG_NRCCPU_HAS_TCM YES) 

set(CFG_NRCCPU_ITCM_ADDR 0x0) 

set(CFG_NRCCPU_ITCM_SIZE 0x4000) 

set(CFG_NRCCPU_ITCM_SIZE_CFG (0x5u<<0x2)) 

set(CFG_NRCCPU_DTCM_ADDR 0x10000) 

set(CFG_NRCCPU_DTCM_SIZE 0x10000) 

set(CFG_NRCCPU_DTCM_SIZE_CFG (0x7u<<0x2)) 

set(CFG_MPU_ASLR_RESERVE 0x300000) 

set(CFG_MPU_ALGN_RESERVE 0x100000) 

set(CFG_DDR_MCORE_RELOC_SIZE 0x10000) 

set(CFG_CONFIG_NVIM YES) 

set(CFG_CONFIG_NR_NVIM YES) 

set(CFG_FEATURE_NV_FLASH_ON NO) 

set(CFG_FEATURE_NV_EMMC_ON YES) 

set(CFG_FEATURE_NV_LFILE_ON NO) 

set(CFG_FEATURE_NV_RFILE_ON NO) 

set(CFG_FEATURE_NV_NO_MODEMNVM_SYS YES) 

set(CFG_FEATURE_NV_SEC_ON YES) 

set(CFG_FEATURE_DIS_HMAC_CHECK NO) 

set(CFG_EFUSE_HUK_BIT_START 1679) 

set(CFG_FEATURE_TLPHY_MAILBOX YES) 

set(CFG_CONFIG_MAILBOX_TYPE YES) 

set(CFG_CONFIG_HIFI_MAILBOX NO) 

set(CFG_FEATURE_CPHY_MAILBOX NO) 

set(CFG_ENABLE_BUILD_OM YES) 

set(CFG_CONFIG_DUMP_LOG_ESCAPE_FIQ YES) 

set(CFG_FEATURE_OM_PHONE NO) 

set(CFG_ENABLE_BUILD_SYSVIEW NO) 

set(CFG_ENABLE_BUILD_CPUVIEW NO) 

set(CFG_ENABLE_BUILD_MEMVIEW NO) 

set(CFG_ENABLE_AMON_SOC NO) 

set(CFG_ENABLE_AMON_MDM YES) 

set(CFG_ENABLE_BUILD_UTRACE NO) 

set(CFG_ENABLE_BUILD_HAC_DUMP YES) 

set(CFG_ENABLE_BUILD_NRRDR YES) 

set(CFG_ENABLE_BUILD_SOCP YES) 

set(CFG_CONFIG_DIAG_SYSTEM YES) 

set(CFG_CONFIG_DIAG_NETLINK NO) 

set(CFG_SOCP_V300 YES) 

set(CFG_ENABLE_BUILD_PRINT YES) 

set(CFG_CONFIG_DIAG_NRM YES) 

set(CFG_CONFIG_FILEBROSWER NO) 

set(CFG_CONFIG_HIMS_NV_PROC YES) 

set(CFG_FEATURE_SOCP_ADDR_64BITS YES) 

set(CFG_DIAG_SYSTEM_5G YES) 

set(CFG_CONFIG_DEFLATE NO) 

set(CFG_CONFIG_APPLOG NO) 

set(CFG_CONFIG_DIAG_FRAME YES) 

set(CFG_FEATURE_HDS_PRINTLOG FEATURE_ON) 

set(CFG_FEATURE_HDS_TRANSLOG FEATURE_ON) 

set(CFG_FEATURE_SRE_PRINT_SLICE FEATURE_ON) 

set(CFG_FEATURE_SRE_PRINT_RTC FEATURE_OFF) 

set(CFG_FEATURE_NOC_PARSE_ES NO) 

set(CFG_CONFIG_MID YES) 

set(CFG_CONFIG_NOC YES) 

set(CFG_CONFIG_NOC_AP NO) 

set(CFG_CONFIG_MODEM_FULL_DUMP NO) 

set(CFG_CONFIG_MODEM_MINI_DUMP YES) 

set(CFG_CONFIG_MODEM_DUMP_BLACKLIST YES) 

set(CFG_DIAG_VERSION_ENG NO) 

set(CFG_CONFIG_BUS_ERR_NR YES) 

set(CFG_CONFIG_BUS_ERR_LR YES) 

set(CFG_CONFIG_BUS_ERR_AP NO) 

set(CFG_CONFIG_PDLOCK NO) 

set(CFG_CONFIG_PDLOCK_RENEW YES) 

set(CFG_CONFIG_PDLOCK_AP NO) 

set(CFG_CONFIG_M3_APB_SPI NO) 

set(CFG_FEATURE_SAMPLE_LTE_CHAN 			 FEATURE_OFF) 

set(CFG_FEATURE_SCI_PROTOL_T1 FEATURE_OFF) 

set(CFG_FEATURE_SCI_ESIM FEATURE_OFF) 

set(CFG_CONFIG_SC NO) 

set(CFG_CONFIG_DSP NO) 

set(CFG_CONFIG_NRDSP YES) 

set(CFG_CONFIG_NRDSP_FULL_DUMP NO) 

set(CFG_CONFIG_NRDSP_MINI_DUMP YES) 

set(CFG_CONFIG_CBBE YES) 

set(CFG_MODEM_MEM_REPAIR YES) 

set(CFG_CONFIG_WATCHPOINT NO) 

set(CFG_CONFIG_MODEM_SOCP_3_0 YES) 

set(CFG_CONFIG_MODEM_DMSS_3_0 YES) 

set(CFG_CONFIG_DSPDVS YES) 

set(CFG_FEATURE_DSP_DFS FEATURE_ON) 

set(CFG_CONFIG_DSP_STRESS NO) 

set(CFG_FEATURE_GUBBP_HANDSHAKE FEATURE_ON) 

set(CFG_FEATURE_GUDRX_NEWVERSION FEATURE_ON) 

set(CFG_FEATURE_BOSTON_ONLY_FEATURE FEATURE_OFF) 

set(CFG_FEATURE_DSP_PM_SEPARATE_MODE FEATURE_ON) 

set(CFG_FEATURE_CSDR_COMBINE FEATURE_ON) 

set(CFG_FEATURE_GUC_TRANSPLANT FEATURE_OFF) 

set(CFG_CONFIG_DFS_DDR NO) 

set(CFG_CONFIG_DFS_NOC NO) 

set(CFG_SUPPORT_PARA_CFG NO) 

set(CFG_FEATURE_TCM_RETENTION FEATURE_ON) 

set(CFG_FEATURE_TCM_PART_RETENTION FEATURE_OFF) 

set(CFG_CONFIG_NRCPM YES) 

set(CFG_CONFIG_NEWVER_RFIC YES) 

set(CFG_CONFIG_NR_NEWMAILBOX YES) 

set(CFG_CONFIG_BBIC_LOWPOWER YES) 

set(CFG_CONFIG_NR_LTE_INTEROPER YES) 

set(CFG_CONFIG_LIGHTSLEEP_POWERDOWN YES) 

set(CFG_CONFIG_DDRC_AUTOGT_BYPASS YES) 

set(CFG_FEATURE_MULTI_CHANNEL			 FEATURE_OFF) 

set(CFG_FEATURE_CHR_OM FEATURE_OFF) 

set(CFG_FEATURE_CHR_NR YES) 

set(CFG_CONFIG_GPIO_PL061 YES) 

set(CFG_CONFIG_TRNG_SEED YES) 

set(CFG_FEATURE_ANTEN_DETECT YES) 

set(CFG_CONFIG_ANTEN YES) 

set(CFG_CONFIG_ONOFF YES) 

set(CFG_CONFIG_MLOADER_COLD_PATCH YES) 

set(CFG_CONFIG_LOAD_SEC_IMAGE YES) 

set(CFG_CONFIG_COMPRESS_CCORE_IMAGE YES) 

set(CFG_CONFIG_COMPRESS_DTB_IMAGE YES) 

set(CFG_CONFIG_MODEM_DTB_LOAD_IN_KERNEL YES) 

set(CFG_CONFIG_MODEM_BALONG_ASLR YES) 

set(CFG_CONFIG_MODEM_BALONG_ASLR_CACHE_ON YES) 

set(CFG_CONFIG_MODEM_ASLR_DEBUG NO) 

set(CFG_CONFIG_MLOADER YES) 

set(CFG_FEATURE_DELAY_MODEM_INIT FEATURE_ON) 

set(CFG_CONFIG_MODEM_PINTRL YES) 

set(CFG_CONFIG_ADC YES) 

set(CFG_CONFIG_EFUSE YES) 

set(CFG_CONFIG_EFUSE_NR YES) 

set(CFG_CONFIG_HW_SPINLOCK YES) 

set(CFG_CONFIG_RTC_ON_SOC YES) 

if(${CFG_MODEM_SANITIZER} MATCHES "FEATURE_ON")
set(CFG_STACK_CANARY_COMPILE NO) 

else()
set(CFG_STACK_CANARY_COMPILE YES) 

endif()
set(CFG_CONFIG_RFILE_ON YES) 

set(CFG_FEATURE_THERMAL YES) 

set(CFG_MODEM_FW_PARTITION_SIZE 138936320) 

set(CFG_CONFIG_CCPU_FIQ_SMP YES) 

set(CFG_OS_K3V3_USE_LPM3_API NO) 

set(CFG_BSP_ICC_MCHANNEL_USE_LPM3TCM YES) 

set(CFG_CONFIG_HIBOOT_UART_NUM 0) 

set(CFG_MCORE_MPU_ENABLE YES) 

set(CFG_CONFIG_MODULE_VIC NO) 

set(CFG_CONFIG_AT_UART NO) 

set(CFG_CONFIG_MEM_DEBUG NO) 

set(CFG_CONFIG_IS_DTB_VERIFY YES) 

set(dts true) 

set(CFG_DTS_STATIC_MEM_SIZE 0X5000) 

set(CFG_CONFIG_AVS YES) 

set(CFG_CONFIG_CPUBUS_MAXFREQ YES) 

set(CFG_CONFIG_CPUFREQ YES) 

set(CFG_CPUFREQ_DEBUG_INTERFACE NO) 

set(CFG_CONFIG_BUSFREQ YES) 

set(CFG_CONFIG_IDLEFREQ YES) 

set(CFG_CONFIG_CCORE_REGULATOR YES) 

set(CFG_CONFIG_CCORE_WDT YES) 

set(CFG_CONFIG_ACORE_WDT NO) 

set(CFG_CONFIG_WDT_HAC_BUILD YES) 

set(CFG_CONFIG_WDT_NR_BUILD YES) 

set(CFG_CONFIG_PMCTRL YES) 

set(CFG_CONFIG_PMIC_FPGA NO) 

set(CFG_CONFIG_NRCCPU_PMU YES) 

set(CFG_CONFIG_PHONE_DCXO_AP YES) 

set(CFG_CONFIG_PMIC_SPMI YES) 

set(CFG_CONFIG_PMU_NEW YES) 

set(CFG_CONFIG_PMIC_SPMI_PROTOCOL YES) 

set(CFG_CONFIG_PMU_XLOADER YES) 

set(CFG_CONFIG_NR_DCXO_NV YES) 

set(CFG_CONFIG_PMU_RAMP_EN YES) 

if(${CFG_EMU_TYPE_ESL} MATCHES "FEATURE_ON")
set(CFG_CONFIG_PMU NO) 

else()
set(CFG_CONFIG_PMU YES) 

endif()
set(CFG_CONFIG_CCORE_NOTIFIER YES) 

set(CFG_CONFIG_NRCCPU_PM YES) 

set(CFG_CONFIG_NRCCPU_PM_DEBUG NO) 

set(CFG_CONFIG_LRCCPU_PM_DEBUG NO) 

set(CFG_CONFIG_BALONG_CCLK YES) 

set(CFG_CONFIG_BALONG_CCLK_DEBUG NO) 

set(CFG_CONFIG_BALONG_CCLK_ATUOGATE YES) 

set(CFG_CONFIG_MODULE_HAC_PM YES) 

set(CFG_CONFIG_CCORE_CPU_IDLE YES) 

set(CFG_CONFIG_HAC_CPU_IDLE YES) 

set(CFG_CONFIG_CCORE_PM YES) 

set(CFG_CONFIG_MODULE_IPC YES) 

set(CFG_CONFIG_IPCM_USE_FPGA_VIC NO) 

set(CFG_CONFIG_MODULE_TIMER YES) 

set(CFG_CONFIG_MODULE_HAC_SLICE YES) 

set(CFG_CONFIG_CCORE_CPU YES) 

set(CFG_CONFIG_HAS_CCORE_WAKELOCK YES) 

set(CFG_CONFIG_HAS_HAC_WAKELOCK YES) 

set(CFG_CONFIG_CCORE_BALONG_PM YES) 

set(CFG_CONFIG_BALONG_EDMA YES) 

set(CFG_CONFIG_EDMA_DEBUG NO) 

set(CFG_CONFIG_PWC_MNTN_CCORE NO) 

set(CFG_CONFIG_HWADP YES) 

set(CFG_CONFIG_SYSCTRL YES) 

set(CFG_CONFIG_SYSBUS NO) 

set(CFG_CONFIG_SYSBUS_NRCCPU NO) 

set(CFG_CONFIG_SYSBUS_HAC NO) 

set(CFG_CONFIG_BALONG_HPM_TEMP NO) 

set(CFG_CONFIG_MEM YES) 

set(CFG_CONFIG_TCXO_BALONG NO) 

set(CFG_CONFIG_BALONG_DPM NO) 

set(CFG_CONFIG_USE_TIMER_STAMP NO) 

set(CFG_CONFIG_BALONG_MODEM_RESET YES) 

set(CFG_CONFIG_MODULE_BUSSTRESS NO) 

set(CFG_CONFIG_RSR_ACC YES) 

set(CFG_CONFIG_RSRACC_DEBUG NO) 

set(CFG_CONFIG_CCPU_HOTPLUG NO) 

set(CFG_CONFIG_MPERF NO) 

set(CFG_CONFIG_PERF_STAT_1 NO) 

set(CFG_CONFIG_PERF_STAT_DEBUG NO) 

set(CFG_CONFIG_AXIMEM_BALONG YES) 

set(CFG_CONFIG_AXIMEM_CLK_RST YES) 

set(CFG_CONFIG_NXDEP_MPU YES) 

set(CFG_CONFIG_NXDEP_MPU_NEW YES) 

set(CFG_CONFIG_PM_OM YES) 

set(CFG_CONFIG_PM_OM_NR YES) 

set(CFG_CONFIG_IPF NO) 

set(CFG_CONFIG_PSAM NO) 

set(CFG_CONFIG_CIPHER NO) 

set(CFG_CONFIG_EIPF YES) 

set(CFG_CONFIG_WAN	 YES) 

set(CFG_IPF_UL_DMA YES) 

set(CFG_CONFIG_NEW_PLATFORM YES) 

set(CFG_CONFIG_CIPHER_NEW YES) 

set(CFG_CONFIG_CIPHER_EN_DC YES) 

set(CFG_CONFIG_ECIPHER YES) 

set(CFG_CONFIG_MAA_BALONG YES) 

set(CFG_CONFIG_MAA_LR YES) 

set(CFG_CONFIG_MAA_NR YES) 

set(CFG_CONFIG_ESPE YES) 

set(CFG_CONFIG_BALONG_ESPE YES) 

set(CFG_CONFIG_ESPE_PHONE_SOC YES) 

set(CFG_CONFIG_MAA_LL1C YES) 

set(CFG_CONFIG_MAA_1V5 YES) 

set(CFG_MAA_ASIC YES) 

set(CFG_CONFIG_IPF_VESION 2) 

set(CFG_CONFIG_IPF_ADQ_LEN 5) 

set(CFG_CONFIG_IPF_PROPERTY_MBB NO) 

set(CFG_CONFIG_USB_DWC3_VBUS_DISCONNECT NO) 

set(CFG_USB3_SYNOPSYS_PHY NO) 

set(CFG_CONFIG_USB_FORCE_HIGHSPEED NO) 

set(CFG_FEATURE_HISOCKET FEATURE_OFF) 

set(CFG_CONFIG_BALONG_TRANS_REPORT YES) 

set(CFG_CONFIG_HMI YES) 

set(CFG_CONFIG_HMI_ICC YES) 

set(CFG_CONFIG_HMI_DEBUG YES) 

set(CFG_ENABLE_TEST_CODE NO) 

set(CFG_CONFIG_LLT_MDRV NO) 

set(CFG_CONFIG_ECDC NO) 

set(CFG_CONFIG_ICC YES) 

set(CFG_CONFIG_NRICC YES) 

set(CFG_CONFIG_ICC_DEBUG NO) 

set(CFG_CONFIG_SEC_ICC YES) 

set(CFG_CONFIG_IPC_MSG YES) 

set(CFG_CONFIG_IPC_MSG_AO_DISC YES) 

set(CFG_CONFIG_EICC_V110 YES) 

set(CFG_CONFIG_EICC_DEBUG YES) 

set(CFG_CONFIG_CSHELL NO) 

set(CFG_CONFIG_NR_CSHELL NO) 

set(CFG_CONFIG_UART_SHELL NO) 

set(CFG_CONFIG_OS_INCLUDE_SHELL NO) 

set(CFG_CONFIG_SHELL_SYMBOL_REG NO) 

set(CFG_CONFIG_DYNMEM_REPORT NO) 

set(CFG_HI_NRSRAM_MEM_ADDR NRCCPU_LLRAM_BASE_ADDR) 

set(CFG_HI_NRSRAM_SIZE                		 NRCCPU_SRAM_SIZE) 

set(CFG_DRV_NRSRAM_ADDR               		 (HI_NRSRAM_MEM_ADDR)) 

set(CFG_DRV_NRSRAM_SIZE						 0x2000) 

set(CFG_LPMCU_DRAM_WINDOW 0x20000000) 

set(CFG_DDR_MEM_ADDR       		 0xA0000000) 

set(CFG_DDR_MEM_SIZE       		 0x15700000) 

set(CFG_DDR_APP_ACP_ADDR 0) 

set(CFG_DDR_APP_ACP_SIZE 0) 

set(CFG_DDR_MDM_ACP_ADDR 0) 

set(CFG_DDR_MDM_ACP_SIZE 0) 

set(CFG_DDR_MCORE_SIZE 0x8900000) 

set(CFG_DDR_MCORE_NR_SIZE 0x3100000) 

set(CFG_DDR_MCORE_DTS_SIZE 0x180000) 

set(CFG_DDR_GU_SIZE 0x40000) 

set(CFG_DDR_TLPHY_IMAGE_SIZE 0x800000) 

set(CFG_DDR_NRPHY_IMAGE_SIZE 0x800000) 

set(CFG_DDR_LPHY_SDR_SIZE 0xC80000) 

set(CFG_DDR_LCS_SIZE 0x280000) 

set(CFG_DDR_SDR_NR_SIZE 0x1400000) 

set(CFG_DDR_CBBE_IMAGE_SIZE 0x0) 

set(CFG_DDR_HARQ_UP_SIZE 0xC0000) 

set(CFG_DDR_HARQ_NRUL_SIZE 0x180000) 

set(CFG_DDR_RFIC_SUB6G_IMAGE_SIZE 0x138000) 

set(CFG_DDR_RFIC_HF_IMAGE_SIZE 0x0c8000) 

set(CFG_DDR_RFIC_DUMP_SIZE 0x100000) 

set(CFG_DDR_SEC_SHARED_SIZE 0x100000) 

set(CFG_DDR_PDE_IMAGE_SIZE 0x300000) 

set(CFG_DDR_MAA_MDM_SIZE 0x1A00000) 

set(CFG_DDR_FULLSTACK_MEM_SIZE 0x500000) 

set(CFG_DDR_LPMCU_IMAGE_SIZE 0x40000) 

set(CFG_DDR_EMU_HAC_LOAD_SIZE 0x180000) 

set(CFG_DDR_MTD_MEM_SIZE 0x0) 

set(CFG_DDR_MCORE_UNCACHE_SIZE 0x2A00000) 

set(CFG_DDR_MCORE_NR_UNCACHE_SIZE 0x00800000) 

set(CFG_DDR_MCORE_ADDR $(CFG_DDR_MEM_ADDR)) 

set(CFG_DDR_MCORE_NR_ADDR $(CFG_DDR_MCORE_ADDR) + $(CFG_DDR_MCORE_SIZE)) 

set(CFG_DDR_MCORE_DTS_ADDR $(CFG_DDR_MCORE_NR_ADDR) + $(CFG_DDR_MCORE_NR_SIZE)) 

set(CFG_DDR_GU_ADDR $(CFG_DDR_MCORE_DTS_ADDR) + $(CFG_DDR_MCORE_DTS_SIZE)) 

set(CFG_DDR_TLPHY_IMAGE_ADDR $(CFG_DDR_GU_ADDR) + $(CFG_DDR_GU_SIZE)) 

set(CFG_DDR_NRPHY_IMAGE_ADDR $(CFG_DDR_TLPHY_IMAGE_ADDR) + $(CFG_DDR_TLPHY_IMAGE_SIZE)) 

set(CFG_DDR_LPHY_SDR_ADDR $(CFG_DDR_NRPHY_IMAGE_ADDR) + $(CFG_DDR_NRPHY_IMAGE_SIZE)) 

set(CFG_DDR_LCS_ADDR $(CFG_DDR_LPHY_SDR_ADDR) + $(CFG_DDR_LPHY_SDR_SIZE)) 

set(CFG_DDR_SDR_NR_ADDR $(CFG_DDR_LCS_ADDR) + $(CFG_DDR_LCS_SIZE)) 

set(CFG_DDR_CBBE_IMAGE_ADDR $(CFG_DDR_SDR_NR_ADDR) + $(CFG_DDR_SDR_NR_SIZE)) 

set(CFG_DDR_HARQ_UP_ADDR $(CFG_DDR_CBBE_IMAGE_ADDR) + $(CFG_DDR_CBBE_IMAGE_SIZE)) 

set(CFG_DDR_HARQ_NRUL_ADDR $(CFG_DDR_HARQ_UP_ADDR) + $(CFG_DDR_HARQ_UP_SIZE)) 

set(CFG_DDR_RFIC_SUB6G_IMAGE_ADDR $(CFG_DDR_HARQ_NRUL_ADDR) + $(CFG_DDR_HARQ_NRUL_SIZE)) 

set(CFG_DDR_RFIC_HF_IMAGE_ADDR $(CFG_DDR_RFIC_SUB6G_IMAGE_ADDR) + $(CFG_DDR_RFIC_SUB6G_IMAGE_SIZE)) 

set(CFG_DDR_RFIC_DUMP_ADDR $(CFG_DDR_RFIC_HF_IMAGE_ADDR) + $(CFG_DDR_RFIC_HF_IMAGE_SIZE)) 

set(CFG_DDR_SEC_SHARED_ADDR $(CFG_DDR_RFIC_DUMP_ADDR) + $(CFG_DDR_RFIC_DUMP_SIZE)) 

set(CFG_DDR_PDE_IMAGE_ADDR $(CFG_DDR_SEC_SHARED_ADDR) + $(CFG_DDR_SEC_SHARED_SIZE)) 

set(CFG_DDR_MAA_MDM_ADDR $(CFG_DDR_PDE_IMAGE_ADDR) + $(CFG_DDR_PDE_IMAGE_SIZE)) 

set(CFG_DDR_EMU_HAC_LOAD_ADDR $(CFG_DDR_MAA_MDM_ADDR) + $(CFG_DDR_MAA_MDM_SIZE)) 

set(CFG_DDR_MTD_MEM_ADDR $(CFG_DDR_EMU_HAC_LOAD_ADDR) + $(CFG_DDR_EMU_HAC_LOAD_SIZE)) 

set(CFG_DDR_FULLSTACK_MEM_ADDR $(CFG_DDR_MTD_MEM_ADDR) + $(CFG_DDR_MTD_MEM_SIZE)) 

set(CFG_DDR_LPMCU_IMAGE_ADDR $(CFG_DDR_FULLSTACK_MEM_ADDR) + $(CFG_DDR_FULLSTACK_MEM_SIZE)) 

set(CFG_DDR_LRCCPU_DTS_SIZE 0x60000) 

set(CFG_DDR_LPMCU_DTS_SIZE 0x60000) 

set(CFG_DDR_NRCCPU_DTS_SIZE 0x60000) 

set(CFG_DDR_L2CPU_DTS_SIZE 0x60000) 

set(CFG_DDR_MDTS_TOTAL_SIZE $(CFG_DDR_LRCCPU_DTS_SIZE)  +  $(CFG_DDR_LPMCU_DTS_SIZE)  +  $(CFG_DDR_NRCCPU_DTS_SIZE)  +  $(CFG_DDR_L2CPU_DTS_SIZE)) 

set(CFG_DDR_LRCCPU_DTS_ADDR $(CFG_DDR_MCORE_DTS_ADDR)) 

set(CFG_DDR_LPMCU_DTS_ADDR $(CFG_DDR_LRCCPU_DTS_ADDR)  +  $(CFG_DDR_LRCCPU_DTS_SIZE)) 

set(CFG_DDR_NRCCPU_DTS_ADDR $(CFG_DDR_LPMCU_DTS_ADDR)  +  $(CFG_DDR_LPMCU_DTS_SIZE)) 

set(CFG_DDR_L2CPU_DTS_ADDR $(CFG_DDR_NRCCPU_DTS_ADDR)  +  $(CFG_DDR_NRCCPU_DTS_SIZE)) 

set(CFG_MEM_ADJUST_INTERCEPT YES) 

set(CFG_MCORE_TEXT_START_ADDR $(CFG_DDR_MCORE_ADDR)) 

set(CFG_HIBOOT_DDR_ENTRY $(CFG_DDR_HIBOOT_ADDR)) 

set(CFG_PRODUCT_CFG_KERNEL_ENTRY $(CFG_DDR_ACORE_ADDR) + 0x80000-0x8000) 

set(CFG_PRODUCT_KERNEL_PARAMS_PHYS $(CFG_DDR_ACORE_ADDR) + 0x100) 

set(CFG_ONCHIP_HIBOOT_ADDR $(CFG_MCORE_TEXT_START_ADDR) + 0x100000-0x1000) 

set(CFG_DDR_MCORE_UNCACHE_ADDR ( (DDR_MCORE_ADDR   )  +  (DDR_MCORE_SIZE) - (DDR_MCORE_UNCACHE_SIZE))) 

set(CFG_DDR_MCORE_NR_UNCACHE_ADDR ( (DDR_MCORE_NR_ADDR   )  +  (DDR_MCORE_NR_SIZE) - (DDR_MCORE_NR_UNCACHE_SIZE))) 

set(CFG_MODEM_SANITIZER_ADDR_OFFSET 0) 

set(CFG_MODEM_SANITIZER_NR_ADDR_OFFSET 0) 

set(CFG_MDM_SANTIZIER_MEM_SIZE 0) 

set(CFG_MDM_SANTIZIER_NR_MEM_SIZE 0) 

set(CFG_MDM_SANTIZIER_MEM_ADDR 0) 

set(CFG_MDM_SANTIZIER_NR_MEM_ADDR 0) 

set(CFG_DDR_UPA_ADDR DDR_GU_ADDR) 

set(CFG_DDR_UPA_SIZE 0x00024000) 

set(CFG_DDR_CQI_ADDR ((DDR_UPA_ADDR)  +  (DDR_UPA_SIZE))) 

set(CFG_DDR_CQI_SIZE 0x00003400) 

set(CFG_DDR_ZSP_UP_ADDR ((DDR_CQI_ADDR)  +  (DDR_CQI_SIZE))) 

set(CFG_DDR_ZSP_UP_SIZE 0x00008000) 

set(CFG_DDR_ZSP_UP_1_ADDR ((DDR_ZSP_UP_ADDR)  +  (DDR_ZSP_UP_SIZE))) 

set(CFG_DDR_ZSP_UP_1_SIZE 0x00008000) 

set(CFG_SHM_SEC_SIZE          		 (DDR_SEC_SHARED_SIZE)) 

set(CFG_SHM_SIZE_PROTECT_BARRIER (0x1000)) 

set(CFG_SHM_SIZE_PARAM_CFG (0x4000)) 

set(CFG_SHM_SIZE_SEC_ICC (0x20000)) 

set(CFG_SHM_SIZE_SEC_RESERVED (0xb000)) 

set(CFG_SHM_SIZE_SEC_MDMA9_PM_BOOT (0x2000)) 

set(CFG_SHM_SIZE_SEC_CERT			 (0)) 

set(CFG_SHM_SIZE_SEC_DUMP			 (0x24000)) 

set(CFG_SHM_SIZE_SEC_MALLOC (0x80000)) 

set(CFG_MODEM_SHARED_DDR_BASE (0x1E000000)) 

set(CFG_MODEM_NSRO_SHARED_MEM_BASE (MODEM_SHARED_DDR_BASE)) 

set(CFG_DDR_NSRO_SHARED_MEM_SIZE 0xC00000) 

set(CFG_DDR_SHA_NV_SIZE 0xA00000) 

set(CFG_DDR_SHA_SEC_MEM_SIZE ((DDR_NSRO_SHARED_MEM_SIZE) - (DDR_SHA_NV_SIZE))) 

set(CFG_DDR_SHA_MEM_SIZE 0x100000) 

set(CFG_DDR_MCORE_NR_SHARED_MEM_SIZE 0x100000) 

set(CFG_DDR_SHARED_MEM_SIZE $(CFG_DDR_SHA_MEM_SIZE) + $(CFG_DDR_NSRO_SHARED_MEM_SIZE)) 

set(CFG_DDR_MNTN_SIZE 0x300000) 

set(CFG_DDR_SHARED_MEM_ADDR ( MODEM_NSRO_SHARED_MEM_BASE )) 

set(CFG_DDR_SHAREDNR_MEM_ADDR ((DDR_SHARED_MEM_ADDR      )  +  (DDR_SHARED_MEM_SIZE      ))) 

set(CFG_DDR_MNTN_ADDR ( (DDR_SHAREDNR_MEM_ADDR      )  +  (DDR_MCORE_NR_SHARED_MEM_SIZE  ) )) 

set(CFG_MODEM_SOCP_DDR_BASE (0xB1280000)) 

set(CFG_DDR_SOCP_SIZE 0x8000000) 

set(CFG_DDR_HIFI_SIZE 0x0) 

set(CFG_DDR_SOCP_ADDR ( MODEM_SOCP_DDR_BASE)) 

set(CFG_DDR_HIFI_ADDR ( (DDR_SOCP_ADDR            )  +  (DDR_SOCP_SIZE            ) )) 

set(CFG_NV_MBN_MAX_SIZE 0x20000) 

set(CFG_NV_DDR_SIZE (DDR_SHA_NV_SIZE)) 

set(CFG_NV_COMM_BIN_FILE_MAX_SIZE 0x9DFC00) 

set(CFG_SHA_SEC_ICC_DDR_SIZE (0x50000)) 

set(CFG_SHM_SIZE_NR_PM (NRCCPU_LLRAM_BASE_SIZE)) 

set(CFG_SHM_SIZE_HAC_PM (DDR_SHA_SEC_MEM_SIZE - SHA_SEC_ICC_DDR_SIZE - SHM_SIZE_NR_PM)) 

set(CFG_DDR_HIFI_MBX_ADDR ((DDR_SHARED_MEM_ADDR)  +  (DDR_NSRO_SHARED_MEM_SIZE))) 

set(CFG_DDR_HIFI_MBX_SIZE (0x9800)) 

set(CFG_SHM_SIZE_HIFI_MBX (DDR_HIFI_MBX_SIZE)) 

set(CFG_SHM_SIZE_HIFI (0)) 

set(CFG_SHM_SIZE_TLPHY (12*1024)) 

set(CFG_SHM_SIZE_TEMPERATURE (1*1024)) 

set(CFG_SHM_SIZE_DDM_LOAD (0)) 

set(CFG_SHM_SIZE_MEM_APPA9_PM_BOOT (0)) 

set(CFG_SHM_SIZE_MEM_MDMA9_PM_BOOT (0)) 

set(CFG_SHM_SIZE_TENCILICA_MULT_BAND (0)) 

set(CFG_SHM_SIZE_ICC (0x79800)) 

set(CFG_SHM_SIZE_IPF (0)) 

set(CFG_SHM_SIZE_PSAM (0)) 

set(CFG_SHM_SIZE_WAN (0)) 

set(CFG_SHM_SIZE_NV_UNSAFE (0x400)) 

set(CFG_SHM_SIZE_NV (NV_DDR_SIZE)) 

set(CFG_SHM_SIZE_M3_MNTN (0)) 

set(CFG_SHM_SIZE_TIMESTAMP (1*1024)) 

set(CFG_SHM_SIZE_IOS (6*1024)) 

set(CFG_SHM_SIZE_RESTORE_AXI (CCPU_LLRAM_BASE_SIZE)) 

set(CFG_SHM_SIZE_PMU (3*1024)) 

set(CFG_SHM_SIZE_PTABLE (0)) 

set(CFG_SHM_SIZE_CCORE_RESET (0x400)) 

set(CFG_SHM_SIZE_PM_OM (256*1024)) 

set(CFG_SHM_SIZE_M3PM (0x1000)) 

set(CFG_SHM_SIZE_SLICE_MEM (0x1000)) 

set(CFG_SHM_SIZE_OSA_LOG (0)) 

set(CFG_SHM_SIZE_WAS_LOG (0)) 

set(CFG_SHM_SIZE_SRAM_BAK (0)) 

set(CFG_SHM_SIZE_SRAM_TO_DDR (0)) 

set(CFG_SHM_SIZE_M3RSRACC_BD (0)) 

set(CFG_SHM_SIZE_SIM_MEMORY (0)) 

set(CFG_SHM_SIZE_PRODUCT_MEM (0)) 

set(CFG_SHM_SIZE_SHA_SEC_ICC (SHA_SEC_ICC_DDR_SIZE)) 

set(CFG_NRSHM_SIZE_ICC (0xC0000)) 

set(CFG_FEATURE_NV_PARTRION_MULTIPLEX FEATURE_ON) 

set(CFG_AXIMEM_BASE_ADDR (0xfe380000)) 

set(CFG_AXIMEM_MAX_LEN (128*1024)) 

set(CFG_AXIMEM_CACHE_MAX_LEN (4*1024)) 

set(CFG_AXIMEM_UNCACHE_MAX_LEN (AXIMEM_MAX_LEN - AXIMEM_CACHE_MAX_LEN)) 

set(CFG_CONFIG_BALONG_CORESIGHT NO) 

set(CFG_CONFIG_PM_OM_DEBUG NO) 

