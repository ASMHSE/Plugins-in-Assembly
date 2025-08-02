checkpmlevels macro 
    mov   xax, 0 
    .if plevel > 0 
        mov   xdx, 0 
        mov   xax, plevel 
        dec   xax 
        mov   xcx, ltab 
        mul   xcx 
        add   xax, pmargin 
    .endif 
    push  xax 
    mov   xax, 0 
    .if mlevel > 0 
        mov   xdx, 0 
        mov   xax, mlevel 
        dec   xax 
        mov   xcx, ltab 
        mul   xcx 
        .if section > 3 
            add   xax, mmargin 
        .endif 
    .endif 
    pop   xdx 
    add   xdx, xax 
    .if xdx > 0 
        invoke memfillb, mem2, xdx, " " 
        mov   mem2, xax 
    .endif 
endm  

checkpoint macro 
    ForLp i, 0, 17 
        lea   xcx, parp 
        mov   xax, [xcx+xax*@ws] 
        invoke szCmpi, vtoken[xbx*@ws], xax, ltoken[xbx*@ws] 
        .if xax == 0 
            mov   xax, i 
            .if xax < 4  
                mov   section, xax 
                .if xax < 3  
                    jmp   loop5b 
                .endif 
            .elseif xax < 9  
                mov   levf1, 1 
            .elseif xax < 14  
                dec   level 
            .else 
                dec   level 
                mov   levf1, 1 
            .endif 
            .if xax > 3 
                jmp   loop5a 
            .endif 
        .endif 
    Next  i 
endm  

checkarroba macro 
    ForLp i, 0, 7 
        lea   xcx, parr 
        mov   xax, [xcx+xax*@ws] 
        invoke szCmpi, vtoken[xbx*@ws], xax, ltoken[xbx*@ws] 
        .if xax == 0 
            mov   xax, i 
            .if xax < 4  
                mov   levf1, 1 
            .elseif xax < 6  
                dec   level 
            .else 
                dec   level 
                mov   levf1, 1 
            .endif 
            jmp   checkflowend 
        .endif 
    Next  i 
endm  

checkconditional macro 
    ForLp i, 0, 25 
        lea   xcx, parb 
        mov   xax, [xcx+xax*@ws] 
        invoke szCmpi, vtoken[xbx*@ws], xax, ltoken[xbx*@ws] 
        .if xax == 0 
            mov   xax, i 
            .if xax < 12  
                mov   levf1, 1 
            .elseif xax == 12 
                dec   level 
            .else 
                dec   level 
                mov   levf1, 1 
            .endif 
            ExitFor i 
        .endif 
    Next  i 
endm  

checkflow macro 
    ForLp i, 1, 8 
        lea   xcx, para 
        mov   xax, [xcx+xax*@ws] 
        invoke szCmpi, vtoken[xbx*@ws], xax, ltoken[xbx*@ws] 
        .if xax == 0 
            .if i == 1 
                dec   level 
                mov   xax, level 
                .if levelt[xax*@ws] == 1 
                    dec   mlevel 
                    mov   levelt[xax*@ws], 0 
                .endif 
            .else 
                mov   xax, level 
                mov   levelt[xax*@ws], 0 
                mov   levf1, 1 
            .endif 
            jmp   checkflowend 
        .endif 
    Next  i 
    ForLp i, 0, 10 
        lea   xcx, pard 
        mov   xax, [xcx+xax*@ws] 
        invoke szCmpi, vtoken[xbx*@ws], xax, ltoken[xbx*@ws] 
        .if xax == 0 
            mov   xax, i 
            .if xax < 5  
                mov   levf1, 1 
                .if xax == 4 
                    mov   rev3, 1 
                .endif 
            .elseif xax < 9  
                dec   level 
                .if xax == 8 
                    dec   level 
                .endif 
            .else 
                .if rev3 == 1 
                    dec   rev3 
                .else 
                    dec   level 
                .endif 
                mov   levf1, 1 
            .endif 
            jmp   checkflowend 
        .endif 
    Next  i 
    checkflowend: 
