.text
.int 0x00100000
.int 0x00000008
	
	nop	
	
	move.l #524288, %d5
	move.l #0x0008070f, %d4
1:
	move.l %d4, (%d5)
	move.l (%d5), %d4
	move.l %d4, (%d5)
	addq.l #4, %d5
	
	cmpi.l #908288, %d5
	bne 1b

	move.l #524288, %d5
2:
	move.l 1048580,%d0
	btst #1,%d0
	jeq 2b
	
	move.l 1048576,%d4
	
	move.b %d4, (%d5)
	move.b (%d5), %d4
	move.b %d4, (%d5)
	
	addq.l #1, %d5

	cmpi.l #908288, %d5
	bne 2b




2:	stop #0x3000
	bra 2b
	nop
