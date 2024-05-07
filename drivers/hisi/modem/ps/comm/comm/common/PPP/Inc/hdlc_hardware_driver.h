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

#ifndef __HDLC_HRADWARE_DRIVER_H__
#define __HDLC_HRADWARE_DRIVER_H__

#include "vos.h"
#include "hdlc_hardware_mntn.h"
#include "soc_hdlc_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ����Ĵ�����32bit Reg�е�bitλ��:�Ĵ���frm_en��bit0 */
#define HDLC_FRM_EN_BITPOS 0UL

/* ����Ĵ�����32bit Reg�е�bitλ��: �Ĵ���def_en��bit0 */
#define HDLC_DEF_EN_BITPOS 0UL

/* ����Ĵ�����32bit Reg�е�bitλ��:�Ĵ���frm_in_lli_1dor2d��bit0;�Ĵ���frm_acfc��bit1 */
#define HDLC_FRM_IN_LLI_1DOR2D_BITPOS 0UL
#define HDLC_FRM_ACFC_BITPOS 1UL
#define HDLC_FRM_PFC_BITPOS 2UL

/*
 * �Ĵ���frm_pfc(2bits)��ֵ����:
 * 00b: Ӳ��ģ�����P��, P����ѹ��;
 * 01b: Ӳ��ģ�����P��, P��ѹ��;
 * 11b: Ӳ��ģ�鲻���P��;
 */
#define HDLC_PROTOCOL_ADD_WITHOUT_COMPRESS 0x00
#define HDLC_PROTOCOL_ADD_WITH_COMPRESS 0x01
#define HDLC_PROTOCOL_NO_ADD 0x03

/*
 * ����Ĵ�����32bit Reg�е�bitλ��:
 * �Ĵ���def_uncompleted_ago��bit0;
 * �Ĵ���def_acfc��bit1;
 */
#define HDLC_DEF_IS_UNCOMPLETED_AGO_BITPOS 0UL
#define HDLC_DEF_ACFC_BITPOS 1UL
#define HDLC_DEF_PFC_BITPOS 2UL

/* �Ĵ���frm_protocol(16bits)��MASK�� */
#define HDLC_FRM_PROTOCOL_MASK 0x0000FFFF

/*
 * �Ĵ���def_pfc(2bits)��ֵ����:
 * 00b: P����ѹ��, �����;
 * 01b: P��ѹ��, �����;
 * 11b: P�򲻰���;
 */
#define HDLC_PROTOCOL_REMOVE_WITHOUT_COMPRESS 0x00
#define HDLC_PROTOCOL_REMOVE_WITH_COMPRESS 0x01
#define HDLC_PROTOCOL_NO_REMOVE 0x03

/*
 * �Ĵ���hdlc_prior_ctrl(2bits)��ֵ����:
 * 00b: ˭�ȱ�ʹ�����Ƚ�˭������;
 * 01b: ���з�װ���ȼ���;
 * 10b: ���н��װ���ȼ���;
 * 11b: ��Ч;
 */
#define HDLC_PRIOR_FCFS 0x00
#define HDLC_PRIOR_DL_FRM_HIGHER 0x01
#define HDLC_PRIOR_UL_DEF_HIGHER 0x02
#define HDLC_PRIOR_INVALID 0x03

/* Boston�汾�����÷�װ���װ��Ϣ����󳤶ȼĴ���0x5DF(1503)Bytes */
#define HDLC_MAX_FRM_DEF_INFO_LEN 0x000005DFU

/* ��װ״̬�Ĵ�����ַ hdlc_frm_status (0x28) begin,��װģ��״̬�Ĵ�����ַ: ���ƫ�Ƶ�ַ��0x28 */

/* �Ĵ���frm_out_seg_num(16bits)��MASK�� */
#define HDLC_FRM_OUT_SEG_NUM_MASK 0x0000FFFF

/* �Ĵ���frm_valid_num(8bits)��MASK�� */
#define HDLC_FRM_VALID_NUM_MASK 0x000000FF

/* �Ĵ���frm_all_pkt_done(1bit)��MASK�� */
#define HDLC_FRM_ALL_PKT_DONE_MASK 0x00000001

/* ��ѯ��װ״̬��Ϣ������ */
#define HDLC_FRM_STATUS_MASK 0x00000003

/*
 * �Ĵ���frm_block_done([0]λ)��ֵ����:
 * 0b: δ���һ���������ݴ���;
 * 1b: ���һ���������ݴ���;
 * �Ĵ���frm_error_index([1]λ)��ֵ����:
 * 1b: ��װ��������;
 */
