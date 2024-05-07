

#ifndef __SDT_DRV_H__
#define __SDT_DRV_H__

/* ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oal_workqueue.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_SDT_DRV_H
/* �궨�� */
#ifdef  NETLINK_WIFI_SDT_HISI
#define NETLINK_TEST                        NETLINK_WIFI_SDT_HISI
#else
#define NETLINK_TEST                        28   /* ����5610 ATP���̣���ֹЭ��ų�ͻ����һ�޸�Ϊ28 */
#endif
#define DATA_BUF_LEN 2048

/* head and end */
#define SDT_DRV_PKT_START_FLG        0x7e
#define SDT_DRV_PKT_END_FLG          0x7e
#define sdt_drv_get_low_byte(_data)  ((uint8_t)((_data)&0xff))
#define sdt_drv_get_high_byte(_data) ((uint8_t)(((_data)&0xff00) >> 8))
#define SDT_DRV_PKT_TAIL_LEN         1

#define SDT_DEV_NAME_INDEX 8

#define MAX_NUM 256

#define MAX_QUEUE_COUNT 20
#define MAX_CO_SIZE     896
#define MAX_NLMSG_LEN   1024

#define SDT_DRV_REPORT_NO_CONNECT_FREQUENCE 500

#define SDT_DRV_PKT_RECORD_MAX_NUM 100

/* ö�ٶ��� */
enum OM_NETLINK_MSG_TYPE_ENUM {
    NETLINK_MSG_HELLO = 0, /* netlink connect hello */
    NETLINK_MSG_SDTCMD,    /* std with device */
    NETLINK_MSG_SDTCMD_OPS /* device to SDT need encapsulation */
};

/* STRUCT ���� */
typedef struct {
    uint32_t ul_cmd;
    uint32_t ul_len;
} sdt_drv_netlink_msg_hdr_stru;

/* SDT��������PC�˵�����ͷ�ṹ��һ��8���ֽ� */
typedef struct {
    uint8_t uc_data_start_flg;          /* ���ݿ�ʼ�ı�־��ͬʱҲ�����ݽ����ı�־ */
    oam_data_type_enum_uint8 en_msg_type; /* ��������(LOG,EVENT,OTA��) */
    uint8_t uc_prim_id;                 /* ͨѶԭ�� */
    uint8_t uc_resv[1];                 /* ���� */
    uint8_t uc_data_len_low_byte;       /* ���ݳ��ȵĵ�8���� */
    uint8_t uc_data_len_high_byte;      /* ���ݳ��ȵĸ�8���� */
    uint8_t uc_sequence_num_low_byte;   /* ���кŵ�8���� */
    uint8_t uc_sequence_num_high_byte;  /* ���кŸ�8���� */
} sdt_drv_pkt_hdr_stru;

/* SDT DRV���ȫ�ֹ����ṹ */
typedef struct {
    oal_work_stru rx_wifi_work;
    oal_netbuf_head_stru rx_wifi_dbg_seq;
    oal_sock_stru *pst_nlsk;
    uint8_t *puc_data;
    uint32_t ul_usepid;
    uint16_t us_sn_num;
    uint8_t auc_resv[2]; /* ����oal_uint8���͵����飬Ԫ�ظ���Ϊ2 */
    oal_spin_lock_stru st_spin_lock;
    oal_atomic ul_unconnect_cnt; /* ͳ�ƽ���send����ʱnetlinkû�����ӵĴ��� */
} sdt_drv_mng_stru;

/* �������� */
int32_t sdt_drv_main_init(void);
void sdt_drv_main_exit(void);

#endif /* end of Sdt_drv.h */