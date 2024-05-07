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

#include "hdlc_hardware_service.h"
#include "ttf_comm.h"
#include "ttf_util.h"
#include "gucttf_tag.h"
#include "securec.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_om.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_HDLC_HARDWARE_SERVICE_C
#define THIS_MODU mod_ppp

#if (FEATURE_PPP_HDLC_HARDWARE == FEATURE_ON)

/* 解封装输出的非完整帧信息 */
STATIC PPP_DRIVER_HdlcHardDefUncompletedInfo g_uncompletedInfo = {0};

/* 保存解封装使用的内存 */
STATIC HDLC_DefBuffInfo *g_hdlcDefBufInfo = VOS_NULL_PTR;

/* 保存封装使用的内存 */
STATIC HDLC_FrmBuffInfo *g_hdlcFrmBufInfo = VOS_NULL_PTR;

/* 业务模块封装结果处理函数 */
STATIC PPP_ServiceHdlcHardDefCallBack g_hldcDefCallBackInfo;

/* 打开HDLC时钟 */
VOS_VOID PPP_ServiceHdlcHardOpenClk(VOS_VOID)
{
    PPP_DriverHdlcHardPeriphClkOpen();
}

/* 关闭HDLC时钟 */
VOS_VOID PPP_ServiceHdlcHardCloseClk(VOS_VOID)
{
    PPP_DriverHdlcHardPeriphClkClose();
}

/* 获取HDLC工作状态 */
PPP_ServiceHdlcHardWorkStatusUint8 PPP_ServiceHdlcHardGetWorkStatus(VOS_VOID)
{
    VOS_BOOL                           frmWork;
    VOS_BOOL                           defWork;
    PPP_ServiceHdlcHardWorkStatusUint8 workStatus;

    PPP_DriverHdlcHardWorkStatus(&frmWork, &defWork);

    if ((frmWork == VOS_TRUE) && (defWork == VOS_TRUE)) {
        workStatus = PPP_SERVICE_HDLC_HARD_BOTH_WORK;
    } else if (frmWork == VOS_TRUE) {
        workStatus = PPP_SERVICE_HDLC_HARD_FRM_WORK;
    } else if (defWork == VOS_TRUE) {
        workStatus = PPP_SERVICE_HDLC_HARD_DEF_WORK;
    } else {
        workStatus = PPP_SERVICE_HDLC_HARD_NOT_WORK;
    }

    return workStatus;
}

/* 初始化该路PPP链接，如清空非完整帧信息 */
VOS_VOID PPP_ServiceHdlcHardSetUp(VOS_UINT16 pppId)
{
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompletedInfo = VOS_NULL_PTR;

    uncompletedInfo = HDLC_DEF_GET_UNCOMPLETED_INFO(pppId);

    (VOS_VOID)memset_s(uncompletedInfo, sizeof(PPP_DRIVER_HdlcHardDefUncompletedInfo), 0,
        sizeof(PPP_DRIVER_HdlcHardDefUncompletedInfo));

    return;
}

/* 对HDLC进行去使能 */
VOS_VOID PPP_ServiceHdlcHardDisable(VOS_VOID)
{
    /*
     * 暂无操作，因为HDLC优化后，一套链表封装或解封装完成时，由硬件自动对frm_en或def_en清零；
     * 封装或解封装过程出错时，硬件也会自动清零，使内部状态机返回IDLE状态?
     */
}

