/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef _ASM_SYSTEM_H
#define _ASM_SYSTEM_H

#include <asm/ptrace.h>
#include <asm/spr.h>
#include <machine/gpt_mach.h>

#ifndef __ASSEMBLY__
#define _ONE 1UL
#else
#define _ONE 1
#endif

/* Kernel passes these values to enable/disable interrupts. */
#define ARCH_IRQ_DISABLED       0
#define ARCH_IRQ_ENABLED        (_ONE << PSC_OFFSE_interrupt_1)
#define ARCH_IRQ_MASK           ARCH_IRQ_ENABLED

#define save_flags(x)	\
do{	\
	x=sprget_gpt(PSC);	\
}while(0)

#define restore_flags(x)	\
do{	\
	sprset_gpt(PSC,x);	\
}while(0)

#define local_irq_save(flags) \
do{	\
        save_flags(flags);	\
	sprset_gpt(PSC, (flags & (~ARCH_IRQ_MASK)));	\
}while(0)

#define local_irq_restore(flags)	\
do{	\
        sprset_gpt(PSC,(flags|ARCH_IRQ_MASK));	\
}while(0)

#define local_irq_disable()	\
do{	\
	unsigned long temp=sprget_gpt(PSC);	\
	sprset_gpt(PSC, (temp & (~ARCH_IRQ_MASK)));	\
}while(0)

#define local_irq_enable()	\
do{	\
	unsigned long temp=sprget_gpt(PSC);	\
	sprset_gpt(PSC, (temp | ARCH_IRQ_MASK));	\
}while(0)

#define prepare_to_switch()	do { } while(0)
#define switch_to(prev,next,last) \
do { \
	(last) = resume(prev, next); \
} while(0)

#define xchg(ptr,x) ((__typeof__(*(ptr)))__xchg((unsigned long)(x),(ptr),sizeof(*(ptr))))
#define tas(ptr) (xchg((ptr),1))

static __inline__ unsigned long
__xchg(unsigned long x, volatile void * ptr, int size)
{
	extern void __bad_xchg(volatile void *, int);

	unsigned long flags, ret;	
	switch (size) {
                case 1:
                        local_irq_save(flags);
                        ret = *(volatile unsigned char *)ptr;
                        *(volatile unsigned char *)ptr = x;
                        local_irq_restore(flags);
                break;

		case 4:
			local_irq_save(flags);
                        ret = *(volatile unsigned long *)ptr;
                        *(volatile unsigned long *)ptr = x;
			local_irq_restore(flags);
		break;

		default:
			__bad_xchg(ptr, size), ret = 0;
	}
	return ret;
}

#endif /* _ASM_SYSTEM_H */
