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

#ifndef __MDRV_EICC_H__
#define __MDRV_EICC_H__
#ifdef __cplusplus
extern "C" {
#endif
/**
 *  @brief   eicc模块在acore上的对外头文件
 *  @file    mdrv_eicc.h
 *  @author  wangzaiwei
 *  @version v1.0
 *  @date    2019.11.15
 *  @note
 *  -v1.0|2019.11.15|创建文件
 *  @since   DRV2.0
 */

/**
 * @brief eicc句柄类型，用于屏蔽不同平台差异
 */
typedef unsigned eicc_chn_t;

/**
 * @brief eicc句柄类型无效值，应该使用此值对eicc_chn_t初始化
 */
#define EICC_CHN_INVAILD_HDL (0xFFFFFFFFU)

/** @brief 错误码：当前资源不足，请重试 */
#define EICC_ERR_EAGAIN (-11)

/**
 * @brief eicc通道号，用于打开通道时的标识 eicc_chn_attr_t.chnid
 */
enum eicc_chn_id {
    /* LPM3<-->AP */
    EICC_CHN_SEND_LPM2AP_MDRV_MSG = 0x00,
    EICC_CHN_RECV_LPM2AP_MDRV_MSG = 0x01,
    EICC_CHN_SEND_AP2LPM_MDRV_MSG = 0x02,
    EICC_CHN_RECV_AP2LPM_MDRV_MSG = 0x03,
    /* LPM3<-->TEEOS */
    EICC_CHN_SEND_LPM2TEE_MDRV_MSG = 0x04,
    EICC_CHN_RECV_LPM2TEE_MDRV_MSG = 0x05,
    EICC_CHN_SEND_TEE2LPM_MDRV_MSG = 0x06,
    EICC_CHN_RECV_TEE2LPM_MDRV_MSG = 0x07,

    /* TSP<-->LPM3 */
    EICC_CHN_SEND_TSP2LPM_MDRV_MSG = 0x10,
    EICC_CHN_RECV_TSP2LPM_MDRV_MSG = 0x11,
    EICC_CHN_SEND_LPM2TSP_MDRV_MSG = 0x12,
    EICC_CHN_RECV_LPM2TSP_MDRV_MSG = 0x13,
    /* TSP<-->HIFI */
    EICC_CHN_SEND_TSP2HIFI_MDRV_MSG = 0x14,
    EICC_CHN_RECV_TSP2HIFI_MDRV_MSG = 0x15,
    EICC_CHN_SEND_HIFI2TSP_MDRV_MSG = 0x16,
    EICC_CHN_RECV_HIFI2TSP_MDRV_MSG = 0x17,
    /* TSP<-->AP */
    EICC_CHN_SEND_TSP2AP_MDRV_MSG = 0x18,
    EICC_CHN_RECV_TSP2AP_MDRV_MSG = 0x19,
    EICC_CHN_SEND_AP2TSP_MDRV_MSG = 0x1A,
    EICC_CHN_RECV_AP2TSP_MDRV_MSG = 0x1B,
    /* TSP<-->TEEOS */
    EICC_CHN_SEND_TSP2TEE_MDRV_MSG = 0x1C,
    EICC_CHN_RECV_TSP2TEE_MDRV_MSG = 0x1D,
    EICC_CHN_SEND_TEE2TSP_MDRV_MSG = 0x1E,
    EICC_CHN_RECV_TEE2TSP_MDRV_MSG = 0x1F,
    /* TSP<-->TVP */
    EICC_CHN_SEND_TSP2TVP_MDRV_MSG = 0x20,
    EICC_CHN_RECV_TSP2TVP_MDRV_MSG = 0x21,
    EICC_CHN_SEND_TVP2TSP_MDRV_MSG = 0x22,
    EICC_CHN_RECV_TVP2TSP_MDRV_MSG = 0x23,

    /* TSP<-->AP */
    EICC_CHN_SEND_TSP2AP_MDRVRFILE = 0x24,
    EICC_CHN_RECV_TSP2AP_MDRVRFILE = 0x25,
    EICC_CHN_SEND_AP2TSP_MDRVRFILE = 0x26,
    EICC_CHN_RECV_AP2TSP_MDRVRFILE = 0x27,

    EICC_CHN_SEND_TSP2AP_MDRVNV = 0x28,
    EICC_CHN_RECV_TSP2AP_MDRVNV = 0x29,
    EICC_CHN_SEND_AP2TSP_MDRVNV = 0x2A,
    EICC_CHN_RECV_AP2TSP_MDRVNV = 0x2B,

    EICC_CHN_SEND_DMA_MDRVMAA0 = 0x40,
    EICC_CHN_SEND_DMA_MDRVMAA1 = 0x42,
    EICC_CHN_SEND_DMA_MDRVMAA2 = 0x44,
    EICC_CHN_SEND_DMA_MDRVMAA3 = 0x46,

    EICC_CHN_SEND_DMA_MDRVPMSR = 0x48,

    /* eicc chn for phy */
    EICC_CHN_SEND_TSP2TVP_PHY0 = 0x80,
    EICC_CHN_RECV_TSP2TVP_PHY0 = 0x81,
    EICC_CHN_SEND_TSP2TVP_PHY1 = 0x82,
    EICC_CHN_RECV_TSP2TVP_PHY1 = 0x83,
    EICC_CHN_SEND_TSP2TVP_PHY2 = 0x84,
    EICC_CHN_RECV_TSP2TVP_PHY2 = 0x85,
    EICC_CHN_SEND_TSP2TVP_PHY3 = 0x86,
    EICC_CHN_RECV_TSP2TVP_PHY3 = 0x87,

    EICC_CHN_SEND_TVP2TSP_PHY0 = 0x88,
    EICC_CHN_RECV_TVP2TSP_PHY0 = 0x89,
    EICC_CHN_SEND_TVP2TSP_PHY1 = 0x8A,
    EICC_CHN_RECV_TVP2TSP_PHY1 = 0x8B,
    EICC_CHN_SEND_TVP2TSP_PHY2 = 0x8C,
    EICC_CHN_RECV_TVP2TSP_PHY2 = 0x8D,
    EICC_CHN_SEND_TVP2TSP_PHY3 = 0x8E,
    EICC_CHN_RECV_TVP2TSP_PHY3 = 0x8F,

    EICC_CHN_SEND_TSP2TVP_PHY4 = 0x90,
    EICC_CHN_RECV_TSP2TVP_PHY4 = 0x91,
    EICC_CHN_SEND_TSP2TVP_PHY5 = 0x92,
    EICC_CHN_RECV_TSP2TVP_PHY5 = 0x93,

    EICC_CHN_SEND_TVP2TSP_PHY4 = 0x98,
    EICC_CHN_RECV_TVP2TSP_PHY4 = 0x99,
    EICC_CHN_SEND_TVP2TSP_PHY5 = 0x9A,
    EICC_CHN_RECV_TVP2TSP_PHY5 = 0x9B,

    EICC_CHN_SEND_DMA_PS0 = 0x140,
    EICC_CHN_SEND_DMA_PS1 = 0x142,
    EICC_CHN_SEND_DMA_PS2 = 0x144,
    EICC_CHN_SEND_DMA_PS3 = 0x146,
    EICC_CHN_SEND_DMA_PS4 = 0x148,
    EICC_CHN_SEND_DMA_PS5 = 0x14A,
    EICC_CHN_SEND_DMA_PS6 = 0x14C,
    EICC_CHN_SEND_DMA_PS7 = 0x14E,

    /* eicc chn for test */
    EICC_CHN_SEND_TSP2TVP_MDRV_LLT = 0xFFFF0000,
    EICC_CHN_RECV_TSP2TVP_MDRV_LLT = 0xFFFF0001,
    EICC_CHN_SEND_TVP2TSP_MDRV_LLT = 0xFFFF0002,
    EICC_CHN_RECV_TVP2TSP_MDRV_LLT = 0xFFFF0003,

    EICC_CHN_SEND_TSP2AP_MDRV_LLT = 0xFFFF0010,
    EICC_CHN_RECV_TSP2AP_MDRV_LLT = 0xFFFF0011,
    EICC_CHN_SEND_AP2TSP_MDRV_LLT = 0xFFFF0012,
    EICC_CHN_RECV_AP2TSP_MDRV_LLT = 0xFFFF0013,

    EICC_CHN_INVAILD = 0xFFFFFFFFUL
};

/**
 * @brief eicc发送通道类型，用于打开通道时eicc_chn_attr_t.type
 */
#define EICC_CHN_TYPE_SEND 0x0
/**
 * @brief eicc接收通道类型，用于打开通道时eicc_chn_attr_t.type
 */
#define EICC_CHN_TYPE_RECV 0x1
/**
 * @brief eicc无效通道类型，用于打开通道时eicc_chn_attr_t.type
 */
#define EICC_CHN_TYPE_INVAILD 0xFFFFFFFFUL

/**
 * @brief eicc回调事件类型
 */
typedef enum {
    EICC_EVENT_DATA_ARRV,
    EICC_EVENT_SEND_DONE,
    EICC_EVENT_SEND_FLOWUP,
    EICC_EVENT_SEND_FLOWDN,
    EICC_EVENT_INVAILD,
} eicc_event;
/**
 * @brief eicc回调事件额外参数
 */
typedef union {
    struct {
        unsigned chnid;
        eicc_chn_t chn_hdl;
    } data_arrv; /* used for give information for event EICC_EVENT_DATA_ARRV */
} eicc_eventinfo;
/**
 * @brief eicc回调
 * @attention
 * <ul><li>此回调直接执行在中断上下文</li></ul>
 * @param[in]  event, 此次回调的事件类型
 * @param[in]  cbk_arg, 用户私有参数，此参数来自于open通道时用户传入
 * @param[in]  info, 此次回调所带的额外信息
 *
 * @retval 0,回调正确处理。
 * @retval 非0,回调未正确处理。
 *
 * @see mdrv_eicc_chn_open
 */
typedef int (*eicc_cbk)(eicc_event event, void *cbk_arg, const eicc_eventinfo *info);

/** @brief 默认情况下, 数据到达，接收方总是有通知的，该标志可以关闭该通知 */
#define EICC_ATTR_FLAGS_NOARRVEVENT (1U << 0)
/** @brief 设置该标记的通道，DPM低功耗时不管理(这种通道必须是DMA通道) */
#define EICC_ATTR_FLAGS_DPMSRIGNORE (1U << 4)
/** @brief 设置该标记的通道，低功耗管理在特定级别(需要硬件资源支持，使用该标记请与开发者确认) */
#define EICC_ATTR_FLAGS_DSSCTRLSR (1U << 31)
/**
 * @brief 用户打开通道时的输入参数
 * @attention
 * <ul><li>该结构体在使用前,必须使用eicc_chn_attr_init初始化</li></ul>
 * @see mdrv_eicc_chn_attr_init mdrv_eicc_chn_open
 */
typedef struct {
    unsigned chnid;        /**< 通道的id定义, 在attr_init后用户填写 */
    unsigned type;         /**< 通道收发属性，例如EICC_CHN_TYPE_SEND ,在attr_init后用户填写 */
    unsigned long long pa; /**< ringbuf物理地址，硬件有对齐要求 8Bytes, 在attr_init后用户填写 */
    void *va;              /**< ringbuf虚拟地址,在attr_init后用户填写 */
    unsigned size;         /**< ringbuf大小，硬件有对齐要求 128Bytes,在attr_init后用户填写 */
    eicc_cbk cbk;          /**< eicc事件发生时的回调函数, 在attr_init后用户填写*/
    void *cbk_arg;         /**< 回调时，透传的用户私有数据，用户需要保证cbk_arg的生命周期,在attr_init后用户填写 */
    unsigned flags;        /**< 一些标志，比如唤醒，报中断等, 不清楚用处的在attr_init后不要操作 */
} eicc_chn_attr_t;

/**
 * @brief 初始化eicc属性的接口
 *
 * @par 描述:
 * pattr用于描述打开通道的各种参数，该函数用于实现将pattr设置为默认值
 *
 * @param[in]  pattr, eicc通道属性描述
 *
 * @retval 0, 执行成功
 * @retval 非0, 执行失败
 * @par 依赖:
 * <ul><li>mdrv_eicc.h：该接口声明所在的头文件。</li></ul>
 *
 * @see mdrv_eicc_chn_open
 */
int mdrv_eicc_chn_attr_init(eicc_chn_attr_t *pattr);

/**
 * @brief 打开eicc通道的接口
 *
 * @par 描述:
 * 根据pattr的描述，打开通道，通道必须打开之后才能进行数据收发
 *
 * @attention
 * <ul><li>pattr需要先调用mdrv_eicc_chn_attr_init初始化, 在设置pattr中相应字段，具体参考pattr结构介绍</li></ul>
 *
 * @param[out]  pchn_hdl, 通道成功打开后返回的句柄,用于调用其他eicc函数
 * @param[in]  pattr, eicc通道属性
 *
 * @retval 0, 执行成功
 * @retval 非0, 执行失败
 * @par 依赖:
 * <ul><li>mdrv_eicc.h：该接口声明所在的头文件。</li></ul>
 *
 * @see mdrv_eicc_chn_attr_init
 */
int mdrv_eicc_chn_open(eicc_chn_t *pchn_hdl, eicc_chn_attr_t *pattr);

/**
 * @brief eicc数据发送接口
 *
 * @par 描述:
 * 将一段buf直接发送到目的端
 *
 * @attention
 * <ul><li>申请高速内存时，若无内存可用，会尝试分配低速内存</li></ul>
 *
 * @param[in]  pchn_hdl, eicc通道句柄，由open通道时得到
 * @param[in]  buf, 要发送的数据地址
 * @param[in]  len, 要发送的数据长度
 *
 * @retval 0, 执行成功
 * @retval 非0, 执行失败
 * @par 依赖:
 * <ul><li>mdrv_eicc.h：该接口声明所在的头文件。</li></ul>
 *
 * @see mdrv_eicc_chn_open
 */
int mdrv_eicc_chn_send_simple(eicc_chn_t chn_hdl, void *buf, unsigned len);

/**
 * @brief eicc数据发送接口
 *
 * @par 描述:
 * 将两段buf合并之后发送到目的端
 *
 * @attention
 * <ul><li>该接口为物理层定制，实现透传子通道号的功能</li></ul>
 * <ul><li>两端buff必须有效，任意一段不能为空或者长度为0</li></ul>
 *
 * @param[in]  pchn_hdl, eicc通道句柄，由open通道时得到
 * @param[in]  prebuf, 要发送的第一段数据地址(物理层用于这段buf里放子通道号)
 * @param[in]  prelen, 要发送的第一段数据长度
 * @param[in]  buf, 要发送的第二段数据地址(物理层用于这段buf里放实际发送数据)
 * @param[in]  len, 要发送的第二段数据长度
 *
 * @retval 0, 执行成功
 * @retval 非0, 执行失败
 * @par 依赖:
 * <ul><li>mdrv_eicc.h：该接口声明所在的头文件。</li></ul>
 *
 * @see mdrv_eicc_chn_open
 */
int mdrv_eicc_chn_send_2block(eicc_chn_t chn_hdl, void *prebuf, unsigned prelen, void *buf,
                                              unsigned len);
/**
 * @brief eicc数据接收接口
 *
 * @par 描述:
 * 尝试往buf中拷贝len长度数据，如果len比实际数据小，则报错，如果len比实际大或者相当，则应该全部拷贝，并且返回成功
 * 当该接口返回成功后，eicc中的数据被取走
 *
 * @param[in]  pchn_hdl, eicc通道句柄，由open通道时得到
 * @param[in]  buf, 用于接收数据的buf
 * @param[in]  len, 用于接受数据的buf的长度
 *
 * @retval 0, 执行成功
 * @retval 非0, 执行失败
 * @par 依赖:
 * <ul><li>mdrv_eicc.h：该接口声明所在的头文件。</li></ul>
 *
 * @see mdrv_eicc_chn_open
 */
int mdrv_eicc_chn_recv_simple(eicc_chn_t chn_hdl, void *buf, unsigned len);

/**
 * @brief eicc数据试取接口(做读取尝试)
 *
 * @par 描述:
 * 尝试往buf中拷贝len长度数据,如果len比实际大 要报错
 * 该接口调用不会把packet真正读走，eicc的数据不受影响
 *
 * @param[in]  chn_hdl, eicc通道句柄，由open通道时得到
 * @param[in]  buf, 用于接收数据的buf
 * @param[in]  len, 用于接受数据的buf的长度
 *
 * @retval 0, 执行成功
 * @retval 非0, 执行失败
 * @par 依赖:
 * <ul><li>mdrv_eicc.h：该接口声明所在的头文件。</li></ul>
 *
 * @see mdrv_eicc_chn_open
 */
int mdrv_eicc_chn_recv_peek(eicc_chn_t chn_hdl, void *buf, unsigned len);

#ifdef __cplusplus
}
#endif

#endif
