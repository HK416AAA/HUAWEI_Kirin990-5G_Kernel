/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#include <securec.h>
#include <bsp_rfile.h>
#include <bsp_nvim.h>
#include "nv_comm.h"
#include "nv_partition.h"

char g_img_file[NV_IMGFILE_MAX][NV_IMGFILE_MAX_LEN] = {
    "nv_ctrl.bin",
    "nv_resume.bin",
    "nv_rdonly.bin",
    "nv_rdwr.bin",
    "tag.bin",
    "nv_rdwr.bk",
    "carrier/nv_comm.bin",
    "carrier/nv_carrier.bin",
    "nv_flag.bin"
};

u32 nv_image_path_init(struct nvm_fs *nvmfs)
{
    u32 ret;
    u32 i;
    struct image_partition *image = &nvmfs->image;
    char *mountpoint = image->pinfo.mountpoint;
    char *filename = NULL;

    ret = strncpy_s(mountpoint, NV_PATH_LEN, nvmfs->mount_path, \
            strnlen((char *)nvmfs->mount_path, NV_MOUNT_PATH_LEN));
    if (ret) {
        nv_printf("copy image mountpoint fail, ret:0x%x\n", ret);
        return ret;
    }

    ret = strncat_s(mountpoint, NV_PATH_LEN, (char *)NV_IMAGE_MOUNTPOINT, \
            strnlen((char *)NV_IMAGE_MOUNTPOINT, NV_MOUNT_PATH_LEN));
    if (ret) {
        nv_printf("strncat image mountpoint fail, ret:0x%x\n", ret);
        return ret;
    }

    nv_printf("nv image mountpoint:%s\n", mountpoint);
    for (i = 0; i < NV_IMGFILE_MAX; i++) {
        image->finfo[i].type = i;
        filename = image->finfo[i].filename;
        ret = strncpy_s(filename, NV_PATH_LEN, mountpoint, strnlen(mountpoint, NV_MOUNT_PATH_LEN));
        if (ret) {
            nv_printf("copy image %d filename fail, ret:0x%x\n", i, ret);
            return ret;
        }

        ret = strncat_s(filename, NV_PATH_LEN, g_img_file[i], strnlen(g_img_file[i], NV_MOUNT_PATH_LEN));
        if (ret) {
            nv_printf("strncat image %d filename fail, ret:0x%x\n", i, ret);
            return ret;
        }
        nv_printf("nv %d filename:%s\n", i, filename);
    }

    image->fcnt = NV_IMGFILE_MAX;
    return NV_OK;
}

static u32 nv_img_write(s8 *filename, s8 *buf, u32 length)
{
    u32 ret;
    int fp;
    s32 flags = RFILE_CREAT | RFILE_TRUNC | RFILE_RDWR;

    fp = nv_file_open((s8 *)filename, flags, 0660);
    if (fp < 0) {
        nv_printf("create nv file err:%d\n", fp);
        return (u32)fp;
    }

    ret = (u32)nv_file_write((u32)fp, buf, length);
    nv_file_close((u32)fp);
    if (ret != length) {
        nv_printf("write nv file:%s err:%d\n", filename, ret);
        return ret;
    }

    nv_debug_printf(NV_INFO, "write %s image file success\n", filename);
    return NV_OK;
}

