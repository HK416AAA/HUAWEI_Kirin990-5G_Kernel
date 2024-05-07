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

#ifndef __HDLC_HARDWARE_H__
#define __HDLC_HARDWARE_H__

#include "hdlc_interface.h"
#include "hdlc_hardware_service.h"
#include "v_typdef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ���������������Ϣ */
typedef struct _HDLC_PARA_LINK_BUILD_PARA_STRU {
    PPP_DataTypeUint8            dataType; /* �������� */
    PPP_ServiceHdlcHardModeUint8 pppMode;  /* PPP����ģʽ */
    VOS_UINT8                    reserve1[2];
    VOS_UINT16                   pppId;    /* PPP ID */
    VOS_UINT16                   protocol; /* Э�����ͣ���װ��ʱ��ʹ�� */
    PPP_ZcQueue                 *dataQ;    /* ���ݶ��� */
} HDLC_ParaLinkBuildPara;

/* ���������������Ϣ */
typedef struct _HDLC_PARA_LINK_BUILD_INFO_STRU {
    VOS_UINT32 dealCnt;                                        /* ���ι���������̴����ݶ������ܹ���������ݰ����� */
    VOS_UINT32 inputLinkNodeCnt;                               /* �����������ڵ���� */
    PPP_Zc    *inputLinkNode[HDLC_INPUT_PARA_LINK_MAX_SIZE];   /* �����������ڵ��Ӧ���㿽���ڴ� */
    VOS_UINT32 outputLinkNodeCnt;                              /* �����������ڵ������ֻ�ڷ�װ��Ч */
    PPP_Zc    *outputLinkNode[HDLC_OUTPUT_PARA_LINK_MAX_SIZE]; /* �����������ڵ��Ӧ���㿽���ڴ棬ֻ�ڷ�װ��Ч */
} HDLC_ParaLinkBuildInfo;

typedef struct {
    VOS_UINT32 hisAcf : 1, /* �Զ��Ƿ�֧�ֵ�ַ������ѹ�� */
        hisPcf : 1,        /* �Զ��Ƿ�֧��Э����ѹ�� */
        myAcf : 1,         /* �����Ƿ�֧�ֵ�ַ������ѹ�� */
        myPcf : 1,         /* �����Ƿ�֧��Э����ѹ�� */
        resv : 28;
    VOS_UINT32 hisAccm; /* �Զ˵�ͬ�첽ת�������� */
} HDLC_CfgInfo;

extern VOS_VOID PPP_HDLC_HardUpdateCfg(HDLC_CfgInfo *hdlcCfg);
extern VOS_UINT32 PPP_HDLC_HardInit(VOS_VOID);
extern VOS_BOOL PPP_HDLC_HardProcData(PPP_Id pppId, PPP_ZcQueue *dataQ);
extern VOS_VOID PPP_HDLC_HardCfgInit(VOS_VOID);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
