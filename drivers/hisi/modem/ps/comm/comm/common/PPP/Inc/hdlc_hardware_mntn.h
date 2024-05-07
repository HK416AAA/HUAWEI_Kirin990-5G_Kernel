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

/* HDLC统计信息地址 */
#define PPP_HDLC_HARD_DATA_PROC_STAT (&g_pppHdlcHardStat)

/*
 * 定义异常信息bit位置:
 *      bit0代表是否进入异常处理(1是进入异常处理，0是没有进入异常处理)
 *      bit1代表是否是中断处理(1是中断处理，0是轮询处理)
 *      bit2代表是否等待信号量超时(1是，0否)
 *      bit3代表是否轮询超时(1是，0否)
 */
#define HDLC_EXCEPTION_IND_BITPOS 0UL
#define HDLC_INTERRUPT_IND_BITPOS 1UL
#define HDLC_SEM_TIMEOUT_IND_BITPOS 2UL
#define HDLC_WAIT_STATUS_TIMEOUT_IND_BITPOS 3UL

/* HDLC Warning Log信息,VOS_GetModemIDFromPid不能用了，PS_GetModemIdFromPid在A核不支持，现在写死MODEM_ID_0 */
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
 * HDLC维测配置信息
 *      bit0置1表示需要勾取参数信息
 *      bit1置1表示需要勾取寄存器信息
 *      bit2置1表示需要勾取数据报文信息
 */
#define PPP_HDLC_MNTN_TRACE_PARA 1
#define PPP_HDLC_MNTN_TRACE_REG 2
#define PPP_HDLC_MNTN_TRACE_DATA 4

/* 可维可测申请的内存最大长度，防止因为可维可测申请不到内存导致单板复位 */
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

/* 统计信息 */
typedef struct {
    VOS_UINT32 defIpDataProcCnt;   /* 解封装IP数据包处理个数 */
    VOS_UINT32 defPppDataProcCnt;  /* 解封装简单PPP帧处理个数 */
    VOS_UINT32 defUncompleteCnt;   /* 解封装有非完整帧输出次数 */
    VOS_UINT32 defWaitIntCnt;      /* 解封装等待中断次数 */
    VOS_UINT32 defWaitQueryCnt;    /* 解封装等待轮询次数 */
    VOS_UINT32 defLcpPauseCnt;     /* 解封装LCP暂停次数 */
    VOS_UINT32 defFullPauseCnt;    /* 解封装输出空间或上报空间满暂停次数 */
    VOS_UINT32 defInputDiscardCnt; /* 解封装丢弃数据包个数 */

    VOS_UINT32 frmIpDataProcCnt;         /* 封装IP数据包处理个数 */
    VOS_UINT32 frmPppDataProcCnt;        /* 封装简单PPP帧处理个数 */
    VOS_UINT32 frmWaitIntCnt;            /* 封装等待中断次数 */
    VOS_UINT32 frmWaitQueryCnt;          /* 封装等待轮询次数 */
    VOS_UINT32 frmAllocOutputMemFailCnt; /* 封装申请目的内存失败次数 */
    VOS_UINT32 frmAllocFirstMemFailCnt;  /* 封装申请第一个节点目的内存失败次数 */
    VOS_UINT32 frmOutputLinkFullCnt;     /* 封装输出链表满次数 */
    VOS_UINT32 frmInputDiscardCnt;       /* 封装丢弃数据包个数 */

    VOS_UINT32 defMaxInputCntOnce;  /* 解封装一次最大输入数据个数 */
    VOS_UINT32 defMaxInputSizeOnce; /* 解封装一次最大输入数据总长度 */
    VOS_UINT32 defMaxValidCntOnce;  /* 解封装一次最大输出有效帧个数 */
    VOS_UINT32 defMaxQueryCnt;      /* 解封装轮询最大次数 */

    VOS_UINT32 frmMaxInputCntOnce;   /* 封装一次最大输入数据个数 */
    VOS_UINT32 frmMaxInputSizeOnce;  /* 封装一次最大输入数据总长度 */
    VOS_UINT32 frmMaxOutputCntOnce;  /* 封装一次最大输出使用节点数个数 */
    VOS_UINT32 frmMaxOutputSizeOnce; /* 封装一次最大输出使用节点总长度 */
    VOS_UINT32 frmMaxQueryCnt;       /* 封装轮询最大次数 */

    VOS_UINT32 maxCntOnce;      /* PPP一次最多处理的结点个数 */
    VOS_UINT32 hdlcHardProcCnt; /* PPP一次最多处理的结点个数 */

    VOS_UINT32 defIsrCnt;          /* 解封装中断服务程序调用次数 */
    VOS_UINT32 frmIsrCnt;          /* 封装中断服务程序调用次数 */
    VOS_UINT32 continueCnt;        /* 队列中的数据需要分多次处理的次数 */
    VOS_UINT16 defExpInfo;         /* 解封装异常信息 */
    VOS_UINT16 frmExpInfo;         /* 封装异常信息 */
    VOS_UINT32 forbidHdlcBugNoCpy; /* 规避HDLC BUG,不拷贝数据 */
    VOS_UINT32 forbidHdlcBugCpy;   /* 规避HDLC BUG,需要拷贝数据 */
} PPP_HDLC_HardDataProcStat;

/* 输入参数链表可维可测结构 */
typedef struct {
    VOS_MSG_HEADER /* 消息头 */
        HDLC_MNTN_EventTypeUint32 msgname;
} HDLC_MNTN_TraceHead;

/* 输出和输出数据内容勾包 */
typedef struct {
    HDLC_MNTN_TraceHead head; /* _H2ASN_Skip */
    VOS_UINT16          nodeIndex;
    VOS_UINT16          len;
    /* VOS_UINT8[usLen]数据内容放在这里，申请的时候由头加上数据内容长度 */
} HDLC_MNTN_NodeData;

/* HDLC关键寄存器可维可测信息保存结构 */
typedef struct {
    VOS_UINT32 hdlcFrmRawInt; /* 封装原始中断寄存器值 */
    VOS_UINT32 hdlcFrmStatus; /* 封装状态寄存器值 */
    VOS_UINT32 hdlcDefRawInt; /* 解封装原始中断寄存器值 */
    VOS_UINT32 hdlcDefStatus; /* 解封装状态寄存器值 */
} HDLC_RegSaveInfo;

/* 统计信息 */
extern PPP_HDLC_HardDataProcStat g_pppHdlcHardStat;

/* 保留清原始中断时的RAW_INT和STATUS值 */
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
