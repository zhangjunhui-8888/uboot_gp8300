/*
 * arch/gpt/include/asm/asm-defs.h
 *
 * GPT low level macro definitions.
 *
 * Copyright (C) 2015, Optimum Semiconductor Technologies
 *  Enrique Barria <ebarria@optimumsemi.com>
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

/* generate the list of gpt sp registers.
   Aternative approach in file ~/arch/gpt/kernel/asm-offsets.c */
#define SPR__(a,b)      .equ a,b
#include <machine/gpt_spr.h>
#undef SPR__

#define SCRATCH_SP		$s0
#define SCRATCH_GD		$s1
#define	SCRATCH_RELOC_ADDR	$s2
#define	SCRATCH_IN_RAM		$s3

.macro ldtpci td, label
        taddpcil        $t4, \label
        taddti          \td, $t4, \label
.endm

.macro ldapci ad, label
        aaddpcilb       \ad, \label
        aadduib         \ad, \label
.endm

.macro rsetui rd, offset, val
        .if (\val < 0)
        rseti           \rd, ((\val) >> \offset) | 0xffffffffffff0000
        .else
        rseti           \rd, ((\val) >> \offset)
        .endif
.endm
/* set long immediate number */
.macro rsetli rd, val
        .if ((\val) >> 47)
        rsetui          \rd, 48, (\val)
        insui           \rd, ((\val) >> 32) & 0xffff
        insui           \rd, ((\val) >> 16) & 0xffff
        insui           \rd, ((\val) & 0xffff)
        .elseif ((\val) >> 31)
        rsetui          \rd, 32, (\val)
        insui           \rd, ((\val) >> 16) & 0xffff
        insui           \rd, ((\val) & 0xffff)
        .elseif ((\val) >> 15)
        rsetui          \rd, 16, (\val)
        insui           \rd, (\val) & 0xffff
        .else
        rseti           \rd, (\val)
        .endif
.endm

.macro cal td, label
        ldtpci          \td, \label
        call            \td
.endm

.macro jmp td, label
        ldtpci          \td, \label
        j               \td
.endm

.macro  get_cpuid rd
	rsetspr         \rd, THID
	extrui          \rd, \rd, THID_OFFSE_cpu, THID_WIDTH_cpu
.endm


/* Implements _pa(va) to convert virtual address to real address. */
.macro  TOPHYS rd rs
        ori             \rd,\rd,-1
        shli            \rd,\rd,PAGE_OFFSET_sft
        subf            \rd,\rd,\rs
.endm

//#define PHYS(label) (label - ((-1) << PAGE_OFFSET_sft))
#define PHYS(label) (label)

/* Implements _va(pa) to transform real address to virtual address. */
.macro  TOVIRT_A ad rs
        ori             \rs,\rs,-1
        shli            \rs,\rs,PAGE_OFFSET_sft
        aaddar_b        \ad,\ad,\rs
.endm

/* Implements _pa(va) to transform virtual address to real address. */
.macro  TOPHYS_A ad rs
        rseti           \rs,1
        shli            \rs,\rs,PAGE_OFFSET_sft
        aaddar_b        \ad,\ad,\rs
.endm

.macro lshbit rd, offset
        .if \offset < 16
        rseti           \rd, 1 << \offset
        .else
        rseti           \rd, 1
        shli            \rd, \rd, \offset
        .endif
.endm


.macro clearbitr rd, r1, offset
        lshbit          \r1, \offset
	cand            \rd, \r1, \rd
.endm

/* Sets all registers to the value in scratch register _rreg */
#define GPT_GR_SET_REGS(_rreg)           \
        addi            $r15,$_rreg,0    ;\
        addi            $r14,$_rreg,0    ;\
        addi            $r13,$_rreg,0    ;\
        addi            $r12,$_rreg,0    ;\
        addi            $r11,$_rreg,0    ;\
        addi            $r10,$_rreg,0    ;\
        addi            $r9,$_rreg,0     ;\
        addi            $r8,$_rreg,0     ;\
        addi            $r7,$_rreg,0     ;\
        addi            $r6,$_rreg,0     ;\
        addi            $r5,$_rreg,0     ;\
        addi            $r4,$_rreg,0     ;\
        addi            $r3,$_rreg,0     ;\
        addi            $r2,$_rreg,0     ;\
        addi            $r1,$_rreg,0     ;\
        addi            $r0,$_rreg,0     ;\
        aaddri          $a0,$_rreg,0     ;\
        aaddri          $a1,$_rreg,0     ;\
        aaddri          $a2,$_rreg,0     ;\
        aaddri          $a3,$_rreg,0     ;\
        aaddri          $a4,$_rreg,0     ;\
        aaddri          $a5,$_rreg,0     ;\
        aaddri          $a6,$_rreg,0     ;\
        aaddri          $a7,$_rreg,0     ;\
        aaddri          $a8,$_rreg,0     ;\
        aaddri          $a9,$_rreg,0     ;\
        aaddri          $a10,$_rreg,0     ;\
        aaddri          $a11,$_rreg,0     ;\
        aaddri          $a12,$_rreg,0     ;\
        aaddri          $a13,$_rreg,0     ;\
        aaddri          $a14,$_rreg,0     ;\
        aaddri          $a15,$_rreg,0     ;\
        ssetr           $s0,$_rreg       ;\
        ssetr           $s1,$_rreg       ;\
        ssetr           $s2,$_rreg       ;\
        ssetr           $s3,$_rreg       ;\
        ssetr           $s4,$_rreg       ;\
        ssetr           $s5,$_rreg       ;\
        ssetr           $s6,$_rreg       ;\
        ssetr           $s7,$_rreg       ;\
        ssetr           $s8,$_rreg       ;\
        ssetr           $s9,$_rreg       ;\
        ssetr           $s10,$_rreg       ;\
        ssetr           $s11,$_rreg       ;\
        ssetr           $s12,$_rreg       ;\
        ssetr           $s13,$_rreg       ;\
        ssetr           $s14,$_rreg       ;\
        ssetr           $s15,$_rreg       ;\
        tsetr           $t0,$_rreg       ;\
        tsetr           $t1,$_rreg       ;\
        tsetr           $t7,$_rreg

/* Refresh hardware shadow registers */
#define GPT_REF_SHADOW_REGS()           \
        aaddai          $a0,$a0,0       ;\
        aaddai          $a1,$a1,0       ;\
        aaddai          $a2,$a2,0       ;\
        aaddai          $a3,$a3,0       ;\
        aaddai          $a4,$a4,0       ;\
        aaddai          $a5,$a5,0       ;\
        aaddai          $a6,$a6,0       ;\
        aaddai          $a7,$a7,0       ;\
        aaddai          $a8,$a8,0       ;\
        aaddai          $a9,$a9,0       ;\
        aaddai          $a10,$a10,0       ;\
        aaddai          $a11,$a11,0       ;\
        aaddai          $a12,$a12,0       ;\
        aaddai          $a13,$a13,0       ;\
        aaddai          $a14,$a14,0       ;\
        aaddai          $a15,$a15,0                                                               
/*
 * u64 set_PSC(u64 new_value);
 * In :  $r8 is new value of psc
 * Out:  $r8 the orginal value of psc
 * */
#define	GPT_set_PSC(val)		\
	addi		$r8, $val, 0	;\
	aaddpci		$a4, 1f		;\
	rseta		$r9, $a4	;\
	sprsetr		$r9, XRA	;\
	sprsetr		$r8, XSA	;\
        retfi_alt			;\
1:					;\


/*
 * u64 get_PSC(void);
 * Out:  $r8 the current value of psc
 * */

#define	GPT_get_PSC()			\
	rsetspr         $r8,PSC		;\

/*
 *      Procedures to save state machine in stack.
 *
 *      Must follow layout defined in struct pt_regs {}
 *      in ~/arch/gpt/include/ptrace.h
 *
 *      pt_regs has three componenets,
 *              general registers ($r.., $a...)
 *              machine registers (SPR, PC, ...)
 *              branch registers ($t0, $t1...)
 */

/* Store/Load register on stack pointed by $asp. _rreg is scratch register. */
#define RREG_SAVE_STK(rreg)             stul            $rreg,$asp,-8    ;
#define AREG_SAVE_STK(areg,_rreg)       rseta           $_rreg,$areg    ;\
                                        stul            $_rreg,$asp,-8   ;
#define SREG_SAVE_STK(sreg,_rreg)       rsets           $_rreg,$sreg    ;\
                                        stul            $_rreg,$asp,-8   ;
#define TREG_SAVE_STK(treg,_rreg)       rsett           $_rreg,$treg    ;\
                                        stul            $_rreg,$asp,-8   ;
#define FREG_SAVE_STK(rreg)             stuf            $rreg,$asp,-8    ;

#define RREG_RSTR_STK(rreg)             ldul            $rreg,$asp,8     ;
#define AREG_RSTR_STK(areg,_rreg)       ldul            $_rreg,$asp,8    ;\
                                        aaddri          $areg,$_rreg,0  ;
#define SREG_RSTR_STK(sreg,_rreg)       ldul            $_rreg,$asp,8    ;\
                                        ssetr           $sreg,$_rreg    ;
#define TREG_RSTR_STK(treg,_rreg)       ldul            $_rreg,$asp,8    ;\
                                        tsetr           $treg,$_rreg    ;
