/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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

#ifndef TTF_IRQ_INTERFACE_FUSION_H
#define TTF_IRQ_INTERFACE_FUSION_H

#include "vos.h"
#include "product_config.h"
#include "mdrv.h"

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
#include "mdrv_irq.h"
#include "mdrv_bio.h"
#include "mdrv_mmu.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
#define W_INT11 "w_cpu_int11"

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
typedef HwiArg TtfHwiArg;
/* 中断属性设置, 需要核对芯片手册，中断触发方式TTF用到的都是H-level模式 */
#define TTF_TSP_ISR_PRI 18  /* TSP中断共享方案GU层二的priority统一配置为18,  */
static inline unsigned int TTF_MdrvIntConnect(unsigned int intNum, mdrv_irq_func fun, unsigned int para)
{
    mdrv_irq_attr_t attribute = {TTF_TSP_ISR_PRI, OS_HWI_LEVEL_TRIGGER};
    unsigned int rslt = mdrv_hwi_set_attribute(intNum, &attribute);
    if (rslt != MDRV_OK) {
        return rslt;
    }
    return mdrv_hwi_create(intNum, fun, para);
}

#define TTF_MDRV_INT_CB_DECLARE(funcName) VOS_VOID funcName(TtfHwiArg para)
#define TTF_MDRV_INT_HANDLED
#define TTF_MDRV_INT_CONNECT(intNum, fun, para)  TTF_MdrvIntConnect((unsigned int)(intNum), fun, para)
#define TTF_MDRV_INT_ENABLE(intNum) mdrv_hwi_enable((unsigned int)(intNum))
#define TTF_MDRV_INT_DISABLE(intNum) mdrv_hwi_disable((unsigned int)(intNum))
#define TTF_MDRV_INT_CORE_BIND(intNum, coreMask) mdrv_hwi_core_bind((unsigned int)(intNum), coreMask)
#else

#define TTF_MDRV_INT_CB_DECLARE(funcName)   mdrv_int_return_e funcName(int para, void *ptr)
#define TTF_MDRV_INT_HANDLED    INT_HANDLED
#define TTF_MDRV_INT_CONNECT(intNum, fun, para) mdrv_int_connect(intNum, fun, para)
#define TTF_MDRV_INT_ENABLE(intNum) mdrv_int_enable(intNum)
#define TTF_MDRV_INT_DISABLE(intNum) mdrv_int_disable(intNum)
#define TTF_MDRV_INT_CORE_BIND(intNum, coreMask) mdrv_int_set_affinity(intNum, coreMask)
#endif

#pragma pack(pop)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
