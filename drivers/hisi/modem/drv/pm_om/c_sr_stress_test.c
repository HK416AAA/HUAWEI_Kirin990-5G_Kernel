/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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
#include <bsp_softtimer.h>
#include <bsp_slice.h>
#include <bsp_print.h>
#include <linux/types.h>
#include <bsp_ipc_fusion.h> // for function of pm_wakeup_ccore
#include "c_sr_stress_test.h"

#undef THIS_MODU
#define THIS_MODU mod_pmom

#define sr_err(fmt, ...) bsp_err("[%s] :" fmt, __FUNCTION__, ##__VA_ARGS__);

u32 _c_sr_stress_get_pseudo_rand(u32 *seed, u32 range)
{
    u32 temp, high, low;
    u32 range_high, range_low;
    u32 rand_num;

    if (*seed == 0) {
        temp = bsp_get_slice_value_hrt();
    } else {
        temp = *seed;
    }
    temp = temp * RAND_A0 + RAND_C0;
    *seed = temp;

    high = temp >> SHIFT_SHORT;
    low = temp & 0xffff;

    range_high = range >> SHIFT_SHORT;
    range_low = range & 0xffff;

    rand_num = (low * range_low) >> SHIFT_SHORT;
    rand_num += high * range_low;

    if (range_high == 0) {
        rand_num >>= SHIFT_SHORT;
    } else {
        rand_num += low * range_high;
        rand_num = (rand_num >> SHIFT_SHORT) + (high * range_high);
    }
    return rand_num;
}

u32 c_sr_stress_get_pseudo_rand(struct c_sr_stress_rand *rand)
{
    return _c_sr_stress_get_pseudo_rand(&(rand->seed), rand->range);
}

void c_sr_stress_pseudo_rand_init(struct c_sr_stress_rand *rand, u32 seed, u32 range)
{
    if (range == 0) {
        sr_err("range=0 is meaningless \n");
        range = RAND_RANGE_DEFAULT;
    }
    rand->range = range;
    rand->seed = seed;
}


// for function of pm_wakeup_ccore

ipc_res_id_e g_c_sr_stress_wake_ipc[DSS_NUM] = {
    IPC_TSP_INT_SRC_AMCPU_DSS0_WAKAEUP,
    IPC_TSP_INT_SRC_AMCPU_DSS1_WAKAEUP,
    IPC_TSP_INT_SRC_AMCPU_DSS2_WAKAEUP,
    IPC_TSP_INT_SRC_AMCPU_DSS3_WAKAEUP
};
ipc_handle_t g_c_sr_stress_wake_ipc_hdl[DSS_NUM];

s32 __wake_ipc_dss_init(u32 dss_id)
{
    s32 ret;
    ipc_res_id_e ipc_e;
    if (dss_id >= DSS_NUM) {
        sr_err("dss_id >= DSS_NUM\n");
        return ERROR;
    }
    ipc_e = g_c_sr_stress_wake_ipc[dss_id];
    ret = bsp_ipc_open(ipc_e, &g_c_sr_stress_wake_ipc_hdl[dss_id]);
    if (ret != OK) {
        sr_err("__wake_ipc_dss_init open ipc ret(%d) failed!\n", ret);
    }
    return ret;
}

void __wake_ipc_init(void)
{
    u32 dss_id;
    s32 ret;
    for (dss_id = 0; dss_id < DSS_NUM; dss_id++) {
        ret = __wake_ipc_dss_init(dss_id);
        if (ret != OK) {
            break;
        }
    }
}

void __wake_dss(u32 dss_id)
{
    int ret;
    ret = bsp_ipc_send(g_c_sr_stress_wake_ipc_hdl[dss_id]);
    if (ret != BSP_OK) {
        sr_err("c_sr_stress_wake_dss fail to send ipc to dss(0x%x)!\n", dss_id);
    }
    return;
}
struct c_sr_stress_wake_ctrl g_c_sr_stress_wake_ctrl;

void c_sr_stress_wake_dss(u32 dss_id)
{
    g_c_sr_stress_wake_ctrl.dss_wake_cnt[dss_id]++;
    __wake_dss(dss_id);
}

void  c_sr_stress_wake_rand_init(u32 seed, u32 range)
{
    c_sr_stress_pseudo_rand_init(&g_c_sr_stress_wake_ctrl.rand, seed, range);
}

int should_wake_dss(struct c_sr_stress_rand *rand)
{
    u32 rand_num = c_sr_stress_get_pseudo_rand(rand);
    u32 range = rand->range;
    return ((rand_num % range) == 0);
}


