/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#include <securec.h>
#include <bsp_nvim.h>
#include "nv_comm.h"
#include "nv_partition.h"

u32 nv_fac_flush(void)
{
    u32 ret, size;
    struct nvm_info *nvminfo = nv_get_nvminfo();
    u64 dtaddr = nvminfo->dtaddr;
    u64 dtsize = nvminfo->dtsize;

    if (dtaddr == 0) {
        nv_printf("nv memory init err\n");
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    size = nv_get_data_size(dtaddr);
    if (size > dtsize) {
        nv_printf("flush factory nv data overflow\n");
        return BSP_ERR_NV_OVER_MEM_ERR;
    }

    ret = nv_blk_write(NV_BLK_FACTORY, (u8 *)(uintptr_t)dtaddr, 0, size);
    if (ret) {
        nv_printf("nv write factory err, ret:0x%x\n", ret);
        return ret;
    }

    nv_debug_printf(NV_INFO, "flush factory success\n");
    return NV_OK;
}
