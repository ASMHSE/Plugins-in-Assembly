; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
.data 

    pmargin         xd   0
    mmargin         xd   4
    tabcolumn       xd   0
    currentcolumn   xd   0
    commentcolumn   xd  25 

    level           xd   0
    levelt          xd  20 dup (0)
    levf1           xd   0
    rev3            xd   0

.code 

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
align 16 

memfillb proc lpmem : XWORD, ln : XWORD, fill : byte 
    mov   xdx, lpmem ; buffer address
    @if ln gt 0 
        mov   al, fill ; fill chars

        mov   xcx, ln ; byte length
        align 4 

      @@: 
        mov   byte ptr [xdx], al 
        inc   xdx 
        dec   xcx 

        jnz   @B 
    @endif 
    mov   xax, xdx 
    ret   

memfillb endp 

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

align 16 

MemCopyX proc Source : PTR BYTE, Dest : PTR BYTE, ln : XWORD 
    USING xsi, xdi 
    SaveRegs 

    ; ---------------------------------------------------------
    ; Copy ln bytes of memory from Source buffer to Dest buffer
    ;      ~~                      ~~~~~~           ~~~~
    ; USAGE:
    ; invoke MemCopyX,ADDR Source,ADDR Dest,4096
    ;
    ; NOTE: Dest buffer must be at least as large as the source
    ;       buffer otherwise a page fault will be generated.
    ; ---------------------------------------------------------

    cld   
    mov   xsi, Source 
    mov   xdi, Dest 
    mov   xcx, ln 

    shr   xcx, 2 
    rep   movsd 

    mov   xcx, ln 
    and   xcx, 3 
    rep   movsb 

    RestoreRegs 
    ret   

MemCopyX endp 

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
compara macro arg1, arg2, arg3, arg4 
    local difiere 
    ifnb  <arg4> 
        ifdifi <xax>, <arg3> 
            _push xax 
        endif 
        mov   xax, arg3 
        @if xax ne arg4 
            ifdifi <xax>, <arg3> 
                _pop  xax 
            endif 
            jmp   difiere 
        @endif 
        ifdifi <xax>, <arg3> 
            _correction +1 
            _pop  xax 
        endif 
    endif 

    invoke szCmpiG, arg1, arg2, arg3 
  difiere: 
endm  

checkpmlevels macro 
    mov   xax, 0 
    @if plevel gt 0 
        mov   xdx, 0 
        mov   xax, plevel 
        dec   xax 
        mov   xcx, ltab 
        mul   xcx 
        add   xax, pmargin 
    @endif 
    _push xax 
    mov   xax, 0 
    @if mlevel gt 0 
        mov   xdx, 0 
        mov   xax, mlevel 
        dec   xax 
        mov   xcx, ltab 
        mul   xcx 
        @if section gt 3 || section eq 2 
            add   xax, mmargin 
        @endif 
    @endif 
    _pop  xdx 
    add   xdx, xax 
    @if xdx gt 0 
        invoke memfillb, mem2, xdx, 32 
        mov   mem2, xax 
    @endif 
endm  

checkpoint macro 
    ForLp i, 0, 17 
        lea   xcx, parp 
        shl   xax, 1 
        mov   _edx, [xcx+xax*@ws+@ws] 
        mov   xax, [xcx+xax*@ws] 
        ;DBout xax
        compara _vtoken(xbx*@ws), xax, _ltoken(xbx*@ws), _edx 
        @if xax eq 0 
            mov   xax, i 
            @if xax lt 4 
                mov   section, xax 
                @if xax lt 3 
                    jmp   loop5b 
                @endif 
            @elseif xax lt 9 
                mov   levf1, 1 
            @elseif xax lt 14 
                dec   level 
            @else 
                dec   level 
                mov   levf1, 1 
            @endif 
            @if xax gt 3 
                jmp   loop5a 
            @endif 
        @endif 
    Next  i 
endm  

checkarroba macro 
    ForLp i, 0, 7 
        lea   xcx, parr 
        shl   xax, 1 
        mov   _edx, [xcx+xax*@ws+@ws] 
        mov   xax, [xcx+xax*@ws] 
        compara _vtoken(xbx*@ws), xax, _ltoken(xbx*@ws), _edx 
        @if xax eq 0 
            mov   xax, i 
            @if xax lt 3 
                mov   levf1, 1 
            @elseif xax lt 6 
                dec   level 
            @else 
                dec   level 
                mov   levf1, 1 
            @endif 
            jmp   checkflowend 
        @endif 
    Next  i 
endm  

