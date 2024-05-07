/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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
#include "cmux_drv_ifc.h"
#include "cmux_framer.h"
#include "cmux_api_ifc.h"
#include "cmux_tgt_osal.h"
#include "mdrv_hsuart.h"
#include "mdrv_usb.h"
#include "cmux_udi_al.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_IOT_CMUX == FEATURE_ON)
extern VOS_UINT32 DMS_PORT_CmuxPhyPortIsHsuart(VOS_VOID);

/* �������� */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* ���CMUX��handle */
int g_CmuxUdiHandle = UDI_INVALID_HANDLE; /*lint !e129*/

/* ���cmȫ�ֱ��� */
struct cmuxlib* g_Cmux = NULL;

struct delayed_work g_at_cmux_expiry_timer;

unsigned char          g_CmuxClosePort = FALSE;
static unsigned int    g_cmux_baud[]    = { 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600,
                                          1000000, 1152000, 1500000, 2000000, 2500000, 3000000 };

/* CMUX���õ�Ĭ��ֵ(Э�������ͬ�ƶ�):
operating_mode                  < dft = 0 >
subset                          < dft = 0 >
max_cmd_num_tx_times_N2         < dft = 3 >
window_size_k                   < dft = 2 >
port_speed                      < dft = 5 >
max_frame_size_N1               < dft = 31 >
response_timer_T1               < dft = 100 >
response_timer_T2               < dft = 900 >
response_timer_T3               < dft = 10 > */
cmux_info_type g_cmux_info = {0,0,3,2,5,31,100,900,10};


cmux_info_type* cmux_get_cmux_info_type(VOS_VOID)
{
    return &g_cmux_info;
}

/* Description: ����gCmuxClosePortȫ�ֱ���ֵ(����balongƽ̨) */
void set_cmux_close_port(unsigned char value)
{
    g_CmuxClosePort = value;
}

/* Description:    AT+CMUX��ʱ��Ļص�����(����balongƽ̨) */
void cmux_at_timer_expiry_cb(struct work_struct* w)
{
    cmux_close_port();
}

/* Description:    CMUXģ���ʼ��������CMUXģʽ(����balongƽ̨) */
int cmux_init_port(cmux_info_type* CmuxInfo, unsigned int cmuxPhyPortIsHsuart)
{
    struct cmux_callback cmux_cb; /*lint !e565*/
    unsigned char        response[] = "\r\nOK\r\n";
    int                  ret;
    struct cmuxlib*      cmux;

    /* ��ʼ��cmuxlib�����Ѳ���cm�浽ȫ�ֱ���g_cmux���� */
    cmux = cmux_cmuxlib_init(CMUX_UEMODE, CmuxInfo);
    cmux_set_g_cmux(cmux);

    if (cmux == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "lib init error\n");
        return VOS_FALSE;
    }

    cmux_cb.callback_func = cmux_callback_func;
#if (defined(LLT_OS_VER))
    cmux_cb.send_to_com = send_to_com; /* dmt��׮���� */
#else
    if (cmuxPhyPortIsHsuart == TRUE) {
        cmux_cb.send_dl_data = cmux_hsuart_send_dldata; /*lint !e64*/
    } else {
        cmux_cb.send_dl_data = cmux_uart_send_dldata; /*lint !e64*/
    }
#endif

    cmux_cb.set_power_cb = cmux_hsuart_set_power_status;

    /* ���ûص����������ڷ������� */
    ret = cmux_register_callback(cmux, &cmux_cb);
    if (ret == FAIL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "register callback error\n");
        return VOS_FALSE;
    }

    /* ��CMUX��HSUART�˿� */
    cmux_open_device(cmuxPhyPortIsHsuart);

    /* ����AT+CMUX����Ӧ:ok */
    if (cmuxPhyPortIsHsuart == TRUE) {
        (void)cmux_hsuart_send_dldata(response, (unsigned short)(strlen(response) + 1)); /*lint !e64*/
    } else {
        (void)cmux_uart_send_dldata(response, (unsigned short)(strlen(response) + 1)); /*lint !e64*/
    }

    /* ��ʼ��AT+CMUX��ʱ��ʱ�� */
    cmux_init_timer(&g_at_cmux_expiry_timer, (void *)&cmux_at_timer_expiry_cb);

    /* ������ʱ����10����δ�յ�SABM 0֡���Զ�������ͨģʽ */
    cmux_timer_begin(&g_at_cmux_expiry_timer, AT_CMUX_EXPIRY_TIME);

    return VOS_TRUE;
}

