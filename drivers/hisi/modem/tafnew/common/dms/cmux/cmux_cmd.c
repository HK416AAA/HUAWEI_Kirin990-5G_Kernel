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

/* Cmux common includes */
#include "cmux_cmd.h"
#include "cmux_framer.h"
#include "cmux_api_ifc.h"
#include "cmux_drv_ifc.h"

/* OSAL, platform specific includes */
#include "cmux_tgt_osal.h"
#include "securec.h"

#ifndef WIN32
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/fs.h>
#include <linux/string.h>
#endif /* WIN32 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_IOT_CMUX == FEATURE_ON)
#ifndef TRUE
#define TRUE 1
#endif
extern unsigned int cmux_struct_to_frame(struct frame **fr, struct cmuxlib *cm);
extern unsigned int cmux_get_g_nsc_received(void);
extern void cmux_set_g_nsc_received(unsigned int nsc_received);
int cmux_framer_init(struct cmuxlib*);
int cmux_validate_te_ue_dlc_params(struct dlc_param_neg*, struct dlc_param_neg*, enum NEGOTIATION_CMD);
int cmux_ue_cmd_handler(void* cmxlib, void* frm);
int cmux_rxed_timer_expiry(struct cmuxlib* cm, unsigned char dlc);

int cmux_get_default_cmux_frame (struct cmuxlib *cm, DLCINFO dlc_info, struct frame **fr,
    unsigned char *data, unsigned int data_len);


int cmux_cmd_init(struct cmuxlib* cm)
{
    if (((cm->cmux_status & CMUX_TE_MODE) != CMUX_TE_MODE) && ((cm->cmux_status & CMUX_UE_MODE) != CMUX_UE_MODE)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_state error!\n");
        return FAIL;
    }

    cm->frame_map.cmux_cmd_handler = cmux_ue_cmd_handler;

    return (cmux_framer_init(cm));
}

void cmux_flush_cmd(struct cmuxlib* cm, unsigned char dlc_to_flush)
{
    struct cmux_cmd *node_to_free = NULL;
    struct cmux_cmd *head = NULL;

    head = cm->dlc_db[dlc_to_flush]->cmd_list;

    while (head) {
        node_to_free = head;
        head         = node_to_free->next;
        if (node_to_free->fr != NULL) {
            cmux_free_frame(node_to_free->fr);
        }
        cmux_free((unsigned char*)node_to_free);
        node_to_free = NULL;
    }
    cm->dlc_db[dlc_to_flush]->cmd_list = (struct cmux_cmd*)NULL;
}

/* Delete the cmd node belonging to a specific DLC Every command deleted is always in the head */
void delete_head_cmd(struct cmuxlib* cm, unsigned char dlc)
{
    struct cmux_cmd *cmd_node = NULL;

    if (cm == NULL) {
        return;
    }
    cmd_node = cm->dlc_db[dlc]->cmd_list;

    if (cmd_node != NULL) {
        if (CHECK_DLC_STATE(cm->dlc_db[dlc]->dlc_state, cm->frame_map.pending_mask)) {
            cmux_stop_timer(cm, dlc);
        }

        cm->dlc_db[dlc]->cmd_list = cmd_node->next;

        if (cmd_node->fr == NULL) {
            cmux_free((unsigned char*)cmd_node);
            cmd_node = NULL;
            return;
        }
        /* update statistics.. */
        if ((cmd_node->fr->f_type == UIH_DATA) && (cm->cmux_status & CMUX_UE_MODE)) {
            if (cm->dlc_db[dlc]->stats.pending_tx_data) {
                cm->dlc_db[dlc]->stats.pending_tx_data--;
            }
            if (cm->dlc_db[dlc]->stats.dlc_buffer_size && cm->dlc_db[dlc]->msc.fc == 1) {
                cm->dlc_db[dlc]->stats.dlc_buffer_size -= cmd_node->fr->length;
            }
        } else {
            cm->dlc_db[dlc]->stats.pending_tx_ctrl--;
        }

        cm->dlc_db[dlc]->stats.pending_tx_total--;
        cmux_free_frame(cmd_node->fr);
        cmux_free((unsigned char*)cmd_node);
        /* juvin<m2m-1638,1639,1641><Crash on wrong frame format,avoid dangling pointer> */
        cmd_node = NULL;
    }
}

void cmux_set_default_sncdb(struct cmuxlib* cm)
{
    int i;

    if (cm == NULL) {
        return;
    }

    for (i = 1; i <= MAX_DLC_TGT; i++) {
        cm->snc_db[i].dlc  = (unsigned char)i;
        cm->snc_db[i].task = SNC_TYPE_AT_DATA;
    }
}

int cmux_create_dlc_entry(struct cmuxlib* cm, unsigned char dlc)
{
    if (cm == NULL) {
        return FAIL;
    }

    if (cm->cmux_status & CMUX_UE_MODE) {
        if (!(dlc <= MAX_DLC)) {
            return FAIL;
        }
    }
    if (dlc == 0) {
        cm->dlc_count = 1;  /* reset */
    }
    if (dlc != 0) {
        cm->dlc_count++;
    }

    /* check if already created.. */
    if (cm->dlc_db[dlc] == NULL) {
        cm->dlc_db[dlc] = (struct DLC_Entry*)cmux_alloc(sizeof(struct DLC_Entry));
        /* juvin: A null check added */
        if (cm->dlc_db[dlc] == NULL) {
            return FAIL;
        }
        /* Hari:<> Begin */
        (void)memset_s((void*)cm->dlc_db[dlc], sizeof(struct DLC_Entry), 0,
                 sizeof(struct DLC_Entry)); /*lint !e160 !e506 !e522*/

        cm->dlc_db[dlc]->dlc = dlc;  /* added newly */
        /* HARI:<> End */

        cm->dlc_db[dlc]->stats.pending_tx_ctrl  = 0;
        cm->dlc_db[dlc]->stats.pending_tx_data  = 0;
        cm->dlc_db[dlc]->stats.pending_tx_total = 0;
        cm->dlc_db[dlc]->stats.dlc_buffer_size  = 0;
        cm->dlc_db[dlc]->dlc_state |= (DLC_ENTRY_DONE | DLC_COM_READY);
        cm->dlc_db[dlc]->cmd_list = (struct cmux_cmd*)NULL;

        /* set DLC params from default set mux params */
        cm->dlc_db[dlc]->dlcparam.muxparams  = cm->mp;
        cm->dlc_db[dlc]->dlcparam.conv_layer = CONVERGENCE_LAYER;
        cm->dlc_db[dlc]->dlcparam.priority   = DEFAULT_PRIORITY;

        if ((dlc != 0) && (dlc > 0 && dlc < (MAX_DLC_TGT + 1))) {
            cm->dlc_db[dlc]->snc.dlci = dlc;
            if (cm->snc_db[dlc].task == SNC_TYPE_AT_DATA) {
                cm->dlc_db[dlc]->snc.s1 = 1;
            } else if (cm->snc_db[dlc].task == SNC_TYPE_AT) {
                cm->dlc_db[dlc]->snc.s1 = 0;
            }
        }
        return PASS;
    }
    return FAIL;
}

/* Description: 关闭dlc指定的通道 */
int cmux_delete_dlc_entry(struct cmuxlib* cm, unsigned char dlc)
{
    int i;
    if (cm == NULL || (dlc > MAX_DLC)) {
        return FAIL;
    }

    if (dlc != 0) {
        cm->dlc_count--;
    }
    
    if ((dlc == 0) && (cm->dlc_count != 1)) {
        for (i = 1; i <= MAX_DLC_TGT; i++) {
            if (cm->dlc_db[i]) {
                cm->dlc_count--;
                cmux_flush_cmd(cm, (unsigned char)i);
                cm->dlc_db[i]->cmd_list = (struct cmux_cmd*)NULL;
                cmux_free((unsigned char*)cm->dlc_db[i]);
                cm->dlc_db[i] = (struct DLC_Entry*)NULL;
            }
        }
    }
    
    if (dlc == 0 && cm->dlc_count != 1) { /* !(only one left that is control-DLC) */
        return FAIL;                      /* we dont allow to delete control DLC until all are deleted */
    }

    /* check if already created.. */
    if (cm->dlc_db[dlc] != NULL) {
        cmux_flush_cmd(cm, dlc);
        cm->dlc_db[dlc]->cmd_list = (struct cmux_cmd*)NULL;
        cmux_free((unsigned char*)cm->dlc_db[dlc]);
        cm->dlc_db[dlc] = (struct DLC_Entry*)NULL;

        if (dlc == 0) {
            cm->cmux_status &= (~(ATCMUX_ON_MODE | CTRL_DLC_MODE));
            cm->cmux_status |= (ATCMUX_OFF_MODE);
        }
        return PASS;
    }
    return FAIL;
}

struct cmux_cmd* cmux_make_cmd(struct frame* fr, struct cmux_cmd** cmd)
{
    /* Put the frame pointer in the command frame
     * Based on the frame-type fill the Rx_Handler();
     * Based on the DLC of frame-type fill the CMUX_Timer value (timeout, retransmission values
     * Return the cmd;
     */
    if (cmd == NULL) {
        return (struct cmux_cmd*)NULL;
    }

    *cmd = (struct cmux_cmd*)cmux_alloc(sizeof(struct cmux_cmd));
    if (*cmd == NULL) {
        return *cmd;
    }

    (*cmd)->fr = fr;

