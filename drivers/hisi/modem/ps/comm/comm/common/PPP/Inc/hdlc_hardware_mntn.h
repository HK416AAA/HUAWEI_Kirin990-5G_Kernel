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

#ifndef __HDLC_HRADWARE_MNTN_H__
#define __HDLC_HRADWARE_MNTN_H__

#include "vos.h"
#include "ttf_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* HDLCͳ����Ϣ��ַ */
#define PPP_HDLC_HARD_DATA_PROC_STAT (&g_pppHdlcHardStat)

/*
 * �����쳣��Ϣbitλ��:
 *      bit0�����Ƿ�����쳣����(1�ǽ����쳣����0��û�н����쳣����)
 *      bit1�����Ƿ����жϴ���(1���жϴ���0����ѯ����)
 *      bit2�����Ƿ�ȴ��ź�����ʱ(1�ǣ�0��)
 *      bit3�����Ƿ���ѯ��ʱ(1�ǣ�0��)
 */
#define HDLC_EXCEPTION_IND_BITPOS 0UL
#define HDLC_INTERRUPT_IND_BITPOS 1UL
#define HDLC_SEM_TIMEOUT_IND_BITPOS 2UL
#define HDLC_WAIT_STATUS_TIMEOUT_IND_BITPOS 3UL

/* HDLC Warning Log��Ϣ,VOS_GetModemIDFromPid�������ˣ�PS_GetModemIdFromPid��A�˲�֧�֣�����д��MODEM_ID_0 */
#define PPP_HDLC_WARNING_LOG(str) TTF_COMM_LOG(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_WARNING, str)
#define PPP_HDLC_WARNING_LOG1(str, para1) \
    TTF_COMM_LOG1(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_WARNING, str, para1)

#define PPP_HDLC_WARNING_LOG2(str, para1, para2) \
    TTF_COMM_LOG2(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_WARNING, str, para1, para2)

#define PPP_HDLC_WARNING_LOG3(str, para1, para2, para3) \
    TTF_COMM_LOG3(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_WARNING, str, para1, para2, para3)

#define PPP_HDLC_WARNING_LOG4(str, para1, para2, para3, para4) \
    TTF_COMM_LOG4(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_WARNING, str, para1, para2, para3, para4)

#define PPP_HDLC_ERROR_LOG(str) TTF_COMM_LOG(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_ERROR, str)
#define PPP_HDLC_ERROR_LOG1(str, para1) \
    TTF_COMM_LOG1(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_ERROR, str, para1)

#define PPP_HDLC_ERROR_LOG2(str, para1, para2) \
    TTF_COMM_LOG2(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_ERROR, str, para1, para2)

#define PPP_HDLC_ERROR_LOG3(str, para1, para2, para3) \
    TTF_COMM_LOG3(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_ERROR, str, para1, para2, para3)

#define PPP_HDLC_ERROR_LOG4(str, para1, para2, para3, para4) \
    TTF_COMM_LOG4(MODEM_ID_0, PS_PID_PPP_HDLC, DIAG_MODE_COMM, LOG_LEVEL_ERROR, str, para1, para2, para3, para4)

/*
 * HDLCά��������Ϣ
 *      bit0��1��ʾ��Ҫ��ȡ������Ϣ
 *      bit1��1��ʾ��Ҫ��ȡ�Ĵ�����Ϣ
 *      bit2��1��ʾ��Ҫ��ȡ���ݱ�����Ϣ
 */
#define PPP_HDLC_MNTN_TRACE_PARA 1
#define PPP_HDLC_MNTN_TRACE_REG 2
#define PPP_HDLC_MNTN_TRACE_DATA 4

/* ��ά�ɲ�������ڴ���󳤶ȣ���ֹ��Ϊ��ά�ɲ����벻���ڴ浼�µ��帴λ */
#define HDLC_MNTN_ALLOC_MEM_MAX_SIZE (2 * 1024)

