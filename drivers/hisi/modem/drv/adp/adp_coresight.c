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

#include <product_config.h>

#include <osl_types.h>
#include <osl_cache.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <nv_stru_drv.h>
#include <bsp_dt.h>
#include <bsp_om_enum.h>
#include <bsp_coresight.h>
#include <bsp_dump.h>
#include <bsp_nvim.h>
#include <bsp_print.h>
#include <securec.h>

#undef THIS_MODU
#define THIS_MODU mod_coresight

#define cs_error(fmt, ...) (void)bsp_err("<%s> line = %d, " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define cs_print(fmt, ...) (void)bsp_err("<%s>" fmt, __FUNCTION__, ##__VA_ARGS__)

#ifndef CONFIG_OCD

#define CPU_NUMS 8 /* lr :0~3,nr :4~7 */

#define CS_ETM_LOCK(addr) do { \
    writel(0x0, (addr) + MDM_CORESIGHT_LAR); \
} while (0)
#define CS_ETM_UNLOCK(addr) do { \
    writel(MDM_CORESIGHT_UNLOCK, (addr) + MDM_CORESIGHT_LAR); \
} while (0)

struct mdmcp_coresight_device_info {
    void *tmc_base;
    unsigned long tmc_phy;
    struct coresight_etb_data_head_info *etb_buf;
    const char *tmc_name;
    void *debug_base;
    void *debug_buffer;
    u32 debug_length;
    u32 index;
};
struct mdmcp_coresight_device_info g_mdmcp_coresight[CPU_NUMS];

#define TIMEOUT_CNT 10000
#define TIMEOUT_PERIOD 10
#define BUF_HEADER_U32LEN 2

#define STSR_TMC_READY BIT(2)
#define FFCR_STOP_ON_FL BIT(12)
#define FFCR_STOP_ON_FL BIT(12)
#define FFCR_FLUSH_MAN BIT(6)
#define BYTES_PER_WORD 4

#define LRCPU0 0
#define LRCPU1 1
#define LRCPU2 2
#define LRCPU3 3
#define NRCPU0 4
#define NRCPU1 5
#define NRCPU2 6
#define NRCPU3 7
#define LR_OR_NR_CPU0_ID  0
#define LR_OR_NR_CPU1_ID  1
#define LR_OR_NR_CPU2_ID  2
#define LR_OR_NR_CPU3_ID  3


static void *get_mdmcp_etb_buf(u32 cpu)
{
    void *addr = NULL;
    switch (cpu) {
        case LRCPU0:
            addr = (void *)bsp_dump_get_field_addr(CP_TRACE_ID(LR_OR_NR_CPU0_ID));
            break;
        case LRCPU1:
            addr = (void *)bsp_dump_get_field_addr(CP_TRACE_ID(LR_OR_NR_CPU1_ID));
            break;
        case LRCPU2:
            addr = (void *)bsp_dump_get_field_addr(CP_TRACE_ID(LR_OR_NR_CPU2_ID));
            break;
        case LRCPU3:
            addr = (void *)bsp_dump_get_field_addr(CP_TRACE_ID(LR_OR_NR_CPU3_ID));
            break;
        case NRCPU0:
            addr = (void *)bsp_dump_get_field_addr(NR_TRACE_ID(LR_OR_NR_CPU0_ID));
            break;
        case NRCPU1:
            addr = (void *)bsp_dump_get_field_addr(NR_TRACE_ID(LR_OR_NR_CPU1_ID));
            break;
        case NRCPU2:
            addr = (void *)bsp_dump_get_field_addr(NR_TRACE_ID(LR_OR_NR_CPU2_ID));
            break;
        case NRCPU3:
            addr = (void *)bsp_dump_get_field_addr(NR_TRACE_ID(LR_OR_NR_CPU3_ID));
            break;
        default:
            return NULL;
    }
    return addr;
}
#define TRACE_FIELD_LEN 0x1100
int mdmcp_coresight_wait_value(const void *base, u32 offset, u32 mask, u32 value)
{
    int i;
    u32 val;

    for (i = TIMEOUT_CNT; i > 0; i--) {
        val = readl(base + offset);
        if ((val & mask) == (value & mask)) {
            return BSP_OK;
        }
        if (i - 1) {
            udelay(TIMEOUT_PERIOD);
        }
    }

    return BSP_ERROR;
}

