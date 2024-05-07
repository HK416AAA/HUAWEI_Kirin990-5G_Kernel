/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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
#include "eicc_platform.h"
#include "eicc_dts.h"
#include "eicc_device.h"
#include "eicc_driver.h"
#include "eicc_core.h"

/* 只能在唤醒中断上下文中使用 */
int eicc_relv_dev_chn_get(struct eicc_device *prdev, u32 ripipe_id, struct eicc_device **ppdev, u32 *plipie_id)
{
    u32 devid;
    u32 lipipeid;
    u32 addrL, addrH;
    eiccsoc_ptr_t lipipe_pa;
    struct eicc_device *pldev = NULL;
    addrL = readl((void *)((char *)prdev->base_va + EICC_IPIPE_CFG_ADDR_L(ripipe_id)));
    addrH = readl((void *)((char *)prdev->base_va + EICC_IPIPE_CFG_ADDR_H(ripipe_id)));

    lipipe_pa = EICC_ADDR_MK(addrL, addrH);
    lipipeid = ((lipipe_pa & 0x7fff) - 0x6000) >> 0x7;

    for (devid = 0; devid < EICC_DEVICE_NUM_MAX; devid++) {
        pldev = eicc_device_get_fast(devid);
        if (pldev == NULL) {
            continue;
        }
        if ((pldev->base_pa & 0xFFFF8000) != (lipipe_pa & 0xFFFF8000)) {
            /* not belongs to this device */
            continue;
        }

        /*  do check */
        if (lipipeid >= pldev->pipepair_cnt || lipipe_pa != pldev->base_pa + EICC_IPIPE_BASE_ADDR_L(lipipeid)) {
            /* big error */
            return -1;
        }
        break;
    }
    if (devid >= EICC_DEVICE_NUM_MAX) {
        return -1;
    }
    *ppdev = pldev;
    *plipie_id = lipipeid;
    return 0;
}

static inline u32 eicc_ffs(u32 num)
{
    u32 i;
    for (i = 0; i < EICC_PIPE_CNT_PER_REG; i++) {
        if ((num >> i) & 1) {
            return i + 1;
        }
    }
    return 0;
}

void eicc_core_unsec(const struct eicc_device *pdev, u32 coreid)
{
    writel(0x10001U << (coreid & 0xF), (void *)((char *)pdev->base_va + EICC_CORE_SECCTRL));
}

void eicc_opipe_unsec(const struct eicc_device *pdev, u32 pipeid)
{
    /* 每个寄存器对应16个pipe */
    u32 reg_idx = (pipeid >> 4) & 0x3;
    u32 regs[EICC_PIPE_SEC_REGS_CNT] = {EICC_OPIPE_SECCTRL0, EICC_OPIPE_SECCTRL1, EICC_OPIPE_SECCTRL2,
                                        EICC_OPIPE_SECCTRL3};
    u32 regs_v210[EICC_PIPE_SEC_REGS_CNT] = {EICC_V210_OPIPE_SECCTRL0, EICC_V210_OPIPE_SECCTRL1,
                                             EICC_V210_OPIPE_SECCTRL2, EICC_V210_OPIPE_SECCTRL3};
    if (pdev->version_id == EICC_HW_VERSION_V210) {
        writel(0x10001U << (pipeid & 0xf), (void *)((char *)pdev->base_va + regs_v210[reg_idx]));
    } else {
        writel(0x10001U << (pipeid & 0xf), (void *)((char *)pdev->base_va + regs[reg_idx]));
    }
}
void eicc_ipipe_unsec(const struct eicc_device *pdev, u32 pipeid)
{
    /* 每个寄存器对应16个pipe */
    u32 reg_idx = (pipeid >> 4) & 0x3;
    u32 regs[EICC_PIPE_SEC_REGS_CNT] = {EICC_IPIPE_SECCTRL0, EICC_IPIPE_SECCTRL1, EICC_IPIPE_SECCTRL2,
                                        EICC_IPIPE_SECCTRL3};
    u32 regs_v210[EICC_PIPE_SEC_REGS_CNT] = {EICC_V210_IPIPE_SECCTRL0, EICC_V210_IPIPE_SECCTRL1,
                                             EICC_V210_IPIPE_SECCTRL2, EICC_V210_IPIPE_SECCTRL3};
    if (pdev->version_id == EICC_HW_VERSION_V210) {
        writel(0x10001U << (pipeid & 0xf), (void *)((char *)pdev->base_va + regs_v210[reg_idx]));
    } else {
        writel(0x10001U << (pipeid & 0xf), (void *)((char *)pdev->base_va + regs[reg_idx]));
    }
}

