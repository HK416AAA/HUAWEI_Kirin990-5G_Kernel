/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <linux/mtd/mtd.h>
#include <linux/err.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <securec.h>
#include <bsp_print.h>
#include <bsp_partition.h>
#include <bsp_blk.h>

#define THIS_MODU mod_blk
#define BLK_PATH_LEN_MAX 128
#define FMC_BAD_BLK 0x12121212
#define BLK_OPS_FMC_ERASE_TRY_TIMES 2

struct rw_ops_para {
    u32 rwsize;
    u32 retlen;
    u32 start_offset;
    u32 data_len;
    u32 remainder;
    u8 *data_buf;
    u8 *tmp_buf;
};

enum {
    FMC_READ = 0,
    FMC_WRITE,
};

int fmc_blk_get_cdromiso_blkname(char *blk_path, int len)
{
    int ret;
    struct mtd_info *mtd = NULL;

    if (blk_path == NULL || len < BLK_PATH_LEN_MAX) {
        bsp_err("<%s> para err, len: %d\n", __func__, len);
        return -EINVAL;
    }

    mtd = get_mtd_device_nm(PTABLE_CDROMISO_NM);
    if (IS_ERR(mtd)) {
        bsp_err("<%s> get_mtd_device_nm(%s) error.\n", __func__, PTABLE_CDROMISO_NM);
        return PTR_ERR(mtd);
    }

    ret = snprintf_s(blk_path, BLK_PATH_LEN_MAX, BLK_PATH_LEN_MAX - 1, "/dev/block/mtdblock%d", mtd->index);
    if (ret < 0) {
        bsp_err("<%s> snprintf_s fail, ret %d.\n", __func__, ret);
        put_mtd_device(mtd);
        return ret;
    }
    put_mtd_device(mtd);
    return 0;
}
void fmc_rw_ops_para_set(struct rw_ops_para *fmc_rw_ops_para, u32 rwsize, u32 start_offset, u32 remainder,
                         u8 *data_buf)
{
    fmc_rw_ops_para->rwsize = rwsize;
    fmc_rw_ops_para->start_offset = start_offset;
    fmc_rw_ops_para->data_buf = data_buf;
    fmc_rw_ops_para->remainder = remainder;
}

int fmc_erase_ops(struct mtd_info *mtd, u64 addr)
{
    int ret;
    struct erase_info instr = {0};

    instr.mtd = mtd;
    instr.addr = addr;
    instr.len = mtd->erasesize;
    instr.retries = BLK_OPS_FMC_ERASE_TRY_TIMES;
    instr.callback = NULL;
    instr.priv = 0;

    ret = mtd_erase(mtd, &instr);
    if (ret) {
        bsp_debug("<%s> mtd_erase error, it's going to markbad. ret = %d, fail_addr = 0x%x\n", __func__, ret,
                  (u32)instr.fail_addr);
        ret = mtd_block_markbad(mtd, (long long)instr.addr);
        if (ret) {
            bsp_err("<%s> mtd_block_markbad error. ret = %d\n", __func__, ret);
            return -1;
        } else {
            return FMC_BAD_BLK;
        }
    }
    return 0;
}

int fmc_write_ops(struct mtd_info *mtd, struct rw_ops_para *p)
{
    int ret;
    size_t retlen = 0;
    long long addrblockalign;
    unsigned char *buffer = NULL;

    addrblockalign = (long long)(div_u64(p->start_offset, mtd->erasesize) * mtd->erasesize);
    if ((addrblockalign != p->start_offset) || (p->data_len < mtd->erasesize)) {
        ret = mtd_read(mtd, addrblockalign, mtd->erasesize, &retlen, p->tmp_buf);
        if ((ret && ret != -EUCLEAN) || (retlen != mtd->erasesize)) {
            bsp_err("<%s> mtd read error, ret = %d. retlen = %ld\n", __func__, ret, (unsigned long)retlen);
            return ret;
        }

        ret = memcpy_s(p->tmp_buf + p->remainder, (unsigned long)(p->rwsize), p->data_buf, (unsigned long)(p->rwsize));
        if (ret != EOK) {
            bsp_err("<%s> line %d memcpy_s err :%d\n", __func__, __LINE__, ret);
            return ret;
        }
        buffer = p->tmp_buf;
    } else {
        buffer = p->data_buf;
    }

    ret = fmc_erase_ops(mtd, addrblockalign);
    if (ret) {
        return ret;
    }

    ret = mtd_write(mtd, addrblockalign, mtd->erasesize, &retlen, (unsigned char *)buffer);
    if (ret || (retlen != mtd->erasesize)) {
        bsp_err("<%s> mtd_write error, it's going to markbad.ret = %d, retlen = %ld\n", __func__, ret,
                (unsigned long)retlen);
        ret = mtd_block_markbad(mtd, addrblockalign);
        if (ret) {
            bsp_err("<%s> mtd_block_markbad error. ret = %d\n", __func__, ret);
            return ret;
        } else {
            return FMC_BAD_BLK;
        }
    }

    return 0;
}

