/*
 *
 ********************************************************************
 * NOTE: This header file defines an interface to U-Boot. Including
 * this (unmodified) header file in another file is considered normal
 * use of U-Boot, and does *not* fall under the heading of "derived
 * work".
 ********************************************************************
 */

#ifndef _U_BOOT_H_
#define _U_BOOT_H_	1

#ifdef CONFIG_SYS_GENERIC_BOARD

/* Use the generic board which requires a unified bd_info */
#include <asm-generic/u-boot.h>
#else /* !CONFIG_SYS_GENERIC_BOARD */

typedef struct bd_info {
	unsigned long	bi_arch_number;	/* unique id for this board */
	unsigned long	bi_boot_params;	/* where this board expects params */
	unsigned long	bi_memstart;	/* start of DRAM memory */
	phys_size_t	bi_memsize;	/* size	 of DRAM memory in bytes */
	unsigned long	bi_flashstart;	/* start of FLASH memory */
	unsigned long	bi_flashsize;	/* size  of FLASH memory */
	unsigned long	bi_flashoffset;	/* reserved area for startup monitor */
} bd_t;

#endif /* !CONFIG_SYS_GENERIC_BOARD */

#ifdef CONFIG_CPU_GPT64
#define IH_ARCH_DEFAULT IH_ARCH_GPT64
#else
#define IH_ARCH_DEFAULT IH_ARCH_GPT
#endif

#endif	/* _U_BOOT_H_ */
