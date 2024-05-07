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

#include "device_tree.h"
#include <securec.h>
#include <bsp_dt.h>
#include <bsp_sec.h>
#include <libfdt.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/of.h>

#define MODEMID_MASK_BITS (0xff)
#define modemid_k_bits(n) ((n >> 24) & MODEMID_MASK_BITS)
#define modemid_h_bits(n) ((n >> 16) & MODEMID_MASK_BITS)
#define modemid_m_bits(n) ((n >> 8) & MODEMID_MASK_BITS)
#define modemid_l_bits(n) (n & MODEMID_MASK_BITS)

#define DT_NODE_MAX_DEPTH 64
#define MALLOC_LEN_LIMIT (1024 * 1024 * 16)
#define HSDT_MAGIC 0x54445348 /* Master DTB magic "HSDT" */

#define MODEM_DRIVER_DT_FILE_NAME "/vendor/modem/modem_vendor/modem_driver_dt.img"
#define MODEM_DRIVER_DTCUST_FILE_NAME "/vendor/modem/modem_vendor/modem_driver_dtcust.img"

struct dtb_table {
    u32 magic;
    u32 version;
    u32 num_entries;
};

struct dtb_entry {
    u8 boardid[4];
    u8 reserved[4];
    u32 dtb_size;
    u32 vrl_size;
    u32 dtb_offset;
    u32 vrl_offset;
    u64 dtb_file;
    u64 vrl_file;
};

struct dt_ctrl g_dt_ctrl = { 0 };

/*
 * return >0 for got file size
 */
int bsp_dt_get_file_size(const char *filename)
{
    int ret;
    struct kstat kstattmp = { 0 };
    unsigned long old_fs;

    old_fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);
    ret = sys_access((s8 *)filename, O_RDONLY);
    if (ret) {
        dt_pr_err("access file %s failed, ret = %d\n", filename, ret);
        set_fs(old_fs);
        return -1;
    }

    ret = vfs_stat(filename, &kstattmp);
    set_fs(old_fs);
    if (ret) {
        dt_pr_err("get file %s size failed, ret = %d\n", filename, ret);
        return -1;
    }
    return kstattmp.size;
}

