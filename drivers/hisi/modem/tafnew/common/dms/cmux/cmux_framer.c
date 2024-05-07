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

#include "cmux_framer.h"
#include "cmux_cmd.h"
#include "cmux_api_ifc.h"
#include "cmux_tgt_osal.h"
#include "securec.h"


#ifndef WIN32
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/fs.h>
#endif /* WIN32 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#if (FEATURE_IOT_CMUX == FEATURE_ON)
/* juvin<m2m-1594,1593,1580><proper NSC response format><24-march> */
unsigned char* g_nsc_frm      = 0;
unsigned int   g_nsc_received = 0;

#define NO_ACCEPT_FLAGS 0xFFFFFFFF
#define NO_REJECT_FLAGS 0x00000000

int default_frame_sabm_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_msc_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);


int default_frame_snc_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_dlc_pn_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_cld_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_test_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_psc_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_nsc_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_fc_resp_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_fc_cmd_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

int default_frame_uih_fill_para(struct cmuxlib *cm, DLCINFO dlc_info, struct frame *lfr,
    unsigned char *data, unsigned int data_len);

unsigned int cmux_fill_header_struct_to_frame (struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);

unsigned int cmux_struct_to_msc_frame(struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);

unsigned int mux_dlcpn_struct_to_frame (struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);

unsigned int cmux_struct_to_test_frame (struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);

unsigned int cmux_struct_to_nsc_frame(struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);

unsigned int cmux_struct_to_uih_frame(struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);

unsigned int cmux_struct_to_snc_frame(struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);
unsigned int mux_struct_to_frame (struct frame *fr_temp,
    unsigned char **vrawptr, unsigned int *offset, unsigned char *ptr, unsigned int templen);

