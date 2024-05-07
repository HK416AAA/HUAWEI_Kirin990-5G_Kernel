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

#include "hdlc_hardware.h"
#include "v_blk_mem.h"
#include "securec.h"
#include "ttf_util.h"
#include "hdlc_os_adaptor_interface.h"
#include "hdlc_pm_interface.h"
#include "ppp_init.h"
#include "ppp_input.h"
#include "hdlc_interface.h"
#include "hdlc_hardware_service.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_HDLC_C

/* ���װ�������뱨����󳤶�Ϊ�������ڴ����󳤶� */
#define PPP_HDLC_DEF_ONE_MAX_SIZE 2048

#define HDLC_MAKE_WORD(hi, low) ((VOS_UINT16)(((VOS_UINT8)(low)) | (((VOS_UINT16)((VOS_UINT8)(hi))) << 8)))
#define GET_BITS_FROM_BYTE(byte, bitMask) ((byte) & (bitMask))

#define INSERT_BYTE_LAST(mem, data, len, byte) do { \
    data[len]     = data[len - 1];         \
    data[len - 1] = byte;                  \
    PPP_ZC_SET_DATA_LEN(mem, 1);           \
} while (0)

__STATIC_ HDLC_CfgInfo g_hdlcCfg;

__STATIC_ PPP_DRIVER_HdlcHardDefErrFramesCnt g_hdlcErrStat = {0};

VOS_UINT32 g_pppMaxDelCnt = 3000;     /* HDLCһ����ദ��3000���ڵ� */
VOS_BOOL   g_pppDelayProc = VOS_TRUE; /* HDLCӲ��Ĭ����Ҫ�����ӳٴ��� */

/* �ͷŴ洢��PPP_ZC_STRU�ṹ */
__STATIC_ VOS_VOID PPP_HDLC_HardCommReleaseLink(PPP_HDLC_HardNodePara *linkNode, VOS_UINT32 relCnt)
{
    VOS_UINT32 nodeLoop;

    for (nodeLoop = 0; nodeLoop < relCnt; nodeLoop++) {
        if (linkNode[nodeLoop].dataNode != VOS_NULL_PTR) {
            PPP_MemFree((PPP_Zc*)(linkNode[nodeLoop].dataNode));

            linkNode[nodeLoop].dataNode = VOS_NULL_PTR;
        }
    }

    return;
}

/* �������֡ */
__STATIC_ VOS_VOID PPP_HDLC_HardDefProcErrorFrames(VOS_UINT16 pppId, PPP_DRIVER_HdlcHardDefErrFramesCnt *errCnt)
{
    PPP_DRIVER_HdlcHardDefErrFramesCnt *errStat = &g_hdlcErrStat;

    /* �����ڴ�����Ϣ */
    if ((errCnt == VOS_NULL_PTR) || (errCnt->errType == 0)) {
        return;
    }
    errStat->fcsErrCnt += errCnt->fcsErrCnt;
    errStat->lenLongCnt += errCnt->lenLongCnt;
    errStat->lenShortCnt += errCnt->lenShortCnt;
    errStat->errProtocolCnt += errCnt->errProtocolCnt;
    errStat->errCtrlCnt += errCnt->errCtrlCnt;
    errStat->errAddrCnt += errCnt->errAddrCnt;
    errStat->flagPosErrCnt += errCnt->flagPosErrCnt;
    errStat->errType = errCnt->errType;
}

/* �����ϱ���Ϣ��ַ�ͳ��ȣ������㿽���ڴ棬���������� */
__STATIC_ VOS_VOID PPP_HDLC_HardDefResultProc(PPP_HDLC_HardDefResult *defResult)
{
    PPP_Zc    *mem = VOS_NULL_PTR;
    VOS_UINT32 fistSegLen;
    VOS_UINT16 protocol;
    VOS_UINT16 defOutLen;

    if (defResult == VOS_NULL_PTR) {
        PPP_HDLC_WARNING_LOG("pstDefResult is NULL");
        return;
    }

    protocol = defResult->protocol;
    if (defResult->segment == PS_TRUE) {
        defOutLen = defResult->dataInfo[0].dataLen + defResult->dataInfo[1].dataLen;
    } else {
        defOutLen = defResult->dataInfo[0].dataLen;
    }

    mem = PPP_MemAlloc(defOutLen, PPP_ZC_UL_RESERVE_LEN);
    if (mem == VOS_NULL_PTR) {
        return;
    }

    PPP_ZC_TRACE_MEM(mem);
    PPP_StatOutputAllocMemCnt(PPP_DATA_DIR_UPLINK, 1);

    if (defResult->segment == PS_TRUE) {
        fistSegLen = defResult->dataInfo[0].dataLen;
        /* ��������ʼ��ַ������ռ�β�������� */
        PPPA_SF_CHK(memcpy_s(PPP_ZC_GET_DATA_PTR(mem), defOutLen, defResult->dataInfo[0].dataAddr, fistSegLen));

        /* ����������ռ��ײ���ʣ������ */
        PPPA_SF_CHK(memcpy_s(PPP_ZC_GET_DATA_PTR(mem) + fistSegLen, (defOutLen - fistSegLen),
            defResult->dataInfo[1].dataAddr, defResult->dataInfo[1].dataLen));
    } else {
        PPPA_SF_CHK(memcpy_s(PPP_ZC_GET_DATA_PTR(mem), defOutLen, defResult->dataInfo[0].dataAddr, defOutLen));
    }

    /* �����㿽�����ݳ��� */
    PPP_ZC_SET_DATA_LEN(mem, defOutLen);

    if (defResult->para == PPP_SERVICE_HDLC_HARD_IP_MODE) {
        PPP_HDLC_ProcIpModeUlData(defResult->pppId, mem, protocol);
    } else {
        PPP_SendUlDataToUu(defResult->pppId, mem, 0);
    }

    return;
}

