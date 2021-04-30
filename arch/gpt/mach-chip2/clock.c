#include <common.h>

#include "asm/io.h"
#include "asm/chip2/sysctrl.h"
#include "asm/chip2/clock.h"

#define	CPLL_REG_OFF	0x34
#define	DPLL_REG_OFF	0x38

#define PDIV_MASK	0x3f
#define MDIV_MASK	0xff
#define NDIV_MASK	0x1f
#define BYPASS_MASK	0x1

#define PDIV_SHIFT	0
#define MDIV_SHIFT	6
#define NDIV_SHIFT	14
#define BYPASS_SHIFT	19
#define TAG_SHIFT	28
#define LOCK_SHIFT	31

cpll_val_e cpll_hz;

/*	Chip2's clock mapping
 *		      |---CPU0
 *		      |---CPU1
 *	    	      |---CPU2
 *    	              |---CPU3
 *	    |-- CPLL->|---VIDMEM
 *	    |	      |---GSNN
 *OSC-----> |         |---SD
 *	    |         |---VCAP->|--VOUT
 *	    |         |---SPINOR
 *	    |         |---L2C->|--APIU->|---apiu peripheral clocks
 *	    |	      |---EXTAPB->apb peripheral clocks
 *	    |
 *	    |-- DPLL->|---DDR
 *	    |
 *	    |-- PCIE
 *
 * Default:
 * OSC = 12MHz
 * CPLL = 1248MHz
 * L2C = 1/2 CPLL;
 * APIU = 1/64 L2C;
 */

struct clk_div_table{
	int val;
	int div;
};

static struct clk_div_table common_div_table[] =
{
        { .val = 7,  .div = GPTCLK_DIV1  },
        { .val = 15, .div = GPTCLK_DIV1  },
        { .val = 0,  .div = GPTCLK_DIV2  },       /*default value*/
        { .val = 8,  .div = GPTCLK_DIV3  },
        { .val = 1,  .div = GPTCLK_DIV4  },
        { .val = 9,  .div = GPTCLK_DIV6  },
        { .val = 2,  .div = GPTCLK_DIV8  },
        { .val = 10, .div = GPTCLK_DIV12 },
        { .val = 3,  .div = GPTCLK_DIV16 },
        { .val = 11, .div = GPTCLK_DIV24 },
        { .val = 4,  .div = GPTCLK_DIV32 },
        { .val = 12, .div = GPTCLK_DIV48 },
        { .val = 13, .div = GPTCLK_DIV48 },
        { .val = 14, .div = GPTCLK_DIV48 },
        { .val = 5,  .div = GPTCLK_DIV64 },
        { .val = 6,  .div = GPTCLK_DIV64 },
        { } /* sentinel */
};

static struct clk_div_table apiu_div_table[] =
{
        { .val = 0,  .div = GPTCLK_DIV4  },
        { .val = 1,  .div = GPTCLK_DIV8  },
        { .val = 2,  .div = GPTCLK_DIV16 },
        { .val = 3,  .div = GPTCLK_DIV32 },
        { .val = 4,  .div = GPTCLK_DIV64 },	/*default value*/
        { .val = 5,  .div = GPTCLK_DIV64 },
        { .val = 6,  .div = GPTCLK_DIV64 },
        { .val = 7,  .div = GPTCLK_DIV64 },
        { } /* sentinel */
};

static struct clk_div_table i2s_div_table[] =
{
        { .val = 0,  .div = GPTCLK_DIV1 },       /*default value*/
        { .val = 1,  .div = GPTCLK_DIV2 },
        { .val = 2,  .div = GPTCLK_DIV4 },
        { .val = 3,  .div = GPTCLK_DIV8 },
        { } /* sentinel */
};

static inline int clk_get_div(struct clk_div_table *tb, int size, int val)
{
	int i;
	for(i=0; i<size; i++)
		if(tb[i].val == val)
			return tb[i].div;
	return -1;
}

static inline int clk_get_val(struct clk_div_table *tb, int size, int div)
{
	int i;
	for(i=0; i<size; i++)
		if(tb[i].div == div)
			return tb[i].val;
	return -1;
}

static inline unsigned long gpt_get_osc(void)
{
	unsigned long osc_hz;
	
	osc_hz = OSC_12MHZ;

	return osc_hz;
}

