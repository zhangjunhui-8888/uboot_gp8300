#include <linux/types.h>
#include <common.h>
#include <asm/io.h>
#include <asm/chip2/apiu.h>


void gpt_apiu_gpio_config(enum APIU_DEV dev, uint32_t param)
{
	unsigned long base = (unsigned long)dev;
	writel(param, base | APIU_UNIT_UCR);
}

void gpt_apiu_uart_config(enum APIU_DEV dev, uint32_t param)
{
	unsigned long base = (unsigned long)dev;
	writel(param, base | APIU_UNIT_UCR);
}

void gpt_apiu_i2s_config(enum APIU_DEV dev, uint32_t param)
{
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
	unsigned long base = (unsigned long)dev;
	writel(param, base | APIU_UNIT_UCR);
}

void gpt_apiu_timer_config(enum APIU_DEV dev, uint32_t param)
{
	unsigned long base = (unsigned long)dev;
	writel(param, base | 0x10);
}

void gpt_apiu_wdt_config(enum APIU_DEV dev, uint32_t param)
{
	unsigned long base = (unsigned long)dev;
	writel(param, base | 0x10);
}

void gpt_apiu_spi_config(enum APIU_DEV dev, uint32_t param)
{
	unsigned long base = (unsigned long)dev;
	writel(param, base | APIU_UNIT_UCR);
}

void gpt_apiu_i2c_config(enum APIU_DEV dev, uint32_t param)
{
	unsigned long base = (unsigned long)dev;
	writel(param, base | APIU_UNIT_UCR);
}

