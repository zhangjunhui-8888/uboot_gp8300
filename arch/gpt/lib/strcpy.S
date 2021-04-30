#@version $Id: strcpy.S 3555 2016-04-07 13:31:09Z phurtley $

# char *strcpy (char *tgt, const char *src);
# Copy src to tgt. Return tgt.

#define TEMP1 $r6

	.text
	.align  2	
	.globl  strcpy

strcpy:

# setup jump targets
  taddpci       $t0, LOOP
  taddpci 	$t1, COPY_BYTES_LOOP

# dst pointer in r10
  rseta		$r10, $a5

# Check if src and dst are aligned
  rseta		$r14, $a6
  or		$r15, $r10, $r14
  andi		$r15, $r15, 0x7
  jci_ne	$t1, $r15, 0

# Process first aligned word
  ldul		$r14, $a6, 8
  countnzb	TEMP1, $r14, $r14
  jci_eq	$t0, TEMP1, 8
  aaddai	$a6, $a6, -8
  j		$t1

LOOP:
# store src to dst and update dst
  stul		$r14, $a5, 8

# load long src in r14 and update src
  ldul		$r14, $a6, 8

  countnzb	TEMP1, $r14, $r14

# loop if no NULL char is encountered
  jci_eq	$t0, TEMP1, 8
  aaddai	$a6, $a6, -8

COPY_BYTES_LOOP:
  ldub		$r15, $a6, 1
  stub		$r15, $a5, 1
  jci_ne	$t1, $r15, 0

  addi		$r8, $r10, 0
  ret
