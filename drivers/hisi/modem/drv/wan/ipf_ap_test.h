/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: eipf interface for drv
 * Author: shennaiyu
 * Create: 2020-10-12
 */

#ifndef __IPF_AP_TEST_H__
#define __IPF_AP_TEST_H__

int ipf_ap_push(void);
int ipf_ap_config_bd(struct sk_buff *skb);
int ipf_test_init_with_maa(void);
int ipf_test_init_no_maa(void);

#endif
