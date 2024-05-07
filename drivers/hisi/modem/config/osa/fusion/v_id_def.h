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

/*
 * ��Ȩ���У�c����Ϊ�������޹�˾ 2015-2019
 * ��������: OSA ����ͷ�ļ�
 */
#ifndef _V_IDDEF_H
#define _V_IDDEF_H

#include "v_typdef.h"
#include "product_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*
 * ����˵��: VOS���ģʽ��PID��FID��Ӧ��ϵ
 */
typedef struct ID_PID_to_FID {
    unsigned long pid; /* PID */
    unsigned long fid; /* PID������FID */
} PID_to_FID_Type;

/************************* MACRO & TYPE DEFINITION ***************************/
#define BEGIN_FID_DEFINITION() \
    enum VOS_Function_ID { \
        VOS_FID_RESERVED = 0,

#define END_FID_DEFINITION() \
        VOS_FID_BUTT \
    };

#define DEFINE_FID(fid) fid,

#define END_DECLARE_PID_BELONG_TO_FID() \
    }

#define ON_DECLARE(pid, fid, pri, ModemId) \
    MOD_RegFidPidRSP(pid, fid, pri, ModemId);

/* VOS���ģʽ�¹���ģ�龲̬������ */
#define FID_NAME_LENGTH         (16)

typedef struct ID_StaticTableType {
    VOS_FID       fid;                       /* FID           */
    VOS_UINT8     achFIDName[FID_NAME_LENGTH]; /* FID����       */
    InitFunType   pfnInitFun;                  /* FID��ʼ������ */
    VOS_UINT32    startOrder;                /* FID����˳��   */
    VOS_UINT32    stackSize;
    /* which should be del when only one FID exists */
    VOS_UINT32 queueSize;
} FID_StaticTableType;

/*
 * ����˵��: Ӧ��ģ���ʼ��˳���
 */
typedef struct APP_StartOrderType {
    VOS_VOID (*pfnInit)(VOS_VOID);
    VOS_UINT32 startOrder; /* FID����˳�� */
} APP_Start_Order_Type;

enum VOS_DOPRA_Function_ID {

    DOPRA_FID_SYS = 0,

    VOS_FID_DOPRAEND /* ϵͳFID���������־ */
};

enum VOS_DOPRA_ProcessId {

    DOPRA_PID_SYS   = 0,
    DOPRA_PID_TIMER = 1
};

#define BEGIN_DECLARE_PID_BELONG_TO_FID()\
VOS_VOID REG_FID_PID_RSP(VOS_VOID){\
        MOD_RegFidPidRSP(DOPRA_PID_SYS, DOPRA_FID_SYS, VOS_PRIORITY_BASE, MODEM_ID_BUTT);\
        MOD_RegFidPidRSP(DOPRA_PID_TIMER, DOPRA_FID_SYS, VOS_PRIORITY_BASE, MODEM_ID_BUTT);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _V_IDDEF_H */