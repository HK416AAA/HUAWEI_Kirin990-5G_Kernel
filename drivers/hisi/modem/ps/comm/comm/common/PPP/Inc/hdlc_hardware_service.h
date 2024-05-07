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

#ifndef __HDLC_HRADWARE_SERVICE_H__
#define __HDLC_HRADWARE_SERVICE_H__

#include "hdlc_hardware_mntn.h"
#include "hdlc_hardware_conf.h"
#include "hdlc_hardware_driver.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ���װ�������������������ڵ����,�޸Ļ�Ӱ��HDLC_INPUT_PARA_LINK_MAX_SIZE��ֵ */
#define TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM 20

/* ���װ��������ڴ��С�ͻ���ַ,HDLC_DEF_INPUT_PARA_LINK_MAX_NUM * sizeof(HDLC_PARA_LINK_NODE_STRU) */
#define TTF_HDLC_DEF_INPUT_PARA_LINK_BUF_LEN (TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM * 12)

/* ���װ��������������������ܳ��� */
#define TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_SIZE (13 * 1024)

/* һ�ν��װ���IP��/��PPP֡������ = һ�ν��װ������볤��/��СIP������(20B) */
#define TTF_HDLC_DEF_OUTPUT_MAX_NUM (TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_SIZE / 20)

/* ���װ��Ϣ�ϱ������� */
#define TTF_HDLC_DEF_RPT_MAX_NUM (TTF_HDLC_DEF_OUTPUT_MAX_NUM)

/* ���װ��Ϣ�ϱ��ռ��ڴ��С�ͻ���ַһ�ν��װ��������IP��/��PPP֡����*sizeof(HDLC_FRM_RPT_NODE) */
#define TTF_HDLC_DEF_RPT_BUF_LEN (TTF_HDLC_DEF_RPT_MAX_NUM * 8)

/*
 * ((TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_SIZE / 6 + 1) * 8)
 * 6Ϊռ�����ռ����С��Ч֡
 * 8Ϊ���ֽڶ���
 * +1 ΪС������ȡ��
 */
#define TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN (18 * 1024)

/* ��HDLC�豸ʹ�õ��ڴ��ܳ��ȣ����ṹ��HDLC_DEF_BUFF_INFO_STRU�仯ʱ���ú�Ӧ����Ӧ�仯 */
#define TTF_HDLC_DEF_BUF_LEN \
    (TTF_HDLC_DEF_INPUT_PARA_LINK_BUF_LEN + TTF_HDLC_DEF_RPT_BUF_LEN + TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN)

/* ��װ�������������������ڵ�������޸Ļ�Ӱ��HDLC_INPUT_PARA_LINK_MAX_SIZE��ֵ */
#define TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM 20

/* ��װ��������������������ܳ��� */
#define TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_SIZE (15 * 1024)

/* ��װ��������ڴ��С = TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM * sizeof(HDLC_PARA_LINK_NODE_STRU) */
#define TTF_HDLC_FRM_INPUT_PARA_LINK_BUF_LEN (TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM * 12)

/* ��װ��Ϣ������ */
#define TTF_HDLC_FRM_RPT_MAX_NUM (TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM)

/* ��װ��Ϣ�ϱ��ռ��ڴ��С�ͻ���ַ,TTF_HDLC_FRM_RPT_MAX_NUM * sizeof(HDLC_DEF_RPT_NODE) */
#define TTF_HDLC_FRM_RPT_BUF_LEN (TTF_HDLC_FRM_RPT_MAX_NUM * 8)

/* ��װ�������������������ڵ���� */
#define TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM (TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM * 2)

/* ��װ��������ڴ��С�ͻ���ַ,TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM * sizeof(HDLC_PARA_LINK_NODE_STRU) */
#define TTF_HDLC_FRM_OUTPUT_PARA_LINK_BUF_LEN (TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM * 12)

/* ��HDLC�豸ʹ�õ��ڴ��ܳ��ȣ����ṹ��HDLC_FRM_BUFF_INFO_STRU�仯ʱ���ú�Ӧ����Ӧ�仯 */
#define TTF_HDLC_FRM_BUF_LEN \
    (TTF_HDLC_FRM_INPUT_PARA_LINK_BUF_LEN + TTF_HDLC_FRM_RPT_BUF_LEN + TTF_HDLC_FRM_OUTPUT_PARA_LINK_BUF_LEN)

