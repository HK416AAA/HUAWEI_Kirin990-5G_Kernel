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

/* HDLC���������Ϣ */
STATIC PPP_DRIVER_HdlcHardConfigInfo g_hdlcConfigInfo;

/* ��λ��32λ�Ĵ��� */
STATIC VOS_UINT32 PPP_HDLC_HardRead32RegByBit(VOS_UINT_PTR regAddr, VOS_UINT8 startBit, VOS_UINT8 endBit)
{
    VOS_UINT32 orgValue;

    orgValue = PPP_HDLC_READ_32REG(regAddr);

    return TTF_GetByBit(PS_PID_PPP_HDLC, orgValue, startBit, endBit);
}

/* HDLC��װ�ж����֪ͨ������� */
TTF_MDRV_INT_CB_DECLARE(PPP_DriverHdlcHardFrmIsr)
{
    PPP_DRIVER_HdlcHardConfigInfo *conf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    g_hdlcRegSaveInfo.hdlcFrmRawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));
    g_hdlcRegSaveInfo.hdlcFrmStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR));

    /* �յ�һ���жϺ����ԭʼ�ж� */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* �ͷŷ�װ����ź��� */
    /*lint -e(455) VOS_SmV�������, �������� */
    VOS_SmV(conf->hdlcFrmSem);

    g_pppHdlcHardStat.frmIsrCnt++;

    return TTF_MDRV_INT_HANDLED;
}

/* HDLC���װ�ж����֪ͨ������� */
TTF_MDRV_INT_CB_DECLARE(PPP_DriverHdlcHardDefIsr)
{
    PPP_DRIVER_HdlcHardConfigInfo *conf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    g_hdlcRegSaveInfo.hdlcDefRawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));
    g_hdlcRegSaveInfo.hdlcDefStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));

    /* �յ�һ���жϺ����ԭʼ�ж� */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* �ͷŷ�װ����ź��� */
    /*lint -e(455) VOS_SmV�������, �������� */
    VOS_SmV(conf->hdlcDefSem);

    g_pppHdlcHardStat.defIsrCnt++;

    return TTF_MDRV_INT_HANDLED;
}

/*
 * ϵͳ��������ַ���壬HDLC��ʱ�Ӳ����͹���״̬��ѯ������ʹ�øüĴ���
 * HDLCʱ�ӼĴ���������SYSCTRL CRG�Ĵ�������.xls������״̬�Ĵ���������SYSCTRL Modem�Ĵ�������.xls
 * ����HDLC������A�˵ĳ������Ĵ����ֱ�����PERI CRG�Ĵ�������.xls �� PERI SYSCTRL�Ĵ�������.xls
 */
#ifdef SOC_HDLC_GET_SYSCTRL_ADDR

/* ��ȡϵͳ����������ַ */
#define PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD (g_hdlcScCtrlBaseAddr)

VOS_UINT_PTR g_hdlcScCtrlBaseAddr = VOS_NULL_PTR;

/* ��ʼ��Hdlcsysctrl�Ĵ�����ַ */
VOS_VOID PPP_DriverHdlcScCtrlBaseAddrInit(VOS_VOID)
{
    VOS_UINT_PTR baseAddr = (VOS_UINT_PTR)SOC_HDLC_GET_SYSCTRL_ADDR();
    g_hdlcScCtrlBaseAddr  = PPP_HDLC_HARD_IO_ADDRESS(baseAddr);
}
#endif

/* �ȴ������������ */
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
 * LOW LEVEL CLK�Ż����ײ�ʱ�ӽӿ���mdrv��װ��
 * mdrv�ṩ mdrv_acc_clk_enable/mdrv_acc_clk_disable/mdrv_acc_clk_is_enabled�ӿ�
 *
 * B5000��ΪMDRVδ���䣬��ʱʹ���Ͻӿ�ʵ�֣���MDRVʵ�ֺ�ɾ����
 */
#if (defined(SOC_HDLC_CRG_CLKEN_OFFSET) && (VOS_OS_VER != VOS_WIN32))

/* ʱ��ʹ�ܼĴ��� */
#define HDLC_CRG_CLKEN4_ADDR(base) ((base) + (SOC_HDLC_CRG_CLKEN_OFFSET))
/* ʱ�ӹرռĴ��� */
#define HDLC_CRG_CLKENDIS4_ADDR(base) ((base) + (SOC_HDLC_CRG_CLKDIS_OFFSET))
/* ʱ��״̬�Ĵ��� */
#define HDLC_CRG_CLKSTA4_ADDR(base) ((base) + (SOC_HDLC_CRG_CLKSTAT_OFFSET))

/* ��ѯHDLCʱ�� */
STATIC VOS_BOOL PPP_DriverHdlcHardGetPeriphClkStatus(VOS_VOID)
{
    VOS_UINT32 value = 0;

    value = PPP_HDLC_READ_32REG(HDLC_CRG_CLKSTA4_ADDR(PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD));
    value &= (1 << SOC_HDLC_CRG_CLK_BITPOS);

    /* ���Ϊ��˵����ǰʱ��δ���� */
    if (value == 0) {
        return VOS_FALSE;
    } else {
        return VOS_TRUE;
    }
}

/* ��HDLCʱ�� */
VOS_VOID PPP_DriverHdlcHardPeriphClkOpen(VOS_VOID)
{
    VOS_UINT32 value = 0;

    value |= (1 << SOC_HDLC_CRG_CLK_BITPOS);
    PPP_HDLC_WRITE_32REG(HDLC_CRG_CLKEN4_ADDR(PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD), value);

    return;
}

/* �ر�HDLCʱ�� */
VOS_VOID PPP_DriverHdlcHardPeriphClkClose(VOS_VOID)
{
    VOS_UINT32 value = (1 << SOC_HDLC_CRG_CLK_BITPOS);

    PPP_DriverHdlcHardWaitStateIdle();

    PPP_HDLC_WRITE_32REG(HDLC_CRG_CLKENDIS4_ADDR(PPP_DRIVER_HDLC_HARD_GET_SC_BASE_ARRD), value);

    return;
}

#else

/* ��ѯHDLCʱ��״̬ */
STATIC VOS_BOOL PPP_DriverHdlcHardGetPeriphClkStatus(VOS_VOID)
{
    return (VOS_BOOL)(1 == mdrv_acc_clk_is_enabled(CLK_HDLC) ? VOS_TRUE : VOS_FALSE);
}

/* ��HDLCʱ�� */
MODULE_EXPORTED VOS_VOID PPP_DriverHdlcHardPeriphClkOpen(VOS_VOID)
{
    mdrv_acc_clk_enable(CLK_HDLC);
}

