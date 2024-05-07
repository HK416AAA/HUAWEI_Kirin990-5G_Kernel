/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/semaphore.h>
#include <linux/regulator/consumer.h>

#include <product_config.h>
#include <hi_efuse.h>
#include <bsp_dt.h>
#include <bsp_icc.h>
#include <bsp_llt.h>
#include <bsp_sec_call.h>
#include <bsp_hardtimer.h>
#include <bsp_efuse.h>
#include <securec.h>
#define THIS_MODU mod_efuse

#define EFUSE_MAX_DELAY_TIME 0x1000000 /* 最大延迟时间 */
#define EFUSE_WRITE_PREPARE_DELAY 1000

void *g_efuse_addr = 0;
struct clk *g_efuse_clk = NULL;
struct regulator *g_efuse_regulator = NULL;
struct semaphore g_efuse_semaphore;

#ifndef CONFIG_TRUSTZONE
#define EFUSE_CLK_AUTO_GATE
#endif

int bsp_efuse_ops_prepare(void)
{
    int ret = EFUSE_OK;

    while (down_interruptible(&g_efuse_semaphore)) {
        ;
    }

#ifdef EFUSE_CLK_AUTO_GATE

    g_efuse_clk = (struct clk *)clk_get(NULL, "efuse_clk");
    if (IS_ERR(g_efuse_clk)) {
        efuse_print_error("clk_get error.\n");
        ret = EFUSE_ERROR_CLK_PREPARE_FAIL;
        goto clk_get_error;
    }

    ret = clk_prepare(g_efuse_clk);
    if (ret) {
        efuse_print_error("clk_prepare error, 0x%x.\n", (u32)ret);
        goto clk_prepare_error;
    }

    ret = clk_enable(g_efuse_clk);
    if (ret) {
        efuse_print_error("clk_enable error.\n");
        goto clk_enable_error;
    }

    return ret;

clk_enable_error:
    clk_unprepare(g_efuse_clk);

clk_prepare_error:
    clk_put(g_efuse_clk);

clk_get_error:
    up(&g_efuse_semaphore);

#endif

    return ret;
}

void bsp_efuse_ops_complete(void)
{
#ifdef EFUSE_CLK_AUTO_GATE
    clk_disable(g_efuse_clk);
    clk_unprepare(g_efuse_clk);
    clk_put(g_efuse_clk);
#endif

    up(&g_efuse_semaphore);
}

int bsp_efuse_write_prepare(void)
{
    int ret;

    ret = bsp_efuse_ops_prepare();
    if (ret) {
        return ret;
    }

    ret = regulator_enable(g_efuse_regulator);
    if (ret) {
        efuse_print_error("regulator_enable error.\n");
        goto regulator_enable_error;
    }

    udelay(EFUSE_WRITE_PREPARE_DELAY); /*lint !e737*/

    return ret;

regulator_enable_error:
    regulator_put(g_efuse_regulator);

    return ret;
}

void bsp_efuse_write_complete(void)
{
    regulator_disable(g_efuse_regulator);
    bsp_efuse_ops_complete();
}

#ifndef CONFIG_TRUSTZONE
static int efuse_wait_state(u32 status_bit, u32 wait_value)
{
    u32 value;
    u32 delay = EFUSE_MAX_DELAY_TIME;

    do {
        value = osl_reg_get_bit(g_efuse_addr + HI_EFUSEC_STATUS_OFFSET, status_bit, status_bit);
    } while ((value == wait_value) && (--delay));

    if (!delay) {
        return EFUSE_ERROR_TIMEOUT;
    }

    return EFUSE_OK;
}

int bsp_efuse_exit_pd(void)
{
    int ret = 0;

    if (osl_reg_get_bit(g_efuse_addr + HI_EFUSEC_STATUS_OFFSET, HI_EFUSE_PD_STAT_BIT, HI_EFUSE_PD_STAT_BIT)) {
        osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_PD_EN_BIT, HI_EFUSE_PD_EN_BIT, 0);

        ret = efuse_wait_state(HI_EFUSE_PD_STAT_BIT, 1);
        if (ret) {
            return ret;
        }
    }

    return ret;
}

