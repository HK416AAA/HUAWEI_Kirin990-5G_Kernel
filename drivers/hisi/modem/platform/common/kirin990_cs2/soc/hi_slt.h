#ifndef __HI_COMMON_H__
#define __HI_COMMON_H__ 
#include <product_config.h>
#include "bsp_memmap.h"
#ifdef MBB_SLT
#ifndef B5000_SLT
#define B5000_SLT 
#endif
#define CONFIG_SLT_LR_CPU_NUM 2
#define CONFIG_SLT_NR_CPU_NUM 2
#define CONFIG_SLT_HAC_CPU_NUM 2
#define CONFIG_SLT_LR_TEST_MAX_NUM 32
#define CONFIG_SLT_NR_TEST_MAX_NUM 10
#define CONFIG_SLT_HAC_TEST_MAX_NUM 10
#define MODEM_SLT_LR_START_ADDR (0x80000000)
#define MODEM_SLT_LR_STATUS (MODEM_SLT_LR_START_ADDR)
#define MODEM_SLT_LR_STATUS_SIZE (0x4*CONFIG_SLT_LR_CPU_NUM)
#define MODEM_SLT_LR_RESULT (MODEM_SLT_LR_STATUS + MODEM_SLT_LR_STATUS_SIZE)
#define MODEM_SLT_LR_RESULT_SIZE (0x4*CONFIG_SLT_LR_CPU_NUM)
#define MODEM_SLT_LR_INDEX (MODEM_SLT_LR_RESULT + MODEM_SLT_LR_RESULT_SIZE)
#define MODEM_SLT_LR_INDEX_SIZE (0x4)
#define MODEM_SLT_LR_ARGUMENTS (MODEM_SLT_LR_INDEX + MODEM_SLT_LR_INDEX_SIZE)
#define MODEM_SLT_LR_ARGUMENTS_SIZE (0x4 * 4 *CONFIG_SLT_LR_TEST_MAX_NUM)
#define MODEM_SLT_NR_START_ADDR (0x80100000)
#define MODEM_SLT_NR_STATUS (MODEM_SLT_NR_START_ADDR)
#define MODEM_SLT_NR_STATUS_SIZE (0x4*CONFIG_SLT_NR_CPU_NUM)
#define MODEM_SLT_NR_RESULT (MODEM_SLT_NR_STATUS + MODEM_SLT_NR_STATUS_SIZE)
#define MODEM_SLT_NR_RESULT_SIZE (0x4*CONFIG_SLT_NR_CPU_NUM)
#define MODEM_SLT_NR_INDEX (MODEM_SLT_NR_RESULT + MODEM_SLT_NR_RESULT_SIZE)
#define MODEM_SLT_NR_INDEX_SIZE (0x4)
#define MODEM_SLT_NR_ARGUMENTS (MODEM_SLT_NR_INDEX + MODEM_SLT_NR_INDEX_SIZE)
#define MODEM_SLT_NR_ARGUMENTS_SIZE (0x4 * 4 *CONFIG_SLT_NR_TEST_MAX_NUM)
#define MODEM_SLT_HAC_START_ADDR (0x80200000)
#define MODEM_SLT_HAC_STATUS (MODEM_SLT_HAC_START_ADDR)
#define MODEM_SLT_HAC_STATUS_SIZE (0x4*CONFIG_SLT_HAC_CPU_NUM)
#define MODEM_SLT_HAC_RESULT (MODEM_SLT_HAC_STATUS + MODEM_SLT_HAC_STATUS_SIZE)
#define MODEM_SLT_HAC_RESULT_SIZE (0x4*CONFIG_SLT_HAC_CPU_NUM)
#define MODEM_SLT_HAC_INDEX (MODEM_SLT_HAC_RESULT + MODEM_SLT_HAC_RESULT_SIZE)
#define MODEM_SLT_HAC_INDEX_SIZE (0x4)
#define MODEM_SLT_HAC_ARGUMENTS (MODEM_SLT_HAC_INDEX + MODEM_SLT_HAC_INDEX_SIZE)
#define MODEM_SLT_HAC_ARGUMENTS_SIZE (0x4 * 4 *CONFIG_SLT_HAC_TEST_MAX_NUM)
#define SLT_NR_IMAGE_BASE_ADDR (DDR_MCORE_NR_ADDR)
#define SLT_HAC_IMAGE_BASE_ADDR (HAC_SLT_LLRAM_ADDR)
#define SLT_LR_IMAGE_BASE_ADDR (DDR_SLT_MCORE_ADDR)
#define MODEM_SLT_PASS (0xA8A8A8A8)
#define SLT_OK 0
#define SLT_ERROR 1
#define LR_RESET_ADDR_REG (0xC0200400)
#define LR_UNRESET_REG (0xC0200024)
#define LR_UNHALT_REG (0xC020042c)
#define NR_RESET_ADDR_REG (0xD9201000)
#define NR_UNRESET_REG (0xD8000094)
#define NR_UNHALT_REG (0xD9201010)
#define HAC_RESET_ADDR_REG (0xF8801000)
#define HAC_UNRESET_REG (0xF800007C)
#define HAC_UNHALT_REG (0xF8801010)
#define CCPU5G_SYSCTL 0xf9201000
#define MODEM5G_CRG 0xf8000000
#define CCPU4G_SYSCTL_PERI 0xE4001000
#define CCPU4G_SYSCTRL 0xE0200000
static __inline__ void slt_prepare_ccpu_a9(void)
{
}
#define slt_writel(addr,value) (*(volatile unsigned *) (HI_IO_ADDRESS(addr))) = (value)
#define slt_readl(addr) (*(volatile unsigned *) (HI_IO_ADDRESS(addr)))
static __inline__ void slt_start_lrccpu(unsigned int addr)
{
  unsigned int rdata;
 addr = addr & 0xffffe000;
 rdata = slt_readl(0xE02004FC);
 rdata = rdata & (~(0x8));
 slt_writel(0xE02004FC, rdata);
 do{
  rdata = slt_readl(0xE0200684);
 }while((rdata & 0x80008) != 0);
 slt_writel(0xE0200020, 0x8000);
 slt_writel(0xE0200C18, 0x1);
 do{
  rdata = slt_readl(0xE0200E04);
 }while((rdata & 0x1) != 1);
 slt_writel(0xE0200C10, 0x1);
 do{
  rdata = slt_readl(0xEDF00F30);
 }while((rdata & 0x20) != 0x20);
 slt_writel(0xE0200010, 0x1000000);
 slt_writel(0xE0200024, 0x8);
 slt_writel(0xE0200070, 0x2667);
 rdata = slt_readl(0xE020042C);
 rdata = rdata & (~(0x300));
 slt_writel(0xE020042C, rdata);
 slt_writel(0xE0200024, 0x9300);
 (*(volatile unsigned *) (HI_IO_ADDRESS(CCPU4G_SYSCTRL + 0x400))) = (addr | 0x1);
 rdata = slt_readl(0xE020042C);
 rdata = rdata & (~(0x30));
 slt_writel(0xE020042C, rdata);
 (*(volatile unsigned *) (HI_IO_ADDRESS(CCPU4G_SYSCTRL + 0x24))) = (0x2040);
 rdata = (*(volatile unsigned *) (HI_IO_ADDRESS(CCPU4G_SYSCTRL + 0x42c)));
    rdata = rdata | (0x3<<8);
 (*(volatile unsigned *) (HI_IO_ADDRESS(CCPU4G_SYSCTRL + 0x42c))) = (rdata);
}
static __inline__ void slt_start_nrccpu(unsigned int addr)
{
  unsigned int rdata;
 addr = addr & 0xffffe000;
 rdata = slt_readl(0xF8000110);
 rdata = rdata | (0x80);
 rdata = rdata & (~(0x40));
 slt_writel(0xF8000110, rdata);
 slt_writel(0xF8001608, 0x2);
 do{
  rdata = slt_readl(0xF8001618);
 }while((rdata&0x2) != 0x2);
 slt_writel(0xF8001604, 0x2);
 do{
  rdata = slt_readl(0xEDF00F30);
 }while((rdata&0x4000) != 0x4000);
 slt_writel(0xF8000000, 0x8000800);
 slt_writel(0xF8000064, 0x20000800);
 rdata = slt_readl(0xF80010A4);
 rdata = rdata & (~(0x2));
 slt_writel(0xF80010A4, rdata);
 do{
  rdata = slt_readl(0xF8001240);
 }while((rdata&0xC) != 0);
 rdata = slt_readl(0xF8000110);
 rdata = rdata | (0x40);
 rdata = rdata & (~(0x80));
 slt_writel(0xF8000110, rdata);
 slt_writel(0xF8000030, 0x7fffc);
 rdata = slt_readl(0xF9201010);
 rdata = rdata & (~(0xF00));
 slt_writel(0xF9201010, rdata);
 slt_writel(0xF8000094, 0xffc30e);
 (*(volatile unsigned *) (HI_IO_ADDRESS(CCPU5G_SYSCTL + 0x0))) = (addr | 0x1);
 (*(volatile unsigned *) (HI_IO_ADDRESS(MODEM5G_CRG + 0x94))) = (0xF0);
 rdata = (*(volatile unsigned *) (HI_IO_ADDRESS(CCPU5G_SYSCTL + 0x10)));
    rdata = rdata | (0xf<<8 );
 (*(volatile unsigned *) (HI_IO_ADDRESS(CCPU5G_SYSCTL + 0x10))) = (rdata);
}
static __inline__ void slt_start_hac(unsigned int addr)
{
    unsigned int sys_ctrl = 0;
 slt_writel(0xF8001608, 0x4);
 do{
  sys_ctrl = slt_readl(0xF8001618);
 }while((sys_ctrl&0x4) != 0x4);
 slt_writel(0xF8001604, 0x4);
 do{
  sys_ctrl = slt_readl(0xEDF00F30);
 }while((sys_ctrl&0x8000) != 0x8000);
 slt_writel(0xF8000000, 0x1000080);
 slt_writel(0xF8000064, 0x2001000);
 sys_ctrl = slt_readl(0xF80010A4);
 sys_ctrl = sys_ctrl & (~(0x8));
 slt_writel(0xF80010A4, sys_ctrl);
 do{
  sys_ctrl = slt_readl(0xF8001240);
 }while((sys_ctrl&0xC0) != 0x0);
 slt_writel(0xF8000020, 0x4003ff0);
 sys_ctrl = slt_readl(0xF8801010);
 sys_ctrl = sys_ctrl & (~(0x300));
 slt_writel(0xF8801010, sys_ctrl);
 slt_writel(0xF800007C, 0x1f99a);
    sys_ctrl = (*(volatile unsigned *) (HI_IO_ADDRESS(HAC_RESET_ADDR_REG)));
    (*(volatile unsigned *) (HI_IO_ADDRESS(HAC_RESET_ADDR_REG))) = (0x1 | (addr));
 sys_ctrl = slt_readl(0xF8801010);
 sys_ctrl = sys_ctrl & (~(0x30));
 slt_writel(0xF8801010, sys_ctrl);
    sys_ctrl = (*(volatile unsigned *) (HI_IO_ADDRESS(HAC_UNRESET_REG)));
    (*(volatile unsigned *) (HI_IO_ADDRESS(HAC_UNRESET_REG))) = (0x60 | sys_ctrl);
 sys_ctrl = (*(volatile unsigned *) (HI_IO_ADDRESS(HAC_UNHALT_REG)));
    (*(volatile unsigned *) (HI_IO_ADDRESS(HAC_UNHALT_REG))) = (0x300 | sys_ctrl);
}
#endif
#endif
