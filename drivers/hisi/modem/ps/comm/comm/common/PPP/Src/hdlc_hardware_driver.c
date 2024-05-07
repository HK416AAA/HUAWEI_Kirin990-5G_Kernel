/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#include "hdlc_hardware_driver.h"
#include "product_config.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_misc.h"
#include "ttf_irq_interface_fusion.h"
#else
#include "mdrv.h"
#include "ttf_irq_interface.h"
#endif
#include "ttf_util.h"
#include "hdlc_hardware_conf.h"
#include "ttf_comm.h"
#include "gucttf_tag.h"
#include "securec.h"
#include "hdlc_os_adaptor_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_HDLC_HARDWARE_DRIVER_C
#define THIS_MODU mod_ppp

#if (FEATURE_PPP_HDLC_HARDWARE == FEATURE_ON)

/* HDLC配置相关信息 */
STATIC PPP_DRIVER_HdlcHardConfigInfo g_hdlcConfigInfo;

/* 按位读32位寄存器 */
STATIC VOS_UINT32 PPP_HDLC_HardRead32RegByBit(VOS_UINT_PTR regAddr, VOS_UINT8 startBit, VOS_UINT8 endBit)
{
    VOS_UINT32 orgValue;

    orgValue = PPP_HDLC_READ_32REG(regAddr);

    return TTF_GetByBit(PS_PID_PPP_HDLC, orgValue, startBit, endBit);
}

/* HDLC封装中断完成通知服务程序 */
TTF_MDRV_INT_CB_DECLARE(PPP_DriverHdlcHardFrmIsr)
{
    PPP_DRIVER_HdlcHardConfigInfo *conf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    g_hdlcRegSaveInfo.hdlcFrmRawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));
    g_hdlcRegSaveInfo.hdlcFrmStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR));

    /* 收到一次中断后清除原始中断 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* 释放封装完成信号量 */
    /*lint -e(455) VOS_SmV语句屏蔽, 正常机制 */
    VOS_SmV(conf->hdlcFrmSem);

    g_pppHdlcHardStat.frmIsrCnt++;

    return TTF_MDRV_INT_HANDLED;
}

/* HDLC解封装中断完成通知服务程序 */
TTF_MDRV_INT_CB_DECLARE(PPP_DriverHdlcHardDefIsr)
{
    PPP_DRIVER_HdlcHardConfigInfo *conf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    g_hdlcRegSaveInfo.hdlcDefRawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));
    g_hdlcRegSaveInfo.hdlcDefStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));

    /* 收到一次中断后清除原始中断 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* 释放封装完成信号量 */
    /*lint -e(455) VOS_SmV语句屏蔽, 正常机制 */
    VOS_SmV(conf->hdlcDefSem);

    g_pppHdlcHardStat.defIsrCnt++;

    return TTF_MDRV_INT_HANDLED;
}

/*
 * 系统控制器地址定义，HDLC的时钟操作和工作状态查询工作会使用该寄存器
 * HDLC时钟寄存器定义在SYSCTRL CRG寄存器描述.xls，工作状态寄存器定义在SYSCTRL Modem寄存器描述.xls
 * 对于HDLC部署在A核的场景，寄存器分别定义在PERI CRG寄存器描述.xls 和 PERI SYSCTRL寄存器描述.xls
 */
#ifdef SOC_HDLC_GET_SYSCTRL_ADDR

/* 获取系统控制器基地址 */
#define PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD (g_hdlcScCtrlBaseAddr)

VOS_UINT_PTR g_hdlcScCtrlBaseAddr = VOS_NULL_PTR;

/* 初始化Hdlcsysctrl寄存器地址 */
VOS_VOID PPP_DriverHdlcScCtrlBaseAddrInit(VOS_VOID)
{
    VOS_UINT_PTR baseAddr = (VOS_UINT_PTR)SOC_HDLC_GET_SYSCTRL_ADDR();
    g_hdlcScCtrlBaseAddr  = PPP_HDLC_HARD_IO_ADDRESS(baseAddr);
}
#endif

/* 等待器件工作完成 */
STATIC VOS_VOID PPP_DriverHdlcHardWaitStateIdle(VOS_VOID)
{
#ifdef SOC_HDLC_SYSCTRL_STAT_OFFSET
    VOS_UINT32   checkLoop = 0;
    VOS_UINT32   bitPos    = (1 << SOC_HDLC_SYSCTRL_STAT_BITPOS);
    VOS_UINT_PTR addr      = HDLC_SYSCTRL_STAT_ADDR(PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD);

    for (checkLoop = 0; checkLoop < HDLC_CHECK_HARDWAWRE_STATE_MAX_TIMES; checkLoop++) {
        if ((PPP_HDLC_READ_32REG(addr) & bitPos) == 0) {
            return;
        }
    }

    PPP_HDLC_ERROR_LOG("HDLC Work State Error");
#endif
    return;
}

/*
 * LOW LEVEL CLK优化，底层时钟接口由mdrv封装，
 * mdrv提供 mdrv_acc_clk_enable/mdrv_acc_clk_disable/mdrv_acc_clk_is_enabled接口
 *
 * B5000因为MDRV未适配，暂时使用老接口实现，待MDRV实现后删除。
 */
#if (defined(SOC_HDLC_CRG_CLKEN_OFFSET) && (VOS_OS_VER != VOS_WIN32))

/* 时钟使能寄存器 */
#define HDLC_CRG_CLKEN4_ADDR(base) ((base) + (SOC_HDLC_CRG_CLKEN_OFFSET))
/* 时钟关闭寄存器 */
#define HDLC_CRG_CLKENDIS4_ADDR(base) ((base) + (SOC_HDLC_CRG_CLKDIS_OFFSET))
/* 时钟状态寄存器 */
#define HDLC_CRG_CLKSTA4_ADDR(base) ((base) + (SOC_HDLC_CRG_CLKSTAT_OFFSET))

/* 查询HDLC时钟 */
STATIC VOS_BOOL PPP_DriverHdlcHardGetPeriphClkStatus(VOS_VOID)
{
    VOS_UINT32 value = 0;

    value = PPP_HDLC_READ_32REG(HDLC_CRG_CLKSTA4_ADDR(PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD));
    value &= (1 << SOC_HDLC_CRG_CLK_BITPOS);

    /* 结果为零说明当前时钟未开启 */
    if (value == 0) {
        return VOS_FALSE;
    } else {
        return VOS_TRUE;
    }
}

/* 打开HDLC时钟 */
VOS_VOID PPP_DriverHdlcHardPeriphClkOpen(VOS_VOID)
{
    VOS_UINT32 value = 0;

    value |= (1 << SOC_HDLC_CRG_CLK_BITPOS);
    PPP_HDLC_WRITE_32REG(HDLC_CRG_CLKEN4_ADDR(PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD), value);

    return;
}

/* 关闭HDLC时钟 */
VOS_VOID PPP_DriverHdlcHardPeriphClkClose(VOS_VOID)
{
    VOS_UINT32 value = (1 << SOC_HDLC_CRG_CLK_BITPOS);

    PPP_DriverHdlcHardWaitStateIdle();

    PPP_HDLC_WRITE_32REG(HDLC_CRG_CLKENDIS4_ADDR(PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD), value);

    return;
}

#else

/* 查询HDLC时钟状态 */
STATIC VOS_BOOL PPP_DriverHdlcHardGetPeriphClkStatus(VOS_VOID)
{
    return (VOS_BOOL)(1 == mdrv_acc_clk_is_enabled(CLK_HDLC) ? VOS_TRUE : VOS_FALSE);
}

/* 打开HDLC时钟 */
MODULE_EXPORTED VOS_VOID PPP_DriverHdlcHardPeriphClkOpen(VOS_VOID)
{
    mdrv_acc_clk_enable(CLK_HDLC);
}

/* 关闭HDLC时钟 */
MODULE_EXPORTED VOS_VOID PPP_DriverHdlcHardPeriphClkClose(VOS_VOID)
{
    PPP_DriverHdlcHardWaitStateIdle();

    mdrv_acc_clk_disable(CLK_HDLC);
}
#endif

#if (VOS_OSA_CPU == OSA_CPU_ACPU) 
typedef mdrv_int_return_e (*PPP_HdlcIrqFunc)(int para, void *ptr);
#else
typedef void (*PPP_HdlcIrqFunc)(TtfHwiArg);
#endif

