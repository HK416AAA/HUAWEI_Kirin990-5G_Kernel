/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef CMUX_API_IFC_H
#define CMUX_API_IFC_H

#include "cmux_framer.h"
#include "cmux_cmd.h"
#if (VOS_OS_VER != VOS_LINUX)
#include "Linuxstub.h"
#endif
#ifndef WIN32
#include <linux/semaphore.h>
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if (FEATURE_IOT_CMUX == FEATURE_ON)
#define COMPORT_WIDTH 10

enum CMUX_MODE {
    CMUX_TEMODE = 1,
    CMUX_UEMODE = 2,
    ATCMUX_ON   = 4,
    CTRL_DLC    = 8,
    ATCMUX_OFF  = 16
};
typedef unsigned int CMUX_ModeUint8;
/* The structure ds_mux_cmux_info_type holds the parameters passed +CMUX and is as per 3GPP 27.010 spec */
typedef struct {
    /* Mode 0 - basic, 1- advanced */
    unsigned int operating_mode;
    /* 0-UIH, 1-UI, 2-I frames */
    unsigned int subset;
    /* Max re-tries N2 (0-255) */
    unsigned int max_cmd_num_tx_times_N2;
    /* Window size default 2, range (1-7) */
    unsigned int window_size_k;
    /* port speed valid values: 1-6 */
    unsigned int port_speed;
    /* Max frame size (1-32768) */
    unsigned int max_frame_size_N1;
    /*
     * Time UE waits for an acknowledgement before. resorting to other action (e.g. transmitting a frame)
     * default 100ms min value 10 ms.
     */
    unsigned int response_timer_T1;
    /* Time mux control channel waits before. re-transmitting a command default 300ms and. min value 20 ms */
    unsigned int response_timer_T2;
    /* Time UE waits for response for a power wake-up command default 10ms and min. value 1 ms */
    unsigned int response_timer_T3;
} cmux_info_type;

#define MAX_SERIAL_PKT_LEN 672
#define MAX_TX_DATA_PKT_LEN(cm) cm->mp.frame_size_n1
#define MAX_TEST_DATA 1540
#define MAX_TEST_DATA_PKT_LEN(cm) (MAX_TEST_DATA - CMUX_FRAME_HDR(cm) - 2)

typedef struct frame_result {
    unsigned char  start_flag : 1; /* Flags below indicate the pending frame details */
    unsigned char  length_flag : 1;
    unsigned char  fcs : 1;
    unsigned char  end_flag : 1;
    unsigned char  err : 1;
    unsigned char  rsv;
    unsigned char  rsv1[2];
    unsigned short start_offset; /* 0..n */
    unsigned short end_offset;   /* 1..n */
} FRAME_RESULT;

typedef struct cmux_input_frame {
    unsigned char *pending_frame;/* CMUX appends data into this*/
    unsigned short pending_frame_idx;/* points to index of where to put data*/
    unsigned char  reserved[6];
    struct frame_result result;
} CMUX_INPUT_FRAME;

enum callback_type {
    TE_SENT          = 1,
    UE_SENT          = 2,
    DLC_STATE_CHANGE = 4,
    AT_STATE_CHANGE  = 8,
    ERROR_FRAME = 16,                   /* called by Input_frame_to_CMUX */
    TIMEOUT_ON_ALL_RETRANSMISSION = 32, /* called by command-node->rx_handler, on all-fails */
    /* called by command-node->rx_handler, more like informing upper layer for retransmission */
    TIMEOUT_RETRANSMISSION = 64, /* called by kick_start() on one-frame transmission */
    TX_COMPLETE = 128,
    RX_COMPLETE = 256,
    TX_FAILED              = 512, /* called by kick_start() on error, and it is a critical issue */
    GENERAL_NOTIFICATION   = 1024,
    DLC_ESTABLISH_COMPLETE = 2048
};

/*
 * Below structure is a parameter to cmux_register_callback(). which is invoked by QT or C-unit for call backs.
 * Note QT fills this structure and calls cmux_register_callback, while QT is called back with this same structure
 */