/* c_sr_stress_rand_wake_tsp_once() wakeup some of the dss, in order 0>1>2>3 */
void c_sr_stress_rand_wake_tsp_once(void)
{
    u32 dss_id;
    for (dss_id = 0; dss_id < DSS_NUM; dss_id++) {
        if (should_wake_dss(&g_c_sr_stress_wake_ctrl.rand)) {
            c_sr_stress_wake_dss(dss_id);
        }
    }
}
/* c_sr_stress_rand_wake_tsp(): implement disorderly wakeup by wakeup more than once */
void c_sr_stress_rand_wake_tsp(void)
{
    int i;
    for (i = 0; i < RAND_WAKE_TSP_TIMES; i++) {
        c_sr_stress_rand_wake_tsp_once();
    }
}

struct c_sr_stress_timer_ctrl g_c_sr_stress_timer_ctrl;

void c_sr_stress_timer_rand_init(u32 seed, u32 range)
{
    c_sr_stress_pseudo_rand_init(&g_c_sr_stress_timer_ctrl.rand, seed, range);
}

u32 c_sr_stress_get_rand_timeout(void)
{
    return c_sr_stress_get_pseudo_rand(&g_c_sr_stress_timer_ctrl.rand);
}

void timer_cb_rand_wake_tsp(void *para)
{
    int ret;
    struct softtimer_list *softtimer = &g_c_sr_stress_timer_ctrl.softtimer;
    u32 timeout = c_sr_stress_get_rand_timeout();

    ret = bsp_softtimer_modify(softtimer, timeout);
    if (ret) {
        sr_err("bsp_softtimer_modify err\n");
        return;
    }
    bsp_softtimer_add(softtimer);

    c_sr_stress_rand_wake_tsp();
    g_c_sr_stress_timer_ctrl.cb_cnt++;
}

void c_sr_stress_timer_init(void)
{
    s32 ret;
    struct softtimer_list *softtimer = &g_c_sr_stress_timer_ctrl.softtimer;
    u32 timeout = c_sr_stress_get_rand_timeout();

    softtimer->wake_type = SOFTTIMER_WAKE;
    softtimer->func = timer_cb_rand_wake_tsp;
    softtimer->name = "c_sr_stress";
    softtimer->unit_type = TYPE_MS;
    softtimer->timeout = timeout;

    ret = bsp_softtimer_create(softtimer);
    if (ret) {
        sr_err("create error\n");
        return;
    }
    bsp_softtimer_add(softtimer);
    return;
}


void c_sr_stress_start(void)
{
    // rand, range 4, for each dss probability 1/4
    c_sr_stress_wake_rand_init(bsp_get_slice_value_hrt(), C_SR_STRESS_WAKE_PROBABILITY);
    __wake_ipc_init();
    c_sr_stress_timer_rand_init(bsp_get_slice_value_hrt(), C_SR_STRESS_TIMEOUT_MAX); // rand, range in 500ms
    c_sr_stress_timer_init();
}
void c_sr_stress_stop(void)
{
    if (g_c_sr_stress_timer_ctrl.softtimer.init_flags == TIMER_INIT_FLAG) {
        (void)bsp_softtimer_delete(&g_c_sr_stress_timer_ctrl.softtimer);
    }
}

void c_sr_stress_restart(void)
{
    struct softtimer_list *softtimer = &g_c_sr_stress_timer_ctrl.softtimer;

    if (softtimer->init_flags == TIMER_INIT_FLAG) {
        u32 timeout = c_sr_stress_get_rand_timeout();
        (void)bsp_softtimer_delete(softtimer);
        (void)bsp_softtimer_modify(softtimer, timeout);
        bsp_softtimer_add(softtimer);
    }
}

void c_sr_stress_debug(void)
{
    struct c_sr_stress_rand *rand;
    u64 timer_cnt;
    u64 *dss_wake_cnt = NULL;
    u32 idx;
    rand = &g_c_sr_stress_wake_ctrl.rand;
    for (idx = 0; idx < DSS_NUM; idx++) {
        dss_wake_cnt = g_c_sr_stress_wake_ctrl.dss_wake_cnt;
        sr_err("DSS_%d wake cnt=%lld\n", idx, dss_wake_cnt[idx]);
    }
    sr_err("tsp wake rand seed=%d range=%d\n", rand->seed, rand->range);

    timer_cnt = g_c_sr_stress_timer_ctrl.cb_cnt;
    rand = &g_c_sr_stress_timer_ctrl.rand;
    sr_err("timer cb cnt=%lld, rand seed=%d range=%d\n", timer_cnt, rand->seed, rand->range);
}

