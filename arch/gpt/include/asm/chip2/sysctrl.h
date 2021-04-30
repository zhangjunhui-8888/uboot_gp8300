#ifndef __GPT_SYSCTRL_H__
#define __GPT_SYSCTRL_H__

#include <asm/io.h>

#define	SYSCTRL_CORE0_CCR	0xF0000000
#define	SYSCTRL_CORE1_CCR	0xF0000004
#define	SYSCTRL_CORE2_CCR	0xF0000008
#define	SYSCTRL_CORE3_CCR	0xF000000C
#define	SYSCTRL_EXTAPB_CCR	0xF0000010
#define	SYSCTRL_ACC_DEL		0xF000001C
#define	SYSCTRL_CORE0_ISLR	0xF0000020
#define	SYSCTRL_CORE1_ISLR	0xF0000024
#define	SYSCTRL_CORE2_ISLR	0xF0000028
#define	SYSCTRL_CORE3_ISLR	0xF000002C
#define	SYSCTRL_PLL_CFG_RD	0xF0000030
#define	SYSCTRL_CPLL_CNTL	0xF0000034
#define	SYSCTRL_DPLL_CNTL	0xF0000038
#define	SYSCTRL_EXTCR_LO	0xF0000100
#define	SYSCTRL_EXTCR_HI	0xF0000104
#define	SYSCTRL_IOCNF		0xF0000200

/* Bits of SYSCTRL_COREx_CCR register */
#define CORE_CLK_EN		(1 << 4)
#define CORE_CLK_GATE		(1 << 5)

/* Bits of SYSCTRL_EXTCR_LO register */
#define	GSNN0_EN	(1 << 0)
#define	GSNN0_CLK_EN	(1 << 1)
#define	GSNN1_EN	(1 << 2)
#define	GSNN1_CLK_EN	(1 << 3)
#define	VCAP0_EN	(1 << 4)
#define	VCAP1_EN	(1 << 5)
#define	VCAP2_EN	(1 << 6)
#define	VOUT_EN		(1 << 7)
#define	VIDMEM_EN	(1 << 8)
#define	SPINOR_EN	(1 << 9)
#define	SD_EN		(1 << 10)
#define	PCIE_EN		(1 << 11)
#define	DDR_EN		(1 << 12)
#define	VID_EN		(1 << 13)
#define DDR_CLK_EN	(1 << 14)

/* Bits of SYSCTRL_EXTCR_HI register */
#define	L2LO_MEM_SLP	(1 << 0)
#define	L2LO_MEM_SD	(1 << 1)
#define	L2HI_MEM_SLP	(1 << 2)
#define	L2HI_MEM_SD	(1 << 3)
#define GSNN0_MEM_SLP	(1 << 4)
#define GSNN0_MEM_SD	(1 << 5)
#define GSNN1_MEM_SLP	(1 << 6)
#define GSNN1_MEM_SD	(1 << 7)
#define VIDMEM_MEM_SLP	(1 << 8)
#define VIDMEM_MEM_SD	(1 << 9)

/* Mask of SYSCTRL_EXTCR_LO register */
#define EXTCR_CLK_MASK  (GSNN0_CLK_EN | GSNN1_CLK_EN | \
			DDR_CLK_EN)
#define EXTCR_SUBSYS_MASK (0xFF & (~EXTCR_CLK_MASK))

static inline void sysctrl_subsys_enable(uint32_t val)
{
	uint32_t tmp, bitmask;
	
	val &= EXTCR_SUBSYS_MASK;
	bitmask = (val << 16);
	tmp = val | bitmask;	
	writel(tmp, SYSCTRL_EXTCR_LO);
}

static inline void sysctrl_clk_disable(uint32_t val)
{
	uint32_t tmp, bitmask;
	
	val &= EXTCR_CLK_MASK;
	bitmask = (val << 16);
	tmp = val | bitmask;	
	writel(tmp, SYSCTRL_EXTCR_LO);
}

static inline void sysctrl_mem_enable(uint32_t val)
{
	uint32_t tmp, bitmask;
	
	bitmask = val << 16;
	tmp = (~val) & bitmask;	
	writel(tmp, SYSCTRL_EXTCR_HI);
}

#endif
