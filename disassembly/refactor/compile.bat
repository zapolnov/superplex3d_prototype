@echo off

if exist supaplex.exe del supaplex.exe

ml /nologo /Zi /Zd /c /coff supaplex.asm
windres -O coff -o supaplex.res supaplex.rc
cl /c /Zi /EHsc /DWIN32 /nologo /DUSE_GLUT /I. supaplex_logic.cpp BMPLoader.cpp
link /DEBUG /nologo supaplex.obj supaplex_logic.obj BMPLoader.obj supaplex.res gdi32.lib opengl32.lib GL/glut32.lib winmm.lib user32.lib
if exist supaplex.obj del supaplex.obj
if exist supaplex_logic.obj del supaplex_logic.obj
if exist BMPLoader.obj del BMPLoader.obj

if exist supaplex.res del supaplex.res

supaplex.exe