/* �ر�HDLCʱ�� */
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
    /* �жϹҽ� */
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

    /* �ж�ʹ�� */
    if (TTF_MDRV_INT_ENABLE(irqNum) != VOS_OK) {
        PPP_HDLC_ERROR_LOG1("PPP_HDLC_HardInit, ERROR, Enable slHdlcISRDef <1> failed!\r\n", irqNum);
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 PPP_DriverHdlcHardInit(PPP_DRIVER_HdlcHardFrmConfig *frmConfig, PPP_DRIVER_HdlcHardDefConfig *defConfig)
{
    /* ��ȡHDLC����ַ */
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
    /* Boston�汾������������װ�Ĵ���ʱ�п��������뿪ʱ��ָ��δִ����ϵ�������ʧ�� */
    PPP_DriverHdlcHardPeriphClkOpen();
#else
    /* �ر�HDLCʱ�� */
    PPP_DriverHdlcHardPeriphClkClose();
#endif

    /* ��ȡHDLC���װ�жϺ� */
    hdlcConf->hdlcIsrDef = mdrv_int_get_num_byname("hdlc_deframer_int");

    /* ��ȡHDLC��װ�жϺ� */
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
        /* Boston�汾�����÷�װ��Ϣ����󳤶ȼĴ���0x5DF(1503)Bytes */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
    }

    PPP_DriverHdlcHardPeriphClkClose();
#endif

    return VOS_OK;
} /* link_HDLCInit */

/* ��ȡHDLC����״̬ */
VOS_VOID PPP_DriverHdlcHardWorkStatus(VOS_BOOL *frmWork, VOS_BOOL *defWork)
{
    VOS_UINT32 frmValue;
    VOS_UINT32 defValue;

    /* SoC���ڴ�����һ�����������ʱ���Զ���ʹ��λ���� */
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
/* ���ý��װ�ж�ˮ�� */
MODULE_EXPORTED VOS_UINT32 PPP_DriverHdlcHardSetDefIntLimit(VOS_UINT32 intLimit)
{
    PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    hdlcConf->hdlcDefIntLimit = intLimit;

    return hdlcConf->hdlcDefIntLimit;
}

/* ���÷�װ�ж�ˮ�� */
MODULE_EXPORTED VOS_UINT32 PPP_DriverHdlcHradSetFrmIntLimit(VOS_UINT32 intLimit)
{
    PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    hdlcConf->hdlcFrmIntLimit = intLimit;

    return hdlcConf->hdlcFrmIntLimit;
}
#endif

/* ����ͨ�üĴ��� */
STATIC VOS_VOID PPP_DriverHdlcHardCommCfgReg(VOS_VOID)
{
    /*
     * prior_timeout_ctrl(0x04)
     *  31  24   23                16   15  9         8             7   2    1          0
     * |--------|----------------------|-------|-------------------|-------|---------------|
     * |   Rsv  | axireq_timeout_value |  Rsv  | axireq_timeout_en |  Rsv  |hdlc_prior_ctrl|
     *
     * Reserved             [31:24] 8'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * axireq_timeout_value [23:16] 8'b0   h/s R/W  �������AXI���߶�д����ʱ���ж�ֵ
     * Reserved             [15:9]  2'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * axireq_timeout_en    [8]     1'b0   h/s R/W  �Ƿ�����Ӳ���ж�AXI���߶�д����ʱ����������ã�
     *                                               0������
     *                                               1����
     * Reserved             [7:2]   1'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * hdlc_prior_ctrl      [1:0]   1'b0   h/s R/W  HDLC��װ�����װ���ȼ����üĴ�����
     *                                                00��һ���ڹ����У���һ��Ҳ��ʹ������£��Ƚ��Ѵ��ڹ����е��Ǹ������꣬
     *                                                    ��˭�ȱ�ʹ�����Ƚ�˭�����ꣻ
     *                                                01�����з�װ���ȼ��ߣ�
     *                                                10�����н��װ���ȼ��ߣ�
     *                                                11����Ч��
     *                                                (HDLC�ڲ����Ʊ���ͣ�ߵļ�����ʼ�����������װ����ͣ����װ���������������
     *                                                ���װ�Ϳ�ʼ��������������װ����ͣ�����װ�������ݰ���������󣬷�װ�Ϳ�ʼ����������)
     */

    VOS_UINT32 value = 0x0;

    /* ����AXI����ʱʱ������ֵ��SoC�ṩ�����ұ�֤��ƽ̨���� */
    value |= (HDLC_AXI_REQ_TIMEOUT_VALUE << 16);

    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_PRIROR_TIMEOUT_CTRL_ADDR(HDLC_IP_BASE_ADDR), value);

    return;
}

/* �ȴ���װ���װ��ͣ����� */
/*lint -e{454} VOS_SmP�������, �������� */
STATIC VOS_UINT32 PPP_DriverHdlcHardCommWaitSem(VOS_SEM hdlcSem, VOS_UINT32 semTimeoutLen)
{
    VOS_UINT32 result;

    /* �ȴ���װ����װ��� */
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

/* ���÷�װʹ�õ��ڴ�����ؼĴ��� */
STATIC VOS_VOID PPP_DriverHdlcHardFrmCfgBufReg(VOS_VOID)
{
    PPP_DRIVER_HdlcHardFrmConfig *frmConf = VOS_NULL_PTR;

    frmConf = PPP_DRIVER_HDLC_HARD_GET_FRM_CONF;

    /* ����װ��������������ʼ��ַ���ø��Ĵ���frm_in_lli_addr */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_IN_LLI_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)frmConf->inputAddr);

    /* ����װ��������������ʼ��ַ���ø��Ĵ���frm_out_lli_addr */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_FRM_OUT_LLI_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)frmConf->outputAddr);

    /* ����װ��Ч֡�����Ϣ�ϱ��ռ���ʼ��ַ���ø��Ĵ���frm_rpt_addr */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_RPT_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)frmConf->reportAddr);

    /* ����װ��Ч֡�����Ϣ�ϱ��ռ�������ø��Ĵ���frm_rpt_dep��[15:0]λ */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(frmConf->rptBufLen & 0xFFFF));

#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    /* ���÷�װ��Ϣ����󳤶ȼĴ���0x5DF(1503)Bytes */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
#endif

    return;
}

/* IPģʽ�����÷�װ��ؼĴ��� */
VOS_VOID PPP_DriverHdlcHardFrmCfgReg(PPP_DRIVER_HdlcHardFrmPara *drvFrmPara)
{
    VOS_UINT32 frmCfg = 0;

    /*
     * hdlc_frm_cfg   (0x20)
     *  31              16 15           4 3      2   1      0
     * |-------------------|--------------|--------|------|------|
     * |      protocol     |      Rsv     |   Pfc  | Acfc |1dor2d|
     *
     * frm_protocol         [31:16] 16'b0   h/s R/W  ��װЭ��ֵ����ЧЭ��ֵ�涨�μ�����б�
     * Reserved             [15:4]  12'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * frm_pfc              [3:2]   2'b0    h/s R/W  P��ѹ��ָʾ��00��Ӳ��ģ�����P��P����ѹ��;
     *                                                            01��Ӳ��ģ�����P��P��ѹ��;
     *                                                            11��Ӳ��ģ�鲻���P��;
     *                                                            ��������Ч;
     * frm_acfc             [1]     1'b0    h/s R/W  AC��ѹ��ָʾ��0��AC����ѹ��;1����ʾAC��ѹ��;
     * frm_in_lli_1dor2d    [0]     1'b0    h/s R/W  ��װ����һά���ά����ѡ��ָʾ�Ĵ�����
     *                                                             0Ϊһά;1Ϊ��ά;
     */

    frmCfg |= (drvFrmPara->pppPcFlag << HDLC_FRM_PFC_BITPOS);
    if (drvFrmPara->pppAcFlag == HDLC_ADDRESS_CTRL_COMPRESS) {
        TTF_SET_A_BIT(frmCfg, HDLC_FRM_ACFC_BITPOS);
    }
    frmCfg |= (((VOS_UINT32)drvFrmPara->protocol) << 16);

    /* ���Ĵ���hdlc_frm_cfg��[0]λfrm_in_lli_1dor2d����Ϊ0 */

    /* ����hdlc_frm_cfg�� P��� AC�� */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_ACCM_ADDR(HDLC_IP_BASE_ADDR), drvFrmPara->accmFlag);

    /* ���üĴ���hdlc_frm_cfg��[31:16]λfrm_protocolΪusProtocol */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_CFG_ADDR(HDLC_IP_BASE_ADDR), frmCfg);

    return;
}

/*
 * ��ȡ�Ĵ�����������
 * �������:ulEnable-��װ���װʹ��ǰ����ʹ�ܺ�VOS_TRUE:ʹ�ܺ�,VOS_FALSE:ʹ��ǰ
 * ulValue-ʹ�ܼĴ������õ�ֵ
 * ulEnableInterrupt-�жϷ�ʽ������ѯ��ʽ������ʹ�ܺ�ȡ�Ĵ���ʱ��Ч
 */
VOS_VOID PPP_DriverHdlcHardTraceFrmRegConfig(
    VOS_UINT32 enable, VOS_UINT32 value, VOS_UINT32 enableInterrupt, PPP_DRIVER_HdlchardFrmRegInfo *frmRegInfo)
{
    HDLC_MNTN_FrmRegConfig         regMntn;
    PPP_DRIVER_HdlchardFrmRegInfo *regConfig = &(regMntn.frmRegInfo);
    VOS_UINT32                     dataLen;

    /* �ڲ����ñ�֤�����ǿ� */

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_REG) == VOS_TRUE) {
        dataLen = sizeof(HDLC_MNTN_FrmRegConfig);

        /* ����ȫ���Ĵ������� */
        (VOS_VOID)memcpy_s(
            regConfig, sizeof(PPP_DRIVER_HdlchardFrmRegInfo), frmRegInfo, sizeof(PPP_DRIVER_HdlchardFrmRegInfo));

        /* ʹ��ǰ������ʹ�ܼĴ�����û�����ã���Ϊ����֮��HDLC�Ὺʼ��������ı������Ĵ�����ֵ */
        if (enable == VOS_FALSE) {
            regConfig->hdlcFrmEn = value;
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_FRM_REG_BEFORE_EN, dataLen);
        } else {
            /* ʹ�ܺ󹴰�ʱ����������жϷ�ʽ����RawInt��Statusȡg_hdlcRegSaveInfo�����ֵ */
            if (enableInterrupt == VOS_TRUE) {
                regConfig->hdlcFrmRawInt = g_hdlcRegSaveInfo.hdlcFrmRawInt;
                regConfig->hdlcFrmStatus = g_hdlcRegSaveInfo.hdlcFrmStatus;
            }
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_FRM_REG_AFTER_EN, dataLen);
        }
    }

    return;
}

/* ����ʹ�ܼĴ��� */
STATIC VOS_UINT32 PPP_DriverHdlcHardFrmCfgEnReg(VOS_UINT32 totalLen)
{
    PPP_DRIVER_HdlchardFrmRegInfo regConfig;

    /*
     * 1.hdlc_frm_en   (0x10)
     * 31   25 24  23 18 17  16  15  14  13  12  11  10   9   8  7    1  0
     * |--------|---|-----|---|---|---|---|---|---|---|---|---|---|------|---|
     * |   Rsv  |en | Rsv |en |en |en |en |en |en |en |en |en |en |  Rsv |en |
     *
     * Reserved            [31:25] 7'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * frm_over_int_en     [24]    1'b0    h/s R/W  һ�������װ�����ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��;
     * Reserved            [23:18] 6'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * frm_rpt_dep_err_en  [17]    1'b0    h/s R/W  ��װ�ⲿ��ȷ֡�����ϱ��ռ䲻���ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��;
     * frm_out_spc_err_en  [16]    1'b0    h/s R/W  ��װ�ⲿ����洢�ռ䲻���ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_rpt_prm_err_en  [15]    1'b0    h/s R/W  ��װ�ϱ��ռ���ز��������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_out_prm_err_en  [14]    1'b0    h/s R/W  ��װ���������ز��������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_in_prm_err_en   [13]    1'b0    h/s R/W  ��װ����������ز��������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_cfg_err_en      [12]    1'b0    h/s R/W  ��װЭ�鼰��ѹ��ָʾ���ô����ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_wr_timeout_en   [11]    1'b0    h/s R/W  ��װʱAXI����д����timeout�ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_rd_timeout_en   [10]    1'b0    h/s R/W  ��װʱAXI���߶�����timeout�ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_wr_err_en       [9]     1'b0    h/s R/W  ��װʱAXI����д���������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * frm_rd_err_en       [8]     1'b0    h/s R/W  ��װʱAXI���߶����������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * Reserved            [7:1]   7'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * frm_en              [0]     1'b0    h/s R/W  һ�������װʹ�ܣ������frm_enд��1'b1������װ����;һ�������װ��ɺ���Ӳ���Զ���frm_en���㣻
     *                                             ��װ���̳���ʱ��Ӳ��Ҳ���frm_en�Զ����㣬ʹ�ڲ�״̬������IDLE״̬��
     *                                             дʱ����һ�������װʹ��;0����ʹ�ܷ�װ����;1��ʹ�ܷ�װ����;
     *                                             ��ʱ����һ�������װ����״̬;0��û�ڽ��з�װ����;1�����ڽ��з�װ����
     */

    VOS_UINT32       enableInterrupt;
    VOS_UINT32       value;
    const VOS_UINT32 interruptValue = 0x0103FF01; /* ʹ���жϷ�ʽʱ����ʹ�ܼĴ�����ֵ */
    const VOS_UINT32 pollValue      = 0x01;       /* ʹ����ѯ��ʽʱ����ʹ�ܼĴ�����ֵ */

    /* �жϴ���װ���ݵ��ܳ��ȣ�������������ʹ���жϷ�ʽ������ʹ����ѯ��ʽ */
    if (totalLen > HDLC_FRM_INTERRUPT_LIMIT) {
        /* ���÷�װ���ʹ�ܼĴ���hdlc_frm_en��[31:0]λΪ0x0103FF01 */
        value           = interruptValue;
        enableInterrupt = VOS_TRUE;

        g_pppHdlcHardStat.frmWaitIntCnt++;
    } else {
        /* ���÷�װ���ʹ�ܼĴ���hdlc_frm_en��[31:0]λΪ0x01 */
        value           = pollValue;
        enableInterrupt = VOS_FALSE;

        g_pppHdlcHardStat.frmWaitQueryCnt++;
    }

    /* ʹ��ǰ����ϴη�װ�����װ��ԭʼ�ж� */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* �ϱ��Ĵ�����ά�ɲ� */
    PPP_DriverHdlcHardGetFrmRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceFrmRegConfig(VOS_FALSE, value, enableInterrupt, &regConfig);

    /* ʹ��Ӳ��֮ǰ��ǿ��ARM˳��ִ�н���ǰ���ָ�� */
    TTF_FORCE_ARM_INSTUCTION();

    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_EN_ADDR(HDLC_IP_BASE_ADDR), value);

