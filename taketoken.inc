parentesisI equ 40 ;   "("
parentesisD equ 41 ;   ")"
angbrkI equ 60     ;   "<"
angbrkD equ 62     ;   ">"
backslash equ 92   ;   "\"

closetoken macro 
        mov   xcx, xax 
        sub   xcx, xdi 
        mov   _ltoken(xbx*@ws), xcx 
        mov   xdx, _ltoken(xbx*@ws) 
        mov   _ttoken(xbx*@ws), 0 
        mov   iniciado, 0 
endm  

storetoken macro tipo 
        inc   xbx 
        mov   _vtoken(xbx*@ws), xax 
        mov   _ltoken(xbx*@ws), 1 
        mov   _ttoken(xbx*@ws), tipo 
endm  

checkenclosers macro 
        @if dl eq "'" && encloser2 eq 0 
            inc   encloser1 
            inc   enclosers 
        @elseif dl eq '"' && encloser1 eq 0 
            inc   encloser2 
            inc   enclosers 
        @elseif dl eq parentesisI 
            inc   encloser3 
            inc   enclosers 
        @elseif dl eq "[" 
            inc   encloser4 
            inc   enclosers 
        @elseif dl eq angbrkI 
            inc   encloser5 
            inc   enclosers 
        @endif 
endm  
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

ProcessLine proc ;here a comment
    local buffer[1024] : byte 
    local iniciado : xword, spacing : xword 
    ;local t_order : xword  ; xbx

    USING xbx, xdi 

    SaveRegs 

    mov   xax, inicio 
    lea   xcx, buffer 

  loop1i: 
    @if xax lt final 
        mov   dl, [xax] 
        mov   byte ptr [xcx], dl 
        inc   xax 
        inc   xcx 
        jmp   loop1i 
    @endif 
    mov   byte ptr [xcx], 0 
    if    @ws eq 8 
    ;    DBout str$(linea), " - ", addr buffer 
    else  
    ;    PrintString buffer
    endif 
    mov   xbx, -1 
    mov   xdi, 0 
    mov   iniciado, 0 
    mov   commentary, 0 
    mov   lcomment, 0 
    mov   enclosers, 0 
    mov   xax, inicio 
  loop2i: 
    @if xax lt final 
        mov   dl, [xax] 
        @if enclosers gt 0 
            @if encloser1 gt 0 && encloser2 eq 0        ;   comillas simples
                @if dl eq "'" 
                    dec   encloser1 
                    dec   enclosers 
                @endif 
            @elseif encloser2 gt 0 && encloser1 eq 0    ;   comillas dobles
                @if dl eq '"' 
                    dec   encloser2 
                    dec   enclosers 
                @endif 
            @elseif encloser3 gt 0                      ;   parentesis
                @if dl eq parentesisD 
                    dec   encloser3 
                    dec   enclosers 
                @endif 
            @elseif encloser4 gt 0                      ;   square encloser
                @if dl eq ']' 
                    dec   encloser4 
                    dec   enclosers 
                @endif 
            @elseif encloser5 gt 0                      ;   square encloser
                @if dl eq angbrkD 
                    dec   encloser5 
                    dec   enclosers 
                @endif 
            @endif 
        @else 
            @if iniciado 
                @if dl eq 20h || dl eq 9 
                    closetoken 
                @elseif dl eq "," 
                    closetoken 
                    storetoken token_comma 
                @elseif dl eq ":" 
                    closetoken 
                    storetoken token_colon 
                @elseif dl eq backslash 
                    closetoken 
                    storetoken token_cutline 
                @elseif dl eq ";" 
                    closetoken 
                    jmp   iscomment 
                @endif 
                checkenclosers 
            @else 
                @if dl eq 20h || dl eq 9 
                    ;@if dl eq 9
                    ;    add spacing, ltab
                    ;@else
                    ;    inc spacing
                    ;@endif
                @elseif dl eq "," 
                    storetoken token_comma 
                @elseif dl eq backslash 
                    storetoken token_cutline 
                @elseif dl eq ";" 
                    jmp   iscomment 
                @elseif dl eq "%" 
                    storetoken token_expand 
                @else 
                    mov   iniciado, 1 
                    inc   xbx 
                    mov   _vtoken(xbx*@ws), xax 
                    mov   xdi, xax 
                    checkenclosers 
                @endif 
            @endif 
        @endif 
        inc   xax 
        @if xbx ne -1 && xbx gt nStoredTokens 
            jmp   searchcomment 
        @endif 
        jmp   loop2i 
    @endif 
    @if iniciado eq 1 
        closetoken 
    @endif 
    jmp   termina 

  searchcomment: 

  loop3i: 
    @if xax le final 
        mov   dl, [xax] 
        @if dl eq ";" 
            @if iniciado eq 1 
                closetoken 
            @endif 
            jmp   iscomment 
        @endif 
        inc   xax 
        jmp   loop3i 
    @endif 
    jmp   termina 

  iscomment: 
    mov   commentary, xax 
    mov   xcx, final 
    sub   xcx, xax 
    mov   lcomment, xcx 
    mov   xax, commentary 
    sub   xax, inicio 
    mov   pcomment, xax 
    jmp   termina 

  termina: 
    mov   ntokens, xbx 

    RestoreRegs 

    ret   
ProcessLine endp 
