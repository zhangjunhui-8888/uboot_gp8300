#ifndef __CLOCK_H__
#define __CLOCK_H__

enum clk_div {
	GPTCLK_DIV1  = 1,
	GPTCLK_DIV2  = 2,
	GPTCLK_DIV3  = 3,
	GPTCLK_DIV4  = 4,
	GPTCLK_DIV6  = 6,
	GPTCLK_DIV8  = 8,
	GPTCLK_DIV12 = 12,
	GPTCLK_DIV16 = 16,
	GPTCLK_DIV24 = 24,
	GPTCLK_DIV32 = 32,
	GPTCLK_DIV48 = 48,
	GPTCLK_DIV64 = 64,
};

enum clk_dev {
	APB_PERIPH_APIU = 0,
	APB_PERIPH_GSNN = 1,
	APB_PERIPH_VMEM = 2,
	APB_PERIPH_APB  = 3,
	APB_PERIPH_SD	= 4,
	APB_PERIPH_I2S	= 5
};

enum osc_e {
	OSC_12MHZ = 12000000,
};

typedef enum cpll_val {
	CPLL_2000MHZ = 1956000000,
	CPLL_1900MHZ = 1908000000,
	CPLL_1800MHZ = 1800000000,
	CPLL_1600MHZ = 1596000000,
}cpll_val_e;

cpll_val_e cpll_hz;

unsigned long gpt_get_l2cclk(void);
unsigned long gpt_get_dpll(void);
int gpt_set_dpll(unsigned long rate);
unsigned long gpt_get_cpll(void);
int gpt_set_cpll(unsigned long rate);
unsigned long gpt_get_cpuclk(u8 cpuid);
void gpt_set_cpuclk(u8 cpuid, enum clk_div div);
void gpt_set_periph_clk(enum clk_dev devid, enum clk_div div);
unsigned long gpt_get_periph_clk(enum clk_dev devid);
void gpt_clock_init(void);

#endif