int ipipe_neg_check(const void *base_addr, u32 ipipe_id)
{
    int ret = 0;
    u32 loop = 0;
    union ipipe_stat i_state = { 0 };
    while (loop++ < EICC_IPIPE_STOP_WAIT) {
        i_state.val = eicc_ipipe_local_status(base_addr, ipipe_id);
        if (i_state.union_stru.pt_stat == EICC_PM_REQ_PD) {
            /* ipipe当localen和remote都为1 然后其中一个变为0. 也就是说上电握手后才有这个过程，没经过握手没这个过程 */
            /* 从这个状态 要进入下一个状态，必须对端为local en为1才行，不然会卡死在这个状态 */
            continue;
        } else if (i_state.union_stru.pt_stat == EICC_NOT_PM_STAT) {
            /* EICCv100， 在影子ipipe回推给实体拒绝下电的时，影子ipipe干完活了, 会造成影子ipipe进入这种状态(bug) */
            /* v200没有上面的bug，该状态只在未完成上电握手又发起下电的场景(ipipe置loclen为1后置为0，而remote一直为0) */
            break;
        } else if (i_state.union_stru.pt_stat == EICC_PM_REFUSE_PD) {
            ret = EICC_ERR_PM_IPIPE_NEG_REFUSE;
            break;
        } else if (i_state.union_stru.pt_stat == EICC_PM_ALLOW_PD) {
            break;
        } else {
            ret = EICC_ERR_PM_IPIPE_NEG_FAIL;
        }
    }
    if (loop >= EICC_IPIPE_STOP_WAIT) {
        ret = EICC_ERR_PM_IPIPE_NEG_TIMEOUT;
    }
    return ret;
}

int ipipe_status_check(const void *base_addr, u32 ipipe_id)
{
    int ret = 0;
    u32 loop = 0;
    union ipipe_stat i_state = { 0 };
    while (loop++ < EICC_IPIPE_IDLE_WAIT) {
        i_state.val = eicc_ipipe_local_status(base_addr, ipipe_id);
        if (i_state.union_stru.is_idle == 0x1) {
            break;
        }
    }
    if (loop >= EICC_IPIPE_IDLE_WAIT) {
        ret = EICC_ERR_PM_IPIPE_NOT_IDLE;
    }

    return ret;
}

int opipe_status_check(const void *base_addr, u32 opipe_id)
{
    int loop = 0;
    while (loop++ < EICC_OPIPE_STOP_WAIT) {
        if (!eicc_opipe_stat_isbusy(base_addr, opipe_id)) {
            break;
        }
    }
    if (loop >= EICC_OPIPE_STOP_WAIT) {
        return EICC_ERR_PM_OPIPE_NOT_IDLE;
    }
    return 0;
}