endm  
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
.data 

    pmargin         xd   0
    mmargin         xd   4
    tabcolumn       xd   0
    currentcolumn   xd   0
    commentcolumn   xd  25 

    level           xd   0
    levelt          xd  20 dup (0)
    levf1           xd 0
    rev3            xd 0
.code 

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

memfillb proc lpmem : XWORD, ln : XWORD, fill : byte 

    mov   xdx, lpmem ; buffer address
    .if ln > 0 
        mov   al, fill ; fill chars

        mov   xcx, ln ; byte length
        align 4 

      @@: 
        mov   byte ptr [xdx], al 
        inc   xdx 
        dec   xcx 

        jnz   @B 
    .endif 
    mov   xax, xdx 
    ret   

memfillb endp 

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

ProcessToken proc uses xbx xdi mem2 : DWORD 
    local sep1 : xword, rev1 : xword, rev2 : xword, i : xword 
    local mem3 :xword 

    mov   sep1, 1 
    mov   rev1, 0 
    mov   rev2, 0 
    mov   levf1, 0 
    mov   xax, mem2 
    mov   mem3, xax 


    .if ntokens != -1 
        ;TODO chequear %

        mov   xbx, 0 ;   primeras palabras

        .if ttoken[xbx*@ws] == token_expand 
            mov   rev1, lreve 
            inc   xbx 
        .endif 

        invoke szCmpi, vtoken[xbx*@ws], offset par3b, ltoken[xbx*@ws] ;   Method    
        .if xax == 0 
            jmp   loop6 
        .endif 
        invoke szCmpi, vtoken[xbx*@ws], offset par4b, ltoken[xbx*@ws] ;   MethodEnd    
        .if xax == 0 
            jmp   loop6 
        .endif 
        invoke szCmpi, vtoken[xbx*@ws], offset par5b, ltoken[xbx*@ws] ;   End    
        .if xax == 0 
            jmp   loop6 
        .endif 
        .if ltoken[xbx*@ws] == 7 
            invoke szCmpi, vtoken[xbx*@ws], offset par6b, ltoken[xbx*@ws] ;   include    
            .if xax == 0 
                jmp   loop5d 
            .endif 
        .endif 

        .if ltoken[xbx*@ws] == 10 
            invoke szCmpi, vtoken[xbx*@ws], offset par7b, ltoken[xbx*@ws] ;   includelib
            .if xax == 0 
                jmp   loop5d 
            .endif 
        .endif 
        .if ntokens > 0 
            inc   xbx ;   segundas palabras

            .if ttoken[xbx*@ws] == token_colon 
                mov   rev1, lrevl 
                mov   xbx, 0 
                jmp   loop5a 
            .endif 

            invoke szCmpi, vtoken[xbx*@ws], offset par1b, ltoken[xbx*@ws] ;   proc
            .if xax == 0 
                mov   xdx, 0 
                mov   xax, plevel 
                .if xax == 0 
                    mov   level, 0 
                .endif 
                mov   xcx, ltab 
                mul   xcx 
                add   xax, pmargin 
                inc   plevel 
                inc   level 
                jmp   loop6 
            .endif 

            invoke szCmpi, vtoken[xbx*@ws], offset par2b, ltoken[xbx*@ws] ;   endp
            .if xax == 0 
                dec   level 
                dec   plevel 
                mov   xdx, 0 
                mov   xax, plevel 
                mov   xcx, ltab 
                mul   xcx 
                add   xax, pmargin 
                jmp   loop6 
            .endif 

            invoke szCmpi, vtoken[xbx*@ws], offset para0, ltoken[xbx*@ws] ;   macro
            .if xax == 0 
                .if mlevel == 0 
                    mov   level, 0 
                .endif 

                mov   xax, level 
                mov   levelt[xax*@ws], 1 
                mov   xdx, 0 
                mov   xax, mlevel 
                dec   xax 
                mov   xcx, ltab 
                mul   xcx 
                inc   mlevel 
                mov   levf1, 1 
                .if section > 4 
                    add   xax, mmargin 
                .endif 
                jmp   loop6 
            .endif 
        .endif 
    ; chequear :


      loop5: 
        mov   xbx, 0 
        .if xbx <= ntokens  
            mov   xax, vtoken[xbx*@ws] 
            mov   dl, [xax] 
            .if dl == "." 
                checkpoint 
            .elseif dl == "@" 
                checkarroba 
            .else 
                .if ttoken[xbx*@ws] == token_comma 
                    mov   rev2, -1 
                .else 
                    checkconditional 
                .endif 
                .if ltoken[xbx*@ws] < 5  
                    mov   xax, 6 
                    mov   xcx, ltoken[xbx*@ws] 
                    sub   xax, xcx 
                    mov   sep1, xax 
                .endif 
            .endif 
            .if section < 3  
              loop5d: 
                mov   xdx, final 
                sub   xdx, inicio 
                .if xdx > 0 
                    push  xdx 
                    invoke MemCopy, inicio, mem3, xdx 
                    pop   xax 
                    add   xax, mem3 
                    mov   mem2, xax 
                .endif 
                jmp   termina2 
            .endif 
            checkflow 
          loop5a: 
            checkpmlevels 
            mov   xdx, 0 
            mov   xax, level 
            sub   xax, mlevel 
            .if xax > 0 
                mov   xcx, ltab 
                mul   xcx 
                add   xax, rev1 
                add   xax, rev2 
                .if xax > 0 ; OJO
                    invoke memfillb, mem2, xax, " " 
                    mov   mem2, xax 
                .endif 

            .endif 

          loop5b: 
            invoke MemCopy, vtoken[xbx*@ws], mem2, ltoken[xbx*@ws] 
            mov   xax, ltoken[xbx*@ws] 
            add   xax, mem2 
            mov   mem2, xax 
            invoke memfillb, mem2, sep1, " " 
            mov   mem2, xax 
            mov   sep1, 1 

            inc   xbx 
        .endif 

      loop5i: 
        .if xbx <= ntokens  
            .if ttoken[xbx*@ws] == token_comma 
                dec   mem2 
            .elseif ttoken[xbx*@ws] == token_colon 
                .if xbx == 1 
                    dec   mem2 
                .endif 
            .endif 
            invoke MemCopy, vtoken[xbx*@ws], mem2, ltoken[xbx*@ws] 
            mov   xax, ltoken[xbx*@ws] 
            add   xax, mem2 
            mov   mem2, xax 
            invoke memfillb, mem2, sep1, " " 
            mov   mem2, xax 
          loop5c: 
            inc   xbx 
            jmp   loop5i 
        .endif 
    .else 
        checkpmlevels 
    .endif 
    jmp   termina 

  loop6: 
    .if ntokens != -1 
        mov   currentcolumn, xax 
        .if xax > 0 
            invoke memfillb, mem2, xax, " " 
            mov   mem2, xax 
        .endif 
        mov   xbx, 0 
      loop6i: 
        .if xbx <= ntokens  

            .if ttoken[xbx*@ws] == token_comma 
                dec   mem2 
            .endif 
            invoke MemCopy, vtoken[xbx*@ws], mem2, ltoken[xbx*@ws] 
            mov   xax, ltoken[xbx*@ws] 
            add   xax, mem2 
            mov   mem2, xax 
            invoke memfillb, mem2, 1, " " 
            mov   mem2, xax 

          loop6c: 
            inc   xbx 
            jmp   loop6i 
        .endif 
    .endif 

  termina: 
    .if lcomment > 0 
        .if ntokens == -1 
            invoke memfillb, mem2, pcomment, " " 
            mov   mem2, xax 
        .endif 
        invoke MemCopy, commentary, mem2, lcomment 
        mov   xax, lcomment 
        add   xax, mem2 
        mov   mem2, xax 
    .endif 

  termina2: 
    mov   xdx, level 
    add   xdx, levf1 
    mov   level, xdx 
    mov   xax, mem2 

    mov   byte ptr [xax], 0Dh 
    inc   xax 
    mov   byte ptr [xax], 0Ah 
    inc   xax 

    ret   
ProcessToken endp 
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
