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

#include "ltev_datatest.h" 

#include <jiffies.h>
#include <kthread.h>
#include <math64.h>
#include <netdevice.h>
#include <securec.h>
#include <semaphore.h>
#include <skbuff.h>
#include <timer.h>

#include "mdrv_pc5_adaptor.h" 
#include "mdrv_print.h"
#include "mdrv.h"
#include <bsp_softtimer.h>

#include "imm_interface.h"

#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>


#define mod_ltevtest "letvtest"
#define THIS_MODU mod_ltevtest

#define OK 0
#define ERR (-1)

#define PRINT_ERR(fmt, ...) (mdrv_err(fmt, ##__VA_ARGS__))

#define MIN(a, b) ((a) < (b)) ? (a) : (b)

#define LTEV_DATA_MAGIC_NUM 0x5A5A

#define LTEV_DBG_STAT 1
#define LTEV_DBG_SEQ  2

typedef struct {
    unsigned long statTime;
    unsigned long lostCntPer;
    unsigned long sendCntPer;
    unsigned long rcvCntPer;
    unsigned long sendBytesPer;
    unsigned long rcvBytesPer;
    unsigned long seqErrCntPer;
    unsigned long totalLostCnt;
    unsigned long totalSendCnt;
    unsigned long totalRcvCnt;
    unsigned long totalSendBytes;
    unsigned long totalRcvBytes;
    unsigned long seqErrCnt;
}StatInfo;
#define MAX_RECV_SEQ_CNT 256
typedef struct {
    struct timer_list statTimer;
    struct softtimer_list timer;
    unsigned short sendSeq;
    unsigned short resv;
    unsigned short timeVal;
    unsigned short dataLen;
    unsigned int sendCnt;
    unsigned int cycleCnt;
    unsigned int loopFlag;
    
    struct semaphore taskSem;
    unsigned int recvSeq[MAX_RECV_SEQ_CNT];
}TestCtrl;

static StatInfo g_stat;

static TestCtrl g_ctrl;

static unsigned int g_dbgFlag = 1;

static bool g_isRunning = false;

static LTEV_TEST_DataReq g_pktHead = {0x1, {0x11, 0x22, 0x33}, {0x44, 0x55, 0x66}, 1, 1 ,0, 0, 0, 0, 0};
static unsigned char g_testData[255];
static bool g_isTestInited = false;


void LTEV_DebugSw(unsigned int sw)
{
    g_dbgFlag = sw;
}

void LTEV_SetHead(unsigned short version, unsigned int srcAddr, unsigned int dstAddr, unsigned char priority,
    unsigned char pduType)
{
    g_pktHead.version = version;
    g_pktHead.srcAddr[0] = (unsigned char)(srcAddr >> 16);
    g_pktHead.srcAddr[1] = (unsigned char)(srcAddr >> 8);
    g_pktHead.srcAddr[2] = (unsigned char)srcAddr;
    g_pktHead.dstAddr[0] = (unsigned char)(dstAddr >> 16);
    g_pktHead.dstAddr[1] = (unsigned char)(dstAddr >> 8);
    g_pktHead.dstAddr[2] = (unsigned char)dstAddr;
    g_pktHead.priority = priority;
    g_pktHead.pduType = pduType;
}

void LTEV_SetTestData(char *data)
{
    unsigned int loop;
    
    for (loop = 0; loop < MIN(sizeof(g_testData), strnlen(data, sizeof(g_testData))); loop++) {
        g_testData[loop] = data[loop];
    }
    g_isTestInited = true;
}

static void LTEV_InitTestData(void)
{
    unsigned int loop;

    for (loop = 0; loop < sizeof(g_testData); loop++) {
        g_testData[loop] = loop + 1;
    }
    g_isTestInited = true;
}