void eicc_opipe_startup(void *base_addr, struct opipe_config *opipe)
{
    /*
     * 步骤 1  配置寄存器PIPE_PTR_ADDR_H，PIPE_PTR_ADDR_L，指定pipe指针更新的地址；
     * 1.  opipe要推送的指针是opipe_rptr；
     * 2.  ipipe要推送的指针是ipipe_wptr；
     * 3.  该寄存器的内容是源端、目的端core的llram/tcm中的地址。
     */
    /* 配置opipe推送的读指针到源端llram/tcm地址中 */
    writel_relaxed(EICC_ADDR_LO(opipe->ptr_paddr), (void *)((char *)base_addr + EICC_OPIPE_PTR_ADDR_L(opipe->id)));
    writel_relaxed(EICC_ADDR_HI(opipe->ptr_paddr), (void *)((char *)base_addr + EICC_OPIPE_PTR_ADDR_H(opipe->id)));

    /* 步骤 3  配置寄存器PIPE_BASE_ADDR_H，PIPE_BASE_ADDR_L，指定pipe基地址； */
    writel_relaxed(EICC_ADDR_LO(opipe->ringbuf_pa), (void *)((char *)base_addr + EICC_OPIPE_BASE_ADDR_L(opipe->id)));
    writel_relaxed(EICC_ADDR_HI(opipe->ringbuf_pa), (void *)((char *)base_addr + EICC_OPIPE_BASE_ADDR_H(opipe->id)));

    /* 步骤 4  配置寄存器PIPE_DEPTH，指定pipe深度； */
    writel_relaxed(opipe->depth, (void *)((char *)base_addr + EICC_OPIPE_DEPTH(opipe->id)));

    /*
     * 步骤 5  配置寄存器PIPE_WPTR和PIPE_EPTR，指定pipe的读写指针；
     * 1.  默认下，pipe从0地址开始搬移；
     * 2.  软件在恢复pipe配置时，可能从指定的读写指针开始搬移。
     */
    writel_relaxed(opipe->wptr, (void *)((char *)base_addr + EICC_OPIPE_WPTR(opipe->id)));
    writel_relaxed(opipe->rptr, (void *)((char *)base_addr + EICC_OPIPE_RPTR(opipe->id)));

    /*
     * 步骤 6  配置寄存器PIPE_CTRL中除了pipe_en以外的控制信息；
     * 1.  配置pipe_type，指定pipe类型；
     * 2.  配置pipe_priority，指定pipe优先级；
     * 3.  配置pipe的上下水线阈值。
     */
    writel_relaxed(opipe->opipe_uctrl.val, (void *)((char *)base_addr + EICC_OPIPE_CTRL(opipe->id)));

    /* 步骤 7  上述步骤没有顺序要求； */
    /*
     * 步骤 8  完成上述步骤后，配置寄存器PIPE_CTRL中的pipe_en，启动pipe。
     * 1.  opipe的pipe_en只有1bits；
     * 2.  ipipe的pipe_en有2bits，分为local_en和remote_en；
     * 3.  内部通道的ipipe，remote_en无效；
     * 4.
     * 外部通道的ipipe的pipe_en的配置，根据上电初始化和下电关闭，流程不同，详见"上电配置pipe_en流程"和"下电配置pipe_en流程"。
     */
}

