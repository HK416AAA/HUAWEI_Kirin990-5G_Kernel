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

/* 变量声明 */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* 存放CMUX的handle */
int g_CmuxUdiHandle = UDI_INVALID_HANDLE; /*lint !e129*/

/* 存放cm全局变量 */
struct cmuxlib* g_Cmux = NULL;

struct delayed_work g_at_cmux_expiry_timer;

unsigned char          g_CmuxClosePort = FALSE;
static unsigned int    g_cmux_baud[]    = { 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600,
                                          1000000, 1152000, 1500000, 2000000, 2500000, 3000000 };

/* CMUX配置的默认值(协议和需求共同制定):
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

/* Description: 设置gCmuxClosePort全局变量值(适配balong平台) */
void set_cmux_close_port(unsigned char value)
{
    g_CmuxClosePort = value;
}

/* Description:    AT+CMUX超时后的回调函数(适配balong平台) */
void cmux_at_timer_expiry_cb(struct work_struct* w)
{
    cmux_close_port();
}

/* Description:    CMUX模块初始化，开启CMUX模式(适配balong平台) */
int cmux_init_port(cmux_info_type* CmuxInfo, unsigned int cmuxPhyPortIsHsuart)
{
    struct cmux_callback cmux_cb; /*lint !e565*/
    unsigned char        response[] = "\r\nOK\r\n";
    int                  ret;
    struct cmuxlib*      cmux;

    /* 初始化cmuxlib，并把参数cm存到全局变量g_cmux里面 */
    cmux = cmux_cmuxlib_init(CMUX_UEMODE, CmuxInfo);
    cmux_set_g_cmux(cmux);

    if (cmux == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "lib init error\n");
        return VOS_FALSE;
    }

    cmux_cb.callback_func = cmux_callback_func;
#if (defined(LLT_OS_VER))
    cmux_cb.send_to_com = send_to_com; /* dmt打桩函数 */
#else
    if (cmuxPhyPortIsHsuart == TRUE) {
        cmux_cb.send_dl_data = cmux_hsuart_send_dldata; /*lint !e64*/
    } else {
        cmux_cb.send_dl_data = cmux_uart_send_dldata; /*lint !e64*/
    }
#endif

    cmux_cb.set_power_cb = cmux_hsuart_set_power_status;

    /* 设置回调函数，用于发送数据 */
    ret = cmux_register_callback(cmux, &cmux_cb);
    if (ret == FAIL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "register callback error\n");
        return VOS_FALSE;
    }

    /* 打开CMUX与HSUART端口 */
    cmux_open_device(cmuxPhyPortIsHsuart);

    /* 发送AT+CMUX的响应:ok */
    if (cmuxPhyPortIsHsuart == TRUE) {
        (void)cmux_hsuart_send_dldata(response, (unsigned short)(strlen(response) + 1)); /*lint !e64*/
    } else {
        (void)cmux_uart_send_dldata(response, (unsigned short)(strlen(response) + 1)); /*lint !e64*/
    }

    /* 初始化AT+CMUX超时定时器 */
    cmux_init_timer(&g_at_cmux_expiry_timer, (void *)&cmux_at_timer_expiry_cb);

    /* 启动定时器，10秒内未收到SABM 0帧，自动返回普通模式 */
    cmux_timer_begin(&g_at_cmux_expiry_timer, AT_CMUX_EXPIRY_TIME);

    return VOS_TRUE;
}

/* Description: 退出CMUX模式，恢复AT模式(适配balong平台) */
void cmux_close_port(void)
{
    cmux_close_cb_t  cmux_close_cb;
    struct cmuxlib  *cmux;

    cmux_close_cb = cmux_get_g_close_port_cb();
    cmux = cmux_get_g_cmux();

    /* 关闭CMUX与HSUART的通道 */
    cmux_close_device();

    /* 关闭CMUX与AT的通道,恢复AT与HSUART的通道 */
    if (cmux_close_cb != NULL) {
        cmux_close_cb();
    }

    /* 关闭通道，释放pending_frame */
    if (cmux != NULL) {
        if (cmux->cmux_if.pending_frame != NULL) {
            cmux_free((unsigned char*)cmux->cmux_if.pending_frame);
        }

        cmux_free((unsigned char*)cmux);
        cmux_set_g_cmux(NULL);
    }
}

/* Description:    获取HSUART的上行数据buffer，送到CMUX lib处理(适配balong平台) */
void cmux_uart_uldata_read_cb(void)
{
    /* 存放数据指针和数据长度 */
    unsigned char   *pucData = NULL;
    unsigned int     ulLen = 0;
    struct cmuxlib  *cmux;

    cmux = cmux_get_g_cmux();

    /* 从上行buffer里面取数据 */
    if (cmux_uart_get_uldata_buff(&pucData, &ulLen) == PASS) {

        (void)cmux_Input_frame_to_cmux(cmux, pucData, (unsigned short)ulLen);

        /* 数据传输完，释放 */
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
    /* 存放数据指针和数据长度 */
    unsigned char   *pucData = NULL;
    unsigned char   *pData = NULL;
    unsigned int     ulLen = 0;
    struct cmuxlib  *cmux;

    cmux = cmux_get_g_cmux();

    /* 从上行buffer里面取数据 */
    if (cmux_hsuart_get_uldata_buff(&pucData, &ulLen) == PASS) {
        /* 根据设备当前模式，分发上行数据 */
        pData = cmux_get_skb((unsigned char *)pucData);

        if (pData == NULL) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "pData is NULL\n");
        }

        (void)cmux_Input_frame_to_cmux(cmux, (unsigned char *)pData, ulLen);

        /* 数据传输完，释放 */
        (void)cmux_hsuart_free_uldata_buff(pucData);

        if (g_CmuxClosePort == TRUE) {
            g_CmuxClosePort = FALSE;
            cmux_close_port();
        }
    }
}

