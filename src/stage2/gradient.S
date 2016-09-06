	move.l #524288, %d5
	move.l #0x0008070f, %d4
1:
	move.l %d4, (%d5)
	addq.l #4, %d5
	
	cmpi.l #908288, %d5
	bne 1b
	
2:	#stop #0x3000
	bra 2b
	nop