/* ��鹹���������������ز����Ƿ�Ϸ� */
__STATIC_ PPP_HDLC_ParaCheckResultUint32 PPP_HDLC_HardDefCheckPara(PPP_Zc *mem, VOS_UINT32 dataLen, PPP_DataTypeUint8 dataType)
{
    PPP_DataTypeUint8 currDataType;

    /* ��ȡ����װ���ݰ����� */
    currDataType = (PPP_ZC_GET_DATA_APP(mem) & 0x00FF);

    if (dataType != currDataType) {
        return PPP_HDLC_PARA_CHECK_FAIL_KEEP;
    }

    /* ���װ������볤�����ڴ�ģ���������󳤶ȣ�Ŀǰ��1536B,���HDLC Bug�ڴ��������1 */
    if ((dataLen == 0) || (dataLen > HDLC_DEF_IN_PER_MAX_CNT)) {
        PPP_HDLC_WARNING_LOG1("PPP_HDLC_HardDefCheckPara, NORMAL, invalid data length <1>\r\n", dataLen);

        return PPP_HDLC_PARA_CHECK_FAIL_DISCARD;
    }

    return PPP_HDLC_PARA_CHECK_PASS;
}

/* ���HDLCӲ��Bug */
__STATIC_ VOS_VOID PPP_HDLC_HardForbiddenHdlcBug(PPP_Zc** ppstMem)
{
    PPP_Zc          *mem     = VOS_NULL_PTR;
    PPP_Zc          *tmpMem  = VOS_NULL_PTR;
    VOS_UINT8       *data    = VOS_NULL_PTR;
    VOS_UINT8       *tmpData = VOS_NULL_PTR;
    VOS_UINT16       len;
    const VOS_UINT8  endByte       = 0x7e;
    const VOS_UINT16 addEndByteLen = sizeof(endByte);

    mem = *ppstMem;

    if (mem == VOS_NULL_PTR) {
        return;
    }

    len  = PPP_ZC_GET_DATA_LEN(mem);
    data = PPP_ZC_GET_DATA_PTR(mem);

    if (len <= 1) {
        return;
    }

    if (endByte != data[len - 2]) {
        /* �����ڶ����ֽڲ�Ϊ7e���ô��� */
        return;
    } else if ((len >= 3) && (endByte == data[len - 3])) {
        /* �����������ֽ�Ϊ7e���ô��� */
        return;
    } else {
        /*
         * �����ڶ����ֽ�Ϊ7e,�����������ֽڲ�Ϊ7e�ĳ���
         * �����һ���ֽں�����һ���ֽ�,ͬʱ�ڴ�ĳ���+1
         *
         * skb������ڴ�ṹ����
         * |--------data(uslen)--------|----32bytes align(0~31)-----|--skb_share_info(256)----|
         */
        if (PPP_ZC_GET_RESERVE_ROOM(mem) > 0) {
            INSERT_BYTE_LAST(mem, data, len, endByte);
            g_pppHdlcHardStat.forbidHdlcBugNoCpy++;
        } else {
            /* ����һ������1�ֽڵ��ڴ� */
            tmpMem = PPP_MemAlloc((VOS_UINT16)(len + addEndByteLen), 0);
            if (tmpMem != VOS_NULL_PTR) {
                PPP_ZC_TRACE_MEM(tmpMem);
                tmpData = PPP_ZC_GET_DATA_PTR(tmpMem);
                PPPA_SF_CHK(memcpy_s(tmpData, (len + addEndByteLen), data, len));
                PPP_ZC_SET_DATA_LEN(tmpMem, len);

                INSERT_BYTE_LAST(tmpMem, tmpData, len, endByte);
                g_pppHdlcHardStat.forbidHdlcBugCpy++;
            }

            /* ������������ڴ��Ƿ�Ϊ�ն�����ȥ,�����洦�� */
            *ppstMem = tmpMem;

            /* ��ԭ�ڴ��ͷ� */
            PPP_MemFree(mem);
        }
    }
}

