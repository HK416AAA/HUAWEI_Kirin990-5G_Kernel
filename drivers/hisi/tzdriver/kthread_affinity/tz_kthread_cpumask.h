/*
 * tz_kthread_cpumask.h
 *
 * exported funcs for get kthread cpumask
 *
 * Copyright (c) 2021-2021 Huawei Technologies Co., Ltd.
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
#ifndef TZ_KTHREAD_CPUMASK_H
#define TZ_KTHREAD_CPUMASK_H

#include <linux/cpumask.h>

int get_kthread_cpumask(struct cpumask *cpumask);

#endif
