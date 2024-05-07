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

#ifndef CMUX_DRV_IFC_H
#define CMUX_DRV_IFC_H

#include "cmux_api_ifc.h"
#include "v_basic_type_def.h"
#include "v_typdef.h"

#if !defined(LLT_OS_VER) || (defined(LLT_OS_VER) && (LLT_OS_VER == LLT_WIN))
#include "mdrv_udi.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if (FEATURE_IOT_CMUX == FEATURE_ON)
/* 简单数据类型定义说明 */
#define CMUX_SUCCESS 0
#define CMUX_FAILURE 1

/* AT+CMUX命令超时恢复正常模式时间: 10000 ms */
#define AT_CMUX_EXPIRY_TIME 9500  /* 留0.5秒缓冲时间 */

typedef enum {
    CMUX_MODE_MIN      = 0,
    CMUX_MODE_BASIC    = CMUX_MODE_MIN,
    CMUX_MODE_ADVANCED = 1,
} cmux_mode_enum_type;

/* Frame type definitions based on 3GPP 27.010 */
typedef enum {
    CMUX_SUBSET_MIN = 0,
    CMUX_SUBSET_UIH = CMUX_SUBSET_MIN,
    CMUX_SUBSET_UI  = 1,
    CMUX_SUBSET_I   = 2,
} cmux_subset_enum_type;

/* Port speed definitions based on 3GPP 27.010 */
typedef enum {
    CMUX_PHY_PORT_SPEED_INVALID = 0,
    CMUX_PHY_PORT_SPEED_1       = 1,  /* 9,600 bit/s */
    CMUX_PHY_PORT_SPEED_2       = 2,  /* 19,200 bit/s */
    CMUX_PHY_PORT_SPEED_3       = 3,  /* 38,400 bit/s */
    CMUX_PHY_PORT_SPEED_4       = 4,  /* 57,600 bit/s */
    CMUX_PHY_PORT_SPEED_5       = 5,  /* 115,200 bit/s */
    CMUX_PHY_PORT_SPEED_6       = 6,  /* 230,400 bit/s */
    CMUX_PHY_PORT_SPEED_7       = 7,  /* 460,800 bit/s */
    CMUX_PHY_PORT_SPEED_8       = 8,  /* 921,600 bit/s */
    CMUX_PHY_PORT_SPEED_9       = 9,  /* 1,000,000 bit/s */
    CMUX_PHY_PORT_SPEED_10      = 10, /* 1,152,000 bit/s */
    CMUX_PHY_PORT_SPEED_11      = 11, /* 1,500,000 bit/s */
    CMUX_PHY_PORT_SPEED_12      = 12, /* 2,000,000 bit/s */
    CMUX_PHY_PORT_SPEED_13      = 13, /* 2,500,000 bit/s */
    CMUX_PHY_PORT_SPEED_14      = 14, /* 3,000,000 bit/s */
} cmux_port_speed_enum_type;

/* Window size definitions based on 3GPP 27.010, applicable only for Advanced mode. */
typedef enum {
    CMUX_WINDOW_SIZE_INVALID = 0,
    CMUX_WINDOW_SIZE_1       = 1,
    CMUX_WINDOW_SIZE_2       = 2,
    CMUX_WINDOW_SIZE_3       = 3,
    CMUX_WINDOW_SIZE_4       = 4,
    CMUX_WINDOW_SIZE_5       = 5,
    CMUX_WINDOW_SIZE_6       = 6,
    CMUX_WINDOW_SIZE_7       = 7,
} cmux_window_size_enum_type;


/* MODEM 管脚信号结构体定义 */
typedef struct tagCMUX_MODEM_MSC_STRU {
    unsigned int  OP_Dtr : 1;    /* DTR CHANGE FLAG */
    unsigned int  OP_Dsr : 1;    /* DSR CHANGE FLAG */
    unsigned int  OP_Cts : 1;    /* CTSCHANGE FLAG */
    unsigned int  OP_Rts : 1;    /* RTS CHANGE FLAG */
    unsigned int  OP_Ri : 1;     /* RI CHANGE FLAG */
    unsigned int  OP_Dcd : 1;    /* DCD CHANGE FLAG */
    unsigned int  OP_Fc : 1;     /* FC CHANGE FLAG */
    unsigned int  OP_Brk : 1;    /* BRK CHANGE FLAG */
    unsigned int  OP_Spare : 24; /* reserve */
    unsigned char ucDtr;         /* DTR  VALUE */
    unsigned char ucDsr;         /* DSR  VALUE */
    unsigned char ucCts;         /* DTS VALUE */
    unsigned char ucRts;         /* RTS  VALUE */
    unsigned char ucRi;          /* RI VALUE */
    unsigned char ucDcd;         /* DCD  VALUE */
    unsigned char ucFc;          /* FC  VALUE */
    unsigned char ucBrk;         /* BRK  VALUE */
    unsigned char ucBrkLen;      /* BRKLEN VALUE */
    unsigned char   reserved[3];
} cmux_modem_msc_stru;
typedef struct tagCMUX_UART_WR_ASYNC_INFO {
    char*        pBuffer;
    unsigned int u32Size;
    unsigned char reserved[4] ;
    void*        pDrvPriv;
} cmux_hsuart_wr_async_info;