extern int cmux_ue_rxed_ua (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_cld_resp (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_te_ue_rxed_dm (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_uihdata (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_dlcpn_cmd (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_test_cmd (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_psc_cmd (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_msc_cmd (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_snc_cmd (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_cld_cmd (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_nsc_handler (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_fcon_cmd (void *cmxlib,void *frm, unsigned char dlc);

extern int cmux_ue_rxed_fcoff_cmd (void *cmxlib, void *frm, unsigned char dlc);

extern int cmux_ue_rxed_fcon_fcoff_resp (void *cmxlib, void *frm, unsigned char dlc);


/*lint -e133 -e43*/
const struct framedetails g_UE_frame_details[] = {
    { CMUX_CTRL_SABM_FRAME, CMUX_CTRL_SABM_FRAME_LENGTH, 0, 0, 0, 0 },
    { CMUX_CTRL_UA, CMUX_CTRL_UA_FRAME_LENGTH, (UE_SENT_DISC_CMD_UA_PENDING | DLC_ESTABLISHED), DLC_CLOSED,
      cmux_ue_rxed_ua, 0 },
    { CMUX_CTRL_DM, CMUX_CTRL_DM_FRAME_LENGTH, UE_SENT_DISC_CMD_UA_PENDING, NO_REJECT_FLAGS, cmux_te_ue_rxed_dm, 0 },
    { CMUX_CTRL_DISC, CMUX_CTRL_DISC_FRAME_LENGTH, DLC_ESTABLISHED, NO_REJECT_FLAGS, 0, 0 },
    { CMUX_CTRL_UIH, 0, 0, 0, 0, 0 },
    { CMUX_MUX_DLC_NEGO_PARAM_CMD, CMUX_MUX_DLC_NEGO_PARAM_LENGTH, NO_ACCEPT_FLAGS, NO_REJECT_FLAGS,
      cmux_ue_rxed_dlcpn_cmd, 0 }, /*lint !e569*/
    { CMUX_MUX_DLC_NEGO_PARAM_RESP, CMUX_MUX_DLC_NEGO_PARAM_LENGTH, 0, 0, 0, 0 },
    { CMUX_MUX_POWER_SAVING_CMD, CMUX_MUX_POWER_SAVING_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_psc_cmd, 0 },
    { CMUX_MUX_POWER_SAVING_RESP, CMUX_MUX_POWER_SAVING_LENGTH, 0, 0, 0, 0 },
    { CMUX_MUX_CLOSE_CMD, CMUX_MUX_CLOSE_CMD_LENGTH, DLC_ESTABLISHED, NO_REJECT_FLAGS, cmux_ue_rxed_cld_cmd, 0 },
    { CMUX_MUX_CLOSE_RESP, CMUX_MUX_CLOSE_CMD_LENGTH, (UE_SENT_MUX_CLD_RESP_PENDING | DLC_ESTABLISHED), DLC_CLOSED,
      cmux_ue_rxed_cld_resp, 0 },
    { CMUX_MUX_TEST_CMD, CMUX_MUX_TEST_CMD_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_test_cmd, 0 },
    { CMUX_MUX_TEST_CMD_RESP, CMUX_MUX_TEST_CMD_LENGTH, 0, 0, 0, 0 },
    { CMUX_MUX_MODEM_STATUS_CMD, CMUX_MUX_MODEM_STATUS_CMD_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_msc_cmd,
      0 },
    { CMUX_MUX_MODEM_STATUS_RESP, CMUX_MUX_MODEM_STATUS_CMD_LENGTH, 0, 0, 0, 0 },
    { CMUX_MUX_NON_SUPORTED_RES, CMUX_MUX_NON_SUPORTED_RES_LENGTH, (DLC_ESTABLISHED | DLC_CLOSED), NO_REJECT_FLAGS,
      cmux_ue_nsc_handler, 0 },
    { CMUX_MUX_SERVICE_NEGO_CMD, CMUX_MUX_SERVICE_NEGO_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_snc_cmd, 0 },
    { CMUX_MUX_SERVICE_NEGO_RESP, CMUX_MUX_SERVICE_NEGO_LENGTH, 0, 0, 0, 0 },
    { CMUX_DATA_UIH, 0, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_uihdata, 0 },
    { CMUX_FCON_CMD, CMUX_FCON_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_fcon_cmd, 0 },
    { CMUX_FCON_RESP, CMUX_FCON_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_fcon_fcoff_resp, 0 },
    { CMUX_FCOFF_CMD, CMUX_FCOFF_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_fcoff_cmd, 0 },
    { CMUX_FCOFF_RESP, CMUX_FCOFF_LENGTH, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_rxed_fcon_fcoff_resp, 0 },
    { CMUX_RLS_CMD, 0, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_nsc_handler, 0 },
    { CMUX_RLS_RESP, 0, 0, 0, 0, 0 },
    { CMUX_RPN_CMD, 0, DLC_ESTABLISHED, DLC_CLOSED, cmux_ue_nsc_handler, 0 },
    { CMUX_RPN_RESP, 0, 0, 0, 0, 0 }
}; /*lint +e133 +e43*/

static const unsigned char g_crc_table[] = {
    0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75, 0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B, 0x1C, 0x8D, 0xFF,
    0x6E, 0x1B, 0x8A, 0xF8, 0x69, 0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67, 0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE,
    0xDC, 0x4D, 0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43, 0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51, 0x2A,
    0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F, 0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05, 0x7E, 0xEF, 0x9D, 0x0C,
    0x79, 0xE8, 0x9A, 0x0B, 0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19, 0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86,
    0x17, 0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D, 0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33, 0x54, 0xC5,
    0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21, 0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F, 0xE0, 0x71, 0x03, 0x92, 0xE7,
    0x76, 0x04, 0x95, 0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B, 0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89,
    0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87, 0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD, 0xD6, 0x47, 0x35,
    0xA4, 0xD1, 0x40, 0x32, 0xA3, 0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1, 0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C,
    0x2E, 0xBF, 0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5, 0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB, 0x8C,
    0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9, 0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7, 0xA8, 0x39, 0x4B, 0xDA,
    0xAF, 0x3E, 0x4C, 0xDD, 0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3, 0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50,
    0xC1, 0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF
};

const defaultframeprocess g_defaultFrameProcessTbl [] = {
    {CTRL_SABM,  0, default_frame_sabm_fill_para},
    {CTRL_UA,    0, default_frame_sabm_fill_para},
    {CTRL_DM,    0, default_frame_sabm_fill_para},
    {CTRL_DISC,  0, default_frame_sabm_fill_para},
    {MSC_CMD,    0, default_frame_msc_fill_para},
    {MSC_RESP,   0, default_frame_msc_fill_para},
    {SNC_CMD,    0, default_frame_snc_fill_para},
    {SNC_RESP,   0, default_frame_snc_fill_para},
    {DLC_PN_CMD, 0, default_frame_dlc_pn_fill_para},
    {DLC_PN_RESP,0, default_frame_dlc_pn_fill_para},
    {CLD_CMD,     0,default_frame_cld_fill_para},
    {CLD_RESP,   0, default_frame_cld_fill_para},
    {TEST_CMD,   0, default_frame_test_fill_para},
    {TEST_RESP,  0, default_frame_test_fill_para},
    {PSC_CMD,    0, default_frame_psc_fill_para},
    {PSC_RESP,   0, default_frame_psc_fill_para},
    {NSC,        0, default_frame_nsc_fill_para},
    {FCON_RESP,  0, default_frame_fc_resp_fill_para},
    {FCOFF_RESP, 0, default_frame_fc_resp_fill_para},
    {FCON_CMD,   0, default_frame_fc_cmd_fill_para},
    {FCOFF_CMD,  0, default_frame_fc_cmd_fill_para},
    {UIH_DATA,  0,  default_frame_uih_fill_para},
};
const structToFrameProcess g_structToFrameProcessTbl[] = {
    {CTRL_SABM,  0, cmux_fill_header_struct_to_frame},
    {CTRL_UA,    0, cmux_fill_header_struct_to_frame},
    {CTRL_DM,    0, cmux_fill_header_struct_to_frame},
    {CTRL_DISC,  0, cmux_fill_header_struct_to_frame},
    {MSC_CMD,    0, cmux_struct_to_msc_frame},
    {MSC_RESP,   0, cmux_struct_to_msc_frame},
    {SNC_CMD,    0, cmux_struct_to_snc_frame},
    {SNC_RESP,   0, cmux_struct_to_snc_frame},
    {DLC_PN_CMD, 0, mux_dlcpn_struct_to_frame},
    {DLC_PN_RESP,0, mux_dlcpn_struct_to_frame},
    {CLD_CMD,    0, mux_struct_to_frame},
    {CLD_RESP,   0, mux_struct_to_frame},
    {TEST_CMD,   0, cmux_struct_to_test_frame},
    {TEST_RESP,  0, cmux_struct_to_test_frame},
    {PSC_CMD,    0, mux_struct_to_frame},
    {PSC_RESP,   0, mux_struct_to_frame},
    {NSC,        0, cmux_struct_to_nsc_frame},
    {FCON_RESP,  0, mux_struct_to_frame},
    {FCOFF_RESP, 0, mux_struct_to_frame},
    {FCON_CMD,   0, mux_struct_to_frame},
    {FCOFF_CMD,  0, mux_struct_to_frame},
    {UIH_DATA,   0, cmux_struct_to_uih_frame},
};
unsigned int cmux_get_g_nsc_received(void)
{
    return g_nsc_received;
}
void cmux_set_g_nsc_received(unsigned int nsc_received)
{
    g_nsc_received = nsc_received;
}

static unsigned char cmux_ts0710_compute_crc(const unsigned char* data, int len)
{
    unsigned char crc       = 0xFF;
    unsigned char crc_const = 0xFF;
    unsigned int  i;

    for (i = 0; i < (unsigned int)len; i++) {
        crc = g_crc_table[crc ^ data[i]];
    }

    crc_const = crc_const - crc;
    return (crc_const);
}

static unsigned char cmux_ts0710_check_fcs(const unsigned char* data, int len, unsigned char cfcs)
{
    unsigned char crc = 0xFF;
    int           i;

    for (i = 0; i < len; i++) {
        crc = g_crc_table[crc ^ data[i]];
    }

    crc = g_crc_table[crc ^ cfcs];

    if (crc == 0xcf) {
        return 0;
    } else {
        return 1;
    }
} /* cmux_ts0710_check_fcs */


int cmux_free_frame(struct frame* fr)
{
    if (fr == NULL) {
        return FAIL;
    } else {
        if (fr->uih_data != NULL) {
            cmux_free((unsigned char*)fr->uih_data);
        }
        if (fr->raw_data != NULL) {
            cmux_free((unsigned char*)fr->raw_data);
        }
        cmux_free((unsigned char*)fr);
        /* juvin<m2m-1638,1639,1641><Crash on wrong frame format,avoid dangling pointer> */
        fr = NULL;
    }

    return PASS;
} /* cmux_free_frame */


unsigned int cmux_check_length(unsigned char* rawptr, struct frame* fr_temp)
{
    const struct framedetails* frame_details = g_UE_frame_details;
    int                        len_vl;

    if ((((*rawptr) & CMUX_EA_BIT_MASK) == (CMUX_EA_BIT_MASK))) {
        if ((((*rawptr) >> 1) & 0x7F) != frame_details[fr_temp->f_type].frame_length) {
            return INVALID_FRAME_ERR;
        }
        /* juvin:M2M-1669: to send NSC if the length valud field is not correct april2 : begin */
        len_vl = (((*rawptr) >> 1) & 0x7F);
        /* check frame end flag(0xF9) */
        if (*(rawptr + len_vl + CMUX_FRAME_END_FLAG_AND_FCS_LEN) != 0xF9) {
            return INVALID_FRAME_ERR;
        }
        /* juvin:M2M-1669: to send NSC if the length valud field is not correct april2 : end */
    } else {
        return INVALID_FRAME_ERR;
    }
    return PASS;
} /* cmux_check_length */

int cmux_frame_data_snc_cmd(unsigned char* rwptr, unsigned char* cmd_struct, char cmd_type)
{
    struct snc_cmd*           snc;
    unsigned int              i;

    snc = (struct snc_cmd*)cmd_struct;
    /* Service Value位中EA位必须为1，S1必须为0或1，S2-7必须为0 */
    if (snc->EA != 1) {
        return 0;
    }
    if (((snc->dlci) > MAX_DLC_TGT)) {
        return 0;
    }
    if ((snc->s2 != 0) || (snc->s3 != 0) || (snc->s4 != 0) || (snc->s5 != 0) || (snc->s6 != 0) || (snc->s7 != 0)) {
        return 0;
    }
    /* Voice Codec Value位中EA位必须为1，V1-7必须为0 */
    if (((*(rwptr + 1)) & CMUX_EA_BIT_MASK) != CMUX_EA_BIT_MASK) {
        return 0;
    }
    /* all other bits must be zero ,get high 7 bits (bit1-bit7) */
    for (i = 1; i <= CMUX_FRAME_LENGTH_BIT_OFFSET; i++) {
        if (((*(rwptr + 1)) >> i) != 0) {
            return 0;
        }
    }
    return 1;
}

int cmux_frame_data_msc_cmd(struct cmuxlib* cm, unsigned char* cmd_struct, char cmd_type)
{
    struct modem_status_cmd* msc;

    msc = (struct modem_status_cmd*)cmd_struct;
    if ((msc->dlci) > MAX_DLC_TGT) {
        return 0;
    }
    if (cm->dlc_db[msc->dlci] == NULL) {
        return 0;
    }
    /* V.24 signals位中EA位必须为1，fc位必须为0或1，resrvd1-2位必须为0 */
    if (msc->EA != 1) {
        return 0;
    }
    if ((msc->resrvd1 != 0) || (msc->resrvd2 != 0)) {
        return 0;
    }
    return 1;
}

/* juvin:M2M-1669: to send NSC if the data field in snc and msc field is not correct april2 : begin */
int check_frame_data(struct cmuxlib* cm, unsigned char* rwptr, unsigned char* cmd_struct, char cmd_type)
{
    if (cmd_type == SNC_CMD) {
        if (cmux_frame_data_snc_cmd(rwptr, cmd_struct, cmd_type)) {
            return 1;
        }

        return 0;
    }

    if (cmd_type == MSC_CMD) {
        if (cmux_frame_data_msc_cmd(cm, cmd_struct, cmd_type)) {
            return 1;
        }

        return 0;
    }

    return 1;
} /* check_frame_data */
/* juvin:M2M-1669: to send NSC if the data field in snc and msc field is not correct april2 : end */

/* DLC_PN_CMD 命令解析 */
unsigned int mux_frame_parse_dlc_pn_cmd(unsigned char* rawptr, struct cmuxlib* cm, struct frame* fr_temp)
{
    unsigned int          rc;
    struct dlc_param_neg* dlcpn = (struct dlc_param_neg*)NULL;

    rc = cmux_check_length(rawptr, fr_temp);
    if (rc != PASS) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }

    fr_temp->uih_data = (void*)cmux_alloc(sizeof(struct dlc_param_neg));
    if (fr_temp->uih_data == NULL) {
        rc = BASIC_FRAME_ERR;
        return rc;
    }
    dlcpn = (struct dlc_param_neg*)fr_temp->uih_data;

    /* 00 00 D6 D5 D4 D3 D2 D1  通道号DLC */
    ++rawptr;
    if ((*rawptr > MAX_DLC_TGT) || (*rawptr == 0)) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    dlcpn->dlci = *rawptr;

    /* CL4 CL3 CL2 CL1 I4 I3 I2 I1  会聚类型 帧类型 */
    ++rawptr;
    if (*rawptr > 0) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    dlcpn->conv_layer = ((*rawptr) >> CMUX_PN_CONV_LAYER_TYPE_OFFSET) & 0x0F;

    /* 00 00 P6 P5 P4 P3 P2 P1  优先级 */
    ++rawptr;
    dlcpn->priority = *rawptr & 0x3F;

    /* T8 T7 T6 T5 T4 T3 T2 T1  确认定时器T1 */
    ++rawptr;
    if (*rawptr > MAX_ACK_TIMER_T1) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    dlcpn->muxparams.ack_timer_t1 = *rawptr;

    /*
     * N8  N7  N6  N5  N4  N3  N2  N1
     * N16 N15 N14 N13 N12 N11 N10 N9  最大帧长N1
     */
    ++rawptr;
    dlcpn->muxparams.frame_size_n1 = ((*rawptr) & 0xFF);
    ++rawptr;

    dlcpn->muxparams.frame_size_n1 |= (*rawptr) << CMUX_BITS_OF_ONE_BYTE;
    if (dlcpn->muxparams.frame_size_n1 > CMUX_MAXIMUM_FRAME_SIZE) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }

    /* NA8 NA7 NA6 NA5 NA4 NA3 NA2 NA1  最大重传次数N2 */
    ++rawptr;
    /* juvin :M2M-1669:check retransmit n2 and return INVALID_FRAME_ERR: april 2 :begin */
    if (*rawptr > MAX_RETRANSMIT_N2) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    /* juvin :M2M-1669:check retransmit n2 and return INVALID_FRAME_ERR: april 2 :end */
    dlcpn->muxparams.retransmit_n2 = *rawptr;

    /* 00 00 00 00 00 K3 K2 K1  错误恢复模式的窗口大小K */
    ++rawptr;
    if (*rawptr > MAX_WINDOW_K) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    dlcpn->muxparams.window_k = *rawptr;

    /* copy dlcpn structure into uih_data void pointer */
    fr_temp->uih_data = (void*)dlcpn;
    ++rawptr;

    return rc;
}

/* TEST_CMD 命令解析 */
unsigned int mux_frame_parse_test_cmd(unsigned char* rawptr, struct cmuxlib* cm, struct frame* fr_temp)
{
    unsigned int rc       = PASS;
    unsigned int test_len = 0;
    int          offset   = 0;
    int          ret;

    test_len = (((*rawptr) >> 1) & 0x7F);
    if (((*rawptr) & CMUX_EA_BIT_MASK) != CMUX_EA_BIT_MASK) {
        test_len = ((*(rawptr + 1)) << CMUX_FRAME_LENGTH_BIT_OFFSET) | test_len;
        offset   = 1;
    }
    if ((*(rawptr + test_len + CMUX_FRAME_END_FLAG_AND_FCS_LEN + offset)) != 0xF9) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    ++rawptr;
    fr_temp->uih_data = (void*)cmux_alloc((int)test_len);
    if (fr_temp->uih_data == NULL) {
        rc = BASIC_FRAME_ERR;
        return rc;
    }
    if (test_len > 0) {
        ret = memcpy_s(fr_temp->uih_data, (size_t)test_len, rawptr + offset, (size_t)test_len);
        CMUX_MEM_CHK_RTN_VAL(ret);
    }

    return rc;
}

/* SNC_CMD 命令解析 */
unsigned int mux_frame_parse_snc_cmd(unsigned char* rawptr, struct cmuxlib* cm, struct frame* fr_temp)
{
    unsigned int    rc;
    struct snc_cmd* snc      = (struct snc_cmd*)NULL;
    int             chk_val;

    rc = cmux_check_length(rawptr, fr_temp);
    if (rc != PASS) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }

    ++rawptr;
    if (((*rawptr) & CMUX_EA_BIT_MASK) != CMUX_EA_BIT_MASK) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    if ((((*rawptr) >> 1) & 0x01) != 0x01) {
        rc = INVALID_FRAME_ERR;  /* mandatory that this bit is 1 */
        return rc;
    }

    /* juvin:M2M-1669: to send NSC if the length valud field is not correct april2 : end */
    /* juvin<m2m-595><length validation to avoid crash in case of wrong length value><24-march><End> */
    fr_temp->uih_data = (void*)cmux_alloc(sizeof(struct snc_cmd));
    if (fr_temp->uih_data == NULL) {
        rc = BASIC_FRAME_ERR;
        return rc;
    }
    snc = (struct snc_cmd*)fr_temp->uih_data;

    snc->dlci = ((*rawptr) >> CMUX_FRAME_DLCI_BIT_OFFSET) & 0x3F;
    if (snc->dlci > MAX_DLC_TGT) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    rawptr++;

    snc->EA = (*rawptr) & 0x01;
    snc->s1 = ((*rawptr) >> CMUX_SNC_FRMAE_S1_OFFSET);
    snc->s2 = ((*rawptr) >> CMUX_SNC_FRMAE_S2_OFFSET);
    snc->s3 = ((*rawptr) >> CMUX_SNC_FRMAE_S3_OFFSET);
    /* juvin: M2M-1669:to to validate total snc data april2 : begin */
    snc->s4 = ((*rawptr) >> CMUX_SNC_FRMAE_S4_OFFSET);
    snc->s5 = ((*rawptr) >> CMUX_SNC_FRMAE_S5_OFFSET);
    snc->s6 = ((*rawptr) >> CMUX_SNC_FRMAE_S6_OFFSET);
    snc->s7 = ((*rawptr) >> CMUX_SNC_FRMAE_S7_OFFSET);

    chk_val = check_frame_data(cm, rawptr, (unsigned char*)snc, (char)SNC_CMD);
    if (chk_val == 0) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }

    /* juvin: M2M-1669: to to validate total snc data april2 : end */
    fr_temp->uih_data = (void*)snc;

    return rc;
}

/* MSC_CMD 命令解析 */
unsigned int mux_frame_parse_msc_cmd(unsigned char* rawptr, struct cmuxlib* cm, struct frame* fr_temp)
{
    unsigned int             rc       = PASS;
    struct modem_status_cmd* msc      = (struct modem_status_cmd*)NULL;
    int                      chk_val;

    rc = cmux_check_length(rawptr, fr_temp);
    if (rc != PASS) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }

    ++rawptr;
    if (((*rawptr) & CMUX_EA_BIT_MASK) != CMUX_EA_BIT_MASK) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
    if ((((*rawptr) >> 1) & 0x01) != 0x01) {
        rc = INVALID_FRAME_ERR;  /* mandatory that this bit is 1 */
        return rc;
    }

    /* juvin:M2M-1669: to send NSC if the length valud field is not correct april2 : end */
    /* juvin<m2m-595><length validation to avoid crash in case of wrong length value><24-march><End> */
    fr_temp->uih_data = (void*)cmux_alloc(sizeof(struct modem_status_cmd));
    if (fr_temp->uih_data == NULL) {
        rc = BASIC_FRAME_ERR;
        return rc;
    }
    msc = (struct modem_status_cmd*)fr_temp->uih_data;

    msc->dlci = ((*rawptr) >> CMUX_FRAME_DLCI_BIT_OFFSET) & 0x3F;
    if (msc->dlci > MAX_DLC_TGT) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }

    rawptr++;
    msc->EA  = (*rawptr) & 0x01;
    msc->fc  = ((*rawptr) >> CMUX_MSC_FRMAE_FC_OFFSET);
    msc->rtc = ((*rawptr) >> CMUX_MSC_FRMAE_RTC_OFFSET);
    msc->rtr = ((*rawptr) >> CMUX_MSC_FRMAE_RTR_OFFSET);
    /* juvin: M2M-1669:to to validate total msc data april2 : begin */
    msc->resrvd1 = ((*rawptr) >> CMUX_MSC_FRMAE_RSV1_OFFSET);
    msc->resrvd2 = ((*rawptr) >> CMUX_MSC_FRMAE_RSV2_OFFSET);
    msc->ic      = ((*rawptr) >> CMUX_MSC_FRMAE_IC_OFFSET);
    msc->dv      = ((*rawptr) >> CMUX_MSC_FRMAE_DV_OFFSET);

    chk_val = check_frame_data(cm, rawptr, (unsigned char*)msc, (char)MSC_CMD);
    if (chk_val == 0) {
        rc = INVALID_FRAME_ERR;
        return rc;
    }

    /* juvin:M2M-1669: to to validate total msc data april2 : begin */
    fr_temp->uih_data = (void*)msc;

    return rc;
}

/* NSC 命令解析 */
unsigned int mux_frame_parse_nsc(unsigned char* rawptr, struct cmuxlib* cm, struct frame* fr_temp)
{
    unsigned int               rc = PASS;
    int                        ret;
    const struct framedetails* frame_details = g_UE_frame_details;

    rc = cmux_check_length(rawptr, fr_temp);
    if (rc == PASS) {
        fr_temp->uih_data = (void*)cmux_alloc(frame_details[fr_temp->f_type].frame_length);
        if (fr_temp->uih_data == NULL) {
            return BASIC_FRAME_ERR;
        }
        if (frame_details[fr_temp->f_type].frame_length > 0) {
            ret = memcpy_s(fr_temp->uih_data, frame_details[fr_temp->f_type].frame_length, rawptr,
                                 frame_details[fr_temp->f_type].frame_length);
            CMUX_MEM_CHK_RTN_VAL(ret);
        }
    } else {
        rc = BASIC_FRAME_ERR;
    }

    return rc;
}

/* frame类型解析 */
void mux_frame_parse_frame_type(unsigned char frameCode, struct frame* fr_temp)
{
    const struct framedetails* frame_details = g_UE_frame_details;
    unsigned int               i;

    for (i = DLC_PN_CMD; i < MAX_CMUX_FRAMES; i++) {
        if ((frameCode == frame_details[i].frame_code)) {
            fr_temp->f_type = i;
            /* 收到了NSC消息，停止流控定时器 */
            if (fr_temp->f_type == NSC) {
                cmux_set_g_nsc_received(1);
            }
            break;
        }
    }
}

/* 集中处理case语句的返回值 */
unsigned int mux_frame_check_rc(struct frame* fr_temp, unsigned int rc)
{
    if ((rc == FAIL) || (rc == BASIC_FRAME_ERR) || (rc == INVALID_FRAME_ERR)) {
        if (fr_temp->uih_data != NULL) {
            cmux_free((unsigned char*)fr_temp->uih_data);
            fr_temp->uih_data = NULL;
        }
        return rc;
    } else {
        return FRAME_OK;
    }
}

unsigned int mux_frame_to_struct(unsigned char* rawptr, struct cmuxlib* cm, struct frame* fr_temp)
{
    unsigned int               rc = PASS;

    if (rawptr == NULL || cm == NULL || fr_temp == NULL) {
        return BASIC_FRAME_ERR;
    }

    /* we reset the value of uih-data, as it is this function responsibility to fill it */
    fr_temp->uih_data = NULL;
    g_nsc_frm         = rawptr;

    mux_frame_parse_frame_type(*rawptr, fr_temp);

    ++rawptr;

    switch (fr_temp->f_type) {
        case DLC_PN_CMD:
            rc = mux_frame_parse_dlc_pn_cmd(rawptr, cm, fr_temp);
            break;

        case CLD_CMD:
            rc = cmux_check_length(rawptr, fr_temp);
            break;
            /* juvin:M2M-1669: to send NSC if the length valud field is not correct april2 : end */
        case TEST_CMD:
            rc = mux_frame_parse_test_cmd(rawptr, cm, fr_temp);
            break;
        case PSC_CMD:
        case FCON_CMD:
        case FCOFF_CMD:
            rc = cmux_check_length(rawptr, fr_temp);
            break;
        case SNC_CMD:
            rc = mux_frame_parse_snc_cmd(rawptr, cm, fr_temp);
            break;
        case MSC_CMD:
            rc = mux_frame_parse_msc_cmd(rawptr, cm, fr_temp);
            break;
        case NSC:
            rc = mux_frame_parse_nsc(rawptr, cm, fr_temp);
            break;
            /* 针对Fcon和Fcoff响应，只对正确的响应做处理，错误的则丢弃 */
        case FCON_RESP:
        case FCOFF_RESP:
            rc = cmux_check_length(rawptr, fr_temp);
            if (rc != PASS) {
                rc = BASIC_FRAME_ERR;
            }
            break;
            /* 对下列命令的响应，不做回应，直接丢弃 */
        case DLC_PN_RESP:
        case CLD_RESP:
        case TEST_RESP:
        case PSC_RESP:
        case SNC_RESP:
        case MSC_RESP:
            rc = BASIC_FRAME_ERR;
            break;
        default:
            rc = INVALID_FRAME_ERR;
            break;
    }

    return mux_frame_check_rc(fr_temp, rc);
} /* mux_frame_to_struct */


unsigned short int cmux_basic_frame_length(unsigned char* ptr, unsigned int* offset)
{
    unsigned short int length   = 0;
    unsigned int       l_offset = 0;

    /* 把EA位顶掉，取后7个字节即为长度值 */
    length = (CMUX_LENGTH_MASK & ((*ptr) >> 1));
    /* Check for EA bit if EA is set returns length as one byte otherwise returns it as 2 bytes */
    if (((*ptr) & CMUX_EA_BIT_MASK) == CMUX_EA_BIT_MASK) {
        length   = (((*ptr) >> 1) & CMUX_LENGTH_MASK);
        l_offset = CMUX_ONE_BYTE_HEADER_LENGTH_OFFSET;
    } else {
        l_offset = CMUX_TWO_BYTE_HEADER_LENGTH_OFFSET;

        length = ((*(ptr + 1)) << CMUX_FRAME_LENGTH_BIT_OFFSET) | length;
    }
    /* offset表示的是长度字节结束的那个字节号 */
    *offset = l_offset;
    return length;
} /* cmux_basic_frame_length */


void cmux_fill_length(struct frame* fr_temp, unsigned int* offset, unsigned char** ptr)
{
    unsigned char *rawptr   = NULL;
    unsigned int   l_offset = 0;

    if (*ptr == NULL) {
        return;
    }
    rawptr = *ptr;
    if ((fr_temp->length) < CMUX_FRAME_ONE_BYTE_MAX_LEN) {
        *rawptr  = (((fr_temp->length) << 1) & 0xFE) | CMUX_EA_BIT_MASK;
        l_offset = CMUX_ONE_BYTE_HEADER_LENGTH_OFFSET;
    } else {
        *rawptr = (unsigned char)((fr_temp->length) << CMUX_FRAME_LENGTH_EA_BIT_OFFSET);
        rawptr++;
        *rawptr  = (unsigned char)((fr_temp->length) >> CMUX_FRAME_LENGTH_BIT_OFFSET);
        l_offset = CMUX_TWO_BYTE_HEADER_LENGTH_OFFSET;
    }
    *offset = l_offset;
    *ptr    = rawptr;
} /* cmux_fill_length */

/*
 * Description: The function is used to check whether the elements of the frame are valid
 * Inputs: Takes four parmeters(unsigned char **rawptr, struct frame *fr_temp,
 *                              unsigned short len, unsigned int offset )
 */
unsigned int cmux_check_frame_data_valid(unsigned char** rawptr, struct frame* fr_temp, unsigned short len,
                                         unsigned int offset, unsigned short max_len)
{
    unsigned char  l_fcs;
    unsigned char* rawptr_temp = (unsigned char*)NULL;

    rawptr_temp = *rawptr;

    /* Length is approved ! */
    fr_temp->length = (unsigned short int)len;
    /* if inframe length is with in buf len then check for fcs and f9 */
    if ((len + offset + CMUX_FRAME_END_FLAG_AND_FCS_LEN) > max_len) {
        return BASIC_FRLEN_ERR;
    }
    /* check for FCS */
    if (*(rawptr_temp + 2) != CMUX_DATA_UI) {
        l_fcs = cmux_ts0710_check_fcs(rawptr_temp + 1, ((int)offset - 1), *(rawptr_temp + len + offset));
    } else {
        l_fcs = cmux_ts0710_check_fcs(rawptr_temp + 1, (int)((offset + len) - 1), *(rawptr_temp + len + offset));
    }
    if (l_fcs) {
        return FCS_ERR;
    }
    /* check for End flag */
    if (*(rawptr_temp + len + offset + 1) != 0xF9) {
        return NO_TAIL;
    }
    /* Upt to here rawptr points of SOF - F9, move ahead from here */
    ++rawptr_temp;  /* Now points to Address field (DLC, CR, EA) */
    *rawptr = rawptr_temp;

    fr_temp->dlc = (((*rawptr_temp) >> CMUX_FRAME_DLCI_BIT_OFFSET) & CMUX_DLC_BIT_MASK);

    /* Check for DLC */
    /* Check for EA bit */
    fr_temp->addr_EA_bit = (*rawptr_temp) & CMUX_EA_BIT_MASK;

    if (fr_temp->addr_EA_bit != 0x01) {
        return BASIC_FR_EA_ERR;
    }

    /* check for CR bit */
    fr_temp->add_CR_bit = ((*rawptr_temp) >> 1) & 0x01;

    /* juvin<m2m-1594><C/R bit validation><24-march><Begin> */
    if (fr_temp->add_CR_bit != 0x01) {
        return BASIC_FR_CR_ERR;
    }
    /* juvin<m2m-1594><C/R bit validation><24-march><End> */
    return FRAME_OK;
}
/*
 * Description: This function determine whether the input parameters are valid
 * Inputs: frame structure (struct frame **fr, struct frame **fr_temp)
 */
unsigned int cmux_check_input_data_valid(struct frame** fr, struct frame** fr_temp)
{
    if (!fr) {
        return BASIC_FRAME_ERR;
    }
    *fr_temp = *fr;
    if (*fr_temp == NULL) {
        return NO_HEADER;
    }
    return FRAME_OK;
}

unsigned int cmux_is_dlc_control_frame_type(CMUX_FrTypeUint8 f_type)
{
    if ((f_type == CTRL_SABM) || (f_type == CTRL_UA) || (f_type == CTRL_DM) || (f_type == CTRL_DISC)) {
        return PASS;
    }
    return FAIL;
}

unsigned int cmux_fill_uih_data_to_struct(unsigned char* rawptr, struct cmuxlib* cm, struct frame* fr_temp)
{
    unsigned int rc  = FRAME_OK;
    int          ret;

    if (cmux_is_dlc_control_frame_type(fr_temp->f_type)) {
        if (fr_temp->pf_bit == 0x00) {
            rc = BASIC_FR_EA_ERR;
        }
        return rc;
    } else if ((fr_temp->f_type == CTRL_UIH) || (fr_temp->f_type == UIH_DATA)) {
        /* UIH帧携带Info长度是0，直接丢弃(含UI帧) */
        if (fr_temp->length == 0) {
            rc = BASIC_FRAME_ERR;
            return rc;
        }

        if (fr_temp->dlc == 0) {
            /* 携带控制消息的UI帧，返回NSC命令 */
            if (fr_temp->UI_bit == 1) {
                rc = INVALID_FRAME_ERR;
                return rc;
            }

            rc = mux_frame_to_struct(rawptr, cm, fr_temp);
            return rc;
        } else {  /* UIH_DATA */
            fr_temp->f_type = UIH_DATA;
            /* 携带数据的UI帧，直接丢弃不做处理 */
            if (fr_temp->UI_bit == 1) {
                rc = BASIC_FRAME_ERR;
                return rc;
            }

            fr_temp->uih_data = (void*)cmux_alloc(fr_temp->length);
            if (fr_temp->uih_data == NULL) {
                rc = FAIL;
                return rc;
            }
            if (fr_temp->length > 0) {
                ret = memcpy_s(fr_temp->uih_data, fr_temp->length, rawptr, fr_temp->length);
                CMUX_MEM_CHK_RTN_VAL(ret);
            }

            rawptr = fr_temp->length + rawptr;
            return rc;
        }
    } else {
        rc = INVALID_FRAME_ERR;
        return rc;
    }
}

unsigned int cmux_fill_frame_types_to_struct(unsigned char* rawptr, struct frame* fr_temp)
{
    unsigned int               i             = 0;
    unsigned int               rc            = FRAME_OK;
    const struct framedetails* frame_details = g_UE_frame_details;

    fr_temp->f_type = NSC;  /* to make the default one as nsc */
    for (i = 0; i < MAX_CMUX_FRAMES; i++) {
        if (*rawptr == CMUX_DATA_UI) {
            fr_temp->f_type = UIH_DATA;
            fr_temp->UI_bit = 1;
            break;
        } else {
            fr_temp->UI_bit = 0;
        }

        if (((*rawptr) & CMUX_PF_BIT_MASK) == (frame_details[i].frame_code & CMUX_PF_BIT_MASK)) {
            /* Extract P, F bit */
            fr_temp->pf_bit = ((*rawptr) & (~CMUX_PF_BIT_MASK)) >> CMUX_FRAME_PF_BIT_OFFSET;
            if (cmux_is_dlc_control_frame_type(i) == PASS) {
                if (fr_temp->pf_bit == 0x00) {
                    rc = BASIC_FR_EA_ERR;
                    return rc;
                }
            }
        }

        if ((*rawptr) == (frame_details[i].frame_code)) {
            fr_temp->f_type = i;
            /* Extract P, F bit */
            fr_temp->pf_bit = ((*rawptr) & (~CMUX_PF_BIT_MASK)) >> CMUX_FRAME_PF_BIT_OFFSET;
            break;
        }
        /* to consider the FF frame also */
        if ((*rawptr) == 0XFF) {  /* In reception Dont-care the PF bit */
            fr_temp->f_type = UIH_DATA;
            /* Extract P, F bit */
            fr_temp->pf_bit = ((*rawptr) & (~CMUX_PF_BIT_MASK)) >> CMUX_FRAME_PF_BIT_OFFSET;
            break;
        }
    }

    if (i == MAX_CMUX_FRAMES) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "there is an unrecoginized command %d\n", *rawptr);
        rc = BASIC_FRAME_ERR;
    }
    return rc;
}
/*
 * Description: This function determine fill_data_to_struct
 * Inputs: frame structure (struct frame *fr_temp,unsigned char *f_ptr , unsigned short raw_len)
 */
unsigned int cmux_fill_raw_data_to_struct(struct frame* fr_temp, unsigned char* f_ptr, unsigned short raw_len)
{
    int  ret;
    fr_temp->raw_len  = raw_len;
    fr_temp->raw_data = cmux_alloc(fr_temp->raw_len);
    if (fr_temp->raw_data == NULL) {
        return FAIL;
    }
    if (f_ptr != NULL && fr_temp->raw_len > 0) {  /* : lint fix */
        ret = memcpy_s(fr_temp->raw_data, fr_temp->raw_len, f_ptr, fr_temp->raw_len);
        CMUX_MEM_CHK_RTN_VAL(ret);
    }
    return PASS;
}

unsigned int cmux_frame_to_struct(struct frame** fr, struct cmuxlib* cm, unsigned char* rawptr, unsigned short raw_len)
{
    unsigned char*     f_ptr   = (unsigned char*)NULL;
    struct frame*      fr_temp = (struct frame*)NULL;
    unsigned int       rc      = 0;
    unsigned int       offset  = 0;
    unsigned short     len     = 0;
    unsigned short     max_len = 0;

    /* unsigned char *data; */
    do {
        rc = cmux_check_input_data_valid(fr, &fr_temp);
        if (rc != FRAME_OK) {
            return rc;
        }

        f_ptr     = rawptr;  /* back up the raw pointer */
        g_nsc_frm = rawptr + 2;
        /* check for starting flag 0xF9 */
        if ((*rawptr) != 0xF9) {
            rc = NO_HEADER;
            break;
        }

        /* check for length,FCS, End flag, DLC, EA bit, (rawptr + 3) point to length field */
        len     = cmux_basic_frame_length(rawptr + CMUX_FRAME_LENGTH_FIELD_OFFSET, &offset);
        max_len = (unsigned short)((MAX_CMUX_FRAME_SIZE(cm) < raw_len ? MAX_CMUX_FRAME_SIZE(cm) : raw_len));
        rc      = cmux_check_frame_data_valid(&rawptr, fr_temp, len, offset, max_len);
        if (rc != FRAME_OK) {
            break;
        }

        /* Extract frame types and accordingly switch */
        ++rawptr;  /* Now points to Control field (SABM,UIH..) */
        rc = cmux_fill_frame_types_to_struct(rawptr, fr_temp);
        if (rc != FRAME_OK) {
            return rc;
        }

        /* Length check has passed, if we are here, so decided based on EA of length field */
        if (offset == CMUX_ONE_BYTE_HEADER_LENGTH_OFFSET) {
            rawptr += CMUX_FRAME_DEFAULT_TL_LEN;
        } else {
            rawptr += CMUX_FRAME_PLUS_TL_LEN;
        }
        /* PARKED the rawptr to DATA or MUX-frame */
        rc = cmux_fill_uih_data_to_struct(rawptr, cm, fr_temp);

    } while (0);

    if ((rc == FRAME_OK) && (fr_temp)) {  /* : lint fix */
        if (cmux_fill_raw_data_to_struct(fr_temp, f_ptr, raw_len) == FAIL) {
            return FAIL;
        }
    }
    return rc;
} /* cmux_frame_to_struct */


unsigned int mux_struct_to_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
    unsigned char *ptr, unsigned int templen)
{
    unsigned char*             rawptr        = (unsigned char*)NULL;
    unsigned int               len_val       = 0;
    const struct framedetails* frame_details = g_UE_frame_details;

    if (fr_temp == NULL || vrawptr == NULL || *vrawptr == NULL) {
        return BASIC_FRAME_ERR;
    }

    if (fr_temp->dlc == 0) {
        rawptr  = *vrawptr;
        *rawptr = frame_details[CTRL_UIH].frame_code;
        ++rawptr;

        /* Fill the raw pointer with Length field */
        cmux_fill_length(fr_temp, offset, &rawptr);
        ++rawptr;

        /* Fill the frame code */
        *rawptr = frame_details[fr_temp->f_type].frame_code;
        ++rawptr;

        /* Check for EA bit and fill the length field to the rawptr */
        if ((fr_temp->f_type == TEST_CMD) || (fr_temp->f_type == TEST_RESP)) {
            if ((fr_temp->length) <= TEST_LEN_LIMIT) {
                len_val = (fr_temp->length - CMUX_FRAME_DEFAULT_TL_LEN);
                *rawptr = (((len_val) << CMUX_FRAME_LENGTH_EA_BIT_OFFSET) & 0xFE) | CMUX_EA_BIT_MASK;
            } else {
                len_val = (fr_temp->length - CMUX_FRAME_PLUS_TL_LEN);
                *rawptr = (unsigned char)((len_val) << CMUX_FRAME_LENGTH_EA_BIT_OFFSET);
                rawptr++;
                *rawptr = (unsigned char)((len_val) >> CMUX_FRAME_LENGTH_BIT_OFFSET);
            }
        } else {
            *rawptr = (((frame_details[fr_temp->f_type].frame_length) << CMUX_FRAME_LENGTH_EA_BIT_OFFSET) & 0xFE) |
                        CMUX_EA_BIT_MASK;
        }
        ++rawptr;
        *vrawptr = rawptr;
    }
    return 0;
} /* mux_struct_to_frame */


unsigned int mux_nsc_struct_to_frame(struct frame* fr_temp, unsigned char** vrawptr)
{
    unsigned char *rawptr = NULL;

    if (fr_temp == NULL || vrawptr == NULL || *vrawptr == NULL) {
        return BASIC_FRAME_ERR;
    }

    rawptr = *vrawptr;

    if (fr_temp->length > 0) {
        /* juvin<m2m-1594,1593,1580><proper NSC response format><24-march><Begin> */
        *rawptr = *g_nsc_frm;
        /* juvin<m2m-1594,1593,1580><proper NSC response format><24-march><End> */
        rawptr = rawptr + 1;
    }

    *vrawptr = rawptr;
    return 0;
} /* mux_nsc_struct_to_frame */


unsigned int cmux_fill_header_struct_to_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
                                              unsigned char *ptr, unsigned int templen)
{
    unsigned char             *rawptr = NULL;
    const struct framedetails *frame_details = g_UE_frame_details;

    if ((fr_temp == NULL) || (vrawptr == NULL)) {
        return BASIC_FRAME_ERR;
    }

    rawptr = *vrawptr;

    /* Fill the raw pointer with control Frame code */
    *rawptr = frame_details[fr_temp->f_type].frame_code;
    ++rawptr;

    /* Fill the raw pointer with Length field one or two bytes */
    cmux_fill_length(fr_temp, offset, &rawptr);

    ++rawptr;
    *vrawptr = rawptr;
    return 0;
} /* cmux_fill_header_struct_to_frame */


unsigned int mux_dlcpn_struct_to_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
                                              unsigned char *ptr, unsigned int templen)
{
    struct dlc_param_neg *dlcpn  = NULL;
    unsigned char        *rawptr = NULL;

    const struct framedetails* frame_details = g_UE_frame_details;
    if ((fr_temp == NULL) || (fr_temp->dlc != 0) || (vrawptr == NULL) || (*vrawptr == NULL)) {
        return BASIC_FRAME_ERR;
    }

    rawptr = *vrawptr;

    *rawptr = frame_details[CTRL_UIH].frame_code;
    ++rawptr;

    /* Fill the raw pointer with Length field */
    cmux_fill_length(fr_temp, offset, &rawptr);
    ++rawptr;

    /* Fill the Mux frame code */
    *rawptr = frame_details[fr_temp->f_type].frame_code;
    ++rawptr;

    /* Check for EA bit and fill the length field to the rawptr */
    *rawptr = (((frame_details[fr_temp->f_type].frame_length) << 1) & 0xFE) | 0x01;
    ++rawptr;

    /* Fill the rawptr with DLC param nego parameters */
    if (fr_temp->uih_data != NULL) {
        dlcpn = (struct dlc_param_neg*)fr_temp->uih_data;
        /* Fill DLC */
        *rawptr = (dlcpn->dlci) & CMUX_DLC_BIT_MASK;
        ++rawptr;
        /* Fill convergence */
        *rawptr = (dlcpn->conv_layer << CMUX_PN_CONV_LAYER_TYPE_OFFSET) & 0xF0;
        ++rawptr;
        /* Fill priority */
        *rawptr = (dlcpn->priority) & CMUX_PRIORITY_BIT_MASK;
        ++rawptr;
        /* Fill ack_timer to raw ptr */
        *rawptr = (unsigned char)dlcpn->muxparams.ack_timer_t1;
        ++rawptr;
        /* Fill frame size */
        *rawptr = (dlcpn->muxparams.frame_size_n1 & 0xFF);
        ++rawptr;
        *rawptr = (*rawptr) | (unsigned char)(dlcpn->muxparams.frame_size_n1 >> 8);
        ++rawptr;
        /* Fill retransmit */
        *rawptr = (unsigned char)dlcpn->muxparams.retransmit_n2;
        ++rawptr;
        /* Fill window_k to rawptr */
        *rawptr = (dlcpn->muxparams.window_k) & CMUX_WINDOW_BIT_MASK;
    }
    ++rawptr;
    *vrawptr = rawptr;
    return 0;
} /* mux_dlcpn_struct_to_frame */

unsigned int cmux_struct_to_msc_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
                                              unsigned char *ptr, unsigned int templen)
{
    struct modem_status_cmd *msc    = NULL;
    unsigned char           *rawptr = (unsigned char*)NULL;

    rawptr = *vrawptr;

    mux_struct_to_frame(fr_temp, &rawptr, offset, ptr, templen);
    if (fr_temp->dlc == 0) {
        if (fr_temp->uih_data != NULL) {
            msc     = (struct modem_status_cmd*)fr_temp->uih_data;
            *rawptr = ((msc->dlci << CMUX_FRAME_DLCI_BIT_OFFSET)& 0xFC) | 0x03;
            rawptr++;
            *rawptr = (msc->EA) | (msc->fc << CMUX_MSC_FRMAE_FC_OFFSET) |
                      (msc->rtc << CMUX_MSC_FRMAE_RTC_OFFSET) | (msc->rtr << CMUX_MSC_FRMAE_RTR_OFFSET) |
                      (msc->ic << CMUX_MSC_FRMAE_IC_OFFSET) | (msc->dv << CMUX_MSC_FRMAE_DV_OFFSET);
        }
        rawptr++;
    }
    *vrawptr = rawptr;
    return 0;
}

unsigned int cmux_struct_to_test_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
    unsigned char *ptr, unsigned int templen)
{
    unsigned char* rawptr  = (unsigned char*)NULL;
    int            ret;

    if (fr_temp->dlc != 0) {
        return 0;
    }

    templen -= *vrawptr - ptr;
    rawptr  = *vrawptr;
    mux_struct_to_frame(fr_temp, &rawptr, offset, ptr, templen);

    if (fr_temp->uih_data != NULL) {
        if ((fr_temp->length) <= TEST_LEN_LIMIT) {
            if ((fr_temp->length > CMUX_FRAME_DEFAULT_TL_LEN) && (templen >= fr_temp->length)) {
                ret = memcpy_s(rawptr, templen, fr_temp->uih_data, (fr_temp->length - CMUX_FRAME_DEFAULT_TL_LEN));
                CMUX_MEM_CHK_RTN_VAL(ret);
                rawptr = rawptr + (fr_temp->length - CMUX_FRAME_DEFAULT_TL_LEN);
            }
        } else {
            if (templen >= fr_temp->length) {
                ret = memcpy_s(rawptr, templen, fr_temp->uih_data, (fr_temp->length - CMUX_FRAME_PLUS_TL_LEN));
                CMUX_MEM_CHK_RTN_VAL(ret);
                rawptr = rawptr + (fr_temp->length - CMUX_FRAME_PLUS_TL_LEN);
            }
        }
    } else {
        rawptr = rawptr + (fr_temp->length - CMUX_FRAME_DEFAULT_TL_LEN);
    }

    *vrawptr = rawptr;
    return 0;
}

