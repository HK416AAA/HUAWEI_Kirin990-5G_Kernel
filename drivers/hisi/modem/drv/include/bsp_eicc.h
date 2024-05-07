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
#ifndef __BSP_EICC_H__
#define __BSP_EICC_H__

#include <osl_types.h>
#include <mdrv_eicc.h>

#ifdef __cplusplus /* __cplusplus */
extern "C" {
#endif /* __cplusplus */

typedef struct {
    unsigned cnt;
    struct {
        void *buf;
        unsigned len;
    } datablk[0];
} eicc_blk_desc_t;

typedef struct {
    unsigned cnt;
    struct {
        void *buf;
        unsigned len;
    } datablk[2];
} eicc_blkx2_desc_t;

typedef struct {
    unsigned cnt;
    struct {
        void *buf;
        unsigned len;
    } datablk[3];
} eicc_blkx3_desc_t;

#define EICC_SEND_FLAGS_NOARRVNOTIFY (1UL << 0)
#define EICC_SEND_FLAGS_NODONENOTIFY (1UL << 1)

#define EICC_RECV_FLAGS_DATAPEEK (1UL << 0)

#define EICC_IOCTL_CHN_NXTPKT_INF 0x40000002
#define EICC_IOCTL_CHN_PACKET_SKIP 0x40000003
#define EICC_IOCTL_CHN_CAPSET_INF 0x40000004
#define EICC_IOCTL_CHN_STATUS_INF 0x40000005

typedef struct {
    u32 len;
    u32 pkthdl;
} ioctl_nxtpkt;

struct eicc_chn_status
{
    u32 busy;
};

int bsp_eicc_init(void);
/************************************************************************
 * 函 数 名  : bsp_eicc_chn_attr_init
 * 功能描述  : pattr用于描述打开通道的各种参数，该函数用于实现将pattr设置为默认值
 * 输入参数  :
 *            pattr: eicc通道属性描述
 * 输出参数  : 无
 * 返 回 值  :  0          操作成功。
 *             其他        操作失败。
 **************************************************************************/
MODULE_EXPORTED int bsp_eicc_chn_attr_init(eicc_chn_attr_t *attr);  // for extend

/************************************************************************
 * 函 数 名  : bsp_eicc_chn_open
 * 功能描述  : 根据pattr的描述，打开通道，通道必须打开之后才能进行数据收发
 *             需要先调用bsp_eicc_chn_attr_init对pattr初始化，pattr必须设置的字段参考pattr介绍
 * 输入参数  :
 *            pattr: eicc通道属性
 * 输出参数  :
 *            pchn_hdl:  eicc通道句柄，用于调用其他函数
 * 返 回 值  :  0          操作成功。
 *             其他        操作失败。
 **************************************************************************/
MODULE_EXPORTED int bsp_eicc_chn_open(eicc_chn_t *pchn_hdl, eicc_chn_attr_t *attr);

/************************************************************************
 * 函 数 名	: bsp_eicc_chn_send
 * 功能描述  : 将一段buf直接发送到目的端
 * 输入参数  :
 *			  chn_hdl: eicc通道句柄，由open通道时得到
 *			  buf: 要发送的数据地址
 *			  len: 要发送的数据长度
 *			  flags: EICC_SEND_FLAGS，不了解的填0
 * 输出参数  :
 *			  无
 * 返 回 值	:  成功返回实际发送长度，失败返回负值。
 **************************************************************************/
MODULE_EXPORTED int bsp_eicc_chn_send(eicc_chn_t chn_hdl, void *buf, unsigned len, u32 flags);

/************************************************************************
 * 函 数 名	: bsp_eicc_chn_blks_send
 * 功能描述  : 将多段buf组合直接发送到目的端
 * 输入参数  :
 *			  chn_hdl: eicc通道句柄，由open通道时得到
 *			  blkdesc: 要发送的数据地址,长度等信息
 *			  flags: EICC_SEND_FLAGS，不了解的填0
 * 输出参数  :
 *			  无
 * 返 回 值	:  成功返回实际发送长度，失败返回负值。
 **************************************************************************/
MODULE_EXPORTED int bsp_eicc_chn_blks_send(eicc_chn_t chn_hdl, eicc_blk_desc_t *blkdesc, u32 flags);

/************************************************************************
 * 函 数 名	: bsp_eicc_chn_recv
 * 功能描述  : 接收数据
 * 输入参数  :
 *			  chn_hdl: eicc通道句柄，由open通道时得到
 *			  buf: 用于接收数据的buf
 *			  len: 用于接收数据的buf的大小
 *			  flags: EICC_RECV_FLAGS，不了解的填0
 * 输出参数  :
 *			  无
 * 返 回 值	:  成功实际接收数据长度(若没有数据，返回0)，失败返回负值(比如参数错误等)。
 **************************************************************************/
MODULE_EXPORTED int bsp_eicc_chn_recv(eicc_chn_t chn_hdl, void *buf, unsigned len, u32 flags);

/************************************************************************
 * 函 数 名	: bsp_eicc_chn_ioctl
 * 功能描述  : arg根据req是输入或者输出
 * 输入参数  :
 *			  chn_hdl: eicc通道句柄，由open通道时得到
 *			  req: 请求命令字 EICC_IOCTL_CHN*
 *			  arg: 请求命令字对应的输入参数
 *			  size: arg的大小
 * 输出参数  :
 *			  arg: 请求命令字对应的输出参数
 * 返 回 值	:  成功返回0，失败返回负值。
 **************************************************************************/
MODULE_EXPORTED int bsp_eicc_chn_ioctl(eicc_chn_t chn_hdl, unsigned req, void *arg, u32 size);

/************************************************************************
 * 函 数 名	: bsp_eicc_chn_before_resetting
 * 功能描述  : 单独复位前EICC通道的处理
 * 输入参数  :
 *			  无
 * 输出参数  :
 *			  无
 * 返 回 值	:  成功返回0，失败返回负值。
 **************************************************************************/
MODULE_EXPORTED int bsp_eicc_chn_before_resetting(void);

/************************************************************************
 * 函 数 名	: bsp_eicc_chn_after_resetting
 * 功能描述  : 单独复位后EICC通道的处理
 * 输入参数  :
 *			  无
 * 输出参数  :
 *			  无
 * 返 回 值	:  成功返回0，失败返回负值。
 **************************************************************************/

MODULE_EXPORTED int bsp_eicc_chn_after_resetting(void);

#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif /*  __BSP_EICC_H__ */

