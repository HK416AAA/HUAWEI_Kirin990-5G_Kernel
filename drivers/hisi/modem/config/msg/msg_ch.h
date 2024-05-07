/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: msg
 * Author:
 * Created: 2020-10-10
 * Last Modified:
 * History:
 * Modification: Create file
 */

#ifndef __MSG_CH_H__
#define __MSG_CH_H__
#ifndef __MSG_ID_H__
#error "please include msg_id.h instead"
#endif
#ifdef __cplusplus
extern "C"
{
#endif

/* 使用通道不要依赖通道的枚举值，枚举只能用于通道标识，不要用于运算 */
enum msg_chnid {
    MSG_CHN_LOOP0,
    MSG_CHN_LOOP1,
    /* BEGIN:这部分通道有特殊约定，要么与Modem外组件通信，要么独占资源 */
    MSG_CHN_PAM_CBT = 0x4,
    MSG_CHN_PMU_CBT = 0x5,
    MSG_CHN_RFILE = 0x6,
    MSG_CHN_NVBATCH = 0x7,
    /* END:这部分通道有特殊约定，要么与Modem外组件通信，要么独占资源 */
    /* BEGIN:Modem使用的通道,若无特殊要求可以直接添加 */
    MSG_CHN_MDM_RST,
    MSG_CHN_MLOADER,
    MSG_CHN_EFUSE,
    MSG_CHN_ADC_CONF,
    MSG_CHN_ADC_DATA,
    MSG_CHN_WARMUP,
    MSG_CHN_THERMAL,
    MSG_CHN_TSENSOR,
    MSG_CHN_DDR_TMON,
    MSG_CHN_HIDSLOG,
    MSG_CHN_PM_PRESS,
    MSG_CHN_VCOM,
    /* END:Modem使用的通道,若无特殊要求可以直接添加 */
    MSG_CHN_CNT_MAX
};

#ifdef __cplusplus
}
#endif
#endif
