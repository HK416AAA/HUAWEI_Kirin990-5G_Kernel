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


#ifdef WIN32
#include <string.h>
#else
#include <linux/string.h>
#endif /* WIN32 */

#include "cmux_api_ifc.h"
#include "cmux_framer.h"
#include "cmux_cmd.h"
#include "cmux_drv_ifc.h"
#include "cmux_tgt_osal.h"
#include <linux/semaphore.h>
#include "securec.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_IOT_CMUX == FEATURE_ON)
/* Functions common to both TE (Host) and UE (Target) mode are placed here */
struct cmux_cmd* cmux_make_cmd(struct frame*, struct cmux_cmd**);
int cmux_queue_cmd(struct cmuxlib*, struct cmux_cmd**);
int cmux_kick_start_cmd(struct cmuxlib*, unsigned char, unsigned short, unsigned short);
int cmux_create_dlc_entry(struct cmuxlib*, unsigned char);
int cmux_cmd_init(struct cmuxlib*);
int cmux_delete_dlc_entry(struct cmuxlib*, unsigned char);
void delete_head_cmd(struct cmuxlib* cm, unsigned char dlc);
int cmux_get_default_cmux_frame (struct cmuxlib *cm, DLCINFO dlc_info, struct frame **fr,
    unsigned char *data, unsigned int data_len);


void cmux_get_default_mux_params(struct mux_params* muxparam, cmux_info_type* cmux_info)
{
    muxparam->ack_timer_t1    = cmux_info->response_timer_T1;
    muxparam->baud_rate       = cmux_info->port_speed;
    muxparam->frame_size_n1   = cmux_info->max_frame_size_N1;
    muxparam->mode            = cmux_info->operating_mode;
    muxparam->res_timer_t2    = cmux_info->response_timer_T2;
    muxparam->retransmit_n2   = cmux_info->max_cmd_num_tx_times_N2;
    muxparam->wakeup_timer_t3 = cmux_info->response_timer_T3;
    muxparam->window_k        = cmux_info->window_size_k;
}

int cmux_mux_param_validate(struct mux_params mux)
{
    if ((mux.ack_timer_t1 < MIN_DEFAULT_ACK_TIMER_T1) || (mux.ack_timer_t1 > MAX_DEFAULT_ACK_TIMER_T1)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mux T1 error!\n");
        return FAIL;
    }

    if ((mux.frame_size_n1 > CMUX_MAXIMUM_FRAME_SIZE) || (mux.frame_size_n1 < CMUX_MINIMUM_FRAME_SIZE)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mux N1 error!\n");
        return FAIL;
    }

    if ((mux.res_timer_t2 < MIN_DEFAULT_RES_TIMER_T2) || (mux.res_timer_t2 > MAX_DEFAULT_RES_TIMER_T2)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mux T2 error!\n");
        return FAIL;
    }

    if (mux.retransmit_n2 > MAX_DEFAULT_RETRANS_N2) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mux N2 error!\n");
        return FAIL;
    }

    if (mux.wakeup_timer_t3 < DEFAULT_WAKEUP_TIMER_T3) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mux T3 error!\n");
        return FAIL;
    }

    if ((mux.window_k > MAX_WINDOW_K) || (mux.window_k == 0)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mux WINDOW K error!\n");
        return FAIL;
    }

    return PASS;
} /* cmux_mux_param_validate */


void cmux_set_default_sncdb(struct cmuxlib* cm);

struct cmuxlib* cmux_cmuxlib_init(CMUX_ModeUint8 mode, cmux_info_type* cmux_info)
{
    CMUXLIB          *cm = NULL;
    int               status;
    struct mux_params mux;

    if (((mode & CMUX_TE_MODE) != CMUX_TE_MODE) && ((mode & CMUX_UE_MODE) != CMUX_UE_MODE)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mode error!\n");
        return (struct cmuxlib*)NULL;
    }

    cm = (CMUXLIB*)cmux_alloc(sizeof(struct cmuxlib));
    if (cm == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cm cmux_alloc error!\n");
        return (struct cmuxlib*)NULL;
    }
    cm->cmux_status = mode;
    /* default we can recv data from host */
    cm->flow_ctrl = 1; /*lint !e63*/
    cmux_get_default_mux_params(&mux, cmux_info);
    cmux_set_default_sncdb(cm);