static void mdmcp_coresight_etb_stop(void *tmc_base)
{
    u32 reg_value;
    /* unlock etb, 配置ETF_LOCK_ACCESS */
    writel(0xC5ACCE55, tmc_base + 0xFB0);

    /* stop etb, 配置ETF_FORMAT_FLUSH_CTRL */
    reg_value = (u32)readl(tmc_base + 0x304);
    /* FFCR StopOnFl */
    reg_value |= FFCR_STOP_ON_FL;
    /* FFCR FlushMem */
    reg_value |= FFCR_FLUSH_MAN;
    writel(reg_value, tmc_base + 0x304);

    /* 等待TMCReady */
    if (mdmcp_coresight_wait_value(tmc_base, 0xc, STSR_TMC_READY, STSR_TMC_READY)) {
        cs_error("save etb time out\n");
    }
    return;
}

static void mdmcp_coresight_etb_store(u32 cpu)
{
    void *tmc_base = NULL;
    void *etb_buf = NULL;
    u32 reg_value;
    u32 i;
    u32 *data = NULL;
    u32 len;

    if (!g_mdmcp_coresight[cpu].tmc_base) {
        return;
    }
    if (g_mdmcp_coresight[cpu].etb_buf) {
        etb_buf = g_mdmcp_coresight[cpu].etb_buf;
    } else {
        etb_buf = get_mdmcp_etb_buf(cpu);
        if (etb_buf == NULL) {
            cs_error("get modem cp cpu%d dump buf failed!\n", cpu);
            return;
        }
        g_mdmcp_coresight[cpu].etb_buf = etb_buf;
    }
    tmc_base = g_mdmcp_coresight[cpu].tmc_base;

    if (*(u32 *)etb_buf == CORESIGHT_MAGIC_NUM) {
        cs_error("%s etb data has store finished,no need to store again!\n", g_mdmcp_coresight[cpu].tmc_name);
        return;
    }

    /* 该cpu是否已经热插拔 */
    if (*(u32 *)etb_buf == CORESIGHT_HOTPLUG_MAGICNUM) {
        cs_error("%s has powerdown or hotplug,no need to store data!\n", g_mdmcp_coresight[cpu].tmc_name);
        return;
    }

    mdmcp_coresight_etb_stop(tmc_base);
    len = (u32)readl(tmc_base + 0x4) * BYTES_PER_WORD;
    /* 导出etb数据 */
    if (memset_s((void *)etb_buf, TRACE_FIELD_LEN, 0x0, (unsigned long)len)) {
        cs_error("memset failed\n");
    }
    /* lint --e{124} */
    data = (u32 *)(etb_buf + sizeof(struct coresight_etb_data_head_info)); /*lint !e124*/
    for (i = 0; i < len / BYTES_PER_WORD; i++) {
        /* read etb, 读取ETF_RAM_RD_DATA */
        reg_value = (u32)readl(tmc_base + 0x10);
        *data = reg_value;
        data++;
        if (reg_value == 0xffffffff) {
            break;
        }
    }

    /* 0-3字节存放标识码 */
    *((u32 *)etb_buf) = (u32)CORESIGHT_MAGIC_NUM;
    /* 4-7个字节存放ETB数据长度 */
    *((u32 *)etb_buf + 1) = i * BYTES_PER_WORD;

    /* lock etb, 配置ETF_LOCK_ACCESS */
    writel(0x1, tmc_base + 0xFB0);

    cs_error("%s store success!\n", g_mdmcp_coresight[cpu].tmc_name);
}