#if (VOS_OS_VER == VOS_WIN32)
    VHW_HDLC_Task();
#endif

    return enableInterrupt;
}

/* ��ѯ״̬�Ĵ��� */
STATIC VOS_UINT32 PPP_DriverHdlcHardFrmWaitStatusChange(VOS_VOID)
{
    VOS_UINT32          frmRsltWaitNum; /* ��ֹӲ���쳣�ı������� */
    volatile VOS_UINT32 frmStatus = 0;  /* ��װ״̬ */

    /* ��ѯhdlc_frm_status (0x28)�ĵ�[0]λ�͵�[1]λ���κ�һ��Ϊ1���߳�ʱ�򷵻� */
    frmRsltWaitNum = 0UL;

    while (frmRsltWaitNum < HDLC_FRM_MAX_WAIT_RESULT_NUM) {
        /* ��ȡ hdlc_frm_status��[0][1]λ */
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

/* �ȴ���װ��ͣ����� */
STATIC VOS_UINT32 PPP_DriverHdlcHardFrmWaitResult(VOS_UINT32 enableInterrupt)
{
    VOS_UINT32                     frmStatus; /* ��װ״̬ */
    VOS_UINT32                     result;
    PPP_DRIVER_HdlchardFrmRegInfo  regConfig;
    const PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    if (enableInterrupt == VOS_TRUE) {
        /* �ȴ��жϵõ���������״̬ */
        result = PPP_DriverHdlcHardCommWaitSem(hdlcConf->hdlcFrmSem, HDLC_FRM_INT_TIMER_LEN);

        /* �������жϷ�������н��������жϲ�������Statusָʾ�Ƿ�����bit��ԭʼ�жϼĴ����������ʴ˴�ȡ������g_hdlcRegSaveInfo�е�״ֵ̬ */
        frmStatus = g_hdlcRegSaveInfo.hdlcFrmStatus;
    } else {
        /* ��ѯ�õ��������� */
        result = PPP_DriverHdlcHardFrmWaitStatusChange();

        /* ��ѯhdlc_frm_status (0x28)��ȡ��װ״̬�����䷵�� */
        frmStatus = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    }

    /* �ϱ��Ĵ�����ά�ɲ� */
    PPP_DriverHdlcHardGetFrmRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceFrmRegConfig(VOS_TRUE, 0, enableInterrupt, &regConfig);

    /* �Ȳ���˵��HDLC���ڹ��� */
    if (result != VOS_OK) {
        return HDLC_FRM_ALL_PKT_DOING;
    }

    frmStatus &= HDLC_FRM_STATUS_MASK;

    return frmStatus;
}

/* ���÷�װ�Ĵ�����ʹ�ܷ�װ���� */
VOS_UINT32 PPP_DriverHdlcHardFrmEnable(PPP_DRIVER_HdlcHardFrmPara *drvFrmPara, VOS_UINT32 *enableInterrupt)
{
    VOS_UINT32 frmStatus;
    VOS_UINT32 enableFlag;

    /* ���÷�װ�����װͨ�üĴ��� */
    PPP_DriverHdlcHardCommCfgReg();

    /* �����ڴ���ؼĴ��� */
    PPP_DriverHdlcHardFrmCfgBufReg();

    /* ���÷�װ��ؼĴ��� */
    PPP_DriverHdlcHardFrmCfgReg(drvFrmPara);

    /* ����ʹ�ܼĴ��������ϱ�ʹ��ǰ�Ĵ�����ά�ɲ� */
    enableFlag = PPP_DriverHdlcHardFrmCfgEnReg(drvFrmPara->inputTotalSize);

    *enableInterrupt = enableFlag;

    frmStatus = PPP_DriverHdlcHardFrmWaitResult(enableFlag);

    return frmStatus;
}

/* ��ȡ��װ��� */
VOS_VOID PPP_DriverHdlcHardGetFrmResult(VOS_UINT16 *frmOutSegNum, VOS_UINT8 *frmValidNum)
{
    /* ��Ч֡���ռ��Ƭ�θ���usFrmOutSegNum= hdlc_frm_status�Ĵ���[31:16]λ��ֵ */
    *frmOutSegNum = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR), 16, 31);

    /* ��Ч֡����usFrmValidNum= hdlc_frm_status�Ĵ���[15:8]λ��ֵ */
    *frmValidNum = (VOS_UINT8)PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR), 8, 15);

    return;
}

/* ��ȡ��װ�жϼĴ���ֵ */
VOS_UINT32 PPP_DriverHdlcHardGetFrmRawInt(VOS_VOID)
{
    VOS_UINT32 rawInt;

    rawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));

    return rawInt;
}

/* ���ý��װʹ�õ��ڴ�����ؼĴ��� */
STATIC VOS_VOID PPP_DriverHdlcHardDefCfgBufReg(VOS_VOID)
{
    PPP_DRIVER_HdlcHardDefConfig *defConf = VOS_NULL_PTR;

    defConf = PPP_DRIVER_HDLC_HARD_GET_DEF_CONF;

    /* ����װ��������������ʼ��ַ���ø��Ĵ���def_in_lli_addr(0x90) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_IN_LLI_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)defConf->inputAddr);

    /* ����װ��������������ʼ��ַ���ø��Ĵ���def_out_spc_addr(0xA0) */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_DEF_OUT_SPC_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)defConf->outputAddr);

    /* ����װ��������������ʼ��ַ���ø��Ĵ���def_out_space_dep(0xA4)��16λ */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_DEF_OUT_SPACE_DEP_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(defConf->outputBufLen & 0xFFFF));

#ifndef PPPC_HDLC_NOC_ST_TEST
    /* ����װ��Ч֡�����Ϣ�ϱ��ռ���ʼ��ַ���ø��Ĵ���def_rpt_addr(0xA8) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_RPT_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(VOS_UINT_PTR)defConf->reportAddr);
#else
    /* ����NOC����,��CDSP��Resv�ռ�0xE39D9000���ø�HDLC */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_RPT_ADDR(HDLC_IP_BASE_ADDR), 0xE39D9000);
#endif

    /* ����װ��Ч֡�����Ϣ�ϱ��ռ�������ø��Ĵ���def_rpt_dep (0xAC)��16λ */
    PPP_HDLC_WRITE_32REG(
        SOC_ARM_HDLC_DEF_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)(defConf->reportBufLen & 0xFFFF));

#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    /* ���÷�װ��Ϣ����󳤶ȼĴ���0x5DF(1503)Bytes */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
#endif

    return;
}

