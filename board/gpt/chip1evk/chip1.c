/*
 * (C) Copyright 2007
 * Vlad Lungu vlad.lungu@windriver.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>

#include <asm/sections.h>
#include <netdev.h>
#include <net.h>

#include <machine/gpt_apiu.h>

DECLARE_GLOBAL_DATA_PTR;

/* Define board data structure */
static bd_t bdata __attribute__ ((section(".data")));

int gpio_init (void)
{
#ifdef CONFIG_XILINX_GPIO
	gpio_alloc_dual(CONFIG_SYS_GPIO_0_ADDR,"xgpio", 0, 32);
#endif
	return 0;
}

void board_init(void)
{
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	gpio_init();
        return 0;
}
#endif

phys_size_t initdram(int board_type)
{
	return MEM_SIZE*1024;
}

phys_size_t get_effective_memsize(void)
{
	return MEM_SIZE*1024;
}

int checkboard(void)
{
	u32 proc_id=0;

	printf("Board: GPT CPU: %d\r\n",proc_id);

	return 0;
}

int misc_init_r(void)
{
	set_io_port_base(0);
	return 0;
}

void setup_gd(ulong top)
{
/*	memset((void *)gd, '\0', sizeof(gd_t));
*/
	gd->bd = &bdata;
	gd->have_console = 1;
	gd->mon_len = (ulong)&__bss_end - CONFIG_SYS_MONITOR_BASE;
	gd->ram_size = initdram(0);
	gd->ram_top = 0x80000;
	gd->relocaddr = 0x80000;
	gd->start_addr_sp =top;
	gd->baudrate = CONFIG_BAUDRATE;

	timer_init();
#ifdef  CONFIG_PL011_SERIAL
	if(CONFIG_APIU_PORT==APIU_UART1)
		gpt_apiu_uart_init(APB_CTRL_REG_UART1, EXT_BUS_CLK_DIV64);
	else
		gpt_apiu_uart_init(APB_CTRL_REG_UART1, EXT_BUS_CLK_DIV64);
#endif
	serial_init();

  /* init ethaddr */
#ifdef CONFIG_ETHADDR
 	eth_parse_enetaddr(__stringify(CONFIG_ETHADDR), gd->bd->bi_enetaddr);
#endif
//	debug("Ram top: %08lX\n", (ulong)gd->ram_top);
}

int board_eth_init(bd_t *bis)
{
	int ret = 0;
	printf("Board: init eth\r\n");
#ifdef CONFIG_XILINX_AXIEMAC
	ret |= xilinx_axiemac_initialize(bis, XILINX_AXIEMAC_BASEADDR,
                                     XILINX_AXIDMA_BASEADDR);
#endif

	return ret;
}
