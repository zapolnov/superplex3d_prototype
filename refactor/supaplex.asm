
public _LevelMap
public _LoadLevelMap
public _FindPlayerPositionOnLevelMap
public _InitRandomNumberGenerator
public _InitPlayerState
public _sub_392DF
public _BeginLevel
public _RunTheLevel
public _JoystickButtons



SPECIAL_PORT	struc ;	(sizeof=0X6)
coords		dw ?
gravity		db ?
freeze_zonks	db ?
freeze_enemies	db ?
unused		db ?
SPECIAL_PORT	ends



; enum MAP_ELEMENT
MAP_SPACE	 = 0
MAP_ZONK	 = 1
MAP_BASE	 = 2
MAP_MURPHY	 = 3
MAP_INFOTRON	 = 4
MAP_RAM_CHIP_SQUARE  = 5
MAP_GRAY_DENTED_PYRAMID	 = 6
MAP_EXIT	 = 7
MAP_DISK_ORANGE	 = 8
MAP_PORT_LEFT_TO_RIGHT	= 9
MAP_PORT_UP_TO_DOWN  = 0Ah
MAP_PORT_RIGHT_TO_LEFT	= 0Bh
MAP_PORT_DOWN_TO_UP  = 0Ch
MAP_PORT_SPECIAL_LEFT_TO_RIGHT	= 0Dh
MAP_PORT_SPECIAL_UP_TO_DOWN  = 0Eh
MAP_PORT_SPECIAL_RIGHT_TO_LEFT	= 0Fh
MAP_PORT_SPECIAL_DOWN_TO_UP  = 10h
MAP_SNIK_SNAK	 = 11h
MAP_DISK_YELLOW	 = 12h
MAP_TERMINAL	 = 13h
MAP_DISK_RED	 = 14h
MAP_PORT_VERTICAL  = 15h
MAP_PORT_HORIZONTAL  = 16h
MAP_PORT_CROSS	 = 17h
MAP_ELECTRON	 = 18h
MAP_BUG		 = 19h
MAP_RAM_CHIP_LEFT  = 1Ah
MAP_RAM_CHIP_RIGHT  = 1Bh
MAP_HARDWARE_1	 = 1Ch
MAP_GREEN_LAMP	 = 1Dh
MAP_BLUE_LAMP	 = 1Eh
MAP_RED_LAMP	 = 1Fh
MAP_YELLOW_BLACK_DIAGONAL  = 20h
MAP_HARDWARE_2	 = 21h
MAP_HARDWARE_3	 = 22h
MAP_HARDWARE_4	 = 23h
MAP_HARDWARE_5	 = 24h
MAP_HARDWARE_6	 = 25h
MAP_RAM_CHIP_TOP  = 26h
MAP_RAM_CHIP_BOTTOM  = 27h
MAP_INVISIBLE_WALL  = 28h



; enum MAP_DIMENSIONS
MAP_HEIGHT	 = 24
MAP_WIDTH	 = 60
MAP_NUM_CELLS	 = 1440

.386
.model flat
.code

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

handleZonk	proc near

			cmp		byte ptr _LevelMap[esi], MAP_ZONK
			je		short @@1
			ret
@@1:		mov		ax,	word ptr _LevelMap[esi]
			cmp		ax, 1
			je		short @@2
			jmp		@@12
@@2:		cmp		byte_40325, 2
			jne		short @@3
			ret
@@3:		mov		ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
			cmp		ax, 0
			je		short @@6
			cmp		ax, 1
			je		short @@4
			cmp		ax, 4
			je		short @@4
			cmp		ax, 5
			je		short @@4
			ret
@@4:		cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
			je		short @@7
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short @@7
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short @@7
@@5:		cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
			je		short @@9
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short @@9
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short @@9
			ret
@@6:		mov		byte ptr _LevelMap[esi + 1], 64
			jmp		short @@12
@@7:		cmp		word ptr _LevelMap[esi], 0
			je		short @@8
			jmp		short @@5
@@8:		mov		byte ptr _LevelMap[esi + 1], 80
			mov		word ptr _LevelMap[esi - 2], 8888h
			jmp		short @@12
@@9:		cmp		word ptr _LevelMap[esi + 2], 0
			je		short @@11
			cmp		word ptr _LevelMap[esi + 2], 9999h
			jne		short @@10
			cmp		byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], 1
			jz		short @@11
@@10:		ret
@@11:		mov		byte ptr _LevelMap[esi + 1], 96
			mov		word ptr _LevelMap[esi + 2], 8888h
			jmp		short @@12
@@12:		mov		bl, byte ptr _LevelMap[esi + 1]
			xor		bh, bh
			mov		al, bl
			and		al, 0F0h
			cmp		al, 10h
			jne		short @@13
			jmp		short @@21
@@13:		cmp		al, 20h
			jne		short @@14
			jmp		loc_37BA3
@@14:		cmp		al, 30h
			jne		short @@15
			jmp		loc_37C08
@@15:		cmp		byte_40325, 2
			jne		short @@16
			ret
@@16:		cmp		al, 40h
			jne		short @@17
			jmp		loc_37C6D
@@17:		cmp		al, 50h
			jne		short @@18
			jmp		loc_37C9B
@@18:		cmp		al, 60h
			jne		short @@19
			jmp		loc_37D09
@@19:		cmp		al, 70h
			jne		short @@20
			jmp		loc_37D77
@@20:		ret
@@21:		xor		bh, bh
			shl		bl, 1
			shl		bl, 1
			and		bl, 1Fh
			;mov		di, [esi+6053h]
			;add		di, [bx+6C0Bh]
			;push	esi
			;mov		esi, word_40892
			;push	ds
			;mov		ax, es
			;mov		ds, ax
			;mov		cx, 12h
;loc_37A26:	movsb
			;movsb
			;add		esi, 78h	; 'x'
			;add		di, 78h	; 'x'
			;loop	loc_37A26
			;pop		ds
			;pop		esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 16h
			jnz		short loc_37A4B
			mov		byte ptr _LevelMap[esi + 1], bl
			sub		esi, 2 * MAP_WIDTH
			call	sub_3826F
			add		esi, 2 * MAP_WIDTH
			ret
loc_37A4B:	cmp		bl, 18h
			jge		short loc_37A55
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37A55:	mov		byte ptr _LevelMap[esi + 1], 0
			cmp		byte_40325, 2
			jne		short loc_37A62
			ret
loc_37A62:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
			jne		short loc_37A6C
			jmp		loc_37AFD
loc_37A6C:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			jne		short loc_37A77
			jmp		loc_37AFD
loc_37A77:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
			jne		short loc_37A81
			jmp		loc_37B34
loc_37A81:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 11h
			jne		short loc_37A8B
			jmp		loc_37B8E
loc_37A8B:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 2BBh
			jne		short loc_37A96
			jmp		loc_37B56
loc_37A96:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 4BBh
			jne		short loc_37AA1
			jmp		loc_37B72
loc_37AA1:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 18h
			jne		short loc_37AAB
			jmp		loc_37B8E
loc_37AAB:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 8
			jne		short loc_37AB5
			jmp		loc_37B95
loc_37AB5:	;call	SND_nnn
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 1
			je		short loc_37ACE
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 4
			je		short loc_37ACE
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 5
			je		short loc_37ACE
			ret
loc_37ACE:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
			je		short loc_37B0A
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short loc_37B0A
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short loc_37B0A
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0
			je		short loc_37B20
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short loc_37B20
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short loc_37B20
			ret
loc_37AFD:	mov		word ptr _LevelMap[esi], 7001h
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_37B0A:	cmp		word ptr _LevelMap[esi - 2], 0
			je		short loc_37B14
			jmp		@@5
loc_37B14:	mov		byte ptr _LevelMap[esi + 1], 50h
			mov		word ptr _LevelMap[esi - 2], 8888h
			ret
loc_37B20:	cmp		word ptr _LevelMap[esi + 2], 0
			jz		short loc_37B28
			ret
loc_37B28:	mov		byte ptr _LevelMap[esi + 1], 60h
			mov		word ptr _LevelMap[esi + 2], 8888h
			ret
loc_37B34:	mov		bl, byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1]
			cmp		bl, 0Eh
			je		locret_37BA2
			cmp		bl, 0Fh
			je		locret_37BA2
			cmp		bl, 28h
			je		short locret_37BA2
			cmp		bl, 29h
			je		short locret_37BA2
			cmp		bl, 25h
			je		short locret_37BA2
			cmp		bl, 26h
			je		short locret_37BA2
loc_37B56:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 18h
			jne		short loc_37B63
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 18h
loc_37B63:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 1Fh
			je		short loc_37B70
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
loc_37B70:	jmp		short loc_37B8E
loc_37B72:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 18h
			jne		short loc_37B7F
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 18h
loc_37B7F:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 1Fh
			je		short loc_37B8C
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0
loc_37B8C:	jmp		short loc_37B8E
loc_37B8E:	add		esi, 2 * MAP_WIDTH
			call	sub_3956F
			ret
loc_37B95:	add		esi, MAP_WIDTH * 2
			mov		ebx, esi
			shr		ebx, 1
			mov		byte ptr SomeLevelData[ebx], 6
			ret
locret_37BA2:
			ret
loc_37BA3:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			;mov		di, [esi+60CBh]
			;push	esi
			;mov		esi, 1294h
			;mov		esi, [bx+esi]
			;push	ds
			;mov		ax, es
			;mov		ds, ax
			;mov		cx, 10h
;loc_37BBC:	movsb
			;movsb
			;movsb
			;movsb
			;add	esi, 76h	; 'v'
			;add	di, 76h	; 'v'
			;loop	loc_37BBC
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 24h
			jnz		short loc_37BDB
			mov		word ptr _LevelMap[esi + 2], 0AAAAh
loc_37BDB:	cmp		bl, 26h
			jnz		short loc_37BEE
			mov		byte ptr _LevelMap[esi + 1], bl
			add		esi, 2
			call	sub_3826F
			sub		esi, 2
			ret
loc_37BEE:	cmp		bl, 28h
			jge		short loc_37BF8
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37BF8:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _LevelMap[esi], 1001h
			ret
loc_37C08:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			;mov		di, [esi+60C9h]
			;push	esi
			;mov		esi, 12A4h
			;mov		esi, [bx+esi]
			;push	ds
			;mov		ax, es
			;mov		ds, ax
			;mov		cx, 10h
;loc_37C21:	movsb
			;movsb
			;movsb
			;movsb
			;add	esi, 76h	; 'v'
			;add	di, 76h	; 'v'
			;loop	loc_37C21
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 34h	; '4'
			jne		short loc_37C40
			mov		word ptr _LevelMap[esi - 2], 0AAAAh
loc_37C40:	cmp		bl, 36h
			jnz		short loc_37C53
			mov		byte ptr _LevelMap[esi + 1], bl
			sub		esi, 2
			call	sub_3826F
			add		esi, 2
			ret
loc_37C53:	cmp		bl, 38h
			jge		short loc_37C5D
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37C5D:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, MAP_WIDTH * 2
			mov		word ptr _LevelMap[esi], 1001h
			ret
loc_37C6D:	inc		bl
			cmp		bl, 42h
			jge		short loc_37C79
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37C79:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
			jz		short loc_37C87
			dec		bl
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37C87:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		byte ptr _LevelMap[esi], 1
			mov		byte ptr _LevelMap[esi + 1], 10h
			ret
loc_37C9B:	xor		bh, bh
			and		bl, 0Fh
			shl		bl, 1
			;mov		di, [esi+60C9h]
			;push	esi
			;mov		esi, 1294h
			;mov		esi, [bx+esi]
			;push	ds
			;mov		ax, es
			;mov		ds, ax
			;mov		cx, 10h
;loc_37CB4:	movsb
			;movsb
			;movsb
			;movsb
			;add		esi, 76h	; 'v'
			;add		di, 76h	; 'v'
			;loop	loc_37CB4
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 52h
			jge		short loc_37CD2
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37CD2:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
			jne		short loc_37D02
			cmp		word ptr _LevelMap[esi - 2], 0
			je		short loc_37CE8
			cmp		word ptr _LevelMap[esi - 2], 8888h
			jne		short loc_37D02
loc_37CE8:	mov		word ptr _LevelMap[esi], 0FFFFh
			sub		esi, 2
			mov		byte ptr _LevelMap[esi], 1
			mov		byte ptr _LevelMap[esi + 1], 22h
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0FFFFh
			ret
loc_37D02:	dec		bl
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37D09:	xor		bh, bh
			and		bl, 7
			shl		bl, 1
			;mov	di, [esi+60CBh]
			;push	esi
			;mov	esi, 12A4h
			;mov	esi, [bx+esi]
			;push	ds
			;mov	ax, es
			;mov	ds, ax
			;mov	cx, 10h
;loc_37D22:	movsb
			;movsb
			;movsb
			;movsb
			;add	esi, 76h	; 'v'
			;add	di, 76h	; 'v'
			;loop	loc_37D22
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 62h
			jge		short loc_37D40
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37D40:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0
			jne		short loc_37D70
			cmp		word ptr _LevelMap[esi + 2], 0
			je		short loc_37D56
			cmp		word ptr _LevelMap[esi + 2], 8888h
			jne		short loc_37D70
loc_37D56:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, 2
			mov		byte ptr _LevelMap[esi], 1
			mov		byte ptr _LevelMap[esi + 1], 32h
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0FFFFh
			ret
loc_37D70:	dec		bl
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37D77:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
			je		short loc_37D87
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			je		short loc_37D87
			ret
loc_37D87:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _LevelMap[esi], 1001h
			jmp		@@21

handleZonk	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


handleInfotron	proc near		; DATA XREF: DATA_SEG:1612o

			cmp		byte ptr _LevelMap[esi], 4
			jz		short @@1
			ret
@@1:		mov		ax, word ptr _LevelMap[esi]
			cmp		ax, 4
			jz		short loc_37DAD
			jmp		loc_37E27
loc_37DAD:	mov		ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
			cmp		ax, 0
			je		short loc_37DF5
			cmp		ax, 1
			je		short loc_37DC6
			cmp		ax, 4
			je		short loc_37DC6
			cmp		ax, 5
			je		short loc_37DC6
			ret
loc_37DC6:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
			je		short loc_37DFC
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short loc_37DFC
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short loc_37DFC
loc_37DDD:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0
			je		short loc_37E12
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short loc_37E12
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short loc_37E12
			ret
loc_37DF5:	mov		byte ptr _LevelMap[esi + 1], 40h
			jmp		short loc_37E27
loc_37DFC:	cmp		word ptr _LevelMap[esi - 2], 0
			je		short loc_37E05
			jmp		short loc_37DDD
loc_37E05:	mov		byte ptr _LevelMap[esi + 1], 50h
			mov		word ptr _LevelMap[esi - 2], 8888h
			jmp		short loc_37E27
loc_37E12:	cmp		word ptr _LevelMap[esi + 2], 0
			je		short loc_37E1A
			ret
loc_37E1A:	mov		byte ptr _LevelMap[esi + 1], 60h
			mov		word ptr _LevelMap[esi + 2], 8888h
			jmp		short loc_37E27
loc_37E27:	mov		bl, byte ptr _LevelMap[esi + 1]
			xor		bh, bh
			mov		al, bl
			and		al, 0F0h
			cmp		al, 10h
			jne		short loc_37E37
			jmp		short loc_37E62
loc_37E37:	cmp		al, 20h
			jne		short loc_37E3E
			jmp		loc_37FAD
loc_37E3E:	cmp		al, 30h
			jne		short loc_37E45
			jmp		loc_38009
loc_37E45:	cmp		al, 40h
			jne		short loc_37E4C
			jmp		loc_38064
loc_37E4C:	cmp		al, 50h
			jne		short loc_37E53
			jmp		loc_38092
loc_37E53:	cmp		al, 60h
			jne		short loc_37E5A
			jmp		loc_38100
loc_37E5A:	cmp		al, 70h
			jne		short locret_37E61
			jmp		loc_3816E
locret_37E61:
			ret
loc_37E62:	xor		bh, bh
			shl		bl, 2
			and		bl, 1Fh
			;mov		di, [esi+6053h]
			;add	di, [bx+6C0Bh]
			;push	esi
			;mov	esi, word_408B4
			;push	ds
			;mov	ax, es
			;mov	ds, ax
			;mov	cx, 12h
;loc_37E80:	movsb
			;movsb
			;add	esi, 78h	; 'x'
			;add	di, 78h	; 'x'
			;loop	loc_37E80
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 16h
			jnz		short loc_37EA5
			mov		byte ptr _LevelMap[esi + 1], bl
			sub		esi, 2 * MAP_WIDTH
			call	sub_382EA
			add		esi, 2 * MAP_WIDTH
			ret
loc_37EA5:	cmp		bl, 18h
			jge		short loc_37EAF
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_37EAF:	mov		byte ptr _LevelMap[esi + 1], 0
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
			jnz		short loc_37EBE
			jmp		loc_37F4D
loc_37EBE:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			jnz		short loc_37EC9
			jmp		loc_37F4D
loc_37EC9:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
			jnz		short loc_37ED3
			jmp		loc_37F84
loc_37ED3:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 14h
			jnz		short loc_37EDD
			jmp		loc_37FA6
loc_37EDD:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 11h
			jnz		short loc_37EE7
			jmp		loc_37FA6
loc_37EE7:	cmp		byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 18h
			jnz		short loc_37EF1
			jmp		loc_37FA6
loc_37EF1:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 12h
			jnz		short loc_37EFB
			jmp		loc_37FA6
loc_37EFB:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 8
			jnz		short loc_37F05
			jmp		loc_37FA6
loc_37F05:	;call	SND_nnn
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 1
			je		short loc_37F1E
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 4
			je		short loc_37F1E
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 5
			je		short loc_37F1E
			ret
loc_37F1E:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
			jz		short loc_37F5A
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 8888h
			jz		short loc_37F5A
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			jz		short loc_37F5A
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0
			jz		short loc_37F70
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 8888h
			jz		short loc_37F70
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			jz		short loc_37F70
			ret
loc_37F4D:	mov		word ptr _LevelMap[esi], 7004h
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_37F5A:	cmp		word ptr _LevelMap[esi - 2], 0
			je		short loc_37F64
			jmp		loc_37DDD
loc_37F64:	mov		byte ptr _LevelMap[esi + 1], 50h
			mov		word ptr _LevelMap[esi - 2], 8888h
			ret
loc_37F70:	cmp		word ptr _LevelMap[esi + 2], 0
			je		short loc_37F78
			ret
loc_37F78:	mov		byte ptr _LevelMap[esi + 1], 60h
			mov		word ptr _LevelMap[esi + 2], 8888h
			ret
loc_37F84:	mov		bl, byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1]
			cmp		bl, 0Eh
			jz		short locret_37FAC
			cmp		bl, 0Fh
			jz		short locret_37FAC
			cmp		bl, 28h
			jz		short locret_37FAC
			cmp		bl, 29h
			jz		short locret_37FAC
			cmp		bl, 25h
			jz		short locret_37FAC
			cmp		bl, 26h
			jz		short locret_37FAC
loc_37FA6:	add		esi, MAP_WIDTH * 2
			call	sub_3956F
locret_37FAC:
			ret
loc_37FAD:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			;mov	di, [esi+60CBh]
			;push	esi
			;mov	esi, 12B6h
			;mov	esi, [bx+esi]
			;push	ds
			;mov	ax, es
			;mov	ds, ax
			;mov	cx, 10h
;loc_37FC6:	movsb
			;movsb
			;movsb
			;movsb
			;add	esi, 76h	; 'v'
			;add	di, 76h	; 'v'
			;loop	loc_37FC6
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 24h
			jne		short loc_37FE5
			mov		word ptr _LevelMap[esi + 2], 0AAAAh
loc_37FE5:	cmp		bl, 26h
			jne		short loc_37FF8
			mov		byte ptr _LevelMap[esi + 1], bl
			add		esi, 2
			call	sub_382EA
			sub		esi, 2
			ret
loc_37FF8:	cmp		bl, 28h
			jge		short loc_38002
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_38002:	mov		word ptr _LevelMap[esi], 7004h
			ret
loc_38009:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			;mov		di, [esi+60C9h]
			;push	esi
			;mov	esi, 12C6h
			;mov	esi, [bx+esi]
			;push	ds
			;mov	ax, es
			;mov		ds, ax
			;mov		cx, 10h
;loc_38022:	movsb
			;movsb
			;movsb
			;movsb
			;add	esi, 76h	; 'v'
			;add	di, 76h	; 'v'
			;loop	loc_38022
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 34h
			jne		short loc_38041
			mov		word ptr _LevelMap[esi - 2], 0AAAAh
loc_38041:	cmp		bl, 36h
			jne		short loc_38053
			mov		byte ptr _LevelMap[esi + 1], bl
			sub		esi, 2
			call	sub_382EA
			add		esi, 2
loc_38053:	cmp		bl, 38h
			jge		short loc_3805D
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_3805D:	mov		word ptr _LevelMap[esi], 7004h
			ret
loc_38064:	inc		bl
			cmp		bl, 42h
			jge		short loc_38070
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_38070:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
			jz		short loc_3807E
			dec		bl
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_3807E:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		byte ptr _LevelMap[esi], 4
			mov		byte ptr _LevelMap[esi + 1], 10h
			ret
loc_38092:	xor		bh, bh
			and		bl, 0Fh
			shl		bl, 1
			;mov	di, [esi+60C9h]
			;push	esi
			;mov	esi, 12B6h
			;mov	esi, [bx+esi]
			;push	ds
			;mov	ax, es
			;mov	ds, ax
			;mov	cx, 10h
;loc_380AB:	movsb
			;movsb
			;movsb
			;movsb
			;add	esi, 76h	; 'v'
			;add	di, 76h	; 'v'
			;loop	loc_380AB
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 52h
			jge		short loc_380C9
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_380C9:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], 0
			jnz		short loc_380F9
			cmp		word ptr _LevelMap[esi - 2], 0
			jz		short loc_380DF
			cmp		word ptr _LevelMap[esi - 2], 8888h
			jnz		short loc_380F9
loc_380DF:	mov		word ptr _LevelMap[esi], 0FFFFh
			sub		esi, 2
			mov		byte ptr _LevelMap[esi], 4
			mov		byte ptr _LevelMap[esi + 1], 22h
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_380F9:	dec		bl
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_38100:	xor		bh, bh
			and		bl, 7
			shl		bl, 1
			;mov	di, [esi+60CBh]
			;push	esi
			;mov	esi, 12C6h
			;mov	esi, [bx+esi]
			;push	ds
			;mov	ax, es
			;mov	ds, ax
			;mov	cx, 10h
