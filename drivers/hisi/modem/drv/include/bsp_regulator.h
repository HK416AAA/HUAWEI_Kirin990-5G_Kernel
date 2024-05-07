/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#ifndef __BSP_REGULATOR_H__
#define __BSP_REGULATOR_H__
#include "osl_common.h"
#include "osl_sem.h"
#include "osl_list.h"
#include "bsp_sram.h"
#include "bsp_print.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
 * struct regulator
 *
 * One for each consumer device.
 */
#define VOLT_NAME_PA_VCC "PA-vcc" /* BUCK0 */
#define VOLT_NAME_RFIC_FRONT_VCC \
    "RFIC_front-vcc" /* BUCK5/LDO11 or NULL,default BUCK5.if change to LDO11,need to modify regulator table */
#define VOLT_NAME_RFIC_VCC "RFIC-vcc"          /* LVS07 */
#define VOLT_NAME_RF_FEM_VCC "RF_FEM-vcc"      /* LDO13 */
#define VOLT_NAME_PA_VBIA2_VCC "PA_Vbias2-vcc" /* LDO12 */

struct regulator_id_ops {
    int (*list_voltage)(int regulator_id, unsigned selector);
    int (*set_voltage)(int regulator_id, int min_uV, int max_uV, unsigned *selector);
    int (*get_voltage)(int regulator_id);
    int (*is_enabled)(int regulator_id);
    int (*enable)(int regulator_id);
    int (*disable)(int regulator_id);
    int (*set_mode)(int regulator_id, unsigned int mode);
    unsigned int (*get_mode)(int regulator_id);
    int (*set_current)(int regulator_id, int min_ua, int max_ua, unsigned *selector);
    int (*get_current_limit)(int regulator_id);
    int (*get_rampup_rate)(int regulator_id);
};
struct regulator_type_map {
    char *regulator_type;
    unsigned int is_enabled_flag;
    struct list_head list;
    struct regulator_id_ops *ops;
};

struct regulator_mtcmos_ops {
    unsigned int mtcoms_id;
    void *mtcmos_addr;
    unsigned int mtcmos_en_offset[2];
    unsigned int mtcmos_dis_offset[2];
    unsigned int clk_en_offset[2];
    unsigned int clk_dis_offset[2];
    unsigned int srs_en_offset[2];
    unsigned int srs_dis_offset[2];
    unsigned int ios_en_offset[2];
    unsigned int ios_dis_offset[2];
    unsigned int mtcmos_stat_offset[2];
};

enum REGULATOR_OPS_ID {
    REGULATOR_OPS_ENABLE = 0,
    REGULATOR_OPS_DISABLE = 1,
    REGULATOR_OPS_IS_ENABLED = 2,
    REGULATOR_OPS_GET_VOLT = 3,
    REGULATOR_OPS_SET_VOLT = 4,
    REGULATOR_OPS_GET_CURR = 5,
    REGULATOR_OPS_SET_CURR = 6,
};
enum REGULATOR_TYPE_ID {
    REGULATOR_TYPE = 0,
    MTCMOS_TYPE = 2,
};

#define PMU_TYPE_LEN (8)
struct regulator_pm_om_log {
    u32 ops_id;
    u32 regulator_id;
    u32 use_count;
    u32 task_id;
    char pmutype[PMU_TYPE_LEN];
};

#include <linux/regulator/consumer.h>

#ifndef CONFIG_REGULATOR
static inline int regulator_pmic_ops_register(struct regulator_id_ops *pmic_ops, const char *pmic_type)
{
    return 0;
}
#else
int regulator_pmic_ops_register(struct regulator_id_ops *pmic_ops, const char *pmic_type);
#endif

/* *****************************for regulator interface end*************************** */

#ifdef __cplusplus
}
#endif

#endif