STATIC VOS_UINT32 PPP_DriverRegIntFunc(VOS_INT32 irqNum, PPP_HdlcIrqFunc fun)
{
    /* 中断挂接 */
    if (TTF_MDRV_INT_CONNECT(irqNum, fun, 0) != VOS_OK) {
        PPP_HDLC_ERROR_LOG1("PPP_HDLC_HardInit, ERROR, connect <1> failed!\r\n", irqNum);
        return VOS_ERR;
    }

#if (VOS_OSA_CPU == OSA_CPU_CCPU)
    if (TTF_MDRV_INT_CORE_BIND((VOS_UINT32)irqNum, VOS_CORE_MASK_CORE0) != VOS_OK) {
        PPP_HDLC_ERROR_LOG1("PPP_HDLC_HardInit, TTF_MDRV_INT_CORE_BIND fail! irqNum: <1>\n", irqNum);
        return VOS_ERR;
    }
#endif

    /* 中断使能 */
    if (TTF_MDRV_INT_ENABLE(irqNum) != VOS_OK) {
        PPP_HDLC_ERROR_LOG1("PPP_HDLC_HardInit, ERROR, Enable slHdlcISRDef <1> failed!\r\n", irqNum);
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 PPP_DriverHdlcHardInit(PPP_DRIVER_HdlcHardFrmConfig *frmConfig, PPP_DRIVER_HdlcHardDefConfig *defConfig)
{
    /* 获取HDLC基地址 */
    VOS_UINT_PTR                   baseAddr = (VOS_UINT_PTR)mdrv_misc_get_ip_baseaddr_byname("hdlc");;
    PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    (VOS_VOID)memset_s(PPP_DRIVER_HDLC_HARD_GET_CONFIG, sizeof(PPP_DRIVER_HdlcHardConfigInfo), 0x00,
        sizeof(PPP_DRIVER_HdlcHardConfigInfo));

    PPP_HDLC_RegVirPhyAddrMap(baseAddr);

    HDLC_IP_BASE_ADDR = PPP_HDLC_HARD_IO_ADDRESS(baseAddr);
    if (VOS_NULL_PTR == HDLC_IP_BASE_ADDR) {
        PPP_HDLC_ERROR_LOG1("HDLC base addr is null", baseAddr);
        return VOS_ERR;
    }

#ifdef SOC_HDLC_GET_SYSCTRL_ADDR
    PPP_DriverHdlcScCtrlBaseAddrInit();
#endif

#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    /* Boston版本后面配置最大封装寄存器时有可能由于与开时钟指令未执行完毕导致配置失败 */
    PPP_DriverHdlcHardPeriphClkOpen();
#else
    /* 关闭HDLC时钟 */
    PPP_DriverHdlcHardPeriphClkClose();
#endif

    /* 获取HDLC解封装中断号 */
    hdlcConf->hdlcIsrDef = mdrv_int_get_num_byname("hdlc_deframer_int");

    /* 获取HDLC封装中断号 */
    hdlcConf->hdlcIsrFrm = mdrv_int_get_num_byname("hdlc_framer_int");

    if (VOS_SmBCreate("HdlcDefSem", 0, VOS_SEMA4_FIFO, (VOS_SEM*)&(hdlcConf->hdlcDefSem)) != VOS_OK) {
        PPP_HDLC_ERROR_LOG("PPP_HDLC_HardInit, ERROR, Create hdlcDefSem failed!\r\n");
        return VOS_ERR;
    }

    if (VOS_SmBCreate("HdlcFrmSem", 0, VOS_SEMA4_FIFO, (VOS_SEM*)&(hdlcConf->hdlcFrmSem)) != VOS_OK) {
        PPP_HDLC_ERROR_LOG("PPP_HDLC_HardInit, ERROR, Create hdlcFrmSem failed!\r\n");
        return VOS_ERR;
    }

    if (PPP_DriverRegIntFunc(hdlcConf->hdlcIsrDef, PPP_DriverHdlcHardDefIsr) != VOS_OK) {
        return VOS_ERR;
    }

    if (PPP_DriverRegIntFunc(hdlcConf->hdlcIsrFrm, PPP_DriverHdlcHardFrmIsr) != VOS_OK) {
        return VOS_ERR;
    }

    hdlcConf->hdlcDefIntLimit = HDLC_DEF_INTERRUPT_LIMIT_DEFAULT;
    hdlcConf->hdlcFrmIntLimit = HDLC_FRM_INTERRUPT_LIMIT_DEFAULT;

    (VOS_VOID)memcpy_s(&(hdlcConf->frmConfig), sizeof(hdlcConf->frmConfig),
        frmConfig, sizeof(PPP_DRIVER_HdlcHardFrmConfig));
    (VOS_VOID)memcpy_s(&(hdlcConf->defConfig), sizeof(hdlcConf->defConfig),
        defConfig, sizeof(PPP_DRIVER_HdlcHardDefConfig));

#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    if (PPP_DriverHdlcHardGetPeriphClkStatus() == VOS_TRUE) {
        /* Boston版本中配置封装信息域最大长度寄存器0x5DF(1503)Bytes */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
    }

    PPP_DriverHdlcHardPeriphClkClose();
#endif

    return VOS_OK;
} /* link_HDLCInit */

/* 获取HDLC工作状态 */
VOS_VOID PPP_DriverHdlcHardWorkStatus(VOS_BOOL *frmWork, VOS_BOOL *defWork)
{
    VOS_UINT32 frmValue;
    VOS_UINT32 defValue;

    /* SoC会在处理完一套输入链表的时候自动将使能位清零 */
    frmValue = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_EN_ADDR(HDLC_IP_BASE_ADDR));
    defValue = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_EN_ADDR(HDLC_IP_BASE_ADDR));

    if (0x01 == (frmValue & 0x01)) {
        *frmWork = VOS_TRUE;
    } else {
        *frmWork = VOS_FALSE;
    }

    if (0x01 == (defValue & 0x01)) {
        *defWork = VOS_TRUE;
    } else {
        *defWork = VOS_FALSE;
    }

    return;
}

#ifdef CONFIG_UART_SHELL
/* 设置解封装中断水线 */
MODULE_EXPORTED VOS_UINT32 PPP_DriverHdlcHardSetDefIntLimit(VOS_UINT32 intLimit)
{
    PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    hdlcConf->hdlcDefIntLimit = intLimit;

    return hdlcConf->hdlcDefIntLimit;
}

/* 设置封装中断水线 */
MODULE_EXPORTED VOS_UINT32 PPP_DriverHdlcHradSetFrmIntLimit(VOS_UINT32 intLimit)
{
    PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    hdlcConf->hdlcFrmIntLimit = intLimit;

    return hdlcConf->hdlcFrmIntLimit;
}
#endif

/* 配置通用寄存器 */
STATIC VOS_VOID PPP_DriverHdlcHardCommCfgReg(VOS_VOID)
{
    /*
     * prior_timeout_ctrl(0x04)
     *  31  24   23                16   15  9         8             7   2    1          0
     * |--------|----------------------|-------|-------------------|-------|---------------|
     * |   Rsv  | axireq_timeout_value |  Rsv  | axireq_timeout_en |  Rsv  |hdlc_prior_ctrl|
     *
     * Reserved             [31:24] 8'b0   h/s R/W  保留位。读时返回0。写时无影响。
     * axireq_timeout_value [23:16] 8'b0   h/s R/W  软件配置AXI总线读写请求超时的判断值
     * Reserved             [15:9]  2'b0   h/s R/W  保留位。读时返回0。写时无影响。
     * axireq_timeout_en    [8]     1'b0   h/s R/W  是否允许硬件判断AXI总线读写请求超时，由软件配置：
     *                                               0不允许
     *                                               1允许
     * Reserved             [7:2]   1'b0   h/s R/W  保留位。读时返回0。写时无影响。
     * hdlc_prior_ctrl      [1:0]   1'b0   h/s R/W  HDLC封装、解封装优先级配置寄存器：
     *                                                00：一个在工作中，另一个也被使能情况下，先将已处于工作中的那个处理完，
     *                                                    即谁先被使能则先将谁处理完；
     *                                                01：下行封装优先级高；
     *                                                10：上行解封装优先级高；
     *                                                11：无效。
     *                                                (HDLC内部控制被暂停者的继续开始工作：当解封装被暂停，封装整套链表被处理完后，
     *                                                解封装就开始继续工作；当封装被暂停，解封装整个数据包被处理完后，封装就开始继续工作。)
     */

    VOS_UINT32 value = 0x0;

    /* 设置AXI请求超时时长，该值由SoC提供，并且保证无平台差异 */
    value |= (HDLC_AXI_REQ_TIMEOUT_VALUE << 16);

    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_PRIROR_TIMEOUT_CTRL_ADDR(HDLC_IP_BASE_ADDR), value);

    return;
}

/* 等待封装解封装暂停或完成 */
/*lint -e{454} VOS_SmP语句屏蔽, 正常机制 */
STATIC VOS_UINT32 PPP_DriverHdlcHardCommWaitSem(VOS_SEM hdlcSem, VOS_UINT32 semTimeoutLen)
{
    VOS_UINT32 result;

    /* 等待封装或解封装完成 */
    result = VOS_SmP(hdlcSem, semTimeoutLen);

    if (result != VOS_OK) {
        PPP_HDLC_WARNING_LOG2(
            "PPP_HDLC_HARD_CommWaitSem, WARNING, VOS_SmP hdlcSem <1> failed! ErrorNo = <2>\r\n",
            hdlcSem, result);

        g_pppHdlcHardStat.defExpInfo |= (1 << HDLC_SEM_TIMEOUT_IND_BITPOS);
        g_pppHdlcHardStat.frmExpInfo |= (1 << HDLC_SEM_TIMEOUT_IND_BITPOS);

        return VOS_ERR;
    }

    return VOS_OK;
}

/* 配置封装使用的内存至相关寄存器 */
STATIC VOS_VOID PPP_DriverHdlcHardFrmCfgBufReg(VOS_VOID)
{
    PPP_DRIVER_HdlcHardFrmConfig *frmConf = VOS_NULL_PTR;

    frmConf = PPP_DRIVER_HDLC_HARD_GET_FRM_CONF;

    /* 将封装输入参数链表的起始地址配置给寄存器frm_in_lli_addr */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_IN_LLI_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)frmConf->inputAddr);

    /* 将封装输出参数链表的起始地址配置给寄存器frm_out_lli_addr */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_FRM_OUT_LLI_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)frmConf->outputAddr);

    /* 将封装有效帧结果信息上报空间起始地址配置给寄存器frm_rpt_addr */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_RPT_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)frmConf->reportAddr);

    /* 将封装有效帧结果信息上报空间深度配置给寄存器frm_rpt_dep的[15:0]位 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(frmConf->rptBufLen & 0xFFFF));

#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    /* 配置封装信息域最大长度寄存器0x5DF(1503)Bytes */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
#endif

    return;
}