/* ��HDLC�豸ʹ�õ��ڴ��ܳ��� */
#define TTF_HDLC_LINK_TOTAL_LEN \
    (TTF_GET_32BYTE_ALIGN_VALUE((TTF_HDLC_DEF_BUF_LEN + TTF_HDLC_FRM_BUF_LEN)))

/* ���ڽ��װ���ܳ����ƻ��������һ֡���ݿ��ܻᱻ�ֳ�2�� */
#define PPP_SERVICE_HDLC_HARD_DEF_MAX_DATA_CNT 2

/* LCPЭ�� */
#define PPP_SERVICE_HDLC_HARD_LCP 0xc021

/* ����PPP ID��ȡ��Ӧ�ڴ棬��ʱֻ��һ·����Ϊ��ֵ�����������ж�· */
#define HDLC_DEF_GET_UNCOMPLETED_INFO(pppId) (&g_uncompletedInfo)
#define HDLC_DEF_GET_BUF_INFO(pppId) (g_hdlcDefBufInfo)
#define HDLC_FRM_GET_BUF_INFO(pppId) (g_hdlcFrmBufInfo)

/* ���ṹ���С�Ƿ�Ϸ� */
#if (defined(LLT_TEST_CODE))
#define PPP_SERVICE_HDLC_HARD_STRU_SIZE_CHECK(size, stru) do { \
    char STRU_SIZE_CHECK_##stru[((size) == sizeof(stru)) ? 1 : -1]; \
    (void)STRU_SIZE_CHECK_##stru[0];                                \
} while (0)
#else
#define PPP_SERVICE_HDLC_HARD_STRU_SIZE_CHECK(size, stru)
#endif

/* TTF_MAX(TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM, TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM) */
#define HDLC_INPUT_PARA_LINK_MAX_SIZE 20
#define HDLC_OUTPUT_PARA_LINK_MAX_SIZE (TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM)

/* ���װ����ռ��ס�β��ַ�������ж��Ƿ��ƻ� */
#define HDLC_DEF_OUTPUT_BUF_START_ADDR ((VOS_UINT8*)(&g_hdlcDefBufInfo->outputDataBuf[0]))
#define HDLC_DEF_OUTPUT_BUF_END_ADDR (HDLC_DEF_OUTPUT_BUF_START_ADDR + TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN)

/* Լ����Ӳ��, ��װһ������ύ1502�ֽڸ�Framer */
#define HDLC_FRM_IN_PER_MAX_CNT (1502L)
#define HDLC_FRM_OUT_PER_MAX_CNT (3013UL)

/* ��װ���ڴ����ֵ */
#define HDLC_FRM_GET_MAX_FRAMED_LEN(len) (2 * len + 13)

/* ���װ�ϱ�ÿ֡�1502�ֽ�, IPv4�1500�ֽ�, PPPģʽ����Э���ֶ�2�ֽ�, ��1502�ֽ� */
#define HDLC_DEF_OUT_PER_MAX_CNT (1502UL)

/* �����װĿ�Ŀռ�ʧ�ܶ�ʱ��ʱ��,��λ���� */
#define HDLC_FRM_TIME_INTERVAL 100

/* ���ķ�װ�󳤶�����Ϊԭ������������˷�װһ�����������Ҫ2���ڴ����洢��װ��� */
#define PPP_HDLC_MAX_OUT_NODE_FOR_ONE_INPUT 2

/* ���װ��������� */
#define PPP_SERVICE_HDLC_HARD_DEF_RES_PROC(rslt) g_hldcDefCallBackInfo.defResProcFunc(rslt)

/* ���װ������Ϣ������ */
#define PPP_SERVICE_HDLC_HARD_DEF_ERR_PROC(pppId, errCnt) g_hldcDefCallBackInfo.defErrProcFunc(pppId, errCnt)

/* HDLC����״̬ */
enum PPP_ServiceHdlcHardWorkStatus {
    PPP_SERVICE_HDLC_HARD_NOT_WORK = 0,
    PPP_SERVICE_HDLC_HARD_FRM_WORK,
    PPP_SERVICE_HDLC_HARD_DEF_WORK,
    PPP_SERVICE_HDLC_HARD_BOTH_WORK,
    PPP_SERVICE_HDLC_HARD_WORK_STATUS_BUTT
};

typedef VOS_UINT8 PPP_ServiceHdlcHardWorkStatusUint8;

