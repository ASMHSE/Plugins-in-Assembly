
.data

    nStoredTokens   equ 150
    ltab            equ   4
    lreve           equ  -1     
    lrevl           equ  -2     
    ;lrevf1          equ   0

    stringtable macro name, tope
        name label xword
        count = 0
        repeat tope
          %  xd offset @CatStr(<name>,%count)   
            count = count+1
        endm    
    endm

    para0   db  "macro",0
    para1   db  "endm",0
    para2   db  "for",0
    para3   db  "forc",0
    para4   db  "repeat",0
    para5   db  "rept",0
    para6   db  "while",0
    para7   db  "irp",0
    para8   db  "irpc",0

    stringtable para, 9

    par1b   db  "proc",0
    par2b   db  "endp",0
    par3b   db  "method",0
    par4b   db  "methodend",0
    par5b   db  "end",0
    par6b   db  "include",0
    par7b   db  "includelib",0


    parb0   db  "if",0
    parb1   db  "ife",0
    parb2   db  "if1",0
    parb3   db  "if2",0  
    parb4   db  "ifdif",0 
    parb5   db  "ifdifi",0  
    parb6   db  "ifidn",0  
    parb7   db  "ifidni",0  
    parb8   db  "ifb",0  
    parb9   db  "ifnb",0  
    parb10  db  "ifdef",0  
    parb11  db  "ifndef",0  
    parb12  db  "endif",0  
    parb13  db  "else",0  
    parb14  db  "elseif",0  
    parb15  db  "elseife",0  
    parb16  db  "elseif1",0  
    parb17  db  "elseif2",0  
    parb18  db  "elseifdif",0  
    parb19  db  "elseifdifi",0  
    parb20  db  "elseifidn",0  
    parb21  db  "elseifidni",0  
    parb22  db  "elseifb",0  
    parb23  db  "elseifnb",0  
    parb24  db  "elseifdef",0  
    parb25  db  "elseifndef",0  

    stringtable parb, 26

    parp0   db  ".const",0
    parp1   db  ".data?",0
    parp2   db  ".data",0
    parp3   db  ".code",0
    parp4   db  ".if",0
    parp5   db  ".while",0
    parp6   db  ".repeat",0
    parp7   db  ".rept",0
    parp8   db  ".switch",0
    parp9   db  ".endif",0
    parp10  db  ".endw",0
    parp11  db  ".endsw",0
    parp12  db  ".until",0
    parp13  db  ".untilcxz",0
    parp14  db  ".elseif",0
    parp15  db  ".else",0
    parp16  db  ".case", 0
    parp17  db  ".default", 0

    stringtable parp, 18

    pard0   db  "ForLp",0
    pard1   db  "ForLpD",0
    pard2   db  "ForLpN",0
    pard3   db  "ForLpR",0
    pard4   db  "switch",0
    pard5   db  "Next",0
    pard6   db  "NextN",0
    pard7   db  "NextR",0
    pard8   db  "endsw",0
    pard9   db  "case",0
    pard10  db  "default",0

    stringtable pard, 11

    parr0   db  "@if",0
    parr1   db  "@while",0
    parr2   db  "@dothis",0
    parr3   db  "@endif",0
    parr4   db  "@endw",0
    parr5   db  "@whilethis",0
    parr6   db  "@elseif",0
    parr7   db  "@else",0

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


    ntokens     dd  0
    vtoken      xd  nStoredTokens dup (0)       ; values of tokens
    ltoken      xd  nStoredTokens dup (0)       ; token length
    ttoken      xd  nStoredTokens dup (0)       ; token type
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
