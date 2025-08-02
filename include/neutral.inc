; ==================================================================================================
; Title:      Neutral.inc
; Author:     Héctor S. Enrique
; Version:    2.0.0
; Purpose:    BNC implementation.
; Notes:      Version 1.0.0, July 2023
;               - First release.
;               Neutral Bitness Code (BNC) support for Masm32SDK and Masm64SDK
;               Adapted from ObjAsm-C.2 (Friedrich, et al.)
;               [ https://github.com/ObjAsm/ObjAsm-C.2 ]  
;             Version 2.0.0, August 2025
;                - Introduce @ws like control (will allow 16 bits apps)
; ==================================================================================================

;%echo @CatStr(%@ws)        ; must be decalred previously :)

    if @ws eq 8    
        @wsLog equ 3
    elseif @ws eq 4    
        @wsLog equ 2
    else
        @wsLog equ 1
    endif
@wl equ @wsLog

if @ws eq 8
    include \masm64\include64\masm64rt.inc

    xword textequ <qword>
    
    xax textequ <rax>
    xbx textequ <rbx>
    xcx textequ <rcx>
    xdx textequ <rdx>
    xsi textequ <rsi>
    xdi textequ <rdi>
    xsp textequ <rsp>
    xbp textequ <rbp>

    xd  textequ < dq>    
    @RegisterSH equ 3

else

    include \masm32\include\masm32rt.inc
    
    xword textequ <dword>

    xax textequ <eax>
    xbx textequ <ebx>
    xcx textequ <ecx>
    xdx textequ <edx>
    xsi textequ <esi>
    xdi textequ <edi>
    xsp textequ <esp>
    xbp textequ <ebp>

    xd  textequ <dd>    
    @RegisterSH equ 2
    
endif

    XWORD textequ <xword>
    POINTER textequ <xword>

@reg32_64 macro reg_32, reg_64
    if @ws eq 4
      @CatStr(<_>,&reg_32) textequ <reg_32>
      @CatStr(<__>,&reg_32) textequ <reg_32>
      @CatStr(<r_>,&reg_32) textequ <reg_32>
    else
      @CatStr(<_>,&reg_32) textequ <reg_64>
      @CatStr(<__>,&reg_32) textequ <@CatStr(&reg_64,<d>)>
      @CatStr(<r_>,&reg_32) textequ <reg_64>
    endif 
endm

@reg32_64d macro reg_32, reg_64
    if @ws eq 4
      @CatStr(<__>,&reg_32) textequ <reg_32>
    else
      @CatStr(<__>,&reg_32) textequ <reg_64>
    endif 
endm

    @if64bits macro RegAux
        if @ws eq 4
            exitm <>
        else
            %exitm <&RegAux>
        endif    
    endm
    @ifno64bits macro RegAux
        if @ws eq 4
            %exitm <&RegAux>
        else
            exitm <>
        endif    
    endm


    @reg32_64 ecx, r10
    @reg32_64 edx, r11
    @reg32_64 eax, r12


    XPOINT struct
        x xword ?
        y xword ?
    XPOINT ends


neutralend macro arg1
    if @ws eq 4
       % exitm <end &arg1>
    else
       exitm <end>
    endif 
endm

; For ML ·················································································

if @ws eq 4

  ; --------------------------------------------------
  ; conout supports a variable number of entries
  ; including quoted text, lf tab and quote characters
  ;                            Adapted from MASM64 SDK
  ; --------------------------------------------------
    conout MACRO args:VARARG
      FOR var,<args>
        IFIDN <var>,<lf>        ;; line feed
          print chr$(13,10)
        ELSEIFIDN <var>,<tab>   ;; tab
          print chr$(9)
        ELSEIFIDN <var>,<qt>    ;; double quote "
          print chr$(34)
        ELSE
          print reparg(var)    ;; quoted text or normal text address
        ENDIF
      ENDM
    ENDM

    waitkey textequ <inkey>

  ; -------------------------------------------------------
  ; Register preservation macros for stack frame procedures
  ;                                 Adapted from MASM64 SDK
  ; -------------------------------------------------------
    MLOCAL equ LOCAL

    USING MACRO arglist:VARARG                      ;; Must be placed before dynamic code
      MLOCAL r64@@_@@[argcount(arglist)] :DWORD     ;; LOCAL QWORD array
      reg_64_bit_list_@@@@ equ arglist
    ENDM

    SaveRegs MACRO                                  ;; must be placed AFTER LOCALs
      cntr@@@_@@@ = 0
      %FOR arg,<reg_64_bit_list_@@@@>
        ;; %echo arg
        mov r64@@_@@[cntr@@@_@@@], arg
        cntr@@@_@@@ = cntr@@@_@@@ + 4
      ENDM
    ENDM

    RestoreRegs MACRO                               ;; must be placed BEFORE "ret"
      cntr@@@_@@@ = 0
      %FOR arg,<reg_64_bit_list_@@@@>
        ;; %echo arg
        mov arg, r64@@_@@[cntr@@@_@@@]
        cntr@@@_@@@ = cntr@@@_@@@ + 4
      ENDM
    ENDM

else
    exit textequ <.exit>

    cls MACRO                       ;; clear screen
      invoke ClearScreen
    ENDM



endif    