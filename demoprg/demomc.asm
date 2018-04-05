    .org    16514
    ld      hl,demotxt
    jr      {+}

-:  rst     10h
    inc     hl
+:  ld      a,(hl)
    cp      $ff
    jr      nz,{-}
    ret

    .asciimap 0,255,$0f             ; default case, '?'
    .asciimap ' ',' ',0
    .asciimap '"','"',$0b
    .asciimap '£','£',$0c
    .asciimap '$','$',$0d
    .asciimap ':',':',$0e
    .asciimap '(','(',$10
    .asciimap ')',')',$11
    .asciimap '>','>',$12
    .asciimap '<','<',$13
    .asciimap '=','=',$14
    .asciimap '+','+',$15
    .asciimap '-','-',$16
    .asciimap '*','*',$17
    .asciimap '/','/',$18
    .asciimap ';',';',$19
    .asciimap ',',',',$1a
    .asciimap '.','.',$1b
    .asciimap '0','9',{*}-'9'+$1c
    .asciimap 'a','z',{*}-'a'+$26
    .asciimap 'A','Z',{*}-'A'+$26

demotxt:
    .asc    "hello, wORLd!"
    .byte   $ff
