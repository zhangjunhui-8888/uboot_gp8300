# -*- mode: Conf; tab-width: 8 -*-
#ifndef SAFE_BOOT_SEQUENCE_H
#define SAFE_BOOT_SEQUENCE_H

#ifndef UNIMPLEMENTED_MARCH_VERSION
#  error "UNIMPLEMENTED_MARCH_VERSION must be defined in Makefile"
#endif

# initial boot code for chip1

   rseti     $r0,0

   sprsetr   $r0,CPUC
   sprsetr   $r0,PSC
   sprsetr   $r0,XEN
#if UNIMPLEMENTED_MARCH_VERSION != MARCH_CHIP1
   sprsetr   $r0,L2CCTLC
#else
   sprsetr   $r0,L2CCTL
#endif

   rseti     $r1,-1

   sprsetr   $r0,IBASE
   sprsetr   $r1,IBND
   sprsetr   $r0,DBASE
   sprsetr   $r1,DBND

   sprsetr   $r0,DM0C
   sprsetr   $r0,DM1C
#  sprsetr   $r0,DM2C
#  sprsetr   $r0,DM3C

   sprsetr   $r0,IM0C
   sprsetr   $r0,IM1C
#  sprsetr   $r0,IM2C
#  sprsetr   $r0,IM3C

   sprsetr   $r0,XPEN
   sprsetr   $r0,TIMC
   sprsetr   $r0,TCD0C
   sprsetr   $r0,0x5d    #  sprsetr   $r0,TCD1C # TCD1C name does not exist in chip2

#if UNIMPLEMENTED_MARCH_VERSION != MARCH_CHIP1
   sprsetr   $r0,IPAGE
   sprsetr   $r0,DPAGE
   sprsetr   $r0,MXB
   sprsetr   $r0,PSCM
#endif
   sprsetr   $r0,PSCK
   sprsetr   $r0,PSCH

#   sprsetr   $r0,XCLR
#   sprsetr   $r0,XSET
   sprsetr   $r0,TCD0
   sprsetr   $r0,0x5     #  sprsetr   $r0,TCD1 # TCD1c name does not exist in chip2


# ---- ADD CRITICAL BOOT INSTRUCTINS HERE
	##
# ---------------------------------------
#ifdef STATUS_RESULT_BLOCK
	taddpci	$t0,status_result_block_end
	j	$t0

#-------------- status result block  ----------------
#  should be 0x8200-0x8400, if it moves, there is a problem
.align 9

.globl status_result_block
status_result_block:
	.rept 512
	.byte 0
	.endr

.align 9
.globl status_result_block_end
status_result_block_end:
#endif
#-------------- status result block end  ----------------


#if 0 // set all sprs to zero, not just architected
   sprsetr   $r0,TIM
   sprsetr   $r0,DRV
   sprsetr   $r0,DRT
   sprsetr   $r0,DRL
   sprsetr   $r0,IRV
   sprsetr   $r0,IRT
   sprsetr   $r0,IRL
   sprsetr   $r0,THID
   sprsetr   $r0,THC
   sprsetr   $r0,XHS
   sprsetr   $r0,XLV
   sprsetr   $r0,XMS
   sprsetr   $r0,XPS
   sprsetr   $r0,TXB
   sprsetr   $r0,KXB
   sprsetr   $r0,HXB
   sprsetr   $r0,XR0
   sprsetr   $r0,XS0
   sprsetr   $r0,IXA0
   sprsetr   $r0,IXI0
   sprsetr   $r0,XR1
   sprsetr   $r0,XS1
   sprsetr   $r0,IXA1
   sprsetr   $r0,IXI1
   sprsetr   $r0,XR0H
   sprsetr   $r0,XS0H
   sprsetr   $r0,IXA0H
   sprsetr   $r0,IXI0H
   sprsetr   $r0,XR1H
   sprsetr   $r0,XS1H
   sprsetr   $r0,IXA1H
   sprsetr   $r0,IXI1H
   sprsetr   $r0,XRM
   sprsetr   $r0,XSM
   sprsetr   $r0,IXAM
   sprsetr   $r0,IXIM
   sprsetr   $r0,XRT
   sprsetr   $r0,XST
   sprsetr   $r0,XRA
   sprsetr   $r0,XSA
   sprsetr   $r0,XRH
   sprsetr   $r0,XSH
   sprsetr   $r0,DM0E
   sprsetr   $r0,DM1E
#  sprsetr   $r0,DM2E
#  sprsetr   $r0,DM3E
   sprsetr   $r0,DM0M
   sprsetr   $r0,DM1M
