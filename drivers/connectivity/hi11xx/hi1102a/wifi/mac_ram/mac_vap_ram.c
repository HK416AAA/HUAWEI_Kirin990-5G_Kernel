

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "mac_vap.h"
#include "mac_device.h"
#include "platform_spec_1102a.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_VAP_RAM_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
mac_vap_rom_stru g_ast_mac_vap_rom[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];

/*****************************************************************************
  3 函数实现
*****************************************************************************/

oal_rom_cb_result_enum_uint8 mac_vap_init_cb(
                mac_vap_stru               *pst_vap,
                oal_uint8                   uc_chip_id,
                oal_uint8                   uc_device_id,
                oal_uint8                   uc_vap_id,
                mac_cfg_add_vap_param_stru *pst_param,
                oal_uint32                 *pul_cb_ret)
{
#ifdef _PRE_WLAN_CHBA_MGMT
    mac_vap_rom_stru *pst_mac_vap_rom = NULL;
#endif
    *pul_cb_ret = OAL_SUCC;

#ifdef _PRE_WLAN_FEATURE_TXBF
    pst_vap->st_txbf_add_cap.bit_min_grouping      = 0;
    pst_vap->st_txbf_add_cap.bit_csi_bfee_max_rows = 0;
    pst_vap->st_txbf_add_cap.bit_channel_est_cap   = 0;
    pst_vap->st_cap_flag.bit_11ntxbf               = 0;
    pst_vap->en_host_txbf_mode = COMPABILITY_TXBF;
#endif
    /* 默认APUT不支持随环境进行自动2040带宽切换 */
    pst_vap->st_cap_flag.bit_2040_autoswitch = OAL_FALSE;
    mac_mib_set_LsigTxopFullProtectionActivated(pst_vap, OAL_FALSE);

    pst_vap->_rom = &g_ast_mac_vap_rom[uc_vap_id];
    memset_s(pst_vap->_rom, OAL_SIZEOF(mac_vap_rom_stru), 0, OAL_SIZEOF(mac_vap_rom_stru));
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
    if (IS_LEGACY_STA(pst_vap)) {
        /* 仅LEGACY_STA支持 */
        pst_vap->st_cap_flag.bit_icmp_filter = OAL_TRUE;
    }
#endif
#ifdef _PRE_WLAN_CHBA_MGMT
    pst_mac_vap_rom = (mac_vap_rom_stru *)pst_vap->_rom;
    /* 同步hmac chba mode信息 */
    pst_mac_vap_rom->chba_mode = pst_param->chba_mode;
#endif

    return OAL_CONTINUE;
}


oal_rom_cb_result_enum_uint8 mac_vap_init_privacy_cb(
                                       mac_vap_stru                       *pst_mac_vap,
                                       mac_cfg80211_connect_security_stru *pst_mac_security_param,
                                       oal_uint32                         *pul_ret)
{
    return OAL_CONTINUE;
}


oal_rom_cb_result_enum_uint8 mac_vap_set_beacon_cb(
                                                 mac_vap_stru          *pst_mac_vap,
                                                 mac_beacon_param_stru *pst_beacon_param,
                                                 oal_uint32            *pul_ret)
{
    return OAL_CONTINUE;
}


oal_rom_cb_result_enum_uint8 mac_vap_del_user_cb(
                                                mac_vap_stru          *pst_vap,
                                                oal_uint16             us_user_idx,
                                                oal_uint32            *pul_ret)
{
    return OAL_CONTINUE;
}

