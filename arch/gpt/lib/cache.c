/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>

#ifdef CONFIG_SYS_CACHELINE_SIZE

static inline unsigned long icache_line_size(void)
{
	return CONFIG_SYS_CACHELINE_SIZE;
}

static inline unsigned long dcache_line_size(void)
{
	return CONFIG_SYS_CACHELINE_SIZE;
}

#else /* !CONFIG_SYS_CACHELINE_SIZE */

static inline unsigned long icache_line_size(void)
{
	return 0;
}

static inline unsigned long dcache_line_size(void)
{
	return 0;
}

#endif /* !CONFIG_SYS_CACHELINE_SIZE */

#ifdef CONFIG_NO_DCACHE
void flush_cache(ulong start_addr, ulong size)
{
}
void flush_dcache_range(ulong start_addr, ulong stop)
{
}
void invalidate_dcache_range(ulong start_addr, ulong stop)
{
}
#else  /* for !CONFIG_NO_DCACHE */
void flush_cache(ulong start_addr, ulong size)
{
#if 0
	unsigned long ilsize = icache_line_size();
	unsigned long dlsize = dcache_line_size();
	const void *addr, *aend;

	/* aend will be miscalculated when size is zero, so we return here */
	if (size == 0)
		return;

	addr = (const void *)(start_addr & ~(dlsize - 1));
	aend = (const void *)((start_addr + size - 1) & ~(dlsize - 1));

	if (ilsize == dlsize) {
		/* flush I-cache & D-cache simultaneously */
		while (1) {
			/* xx_cache */
			/* xx_cache */
			gpt_dcache(CACHE_WRITE_ALL, addr);
			gpt_icache(CACHE_WRITE_ALL, addr);
			if (addr == aend)
				break;
			addr += dlsize;
		}
		return;
	} else {

		/* flush D-cache */
		while (1) {
			gpt_dcache(CACHE_WRITE_ALL, addr);
			if (addr == aend)
				break;
			addr += dlsize;
		}

		/* flush I-cache */
		addr = (const void *)(start_addr & ~(ilsize - 1));
		aend = (const void *)((start_addr + size - 1) & ~(ilsize - 1));
		while (1) {
			gpt_icache(CACHE_WRITE_ALL, addr);
			if (addr == aend)
				break;
			addr += ilsize;
		}
	}
#endif
}

void flush_dcache_range(ulong start_addr, ulong stop)
{
#if 0
	unsigned long lsize = dcache_line_size();
	const void *addr = (const void *)(start_addr & ~(lsize - 1));
	const void *aend = (const void *)((stop - 1) & ~(lsize - 1));

	while (1) {
		gpt_dcache(CACHE_WRITE_ALL, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
#endif
}

void invalidate_dcache_range(ulong start_addr, ulong stop)
{
#if 0
	unsigned long lsize = dcache_line_size();
	const void *addr = (const void *)(start_addr & ~(lsize - 1));
	const void *aend = (const void *)((stop - 1) & ~(lsize - 1));

	while (1) {
		gpt_invalid_dcache(0, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
#endif
}
#endif  /* end of CONFIG_NO_DCACHE */
