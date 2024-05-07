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

/* 定义寄存器在32bit Reg中的bit位置:寄存器frm_en在bit0 */
#define HDLC_FRM_EN_BITPOS 0UL

/* 定义寄存器在32bit Reg中的bit位置: 寄存器def_en在bit0 */
#define HDLC_DEF_EN_BITPOS 0UL

/* 定义寄存器在32bit Reg中的bit位置:寄存器frm_in_lli_1dor2d在bit0;寄存器frm_acfc在bit1 */
#define HDLC_FRM_IN_LLI_1DOR2D_BITPOS 0UL
#define HDLC_FRM_ACFC_BITPOS 1UL
#define HDLC_FRM_PFC_BITPOS 2UL

/*
 * 寄存器frm_pfc(2bits)各值含义:
 * 00b: 硬件模块添加P域, P域无压缩;
 * 01b: 硬件模块添加P域, P域压缩;
 * 11b: 硬件模块不添加P域;
 */
#define HDLC_PROTOCOL_ADD_WITHOUT_COMPRESS 0x00
#define HDLC_PROTOCOL_ADD_WITH_COMPRESS 0x01
#define HDLC_PROTOCOL_NO_ADD 0x03

/*
 * 定义寄存器在32bit Reg中的bit位置:
 * 寄存器def_uncompleted_ago在bit0;
 * 寄存器def_acfc在bit1;
 */
#define HDLC_DEF_IS_UNCOMPLETED_AGO_BITPOS 0UL
#define HDLC_DEF_ACFC_BITPOS 1UL
#define HDLC_DEF_PFC_BITPOS 2UL

/* 寄存器frm_protocol(16bits)的MASK表 */
#define HDLC_FRM_PROTOCOL_MASK 0x0000FFFF

/*
 * 寄存器def_pfc(2bits)各值含义:
 * 00b: P域无压缩, 需剥离;
 * 01b: P域压缩, 需剥离;
 * 11b: P域不剥离;
 */
#define HDLC_PROTOCOL_REMOVE_WITHOUT_COMPRESS 0x00
#define HDLC_PROTOCOL_REMOVE_WITH_COMPRESS 0x01
#define HDLC_PROTOCOL_NO_REMOVE 0x03

/*
 * 寄存器hdlc_prior_ctrl(2bits)各值含义:
 * 00b: 谁先被使能则先将谁处理完;
 * 01b: 下行封装优先级高;
 * 10b: 上行解封装优先级高;
 * 11b: 无效;
 */
#define HDLC_PRIOR_FCFS 0x00
#define HDLC_PRIOR_DL_FRM_HIGHER 0x01
#define HDLC_PRIOR_UL_DEF_HIGHER 0x02
#define HDLC_PRIOR_INVALID 0x03

/* Boston版本中配置封装解封装信息域最大长度寄存器0x5DF(1503)Bytes */
#define HDLC_MAX_FRM_DEF_INFO_LEN 0x000005DFU

/* 封装状态寄存器地址 hdlc_frm_status (0x28) begin,封装模块状态寄存器地址: 相对偏移地址量0x28 */

/* 寄存器frm_out_seg_num(16bits)的MASK表 */
#define HDLC_FRM_OUT_SEG_NUM_MASK 0x0000FFFF

/* 寄存器frm_valid_num(8bits)的MASK表 */
#define HDLC_FRM_VALID_NUM_MASK 0x000000FF

/* 寄存器frm_all_pkt_done(1bit)的MASK表 */
#define HDLC_FRM_ALL_PKT_DONE_MASK 0x00000001

/* 查询封装状态信息的掩码 */
#define HDLC_FRM_STATUS_MASK 0x00000003

/*
 * 寄存器frm_block_done([0]位)各值含义:
 * 0b: 未完成一套链表数据处理;
 * 1b: 完成一套链表数据处理;
 * 寄存器frm_error_index([1]位)各值含义:
 * 1b: 封装发生错误;
 */
#define HDLC_FRM_ALL_PKT_DOING 0x00
#define HDLC_FRM_ALL_PKT_DONE 0x01
#define HDLC_FRM_STOP_EXCEPTION_OCCUR 0x02
#define HDLC_FRM_DONE_EXCEPTION_OCCUR 0x03

/* 解封装状态寄存器地址dlc_def_status (0x88) begin,寄存器dlc_def_status(5bits)的MASK表 */
#define HDLC_DEFRAMER_BLOCK_STATUS_MASK 0x0000001B

/* 查询解封装状态信息的掩码 */
#define HDLC_DEF_STATUS_MASK 0x0000003B

/*
 * 寄存器dlc_def_status(5bits)各值含义:
 * 000000b: 未完成一组数据处理;
 * 010000b: 未完成一组数据处理，解封装外部正确帧信息上报空间存满暂停;
 * 001000b: 未完成一组数据处理, 解封装外部输出存储空间存满暂停;
 * 000001b: 未完成一组数据处理, 已解出LCP帧, 硬件处于暂停状态;
 * 000010b: 完成一组数据处理, 但无帧上报;
 * 000011b: 完成一组数据处理, 且有帧上报;
 * 1xx0xxb: 有异常发生;
 */
