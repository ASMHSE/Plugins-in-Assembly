@echo off
if exist "Wrap.obj" del "Wrap.obj"
if exist "Wrap.dll" del "Wrap.dll"
\masm32\bin64\ml64 /c /D@ws=8 /Deditor=1 "Wrap.asm"
\masm32\bin64\Link /SUBSYSTEM:CONSOLE /entry:PluginEntryPoint /DLL /DEF:"Wrap_CE.def" "Wrap.obj" 
copy Wrap.dll \masm32\pluginsCE\Wrap.dll
del "Wrap.obj"
del "Wrap.exp"
dir "Wrap.*"
pause
