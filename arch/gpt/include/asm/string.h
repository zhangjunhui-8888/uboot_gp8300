/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef _ASM_STRING_H
#define _ASM_STRING_H

#include <config.h>

/*
 * We don't do inline string functions, since the
 * optimised inline asm versions are not small.
 */

#undef __HAVE_ARCH_STRCPY
#ifdef CONFIG_USE_ARCH_STRCPY
#define __HAVE_ARCH_STRCPY
#endif
extern char *strcpy(char *__dest, __const__ char *__src);

#undef __HAVE_ARCH_STRNCPY
extern char *strncpy(char *__dest, __const__ char *__src, __kernel_size_t __n);

#undef __HAVE_ARCH_STRLEN
#ifdef CONFIG_USE_ARCH_STRLEN
#define __HAVE_ARCH_STRLEN
#endif
extern size_t strlen(__const__ char *__s);

#undef __HAVE_ARCH_STRCMP
#ifdef CONFIG_USE_ARCH_STRCMP
#define __HAVE_ARCH_STRCMP
#endif
extern int strcmp(__const__ char *__cs, __const__ char *__ct);

#undef __HAVE_ARCH_STRNCMP
extern int strncmp(__const__ char *__cs, __const__ char *__ct, __kernel_size_t __count);

#undef __HAVE_ARCH_MEMSET
#ifdef CONFIG_USE_ARCH_MEMSET
#define __HAVE_ARCH_MEMSET
#endif
extern void *memset(void *__s, int __c, __kernel_size_t __count);

#undef __HAVE_ARCH_MEMCPY
#ifdef CONFIG_USE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMCPY
#endif
extern void *memcpy(void *__to, __const__ void *__from, __kernel_size_t __n);

#undef __HAVE_ARCH_MEMMOVE
extern void *memmove(void *__dest, __const__ void *__src, __kernel_size_t __n);

#endif /* _ASM_STRING_H */
