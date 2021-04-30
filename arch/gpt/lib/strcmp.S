# @version $Id: strcmp.S 3578 2016-04-11 14:26:18Z phurtley $

# int strcmp(const char *left, const char *right);
# Compare two strings using unsigned comparison.

	.text
	.align  	2	
	.globl  	strcmp

strcmp:
	aaddri		$a5,$r8,0
	aaddri		$a6,$r9,0
	taddpci		$t0,align_right_setup
	rseta		$r11,$a5
	andi		$r11,$r11,7
	rseta		$r12,$a6
	andi		$r12,$r12,7
	jcs_lt		$t0,$r12,$r11

# Compare bytes till left is aligned on a long word boundary.
align_left_setup:
	taddpci		$t0,check_right_alignment
	jci_eq		$t0,$r11,0
	taddpci		$t0,char_compare
	taddpci		$t1,align_left_loop
	subfi		$r7,$r11,8

align_left_loop:
	ldub		$r8,$a5,1
	ldub		$r9,$a6,1
	jc_ne		$t0,$r8,$r9
	jci_eq		$t0,$r8,0
	jdecnz		$t1,$r7,$r7

# Left is aligned on a long word boundary.
# Check right's alignment.
check_right_alignment:

	taddpci		$t0,aligned_loop_setup
	jc_eq		$t0,$r11,$r12

# Compare long words.
misaligned_right_setup:
	taddpci		$t0,long_compare
	taddpci		$t1,misaligned_right_loop
	rseta		$r8,$a6
	andi		$r8,$r8,-8
	aaddri		$a6,$r8,0
	ldul		$r10,$a6,8
	subf		$r11,$r11,$r12
	shli		$r11,$r11,3
	subfi		$r12,$r11,64

misaligned_right_loop:
	ldul		$r8,$a5,8
	addi		$r9,$r10,0
	ldul		$r10,$a6,8
	shr		$r9,$r9,$r11
	shl		$r13,$r10,$r12
	or		$r9,$r9,$r13
	countnzb	$r6,$r8,$r9
	jci_ne		$t0,$r6,8
	jc_eq		$t1,$r8,$r9

	taddpci		$t0,less_than
	swap		$r8,$r8
	swap		$r9,$r9
	jcu_lt		$t0,$r8,$r9
	rseti		$r8,1
	ret

# Compare bytes till right is aligned on a long word boundary.
align_right_setup:
	taddpci		$t0,misaligned_left_setup
	jci_eq		$t0,$r12,0
	taddpci		$t0,char_compare
	taddpci		$t1,align_right_loop
	subfi		$r7,$r12,8

align_right_loop:
	ldub		$r8,$a5,1
	ldub		$r9,$a6,1
	jc_ne		$t0,$r8,$r9
	jci_eq		$t0,$r8,0
	jdecnz		$t1,$r7,$r7

# Right is aligned on a long word boundary.
# Compare long words.
misaligned_left_setup:
	taddpci		$t0,long_compare
	taddpci		$t1,misaligned_left_loop
	rseta		$r9,$a5
	andi		$r9,$r9,-8
	aaddri		$a5,$r9,0
	ldul		$r10,$a5,8
	subf		$r11,$r12,$r11
	shli		$r11,$r11,3
	subfi		$r12,$r11,64

misaligned_left_loop:
	addi		$r8,$r10,0
	ldul		$r9,$a6,8
	ldul		$r10,$a5,8
	shr		$r8,$r8,$r11
	shl		$r13,$r10,$r12
	or		$r8,$r8,$r13
	countnzb	$r6,$r8,$r9
	jci_ne		$t0,$r6,8
	jc_eq		$t1,$r8,$r9

	taddpci		$t0,less_than
	swap		$r8,$r8
	swap		$r9,$r9
	jcu_lt		$t0,$r8,$r9
	rseti		$r8,1
	ret

# Left and right are both aligned.
# Compare long words.
aligned_loop_setup:
	taddpci		$t0,long_compare
	taddpci		$t1,aligned_loop

aligned_loop:
	ldul		$r8,$a5,8
	ldul		$r9,$a6,8
	countnzb	$r6,$r8,$r9
	jci_ne		$t0,$r6,8
	jc_eq		$t1,$r8,$r9

	taddpci		$t0,less_than
	swap		$r8,$r8
	swap		$r9,$r9
	jcu_lt		$t0,$r8,$r9
	rseti		$r8,1
	ret

# Return the result of comparing two aligned long words.
# A terminating null has been found, and $r6 contains the countnzb value.
long_compare:
	taddpci		$t0,less_than
	taddpci		$t1,equal
	shli		$r6,$r6,3
	subfi		$r6,$r6,56
	shl		$r8,$r8,$r6
	shl		$r9,$r9,$r6
	swap		$r8,$r8
	swap		$r9,$r9
	jcu_lt		$t0,$r8,$r9
	jc_eq		$t1,$r8,$r9
	rseti		$r8,1
	ret

equal:
	rseti		$r8,0
	ret

less_than:
	rseti		$r8,-1
	ret

# Return the result of comparing a single byte
char_compare:
	subf		$r8,$r9,$r8
	ret
