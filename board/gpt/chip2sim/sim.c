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