static int LTEV_TxOnePkt(unsigned short dataLen)
{
    int  ret;
    unsigned int copyLen = 0;
    unsigned int contentLen = dataLen - sizeof(LTEV_TEST_DataReq);
    LTEV_TEST_DataReq *pkt = NULL;
    
    
    struct sk_buff *mem = dev_alloc_skb(dataLen);
    if (mem == NULL) {
        PRINT_ERR("LTEV_TxOnePkt: alloc skb mem fail\n");
        return ERR;
    }

    skb_put(mem, dataLen);
    pkt = (LTEV_TEST_DataReq*)(mem->data);

    *pkt = g_pktHead;
    pkt->seq = g_ctrl.sendSeq++;
    pkt->magicNum = LTEV_DATA_MAGIC_NUM;
    while (copyLen < contentLen) {
       ret = memcpy_s(&pkt->data[copyLen], contentLen - copyLen, g_testData, 
            MIN(sizeof(g_testData), contentLen - copyLen));
        if (ret != EOK) {
            PRINT_ERR("TTF_PRINT_ERR: memcpy_s fail,%d\n", ret);
        }
        copyLen += MIN(sizeof(g_testData), contentLen - copyLen);
    }

    if ((g_dbgFlag&LTEV_DBG_SEQ) == LTEV_DBG_SEQ) {
        PRINT_ERR("LTEV_TxOnePkt, seq: %u., len, %d\n", pkt->seq, dataLen);
    }
    
    g_stat.sendCntPer++;
    g_stat.sendBytesPer += dataLen;
    g_stat.totalSendCnt++;
    g_stat.totalSendBytes += dataLen;
    ret = mdrv_pc5_tx(mem);
    if (ret != 0) {
        PRINT_ERR("LTEV_TxOnePkt:mdrv_pc5_tx fail: %d\n", ret);
        return ERR;
    }

    return OK;
}

static int LTEV_LoopPkt(LTEV_TEST_DataInd *dlPkt, unsigned int dataLen)
{
    LTEV_TEST_DataReq* pkt = NULL;
    unsigned int contentLen = sizeof(LTEV_TEST_DataReq) + dataLen;
    int       ret;
    
    struct sk_buff *mem = dev_alloc_skb(dataLen);
    if (mem == NULL) {
        PRINT_ERR("LTEV_LoopPkt: alloc mem fail.\n");
        return ERR;
    }

    skb_put(mem, dataLen);
    pkt = (LTEV_TEST_DataReq*)(mem->data);

    pkt->version = g_pktHead.version;
    (void)memcpy_s(pkt->dstAddr, sizeof(pkt->dstAddr), dlPkt->dstAddr, sizeof(dlPkt->dstAddr));
    (void)memcpy_s(pkt->srcAddr, sizeof(pkt->srcAddr), dlPkt->srcAddr, sizeof(dlPkt->srcAddr));
    pkt->priority = dlPkt->priority;
    pkt->pduType = g_pktHead.pduType;
    pkt->seq = dlPkt->seq;
    pkt->magicNum = LTEV_DATA_MAGIC_NUM;
    (void)memcpy_s(pkt->data, contentLen, dlPkt->data, contentLen);

    if ((g_dbgFlag&LTEV_DBG_SEQ) == LTEV_DBG_SEQ) { 
        PRINT_ERR("LTEV_LoopPkt, seq: %u, len: %d.\n", pkt->seq, dataLen);
    }

    g_stat.sendCntPer++;
    g_stat.sendBytesPer += dataLen;
    g_stat.totalSendCnt++;
    g_stat.totalSendBytes += dataLen;
    ret= mdrv_pc5_tx(mem);
    if (ret != 0) {
        PRINT_ERR("LTEV_LoopPkt:mdrv_pc5_tx fail: %d\n", ret);
        return ERR;
    }

    return OK;
}

#define LTEV_TEST_INVALID_SEQ 0xFFFFFFFF
#define LTEV_TEST_SRC_IDX 2
#define LTEV_TEST_MAX_SEQ 0xFFFF
static int LTEV_RxPkt(struct sk_buff* skb)
{
    unsigned int dataLen;
    LTEV_TEST_DataInd* pkt = NULL;
    unsigned int lostCnt;

    if (skb->len <= sizeof(LTEV_TEST_DataInd)) {
        PRINT_ERR("LTEV_RxPkt, pktLen err, %d\n", skb->len);
        return ERR;
    }

    pkt = (LTEV_TEST_DataInd*)(skb->data);
    dataLen = skb->len;
    g_stat.rcvCntPer++;
    g_stat.rcvBytesPer += dataLen;
    g_stat.totalRcvCnt++;
    g_stat.totalRcvBytes += dataLen;
    if (g_ctrl.recvSeq[pkt->srcAddr[LTEV_TEST_SRC_IDX]] == LTEV_TEST_INVALID_SEQ) {
        g_ctrl.recvSeq[pkt->srcAddr[LTEV_TEST_SRC_IDX]] = pkt->seq;
    }
    lostCnt = pkt->seq - g_ctrl.recvSeq[pkt->srcAddr[LTEV_TEST_SRC_IDX]];
    g_ctrl.recvSeq[pkt->srcAddr[LTEV_TEST_SRC_IDX]] = (pkt->seq + 1) & LTEV_TEST_MAX_SEQ;
    if (lostCnt > 0) {
        g_stat.lostCntPer += lostCnt;
        g_stat.totalLostCnt += lostCnt;
    }
    
    if ((g_dbgFlag&LTEV_DBG_SEQ) == LTEV_DBG_SEQ) { 
        PRINT_ERR("LTEV_RxPkt, seq: %u, len: %d.\n", pkt->seq, dataLen);
    }
    
    if (g_ctrl.loopFlag != 0) {
       LTEV_LoopPkt(pkt, dataLen);
    }

    return OK;
}