;loc_38119:	movsb
			;movsb
			;movsb
			;movsb
			;add	esi, 76h	; 'v'
			;add	di, 76h	; 'v'
			;loop	loc_38119
			;pop	ds
			;pop	esi
			mov		bl, byte ptr _LevelMap[esi + 1]
			inc		bl
			cmp		bl, 62h
			jge		short loc_38137
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_38137:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], 0
			jne		short loc_38167
			cmp		word ptr _LevelMap[esi + 2], 0
			je		short loc_3814D
			cmp		word ptr _LevelMap[esi + 2], 8888h
			jne		short loc_38167
loc_3814D:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, 2
			mov		byte ptr _LevelMap[esi], 4
			mov		byte ptr _LevelMap[esi + 1], 32h
			mov		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_38167:	dec		bl
			mov		byte ptr _LevelMap[esi + 1], bl
			ret
loc_3816E:	cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
			je		short loc_3817E
			cmp		word ptr _LevelMap[esi + 2 * MAP_WIDTH], 9999h
			je		short loc_3817E
			ret
loc_3817E:	mov		word ptr _LevelMap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _LevelMap[esi], 1004h
			jmp		loc_37E62

handleInfotron	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_38191	proc near

			cmp		byte ptr _LevelMap[esi], 1Fh
			je		short loc_3819E
			mov		word ptr _LevelMap[esi], 0
loc_3819E:	cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
			je		short loc_381C8
			cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_381C8
			cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 1
			je		short loc_381BC
			cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 4
			je		short loc_381C2
			ret
loc_381BC:	mov		byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 40h
			ret
loc_381C2:	mov		byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 40h
			ret
loc_381C8:	cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], 1
			je		short loc_381E5
			cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], 4
			je		loc_3822A
loc_381D6:	cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], 1
			je		short loc_38208
			cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], 4
			je		loc_3824D
			ret
loc_381E5:	cmp		word ptr _LevelMap[esi - 2], 1
			je		short loc_381FC
			cmp		word ptr _LevelMap[esi - 2], 4
			je		short loc_381FC
			cmp		word ptr _LevelMap[esi - 2], 5
			je		short loc_381FC
			jmp		short loc_381D6
loc_381FC:	mov		byte ptr _LevelMap[esi + (2 * MAP_WIDTH + 1)], 60h
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_38208:	cmp		word ptr _LevelMap[esi + 2], 1
			je		short loc_3821E
			cmp		word ptr _LevelMap[esi + 2], 4
			je		short loc_3821E
			cmp		word ptr _LevelMap[esi + 2], 5
			je		short loc_3821E
			ret
loc_3821E:	mov		byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 3)], 50h
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
loc_3822A:	cmp		word ptr _LevelMap[esi - 2], 1
			je		short loc_38241
			cmp		word ptr _LevelMap[esi - 2], 4
			je		short loc_38241
			cmp		word ptr _LevelMap[esi - 2], 5
			je		short loc_38241
			jmp		loc_381D6
loc_38241:	mov		byte ptr _LevelMap[esi + (2 * MAP_WIDTH + 1)], 60h ; '`'
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_3824D:	cmp		word ptr _LevelMap[esi + 2], 1
			je		short loc_38263
			cmp		word ptr _LevelMap[esi + 2], 4
			je		short loc_38263
			cmp		word ptr _LevelMap[esi + 2], 5
			je		short loc_38263
			ret
loc_38263:	mov		byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 3)], 50h
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_38191	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3826F	proc near

			cmp		byte ptr _LevelMap[esi], 1Fh
			je		short loc_3827C
			mov		word ptr _LevelMap[esi], 0
loc_3827C:	cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
			je		short loc_38294
			cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_3828C
			ret
loc_3828C:	cmp		byte ptr _LevelMap[esi - (4 * MAP_WIDTH)], 4
			je		short loc_38294
			ret
loc_38294:	cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], 1
			je		short loc_382A3
loc_3829B:	cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], 1
			je		short loc_382C7
			ret
loc_382A3:	cmp		word ptr _LevelMap[esi - 2], 1
			je		short loc_382BA
			cmp		word ptr _LevelMap[esi - 2], 4
			je		short loc_382BA
			cmp		word ptr _LevelMap[esi - 2], 5
			je		short loc_382BA
			jmp		short loc_3829B
loc_382BA:	mov		word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], 6001h
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_382C7:	cmp		word ptr _LevelMap[esi + 2], 1
			je		short loc_382DD
			cmp		word ptr _LevelMap[esi + 2], 4
			je		short loc_382DD
			cmp		word ptr _LevelMap[esi + 2], 5
			je		short loc_382DD
			ret
loc_382DD:	mov		word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], 5001h
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_3826F	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_382EA	proc near

			cmp		byte ptr _LevelMap[esi], MAP_RED_LAMP
			je		short loc_382F7
			mov		word ptr _LevelMap[esi], MAP_SPACE
loc_382F7:	cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
			je		short loc_3830F
			cmp		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_38307
			ret
loc_38307:	cmp		byte ptr _LevelMap[esi - (4 * MAP_WIDTH)], 1
			je		short loc_3830F
			ret
loc_3830F:	cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], 4
			je		short loc_3831E
loc_38316:	cmp		word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], 4
			je		short loc_38342
			ret
loc_3831E:	cmp		word ptr _LevelMap[esi - 2], 1
			je		short loc_38335
			cmp		word ptr _LevelMap[esi - 2], 4
			je		short loc_38335
			cmp		word ptr _LevelMap[esi - 2], 5
			je		short loc_38335
			jmp		short loc_38316
loc_38335:	mov		word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], 6004h
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_38342:	cmp		word ptr _LevelMap[esi + 2], 1
			je		short loc_38358
			cmp		word ptr _LevelMap[esi + 2], 4
			je		short loc_38358
			cmp		word ptr _LevelMap[esi + 2], 5
			je		short loc_38358
			ret
loc_38358:	mov		word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], 5004h
			mov		word ptr _LevelMap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_382EA	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


_InitPlayerState	proc near

			mov		ax, PlayerPosition_MapX
			mov		PlayerPosition2_MapX, ax
			mov		ax, PlayerPosition_MapY
			mov		PlayerPosition2_MapY, ax
			xor		ax, ax
			mov		word_403BB, ax
			mov		word_403C1, ax
			mov		word_40C64, ax
			mov		TimeoutAfterWhichLeaveTheLevel, ax
			mov		byte_40C4C, 0
			mov		byte_40C6C, 0
			mov		word_4C47F, 0
			mov		word_4C47B, 0
			mov		word_4C47D, 0
			mov		word_403BD, 0
			mov		byte_403A7, 0FFh
			mov		byte_403A8, 0FFh
			mov		byte_403A9, 0FFh
			mov		byte_4039E, 1
			mov		byte_4039F, 0
			mov		byte_403A0, 0
			mov		byte_403A1, 0
			mov		byte_403A2, 0
			mov		byte_403A4, 0
			mov		byte_403A5, 0
			mov		byte_403A6, 0
			mov		byte_403B0, 0
			mov		byte_40C5A, 7Fh
			mov		byte_40C5B, 0
			mov		word_40C4D, 0
			mov		byte_403B1, 1
			mov		byte_403B2, 0
			mov		byte_403C7, 0
			ret

_InitPlayerState	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

_BeginLevel proc near

			cmp		IsDemoRunning, 0
			jz		short loc_38456
			mov		byte_403AA, 0
			jmp		short loc_3845B
loc_38456:	mov		byte_403AA, 1
loc_3845B:	;mov		word_40392, 1
			ret

_BeginLevel endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

_RunTheLevel	proc near

			push	ebp
			push	esi
			push	edi
			push 	ebx

			cmp		IsDemoRunning, 0
			jne		short loc_3846B
			;call	PollJoystick
loc_3846B:	xor ebx, ebx;call	PollMouse
			cmp		bx, 1
			jnz		short loc_38480
			cmp		TimeoutAfterWhichLeaveTheLevel, 0
			jg		short loc_38480
			mov		word_403C1, 1
loc_38480:	cmp		bx, 2
			jnz		short loc_3848F
			cmp		word_40C60, 0
			jz		short loc_3848F
			;call	sub_386E7
loc_3848F:	;call	HandleUserInput
			cmp		word_40CF3, 0
			jz		short loc_384A5
			;mov	dx, 3C8h
			;xor	al, al
			;out	dx, al
			;inc	dx
			;mov	al, 35h	; '5'
			;out	dx, al
			;out	dx, al
			;out	dx, al
loc_384A5:	call	MoveObjectsInTheLevel
			cmp		word_40CF3, 0
			jz		short loc_384BB
			;mov	dx, 3C8h
			;xor	al, al
			;out	dx, al
			;inc	dx
			;mov	al, 21h	; '!'
			;out	dx, al
			;out	dx, al
			;out	dx, al
loc_384BB:	;call	PrepareStatsInfoForBottomPanel
			;call	sub_3EC47
			cmp		word_40CF3, 0
			jz		short loc_384D8
			;mov	dx, 3C8h
			;xor	al, al
			;out	dx, al
			;inc	dx
			;mov	al, 2Dh	; '-'
			;out	dx, al
			;mov	al, 21h	; '!'
			;out	dx, al
			;mov	al, 0Fh
			;out	dx, al
loc_384D8:	call	sub_39860
			call	sub_39530
			call	sub_38F26
			mov		bx, word_40C4F
			mov		cl, 3
			shr		bx, cl
			mov		ax, word_40C51
			mov		cx, 2 * MAP_WIDTH + 2
			mul		cx
			;add		bx, ax
			;add		bx, 4D34h
			;mov		VID_VgaMemStartAddress,	bx
			;mov	dx, 3D4h
			;mov	al, 0Dh
			;out	dx, al
			;inc	dx
			;mov	al, bl
			;out	dx, al
			;mov	dx, 3D4h
			;mov	al, 0Ch
			;out	dx, al
			;inc	dx
			;mov	al, bh
			;out	dx, al
			mov		cx, word_40C4F
			mov		ah, cl
			and		ah, 7
			cmp		word_40CF3, 0
			jz		short loc_3852B
			;mov		dx, 3C8h
			;xor		al, al
			;out	dx, al
			;inc	dx
			;mov	al, 3Fh	; '?'
			;out	dx, al
			;out	dx, al
			;out	dx, al
loc_3852B:	;mov		VID_HorizontalPanning, ah
			;call	VID_SetHorizontalPanning
			cmp		word_40CF7, 1
			jbe		short loc_3853C
			;call	SND_bbb
loc_3853C:	mov		cx, word_40CF7
loc_38540:	dec		cx
			jz		short loc_38581
			;call	VID_WaitVerticalRetrace
			;call	VID_SetHorizontalPanning
			push	cx
			;call	HandleUserInput
			call	sub_38F26
			;mov		bx, word_40C4F
			;mov		cl, 3
			;shr		bx, cl
			;mov		ax, word_40C51
			;mov		cx, MAP_WIDTH * 2 + 2
			;mul		cx
			;add		bx, ax
			;add		bx, 4D34h
			;mov	VID_VgaMemStartAddress,	bx
			;mov	dx, 3D4h
			;mov	al, 0Dh
			;out	dx, al
			;inc	dx
			;mov	al, bl
			;out	dx, al
			;mov	dx, 3D4h
			;mov	al, 0Ch
			;out	dx, al
			;inc	dx
			;mov	al, bh
			;out	dx, al
			pop		cx
			jmp		short loc_38540
loc_38581:	cmp		word_40CF3, 0
			jz		short loc_38592
			;mov	dx, 3C8h
			;xor	al, al
			;out	dx, al
			;inc	dx
			;;out	dx, al
			;out	dx, al
			;out	dx, al
loc_38592:	inc		word_40C4D
			cmp		word_40C64, 1
			jz		short loc_385B0
			cmp		TimeoutAfterWhichLeaveTheLevel, 0
			jnz		short loc_385A7
			mov		eax, 1
			jmp short locret
loc_385A7:	dec		TimeoutAfterWhichLeaveTheLevel
			jz		short loc_385B0
			mov		eax, 1
			jmp short locret
loc_385B0:	;mov		word_40392, 0
			;cmp		byte_403D3, 0
			;jz		short loc_385C0
			;call	sub_38A5C
;loc_385C0:	
			cmp		byte_403A3, 0
			jz		short locret_385DC
			mov		al, byte_403A0
			mov		byte_403A4, al
			mov		al, byte_403A1
			mov		byte_403A5, al
			mov		al, byte_403A2
			mov		byte_403A6, al
			;call	sub_398B5
locret_385DC:
			xor		eax, eax

locret:
			pop		ebx
			pop		edi
			pop		esi
			pop		ebp
			ret

_RunTheLevel	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

MoveObjectsInTheLevel proc near

			;mov		dx, 3CEh
			;mov		al, 5
			;out		dx, al
			;inc	dx
			;mov	al, 1
			;out	dx, al
			mov		esi, PlayerPosition_Ofs
			call	xxx_HandleMurphy
			mov		PlayerPosition_Ofs, esi
			cmp		word_40CF3, 0
			jz		short loc_38E6D
			;mov	dx, 3C8h
			;xor	al, al
			;out	dx, al
			;inc	dx
			;mov	al, 3Fh	; '?'
			;out	dx, al
			;out	dx, al
			;mov	al, 21h	; '!'
			;out	dx, al
loc_38E6D:	cmp		byte_403B1, 0
			jz		short loc_38E77
			;call	sub_3F116
loc_38E77:	;mov	dx, 3CEh
			;mov	al, 5
			;out	dx, al
			;inc	dx
			;mov	al, 1
			;out	dx, al
			cmp		word_40CF3, 0
			jz		short loc_38E96
			;mov	dx, 3C8h
			;xor	al, al
			;out	dx, al
			;inc	dx
			;mov	al, 3Fh	; '?'
			;out	dx, al
			;mov	al, 21h	; '!'
			;out	dx, al
			;out	dx, al
loc_38E96:	mov		esi, 2 * MAP_WIDTH + 2
			;mov		edi, offset dword_46231
			mov		ecx, MAP_WIDTH * MAP_HEIGHT - (2 * MAP_WIDTH + 2)
			mov		edx, 0
loc_38EA2:	mov		bl, byte ptr _LevelMap[esi]
			cmp		bl, 32
			jnb		short loc_38EC9
			movzx	ebx, bl
			shl		ebx, 2
			cmp		MapHandlers_4[ebx], 0
			je		short loc_38EC9
			mov		eax, MapHandlers_4[ebx]
			push	esi
			push	edi
			push	ecx
			call	eax
			pop		ecx
			pop		edi
			pop		esi
loc_38EC9:	add		esi, 2
			loop	loc_38EA2
loc_38EE8:	;mov	dx, 3CEh
			;mov	al, 5
			;out	dx, al
			;inc	dx
			;mov	al, 0
			;out	dx, al
			cmp		word_403C1, 1
			je		short loc_38F01
			cmp		word_403BF, 0
			je		short loc_38F01
			ret
loc_38F01:	cmp		TimeoutAfterWhichLeaveTheLevel, 0
			jne		short loc_38F1B
			mov		word_403C1, 0
			mov		esi, word_403B7
			call	sub_3956F
			mov		TimeoutAfterWhichLeaveTheLevel, 40h
loc_38F1B:	;mov		dx, 3CEh
			;mov	al, 5
			;out	dx, al
			;inc	dx
			;mov	al, 1
			;out	dx, al
			ret

MoveObjectsInTheLevel endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_38F26	proc near

		cmp	byte_403B0, 0
		jz	short loc_38F42
		call	GenerateRandomNumber
		mov	cl, al
		mov	dl, ah
		and	cx, 1
		and	dx, 1
		mov	cx, 0
		mov	dx, 0
		jmp	short loc_38F48


loc_38F42:
		mov	cx, 0
		mov	dx, 0

loc_38F48:
		mov	bx, PlayerPosition_PixelsX
		mov	ax, PlayerPosition_PixelsY
		add	bx, cx
		add	ax, dx
		sub	bx, 98h	; ''
		cmp	bx, 8
		jge	short loc_38F5F
		mov	bx, 8

loc_38F5F:
		cmp	bx, 278h
		jle	short loc_38F68
		mov	bx, 278h

loc_38F68:
		cmp	byte_403B1, 0
		jz	short loc_38F74
		sub	ax, 58h	; 'X'
		jmp	short loc_38F77


loc_38F74:
		sub	ax, 64h	; 'd'

loc_38F77:
		cmp	ax, 0
		jge	short loc_38F7F
		mov	ax, 0

loc_38F7F:
		cmp	byte_403B1, 0
		jz	short loc_38F90
		cmp	ax, 0C0h ; 'Р'
		jle	short loc_38F8E
		mov	ax, 0C0h ; 'Р'

loc_38F8E:
		jmp	short loc_38F98


loc_38F90:
		cmp	ax, 0A8h ; 'Ј'
		jle	short loc_38F98
		mov	ax, 0A8h ; 'Ј'

loc_38F98:
		add	bx, word_40C53
		add	ax, word_40C55
		mov	word_40C4F, bx
		mov	word_40C51, ax
		;mov	cl, 3
		;shr	bx, cl
		;mov	cx, 7Ah	; 'z'
		;mul	cx
		;add	bx, ax
		;add	bx, 4D34h
		;mov	VID_VgaMemStartAddress,	ax
		ret
sub_38F26	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

xxx_HandleBug	proc near
		cmp	byte ptr _LevelMap[esi], MAP_BUG
		jz	short loc_39012
		ret

loc_39012:
		mov	ax, word_40C4D
		and	ax, 3
		cmp	ax, 0
		jz	short loc_3901E
		ret

loc_3901E:
		mov	bl, byte ptr _LevelMap[esi + 1]
		inc	bl
		cmp	bl, MAP_PORT_SPECIAL_UP_TO_DOWN
		jl	short loc_39034
		call	GenerateRandomNumber
		and	al, 3Fh
		add	al, 20h	; ' '
		neg	al
		mov	bl, al

loc_39034:
		mov	byte ptr _LevelMap[esi + 1], bl
		cmp	bl, MAP_SPACE
		jge	short loc_3903E
		ret


loc_3903E:
		cmp	byte ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _LevelMap[esi - 2], MAP_MURPHY	; -1
		jz	short loc_39078
		cmp	byte ptr _LevelMap[esi + 2], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], MAP_MURPHY
		jz	short loc_39078
		jmp	short loc_3907B


loc_39078:
		;call	SND_aaa

loc_3907B:
		xor	bh, bh
		;mov	di, word_456CB[esi]
		;mov	esi, bx
		;shl	esi, 1
		;add	esi, offset unk_40872
		;mov	esi, [esi]
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 16

;loc_39093:
		;movsb
		;movsb
		;add	esi, 120
		;add	di, 120
		;loop	loc_39093
		;pop	ds
		ret

xxx_HandleBug	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


xxx_HandleTerminal proc	near		; DATA XREF: DATA_SEG:1630o
		cmp	byte ptr _LevelMap[esi], MAP_TERMINAL
		jz	short loc_390A7
		ret


loc_390A7:
		mov	bl, byte ptr _LevelMap[esi + 1]
		inc	bl
		cmp	bl, 0
		jg	short loc_390B7
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_390B7:
		call	GenerateRandomNumber
		and	al, byte_40C5A
		neg	al
		mov	bl, al
		mov	byte ptr _LevelMap[esi + 1], bl
		;mov	edi, word_456CB[esi]
		;mov	esi, edi
		;add	edi, 4C4h
		;add	esi, 0F4h ; 'є'
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;push	si
		;push	di
		;movsb
		;add	di, 0FC2Fh
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;pop	di
		;pop	si
		;inc	si
		;inc	di
		;movsb
		;add	di, 0FC2Fh
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;movsb
		;add	di, 79h	; 'y'
		;add	si, 79h	; 'y'
		;pop	ds
		;ret
xxx_HandleTerminal endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

_InitRandomNumberGenerator proc near

		mov cx, 01234h
		mov dx, 4321h ; FIXME
		xor	cx, dx
		mov	RandomSeed, cx
		ret
_InitRandomNumberGenerator endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


GenerateRandomNumber proc near
		mov	ax, RandomSeed
		mov	bx, 1509
		mul	bx
		add	ax, 49
		mov	RandomSeed, ax
		shr	ax, 1
		ret
GenerateRandomNumber endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

_FindPlayerPositionOnLevelMap proc near

			push	ebp
			push	esi
			push	edi
			push 	ebx

			xor		ax, ax
			mov		word_40C4F, ax
			mov		word_40C51, ax
			mov		edi, offset _LevelMap
			mov		ecx, MAP_NUM_CELLS
			mov		ax, MAP_MURPHY
			repne 	scasw
			dec		edi
			dec		edi
			mov		eax, MAP_WIDTH * MAP_HEIGHT - 1
			sub		eax, ecx
			mov		ecx, eax
			shl		ecx, 1
			mov		PlayerPosition_Ofs, ecx
			mov		bl, MAP_WIDTH
			div		bl
			mov		bl, ah
			xor		bh, bh
			mov		PlayerPosition_MapX, bx
			xor		ah, ah
			mov		PlayerPosition_MapY, ax
			mov		cl, 4
			shl		ax, cl
			shl		bx, cl
			mov		PlayerPosition_PixelsX,	bx
			mov		PlayerPosition_PixelsY,	ax
			call	sub_38F26

			pop		ebx
			pop		edi
			pop		esi
			pop		ebp

			ret

_FindPlayerPositionOnLevelMap endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


_sub_392DF	proc near

		push	ebp
		push	esi
		push	edi
		push 	ebx

		xor	ebx, ebx
		xor	edx, edx
		mov	ecx, MAP_WIDTH * MAP_HEIGHT
		mov	esi, offset _LevelMap

loc_392E9:
		mov	ax, [esi]
		cmp	ax, MAP_INFOTRON
		jz	short loc_39324
		cmp	ax, MAP_SNIK_SNAK
		jz	short loc_39333
		cmp	ax, MAP_ELECTRON
		jz	loc_39361
		cmp	ax, MAP_RAM_CHIP_LEFT
		jz	short loc_39327
		cmp	ax, MAP_RAM_CHIP_RIGHT
		jz	short loc_39327
		cmp	ax, MAP_RAM_CHIP_TOP
		jz	short loc_39327
		cmp	ax, MAP_RAM_CHIP_BOTTOM
		jz	short loc_39327
		cmp	ax, MAP_HARDWARE_1
		jl	short loc_39318
		cmp	ax, MAP_RAM_CHIP_TOP
		jl	short loc_3932D

