.align 2
.global interrupt_isr_trampoline
interrupt_isr_trampoline:
	move.l %a0, -(%sp)
	move.l %a1, -(%sp)
	move.l %d0, -(%sp)
	move.l %d1, -(%sp)
	
	link %fp, #0
	
	move.w 26(%fp), %d0
	andi.l #0x0FFF, %d0
	lsr.l #2, %d0
	move.l %d0, -(%sp)
	jsr.l interrupt_isr_stage2
	adda.l #4, %sp
	
	unlk %fp
	
	move.l (%sp)+, %d1
	move.l (%sp)+, %d0
	move.l (%sp)+, %a1
	move.l (%sp)+, %a0
	rte

.align 2
.global interrupt_isr_unhandled
interrupt_isr_unhandled:
	rte

.align 2
.global interrupt_move_vector
interrupt_move_vector:
	link %fp, #0
	move.l 8(%fp), %a0
	move.l %a0, %d1
	
	movec %d1, %vbr
	unlk %fp
	rts

.align 2
.global interrupt_global_enable
interrupt_global_enable:
	and.w #0xF8FF, %sr
	#mov.w #0x2000, %sr
	rts

.align 2
.global interrupt_global_disable
interrupt_global_disable:
	or.w #0x0700, %sr
	rts