#define HDLC_DEF_STATUS_DOING 0x00
#define HDLC_DEF_STATUS_PAUSE_RPT_SPACE_FULL 0x10
#define HDLC_DEF_STATUS_PAUSE_OUTPUT_SPACE_FULL 0x08
#define HDLC_DEF_STATUS_PAUSE_LCP 0x01
#define HDLC_DEF_STATUS_DONE_WITHOUT_FRAMES 0x02
#define HDLC_DEF_STATUS_DONE_WITH_FRAMES 0x03

/* 解封装是否有非完整帧标识 */
#define HDLC_DEF_UNCOMPLETED_NOT_EXIST 0x0
#define HDLC_DEF_UNCOMPLETED_EXIST 0x1

/* 轮询封装或解封装完成的次数 */
#define HDLC_FRM_MAX_WAIT_RESULT_NUM 0xFFFF
#define HDLC_DEF_MAX_WAIT_RESULT_NUM 0xFFFF

/* 等待封装或解封装响应中断时间,以毫秒为单位 */
#define HDLC_FRM_INT_TIMER_LEN 5000
#define HDLC_DEF_INT_TIMER_LEN 5000

/* 采用轮询还是中断使能的水线，默认值 */
#define HDLC_DEF_INTERRUPT_LIMIT_DEFAULT (2 * 1024)
#define HDLC_FRM_INTERRUPT_LIMIT_DEFAULT (2 * 1024)

/* AXI总线读写请求超时的判断值，该值由SoC提供，软件负责配置 */
#define HDLC_AXI_REQ_TIMEOUT_VALUE 255

/* HDLC地址控制域压缩 */
#define HDLC_ADDRESS_CTRL_NOCOMPRESS 0
#define HDLC_ADDRESS_CTRL_COMPRESS 1

/* HDLC硬件解封装错误z指示BIT位 */
#define PPP_DRIVER_HDLC_HARD_DEF_FCS_ERR 0
#define PPP_DRIVER_HDLC_HARD_DEF_FRAME_TOO_LONG 1
#define PPP_DRIVER_HDLC_HARD_DEF_FRAME_TOO_SHORT 2
#define PPP_DRIVER_HDLC_HARD_DEF_PROTOCOL_ERR 3
#define PPP_DRIVER_HDLC_HARD_DEF_CTRL_ERR 4
#define PPP_DRIVER_HDLC_HARD_DEF_ADDR_ERR 5
#define PPP_DRIVER_HDLC_HARD_DEF_FLAG_POS_ERR 6

/* 获取HDLC DRIVER配置信息 */
#define PPP_DRIVER_HDLC_HARD_GET_CONFIG (&g_hdlcConfigInfo)

/* 获取HDLC DRIVER封装配置信息 */
#define PPP_DRIVER_HDLC_HARD_GET_FRM_CONF (&(PPP_DRIVER_HDLC_HARD_GET_CONFIG->frmConfig))

/* 获取HDLC DRIVER解封装配置信息 */
#define PPP_DRIVER_HDLC_HARD_GET_DEF_CONF (&(PPP_DRIVER_HDLC_HARD_GET_CONFIG->defConfig))

/* 采用轮询还是中断使能的水线 */
#define HDLC_DEF_INTERRUPT_LIMIT (PPP_DRIVER_HDLC_HARD_GET_CONFIG->hdlcDefIntLimit)
#define HDLC_FRM_INTERRUPT_LIMIT (PPP_DRIVER_HDLC_HARD_GET_CONFIG->hdlcFrmIntLimit)

/* HDLC IP基地址 */
#define HDLC_IP_BASE_ADDR (PPP_DRIVER_HDLC_HARD_GET_CONFIG->hdlcIpBaseAddr)

/* 停止时钟前，等待HDLC工作完成的最大次数 */
#define HDLC_CHECK_HARDWAWRE_STATE_MAX_TIMES 100

/* 硬件加速器状态寄存器 */
#define HDLC_SYSCTRL_STAT_ADDR(base) ((base) + SOC_HDLC_SYSCTRL_STAT_OFFSET)

/* hdlc硬件地址类型，目前使用32bit地址类型 */
typedef VOS_UINT32 HDLC_ADDR_TYPE;

enum PPP_DRIVER_HdlcReg {
    PPP_DRIVER_MAX_FRM_INFO_REG = 0x00,
    PPP_DRIVER_MAX_DEF_INFO_REG = 0x01,

    PPP_DRIVER_HDLC_REG_BUTT
};
typedef VOS_UINT8 PPP_DRIVER_HdlcRegUint8;