/* IP模式下配置封装相关寄存器 */
VOS_VOID PPP_DriverHdlcHardFrmCfgReg(PPP_DRIVER_HdlcHardFrmPara *drvFrmPara)
{
    VOS_UINT32 frmCfg = 0;

    /*
     * hdlc_frm_cfg   (0x20)
     *  31              16 15           4 3      2   1      0
     * |-------------------|--------------|--------|------|------|
     * |      protocol     |      Rsv     |   Pfc  | Acfc |1dor2d|
     *
     * frm_protocol         [31:16] 16'b0   h/s R/W  封装协议值。有效协议值规定参见规格列表。
     * Reserved             [15:4]  12'b0   h/s R/W  保留位。读时返回0。写时无影响。
     * frm_pfc              [3:2]   2'b0    h/s R/W  P域压缩指示：00：硬件模块添加P域，P域无压缩;
     *                                                            01：硬件模块添加P域，P域压缩;
     *                                                            11：硬件模块不添加P域;
     *                                                            其他：无效;
     * frm_acfc             [1]     1'b0    h/s R/W  AC域压缩指示：0：AC域无压缩;1：表示AC域压缩;
     * frm_in_lli_1dor2d    [0]     1'b0    h/s R/W  封装输入一维或二维链表选择指示寄存器：
     *                                                             0为一维;1为二维;
     */

    frmCfg |= (drvFrmPara->pppPcFlag << HDLC_FRM_PFC_BITPOS);
    if (drvFrmPara->pppAcFlag == HDLC_ADDRESS_CTRL_COMPRESS) {
        TTF_SET_A_BIT(frmCfg, HDLC_FRM_ACFC_BITPOS);
    }
    frmCfg |= (((VOS_UINT32)drvFrmPara->protocol) << 16);

    /* 将寄存器hdlc_frm_cfg的[0]位frm_in_lli_1dor2d配置为0 */

    /* 配置hdlc_frm_cfg的 P域和 AC域 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_ACCM_ADDR(HDLC_IP_BASE_ADDR), drvFrmPara->accmFlag);

    /* 配置寄存器hdlc_frm_cfg的[31:16]位frm_protocol为usProtocol */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_CFG_ADDR(HDLC_IP_BASE_ADDR), frmCfg);

    return;
}

/*
 * 勾取寄存器配置数据
 * 输入参数:ulEnable-封装解封装使能前还是使能后，VOS_TRUE:使能后,VOS_FALSE:使能前
 * ulValue-使能寄存器配置的值
 * ulEnableInterrupt-中断方式还是轮询方式，仅在使能后勾取寄存器时生效
 */
VOS_VOID PPP_DriverHdlcHardTraceFrmRegConfig(
    VOS_UINT32 enable, VOS_UINT32 value, VOS_UINT32 enableInterrupt, PPP_DRIVER_HdlchardFrmRegInfo *frmRegInfo)
{
    HDLC_MNTN_FrmRegConfig         regMntn;
    PPP_DRIVER_HdlchardFrmRegInfo *regConfig = &(regMntn.frmRegInfo);
    VOS_UINT32                     dataLen;

    /* 内部调用保证参数非空 */

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_REG) == VOS_TRUE) {
        dataLen = sizeof(HDLC_MNTN_FrmRegConfig);

        /* 拷贝全部寄存器内容 */
        (VOS_VOID)memcpy_s(
            regConfig, sizeof(PPP_DRIVER_HdlchardFrmRegInfo), frmRegInfo, sizeof(PPP_DRIVER_HdlchardFrmRegInfo));

        /* 使能前勾包，使能寄存器还没有配置，因为配置之后HDLC会开始工作，会改变其他寄存器的值 */
        if (enable == VOS_FALSE) {
            regConfig->hdlcFrmEn = value;
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_FRM_REG_BEFORE_EN, dataLen);
        } else {
            /* 使能后勾包时，如果采用中断方式，则RawInt和Status取g_hdlcRegSaveInfo保存的值 */
            if (enableInterrupt == VOS_TRUE) {
                regConfig->hdlcFrmRawInt = g_hdlcRegSaveInfo.hdlcFrmRawInt;
                regConfig->hdlcFrmStatus = g_hdlcRegSaveInfo.hdlcFrmStatus;
            }
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_FRM_REG_AFTER_EN, dataLen);
        }
    }

    return;
}

/* 配置使能寄存器 */
STATIC VOS_UINT32 PPP_DriverHdlcHardFrmCfgEnReg(VOS_UINT32 totalLen)
{
    PPP_DRIVER_HdlchardFrmRegInfo regConfig;

    /*
     * 1.hdlc_frm_en   (0x10)
     * 31   25 24  23 18 17  16  15  14  13  12  11  10   9   8  7    1  0
     * |--------|---|-----|---|---|---|---|---|---|---|---|---|---|------|---|
     * |   Rsv  |en | Rsv |en |en |en |en |en |en |en |en |en |en |  Rsv |en |
     *
     * Reserved            [31:25] 7'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * frm_over_int_en     [24]    1'b0    h/s R/W  一套链表封装结束中断使能;0：中断禁止;1：中断使能;
     * Reserved            [23:18] 6'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * frm_rpt_dep_err_en  [17]    1'b0    h/s R/W  封装外部正确帧长度上报空间不足中断使能;0：中断禁止;1：中断使能;
     * frm_out_spc_err_en  [16]    1'b0    h/s R/W  封装外部输出存储空间不足中断使能;0：中断禁止;1：中断使能
     * frm_rpt_prm_err_en  [15]    1'b0    h/s R/W  封装上报空间相关参数错误中断使能;0：中断禁止;1：中断使能
     * frm_out_prm_err_en  [14]    1'b0    h/s R/W  封装输出链表相关参数错误中断使能;0：中断禁止;1：中断使能
     * frm_in_prm_err_en   [13]    1'b0    h/s R/W  封装输入链表相关参数错误中断使能;0：中断禁止;1：中断使能
     * frm_cfg_err_en      [12]    1'b0    h/s R/W  封装协议及其压缩指示配置错误中断使能;0：中断禁止;1：中断使能
     * frm_wr_timeout_en   [11]    1'b0    h/s R/W  封装时AXI总线写请求timeout中断使能;0：中断禁止;1：中断使能
     * frm_rd_timeout_en   [10]    1'b0    h/s R/W  封装时AXI总线读请求timeout中断使能;0：中断禁止;1：中断使能
     * frm_wr_err_en       [9]     1'b0    h/s R/W  封装时AXI总线写操作错误中断使能;0：中断禁止;1：中断使能
     * frm_rd_err_en       [8]     1'b0    h/s R/W  封装时AXI总线读操作错误中断使能;0：中断禁止;1：中断使能
     * Reserved            [7:1]   7'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * frm_en              [0]     1'b0    h/s R/W  一套链表封装使能，软件向frm_en写入1'b1启动封装工作;一套链表封装完成后，由硬件自动对frm_en清零；
     *                                             封装过程出错时，硬件也会对frm_en自动清零，使内部状态机返回IDLE状态；
     *                                             写时设置一套链表封装使能;0：不使能封装处理;1：使能封装处理;
     *                                             读时返回一套链表封装处理状态;0：没在进行封装处理;1：正在进行封装处理。
     */

    VOS_UINT32       enableInterrupt;
    VOS_UINT32       value;
    const VOS_UINT32 interruptValue = 0x0103FF01; /* 使用中断方式时配置使能寄存器的值 */
    const VOS_UINT32 pollValue      = 0x01;       /* 使用轮询方式时配置使能寄存器的值 */

    /* 判断待封装数据的总长度，若大于门限则使用中断方式，否则使用轮询方式 */
    if (totalLen > HDLC_FRM_INTERRUPT_LIMIT) {
        /* 配置封装相关使能寄存器hdlc_frm_en的[31:0]位为0x0103FF01 */
        value           = interruptValue;
        enableInterrupt = VOS_TRUE;

        g_pppHdlcHardStat.frmWaitIntCnt++;
    } else {
        /* 配置封装相关使能寄存器hdlc_frm_en的[31:0]位为0x01 */
        value           = pollValue;
        enableInterrupt = VOS_FALSE;

        g_pppHdlcHardStat.frmWaitQueryCnt++;
    }

    /* 使能前清除上次封装、解封装的原始中断 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* 上报寄存器可维可测 */
    PPP_DriverHdlcHardGetFrmRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceFrmRegConfig(VOS_FALSE, value, enableInterrupt, &regConfig);

    /* 使能硬件之前先强制ARM顺序执行结束前面的指针 */
    TTF_FORCE_ARM_INSTUCTION();

    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_EN_ADDR(HDLC_IP_BASE_ADDR), value);

#if (VOS_OS_VER == VOS_WIN32)
    VHW_HDLC_Task();
#endif

    return enableInterrupt;
}

/* 轮询状态寄存器 */
STATIC VOS_UINT32 PPP_DriverHdlcHardFrmWaitStatusChange(VOS_VOID)
{
    VOS_UINT32          frmRsltWaitNum; /* 防止硬件异常的保护变量 */
    volatile VOS_UINT32 frmStatus = 0;  /* 封装状态 */

    /* 查询hdlc_frm_status (0x28)的第[0]位和第[1]位，任何一个为1或者超时则返回 */
    frmRsltWaitNum = 0UL;

    while (frmRsltWaitNum < HDLC_FRM_MAX_WAIT_RESULT_NUM) {
        /* 读取 hdlc_frm_status的[0][1]位 */
        frmStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR));

        if ((frmStatus & HDLC_FRM_STATUS_MASK) != HDLC_FRM_ALL_PKT_DOING) {
            break;
        }

        frmRsltWaitNum++;
    }

    if (frmRsltWaitNum >= HDLC_FRM_MAX_WAIT_RESULT_NUM) {
        PPP_HDLC_WARNING_LOG2(
            "PPP_HDLC_HARD_FrmWaitStatusChange, WARNING, wait hdlc_frm_status timeout <1> status <2>!\r\n",
            frmRsltWaitNum, frmStatus);

        g_pppHdlcHardStat.frmExpInfo |= (1 << HDLC_WAIT_STATUS_TIMEOUT_IND_BITPOS);

        return VOS_ERR;
    }

    g_pppHdlcHardStat.frmMaxQueryCnt = TTF_MAX(g_pppHdlcHardStat.frmMaxQueryCnt, frmRsltWaitNum);

    return VOS_OK;
}

