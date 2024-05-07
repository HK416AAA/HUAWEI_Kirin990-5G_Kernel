#ifndef __MNTN_SUB_EXCEPTION_H__
#define __MNTN_SUB_EXCEPTION_H__ 
typedef enum NR_SUB_EXC_TYPE
{
    NR_CCPU_DRV = 0,
    NR_CCPU_PAM,
    NR_CCPU_NAS,
    NR_CCPU_AS,
    NR_CCPU_PS,
    NR_CCPU_PHY,
    NR_L2HAC_DRV,
    NR_L2HAC_PAM,
    NR_L2HAC_NAS,
    NR_L2HAC_AS,
    NR_L2HAC_PS,
    NR_L2HAC_PHY,
    NR_HL1C,
};
#define NR_SUB_EXC(x) (#x)
#endif
