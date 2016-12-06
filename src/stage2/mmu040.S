.text

text_dttr:
.asciz "set dttr 0x%x\n"
text_ittr:
.asciz "set ittr 0x%x\n"

.align 2

.global mmu_int_stub
mmu_int_stub:
	jsr.l mmu_bus_error
	rte

.global mmu_test_read
mmu_test_read:
	link %fp, #0
	move.l 8(%fp), %a0
	
	move.l #0x6, %d0
	movec.l %d0, %sfc
	movec.l %d0, %dfc
	
	ptestr (%a0)
	
	movec.l %mmusr, %d0
	unlk %fp
	rts

| .global mmu_get_physical
| mmu_get_physical:
| 	link %fp, #-4
| 	movec %tc, %d0
| 	ori.l #0x80000000, %d0
| 	movec %d0, %tc
| 	mov.l 8(%fp), %a1
| 	
| 	move.l #6, %d0
| 	movec %d0, %dfc
| 	ptestr #6, (%a1), #7, %a0
| 	
| 	andi.l #0x7FFFFFFF, %d0
| 	movec %d0, %tc
| 	mov.l (%a0), %d0
| 	andi.l #0xFFFFFF00, %d0
| 	mov.l 8(%fp), %d1
| 	andi.l #0xFFF, %d1
| 	or.l %d1, %d0
| 	unlk %fp
| 	rts

.global mmu040_zero_4k
mmu040_zero_4k:
	link %fp, #0
	move.l 8(%fp), %a0
	move.l %a0, %d0
	addi.l #4096, %d0
	clr.l %d1
	
1:
	move.l %d1, (%a0)+
	move.l %d1, (%a0)+
	move.l %d1, (%a0)+
	move.l %d1, (%a0)+
	
	cmp %a0, %d0
	bne 1b
	
	unlk %fp
	rts

.global mmu_enable
mmu_enable:
	cpusha %dc
	move.l #0x00000000, %d0
	movec %d0, %cacr
	
	#movec %tc, %d0
	move.w #0x8000, %d0
	pflusha
	cinva %bc
	movec %d0, %tc
	
	rts

.global mmu_disable
mmu_disable:
	move.w #0x0000, %d0
	movec %d0, %tc
	
	rts

.global mmu_enable_and_jump
mmu_enable_and_jump:
	link %fp, #0
	
	cpusha %dc
	move.l #0x00000000, %d0
	movec %d0, %cacr
	
	#movec %tc, %d0
	move.w #0x8000, %d0
	pflusha
	cinva %bc
	movec %d0, %tc
	
	move.l 8(%fp), %a0
	move.l #0x0, %sp
	move.l 16(%fp), -(%sp)
	move.l 12(%fp), -(%sp)
	move.l #0, -(%sp)
	
	jmp (%a0)

.global mmu_set_tc
mmu_set_tc:
	link %fp, #0
	move.l 8(%fp), %a0
	move.l (%a0), %d0
	movec %d0, %tc
	unlk %fp
	rts

.global mmu_get_tc
mmu_get_tc:
	link %fp, #0
	move.l 8(%fp), %a0
	movec %tc, %d0
	move.l %d0, (%a0)
	unlk %fp
	rts

.global mmu_set_srp
mmu_set_srp:
	link %fp, #0
	move.l 8(%fp), %a0
	move.l (%a0), %d0
	movec %d0, %srp
	unlk %fp
	rts

.global mmu_get_srp
mmu_get_srp:
	link %fp, #0
	move.l 8(%fp), %a0
	movec %srp, %d0
	move.l %d0, (%a0)
	unlk %fp
	rts

.global mmu_set_urp
mmu_set_urp:
	link %fp, #0
	move.l 8(%fp), %a0
	move.l (%a0), %d0
	movec %d0, %urp
	unlk %fp
	rts

.global mmu_get_urp
mmu_get_urp:
	link %fp, #0
	move.l 8(%fp), %a0
	movec %urp, %d0
	move.l %d0, (%a0)
	unlk %fp
	rts

.global lol_ttr
lol_ttr:
	move.l #0x0000A020, %d0
	movec.l %d0, %itt0
	movec.l %d0, %dtt0
	rts

.global mmu_set_ittr
mmu_set_ittr:
	link %fp, #0
	move.l 8(%fp), %a0
	move.l (%a0), %d0
	
	move.l 12(%fp), %d1
	btst.l #0, %d1
	jne 1f
	
	movec.l %d0, %itt0
	unlk %fp
	rts
1:
	movec.l %d0, %itt1
	unlk %fp
	rts

.global mmu_get_ittr
mmu_get_ittr:
	link %fp, #0
	move.l 8(%fp), %a0
	
	move.l 12(%fp), %d1
	btst.l #0, %d1
	jne 1f
	
	movec.l %itt0, %d0
	move.l %d0, (%a0)
	unlk %fp
	rts
1:
	movec.l %itt1, %d0
	move.l %d0, (%a0)
	unlk %fp
	rts

.global mmu_set_dttr
mmu_set_dttr:
	link %fp, #0
	move.l 8(%fp), %a0
	move.l (%a0), %d0
	
	move.l 12(%fp), %d1
	btst.l #0, %d1
	jne 1f

	movec.l %d0, %dtt0
	unlk %fp
	rts
1:
	movec.l %d0, %dtt1
	unlk %fp
	rts

.global mmu_get_dttr
mmu_get_dttr:
	link %fp, #0
	move.l 8(%fp), %a0
	
	move.l 12(%fp), %d1
	btst.l #0, %d1
	jne 1f
	
	movec.l %dtt0, %d0
	move.l %d0, (%a0)
	unlk %fp
	rts
1:
	movec.l %dtt1, %d0
	move.l %d0, (%a0)
	unlk %fp
	rts