/* Description:    调用IOCTL从上行buffer中取数据(适配balong平台) */
unsigned int cmux_uart_get_uldata_buff(unsigned char** pucData, unsigned int* ulLen)
{
    struct acm_wr_async_info        stCtlParam;
    int                             lUdiHandle = g_CmuxUdiHandle;
    signed int                      lResult;

    /* 检查UDI句柄有效性 */
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* 获取底软上行数据BUFFER */
    stCtlParam.virt_addr = NULL;
    stCtlParam.phy_addr  = NULL;
    stCtlParam.size      = 0;
    stCtlParam.priv      = NULL;

    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_GET_RD_BUFF, &stCtlParam);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Get buffer failed!\n");
        return FAIL;
    }

    /* 数据有效性检查 */
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

    /* 检查UDI句柄有效性 */
    lUdiHandle = g_CmuxUdiHandle;
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* 获取底软上行数据BUFFER */
    stCtlParam.pBuffer  = NULL;
    stCtlParam.u32Size  = 0;
    stCtlParam.pDrvPriv = NULL;

    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_GET_RD_BUFF, &stCtlParam);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Get buffer failed!\n");
        return FAIL;
    }

    /* 数据有效性检查 */
    if ((stCtlParam.pBuffer == NULL) || (stCtlParam.u32Size == 0)) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Data buffer error!\n");
        return FAIL;
    }

    *pucData = stCtlParam.pBuffer;
    *ulLen   = stCtlParam.u32Size;

    return PASS;
}

/* Description:    下行数据内存释放(适配balong平台) */
void cmux_hsuart_free_dldata_buff(char* pucData)
{
    /* 释放内存的函数 */
    if (pucData != NULL) {
        cmux_free_skb((unsigned char*)pucData);
    }
}

/* Description:    上行数据内存释放(适配balong平台) */
unsigned int cmux_uart_free_uldata_buff(unsigned char* pucData)
{
    struct acm_wr_async_info ctl_param;
    int                      lUdiHandle = g_CmuxUdiHandle;
    signed int               lResult;

    /* 检查UDI句柄有效性 */
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* 填写待释放的内存地址 */
    ctl_param.virt_addr = (char*)pucData;
    ctl_param.phy_addr  = NULL;
    ctl_param.size      = 0;
    ctl_param.priv      = NULL;

    /* 底软执行释放内存操作 */
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

    /* 检查UDI句柄有效性 */
    lUdiHandle = g_CmuxUdiHandle;
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\n");
        return FAIL;
    }

    /* 填写待释放的内存地址 */
    ctl_param.pBuffer  = pucData;
    ctl_param.u32Size  = 0;
    ctl_param.pDrvPriv = NULL;

    /* 底软执行释放内存操作 */
    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_RETURN_BUFF, &ctl_param);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Return buffer failed!\n");
        cmux_free(pucData);
        return FAIL;
    }

    return PASS;
}

/* Description:    打开CMUX和HSUART设备,注册回调函数(适配balong平台) */
void cmux_open_device(unsigned int cmuxPhyPortIsHsuart)
{
    struct udi_open_param stParam;
    int                   lUdiHandle;

    stParam.devid = UDI_UART_0_ID;
    if (cmuxPhyPortIsHsuart == TRUE) {
        stParam.devid = UDI_HSUART_0_ID;
    }

    /* 打开Device，获得ID */
    lUdiHandle = mdrv_udi_open(&stParam);
    if (lUdiHandle != UDI_INVALID_HANDLE) {
        /* 注册UART设备上行数据接收回调 */
        if (cmuxPhyPortIsHsuart == TRUE) {
            if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_READ_CB, cmux_hsuart_uldata_read_cb) != CMUX_SUCCESS) {
                CMUX_PRINT(CMUX_DEBUG_ERR, "Register Read Callback Error!\n");
            }
        } else {
            if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_READ_CB, cmux_uart_uldata_read_cb) != CMUX_SUCCESS) {
                CMUX_PRINT(CMUX_DEBUG_ERR, "Register Read Callback Error!\n");
            }
        }

        /* 注册UART下行数据内存释放接口 */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_FREE_CB, cmux_hsuart_free_dldata_buff) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Register Free Callback Error!\n");
        }

        /* 注册管脚信号通知回调 */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_MSC_READ_CB, cmux_hsuart_msc_read_cb) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Set Msc Read Callback Error!\n");
        }
    }

    g_CmuxUdiHandle = lUdiHandle;
}

