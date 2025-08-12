@echo off
if exist "Wrap.obj" del "Wrap.obj"
if exist "Wrap.dll" del "Wrap.dll"
\masm32\bin64\ml64 /c /D@ws=8 /Deditor=0 "Wrap.asm"
\masm32\bin64\Link /SUBSYSTEM:CONSOLE /entry:LibMain /DLL /DEF:"Wrap_TE.def" "Wrap.obj" 
copy Wrap.dll \masm32\plugins64\Wrap.dll
del "Wrap.obj"
del "Wrap.exp"
dir "Wrap.*"
pause
