.align 2

.global _start
_start:
	move.l #0x84000000, %a7
	lea __bss_start, %a0
	lea __bss_end, %a1
	
	move.l %a1, %d0
	addq.l #3, %d0
	andi.l #0xFFFFFFFC, %d0
	move.l %d0, %a1
	
1:
	cmp.l %a0, %a1
	beq 2f
	move.l #0x00, (%a0)+
	bra 1b
	
2:
	cinva %bc
	move.l #0x80008000, %d0
	movec %d0, %cacr
	
	bra.l main
	
	cmp #0, %d0
	bne fail
success:
	bra loop
fail:
	bra loop

loop:
	bra loop