int bsp_dt_read_file(const char *file_name, unsigned int offset, unsigned int length, char *buffer)
{
    struct file *fp = NULL;
    int retval;
    loff_t loff = (loff_t)offset;

    fp = filp_open(file_name, O_RDONLY, 0600);
    if ((!fp) || (IS_ERR(fp))) {
        retval = (int)PTR_ERR(fp);
        dt_pr_err("filp_open(%s) failed, ret %d", file_name, retval);
        return retval;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
    retval = kernel_read(fp, buffer, (unsigned long)length, (loff_t *)&loff);
#else
    retval = kernel_read(fp, loff, buffer, (unsigned long)length);
#endif
    if (retval != (int)length) {
        dt_pr_err("kernel_read %s failed, retval %d, require len %u\n", file_name, retval, length);
        if (retval >= 0) {
            retval = -EIO;
        }
    }

    filp_close(fp, NULL);
    return retval;
}

static int bsp_dt_find_dtb_entry(u32 modemid, const struct dtb_entry *base_ptr, u32 num, struct dtb_entry *dtb_entry)
{
    int ret;
    u32 i;
    u8 mdm_id[3] = {0};

    mdm_id[0] = modemid_k_bits(modemid);
    mdm_id[1] = modemid_h_bits(modemid);
    mdm_id[2] = modemid_m_bits(modemid);

    for (i = 0; i < num; i++) {
        if ((base_ptr->boardid[0] == mdm_id[0]) && (base_ptr->boardid[1] == mdm_id[1]) &&
            (base_ptr->boardid[2] == mdm_id[2]) && (base_ptr->boardid[3] == 0)) {
            dt_pr_err("found entry %d for modem_id(0x%x, 0x%x, 0x%x, 0x%x)\n", i, base_ptr->boardid[0],
                base_ptr->boardid[1], base_ptr->boardid[2], base_ptr->boardid[3]);

            ret = memcpy_s((void *)dtb_entry, sizeof(*dtb_entry), (void *)base_ptr, sizeof(*base_ptr));
            if (ret) {
                dt_pr_err("%s memcpy_s error, ret = %d\n", __FUNCTION__, ret);
                return BSP_ERROR;
            }
            return BSP_OK;
        }
        base_ptr++;
    }

    if (i == num) {
        dt_pr_err("not found entry for modemid(0x%x, 0x%x, 0x%x, 0x%x)\n", base_ptr->boardid[0], base_ptr->boardid[1],
            base_ptr->boardid[2], base_ptr->boardid[3]);
        return BSP_ERROR;
    }

    return BSP_OK;
}

static void *bsp_dt_alloc_buffer(unsigned int bufsz)
{
    void *vaddr = NULL;

    if (bufsz > MALLOC_LEN_LIMIT) {
        dt_pr_err("malloc too large size %u\n", bufsz);
        return NULL;
    }

    vaddr = kzalloc((size_t)bufsz, GFP_KERNEL);
    if (vaddr == NULL) {
        dt_pr_err("malloc buffer failed, size %u\n", bufsz);
        return NULL;
    }

    return vaddr;
}

static void bsp_dt_free_buffer(void *buf)
{
    if (buf != NULL) {
        kfree(buf);
    }
}

static int bsp_dt_get_dtb_entry(const char *file_name, u32 mdm_id, struct dtb_entry *entry_out)
{
    int ret = BSP_ERROR;
    int rd_bytes;
    u32 offset;
    struct dtb_table dtb_table = { 0, 0, 0 };
    struct dtb_entry *entries = NULL;
    u32 len;

    /* get the dtb header and entries */
    offset = 0;
    rd_bytes = bsp_dt_read_file(file_name, offset, (u32)sizeof(struct dtb_table), (char *)(&dtb_table));
    if (rd_bytes != (int)sizeof(struct dtb_table)) {
        dt_pr_err("read dtb header failed, readed %d, size %u\n", rd_bytes, sizeof(struct dtb_table));
        return BSP_ERROR;
    }
    if (dtb_table.magic != HSDT_MAGIC) {
        dt_pr_err("bad adrv dtb magic 0x%08x\n", dtb_table.magic);
        return BSP_ERROR;
    }

    len = sizeof(struct dtb_entry) * dtb_table.num_entries;
    entries = (struct dtb_entry *)bsp_dt_alloc_buffer(len);
    if (entries == NULL) {
        dt_pr_err("malloc dtb entries buffer failed, size %u, entries num %u\n", len, dtb_table.num_entries);
        return BSP_ERROR;
    }

    rd_bytes = bsp_dt_read_file(file_name, offset + sizeof(struct dtb_table), len, (char *)entries);
    if (rd_bytes != (int)len) {
        dt_pr_err("read dtb entries failed, readed %d, size 0x%u\n", rd_bytes, len);
        goto out;
    }

    if (bsp_dt_find_dtb_entry(mdm_id, entries, dtb_table.num_entries, entry_out)) {
        dt_pr_err("fail to get dtb entry\n");
        goto out;
    }
    ret = BSP_OK;

out:
    bsp_dt_free_buffer(entries);
    return ret;
}

/*
 * return NULL if failed
 * caller release the allocated buffer
 */
static void *bsp_dt_load_dtb(const char *dtimg_name, u32 mdm_id, unsigned int extra_bufsz)
{
    int rd_bytes;
    u32 offset;
    struct dtb_entry entry;
    void *fdt = NULL;

    offset = 0;
    if (memset_s((void *)&entry, sizeof(struct dtb_entry), 0, sizeof(struct dtb_entry))) {
        return NULL;
    }

    if (bsp_dt_get_dtb_entry(dtimg_name, mdm_id, &entry)) {
        dt_pr_err("fail to get dtb entry\n");
        return NULL;
    }

    fdt = bsp_dt_alloc_buffer(entry.dtb_size + extra_bufsz);
    if (fdt == NULL) {
        dt_pr_err("malloc dtb buffer failed, dtb size is %u\n", entry.dtb_size);
        return NULL;
    }

    rd_bytes = bsp_dt_read_file(dtimg_name, offset + entry.dtb_offset, entry.dtb_size, (char *)fdt);
    if (rd_bytes != entry.dtb_size) {
        dt_pr_err("read dtb %s failed: readed %d, dtb size %u\n", dtimg_name, rd_bytes, entry.dtb_size);
        bsp_dt_free_buffer(fdt);
        fdt = NULL;
        return NULL;
    }

    return fdt;
}

static int bsp_dt_overlay_apply(void *fdt, void *fdto, uint32_t bufsz)
{
    int ret;
    uint32_t dt_len, ov_len, total_len;

    ret = fdt_check_header(fdt);
    if (ret != 0) {
        return ret;
    }
    ret = fdt_check_header(fdto);
    if (ret != 0) {
        return ret;
    }

    /* grow the blob to worst case */
    dt_len = fdt_totalsize(fdt);
    ov_len = fdt_totalsize(fdto);
    total_len = dt_len + ov_len;
    if (bufsz < total_len) {
        return -FDT_ERR_NOSPACE;
    }

    ret = fdt_open_into(fdt, fdt, (int)bufsz);
    if (ret != 0) {
        return ret;
    }
    ret = fdt_overlay_apply(fdt, fdto);
    if (ret != 0) {
        return ret;
    }
    ret = fdt_pack(fdt);
    if (ret != 0) {
        return ret;
    }

    return BSP_OK;
}

/*
 * get product_id
 */
static int bsp_dt_get_product_id(u32 *product_id)
{
    struct device_node *node = NULL;
    u32 value = 0;
    int ret;

    node = of_find_compatible_node(NULL, NULL, "hisilicon,vendor");
    if (node == NULL) {
        dt_pr_err("get hisi vendor node failed\n");
        return BSP_ERROR;
    }

    ret = of_property_read_u32(node, "hisi,product_id", &value);
    if (ret) {
        dt_pr_err("read hisi,product_id failed\n");
        return BSP_ERROR;
    }
    if (value == 0xffffffff) { /* 0xffffffff for single_ap */
        dt_pr_err("hisi,product_id is 0xffffffff\n");
        return BSP_ERROR;
    }
    *product_id = value;
    return BSP_OK;
}

const void *bsp_dt_load_merge_dtb(void)
{
    u32 mdm_id = 0;
    void *fdt = NULL;
    void *fdto = NULL;
    int ret;

    if (bsp_dt_get_product_id(&mdm_id) != BSP_OK) {
        dt_pr_err("get product_id failed\n");
        goto err_out;
    }
    dt_pr_err("get modem_id 0x%x to load dtb\n", mdm_id);
    fdto = bsp_dt_load_dtb(MODEM_DRIVER_DTCUST_FILE_NAME, mdm_id, 0);
    if (fdto == NULL) {
        g_dt_ctrl.err_nr = DT_ERR_DTBO_LOAD;
        goto err_out;
    }
    dt_pr_err("load dtb ok from file %s\n", MODEM_DRIVER_DTCUST_FILE_NAME);

    ret = fdt_check_header(fdto);
    if (ret != 0) {
        g_dt_ctrl.err_nr = DT_ERR_BAD_DTBO_HDR;
        dt_pr_err("invalid dtbo header\n");
        goto err_out;
    }

    dt_pr_err("use 0 as modem_id to load base dtb\n");
    mdm_id = 0;
    fdt = bsp_dt_load_dtb(MODEM_DRIVER_DT_FILE_NAME, mdm_id, fdt_totalsize(fdto));
    if (fdt == NULL) {
        g_dt_ctrl.err_nr = DT_ERR_DTB_LOAD;
        goto err_out;
    }
    dt_pr_err("load dtb ok from file %s\n", MODEM_DRIVER_DT_FILE_NAME);

    ret = fdt_check_header(fdt);
    if (ret != 0) {
        g_dt_ctrl.err_nr = DT_ERR_BAD_BASE_HDR;
        dt_pr_err("invalid dtb header\n");
        goto err_out;
    }

    ret = bsp_dt_overlay_apply(fdt, fdto, fdt_totalsize(fdt) + fdt_totalsize(fdto));
    if (ret != BSP_OK) {
        g_dt_ctrl.err_nr = DT_ERR_OVERLAY;
        dt_pr_err("overlay modem acore dtb failed(%d): %s\n", ret, fdt_strerror(ret));
        goto err_out;
    }
    dt_pr_err("overlay modem acore dtb ok\n");

    return (const void *)fdt;

err_out:
    bsp_dt_free_buffer(fdto);
    bsp_dt_free_buffer(fdt);
    return NULL;
}

const void *bsp_dt_load(void)
{
    const char *dtimg_name = MODEM_DRIVER_DT_FILE_NAME;
    const u32 mdm_id = 0;
    void *fdt = NULL;

    dt_pr_err("use 0 as modem_id to load base dtb\n");
    fdt = bsp_dt_load_dtb(dtimg_name, mdm_id, 0);
    if (fdt == NULL) {
        g_dt_ctrl.err_nr = DT_ERR_DTB_LOAD;
        return NULL;
    }
    dt_pr_err("load dtb ok from file %s\n", dtimg_name);

    return (const void *)fdt;
}

static int bsp_dt_sum_nodes_and_props(const void *fdt)
{
    int node_offset;
    int prop_offset;
    int depth = -1;

    for (node_offset = fdt_next_node(fdt, -1, &depth); node_offset >= 0 && depth >= 0;
        node_offset = fdt_next_node(fdt, node_offset, &depth)) {
        ++g_dt_ctrl.node_num;

        prop_offset = node_offset;
        for (prop_offset = fdt_first_property_offset(fdt, prop_offset); prop_offset >= 0;
            prop_offset = fdt_next_property_offset(fdt, prop_offset)) {
            ++g_dt_ctrl.prop_num;
        }
    }

    return BSP_OK;
}

static inline const void *dt_get_offset_ptr(const void *fdt, int offset)
{
    return (const char *)fdt + fdt_off_dt_struct(fdt) + offset;
}

static void dt_find_node_chief_props(const void *fdt, struct fdt_device_node *node)
{
    const struct fdt_property *fdt_prop = NULL;
    const char *propname = NULL;
    unsigned int index;

    node->name = ((const struct fdt_node_header *)dt_get_offset_ptr(fdt, node->offset))->name;

    for (index = 0; index < node->prop_count; ++index) {
        fdt_prop = g_dt_ctrl.allprops[node->prop_start + index];
        propname = (const char *)fdt + fdt_off_dt_strings(fdt) + fdt32_to_cpu(fdt_prop->nameoff);
        /* propname is const char pointer type, use strcmp for boot speed */
        if (fdt32_to_cpu(fdt_prop->len) > 0 && strncmp(propname, "compatible", 11) == 0) { /*lint !e421*/
            node->compatible = (const char *)fdt_prop->data;
        } else if (fdt32_to_cpu(fdt_prop->len) > 0 && strncmp(propname, "device_type", 12) == 0) { /*lint !e421*/
            node->type = (const char *)fdt_prop->data;
        } else if (fdt32_to_cpu(fdt_prop->len) == sizeof(uint32_t) &&
            strncmp(propname, "phandle", strlen(propname)) == 0) { /*lint !e421*/
            node->phandle = (uint32_t)fdt32_to_cpu(*(const fdt32_t *)fdt_prop->data);
        }
    }
}

static int dt_parse_node_prop(const void *fdt, struct fdt_device_node *node)
{
    int offset = node->offset;
    static unsigned int prop_alloc_index;

    for (offset = fdt_first_property_offset(fdt, offset); offset >= 0; offset = fdt_next_property_offset(fdt, offset)) {
        if (prop_alloc_index >= g_dt_ctrl.prop_num) {
            g_dt_ctrl.err_nr = DT_ERR_OV_PROP;
            dt_pr_err("prop table overflow!\n");
            return BSP_ERROR;
        }
        g_dt_ctrl.allprops[prop_alloc_index] = (const struct fdt_property *)dt_get_offset_ptr(fdt, offset);
        if (node->prop_count == 0) {
            node->prop_start = prop_alloc_index;
        }
        node->prop_count += 1;
        ++prop_alloc_index;
    }

    dt_find_node_chief_props(fdt, node);
    return BSP_OK;
}

static int dt_populate_node(const void *fdt, int offset, struct fdt_device_node *dad, struct fdt_device_node **pnp)
{
    struct fdt_device_node *node = NULL;
    static int node_alloc_index;

    if (node_alloc_index >= g_dt_ctrl.node_num) {
        g_dt_ctrl.err_nr = DT_ERR_OV_NODE;
        dt_pr_err("mem overflow!\n");
        return BSP_ERROR;
    }

    node = &g_dt_ctrl.allnodes[node_alloc_index++];
    node->offset = offset;

    if (dad != NULL) {
        node->parent = dad;
        if (dad->child == NULL) {
            dad->child = node;
            dad->child_tail = node;
        } else {
            dad->child_tail->sibling = node;
            dad->child_tail = node;
        }
    }
    *pnp = node;

    if (dt_parse_node_prop(fdt, node) != 0) {
        return BSP_ERROR;
    }

    return BSP_OK;
}

static int bsp_dt_unflatten_nodes(const void *fdt)
{
    int offset;
    int depth = -1;
    struct fdt_device_node *nps[DT_NODE_MAX_DEPTH + 1] = {0};
    int err;

    for (offset = fdt_next_node(fdt, -1, &depth); offset >= 0 && depth >= 0;
        offset = fdt_next_node(fdt, offset, &depth)) {
        if (depth >= DT_NODE_MAX_DEPTH) {
            g_dt_ctrl.err_nr = DT_ERR_OV_DEPTH;
            dt_pr_err("depth overflow!\n");
            return BSP_ERROR;
        }
        err = dt_populate_node(fdt, offset, nps[depth], &nps[depth + 1]); /*lint !e679*/
        if (err != BSP_OK) {
            return BSP_ERROR;
        }
    }

    return BSP_OK;
}

static int bsp_dt_alloc_unflatten_buf(void)
{
    unsigned int len_nodes;
    unsigned int len_props;

    len_nodes = g_dt_ctrl.node_num * sizeof(struct fdt_device_node);
    g_dt_ctrl.allnodes = (struct fdt_device_node *)bsp_dt_alloc_buffer(len_nodes);
    if (g_dt_ctrl.allnodes == NULL) {
        dt_pr_err("malloc device_node buffer failed, size is %u\n", len_nodes);
        goto err_out;
    }

    len_props = g_dt_ctrl.prop_num * sizeof(void *);
    g_dt_ctrl.allprops = (const struct fdt_property **)bsp_dt_alloc_buffer(len_props);
    if (g_dt_ctrl.allprops == NULL) {
        dt_pr_err("malloc fdt_prop ptr buffer failed, size is %u\n", len_props);
        goto err_out;
    }

    return BSP_OK;

err_out:
    bsp_dt_free_buffer(g_dt_ctrl.allnodes);
    g_dt_ctrl.allnodes = NULL;
    return BSP_ERROR;
}

static void bsp_dt_free_unflatten_buf(void)
{
    bsp_dt_free_buffer(g_dt_ctrl.allnodes);
    g_dt_ctrl.allnodes = NULL;
    bsp_dt_free_buffer(g_dt_ctrl.allprops);
    g_dt_ctrl.allprops = NULL;
}

int bsp_dt_parse(const void *fdt)
{
    int ret;

    if (fdt_check_header(fdt)) {
        g_dt_ctrl.err_nr = DT_ERR_BAD_DTB_HDR;
        dt_pr_err("parse modem driver dtb get invalid dt header\n");
        return BSP_ERROR;
    }

    ret = bsp_dt_sum_nodes_and_props(fdt);
    if (ret != BSP_OK) {
        dt_pr_err("parse modem driver dtb first round failed\n");
        return BSP_ERROR;
    }

    ret = bsp_dt_alloc_unflatten_buf();
    if (ret != BSP_OK) {
        dt_pr_err("malloc nodes_and_props failed\n");
        goto err_out;
    }

    if (bsp_dt_unflatten_nodes(fdt)) {
        dt_pr_err("unflatten modem driver dtb failed\n");
        ret = BSP_ERROR;
        goto err_out;
    }

    dt_pr_err("unflatten modem driver dtb success\n");
    return BSP_OK;

err_out:
    bsp_dt_free_unflatten_buf();
    return ret;
}

int bsp_dt_init(void)
{
    dt_pr_err("%s enter\n", __func__);
    g_dt_ctrl.state = 0;
    g_dt_ctrl.fdt_addr = bsp_dt_load();
    if (g_dt_ctrl.fdt_addr == NULL) {
        dt_pr_err("%s load modem driver dtb failed\n", __func__);
        return BSP_ERROR;
    }
    g_dt_ctrl.state = 1;
    if (bsp_dt_parse(g_dt_ctrl.fdt_addr) != BSP_OK) {
        dt_pr_err("%s parse modem driver dtb failed\n", __func__);
        goto err_out;
    }
    g_dt_ctrl.state = 2;
    dt_pr_err("%s exit ok\n", __func__);
    return BSP_OK;

err_out:
    bsp_dt_free_buffer((void *)g_dt_ctrl.fdt_addr);
    g_dt_ctrl.fdt_addr = NULL;
    return BSP_ERROR;
}

static void __exit bsp_dt_exit(void)
{
    bsp_dt_free_unflatten_buf();

    bsp_dt_free_buffer(g_dt_ctrl.fdt_addr);
    g_dt_ctrl.fdt_addr = NULL;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(bsp_dt_init);
module_exit(bsp_dt_exit);
#endif
