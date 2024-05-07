/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __BSP_THERMAL_H__
#define __BSP_THERMAL_H__

#include <product_config.h>
#include <osl_types.h>

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************
 *                           Attention                           *
 ******************************************************************************
 * Description : Driver for tsensor
 * Core        : Acore¡¢Ccore¡¢Mcore¡¢L2HAC
 * Header File : the following head files need to be modified at the same time
 *             : /acore/bsp_thermal.h
 *             : /ccore/bsp_thermal.h
 *             : /mcore/bsp_thermal.h
 *             : /l2hac/bsp_thermal.h
 ******************************************************************************/
enum thermal_user {
    TEM_PRO_USER_INVALID = -1,
    TEM_PRO_USER_MIN = 0,
    TEM_PRO_FREQ = TEM_PRO_USER_MIN,
    TEM_PRO_USER_MAX,
};

enum thermal_level {
    TEM_PRO_LOW = -1,
    TEM_PRO_NORMAL,
    TEM_PRO_HIGH,
};

enum thermal_event {
    TEM_PRO_OUT,
    TEM_PRO_IN,
};

enum tsensor_get_mode {
    TSENSOR_GET_ICC = 0,
    TSENSOR_GET_SHM,
};

int bsp_thermal_tem_get(u32 tsen_id, enum tsensor_get_mode mode);
typedef int (*bsp_thermal_protect_cb)(int level, int event);
int bsp_thermal_pro_cb_register(int user_id, bsp_thermal_protect_cb pro_cb);
int bsp_thermal_status_get(void);

/*****************************************************************************
 *                        Attention   END                        *
 *****************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* #ifdef __BSP_THERMAL_H__  */