loc_39318:
		cmp	ax, MAP_PORT_SPECIAL_LEFT_TO_RIGHT
		jl	short loc_39322
		cmp	ax, MAP_SNIK_SNAK
		jl	loc_3938F

loc_39322:
		jmp	loc_39398


loc_39324:
		inc	edx
		jmp	loc_39398


loc_39327:
		mov	word ptr [esi], MAP_RAM_CHIP_SQUARE
		jmp	loc_39398


loc_3932D:
		mov	word ptr [esi], MAP_GRAY_DENTED_PYRAMID
		jmp	loc_39398


loc_39333:
		cmp	word ptr [esi-2], MAP_SPACE
		jnz	short loc_3933F
		mov	byte ptr [esi+1], MAP_ZONK
		jmp	short loc_39398


loc_3933F:
		cmp	word ptr [esi-78h], 0
		jnz	short loc_39350
		mov	word ptr [esi-78h], 1011h
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398


loc_39350:
		cmp	word ptr [esi+2], 0
		jnz	short loc_39398
		mov	word ptr [esi+2], 2811h
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398

loc_39361:
		cmp	word ptr [esi-2], 0
		jnz	short loc_3936D
		mov	byte ptr [esi+1], 1
		jmp	short loc_39398

loc_3936D:
		cmp	word ptr [esi-78h], 0
		jnz	short loc_3937E
		mov	word ptr [esi-78h], 1018h
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398

loc_3937E:
		cmp	word ptr [esi+2], 0
		jnz	short loc_39398
		mov	word ptr [esi+2], 2818h
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398

loc_3938F:
		sub	byte ptr [esi], 4
		mov	byte ptr [esi+1], 1
		jmp	short loc_39398

loc_39398:
		add	esi, 2
		inc	ebx
		dec	ecx
		jz	short loc_393A2
		jmp	loc_392E9


loc_393A2:
		cmp	byte_40326, 0
		jz	short loc_393AD
		mov	dl, byte_40326

loc_393AD:
		mov	InfotronsLeftToCollect,	dl
		mov	TotalInfotronsInTheLevel, dl
		;call	sub_3EC01

		pop	ebx
		pop	edi
		pop	esi
		pop	ebp

		ret
_sub_392DF	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

xxx_handleOrangeDisk proc near		; DATA XREF: DATA_SEG:161Ao
		cmp	byte ptr _LevelMap[esi], MAP_DISK_ORANGE
		jz	short loc_393D7
		ret


loc_393D7:
		mov	ax, word ptr _LevelMap[esi]
		cmp	ax, 3008h
		jge	short loc_39424
		cmp	ax, 2008h
		jge	short loc_393FA
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
		cmp	ax, 0
		jz	short loc_393EF
		ret


loc_393EF:
		mov	byte ptr _LevelMap[esi + 1], 20h ; ' '
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 8
		ret

loc_393FA:
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_39408
		mov	word ptr _LevelMap[esi], 8
		ret

loc_39408:
		mov	bl, byte ptr _LevelMap[esi + 1]
		inc	bl
		cmp	bl, 22h	; '"'
		jnz	short loc_39415
		mov	bl, 30h	; '0'

loc_39415:
		mov	byte ptr _LevelMap[esi + 1], bl
		ret

		mov	byte ptr _LevelMap[esi + 1], 30h
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 8

loc_39424:
		;push	si
		;mov	bl, byte ptr _LevelMap[esi + 1]
		;xor	bh, bh
		;mov	al, bl
		;shl	bx, 1
		;and	bx, 0Fh
		;mov	di, [si+60CBh]
		;shl	bx, 1
		;add	di, [bx+6C0Bh]
		;mov	si, 12F6h
		;mov	si, [si]
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 12h

;loc_39449:
	;	movsb
	;	movsb
	;	add	si, 78h	; 'x'
	;	add	di, 78h	; 'x'
	;	loop	loc_39449
	;	pop	ds
	;	pop	si
		mov	bl, byte ptr _LevelMap[esi + 1]
		inc	bl
		mov	al, bl
		and	al, 7
		jz	short loc_39466
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_39466:
		mov	word ptr _LevelMap[esi], 0
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 8
		add	esi, 78h	; 'x'
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_39487
		mov	byte ptr _LevelMap[esi + 1], 30h ; '0'
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 8
		ret


loc_39487:
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 1Fh
		jnz	short loc_3948F
		ret


loc_3948F:
		call	sub_3956F
		ret
xxx_handleOrangeDisk endp



loc_39493:
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3949B
		ret


loc_3949B:
		mov	ax, word_40C4D
		and	ax, 3
		cmp	ax, 0
		jz	short loc_394A7
		ret


loc_394A7:
		mov	bl, byte ptr _LevelMap[esi + 1]
		test	bl, 80h
		jnz	short loc_394F0
		inc	bl
		mov	byte ptr _LevelMap[esi + 1], bl
		;push	si
		;push	bx
		;xor	bh, bh
		;dec	bl
		;mov	di, [si+60CBh]
		;;mov	si, bx
		;shl	si, 1
		;add	si, 12D6h
		;mov	si, [si]
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 10h

;loc_394D2:
;		movsb
;		movsb
;		add	si, 78h	; 'x'
;		add	di, 78h	; 'x'
;		loop	loc_394D2
;		pop	ds
;		pop	bx
;		pop	si
		cmp	bl, 8
		jnz	short locret_394EF
		mov	word ptr _LevelMap[esi], 0
		mov	byte_403B0, 0

locret_394EF:
		ret


loc_394F0:
		inc	bl
		cmp	bl, 89h
		jnz	short loc_39503
		mov	word ptr _LevelMap[esi], 4
		mov	byte_403B0, 0
		ret


loc_39503:
		mov	byte ptr _LevelMap[esi + 1], bl
		;xor	bh, bh
		;dec	bl
		;and	bl, 0Fh
		;mov	di, [si+60CBh]
		;mov	si, bx
		;shl	si, 1
		;add	si, 12E6h
		;mov	si, [si]
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 10h

;loc_39524:
		;movsb
		;movsb
		;add	si, 78h	; 'x'
		;add	di, 78h	; 'x'
		;loop	loc_39524
		;pop	ds
		ret

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_39530	proc near
		mov	esi, 0
		mov	ebx, 0
		mov	ecx, MAP_WIDTH * MAP_HEIGHT

loc_39539:
		cmp	byte ptr SomeLevelData[ebx], 0
		jz	short loc_39551
		jl	short loc_39558
		dec	byte ptr SomeLevelData[ebx]
		jnz	short loc_39551
		push	esi
		push	ecx
		push	ebx
		call	sub_3956F
		pop	ebx
		pop	ecx
		pop	esi

loc_39551:
		add	esi, 2
		inc	ebx
		loop	loc_39539
		ret


loc_39558:
		inc	byte ptr SomeLevelData[ebx]
		jnz	short loc_39551
		push	esi
		push	ecx
		push	ebx
		mov	word ptr _LevelMap[esi], 0FF18h
		call	sub_3956F
		pop	ebx
		pop	ecx
		pop	esi
		jmp	short loc_39551
sub_39530	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3956F	proc near
		cmp	byte ptr _LevelMap[esi], MAP_GRAY_DENTED_PYRAMID
		jnz	short loc_39577
		ret


loc_39577:
		mov	byte_403B0, 1
		cmp	byte ptr _LevelMap[esi], 3
		jnz	short loc_39589
		mov	word_403C1, 1

loc_39589:
		cmp	byte ptr _LevelMap[esi], 18h
		jnz	short loc_39597
		mov	cx, 801Fh
		mov	dl, 0F3h
		jmp	short loc_3959C


loc_39597:
		mov	ecx, 1Fh
		mov	dl, 0Dh

loc_3959C:
		mov	ebx, esi
		shr	ebx, 1
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)]
		cmp	al, 8
		jz	short loc_395D0
		cmp	al, 12h
		jz	short loc_395D0
		cmp	al, 11h
		jz	short loc_395D0
		cmp	al, 1
		jz	short loc_395EC
		cmp	al, 4
		jz	short loc_395E2
		cmp	al, 18h
		jnz	short loc_395C6
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_395D0


loc_395C6:
		cmp	al, 3
		jnz	short loc_395D8
		mov	word_403C1, 1

loc_395D0:
		cmp	al, 6
		jz	short loc_395D8
		mov	SomeLevelData[ebx - MAP_WIDTH - 1], dh

loc_395D8:
		cmp	al, 6
		jz	short loc_395E0
		mov	word ptr _LevelMap[esi - (2 * MAP_WIDTH + 2)], cx

loc_395E0:
		jmp	short loc_395F6


loc_395E2:
		push	esi
		add	esi, -(MAP_WIDTH * 2 + 2)
		call	sub_39969
		pop	esi
		jmp	short loc_395F6


loc_395EC:
		push	esi
		add	esi, -(MAP_WIDTH * 2 + 2)
		call	sub_398F9
		pop	esi
		jmp	short loc_395F6

loc_395F6:
		pop	ecx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi - 2 * MAP_WIDTH]
		cmp	al, 8
		jz	short loc_39627
		cmp	al, 12h
		jz	short loc_39627
		cmp	al, 11h
		jz	short loc_39627
		cmp	al, 1
		jz	short loc_39643
		cmp	al, 4
		jz	short loc_39639
		cmp	al, 18h
		jnz	short loc_3961D
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39627


loc_3961D:
		cmp	al, 3
		jnz	short loc_3962F
		mov	word_403C1, 1

loc_39627:
		cmp	al, 6
		jz	short loc_3962F
		mov	SomeLevelData[ebx - MAP_WIDTH], dh

loc_3962F:
		cmp	al, 6
		jz	short loc_39637
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], cx

loc_39637:
		jmp	short loc_3964D

loc_39639:
		push	esi
		add	esi, -(MAP_WIDTH * 2)
		call	sub_39969
		pop	esi
		jmp	short loc_3964D

loc_39643:
		push	esi
		add	esi, -(MAP_WIDTH * 2)
		call	sub_398F9
		pop	esi
		jmp	short loc_3964D

loc_3964D:
		pop	ecx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)]
		cmp	al, 8
		jz	short loc_3967E
		cmp	al, 12h
		jz	short loc_3967E
		cmp	al, 11h
		jz	short loc_3967E
		cmp	al, 1
		jz	short loc_3969A
		cmp	al, 4
		jz	short loc_39690
		cmp	al, 18h
		jnz	short loc_39674
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_3967E


loc_39674:
		cmp	al, 3
		jnz	short loc_39686
		mov	word_403C1, 1

loc_3967E:
		cmp	al, 6
		jz	short loc_39686
		;mov	[bx+237Fh], dh

loc_39686:
		cmp	al, 6
		jz	short loc_3968E
		mov	word ptr _LevelMap[esi - (2 * MAP_WIDTH - 2)], cx

loc_3968E:
		jmp	short loc_396A4

loc_39690:
		push	esi
		add	esi, -(2 * MAP_WIDTH - 2)
		call	sub_39969
		pop	esi
		jmp	short loc_396A4

loc_3969A:
		push	esi
		add	esi, -(2 * MAP_WIDTH - 2)
		call	sub_398F9
		pop	esi
		jmp	short loc_396A4

loc_396A4:
		pop	ecx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi - 2]
		cmp	al, 8
		jz	short loc_396D5
		cmp	al, 12h
		jz	short loc_396D5
		cmp	al, 11h
		jz	short loc_396D5
		cmp	al, 1
		jz	short loc_396F1
		cmp	al, 4
		jz	short loc_396E7
		cmp	al, 18h
		jnz	short loc_396CB
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_396D5


loc_396CB:
		cmp	al, 3
		jnz	short loc_396DD
		mov	word_403C1, 1

loc_396D5:
		cmp	al, 6
		jz	short loc_396DD
		mov	SomeLevelData[ebx - 1], dh

loc_396DD:
		cmp	al, 6
		jz	short loc_396E5
		mov	word ptr _LevelMap[esi - 2], cx

loc_396E5:
		jmp	short loc_396FB

loc_396E7:
		push	esi
		add	esi, -2
		call	sub_39969
		pop	esi
		jmp	short loc_396FB

loc_396F1:
		push	esi
		add	esi, -2
		call	sub_398F9
		pop	esi
		jmp	short loc_396FB

loc_396FB:
		pop	ecx
		mov	word ptr _LevelMap[esi], cx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi + 2]
		cmp	al, 8
		jz	short loc_39730
		cmp	al, 12h
		jz	short loc_39730
		cmp	al, 11h
		jz	short loc_39730
		cmp	al, 1
		jz	short loc_3974C
		cmp	al, 4
		jz	short loc_39742
		cmp	al, 18h
		jnz	short loc_39726
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39730

loc_39726:
		cmp	al, 3
		jnz	short loc_39738
		mov	word_403C1, 1

loc_39730:
		cmp	al, 6
		jz	short loc_39738
		mov	SomeLevelData[ebx + 1], dh

loc_39738:
		cmp	al, 6
		jz	short loc_39740
		mov	word ptr _LevelMap[esi + 2], cx

loc_39740:
		jmp	short loc_39756

loc_39742:
		push	esi
		add	esi, 2
		call	sub_39969
		pop	esi
		jmp	short loc_39756

loc_3974C:
		push	esi
		add	esi, 2
		call	sub_398F9
		pop	esi
		jmp	short loc_39756

loc_39756:
		pop	ecx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2]
		cmp	al, 8
		jz	short loc_39787
		cmp	al, 12h
		jz	short loc_39787
		cmp	al, 11h
		jz	short loc_39787
		cmp	al, 1
		jz	short loc_397A3
		cmp	al, 4
		jz	short loc_39799
		cmp	al, 18h
		jnz	short loc_3977D
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39787


loc_3977D:
		cmp	al, 3
		jnz	short loc_3978F
		mov	word_403C1, 1

loc_39787:
		cmp	al, 6
		jz	short loc_3978F
		;mov	[bx+23F5h], dh

loc_3978F:
		cmp	al, 6
		jz	short loc_39797
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH - 2], cx

loc_39797:
		jmp	short loc_397AD


loc_39799:
		push	esi
		add	esi, 76h
		call	sub_39969
		pop	esi
		jmp	short loc_397AD

loc_397A3:
		push	esi
		add	esi, 76h	; 'v'
		call	sub_398F9
		pop	esi
		jmp	short loc_397AD

loc_397AD:
		pop	ecx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
		cmp	al, 8
		jz	short loc_397DE
		cmp	al, 12h
		jz	short loc_397DE
		cmp	al, 11h
		jz	short loc_397DE
		cmp	al, 1
		jz	short loc_397FA
		cmp	al, 4
		jz	short loc_397F0
		cmp	al, 18h
		jnz	short loc_397D4
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_397DE


loc_397D4:
		cmp	al, 3
		jnz	short loc_397E6
		mov	word_403C1, 1

loc_397DE:
		cmp	al, 6
		jz	short loc_397E6
		mov	SomeLevelData[ebx + MAP_WIDTH], dh

loc_397E6:
		cmp	al, 6
		jz	short loc_397EE
		mov word ptr _LevelMap[esi + 2 * MAP_WIDTH], cx

loc_397EE:
		jmp	short loc_39804

loc_397F0:
		push	esi
		add	esi, 78h
		call	sub_39969
		pop	esi
		jmp	short loc_39804

loc_397FA:
		push	esi
		add	esi, 78h
		call	sub_398F9
		pop	esi
		jmp	short loc_39804

loc_39804:
		pop	ecx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2]
		cmp	al, 8
		jz	short loc_39835
		cmp	al, 12h
		jz	short loc_39835
		cmp	al, 11h
		jz	short loc_39835
		cmp	al, 1
		jz	short loc_39851
		cmp	al, 4
		jz	short loc_39847
		cmp	al, 18h
		jnz	short loc_3982B
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39835


loc_3982B:
		cmp	al, 3
		jnz	short loc_3983D
		mov	word_403C1, 1

loc_39835:
		cmp	al, 6
		jz	short loc_3983D
		mov	SomeLevelData[ebx + MAP_WIDTH + 1], dh

loc_3983D:
		cmp	al, 6
		jz	short loc_39845
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2], cx

loc_39845:
		jmp	short loc_3985B

loc_39847:
		push	esi
		add	esi, 7Ah
		call	sub_39969
		pop	esi
		jmp	short loc_3985B


loc_39851:
		push	esi
		add	esi, 7Ah
		call	sub_398F9
		pop	esi
		jmp	short loc_3985B

loc_3985B:
		pop	ecx
		;call	SND_ttt
		ret
sub_3956F	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_39860	proc near
		mov		al, byte_403CB
		cmp		al, 1
		jle		short loc_398AA
		mov		esi, word_403CC
		cmp		word ptr _LevelMap[esi], 0
		jnz		short loc_39882
		mov		word ptr _LevelMap[esi], 14h
loc_39882:
		inc	byte_403CB
		cmp	byte_403CB, 28h
		jl	short loc_398AA
		mov	esi, word_403CC
		call	sub_3956F
		mov	byte_403CB, 0

loc_398AA:
		ret
sub_39860	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_398F9	proc near
		mov	word ptr _LevelMap[esi], cx
		and	ah, 0F0h
		cmp	ah, 10h
		jz	short loc_39924
		cmp	ah, 70h
		jz	short loc_39924
		cmp	ah, 20h
		jz	short loc_3993A
		cmp	ah, 30h
		jz	short loc_39947
		cmp	ah, 50h
		jz	short loc_39954
		cmp	ah, 60h
		jz	short loc_3995B
		cmp	ah, 70h
		jz	short loc_39962
		ret

loc_39924:
		sub	esi, 78h
		call	sub_399E9
		add	esi, 0F0h
		cmp	word ptr _LevelMap[esi], 9999h
		jnz	short locret_39939
		call	sub_399E9

locret_39939:
		ret

loc_3993A:
		add	esi, 2
		call	sub_399E9
		add	esi, 76h
		call	sub_399E9
		ret

loc_39947:
		sub	esi, 2
		call	sub_399E9
		add	esi, 7Ah
		call	sub_399E9
		ret

loc_39954:
		sub	esi, 2
		call	sub_399E9
		ret

loc_3995B:
		add	esi, 2
		call	sub_399E9
		ret

loc_39962:
		add	esi, 78h
		call	sub_399E9
		ret
sub_398F9	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_39969	proc near

		mov	word ptr _LevelMap[esi], cx
		and	ah, 0F0h
		cmp	ah, 10h
		jz	short loc_39994
		cmp	ah, 70h
		jz	short loc_39994
		cmp	ah, 20h
		jz	short loc_399AA
		cmp	ah, 30h
		jz	short loc_399BF
		cmp	ah, 50h
		jz	short loc_399D4
		cmp	ah, 60h
		jz	short loc_399DB
		cmp	ah, 70h
		jz	short loc_399E2
		ret

loc_39994:
		sub	esi, 78h
		call	sub_399E9
		add	esi, 0F0h 
		cmp	word ptr _LevelMap[esi], 9999h
		jnz	short locret_399A9
		call	sub_399E9

locret_399A9:
		ret

loc_399AA:
		add	esi, 2
		call	sub_399E9
		add	esi, 76h
		cmp	word ptr _LevelMap[esi], 9999h
		jnz	short locret_399BE
		call	sub_399E9

locret_399BE:
		ret

loc_399BF:
		sub	esi, 2
		call	sub_399E9
		add	esi, 7Ah
		cmp	word ptr _LevelMap[esi], 9999h
		jnz	short locret_399D3
		call	sub_399E9

locret_399D3:
		ret

loc_399D4:
		sub	esi, 2
		call	sub_399E9
		ret

loc_399DB:
		add	esi, 2
		call	sub_399E9
		ret

loc_399E2:
		add	esi, 78h
		call	sub_399E9
		ret
sub_39969	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_399E9	proc near

		cmp	byte ptr _LevelMap[esi], 1Fh
		jnz	short loc_399F1
		ret

loc_399F1:
		mov	word ptr _LevelMap[esi], 0
		;push	si
		;push	ds
		;mov	di, [si+60CBh]
		;mov	si, word_40870
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 10h

;loc_39A08:				; CODE XREF: sub_399E9+27j
;		movsb
;		movsb
;		add	si, 78h	; 'x'
;		add	di, 78h	; 'x'
;		loop	loc_39A08
;		pop	ds
;		pop	si
		ret
sub_399E9	endp


_LoadLevelMap	proc near

		push	edi

		mov		edi, offset SomeLevelData
		xor		al, al
		mov		ecx, 1536
		cld
		rep 	stosb

		pop edi
		ret

_LoadLevelMap	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

xxx_HandleMurphy proc near

		mov	word_403BF, 1
		cmp	byte ptr _LevelMap[esi], MAP_MURPHY
		jz	short loc_3CA6E
		ret

loc_3CA6E:
		mov	word_403B7, esi
		mov	ax, word ptr _LevelMap[esi]
		cmp	ax, MAP_MURPHY
		jz	short loc_3CA7E
		jmp	loc_3D84B

loc_3CA7E:
		mov	byte_403C8, 0
		cmp	byte_4030C, 0
		jz	short loc_3CAAB
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], MAP_PORT_DOWN_TO_UP
		jz	short loc_3CAAB
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], MAP_PORT_VERTICAL
		jz	short loc_3CAAB
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], MAP_PORT_CROSS
		jz	short loc_3CAAB
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], MAP_SPACE
		jnz	short loc_3CAAB
		mov	byte_403C8, 1

loc_3CAAB:
		mov	bl, _JoystickButtons
		cmp	bl, 0
		jz	short loc_3CAB7
		jmp	loc_3CBEF


loc_3CAB7:
		mov	byte_403C3, 1
		cmp	byte_403C8, 0
		jz	short loc_3CAC6
		jmp	loc_3CF84

loc_3CAC6:
		mov	ax, word_40C4D
		and	ax, 3
		jz	short loc_3CACF
		ret

loc_3CACF:
		inc	word_403BD
		cmp	word_403BD, 4
		jnz	short loc_3CAEE
		ret

loc_3CAEE:
		cmp	word_403BD, 500
		jg	short loc_3CAF7
		ret

