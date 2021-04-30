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

#define CONFIG_CHIP1_FPGA

#define CONFIG_BOARD_LATE_INIT

#ifdef CONFIG_GPT_LITTLE_ENDIAN
#define CONFIG_SYS_LITTLE_ENDIAN
#else
#define CONFIG_SYS_BIG_ENDIAN
#endif

/* Enable flat device tree support */
#define CONFIG_OF_LIBFDT	1
/* #define CONFIG_DISPLAY_BOARDINFO */
/* #define CONFIG_MISC_INIT_R */
#define CONFIG_REINIT_GLOBAL_DATA

#define CONFIG_BOOTDELAY	4	/* autoboot after 4 seconds */

#define CONFIG_DISPLAY_BOARDINFO_LATE

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */

/* DDR addr */
#define CONFIG_SYS_DDR_SDRAM_START	0x80000000
#define CONFIG_SYS_DDR_SDRAM_SIZE	0x40000000
#define CONFIG_BOOTFILE  "uImage"
#define CONFIG_LOADADDR  0x80000000
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
#else  /* for BOARD 180 */
#define CONFIG_SERVERIP  192.168.1.173
#define CONFIG_IPADDR    192.168.1.180
#define CONFIG_ETHADDR   20:00:00:FF:FF:FF
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
#define CONFIG_BOOTFILE  "uImage"
#endif

#define CONFIG_IFNAME         eth0
#define CONFIG_ROOTPATH       "/nfsroot/"
#define CONFIG_NFSBOOTCOMMAND "run nfsargs; nfs && bootm"
/* #define CONFIG_BOOTCOMMAND    "run tftpbootm" */
#define CONFIG_BOOTCOMMAND    "run tftpboot"
#define CONFIG_BOOTM_LOW      CONFIG_SYS_DDR_SDRAM_START
#define CONFIG_BOOTM_SIZE     CONFIG_SYS_DDR_SDRAM_SIZE

#define CONFIG_RAMDISK_ADDR 0xa0000000

#define CONFIG_ENV_VARS_UBOOT_CONFIG

#define CONFIG_EXTRA_ENV_SETTINGS \
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
    "tftpboot=run nfsargs; tftp vmlinux.bin && go ${loadaddr}\0" \
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

#define CONFIG_SYS_MHZ			100
#define MHz 			     	(1000 * 1000)

#define CONFIG_XILINX_UARTLITE

#ifdef CONFIG_XILINX_UARTLITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_BOOTARGS	  		"console=ttyUL0,115200"
#define XILINX_UARTLITE_BASEADDR	0x40600000
#define CONFIG_SERIAL_BASE     XILINX_UARTLITE_BASEADDR
#else
#define CONFIG_PL011_SERIAL
#ifdef  CONFIG_PL011_SERIAL
#define CONFIG_BAUDRATE			115200
#define CONFIG_BOOTARGS	 		"console=pl01x_serial,115200"
#define CONFIG_PL01x_PORTS              {(void*)0xf0008000, (void *)0xf0009000}
#define CONFIG_CONS_INDEX               0
#define	CONFIG_APIU_PORT		0xf0008000
#define CONFIG_PL011_CLOCK_DIVIDER      2
#define CONFIG_PL011_CLOCK     		((CONFIG_SYS_MHZ * MHz) >> CONFIG_PL011_CLOCK_DIVIDER)
#endif
#endif

#define CONFIG_SYS_BAUDRATE_TABLE      { CONFIG_BAUDRATE }
#define CONFIG_DISPLAY_BOARDINFO_LATE

/*#define CONFIG_CMD_IDE */
/*#define CONFIG_DOS_PARTITION */

/*#define CONFIG_SYS_IDE_MAXBUS		2 */
/*#define CONFIG_SYS_ATA_IDE0_OFFSET	0x1f0 */
/*#define CONFIG_SYS_ATA_IDE1_OFFSET	0x170 */
/*#define CONFIG_SYS_ATA_DATA_OFFSET	0 */
/*#define CONFIG_SYS_ATA_REG_OFFSET	0 */
/*#define CONFIG_SYS_ATA_BASE_ADDR	0xffffffffb4000000 */

/*#define CONFIG_SYS_IDE_MAXDEVICE	4 */

/*#define CONFIG_CMD_RARP */

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */

#define CONFIG_SKIP_LOWLEVEL_INIT

/* Monitor Command Prompt */
#undef CONFIG_SYS_PROMPT
#if defined(CONFIG_SYS_LITTLE_ENDIAN)
#define CONFIG_SYS_PROMPT		CONFIG_SYS_BOARD "le # "
#else
#define CONFIG_SYS_PROMPT		CONFIG_SYS_BOARD "be # "
#endif

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
/* #define CONFIG_SYS_HUSH_PARSER */

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE		256
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
/* max number of command args */
#define CONFIG_SYS_MAXARGS		16

#define CONFIG_SYS_MALLOC_LEN		32*1024

/* #define CONFIG_SYS_BOOTPARAMS_LEN	4*1024 */



#define CONFIG_SYS_GPT_TIMER_FREQ	((CONFIG_SYS_MHZ * 1000000)/8)

/*#define CONFIG_SYS_SDRAM_BASE           0x80000 */    /* CONFIG_SYS_DDR_SDRAM_BASE */

/* default load address */
#define CONFIG_SYS_LOAD_ADDR		0x8000

#define CONFIG_SYS_MEMTEST_START	0x70000
#define CONFIG_SYS_MEMTEST_END		0x80000

/*RAM is end of 0x80000=512k*/
#define CONFIG_SYS_INIT_SP_ADDR     0x68000
#define CONFIG_STACKSIZE		(32 * 1024)

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
/* The following #defines are needed to get flash environment right */
#define CONFIG_RAMBOOT
#ifdef CONFIG_RAMBOOT
#define CONFIG_SYS_TEXT_BASE		0x8000 /* RAM version */
#else
#define CONFIG_SYS_TEXT_BASE		0x0 /* Flash version */
#endif
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE
/*#define CONFIG_SYS_MONITOR_LEN		(16 << 10) */

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
#define CONFIG_ENV_ADDR		0x64000
/* Address and size of Primary Environment Sector */
#define CONFIG_ENV_SIZE		0x4000

#define CONFIG_ENV_OVERWRITE	1

#define MEM_SIZE		512

/*#define CONFIG_LZM */
#define CONFIG_SYS_OUT_OF_ORDER_OFF
/*-----------------------------------------------------------------------
 * Cache Configuration
 */
/*#define CONFIG_NO_DCACHE */
#define CONFIG_SYS_DCACHE_SIZE		32768
#define CONFIG_SYS_ICACHE_SIZE		32768
#define CONFIG_SYS_CACHELINE_SIZE	32

/*#define CONFIG_SYS_ICACHE_OFF */
#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_SYS_L2CACHE_OFF

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
# define CONFIG_XILINX_AXIEMAC	1
# define CONFIG_SYS_ENET
#endif

#if defined(CONFIG_XILINX_AXIEMAC)
# define CONFIG_MII		1
/* # define CONFIG_CMD_MII		1 */
# define CONFIG_SYS_FAULT_ECHO_LINK_DOWN	1
# define CONFIG_PHYLIB		1
# define CONFIG_PHY_MARVELL	1
#else
# undef CONFIG_MII
# undef CONFIG_CMD_MII
# undef CONFIG_PHYLIB
#endif

#endif /* __CONFIG_H */
