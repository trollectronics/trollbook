.text
.align	2
.globl	loadhex

.int 0x00100000
.int 0x00000008

nop

loadhex:
	jsr get_byte
	cmp.b #58, %d7
	jne .skip
/* Load byte count */
	jsr decode_pair
	move.b %d7, %d4
/* Load lower address */
	jsr decode_word
	move.w %d7, %a0
/* Load record type */
	jsr decode_pair
	move.b %d7, %d0
/* Load decide record type */
	cmp.b #0, %d0
	jeq .read_data
	cmp.b #1, %d0
	jeq .done
	cmp.b #4, %d0
	jeq .upper_address
	cmp.b #5, %d0
	jeq .start_address
	jra .skip

.read_data:
	cmp.b #0, %d0
	jeq .skip
	jsr decode_pair
	sub.b #-1, %d0
	move.b %d0, (%a0)+
	jra .read_data

.upper_address:
	jsr decode_word
	swap %d7
	move.l %a0, %d6
	move.w %d6, %d7
	move.l %d7, %a0
	jra .skip

.start_address:
	jsr decode_word
	move %d7, %d3
	jsr decode_word
	swap %d3
	move.w %d7, %d3
	move.l %d3, %a1
	jra .skip

.skip:
	jsr get_byte
	cmp.b #10, %d7
	jeq loadhex
	jra .skip
.done:
	move.l %a1, -(%sp)
	rts
	

decode_word:
	jsr decode_pair
	move %d5, %d7
	jsr decode_pair
	lsl.w #8, %d5
	or.w %d5, %d7
	rts

decode_pair:
	jsr decode_byte
	move.l %d7, %d6
	jsr decode_byte
	lsl.b #4, %d6
	or.b %d6, %d7
	rts


decode_byte:
	jsr get_byte
	cmp.b #57, %d7
	jle .decode_byte_lower
	add.b #-7, %d7
.decode_byte_lower:
	add.b #-48, %d7
	rts


get_byte:
	move.l 1048580, %d7
	btst #1, %d7
	jeq get_byte
	move.l 1048576, %d7
	rts

nop