/* Description:    关闭CMUX和HSUART的接口(适配balong平台) */
void cmux_close_device(void)
{
    int lUdiHandle = g_CmuxUdiHandle;

    /* 释放相关回调 */
    if (lUdiHandle != UDI_INVALID_HANDLE) {
        /* 注册UART设备上行数据接收回调 */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_READ_CB, NULL) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Free Read Callback Error!\n");
        }

        /* 注册UART下行数据内存释放接口 */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_FREE_CB, NULL) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Free Free Callback Error!\n");
        }

        /* 注册管脚信号通知回调 */
        if (mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_SET_MSC_READ_CB, NULL) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Free Msc Read Callback Error!\n");
        }
    }

    /* 关闭Device */
    if (lUdiHandle != UDI_INVALID_HANDLE) {
        if (mdrv_udi_close(lUdiHandle) != CMUX_SUCCESS) {
            CMUX_PRINT(CMUX_DEBUG_ERR, "Close CMUX device failed!\r\n");
        }
    }
}

/* Description:    将cmux的数据发送给串口驱动(适配balong平台) */
unsigned int cmux_uart_write_data_async(unsigned char* pucData, unsigned int ulLen)
{
    int        lUdiHandle;
    signed int lResult;

    /* 检查数据有效性 */
    if ((pucData == NULL) || (ulLen == 0)) {
        return FAIL;
    }

    /* 检查UDI句柄有效性 */
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

    /* 检查UDI句柄有效性 */
    lUdiHandle = g_CmuxUdiHandle;
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\r\n");
        cmux_hsuart_free_dldata_buff(data);
        return FAIL;
    }

    /* 待写入数据内存 */
    stCtlParam.pBuffer  = data;
    stCtlParam.u32Size  = 0;
    stCtlParam.pDrvPriv = NULL;

    /* 异步方式写数 */
    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_WRITE_ASYNC, &stCtlParam);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Write failed!\r\n");
        cmux_hsuart_free_dldata_buff(data);
        return FAIL;
    }

    return PASS;
}

/* Description:    CMUX lib向HSUART发送数据接口(适配balong平台) */
unsigned int cmux_uart_send_dldata(unsigned char* pucData, unsigned short usLen)
{
    unsigned int ulResult;

    /* 异步写HSUART设备, 写成功后内存由底软负责释放 */
    ulResult = cmux_uart_write_data_async(pucData, usLen);

    return ulResult;
}

unsigned int cmux_hsuart_send_dldata(unsigned char* pucData, unsigned short usLen)
{
    unsigned char *putData = NULL;
    unsigned int ulResult;

    /* 分配空间 */
    putData = cmux_alloc_skb((int)usLen);
    if (putData == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "putData null error!\r\n");
        return FAIL;
    }

    /* 拷贝数据 */
    cmux_put_skb(putData, pucData, (int)usLen);

    /* 异步写HSUART设备, 写成功后内存由底软负责释放 */
    ulResult = cmux_hsuart_write_data_async((char *)putData);

    return ulResult;
}

/* Description:    CMUX lib设置HSUART睡眠投票接口(适配balong平台) */
unsigned int cmux_hsuart_set_power_status(void)
{
    signed int lResult;
    int        lUdiHandle = g_CmuxUdiHandle;

    /* 检查UDI句柄有效性 */
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

/* Description:    CMUX lib向HSUART管脚信号写接口(适配balong平台) */
unsigned int cmux_hsuart_write_msc_cmd(cmux_modem_msc_stru* pstDceMsc)
{
    int        lUdiHandle = g_CmuxUdiHandle;
    signed int lResult;

    /* 检查UDI句柄有效性 */
    if (lUdiHandle == UDI_INVALID_HANDLE) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Invalid UDI handle!\r\n");
        return FAIL;
    }

    /* 写管脚信号 */
    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_MSC_WRITE_CMD, pstDceMsc);
    if (lResult != CMUX_SUCCESS) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "Write failed!\r\n");
        return FAIL;
    }

    return PASS;
}

/* Description:    HSUART硬件管脚信号读回调 */
void cmux_hsuart_msc_read_cb(void* pParam)
{
    unsigned char        DlcIndex;
    cmux_modem_msc_stru* pstDceMsc = NULL;

    /* 入参检查 */
    if (pParam == NULL) {
        CMUX_PRINT(CMUX_DEBUG_ERR, "pstDceMsc is NULL!\r\n");
        return;
    }
    pstDceMsc = (cmux_modem_msc_stru*)pParam;

    /* 默认用DLC 0通道承载 */
    DlcIndex = CMUX_UDI_DLC_CTL;

    /* 往AT侧发送msc状态消息 */
    CMUX_PRINT(CMUX_DEBUG_INFO, "DlcIndex is %d, OP_Dtr is %d, ucDtr is %d!\r\n", DlcIndex, pstDceMsc->OP_Dtr,
               pstDceMsc->ucDtr);
    cmux_send_msc_status(DlcIndex, pstDceMsc);
}

/* Description:    判断波特率是否在CMUX支持范围 */
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