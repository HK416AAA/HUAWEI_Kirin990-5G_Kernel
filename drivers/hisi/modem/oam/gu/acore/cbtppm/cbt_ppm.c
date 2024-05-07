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

/*
 * ��Ȩ���У�c����Ϊ�������޹�˾ 2001-2011
 * ��������: CBT������˿ڹ���ģ��
 * �޸���ʷ:
 * 1.��    ��: 2014��5��31��
 *   �޸�����: V8R1 OM_Optimize��Ŀ����
 */

#include "cbt_ppm.h"
#include "at_app_vcom_interface.h"
#include "cbt_cpm.h"
#include "cbt_scm_soft_decode.h"
#include "om_private.h"
#include "mdrv.h"
#include "pam_tag.h"
#ifdef CBT_ENABLED
#include "cbt_cpm.h"
#endif
#if (VOS_OS_VER != VOS_WIN32)
#include <securec.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_CBT_PPM_C
#define THIS_MODU mod_pam_cbt

#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED) || defined(CBT_ENABLED)

#define CBT_ACPU_OM_DATA_MAX_LEN 5000

/*
 * CBT socket ������ջ��С
 */
#define CBT_SOCKET_TASK_STACK_SIZE 8096

VOS_SEM g_cbtUsbPseudoSyncSemId; /* α��Ϊͬ���ӿ���ʹ�õ��ź��� */

/* ����ACPU��CBT�˿ڵ�UDI��� */
int g_cbtPortUDIHandle = VOS_ERROR;

CBT_ACPU_DEBUG_INFO g_cbtAcpuDebugInfo;

#ifdef OAM_UT_DEBUG
VOS_UINT32 g_cbtAcpuDbgFlag = VOS_NULL_DWORD;
#else
VOS_UINT32 g_cbtAcpuDbgFlag = VOS_FALSE;
#endif

/*
 * ��������: ��ӡ��ǰOMͨ����״̬
 * �޸���ʷ:
 * 2015.11.18 Creat Function
 */
VOS_VOID CBT_AcpuOpenLog(VOS_UINT32 flag)
{
    g_cbtAcpuDbgFlag = flag;

    return;
}

/*
 * Description: print om data info in debug mode.
 * History:
 *    Date: 2019-10-30
 *    Modification: Created function
 */
VOS_VOID CBT_AcpuPrintDbgInfo(const VOS_CHAR *funcName, const VOS_UINT8 *data, VOS_UINT32 len, VOS_UINT32 bitmap)
{
    VOS_UINT32 omDbgIndex;
    if ((g_cbtAcpuDbgFlag & bitmap) != VOS_FALSE) {
        mdrv_debug("<%s> Data Len=%d\n", funcName, len);
        for (omDbgIndex = 0; (omDbgIndex < len) && (omDbgIndex < CBT_ACPU_OM_DATA_MAX_LEN); omDbgIndex++) {
            mdrv_debug("%02x \n", *((VOS_UINT8 *)data + omDbgIndex));
        }
        mdrv_debug("<%s>\n", funcName);
    }
}

#if (!defined(DMT))
/*
 * ��������: ����ִ��ʧ��ʱ��ӡʧ����Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2019��10��28��
 *    �޸�����: Creat Function
 */
VOS_VOID CBT_PrintReturnVal(VOS_UINT32 ret, const VOS_CHAR *funcName, const VOS_CHAR *errInfo)
{
    if (ret != VOS_OK) {
        mdrv_err("<%s> %s return %u.\n", funcName, errInfo, ret);
    }
    return;
}
#endif

/*
 * ��������: ����CBT����
 * �������: pucVirAddr:   �������ַ
 *           pucPhyAddr:  ����ʵ��ַ
 *           dataLen:   ���ݳ���
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��26��
 *    �޸�����: Creat Function
 */
