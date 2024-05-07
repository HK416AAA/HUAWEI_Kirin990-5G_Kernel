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

#ifndef PS_LOG_IMSA_FILE_ID_DEFINE_H
#define PS_LOG_IMSA_FILE_ID_DEFINE_H

#include "ps_log_file_id_base.h"
#include "ps_log_print.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum
{
    /* imsa\imsa\main */
    PS_FILE_ID_IMSA_MAIN_C = IMSA_TEAM_FILE_ID,
    PS_FILE_ID_IMSA_PROC_TIMER_MSG_C,

    /* imsa\imsa\api */
    PS_FILE_ID_IMSA_API_C,
    PS_FILE_ID_IMSA_IMS_API_C,
    PS_FILE_ID_IMSA_UDPM_API_C,

    /* imsa\imsa\at */
    PS_FILE_ID_IMSA_AT_MAIN_C,
    PS_FILE_ID_IMSA_AT_LOG_FILTER_C,
    PS_FILE_ID_IMSA_AT_PROC_CFG_MSG_C,
    PS_FILE_ID_IMSA_AT_PROC_FUSIONCALL_MSG_C,
    PS_FILE_ID_IMSA_AT_PROC_IMS_DMDYN_MSG_C,
    PS_FILE_ID_IMSA_AT_PROC_IMS_ECALL_MSG_C,
    PS_FILE_ID_IMSA_AT_PROC_IMS_IPCAP_MSG_C,
    PS_FILE_ID_IMSA_AT_PROC_IMS_MSG_C,
    PS_FILE_ID_IMSA_AT_PROC_IMS_RTT_MSG_C,
    PS_FILE_ID_IMSA_AT_PROC_IMS_TIMER_MSG_C,
    PS_FILE_ID_IMSA_AT_SEND_MSG_C,

    /* imsa\imsa\call */
    PS_FILE_ID_IMSA_CALL_API_C,
    PS_FILE_ID_IMSA_CALL_CCM_LOG_FILTER_C,
    PS_FILE_ID_IMSA_CALL_CHR_C,
    PS_FILE_ID_IMSA_CALL_COMM_C,
    PS_FILE_ID_IMSA_CALL_DMS_EMC_ACQUIRE_PROCESS_C,
    PS_FILE_ID_IMSA_CALL_DMS_EMC_CONTROL_C,
    PS_FILE_ID_IMSA_CALL_DMS_EMC_SELECT_STRATEGY_C,
    PS_FILE_ID_IMSA_CALL_ENTITY_C,
    PS_FILE_ID_IMSA_CALL_HIFI_DATA_LOST_C,
    PS_FILE_ID_IMSA_CALL_IMS_LOG_FILTER_C,
    PS_FILE_ID_IMSA_CALL_INIT_C,
    PS_FILE_ID_IMSA_CALL_MAIN_C,
    PS_FILE_ID_IMSA_CALL_PRE_DIAL_ACTION_FSM_C,
    PS_FILE_ID_IMSA_CALL_PRE_DIAL_ACTION_FSM_EMC_C,
    PS_FILE_ID_IMSA_CALL_PRE_DIAL_ACTION_FSM_NRM_C,
    PS_FILE_ID_IMSA_CALL_PRE_DIAL_CHECK_EMC_C,
    PS_FILE_ID_IMSA_CALL_PRE_DIAL_CHECK_NRM_C,
    PS_FILE_ID_IMSA_CALL_PRE_DIAL_COMM_C,
    PS_FILE_ID_IMSA_CALL_PRE_DIAL_MANAGER_C,
    PS_FILE_ID_IMSA_CALL_PROC_CCM_MSG_C,
    PS_FILE_ID_IMSA_CALL_PROC_CCM_MSG_DTMF_C,
    PS_FILE_ID_IMSA_CALL_PROC_CCM_MSG_ORIG_C,
    PS_FILE_ID_IMSA_CALL_PROC_CCM_MSG_SRVCC_C,
    PS_FILE_ID_IMSA_CALL_PROC_CCM_MSG_SUPS_C,
    PS_FILE_ID_IMSA_CALL_PROC_FUSIONCALL_MSG_C,
    PS_FILE_ID_IMSA_CALL_PROC_IMS_MSG_C,
    PS_FILE_ID_IMSA_CALL_PROC_IMS_MSG_CALL_STATUS_C,
    PS_FILE_ID_IMSA_CALL_PROC_IMS_MSG_DISCONNECT_C,
    PS_FILE_ID_IMSA_CALL_PROC_IMS_MSG_ERR_C,
    PS_FILE_ID_IMSA_CALL_PROC_IMS_MSG_OK_C,
    PS_FILE_ID_IMSA_CALL_PROC_INTER_MSG_C,
    PS_FILE_ID_IMSA_CALL_PROC_INTRA_MSG_C,
    PS_FILE_ID_IMSA_CALL_PROC_NRRC_MSG_C,
    PS_FILE_ID_IMSA_CALL_PROC_SRV_MSG_C,
    PS_FILE_ID_IMSA_CALL_REDIAL_C,
    PS_FILE_ID_IMSA_CALL_RELEASE_C,
    PS_FILE_ID_IMSA_CALL_SEND_CCM_MSG_C,
    PS_FILE_ID_IMSA_CALL_SEND_HIFI_MSG_C,
    PS_FILE_ID_IMSA_CALL_SEND_IMS_MSG_C,
    PS_FILE_ID_IMSA_CALL_SEND_INTER_MSG_C,
    PS_FILE_ID_IMSA_CALL_SEND_INTRA_MSG_C,
    PS_FILE_ID_IMSA_CALL_TIMER_C,
    PS_FILE_ID_IMSA_PROC_MMA_MSG_C,
    PS_FILE_ID_IMSA_CALL_PROC_SCC_MSG_C,

    /* imsa\imsa\comm */
    PS_FILE_ID_IMSA_CHR_C,
    PS_FILE_ID_IMSA_COMM_CHECK_MSG_LEN_C,
    PS_FILE_ID_IMSA_COMM_LOG_FILTER_C,
    PS_FILE_ID_IMSA_IMS_COMM_C,
    PS_FILE_ID_IMSA_PRINT_MSG_C,
    PS_FILE_ID_IMSA_PROC_CAP_ADAPTOR_C,
    PS_FILE_ID_IMSA_PUBLIC_C,
    PS_FILE_ID_IMSA_SEND_MSG_C,
    PS_FILE_ID_IMSA_TIMER_C,

    /* imsa\imsa\conn */
    PS_FILE_ID_IMSA_CONN_BEARER_INFO_NOTIFY_C,
    PS_FILE_ID_IMSA_CONN_CHR_C,
    PS_FILE_ID_IMSA_CONN_DELETE_PDP_INFO_C,
    PS_FILE_ID_IMSA_CONN_DRB_CALL_C,
    PS_FILE_ID_IMSA_CONN_DRB_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_DRB_LTE_C,
    PS_FILE_ID_IMSA_CONN_DRB_NR_C,
    PS_FILE_ID_IMSA_CONN_DRB_REG_C,
    PS_FILE_ID_IMSA_CONN_DRB_SMS_C,
    PS_FILE_ID_IMSA_CONN_DRB_TIMER_C,
    PS_FILE_ID_IMSA_CONN_DRB_USSD_C,
    PS_FILE_ID_IMSA_CONN_INIT_C,
    PS_FILE_ID_IMSA_CONN_MAIN_C,
    PS_FILE_ID_IMSA_CONN_MANAGER_C,
    PS_FILE_ID_IMSA_CONN_MANAGER_PDP_MEDIA_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_MANAGER_PDP_SIGNAL_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_MANAGER_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PROC_DSDS_MSG_C,
    PS_FILE_ID_IMSA_CONN_NIC_PDP_INFO_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_NIC_PDP_INFO_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_ACTIVE_CNF_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_PDN_ACTIVE_CNF_CELLULAR_HO_C,
    PS_FILE_ID_IMSA_CONN_PDN_ACTIVE_CNF_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_ACTIVE_IND_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_PDN_ACTIVE_REJ_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_PDN_ACTIVE_REJ_CELLULAR_HO_C,
    PS_FILE_ID_IMSA_CONN_PDN_ACTIVE_REJ_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_CN_MODE_CHANGE_IND_C,
    PS_FILE_ID_IMSA_CONN_PDN_DEACTIVE_CNF_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_PDN_DEACTIVE_CNF_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_DEACTIVE_IND_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_PDN_DEACTIVE_IND_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_HO_REQ_C,
    PS_FILE_ID_IMSA_CONN_PDN_IPV6_INFO_IND_C,
    PS_FILE_ID_IMSA_CONN_PDN_MODIFY_IND_CELLULAR_C,
    PS_FILE_ID_IMSA_CONN_PDN_MODIFY_IND_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_READY_IND_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_REESTABLISH_REQ_CELLUALR_C,
    PS_FILE_ID_IMSA_CONN_PDN_RELEASE_REQ_C,
    PS_FILE_ID_IMSA_CONN_PDN_RELEASE_REQ_HO_C,
    PS_FILE_ID_IMSA_CONN_PDN_RELEASE_REQ_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PDN_SETUP_REQ_C,
    PS_FILE_ID_IMSA_CONN_PDN_SETUP_REQ_WIFI_C,
    PS_FILE_ID_IMSA_CONN_PROC_CDS_MSG_C,
    PS_FILE_ID_IMSA_CONN_PROC_TAF_CALL_MSG_C,
    PS_FILE_ID_IMSA_CONN_PROC_UDPM_MSG_C,
    PS_FILE_ID_IMSA_CONN_PROC_WIFI_ERROR_IND_MSG_C,
    PS_FILE_ID_IMSA_CONN_PROC_WIFI_LOCATION_UPDATE_MSG_C,
    PS_FILE_ID_IMSA_CONN_SAVE_MODIFY_PDP_INFO_C,
    PS_FILE_ID_IMSA_CONN_SEND_AT_MSG_C,
    PS_FILE_ID_IMSA_CONN_SEND_INTRA_MSG_C,
    PS_FILE_ID_IMSA_CONN_SEND_RNIC_MSG_C,
    PS_FILE_ID_IMSA_CONN_SUPL_C,
    PS_FILE_ID_IMSA_CONN_THROT_C,
    PS_FILE_ID_IMSA_CONN_TIMER_C,
    PS_FILE_ID_IMSA_CONN_URSP_MANAGEMENT_C,
    PS_FILE_ID_IMSA_CONN_URSP_PROC_AT_C,
    PS_FILE_ID_IMSA_CONN_URSP_PROC_NRMM_MSG_C,
    PS_FILE_ID_IMSA_CONN_URSP_PROC_NV_C,
    PS_FILE_ID_IMSA_CONN_URSP_PROC_SLICE_CAUSE_C,
    PS_FILE_ID_IMSA_CONN_URSP_PROC_THROT_MSG_C,
    PS_FILE_ID_IMSA_PROC_CDS_MSG_C,
    PS_FILE_ID_IMSA_PROC_ESM_MSG_C,
    PS_FILE_ID_IMSA_PROC_LMM_MSG_C,
    PS_FILE_ID_IMSA_PROC_LPDCP_MSG_C,
    PS_FILE_ID_IMSA_PROC_NRMM_MSG_C,

    /* imsa\imsa\dsds */
    PS_FILE_ID_IMSA_DSDS_EVENT_C,
    PS_FILE_ID_IMSA_DSDS_INIT_C,
    PS_FILE_ID_IMSA_DSDS_MAIN_C,
    PS_FILE_ID_IMSA_DSDS_PROC_INTRA_MSG_C,
    PS_FILE_ID_IMSA_DSDS_PROC_RRM_MSG_C,
    PS_FILE_ID_IMSA_DSDS_PUBLIC_C,
    PS_FILE_ID_IMSA_DSDS_RESOURCE_APPLY_CNF_C,
    PS_FILE_ID_IMSA_DSDS_RESOURCE_CHECK_C,
    PS_FILE_ID_IMSA_DSDS_RESOURCE_OCCUPY_C,
    PS_FILE_ID_IMSA_DSDS_RESOURCE_RECOVER_C,
    PS_FILE_ID_IMSA_DSDS_SEND_INTRA_MSG_C,
    PS_FILE_ID_IMSA_DSDS_SEND_RRM_MSG_C,
    PS_FILE_ID_IMSA_DSDS_TIMER_C,

    /* msa\imsa\nvim */
    PS_FILE_ID_IMSA_NVIM_COMM_C,
    PS_FILE_ID_IMSA_NVIM_INIT_C,
    PS_FILE_ID_IMSA_NVIM_MAIN_C,
    PS_FILE_ID_IMSA_NVIM_OM_C,
    PS_FILE_ID_IMSA_NVIM_READ_IMS_NV_C,
    PS_FILE_ID_IMSA_NVIM_READ_IMSA_NV_PART1_C,
    PS_FILE_ID_IMSA_NVIM_READ_IMSA_NV_PART2_C,
    PS_FILE_ID_IMSA_NVIM_READ_IMSA_NV_PART3_C,
    PS_FILE_ID_IMSA_SIM_COMM_C,
    PS_FILE_ID_IMSA_SIM_DECODE_C,
    PS_FILE_ID_IMSA_SIM_INIT_C,
    PS_FILE_ID_IMSA_SIM_MAIN_C,
    PS_FILE_ID_IMSA_SIM_PROC_AT_MSG_C,
    PS_FILE_ID_IMSA_SIM_PROC_PIH_MSG_C,
    PS_FILE_ID_IMSA_SIM_PROC_USIM_MSG_C,
    PS_FILE_ID_IMSA_SIM_READ_SIM_FILE_C,
    PS_FILE_ID_IMSA_SIM_SEND_IMS_MSG_C,
    PS_FILE_ID_IMSA_SIM_SEND_USIM_MSG_C,
    PS_FILE_ID_IMSA_SIM_SET_IMPU_REC_POS_C,

    /* imsa\imsa\reg */
    PS_FILE_ID_IMSA_REG_ADDR_PAIR_ADD_C,
    PS_FILE_ID_IMSA_REG_ADDR_PAIR_C,
    PS_FILE_ID_IMSA_REG_ADDR_PAIR_GET_C,
    PS_FILE_ID_IMSA_REG_ADDR_PAIR_REMOVE_C,
    PS_FILE_ID_IMSA_REG_API_C,
    PS_FILE_ID_IMSA_REG_CHR_C,
    PS_FILE_ID_IMSA_REG_COMM_C,
    PS_FILE_ID_IMSA_REG_DSDS_EVENT_C,
    PS_FILE_ID_IMSA_REG_FSM_C,
    PS_FILE_ID_IMSA_REG_FSM_DEREGING_C,
    PS_FILE_ID_IMSA_REG_FSM_PENDING_C,
    PS_FILE_ID_IMSA_REG_FSM_REGISTERED_C,
    PS_FILE_ID_IMSA_REG_FSM_REGISTERING_C,
    PS_FILE_ID_IMSA_REG_FSM_ROLLING_BACK_C,
    PS_FILE_ID_IMSA_REG_FSM_WAIT_RETRY_C,
    PS_FILE_ID_IMSA_REG_FSM_WAIT_RF_RESOURCE_C,
    PS_FILE_ID_IMSA_REG_INIT_C,
    PS_FILE_ID_IMSA_REG_MAIN_C,
    PS_FILE_ID_IMSA_REG_PROC_IMS_MSG_C,
    PS_FILE_ID_IMSA_REG_PROC_INTRA_MSG_C,
    PS_FILE_ID_IMSA_REG_PROC_TIMER_MSG_C,
    PS_FILE_ID_IMSA_REG_SEND_INTER_MSG_C,
    PS_FILE_ID_IMSA_REG_SEND_INTRA_MSG_C,
    PS_FILE_ID_IMSA_REG_STRATEGY_C,
    PS_FILE_ID_IMSA_REG_SUBSCRIBE_C,
    PS_FILE_ID_IMSA_REG_THROD_C,
    PS_FILE_ID_IMSA_STK_LOG_FILTER_C,
    PS_FILE_ID_IMSA_PROC_STK_MSG_C,

    /* imsa\imsa\service */
    PS_FILE_ID_IMSA_CELL_NET_C,
    PS_FILE_ID_IMSA_SERVICE_API_C,
    PS_FILE_ID_IMSA_SERVICE_CALL_API_C,
    PS_FILE_ID_IMSA_SERVICE_CHR_C,
    PS_FILE_ID_IMSA_SERVICE_COMM_C,
    PS_FILE_ID_IMSA_SERVICE_CONFIG_IMS_C,
    PS_FILE_ID_IMSA_SERVICE_INIT_C,
    PS_FILE_ID_IMSA_SERVICE_MAIN_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_AT_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_BASTET_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_CALL_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_CONN_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_DSDS_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_IMS_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_INTER_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_REG_FAIL_ACTION_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_REG_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_SERVICE_CHANGE_IND_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_SMS_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_SUPL_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_UDPM_DOMAIN_SWITCH_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_UDPM_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_PROVIDER_C,
    PS_FILE_ID_IMSA_SERVICE_SEND_IMS_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_SEND_INTER_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_SEND_INTRA_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_SEND_UDPM_MSG_C,
    PS_FILE_ID_IMSA_SERVICE_TIMER_C,
    PS_FILE_ID_IMSA_PROC_MSCC_MSG_C,
    PS_FILE_ID_IMSA_PROC_LRRC_MSG_C,
    PS_FILE_ID_IMSA_WIFI_NET_C,
    PS_FILE_ID_IMSA_SERVICE_DM_C,
    PS_FILE_ID_IMSA_SERVICE_PROC_REG_REPORT_C,

    /* imsa\imsa\sms */
    PS_FILE_ID_IMSA_SMS_COMM_C,
    PS_FILE_ID_IMSA_SMS_PROC_DSDS_MSG_C,
    PS_FILE_ID_IMSA_SMS_INIT_C,
    PS_FILE_ID_IMSA_SMS_MAIN_C,
    PS_FILE_ID_IMSA_SMS_PROC_AT_MSG_C,
    PS_FILE_ID_IMSA_SMS_PROC_IMS_MSG_ERROR_C,
    PS_FILE_ID_IMSA_SMS_PROC_IMS_MSG_OK_C,
    PS_FILE_ID_IMSA_SMS_PROC_IMS_MSG_SMS_C,
    PS_FILE_ID_IMSA_SMS_PROC_INTRA_MSG_C,
    PS_FILE_ID_IMSA_SMS_PROC_TAF_MSG_C,
    PS_FILE_ID_IMSA_SMS_SEND_IMS_MSG_C,
    PS_FILE_ID_IMSA_SMS_SEND_INTER_MSG_C,
    PS_FILE_ID_IMSA_SMS_SEND_INTRA_MSG_C,
    PS_FILE_ID_IMSA_SMS_TIMER_C,

    /* imsa\imsa\ussd */
    PS_FILE_ID_IMSA_USSD_COMM_C,
    PS_FILE_ID_IMSA_USSD_INIT_C,
    PS_FILE_ID_IMSA_USSD_MAIN_C,
    PS_FILE_ID_IMSA_USSD_PROC_DSDS_MSG_C,
    PS_FILE_ID_IMSA_USSD_PROC_IMS_MSG_C,
    PS_FILE_ID_IMSA_USSD_PROC_INTRA_MSG_C,
    PS_FILE_ID_IMSA_USSD_PROC_SSA_MSG_C,
    PS_FILE_ID_IMSA_USSD_SEND_IMS_MSG_C,
    PS_FILE_ID_IMSA_USSD_SEND_INTRA_MSG_C,
    PS_FILE_ID_IMSA_USSD_SEND_INTER_MSG_C,
    PS_FILE_ID_IMSA_USSD_SEND_SSA_MSG_C,
    PS_FILE_ID_IMSA_USSD_TIMER_C,

    /* imsa/imsva/ */
    PS_FILE_ID_IMSVA_IMS_API_C,
    PS_FILE_ID_IMSVA_IMS_HIFI_LOG_FILTER_C,
    PS_FILE_ID_IMSVA_IMS_IMSA_LOG_FILTER_C,
    PS_FILE_ID_IMSVA_HIFI_DATA_MANAGER_C,
    PS_FILE_ID_IMSVA_INIT_C,
    PS_FILE_ID_IMSVA_MAIN_C,
    PS_FILE_ID_IMSVA_PROC_HIFI_MSG_C,
    PS_FILE_ID_IMSVA_PROC_MSP_MSG_C,
    PS_FILE_ID_IMSVA_SEND_HIFI_MSG_C,
    PS_FILE_ID_IMSVA_SEND_IMSA_MSG_C,
    PS_FILE_ID_IMSVA_TIMER_C,

    /* imsa/udpm/ */
    PS_FILE_ID_UDPM_API_C,
    PS_FILE_ID_UDPM_CA_MAIN_C,
    PS_FILE_ID_UDPM_CA_MANAGER_C,
    PS_FILE_ID_UDPM_CA_PROC_CFG_MSG_C,
    PS_FILE_ID_UDPM_CA_PROC_CS_STATUS_IND_C,
    PS_FILE_ID_UDPM_CA_PROC_DEREG_MSG_C,
    PS_FILE_ID_UDPM_CA_PROC_HIFI_MSG_C,
    PS_FILE_ID_UDPM_CA_PROC_PS_STATUS_IND_C,
    PS_FILE_ID_UDPM_CA_PROC_SIGNAL_QUALITY_C,
    PS_FILE_ID_UDPM_CA_PROC_START_STOP_C,
    PS_FILE_ID_UDPM_DOMAIN_API_C,
    PS_FILE_ID_UDPM_DOMAIN_COMM_C,
    PS_FILE_ID_UDPM_DOMAIN_FIND_CANDIDATED_C,
    PS_FILE_ID_UDPM_DOMAIN_INIT_C,
    PS_FILE_ID_UDPM_DOMAIN_JUDGE_SWITCH_C,
    PS_FILE_ID_UDPM_DOMAIN_MAIN_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_IMS_SERVICE_CHANGE_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_SERVICE_STATUS_C,
    PS_FILE_ID_UDPM_DOMAIN_SELECT_C,
    PS_FILE_ID_UDPM_DOMAIN_SELECT_DUAL_C,
    PS_FILE_ID_UDPM_DOMAIN_SELECT_LTE_ONLY_C,
    PS_FILE_ID_UDPM_DOMAIN_SELECT_WIFI_ONLY_C,
    PS_FILE_ID_UDPM_DOMAIN_SEND_IMSA_MSG_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_START_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_STOP_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_ERROR_IND_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_DEACTIVE_IND_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_STATE_IND_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_HIFI_EVENT_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_TUNNEL_HOLD_IND_C,
    PS_FILE_ID_UDPM_DOMAIN_CELLULAR_SIGNAL_QUALITY_C,
    PS_FILE_ID_UDPM_DOMAIN_CFG_SMS_CAP_C,
    PS_FILE_ID_UDPM_DOMAIN_CFG_IMS_SWITCH_C,
    PS_FILE_ID_UDPM_DOMAIN_CFG_IMS_SWITCH_OFF_C,
    PS_FILE_ID_UDPM_DOMAIN_CFG_IMS_SWITCH_ON_C,
    PS_FILE_ID_UDPM_DOMAIN_CFG_RCS_SWITCH_C,
    PS_FILE_ID_UDPM_DOMAIN_CFG_PREFER_DOMAIN_C,
    PS_FILE_ID_UDPM_DOMAIN_CFG_VOICE_DOMAIN_C,
    PS_FILE_ID_UDPM_DOMAIN_DEREG_ESM_DEREG_C,
    PS_FILE_ID_UDPM_DOMAIN_DEREG_MSCC_DEREG_C,
    PS_FILE_ID_UDPM_DOMAIN_CELLULAR_START_C,
    PS_FILE_ID_UDPM_DOMAIN_CELLULAR_STOP_C,
    PS_FILE_ID_UDPM_DOMAIN_NW_STATUS_1X_REG_STATUS_C,
    PS_FILE_ID_UDPM_DOMAIN_NW_STATUS_AREA_LOST_C,
    PS_FILE_ID_UDPM_DOMAIN_NW_STATUS_PS_SRV_CHANGE_C,
    PS_FILE_ID_UDPM_DOMAIN_NW_STATUS_PS_SRV_CHANGE_DUAL_DOMAIN_C,
    PS_FILE_ID_UDPM_DOMAIN_NW_STATUS_CS_SRV_CHANGE_C,
    PS_FILE_ID_UDPM_DOMAIN_NW_STATUS_GPRS_STATE_IND_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_START_STOP_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_READ_USIM_CMP_IND_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_BEAR_CHANGE_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_SWITCH_RESULT_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_SRV_READY_IND_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_ACTION_RESULT_C,
    PS_FILE_ID_UDPM_DOMAIN_SELECT_CS_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_POWER_ON_WIFI_PREFER_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_HO_ATTACH_C,
    PS_FILE_ID_UDPM_DOMAIN_WIFI_KEEP_ALIVE_C,
    PS_FILE_ID_UDPM_DOMAIN_IMSA_SWITCH_ANTI_PINGPONG_C,
    PS_FILE_ID_UDPM_DOMAIN_SEND_SCC_MSG_C,
    PS_FILE_ID_UDPM_MAIN_C,
    PS_FILE_ID_UDPM_NVIM_C,
    PS_FILE_ID_UDPM_PUBLIC_C,
    PS_FILE_ID_UDPM_WA_MAIN_C,
    PS_FILE_ID_UDPM_WA_MANAGER_C,
    PS_FILE_ID_UDPM_WA_PROC_PDN_DEACTIVE_C,
    PS_FILE_ID_UDPM_WA_PROC_POWER_ON_OFF_C,
    PS_FILE_ID_UDPM_WA_PROC_TUNNEL_HOLD_C,
    PS_FILE_ID_UDPM_WA_PROC_WIFI_ERROR_IND_C,
    PS_FILE_ID_UDPM_WA_PROC_WIFI_STATE_IND_C,

    PS_FILE_ID_SCC_INIT_C,
    PS_FILE_ID_SCC_MAIN_C,
    PS_FILE_ID_SCC_MANAGER_C,
    PS_FILE_ID_SCC_PROC_CCM_MSG_C,
    PS_FILE_ID_SCC_PROC_IMSA_MSG_C,
    PS_FILE_ID_SCC_PROC_DSM_MSG_C,
    PS_FILE_ID_SCC_PROC_TIMER_MSG_C,
    PS_FILE_ID_SCC_PROC_UDPM_MSG_C,
    PS_FILE_ID_SCC_SEND_MSG_C,
    PS_FILE_ID_SCC_TIMER_C,

    IMSA_FILE_ID_BUTT = IMSA_TEAM_FILE_ID + 0x400,
} IMSA_FILE_ID_DEFINE_ENUM;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif