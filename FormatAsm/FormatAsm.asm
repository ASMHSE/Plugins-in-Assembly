; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
if    @ws eq 8 
    ifndef @WordSize 
        @WordSize equ 8 
    endif 
endif 
AllowPromotions equ 1 
include \masm32\include\neutral.inc 
include \masm32\macros\SmplMath\math.inc 
if    @ws eq 4 
;    include \masm32\include\debug.inc
;    includelib \masm32\lib\debug.lib
endif 

@reg32_64 edx, r11

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

  ; **********************
  ; handles for QE windows
  ; **********************
GethInstance MACRO 
    push  0 
    call  QEinterface 
    EXITM <eax> 
ENDM  

GethWnd MACRO 
    push  1 
    call  QEinterface 
    EXITM <eax> 
ENDM  

GethEdit MACRO 
    push  2 
    call  QEinterface 
    EXITM <eax> 
ENDM  

GethToolBar MACRO 
    push  4 
    call  QEinterface 
    EXITM <eax> 
ENDM  

GethStatus MACRO 
    push  5 
    call  QEinterface 
    EXITM <eax> 
ENDM  

GethMnu MACRO 
    push  6 
    call  QEinterface 
    EXITM <eax> 
ENDM  

.data? 
    ; -------------------------
    ; local DLL instance handle
    ; -------------------------
      DLLinstance xd ?

    ; ----------
    ; QE handles
    ; ----------
      hInstance xd ?
      hWnd      xd ?
      hEdit     xd ?
      hToolBar  xd ?
      hStatus   xd ?
      hMenu     xd ?
      hIcon     xd ?

.data 

include datacore.inc

if @ws eq 8
    switch  textequ <.switch>
    case    textequ <.case>
    endsw   textequ <.endsw>
    free    textequ <mfree>
endif

.code 

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
if    editor eq 1 
PluginEntryPoint proc instance : QWORD, reason : QWORD, unused : QWORD 

    .if reason == DLL_PROCESS_ATTACH 
        mrm   hInstance, instance ; copy stack arg to global
        mov   rax, TRUE ; return TRUE so DLL will start
    .endif 

    ret   

PluginEntryPoint endp 

    ; «»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»

piInterface proc hWin : QWORD, Menu : QWORD, Edit : QWORD, tbar : QWORD, sbar : QWORD 

      ; ------------------------------------
      ; load arguments into GLOBAL variables
      ; ------------------------------------
    mrm   hWnd, hWin 
    mrm   hMenu, Menu 
    mrm   hEdit, Edit 
    mrm   hToolBar, tbar 
    mrm   hStatus, sbar 

    call  plugin_function 

    ret   

piInterface endp 

else  

LibMain proc instance : XWORD, reason : XWORD, unused : XWORD 

    switch reason 
        case  DLL_PROCESS_ATTACH 
            mrm   DLLinstance, instance 
            mov   eax, TRUE 

        case  DLL_PROCESS_DETACH 
            nop   

        case  DLL_THREAD_ATTACH 
            nop   

        case  DLL_THREAD_DETACH 
            nop   

    endsw 

    ret   

LibMain endp 

    ; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
if    @ws eq 4 
PiEntryPoint proc QEinterface : DWORD 

          ; -------------------------------
          ; get any required editor handles
          ; -------------------------------
    mov   hInstance, GethInstance() 
    mov   hWnd, GethWnd() 
    mov   hEdit, GethEdit() 
    mov   hToolBar, GethToolBar() 
    mov   hStatus, GethStatus() 
    mov   hMenu, GethMnu() 

    call  plugin_function 

    ret   

PiEntryPoint endp 
else  
Plugin_Interface proc \ 
    Instance :QWORD, \ 
    Wnd   :QWORD, \ 
    Menu  :QWORD, \ 
    ToolBar :QWORD, \ 
    Status :QWORD, \ 
    Edit  :QWORD, \ 
    Icon  :QWORD 

          ; ---------------------------
          ; load arguments into globals
          ; ---------------------------
    mrm   hInstance, Instance 
    mrm   hWnd, Wnd 
    mrm   hMenu, Menu 
    mrm   hToolBar, ToolBar 
    mrm   hStatus, Status 
    mrm   hEdit, Edit 
    mrm   hIcon, Icon 

    call  plugin_function 

    ret   
Plugin_Interface endp 
endif 
endif 

include codecore.inc 

end 