loc_3CAF7:
		cmp	word_403BD, 522
		jg	short loc_3CB20
		ret

loc_3CB20:
		cmp	word_403BD, 1000
		jg	short loc_3CB29
		ret

loc_3CB29:
		cmp	word_403BD, 1022
		jg	short loc_3CB52
		ret

loc_3CB52:
		cmp	word_403BD, 1600
		jg	short loc_3CB5B
		ret

loc_3CB5B:
		cmp	word_403BD, 1622
		jg	short loc_3CB84
		ret

loc_3CB84:
		cmp	word_403BD, 1654
		jg	short locret_3CBA0
		cmp	word ptr _LevelMap[esi - 2], 0
		jnz	short loc_3CBA1
		cmp	word ptr _LevelMap[esi + 2], 0
		jnz	short loc_3CBC8
		mov	word_403BD, 24h

locret_3CBA0:
loc_3CBA1:
loc_3CBC8:
		ret

loc_3CBEF:
		cmp	byte_403C8, 0
		jz	short loc_3CC23
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3CC23
		cmp	bl, 1
		jnz	short loc_3CC09
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 2
		jz	short loc_3CC23

loc_3CC09:				; CODE XREF: xxx_HandleMurphy+1A0j
		cmp	bl, 2
		jnz	short loc_3CC15
		cmp	word ptr _LevelMap[esi - 2], 2
		jz	short loc_3CC23

loc_3CC15:				; CODE XREF: xxx_HandleMurphy+1ACj
		cmp	bl, 4
		jnz	short loc_3CC21
		cmp	word ptr _LevelMap[esi + 2], 2
		jz	short loc_3CC23

loc_3CC21:				; CODE XREF: xxx_HandleMurphy+1B8j
		mov	bl, 3

loc_3CC23:				; CODE XREF: xxx_HandleMurphy+194j
					; xxx_HandleMurphy+19Bj ...
		cmp	bl, 1
		jnz	short loc_3CC2F
		mov	byte_403C3, 0
		jmp	loc_3CC98


loc_3CC2F:				; CODE XREF: xxx_HandleMurphy+1C6j
		cmp	bl, 2
		jnz	short loc_3CC3C
		mov	byte_403C3, 0
		jmp	loc_3CCFA


loc_3CC3C:				; CODE XREF: xxx_HandleMurphy+1D2j
		cmp	bl, 3
		jnz	short loc_3CC49
		mov	byte_403C3, 0
		jmp	loc_3CD74


loc_3CC49:				; CODE XREF: xxx_HandleMurphy+1DFj
		cmp	bl, 4
		jnz	short loc_3CC56
		mov	byte_403C3, 0
		jmp	loc_3CDD6


loc_3CC56:				; CODE XREF: xxx_HandleMurphy+1ECj
		cmp	bl, 5
		jnz	short loc_3CC63
		mov	byte_403C3, 0
		jmp	loc_3CE4E


loc_3CC63:				; CODE XREF: xxx_HandleMurphy+1F9j
		cmp	bl, 6
		jnz	short loc_3CC70
		mov	byte_403C3, 0
		jmp	loc_3CE78


loc_3CC70:				; CODE XREF: xxx_HandleMurphy+206j
		cmp	bl, 7
		jnz	short loc_3CC7D
		mov	byte_403C3, 0
		jmp	loc_3CEA8


loc_3CC7D:				; CODE XREF: xxx_HandleMurphy+213j
		cmp	bl, 8
		jnz	short loc_3CC8A
		mov	byte_403C3, 0
		jmp	loc_3CED2


loc_3CC8A:				; CODE XREF: xxx_HandleMurphy+220j
		cmp	bl, 9
		jnz	short loc_3CC92
		jmp	loc_3CF02


loc_3CC92:				; CODE XREF: xxx_HandleMurphy+22Dj
		mov	byte_403C3, 0
		ret


loc_3CC98:				; CODE XREF: xxx_HandleMurphy+1CDj
					; xxx_HandleMurphy+297j
		mov	ax, word ptr _LevelMap[esi - 2 * MAP_WIDTH]
		cmp	ax, 0
		jnz	short loc_3CCA4
		jmp	loc_3CF32


loc_3CCA4:				; CODE XREF: xxx_HandleMurphy+23Fj
		cmp	ax, 2
		jnz	short loc_3CCAC
		jmp	loc_3CFF3


loc_3CCAC:				; CODE XREF: xxx_HandleMurphy+247j
		cmp	al, MAP_BUG
		jnz	short loc_3CCB3
		jmp	loc_3CFE2


loc_3CCB3:				; CODE XREF: xxx_HandleMurphy+24Ej
		cmp	ax, MAP_INFOTRON
		jnz	short loc_3CCBB
		jmp	loc_3D1E6


loc_3CCBB:				; CODE XREF: xxx_HandleMurphy+256j
		cmp	ax, MAP_EXIT
		jnz	short loc_3CCC3
		jmp	loc_3D376


loc_3CCC3:				; CODE XREF: xxx_HandleMurphy+25Ej
		cmp	al, MAP_TERMINAL
		jnz	short loc_3CCCA
		jmp	loc_3D437


loc_3CCCA:				; CODE XREF: xxx_HandleMurphy+265j
		cmp	al, MAP_PORT_DOWN_TO_UP
		jnz	short loc_3CCD1
		jmp	loc_3D536


loc_3CCD1:				; CODE XREF: xxx_HandleMurphy+26Cj
		cmp	al, MAP_PORT_VERTICAL
		jnz	short loc_3CCD8
		jmp	loc_3D536


loc_3CCD8:				; CODE XREF: xxx_HandleMurphy+273j
		cmp	al, MAP_PORT_CROSS
		jnz	short loc_3CCDF
		jmp	loc_3D536


loc_3CCDF:				; CODE XREF: xxx_HandleMurphy+27Aj
		cmp	al, MAP_DISK_RED
		jnz	short loc_3CCE6
		jmp	loc_3D5C6


loc_3CCE6:				; CODE XREF: xxx_HandleMurphy+281j
		cmp	al, MAP_DISK_YELLOW
		jnz	short loc_3CCED
		jmp	loc_3D6D8


loc_3CCED:				; CODE XREF: xxx_HandleMurphy+288j
		push	esi
		sub	esi, 78h	; 'x'
		call	sub_3E0D7
		pop	esi
		jb	short locret_3CCF9
		jmp	loc_3CC98


locret_3CCF9:				; CODE XREF: xxx_HandleMurphy+295j
		ret


loc_3CCFA:				; CODE XREF: xxx_HandleMurphy+1D9j
					; xxx_HandleMurphy+311j
		mov	word_403BB, 1
		mov	ax, word ptr _LevelMap[esi - 2]
		cmp	ax, 0
		jnz	short loc_3CD0C
		jmp	loc_3CF61


loc_3CD0C:				; CODE XREF: xxx_HandleMurphy+2A7j
		cmp	ax, 2
		jnz	short loc_3CD14
		jmp	loc_3D03C


loc_3CD14:				; CODE XREF: xxx_HandleMurphy+2AFj
		cmp	al, 19h
		jnz	short loc_3CD1B
		jmp	loc_3D02B


loc_3CD1B:				; CODE XREF: xxx_HandleMurphy+2B6j
		cmp	ax, 4
		jnz	short loc_3CD23
		jmp	loc_3D21E


loc_3CD23:				; CODE XREF: xxx_HandleMurphy+2BEj
		cmp	ax, 7
		jnz	short loc_3CD2B
		jmp	loc_3D376


loc_3CD2B:				; CODE XREF: xxx_HandleMurphy+2C6j
		cmp	ax, 1
		jnz	short loc_3CD33
		jmp	loc_3D3BB


loc_3CD33:				; CODE XREF: xxx_HandleMurphy+2CEj
		cmp	al, 13h
		jnz	short loc_3CD3A
		jmp	loc_3D46E


loc_3CD3A:				; CODE XREF: xxx_HandleMurphy+2D5j
		cmp	al, 0Bh
		jnz	short loc_3CD41
		jmp	loc_3D55A


loc_3CD41:				; CODE XREF: xxx_HandleMurphy+2DCj
		cmp	al, 16h
		jnz	short loc_3CD48
		jmp	loc_3D55A


loc_3CD48:				; CODE XREF: xxx_HandleMurphy+2E3j
		cmp	al, 17h
		jnz	short loc_3CD4F
		jmp	loc_3D55A


loc_3CD4F:				; CODE XREF: xxx_HandleMurphy+2EAj
		cmp	ax, 14h
		jnz	short loc_3CD57
		jmp	loc_3D5EE


loc_3CD57:				; CODE XREF: xxx_HandleMurphy+2F2j
		cmp	ax, 12h
		jnz	short loc_3CD5F
		jmp	loc_3D711


loc_3CD5F:				; CODE XREF: xxx_HandleMurphy+2FAj
		cmp	ax, 8
		jnz	short loc_3CD67
		jmp	loc_3D7BB


loc_3CD67:				; CODE XREF: xxx_HandleMurphy+302j
		push	esi
		sub	esi, 2
		call	sub_3E0D7
		pop	esi
		jb	short locret_3CD73
		jmp	loc_3CCFA


locret_3CD73:				; CODE XREF: xxx_HandleMurphy+30Fj
		ret


loc_3CD74:				; CODE XREF: xxx_HandleMurphy+1E6j
					; xxx_HandleMurphy+373j
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
		cmp	ax, 0
		jnz	short loc_3CD80
		jmp	loc_3CF84


loc_3CD80:				; CODE XREF: xxx_HandleMurphy+31Bj
		cmp	ax, 2
		jnz	short loc_3CD88
		jmp	loc_3D079


loc_3CD88:				; CODE XREF: xxx_HandleMurphy+323j
		cmp	al, 19h
		jnz	short loc_3CD8F
		jmp	loc_3D068


loc_3CD8F:				; CODE XREF: xxx_HandleMurphy+32Aj
		cmp	ax, 4
		jnz	short loc_3CD97
		jmp	loc_3D24A


loc_3CD97:				; CODE XREF: xxx_HandleMurphy+332j
		cmp	ax, 7
		jnz	short loc_3CD9F
		jmp	loc_3D376


loc_3CD9F:				; CODE XREF: xxx_HandleMurphy+33Aj
		cmp	al, 13h
		jnz	short loc_3CDA6
		jmp	loc_3D4A4


loc_3CDA6:				; CODE XREF: xxx_HandleMurphy+341j
		cmp	al, 0Ah
		jnz	short loc_3CDAD
		jmp	loc_3D57E


loc_3CDAD:				; CODE XREF: xxx_HandleMurphy+348j
		cmp	al, 15h
		jnz	short loc_3CDB4
		jmp	loc_3D57E


loc_3CDB4:				; CODE XREF: xxx_HandleMurphy+34Fj
		cmp	al, 17h
		jnz	short loc_3CDBB
		jmp	loc_3D57E


loc_3CDBB:				; CODE XREF: xxx_HandleMurphy+356j
		cmp	al, 14h
		jnz	short loc_3CDC2
		jmp	loc_3D617


loc_3CDC2:				; CODE XREF: xxx_HandleMurphy+35Dj
		cmp	al, 12h
		jnz	short loc_3CDC9
		jmp	loc_3D74A


loc_3CDC9:				; CODE XREF: xxx_HandleMurphy+364j
		push	esi
		add	esi, 78h	; 'x'
		call	sub_3E0D7
		pop	esi
		jb	short locret_3CDD5
		jmp	loc_3CD74


locret_3CDD5:				; CODE XREF: xxx_HandleMurphy+371j
		ret


loc_3CDD6:				; CODE XREF: xxx_HandleMurphy+1F3j
					; xxx_HandleMurphy+3EBj
		mov	word_403BB, 0
		mov	ax, word ptr _LevelMap[esi + 2]
		cmp	ax, 0
		jnz	short loc_3CDE8
		jmp	loc_3CFB9


loc_3CDE8:				; CODE XREF: xxx_HandleMurphy+383j
		cmp	ax, 2
		jnz	short loc_3CDF0
		jmp	loc_3D0C2


loc_3CDF0:				; CODE XREF: xxx_HandleMurphy+38Bj
		cmp	al, 19h
		jnz	short loc_3CDF7
		jmp	loc_3D0B1


loc_3CDF7:				; CODE XREF: xxx_HandleMurphy+392j
		cmp	ax, 4
		jnz	short loc_3CDFF
		jmp	loc_3D282


loc_3CDFF:				; CODE XREF: xxx_HandleMurphy+39Aj
		cmp	ax, 7
		jnz	short loc_3CE07
		jmp	loc_3D376


loc_3CE07:				; CODE XREF: xxx_HandleMurphy+3A2j
		cmp	ax, 1
		jnz	short loc_3CE0F
		jmp	loc_3D3F4


loc_3CE0F:				; CODE XREF: xxx_HandleMurphy+3AAj
		cmp	al, 13h
		jnz	short loc_3CE16
		jmp	loc_3D4DA


loc_3CE16:				; CODE XREF: xxx_HandleMurphy+3B1j
		cmp	al, 9
		jnz	short loc_3CE1D
		jmp	loc_3D5A2


loc_3CE1D:				; CODE XREF: xxx_HandleMurphy+3B8j
		cmp	al, 16h
		jnz	short loc_3CE24
		jmp	loc_3D5A2


loc_3CE24:				; CODE XREF: xxx_HandleMurphy+3BFj
		cmp	al, 17h
		jnz	short loc_3CE2B
		jmp	loc_3D5A2


loc_3CE2B:				; CODE XREF: xxx_HandleMurphy+3C6j
		cmp	al, 14h
		jnz	short loc_3CE32
		jmp	loc_3D63F


loc_3CE32:				; CODE XREF: xxx_HandleMurphy+3CDj
		cmp	al, 12h
		jnz	short loc_3CE39
		jmp	loc_3D783


loc_3CE39:				; CODE XREF: xxx_HandleMurphy+3D4j
		cmp	ax, 8
		jnz	short loc_3CE41
		jmp	loc_3D7F3


loc_3CE41:				; CODE XREF: xxx_HandleMurphy+3DCj
		push	esi
		add	esi, 2
		call	sub_3E0D7
		pop	esi
		jb	short locret_3CE4D
		jmp	loc_3CDD6


locret_3CE4D:				; CODE XREF: xxx_HandleMurphy+3E9j
		ret


loc_3CE4E:				; CODE XREF: xxx_HandleMurphy+200j
		mov	ax, word ptr _LevelMap[esi - 2 * MAP_WIDTH]
		cmp	ax, 2
		jnz	short loc_3CE5A
		jmp	loc_3D0FF


loc_3CE5A:				; CODE XREF: xxx_HandleMurphy+3F5j
		cmp	al, 19h
		jnz	short loc_3CE61
		jmp	loc_3D0EE


loc_3CE61:				; CODE XREF: xxx_HandleMurphy+3FCj
		cmp	ax, 4
		jnz	short loc_3CE69
		jmp	loc_3D2AE


loc_3CE69:				; CODE XREF: xxx_HandleMurphy+404j
		cmp	al, 13h
		jnz	short loc_3CE70
		jmp	loc_3D437


loc_3CE70:				; CODE XREF: xxx_HandleMurphy+40Bj
		cmp	al, 14h
		jnz	short locret_3CE77
		jmp	loc_3D668


locret_3CE77:				; CODE XREF: xxx_HandleMurphy+412j
		ret


loc_3CE78:				; CODE XREF: xxx_HandleMurphy+20Dj
		mov	word_403BB, 1
		mov	ax, word ptr _LevelMap[esi - 2]
		cmp	ax, 2
		jnz	short loc_3CE8A
		jmp	loc_3D13D


loc_3CE8A:				; CODE XREF: xxx_HandleMurphy+425j
		cmp	al, 19h
		jnz	short loc_3CE91
		jmp	loc_3D12C


loc_3CE91:				; CODE XREF: xxx_HandleMurphy+42Cj
		cmp	ax, 4
		jnz	short loc_3CE99
		jmp	loc_3D2E0


loc_3CE99:				; CODE XREF: xxx_HandleMurphy+434j
		cmp	al, 13h
		jnz	short loc_3CEA0
		jmp	loc_3D46E


loc_3CEA0:				; CODE XREF: xxx_HandleMurphy+43Bj
		cmp	al, 14h
		jnz	short locret_3CEA7
		jmp	loc_3D684


locret_3CEA7:				; CODE XREF: xxx_HandleMurphy+442j
		ret


loc_3CEA8:				; CODE XREF: xxx_HandleMurphy+21Aj
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
		cmp	ax, 2
		jnz	short loc_3CEB4
		jmp	loc_3D17B


loc_3CEB4:				; CODE XREF: xxx_HandleMurphy+44Fj
		cmp	al, 19h
		jnz	short loc_3CEBB
		jmp	loc_3D16A


loc_3CEBB:				; CODE XREF: xxx_HandleMurphy+456j
		cmp	ax, 4
		jnz	short loc_3CEC3
		jmp	loc_3D312


loc_3CEC3:				; CODE XREF: xxx_HandleMurphy+45Ej
		cmp	al, 13h
		jnz	short loc_3CECA
		jmp	loc_3D4A4


loc_3CECA:				; CODE XREF: xxx_HandleMurphy+465j
		cmp	al, 14h
		jnz	short locret_3CED1
		jmp	loc_3D6A0


locret_3CED1:				; CODE XREF: xxx_HandleMurphy+46Cj
		ret


loc_3CED2:				; CODE XREF: xxx_HandleMurphy+227j
		mov	word_403BB, 0
		mov	ax, word ptr _LevelMap[esi + 2]
		cmp	ax, 2
		jnz	short loc_3CEE4
		jmp	loc_3D1B9


loc_3CEE4:				; CODE XREF: xxx_HandleMurphy+47Fj
		cmp	al, 19h
		jnz	short loc_3CEEB
		jmp	loc_3D1A8


loc_3CEEB:				; CODE XREF: xxx_HandleMurphy+486j
		cmp	ax, 4
		jnz	short loc_3CEF3
		jmp	loc_3D344


loc_3CEF3:				; CODE XREF: xxx_HandleMurphy+48Ej
		cmp	al, 13h
		jnz	short loc_3CEFA
		jmp	loc_3D4DA


loc_3CEFA:				; CODE XREF: xxx_HandleMurphy+495j
		cmp	al, 14h
		jnz	short locret_3CF01
		jmp	loc_3D6BC


locret_3CF01:				; CODE XREF: xxx_HandleMurphy+49Cj
		ret


loc_3CF02:				; CODE XREF: xxx_HandleMurphy+22Fj
		cmp	byte_40C4C, 0
		jz	short locret_3CF31
		cmp	byte_403CB, 0
		jnz	short locret_3CF31
		cmp	byte_403C3, 1
		jnz	short locret_3CF31
		mov	byte ptr _LevelMap[esi + 1], 2Ah ; '*'
		mov	word_403DE, 40h	; '@'
		mov	dx, 110Eh
		mov	byte_403CB, 1
		mov	word_403CC, esi
		jmp	loc_3D835


locret_3CF31:				; CODE XREF: xxx_HandleMurphy+4A7j
					; xxx_HandleMurphy+4AEj ...
		ret


loc_3CF32:				; CODE XREF: xxx_HandleMurphy+241j
		cmp	word_403BB, 0
		jz	short loc_3CF3E
		mov	dx, 0DFEh
		jmp	short loc_3CF41


loc_3CF3E:				; CODE XREF: xxx_HandleMurphy+4D7j
		mov	dx, 0E0Eh

loc_3CF41:				; CODE XREF: xxx_HandleMurphy+4DCj
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 1
		mov	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		sub	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3CF61:				; CODE XREF: xxx_HandleMurphy+2A9j
		mov	dx, 0E1Eh
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi - 1], 2
		mov	byte ptr _LevelMap[esi - 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		sub	esi, 2
		jmp	loc_3D835


loc_3CF84:				; CODE XREF: xxx_HandleMurphy+63j
					; xxx_HandleMurphy+31Dj
		cmp	word_403BB, 0
		jz	short loc_3CF90
		mov	dx, 0E2Eh
		jmp	short loc_3CF93


loc_3CF90:				; CODE XREF: xxx_HandleMurphy+529j
		mov	dx, 0E3Eh

loc_3CF93:				; CODE XREF: xxx_HandleMurphy+52Ej
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 3
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		add	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3CFB9:				; CODE XREF: xxx_HandleMurphy+385j
		mov	dx, 0E4Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 3], 4
		mov	byte ptr _LevelMap[esi + 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		add	esi, 2
		jmp	loc_3D835


loc_3CFE2:				; CODE XREF: xxx_HandleMurphy+250j
		cmp	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 0
		jl	short loc_3CFED
		call	sub_3956F
		ret


loc_3CFED:				; CODE XREF: xxx_HandleMurphy+587j
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 2

loc_3CFF3:				; CODE XREF: xxx_HandleMurphy+249j
		;call	SND_bbb
		cmp	word_403BB, 0
		jz	short loc_3D002
		mov	dx, 0E6Eh
		jmp	short loc_3D005


loc_3D002:				; CODE XREF: xxx_HandleMurphy+59Bj
		mov	dx, 0E7Eh

loc_3D005:				; CODE XREF: xxx_HandleMurphy+5A0j
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 5
		mov	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		sub	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3D02B:				; CODE XREF: xxx_HandleMurphy+2B8j
		cmp	byte ptr _LevelMap[esi - 1], 0
		jl	short loc_3D036
		call	sub_3956F
		ret


loc_3D036:				; CODE XREF: xxx_HandleMurphy+5D0j
		mov	word ptr _LevelMap[esi - 2], 2

loc_3D03C:				; CODE XREF: xxx_HandleMurphy+2B1j
		;call	SND_bbb
		mov	dx, 0E8Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi - 1], 2
		mov	byte ptr _LevelMap[esi - 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		sub	esi, 2
		jmp	loc_3D835


loc_3D068:				; CODE XREF: xxx_HandleMurphy+32Cj
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 0
		jl	short loc_3D073
		call	sub_3956F
		ret


loc_3D073:				; CODE XREF: xxx_HandleMurphy+60Dj
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 2

loc_3D079:				; CODE XREF: xxx_HandleMurphy+325j
		;call	SND_bbb
		cmp	word_403BB, 0
		jz	short loc_3D088
		mov	dx, 0E9Eh
		jmp	short loc_3D08B


loc_3D088:				; CODE XREF: xxx_HandleMurphy+621j
		mov	dx, 0EAEh

loc_3D08B:				; CODE XREF: xxx_HandleMurphy+626j
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 7
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		add	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3D0B1:				; CODE XREF: xxx_HandleMurphy+394j
		cmp	byte ptr _LevelMap[esi + 3], 0
		jl	short loc_3D0BC
		call	sub_3956F
		ret


loc_3D0BC:				; CODE XREF: xxx_HandleMurphy+656j
		mov	word ptr _LevelMap[esi + 2], 2

loc_3D0C2:				; CODE XREF: xxx_HandleMurphy+38Dj
		;call	SND_bbb
		mov	dx, 0EBEh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 3], 8
		mov	byte ptr _LevelMap[esi + 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		add	esi, 2
		jmp	loc_3D835


loc_3D0EE:				; CODE XREF: xxx_HandleMurphy+3FEj
		cmp	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 0
		jl	short loc_3D0F9
		call	sub_3956F
		ret


loc_3D0F9:				; CODE XREF: xxx_HandleMurphy+693j
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 2

loc_3D0FF:				; CODE XREF: xxx_HandleMurphy+3F7j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+10h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_bbb
		mov	dx, 0ECEh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 10h
		jmp	loc_3D835


loc_3D12C:				; CODE XREF: xxx_HandleMurphy+42Ej
		cmp	byte ptr _LevelMap[esi - 1], 0
		jl	short loc_3D137
		call	sub_3956F
		ret


loc_3D137:				; CODE XREF: xxx_HandleMurphy+6D1j
		mov	word ptr _LevelMap[esi - 2], 2

loc_3D13D:				; CODE XREF: xxx_HandleMurphy+427j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+12h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_bbb
		mov	dx, 0EDEh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 11h
		jmp	loc_3D835


loc_3D16A:				; CODE XREF: xxx_HandleMurphy+458j
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 0
		jl	short loc_3D175
		call	sub_3956F
		ret


loc_3D175:				; CODE XREF: xxx_HandleMurphy+70Fj
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 2

loc_3D17B:				; CODE XREF: xxx_HandleMurphy+451j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+14h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_bbb
		mov	dx, 0EEEh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 12h
		jmp	loc_3D835


loc_3D1A8:				; CODE XREF: xxx_HandleMurphy+488j
		cmp	byte ptr _LevelMap[esi + 3], 0
		jl	short loc_3D1B3
		call	sub_3956F
		ret


loc_3D1B3:				; CODE XREF: xxx_HandleMurphy+74Dj
		mov	word ptr _LevelMap[esi + 2], 2

loc_3D1B9:				; CODE XREF: xxx_HandleMurphy+481j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+16h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_bbb
		mov	dx, 0EFEh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 13h
		jmp	loc_3D835


loc_3D1E6:				; CODE XREF: xxx_HandleMurphy+258j
		;call	SND_ppp
		cmp	word_403BB, 0
		jz	short loc_3D1F5
		mov	dx, 0F0Eh
		jmp	short loc_3D1F8


loc_3D1F5:				; CODE XREF: xxx_HandleMurphy+78Ej
		mov	dx, 0F1Eh

loc_3D1F8:				; CODE XREF: xxx_HandleMurphy+793j
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 9
		mov	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		sub	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3D21E:				; CODE XREF: xxx_HandleMurphy+2C0j
		;call	SND_ppp
		mov	dx, 0F2Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi - 1], 0Ah
		mov	byte ptr _LevelMap[esi - 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		sub	esi, 2
		jmp	loc_3D835


loc_3D24A:				; CODE XREF: xxx_HandleMurphy+334j
		;call	SND_ppp
		cmp	word_403BB, 0
		jz	short loc_3D259
		mov	dx, 0F3Eh
		jmp	short loc_3D25C


loc_3D259:				; CODE XREF: xxx_HandleMurphy+7F2j
		mov	dx, 0F4Eh

loc_3D25C:				; CODE XREF: xxx_HandleMurphy+7F7j
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 0Bh
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		add	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3D282:				; CODE XREF: xxx_HandleMurphy+39Cj
		;call	SND_ppp
		mov	dx, 0F5Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 3], 0Ch
		mov	byte ptr _LevelMap[esi + 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		add	esi, 2
		jmp	loc_3D835


loc_3D2AE:				; CODE XREF: xxx_HandleMurphy+406j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+10h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_ppp
		mov	dx, 0F6Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 14h
		mov	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 0FFh
		jmp	loc_3D835


loc_3D2E0:				; CODE XREF: xxx_HandleMurphy+436j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+12h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_ppp
		mov	dx, 0F7Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 15h
		mov	byte ptr _LevelMap[esi - 1], 0FFh
		jmp	loc_3D835


loc_3D312:				; CODE XREF: xxx_HandleMurphy+460j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+14h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_ppp
		mov	dx, 0F8Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 16h
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 0FFh
		jmp	loc_3D835


loc_3D344:				; CODE XREF: xxx_HandleMurphy+490j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+16h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		;call	SND_ppp
		mov	dx, 0F9Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 17h
		mov	byte ptr _LevelMap[esi + 3], 0FFh
		jmp	loc_3D835


loc_3D376:				; CODE XREF: xxx_HandleMurphy+260j
					; xxx_HandleMurphy+2C8j ...
		cmp	InfotronsLeftToCollect,	0
		jnz	short locret_3D3BA
		;call	SND_ccc
		push	esi
		cmp	IsDemoRunning, 0
		jnz	short loc_3D397
		mov	byte_403AB, 1
		mov	byte_403AA, 0
		;call	sub_3BF39
		jmp	short loc_3D39C


loc_3D397:				; CODE XREF: xxx_HandleMurphy+926j
		mov	byte_403AA, 0

loc_3D39C:				; CODE XREF: xxx_HandleMurphy+935j
		mov	TimeoutAfterWhichLeaveTheLevel, 40h	; '@'
		pop	esi
		mov	dx, 0E5Eh
		mov	word_403C9, 0
		mov	word_403DE, 0
		mov	byte ptr _LevelMap[esi + 1], 0Dh
		jmp	loc_3D835


locret_3D3BA:				; CODE XREF: xxx_HandleMurphy+91Bj
		ret


loc_3D3BB:				; CODE XREF: xxx_HandleMurphy+2D0j
		mov	ax, word ptr _LevelMap[esi - 4]
		cmp	ax, 0
		jz	short loc_3D3C5
		ret


loc_3D3C5:				; CODE XREF: xxx_HandleMurphy+962j
		mov	byte ptr _LevelMap[esi - 3], 1
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086A
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 0FAEh
		mov	word_403C9, 0
		mov	word_403DE, 8
		mov	byte ptr _LevelMap[esi + 1], 0Eh
		jmp	loc_3D835


loc_3D3F4:				; CODE XREF: xxx_HandleMurphy+3ACj
		mov	ax, word ptr _LevelMap[esi + 4]
		cmp	ax, 0
		jz	short loc_3D3FE
		ret


loc_3D3FE:				; CODE XREF: xxx_HandleMurphy+99Bj
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2]
		cmp	ax, 0
		jnz	short loc_3D408
		ret


loc_3D408:				; CODE XREF: xxx_HandleMurphy+9A5j
		mov	byte ptr _LevelMap[esi + 5], 1
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086C
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 0FBEh
		mov	word_403C9, 0
		mov	word_403DE, 8
		mov	byte ptr _LevelMap[esi + 1], 0Fh
		jmp	loc_3D835


loc_3D437:				; CODE XREF: xxx_HandleMurphy+267j
					; xxx_HandleMurphy+40Dj
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+10h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		cmp	byte_40C5B, 0
		jz	short loc_3D458
		mov	word_403BD, 0Ah
		ret


loc_3D458:				; CODE XREF: xxx_HandleMurphy+9EFj
		;push	si
		;mov	di, [si+6053h]
		;mov	si, SomeVideoMemOffsets+18h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		jmp	loc_3D510


loc_3D46E:				; CODE XREF: xxx_HandleMurphy+2D7j
					; xxx_HandleMurphy+43Dj
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+12h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		cmp	byte_40C5B, 0
		jz	short loc_3D48F
		mov	word_403BD, 0Ah
		ret


loc_3D48F:				; CODE XREF: xxx_HandleMurphy+A26j
		;push	si
		;mov	di, [si+60C9h]
		;mov	si, SomeVideoMemOffsets+18h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		jmp	loc_3D510


loc_3D4A4:				; CODE XREF: xxx_HandleMurphy+343j
					; xxx_HandleMurphy+467j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+14h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		cmp	byte_40C5B, 0
		jz	short loc_3D4C5
		mov	word_403BD, 0Ah
		ret


loc_3D4C5:				; CODE XREF: xxx_HandleMurphy+A5Cj
		;push	si
		;mov	di, [si+6143h]
		;mov	si, SomeVideoMemOffsets+18h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		jmp	short loc_3D510


loc_3D4DA:				; CODE XREF: xxx_HandleMurphy+3B3j
					; xxx_HandleMurphy+497j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, SomeVideoMemOffsets+16h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		cmp	byte_40C5B, 0
		jz	short loc_3D4FB
		mov	word_403BD, 0Ah
		ret


loc_3D4FB:				; CODE XREF: xxx_HandleMurphy+A92j
		;push	si
		;mov	di, [si+60CDh]
		;mov	si, SomeVideoMemOffsets+18h
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		jmp	short loc_3D510

loc_3D510:				; CODE XREF: xxx_HandleMurphy+A0Bj
					; xxx_HandleMurphy+A42j ...
		mov	byte_40C5A, 7
		mov	byte_40C5B, 1
		push	esi
		mov	esi, 0
		mov	cx, MAP_WIDTH * MAP_HEIGHT

loc_3D521:				; CODE XREF: xxx_HandleMurphy+AD2j
		cmp	word ptr _LevelMap[esi], 12h
		jnz	short loc_3D52F
		push	cx
		push	esi
		call	sub_3956F
		pop	esi
		pop	ecx

loc_3D52F:				; CODE XREF: xxx_HandleMurphy+AC6j
		add	esi, 2
		loop	loc_3D521
		pop	esi
		ret


loc_3D536:				; CODE XREF: xxx_HandleMurphy+26Ej
					; xxx_HandleMurphy+275j ...
		cmp	word ptr _LevelMap[esi - (4 * MAP_WIDTH)], 0
		jz	short loc_3D53E
		ret


loc_3D53E:				; CODE XREF: xxx_HandleMurphy+ADBj
		mov	dx, 0FCEh
		mov	word_403DE, 0
		mov	word_403C9, 1
		mov	byte ptr _LevelMap[esi + 1], 18h
		mov	byte ptr _LevelMap[esi - (4 * MAP_WIDTH - 1)], 3
		jmp	loc_3D835


loc_3D55A:				; CODE XREF: xxx_HandleMurphy+2DEj
					; xxx_HandleMurphy+2E5j ...
		cmp	word ptr _LevelMap[esi - 4], 0
		jz	short loc_3D562
		ret


loc_3D562:				; CODE XREF: xxx_HandleMurphy+AFFj
		mov	dx, 0FDEh
		mov	word_403DE, 0
		mov	word_403C9, 1
		mov	byte ptr _LevelMap[esi + 1], 19h
		mov	byte ptr _LevelMap[esi - 3], 3
		jmp	loc_3D835


loc_3D57E:				; CODE XREF: xxx_HandleMurphy+34Aj
					; xxx_HandleMurphy+351j ...
		cmp	word ptr _LevelMap[esi + (4 * MAP_WIDTH)], 0
		jz	short loc_3D586
		ret


loc_3D586:				; CODE XREF: xxx_HandleMurphy+B23j
		mov	dx, 0FEEh
		mov	word_403DE, 0
		mov	word_403C9, 1
		mov	byte ptr _LevelMap[esi + 1], 1Ah
		mov	byte ptr _LevelMap[esi + MAP_WIDTH * 4 + 1], 3
		jmp	loc_3D835


loc_3D5A2:				; CODE XREF: xxx_HandleMurphy+3BAj
					; xxx_HandleMurphy+3C1j ...
		cmp	word ptr _LevelMap[esi + 4], 0
		jz	short loc_3D5AA
		ret


loc_3D5AA:				; CODE XREF: xxx_HandleMurphy+B47j
		mov	dx, 0FFEh
		mov	word_403DE, 0
		mov	word_403C9, 1
		mov	byte ptr _LevelMap[esi + 1], 1Bh
		mov	byte ptr _LevelMap[esi + 5], 3
		jmp	loc_3D835


loc_3D5C6:				; CODE XREF: xxx_HandleMurphy+283j
		cmp	word_403BB, 0
		jz	short loc_3D5D2
		mov	dx, 100Eh
		jmp	short loc_3D5D5


loc_3D5D2:				; CODE XREF: xxx_HandleMurphy+B6Bj
		mov	dx, 101Eh

loc_3D5D5:				; CODE XREF: xxx_HandleMurphy+B70j
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 1Ch
		mov	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 3
		jmp	loc_3D835


loc_3D5EE:				; CODE XREF: xxx_HandleMurphy+2F4j
		mov	dx, 102Eh
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi - 1], 1Dh
		mov	byte ptr _LevelMap[esi - 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		sub	esi, 2
		jmp	loc_3D835


loc_3D617:				; CODE XREF: xxx_HandleMurphy+35Fj
		cmp	word_403BB, 0
		jz	short loc_3D623
		mov	dx, 103Eh
		jmp	short loc_3D626


loc_3D623:				; CODE XREF: xxx_HandleMurphy+BBCj
		mov	dx, 104Eh

loc_3D626:				; CODE XREF: xxx_HandleMurphy+BC1j
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 1Eh
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 3
		jmp	loc_3D835


loc_3D63F:				; CODE XREF: xxx_HandleMurphy+3CFj
		mov	dx, 105Eh
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 3], 1Fh
		mov	byte ptr _LevelMap[esi + 2], 3
		mov	byte ptr _LevelMap[esi + 1], 3
		mov	byte ptr _LevelMap[esi], 0
		add	esi, 2
		jmp	loc_3D835


loc_3D668:				; CODE XREF: xxx_HandleMurphy+414j
		mov	dx, 106Eh
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 20h ; ' '
		mov	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 3
		jmp	loc_3D835


loc_3D684:				; CODE XREF: xxx_HandleMurphy+444j
		mov	dx, 107Eh
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 21h ; '!'
		mov	byte ptr _LevelMap[esi - 1], 3
		jmp	loc_3D835


loc_3D6A0:				; CODE XREF: xxx_HandleMurphy+46Ej
		mov	dx, 108Eh
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 22h ; '"'
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 3
		jmp	loc_3D835


loc_3D6BC:				; CODE XREF: xxx_HandleMurphy+49Ej
		mov	dx, 109Eh
		mov	word_403DE, 0
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 23h ; '#'
		mov	byte ptr _LevelMap[esi + 3], 3
		jmp	loc_3D835


loc_3D6D8:				; CODE XREF: xxx_HandleMurphy+28Aj
		mov	ax, word ptr _LevelMap[esi - (4 * MAP_WIDTH)]
		cmp	ax, 0
		jz	short loc_3D6E2
		ret


loc_3D6E2:				; CODE XREF: xxx_HandleMurphy+C7Fj
		mov	byte ptr _LevelMap[esi - (4 * MAP_WIDTH - 1)], 12h
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086C
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 10AEh
		mov	word_403DE, 8
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 24h ; '$'
		jmp	loc_3D835


loc_3D711:				; CODE XREF: xxx_HandleMurphy+2FCj
		mov	ax, word ptr _LevelMap[esi - 4]
		cmp	ax, 0
		jz	short loc_3D71B
		ret


loc_3D71B:				; CODE XREF: xxx_HandleMurphy+CB8j
		mov	byte ptr _LevelMap[esi - 3], 12h
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086A
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 10BEh
		mov	word_403DE, 8
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 25h ; '%'
		jmp	loc_3D835


loc_3D74A:				; CODE XREF: xxx_HandleMurphy+366j
		mov	ax, word ptr _LevelMap[esi + (4 * MAP_WIDTH)]
		cmp	ax, 0
		jz	short loc_3D754
		ret


loc_3D754:				; CODE XREF: xxx_HandleMurphy+CF1j
		mov	byte ptr _LevelMap[esi + MAP_WIDTH * 4 + 1], 12h
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086C
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 10CEh
		mov	word_403DE, 8
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 27h ; '''
		jmp	loc_3D835


loc_3D783:				; CODE XREF: xxx_HandleMurphy+3D6j
		mov	ax, word ptr _LevelMap[esi + 4]
		cmp	ax, 0
		jz	short loc_3D78D
		ret


loc_3D78D:				; CODE XREF: xxx_HandleMurphy+D2Aj
		mov	byte ptr _LevelMap[esi + 5], 12h
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086C
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 10DEh
		mov	word_403DE, 8
		mov	word_403C9, 0
		mov	byte ptr _LevelMap[esi + 1], 26h ; '&'
		jmp	loc_3D835


loc_3D7BB:				; CODE XREF: xxx_HandleMurphy+304j
		mov	ax, word ptr _LevelMap[esi - 4]
		cmp	ax, 0
		jz	short loc_3D7C5
		ret


loc_3D7C5:				; CODE XREF: xxx_HandleMurphy+D62j
		mov	byte ptr _LevelMap[esi - 3], 8
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086A
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 10EEh
		mov	word_403C9, 0
		mov	word_403DE, 8
		mov	byte ptr _LevelMap[esi + 1], 28h ; '('
		jmp	short loc_3D835


loc_3D7F3:				; CODE XREF: xxx_HandleMurphy+3DEj
		mov	ax, word ptr _LevelMap[esi + 4]
		cmp	ax, 0
		jz	short loc_3D7FD
		ret


loc_3D7FD:				; CODE XREF: xxx_HandleMurphy+D9Aj
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH + 2]
		cmp	ax, 0
		jnz	short loc_3D807
		ret


loc_3D807:				; CODE XREF: xxx_HandleMurphy+DA4j
		mov	byte ptr _LevelMap[esi + 5], 1
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086C
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		mov	dx, 10FEh
		mov	word_403C9, 0
		mov	word_403DE, 8
		mov	byte ptr _LevelMap[esi + 1], 29h ; ')'
		jmp	short $+2

loc_3D835:				; CODE XREF: xxx_HandleMurphy+4CEj
					; xxx_HandleMurphy+4FEj ...
		;push	esi
		;push	edi
		;push	es
		;mov	si, dx
		;mov	di, 0DE0h
		;mov	ax, ds
		;mov	es, ax
		;mov	cx, 7
		;rep movsw
		;pop	es
		;pop	di
		;pop	si
		jmp	short loc_3D84B

loc_3D84B:				; CODE XREF: xxx_HandleMurphy+1Bj
		mov	word_403BD, 0
		mov	ax, word_403DE
		cmp	ax, 0
		jz	short loc_3D8AF
		dec	ax
		mov	word_403DE, ax
		jnz	short loc_3D862
		;call	SND_mmm

loc_3D862:				; CODE XREF: xxx_HandleMurphy+DFDj
		mov	bl, byte ptr _LevelMap[esi + 1]
		cmp	bl, 0Eh
		jnz	short loc_3D86E
		jmp	loc_3DB9E


loc_3D86E:				; CODE XREF: xxx_HandleMurphy+E09j
		cmp	bl, 0Fh
		jnz	short loc_3D876
		jmp	loc_3DBDC


loc_3D876:				; CODE XREF: xxx_HandleMurphy+E11j
		cmp	bl, 28h	; '('
		jnz	short loc_3D87E
		jmp	loc_3DC1A


loc_3D87E:				; CODE XREF: xxx_HandleMurphy+E19j
		cmp	bl, 29h	; ')'
		jnz	short loc_3D886
		jmp	loc_3DC58


loc_3D886:				; CODE XREF: xxx_HandleMurphy+E21j
		cmp	bl, 24h	; '$'
		jnz	short loc_3D88E
		jmp	loc_3DC96


loc_3D88E:				; CODE XREF: xxx_HandleMurphy+E29j
		cmp	bl, 25h	; '%'
		jnz	short loc_3D896
		jmp	loc_3DCD4


loc_3D896:				; CODE XREF: xxx_HandleMurphy+E31j
		cmp	bl, 27h	; '''
		jnz	short loc_3D89E
		jmp	loc_3DD12


loc_3D89E:				; CODE XREF: xxx_HandleMurphy+E39j
		cmp	bl, 26h	; '&'
		jnz	short loc_3D8A6
		jmp	loc_3DD50


loc_3D8A6:				; CODE XREF: xxx_HandleMurphy+E41j
		cmp	bl, 2Ah	; '*'
		jnz	short locret_3D8AE
		jmp	loc_3DD8E


locret_3D8AE:				; CODE XREF: xxx_HandleMurphy+E49j
		ret


loc_3D8AF:				; CODE XREF: xxx_HandleMurphy+DF7j
		;push	si
		;push	di
		mov	ax, word_403EA
		add	PlayerPosition_PixelsX,	ax
		mov	ax, word_403EC
		add	PlayerPosition_PixelsY,	ax
		;mov	di, [si+60CBh]
		;add	di, word_403E0
		;mov		esi, word_403E8
		;mov	ax, [si]
		;add		esi, 2
		;mov		word_403E8, esi
		;mov	si, ax
		;mov		bx, word_403E4
		;mov		dx, word_403E6
		;push	ds
		;mov	ax, es
		;mov	ds, ax

;loc_3D8E3:				; CODE XREF: xxx_HandleMurphy+E9Cj
		;mov	cx, bx
		;rep movsb
		;add	si, 7Ah	; 'z'
		;sub	si, bx
		;add	di, 7Ah	; 'z'
		;sub	di, bx
		;cmp	si, 4D34h
		;jb	short loc_3D8FB
		;sub	si, 4D0Ch

;loc_3D8FB:				; CODE XREF: xxx_HandleMurphy+E95j
		;dec	dx
		;jnz	short loc_3D8E3
		;mov	ax, ds
		;pop	ds
		;cmp	word_403C9, 0
		;jz	short loc_3D93E
		;sub	di, 7A0h
		;add	di, word_403E2
		;mov	esi, word_403E8
		;add	esi, 10h
		;mov	esi, [esi]
		;mov	dx, word_403E6
		;push	ds
		;mov	ds, ax

;loc_3D920:				; CODE XREF: xxx_HandleMurphy+ED9j
		;mov	cx, bx
		;rep movsb
		;add	si, 7Ah	; 'z'
		;sub	si, bx
		;add	di, 7Ah	; 'z'
		;sub	di, bx
		;cmp	si, 4D34h
		;jb	short loc_3D938
		;sub	si, 4D0Ch