static int mdmcp_coresight_tmc_probe(device_node_s *dev_node)
{
    u32 cpu_index = 0;
    int ret;
    unsigned int phy_addr = 0;
    const __be32 *regaddr_p = NULL;
    int na;

    ret = bsp_dt_property_read_u32(dev_node, "cpu_index", &cpu_index);
    if (ret) {
        cs_error("read cpu_index failed,use default value! ret = %d\n", ret);
        cpu_index = 0;
    }

    regaddr_p = bsp_dt_get_property(dev_node, "reg", NULL);
    if (regaddr_p == NULL) {
        cs_error("get reg prop failed\n");
        return -1;
    }
    if (!g_mdmcp_coresight[cpu_index].tmc_base) {
        g_mdmcp_coresight[cpu_index].tmc_base = (void *)bsp_dt_iomap(dev_node, 0);
        na = bsp_dt_n_addr_cells(dev_node);
        phy_addr = (u32)bsp_dt_read_number(regaddr_p, na);
        g_mdmcp_coresight[cpu_index].tmc_phy = phy_addr;
    } else {
        cs_error("cpu %d have init before,cpu index may be wrong!\n", cpu_index);
    }

    g_mdmcp_coresight[cpu_index].etb_buf = get_mdmcp_etb_buf(cpu_index);

    ret = bsp_dt_property_read_string(dev_node, "coresight-name", &(g_mdmcp_coresight[cpu_index].tmc_name));
    if (ret) {
        cs_error("read name failed! ret = %d\n", ret);
    }

    return 0;
}

static void mdmcp_cpudebug_store(u32 cpu)
{
    int size;
    int i;
    u32 value;
    u32 *debug_base = (u32 *)g_mdmcp_coresight[cpu].debug_base;
    u32 *debug_buffer = (u32 *)g_mdmcp_coresight[cpu].debug_buffer;
    u32 *data = NULL;

    if (debug_buffer == NULL)
        debug_buffer = (u32 *)bsp_dump_get_field_addr(DUMP_LRCCPU_DEBUGREG_CPU + cpu);

    if (debug_buffer == NULL) {
        return;
    }

    if (((*debug_buffer) == CORESIGHT_HOTPLUG_MAGICNUM) || ((*debug_buffer) == CORESIGHT_MAGIC_NUM)) {
        cs_error("modem cp cpu%d has powerdown or hotplug ,no need to store cpudebug reg!\n", (int)cpu);
        return;
    }

    if (debug_base != NULL && debug_buffer != NULL) {
        size = g_mdmcp_coresight[cpu].debug_length / BYTES_PER_WORD;
        data = debug_buffer;
        debug_buffer += BUF_HEADER_U32LEN;

        for (i = 0; i < size; i++) {
            value = (u32)readl(debug_base);
            *debug_buffer = value;
            debug_base++;
            debug_buffer++;
        }
        *data = (u32)CORESIGHT_MAGIC_NUM;
        cache_sync();
    }
    return;
}

static int mdmcp_cpudebug_probe(device_node_s *dev_node)
{
    u32 cpu_index = 0;
    int ret;
    u32 length;
    u32 phy_addr;
    const __be32 *regaddr_p = NULL;
    int na;
    int ns;

    ret = bsp_dt_property_read_u32(dev_node, "cpu_index", &cpu_index);
    if (ret) {
        cs_error("mdmcp_cpudebug_probe,read cpu_index failed ret = %d\n", ret);
        return -1;
    }
    if (cpu_index >= CPU_NUMS) {
        cs_error("mdmcp_cpudebug_probe, cpu_index value is wrong, cpu_index = 0x%x\n", cpu_index);
        return -1;
    }

    regaddr_p = bsp_dt_get_property(dev_node, "reg", NULL);
    if (regaddr_p == NULL) {
        cs_error("get reg prop failed\n");
        return -1;
    }
    na = bsp_dt_n_addr_cells(dev_node);
    ns = bsp_dt_n_size_cells(dev_node);
    phy_addr = (u32)bsp_dt_read_number(regaddr_p, na);
    length = (u32)bsp_dt_read_number(regaddr_p + na, ns);
    if (length && phy_addr) {
        g_mdmcp_coresight[cpu_index].index = cpu_index;
        g_mdmcp_coresight[cpu_index].debug_base = (void *)ioremap(phy_addr, length);
        g_mdmcp_coresight[cpu_index].debug_buffer =
            (void *)bsp_dump_get_field_addr(DUMP_LRCCPU_DEBUGREG_CPU + cpu_index);
        g_mdmcp_coresight[cpu_index].debug_length = length;
    } else {
        cs_error("mdmcp_cpudebug_probe,lenght or phy addr wrong %d,0x%x\n", length, phy_addr);
        return -1;
    }
    return 0;
}