VOS_UINT32 CBTPPM_OamCbtPortDataSnd(VOS_UINT8 *virAddr, VOS_UINT32 dataLen)
{
    CBTCPM_SEND_FUNC func = VOS_NULL_PTR;

    func = CBTCPM_GetSndFunc();
    if (func == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    return func(virAddr, dataLen);
}

/*
 * ��������: �����������ͨ��USB(APP��)���͸�PC��
 * �������: pucVirAddr:   �������ַ
 *           pucPhyAddr:  ����ʵ��ַ
 *           dataLen:   ���ݳ���
 * �޸���ʷ:
 * 1. ��    ��: 2011��10��8��
 *    �޸�����: Creat Function
 */
VOS_UINT32 CBTPPM_OamUsbCbtSendData(VOS_UINT8 *virAddr, VOS_UINT32 dataLen)
{
    VOS_INT32                ret;
    struct acm_wr_async_info vcom;
    VOS_UINT32               writeSlice;

    if (virAddr == VOS_NULL_PTR) {
        /* ��ӡ���� */
        mdrv_err("<CBTPPM_OamUsbCbtSendData> Vir Addr is Null \n");

        return CBTCPM_SEND_PARA_ERR;
    }

    vcom.virt_addr = (VOS_CHAR *)virAddr;
    vcom.phy_addr  = VOS_NULL_PTR;
    vcom.size      = dataLen;
    vcom.priv      = VOS_NULL_PTR;

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteNum1++;

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteInSlice = VOS_GetSlice();

    ret = mdrv_udi_ioctl(g_cbtPortUDIHandle, ACM_IOCTL_WRITE_ASYNC, &vcom);

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteNum2++;

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteOutSlice = VOS_GetSlice();

    writeSlice = g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteOutSlice -
                 g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteInSlice;

    if (writeSlice > g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteMaxTime) {
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteMaxTime = writeSlice;
    }

    if (ret == MDRV_OK) { /* ��ǰ���ͳɹ� */
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemInSlice = VOS_GetSlice();

        if (VOS_SmP(g_cbtUsbPseudoSyncSemId, 0) != VOS_OK) {
            g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemErrNum++;

            mdrv_err("<CBTPPM_OamUsbCbtSendData> mdrv_udi_ioctl Send Data Fail Ret= %d\n", ret);
            return VOS_ERR;
        }

        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemOutSlice = VOS_GetSlice();
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemNum++;
        return VOS_OK;
    } else if (ret < MDRV_OK) { /* ��ʱ���� */
        /* ��ӡ��Ϣ������UDI�ӿڵĴ�����Ϣ */
        mdrv_err("<CBTPPM_OamUsbCbtSendData> mdrv_udi_ioctl Send Data Fail Ret=%d\n", ret);

        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrNum++;
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrLen += dataLen;
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrValue = (VOS_UINT32)ret;
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrTime  = VOS_GetSlice();

        return VOS_ERR; /* ������ʱ������Ҫ����NULL�������� */
    } else {            /* ����������Ҫ��λ���� */
        /* ��ӡ��Ϣ������UDI�ӿ� */
        mdrv_err("<CBTPPM_OamUsbCbtSendData> mdrv_udi_ioctl Send Data Fail Ret=%d\n", ret);

        mdrv_om_system_error(OAM_USB_SEND_ERROR, (VOS_INT)THIS_FILE_ID, (VOS_INT)__LINE__, (VOS_CHAR *)&ret,
                             sizeof(VOS_INT32));

        return VOS_ERR;
    }
}

/*
 * ��������: ���ڹر�USBУ׼�˿ڵĴ���
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��24��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamUsbCbtPortClose(VOS_VOID)
{
    VOS_INT32 ret;
    if (g_cbtPortUDIHandle == VOS_ERROR) {
        return;
    }

    ret = mdrv_udi_close(g_cbtPortUDIHandle);
    CBT_PRINT_RETURN_VAL(ret, "CBTPPM_OamUsbCbtPortClose", "mdrv_udi_close");
    g_cbtPortUDIHandle = VOS_ERROR;

    /* CBT�˿ڴ�USB��VCOM�л���UARTʱ���պ���ָ��Ϊ�գ��յ�USB״̬���ʱ�����Ͽ����� */
    if (CBTCPM_GetRcvFunc() == VOS_NULL_PTR) {
        return;
    }

    return;
}

/*
 * ��������: USB���ص�CBT�˿��첽�������ݵĻص�
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��24��
 *    �޸�����: V8R1 OM_Optimize��Ŀ����
 */
VOS_VOID CBTPPM_OamUsbCbtWriteDataCB(VOS_CHAR *virData, VOS_CHAR *phyData, VOS_INT len)
{
    VOS_UINT32 ret;

    if (len < 0) {
        mdrv_err("<CBTPPM_OamUsbCbtWriteDataCB> lLen < 0. \n");
    }

    /* αͬ���ӿڣ��ͷ��ź��� */
    ret = VOS_SmV(g_cbtUsbPseudoSyncSemId);
    CBT_PRINT_RETURN_VAL(ret, "CBTPPM_OamUsbCbtWriteDataCB", "VOS_SmV");
    return;
}

/*
 * ��������: ����ACPU���洦��USB����˿ڶϿ����У׼�Ͽ�
 * �������: enPortState:   �˿�״̬
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��24��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamUsbCbtStatusCB(ACM_EVT_E portState)
{
    /* CBT�˿ڴ�USB��VCOM�л���UARTʱ���պ���ָ��Ϊ�գ��յ�USB״̬���ʱ�����Ͽ����� */
    if (CBTCPM_GetRcvFunc() == VOS_NULL_PTR) {
        mdrv_err("<CBTPPM_OamUsbCbtStatusCB> CBTCPM_GetRcvFunc return NULL.\n");
        return;
    }

    return;
}