/*lint -save -e718 -e746*/
/* ****************************************************************************
 * 函 数 名  : bsp_efuse_read
 *
 * 功能描述  : 按组读取EFUSE中的数据
 *
 * 输入参数  : group  起始group
 * num  数组长度（word数，不超过16）
 * 输出参数  : p_buf ：EFUSE中的数据
 *
 * 返 回 值  :
 *
 * 其他说明  :
 *
 * *************************************************************************** */
int bsp_efuse_read(u32 *p_buf, const u32 group, const u32 num)
{
    int ret;
    u32 count;
    u32 *buffer = NULL;

    if ((num == 0) || (group >= EFUSE_MAX_SIZE) || (num > EFUSE_MAX_SIZE) || (group + num > EFUSE_MAX_SIZE) ||
        (p_buf == NULL)) {
        efuse_print_error("error args, group=%d, num=%d.\n", group, num);
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_ops_prepare();
    if (ret) {
        return ret;
    }

    buffer = p_buf;

    ret = bsp_efuse_exit_pd();
    if (ret) {
        goto efuse_read_error;
    }

    osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_AIB_SEL_BIT, HI_EFUSE_AIB_SEL_BIT, 1);

    for (count = 0; count < num; count++) {
        osl_reg_set_bit(g_efuse_addr + HI_EFUSE_GROUP_OFFSET, HI_EFUSE_GROUP_LBIT, HI_EFUSE_GROUP_HBIT, group + count);
        osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_RD_EN_BIT, HI_EFUSE_RD_EN_BIT, 1);

        ret = efuse_wait_state(HI_EFUSE_RD_STAT_BIT, 0);
        if (ret) {
            goto efuse_read_error;
        }

        *buffer = osl_reg_get_bit(g_efuse_addr + HI_EFUSEC_DATA_OFFSET, HI_EFUSE_DATA_LBIT, HI_EFUSE_DATA_HBIT);
        buffer++;
    }

    osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_AIB_SEL_BIT, HI_EFUSE_AIB_SEL_BIT, 0);

efuse_read_error:
    bsp_efuse_ops_complete();

    return ret;
}
/*lint -restore*/
int bsp_efuse_sec_read(u32 *p_buf, u32 group, u32 num)
{
    return bsp_efuse_read(p_buf, group, num);
}

/* ****************************************************************************
 * 函 数 名  : bsp_efuse_write
 *
 * 功能描述  : 烧写Efsue
 *
 * 输入参数  : p_buf:待烧写的EFUSE值
 * group,Efuse地址偏移
 * len,烧写长度
 * 输出参数  :
 *
 * 返 回 值  :
 *
 * 其它说明  :
 *
 * *************************************************************************** */
int bsp_efuse_write(u32 *p_buf, const u32 group, const u32 len)
{
    int ret;
    u32 count;
    u32 value;
    u32 *buffer = NULL;

    if ((len == 0) || (group >= EFUSE_MAX_SIZE) || (len > EFUSE_MAX_SIZE) || (group + len > EFUSE_MAX_SIZE) ||
        (p_buf == NULL)) {
        efuse_print_error("error args, group=%d, len=%d.\n", group, len);
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_write_prepare();
    if (ret) {
        return ret;
    }

    if (osl_reg_get_bit(g_efuse_addr + HI_HW_CFG_OFFSET, HI_EFUSE_DISFLAG_BIT, HI_EFUSE_DISFLAG_BIT)) {
        efuse_print_error("write is disable.\n");
        goto efuse_write_error;
    }

    ret = bsp_efuse_exit_pd();
    if (ret) {
        goto efuse_write_error;
    }

    osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_AIB_SEL_BIT, HI_EFUSE_AIB_SEL_BIT, 1);
    osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_COUNT_OFFSET, HI_EFUSE_COUNT_LBIT, HI_EFUSE_COUNT_HBIT, EFUSE_COUNT_CFG);
    osl_reg_set_bit(g_efuse_addr + HI_PGM_COUNT_OFFSET, HI_EFUSE_PGM_COUNT_LBIT, HI_EFUSE_PGM_COUNT_HBIT,
        PGM_COUNT_CFG);
    osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_PRE_PG_BIT, HI_EFUSE_PRE_PG_BIT, 1);

    ret = efuse_wait_state(HI_EFUSE_PGENB_STAT_BIT, 0);
    if (ret) {
        goto efuse_write_error;
    }

    buffer = p_buf;
    for (count = 0; count < len; count++) {
        osl_reg_set_bit(g_efuse_addr + HI_EFUSE_GROUP_OFFSET, HI_EFUSE_GROUP_LBIT, HI_EFUSE_GROUP_HBIT, group + count);
        value = is_in_llt() ? 0 : *buffer;
        osl_reg_set_bit(g_efuse_addr + HI_PG_VALUE_OFFSET, HI_EFUSE_PG_VALUE_LBIT, HI_EFUSE_PG_VALUE_HBIT, value);
        osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_PGEN_BIT, HI_EFUSE_PGEN_BIT, 1);

        ret = efuse_wait_state(HI_EFUSE_PG_STAT_BIT, 0);
        if (ret) {
            goto efuse_write_error;
        }
        buffer++;
    }

