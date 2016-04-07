.text
.int 0x00000000
.int 0x00000008
	
	nop

	move.l #0x5555555, %d0
	move.l #0xAAAAAA0, %a0
	move.l %d0, (%a0)

	move.l #0x00000000, %d0
	move.l #0, %a0

1:
	move.w %d0, (%a0)
	
	addq.l #2, %a0
	cmpi.l #767998, a0
	bne 1b



2:	stop #0x3000
	bra 2b