#define HDLC_FRM_ALL_PKT_DOING 0x00
#define HDLC_FRM_ALL_PKT_DONE 0x01
#define HDLC_FRM_STOP_EXCEPTION_OCCUR 0x02
#define HDLC_FRM_DONE_EXCEPTION_OCCUR 0x03

/* ���װ״̬�Ĵ�����ַdlc_def_status (0x88) begin,�Ĵ���dlc_def_status(5bits)��MASK�� */
#define HDLC_DEFRAMER_BLOCK_STATUS_MASK 0x0000001B

/* ��ѯ���װ״̬��Ϣ������ */
#define HDLC_DEF_STATUS_MASK 0x0000003B

/*
 * �Ĵ���dlc_def_status(5bits)��ֵ����:
 * 000000b: δ���һ�����ݴ���;
 * 010000b: δ���һ�����ݴ������װ�ⲿ��ȷ֡��Ϣ�ϱ��ռ������ͣ;
 * 001000b: δ���һ�����ݴ���, ���װ�ⲿ����洢�ռ������ͣ;
 * 000001b: δ���һ�����ݴ���, �ѽ��LCP֡, Ӳ��������ͣ״̬;
 * 000010b: ���һ�����ݴ���, ����֡�ϱ�;
 * 000011b: ���һ�����ݴ���, ����֡�ϱ�;
 * 1xx0xxb: ���쳣����;
 */
#define HDLC_DEF_STATUS_DOING 0x00
#define HDLC_DEF_STATUS_PAUSE_RPT_SPACE_FULL 0x10
#define HDLC_DEF_STATUS_PAUSE_OUTPUT_SPACE_FULL 0x08
#define HDLC_DEF_STATUS_PAUSE_LCP 0x01
#define HDLC_DEF_STATUS_DONE_WITHOUT_FRAMES 0x02
#define HDLC_DEF_STATUS_DONE_WITH_FRAMES 0x03

/* ���װ�Ƿ��з�����֡��ʶ */
#define HDLC_DEF_UNCOMPLETED_NOT_EXIST 0x0
#define HDLC_DEF_UNCOMPLETED_EXIST 0x1

/* ��ѯ��װ����װ��ɵĴ��� */
#define HDLC_FRM_MAX_WAIT_RESULT_NUM 0xFFFF
#define HDLC_DEF_MAX_WAIT_RESULT_NUM 0xFFFF

/* �ȴ���װ����װ��Ӧ�ж�ʱ��,�Ժ���Ϊ��λ */
#define HDLC_FRM_INT_TIMER_LEN 5000
#define HDLC_DEF_INT_TIMER_LEN 5000

/* ������ѯ�����ж�ʹ�ܵ�ˮ�ߣ�Ĭ��ֵ */
#define HDLC_DEF_INTERRUPT_LIMIT_DEFAULT (2 * 1024)
#define HDLC_FRM_INTERRUPT_LIMIT_DEFAULT (2 * 1024)

/* AXI���߶�д����ʱ���ж�ֵ����ֵ��SoC�ṩ������������� */
#define HDLC_AXI_REQ_TIMEOUT_VALUE 255

/* HDLC��ַ������ѹ�� */
#define HDLC_ADDRESS_CTRL_NOCOMPRESS 0
#define HDLC_ADDRESS_CTRL_COMPRESS 1

/* HDLCӲ�����װ����zָʾBITλ */
#define PPP_DRIVER_HDLC_HARD_DEF_FCS_ERR 0
#define PPP_DRIVER_HDLC_HARD_DEF_FRAME_TOO_LONG 1
#define PPP_DRIVER_HDLC_HARD_DEF_FRAME_TOO_SHORT 2
#define PPP_DRIVER_HDLC_HARD_DEF_PROTOCOL_ERR 3
#define PPP_DRIVER_HDLC_HARD_DEF_CTRL_ERR 4
#define PPP_DRIVER_HDLC_HARD_DEF_ADDR_ERR 5
#define PPP_DRIVER_HDLC_HARD_DEF_FLAG_POS_ERR 6

/* ��ȡHDLC DRIVER������Ϣ */
#define PPP_DRIVER_HDLC_HARD_GET_CONFIG (&g_hdlcConfigInfo)

/* ��ȡHDLC DRIVER��װ������Ϣ */
#define PPP_DRIVER_HDLC_HARD_GET_FRM_CONF (&(PPP_DRIVER_HDLC_HARD_GET_CONFIG->frmConfig))