/* ���ý��װ���üĴ��� */
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
     * Reserved             [31:4]  28'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_pfc              [3:2]   2'b0    h/s R/W  P��ѹ��ָʾ��00��P����ѹ��������룻01��P��ѹ��������룻11��P�򲻰��룻��������Ч��
     * def_acfc             [1]     1'b0    h/s R/W  AC��ѹ��ָʾ��0��AC����ѹ����1����ʾAC��ѹ����
     * def_uncompleted_ago  [0]     1'b0    h/s R/W  ����ָʾ��Ӧ��ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������װ���������Ƿ��н��������֡��
     *                                               Ϊ��֧�ֶ��PPP/IP���Ŷ����ӵ����ã�0��û�У�1����
     */

    defConf = PPP_DRIVER_HDLC_HARD_GET_DEF_CONF;

    defCfg |= drvDefPara->pppPcFlag << HDLC_DEF_PFC_BITPOS;

    if (drvDefPara->pppAcFlag == HDLC_ADDRESS_CTRL_COMPRESS) {
        TTF_SET_A_BIT(defCfg, HDLC_DEF_ACFC_BITPOS);
    }

    /* ���÷�����֡�����Ϣ */
    uncompleteInfo = drvDefPara->uncompleteInfo;
    if ((uncompleteInfo != VOS_NULL_PTR) && (uncompleteInfo->existFlag == HDLC_DEF_UNCOMPLETED_EXIST)) {
        /*
         * def_uncompleted_len  (0x74)
         *  31                 16  15                  0
         * |---------------------|----------------------|
         * |         Rsv         |         Len          |
         * Reserved             [31:16] 16'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
         * def_uncompleted_len  [15:0]  16'b0   h/s R/W ��Ӧ��ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������װ����������������֡�ĳ��ȣ�Ϊ��֧�ֶ��PPP/IP���Ŷ����ӵ�����
         */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_LEN_ADDR(HDLC_IP_BASE_ADDR),
            (VOS_UINT32)uncompleteInfo->defOutOneLen & 0xFFFF);

        /*
         * def_uncompleted_pro  (0x78)
         * 31                 16  15                  0
         * |---------------------|----------------------|
         * |         Rsv         |         Pro          |
         * Reserved             [31:16] 16'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
         * def_uncompleted_pro  [15:0]  16'b0   h/s R/W  ��Ӧ��ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������װ����������������֡��
         *                                              Э�飬Ϊ��֧�ֶ��PPP/IP���Ŷ����ӵ����ã��������е�0Byte��1Byte��2Byte��Ч��
         */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_PRO_ADDR(HDLC_IP_BASE_ADDR),
            (VOS_UINT32)uncompleteInfo->defOutOnePro & 0xFFFF);

        /*
         * def_uncompleted_addr  (0x7C)
         * 31                  0
         * |----------------------|
         * |         Addr         |
         * def_uncompleted_addr [31:0]  32'b0   h/s R/W  ��Ӧ��ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������װ����������������֡��
         *                                              �ⲿ�洢��ʼ��ַ��Ϊ��֧�ֶ��PPP/IP���Ŷ����ӵ����ã��õ�ַ��������ԭ���ϱ���ͬ���µ�ַ��
         */
        PPP_HDLC_WRITE_32REG(
            SOC_ARM_HDLC_DEF_UNCOMPLETED_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)uncompleteInfo->defOutOneAddr);

        /*
         * def_uncomplet_st_ago  (0x80)
         * 31                  16 15             5 4     0
         * |----------------------|----------------|-------|
         * |         Ago          |       Rsv      |  Ago  |
         * crc16_result_ago     [31:16] 16'b0   h/s R/W  �뵱ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������׽��װ��������������������֡ʱ��CRCУ��ֵ
         * Reserved             [15:5]  11'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
         * def_data_st_curr_ago [4:0]   5'b0    h/s R/W  �뵱ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������׽��װ��������������������֡ʱ������״̬����ǰ״̬
         */
        PPP_HDLC_WRITE_32REG(
            SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_AGO_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)uncompleteInfo->defStAgo);

        /*
         * def_info_frl_cnt_ago  (0xC0)
         * 31        27 26                 16 15   11 10              0
         * |------------|---------------------|-------|-----------------|
         * |    Rsv     |         Ago         |  Rsv  |       Ago       |
         * Reserved             [31:27] 5'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
         * def_framel_cnt_ago   [26:16] 11'b0   h/s R/W  �뵱ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������׽��װ��������������������֡ʱ��֡����
         * Reserved             [15:11] 5'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
         * def_info_cnt_ago     [10:0]  11'b0   h/s R/W  �뵱ǰ���װ��������ͬһPPP/IP���ŵ���ǰ������׽��װ��������������������֡ʱ����Ϣ����
         */
        PPP_HDLC_WRITE_32REG(
            SOC_ARM_HDLC_DEF_INFO_FRL_CNT_AGO_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)uncompleteInfo->defInfoFrlCntAgo);

        TTF_SET_A_BIT(defCfg, HDLC_DEF_IS_UNCOMPLETED_AGO_BITPOS);
    }

    /* ����ulMode��P���AC���Ƿ�ѹ������hdlc_def_cfg (0x70) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_CFG_ADDR(HDLC_IP_BASE_ADDR), defCfg);

    /* ��������������󵥰����� */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_IN_PKT_LEN_MAX_ADDR(HDLC_IP_BASE_ADDR), (VOS_UINT32)defConf->perInMaxLen);

    return;
}