/* HDLC����ģʽ */
enum PPP_ServiceHdlcHardMode {
    PPP_SERVICE_HDLC_HARD_IP_MODE  = 0,
    PPP_SERVICE_HDLC_HARD_PPP_MODE = 1,
    PPP_SERVICE_HDLC_HARD_MODE_BUTT
};

typedef VOS_UINT8 PPP_ServiceHdlcHardModeUint8;

/* HDLC������ */
enum PPP_HDLC_HardProcResult {
    PPP_HDLC_HARD_PROC_ALL_SUCC = 0,
    PPP_HDLC_HARD_PROC_PART_SUCC,
    PPP_HDLC_HARD_PROC_STATUS_ERR,
    PPP_HDLC_HARD_PROC_PARA_ERR,
    PPP_HDLC_HARD_PROC_FAIL,
    PPP_HDLC_HARD_PROC_RESULT_BUTT
};

typedef VOS_UINT32 PPP_HDLC_HardProcResultUint32;

/* HDLC���������ö�� */
enum PPP_HDLC_ParaCheckResult {
    PPP_HDLC_PARA_CHECK_PASS         = 0, /* ��������Ҫ�󣬽������ݳ��� */
    PPP_HDLC_PARA_CHECK_FAIL_DISCARD = 1, /* ���ݰ�����������Ҫ����Ҫ���������Ǽ���������һ�����ݰ� */
    PPP_HDLC_PARA_CHECK_FAIL_KEEP    = 2, /* ���ݰ�����������Ҫ�󣬵���Ҫ�����������´δ��� */

    PPP_HDLC_PARA_CHECK_BUTT
};
typedef VOS_UINT32 PPP_HDLC_ParaCheckResultUint32;

/*
 * ��������ڵ�ṹ:
 *   �����֤���װ�������������ʼ��ַWord���룻(�μ�HiHDLCV200�߼�������˵����)��
 *   ���ϵ��£���ַ���ӣ��ֱ�Ϊ�ڵ��ڴ�ָ�롢�ڵ㳤�ȡ��¸��ڵ��ַ��
 *   ����LEN��Ч16bits���ڵ�Half_Word����ַADDR 32bits
 */
typedef struct tagHDLC_ParaLinkNode {
    HDLC_ADDR_TYPE dataAddr; /* �ò����ڵ�ָ���ڴ��ַ  */
    VOS_UINT16     dataLen;  /* �ڴ泤�ȣ���λByte */
    VOS_UINT8      reserve[2];
    HDLC_ADDR_TYPE nextNode; /* ָ����һ�������ڵ��ָ�� */
} HDLC_ParaLinkNode;

/*
 * ���װ�ϱ��ռ���ṹ:
 *     ����֤���װ�ϱ��ռ���ʼ��ַWord���룻(�μ�HiHDLCV200�߼�������˵����)��
 *     ���ϵ��£���ַ���ӣ��ֱ�Ϊ�������Э�����͡����Ⱥʹ洢��ַ��
 *     ����LEN��Ч16bits���ڵ�Half_Word��Э��PRO��Ч16bits���ڸ�Half-Word����ַADDR 32bits
 */
typedef struct {
    VOS_UINT16     defOutOneLen;  /* ��Ч֡�ĳ��� */
    VOS_UINT16     defOutOnePro;  /* ��Ч֡��Э�� */
    HDLC_ADDR_TYPE defOutOneAddr; /* ָ����װ������Ч֡����洢����ʼ��ַ */
} HDLC_DefRptNode;

/* ������װʹ�õ��ڴ� */
typedef struct {
    HDLC_ParaLinkNode inputParaLinkNodeBuf[TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM]; /* ���װ�����������ʹ�õ��ڴ� */
    HDLC_DefRptNode   rptNodeBuf[TTF_HDLC_DEF_RPT_MAX_NUM];                       /* ���װ��Ϣ�ϱ��ռ�ʹ�õ��ڴ� */
    VOS_UINT8         outputDataBuf[TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN];            /* ���װĿ�Ŀռ�ʹ�õ��ڴ� */
} HDLC_DefBuffInfo;

/*
 * ��װ�ϱ��ռ���ṹ:
 *     �����֤��װ�ϱ��ռ���ʼ��ַWord���룻(�μ�HiHDLCV200�߼�������˵����)��
 *     ���ϵ��£���ַ���ӣ��ֱ�Ϊ���������ʼ��ַ�����ݳ��ȣ�
 */