/* write section data to each file */
static u32 nv_img_snwrite(u32 filetype)
{
    u32 ret, sntype, length;
    u32 offset = 0;
    s8 *buf = NULL;
    char *filename = NULL;
    struct section_info *sninfo = NULL;
    u64 dtaddr = nv_get_mem_addr();
    struct nv_ctrl_info *ctrl_info = nv_get_ctrl_info();
    struct image_partition *image = nv_get_image_partition();

    if ((dtaddr == 0) || (ctrl_info == NULL)) {
        nv_printf("nv data memory init err\n");
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    sninfo = nv_get_sninfo();

    switch (filetype) {
        case NV_HD:
            sntype = NV_SECTION_HD;
            buf = (s8 *)((uintptr_t)dtaddr + offset);
            length = ctrl_info->hdsize;
            filename = image->finfo[filetype].filename;
            break;
        case NV_RS:
        case NV_RO:
        case NV_RW:
        case NV_TAG:
            sntype = filetype;
            offset = sninfo[sntype - 1].offset;
            buf = (s8 *)((uintptr_t)dtaddr + offset);
            length = sninfo[sntype - 1].length;
            filename = image->finfo[filetype].filename;
            break;
        case NV_RWBK:
            sntype = NV_SECTION_RW;
            offset = sninfo[sntype - 1].offset;
            buf = (s8 *)((uintptr_t)dtaddr + offset);
            length = sninfo[sntype - 1].length;
            filename = image->finfo[filetype].filename;
            break;
        default:
            return BSP_ERR_NV_NO_FILE;
    }

    ret = nv_img_write(filename, buf, length);
    if (ret != NV_OK) {
        nv_printf("write nv file:%s err\n", filename);
        return ret;
    }

    return NV_OK;
}

static u32 nv_img_single_rm(u32 type)
{
    u32 ret;
    struct image_partition *image = nv_get_image_partition();

    if (type >= NV_IMGFILE_MAX) {
        nv_printf("rm invalid %d image file type\n", type);
        return BSP_ERR_NV_INVALID_PARAM;
    }

    ret = (u32)nv_file_remove((s8 *)image->finfo[type].filename);
    if (ret) {
        nv_printf("remove image %s file err,ret:%d\n", image->finfo[type].filename, ret);
    }

    return ret;
}

static u32 nv_img_flag_read(void)
{
    u32 ret;
    u32 flag = 0;
    int fp;
    struct image_partition *image = nv_get_image_partition();
    char *filename = image->finfo[NV_FLG].filename;

    fp = nv_file_open((s8 *)filename, RFILE_RDONLY, 0640);
    if (fp < 0) {
        nv_printf("open %s rd file fail:%d\n", filename, fp);
        return (u32)fp;
    }

    ret = (u32)nv_file_read((u32)fp, (s8 *)&flag, 0, sizeof(u32));
    nv_file_close((u32)fp);
    if (ret != sizeof(u32)) {
        nv_printf("read %s file fail:%d\n", filename, fp);
        return ret;
    }

    return flag;
}

static u32 nv_img_flag_write(u32 flag)
{
    u32 ret;
    struct image_partition *image = nv_get_image_partition();
    char *filename = image->finfo[NV_FLG].filename;

    ret = nv_img_write((s8 *)filename, (s8 *)&flag, sizeof(u32));
    if (ret != NV_OK) {
        nv_printf("write %s file fail\n", filename);
        return ret;
    }

    return NV_OK;
}

u32 nv_img_rmflg(void)
{
    return nv_img_single_rm(NV_FLG);
}

static u32 nv_img_rwwrite(void)
{
    u32 ret;

    if (nv_img_flag_read() == NVM_IMG_BIN) {
        ret = nv_img_snwrite(NV_RWBK);
        if (ret != NV_OK) {
            nv_printf("write image rw bak err\n");
            return ret;
        }

        ret = nv_img_flag_write(NVM_IMG_BAK);
        if (ret != NV_OK) {
            nv_printf("write image flag bak err\n");
            return ret;
        }
    } else {
        ret = nv_img_snwrite(NV_RW);
        if (ret != NV_OK) {
            nv_printf("write image rw bin err\n");
            return ret;
        }

        ret = nv_img_flag_write(NVM_IMG_BIN);
        if (ret != NV_OK) {
            nv_printf("write image flag bin err\n");
            return ret;
        }
    }

    return NV_OK;
}

static u32 nv_img_rwbuf_write(u32 type, u32 buf)
{
    u32 ret, sntype, length;
    s8 *virbuf = NULL;
    s8 *filename = NULL;
    u64 dtaddr = nv_get_mem_addr();
    struct section_info *sninfo = nv_get_sninfo();
    struct image_partition *image = nv_get_image_partition();

    if ((dtaddr == 0) || (sninfo == NULL)) {
        nv_printf("nv data memory init err\n");
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    if ((type != NV_RW) && (type != NV_RWBK)) {
        nv_printf("invalid rwbuf file type");
        return BSP_ERR_NV_INVALID_PARAM;
    }

    sntype = NV_SECTION_RW;
    length = sninfo[sntype - 1].length;
    filename = image->finfo[type].filename;
    virbuf = (s8 *)(uintptr_t)SHD_DDR_P2V(buf);

    nv_debug_printf(NV_INFO, "virbuf:0x%llx, phy buf:0x%x\n", (u64)(uintptr_t)virbuf, buf);
    ret = nv_img_write(filename, virbuf, length);
    if (ret != NV_OK) {
        nv_printf("write rwbuf nv file:%s err\n", filename);
        return ret;
    }

    return NV_OK;
}

u32 nv_img_rwflush(u32 buf)
{
    u32 ret;

    if (nv_img_flag_read() == NVM_IMG_BIN) {
        ret = nv_img_rwbuf_write(NV_RWBK, buf);
        if (ret != NV_OK) {
            nv_printf("write image rw bak err\n");
            return ret;
        }

        ret = nv_img_flag_write(NVM_IMG_BAK);
        if (ret != NV_OK) {
            nv_printf("write image flag bak err\n");
            return ret;
        }
    } else {
        ret = nv_img_rwbuf_write(NV_RW, buf);
        if (ret != NV_OK) {
            nv_printf("write image rw bin err\n");
            return ret;
        }

        ret = nv_img_flag_write(NVM_IMG_BIN);
        if (ret != NV_OK) {
            nv_printf("write image flag bin err\n");
            return ret;
        }
    }

    return NV_OK;
}

/* flush section data to image file */
u32 nv_img_single_flush(u32 type)
{
    u32 ret = 0;

    switch (type) {
        case NV_HD:
        case NV_RS:
        case NV_RO:
        case NV_TAG:
            ret = nv_img_snwrite(type);
            break;
        case NV_RW:
        case NV_RWBK:
            ret = nv_img_rwwrite();
            break;
        default:
            nv_printf("nv flush invalid img type\n");
            return BSP_ERR_NV_NOT_SUPPORT_ERR;
    }

    if (ret != NV_OK) {
        nv_printf("nv flush %d img err\n", type);
        return ret;
    }

    return NV_OK;
}

u32 nv_img_tag_flush(void)
{
    u32 ret;

    ret = nv_img_single_flush(NV_TAG);
    if (ret != NV_OK) {
        nv_printf("nv img tag file flush err\n");
        return ret;
    }

    return NV_OK;
}

u32 nv_img_rm_tag(void)
{
    s32 ret;
    struct image_partition *image = nv_get_image_partition();

    ret = nv_file_remove((s8 *)image->finfo[NV_TAG].filename);
    if (ret) {
        nv_printf("remove image tag file err,ret:%d\n", ret);
    }

    return ret;
}

/* flush nv data to image not include tag section */
u32 nv_img_flush_data(void)
{
    u32 ret = 0;
    u32 i;
    struct image_partition *image = nv_get_image_partition();

    for (i = 0; i <= NV_RW; i++) {
        ret = nv_img_single_flush(i);
        if (ret != NV_OK) {
            nv_printf("nv img file:%s write err\n", image->finfo[i].filename);
            return ret;
        }

        nv_printf("nv img file:%s write success\n", image->finfo[i].filename);
    }

    return NV_OK;
}

u32 nv_img_flush_base(void)
{
    u32 ret = 0;
    u32 i;
    struct image_partition *image = nv_get_image_partition();

    for (i = 0; i <= NV_TAG; i++) {
        ret = nv_img_single_flush(i);
        if (ret != NV_OK) {
            nv_printf("nv img file:%s write err\n", image->finfo[i].filename);
            return ret;
        }

        nv_printf("nv img file:%s write success\n", image->finfo[i].filename);
    }

    return NV_OK;
}

u32 nv_img_flush_carrier(void)
{
    u32 i, ret, length;
    u64 mbnaddr = nv_get_mbnaddr();
    struct section_info *sninfo = NULL;
    struct nv_ctrl_info *ctrl_info = (struct nv_ctrl_info *)(uintptr_t)mbnaddr;
    struct image_partition *image = nv_get_image_partition();

    if (ctrl_info->magicnum != NV_HEAD_MAGICNUM) {
        nv_printf("nv carrier data null, no need flush\n");
        return NV_OK;
    }

    sninfo = (struct section_info *)((uintptr_t)ctrl_info + ctrl_info->snidxoff);

    length = ctrl_info->hdsize;
    for (i = 0; i < ctrl_info->sncnt; i++) {
        length += sninfo[i].length;
    }

    if (length > NV_MBN_SIZE) {
        nv_printf("nv carrier data over size err\n");
        return BSP_ERR_NV_OVER_MEM_ERR;
    }

    ret = nv_img_write(image->finfo[NV_CCM].filename, (s8 *)(uintptr_t)mbnaddr, length);
    if (ret != NV_OK) {
        nv_printf("nv carrier data write err\n");
        return ret;
    }

    nv_printf("flush image carrier comm success\n");
    return NV_OK;
}

/* remove hd,rs,ro,rw,tag,rwbak,flg image file */
void nv_img_rm_all(void)
{
    u32 i;
    s32 ret = 0;

    for (i = 0; i < NV_IMGFILE_MAX; i++) {
        ret = nv_img_single_rm(i);
        if (ret) {
            nv_printf("remove image %d file fail(ignore)\n", i);
        }
    }
}