static bool g_ltevTestAlive = false;

static void LTEV_Stat(unsigned long unused)
{    
    char *unitList[] = {"bps", "kbps", "mbps"};
    int unitBase[] = {1, 1000, 1000000};
    unsigned long sendRat, RcvRat, sendCntPer, rcvCntPer, lostCntPer, rcvBytesPer, sendBytesPer, seqErrCntPer;
    unsigned int sendIdx, rcvIdx;
    
    if (g_ltevTestAlive == true) {
        mod_timer(&g_ctrl.statTimer, jiffies + HZ);
    }

    sendCntPer = g_stat.sendCntPer;
    g_stat.sendCntPer = 0;
    rcvCntPer = g_stat.rcvCntPer;
    g_stat.rcvCntPer = 0;
    lostCntPer = g_stat.lostCntPer;
    g_stat.lostCntPer = 0;
    rcvBytesPer = g_stat.rcvBytesPer;
    g_stat.rcvBytesPer = 0;
    sendBytesPer = g_stat.sendBytesPer;
    g_stat.sendBytesPer = 0;
    seqErrCntPer = g_stat.seqErrCntPer;
    g_stat.seqErrCntPer = 0;
    g_stat.statTime++;
    
    if ((g_dbgFlag&LTEV_DBG_STAT) == LTEV_DBG_STAT) {
        if ((sendCntPer > 0) || (rcvCntPer > 0) || (lostCntPer > 0)) {
            rcvIdx = (rcvBytesPer > 1250) + (rcvBytesPer > 1250000);
            RcvRat = rcvBytesPer*8/unitBase[rcvIdx];
            sendIdx = (sendBytesPer > 1250) + (sendBytesPer > 1250000);
            sendRat = sendBytesPer*8/unitBase[sendIdx];
            PRINT_ERR("Send: %u(%u%s), Rcv: %u(%u%s), Lost: %u, Err: %u\n", sendCntPer, sendRat, unitList[sendIdx], 
                rcvCntPer, RcvRat, unitList[rcvIdx], lostCntPer, seqErrCntPer);
        }
    }
}

static void LTEV_TimerProc(void* agrv)
{
    if (g_isRunning == true) {
        bsp_softtimer_add(&g_ctrl.timer);
    }
    up(&g_ctrl.taskSem);
}

#define MAX_PKT_LEN 8188
#define DEFAULT_PKT_LEN 1200

static int LTEV_TestTask(void *data)
{
    unsigned int cycleCnt;
    unsigned int sendCnt;
    static bool isTimerCreate = false;
    
    PRINT_ERR("LTEV Test Start, dataCnt:%d, dataLen:%d, timeVal:%d\n", g_ctrl.sendCnt, g_ctrl.dataLen, g_ctrl.timeVal);
    
    sema_init(&g_ctrl.taskSem, 0);

    if ((g_ctrl.dataLen == 0) && (g_ctrl.sendCnt == 0)) {
        g_ctrl.loopFlag = g_ctrl.timeVal;
        down(&g_ctrl.taskSem);
    } else {
        if ((g_ctrl.dataLen < sizeof(LTEV_TEST_DataReq) || (g_ctrl.dataLen) > MAX_PKT_LEN)) {
            g_ctrl.dataLen = DEFAULT_PKT_LEN;
        }
    }

    if (g_ctrl.timeVal != 0) {
        if (isTimerCreate == false) {
            g_ctrl.timer.func = LTEV_TimerProc;
            g_ctrl.timer.para = NULL;
            g_ctrl.timer.timeout = g_ctrl.timeVal;
            g_ctrl.timer.wake_type = SOFTTIMER_WAKE;
            g_ctrl.timer.unit_type = TYPE_MS;
            g_ctrl.timer.name = "ltevtest";
            bsp_softtimer_create(&g_ctrl.timer);
            isTimerCreate = true;
        }
        bsp_softtimer_modify(&g_ctrl.timer, g_ctrl.timeVal);
        bsp_softtimer_add(&g_ctrl.timer);
    }

    cycleCnt = g_ctrl.cycleCnt;
    while ((g_isRunning == true) && ((g_ctrl.cycleCnt == 0) || (cycleCnt-- > 0))) {
        for (sendCnt = 0; sendCnt < g_ctrl.sendCnt; sendCnt++) {
            LTEV_TxOnePkt(g_ctrl.dataLen);
        }
        if (g_ctrl.timeVal != 0) {
            down(&g_ctrl.taskSem);
        }
    }
    g_isRunning = false;
    if (g_ctrl.timeVal != 0) {
        bsp_softtimer_delete(&g_ctrl.timer);
    }
    return 0;
}

