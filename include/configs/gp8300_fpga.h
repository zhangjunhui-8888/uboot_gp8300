/*
 * This file contains the configuration parameters for gpt chip2 target.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_ARCH_GPT

/* #define CONFIG_IMAGE_FORMAT_LEGACY */

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_EARLY_INIT_R

/* Enable flat device tree support */
#define CONFIG_OF_LIBFDT	1
/* #define CONFIG_DISPLAY_BOARDINFO */
/* #define CONFIG_MISC_INIT_R */
/* #define CONFIG_REINIT_GLOBAL_DATA */

#define CONFIG_BOOTDELAY	4	/* autoboot after 4 seconds */

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */

/* DDR addr */
#define CONFIG_BOOTFILE  "uImage"
#define CONFIG_LOADADDR  0x200010000
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

/* SD Card */
/*#define CONFIG_SPL_MMC_SUPPORT*/
#define CONFIG_GPT_SDHCI_BASE 0xf0110000
#define CONFIG_GPT_SDHCI_MAX_FREQ (50*MHz)   /*SD3.0 max freq:208MHz*/

#define CONFIG_RAMDISK_ADDR 0xa0000000

/*
 * System clock configuration.
 */
#define MHz 			     	(1000 * 1000)
#define MB				(1024 * 1024)

#define CONFIG_OSC_HZ                   (12 * 1000 *1000)
#define CONFIG_CPLL_HZ                  (1596 * 1000 * 1000)

#define CONFIG_SYS_CLK			(50 * MHz)
#define CONFIG_BAUDRATE			115200
#define CONFIG_GPT_PL011_CLOCK_DIVIDER  2
#define CONFIG_GPT_PL011_CLOCK     	((CONFIG_SYS_CLK) >> CONFIG_GPT_PL011_CLOCK_DIVIDER)
#define CONFIG_BOOTARGS 		"console=pl01x_serial,115200"
#define CONFIG_SYS_GPT_TIMER_FREQ	(CONFIG_SYS_CLK / 8)

#define	CONFIG_GPT_PL011_SERIAL
#ifdef  CONFIG_GPT_PL011_SERIAL
#define CONFIG_GPT_PL01x_PORTS          {(void*)0xF0010000, (void *)0xF0011000, (void*)0xF0012000, (void *)0xF0013000}
#define CONFIG_CONS_INDEX               0
#define	CONFIG_APIU_PORT		0xF0010000
#endif /* CONFIG_GPT_PL011_SERIAL */

#define CONFIG_SYS_BAUDRATE_TABLE      { CONFIG_BAUDRATE }
#define CONFIG_DISPLAY_BOARDINFO_LATE

/*
 *bit0:  gsnn0_enable
 *bit1:  gsnn0_clk_dis
 *bit2:  gsnn1_enable
 *bit3:  gsnn1_clk_dis
 *bit4:  vcap0_enable
 *bit5:  vcap1_enable
 *bit6:  vcap2_enable
 *bit7:  vout_enable
 *bit8:  vidmem_enable
 *bit9:  spinor_enable
 *bit10: sd_enable
 *bit11: pcie_enable
 *bit12: ddr_enable
 *bit13: vid_enable
 */
#define GPT_EXTSCR_DEFAULT	0xffff29f0

#ifdef	CONFIG_SPL_BUILD
#define	CONFIG_SPL_SERIAL_SUPPORT
#define	CONFIG_SPL_FRAMEWORK
#define	CONFIG_SPL_YMODEM_SUPPORT
#define	CONFIG_SPL_LIBGENERIC_SUPPORT
#define	CONFIG_SPL_YMODEM_SUPPORT
#define	CONFIG_SPL_LIBCOMMON_SUPPORT
#define	CONFIG_SPL_MAX_SIZE		(64 * 1024)	/* max 64KB */
#define	CONFIG_BOARD_EARLY_INIT_F

#define	CONFIG_SPL_TCM_START_ADDR	0xc000000 /* start at 256M */
#define	CONFIG_SPL_TEXT_BASE		0x8000
#define	CONFIG_SPL_BSS_START_ADDR	(CONFIG_SPL_TCM_START_ADDR + (492*1024)) /* This should be tcm addr */
#define	CONFIG_SPL_BSS_MAX_SIZE		0x1000 		/* 4KB */
#define	CONFIG_SPL_STACK		(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
#define	CONFIG_SPL_STACKSIZE		(16 * 1024)

#define	CONFIG_SPL_NOR_SUPPORT
#define	CONFIG_SYS_UBOOT_BASE		0x28000   /*128k*/

#define CONFIG_SPL_SPI_NORFLASH_SUPPORT
#define CONFIG_SPL_SKIP_RELOCATE
#endif /* CONFIG_SPL_BUILD */

#define CONFIG_SYS_DEBUG_BASE           0x108000   /*1M*/

#define	CONFIG_SYS_MAX_NAND_DEVICE	1
#define	CONFIG_SYS_NAND_SELF_INIT
#define	CONFIG_NAND_GPT_SPI

#define	CONFIG_SYS_NAND_BASE	0xf0101000
/*#define CONFIG_CMD_RARP */

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

#ifdef	CONFIG_GPT_TCMBOOT
#define CONFIG_SYS_TEXT_BASE		0xc000000
#else
#define CONFIG_SYS_TEXT_BASE		0x8000
#endif /* CONFIG_GPT_TCMBOOT */

