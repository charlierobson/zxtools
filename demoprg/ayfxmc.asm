        .org    16514

init:   jr      init_default
reinit: jr      init_again
play:   ld      a,0
        jp      AFXPLAY

init_again:
        ld      hl,$2000
        jr      {+}

init_default:
        ld      hl,sfx

+:      call    INIT_AFX

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
sfxlen: