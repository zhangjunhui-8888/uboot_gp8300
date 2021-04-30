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

#define CONFIG_BOARD_EARLY_INIT_F

/* Enable flat device tree support */
#define CONFIG_OF_LIBFDT	0 
/* #define CONFIG_DISPLAY_BOARDINFO */
/* #define CONFIG_MISC_INIT_R */
/* #define CONFIG_REINIT_GLOBAL_DATA */

#define CONFIG_BOOTDELAY	2	/* autoboot after 4 seconds */

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */

/* DDR addr */
#define CONFIG_BOOTFILE  "vmlinux.bin"
#define CONFIG_LOADADDR  0x80010000
#define CONFIG_GATEWAYIP 192.168.1.4
#define CONFIG_NETMASK   255.255.255.0

/* #define CONFIG_BOARD_178 */
/* #define CONFIG_BOARD_179 */
/* #define CONFIG_BOARD_180 */
#if defined(CONFIG_BOARD_178)
#define CONFIG_SERVERIP  192.168.1.59
#define CONFIG_IPADDR    192.168.1.178
#define CONFIG_ETHADDR   20:00:00:F0:F1:F1
#elif defined(CONFIG_BOARD_179)
#define CONFIG_SERVERIP  192.168.1.164
#define CONFIG_IPADDR    192.168.1.179
#define CONFIG_ETHADDR   20:00:00:F0:F1:F2
#else  /* for Haps BOARD */
#define CONFIG_SERVERIP  172.16.100.10
#define CONFIG_IPADDR    172.16.100.20
#define CONFIG_ETHADDR   20:00:00:F1:F1:F1
#endif

/* #define ETHERNET_DIRECT_TEST */
#ifdef ETHERNET_DIRECT_TEST
#undef CONFIG_IPADDR
#undef CONFIG_SERVERIP
#undef CONFIG_GATEWAYIP
#undef CONFIG_BOOTFILE
#define CONFIG_GATEWAYIP 192.168.2.10
#define CONFIG_SERVERIP  192.168.2.10
#define CONFIG_IPADDR    192.168.2.11
#define CONFIG_BOOTFILE  "vmlinux.bin"
#endif

/* DDR addr */
#define CONFIG_SYS_DDR_SDRAM_START	0x80000000
#define CONFIG_SYS_DDR_SDRAM_SIZE	(1024*1024*1024)
#define CONFIG_SYS_SDRAM_BASE           CONFIG_SYS_DDR_SDRAM_START
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_DDR_SDRAM_START 
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_DDR_SDRAM_START + CONFIG_SYS_DDR_SDRAM_SIZE 

#define CONFIG_IFNAME         eth0
#define CONFIG_ROOTPATH       "/nfsroot/"
#define CONFIG_NFSBOOTCOMMAND "nfs && bootm"
/* #define CONFIG_BOOTCOMMAND    "run tftpbootm" */
#define CONFIG_BOOTCOMMAND    "tftpboot"
/*#define CONFIG_BOOTCOMMAND    "tftp 80000000 mini_bench.bin; go 80000000" */
#define CONFIG_BOOTM_LOW      CONFIG_SYS_DDR_SDRAM_START
#define CONFIG_BOOTM_SIZE     CONFIG_SYS_DDR_SDRAM_SIZE

#define CONFIG_RAMDISK_ADDR 0xa0000000

#define CONFIG_ENV_VARS_UBOOT_CONFIG

/*
 * System serial configuration.
 */
#define	CONFIG_GPT_PL011_SERIAL
#ifdef  CONFIG_GPT_PL011_SERIAL
#define CONFIG_BAUDRATE			115200
#define CONFIG_GPT_PL01x_PORTS          {(void*)0xF0010000, (void *)0xF0011000, (void*)0xF0012000, (void *)0xF0013000}
#endif /* CONFIG_GPT_PL011_SERIAL */

#define CONFIG_SYS_BAUDRATE_TABLE       {9600, 19200, 38400, 57600, 115200, 230400}

