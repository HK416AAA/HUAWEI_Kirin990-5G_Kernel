/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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
#include "eicc_platform.h"
#include "eicc_device.h"
#include "eicc_driver.h"
#include "eicc_core.h"

#define EICC_TEST_PIPE_DEPTH_MAX (0x1000)
#define TEST_BUFFER_SIZE 256

struct eicc_test_add {
    void *push_ptr_va;
    eiccsoc_ptr_t push_ptr_pa;
};

struct eicc_test_add g_send_add;
struct eicc_test_add g_recv_add;

static int eicc_test_report(int expression)
{
    if (expression) {
        eicc_print_always("PASS!\n");
        return 0;
    } else {
        eicc_print_always("FAIL!\n");
        return -1;
    }
}

#define EICC_TEST_LOOP_TIMES_MAX 128
static u8 g_fusion_test_send_buf[EICC_TEST_PIPE_DEPTH_MAX];
static u8 g_fusion_test_recv_buf[EICC_TEST_PIPE_DEPTH_MAX];

int eicc_fusion_opensend_test(unsigned user_id)
{
    int ret;
    eicc_chn_t handle;
    eicc_chn_attr_t attr;
    bsp_eicc_chn_attr_init(&attr);
    attr.chnid = user_id;
    attr.type = EICC_CHN_TYPE_SEND;
    attr.size = EICC_TEST_PIPE_DEPTH_MAX;
    attr.va = eicc_dma_alloc(attr.size, &attr.pa, EICC_PIPE_ADDR_ALIGN);

    if (attr.va == NULL) {
        return eicc_test_report(attr.va != NULL);
    }

    attr.cbk = NULL;
    attr.cbk_arg = NULL;
    attr.flags = 0;

    ret = bsp_eicc_chn_open(&handle, &attr);
    if (ret) {
        eicc_print_always("bsp_eicc_chn_open failed!\n");
        eicc_dma_free(attr.size, attr.va, attr.pa, EICC_PIPE_ADDR_ALIGN);
    }
    return eicc_test_report(ret == 0);
}

int eicc_fusion_openrecv_test(unsigned user_id)
{
    int ret;
    eicc_chn_t handle;
    eicc_chn_attr_t attr;
    bsp_eicc_chn_attr_init(&attr);
    attr.chnid = user_id;
    attr.type = EICC_CHN_TYPE_RECV;
    attr.size = EICC_TEST_PIPE_DEPTH_MAX;
    attr.va = eicc_dma_alloc(attr.size, &attr.pa, EICC_PIPE_ADDR_ALIGN);
    if (attr.va == NULL) {
        return eicc_test_report(attr.va != NULL);
    }

    attr.cbk = NULL;
    attr.cbk_arg = NULL;
    attr.flags = 0;

    ret = bsp_eicc_chn_open(&handle, &attr);
    if (ret) {
        eicc_print_always("bsp_eicc_chn_open failed!\n");
        eicc_dma_free(attr.size, attr.va, attr.pa, EICC_PIPE_ADDR_ALIGN);
    }
    return eicc_test_report(ret == 0);
}

int eicc_fusion_send_test(unsigned user_id, unsigned len)
{
    int ret;
    eicc_chn_t handle;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    eicc_blkx2_desc_t send_desc;

    ret = eicc_dev_chn_get_byid(user_id, &pdev, &pchannel);
    if (ret) {
        eicc_print_always("user_id is invaild\n");
        return eicc_test_report(ret == 0);
    }
    handle = pchannel->ldrvchn_id;
    if (len > sizeof(g_fusion_test_send_buf)) {
        return eicc_test_report(len <= sizeof(g_fusion_test_send_buf));
    }
    send_desc.cnt = 1;
    send_desc.datablk[0].len = len;
    send_desc.datablk[0].buf = (void *)g_fusion_test_send_buf;
    ret = bsp_eicc_chn_blks_send(handle, (eicc_blk_desc_t *)&send_desc, 0);
    return eicc_test_report(len == ret || ret == 0);
}

