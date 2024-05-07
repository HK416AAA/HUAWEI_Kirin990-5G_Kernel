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

#ifndef CMUX_FRAMER_H
#define CMUX_FRAMER_H

#include "product_config.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_IOT_CMUX == FEATURE_ON)

// Macros for Actual frame content (during frame-formation)
#define CMUX_BASIC_MODE                  0
#define CMUX_DEFAULT_FRAME_SIZE          256
#define CMUX_PENDING_FRAME_SIZE          3080
#define CMUX_MAXIMUM_FRAME_SIZE          1540
#define CMUX_MINIMUM_FRAME_SIZE          31
#define CMUX_MAX_CHANNELS                63
#define CMUX_FRAME_FLAG                  0xF9
#define MAX_FRAME_LENGTH                 128
#define CMUX_FRAME_HDR(cm)      \
    (cm->mp.frame_size_n1 > (CMUX_LENGTH_MASK) ? CMUX_TWO_BYTE_HEADER_LENGTH : CMUX_ONE_BYTE_HEADER_LENGTH)
#define MAX_CMUX_FRAME_SIZE(cm) ((cm->mp.frame_size_n1) + CMUX_FRAME_HDR(cm))
#define MAX_DLC 63
#define MAX_PRIORITY 63
#define MAX_DLC_TGT 4  /* the number of dlcs in the target. */
#define MAX_CMUX_FRAMES 27
#define MAX_ACK_TIMER_T1 250
#define MAX_RESP_TIMER_T2 250
#define MAX_RETRANSMIT_N2 10
#define MAX_WINDOW_K 7
#define MIN_DEFAULT_ACK_TIMER_T1 100
#define MAX_DEFAULT_ACK_TIMER_T1 2500
#define MIN_DEFAULT_RETRANS_N2 0
#define MAX_DEFAULT_RETRANS_N2 10
#define DEFAULT_RETRANSMIT_N2 3
#define DEFAULT_WINDOW_K 2
#define MIN_DEFAULT_RES_TIMER_T2 100
#define MAX_DEFAULT_RES_TIMER_T2 2500

#define DEFAULT_WAKEUP_TIMER_T3 1
#define MAX_WAKEUP_TIMER_T3 255  /* sec */

#define TEST_LEN_LIMIT 129

#define CMUX_FRAME_DEFAULT_TL_LEN 2
#define CMUX_FRAME_PLUS_TL_LEN 3
#define CMUX_BUFF1_LEN 2
#define CMUX_BITS_OF_ONE_BYTE 8
#define CMUX_FRAME_ONE_BYTE_MAX_LEN 128
#define CMUX_FRAME_LENGTH_FIELD_OFFSET 3
#define CMUX_FRAME_END_FLAG_AND_FCS_LEN 2

#define CMUX_FRAME_LENGTH_BIT_OFFSET 7
#define CMUX_FRAME_LENGTH_EA_BIT_OFFSET 1
#define CMUX_PN_CONV_LAYER_TYPE_OFFSET 4
#define CMUX_FRAME_PF_BIT_OFFSET 4
#define CMUX_FRAME_DLCI_BIT_OFFSET 2

#define CMUX_SNC_FRMAE_S1_OFFSET 1
#define CMUX_SNC_FRMAE_S2_OFFSET 2
#define CMUX_SNC_FRMAE_S3_OFFSET 3
#define CMUX_SNC_FRMAE_S4_OFFSET 4
#define CMUX_SNC_FRMAE_S5_OFFSET 5
#define CMUX_SNC_FRMAE_S6_OFFSET 6
#define CMUX_SNC_FRMAE_S7_OFFSET 7

#define CMUX_MSC_FRMAE_FC_OFFSET 1
#define CMUX_MSC_FRMAE_RTC_OFFSET 2
#define CMUX_MSC_FRMAE_RTR_OFFSET 3
#define CMUX_MSC_FRMAE_RSV1_OFFSET 4
#define CMUX_MSC_FRMAE_RSV2_OFFSET 5
#define CMUX_MSC_FRMAE_IC_OFFSET 6
#define CMUX_MSC_FRMAE_DV_OFFSET 7