#  sprsetr   $r0,DM2M
#  sprsetr   $r0,DM3M
   sprsetr   $r0,IM0E
   sprsetr   $r0,IM1E
#  sprsetr   $r0,IM2E
#  sprsetr   $r0,IM3E
   sprsetr   $r0,IM0M
   sprsetr   $r0,IM1M
#  sprsetr   $r0,IM2M
#  sprsetr   $r0,IM3M

  sprsetr   $r0,IWV	# DO init, so later spurious write to IWB will not be a problem
  sprsetr   $r0,IWT
#sprsetr   $r0,IWB
#sprsetr   $r0,IRB
  sprsetr   $r0,DWV	# DO init, so later spurious write to IDB will not be a problem
  sprsetr   $r0,DWT
#sprsetr   $r0,DWB
#sprsetr   $r0,DRB

#  sprsetr   $r0,ICCTL

#else // set all sprs to zero
	rseti	$r15,0
#zeroing all of the SPR memory values ( a couple of asm tests need this )

#	sprsetr	PSC_VALUE,0x00	# psc - already set
	sprsetr	$r15,0x01
	sprsetr	$r15,0x02
	sprsetr	$r15,0x03
	sprsetr	$r15,0x04
	sprsetr	$r15,0x05
	sprsetr	$r15,0x06
	sprsetr	$r15,0x07
	sprsetr	$r15,0x08
	sprsetr	$r15,0x09
	sprsetr	$r15,0x0a
	sprsetr	$r15,0x0b
	sprsetr	$r15,0x0c
	sprsetr	$r15,0x0d
	sprsetr	$r15,0x0e
	sprsetr	$r15,0x0f

#	sprsetr	PSC_VALUE,0x10	# CPUC - already set
	sprsetr	$r15,0x11
	sprsetr	$r15,0x12
	sprsetr	$r15,0x13
	sprsetr	$r15,0x14
	sprsetr	$r15,0x15
	sprsetr	$r15,0x16
	sprsetr	$r15,0x17
	sprsetr	$r15,0x18
	sprsetr	$r15,0x19
	sprsetr	$r15,0x1a
	sprsetr	$r15,0x1b
#       sprsetr	PSC_VALUE,0x1c	# PSCK - already set
#       sprsetr	PSC_VALUE,0x1d	# PSCH - already set
	sprsetr	$r15,0x1e
	sprsetr	$r15,0x1f

	sprsetr	$r15,0x20
	sprsetr	$r15,0x21
	sprsetr	$r15,0x22
	sprsetr	$r15,0x23
	sprsetr	$r15,0x24
	sprsetr	$r15,0x25
	sprsetr	$r15,0x26
	sprsetr	$r15,0x27
	sprsetr	$r15,0x28
	sprsetr	$r15,0x29
	sprsetr	$r15,0x2a
	sprsetr	$r15,0x2b
	sprsetr	$r15,0x2c
	sprsetr	$r15,0x2d
	sprsetr	$r15,0x2e
	sprsetr	$r15,0x2f

	sprsetr	$r15,0x30
	sprsetr	$r15,0x31
	sprsetr	$r15,0x32
	sprsetr	$r15,0x33
	sprsetr	$r15,0x34
	sprsetr	$r15,0x35
	sprsetr	$r15,0x36
	sprsetr	$r15,0x37
	sprsetr	$r15,0x38
	sprsetr	$r15,0x39
	sprsetr	$r15,0x3a
	sprsetr	$r15,0x3b
	sprsetr	$r15,0x3c
	sprsetr	$r15,0x3d
	sprsetr	$r15,0x3e
	sprsetr	$r15,0x3f

	sprsetr	$r15,0x40
	sprsetr	$r15,0x41
	sprsetr	$r15,0x42
	sprsetr	$r15,0x43
	sprsetr	$r15,0x44
	sprsetr	$r15,0x45
	sprsetr	$r15,0x46
	sprsetr	$r15,0x47
	sprsetr	$r15,0x48
	sprsetr	$r15,0x49
	sprsetr	$r15,0x4a
	sprsetr	$r15,0x4b
	sprsetr	$r15,0x4c
	sprsetr	$r15,0x4d
	sprsetr	$r15,0x4e
	sprsetr	$r15,0x4f

	sprsetr	$r15,0x50
	sprsetr	$r15,0x51
	sprsetr	$r15,0x52
	sprsetr	$r15,0x53
	sprsetr	$r15,0x54
	sprsetr	$r15,0x55
	sprsetr	$r15,0x56
	sprsetr	$r15,0x57
	sprsetr	$r15,0x58
	sprsetr	$r15,0x59
	sprsetr	$r15,0x5a
	sprsetr	$r15,0x5b
	sprsetr	$r15,0x5c
	sprsetr	$r15,0x5d	 # old TCD1C already set
	sprsetr	$r15,0x5e
	sprsetr	$r15,0x5f