int eicc_fusion_sendblk_test(unsigned user_id, unsigned len1, unsigned len2)
{
    int ret;
    eicc_chn_t handle;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    eicc_blkx2_desc_t send_desc;

    ret = eicc_dev_chn_get_byid(user_id, &pdev, &pchannel);
    if (ret) {
        eicc_print_always("user_id is invaild\n");
        return eicc_test_report(ret == 0);
    }
    handle = pchannel->ldrvchn_id;
    if (len1 + len2 > sizeof(g_fusion_test_send_buf)) {
        return eicc_test_report(len1 + len2 <= sizeof(g_fusion_test_send_buf));
    }
    send_desc.cnt = 2;
    send_desc.datablk[0].len = len1;
    send_desc.datablk[0].buf = g_fusion_test_send_buf;
    send_desc.datablk[1].len = len2;
    send_desc.datablk[1].buf = g_fusion_test_send_buf + len1;
    ret = bsp_eicc_chn_blks_send(handle, (eicc_blk_desc_t *)&send_desc, 0);
    return eicc_test_report(len1 + len2 == ret || ret == 0);
}

int eicc_fusion_sendloop_test(unsigned user_id, unsigned startlen, unsigned times)
{
    int ret = 0;
    int len = 0;
    unsigned idx;
    eicc_chn_t handle;

    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;
    eicc_blkx2_desc_t send_desc;

    ret = eicc_dev_chn_get_byid(user_id, &pdev, &pchannel);
    if (ret) {
        eicc_print_always("user_id is invaild\n");
        return eicc_test_report(ret == 0);
    }
    handle = pchannel->ldrvchn_id;
    if (times > EICC_TEST_LOOP_TIMES_MAX) {
        times = EICC_TEST_LOOP_TIMES_MAX;
    }
    send_desc.cnt = 1;
    send_desc.datablk[0].buf = (void *)g_fusion_test_send_buf;
    send_desc.datablk[0].len = startlen;
    for (idx = 0; idx < times; idx++) {
        if (send_desc.datablk[0].len > sizeof(g_fusion_test_send_buf)) {
            break;
        }
        len = bsp_eicc_chn_blks_send(handle, (eicc_blk_desc_t *)&send_desc, EICC_SEND_FLAGS_NOARRVNOTIFY);
        if (len == 0) {
            eicc_print_always("no space left to send\n");
            ret = 0;
            break;
        }
        if (len != send_desc.datablk[0].len) {
            ret = -1;
            break;
        }

        send_desc.datablk[0].len++;
    }
    return eicc_test_report(ret == 0);
}

int eicc_fusion_recv_test(unsigned user_id)
{
    int ret;
    eicc_chn_t handle;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    ret = eicc_dev_chn_get_byid(user_id, &pdev, &pchannel);
    if (ret) {
        eicc_print_always("user_id is invaild\n");
        return eicc_test_report(ret == 0);
    }
    handle = pchannel->ldrvchn_id;
    ret = bsp_eicc_chn_recv(handle, g_fusion_test_recv_buf, sizeof(g_fusion_test_recv_buf), 0);
    eicc_print_always("eicc_fusion_recv_test ret  = 0x%x\n", (unsigned)ret);
    return eicc_test_report(ret >= 0);
}

int eicc_fusion_recvpeek_test(unsigned user_id)
{
    int ret;
    eicc_chn_t handle;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    ret = eicc_dev_chn_get_byid(user_id, &pdev, &pchannel);
    if (ret) {
        eicc_print_always("user_id is invaild\n");
        return eicc_test_report(ret == 0);
    }
    handle = pchannel->ldrvchn_id;
    ret = bsp_eicc_chn_recv(handle, g_fusion_test_recv_buf, sizeof(g_fusion_test_recv_buf), EICC_RECV_FLAGS_DATAPEEK);
    return eicc_test_report(ret >= 0);
}

int eicc_fusion_recvloop_test(unsigned user_id, unsigned times)
{
    int ret = 0;
    unsigned idx;
    eicc_chn_t handle;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    ret = eicc_dev_chn_get_byid(user_id, &pdev, &pchannel);
    if (ret) {
        eicc_print_always("user_id is invaild\n");
        return eicc_test_report(ret == 0);
    }
    handle = pchannel->ldrvchn_id;
    for (idx = 0; idx < times; idx++) {
        ret = bsp_eicc_chn_recv(handle, g_fusion_test_recv_buf, sizeof(g_fusion_test_recv_buf), 0);
        if (ret <= 0) {
            break;
        }
    }
    return eicc_test_report(ret >= 0);
}