typedef struct {
    HDLC_ADDR_TYPE frmOutOneAddr; /* ָ���װ������Ч֡������洢��ʼ��ַ */
    VOS_UINT16     frmOutOneLen;  /* ��Ч֡�ĳ��� */
    VOS_UINT8      reserve[2];
} HDLC_FrmRptNode;

/* �����װʹ�õ��ڴ� */
typedef struct {
    HDLC_ParaLinkNode inputParaLinkNodeBuf[TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM];   /* ��װ�����������ʹ�õ��ڴ� */
    HDLC_FrmRptNode   rptNodeBuf[TTF_HDLC_FRM_RPT_MAX_NUM];                         /* ��װ��Ϣ�ϱ��ռ�ʹ�õ��ڴ� */
    HDLC_ParaLinkNode outputParaLinkNodeBuf[TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM]; /* ��װ�����������ʹ�õ��ڴ� */
} HDLC_FrmBuffInfo;

/* HDLC��װ�����װ�ڵ��ڴ���� */
typedef struct {
    VOS_UINT8 *dataAddr; /* ָ�����ݵ��ڴ��ַ */
    VOS_UINT16 dataLen;  /* ���ݳ��� */
    VOS_UINT8  reserve[2];
    VOS_VOID  *dataNode; /* �˲�������ҵ��ģ���ݴ���������ڵ���Ϣ��Service��ʹ�ô˲��� */
} PPP_HDLC_HardNodePara;

/* HDLC��װ���� */
typedef struct {
    VOS_UINT16                   pppId;                                                 /* PPP ID */
    VOS_UINT16                   protocol;                                              /* Э������ */
    PPP_HDLC_HardNodePara        inputNodePara[TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM];   /* ��װ����ڵ���� */
    VOS_UINT32                   inputNodeParaCnt;                                      /* ��װ����ڵ�������� */
    VOS_UINT32                   inputTotalSize;                                        /* ���װ����ڵ��ܴ�С */
    PPP_HDLC_HardNodePara        outputNodePara[TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM]; /* ��װ����ڵ���� */
    VOS_UINT32                   outputNodeParaCnt;                                     /* ��װ����ڵ�������� */
    VOS_UINT32                   pppAcFlag;                                             /* PPP�ĵ�ַ������ѹ����ʶ */
    VOS_UINT32                   pppPcFlag;                                             /* PPP��Э����ѹ����ʶ */
    VOS_UINT32                   accmFlag;                                              /* PPP��ͬ�첽�����ֱ�ʶ */
    PPP_ServiceHdlcHardModeUint8 pppMode;                                               /* PPP����ģʽ */
    VOS_UINT8                    reserve[3];
} PPP_HDLC_HardFrmPara;

/* HDLC��װ��� */
typedef struct {
    VOS_UINT8             frmValidCnt; /* �ɹ���װ���ĸ��� */
    PPP_HDLC_HardNodePara frmResultNode[TTF_HDLC_FRM_RPT_MAX_NUM];
} PPP_HDLC_HardFrmResult;

/* HDLC���װ������Ϣ */
typedef struct {
    VOS_UINT16 dataLen; /* ���װ���ݳ��� */
    VOS_UINT16 reserve;
    VOS_UINT8 *dataAddr; /* ���װ���ݵ�ַ */
} PPP_HDLC_HardDefDataInfo;

/* HDLC���װ��� */
typedef struct {
    VOS_UINT16   pppId;                                                        /* PPP ID */
    VOS_UINT16   protocol;                                                     /* Э������ */
    PS_BoolUint8 segment;                                                      /* �����Ƿ񱻷ֶ� */
    PPP_HDLC_HardDefDataInfo dataInfo[PPP_SERVICE_HDLC_HARD_DEF_MAX_DATA_CNT]; /* ���ڽ��װ������ܳ����ƻأ�����п���һ֡���ݱ��ֳ����� */
    VOS_UINT32 para;                                                           /* ҵ��ģ��͸������ */
} PPP_HDLC_HardDefResult;

/* ҵ��ģ�鴦��HDLC���װ������� */
typedef VOS_VOID (*PPP_ServiceHdlcHardDefResProcFunc)(PPP_HDLC_HardDefResult*);

/* ҵ��ģ�鴦��HDLC���װ������Ϣ���� */
typedef VOS_VOID (*PPP_ServiceHdlcHardDefErrInfoProcFunc)(VOS_UINT16 pppId, PPP_DRIVER_HdlcHardDefErrFramesCnt *errCnt);

