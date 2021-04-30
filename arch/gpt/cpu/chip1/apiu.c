/*
The APB_Peripheral Interface Unit (APIU) provides access to two UARTs and two 8-bit
GPIO blocks. The APIU is accessed through a direct memory path, with internal
translation (onto an APB bus) performed for communicating with the APB compliant
peripherals.
*/
#include <common.h>
#include <asm/io.h>
#include <asm/mach-chip1/apiu.h>
#include <linux/types.h>

/*
 * 31------------------16-----12-----------2----1----0
 *  |       F000       | unit | reg addr   |uart| w/r|
 */
void gpt_apiu_enable(uint32_t dev_en)
{
	dev_en=dev_en &	0xf0000000;
	gpt_apiu_write(APIU_CTRL,0,dev_en);
}

uint32_t gpt_apiu_read(enum APIU_DEV dev, void* reg)
{
	uint64_t addr = (dev | ((uint64_t)reg & 0xfff) | 0x1);

	writeb(0, addr);
	udelay(1);

	return (uint32_t)readl(addr);
}

void gpt_apiu_write(enum APIU_DEV dev, void* reg, uint32_t val)
{
	writel(val, (dev | ((uint64_t)reg & 0xffe)));
}

void gpt_apiu_uart_init(uint32_t uart_en, uint32_t div)
{
	gpt_apiu_enable(uart_en);
        gpt_apiu_write(APIU_SYS1, 0, div);
}