#define CONFIG_EXTRA_ENV_SETTINGS \
    "memtest_start=" __stringify(CONFIG_SYS_MEMTEST_START) "\0" \
    "memtest_end=" __stringify(CONFIG_SYS_MEMTEST_END) "\0" \
    "bootm_low=" __stringify(CONFIG_BOOTM_LOW) "\0" \
    "bootm_size=" __stringify(CONFIG_BOOTM_SIZE) "\0" \
    "ifname=" __stringify(CONFIG_IFNAME) "\0" \
    "ethaddr=" __stringify(CONFIG_ETHADDR) "\0" \
    "console=ttyUL0," __stringify(CONFIG_BAUDRATE) "n8\0" \
    "set_ip=setenv arg_ip ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${board}:${ifname}:off\0" \
    "set_nfsroot=setenv arg_nfsroot nfsroot=${serverip}:${rootpath},nolock\0" \
    "set_nfs=run set_ip; run set_nfsroot; setenv arg_nfs root=/dev/nfs rw ${arg_nfsroot} ${arg_ip}\0" \
    "set_arg_extra=set arg_extra rootwait hlt\0" \
    "setbaseargs=run set_arg_extra; setenv baseargs console=${console} ${arg_extra}\0" \
    "nfsargs=run set_nfs; run setbaseargs; setenv bootargs ${baseargs} ${arg_nfs}\0"   \
    "tftpbootm=run nfsargs; tftp && bootm\0" \
    "tftpboot=tftp vmlinux.bin && go ${loadaddr}\0" \
    "ramdiskboot=run nfsargs; tftp vmlinux.bin && tftp " __stringify(CONFIG_RAMDISK_ADDR) " ramdisk.img && go ${loadaddr}\0" \
    ""

/*
 * BOOTP options
 */
/*#define CONFIG_BOOTP_BOOTFILESIZE */
/*#define CONFIG_BOOTP_BOOTPATH */
/*#define CONFIG_BOOTP_GATEWAY */
/*#define CONFIG_BOOTP_HOSTNAME */

/*
 * Command line configuration.
 */
/*#define CONFIG_CMD_FAT */
/*#define CONFIG_CMD_EXT2 */
/*#define CONFIG_CMD_DHCP */

/*#define CONFIG_XILINX_GPIO */
/*#define CONFIG_SYS_GPIO_0_ADDR		0x40000000 */

#define CONFIG_SYS_MHZ			10
#define CONFIG_EXT_MHZ			10
#define MHz 			     	(1000 * 1000)

#define	CONFIG_BP_DATA_ADDR		0x80000000
/*#define CONFIG_CMD_MEMTEST*/

/* #define CONFIG_XILINX_UARTLITE */
#ifdef CONFIG_XILINX_UARTLITE
#define CONFIG_BAUDRATE                 115200
#define CONFIG_BOOTARGS                 "console=ttyUL0,115200"
#define XILINX_UARTLITE_BASEADDR        0x40600000
#define CONFIG_SERIAL_BASE     		XILINX_UARTLITE_BASEADDR
#else
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
#endif

#define CONFIG_DISPLAY_BOARDINFO_LATE
/*#define CONFIG_CMD_LOADB*/
/*
 * Miscellaneous configurable options
 */
#define CONFIG_SKIP_LOWLEVEL_INIT

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
#define CONFIG_SYS_DDR_SDRAM_START	0x80000000
#define CONFIG_SYS_DDR_SDRAM_SIZE	(1024*1024*1024)
#define CONFIG_SYS_SDRAM_BASE           CONFIG_SYS_DDR_SDRAM_START

/* default load address */
#define	CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE

/*RAM is end of 0x40000=256k*/
#define CONFIG_SYS_INIT_SP_ADDR		(224 * 1024)
#define CONFIG_STACKSIZE		(32 * 1024)

#define CONFIG_SYS_MALLOC_LEN		16*1024

#define CONFIG_ENV_ADDR		(CONFIG_SYS_TEXT_BASE + 0x38000)
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

/* I2S */
#define CONFIG_GPT_I2S_REFCLK	     12288000

/* Ethernet */
#define CONFIG_IP_DEFRAG
#define CONFIG_TFTP_BLOCKSIZE 1460
#define CONFIG_SYS_RX_ETH_BUFFER 8
/* Ethernet controller is Ethernet_MAC */
#define XILINX_AXIEMAC_BASEADDR       0x40C00000
#define XILINX_AXIDMA_BASEADDR        0x41E00000

#define CONFIG_PHY_ADDR               0x0

#define CONFIG_PHY_CONNECT_NO_RESET
#define CONFIG_PHY_NO_CONFIG

#undef CONFIG_SYS_ENET
#if defined(XILINX_AXIEMAC_BASEADDR)
# define CONFIG_GPT_AXIEMAC	1
# define CONFIG_SYS_ENET
#endif

#if defined(CONFIG_GPT_AXIEMAC)
# define CONFIG_MII		1
# define CONFIG_CMD_MII		1 
# define CONFIG_SYS_FAULT_ECHO_LINK_DOWN	1
# define CONFIG_PHYLIB		1
# define CONFIG_PHY_MARVELL	1
#else
# undef CONFIG_MII
# undef CONFIG_CMD_MII
# undef CONFIG_PHYLIB
#endif

#endif /* __CONFIG_H */