static struct of_device_id g_coresight_match[] = {
    { .name = "coresight-tmc",
      .compatible = "arm,coresight-tmc,cp" }, /*lint !e785*/
    {
        .name = "cpu-debug",
        .compatible = "arm,r8,cpu_debug" }, /*lint !e785*/
    {}                                      /*lint !e785*/
};

int mdmcp_coresight_init(void)
{
    device_node_s *node = NULL;
    device_node_s *child = NULL;

    /* parse tmc node */
    node = bsp_dt_find_compatible_node(NULL, NULL, g_coresight_match[0].compatible);
    if (node == NULL) {
        cs_error("can not find %s node!\n", g_coresight_match[0].compatible);
        return -1;
    }
    bsp_dt_for_each_child_of_node(node, child)
    {
        mdmcp_coresight_tmc_probe(child);
    }
    node = bsp_dt_find_compatible_node(NULL, NULL, g_coresight_match[1].compatible);
    if (node != NULL) {
        bsp_dt_for_each_child_of_node(node, child)
        {
            mdmcp_cpudebug_probe(child);
        }
    } else {
        cs_error("can not find %s node!\n", g_coresight_match[1].compatible);
    }

    cs_print("ok\n");

    return 0;
}

int mdmcp_coresight_stop(void)
{
    int cpu;

    for (cpu = 0; cpu < CPU_NUMS; cpu++) {
        mdmcp_coresight_etb_store((u32)cpu);
        mdmcp_cpudebug_store((u32)cpu);
    }

    return 0;
}

#else
struct reg_bank_info {
    u32 *reg_mask;
    u32 reg_offset;
    u32 reg_nums;
    u32 mask_length;
};

struct reg_group_info {
    struct reg_bank_info reg_bank[MAX_REG_BANKS];
    u32 banks;
};

struct tsp_regs_drvdata {
    u32 *tsp_reg_baseaddr;
    u32 phy_threadid[MAX_THREAD_NUM]; /* logic thread id to physic thread id */
    u32 is_powerdown[MAX_THREAD_NUM];
    u32 *thread_dump_base_addr[MAX_THREAD_NUM];
    u32 *dss_dump_base_addr[MAX_DSS_NUM];
    u32 *mpss_dump_base_addr;
    u32 thread_num;
    u32 dss_num;
    u32 thread_reg_num;
    u32 dss_reg_num;
    u32 mpss_reg_num;
    u32 init;
};

static struct of_device_id g_tsp_regs_match[REG_GROUP_NUMS] = {
    { .compatible = "hisilicon,tsp,thread_regs" },
    { .compatible = "hisilicon,tsp,dss_regs" },
    { .compatible = "hisilicon,tsp,mp_regs" }
};

struct reg_group_info g_tsp_reg_group[REG_GROUP_NUMS];  // 0:thread 1:dss 2:mpss

struct tsp_regs_drvdata g_tsp_regs_drvdata;

#define DSS_BASEADDR(dss) (void *)((uintptr_t)g_tsp_regs_drvdata.tsp_reg_baseaddr + DSS_REGS_OFFSET(dss))

#define THREAD_BASEADDR(dss, thread) (void *)(DSS_BASEADDR(dss) + THREAD_REGS_OFFSET(thread))