struct cmux_callback {
    enum callback_type callback;  /* consumed by QT/C-unit, filled by CMUXLib */
    unsigned char  reserved[4];
    struct frame*      fr;        /* consumed by QT/C-unit, filled by CMUXLib */
    void (*callback_func)(unsigned int cbtype, const void* ptr, struct cmux_timer* ct);
#if (defined(LLT_OS_VER))
    int (*send_to_com)(char* com_name, unsigned char* rawptr, unsigned short len);
#else
    unsigned int (*send_dl_data)(unsigned char* rawptr, unsigned short len);
#endif
    unsigned int (*set_power_cb)(void);
    /*
     * timer structure has meaning only in RX-path, where the callback_type can have TIMEOUT_RETRANSMISSION value,
     * so the QT application can open this structure to figure out retransmission count
     */
    struct cmux_timer* ct;
};

/*
 * cmuxlib is the main structure which the QT library will use when calling any entry functions into CMUX library,
 * as it is only through this structure does the CMUX library remember its state of DLCs, commands,
 * frames sent/received etc, This structure is initialized using cmux_init, destroyed using cmux_del routines
 */
typedef struct cmuxlib {

    CMUX_ModeUint8 cmux_status;                /* Mux-on/off , Control-DLC established-status */
    char com_port [COMPORT_WIDTH];
    unsigned short dlc_count;                 /* Mux-Parameters set through AT+CMUX */
    struct mux_params mp;                     /* mux_params refere cmux_framer */
    struct qt_com_status* comdb;              /* For QT ?reference & usage only */
    struct DLC_Entry* dlc_db[MAX_DLC + 1];    /* array of pointers to DLC-Entry, only for allocated DLCs */
    struct snc_entry snc_db[MAX_DLC_TGT + 1]; /* should be changes as  per the number of dlcs required in target */
    struct cmux_input_frame cmux_if;          /* Frame details */
    struct cmux_callback    cb;
    unsigned int   ue_dlc_ctrl_0;
    unsigned int   ue_dlc_ctrl_1;
    char           flow_ctrl;
    unsigned char  reserved[7];
    /*Framer related*/
    struct framer  frame_map; /* Framer related */
    void*          qtcom;
    void*          qttimer;
    struct file*   fp;        /* 存放设备指针，linux用 */
    struct         semaphore stKickSem;
} CMUXLIB;

typedef int (*PROCFUNC)(struct cmuxlib*, struct dlcinfo, struct frame*, unsigned char*, unsigned int data_len);
typedef struct _defaultframeprocess {
    CMUX_FrTypeUint8  frType;
    unsigned int     reserved;
    PROCFUNC         func;
} defaultframeprocess;

typedef unsigned int (*procFunc)(struct frame*, unsigned char**, unsigned int*, unsigned char *ptr, unsigned int templen);

typedef struct _structToFrameProcess {
    CMUX_FrTypeUint8  frType;
    unsigned int     reserved;
    procFunc         func;
} structToFrameProcess;

struct cmuxlib* cmux_cmuxlib_init(CMUX_ModeUint8 mode, cmux_info_type* cmux_info);
int cmux_register_callback(struct cmuxlib* cm, struct cmux_callback* callbk);
int cmux_Input_frame_to_cmux(struct cmuxlib* cm, unsigned char* buffer, unsigned short buffer_len);
int cmux_send_dlc_data(struct cmuxlib* cmxlib, unsigned char* buff, unsigned short len, unsigned char dlc, unsigned int control);
int cmux_send_msc(struct cmuxlib* cm, unsigned char dlc, struct modem_status_cmd* msc);
int cmux_send_fcon_fcoff(struct cmuxlib* cm, unsigned char dlc, CMUX_FrTypeUint8 ftype);
void cmux_fcon_fcoff_check_dlc_state(unsigned int *dlc0_state, int *stop_proc);
#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* CMUX_API_IFC_H */
