#ifndef __BBOX_DIAGINFO_ID_DEF_H__
#define __BBOX_DIAGINFO_ID_DEF_H__ 
enum bbox_diaginfo_errid {
 SoC_DIAGINFO_START=925200000,
 L3_ECC_CE = SoC_DIAGINFO_START,
 CPU_UP_FAIL,
 CPU_PANIC_INFO,
 NOC_FAULT_INFO,
 L3_ECC_UE = 925201200,
 IOMCU_DIAG_INFO_HW_START = 925201600,
 IGS_DMD_FDUL_PW_ON = IOMCU_DIAG_INFO_HW_START,
 IGS_DMD_FDUL_PW_OFF = 925201601,
 IGS_DMD_HWTS_PW_ON = 925201602,
 IGS_DMD_HWTS_PW_OFF = 925201603,
 IGS_DMD_AIC_PW_ON = 925201604,
 IGS_DMD_AIC_PW_OFF = 925201605,
 IGS_DMD_CAM_PW_ON = 925201608,
 IGS_DMD_CAM_PW_OFF = 925201609,
 IGS_DMD_CAM_IR_PW = 925201610,
 IGS_DMD_CAM_TIMEOUT = 925201611,
 IOMCU_DIAG_INFO_HW_END = 925201699,
 IOMCU_DIAG_INFO_SW_START = 925201700,
 IGS_DMD_SLEEP_FUSION = 925201714,
 IOMCU_DIAG_INFO_SW_END = 925201899,
 IOMCU_DIAG_INFO_MONITER_START = 925201900,
 IOMCU_DIAG_INFO_MONITER_END = 925201959,
 IOMCU_DIAG_INFO_RESERVED_START = 925201960,
 IOMCU_DIAG_INFO_RESERVED_END = 925201999,
 DDR_DIAGINFO_START=925202000,
 LPM3_DDR_FAIl = DDR_DIAGINFO_START,
 FBE_DIAG_FAIL_ID = 925204500,
 BLOCK_DMD_CP_IO = 925205500,
 BLOCK_DMD_NORMAL_IO = 925205501,
 IGS_DMD_NPU_PLL_RETRY = 925206302,
 BBOX_DIAGINFO_END = 925299999
};
enum bbox_diaginfo_module {
 SoC_AP = 1,
 DSS,
 DDR,
 FDUL,
};
enum bbox_diaginfo_level {
 CRITICAL = 1,
 WARNING,
 INFO,
};
enum bbox_diaginfo_type {
 HW = 1,
 SW,
};
#endif