efuse_write_error:
    osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_AIB_SEL_BIT, HI_EFUSE_AIB_SEL_BIT, 0);
    osl_reg_set_bit(g_efuse_addr + HI_EFUSEC_CFG_OFFSET, HI_EFUSE_PRE_PG_BIT, HI_EFUSE_PRE_PG_BIT, 0);

    bsp_efuse_write_complete();

    return ret;
}

static int __init bsp_efuse_init(void)
{
    void *addr = NULL;
    struct clk *efuse_clk = NULL;
    device_node_s *dev = NULL;
    const char name[] = "hisilicon,efuse_balong_app";

    sema_init(&g_efuse_semaphore, 1);

    dev = bsp_dt_find_compatible_node(NULL, NULL, name);
    if (NULL == dev) {
        efuse_print_error("device node not found\n");
        return EFUSE_ERROR;
    }

    addr = bsp_dt_iomap(dev, 0);
    if (0 == addr) {
        efuse_print_error("remap addr fail\n");
        return EFUSE_ERROR;
    }

    g_efuse_addr = addr;

    efuse_clk = (struct clk *)clk_get(NULL, "efuse_clk");
    if (IS_ERR(efuse_clk)) {
        efuse_print_error("efuse clk cannot get.\n");
        return EFUSE_ERROR;
    }

    g_efuse_regulator = regulator_get(NULL, "EFUSE-vcc");
    if (g_efuse_regulator == NULL) {
        efuse_print_error("regulator_get error.\n");
        return EFUSE_ERROR_REGULATOR_GET_FAIL;
    }

    (void)clk_prepare(efuse_clk);
    (void)clk_enable(efuse_clk);
    clk_disable(efuse_clk);
    clk_unprepare(efuse_clk);
    clk_put(efuse_clk);

    efuse_print_info("efuse init ok.\n");

    return EFUSE_OK;
}

#else

u32 g_efuse_buffer[EFUSE_MAX_SIZE];

