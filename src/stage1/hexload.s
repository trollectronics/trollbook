.text
.align	2

.int 0x00100000
.int 0x00000008

	bra start

romfunc:
	#0xC
	bra success
	#0x10
	bra fail

start:
	move.l #524288, %d5
	move.l #0x03030303, %d4
1:
	move.l %d4, (%d5)
	addq.l #4, %d5
	
	cmpi.l #908288, %d5
	bne 1b
	
	bra loadhex


#NO_APP
	.file	"hexload.c"
	.text
	.align	2
	.globl	fetch_byte
	.type	fetch_byte, @function
fetch_byte:
	move.l %fp,-(%sp)
	move.l %sp,%fp
.L3:
	move.l 1048580,%d0
	btst #1,%d0
	jeq .L3
	move.l 1048576,%d0
	unlk %fp
	rts
	.size	fetch_byte, .-fetch_byte
	.align	2
	.globl	loadhex
	.type	loadhex, @function
loadhex:
	link.w %fp,#-4
	movem.l #16188,-(%sp)
	sub.l %a4,%a4
	clr.b %d7
	clr.w %d6
	clr.l %d4
	lea 0.w,%a2
	moveq #1,%d5
	swap %d5
	clr.l %d3
	lea fetch_byte,%a3
.L26:
	move.l %d3,%d0
	subq.l #1,%d0
	moveq #5,%d1
	cmp.l %d0,%d1
	jcs .L6
	move.w .L8(%pc,%d0.l*2),%d0
	jmp %pc@(2,%d0:w)
	.balignw 2,0x284c
	.swbeg	&6
.L8:
	.word .L7-.L8
	.word .L9-.L8
	.word .L28-.L8
	.word .L11-.L8
	.word .L39-.L8
	.word .L13-.L8
.L6:
	jsr (%a3)
	cmp.b #58,%d0
	jeq .L30
	cmp.b #10,%d0
	jne .L39
	jra .L41
.L7:
	clr.l %d0
	move.b %d2,%d0
	move.l %d0,%a5
	clr.l %d4
	moveq #2,%d3
	jra .L10
.L9:
	and.w #255,%d2
	moveq #1,%d1
	cmp.l %d4,%d1
	jne .L14
	move.w %d2,%d6
	lsl.w #8,%d6
	jra .L10
.L14:
	or.w %d2,%d6
	moveq #3,%d3
	jra .L10
.L11:
	tst.b %d7
	jne .L15
	clr.l %d0
	move.w %d6,%d0
	lea (%a2,%d0.l),%a0
	move.b %d2,(%a0)
	addq.w #1,%d6
	jra .L16
.L15:
	cmp.b #1,%d7
	jeq .L32
	cmp.b #4,%d7
	jne .L17
	and.l #255,%d2
	moveq #1,%d0
	cmp.l %d4,%d0
	jne .L18
	moveq #24,%d1
	lsl.l %d1,%d2
	move.l %d2,%a2
	jra .L16
.L18:
	swap %d2
	clr.w %d2
	add.l %d2,%a2
	jra .L16
.L17:
	cmp.b #5,%d7
	jne .L19
	moveq #1,%d0
	cmp.l %d4,%d0
	sne %d0
	extb.l %d0
	and.l %d0,%d5
	and.l #255,%d2
	moveq #4,%d0
	sub.l %d4,%d0
	lsl.l #3,%d0
	lsl.l %d0,%d2
	add.l %d2,%d5
.L16:
	cmp.l %d4,%a5
	jne .L10
	moveq #5,%d3
	jra .L10
.L13:
	jsr (%a3)
	cmp.b #10,%d0
	jne .L12
	move.w %a4,%d1
	tst.b %d1
	jne .L22
.L41:
	clr.l %d3
	jra .L12
.L28:
	move.b %d2,%d7
	clr.l %d4
	moveq #4,%d3
	jra .L10
.L30:
	moveq #1,%d3
.L10:
	jsr (%a3)
	cmp.b #57,%d0
	jls .L23
	add.b #-55,%d0
	jra .L24
.L23:
	add.b #-48,%d0
.L24:
	move.b %d0,%d1
	lsl.b #4,%d1
	move.l %d1,-4(%fp)
	jsr fetch_byte
	move.l -4(%fp),%d1
	move.b %d0,%d2
	cmp.b #57,%d0
	jls .L25
	add.b #-55,%d2
	jra .L40
.L25:
	add.b #-48,%d2
.L40:
	or.b %d1,%d2
	jra .L12
.L32:
	move.w #1,%a4
.L39:
	moveq #6,%d3
.L12:
	addq.l #1,%d4
	jra .L26
.L22:
	moveq #-1,%d0
	cmp.l %d5,%d0
	jeq .L19
	move.l %d5,%a0
	jmp (%a0)
.L19:
.L27:
	jra .L27
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
