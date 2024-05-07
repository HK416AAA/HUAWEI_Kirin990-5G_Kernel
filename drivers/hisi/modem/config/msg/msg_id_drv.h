/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: msg
 * Author:
 * Created: 2019-09-27
 * Last Modified:
 * History:
 * Modification: Create file
 */

#ifndef __MSG_ID_DRV_H__
#define __MSG_ID_DRV_H__

#include "msg_id_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MDRV_MSG_DRV_MIDS \
        DRV_MID_START = MSG_DRV_MID_START,            /* 4096 + 0 = 4096 */  \
        DEFINE_MSG_MID(DRV_MID_MSG)                   /* 4096 + 1 = 4097 */  \
        DEFINE_MSG_MID(DRV_MID_TEST1)      \
        DEFINE_MSG_MID(DRV_MID_TEST2)      \
        DEFINE_MSG_MID(DRV_MID_TEST3)      \
        DEFINE_MSG_MID(DRV_MID_TEST4)      \
        DEFINE_MSG_MID(DRV_MID_TEST5)      \
        DEFINE_MSG_MID(DRV_MID_TEST6)      \
        DEFINE_MSG_MID(DRV_MID_TEST7)      \
        DEFINE_MSG_MID(DRV_MID_TEST8)      \
        DEFINE_MSG_MID(DRV_MID_DIAG_APSS)  \
        DEFINE_MSG_MID(MSP_PID_DIAG_AGENT = DRV_MID_DIAG_APSS) /* compatible ps msg check */\
        DEFINE_MSG_MID(DRV_MID_DIAG_TSP)   \
        DEFINE_MSG_MID(MSP_PID_DIAG_APP_AGENT = DRV_MID_DIAG_TSP) /* compatible ps msg check */\
        DEFINE_MSG_MID(DRV_MID_BBPDS)      \
        DEFINE_MSG_MID(DRV_MID_HIMS_PROC)  \
        DEFINE_MSG_MID(DRV_MID_NV_APSS)    \
        DEFINE_MSG_MID(DRV_MID_NV_TSP)     \
        DEFINE_MSG_MID(DRV_MID_ONOFF_APSS) \
        DEFINE_MSG_MID(DRV_MID_ONOFF_TSP)  \
        DEFINE_MSG_MID(DRV_MID_RESERVED17)     \
        DEFINE_MSG_MID(DRV_MID_RESERVED18)      \
        DEFINE_MSG_MID(DRV_MID_VDEV_APSS)      \
        DEFINE_MSG_MID(DRV_MID_VDEV_TSP)      \
        DEFINE_MSG_MID(DRV_MID_ECDC_APSS)   \
        DEFINE_MSG_MID(DRV_MID_ECDC_TSP)   \
        DEFINE_MSG_MID(DRV_MID_ADC_TSP_DATA)      \
        DEFINE_MSG_MID(DRV_MID_ADC_TSP_CONF)      \
        DEFINE_MSG_MID(DRV_MID_ADC_APSS)      \
        DEFINE_MSG_MID(DRV_MID_ADC_LPMCU_DATA)      \
        DEFINE_MSG_MID(DRV_MID_ADC_LPMCU_CONF)      \
        DEFINE_MSG_MID(DRV_MID_SCI0_TSP)   \
        DEFINE_MSG_MID(DRV_MID_SCI1_TSP)   \
        DEFINE_MSG_MID(DRV_MID_SCI0_APSS)   \
        DEFINE_MSG_MID(DRV_MID_SCI1_APSS)   \
        DEFINE_MSG_MID(DRV_MID_TSENSOR_LPMCU)      \
        DEFINE_MSG_MID(DRV_MID_TSENSOR_APSS)      \
        DEFINE_MSG_MID(DRV_MID_TSENSOR_TSP)   \
        DEFINE_MSG_MID(DRV_MID_EFUSE_APSS)      \
        DEFINE_MSG_MID(DRV_MID_EFUSE_TSP)      \
        DEFINE_MSG_MID(DRV_MID_CPUDFS)      \
        DEFINE_MSG_MID(DRV_MID_BBPDS_APSS) \
        DEFINE_MSG_MID(DRV_MID_THERMAL_TSP) \
        DEFINE_MSG_MID(DRV_MID_THERMAL_APSS) \
        DEFINE_MSG_MID(DRV_MID_THERMAL_LPMCU) \
        DEFINE_MSG_MID(DRV_MID_RESET_LPMCU)      \
        DEFINE_MSG_MID(DRV_MID_RESET_APP)      \
        DEFINE_MSG_MID(DRV_MID_GNSS_APSS)   \
        DEFINE_MSG_MID(DRV_MID_GNSS_TSP)   \
        DEFINE_MSG_MID(DRV_MID_DDR_TEMP)   \
        DEFINE_MSG_MID(DRV_MID_RESERVED47)    \
        DEFINE_MSG_MID(DRV_MID_PMOM_TSP) \
        DEFINE_MSG_MID(DRV_MID_PMOM_APSS) \
        DEFINE_MSG_MID(DRV_MID_LOGHIDS_LPMCU) \
        DRV_MID_END,                                 /* drv max mid num is 4096+4096-1 = 8191 */

#ifdef __cplusplus
}
#endif
#endif
