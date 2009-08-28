@echo off

if exist supaplex.exe del supaplex.exe

ml /nologo /c /Zd /Zi /coff supaplex.asm
cl /nologo /c /Zi supaplex_logic.cpp
windres -o supaplex.res -O res supaplex.rc
link /nologo /debug supaplex.obj supaplex_logic.obj supaplex.res gdi32.lib user32.lib

if exist supaplex.obj del supaplex.obj
if exist supaplex_logic.obj del supaplex_logic.obj
if exist supaplex.res del supaplex.res

supaplex
