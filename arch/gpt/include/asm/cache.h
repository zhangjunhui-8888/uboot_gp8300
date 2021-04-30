/*
 * Copyright (c) 2011 The Chromium OS Authors.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __GPT_CACHE_H__
#define __GPT_CACHE_H__

#define CONFIG_SYS_CACHELINE_SIZE	(2<<CONFIG_GPT_CACHE_LINE_SHIFT)
#define ARCH_DMA_MINALIGN		CONFIG_SYS_CACHELINE_SIZE

/* flush only l1 cache */
#define		CACHE_WRITE_L1		0x00
/* flush all cache */
#define		CACHE_WRITE_ALL		0x11

static inline void gpt_dcache(int op, const volatile void *addr)
{
	return;
}

static inline void gpt_icache(int op, const volatile void *addr)
{
	return;
}

static inline void gpt_invalid_dcache(int op, const volatile void *addr)
{
	__asm__ __volatile__ (
	"aaddri		 $a7, $r9, 0\n\t"
	"dctl_invalid_l1 $a7"
	);
}

#endif /* __GPT_CACHE_H__ */
