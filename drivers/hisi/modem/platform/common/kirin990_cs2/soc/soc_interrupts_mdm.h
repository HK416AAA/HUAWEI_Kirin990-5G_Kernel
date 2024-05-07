#ifndef __SOC_INTERRUPTS_MDM_H
#define __SOC_INTERRUPTS_MDM_H 
#ifdef __cplusplus
extern "C" {
#endif
#include "soc_interrupts_comm.h"
#define INT_LVL_IPCM2MDM0 32
#define INT_LVL_IPCM2MDM1 33
#define INT_LVL_MDM_COMMRX 34
#define INT_LVL_MDM_COMMTX 35
#define INT_LVL_MDM_GPIO0 112
#define INT_LVL_MDM_GPIO1 113
#define INT_LVL_MDM_GPIO2 114
#define INT_LVL_MDM_GPIO3 115
#define INT_LVL_MDM_RESERVE0 125
#define INT_LVL_MDM_RESERVE1 126
#define INT_LVL_SYSCONCTRL2 132
#define INT_LVL_SYSCONCTRL3 133
#define INT_LVL_APP_EVENT0 142
#define INT_LVL_PCIE1_LINK_DOWN_INT 199
#define INT_LVL_PCIE1_EDMA_INT 200
#define INT_LVL_PCIE1_PM_INT 201
#define INT_LVL_PCIE1_RADM_INTA 202
#define INT_LVL_PCIE1_RADM_INTB 203
#define INT_LVL_PCIE1_RADM_INTC 204
#define INT_LVL_PCIE1_RADM_INTD 205
#define INT_LVL_MODA9_DEFLAGS_COMB_INT 208
#define INT_LVL_PCIE_AXI_DLOCK_INT 209
#define INT_LVL_USB3_INT1 210
#define INT_LVL_USB3_INT2 211
#ifdef __cplusplus
}
#endif
#endif