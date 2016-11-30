.text
.global spi_load


spi_wait_ready:
	nop
1:
	btst.b #0, 0x100807
	bne.b 1b

	rts

spi_select:
	bsr.b spi_wait_ready
	move.b %d0, 0x10080B
	nop
	rts

spi_send:
	bsr.b spi_wait_ready
	move.b %d0, 0x100803
	nop
	rts

spi_read:
	bsr.b spi_wait_ready
	move.b #0xFF, 0x100803
	bsr.b spi_wait_ready
	move.b 0x100803, %d0
	rts

spi_send_addr:
	movem.l %d0, -(%sp)
	rol.l #8, %d0
	rol.l #8, %d0
	bsr spi_send
	rol.l #8, %d0
	bsr.b spi_send
	rol.l #8, %d0
	bsr.b spi_send
	movem.l (%sp)+, %d0
	rts

spi_load_data:
	movem.l %d0/%d3/%a0, -(%sp)

	# Select slave
	move.b #1, %d0
	bsr.b spi_select
	
	# Read command
	move.b #3, %d0
	bsr.b spi_send
	
	# Address to read
	move.l %d5, %d0
	bsr.b spi_send_addr

	movea.l %d4, %a0
	subq.l #1, %d3
1:
	bsr.b spi_read
	move.b %d0, (%a0)+
	dbf %d3, 1b
	
	# Select slave
	move.b #0, %d0
	bsr.b spi_select

	movem.l (%sp)+, %d0/%d3/%a0
	rts


spi_load:
	movem.l %d1-%d6/%a0-%a2, -(%sp)

	suba.l #20, %sp
	move.l #0x7FE00, %d5
	move.l #20, %d3
	move.l %sp, %d4
	bsr spi_load_data
	

	cmpi.l #0xDEADBEEF, (%sp)+
	jne 3f
	movea.l (%sp)+, %a0
	move.l (%sp)+, %d3
	move.l (%sp)+, %d4
	move.l (%sp)+, %d5

	bsr.b spi_load_data

2:	# Success
	move.b #0x53, %d0
	move.b %d0, 0x100903
	jsr (%a0)
	bra.b 1f

3:	
	# Error
	move.b #0x44, %d0
	move.b %d0, 0x100903
	move.l %sp, %d0
1:	
	# Return
	movem (%sp)+, %a0-%a2/%d1-%d2
