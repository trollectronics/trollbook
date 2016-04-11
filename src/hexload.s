#NO_APP
.text
.align	2

.int 0x00100000
.int 0x00000008

loadhex:
	clr.b %d3
	clr.w %d4
	clr.l %d1
	lea 0.w,%a1
	move.l #65536,%a0
	clr.l %d0
.L31:
	move.l %d0,%d5
	subq.l #1,%d5
	moveq #5,%d7
	cmp.l %d5,%d7
	jcs .L11
	move.w .L4(%pc,%d5.l*2),%d5
	jmp %pc@(2,%d5:w)
	.balignw 2,0x284c
	.swbeg	&6
.L4:
	.word .L3-.L4
	.word .L5-.L4
	.word .L6-.L4
	.word .L7-.L4
	.word .L33-.L4
	.word .L22-.L4
.L6:
	move.b %d2,%d3
	clr.l %d1
	moveq #4,%d0
	jra .L24
.L11:
	move.l 1048580,%d0
	btst #1,%d0
	jeq .L11
	move.l 1048576,%d0
	cmp.b #58,%d0
	jne .L33
	jra .L34
.L3:
	clr.l %d6
	move.b %d2,%d6
	clr.l %d1
	moveq #2,%d0
	jra .L24
.L5:
	and.w #255,%d2
	moveq #1,%d5
	cmp.l %d1,%d5
	jne .L12
	move.w %d2,%d4
	lsl.w #8,%d4
	jra .L24
.L12:
	or.w %d2,%d4
	moveq #3,%d0
	jra .L24
.L7:
	tst.b %d3
	jne .L13
	clr.l %d5
	move.w %d4,%d5
	move.b %d2,(%a1,%d5.l)
	jra .L14
.L13:
	cmp.b #1,%d3
	jeq .L15
	cmp.b #4,%d3
	jne .L16
	and.l #255,%d2
	moveq #1,%d7
	cmp.l %d1,%d7
	jne .L17
	moveq #24,%d5
	lsl.l %d5,%d2
	move.l %d2,%a1
	jra .L14
.L17:
	swap %d2
	clr.w %d2
	add.l %d2,%a1
	jra .L14
.L16:
	cmp.b #5,%d3
	jne .L19
	moveq #1,%d7
	cmp.l %d1,%d7
	sne %d5
	extb.l %d5
	move.l %a0,%d7
	and.l %d5,%d7
	move.l %d7,%a0
	and.l #255,%d2
	moveq #4,%d5
	sub.l %d1,%d5
	lsl.l #3,%d5
	lsl.l %d5,%d2
	add.l %d2,%a0
.L14:
	cmp.l %d1,%d6
	jne .L24
	moveq #5,%d0
	jra .L24
.L22:
	move.l 1048580,%d2
	btst #1,%d2
	jeq .L22
	move.l 1048576,%d2
	cmp.b #10,%d2
	sne %d2
	extb.l %d2
	and.l %d2,%d0
	jra .L24
.L33:
	moveq #6,%d0
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
	add.b #-56,%d2
	jra .L26
.L25:
	add.b #-48,%d2
.L26:
	lsl.b #4,%d2
.L28:
	move.l 1048580,%d5
	btst #1,%d5
	jeq .L28
	move.l 1048576,%d5
	cmp.b #57,%d5
	jls .L29
	add.b #-56,%d5
	jra .L38
.L29:
	add.b #-48,%d5
.L38:
	or.b %d5,%d2
	addq.l #1,%d1
	jra .L31
.L15:
	jmp (%a0)
.L19:
.L32:
	jra .L19
	nop
