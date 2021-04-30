/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 99 Ralf Baechle
 * Copyright (C) 2000, 2002  Maciej W. Rozycki
 * Copyright (C) 1990, 1999 by Silicon Graphics, Inc.
 */
#ifndef _ASM_ADDRSPACE_H
#define _ASM_ADDRSPACE_H

/*
 *  Configure language
 */
#ifdef __ASSEMBLY__
#define _ATYPE_
#define _ATYPE32_
#define _ATYPE64_
#define _CONST64_(x)	x
#else
#define _ATYPE_		__PTRDIFF_TYPE__
#define _ATYPE32_	int
#define _ATYPE64_	__s64
#ifdef CONFIG_64BIT
#define _CONST64_(x)	x ## L
#else
#define _CONST64_(x)	x ## LL
#endif
#endif

/*
 *  32-bit MIPS address spaces
 */
#ifdef __ASSEMBLY__
#define _ACAST32_
#define _ACAST64_
#else
#define _ACAST32_		(_ATYPE_)(_ATYPE32_)	/* widen if necessary */
#define _ACAST64_		(_ATYPE64_)		/* do _not_ narrow */
#endif

/*
 * Returns the physical address of a CKSEGx / XKPHYS address
 */
#define CPHYSADDR(a)		((_ACAST32_(a)) & 0x1fffffff)
#define XPHYSADDR(a)		((_ACAST64_(a)) &			\
				 _CONST64_(0x000000ffffffffff))
/*
 * Cache modes for XKPHYS address conversion macros
 */
#define K_CALG_COH_EXCL1_NOL2	0
#define K_CALG_COH_SHRL1_NOL2	1
#define K_CALG_UNCACHED		2
#define K_CALG_NONCOHERENT	3
#define K_CALG_COH_EXCL		4
#define K_CALG_COH_SHAREABLE	5
#define K_CALG_NOTUSED		6
#define K_CALG_UNCACHED_ACCEL	7

/*
 * 64-bit address conversions
 */
#define XKPHYS_TO_PHYS(p)		((p) & TO_PHYS_MASK)
#define PHYS_TO_XKPHYS(cm, a)		(_CONST64_(0x8000000000000000) | \
					 (_CONST64_(cm) << 59) | (a))

/*
 * Returns the uncached address of a sdram address
 */
#ifndef __ASSEMBLY__
#if defined(CONFIG_SOC_AU1X00) || defined(CONFIG_TB0229)
/* We use a 36 bit physical address map here and
   cannot access physical memory directly from core */
#define UNCACHED_SDRAM(a) (((unsigned long)(a)) | 0x20000000)
#else	/* !CONFIG_SOC_AU1X00 */
#define UNCACHED_SDRAM(a) CKSEG1ADDR(a)
#endif	/* CONFIG_SOC_AU1X00 */
#endif	/* __ASSEMBLY__ */

/*
 * The ultimate limited of the 64-bit MIPS architecture:  2 bits for selecting
 * the region, 3 bits for the CCA mode.  This leaves 59 bits of which the
 * R8000 implements most with its 48-bit physical address space.
 */
#define TO_PHYS_MASK	_CONST64_(0x07ffffffffffffff)	/* 2^^59 - 1 */

#ifndef CONFIG_CPU_R8000

/*
 * The R8000 doesn't have the 32-bit compat spaces so we don't define them
 * in order to catch bugs in the source code.
 */

#define COMPAT_K1BASE32		_CONST64_(0xffffffffa0000000)
#define PHYS_TO_COMPATK1(x)	((x) | COMPAT_K1BASE32) /* 32-bit compat k1 */

#endif

#define KDM_TO_PHYS(x)		(_ACAST64_ (x) & TO_PHYS_MASK)
#define PHYS_TO_K0(x)		(_ACAST64_ (x) | CAC_BASE)

#endif /* _ASM_ADDRSPACE_H */
