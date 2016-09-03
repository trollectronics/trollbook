#NO_APP
	.file	"hexload.c"
	.text
	.align	2
	.globl	loadhex
	.type	loadhex, @function

.int 0x00100000
.int 0x00000008

nop

	move.l #524288, %d5
	move.l #0x03030303, %d4
1:
	move.l %d4, (%d5)
	addq.l #4, %d5
	
	cmpi.l #908288, %d5
	bne 1b
loadhex:
	move.l %fp,-(%sp)
	move.l %sp,%fp
	movem.l #16160,-(%sp)
	clr.b %d4
	clr.w %d3
	clr.l %d1
	clr.l %d5
	move.l #65536,%a0
	clr.l %d0
.L30:
	move.l %d0,%a1
	subq.l #1,%a1
	moveq #5,%d6
	cmp.l %a1,%d6
	jcs .L11
	move.w .L4(%pc,%a1.l*2),%a1
	jmp %pc@(2,%a1:w)
	.balignw 2,0x284c
	.swbeg	&6
.L4:
	.word .L3-.L4
	.word .L5-.L4
	.word .L32-.L4
	.word .L7-.L4
	.word .L35-.L4
	.word .L22-.L4
.L11:
	move.l 1048580,%d0
	btst #1,%d0
	jeq .L11
	move.l 1048576,%d0
	cmp.b #58,%d0
	jeq .L34
	cmp.b #10,%d0
	jne .L35
	clr.l %d0
	jra .L8
.L3:
	clr.l %d7
	move.b %d2,%d7
	move.l %d7,%a2
	clr.l %d1
	moveq #2,%d0
	jra .L24
.L5:
	and.w #255,%d2
	moveq #1,%d6
	cmp.l %d1,%d6
	jne .L12
	move.w %d2,%d3
	lsl.w #8,%d3
	jra .L24
.L12:
	or.w %d2,%d3
	moveq #3,%d0
	jra .L24
.L7:
	tst.b %d4
	jne .L13
	clr.l %d6
	move.w %d3,%d6
	move.l %d5,%a1
	add.l %d6,%a1
	move.b %d2,(%a1)
	addq.w #1,%d3
	jra .L14
.L13:
	cmp.b #1,%d4
	jeq .L15
	cmp.b #4,%d4
	jne .L16
	and.l #255,%d2
	moveq #1,%d7
	cmp.l %d1,%d7
	jne .L17
	move.l %d2,%d5
	moveq #24,%d6
	lsl.l %d6,%d5
	jra .L14
.L17:
	swap %d2
	clr.w %d2
	add.l %d2,%d5
	jra .L14
.L16:
	cmp.b #5,%d4
	jne .L18
	moveq #1,%d7
	cmp.l %d1,%d7
	sne %d6
	extb.l %d6
	move.l %a0,%d7
	and.l %d6,%d7
	move.l %d7,%a0
	and.l #255,%d2
	moveq #4,%d6
	sub.l %d1,%d6
	lsl.l #3,%d6
	lsl.l %d6,%d2
	add.l %d2,%a0
.L14:
	cmp.l %d1,%a2
	jne .L24
	moveq #5,%d0
	jra .L24
.L22:
	move.l 1048580,%d6
	btst #1,%d6
	jeq .L22
	move.l 1048576,%d6
	cmp.b #10,%d6
	sne %d6
	extb.l %d6
	and.l %d6,%d0
	jra .L8
.L32:
	move.b %d2,%d4
	clr.l %d1
	moveq #4,%d0
	jra .L24
.L34:
	moveq #1,%d0
.L24:
	move.l 1048580,%d2
	btst #1,%d2
	jeq .L24
	move.l 1048576,%d2
	cmp.b #57,%d2
	jls .L25
	add.b #-55,%d2
	jra .L26
.L25:
	add.b #-48,%d2
.L26:
	lsl.b #4,%d2
.L28:
	move.l 1048580,%d6
	btst #1,%d6
	jeq .L28
	move.l 1048576,%d6
	cmp.b #57,%d6
	jls .L29
	add.b #-55,%d6
	jra .L43
.L29:
	add.b #-48,%d6
.L43:
	or.b %d6,%d2
	jra .L8
.L35:
	moveq #6,%d0
.L8:
	addq.l #1,%d1
	jra .L30
.L15:
	jmp 0xddb00
.L18:
.L31:
	jra .L31
	.size	loadhex, .-loadhex
	.ident	"GCC: (GNU) 4.8.2"

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