checkconditional macro 
    ForLp i, 0, 25 
        lea   xcx, parb 
        shl   xax, 1 
        mov   _edx, [xcx+xax*@ws+@ws] 
        mov   xax, [xcx+xax*@ws] 
        compara _vtoken(xbx*@ws), xax, _ltoken(xbx*@ws), _edx 
        @if xax eq 0 
            mov   xax, i 
            @if xax lt 12 
                mov   levf1, 1 
            @elseif xax eq 12 
                dec   level 
            @else 
                dec   level 
                mov   levf1, 1 
            @endif 
            ExitFor i 
        @endif 
    Next  i 
endm  

checkflow macro 
    ForLp i, 1, 8 
        lea   xcx, para 
        shl   xax, 1 
        mov   _edx, [xcx+xax*@ws+@ws] 
        mov   xax, [xcx+xax*@ws] 
        compara _vtoken(xbx*@ws), xax, _ltoken(xbx*@ws), _edx 
        @if xax eq 0 
            lea   xcx, levelt 
            @if i eq 1 
                dec   level 
                mov   xax, level 
                @if xword ptr[xcx+xax*@ws] eq 1 
                    dec   mlevel 
                    mov   xword ptr [xcx+xax*@ws], 0 
                @endif 
            @else 
                mov   xax, level 
                mov   xword ptr [xcx+xax*@ws], 0 
                mov   levf1, 1 
            @endif 
            jmp   checkflowend 
        @endif 
    Next  i 
    ForLp i, 0, 10 
        lea   xcx, pard 
        shl   xax, 1 
        mov   _edx, [xcx+xax*@ws+@ws] 
        mov   xax, [xcx+xax*@ws] 
        compara _vtoken(xbx*@ws), xax, _ltoken(xbx*@ws), _edx 
        @if xax eq 0 
            mov   xax, i 
            @if xax lt 5 
                mov   levf1, 1 
                @if xax eq 4 
                    mov   rev3, 1 
                @endif 
            @elseif xax lt 9 
                dec   level 
                @if xax eq 8 
                    dec   level 
                @endif 
            @else 
                @if rev3 eq 1 
                    dec   rev3 
                @else 
                    dec   level 
                @endif 
                mov   levf1, 1 
            @endif 
            jmp   checkflowend 
        @endif 
    Next  i 
endm  

