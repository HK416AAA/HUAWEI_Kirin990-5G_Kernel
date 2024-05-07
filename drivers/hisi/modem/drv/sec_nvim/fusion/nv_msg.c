/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#include <securec.h>
#include <mdrv_eicc.h>
#include <bsp_eicc.h>
#include <bsp_slice.h>
#include <bsp_nvim.h>
#include "nv_comm.h"
#include "nv_msg.h"

struct nvm_msg_info *nv_get_msginfo(void)
{
    struct nvm_info *nvminfo = nv_get_nvminfo();

    return &nvminfo->msg_info;
}

struct nv_msg_element *nv_alloc_msgelement(u32 data_len)
{
    struct nv_msg_element *msg = NULL;

    msg = nv_malloc(sizeof(struct nv_msg_element));
    if (msg == NULL) {
        nv_printf("msg malloc fail\n");
        return NULL;
    }

    msg->msg_data = nv_malloc(sizeof(struct nv_msg_data) + data_len);
    if (msg->msg_data == NULL) {
        nv_free(msg);
        nv_printf("msg data free fail\n");
        return NULL;
    }

    (void)memset_s(msg->msg_data->data, data_len, 0, data_len);

    msg->msg_data->magicnum = 0;
    msg->msg_data->slice = 0;
    msg->msg_data->msg_type = 0;
    msg->msg_data->msg = 0;
    msg->msg_data->sn = 0;
    msg->msg_data->ret = 0;
    msg->msg_data->rcvlen = 0;
    msg->msg_data->data_len = data_len;
    INIT_LIST_HEAD(&msg->list);

    return msg;
}

u32 nv_free_msgelement(struct nv_msg_element *msg)
{
    u32 ret = NV_OK;

    nv_free(msg->msg_data);
    nv_free(msg);
    return ret;
}

struct nv_msg_element *nv_get_msg_element(void)
{
    unsigned long flags;
    struct nv_msg_element *msg_element = NULL;
    struct msg_queue *msgqueue = NULL;
    struct nvm_msg_info *msg_info = nv_get_msginfo();

    msgqueue = &msg_info->msgqueue;

    spin_lock_irqsave(&msgqueue->lock, flags);
    if (!list_empty(&msgqueue->list)) {
        msg_element = list_first_entry(&msgqueue->list, struct nv_msg_element, list);
        list_del(&msg_element->list);
    }
    spin_unlock_irqrestore(&msgqueue->lock, flags);

    if (msg_element != NULL) {
        return msg_element;
    }

    msgqueue = &msg_info->lwmsgqueue;

    spin_lock_irqsave(&msgqueue->lock, flags);
    if (!list_empty(&msgqueue->list)) {
        msg_element = list_first_entry(&msgqueue->list, struct nv_msg_element, list);
        list_del(&msg_element->list);
    }
    spin_unlock_irqrestore(&msgqueue->lock, flags);

    return msg_element;
}

u32 nv_awake_msgtask(struct nv_msg_data *msg_data)
{
    u32 ret = 0;
    unsigned long flags;
    struct nv_msg_data *msg = NULL;
    struct nv_msg_element *msg_element = NULL;
    struct msg_queue *msgqueue = NULL;
    struct nvm_info *nvminfo = nv_get_nvminfo();
    struct nv_eicc_chan *chn = &nvminfo->msg_info.recvchn;

    switch (msg_data->msg_type) {
        case NV_ICC_REQ_FLUSH:
        case NV_ICC_REQ_FLUSH_RWNV:
        case NV_ICC_REQ_LOAD_BACKUP:
        case NV_ICC_REQ_UPDATE_DEFAULT:
        case NV_ICC_REQ_UPDATE_BACKUP:
        case NV_ICC_REQ_FACTORY_RESET:
        case NV_ICC_REQ_DATA_WRITEBACK:
        case NV_ICC_REQ_READ_BLK:
            msgqueue = &nvminfo->msg_info.msgqueue;
            break;

        case NV_ICC_REQ_FLUSH_RDWR_ASYNC:
            msgqueue = &nvminfo->msg_info.lwmsgqueue;
            break;
        default:
            nv_printf("invalid nv icc msg type %d \n", msg_data->msg_type);
            return BSP_ERR_NV_ICC_CHAN_ERR;
    }

    msg_element = nv_alloc_msgelement(msg_data->data_len);
    if (msg_element == NULL) {
        nv_printf("msg element malloc fail\n");
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    msg = msg_element->msg_data;
    msg->magicnum = msg_data->magicnum;
    msg->slice = bsp_get_slice_value();
    msg->msg_type = msg_data->msg_type;
    msg->msg = msg_data->msg;
    msg->sn = msg_data->sn;
    if (msg_data->data_len != 0) {
        ret = bsp_eicc_chn_recv(chn->chn, msg->data, msg->data_len, 0);
        if (ret != msg->data_len) {
            (void)nv_free_msgelement(msg_element);
            nv_printf("nv read eicc msg fail, ret:%d\n", ret);
            return ret;
        }
    }

    nv_dbg_printf("add sn:0x%x, msg type:0x%x, private data len:0x%x to list\n", msg->sn, msg->msg_type, msg->data_len);
    spin_lock_irqsave(&msgqueue->lock, flags);
    list_add_tail(&msg_element->list, &msgqueue->list);
    spin_unlock_irqrestore(&msgqueue->lock, flags);

    nv_dbg_printf("wake up msg task\n");
    osl_sem_up(&nvminfo->task_sem);

    return NV_OK;
}

u32 nv_msg_init(void)
{
    u32 ret;
    struct nvm_msg_info *msg_info = nv_get_msginfo();

    spin_lock_init(&msg_info->msgqueue.lock);
    INIT_LIST_HEAD(&msg_info->msgqueue.list);

    spin_lock_init(&msg_info->lwmsgqueue.lock);
    INIT_LIST_HEAD(&msg_info->lwmsgqueue.list);

    ret = nv_cpmsg_chan_init(msg_info);
    if (ret != NV_OK) {
        nv_printf("nv cp msg chan init fail\n");
        return ret;
    }

    nv_printf("nv msg init success\n");
    return NV_OK;
}
