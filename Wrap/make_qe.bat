@echo off
if exist "Wrap.obj" del "Wrap.obj"
if exist "Wrap.dll" del "Wrap.dll"
\masm32\bin\ml /c /coff /D@ws=4 /Deditor=0 "Wrap.asm"
\masm32\bin\Link /SUBSYSTEM:CONSOLE  /entry:LibMain /DLL /DEF:"Wrap_qe.def" "Wrap.obj" 
copy Wrap.dll \masm32\plugins\Wrap.dll
del "Wrap.obj"
del "Wrap.exp"
dir "Wrap.*"
pause
