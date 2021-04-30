#
# (C) Copyright 2016
# scxie,General Processor Technologies Inc, scxie@hxgpt.com.
# 
# SPDX-License-Identifier:      GPL-2.0+
#
#include <config.h>
#include <linux/linkage.h>
#include <asm/asm-defs.h>
#include <machine/gpt_kernel.h>
/*
	adjust extbus clock from 32MHZ to 64MHZ
	all these code should be aligned to 32bytes icache line
*/

#define EXT_BUS_CLK_64MHZ	(0x42)
#define EXT_BUS_CLK_128MHZ	(0x41)
#if defined CONFIG_EXT_BUS_CLK_128MHZ
#define EXT_BUS_CLK		EXT_BUS_CLK_128MHZ
#elif defined CONFIG_EXT_BUS_CLK_64MHZ
#define EXT_BUS_CLK		EXT_BUS_CLK_64MHZ
#else
#define SKIP_EXT_BUS_CLK_ADJUST
#endif
ENTRY(lowlevel_init)
	rsett   $r6,$t7
	rseti		$r8,0xf
	rseti		$r9,0
	rseti		$r10,1
	shli		$r10,$r10,31	 #enable gpio2
        taddpci         $t0,apiu_set
        call            $t0
	nop

        rseti           $r8,0xb           #gpio2,set bit0 as output
        rseti           $r9,0x400         
        rseti           $r10,1       
	call		$t0
        rseti           $r8,0xb           #gpio2,set bit0=0,default ext bus clock=32MHZ
        rseti           $r9,(1<<2)         
        rseti           $r10,0       
	call		$t0	
#ifndef SKIP_EXT_BUS_CLK_ADJUST
#24 	
	nop
	nop
        rseti           $r8,0x1
        taddpci         $t1,loop
        call  $t1
	rseti		$r8,0xe		#sys ctrl
	rseti           $r9,0
	rseti           $r10,EXT_BUS_CLK	
#40
	call		$t0
	rseti		$r8,0xb
        rseti           $r9,(1<<2)	#set gpio2[0]=1,(address offset:1<<2-9bits)
        rseti           $r10,1         	
	call		$t0
	rseti           $r8,0x7fff	#0x3ff      #delay
        call  $t1
	nop
#endif
#88
disable_cpu_odt:
	rseti           $r8,0xe		
	rseti           $r9,0x4
	rseti           $r10,0x801	
	call		$t0
	nop
	nop
	tsetr           $t7,$r6	
	ret
#96

loop:
        taddpci         $t1,1f
1:
	nop
	nop
	nop
	nop
	nop
        jdecnz          $t1,$r8,$r8
	ret

/*void apiu_set(dev_addr,reg,val)
  dev_addr:
	0x8:uart1
	0x9:uart2
	0xa:gpio1
	0xb:gpio2
	0xe:sys ctrl
*/
apiu_set:	
	rseti		$r15,0xf
	shli		$r15,$r15,28		#0xf0000000,apiu base address
	shli		$r8,$r8,12				
	add		$r15,$r15,$r8		#device address
	or		$r15,$r15,$r9		#reg address
	aaddri		$a7,$r15,0
	stw		$r10,$a7
	ret	