/* ��ȡHDLC DRIVER���װ������Ϣ */
#define PPP_DRIVER_HDLC_HARD_GET_DEF_CONF (&(PPP_DRIVER_HDLC_HARD_GET_CONFIG->defConfig))

/* ������ѯ�����ж�ʹ�ܵ�ˮ�� */
#define HDLC_DEF_INTERRUPT_LIMIT (PPP_DRIVER_HDLC_HARD_GET_CONFIG->hdlcDefIntLimit)
#define HDLC_FRM_INTERRUPT_LIMIT (PPP_DRIVER_HDLC_HARD_GET_CONFIG->hdlcFrmIntLimit)

/* HDLC IP����ַ */
#define HDLC_IP_BASE_ADDR (PPP_DRIVER_HDLC_HARD_GET_CONFIG->hdlcIpBaseAddr)

/* ֹͣʱ��ǰ���ȴ�HDLC������ɵ������� */
#define HDLC_CHECK_HARDWAWRE_STATE_MAX_TIMES 100

/* Ӳ��������״̬�Ĵ��� */
#define HDLC_SYSCTRL_STAT_ADDR(base) ((base) + SOC_HDLC_SYSCTRL_STAT_OFFSET)

/* hdlcӲ����ַ���ͣ�Ŀǰʹ��32bit��ַ���� */
typedef VOS_UINT32 HDLC_ADDR_TYPE;

enum PPP_DRIVER_HdlcReg {
    PPP_DRIVER_MAX_FRM_INFO_REG = 0x00,
    PPP_DRIVER_MAX_DEF_INFO_REG = 0x01,

    PPP_DRIVER_HDLC_REG_BUTT
};
typedef VOS_UINT8 PPP_DRIVER_HdlcRegUint8;

/* һ�����ݽ���Ĵ���֡��ͳ�ƽṹ */
typedef struct {
    VOS_UINT16 fcsErrCnt;      /* FCS���� */
    VOS_UINT16 lenLongCnt;     /* ֡���ȹ���, ����1502�ֽ� */
    VOS_UINT16 lenShortCnt;    /* ֡���ȹ���, ����4�ֽ� */
    VOS_UINT16 errProtocolCnt; /* Э���ֶβ�����xxxxxxx0, xxxxxxx1����ʽ */
    VOS_UINT16 errCtrlCnt;     /* �����ֶβ�Ϊ0xFF */
    VOS_UINT16 errAddrCnt;     /* ��ַ�ֶβ�Ϊ0x03 */
    VOS_UINT16 flagPosErrCnt;  /* 0x7D�������0x7E */
    VOS_UINT8  errType;        /* �������� */
    VOS_UINT8  reserve;
} PPP_DRIVER_HdlcHardDefErrFramesCnt;

/* HDLC��װ���������Ϣ�ṹ */
typedef struct {
    VOS_VOID  *inputAddr;  /* ���������ַ */
    VOS_VOID  *outputAddr; /* ��������ַ */
    VOS_VOID  *reportAddr; /* ��װ����ϱ���ַ */
    VOS_UINT32 rptBufLen;  /* �ϱ��ռ��С */
} PPP_DRIVER_HdlcHardFrmConfig;

/* HDLC���װ���������Ϣ�ṹ */
typedef struct {
    VOS_VOID  *inputAddr;    /* ���������ַ */
    VOS_VOID  *outputAddr;   /* ��������ַ */
    VOS_VOID  *reportAddr;   /* ��װ����ϱ���ַ */
    VOS_UINT32 outputBufLen; /* ����ռ��С */
    VOS_UINT32 reportBufLen; /* �ϱ��ռ��С */
    VOS_UINT32 perInMaxLen;  /* ����������󵥰����� */
} PPP_DRIVER_HdlcHardDefConfig;

/* HDLC���������Ϣ�ṹ */
typedef struct {
    VOS_UINT_PTR                 hdlcIpBaseAddr;   /* �ӵ����ȡ��HDLC����ַ */
    VOS_SEM                      hdlcDefSem;       /* ���װ�ź��� */
    VOS_SEM                      hdlcFrmSem;       /* ��װ�ź��� */
    VOS_INT32                    hdlcIsrDef;       /* ���װ�жϺ� */
    VOS_INT32                    hdlcIsrFrm;       /* ��װ�жϺ� */
    VOS_UINT32                   hdlcDefIntLimit;  /* ���װ�ж�ˮ�� */
    VOS_UINT32                   hdlcFrmIntLimit;  /* ��װ�ж�ˮ�� */
    PPP_DRIVER_HdlcHardFrmConfig frmConfig;        /* ��װ������Ϣ */
    PPP_DRIVER_HdlcHardDefConfig defConfig;        /* ���װ������Ϣ */
} PPP_DRIVER_HdlcHardConfigInfo;