    /* VALIDATE input mux structure is with proper values */
    status = cmux_mux_param_validate(mux);
    if (status == 0) {
        cmux_free((unsigned char*)cm);
        CMUX_PRINT(CMUX_DEBUG_ERR, "mux validate error!\n");
        return (struct cmuxlib*)NULL;
    }
    cm->mp = mux;

    /* Perform cmux_cmd, cmux_framer module initialisation, if any */
    cmux_cmd_init(cm);
    sema_init(&cm->stKickSem, 1);
    return cm;
} /* cmux_cmuxlib_init */


int cmux_register_callback(struct cmuxlib* cm, struct cmux_callback* cmux_cb)
{
    if (cm == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cm NULL error!\n");
        return FAIL;
    }

    if (((cm->cmux_status & CMUX_TE_MODE) != CMUX_TE_MODE) && ((cm->cmux_status & CMUX_UE_MODE) != CMUX_UE_MODE)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_status error!\n");
        return FAIL;
    }

    if ((cmux_cb->callback_func == NULL)
#if (!defined(LLT_OS_VER))
        || (cmux_cb->send_dl_data == NULL)
#endif
        || (cmux_cb->set_power_cb == NULL)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "callbk NULL error!\n");
        return FAIL;
    }

    cm->cb.callback_func = cmux_cb->callback_func;

#if (defined(LLT_OS_VER))
    cm->cb.send_to_com = cmux_cb->send_to_com; /* dmt打桩函数 */
#else
    cm->cb.send_dl_data = cmux_cb->send_dl_data;
#endif
    cm->cb.set_power_cb = cmux_cb->set_power_cb;

    return PASS;
} /* cmux_register_callback */


int cmux_get_cmux_frame_from_max_frame(unsigned char* buff, unsigned short len,
    struct cmuxlib*  cm, unsigned char dlc)
{
    struct cmux_cmd  *cmd_node = NULL;
    struct frame     *fr  = NULL;
    unsigned char*    ptr = (unsigned char*)NULL;
    DLCINFO           dlc_info;
    int               rc;
    unsigned short    i;
    unsigned short    fragment_len = 0;

    rc = FAIL;
    (void)memset_s((void*)&dlc_info, sizeof(DLCINFO), 0, sizeof(DLCINFO));
    ptr = buff;
    for (i = 0; i < len;) {
        if ((unsigned int)(len - i) > MAX_TX_DATA_PKT_LEN(cm)) { /*lint !e574*/
            fragment_len = (unsigned short)MAX_TX_DATA_PKT_LEN(cm);
        } else {
            fragment_len = len - i;
        }

        /* Making a new frame */
        dlc_info.dlc   = dlc;
        dlc_info.ftype = UIH_DATA;
        rc             = cmux_get_default_cmux_frame(cm, dlc_info, &fr, &ptr[i], fragment_len);

        if (rc == PASS) {
            cmd_node = cmux_make_cmd(fr, &cmd_node);
            if (cmd_node == NULL) {
                cmux_free_frame(fr);
                return FAIL;
            }
            cmux_queue_cmd(cm, &cmd_node);
        } else {
            break;
        }
        i += fragment_len;
    }

    return PASS;
}

int cmux_check_status_legality(struct cmuxlib*  cm, unsigned char* buff, unsigned short len, unsigned char dlc,
        unsigned int control)
{
    if ((cm == NULL) || ((cm->cmux_status & (ATCMUX_ON_MODE | CTRL_DLC_MODE)) != (ATCMUX_ON_MODE | CTRL_DLC_MODE)) ||
        (dlc > MAX_DLC) || (!(control & (CMUX_FRAGMENT_DATA | CMUX_FLUSH_DATA)))) {
        return FAIL;
    }

    /* Check for FC bit for that particular dlc */
    if ((cm->dlc_db[dlc] == NULL) || (!(CHECK_DLC_STATE(cm->dlc_db[dlc]->dlc_state, DLC_ESTABLISHED)))) {
        return FAIL;  /* Not allowed any further */
    }

    /* Additional validations for CMUX_FRAGMENT_DATA */
    if (control & CMUX_FRAGMENT_DATA) {
        if (buff == NULL || len > MAX_SERIAL_PKT_LEN || len == 0) {
            return FAIL;
        }
    }

    return PASS;
}