/*
 * ��������: ���ڳ�ʼ��USB���ص�CBTʹ�õĶ˿�
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��24��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamUsbCbtPortOpen(VOS_VOID)
{
    CBTPPM_OamCbtPortDataInit(OM_USB_CBT_PORT_HANDLE, CBTPPM_OamUsbCbtReadDataCB, CBTPPM_OamUsbCbtWriteDataCB,
                              CBTPPM_OamUsbCbtStatusCB);

    return;
}

/*
 * ��������: ����ACPU��������USB���ص�CBT������ͨ��ICC���͸�OMģ��
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��24��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamUsbCbtReadDataCB(VOS_VOID)
{
    struct acm_wr_async_info info;
    CBTCPM_RCV_FUNC          func = VOS_NULL_PTR;
    VOS_UINT32               readSlice;
    VOS_INT32                ret;

    if (g_cbtPortUDIHandle == VOS_ERROR) {
        mdrv_err("<CBTPPM_OamUsbCbtReadDataCB> Input HANDLE is err. \n");

        return;
    }

    (VOS_VOID)memset_s(&info, sizeof(info), 0, sizeof(info));

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadNum1++;

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadInSlice = VOS_GetSlice();

    /* ��ȡUSB��IO CTRL�ڵĶ����� */
    ret = mdrv_udi_ioctl(g_cbtPortUDIHandle, UDI_ACM_IOCTL_GET_READ_BUFFER_CB, &info);
    if (ret != VOS_OK) {
        mdrv_err("<CBTPPM_OamUsbCbtReadDataCB> Call mdrv_udi_ioctl is Failed\n");

        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadErrNum++;
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadErrValue = (VOS_UINT32)ret;
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadErrTime  = VOS_GetSlice();

        return;
    }

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadNum2++;

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadOutSlice = VOS_GetSlice();

    readSlice = g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadOutSlice -
                g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadInSlice;

    if (readSlice > g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadMaxTime) {
        g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadMaxTime = readSlice;
    }

    /*lint -e40*/
    CBT_AcpuPrintDbgInfo("CBTPPM_OamUsbCbtReadDataCB", (VOS_UINT8 *)info.virt_addr, info.size, CBT_ACPU_USB_CB);
    /*lint +e40*/

    /* ���ݽ��պ��� */
    func = CBTCPM_GetRcvFunc();
    if (func != VOS_NULL_PTR) {
        if (func((VOS_UINT8 *)info.virt_addr, info.size) != VOS_OK) {
            g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbDecodeErrNum++;
        }
    }

    if (mdrv_udi_ioctl(g_cbtPortUDIHandle, UDI_ACM_IOCTL_RETUR_BUFFER_CB, &info) != VOS_OK) {
        mdrv_err("<CBTPPM_OamUsbCbtReadDataCB> Call mdrv_udi_ioctl UDI_ACM_IOCTL_RETUR_BUFFER_CB is Failed\n");

        return;
    }

    return;
}

/*
 * ��������: ���ڳ�ʼ��CBTʹ�õĶ˿��豸
 * �������: enHandle: �˿ڵľ��
 *           pReadCB: �ö˿�����Ķ�ȡ�ص�����
 *           pWriteCB: �ö˿�������첽д�ص�����
 *           pStateCB: �ö˿������״̬�ص�����
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��24��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamCbtPortDataInit(OM_ProtHandleUint32 handle, VOS_VOID *readCb, VOS_VOID *writeCb, VOS_VOID *stateCb)
{
    struct udi_open_param     udiPara;
    struct acm_read_buff_info readBuffInfo;

    udiPara.devid = UDI_ACM_OM_ID;

    readBuffInfo.buff_size = OM_ICC_BUFFER_SIZE;
    readBuffInfo.buff_num  = OM_DRV_MAX_IO_COUNT;

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenNum++;
    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenSlice = VOS_GetSlice();

    if (g_cbtPortUDIHandle != VOS_ERROR) {
        mdrv_err("<CBTPPM_OamCbtPortDataInit> The UDI Handle is not Null !");

        return;
    }

    /* ��CBTʹ�õ�USBͨ�� */
    g_cbtPortUDIHandle = mdrv_udi_open(&udiPara);
    if (g_cbtPortUDIHandle == VOS_ERROR) {
        mdrv_err("<CBTPPM_OamCbtPortDataInit> Open UDI ACM failed!");

        return;
    }

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOkNum++;
    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOkSlice = VOS_GetSlice();

    /* ����CBTʹ�õ�USBͨ������ */
    if (mdrv_udi_ioctl(g_cbtPortUDIHandle, ACM_IOCTL_RELLOC_READ_BUFF, &readBuffInfo) != VOS_OK) {
        mdrv_err("<CBTPPM_OamCbtPortDataInit> mdrv_udi_ioctl ACM_IOCTL_RELLOC_READ_BUFF Failed\n");

        return;
    }

    /* ע��OMʹ�õ�USB�����ݻص����� */
    if (mdrv_udi_ioctl(g_cbtPortUDIHandle, UDI_ACM_IOCTL_SET_READ_CB, readCb) != VOS_OK) {
        mdrv_err("<CBTPPM_OamCbtPortDataInit> mdrv_udi_ioctl UDI_ACM_IOCTL_SET_READ_CB Failed\n");

        return;
    }

    if (mdrv_udi_ioctl(g_cbtPortUDIHandle, ACM_IOCTL_SET_WRITE_CB, writeCb) != VOS_OK) {
        mdrv_err("<CBTPPM_OamCbtPortDataInit> mdrv_udi_ioctl ACM_IOCTL_SET_WRITE_CB Failed\n");

        return;
    }

    if (mdrv_udi_ioctl(g_cbtPortUDIHandle, ACM_IOCTL_SET_EVT_CB, stateCb) != VOS_OK) {
        mdrv_err("<CBTPPM_OamCbtPortDataInit> mdrv_udi_ioctl ACM_IOCTL_SET_EVT_CB Failed\n");

        return;
    }

    /* mdrv_udi_ioctl() ����������Ϊ0������COPY��Ϊ1��DO COPY���˴�����δ�ṩ��Ӧ�� */
    if (mdrv_udi_ioctl(g_cbtPortUDIHandle, ACM_IOCTL_WRITE_DO_COPY, (VOS_VOID *)1) != VOS_OK) {
        mdrv_err("<CBTPPM_OamCbtPortDataInit> mdrv_udi_ioctl ACM_IOCTL_WRITE_DO_COPY Failed\n");

        return;
    }

    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOk2Num++;
    g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOk2Slice = VOS_GetSlice();

    return;
}