int fmc_read_ops(struct mtd_info *mtd, struct rw_ops_para *p)
{
    int ret;
    size_t retlen = 0;
    ret = mtd_read(mtd, (long long)p->start_offset, p->rwsize, &retlen, p->data_buf);
    if ((ret && ret != -EUCLEAN) || (retlen != p->rwsize)) {
        bsp_err("<%s> mtd read error, ret = %d, retlen = %d, rwsize = %ld\n", __func__, ret, (int)retlen,
                (long int)p->rwsize);
        return ret;
    }
    return 0;
}

int fmc_read_write_ops(struct mtd_info *mtd, struct rw_ops_para *p_para, u32 ops_type)
{
    int ret;
    u32 rwsize, start_blknum;
    u32 remainder = 0;
    u32 start_offset = 0;
    unsigned char *data_buf = NULL;

    start_blknum = div_u64_rem(p_para->start_offset, mtd->erasesize, &start_offset);
    data_buf = p_para->data_buf;
    while (p_para->data_len > 0) {
        if (start_offset >= mtd->size) {
            bsp_debug("<%s> partition offset 0x%x beyond partition\n", __func__, start_offset);
            return -EINVAL;
        }

        div_u64_rem(start_offset, mtd->erasesize, &remainder);
        ret = mtd_block_isbad(mtd, (long long)(start_offset - remainder));
        if (ret) {
            bsp_debug("<%s> bad block detected, partition offset 0x%x\n", __func__, start_offset - remainder);
            start_offset += mtd->erasesize;
            continue;
        }

        if (start_blknum != 0) {
            start_offset += mtd->erasesize;
            start_blknum--;
            continue;
        }

        rwsize = (remainder + p_para->data_len < mtd->erasesize) ? p_para->data_len : (mtd->erasesize - remainder);
        if (!rwsize) {
            bsp_err("<%s> rwsize is zero!\n", __func__);
            return -1;
        }

        fmc_rw_ops_para_set(p_para, rwsize, start_offset, remainder, data_buf);

        if (ops_type == FMC_READ)
            ret = fmc_read_ops(mtd, p_para);
        else if (ops_type == FMC_WRITE)
            ret = fmc_write_ops(mtd, p_para);
        if (ret == FMC_BAD_BLK) {
            start_offset += mtd->erasesize;
            continue;
        } else if (ret) {
            bsp_err("<%s> fmc read write failed, ops 0x%x \n", __func__, ops_type);
            return ret;
        }

        start_offset += rwsize;
        p_para->data_len = (p_para->data_len >= rwsize) ? p_para->data_len - rwsize : 0;
        data_buf += rwsize;
    }

    return 0;
}

int fmc_blk_read(const char *partition_name, loff_t partition_offset, void *data_addr, size_t data_len)
{
    int ret;
    struct mtd_info *mtd = NULL;
    struct rw_ops_para fmc_rw_ops_para = {0};

    if ((partition_name == NULL) || (data_addr == NULL)) {
        bsp_err("<%s> para error\n", __func__);
        return -EINVAL;
    }

    mtd = get_mtd_device_nm(partition_name);
    if (IS_ERR(mtd)) {
        bsp_err("<%s> get_mtd_device_nm(%s) error\n", __func__, partition_name);
        return PTR_ERR(mtd);
    }

    fmc_rw_ops_para.start_offset = partition_offset;
    fmc_rw_ops_para.data_len = data_len;
    fmc_rw_ops_para.data_buf = data_addr;
    fmc_rw_ops_para.tmp_buf = NULL;

    ret = fmc_read_write_ops(mtd, &fmc_rw_ops_para, FMC_READ);
    put_mtd_device(mtd);
    return ret;
}

int fmc_blk_write(const char *partition_name, loff_t partition_offset, const void *data_addr, size_t data_len)
{
    int ret;
    struct mtd_info *mtd = NULL;
    unsigned char *tmp_buf = NULL;
    struct rw_ops_para fmc_rw_ops_para = {0};

    if ((partition_name == NULL) || (data_addr == NULL)) {
        bsp_err("<%s> para error\n", __func__);
        return -EINVAL;
    }

    mtd = get_mtd_device_nm(partition_name);
    if (IS_ERR(mtd)) {
        bsp_err("<%s> get_mtd_device_nm(%s) error\n", __func__, partition_name);
        return PTR_ERR(mtd);
    }

    tmp_buf = (unsigned char *)vmalloc(mtd->erasesize);
    if (tmp_buf == NULL) {
        bsp_err("<%s> vmalloc failed!\n", __func__);
        put_mtd_device(mtd);
        return -ENOMEM;
    }

    fmc_rw_ops_para.start_offset = partition_offset;
    fmc_rw_ops_para.data_len = data_len;
    fmc_rw_ops_para.data_buf = (void *)data_addr;
    fmc_rw_ops_para.tmp_buf = tmp_buf;

    ret = fmc_read_write_ops(mtd, &fmc_rw_ops_para, FMC_WRITE);

    if (tmp_buf != NULL) {
        vfree(tmp_buf);
    }

    put_mtd_device(mtd);
    return ret;
}

int fmc_blk_erase(const char *partition_name)
{
    int ret;
    u64 count = 0;
    struct mtd_info *mtd = NULL;

    if (partition_name == NULL) {
        bsp_err("<%s> para err.\n", __func__);
        return -EINVAL;
    }

    mtd = get_mtd_device_nm(partition_name);
    if (IS_ERR(mtd)) {
        bsp_err("<%s> get_mtd_device_nm(%s) error\n", __func__, partition_name);
        return PTR_ERR(mtd);
    }

    while (count <= (mtd->size - mtd->erasesize)) {
        ret = mtd_block_isbad(mtd, (long long)(count));
        if (ret) {
            bsp_err("<%s> bad block detected, partition offset 0x%x\n", __func__, (u32)count);
            count += mtd->erasesize;
            ret = 0;
            continue;
        }

        ret = fmc_erase_ops(mtd, count);
        if (ret == FMC_BAD_BLK) {
            count += mtd->erasesize;
            ret = 0;
            continue;
        } else if (ret) {
            bsp_debug("<%s> fmc_erase_ops err, partition = %s, offset = 0x%x, ret %d\n", __func__, partition_name,
                      (u32)count, ret);
            break;
        }
        count += mtd->erasesize;
    }

    put_mtd_device(mtd);
    return ret;
}

int fmc_blk_isbad(const char *partition_name, loff_t partition_offset)
{
    int ret;
    struct mtd_info *mtd = NULL;

    if (partition_name == NULL) {
        bsp_err("<%s> part name error.\n", __func__);
        return -EINVAL;
    }

    mtd = get_mtd_device_nm(partition_name);
    if (IS_ERR(mtd)) {
        bsp_err("<%s> get_mtd_device_nm(%s) error\n", __func__, partition_name);
        return PTR_ERR(mtd);
    }

    ret = mtd_block_isbad(mtd, (long long)partition_offset);
    if (ret) {
        bsp_debug("<%s> bad block detected, partition offset 0x%x\n", __func__, (unsigned int)partition_offset);
    }

    put_mtd_device(mtd);
    return ret;
}

int fmc_blk_size(const char *part_name, u32 *size)
{
    struct mtd_info *mtd = NULL;

    if ((part_name == NULL) || (size == NULL)) {
        bsp_err("<%s> param error\n", __func__);
        return -1;
    }

    mtd = get_mtd_device_nm(part_name);
    if (IS_ERR(mtd)) {
        bsp_err("<%s> get_mtd_device_nm(%s) error\n", __func__, part_name);
        return PTR_ERR(mtd);
    }

    *size = (u32)mtd->size;

    put_mtd_device(mtd);
    return 0;
}

struct block_ops g_block_ops_fmc = {
    .read = fmc_blk_read,
    .write = fmc_blk_write,
    .erase = fmc_blk_erase,
    .isbad = fmc_blk_isbad,
    .get_part_size = fmc_blk_size,
    .get_cdromiso_blkname = fmc_blk_get_cdromiso_blkname,
};

int fmc_init(void)
{
    if (bsp_blk_is_support() != BLOCK_BALONG_IS_SUPPORT) {
        bsp_err("<%s> flash is not needed\n", __func__);
        return 0;
    }
    if (!bsp_boot_mode_is_fmc()) {
        return 0;
    }
    if (bsp_blk_register_ops(&g_block_ops_fmc)) {
        bsp_err("<%s> bsp_blk_register_ops failed\n", __func__);
        return -1;
    }
    return 0;
}

fs_initcall(fmc_init);

