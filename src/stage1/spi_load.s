.text
.globl spi_load_data

load_spi:


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

	rol.l #8, %d0
	rol.l #8, %d0
	bsr.b spi_send_recv
	rol.l #8, %d0
	bsr.b spi_send_recv
	rol.l #8, %d0
	bsr.b spi_send_recv

_spi_recv_loop:
	tst.l %d1
	jeq _done
	bsr.b spi_send_recv
	move.b %d0, (%a1)+
	jra spi_send_recv
_done:
	move.b #0, %d0
	bsr.b spi_set_slave
	move.l (%sp)+, %a1
	move.l (%sp)+, %d1
	rts

spi_read_long:
	move.l %d1, -(%sp)
	move.l %a1, -(%sp)
	eor.l %d0, %d0
	move.l %sp, %a1
	move.l #4, %d1
	bsr.b spi_load_data
	move.b (%a1)+, %d0
	lsl.b #8, %d0
	move.b (%a1)+, %d0
	lsl.b #8, %d0
	move.b (%a1)+, %d0
	lsl.b #8, %d0
	move.b (%a1)+, %d0

	move.l %a1, -(%sp)
	move.l %d1, -(%sp)
	rts

spi_set_slave:
	move.l %d0, 0x200008
	nop
	rts

spi_send_recv:
	move.l %d1, -(%sp)
	
_spi_send_recv_wait_avail:
	nop
	move.l 0x200004, %d1
	tst.b %d1
	jne _spi_send_recv_wait_avail
	nop
	move.l %d0, 0x200000
	nop
_spi_send_recv_wait_done:
	nop
	move.l 0x200004, %d1
	tst.b %d1
	jne _spi_send_recv_wait_avail
	nop
	move.l 0x200000, %d0

	move.l (%sp)+, %d1
	rts


spi_load:
	move.l %d1, -(%sp)
	move.l %d2, -(%sp)
	move.l %d3, -(%sp)
	move.l %a0, -(%sp)

	move.l #0, %d1
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
	bsr.b spi_read_long
	move.l %d0, %a0
	add.b #4, %d1
	exg %d1, %d3
	move.l %d2, %d0
	bsr spi_load_data
	bsr spi_read_long
	move.l %d0, %a0
	jsr (%a0)
_spi_return:
	move.l %a0, -(%sp)
	move.l %d3, -(%sp)
	move.l %d2, -(%sp)
	move.l %a1, -(%sp)
	rts
_spi_fail:
	move.l #0, %d0
	jra _spi_return