int cmux_send_dlc_data(struct cmuxlib* cmxlib, unsigned char* buff, unsigned short len, unsigned char dlc,
    unsigned int control)
{
    struct cmuxlib*  cm = (struct cmuxlib*)cmxlib;
    int              rc;

    rc = cmux_check_status_legality(cm, buff, len, dlc, control);
    if (rc == FAIL) {
        return FAIL;
    }

    /*
     * STEP 1: First fragment to smaller pieces if a huge-data than max-cmux-fr size, OR fill smaller pieces of data in
     * to one-cmux frame if max-cmux-frame size is BIG kick_start_rdy is set only if atleast one frame is full up to
     * max-cmux-fr size
     */
    if (control & CMUX_FRAGMENT_DATA) {
        /* check dlc establised. */
        if ((cm->dlc_db[dlc] != NULL) && (CHECK_DLC_STATE(cm->dlc_db[dlc]->dlc_state, DLC_ESTABLISHED))) {
            rc = cmux_get_cmux_frame_from_max_frame(buff, len, cm, dlc);
            if (rc == FAIL) {
                return FAIL;
            }
        }
    }

    if (control & CMUX_FLUSH_DATA) {
        if (!((cm->cmux_status & CMUX_UE_MODE) && (cm->dlc_db[dlc]) && (cm->dlc_db[dlc]->msc.fc == 1))) {
            rc = cmux_kick_start_cmd(cm, dlc, 0, 0);
        }
    }
    return rc;
} /* cmux_send_dlc_data */

int cmux_send_msc_cmd(struct cmuxlib* cm, unsigned char dlc, int kickstart, struct modem_status_cmd* msc)
{
    struct frame    *fr       = NULL;
    struct cmux_cmd *cmd_node = NULL;
    DLCINFO          dlc_info;
    int              rc;

    (void)memset_s((void*)&dlc_info, sizeof(DLCINFO), 0, sizeof(DLCINFO));
    if ((cm == NULL) || (dlc > MAX_DLC) ||
        ((cm->cmux_status & (ATCMUX_ON_MODE | CTRL_DLC_MODE)) != (ATCMUX_ON_MODE | CTRL_DLC_MODE))) {
        return FAIL;
    }

    if (cm->dlc_db[dlc] == NULL) {
        return FAIL;
    }

    cm->dlc_db[dlc]->msc.dlci = msc->dlci;
    cm->dlc_db[dlc]->msc.fc   = msc->fc;
    cm->dlc_db[dlc]->msc.rtc  = msc->rtc;
    cm->dlc_db[dlc]->msc.rtr  = msc->rtr;
    cm->dlc_db[dlc]->msc.dv   = msc->dv;
    cm->dlc_db[dlc]->msc.ic   = msc->ic;

    /* 当前此函数仅做MSC主动上报用，用MSC_RESP使TypeCR位为0 */
    dlc_info.dlc   = dlc;
    dlc_info.ftype = MSC_RESP;
    rc             = cmux_get_default_cmux_frame(cm, dlc_info, &fr, (unsigned char*)msc, 0);
    if (rc == PASS) {
        cmd_node = cmux_make_cmd(fr, &cmd_node);
        if (cmd_node == NULL) {
            cmux_free_frame(fr);
            return FAIL;
        }
        cmux_queue_cmd(cm, &cmd_node);
    }
    if (kickstart == 1) {
        rc = cmux_kick_start_cmd(cm, 0, 0, 0);
    }

    return rc;
} /* cmux_send_msc_cmd */

int cmux_send_msc(struct cmuxlib* cm, unsigned char dlc, struct modem_status_cmd* msc)
{
    int status;

    /* No DLC-param for DLC-0, also nothing to goahead with DLC-0 being up */
    if ((cm == NULL) || (dlc > MAX_DLC) || (dlc == 0) || (cm->dlc_db[0] == NULL) ||
        ((cm->cmux_status & (ATCMUX_ON_MODE | CTRL_DLC_MODE)) != (ATCMUX_ON_MODE | CTRL_DLC_MODE))) {
        return FAIL;
    }
    if ((cm->dlc_db[dlc] == NULL) || (!(CHECK_DLC_STATE(cm->dlc_db[dlc]->dlc_state, DLC_ESTABLISHED)))) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "dlc is not done!\n");
        return FAIL;  /* Not allowed any further */
    }
    status = cmux_send_msc_cmd(cm, dlc, 1, msc);

    return status;
}

