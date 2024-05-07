/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#include <securec.h>
#include <product_config.h>
#ifdef BSP_CONFIG_PHONE_TYPE
#include <adrv.h>
#else
#include <bsp_partition.h>
#endif
#include <mdrv_eicc.h>
#include <bsp_eicc.h>
#include <bsp_slice.h>
#include <bsp_blk.h>
#include <bsp_rfile.h>
#include <bsp_nvim.h>
#include "nv_comm.h"
#include "nv_msg.h"
#include "nv_partition.h"

static char g_blk_info[NV_BLK_MAX][BLK_MAX_NAME] = {
#ifdef BSP_CONFIG_PHONE_TYPE
    PARTITION_MODEM_NVBACKUP_NAME,
    PARTITION_MODEM_NVDEFAULT_NAME
#else
    PTABLE_NVBACK_NM,
    PTABLE_NVDEFAULT_NM
#endif
};

struct nvm_fs *nv_get_nvmfs(void)
{
    struct nvm_info *nvminfo = nv_get_nvminfo();

    return &nvminfo->nvmfs;
}

struct image_partition *nv_get_image_partition(void)
{
    struct nvm_fs *nvmfs = nv_get_nvmfs();

    return &nvmfs->image;
}

u32 nv_blk_read(u32 partition, u8 *buf, u32 offset, u32 size)
{
    return (u32)bsp_blk_read(g_blk_info[partition], offset, buf, size);
}

u32 nv_blk_write(u32 partition, u8 *buf, u32 offset, u32 size)
{
    return (u32)bsp_blk_write(g_blk_info[partition], offset, buf, size);
}

s32 nv_file_access(s8 *file, s32 mode)
{
    return bsp_access(file, mode);
}

s32 nv_file_remove(s8 *file)
{
    return bsp_remove(file);
}

s32 nv_file_open(s8 *file, s32 flags, s32 mode)
{
    return bsp_open(file, flags, mode);
}

s32 nv_file_read(u32 fp, s8 *buf, u32 offset, u32 size)
{
    s32 ret;

    ret = bsp_lseek(fp, offset, SEEK_SET);
    if (ret < 0) {
        nv_printf("seek nv file fail, ret:%d\n", ret);
        return ret;
    }

    return bsp_read(fp, buf, size);
}

s32 nv_file_write(u32 fp, s8 *buf, u32 size)
{
    return bsp_write_sync(fp, buf, size);
}

void nv_file_close(u32 fp)
{
    s32 ret;
    ret = bsp_close(fp);
    if (ret) {
        nv_printf("close fp:%d fail, ret:%d\n", fp, ret);
    }
}

u32 nv_load_block(struct nv_msg_data *msg_data)
{
    u32 ret = 0;
    struct nvm_info *nvminfo = nv_get_nvminfo();
    struct nv_rdblk_info *blk = (struct nv_rdblk_info *)msg_data->data;
    struct nv_msg_data *msg = NULL;

    nv_debug_printf(NV_DBG, "read partition %d offset:0x%x, size:0x%x\n", blk->partition, blk->offset, blk->size);
    if ((blk->magicnum != NV_PARTITION_MAGICNUM) || (blk->partition >= NV_BLK_MAX) || (blk->size == 0) ||
        (blk->size > NV_EICC_MAX_LEN)) {
        nv_printf("invalid blk info\n");
        ret = BSP_ERR_NV_DATA_MAGICNUM_ERR;
        goto out;
    }

    msg = nv_malloc(sizeof(struct nv_msg_data) + blk->size);
    if (msg == NULL) {
        nv_printf("nv load block malloc fail\n");
        ret = BSP_ERR_NV_MALLOC_FAIL;
        goto out;
    }

    msg->magicnum = NV_MSG_MAGICNUM;
    msg->slice = bsp_get_slice_value();
    msg->msg_type = NV_ICC_CNF;
    msg->msg = 0;
    msg->sn = msg_data->sn;
    msg->rcvlen = 0;

    ret = nv_blk_read(blk->partition, msg->data, blk->offset, blk->size);
    if (ret) {
        nv_free(msg);
        nv_printf("nv load block read fail, ret:%d\n", ret);
        goto out;
    }

    msg->data_len = blk->size;
    msg->ret = 0;

    nv_debug_printf(NV_DBG, "send read data sn:0x%x, msg type:0x%x, private data len:0x%x\n", msg->sn, msg->msg_type,
        msg->data_len);
    ret = nv_cpmsg_send((u8 *)msg, sizeof(struct nv_msg_data) + blk->size);
    if (ret) {
        nv_printf("nv load block send sp msg fail\n");
    }

    nv_free(msg);
out:
    __pm_relax(&nvminfo->wakelock);
    nvminfo->msg_info.cb_reply++;
    return ret;
}

static u32 nv_mountpath_init(struct nvm_fs *nvmfs)
{
    u32 ret;

    ret = (u32)nv_file_access((s8 *)NV_ROOT_PATH, 0);
    if (ret) {
        nv_printf("can't access nvm fs mountpath:%s\n", NV_ROOT_PATH);
        return ret;
    }

    ret = strncpy_s((char *)nvmfs->mount_path, NV_PATH_LEN, (char *)NV_ROOT_PATH, \
            strnlen((char *)NV_ROOT_PATH, NV_MOUNT_PATH_LEN));
    if (ret) {
        nv_printf("copy mount path fail, ret:0x%x\n", ret);
        return ret;
    }

    return NV_OK;
}

static u32 nvmfs_path_init(struct nvm_fs *nvmfs)
{
    u32 ret;

    ret = nv_mountpath_init(nvmfs);
    if (ret != NV_OK) {
        nv_printf("nv mountpath init fail\n");
        return ret;
    }

    ret = nv_image_path_init(nvmfs);
    if (ret != NV_OK) {
        nv_printf("nv image path init fail\n");
        return ret;
    }

    return NV_OK;
}

u32 nv_nvmfs_init(void)
{
    u32 ret;
    struct nvm_fs *nvmfs = nv_get_nvmfs();

    ret = nvmfs_path_init(nvmfs);
    if (ret != NV_OK) {
        nv_printf("nvmfs path init fail\n");
        return ret;
    }

    nv_printf("nv file system init success\n");
    return NV_OK;
}

u32 nv_get_data_size(u64 dtaddr)
{
    u32 i;
    u32 size;
    struct nv_ctrl_info *ctrl_info = (struct nv_ctrl_info *)(uintptr_t)dtaddr;
    struct section_info *sninfo = NULL;

    if (ctrl_info->magicnum != NV_HEAD_MAGICNUM) {
        nv_printf("invalid nv memory data\n");
        return NV_ERROR;
    }

    size = ctrl_info->hdsize;
    sninfo = (struct section_info *)(dtaddr + ctrl_info->snidxoff);

    for (i = 0; i < ctrl_info->sncnt; i++) {
        size += sninfo[i].length;
    }

    return size;
}