    /* timer memset to zero */
    (*cmd)->next = (struct cmux_cmd*)NULL;

    return *cmd;
} /* cmux_make_cmd */

int cmux_queue_cmd(struct cmuxlib* cm, struct cmux_cmd** cmd)
{
    /*
      * Get the DLC for which this is destined (cmd->frame->DLC)
      * Follow the list in DLC_Entry[DLC].cmd_list and Queue the command node
      */
    struct cmux_cmd  *head = NULL;
    unsigned char     dlc;
    struct cmux_timer cmux_time;

    dlc = (*cmd)->fr->dlc;
    /* update statistics.. */
    if (((*cmd)->fr->f_type == UIH_DATA) && (cm->cmux_status & CMUX_UE_MODE)) {
        cm->dlc_db[dlc]->stats.pending_tx_data++;

        if (cm->dlc_db[dlc]->msc.fc == 1) {
            cm->dlc_db[dlc]->stats.dlc_buffer_size += (*cmd)->fr->length;
        }
    } else {
        cm->dlc_db[dlc]->stats.pending_tx_ctrl++;
    }

    /* DLC close or Mux close (for DLC-0) */
    if (((*cmd)->fr->f_type == CTRL_DISC) || ((*cmd)->fr->f_type == CLD_CMD)) {
        /* No point in having anything queued.. */
        cmux_flush_cmd(cm, dlc);
    }
    if (cm->dlc_db[dlc]->cmd_list == NULL) {
        cm->dlc_db[dlc]->cmd_list = *cmd;
    } else {
        head = cm->dlc_db[dlc]->cmd_list;

        while (head->next != NULL) {
            head = head->next;
        }

        head->next   = *cmd;
        (*cmd)->next = (struct cmux_cmd*)NULL;
    }

    cm->dlc_db[dlc]->stats.pending_tx_total++;

    /* For the added entry, if its response pending type.. copy the timeout requirements */
    cmux_time.ack_timer_t1    = cm->dlc_db[dlc]->dlcparam.muxparams.ack_timer_t1;
    cmux_time.res_timer_t2    = cm->dlc_db[dlc]->dlcparam.muxparams.res_timer_t2;
    cmux_time.retransmit_n2   = cm->dlc_db[dlc]->dlcparam.muxparams.retransmit_n2;
    cmux_time.wakeup_timer_t3 = cm->dlc_db[dlc]->dlcparam.muxparams.wakeup_timer_t3;
    cmux_time.window_k        = cm->dlc_db[dlc]->dlcparam.muxparams.window_k;
    (*cmd)->ct                = cmux_time;
    return PASS;
}

void cmux_invoke_uihdata_callback(void* cmxlib, struct frame* rxfr)
{
    int             len_bkp;
    int             len_vrfy;
    int             i;
    unsigned char*  data_ptr_bkp = (unsigned char*)NULL;
    struct cmuxlib* cm           = (struct cmuxlib*)cmxlib;

    if (cm == NULL || rxfr == NULL || rxfr->uih_data == 0 || rxfr->length == 0) {
        return;
    }

    data_ptr_bkp = (unsigned char*)rxfr->uih_data;
    len_vrfy     = rxfr->length;
    len_bkp      = len_vrfy;

    for (i = 0; i < len_bkp;) {
        if (len_vrfy > MAX_SERIAL_PKT_LEN) {
            rxfr->uih_data = ((unsigned char*)rxfr->uih_data) + i;
            rxfr->length   = MAX_SERIAL_PKT_LEN;
            len_vrfy       = len_vrfy - MAX_SERIAL_PKT_LEN;
            (*(cm->cb.callback_func))(TE_SENT_CB_TYPE | RX_COMPLETE_CB_TYPE, (void*)rxfr, (struct cmux_timer*)NULL);
        } else {
            rxfr->uih_data = ((unsigned char*)rxfr->uih_data) + i;
            rxfr->length   = (unsigned short int)len_vrfy;

            (*(cm->cb.callback_func))(TE_SENT_CB_TYPE | RX_COMPLETE_CB_TYPE, (void*)rxfr, (struct cmux_timer*)NULL);
            break;
        }
        i += MAX_SERIAL_PKT_LEN;
    }
    rxfr->uih_data = (void*)data_ptr_bkp;
    rxfr->length   = (unsigned short int)len_bkp;
}

int cmux_send_to_com_fragmented(struct cmuxlib* cmx, char* comname, unsigned char* rawptr, unsigned short len)
{
    struct cmuxlib* cm = (struct cmuxlib*)cmx;
    int             rc = PASS;
    unsigned short  i;

    for (i = 0; i < len;) {
        if ((len - i) > MAX_SERIAL_PKT_LEN) {
#if (defined(LLT_OS_VER))
            rc = (*(cm->cb.send_to_com))(comname, rawptr + i, MAX_SERIAL_PKT_LEN);
#else
            rc = (*(cm->cb.send_dl_data))(rawptr + i, MAX_SERIAL_PKT_LEN);
#endif
            if (rc == FAIL) {
                break;
            }
        } else {
#if (defined(LLT_OS_VER))
            rc = (*(cm->cb.send_to_com))(comname, rawptr + i, (len - i));
#else
            rc = (*(cm->cb.send_dl_data))(rawptr + i, (len - i));
#endif
            break;
        }
        i += MAX_SERIAL_PKT_LEN;
    }
    return rc;
}

int cmux_check_buff_enable(unsigned short maxlen, unsigned char jumbo_data_frame_mode,
    unsigned short cmux_buffered_frame_len, struct cmuxlib *cm)
{
    // Means buffering enabled.. and value is above MAX_CMUX_FRAME_SIZE
    // We check if we can stand one more iteration where one-full-CMUX frame can be filled in ?

    if (maxlen != 0 && jumbo_data_frame_mode == 0) {
        if ((maxlen - cmux_buffered_frame_len) < (unsigned short)MAX_CMUX_FRAME_SIZE(cm))/* lint !e574 */ {
            return PASS;
        }
    }
    return FAIL;
}

int cmux_check_frame_para_valid(struct frame* fr, struct cmuxlib *cm, struct cmux_cmd** cmd_node,
    unsigned char dlc, int *rc)
{
    /*
     * If frame type is MUX CLD ensure to proceed only if all DLCs are closed, else let it stay in Q
     * It will transmitted only if all DLCs are down, this is a special case handling
     */
    if (fr->f_type == CLD_CMD && cm->dlc_count != 1) {
        return FAIL;
    }

    if (cmux_struct_to_frame(&fr, cm) == FAIL) {
        /* struct to frame conversion not proper; delete the node */
        delete_head_cmd(cm, dlc);
        *cmd_node = cm->dlc_db[dlc]->cmd_list;
        *rc      = FAIL;
        return FAIL;
    }

    if (fr->raw_data == NULL) {
        /* struct to frame conversion not proper; delete the node */
        delete_head_cmd(cm, dlc);
        *cmd_node = cm->dlc_db[dlc]->cmd_list;
        *rc      = FAIL;
        return FAIL;
    }
    return PASS;
}

void cmux_kick_start_init(unsigned char *jumbo_data_frame_mode, unsigned int *templen, unsigned short *maxlen,
    unsigned char **cmux_buffered_frame, struct cmuxlib *cm)
{

    if (MAX_CMUX_FRAME_SIZE(cm) > MAX_SERIAL_PKT_LEN) {
        *jumbo_data_frame_mode = 1;
        *maxlen                = 0;
    }

    if (*maxlen == 0) {
        *templen             = MAX_CMUX_FRAME_SIZE(cm);
        *cmux_buffered_frame = cmux_alloc((int)*templen);
    } else {
        *templen             = *maxlen;
        *cmux_buffered_frame = cmux_alloc((int)*templen);
    }
}

int is_ready_to_send_com(const struct frame *fr,const  struct cmux_cmd* cmd_node, unsigned char jumbo_data_frame_mode,
    unsigned char filled_jumbo_data_frame, unsigned short maxlen)
{
    if ((fr->pf_bit == 1) || (cmd_node->next == NULL) || (jumbo_data_frame_mode && filled_jumbo_data_frame) ||
            (jumbo_data_frame_mode == 0 && maxlen == 0)) {
        return PASS;
    }
    return FAIL;
}

void cpy_buffer_to_send(unsigned short *cmux_buffered_frame_len, struct frame *fr, unsigned int templen,
    unsigned char* cmux_buffered_frame)
{
    int ret;
    if ((unsigned int)(*cmux_buffered_frame_len + fr->raw_len) <= templen && fr->raw_len > 0) {
        /*
         * First send whatever that was buffered.. and send the current node as well
         * Best way to do it is append to buffer and send it as "one" send to come
         */
        ret = memcpy_s((cmux_buffered_frame + *cmux_buffered_frame_len), templen - *cmux_buffered_frame_len,
                       fr->raw_data, fr->raw_len);
        CMUX_MEM_CHK_RTN_VAL(ret);
        *cmux_buffered_frame_len += fr->raw_len;
    }
}

void cmux_buffer_frame(unsigned char jumbo_data_frame_mode, unsigned short *cmux_buffered_frame_len,
    const struct frame *fr, unsigned int templen, unsigned char* cmux_buffered_frame)
{
    int ret;
    if (jumbo_data_frame_mode == 0) {
        if ((unsigned int)(*cmux_buffered_frame_len + fr->raw_len) <= templen && fr->raw_len > 0) {
            /*
             * Means pf bit zero and next is there..
             * Buffer now.. dont send it
             */
            ret = memcpy_s((cmux_buffered_frame + *cmux_buffered_frame_len), templen - *cmux_buffered_frame_len,
                           fr->raw_data, fr->raw_len);
            CMUX_MEM_CHK_RTN_VAL(ret);
            *cmux_buffered_frame_len += fr->raw_len;
        }
    }
}