void eicc_ipipe_startup(void *base_addr, struct ipipe_config *ipipe)
{
    /*
     * 步骤 1  配置寄存器PIPE_PTR_ADDR_H，PIPE_PTR_ADDR_L，指定pipe指针更新的地址；
     * 1.  opipe要推送的指针是opipe_rptr；
     * 2.  ipipe要推送的指针是ipipe_wptr；
     * 3.  该寄存器的内容是源端、目的端core的llram/tcm中的地址。
     */
    /* 配置ipipe推送的写指针到目的端llram/tcm地址中 */
    /* 目的端的tcm地址ip看到的和cpu看到的不一样? */
    writel_relaxed(EICC_ADDR_LO(ipipe->ptr_paddr), (void *)((char *)base_addr + EICC_IPIPE_PTR_ADDR_L(ipipe->id)));
    writel_relaxed(EICC_ADDR_HI(ipipe->ptr_paddr), (void *)((char *)base_addr + EICC_IPIPE_PTR_ADDR_H(ipipe->id)));

    /*
     * 步骤 2  配置寄存器PIPE_CFG_ADDR_H，PIPE_CFG_ADDR_L，指定对端EICC关联的pipe配置空间起始地址；
     * 1.  只有外部通道的ipipe需要配此寄存器；
     * 2.  用于推送描述符和remote_en；
     */
    writel_relaxed(EICC_ADDR_LO(ipipe->relv_pipe_paddr),
        (void *)((char *)base_addr + EICC_IPIPE_CFG_ADDR_L(ipipe->id)));
    writel_relaxed(EICC_ADDR_HI(ipipe->relv_pipe_paddr),
        (void *)((char *)base_addr + EICC_IPIPE_CFG_ADDR_H(ipipe->id)));

    /* 步骤 3  配置寄存器PIPE_BASE_ADDR_H，PIPE_BASE_ADDR_L，指定pipe基地址； */
    writel_relaxed(EICC_ADDR_LO(ipipe->ringbuf_pa), (void *)((char *)base_addr + EICC_IPIPE_BASE_ADDR_L(ipipe->id)));
    writel_relaxed(EICC_ADDR_HI(ipipe->ringbuf_pa), (void *)((char *)base_addr + EICC_IPIPE_BASE_ADDR_H(ipipe->id)));

    /* 步骤 4  配置寄存器PIPE_DEPTH，指定pipe深度； */
    writel_relaxed(ipipe->depth, (void *)((char *)base_addr + EICC_IPIPE_DEPTH(ipipe->id)));

    /*
     * 步骤 5  配置寄存器PIPE_WPTR和PIPE_EPTR，指定pipe的读写指针；
     * 1.  默认下，pipe从0地址开始搬移；
     * 2.  软件在恢复pipe配置时，可能从指定的读写指针开始搬移。
     */
    writel_relaxed(ipipe->wptr & 0xFFFF, (void *)((char *)base_addr + EICC_IPIPE_WPTR(ipipe->id)));
    writel_relaxed(ipipe->rptr, (void *)((char *)base_addr + EICC_IPIPE_RPTR(ipipe->id)));

    /*
     * 步骤 6  配置寄存器PIPE_CTRL中除了pipe_en以外的控制信息；
     * 1.  配置pipe_type，指定pipe类型；
     * 2.  配置pipe_priority，指定pipe优先级；
     * 3.  配置pipe的上下水线阈值。
     */
    writel_relaxed(ipipe->ipipe_uctrl.val, (void *)((char *)base_addr + EICC_IPIPE_CTRL(ipipe->id)));

    /*
     * 步骤 8  完成上述步骤后，配置寄存器PIPE_CTRL中的pipe_en，启动pipe。
     * 1.  opipe的pipe_en只有1bits；
     * 2.  ipipe的pipe_en有2bits，分为local_en和remote_en；
     * 3.  内部通道的ipipe，remote_en无效；
     * 4.
     * 外部通道的ipipe的pipe_en的配置，根据上电初始化和下电关闭，流程不同，详见"上电配置pipe_en流程"和"下电配置pipe_en流程"。
     */
}

void eicc_shadow_ipipe_startup(void *base_addr, struct ipipe_config *ipipe)
{
    /*
     * 配置寄存器PIPE_CFG_ADDR_H，PIPE_CFG_ADDR_L，指定对端EICC关联的pipe配置空间起始地址；
     * 1.  只有外部通道的ipipe需要配此寄存器；
     * 2.  用于推送描述符和remote_en；
     */
    writel_relaxed(EICC_ADDR_LO(ipipe->relv_pipe_paddr),
        (void *)((char *)base_addr + EICC_IPIPE_CFG_ADDR_L(ipipe->id)));
    writel_relaxed(EICC_ADDR_HI(ipipe->relv_pipe_paddr),
        (void *)((char *)base_addr + EICC_IPIPE_CFG_ADDR_H(ipipe->id)));

    writel_relaxed(ipipe->ipipe_uctrl.val, (void *)((char *)base_addr + EICC_IPIPE_CTRL(ipipe->id)));
}