enum HDLC_MNTN_EventType {
    ID_HDLC_MNTN_FRM_REG_BEFORE_EN = 101, /* _H2ASN_MsgChoice HDLC_MNTN_FRM_REG_CONFIG_STRU */
    ID_HDLC_MNTN_FRM_REG_AFTER_EN,        /* _H2ASN_MsgChoice HDLC_MNTN_FRM_REG_CONFIG_STRU */
    ID_HDLC_MNTN_FRM_INPUT_PARA,          /* _H2ASN_MsgChoice HDLC_MNTN_INPUT_PARA_LINK_STRU */
    ID_HDLC_MNTN_FRM_OUTPUT_PARA,         /* _H2ASN_MsgChoice HDLC_MNTN_FRM_OUTPUT_PARA_STRU */
    ID_HDLC_MNTN_FRM_INPUT_DATA,          /* _H2ASN_MsgChoice HDLC_MNTN_NODE_DATA_STRU */
    ID_HDLC_MNTN_FRM_OUTPUT_DATA,         /* _H2ASN_MsgChoice HDLC_MNTN_NODE_DATA_STRU */
    ID_HDLC_MNTN_DEF_REG_BEFORE_EN,       /* _H2ASN_MsgChoice HDLC_MNTN_DEF_REG_CONFIG_STRU */
    ID_HDLC_MNTN_DEF_REG_AFTER_EN,        /* _H2ASN_MsgChoice HDLC_MNTN_DEF_REG_CONFIG_STRU */
    ID_HDLC_MNTN_DEF_INPUT_PARA,          /* _H2ASN_MsgChoice HDLC_MNTN_INPUT_PARA_LINK_STRU */
    ID_HDLC_MNTN_DEF_OUTPUT_PARA,         /* _H2ASN_MsgChoice HDLC_MNTN_DEF_OUTPUT_PARA_STRU */
    ID_HDLC_MNTN_DEF_INPUT_DATA,          /* _H2ASN_MsgChoice HDLC_MNTN_NODE_DATA_STRU */
    ID_HDLC_MNTN_DEF_OUTPUT_DATA,         /* _H2ASN_MsgChoice HDLC_MNTN_NODE_DATA_STRU */
    ID_HDLC_MNTN_DEF_UNCOMPLETED_INFO,    /* _H2ASN_MsgChoice HDLC_MNTN_DEF_UNCOMPLETED_INFO_STRU */