/* 初始化HDLC，完成内存申请硬件配置等工作 */
VOS_UINT32 PPP_ServiceHdlcHardInit(PPP_HDLC_HardConfigInfo *hdlcConfig, PPP_ServiceHdlcHardDefCallBack *defCBInfo)
{
    VOS_UINT_PTR                 pppVirtAddr    = 0;
    VOS_UINT_PTR                 pppPhyAddr     = 0;
    VOS_UINT32                   pppTotalBufLen = 0;
    PPP_DRIVER_HdlcHardFrmConfig frmConfig; /* 封装配置信息 */
    PPP_DRIVER_HdlcHardDefConfig defConfig; /* 解封装配置信息 */

    if (PPP_HdlcHardConfig(hdlcConfig) == VOS_ERR) {
        return VOS_ERR;
    }

    if ((defCBInfo == VOS_NULL_PTR) || (defCBInfo->defErrProcFunc == VOS_NULL_PTR) ||
        (defCBInfo->defResProcFunc == VOS_NULL_PTR)) {
        PPP_HDLC_ERROR_LOG("defResProcFunc is NULL");
        return VOS_ERR;
    }

    g_hldcDefCallBackInfo.defResProcFunc = defCBInfo->defResProcFunc;
    g_hldcDefCallBackInfo.defErrProcFunc = defCBInfo->defErrProcFunc;

    /* 计算内存长度，实际使用长度为0x6140，申请按照0x8000(32K)对齐 */
    pppTotalBufLen = hdlcConfig->pppTotalBufLen;

    /* 内存申请 */
    pppVirtAddr = (VOS_UINT_PTR)VOS_UnCacheMemAllocDebug(
        pppTotalBufLen, VOS_UNCACHE_MEM_ADDR_ALIGN_4K, &pppPhyAddr, TTF_HDLC_UNCACHE_MEM);
    if (pppVirtAddr == VOS_NULL_PTR) {
        PPP_HDLC_ERROR_LOG("PPP_HDLC_HARD_InitBuf, ERROR, VOS_UnCacheMemAlloc Fail\r\n");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((void*)pppVirtAddr, pppTotalBufLen, 0, pppTotalBufLen);

    /* 根据TtfMemoryMap.h初始化HDLC所需内存 */
    g_hdlcDefBufInfo = (HDLC_DefBuffInfo*)pppVirtAddr;
    g_hdlcFrmBufInfo = (HDLC_FrmBuffInfo*)(pppVirtAddr + sizeof(HDLC_DefBuffInfo));

    g_pppHdlcConfig.pppPhyAddr  = pppPhyAddr;
    g_pppHdlcConfig.pppVirtAddr = pppVirtAddr;

    /* TTF_HDLC_DEF_BUF_LEN必须与结构HDLC_DEF_BUFF_INFO_STRU的大小一致 */
    PPP_SERVICE_HDLC_HARD_STRU_SIZE_CHECK(TTF_HDLC_DEF_BUF_LEN, HDLC_DefBuffInfo);

    /* TTF_HDLC_FRM_BUF_LEN收必须与结构HDLC_FRM_BUFF_INFO_STRU的大小一致 */
    PPP_SERVICE_HDLC_HARD_STRU_SIZE_CHECK(TTF_HDLC_FRM_BUF_LEN, HDLC_FrmBuffInfo);

    /* 封装配置参数 */
    frmConfig.inputAddr  = PPP_HDLC_HARD_BUF_VIRT2PHY(g_hdlcFrmBufInfo->inputParaLinkNodeBuf);
    frmConfig.outputAddr = PPP_HDLC_HARD_BUF_VIRT2PHY(g_hdlcFrmBufInfo->outputParaLinkNodeBuf);
    frmConfig.reportAddr = PPP_HDLC_HARD_BUF_VIRT2PHY(g_hdlcFrmBufInfo->rptNodeBuf);
    frmConfig.rptBufLen  = TTF_HDLC_FRM_RPT_BUF_LEN;

    /* 解封装配置参数 */
    defConfig.inputAddr    = PPP_HDLC_HARD_BUF_VIRT2PHY(g_hdlcDefBufInfo->inputParaLinkNodeBuf);
    defConfig.outputAddr   = PPP_HDLC_HARD_BUF_VIRT2PHY(g_hdlcDefBufInfo->outputDataBuf);
    defConfig.reportAddr   = PPP_HDLC_HARD_BUF_VIRT2PHY(g_hdlcDefBufInfo->rptNodeBuf);
    defConfig.outputBufLen = TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN;
    defConfig.reportBufLen = TTF_HDLC_DEF_RPT_BUF_LEN;
    defConfig.perInMaxLen  = HDLC_DEF_IN_PER_MAX_CNT;

    if (PPP_DriverHdlcHardInit(&frmConfig, &defConfig) == VOS_ERR) {
        PPP_HDLC_ERROR_LOG("HDLC Hardware Init Fail");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 勾取输入参数链表内容 */
STATIC VOS_VOID PPP_ServiceHdlcHardTraceInputPara(
    HDLC_MNTN_EventTypeUint32 eventType, VOS_UINT32 linkNodeCnt, HDLC_ParaLinkNode *linkNode)
{
    HDLC_MNTN_InputParaLink  inputPara;
    HDLC_MNTN_InputParaLink *inputParaInfo = &inputPara;
    VOS_UINT32               dataLen;

    dataLen = sizeof(HDLC_MNTN_InputParaLink);

    /* 记录并上报参数链表所有节点的信息 */
    inputParaInfo->inputLinkNodeCnt = linkNodeCnt;

    /* 参数链表每个节点的内容 */
    TTF_SF_CHK(memcpy_s(&(inputParaInfo->inputParaLinkNodeBuf[0]), sizeof(inputParaInfo->inputParaLinkNodeBuf),
        linkNode, linkNodeCnt * sizeof(HDLC_ParaLinkNode)));

    PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)inputParaInfo, eventType, dataLen);

    return;
}

/* 勾取封装输入参数链表和数据 */
STATIC VOS_VOID PPP_ServiceHdlcHardTraceFrmInput(HDLC_FrmBuffInfo *frmBuffInfo, PPP_HDLC_HardFrmPara *frmPara)
{
    VOS_UINT32         nodeLoop;
    HDLC_ParaLinkNode *paraNode = VOS_NULL_PTR;
    VOS_UINT8         *dataAddr = VOS_NULL_PTR;

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_PARA) == VOS_TRUE) {
        PPP_ServiceHdlcHardTraceInputPara(
            ID_HDLC_MNTN_FRM_INPUT_PARA, frmPara->inputNodeParaCnt, &(frmBuffInfo->inputParaLinkNodeBuf[0]));
    }

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_DATA) == VOS_TRUE) {
        /* 记录并上报参数链表所有节点的数据内容，每个节点是一个IP包 */
        for (nodeLoop = 0; nodeLoop < frmPara->inputNodeParaCnt; nodeLoop++) {
            paraNode = &(frmBuffInfo->inputParaLinkNodeBuf[nodeLoop]);
            dataAddr = (VOS_UINT8*)PPP_HDLC_HARD_MEM_PHY2VIRT(paraNode->dataAddr);
            PPP_HDLC_HardMntnTraceSingleData(paraNode->dataLen, dataAddr, ID_HDLC_MNTN_FRM_INPUT_DATA, nodeLoop);
        }
    }

    return;
}

/* 更新封装内存相关信息 */
STATIC VOS_UINT32 PPP_ServiceHdlcHardUpdateFrmBuffInfo(PPP_HDLC_HardFrmPara *frmPara)
{
    VOS_UINT32                   nodeParaIndex;
    HDLC_ParaLinkNode           *paraLinkNode = VOS_NULL_PTR;
    const PPP_HDLC_HardNodePara *nodePara     = VOS_NULL_PTR;
    VOS_UINT32                   outputSize   = 0;
    PPP_HDLC_HardDataProcStat   *hdlcStat     = VOS_NULL_PTR;
    HDLC_FrmBuffInfo            *frmBuffInfo  = VOS_NULL_PTR;

    if ((frmPara->inputNodeParaCnt > TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM) ||
        (frmPara->outputNodeParaCnt > TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM)) {
        PPP_HDLC_WARNING_LOG2("ulInputMemParaCnt or ulOutputMemParaCnt is too big", frmPara->inputNodeParaCnt,
            frmPara->outputNodeParaCnt);
        return VOS_ERR;
    }

    frmBuffInfo = HDLC_FRM_GET_BUF_INFO(frmPara->usPppId);

    /* 更新输入链表参数 */
    for (nodeParaIndex = 0; nodeParaIndex < frmPara->inputNodeParaCnt; nodeParaIndex++) {
        paraLinkNode           = &frmBuffInfo->inputParaLinkNodeBuf[nodeParaIndex];
        nodePara               = &frmPara->inputNodePara[nodeParaIndex];
        paraLinkNode->dataAddr = (HDLC_ADDR_TYPE)(VOS_UINT_PTR)PPP_HDLC_HARD_MEM_VIRT2PHY(nodePara->dataAddr);
        paraLinkNode->dataLen  = nodePara->dataLen;
        paraLinkNode->nextNode = VOS_NULL_PTR;

        if (nodeParaIndex != 0) {
            frmBuffInfo->inputParaLinkNodeBuf[nodeParaIndex - 1].nextNode = (HDLC_ADDR_TYPE)(
                VOS_UINT_PTR)PPP_HDLC_HARD_BUF_VIRT2PHY(paraLinkNode);
        }
    }

    /* 更新输出链表参数 */
    for (nodeParaIndex = 0; nodeParaIndex < frmPara->outputNodeParaCnt; nodeParaIndex++) {
        paraLinkNode           = &frmBuffInfo->outputParaLinkNodeBuf[nodeParaIndex];
        nodePara               = &frmPara->outputNodePara[nodeParaIndex];
        paraLinkNode->dataAddr = (HDLC_ADDR_TYPE)(VOS_UINT_PTR)PPP_HDLC_HARD_MEM_VIRT2PHY(nodePara->dataAddr);
        paraLinkNode->dataLen  = nodePara->dataLen;
        paraLinkNode->nextNode = VOS_NULL_PTR;

        if (nodeParaIndex != 0) {
            frmBuffInfo->outputParaLinkNodeBuf[nodeParaIndex - 1].nextNode = (HDLC_ADDR_TYPE)(
                VOS_UINT_PTR)PPP_HDLC_HARD_BUF_VIRT2PHY(paraLinkNode);
        }

        outputSize += nodePara->dataLen;
    }

    /* 统计信息更新 */
    hdlcStat                       = PPP_HDLC_HARD_DATA_PROC_STAT;
    hdlcStat->frmMaxInputCntOnce   = TTF_MAX(hdlcStat->frmMaxInputCntOnce, frmPara->inputNodeParaCnt);
    hdlcStat->frmMaxInputSizeOnce  = TTF_MAX(hdlcStat->frmMaxInputSizeOnce, frmPara->inputTotalSize);
    hdlcStat->frmMaxOutputCntOnce  = TTF_MAX(hdlcStat->frmMaxOutputCntOnce, frmPara->outputNodeParaCnt);
    hdlcStat->frmMaxOutputSizeOnce = TTF_MAX(hdlcStat->frmMaxOutputSizeOnce, outputSize);

    /* 报输入输出参数链表内容可维可测 */
    PPP_ServiceHdlcHardTraceFrmInput(frmBuffInfo, frmPara);

    return VOS_OK;
}