/* Command Frame Macros */
#define CMUX_CTRL_SABM_FRAME 0x3F
#define CMUX_CTRL_SABM_FRAME_LENGTH 0
#define CMUX_CTRL_UA 0x73
#define CMUX_CTRL_UA_FRAME_LENGTH 0
#define CMUX_CTRL_DM 0x1F
#define CMUX_CTRL_DM_FRAME_LENGTH 0
#define CMUX_CTRL_DISC 0x53
#define CMUX_CTRL_DISC_FRAME_LENGTH 0
/*
 * juvin: M2M-1669 to make the control frames as EF on sending and as well as responding..
 * this was made as per the chat done on 2/4/2014 with xiajiaxin
 */
#define CMUX_CTRL_UIH 0xEF
#define CMUX_DATA_UIH 0xEF  /* In pure UIH-data, P/F bit is NOT set */
#define CMUX_DATA_UI 0x03   /* In pure UI-data, P/F bit is NOT set */
#define CMUX_PPP_DATA 1
#define CMUX_VOICE_DLC 2
/* MUX Frames Macros */
#define CMUX_MUX_DLC_NEGO_PARAM_CMD 0x83
#define CMUX_MUX_DLC_NEGO_PARAM_RESP 0x81

#define CMUX_MUX_SERVICE_NEGO_CMD 0xD3
#define CMUX_MUX_SERVICE_NEGO_RESP 0xD1
#define CMUX_MUX_POWER_SAVING_CMD 0x43
#define CMUX_MUX_CLOSE_CMD 0xC3
#define CMUX_MUX_MODEM_STATUS_CMD 0xE3
#define CMUX_MUX_MODEM_STATUS_RESP 0xE1

#define CMUX_MUX_TEST_CMD 0x23
/* juvin<m2m-1594,1593,1580><proper NSC response format><24-march> */
#define CMUX_MUX_NON_SUPORTED_RES 0X11
#define CMUX_MUX_POWER_SAVING_RESP 0x41
#define CMUX_MUX_CLOSE_RESP 0xC1
#define CMUX_MUX_TEST_CMD_RESP 0x21

#define CMUX_FCON_CMD 0xA3
#define CMUX_FCON_RESP 0xA1
#define CMUX_FCOFF_CMD 0x63
#define CMUX_FCOFF_RESP 0x61
#define CMUX_RLS_CMD 0x53
#define CMUX_RLS_RESP 0x51
#define CMUX_RPN_CMD 0x93
#define CMUX_RPN_RESP 0x91

#define CONVERGENCE_LAYER 0
#define DEFAULT_PRIORITY 1
/* Length macros for each mux control frames */
#define CMUX_MUX_DLC_NEGO_PARAM_LENGTH 8
#define CMUX_MUX_SERVICE_NEGO_LENGTH 3
#define CMUX_MUX_POWER_SAVING_LENGTH 0
#define CMUX_MUX_CLOSE_CMD_LENGTH 0
#define CMUX_MUX_MODEM_STATUS_CMD_LENGTH 2
#define CMUX_MUX_TEST_CMD_LENGTH 0
#define CMUX_MUX_NON_SUPORTED_RES_LENGTH 1

#define CMUX_FCON_LENGTH 0
#define CMUX_FCOFF_LENGTH 0

#define CMUX_ONE_BYTE_HEADER_LENGTH 6
#define CMUX_TWO_BYTE_HEADER_LENGTH 7
#define CMUX_ONE_BYTE_HEADER_LENGTH_OFFSET 4
#define CMUX_TWO_BYTE_HEADER_LENGTH_OFFSET 5

#define CMUX_PF_BIT_MASK 0xEF
#define CMUX_LENGTH_MASK 0x7F
#define CMUX_EA_BIT_MASK 0x01
#define CMUX_DLC_BIT_MASK 0x3F
#define CMUX_PRIORITY_BIT_MASK 0x3F
#define CMUX_WINDOW_BIT_MASK 0x07
#define PASS 1
#ifndef FAIL
#define FAIL 0
#endif

#ifndef OK
#define OK 0
#endif

/*lint -e958 -e959 ;cause:64bit*/
struct framedetails {
    unsigned char frame_code;
    unsigned char frame_length;  /* will be hardcoded */
    unsigned int  accept_flag;
    unsigned int  reject_flag;
    int (*frame_rx_handler)(void* cm, void* rxfr, unsigned char dlc);
    unsigned int dlc_next_state;
    /*
     * For the given frame type, that is transmitted, what will be the DLC-state,
     * that will be auto-set after successful transmission of frame is set above
     * kick_start cmd, TE/UE response() is the central place where
     * the frame tranmssion actually happens, so that update there..
     */
};
/*lint +e958 +e959 ;cause:64bit*/
/*
 * MUX frame structures
 * Service Negotiation Parameter
 */