    ID_HDLC_MNTN_EVENT_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT32 HDLC_MNTN_EventTypeUint32;

/* ͳ����Ϣ */
typedef struct {
    VOS_UINT32 defIpDataProcCnt;   /* ���װIP���ݰ�������� */
    VOS_UINT32 defPppDataProcCnt;  /* ���װ��PPP֡������� */
    VOS_UINT32 defUncompleteCnt;   /* ���װ�з�����֡������� */
    VOS_UINT32 defWaitIntCnt;      /* ���װ�ȴ��жϴ��� */
    VOS_UINT32 defWaitQueryCnt;    /* ���װ�ȴ���ѯ���� */
    VOS_UINT32 defLcpPauseCnt;     /* ���װLCP��ͣ���� */
    VOS_UINT32 defFullPauseCnt;    /* ���װ����ռ���ϱ��ռ�����ͣ���� */
    VOS_UINT32 defInputDiscardCnt; /* ���װ�������ݰ����� */

    VOS_UINT32 frmIpDataProcCnt;         /* ��װIP���ݰ�������� */
    VOS_UINT32 frmPppDataProcCnt;        /* ��װ��PPP֡������� */
    VOS_UINT32 frmWaitIntCnt;            /* ��װ�ȴ��жϴ��� */
    VOS_UINT32 frmWaitQueryCnt;          /* ��װ�ȴ���ѯ���� */
    VOS_UINT32 frmAllocOutputMemFailCnt; /* ��װ����Ŀ���ڴ�ʧ�ܴ��� */
    VOS_UINT32 frmAllocFirstMemFailCnt;  /* ��װ�����һ���ڵ�Ŀ���ڴ�ʧ�ܴ��� */
    VOS_UINT32 frmOutputLinkFullCnt;     /* ��װ������������� */
    VOS_UINT32 frmInputDiscardCnt;       /* ��װ�������ݰ����� */

    VOS_UINT32 defMaxInputCntOnce;  /* ���װһ������������ݸ��� */
    VOS_UINT32 defMaxInputSizeOnce; /* ���װһ��������������ܳ��� */
    VOS_UINT32 defMaxValidCntOnce;  /* ���װһ����������Ч֡���� */
    VOS_UINT32 defMaxQueryCnt;      /* ���װ��ѯ������ */

    VOS_UINT32 frmMaxInputCntOnce;   /* ��װһ������������ݸ��� */
    VOS_UINT32 frmMaxInputSizeOnce;  /* ��װһ��������������ܳ��� */
    VOS_UINT32 frmMaxOutputCntOnce;  /* ��װһ��������ʹ�ýڵ������� */
    VOS_UINT32 frmMaxOutputSizeOnce; /* ��װһ��������ʹ�ýڵ��ܳ��� */
    VOS_UINT32 frmMaxQueryCnt;       /* ��װ��ѯ������ */

    VOS_UINT32 maxCntOnce;      /* PPPһ����ദ��Ľ����� */
    VOS_UINT32 hdlcHardProcCnt; /* PPPһ����ദ��Ľ����� */

    VOS_UINT32 defIsrCnt;          /* ���װ�жϷ��������ô��� */
    VOS_UINT32 frmIsrCnt;          /* ��װ�жϷ��������ô��� */
    VOS_UINT32 continueCnt;        /* �����е�������Ҫ�ֶ�δ���Ĵ��� */
    VOS_UINT16 defExpInfo;         /* ���װ�쳣��Ϣ */
    VOS_UINT16 frmExpInfo;         /* ��װ�쳣��Ϣ */
    VOS_UINT32 forbidHdlcBugNoCpy; /* ���HDLC BUG,���������� */
    VOS_UINT32 forbidHdlcBugCpy;   /* ���HDLC BUG,��Ҫ�������� */
} PPP_HDLC_HardDataProcStat;

/* ������������ά�ɲ�ṹ */
typedef struct {
    VOS_MSG_HEADER /* ��Ϣͷ */
        HDLC_MNTN_EventTypeUint32 msgname;
} HDLC_MNTN_TraceHead;

/* ���������������ݹ��� */
typedef struct {
    HDLC_MNTN_TraceHead head; /* _H2ASN_Skip */
    VOS_UINT16          nodeIndex;
    VOS_UINT16          len;
    /* VOS_UINT8[usLen]�������ݷ�����������ʱ����ͷ�����������ݳ��� */
} HDLC_MNTN_NodeData;

/* HDLC�ؼ��Ĵ�����ά�ɲ���Ϣ����ṹ */
typedef struct {
    VOS_UINT32 hdlcFrmRawInt; /* ��װԭʼ�жϼĴ���ֵ */
    VOS_UINT32 hdlcFrmStatus; /* ��װ״̬�Ĵ���ֵ */
    VOS_UINT32 hdlcDefRawInt; /* ���װԭʼ�жϼĴ���ֵ */
    VOS_UINT32 hdlcDefStatus; /* ���װ״̬�Ĵ���ֵ */
} HDLC_RegSaveInfo;

/* ͳ����Ϣ */
extern PPP_HDLC_HardDataProcStat g_pppHdlcHardStat;

/* ������ԭʼ�ж�ʱ��RAW_INT��STATUSֵ */
extern HDLC_RegSaveInfo g_hdlcRegSaveInfo;

extern VOS_BOOL PPP_HDLC_HardMntnGetConfig(VOS_UINT32 mod);
extern VOS_VOID PPP_HDLC_HardMntnTraceMsg(
    HDLC_MNTN_TraceHead *head, HDLC_MNTN_EventTypeUint32 msgname, VOS_UINT32 dataLen);
extern VOS_VOID PPP_HDLC_HardMntnTraceSingleData(VOS_UINT16 dataLen, const VOS_UINT8 *dataAddr,
    HDLC_MNTN_EventTypeUint32 eventType, VOS_UINT32 nodeIndex);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