static void eicc_opipe_irq_handler(struct eicc_device *pdev, u32 opipe_id)
{
    u32 int_level_status;

    int_level_status = eicc_opipe_devint_stat(pdev->base_va, opipe_id);
    if (EICC_INT_WR_OVERFLOW & int_level_status) {
        eicc_print_trace("EICC_INT_WR_OVERFLOW detected for  opipe_id 0x%x \n", opipe_id);
        eicc_opipe_devint_dis(pdev->base_va, opipe_id, EICC_INT_WR_OVERFLOW);
        eicc_normal_event_notify(EICC_INT_WR_OVERFLOW, pdev, opipe_id);
    } else if (EICC_INT_WR_UNDERFLOW & int_level_status) {
        eicc_print_trace("EICC_INT_WR_UNDERFLOW detected for  opipe_id 0x%x \n", opipe_id);
        eicc_opipe_devint_dis(pdev->base_va, opipe_id, EICC_INT_WR_UNDERFLOW);
        eicc_normal_event_notify(EICC_INT_WR_UNDERFLOW, pdev, opipe_id);
    } else if (EICC_INT_WR_DATA_DONE & int_level_status) {
        eicc_print_trace("EICC_INT_WR_DATA_DONE detected for  opipe_id 0x%x \n", opipe_id);
        eicc_opipe_devint_clr(pdev->base_va, opipe_id, EICC_INT_WR_DATA_DONE);
    } else if (EICC_INT_WR_PIPE_ERR & int_level_status) {
        eicc_print_crit("EICC_INT_WR_PIPE_ERR detected for  opipe_id 0x%x \n", opipe_id);
        eicc_opipe_devint_clr(pdev->base_va, opipe_id, EICC_INT_WR_PIPE_ERR);
    } else {
        eicc_print_crit("eicc_ipipe_irq_handler  opipe_id 0x%x no detected \n", opipe_id);
    }

    return;
}

irqreturn_t eicc_localsend_handler(int irq_no, void *arg)
{
    u32 int_pipe_status;
    u32 pipe_id;
    struct irq_bundle *bundle = (struct irq_bundle *)arg;
    struct eicc_device *pdev = bundle->pdev;

    int_pipe_status = readl((void *)((char *)pdev->base_va + EICC_CORE_OPIPE_INT_STAT0(bundle->cpu_id)));
    if (int_pipe_status) {
        pipe_id = eicc_ffs(int_pipe_status);
        eicc_opipe_irq_handler(pdev, pipe_id - 1);
        return IRQ_HANDLED;
    }
    int_pipe_status = readl((void *)((char *)pdev->base_va + EICC_CORE_OPIPE_INT_STAT1(bundle->cpu_id)));
    if (int_pipe_status) {
        pipe_id = eicc_ffs(int_pipe_status);
        eicc_opipe_irq_handler(pdev, pipe_id + EICC_PIPE_CNT_PER_REG - 1);
        return IRQ_HANDLED;
    }
    return IRQ_NONE;
}

static void eicc_ipipe_irq_handler(struct eicc_device *pdev, u32 ipipe_id)
{
    u32 int_level_status;
    u32 lipipe_id;
    u32 is_inner_irq = 0;
    struct eicc_device *pldev = NULL;
    struct eicc_channel *pchannel = NULL;
    /* 需要判断这是个PIPE是不是自己可以直接访问的PIPE,
    比如EICC_INT_RD_PIPE_WAKEUP可能是影子报过来的 */
    if (ipipe_id < EICC_CHN_ID_MAX) {
        pchannel = pdev->channels[ipipe_id];
        if (pchannel != NULL && pchannel->type == EICC_CHN_TYPE_MSG_INRRECV) {
            is_inner_irq = 1;
        }
    }

    int_level_status = eicc_ipipe_devint_stat(pdev->base_va, ipipe_id);
    if (EICC_INT_RD_DATA_ARV & int_level_status) {
        eicc_print_trace("EICC_INT_RD_DATA_ARV detected for  ipipe_id 0x%x \n", ipipe_id);
        eicc_ipipe_devint_clr(pdev->base_va, ipipe_id, EICC_INT_RD_DATA_ARV);
        eicc_normal_event_notify(EICC_INT_RD_DATA_ARV, pdev, ipipe_id);
    } else if (EICC_INT_RD_PIPE_WAKEUP & int_level_status) {
        eicc_print_trace("EICC_INT_RD_PIPE_WAKEUP detected for remote ipipe_id 0x%x \n", ipipe_id);
        /* 清唤醒中断 */
        eicc_ipipe_devint_clr(pdev->base_va, ipipe_id, EICC_INT_RD_PIPE_WAKEUP);
        if (is_inner_irq) {
            return;
        } else {
            /* 根据对端设备和pipe里的信息获取本端的dev和channel */
            if (eicc_relv_dev_chn_get(pdev, ipipe_id, &pldev, &lipipe_id) == 0) {
                /* 设置本端对应ipipe的接收flag或者直接打开remote_en */
                eicc_wakeup_event_notify(pldev, lipipe_id);
            } else {
                eicc_print_crit("relv dev and chn get failed\n");
            }
        }
    } else {
        eicc_print_crit("eicc_ipipe_irq_handler  ipipe_id 0x%x no detected \n", ipipe_id);
    }
    return;
}