int frame_process_result(int rc, struct cmuxlib *cm, struct frame *fr, struct cmux_cmd** cmd_node, unsigned char dlc)
{
    /* Inform result of processing the frame to upper layers */
    if (rc == PASS) {
        (*(cm->cb.callback_func))(TE_SENT_CB_TYPE | TX_COMPLETE_CB_TYPE, (void*)fr, &((*cmd_node)->ct));
        cm->dlc_db[dlc]->dlc_state |= (cm->frame_map.frdetails[fr->f_type].dlc_next_state);
    } else {
        (*(cm->cb.callback_func))(TE_SENT_CB_TYPE | TX_FAILED_CB_TYPE, (void*)fr, &((*cmd_node)->ct));
        cm->dlc_db[dlc]->dlc_state |= (cm->frame_map.frdetails[fr->f_type].dlc_next_state | DLC_COMMAND_FAILED);

        /* free up node and frame along with it as well */
        delete_head_cmd(cm, dlc);
        *cmd_node = cm->dlc_db[dlc]->cmd_list;
        /* rc already fail */
        return FAIL;
    }

    /* Poll bit set to 1 means, sending station has to poll/wait for response.. */
    if (fr->pf_bit == 1) {
        /* Needs response, so hold the node in list and kick-start the timer */
        /* Kick start the CMUX-timer (DLC, cmux_cmd*, timeout function) */
        cmux_start_timer(cm, dlc, &((*cmd_node)->ct), cmux_rxed_timer_expiry);
        return FAIL;  /* cannot continue for any further transmission */
    } else {
        delete_head_cmd(cm, dlc);
        *cmd_node = cm->dlc_db[dlc]->cmd_list;
    }
    return PASS;
}

int kick_start_valid_check(struct cmuxlib *cm, unsigned short maxlen, struct cmux_cmd** cmd_node, unsigned char dlc)
{
    down(&cm->stKickSem);
     /* First check DLC exists ? */
    if (cm->dlc_db[dlc] == NULL) {
        up(&cm->stKickSem);
        return FAIL;
    }

    *cmd_node = cm->dlc_db[dlc]->cmd_list;

    /* maxlen to be 0 or above MAX_CMUX_FRAME_SIZE */
    if (maxlen > MAX_SERIAL_PKT_LEN) {
        up(&cm->stKickSem);
        return FAIL;  /* wrong parameter range */
    }
    return PASS;
}

/*
 * Description: Dequeue from specific DLC commands one by one until pending flag is set Then transmit over Physical COM
 * (or COM associated with DLC-0) After transmit kick start the timer for that command (if pending) If no ack-required
 * free up that command move to next one maxlen defines the maximum frame size sent in one shot (maxlen DOES NOT
 * guarantee that caller will get that much size, it only means atleast we will get that much size and wont CROSS that
 * size. Which also means that we need to read-head the next frames (buffering) and give.. instead of giving frames one
 * by one... no_of_frames defines how many frames to send if maxlen = 31, no_of_frames = 1, only one is sent, until 31
 * bytes are hit (non-stop, means in between pf bit should not be set) if maxlen = 500, no_of_frames = 1, loop
 * accumulates up to 500, provided EOF-list or  pf bit should not be set (**Used in Target for first frame kick start)
 * if maxlen = 0, no_of_frames = 5, dequeues and sends 5 - frames in NON-buffered manner if maxlen = 0,no_of_frames = 0,
 * dequeues the full LIST,(**Typical use in Host for all control frames and Data) if maxlen = 31, no_of_frames = 0,
 * deques full LIST but buffers in chunks of 31 bytes (provided no pf bit frames in between are encountered)
 */
int cmux_kick_start_cmd(struct cmuxlib* cm, unsigned char dlc, unsigned short maxlen, unsigned short no_of_frames)
{
    struct cmux_cmd *cmd_node                = NULL;
    struct frame    *fr                      = NULL;
    int              rc                      = PASS;  /* return code */
    unsigned int     templen                 = 0;
    unsigned short   cmux_buffered_frame_len = 0;
    unsigned char   *cmux_buffered_frame     = NULL;
    unsigned char    jumbo_data_frame_mode   = 0;
    unsigned char    filled_jumbo_data_frame = 0;

    if (kick_start_valid_check(cm, maxlen, &cmd_node, dlc) == FAIL) {
        return FAIL;
    }

    no_of_frames = (no_of_frames == 0) ? 0xffff : no_of_frames; /* 0 means flush out the list */


    cmux_kick_start_init(&jumbo_data_frame_mode, &templen, &maxlen, &cmux_buffered_frame, cm);

    if (cmux_buffered_frame == NULL) {
        up(&cm->stKickSem);
        return FAIL;
    }

    /* if DLC has data and we dont overflow maxlen (space for ONE Single 31 byte frame is there proceed.. else dont..
     * cmux_buffered_frame_len is zeroed out, once a tranmission is done, and it starts buffering until the desired
     * no_of_frames is hit!
     */
    while (cmd_node && no_of_frames) {
        if (cmux_check_buff_enable(maxlen, jumbo_data_frame_mode, cmux_buffered_frame_len, cm) == PASS) {
            rc = cmux_send_to_com_fragmented(cm, cm->com_port, cmux_buffered_frame, cmux_buffered_frame_len);
            cmux_buffered_frame_len = 0;  /* RESET after send to com */
            no_of_frames--;
            if (no_of_frames == 0) {
                break;
            }
        }

        /* Get the frame to be transmitted */
        fr = cmd_node->fr;

        if (cmux_check_frame_para_valid(fr, cm, &cmd_node, dlc, &rc) == FAIL) {
            break;
        }

        /*
         * Jumbo frame support.. if we have filled to max,..send it
         * if (jumbo_data_frame_mode && fr->length == MAX_TX_DATA_PKT_LEN(cm))
         */
        filled_jumbo_data_frame = 1;

        /*
         * Conversion Done. Send it to COM (BUFFERED and non-buffered approach)
         * Suggest to aggregate data or not, for efficiency purposes
         * All  are termination conditions.. which means data MUST be sent to COM..
         * pf bit set, next is null or non-buffered mode (with cmux-frame size less than 672) or jumbo-data frame
         */
        if (is_ready_to_send_com(fr, cmd_node, jumbo_data_frame_mode, filled_jumbo_data_frame, maxlen) == PASS) {
            cpy_buffer_to_send(&cmux_buffered_frame_len, fr, templen, cmux_buffered_frame);
            /* READY to send to com */
            if (cmux_buffered_frame_len) {
                rc = cmux_send_to_com_fragmented(cm, cm->com_port, cmux_buffered_frame, cmux_buffered_frame_len);
                cmux_buffered_frame_len = 0;  /* RESET after send to com */
                no_of_frames--;
            }
        } else {
            /* lets buffer it ONLY if relevant like MAX_CMUX_FRAME_SIZE(cm) <= MAX_SERIAL_PKT_LEN ie,NOT jumb frame mode */
            cmux_buffer_frame(jumbo_data_frame_mode, &cmux_buffered_frame_len, fr, templen, cmux_buffered_frame);
        }

        if (frame_process_result(rc, cm, fr, &cmd_node, dlc) == FAIL) {
            break;
        }
    }

    if (cmux_buffered_frame_len) {
        rc = cmux_send_to_com_fragmented(cm, cm->com_port, cmux_buffered_frame, cmux_buffered_frame_len);
        cmux_buffered_frame_len = 0;  /* RESET after send to com */
    }

    cmux_free(cmux_buffered_frame);
    cmux_buffered_frame = NULL;

    up(&cm->stKickSem);
    return rc;
} /* cmux_kick_start_cmd */

