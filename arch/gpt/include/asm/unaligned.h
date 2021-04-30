#ifndef _ASM_GPT_UNALIGNED_H
#define _ASM_GPT_UNALIGNED_H

#include <linux/compiler.h>
#if defined(__GPTEB__)
#define get_unaligned	__get_unaligned_be
#define put_unaligned	__put_unaligned_be
#elif defined(__GPTEL__)
#define get_unaligned	__get_unaligned_le
#define put_unaligned	__put_unaligned_le
#else
#error "GPT, but neither __GPTEB__, nor __GPTEL__???"
#endif

#include <linux/unaligned/le_byteshift.h>
#include <linux/unaligned/be_byteshift.h>
#include <linux/unaligned/generic.h>

#endif
