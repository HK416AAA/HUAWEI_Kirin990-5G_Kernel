/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#ifndef __NV_MSG_H__
#define __NV_MSG_H__

#include <osl_types.h>
#include <osl_list.h>
#include <osl_sem.h>
#include <osl_spinlock.h>
#include <mdrv_eicc.h>

#define NV_MSG_MAGICNUM (0x766e564e)
#define NV_MSG_LEN 0x100
#define NV_MSG_TIMEOUT (5000) /* max timeout 50s */

#define NV_MSG_SEND_COUNT 5

#define NV_CHANNAL_FULL EICC_ERR_EAGAIN

/* icc msg type */
enum nv_msg_type {
    NV_ICC_REQ_READ_BLK = 0x39,
    NV_ICC_REQ_FLUSH = 0x40,
    NV_ICC_REQ_FLUSH_RWNV,
    NV_ICC_REQ_LOAD_BACKUP,
    NV_ICC_REQ_UPDATE_DEFAULT,
    NV_ICC_REQ_UPDATE_BACKUP,
    NV_ICC_REQ_FACTORY_RESET,
    NV_ICC_REQ_FLUSH_RDWR_ASYNC,
    NV_ICC_REQ_DATA_WRITEBACK,
    NV_ICC_CNF = 127,
    NV_ICC_BUTT = 0xFF,
};


struct nv_send_msg {
    u32 msg_type;
    u32 msg;
    u32 data_len;
    u8 *data;
    u32 rcvmsg_len;
};

struct nv_msg_data {
    u32 magicnum;
    u32 slice;
    u32 msg_type;
    u32 msg;
    u32 sn;
    u32 ret;
    u32 rcvlen;
    u32 data_len;
    u8 data[0];
};

struct nv_msg_element {
    struct list_head list;
    struct nv_msg_data *msg_data;
};

struct msg_queue {
    struct list_head list;
    spinlock_t lock;
};

#define NV_EICC_BUF_ALIGN 32
#define NV_EICC_BUF_LEN (64 * 1024 - 128)
#define NV_EICC_MAX_LEN (NV_EICC_BUF_LEN - 128)

struct nv_eicc_chan {
    eicc_chn_t chn;
    eicc_chn_attr_t attr;
};

struct nvm_msg_info {
    u32 msg_sn;
    u32 cb_count;
    u32 cb_reply;
    u32 timeoutcnt;
    struct msg_queue msgqueue;
    struct msg_queue lwmsgqueue; /* low priority msg queue */
    struct nv_eicc_chan recvchn;
    struct nv_eicc_chan sendchn;
};

struct nvm_msg_info *nv_get_msginfo(void);

u32 nv_free_msgelement(struct nv_msg_element *msg);

struct nv_msg_element *nv_get_msg_element(void);

u32 nv_awake_msgtask(struct nv_msg_data *msg_data);

u32 nv_cpmsg_send(u8 *pdata, u32 len);

void nv_cpmsg_cb(struct nv_msg_data *msg);

u32 nv_cpmsg_chan_init(struct nvm_msg_info *msg_info);

u32 nv_msg_init(void);
s32 nv_init_dev(void);

#endif