/* 等待封装暂停或完成 */
STATIC VOS_UINT32 PPP_DriverHdlcHardFrmWaitResult(VOS_UINT32 enableInterrupt)
{
    VOS_UINT32                     frmStatus; /* 封装状态 */
    VOS_UINT32                     result;
    PPP_DRIVER_HdlchardFrmRegInfo  regConfig;
    const PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    if (enableInterrupt == VOS_TRUE) {
        /* 等待中断得到出错或完成状态 */
        result = PPP_DriverHdlcHardCommWaitSem(hdlcConf->hdlcFrmSem, HDLC_FRM_INT_TIMER_LEN);

        /* 由于在中断服务程序中进行了清中断操作，而Status指示是否出错的bit由原始中断寄存器决定，故此处取保存在g_hdlcRegSaveInfo中的状态值 */
        frmStatus = g_hdlcRegSaveInfo.hdlcFrmStatus;
    } else {
        /* 轮询得到出错或完成 */
        result = PPP_DriverHdlcHardFrmWaitStatusChange();

        /* 查询hdlc_frm_status (0x28)获取封装状态并将其返回 */
        frmStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    }

    /* 上报寄存器可维可测 */
    PPP_DriverHdlcHardGetFrmRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceFrmRegConfig(VOS_TRUE, 0, enableInterrupt, &regConfig);

    /* 等不到说明HDLC还在工作 */
    if (result != VOS_OK) {
        return HDLC_FRM_ALL_PKT_DOING;
    }

    frmStatus &= HDLC_FRM_STATUS_MASK;

    return frmStatus;
}

/* 配置封装寄存器并使能封装功能 */
VOS_UINT32 PPP_DriverHdlcHardFrmEnable(PPP_DRIVER_HdlcHardFrmPara *drvFrmPara, VOS_UINT32 *enableInterrupt)
{
    VOS_UINT32 frmStatus;
    VOS_UINT32 enableFlag;

    /* 配置封装、解封装通用寄存器 */
    PPP_DriverHdlcHardCommCfgReg();

    /* 配置内存相关寄存器 */
    PPP_DriverHdlcHardFrmCfgBufReg();

    /* 配置封装相关寄存器 */
    PPP_DriverHdlcHardFrmCfgReg(drvFrmPara);

    /* 配置使能寄存器，并上报使能前寄存器可维可测 */
    enableFlag = PPP_DriverHdlcHardFrmCfgEnReg(drvFrmPara->inputTotalSize);

    *enableInterrupt = enableFlag;

    frmStatus = PPP_DriverHdlcHardFrmWaitResult(enableFlag);

    return frmStatus;
}

/* 获取封装结果 */
VOS_VOID PPP_DriverHdlcHardGetFrmResult(VOS_UINT16 *frmOutSegNum, VOS_UINT8 *frmValidNum)
{
    /* 有效帧存放占用片段个数usFrmOutSegNum= hdlc_frm_status寄存器[31:16]位的值 */
    *frmOutSegNum = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR), 16, 31);

    /* 有效帧个数usFrmValidNum= hdlc_frm_status寄存器[15:8]位的值 */
    *frmValidNum = (VOS_UINT8)PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR), 8, 15);

    return;
}

/* 获取封装中断寄存器值 */
VOS_UINT32 PPP_DriverHdlcHardGetFrmRawInt(VOS_VOID)
{
    VOS_UINT32 rawInt;

    rawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));

    return rawInt;
}

/* 配置解封装使用的内存至相关寄存器 */
STATIC VOS_VOID PPP_DriverHdlcHardDefCfgBufReg(VOS_VOID)
{
    PPP_DRIVER_HdlcHardDefConfig *defConf = VOS_NULL_PTR;

    defConf = PPP_DRIVER_HDLC_HARD_GET_DEF_CONF;

    /* 将封装输入参数链表的起始地址配置给寄存器def_in_lli_addr(0x90) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_IN_LLI_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)defConf->inputAddr);

    /* 将封装输出参数链表的起始地址配置给寄存器def_out_spc_addr(0xA0) */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_DEF_OUT_SPC_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)defConf->outputAddr);

    /* 将封装输出参数链表的起始地址配置给寄存器def_out_space_dep(0xA4)低16位 */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_DEF_OUT_SPACE_DEP_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(defConf->outputBufLen & 0xFFFF));

#ifndef PPPC_HDLC_NOC_ST_TEST
    /* 将封装有效帧结果信息上报空间起始地址配置给寄存器def_rpt_addr(0xA8) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_RPT_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)defConf->reportAddr);
#else
    /* 用于NOC测试,将CDSP的Resv空间0xE39D9000配置给HDLC */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_RPT_ADDR(HDLC_IP_BASE_ADDR), 0xE39D9000);
#endif

    /* 将封装有效帧结果信息上报空间深度配置给寄存器def_rpt_dep (0xAC)低16位 */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_DEF_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(defConf->reportBufLen & 0xFFFF));

#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    /* 配置封装信息域最大长度寄存器0x5DF(1503)Bytes */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
#endif

    return;
}

