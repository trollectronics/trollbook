.text
.int 0x00100000
.int 0x00000008
	
	nop
	

	move.l #0x45504F4E, %d0
	clr.l %d1
	move.w %d0, -(%sp)
	move.w (%sp)+, %d1

1:
	move.l 1048580,%d0
	btst #0,%d0
	jeq 1b
	
	move.l %d1, 1048576
	
	lsr.l #8, %d1
	jne 1b
	nop	
2:
	jra 2b
	
	move.l #524288, %a0
2:
	move.l 1048580,%d0
	btst #1,%d0
	jeq 2b
	
	move.l 1048576,%d4
	
	move.b %d4, (%a0)
	
	addq.l #1, %a0

	cmpi.l #908288, a0
	bne 2b




2:	stop #0x3000
	bra 2b
	nop