/* ��ѯ״̬�Ĵ��� */
STATIC VOS_UINT32 PPP_DriverHdlcHardDefWaitStatusChange(VOS_VOID)
{
    /*
     * hdlc_def_status  (0x88)
     * 31 30   24 23   8 7 6  5   4   3   2  1  0
     * |---|-------|------|---|---|---|---|---|----|
     * |Rsv|  Type |  Num |Rsv|Idx|Ful|Ful|Now|Stat|
     * Reserved             [31]    1'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_err_type         [30:24] 7'b0    h/s RO   ��֡�ϱ�ʱ������֡���ͣ���Ӧ��bitλΪ1���������������ʹ���
     *                                              bit 30����������6��ת���ַ�0x7D�����һ��Flag��
     *                                              bit 29����������5����AC����ѹ��ʱ��Address��ֵ��0xFF��
     *                                              bit 28����������4����AC����ѹ��ʱ��Control��ֵ��0x03��
     *                                              bit 27����������3����P�������ʱ���յ��Ƿ���Protocol��ֵ��
     *                                              bit 26����������2�����װ��֡�ֽ���С��4bites��
     *                                              bit
     * 25����������1�����װ��֡�ֽ�������1502bytes��PPP֡��Information�򲻳���1500Bytes������Э���򲻳���1502Bytes���� bit 24����������0�� CRCУ�����
     * def_valid_num        [23:8]  16'b0   h/s RO   ��֡�ϱ�ʱ����Ч֡��Ŀ�������������һ�����ܵķ�����֡�� 
     * Reserved             [7:6]   2'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣 
     * def_error_index      [5]     1'b0    h/s RO   ���װ��������ָʾ
     * def_rpt_ful          [4]     1'b0    h/s RO   ���װ�ⲿ��ȷ֡��Ϣ�ϱ��ռ������ָͣʾ
     * def_out_spc_ful      [3]     1'b0    h/s RO   ���װ�ⲿ����洢�ռ������ָͣʾ
     * def_uncompleted_now  [2]     1'b0    h/s RO   ����ָʾ��ǰ�����Ƿ��н��������֡��Ϊ��֧�ֶ��PPP/IP���Ŷ����ӵ����ã�0��û�У�1����
     * def_all_pkt_pro_stat [1:0]   2'b0    h/s RO   һ������������״̬��00��δ���һ������������01��δ���һ�������������ѽ��LCP֡��Ӳ��������ͣ״̬�� 
     *                                               10�����һ����������������֡�ϱ���11: ���һ����������������֡�ϱ���
     */
    VOS_UINT32          rsltWaitNum; /* ��ֹӲ���쳣�ı������� */
    volatile VOS_UINT32 status = 0;  /* ���װ״̬ */

    /* ��ѯhdlc_frm_status (0x28)�ĵ�[0]λ�͵�[1]λ���κ�һ��Ϊ1���߳�ʱ�򷵻� */
    rsltWaitNum = 0UL;

    while (rsltWaitNum < HDLC_DEF_MAX_WAIT_RESULT_NUM) {
        /* ��ѯ״̬�Ĵ���hdlc_def_status (0x88)��0-1��3-5λ���κ�һλ��Ϊ1��ʾ���װģ����ͣ��ֹͣ */
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
 * ��ȡ���װ�Ĵ�����������
 * �������:ulEnable-��װ���װʹ��ǰ����ʹ�ܺ�VOS_TRUE:ʹ�ܺ�,VOS_FALSE:ʹ��ǰ
 * ulValue-ʹ�ܼĴ������õ�ֵ
 * ulEnableInterrupt-�жϷ�ʽ������ѯ��ʽ������ʹ�ܺ�ȡ�Ĵ���ʱ��Ч
 */
VOS_VOID PPP_DriverHdlcHardTraceDefRegconfig(
    VOS_UINT32 enable, VOS_UINT32 value, VOS_UINT32 enableInterrupt, PPP_DRIVER_HdlchardDefRegInfo *defRegInfo)
{
    HDLC_MNTN_DefRegConfig         regMntn;
    PPP_DRIVER_HdlchardDefRegInfo *regConfig = &(regMntn.defRegInfo);
    VOS_UINT32                     dataLen;

    /* �ڲ����ñ�֤�����ǿ� */

    if (PPP_HDLC_HardMntnGetConfig(PPP_HDLC_MNTN_TRACE_REG) == VOS_TRUE) {
        dataLen = sizeof(HDLC_MNTN_DefRegConfig);

        /* ����ȫ���Ĵ������� */
        (VOS_VOID)memcpy_s(
            regConfig, sizeof(PPP_DRIVER_HdlchardDefRegInfo), defRegInfo, sizeof(PPP_DRIVER_HdlchardDefRegInfo));

        /* ʹ��ǰ������ʹ�ܼĴ�����û�����ã���Ϊ����֮��HDLC�Ὺʼ��������ı������Ĵ�����ֵ */
        if (enable == VOS_FALSE) {
            regConfig->hdlcDefEn = value;
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_DEF_REG_BEFORE_EN, dataLen);
        } else {
            /* ʹ�ܺ󹴰�ʱ����������жϷ�ʽ����RawInt��Statusȡg_hdlcRegSaveInfo�����ֵ */
            if (enableInterrupt == VOS_TRUE) {
                regConfig->hdlcDefRawInt = g_hdlcRegSaveInfo.hdlcDefRawInt;
                regConfig->hdlcDefStatus = g_hdlcRegSaveInfo.hdlcDefStatus;
            }
            PPP_HDLC_HardMntnTraceMsg((HDLC_MNTN_TraceHead*)&regMntn, ID_HDLC_MNTN_DEF_REG_AFTER_EN, dataLen);
        }
    }

    return;
}

/* �ȴ����װ��ͣ����� */
VOS_UINT32 PPP_DriverHdlcHardDefWaitResult(VOS_UINT32 enableInterrupt)
{
    VOS_UINT32                     status; /* ���װ״̬ */
    VOS_UINT32                     result;
    PPP_DRIVER_HdlchardDefRegInfo  regConfig;
    const PPP_DRIVER_HdlcHardConfigInfo *hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    if (enableInterrupt == VOS_TRUE) {
        /* �ȴ��жϵõ���ͣ�����״̬ */
        result = PPP_DriverHdlcHardCommWaitSem(hdlcConf->hdlcDefSem, HDLC_DEF_INT_TIMER_LEN);

        /* �������жϷ�������н��������жϲ�������Statusָʾ�Ƿ�����bit��ԭʼ�жϼĴ����������ʴ˴�ȡ������g_hdlcRegSaveInfo�е�״ֵ̬ */
        status = g_hdlcRegSaveInfo.hdlcDefStatus;

    } else {
        /* ��ѯ�õ���ͣ����� */
        result = PPP_DriverHdlcHardDefWaitStatusChange();

        /* ��ѯhdlc_def_status (0x88)��ȡ���װ״̬�����䷵�� */
        status = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));
    }

    /* �ϱ��Ĵ�����ά�ɲ� */
    PPP_DriverHdlcHardGetDefRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceDefRegconfig(VOS_TRUE, 0, enableInterrupt, &regConfig);

    /* �Ȳ���˵��HDLC���ڹ��� */
    if (result != VOS_OK) {
        return HDLC_DEF_STATUS_DOING;
    }

    status &= HDLC_DEF_STATUS_MASK;

    return status;
}

