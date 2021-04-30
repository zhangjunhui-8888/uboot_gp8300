/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */
#ifndef _ASM_PROCESSOR_H
#define _ASM_PROCESSOR_H

#include <asm/spr.h>

/* SMP boot structure */
struct cpulaunch {
	unsigned long       jump_addr;
	unsigned long       cpu_id;
	unsigned long       err_flag;
};

static inline unsigned long get_cpu_id(void)
{
	unsigned long cpuid;
	cpuid = (sprget_gpt(THID) >> THID_OFFSE_cpu) & \
		((1UL<<THID_WIDTH_cpu) - 1);	
	return cpuid;
}

static inline unsigned long get_vendor_id(void)
{
	unsigned long vid;
	vid = (sprget_gpt(THID) >> THID_OFFSE_vid) & \
		((1UL<<THID_WIDTH_vid) - 1);
	return vid;
}

#endif /* _ASM_PROCESSOR_H */