;loc_3D938:				; CODE XREF: xxx_HandleMurphy+ED2j
		;dec	dx
		;jnz	short loc_3D920
		;pop	ds
		;jmp	short loc_3D948


;loc_3D93E:				; CODE XREF: xxx_HandleMurphy+EA6j
;		mov	ax, word_403E0
;		add	ax, word_403E2
;		mov	word_403E0, ax

;loc_3D948:				; CODE XREF: xxx_HandleMurphy+EDCj
;		mov	esi, word_403E8
;		cmp	word ptr [esi], 0FFFFh
;		jz	short loc_3D955
;		pop	di
;		pop	si
;		ret


loc_3D955:				; CODE XREF: xxx_HandleMurphy+EF0j
		;pop	edi
		;pop	esi
		mov	ax, word_403EA
		shr	ax, 1
		mov	bx, word_403EC
		shr	bx, 1
		add	PlayerPosition_MapX, ax
		add	PlayerPosition_MapY, bx
		mov	bl, byte ptr _LevelMap[esi + 1]
		mov	byte ptr _LevelMap[esi + 1], 0
		cmp	bl, 1
		jnz	short loc_3D97B
		jmp	loc_3DAE8


loc_3D97B:				; CODE XREF: xxx_HandleMurphy+F16j
		cmp	bl, 2
		jnz	short loc_3D983
		jmp	loc_3DB06


loc_3D983:				; CODE XREF: xxx_HandleMurphy+F1Ej
		cmp	bl, 3
		jnz	short loc_3D98B
		jmp	loc_3DB24


loc_3D98B:				; CODE XREF: xxx_HandleMurphy+F26j
		cmp	bl, 4
		jnz	short loc_3D993
		jmp	loc_3DDE4


loc_3D993:				; CODE XREF: xxx_HandleMurphy+F2Ej
		cmp	bl, 5
		jnz	short loc_3D99B
		jmp	loc_3DAE8


loc_3D99B:				; CODE XREF: xxx_HandleMurphy+F36j
		cmp	bl, 6
		jnz	short loc_3D9A3
		jmp	loc_3DB06


loc_3D9A3:				; CODE XREF: xxx_HandleMurphy+F3Ej
		cmp	bl, 7
		jnz	short loc_3D9AB
		jmp	loc_3DB24


loc_3D9AB:				; CODE XREF: xxx_HandleMurphy+F46j
		cmp	bl, 8
		jnz	short loc_3D9B3
		jmp	loc_3DDE4


loc_3D9B3:				; CODE XREF: xxx_HandleMurphy+F4Ej
		cmp	bl, 9
		jnz	short loc_3D9BB
		jmp	loc_3DADA


loc_3D9BB:				; CODE XREF: xxx_HandleMurphy+F56j
		cmp	bl, 0Ah
		jnz	short loc_3D9C3
		jmp	loc_3DAF8


loc_3D9C3:				; CODE XREF: xxx_HandleMurphy+F5Ej
		cmp	bl, 0Bh
		jnz	short loc_3D9CB
		jmp	@@InfotronCollected ; __linkproc__ InfotronCollected


loc_3D9CB:				; CODE XREF: xxx_HandleMurphy+F66j
		cmp	bl, 0Ch
		jnz	short loc_3D9D3
		jmp	loc_3DDD6


loc_3D9D3:				; CODE XREF: xxx_HandleMurphy+F6Ej
		cmp	bl, 0Eh
		jnz	short loc_3D9DB
		jmp	loc_3DB38


loc_3D9DB:				; CODE XREF: xxx_HandleMurphy+F76j
		cmp	bl, 0Fh
		jnz	short loc_3D9E3
		jmp	loc_3DB5B


loc_3D9E3:				; CODE XREF: xxx_HandleMurphy+F7Ej
		cmp	bl, 10h
		jnz	short loc_3D9EB
		jmp	loc_3DE02


loc_3D9EB:				; CODE XREF: xxx_HandleMurphy+F86j
		cmp	bl, 11h
		jnz	short loc_3D9F3
		jmp	loc_3DE1E


loc_3D9F3:				; CODE XREF: xxx_HandleMurphy+F8Ej
		cmp	bl, 13h
		jnz	short loc_3D9FB
		jmp	loc_3DE56


loc_3D9FB:				; CODE XREF: xxx_HandleMurphy+F96j
		cmp	bl, 12h
		jnz	short loc_3DA03
		jmp	loc_3DE3A


loc_3DA03:				; CODE XREF: xxx_HandleMurphy+F9Ej
		cmp	bl, 13h
		jnz	short loc_3DA0B
		jmp	loc_3DE56


loc_3DA0B:				; CODE XREF: xxx_HandleMurphy+FA6j
		cmp	bl, 14h
		jnz	short loc_3DA13
		jmp	loc_3DDF4


loc_3DA13:				; CODE XREF: xxx_HandleMurphy+FAEj
		cmp	bl, 15h
		jnz	short loc_3DA1B
		jmp	loc_3DE10


loc_3DA1B:				; CODE XREF: xxx_HandleMurphy+FB6j
		cmp	bl, 17h
		jnz	short loc_3DA23
		jmp	loc_3DE48


loc_3DA23:				; CODE XREF: xxx_HandleMurphy+FBEj
		cmp	bl, 16h
		jnz	short loc_3DA2B
		jmp	loc_3DE2C


loc_3DA2B:				; CODE XREF: xxx_HandleMurphy+FC6j
		cmp	bl, 17h
		jnz	short loc_3DA33
		jmp	loc_3DE48


loc_3DA33:				; CODE XREF: xxx_HandleMurphy+FCEj
		cmp	bl, 0Dh
		jnz	short loc_3DA3B
		jmp	loc_3DB97


loc_3DA3B:				; CODE XREF: xxx_HandleMurphy+FD6j
		cmp	bl, 18h
		jnz	short loc_3DA43
		jmp	loc_3DE64


loc_3DA43:				; CODE XREF: xxx_HandleMurphy+FDEj
		cmp	bl, 19h
		jnz	short loc_3DA4B
		jmp	loc_3DE92


loc_3DA4B:				; CODE XREF: xxx_HandleMurphy+FE6j
		cmp	bl, 1Ah
		jnz	short loc_3DA53
		jmp	loc_3DEBF


loc_3DA53:				; CODE XREF: xxx_HandleMurphy+FEEj
		cmp	bl, 1Bh
		jnz	short loc_3DA5B
		jmp	loc_3DEED


loc_3DA5B:				; CODE XREF: xxx_HandleMurphy+FF6j
		cmp	bl, 1Ch
		jnz	short loc_3DA63
		jmp	loc_3DF1A


loc_3DA63:				; CODE XREF: xxx_HandleMurphy+FFEj
		cmp	bl, 1Dh
		jnz	short loc_3DA6B
		jmp	loc_3DF38


loc_3DA6B:				; CODE XREF: xxx_HandleMurphy+1006j
		cmp	bl, 1Eh
		jnz	short loc_3DA73
		jmp	loc_3DF53


loc_3DA73:				; CODE XREF: xxx_HandleMurphy+100Ej
		cmp	bl, 1Fh
		jnz	short loc_3DA7B
		jmp	loc_3DF71


loc_3DA7B:				; CODE XREF: xxx_HandleMurphy+1016j
		cmp	bl, 20h	; ' '
		jnz	short loc_3DA83
		jmp	loc_3DF8C


loc_3DA83:				; CODE XREF: xxx_HandleMurphy+101Ej
		cmp	bl, 21h	; '!'
		jnz	short loc_3DA8B
		jmp	loc_3DFA1


loc_3DA8B:				; CODE XREF: xxx_HandleMurphy+1026j
		cmp	bl, 22h	; '"'
		jnz	short loc_3DA93
		jmp	loc_3DFB6


loc_3DA93:				; CODE XREF: xxx_HandleMurphy+102Ej
		cmp	bl, 23h	; '#'
		jnz	short loc_3DA9B
		jmp	loc_3DFCB


loc_3DA9B:				; CODE XREF: xxx_HandleMurphy+1036j
		cmp	bl, 24h	; '$'
		jnz	short loc_3DAA3
		jmp	loc_3DFE0


loc_3DAA3:				; CODE XREF: xxx_HandleMurphy+103Ej
		cmp	bl, 25h	; '%'
		jnz	short loc_3DAAB
		jmp	loc_3DFFD