void LTEV_StopTest(void)
{
    if (g_isRunning != true) {
        return;
    }
    g_isRunning = false;
    up(&g_ctrl.taskSem);
}


short g_ltevTestPort = 65432;
struct socket *g_ltevSock = NULL;
struct sockaddr_in g_ltevAddr = {.sin_family = AF_INET, .sin_port = 0x98ff, .sin_addr.s_addr=0x6401a8c0};
void LTEV_SetTestPort(short port)
{
    g_ltevTestPort = port;
}
void LTEV_SetAddr(char *addr, short port)
{
    g_ltevAddr.sin_family = AF_INET;
    g_ltevAddr.sin_port = htons(port);
    g_ltevAddr.sin_addr.s_addr = in_aton(addr);
}

void LTEV_SetAddrNew(unsigned addr, short port)
{
    g_ltevAddr.sin_family = AF_INET;
    g_ltevAddr.sin_port = port;
    g_ltevAddr.sin_addr.s_addr = addr;
}

int LTEV_TestServer(void *data)
{
    struct kvec iov;
    struct msghdr msg = {0};
    static char buf[MAX_PKT_LEN];
    int dataLen;
    struct sk_buff *mem = NULL;

    while (g_ltevTestAlive) {
        iov.iov_len = MAX_PKT_LEN;
        iov.iov_base = buf;
        if (g_ltevSock == NULL) {
            return 0;
        }
        dataLen = kernel_recvmsg(g_ltevSock, &msg, &iov, 1, MAX_PKT_LEN, 0);
        if (dataLen == -1) {
            PRINT_ERR("ltev recv msg fail\n");
            g_ltevSock = NULL;
            return 0;
        }
        mem = dev_alloc_skb(dataLen);
        if (mem == NULL) {
            PRINT_ERR("ltev alloc mem fail\n");
            continue;
        }
        skb_put(mem, dataLen);
        *(unsigned short *)&buf[10] = VOS_GetSlice() & 0xFFFF;
        (void)memcpy_s(mem->data, dataLen, buf, dataLen);
        if (mdrv_pc5_tx(mem) != 0) {
            PRINT_ERR("tx pc5 pkt fail\n");
        }
    }
    PRINT_ERR("ltev test server exit\n");
    return 0;
}

static struct sk_buff_head g_ltevDlQueue;
static struct semaphore g_ltevDlDataSem;

int LTEV_TsetRcvPc5PktProc(struct sk_buff* skb)
{
    if (skb == NULL) {
        return 0;
    }
    if (g_ltevTestAlive == false) {
        IMM_ZcFree(skb);
        return 0;
    }
    skb_queue_tail(&g_ltevDlQueue, skb);
    up(&g_ltevDlDataSem);
    return 0;
}