/* Range of size in bytes in Information field as per 3GPP 27.010 */
#define CMUX_MIN_FRAME_N1 31
#define CMUX_MAX_FRAME_N1 1540
/* Range of number of re-transmissions as per 3GPP 27.010 */
#define CMUX_MAX_FRAME_N2 10
/* The values of T1, T2 should extend to ms */
#define T1_T2_FACTOR 10
/* Default size in bytes in Information field per 3GPP 27.010, basic mode */
#define CMUX_BASIC_MODE_DEFAULT_FRAME_N1 31
/* Default size in bytes in Information field per 3GPP 27.010, advanced mode */
#define CMUX_ADVANCED_MODE_DEFAULT_FRAME_N1 63
/* Max Number of re-tranmission as per 3GPP 27.010 */
#define CMUX_DEFAULT_MAX_TX_N2 3
/* Default value of acknowledgement timer in ms as per 3GPP 27.010 */
#define CMUX_DEFAULT_CMD_TIMER_T1 100
/* Default value of DLCI0 response timer in ms as per 3GPP 27.010 */
#define CMUX_DEFAULT_DLCI0_TIMER_T2 900
/* Default value of response timer as per 3GPP 27.010 */
#define CMUX_DEFAULT_TIMER_T3 10
/* Default value of window size as per 3GPP 27.010 */
#define CMUX_DEFAULT_WINDOW_SIZE_K 2

void cmux_send_msc_status(unsigned char ucIndex, cmux_modem_msc_stru* pModemMsc);

cmux_info_type* cmux_get_cmux_info_type(VOS_VOID);

/* Description: 设置gCmuxClosePort全局变量值(适配balong平台) */
void set_cmux_close_port(unsigned char value);

/* Description: CMUX模块初始化，开启CMUX模式(适配balong平台) */
int cmux_init_port(cmux_info_type* CmuxInfo, unsigned int cmuxPhyPortIsHsuart);

/* Description: 退出CMUX模式，恢复AT模式(适配balong平台) */
void cmux_close_port(void);

/* Description: 获取HSUART的上行数据buffer，送到CMUX lib处理(适配balong平台) */
void cmux_hsuart_uldata_read_cb(void);

void cmux_uart_uldata_read_cb(void);

/* Description: 调用IOCTL从上行buffer中取数据(适配balong平台) */
unsigned int cmux_hsuart_get_uldata_buff(unsigned char** pucData, unsigned int* ulLen);

unsigned int cmux_uart_get_uldata_buff(unsigned char** pucData, unsigned int* ulLen);

/* Description: 下行数据内存释放(适配balong平台) */
void cmux_hsuart_free_dldata_buff(char* pucData);

/* Description: 上行数据内存释放(适配balong平台) */
unsigned int cmux_hsuart_free_uldata_buff(unsigned char* pucData);

unsigned int cmux_uart_free_uldata_buff(unsigned char* pucData);

/* Description: 打开CMUX和HSUART设备,注册回调函数(适配balong平台) */
void cmux_open_device(VOS_UINT32 cmuxPhyPortIsHsuart);

/* Description: 关闭CMUX和HSUART的接口(适配balong平台) */
void cmux_close_device(void);

/* Description: 将cmux的数据发送给串口驱动(适配balong平台) */

unsigned int cmux_uart_write_data_async(unsigned char* pucData, unsigned int ulLen);

unsigned int cmux_hsuart_write_data_async(char* data);


/* Description: CMUX lib向HSUART发送数据接口(适配balong平台) */
unsigned int cmux_uart_send_dldata(unsigned char* pucData, unsigned short usLen);

unsigned int cmux_hsuart_send_dldata(unsigned char* pucData, unsigned short usLen);

/* Description: HSUART硬件管脚信号读回调 */
void cmux_hsuart_msc_read_cb(void* pParam);

/* Description: CMUX lib设置HSUART睡眠投票接口(适配balong平台) */
unsigned int cmux_hsuart_set_power_status(void);

/* Description: 判断波特率是否在CMUX支持范围 */
unsigned int cmux_baud_to_num(unsigned int enBaudRate);

struct cmuxlib* cmux_get_g_cmux(void);
void cmux_set_g_cmux(struct cmuxlib* cmux);
struct delayed_work cmux_get_g_at_cmux_expiry_timer(void);
void cmux_set_g_at_cmux_expiry_timer(struct delayed_work cmux_expiry_timer);

extern int send_to_com(char* comname, unsigned char* rawptr, unsigned short len);


#endif /* FEATURE_ON == IOT_BALONG711_HSUART_CMUX */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* CMUX_DRV_IFC_H */