/* 配置解封装配置寄存器 */
VOS_VOID PPP_DriverHdlcHardDefCfgReg(PPP_DRIVER_HdlcHardDefPara *drvDefPara)
{
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo = VOS_NULL_PTR;
    PPP_DRIVER_HdlcHardDefConfig          *defConf        = VOS_NULL_PTR;
    VOS_UINT32                             defCfg         = 0;

    /*
     * hdlc_def_cfg  (0x70)
     * 31                           4 3     2   1     0
     * |-------------------------------|-------|-----|-----|
     * |              Rsv              |  Pfc  | Acfc| ago |
     * Reserved             [31:4]  28'b0   h/s R/W  保留位。读时返回0。写时无影响。
     * def_pfc              [3:2]   2'b0    h/s R/W  P域压缩指示：00：P域无压缩，需剥离；01：P域压缩，需剥离；11：P域不剥离；其他：无效；
     * def_acfc             [1]     1'b0    h/s R/W  AC域压缩指示：0：AC域无压缩；1：表示AC域压缩；
     * def_uncompleted_ago  [0]     1'b0    h/s R/W  用于指示对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表是否有解出非完整帧，
     *                                               为了支持多个PPP/IP拨号而增加的配置：0，没有；1，有
     */

    defConf = PPP_DRIVER_HDLC_HARD_GET_DEF_CONF;

    defCfg |= drvDefPara->pppPcFlag << HDLC_DEF_PFC_BITPOS;

    if (drvDefPara->pppAcFlag == HDLC_ADDRESS_CTRL_COMPRESS) {
        TTF_SET_A_BIT(defCfg, HDLC_DEF_ACFC_BITPOS);
    }

    /* 设置非完整帧相关信息 */
    uncompleteInfo = drvDefPara->uncompleteInfo;
    if ((uncompleteInfo != VOS_NULL_PTR) && (uncompleteInfo->existFlag == HDLC_DEF_UNCOMPLETED_EXIST)) {
        /*
         * def_uncompleted_len  (0x74)
         *  31                 16  15                  0
         * |---------------------|----------------------|
         * |         Rsv         |         Len          |
         * Reserved             [31:16] 16'b0   h/s R/W  保留位。读时返回0。写时无影响。
         * def_uncompleted_len  [15:0]  16'b0   h/s R/W 对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表解出非完整帧的长度，为了支持多个PPP/IP拨号而增加的配置
         */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_LEN_ADDR(HDLC_IP_BASE_ADDR),
            (VOS_UINT32)uncompleteInfo->defOutOneLen & 0xFFFF);

        /*
         * def_uncompleted_pro  (0x78)
         * 31                 16  15                  0
         * |---------------------|----------------------|
         * |         Rsv         |         Pro          |
         * Reserved             [31:16] 16'b0   h/s R/W  保留位。读时返回0。写时无影响。
         * def_uncompleted_pro  [15:0]  16'b0   h/s R/W  对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表解出非完整帧的
         *                                              协议，为了支持多个PPP/IP拨号而增加的配置（可能其中的0Byte、1Byte或2Byte有效）
         */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_PRO_ADDR(HDLC_IP_BASE_ADDR),
            (VOS_UINT32)uncompleteInfo->defOutOnePro & 0xFFFF);

        /*
         * def_uncompleted_addr  (0x7C)
         * 31                  0
         * |----------------------|
         * |         Addr         |
         * def_uncompleted_addr [31:0]  32'b0   h/s R/W  对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表解出非完整帧的
         *                                              外部存储起始地址，为了支持多个PPP/IP拨号而增加的配置（该地址可能是与原来上报不同的新地址）
         */
        PPP_HDLC_WRITE_32REG(
            SOC_ARM_HDLC_DEF_UNCOMPLETED_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)uncompleteInfo->defOutOneAddr);

        /*
         * def_uncomplet_st_ago  (0x80)
         * 31                  16 15             5 4     0
         * |----------------------|----------------|-------|
         * |         Ago          |       Rsv      |  Ago  |
         * crc16_result_ago     [31:16] 16'b0   h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的CRC校验值
         * Reserved             [15:5]  11'b0   h/s R/W  保留位。读时返回0。写时无影响。
         * def_data_st_curr_ago [4:0]   5'b0    h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的数据状态机当前状态
         */
        PPP_HDLC_WRITE_32REG(
            SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_AGO_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)uncompleteInfo->defStAgo);

        /*
         * def_info_frl_cnt_ago  (0xC0)
         * 31        27 26                 16 15   11 10              0
         * |------------|---------------------|-------|-----------------|
         * |    Rsv     |         Ago         |  Rsv  |       Ago       |
         * Reserved             [31:27] 5'b0    h/s R/W  保留位。读时返回0。写时无影响。
         * def_framel_cnt_ago   [26:16] 11'b0   h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的帧长度
         * Reserved             [15:11] 5'b0    h/s R/W  保留位。读时返回0。写时无影响。
         * def_info_cnt_ago     [10:0]  11'b0   h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的信息长度
         */
        PPP_HDLC_WRITE_32REG(
            SOC_ARM_HDLC_DEF_INFO_FRL_CNT_AGO_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)uncompleteInfo->defInfoFrlCntAgo);

        TTF_SET_A_BIT(defCfg, HDLC_DEF_IS_UNCOMPLETED_AGO_BITPOS);
    }

    /* 根据ulMode、P域和AC域是否压缩配置hdlc_def_cfg (0x70) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_CFG_ADDR(HDLC_IP_BASE_ADDR), defCfg);

    /* 设置输入数据最大单包长度 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_IN_PKT_LEN_MAX_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)defConf->perInMaxLen);

    return;
}

/* 轮询状态寄存器 */
STATIC VOS_UINT32 PPP_DriverHdlcHardDefWaitStatusChange(VOS_VOID)
{
    /*
     * hdlc_def_status  (0x88)
     * 31 30   24 23   8 7 6  5   4   3   2  1  0
     * |---|-------|------|---|---|---|---|---|----|
     * |Rsv|  Type |  Num |Rsv|Idx|Ful|Ful|Now|Stat|
     * Reserved             [31]    1'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * def_err_type         [30:24] 7'b0    h/s RO   有帧上报时，错误帧类型，对应的bit位为1即表明发生该类型错误：
     *                                              bit 30：错误类型6，转义字符0x7D后紧接一个Flag域；
     *                                              bit 29：错误类型5，当AC域无压缩时，Address域值非0xFF；
     *                                              bit 28：错误类型4，当AC域无压缩时，Control域值非0x03；
     *                                              bit 27：错误类型3，当P域需剥离时，收到非法的Protocol域值；
     *                                              bit 26：错误类型2，解封装后帧字节数小于4bites；
     *                                              bit
     * 25：错误类型1，解封装后帧字节数大于1502bytes（PPP帧的Information域不超过1500Bytes，加上协议域不超过1502Bytes）； bit 24：错误类型0， CRC校验错误。
     * def_valid_num        [23:8]  16'b0   h/s RO   有帧上报时，有效帧数目；（不包括最后一个可能的非完整帧） 
     * Reserved             [7:6]   2'b0    h/s R/W  保留位。读时返回0。写时无影响。 
     * def_error_index      [5]     1'b0    h/s RO   解封装发生错误指示
     * def_rpt_ful          [4]     1'b0    h/s RO   解封装外部正确帧信息上报空间存满暂停指示
     * def_out_spc_ful      [3]     1'b0    h/s RO   解封装外部输出存储空间存满暂停指示
     * def_uncompleted_now  [2]     1'b0    h/s RO   用于指示当前链表是否有解出非完整帧，为了支持多个PPP/IP拨号而增加的配置：0，没有；1，有
     * def_all_pkt_pro_stat [1:0]   2'b0    h/s RO   一套输入链表处理状态：00：未完成一套输入链表处理；01：未完成一套输入链表处理，已解出LCP帧，硬件处于暂停状态； 
     *                                               10：完成一套输入链表处理，但无帧上报；11: 完成一套输入链表处理，且有帧上报；
     */
    VOS_UINT32          rsltWaitNum; /* 防止硬件异常的保护变量 */
    volatile VOS_UINT32 status = 0;  /* 解封装状态 */

    /* 查询hdlc_frm_status (0x28)的第[0]位和第[1]位，任何一个为1或者超时则返回 */
    rsltWaitNum = 0UL;

    while (rsltWaitNum < HDLC_DEF_MAX_WAIT_RESULT_NUM) {
        /* 查询状态寄存器hdlc_def_status (0x88)的0-1和3-5位，任何一位变为1表示解封装模块暂停或停止 */
        status = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));

        if ((status & HDLC_DEF_STATUS_MASK) != HDLC_DEF_STATUS_DOING) {
            break;
        }

        rsltWaitNum++;
    }

    if (rsltWaitNum >= HDLC_DEF_MAX_WAIT_RESULT_NUM) {
        PPP_HDLC_WARNING_LOG2(
            "PPP_HDLC_HARD_DefWaitStatusChange, WARNING, wait hdlc_def_status timeout <1> status <2>!\r\n", rsltWaitNum,
            status);

        g_pppHdlcHardStat.defExpInfo |= (1 << HDLC_WAIT_STATUS_TIMEOUT_IND_BITPOS);

        return VOS_ERR;
    }

    g_pppHdlcHardStat.defMaxQueryCnt = TTF_MAX(g_pppHdlcHardStat.defMaxQueryCnt, rsltWaitNum);

    return VOS_OK;
}

/*
 * 勾取解封装寄存器配置数据
 * 输入参数:ulEnable-封装解封装使能前还是使能后，VOS_TRUE:使能后,VOS_FALSE:使能前
 * ulValue-使能寄存器配置的值
 * ulEnableInterrupt-中断方式还是轮询方式，仅在使能后勾取寄存器时生效
 */
VOS_VOID PPP_DriverHdlcHardTraceDefRegconfig(
    VOS_UINT32 enable, VOS_UINT32 value, VOS_UINT32 enableInterrupt, PPP_DRIVER_HdlchardDefRegInfo *defRegInfo)
{
    HDLC_MNTN_DefRegConfig         regMntn;
    PPP_DRIVER_HdlchardDefRegInfo *regConfig = &(regMntn.defRegInfo);
    VOS_UINT32                     dataLen;

    /* 内部调用保证参数非空 */

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_REG) == VOS_TRUE) {
        dataLen = sizeof(HDLC_MNTN_DefRegConfig);

        /* 拷贝全部寄存器内容 */
        (VOS_VOID)memcpy_s(
            regConfig, sizeof(PPP_DRIVER_HdlchardDefRegInfo), defRegInfo, sizeof(PPP_DRIVER_HdlchardDefRegInfo));

        /* 使能前勾包，使能寄存器还没有配置，因为配置之后HDLC会开始工作，会改变其他寄存器的值 */
        if (enable == VOS_FALSE) {
            regConfig->hdlcDefEn = value;
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_DEF_REG_BEFORE_EN, dataLen);
        } else {
            /* 使能后勾包时，如果采用中断方式，则RawInt和Status取g_hdlcRegSaveInfo保存的值 */
            if (enableInterrupt == VOS_TRUE) {
                regConfig->hdlcDefRawInt = g_hdlcRegSaveInfo.hdlcDefRawInt;
                regConfig->hdlcDefStatus = g_hdlcRegSaveInfo.hdlcDefStatus;
            }
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_DEF_REG_AFTER_EN, dataLen);
        }
    }

    return;
}

/* 等待解封装暂停或完成 */
VOS_UINT32 PPP_DriverHdlcHardDefWaitResult(VOS_UINT32 enableInterrupt)
{
    VOS_UINT32                     status; /* 解封装状态 */
    VOS_UINT32                     result;
    PPP_DRIVER_HdlchardDefRegInfo  regConfig;
    const PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    if (enableInterrupt == VOS_TRUE) {
        /* 等待中断得到暂停或完成状态 */
        result = PPP_DriverHdlcHardCommWaitSem(hdlcConf->hdlcDefSem, HDLC_DEF_INT_TIMER_LEN);

        /* 由于在中断服务程序中进行了清中断操作，而Status指示是否出错的bit由原始中断寄存器决定，故此处取保存在g_hdlcRegSaveInfo中的状态值 */
        status = g_hdlcRegSaveInfo.hdlcDefStatus;

    } else {
        /* 轮询得到暂停或完成 */
        result = PPP_DriverHdlcHardDefWaitStatusChange();

        /* 查询hdlc_def_status (0x88)获取解封装状态并将其返回 */
        status = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    }

    /* 上报寄存器可维可测 */
    PPP_DriverHdlcHardGetDefRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceDefRegconfig(VOS_TRUE, 0, enableInterrupt, &regConfig);

    /* 等不到说明HDLC还在工作 */
    if (result != VOS_OK) {
        return HDLC_DEF_STATUS_DOING;
    }

    status &= HDLC_DEF_STATUS_MASK;

    return status;
}

