/*
 * rpmb_driver.c
 *
 * function for rpmb
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "rpmb_driver.h"
#include <linux/kallsyms.h>
#include "tc_ns_log.h"

typedef int *(rpmb_ioctl_func)(enum func_id id, enum rpmb_op_type operation,
	struct storage_blk_ioc_rpmb_data *storage_data);
int rpmb_ioctl_cmd(enum func_id id, enum rpmb_op_type operation,
	struct storage_blk_ioc_rpmb_data *storage_data)
{
	static rpmb_ioctl_func *rpmb_ioctl = NULL;

	if (storage_data == NULL)
		return NULL;

	if (rpmb_ioctl == NULL) {
		rpmb_ioctl =
			(rpmb_ioctl_func *)(uintptr_t)kallsyms_lookup_name("vendor_rpmb_ioctl_cmd");
		if (rpmb_ioctl == NULL) {
			tloge("fail to find symbol vendor_rpmb_ioctl_cmd\n");
			return NULL;
		}
	}
	return rpmb_ioctl(id, operation, storage_data);
}