#define FREG_RSTR_STK(rreg)             lduf            $rreg,$asp,8     ;


/* Store General Registers. */
#define GPT_GR_SAVE(_rreg)      \
        RREG_SAVE_STK(r15)      \
        RREG_SAVE_STK(r14)      \
        RREG_SAVE_STK(r13)      \
        RREG_SAVE_STK(r12)      \
        RREG_SAVE_STK(r11)      \
        RREG_SAVE_STK(r10)      \
        RREG_SAVE_STK(r9)       \
        RREG_SAVE_STK(r8)       \
        RREG_SAVE_STK(r7)       \
        RREG_SAVE_STK(r6)       \
        RREG_SAVE_STK(r5)       \
        RREG_SAVE_STK(r4)       \
        RREG_SAVE_STK(r3)       \
        RREG_SAVE_STK(r2)       \
        RREG_SAVE_STK(r1)       \
        RREG_SAVE_STK(r0)       \
        AREG_SAVE_STK(a7,_rreg) \
        AREG_SAVE_STK(a6,_rreg) \
        AREG_SAVE_STK(a5,_rreg) \
        AREG_SAVE_STK(a4,_rreg) \
        AREG_SAVE_STK(a3,_rreg) \
        AREG_SAVE_STK(a2,_rreg) \
        AREG_SAVE_STK(a1,_rreg) \
        SREG_SAVE_STK(s2,_rreg)

/* Restore General Registers. $asp is restored when stack is emptied.  */
#define GPT_GR_RSTR(_rreg)      \
        aaddai  $asp,$asp,8       ; /* skips over value of $a0 in stack */ \
        AREG_RSTR_STK(a1,_rreg) \
        AREG_RSTR_STK(a2,_rreg) \
        AREG_RSTR_STK(a3,_rreg) \
        AREG_RSTR_STK(a4,_rreg) \
        AREG_RSTR_STK(a5,_rreg) \
        AREG_RSTR_STK(a6,_rreg) \
        AREG_RSTR_STK(a7,_rreg) \
        RREG_RSTR_STK(r0)       \
        RREG_RSTR_STK(r1)       \
        RREG_RSTR_STK(r2)       \
        RREG_RSTR_STK(r3)       \
        RREG_RSTR_STK(r4)       \
        RREG_RSTR_STK(r5)       \
        RREG_RSTR_STK(r6)       \
        RREG_RSTR_STK(r7)       \
        RREG_RSTR_STK(r8)       \
        RREG_RSTR_STK(r9)       \
        RREG_RSTR_STK(r10)      \
        RREG_RSTR_STK(r11)      \
        RREG_RSTR_STK(r12)      \
        RREG_RSTR_STK(r13)      \
        RREG_RSTR_STK(r14)      \
        RREG_RSTR_STK(r15)


/* Save Machine state registers In: $t0 is exception entry. Out: $_rreg is PSC*/
#define GPT_MR_SAVE(lv,_rreg)                   \
        rsett           $_rreg,$t0              ; /* Exc. entry */\
        RREG_SAVE_STK(_rreg)                    ;\
.ifc    T,##lv                                  ;\
        rseti           $_rreg,0                ; /* skips over Addr,Inst  */\
        RREG_SAVE_STK(_rreg)                    ;\
        RREG_SAVE_STK(_rreg)                    ;\
.else                                           ;\
        rsetspr         $_rreg,IXA##lv          ; /* Addr. */\
        RREG_SAVE_STK(_rreg)                    ;\
        rsetspr         $_rreg,IXI##lv          ; /* Ins. */\
        RREG_SAVE_STK(_rreg)                    ;\
.endif                                          ;\
        rsetspr         $_rreg,XR##lv           ; /* PC */ \
        RREG_SAVE_STK(_rreg)                    ;\
        rsetspr         $_rreg,XS##lv           ; /* SR */ \
        RREG_SAVE_STK(_rreg)                    ;\

/* Restore Machine state registers, (only XS, XR). */
#define GPT_MR_RSTR(lv,_rreg)                   \
        RREG_RSTR_STK(_rreg)                    ;\
        sprsetr         $_rreg,XS##lv           ; /* SR */ \
        RREG_RSTR_STK(_rreg)                    ;\
        sprsetr         $_rreg,XR##lv           ; /* PC */ \
        aaddai          $asp,$asp,MACH_REGS_SIZE-16; /* skips over rest  */


/* Save Branch/misc registers */
#define GPT_BR_SAVE(_rreg)                       \
        RREG_SAVE_STK(_rreg)                    ;\
        SREG_SAVE_STK(s2,_rreg)                 ;\
        TREG_SAVE_STK(t7,_rreg)                 ;\
        TREG_SAVE_STK(t1,_rreg)                 ;\
        TREG_SAVE_STK(t0,_rreg)

