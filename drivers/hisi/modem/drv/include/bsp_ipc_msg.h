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

#ifndef __BSP_IPC_MSG_H__
#define __BSP_IPC_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <product_config.h>
#include <osl_spinlock.h>
#include <osl_common.h>

typedef s32 (*read_cb_func)(u8 *data, u32 len); /*lint !e761*/
#ifdef CONFIG_BALONG_IPC_MSG_V230

typedef struct ipc_msg_attr_func {
    read_cb_func read_cb;
} ipcmsg_attr_s;

/*****************************************************************************
* 函 数 名  : bsp_ipc_msg_open
*
* 功能描述  : 带消息IPC注册读取回调
*
* 输入参数  : 读回调
*
* 输出参数  : 无
*
* 返 回 值  :
*
* 修改记录  : 
*****************************************************************************/
int bsp_ipc_msg_open(u32 chanId, ipcmsg_attr_s *chan_attr);

/*****************************************************************************
* 函 数 名  : bsp_ipc_msg_close
*
* 功能描述  : 释放通道
*
* 输入参数  : 通道
*
* 输出参数  : 无
*
* 返 回 值  :
*
* 修改记录  :
*****************************************************************************/
int bsp_ipc_msg_close(u32 chanId);

/*****************************************************************************
* 函 数 名  :  bsp_ipc_msg_send
*
* 功能描述  :  IPC MSG对外发送接口函数
*
* 输入参数  :  邮箱通道 数据 长度

* 输出参数  :  无
*
* 返 回 值  :  无
*
* 修改记录  :
*****************************************************************************/
int bsp_ipc_msg_send(u32 chanId, u8 *data, u32 data_len);

#ifdef __cplusplus
}
#endif /* end cplusplus */
#endif /* end CONFIG_BALONG_IPC_MSG_V230 */
#endif /* end __BSP_IPC_MSG_H__ */





