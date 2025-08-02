; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
@ws equ 4 
include \masm32\include\neutral.inc 
include \masm32\macros\SmplMath\math.inc 
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
      DLLinstance dd ?

    ; ----------
    ; QE handles
    ; ----------
      hInstance dd ?
      hWnd dd ?
      hEdit dd ?
      hToolBar dd ?
      hStatus dd ?
      hMnu dd ?

.data 

include datacore.inc

.code 

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

LibMain proc instance : DWORD, reason : DWORD, unused : DWORD 

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

PiEntryPoint proc QEinterface : DWORD 

  ; -------------------------------
  ; get any required editor handles
  ; -------------------------------
    mov   hInstance, GethInstance() 
    mov   hWnd, GethWnd() 
    mov   hEdit, GethEdit() 
    mov   hToolBar, GethToolBar() 
    mov   hStatus, GethStatus() 
    mov   hMnu, GethMnu() 

    call  plugin_function 

    ret   

PiEntryPoint endp 

include codecore.inc 

end LibMain 
