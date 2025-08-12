@echo off
if exist "FormatAsm.obj" del "FormatAsm.obj"
if exist "FormatAsm.dll" del "FormatAsm.dll"
\masm32\bin64\ml64 /c /D@ws=8 /Deditor=0 "FormatAsm.asm"
\masm32\bin64\Link /SUBSYSTEM:CONSOLE /entry:LibMain /DLL /DEF:"FormatAsm_TE.def" "FormatAsm.obj" 
copy FormatAsm.dll \masm32\plugins64\FormatAsm.dll
del "FormatAsm.obj"
del "FormatAsm.exp"
dir "FormatAsm.*"
pause
