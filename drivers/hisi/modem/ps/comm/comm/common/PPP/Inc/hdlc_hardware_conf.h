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

#ifndef __HDLC_HRADWARE_CONF_H__
#define __HDLC_HRADWARE_CONF_H__

#include "vos.h"
#include "ttf_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ����Ʒ���Ժ�ӳ��ɿ���HDLC HARDWARE�����Ƿ����� */
#if ((FEATURE_HARDWARE_HDLC_ON_CCPU == FEATURE_ON) || (FEATURE_HARDWARE_HDLC_FUNC == FEATURE_ON) || \
     (TEST_HDLC_HARDWARE_FEATURE == FEATURE_ON))
#define FEATURE_PPP_HDLC_HARDWARE FEATURE_ON
#else
#define FEATURE_PPP_HDLC_HARDWARE FEATURE_OFF
#endif

/* HDLC��ʹ�õ�PID */
#define PS_PID_PPP_HDLC (g_pppHdlcConfig.userPid)

/* HDLC��д�Ĵ��� */
#define PPP_HDLC_WRITE_32REG(addr, value) g_pppHdlcConfig.write32RegFunc(addr, value)
#define PPP_HDLC_READ_32REG(addr) g_pppHdlcConfig.read32RegFunc(addr)
/* HDLCʹ�õķ�װ���װ�ڴ���ʵ��ַת�� */
#define PPP_HDLC_HARD_BUF_PHY2VIRT(pAddr) PPP_HDLC_HardBufPhy2Virt((VOS_UINT_PTR)pAddr)
#define PPP_HDLC_HARD_BUF_VIRT2PHY(vAddr) PPP_HDLC_HardBufVirt2Phy((VOS_UINT_PTR)vAddr)
/* HDLCҵ��ģ���ڴ���ʵ��ַת�� */
#define PPP_HDLC_HARD_MEM_PHY2VIRT(pAddr) PPP_HDLC_HardMemPhy2Virt((VOS_UINT_PTR)pAddr)
#define PPP_HDLC_HARD_MEM_VIRT2PHY(vAddr) PPP_HDLC_HardMemVirt2Phy((VOS_UINT_PTR)vAddr)
/* HDLC����ַIOת�� */
#define PPP_HDLC_HARD_IO_ADDRESS(addr) PPP_HDLC_HardIOAddrCvt(addr)
/* HDLC���װ����������󳤶� */
#define HDLC_DEF_IN_PER_MAX_CNT (g_pppHdlcConfig.defOneMaxSize)

/* hdlcʹ�õ��ڴ����ú��� */
typedef VOS_VOID (*PPP_HDLC_HardMemSetFunc)(
    VOS_VOID *dest, VOS_SIZE_T destSize, VOS_CHAR cChar, VOS_SIZE_T count, VOS_UINT32 fileId, VOS_UINT32 lineNo);

/* hdlcʹ�õ��ڴ濽������ */
typedef VOS_VOID (*PPP_HDLC_HardMemCpyFunc)(
    VOS_VOID *dest, VOS_SIZE_T destSize, const VOS_VOID *src, VOS_SIZE_T count, VOS_UINT32 fileId, VOS_UINT32 lineNo);

/* ҵ��ģ���ڴ���ʵ��ַת������ */
typedef VOS_UINT_PTR (*PPP_HDLC_HardMemCvtFunc)(VOS_UINT_PTR addr);

/* hdlcʹ�õ��ڴ����ʵ��ַת������ */
typedef VOS_UINT_PTR (*PPP_HDLC_HardBufCvtFunc)(
    const VOS_UINT8 *pucCurAddr, const VOS_UINT8 *pucStartAddr1, const VOS_UINT8 *startAddr2, VOS_UINT32 bufLen);

/* д32λ�Ĵ��� */
typedef VOS_VOID (*PPP_HDLC_HardWriteRegFunc)(VOS_UINT_PTR addr, VOS_UINT32 value);

/* ��32λ�Ĵ��� */
typedef VOS_UINT32 (*PPP_HDLC_HardReadRegFunc)(VOS_UINT_PTR addr);

/* IO��ַӳ�亯�� */
typedef VOS_UINT_PTR (*PPP_HDLC_HardIoCvtFunc)(VOS_UINT_PTR addr);

/*
 * hdlc������Ϣ
 * pMemPhy2VirtFunc��pMemVirt2PhyFunc��pBufPhy2VirtFunc��pBufVirt2PhyFunc��
 * pIOAddrCvtFuncΪ�ձ�ʾ����Ҫת��
 * pMemSetFunc��pMemCpyFunc��ulUserPid��ulPppTotalBufLen��pWrite32RegFunc��
 * pRead32RegFunc������Ϊ��
 * ulPppVirtAddr��ulPppPhyAddrΪServiceģ�����룬ҵ��㲻������
 */
typedef struct {
    VOS_UINT32                pppTotalBufLen;  /* HDLCʹ�õ��ڴ泤�ȣ���ҵ��ģ������ */
    VOS_UINT_PTR              pppVirtAddr;     /* HDLCʹ���ڴ�����ַ��ҵ��ģ�����ó��ȣ�Service���� */
    VOS_UINT_PTR              pppPhyAddr;      /* HDLCʹ���ڴ��ʵ��ַ */
    PPP_HDLC_HardMemCvtFunc   memPhy2VirtFunc; /* ҵ��ģ���ڴ�ʵ��ַת���ַ������ҵ��ģ������ */
    PPP_HDLC_HardMemCvtFunc   memVirt2PhyFunc; /* ҵ��ģ���ڴ����ַתʵ��ַ������ҵ��ģ������ */
    PPP_HDLC_HardBufCvtFunc   bufPhy2VirtFunc; /* HDLC�ڴ�ʵ��ַת���ַ������ҵ��ģ������ */
    PPP_HDLC_HardBufCvtFunc   bufVirt2PhyFunc; /* HDLC�ڴ����ַתʵ��ַ������ҵ��ģ������ */
    VOS_UINT32                userPid;         /* ҵ��ģ��PID */
    PPP_HDLC_HardWriteRegFunc write32RegFunc;  /* д�Ĵ������� */
    PPP_HDLC_HardReadRegFunc  read32RegFunc;   /* ���Ĵ������� */
    PPP_HDLC_HardIoCvtFunc    ioAddrCvtFunc;   /* IO��ַӳ�亯�� */
    VOS_UINT32                defOneMaxSize;   /* ���װ���뵥��������󳤶� */
} PPP_HDLC_HardConfigInfo;

extern PPP_HDLC_HardConfigInfo g_pppHdlcConfig;

VOS_UINT32 PPP_HdlcHardConfig(PPP_HDLC_HardConfigInfo *hdlcConfig);

VOS_UINT8* PPP_HDLC_HardBufPhy2Virt(VOS_UINT_PTR pAddr);
VOS_UINT8* PPP_HDLC_HardBufVirt2Phy(VOS_UINT_PTR vAddr);

VOS_UINT8* PPP_HDLC_HardMemPhy2Virt(VOS_UINT_PTR pAddr);
VOS_UINT8* PPP_HDLC_HardMemVirt2Phy(VOS_UINT_PTR vAddr);

VOS_UINT_PTR PPP_HDLC_HardIOAddrCvt(VOS_UINT_PTR addr);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
