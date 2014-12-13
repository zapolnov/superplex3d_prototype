@echo off

sed -e "s/KeyboardState.key/KeyboardState + KEYBOARD_STATE.key/g" -i supaplex.asm
sed -e "s/KeyboardState.unused/KeyboardState + KEYBOARD_STATE.unused/g" -i supaplex.asm
sed -e "s/CODE_SEG	segment	byte/CODE_SEG	segment	para/g" -i supaplex.asm
sed -e "s/ClickableAreas.handler/ClickableAreas + ON_SCREEN_CLICKABLE.handler/g" -i supaplex.asm

del sed??????

if exist supaplex.exe del supaplex.exe
if exist my.exe del my.exe
ml.exe /nologo supaplex.asm
if exist supaplex.obj del supaplex.obj
if exist C:\SUPAPLEX\SUPAPLEX\my.exe del C:\SUPAPLEX\SUPAPLEX\my.exe
if exist supaplex.exe move supaplex.exe C:\SUPAPLEX\SUPAPLEX\my.exe

if exist supaplex_debugger.exe del supaplex_debugger.exe
if exist dbg.exe del dbg.exe
ml.exe /nologo supaplex_debugger.asm
if exist C:\SUPAPLEX\SUPAPLEX\dbg.exe del C:\SUPAPLEX\SUPAPLEX\dbg.exe
if exist supaplex_debugger.exe move supaplex_debugger.exe C:\SUPAPLEX\SUPAPLEX\dbg.exe
if exist supaplex_debugger.obj del supaplex_debugger.obj
