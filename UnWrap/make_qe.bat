@echo off
if exist "UnWrap.obj" del "UnWrap.obj"
if exist "UnWrap.dll" del "UnWrap.dll"
\masm32\bin\ml /c /coff /D@ws=4 /Deditor=0 "UnWrap.asm"
\masm32\bin\Link /SUBSYSTEM:CONSOLE  /entry:LibMain /DLL /DEF:"UnWrap_qe.def" "UnWrap.obj" 
copy UnWrap.dll \masm32\plugins\UnWrap.dll
del "UnWrap.obj"
del "UnWrap.exp"
dir "UnWrap.*"
pause