/* 勾取封装输出参数链表和数据 */
STATIC VOS_VOID PPP_ServiceHdlcHardTraceFrmOutput(
    VOS_UINT8 frmValidNum, VOS_UINT16 frmOutSegNum, HDLC_FrmBuffInfo *frmBuffInfo, PPP_HDLC_HardFrmPara *frmPara)
{
    HDLC_MNTN_FrmOutputPara  outputPara;
    HDLC_MNTN_FrmOutputPara *outputParaInfo = &outputPara;
    VOS_UINT32               dataLen;
    VOS_UINT32               nodeLoop;
    HDLC_ParaLinkNode       *paraNode = VOS_NULL_PTR;
    VOS_UINT8               *dataAddr = VOS_NULL_PTR;

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_PARA) == VOS_TRUE) {
        dataLen = sizeof(HDLC_MNTN_FrmOutputPara);

        outputParaInfo->outputLinkNodeCnt = frmPara->outputNodeParaCnt;
        outputParaInfo->frmValidNum       = frmValidNum;
        outputParaInfo->outputNodeUsedCnt = frmOutSegNum;

        (VOS_VOID)memcpy_s(&(outputParaInfo->outputParaLinkNodeBuf[0]), sizeof(outputParaInfo->outputParaLinkNodeBuf),
            &(frmBuffInfo->outputParaLinkNodeBuf[0]), sizeof(frmBuffInfo->outputParaLinkNodeBuf));

        (VOS_VOID)memcpy_s(&(outputParaInfo->rptNodeBuf[0]), sizeof(outputParaInfo->rptNodeBuf),
            &(frmBuffInfo->rptNodeBuf[0]), sizeof(frmBuffInfo->rptNodeBuf));

        PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)outputParaInfo, ID_HDLC_MNTN_FRM_OUTPUT_PARA, dataLen);
    }

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_DATA) == VOS_TRUE) {
        for (nodeLoop = 0; nodeLoop < frmOutSegNum; nodeLoop++) {
            paraNode = &(frmBuffInfo->outputParaLinkNodeBuf[nodeLoop]);
            dataAddr = (VOS_UINT8*)PPP_HDLC_HARD_MEM_PHY2VIRT(paraNode->dataAddr);
            PPP_HDLC_HardMntnTraceSingleData(paraNode->dataLen, dataAddr, ID_HDLC_MNTN_FRM_OUTPUT_DATA, nodeLoop);
        }
    }

    return;
}

STATIC VOS_UINT32 PPP_CheckHdlcHardFrmResult(PPP_HDLC_HardFrmPara *frmPara, VOS_UINT16 frmOutSegNum,
    VOS_UINT8 frmValidNum)
{
    /* ucFrmValidNum肯定要小于等于使用的内存块数usFrmOutSegNum */
    if (frmValidNum > frmOutSegNum) {
        PPP_HDLC_WARNING_LOG2("frm_valid_num > usFrmOutSegNum", frmValidNum, frmOutSegNum);
        return VOS_ERR;
    }

    /* ucFrmValidNum应该与输入数据相同ulInputLinkNodeCnt，如果解封装出错可能会比后者小 */
    if (frmValidNum > frmPara->inputNodeParaCnt) {
        PPP_HDLC_WARNING_LOG2("frm_valid_num > ulInputNodeParaCnt", frmValidNum, frmPara->inputNodeParaCnt);
        return VOS_ERR;
    }

    /* usFrmOutSegNum使用的内存块数肯定小于等于ulOutputLinkNodeCnt */
    if (frmOutSegNum > frmPara->outputNodeParaCnt) {
        PPP_HDLC_WARNING_LOG2("frm_out_seg_num > ulOutputMemParaCnt", frmOutSegNum, frmPara->outputNodeParaCnt);
        return VOS_ERR;
    }

    return VOS_OK;
}

