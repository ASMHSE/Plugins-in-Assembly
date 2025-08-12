@echo off
if exist "UnWrap.obj" del "UnWrap.obj"
if exist "UnWrap.dll" del "UnWrap.dll"
\masm32\bin64\ml64 /c /D@ws=8 /Deditor=0 "UnWrap.asm"
\masm32\bin64\Link /SUBSYSTEM:CONSOLE /entry:LibMain /DLL /DEF:"UnWrap_TE.def" "UnWrap.obj" 
copy UnWrap.dll \masm32\plugins64\UnWrap.dll
del "UnWrap.obj"
del "UnWrap.exp"
dir "UnWrap.*"
pause
