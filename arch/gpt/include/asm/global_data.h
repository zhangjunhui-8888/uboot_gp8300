/*
 * (C) Copyright 2002-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef	__ASM_GBL_DATA_H
#define __ASM_GBL_DATA_H

/* Architecture-specific global data */
struct arch_global_data {
};

#include <asm-generic/global_data.h>
#define DECLARE_GLOBAL_DATA_PTR     register volatile gd_t *gd asm ("$a2")

#endif /* __ASM_GBL_DATA_H */