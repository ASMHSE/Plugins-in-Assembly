closetoken macro 
    mov   xcx, xax 
    sub   xcx, xdi 
    mov   ltoken[xbx*@ws], xcx 
    mov   xdx, ltoken[xbx*@ws] 
    mov   ttoken[xbx*@ws], 0 
    mov   iniciado, 0 
endm  

storetoken macro tipo 
    inc   xbx 
    mov   vtoken[xbx*@ws], xax 
    mov   ltoken[xbx*@ws], 1 
    mov   ttoken[xbx*@ws], tipo 
endm  

checkenclosers macro 
    .if dl == "'" && encloser2 == 0 
        inc   encloser1 
        inc   enclosers 
    .elseif dl == '"' && encloser1 == 0 
        inc   encloser2 
        inc   enclosers 
    .elseif dl == "(" 
        inc   encloser3 
        inc   enclosers 
    .elseif dl == "[" 
        inc   encloser4 
        inc   enclosers 
    .elseif dl == "<" 
        inc   encloser5 
        inc   enclosers 
    .endif 
endm  
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

szProcessLine proc uses xbx xdi ;here a comment
    local buffer[1024] : byte 
    local iniciado : xword, spacing : xword 

    mov   xax, inicio 
    lea   xcx, buffer 

  loop1i: 
    .if xax < final  
        mov   dl, [xax] 
        mov   byte ptr [xcx], dl 
        inc   xax 
        inc   xcx 
        jmp   loop1i 
    .endif 
    mov   byte ptr [xcx], 0 
    mov   xbx, -1 
    mov   xdi, 0 
    mov   iniciado, 0 
    mov   commentary, 0 
    mov   lcomment, 0 
    mov   enclosers, 0 
    mov   xax, inicio 
  loop2i: 
    .if xax < final  
        mov   dl, [xax] 
        .if enclosers > 0 
            .if encloser1 > 0 && encloser2 == 0 ;   comillas simples
                .if dl == "'" 
                    dec   encloser1 
                    dec   enclosers 
                .endif 
            .elseif encloser2 > 0 && encloser1 == 0 ;   comillas dobles
                .if dl == '"' 
                    dec   encloser2 
                    dec   enclosers 
                .endif 
            .elseif encloser3 > 0 ;   parentesis
                .if dl == ')' 
                    dec   encloser3 
                    dec   enclosers 
                .endif 
            .elseif encloser4 > 0 ;   square encloser
                .if dl == ']' 
                    dec   encloser4 
                    dec   enclosers 
                .endif 
            .elseif encloser5 > 0 ;   square encloser
                .if dl == '>' 
                    dec   encloser5 
                    dec   enclosers 
                .endif 
            .endif 
        .else 
            .if iniciado 
                .if dl == 20h || dl == 9 
                    closetoken 
                .elseif dl == "," 
                    closetoken 
                    storetoken token_comma 
                .elseif dl == ":" 
                    closetoken 
                    storetoken token_colon 
                .elseif dl == "\" 
                    closetoken 
                    storetoken token_cutline 
                .elseif dl == ";" 
                    closetoken 
                    jmp   iscomment 
                .endif 
                checkenclosers 
            .else 
                .if dl == 20h || dl == 9 
                .elseif dl == "," 
                    storetoken token_comma 
                .elseif dl == "\" 
                    storetoken token_cutline 
                .elseif dl == ";" 
                    jmp   iscomment 
                .elseif dl == "%" 
                    storetoken token_expand 
                .else 
                    mov   iniciado, 1 
                    inc   xbx 
                    mov   vtoken[xbx*@ws], xax 
                    mov   xdi, xax 
                    checkenclosers 
                .endif 
            .endif 
        .endif 
        inc   xax 
        .if xbx != -1 && xbx > nStoredTokens 
            jmp   searchcomment 
        .endif 
        jmp   loop2i 
    .endif 
    .if iniciado == 1 
        closetoken 
    .endif 
    jmp   termina 

  searchcomment: 

  loop3i: 

    .if xax <= final  
        mov   dl, [xax] 
        .if dl == ";" 
            .if iniciado == 1 
                closetoken 
            .endif 
            jmp   iscomment 
        .endif 
        inc   xax 
        jmp   loop3i 
    .endif 
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
    ret   
szProcessLine endp 
