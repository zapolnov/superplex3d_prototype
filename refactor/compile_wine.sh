#!/bin/sh

rm -f supaplex.exe

wine ../ml.exe /nologo /Zi /Zd /c /coff supaplex.asm
i586-mingw32msvc-windres -O coff -o supaplex.res supaplex.rc
i586-mingw32msvc-g++ -fno-leading-underscore -g -DUSE_GLUT -I. -o supaplex.exe supaplex_logic.cpp BMPLoader.cpp supaplex.obj supaplex.res -lgdi32 -lopengl32 GL/glut32.lib -lwinmm
rm -f supaplex.obj
rm -f supaplex.res

wine supaplex.exe