#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/* DDR addr */
#define CONFIG_SYS_DDR_SDRAM_START	0x200000000 
#define CONFIG_SYS_DDR_SDRAM_SIZE	0x40000000  /* 1GByte */
#define CONFIG_SYS_SDRAM_BASE           CONFIG_SYS_DDR_SDRAM_START

#define	CONFIG_BP_DATA_ADDR		CONFIG_SYS_DDR_SDRAM_START

#define CONFIG_BOOTM_LOW      		CONFIG_SYS_DDR_SDRAM_START
#define CONFIG_BOOTM_SIZE     		CONFIG_SYS_DDR_SDRAM_SIZE
#define	CONFIG_BOOTM_MAPSIZE  		CONFIG_BOOTM_SIZE 
/* default load address */
#define	CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE

/*RAM is end of 0x80000=512k*/
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_TEXT_BASE + 0x7c000)
#define CONFIG_STACKSIZE		(32 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(2048 * 1024)
/*#define CONFIG_SYS_INIT_SP_OFFSET	0x400 */
/* #define CONFIG_ENV_IS_IN_NVRAM */

#define CONFIG_ENV_OVERWRITE	1
/* Norflash */
#define GPT_SPI_NOR_APB_BASE          0xf0100000
#define GPT_SPI_NOR_AXI_BASE          0x8000

#define NORFLASH_MX25L3206E_4MB
/* #define NORFLASH_GD25Q18_16MB*/


/* Address and size of Primary Environment Sector */
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_MAX_FLASH_SECT	4096
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE
/* #define CONFIG_DELAY_ENVIRONMENT  */

#define CONFIG_ENV_IS_IN_FLASH 
#ifdef CONFIG_ENV_IS_IN_FLASH
#define CONFIG_SYS_FLASH_BASE		0x0
#define CONFIG_ENV_OFFSET  		0x300000    /*Reserve 512 kB for environment*/
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)  
#define CONFIG_ENV_SIZE			0x8000	/* 32kB */
#define CONFIG_FLASH_SECT_SIZE		0x1000	/* 4kB */
#endif /* CONFIG_ENV_IS_IN_FLASH */

/* I2S */
#define CONFIG_GPT_I2S_REFCLK	     12288000

/* Ethernet */
#define CONFIG_IP_DEFRAG
#define CONFIG_TFTP_BLOCKSIZE 1460
#define CONFIG_SYS_RX_ETH_BUFFER 8
/* Ethernet controller is Ethernet_MAC */

#define CONFIG_GPT_AXIEMAC
#if defined(CONFIG_GPT_AXIEMAC)
#define XILINX_AXIEMAC_BASEADDR       0x300000000
#define XILINX_AXIDMA_BASEADDR        0x300050000
#endif

#define CONFIG_PHY_ADDR               0x0

#define CONFIG_PHY_CONNECT_NO_RESET
#define CONFIG_PHY_NO_CONFIG

#undef CONFIG_SYS_ENET

#define CONFIG_RTL8169
#if defined(CONFIG_RTL8169)
# define CONFIG_MII		1
/* # define CONFIG_CMD_MII		1 */
# define CONFIG_SYS_FAULT_ECHO_LINK_DOWN	1
# define CONFIG_PHYLIB		1
# define CONFIG_PHY_MARVELL	1
#endif

#undef CONFIG_SYS_ENET
#if defined(CONFIG_GPT_AXIEMAC)
# define CONFIG_SYS_ENET
# define CONFIG_MII		1
/* # define CONFIG_CMD_MII		1 */
# define CONFIG_SYS_FAULT_ECHO_LINK_DOWN	1
# define CONFIG_PHYLIB		1
# define CONFIG_PHY_MARVELL	1
#endif

#if 0 
#define CONFIG_PCI
#define CONFIG_PCI_PNP
#define CONFIG_CMD_PCI
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_SYS_PCI_64BIT
#endif

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
    "nfsargs=run set_nfs; run setbaseargs; setenv bootargs00 ${baseargs} ${arg_nfs}\0"   \
    "tftpbootm=run nfsargs; tftp && bootm\0" \
    "tftpboot=run nfsargs; tftp vmlinux.bin && go ${loadaddr}\0" \
    "tftp_sd=run nfsargs; tftp vmlinux.bin && ext4write mmc 0:1 ${loadaddr} /vmlinux.bin $filesize\0" \
    "tftp_sd_fat=run nfsargs; tftp vmlinux.bin && fatwrite mmc 0:1 ${loadaddr} /vmlinux.bin $filesize\0" \
    "tftp_sd2=run nfsargs; tftp vmlinux.bin && ext4write mmc 0:2 ${loadaddr} /boot/vmlinux.bin $filesize\0" \
    "boot_sd=mmc rescan; ext4load mmc 0:1 ${loadaddr} /vmlinux.bin && go ${loadaddr}\0" \
    "boot_sd_fat=mmc rescan; fatload mmc 0:1 ${loadaddr} /vmlinux.bin && go ${loadaddr}\0" \
    "boot_sd2=mmc rescan; ext4load mmc 0:2 ${loadaddr} /boot/vmlinux.bin && go ${loadaddr}\0" \
    "ramdiskboot=run nfsargs; tftp vmlinux.bin && tftp " __stringify(CONFIG_RAMDISK_ADDR) " ramdisk.img && go ${loadaddr}\0" \
    ""

/* Filesystem */
#define CONFIG_DOS_PARTITION
#define HAVE_BLOCK_DEVICE

#endif /* __CONFIG_H */
