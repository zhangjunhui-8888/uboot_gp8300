/*
 * (C) Copyright 2010-2012
 * Texas Instruments, <www.ti.com>
 *
 * Aneesh V <aneesh@ti.com>
 * Tom Rini <trini@ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <config.h>
#include <spl.h>
#include <image.h>
#include <linux/compiler.h>
#include <asm/chip2/apiu.h>
#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
#include <asm/chip2/clock.h>
#endif

#ifndef CONFIG_SPL_DM
/* Pointer to as well as the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;

/*
 * WARNING: This is going away very soon. Don't use it and don't submit
 * pafches that rely on it. The global_data area is set up in crt0.S.
 */

#endif

u32 spl_boot_device(void)
{
#if	defined(CONFIG_ARCH_GPT)
	/* Polaris only support nor flash boot */
	return BOOT_DEVICE_NOR;
#else
	int c;

	/*tmp instead boot pin*/
	printf("Please select boot mode\n");
	printf("n: boot from nand\n");
	printf("y: boot from ymodem\n");
	printf("Please input n or y: ");

	c = getc();
	switch(c) {
		case 121:
			return BOOT_DEVICE_UART;
		case 110:
			return BOOT_DEVICE_NAND;
		default:
			return -1;
	}
	return BOOT_DEVICE_NONE;
#endif
}


/*
 * In the context of SPL, board_init_f must ensure that any clocks/etc for
 * DDR are enabled, ensure that the stack pointer is valid, clear the BSS
 * and call board_init_f.  We provide this version by default but mark it
 * as __weak to allow for platforms to do this in their own way if needed.
 */
void __weak board_init_f(ulong dummy)
{

#if     defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	gpt_clock_init();
#endif

	gpt_apiu_init();
	serial_init();
	preloader_console_init();
	board_init_r(NULL, 0);
}

/*
 * This function jumps to an image with argument. Normally an FDT or ATAGS
 * image.
 * arg: Pointer to paramter image in RAM
 */
#ifdef CONFIG_SPL_OS_BOOT
void __noreturn jump_to_image_linux(void *arg)
{
	unsigned long machid = 0xffffffff;
#ifdef CONFIG_MACH_TYPE
	machid = CONFIG_MACH_TYPE;
#endif

	debug("Entering kernel arg pointer: 0x%p\n", arg);
	typedef void (*image_entry_arg_t)(int, int, void *)
		__attribute__ ((noreturn));
	image_entry_arg_t image_entry =
		(image_entry_arg_t) spl_image.entry_point;
	cleanup_before_linux();
	image_entry(0, machid, arg);
}
#endif
