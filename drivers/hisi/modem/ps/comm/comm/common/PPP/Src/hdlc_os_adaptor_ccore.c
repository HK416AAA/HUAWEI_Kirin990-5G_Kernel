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

#include "hdlc_os_adaptor_interface.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_mmu.h"
#endif
#include "ttf_comm.h"
#include "gucttf_tag.h"

#define HDLC_REG_LEN_TOTAL OS_MMU_SIZE_ALIGN(0x1000)
#define THIS_MODU mod_ppp

STATIC VOS_VOID PPP_HDLC_HardWrite32Reg(VOS_UINT_PTR addr, VOS_UINT32 value)
{
    TTF_WRITE_32REG(addr, value);
}

STATIC VOS_UINT32 PPP_HDLC_HardRead32Reg(VOS_UINT_PTR addr)
{
    VOS_UINT32 value = TTF_READ_32REG(addr);
    return value;
}

VOS_UINT32 PPP_HDLC_GetAlignBufLen(VOS_UINT32 addr)
{
    return TTF_DATA_ALIGN(addr);
}

VOS_VOID PPP_HDLC_CacheInvalid(VOS_VOID* data, VOS_UINT32 len)
{
}

VOS_VOID PPP_HDLC_CacheFlush(VOS_VOID* data, VOS_UINT32 len)
{
}

VOS_VOID PPP_HDLC_FillIoConfig(PPP_HDLC_HardConfigInfo* cfg)
{    
    cfg->write32RegFunc = PPP_HDLC_HardWrite32Reg;
    cfg->read32RegFunc  = PPP_HDLC_HardRead32Reg;
}

VOS_VOID PPP_HDLC_HardwareDeviceReg(VOS_VOID)
{
}

VOS_VOID PPP_HDLC_RegVirPhyAddrMap(VOS_UINT_PTR ioBaseAddr)
{
#ifdef MODEM_FUSION_VERSION
    VOS_UINT32 rtn = mdrv_mmu_assign(OS_MMU_ADDR_ALIGN(ioBaseAddr), OS_MMU_ADDR_ALIGN(ioBaseAddr),
        HDLC_REG_LEN_TOTAL, TTF_OS_MMU_ATTR_REG);
    if (rtn != MDRV_OK) {
        TTF_PRINT_ERR("TTF_CIPHER_Init fail. fail Num: %d, Addr: 0x%x.\n", rtn, ioBaseAddr);
    }
#endif
}