/* 处理硬件封装结果 */
STATIC PPP_HDLC_HardProcResultUint32 PPP_ServiceHdlcHardFrmResultProc(PPP_HDLC_HardFrmPara *frmPara,
    PPP_HDLC_HardFrmResult *frmResult)
{
    VOS_UINT16         frmOutSegNum = 0;
    VOS_UINT8          frmValidNum  = 0;
    VOS_UINT8          rptSpaceIndex;
    VOS_UINT32         outputLinkIndex = 0;
    HDLC_ADDR_TYPE     frmOutAddr;
    VOS_UINT16         frmOutLen;
    HDLC_FrmBuffInfo  *frmBuffInfo    = HDLC_FRM_GET_BUF_INFO(frmPara->usPppId); /* 根据usPppId找到对应的内存  */
    HDLC_ParaLinkNode *outputParaLink = VOS_NULL_PTR;
    HDLC_FrmRptNode   *frmRptNodeStru = VOS_NULL_PTR;

    PPP_DriverHdlcHardGetFrmResult(&frmOutSegNum, &frmValidNum);

    /* 上报封装后数据可维可测:上报空间信息、输出链表内容 */
    PPP_ServiceHdlcHardTraceFrmOutput(frmValidNum, frmOutSegNum, frmBuffInfo, frmPara);
    if (PPP_CheckHdlcHardFrmResult(frmPara, frmOutSegNum, frmValidNum) != VOS_OK) {
        return PPP_HDLC_HARD_PROC_FAIL;
    }

    frmRptNodeStru = &(frmBuffInfo->rptNodeBuf[0]);            /* 封装上报空间首地址 */
    outputParaLink = &(frmBuffInfo->outputParaLinkNodeBuf[0]); /* 封装输出链表首地址 */
    for (rptSpaceIndex = 0; rptSpaceIndex < frmValidNum; rptSpaceIndex++) {
        frmOutAddr = frmRptNodeStru[rptSpaceIndex].frmOutOneAddr;
        frmOutLen  = frmRptNodeStru[rptSpaceIndex].frmOutOneLen;
        /* 封装后的长度为0或者大于封装输出最大长度的话都是异常，不能继续处理后边报文 */
        if ((frmOutLen == 0) || (frmOutLen > HDLC_FRM_OUT_PER_MAX_CNT)) {
            PPP_HDLC_WARNING_LOG1("invalid usFrmOutOneLen", frmOutLen);
            frmResult->frmValidCnt = rptSpaceIndex;
            return PPP_HDLC_HARD_PROC_PART_SUCC;
        }

        /* 封装上报结果的地址和封装输出结果地址不一致时已经异常，不能继续处理后边报文 */
        outputParaLink = &(frmBuffInfo->outputParaLinkNodeBuf[outputLinkIndex]);
        if (frmOutAddr != outputParaLink->dataAddr) {
            PPP_HDLC_WARNING_LOG(" SOC copy error!");
            frmResult->frmValidCnt = rptSpaceIndex;
            return PPP_HDLC_HARD_PROC_PART_SUCC;
        }

        frmResult->frmResultNode[rptSpaceIndex].dataLen  = frmOutLen;
        frmResult->frmResultNode[rptSpaceIndex].dataAddr = PPP_HDLC_HARD_MEM_PHY2VIRT(frmOutAddr);

        /* 封装后的长度大于封装输出节点的数据长度说明该封装后的结果占用了两个输出节点 */
        outputLinkIndex++;
        if (frmOutLen > outputParaLink->dataLen) {
            outputLinkIndex++;
        }
    }

    frmResult->frmValidCnt = rptSpaceIndex;

    return PPP_HDLC_HARD_PROC_ALL_SUCC;
}

/* 封装硬件异常处理 */
STATIC VOS_VOID PPP_ServiceHdlcHardFrmProcException(VOS_UINT32 status, VOS_UINT32 enableInterrupt)
{
    VOS_UINT32                  rawInt;
    PPP_DRIVER_HardwareDumpInfo dumpInfo;

    if (enableInterrupt == VOS_TRUE) {
        /* 由于在中断服务程序中进行了清中断操作，故此处取保存在g_hdlcRegSaveInfo中的原始中断寄存器值 */
        rawInt = g_hdlcRegSaveInfo.hdlcFrmRawInt;
        g_pppHdlcHardStat.frmExpInfo |= (1 << HDLC_INTERRUPT_IND_BITPOS);
    } else {
        rawInt = PPP_DriverHdlcHardGetFrmRawInt();
    }

    PPP_HDLC_ERROR_LOG2("Frm Exception ocurr", status, rawInt);

    g_pppHdlcHardStat.frmExpInfo |= (1 << HDLC_EXCEPTION_IND_BITPOS);

    PPP_DriverHdlcHardGetDumpInfo(&dumpInfo);

    /* 如果HDLC出现异常，则单板异常重启 */
    mdrv_om_system_error(
        HDLC_FRM_SYSTEM_ERROR_ID, __LINE__, (VOS_INT)status, (VOS_CHAR*)&dumpInfo, (VOS_INT32)sizeof(dumpInfo));

    return;
}

/* 构造硬件封装参数 */
VOS_VOID PPP_ServiceHdlcHardBuildFrmPara(PPP_HDLC_HardFrmPara *frmPara, PPP_DRIVER_HdlcHardFrmPara *drvFrmPara)
{
    VOS_UINT32 accmFlag = 0xFFFFFFFF;

    /*  IP模式一定添加P域,PPP模式一定不添加P域LCP帧: AC域不压缩，P域不压缩 */
    if (frmPara->pppMode == PPP_SERVICE_HDLC_HARD_IP_MODE) {
        if (frmPara->protocol != PPP_SERVICE_HDLC_HARD_LCP) {
            if (frmPara->pppAcFlag == 1) {
                drvFrmPara->pppAcFlag = HDLC_ADDRESS_CTRL_COMPRESS;
            }

            if (frmPara->pppPcFlag == 1) {
                drvFrmPara->pppPcFlag = HDLC_PROTOCOL_ADD_WITH_COMPRESS;
            }

            accmFlag = frmPara->accmFlag;
        }
    } else {
        if (frmPara->protocol != PPP_SERVICE_HDLC_HARD_LCP) {
            if (frmPara->pppAcFlag == 1) {
                drvFrmPara->pppAcFlag = HDLC_ADDRESS_CTRL_COMPRESS;
            }
        }
        drvFrmPara->pppPcFlag = HDLC_PROTOCOL_NO_ADD;
    }

    drvFrmPara->protocol       = frmPara->protocol;
    drvFrmPara->accmFlag       = accmFlag;
    drvFrmPara->inputTotalSize = frmPara->inputTotalSize;

    return;
}