/* Restore Branch/misc registers */
#define GPT_BR_RSTR(_rreg)                       \
        TREG_RSTR_STK(t0,_rreg)                 ;\
        TREG_RSTR_STK(t1,_rreg)                 ;\
        TREG_RSTR_STK(t7,_rreg)                 ;\
        SREG_RSTR_STK(s2,_rreg)                 ;\
        ldl             $_rreg,$asp

/*
 *      Resume exection at specified 'label' with given PSC.
 *      Uses XSA,XRA to atomically set PSC when resuming at 'label'
 *
 * args:
 *      psc: PSC value in register
 *      label: address of location where to resume execution.
 */
#define SET_PSC_RESUME(psc,label)        \
        sprsetr         $psc,XSA        ;\
        rseta           $psc,$label     ;\
        sprsetr         $psc,XRA        ;\
        taddpci         $t1,1f          ;\
        call            $t1             ;\
1:                                      ;\
        retfi_alt

/* Load PSC indirectly through XSA/XRA and retfi instead of through sprset */
.macro  LOAD_PSC rs
        sprsetr         \rs,XSA
        taddpci         $t1,2f
        rsett           \rs,$t1
        sprsetr         \rs,XRA
        taddpci         $t1,1f
        call            $t1
1:
        retfi_alt
2:
.endm

/* OR \rs to content of CPUC and PSC */
.macro  OR_CPUC_SPR rd rs
        rsetspr         \rs,CPUC
        or              \rs,\rs,\rd
	barrier
        sprsetr         \rs,CPUC
        rsetspr         \rs,PSC
	barrier
        or              \rs,\rs,\rd
        LOAD_PSC        \rs
.endm

.macro or_spr rd, rs, name
        rsetspr         \rs, \name
        or              \rs, \rs, \rd
        sprsetr         \rs, \name
.endm

.macro set_psc_safe	rreg, areg
	sprsetr		\rreg, XSA
	aaddpci		\areg, 1f
	rseta		\rreg, \areg
	sprsetr		\rreg, XRA
	retfi_alt
	1:
.endm

.macro	or_psc_safe	rreg0, rreg1 ,areg
	rsetspr		\rreg0, PSC
	or		\rreg0, \rreg1, \rreg0
	set_psc_safe	\rreg0, \areg
.endm
	
/* To disable IRQs in local machine (level1 interrupts) */
/* arch_local_irq_restore(ARCH_IRQ_DISABLED) */
.macro LOCAL_IRQ_DISABLE r1 r2 a1
        rsetspr         \r1,PSC
	ldri		\r2, ARCH_IRQ_MASK
	cand		\r1, \r2, \r1
	ldri		\r2, ARCH_IRQ_DISABLED
        or              \r1,\r1,\r2
        LOAD_PSC        \r1
.endm

/* To enable IRQs in local machine (level1 interrupts) */
/* arch_local_irq_restore(ARCH_IRQ_ENABLED) */
.macro LOCAL_IRQ_ENABLE r1 r2 a1
        rsetspr         \r1,PSC
	ldri		\r2, ARCH_IRQ_MASK
	cand		\r1, \r2, \r1
	ldri		\r2, ARCH_IRQ_ENABLED
        or              \r1,\r1,\r2
        LOAD_PSC        \r1
.endm

#define GPT_STATE_SAVE(_areg,_rreg)     \
        PTREG_SAVE_AREG(a1,_areg)       \
        PTREG_SAVE_AREG(a2,_areg)       \
        PTREG_SAVE_AREG(a3,_areg)       \
        PTREG_SAVE_RREG(r0,_areg)       \
        PTREG_SAVE_RREG(r1,_areg)       \
        PTREG_SAVE_RREG(r2,_areg)       \
        PTREG_SAVE_RREG(r3,_areg)       \
        PTREG_SAVE_RREG(r4,_areg)       \
        PTREG_SAVE_RREG(r5,_areg)       \
        PTREG_SAVE_TREG(t7,_areg,_rreg)

#define GPT_STATE_RSTR(_areg,_rreg)     \
        PTREG_RSTR_AREG(a1,_areg)       \
        PTREG_RSTR_AREG(a2,_areg)       \
        PTREG_RSTR_AREG(a3,_areg)       \
        PTREG_RSTR_RREG(r0,_areg)       \
        PTREG_RSTR_RREG(r1,_areg)       \
        PTREG_RSTR_RREG(r2,_areg)       \
        PTREG_RSTR_RREG(r3,_areg)       \
        PTREG_RSTR_RREG(r4,_areg)       \
        PTREG_RSTR_RREG(r5,_areg)       \
        PTREG_RSTR_TREG(t7,_areg,_rreg)