/* Description: �˳�CMUXģʽ���ָ�ATģʽ(����balongƽ̨) */
void cmux_close_port(void)
{
    cmux_close_cb_t  cmux_close_cb;
    struct cmuxlib  *cmux;

    cmux_close_cb = cmux_get_g_close_port_cb();
    cmux = cmux_get_g_cmux();

    /* �ر�CMUX��HSUART��ͨ�� */
    cmux_close_device();

    /* �ر�CMUX��AT��ͨ��,�ָ�AT��HSUART��ͨ�� */
    if (cmux_close_cb != NULL) {
        cmux_close_cb();
    }

    /* �ر�ͨ�����ͷ�pending_frame */
    if (cmux != NULL) {
        if (cmux->cmux_if.pending_frame != NULL) {
            cmux_free((unsigned char*)cmux->cmux_if.pending_frame);
        }

        cmux_free((unsigned char*)cmux);
        cmux_set_g_cmux(NULL);
    }
}

/* Description:    ��ȡHSUART����������buffer���͵�CMUX lib����(����balongƽ̨) */
void cmux_uart_uldata_read_cb(void)
{
    /* �������ָ������ݳ��� */
    unsigned char   *pucData = NULL;
    unsigned int     ulLen = 0;
    struct cmuxlib  *cmux;

    cmux = cmux_get_g_cmux();

    /* ������buffer����ȡ���� */
    if (cmux_uart_get_uldata_buff(&pucData, &ulLen) == PASS) {

        (void)cmux_Input_frame_to_cmux(cmux, pucData, (unsigned short)ulLen);

        /* ���ݴ����꣬�ͷ� */
        (void)cmux_uart_free_uldata_buff(pucData);

        if (g_CmuxClosePort == TRUE) {
            g_CmuxClosePort = FALSE;
            cmux_close_port();
        }
    }

    return;
}

void cmux_hsuart_uldata_read_cb(void)
{
    /* �������ָ������ݳ��� */
    unsigned char   *pucData = NULL;
    unsigned char   *pData = NULL;
    unsigned int     ulLen = 0;
    struct cmuxlib  *cmux;

    cmux = cmux_get_g_cmux();

    /* ������buffer����ȡ���� */
    if (cmux_hsuart_get_uldata_buff(&pucData, &ulLen) == PASS) {
        /* �����豸��ǰģʽ���ַ��������� */
        pData = cmux_get_skb((unsigned char *)pucData);

        if (pData == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "pData is NULL\n");
        }

        (void)cmux_Input_frame_to_cmux(cmux, (unsigned char *)pData, ulLen);

        /* ���ݴ����꣬�ͷ� */
        (void)cmux_hsuart_free_uldata_buff(pucData);

        if (g_CmuxClosePort == TRUE) {
            g_CmuxClosePort = FALSE;
            cmux_close_port();
        }
    }
}

