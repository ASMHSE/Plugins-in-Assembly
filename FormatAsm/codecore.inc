
include taketoken.inc
include processtoken.inc

szFormatText proc uses xdi xsi mem : DWORD, mem2 : DWORD, nlen : DWORD 
;    local inicio : dword, final : dword

    mov   xdi, mem 
    mov   xsi, mem2 
    ;inc xsi
    mov   inicio, xdi 
    .while nlen > 0 
        mov   al, byte ptr [xdi] 
        .if al == 0Dh 
        .elseif al == 0Ah 
            mov   final, xdi 
            sub   final, 1 
            invoke szProcessLine 
            invoke ProcessToken, xsi 
            mov   xsi, xax 
            mov   inicio, xdi 
            add   inicio, 1 
            inc   linea 
        .endif 
        dec   nlen 
        inc   xdi 
    .endw 
    inc   xsi 
    mov   byte ptr [xsi], 0 
    ret   
szFormatText endp 

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

plugin_function proc 

    LOCAL tLen :DWORD 
    LOCAL pMem :DWORD 
    LOCAL pMem2 :DWORD 

    invoke SendMessage, hEdit, WM_GETTEXTLENGTH, 0, 0 ; get the text length
    inc   eax 
    mov   tLen, eax 
    shl   eax, 1 
    mov   pMem2, alloc(eax) 
    mov   pMem, alloc(tLen) 

    invoke SendMessage, hEdit, WM_GETTEXT, tLen, pMem ; current line written to tbuf

    invoke szFormatText, pMem, pMem2, tLen 

    invoke SendMessage, hEdit, WM_SETTEXT, 0, pMem2 ; get the character range

    free  pMem 
    free  pMem2 

    ret   

plugin_function endp 