STATIC VOS_VOID PPP_HDLC_FillHardDefPara(PPP_HDLC_HardDefPara *defPara, const HDLC_ParaLinkBuildPara *buildPara)
{
    defPara->pppId     = buildPara->pppId;
    defPara->pppAcFlag = g_hdlcCfg.myAcf;
    defPara->pppPcFlag = g_hdlcCfg.myPcf;
    defPara->pppMode   = buildPara->pppMode;
    defPara->para      = buildPara->pppMode;
}

STATIC VOS_VOID PPP_HDLC_FillHardDefInputPara(PPP_HDLC_HardDefPara *defPara, const PPP_Zc *mem, VOS_UINT16 dataLen,
    VOS_UINT32 nodeCnt)
{
    defPara->inputNodePara[nodeCnt].dataAddr = PPP_ZC_GET_DATA_PTR(mem);
    defPara->inputNodePara[nodeCnt].dataLen  = dataLen;
    defPara->inputNodePara[nodeCnt].dataNode = mem;
    defPara->inputTotalSize += dataLen;
}

/* ������װ����������� */
__STATIC_ VOS_UINT32 PPP_HDLC_HardDefBuildInputParaLink(HDLC_ParaLinkBuildPara *buildPara,
    PPP_HDLC_HardDefPara *defPara, VOS_UINT32 *dealCnt)
{
    PPP_Zc                        *mem = VOS_NULL_PTR;
    PPP_HDLC_ParaCheckResultUint32 checkResult;
    VOS_UINT16                     dataLen;
    VOS_UINT32                     nodeCnt = 0;
    VOS_UINT32                     maxDataLen1Time;
    VOS_UINT32                     dropCnt = 0;

    PPP_HDLC_FillHardDefPara(defPara, buildPara);
    maxDataLen1Time = PPP_ServiceHdlcHardGetMaxDefLen(buildPara->pppId);

    while (nodeCnt < TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM) {
        mem = (PPP_Zc*)PPP_ZC_PEEK_QUEUE_HEAD(buildPara->dataQ);
        /* ��û�е����ڵ��������Ƕ������Ѿ�û������ */
        if (mem == VOS_NULL_PTR) {
            break;
        }

        dataLen     = PPP_ZC_GET_DATA_LEN(mem);
        checkResult = PPP_HDLC_HardDefCheckPara(mem, dataLen, buildPara->dataType);

        if (checkResult == PPP_HDLC_PARA_CHECK_FAIL_DISCARD) {
            mem = PPP_ZC_DEQUEUE_HEAD(buildPara->dataQ);
            PPP_MemFree(mem);
            dropCnt++;
            g_pppHdlcHardStat.defInputDiscardCnt++;
            continue;
        }

        if (checkResult == PPP_HDLC_PARA_CHECK_FAIL_KEEP) {
            break;
        }

        /* ��PPP����ȡ��ͷ��㣬������������� pstBuildInfo->apstInputLinkNode */
        mem = PPP_ZC_DEQUEUE_HEAD(buildPara->dataQ);
        /* ���HDLC BUG,�ڷ���HDLC BUG�����¶�ԭ�����ݽ����滻,�����pstMemΪ�滻����ڴ�ָ��,ԭʼ�ڴ�ָ������Ѿ����ͷ� */
        PPP_HDLC_HardForbiddenHdlcBug(&mem);
        /* �滻�ڴ��ʱ�������ڴ�ʧ��,ֱ�Ӵ�����һ������  */
        if (mem == VOS_NULL_PTR) {
            continue;
        }

        /* �ڴ���ܷ����滻,����ȡ���� */
        dataLen = PPP_ZC_GET_DATA_LEN(mem);
        /* һ������������д���װ���ݰ��ܳ����ܳ���15KB,���HDLC BUG,�������ݱ��޸Ĺ�,����������1�ֽ�,��Ҫ�ŵ������ж� */
        if (defPara->inputTotalSize + dataLen > maxDataLen1Time) {
            /* �ڴ�Żض���,�´��ٴ��� */
            PPP_ZC_ENQUEUE_HEAD(buildPara->dataQ, mem);
            break;
        }

        /* ��д������������������ */
        PPP_HDLC_FillHardDefInputPara(defPara, mem, dataLen, nodeCnt);

        /* ��Ҫ������д��DDR��HDLC��DDR�ж����� */
        PPP_HDLC_CacheFlush(PPP_ZC_GET_DATA_PTR(mem), PPP_ZC_GET_DATA_LEN(mem));
        nodeCnt++;
    }

    defPara->inputNodeParaCnt = nodeCnt;
    *dealCnt                  = nodeCnt + dropCnt;
    return ((nodeCnt == 0) ? VOS_ERR : VOS_OK);
}