/* Description:    ����IOCTL������buffer��ȡ����(����balongƽ̨) */
unsigned int cmux_uart_get_uldata_buff(unsigned char** pucData, unsigned int* ulLen)
{
    struct acm_wr_async_info        stCtlParam;
    int                             lUdiHandle = g_CmuxUdiHandle;
    signed int                      lResult;

    /* ���UDI�����Ч�� */
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* ��ȡ������������BUFFER */
    stCtlParam.virt_addr = NULL;
    stCtlParam.phy_addr  = NULL;
    stCtlParam.size      = 0;
    stCtlParam.priv      = NULL;

    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_GET_RD_BUFF, &stCtlParam);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Get buffer failed!\n");
        return FAIL;
    }

    /* ������Ч�Լ�� */
    if ((stCtlParam.virt_addr == NULL) || (stCtlParam.size == 0)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Data buffer error!\n");
        return FAIL;
    }

    *pucData = (unsigned char*)stCtlParam.virt_addr;
    *ulLen   = stCtlParam.size;

    return PASS;
}

unsigned int cmux_hsuart_get_uldata_buff(unsigned char** pucData, unsigned int* ulLen)
{
    cmux_hsuart_wr_async_info           stCtlParam;
    int                                 lUdiHandle;
    signed int                          lResult;

    /* ���UDI�����Ч�� */
    lUdiHandle = g_CmuxUdiHandle;
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* ��ȡ������������BUFFER */
    stCtlParam.pBuffer  = NULL;
    stCtlParam.u32Size  = 0;
    stCtlParam.pDrvPriv = NULL;

    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_GET_RD_BUFF, &stCtlParam);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Get buffer failed!\n");
        return FAIL;
    }

    /* ������Ч�Լ�� */
    if ((stCtlParam.pBuffer == NULL) || (stCtlParam.u32Size == 0)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Data buffer error!\n");
        return FAIL;
    }

    *pucData = stCtlParam.pBuffer;
    *ulLen   = stCtlParam.u32Size;

    return PASS;
}

/* Description:    ���������ڴ��ͷ�(����balongƽ̨) */
void cmux_hsuart_free_dldata_buff(char* pucData)
{
    /* �ͷ��ڴ�ĺ��� */
    if (pucData != NULL) {
        cmux_free_skb((unsigned char*)pucData);
    }
}

/* Description:    ���������ڴ��ͷ�(����balongƽ̨) */
unsigned int cmux_uart_free_uldata_buff(unsigned char* pucData)
{
    struct acm_wr_async_info ctl_param;
    int                      lUdiHandle = g_CmuxUdiHandle;
    signed int               lResult;

    /* ���UDI�����Ч�� */
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* ��д���ͷŵ��ڴ��ַ */
    ctl_param.virt_addr = (char*)pucData;
    ctl_param.phy_addr  = NULL;
    ctl_param.size      = 0;
    ctl_param.priv      = NULL;

    /* ����ִ���ͷ��ڴ���� */
    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_RETURN_BUFF, &ctl_param);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Return buffer failed!\n");
        cmux_free(pucData);
        return FAIL;
    }

    return PASS;
}

unsigned int cmux_hsuart_free_uldata_buff(unsigned char* pucData)
{
    cmux_hsuart_wr_async_info ctl_param;
    int                      lUdiHandle;
    signed int               lResult;

    /* ���UDI�����Ч�� */
    lUdiHandle = g_CmuxUdiHandle;
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* ��д���ͷŵ��ڴ��ַ */
    ctl_param.pBuffer  = pucData;
    ctl_param.u32Size  = 0;
    ctl_param.pDrvPriv = NULL;

    /* ����ִ���ͷ��ڴ���� */
    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_RETURN_BUFF, &ctl_param);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Return buffer failed!\n");
        cmux_free(pucData);
        return FAIL;
    }

    return PASS;
}

