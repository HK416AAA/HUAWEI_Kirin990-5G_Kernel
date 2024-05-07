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

#ifndef __DDR_TEST_DEV__
#define __DDR_TEST_DEV__

#define PMU_RESET_VALUE 500
#define SYS_OPEN_MODE_0 0660
#define SYS_OPEN_MODE_1 0775
#define LOG_BUF_LEN 22
#define LOG_YEAR_START 1900
#define DDR_PCIE_PRIORITY 76
#define DDR_PCIE_TASK_STACK 4096

#ifndef EXTRA_MODEM_DDRTEST
unsigned int ddr_test_get_boot_flag(void)
{
    return 0;
}
#else
unsigned int ddr_test_get_boot_flag(void);
#include <mdrv_rfile.h>
#define DDR_TEST_LOG_PATH (MODEM_LOG_ROOT "/ddr_test")

#define DDR_TEST_LOG_HIS_PATH (MODEM_LOG_ROOT "/ddr_test_history")
#define DDR_TEST_LOG_PAR_PATH "/data/hisi_logs/modem_log/"

enum ddr_test_log_type {
    PCIE_LOG_DDR_TEST_XLOADER = 0x30,
    PCIE_LOG_DDR_TEST_HIBOOT = 0x31,
};

enum ddr_test_op_type {
    DDR_TEST_LOG = 0x40,
    DDR_TEST_GET_TEST_PARAM = 0x41,
};

struct ddr_test_param {
    u32 test_type;
    u32 sub_test_type;
    u32 profile;
    u32 bias;
};

#define DDR_TEST_LOG_LENGH 0x700
/* msg B->A */
struct ddr_test_msi_log_rcv_data {
    u32 op_type;
    u8 log[DDR_TEST_LOG_LENGH];
};
/* msg A->B */
struct ddr_test_msi_log_snd_data {
    u32 op_type;
    u32 log_status;
    struct ddr_test_param param;
};
#ifndef FILE
#define FILE void
#endif

#endif

#endif
