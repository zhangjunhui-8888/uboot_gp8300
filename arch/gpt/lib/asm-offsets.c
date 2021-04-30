/*
 *
 * This program is used to generate definitions needed by
 * assembly language modules.
 *
 * We use the technique used in the OSF Mach kernel code:
 * generate asm statements containing #defines,
 * compile this file to assembler, and then extract the
 * #defines from the assembly-language output.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>

#include <linux/kbuild.h>
#include <asm/processor.h>

int main(void)
{
	/* Round up to make sure size gives nice stack alignment */
	DEFINE(GENERATED_GBL_DATA_SIZE,
		(sizeof(struct global_data) + 15) & ~15);

	DEFINE(GENERATED_BD_INFO_SIZE,
		(sizeof(struct bd_info) + 15) & ~15);

	DEFINE(GD_SIZE, sizeof(struct global_data));

	DEFINE(GD_BD, offsetof(struct global_data, bd));
#ifdef CONFIG_SYS_MALLOC_F_LEN
	DEFINE(GD_MALLOC_BASE, offsetof(struct global_data, malloc_base));
#endif

	DEFINE(GD_RELOCADDR, offsetof(struct global_data, relocaddr));

	DEFINE(GD_RELOC_OFF, offsetof(struct global_data, reloc_off));

	DEFINE(GD_START_ADDR_SP, offsetof(struct global_data, start_addr_sp));

	DEFINE(GD_FLAGS, offsetof(struct global_data, flags));

	DEFINE(SMP_JUMP_ADDR, offsetof(struct cpulaunch, jump_addr));

	DEFINE(SMP_CPU_ID, offsetof(struct cpulaunch, cpu_id));

	DEFINE(SMP_ERR_FLAG, offsetof(struct cpulaunch, err_flag));

	DEFINE(SMP_CPULAUNCH_SIZE, sizeof(struct cpulaunch));

	return 0;
}