/* ҵ��ģ����װ�ص��ṹ */
typedef struct {
    PPP_ServiceHdlcHardDefResProcFunc     defResProcFunc; /* ���װ��������� */
    PPP_ServiceHdlcHardDefErrInfoProcFunc defErrProcFunc; /* ���װ������Ϣ������ */
} PPP_ServiceHdlcHardDefCallBack;

/* HDLC���װ���� */
typedef struct {
    VOS_UINT16                   pppId;   /* PPP ID */
    PPP_ServiceHdlcHardModeUint8 pppMode; /* PPP����ģʽ */
    VOS_UINT8                    reserve;
    VOS_UINT32                   para; /* ҵ��ģ��͸���������ڽ���д��ظ�ҵ��ģ�� */
    PPP_HDLC_HardNodePara        inputNodePara[TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM]; /* ���װ����ڵ���� */
    VOS_UINT32                   inputNodeParaCnt;                                    /* ���װ���뼰�ڵ�������� */
    VOS_UINT32                   inputTotalSize;                                      /* ���װ����ڵ��ܴ�С */
    VOS_UINT32                   pppAcFlag;                                           /* PPP�ĵ�ַ������ѹ����ʶ */
    VOS_UINT32                   pppPcFlag;                                           /* PPP��Э����ѹ����ʶ */
} PPP_HDLC_HardDefPara;

/* ���װ������֡��ά�ɲ�ṹ */
typedef struct {
    HDLC_MNTN_TraceHead                   head; /* _H2ASN_Skip */
    PPP_DRIVER_HdlcHardDefUncompletedInfo uncompletedInfo;
} HDLC_MNTN_DefUncompletedInfo;

/* ������������ά�ɲ�ṹ */
typedef struct {
    HDLC_MNTN_TraceHead head;             /* _H2ASN_Skip */
    VOS_UINT32          inputLinkNodeCnt; /* �����������ڵ���� */
    HDLC_ParaLinkNode   inputParaLinkNodeBuf[HDLC_INPUT_PARA_LINK_MAX_SIZE];
} HDLC_MNTN_InputParaLink;

/* ��װ��������ά�ɲ�ṹ */
typedef struct {
    HDLC_MNTN_TraceHead head; /* _H2ASN_Skip */
    VOS_UINT8           frmValidNum;
    VOS_UINT8           reserve1[1];
    VOS_UINT16          outputNodeUsedCnt;
    VOS_UINT32          outputLinkNodeCnt; /* �����������ڵ������ֻ�ڷ�װ��Ч */
    HDLC_ParaLinkNode   outputParaLinkNodeBuf[TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM]; /* ��װ�����������ʹ�õ��ڴ� */
    HDLC_FrmRptNode     rptNodeBuf[TTF_HDLC_FRM_RPT_MAX_NUM];                         /* ��װ��Ϣ�ϱ��ռ�ʹ�õ��ڴ� */
} HDLC_MNTN_FrmOutputPara;

/* ���װ��������ά�ɲ�ṹ */
typedef struct {
    HDLC_MNTN_TraceHead head; /* _H2ASN_Skip */
    VOS_UINT16          defValidNum;
    VOS_UINT16          traceNum;
} HDLC_MNTN_DefOutputPara;

extern VOS_UINT32 PPP_ServiceHdlcHardInit(
    PPP_HDLC_HardConfigInfo *hdlcConfig, PPP_ServiceHdlcHardDefCallBack *defCBInfo);
extern VOS_VOID                      PPP_ServiceHdlcHardDisable(VOS_VOID);
extern VOS_VOID                      PPP_ServiceHdlcHardSetUp(VOS_UINT16 pppId);
extern VOS_UINT32                    PPP_ServiceHdlcHardGetMaxDefLen(VOS_UINT16 pppId);
extern PPP_HDLC_HardProcResultUint32 PPP_ServiceHdlcHardFrmPacket(
    PPP_HDLC_HardFrmPara *frmPara, PPP_HDLC_HardFrmResult *frmResult);
extern PPP_HDLC_HardProcResultUint32      PPP_ServiceHdlcHardDefPacket(PPP_HDLC_HardDefPara *defPara);
extern VOS_VOID                           PPP_ServiceHdlcHardOpenClk(VOS_VOID);
extern VOS_VOID                           PPP_ServiceHdlcHardCloseClk(VOS_VOID);
extern PPP_ServiceHdlcHardWorkStatusUint8 PPP_ServiceHdlcHardGetWorkStatus(VOS_VOID);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
