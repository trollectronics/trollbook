	move.l #524288, %d5
	move.l #0x0008070f, %d4
1:
	move.l %d4, (%d5)
	addq.l #4, %d5
	
	cmpi.l #908288, %d5
	bne 1b
	
2:
	jsr uart_recv
	lea hello, %a0
	jsr uart_send_string
	bra 2b

uart_send_string:
1:
	move.b (%a0)+, %d0
	beq 2f
	jsr uart_send
	bra 1b
2:
	rts

uart_send:
1:
	move.l 1048580,%d1
	btst #0,%d1
	beq 1b
	
	move.l %d0, 1048576
	
	rts

uart_recv:
1:
	move.l 1048580,%d1
	btst #1,%d1
	beq 1b
	
	move.l 1048576, %d0
	
	rts

hello: .asciz "Hello, world\r\n"
