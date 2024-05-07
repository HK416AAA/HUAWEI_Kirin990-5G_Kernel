#ifndef __SOC_SCTRL_INTERFACE_H__
#define __SOC_SCTRL_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#define SOC_SCTRL_SC_PERIPH_CTRL4_ADDR(base) ((base) + (0x410))
#define SOC_SCTRL_SC_PERIPH_CTRL5_ADDR(base) ((base) + (0x414))
#define SOC_SCTRL_SC_STAT8_REG_ADDR(base) ((base) + (0x620))
#define SOC_SCTRL_SC_STAT8_CICOM0_BIT_POS (0)
#define SOC_SCTRL_SC_STAT8_CICOM1_BIT_POS (1)
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gucicom0sel_mode : 1;
        unsigned int gucicom1sel_mode : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_SCtrl_SC_PERIPH_CTRL1_UNION;
#define SOC_SCtrl_SC_PERIPH_CTRL1_cicomsel_mode_START (0)
#define SOC_SCtrl_SC_PERIPH_CTRL1_cicomsel_mode_END (0)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