unsigned int cmux_struct_to_nsc_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
                                              unsigned char *ptr, unsigned int templen)
{
    unsigned char* rawptr = (unsigned char*)NULL;
    rawptr                = *vrawptr;

    mux_struct_to_frame(fr_temp, &rawptr, offset, ptr, templen);
    if (fr_temp->dlc == 0) {
        mux_nsc_struct_to_frame(fr_temp, &rawptr);
    }
    *vrawptr = rawptr;
    return 0;
}

unsigned int cmux_struct_to_uih_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
    unsigned char *ptr, unsigned int templen)
{
    unsigned char* rawptr = (unsigned char*)NULL;
    int         ret;
    rawptr   = *vrawptr;
    templen -= *vrawptr - ptr;

    /* Fill the UIH_DATA  code */
    cmux_fill_header_struct_to_frame(fr_temp, &rawptr, offset, ptr, templen);

    if (fr_temp->length > 0 && templen >= fr_temp->length) {
        ret = memcpy_s(rawptr, templen, fr_temp->uih_data, fr_temp->length);
        CMUX_MEM_CHK_RTN_VAL(ret);
    }
    rawptr   = rawptr + fr_temp->length;
    *vrawptr = rawptr;
    return 0;
}

unsigned int cmux_struct_to_snc_frame(struct frame* fr_temp, unsigned char** vrawptr, unsigned int* offset,
    unsigned char *ptr, unsigned int templen)
{
    struct snc_cmd *snc    = NULL;
    unsigned char  *rawptr = (unsigned char*)NULL;
    rawptr                 = *vrawptr;

    mux_struct_to_frame(fr_temp, &rawptr, offset, ptr, templen);
    if (fr_temp->dlc == 0) {
        if (fr_temp->uih_data != NULL) {
            snc     = (struct snc_cmd*)fr_temp->uih_data;
            *rawptr = ((snc->dlci << CMUX_FRAME_DLCI_BIT_OFFSET) & 0xFC) | 0x03;
            rawptr++;
            *rawptr = (snc->EA) | (snc->s1 << CMUX_SNC_FRMAE_S1_OFFSET) |
                (snc->s2 << CMUX_SNC_FRMAE_S2_OFFSET) | (snc->s3 << CMUX_SNC_FRMAE_S3_OFFSET) |
                (snc->s4 << CMUX_SNC_FRMAE_S4_OFFSET);
            rawptr++;
            *rawptr |= 0x01;
        }
        rawptr++;
    }
    *vrawptr = rawptr;
    return 0;
}
/*
 * Description: This function determine whether the input parameters are valid
 * Inputs: frame structure (struct frame **fr, void *cmx)
 */
