/*
 * (C) Copyright 2007
 * Vlad Lungu vlad.lungu@windriver.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>

#include <asm/chip2/apiu.h>
#include <asm/chip2/sysctrl.h>
#include <asm/sections.h>
#include <asm/io.h>
#include <netdev.h>
#include <net.h>
#include "asm/chip2/clock.h"
#include "asm/chip2/gpt_ddr3l.h"
#include "exception.h"

DECLARE_GLOBAL_DATA_PTR;

int gpio_init (void)
{
#ifdef CONFIG_XILINX_GPIO
	gpio_alloc_dual(CONFIG_SYS_GPIO_0_ADDR,"xgpio", 0, 32);
#endif
	return 0;
}
void gpt_set_extscr(void)
{
	int i;
	unsigned int regs;
#define GPT_EXT_SUB_BASE	0xf0000100
	regs = readl(GPT_EXT_SUB_BASE);
	for (i = 9; i < 12; i++)
		if (GPT_EXTSCR_DEFAULT & (1 << i))
			regs |= (1 << i) | (1 << (i + 0x10));
	writel(regs, GPT_EXT_SUB_BASE);
}

int board_early_init_r(void)
{
	gpt_set_extscr();
	gpt_debug_init();
	return 0;
}

#ifdef CONFIG_BOARD_EARLY_INIT_F
int board_early_init_f (void)
{

//	gpt_apiu_init();	
	timer_init();
	return 0;
}
#endif
#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	gpio_init();
        return 0;
}
#endif

int dram_init(void)
{

	gpt_ddr_init();
	gd->ram_size = get_ram_size(
		(void *)CONFIG_SYS_SDRAM_BASE,
		CONFIG_SYS_DDR_SDRAM_SIZE);

	writel(0x1000000, (int *)0xf1042110);
	writel(0x40, (int *)0xf104210c);
	return 0;
}

int debug_pod_test(void)
{
	u32 val;

#define GPT_DEBUG_POD_TEST	0xf0007110
	val = readl(GPT_DEBUG_POD_TEST);
	if(0xDBDB == val){
		printf("===========================GPT DEBUG POD TEST START!"
						"===========================\n");	
		__asm__ __volatile__(" IDLE ");
	}
	return 0;	
}

int checkboard(void)
{
	u32 proc_id=0;

	printf("Board: GPT CPU: %d\r\n",proc_id);

	printf("===========================GPT CPU AND PERIPH CLK"
						"===========================\n");
	printf("CPU0 CLK: %10ldhz, L2 CACHE CLK: %10ldhz\n",
				gpt_get_cpuclk(0), gpt_get_l2cclk());
	printf("DPLL CLK: %10ldhz, DDR IO SPEED: %10ldbps\n",
				gpt_get_dpll(), gpt_get_dpll() * 4);
	printf("APIU CLK: %10ldhz, GSNN CLK: %10ldhz, VIDMEM   CLK: %10ldhz\n",
				gpt_get_periph_clk(APB_PERIPH_APIU),
				gpt_get_periph_clk(APB_PERIPH_GSNN),
				gpt_get_periph_clk(APB_PERIPH_VMEM));
	printf("APB  CLK: %10ldhz, SD   CLK: %10ldhz, I2S      CLK: %10ldhz\n",
				gpt_get_periph_clk(APB_PERIPH_APB),
				gpt_get_periph_clk(APB_PERIPH_SD),
				gpt_get_periph_clk(APB_PERIPH_I2S));
	printf("================================================="
						"===========================\n");
	debug_pod_test();

	return 0;
}

int board_eth_init(bd_t *bis)
{
	int ret = 0;
	printf("Board: init eth\n");
#ifdef CONFIG_ETHADDR
	eth_parse_enetaddr(__stringify(CONFIG_ETHADDR), gd->bd->bi_enetaddr);
#endif

#ifdef CONFIG_GPT_AXIEMAC
	ret |= xilinx_axiemac_initialize(bis, XILINX_AXIEMAC_BASEADDR,
                                     XILINX_AXIDMA_BASEADDR);
#endif	

	pci_eth_init(bis);

	return ret;
}