/* HDLC��װ�����ṹ */
typedef struct {
    VOS_UINT32 inputTotalSize; /* ���������ܴ�С�������ж���ʹ���жϻ�����ѵ��ʽ */
    VOS_UINT32 accmFlag;       /* ͬ�첽������ */
    VOS_UINT32 pppAcFlag;      /* ��ַ������ѹ����Ϣ */
    VOS_UINT32 pppPcFlag;      /* Э����ѹ����Ϣ */
    VOS_UINT16 protocol;       /* Э������ */
    VOS_UINT8  reserve[2];
} PPP_DRIVER_HdlcHardFrmPara;

/* HDLC���װ������֡��Ϣ�洢�ṹ */
typedef struct {
    VOS_UINT8      existFlag; /* ������֡�Ƿ���ڱ�ʶ */
    VOS_UINT8      reserve1[3];
    VOS_UINT16     defOutOnePro;     /* ������֡��Э�� */
    VOS_UINT16     defOutOneLen;     /* ������֡�ĳ��� */
    HDLC_ADDR_TYPE defOutOneAddr;    /* ָ����װ���ķ�����֡����洢����ʼ��ַ */
    VOS_UINT32     defStAgo;         /* ���ڱ���def_uncomplet_st_now�Ĵ���ֵ */
    VOS_UINT32     defInfoFrlCntAgo; /* ���ڱ���def_info_frl_cnt_ago�Ĵ���ֵ */
} PPP_DRIVER_HdlcHardDefUncompletedInfo;

/* HDLC���װ�����ṹ */
typedef struct {
    VOS_UINT32                             inputTotalSize; /* ���������ܴ�С�������ж���ʹ���жϻ�����ѵ��ʽ */
    VOS_UINT32                             pppPcFlag;      /* Э������Ӽ�ѹ����Ϣ */
    VOS_UINT32                             pppAcFlag;      /* ��ַ������ѹ����Ϣ */
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo; /* �ϴν��װ�Ĳ�����֡��Ϣ */
} PPP_DRIVER_HdlcHardDefPara;

/* ��װ�Ĵ�����Ϣ */
typedef struct {
    VOS_UINT32 stateSwRst;
    VOS_UINT32 priorTimeoutCtrl;
    VOS_UINT32 rdErrCurrAddr;
    VOS_UINT32 wrErrCurrAddr;
    VOS_UINT32 hdlcFrmEn;
    VOS_UINT32 hdlcFrmRawInt;
    VOS_UINT32 hdlcFrmIntStatus;
    VOS_UINT32 hdlcFrmIntClr;
    VOS_UINT32 hdlcFrmCfg;
    VOS_UINT32 hdlcFrmAccm;
    VOS_UINT32 hdlcFrmStatus;
    VOS_UINT32 frmInLliAddr;
    VOS_UINT32 frmInSublliAddr;
    VOS_UINT32 frmInPktLen;
    VOS_UINT32 frmInBlkAddr;
    VOS_UINT32 frmInBlkLen;
    VOS_UINT32 frmOutLliAddr;
    VOS_UINT32 frmOutSpaceAddr;
    VOS_UINT32 frmOutSpaceDep;
    VOS_UINT32 frmRptAddr;
    VOS_UINT32 frmRptDep;
} PPP_DRIVER_HdlchardFrmRegInfo;

/* ���װ�Ĵ�����Ϣ */
typedef struct {
    VOS_UINT32 stateSwRst;
    VOS_UINT32 priorTimeoutCtrl;
    VOS_UINT32 rdErrCurrAddr;
    VOS_UINT32 wrErrCurrAddr;
    VOS_UINT32 hdlcDefEn;
    VOS_UINT32 hdlcDefRawInt;
    VOS_UINT32 hdlcDefIntStatus;
    VOS_UINT32 hdlcDefIntClr;
    VOS_UINT32 hdlcDefCfg;
    VOS_UINT32 defUncompletedLen;
    VOS_UINT32 defUncompletedPro;
    VOS_UINT32 defUncompletedAddr;
    VOS_UINT32 defUncompleteStAgo;
    VOS_UINT32 hdlcDefGoOn;
    VOS_UINT32 hdlcDefStatus;
    VOS_UINT32 defUncompletStNow;
    VOS_UINT32 defInLliAddr;
    VOS_UINT32 defInPktAddr;
    VOS_UINT32 defInPktLen;
    VOS_UINT32 defInPktLenMax;
    VOS_UINT32 defOutSpcAddr;
    VOS_UINT32 defOutSpaceDep;
    VOS_UINT32 defRptAddr;
    VOS_UINT32 defRptDep;
    VOS_UINT32 hdlcDefErrInfor0;
    VOS_UINT32 hdlcDefErrInfor1;
    VOS_UINT32 hdlcDefErrInfor2;
    VOS_UINT32 hdlcDefErrInfor3;
    VOS_UINT32 defInfoFr1CntAgo;
    VOS_UINT32 defInfoFr1CntNow;
} PPP_DRIVER_HdlchardDefRegInfo;

