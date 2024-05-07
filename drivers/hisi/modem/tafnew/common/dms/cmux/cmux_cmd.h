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

#ifndef CMUX_CMD_H
#define CMUX_CMD_H

#include "cmux_framer.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if (FEATURE_IOT_CMUX == FEATURE_ON)

#define ACK_TIMER_T1_DEFAULT_VALUE  10
#define FRAME_SIZE_N1_DEFAULT_VALUE 31
#define RETRANSMIT_N2_DEFAULT_VALUE 3
#define WINDOW_K_DEFAULT_VALUE      2
#define RES_TIMER_T2_DEFAULT_VALUE  0

#define CMUX_TE_MODE    1
#define CMUX_UE_MODE    2
#define ATCMUX_ON_MODE  4
#define CTRL_DLC_MODE   8
#define ATCMUX_OFF_MODE 16

#define CMUX_FRAGMENT_DATA  1
#define CMUX_FLUSH_DATA     2


#define TE_SENT_CB_TYPE          1
#define UE_SENT_CB_TYPE          2
#define DLC_STATE_CHANGE_CB_TYPE 4
#define AT_STATE_CHANGE_CB_TYPE  8
#define ERROR_FRAME_CB_TYPE      16

#define TIMEOUT_ON_ALL_RETRANSMISSION_CB_TYPE  32
#define TIMEOUT_RETRANSMISSION_CB_TYPE         64

#define TX_COMPLETE_CB_TYPE  128
#define RX_COMPLETE_CB_TYPE  256
#define TX_FAILED_CB_TYPE    512

#define GENERAL_NOTIFICATION_CB_TYPE    1024
#define DLC_ESTABLISH_COMPLETE_CB_TYPE  2048
/*
 * <juvin><M2M-1534><to inclrease the buffer size of serialisation
 * of At commands on all dlcs>
 */
#define DSTASK_MAX_BUFF_CMD_SIZE 128

#define CMUX_MEM_CHK_RTN_VAL(ret) if (ret) {\
    CMUX_PRINT(CMUX_DEBUG_INFO, "Cmux_memcpy_s wrong!\r\n");\
}

enum NEGOTIATION_CMD {
    MATCH    = 1,
    NO_MATCH = 2,
    MODIFY   = 3,
    COMPARE  = 4
};
typedef enum {
    SNC_TYPE_AT = 0,
    SNC_TYPE_DATA,
    SNC_TYPE_GPS,
    SNC_TYPE_AT_DATA,
    SNC_TYPE_INVALID
} SNC_Type;

/* Expects cmux_framer.h to be included first */

struct cmux_timer {
    unsigned int ack_timer_t1;     /* acknowledgement Timer (T1) */
    unsigned int retransmit_n2;    /* Max no of retransmission (NA), (0-255), default 3 */
    unsigned int window_k;         /* window size for error recovery mode (K) */
    unsigned int res_timer_t2;     /* Response Timer (T2) */
    unsigned int wakeup_timer_t3;  /* Wakeup Response Timer (T3) */
    /* coverity´íÎóÐÞ¸Ä£¬É¾³ýres_timer */
};

struct cmux_cmd {
    struct frame*     fr;
    struct cmux_timer ct;
    unsigned char reserved[4];
    struct cmux_cmd*  next;  /* next cmd_node */
};

struct DLC_statistics {
    unsigned int pending_tx_data;
    unsigned int pending_tx_total;
    unsigned int pending_tx_ctrl;
    unsigned int dlc_buffer_size;
};
struct snc_entry {
    unsigned char dlc;
    unsigned char reserved[3];
    SNC_Type      task;
};  /* new one added to include snc mapping for dlc in target. */
struct DLC_Entry {
    struct cmux_cmd *cmd_list;
    struct DLC_statistics stats;
    struct dlc_param_neg dlcparam;
    unsigned char reserved1[4];
    void *dlc_user_tcb_ptr;
    void *dlc_user_wm_ptr;
    struct modem_status_cmd msc;
    struct snc_cmd snc;
    /* AT-serialiser DLC Buffer */
    unsigned char at_buff [DSTASK_MAX_BUFF_CMD_SIZE];
    unsigned int dlc_state;
    unsigned short at_buff_used_len;
    unsigned char dlc;
    unsigned char reserved;
};

struct cmux_tgt_dlc_entry {
    unsigned char dlc;
    unsigned char dlc_status;
    unsigned char free_after_enqueue_to_task;
    unsigned char reserved[5];
    unsigned long signal_task;

    void *dlc_user_tcb_ptr;
    void *dlc_user_wm_ptr;
};

/*
 * Note: Struct DLC-entry represents only one-DLC entry,
 * refer to CMUXLib structure which contains array of pointers of type Struct DLC-entry,
 * or in other words each Non-NULL index points to an allocated dlc-entry
 */

/* LIST OF DLC-STATES */
#define DLC_ENTRY_DONE 0x1
#define DLC_COM_READY 0x02
#define DLC_COMMAND_FAILED 0x80000000
#define DLC_ESTABLISHED 0x40000000
#define DLC_CLOSED 0x20000000
#define DLC_NON_CRIT_COMMAND_FAILED 0x10000000

/* UE only */
#define UE_RECVD_DLC_PN_RESP_SENT 0x4
#define UE_RECVD_SABM_CMD_UA_SENT 0x8
#define UE_RECVD_SABM_CMD_DM_SENT 0x10
#define UE_RECVD_MSC_RESP_SENT 0x20
#define UE_RECVD_SNC_RESP_SENT 0x40
#define UE_RECVD_TEST_CMD_RESP_SENT 0x80
#define UE_RECVD_PSC_RESP_SENT 0x100
#define UE_SENT_NSC_RESP 0x200
#define UE_RECVD_UIH_DATA 0x400
#define UE_RECVD_DISC_CMD_UA_SENT 0x800
#define UE_RECVD_DISC_CMD_DM_SENT 0x1000
#define UE_SENT_DISC_CMD_UA_PENDING 0x2000
#define UE_SENT_DISC_CMD_UA_RECVD (~0x2000)
#define UE_RECVD_CLD_RESP_SENT 0x4000

#define UE_DLC_ENABLED 0x8000
#define UE_DLC_DISABLED 0x10000

#define UE_RECVD_FCON_RESP_SENT 0x20000
#define UE_RECVD_FCOFF_RESP_SENT 0x40000
#define UE_SENT_MUX_CLD_RESP_PENDING 0x80000
#define UE_SENT_MUX_CLD_RESP_RECVD (~0x80000)
/* shiyanan added for FCON/FCOFF command sending */
#define UE_SENT_FCON_PENDING 0x100000
#define UE_SENT_FCOFF_PENDING 0x200000
#define UE_SENT_FCON_RESP_RECVD (~0x100000)
#define UE_SENT_FCOFF_RESP_RECVD (~0x200000)

#define UE_PENDING_MASK UE_SENT_DISC_CMD_UA_PENDING

#define CHECK_DLC_STATE(DLC_STATE, EXPECTED_STATE) ((DLC_STATE & EXPECTED_STATE) == EXPECTED_STATE)
#define SET_DLC_STATE(DLC_STATE, NEW_STATE) ((DLC_STATE) |= (NEW_STATE))
#define CLEAR_DLC_STATE(DLC_STATE, STATE) ((DLC_STATE) &= (STATE))

#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* ifndef CMUX_CMD_H */
