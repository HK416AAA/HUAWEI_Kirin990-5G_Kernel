/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: msg
 * Author:
 * Created: 2019-09-27
 * Last Modified:
 * History:
 * Modification: Create file
 */

#ifndef __MSG_ID_PHY_H__
#define __MSG_ID_PHY_H__

#include "msg_id_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define GPHY_HIFI_MID_LENTH 20UL
#define PHY_FRONT_PID_LENTH 20UL
#define UPHY_TASK_MID_LENTH 50UL

#define MDRV_MSG_PHY_MIDS \
        PHY_MID_START = MSG_PHY_MID_START,                                       /* 8192 + 0 = 8192 */  \
        /* GU HIFI MID define here, top 20 MID for GU HIFI */ \
        GPHY_HIFI_MID_START = PHY_MID_START,                                     /* 8192 + 0 = 8192 */  \
        GPHY_MAIN_MSG_MID_0 = GPHY_HIFI_MID_START,                               /* 8192 + 0 = 8192 */  \
        GPHY_MAIN_MSG_MID_1,                                                     /* 8192 + 1 = 8193 */  \
        GPHY_MAIN_MSG_MID_2,                                                     /* 8192 + 2 = 8194 */  \
        GPHY_MAIN_MSG_END, \
        \
        WPHY_MAIN_MSG_ENTRY_MID_0 = GPHY_MAIN_MSG_END,                           /* 8192 + 3 = 8195 */  \
        WPHY_MAIN_MSG_ENTRY_MID_1,                                               /* 8192 + 4 = 8196 */  \
        WPHY_MAIN_MSG_ENTRY_MID_END, \
        \
        UPHY_UL1M_RCM_M_MID = WPHY_MAIN_MSG_ENTRY_MID_END,                       /* 8192 + 5 = 8197 */  \
        UPHY_UL1M_RCM_M_MID_END, \
        \
        GPHY_HIFI_MID_END = GPHY_HIFI_MID_START + GPHY_HIFI_MID_LENTH, \
        /* GU use top 20 mid for hifi, other front pid start here */ \
        \
        PHY_FRONT_PID_START = GPHY_HIFI_MID_END,                                 /* 8192 + 20 = 8212 */ \
        UPHY_NRSCHED_HL1C_SYMBINT_MID_0 = PHY_FRONT_PID_START,                   /* 8192 + 20 = 8212 */  \
        UPHY_NRSCHED_HL1C_SYMBINT_MID_1,                                         /* 8192 + 21 = 8213 */  \
        UPHY_NRSCHED_HL1C_SYMBINT_MID_END, \
        \
        UPHY_LTE0SCHED_SYMBINT_HL1C_MID = UPHY_NRSCHED_HL1C_SYMBINT_MID_END,     /* 8192 + 22 = 8214 */  \
        UPHY_LTE0SCHED_SYMBINT_HL1C_MID_END,\
        \
        UPHY_LTE1SCHED_SYMBINT_HL1C_MID = UPHY_LTE0SCHED_SYMBINT_HL1C_MID_END,   /* 8192 + 23 = 8215 */ \
        UPHY_LTE1SCHED_SYMBINT_HL1C_MID_END,\
        \
        LPHY_HL1C_HIGH_MSG_MID_0 = UPHY_LTE1SCHED_SYMBINT_HL1C_MID_END,          /* 8192 + 24 = 8216 */ \
        LPHY_HL1C_HIGH_MSG_MID_1,                                                /* 8192 + 25 = 8217 */ \
        LPHY_HL1C_HIGH_MSG_MID_END, \
        \
        NPHY_HL1C_HIGH_MSG_MID_0 = LPHY_HL1C_HIGH_MSG_MID_END,                   /* 8192 + 26 = 8218 */ \
        NPHY_HL1C_HIGH_MSG_MID_1,                                                /* 8192 + 27 = 8219 */ \
        NPHY_HL1C_HIGH_MSG_MID_END, \
        \
        PHY_FRONT_PID_END = PHY_FRONT_PID_START + PHY_FRONT_PID_LENTH, \
        \
        /* top 40 pid have uesed, other mid start here */ \
        /* uphy task mid  */ \
        UPHY_TASK_MID_START = PHY_FRONT_PID_END,                                 /* 8192 + 40 = 8232 */  \
        UPHY_UL1M_INIT_MID = UPHY_TASK_MID_START,                                /* 8192 + 40 = 8232 */  \
        UPHY_OM_MSG_PROC_MID,                                                    /* 8192 + 41 = 8233 */  \
        UPHY_INST_MAIN_MID,                                                      /* 8192 + 42 = 8234 */  \
        UPHY_HLD_LPC_MAIN_MID,                                                   /* 8192 + 43 = 8235 */  \
        LTEV_HIGHMBX_COMM_PROC_MID,                                              /* 8192 + 44 = 8236 */  \
        UPHY_RTT_HPA_MID,                                                        /* 8192 + 45 = 8237 */  \
        UPHY_FESS_H_PRIO_PROC_MID,                                               /* 8192 + 46 = 8238 */  \
        UPHY_FESS_M_PRIO_PROC_MID,                                               /* 8192 + 47 = 8239 */  \
        UPHY_FESS_L_PRIO_PROC_MID,                                               /* 8192 + 48 = 8240 */  \
        UPHY_RTT_SHELL_ENTRY_MID,                                                /* 8192 + 49 = 8241 */  \
        UPHY_RTT_SHELL_PREEMPT_MID,                                              /* 8192 + 50 = 8242 */  \
        UPHY_RTT_SHELL_L3_MID,                                                   /* 8192 + 51 = 8243 */  \
        UPHY_RTT_SHELL_MATCH_MID,                                                 /* 8192 + 52 = 8244 */  \
        UPHY_RTT_SHELL_ONLINE_MID,                                                /* 8192 + 53 = 8245 */  \
        UPHY_DVFS_VOTE_SOC_MID,                                                  /* 8192 + 54 = 8246 */  \
        UPHY_OM_AT_MSG_PROC_MID,                                                 /* 8192 + 55 = 8247 */  \
        UPHY_OM_CHR_MSG_PROC_MID,                                                /* 8192 + 56 = 8248 */  \
        UPHY_LAYER_MSG_TRANS_MID,                                                /* 8192 + 57 = 8249 */  \
        UL1M_HIGHMBX_COMM_PROC_MID,                                              /* 8192 + 58 = 8250 */  \
        FRM_HIGHMBX_COMM_PROC_MID,                                               /* 8192 + 59 = 8251 */  \
        NPHY_H1LC0_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 60 = 8252 */  \
        NPHY_H1LC1_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 61 = 8253 */  \
        NPHY_L1LC0_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 62 = 8254 */  \
        NPHY_L1LC1_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 63 = 8255 */  \
        NPHY_L1LC2_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 64 = 8256 */  \
        NPHY_L1LC3_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 65 = 8257 */  \
        NPHY_L1LC4_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 66 = 8258 */  \
        NPHY_L1LC5_HIGHMBX_COMM_PROC_MID,                                        /* 8192 + 67 = 8259 */  \
        LPHY_M0_HIGHMBX_COMM_PROC_MID,                                           /* 8192 + 68 = 8260 */  \
        LPHY_M1_HIGHMBX_COMM_PROC_MID,                                           /* 8192 + 69 = 8261 */  \
        UPHY_COMM_INST_RELEASE_MID,                                              /* 8192 + 70 = 8262 */  \
        UPHY_TASK_MID_END = UPHY_TASK_MID_START + UPHY_TASK_MID_LENTH, \
        \
        UPHY_SDRM_SDR_MSG_MID = UPHY_TASK_MID_END,                               /* 8192 + 90 = 8282 */  \
        UPHY_SDRM_SDR_MSG_MID_END, \
        \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_0 = UPHY_SDRM_SDR_MSG_MID_END,             /* 8192 + 91 = 8283 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_1,                                         /* 8192 + 92 = 8284 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_2,                                         /* 8192 + 93 = 8285 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_3,                                         /* 8192 + 94 = 8286 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_4,                                         /* 8192 + 95 = 8287 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_5,                                         /* 8192 + 96 = 8288 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_6,                                         /* 8192 + 97 = 8289 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_7,                                         /* 8192 + 98 = 8290 */  \
        NPHY_M0_LL1D_SYMBINT_IDX1_MID_END, \
        \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_0 = NPHY_M0_LL1D_SYMBINT_IDX1_MID_END,     /* 8192 + 99  = 8291 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_1,                                         /* 8192 + 100 = 8292 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_2,                                         /* 8192 + 101 = 8293 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_3,                                         /* 8192 + 102 = 8294 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_4,                                         /* 8192 + 103 = 8295 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_5,                                         /* 8192 + 104 = 8296 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_6,                                         /* 8192 + 105 = 8297 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_7,                                         /* 8192 + 106 = 8298 */  \
        NPHY_M1_LL1D_SYMBINT_IDX1_MID_END, \
        \
        NPHY_M0_LL1U_TOP_MSG_MID_0 = NPHY_M1_LL1D_SYMBINT_IDX1_MID_END,          /* 8192 + 107 = 8299 */  \
        NPHY_M0_LL1U_TOP_MSG_MID_1,                                              /* 8192 + 108 = 8300 */  \
        NPHY_M0_LL1U_TOP_MSG_MID_2,                                              /* 8192 + 109 = 8301 */  \
        NPHY_M0_LL1U_TOP_MSG_MID_3,                                              /* 8192 + 110 = 8302 */  \
        NPHY_M0_LL1U_TOP_MSG_MID_END, \
        \
        NPHY_M1_LL1U_TOP_MSG_MID_0 = NPHY_M0_LL1U_TOP_MSG_MID_END,               /* 8192 + 111 = 8303 */  \
        NPHY_M1_LL1U_TOP_MSG_MID_1,                                              /* 8192 + 112 = 8304 */  \
        NPHY_M1_LL1U_TOP_MSG_MID_2,                                              /* 8192 + 113 = 8305 */  \
        NPHY_M1_LL1U_TOP_MSG_MID_3,                                              /* 8192 + 114 = 8306 */  \
        NPHY_M1_LL1U_TOP_MSG_MID_END, \
        \
        NPHY_M0_LL1U_HIGH_ISR_MID_0 = NPHY_M1_LL1U_TOP_MSG_MID_END,              /* 8192 + 115 = 8307 */  \
        NPHY_M0_LL1U_HIGH_ISR_MID_1,                                             /* 8192 + 116 = 8308 */  \
        NPHY_M0_LL1U_HIGH_ISR_MID_2,                                             /* 8192 + 117 = 8309 */  \
        NPHY_M0_LL1U_HIGH_ISR_MID_3,                                             /* 8192 + 118 = 8310 */  \
        NPHY_M0_LL1U_HIGH_ISR_MID_END, \
        \
        NPHY_M1_LL1U_HIGH_ISR_MID_0 = NPHY_M0_LL1U_HIGH_ISR_MID_END,             /* 8192 + 119 = 8311 */  \
        NPHY_M1_LL1U_HIGH_ISR_MID_1,                                             /* 8192 + 120 = 8312 */  \
        NPHY_M1_LL1U_HIGH_ISR_MID_2,                                             /* 8192 + 121 = 8313 */  \
        NPHY_M1_LL1U_HIGH_ISR_MID_3,                                             /* 8192 + 122 = 8314 */  \
        NPHY_M1_LL1U_HIGH_ISR_MID_END, \
        \
        NPHY_M0_LL1U_HIGH_MSG_MID_0 = NPHY_M1_LL1U_HIGH_ISR_MID_END,             /* 8192 + 123 = 8315 */  \
        NPHY_M0_LL1U_HIGH_MSG_MID_1,                                             /* 8192 + 124 = 8316 */  \
        NPHY_M0_LL1U_HIGH_MSG_MID_2,                                             /* 8192 + 125 = 8317 */  \
        NPHY_M0_LL1U_HIGH_MSG_MID_3,                                             /* 8192 + 126 = 8318 */  \
        NPHY_M0_LL1U_HIGH_MSG_MID_END, \
        \
        NPHY_M1_LL1U_HIGH_MSG_MID_0 = NPHY_M0_LL1U_HIGH_MSG_MID_END,             /* 8192 + 127 = 8319 */  \
        NPHY_M1_LL1U_HIGH_MSG_MID_1,                                             /* 8192 + 128 = 8320 */  \
        NPHY_M1_LL1U_HIGH_MSG_MID_2,                                             /* 8192 + 129 = 8321 */  \
        NPHY_M1_LL1U_HIGH_MSG_MID_3,                                             /* 8192 + 130 = 8322 */  \
        NPHY_M1_LL1U_HIGH_MSG_MID_END, \
        \
        LPHY_M0_SCHED_SYMBINT_LL1C0_MID = NPHY_M1_LL1U_HIGH_MSG_MID_END,         /* 8192 + 131 = 8323 */  \
        LPHY_M0_SCHED_SYMBINT_LL1C1_MID,                                         /* 8192 + 132 = 8324 */  \
        LPHY_M0_SCHED_SYMBINT_LL1C2_MID,                                         /* 8192 + 133 = 8325 */  \
        LPHY_M0_SCHED_SYMBINT_LL1C3_MID,                                         /* 8192 + 134 = 8326 */  \
        LPHY_M0_SCHED_SYMBINT_LL1C4_MID,                                         /* 8192 + 135 = 8327 */  \
        LPHY_M0_SCHED_SYMBINT_MID_END, \
        \
        LPHY_M1_SCHED_SYMBINT_LL1C0_MID = LPHY_M0_SCHED_SYMBINT_MID_END,         /* 8192 + 136 = 8328 */  \
        LPHY_M1_SCHED_SYMBINT_LL1C1_MID,                                         /* 8192 + 137 = 8329 */  \
        LPHY_M1_SCHED_SYMBINT_LL1C2_MID,                                         /* 8192 + 138 = 8330 */  \
        LPHY_M1_SCHED_SYMBINT_LL1C3_MID,                                         /* 8192 + 139 = 8331 */  \
        LPHY_M1_SCHED_SYMBINT_LL1C4_MID,                                         /* 8192 + 140 = 8332 */  \
        LPHY_M1_SCHED_SYMBINT_MID_END, \
        \
        UPHY_NRPROC_HL1C_LOW_PROC_MID_0 = LPHY_M1_SCHED_SYMBINT_MID_END,         /* 8192 + 141 = 8333 */  \
        UPHY_NRPROC_HL1C_LOW_PROC_MID_1,                                         /* 8192 + 142 = 8334 */  \
        UPHY_NRPROC_HL1C_LOW_PROC_MID_END, \
        \
        UPHY_LTEPROC_HL1C_LOW_PROC_MID_0 = UPHY_NRPROC_HL1C_LOW_PROC_MID_END,    /* 8192 + 143 = 8335 */  \
        UPHY_LTEPROC_HL1C_LOW_PROC_MID_1,                                        /* 8192 + 144 = 8336 */  \
        UPHY_LTEPROC_HL1C_LOW_PROC_MID_END, \
        \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_0 = UPHY_LTEPROC_HL1C_LOW_PROC_MID_END,   /* 8192 + 145 = 8337 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_1,                                        /* 8192 + 146 = 8338 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_2,                                        /* 8192 + 147 = 8339 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_3,                                        /* 8192 + 148 = 8340 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_4,                                        /* 8192 + 149 = 8341 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_5,                                        /* 8192 + 150 = 8342 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_6,                                        /* 8192 + 151 = 8343 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_7,                                        /* 8192 + 152 = 8344 */  \
        NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_END, \
        \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_0 = NPHY_M0_LL1D_HIGH_FIXEDMSG_MID_END,   /* 8192 + 153 = 8345 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_1,                                        /* 8192 + 154 = 8346 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_2,                                        /* 8192 + 155 = 8347 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_3,                                        /* 8192 + 156 = 8348 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_4,                                        /* 8192 + 157 = 8349 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_5,                                        /* 8192 + 158 = 8350 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_6,                                        /* 8192 + 159 = 8351 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_7,                                        /* 8192 + 160 = 8352 */  \
        NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_END, \
        \
        NPHY_M0_LL1U_HIGH_FIXEDMSG_MID_0 = NPHY_M1_LL1D_HIGH_FIXEDMSG_MID_END,   /* 8192 + 161 = 8353 */  \
        NPHY_M0_LL1U_HIGH_FIXEDMSG_MID_1,                                        /* 8192 + 162 = 8354 */  \
        NPHY_M0_LL1U_HIGH_FIXEDMSG_MID_2,                                        /* 8192 + 163 = 8355 */  \
        NPHY_M0_LL1U_HIGH_FIXEDMSG_MID_3,                                        /* 8192 + 164 = 8356 */  \
        NPHY_M0_LL1U_HIGH_FIXEDMSG_MID_END, \
        \
        NPHY_M1_LL1U_HIGH_FIXEDMSG_MID_0 = NPHY_M0_LL1U_HIGH_FIXEDMSG_MID_END,   /* 8192 + 165 = 8357 */  \
        NPHY_M1_LL1U_HIGH_FIXEDMSG_MID_1,                                        /* 8192 + 166 = 8358 */  \
        NPHY_M1_LL1U_HIGH_FIXEDMSG_MID_2,                                        /* 8192 + 167 = 8359 */  \
        NPHY_M1_LL1U_HIGH_FIXEDMSG_MID_3,                                        /* 8192 + 168 = 8360 */  \
        NPHY_M1_LL1U_HIGH_FIXEDMSG_MID_END, \
        \
        UPHY_UL1M_FLPM_H_MID = NPHY_M1_LL1U_HIGH_FIXEDMSG_MID_END,                /* 8192 + 169 = 8361 */  \
        UPHY_UL1M_FLPM_H_MID_END, \
        \
        UPHY_LTEVPROC_HL1C_LOW_PROC_MID_0 = UPHY_UL1M_FLPM_H_MID_END,          /* 8192 + 170 = 8362 */  \
        UPHY_LTEVPROC_HL1C_LOW_PROC_MID_END, \
        \
        LPHY_M0_LL1U_SYMBINT_MID = UPHY_LTEVPROC_HL1C_LOW_PROC_MID_END,          /* 8192 + 171 = 8363 */  \
        LPHY_M1_LL1U_SYMBINT_MID,                                                /* 8192 + 172 = 8364 */  \
        LPHY_LL1U_SYMBINT_MID_END, \
        \
        WPHY_UPA_MSG_ENTRY_MID = LPHY_LL1U_SYMBINT_MID_END,                      /* 8192 + 173 = 8365 */  \
        WPHY_UPA_MSG_ENTRY_END, \
        \
        UL1M_LPM_MSG_MID = WPHY_UPA_MSG_ENTRY_END,                               /* 8192 + 174 = 8366 */  \
        UL1M_LPM_MSG_END, \
        \
        GPHY_LOW_MSG_MID_0 = UL1M_LPM_MSG_END,                                   /* 8192 + 175 = 8367 */  \
        GPHY_LOW_MSG_MID_1,                                                      /* 8192 + 176 = 8368 */  \
        GPHY_LOW_MSG_MID_2,                                                      /* 8192 + 177 = 8369 */  \
        GPHY_LOW_MSG_END, \
        \
        GPHY_MNTN_MSG_MID = GPHY_LOW_MSG_END,                                    /* 8192 + 178 = 8370 */  \
        GPHY_MNTN_MSG_END, \
        \
        GPHY_SLEEP_MSG_MID_0 = GPHY_MNTN_MSG_END,                                /* 8192 + 179 = 8371 */  \
        GPHY_SLEEP_MSG_MID_1,                                                    /* 8192 + 180 = 8372 */  \
        GPHY_SLEEP_MSG_MID_2,                                                    /* 8192 + 181 = 8373 */  \
        GPHY_SLEEP_MSG_END, \
        \
        GPHY_WAKE_MSG_MID_0 = GPHY_SLEEP_MSG_END,                                /* 8192 + 182 = 8374 */  \
        GPHY_WAKE_MSG_MID_1,                                                     /* 8192 + 183 = 8375 */  \
        GPHY_WAKE_MSG_MID_2,                                                     /* 8192 + 184 = 8376 */  \
        GPHY_WAKE_MSG_END, \
        \
        UL1M_APM_MSG_MID_0 = GPHY_WAKE_MSG_END,                               /* 8192 + 185 = 8377 */  \
        UL1M_APM_MSG_MID_1,                                                      /* 8192 + 186 = 8378 */  \
        UL1M_APM_MSG_MID_2,                                                      /* 8192 + 187 = 8379 */  \
        UL1M_APM_MSG_MID_END, \
        \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_0 = UL1M_APM_MSG_MID_END,                  /* 8192 + 188 = 8380 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_1,                                         /* 8192 + 189 = 8381 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_2,                                         /* 8192 + 190 = 8382 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_3,                                         /* 8192 + 191 = 8383 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_4,                                         /* 8192 + 192 = 8384 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_5,                                         /* 8192 + 193 = 8385 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_6,                                         /* 8192 + 194 = 8386 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_7,                                         /* 8192 + 195 = 8387 */  \
        NPHY_M0_LL1D_PDE_SSB_CALC_MID_END, \
        \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_0 = NPHY_M0_LL1D_PDE_SSB_CALC_MID_END,     /* 8192 + 196 = 8388 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_1,                                         /* 8192 + 197 = 8389 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_2,                                         /* 8192 + 198 = 8390 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_3,                                         /* 8192 + 199 = 8391 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_4,                                         /* 8192 + 200 = 8392 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_5,                                         /* 8192 + 201 = 8393 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_6,                                         /* 8192 + 202 = 8394 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_7,                                         /* 8192 + 203 = 8395 */  \
        NPHY_M1_LL1D_PDE_SSB_CALC_MID_END, \
        \
        UPHY_UL1M_RCM_D_MID = NPHY_M1_LL1D_PDE_SSB_CALC_MID_END,                 /* 8192 + 204 = 8396 */  \
        UPHY_UL1M_RCM_D_MID_END, \
        \
        UPHY_UL1M_FLPM_MID = UPHY_UL1M_RCM_D_MID_END,                            /* 8192 + 205 = 8397 */  \
        UPHY_UL1M_FLPM_MID_END, \
        \
        NPHY_HL1C_AAGC_IND_MID_0 = UPHY_UL1M_FLPM_MID_END,                       /* 8192 + 206 = 8398 */  \
        NPHY_HL1C_AAGC_IND_MID_1,                                                /* 8192 + 207 = 8399 */  \
        NPHY_HL1C_AAGC_IND_MID_END, \
        \
        LPHY_HL1C_AAGC_IND_MID_0 = NPHY_HL1C_AAGC_IND_MID_END,                   /* 8192 + 208 = 8400 */  \
        LPHY_HL1C_AAGC_IND_MID_1,                                                /* 8192 + 209 = 8401 */  \
        LPHY_HL1C_AAGC_IND_MID_END, \
        \
        NPHY_HL1C_AGC_WB_RSSI_MID_0 = LPHY_HL1C_AAGC_IND_MID_END,                /* 8192 + 210 = 8402 */  \
        NPHY_HL1C_AGC_WB_RSSI_MID_1,                                             /* 8192 + 211 = 8403 */  \
        NPHY_HL1C_AGC_WB_RSSI_MID_END, \
        \
        LPHY_HL1C_AGC_WB_RSSI_MID_0 = NPHY_HL1C_AGC_WB_RSSI_MID_END,             /* 8192 + 212 = 8404 */  \
        LPHY_HL1C_AGC_WB_RSSI_MID_1,                                             /* 8192 + 213 = 8405 */  \
        LPHY_HL1C_AGC_WB_RSSI_MID_END, \
        \
        LTEV_HL1C_AAGC_IND_MID = LPHY_HL1C_AGC_WB_RSSI_MID_END,                  /* 8192 + 214 = 8406 */  \
        LTEV_HL1C_AAGC_IND_MID_END, \
        \
        LTEV_HL1C_AGC_WB_RSSI_MID = LTEV_HL1C_AAGC_IND_MID_END,                  /* 8192 + 215 = 8407 */  \
        LTEV_HL1C_AGC_WB_RSSI_MID_END, \
        \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_0 = LTEV_HL1C_AGC_WB_RSSI_MID_END,       /* 8192 + 216 = 8408 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_1,                                       /* 8192 + 217 = 8409 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_2,                                       /* 8192 + 218 = 8410 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_3,                                       /* 8192 + 219 = 8411 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_4,                                       /* 8192 + 220 = 8412 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_5,                                       /* 8192 + 221 = 8413 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_6,                                       /* 8192 + 222 = 8414 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_7,                                       /* 8192 + 223 = 8415 */  \
        NPHY_M0_BM_CSIRS_POSTHANDLE_MID_END, \
        \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_0 = NPHY_M0_BM_CSIRS_POSTHANDLE_MID_END, /* 8192 + 224 = 8416 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_1,                                       /* 8192 + 225 = 8417 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_2,                                       /* 8192 + 226 = 8418 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_3,                                       /* 8192 + 227 = 8419 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_4,                                       /* 8192 + 228 = 8420 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_5,                                       /* 8192 + 229 = 8421 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_6,                                       /* 8192 + 230 = 8422 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_7,                                       /* 8192 + 231 = 8423 */  \
        NPHY_M1_BM_CSIRS_POSTHANDLE_MID_END, \
        \
        GPHY_HIGH_MSG_MID_0 = NPHY_M1_BM_CSIRS_POSTHANDLE_MID_END,               /* 8192 + 232 = 8424 */  \
        GPHY_HIGH_MSG_MID_1,                                                     /* 8192 + 233 = 8425 */  \
        GPHY_HIGH_MSG_MID_2,                                                     /* 8192 + 234 = 8426 */  \
        GPHY_HIGH_MSG_MID_END, \
        \
        GPHY_MINOR_MSG_MID_0 = GPHY_HIGH_MSG_MID_END,                            /* 8192 + 235 = 8427 */  \
        GPHY_MINOR_MSG_MID_1,                                                    /* 8192 + 236 = 8428 */  \
        GPHY_MINOR_MSG_MID_2,                                                    /* 8192 + 237 = 8429 */  \
        GPHY_MINOR_MSG_MID_END, \
        \
        WPHY_SLEEP_MSG_MID_0 = GPHY_MINOR_MSG_MID_END,                           /* 8192 + 238 = 8430 */  \
        WPHY_SLEEP_MSG_MID_1,                                                    /* 8192 + 239 = 8431 */  \
        WPHY_SLEEP_MSG_MID_END, \
        \
        WPHY_WAKE_MSG_MID_0 = WPHY_SLEEP_MSG_MID_END,                            /* 8192 + 240 = 8432 */  \
        WPHY_WAKE_MSG_MID_1,                                                     /* 8192 + 241 = 8433 */  \
        WPHY_WAKE_MSG_MID_END, \
        \
        WPHY_MNTN_MSG_MID = WPHY_WAKE_MSG_MID_END,                               /* 8192 + 242 = 8434 */  \
        WPHY_MNTN_MSG_END, \
        \
        WPHY_HIGH_MSG_MID_0 = WPHY_MNTN_MSG_END,                                 /* 8192 + 243 = 8435 */  \
        WPHY_HIGH_MSG_MID_1,                                                     /* 8192 + 244 = 8436 */  \
        WPHY_HIGH_MSG_MID_END, \
        \
        UPHY_LTE0SCHED_IRAT_MID = WPHY_HIGH_MSG_MID_END,                          /* 8192 + 245 = 8437 */  \
        UPHY_LTE1SCHED_IRAT_MID,                                                 /* 8192 + 246 = 8438 */  \
        UPHY_NR0SCHED_IRAT_MID,                                                  /* 8192 + 247 = 8439 */  \
        UPHY_SCHED_IRAT_MID_END, \
        \
        NPHY_LL1D_SDR_POST_HANDLE_MID_0 = UPHY_SCHED_IRAT_MID_END,              /* 8192 + 248 = 8440 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_1,                                        /* 8192 + 249 = 8441 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_2,                                        /* 8192 + 250 = 8442 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_3,                                        /* 8192 + 251 = 8443 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_4,                                        /* 8192 + 252 = 8444 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_5,                                        /* 8192 + 253 = 8445 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_6,                                        /* 8192 + 254 = 8446 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_7,                                        /* 8192 + 255 = 8447 */  \
        NPHY_LL1D_SDR_POST_HANDLE_MID_END, \
        \
        LPHY_LL1D_SDR_POST_HANDLE_MID_0 = NPHY_LL1D_SDR_POST_HANDLE_MID_END,    /* 8192 + 256 = 8448 */  \
        LPHY_LL1D_SDR_POST_HANDLE_MID_1,                                        /* 8192 + 257 = 8449 */  \
        LPHY_LL1D_SDR_POST_HANDLE_MID_2,                                        /* 8192 + 258 = 8450 */  \
        LPHY_LL1D_SDR_POST_HANDLE_MID_3,                                        /* 8192 + 259 = 8451 */  \
        LPHY_LL1D_SDR_POST_HANDLE_MID_4,                                        /* 8192 + 260 = 8452 */  \
        LPHY_LL1D_SDR_POST_HANDLE_MID_END, \
        \
        NPHY_M0_LL1U_TOP_ISR_MID_0 = LPHY_LL1D_SDR_POST_HANDLE_MID_END,            /* 8192 + 261 = 8453 */  \
        NPHY_M0_LL1U_TOP_ISR_MID_1,                                                /* 8192 + 262 = 8454 */  \
        NPHY_M0_LL1U_TOP_ISR_MID_2,                                                /* 8192 + 263 = 8455 */  \
        NPHY_M0_LL1U_TOP_ISR_MID_3,                                                /* 8192 + 264 = 8456 */  \
        NPHY_M0_LL1U_TOP_ISR_MID_END, \
        \
        NPHY_M1_LL1U_TOP_ISR_MID_0 = NPHY_M0_LL1U_TOP_ISR_MID_END,                 /* 8192 + 265 = 8457 */  \
        NPHY_M1_LL1U_TOP_ISR_MID_1,                                                /* 8192 + 266 = 8458 */  \
        NPHY_M1_LL1U_TOP_ISR_MID_2,                                                /* 8192 + 267 = 8459 */  \
        NPHY_M1_LL1U_TOP_ISR_MID_3,                                                /* 8192 + 268 = 8460 */  \
        NPHY_M1_LL1U_TOP_ISR_MID_END, \
        \
        NPHY_M0_LL1U_LOW_ISR_MID_0 = NPHY_M1_LL1U_TOP_ISR_MID_END,                 /* 8192 + 269 = 8461 */  \
        NPHY_M0_LL1U_LOW_ISR_MID_1,                                                /* 8192 + 270 = 8462 */  \
        NPHY_M0_LL1U_LOW_ISR_MID_2,                                                /* 8192 + 271 = 8463 */  \
        NPHY_M0_LL1U_LOW_ISR_MID_3,                                                /* 8192 + 272 = 8464 */  \
        NPHY_M0_LL1U_LOW_ISR_MID_END, \
        \
        NPHY_M1_LL1U_LOW_ISR_MID_0 = NPHY_M0_LL1U_LOW_ISR_MID_END,                 /* 8192 + 273 = 8465 */  \
        NPHY_M1_LL1U_LOW_ISR_MID_1,                                                /* 8192 + 274 = 8466 */  \
        NPHY_M1_LL1U_LOW_ISR_MID_2,                                                /* 8192 + 275 = 8467 */  \
        NPHY_M1_LL1U_LOW_ISR_MID_3,                                                /* 8192 + 276 = 8468 */  \
        NPHY_M1_LL1U_LOW_ISR_MID_END, \
        \
        LTEV_SCHED_SYMBINT_HL1C_MID = NPHY_M1_LL1U_LOW_ISR_MID_END,                /* 8192 + 277 = 8469 */  \
        LTEV_SCHED_SYMBINT_LL1C_MID,                                               /* 8192 + 278 = 8470 */  \
        LTEV_SCHED_SYMBINT_MID_END, \
        \
        PHY_MID_END,                                             /* phy max mid num is 8192+4096-1 = 12287 */

#ifdef __cplusplus
}
#endif
#endif