/* Description:    ��CMUX��HSUART�豸,ע��ص�����(����balongƽ̨) */
void cmux_open_device(unsigned int cmuxPhyPortIsHsuart)
{
    struct udi_open_param stParam;
    int                   lUdiHandle;

    stParam.devid = UDI_UART_0_ID;
    if (cmuxPhyPortIsHsuart == TRUE) {
        stParam.devid = UDI_HSUART_0_ID;
    }

    /* ��Device�����ID */
    lUdiHandle = mdrv_udi_open(&stParam);
    if (lUdiHandle != UDI_INVALID_HANDLE) {
        /* ע��UART�豸�������ݽ��ջص� */
        if (cmuxPhyPortIsHsuart == TRUE) {
            if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_READ_CB, cmux_hsuart_uldata_read_cb) != CMUX_SUCCESS) {
                CMUX_PRINT(CMUX_DEBUG_ERR, "Register Read Callback Error!\n");
            }
        } else {
            if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_READ_CB, cmux_uart_uldata_read_cb) != CMUX_SUCCESS) {
                CMUX_PRINT(CMUX_DEBUG_ERR, "Register Read Callback Error!\n");
            }
        }

        /* ע��UART���������ڴ��ͷŽӿ� */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_FREE_CB, cmux_hsuart_free_dldata_buff) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Register Free Callback Error!\n");
        }

        /* ע��ܽ��ź�֪ͨ�ص� */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_MSC_READ_CB, cmux_hsuart_msc_read_cb) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Set Msc Read Callback Error!\n");
        }
    }

    g_CmuxUdiHandle = lUdiHandle;
}

/* Description:    �ر�CMUX��HSUART�Ľӿ�(����balongƽ̨) */
void cmux_close_device(void)
{
    int lUdiHandle = g_CmuxUdiHandle;

    /* �ͷ���ػص� */
    if (lUdiHandle != UDI_INVALID_HANDLE) {
        /* ע��UART�豸�������ݽ��ջص� */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_READ_CB, NULL) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Free Read Callback Error!\n");
        }

        /* ע��UART���������ڴ��ͷŽӿ� */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_FREE_CB, NULL) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Free Free Callback Error!\n");
        }

        /* ע��ܽ��ź�֪ͨ�ص� */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_MSC_READ_CB, NULL) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Free Msc Read Callback Error!\n");
        }
    }

    /* �ر�Device */
    if (lUdiHandle != UDI_INVALID_HANDLE) {
        if (mdrv_udi_close(lUdiHandle) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Close CMUX device failed!\r\n");
        }
    }
}

/* Description:    ��cmux�����ݷ��͸���������(����balongƽ̨) */
unsigned int cmux_uart_write_data_async(unsigned char* pucData, unsigned int ulLen)
{
    int        lUdiHandle;
    signed int lResult;

    /* ���������Ч�� */
    if ((pucData == NULL) || (ulLen == 0)) {
        return FAIL;
    }

    /* ���UDI�����Ч�� */
    lUdiHandle = g_CmuxUdiHandle;
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\r\n");
        cmux_hsuart_free_dldata_buff(pucData);
        return FAIL;
    }

    lResult = mdrv_udi_write(lUdiHandle, pucData, ulLen);

    return PASS;
}

unsigned int cmux_hsuart_write_data_async(char* data)
{
    cmux_hsuart_wr_async_info           stCtlParam;
    int                                 lUdiHandle;
    signed int                          lResult;

    /* ���UDI�����Ч�� */
    lUdiHandle = g_CmuxUdiHandle;
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\r\n");
        cmux_hsuart_free_dldata_buff(data);
        return FAIL;
    }

    /* ��д�������ڴ� */
    stCtlParam.pBuffer  = data;
    stCtlParam.u32Size  = 0;
    stCtlParam.pDrvPriv = NULL;

    /* �첽��ʽд�� */
    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_WRITE_ASYNC, &stCtlParam);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Write failed!\r\n");
        cmux_hsuart_free_dldata_buff(data);
        return FAIL;
    }

    return PASS;
}

/* Description:    CMUX lib��HSUART�������ݽӿ�(����balongƽ̨) */
unsigned int cmux_uart_send_dldata(unsigned char* pucData, unsigned short usLen)
{
    unsigned int ulResult;

    /* �첽дHSUART�豸, д�ɹ����ڴ��ɵ������ͷ� */
    ulResult = cmux_uart_write_data_async(pucData, usLen);

    return ulResult;
}

