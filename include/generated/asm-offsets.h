#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__
/*
 * DO NOT MODIFY.
 *
 * This file was generated by Kbuild
 */

#define GENERATED_GBL_DATA_SIZE 288 /* (sizeof(struct global_data) + 15) & ~15	# */
#define GENERATED_BD_INFO_SIZE 112 /* (sizeof(struct bd_info) + 15) & ~15	# */
#define GD_SIZE 288 /* sizeof(struct global_data)	# */
#define GD_BD 0 /* offsetof(struct global_data, bd)	# */
#define GD_RELOCADDR 88 /* offsetof(struct global_data, relocaddr)	# */
#define GD_RELOC_OFF 128 /* offsetof(struct global_data, reloc_off)	# */
#define GD_START_ADDR_SP 120 /* offsetof(struct global_data, start_addr_sp)	# */
#define GD_FLAGS 8 /* offsetof(struct global_data, flags)	# */
#define SMP_JUMP_ADDR 0 /* offsetof(struct cpulaunch, jump_addr)	# */
#define SMP_CPU_ID 8 /* offsetof(struct cpulaunch, cpu_id)	# */
#define SMP_ERR_FLAG 16 /* offsetof(struct cpulaunch, err_flag)	# */
#define SMP_CPULAUNCH_SIZE 24 /* sizeof(struct cpulaunch)	# */

#endif