/*
 * ��������: ����USB ��CBT�˿�ͨ���ĳ�ʼ��
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��25��
 *    �޸�����: Creat Function
 */
VOS_UINT32 CBTPPM_OamUsbCbtPortInit(VOS_VOID)
{
    VOS_UINT32 ret;

    /* CBT�˿���PCVOICE���ã���̬ע�����ݽ��պ��� */
    CBTCPM_PortRcvReg(CBTSCM_SoftDecodeDataRcv);

    /* ��̬ע�����ݷ��ͺ��� */
    CBTCPM_PortSndReg(CBTPPM_OamUsbCbtSendData);

    /* USB���ص�CBT�˿ڣ����õ�����첽�ӿڷ������ݣ�������Ҫα���ͬ���ӿڣ������ź��� */
    if (VOS_SmCCreate("UCBT", 0, VOS_SEMA4_FIFO, &g_cbtUsbPseudoSyncSemId) != VOS_OK) {
        mdrv_err("<CBTPPM_OamUsbCbtPortInit> create g_cbtUsbPseudoSyncSemId failedd\n");

        return VOS_ERR;
    }

    /* USB���ص�CBT�˿ڳ�ʼ��UDI�豸��� */
    g_cbtPortUDIHandle = VOS_ERROR;

    /* CBT�˿�ͨ��CBT��CPM������ע�������ͺ��� */
    ret = mdrv_usb_reg_enablecb((usb_udi_enable_cb)CBTPPM_OamUsbCbtPortOpen);
    CBT_PRINT_RETURN_VAL(ret, "CBTPPM_OamUsbCbtPortInit", "mdrv_usb_reg_enablecb");
    ret = mdrv_usb_reg_disablecb((usb_udi_disable_cb)CBTPPM_OamUsbCbtPortClose);
    CBT_PRINT_RETURN_VAL(ret, "CBTPPM_OamUsbCbtPortInit", "mdrv_usb_reg_disablecb");
    return VOS_OK;
}

/*
 * ��������: ��VCOM�˿ڶ�ȡCBT������
 * �������: ucDevIndex: ����˿�
 *           pData    : �յ�����
 *           ullength : ���ݳ���
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��26��
 *    �޸�����: Creat Function
 */
VOS_INT CBTPPM_OamVComCbtReadData(VOS_UINT8 devIndex, VOS_UINT8 *data, VOS_UINT32 length)
{
    CBTCPM_RCV_FUNC func = VOS_NULL_PTR;

    if (devIndex != APP_VCOM_DEV_INDEX_LOG) {
        mdrv_err("<CBTPPM_OamVComCbtReadData> PhyPort port is invalid, ucDevIndex=%d\n", devIndex);

        return VOS_ERR;
    }

    g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvNum++;
    g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvLen += length;

    if ((data == VOS_NULL_PTR) || (length == 0)) {
        mdrv_err("<CBTPPM_OamVComCbtReadData>  Send data is NULL\n");

        return VOS_ERR;
    }

    /*lint -e40*/
    CBT_AcpuPrintDbgInfo("CBTPPM_OamVComCbtReadData", (VOS_UINT8 *)data, length, CBT_ACPU_VCOM_CB);
    /*lint +e40*/

    func = CBTCPM_GetRcvFunc();
    /* ���ݽ��պ��� */
    if (func == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    if (func((VOS_UINT8 *)data, length) != VOS_OK) {
        g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvErrNum++;
        g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvErrLen += length;

        mdrv_err("<CBTPPM_OamVComCbtReadData> Info:Call CBT Data Rcv Func fail\n");

        return VOS_ERR;
    }

    return VOS_OK;
}

/*
 * ��������: ��VCOM�˿ڷ���CBT����
 * �������: pucVirAddr:   �������ַ
 *           pucPhyAddr:   ����ʵ��ַ
 *           dataLen:    ���ݳ���
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��26��
 *    �޸�����: Creat Function
 */
VOS_UINT32 CBTPPM_OamVComCbtSendData(VOS_UINT8 *virAddr, VOS_UINT32 dataLen)
{
    g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendNum++;
    g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendLen += dataLen;

    if (APP_VCOM_SEND(APP_VCOM_DEV_INDEX_LOG, virAddr, dataLen) != VOS_OK) {
        g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendErrNum++;
        g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendErrLen += dataLen;

        return VOS_ERR;
    }

    return VOS_OK;
}

/*
 * ��������: ���� Vcom CBT�˿ڵĳ�ʼ��
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��24��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamVComCbtPortInit(VOS_VOID)
{
    VOS_UINT32 ret;

    /* CBT�˿���PCVOICE���ã���̬ע�����ݽ��պ��� */
    CBTCPM_PortRcvReg(CBTSCM_SoftDecodeDataRcv);

    /* У׼��VCOM28�����������·������ݵĻظ�����CPM��ֱ�ӷ��� */
    ret = APP_VCOM_REG_DATA_CALLBACK(APP_VCOM_DEV_INDEX_LOG, CBTPPM_OamVComCbtReadData);
    CBT_PRINT_RETURN_VAL(ret, "CBTPPM_OamVComCbtPortInit", "APP_VCOM_REG_DATA_CALLBACK");
    CBTCPM_PortSndReg(CBTPPM_OamVComCbtSendData);

    return;
}