/* 根据本次解封装数据量大小，决定使用何况方式等待HDLC的结果 */
STATIC VOS_UINT32 PPP_DriverHdlcHardDefCfgEnReg(VOS_UINT32 totalLen)
{
    /*
     * hdlc_def_en   (0x60)
     * 31   25 24  23 19 18  17  16  15  14  13  12  11  10   9   8   7   1  0
     * |--------|---|-----|---|---|---|---|---|---|---|---|---|---|---|------|---|
     * |   Rsv  |en | Rsv |en |en |en |en |en |en |en |en |en |en |en |  Rsv |en |
     *
     * Reserved            [31:25] 7'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * def_over_int_en     [24]    1'b0    h/s R/W  一套链表解封装结束中断使能;0：中断禁止;1：中断使能;
     * Reserved            [23:19] 5'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * def_rpt_ful_en      [18]    1'b0    h/s R/W  解封装外部正确帧信息上报空间存满暂停中断使能;0：中断禁止;1：中断使能;
     * def_out_spc_ful_en  [17]    1'b0    h/s R/W  解封装外部输出存储空间存满暂停中断使能;0：中断禁止;1：中断使能
     * def_lcp_int_en      [16]    1'b0    h/s R/W  解封装解出有效LCP帧暂停中断上报使能;0：中断禁止;1：中断使能
     * def_rpt_prm_err_en  [15]    1'b0    h/s R/W  解封装上报空间相关参数错误中断使能;0：中断禁止;1：中断使能
     * def_out_prm_err_en  [14]    1'b0    h/s R/W  解封装输出空间相关参数错误中断使能;0：中断禁止;1：中断使能
     * def_in_prm_err_en   [13]    1'b0    h/s R/W  解封装输入链表相关参数错误中断使能;0：中断禁止;1：中断使能
     * def_cfg_err_en      [12]    1'b0    h/s R/W  解封装协议压缩指示配置错误中断使能;0：中断禁止;1：中断使能
     * def_wr_timeout_en   [11]    1'b0    h/s R/W  解封装时AXI总线写请求timeout中断使能;0：中断禁止;1：中断使能
     * def_rd_timeout _en  [10]    1'b0    h/s R/W  解封装时AXI总线读请求timeout中断使能;0：中断禁止;1：中断使能
     * def_wr_err_en       [9]     1'b0    h/s R/W  解封装时AXI总线写操作错误中断使能;0：中断禁止;1：中断使能
     * def_rd_err_en       [8]     1'b0    h/s R/W  解封装时AXI总线读操作错误中断使能;0：中断禁止;1：中断使能
     * Reserved            [7:1]   7'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * def_en              [0]     1'b0    h/s R/W  一套输入链表解封装使能，软件向def_en写入1'b1启动解封装工作；一套输入链表解封装完成后，由硬件自动对def_en清零；
     *                                             解封装过程出错时，硬件也会对def_en自动清零，使内部状态机返回IDLE状态；读该寄存器返回解封装处理状态。
     *                                             写时设置一套输入链表解封装使能：0：不使能解封装处理；1：使能解封装处理；
     *                                             读时返回一套输入链表解封装处理状态：0：没在进行解封装处理；1：正在进行解封装处理。
     */
    VOS_UINT32                    enableInterrupt;
    VOS_UINT32                    value;
    const VOS_UINT32              interruptValue = 0x0107FF01; /* 使用中断方式时配置使能寄存器的值 */
    const VOS_UINT32              pollValue      = 0x01;       /* 使用轮询方式时配置使能寄存器的值 */
    PPP_DRIVER_HdlchardDefRegInfo regConfig;

    if (totalLen > HDLC_DEF_INTERRUPT_LIMIT) {
        /* 配置封装相关使能寄存器hdlc_def_en的[31:0]位为0x0107FF01 */
        value           = interruptValue;
        enableInterrupt = VOS_TRUE;

        g_pppHdlcHardStat.defWaitIntCnt++;
    } else {
        /* 配置封装相关使能寄存器hdlc_frm_en的[31:0]位为0x01 */
        value           = pollValue;
        enableInterrupt = VOS_FALSE;

        g_pppHdlcHardStat.defWaitQueryCnt++;
    }

    /* 使能前清除上次封装、解封装的原始中断 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* 上报寄存器可维可测 */
    PPP_DriverHdlcHardGetDefRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceDefRegconfig(VOS_FALSE, value, enableInterrupt, &regConfig);

    /* 使能硬件之前先强制ARM顺序执行结束前面的指针 */
    TTF_FORCE_ARM_INSTUCTION();

    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_EN_ADDR(HDLC_IP_BASE_ADDR), value);

#if (VOS_OS_VER == VOS_WIN32)
    VHW_HDLC_Task();
#endif

    return enableInterrupt;
}

/* 配置解封装寄存器并使能解封装功能 */
VOS_UINT32 PPP_DriverHdlcHardDefEnable(PPP_DRIVER_HdlcHardDefPara *drvDefPara, VOS_UINT32 *enableInterrupt)
{
    if ((drvDefPara == VOS_NULL_PTR) || (enableInterrupt == VOS_NULL_PTR)) {
        PPP_HDLC_WARNING_LOG("pstDrvDefPara or pulEnableInterrupt is NULL");
        return VOS_ERR;
    }

    /* 配置封装、解封装通用寄存器 */
    PPP_DriverHdlcHardCommCfgReg();

    /* 配置内存相关寄存器 */
    PPP_DriverHdlcHardDefCfgBufReg();

    /* 配置压缩指示、非完整帧相关信息寄存器 */
    PPP_DriverHdlcHardDefCfgReg(drvDefPara);

    /* 配置使能寄存器 */
    *enableInterrupt = PPP_DriverHdlcHardDefCfgEnReg(drvDefPara->inputTotalSize);

    return VOS_OK;
}

/* 根据HDLC暂停状态，配置GO_ON寄存器对应比特位 */
VOS_VOID PPP_DriverHdlcHardDefCfgGoOnReg(VOS_UINT32 defStatus)
{
    /*
     * hdlc_def_go_on  (0x84)
     * 31                  17  16  15    9   8  7   1  0
     * |----------------------|----|-------|----|-----|----|
     * |         Rsv          |Goon|  Rsv  |Goon| Rsv |Goon|
     * Reserved             [31:17] 15'b0   h/s R/W  保留位。读时返回0。写时无影响。
     * def_rpt_ful_goon     [16]    1'b0    h/s WO   外部解封装有效帧信息上报空间存满暂停解除
     * Reserved             [15:9]  7'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * def_outspc_ful_goon  [8]     1'b0    h/s WO   外部解封装输出数据存储空间存满暂停状态清除
     * Reserved             [7:1]   7'b0    h/s R/W  保留位。读时返回0。写时无影响。
     * def_lcp_goon         [0]     1'b0    h/s WO
     * 解出一个合法LCP帧导致的硬件暂停状态清除。当解封装模块未处理完一组待解封装的数据（<=2KB(def_in_pkt_len_max)），解出一个合法LCP帧，则会暂停解帧，等待此软件向此寄存器写"1"，再继续处理剩余的数据。
     */

    /* GO_ON前清除上次解封装的原始中断 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    if (defStatus == HDLC_DEF_STATUS_PAUSE_RPT_SPACE_FULL) {
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_GO_ON_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)0x10000);
    } else if (defStatus == HDLC_DEF_STATUS_PAUSE_OUTPUT_SPACE_FULL) {
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_GO_ON_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)0x100);
    } else if (defStatus == HDLC_DEF_STATUS_PAUSE_LCP) {
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_GO_ON_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)0x1);
    } else {
        PPP_HDLC_ERROR_LOG1("PPP_HDLC_HARD_DefCfgGoOnReg, ERROR, Wrong ulDefStatus <1>!\r\n", defStatus);
    }

    return;
}

/* 获取解封装有效帧个数 */
VOS_UINT16 PPP_DriverHdlcHardGetDefVaildNum(VOS_VOID)
{
    VOS_UINT16 validFrameNum;

    validFrameNum = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR), 8, 23);

    return validFrameNum;
}

/* 获取解封装非完整帧信息 */
VOS_VOID PPP_DriverHdlcHardGetDefUncompletInfo(
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo, VOS_UINT32 *validNum)
{
    VOS_UINT32 status; /* 解封装状态 */
    VOS_UINT32 validFrameNum;

    status = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));

    /* 查询状态寄存器hdlc_def_status (0x88)的第2位为1表示本次解封装有非完整帧输出，为0表示无非完整帧输出 */
    if ((status & 0x4) == 0) {
        uncompleteInfo->existFlag = HDLC_DEF_UNCOMPLETED_NOT_EXIST;

        return;
    }

    g_pppHdlcHardStat.defUncompleteCnt++;

    /* def_valid_num        [23:8]  16'b0   h/s RO   有帧上报时，有效帧数目；（不包括最后一个可能的非完整帧） */
    validFrameNum = (status & 0xFFFF00) >> 8;

    *validNum = validFrameNum;
    /* 有非完整帧时需要读取并保存def_uncomplet_st_now(0x8C)、def_info_frl_cnt_now(0xC4)读取并保存上报空间有效帧之后的非完整帧长度、协议和数据地址 */
    uncompleteInfo->existFlag = HDLC_DEF_UNCOMPLETED_EXIST;

    /* 软件只是暂存这些信息，等下解封装的时候再原样配给HDLC */
    uncompleteInfo->defStAgo         = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_NOW_ADDR(HDLC_IP_BASE_ADDR));
    uncompleteInfo->defInfoFrlCntAgo = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INFO_FRL_CNT_NOW_ADDR(HDLC_IP_BASE_ADDR));

    return;
}

