/*
 * (C) Copyright 2007
 * Vlad Lungu vlad.lungu@windriver.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>

#include <asm/chip2/apiu.h>
#include <asm/sections.h>
#include <asm/io.h>
#include <netdev.h>
#include <net.h>

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
	for (i = 0; i < 15; i++)
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
	gpt_apiu_init();	
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
	
	gd->ram_size = get_ram_size(
		(void *)CONFIG_SYS_SDRAM_BASE,
		CONFIG_SYS_DDR_SDRAM_SIZE);
	return 0;
}

int checkboard(void)
{
	u32 proc_id=0;

	printf("Board: GPT CPU: %d\r\n",proc_id);

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
