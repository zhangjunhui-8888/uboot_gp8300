/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <image.h>
#include <fdt_support.h>
#include <asm/addrspace.h>
#include <mapmem.h>

DECLARE_GLOBAL_DATA_PTR;

#define	LINUX_MAX_ENVS		256
#define	LINUX_MAX_ARGS		256

static int linux_argc;
static char **linux_argv;
static char *linux_argp;

static char **linux_env;
static char *linux_env_p;
static int linux_env_idx;

static ulong arch_get_sp(void)
{
	ulong ret;

	__asm__ __volatile__("rseta %0, $asp" : "=r"(ret) : );

	return ret;
}

void arch_lmb_reserve(struct lmb *lmb)
{
	ulong sp;

	sp = arch_get_sp();
	debug("## Current stack ends at 0x%08lx\n", sp);

	/* adjust sp by 4K to be safe */
	sp += 4096;
	lmb_reserve(lmb, sp, CONFIG_SYS_SDRAM_BASE + gd->ram_size - sp);
}

static void linux_cmdline_init(void)
{
	linux_argc = 1;
//	linux_argv = (char **)UNCACHED_SDRAM(gd->bd->bi_boot_params);
	linux_argv = (char **)(gd->bd->bi_boot_params);
	linux_argv[0] = 0;
	linux_argp = (char *)(linux_argv + LINUX_MAX_ARGS);
}

static void linux_cmdline_set(const char *value, size_t len)
{
	linux_argv[linux_argc] = linux_argp;
	memcpy(linux_argp, value, len);
	linux_argp[len] = 0;

	linux_argp += len + 1;
	linux_argc++;
}

static void linux_cmdline_dump(void)
{
	int i;

	debug("## cmdline argv at 0x%p, argp at 0x%p\n",
	      linux_argv, linux_argp);

	for (i = 1; i < linux_argc; i++)
		debug("   arg %03d: %s\n", i, linux_argv[i]);
}

static void linux_cmdline_legacy(bootm_headers_t *images)
{
	const char *bootargs, *next, *quote;

	linux_cmdline_init();

	bootargs = getenv("bootargs");
	if (!bootargs)
		return;

	next = bootargs;

	while (bootargs && *bootargs && linux_argc < LINUX_MAX_ARGS) {
		quote = strchr(bootargs, '"');
		next = strchr(bootargs, ' ');

		while (next && quote && quote < next) {
			/*
			 * we found a left quote before the next blank
			 * now we have to find the matching right quote
			 */
			next = strchr(quote + 1, '"');
			if (next) {
				quote = strchr(next + 1, '"');
				next = strchr(next + 1, ' ');
			}
		}

		if (!next)
			next = bootargs + strlen(bootargs);

		linux_cmdline_set(bootargs, next - bootargs);

		if (*next)
			next++;

		bootargs = next;
	}
}

static void linux_cmdline_append(bootm_headers_t *images)
{
	char buf[24];
	ulong mem, rd_start, rd_size;

	/* append mem */
	mem = gd->ram_size >> 20;
	sprintf(buf, "mem=%luM", mem);
	linux_cmdline_set(buf, strlen(buf));

	/* append rd_start and rd_size */
	rd_start = images->initrd_start;
	rd_size = images->initrd_end - images->initrd_start;

	if (rd_size) {
		sprintf(buf, "rd_start=0x%08lX", rd_start);
		linux_cmdline_set(buf, strlen(buf));
		sprintf(buf, "rd_size=0x%lX", rd_size);
		linux_cmdline_set(buf, strlen(buf));
	}
}

static void linux_env_init(void)
{
	linux_env = (char **)(((ulong) linux_argp + 15) & ~15);
	linux_env[0] = 0;
	linux_env_p = (char *)(linux_env + LINUX_MAX_ENVS);
	linux_env_idx = 0;
}

static void linux_env_set(const char *env_name, const char *env_val)
{
	if (linux_env_idx < LINUX_MAX_ENVS - 1) {
		linux_env[linux_env_idx] = linux_env_p;

		strcpy(linux_env_p, env_name);
		linux_env_p += strlen(env_name);

		if (CONFIG_IS_ENABLED(MALTA)) {
			linux_env_p++;
			linux_env[++linux_env_idx] = linux_env_p;
		} else {
			*linux_env_p++ = '=';
		}

		strcpy(linux_env_p, env_val);
		linux_env_p += strlen(env_val);

		linux_env_p++;
		linux_env[++linux_env_idx] = 0;
	}
}

static void linux_env_legacy(bootm_headers_t *images)
{
	char env_buf[12];
	const char *cp;
	ulong rd_start, rd_size;

	if (CONFIG_IS_ENABLED(MEMSIZE_IN_BYTES)) {
		sprintf(env_buf, "%lu", (ulong)gd->ram_size);
		debug("## Giving linux memsize in bytes, %lu\n",
		      (ulong)gd->ram_size);
	} else {
		sprintf(env_buf, "%lu", (ulong)(gd->ram_size >> 20));
		debug("## Giving linux memsize in MB, %lu\n",
		      (ulong)(gd->ram_size >> 20));
	}

	/* maybe NEED MODIFY */
	rd_start = images->initrd_start;
	rd_size = images->initrd_end - images->initrd_start;

	linux_env_init();

	linux_env_set("memsize", env_buf);

	sprintf(env_buf, "0x%08lX", rd_start);
	linux_env_set("initrd_start", env_buf);

	sprintf(env_buf, "0x%lX", rd_size);
	linux_env_set("initrd_size", env_buf);

	sprintf(env_buf, "0x%08X", (uint) (gd->bd->bi_flashstart));
	linux_env_set("flash_start", env_buf);

	sprintf(env_buf, "0x%X", (uint) (gd->bd->bi_flashsize));
	linux_env_set("flash_size", env_buf);

	cp = getenv("ethaddr");
	if (cp)
		linux_env_set("ethaddr", cp);

	cp = getenv("eth1addr");
	if (cp)
		linux_env_set("eth1addr", cp);

	if (CONFIG_IS_ENABLED(MALTA)) {
		sprintf(env_buf, "%un8r", gd->baudrate);
		linux_env_set("modetty0", env_buf);
	}
}

