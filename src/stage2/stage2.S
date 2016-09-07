.global _start
_start:
	lea __bss_start, %a0
	lea __bss_end, %a1
	
1:
	cmp %a0, %a1
	beq 2f
	move.b #0x00, (%a0)+
	bra 1b
	
2:
	jsr main

	cmp #0, %d0
	jmp 0xC
	jmp 0x10
