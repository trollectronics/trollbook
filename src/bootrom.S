.text
.int 0x00100000
.int 0x00000008
	
	nop
	move.l #524288, %a0

#1:
#	move.l #0, (%a0)
#	
#	addq.l #4, %a0
#	cmpi.l #908288, a0
#	bne 1b
	
#	move.l #524288, %a0
1:
	move.l 1048580,%d0
	btst #1,%d0
	jeq 1b
	
	move.l 1048576,%d0
	
	move.b %d0, (%a0)
	
	addq.l #1, %a0
	cmpi.l #908288, a0
	bne 1b

2:	
	bra 2b
	nop
