/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file contains the configuration parameters for qemu-mips64 target.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_CHIP2_SIM

#define CONFIG_BOARD_EARLY_INIT_F

/* Enable flat device tree support */
#define CONFIG_OF_LIBFDT	1 
/* #define CONFIG_DISPLAY_BOARDINFO */
/* #define CONFIG_MISC_INIT_R */
/* #define CONFIG_REINIT_GLOBAL_DATA */

#define CONFIG_BOOTDELAY	4	/* autoboot after 4 seconds */

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */

#define CONFIG_SYS_MHZ			50
#define CONFIG_EXT_MHZ			50
#define MHz 			     	(1000 * 1000)

#define	CONFIG_GPT_PL011_SERIAL
#ifdef  CONFIG_GPT_PL011_SERIAL
#define CONFIG_BAUDRATE			115200
#define CONFIG_BOOTARGS	 		"console=pl01x_serial,115200"
#define CONFIG_GPT_PL01x_PORTS              {(void*)0xF0010000, (void *)0xF0011000, (void*)0xF0012000, (void *)0xF0013000}
#define CONFIG_CONS_INDEX               0
#define	CONFIG_APIU_PORT		0xF0010000
#define CONFIG_GPT_PL011_CLOCK_DIVIDER      2
#define CONFIG_GPT_PL011_CLOCK     		((CONFIG_EXT_MHZ * MHz) >> CONFIG_GPT_PL011_CLOCK_DIVIDER)
#endif

#define CONFIG_SYS_BAUDRATE_TABLE      { CONFIG_BAUDRATE }
#define CONFIG_DISPLAY_BOARDINFO_LATE

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SKIP_LOWLEVEL_INIT
/* #define CONFIG_SPL_SKIP_RELOCATE */


#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
/* #define CONFIG_SYS_HUSH_PARSER */

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE		256
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
/* max number of command args */
#define CONFIG_SYS_MAXARGS		16

#define CONFIG_SYS_GPT_TIMER_FREQ	((CONFIG_SYS_MHZ * 1000000)/8)

#define CONFIG_SYS_TEXT_BASE		0x8000 
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/* DDR addr */
#define CONFIG_SYS_DDR_SDRAM_START	0x200000000 
#define CONFIG_SYS_DDR_SDRAM_SIZE	(1*1024*1024)
#define CONFIG_SYS_SDRAM_BASE           CONFIG_SYS_DDR_SDRAM_START

/* default load address */
#define	CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 1024*1024) 

/*RAM is end of 0x80000=512k*/
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_TEXT_BASE + 0x70000)
#define CONFIG_STACKSIZE		(32 * 1024)

#define CONFIG_SYS_MALLOC_LEN		32*1024

#define CONFIG_ENV_ADDR		(CONFIG_SYS_TEXT_BASE + 0x68000)
/* Address and size of Primary Environment Sector */
#define CONFIG_ENV_SIZE		0x2000


/*#define CONFIG_SYS_INIT_SP_OFFSET	0x400 */

#define CONFIG_SYS_NO_FLASH

/* We don't have a flash,add below paramters to avoid compiling error */
#define CONFIG_SYS_FLASH_BASE		0x0
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_MAX_FLASH_SECT	128
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE

#define CONFIG_DELAY_ENVIRONMENT
/*#define CONFIG_ENV_IS_IN_FLASH */
/*#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_MONITOR_LEN) */
#define CONFIG_ENV_IS_IN_NVRAM

#define CONFIG_ENV_OVERWRITE	1

#endif /* __CONFIG_H */
