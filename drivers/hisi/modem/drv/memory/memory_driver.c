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

#include "memory_driver.h"
#include <product_config.h>
#include <linux/version.h>
#include <bsp_dt.h>
#include <linux/module.h>
#include <linux/io.h>
#include <securec.h>

#undef THIS_MODU
#define THIS_MODU mod_mem
mem_mgr_info_s g_bydts_mem_info[SHM_ATTR_MAX] = {};

#define MEM_INFO_GET_DTSINFO(x) (g_bydts_mem_info[x].dts_info)
#define MEM_INFO_GET_ADDR(x) (g_bydts_mem_info[x].base_addr)
#define MEM_INFO_GET_VIRT_ADDR(x) (g_bydts_mem_info[x].virt_base_addr)
#define MEM_INFO_GET_SIZE(x) (g_bydts_mem_info[x].size)
#define MEM_INFO_GET_DTSINFO_SIZE(x) (g_bydts_mem_info[x].dts_info_size)
#define MEM_NODE_GET_OFFSET(dts_info, i) ((dts_info) + (i))

#ifdef BSP_CONFIG_PHONE_TYPE
#define SHM_DTS_MAGIC_NUM 0xFDFDFDFD

static s32 meminfo_of_parse(const char* memid_name, unsigned int mem_id)
{
    u32 ret = 0;
    u32 mem_offset = 0;
    device_node_s *dev_node = NULL;
    device_node_s *child_node = NULL;
    const char* mem_name = NULL;
    u32 mem_size;
    mem_err("[memory]meminfo_of_parse begin\n");
    mem_mgr_node_info_s *dts_info = MEM_INFO_GET_DTSINFO(mem_id);
    dev_node = bsp_dt_find_compatible_node(NULL, NULL, memid_name);
    if (dev_node == NULL) {
        mem_err("fail to get %s dts!\n", memid_name);
        return MEM_ERROR;
    }
    bsp_dt_for_each_child_of_node(dev_node, child_node) {
        if (bsp_dt_property_read_string(child_node, (const char*)"mem-names", &mem_name)) {
            mem_err("fail to read mem-names!\n");
            return MEM_ERROR;
        }
        if (strcpy_s((void*)(MEM_NODE_GET_OFFSET(dts_info, ret))->name,
            sizeof((MEM_NODE_GET_OFFSET(dts_info, ret))->name), (void*)mem_name)) {
            mem_err("mem_name strcpy_s get failed!\n");
            return MEM_ERROR;
        }
        if (bsp_dt_property_read_u32(child_node, (const char*)"size", &mem_size)) {
            mem_err("mem_size get failed!\n");
            return MEM_ERROR;
        }
        (MEM_NODE_GET_OFFSET(dts_info, ret))->size = mem_size;
        (MEM_NODE_GET_OFFSET(dts_info, ret))->offset = mem_offset;
        (MEM_NODE_GET_OFFSET(dts_info, ret))->magic = SHM_DTS_MAGIC_NUM;
        mem_offset = mem_offset + mem_size;
        ret++;
        if (ret >= MEM_INFO_GET_DTSINFO_SIZE(mem_id) / sizeof(mem_mgr_node_info_s)) {
            mem_err("node num overflowed\n");
            return MEM_ERROR;
        }
        if (mem_offset >= MEM_INFO_GET_SIZE(mem_id)) {
            mem_err("mem_offset overflowed mem_offset 0x%x, size 0x%x\n", mem_offset, MEM_INFO_GET_SIZE(mem_id));
            return MEM_ERROR;
        }
    }
    mem_err("[memory]meminfo_of_parse end\n");
    return MEM_OK;
}
#endif