struct service_param_neg {
    unsigned char dlci;
    unsigned char service;  /* voice or data */
    unsigned char voice_codec;
    unsigned char reserved;
    /* Type field (EA, CR, code), length, will be hardcoded for this frame type, refer macro */
};
/* DLC-Mux-params can be used for a specific DLC or for entire channel hence making a structure for re-usability */
struct mux_params {
    unsigned int mode;
    unsigned int baud_rate;
    unsigned int frame_size_n1;    /* Max frame size (N1), (1-128), default 31 */
    unsigned int ack_timer_t1;     /* acknowledgement Timer (T1),default 0,1s */
    unsigned int retransmit_n2;    /* Max no of retransmission (NA), (0-255), default 3 */
    unsigned int window_k;         /* window size for error recovery mode (K) */
    unsigned int res_timer_t2;     /* Response Timer (T2),default 0,3s */
    unsigned int wakeup_timer_t3;  /* Wakeup Response Timer (T3),1-255s default 10s */
};

struct test_cmd
{
    unsigned short test_data_len;
    unsigned char reserved[6];
    unsigned char *dataptr;
};

/* DLC parameter Negotiation structure */
struct dlc_param_neg {
    /* Type field (EA, CR, code), length, will be hardcoded for this frame type, refer macro */
    unsigned char     dlci;        /* For which DLCI */
    unsigned char     priority;    /* Priority for particular DLC */
    unsigned char     conv_layer;  /* To be hardcoded refer macros */
    unsigned char     reserved;
    struct mux_params muxparams;
};

/* Modem status command structure */
struct modem_status_cmd {
    /* cmd, length to be hardcoded value */
    unsigned char dlci;         /* For which DLCI */
    unsigned char msc_command;  /* Modem status command or response */
    unsigned char fc : 1;       /* Flow control */
    unsigned char rtc : 1;      /* Ready to communicate */
    unsigned char rtr : 1;      /* Ready to Receive */
    unsigned char resrvd1 : 1;  /* reserved bit1 */
    unsigned char resrvd2 : 1;  /* reserved bit2 */
    unsigned char ic : 1;       /* Incoming call indicator */
    unsigned char dv : 1;       /* valid data sent */
    unsigned char EA : 1;       /* hard coded as 1 */
    unsigned char resrvd;
};

struct non_supported_cmd_resp {
    unsigned char nsc_cmd_type;
    unsigned char resrvd[3];
};

/* Service Negotiation command structure */
struct snc_cmd {
    unsigned char dlci;    /* For which DLCI */
    unsigned char s1 : 1;  /* Service Value 1 */
    unsigned char s2 : 1;  /* Service Value 2 */
    unsigned char s3 : 1;  /* Service Value 3 */
    unsigned char s4 : 1;  /* Service Value 4 */
    unsigned char s5 : 1;  /* Service Value 5 */
    unsigned char s6 : 1;  /* Service Value 6 */
    unsigned char s7 : 1;  /* Service Value 7 */
    unsigned char EA : 1;  /* hard coded as 1 */
    unsigned char resrvd[2];
};
enum fr_type {
    /* support for basic mode only */
    /* DLC Control Commands */
    CTRL_SABM = 0, /* Set Asynchronous Balanced Mode */
    CTRL_UA   = 1, /* Unnumbered Acknowledgement Response */
    CTRL_DM   = 2, /* Disconnected Mode */
    CTRL_DISC = 3, /* Disconnect mode */
    CTRL_UIH  = 4, /* Unnumbered information with header check and response */

