/*
 * arch/gpt/lib/cache_init.S
 *
 * GPT cache initilization code.
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
#include <asm/asm-defs.h>
#include <machine/gpt_kernel.h>

ENTRY(cache_init)        
        rsett   $r6,$t7
        /* invalidate cache:
           ictl_flush_all  $r15 # invalid instruction
        */
#ifndef CONFIG_SYS_ICACHE_OFF
/* invalidate_icache */
        rseti           $r8,0x1                 # 2nd page, page 0 verbotten
        shli            $r8,$r8,DWV_OFFSE_ra    # page size
        rseti           $r15,256                # number of lines to process (size/szln)
        taddpci         $t0,1f                  # top of  loop
1:
        sprsetr         $r8,ICCTL               # icache invalidate
        addi            $r8,$r8,32              # advance next line
        jdecnz          $t0,$r15,$r15           # each line

        rseti           $r15,0
        rseti           $r14,1
        shli            $r14,$r14,PSC_OFFSE_cache_ic
        OR_CPUC_SPR     $r14,$r15
#endif
#ifndef CONFIG_SYS_DCACHE_OFF
/* invalidate_dcache */
        rseti           $r8,0x1                 # 2nd page, page 0 verbotten
        shli            $r8,$r8,DWV_OFFSE_ra    # page size
        rseti           $r15,256                # number of lines to process (size/szln)
        taddpci         $t0,1f                  # top of  loop
1:
        aaddri          $a4,$r8,0               # for dctl
        addi            $r8,$r8,32              # advance next line
        dctl_invalid_l1 $a4                     # dcache invalidate
        jdecnz          $t0,$r15,$r15           # each line

        rseti           $r15,0
        rseti           $r14,1
        shli            $r14,$r14,PSC_OFFSE_cache_dc
        OR_CPUC_SPR     $r14,$r15
#endif
#ifndef CONFIG_SYS_L2CACHE_OFF
/*invalidate l2cache*/

#define lnl2 21                              /* 2 mbytes                    */
#define l2szln 5                             /* 32 bytes / line             */
#define l2setln 3                            /* 8 cache sets in L2          */
#define l2ind (lnl2-l2szln-l2setln)          /* result: 13 bit index        */
invalidate_l2:
        rseti           $r8,0x1                 # 2nd page, page 0 verbotten
        shli            $r8,$r8,DWV_OFFSE_ra    # page size
        aaddri          $a4,$r8,0
        rseti           $r15,1
        shli            $r15,$r15,l2ind         # number of lines to process (size/(szln*#sets)
        taddpci         $t0,1f                  # top of  loop
1:                                              # each cache line
        dctl_invalid_l2 $a4                     # dcache invalidate
        aaddai          $a4,$a4,32              # advance next line
        jdecnz          $t0,$r15,$r15           # each line

        rseti           $r15,0
        rseti           $r14,1
        shli            $r14,$r14,PSC_OFFSE_cache_l2c
        OR_CPUC_SPR     $r14,$r15
#endif
        tsetr   	$t7,$r6
        ret
ENDPROC(cache_init)
/*
 * dcache_status - get cache status
 *
 * RETURNS: 0 - cache disabled; 1 - cache enabled
 *
 */
ENTRY(dcache_status)
	GPT_get_PSC();
	rseti		$r15, 0x1
	shli		$r15, $r15, CPUC_OFFSE_cache_dc
	and		$r8, $r8, $r15
	shrai		$r8, $r8, CPUC_OFFSE_cache_dc
	ret
ENDPROC(dcache_status)
/*
 * dcache_disable - disable cache
 *
 * RETURNS: N/A
 *
 */
ENTRY(dcache_disable)
	GPT_get_PSC();			#ret val -> r8
	rseti		$r15, 0x00
	candi		$r15, $r15, -1
	rseti		$r14, 0x01
	shli		$r14, $r14, PSCH_OFFSE_cache_dc
	cand		$r15, $r14, $r15
	and		$r8, $r8, $r15

	GPT_set_PSC(r8)
	ret
ENDPROC(dcache_disable)
/*
 * dcache_enable - enable cache
 *
 * RETURNS: N/A
 *
 */
ENTRY(dcache_enable)
	GPT_get_PSC();
	rseti		$r15, 0x01
	shli		$r15, $r15, PSCH_OFFSE_cache_dc
	or		$r8, $r8, $r15
	GPT_set_PSC(r8)
	ret
ENDPROC(dcache_enable)	