#define OCD_BASEADDR(dss, thread) (void *)(THREAD_BASEADDR(dss, thread) + OCD_REGS_OFFSET)

void mdmcp_ltarce_disable(uint32_t logic_threadid)
{
    u32 phy_threadid = g_tsp_regs_drvdata.phy_threadid[logic_threadid];
    u32 dss = phy_threadid / ARCHDEF_THREAD_PER_DSS;
    u32 thread = phy_threadid % ARCHDEF_THREAD_PER_DSS;
    writel(OCD_LTRACE_CTL_MAN_DIS, (void *)((uintptr_t)OCD_BASEADDR(dss, thread) + OCD_LTRACE_CTL));
    mb();
}

static void *mdmcp_get_ltrace_buf(u32 logic_threadid)
{
    void *addr = NULL;
    addr = (void *)bsp_dump_get_field_addr(DUMP_TSP_LTRACE0 + logic_threadid);
    return addr;
}

static void mdmcp_ltrace_store(u32 logic_threadid)
{
    u32 *ltrace_buf = NULL;
    u32 wptr, warp, offset;
    u32 phy_threadid = g_tsp_regs_drvdata.phy_threadid[logic_threadid];
    u32 dss = phy_threadid / ARCHDEF_THREAD_PER_DSS;
    u32 thread = phy_threadid % ARCHDEF_THREAD_PER_DSS;
    int ret;

    if (g_tsp_regs_drvdata.tsp_reg_baseaddr == NULL) {
        return;
    }
    ltrace_buf = (u32 *)mdmcp_get_ltrace_buf(logic_threadid);
    if (ltrace_buf == NULL) {
        cs_error("get tsp cpu%d ltarce buf failed!\n", logic_threadid);
        return;
    }

    if (*ltrace_buf == OCD_HOTPLUG_MAGICNUM) {
        g_tsp_regs_drvdata.is_powerdown[logic_threadid] = 1;
        return;
    }
    if (*ltrace_buf == OCD_MAGIC_NUM) {
        return;
    }

    mdmcp_ltarce_disable(logic_threadid);

    /* 导出etb数据 */
    ret = memset_s((void *)ltrace_buf, LTRACE_DUMP_FIELD_LEN, 0x0, (unsigned long)LTRACE_DUMP_FIELD_LEN);
    if (ret != EOK) {
        cs_error("memset failed\n");
    }

    *ltrace_buf = OCD_MAGIC_NUM;
    ltrace_buf++;
    warp = readl(OCD_BASEADDR(dss, thread) + OCD_LTRACE_CTL) & OCD_LTRACE_CTL_WRAP;
    wptr = readl(OCD_BASEADDR(dss, thread) + OCD_LTRACE_WRPTR);
    if (warp) {
        *ltrace_buf = LTRACE_MAX_BUF;
        ltrace_buf++;
        for (offset = wptr; offset < LTRACE_MAX_BUF; offset += DATAWIDTH_U32) {
            *ltrace_buf = readl(OCD_BASEADDR(dss, thread) + OCD_LTRACE_RDATA(0) + offset);
            ltrace_buf++;
        }
        for (offset = 0; offset < wptr; offset += DATAWIDTH_U32) {
            *ltrace_buf = readl(OCD_BASEADDR(dss, thread) + OCD_LTRACE_RDATA(0) + offset);
            ltrace_buf++;
        }
    } else {
        *ltrace_buf = wptr;
        ltrace_buf++;
        for (offset = 0; offset < wptr; offset += DATAWIDTH_U32) {
            *ltrace_buf = readl(OCD_BASEADDR(dss, thread) + OCD_LTRACE_RDATA(0) + offset);
            ltrace_buf++;
        }
    }
}