int cmux_proc_diff_cmd_type(struct cmux_cmd *cmd_node, struct cmuxlib* cm, unsigned char dlc, unsigned int* dlc_state)
{
    unsigned int            *dlcx_state = NULL;
    unsigned char            dlcx;
    struct dlc_param_neg    *dlcpn_txed = NULL;
    struct modem_status_cmd *msc_txed   = NULL;
    int                      rc         = FAIL;

    if (cmd_node->fr->f_type == DLC_PN_CMD) {
        dlcpn_txed = (struct dlc_param_neg*)cmd_node->fr->uih_data;
        dlcx       = dlcpn_txed->dlci;
        if (!(cm->dlc_db[dlcx])) {
            return PASS;
        }
        dlcx_state = &(cm->dlc_db[dlcx]->dlc_state);
        SET_DLC_STATE(*dlcx_state, (DLC_COMMAND_FAILED | DLC_CLOSED));
        CLEAR_DLC_STATE(*dlcx_state, (~DLC_ESTABLISHED));
        delete_head_cmd(cm, 0);
        cmux_delete_dlc_entry(cm, dlcx);
        rc = FAIL;
    } else {
        if (cmd_node->fr->f_type == FCON_CMD) {
            dlc_state = &(cm->dlc_db[dlc]->dlc_state);
            SET_DLC_STATE(*dlc_state, (DLC_NON_CRIT_COMMAND_FAILED));
            if (CHECK_DLC_STATE(*dlc_state, UE_SENT_FCON_PENDING)) {
                CMUX_PRINT(CMUX_DEBUG_INFO, "FCON is pending, clear...\n");
                CLEAR_DLC_STATE(*dlc_state, (UE_SENT_FCON_RESP_RECVD));
            }
            delete_head_cmd(cm, dlc);
            rc = FAIL;
        } else if (cmd_node->fr->f_type == FCOFF_CMD) {
            dlc_state = &(cm->dlc_db[dlc]->dlc_state);
            SET_DLC_STATE(*dlc_state, (DLC_NON_CRIT_COMMAND_FAILED));
            if (CHECK_DLC_STATE(*dlc_state, UE_SENT_FCOFF_PENDING)) {
                CMUX_PRINT(CMUX_DEBUG_INFO, "FCOFF is pending, clear...\n");
                CLEAR_DLC_STATE(*dlc_state, (UE_SENT_FCOFF_RESP_RECVD));
            }
            delete_head_cmd(cm, dlc);
            rc = FAIL;
        } else if (cmd_node->fr->f_type == TEST_CMD) {
            dlc_state = &(cm->dlc_db[dlc]->dlc_state);
            SET_DLC_STATE(*dlc_state, (DLC_NON_CRIT_COMMAND_FAILED));
            delete_head_cmd(cm, dlc);
            rc = FAIL;
        } else if (cmd_node->fr->f_type == PSC_CMD) {
            dlc_state = &(cm->dlc_db[dlc]->dlc_state);
            SET_DLC_STATE(*dlc_state, (DLC_NON_CRIT_COMMAND_FAILED));
            delete_head_cmd(cm, dlc);
            rc = FAIL;
        } else if (cmd_node->fr->f_type == MSC_CMD) {
            dlc_state = &(cm->dlc_db[dlc]->dlc_state);
            msc_txed  = (struct modem_status_cmd*)cmd_node->fr->uih_data;
            dlcx      = msc_txed->dlci;
            if (dlcx > MAX_DLC) {
                return PASS;
            }
            dlcx_state = &(cm->dlc_db[dlcx]->dlc_state);
            SET_DLC_STATE(*dlcx_state, (DLC_NON_CRIT_COMMAND_FAILED));
            SET_DLC_STATE(*dlc_state, (DLC_NON_CRIT_COMMAND_FAILED));
            delete_head_cmd(cm, dlc);
            rc = FAIL;
        } else if (cmd_node->fr->f_type == SNC_CMD) {
            dlc_state = &(cm->dlc_db[dlc]->dlc_state);

            SET_DLC_STATE(*dlc_state, (DLC_NON_CRIT_COMMAND_FAILED));
            delete_head_cmd(cm, dlc);
            rc = FAIL;
        } else {
            /* Will come out as COMMAND RESPONSE pending + DLC Failed flag set */
            SET_DLC_STATE(*dlc_state, (DLC_COMMAND_FAILED | DLC_CLOSED));
            CLEAR_DLC_STATE(*dlc_state, (~DLC_ESTABLISHED));

            (*(cm->cb.callback_func))(DLC_STATE_CHANGE, (void*)(VOS_UINT_PTR)dlc, (struct cmux_timer*)NULL);

            cmux_delete_dlc_entry(cm, dlc);
            rc = FAIL;
        }
    }

    return rc;
}

int cmux_rxed_timer_expiry(struct cmuxlib* cm, unsigned char dlc)
{
    struct cmux_cmd *cmd_node  = NULL;
    unsigned int    *dlc_state = NULL;
    int              rc        = FAIL;

    if ((cm->cmux_status & ATCMUX_ON_MODE) != ATCMUX_ON_MODE) {
        cm->cb.callback_func(TIMEOUT_ON_ALL_RETRANSMISSION, NULL, (struct cmux_timer*)NULL);
        cmux_stop_timer(cm, dlc);
        return PASS;
    }

    if (cm->dlc_db[dlc] == NULL || cm->dlc_db[dlc]->cmd_list == NULL) {
        return FAIL;
    }

    cmd_node  = cm->dlc_db[dlc]->cmd_list;
    dlc_state = &(cm->dlc_db[dlc]->dlc_state);


    CMUX_PRINT(CMUX_DEBUG_INFO, "cmd_node->ct.retransmit_n2 = %d!\n", cmd_node->ct.retransmit_n2);
    /* Timed out case check for retransmission, Did not exceed maximum Re-tries */
    if (cmd_node->ct.retransmit_n2 > 1 /* 0 means Tx + 3 times, so 4 times */) {
        /* Re-transmit the packet in cmd_node over DLC-0 */
        cmd_node->ct.retransmit_n2--;

        /* Send for ALL the frames except MUX-CLD, as it is handled below in generic fashion */
        if (!(cm->dlc_count == 1 && cm->dlc_db[0]->cmd_list->fr->f_type == CLD_CMD)) {
            /* kick start will inform upper layer about retransmission */
            cmux_kick_start_cmd(cm, dlc, 0, 0);
        }
        rc = PASS;
    } else {
        rc =  cmux_proc_diff_cmd_type(cmd_node, cm, dlc, dlc_state);
        if (rc == PASS) {
            return FAIL;
        }
    }

    /* LAST DLC check: Put in common cleanup */
    if (cm->dlc_count == 1) {
        if (cm->dlc_db[0] != NULL && cm->dlc_db[0]->cmd_list != NULL &&
            cm->dlc_db[0]->cmd_list->fr->f_type == CLD_CMD) {
            cmux_kick_start_cmd(cm, 0, 0, 0);
        }
    }
    return rc;
} /* cmux_rxed_timer_expiry */

void cmux_get_default_dlc_param_nego(struct dlc_param_neg* dlcpn_default, unsigned char dlc)
{
    dlcpn_default->dlci       = dlc;
    dlcpn_default->priority   = DEFAULT_PRIORITY;
    dlcpn_default->conv_layer = CONVERGENCE_LAYER;

    dlcpn_default->muxparams.ack_timer_t1  = ACK_TIMER_T1_DEFAULT_VALUE;
    dlcpn_default->muxparams.frame_size_n1 = FRAME_SIZE_N1_DEFAULT_VALUE;
    dlcpn_default->muxparams.retransmit_n2 = RETRANSMIT_N2_DEFAULT_VALUE;
    dlcpn_default->muxparams.window_k      = WINDOW_K_DEFAULT_VALUE;
    dlcpn_default->muxparams.res_timer_t2  = RES_TIMER_T2_DEFAULT_VALUE;
}

int cmux_validate_te_ue_dlc_params(struct dlc_param_neg* dlcpn_tx, struct dlc_param_neg* dlcpn_rx,
                                   enum NEGOTIATION_CMD neg_cmd)
{
    if ((neg_cmd == MODIFY) && (dlcpn_tx->muxparams.frame_size_n1 != dlcpn_rx->muxparams.frame_size_n1)) {
        dlcpn_rx->muxparams.frame_size_n1 = dlcpn_tx->muxparams.frame_size_n1;
        dlcpn_rx->muxparams.ack_timer_t1  = dlcpn_tx->muxparams.ack_timer_t1;
        dlcpn_rx->muxparams.window_k      = dlcpn_tx->muxparams.window_k;
        dlcpn_rx->muxparams.retransmit_n2 = dlcpn_tx->muxparams.retransmit_n2;
        dlcpn_rx->dlci                    = dlcpn_tx->dlci;
        dlcpn_rx->priority                = dlcpn_tx->priority;
        dlcpn_rx->conv_layer              = dlcpn_tx->conv_layer;
        return PASS;
    } else if ((neg_cmd == COMPARE) && (dlcpn_tx->conv_layer == dlcpn_rx->conv_layer) &&
               (dlcpn_tx->dlci == dlcpn_rx->dlci) && (dlcpn_tx->priority == dlcpn_rx->priority) &&
               (dlcpn_tx->muxparams.ack_timer_t1 == dlcpn_rx->muxparams.ack_timer_t1) &&
               (dlcpn_tx->muxparams.frame_size_n1 == dlcpn_rx->muxparams.frame_size_n1) &&
               (dlcpn_tx->muxparams.retransmit_n2 == dlcpn_rx->muxparams.retransmit_n2) &&
               (dlcpn_tx->muxparams.window_k == dlcpn_rx->muxparams.window_k)) {
        return MATCH;
    } else {
        return NO_MATCH;
    }
} /* cmux_validate_te_ue_dlc_params */

/*
 * cmux_te_ue_rxed_dm: CTRL_DM Received
 * Merger of two functions: cmux_UE_rxed_DM AND cmux_TE_rxed_DM
 * Accept Flags: DLC_CLOSED or TE_PENDING_MASK
 * Reject Flags: NO_REJECT_FLAGS
 * Description: DM response received means its a NACK for SABM (or any command that is pending for a response) or TE
 *              sent a command to DLC which is "already down" However it does not necessarily mean that TE is pending
 *              for a response, DM can be sent for any command by UE It only means UE not willing to negotiate now, so
 *              if any pending commands are there clear them off and close-DLC This cannot be called in timeout case
 * cmux_UE_rxed_DM: CTRL_DM Received
 * Accept Flags: UE_SENT_DISC_CMD_UA_PENDING
 * Reject Flags: NO_REJECT_FLAGS
 * Description: Target receives DM response only if DISC cmd is sent from Target to a wrong DLC number or if DLC is
 *              already down in TE side This ideally should not be done by target, however for frame-reception point
 *              of view handler MUST be present Also if Target has sent DISC cmd sent to a right DLC, it is closed by
 *              UA response from host If DLC-down then target has sent for wrong DLC So only possible action is
 *              "if UE_DLC_ESTABLISHED or any DISC cmd send and response pending close timer and the DLC"
 */
