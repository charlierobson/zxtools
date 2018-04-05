;-------------------------------------------------------------------------------

        .org    $4009

        .exportmode NO$GMB
        .export

versn	.byte   $00
e_ppc	.word   $0000
d_file	.word   dfile
df_cc	.word   dfile+1 
vars	.word   var 
dest	.word   $0000 
e_line	.word   var+1 
ch_add	.word   last-1 
x_ptr	.word   $0000 
stkbot	.word   last 
stkend	.word   last 
breg	.byte   $00 
mem     .word   membot 
unuseb	.byte   $00 
df_sz	.byte   $02 
s_top	.word   $0000 
last_k	.word   $ffff 
db_st	.byte   $ff
margin	.byte   55 
nxtlin	.word   line10 
oldpc   .word   $0000
flagx   .byte   $00
strlen  .word   $0000 
t_addr  .word   $0c8d; $0c6b
seed    .word   $0000 
frames  .word   $ffff
coords  .byte   $00 
        .byte   $00 
pr_cc   .byte   188 
s_posn  .byte   33 
s_psn1  .byte   24 
cdflag  .byte   64 
PRTBUF  .fill   32,0
prbend  .byte   $76 
membot  .fill   32,0

line1:  .byte   0,1
        .word   line1end-$-2
        .byte   $ea

start:  jr      init

        ld      a,0
        jp      AFXPLAY

init:
        ld      bc,$e007                ; go low
        ld      a,$b2
        out     (c),a

        ld      hl,sfx                  ; move sfx bank
        ld      de,$2000
        ld      bc,sfxlen-sfx
        ldir

        ld      hl,$2000
        call    INIT_AFX

	ld	ix,relog                ; install irq handler
	ld	(iy+$34),$ff
	ret

relog:
	ld	a,r
	ld	bc,$1901
	ld	a,$f5
	call	$02b5
	call	$0292
	call	$0220

	; do here
        push    iy
        call    AFXFRAME
        pop     iy

+:	ld      ix,relog
	jp      $02a4

        #include "ayfxplay.asm"
sfx:
        .incbin zedragon.afb

    .byte   076H                    ; N/L
line1end:

;-------------------------------------------------------------------------------

line10:
        .byte   0,10
        .word   line10end-$-2
        .byte   $F9,$D4,$C5,$0B         ; RAND USR VAL "
        .byte   $1D,$22,$21,$1D,$20	; 16514 
        .byte   $0B                     ; "
        .byte   076H                    ; N/L
line10end:

;-------------------------------------------------------------------------------

dfile:
        .byte   076H
        .repeat 24
        .fill   32,0
        .byte   $76
        .loop

;-------------------------------------------------------------------------------

var:    .byte   080H
last:

        .end