static int gpt_set_pll(void *reg, unsigned long rate, unsigned long parent_rate)
{
	u32 mdiv, ndiv, pdiv, M, N, P;
	u32 val;
	/* 187.5MHZ >= Fin/N >= 7.5MHZ, 
	 * default OSC freq is 12MHZ, So N should be 1;
	 */
	if( (parent_rate < 7500000) || (parent_rate > 187500000))
	{
		printf("Input OSC is not supported, \
			it should be 187.5MHZ >= Fin/N >= 7.5MHZ\n");
		return -1;
	}
	/*  Fvco = (M/N)*Fin, When FSE = 1;
	 *  Fout = Fvco/P
	 *  Fvco = (M*P/N)*Fin, when FSE = 0;
	 *  Fout = Fvco/P
	 *  7.5MHz <= Fin/N <= 187.5MHz
	 *  1200MHz <= Fvco <= 3000MHz
	 */
	if(rate > 3000000000) {
		printf("Desired rate %ld is not supported\n", rate);
		return -2;
	}else if(rate >= 1200000000) {
		N = 1;
		P = 1;
		M = (rate*P)/ parent_rate;
	}else if(rate >=64000000) {
		N = 1;
		P = 4;	/* 1<=P<=64 */
		M = (rate*P) / parent_rate; /* 16<=M<=256 */
	}else{
                printf("Desired rate %ld is not supported\n", rate);
                return -3;
	}
	ndiv = (N - 1) << NDIV_SHIFT;
	pdiv = (P - 1) << PDIV_SHIFT;
	mdiv = (M - 1) << MDIV_SHIFT;
	val = 0xA << TAG_SHIFT | ndiv | pdiv | mdiv;

	writel(val, reg);
	
	return 0;
}

static unsigned long gpt_get_pll(void *reg, unsigned long parent_rate)
{
	unsigned long m, n, p, rate;
	u32 pllcon;

	pllcon = readl(reg);
	if ((pllcon >> BYPASS_SHIFT) & BYPASS_MASK) {
		printf("%s: pll is bypassed\n", __func__);
		return parent_rate;
	}
	
	p = ((pllcon >> PDIV_SHIFT) & PDIV_MASK) + 1;
	m = ((pllcon >> MDIV_SHIFT) & MDIV_MASK) + 1;
	n = ((pllcon >> NDIV_SHIFT) & NDIV_MASK) + 1;
	
	rate = ((m/n)*parent_rate)/p;
	
	return (unsigned long)rate;
}

unsigned long gpt_get_dpll(void)
{
	u32 *dpllreg = (u32*)(SYSCTRL_DPLL_CNTL);
	return gpt_get_pll(dpllreg, gpt_get_osc());
}

int gpt_set_dpll(unsigned long rate)
{
	int ret;
	u32 *dpllreg = (u32*)(SYSCTRL_DPLL_CNTL);
	ret = gpt_set_pll(dpllreg, rate, gpt_get_osc());
	
	/* wait DPLL locked */
	while(readl(dpllreg)>>LOCK_SHIFT) ;
	
	return ret;
}

unsigned long gpt_get_cpll(void)
{
	u32 *cpllreg = (u32*)(SYSCTRL_CPLL_CNTL);
	return gpt_get_pll(cpllreg, gpt_get_osc());
}

int gpt_set_cpll(unsigned long rate)
{
	u32 *cpllreg = (u32*)(SYSCTRL_CPLL_CNTL);
	
	return gpt_set_pll(cpllreg, rate, gpt_get_osc());
}

unsigned long gpt_get_cpuclk(u8 cpuid)
{
	unsigned long parent_rate, rate;
	u32 *ccreg = (u32*)SYSCTRL_CORE0_CCR + cpuid;
	u8 val = readl(ccreg) & 0xF;
	int div = clk_get_div(common_div_table, ARRAY_SIZE(common_div_table), val);
	if(div < 0)
		return 0;

	parent_rate = gpt_get_cpll();
	rate = parent_rate / div;
	return rate;
}

void gpt_set_cpuclk(u8 cpuid, enum clk_div div)
{
	u32 regval;
	u32 *ccreg = (u32*)SYSCTRL_CORE0_CCR + cpuid;
	int val = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), div);
	if(val < 0){
		printf("invalid dividor %d\n", div);
		return ;
	}
	regval = readl(ccreg) & (~0xF);
	regval |= val;
	writel(regval, ccreg);
}

unsigned long gpt_get_l2cclk(void)
{
	unsigned long parent_rate, rate;
	u32 *ccreg = (u32*)SYSCTRL_EXTAPB_CCR;
	u8 val = (readl(ccreg)>>4) & 0xF;
	int div = clk_get_div(common_div_table, ARRAY_SIZE(common_div_table), val);
	if(div < 0)
		return 0;
	parent_rate = gpt_get_cpll();
	rate = parent_rate / div;

	return rate;
}

static inline void gpt_gsnn_disable(void)
{
	sysctrl_clk_disable(GSNN0_CLK_EN | GSNN1_CLK_EN);
	sysctrl_subsys_enable(GSNN0_EN | GSNN1_EN);
}

static inline void gpt_cpu_gate(uint32_t cpuid)
{
	u32 *ccreg = (u32*)SYSCTRL_CORE0_CCR + cpuid;
	/* gate cpu clk */
	u32 val = readl(ccreg) | CORE_CLK_GATE;
	writel(val, ccreg);
}

static void gpt_set_periph_clks(void)
{
	u32 extclk_div, val, mask;
	u32 *ext_ccreg = (u32*)SYSCTRL_EXTAPB_CCR;
	int maxdiv = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), GPTCLK_DIV64);
	int vidmem_div = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), GPTCLK_DIV4);
	int apbdiv = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), GPTCLK_DIV8);
	int sddiv = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), GPTCLK_DIV32);

	extclk_div =	(maxdiv << 8) | \
			(vidmem_div << 12) | \
			(apbdiv << 16) | \
			(sddiv << 20);
	mask = 0x00ffff00;
	val = readl(ext_ccreg) & (~mask);
	writel(extclk_div | val, ext_ccreg);
}