int cmux_te_ue_rxed_dm(void* cmxlib, void* frm, unsigned char dlc)
{
    struct cmux_cmd *cmd_node  = NULL;
    struct frame    *txfr      = NULL;
    unsigned int    *dlc_state = NULL;

    struct cmuxlib  *cm   = (struct cmuxlib*)cmxlib;
    struct frame    *rxfr = (struct frame*)frm;

    /* CHECK not required, as its already done ahead.. for pclint it was done */
    if (cm->dlc_db[dlc] == NULL) {
        if ((cm->cmux_status & CMUX_TE_MODE) == CMUX_TE_MODE) {
            (*(cm->cb.callback_func))(UE_SENT_CB_TYPE | RX_COMPLETE_CB_TYPE, (void*)rxfr, (struct cmux_timer*)NULL);
        }
        return PASS;
    }
    cmd_node = cm->dlc_db[dlc]->cmd_list;

    if (cmd_node != NULL) {
        txfr = cmd_node->fr;
    } else {
        txfr = (struct frame*)NULL;
    }
    dlc_state = &(cm->dlc_db[dlc]->dlc_state);

    /* case 1: SABM sent DM received, UE not willing to start DLC */
    if ((*dlc_state) & cm->frame_map.pending_mask) {
        cmux_stop_timer(cm, dlc);
        if (txfr != NULL) {
            /* Removes DLC PN, and next command is moved ahead */
            delete_head_cmd(cm, txfr->dlc);
        }
    }
    /*
     * case 2: DM response received AND NO-response pending. ie,. Un-solicited DM response
     * DM response received for no-frame transmission on a Healthy DLC.
     */
    SET_DLC_STATE(*dlc_state, (DLC_CLOSED));
    CLEAR_DLC_STATE(*dlc_state, (~DLC_ESTABLISHED));

    /* Call upper layer and inform */
    if ((cm->cmux_status & CMUX_TE_MODE) == CMUX_TE_MODE) {
        (*(cm->cb.callback_func))(UE_SENT_CB_TYPE | RX_COMPLETE_CB_TYPE, (void*)rxfr, (struct cmux_timer*)NULL);
    }
    (*(cm->cb.callback_func))(DLC_STATE_CHANGE_CB_TYPE, (void*)(VOS_UINT_PTR)dlc, (struct cmux_timer*)NULL);
    return PASS;
} /* cmux_te_ue_rxed_dm */

int cmux_check_need_uih_data_frame(CMUX_FrTypeUint8 frame_type, void* muxframe)
{
    if (((frame_type == TEST_RESP) || (frame_type == MSC_RESP) || (frame_type == DLC_PN_RESP) ||
        (frame_type == SNC_RESP)) && (muxframe == NULL)) {
        return FAIL;
    }
    return PASS;
}

int cmux_check_unneed_uih_data_frame(CMUX_FrTypeUint8 frame_type, void* muxframe)
{
    if (((frame_type == CTRL_SABM) || (frame_type == CTRL_UA) || (frame_type == CTRL_DM) ||
        (frame_type == CTRL_DISC) || (frame_type == CTRL_DM) || (frame_type == FCON_RESP)) && (muxframe != NULL)) {
        return FAIL;
    }
    return PASS;
}
int cmux_check_send_frame(CMUX_FrTypeUint8 frame_type, void* muxframe)
{
    if (cmux_check_need_uih_data_frame(frame_type, muxframe) == FAIL) {
        return FAIL;
    }
    if (cmux_check_unneed_uih_data_frame(frame_type, muxframe) == FAIL) {
        return FAIL;
    }
    return PASS;
}

int cmux_ue_send_response(struct cmuxlib* cm, unsigned char dlc, CMUX_FrTypeUint8 frame_type, void* muxframe)
{
    struct frame  *fr       = (struct frame*)NULL;
    struct frame  *frm      = (struct frame*)NULL;
    DLCINFO        dlc_info;
    unsigned char *buf      = NULL;
    unsigned short length;
    int            rc;

    (void)memset_s((void *)&dlc_info, sizeof(DLCINFO), 0, sizeof(DLCINFO));
    if (cmux_check_send_frame(frame_type, muxframe) == FAIL) {
        return FAIL;
    }

    if (frame_type == TEST_RESP) {
        frm = (struct frame *)muxframe;
        buf = (unsigned char*)frm->uih_data;
        if (frm->length <= TEST_LEN_LIMIT) {
            length = frm->length - CMUX_FRAME_DEFAULT_TL_LEN;
        } else {
            length = frm->length - CMUX_FRAME_PLUS_TL_LEN;
        }
        dlc_info.dlc   = dlc;
        dlc_info.ftype = frame_type;
        rc = cmux_get_default_cmux_frame(cm, dlc_info, &fr, (unsigned char*)buf, length);
        if (rc == FAIL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_get_default_cmux_frame failed1!\n");
            return FAIL;
        }
    } else {
        dlc_info.dlc   = dlc;
        dlc_info.ftype = frame_type;
        rc = cmux_get_default_cmux_frame(cm, dlc_info, &fr, (unsigned char *) muxframe, 0);
        if (rc == FAIL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_get_default_cmux_frame failed2!\n");
            return FAIL;
        }
    }

    /* Fill in the values. */
    if (cmux_struct_to_frame(&fr, cm) == FAIL) {
        cmux_free_frame(fr);
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_struct_to_frame failed!\n");
        return FAIL;
    }

    rc = cmux_send_to_com_fragmented(cm, cm->com_port, fr->raw_data, fr->raw_len);
    if (rc == FAIL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_send_to_com_fragmented failed!\n");
        (*(cm->cb.callback_func))(TE_SENT_CB_TYPE | TX_FAILED_CB_TYPE, (void*)fr, (struct cmux_timer*)NULL);
    }
    cmux_free_frame(fr);  /* Free up the just-sent frame */
    return PASS;
} /* cmux_ue_send_response */

void mux_close_data_service(unsigned char Index)
{
    cmux_modem_msc_stru msc_stru;

    (void)memset_s((void *)&msc_stru, sizeof(cmux_modem_msc_stru), 0, sizeof(cmux_modem_msc_stru)); /*lint !e160 !e506 !e522*/

    msc_stru.OP_Dtr = 1;
    msc_stru.ucDtr  = 0;

    CMUX_PRINT(CMUX_DEBUG_INFO, "Index is %d, MSC dtr is %d!\r\n", Index, msc_stru.ucDtr);
    cmux_send_msc_status(Index, &msc_stru);
}

void cmux_ue_cmd_handler_type_sabm(struct frame* fr, struct cmuxlib* cm)
{
    struct delayed_work at_cmux_expiry_timer;

    at_cmux_expiry_timer = cmux_get_g_at_cmux_expiry_timer();

    if (fr->f_type == CTRL_SABM) {
        cmux_ue_send_response(cm, fr->dlc, CTRL_UA, NULL);
        cmux_create_dlc_entry(cm, fr->dlc);
        SET_DLC_STATE(cm->dlc_db[fr->dlc]->dlc_state, UE_RECVD_SABM_CMD_UA_SENT | DLC_ESTABLISHED);
        CLEAR_DLC_STATE(cm->dlc_db[fr->dlc]->dlc_state, (~DLC_CLOSED));

        /* UE side enable DLC-0 */
        if (fr->dlc == 0) {
            cm->cmux_status |= (ATCMUX_ON_MODE | CTRL_DLC_MODE);

            /* 如果SABM 0帧成功建立了通道，关闭timer定时器 */
            cmux_timer_end(&at_cmux_expiry_timer);
            cmux_set_g_at_cmux_expiry_timer(at_cmux_expiry_timer);
        }

        if ((fr->dlc != 0) && ((fr->dlc > 0) && (fr->dlc <= MAX_DLC_TGT))) {
            /* 创建非0的DLC通道上报管脚状态 */
            (*(cm->cb.callback_func))(DLC_ESTABLISH_COMPLETE_CB_TYPE, (void*)(VOS_UINT_PTR)fr->dlc,
                                      (struct cmux_timer*)NULL);
        }

        (*(cm->cb.callback_func))(DLC_STATE_CHANGE_CB_TYPE, (void*)(VOS_UINT_PTR)fr->dlc, (struct cmux_timer*)NULL);
    }
}