unsigned int cmux_check_struct_data_valid(struct frame** fr, struct cmuxlib* cmx)
{
    if (fr == NULL || *fr == NULL || cmx == NULL) {
        return FAIL;
    }
    return PASS;
}
/*
 * Description: This function is check struct to frame is already done
 * Inputs: frame structure (struct frame **)
 */
unsigned int cmux_struct_to_frame_is_done(struct frame* fr_temp)
{
    if (fr_temp->raw_data != NULL && fr_temp->raw_len != 0) {
        return PASS;
    }
    return FAIL;
}

unsigned int cmux_struct_to_frame(struct frame** fr, struct cmuxlib* cmx)
{
    unsigned char     *vraw    = NULL;
    unsigned char     *f_ptr   = NULL;
    unsigned char     *ptr     = NULL;
    unsigned short int len     = 0;
    unsigned int       offset  = 0;
    unsigned int       templen = 0;
    unsigned int       msgCnt  = 0;
    unsigned int       i       = 0;
    struct frame      *fr_temp = (struct frame*)NULL;
    struct cmuxlib    *cm      = NULL;

    if (cmux_check_struct_data_valid(fr, cmx) == FAIL) {
        return BASIC_FRAME_ERR;
    }

    cm      = (struct cmuxlib*)cmx;
    fr_temp = *fr;

    /* conversion already done */
    if (cmux_struct_to_frame_is_done(fr_temp) == PASS) {
        return PASS;
    }

    templen = (MAX_CMUX_FRAME_SIZE(cm));
    vraw    = (unsigned char*)cmux_alloc((int)templen);
    if (vraw == NULL) {
        return BASIC_FRAME_ERR;
    }

    ptr = vraw;

    /* Fill the start flag to the local pointer */
    *vraw = CMUX_FRAME_FLAG;
    ++vraw;
    f_ptr = vraw;  /* used for fcs */

    *vraw = (((fr_temp->dlc) << CMUX_FRAME_DLCI_BIT_OFFSET) & 0xFC) |
        (0x03 & ((fr_temp->addr_EA_bit & 0x01) | ((fr_temp->add_CR_bit) << 1)));
    ++vraw;

    msgCnt = sizeof(g_structToFrameProcessTbl) / sizeof(structToFrameProcess);

    for (i = 0; i < msgCnt; i++) {
        if (fr_temp->f_type == g_structToFrameProcessTbl[i].frType) {
            if (g_structToFrameProcessTbl[i].func != NULL) {
                g_structToFrameProcessTbl[i].func(fr_temp, &vraw, &offset, ptr, templen);
                break;
            }
        }
    }

    /* fr_temp->f_type check fail，offset illegal */
    if (offset == 0) {
        cmux_free(ptr);
        return FAIL;
    }

    /* Calculate FCS */
    *vraw = cmux_ts0710_compute_crc((f_ptr), ((int)offset - 1));
    ++vraw;
    /* Fill the rawptr with End Flag */
    *vraw = CMUX_FRAME_FLAG;
    /* Copy the vraw pointer to the void raw_data pointer in the frame structure which will be to the serial line for */
    /* transmission */
    len = (unsigned short int)(vraw - ptr);

    fr_temp->raw_data = ptr;
    fr_temp->raw_len  = len + 1;

    return PASS;
} /* cmux_struct_to_frame */


int default_frame_data_check(DLCINFO dlc_info, unsigned char* data)
{
    if (dlc_info.ftype == MSC_CMD || dlc_info.ftype == MSC_RESP || dlc_info.ftype == SNC_CMD ||
        dlc_info.ftype == SNC_RESP || dlc_info.ftype == DLC_PN_CMD || dlc_info.ftype == DLC_PN_RESP) {
        if (data == NULL) {
            return FAIL;
        }
    }
    return PASS;
}

int default_frame_datalen_check(DLCINFO dlc_info, unsigned char* data, unsigned int data_len)
{
    if (dlc_info.ftype == UIH_DATA || dlc_info.ftype == TEST_CMD || dlc_info.ftype == TEST_RESP) {
        if (data == NULL || data_len == 0) {
            return FAIL;
        }
    }
    return default_frame_data_check(dlc_info, data);
}

int default_frame_sabm_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                 unsigned int data_len)
{
    lfr->length      = g_UE_frame_details[dlc_info.ftype].frame_length;
    lfr->f_type      = dlc_info.ftype;
    lfr->dlc         = dlc_info.dlc;
    lfr->addr_EA_bit = 1;
    lfr->add_CR_bit  = 1;
    lfr->pf_bit      = 1;

    return PASS;
}

int default_frame_msc_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                unsigned int data_len)
{
    struct modem_status_cmd *mscgui, *msc = (struct modem_status_cmd*)NULL;
    mscgui = (struct modem_status_cmd*)data;

    msc = (struct modem_status_cmd*)cmux_alloc(sizeof(struct modem_status_cmd));
    if (msc == NULL) {
        return FAIL;
    }
    lfr->dlc         = 0;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    lfr->f_type = dlc_info.ftype;
    lfr->pf_bit = 0;
    lfr->length = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;
    if (dlc_info.ftype == MSC_CMD) {
        msc->dlci = dlc_info.dlc;
    } else {
        msc->dlci = mscgui->dlci;
    }
    msc->dv       = mscgui->dv;
    msc->ic       = mscgui->ic;
    msc->EA       = 1;
    msc->fc       = mscgui->fc;
    msc->rtc      = mscgui->rtc;
    msc->rtr      = mscgui->rtr;
    lfr->uih_data = (void*)msc;

    return PASS;
}

int default_frame_snc_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                unsigned int data_len)
{
    struct snc_cmd *sncgui, *snc = (struct snc_cmd*)NULL;
    sncgui = (struct snc_cmd*)data;

    snc = (struct snc_cmd*)cmux_alloc(sizeof(struct snc_cmd));
    if (snc == NULL) {
        return FAIL;
    }
    lfr->dlc         = 0;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    lfr->f_type = dlc_info.ftype;
    lfr->pf_bit = 1;
    lfr->length = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;
    if (dlc_info.ftype == SNC_CMD) {
        snc->dlci = dlc_info.dlc;
    } else {
        snc->dlci = sncgui->dlci;
    }
    snc->EA       = 1;
    snc->s1       = sncgui->s1;
    snc->s2       = sncgui->s2;
    snc->s3       = sncgui->s3;
    snc->s4       = sncgui->s4;
    snc->s5       = sncgui->s5;
    snc->s6       = sncgui->s6;
    snc->s7       = sncgui->s7;
    lfr->uih_data = (void*)snc;

    return PASS;
}

int default_frame_dlc_pn_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                   unsigned int data_len)
{
    struct dlc_param_neg *dlcpnrxed, *dlcpn = (struct dlc_param_neg*)NULL;

    dlcpnrxed = (struct dlc_param_neg*)data;

    dlcpn = (struct dlc_param_neg*)cmux_alloc(sizeof(struct dlc_param_neg));
    if (dlcpn == NULL) {
        return FAIL;
    }
    lfr->dlc         = 0;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    lfr->f_type       = dlc_info.ftype;
    lfr->pf_bit       = 1;
    lfr->length       = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;
    dlcpn->priority   = DEFAULT_PRIORITY;
    dlcpn->conv_layer = CONVERGENCE_LAYER;

    dlcpn->dlci                    = dlcpnrxed->dlci;
    dlcpn->muxparams.ack_timer_t1  = dlcpnrxed->muxparams.ack_timer_t1;
    dlcpn->muxparams.frame_size_n1 = dlcpnrxed->muxparams.frame_size_n1;
    dlcpn->muxparams.retransmit_n2 = dlcpnrxed->muxparams.retransmit_n2;
    dlcpn->muxparams.window_k      = dlcpnrxed->muxparams.window_k;
    dlcpn->muxparams.res_timer_t2  = dlcpnrxed->muxparams.res_timer_t2;

    lfr->uih_data = (void*)dlcpn;

    return PASS;
}

int default_frame_cld_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                unsigned int data_len)
{
    lfr->dlc         = 0;
    lfr->f_type      = dlc_info.ftype;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    lfr->pf_bit = 1;
    lfr->length = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;

    return PASS;
}

int default_frame_test_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                 unsigned int data_len)
{
    int ret;
    lfr->dlc         = 0;
    lfr->f_type      = dlc_info.ftype;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    lfr->pf_bit = 1;
    if (data_len <= (TEST_LEN_LIMIT - 2)) { /*lint !e574*/
        lfr->length = (unsigned short)data_len + CMUX_FRAME_DEFAULT_TL_LEN;
    } else {
        lfr->length = (unsigned short)data_len + CMUX_FRAME_PLUS_TL_LEN;
    }
    lfr->uih_data = (void*)cmux_alloc((int)data_len);
    if (lfr->uih_data == NULL) {
        return FAIL;
    }
    if (data_len > 0) {
        ret = memcpy_s(lfr->uih_data, data_len, data, data_len);
        CMUX_MEM_CHK_RTN_VAL(ret);
    }
    return PASS;
}

int default_frame_psc_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                unsigned int data_len)
{
    lfr->dlc         = 0;
    lfr->f_type      = dlc_info.ftype;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->pf_bit = 1;
    } else {
        lfr->pf_bit = 0;
    }
    lfr->length = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;
    return PASS;
}

int default_frame_nsc_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                unsigned int data_len)
{
    struct non_supported_cmd_resp* nsc = (struct non_supported_cmd_resp*)NULL;

    lfr->dlc         = 0;
    lfr->f_type      = dlc_info.ftype;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    lfr->pf_bit = 1;
    lfr->length = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;
    nsc         = (struct non_supported_cmd_resp*)cmux_alloc(sizeof(struct non_supported_cmd_resp));
    if (nsc == NULL) {
        return FAIL;
    }
    lfr->uih_data = (void*)nsc;
    return PASS;
}

int default_frame_fc_resp_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                    unsigned int data_len)
{
    lfr->dlc         = 0;
    lfr->f_type      = dlc_info.ftype;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    lfr->pf_bit = 1;
    lfr->length = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;
    return PASS;
}

int default_frame_fc_cmd_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                   unsigned int data_len)
{
    lfr->dlc         = 0;
    lfr->f_type      = dlc_info.ftype;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }
    /* shiyanan modified for FCON/FCOFF re-sending */
    lfr->pf_bit = 1; /*lint !e539 !e830*/
    lfr->length = g_UE_frame_details[dlc_info.ftype].frame_length + CMUX_FRAME_DEFAULT_TL_LEN;
    return PASS;
}

int default_frame_uih_fill_para(struct cmuxlib* cm, struct dlcinfo dlc_info, struct frame* lfr, unsigned char* data,
                                unsigned int data_len)
{
    int ret;
    lfr->dlc         = dlc_info.dlc;
    lfr->f_type      = dlc_info.ftype;
    lfr->addr_EA_bit = 1;
    if (cm->cmux_status & CMUX_TE_MODE) {
        lfr->add_CR_bit = 1;
    } else {
        lfr->add_CR_bit = 0;
    }

    lfr->uih_data = (void*)cmux_alloc((int)data_len);
    if (lfr->uih_data == NULL) {
        return FAIL;
    }
    if (data_len > 0) {
        ret = memcpy_s(lfr->uih_data, data_len, data, data_len);
        CMUX_MEM_CHK_RTN_VAL(ret);
    }
    lfr->length = (unsigned short)data_len;
    return PASS;
}

int cmux_default_frame_para_check(DLCINFO dlc_info, struct frame** fr)
{
    if (fr == NULL || dlc_info.ftype > MAX_CMUX_FRAMES) {
        return FAIL;
    }
    if (dlc_info.dlc > MAX_DLC_TGT) {
        if (dlc_info.ftype != CTRL_DM) {
            return FAIL;
        }
    }
    /* Enters with assumption frame is allocated */
    return PASS;
}

int cmux_get_default_cmux_frame(struct cmuxlib* cm, DLCINFO dlc_info, struct frame** fr, unsigned char* data,
                                unsigned int data_len)
{
    struct frame* lfr    = (struct frame*)NULL;
    int           rc     = FAIL;
    unsigned int  i      = 0;
    unsigned int  msgCnt;

    if (cmux_default_frame_para_check(dlc_info, fr) == FAIL ||
        default_frame_datalen_check(dlc_info, data, data_len) == FAIL) {
        return FAIL;
    }

    lfr = (struct frame*)cmux_alloc(sizeof(struct frame));

    if (lfr == NULL) {
        return FAIL;  /* malloc failure */
    }
    (void)memset_s(lfr, sizeof(struct frame), 0, sizeof(struct frame));

    msgCnt = sizeof(g_defaultFrameProcessTbl) / sizeof(defaultframeprocess);

    for (i = 0; i < msgCnt; i++) {
        if (dlc_info.ftype == g_defaultFrameProcessTbl[i].frType) {
            if (g_defaultFrameProcessTbl[i].func != NULL) {
                rc = g_defaultFrameProcessTbl[i].func(cm, dlc_info, lfr, data, data_len);
                break;
            }
        }
    }

    if (rc == FAIL) {
        cmux_free((unsigned char*)lfr);
        lfr = NULL;
    } else {
        *fr = lfr;
    }
    return rc;
} /* cmux_get_default_cmux_frame */
unsigned int cmux_check_proper_frame_fcs( struct frame_result* result, unsigned char* rawptr,
    unsigned int len, unsigned int offset)
{
    unsigned int l_fcs;
    if ((*(rawptr + result->start_offset + 2)) != CMUX_DATA_UI) {
        l_fcs = cmux_ts0710_check_fcs(rawptr + result->start_offset + 1, (int)offset - 1,
                                  *(rawptr + result->start_offset + len + offset));
    } else {
        l_fcs = cmux_ts0710_check_fcs(rawptr + result->start_offset + 1, (int)((offset + len) - 1),
                                      *(rawptr + result->start_offset + len + offset));
    }

    if (l_fcs) {
        result->fcs = 0;
        result->err = 1;
        CMUX_PRINT(CMUX_DEBUG_INFO, "fcs wrong!\r\n");
        return FAIL;
    }
    result->fcs = 1;
    return PASS;
}
unsigned int cmux_frame_find_start_flag(unsigned char* rawptr, unsigned short rawlen)
{
    unsigned int i;
    for (i = 0; i < rawlen; i++) {
        if (*(rawptr + i) != CMUX_FRAME_FLAG) {
            continue;
        } else {
            break;
        }
    }
    return i;
}

int cmux_check_proper_frame(unsigned char* rawptr, struct cmuxlib* cm, unsigned short rawlen,
    struct frame_result* fr_result)
{
    unsigned int         len, i, offset, l_fcs;
    struct frame_result* result;

    result = (struct frame_result*)fr_result;

    (void)memset_s((void*)result, sizeof(struct frame_result), 0, sizeof(struct frame_result));

    i = cmux_frame_find_start_flag(rawptr, rawlen);

    result->start_offset = (unsigned short)i;

    /* Surely atleast one byte is there..so check start flag */
    /* 1. Check start flag */
    if ((i >= rawlen) || (*(rawptr + i) != CMUX_FRAME_FLAG)) {
        result->start_flag = 0;
        result->err        = 1;
        CMUX_PRINT(CMUX_DEBUG_INFO, "start_flag wrong!\r\n");
        return -1;
    } else {
        result->start_flag = 1;
        result->err        = 0;
    }

    /* juvin:M2M-1669: check  ea bit of dlc before proceeding to data:April 2 :begin */
    if (!(rawlen - i < 2)) {
        if (((*(rawptr + result->start_offset + 1) & CMUX_EA_BIT_MASK) != CMUX_EA_BIT_MASK)) {
            result->err = 1;
            CMUX_PRINT(CMUX_DEBUG_INFO, "address wrong!\r\n");
            return -1;
        }
    }
    /* juvin: M2M-1669:check  ea bit of dlc before proceeding to data:April 2 :begin */
    /* Now check if we have enough bytes for header width to even read.. */
    if (rawlen - i < CMUX_ONE_BYTE_HEADER_LENGTH_OFFSET) {
        return 0;
    }
    /* (rawptr + result->start_offset + 3)是长度域 length, 判断length的EA flag */
    if (!((*(rawptr + result->start_offset + CMUX_FRAME_LENGTH_FIELD_OFFSET) & CMUX_EA_BIT_MASK) == CMUX_EA_BIT_MASK)) {
        if (rawlen - i < CMUX_TWO_BYTE_HEADER_LENGTH_OFFSET) {
            return 0;
        }
    }
    /* 2. Check Length */
    /* Check for length offset=4 for 1 byte length and 5 for 2 byte length */
    len = cmux_basic_frame_length(rawptr + result->start_offset + CMUX_FRAME_LENGTH_FIELD_OFFSET, &offset);

    /* offset = 4/5,MAX_CMUX_FRAME_SIZE(cm) - (offset + 2) 获取information field 长度 */
    if (len > (MAX_CMUX_FRAME_SIZE(cm) - (offset + CMUX_FRAME_END_FLAG_AND_FCS_LEN))) {
        result->length_flag = 0;
        result->err         = 1;
        CMUX_PRINT(CMUX_DEBUG_INFO, "length wrong!\r\n");
        return -1;
    }

    /* if inframe length is with in buf len then check for fcs and f9 */
    /* offset is 4 or 5 (according to EA bit, len-field), 2 is for FCS,f9 */
    /* (len + offset + 2)是一个完整帧长度 */
    if ((len + offset + 2) <= rawlen) {
        result->length_flag = 1;

        /* (rawptr + result->start_offset + 2)是contral域 */
        l_fcs = cmux_check_proper_frame_fcs(result, rawptr, len, offset);
        if (l_fcs == FAIL) {
            return -1;
        }

        /* Move to End flag */
        rawptr = rawptr + result->start_offset + len + offset + 1;

        /* check for End flag */
        if (*(rawptr) != CMUX_FRAME_FLAG) {
            result->end_flag = 0;
            result->err      = 1;
            CMUX_PRINT(CMUX_DEBUG_INFO, "end_flag wrong!\r\n");
            return -1;
        }
        result->end_flag   = 1;
        result->end_offset = (unsigned short)(result->start_offset + len + offset + 1);
        return PASS;
    }
    return PASS;
} /* cmux_check_proper_frame */



int cmux_check_proper_frame_phase1(struct cmuxlib *cm, unsigned char **temp_pending_frame,
    unsigned int *k, unsigned int len)
{
    int rc;

    /* juvin<M2m 1679><to check for consequtivee F9> <Begin> */
    for(*k = 0; *k < len; (*k)++) {
        if(*((cm->cmux_if.pending_frame) + *k) == CMUX_FRAME_FLAG) {
            if(*((cm->cmux_if.pending_frame) + *k + 1) == CMUX_FRAME_FLAG) {
                continue;
            }
            break;
        }
    }
    *temp_pending_frame = cm->cmux_if.pending_frame + *k;
    /* juvin<M2m 1679><to check for consequtivee F9> <END> */
    rc = cmux_check_proper_frame (*temp_pending_frame, cm, (unsigned short)(len - *k), &(cm->cmux_if.result));
    return rc;
}

int cmux_input_frame_is_good_frame(struct cmuxlib *cm)
{
    if (cm->cmux_if.result.start_flag == 1 &&
        cm->cmux_if.result.length_flag == 1 &&
        cm->cmux_if.result.fcs == 1 &&
        cm->cmux_if.result.end_flag == 1) {
        return 1;
    }
    return 0;
}

int cmux_input_frame_is_pending_frame(struct cmuxlib *cm, unsigned short offset)
{
    if (cm->cmux_if.result.start_flag == 1 &&
        cm->cmux_if.result.end_flag == 0 &&
        cm->cmux_if.result.err == 0 &&
        ((unsigned int)(cm->cmux_if.pending_frame_idx + offset) < MAX_CMUX_FRAME_SIZE (cm))) {
        return 1;
    }
    return 0;
}

void cmux_input_frame_to_cmux_fill_buf1(unsigned char *rawptr[],
    unsigned short rawptrlen[], unsigned char *buffer, unsigned short buffer_len)
{
    rawptr [0]    = (unsigned char *) NULL;
    rawptrlen [0] = 0;

    rawptr [1]    = buffer;
    rawptrlen [1] = buffer_len;
}

int cmux_input_frame_to_cmux_step1(struct cmuxlib *cm, unsigned char *rawptr[],
    unsigned short rawptrlen[], unsigned char *buffer, unsigned short buffer_len)
{
    unsigned int   k;
    unsigned char *temp_pending_frame = NULL;
    unsigned short offset;
    int            rc;
    unsigned int   len;
    int            ret;

    /*
     * STEP 1: Consider Pending data if any
     * Before going to extract several frames from the buffer[], first check for pending frame
     * If Previous pending frame is there, then it will be appended in end, to see if the frame makes sense in parsing
     * Logic below fills rawptr[0,1], rawptrlen[0,1] respectively with index-0 filled means its an approved & merged proper pending frame
     */
    if (cm->cmux_if.pending_frame_idx == 0) {

        CMUX_PRINT(CMUX_DEBUG_INFO, "there is no pending frame\r\n");
        /* 没有pending_frame的情形，放在rawptr[1]里面检验 */
        cmux_input_frame_to_cmux_fill_buf1(rawptr, rawptrlen, buffer, buffer_len);

        return 1;
    }

    /*
     * Now that there is a pending frame, check if there a pending counter part in received frame
     * Copy upto BASIC_FRAME_LEN of Pending-Buffer,.. from buffer_len
     * Check for frame correctness
     */
    if (buffer_len >= MAX_CMUX_FRAME_SIZE (cm)) {
        /* Note: full buffer not copied */
        offset = (unsigned short)MAX_CMUX_FRAME_SIZE (cm);
    } else {
        offset = buffer_len;
    }
    if(cm->cmux_if.pending_frame_idx + offset > CMUX_PENDING_FRAME_SIZE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "buffer cross the boundary!\r\n");
        return -1;
    }
    if(offset > 0) {
        ret = memcpy_s((cm->cmux_if.pending_frame + cm->cmux_if.pending_frame_idx),
                        (CMUX_PENDING_FRAME_SIZE - cm->cmux_if.pending_frame_idx), buffer, offset);
        CMUX_MEM_CHK_RTN_VAL(ret);
    }

    /*
     * Always check.. Call cmux_check_proper_frame() only if MAX size is Hit !!
     * 校验从pending_frame开始是不是一个完整的帧
     */
    if ((unsigned int)(cm->cmux_if.pending_frame_idx + offset) >= MAX_CMUX_FRAME_SIZE (cm)) {
        len = MAX_CMUX_FRAME_SIZE(cm);
    } else {
        len = (cm->cmux_if.pending_frame_idx) + offset;
    }

    rc = cmux_check_proper_frame_phase1(cm, &temp_pending_frame, &k, len);
    if (rc == -1) {
        if (cm->cmux_if.pending_frame != NULL) {
            (void)memset_s((void*)(cm->cmux_if.pending_frame), CMUX_PENDING_FRAME_SIZE, 0, CMUX_PENDING_FRAME_SIZE);
        }
        cm->cmux_if.pending_frame_idx = 0;

        return 0;
    }

    /* if correct then pending frame + consume cm->if.result.end_offset - cm->if.pending_frame_idx) + 1 from buffer[] */
    if (cmux_input_frame_is_good_frame(cm) == 1) {
        /*
         * 有头有尾，取完整的帧
         * GOOD frame case
         */
        rawptr [0]    = temp_pending_frame;
        rawptrlen [0] = cm->cmux_if.result.end_offset + 1;

        rawptr [1]    = buffer + cm->cmux_if.result.end_offset - cm->cmux_if.pending_frame_idx + k + 1;
        rawptrlen [1] = (unsigned short)(buffer_len - ((cm->cmux_if.result.end_offset
                                    - cm->cmux_if.pending_frame_idx + k) + 1));
    } else if (cmux_input_frame_is_pending_frame(cm, offset) == 1) {
        /*
         * 有头无尾，接在后面
         * STILL PENDING case
         * Still pending, but cannot be dismissed as incorrect-frame
         * ie,. start flag is there, but no end flag..and no-error at the same time total pending data that was check has not cross MAX-CMUX frame size
         * No pending frame to process, but pending index gets an update
         */
        cm->cmux_if.pending_frame_idx = cm->cmux_if.pending_frame_idx + offset;
        rawptr [0]                    = (unsigned char *) NULL;
        rawptrlen [0]                 = 0;

        /*
         * If control is here, it certainly means that offset == buffer_len AND less than MAX_CMUX_FRAME_SIZE,
         * and is FULLY consumed nothing to pending with nothing left to process in buffer
         */
        rawptr [1]                    = (unsigned char *) NULL;
        rawptrlen [1]                 = 0;
        CMUX_PRINT(CMUX_DEBUG_INFO, "frame is added to pending frame!\r\n");
        return 0;
        /* Not to worry that we overflow the pending buffer, code checks for accomodating ONLY up to MAX CMUX frame size and not beyond */
    } else {
        /*
         * 其他情形，全部送进STEP3处理
         * BY DEFAULT frame HAS to be GOOD or fall to PENDING, else we flush

         * if incorrect flush pending frame, go ahead with new  buffer
         * Eg: cases like this will fall here: (cm->cmux_if.result.start_flag == 0 || cm->cmux_if.result.err == 1) (OR)
         * start flag == 1 and end flag == 0 and No-error BUT pending_frame_idx + offset >= MAX_CMUX_FRAME_SIZE, then no way we store the pending frame anymore
         */
        cm->cmux_if.pending_frame_idx = 0;
        (void)memset_s((void *)(cm->cmux_if.pending_frame), CMUX_PENDING_FRAME_SIZE, 0, CMUX_PENDING_FRAME_SIZE);

        cmux_input_frame_to_cmux_fill_buf1(rawptr, rawptrlen, buffer, buffer_len);
    }

    return 1;
}

