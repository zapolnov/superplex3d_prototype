@echo off
set PATH=TC
if exist decomp.exe del decomp.exe
TC\tcc.exe -1 -G -ITC\INCLUDE -LTC\LIB -N -O -Z -a -d -f87 -ms -r decomp.c
if exist decomp.obj del decomp.obj