int cmux_ue_cmd_handler_diff_type(struct frame* fr, struct cmuxlib* cm)
{
    const struct framedetails *frame_details = NULL;
    unsigned int              *dlc_state = NULL;
    int                        rc = FAIL;

    if (fr->f_type == CTRL_SABM || fr->f_type == CTRL_DISC) {
        /* Handle for SABM (causes DLC entry creation) and DISC cmd on reception */
        if (cm->dlc_db[fr->dlc] == NULL) {
            /* DLC disabled (any frame type) or got DISC for non-existing DLC-entry -> DM */
            if ((fr->f_type == CTRL_DISC) && (cm->dlc_db[fr->dlc] == NULL)) {
                cmux_ue_send_response(cm, fr->dlc, CTRL_DM, NULL);
                rc = PASS;
                return rc;
            }
            /*
            * Got a frame for non-exisiting/created DLC, Check if its SABM,then UA by default, if DISC, then send
            * DM (as DLC is already down)
            */
            cmux_ue_cmd_handler_type_sabm(fr, cm);
        } else {
            /* DLC entry is made, which means its Enabled/created, if DISC/SABM send UA */
            dlc_state = &(cm->dlc_db[fr->dlc]->dlc_state);
            if (fr->f_type == CTRL_SABM) {
                SET_DLC_STATE(*dlc_state, UE_RECVD_SABM_CMD_UA_SENT | DLC_ESTABLISHED);
                CLEAR_DLC_STATE(*dlc_state, (~DLC_CLOSED));
            }

            if (fr->f_type == CTRL_DISC) {
                SET_DLC_STATE(*dlc_state, UE_RECVD_DISC_CMD_UA_SENT | DLC_CLOSED);
                CLEAR_DLC_STATE(*dlc_state, (~DLC_ESTABLISHED));
                mux_close_data_service(fr->dlc);
            }

            cmux_ue_send_response(cm, fr->dlc, CTRL_UA, NULL);
                (*(cm->cb.callback_func))(DLC_STATE_CHANGE, (void*)(VOS_UINT_PTR)fr->dlc, (struct cmux_timer*)NULL);
        }
        rc = PASS;
        return rc;
    }  /* Main check for SABM/DISC frame type */

    /* Now pending frames are Ctrl UA, Ctrl DM, MUX-commands, UIH-data, for which DLC-Entry MUST be created.. */
    if (cm->dlc_db[fr->dlc] == NULL) {
        /* to send dm response in case if the dlc0 is not created */
        cmux_ue_send_response(cm, fr->dlc, CTRL_DM, NULL);
        rc = PASS;
        return rc;
    }

    /*
     * Check accept & reject flags
     * Current DLC state && DLC states Allowed for that particular frame format
     * In main data path, no need to check for UE/TE its already loaded at init-time,one-time
     */

    frame_details = &(cm->frame_map.frdetails[fr->f_type]);

    if (((frame_details->accept_flag & cm->dlc_db[fr->dlc]->dlc_state) == 0) ||
        ((frame_details->reject_flag & cm->dlc_db[fr->dlc]->dlc_state) != 0)) {
        rc = FAIL;  /* Not allowed any further */
        return rc;
    }

    /* Now go for specific frame type handling...approach */
    rc = (*(frame_details->frame_rx_handler))(cm, fr, fr->dlc);
    return rc;
}

int cmux_ue_cmd_handler_check_para_valid(struct cmuxlib* cm, struct frame* fr, int* rc)
{
    if ((fr->f_type == CTRL_SABM) && (cm->dlc_db[0] == NULL) && (fr->dlc != 0)) {
        /* to send dm response in case if the dlc0 is not created */
        cmux_ue_send_response(cm, fr->dlc, CTRL_DM, NULL);
        *rc = PASS;
        return PASS;
    }

    if ((fr->f_type != CTRL_SABM) && (cm->dlc_db[0] == NULL)) {
        /* to send dm response in case if the dlc0 is not created */
        cmux_ue_send_response(cm, fr->dlc, CTRL_DM, NULL);
        *rc = PASS;
        return PASS;
    }
    if (fr->dlc > MAX_DLC_TGT) {
        cmux_ue_send_response(cm, fr->dlc, CTRL_DM, NULL);
        *rc = PASS;
        return PASS;
    }

    return FAIL;
}

/*
 * Description: called by cmux_Input_frame_to_cmux, which will try and settle the received frame In UE side (Target),
 * it handles SABM, DISC here itself as part of generic code itself unlike TE as its simpler if done here For other
 * frames handled through pointers For DLC param neg. it happens on DLC0, and DLC X not created DLC X create only on
 * reception of SBAM and if DLC is enabled
 */
int cmux_ue_cmd_handler(void* cmxlib, void* frm)
{
    int                        rc = FAIL;
    int                        rslt;

    struct cmuxlib* cm = (struct cmuxlib*)cmxlib;
    struct frame*   fr = (struct frame*)frm;

    if (cm == NULL) {
        return rc;
    }

    rslt =  cmux_ue_cmd_handler_check_para_valid(cm, fr, &rc);
    if (rslt == PASS) {
        return rc;
    }

    rc = cmux_ue_cmd_handler_diff_type(fr, cm);

    /* General DLC-clean up job.. */
    if ((cm->dlc_db[fr->dlc] != NULL) && (CHECK_DLC_STATE(cm->dlc_db[fr->dlc]->dlc_state, DLC_CLOSED))) {
        cmux_delete_dlc_entry(cm, fr->dlc);
    }

    if ((cm->cmux_status & ATCMUX_OFF_MODE) == ATCMUX_OFF_MODE) {
        set_cmux_close_port(TRUE);
    }

    /* LAST DLC check: Put in common cleanup */
    if (fr->f_type == CTRL_UA && cm->dlc_count == 1) {
        if ((cm->dlc_db[0] != NULL) && (cm->dlc_db[0]->cmd_list != NULL) &&
            (cm->dlc_db[0]->cmd_list->fr->f_type == CLD_CMD)) {
            cmux_kick_start_cmd(cm, 0, 0, 0);
        }
    }
    return rc;
} /* cmux_ue_cmd_handler */

/*
 * cmux_ue_rxed_ua: CTRL_UA Received
 * Accept Flags: UE_SENT_DISC_CMD_UA_PENDING or UE_DLC_ESTABLISHED
 * Reject Flags: UE_DLC_CLOSED
 * Target receives UA response only if DISC cmd is sent from Target
 * and it is the only command that is waiting for response etc,.
 */
int cmux_ue_rxed_ua(void* cmxlib, void* frm, unsigned char dlc)
{
    struct cmux_cmd *cmd_node  = NULL;
    struct frame    *txfr      = NULL;
    unsigned int    *dlc_state = NULL;

    struct cmuxlib  *cm   = (struct cmuxlib*)cmxlib;
    struct frame    *rxfr = (struct frame*)frm;

    if (cm == NULL) {
        return FAIL;
    }
    cmd_node = cm->dlc_db[dlc]->cmd_list;
    if (cmd_node == NULL) {
        return FAIL;
    } else {
        txfr = cmd_node->fr;
    }

    dlc_state = &(cm->dlc_db[dlc]->dlc_state);

    /* Check sanity of values */
    if (txfr == NULL) {  /* Ideally should not get txfr NULL */
        return FAIL;
    }
    if (cmd_node->fr->pf_bit != 1) {
        return FAIL;
    }

    /*
     * Normal reception
     * Check sanity of values
     */
    if (rxfr->dlc != txfr->dlc) {  /* this MUST never happen */
        return FAIL;
    }

    /* Stop the timer (as it is not required now) */
    cmux_stop_timer(cm, dlc);

    if (CHECK_DLC_STATE(*dlc_state, UE_SENT_DISC_CMD_UA_PENDING)) {
        CLEAR_DLC_STATE(*dlc_state, UE_SENT_DISC_CMD_UA_RECVD);
        CLEAR_DLC_STATE(*dlc_state, (~DLC_ESTABLISHED));
        SET_DLC_STATE(*dlc_state, (DLC_CLOSED));
        (*(cm->cb.callback_func))(DLC_STATE_CHANGE, (void*)(VOS_UINT_PTR)dlc, (struct cmux_timer*)NULL);
    }

    /* Free-up the txfr, and rxfr */
    delete_head_cmd(cm, dlc);

    /* Delete DLC entry is done in generic fashion */
    return PASS;
} /* cmux_ue_rxed_ua */

int cmux_ue_rxed_cld_resp(void* cmxlib, void* frm, unsigned char dlc)
{
    unsigned int *dlc_state = NULL;
    int           i;
    int           rc = FAIL;

    struct cmuxlib *cm   = (struct cmuxlib*)cmxlib;
    struct frame   *rxfr = (struct frame*)frm;

    if ((cm == NULL) || ((cm->cmux_status & ATCMUX_ON_MODE) != ATCMUX_ON_MODE) || (rxfr == NULL)) {
        return rc;
    }

    for (i = 1; i <= MAX_DLC_TGT; i++) {
        if (cm->dlc_db[i] != NULL) {
            /* check for flags,. estbalisehd etc,. and delete the dlc */
        }
    }

    if (cm->dlc_db[dlc] != NULL) {
        dlc_state = &(cm->dlc_db[dlc]->dlc_state);

        if (CHECK_DLC_STATE(*dlc_state, UE_SENT_MUX_CLD_RESP_PENDING)) {
            cmux_stop_timer(cm, dlc);
            SET_DLC_STATE(*dlc_state, (UE_SENT_MUX_CLD_RESP_RECVD | DLC_CLOSED));
            CLEAR_DLC_STATE(*dlc_state, (~DLC_ESTABLISHED));
            cm->cb.callback_func(DLC_STATE_CHANGE, (void*)0, (struct cmux_timer*)NULL);
        }

        /* Free-up */
        cm->cmux_status &= (~(ATCMUX_ON_MODE| CTRL_DLC_MODE));
    }

    return PASS;
} /* cmux_ue_rxed_cld_resp */

/*
 * cmux_ue_rxed_uihdata: UIH_DATA Received
 * Accept Flags: DLC_ESTABLISHED
 * Reject Flags: DLC_CLOSED or UE_PENDING_MASK
 * Means DLC !=0 and it is UIH-Data framed in to CMUX-frame
 * send data to consumer (watermark IDs of mapped DLC)
 */
int cmux_ue_rxed_uihdata(void* cmxlib, void* frm, unsigned char dlc)
{
    struct frame*   rxfr = (struct frame*)frm;
    struct cmuxlib* cm   = (struct cmuxlib*)cmxlib;
    int             rc;

    cmux_invoke_uihdata_callback(cm, rxfr);
    rc = PASS;
    return rc;
} /* cmux_ue_rxed_uihdata */

int cmux_ue_rxed_fcon_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    unsigned int   *dlc0_state = NULL;
    int             rc   = FAIL;
    struct cmuxlib *cm   = (struct cmuxlib*)cmxlib;
    struct frame   *rxfr = (struct frame*)frm;
    int             i;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }
    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);
    SET_DLC_STATE(*dlc0_state, (UE_RECVD_FCON_RESP_SENT));
    for (i = 1; i <= MAX_DLC_TGT; i++) {
        if (cm->dlc_db[i] == NULL) {
            continue;
        }
        cm->dlc_db[i]->msc.fc = 0;
    }
    cmux_ue_send_response(cm, rxfr->dlc, FCON_RESP, NULL);
    for (i = 0; i <= MAX_DLC_TGT; i++) {
        if (cm->dlc_db[i] == NULL) {
            continue;
        }

        if (cm->dlc_db[i]->cmd_list) {
            cmux_kick_start_cmd(cm, (unsigned char)i, 0, 0);
            cm->dlc_db[i]->stats.dlc_buffer_size = 0;
        }
    }
    return PASS;
} /* cmux_ue_rxed_fcon_cmd */

int cmux_ue_rxed_fcoff_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    unsigned int   *dlc0_state = NULL;
    int             rc   = FAIL;
    struct cmuxlib *cm   = (struct cmuxlib*)cmxlib;
    struct frame   *rxfr = (struct frame*)frm;
    int             i;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }
    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);
    SET_DLC_STATE(*dlc0_state, (UE_RECVD_FCOFF_RESP_SENT));

    for (i = 1; i <= MAX_DLC_TGT; i++) {
        if ((cm->dlc_db[i] == NULL) || (!(CHECK_DLC_STATE(cm->dlc_db[i]->dlc_state, DLC_ESTABLISHED)))) {
            continue;
        }
        cm->dlc_db[i]->msc.fc = 1;
    }

    cmux_ue_send_response(cm, rxfr->dlc, FCOFF_RESP, NULL);
    return PASS;
} /* cmux_ue_rxed_fcoff_cmd */

int cmux_ue_rxed_test_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    int             rc         = FAIL;
    unsigned int   *dlc0_state = NULL;
    struct cmuxlib *cm         = (struct cmuxlib*)cmxlib;
    struct frame*   rxfr       = (struct frame*)frm;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }

    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);

    SET_DLC_STATE(*dlc0_state, (UE_RECVD_TEST_CMD_RESP_SENT));
    cmux_ue_send_response(cm, rxfr->dlc, TEST_RESP, (void*)rxfr);

    return PASS;
} /* cmux_ue_rxed_test_cmd */

void cmux_msc_handler(struct modem_status_cmd* mscrxed)
{
    cmux_modem_msc_stru msc_stru;
    unsigned char       indexNum;

    if (mscrxed == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "mscrxed is NULL!\r\n");
        return;
    }

    (void)memset_s((void*)&msc_stru, sizeof(cmux_modem_msc_stru), 0, sizeof(cmux_modem_msc_stru)); /*lint !e160 !e506 !e522*/
    indexNum = mscrxed->dlci;

    msc_stru.OP_Dtr = 1;
    msc_stru.ucDtr  = mscrxed->rtc;

    cmux_send_msc_status(indexNum, &msc_stru);
}

int cmux_ue_rxed_msc_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    unsigned int            *dlc0_state = NULL;
    unsigned char            dlcx;
    int                      rc         = FAIL;
    struct modem_status_cmd *mscrxed    = NULL;
    struct cmuxlib*          cm         = (struct cmuxlib*)cmxlib;
    struct frame*            rxfr       = (struct frame*)frm;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }

    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);
    mscrxed    = (struct modem_status_cmd*)rxfr->uih_data;
    dlcx       = mscrxed->dlci;
    SET_DLC_STATE(*dlc0_state, (UE_RECVD_MSC_RESP_SENT));
    if (dlcx > MAX_DLC) {
        return FAIL;
    }
    if (cm->dlc_db[dlcx] == NULL) {
        return FAIL;
    }
    if (!((CHECK_DLC_STATE(cm->dlc_db[dlcx]->dlc_state, DLC_ESTABLISHED)))) {
        return FAIL;
    }
    cm->dlc_db[dlcx]->msc.fc = mscrxed->fc;

    /* 根据下发的DTR位的消息，做对应的处理 */
    cmux_msc_handler(mscrxed);

    /* Carefully modifiy other flags/fields in rx-frame */
    cmux_ue_send_response(cm, rxfr->dlc, MSC_RESP, (void*)mscrxed);
    if (cm->dlc_db[dlcx]->msc.fc == 0) {
        if (cm->dlc_db[dlcx]->cmd_list) {
            cmux_kick_start_cmd(cm, dlcx, 0, 0);
            cm->dlc_db[dlcx]->stats.dlc_buffer_size = 0;
        }
    }
    return PASS;
} /* cmux_ue_rxed_msc_cmd */

int cmux_ue_rxed_psc_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    int             rc = FAIL;
    unsigned int   *dlc0_state = NULL;
    struct cmuxlib *cm   = (struct cmuxlib*)cmxlib;
    struct frame*   rxfr = (struct frame*)frm;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }

    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);
    SET_DLC_STATE(*dlc0_state, (UE_RECVD_PSC_RESP_SENT));
    cmux_ue_send_response(cm, rxfr->dlc, PSC_RESP, NULL);

    if (cm->cb.set_power_cb) {
        rc = (int)cm->cb.set_power_cb();
        if (rc == FAIL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "cmux_set_power_status failed!\n");
            return rc;
        }
    }

    return PASS;
} /* cmux_ue_rxed_psc_cmd */
void cmux_fill_snc_db_data(unsigned int dlcx, struct snc_cmd* sncrxed, struct cmuxlib* cm,
    unsigned char *callFlag)
{
    unsigned int   i;
    if (sncrxed->s1 == 1) {
        cm->snc_db[dlcx].task = SNC_TYPE_DATA;
        *callFlag           = PASS;
    } else if ((sncrxed->s1 == 0) && (cm->snc_db[dlcx].task == SNC_TYPE_DATA)) {
        for (i = 1; i <= MAX_DLC_TGT; i++) {
            if (i == dlcx) {
                continue;
            }
            if (cm->snc_db[i].task == SNC_TYPE_AT) {
                continue;
            } else {
                break;
            }
        }
        CMUX_PRINT(CMUX_DEBUG_ERR, "sncrxed->s1 = %d !i = %d!\n", sncrxed->s1, i);
        if (i > MAX_DLC_TGT) {
            *callFlag           = PASS;
            cm->snc_db[dlcx].task = SNC_TYPE_AT_DATA;
        }
    }
    CMUX_PRINT(CMUX_DEBUG_INFO, "ucCallFlag = %d!\n", *callFlag);
}
void cmux_snc_callback_func(unsigned int dlcx, struct snc_cmd* sncrxed, struct cmuxlib* cm)
{
    struct snc_cmd snc_tmp = {0};
    unsigned int   i;

    for (i = 1; i <= MAX_DLC_TGT; i++) {
        if (i != dlcx) {
            if (cm->snc_db[dlcx].task == SNC_TYPE_DATA) {
                cm->snc_db[i].task = SNC_TYPE_AT;
            } else if (cm->snc_db[dlcx].task == SNC_TYPE_AT_DATA) {
                cm->snc_db[i].task = SNC_TYPE_AT_DATA;
            }
        }
    }
    if (cm->cb.callback_func) {
        snc_tmp.dlci = (unsigned char)dlcx;
        snc_tmp.s1   = sncrxed->s1;
        cm->cb.callback_func(AT_STATE_CHANGE, (void*)&snc_tmp, NULL);
    }
}

void cmux_snc_set_dlc_process(unsigned int dlcx, struct snc_cmd* sncrxed, struct cmuxlib* cm)
{
    /* flag that if the further process should be done */
    unsigned char ucCallFlag = FAIL;
    if (cm == NULL || sncrxed == NULL) {
        return;
    }
    if (!(dlcx > 0 && dlcx < (MAX_DLC_TGT + 1))) {
        return;
    }
    CMUX_PRINT(CMUX_DEBUG_INFO, "start!\n");
    CMUX_PRINT(CMUX_DEBUG_INFO, "dlcx = %d !sncrxed->s1 = %d !\n", dlcx, sncrxed->s1);

    cmux_fill_snc_db_data(dlcx, sncrxed, cm, &ucCallFlag);

    if (ucCallFlag) {
        cmux_snc_callback_func(dlcx, sncrxed, cm);
    }
} /* cmux_snc_set_dlc_process */


