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
#ifndef __STA_TAFM_H__
#define __STA_TAFM_H__

#include "vos.h"
#include "taf_app_mma.h"

/* *************************************************************************** */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ================================================================= */
#pragma pack(push, 4)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ----------------------  常量定义 ---------------------- */
/* ======================================================= */
/* ======================================================= */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ----------------  接口参数数据结构定义 ---------------- */
/* ======================================================= */
/* --------- Sta_PhoneStart 接口参数结构 ------- */

/* --------- Sta_SoftReset 接口参数结构 -------- */
#define STA_PH_RESET_ALL 0  /* 重启整个模块 */
#define STA_PH_RESET_FSM 1  /* 状态机复位 */
#define STA_PH_RESET_FLAG 2 /* 状态机标志复位 */
#define STA_PH_RESET_DATA 3 /* 其他全局数据复位 */
#define STA_PH_RESET_INIT 4 /* 协议栈初始化调用 */

/* ======================================================= */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ----------- STATUS 给TAFM提供的API接口函数 ------------ */
/* ======================================================= */

/* --------------- PLMN搜索过程  --------------- */

/* ------------- PLMN搜索过程终止 -------------- */
/* ------------ PLMN重选（自动模式）------------ */

/* --------------- 选择指定的PLMN -------------- */

/* ------ 向上层应用发送协议栈消息的接口 ------- */

/* Sta_MsgInd函数移动位置 */

/* -------------- DETACH过程请求 -------------- */
/* Sta_Detach */
/* ------------- ATTACH过程请求 --------------- */

/* ------- STATUS定时器超时事件处理过程 ------- */

/* ======================================================= */

/* ================================================================= */
#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

/* *************************************************************************** */

/* =========================================================================== */
#endif /* __STA_TAFM_H__ */

/* **** End of the file **** */