/*
 * ��������: CBT�˿ڳ�ʼ��
 * �޸���ʷ:
 * 1. ��    ��: 2014��5��25��
 *    �޸�����: V8R1 OM_Optimize��Ŀ����
 */
VOS_VOID CBTPPM_OamCbtPortInit(VOS_VOID)
{
    OM_ChannlePortCfg portCfg;
    VOS_UINT32 ret;

    /* ��ʼ������ */
    (VOS_VOID)memset_s(&g_cbtAcpuDebugInfo, sizeof(g_cbtAcpuDebugInfo), 0, sizeof(g_cbtAcpuDebugInfo));
    (VOS_VOID)memset_s(&portCfg, sizeof(portCfg), 0, sizeof(portCfg));

    /* ��ȡOM���������ͨ�� */
    if (mdrv_nv_read(NV_ITEM_OM_PORT_TYPE, &portCfg, sizeof(OM_ChannlePortCfg)) != NV_OK) {
        portCfg.cbtPortNum = CPM_CBT_PORT_VCOM;
    }

    if (portCfg.cbtPortNum == CPM_CBT_PORT_USB) {
        /* USB ���ص�CBT�˿ڵĳ�ʼ�� */
        ret = CBTPPM_OamUsbCbtPortInit();
        CBT_PRINT_RETURN_VAL(ret, "CBTPPM_OamCbtPortInit", "CBTPPM_OamUsbCbtPortInit");
    } else if (portCfg.cbtPortNum == CPM_CBT_PORT_SOCKET) {
        CBTPPM_SocketPortInit();
    }
    /* Ĭ�϶���VCOM��CBT */
    else {
        /* Vcom ��CBTͨ���ĳ�ʼ�� */
        CBTPPM_OamVComCbtPortInit();
    }

    return;
}

/*
 * ��������: ��ӡ������Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2017��7��13��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamUsbDebugInfoShow(VOS_VOID)
{
    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port usbOpenNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenNum);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUSBOpenSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUSBOpenOkNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOkNum);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUSBOpenOkSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOkSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUSBOpenOk2Num=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOk2Num);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUSBOpenOk2Slice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbOpenOk2Slice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbReadNum1=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadNum1);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbReadInSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadInSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbReadNum2=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadNum2);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbReadOutSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadOutSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port usbReadMaxTime=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadMaxTime);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbReadErrNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadErrNum);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbReadErrValue=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadErrValue);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbReadErrTime=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbReadErrTime);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbDecodeErrNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbDecodeErrNum);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteNum1=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteNum1);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteInSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteInSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteNum2=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteNum2);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteOutSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteOutSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port usbWriteMaxTime=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteMaxTime);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbPseudoSyncSemInSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemInSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbPseudoSyncSemOutSlice=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemOutSlice);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbPseudoSyncSemNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemNum);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbPseudoSyncSemErrNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbPseudoSyncSemErrNum);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteErrNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrNum);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteErrLen=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrLen);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteErrValue=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrValue);

    mdrv_debug("<CBTPPM_OamUsbDebugInfoShow> CBT Usb Port ulUsbWriteErrTime=%d.\n",
               g_cbtAcpuDebugInfo.cbtUsbAcpuDebugInfo.usbWriteErrTime);

    return;
}

/*
 * ��������: ��ӡ������Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2018��12��18��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_OamVComDebugInfoShow(VOS_VOID)
{
    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulVCOMSendSn=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendSn);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulVCOMSendNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendNum);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulVCOMSendLen=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendLen);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulVCOMSendErrNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendErrNum);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulVCOMSendErrLen=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomSendErrLen);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulVCOMRcvSn=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvSn);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulvcomRcvNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvNum);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulvcomRcvLen=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvLen);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulvcomRcvErrNum=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvErrNum);

    mdrv_debug("<CBTPPM_OamVComDebugInfoShow> CBT VCom Port  ulvcomRcvErrLen=%d.\n",
               g_cbtAcpuDebugInfo.cbtVcomAcpuDebugInfo.vcomRcvErrLen);
    return;
}

#if (defined(FEATURE_HISOCKET) && (FEATURE_HISOCKET == FEATURE_ON) && (VOS_WIN32 != VOS_OS_VER))

CBTPPM_SocketCtrlInfo g_cbtPpmSocketCtrlInfo;

CBTPPM_SocketDebugInfo g_cbtPpmSocketDebugInfo;

/*
 * ��������: socket�������󶨼���
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_SockBindListen(VOS_VOID)
{
    struct sockaddr_in addr;
    VOS_INT            addLen;
    VOS_UINT32         ret;

    if ((g_cbtPpmSocketCtrlInfo.listener != CBTPPM_SOCK_NULL) || (g_cbtPpmSocketCtrlInfo.socket != CBTPPM_SOCK_NULL)) {
        return;
    }

    g_cbtPpmSocketCtrlInfo.listener = CBT_socket(AF_INET, SOCK_STREAM, 0);

    if (g_cbtPpmSocketCtrlInfo.listener < VOS_OK) {
        mdrv_err("<CBTPPM_SockBindListen> socket fail !\n");
        g_cbtPpmSocketCtrlInfo.listener = CBTPPM_SOCK_NULL;
        ret = VOS_TaskDelay(100); /* delay 100 ms */
        CBT_PRINT_RETURN_VAL(ret, "CBTPPM_SockBindListen", "VOS_TaskDelay");
        return;
    }

    (VOS_VOID)memset_s(&addr, sizeof(addr), 0, sizeof(addr));
    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr = 0;

    /* �����Ķ˿ں� */
    addr.sin_port = htons(CBTPPM_SOCK_PORT_NUM);

    addLen = sizeof(struct sockaddr_in);

    /* ������Socket�󶨵���Ӧ�Ķ˿��� */
    if (CBT_bind((g_cbtPpmSocketCtrlInfo.listener), ((struct sockaddr *)&addr), (addLen)) == CBTPPM_SOCKET_ERROR) {
        CBT_closesocket(g_cbtPpmSocketCtrlInfo.listener);
        g_cbtPpmSocketCtrlInfo.listener = CBTPPM_SOCK_NULL;
        mdrv_err("<CBTPPM_SockBindListen> bind fail !\n");
        return;
    }

    /* ���÷������˼��������ͻ����� */
    if (CBT_listen(g_cbtPpmSocketCtrlInfo.listener, CBTPPM_SOCKET_NUM_MAX) == CBTPPM_SOCKET_ERROR) {
        CBT_closesocket(g_cbtPpmSocketCtrlInfo.listener);
        g_cbtPpmSocketCtrlInfo.listener = CBTPPM_SOCK_NULL;
        mdrv_err("<CBTPPM_SockBindListen> listen fail !\n");
        return;
    }
}