loc_3DAAB:				; CODE XREF: xxx_HandleMurphy+1046j
		cmp	bl, 27h	; '''
		jnz	short loc_3DAB3
		jmp	loc_3E01A


loc_3DAB3:				; CODE XREF: xxx_HandleMurphy+104Ej
		cmp	bl, 26h	; '&'
		jnz	short loc_3DABB
		jmp	loc_3E037


loc_3DABB:				; CODE XREF: xxx_HandleMurphy+1056j
		cmp	bl, 28h	; '('
		jnz	short loc_3DAC3
		jmp	loc_3E054


loc_3DAC3:				; CODE XREF: xxx_HandleMurphy+105Ej
		cmp	bl, 29h	; ')'
		jnz	short loc_3DACB
		jmp	loc_3E071


loc_3DACB:				; CODE XREF: xxx_HandleMurphy+1066j
		cmp	bl, 2Ah	; '*'
		jnz	short loc_3DAD3
		jmp	loc_3E09F


loc_3DAD3:				; CODE XREF: xxx_HandleMurphy+106Ej
		mov	word_40C64, 1
		ret


loc_3DADA:				; CODE XREF: xxx_HandleMurphy+F58j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DAE5
		dec	InfotronsLeftToCollect

loc_3DAE5:				; CODE XREF: xxx_HandleMurphy+107Fj
		;call	sub_3EC01

loc_3DAE8:				; CODE XREF: xxx_HandleMurphy+F18j
					; xxx_HandleMurphy+F38j
		mov	word ptr _LevelMap[esi], 3
		add	esi, 78h	; 'x'
		call	sub_38191
		sub	esi, 78h	; 'x'
		ret


loc_3DAF8:				; CODE XREF: xxx_HandleMurphy+F60j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DB03
		dec	InfotronsLeftToCollect

loc_3DB03:				; CODE XREF: xxx_HandleMurphy+109Dj
		;call	sub_3EC01

loc_3DB06:				; CODE XREF: xxx_HandleMurphy+F20j
					; xxx_HandleMurphy+F40j
		mov	word ptr _LevelMap[esi], 3
		add	esi, 2
		call	sub_38191
		sub	esi, 2
		ret


; __linkproc__ InfotronCollected
@@InfotronCollected:			; CODE XREF: xxx_HandleMurphy+F68j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DB21
		dec	InfotronsLeftToCollect

loc_3DB21:				; CODE XREF: xxx_HandleMurphy+10BBj
		;call	sub_3EC01

loc_3DB24:				; CODE XREF: xxx_HandleMurphy+F28j
					; xxx_HandleMurphy+F48j
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 1Fh
		jz	short loc_3DB31
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0

loc_3DB31:				; CODE XREF: xxx_HandleMurphy+10C9j
		mov	word ptr _LevelMap[esi], 3
		ret


loc_3DB38:				; CODE XREF: xxx_HandleMurphy+F78j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DB45
		mov	word ptr _LevelMap[esi], 0

loc_3DB45:				; CODE XREF: xxx_HandleMurphy+10DDj
		mov	word ptr _LevelMap[esi - 2], 3
		mov	word ptr _LevelMap[esi - 4], 1
		sub	esi, 4
		call	sub_3DB7E
		add	esi, 2
		ret


loc_3DB5B:				; CODE XREF: xxx_HandleMurphy+F80j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DB68
		mov	word ptr _LevelMap[esi], 0

loc_3DB68:				; CODE XREF: xxx_HandleMurphy+1100j
		mov	word ptr _LevelMap[esi + 2], 3
		mov	word ptr _LevelMap[esi + 4], 1
		add	esi, 4
		call	sub_3DB7E
		sub	esi, 2
		ret
xxx_HandleMurphy endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3DB7E	proc near		; CODE XREF: xxx_HandleMurphy+10F4p
					; xxx_HandleMurphy+1117p
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 11h
		jz	short loc_3DB8D
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 0BBh ; 'Л'
		jz	short loc_3DB8D
		ret


loc_3DB8D:				; CODE XREF: sub_3DB7E+5j sub_3DB7E+Cj
		add	esi, 78h	; 'x'
		call	sub_3956F
		sub	esi, 78h	; 'x'
		ret
sub_3DB7E	endp


; START	OF FUNCTION CHUNK FOR xxx_HandleMurphy

loc_3DB97:				; CODE XREF: xxx_HandleMurphy+FD8j
		mov	word_40C64, 1
		ret


loc_3DB9E:				; CODE XREF: xxx_HandleMurphy+E0Bj
		mov	bl, _JoystickButtons
		cmp	bl, 2
		jnz	short loc_3DBAF
		cmp	word ptr _LevelMap[esi - 2], 1
		jnz	short loc_3DBAF
		ret


loc_3DBAF:				; CODE XREF: xxx_HandleMurphy+1145j
					; xxx_HandleMurphy+114Cj
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi - 2], 1
		cmp	byte ptr _LevelMap[esi - 4], 1Fh
		jz	short loc_3DBC8
		mov	word ptr _LevelMap[esi - 4], 0

loc_3DBC8:				; CODE XREF: xxx_HandleMurphy+1160j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DBDC:				; CODE XREF: xxx_HandleMurphy+E13j
		mov	bl, _JoystickButtons
		cmp	bl, 4
		jnz	short loc_3DBED
		cmp	word ptr _LevelMap[esi + 2], 1
		jnz	short loc_3DBED
		ret


loc_3DBED:				; CODE XREF: xxx_HandleMurphy+1183j
					; xxx_HandleMurphy+118Aj
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi + 2], 1
		cmp	byte ptr _LevelMap[esi + 4], 1Fh
		jz	short loc_3DC06
		mov	word ptr _LevelMap[esi + 4], 0

loc_3DC06:				; CODE XREF: xxx_HandleMurphy+119Ej
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DC1A:				; CODE XREF: xxx_HandleMurphy+E1Bj
		mov	bl, _JoystickButtons
		cmp	bl, 2
		jnz	short loc_3DC2B
		cmp	word ptr _LevelMap[esi - 2], 8
		jnz	short loc_3DC2B
		ret


loc_3DC2B:				; CODE XREF: xxx_HandleMurphy+11C1j
					; xxx_HandleMurphy+11C8j
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi - 2], 8
		cmp	byte ptr _LevelMap[esi - 4], 1Fh
		jz	short loc_3DC44
		mov	word ptr _LevelMap[esi - 4], 0

loc_3DC44:				; CODE XREF: xxx_HandleMurphy+11DCj
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DC58:				; CODE XREF: xxx_HandleMurphy+E23j
		mov	bl, _JoystickButtons
		cmp	bl, 4
		jnz	short loc_3DC69
		cmp	word ptr _LevelMap[esi + 2], 8
		jnz	short loc_3DC69
		ret


loc_3DC69:				; CODE XREF: xxx_HandleMurphy+11FFj
					; xxx_HandleMurphy+1206j
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi + 2], 8
		cmp	byte ptr _LevelMap[esi + 4], 1Fh
		jz	short loc_3DC82
		mov	word ptr _LevelMap[esi + 4], 0

loc_3DC82:				; CODE XREF: xxx_HandleMurphy+121Aj
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DC96:				; CODE XREF: xxx_HandleMurphy+E2Bj
		mov	bl, _JoystickButtons
		cmp	bl, 1
		jnz	short loc_3DCA7
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 12h
		jnz	short loc_3DCA7
		ret


loc_3DCA7:				; CODE XREF: xxx_HandleMurphy+123Dj
					; xxx_HandleMurphy+1244j
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 12h
		cmp	byte ptr _LevelMap[esi - (4 * MAP_WIDTH)], 1Fh
		jz	short loc_3DCC0
		mov	word ptr _LevelMap[esi - (4 * MAP_WIDTH)], 0

loc_3DCC0:				; CODE XREF: xxx_HandleMurphy+1258j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DCD4:				; CODE XREF: xxx_HandleMurphy+E33j
		mov	bl, _JoystickButtons
		cmp	bl, 2
		jnz	short loc_3DCE5
		cmp	word ptr _LevelMap[esi - 2], 12h
		jnz	short loc_3DCE5
		ret


loc_3DCE5:				; CODE XREF: xxx_HandleMurphy+127Bj
					; xxx_HandleMurphy+1282j
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi - 2], 12h
		cmp	byte ptr _LevelMap[esi - 4], 1Fh
		jz	short loc_3DCFE
		mov	word ptr _LevelMap[esi - 4], 0

loc_3DCFE:				; CODE XREF: xxx_HandleMurphy+1296j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DD12:				; CODE XREF: xxx_HandleMurphy+E3Bj
		mov	bl, _JoystickButtons
		cmp	bl, 3
		jnz	short loc_3DD23
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 12h
		jnz	short loc_3DD23
		ret


loc_3DD23:				; CODE XREF: xxx_HandleMurphy+12B9j
					; xxx_HandleMurphy+12C0j
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 12h
		cmp	byte ptr _LevelMap[esi + (4 * MAP_WIDTH)], 1Fh
		jz	short loc_3DD3C
		mov	word ptr _LevelMap[esi + (4 * MAP_WIDTH)], 0

loc_3DD3C:				; CODE XREF: xxx_HandleMurphy+12D4j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DD50:				; CODE XREF: xxx_HandleMurphy+E43j
		mov	bl, _JoystickButtons
		cmp	bl, 4
		jnz	short loc_3DD61
		cmp	word ptr _LevelMap[esi + 2], 12h
		jnz	short loc_3DD61
		ret


loc_3DD61:				; CODE XREF: xxx_HandleMurphy+12F7j
					; xxx_HandleMurphy+12FEj
		mov	word ptr _LevelMap[esi], 3
		mov	word ptr _LevelMap[esi + 2], 12h
		cmp	byte ptr _LevelMap[esi + 4], 1Fh
		jz	short loc_3DD7A
		mov	word ptr _LevelMap[esi + 4], 0

loc_3DD7A:				; CODE XREF: xxx_HandleMurphy+1312j
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		;pop	si
		ret


loc_3DD8E:				; CODE XREF: xxx_HandleMurphy+E4Bj
		mov	bl, _JoystickButtons
		cmp	bl, 9
		jnz	short loc_3DDB7
		cmp	word_403DE, 20h	; ' '
		jg	short locret_3DDB6
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, var_40A80
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		mov	byte_403CB, 1
		;pop	si

locret_3DDB6:				; CODE XREF: xxx_HandleMurphy+133Cj
		ret


loc_3DDB7:				; CODE XREF: xxx_HandleMurphy+1335j
		;push	si
		mov		word ptr _LevelMap[esi], 3
		;mov	di, [si+60CBh]
		;mov	si, word_4086E
		;mov	dx, 10h
		;mov	bx, 2
		;call	sub_3E0B5
		mov		byte_403CB, 0
		;pop	si
		ret


loc_3DDD6:				; CODE XREF: xxx_HandleMurphy+F70j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DDE1
		dec	InfotronsLeftToCollect

loc_3DDE1:				; CODE XREF: xxx_HandleMurphy+137Bj
		;call	sub_3EC01

loc_3DDE4:				; CODE XREF: xxx_HandleMurphy+F30j
					; xxx_HandleMurphy+F50j
		sub	esi, 2
		call	sub_38191
		add	esi, 2
		mov	word ptr _LevelMap[esi], 3
		ret


loc_3DDF4:				; CODE XREF: xxx_HandleMurphy+FB0j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DDFF
		dec	InfotronsLeftToCollect

loc_3DDFF:				; CODE XREF: xxx_HandleMurphy+1399j
		;call	sub_3EC01

loc_3DE02:				; CODE XREF: xxx_HandleMurphy+F88j
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 1Fh
		jz	short locret_3DE0F
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0

locret_3DE0F:				; CODE XREF: xxx_HandleMurphy+13A7j
		ret


loc_3DE10:				; CODE XREF: xxx_HandleMurphy+FB8j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DE1B
		dec	InfotronsLeftToCollect

loc_3DE1B:				; CODE XREF: xxx_HandleMurphy+13B5j
		;call	sub_3EC01

loc_3DE1E:				; CODE XREF: xxx_HandleMurphy+F90j
		cmp	byte ptr _LevelMap[esi - 2], 1Fh
		jz	short locret_3DE2B
		mov	word ptr _LevelMap[esi - 2], 0

locret_3DE2B:				; CODE XREF: xxx_HandleMurphy+13C3j
		ret


loc_3DE2C:				; CODE XREF: xxx_HandleMurphy+FC8j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DE37
		dec	InfotronsLeftToCollect

loc_3DE37:				; CODE XREF: xxx_HandleMurphy+13D1j
		;call	sub_3EC01

loc_3DE3A:				; CODE XREF: xxx_HandleMurphy+FA0j
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 1Fh
		jz	short locret_3DE47
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0

locret_3DE47:				; CODE XREF: xxx_HandleMurphy+13DFj
		ret


loc_3DE48:				; CODE XREF: xxx_HandleMurphy+FC0j
					; xxx_HandleMurphy+FD0j
		cmp	InfotronsLeftToCollect,	0
		jbe	short loc_3DE53
		dec	InfotronsLeftToCollect

loc_3DE53:				; CODE XREF: xxx_HandleMurphy+13EDj
		;call	sub_3EC01

loc_3DE56:				; CODE XREF: xxx_HandleMurphy+F98j
					; xxx_HandleMurphy+FA8j
		cmp	byte ptr _LevelMap[esi + 2], 1Fh
		jz	short locret_3DE63
		mov	word ptr _LevelMap[esi + 2], 0

locret_3DE63:				; CODE XREF: xxx_HandleMurphy+13FBj
		ret


loc_3DE64:				; CODE XREF: xxx_HandleMurphy+FE0j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DE71
		mov	word ptr _LevelMap[esi], 0

loc_3DE71:				; CODE XREF: xxx_HandleMurphy+1409j
		mov	word ptr _LevelMap[esi - (4 * MAP_WIDTH)], 3
		mov	word_403C9, 0
		sub	esi, 0F0h ; '№'
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 1], 1
		jnz	short locret_3DE91
		add	esi, 78h	; 'x'
		call	sub_3E190
		sub	esi, 78h	; 'x'

locret_3DE91:				; CODE XREF: xxx_HandleMurphy+1426j
		ret


loc_3DE92:				; CODE XREF: xxx_HandleMurphy+FE8j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DE9F
		mov	word ptr _LevelMap[esi], 0

loc_3DE9F:				; CODE XREF: xxx_HandleMurphy+1437j
		mov	word ptr _LevelMap[esi - 4], 3
		mov	word_403C9, 0
		sub	esi, 4
		cmp	byte ptr _LevelMap[esi + 3], 1
		jnz	short locret_3DEBE
		add	esi, 2
		call	sub_3E190
		sub	esi, 2

locret_3DEBE:				; CODE XREF: xxx_HandleMurphy+1453j
		ret


loc_3DEBF:				; CODE XREF: xxx_HandleMurphy+FF0j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DECC
		mov	word ptr _LevelMap[esi], 0

loc_3DECC:				; CODE XREF: xxx_HandleMurphy+1464j
		mov	word ptr _LevelMap[esi + (4 * MAP_WIDTH)], 3
		mov	word_403C9, 0
		add	esi, 0F0h ; '№'
		cmp	byte ptr _LevelMap[esi - (2 * MAP_WIDTH - 1)], 1
		jnz	short locret_3DEEC
		sub	esi, 78h	; 'x'
		call	sub_3E190
		add	esi, 78h	; 'x'

locret_3DEEC:				; CODE XREF: xxx_HandleMurphy+1481j
		ret


loc_3DEED:				; CODE XREF: xxx_HandleMurphy+FF8j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DEFA
		mov	word ptr _LevelMap[esi], 0

loc_3DEFA:				; CODE XREF: xxx_HandleMurphy+1492j
		mov	word ptr _LevelMap[esi + 4], 3
		mov	word_403C9, 0
		add	esi, 4
		cmp	byte ptr _LevelMap[esi - 1], 1
		jnz	short locret_3DF19
		sub esi, 2
		call	sub_3E190
		add	esi, 2

locret_3DF19:				; CODE XREF: xxx_HandleMurphy+14AEj
		ret


loc_3DF1A:				; CODE XREF: xxx_HandleMurphy+1000j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DF27
		mov	word ptr _LevelMap[esi], 0

loc_3DF27:				; CODE XREF: xxx_HandleMurphy+14BFj
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		inc	byte_40C4C
		;call	sub_3EC91
		sub	esi, 78h	; 'x'
		ret


loc_3DF38:				; CODE XREF: xxx_HandleMurphy+1008j
		cmp	byte ptr _LevelMap[esi + 2], 1Fh
		jz	short loc_3DF45
		mov	word ptr _LevelMap[esi + 2], 0

loc_3DF45:				; CODE XREF: xxx_HandleMurphy+14DDj
		mov	word ptr _LevelMap[esi], 3
		inc	byte_40C4C
		;call	sub_3EC91
		ret


loc_3DF53:				; CODE XREF: xxx_HandleMurphy+1010j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DF60
		mov	word ptr _LevelMap[esi], 0

loc_3DF60:				; CODE XREF: xxx_HandleMurphy+14F8j
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		inc	byte_40C4C
		;call	sub_3EC91
		add	esi, 78h	; 'x'
		ret


loc_3DF71:				; CODE XREF: xxx_HandleMurphy+1018j
		cmp	byte ptr _LevelMap[esi - 2], 1Fh
		jz	short loc_3DF7E
		mov	word ptr _LevelMap[esi - 2], 0

loc_3DF7E:				; CODE XREF: xxx_HandleMurphy+1516j
		mov	word ptr _LevelMap[esi], 3
		inc	byte_40C4C
		;call	sub_3EC91
		ret


loc_3DF8C:				; CODE XREF: xxx_HandleMurphy+1020j
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 1Fh
		jz	short loc_3DF99
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0

loc_3DF99:				; CODE XREF: xxx_HandleMurphy+1531j
		inc	byte_40C4C
		;call	sub_3EC91
		ret


loc_3DFA1:				; CODE XREF: xxx_HandleMurphy+1028j
		cmp	byte ptr _LevelMap[esi - 2], 1Fh
		jz	short loc_3DFAE
		mov	word ptr _LevelMap[esi - 2], 0

loc_3DFAE:				; CODE XREF: xxx_HandleMurphy+1546j
		inc	byte_40C4C
		;call	sub_3EC91
		ret


loc_3DFB6:				; CODE XREF: xxx_HandleMurphy+1030j
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 1Fh
		jz	short loc_3DFC3
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0

loc_3DFC3:				; CODE XREF: xxx_HandleMurphy+155Bj
		inc	byte_40C4C
		;call	sub_3EC91
		ret


loc_3DFCB:				; CODE XREF: xxx_HandleMurphy+1038j
		cmp	byte ptr _LevelMap[esi + 2], 1Fh
		jz	short loc_3DFD8
		mov	word ptr _LevelMap[esi + 2], 0

loc_3DFD8:				; CODE XREF: xxx_HandleMurphy+1570j
		inc	byte_40C4C
		;call	sub_3EC91
		ret


loc_3DFE0:				; CODE XREF: xxx_HandleMurphy+1040j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3DFED
		mov	word ptr _LevelMap[esi], 0

loc_3DFED:				; CODE XREF: xxx_HandleMurphy+1585j
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		mov	word ptr _LevelMap[esi - (4 * MAP_WIDTH)], 12h
		sub	esi, 78h	; 'x'
		ret


loc_3DFFD:				; CODE XREF: xxx_HandleMurphy+1048j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3E00A
		mov	word ptr _LevelMap[esi], 0

loc_3E00A:				; CODE XREF: xxx_HandleMurphy+15A2j
		mov	word ptr _LevelMap[esi - 2], 3
		mov	word ptr _LevelMap[esi - 4], 12h
		sub	esi, 2
		ret


loc_3E01A:				; CODE XREF: xxx_HandleMurphy+1050j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3E027
		mov	word ptr _LevelMap[esi], 0

loc_3E027:				; CODE XREF: xxx_HandleMurphy+15BFj
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		mov	word ptr _LevelMap[esi + (4 * MAP_WIDTH)], 12h
		add	esi, 78h	; 'x'
		ret


loc_3E037:				; CODE XREF: xxx_HandleMurphy+1058j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3E044
		mov	word ptr _LevelMap[esi], 0

loc_3E044:				; CODE XREF: xxx_HandleMurphy+15DCj
		mov	word ptr _LevelMap[esi + 2], 3
		mov	word ptr _LevelMap[esi + 4], 12h
		add	esi, 2
		ret


loc_3E054:				; CODE XREF: xxx_HandleMurphy+1060j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3E061
		mov	word ptr _LevelMap[esi], 0

loc_3E061:				; CODE XREF: xxx_HandleMurphy+15F9j
		mov	word ptr _LevelMap[esi - 2], 3
		mov	word ptr _LevelMap[esi - 4], 8
		sub	esi, 2
		ret


loc_3E071:				; CODE XREF: xxx_HandleMurphy+1068j
		cmp	byte ptr _LevelMap[esi], 1Fh
		jz	short loc_3E07E
		mov	word ptr _LevelMap[esi], 0

loc_3E07E:				; CODE XREF: xxx_HandleMurphy+1616j
		mov	word ptr _LevelMap[esi + 2], 3
		mov	word ptr _LevelMap[esi + 4], 8
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH + 4], 0
		jnz	short loc_3E09B
		mov	byte ptr _LevelMap[esi + 5], 20h ; ' '
		mov	byte ptr _LevelMap[esi + 2 * MAP_WIDTH + 5], 8

loc_3E09B:				; CODE XREF: xxx_HandleMurphy+162Fj
		add	esi, 2
		ret


loc_3E09F:				; CODE XREF: xxx_HandleMurphy+1070j
		mov	word ptr _LevelMap[esi], 3
		mov	byte_403CB, 2
		dec	byte_40C4C
		;call	sub_3EC91
		;call	SND_mmm
		ret
; END OF FUNCTION CHUNK	FOR xxx_HandleMurphy

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3E0D7	proc near		; CODE XREF: xxx_HandleMurphy+291p
					; xxx_HandleMurphy+30Bp ...
		cmp	ax, 0FFFFh
		jz	short loc_3E14E
		cmp	ax, 0AAAAh
		jz	short loc_3E14E
		cmp	ah, 0
		jz	short loc_3E14E
		cmp	al, 1
		jz	short loc_3E107
		cmp	al, 1Fh
		jz	short loc_3E150
		cmp	al, 8
		jz	short loc_3E14E
		cmp	al, 9
		jz	short loc_3E14E
		cmp	al, 0Ah
		jz	short loc_3E14E
		cmp	al, 0Bh
		jz	short loc_3E14E
		cmp	al, 0Ch
		jz	short loc_3E14E
		call	sub_3956F
		stc
		ret


loc_3E107:				; CODE XREF: sub_3E0D7+11j
		cmp	bl, 2
		jz	short loc_3E116
		cmp	bl, 4
		jz	short loc_3E132
		call	sub_3956F
		stc
		ret


loc_3E116:				; CODE XREF: sub_3E0D7+33j
		and	ah, 0F0h
		cmp	ah, 20h	; ' '
		jz	short loc_3E130
		cmp	ah, 40h	; '@'
		jz	short loc_3E130
		cmp	ah, 50h	; 'P'
		jz	short loc_3E130
		cmp	ah, 70h	; 'p'
		jz	short loc_3E130
		call	sub_3956F

loc_3E130:				; CODE XREF: sub_3E0D7+45j
					; sub_3E0D7+4Aj ...
		stc
		ret


loc_3E132:				; CODE XREF: sub_3E0D7+38j
		and	ah, 0F0h
		cmp	ah, 30h	; '0'
		jz	short loc_3E14C
		cmp	ah, 40h	; '@'
		jz	short loc_3E14C
		cmp	ah, 60h	; '`'
		jz	short loc_3E14C
		cmp	ah, 70h	; 'p'
		jz	short loc_3E14C
		call	sub_3956F

loc_3E14C:				; CODE XREF: sub_3E0D7+61j
					; sub_3E0D7+66j ...
		stc
		ret


loc_3E14E:				; CODE XREF: sub_3E0D7+3j sub_3E0D7+8j ...
		stc
		ret


loc_3E150:				; CODE XREF: sub_3E0D7+15j
		test	ah, 80h
		jnz	short loc_3E15A
		cmp	ah, 4
		jge	short loc_3E15F

loc_3E15A:				; CODE XREF: sub_3E0D7+7Cj
		call	sub_3956F
		stc
		ret


loc_3E15F:				; CODE XREF: sub_3E0D7+81j
		mov	word ptr _LevelMap[esi], 0
		clc
		ret
sub_3E0D7	endp


		cmp	bl, 2
		jz	short loc_3E176
		cmp	bl, 4
		jz	short loc_3E183
		call	sub_3956F
		stc
		ret


loc_3E176:				; CODE XREF: CODE_SEG:754Aj
		and	ah, 0F0h
		cmp	ah, 20h	; ' '
		jz	short loc_3E181
		call	sub_3956F

loc_3E181:				; CODE XREF: CODE_SEG:755Cj
		stc
		ret


loc_3E183:				; CODE XREF: CODE_SEG:754Fj
		and	ah, 0F0h
		cmp	ah, 20h	; ' '
		jz	short loc_3E18E
		call	sub_3956F

loc_3E18E:				; CODE XREF: CODE_SEG:7569j
		stc
		ret

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3E190	proc near		; CODE XREF: xxx_HandleMurphy+142Bp
					; xxx_HandleMurphy+1458p ...
		mov	cl, byte_40327
		xor	ch, ch
		cmp	cx, 0
		jz	short locret_3E1AB
		mov	di, 0D28h

loc_3E19E:				; CODE XREF: sub_3E190+19j
		mov	bx, 0;[di]
		xchg	bl, bh
		cmp	bx, si
		jz	short loc_3E1AC
		add	di, 6
		loop	loc_3E19E

locret_3E1AB:				; CODE XREF: sub_3E190+9j
		ret


loc_3E1AC:				; CODE XREF: sub_3E190+14j
		mov	al, 0;[di+2]
		mov	byte_4030C, al
		mov	al, 0;[di+3]
		mov	byte_40325, al
		mov	al, 0;[di+4]
		mov	byte_403C7, al
		mov	ax, word_4039C
		xor	ax, RandomSeed
		mov	word_4039C, ax
		ret
sub_3E190	endp



; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


handleSnikSnak	proc near		; DATA XREF: DATA_SEG:162Co
		cmp	byte_403C7, 1
		jz	short locret_3E1F1
		cmp	byte ptr _LevelMap[esi], MAP_SNIK_SNAK
		jnz	short locret_3E1F1
		mov	bl, byte ptr _LevelMap[esi + 1]
		movzx ebx, bl
		shl	ebx, 2
		mov	eax, MapHandlers_1[ebx]
		cmp	eax, 0FFFFh
		je	short locret_3E1F1
		call	eax

locret_3E1F1:				; CODE XREF: handleSnikSnak+5j
					; handleSnikSnak+Cj ...
		ret
handleSnikSnak	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3E1F2	proc near		
		mov	ax, word_40C4D
		and	ax, 3
		jz	short loc_3E200
		cmp	ax, 3
		jz	short loc_3E242
		ret


loc_3E200:				; CODE XREF: sub_3E1F2+6j
		;push	ds
		;push	si
		;push	bx
		;mov	di, [si+60CBh]
		;mov	si, [bx+1388h]
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 10h

;loc_3E212:				; CODE XREF: sub_3E1F2+28j
		;movsb
		;movsb
		;add	si, 78h	; 'x'
		;add	di, 78h	; 'x'
		;loop	loc_3E212
		;pop	bx
		;pop	si
		;pop	ds
		shr	bx, 1
		inc	bx
		and	bx, 7
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_3E22A:				; CODE XREF: sub_3E1F2+79j
					; sub_3E1F2+9Cj ...
		cmp	ah, 1Bh
		jz	short locret_3E241
		cmp	ah, 19h
		jz	short locret_3E241
		cmp	ah, 18h
		jz	short locret_3E241
		cmp	ah, 1Ah
		jz	short locret_3E241
		call	sub_3956F

locret_3E241:				; CODE XREF: sub_3E1F2+3Bj
					; sub_3E1F2+40j ...
		ret


loc_3E242:				; CODE XREF: sub_3E1F2+Bj
		mov	bl, byte ptr _LevelMap[esi + 1]
		cmp	bl, 0
		jz	short loc_3E25B
		cmp	bl, 2
		jz	short loc_3E27E
		cmp	bl, 4
		jz	short loc_3E2A1
		cmp	bl, 6
		jz	loc_3E2C7
		ret


loc_3E25B:				; CODE XREF: sub_3E1F2+57j
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E26E
		mov	ax, word ptr _LevelMap[esi - 2 * MAP_WIDTH]
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jz	short loc_3E22A
		ret


loc_3E26E:				; CODE XREF: sub_3E1F2+6Ej
		mov	word ptr _LevelMap[esi], 1BBh
		sub	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 1011h
		ret


loc_3E27E:				; CODE XREF: sub_3E1F2+5Cj
		cmp	word ptr _LevelMap[esi - 2], 0
		jz	short loc_3E291
		mov	ax, word ptr _LevelMap[esi - 2]
		cmp	byte ptr _LevelMap[esi - 2], 3
		jz	loc_3E22A
		ret


loc_3E291:				; CODE XREF: sub_3E1F2+91j
		mov	word ptr _LevelMap[esi], 2BBh
		sub	esi, 2
		mov	word ptr _LevelMap[esi], 1811h
		ret


loc_3E2A1:				; CODE XREF: sub_3E1F2+61j
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E2B7
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short locret_3E2B6
		jmp	loc_3E22A


locret_3E2B6:				; CODE XREF: sub_3E1F2+BFj
		ret


loc_3E2B7:				; CODE XREF: sub_3E1F2+B4j
		mov	word ptr _LevelMap[esi], 3BBh
		add	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 2011h
		ret


loc_3E2C7:				; CODE XREF: sub_3E1F2+66j
		cmp	word ptr _LevelMap[esi + 2], 0
		jz	short loc_3E2DD
		mov	ax, word ptr _LevelMap[esi + 2]
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short locret_3E2DC
		jmp	loc_3E22A


locret_3E2DC:				; CODE XREF: sub_3E1F2+E5j
		ret


loc_3E2DD:				; CODE XREF: sub_3E1F2+DAj
		mov	word ptr _LevelMap[esi], 4BBh
		add	esi, 2
		mov	word ptr _LevelMap[esi], 2811h
		ret
sub_3E1F2	endp



loc_3E2ED:				; DATA XREF: DATA_SEG:155Ao
					; DATA_SEG:155Co ...
		mov	ax, word_40C4D
		and	ax, 3
		jz	short loc_3E2FB
		cmp	ax, 3
		jz	short loc_3E33F
		ret


loc_3E2FB:				; CODE XREF: CODE_SEG:76D3j
		;push	ds
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, [bx+1388h]
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 10h

;loc_3E30C:				; CODE XREF: CODE_SEG:76F4j
		;movsb
		;movsb
		;add	si, 78h	; 'x'
		;add	di, 78h	; 'x'
		;loop	loc_3E30C
		;pop	si
		;pop	ds
		shr	bx, 1
		inc	bl
		and	bl, 7
		or	bl, 8
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_3E327:				; CODE XREF: CODE_SEG:7748j
					; CODE_SEG:776Bj ...
		cmp	ah, 1Bh
		jz	short locret_3E33E
		cmp	ah, 19h
		jz	short locret_3E33E
		cmp	ah, 18h
		jz	short locret_3E33E
		cmp	ah, 1Ah
		jz	short locret_3E33E
		call	sub_3956F

locret_3E33E:				; CODE XREF: CODE_SEG:770Aj
					; CODE_SEG:770Fj ...
		ret


loc_3E33F:				; CODE XREF: CODE_SEG:76D8j
		mov	bl, byte ptr _LevelMap[esi + 1]
		cmp	bl, 8
		jz	short loc_3E358
		cmp	bl, 0Ah
		jz	loc_3E3C4
		cmp	bl, 0Ch
		jz	short loc_3E39E
		cmp	bl, 0Eh
		jz	short loc_3E37B
		ret


loc_3E358:				; CODE XREF: CODE_SEG:7726j
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E36B
		mov	ax, word ptr _LevelMap[esi - 2 * MAP_WIDTH]
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jz	short loc_3E327
		ret


loc_3E36B:				; CODE XREF: CODE_SEG:773Dj
		mov	word ptr _LevelMap[esi], 1BBh
		sub	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 1011h
		ret


loc_3E37B:				; CODE XREF: CODE_SEG:7735j
		cmp	word ptr _LevelMap[esi - 2], 0
		jz	short loc_3E38E
		mov	ax, word ptr _LevelMap[esi - 2]
		cmp	byte ptr _LevelMap[esi - 2], 3
		jz	loc_3E327
		ret


loc_3E38E:				; CODE XREF: CODE_SEG:7760j
		mov	word ptr _LevelMap[esi], 2BBh
		sub	esi, 2
		mov	word ptr _LevelMap[esi], 1811h
		ret


loc_3E39E:				; CODE XREF: CODE_SEG:7730j
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E3B4
		mov	ax, word ptr _LevelMap[esi + 2 * MAP_WIDTH]
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short locret_3E3B3
		jmp	loc_3E327


locret_3E3B3:				; CODE XREF: CODE_SEG:778Ej
		ret


loc_3E3B4:				; CODE XREF: CODE_SEG:7783j
		mov	word ptr _LevelMap[esi], 3BBh
		add	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 2011h
		ret


loc_3E3C4:				; CODE XREF: CODE_SEG:772Bj
		cmp	word ptr _LevelMap[esi + 2], 0
		jz	short loc_3E3DA
		mov	ax, word ptr _LevelMap[esi + 2]
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short locret_3E3D9
		jmp	loc_3E327


locret_3E3D9:				; CODE XREF: CODE_SEG:77B4j
		ret


loc_3E3DA:				; CODE XREF: CODE_SEG:77A9j
		mov	word ptr _LevelMap[esi], 4BBh
		add	esi, 2
		mov	word ptr _LevelMap[esi], 2811h
		ret


loc_3E3EA:				; DATA XREF: DATA_SEG:156Ao
					; DATA_SEG:156Co ...
		;push	si
		;mov	di, [si+6143h]
		;mov	si, [bx+1388h]
		;sub	bx, 1Eh
		;sub	di, [bx+6C0Bh]
		;sub	di, [bx+6C0Bh]
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 12h

;loc_3E406:				; CODE XREF: CODE_SEG:77EEj
		;movsb
		;movsb
		;add	di, 78h	; 'x'
		;add	si, 78h	; 'x'
		;loop	loc_3E406
		;pop	ds
		;pop	si
		shr	bx, 1
		cmp	bl, 7
		jnz	short loc_3E426
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 1Fh
		jz	short loc_3E426
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0

loc_3E426:				; CODE XREF: CODE_SEG:77F7j
					; CODE_SEG:77FEj
		cmp	bl, 8
		jge	short loc_3E433
		add	bl, 10h
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_3E433:				; CODE XREF: CODE_SEG:7809j
		mov	word ptr _LevelMap[esi], 11h
		cmp	word ptr _LevelMap[esi - 2], 0
		jnz	short loc_3E446
		mov	byte ptr _LevelMap[esi + 1], 1
		ret


loc_3E446:				; CODE XREF: CODE_SEG:781Ej
		cmp	byte ptr _LevelMap[esi - 2], 3
		jnz	short loc_3E453
		mov	byte ptr _LevelMap[esi + 1], 1
		ret


loc_3E453:				; CODE XREF: CODE_SEG:782Bj
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E46A
		mov	word ptr _LevelMap[esi], 1BBh
		sub	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 1011h
		ret


loc_3E46A:				; CODE XREF: CODE_SEG:7838j
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E475
		call	sub_3956F
		ret


loc_3E475:				; CODE XREF: CODE_SEG:784Fj
		cmp	word ptr _LevelMap[esi + 2], 0
		jnz	short loc_3E482
		mov	byte ptr _LevelMap[esi + 1], 9
		ret


loc_3E482:				; CODE XREF: CODE_SEG:785Aj
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short loc_3E48F
		mov	byte ptr _LevelMap[esi + 1], 9
		ret


loc_3E48F:				; CODE XREF: CODE_SEG:7867j
		mov	byte ptr _LevelMap[esi + 1], 1
		ret


loc_3E495:				; DATA XREF: DATA_SEG:157Ao
					; DATA_SEG:157Co ...
		;push	si
		;mov	di, [si+60CBh]
		;mov	si, [bx+1388h]
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 10h

;loc_3E4A6:				; CODE XREF: CODE_SEG:7890j
		;movsb
		;movsb
		;movsb
		;movsb
		;add	di, 76h	; 'v'
		;add	si, 76h	; 'v'
		;loop	loc_3E4A6
		;pop	ds
		;pop	si
		shr	bx, 1
		and	bx, 7
		inc	bx
		cmp	bl, 7
		jnz	short loc_3E4CC
		cmp	byte ptr _LevelMap[esi + 2], 1Fh
		jz	short loc_3E4CC
		mov	word ptr _LevelMap[esi + 2], 0

loc_3E4CC:				; CODE XREF: CODE_SEG:789Dj
					; CODE_SEG:78A4j
		cmp	bl, 8
		jge	short loc_3E4D9
		add	bl, 18h
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_3E4D9:				; CODE XREF: CODE_SEG:78AFj
		mov	word ptr _LevelMap[esi], 11h
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E4EC
		mov	byte ptr _LevelMap[esi + 1], 3
		ret


loc_3E4EC:				; CODE XREF: CODE_SEG:78C4j
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E4F9
		mov	byte ptr _LevelMap[esi + 1], 3
		ret


loc_3E4F9:				; CODE XREF: CODE_SEG:78D1j
		cmp	word ptr _LevelMap[esi - 2], 0
		jnz	short loc_3E510
		mov	word ptr _LevelMap[esi], 2BBh
		sub	esi, 2
		mov	word ptr _LevelMap[esi], 1811h
		ret


loc_3E510:				; CODE XREF: CODE_SEG:78DEj
		cmp	byte ptr _LevelMap[esi - 2], 3
		jnz	short loc_3E51B
		call	sub_3956F
		ret


loc_3E51B:				; CODE XREF: CODE_SEG:78F5j
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E528
		mov	byte ptr _LevelMap[esi + 1], 0Fh
		ret


loc_3E528:				; CODE XREF: CODE_SEG:7900j
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E535
		mov	byte ptr _LevelMap[esi + 1], 0Fh
		ret


loc_3E535:				; CODE XREF: CODE_SEG:790Dj
		mov	byte ptr _LevelMap[esi + 1], 3
		ret


loc_3E53B:				
		;push	si
		;mov	di, [si+6053h]
		;mov	si, [bx+1388h]
		;sub	bx, 40h	; '@'
		;add	di, [bx+6C0Bh]
		;add	di, [bx+6C0Bh]
		;push	ds
		;mov	ax, es
		;mov	ds, ax
		;mov	cx, 12h

;loc_3E557:				; CODE XREF: CODE_SEG:793Fj
		;movsb
		;movsb
		;add	di, 78h	; 'x'
		;add	si, 78h	; 'x'
		;loop	loc_3E557
		;pop	ds
		;pop	si
		shr	bx, 1
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E579
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 1Fh
		jz	short loc_3E579
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0

loc_3E579:				; CODE XREF: CODE_SEG:794Aj
					; CODE_SEG:7951j
		cmp	bl, 8
		jge	short loc_3E586
		add	bl, 20h	; ' '
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_3E586:				; CODE XREF: CODE_SEG:795Cj
		mov	word ptr _LevelMap[esi], 11h
		cmp	word ptr _LevelMap[esi + 2], 0
		jnz	short loc_3E599
		mov	byte ptr _LevelMap[esi + 1], 5
		ret


loc_3E599:				; CODE XREF: CODE_SEG:7971j
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short loc_3E5A6
		mov	byte ptr _LevelMap[esi + 1], 5
		ret


loc_3E5A6:				; CODE XREF: CODE_SEG:797Ej
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E5BD
		mov	word ptr _LevelMap[esi], 3BBh
		add	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 2011h
		ret


loc_3E5BD:				; CODE XREF: CODE_SEG:798Bj
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E5C8
		call	sub_3956F
		ret


loc_3E5C8:				; CODE XREF: CODE_SEG:79A2j
		cmp	word ptr _LevelMap[esi - 2], 0
		jnz	short loc_3E5D5
		mov	byte ptr _LevelMap[esi + 1], 0Dh
		ret


loc_3E5D5:				; CODE XREF: CODE_SEG:79ADj
		cmp	byte ptr _LevelMap[esi - 2], 3
		jnz	short loc_3E5E2
		mov	byte ptr _LevelMap[esi + 1], 0Dh
		ret


loc_3E5E2:				; CODE XREF: CODE_SEG:79BAj
		mov	byte ptr _LevelMap[esi + 1], 5
		ret


loc_3E5E8:	
		shr	bx, 1
		and	bl, 7
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E620
		cmp	byte ptr _LevelMap[esi - 2], 1Fh
		jz	short loc_3E620
		mov	word ptr _LevelMap[esi - 2], 0

loc_3E620:
		cmp	bl, 8
		jge	short loc_3E62D
		add	bl, 28h
		mov	byte ptr _LevelMap[esi + 1], bl
		ret

loc_3E62D:
		mov	word ptr _LevelMap[esi], 11h
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E640
		mov	byte ptr _LevelMap[esi + 1], 7
		ret

loc_3E640:
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E64D
		mov	byte ptr _LevelMap[esi + 1], 7
		ret

loc_3E64D:
		cmp	word ptr _LevelMap[esi + 2], 0
		jnz	short loc_3E664
		mov	word ptr _LevelMap[esi], 4BBh
		add	esi, 2
		mov	word ptr _LevelMap[esi], 2811h
		ret

loc_3E664:
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short loc_3E66F
		call	sub_3956F
		ret

loc_3E66F:
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E67C
		mov	byte ptr _LevelMap[esi + 1], 0Bh
		ret

loc_3E67C:
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E689
		mov	byte ptr _LevelMap[esi + 1], 0Bh
		ret

loc_3E689:
		mov	byte ptr _LevelMap[esi + 1], 7
		ret

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

handleElectron	proc near
		cmp	byte_403C7, 1
		jz	short locret_3E6B0
		cmp	byte ptr _LevelMap[esi], 18h
		jnz	short locret_3E6B0
		mov	bl, byte ptr _LevelMap[esi + 1]
		movzx ebx, bl
		shl	ebx, 2
		mov	eax, dword ptr MapHandlers_5[ebx]
		cmp	eax, 0FFFFh
		jz	short locret_3E6B0
		call	eax

locret_3E6B0:
		ret
handleElectron	endp


loc_3E6B1:
		mov	ax, word_40C4D
		and	ax, 3
		jz	short loc_3E6BF
		cmp	ax, 3
		jz	short loc_3E6ED
		ret

loc_3E6BF:
		shr	bx, 1
		inc	bx
		and	bx, 7
		mov	byte ptr _LevelMap[esi + 1], bl
		ret

loc_3E6E9:
		call	sub_3956F
		ret

loc_3E6ED:
		mov	bl, byte ptr _LevelMap[esi + 1]
		cmp	bl, 0
		jz	short loc_3E706
		cmp	bl, 2
		jz	short loc_3E725
		cmp	bl, 4
		jz	short loc_3E744
		cmp	bl, 6
		jz	loc_3E763
		ret

loc_3E706:
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E715
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jz	short loc_3E6E9
		ret

loc_3E715:
		mov	word ptr _LevelMap[esi], 1BBh
		sub	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 1018h
		ret

loc_3E725:
		cmp	word ptr _LevelMap[esi - 2], 0
		jz	short loc_3E734
		cmp	byte ptr _LevelMap[esi - 2], 3
		jz	short loc_3E6E9
		ret

loc_3E734:
		mov	word ptr _LevelMap[esi], 2BBh
		sub	esi, 2
		mov	word ptr _LevelMap[esi], 1818h
		ret

loc_3E744:
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E753
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jz	loc_3E6E9
		ret

loc_3E753:
		mov	word ptr _LevelMap[esi], 3BBh
		add	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 2018h
		ret

loc_3E763:
		cmp	word ptr _LevelMap[esi + 2], 0
		jz	short loc_3E775
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short locret_3E774
		jmp	loc_3E6E9

locret_3E774:
		ret

loc_3E775:
		mov	word ptr _LevelMap[esi], 4BBh
		add	esi, 2
		mov	word ptr _LevelMap[esi], 2818h
		ret

loc_3E785:
		mov	ax, word_40C4D
		and	ax, 3
		jz	short loc_3E793
		cmp	ax, 3
		jz	short loc_3E7C3
		ret

loc_3E793:
		shr	bx, 1
		inc	bl
		and	bl, 7
		or	bl, 8
		mov	byte ptr _LevelMap[esi + 1], bl
		ret

loc_3E7BF:
		call	sub_3956F
		ret

loc_3E7C3:
		mov	bl, byte ptr _LevelMap[esi + 1]
		cmp	bl, 8
		jz	short loc_3E7DC
		cmp	bl, 0Ah
		jz	loc_3E839
		cmp	bl, 0Ch
		jz	short loc_3E81A
		cmp	bl, 0Eh
		jz	short loc_3E7FB
		ret

loc_3E7DC:
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E7EB
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jz	short loc_3E7BF
		ret

loc_3E7EB:
		mov	word ptr _LevelMap[esi], 1BBh
		sub	esi, 78h
		mov	word ptr _LevelMap[esi], 1018h
		ret

loc_3E7FB:
		cmp	word ptr _LevelMap[esi - 2], 0
		jz	short loc_3E80A
		cmp	byte ptr _LevelMap[esi - 2], 3
		jz	short loc_3E7BF
		ret

loc_3E80A:
		mov	word ptr _LevelMap[esi], 2BBh
		sub	esi, 2
		mov	word ptr _LevelMap[esi], 1818h
		ret

loc_3E81A:
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E829
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jz	loc_3E7BF
		ret

loc_3E829:
		mov	word ptr _LevelMap[esi], 3BBh
		add	esi, 78h
		mov	word ptr _LevelMap[esi], 2018h
		ret

loc_3E839:
		cmp	word ptr _LevelMap[esi + 2], 0
		jz	short loc_3E84B
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short locret_3E84A
		jmp	loc_3E7BF

locret_3E84A:
		ret

loc_3E84B:
		mov	word ptr _LevelMap[esi], 4BBh
		add	esi, 2
		mov	word ptr _LevelMap[esi], 2818h
		ret

loc_3E85B:			
		shr	bx, 1
		cmp	bl, 7
		jnz	short loc_3E897
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 1Fh
		jz	short loc_3E897
		mov	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0

loc_3E897:
		cmp	bl, 8
		jge	short loc_3E8A4
		add	bl, 10h
		mov	byte ptr _LevelMap[esi + 1], bl
		ret

loc_3E8A4:
		mov	word ptr _LevelMap[esi], 18h
		cmp	word ptr _LevelMap[esi - 2], 0
		jnz	short loc_3E8B7
		mov	byte ptr _LevelMap[esi + 1], 1
		ret

loc_3E8B7:
		cmp	byte ptr _LevelMap[esi - 2], 3
		jnz	short loc_3E8C4
		mov	byte ptr _LevelMap[esi + 1], 1
		ret

loc_3E8C4:
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E8DB
		mov	word ptr _LevelMap[esi], 1BBh
		sub	esi, 78h
		mov	word ptr _LevelMap[esi], 1018h
		ret

loc_3E8DB:
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E8E6
		call	sub_3956F
		ret

loc_3E8E6:
		cmp	word ptr _LevelMap[esi + 2], 0
		jnz	short loc_3E8F3
		mov	byte ptr _LevelMap[esi + 1], 9
		ret

loc_3E8F3:
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short loc_3E900
		mov	byte ptr _LevelMap[esi + 1], 9
		ret


loc_3E900:
		mov	byte ptr _LevelMap[esi + 1], 1
		ret

loc_3E906:
		shr	bx, 1
		and	bx, 7
		inc	bx
		cmp	bl, 7
		jnz	short loc_3E93D
		cmp	byte ptr _LevelMap[esi + 2], 1Fh
		jz	short loc_3E93D
		mov	word ptr _LevelMap[esi + 2], 0

loc_3E93D:
		cmp	bl, 8
		jge	short loc_3E94A
		add	bl, 18h
		mov	byte ptr _LevelMap[esi + 1], bl
		ret


loc_3E94A:
		mov	word ptr _LevelMap[esi], 18h
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E95D
		mov	byte ptr _LevelMap[esi + 1], 3
		ret


loc_3E95D:
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E96A
		mov	byte ptr _LevelMap[esi + 1], 3
		ret


loc_3E96A:
		cmp	word ptr _LevelMap[esi - 2], 0
		jnz	short loc_3E981
		mov	word ptr _LevelMap[esi], 2BBh
		sub	esi, 2
		mov	word ptr _LevelMap[esi], 1818h
		ret


loc_3E981:
		cmp	byte ptr _LevelMap[esi - 2], 3
		jnz	short loc_3E98C
		call	sub_3956F
		ret


loc_3E98C:
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E999
		mov	byte ptr _LevelMap[esi + 1], 0Fh
		ret


loc_3E999:
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E9A6
		mov	byte ptr _LevelMap[esi + 1], 0Fh
		ret


loc_3E9A6:
		mov	byte ptr _LevelMap[esi + 1], 3
		ret


loc_3E9AC:
		shr	bx, 1
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E9EA
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 1Fh
		jz	short loc_3E9EA
		mov	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0

loc_3E9EA:
		cmp	bl, 8
		jge	short loc_3E9F7
		add	bl, 20h	; ' '
		mov	byte ptr _LevelMap[esi + 1], bl
		ret

loc_3E9F7:
		mov	word ptr _LevelMap[esi], 18h
		cmp	word ptr _LevelMap[esi + 2], 0
		jnz	short loc_3EA0A
		mov	byte ptr _LevelMap[esi + 1], 5
		ret

loc_3EA0A:
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short loc_3EA17
		mov	byte ptr _LevelMap[esi + 1], 5
		ret

loc_3EA17:
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3EA2E
		mov	word ptr _LevelMap[esi], 3BBh
		add	esi, 78h	; 'x'
		mov	word ptr _LevelMap[esi], 2018h
		ret

loc_3EA2E:
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3EA39
		call	sub_3956F
		ret

loc_3EA39:
		cmp	word ptr _LevelMap[esi - 2], 0
		jnz	short loc_3EA46
		mov	byte ptr _LevelMap[esi + 1], 0Dh
		ret

loc_3EA46:
		cmp	byte ptr _LevelMap[esi - 2], 3
		jnz	short loc_3EA53
		mov	byte ptr _LevelMap[esi + 1], 0Dh
		ret

loc_3EA53:
		mov	byte ptr _LevelMap[esi + 1], 5
		ret

loc_3EA59:
		shr	bx, 1
		and	bl, 7
		inc	bl
		cmp	bl, 7
		jnz	short loc_3EA91
		cmp	byte ptr _LevelMap[esi - 2], 1Fh
		jz	short loc_3EA91
		mov	word ptr _LevelMap[esi - 2], 0

loc_3EA91:
		cmp	bl, 8
		jge	short loc_3EA9E
		add	bl, 28h
		mov	byte ptr _LevelMap[esi + 1], bl
		ret

loc_3EA9E:
		mov	word ptr _LevelMap[esi], 18h
		cmp	word ptr _LevelMap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3EAB1
		mov	byte ptr _LevelMap[esi + 1], 7
		ret

loc_3EAB1:
		cmp	byte ptr _LevelMap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3EABE
		mov	byte ptr _LevelMap[esi + 1], 7
		ret

loc_3EABE:
		cmp	word ptr _LevelMap[esi + 2], 0
		jnz	short loc_3EAD5
		mov	word ptr _LevelMap[esi], 4BBh
		add	esi, 2
		mov	word ptr _LevelMap[esi], 2818h
		ret

loc_3EAD5:
		cmp	byte ptr _LevelMap[esi + 2], 3
		jnz	short loc_3EAE0
		call	sub_3956F
		ret

loc_3EAE0:
		cmp	word ptr _LevelMap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3EAED
		mov	byte ptr _LevelMap[esi + 1], 0Bh
		ret

loc_3EAED:
		cmp	byte ptr _LevelMap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3EAFA
		mov	byte ptr _LevelMap[esi + 1], 0Bh
		ret

loc_3EAFA:
		mov	byte ptr _LevelMap[esi + 1], 7
		ret

.data

byte_4030C	db 0
byte_40325	db 0			
byte_40326	db 0	
byte_40327	db 0	

word_4039C	dw 0
byte_4039E	db 0
byte_4039F	db 0	
byte_403A0	db 0	
byte_403A1	db 0	
byte_403A2	db 0	
byte_403A3	db 0
byte_403A4	db 0
byte_403A5	db 0
byte_403A6	db 0
byte_403A7	db 0
byte_403A8	db 0
byte_403A9	db 0
byte_403AA	db 0
byte_403AB	db 0
PlayerPosition2_MapX dw	0
PlayerPosition2_MapY dw	0
byte_403B0	db 0
byte_403B1	db 0
byte_403B2	db 0
PlayerPosition_MapX dw 0
PlayerPosition_MapY dw 0
word_403B7	dd 0
PlayerPosition_Ofs dd 0
word_403BB	dw 0
word_403BD	dw 0
word_403BF	dw 0
word_403C1	dw 0
byte_403C3	db 0
byte_403C7	db 0
byte_403C8	db 0
word_403C9	dw 0
byte_403CB	db 0
word_403CC	dd 0
IsDemoRunning	db 0
PlayerPosition_PixelsX dw 0
PlayerPosition_PixelsY dw 0


word_403DE	dw 0
word_403E0	dw 0
word_403E2	dw 0
word_403E4	dw 0
word_403E6	dw 0
word_403E8	dd 0
word_403EA	dw 0
word_403EC	dw 0

MapHandlers_1	dd offset sub_3E1F2	; DATA XREF: handleSnikSnak+16r
		dd offset sub_3E1F2
		dd offset sub_3E1F2
		dd offset sub_3E1F2
		dd offset sub_3E1F2
		dd offset sub_3E1F2
		dd offset sub_3E1F2
		dd offset sub_3E1F2
		dd offset loc_3E2ED
		dd offset loc_3E2ED
		dd offset loc_3E2ED
		dd offset loc_3E2ED
		dd offset loc_3E2ED
		dd offset loc_3E2ED
		dd offset loc_3E2ED
		dd offset loc_3E2ED
		dd offset loc_3E3EA
		dd offset loc_3E3EA
		dd offset loc_3E3EA
		dd offset loc_3E3EA
		dd offset loc_3E3EA
		dd offset loc_3E3EA
		dd offset loc_3E3EA
		dd offset loc_3E3EA
		dd offset loc_3E495
		dd offset loc_3E495
		dd offset loc_3E495
		dd offset loc_3E495
		dd offset loc_3E495
		dd offset loc_3E495
		dd offset loc_3E495
		dd offset loc_3E495
MapHandlers_2	dd offset loc_3E53B
		dd offset loc_3E53B
		dd offset loc_3E53B
		dd offset loc_3E53B
		dd offset loc_3E53B
		dd offset loc_3E53B
		dd offset loc_3E53B
		dd offset loc_3E53B
		dd offset loc_3E5E8
		dd offset loc_3E5E8
		dd offset loc_3E5E8
		dd offset loc_3E5E8
		dd offset loc_3E5E8
		dd offset loc_3E5E8
		dd offset loc_3E5E8
		dd offset loc_3E5E8
MapHandlers_5		dd offset loc_3E6B1
		dd offset loc_3E6B1
		dd offset loc_3E6B1
		dd offset loc_3E6B1
		dd offset loc_3E6B1
		dd offset loc_3E6B1
		dd offset loc_3E6B1
		dd offset loc_3E6B1
		dd offset loc_3E785
		dd offset loc_3E785
		dd offset loc_3E785
		dd offset loc_3E785
		dd offset loc_3E785
		dd offset loc_3E785
		dd offset loc_3E785
		dd offset loc_3E785
MapHandlers_3		dd offset loc_3E85B
		dd offset loc_3E85B
		dd offset loc_3E85B
		dd offset loc_3E85B
		dd offset loc_3E85B
		dd offset loc_3E85B
		dd offset loc_3E85B
		dd offset loc_3E85B
		dd offset loc_3E906
		dd offset loc_3E906
		dd offset loc_3E906
		dd offset loc_3E906
		dd offset loc_3E906
		dd offset loc_3E906
		dd offset loc_3E906
		dd offset loc_3E906
		dd offset loc_3E9AC
		dd offset loc_3E9AC
		dd offset loc_3E9AC
		dd offset loc_3E9AC
		dd offset loc_3E9AC
		dd offset loc_3E9AC
		dd offset loc_3E9AC
		dd offset loc_3E9AC
		dd offset loc_3EA59
		dd offset loc_3EA59
		dd offset loc_3EA59
		dd offset loc_3EA59
		dd offset loc_3EA59
		dd offset loc_3EA59
		dd offset loc_3EA59
		dd offset loc_3EA59
MapHandlers_4		dd 0
		dd offset handleZonk
		dd 0
		dd 0
		dd offset handleInfotron
		dd 0
		dd 0
		dd 0
		dd offset xxx_handleOrangeDisk
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd offset handleSnikSnak
		dd 0
		dd offset xxx_HandleTerminal
		dd 0
		dd 0
		dd 0
		dd 0
		dd offset handleElectron
		dd offset xxx_HandleBug
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd offset loc_39493
InfotronsLeftToCollect db 0
TotalInfotronsInTheLevel db 0
byte_40C4C	db 0
word_40C4D	dw 0F000h
word_40C4F	dw 0	
word_40C51	dw 0
word_40C53	dw 0
word_40C55	dw 0
byte_40C5A	db 0
byte_40C5B	db 0
word_40C60	dw 0
word_40C64	dw 0
TimeoutAfterWhichLeaveTheLevel	dw 0
byte_40C6C	db 0


word_40CF3	dw 0
word_40CF7	dw 1

_LevelMap	dw 1536 dup(0)
SomeLevelData	db 1536	dup(0)
RandomSeed	dw 0
word_4C47B	dw 0
word_4C47D	dw 0
word_4C47F	dw 2008 dup(0)
			db 0 

_JoystickButtons db 0

		end