/* 获取错误帧信息 */
VOS_VOID PPP_DriverHdlcHardGetDefErrorInfo(PPP_DRIVER_HdlcHardDefErrFramesCnt *errCnt)
{
    VOS_UINT8 errType;

    /* 查询状态寄存器hdlc_def_status (0x88)的第24:30对应比特位为1表示有某种错误帧输出，
       为0表示无帧输出 */
    errType = (VOS_UINT8)PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR), 24, 30);

    if ((errType == 0) || (errCnt == VOS_NULL_PTR)) {
        return;
    }

    (VOS_VOID)memset_s(
        errCnt, sizeof(PPP_DRIVER_HdlcHardDefErrFramesCnt), 0x00, sizeof(PPP_DRIVER_HdlcHardDefErrFramesCnt));

    errCnt->errType = errType;

    /* get fcs error count */
    if (TTF_GET_A_BIT(errType, PPP_DRIVER_HDLC_HARD_DEF_FCS_ERR) == 1) {
        errCnt->fcsErrCnt = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(
            SOC_ARM_HDLC_DEF_ERR_INFO_0_ADDR(HDLC_IP_BASE_ADDR), 0, 15);
        PPP_HDLC_WARNING_LOG("bad hdlc fcs");
    }

    /* get frame too long error count */
    if (TTF_GET_A_BIT(errType, PPP_DRIVER_HDLC_HARD_DEF_FRAME_TOO_LONG) == 1) {
        errCnt->lenLongCnt = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(
            SOC_ARM_HDLC_DEF_ERR_INFO_0_ADDR(HDLC_IP_BASE_ADDR), 16, 31);
        PPP_HDLC_WARNING_LOG("bad hdlc frame length too long");
    }

    /* get frame too short error count */
    if (TTF_GET_A_BIT(errType, PPP_DRIVER_HDLC_HARD_DEF_FRAME_TOO_SHORT) == 1) {
        errCnt->lenShortCnt = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(
            SOC_ARM_HDLC_DEF_ERR_INFO_1_ADDR(HDLC_IP_BASE_ADDR), 0, 15);
        PPP_HDLC_WARNING_LOG("bad hdlc frame length too short");
    }

    /* get error protocol count */
    if (TTF_GET_A_BIT(errType, PPP_DRIVER_HDLC_HARD_DEF_PROTOCOL_ERR) == 1) {
        errCnt->errProtocolCnt = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(
            SOC_ARM_HDLC_DEF_ERR_INFO_1_ADDR(HDLC_IP_BASE_ADDR), 16, 31);
        PPP_HDLC_WARNING_LOG("bad hdlc frame protocol");
    }

    /* get error control count */
    if (TTF_GET_A_BIT(errType, PPP_DRIVER_HDLC_HARD_DEF_CTRL_ERR) == 1) {
        errCnt->errCtrlCnt = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(
            SOC_ARM_HDLC_DEF_ERR_INFO_2_ADDR(HDLC_IP_BASE_ADDR), 0, 15);
        PPP_HDLC_WARNING_LOG("bad hdlc frame control");
    }

    /* get error address count */
    if (TTF_GET_A_BIT(errType, PPP_DRIVER_HDLC_HARD_DEF_ADDR_ERR) == 1) {
        errCnt->errAddrCnt = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(
            SOC_ARM_HDLC_DEF_ERR_INFO_2_ADDR(HDLC_IP_BASE_ADDR), 16, 31);
        PPP_HDLC_WARNING_LOG("bad hdlc frame address");
    }

    /* get error flag position count */
    if (TTF_GET_A_BIT(errType, PPP_DRIVER_HDLC_HARD_DEF_FLAG_POS_ERR) == 1) {
        errCnt->flagPosErrCnt = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(
            SOC_ARM_HDLC_DEF_ERR_INFO_3_ADDR(HDLC_IP_BASE_ADDR), 0, 15);
        PPP_HDLC_WARNING_LOG("bad hdlc frame flag position");
    }

    return;
}

/* 获取解封装中断寄存器值 */
VOS_UINT32 PPP_DriverHdlcHardGetDefRawInt(VOS_VOID)
{
    VOS_UINT32 rawInt;

    rawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));

    return rawInt;
}

/* 获取封装寄存器信息 */
VOS_VOID PPP_DriverHdlcHardGetFrmRegInfo(PPP_DRIVER_HdlchardFrmRegInfo* regConfig)
{
    if (regConfig == VOS_NULL_PTR) {
        PPP_HDLC_WARNING_LOG("pstRegConfig");
        return;
    }

    /* 拷贝全部寄存器内容 */
    regConfig->stateSwRst       = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_STATE_SW_RST_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->priorTimeoutCtrl = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_PRIROR_TIMEOUT_CTRL_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->rdErrCurrAddr    = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_RD_ERR_CURR_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->wrErrCurrAddr    = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_WR_ERR_CURR_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcFrmEn        = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_EN_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcFrmRawInt    = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcFrmIntStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_INT_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcFrmIntClr    = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_INT_CLR_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcFrmCfg       = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_CFG_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcFrmAccm      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_ACCM_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcFrmStatus    = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmInLliAddr     = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_IN_LLI_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmInSublliAddr  = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_IN_SUBLLI_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmInPktLen      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_IN_PKT_LEN_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmInBlkAddr     = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_IN_BLK_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmInBlkLen      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_IN_BLK_LEN_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmOutLliAddr    = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_OUT_LLI_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmOutSpaceAddr  = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_OUT_SPACE_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmOutSpaceDep   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_OUT_SPACE_DEP_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmRptAddr       = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_RPT_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->frmRptDep        = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR));

    return;
}

/* 获取解封装寄存器信息 */
VOS_VOID PPP_DriverHdlcHardGetDefRegInfo(PPP_DRIVER_HdlchardDefRegInfo* regConfig)
{
    if (regConfig == VOS_NULL_PTR) {
        PPP_HDLC_WARNING_LOG("pstRegConfig");
        return;
    }

    /* 拷贝全部寄存器内容 */
    regConfig->stateSwRst         = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_STATE_SW_RST_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->priorTimeoutCtrl   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_PRIROR_TIMEOUT_CTRL_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->rdErrCurrAddr      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_RD_ERR_CURR_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->wrErrCurrAddr      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_WR_ERR_CURR_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefEn          = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_EN_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefRawInt      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefIntStatus   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INT_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefIntClr      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefCfg         = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_CFG_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defUncompletedLen  = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_LEN_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defUncompletedPro  = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_PRO_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defUncompletedAddr = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defUncompleteStAgo = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_AGO_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefGoOn        = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_GO_ON_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefStatus      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defUncompletStNow  = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_NOW_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defInLliAddr       = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_IN_LLI_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defInPktAddr       = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_IN_PKT_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defInPktLen        = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_IN_PKT_LEN_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defInPktLenMax     = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_IN_PKT_LEN_MAX_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defOutSpcAddr      = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_OUT_SPC_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defOutSpaceDep     = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_OUT_SPACE_DEP_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defRptAddr         = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_RPT_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defRptDep          = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefErrInfor0   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_ERR_INFO_0_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefErrInfor1   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_ERR_INFO_1_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefErrInfor2   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_ERR_INFO_2_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->hdlcDefErrInfor3   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_ERR_INFO_3_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defInfoFr1CntAgo   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INFO_FRL_CNT_AGO_ADDR(HDLC_IP_BASE_ADDR));
    regConfig->defInfoFr1CntNow   = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INFO_FRL_CNT_NOW_ADDR(HDLC_IP_BASE_ADDR));

    return;
}

/* 获取HDLCharddumpinformation */
VOS_VOID PPP_DriverHdlcHardGetDumpInfo(PPP_DRIVER_HardwareDumpInfo* dumpInfo)
{
    dumpInfo->hdlcRegInfo = g_hdlcRegSaveInfo;

    PPP_DriverHdlcHardGetFrmRegInfo(&(dumpInfo->hdlcFrmRegInfo));

    PPP_DriverHdlcHardGetDefRegInfo(&(dumpInfo->hdlcDefRegInfo));

    dumpInfo->hdlcConfigInfo = g_hdlcConfigInfo;
    return;
}

#ifdef CONFIG_UART_SHELL
/* 打印HDLC配置信息 */
MODULE_EXPORTED VOS_VOID PPP_DriverHdlcHardShowConfigInfo(VOS_VOID)
{
    const PPP_DRIVER_HdlcHardConfigInfo* hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    /* 输出IP相关信息 */
    TTF_PRINT_DEBUG("HDLC_IP_BASE_ADDR             = 0x%x\n", HDLC_IP_BASE_ADDR);
    TTF_PRINT_DEBUG("slHdlcISRDef                  = %d\n", hdlcConf->hdlcIsrDef);
    TTF_PRINT_DEBUG("slHdlcISRFrm                  = %d\n", hdlcConf->hdlcIsrFrm);
    TTF_PRINT_DEBUG("ulHdlcDefSem                  = %d\n", hdlcConf->hdlcDefSem);
    TTF_PRINT_DEBUG("ulHdlcFrmSem                  = %d\n", hdlcConf->hdlcFrmSem);
    TTF_PRINT_DEBUG("ulHdlcDefIntLimit             = %d\n", hdlcConf->hdlcDefIntLimit);
    TTF_PRINT_DEBUG("ulHdlcFrmIntLimit             = %d\n", hdlcConf->hdlcFrmIntLimit);
    return;
}