/* ά�������Ϣ */

/* ��װ��ؼĴ�����ά�ɲ�ṹ */
typedef struct {
    HDLC_MNTN_TraceHead           head; /* _H2ASN_Skip */
    PPP_DRIVER_HdlchardFrmRegInfo frmRegInfo;
} HDLC_MNTN_FrmRegConfig;

/* ���װ��ؼĴ�����ά�ɲ�ṹ */
typedef struct {
    HDLC_MNTN_TraceHead           head; /* _H2ASN_Skip */
    PPP_DRIVER_HdlchardDefRegInfo defRegInfo;
} HDLC_MNTN_DefRegConfig;

typedef struct {
    HDLC_RegSaveInfo              hdlcRegInfo;
    PPP_DRIVER_HdlchardFrmRegInfo hdlcFrmRegInfo;
    PPP_DRIVER_HdlchardDefRegInfo hdlcDefRegInfo;
    PPP_DRIVER_HdlcHardConfigInfo hdlcConfigInfo;
} PPP_DRIVER_HardwareDumpInfo;

VOS_VOID   PPP_DriverHdlcHardPeriphClkOpen(VOS_VOID);
VOS_VOID   PPP_DriverHdlcHardPeriphClkClose(VOS_VOID);
VOS_UINT32 PPP_DriverHdlcHardInit(PPP_DRIVER_HdlcHardFrmConfig *frmConfig, PPP_DRIVER_HdlcHardDefConfig *defConfig);
VOS_VOID   PPP_DriverHdlcHardWorkStatus(VOS_BOOL *frmWork, VOS_BOOL *defWork);
VOS_VOID   PPP_DriverHdlcHardGetFrmResult(VOS_UINT16 *frmOutSegNum, VOS_UINT8 *frmValidNum);
VOS_UINT32 PPP_DriverHdlcHardFrmEnable(PPP_DRIVER_HdlcHardFrmPara *drvFrmPara, VOS_UINT32 *enableInterrupt);
VOS_UINT16 PPP_DriverHdlcHardGetDefVaildNum(VOS_VOID);
VOS_VOID   PPP_DriverHdlcHardGetDefUncompletInfo(
      PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo, VOS_UINT32 *validNum);
VOS_VOID PPP_DriverHdlcHardGetDefErrorInfo(PPP_DRIVER_HdlcHardDefErrFramesCnt *errCnt);

VOS_UINT32 PPP_DriverHdlcHardDefWaitResult(VOS_UINT32 enableInterrupt);
VOS_UINT32 PPP_DriverHdlcHardDefEnable(PPP_DRIVER_HdlcHardDefPara *drvDefPara, VOS_UINT32 *enableInterrupt);
VOS_VOID   PPP_DriverHdlcHardDefCfgGoOnReg(VOS_UINT32 defStatus);
VOS_VOID   PPP_DriverHdlcHardDefCfgReg(PPP_DRIVER_HdlcHardDefPara *drvDefPara);
VOS_VOID   PPP_DriverHdlcHardGetFrmRegInfo(PPP_DRIVER_HdlchardFrmRegInfo *regConfig);

VOS_VOID   PPP_DriverHdlcHardGetDefRegInfo(PPP_DRIVER_HdlchardDefRegInfo *regConfig);
VOS_UINT32 PPP_DriverHdlcHardGetDefRawInt(VOS_VOID);
VOS_UINT32 PPP_DriverHdlcHardGetFrmRawInt(VOS_VOID);
VOS_VOID   PPP_DriverHdlcSetMaxFrmDefInfoLen(VOS_VOID);
VOS_VOID   PPP_DriverHdlcHardGetDumpInfo(PPP_DRIVER_HardwareDumpInfo *dumpInfo);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