# set value and tag first, before DWB and IWB
	sprsetr	$r15,0x61	# IWV
	sprsetr	$r15,0x62	# IWT
	sprsetr	$r15,0x65	# DWV
	sprsetr	$r15,0x66	# DWT

	sprsetr	$r15,0x64	# DWB
	sprsetr	$r15,0x67	# DRB

	sprsetr	$r15,0x60	# IWB
	sprsetr	$r15,0x63	# IRB


	sprsetr	$r15,0x68
	sprsetr	$r15,0x69
	sprsetr	$r15,0x6a
#       sprsetr	$r15,0x6b	# IBND - already set
	sprsetr	$r15,0x6c
	sprsetr	$r15,0x6d
	sprsetr	$r15,0x6e
#       sprsetr	$r15,0x6F	# DBND - already set

	sprsetr	$r15,0x70
	sprsetr	$r15,0x71
	sprsetr	$r15,0x72
	sprsetr	$r15,0x73
	sprsetr	$r15,0x74
	sprsetr	$r15,0x75
	sprsetr	$r15,0x76
	sprsetr	$r15,0x77
	sprsetr	$r15,0x78
	sprsetr	$r15,0x79
	sprsetr	$r15,0x7a
	sprsetr	$r15,0x7b
	sprsetr	$r15,0x7c
	sprsetr	$r15,0x7d
	sprsetr	$r15,0x7e
	sprsetr	$r15,0x7f

#endif // set all sprs to zero

#ifdef SAFE_BOOT_LOW_POWER_IO
#define APB_POWER_IO_MODE_PTR  0xf000e004
#define APB_POWER_IO_LOW_POWER 0x801        
_safe_boot_low_power_io:
#if 1
	ldri    $r15, APB_POWER_IO_MODE_PTR
	aaddri  $a1, $r15, 0
	ldri    $r15, APB_POWER_IO_LOW_POWER
	stw     $r15, $a1
#else
   aaddpci	$a0,apb_0xf000e004_ptr
   ldul		$r15,$a0,8
   lda		$a1,$a0
   stw		$r15,$a1
#endif
#endif

# in case DTLB gets turned on (VHDL uninitialized)
	rseti		$r8,1
	shli		$r8,$r8,DRV_OFFSE_ra  	# 15
	aaddri		$a0,$r8,0
	aaddri		$a1,$r8,0
	aaddri		$a2,$r8,0
	aaddri		$a3,$r8,0
	aaddri		$a4,$r8,0
	aaddri		$a5,$r8,0
	aaddri		$a6,$r8,0
	aaddri		$a7,$r8,0

#ifdef SAFE_BOOT_SET_TXB_HXB_INTS
_safe_boot_set_TXB_HXB:

   aaddpci	$a4,TXBptr
   ldl		$r8,$a4
   sprsetr 	$r8,TXB

#if UNIMPLEMENTED_MARCH_VERSION != MARCH_CHIP1
   rseti	$r15,0x7f
   sprsetr	$r15,TRAPM	# allow 128 traps
#endif

   aaddpci	$a4,ix0ptr # was IX0Bp	# HYPERVISOR set to ix0
   ldl		$r8,$a4
   sprsetr 	$r8,HXB

#if SET_CACHE_MODE & 0x1
   rseti   	$r15,-1                      # 0xffff...fff
#else
   rseti   	$r15,1                      # 0xffff...fff
   shli		$r15,$r15,PSC_OFFSE_ooen
   xori		$r15,$r15,-1
#endif
   sprsetr 	$r15,CPUC

_first_status_write:
	aaddpci	$a7,status_result_block_end
	aaddai	$a7,$a7,-8
	rseti	$r8,0x44
	shli	$r8,$r8,56
	ori	$r8,$r8,0x1
	stl	$r8,$a7
	ldl	$r8,$a7

_safe_boot_enable_ints:

# interrupt lvl 0 is on for this type of run
   rseti	$r15,1
   shli		$r15,$r15,PSC_OFFSE_interrupt_0
   or		PSC_VALUE,PSC_VALUE,$r15
   rseti	$r15,1
   shli		$r15,$r15,PSC_OFFSE_interrupt_mc
   or		PSC_VALUE,PSC_VALUE,$r15
   sprsetr	PSC_VALUE,PSC

#endif
_safe_boot_finis:

#endif // #ifndef SAFE_BOOT_SEQUENCE_H
