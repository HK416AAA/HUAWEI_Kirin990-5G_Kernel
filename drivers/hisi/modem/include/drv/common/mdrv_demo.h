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

/**
 *  @brief   demo模块在acore上的对外头文件,接口自动化格式模板
 *  @file    mdrv_demo.h
 *  @author  xxx
 *  @version v1.0
 *  @date    xxx.xx.xx
 *  @note    修改记录(版本号|修订日期|说明) 
 *  <ul><li>v1.0|xxx.xx.xx|创建文件</li></ul>
 *  @since   
 */

#ifndef __MDRV_DEMO_H__
#define __MDRV_DEMO_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @brief 共享区域数据边界魔幻数
*/
#define DEMO_MAGIC_DATA (0x5A5A5A5A)

/**
* @brief 支持的逻辑通道号列表
*/
typedef enum demo_logic_channel
{
    DEMO_LOGIC_CHANNEL_MIN = 0,     /**< 逻辑通道最小边界值 */
    DEMO_LOGIC_CHANNEL_BATTERY = 0, /**< 电池逻辑通道 */
    DEMO_LOGIC_CHANNEL_BAT_ET = 16, /**< 电池ET逻辑通道 */
    DEMO_LOGIC_CHANNEL_MAX
} demo_logic_channel_e;

/**
* @brief demo信息结构体
*/
typedef struct demo_data
{
    unsigned short code;       /**< demo转换的码值 */
    unsigned short code_r;     /**< demo转换的码值按位取反 */
    unsigned short voltage;    /**< demo转换的电压值 */
    unsigned short voltage_r;  /**< demo转换的电压值按位取反 */
} demo_data_s;

/**
* @brief 共享区域数据排布结构体
*/
struct demo_data_shm
{
    unsigned int magic_start;                /**< 其值为DEMO_MAGIC_DATA */
    struct demo_data data[ADC_CHANNEL_NUM];  /**< 各通道的adc信息 */
    unsigned int magic_end;                  /**< 其值为DEMO_MAGIC_DATA */
};

/**
 * @brief 获取平台温度接口（包括tsensor和adc）
 *
 * @par 描述:
 * 获取平台温度接口（包括tsensor和adc）
 *
 * @attention
 * <ul><li>tensor时电压码值和反码值为0。</li></ul>
 *
 * @param[in]  module_id , 逻辑通道号。
 * @param[in]  channel_id, 物理通道号。
 * @param[out] data      , 采集值，对应通道的电压码值、电压反码值。
 *
 * @retval 0,表示函数执行成功。
 * @retval ！=0,表示函数执行失败。

 * @par 依赖:
 * <ul><li>mdrv_demo.h：该接口声明所在的头文件。</li></ul>
 *
 * @see demo_logic_channel_e demo_data_s
 */
int mdrv_get_demo_info(demo_logic_channel_e module_id, int channel_id, demo_data_s *data);

#ifdef __cplusplus
}
#endif

#endif