irqreturn_t eicc_localrecv_handler(int irq_no, void *arg)
{
    u32 int_pipe_status;
    u32 pipe_id;
    struct irq_bundle *bundle = (struct irq_bundle *)arg;
    struct eicc_device *pdev = bundle->pdev;

    int_pipe_status = readl((void *)((char *)pdev->base_va + EICC_CORE_IPIPE_INT_STAT0(bundle->cpu_id)));
    if (int_pipe_status) {
        pipe_id = eicc_ffs(int_pipe_status);
        eicc_ipipe_irq_handler(pdev, pipe_id - 1);
        return IRQ_HANDLED;
    }
    int_pipe_status = readl((void *)((char *)pdev->base_va + EICC_CORE_IPIPE_INT_STAT1(bundle->cpu_id)));
    if (int_pipe_status) {
        pipe_id = eicc_ffs(int_pipe_status);
        eicc_ipipe_irq_handler(pdev, pipe_id + EICC_PIPE_CNT_PER_REG - 1);
        return IRQ_HANDLED;
    }
    return IRQ_NONE;
}

/* pdev should be a remote eicc device( and must poweon) */
static void eicc_shadow_ipipe_irq_handler(struct eicc_device *pdev, u32 ipipe_id)
{
    u32 int_level_status;
    u32 lipipe_id;
    struct eicc_device *pldev = NULL;

    /* 此函数直接访问非本子系统的外设，必须由流程保证远端外设在此访问过程不会下电 */
    int_level_status = eicc_ipipe_devint_stat(pdev->base_va, ipipe_id);
    if (EICC_INT_RD_PIPE_WAKEUP & int_level_status) {
        eicc_print_trace("EICC_INT_RD_PIPE_WAKEUP detected for remote ipipe_id 0x%x \n", ipipe_id);
        /* 清对端的唤醒中断 */
        eicc_ipipe_devint_clr(pdev->base_va, ipipe_id, EICC_INT_RD_PIPE_WAKEUP);
        /* 根据对端设备和pipe里的信息获取本端的dev和channel */
        if (eicc_relv_dev_chn_get(pdev, ipipe_id, &pldev, &lipipe_id) == 0) {
            /* 设置本端对应ipipe的接收flag或者直接打开remote_en */
            eicc_wakeup_event_notify(pldev, lipipe_id);
        } else {
            eicc_print_crit("relv dev and chn get failed\n");
        }
    } else {
        eicc_print_crit("eicc_shadow_ipipe_irq_handler ipipe_id 0x%x no detected \n", ipipe_id);
    }
    return;
}

/* pdev should be a remote eicc device( and must poweon) */
irqreturn_t eicc_remote_handler(int irq_no, void *arg)
{
    u32 int_pipe_status;
    u32 pipe_id;
    struct irq_bundle *bundle = (struct irq_bundle *)arg;
    /* 此函数直接访问非本子系统的外设，必须由流程保证远端外设在此访问过程不会下电 */
    struct eicc_device *pdev = bundle->pdev;

    int_pipe_status = readl((void *)((char *)pdev->base_va + EICC_CORE_IPIPE_INT_STAT0(bundle->cpu_id)));
    if (int_pipe_status) {
        pipe_id = eicc_ffs(int_pipe_status);
        eicc_shadow_ipipe_irq_handler(pdev, pipe_id - 1);
        return IRQ_HANDLED;
    }

    int_pipe_status = readl((void *)((char *)pdev->base_va + EICC_CORE_IPIPE_INT_STAT1(bundle->cpu_id)));
    if (int_pipe_status) {
        pipe_id = eicc_ffs(int_pipe_status);
        eicc_shadow_ipipe_irq_handler(pdev, pipe_id + EICC_PIPE_CNT_PER_REG - 1);
        return IRQ_HANDLED;
    }

    eicc_print_crit("eicc_remote_handler no handled \n");
    return IRQ_NONE;
}