/* 一组数据解出的错误帧数统计结构 */
typedef struct {
    VOS_UINT16 fcsErrCnt;      /* FCS错误 */
    VOS_UINT16 lenLongCnt;     /* 帧长度过长, 超过1502字节 */
    VOS_UINT16 lenShortCnt;    /* 帧长度过短, 少于4字节 */
    VOS_UINT16 errProtocolCnt; /* 协议字段不满足xxxxxxx0, xxxxxxx1的形式 */
    VOS_UINT16 errCtrlCnt;     /* 控制字段不为0xFF */
    VOS_UINT16 errAddrCnt;     /* 地址字段不为0x03 */
    VOS_UINT16 flagPosErrCnt;  /* 0x7D后紧接着0x7E */
    VOS_UINT8  errType;        /* 错误类型 */
    VOS_UINT8  reserve;
} PPP_DRIVER_HdlcHardDefErrFramesCnt;

/* HDLC封装配置相关信息结构 */
typedef struct {
    VOS_VOID  *inputAddr;  /* 输入链表地址 */
    VOS_VOID  *outputAddr; /* 输出链表地址 */
    VOS_VOID  *reportAddr; /* 封装结果上报地址 */
    VOS_UINT32 rptBufLen;  /* 上报空间大小 */
} PPP_DRIVER_HdlcHardFrmConfig;

/* HDLC解封装配置相关信息结构 */
typedef struct {
    VOS_VOID  *inputAddr;    /* 输入链表地址 */
    VOS_VOID  *outputAddr;   /* 输出链表地址 */
    VOS_VOID  *reportAddr;   /* 封装结果上报地址 */
    VOS_UINT32 outputBufLen; /* 输出空间大小 */
    VOS_UINT32 reportBufLen; /* 上报空间大小 */
    VOS_UINT32 perInMaxLen;  /* 输入数据最大单包长度 */
} PPP_DRIVER_HdlcHardDefConfig;

/* HDLC配置相关信息结构 */
typedef struct {
    VOS_UINT_PTR                 hdlcIpBaseAddr;   /* 从底软获取的HDLC基地址 */
    VOS_SEM                      hdlcDefSem;       /* 解封装信号量 */
    VOS_SEM                      hdlcFrmSem;       /* 封装信号量 */
    VOS_INT32                    hdlcIsrDef;       /* 解封装中断号 */
    VOS_INT32                    hdlcIsrFrm;       /* 封装中断号 */
    VOS_UINT32                   hdlcDefIntLimit;  /* 解封装中断水线 */
    VOS_UINT32                   hdlcFrmIntLimit;  /* 封装中断水线 */
    PPP_DRIVER_HdlcHardFrmConfig frmConfig;        /* 封装配置信息 */
    PPP_DRIVER_HdlcHardDefConfig defConfig;        /* 解封装配置信息 */
} PPP_DRIVER_HdlcHardConfigInfo;

/* HDLC封装参数结构 */
typedef struct {
    VOS_UINT32 inputTotalSize; /* 输入链表总大小，用于判断是使用中断还是轮训方式 */
    VOS_UINT32 accmFlag;       /* 同异步控制字 */
    VOS_UINT32 pppAcFlag;      /* 地址控制域压缩信息 */
    VOS_UINT32 pppPcFlag;      /* 协议域压缩信息 */
    VOS_UINT16 protocol;       /* 协议类型 */
    VOS_UINT8  reserve[2];
} PPP_DRIVER_HdlcHardFrmPara;

/* HDLC解封装非完整帧信息存储结构 */
typedef struct {
    VOS_UINT8      existFlag; /* 非完整帧是否存在标识 */
    VOS_UINT8      reserve1[3];
    VOS_UINT16     defOutOnePro;     /* 非完整帧的协议 */
    VOS_UINT16     defOutOneLen;     /* 非完整帧的长度 */
    HDLC_ADDR_TYPE defOutOneAddr;    /* 指向解封装出的非完整帧在外存储的起始地址 */
    VOS_UINT32     defStAgo;         /* 用于保存def_uncomplet_st_now寄存器值 */
    VOS_UINT32     defInfoFrlCntAgo; /* 用于保存def_info_frl_cnt_ago寄存器值 */
} PPP_DRIVER_HdlcHardDefUncompletedInfo;

/* HDLC解封装参数结构 */
typedef struct {
    VOS_UINT32                             inputTotalSize; /* 输入链表总大小，用于判断是使用中断还是轮训方式 */
    VOS_UINT32                             pppPcFlag;      /* 协议域添加及压缩信息 */
    VOS_UINT32                             pppAcFlag;      /* 地址控制域压缩信息 */
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo; /* 上次解封装的不完整帧信息 */
} PPP_DRIVER_HdlcHardDefPara;

/* 封装寄存器信息 */
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

/* 解封装寄存器信息 */
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

/* 维测相关信息 */

/* 封装相关寄存器可维可测结构 */
typedef struct {
    HDLC_MNTN_TraceHead           head; /* _H2ASN_Skip */
    PPP_DRIVER_HdlchardFrmRegInfo frmRegInfo;
} HDLC_MNTN_FrmRegConfig;

/* 解封装相关寄存器可维可测结构 */
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
