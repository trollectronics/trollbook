.global _start
_start:
	lea __bss_start, %a0
	lea __bss_end, %a1
	
1:
	cmp.l %a0, %a1
	beq 2f
	move.b #0x00, (%a0)+
	bra 1b
	
2:
	cinva %bc
	move.l #0x00008000, %d0
	movec %d0, %cacr
	
	jsr main
	
	cmp #0, %d0
	bne fail
success:
	jmp 0xC
fail:
	jmp 0x10


.align 2
.global reboot
reboot:
	lea 0x100000, %sp
	jmp 0x8
	
