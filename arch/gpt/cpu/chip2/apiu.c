/*
The APB_Peripheral Interface Unit (APIU) provides access to two UARTs and two 8-bit
GPIO blocks. The APIU is accessed through a direct memory path, with internal
translation (onto an APB bus) performed for communicating with the APB compliant
peripherals.
*/
#include <common.h>
#include <asm/io.h>
#include <asm/chip2/apiu.h>
#include <linux/types.h>

/*
 * 31------------------16-----12-----------2----1----0
 *  |       F000       | unit | reg addr   |uart| w/r|
 */
void gpt_apiu_enable(uint32_t dev_en)
{
	//dev_en=dev_en &	0xf0000000;
	//gpt_apiu_write(APIU_CTRL,0,dev_en);
}

static inline void apiu_tx_fifo_full(uint64_t dev)
{	
#if 1
	uint32_t status;
	do{
		status = readl((uint64_t)(dev | APIU_UNIT_USFR));
	}while(status & ((0x1 << 4) << 16));
#else
	udelay(1);
#endif
}

static inline void apiu_rx_fifo_not_empty(uint64_t dev)
{
#if 1
	uint32_t status;
	do{
		status = readl((uint64_t)(dev | APIU_UNIT_USFR));
	}while(!(status & (0x1 << 1) << 16));
#else
	udelay(1);
#endif
}

uint32_t gpt_apiu_readl(void *dev, void* reg)
{
	uint64_t addr = (((uint64_t)reg & RW_MASK) | 0x1);

	apiu_tx_fifo_full((uint64_t)dev);
	writeb(0, addr);
	apiu_rx_fifo_not_empty((uint64_t)dev);
	return readl((uint64_t)reg & RW_MASK);
}

uint16_t gpt_apiu_readw(enum APIU_DEV dev, void* reg)
{
	uint64_t addr = (((uint64_t)reg & RW_MASK) | 0x1);

	apiu_tx_fifo_full(dev);
	writeb(0, addr);
	apiu_rx_fifo_not_empty(dev);
	return readl((uint64_t)reg & RW_MASK) & 0xffff;
}

uint8_t gpt_apiu_readb(enum APIU_DEV dev, void* reg)
{
	uint64_t addr = (((uint64_t)reg & RW_MASK) | 0x1);

	apiu_tx_fifo_full((uint64_t)dev);
	writeb(0, addr);
	apiu_rx_fifo_not_empty((uint64_t)dev);
	return readl((uint64_t)reg & RW_MASK) & 0xff;
}

void gpt_apiu_writeb(enum APIU_DEV dev, void* reg, uint8_t val)
{
	apiu_tx_fifo_full(dev);
	writel(val, ((uint64_t)reg & RW_MASK));
}

void gpt_apiu_writew(enum APIU_DEV dev, void* reg, uint16_t val)
{
	apiu_tx_fifo_full(dev);
	writel(val, ((uint64_t)reg & RW_MASK));
}

void gpt_apiu_writel(void *dev, void* reg, uint32_t val)
{
	apiu_tx_fifo_full((uint64_t)dev);
	writel(val, ((uint64_t)reg & RW_MASK));
}

void gpt_apiu_uart_init(uint32_t uart_en, uint32_t div)
{
	//gpt_apiu_enable(uart_en);
	//gpt_apiu_write(APIU_SYS1, 0, div);
}

void gpt_apiu_init(void)
{
#if 0
	/* SYS Control Unit configure */
	 writel(0x0, APIU_CCCR1);  
	 writel(0x0, APIU_CCCR2);  
	 writel(0x0, APIU_CCCR3);  
	 writel(0x0, APIU_CCCR4);  

	/* Clock Control configure */
	writel((EXT_BUS_CLK_DIV4 << EXT3_CLK_SHIFT) | (EXT_BUS_CLK_DIV4 << EXT2_CLK_SHIFT) |(EXT_BUS_CLK_DIV4 << EXT1_CLK_SHIFT) |
			(L2_BUS_CLK_DIV1 << L2S_CLK_SHIFT) | 
			APB_BUS_CLK_DIV4, 
			APIU_ICCR);
#endif	
	/* GPIO Unit Configure */
	writel(0x1, APIU_GPIO1 | APIU_UNIT_UCR);
	writel(0x1, APIU_GPIO2 | APIU_UNIT_UCR);
	writel(0x1, APIU_GPIO3 | APIU_UNIT_UCR);
	writel(0x1, APIU_GPIO4 | APIU_UNIT_UCR);
	writel(0x1, APIU_GPIO5 | APIU_UNIT_UCR);
	writel(0x1, APIU_GPIO6 | APIU_UNIT_UCR);
	writel(0x1, APIU_GPIO7 | APIU_UNIT_UCR);
	writel(0x1, APIU_GPIO8 | APIU_UNIT_UCR);

	/* UART Unit Configure */
	writel((0xFFFF << 16) | (0x88 << 8) | 0x1, APIU_UART0 | APIU_UNIT_UCR);
	writel((0xFFFF << 16) | (0x88 << 8) | 0x1, APIU_UART1 | APIU_UNIT_UCR);
	writel((0xFFFF << 16) | (0x88 << 8) | 0x1, APIU_UART2 | APIU_UNIT_UCR);
	writel((0xFFFF << 16) | (0x88 << 8) | 0x1, APIU_UART3 | APIU_UNIT_UCR);

	 /* I2S Unit Configure 
	  * Func                :         Shift bits
	  * I2S Mode            :         0  00-I2S 01-Left 1X-Right
	  * Sample Size         :         2  00-16b 01-20b 10-24b 11-32b
	  * Frame Size          :         4  000-32b 001-48b 010-64b 011-128b 100-256b 1XX-384b
	  * Unit Enable         :         7  1-en 
	  * Queue Threshold     :         8
	  * Flag Mask Enable    :         16
	  * Master/Slave Select :         31 1-M 0-S
	  * */
	 writel(0x1 << 7, APIU_I2S1 | APIU_UNIT_UCR);
	 writel(0x1 << 7, APIU_I2S2 | APIU_UNIT_UCR);

	 /* GP/WD TIMER Unit Configure */
	 writel(0, APIU_GPTIMER1 | 0x10);
	 writel(0, APIU_GPTIMER2 | 0x10);
	 writel(0, APIU_GPTIMER3 | 0x10);
	 writel(0, APIU_GPTIMER4 | 0x10);
	 writel(0, APIU_WDTIMER  | 0x10);

	 /* SPI Unit Configure */
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_SPI1 | APIU_UNIT_UCR);
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_SPI2 | APIU_UNIT_UCR);
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_SPI3 | APIU_UNIT_UCR);

	 /* I2C Unit Configure */
#ifdef CONFIG_TEST_ON_SOC 
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_I2C1 | APIU_UNIT_UCR);
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_I2C2 | APIU_UNIT_UCR);
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_I2C3 | APIU_UNIT_UCR);
#else
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_I2C1 | APIU_UNIT_UCR);
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_I2C2 | APIU_UNIT_UCR);
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_I2C3 | APIU_UNIT_UCR);
	 writel((0x1FFF << 16) | (0x55 << 8) | 0x3, APIU_I2C4 | APIU_UNIT_UCR);
#endif
}