/* ���ݱ��ν��װ��������С������ʹ�úο���ʽ�ȴ�HDLC�Ľ�� */
STATIC VOS_UINT32 PPP_DriverHdlcHardDefCfgEnReg(VOS_UINT32 totalLen)
{
    /*
     * hdlc_def_en   (0x60)
     * 31   25 24  23 19 18  17  16  15  14  13  12  11  10   9   8   7   1  0
     * |--------|---|-----|---|---|---|---|---|---|---|---|---|---|---|------|---|
     * |   Rsv  |en | Rsv |en |en |en |en |en |en |en |en |en |en |en |  Rsv |en |
     *
     * Reserved            [31:25] 7'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_over_int_en     [24]    1'b0    h/s R/W  һ��������װ�����ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��;
     * Reserved            [23:19] 5'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_rpt_ful_en      [18]    1'b0    h/s R/W  ���װ�ⲿ��ȷ֡��Ϣ�ϱ��ռ������ͣ�ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��;
     * def_out_spc_ful_en  [17]    1'b0    h/s R/W  ���װ�ⲿ����洢�ռ������ͣ�ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_lcp_int_en      [16]    1'b0    h/s R/W  ���װ�����ЧLCP֡��ͣ�ж��ϱ�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_rpt_prm_err_en  [15]    1'b0    h/s R/W  ���װ�ϱ��ռ���ز��������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_out_prm_err_en  [14]    1'b0    h/s R/W  ���װ����ռ���ز��������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_in_prm_err_en   [13]    1'b0    h/s R/W  ���װ����������ز��������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_cfg_err_en      [12]    1'b0    h/s R/W  ���װЭ��ѹ��ָʾ���ô����ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_wr_timeout_en   [11]    1'b0    h/s R/W  ���װʱAXI����д����timeout�ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_rd_timeout _en  [10]    1'b0    h/s R/W  ���װʱAXI���߶�����timeout�ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_wr_err_en       [9]     1'b0    h/s R/W  ���װʱAXI����д���������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * def_rd_err_en       [8]     1'b0    h/s R/W  ���װʱAXI���߶����������ж�ʹ��;0���жϽ�ֹ;1���ж�ʹ��
     * Reserved            [7:1]   7'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_en              [0]     1'b0    h/s R/W  һ������������װʹ�ܣ������def_enд��1'b1�������װ������һ������������װ��ɺ���Ӳ���Զ���def_en���㣻
     *                                             ���װ���̳���ʱ��Ӳ��Ҳ���def_en�Զ����㣬ʹ�ڲ�״̬������IDLE״̬�����üĴ������ؽ��װ����״̬��
     *                                             дʱ����һ������������װʹ�ܣ�0����ʹ�ܽ��װ����1��ʹ�ܽ��װ����
     *                                             ��ʱ����һ������������װ����״̬��0��û�ڽ��н��װ����1�����ڽ��н��װ����
     */
    VOS_UINT32                    enableInterrupt;
    VOS_UINT32                    value;
    const VOS_UINT32              interruptValue = 0x0107FF01; /* ʹ���жϷ�ʽʱ����ʹ�ܼĴ�����ֵ */
    const VOS_UINT32              pollValue      = 0x01;       /* ʹ����ѯ��ʽʱ����ʹ�ܼĴ�����ֵ */
    PPP_DRIVER_HdlchardDefRegInfo regConfig;

    if (totalLen > HDLC_DEF_INTERRUPT_LIMIT) {
        /* ���÷�װ���ʹ�ܼĴ���hdlc_def_en��[31:0]λΪ0x0107FF01 */
        value           = interruptValue;
        enableInterrupt = VOS_TRUE;

        g_pppHdlcHardStat.defWaitIntCnt++;
    } else {
        /* ���÷�װ���ʹ�ܼĴ���hdlc_frm_en��[31:0]λΪ0x01 */
        value           = pollValue;
        enableInterrupt = VOS_FALSE;

        g_pppHdlcHardStat.defWaitQueryCnt++;
    }

    /* ʹ��ǰ����ϴη�װ�����װ��ԭʼ�ж� */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INT_CLR_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);

    /* �ϱ��Ĵ�����ά�ɲ� */
    PPP_DriverHdlcHardGetDefRegInfo(&regConfig);
    PPP_DriverHdlcHardTraceDefRegconfig(VOS_FALSE, value, enableInterrupt, &regConfig);

    /* ʹ��Ӳ��֮ǰ��ǿ��ARM˳��ִ�н���ǰ���ָ�� */
    TTF_FORCE_ARM_INSTUCTION();

    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_EN_ADDR(HDLC_IP_BASE_ADDR), value);

#if (VOS_OS_VER == VOS_WIN32)
    VHW_HDLC_Task();
#endif

    return enableInterrupt;
}

/* ���ý��װ�Ĵ�����ʹ�ܽ��װ���� */
VOS_UINT32 PPP_DriverHdlcHardDefEnable(PPP_DRIVER_HdlcHardDefPara *drvDefPara, VOS_UINT32 *enableInterrupt)
{
    if ((drvDefPara == VOS_NULL_PTR) || (enableInterrupt == VOS_NULL_PTR)) {
        PPP_HDLC_WARNING_LOG("pstDrvDefPara or pulEnableInterrupt is NULL");
        return VOS_ERR;
    }

    /* ���÷�װ�����װͨ�üĴ��� */
    PPP_DriverHdlcHardCommCfgReg();

    /* �����ڴ���ؼĴ��� */
    PPP_DriverHdlcHardDefCfgBufReg();

    /* ����ѹ��ָʾ��������֡�����Ϣ�Ĵ��� */
    PPP_DriverHdlcHardDefCfgReg(drvDefPara);

    /* ����ʹ�ܼĴ��� */
    *enableInterrupt = PPP_DriverHdlcHardDefCfgEnReg(drvDefPara->inputTotalSize);

    return VOS_OK;
}

/* ����HDLC��ͣ״̬������GO_ON�Ĵ�����Ӧ����λ */
VOS_VOID PPP_DriverHdlcHardDefCfgGoOnReg(VOS_UINT32 defStatus)
{
    /*
     * hdlc_def_go_on  (0x84)
     * 31                  17  16  15    9   8  7   1  0
     * |----------------------|----|-------|----|-----|----|
     * |         Rsv          |Goon|  Rsv  |Goon| Rsv |Goon|
     * Reserved             [31:17] 15'b0   h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_rpt_ful_goon     [16]    1'b0    h/s WO   �ⲿ���װ��Ч֡��Ϣ�ϱ��ռ������ͣ���
     * Reserved             [15:9]  7'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_outspc_ful_goon  [8]     1'b0    h/s WO   �ⲿ���װ������ݴ洢�ռ������ͣ״̬���
     * Reserved             [7:1]   7'b0    h/s R/W  ����λ����ʱ����0��дʱ��Ӱ�졣
     * def_lcp_goon         [0]     1'b0    h/s WO
     * ���һ���Ϸ�LCP֡���µ�Ӳ����ͣ״̬����������װģ��δ������һ������װ�����ݣ�<=2KB(def_in_pkt_len_max)�������һ���Ϸ�LCP֡�������ͣ��֡���ȴ��������˼Ĵ���д"1"���ټ�������ʣ������ݡ�
     */

    /* GO_ONǰ����ϴν��װ��ԭʼ�ж� */
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

/* ��ȡ���װ��Ч֡���� */
VOS_UINT16 PPP_DriverHdlcHardGetDefVaildNum(VOS_VOID)
{
    VOS_UINT16 validFrameNum;

    validFrameNum = (VOS_UINT16)PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR), 8, 23);

    return validFrameNum;
}

/* ��ȡ���װ������֡��Ϣ */
VOS_VOID PPP_DriverHdlcHardGetDefUncompletInfo(
    PPP_DRIVER_HdlcHardDefUncompletedInfo *uncompleteInfo, VOS_UINT32 *validNum)
{
    VOS_UINT32 status; /* ���װ״̬ */
    VOS_UINT32 validFrameNum;

    status = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));

    /* ��ѯ״̬�Ĵ���hdlc_def_status (0x88)�ĵ�2λΪ1��ʾ���ν��װ�з�����֡�����Ϊ0��ʾ�޷�����֡��� */
    if ((status & 0x4) == 0) {
        uncompleteInfo->existFlag = HDLC_DEF_UNCOMPLETED_NOT_EXIST;

        return;
    }

    g_pppHdlcHardStat.defUncompleteCnt++;

    /* def_valid_num        [23:8]  16'b0   h/s RO   ��֡�ϱ�ʱ����Ч֡��Ŀ�������������һ�����ܵķ�����֡�� */
    validFrameNum = (status & 0xFFFF00) >> 8;

    *validNum = validFrameNum;
    /* �з�����֡ʱ��Ҫ��ȡ������def_uncomplet_st_now(0x8C)��def_info_frl_cnt_now(0xC4)��ȡ�������ϱ��ռ���Ч֮֡��ķ�����֡���ȡ�Э������ݵ�ַ */
    uncompleteInfo->existFlag = HDLC_DEF_UNCOMPLETED_EXIST;

    /* ���ֻ���ݴ���Щ��Ϣ�����½��װ��ʱ����ԭ�����HDLC */
    uncompleteInfo->defStAgo         = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_NOW_ADDR(HDLC_IP_BASE_ADDR));
    uncompleteInfo->defInfoFrlCntAgo = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INFO_FRL_CNT_NOW_ADDR(HDLC_IP_BASE_ADDR));

    return;
}

