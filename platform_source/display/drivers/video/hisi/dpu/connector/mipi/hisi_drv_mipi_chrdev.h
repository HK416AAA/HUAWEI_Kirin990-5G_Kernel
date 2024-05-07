/** @file
 * Copyright (c) 2020-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef HISI_DRV_MIPI_CHRDEV_H
#define HISI_DRV_MIPI_CHRDEV_H

#include "hisi_connector_utils.h"

void hisi_drv_mipi_create_chrdev(struct hisi_connector_device *connector_data);
void hisi_drv_mipi_destroy_chrdev(struct hisi_connector_device *connector_data);


#endif /* HISI_DRV_MIPI_CHRDEV_H */
