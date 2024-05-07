/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */

#include <product_config_ddr.h>
#include <bsp_dt.h>
#include <bsp_fiq.h>
#include <bsp_slice.h>
#include <bsp_sysctrl.h>
#include <bsp_print.h>
#include <osl_bio.h>
#include <osl_types.h>
#include <osl_irq.h>
#include <securec.h>
#include <linux/delay.h>

#undef THIS_MODU
#define THIS_MODU mod_nmi

#define BASE_ADDR TSP_MPSCU_MP_REG_BASE
#define CORE_NUM 16
#define DSS_NUM 4
#define NMI_MAX 8  // MAX interrupt sources per thread
#define CORE_NUM_PER_REG (32 / NMI_MAX)
#define NMI_CHECK_TIME 2000U

#define nmi_err(fmt, ...) (bsp_err(fmt, ##__VA_ARGS__))

struct nmi_ctrl {
    u32 nmi_init;
    u32 recv_count[NMI_MAX];
    u32 send_count[NMI_MAX];
    void *base_sysctrl;
    u32 nmi_sysctrl[DSS_NUM][3];  // en, dis, status  bit[7:0]->nmi0/4/8/12	 bit[15:8]->nmi1/5/9/13
                                  // bit[23:16]->nmi2/6/10/14  bit[31:24]->nmi3/8/10/15
};
struct nmi_ctrl g_nmi_ctrl;
#define IS_NMI_SYS_VALID(reg_id, idx) (g_nmi_ctrl.nmi_sysctrl[reg_id][idx] > 0)

int bsp_nmi_trigger(u32 core_mask, fiq_num fiq_id)
{
    u32 reg_id = 0;
    u32 reg_core_id = 0;
    u32 core_id = 0;
    u32 tmp = 0;
    u32 trigger_value[DSS_NUM] = { 0, 0, 0, 0 };

    if (!g_nmi_ctrl.nmi_init) {
        nmi_err("nmi init fail\n");
        return BSP_ERROR;
    }

    if (fiq_id >= NMI_MAX) {
        nmi_err("nmi fiq_id %d error\n", fiq_id);
        return BSP_ERROR;
    }

    for (core_id = 0; core_id < CORE_NUM; core_id++) {
        tmp = ((u32)0x1 << (u32)core_id);
        if (tmp & core_mask) {
            reg_core_id = (u32)(core_id % CORE_NUM_PER_REG);
            reg_id = (u32)(core_id / CORE_NUM_PER_REG);
            trigger_value[reg_id] |= ((u32)0x1 << (u32)(NMI_MAX * reg_core_id + fiq_id));
        }
    }
    for (reg_id = 0; reg_id < DSS_NUM; reg_id++) {
        if (IS_NMI_SYS_VALID(reg_id, 0) && (trigger_value[reg_id] > 0)) {
            writel(trigger_value[reg_id], (void *)(g_nmi_ctrl.base_sysctrl + g_nmi_ctrl.nmi_sysctrl[reg_id][0]));
        }
    }
    return BSP_OK;
}

int check_nmi_status(fiq_num fiq_id)
{
    u32 reg_id;
    u32 nmi_status[DSS_NUM] = { 0, 0, 0, 0 };
    u32 res = 0;
    udelay(NMI_CHECK_TIME);

    if (!g_nmi_ctrl.nmi_init) {
        nmi_err("nmi init fail\n");
        return BSP_ERROR;
    }

    if (fiq_id >= NMI_MAX) {
        nmi_err("nmi fiq_id %d error\n", fiq_id);
        return BSP_ERROR;
    }

    for (reg_id = 0; reg_id < DSS_NUM; reg_id++) {
        if (g_nmi_ctrl.nmi_sysctrl[reg_id][0x2] != 0) {
            nmi_status[reg_id] = readl((void *)(g_nmi_ctrl.base_sysctrl + g_nmi_ctrl.nmi_sysctrl[reg_id][0x2]));
            res += nmi_status[reg_id];
        }
        if (nmi_status[reg_id] != 0) {
            nmi_err("nmi%d, send_cnt[%u], recv_cnt[%u], error, status:0x%x\n", fiq_id, g_nmi_ctrl.send_count[fiq_id],
                g_nmi_ctrl.recv_count[fiq_id], nmi_status[reg_id]);
        }
    }
    if (res) {
        nmi_err("recv fail\n");
        return BSP_ERROR;
    }
    g_nmi_ctrl.recv_count[fiq_id]++;
    nmi_err("nmi%d, send_cnt[%u], recv_cnt[%u], OK\n", fiq_id, g_nmi_ctrl.send_count[fiq_id],
                g_nmi_ctrl.recv_count[fiq_id]);

    return BSP_OK;
}

int bsp_send_cp_fiq(fiq_num fiq_id)
{
    int res;
    g_nmi_ctrl.send_count[fiq_id]++;
    res = bsp_nmi_trigger(0xFFFF, fiq_id);
    if (res != BSP_OK) {
        return BSP_ERROR;
    }
    return check_nmi_status(fiq_id);
}

int get_nmi_dts(void)
{
    device_node_s *node = NULL;

    node = bsp_dt_find_compatible_node(NULL, NULL, "hisilicon,nmi");
    if (node == NULL) {
        nmi_err("can't find nmi node\n");
        return BSP_ERROR;
    }

    if (bsp_dt_property_read_u32_array(node, "sysctrl_nmi", (u32 *)&g_nmi_ctrl.nmi_sysctrl,
        sizeof(g_nmi_ctrl.nmi_sysctrl) / sizeof(u32))) {
        nmi_err("read sysctrl_nmi err\n");
        return BSP_ERROR;
    }
    return BSP_OK;
}

__init int nmi_init(void)
{
    // global value init
    if (memset_s((void *)&g_nmi_ctrl, sizeof(struct nmi_ctrl), 0, sizeof(struct nmi_ctrl))) {
        nmi_err("g_nmi_ctrl memset fail\n");
        return BSP_ERROR;
    }

    // read sysctrl peri
    g_nmi_ctrl.base_sysctrl = bsp_sysctrl_addr_byindex(SYSCTRL_AP_PERI);
    if (g_nmi_ctrl.base_sysctrl == NULL) {
        nmi_err("get sysctrl_ap_peri err\n");
        return BSP_ERROR;
    }

    // read dts config
    if (get_nmi_dts() != BSP_OK) {
        nmi_err("get nmi dts error\n");
        return BSP_ERROR;
    }

    g_nmi_ctrl.nmi_init = 1;
    return BSP_OK;
}

EXPORT_SYMBOL(bsp_nmi_trigger);
EXPORT_SYMBOL(bsp_send_cp_fiq);
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
arch_initcall(nmi_init);
#endif
