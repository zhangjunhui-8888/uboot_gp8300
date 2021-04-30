#@version $Id: strlen.S 3578 2016-04-11 14:26:18Z phurtley $

# int strlen (const char *src);
# Return the length of src.

#define TEMP1 $r6

	.text
	.align  2	
	.globl  strlen

strlen:

  aaddri	$a5,$r8, 0
  rseta		TEMP1, $a5
  taddpci       $t0, CALC_LEN_ALIGNED
  taddpci	$t1, RETURN

# Make the starting address aligned on a long word
# boundary, load the long word, skip the unwanted 
# bytes and check for NULL char

  rseta		$r14, $a5
  andi		$r8, $r14, 0x7
  subfi		$r11, $r8, 8
  subf		$r13, $r8, $r14
  jci_eq	$t0, $r8, 0
  shli		$r12, $r8, 3
  aaddri	$a5, $r13, 0
  ldul		$r14, $a5, 8
  shr		$r14, $r14, $r12
  countnzb	$r8, $r14, $r14 
  jc_ne		$t1, $r11, $r8

# Loop on an aligned boundary and check for NULL char
  
CALC_LEN_ALIGNED:
  ldul          $r14, $a5, 8
  countnzb      $r8, $r14, $r14
  jci_eq        $t0, $r8, 8

# Calculate the final length

CALC_FINAL_LEN:
  aaddai        $a5, $a5, -8
  aaddri	$a6, TEMP1, 0
  rsubaa_b	$r14, $a5, $a6
  add		$r8, $r8, $r14

RETURN:
  ret