    /* UIH MUX frames */
    DLC_PN_CMD  = 5,   /* DLC Parameter Negotiation command */
    DLC_PN_RESP = 6,   /* DLC Parameter Negotiation response */
    PSC_CMD     = 7,   /* Power saving Control command */
    PSC_RESP    = 8,   /* Power saving Control response */
    CLD_CMD     = 9,   /* Multiplexer Close Down command */
    CLD_RESP    = 10,  /* Multiplexer Close Down response */
    TEST_CMD    = 11,  /* Test Command command */
    TEST_RESP   = 12,  /* Test Command response */
    MSC_CMD     = 13,  /* Modem Status Command command */
    MSC_RESP    = 14,  /* Modem Status response */
    NSC         = 15,  /* Non Supported Command Response */
    SNC_CMD     = 16,  /* Service Negotiation Command */
    SNC_RESP    = 17,  /* Service Negotiation response */

    /* Data frames */
    UIH_DATA   = 18,
    FCON_CMD   = 19,
    FCON_RESP  = 20,
    FCOFF_CMD  = 21,  /* Flow control off command */
    FCOFF_RESP = 22,  /* Flow control off response */
    RLS_CMD    = 23,
    RLS_RESP   = 24,
    RPN_CMD    = 25,
    RPN_RESP   = 26
};
typedef unsigned int CMUX_FrTypeUint8;
/*
 * Frame error codes
 * write drop scenarios
 */
enum fr_err_code {
    FRAME_OK  = 0, /* store nfptr */
    NO_HEADER = 1,
    NO_TAIL   = 2,  /* too short frame, store nfptr and NOT rawptr */

    /* For all below error codes, use nfptr and call back */
    BASIC_FRAME_ERR   = 4,
    INVALID_FRAME_ERR = 5,    /* juvin: to send the nsc only to the invlid frames: 28march */
    FCS_ERR           = 8,    /* HARI */
    BASIC_FRLEN_ERR   = 16,   /* error in length */
    BASIC_FR_DLC_ERR  = 32,   /* dlc error */
    BASIC_FR_EA_ERR   = 64,   /* EA bit error */
    BASIC_FR_CR_ERR   = 128,  /* CR bit error */
    BASIC_FRTYPE_ERR  = 256,  /* frame type error */

    MUX_FRTYPE_ERR = 512,  /* Mux frame errors */
    DATA_ERR       = 1024,
    /*
     * in this case the first frame is taken return unconsumed-bytes (filled in length),
     * so as to callback with more data later
     */
    EXTRA_INCOMPLETE_FRAME = 4,
    EXTRA_ERROR_FRAME      = 8,  /* new frame does not have header */
    /* Header 0xf9, moved-IN but still tail not available, return value indicates */
    HEADER_MOVED_IN_INCOMPLETE_FRAME = 16,
    HEADER_MOVED_IN_COMPLETE_FRAME   = 32
};

/* Basic frame structure (includes control, mux and data) */

struct frame {
    /* used for control_field (or) UIH-Data (or) UIH-MUX */
    CMUX_FrTypeUint8    f_type;
    unsigned short int length;
    unsigned char      dlc; /* goes into addr-field */
    unsigned char      fcs;
    unsigned char      addr_EA_bit : 1; /* goes into addr-field */
    unsigned char      add_CR_bit : 1;  /* goes into addr-field */
    unsigned char      UI_bit : 1;      /* UI or UIH data */
    unsigned char      pf_bit;          /* poll or final bit */
    unsigned char      rsv[6];
    /*
     * points to DLC-0, UIH Mux or UIH data (for DLC-1-63 UIH pure data),
     * will be void* to a variable length structure or data,
     * typecasted based on fr_type
     */
    void* uih_data;
    /*
     * contains the framed data to be sent over actual serial port,
     * or received over serial port, useful for raw-dump-logs in QT
     */
    unsigned char* raw_data;
    /*
     * contains the length of the frame data to be sent over actual serial port,
     * or received over serial port, useful for raw-dump-logs in QT
     */
    unsigned short int raw_len;
    /* contains PROCEED_TO_NEXT, or not */
    unsigned char      flag;
    unsigned char      rsv1[5];
};

/* This structure is used for TE/UE based registration, which happens one time */
typedef struct framer {
    const struct framedetails* frdetails;
    int (*cmux_cmd_handler)(void* cmxlib, void* frm);
    unsigned int pending_mask;
    unsigned char rsv[4];
} FRAMER;

typedef struct dlcinfo {
    CMUX_FrTypeUint8  ftype;
    unsigned char    dlc;
    unsigned char    rsv[3];
} DLCINFO;

int cmux_free_frame(struct frame*);

#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* ifndef CMUX_FRAMER_H */