static int mdmcp_get_reg_buffer(void)
{
    u32 size;
    u32 i;
    u8 *addr;

    /* register dump area for thread regs */
    size = g_tsp_regs_drvdata.thread_reg_num * sizeof(u32) + 0x4;
    addr = bsp_dump_get_field_addr(DUMP_TSP_THREAD_REGS);
    if (addr == NULL) {
        cs_error("get thread dump area fail\n");
        return BSP_ERROR;
    }
    for (i = 0; i < g_tsp_regs_drvdata.thread_num; i++) {
        g_tsp_regs_drvdata.thread_dump_base_addr[i] = (u32 *)((uintptr_t)addr + (uintptr_t)(size * i)); /*lint !e647*/
    }

    /* register dump area for dss regs */
    size = g_tsp_regs_drvdata.dss_reg_num * sizeof(u32) + 0x4;
    addr = bsp_dump_get_field_addr(DUMP_TSP_DSS_REGS);
    if (addr == NULL) {
        cs_error("get dss dump area fail\n");
        return BSP_ERROR;
    }
    for (i = 0; i < g_tsp_regs_drvdata.dss_num; i++) {
        g_tsp_regs_drvdata.dss_dump_base_addr[i] = (u32 *)((uintptr_t)addr + (uintptr_t)(size * i)); /*lint !e647*/
    }

    /* register dump area for mpss regs */
    size = g_tsp_regs_drvdata.mpss_reg_num * sizeof(u32) + 0x4;
    addr = bsp_dump_get_field_addr(DUMP_TSP_MPSS_REGS);
    if (addr == NULL) {
        cs_error("get mpss dump area fail\n");
        return BSP_ERROR;
    }
    g_tsp_regs_drvdata.mpss_dump_base_addr = (u32 *)addr;
    return BSP_OK;
}

static inline u32 mdmcp_get_mask_bit(const u32 *reg_mask, u32 pos)
{
    u32 bank = pos / BITS_U32;
    u32 offset = pos & (BITS_U32 - 1);
    u32 ret = reg_mask[bank] & (1 << offset);
    return !!ret;
}

void mdmcp_save_tsp_regs_core(u32 *baseaddr, u32 *dump_addr, struct reg_group_info *regs)
{
    u32 i, n_bit;
    int pos = 1;
    u32 bits;
    u32 *reg_addr = NULL;
    struct reg_bank_info *reg_bank = NULL;
    for (i = 0; i < regs->banks; i++) {
        reg_bank = &regs->reg_bank[i];
        bits = reg_bank->mask_length * BITS_U32;
        reg_addr = (u32 *)((uintptr_t)baseaddr + reg_bank->reg_offset);
        for (n_bit = 0; n_bit < bits; n_bit++) {
            if (mdmcp_get_mask_bit(reg_bank->reg_mask, n_bit)) {
                dump_addr[pos] = (u32)readl((const void *)(uintptr_t)reg_addr);
                pos++;
            }
            reg_addr++;
        }
    }
}

void mdmcp_save_current_thread_regs(u32 logic_threadid)
{
    u32 phy_threadid = g_tsp_regs_drvdata.phy_threadid[logic_threadid];
    u32 dss = phy_threadid / ARCHDEF_THREAD_PER_DSS;
    u32 thread = phy_threadid % ARCHDEF_THREAD_PER_DSS;
    u32 *dump_addr = g_tsp_regs_drvdata.thread_dump_base_addr[logic_threadid];
    if (dump_addr == NULL || dump_addr[0] == DONE_MAGICNUM) {
        return;
    }
    dump_addr[0] = DONE_MAGICNUM;

    mdmcp_save_tsp_regs_core(THREAD_BASEADDR(dss, thread), dump_addr, &g_tsp_reg_group[REG_GROUP_THREAD]);
}

void mdmcp_save_current_dss_regs(u32 logic_threadid)
{
    u32 phy_threadid = g_tsp_regs_drvdata.phy_threadid[logic_threadid];
    u32 phy_dss = phy_threadid / ARCHDEF_THREAD_PER_DSS;
    u32 *dump_addr = g_tsp_regs_drvdata.dss_dump_base_addr[phy_dss];

    if (dump_addr == NULL || dump_addr[0] == DONE_MAGICNUM) {
        return;
    }

    dump_addr[0] = DONE_MAGICNUM;

    mdmcp_save_tsp_regs_core(DSS_BASEADDR(phy_dss), dump_addr, &g_tsp_reg_group[REG_GROUP_DSS]);
}

