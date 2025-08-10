include szCmpiG.inc
include taketoken.inc
include processtoken.inc

GetLastVisibleLine proc 
    local r : RECT 
    local p : POINT 

    invoke SendMessage, hEdit, EM_GETRECT, 0, addr r 
    mov   eax, r.left 
    add   eax, 2 
    mov   p.x, eax 
    mov   eax, r.bottom 
    sub   eax, 2 
    mov   p.y, eax 
    invoke SendMessage, hEdit, EM_CHARFROMPOS, 0, addr p 
    invoke SendMessage, hEdit, EM_EXLINEFROMCHAR, 0, xax 
    ret   
GetLastVisibleLine endp 

szFormatText proc mem : XWORD, mem2 : XWORD, nlen : XWORD 
    USINGS <xbx, xdi>, <r13,r14,r15> 

    SaveRegs 

    if    @ws eq 8 
        lea   r13, vtoken 
        lea   r14, ltoken 
        lea   r15, ttoken 
    endif 

    mov   xdi, mem 
    mov   xsi, mem2 
    mov   xbx, nlen 
   ;inc xsi
    mov   inicio, xdi 
    @while xbx gt 0 
        mov   al, byte ptr [xdi] 
        @if al eq 0Dh 
        @elseif al eq 0Ah 
            mov   final, xdi 
           sub   final, 1 
            invoke ProcessLine 
            invoke ProcessToken, xsi 
            mov   xsi, xax 
            mov   inicio, xdi 
            add   inicio, 1 
            inc   linea 
        @endif 
        dec   xbx 
        inc   xdi 
    @endw 
    inc   xsi 
    mov   byte ptr [xsi], 0 
    RestoreRegs 

    ret   
szFormatText endp 

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

plugin_function proc 

    LOCAL tLen :XWORD 
    LOCAL pMem :XWORD 
    LOCAL pMem2 :XWORD 
    local nline :XWORD, mline :XWORD 
    local cr : CHARRANGE 

    if @ws eq 8 
        ;DBbegin 
        ;DBout "running"
    endif 
    invoke SendMessage, hEdit, WM_GETTEXTLENGTH, 0, 0 ; get the text length
    inc   xax 
    mov   tLen, xax 
    shl   xax, 1 
    mov   pMem2, alloc(xax) 
    mov   pMem, alloc(tLen) 

    invoke SendMessage, hEdit, EM_GETFIRSTVISIBLELINE, 0, 0 
    mov   nline, xax 
    call  GetLastVisibleLine 
    mov   mline, xax 

    invoke SendMessage, hEdit, WM_GETTEXT, tLen, pMem ; current line written to tbuf

    invoke szFormatText, pMem, pMem2, tLen 
    if @ws eq 4
    invoke szCmpi, pMem, pMem2, tLen 
    else
    invoke szCmpi, pMem, pMem2
 ;   DBreg xax
 ;   DBout pMem2
    mov xax, 1
    endif
    @if xax gt 0 
        invoke SendMessage, hEdit, WM_SETTEXT, 0, pMem2 ; get the character range

        invoke SendMessage, hEdit, EM_LINEINDEX, nline, 0 
        mov   cr.cpMin, eax 
        mov   cr.cpMax, eax 
        invoke SendMessage, hEdit, EM_EXSETSEL, 0, ADDR cr 
        invoke SendMessage, hEdit, EM_LINEINDEX, mline, 0 
        mov   cr.cpMin, eax 
        mov   cr.cpMax, eax 
        invoke SendMessage, hEdit, EM_EXSETSEL, 0, ADDR cr 
        invoke SendMessage, hEdit, EM_SETMODIFY, TRUE, 0 
    @endif 
    free  pMem 
    free  pMem2 
    if    @ws eq 8 
        ;DBend 
    endif 
    ret   

plugin_function endp 
