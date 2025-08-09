
.data

    nStoredTokens   equ 150
    ltab            equ   4
    lreve           equ  -1     
    lrevl           equ  -2     
    ;lrevf1          equ   0

    stringitem macro name, value
       % @CatStr(<name>,<_len>) = @SizeStr(<value >)-1 
        exitm <&name db "&value",0>
    endm

    stringtable macro name, tope
        align @ws
        name label xword
        count = 0
        repeat tope
          %  xd offset @CatStr(<name>,%count)   
          %  xd @CatStr(<name>,%count,<_len>)   
            count = count+1
        endm    
    endm

    stringitem(para0 ,macro)
    stringitem(para1 ,endm)
    stringitem(para2 ,for)
    stringitem(para3 ,forc)
    stringitem(para4 ,repeat)
    stringitem(para5 ,rept)
    stringitem(para6 ,while)
    stringitem(para7 ,irp)
    stringitem(para8 ,irpc)

    stringtable para, 9

    par1b   db  "proc",0
    par2b   db  "endp",0
    par3b   db  "method",0
    par4b   db  "methodend",0
    par5b   db  "end",0
    par6b   db  "include",0
    par7b   db  "includelib",0


    stringitem(parb0 ,if)
    stringitem(parb1 ,ife)
    stringitem(parb2 ,if1)
    stringitem(parb3 ,if2)
    stringitem(parb4 ,ifdif)
    stringitem(parb5 ,ifdifi)
    stringitem(parb6 ,ifidn)  
    stringitem(parb7 ,ifidni)
    stringitem(parb8 ,ifb)
    stringitem(parb9 ,ifnb)
    stringitem(parb10,ifdef)
    stringitem(parb11,ifndef)
    stringitem(parb12,endif)
    stringitem(parb13,else)
    stringitem(parb14,elseif)
    stringitem(parb15,elseife)
    stringitem(parb16,elseif1)
    stringitem(parb17,elseif2)
    stringitem(parb18,elseifdif)
    stringitem(parb19,elseifdifi)
    stringitem(parb20,elseifidn)
    stringitem(parb21,elseifidni)
    stringitem(parb22,elseifb)
    stringitem(parb23,elseifnb)
    stringitem(parb24,elseifdef)
    stringitem(parb25,elseifndef)

    stringtable parb, 26

    stringitem(parp0 ,.const)
    stringitem(parp1 ,.data?)
    stringitem(parp2 ,.data)
    stringitem(parp3 ,.code)
    stringitem(parp4 ,.if)
    stringitem(parp5 ,.while)
    stringitem(parp6 ,.repeat)
    stringitem(parp7 ,.rept)
    stringitem(parp8 ,.switch)
    stringitem(parp9 ,.endif)
    stringitem(parp10,.endw)
    stringitem(parp11,.endsw)
    stringitem(parp12,.until)
    stringitem(parp13,.untilcxz)
    stringitem(parp14,.elseif)
    stringitem(parp15,.else)
    stringitem(parp16,.case)
    stringitem(parp17,.default)

    stringtable parp, 18

    stringitem(pard0 ,ForLp)
    stringitem(pard1 ,ForLpD)
    stringitem(pard2 ,ForLpN)
    stringitem(pard3 ,ForLpR)
    stringitem(pard4 ,switch)
    stringitem(pard5 ,Next)
    stringitem(pard6 ,NextN)
    stringitem(pard7 ,NextR)
    stringitem(pard8 ,endsw)
    stringitem(pard9 ,case)
    stringitem(pard10,default)

    stringtable pard, 11

    stringitem(parr0,@if)
    stringitem(parr1,@while)
    stringitem(parr2,@dothis)
    stringitem(parr3,@endif)
    stringitem(parr4,@endw)
    stringitem(parr5,whilethis)
    stringitem(parr6,@elseif)
    stringitem(parr7,@else)

    stringtable parr, 8

    plevel  xd  0
    mlevel  xd  0
    pini    xd  0
    mini    xd  0

    token_word      equ 0
    token_label     equ 1
    token_proc      equ 2
    token_macro     equ 3
    token_comma     equ 4
    token_cutline   equ 5
    token_expand    equ 6
    token_point     equ 7
    token_arroba    equ 8
    token_colon     equ 9

    section_const   equ 0
    section_data?   equ 1
    section_data    equ 2
    section_code    equ 3


    ntokens     xd  0
    vtoken      xd  nStoredTokens dup (0)       ; values of tokens
    ltoken      xd  nStoredTokens dup (0)       ; token length
    ttoken      xd  nStoredTokens dup (0)       ; token type
    _vtoken macro arg1
        if @ws eq 4
           % exitm <vtoken[&arg1&]>
        else
            exitm <qword ptr [r13+&arg1&]>
        endif 
    endm 
    _ltoken macro arg1
        if @ws eq 4
           % exitm <ltoken[&arg1&]>
        else
            exitm <qword ptr [r14+&arg1&]>
        endif 
    endm 
    _ttoken macro arg1
        if @ws eq 4
           % exitm <ttoken[&arg1&]>
        else
            exitm <qword ptr [r15+&arg1&]>
        endif 
    endm 

    commentary  xd  0
    lcomment    xd  0 
    pcomment    xd  0 
    line_length xd  0

    enclosers   xd  0
    encloser1   xd  0     ; '
    encloser2   xd  0     ; "  
    encloser3   xd  0     ; ()
    encloser4   xd  0     ; []
    encloser5   xd  0     ; <>


    openproc    xd  0
    openmac     xd  0

    inicio      xd  0
    final       xd  0
    section     xd  5

    linea       xd  1