/* 硬件封装报文 */
PPP_HDLC_HardProcResultUint32 PPP_ServiceHdlcHardFrmPacket(
    PPP_HDLC_HardFrmPara *frmPara, PPP_HDLC_HardFrmResult *frmResult)
{
    VOS_UINT32                 enableInterrupt = 0;
    VOS_UINT32                 checkParaResutl;
    VOS_UINT32                 frmStatus;
    VOS_UINT32                 result;
    PPP_DRIVER_HdlcHardFrmPara drvFrmPara = {0};

    if ((frmPara == VOS_NULL_PTR) || (frmResult == VOS_NULL_PTR)) {
        PPP_HDLC_WARNING_LOG("pstFrmPara is NULL");
        return PPP_HDLC_HARD_PROC_PARA_ERR;
    }

    checkParaResutl = PPP_ServiceHdlcHardUpdateFrmBuffInfo(frmPara);
    if (checkParaResutl == VOS_ERR) {
        return PPP_HDLC_HARD_PROC_PARA_ERR;
    }
    PPP_ServiceHdlcHardBuildFrmPara(frmPara, &drvFrmPara);

    frmStatus = PPP_DriverHdlcHardFrmEnable(&drvFrmPara, &enableInterrupt);

    /* 使能中断，则等待中断辅程序释放信号量；否则轮询解封装状态寄存器 */
    if (frmStatus != HDLC_FRM_ALL_PKT_DONE) {
        /* 寄存器状态异常时需要复位 */
        PPP_ServiceHdlcHardFrmProcException(frmStatus, enableInterrupt);
        return PPP_HDLC_HARD_PROC_STATUS_ERR;
    }

    result = PPP_ServiceHdlcHardFrmResultProc(frmPara, frmResult);

    return result;
}

/* 勾取解封装输入参数链表和数据 */
VOS_VOID PPP_ServiceHdlcHardTraceDefInput(PPP_HDLC_HardDefPara *defPara, HDLC_DefBuffInfo *defBuffInfo)
{
    VOS_UINT32         nodeLoop;
    HDLC_ParaLinkNode *paraNode = VOS_NULL_PTR;
    VOS_UINT8         *dataAddr = VOS_NULL_PTR;

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_PARA) == VOS_TRUE) {
        PPP_ServiceHdlcHardTraceInputPara(
            ID_HDLC_MNTN_DEF_INPUT_PARA, defPara->inputNodeParaCnt, &(defBuffInfo->inputParaLinkNodeBuf[0]));
    }

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_DATA) == VOS_TRUE) {
        /* 记录并上报参数链表所有节点的数据内容，每个节点是一个IP包 */
        for (nodeLoop = 0; nodeLoop < defPara->inputNodeParaCnt; nodeLoop++) {
            paraNode = &(defBuffInfo->inputParaLinkNodeBuf[nodeLoop]);

            dataAddr = (VOS_UINT8*)PPP_HDLC_HARD_MEM_PHY2VIRT(paraNode->dataAddr);
            PPP_HDLC_HardMntnTraceSingleData(paraNode->dataLen, dataAddr, ID_HDLC_MNTN_DEF_INPUT_DATA, nodeLoop);
        }
    }

    return;
}

/* 更新解封装内存相关信息 */
VOS_UINT32 PPP_ServiceHdlcHardUpdateDefBuffInfo(PPP_HDLC_HardDefPara *defPara)
{
    VOS_UINT32                   nodeParaIndex;
    HDLC_ParaLinkNode           *paraLinkNode = VOS_NULL_PTR;
    const PPP_HDLC_HardNodePara *nodePara     = VOS_NULL_PTR;
    PPP_HDLC_HardDataProcStat   *hdlcStat     = VOS_NULL_PTR;
    HDLC_DefBuffInfo            *defBuffInfo  = VOS_NULL_PTR;

    if (defPara->inputNodeParaCnt > TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM) {
        PPP_HDLC_WARNING_LOG1("ulInputMemParaCnt is too big", defPara->inputNodeParaCnt);
        return VOS_ERR;
    }

    defBuffInfo = HDLC_DEF_GET_BUF_INFO(defPara->pppId);

    /* 更新输入链表参数 */
    for (nodeParaIndex = 0; nodeParaIndex < defPara->inputNodeParaCnt; nodeParaIndex++) {
        paraLinkNode           = &defBuffInfo->inputParaLinkNodeBuf[nodeParaIndex];
        nodePara               = &defPara->inputNodePara[nodeParaIndex];
        paraLinkNode->dataAddr = (HDLC_ADDR_TYPE)(VOS_UINT_PTR)PPP_HDLC_HARD_MEM_VIRT2PHY(nodePara->dataAddr);
        paraLinkNode->dataLen  = nodePara->dataLen;
        paraLinkNode->nextNode = VOS_NULL_PTR;

        if (nodeParaIndex != 0) {
            defBuffInfo->inputParaLinkNodeBuf[nodeParaIndex - 1].nextNode = (HDLC_ADDR_TYPE)(
                VOS_UINT_PTR)PPP_HDLC_HARD_BUF_VIRT2PHY(paraLinkNode);
        }
    }

    /* 统计信息更新 */
    hdlcStat                      = PPP_HDLC_HARD_DATA_PROC_STAT;
    hdlcStat->defMaxInputCntOnce  = TTF_MAX(hdlcStat->defMaxInputCntOnce, defPara->inputNodeParaCnt);
    hdlcStat->defMaxInputSizeOnce = TTF_MAX(hdlcStat->defMaxInputSizeOnce, defPara->inputTotalSize);

    /* 上报输入参数链表内容可维可测 */
    PPP_ServiceHdlcHardTraceDefInput(defPara, defBuffInfo);

    return VOS_OK;
}

/* 构造硬件解封装参数 */
VOS_VOID PPP_ServiceHdlcHardBuildDefPara(PPP_HDLC_HardDefPara *defPara, PPP_DRIVER_HdlcHardDefPara *drvDefPara)
{
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo = VOS_NULL_PTR;

    /* 内部调用，保证入参非空 */
    uncompleteInfo = HDLC_DEF_GET_UNCOMPLETED_INFO(defPara->pppId);

    drvDefPara->inputTotalSize = defPara->inputTotalSize;

    /* IP模式下不合入协议域，PPP模式下合入 */
    if (defPara->pppMode == PPP_SERVICE_HDLC_HARD_IP_MODE) {
        if (defPara->pppPcFlag == 1) {
            drvDefPara->pppPcFlag = HDLC_PROTOCOL_REMOVE_WITH_COMPRESS;
        }
    } else {
        drvDefPara->pppPcFlag = HDLC_PROTOCOL_NO_REMOVE;
    }

    if (defPara->pppAcFlag == 1) {
        drvDefPara->pppAcFlag = HDLC_ADDRESS_CTRL_COMPRESS;
    }

    drvDefPara->uncompleteInfo = uncompleteInfo;

    return;
}

