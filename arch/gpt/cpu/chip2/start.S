/*
 * arch/gpt/cpu/start.S
 *
 * GPT boot startup code.
 *
 * Copyright (C) 2016, General Processor Techologies Inc.
 * xiesc <xiesc@hxgpt.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <config.h>
#include <linux/linkage.h>
#include <machine/gpt_kernel.h>
#include <asm/asm-defs.h>
#include <asm/asm-offsets.h>
#include <asm-generic/global_data.h>
#include <asm/asm-debug.h>

        .globl _start
_start:
#ifdef	CONFIG_SPL_BUILD
#include <asm/safe_boot_sequence.h>
#endif
	rseti           $r15,0
        sprsetr         $r15,XLV
        sprsetr         $r15,XHS
        sprsetr         $r15,XMS
        sprsetr         $r15,XEN
#ifdef	CONFIG_SPL_BUILD
        sprsetr         $r15,L2CCTLC
#endif
        sprsetr         $r15,CPUC
        sprsetr         $r15,PSC
        sprsetr         $r15,PSCH
        sprsetr         $r15,PSCK
        GPT_GR_SET_REGS(r15)

	rseti		$r15, -1
#ifdef	CONFIG_SYS_OUT_OF_ORDER_OFF
	clearbitr	$r15, $r14, CPUC_OFFSE_ooen
#endif
        sprsetr         $r15, CPUC
set_procid:
        /* Set PSC procid to 1 */
#define PROCID 1
#define PROCID_MASK     (~(((1 << PSC_WIDTH_procid) - 1) << PSC_OFFSE_procid))
        rseti           $r14, PROCID
        shli            $r14, $r14, PSC_OFFSE_procid
        or_psc_safe	$r15, $r14, $a7
set_fpu:
 	rseti           $r14, 1
        shli            $r14, $r14, PSC_OFFSE_unit_fpu
        or_psc_safe	$r15, $r14, $a7

set_mem_io:
	rseti		$r14, 1
	shli		$r14, $r14, PSC_OFFSE_mem_io
	or_psc_safe     $r15, $r14, $a7


#ifdef	CONFIG_GPT_SMP_SUPPORT
smp_setup:
	get_cpuid	$r15
	taddpci		$t0, bound_setup
	jci_eq		$t0, $r15, 0
smp_boot_check:
	/* get cpu id and match */
	rsetli		$r14, CONFIG_BP_DATA_ADDR
	rsetli		$r13, SMP_CPULAUNCH_SIZE
	/* calculate offset */
	mul_uu		$r13, $r13, $r15
	add		$r14, $r14, $r13
cpuid_check:       
	aaddri		$a6, $r14, SMP_CPU_ID
	ldl		$r12, $a6

	taddpci		$t0, secondary_start
	jc_eq		$t0, $r15, $r12
       
	taddpci		$t1, delay
	rseti		$r11, 1024       
delay:
	nop
	jdecnz		$t1, $r11, $r11
		
	taddpci		$t0, cpuid_check
	j		$t0
secondary_start:
//#ifdef	CONFIG_DBG_MONITOR
	rsetli       $r13, MC_DBG_STACK_TOP
	ssetr        $s15, $r13
//#endif

	aaddri		$a7, $r14, SMP_JUMP_ADDR
	ldt		$t1, $a7
	j		$t1
#endif

bound_setup:
#ifdef	CONFIG_SPL_BUILD
tcm_setup:
	rsetli		$r15, CONFIG_SPL_TCM_START_ADDR
	rseti		$r14, 1
	shli		$r14, $r14, L2CCTL_OFFSE_mem
	or		$r15, $r15, $r14

	barrier
	barrier
	barrier

	sprsetr		$r15, L2CCTLC
	barrier
	barrier
	barrier

	barrier
	barrier
	barrier
	barrier
#endif
	rseti		$r15, 0
        sprsetr         $r15, IBASE
        sprsetr         $r15, DBASE

	rseti		$r15, -1
        sprsetr         $r15,IBND
        sprsetr         $r15,DBND

#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	cal		$t0, lowlevel_init
#endif

#ifdef	CONFIG_SPL_BUILD
cp_data_section:
	ldapci		$a7, __data_start
	ldapci		$a6, __data_end
	ldapci          $a5, __data_load_start

	taddpci         $t1, L.loop
L.loop:
	ldul            $r14, $a5, 8
	stul            $r14, $a7, 8

	jca_ne		$t1, $a7, $a6

clear_bss:
	/* clear _bss, i.e. region from __bss_start to _end */
	ldapci		$a6, __bss_start
	ldapci		$a7, __bss_end
        rseti           $r13, 0

        taddpci         $t0, 1f
1:
        stul            $r13, $a6, 8
        jca_ne          $t0, $a7, $a6
#endif

#ifndef CONFIG_SPL_SKIP_RELOCATE
/* Since the gpt-sim can't support load dyn section, 
 * we have to add this.
 */
#ifdef CONFIG_TARGET_CHIP2_SIM
	cal		$t0, boot_relocate_code
#endif
#endif
    /* setup stack*/
start_setup_stack:
#if	defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_STACK)
	rsetli		$r14, CONFIG_SPL_STACK
	rsetli		$r15, CONFIG_SPL_STACKSIZE
#else
	rsetli		$r14, CONFIG_SYS_INIT_SP_ADDR
	rsetli		$r15, CONFIG_STACKSIZE
#endif

        add             $r13, $r14, $r15
        aaddri          $asp, $r13, -8

in_flash:
	rseta   	$r8, $asp
	cal		$t0, board_init_f_mem
	aaddri          $asp, $r8, 0

init_cache:
	cal		$t0, cache_init

/*
 * The scratch registers $s0-$s3 are used to save paramters,
 * refer to lib/relocate.S for details.
 */
#ifndef	CONFIG_SPL_BUILD
	ldtpci		$t0, in_ram
	rsett		$r15, $t0
	ssetr		SCRATCH_IN_RAM, $r15
#endif
#ifdef CONFIG_SPL_SKIP_RELOCATE
	/* $a2 is the GD pointer defined in asm/global_data.h */
	rsetli		$r15, GD_FLG_SKIP_RELOC
	aaddai		$a7, $a2, GD_FLAGS
	ldl		$r8, $a7
	or		$r8, $r8, $r15
	stl		$r8, $a7

	sseta		SCRATCH_SP, $asp
	sseta		SCRATCH_GD, $a2
	/* the relocation address is not used */
	rseti		$r15, 0
	ssetr		SCRATCH_RELOC_ADDR, $r15
#else
	rseti		$r8, 0
#endif
	cal		$t0, board_init_f

in_ram:
	/*
	 * Set new stack pointer
	 */
	asets		$asp, SCRATCH_SP

clear_new_bss:
	/* clear _bss, i.e. region from __bss_start to _end */
	ldapci		$a6, __bss_start
	ldapci		$a7, __bss_end
        rseti           $r13, 0

        taddpci         $t0, 1f
1:
        stul            $r13, $a6, 8
        jca_ne          $t0, $a7, $a6

	/* call board_init_r(gd_t *id, ulong dest_addr) */
	rsets		$r8, SCRATCH_GD
	rsets		$r9, SCRATCH_RELOC_ADDR

	jmp		$t0, board_init_r

/* Should never be here */
ENTRY(fatal_error)
        taddpci         $t0,1f
1:
        j               $t0;