/* Ӳ����ʽIP���ͽ��װ */
__STATIC_ VOS_UINT32 PPP_HDLC_HardDefPacket(HDLC_ParaLinkBuildPara *buildPara, VOS_UINT32 *dealCnt)
{
    VOS_UINT32           buildResult;
    PPP_HDLC_HardDefPara defPara = {0};

    /* ������������������ϱ������ڴ��ά�ɲ� */
    buildResult = PPP_HDLC_HardDefBuildInputParaLink(buildPara, &defPara, dealCnt);
    if (buildResult != VOS_OK) {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HardDefPacket, WARNING, BuildParaLink Fail.\r\n");
        return buildResult;
    }

    PPP_ServiceHdlcHardDefPacket(&defPara);

    /* �ͷ��ѽ��װ��ɵ����� */
    PPP_HDLC_HardCommReleaseLink(defPara.inputNodePara, defPara.inputNodeParaCnt);
    PPP_StatInputDataFreeCnt(PPP_PULL_PACKET_TYPE, defPara.inputNodeParaCnt);

    return VOS_OK;
}

/* ��װ���Ŀ�Ŀռ����� */
__STATIC_ VOS_UINT32 PPP_HDLC_HardFrmOutputMemAlloc(VOS_UINT16 len, PPP_HDLC_HardFrmPara *frmPara)
{
    VOS_UINT16             frmedMaxLen; /* ��װ��Ŀ��ܵ���󳤶� */
    VOS_UINT16             allocLen;
    VOS_UINT32             outputNodeCnt   = 0;
    VOS_UINT32             outputNodeIndex = 0;
    VOS_UINT32             loopCnt;
    PPP_Zc                *mem                                                 = VOS_NULL_PTR;
    PPP_Zc                *apstOutputNode[PPP_HDLC_MAX_OUT_NODE_FOR_ONE_INPUT] = { VOS_NULL_PTR, VOS_NULL_PTR };
    PPP_HDLC_HardNodePara *nodePara                                            = VOS_NULL_PTR;
    VOS_UINT16             ausOutputNodeLen[PPP_HDLC_MAX_OUT_NODE_FOR_ONE_INPUT];

    /* ��װ����ܵ�������ݳ���(2*ԭʼ���ݳ���+13B) */
    frmedMaxLen = (VOS_UINT16)HDLC_FRM_GET_MAX_FRAMED_LEN(len);
    loopCnt     = TTF_CEIL(frmedMaxLen, PPP_ZC_MAX_DATA_LEN);

    while (outputNodeCnt < loopCnt) {
        allocLen = PPP_ZC_MAX_DATA_LEN;
        mem      = PPP_MemAlloc(allocLen, PPP_ZC_DL_RESERVE_LEN);

        if (mem == VOS_NULL_PTR) {
            break;
        }

        PPP_ZC_TRACE_MEM(mem);

        apstOutputNode[outputNodeCnt]   = mem;
        ausOutputNodeLen[outputNodeCnt] = allocLen;

        outputNodeCnt++;
    }

    /* ����������ڴ�ʧ�ܵ���������ͷű����Ѿ�������ڴ� */
    if (outputNodeCnt < loopCnt) {
        if (apstOutputNode[0] != VOS_NULL_PTR) {
            PPP_MemFree(apstOutputNode[0]);
        }
        return VOS_ERR;
    }

    PPP_StatOutputAllocMemCnt(PPP_DATA_DIR_DOWNLINK, outputNodeCnt);

    for (outputNodeIndex = 0; outputNodeIndex < outputNodeCnt; outputNodeIndex++) {
        /* ����װ�ڵ���ڴ���Ϣ���浽Ӳ����װ�ڵ�����б��� */
        nodePara           = &(frmPara->outputNodePara[frmPara->outputNodeParaCnt + outputNodeIndex]);
        nodePara->dataAddr = PPP_ZC_GET_DATA_PTR(apstOutputNode[outputNodeIndex]);
        nodePara->dataLen  = ausOutputNodeLen[outputNodeIndex];
        /* ���ڵ���Ϣ���棬�����Ҫ���˽����ڴ��ͷŵȲ��� */
        nodePara->dataNode = apstOutputNode[outputNodeIndex];

        PPP_HDLC_CacheInvalid(nodePara->dataAddr, ausOutputNodeLen[outputNodeIndex]);
    }

    frmPara->outputNodeParaCnt += outputNodeCnt;

    return VOS_OK;
}