/*
 * ��������: ���ڴ���ͻ��˵����󣬽��������ύ���ϲ㴦��
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_SockAcceptRecv(VOS_VOID)
{
    CBT_Ip_fd_set      listen1;
    VOS_INT32          ret;
    VOS_INT32          len;
    struct sockaddr_in from;
    CBTPPM_SOCKET      socket;
    CBTPPM_SOCKET      maxSocket;
    CBTCPM_RCV_FUNC    func = VOS_NULL_PTR;
    VOS_UINT32         rslt;

    if ((g_cbtPpmSocketCtrlInfo.listener == CBTPPM_SOCK_NULL) && (g_cbtPpmSocketCtrlInfo.socket == CBTPPM_SOCK_NULL)) {
        mdrv_err("<CBTPPM_SockAcceptRecv> listener and socket fail !\n");
        return;
    }

    mdrv_sock_fd_zero(&listen1);

    /* HI_FD_SET�л�Ե�һ�������������ƣ���ǰlistenerΪint�ͣ��ᱨpclint�澯����˴˴�ǿתΪVOS_UINT32 */
    HI_FD_SET((VOS_UINT32)(g_cbtPpmSocketCtrlInfo.listener), &listen1);

    if (g_cbtPpmSocketCtrlInfo.socket != CBTPPM_SOCK_NULL) {
        /* HI_FD_SET�л�Ե�һ�������������ƣ���ǰlistenerΪint�ͣ��ᱨpclint�澯����˴˴�ǿתΪVOS_UINT32 */
        HI_FD_SET((VOS_UINT32)(g_cbtPpmSocketCtrlInfo.socket), &listen1);
    }

    maxSocket = g_cbtPpmSocketCtrlInfo.socket > g_cbtPpmSocketCtrlInfo.listener ? g_cbtPpmSocketCtrlInfo.socket
                                                                                : g_cbtPpmSocketCtrlInfo.listener;

    ret = CBT_select((int)maxSocket + 1, &listen1, NULL, NULL, NULL);
    if (ret < 0) {
        mdrv_err("<CBTPPM_SockAcceptRecv> select fail !\n");
        return;
    }
    /* HI_FD_SET�л�Ե�һ�������������ƣ���ǰlistenerΪint�ͣ��ᱨpclint�澯����˴˴�ǿתΪVOS_UINT32 */
    if ((g_cbtPpmSocketCtrlInfo.listener != CBTPPM_SOCK_NULL) &&
        (HI_FD_ISSET((VOS_UINT32)(g_cbtPpmSocketCtrlInfo.listener), &listen1))) {
        len    = sizeof(struct sockaddr_in);
        socket = CBT_accept((g_cbtPpmSocketCtrlInfo.listener), ((struct sockaddr *)&from), ((int *)&len));
        if (socket < 0) {
            CBT_closesocket(g_cbtPpmSocketCtrlInfo.listener);
            g_cbtPpmSocketCtrlInfo.listener = CBTPPM_SOCK_NULL;
            mdrv_err("<CBTPPM_SockAcceptRecv> accept fail !\n");

            return;
        }

        g_cbtPpmSocketDebugInfo.listernNum1++;

        /* a new socket reconnct */
        if (g_cbtPpmSocketCtrlInfo.socket != CBTPPM_SOCK_NULL) {
            rslt = VOS_SmP(g_cbtPpmSocketCtrlInfo.smClose, 0);
            CBT_PRINT_RETURN_VAL(rslt, "CBTPPM_SockAcceptRecv", "VOS_SmP1");
            CBT_closesocket(g_cbtPpmSocketCtrlInfo.socket);
            rslt = VOS_SmV(g_cbtPpmSocketCtrlInfo.smClose);
            CBT_PRINT_RETURN_VAL(rslt, "CBTPPM_SockAcceptRecv", "VOS_SmV1");
        }

        g_cbtPpmSocketDebugInfo.listernNum2++;

        rslt = VOS_SmP(g_cbtPpmSocketCtrlInfo.smClose, 0);
        CBT_PRINT_RETURN_VAL(rslt, "CBTPPM_SockAcceptRecv", "VOS_SmP2");
        g_cbtPpmSocketCtrlInfo.socket = socket;
        rslt = VOS_SmV(g_cbtPpmSocketCtrlInfo.smClose);
        CBT_PRINT_RETURN_VAL(rslt, "CBTPPM_SockAcceptRecv", "VOS_SmV2");
    }
    /* HI_FD_SET�л�Ե�һ�������������ƣ���ǰlistenerΪint�ͣ��ᱨpclint�澯����˴˴�ǿתΪVOS_UINT32 */
    if ((g_cbtPpmSocketCtrlInfo.socket != CBTPPM_SOCK_NULL) &&
        (HI_FD_ISSET((VOS_UINT32)(g_cbtPpmSocketCtrlInfo.socket), &listen1))) {
        ret = CBT_recv((int)g_cbtPpmSocketCtrlInfo.socket, g_cbtPpmSocketCtrlInfo.buf, CBTPPM_SOCK_MSG_LEN, 0);
        if (ret <= 0) { /* �ͻ��˶Ͽ�֮�����˻�����ܵ�����Ϊ0�����ݰ� */
            rslt = VOS_SmP(g_cbtPpmSocketCtrlInfo.smClose, 0);
            CBT_PRINT_RETURN_VAL(rslt, "CBTPPM_SockAcceptRecv", "VOS_SmP3");
            CBT_closesocket(g_cbtPpmSocketCtrlInfo.socket);
            g_cbtPpmSocketCtrlInfo.socket = CBTPPM_SOCK_NULL;
            rslt = VOS_SmV(g_cbtPpmSocketCtrlInfo.smClose);
            CBT_PRINT_RETURN_VAL(rslt, "CBTPPM_SockAcceptRecv", "VOS_SmV3");
            mdrv_err("<CBTPPM_SockAcceptRecv> rcv fail !\n");
            return;
        }

        g_cbtPpmSocketDebugInfo.rcvData += ret;

        /* �����յ��������ύ���ϲ㴦�� */
        func = CBTCPM_GetRcvFunc();
        if (func != VOS_NULL_PTR) {
            if (func((VOS_UINT8 *)g_cbtPpmSocketCtrlInfo.buf, ret) != VOS_OK) {
                /* ���ӿ�ά�ɲ���� */
                g_cbtPpmSocketDebugInfo.failToProcess++;
            }
        }
    }
}

