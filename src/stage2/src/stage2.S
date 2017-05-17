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
	move.l #0x80008000, %d0
	movec %d0, %cacr
	
	move.l #0x0000A040, %d0
	movec.l %d0, %dtt0
	
	move.l #0x0000A000, %d0
	movec.l %d0, %itt0
	
	jsr main
	
	cmp #0, %d0
	bne fail

fail:
	move.l #0x04040404, %d4
	bra fill
	
success:
	move.l #0x02020202, %d4
	bra fill

fill:
	move.l #524288, %d5
1:
	move.l %d4, (%d5)
	addq.l #4, %d5
	
	cmpi.l #908288, %d5
	bne 1b
2:
	bra 2b

.align 2
.global reboot
reboot:
	move.l #0x00000000, %d0
	movec %d0, %cacr
	lea 0x100000, %sp
	jmp 0x8
	