int mem_info_init(const char *memid_name, u32 mem_id)
{
    int ret = 0;
    unsigned int addr = 0;
    unsigned int total_size = 0;
    unsigned int dts_info_size = 0;
    void *virt_addr = NULL;
    mem_mgr_node_info_s *shm_dtsinfo = NULL;
    device_node_s *node = NULL;

    if (mem_id >= SHM_ATTR_MAX) {
        mem_err("mem_id %d is overflow.\n", mem_id);
        return MEM_ERROR;
    }

    node = bsp_dt_find_compatible_node(NULL, NULL, memid_name);
    if (node == NULL) {
        mem_err("mem_name parent_node get failed.\n");
        return MEM_ERROR;
    }
    ret += bsp_dt_property_read_u32_array(node, "addr", &addr, 1);
    ret += bsp_dt_property_read_u32_array(node, "size", &total_size, 1);
    ret += bsp_dt_property_read_u32_array(node, "dts_info_size", &dts_info_size, 1);
    if (ret) {
        mem_err("%s addr node get failed.\n", memid_name);
        return MEM_ERROR;
    }
    virt_addr = ioremap_wc(addr, total_size);
    if (virt_addr == NULL) {
        mem_err("%s ioremap fail\n", memid_name);
        return MEM_ERROR;
    }
    MEM_INFO_GET_VIRT_ADDR(mem_id) = (void *)(uintptr_t)((uintptr_t)virt_addr + dts_info_size);
    MEM_INFO_GET_ADDR(mem_id) = (phy_addr_t)(dts_info_size + (unsigned int)addr);
    MEM_INFO_GET_SIZE(mem_id) = (unsigned int)((unsigned int)total_size - dts_info_size);
    MEM_INFO_GET_DTSINFO_SIZE(mem_id) = dts_info_size;
    shm_dtsinfo = (mem_mgr_node_info_s *)(uintptr_t)virt_addr;
    MEM_INFO_GET_DTSINFO(mem_id) = shm_dtsinfo;
#ifdef BSP_CONFIG_PHONE_TYPE
    ret = meminfo_of_parse(memid_name, mem_id);
    if (ret) {
        mem_err("meminfo_of_parse failed.\n", memid_name);
        return MEM_ERROR;
    }
#endif
    return MEM_OK;
}

void *mdrv_mem_share_get(const char *name, phy_addr_t *addr, unsigned int *size, u32 flag)
{
    u32 meminfo_id;
    mem_mgr_node_info_s *dts_info = NULL;
    u32 i;
    u32 max_num;
    if (name == NULL) {
        mem_err("[memory]name error!\n");
        return NULL;
    }
    if (size == NULL || addr == NULL) {
        mem_err("[memory]size or addr error!\n");
        return NULL;
    }
    if (flag >= SHM_ATTR_MAX) {
        mem_err("[memory]flag error!\n");
        return NULL;
    }
    meminfo_id = flag;
    dts_info = MEM_INFO_GET_DTSINFO(meminfo_id);
    if (dts_info == NULL) {
        mem_err("mem_dts %d info is NULL\n", flag);
        return NULL;
    }
    max_num = MEM_INFO_GET_DTSINFO_SIZE(flag) / sizeof(mem_mgr_node_info_s);
    for (i = 0; i < max_num; i++) {
        if (strncmp((MEM_NODE_GET_OFFSET(dts_info, i))->name, name, MEM_MGR_NAME_SIZE) == 0) {
            *size = (MEM_NODE_GET_OFFSET(dts_info, i))->size;
            *addr = (phy_addr_t)(MEM_INFO_GET_ADDR(meminfo_id) + (MEM_NODE_GET_OFFSET(dts_info, i))->offset);
            return (void *)((uintptr_t)MEM_INFO_GET_VIRT_ADDR(meminfo_id) + (MEM_NODE_GET_OFFSET(dts_info, i))->offset);
        }
    }
    mem_err("[memory]mem %s not find in dts\n", name);
    return NULL;
}
EXPORT_SYMBOL_GPL(mdrv_mem_share_get);

int bsp_memory_init(void)
{
    if (memset_s((void *)g_bydts_mem_info, sizeof(g_bydts_mem_info), 0x0, (sizeof(mem_mgr_info_s) * SHM_ATTR_MAX))) {
        mem_err("[memory]memset_s error\n");
    }
    if (mem_info_init("hisilicon,shared_unsec_mem", SHM_UNSEC) != MEM_OK) {
        mem_err("[memory]shared_unsec_mem init failed\n");
        return MEM_ERROR;
    }
    if (mem_info_init("hisilicon,shared_nsro_mem", SHM_NSRO) != MEM_OK) {
        mem_err("[memory]shared_nsro_mem init failed\n");
        return MEM_ERROR;
    }
#ifdef BSP_CONFIG_PHONE_TYPE
    if (mem_info_init("hisilicon,shared_sec_mem", SHM_SEC) != MEM_OK) {
        mem_err("[memory]shared_sec_mem init failed\n");
        return MEM_ERROR;
    }
#endif
    mem_err("[memory]init ok\n");
    return MEM_OK;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
pure_initcall(bsp_memory_init); /*lint !e528*/
#else
module_init(bsp_memory_init); /*lint !e528*/
#endif
#endif