int cmux_ue_rxed_snc_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    unsigned int   *dlc0_state = NULL;
    struct snc_cmd *sncrxed    = NULL;
    struct cmuxlib *cm         = (struct cmuxlib*)cmxlib;
    struct frame*   rxfr       = (struct frame*)frm;
    unsigned char   dlcx;
    int             rc         = FAIL;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }

    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);
    sncrxed    = (struct snc_cmd*)rxfr->uih_data;
    dlcx       = sncrxed->dlci;
    SET_DLC_STATE(*dlc0_state, (UE_RECVD_SNC_RESP_SENT));

    if (cmux_get_active_ppp_pstask_dlc() != 0) {
        cmux_ue_send_response(cm, dlcx, NSC, NULL);
        return PASS;
    }

    if (dlcx > MAX_DLC_TGT) {
        cmux_ue_send_response(cm, dlcx, CTRL_DM, NULL);
    } else {
        cmux_snc_set_dlc_process(dlcx, sncrxed, cm);
        cmux_ue_send_response(cm, rxfr->dlc, SNC_RESP, (void*)sncrxed);
    }

    return PASS;
} /* cmux_ue_rxed_snc_cmd */

int cmux_ue_rxed_dlcpn_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    unsigned int         *dlc0_state = NULL;
    unsigned char         dlcx;
    int                   rc = FAIL;
    struct dlc_param_neg  dlcpn_default;
    struct dlc_param_neg *dlcpn_rxed = NULL;
    struct cmuxlib       *cm   = (struct cmuxlib*)cmxlib;
    struct frame         *rxfr = (struct frame*)frm;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }

    dlc0_state = &(cm->dlc_db[dlc]->dlc_state);
    dlcpn_rxed = (struct dlc_param_neg*)rxfr->uih_data;
    dlcx       = dlcpn_rxed->dlci;

    cmux_get_default_dlc_param_nego(&dlcpn_default, dlcx);

    cmux_validate_te_ue_dlc_params(&dlcpn_default, dlcpn_rxed, MODIFY);
    SET_DLC_STATE(*dlc0_state, (UE_RECVD_DLC_PN_RESP_SENT));

    /* Carefully modifiy other flags/fields in rx-frame */
    cmux_ue_send_response(cm, rxfr->dlc, DLC_PN_RESP, (void*)dlcpn_rxed);

    return PASS;
} /* cmux_ue_rxed_dlcpn_cmd */

int cmux_ue_rxed_fcon_fcoff_resp (void *cmxlib, void *frm, unsigned char dlc)
{
    struct cmuxlib  *cm   = (struct cmuxlib *) cmxlib;
    struct frame    *rxfr = (struct frame *) frm;
    unsigned int    *dlc0_state = NULL;
    int              stop_proc = 0;
    int              rc = FAIL;
    CMUX_PRINT(CMUX_DEBUG_INFO, "enter...\n");
    if ((cm == NULL) || ((cm->cmux_status & ATCMUX_ON_MODE) != ATCMUX_ON_MODE) || (rxfr == NULL)) {
        return rc;
    }
    dlc0_state = &(cm->dlc_db [dlc]->dlc_state);

    if (CHECK_DLC_STATE(*dlc0_state, UE_SENT_FCON_PENDING)) {
        CMUX_PRINT(CMUX_DEBUG_INFO, "FCON is pending, clear...\n");
        CLEAR_DLC_STATE (*dlc0_state, (UE_SENT_FCON_RESP_RECVD));
        stop_proc = 1;
    }
    if (CHECK_DLC_STATE(*dlc0_state, UE_SENT_FCOFF_PENDING)) {
        CMUX_PRINT(CMUX_DEBUG_INFO, "FCOFF is pending, clear...\n");
        CLEAR_DLC_STATE (*dlc0_state, (UE_SENT_FCOFF_RESP_RECVD));
        stop_proc = 1;
    }
    if (stop_proc) {
        cmux_stop_timer (cm, dlc);
        delete_head_cmd (cm, dlc);
    }

    return PASS;
} /* cmux_TE_rxed_FCON_RESP */

void cmux_set_dlc_state(unsigned char frame_code, unsigned int *dlc0_state)
{
    if (frame_code == CMUX_MUX_POWER_SAVING_CMD) {
        SET_DLC_STATE(*dlc0_state, (UE_RECVD_PSC_RESP_SENT));
    }
    if (frame_code == CMUX_MUX_TEST_CMD) {
        SET_DLC_STATE(*dlc0_state, (UE_RECVD_TEST_CMD_RESP_SENT));
    }
    if (frame_code == CMUX_MUX_MODEM_STATUS_CMD) {
        SET_DLC_STATE(*dlc0_state, (UE_RECVD_MSC_RESP_SENT));
    }
    if (frame_code == CMUX_MUX_SERVICE_NEGO_CMD) {
        SET_DLC_STATE(*dlc0_state, (UE_RECVD_SNC_RESP_SENT));
    }
    /* amrit:NSC support Begin */
    if (frame_code == CMUX_RLS_CMD) {
        SET_DLC_STATE(*dlc0_state, (UE_RECVD_SNC_RESP_SENT));
    }
    if (frame_code == CMUX_RPN_CMD) {
        SET_DLC_STATE(*dlc0_state, (UE_RECVD_SNC_RESP_SENT));
    }
}
int cmux_check_nsc_frame(struct cmuxlib* cm, struct frame* rxfr, unsigned char dlc)
{
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return FAIL;
    }

    if (dlc > MAX_DLC) {
        return FAIL;
    }
    return PASS;
}

int cmux_ue_nsc_handler(void* cmxlib, void* frm, unsigned char dlc)
{
    struct cmuxlib *cm   = (struct cmuxlib*)cmxlib;
    struct frame   *rxfr = (struct frame*)frm;
    int             rc   = FAIL;
    struct frame   *fr   = (struct frame*)NULL;
    DLCINFO         dlc_info;
    unsigned int   *dlc0_state = NULL;
    unsigned char   frame_code;

    (void)memset_s((void*)&dlc_info, sizeof(DLCINFO), 0, sizeof(DLCINFO));
    /* check cm done, ATCMUX on */
    rc = cmux_check_nsc_frame(cm, rxfr, dlc);
    if (rc == FAIL) {
        return FAIL;
    }

    /* 如果是收到的NSC响应，停止流控计时器 */
    if (cmux_get_g_nsc_received() == 1) {
        rc             = cmux_ue_rxed_fcon_fcoff_resp(cm, rxfr, dlc);
        cmux_set_g_nsc_received(0);
        return rc;
    }

    dlc0_state     = &(cm->dlc_db[dlc]->dlc_state);
    dlc_info.dlc   = dlc;
    dlc_info.ftype = NSC;
    if (cmux_get_default_cmux_frame(cm, dlc_info, &fr, 0, 0) == FAIL) {
        return FAIL;
    }

    frame_code = cm->frame_map.frdetails[rxfr->f_type].frame_code;
    rc         = (int)cmux_struct_to_frame(&fr, cm);
    if (rc == FAIL) {
        cmux_free_frame(fr);
        return FAIL;
    }

    if (cm->dlc_db[fr->dlc] == NULL) {
        cmux_ue_send_response(cm, rxfr->dlc, CTRL_DM, NULL);
    } else {
        cmux_set_dlc_state(frame_code, dlc0_state);
        /* amirt:NSC support END */
        rc = cmux_send_to_com_fragmented(cm, cm->com_port, fr->raw_data, fr->raw_len);

        if (rc == PASS) {
            (*(cm->cb.callback_func))(TE_SENT_CB_TYPE| TX_COMPLETE_CB_TYPE, (void*)fr, (struct cmux_timer*)NULL);
        } else {
            (*(cm->cb.callback_func))(TE_SENT_CB_TYPE| TX_FAILED_CB_TYPE, (void*)fr, (struct cmux_timer*)NULL);
        }
    }
    cmux_free_frame(fr);
    return PASS;
} /* cmux_ue_nsc_handler */

int cmux_ue_rxed_cld_cmd(void* cmxlib, void* frm, unsigned char dlc)
{
    unsigned int   *dlc_state = NULL;
    int             rc = FAIL;
    int             i;

    struct cmuxlib *cm   = (struct cmuxlib*)cmxlib;
    struct frame   *rxfr = (struct frame*)frm;

    /* check cm done, ATCMUX on */
    if (!((cm != NULL) && ((cm->cmux_status & ATCMUX_ON_MODE) == ATCMUX_ON_MODE) && (rxfr != NULL))) {
        return rc;
    }
    dlc_state = &(cm->dlc_db[dlc]->dlc_state);

    for (i = 1; i <= MAX_DLC_TGT; i++) {
        if (cm->dlc_db[i] != NULL) {
            SET_DLC_STATE(cm->dlc_db[i]->dlc_state, DLC_CLOSED);
            CLEAR_DLC_STATE(cm->dlc_db[i]->dlc_state, (~DLC_ESTABLISHED));
            mux_close_data_service((unsigned char)i); /*lint !e10*/
            cmux_delete_dlc_entry(cm, (unsigned char)i);
            (*(cm->cb.callback_func))(DLC_STATE_CHANGE, (void*)(VOS_INT_PTR)i, (struct cmux_timer*)NULL);
        }
    }

    if (cm->dlc_count == 1) {
        SET_DLC_STATE(*dlc_state, DLC_CLOSED);
        CLEAR_DLC_STATE(*dlc_state, (~DLC_ESTABLISHED));
        cmux_ue_send_response(cm, rxfr->dlc, CLD_RESP, NULL);
        cm->cmux_status &= (~(ATCMUX_ON_MODE | CTRL_DLC_MODE));
        cm->cmux_status |= (ATCMUX_OFF_MODE);
        (*(cm->cb.callback_func))(DLC_STATE_CHANGE, (void*)(VOS_UINT_PTR)dlc, (struct cmux_timer*)NULL);
    }

    return PASS;
} /* cmux_ue_rxed_cld_cmd */

#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */