/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

/*
 * 版权所有（c）华为技术有限公司  2012-2019
 * 功能描述: Hids勾包消息结构体定义
 */
#include "product_config.h"

#if (VOS_OS_VER != VOS_WIN32)
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
#include "taf_app_xsms_interface.h"
#include "taf_xsms_stk_interface.h"
#include "TafXpdsXsmsInterface.h"
#include "xcc_sms_pif.h"
#include "cas_1x_control_main_nas_pif.h"
#include "TafXsmsInterMsg.h"
#include "at_xpds_interface.h"
#include "xcc_xpds_pif.h"
#include "TafXpdsSndInternalMsg.h"
#include "CnasXregSndInternalMsg.h"
#include "CnasXregListProc.h"
#include "CnasXregProcess.h"
#include "CnasHsdSndInternalMsg.h"
#include "CnasHsdMntn.h"
#include "CnasMntn.h"

#include "xsd_xreg_pif.h"
#include "cas_1x_access_ctrl_proc_nas_pif.h"
#include "TafXpdsCtx.h"
#include "TafXpdsMntn.h"
#include "TafXpdsFsmMainTbl.h"
#include "TafXpdsFsmUpLinkCtrlTbl.h"
#include "TafXpdsFsmCpLinkCtrlTbl.h"
#include "TafXpdsCtx.h"
#include "TafXpdsFsmPdeProcTbl.h"
#include "TafXpdsFsmCpPdeProcTbl.h"
#include "TafXpdsEncode.h"
#include "nas_om_trans.h"
#endif

#include "at_oam_interface.h"
#include "at_csimagent.h"
#include "cbpa_ps_interface.h"
#include "taf_app_mma.h"
#include "at_mta_interface.h"
#include "mta_comm_interface.h"
#include "phy_oam_interface.h"
#include "rrc_pam_interface.h"
#include "spy_system.h"

#include "emat_msg_proc.h"

#include "pam_om.h"

#include "pb_base_phone.h"
#include "pb_data_phone.h"
#include "pb_msg_phone.h"

#include "pih_api.h"
#include "pih_at_cnf.h"
#include "pih_at_req.h"
#include "pih_channel.h"
#include "pih_chr_proc.h"
#include "pih_ind_report.h"
#include "pih_key_file.h"
#include "pih_list.h"
#include "pih_msg_proc.h"
#include "pih_protect_reset.h"
#include "pih_sci_chg.h"
#include "pih_vsim_icc.h"

#include "ps_regrpt.h"

#include "sc_comm.h"
#include "sc_comm_ctx.h"
#include "sc_ctx.h"
#include "sc_factory.h"
#include "sc_personalization.h"
#include "sc_public_key.h"

#include "si_app_pih.h"
#include "si_app_stk.h"
#include "si_app_emat.h"
#include "si_pih.h"
#include "si_stk.h"
#include "si_pb.h"
#include "si_emat.h"
#include "si_ccore_api.h"

#include "stk_api.h"
#include "stk_at_printf.h"
#include "stk_call.h"
#include "stk_cmd_proc.h"
#include "stk_codec.h"
#include "stk_custom_imsi_change.h"
#include "stk_envelope.h"
#include "stk_menu.h"
#include "stk_msg_proc.h"
#include "stk_provide_location.h"
#include "stk_refresh.h"
#include "stk_sms.h"
#include "stk_ss.h"
#include "stk_sysinfo.h"
#include "stk_timer.h"

#include "usimm_ps_comm_interface.h"
#include "usimm_ps_interface.h"
#include "usimm_apdu.h"
#include "usimm_api.h"
#include "usimm_auth.h"
#include "usimm_base_file.h"
#include "usimm_card_app_info.h"
#include "usimm_cdma_auth.h"
#include "usimm_channel.h"
#include "usimm_clear_var.h"
#include "usimm_comm_init.h"
#include "usimm_csim_init.h"
#include "usimm_dl.h"
#include "usimm_drv.h"
#include "usimm_fdn.h"
#include "usimm_file_pool.h"
#include "usimm_hotplug.h"
#include "usimm_icc_init.h"
#include "usimm_isim_init.h"
#include "usimm_log.h"
#include "usimm_msg_def.h"
#include "usimm_msg_proc.h"
#include "usimm_pin.h"
#include "usimm_print.h"
#include "usimm_protect_reset.h"
#include "usimm_query_set.h"
#include "usimm_read_file.h"
#include "usimm_roaming.h"
#include "usimm_sat.h"
#include "usimm_sci_config.h"
#include "usimm_select_file.h"
#include "usimm_sim_init.h"
#include "usimm_status.h"
#include "usimm_uicc_init.h"
#include "usimm_uim_init.h"
#include "usimm_update_file.h"
#include "usimm_usim_init.h"
#include "usimm_vsim.h"
#include "usimm_vsim_auth.h"

#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED) || defined(CBT_ENABLED)
#include "pc_ue_cal.h"
#include "bbic_cal.h"
#ifdef MODEM_FUSION_VERSION
#include "bbic_phy_cal_ext.h"
#include "ct_mntn.h"
#include "fess_lfcal_comm_shared.h"
#include "fess_lfcal_interface.h"
#include "pc_ue_bt.h"
#else
#include "pc_ue_bt.h"
#endif
#include "nascbtinterface.h"
#if (FEATURE_EASYRF == FEATURE_ON)
#ifndef FEATURE_M533_LNR_BT_DISABLE /* g200平台单独定义屏蔽相关引用 */
#include "nrrc_nphy_interface.h"
#include "nmac_nphy_interface.h"
#include "nbt_listmode.h"
#ifdef MODEM_FUSION_VERSION
#include "phy_oam_interface.h"
#include "nbt_comm.h"
#include "lrrc_phy_interface.h"
#include "mac_lphy_interface.h"
#if (FEATURE_LTEV == FEATURE_ON)
#include "vrrc_vphy_interface.h"
#include "vmac_phy_interface.h"
#include "vphy_nbt_interface.h"
#endif
#include "nbt_nr_call.h"
#include "fess_flpm_pam_interface.h"
#endif /* end of MODEM_FUSION_VERSION */
#endif /* end of FEATURE_M533_LNR_BT_DISABLE */
#endif /* end of FEATURE_EASYRF == FEATURE_ON */

#endif /* end of defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED) || defined(CBT_ENABLED) */

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
typedef struct {
    VOS_MSG_HEADER
    TAF_XPDS_MNTN_MSG_ID_ENUM_UINT32 msgId;
    VOS_UINT32                       moduleId;
    TAF_XPDS_FSM_ID_ENUM_UINT32      curFsmId;
    union {
        TAF_XPDS_L1_STA_ENUM_UINT32              l1MainState;
        TAF_XPDS_LINK_CTRL_SUBSTA_ENUM_UINT32    l2LinkState;
        TAF_XPDS_CP_LINK_Ctrl_SUBSTA_ENUM_UINT32 l2CpLinkState;
        TAF_XPDS_L2_MPC_PROC_STA_ENUM_UINT32     l2MpcState;
        TAF_XPDS_L2_PDE_PROC_STA_ENUM_UINT32     l2PdeState;
        TAF_XPDS_CP_L2_PDE_PROC_STA_ENUM_UINT32  l2CpPdeState;
    } unCurFsmState;
} TAF_XPDS_ELF_MNTN_LOG_FSM_INFO_STRU;

typedef struct {
    VOS_MSG_HEADER
    TAF_XSMS_HOOK_MSG_TYPE_ENUM_UINT32 msgId;
    VOS_UINT32                         errorType;
} TAF_XSMS_ELF_INT_3RD_PARTY_ERR_STRU;

/*
 * 结构说明: 当前MO STATE的状态
 */
typedef struct {
    VOS_MSG_HEADER
    TAF_XSMS_HOOK_MSG_TYPE_ENUM_UINT32 msgId;
    TAF_XSMS_MO_FSM_ID_ENUM_UINT32     curMoState;
} TAF_XSMS_ELF_INT_CUR_MO_FSM_STRU;

/*
 * 结构说明: 当前MT STATE的状态
 */
typedef struct {
    VOS_MSG_HEADER
    TAF_XSMS_HOOK_MSG_TYPE_ENUM_UINT32 msgId;
    TAF_XSMS_MT_FSM_ID_ENUM_UINT32     curMtState;
} TAF_XSMS_ELF_INT_CUR_MT_FSM_STRU;

/*
 * 结构说明: 短信发送或者接收的数据结构
 */
typedef struct {
    VOS_MSG_HEADER
    TAF_XSMS_HOOK_MSG_TYPE_ENUM_UINT32 msgId;
    VOS_UINT32                         teleserviceID;
    VOS_UINT32                         serviceCategory;
    VOS_UINT32                         replySeq;
    TAF_XSMS_Addr                 addr;
    VOS_UINT32                         bearerDataLen;
    VOS_UINT8                          bearerData[TAF_XSMS_BEARER_DATA_MAX + 1];
    TAF_XSMS_MsgId               xsmsMsgIdData;
    TAF_XSMS_BdMsgUserData     userData;
} TAF_XSMS_ELF_INT_SMS_CONTENT_STRU;

/*
 * 结构说明: 短信发送或者接收的TL_ACK数据结构
 */
typedef struct {
    VOS_MSG_HEADER
    TAF_XSMS_HOOK_MSG_TYPE_ENUM_UINT32 msgId;
    TAF_XSMS_CauseCode           causeCode;
} TAF_XSMS_ELF_INT_TL_ACK_STRU;

/*
 * 结构说明: 1x信令加解密勾包的消息结构
 */
typedef struct {
    VOS_MSG_HEADER
    TAF_XSMS_HOOK_MSG_TYPE_ENUM_UINT32  msgId;
    TAF_SDC_1XSigEncryptionInfo st1xSigEncrypInfo;
} TAF_XSMS_ELF_INT_1X_SIG_ENCRYPTION_INFO_STRU;

/*
 * 结构说明: 发送DBM消息勾包的消息结构
 */
typedef struct {
    VOS_MSG_HEADER
    TAF_XSMS_HOOK_MSG_TYPE_ENUM_UINT32 msgId;
    TAF_XSMS_DataBurstMsg       dataBurMsg;
} TAF_XSMS_ELF_INT_SND_DATA_BURST_MSG_STRU;

#endif

#if defined(PAM_BT_ENABLED) || defined(CBT_ENABLED)
#if (FEATURE_EASYRF == FEATURE_ON)
#ifndef FEATURE_M533_LNR_BT_DISABLE
typedef struct {
    VOS_MSG_HEADER
    NRRC_NPHY_PifMsgTypeUint32         msgId;
    VOS_UINT16                         opId;
    ModemIdUint16               modemId;
    VOS_UINT8                          reserved[2];
    VOS_UINT8                          cfgCGNum; /**< number of configured CGs (astCGCfgInfo) */
    VOS_UINT8                          cfgCCNum; /**< @ARRAY_TYPE_TO_ASN NRRC_NPHY_DEDI_RL_CFG_PER_CC_STRU */
    NRRC_NPHY_CgCfg                    cGCfgInfo[NRRC_NPHY_MAX_CG_NUM];       /**< complete config */
    NRRC_NPHY_DediCgInfoForUl          dediCGInfoForUl[NRRC_NPHY_MAX_CG_NUM]; /**< Dl info needed by UL */
    NRRC_NPHY_SlotFormatIndCfg         sFIInfo[NRRC_NPHY_MAX_SERVING_CC_NUM]; /**< SFI info */
    VOS_UINT8                          cCPara[4];                             /**< @ARRAY_NUM_TO_ASN ucCfgCCNum */
} NOSIG_NPHY_DEDI_RL_SETUP_REQ_STRU;

typedef struct {
    VOS_UINT8                              cellIdx;
    VOS_UINT8                              cgIdx; /**< index of astCGCfgInfo/astDlCGInfoForUl for this CG */
    /**< first UL: with PUCCH if PUCCH included in this CC, or w/o PUCCH if not included @ARRAY_TYPE_TO_ASN NRRC_NPHY_DEDI_RL_CFG_PER_CC_UL_STRU */
    NRRC_NPHY_ParaValidUint8               firstUlValid;
    /**< second UL: no PUCCH @ARRAY_TYPE_TO_ASN NRRC_NPHY_DEDI_RL_CFG_WO_PUCCH_PER_CC_UL_STRU */
    NRRC_NPHY_ParaValidUint8               secondUlValid;
    NRRC_NPHY_DediRlCfgPerCcComm           dediRlCommInfo; /**< comm part of Dedicated RL */
    NRRC_NPHY_DediRlCfgPerCcDl             dediRlDlInfo;   /**< DL part of Dedicated RL */
    NRRC_NPHY_DediRlCfgPerCcDl             dediRlUlInfo;   /**< @ARRAY_NUM_TO_ASN enFirstUlValid enSecondUlValid */
} NOSIG_NPHY_DEDI_RL_CFG_PER_CC_STRU;
#endif
#endif
#endif

#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED)
VOS_VOID PAM_RfDebugStruDef(VOS_VOID)
{
#ifdef MODEM_FUSION_VERSION
    /* RFDEBUG */
    BBIC_CAL_RF_DEBUG_TRX_REQ_STRU       bbicCalRfDebugTrxReqStru;
    BBIC_CAL_RF_DEBUG_RM_REQ_STRU        bbicCalRfDebugRmReqStru;
    BBIC_CAL_RF_DEBUG_RM_CNF_STRU        bbicCalRfDebugRmCnfStru;
    BBIC_CAL_RF_DEBUG_TX_REQ_STRU        bbicCalRfDebugTxReqStru;
    BBIC_CAL_RF_DEBUG_TX_CNF_STRU        bbicCalRfDebugTxCnfStru;
    BBIC_CAL_RF_DEBUG_RX_REQ_STRU        bbicCalRxDebugRxReqStru;
    BBIC_CAL_RF_DEBUG_RX_CNF_STRU        bbicCalRxDebugRxCnfStru;
    BBIC_CAL_RF_DEBUG_RM_NOTIFY_REQ_STRU bbicCalRxDebugRmNotifyStru;
    BBIC_LPHY_CalTxModuReq               bbicCalLteTxModuReqStru;
    PHY_BBIC_CalMsgCnf                   bbicCalPhyMsgCnf;
    BBIC_WPHY_CalTxModuReq               bbicCalWcdmaTxModuReqStru;
    BBIC_NPHY_CalTxModuReq               bbicCalNphyTxModuReqStru;
    BBIC_GPHY_CalTxReq                   bbicCalGphyTxReqStru;
    BBIC_CAL_GphyTxApcReq                bbicCalGphyTxApcReqStru;
    BBIC_CAL_GphyTxApcInd                bbicCalGphyTxApcIndStru;
    BBIC_PHY_CalRxRssiReq                bbicCalRxRssiReqStru;
    PHY_BBIC_CalRxRssiInd                bbicCalRxRssiIndStru;
#endif
}

VOS_VOID PAM_MtStruDef(VOS_VOID)
{
#ifdef MODEM_FUSION_VERSION
    /* MT */
    BBIC_CAL_DCXO_TX_REQ bbicCalDcxoTxReqBbic;
    AT_BBIC_CAL_MT_RX_REQ atBbicCalMtRxReq;
    AT_BBIC_CAL_MT_RX_RSSI_REQ atBbicCalMtRxRssiReq;
    AT_BBIC_CAL_MT_TX_REQ atBbicCalMtTxReq;
    BBIC_CAL_AT_MT_TRX_CNF bbicCalAtMtTrxCnf;
    BBIC_CAL_AT_MT_TX_PD_IND bbicCalAtMtTxPdInd;
    BBIC_CAL_AT_MT_RX_RSSI_IND bbicCalAtMtRxRssiInd;
#endif
}

VOS_VOID PAM_FusionStruDef(VOS_VOID)
{
#ifdef MODEM_FUSION_VERSION
    /* CT */
    CT_MntnAlgVccLut ctMntnAlgVccLut;
    BBIC_CAL_PaVccCompReq bbicCalPaVccCompressStartReq;
    BBIC_CAL_PaVccLutSampleInd bbicCalPaVccExtractSampleInd;
    BBA_CAL_TrxMntnMsg trxFastCalMntn;
    BBIC_CAL_DpdResultIndHids bbicCalDpdResultIndHids;
    BBIC_CAL_DpdStartReqHids bbicCalDpdStartReqHids;

    PAM_RfDebugStruDef();

    PAM_MtStruDef();
#endif
}

#ifdef MODEM_FUSION_VERSION
VOS_VOID PAM_HidsHfCtStruDef(VOS_VOID)
{
    /* HFCAL */
    FESS_HFCAL_ReloadNvReq hfReloadNvReq;
    FESS_HFCAL_RxDcocReq hfRxDcocReq;
    FESS_HFCAL_RxAdcCalReq hfRxAdcReq;
    FESS_HFCAL_RxFiiqReq hfRxFiiqReq;
    FESS_HFCAL_TrxFdiqReq hfTrxFdiqReq;
    FESS_HFCAL_SelfCalReq hfSelfCalReq;
    FESS_HFCAL_TxStartReq hfTxStartReq;
    FESS_HFCAL_TxApcSetReq hfTxApcSetReq;
    FESS_HFCAL_TxStopReq hfTxStopReq;
    FESS_HFCAL_TempReq hfTempQueryReq;
    FESS_HFCAL_RxStartReq hfRxStartReq;
    FESS_HFCAL_RxAgcSetReq hfRxAgcSetReq;
    FESS_HFCAL_RxStopReq hfRxStopReq;
    FESS_HFCAL_RxSyncReq hfRxSyncReq;
    FESS_HFCAL_TxDpdParaLoadReq hfDpdLoadParaReq;
}
#endif

VOS_VOID PAM_FUSION_NrStruSeparateDef(VOS_VOID)
{
#ifdef MODEM_FUSION_VERSION
    /* BT */
#ifdef PAM_BT_ENABLED
    NBT_TaskEndInd                          nbtTaskIndMsg;
    NRNOSIG_BT_TestCnf                      nbtTestSendMsg;
    NBT_CallCnf                             nbtCallCnf;
    NRNOSIG_BT_TestInd                      nrnosigTestInd;
    NBT_LPHY_TxTestReq                      nbtTxTestReq;
    NBT_CallReqStru                         nbtCallReqStru;
#endif
    NBT_NPHY_CfgCompleteNtf                 nbtCfgCompleteNtf;
    LRRC_LPHY_DdrConfigMsgReq               lrrcDdrConfigMsgReq;
    LRRC_LPHY_MbsfnSubframeCfgReq           lrrcMbsfnSubframeCfgReq;
    LRRC_LPHY_CampedMaincellReq             lrrcCampedMaincellReq;
    LRRC_LPHY_PdschBchSetupReq              lrrcPdschBchSetupReq;
    LRRC_LPHY_PdschBchReleaseReq            lrrcPdschBchReleaseReq;
    LRRC_LPHY_PbchReleaseReq                lrrcPbchReleaseReq;
    LRRC_LPHY_PbchSetupReq                  lrrcPbchSetupReq;
    LRRC_LPHY_CellSearchingReq              lrrcCellSearchingReq;
    NBT_LPHY_PresetNetOptReq                nbtPresetNetOptReq;
    LRRC_LPHY_TddConfigNtf                  lrrcTddConfigNtf;
    LMAC_LPHY_RaAckNtf                      lmacRaAckNtf;
    LMAC_LPHY_CrntiConfigNtf                lmacCrntiNtf;
    LRRC_LPHY_ChangeToConnectedNtf          lrrcChangeToConnectedNtf;
    LMAC_LPHY_AccessReq                     lmacAccessReq;
    LMAC_LPHY_RarConfigReq                  lmacRarConfigReq;
    LRRC_LPHY_MeasInConnectedIntraReq       lrrcMeasInConnectedIntraReq;
    NMAC_NPHY_AccessReq                     nmacAccessReq;
    NMAC_NPHY_RarConfigReq                  nmacRarConfigReq;
    NMAC_NPHY_RaAckNtf                      nmacRaAckNtf;
    NBT_NPHY_CaGroupReq                     nbtCaGroupReq;
    LRRC_LPHY_RelallReq                     lrrcRelallReq;
    NBT_LPHY_BlerTestReq                    nbtBlerTestReq;
    NBT_LPHY_RssiTestReq                    nbtRssiTestReq;
#if (FEATURE_LTEV == FEATURE_ON)
    NBT_VPHY_SetPowerReq                    nbtSetPowerReq;
#endif
    OAM_PHY_SetWorkModeReq                  oamSetWorkModeReq;
    NBT_NPHY_BeamReq                        nbtBeamReq;
    NBT_NPHY_HoldReq                        nbtHoldReq ;
    FESS_FLPM_LpcReq                        fessFlpmLpcReq;

    /* TRX ISR STEP */
    BBIC_CAL_MSG_HDR_STRU bbicTrxStepReq;
    PAM_HidsHfCtStruDef();

#endif

}

VOS_VOID PAM_HidsNrBtStruDef(VOS_VOID)
{
#if (FEATURE_EASYRF == FEATURE_ON)
#ifndef FEATURE_M533_LNR_BT_DISABLE
    NBT_NPHY_RxBlerTestReq                 nbtNphyRxBlerTestReqStru;
    NBT_NPHY_RxRssiTestReq                 nbtNphyRxRssiTestReqStru;
    NBT_NPHY_TxTestReq                     nbtNphyTxTestReqStru;
    NPHY_NBT_RxBlerReportInd               nphyNbtRxBlerReportIndStru;
    NPHY_NBT_RxRssiReportInd               nphyNbtRxRssiReportIndStru;
    NPHY_NBT_RxTestCnf                     nphyNbtRxTestCnfStru;
    NPHY_NBT_TxTestCnf                     nphyNbtTxTestCnfStru;
#endif
#endif
    PAM_FUSION_NrStruSeparateDef();
}

VOS_VOID PAM_GucBtStruDef(VOS_VOID)
{
#ifndef MODEM_FUSION_VERSION
    NBT_TransMsgReq nbtTransMsgHids;
    NBT_UniformMsg nbtUniformMsgHids;
    NBT_ErrCodeInd nbtErrCodeHids;
    NBT_GeDlStatusReq nbtGeDlStatusHids;
    NBT_WcBerReq nbtWcBerDataHids;
    NBT_WSetPowerReq nbtWSetPowerHids;
    NBT_WgGetRssiReq nbtWgGetRssiHids;
    NBT_CSetPowerReq nbtCSetPowerHids;
    NBT_ListmodeContextReqPara nbtGuListmodeHids;
    NBT_ListModeTestReq nbtListModeHids;
    NBT_WBerDataInd nbtWBerDataIndHids;
    NBT_GetMsgCnf nbtGetMsgCnfHids;
    NBT_ListModeBerInd nbtListModeBerIndHids;
    NBT_ListModeRssiInd nbtListModeRssiIndHids;
    NBT_RssiCnf nbtRssiCnfHids;
    NBT_GBerInd nbtGBerDataIndHids;
    NBT_CFerCnf nbtCFerDataCnfHids;
#else 
    NBT_TransMsgReq nbtTransMsgHids;
    NBT_UniformMsg nbtUniformMsgHids;
    NBT_ErrCodeInd nbtErrCodeHids;
    NBT_GeDlStatusReq nbtGeDlStatusHids;
    NBT_WcBerReq nbtWcBerDataHids;
    NBT_WSetPowerReq nbtWSetPowerHids;
    NBT_WgGetRssiReq nbtWgGetRssiHids;
    NBT_CSetPowerReq nbtCSetPowerHids;
    NBT_ListmodeContextReqPara nbtGuListmodeHids;
    NBT_ListModeTestReqHids nbtListModeHids;
    NBT_WBerDataIndHids nbtWBerDataIndHids;
    NBT_GetMsgCnf nbtGetMsgCnfHids;
    NBT_ListModeBerInd nbtListModeBerIndHids;
    NBT_ListModeRssiInd nbtListModeRssiIndHids;
    NBT_RssiCnf nbtRssiCnfHids;
    NBT_GBerInd nbtGBerDataIndHids;
    NBT_CFerCnf nbtCFerDataCnfHids;
#endif
}

#endif

VOS_VOID PAM_HidsStruDefEntry(VOS_VOID)
{
#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED) || defined(CBT_ENABLED)
#if (FEATURE_EASYRF == FEATURE_ON)
#ifndef FEATURE_M533_LNR_BT_DISABLE
    OAM_PHY_SetWorkModeReq                 uecbtSetNrWorkModeReqStru;
    PHY_SET_NrWorkModeCnf                  phySetNrWorkModeCnfStru;
#ifndef MODEM_FUSION_VERSION
    GUCCBT_NrnosigIndMsg                   guccbtNrnosigIndMsgStru;
#endif

#ifdef PAM_BT_ENABLED
    NOSIG_NPHY_DEDI_RL_SETUP_REQ_STRU      nosigNphyDediRlSetupReqStru;
    NOSIG_NPHY_DEDI_RL_CFG_PER_CC_STRU     nosigNphyDediRlCfgPerCcStru;
    NRNOSIG_BT_TestReq                     nrnosigBtTestReqStru;
    NRNOSIG_BT_TestCnf                     nrnosigBtTestCnfStru;
#endif
#endif
#endif
    BBIC_CAL_MSG_CNF_STRU                bbicCalMsgCnfStru;
    BBIC_CAL_RF_DEBUG_TX_RESULT_IND_STRU bbicCalRfDebugTxResultIndStru;
    BBIC_CAL_RF_DEBUG_GTX_MRX_IND_STRU   bbicCalRfDebugGtxMrxIndStru;
    BBIC_CAL_RF_DEBUG_RX_RSSI_IND_STRU   bbicCalRfDebugRxRssiIndStru;
    BBIC_CAL_RF_DEBUG_REGISTER_IND_STRU  bbicCalRfDebugRegisterIndStru;
    BBIC_CAL_PLL_QRY_IND_STRU            bbicCalPllQryIndStru;
    BBIC_CAL_TEMP_QRY_IND_STRU           bbicCalTempQryIndStru;
    AT_CCBT_LOAD_PHY_CNF_STRU            atCcbtLoadPhyCnfStru;

    /* UECBT */
    AT_CCBT_LOAD_PHY_REQ_STRU atCcbtLoadPhyReqStru;
    AT_CCBT_LOAD_PHY_CNF_STRU atCcbtLoadPhyCnf;
    PAM_FusionStruDef();
    PAM_GucBtStruDef();
    PAM_HidsNrBtStruDef();

#endif
}


