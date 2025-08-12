@echo off
if exist "UnWrap.obj" del "UnWrap.obj"
if exist "UnWrap.dll" del "UnWrap.dll"
\masm32\bin64\ml64 /c /D@ws=8 /Deditor=1 "UnWrap.asm"
\masm32\bin64\Link /SUBSYSTEM:CONSOLE /entry:PluginEntryPoint /DLL /DEF:"UnWrap_CE.def" "UnWrap.obj" 
copy UnWrap.dll \masm32\pluginsCE\UnWrap.dll
del "UnWrap.obj"
del "UnWrap.exp"
dir "UnWrap.*"
pause