/* 勾取解封装输出数据 */
STATIC VOS_VOID PPP_ServiceHdlcHardTraceDefOutput(VOS_UINT16 validFrameNum, HDLC_DefBuffInfo *defBuffInfo)
{
    HDLC_MNTN_DefOutputPara *outputPara = VOS_NULL_PTR;
    VOS_UINT32               dataLen;
    VOS_UINT32               copyLen;
    VOS_UINT32               nodeLoop;
    HDLC_DefRptNode         *rptNode = VOS_NULL_PTR;
    VOS_UINT16               maxFrameNum;
    VOS_UINT8               *dataAddr = VOS_NULL_PTR;

    /* 解封装上报空间可维可测 */
    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_PARA) == VOS_TRUE) {
        maxFrameNum = (HDLC_MNTN_ALLOC_MEM_MAX_SIZE - sizeof(HDLC_MNTN_DefOutputPara)) / sizeof(HDLC_DefRptNode);
        maxFrameNum = TTF_MIN(maxFrameNum, validFrameNum);

        copyLen = maxFrameNum * sizeof(HDLC_DefRptNode);

        dataLen    = sizeof(HDLC_MNTN_DefOutputPara) + copyLen;
        outputPara = (HDLC_MNTN_DefOutputPara*)PS_MEM_ALLOC(PS_PID_PPP_HDLC, dataLen);

        if (outputPara == VOS_NULL_PTR) {
            PPP_HDLC_WARNING_LOG1(
                "PPP_HDLC_HARD_MntnDefTraceOutput, NORMAL, Alloc mem failed ulDataLen <1>!\r\n", dataLen);
            return;
        }

        outputPara->defValidNum = validFrameNum;
        outputPara->traceNum    = maxFrameNum;

        if (copyLen > 0) {
            (VOS_VOID)memcpy_s(outputPara + 1, copyLen, (VOS_UINT8*)(&(defBuffInfo->rptNodeBuf[0])), copyLen);
        }

        PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)outputPara, ID_HDLC_MNTN_DEF_OUTPUT_PARA, dataLen);

        PS_MEM_FREE(PS_PID_PPP_HDLC, outputPara);
    }

    /* 解封装目的空间中每个有效帧可维可测 */
    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_DATA) == VOS_TRUE) {
        for (nodeLoop = 0; nodeLoop < validFrameNum; nodeLoop++) {
            rptNode  = &(defBuffInfo->rptNodeBuf[nodeLoop]);
            dataAddr = PPP_HDLC_HARD_BUF_PHY2VIRT(rptNode->defOutOneAddr);
            PPP_HDLC_HardMntnTraceSingleData(rptNode->defOutOneLen, dataAddr, ID_HDLC_MNTN_DEF_OUTPUT_DATA, nodeLoop);
        }
    }

    return;
}

/* 处理硬件封装结果 */
STATIC VOS_VOID PPP_ServiceHdlcHardProcDefResult(PPP_HDLC_HardDefPara *defPara,
    PPP_DRIVER_HdlcHardDefErrFramesCnt *errCnt)
{
    VOS_UINT16             validFrameNum = PPP_DriverHdlcHardGetDefVaildNum();
    HDLC_DefBuffInfo      *defBuffInfo = HDLC_DEF_GET_BUF_INFO(defPara->pppId);
    VOS_UINT32             frameLoop;
    HDLC_DefRptNode       *rptNode = VOS_NULL_PTR;
    PPP_HDLC_HardDefResult defResult = {0};
    VOS_UINT16             defOutLen;
    VOS_UINT8             *defOutOneAddr = VOS_NULL_PTR;
    VOS_UINT16             fistSegLen;

    /* 上报解封装后数据可维可测:上报空间信息、输出内容 */
    PPP_ServiceHdlcHardTraceDefOutput(validFrameNum, defBuffInfo);

    /* 有效帧数最大值检查 */
    if (validFrameNum > TTF_HDLC_DEF_RPT_MAX_NUM) {
        PPP_HDLC_WARNING_LOG2("PPP_HDLC_HARD_DefProcValidFrames, validFrameNum = <1> > TTF_HDLC_DEF_RPT_MAX_NUM = <2>",
            validFrameNum, TTF_HDLC_DEF_RPT_MAX_NUM);
        return;
    }

    g_pppHdlcHardStat.defMaxValidCntOnce = TTF_MAX(g_pppHdlcHardStat.defMaxValidCntOnce, validFrameNum);

    defResult.pppId = defPara->pppId;
    defResult.para  = defPara->para;

    for (frameLoop = 0; frameLoop < validFrameNum; frameLoop++) {
        rptNode   = &(defBuffInfo->rptNodeBuf[frameLoop]);
        defOutLen = rptNode->defOutOneLen;
        if ((defOutLen == 0) || (defOutLen > HDLC_DEF_OUT_PER_MAX_CNT)) {
            PPP_HDLC_WARNING_LOG1("invalid usDefOutOneLen", defOutLen);
            continue;
        }

        defOutOneAddr = PPP_HDLC_HARD_BUF_PHY2VIRT(rptNode->defOutOneAddr);

        /* 判断该帧起始加长度是否超过输出空间尾部，超出后按绕回处理 */
        if ((defOutOneAddr + defOutLen) > HDLC_DEF_OUTPUT_BUF_END_ADDR) {
            if (defOutOneAddr <= HDLC_DEF_OUTPUT_BUF_END_ADDR) {
                fistSegLen                     = (VOS_UINT16)(HDLC_DEF_OUTPUT_BUF_END_ADDR - defOutOneAddr);
                defResult.segment              = PS_TRUE;
                defResult.dataInfo[0].dataAddr = defOutOneAddr;
                defResult.dataInfo[0].dataLen  = fistSegLen;
                defResult.dataInfo[1].dataAddr = HDLC_DEF_OUTPUT_BUF_START_ADDR;
                defResult.dataInfo[1].dataLen  = defOutLen - fistSegLen;
            } else {
                PPP_HDLC_WARNING_LOG("Def Result Proc Err");
                continue;
            }
        } else {
            defResult.segment              = PS_FALSE;
            defResult.dataInfo[0].dataAddr = defOutOneAddr;
            defResult.dataInfo[0].dataLen  = defOutLen;
        }
        defResult.protocol = rptNode->defOutOnePro;
        PPP_SERVICE_HDLC_HARD_DEF_RES_PROC(&defResult);
    }

    PPP_SERVICE_HDLC_HARD_DEF_ERR_PROC(defPara->pppId, errCnt);
}