unsigned int cmux_hsuart_send_dldata(unsigned char* pucData, unsigned short usLen)
{
    unsigned char *putData = NULL;
    unsigned int ulResult;

    /* ����ռ� */
    putData = cmux_alloc_skb((int)usLen);
    if (putData == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "putData null error!\r\n");
        return FAIL;
    }

    /* �������� */
    cmux_put_skb(putData, pucData, (int)usLen);

    /* �첽дHSUART�豸, д�ɹ����ڴ��ɵ������ͷ� */
    ulResult = cmux_hsuart_write_data_async((char *)putData);

    return ulResult;
}

/* Description:    CMUX lib����HSUART˯��ͶƱ�ӿ�(����balongƽ̨) */
unsigned int cmux_hsuart_set_power_status(void)
{
    signed int lResult;
    int        lUdiHandle = g_CmuxUdiHandle;

    /* ���UDI�����Ч�� */
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\r\n");
        return FAIL;
    }

    lResult = mdrv_udi_ioctl(lUdiHandle, UDI_UART_IOCTL_SLEEP_UNLOCK, NULL);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "set hsuart sleep failed!\r\n");
        return FAIL;
    }

    return PASS;
}

/* Description:    CMUX lib��HSUART�ܽ��ź�д�ӿ�(����balongƽ̨) */
unsigned int cmux_hsuart_write_msc_cmd(cmux_modem_msc_stru* pstDceMsc)
{
    int        lUdiHandle = g_CmuxUdiHandle;
    signed int lResult;

    /* ���UDI�����Ч�� */
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\r\n");
        return FAIL;
    }

    /* д�ܽ��ź� */
    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_MSC_WRITE_CMD, pstDceMsc);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Write failed!\r\n");
        return FAIL;
    }

    return PASS;
}

/* Description:    HSUARTӲ���ܽ��źŶ��ص� */
void cmux_hsuart_msc_read_cb(void* pParam)
{
    unsigned char        DlcIndex;
    cmux_modem_msc_stru* pstDceMsc = NULL;

    /* ��μ�� */
    if (pParam == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "pstDceMsc is NULL!\r\n");
        return;
    }
    pstDceMsc = (cmux_modem_msc_stru*)pParam;

    /* Ĭ����DLC 0ͨ������ */
    DlcIndex = CMUX_UDI_DLC_CTL;

    /* ��AT�෢��msc״̬��Ϣ */
    CMUX_PRINT(CMUX_DEBUG_INFO, "DlcIndex is %d, OP_Dtr is %d, ucDtr is %d!\r\n", DlcIndex, pstDceMsc->OP_Dtr,
               pstDceMsc->ucDtr);
    cmux_send_msc_status(DlcIndex, pstDceMsc);
}

/* Description:    �жϲ������Ƿ���CMUX֧�ַ�Χ */
unsigned int cmux_baud_to_num(unsigned int enBaudRate)
{
    unsigned int indexNum = CMUX_PHY_PORT_SPEED_INVALID;

    for (indexNum = CMUX_PHY_PORT_SPEED_INVALID; indexNum < CMUX_PHY_PORT_SPEED_14; indexNum++) {
        if (g_cmux_baud[indexNum] == enBaudRate) {
            return (indexNum + 1);
        }
    }

    return CMUX_PHY_PORT_SPEED_INVALID;
}
struct cmuxlib* cmux_get_g_cmux(void)
{
    return g_Cmux;
}
void cmux_set_g_cmux(struct cmuxlib* cmux)
{
    g_Cmux = cmux;
}

struct delayed_work cmux_get_g_at_cmux_expiry_timer(void)
{
    return g_at_cmux_expiry_timer;
}
void cmux_set_g_at_cmux_expiry_timer(struct delayed_work cmux_expiry_timer)
{
    g_at_cmux_expiry_timer = cmux_expiry_timer;
}

#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */