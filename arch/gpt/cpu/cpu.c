/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <netdev.h>
#include <linux/compiler.h>
#include <asm/chip2/apiu.h>
#include <asm/chip2/sysctrl.h>

void __weak _machine_restart(void)
{
	unsigned int val;
	val = readl(SYSCTRL_CPLL_CNTL);
	val &=~0xfff80000;
	writel(val | 0xa0000000, SYSCTRL_CPLL_CNTL);

	while (1)
		/* NOP */;
}
int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	_machine_restart();

	return 0;
}

#ifdef CONFIG_GPT_WATCHDOG
/* Hardware timeout in seconds */
#define WDT_HW_TIMEOUT 30

#define WDT_START		BIT(0)
#define WDT_STOP		!(BIT(0))
#define CPU0_WDT_ACTIVE		BIT(4)

struct gpt_wdt_regs {
	u32 clkdiv;
	u32 init_val;
	u32 curent_val;
	u32 wdt_rcc;
	u32 wdt_ctrl;
	u32 res1;
	u32 res2;
	u32 intr_st;
};

struct gpt_wdt_config {
	u32 clkdiv;
	u32 init_val;
	u32 feed;
};

static unsigned int wdt_feed_cpu0 = 0x1ACCE770;

void hw_watchdog_reset(void)
{
	struct gpt_wdt_regs *wdt = (struct gpt_wdt_regs *) APIU_WDTIMER;
	
	writel(wdt_feed_cpu0, &wdt->wdt_rcc);
}

void hw_watchdog_enable(void)
{
	struct gpt_wdt_regs *wdt = (struct gpt_wdt_regs *) APIU_WDTIMER;

	writel(WDT_START | CPU0_WDT_ACTIVE, &wdt->wdt_ctrl);	// enable cpu0 wdt.
}

void hw_watchdog_disable(void)
{
	struct gpt_wdt_regs *wdt = (struct gpt_wdt_regs *) APIU_WDTIMER;

	/*
	 * Disable watchdog
	 */
	//writel(wdt_feed_cpu0, &wdt->wdt_rcc);
	writel(~wdt_feed_cpu0, &wdt->wdt_rcc);
	
	writel(0, &wdt->wdt_ctrl);
}

int hw_watchdog_settimeout(unsigned long val)
{
	struct gpt_wdt_regs * wdt = (struct gpt_wdt_regs *) APIU_WDTIMER;

	if (val <= 0){
		hw_watchdog_disable();
	}
	else if (val >= 2) { // great 2 is save.
		writel(val, &wdt->init_val);	//decline 16s.
		if( val != readl(&wdt->init_val)){
			return -1;
		}
	}

	return 0;
}

/*
 *	cpll	= 1800000000
 *	l2clk	= cpull/2 = 1800000000/2 = 900000000 //0x35A4E900
 *
 *	apiu_clk= l2clk/4 = 900000000/4  = 225000000
 *	apb_clk = cpll/8  = 1800000000/8 = 225000000 
 *	
 * */
void hw_watchdog_init(void)
{
	struct gpt_wdt_regs *wdt = (struct gpt_wdt_regs *) APIU_WDTIMER;

	writel(0x35A4E900, &wdt->clkdiv);

	hw_watchdog_settimeout(WDT_HW_TIMEOUT);

	hw_watchdog_enable();
}
#endif
