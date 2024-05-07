/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#include <securec.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/dma-mapping.h>
#include <osl_malloc.h>
#include <mdrv_eicc.h>
#include <bsp_eicc.h>
#include <bsp_slice.h>
#include <bsp_nvim.h>
#include "nv_comm.h"
#include "nv_msg.h"
#include "nv_debug.h"

static struct platform_device g_modem_nv_device = {
    .name = "modem_nv",
    .id = 0,
    .dev = {
        .init_name = "modem_nv",
    },
};
void nv_dma_init(void)
{
    struct device *pdev = &g_modem_nv_device.dev;
    of_dma_configure(pdev, NULL);
    dma_set_mask_and_coherent(pdev, ~(0x1FULL));//32 bytes align

    return;
}
/* 仅用于初始化nv设备 */
s32 nv_init_dev(void)
{
    u32 ret;
    ret = (u32)platform_device_register(&g_modem_nv_device);
    if (ret) {
        nv_printf("platform_device_register modem_nv_device fail !\n");
        return -1;
    }
    nv_dma_init();
    nv_printf("init modem nv dev ok\n");
    return NV_OK;
}
void *nv_dma_alloc(u32 size, dma_addr_t *dma_handle)
{
    void *mem_addr = NULL;
    struct device *pdev = &g_modem_nv_device.dev;

    mem_addr = dma_alloc_coherent(pdev, size, dma_handle, GFP_KERNEL);
    if (IS_ERR_OR_NULL(mem_addr)) {
        nv_printf("nv alloc failed\n");
        return NULL;
    }
    return mem_addr;
}

void nv_dma_free(size_t size, void *cpu_addr, dma_addr_t dma_handle)
{
    struct device *pdev = &g_modem_nv_device.dev;
    dma_free_coherent(pdev, size, cpu_addr, dma_handle);
    return;
}

#define NV_CPMSG_TSK_DELAY 5
u32 nv_cpmsg_send(u8 *pdata, u32 len)
{
    int ret = 0;
    u32 send_count = 0;
    struct nvm_msg_info *msg_info = nv_get_msginfo();
    struct nv_eicc_chan *chn = &msg_info->sendchn;

    for (send_count = 0; send_count < NV_MSG_SEND_COUNT; send_count++) {
        ret = bsp_eicc_chn_send(chn->chn, pdata, len, 0);
        nv_dbg_printf("send count:0x%x, ret:0x%x, len:0x%x\n", send_count, ret, len);
        if (ret == NV_CHANNAL_FULL) {
            osl_task_delay(NV_CPMSG_TSK_DELAY);
            continue;
        } else if (ret != len) {
            nv_printf("nv send msg fail, ret:%d\n", ret);
            return (u32)ret;
        } else {
            nv_debug_record(NV_DEBUG_SEND_ICC | (((struct nv_msg_data *)pdata)->msg_type << 0x10));
            return NV_OK;
        }
    }

    nv_printf("nv send eicc msg fail\n");
    return NV_ERROR;
}

void nv_cpmsg_cb(struct nv_msg_data *msg)
{
    u32 ret;
    struct nv_msg_data msg_data = { 0 };
    struct nvm_info *nvminfo = nv_get_nvminfo();

    msg_data.magicnum = NV_MSG_MAGICNUM;
    msg_data.slice = bsp_get_slice_value();
    msg_data.msg_type = NV_ICC_CNF;
    msg_data.msg = 0;
    msg_data.sn = msg->sn;
    msg_data.ret = msg->ret;
    msg_data.rcvlen = 0;
    msg_data.data_len = 0;

    ret = nv_cpmsg_send((u8 *)&msg_data, sizeof(struct nv_msg_data));
    if (ret) {
        nv_printf("send cp msg fail\n");
    }

    __pm_relax(&nvminfo->wakelock);
    nvminfo->msg_info.cb_reply++;
}

static u32 nv_handle_recv_msg(struct nv_msg_data *msg_data)
{
    u32 ret = 0;
    struct nvm_info *nvminfo = nv_get_nvminfo();

    nvminfo->msg_info.cb_count++;

    switch (msg_data->msg_type) {
        case NV_ICC_REQ_FLUSH:
        case NV_ICC_REQ_FLUSH_RWNV:
        case NV_ICC_REQ_LOAD_BACKUP:
        case NV_ICC_REQ_UPDATE_DEFAULT:
        case NV_ICC_REQ_UPDATE_BACKUP:
        case NV_ICC_REQ_FACTORY_RESET:
        case NV_ICC_REQ_DATA_WRITEBACK:
        case NV_ICC_REQ_READ_BLK:
            __pm_stay_awake(&nvminfo->wakelock);
            ret = nv_awake_msgtask(msg_data);
            break;
        case NV_ICC_REQ_FLUSH_RDWR_ASYNC:
            ret = nv_awake_msgtask(msg_data);
            break;
        default:
            nv_printf("invalid nv msg type %d\n", msg_data->msg_type);
            ret = BSP_ERR_NV_ICC_CHAN_ERR;
    }

    nv_debug_record(NV_DEBUG_RECEIVE_ICC | (msg_data->msg_type << 0x10));

    return ret;
}

