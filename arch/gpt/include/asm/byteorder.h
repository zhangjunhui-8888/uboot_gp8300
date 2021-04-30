/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 99, 2003 by Ralf Baechle
 */
#ifndef _ASM_BYTEORDER_H
#define _ASM_BYTEORDER_H

#include <asm/types.h>

#ifdef __GNUC__


#if !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#endif /* __GNUC__ */

#if defined(__GPTEB__)
#  include <linux/byteorder/big_endian.h>
#elif defined(__GPTEL__)
#  include <linux/byteorder/little_endian.h>
#else
#  error "GPT, but neither __GPTEB__, nor __GPTEL__???"
#endif

#endif /* _ASM_BYTEORDER_H */