int LTEV_TestDlTask(void *data)
{
    struct msghdr msg = {0};
    struct kvec iov;
    int i, cnt;
    struct sk_buff *skb = NULL;

    while (g_ltevTestAlive) {
        down(&g_ltevDlDataSem);
        cnt = skb_queue_len(&g_ltevDlQueue);
        for (i = 0; i < cnt; i++) {
            skb = skb_dequeue(&g_ltevDlQueue);
            if (skb == NULL) {
                continue;
            }
            (void)LTEV_RxPkt(skb);
            iov.iov_base = skb->data;
            iov.iov_len = skb->len;
            msg.msg_name = &g_ltevAddr;
            msg.msg_namelen = sizeof(g_ltevAddr);
            if (g_ltevSock == NULL) {
                return 0;
            }
            if (kernel_sendmsg(g_ltevSock, &msg, &iov, 1, skb->len) == -1) {
                PRINT_ERR("send pc5 pkt fail\n");
            }
            IMM_ZcFree(skb);
        }
        
    }
    return 0;
}
void LTEV_TestInit(void)
{
    struct socket *sock = 0;
    struct sockaddr_in addr = {0};
    
    if (g_ltevTestAlive == true) {
        PRINT_ERR("ltev test already init\n");
        return;
    }

    skb_queue_head_init(&g_ltevDlQueue);
    sema_init(&g_ltevDlDataSem, 0);
    g_ltevTestAlive = true;
    memset_s(&g_stat, sizeof(g_stat), 0, sizeof(g_stat));
    setup_timer(&g_ctrl.statTimer, LTEV_Stat, 0);
    g_ctrl.statTimer.expires = jiffies + HZ;
    add_timer(&g_ctrl.statTimer);

    if (sock_create_kern(&init_net, AF_INET, SOCK_DGRAM, IPPROTO_UDP, &sock) < 0) {
        PRINT_ERR("create sock fail\n");
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(g_ltevTestPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (kernel_bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        PRINT_ERR("bind sock fail\n");
        sock_release(sock);
        return;
    }
    g_ltevSock = sock;

    if (mdrv_pc5_cb_register(LTEV_TsetRcvPc5PktProc) != 0) {
        PRINT_ERR("reg rcv func fail\n");
    }

    (void)kthread_run(LTEV_TestServer, NULL, "LtevTestServer");
    (void)kthread_run(LTEV_TestDlTask, NULL, "LtevTestDlTask");
}

void LTEV_TestDeInit(void)
{
    if (g_ltevTestAlive == true) {
        g_ltevTestAlive = false;
        del_timer(&g_ctrl.statTimer);
        up(&g_ltevDlDataSem);
        sock_release(g_ltevSock);
        g_ltevSock = NULL;
    }
}

void LTEV_TestClearStat(void)
{
    (void)memset_s(&g_stat, sizeof(g_stat), 0, sizeof(g_stat));
    (void)memset_s(g_ctrl.recvSeq, sizeof(g_ctrl.recvSeq), 0xFF, sizeof(g_ctrl));
}

void LTEV_TestShowStat(void)
{
    char *unitList[] = {"bps", "kbps", "mbps"};
    int unitBase[] = {1, 1000, 1000000};
    int sendIdx, rcvIdx;
    unsigned long sendRat, RcvRat, statTime;
    
    statTime = (g_stat.statTime == 0) ? 1 : g_stat.statTime;
    rcvIdx = (g_stat.totalRcvBytes/statTime > 1250) + (g_stat.totalRcvBytes/statTime > 1250000);
    RcvRat = g_stat.totalRcvBytes*8/unitBase[rcvIdx]/statTime;
    sendIdx = (g_stat.totalSendBytes/statTime > 1250) + (g_stat.totalSendBytes/statTime > 1250000);
    sendRat = g_stat.totalSendBytes*8/unitBase[sendIdx]/statTime;
    PRINT_ERR("LTEV Test Stat, Send:%u(%u%s), Rcv:%u(%u%s), Lost:%u, Err:%u\n", g_stat.totalSendCnt, sendRat, 
        unitList[sendIdx], g_stat.totalRcvCnt, RcvRat, unitList[rcvIdx], g_stat.totalLostCnt, g_stat.seqErrCnt);
}

int LTEV_StartTest(unsigned int dataCnt, unsigned short dataLen, unsigned short timeVal, unsigned int cycleCnt)
{
    if (g_isRunning == true) {
        PRINT_ERR("LTEV_Test: is running");
        return ERR;
    }

    if (g_isTestInited == false) {
        LTEV_InitTestData();
    }

    if (g_ltevTestAlive == false) {
        LTEV_TestInit();
    }
    
    g_isRunning = true;

    g_ctrl.timeVal = timeVal;
    g_ctrl.sendCnt = dataCnt;
    g_ctrl.dataLen = dataLen;
    g_ctrl.cycleCnt = cycleCnt;

    (void)kthread_run(LTEV_TestTask, NULL, "ltevTest");

    return OK;
}

unsigned short LTEV_GetVersion(void)
{
    PRINT_ERR("LTEV_GetVersion, version =  %d\n", VPS_TEST_VERSION);
    return VPS_TEST_VERSION;
}

