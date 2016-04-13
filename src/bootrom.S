.text
.int 0x00100000
.int 0x00000008
	
	nop
	

	
	move.l #524288, %a0
2:
	move.l 1048580,%d0
	btst #1,%d0
	jeq 2b
	
	move.l 1048576,%d0
	
	move.b %d0, (%a0)
	
	addq.l #1, %a0
	
	cmpi.l #908288, a0
	bne 2b




2:	stop #0x3000
	bra 2b
	nop
