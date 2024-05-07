/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: msg
 * Author:
 * Created: 2019-09-27
 * Last Modified:
 * History:
 * Modification: Create file
 */

#ifndef __MSG_ID_DEF_H__
#define __MSG_ID_DEF_H__
#ifdef __cplusplus
extern "C"
{
#endif

#define MSG_SID_MAX        (4096)
#define MDRV_MSG_MAX_ID    (1024)

#define MDRV_MSG_HEAD_VER  (1)

#define MSG_GID_PS  0
#define MSG_GID_DRV 1
#define MSG_GID_PHY 2
#define MSG_GID_MAX 3

#define MSG_PS_MID_START    (MSG_SID_MAX*MSG_GID_PS)
#define MSG_DRV_MID_START   (MSG_SID_MAX*MSG_GID_DRV)
#define MSG_PHY_MID_START   (MSG_SID_MAX*MSG_GID_PHY)

#define DEFINE_MSG_MID(mid) mid,

#ifdef __cplusplus
}
#endif
#endif