int pam_main()
{
    /* enum define */
    /* uism */
    enum USIMM_PollFcp           usimmPollFcpEnum;
    enum USIMM_FbdnHandle        usimmFbdnHandleEnum;
    enum USIMM_PbInitStatus     usimmPbInitStatusEnum;
    enum USIMM_RestRicCmd        usimmRestricCmdEnum;
    enum USIMM_CmdTypeReq        usimmCmdtypeReqEnum;
    enum USIMM_CardApp            usimmCardappEnum;
    enum USIMM_PinCmdType       usimmPinCmdTypeEnum;
    enum USIMM_PinType           usimmPinTypeEnum;
    enum USIMM_VsimState         usimmVsimStateEnum;
    enum USIMM_RealisimState     usimmRealisimStateEnum;
    enum USIMM_CardStatusIndType     usimmCardstatusIndEnum;
    enum USIMM_ChangeModeError  usimmChangeModeErrorEnum;
    enum USIMM_SWCheck            usimmSwcheckEnum;
    enum USIMM_CardstatusPlugIndType    usimmCardStatusIndEnum;
    enum USIMM_PassThroughState usimmPassThroughState;

    enum USIMM_DspLimitType         usimmDspDspLimitEnum;
    enum SI_PIH_ChangePollTimer  siPihChangepolltimerEnum;
    enum SI_PIH_Event            siPihEventEnum;
    enum SI_PIH_SimState        siPihSimStateEnum;
    enum SI_PIH_UiccAuthType         siPihUiccauthEnum;
    enum SI_PIH_UiccApp          siPihUiccappEnum;
    enum SI_PIH_AccessType       siPihAccesstypeEnum;
    enum SI_PIH_CardSlot        siPihCardSlotEnum;
    enum SI_PB_Type              siPbTypeEnum;
    enum SI_PB_ActiveState      siPbActiveStateEnum;
    enum SI_PB_InitStateType        siPbInitStateEnum;
    enum SI_PB_Storage           siPbStorageEnum;
    enum SI_PIH_CmdReqType     siPihCmdReqTypeEnum;
    enum SI_PIH_CmdCnfType     siPihCmdCnfTypeEnum;
    enum SI_PIH_PassThroughState siPihPassThroughState;
    enum USIMM_SimsqState  usimmSimsqStateEnum;

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    /* xsms */
    enum TAF_XSMS_SendOption     tafXsmsSendOptionEnum;
    enum TAF_XSMS_ApMemFull     tafXsmsApMemFullEnum;
    enum TAF_XSMS_Status          tafXsmsStatusEnum;
    enum TAF_XSMS_StkMsgType    tafXsmsStkMsgTypeEnum;
    enum TAF_XPDS_XSMS_MSG_TYPE_ENUM   tafXpdsXsmsMsgTypeEnum;
    enum XCC_SMS_PifMsgType    xccSmsPifMsgTypeEnum;
    enum TAF_XSMS_TlErrorClass  tafXsmsTlErrorClassEnum;
    enum TAF_XSMS_TlCauseCode   tafXsmsTlCauseCodeEnum;

    /* XCC XSMS */
    enum TAF_CDMA_SoType   tafCdmaSoTypeEnum;
    enum TAF_XCC_Cause      tafXccCauseEnum;
    enum TAF_XCC_FailLayer tafXccFailLayerEnum;
    enum TAF_XCC_EndReason tafXccEndReasonEnum;
    enum XCC_CSMS_Result    xccCsmsResultEnum;

    /* CAS XSMS(XPDS) */
    enum CAS_1X_CONTROL_MAIN_NAS_PIF_MSG_TYPE_ENUM cas1xControlMainNasPifMsgTypeEnum;
    enum CAS_CNAS_1X_TX_TCH_MSG_TYPE_ENUM          casCnas1xTxTchMsgTypeEnum;
    enum PS_Bool                                   psBoolEnum;
    enum CAS_CNAS_1X_DATA_CNF_RSLT_ENUM            casCnas1xDataCnfRsltEnum;
    enum CAS_CNAS_1X_RX_TCH_MSG_TYPE_ENUM          casCnas1xRxTchMsgTypeEnum;
    enum CAS_CNAS_1X_TX_COMMON_MSG_TYPE_ENUM       casCnas1xTxCommonMsgTypeEnum;
    enum CAS_CNAS_1X_DATA_BURST_TYPE_ENUM          casCnas1xDataBurstTypeEnum;
    enum CAS_CNAS_1X_MSG_DATA_TYPE_ENUM            casCnas1xMsgDataTypeEnum;
    enum CAS_CNAS_1X_RX_COMMON_MSG_TYPE_ENUM       casCnas1xRxCommonMsgTypeEnum;
    enum TAF_XSMS_MT_FSM_ID_ENUM                   tafXsmsMtFsmIdEnum;
    enum TAF_XSMS_MO_FSM_ID_ENUM                   tafXsmsMoFsmIdEnum;

    /* XPDS AT */
    enum AT_XPDS_MsgType                      atXpdsMsgTypeEnum;
    enum TAF_XPDS_InitialType                 tafXpdsInitialTypeEnum;
    enum XPDS_AT_Result                        xpdsAtResultEnum;
    enum TAF_XPDS_CallType                    tafXpdsCallTypeEnum;
    enum TAF_XPDS_FixMode                     tafXpdsFixModeEnum;
    enum AT_XPDS_ReplySyncTime               atXpdsReplySyncTimeEnum;
    enum XPDS_AT_SyncTime                     xpdsAtSyncTimeEnum;
    enum TAF_MSG_CdmaGpsFixQuality          tafMsgCdmaGpsFixQualityEnum;
    enum TAF_MSG_CdmaGpsFixType             tafMsgCdmaGpsFixTypeEnum;
    enum TAF_MSG_CdmaGpsSelectionType       tafMsgCdmaGpsSelectionTypeEnum;
    enum AT_XPDS_GpsNiReqReplyResult       atXpdsGpsNiReqReplyResultEnum;
    enum AT_XPDS_GpsNiReqRejectType        atXpdsGpsNiReqRejectTypeEnum;
    enum TAF_XPDS_NotiftAndVerifyIndicator  tafXpdsNotiftAndVerifyIndicatorEnum;
    enum TAF_XPDS_PosTechIndicator           tafXpdsPosTechIndicatorEnum;
    enum TAF_XPDS_Is801Mode                   tafXpdsIs801ModeEnum;
    enum XPDS_AT_AbortPosReason              xpdsAtAbortPosReasonEnum;
    enum TAF_XPDS_GpsStartMode               tafXpdsGpsStartModeEnum;

    /* XPDS XCC */
    enum AT_XPDS_AgpsDataCallCtrl  atXpdsAgpsDataCallCtrlEnum;
    enum AT_XPDS_DataCallStatus     atXpdsDataCallStatusEnum;
    enum AT_XPDS_ServerMode          atXpdsServerModeEnum;
    enum AT_XPDS_ServerBindStatus   atXpdsServerBindStatusEnum;
    enum XCC_AGPS_PIF_MSG_TYPE_ENUM       xccAgpsPifMsgTypeEnum;
    enum XCC_XPDS_RESULT_ENUM             xccXpdsResultEnum;

    /* XPDS CAS */
    enum XPDS_CAS_REF_LOC_RPT_TYPE_ENUM  xpdsCasRefLocRptTypeEnum;
    enum XPDS_CAS_RESULT_ENUM            xpdsCasResultEnum;

    /* XPDS XPDS */
    enum TAF_XPDS_XPDS_INTERNAL_MSG_ID_ENUM tafXpdsXpdsInternalMsgIdEnum;
    enum TAF_XPDS_SERVER_BIND_OPER_ENUM     tafXpdsServerBindOperEnum;
    enum TAF_XPDS_POS_PROC_RSLT_ENUM        tafXpdsPosProcRsltEnum;
    enum TAF_XPDS_MPC_OPER_TYPE_ENUM        tafXpdsMpcOperTypeEnum;
    enum TAF_XPDS_MNTN_MSG_ID_ENUM          tafXpdsMntnMsgIdEnum;
    enum TAF_XPDS_FSM_ID_ENUM               tafXpdsFsmIdEnum;
    enum TAF_XPDS_L1_STA_ENUM               tafXpdsL1StaEnum;
    enum TAF_XPDS_LINK_CTRL_SUBSTA_ENUM     tafXpdsLinkCtrlSubstaEnum;
    enum TAF_XPDS_CP_LINK_Ctrl_SUBSTA_ENUM  tafXpdsCpLinkCtrlSubstaEnum;
    enum TAF_XPDS_L2_MPC_PROC_STA_ENUM      tafXpdsL2MpcProcStaEnum;
    enum TAF_XPDS_L2_PDE_PROC_STA_ENUM      tafXpdsL2PdeProcStaEnum;
    enum TAF_XPDS_CP_L2_PDE_PROC_STA_ENUM   tafXpdsCpL2PdeProcStaEnum;
    enum TAF_XPDS_RL_MSGTYPE_REQ_ENUM       tafXpdsRlMsgtypeReqEnum;
    enum TAF_XPDS_RL_MSGTYPE_RESP_ENUM      tafXpdsRlMsgtypeRespEnu;
    enum TAF_XPDS_MS_TO_MPC_MSG_TYPE_ENUM   tafXpdsMsToMpcMsgTypeEnum;
    enum TAF_XPDS_MS_POS_CAPABILITY_ENUM    tafXpdsMsPosCapabilityEnum;
    enum TAF_XPDS_APP_TYPE_INDICATOR_ENUM   tafXpdsAppTypeIndicatorEnum;
    enum TAF_XPDS_RL_REJ_REASON_ENUM        tafXpdsRlRejReasonEnum;

    /* OM XPDS */
    enum NAS_OM_CdmaGpsFixRslt         nasOmCdmaGpsFixRsltEnum;
    enum NAS_OM_CdmaGpsFixMode         nasOmCdmaGpsFixModeEnum;
    enum NAS_OM_CdmaGpsStartMode       nasOmCdmaGpsStartModeEnum;
    enum NAS_OM_CdmaGpsFixType         nasOmCdmaGpsFixTypeEnum;
    enum NAS_OM_CdmaGpsSignalMsgType  nasOmCdmaGpsSignalMsgTypeEnum;

    /* xreg */
    enum CNAS_XREG_INTERNAL_MSG_TYPE_ENUM   cnasXregInternalMsgTypeEnum;
    enum CAS_CNAS_1X_REGISTRATION_TYPE_ENUM casCnas1xRegistrationTypeEnum;
    enum CNAS_XREG_REG_RESULT_ENUM          cnasXregRegResultEnum;
    enum CNAS_XREG_REGING_STA_ENUM          cnasXregRegingStaEnum;
    enum CNAS_XREG_SWITCH_ON_RESULT_ENUM    cnasXregSwitchOnResultEnum;
    enum CNAS_XREG_MNTN_MSGID_ENUM          cnasXregMntnMsgidEnum;
    enum CNAS_XREG_L1_STA_ENUM              cnasXregL1StaEnum;
    enum XSD_XREG_PIF_MSG_TYPE_ENUM         xsdXregPifMsgTypeEnum;

    enum CNAS_XREG_VAR_VALUE_ENUM   cnasXregVarValueEnum;
    enum CNAS_XREG_TIMER_STATE_ENUM cnasXregTimerStateEnum;
    enum CAS_CNAS_1X_CAS_STATE_ENUM casCnas1xCasStateEnum;
    enum CNAS_XREG_BLKSYS_ENUM      cnasXregBlksysEnum;
    enum NAS_OM_CdmaRegType   nasOmCdmaRegTypeEnum;

    /* hsd */
    enum CNAS_HSD_FSM_ID_ENUM                       cnasHsdFsmIdEnum;
    enum CNAS_HSD_SYS_ACQ_SCENE_ENUM                cnasHsdSysAcqSceneEnum;
    enum CNAS_HSD_AVOID_ITEM_USED_FALG_ENUM         cnasHsdAvoidItemUsedFalgEnum;
    enum CNAS_HSD_INTERNAL_MSG_TYPE_ENUM            cnasHsdInternalMsgTypeEnum;
    enum CNAS_HSD_ABORT_TYPE_ENUM                   cnasHsdAbortTypeEnum;
    enum CNAS_HSD_SWITCH_ON_RESULT_ENUM             cnasHsdSwitchOnResultEnum;
    enum CNAS_HSD_BSR_RESULT_ENUM                   cnasHsdBsrResultEnum;
    enum CAS_CNAS_HRPD_NETWORK_LOST_REASON_ENUM     casCnasHrpdNetworkLostReasonEnum;
    enum CNAS_HSD_SYS_ACQ_FAIL_ACT_ENUM             cnasHsdSysAcqFailActEnum;
    enum VOS_RatMode                                vosRatmodeEnumUint32;
    enum CNAS_HSD_MNTN_MRU_LOG_TYPE_ENUM            cnasHsdMntnMruLogTypeEnum;
    enum CAS_HRPD_AIRLINKMGMT_NAS_PIF_MSG_TYPE_ENUM casHrpdAirlinkmgmtNasPifMsgTypeEnum;
    enum CNAS_HSD_PRL_SOURCE_TYPE_ENUM              cnasHsdPrlSourceTypeEnum;
    enum PAM_NV_Id                              pamNvIdEnum;
    enum CNAS_HSD_SYSTEM_ACQUIRED_RESULT_ENUM       cnasHsdSystemAcquiredResultEnum;
    enum CNAS_HSD_REACQ_FALG_ENUM                   cnasHsdReacqFalgEnum;
    enum CNAS_PRL_BAND_CLASS_ENUM                   cnasPrlBandClassEnum;
    enum CNAS_HSD_FREQ_TYPE_INDICATION_ENUM         cnasHsdFreqTypeIndicationEnum;
    enum CNAS_HSD_AVOID_REASON_ENUM                 cnasHsdAvoidReasonEnum;

#endif

    /* taf mma */
    enum TAF_MMA_MsgType  tafMmaMsgTypeEnum;

    /* mta at */
    enum MTA_AT_Result  mtaAtResultEnum;

    /* CBT */
    enum UPHY_OAM_BusinessType uphyOamBusinessTypeEnum;

    /* USIMM */

    USIMM_ActiveCardReq       usimmActivecardReqStru;
    USIMM_InitContinueReq     usimmInitcontinueReqStru;
    USIMM_ProtectResetReq     usimmProtectresetReqStru;
    USIMM_DeactiveCardReq     usimmDeactivecardReqStru;
    USIMM_ReadFileReq         usimmReadfileReqStru;
    USIMM_QueryFileReq        usimmQueryfileReqStru;
    USIMM_StatusCmdReq        usimmStatuscmdReqStru;
    USIMM_SearchFileReq       usimmSearchfileReqStru;
    USIMM_FdnProcessReq       usimmFdnprocessReqStru;
    USIMM_OpenChannelReq      usimmOpenchannelReqStru;
    USIMM_CloseChannelReq     usimmClosechannelReqStru;
    USIMM_SendTpduDataReq     usimmSendtpdudataReqStru;
    USIMM_TerminalResponseReq usimmTermianlresponseStru;
    USIMM_STKEnvelopeReq      usimmEnvelopeReqStru;
    USIMM_StkrefreshReq       usimmStkrefreshReqStru;
    USIMM_PbinitStatusInd    usimmPbinitStatusIndStru;
    USIMM_RaccessReq          usimmRaccessReqStru;
    USIMM_SetMutilFileReq     usimmSetmutilfileReqStru;
    USIMM_HotInOutInd         usimmHotinoutIndStru;
    USIMM_CardPlugStatusInd      usimmCardPlugStatusIndStru;
    USIMM_CmdHeaderReq        usimmCmdheaderReqStru;
    USIMM_AuthenticationReq   usimmAuthenticationReqStru;
    USIMM_PinHandleReq        usimmPinhandleReqStru;
    USIMM_UpdateFileReq       usimmUpdatefileReqStru;
    USIMM_CardSciErrInd       usimmCardtimeoutindStru;
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    USIMM_BschallengeReq      usimmBasechallengeReqStru;
    USIMM_GenerateKeyvpmReq  usimmGenerateKeyvpmReqStru;
    USIMM_ManagessdReq        usimmManagessdReqStru;
    USIMM_CdmaSpecAuthReq     usimmCdmaspecauthReqStru;
    USIMM_QueryEsnMeidRsltReq usimmQueryesnmeidrsltReqStru;
#endif
#if (FEATURE_VSIM == FEATURE_ON)
    USIMM_VsimFileDataWriteReq usimmVsimfiledatawriteReqStru;
#endif

    USIMM_PinHandleCnf      usimmPinhandleCnfStru;
    USIMM_UpdateFileCnf     usimmUpdatefileCnfStru;
    USIMM_ReadFileCnf       usimmReadfileCnfStru;
    USIMM_STKRefreshCnf     usimmStkrefreshCnfStru;
    USIMM_AuthenticationCnf usimmAuthenticationCnfStru;
    USIMM_RaccessCnf        usimmRaccessCnfStru;
    USIMM_STKTerminalRspCnf usimmStkterminalrspCnfStru;
    USIMM_STKEnvelopeCnf    usimmStkenvelopeCnfStru;
    USIMM_SingleCmdCnf      usimmSinglecmdCnfStru;
    USIMM_FdnProcessCnf     usimmFdnprocessCnfStru;
    USIMM_SetMutilFileCnf   usimmSetmutilfileCnfStru;
    USIMM_SearchFileCnf     usimmSearchfileCnfStru;
    USIMM_OpenChannelCnf    usimmOpenchannelCnfStru;
    USIMM_CloseChannelCnf   usimmClosechannelCnfStru;
    USIMM_SendTpduDataCnf   usimmSendtpdudataCnfStru;
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    USIMM_BschallengeCnf      usimmBschallengeCnfStru;
    USIMM_GenerateKeyVpmCnf  usimmGenerateKeyvpmCnfStru;
    USIMM_QueryEsnMeidRsltCnf usimmQueryesnmeidrsltCnfStru;
    USIMM_CdmaSpecAuthCnf     usimmCdmaspecauthCnfStru;
    USIMM_XeccNumberInd       usimmXeccnumberIndStru;
#endif

    USIMM_PrivateCglaHandleCnf usimmPrivatecglahandleCnfStru;
    USIMM_QueryCardTypeCnf    usimmQueryCardtypeCnfStru;
    USIMM_SilentPinHandleCnf  usimmSilentPinhandleCnfStru;

    USIMM_GetIdentityCnf  usimmGetIdentityCnfStru;

    USIMM_CardStatusInd      usimmCardstatusindStru;
    USIMM_STKDataInd         usimmStkdataIndStru;
    USIMM_VsimRdhInd        usimmVsimRdhIndStru;
    USIMM_EccNumberInd       usimmEccnumberIndStru;
    USIMM_FdnStatusInd       usimmFdnstatusIndStru;
    USIMM_FdnQueryCnf        usimmFdnqueryCnfStru;
    USIMM_FdnQueryReq        usimmFdnqueryReqStru;
    USIMM_ChangeCardModeCnf usimmChangeCardmodeCnfStru;
    USIMM_ChangeCardModeReq usimmChangeCardmodeReqStru;
    USIMM_CardErrorInd       usimmCarderrorIndStru;
    USIMM_IccidContentInd    usimmIccidcontentIndStru;
    USIMM_SwcheckInd         usimmSwcheckIndStru;
    USIMM_CardFetchInd       usimmCardfetchIndStru;
    USIMM_DspLimit           usimmDspLimitStru;
    USIMM_SciCfgSetReq      usimmScicfgSetReqStru;

    USIMM_EsimSwtichReq     usimmEsimswtichReqStru;
    USIMM_SlotCardType      usimmSlotCardtypeStru;
    USIMM_EsimSwtich         usimmEsimswtichStru;
    USIMM_EsimSwtichQryCnf usimmEsimswtichQryCnfStru;
    USIMM_PassThroughReq    usimmPassThroughReqStru;
    USIMM_PassThroughQryCnf usimmPassThroughQryCnfStru;

    USIMM_RefreshRecordPlmn usimmRefreshRecordPlmnStru;
    USIMM_RefreshPlmnMsgInd    usimmRefreshPlmnIndStru;

    USIMM_SimsqStatusInd usimmSimsqstatusIndStru;
    USIMM_PbStatusCnf   usimmPbStatusCnfStr;

    USIMM_STKEnvelopeRetry       usimmStkenvelopeRetryStru;
    USIMM_ScichgInd              usimmScichgIndStru;
    USIMM_Support5GCardFileInd usimmSupport5gCardfileIndStru;
    USIMM_UpdateFileAtInd usimmUpdateFileAtIndStru;
    USIMM_UiccToIccChrInd usimmUiccToIccChrIndStru;
    USIMM_CardBusyChrInd usimmCardBusyChrIndStru;
    USIMM_FcpErrorChrInd usimmFcpErrorChrIndStru;
    USIMM_AuthFailChrInd usimmAuthFailChrIndStru;
    USIMM_QueryVoltageReq usimmQueryVoltageReqStru;
    USIMM_QueryVoltageCnf usimmQueryVoltageCnfStru;
    USIMM_CmdHeaderCnf usimmCmdHeaderCnfStru;
#if (FEATURE_PTM == FEATURE_ON)
    USIMM_StatisEventInd usimmStatisEventIndStru;
#endif

    SI_PIH_FdnEnableReq          siPihFdnEnableReqStru;
    SI_PIH_FdnDisableReq         siPihFdnDisableReqStru;
    SI_PIH_MsgHeader              siPihMsgHeaderStru;
    SI_PIH_GaccessReq             siPihGaccessReqStru;
    SI_PIH_IsdbAccessReqMsg         siPihIsdbAccessReqStru;
    SI_PIH_CchoSetReqMsg            siPihCchoSetReqStru;
    SI_PIH_CchcSetReqMsg           siPihCchcSetReqStru;
    SI_PIH_CglaReq                siPihCglaReqStru;
    SI_PIH_HvsstReq               siPihHvsstReqStru;
    SI_PIH_ScicfgSetReq          siPihScicfgReqStru;
    SI_PIH_HvcheckcardReq         siPihHvcheckcardReqStru;
    SI_PIH_UiccauthReq            siPihUiccauthReqStru;
    SI_PIH_AccessfileReq          siPihAccessfileReqStru;
    SI_PIH_CrsmSetReqMsg            siPihCrsmSetReqStru;
    SI_PIH_CrlaSetReqMsg            siPihCrlaSetReqStru;
    MN_APP_PihAtCnf              mnAppPihAtCnfStru;
    SI_PIH_HvsdhSetReq           siPihHvsdhSetReqStru;
    SI_PIH_HvsQryReq             siPihHvsQryReqStru;
    SI_PIH_FileWriteReq          siPihFileWriteReqStru;
    SI_PIH_HvteeSetReq           siPihHvteeSetReqStru;
    SI_PIH_PassThroughReqMsg    siPihPassThroughReqMsgStru;
    SI_PIH_StartCheckKeyFileNtf siPihStartCheckKeyfileNtfStru;
    SI_PIH_StopCheckKeyFileNtf  siPihStopCheckKeyfileNtfStru;
    SI_PIH_CheckKeyFileRlstInd  siPihCheckKeyfileRlstIndStru;
    SI_PIH_HookMsg siPihHookMsgStru;
    SI_PIH_QueryCardStatusCnf siPihQueryCardStatusCnfStru;
    SI_PIH_SilentPinReq siPihSilentPinReqStru;
    SI_PIH_SilentPininfoReq siPihSilentPininfoReqStru;
    SI_PIH_EsimswitchReq siPihEsimswitchReqStru;
    SI_PIH_BwtSetReq siPihBwtSetReqStru;
    SI_PIH_RegRefreshPidReq siPihRegRefreshPidReqStru;
    PS_SI_Msg psSiMsgStru;

#if (FEATURE_VSIM == FEATURE_ON)
    SI_PIH_VsimApnInd siPihVsimapnIndStru;
#endif

#if ((FEATURE_PHONE_USIM == FEATURE_OFF) && (MULTI_MODEM_NUMBER == 1))
    SI_PIH_CardAppAidSetReq siPihCardAppAidSetReqStru;
#endif

    SI_STK_Req                  siStkReqStru;
    STK_AS_TaInfoReq          stkAsTaInfoReqStru;
    STK_AS_NmrInfoReq         stkAsNmrInfoReqStru;
    SI_STK_TafSendmsg         siStkTafSendmsgStru;
    SI_STK_EnvelopedownReq    siStkEnvelopedownReqStru;
    SI_STK_EnvelopeDwonCnf    siStkEnvelopedwoncnfStru;
    MN_APP_StkAtCnf           mnAppStkAtCnfStru;
    STK_NAS_SteeringOfRoaming stkNasSteeringOfRoamingStru;

    SI_PB_ReadReq             siPbReadReqStru;
    SI_PB_QueryReq            siPbQueryReqStru;
    SI_PB_SetReq              siPbSetReqStru;
    SI_PB_AddRep              siPbAddReqStru;
    SI_PB_ModifyRep           siPbModifyReqStru;
    SI_PB_DeleteReq           siPbDeleteReqStru;
    SI_PB_SearchReq           siPbSearchReqStru;
    SI_PB_EcallQryReq         siPbEcallReqStru;
    SI_PB_EcallInitInd        siPbEcallinitIndStru;
    SI_PB_FdnCheckReq        siPbFdnCheckReqStru;
    SI_PB_UpdateglobalInd     siPbUpdateglobalIndStru;
    SI_PB_SetpbInd            siPbSetpbIndStru;
    MN_APP_PbAtCnf           mnAppPbAtCnfStru;
    MN_APP_CallCallorigReq   mnAppCallCallorigReqStru;
    SI_PIH_ChangePollTimerReq siPihChangeepolltimerReqStru;
    SI_PB_EcallQryCnf         siPbEcallqryCnfStru;

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    /* at xsms stru */
    TAF_XSMS_SendMsgReq                tafXsmsSendMsgReqStru;
    TAF_XSMS_AppMsgSetApMemFullReq tafXsmsAppMsgSetApMemFullReqStru;
    TAF_XSMS_WriteMsgReq               tafXsmsWriteMsgReqStru;
    TAF_XSMS_DeleteMsgReq              tafXsmsDeleteMsgReqStru;
    TAF_XSMS_InitInd                    tafXsmsInitIndStru;
    TAF_XSMS_WriteCnf                   tafXsmsWriteCnfStru;
    TAF_XSMS_SendSuccInd               tafXsmsSendSuccIndStru;
    TAF_XSMS_RcvInd                     tafXsmsRcvIndStru;
    TAF_XSMS_Message                     tafXsmsMessageStru;
    TAF_XSMS_AppAtCnf                  tafXsmsAppAtCnfStru;

    /* stk xsms */
    XSMS_STK_SendSmsCnf      xsmsStkSendSmsCnfStru;
    XSMS_STK_SendSmsRsltInd xsmsStkSendSmsRsltIndStru;
    XSMS_STK_RcvSmsInd       xsmsStkRcvSmsIndStru;
    STK_XSMS_SendSmsReq      stkXsmsSendSmsReqStru;

    /* xpds xsms */
    TAF_XSMS_XPDS_REV_IND_STRU  tafXsmsXpdsRevIndStru;
    TAF_XPDS_XSMS_SEND_REQ_STRU tafXpdsXsmsSendReqStru;

    /* XCC XSMS */
    CSMS_XCC_OrigSmsCallReq   csmsXccOrigSmsCallReqStru;
    XCC_CSMS_OrigSmsCallCnf   xccCsmsOrigSmsCallCnfStru;
    CSMS_XCC_EndSmsCallReq    csmsXccEndSmsCallReqStru;
    XCC_CSMS_EndSmsCallCnf    xccCsmsEndSmsCallCnfStru;
    XCC_CSMS_IncomingCallInd   xccCsmsIncomingCallIndStru;
    CSMS_XCC_IncomingCallRsp   csmsXccIncomingCallRspStru;
    XCC_CSMS_CallConnInd       xccCsmsCallConnIndStru;
    CSMS_XCC_AnswerCallReq     csmsXccAnswerCallReqStru;
    XCC_CSMS_AnswerCallCnf     xccCsmsAnswerCallCnfStru;
    XCC_CSMS_MessageWaitingInd xccCsmsMessageWaitingIndStru;
    XCC_CSMS_ServiceConnectInd xccCsmsServiceConnectIndStru;
    XCC_CSMS_MsgWatingInd      xccCsmsMsgWatingIndStru;
    /* CAS XSMS(XPDS) */
    CNAS_CAS_1X_DSCH_DATA_REQ_STRU cnasCas1xDschDataReqStru;
    CAS_CNAS_1X_DSCH_DATA_CNF_STRU casCnas1xDschDataCnfStru;
    CAS_CNAS_1X_DSCH_DATA_IND_STRU casCnas1xDschDataIndStru;
    CNAS_CAS_1X_CSCH_DATA_REQ_STRU cnasCas1xCschDataReqStru;
    CAS_CNAS_1X_CSCH_DATA_CNF_STRU casCnas1xCschDataCnfStru;
    CAS_CNAS_1X_CSCH_DATA_IND_STRU casCnas1xCschDataIndStru;
    CAS_CNAS_1X_SMS_ABORT_IND_STRU casCnas1xSmsAbortIndStru;

    /* XSMS XSMS */
    TAF_XSMS_ELF_INT_3RD_PARTY_ERR_STRU          tafXsmsInt3rdPartyErrStru;
    TAF_XSMS_ELF_INT_CUR_MO_FSM_STRU             tafXsmsIntCurMoFsmStru;
    TAF_XSMS_ELF_INT_CUR_MT_FSM_STRU             tafXsmsIntCurMtFsmStru;
    TAF_XSMS_ELF_INT_SMS_CONTENT_STRU            tafXsmsIntSmsContentStru;
    TAF_XSMS_ELF_INT_TL_ACK_STRU                 tafXsmsIntTlAckStru;
    TAF_XSMS_ELF_INT_1X_SIG_ENCRYPTION_INFO_STRU tafXsmsInt1xSigEncryptionInfoStru;
    TAF_XSMS_ELF_INT_SND_DATA_BURST_MSG_STRU     tafXsmsIntSndDataBurstMsgStru;

    /* XPDS AT */
    AT_XPDS_GpsStartReq             atXpdsGpsStartReqStru;
    XPDS_AT_ResultCnf                xpdsAtResultCnfStru;
    AT_XPDS_GpsCfgMpcAddrReq      atXpdsGpsCfgMpcAddrReqStru;
    AT_XPDS_GpsCfgPdeAddrReq      atXpdsGpsCfgPdeAddrReqStru;
    AT_XPDS_GpsCfgPositionModeReq atXpdsGpsCfgPositionModeReqStru;
    AT_XPDS_GpsQryReflocInfoReq   atXpdsGpsQryReflocInfoReqStru;
    XPDS_AT_GpsReflocInfoCnf       xpdsAtGpsReflocInfoCnfStru;
    AT_XPDS_GpsQryTimeInfoReq     atXpdsGpsQryTimeInfoReqStru;
    XPDS_AT_GpsSyncTimeInd         xpdsAtGpsSyncTimeIndStru;
    AT_XPDS_GpsStopReq              atXpdsGpsStopReqStru;
    AT_XPDS_GpsPrmInfoRsp          atXpdsGpsPrmInfoRspStru;
    AT_XPDS_GpsPosInfoRsp          atXpdsGpsPosInfoRspStru;
    AT_XPDS_GpsReplyNiRsp          atXpdsGpsReplyNiRspStru;
    XPDS_AT_GpsIonInfoInd          xpdsAtGpsIonInfoIndStru;
    XPDS_AT_GpsEphInfoInd          xpdsAtGpsEphInfoIndStru;
    XPDS_AT_GpsAlmInfoInd          xpdsAtGpsAlmInfoIndStru;
    XPDS_AT_GpsPdePosiInfoInd     xpdsAtGpsPdePosiInfoIndStru;
    XPDS_AT_GpsNiSessionInd        xpdsAtGpsNiSessionIndStru;
    XPDS_AT_GpsStartReq             xpdsAtGpsStartReqStru;
    XPDS_AT_GpsCancelInd            xpdsAtGpsCancelIndStru;
    XPDS_AT_GpsAcqAssistDataInd   xpdsAtGpsAcqAssidataIndStru;
    XPDS_AT_GpsDelAssistDataInd   xpdsAtGpsDelAssidataIndStru;
    XPDS_AT_GpsAbortInd             xpdsAtGpsAbortIndStru;
    XPDS_AT_GpsNiCpStart           xpdsAtGpsNiCpStartStru;
    XPDS_AT_GpsNiCpStop            xpdsAtGpsNiCpStopStru;
    XPDS_AT_ApDataCallReq          xpdsAtApDataCallReqStru;
    AT_XPDS_ApDataCallStatusInd   atXpdsApDataCallStatusIndStru;
    XPDS_AT_ApServerBindReq        xpdsAtApServerBindReqStru;
    AT_XPDS_ApServerBindStatusInd atXpdsApServerBindStatusIndStru;
    XPDS_AT_ApReverseDataInd       xpdsAtApReverseDataIndStru;
    AT_XPDS_ApForwardDataInd       atXpdsApForwardDataIndStru;
    AT_XPDS_Msg                       atXpdsMsgStru;
    XPDS_AT_UtsGpsPosInfoInd      xpdsAtUtsGpsPosInfoIndStru;
    XPDS_AT_GpsOmTestStartReq     xpdsAtGpsOmTestStartReqStru;
    XPDS_AT_GpsOmTestStopReq      xpdsAtGpsOmTestStopReqStru;

    /* XPDS XCC */
    XPDS_XCC_OrigAgpsCallReq  xpdsXccOrigAgpsCallReqStru;
    XCC_XPDS_OrigAgpsCallCnf  xccXpdsOrigAgpsCallCnfStru;
    XPDS_XCC_EndAgpsCallReq   xpdsXccEndAgpsCallReqStru;
    XCC_XPDS_EndAgpsCallCnf   xccXpdsEndAgpsCallCnfStru;
    XCC_XPDS_IncomingCallInd   xccXpdsIncomingCallIndStru;
    XPDS_XCC_IncomingCallRsp   xpdsXccIncomingCallRspStru;
    XCC_XPDS_CallConnInd       xccXpdsCallConnIndStru;
    XPDS_XCC_AnswerCallReq     xpdsXccAnswerCallReqStru;
    XCC_XPDS_AnswerCallCnf     xccXpdsAnswerCallCnfStru;
    XCC_XPDS_ServiceConnectInd xccXpdsServiceConnectIndStru;

    /* XPDS CAS */
    XPDS_CAS_REF_LOC_REQ_STRU   xpdsCasRefLocReqStru;
    CAS_XPDS_REF_LOC_CNF_STRU   casXpdsRefLocCnfStru;
    CAS_XPDS_REF_LOC_IND_STRU   casXpdsRefLocIndStru;
    XPDS_CAS_TIME_SYNC_REQ_STRU xpdsCasTimeSyncReqStru;
    CAS_XPDS_TIME_SYNC_IND_STRU casXpdsTimeSyncIndStru;

    /* XPDS XPDS */
    TAF_XPDS_XPDS_UP_LINK_CTRL_REQ_STRU                    tafXpdsXpdsUpLinkCtrlReqStru;
    TAF_XPDS_XPDS_UP_LINK_CTRL_CNF_STRU                    tafXpdsXpdsUpLinkCtrlCnfStru;
    TAF_XPDS_XPDS_INT_MPC_PROC_START_REQ_STRU              tafXpdsXpdsIntMpcProcStartReqStru;
    TAF_XPDS_XPDS_INT_MPC_PROC_RSLT_IND_STRU               tafXpdsXpdsIntMpcProcRsltIndStru;
    TAF_XPDS_XPDS_INT_RCV_MPC_START_POS_RESP_STRU          tafXpdsXpdsIntRcvMpcStartPosRespStru;
    TAF_XPDS_XPDS_CP_LINK_PROC_START_REQ_STRU              tafXpdsXpdsCpLinkProcStartReqStru;
    TAF_XPDS_XPDS_CP_LINK_PROC_RSLT_IND_STRU               tafXpdsXpdsCpLinkProcRsltIndStru;
    TAF_XPDS_XPDS_CP_PDE_PROC_START_REQ_STRU               tafXpdsXpdsCpPdeProcStartReqStru;
    TAF_XPDS_XPDS_FORWARD_PDE_DATA_IND_STRU                tafXpdsXpdsForwardPdeDataIndStru;
    TAF_XPDS_XPDS_PDE_PROC_START_REQ_STRU                  tafXpdsXpdsPdeProcStartReqStru;
    TAF_XPDS_XPDS_INT_RCV_MPC_POS_REPORT_RESP_STRU         tafXpdsXpdsIntRcvMpcPosReportRespStru;
    TAF_XPDS_XPDS_INT_XSMS_CANCEL_NI_TRACK_SESSION_STRU    tafXpdsXpdsIntXsmsCancelNiTrackSessionStru;
    TAF_XPDS_XPDS_PPM_COMPLETE_MSG_STRU                    tafXpdsXpdsPpmCompleteMsgStru;
    TAF_XPDS_XPDS_INT_ABNORMAL_ABORT_AGPS_SESSION_IND_STRU tafXpdsXpdsIntAbnormalAbortAgpsSessionIndStru;
    TAF_XPDS_XPDS_PDE_PROC_RSLT_IND_STRU                   tafXpdsXpdsPdeProcRsltIndStru;
    TAF_XPDS_ELF_MNTN_LOG_FSM_INFO_STRU                    tafXpdsElfMntnLogFsmInfoStru;
    TAF_XPDS_MNTN_SESSION_BEGIN_IND_STRU                   tafXpdsMntnSessionBeginIndStru;
    TAF_XPDS_MNTN_SESSION_END_IND_STRU                     tafXpdsMntnSessionEndIndStru;
    TAF_XPDS_MNTN_REVERSE_MSG_INFO_IND_STRU                tafXpdsMntnReverseMsgInfoIndStru;
    TAF_XPDS_MNTN_FORWARD_MSG_INFO_IND_STRU                tafXpdsMntnForwardMsgInfoIndStru;
    TAF_XPDS_MNTN_PILOT_INFO_STRU                          tafXpdsMntnPilotInfoStru;
    TAF_XPDS_MNTN_PILOT_INFO_CTX_IND_STRU                  tafXpdsMntnPilotInfoCtxIndStru;
    TAF_XPDS_MNTN_PSEU_RANGE_INFO_CTX_IND_STRU             tafXpdsMntnPseuRangeInfoCtxIndStru;
    TAF_XPDS_MNTN_ASSIST_DATA_CTX_IND_STRU                 tafXpdsMntnAssistDataCtxIndStru;
    TAF_XPDS_MNTN_POS_INFO_CTX_IND_STRU                    tafXpdsMntnPosInfoCtxIndStru;
    TAF_XPDS_MNTN_EPH_INFO_CTX_IND_STRU                    tafXpdsMntnEphInfoCtxIndStru;
    TAF_XPDS_MNTN_ALM_INFO_CTX_IND_STRU                    tafXpdsMntnAlmInfoCtxIndStru;
    TAF_XPDS_MNTN_FORWARD_PDE_MSG_HEADER_IND_STRU_STRU     tafXpdsMntnForwardPdeMsgHeaderIndStruStru;
    TAF_XPDS_MNTN_REVERSE_PDE_MSG_HEADER_IND_STRU_STRU     tafXpdsMntnReversePdeMsgHeaderIndStruStru;
    TAF_XPDS_MNTN_REVERSE_START_DELIVER_DATA_IND_STRU_STRU tafXpdsMntnReverseStartDeliverDataIndStruStru;
    TAF_XPDS_MNTN_REVERSE_MPC_START_POS_REQ_IND_STRU_STRU  tafXpdsMntnReverseMpcStartPosReqIndStruStru;
    TAF_XPDS_MNTN_REVERSE_MPC_POS_REPORT_REQ_IND_STRU_STRU tafXpdsMntnReverseMpcPosReportReqIndStruStru;
    TAF_XPDS_MNTN_REVERSE_PDE_REJ_RSP_IND_STRU_STRU        tafXpdsMntnReversePdeRejRspIndStruStru;
    TAF_XPDS_MNTN_1X_SIG_ENCRYPTION_INFO_IND_STRU          tafXpdsMntn1xSigEncryptionInfoIndStru;
    TAF_XPDS_MNTN_DATA_BURST_MSG_IND_STRU                  tafXpdsMntnDataBurstMsgIndStru;

    /* OM XPDS */
    OM_NAS_CdmaStartGpsFixReq  omNasCdmaStartGpsFixReqStru;
    NAS_OM_CdmaStartGpsFixCnf  nasOmCdmaStartGpsFixCnfStru;
    OM_NAS_CdmaStopGpsFixReq   omNasCdmaStopGpsFixReqStru;
    NAS_OM_CdmaStopGpsFixCnf   nasOmCdmaStopGpsFixCnfStru;
    NAS_OM_CdmaGpsUtcTime       nasOmCdmaGpsUtcTimeStru;
    NAS_OM_CdmaGpsPosInfo       nasOmCdmaGpsPosInfoStru;
    NAS_OM_CdmaGpsFixRsltInd   nasOmCdmaGpsFixRsltIndStru;
    NAS_OM_CdmaGpsSignalMsgInd nasOmCdmaGpsSignalMsgIndStru;
    NAS_OM_CdmaGpsFixStartInd  nasOmCdmaGpsFixStartIndStru;

    /* XREG XREG */
    CNAS_XREG_XREG_REG_REQ_STRU            cnasXregXregRegReqStru;
    CNAS_XREG_XREG_REG_CNF_STRU            cnasXregXregRegCnfStru;
    CNAS_XREG_XREG_ABORT_FSM_STRU          cnasXregXregAbortFsmStru;
    CNAS_XREG_XREG_SWITCH_ON_RSLT_CNF_STRU cnasXregXregSwitchOnRsltCnfStru;
    CNAS_XREG_ZONE_LIST_MNTN_STRU          cnasXregZoneListMntnStru;
    CNAS_XREG_SIDNID_LIST_MNTN_STRU        cnasXregSidnidListMntnStru;
    CNAS_XREG_MNTN_MSG_STRU                cnasXregMntnMsgStru;
    CNAS_MNTN_LOG_FSM_INFO_STRU            cnasMntnLogFsmInfoStru;

    /* XREG XSD */
    XREG_XSD_REG_SUCCESS_IND_STRU xregXsdRegSuccessIndStru;
    XREG_XSD_START_CNF_STRU       xregXsdStartCnfStru;
    XREG_XSD_POWER_OFF_CNF_STRU   xregXsdPowerOffCnfStru;

    /* HSD HSD */
    CNAS_HSD_HSD_ABORT_REQ_STRU                   cnasHsdHsdAbortReqStru;
    CNAS_HSD_HSD_SWITCH_ON_RSLT_CNF_STRU          cnasHsdHsdSwitchOnRsltCnfStru;
    CNAS_HSD_HSD_BSR_RSLT_CNF_STRU                cnasHsdHsdBsrRsltCnfStru;
    CNAS_HSD_HSD_POWER_OFF_RSLT_CNF_STRU          cnasHsdHsdPowerOffRsltCnfStru;
    CNAS_HSD_HSD_SYSTEM_ACQUIRED_REQ_STRU         cnasHsdHsdSystemAcquiredReqStru;
    CNAS_HSD_HSD_SYSTEM_ACQUIRED_RSLT_CNF_STRU    cnasHsdHsdSystemAcquiredRsltCnfStru;
    CNAS_XSD_HSD_SYS_ACQ_RSLT_IND_STRU            cnasXsdHsdSysAcqRsltIndStru;
    CNAS_HSD_HSD_POWER_SAVE_RSLT_CNF_STRU         cnasHsdHsdPowerSaveRsltCnfStru;
    CNAS_HSD_HSD_INTER_SYS_RSLT_CNF_STRU          cnasHsdHsdInterSysRsltCnfStru;
    CNAS_HSD_LOG_SCAN_FREQ_LIST_MSG_STRU          cnasHsdLogScanFreqListMsgStru;
    CNAS_HSD_LOG_HRPD_SYS_LIST_MSG_STRU           cnasHsdLogHrpdSysListMsgStru;
    CNAS_HSD_LOG_MRU_LIST_MSG_STRU                cnasHsdLogMruListMsgStru;
    CNAS_HSD_LOG_OOC_TIMER_SCHEDULE_INFO_MSG_STRU cnasHsdLogOocTimerScheduleInfoMsgStru;
    CNAS_HSD_LOG_OOC_CTX_INFO_MSG_STRU            cnasHsdLogOocCtxInfoMsgStru;
    CNAS_HSD_LOG_AVOID_FREQ_LIST_MSG_STRU         cnasHsdLogAvoidFreqListMsgStru;
    CNAS_HSD_LOG_REDIRCT_MSG_STRU                 cnasHsdLogRedirctMsgStru;
    CNAS_HSD_LOG_OPER_LOCK_SYS_WHITE_LIST_STRU    cnasHsdLogOperLockSysWhiteListStru;
    CNAS_HSD_LOG_CTCC_CUSTOMIZE_FREQ_LIST_STRU    cnasHsdLogCtccCustomizeFreqListStru;
    CNAS_HSD_LOG_PRL_SOURCE_TYPE_MSG_STRU         cnasHsdLogPrlSourceTypeMsgStru;
    CNAS_HSD_HSD_BG_SRCH_RSLT_CNF_STRU            cnasHsdHsdBgSrchRsltCnfStru;
    CNAS_HSD_HSD_INTERNAL_OOC_NTF_STRU            cnasHsdHsdInternalOocNtfStru;
    CNAS_HSD_HSD_OOC_NTF_RSLT_CNF_STRU            cnasHsdHsdOocNtfRsltCnfStru;
#endif

    REL_TimerMsgBlock relTimerMsg;

    /* TAF MMA */
    TAF_MMA_CfreqLockSetReq tafMmaCfreqLockSetReqStru;
    TAF_MMA_CfreqLockSetCnf tafMmaCfreqLockSetCnfStru;
    TAF_MMA_1XchanQueryReq   tafMma1xchanQueryReqStru;
    TAF_MMA_1XchanQueryCnf   tafMma1xchanQueryCnfStru;
    TAF_MMA_CverQueryReq     tafMmaCverQueryReqStru;
    TAF_MMA_CverQueryCnf     tafMmaCverQueryCnfStru;
    TAF_MMA_StateQueryReq    tafMmaStateQueryReqStru;
    TAF_MMA_StateQueryCnf    tafMmaStateQueryCnfStru;
    TAF_MMA_ChighverQueryReq tafMmaChighverQueryReqStru;
    TAF_MMA_ChighverQueryCnf tafMmaChighverQueryCnfStru;

    /* at mta */
    AT_MTA_MeidSetReq atMtaMeidSetReqStru;
    MTA_AT_MeidQryCnf mtaAtMeidQryCnfStru;

    MTA_NV_RefreshInd mtaNvRefreshIndStru;
    MTA_NV_RefreshRsp mtaNvRefreshRspStru;

    UECBT_CAL_ComponentId uecbtCalComponentIdStru;
    DRX_TimerWakeSrcVote drxTimerWakeSrcVote;
    VOS_TimerOmEvent vosTimerOmEvent;
    /* EMAT */
    SI_EMAT_MsgHeader siEmatMsgHeaderStru;

    /* SAR */
    SPY_Msg spyMsgStru;
    PHY_PAM_AntenStatusInd phyPamAntenStatusIndStru;

    PAM_HidsStruDefEntry();

    return 0;
}
#endif
