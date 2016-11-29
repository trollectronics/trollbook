.section text

.global invalidate
.align 2
invalidate:
	cinva %bc
	rts

.global lolhest
.align 2
lolhest:
	move.l #0x40000000, %a0
	lea hejtest, %a1
	move16 (%a0)+, (%a1)+
	rts

.align 4
loldata:
.int 0xDEADBEEF
.int 0x00000000
.int 0xCAFEBABE
.int 0x5A5A5A5A

.section data
.align 4
.global hejtest
hejtest:
.int 0x0
.int 0x0
.int 0x0
.int 0x0
