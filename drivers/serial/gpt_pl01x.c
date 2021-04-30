/*
 * (C) Copyright 2000
 * Rob Taylor, Flying Pig Systems. robt@flyingpig.com.
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* Simple U-Boot driver for the GPT PL010/PL011 UARTs */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <watchdog.h>
#include <asm/io.h>
#include <serial.h>
#include <dm/platform_data/serial_pl01x.h>
#include <linux/compiler.h>
#include "serial_pl01x_internal.h"
#include <fdtdec.h>
#include <asm/chip2/clock.h>

#if defined(CONFIG_GPT_CHIP2)
#include <asm/chip2/apiu.h>
#elif defined(CONFIG_TARGET_GP8300_FPGA)
#include <asm/polaris/apiu.h>
#elif defined(CONFIG_GPT_CHIP1)
#include <asm/chip1/apiu.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#ifndef CONFIG_DM_SERIAL
static volatile unsigned char *const port[] = CONFIG_GPT_PL01x_PORTS;
static enum pl01x_type pl01x_type __attribute__ ((section(".data")));
static struct pl01x_regs *base_regs __attribute__ ((section(".data")));

#if defined(CONFIG_GPT_APIU_UART0)
static enum UART uart_index = UART0;
static enum APIU_DEV apiu_dev = APIU_UART0;
#elif defined(CONFIG_GPT_APIU_UART1)
static enum UART uart_index = UART1;
static enum APIU_DEV apiu_dev = APIU_UART1;
#elif defined(CONFIG_GPT_APIU_UART2)
static enum UART uart_index = UART2;
static enum APIU_DEV apiu_dev = APIU_UART2;
#elif defined(CONFIG_GPT_APIU_UART3)
static enum UART uart_index = UART3;
static enum APIU_DEV apiu_dev = APIU_UART3;
#endif
#endif

#ifdef CONFIG_APIU
#undef	writew
#undef	writeb
#undef	readw
#undef	readb
#define writew(dev, val, addr)		gpt_apiu_writew(dev, addr, val)
#define writeb(dev, val, addr)		gpt_apiu_writeb(dev, addr, val)
#define readw(dev, addr)		gpt_apiu_readw(dev, addr)
#define readb(dev, addr)		gpt_apiu_readb(dev, addr)
#endif

static int pl01x_putc(struct pl01x_regs *regs, char c)
{
	uint32_t value;
	/* Wait until there is space in the FIFO */

	value =  (readw(apiu_dev, &regs->fr) & UART_PL01x_FR_TXFF);
	while (value) {
		value = (readw(apiu_dev, &regs->fr) & UART_PL01x_FR_TXFF);
	}

	if (c == '\n') {
		pl01x_putc(regs, '\r');
	}
	/* Send the character */
	writew(apiu_dev, c & 0xff, &regs->dr);

	while (readw(apiu_dev, &regs->fr) & UART_PL01x_FR_BUSY)
		;

	return 0;
}

static int pl01x_getc(struct pl01x_regs *regs)
{
	unsigned int data;

	/* Wait until there is data in the FIFO */
	data =  (readw(apiu_dev, &regs->fr) & UART_PL01x_FR_RXFE);
	while (data) {
		data = (readw(apiu_dev, &regs->fr) & UART_PL01x_FR_RXFE);
	}
	data = readw(apiu_dev, &regs->dr);

	return (int) data;
}

static int pl01x_tstc(struct pl01x_regs *regs)
{
	WATCHDOG_RESET();
	return !(readw(apiu_dev, &regs->fr) & UART_PL01x_FR_RXFE);
}