void gpt_set_periph_clk(enum clk_dev devid, enum clk_div div)
{
	u32 regval, val, mask, shift;

	u32 *extreg = (u32*)SYSCTRL_EXTAPB_CCR;

	switch(devid)
	{
		case APB_PERIPH_APIU:
		val = clk_get_val(apiu_div_table, ARRAY_SIZE(apiu_div_table), div);
		shift = 0; mask = 0x7;
		break;
		case APB_PERIPH_GSNN:
		val = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), div);
		shift = 8; mask = 0xf;
		break;
		case APB_PERIPH_VMEM:
		val = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), div);
		shift = 12; mask = 0xf;
		break;
		case APB_PERIPH_APB:
		val = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), div);
		shift = 16; mask = 0xf;
		break;
		case APB_PERIPH_SD:
		val = clk_get_val(common_div_table, ARRAY_SIZE(common_div_table), div);
		shift = 20; mask = 0xf;
		break;
		case APB_PERIPH_I2S:
		val = clk_get_val(i2s_div_table, ARRAY_SIZE(i2s_div_table), div);
		shift = 24; mask = 0x3;
		break;
		default:
		printf("invalid devid %d\n", devid);
		return ;
	}
	if(val < 0){
		printf("invalid dividor %d\n", div);
		return ;
	}
	regval = readl(extreg) & (~(mask << shift));
	regval |= val << shift;
	writel(regval, extreg);
}

unsigned long gpt_get_periph_clk(enum clk_dev devid)
{
	u32 regval, val;
	unsigned long parent_rate, rate;
	int div;

	u32 *extreg = (u32*)SYSCTRL_EXTAPB_CCR;
	regval = readl(extreg);
	switch(devid)
	{
		case APB_PERIPH_APIU:
		val = regval & 0x7;
		div = clk_get_div(apiu_div_table, ARRAY_SIZE(apiu_div_table), val);
		parent_rate = gpt_get_l2cclk();
		break;
		case APB_PERIPH_GSNN:
		val = (regval>>8) & 0xf;
		div = clk_get_div(common_div_table, ARRAY_SIZE(common_div_table), val);
		parent_rate = gpt_get_cpll();
		break;
		case APB_PERIPH_VMEM:
		val = (regval>>12) & 0xf;
		div = clk_get_div(common_div_table, ARRAY_SIZE(common_div_table), val);
		parent_rate = gpt_get_cpll();
		break;
		case APB_PERIPH_APB:
		val = (regval>>16) & 0xf;
		div = clk_get_div(common_div_table, ARRAY_SIZE(common_div_table), val);
		parent_rate = gpt_get_cpll();
		break;
		case APB_PERIPH_SD:
		val = (regval>>20) & 0xf;
		div = clk_get_div(common_div_table, ARRAY_SIZE(common_div_table), val);
		parent_rate = gpt_get_cpll();
		break;
		case APB_PERIPH_I2S:
		val = (regval>>24) & 0x3;
		div = clk_get_div(i2s_div_table, ARRAY_SIZE(i2s_div_table), val);
		parent_rate = CONFIG_GPT_I2S_REFCLK;
		break;
		default:
		printf("invalid devid %d\n", devid);
		return 0;
	}
	if(div < 0){
		printf("unexcepted regval %x\n", val);
		return 0;
	}
	rate = parent_rate/div;
	return rate;
}

static void gpt_cpll_setup(void)
{
	unsigned long rate;

#ifdef CONFIG_CPLL_2000MHZ	
	cpll_hz = CPLL_2000MHZ;			/* 1956MHz */
#endif

#ifdef CONFIG_CPLL_1900MHZ	
	cpll_hz = CPLL_1900MHZ;			/* 1908MHz */
#endif

#ifdef CONFIG_CPLL_1800MHZ	
	cpll_hz = CPLL_1800MHZ;			/* 1800MHz */
#endif

#ifdef CONFIG_CPLL_1600MHZ	
	cpll_hz = CPLL_1600MHZ;			/* 1596MHz */
#endif

	rate = gpt_get_cpll();
	if(rate != cpll_hz) {
		gpt_set_cpll(cpll_hz);
		/* System will reboot once CPLL register is written */
		while(1);
	}
}

static void gpt_clks_setup(void)
{
	/* Disable necessary clocks to save power */
	gpt_set_cpuclk(0, GPTCLK_DIV1);
	gpt_cpu_gate(1);
	gpt_cpu_gate(2);
	gpt_cpu_gate(3);
	gpt_gsnn_disable();

	gpt_set_periph_clks();
	gpt_set_periph_clk(APB_PERIPH_APIU, GPTCLK_DIV4);
}

void gpt_clock_init(void)
{
	gpt_cpll_setup();
	gpt_clks_setup();
}