void cmux_fcon_fcoff_check_dlc_state(unsigned int *dlc0_state, int *stop_proc)
{
    if (CHECK_DLC_STATE(*dlc0_state, UE_SENT_FCON_PENDING)) {
        CMUX_PRINT(CMUX_DEBUG_INFO, "FCON is pending, clear...\n");
        CLEAR_DLC_STATE(*dlc0_state, (UE_SENT_FCON_RESP_RECVD));
        *stop_proc = PASS;
    }
    if (CHECK_DLC_STATE(*dlc0_state, UE_SENT_FCOFF_PENDING)) {
        CMUX_PRINT(CMUX_DEBUG_INFO, "FCOFF is pending, clear...\n");
        CLEAR_DLC_STATE(*dlc0_state, (UE_SENT_FCOFF_RESP_RECVD));
        *stop_proc = PASS;
    }
}

int cmux_send_fcon_fcoff_cmd(struct cmuxlib* cm, unsigned char dlc, int kickstart, CMUX_FrTypeUint8 ftype)
{
    struct frame    *fr         = NULL;
    struct cmux_cmd *cmd_node   = NULL;
    unsigned int    *dlc0_state = NULL;
    DLCINFO          dlc_info;
    int              rc;
    int              stop_proc  = FAIL;

    (void)memset_s((void*)&dlc_info, sizeof(DLCINFO), 0, sizeof(DLCINFO));
    if ((cm == NULL) || (dlc > MAX_DLC) ||
        ((cm->cmux_status & (ATCMUX_ON_MODE | CTRL_DLC_MODE)) != (ATCMUX_ON_MODE | CTRL_DLC_MODE)) ||
        (cm->dlc_db[dlc] == NULL)) {
        return FAIL;
    }

    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);

    cmux_fcon_fcoff_check_dlc_state(dlc0_state, &stop_proc);

    if (stop_proc == PASS) {
        cmux_stop_timer(cm, dlc);
        delete_head_cmd(cm, dlc);
    }
    if (ftype == FCOFF_CMD) {
        CMUX_PRINT(CMUX_DEBUG_INFO, "FCOFF pending is set...\n");
        SET_DLC_STATE(*dlc0_state, (UE_SENT_FCOFF_PENDING));
    } else if (ftype == FCON_CMD) {
        CMUX_PRINT(CMUX_DEBUG_INFO, "FCON pending is set...\n");
        SET_DLC_STATE(*dlc0_state, (UE_SENT_FCON_PENDING));
    }

    dlc_info.dlc   = 0;
    dlc_info.ftype = ftype;
    rc             = cmux_get_default_cmux_frame(cm, dlc_info, &fr, 0, 0);  /* SABM for DLC */
    if (rc == PASS) {
        cmd_node = cmux_make_cmd(fr, &cmd_node);
        if (cmd_node == NULL) {
            cmux_free_frame(fr);
            return FAIL;
        }
        if (fr->pf_bit == 0x01) {
            cmd_node->ct.res_timer_t2  = cm->mp.res_timer_t2;
            cmd_node->ct.retransmit_n2 = cm->mp.retransmit_n2;
        }
        cmux_queue_cmd(cm, &cmd_node);
    }
    if (kickstart == 1) {
        rc = cmux_kick_start_cmd(cm, 0, 0, 0);
    }

    return rc;
} /* cmux_send_fcon_fcoff_cmd */

int cmux_send_fcon_fcoff(struct cmuxlib* cm, unsigned char dlc, CMUX_FrTypeUint8 ftype)
{
    int status = 0;

    if ((ftype != FCOFF_CMD) && (ftype != FCON_CMD)) {
        return FAIL;
    }
    /* No DLC-param for DLC-0, also nothing to goahead with DLC-0 being up */
    if ((cm == NULL) || (dlc != 0) || (cm->dlc_db[0] == NULL) ||
        ((cm->cmux_status & (ATCMUX_ON_MODE | CTRL_DLC_MODE)) != (ATCMUX_ON_MODE | CTRL_DLC_MODE))) {
        return FAIL;
    }

    status = cmux_send_fcon_fcoff_cmd(cm, dlc, 1, ftype);

    return status;
} /* cmux_send_fcon_fcoff */
#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */