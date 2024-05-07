/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <product_config.h>
#include <mdrv_udi.h>
#include <mdrv_usb.h>
#include <nv_stru_drv.h>
#ifdef FEATURE_NVA_ON
#include <nva_stru.h>
#include <nva_id.h>
#endif
#include <bsp_nva.h>
#include <bsp_nvim.h>
#include <bsp_cshell.h>
#include <bsp_print.h>
#include <securec.h>
#include <adrv.h>
#include "console.h"
#include "virtshell.h"
#include "cshell_logger.h"
/*lint --e{64}*/

int g_acm_chn_handle = 0;
struct semaphore g_recv_sem;
struct semaphore g_send_sem;

#undef THIS_MODU
#define THIS_MODU mod_cshell

#define TMP_BUFF_SIZE 128
#define cshell_port_info(fmt, ...) (bsp_info("%s " fmt, __FUNCTION__, ##__VA_ARGS__))
#define cshell_port_err(fmt, ...) (bsp_err("%s " fmt, __FUNCTION__, ##__VA_ARGS__))

void cshell_starttx(struct console *con);
struct console g_bluetooth_con = {
    .name = "cshell_con",
    .data = NULL,
    .start_tx = cshell_starttx,
    .start_shell = virtshl_start_shell,
    .coreid = CON_CCORE,
    .inflags = CON_INFLG_AGENT | CON_INFLG_ICC | CON_INFLG_CONSTANT,
    .icchn = NULL,
    .predecessor = NULL
}
; /*lint !e785*/
EXPORT_SYMBOL(g_bluetooth_con);

unsigned char g_bluetooth_logbuf[1U << LOG_BUF_SHIFT];
unsigned char g_bluetooth_cmdbuf[1U << CMD_BUF_SHIFT];

int legal_chr(char chr)
{
    if (chr >= 0x20 && chr <= 0x7E) /* visible character */
        return 1;
    if ('\r' == chr || '\n' == chr || 0x8 == chr || /* backspace */
        0x7F == chr ||                              /* del */
        0x1B == chr || /* esc(1b) for up(1b 5b 41)/down(1b 5b 42)/left(1b 5b 44)/right(1b 5b 43) */
        0x4 == chr ||  /* ctrl+d */
        '\t' == chr)
        return 1;

    return 0;
}

int recv_func(void *data)
{
    struct acm_wr_async_info acm_wt_info = {0};
    int i = 0;

    while (1) {
        down(&g_recv_sem);

        if (0 == g_acm_chn_handle)
            continue;

        if (mdrv_udi_ioctl(g_acm_chn_handle, ACM_IOCTL_GET_RD_BUFF, &acm_wt_info))
            continue;

        for (i = 0; i < acm_wt_info.size; i++) { /*lint !e737*/ /*lint !e574*/
            if (!legal_chr(acm_wt_info.virt_addr[i])) {
                cshell_port_err("[bluetooth port]input contains unvisible data\n");
                break;
            }
        }
        if (i < acm_wt_info.size) { /*lint !e737*/ /*lint !e574*/
            (void)mdrv_udi_ioctl(g_acm_chn_handle, ACM_IOCTL_RETURN_BUFF, &acm_wt_info);
            continue;
        }
        g_bluetooth_con.rx(&g_bluetooth_con, (unsigned char *)acm_wt_info.virt_addr, acm_wt_info.size);

        (void)mdrv_udi_ioctl(g_acm_chn_handle, ACM_IOCTL_RETURN_BUFF, &acm_wt_info);
    }

    return 0; /*lint !e527*/
} /*lint !e715*/

static unsigned char *g_tmpbuf = NULL;
static int tsk_send_func(void *data)
{
    int len = 0;
    int ret = 0;
    int print_cnt = 0;
    errno_t err = EOK;

    if (NULL == g_tmpbuf) {
        g_tmpbuf = kmalloc(TMP_BUFF_SIZE, GFP_KERNEL);
        if (NULL == g_tmpbuf) {
            cshell_port_err("tsk_send_func g_tmpbuf malloc fail\n");
            return -1;
        }
        err = memset_s(g_tmpbuf, TMP_BUFF_SIZE, 0, TMP_BUFF_SIZE);
        if (err != EOK) {
            cshell_port_err("tsk_send_func memset fail, err = %d\n", err);
        }
    }

    while (!kthread_should_stop()) {
        down(&g_send_sem);
        /*lint -e732*/
        while ((len = g_bluetooth_con.tx(&g_bluetooth_con, g_tmpbuf, TMP_BUFF_SIZE)) > 0) { /*lint !e747*/
            ret = mdrv_udi_write(g_acm_chn_handle, (void *)g_tmpbuf, len);
            if (g_acm_chn_handle && (0 > ret)) {
                if (print_cnt == 0) {
                    cshell_port_err("mdrv_udi_write fail in tsk_send_func, ret = %d\n", ret);
                    print_cnt++;
                }
            }
            a_cshell_write(len, g_tmpbuf);
        }
        print_cnt = 0;
    } /*lint +e732*/

    kfree(g_tmpbuf);
    g_tmpbuf = NULL;
    return 0; /*lint !e527*/
} /*lint !e715*/

void cshell_starttx(struct console *con)
{
    up(&g_send_sem);
} /*lint !e715*/
EXPORT_SYMBOL(cshell_starttx);