static int pl01x_generic_serial_init(struct pl01x_regs *regs,
				     enum pl01x_type type)
{
	switch (type) {
	case TYPE_PL010:
		/* disable everything */
		writew(apiu_dev, 0, &regs->pl010_cr);
		break;
	case TYPE_PL011:
		/* disable everything */
		writew(apiu_dev, 0, &regs->pl011_cr);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int pl011_set_line_control(struct pl01x_regs *regs)
{
	unsigned int lcr;
	/*
	 * Internal update of baud rate register require line
	 * control register write
	 */
	lcr = UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN;
	writew(apiu_dev, lcr, &regs->pl011_lcrh);
	return 0;
}

static int pl01x_generic_setbrg(struct pl01x_regs *regs, enum pl01x_type type,
				int clock, int baudrate)
{
	switch (type) {
	case TYPE_PL010: {
		unsigned int divisor;

		/* disable everything */
		writew(apiu_dev, 0, &regs->pl010_cr);

		switch (baudrate) {
		case 9600:
			divisor = UART_PL010_BAUD_9600;
			break;
		case 19200:
			divisor = UART_PL010_BAUD_9600;
			break;
		case 38400:
			divisor = UART_PL010_BAUD_38400;
			break;
		case 57600:
			divisor = UART_PL010_BAUD_57600;
			break;
		case 115200:
			divisor = UART_PL010_BAUD_115200;
			break;
		default:
			divisor = UART_PL010_BAUD_38400;
		}

		writew(apiu_dev, (divisor & 0xf00) >> 8, &regs->pl010_lcrm);
		writew(apiu_dev, divisor & 0xff, &regs->pl010_lcrl);

		/*
		 * Set line control for the PL010 to be 8 bits, 1 stop bit,
		 * no parity, fifo enabled
		 */
		writew(apiu_dev, UART_PL010_LCRH_WLEN_8 | UART_PL010_LCRH_FEN,
		       &regs->pl010_lcrh);
		/* Finally, enable the UART */
		writew(apiu_dev, UART_PL010_CR_UARTEN, &regs->pl010_cr);
		break;
	}
	case TYPE_PL011: {
		unsigned int temp;
		unsigned int divider;
		unsigned int remainder;
		unsigned int fraction;

		/*
		* Set baud rate
		*
		* IBRD = UART_CLK / (16 * BAUD_RATE)
		* FBRD = RND((64 * MOD(UART_CLK,(16 * BAUD_RATE)))
		*		/ (16 * BAUD_RATE))
		*/
		temp = 16 * baudrate;
		divider = clock / temp;
		remainder = clock % temp;
		temp = (8 * remainder) / baudrate;
		fraction = (temp >> 1) + (temp & 1);

		writew(apiu_dev, divider, &regs->pl011_ibrd);
		writeb(apiu_dev, fraction, &regs->pl011_fbrd);

		pl011_set_line_control(regs);
		/* Finally, enable the UART */
		writew(apiu_dev, UART_PL011_CR_UARTEN | UART_PL011_CR_TXE |
		       UART_PL011_CR_RXE | UART_PL011_CR_RTS, &regs->pl011_cr);
		break;
	}
	default:
		return -EINVAL;
	}

	return 0;
}

#ifndef CONFIG_DM_SERIAL
static void pl01x_serial_init_baud(int baudrate)
{
	int clock = 0;

#if defined(CONFIG_GPT_PL010_SERIAL)
	pl01x_type = TYPE_PL010;
#elif defined(CONFIG_GPT_PL011_SERIAL)
	pl01x_type = TYPE_PL011;
#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	clock = gpt_get_periph_clk(APB_PERIPH_APIU);
#elif defined(CONFIG_TARGET_CHASE_XILINX) || defined(CONFIG_TARGET_GP8300_FPGA) || defined(CONFIG_TARGET_GP8300_TCX)
	clock = CONFIG_GPT_PL011_CLOCK;
#endif
#endif

	base_regs = (struct pl01x_regs *)port[uart_index];

	pl01x_generic_serial_init(base_regs, pl01x_type);
	pl01x_generic_setbrg(base_regs, pl01x_type, clock, baudrate);
}

/*
 * Integrator AP has two UARTs, we use the first one, at 38400-8-N-1
 * Integrator CP has two UARTs, use the first one, at 38400-8-N-1
 * Versatile PB has four UARTs.
 */
int pl01x_serial_init(void)
{
#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG) || defined(CONFIG_TARGET_CHASE_XILINX) || \
    defined(CONFIG_TARGET_GP8300_FPGA) || defined(CONFIG_TARGET_GP8300_TCX)
        /* Enable APIU Uart Unit */
        gpt_apiu_uart_config(apiu_dev, APIU_UART_ENABLE| \
                                APIU_UART_BQ_DIS| \
                                APIU_UART_IRQ_EN);
#endif
	pl01x_serial_init_baud(CONFIG_BAUDRATE);

	return 0;
}

static void pl01x_serial_putc(const char c)
{
#if 0
	if (c == '\n')
		while (pl01x_putc(base_regs, '\r') == -EAGAIN);
#endif
	while (pl01x_putc(base_regs, c) == -EAGAIN);
}

static int pl01x_serial_getc(void)
{
	while (1) {
		int ch = pl01x_getc(base_regs);

		if (ch == -EAGAIN) {
			WATCHDOG_RESET();
			continue;
		}

#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	//udelay(0x100);  /* delay for RS485 */
#endif
		return ch;
	}
}

static int pl01x_serial_tstc(void)
{
	return pl01x_tstc(base_regs);
}

static void pl01x_serial_setbrg(void)
{
	/*
	 * Flush FIFO and wait for non-busy before changing baudrate to avoid
	 * crap in console
	 */
	while (!(readw(apiu_dev, &base_regs->fr) & UART_PL01x_FR_TXFE))
		WATCHDOG_RESET();
	while (readw(apiu_dev, &base_regs->fr) & UART_PL01x_FR_BUSY)
		WATCHDOG_RESET();
	pl01x_serial_init_baud(gd->baudrate);
}

static struct serial_device pl01x_serial_drv = {
	.name	= "pl01x_serial",
	.start	= pl01x_serial_init,
	.stop	= NULL,
	.setbrg	= pl01x_serial_setbrg,
	.putc	= pl01x_serial_putc,
	.puts	= default_serial_puts,
	.getc	= pl01x_serial_getc,
	.tstc	= pl01x_serial_tstc,
};

void pl01x_serial_initialize(void)
{
	serial_register(&pl01x_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
	return &pl01x_serial_drv;
}

#endif /* nCONFIG_DM_SERIAL */

#ifdef CONFIG_DM_SERIAL

struct pl01x_priv {
	struct pl01x_regs *regs;
	enum pl01x_type type;
};

static int pl01x_serial_setbrg(struct udevice *dev, int baudrate)
{
	struct pl01x_serial_platdata *plat = dev_get_platdata(dev);
	struct pl01x_priv *priv = dev_get_priv(dev);

	pl01x_generic_setbrg(priv->regs, priv->type, plat->clock, baudrate);

	return 0;
}

static int pl01x_serial_probe(struct udevice *dev)
{
	struct pl01x_serial_platdata *plat = dev_get_platdata(dev);
	struct pl01x_priv *priv = dev_get_priv(dev);

	priv->regs = (struct pl01x_regs *)plat->base;
	priv->type = plat->type;
	return pl01x_generic_serial_init(priv->regs, priv->type);
}

static int pl01x_serial_getc(struct udevice *dev)
{
	struct pl01x_priv *priv = dev_get_priv(dev);

	return pl01x_getc(priv->regs);
}

static int pl01x_serial_putc(struct udevice *dev, const char ch)
{
	struct pl01x_priv *priv = dev_get_priv(dev);

	return pl01x_putc(priv->regs, ch);
}

static int pl01x_serial_pending(struct udevice *dev, bool input)
{
	struct pl01x_priv *priv = dev_get_priv(dev);
	unsigned int fr = readw(&priv->regs->fr);

	if (input)
		return pl01x_tstc(priv->regs);
	else
		return fr & UART_PL01x_FR_TXFF ? 0 : 1;
}

static const struct dm_serial_ops pl01x_serial_ops = {
	.putc = pl01x_serial_putc,
	.pending = pl01x_serial_pending,
	.getc = pl01x_serial_getc,
	.setbrg = pl01x_serial_setbrg,
};

#if CONFIG_IS_ENABLED(OF_CONTROL)
static const struct udevice_id pl01x_serial_id[] ={
	{.compatible = "gpt,pl011", .data = TYPE_PL011},
	{.compatible = "gpt,pl010", .data = TYPE_PL010},
	{}
};

static int pl01x_serial_ofdata_to_platdata(struct udevice *dev)
{
	struct pl01x_serial_platdata *plat = dev_get_platdata(dev);
	fdt_addr_t addr;

	addr = dev_get_addr(dev);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	plat->base = addr;
	plat->clock = fdtdec_get_int(gd->fdt_blob, dev->of_offset, "clock", 1);
	plat->type = dev_get_driver_data(dev);
	return 0;
}
#endif

U_BOOT_DRIVER(serial_pl01x) = {
	.name	= "serial_pl01x",
	.id	= UCLASS_SERIAL,
	.of_match = of_match_ptr(pl01x_serial_id),
	.ofdata_to_platdata = of_match_ptr(pl01x_serial_ofdata_to_platdata),
	.platdata_auto_alloc_size = sizeof(struct pl01x_serial_platdata),
	.probe = pl01x_serial_probe,
	.ops	= &pl01x_serial_ops,
	.flags = DM_FLAG_PRE_RELOC,
	.priv_auto_alloc_size = sizeof(struct pl01x_priv),
};

#endif
