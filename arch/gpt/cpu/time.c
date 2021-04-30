/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/spr.h>
#include "asm/chip2/clock.h"

int timer_init()
{
        // Enable and reset clock source 
        sprset_gpt(TIMC, 1 << 0);
        sprset_gpt(TIM, 0);

	return 0;
}

unsigned long notrace timer_read_counter(void)
{
	return sprget_gpt(TIM);
}

ulong notrace get_tbclk(void)
{
	ulong gpt_timer_freq;

#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
#ifdef CONFIG_CPLL_2000MHZ	
	gpt_timer_freq = CPLL_2000MHZ/8;			/* 1956MHz */
#endif

#ifdef CONFIG_CPLL_1900MHZ	
	gpt_timer_freq = CPLL_1900MHZ/8;			/* 1908MHz */
#endif

#ifdef CONFIG_CPLL_1800MHZ	
	gpt_timer_freq = CPLL_1800MHZ/8;			/* 1800MHz */
#endif

#ifdef CONFIG_CPLL_1600MHZ	
	gpt_timer_freq = CPLL_1600MHZ/8;			/* 1596MHz */
#endif
#elif defined(CONFIG_TARGET_CHASE_XILINX) || defined(CONFIG_TARGET_GP8300_FPGA) || defined(CONFIG_TARGET_GP8300_TCX) || defined(CONFIG_TARGET_GP8300_HERA)
	gpt_timer_freq = CONFIG_SYS_GPT_TIMER_FREQ;	
#endif

	return gpt_timer_freq;
}
