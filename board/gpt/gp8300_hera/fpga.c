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

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOARD_EARLY_INIT_F
int board_early_init_f (void)
{
	gpt_apiu_init();	
	timer_init();
	return 0;
}
#endif

static ulong arch_get_sp(void)
{
	ulong ret;

	__asm__ __volatile__("rseta %0, $asp" : "=r"(ret) : );

	return ret;
}

int dram_init(void)
{	
	gd->ram_size = get_ram_size(
		(void *)CONFIG_SYS_SDRAM_BASE,
		CONFIG_SYS_DDR_SDRAM_SIZE);

#if defined(CONFIG_TARGET_CHASE_XILINX) || defined(CONFIG_TARGET_GP8300_TCX) || defined(CONFIG_TARGET_GP8300_HERA)
	ulong sp = arch_get_sp();
	printf("\nCurrent stack ends at 0x%08lx\n", sp);
        writel(0x1000000, (int *)0xf1042110);
        writel(0x40, (int *)0xf104210c);
#endif
	
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
	return ret;
}