int cmux_input_frame_to_cmux_step2(struct cmuxlib *cm, unsigned char *rawptr,
    unsigned short rawptrlen, int *count)
{
    struct frame  *fr = NULL;
    unsigned int   rc_check = 0xff;

    /*
     * STEP 2: Send pending frame and new buffer frame now
     * rawptr[0] is for successful pending frame formation
     */
    if (rawptr != NULL  && rawptrlen != 0) {
        fr = (struct frame *) cmux_alloc (sizeof (struct frame));

        if (fr == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "fr null error!\r\n");
            return -1;
        }

        /* juvin <M2M-1552> <Sending NSC response to unsupported commands><begin> */
        rc_check = cmux_frame_to_struct (&fr, cm, rawptr, rawptrlen);

        /* juvin: to send nsc for invalid frame */
        if(rc_check == INVALID_FRAME_ERR) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "rc_check is INVALID_FRAME_ERR\r\n");
            fr->f_type = NSC;
        }

        /* juvin <M2M-1552> <Sending NSC response to unsupported commands><end> */
        *count = 1;

        /* Give frame to command layer */
        if((rc_check == INVALID_FRAME_ERR) || (rc_check == FRAME_OK)) {
            (*(cm->frame_map.cmux_cmd_handler))((struct cmuxlib *) cm, (struct frame *) fr);
        }
        cmux_free_frame (fr);
        g_nsc_frm = NULL;

        cm->cmux_if.pending_frame_idx = 0;
        if (cm->cmux_if.pending_frame != NULL) { /* coverity fix */
            (void)memset_s((void*)(cm->cmux_if.pending_frame), CMUX_PENDING_FRAME_SIZE, 0, CMUX_PENDING_FRAME_SIZE);
        }
    }

    return 1;
}

int cmux_input_frame_is_vaild_start_flag(unsigned char *rawptr[], unsigned short rawptrlen[], int offset)
{
    /* Do a basic check for Start flag,..before calling cmux_check_proper_frame() */
    if (rawptrlen [1] > 1) {
        if (!((*(rawptr [1] + offset) == CMUX_FRAME_FLAG) && (*(rawptr [1] + offset + 1) != CMUX_FRAME_FLAG))) {
            return 0;
        }
    } else {
        if (*(rawptr [1] + offset) != CMUX_FRAME_FLAG) {
            return 0;
        }
    }
    return 1;
}

int cmux_input_frame_to_cmux_correct_frame_process(struct cmuxlib *cm, struct frame_result *fresult,
    unsigned char *rawptr, int *count, int i)
{
    struct frame *fr = NULL;
    unsigned int  rc_check = 0xff;
    int           result;

    /* CORRECT frame check */
    if (fresult->start_flag == 1 &&
        fresult->length_flag == 1 &&
        fresult->fcs == 1 &&
        fresult->end_flag == 1 &&
        fresult->err == 0) {
        /* Entry of a good frame */
        if (*count == -1) {
            *count = 0;
        }

        fr = (struct frame *) cmux_alloc (sizeof (struct frame));
        if (fr == NULL) {
            return -1;
        }

        /* juvin <M2M-1552> <Sending NSC response to unsupported commands><begin> */
        rc_check = cmux_frame_to_struct (&fr, cm, rawptr + i, (fresult->end_offset - fresult->start_offset + 1));
        CMUX_PRINT(CMUX_DEBUG_INFO, "rc_check = %d\n", rc_check);

        /* juvin: to send nsc for invalid frame */
        if(rc_check == INVALID_FRAME_ERR) {
            fr->f_type = NSC;
        }

        /* juvin <M2M-1552> <Sending NSC response to unsupported commands><end> */
        (*count)++;

        /* Give frame to command layer */
        if((rc_check == INVALID_FRAME_ERR) || (rc_check == FRAME_OK)) {
            result = (*cm->frame_map.cmux_cmd_handler)((struct cmuxlib *) cm, (struct frame *) fr);
            CMUX_PRINT(CMUX_DEBUG_INFO, "cmux_cmd_handler return %d\n", result);
        }
        cmux_free_frame (fr);
        g_nsc_frm = NULL;
    }

    return 0;
}

int cmux_input_frame_to_cmux_step3(struct cmuxlib *cm, unsigned char *rawptr[],
    unsigned short rawptrlen[], int count)
{
    int rc  = 0;
    int ret;
    unsigned short i;
    unsigned short offset;
    struct frame_result fresult;

    /*
     * STEP 3: Call series of cmux_frame_to_struct and get the framing done for buffer
     * rawptr[1] is for buffer
     */
    for (i = 0; i < (rawptrlen [1]); )
    {
        /* Do a basic check for Start flag,..before calling cmux_check_proper_frame() */
        if (cmux_input_frame_is_vaild_start_flag(rawptr, rawptrlen, i) == 0) {  /* 不合法，往后偏移 */
            i++;
            continue;
        }

        /* Give only required length to check for frame */
        if ((unsigned int)(rawptrlen [1] - i) < MAX_CMUX_FRAME_SIZE (cm)) {
            offset = rawptrlen [1] - i;
        } else {
            offset = (unsigned short)MAX_CMUX_FRAME_SIZE (cm);
        }
        CMUX_PRINT(CMUX_DEBUG_INFO, "offset = %d\n", offset);
        /* Best is been done,start flag locaed, length found and trimmed to max.length, now check for proper frame */
        rc = cmux_check_proper_frame (rawptr [1] + i, cm, offset, &fresult);
        if (rc == -1) {
            if (cm->cmux_if.pending_frame != NULL) {
                memset_s((void*)(cm->cmux_if.pending_frame), CMUX_PENDING_FRAME_SIZE, 0, CMUX_PENDING_FRAME_SIZE);
            }
            cm->cmux_if.pending_frame_idx = 0;

            return 0;
        }

        /* CORRECT frame check */
        rc = cmux_input_frame_to_cmux_correct_frame_process(cm, &fresult, rawptr[1], &count, i);
        if (rc == -1) {
            return rc;
        }

        /* Pending frame creation happens here.. */
        if (fresult.start_flag == 1 && fresult.end_flag == 0 && fresult.err == 0) {
            if (count == -1) {
                count = 0;               /* Entry of a good frame */
            }
            /* Control is here, means we are at the near end of buffer.. so back up to pending frame */
            if (offset > 0 && offset <= CMUX_PENDING_FRAME_SIZE) {
                ret = memcpy_s(cm->cmux_if.pending_frame, CMUX_PENDING_FRAME_SIZE,
                    (rawptr [1] + i + fresult.start_offset), offset);
                CMUX_MEM_CHK_RTN_VAL(ret);
            }
            cm->cmux_if.pending_frame_idx = offset;
            CMUX_PRINT(CMUX_DEBUG_INFO, "frame is not full.count is %d\n", count);
            return count;
        }

        /* Move the offset (good frame offset or the bad frame offset(atleast one byte increment and go to check F9 logic ) */
        i = i + fresult.end_offset + 1;
    }

    CMUX_PRINT(CMUX_DEBUG_INFO, "THE number of frames are %d\n",count);

    return count;
}

int cmux_Input_frame_to_cmux(struct cmuxlib *cm, unsigned char *buffer, unsigned short buffer_len)
{/*lint !e18 !e516*/
    int count = -1;
    int ret;
    unsigned char *rawptr [CMUX_BUFF1_LEN];
    unsigned short rawptrlen [CMUX_BUFF1_LEN];

    CMUX_PRINT(CMUX_DEBUG_INFO, "cmux_hex_dump!\r\n");
    cmux_hex_dump(CMUX_DEBUG_INFO, buffer, buffer_len);
    /* Entry validations */
    if (cm == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cm or COM null error!\r\n");
        return -1;
    }

    if (buffer == NULL || buffer_len == 0) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "buffer length error!\r\n");
        return -1;
    }
    CMUX_PRINT(CMUX_DEBUG_INFO, "buffer_len = %d!\r\n", buffer_len);
    CMUX_PRINT(CMUX_DEBUG_INFO, "pending_frame_idx = %d!\r\n", cm->cmux_if.pending_frame_idx);

    /* STEP 1: Consider Pending data if any */
    ret = cmux_input_frame_to_cmux_step1(cm, rawptr, rawptrlen, buffer, buffer_len);
    if (ret != 1) {
        return ret;
    }

    /* STEP 2: Send pending frame and new buffer frame now */
    ret = cmux_input_frame_to_cmux_step2(cm, rawptr[0], rawptrlen[0], &count);
    if (ret != 1) {
        return ret;
    }

    /* STEP 3: Call series of cmux_frame_to_struct and get the framing done for buffer */
    CMUX_PRINT(CMUX_DEBUG_INFO, "rawptrlen [1] = %d\n", rawptrlen [1]);
    return cmux_input_frame_to_cmux_step3(cm, rawptr, rawptrlen, count);

} /* Cmux_Input_frame_to_CMUX */


int cmux_framer_init(struct cmuxlib* cm)
{
    if (((cm->cmux_status & CMUX_TE_MODE) != CMUX_TE_MODE) && ((cm->cmux_status & CMUX_UE_MODE) != CMUX_UE_MODE)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_status error!\n");
        return FAIL;
    }

    if (cm->cmux_status & CMUX_TE_MODE) {
        cm->frame_map.frdetails    = (struct framedetails*)NULL;
        cm->frame_map.pending_mask = 0;
    } else {
        cm->frame_map.frdetails    = g_UE_frame_details;
        cm->frame_map.pending_mask = UE_PENDING_MASK;
    }
    cm->cmux_if.pending_frame = cmux_alloc(CMUX_PENDING_FRAME_SIZE);
    return PASS;
} /* cmux_framer_init */


#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */