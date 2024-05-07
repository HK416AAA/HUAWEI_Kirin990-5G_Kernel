/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: msg
 * Author:
 * Created: 2019-09-27
 * Last Modified:
 * History:
 * Modification: Create file
 */

#ifndef __MSG_ID_H__
#define __MSG_ID_H__

#include "msg_id_def.h"
#include "msg_id_drv.h"
#include "msg_id_ps.h"
#include "msg_id_phy.h"
#include "msg_ch.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum MDRV_MSG_MIDS {
    MDRV_MSG_PS_MIDS
    MDRV_MSG_DRV_MIDS
    MDRV_MSG_PHY_MIDS
    MDRV_MSG_MIDS_MAX
};

/* Do not use macros and functions below ！！！(it's for inner and check use) */
/*lint -emacro((656), PS_MID_TOTAL_NUM)*/
#define PS_MID_TOTAL_NUM  (PS_MID_END - PS_MID_START + 1)
/*lint -emacro((656), DRV_MID_TOTAL_NUM)*/
#define DRV_MID_TOTAL_NUM  (DRV_MID_END - DRV_MID_START + 1)
/*lint -emacro((656), PHY_MID_TOTAL_NUM)*/
#define PHY_MID_TOTAL_NUM  (PHY_MID_END - PHY_MID_START + 1)
/*lint -emacro((656), MDRV_MSG_MID_TOTAL_NUM)*/
#define MDRV_MSG_MID_TOTAL_NUM (DRV_MID_TOTAL_NUM + PS_MID_TOTAL_NUM + PHY_MID_TOTAL_NUM)
#define MSG_COMPILE_ASSERT(expression) switch (0) {case 0: break;case expression:break;default: break;}
static inline int msg_compile_check(void)
{
    /* if compiled error here, please check added mid value */
    MSG_COMPILE_ASSERT((PS_MID_TOTAL_NUM <= MDRV_MSG_MAX_ID))
    MSG_COMPILE_ASSERT((DRV_MID_TOTAL_NUM <= MDRV_MSG_MAX_ID))
    MSG_COMPILE_ASSERT((PHY_MID_TOTAL_NUM <= MDRV_MSG_MAX_ID))
    return 0;
}

#ifdef __cplusplus
}
#endif
#endif