void mdmcp_save_current_mpss_regs(void)
{
    u32 *dump_addr = g_tsp_regs_drvdata.mpss_dump_base_addr;

    if (dump_addr == NULL || dump_addr[0] == DONE_MAGICNUM) {
        return;
    }

    dump_addr[0] = DONE_MAGICNUM;

    mdmcp_save_tsp_regs_core(g_tsp_regs_drvdata.tsp_reg_baseaddr, dump_addr, &g_tsp_reg_group[REG_GROUP_MPSS]);
}

void mdmcp_tsp_regs_store(u32 logic_threadid)
{
    if (mdmcp_get_reg_buffer() != BSP_OK) {
        return;
    }
    if (g_tsp_regs_drvdata.is_powerdown[logic_threadid]) {
        return;
    }
    mdmcp_save_current_thread_regs(logic_threadid);
    mdmcp_save_current_dss_regs(logic_threadid);
    mdmcp_save_current_mpss_regs();
    return;
}

static void mdmcp_free_reg_group_info(void)
{
    u32 i, j;
    for (i = 0; i < REG_GROUP_NUMS; i++) {
        for (j = 0; j < MAX_REG_BANKS; j++) {
            if (g_tsp_reg_group[i].reg_bank[j].reg_mask != NULL) {
                kfree(g_tsp_reg_group[i].reg_bank[j].reg_mask);
                g_tsp_reg_group[i].reg_bank[j].reg_mask = NULL;
            }
        }
    }
}

static int mdmcp_reginfo_probe(device_node_s *dev_node, int group)
{
    device_node_s *child_node = NULL;
    struct reg_bank_info *bank = NULL;
    u32 reg_banks = 0;
    u32 i = 0;
    int ret;

    if (bsp_dt_property_read_u32_array(dev_node, "reg_banks", &reg_banks, 1) != BSP_OK || reg_banks > MAX_REG_BANKS) {
        mdmcp_free_reg_group_info();
        return BSP_ERROR;
    }
    bsp_dt_for_each_child_of_node(dev_node, child_node)
    {
        if (i >= reg_banks) {
            break;
        }
        bank = &g_tsp_reg_group[group].reg_bank[i];
        ret = bsp_dt_property_read_u32_array(child_node, "reg_offset", &bank->reg_offset, 1);
        if (ret != BSP_OK) {
            mdmcp_free_reg_group_info();
            return BSP_ERROR;
        }
        ret = bsp_dt_property_read_u32_array(child_node, "array_length", &bank->mask_length, 1);
        if (ret != BSP_OK || bank->mask_length > MAX_REG_MASK_LENGTH) {
            mdmcp_free_reg_group_info();
            return BSP_ERROR;
        }
        ret = bsp_dt_property_read_u32_array(child_node, "reg_nums", &bank->reg_nums, 1);
        if (ret != BSP_OK || bank->reg_nums > MAX_REG_NUM || bank->reg_nums > bank->mask_length * BITS_U32) {
            mdmcp_free_reg_group_info();
            return BSP_ERROR;
        }

        bank->reg_mask = (u32 *)kmalloc(sizeof(u32) * bank->mask_length, GFP_KERNEL);
        if (bank->reg_mask == NULL) {
            mdmcp_free_reg_group_info();
            return BSP_ERROR;
        }
        ret = bsp_dt_property_read_u32_array(child_node, "reg_mask", bank->reg_mask, bank->mask_length);
        if (ret != BSP_OK) {
            mdmcp_free_reg_group_info();
            return BSP_ERROR;
        }

        i++;
    }
    g_tsp_reg_group[group].banks = i;
    return BSP_OK;
}