ProcessToken proc mem2 : XWORD 
    USING xbx, xdi 
    local sep1 : xword, rev1 : xword, rev2 : xword, i : xword 
    local mem3 :xword 
    local fregTLS() 

    SaveRegs 

    mov   sep1, 1 
    mov   rev1, 0 
    mov   rev2, 0 
    mov   levf1, 0 
    mov   xax, mem2 
    mov   mem3, xax 

    @if ntokens ne -1 
        ;TODO chequear %

        mov   xbx, 0 ;   primeras palabras

        @if _ttoken(xbx*@ws) eq token_expand 
            mov   rev1, lreve 
            inc   xbx 
        @endif 
        compara _vtoken(xbx*@ws), offset par3b, _ltoken(xbx*@ws), 5 ;   Method    
        @if xax eq 0 
            jmp   loop6 
        @endif 
        compara _vtoken(xbx*@ws), offset par4b, _ltoken(xbx*@ws), 8 ;   MethodEnd    
        @if xax eq 0 
            jmp   loop6 
        @endif 
        compara _vtoken(xbx*@ws), offset par5b, _ltoken(xbx*@ws), 3 ;   End    
        @if xax eq 0 
            jmp   loop6 
        @endif 
        compara _vtoken(xbx*@ws), offset par6b, _ltoken(xbx*@ws), 7 ;   include    
        @if xax eq 0 
            jmp   loop5d 
        @endif 
        compara _vtoken(xbx*@ws), offset par7b, _ltoken(xbx*@ws), 10 ;   includelib
        @if xax eq 0 
            jmp   loop5d 
        @endif 

        @if ntokens gt 0 
            inc   xbx ;   segundas palabras

            @if _ttoken(xbx*@ws) eq token_colon 
                mov   rev1, lrevl 
                mov   xbx, 0 
                jmp   loop5a 
            @endif 

            compara _vtoken(xbx*@ws), offset par1b, _ltoken(xbx*@ws), 4 ;   proc
            @if xax eq 0 
                mov   xdx, 0 
                mov   xax, plevel 
                ;.if xax == 0 
                mov   level, 1 
                ;.endif 
                mov   xcx, ltab 
                mul   xcx 
                add   xax, pmargin 
                inc   plevel 
                ;inc   level 
                jmp   loop6 
            @endif 

            compara _vtoken(xbx*@ws), offset par2b, _ltoken(xbx*@ws), 4 ;   endp
            @if xax eq 0 
                mov   level, 0 
                dec   plevel 
                mov   xdx, 0 
                mov   xax, plevel 
                mov   xcx, ltab 
                mul   xcx 
                add   xax, pmargin 
                jmp   loop6 
            @endif 

            compara _vtoken(xbx*@ws), offset para0, _ltoken(xbx*@ws), 5 ;   macro
            @if xax eq 0 
                ;.if mlevel == 0 
                mov   level, 0 
                ;.endif 

                mov   xax, level 
                lea   xcx, levelt 
                mov   xword ptr [xcx+xax*@ws], 1 
                mov   xdx, 0 
                mov   xax, mlevel 
                dec   xax 
                mov   xcx, ltab 
                mul   xcx 
                inc   mlevel 
                mov   levf1, 1 
                @if section gt 4 
                    add   xax, mmargin 
                @endif 
                jmp   loop6 
            @endif 
        @endif 
    ; chequear :

      loop5: 
        mov   xbx, 0 
        @if xbx le ntokens 
            mov   xax, _vtoken(xbx*@ws) 
            mov   dl, [xax] 
            @if dl eq "." 
                checkpoint 
            @elseif dl eq "@" 
                checkarroba 
            @else 
                @if _ttoken(xbx*@ws) eq token_comma 
                    mov   rev2, -1 
                @else 
                    checkconditional 
                @endif 
                @if _ltoken(xbx*@ws) lt 5 
                    mov   xax, 6 
                    mov   xcx, _ltoken(xbx*@ws) 
                    sub   xax, xcx 
                    mov   sep1, xax 
                @endif 
            @endif 
            @if section lt 3 
              loop5d: 
                mov   _edx, final 
                sub   _edx, inicio 
                @if xdx gt 0 
                    _push _edx 
                    invoke MemCopyX, inicio, mem3, _edx 
                    _pop  xax 
                    add   xax, mem3 
                    mov   mem2, xax 
                @endif 
                jmp   termina2 
            @endif 
            checkflow 
          checkflowend: 

          loop5a: 
            checkpmlevels 
            mov   xdx, 0 
            mov   xax, level 
          ;  sub   xax, mlevel 
          ; sub   xax, plevel 
            @if xax gt 0 
                mov   xcx, ltab 
                mul   xcx 
                add   xax, rev1 
                add   xax, rev2 
                @if xax gt 0 ; OJO
                    ;int 3
                    invoke memfillb, mem2, xax, 32 
                    mov   mem2, xax 
                @endif 
            @endif 

          loop5b: 
            invoke MemCopyX, _vtoken(xbx*@ws), mem2, _ltoken(xbx*@ws) 
            mov   xax, _ltoken(xbx*@ws) 
            add   xax, mem2 
            mov   mem2, xax 
            invoke memfillb, mem2, sep1, 32 
            mov   mem2, xax 
            mov   sep1, 1 

            inc   xbx 
        @endif 

      loop5i: 
        @if xbx le ntokens 
            @if _ttoken(xbx*@ws) eq token_comma 
                dec   mem2 
            @elseif _ttoken(xbx*@ws) eq token_colon 
                @if xbx eq 1 
                    dec   mem2 
                @endif 
            @endif 
            invoke MemCopyX, _vtoken(xbx*@ws), mem2, _ltoken(xbx*@ws) 
            mov   xax, _ltoken(xbx*@ws) 
            add   xax, mem2 
            mov   mem2, xax 
            invoke memfillb, mem2, sep1, 32 
            mov   mem2, xax 
          loop5c: 
            inc   xbx 
            jmp   loop5i 
        @endif 
    @else 
        checkpmlevels 
    @endif 
    jmp   termina 

  loop6: 
    @if ntokens ne -1 
        mov   currentcolumn, xax 
        @if xax gt 0 
            invoke memfillb, mem2, xax, 32 
            mov   mem2, xax 
        @endif 
        mov   xbx, 0 
      loop6i: 
        @if xbx le ntokens 
            @if _ttoken(xbx*@ws) eq token_comma 
                dec   mem2 
            @endif 
            invoke MemCopyX, _vtoken(xbx*@ws), mem2, _ltoken(xbx*@ws) 
            mov   xax, _ltoken(xbx*@ws) 
            add   xax, mem2 
            mov   mem2, xax 
            invoke memfillb, mem2, 1, 32 
            mov   mem2, xax 

          loop6c: 
            inc   xbx 
            jmp   loop6i 
        @endif 
    @endif 

  termina: 
    @if lcomment gt 0 
        @if ntokens eq -1 
            invoke memfillb, mem2, pcomment, 32 
            mov   mem2, xax 
        @endif 
        invoke MemCopyX, commentary, mem2, lcomment 
        mov   xax, lcomment 
        add   xax, mem2 
        mov   mem2, xax 
    @endif 

  termina2: 
    mov   xdx, level 
    add   xdx, levf1 
    mov   level, xdx 
    mov   xax, mem2 

    mov   byte ptr [xax], 0Dh 
    inc   xax 
    mov   byte ptr [xax], 0Ah 
    inc   xax 

    RestoreRegs 
    ret   
ProcessToken endp 
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