void cshell_acm_event_cb(void)
{
    up(&g_recv_sem);
}
EXPORT_SYMBOL(cshell_acm_event_cb);

void acm_open_cb(void)
{
    int acm_chn_handle;
    int retval;
    struct cshell_open_s flags;
    struct udi_open_param cshell_handle;
    cshell_port_info("acm_open_cb called\n");

    cshell_handle.devid = UDI_ACM_SHELL_ID;

    acm_chn_handle = mdrv_udi_open(&cshell_handle); /*lint !e838*/
    if (!acm_chn_handle) {
        cshell_port_err("A:cshell_udi_open_cb acm open fail: [0x%x]\n", acm_chn_handle);
        return;
    }

    retval = mdrv_udi_ioctl(acm_chn_handle, ACM_IOCTL_SET_READ_CB, cshell_acm_event_cb);
    if (retval) {
        cshell_port_err("A:cshell_udi_open_cb cb register fail: [0x%x]\n", retval);
        return;
    }

    g_acm_chn_handle = acm_chn_handle;
    flags.cshell_open_flags = (((u32)0x1 << (u32)CSHELL_USB) << 0x10) | ((u32)0x1 << CSHELL_USB);

    retval = bsp_icc_send((u32)ICC_CPU_MODEM, (u32)((ICC_CHN_IFC << 0x10) | IFC_RECV_FUNC_CSHELL), (u8 *)&flags,
                          (u32)sizeof(struct cshell_open_s));

    if (retval != (int)sizeof(struct cshell_open_s)) {
        cshell_port_err("acm_open_cb failed,icc send fail,ret=0x%x\n", retval);
        return;
    }
}
EXPORT_SYMBOL(acm_open_cb);

void acm_close_cb(void)
{
    int retval;
    int acm_chn_handle = g_acm_chn_handle;
    struct cshell_open_s flags;
    cshell_port_info("acm_close_cb called\n");

    (void)mdrv_udi_ioctl(acm_chn_handle, ACM_IOCTL_SET_READ_CB, NULL);
    (void)mdrv_udi_close(acm_chn_handle);
    g_acm_chn_handle = 0;
    flags.cshell_open_flags = (((u32)((u32)0x1 << (u32)CSHELL_USB)) << 0x10) & 0xFFFF0000;

    retval = bsp_icc_send((u32)ICC_CPU_MODEM, (u32)((ICC_CHN_IFC << 0x10) | IFC_RECV_FUNC_CSHELL), (u8 *)&flags,
                          (u32)sizeof(struct cshell_open_s)); /*lint !e838*/
    if (retval != (int)sizeof(struct cshell_open_s)) {
        cshell_port_err("acm_close_cb failed,icc send fail,ret=0x%x\n", retval);
        return;
    }
}
EXPORT_SYMBOL(acm_close_cb);

int cshell_connect(void)
{
    return establish_icc_chn(CON_CCORE, &g_bluetooth_con); /*lint !e713*/
}
EXPORT_SYMBOL(cshell_connect);

int cshell_port_init(void)
{
    struct task_struct *tsk_hdl = NULL;
#ifdef FEATURE_NVA_ON
    drv_uart_shell_stru_s uartcshell_nv = {
        .extendedbits = 0
    };

    if (bsp_nv_read(NVA_ID_DRV_UART_SHELL_FLAG, (u8 *)&uartcshell_nv, sizeof(drv_uart_shell_stru_s))) { /*lint !e747*/
        cshell_port_err("[A:CSHELL]:bsp_nva_read fails  line:%d uartcshell_nv.cshell_to_auart:%d\n", __LINE__,
                        uartcshell_nv.extendedbits);
    }

#else
    DRV_UART_SHELL_FLAG uartcshell_nv = {
        .extendedbits = 0
    };

    if (bsp_nvm_read(NV_ID_DRV_UART_SHELL_FLAG, (u8 *)&uartcshell_nv, sizeof(DRV_UART_SHELL_FLAG))) { /*lint !e747*/
        cshell_port_err("[A:CSHELL]:bsp_nv_read fails  line:%d uartcshell_nv.cshell_to_auart:%d\n", __LINE__,
                        uartcshell_nv.extendedbits);
    }
#endif
    sema_init(&g_recv_sem, 0);
    sema_init(&g_send_sem, 0);
    tsk_hdl = kthread_run(recv_func, NULL, "cshell_recv_thread"); /*lint !e838*/
    if (IS_ERR(tsk_hdl)) {
        cshell_port_err("tsk_hdl creat failed\n");
    }
    tsk_hdl = kthread_run(tsk_send_func, NULL, "cshell_send_thread"); /*lint !e548*/

    mdrv_usb_reg_enablecb(acm_open_cb);
    mdrv_usb_reg_disablecb(acm_close_cb);

    con_ring_init(&g_bluetooth_con.cmdbuf, g_bluetooth_cmdbuf, 1U << CMD_BUF_SHIFT); /*lint !e747*/
    con_ring_init(&g_bluetooth_con.logbuf, g_bluetooth_logbuf, 1U << LOG_BUF_SHIFT); /*lint !e747*/
    con_register(&g_bluetooth_con);

    if (uartcshell_nv.c_shell)
        cshell_connect();

    cshell_port_err("[LR:CSHELL] init OK\n");

    return 0; /*lint !e438*/
}
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
/*lint -e528*/
module_init(cshell_port_init);
#endif