/*
 * ��������: socket�������̣߳���������������˺Ϳͻ��˵���������
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_SocketServerTask(VOS_VOID)
{
    for (;;) {
        CBTPPM_SockBindListen();
        CBTPPM_SockAcceptRecv();
    }
}

/*
 * ��������: socket�����Ƿ�֧��
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */

VOS_BOOL CBTPPM_SockIsEnable(VOS_VOID)
{
    OM_ChannlePortCfg portCfg;

    (VOS_VOID)memset_s(&portCfg, sizeof(portCfg), 0, sizeof(portCfg));
    /* ��ȡOM���������ͨ�� */
    if (mdrv_nv_read(NV_ITEM_OM_PORT_TYPE, &portCfg, sizeof(OM_ChannlePortCfg)) != NV_OK) {
        portCfg.cbtPortNum = CPM_CBT_PORT_VCOM;
    }

    if (portCfg.cbtPortNum != CPM_CBT_PORT_SOCKET) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*
 * ��������: ��ʼ��socket��������
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_UINT32 CBTPPM_SocketTaskInit(VOS_VOID)
{
    VOS_UINT32 relVal;

    if ((mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_SUPPORT) && (CBTPPM_SockIsEnable() == VOS_TRUE)) {
        g_cbtPpmSocketCtrlInfo.listener = CBTPPM_SOCK_NULL;

        g_cbtPpmSocketCtrlInfo.socket = CBTPPM_SOCK_NULL;

        (VOS_VOID)memset_s(&g_cbtPpmSocketDebugInfo, sizeof(g_cbtPpmSocketDebugInfo), 0,
                           sizeof(g_cbtPpmSocketDebugInfo));

        if (VOS_SmMCreate("CBTSOCK", VOS_SEMA4_PRIOR | VOS_SEMA4_INVERSION_SAFE, &(g_cbtPpmSocketCtrlInfo.smClose))
            != VOS_OK) {
            return VOS_ERR;
        }

        /* ����SOCKET���ݵ��Դ������� */
        relVal = VOS_RegisterSelfTaskPrio(ACPU_FID_CBT, (VOS_TaskEntryType)CBTPPM_SocketServerTask,
                                          COMM_SOCK_SELFTASK_PRIO, CBT_SOCKET_TASK_STACK_SIZE);
        if (relVal == VOS_NULL_BYTE) {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*
 * ��������: ��socket����CBT����
 * �������: pucVirAddr �������ַ
 *           pucPhyAddr ����ʵ��ַ
 *           dataLen ���ݳ���
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_UINT32 CBTPPM_SocketSendData(VOS_UINT8 *virAddr, VOS_UINT32 dataLen)
{
    CBTPPM_SOCKET socket;
    VOS_INT       nSndNum;
    VOS_UINT32    ret;

    ret = VOS_SmP(g_cbtPpmSocketCtrlInfo.smClose, 0);
    CBT_PRINT_RETURN_VAL(ret, "CBTPPM_SocketSendData", "VOS_SmP");
    socket = g_cbtPpmSocketCtrlInfo.socket;

    if (socket == CBTPPM_SOCK_NULL) {
        ret = VOS_SmV(g_cbtPpmSocketCtrlInfo.smClose);
        CBT_PRINT_RETURN_VAL(ret, "CBTPPM_SocketSendData", "VOS_SmV1");
        mdrv_err("<CBTPPM_SocketSendData>  socket fail !\n");
        return VOS_ERR;
    }

    /* ����send������ͨ��socket���ͳ�ȥ����TCP */
    nSndNum = CBT_send(socket, virAddr, dataLen, 0);

    ret = VOS_SmV(g_cbtPpmSocketCtrlInfo.smClose);
    CBT_PRINT_RETURN_VAL(ret, "CBTPPM_SocketSendData", "VOS_SmV2");
    if (nSndNum != dataLen) {
        g_cbtPpmSocketDebugInfo.failToSend++;

        return VOS_ERR;
    }

    g_cbtPpmSocketDebugInfo.sndData += dataLen;

    return VOS_OK;
}

/*
 * ��������: ����socket��CBT�˿�ͨ���ĳ�ʼ��
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_SocketPortInit(VOS_VOID)
{
    /* CBT�˿���PCVOICE���ã���̬ע�����ݽ��պ��� */
    CBTCPM_PortRcvReg(CBTSCM_SoftDecodeDataRcv);

    CBTCPM_PortSndReg(CBTPPM_SocketSendData);

    return;
}

/*
 * ��������: ��ӡ������Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_SocketCtrlInfoShow(VOS_VOID)
{
    mdrv_debug("<CBTPPM_SocketCtrlInfoShow> CBTPPM listen=%d.\n", g_cbtPpmSocketCtrlInfo.listener);

    mdrv_debug("<CBTPPM_SocketCtrlInfoShow> CBTPPM socket=%d.\n", g_cbtPpmSocketCtrlInfo.socket);

    return;
}

/*
 * ��������: ��ӡ������Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_VOID CBTPPM_SocketDebugInfoShow(VOS_VOID)
{
    mdrv_debug("<CBTPPM_SocketDebugInfoShow> CBTPPM ulListernNum1=%d.\n", g_cbtPpmSocketDebugInfo.listernNum1);

    mdrv_debug("<CBTPPM_SocketDebugInfoShow> CBTPPM ulListernNum2=%d.\n", g_cbtPpmSocketDebugInfo.listernNum2);

    mdrv_debug("<CBTPPM_SocketDebugInfoShow> CBTPPM ulRcvData=%d.\n", g_cbtPpmSocketDebugInfo.rcvData);

    mdrv_debug("<CBTPPM_SocketDebugInfoShow> CBTPPM ulFailToProcess=%d.\n", g_cbtPpmSocketDebugInfo.failToProcess);

    mdrv_debug("<CBTPPM_SocketDebugInfoShow> CBTPPM ulSndData=%d.\n", g_cbtPpmSocketDebugInfo.sndData);

    mdrv_debug("<CBTPPM_SocketDebugInfoShow> CBTPPM ulFailToSend=%d.\n", g_cbtPpmSocketDebugInfo.failToSend);

    return;
}

#else

/*
 * ��������: CBT�˿ڳ�ʼ��
 * �޸���ʷ:
 * 1. ��    ��: 2015��12��21��
 *    �޸�����: Creat Function
 */
VOS_UINT32 CBTPPM_SocketTaskInit(VOS_VOID)
{
    return VOS_OK;
}

VOS_VOID CBTPPM_SocketPortInit(VOS_VOID)
{
    return;
}

#endif

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