/* ��ȡ����֡��Ϣ */
VOS_VOID PPP_DriverHdlcHardGetDefErrorInfo(PPP_DRIVER_HdlcHardDefErrFramesCnt *errCnt)
{
    VOS_UINT8 errType;

    /* ��ѯ״̬�Ĵ���hdlc_def_status (0x88)�ĵ�24:30��Ӧ����λΪ1��ʾ��ĳ�ִ���֡�����
       Ϊ0��ʾ��֡��� */
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

/* ��ȡ���װ�жϼĴ���ֵ */
VOS_UINT32 PPP_DriverHdlcHardGetDefRawInt(VOS_VOID)
{
    VOS_UINT32 rawInt;

    rawInt = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_RAW_INT_ADDR(HDLC_IP_BASE_ADDR));

    return rawInt;
}

/* ��ȡ��װ�Ĵ�����Ϣ */
VOS_VOID PPP_DriverHdlcHardGetFrmRegInfo(PPP_DRIVER_HdlchardFrmRegInfo* regConfig)
{
    if (regConfig == VOS_NULL_PTR) {
        PPP_HDLC_WARNING_LOG("pstRegConfig");
        return;
    }

    /* ����ȫ���Ĵ������� */
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

/* ��ȡ���װ�Ĵ�����Ϣ */
VOS_VOID PPP_DriverHdlcHardGetDefRegInfo(PPP_DRIVER_HdlchardDefRegInfo* regConfig)
{
    if (regConfig == VOS_NULL_PTR) {
        PPP_HDLC_WARNING_LOG("pstRegConfig");
        return;
    }

    /* ����ȫ���Ĵ������� */
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

/* ��ȡHDLCharddumpinformation */
VOS_VOID PPP_DriverHdlcHardGetDumpInfo(PPP_DRIVER_HardwareDumpInfo* dumpInfo)
{
    dumpInfo->hdlcRegInfo = g_hdlcRegSaveInfo;

    PPP_DriverHdlcHardGetFrmRegInfo(&(dumpInfo->hdlcFrmRegInfo));

    PPP_DriverHdlcHardGetDefRegInfo(&(dumpInfo->hdlcDefRegInfo));

    dumpInfo->hdlcConfigInfo = g_hdlcConfigInfo;
    return;
}

#ifdef CONFIG_UART_SHELL
/* ��ӡHDLC������Ϣ */
MODULE_EXPORTED VOS_VOID PPP_DriverHdlcHardShowConfigInfo(VOS_VOID)
{
    const PPP_DRIVER_HdlcHardConfigInfo* hdlcConf = PPP_DRIVER_HDLC_HARD_GET_CONFIG;

    /* ���IP�����Ϣ */
    TTF_PRINT_DEBUG("HDLC_IP_BASE_ADDR             = 0x%x\n", HDLC_IP_BASE_ADDR);
    TTF_PRINT_DEBUG("slHdlcISRDef                  = %d\n", hdlcConf->hdlcIsrDef);
    TTF_PRINT_DEBUG("slHdlcISRFrm                  = %d\n", hdlcConf->hdlcIsrFrm);
    TTF_PRINT_DEBUG("ulHdlcDefSem                  = %d\n", hdlcConf->hdlcDefSem);
    TTF_PRINT_DEBUG("ulHdlcFrmSem                  = %d\n", hdlcConf->hdlcFrmSem);
    TTF_PRINT_DEBUG("ulHdlcDefIntLimit             = %d\n", hdlcConf->hdlcDefIntLimit);
    TTF_PRINT_DEBUG("ulHdlcFrmIntLimit             = %d\n", hdlcConf->hdlcFrmIntLimit);
    return;
}

/* ��ӡ��װ�Ĵ�����ǰ���� */
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

/* ��ӡ���װ�Ĵ�����ǰ���� */
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
/* ��ȡHDLC�Ĵ�����Ϣ */
VOS_UINT32 PPP_DriverHdlcGetRegValue(PPP_DRIVER_HdlcRegUint8 hdlcRegType, VOS_UINT32* value)
{
    /* �����߱�֤pulValue�ǿ� */

    switch (hdlcRegType) {
        case PPP_DRIVER_MAX_FRM_INFO_REG:

            *value = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR));

            break;

        case PPP_DRIVER_MAX_DEF_INFO_REG:

            *value = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR));

            break;

        default:

            /* �Ƿ��������ֵ��Ϊȫf */
            *value = 0xffffffff;
            PPP_HDLC_WARNING_LOG1("Invalid Hdlc Reg Type!", hdlcRegType);

            return PS_FAIL;
    }

    return PS_SUCC;
}

/* ��ȡHDLCIP�汾��,kirin970֮ǰ�汾��֧��kirin */
MODULE_EXPORTED VOS_VOID PPP_HDLC_ReadVer(VOS_VOID)
{
    VOS_UINT32 ver = 0;

    /* ��ʱ�� */
    PPP_DriverHdlcHardPeriphClkOpen();

    if (PPP_DriverHdlcHardGetPeriphClkStatus() == VOS_TRUE) {
        /* ulVer = hdlc_version�Ĵ���[31:0]λ��ֵ */
        ver = PPP_HDLC_HardRead32RegByBit(SOC_ARM_HDLC_VERSION_ADDR(HDLC_IP_BASE_ADDR), 0, 31);
        TTF_PRINT_DEBUG("HDLC Version %d\r\n!", ver);
    }

    /* ��ʱ�� */
    PPP_DriverHdlcHardPeriphClkClose();

    return;
}
#endif

/* ��������װ���װ�Ĵ�����Ϣ��ֻ����һ�� */
VOS_VOID PPP_DriverHdlcSetMaxFrmDefInfoLen(VOS_VOID)
{
#ifdef SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR
    /* ��װ���װ��Ϣ��ֻ����һ�� */
    static VOS_UINT8 cfgHdlcMaxLenFlag = 0;
    VOS_UINT32       hdlcRegValue      = 0;
    VOS_UINT32       result;

    /* ��ֹ��ʼ��ʱ���ڿ�ʱ��������������󳤶���Ϣ��ʧ��,ֻ���һ�� */
    if (cfgHdlcMaxLenFlag == 0) {
        cfgHdlcMaxLenFlag = 1;
        result            = PPP_DriverHdlcGetRegValue(PPP_DRIVER_MAX_FRM_INFO_REG, &hdlcRegValue);
        if (result != PS_SUCC) {
            PPP_HDLC_WARNING_LOG2("Get Frm Reg Fail!", result, hdlcRegValue);
        }

        /* ���Ĵ���ʧ��Ҳ��ǿ������һ�� */
        if (hdlcRegValue != HDLC_MAX_FRM_DEF_INFO_LEN) {
            PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_INFO_MAX_LEN_ADDR(HDLC_IP_BASE_ADDR), HDLC_MAX_FRM_DEF_INFO_LEN);
        }

        result = PPP_DriverHdlcGetRegValue(PPP_DRIVER_MAX_DEF_INFO_REG, &hdlcRegValue);
        if (result != PS_SUCC) {
            PPP_HDLC_WARNING_LOG2("Get Def Reg Fail", result, hdlcRegValue);
        }

        /* ���Ĵ���ʧ��Ҳ��ǿ������һ�� */
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
