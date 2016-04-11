.text
.int 0x00000400
.int 0x00000008
	
	nop
	
	move.l #A5A5A5A5, %d0
	move.l #0x100000, %a0
	
#	move.l %d0, (%a0)

	move.l #0x00000000, %d0
	move.l #524288, %a0
	
1:
	move.l %d0, (%a0)
	
	addq.l #4, %a0
	cmpi.l #908288, a0
	bne 1b



2:	
	bra 2b
