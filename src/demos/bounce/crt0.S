.align 2

.global _start
_start:
|	lea __bss_start, %a0
|	lea __bss_end, %a1
|	
|1:
|	cmp.l %a0, %a1
|	beq 2f
|	move.b #0x00, (%a0)+
|	bra 1b
	
2:
	cinva %bc
	move.l #0x80008000, %d0
	movec %d0, %cacr
	
	bra main
	
	cmp #0, %d0
	bne fail
success:
	jmp 0xC
fail:
	jmp 0x10
