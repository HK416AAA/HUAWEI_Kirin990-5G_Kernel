/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: the hw_kernel_stp_utils.h for kernel stp
 * Create: 2018-03-31
 */

#ifndef _HW_KERNEL_STP_UTILS_H_
#define _HW_KERNEL_STP_UTILS_H_

#include <linux/buffer_head.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/ratelimit.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#define KSTP_ERRCODE (-1)

#define KSTP_ERROR "[Error]"
#define kstp_log_error(tag, fmt, args...) \
	pr_err_ratelimited("%s[%s][%s] " fmt "\n", KSTP_ERROR, \
						tag, __func__, ##args)

#define KSTP_WARNING "[Warning]"
#define kstp_log_warning(tag, fmt, args...) \
	pr_warn_ratelimited("%s[%s][%s] " fmt "\n", KSTP_WARNING, \
						tag, __func__, ##args)

#define KSTP_TRACE "[Trace]"
#define kstp_log_trace(tag, fmt, args...) \
	pr_info_ratelimited("%s[%s][%s] " fmt "\n", KSTP_TRACE, \
						tag, __func__, ##args)

#ifdef CONFIG_HW_KERNEL_STP_ENG_DEBUG

#define KSTP_DEBUG "[Debug]"
#define kstp_log_debug(tag, fmt, args...) \
	printk_ratelimited("%s[%s][%s] " fmt "\n", KSTP_DEBUG, \
							tag, __func__, ##args)

#else
#define kstp_log_debug(tag, fmt, args...) no_printk(fmt, ##args)
#endif

#endif /* _HW_KERNEL_STP_UTILS_H_ */