/* PPPģʽ�»�ȡ����װ���ݰ�Э��ֵ */
__STATIC_ VOS_UINT32 PPP_HDLC_HardFrmGetProtocol(PPP_Zc *mem, VOS_UINT16 *protocol)
{
    VOS_UINT8  proto[2] = { 0, 0 };
    VOS_UINT32 rtn;

    rtn = PPP_MemGet(mem, 0, proto, 2);

    if (rtn == PS_FAIL) {
        return VOS_ERR;
    }

    if (0x01 == GET_BITS_FROM_BYTE(proto[0], 0x01)) {
        *protocol = HDLC_MAKE_WORD(0x00, proto[0]);
    } else {
        if (0x01 == GET_BITS_FROM_BYTE(proto[1], 0x01)) {
            *protocol = HDLC_MAKE_WORD(proto[0], proto[1]);
        } else {
            PPP_HDLC_WARNING_LOG("PPP_HDLC_HardFrmGetProtocol, Warning, PPP mode framer data protocol error\r\n");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/* ��鹹�������������������ز����Ƿ�Ϸ� */
__STATIC_ PPP_HDLC_ParaCheckResultUint32 PPP_HDLC_HardFrmCheckPara(PPP_Zc *mem, VOS_UINT32 dataLen, PPP_DataTypeUint8 dataType,
    VOS_UINT16 protocol)
{
    PPP_DataTypeUint8 currDataType;
    VOS_UINT32        rlst;
    VOS_UINT16        currProtocol;

    /* ��ȡ����װ���ݰ����� */
    currDataType = (PPP_ZC_GET_DATA_APP(mem) & 0x00FF);

    if (dataType != currDataType) {
        return PPP_HDLC_PARA_CHECK_FAIL_KEEP;
    }

    /* ����װ���ݰ������쳣 */
    if ((0 == dataLen) || (HDLC_FRM_IN_PER_MAX_CNT < dataLen)) {
        PPP_HDLC_WARNING_LOG1("PPP_HDLC_HardFrmCheckPara,IP mode framer data len wrong <1>\r\n", dataLen);

        return PPP_HDLC_PARA_CHECK_FAIL_DISCARD;
    }

    /* �������PPPģʽ */
    if (currDataType != PPP_PUSH_RAW_DATA_TYPE) {
        return PPP_HDLC_PARA_CHECK_PASS;
    }

    rlst = PPP_HDLC_HardFrmGetProtocol(mem, &currProtocol);

    if (rlst != VOS_OK) {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HardFrmCheckPara, Warning, PPP mode get framer data protocol error\r\n");
        return PPP_HDLC_PARA_CHECK_FAIL_DISCARD;
    }

    /* ��֤һ���������ù����з�װЭ��ֵһ�� */
    if (protocol != currProtocol) {
        return PPP_HDLC_PARA_CHECK_FAIL_KEEP;
    }

    return PPP_HDLC_PARA_CHECK_PASS;
}

STATIC VOS_VOID PPP_HDLC_FillHardFrmPara(PPP_HDLC_HardFrmPara *frmPara, const HDLC_ParaLinkBuildPara *buildPara)
{
    frmPara->pppId     = buildPara->pppId;
    frmPara->protocol  = buildPara->protocol;
    frmPara->pppAcFlag = g_hdlcCfg.hisAcf;
    frmPara->pppPcFlag = g_hdlcCfg.hisPcf;
    frmPara->accmFlag  = g_hdlcCfg.hisAccm;
    frmPara->pppMode   = buildPara->pppMode;
}

STATIC VOS_VOID PPP_HDLC_FillHardFrmInputNode(PPP_HDLC_HardFrmPara *frmPara, const PPP_Zc *mem, VOS_UINT16 unFrmLen,
    VOS_UINT32 nodeCnt)
{
    frmPara->inputNodePara[nodeCnt].dataAddr = PPP_ZC_GET_DATA_PTR(mem);
    frmPara->inputNodePara[nodeCnt].dataLen  = unFrmLen;
    frmPara->inputTotalSize += unFrmLen;
    frmPara->inputNodePara[nodeCnt].dataNode = mem;
}

/* ����������������������ϱ������ڴ��ά�ɲ� */
__STATIC_ VOS_UINT32 PPP_HDLC_HardFrmBuildParaLink(HDLC_ParaLinkBuildPara *buildPara, PPP_HDLC_HardFrmPara *frmPara,
    VOS_UINT32 *dealCnt)
{
    PPP_Zc                        *mem = VOS_NULL_PTR;
    VOS_UINT16                     unFrmLen;
    VOS_UINT32                     allocResult;
    VOS_UINT32                     nodeCnt = 0;
    PPP_HDLC_ParaCheckResultUint32 checkResult;
    VOS_UINT32                     allocMemFail;
    VOS_UINT32                     dropCnt = 0;

    PPP_HDLC_FillHardFrmPara(frmPara, buildPara);

    frmPara->inputNodeParaCnt = 0;
    allocMemFail              = VOS_FALSE;
    while (nodeCnt < TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM) {
        mem = (PPP_Zc*)PPP_ZC_PEEK_QUEUE_HEAD(buildPara->dataQ);
        if (mem == VOS_NULL_PTR) {
            break;
        }

        unFrmLen    = PPP_ZC_GET_DATA_LEN(mem);
        checkResult = PPP_HDLC_HardFrmCheckPara(mem, unFrmLen, buildPara->dataType, buildPara->protocol);

        if (checkResult == PPP_HDLC_PARA_CHECK_FAIL_DISCARD) {
            mem = PPP_ZC_DEQUEUE_HEAD(buildPara->dataQ);
            PPP_MemFree(mem);
            dropCnt++;
            g_pppHdlcHardStat.frmInputDiscardCnt++;
            continue;
        }

        if (checkResult == PPP_HDLC_PARA_CHECK_FAIL_KEEP) {
            break;
        }

        /* һ������������д���װ���ݰ��ܳ����ܳ���15KB */
        if (frmPara->inputTotalSize + unFrmLen > TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_SIZE) {
            break;
        }

        /* �����װĿ�Ŀռ� */
        allocResult = PPP_HDLC_HardFrmOutputMemAlloc(unFrmLen, frmPara);

        if (allocResult != VOS_OK) {
            allocMemFail = VOS_TRUE;
            g_pppHdlcHardStat.frmAllocOutputMemFailCnt++;
            break;
        }

        /* ��PPP����ȡ��ͷ��㣬������������� pstBuildInfo->apstInputLinkNode */
        mem = PPP_ZC_DEQUEUE_HEAD(buildPara->dataQ);
        PPP_HDLC_FillHardFrmInputNode(frmPara, mem, unFrmLen, nodeCnt);
        /* ��Ҫ������д��DDR��HDLC��DDR�ж����� */
        PPP_HDLC_CacheFlush(PPP_ZC_GET_DATA_PTR(mem), PPP_ZC_GET_DATA_LEN(mem));
        nodeCnt++;
    }

    frmPara->inputNodeParaCnt = nodeCnt;
    *dealCnt                  = nodeCnt + dropCnt;
    return ((nodeCnt == 0) ? VOS_ERR : VOS_OK);
}

/* ������Ч֡�����ͷ�ʣ���ڴ� */
__STATIC_ VOS_UINT32 PPP_HDLC_HardFrmResultProc(PPP_HDLC_HardFrmResult *frmResult, PPP_HDLC_HardFrmPara *frmPara)
{
    PPP_Zc    *mem = VOS_NULL_PTR;
    VOS_UINT32 rptSpaceIndex;
    VOS_UINT32 outputLinkIndex = 0;
    VOS_UINT16 frmOutLen;
    VOS_UINT16 dataLen;

    for (rptSpaceIndex = 0; rptSpaceIndex < frmResult->frmValidCnt; rptSpaceIndex++) {
        frmOutLen = frmResult->frmResultNode[rptSpaceIndex].dataLen;

        while (frmOutLen > 0) {
            /* ��pstBuildInfo->apstOutputLinkNodeȡ��ͷ��� pstMem */
            mem = (PPP_Zc*)(frmPara->outputNodePara[outputLinkIndex].dataNode);

            /* ���������������ָĿ�Ŀռ�Ĵ�С */
            dataLen = frmPara->outputNodePara[outputLinkIndex].dataLen;

            if (frmOutLen > dataLen) {
                PPP_ZC_SET_DATA_LEN(mem, dataLen);
                frmOutLen -= dataLen;
            } else {
                PPP_ZC_SET_DATA_LEN(mem, frmOutLen);
                frmOutLen = 0;
            }
            PPP_SEND_NORMAL_PRIORITY_DATA_TO_AT((PPP_Id)frmPara->pppId, mem);

            /* ������������ݰ���apstOutputLinkNode����� */
            frmPara->outputNodePara[outputLinkIndex].dataNode = VOS_NULL_PTR;

            outputLinkIndex++;
        }
    }

    return outputLinkIndex;
}

/* IPģʽ���з�װ�������� */
__STATIC_ VOS_UINT32 PPP_HDLC_HardFrmPacket(HDLC_ParaLinkBuildPara *buildPara, VOS_UINT32 *dealCnt)
{
    VOS_UINT32                    buildResult;
    VOS_UINT32                    validFrmNum = 0;
    PPP_HDLC_HardFrmPara          frmPara = {0};
    PPP_HDLC_HardFrmResult        frmResult = {0};
    PPP_HDLC_HardProcResultUint32 procResult;

    /* ����������������������ϱ������ڴ��ά�ɲ� */
    buildResult = PPP_HDLC_HardFrmBuildParaLink(buildPara, &frmPara, dealCnt);
    if (buildResult != VOS_OK) {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HardFrmPacket, WARNING, BuildParaLink Fail.\r\n");
        return buildResult;
    }

    procResult = PPP_ServiceHdlcHardFrmPacket(&frmPara, &frmResult);
    if ((procResult == PPP_HDLC_HARD_PROC_ALL_SUCC) || (procResult == PPP_HDLC_HARD_PROC_PART_SUCC)) {
        validFrmNum = PPP_HDLC_HardFrmResultProc(&frmResult, &frmPara);
    }

    /* �ͷŷ�װ�����δʹ���ڴ漰��װ���������е��ڴ� */
    PPP_HDLC_HardCommReleaseLink(frmPara.inputNodePara, frmPara.inputNodeParaCnt);
    PPP_HDLC_HardCommReleaseLink(frmPara.outputNodePara, frmPara.outputNodeParaCnt);

    PPP_StatInputDataFreeCnt(PPP_PUSH_PACKET_TYPE, frmPara.inputNodeParaCnt);
    PPP_StatOutputFreeCntBeforeSnd(frmPara.outputNodeParaCnt - validFrmNum);

    return VOS_OK;
}

#ifdef CONFIG_UART_SHELL
MODULE_EXPORTED VOS_VOID PPP_SetMaxDelCnt(VOS_UINT32 cnt)
{
    g_pppMaxDelCnt = cnt;
}
MODULE_EXPORTED VOS_VOID PPP_SetDelayProc(VOS_UINT32 delayProc)
{
    g_pppDelayProc = delayProc;
}
#endif

STATIC VOS_UINT32 PPP_HDLC_PacketTypeProc(PPP_Zc *mem, HDLC_ParaLinkBuildPara *buildPara, VOS_UINT16 ipProtocol,
    VOS_UINT32 *dealResult, VOS_UINT32 *dealCurCnt)
{
    VOS_UINT16 protocol;

    switch (buildPara->dataType) {
        case PPP_PULL_PACKET_TYPE:
            buildPara->protocol = ipProtocol;
            buildPara->pppMode  = PPP_SERVICE_HDLC_HARD_IP_MODE;
            (*dealResult)       = PPP_HDLC_HardDefPacket(buildPara, dealCurCnt);

            g_pppHdlcHardStat.defIpDataProcCnt += (*dealCurCnt);
            break;
        case PPP_PUSH_PACKET_TYPE:
            buildPara->protocol = ipProtocol;
            buildPara->pppMode  = PPP_SERVICE_HDLC_HARD_IP_MODE;
            (*dealResult)       = PPP_HDLC_HardFrmPacket(buildPara, dealCurCnt);

            g_pppHdlcHardStat.frmIpDataProcCnt += (*dealCurCnt);
            break;
        case PPP_PULL_RAW_DATA_TYPE:
            buildPara->protocol = PPPA_IP;
            buildPara->pppMode  = PPP_SERVICE_HDLC_HARD_PPP_MODE;
            (*dealResult)       = PPP_HDLC_HardDefPacket(buildPara, dealCurCnt);

            g_pppHdlcHardStat.defIpDataProcCnt += (*dealCurCnt);
            break;
        case PPP_PUSH_RAW_DATA_TYPE:
            /* ��ȡ����װ���ݰ���Э��ֵusProtocol */
            if (PPP_HDLC_HardFrmGetProtocol(mem, &protocol) == VOS_OK) {
                buildPara->protocol = protocol;
                buildPara->pppMode  = PPP_SERVICE_HDLC_HARD_PPP_MODE;
                (*dealResult)       = PPP_HDLC_HardFrmPacket(buildPara, dealCurCnt);

                g_pppHdlcHardStat.frmPppDataProcCnt += (*dealCurCnt);
            } else {
                return VOS_ERR;
            }
            break;
        default:
            PPP_HDLC_WARNING_LOG1("PPP_HDLC_HardProcData, WARNING, ucDataType <1> is Abnormal!", buildPara->dataType);
            return VOS_ERR;
    }

    return VOS_OK;
}

VOS_BOOL PPP_HDLC_HardProcDataOnce(PPP_Id pppId, PPP_ZcQueue *dataQ)
{
    PPP_Zc                *mem          = VOS_NULL_PTR;
    VOS_UINT8             *dataSrc      = VOS_NULL_PTR;
    VOS_UINT32             dealTotalCnt = 0;
    VOS_UINT32             dealCurCnt   = 0;
    VOS_UINT32             dealResult;
    VOS_UINT16             protocol;
    PPP_DataTypeUint8      dataType;
    HDLC_ParaLinkBuildPara buildPara    = {0};
    VOS_UINT32             loopCnt      = 0;
    VOS_UINT32             ipVersion;
    VOS_UINT16             ipProtocol;
    g_pppHdlcHardStat.hdlcHardProcCnt++;

    buildPara.pppId = pppId;
    buildPara.dataQ = dataQ;

    /* һ����ദ�� PPP_ONCE_DEAL_MAX_CNT ���ڵ� */
    while ((dealTotalCnt < g_pppMaxDelCnt) && (++loopCnt <= g_pppMaxDelCnt)) {
        mem = (PPP_Zc*)PPP_ZC_PEEK_QUEUE_HEAD(dataQ);
        /* ����Ϊ������Ҫ�������� */
        if (mem == VOS_NULL_PTR) {
            return VOS_FALSE;
        }
        if (PPP_ZC_GET_DATA_LEN(mem) == 0) {
            mem = (PPP_Zc*)PPP_ZC_DEQUEUE_HEAD(dataQ);
            PPP_MemFree(mem);
            continue;
        }
        dataSrc    = PPP_ZC_GET_DATA_PTR(mem);
        ipVersion  = PPP_GET_IP_VERSION(dataSrc);
        ipProtocol = PPP_GET_IP_PROTOCOL(ipVersion);

        /* ����ý��(�����ͷŶ����Ѿ��ڸ��������ڲ���ɣ��������ͷŽ��) */
        dataType   = (PPP_ZC_GET_DATA_APP(mem) & 0x00FF);
        dealResult = VOS_OK;
        dealCurCnt = 0;

        buildPara.dataType = dataType;
        if (PPP_HDLC_PacketTypeProc(mem, &buildPara, ipProtocol, &dealResult, &dealCurCnt) != VOS_OK) {
            mem = (PPP_Zc*)PPP_ZC_DEQUEUE_HEAD(dataQ);
            PPP_MemFree(mem);
        }
        /* ͳ�� */
        dealTotalCnt += dealCurCnt;
        g_pppHdlcHardStat.maxCntOnce = TTF_MAX(g_pppHdlcHardStat.maxCntOnce, dealTotalCnt);
        /* ����ʧ������Ҫ�ӳٴ�����д��� */
        if (dealResult != VOS_OK) {
            PPP_HDLC_WARNING_LOG("PPP_HDLC_HardProcData, WARNING, ulDealResult = ERROR!");
            return VOS_TRUE;
        }
    }

    /* ����һ�������ڵ����Ҫ�ӳ�һ��ʱ��ճ�PPP������������Ϣ */
    g_pppHdlcHardStat.continueCnt++;
    return g_pppDelayProc;
}

VOS_BOOL PPP_HDLC_HardProcData(PPP_Id pppId, PPP_ZcQueue *dataQ)
{
    VOS_BOOL ret;

    HDLC_PM_LOCK();
    ret = PPP_HDLC_HardProcDataOnce(pppId, dataQ);
    HDLC_PM_UNLOCK();

    return ret;
}

/* ��ʼHDLC */
VOS_UINT32 PPP_HDLC_HardInit(VOS_VOID)
{
    PPP_HDLC_HardConfigInfo        hdlcConfig = {0};
    PPP_ServiceHdlcHardDefCallBack defCBInfo;

    hdlcConfig.userPid         = PS_PID_APP_PPP;
    hdlcConfig.pppTotalBufLen  = PPP_HDLC_GetAlignBufLen(TTF_HDLC_LINK_TOTAL_LEN);
    hdlcConfig.defOneMaxSize   = PPP_HDLC_DEF_ONE_MAX_SIZE;

    PPP_HDLC_FillIoConfig(&hdlcConfig);

    defCBInfo.defErrProcFunc = PPP_HDLC_HardDefProcErrorFrames;
    defCBInfo.defResProcFunc = PPP_HDLC_HardDefResultProc;

    if (PPP_ServiceHdlcHardInit(&hdlcConfig, &defCBInfo) != VOS_OK) {
        return VOS_ERR;
    }

    HDLC_PM_INIT();

    PPP_HDLC_HardwareDeviceReg();

    PPP_HDLC_HardCfgInit();
    return VOS_OK;
}

VOS_VOID PPP_HDLC_HardCfgInit(VOS_VOID)
{
    g_hdlcCfg.hisAcf = VOS_FALSE;
    g_hdlcCfg.hisPcf = VOS_FALSE;
    g_hdlcCfg.hisAccm = PPPA_DEFAULT_ACCM;
    g_hdlcCfg.myAcf  = VOS_FALSE;
    g_hdlcCfg.myPcf  = VOS_FALSE;
}

VOS_VOID PPP_HDLC_HardUpdateCfg(HDLC_CfgInfo *hdlcCfg)
{
    g_hdlcCfg = *hdlcCfg;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
