/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#ifndef __NV_PARTITION_H__
#define __NV_PARTITION_H__

#include "nv_msg.h"
#include "bsp_nvim.h"

#define NV_PARTITION_MAGICNUM (0x6e764e56)
#define NV_HEAD_MAGICNUM (0x224e4944)

#define BLK_MAX_NAME (64)

typedef struct _nv_wr_req {
    u32 itemid;
    u32 modemid;
    u32 offset;
    u8 *pdata;
    u32 size;
} nv_rdwr_req;

#define NV_MAX_MODEM_NUM 3
struct item_info {
    u32 itemid;             /* NV ID */
    u32 offset[NV_MAX_MODEM_NUM];          /* offset of modem0~2 */
    u16 length;             /* length of nv */
    u8  modem_num;          /* modem num */
    u8  priority;           /* write priority */
    u8  resume;             /* resume flag */
    u8  rsv[0x2];             /* reserve */
    u8  snid;               /* ID of section of this nv */
};

/* nv filesystem have rs,ro,rw,tag section, head is a special section
 * enum nv_section_type must be same order as enum nv_image_type
 */
enum nv_section_type {
    NV_SECTION_HD = 0,    /* nv head section */
    NV_SECTION_RS = 1,    /* nv resume section */
    NV_SECTION_RO = 2,    /* nv read only section */
    NV_SECTION_RW = 3,    /* nv read/write secion */
    NV_SECTION_TAG = 4,   /* nv tag section */
    NV_SECTION_END
};

/* nv file in image partition */
enum nv_image_type {
    NV_HD = 0,   /* nv data head file */
    NV_RS,
    NV_RO,
    NV_RW,
    NV_TAG,
    NV_RWBK,
    NV_CCM,
    NV_CCR,
    NV_FLG,
    NV_IMGFILE_MAX
};

#define NV_IMGFILE_MAX_LEN 32

#define NV_SECTION_NAME_LEN 20
struct section_info {
    u8 id;
    u8 type;
    u8 rsv[0x2];
    u8 name[NV_SECTION_NAME_LEN];
    u32 itmcnt;    /* count of items section contains */
    u32 offset;
    u32 length;
};

struct nv_ctrl_info {
    u32  magicnum;
    u32  hdsize;                        /* head data size, include section and item index info */
    u8   version[NV_FILE_VER_LEN];                    /* head version */
    u8   modemnum;                      /* modem num */
    u8   crc_mark;
    u32  snidxoff;                      /* offset of section index */
    u32  sncnt;                         /* section cnt */
    u32  snidxsize;                     /* section index size */
    u32  itmidxoff;                     /* offset of item index */
    u32  itmcnt;                        /* item cnt */
    u32  itmidxsize;                    /* item index size */
    u8   rsv[0xC];
    u32  timetag[NV_TIME_TAG_LEN];                    /* time stamp */
    u32  product_version[NV_PRDT_VER_LEN];            /* product version */
};

#define NV_TAG_DATA_LEN 8
#define NV_TAG_TIME_LEN 4
struct tag_info {
    u32 data[NV_TAG_DATA_LEN];
    u32 time[NV_TAG_TIME_LEN];
};
#ifdef BSP_CONFIG_PHONE_TYPE
#define NV_ROOT_PATH "/mnt/modem/modem_nv/"

#else
#define NV_ROOT_PATH "/modem_nv/"
#endif
#define NV_PATH_LEN 64
#define NV_MOUNT_PATH_LEN 32

struct partition_info {
    char mountpoint[NV_PATH_LEN];
    u32 magicnum;
    u32 dtoff;    /* data offset */
    u32 size;    /* not include verify data size */
};

#define NV_IMAGE_MOUNTPOINT "image/"

#define NV_IMAGE_MAGICNUM (0x764e6e56)

struct img_file_info {
    u32 type;
    u32 magicnum;
    char filename[NV_PATH_LEN];
};

struct image_partition {
    u32 fcnt;
    struct img_file_info finfo[NV_IMGFILE_MAX];
    struct tag_info tag[0x2];
    struct partition_info pinfo;
};

#define NV_FS_VER_LEN 8
struct nvm_fs {
    char mount_path[NV_PATH_LEN];
    u32 sncnt;    /* section cnt */
    u32 itmcnt;   /* item cnt */
    u32 modemnum;
    u32 dtlen;    /* nv data length, include ctrl info, nv data, tag data */
    u32 version[NV_FS_VER_LEN];
    struct image_partition image;
};

enum {
    NV_BLK_BACKUP = 0,
    NV_BLK_FACTORY = 1,
    NV_BLK_MAX
};

struct nv_rdblk_info {
    u32 magicnum;
    u32 partition;
    u32 offset;
    u32 size;
};

struct nvm_fs *nv_get_nvmfs(void);

struct image_partition *nv_get_image_partition(void);

u32 nv_blk_read(u32 partition, u8 *buf, u32 offset, u32 size);

u32 nv_blk_write(u32 partition, u8 *buf, u32 offset, u32 size);

s32 nv_file_access(s8 *file, s32 mode);

s32 nv_file_remove(s8 *file);

s32 nv_file_open(s8 *file, s32 flags, s32 mode);

s32 nv_file_read(u32 fp, s8 *buf, u32 offset, u32 size);

s32 nv_file_write(u32 fp, s8 *buf, u32 size);

void nv_file_close(u32 fp);

u32 nv_load_block(struct nv_msg_data *msg_data);

u32 nv_image_path_init(struct nvm_fs *nvmfs);

u32 nv_img_rmflg(void);

u32 nv_img_rwflush(u32 buf);

void nv_img_rmrw(void);

u32 nv_img_single_flush(u32 type);

u32 nv_img_tag_flush(void);

u32 nv_img_rm_tag(void);

u32 nv_img_flush_data(void);

void nv_img_rm_data(void);

u32 nv_img_flush_base(void);

u32 nv_img_flush_carrier(void);

void nv_img_rm_all(void);

u32 nv_bak_flush(void);

u32 nv_fac_flush(void);

u32 nv_nvmfs_init(void);

u32 nv_get_data_size(u64 dtaddr);
#endif