int eicc_irqs_local_init(struct eicc_device *pdev)
{
    u32 idx;
    u32 uret;
    int ret = 0;
    struct irq_bundle *bundle = NULL;
    for (idx = 0; idx < sizeof(pdev->maps) / sizeof(pdev->maps[0]); idx++) {
        bundle = pdev->maps[idx];
        if (bundle == NULL) {
            continue;
        }
        bundle->pdev = pdev;
        uret = 0;
        uret |= (u32)request_irq(bundle->irq[0x0], eicc_localsend_handler, 0, "eicc_irq_lsend", (void *)bundle);
        uret |= (u32)request_irq(bundle->irq[0x1], eicc_localrecv_handler, 0, "eicc_irq_lrecv", (void *)bundle);
        if (uret) {
            ret = -1;
            eicc_print_error("eicc_irq[%d]or[%d] failed!\n", bundle->irq[0x0], bundle->irq[0x1]);
        }
    }
    return ret;
}
int eicc_irqs_remote_init(struct eicc_device *pdev)
{
    u32 idx;
    u32 uret;
    int ret = 0;
    struct irq_bundle *bundle = NULL;
    for (idx = 0; idx < sizeof(pdev->maps) / sizeof(pdev->maps[0]); idx++) {
        bundle = pdev->maps[idx];
        if (bundle == NULL) {
            continue;
        }
        bundle->pdev = pdev;
        uret = 0;
        uret |= (u32)request_irq(bundle->irq[0x0], eicc_remote_handler, 0, "eicc_irq_remote", (void *)bundle);
        if (uret) {
            ret = -1;
            eicc_print_error("eicc_irq[%d] failed!\n", bundle->irq[0x0]);
        }
    }
    return ret;
}

int _eicc_irqs_init(struct eicc_device *pdev)
{
    int ret = 0;
    if (pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_HOST || pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_GUEST) {
        ret = eicc_irqs_local_init(pdev);
    } else if (pdev->ctrl_level == EICC_DEV_CONTROL_LEVEL_IRQCLR) {
        ret = eicc_irqs_remote_init(pdev);
    } else {
        ; /* nothing need to do */
    }
    if (ret) {
        return -1;
    }
    return 0;
}
int eicc_irqs_init(void)
{
    int ret;
    u32 idx = 0;
    struct eicc_device *pdev = NULL;
    struct eicc_device *prdev = NULL;
    struct eicc_channel *pchannel = NULL;
    u32 cpu_id;

    /* get every channel, do check init pipe mask */
    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        cpu_id = EICC_GET_CORE_ID(pchannel->ldrvchn_id);
        if (pdev->maps[cpu_id] == NULL) {
            eicc_print_error("eicc_irqs_init failed!\n");
            return -1;
        }
        if (pchannel->type == EICC_CHN_TYPE_MSG_OUTRECV) {
            prdev = eicc_device_get_fast(EICC_GET_DEV_ID(pchannel->rdrvchn_id));
            if (prdev == NULL) {
                return -1;
            }
            if (prdev->maps[EICC_GET_RCORE_ID(pchannel->rdrvchn_id)] == NULL) {
                eicc_print_error("eicc_irqs_init failed!\n");
                return -1;
            }
        }
    }
    for (idx = 0; idx < EICC_DEVICE_NUM_MAX; idx++) {
        pdev = eicc_device_get_fast(idx);
        if (pdev == NULL) {
            continue;
        }
        ret = _eicc_irqs_init(pdev);
        if (ret) {
            return ret;
        }
    }

    return 0;
}