static eicc_chn_t g_snd_chn_hdl;
static eicc_chn_t g_rcv_chn_hdl;
static u8 g_eicc_example_send_buf[128];
static u8 g_eicc_example_recv_buf[128];

int eicc_example_cbk(eicc_event event, void *cbk_arg, const eicc_eventinfo *pevent_info)
{
    int ret;
    ioctl_nxtpkt pktinfo;
    const eicc_eventinfo *event_info = (const eicc_eventinfo *)pevent_info;
    if (event == EICC_EVENT_DATA_ARRV) {
        /* 来一个到达通知可能是多条消息 */
        /* 获取通道里的消息长度 */
        if (bsp_eicc_chn_ioctl(event_info->data_arrv.chn_hdl, EICC_IOCTL_CHN_NXTPKT_INF, &pktinfo, sizeof(pktinfo))) {
            eicc_print_error("Something went wrong\n");
            return -1;
        }
        while (pktinfo.len) {
            ret = bsp_eicc_chn_recv(event_info->data_arrv.chn_hdl, g_eicc_example_recv_buf,
                sizeof(g_eicc_example_recv_buf), 0);
            eicc_print_info("bsp_eicc_chn_recv ret 0x%x\n", ret);
            if (bsp_eicc_chn_ioctl(event_info->data_arrv.chn_hdl, EICC_IOCTL_CHN_NXTPKT_INF, &pktinfo,
                sizeof(pktinfo))) {
                eicc_print_error("Something went wrong\n");
                return -1;
            }
        }
    } else {
        eicc_print_error("Something went wrong\n");
    }
    return 0;
}
int eicc_example_open(u32 send_id, u32 recv_id)
{
    int ret;
    eicc_chn_attr_t attr;
    /* 打开通道的attr要先初始化 */
    bsp_eicc_chn_attr_init(&attr);

    /* 指定通道编号 */
    attr.chnid = send_id;
    /* 指定类型为发送 */
    attr.type = EICC_CHN_TYPE_SEND;
    /* size必须是128Bytes对齐的 */
    attr.size = 0x200;
    /* eicc用的dma内存是调用者给的，调用者给的内存需要至少对齐到8Bytes，最好对齐到128Bytes */
    attr.va = eicc_dma_alloc(attr.size, &attr.pa, 128);
    if (attr.va == NULL) {
        return eicc_test_report(attr.va != NULL);
    }

    /* 发送不需要回调 */
    attr.cbk = NULL;
    attr.cbk_arg = NULL;

    ret = bsp_eicc_chn_open(&g_snd_chn_hdl, &attr);
    if (ret) {
        eicc_dma_free(attr.size, attr.va, attr.pa, EICC_PIPE_ADDR_ALIGN);
        return eicc_test_report(ret == 0);
    }

    /* 重新初始化，准备打开接收通道 */
    bsp_eicc_chn_attr_init(&attr);
    attr.chnid = recv_id;
    attr.type = EICC_CHN_TYPE_RECV;
    attr.size = 0x200;
    attr.va = eicc_dma_alloc(attr.size, &attr.pa, 128);
    if (attr.va == NULL) {
        return eicc_test_report(attr.va != NULL);
    }

    attr.cbk = eicc_example_cbk;
    attr.cbk_arg = NULL; /* 这个就是eicc_example_cbk中的第二个参数 */
    ret = bsp_eicc_chn_open(&g_rcv_chn_hdl, &attr);
    if (ret) {
        eicc_dma_free(attr.size, attr.va, attr.pa, EICC_PIPE_ADDR_ALIGN);
        return eicc_test_report(ret == 0);
    }
    return eicc_test_report(ret == 0);
}

int eicc_example_send(unsigned times)
{
    unsigned i;
    for (i = 0; i < times && i < sizeof(g_eicc_example_send_buf); i++) {
        bsp_eicc_chn_send(g_snd_chn_hdl, g_eicc_example_send_buf, i + 1, 0);
    }
    return 0;
}