int bsp_efuse_inner_read(u32 *p_buf, u32 group, u32 num, u32 is_sec)
{
    u32 i;
    int ret;
    FUNC_CMD_ID sec_call_cmd = is_sec ? FUNC_EFUSE_SEC_READ : FUNC_EFUSE_READ;

    if ((num == 0) || (group >= EFUSE_MAX_SIZE) || (num > EFUSE_MAX_SIZE) || (group + num > EFUSE_MAX_SIZE) ||
        (p_buf == NULL)) {
        efuse_print_error("error args, group=%d, num=%d, p_buf=0x%x.\n", group, num, (u32)(uintptr_t)p_buf);
        return EFUSE_ERROR;
    }

    for (i = 0; i < sizeof(g_efuse_buffer) / sizeof(g_efuse_buffer[0]); i++) {
        g_efuse_buffer[i] = 0;
    }

    ret = bsp_efuse_ops_prepare();
    if (ret) {
        return ret;
    }

    if (!is_in_llt()) {
        ret = bsp_sec_call_ext(sec_call_cmd, 0, (void *)&g_efuse_buffer[0], sizeof(g_efuse_buffer));
    }

    bsp_efuse_ops_complete();

    for (i = 0; i < num; i++) {
        p_buf[i] = g_efuse_buffer[group + i];
    }

    return ret;
}
int bsp_efuse_read(u32 *p_buf, u32 group, u32 num)
{
    return bsp_efuse_inner_read(p_buf, group, num, 0);
}
int bsp_efuse_sec_read(u32 *p_buf, u32 group, u32 num)
{
    return bsp_efuse_inner_read(p_buf, group, num, 1);
}
int bsp_efuse_write(u32 *p_buf, u32 group, u32 len)
{
    int ret;
    FUNC_CMD_ID cmd;

    if ((len == 0) || (group >= EFUSE_MAX_SIZE) || (len > EFUSE_MAX_SIZE) || (group + len > EFUSE_MAX_SIZE) ||
        (p_buf == NULL)) {
        efuse_print_error("error args, group=%d, len=%d, p_buf=0x%x.\n", group, len, (u32)(uintptr_t)p_buf);
        return EFUSE_ERROR;
    }

    ret = memset_s(g_efuse_buffer, sizeof(g_efuse_buffer), 0, sizeof(g_efuse_buffer));
    if (ret != EOK) {
        efuse_print_error("<%s> memset_s error, ret = %d\n", __FUNCTION__, ret);
    }

    ret = memcpy_s(&g_efuse_buffer[group], sizeof(u32) * (EFUSE_MAX_SIZE - group), p_buf, sizeof(u32) * len);
    if (ret != EOK) {
        efuse_print_error("<%s> memcpy_s error, ret = %d\n", __FUNCTION__, ret);
    }

    ret = bsp_efuse_write_prepare();
    if (ret) {
        return ret;
    }

#ifdef CONFIG_DMPU
    cmd = (group == EFUSE_LAYOUT_KCE_OFFSET) ? FUNC_EFUSE_WRITE_WITH_DMPU : FUNC_EFUSE_WRITE;
#else
    cmd = FUNC_EFUSE_WRITE;
#endif

    if (!is_in_llt()) {
        ret = bsp_sec_call_ext(cmd, 0, (void *)&g_efuse_buffer[0], sizeof(g_efuse_buffer));
    }

    bsp_efuse_write_complete();

    return ret;
}
static int __init bsp_efuse_init(void)
{
    sema_init(&g_efuse_semaphore, 1);

    g_efuse_regulator = regulator_get(NULL, "EFUSE-vcc");
    if (g_efuse_regulator == NULL) {
        efuse_print_error("regulator_get error.\n");
        return EFUSE_ERROR_REGULATOR_GET_FAIL;
    }

    return EFUSE_OK;
}

#endif

int bsp_efuse_read_debug(u32 group)
{
    int ret;
    u32 value = 0;

    ret = bsp_efuse_read(&value, group, 1);
    efuse_print_error("group = 0x%08X, value = 0x%08X\n", group, value);

    return ret;
}

int bsp_efuse_write_debug(u32 group, u32 value)
{
    int ret;

    ret = bsp_efuse_write(&value, group, 1);
    if (!ret) {
        ret = bsp_efuse_read_debug(group);
    }

    return ret;
}

void bsp_efuse_show(void)
{
    u32 group;
    u32 value = 0;

    for (group = 0; group < EFUSE_MAX_SIZE; group++) {
        if (!bsp_efuse_read(&value, group, 1)) {
            efuse_print_error("group = 0x%08X, value = 0x%08X\n", group, value);
        } else {
            efuse_print_error("efuse group %d read fail.\n", group);
            return;
        }
    }
}

fs_initcall_sync(bsp_efuse_init);

EXPORT_SYMBOL(bsp_efuse_show);
EXPORT_SYMBOL(bsp_efuse_read);
EXPORT_SYMBOL(bsp_efuse_write);
EXPORT_SYMBOL(bsp_efuse_read_debug);
EXPORT_SYMBOL(bsp_efuse_write_debug);
