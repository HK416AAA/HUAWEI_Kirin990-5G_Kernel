/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__
#include "mdrv_adc.h"
#ifdef FEATURE_NVA_ON
#include <nva_id.h>
#endif
#include <nv_id_drv.h>
#include <hi_adc.h>
#include <osl_types.h>
#include <bsp_print.h>
#define adc_log(fmt, ...) (bsp_err("<%s>,[line: %d]" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define adc_dbg(fmt, ...) (bsp_debug("<%s>,[line: %d]" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))

struct adc_reg {
    u32 version;

    u32 ctrl;
    u32 start;
    u32 data_l;
    u32 data_h;
    u32 mode_cfg;
    u32 cali_ctrl;
    u32 cali_value;
    u32 ref_voltage;

    u32 xoadc_pwrup_cali_end;
    u32 xoadc_autocali_ave0;
    u32 xoadc_autocali_ave1;
    u32 xoadc_autocali_ave2;
    u32 xoadc_autocali_ave3;
    u32 xoadc_soc_ana_en;
    u32 xoadc_state;
    u32 xoadc_data_l;
    u32 xoadc_data_h;
    u32 xoadc_ref_voltage;
};


#ifdef FEATURE_NVA_ON
#define NV_ID_ADC_CHANNEL_MAP (NVA_ID_ADC_CHANNEL_MAP)
#else // FEATURE_NVA_ON
#define NV_ID_ADC_CHANNEL_MAP (0xd126)
#endif // FEATURE_NVA_ON

#ifdef ADC_DRV_NV
#ifdef FEATURE_NVA_ON
#define NV_ID_ADC_XO_CONVERT_TABLE (NVA_ID_DRV_ADC_XO_CONVERT_TABLE)
#define NV_ID_ADC_PA_CONVERT_TABLE (NVA_ID_DRV_ADC_PA_CONVERT_TABLE)
#else // FEATURE_NVA_ON
#define NV_ID_ADC_XO_CONVERT_TABLE (NV_ID_DRV_ADC_XO_CONVERT_TABLE)
#define NV_ID_ADC_PA_CONVERT_TABLE (NV_ID_DRV_ADC_PA_CONVERT_TABLE)
#endif // FEATURE_NVA_ON
#else // ADC_DRV_NV
#define NV_ID_ADC_XO_CONVERT_TABLE (10016)
#define NV_ID_ADC_PA_CONVERT_TABLE (10015)
#endif // ADC_DRV_NV

#define MCORE_ACORE_FUNC_ADC_DATA (MCORE_ACORE_FUNC_HKADC)

#define ADC_ICC_CHN_MCORE_ACORE_DATA (ICC_CHN_MCORE_ACORE << 16 | MCORE_ACORE_FUNC_ADC_DATA)

#ifdef CONFIG_BALONG_ADC
#define ACORE_ACORE_FUNC_ADC_DATA (ACORE_ACORE_FUNC_HKADC)
#define ADC_ICC_CHN_ACORE_ACORE_DATA (ICC_CHN_APLUSB_IFC << 16 | ACORE_ACORE_FUNC_ADC_DATA)
#endif

int bsp_adc_convert(u32 channel_id, u16 *value);
int bsp_adc_get_by_shm(int phy_channel_id, struct adc_data *data);
int bsp_adc_get_by_icc(int phy_channel_id, struct adc_data *data);
int bsp_adc_init(void);


#endif
