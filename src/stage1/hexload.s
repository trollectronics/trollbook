.text
.align	2

.int 0x00100000
.int 0x00000008

	move.l #0x03030303, %d4

fill:
	move.l #524288, %d5
1:
	move.l %d4, (%d5)
	addq.l #4, %d5
	
	cmpi.l #908288, %d5
	bne.b 1b

	move.l #0x00000021, %d1
1:
	move.l 0x100904, %d0
	btst.b #0, %d0
	beq.b 1b
	move.l %d1, 0x100900

bsr loadhex



.text
.align 2
.globl loadhex

## Calling conventions:
	## All registers are callee-save, with exception for the return register d0
	
# Internal register map:
# a0 = next write address
# a1 = entry point
# d1 = end of file flag

loadhex:
	movem.l %d1-%d3/%a0-%a1, -(%sp)
	eor.l %d1, %d1
	move.l %d1, %a0
	move.l #-1, %a1
_loadhex_line:
	bsr get_byte
	cmp.b #58, %d0
	jne _skip
## Load byte count
	bsr.b decode_pair
	move.b %d0, %d2
## Load low address
	bsr.b decode_word
	move.l %a0, %d3
	move.w %d0, %d3
	move.l %d3, %a0
## Load record type
	bsr.b decode_pair

	# tst compares with 0
	tst.b %d0
	jeq _read_data
	cmp.b #1, %d0
	jeq _end_of_file
	cmp.b #4, %d0
	jeq _upper_address
	cmp.b #5, %d0
	jeq _entry_point
_skip:
	bsr.b get_byte
	cmp.b #10, %d0
	jne _skip
	tst %d1
	jeq _loadhex_line
_done:
	eor.l %d0, %d0
	move.l #-1, %d1
	cmp %a1, %d1
	jeq _returned
	jsr (%a1)
_returned:
## Now return back to caller of hexload
	movem.l (%sp)+, %d1-%d3/%a0-%a1
	rts

_read_data:
	tst.b %d2
	jeq _skip
	bsr.b decode_pair
	sub.b #1, %d2
	move.b %d0, (%a0)+
	jra _read_data
_upper_address:
	bsr.b decode_word
	swap %d0
	move.w %a0, %d3
	move.w %d3, %d0
	move.l %d0, %a0
	jra _skip
_entry_point:
	bsr.b decode_word
	swap %d0
	move.l %d0, %d3
	bsr.b decode_word
	move.w %d0, %d3
	move.l %d3, %a1
	jra _skip
_end_of_file:
	not.b %d1
	jra _skip


decode_word:
	move.l %d2, -(%sp)
	
	bsr.b decode_pair
	move.l %d0, %d2
	bsr.b decode_pair
	lsl.w #8, %d2
	or.w %d2, %d0
	
	move.l (%sp)+, %d2
	rts

decode_pair:
	move.l %d2, -(%sp)

	bsr.b decode_byte
	move.l %d0, %d2
	bsr.b decode_byte
	lsl.b #4, %d2
	or.b %d2, %d0

	move.l (%sp)+, %d2
	rts

decode_byte:
	bsr.b get_byte
	cmp.b #57, %d0
	jle _decode_byte_lower
	add.b #-7, %d0
_decode_byte_lower:
	add.b #-48, %d0
	rts

get_byte:
	move.l 0x100904, %d0
	btst #1, %d0
	jeq get_byte
	move.l 0x100900, %d0
	and.l #0xFF, %d0
	rts


.include "spi_load_new.s"
