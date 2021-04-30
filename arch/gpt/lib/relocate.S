/*
 * arch/gpt/lib/relocate.S
 *
 * relocate - common relocation function for GPT U-Boot
 *
 * Copyright (C) 2017, General Processor Techologies Inc.
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
#include <asm/asm-defs.h>
#include <machine/gpt_kernel.h>
/*
 * void	relocate_code(ulong, gd_t *, ulong) __attribute__ ((noreturn))
 * $r8 - gd->start_addr_sp
 * $r9 - gd->new_gd
 * $r10- gd->relocaddr
 */ 	  
ENTRY(relocate_code)
	ssetr	SCRATCH_SP, $r8
	ssetr	SCRATCH_GD, $r9	
	ssetr	SCRATCH_RELOC_ADDR, $r10
		
	/*
	 * Copy u-boot from flash to RAM
	 */
	ldapci	$a7, __image_copy_start 
	rseta	$r15, $a7
	subf	$r14, $r15, $r10	/* $r14 <- relocation offset */
	
	aaddri	$a6, $r10, 0		/* $a6 dest addr */
	ldapci	$a5, __image_copy_end	/* $a5 source end addr */
	taddpci $t1, copy_loop
	
copy_loop:
	ldul	$r15, $a7, 8		/* copy from source address */
	stul	$r15, $a6, 8		/* copy to   target address */
	jcau_lt	$t1, $a7, $a5

	/*
	 * Fix .rela.dyn relocations
	 */
	ldapci	$a7, __rel_dyn_start
	ldapci	$a5, __rel_dyn_end
	rsetli	$r11, 0xffffffff
fixloop:
	ldul	$r15, $a7, 8		/* offset */
	ldul	$r13, $a7, 8		/* info   */
	ldul	$r12, $a7, 8		/* addend */
	
	taddpci	$t1, fixnext		
	and	$r13, $r11, $r13
	jci_ne	$t1, $r13, 33		/* R_GPTX_RELATIVE is 33 */
	
	/* relative fix: store addend plus offset at dest location */		
	aaddri		$a6, $r15, 0
	aaddar_b	$a6, $a6, $r14		
	add		$r12, $r12, $r14
	stl		$r12, $a6
fixnext:
	taddpci	$t1, fixloop
	jcau_lt	$t1, $a7, $a5
	
return_addr:
	rsets	$r15, SCRATCH_IN_RAM
	add	$r15, $r15, $r14
	tsetr	$t7, $r15			
relocate_done:
	ret	
ENDPROC(relocate_code)


ENTRY(boot_relocate_code)
	ldapci	$a7, __image_copy_start 
	rseta	$r14, $a7
	/*
	 * Fix .rela.dyn relocations
	 */
	ldapci	$a7, __rel_dyn_start
	ldapci	$a5, __rel_dyn_end
fixloop1:
	ldul	$r15, $a7, 8		/* offset */
	ldul	$r13, $a7, 8		/* info   */
	ldul	$r12, $a7, 8		/* addend */
	
	taddpci	$t1, fixnext1	
	rsetli	$r11, 0xffffffff
	and	$r13, $r11, $r13
	jci_ne	$t1, $r13, 33		/* R_GPTX_RELATIVE */
	
	/* relative fix: store addend plus offset at dest location */		
	aaddri	$a6, $r15, 0
	stl	$r12, $a6
fixnext1:
	taddpci	$t1, fixloop1
	jcau_lt	$t1, $a7, $a5
	
	ret				
ENDPROC(boot_relocate_code)

