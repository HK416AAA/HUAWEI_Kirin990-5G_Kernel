/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#include <securec.h>
#include "nv_comm.h"
#include "nv_partition.h"
#include "nv_debug.h"

u32 nv_data_writeback(void)
{
    u32 ret;

    nv_img_rm_all();

    ret = nv_img_flush_carrier();
    if (ret != NV_OK) {
        nv_printf("nv img carrier write back err\n");
        return ret;
    }

    ret = nv_img_flush_data();
    if (ret != NV_OK) {
        nv_printf("nv img data write back err\n");
        return ret;
    }

    ret = nv_bak_flush();
    if (ret != NV_OK) {
        nv_printf("nv bak data write back err\n");
        return ret;
    }

    return NV_OK;
}

u32 nv_clear_upgrade_flag(void)
{
    u32 ret;

    ret = nv_img_tag_flush();
    if (ret != NV_OK) {
        nv_printf("nv clear upgrade flag err\n");
        return ret;
    }

    nv_printf("nv img tag.bin file write success\n");
    return NV_OK;
}

u32 nv_set_upgrade_flag(void)
{
    u32 ret;

    ret = (s32)nv_img_rm_tag();
    if (ret != NV_OK) {
        nv_printf("nv set upgrade flag err\n");
        return ret;
    }

    return NV_OK;
}

u32 nv_upgrade_flush(void)
{
    u32 ret;

    nv_printf("nv upgrade flush start\n");

    ret = nv_data_writeback();
    if (ret) {
        nv_printf("nv upgrade write back failed, ret:0x%x\n", ret);
        return ret;
    }

    ret = nv_clear_upgrade_flag();
    if (ret) {
        nv_printf("nv upgrade clear upgrade flag failed, ret:0x%x\n", ret);
        return ret;
    }

    nv_printf("nv upgrade flush end\n");
    return NV_OK;
}
