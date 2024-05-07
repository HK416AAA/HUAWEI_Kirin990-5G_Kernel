/*
 * u_ppp_ndis_ctrl.h -- interface to USB gadget "ethernet link" utilities
 *
 * Copyright (C) 2003-2005,2008 David Brownell
 * Copyright (C) 2003-2004 Robert Schwebel, Benedikt Spranger
 * Copyright (C) 2008 Nokia Corporation
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2016. Dongyue Chen  <foss@huawei.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __U_PPP_NDIS_CTRL_H
#define __U_PPP_NDIS_CTRL_H

#define usb_err(fmt, ...) do { \
    bsp_err("<%s>" fmt, __func__, ##__VA_ARGS__); \
} while (0)

#define usb_info(fmt, ...) do { \
    bsp_info("<%s>" fmt, __func__, ##__VA_ARGS__); \
} while (0)

#define usb_dbg(fmt, ...) do { \
    if (maa->dbg & MAA_DEBUG_MSG) {                   \
        bsp_err("<%s>" fmt, __func__, ##__VA_ARGS__); \
    }                                                 \
} while (0)

#define usb_bug_on() do { \
    dump_stack();                                                \
    if (maa->dbg & MAA_DEBUG_SYSTEM_ERR) {                       \
        system_error(DRV_ERRNO_MAA_ADDR_CHECK_FAIL, 0, 0, 0, 0); \
    }                                                            \
    if (maa->dbg & MAA_DEBUG_BUGON) {                            \
        BUG_ON(1);                                               \
    }                                                            \
} while (0)

#define USB_IOCTL_USB_READY                0x5F01

/* ppp cmd */
#define USB_IOCTL_PPP_MSC_READ             0x5F02
#define USB_IOCTL_PPP_DATA                 0x5F04
#define USB_IOCTL_PPP_MSC_WRITE            0x3F000011

/* ndis cmd */
#define USB_IOCTL_SPEED_CHANGE             0x2F000011
#define USB_IOCTL_CONNECT_NOTIFY           0x2F000012
#define USB_IOCTL_SET_FLOW_CTRL            0x4F000011
#define USB_IOCTL_PDUINFO_CFG              0x4F000019
#define USB_IOCTL_STAT_CHG                 0x5F03
#define USB_IOCTL_NDIS_DATA                0x5F05
#define VCOM_IOCTL_PPP_OPEN                0x5F06

#define USB_MODEM_UL_DATA_BUFF_SIZE 1536
#define USB_MODEM_UL_DATA_BUFF_NUM 16
#define USB_SPE_TD_SIZE 1024
#define USB_SPE_RD_SIZE 1024

struct stat_ctx {
    unsigned int open;
    unsigned int close;

    unsigned int write_succ;
    unsigned int write_fail;

    unsigned int write_msc_succ;
    unsigned int write_msc_fail;
    unsigned int write_msc_done;
    unsigned int write_default;

    unsigned int read_succ;
    unsigned int read_fail;
    unsigned int read_free_skb;
    unsigned int read_alloc_maa_fail;

    unsigned int return_buff_succ;
    unsigned int return_buff_fail;
    unsigned int read_msc_succ;
    unsigned int read_msc_fail;
};

struct u_ppp_ndis_ctx {
    struct net_device *net;
    int bypass;
    unsigned int spe_port;
    int modem_handle;
    int stick_handle;
    unsigned int ndis_data_handle;

    struct sk_buff_head skb_head;
    spinlock_t tx_lock;
    struct workqueue_struct *workqueue;
    struct work_struct work;

    struct stat_ctx modem_stat;
    struct stat_ctx stick_stat;
    spinlock_t ctx_lock;
    unsigned int usb_enum;
    unsigned int ccore_spe_ok;
    unsigned int enum_nortify;
};

struct u_ppp_ndis_ctx *usb_ppp_ndis_get_ctx(void);

#ifdef CONFIG_USB_GADGET
void usb_ndis_ctrl_xmit(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb, unsigned int cmd);
int usb_ndis_ctrl_open(void);
int usb_ndis_ctrl_close(void);
int usb_ndis_support(void);

#else
void usb_ndis_ctrl_xmit(struct u_ppp_ndis_ctx *ctx, struct sk_buff *skb, unsigned int cmd)
{
    return;
}

int usb_ndis_ctrl_open(void)
{
    return 0;
}

int usb_ndis_ctrl_close(void)
{
    return 0;
}

int usb_ndis_support(void)
{
    return 0;
}
#endif

#endif /* __U_PPP_NDIS_CTRL_H */