/* 保存解封装非完整帧信息 */
STATIC VOS_VOID PPP_ServiceHdlcHardDefSaveUncompleteInfo(VOS_UINT16 pppId)
{
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo = VOS_NULL_PTR;
    HDLC_DefBuffInfo                      *defBuffInfo    = VOS_NULL_PTR;
    VOS_UINT32                             validNum       = 0;
    VOS_UINT8                             *defOutOneAddr  = VOS_NULL_PTR;
    HDLC_DefRptNode                       *rptNode        = VOS_NULL_PTR;
    HDLC_MNTN_DefUncompletedInfo           mntnUncompletedInfo;

    uncompleteInfo = HDLC_DEF_GET_UNCOMPLETED_INFO(pppId);
    defBuffInfo    = HDLC_DEF_GET_BUF_INFO(pppId);

    PPP_DriverHdlcHardGetDefUncompletInfo(uncompleteInfo, &validNum);

    if (uncompleteInfo->existFlag == HDLC_DEF_UNCOMPLETED_NOT_EXIST) {
        return;
    }

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_PARA) == VOS_TRUE) {
        /* 勾取解封装输出的非完整帧信息 */
        (VOS_VOID)memcpy_s(&mntnUncompletedInfo.uncompletedInfo, sizeof(mntnUncompletedInfo.uncompletedInfo),
            uncompleteInfo, sizeof(PPP_DRIVER_HdlcHardDefUncompletedInfo));

        PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&mntnUncompletedInfo, ID_HDLC_MNTN_DEF_UNCOMPLETED_INFO,
            sizeof(HDLC_MNTN_DefUncompletedInfo));
    }

    /* 非完整帧的上报信息在有效帧后面，但是不算在有效帧数目内 */
    if (validNum >= TTF_HDLC_DEF_RPT_MAX_NUM) {
        PPP_HDLC_WARNING_LOG2("ulValidNum >= TTF_HDLC_DEF_RPT_MAX_NUM ", validNum, TTF_HDLC_DEF_RPT_MAX_NUM);

        uncompleteInfo->existFlag = HDLC_DEF_UNCOMPLETED_NOT_EXIST;

        return;
    }

    rptNode = &(defBuffInfo->rptNodeBuf[validNum]);

    defOutOneAddr = PPP_HDLC_HARD_BUF_PHY2VIRT(rptNode->defOutOneAddr);

    if (defOutOneAddr != HDLC_DEF_OUTPUT_BUF_START_ADDR) {
        if ((defOutOneAddr - HDLC_DEF_OUTPUT_BUF_START_ADDR) >= rptNode->defOutOneLen) {
            TTF_SF_CHK(memcpy_s(HDLC_DEF_OUTPUT_BUF_START_ADDR, TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN, defOutOneAddr,
                rptNode->defOutOneLen));
        } else {
            TTF_SF_CHK(memmove_s(HDLC_DEF_OUTPUT_BUF_START_ADDR, TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN, defOutOneAddr,
                rptNode->defOutOneLen));
        }

        rptNode->defOutOneAddr = (HDLC_ADDR_TYPE)(VOS_UINT_PTR)PPP_HDLC_HARD_BUF_VIRT2PHY(
            HDLC_DEF_OUTPUT_BUF_START_ADDR);
    }

    /* 非完整帧的协议、长度和在存储空间的地址，软件只是暂存这些信息，等下解封装的时候再原样配给HDLC */
    uncompleteInfo->defOutOnePro  = rptNode->defOutOnePro;
    uncompleteInfo->defOutOneLen  = rptNode->defOutOneLen;
    uncompleteInfo->defOutOneAddr = rptNode->defOutOneAddr;

    return;
}

/* 解封装硬件异常处理 */
STATIC VOS_VOID PPP_ServiceHdlcHardDefProcException(VOS_UINT32 status, VOS_UINT32 enableInterrupt)
{
    VOS_UINT32                  rawInt;
    PPP_DRIVER_HardwareDumpInfo dumpInfo;

    if (enableInterrupt == VOS_TRUE) {
        /* 由于在中断服务程序中进行了清中断操作，故此处取保存在g_hdlcRegSaveInfo中的原始中断寄存器值 */
        rawInt = g_hdlcRegSaveInfo.hdlcDefRawInt;
        g_pppHdlcHardStat.defExpInfo |= (1 << HDLC_INTERRUPT_IND_BITPOS);
    } else {
        rawInt = PPP_DriverHdlcHardGetDefRawInt();
    }

    PPP_HDLC_ERROR_LOG2("Def Exception ocurr", status, rawInt);

    g_pppHdlcHardStat.defExpInfo |= (1 << HDLC_EXCEPTION_IND_BITPOS);

    PPP_DriverHdlcHardGetDumpInfo(&dumpInfo);

    /* 如果HDLC出现异常，则单板异常重启 */
    mdrv_om_system_error(
        HDLC_DEF_SYSTEM_ERROR_ID, __LINE__, (VOS_INT)status, (VOS_CHAR*)&dumpInfo, (VOS_INT32)sizeof(dumpInfo));

    return;
}

/* 等待解封装暂停或完成，然后处理输出数据，可能会有多次停等的过程 */
STATIC VOS_VOID PPP_ServiceHdlcHardDefWaitAndProc(
    VOS_UINT32 enableInterrupt, PPP_DRIVER_HdlcHardDefPara *drvDefPara, PPP_HDLC_HardDefPara *defPara)
{
    VOS_UINT32                         defStatus;
    VOS_UINT32                         isContinue;
    PPP_DRIVER_HdlcHardDefErrFramesCnt errCnt;

    (VOS_VOID)memset_s(
        &errCnt, sizeof(PPP_DRIVER_HdlcHardDefErrFramesCnt), 0x00, sizeof(PPP_DRIVER_HdlcHardDefErrFramesCnt));

    for (;;) {
        /* 使能中断，则等待中断辅程序释放信号量；否则轮询解封装状态寄存器 */
        defStatus = PPP_DriverHdlcHardDefWaitResult(enableInterrupt);

        switch (defStatus) {
            case HDLC_DEF_STATUS_PAUSE_RPT_SPACE_FULL:
            case HDLC_DEF_STATUS_PAUSE_OUTPUT_SPACE_FULL:
                /* 处理有效帧，配置GO_ON寄存器 */
                PPP_ServiceHdlcHardProcDefResult(defPara, VOS_NULL_PTR);
                PPP_DriverHdlcHardDefCfgGoOnReg(defStatus);

                isContinue = VOS_TRUE;
                g_pppHdlcHardStat.defFullPauseCnt++;
                break;
            case HDLC_DEF_STATUS_PAUSE_LCP:
                /* 处理有效帧和LCP帧，更新配置寄存器，配置GO_ON寄存器 */
                PPP_ServiceHdlcHardProcDefResult(defPara, VOS_NULL_PTR);
                drvDefPara->uncompleteInfo = VOS_NULL_PTR;
                PPP_DriverHdlcHardDefCfgReg(drvDefPara);
                PPP_DriverHdlcHardDefCfgGoOnReg(defStatus);

                isContinue = VOS_TRUE;
                g_pppHdlcHardStat.defLcpPauseCnt++;
                break;
            case HDLC_DEF_STATUS_DONE_WITHOUT_FRAMES:
                /* 根据非完整帧指示，保存非完整帧信息 */
                PPP_ServiceHdlcHardDefSaveUncompleteInfo(defPara->pppId);

                isContinue = VOS_FALSE;
                break;
            case HDLC_DEF_STATUS_DONE_WITH_FRAMES:
                /* 处理有效帧、错误帧和LCP帧(可能是最后一帧)，根据非完整帧指示，保存非完整帧信息 */
                PPP_DriverHdlcHardGetDefErrorInfo(&errCnt);
                PPP_ServiceHdlcHardProcDefResult(defPara, &errCnt);
                PPP_ServiceHdlcHardDefSaveUncompleteInfo(defPara->pppId);
                isContinue = VOS_FALSE;
                break;
            case HDLC_DEF_STATUS_DOING:
            default:
                /* 打印异常日志，挂起PPP任务 */
                PPP_ServiceHdlcHardDefProcException(defStatus, enableInterrupt);

                isContinue = VOS_FALSE;
                break;
        }

        /* 暂停状态需要继续处理，其他状态解封装完成退出 */
        if (isContinue != VOS_TRUE) {
            break;
        }
    }

    g_hdlcRegSaveInfo.hdlcDefRawInt = 0xFFFFFFFFU;
    g_hdlcRegSaveInfo.hdlcDefStatus = 0xFFFFFFFFU;

    return;
}