static int boot_reloc_ramdisk(bootm_headers_t *images)
{
	ulong rd_len = images->rd_end - images->rd_start;

	/*
	 * In case of legacy uImage's, relocation of ramdisk is already done
	 * by do_bootm_states() and should not repeated in 'bootm prep'.
	 */
	if (images->state & BOOTM_STATE_RAMDISK) {
		debug("## Ramdisk already relocated\n");
		return 0;
	}

	return boot_ramdisk_high(&images->lmb, images->rd_start,
		rd_len, &images->initrd_start, &images->initrd_end);
}

static int boot_reloc_fdt(bootm_headers_t *images)
{
	/*
	 * In case of legacy uImage's, relocation of FDT is already done
	 * by do_bootm_states() and should not repeated in 'bootm prep'.
	 */
	if (images->state & BOOTM_STATE_FDT) {
		debug("## FDT already relocated\n");
		return 0;
	}

#if CONFIG_IS_ENABLED(GPT_BOOT_FDT) && CONFIG_IS_ENABLED(OF_LIBFDT)
	boot_fdt_add_mem_rsv_regions(&images->lmb, images->ft_addr);
	return boot_relocate_fdt(&images->lmb, &images->ft_addr,
		&images->ft_len);
#else
	return 0;
#endif
}

int arch_fixup_memory_node(void *blob)
{
#if CONFIG_IS_ENABLED(GPT_BOOT_FDT) && CONFIG_IS_ENABLED(OF_LIBFDT)
	u64 mem_start = 0;
	u64 mem_size = gd->ram_size;

	return fdt_fixup_memory_banks(blob, &mem_start, &mem_size, 1);
#else
	return 0;
#endif
}

static int boot_setup_fdt(bootm_headers_t *images)
{
	return image_setup_libfdt(images, images->ft_addr, images->ft_len, &images->lmb);
}

static void boot_prep_linux(bootm_headers_t *images)
{
	boot_reloc_ramdisk(images);

	if (CONFIG_IS_ENABLED(GPT_BOOT_FDT) && images->ft_len) {
		boot_reloc_fdt(images);
		boot_setup_fdt(images);
	} else {
		if (CONFIG_IS_ENABLED(CONFIG_GPT_BOOT_ENV_LEGACY))
			linux_env_legacy(images);

		if (CONFIG_IS_ENABLED(GPT_BOOT_CMDLINE_LEGACY)) {
			linux_cmdline_legacy(images);

			if (!CONFIG_IS_ENABLED(CONFIG_GPT_BOOT_ENV_LEGACY))
				linux_cmdline_append(images);

			linux_cmdline_dump();
		}
	}
}

static void boot_jump_linux(bootm_headers_t *images)
{
	typedef void __noreturn (*kernel_entry_t)(int, ulong, ulong, ulong);
	kernel_entry_t kernel = (kernel_entry_t) images->ep;
	ulong linux_extra = 0;

	debug("## Transferring control to Linux (at address %p) ...\n", kernel);

	bootstage_mark(BOOTSTAGE_ID_RUN_OS);

	if (CONFIG_IS_ENABLED(MALTA))
		linux_extra = gd->ram_size;

#if CONFIG_IS_ENABLED(BOOTSTAGE_FDT)
	bootstage_fdt_add_report();
#endif
#if CONFIG_IS_ENABLED(BOOTSTAGE_REPORT)
	bootstage_report();
#endif

	if (images->ft_len)
		kernel(-2, (ulong)images->ft_addr, 0, 0);
	else
		kernel(linux_argc, (ulong)linux_argv, (ulong)linux_env,
			linux_extra);
}

int do_bootm_linux(int flag, int argc, char * const argv[],
			bootm_headers_t *images)
{
	/* No need for those on GPT */
	if (flag & BOOTM_STATE_OS_BD_T)
		return -1;

	/*
	 * Cmdline init has been moved to 'bootm prep' because it has to be
	 * done after relocation of ramdisk to always pass correct values
	 * for rd_start and rd_size to Linux kernel.
	 */
	if (flag & BOOTM_STATE_OS_CMDLINE)
		return 0;

	if (flag & BOOTM_STATE_OS_PREP) {
		boot_prep_linux(images);
		return 0;
	}

	if (flag & (BOOTM_STATE_OS_GO | BOOTM_STATE_OS_FAKE_GO)) {
		boot_jump_linux(images);
		return 0;
	}

	/* does not return */
	return 1;
}

#ifdef CONFIG_CMD_BOOTZ

struct zimage_header {
	uint32_t	code[9];
	uint32_t	zi_magic;
	uint32_t	zi_start;
	uint32_t	zi_end;
};

#define	LINUX_GPT_ZIMAGE_MAGIC	0x28f0f01c

int bootz_setup(ulong image, ulong *start, ulong *end)
{
	struct zimage_header *zi;

	zi = (struct zimage_header *)map_sysmem(image, 0);
	if (zi->zi_magic != LINUX_GPT_ZIMAGE_MAGIC) {
		puts("Bad Linux GPT zImage magic!\n");
		return 1;
	}

	*start = zi->zi_start;
	*end = zi->zi_end;

	printf("Kernel image @ %#08lx [ %#08lx - %#08lx ]\n", image, *start,
	      *end);

	return 0;
}

#endif	/* CONFIG_CMD_BOOTZ */