/*
 * read eicc msg need in queue, otherwise will read bad msg
 */
static int nv_cpmsg_handle(eicc_event event, void *arg, const eicc_eventinfo *event_info)
{
    int ret;
    struct nvm_msg_info *msg_info = nv_get_msginfo();
    struct nv_eicc_chan *chn = &msg_info->recvchn;
    struct nv_msg_data msg_data = { 0 };

    if ((event != EICC_EVENT_DATA_ARRV) || (event_info == NULL)) {
        nv_printf("nv eicc msg receive fail\n");
        return -1;
    }

    if (event_info->data_arrv.chn_hdl != chn->chn) {
        nv_printf("nv eicc chan not match\n");
        return -1;
    }

    ret = bsp_eicc_chn_recv(chn->chn, &msg_data, sizeof(struct nv_msg_data), 0);
    if (ret != sizeof(struct nv_msg_data)) {
        nv_printf("nv read eicc msg fail, ret:%d\n", ret);
        return ret;
    }

    if (msg_data.magicnum != NV_MSG_MAGICNUM) {
        nv_printf("invlid msg data magicnum:0x%x\n", msg_data.magicnum);
        return -1;
    }

    nv_dbg_printf("sn:0x%x, msg_type:0x%x recv\n", msg_data.sn, msg_data.msg_type);
    nv_debug_record(NV_DEBUG_READ_ICC | ((msg_data.sn) << 0x10));

    ret = (int)nv_handle_recv_msg(&msg_data);
    if (ret) {
        nv_printf("handle apmsg failed\n");
    }

    return ret;
}

/*
 * nv_eicc_chan_init bufsize must over 32 byte
 */
static int nv_eicc_chan_init(struct nv_eicc_chan *chn, eicc_cbk cbk, unsigned chnid, unsigned type, unsigned bufsize)
{
    int ret;
    void *addr = NULL;
    dma_addr_t dma_handle;

    ret = bsp_eicc_chn_attr_init(&chn->attr);
    if (ret) {
        nv_printf("nv eicc chan init fail, ret:%d\n", ret);
        return (u32)ret;
    }

    addr = nv_dma_alloc(bufsize, &dma_handle);
    if (addr == NULL) {
        nv_printf("nv malloc chan buf fail\b");
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    chn->attr.chnid = chnid;
    chn->attr.type = type;
    chn->attr.pa = dma_handle;
    chn->attr.va = addr;
    chn->attr.size = bufsize;
    chn->attr.cbk = cbk;
    chn->attr.cbk_arg = NULL;

    ret = bsp_eicc_chn_open(&chn->chn, &chn->attr);
    if (ret) {
        nv_dma_free(bufsize, chn->attr.va, chn->attr.pa);
        nv_printf("open nv eicc chan fail, ret:%d\n", ret);
        return ret;
    }

    return 0;
}

u32 nv_cpmsg_chan_init(struct nvm_msg_info *msg_info)
{
    u32 ret;
    struct nv_eicc_chan *chn = NULL;

    chn = &msg_info->recvchn;
    ret = (u32)nv_eicc_chan_init(chn, (eicc_cbk)nv_cpmsg_handle, EICC_CHN_RECV_TSP2AP_MDRVNV, EICC_CHN_TYPE_RECV,
        NV_EICC_BUF_LEN);
    if (ret) {
        nv_printf("nv recv eicc init fail\n");
        return ret;
    }

    chn = &msg_info->sendchn;
    ret = (u32)nv_eicc_chan_init(chn, NULL, EICC_CHN_SEND_AP2TSP_MDRVNV, EICC_CHN_TYPE_SEND, NV_EICC_BUF_LEN);
    if (ret) {
        nv_dma_free(NV_EICC_BUF_LEN, msg_info->recvchn.attr.va, msg_info->recvchn.attr.pa);
        nv_printf("nv send eicc init fail\n");
        return ret;
    }

    return 0;
}
