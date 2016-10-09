.text
.globl spi_load


# %a0 = buffer address
# %d0 = flash data pointer
# %d1 = byte count
spi_load_data:
	move.l %d1, -(%sp)
	move.l %a1, -(%sp)
	move.l %a0, %a1
	
	move.l %d0, -(%sp)
	move.b #1, %d0
	bsr.b spi_set_slave
	
	move.b #0x3, %d0
	bsr.b spi_send_recv
	move.l (%sp)+, %d0
	
	move.l %d1, -(%sp)
	move.l %d0, %d1
	rol.l #8, %d1
	rol.l #8, %d1
	move.l %d1, %d0
	bsr.b spi_send_recv
	rol.l #8, %d1
	move.l %d1, %d0
	bsr.b spi_send_recv
	rol.l #8, %d1
	move.l %d1, %d0
	bsr.b spi_send_recv
	move.l (%sp)+, %d1

_spi_recv_loop:
	tst.l %d1
	beq _spi_load_done
	bsr.b spi_send_recv
	#move.b %d0, (%a1)+
	subq.l #1, %d1
	bra.b _spi_recv_loop
_spi_load_done:
	move.b #0, %d0
	bsr.b spi_set_slave
	move.l (%sp)+, %a1
	move.l (%sp)+, %d1
	rts

spi_read_long:
	move.l %d1, -(%sp)
	move.l %a0, -(%sp)
	
	subq.l #4, %sp
	move.l %sp, %a0
	move.l #4, %d1
	bsr.b spi_load_data
	
	move.b (%a0)+, %d0
	lsl.l #8, %d0
	or.b (%a0)+, %d0
	lsl.l #8, %d0
	or.b (%a0)+, %d0
	lsl.l #8, %d0
	or.b (%a0)+, %d0
	
	addq.l #4, %sp
	
	move.l (%sp)+, %a0
	move.l (%sp)+, %d1
	rts

spi_set_slave:
	move.l %d0, 0x100808
	nop
	rts

spi_send_recv:
	move.l %d1, -(%sp)
	
_spi_send_recv_wait_avail:
	nop
	move.l 0x100804, %d1
	tst.b %d1
	jne _spi_send_recv_wait_avail
	nop
	move.l %d0, 0x100800
_spi_send_recv_wait_done:
	nop
	move.l 0x100804, %d1
	tst.b %d1
	jne _spi_send_recv_wait_done
	nop
	move.l 0x100800, %d0

	move.l (%sp)+, %d1
	rts


spi_load:
	move.l %d1, -(%sp)
	move.l %d2, -(%sp)
	move.l %d3, -(%sp)
	move.l %a0, -(%sp)

	move.l #0, %d0
	bsr.b spi_read_long
	
	cmp.l #0xDEADBEEF, %d0
	jeq _spi_fail
	
	add.b #4, %d1
	bsr.b spi_read_long
	move.l %d0, %d2
	add.b #4, %d1
	bsr.b spi_read_long
	move.l %d0, %d3
	add.b #4, %d1
	bsr spi_read_long
	move.l %d0, %a0
	add.b #4, %d1
	exg %d1, %d3
	move.l %d2, %d0
	bsr spi_load_data
	bsr spi_read_long
	move.l %d0, %a0
	jsr (%a0)
_spi_return:
	move.l (%sp)+, %a0 
	move.l (%sp)+, %d3
	move.l (%sp)+, %d2
	move.l (%sp)+, %d1
	rts
_spi_fail:
	move.l #0, %d0
	jra _spi_return