/* 打印封装寄存器当前数据 */
MODULE_EXPORTED VOS_VOID PPP_DriverHdlcHardShowFrmReg(VOS_VOID)
{
    PPP_DRIVER_HdlchardFrmRegInfo  regConfigInfo;
    PPP_DRIVER_HdlchardFrmRegInfo* regConfig = &regConfigInfo;

    PPP_DriverHdlcHardGetFrmRegInfo(regConfig);

    TTF_PRINT_DEBUG("HDLC Hardware ShowFrmReg Begin\n");

    TTF_PRINT_DEBUG("stateSwRst                  = 0x%x\n", regConfig->stateSwRst);
    TTF_PRINT_DEBUG("priorTimeoutCtrl            = 0x%x\n", regConfig->priorTimeoutCtrl);
    TTF_PRINT_DEBUG("rdErrCurrAddr               = 0x%x\n", regConfig->rdErrCurrAddr);
    TTF_PRINT_DEBUG("wrErrCurrAddr               = 0x%x\n", regConfig->wrErrCurrAddr);
    TTF_PRINT_DEBUG("hdlcFrmEn                   = 0x%x\n", regConfig->hdlcFrmEn);
    TTF_PRINT_DEBUG("hdlcFrmRawInt               = 0x%x\n", regConfig->hdlcFrmRawInt);
    TTF_PRINT_DEBUG("hdlcFrmIntStatus            = 0x%x\n", regConfig->hdlcFrmIntStatus);
    TTF_PRINT_DEBUG("hdlcFrmIntClr               = 0x%x\n", regConfig->hdlcFrmIntClr);
    TTF_PRINT_DEBUG("hdlcFrmCfg                  = 0x%x\n", regConfig->hdlcFrmCfg);
    TTF_PRINT_DEBUG("hdlcFrmAccm                 = 0x%x\n", regConfig->hdlcFrmAccm);
    TTF_PRINT_DEBUG("hdlcFrmStatus               = 0x%x\n", regConfig->hdlcFrmStatus);
    TTF_PRINT_DEBUG("frmInLliAddr                = 0x%x\n", regConfig->frmInLliAddr);
    TTF_PRINT_DEBUG("frmInSublliAddr             = 0x%x\n", regConfig->frmInSublliAddr);
    TTF_PRINT_DEBUG("frmInPktLen                 = 0x%x\n", regConfig->frmInPktLen);
    TTF_PRINT_DEBUG("frmInBlkAddr                = 0x%x\n", regConfig->frmInBlkAddr);
    TTF_PRINT_DEBUG("frmInBlkLen                 = 0x%x\n", regConfig->frmInBlkLen);
    TTF_PRINT_DEBUG("frmOutLliAddr               = 0x%x\n", regConfig->frmOutLliAddr);
    TTF_PRINT_DEBUG("frmOutSpaceAddr             = 0x%x\n", regConfig->frmOutSpaceAddr);
    TTF_PRINT_DEBUG("frmOutSpaceDep              = 0x%x\n", regConfig->frmOutSpaceDep);
    TTF_PRINT_DEBUG("frmRptAddr                  = 0x%x\n", regConfig->frmRptAddr);
    TTF_PRINT_DEBUG("frmRptDep                   = 0x%x\n", regConfig->frmRptDep);

    TTF_PRINT_DEBUG("HDLC Hardware ShowFrmReg End\n");

    return;
}

/* 打印解封装寄存器当前数据 */
MODULE_EXPORTED VOS_VOID PPP_DriverHdlcHardShowDefReg(VOS_VOID)
{
    PPP_DRIVER_HdlchardDefRegInfo  regConfigInfo;
    PPP_DRIVER_HdlchardDefRegInfo* regConfig = &regConfigInfo;

    PPP_DriverHdlcHardGetDefRegInfo(regConfig);

    TTF_PRINT_DEBUG("HDLC Hardware ShowDefReg Begin\n");

    TTF_PRINT_DEBUG("stateSwRst             = 0x%x\n", regConfig->stateSwRst);
    TTF_PRINT_DEBUG("priorTimeoutCtrl       = 0x%x\n", regConfig->priorTimeoutCtrl);
    TTF_PRINT_DEBUG("rdErrCurrAddr          = 0x%x\n", regConfig->rdErrCurrAddr);
    TTF_PRINT_DEBUG("wrErrCurrAddr          = 0x%x\n", regConfig->wrErrCurrAddr);
    TTF_PRINT_DEBUG("hdlcDefEn              = 0x%x\n", regConfig->hdlcDefEn);
    TTF_PRINT_DEBUG("hdlcDefRawInt          = 0x%x\n", regConfig->hdlcDefRawInt);
    TTF_PRINT_DEBUG("hdlcDefIntStatus       = 0x%x\n", regConfig->hdlcDefIntStatus);
    TTF_PRINT_DEBUG("hdlcDefIntClr          = 0x%x\n", regConfig->hdlcDefIntClr);
    TTF_PRINT_DEBUG("hdlcDefCfg             = 0x%x\n", regConfig->hdlcDefCfg);
    TTF_PRINT_DEBUG("defUncompletedLen      = 0x%x\n", regConfig->defUncompletedLen);
    TTF_PRINT_DEBUG("defUncompletedPro      = 0x%x\n", regConfig->defUncompletedPro);
    TTF_PRINT_DEBUG("defUncompletedAddr     = 0x%x\n", regConfig->defUncompletedAddr);
    TTF_PRINT_DEBUG("defUncompleteStAgo     = 0x%x\n", regConfig->defUncompleteStAgo);
    TTF_PRINT_DEBUG("hdlcDefGoOn            = 0x%x\n", regConfig->hdlcDefGoOn);
    TTF_PRINT_DEBUG("hdlcDefStatus          = 0x%x\n", regConfig->hdlcDefStatus);
    TTF_PRINT_DEBUG("defUncompletStNow      = 0x%x\n", regConfig->defUncompletStNow);
    TTF_PRINT_DEBUG("defInLliAddr           = 0x%x\n", regConfig->defInLliAddr);
    TTF_PRINT_DEBUG("defInPktAddr           = 0x%x\n", regConfig->defInPktAddr);
    TTF_PRINT_DEBUG("defInPktLen            = 0x%x\n", regConfig->defInPktLen);
    TTF_PRINT_DEBUG("defInPktLenMax         = 0x%x\n", regConfig->defInPktLenMax);
    TTF_PRINT_DEBUG("defOutSpcAddr          = 0x%x\n", regConfig->defOutSpcAddr);
    TTF_PRINT_DEBUG("defOutSpaceDep         = 0x%x\n", regConfig->defOutSpaceDep);
    TTF_PRINT_DEBUG("defRptAddr             = 0x%x\n", regConfig->defRptAddr);
    TTF_PRINT_DEBUG("defRptDep              = 0x%x\n", regConfig->defRptDep);
    TTF_PRINT_DEBUG("hdlcDefErrInfor0       = 0x%x\n", regConfig->hdlcDefErrInfor0);
    TTF_PRINT_DEBUG("hdlcDefErrInfor1       = 0x%x\n", regConfig->hdlcDefErrInfor1);
    TTF_PRINT_DEBUG("hdlcDefErrInfor2       = 0x%x\n", regConfig->hdlcDefErrInfor2);
    TTF_PRINT_DEBUG("hdlcDefErrInfor3       = 0x%x\n", regConfig->hdlcDefErrInfor3);
    TTF_PRINT_DEBUG("defInfoFr1CntAgo       = 0x%x\n", regConfig->defInfoFr1CntAgo);
    TTF_PRINT_DEBUG("defInfoFr1CntNow       = 0x%x\n", regConfig->defInfoFr1CntNow);

    TTF_PRINT_DEBUG("HDLC Hardware ShowDefReg End\n");
}
#endif

#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
/* 获取HDLC寄存器信息 */
VOS_UINT32 PPP_DriverHdlcGetRegValue(PPP_DRIVER_HdlcRegUint8 hdlcRegType, VOS_UINT32* value)
{
    /* 调用者保证pulValue非空 */

    switch (hdlcRegType) {
        case PPP_DRIVER_MAX_FRM_INFO_REG:

            *value = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR));

            break;

        case PPP_DRIVER_MAX_DEF_INFO_REG:

            *value = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR));

            break;

        default:

            /* 非法的情况将值置为全f */
            *value = 0xffffffff;
            PPP_HDLC_WARNING_LOG1("Invalid Hdlc Reg Type!", hdlcRegType);

            return PS_FAIL;
    }

    return PS_SUCC;
}

/* 读取HDLCIP版本号,kirin970之前版本不支持kirin */
MODULE_EXPORTED VOS_VOID PPP_HDLC_ReadVer(VOS_VOID)
{
    VOS_UINT32 ver = 0;

    /* 开时钟 */
    PPP_DriverHdlcHardPeriphClkOpen();

    if (PPP_DriverHdlcHardGetPeriphClkStatus() == VOS_TRUE) {
        /* ulVer = hdlc_version寄存器[31:0]位的值 */
        ver = PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_VERSION_ADDR(HDLC_IP_BASE_ADDR), 0, 31);
        TTF_PRINT_DEBUG("HDLC Version %d\r\n!", ver);
    }

    /* 关时钟 */
    PPP_DriverHdlcHardPeriphClkClose();

    return;
}
#endif

/* 设置最大封装解封装寄存器信息，只配置一次 */
VOS_VOID PPP_DriverHdlcSetMaxFrmDefInfoLen(VOS_VOID)
{
#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    /* 封装解封装信息域只配置一次 */
    static VOS_UINT8 cfgHdlcMaxLenFlag = 0;
    VOS_UINT32       hdlcRegValue      = 0;
    VOS_UINT32       result;

    /* 防止初始化时由于开时钟慢导致配置最大长度信息域失败,只检查一次 */
    if (cfgHdlcMaxLenFlag == 0) {
        cfgHdlcMaxLenFlag = 1;
        result            = PPP_DriverHdlcGetRegValue(PPP_DRIVER_MAX_FRM_INFO_REG, &hdlcRegValue);
        if (result != PS_SUCC) {
            PPP_HDLC_WARNING_LOG2("Get Frm Reg Fail!", result, hdlcRegValue);
        }

        /* 读寄存器失败也会强制配置一次 */
        if (hdlcRegValue != HDLC_MAX_FRM_DEF_INFO_LEN) {
            PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
        }

        result = PPP_DriverHdlcGetRegValue(PPP_DRIVER_MAX_DEF_INFO_REG, &hdlcRegValue);
        if (result != PS_SUCC) {
            PPP_HDLC_WARNING_LOG2("Get Def Reg Fail", result, hdlcRegValue);
        }

        /* 读寄存器失败也会强制配置一次 */
        if (hdlcRegValue != HDLC_MAX_FRM_DEF_INFO_LEN) {
            PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
        }
    }

#endif
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