/* 硬件方式IP类型解封装 */
PPP_HDLC_HardProcResultUint32 PPP_ServiceHdlcHardDefPacket(PPP_HDLC_HardDefPara *defPara)
{
    VOS_UINT32                 enableInterrupt = 0;
    VOS_UINT32                 updDefBufResutl;
    PPP_DRIVER_HdlcHardDefPara drvDefPara = {0};

    if (defPara == VOS_NULL_PTR) {
        PPP_HDLC_WARNING_LOG("pstFrmPara is NULL");
        return PPP_HDLC_HARD_PROC_PARA_ERR;
    }

    updDefBufResutl = PPP_ServiceHdlcHardUpdateDefBuffInfo(defPara);
    if (updDefBufResutl == VOS_ERR) {
        return PPP_HDLC_HARD_PROC_PARA_ERR;
    }
    PPP_ServiceHdlcHardBuildDefPara(defPara, &drvDefPara);

    PPP_DriverHdlcHardDefEnable(&drvDefPara, &enableInterrupt);

    /* 等待解封装暂停或完成，然后处理输出数据，可能会有多次停等的过程 */
    PPP_ServiceHdlcHardDefWaitAndProc(enableInterrupt, &drvDefPara, defPara);

    return PPP_HDLC_HARD_PROC_ALL_SUCC;
}

/* 获取解封装最大输入长度 */
VOS_UINT32 PPP_ServiceHdlcHardGetMaxDefLen(VOS_UINT16 pppId)
{
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo  = VOS_NULL_PTR;
    VOS_UINT32                             maxDataLen1Time = TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_SIZE;

    uncompleteInfo = HDLC_DEF_GET_UNCOMPLETED_INFO(pppId);
    if (uncompleteInfo->existFlag == HDLC_DEF_UNCOMPLETED_EXIST) {
        maxDataLen1Time -= uncompleteInfo->defOutOneLen;
    }

    return maxDataLen1Time;
}

#ifdef CONFIG_UART_SHELL
/* HDLCNOC测试使用，HDLC组帧 */
MODULE_EXPORTED VOS_VOID PPP_Servicenoctest(VOS_UINT_PTR readAddr, VOS_UINT_PTR writeAddr)
{
#define FRM_INPUT_SIZE (sizeof(frmInput))
#define FRM_OUTOUT_SIZE(inputSize) (2 * (inputSize + 13))

    VOS_UINT32             frmResult;
    PPP_HDLC_HardFrmPara   drvFrmPara    = {0};
    PPP_HDLC_HardFrmResult frmResultInfo = {0};
    VOS_UINT8              frmInput[]    = {
        0x1c, 0x00, 0x01, 0x01, 0x02, 0x07, 0x02, 0x08, 0x00, 0x00, 0x06, 0x02, 0x04, 0x01, 0x00, 0x00,
        0x06, 0x05, 0xdc, 0x05, 0x43, 0x05, 0x00, 0x00, 0x23, 0xc0, 0x04, 0x03
    };
    VOS_UINT8 frmOutput[FRM_OUTOUT_SIZE(FRM_INPUT_SIZE)];

    readAddr  = (VOS_NULL_PTR != readAddr) ? (VOS_UINT_PTR)PPP_HDLC_HardMemPhy2Virt(readAddr) : 0;
    writeAddr = (VOS_NULL_PTR != writeAddr) ? (VOS_UINT_PTR)PPP_HDLC_HardMemPhy2Virt(writeAddr) : 0;

    drvFrmPara.pppId     = 1;
    drvFrmPara.protocol  = PPP_SERVICE_HDLC_HARD_LCP;
    drvFrmPara.pppAcFlag = 0;
    drvFrmPara.pppPcFlag = 0;
    drvFrmPara.accmFlag  = 1;
    drvFrmPara.pppMode   = PPP_SERVICE_HDLC_HARD_IP_MODE;

    if (readAddr != VOS_NULL_PTR) {
        drvFrmPara.inputNodePara[0].dataAddr = (VOS_UINT8*)readAddr;
        TTF_SF_CHK(memcpy_s((VOS_UINT8*)readAddr, FRM_INPUT_SIZE, frmInput, FRM_INPUT_SIZE));
    } else {
        drvFrmPara.inputNodePara[0].dataAddr = (VOS_UINT8*)frmInput;
    }

    drvFrmPara.inputNodeParaCnt         = 1;
    drvFrmPara.inputNodePara[0].dataLen = FRM_INPUT_SIZE;
    drvFrmPara.inputTotalSize           = FRM_INPUT_SIZE;

    drvFrmPara.outputNodeParaCnt          = 1;
    drvFrmPara.outputNodePara[0].dataAddr = (writeAddr != VOS_NULL_PTR) ? (VOS_UINT8*)writeAddr : frmOutput;
    drvFrmPara.outputNodePara[0].dataLen  = sizeof(frmOutput);

    frmResult = PPP_ServiceHdlcHardFrmPacket(&drvFrmPara, &frmResultInfo);
    TTF_PRINT_DEBUG("hld noc frm result:%d.\n", frmResult);
}
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
