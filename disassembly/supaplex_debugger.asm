
		.386

EXE_HEADER struc
mz db 2 dup(?)
lastPageSize dw ? ; Number of bytes in last 512-byte page of executable
totalPages dw ? ; Total number of 512-byte pages in executable (including the last page)
numRelocEntries dw ? ; Number of relocation entries
headerSize dw ? ; Header size in paragraphs (one paragraph = 10h)
minMemory dw ? ; Minimum paragraphs of memory allocated in addition to the code size
maxMemory dw ? ; Maximum paragraphs of memory allocated in addition to the code size
relativeSS dw ? ; Initial SS relative to start of executable
initialSP dw ? ; Initial SP
checksum dw ? ; Checksum
initialIP dw ? ; Offset of the entry point
relativeCS dw ? ; CS relative to start of executable
relocTable dw ? ; Offset of relocation table
overlayNumber dw ? ; Overlay number (0 = main program)
EXE_HEADER ends

; =========================================================================

CODE_SEG segment para public use16

		assume ds:CODE_SEG, cs:CODE_SEG, es:CODE_SEG, ss:STACK_SEG

; -------------------------------------------------------------------------

main	proc near

		mov ax, cs
		mov ds, ax
		mov es, ax

		mov al, 0CDh
		mov di, offset SavedIntCD
		mov si, offset intCD
		call replace_int_vector

		mov	ax, 3C00h
		mov	cx, 0
		mov	dx, offset LOG_FILE_NAME
		int	21h
		jc dos_error
		mov logFileHandle, ax

		mov ax, seg SUPAPLEX_SEG
		mov si, offset msgSupaplexLoadSeg
		push ax
		call my_fprint
		add sp, 2

		call load_supaplex

		push es
		mov ax, seg SUPAPLEX_SEG
		mov es, ax
		assume es:SUPAPLEX_SEG

		mov ax, word ptr es:[EXE_HEADER.mz]
		cmp ax, 'ZM'
		jne not_an_exe

		mov ax, word ptr es:[EXE_HEADER.headerSize]
		add word ptr es:[EXE_HEADER.relativeSS], ax
		add word ptr es:[EXE_HEADER.relativeCS], ax
		mov ax, seg SUPAPLEX_SEG
		add word ptr es:[EXE_HEADER.relativeSS], ax
		add word ptr es:[EXE_HEADER.relativeCS], ax

mov ax, word ptr es:[EXE_HEADER.relativeCS]
mov fs, ax
;mov byte ptr fs:[26BFh], 0C3h ;2238h

		mov si, offset msgHorizontalLine
		call my_fprint
		mov ax, word ptr es:[EXE_HEADER.headerSize]
		shl ax, 4
		push ax
		mov si, offset msgHeaderSize
		call my_fprint
		push word ptr es:[EXE_HEADER.totalPages]
		mov si, offset msgTotalPages
		call my_fprint
		push word ptr es:[EXE_HEADER.lastPageSize]
		mov si, offset msgLastPageSize
		call my_fprint
		push word ptr es:[EXE_HEADER.numRelocEntries]
		mov si, offset msgNumRelocs
		call my_fprint
		push word ptr es:[EXE_HEADER.relocTable]
		mov si, offset msgRelocsOfs
		call my_fprint
		push word ptr es:[EXE_HEADER.initialSP]
		push word ptr es:[EXE_HEADER.relativeSS]
		mov si, offset msgInitialStack
		call my_fprint
		push word ptr es:[EXE_HEADER.initialIP]
		push word ptr es:[EXE_HEADER.relativeCS]
		mov si, offset msgEntryPoint
		call my_fprint
		mov si, offset msgHorizontalLine
		call my_fprint
		add sp, 16

		mov cx, word ptr es:[EXE_HEADER.numRelocEntries]
		jcxz @@done_reloc
		mov si, word ptr es:[EXE_HEADER.relocTable]
		mov bx, seg SUPAPLEX_SEG
		add bx, word ptr es:[EXE_HEADER.headerSize]
@@apply_reloc:
		mov di, word ptr es:[si]
		mov ax, word ptr es:[si + 2]
		add ax, bx
		push es
		assume es:nothing
		mov es, ax
		add es:[di], bx
		pop es
		add si, 4
		assume es:SUPAPLEX_SEG
		loop @@apply_reloc
@@done_reloc:

		pop es
		assume es:CODE_SEG

		mov al, 21h
		mov di, offset SavedInt21
		mov si, offset int21
		call replace_int_vector

		mov al, 10h
		mov di, offset SavedInt10
		mov si, offset int10
		call replace_int_vector

		mov al, 3h
		mov di, offset SavedInt3
		mov si, offset int03
		call replace_int_vector

		mov al, 1h
		mov di, offset SavedInt1
		mov si, offset int01
		call replace_int_vector

		call set_breakpoints

		mov si, offset msgCallingProgram
		call my_fprint

		mov ax, seg SUPAPLEX_SEG
		mov es, ax
		assume es:SUPAPLEX_SEG

		jmp dword ptr es:[EXE_HEADER.initialIP]

main	endp

; -------------------------------------------------------------------------

exit_application proc

		mov al, 21h
		mov si, offset SavedInt21
		call restore_int_vector

		mov al, 10h
		mov si, offset SavedInt10
		call restore_int_vector

		mov al, 3h
		mov si, offset SavedInt3
		call restore_int_vector

		mov al, 1h
		mov si, offset SavedInt1
		call restore_int_vector

		mov ax, 3E00h
		mov bx, logFileHandle
		int 21h

		mov al, 0CDh
		mov si, offset SavedIntCD
		call restore_int_vector

		mov ax, 4C00h
		int 21h

exit_application endp

; -------------------------------------------------------------------------

set_breakpoints proc

		push es
		mov ax, SUPAPLEX_SEG
		mov es, ax
		assume es:SUPAPLEX_SEG
		mov ax, word ptr es:[EXE_HEADER.relativeCS]
		mov es, ax
		assume es:nothing

		mov si, offset breakpoints
		cld
@@next_breakpoint:
		lodsw
		cmp ax, 0FFFFh
		je short @@done_breakpoints
		mov bx, ax
		add si, 4

		mov al, 0CCh
		xchg al, es:[bx]
		mov [si-1], al

		jmp short @@next_breakpoint

@@done_breakpoints:

		pop es
		assume es:CODE_SEG
		ret

set_breakpoints endp

; -------------------------------------------------------------------------

replace_int_vector proc

		push es
		push ds

		push ax
		mov ah, 35h
		int 21h
		pop ax

		mov cs:[di], bx
		mov cs:[di + 2], es

		mov bx, cs
		mov ds, bx
		mov dx, si

		mov ah, 25h
		int 21h

		pop ds
		pop es

		ret

replace_int_vector endp

; -------------------------------------------------------------------------

restore_int_vector proc

		push es
		push ds

		mov ah, 25h
		mov dx, [si]
		mov ds, [si + 2]
		int 21h

		pop ds
		pop es

		ret

restore_int_vector endp

; -------------------------------------------------------------------------

intCD proc

		iret

intCD endp

; -------------------------------------------------------------------------

int01 proc

		push bp
		mov bp, sp
		and word ptr ss:[bp+6], 0FEFFh	; Reset the instruction trap flag

		push bx
		push ds

		lds bx, cs:[LastBreakpoint]
		mov byte ptr ds:[bx], 0CCh
		pop ds

		pop bx
		pop bp

		iret

int01 endp

; -------------------------------------------------------------------------

int03 proc

		push ax
		push bx
		push cx
		push dx
		push si
		push di
		push bp

		mov bp, sp
		dec word ptr ss:[bp+14]
		mov bx, ss:[bp+14]
		mov dx, ss:[bp+16]
		or word ptr ss:[bp+18], 100h	; Set the instruction trap flag

		push ds
		mov ax, cs
		mov ds, ax

		push es
		mov ax, SUPAPLEX_SEG
		mov es, ax
		assume es:SUPAPLEX_SEG
		mov ax, word ptr es:[EXE_HEADER.relativeCS]
		mov es, ax
		assume es:nothing

		mov si, offset breakpoints
		cld
@@next_breakpoint:
		lodsw
		cmp ax, 0FFFFh
		je short @@done_breakpoints
		cmp ax, bx
		je short @@found_breakpoint
		add si, 4
		jmp short @@next_breakpoint
@@found_breakpoint:
		mov es, dx
		mov al, [si+3]
		mov es:[bx], al
		mov word ptr [LastBreakpoint], bx
		mov word ptr [LastBreakpoint + 2], es
		push word ptr [si]
		push bx
		push dx
		mov si, offset msgBreakpointHit
		call my_fprint
		add sp, 6
@@done_breakpoints:

		pop es
		assume es:CODE_SEG
		pop ds
		assume ds:CODE_SEG

		pop bp
		pop di
		pop si
		pop dx
		pop cx
		pop bx
		pop ax

		iret

int03 endp

; -------------------------------------------------------------------------

int10 proc

		jmp dword ptr cs:[SavedInt10]

int10 endp

; -------------------------------------------------------------------------

int21 proc

		cmp ah, 4Ch
		jne short @@call_old_handler

		mov ax, cs
		mov ds, ax
		mov es, ax

		mov si, offset msgSupaplexExited
		call my_fprint

		jmp exit_application

@@call_old_handler:
		jmp dword ptr cs:[SavedInt21]

int21 endp

; -------------------------------------------------------------------------

load_supaplex proc

		mov ax, 3D00h
		mov dx, offset SUPAPLEX_EXE_NAME
		int 21h
		jc dos_error
		mov exeFileHandle, ax

		mov bx, seg SUPAPLEX_SEG
		xor dx, dx

@@load_next_chunk:

		push dx
		push bx
		push ds
		mov ds, bx
		assume ds:nothing
		mov ax, 3F00h
		mov bx, exeFileHandle
		mov cx, 1000h
		int 21h
		pop ds
		assume ds:CODE_SEG
		pop bx
		pop dx
		jc dos_error
		or ax, ax
		jz short @@load_done
		mov cx, ax
		shr cx, 4
		add bx, cx
		and ax, 0Fh
		add dx, ax
		jmp short @@load_next_chunk

@@load_done:

		mov ax, 3E00h
		mov bx, exeFileHandle
		int 21h

		ret

load_supaplex endp

; -------------------------------------------------------------------------

not_an_exe:
		mov dx, offset msgNotAnExe
		jmp fatal_exit

dos_error:
		mov dx, offset msgDosError
		jmp fatal_exit

fatal_exit:
		mov ax, cs
		mov ds, ax
		mov es, ax
		assume ds:CODE_SEG, es:CODE_SEG

		mov ah, 9
		int 21h

		mov ax, 4C01h
		int 21h

; -------------------------------------------------------------------------

my_fprint proc

		push bp
		mov bp, sp
		add bp, 4
		push es
		push ds
		mov ax, cs
		mov ds, ax
		mov es, ax
		assume cs:CODE_SEG,ds:CODE_SEG
		mov di, offset LOG_BUFFER
		xor cx, cx
		cld
@@1:	lodsb
		or al, al
		jz @@done
		cmp al,'%'
		je short @@format
		stosb
		inc cx
		jmp short @@1
@@format:
		lodsb
		cmp al,'x'
		je short @@hex
		cmp al,'d'
		je short @@dec
		cmp al,'s'
		je short @@str
		push ax
		mov al,'%'
		stosb
		pop ax
		stosb
		add cx, 2
		jmp short @@1
@@hex:	push cx
		mov ax, ss:[bp]
		add bp, 2
		mov cl, 12
@@hex_1:mov bx, ax
		shr bx, cl
		and bx, 0Fh
		mov ch, [hexchars + bx]
		mov [di], ch
		inc di
		or cl,cl
		jz short @@hex_2
		sub cl, 4
		jmp short @@hex_1
@@hex_2:pop cx
		add cx, 4
		jmp short @@1
@@dec:	push cx
		mov ax, ss:[bp]
		add bp, 2
		mov bx, offset INT_BUFFER + 31
@@dec_1:xor dx, dx
		mov cx, 10
		div cx
		add dl,'0'
		mov [bx], dl
		dec bx
		or ax, ax
		jnz short @@dec_1
		pop cx
		inc bx
@@dec_2:cmp bx, offset INT_BUFFER + 32
		je short @@1
		mov al, [bx]
		stosb
		inc bx
		inc cx
		jmp short @@dec_2
@@str:	mov bx, ss:[bp]
		add bp, 2
@@str_1:mov al, [bx]
		or al, al
		jz @@1
		stosb
		inc bx
		inc cx
		jmp short @@str_1
@@done:	mov ax, 4000h
		mov dx, offset LOG_BUFFER
		mov bx, seg SUPAPLEX_SEG
		mov es, bx
		add bx, word ptr es:[EXE_HEADER.headerSize]
;		mov bx, logFileHandle
		int 0CDh
		jc dos_error

		pop ds
		pop es
		pop bp

		ret

my_fprint endp

; -------------------------------------------------------------------------

LOG_FILE_NAME db "supaplex.log",0
SUPAPLEX_EXE_NAME db "my.exe",0

msgDosError db "DOS error",13,10,'$',0
msgNotAnExe db "Not an MZ file",13,10,'$',0

msgSupaplexLoadSeg db "Supaplex load seg: %x",10,0
msgCallingProgram db "Starting the program...",10,0
msgSupaplexExited db "Program has called INT 21 AH=4C",10,0
msgBreakpointHit db "Breakpoint hit at %x:%x (%s)",10,0

msgHorizontalLine db "------------------------------",10,0
msgHeaderSize db "Header size:    %d bytes",10,0
msgTotalPages db "Total pages:    %d",10,0
msgLastPageSize db "Last page size: %d bytes",10,0
msgNumRelocs db "Num relocs:     %d",10,0
msgRelocsOfs db "Relocs ofs:     %x",10,0
msgInitialStack db "Initial stack:  %x:%x",10,0
msgEntryPoint db "Entry point:    %x:%x",10,0

hexchars db '01234567890ABCDEF'

; Format:
;	dw OFFSET
;	dw NAME_OFFSET
;	db SAVED_BYTE

breakpoints dw 0, offset funcMain, 0
;			dw 03477h, offset func3477, 0
;			dw 05ABEh, offset func5ABE, 0
;			dw 05AFEh, offset func5AFE, 0
;			dw 05B27h, offset func5B27, 0
;			dw 05B90h, offset func5B90, 0
;			dw 05BF9h, offset func5BF9, 0
;			dw 05C62h, offset func5C62, 0
;			dw 05CCBh, offset func5CCB, 0
;			dw 05D35h, offset func5D35, 0
;			dw 05D9Eh, offset func5D9E, 0
;			dw 05E10h, offset func5E10, 0
;			dw 05E40h, offset func5E40, 0
;			dw 06F5Eh, offset func6F5E, 0
;			dw 07495h, offset func7495, 0
;			dw 074B7h, offset func74B7, 0
;			dw 07570h, offset func7570, 0
;			dw 075B0h, offset func75B0, 0
;			dw 075D2h, offset func75D2, 0
			dw 0FFFFh

funcMain db 'main',0
func3477 db 'MainMenu_xxx',0
func5ABE db 'SND_zzz',0
func5AFE db 'SND_qqq',0
func5B27 db 'SND_ttt',0		; play bang sound?
func5B90 db 'SND_ppp',0
func5BF9 db 'SND_mmm',0
func5C62 db 'SND_nnn',0		; play infotron fall sound?
func5CCB db 'SND_aaa',0
func5D35 db 'SND_bbb',0
func5D9E db 'SND_ccc',0
func5E10 db 'SND_ddd',0		; update sound?
func5E40 db 'sub_3CA60',0	; main loop?
func6F5E db 'sub_3DB7E',0
func7495 db 'sub_3E0B5',0
func74B7 db 'sub_3E0D7',0
func7570 db 'sub_3E190',0
func75B0 db 'sub_3E1D0',0
func75D2 db 'sub_3E1F2',0

LastBreakpoint dd ?

logFileHandle dw ?
exeFileHandle dw ?

SavedInt1 dd ?
SavedInt3 dd ?
SavedInt10 dd ?
SavedInt21 dd ?
SavedIntCD dd ?

INT_BUFFER db 32 dup(?)
LOG_BUFFER db 8192 dup(?)

CODE_SEG ends

; =========================================================================

STACK_SEG segment byte stack use16
		db 128 dup(?)
STACK_SEG ends

; =========================================================================

SUPAPLEX_SEG segment para public use16
SUPAPLEX_SEG ends

		end main