static int mdmcp_baseinfo_probe(device_node_s *dev_node)
{
    int ret;
    int i, j;
    u32 reg_nums[REG_GROUP_NUMS];

    ret = bsp_dt_property_read_u32_array(dev_node, "dss_num", &g_tsp_regs_drvdata.dss_num, 1);
    if (ret != BSP_OK || g_tsp_regs_drvdata.dss_num > MAX_DSS_NUM) {
        cs_error("dss_num not found/invalid\n");
        return BSP_ERROR;
    }
    ret = bsp_dt_property_read_u32_array(dev_node, "thread_num", &g_tsp_regs_drvdata.thread_num, 1);
    if (ret != BSP_OK || g_tsp_regs_drvdata.thread_num > MAX_THREAD_NUM) {
        cs_error("thread_num not found/invalid\n");
        return BSP_ERROR;
    }
    ret = bsp_dt_property_read_u32_array(dev_node, "phy_threadid", g_tsp_regs_drvdata.phy_threadid,
                                         g_tsp_regs_drvdata.thread_num);
    if (ret != BSP_OK) {
        cs_error("phy_threadid not found/invalid\n");
        return BSP_ERROR;
    }

    g_tsp_regs_drvdata.tsp_reg_baseaddr = bsp_dt_iomap(dev_node, 0);
    if (!g_tsp_regs_drvdata.tsp_reg_baseaddr) {
        cs_error("tsp_reg_baseaddr not found/invalid\n");
        return BSP_ERROR;
    }

    for (i = 0; i < REG_GROUP_NUMS; i++) {
        reg_nums[i] = 0;
        for (j = 0; j < g_tsp_reg_group[i].banks; j++) {
            reg_nums[i] += g_tsp_reg_group[i].reg_bank[j].reg_nums;
        }
    }
    g_tsp_regs_drvdata.thread_reg_num = reg_nums[REG_GROUP_THREAD];
    g_tsp_regs_drvdata.dss_reg_num = reg_nums[REG_GROUP_DSS];
    g_tsp_regs_drvdata.mpss_reg_num = reg_nums[REG_GROUP_MPSS];

    return BSP_OK;
}

int mdmcp_coresight_init(void)
{
    device_node_s *dev_node = NULL;
    device_node_s *child_node = NULL;
    int i;
    int ret;

    dev_node = bsp_dt_find_compatible_node(NULL, NULL, "hisilicon,dump_tsp_regs");
    if (dev_node == NULL) {
        cs_error("find node fail\n");
        return BSP_OK;
    }

    for (i = 0; i < REG_GROUP_NUMS; i++) {
        child_node = bsp_dt_find_compatible_node(dev_node, NULL, g_tsp_regs_match[i].compatible);
        if (child_node == NULL) {
            cs_error("find node %s fail\n", g_tsp_regs_match[i].compatible);
            return BSP_OK;
        }
        ret = mdmcp_reginfo_probe(child_node, i);
        if (ret != BSP_OK) {
            return ret;
        }
    }

    ret = mdmcp_baseinfo_probe(dev_node);
    if (ret != BSP_OK) {
        cs_error("get baseinfo error!");
        mdmcp_free_reg_group_info();
        return BSP_ERROR;
    }
    g_tsp_regs_drvdata.init = 1;

    cs_print("ok\n");
    return BSP_OK;
}

int mdmcp_coresight_stop(void)
{
    u32 cpu;

    if (!g_tsp_regs_drvdata.init) {
        cs_error("not inited\n");
        return 0;
    }

    for (cpu = 0; cpu < g_tsp_regs_drvdata.thread_num; cpu++) {
        mdmcp_ltrace_store((u32)cpu);
        mdmcp_tsp_regs_store((u32)cpu);
    }

    return 0;
}

#endif

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
/*lint --e{528}*/
late_initcall(mdmcp_coresight_init); /*lint !e528*/
#endif

int bsp_coresight_stop_cp(void)
{
    return mdmcp_coresight_stop();
}
