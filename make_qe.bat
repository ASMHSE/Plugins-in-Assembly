@echo off
if exist "FormatAsm.obj" del "FormatAsm.obj"
if exist "FormatAsm.dll" del "FormatAsm.dll"
\masm32\bin\ml /c /coff /D@ws=4 /Deditor=0 "FormatAsm.asm"
\masm32\bin\Link /SUBSYSTEM:CONSOLE  /entry:LibMain /DLL /DEF:"FormatAsm_qe.def" "FormatAsm.obj" 
copy FormatAsm.dll \masm32\plugins\FormatAsm.dll
del "FormatAsm.obj"
del "FormatAsm.exp"
dir "FormatAsm.*"
pause
