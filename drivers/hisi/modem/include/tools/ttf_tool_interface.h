/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2011-2019. All rights reserved.
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

#ifndef _TTF_TOOL_INTERFACE_H_
#define _TTF_TOOL_INTERFACE_H_

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ����������ʵ�� */
typedef struct {
    VOS_UINT32                              ulThroughput;          /*����������B/s����Ϣ�ϱ���ʱ����Ϊ1s����������ÿ���ϱ�֮������ͳ��*/
    VOS_UINT32                              ulNewDataBlk;          /*�����¿���������1sͳ��*/
    VOS_UINT32                              ulNackDataBlk;         /*����NACK�ش�����������1sͳ��*/
    VOS_UINT32                              ulPendDataBlk;         /*����PENDING�ش�����������1sͳ��*/
    VOS_UINT32                              ulDummyBlk;            /*����Dummy����������1sͳ��*/
} GRM_MntnUlThroughputEntity;

/* ����������ʵ�� */
typedef struct {
    VOS_UINT32                              dlThroughput;          /*���������� B/s*/
    VOS_UINT32                              dlDataAmount;          /*�������ݿ�����, ��1sͳ��*/
    VOS_UINT32                              dlCorrectData;         /*������ȷ�����ݿ�, ��1sͳ��*/
    VOS_UINT32                              dlDataOkBeforeHarq;    /*HARQǰ������ȷ�����ݿ�, ��GPRSģʽ��û��HARQ���룬������GPRSģʽ��ʹ�ø����������ͳ�ƣ���1sͳ��*/
    VOS_UINT32                              dlPendDataBlk;         /*����PENGDING�ش�����������1sͳ��*/
    VOS_UINT32                              dlDataErrBeforeHarq;   /* HARQǰ�����������ݿ飬��1sͳ��*/
    VOS_UINT32                              dlRetransDataBlk;      /* �����ش���PDU���� */
} GRM_MntnDlThroughputEntity;

/*GRLC��ά�ɲ���Ϣʵ��*/
typedef struct {
    GRM_MntnUlThroughputEntity      ulThroughputEntity;   /*������������Ϣ*/
    GRM_MntnDlThroughputEntity      dlThroughputEntity;   /*������������Ϣ*/
} GRM_MntnOmThroughputInfo;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

