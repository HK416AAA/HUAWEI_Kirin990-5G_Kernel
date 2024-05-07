/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#include <securec.h>
#include <bsp_slice.h>
#include <bsp_nvim.h>
#include "nv_comm.h"
#include "nv_msg.h"
#include "nv_partition.h"

u32 nv_flush_data(u32 msg)
{
    u32 i;
    u32 ret;

    for (i = 0; i < NV_SECTION_END; i++) {
        if (msg & ((u32)1 << i)) {
            ret = nv_img_single_flush(i);
            if (ret != NV_OK) {
                nv_printf("nv image %d file err\n", i);
                return ret;
            }
        }
    }

    return NV_OK;
}

u32 nv_flush_rwbuf(u32 buf)
{
    u32 ret;

    ret = nv_img_rwflush(buf);
    if (ret != NV_OK) {
        nv_printf("flush rwbuf err\n");
        return ret;
    }

    return NV_OK;
}

/* need flush image after reload backup */
u32 nv_reload_flush(void)
{
    u32 ret;

    ret = nv_img_flush_base();
    if (ret != NV_OK) {
        nv_printf("[reload bak]flush img base err\n");
        return ret;
    }

    nv_printf("reload backup flush image success\n");
    return NV_OK;
}

u32 nv_update_factory(void)
{
    u32 ret;

    nv_printf("^INFORBU: factory bakup start!\n");

    ret = nv_fac_flush();
    if (ret) {
        nv_printf("^INFORBU: flush factory nv faild! 0x%x\n", ret);
        return ret;
    }

    ret = nv_bak_flush();
    if (ret) {
        nv_printf("^INFORBU: flush backup nv failed! 0x%x\n", ret);
        return ret;
    }

    nv_printf("^INFORBU: factory bakup end!\n");

    return NV_OK;
}

u32 nv_update_backup(void)
{
    u32 ret;

    nv_printf("^NVBACKUP: bakup start!\n");
    ret = nv_img_flush_base();
    if (ret) {
        nv_printf("^NVBACKUP: flush image nv failed! ret :0x%x\n", ret);
        return ret;
    }

    ret = nv_bak_flush();
    if (ret) {
        nv_printf("^NVBACKUP: flush backup nv failed! 0x%x\n", ret);
        return ret;
    }

    nv_printf("^NVBACKUP: bakup end!\n");
    return NV_OK;
}

u32 nv_factory_reset(void)
{
    u32 ret;

    nv_printf("nv factory reset start\n");
    ret = nv_bak_flush();
    if (ret) {
        nv_printf("nv factory reset flush backup failed, ret:0x%x\n", ret);
        return ret;
    }

    ret = nv_set_upgrade_flag();
    if (ret) {
        nv_printf("nv factory reset set upgrade flag failed, ret:0x%x\n", ret);
        return ret;
    }

    nv_printf("nv factory reset end\n");

    return NV_OK;
}
