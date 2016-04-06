.text
.int 0x00000000
.int 0x00000008
	
	move.l #0x00030005, %d0
	move.l #160000, %a0

1:
	move.w %a0, (%a0)
	
	subq.l #1, %a0
	bne 1b



	stop #0x3000
1:	bra 1b
