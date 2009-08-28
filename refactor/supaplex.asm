
public levelmap
public loadlevelmap
public findplayerpositiononlevelmap
public initrandomnumbergenerator
public initplayerstate
public sub_392df
public beginlevel
public runthelevel
public joystickbuttons

public _levelmap
public _loadlevelmap
public _findplayerpositiononlevelmap
public _initrandomnumbergenerator
public _initplayerstate
public _sub_392df
public _beginlevel
public _runthelevel
public _joystickbuttons


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

ifdef TASM
 locals @@
else
 .model flat
endif

.code

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

handleZonk	proc near

			cmp		byte ptr _levelmap[esi], MAP_ZONK
			je		short @@1
			ret
@@1:		mov		ax,	word ptr _levelmap[esi]
			cmp		ax, MAP_ZONK
			je		short @@2
			jmp		@@12
@@2:		cmp		byte ptr [byte_40325], 2
			jne		short @@3
			ret
@@3:		mov		ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
			cmp		ax, MAP_SPACE
			je		short @@6
			cmp		ax, MAP_ZONK
			je		short @@4
			cmp		ax, MAP_INFOTRON
			je		short @@4
			cmp		ax, MAP_RAM_CHIP_SQUARE
			je		short @@4
			ret
@@4:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0
			je		short @@7
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short @@7
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short @@7
@@5:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0
			je		short @@9
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short @@9
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short @@9
			ret
@@6:		mov		byte ptr _levelmap[esi + 1], 40h
			jmp		short @@12
@@7:		cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			je		short @@8
			jmp		short @@5
@@8:		mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			jmp		short @@12
@@9:		cmp		word ptr _levelmap[esi + 2], MAP_SPACE
			je		short @@11
			cmp		word ptr _levelmap[esi + 2], 9999h
			jne		short @@10
			cmp		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 1
			jz		short @@11
@@10:		ret
@@11:		mov		byte ptr _levelmap[esi + 1], 60h
			mov		word ptr _levelmap[esi + 2], 8888h
			jmp		short @@12
@@12:		mov		bl, byte ptr _levelmap[esi + 1]
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
@@15:		cmp		byte ptr [byte_40325], 2
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
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 16h
			jnz		short loc_37A4B
			mov		byte ptr _levelmap[esi + 1], bl
			sub		esi, 2 * MAP_WIDTH
			call	sub_3826F
			add		esi, 2 * MAP_WIDTH
			ret
loc_37A4B:	cmp		bl, MAP_ELECTRON
			jge		short loc_37A55
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37A55:	mov		byte ptr _levelmap[esi + 1], 0
			cmp		byte ptr [byte_40325], 2
			jne		short loc_37A62
			ret
loc_37A62:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			jne		short loc_37A6C
			jmp		loc_37AFD
loc_37A6C:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			jne		short loc_37A77
			jmp		loc_37AFD
loc_37A77:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			jne		short loc_37A81
			jmp		loc_37B34
loc_37A81:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SNIK_SNAK
			jne		short loc_37A8B
			jmp		loc_37B8E
loc_37A8B:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 200h + 0BBh
			jne		short loc_37A96
			jmp		loc_37B56
loc_37A96:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 400h + 0BBh
			jne		short loc_37AA1
			jmp		loc_37B72
loc_37AA1:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ELECTRON
			jne		short loc_37AAB
			jmp		loc_37B8E
loc_37AAB:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 8
			jne		short loc_37AB5
			jmp		loc_37B95
loc_37AB5:	;call	SND_nnn
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 1
			je		short loc_37ACE
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 4
			je		short loc_37ACE
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 5
			je		short loc_37ACE
			ret
loc_37ACE:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0
			je		short loc_37B0A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short loc_37B0A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short loc_37B0A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0
			je		short loc_37B20
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short loc_37B20
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short loc_37B20
			ret
loc_37AFD:	mov		word ptr _levelmap[esi], 7000h + MAP_ZONK
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_37B0A:	cmp		word ptr _levelmap[esi - 2], MAP_BASE
			je		short loc_37B14
			jmp		@@5
loc_37B14:	mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			ret
loc_37B20:	cmp		word ptr _levelmap[esi + 2], 0
			jz		short loc_37B28
			ret
loc_37B28:	mov		byte ptr _levelmap[esi + 1], 60h
			mov		word ptr _levelmap[esi + 2], 8888h
			ret
loc_37B34:	mov		bl, byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1]
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
loc_37B56:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_ELECTRON
			jne		short loc_37B63
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ELECTRON
loc_37B63:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_RED_LAMP
			je		short loc_37B70
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0
loc_37B70:	jmp		short loc_37B8E
loc_37B72:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_ELECTRON
			jne		short loc_37B7F
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ELECTRON
loc_37B7F:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_RED_LAMP
			je		short loc_37B8C
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0
loc_37B8C:	jmp		short loc_37B8E
loc_37B8E:	add		esi, 2 * MAP_WIDTH
			call	CreateExplosion
			ret
loc_37B95:	add		esi, 2 * MAP_WIDTH
			mov		ebx, esi
			shr		ebx, 1
			mov		byte ptr SomeLevelData[ebx], 6
			ret
locret_37BA2:
			ret
loc_37BA3:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 24h
			jnz		short loc_37BDB
			mov		word ptr _levelmap[esi + 2], 0AAAAh
loc_37BDB:	cmp		bl, 26h
			jnz		short loc_37BEE
			mov		byte ptr _levelmap[esi + 1], bl
			add		esi, 2
			call	sub_3826F
			sub		esi, 2
			ret
loc_37BEE:	cmp		bl, 28h
			jge		short loc_37BF8
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37BF8:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _levelmap[esi], 1000h + 1
			ret
loc_37C08:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 34h	; '4'
			jne		short loc_37C40
			mov		word ptr _levelmap[esi - 2], 0AAAAh
loc_37C40:	cmp		bl, 36h
			jnz		short loc_37C53
			mov		byte ptr _levelmap[esi + 1], bl
			sub		esi, 2
			call	sub_3826F
			add		esi, 2
			ret
loc_37C53:	cmp		bl, 38h
			jge		short loc_37C5D
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37C5D:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, MAP_WIDTH * 2
			mov		word ptr _levelmap[esi], 1001h
			ret
loc_37C6D:	inc		bl
			cmp		bl, 42h
			jge		short loc_37C79
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37C79:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
			jz		short loc_37C87
			dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37C87:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		byte ptr _levelmap[esi], 1
			mov		byte ptr _levelmap[esi + 1], 10h
			ret
loc_37C9B:	xor		bh, bh
			and		bl, 0Fh
			shl		bl, 1
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 52h
			jge		short loc_37CD2
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37CD2:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0
			jne		short loc_37D02
			cmp		word ptr _levelmap[esi - 2], 0
			je		short loc_37CE8
			cmp		word ptr _levelmap[esi - 2], 8888h
			jne		short loc_37D02
loc_37CE8:	mov		word ptr _levelmap[esi], 0FFFFh
			sub		esi, 2
			mov		byte ptr _levelmap[esi], 1
			mov		byte ptr _levelmap[esi + 1], 22h
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0FFFFh
			ret
loc_37D02:	dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37D09:	xor		bh, bh
			and		bl, 7
			shl		bl, 1
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 62h
			jge		short loc_37D40
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37D40:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0
			jne		short loc_37D70
			cmp		word ptr _levelmap[esi + 2], 0
			je		short loc_37D56
			cmp		word ptr _levelmap[esi + 2], 8888h
			jne		short loc_37D70
loc_37D56:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2
			mov		byte ptr _levelmap[esi], 1
			mov		byte ptr _levelmap[esi + 1], 32h
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0FFFFh
			ret
loc_37D70:	dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37D77:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
			je		short loc_37D87
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			je		short loc_37D87
			ret
loc_37D87:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _levelmap[esi], 1001h
			jmp		@@21

handleZonk	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


handleInfotron	proc near		; DATA XREF: DATA_SEG:1612o

			cmp		byte ptr _levelmap[esi], 4
			jz		short @@1
			ret
@@1:		mov		ax, word ptr _levelmap[esi]
			cmp		ax, 4
			jz		short loc_37DAD
			jmp		loc_37E27
loc_37DAD:	mov		ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
			cmp		ax, 0
			je		short loc_37DF5
			cmp		ax, 1
			je		short loc_37DC6
			cmp		ax, 4
			je		short loc_37DC6
			cmp		ax, 5
			je		short loc_37DC6
			ret
loc_37DC6:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0
			je		short loc_37DFC
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short loc_37DFC
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short loc_37DFC
loc_37DDD:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0
			je		short loc_37E12
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short loc_37E12
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short loc_37E12
			ret
loc_37DF5:	mov		byte ptr _levelmap[esi + 1], 40h
			jmp		short loc_37E27
loc_37DFC:	cmp		word ptr _levelmap[esi - 2], 0
			je		short loc_37E05
			jmp		short loc_37DDD
loc_37E05:	mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			jmp		short loc_37E27
loc_37E12:	cmp		word ptr _levelmap[esi + 2], 0
			je		short loc_37E1A
			ret
loc_37E1A:	mov		byte ptr _levelmap[esi + 1], 60h
			mov		word ptr _levelmap[esi + 2], 8888h
			jmp		short loc_37E27
loc_37E27:	mov		bl, byte ptr _levelmap[esi + 1]
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
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 16h
			jnz		short loc_37EA5
			mov		byte ptr _levelmap[esi + 1], bl
			sub		esi, 2 * MAP_WIDTH
			call	sub_382EA
			add		esi, 2 * MAP_WIDTH
			ret
loc_37EA5:	cmp		bl, MAP_ELECTRON
			jge		short loc_37EAF
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_37EAF:	mov		byte ptr _levelmap[esi + 1], 0
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
			jnz		short loc_37EBE
			jmp		loc_37F4D
loc_37EBE:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			jnz		short loc_37EC9
			jmp		loc_37F4D
loc_37EC9:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
			jnz		short loc_37ED3
			jmp		loc_37F84
loc_37ED3:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_RED
			jnz		short loc_37EDD
			jmp		loc_37FA6
loc_37EDD:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SNIK_SNAK
			jnz		short loc_37EE7
			jmp		loc_37FA6
loc_37EE7:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ELECTRON
			jnz		short loc_37EF1
			jmp		loc_37FA6
loc_37EF1:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_YELLOW
			jnz		short loc_37EFB
			jmp		loc_37FA6
loc_37EFB:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 8
			jnz		short loc_37F05
			jmp		loc_37FA6
loc_37F05:	;call	SND_nnn
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 1
			je		short loc_37F1E
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 4
			je		short loc_37F1E
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 5
			je		short loc_37F1E
			ret
loc_37F1E:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0
			jz		short loc_37F5A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			jz		short loc_37F5A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			jz		short loc_37F5A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0
			jz		short loc_37F70
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			jz		short loc_37F70
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			jz		short loc_37F70
			ret
loc_37F4D:	mov		word ptr _levelmap[esi], 7000h + 4
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_37F5A:	cmp		word ptr _levelmap[esi - 2], 0
			je		short loc_37F64
			jmp		loc_37DDD
loc_37F64:	mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			ret
loc_37F70:	cmp		word ptr _levelmap[esi + 2], 0
			je		short loc_37F78
			ret
loc_37F78:	mov		byte ptr _levelmap[esi + 1], 60h
			mov		word ptr _levelmap[esi + 2], 8888h
			ret
loc_37F84:	mov		bl, byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1]
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
			call	CreateExplosion
locret_37FAC:
			ret
loc_37FAD:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 24h
			jne		short loc_37FE5
			mov		word ptr _levelmap[esi + 2], 0AAAAh
loc_37FE5:	cmp		bl, 26h
			jne		short loc_37FF8
			mov		byte ptr _levelmap[esi + 1], bl
			add		esi, 2
			call	sub_382EA
			sub		esi, 2
			ret
loc_37FF8:	cmp		bl, 28h
			jge		short loc_38002
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_38002:	mov		word ptr _levelmap[esi], 7000h + 4
			ret
loc_38009:	and		bl, 7
			shl		bl, 1
			xor		bh, bh
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 34h
			jne		short loc_38041
			mov		word ptr _levelmap[esi - 2], 0AAAAh
loc_38041:	cmp		bl, 36h
			jne		short loc_38053
			mov		byte ptr _levelmap[esi + 1], bl
			sub		esi, 2
			call	sub_382EA
			add		esi, 2
loc_38053:	cmp		bl, 38h
			jge		short loc_3805D
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_3805D:	mov		word ptr _levelmap[esi], 7000h + 4
			ret
loc_38064:	inc		bl
			cmp		bl, 42h
			jge		short loc_38070
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_38070:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
			jz		short loc_3807E
			dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_3807E:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		byte ptr _levelmap[esi], 4
			mov		byte ptr _levelmap[esi + 1], 10h
			ret
loc_38092:	xor		bh, bh
			and		bl, 0Fh
			shl		bl, 1
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 52h
			jge		short loc_380C9
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_380C9:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0
			jnz		short loc_380F9
			cmp		word ptr _levelmap[esi - 2], 0
			jz		short loc_380DF
			cmp		word ptr _levelmap[esi - 2], 8888h
			jnz		short loc_380F9
loc_380DF:	mov		word ptr _levelmap[esi], 0FFFFh
			sub		esi, 2
			mov		byte ptr _levelmap[esi], 4
			mov		byte ptr _levelmap[esi + 1], 22h
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_380F9:	dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_38100:	xor		bh, bh
			and		bl, 7
			shl		bl, 1
			mov		bl, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 62h
			jge		short loc_38137
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_38137:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0
			jne		short loc_38167
			cmp		word ptr _levelmap[esi + 2], 0
			je		short loc_3814D
			cmp		word ptr _levelmap[esi + 2], 8888h
			jne		short loc_38167
loc_3814D:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2
			mov		byte ptr _levelmap[esi], 4
			mov		byte ptr _levelmap[esi + 1], 32h
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_38167:	dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_3816E:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
			je		short loc_3817E
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			je		short loc_3817E
			ret
loc_3817E:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _levelmap[esi], 1004h
			jmp		loc_37E62

handleInfotron	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_38191	proc near

			cmp		byte ptr _levelmap[esi], MAP_RED_LAMP
			je		short loc_3819E
			mov		word ptr _levelmap[esi], 0
loc_3819E:	cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
			je		short loc_381C8
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_381C8
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 1
			je		short loc_381BC
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 4
			je		short loc_381C2
			ret
loc_381BC:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 40h
			ret
loc_381C2:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 40h
			ret
loc_381C8:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 1
			je		short loc_381E5
			cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 4
			je		loc_3822A
loc_381D6:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 1
			je		short loc_38208
			cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 4
			je		loc_3824D
			ret
loc_381E5:	cmp		word ptr _levelmap[esi - 2], 1
			je		short loc_381FC
			cmp		word ptr _levelmap[esi - 2], 4
			je		short loc_381FC
			cmp		word ptr _levelmap[esi - 2], 5
			je		short loc_381FC
			jmp		short loc_381D6
loc_381FC:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH + 1)], 60h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_38208:	cmp		word ptr _levelmap[esi + 2], 1
			je		short loc_3821E
			cmp		word ptr _levelmap[esi + 2], 4
			je		short loc_3821E
			cmp		word ptr _levelmap[esi + 2], 5
			je		short loc_3821E
			ret
loc_3821E:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 3)], 50h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
loc_3822A:	cmp		word ptr _levelmap[esi - 2], 1
			je		short loc_38241
			cmp		word ptr _levelmap[esi - 2], 4
			je		short loc_38241
			cmp		word ptr _levelmap[esi - 2], 5
			je		short loc_38241
			jmp		loc_381D6
loc_38241:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH + 1)], 60h ; '`'
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_3824D:	cmp		word ptr _levelmap[esi + 2], 1
			je		short loc_38263
			cmp		word ptr _levelmap[esi + 2], 4
			je		short loc_38263
			cmp		word ptr _levelmap[esi + 2], 5
			je		short loc_38263
			ret
loc_38263:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 3)], 50h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_38191	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3826F	proc near

			cmp		byte ptr _levelmap[esi], MAP_RED_LAMP
			je		short loc_3827C
			mov		word ptr _levelmap[esi], 0
loc_3827C:	cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
			je		short loc_38294
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_3828C
			ret
loc_3828C:	cmp		byte ptr _levelmap[esi - (4 * MAP_WIDTH)], 4
			je		short loc_38294
			ret
loc_38294:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 1
			je		short loc_382A3
loc_3829B:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 1
			je		short loc_382C7
			ret
loc_382A3:	cmp		word ptr _levelmap[esi - 2], 1
			je		short loc_382BA
			cmp		word ptr _levelmap[esi - 2], 4
			je		short loc_382BA
			cmp		word ptr _levelmap[esi - 2], 5
			je		short loc_382BA
			jmp		short loc_3829B
loc_382BA:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 6001h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_382C7:	cmp		word ptr _levelmap[esi + 2], 1
			je		short loc_382DD
			cmp		word ptr _levelmap[esi + 2], 4
			je		short loc_382DD
			cmp		word ptr _levelmap[esi + 2], 5
			je		short loc_382DD
			ret
loc_382DD:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 5001h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_3826F	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_382EA	proc near

			cmp		byte ptr _levelmap[esi], MAP_RED_LAMP
			je		short loc_382F7
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_382F7:	cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
			je		short loc_3830F
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_38307
			ret
loc_38307:	cmp		byte ptr _levelmap[esi - (4 * MAP_WIDTH)], 1
			je		short loc_3830F
			ret
loc_3830F:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 4
			je		short loc_3831E
loc_38316:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 4
			je		short loc_38342
			ret
loc_3831E:	cmp		word ptr _levelmap[esi - 2], 1
			je		short loc_38335
			cmp		word ptr _levelmap[esi - 2], 4
			je		short loc_38335
			cmp		word ptr _levelmap[esi - 2], 5
			je		short loc_38335
			jmp		short loc_38316
loc_38335:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 6004h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_38342:	cmp		word ptr _levelmap[esi + 2], 1
			je		short loc_38358
			cmp		word ptr _levelmap[esi + 2], 4
			je		short loc_38358
			cmp		word ptr _levelmap[esi + 2], 5
			je		short loc_38358
			ret
loc_38358:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 5004h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_382EA	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

initplayerstate label byte
_initplayerstate	proc near

			mov		ax, word ptr [PlayerPosition_MapX]
			mov		word ptr [PlayerPosition2_MapX], ax
			mov		ax, word ptr [PlayerPosition_MapY]
			mov		word ptr [PlayerPosition2_MapY], ax
			xor		ax, ax
			mov		word ptr [word_403BB], ax
			mov		word ptr [word_403C1], ax
			mov		word ptr [word_40C64], ax
			mov		word ptr [TimeoutAfterWhichLeaveTheLevel], ax
			mov		byte ptr [byte_40C4C], 0
			mov		byte ptr [byte_40C6C], 0
			mov		word ptr [word_4C47F], 0
			mov		word ptr [word_4C47B], 0
			mov		word ptr [word_4C47D], 0
			mov		word ptr [word_403BD], 0
			mov		byte ptr [byte_403A7], 0FFh
			mov		byte ptr [byte_403A8], 0FFh
			mov		byte ptr [byte_403A9], 0FFh
			mov		byte ptr [byte_4039E], 1
			mov		byte ptr [byte_4039F], 0
			mov		byte ptr [byte_403A0], 0
			mov		byte ptr [byte_403A1], 0
			mov		byte ptr [byte_403A2], 0
			mov		byte ptr [byte_403A4], 0
			mov		byte ptr [byte_403A5], 0
			mov		byte ptr [byte_403A6], 0
			mov		byte ptr [byte_403B0], 0
			mov		byte ptr [byte_40C5A], 7Fh
			mov		byte ptr [byte_40C5B], 0
			mov		word ptr [word_40C4D], 0
			mov		byte ptr [byte_403B1], 1
			mov		byte ptr [byte_403B2], 0
			mov		byte ptr [byte_403C7], 0
			ret

_initplayerstate	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

beginlevel label byte
_beginlevel proc near

			cmp		byte ptr [IsDemoRunning], 0
			jz		short loc_38456
			mov		byte ptr [byte_403AA], 0
			jmp		short loc_3845B
loc_38456:	mov		byte ptr [byte_403AA], 1
loc_3845B:	;mov		word ptr [word_40392], 1
			ret

_beginlevel endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

runthelevel label byte
_runthelevel	proc near

			push	ebp
			push	esi
			push	edi
			push 	ebx

			cmp		byte ptr [IsDemoRunning], 0
			jne		short loc_3846B
			;call	PollJoystick
loc_3846B:	xor ebx, ebx;call	PollMouse
			cmp		bx, 1
			jnz		short loc_38480
			cmp		word ptr [TimeoutAfterWhichLeaveTheLevel], 0
			jg		short loc_38480
			mov		word ptr [word_403C1], 1
loc_38480:	cmp		bx, 2
			jnz		short loc_3848F
			cmp		word ptr [word_40C60], 0
			jz		short loc_3848F
			;call	sub_386E7
loc_3848F:	;call	HandleUserInput
			cmp		word ptr [word_40CF3], 0
			jz		short loc_384A5
loc_384A5:	call	MoveObjectsInTheLevel
			cmp		word ptr [word_40CF3], 0
			jz		short loc_384BB
loc_384BB:	;call	PrepareStatsInfoForBottomPanel
			;call	sub_3EC47
			cmp		word ptr [word_40CF3], 0
			jz		short loc_384D8
loc_384D8:	call	PlaceRedDisk
			call	sub_39530
			call	sub_38F26
			mov		bx, word ptr [word_40C4F]
			mov		cl, 3
			shr		bx, cl
			mov		ax, word ptr [word_40C51]
			mov		cx, word ptr [word_40C4F]
			mov		ah, cl
			and		ah, 7
			cmp		word ptr [word_40CF3], 0
			jz		short loc_3852B
			cmp		word ptr [word_40CF7], 1
loc_3852B:	jbe		short loc_3853C
			;call	SND_bbb
loc_3853C:	mov		cx, word ptr [word_40CF7]
loc_38540:	dec		cx
			jz		short loc_38581
			;call	VID_WaitVerticalRetrace
			;call	VID_SetHorizontalPanning
			push	ecx
			;call	HandleUserInput
			call	sub_38F26
			pop		ecx
			jmp		short loc_38540
loc_38581:	cmp		word ptr [word_40CF3], 0
			jz		short loc_38592
loc_38592:	inc		word ptr [word_40C4D]
			cmp		word ptr [word_40C64], 1
			jz		short loc_385B0
			cmp		word ptr [TimeoutAfterWhichLeaveTheLevel], 0
			jnz		short loc_385A7
			mov		eax, 1
			jmp short locret
loc_385A7:	dec		word ptr [TimeoutAfterWhichLeaveTheLevel]
			jz		short loc_385B0
			mov		eax, 1
			jmp short locret
loc_385B0:	;mov		[word_40392], 0
			;cmp		[byte_403D3], 0
			;jz		short loc_385C0
			;call	sub_38A5C
;loc_385C0:	
			cmp		byte ptr [byte_403A3], 0
			jz		short locret_385DC
			mov		al, byte ptr [byte_403A0]
			mov		byte ptr [byte_403A4], al
			mov		al, byte ptr [byte_403A1]
			mov		byte ptr [byte_403A5], al
			mov		al, byte ptr [byte_403A2]
			mov		byte ptr [byte_403A6], al
			;call	sub_398B5
locret_385DC:
			xor		eax, eax

locret:
			pop		ebx
			pop		edi
			pop		esi
			pop		ebp
			ret

_runthelevel	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

MoveObjectsInTheLevel proc near

			mov		esi, dword ptr [PlayerPosition_Ofs]
			call	xxx_HandleMurphy
			mov		dword ptr [PlayerPosition_Ofs], esi
			cmp		word ptr [word_40CF3], 0
			jz		short loc_38E6D
loc_38E6D:	cmp		byte ptr [byte_403B1], 0
			jz		short loc_38E77
			;call	sub_3F116
loc_38E77:	cmp		byte ptr [word_40CF3], 0
			jz		short loc_38E96
loc_38E96:	mov		esi, 2 * MAP_WIDTH + 2
			;mov		edi, offset dword_46231
			mov		ecx, MAP_WIDTH * MAP_HEIGHT - (2 * MAP_WIDTH + 2)
			mov		edx, 0
loc_38EA2:	mov		bl, byte ptr _levelmap[esi]
			cmp		bl, 32
			jnb		short loc_38EC9
			movzx	ebx, bl
			shl		ebx, 2
			cmp		dword ptr MapHandlers_4[ebx], 0
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
loc_38EE8:	cmp		word ptr [word_403C1], 1
			je		short loc_38F01
			cmp		word ptr [word_403BF], 0
			je		short loc_38F01
			ret
loc_38F01:	cmp		word ptr [TimeoutAfterWhichLeaveTheLevel], 0
			jne		short loc_38F1B
			mov		word ptr [word_403C1], 0
			mov		esi, dword ptr [dword_403B7]
			call	CreateExplosion
			mov		word ptr [TimeoutAfterWhichLeaveTheLevel], 40h
loc_38F1B:	ret

MoveObjectsInTheLevel endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_38F26	proc near

		cmp	byte ptr [byte_403B0], 0
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
		mov	bx, word ptr [PlayerPosition_PixelsX]
		mov	ax, word ptr [PlayerPosition_PixelsY]
		add	bx, cx
		add	ax, dx
		sub	bx, 98h
		cmp	bx, 8
		jge	short loc_38F5F
		mov	bx, 8

loc_38F5F:
		cmp	bx, 278h
		jle	short loc_38F68
		mov	bx, 278h

loc_38F68:
		cmp	byte ptr [byte_403B1], 0
		jz	short loc_38F74
		sub	ax, 58h
		jmp	short loc_38F77


loc_38F74:
		sub	ax, 64h

loc_38F77:
		cmp	ax, 0
		jge	short loc_38F7F
		mov	ax, 0

loc_38F7F:
		cmp	byte ptr [byte_403B1], 0
		jz	short loc_38F90
		cmp	ax, 0C0h
		jle	short loc_38F8E
		mov	ax, 0C0h

loc_38F8E:
		jmp	short loc_38F98


loc_38F90:
		cmp	ax, 0A8h ; 'Ј'
		jle	short loc_38F98
		mov	ax, 0A8h ; 'Ј'

loc_38F98:
		add	bx, word ptr [word_40C53]
		add	ax, word ptr [word_40C55]
		mov	word ptr [word_40C4F], bx
		mov	word ptr [word_40C51], ax
		ret
sub_38F26	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

xxx_HandleBug	proc near
		cmp	byte ptr _levelmap[esi], MAP_BUG
		jz	short loc_39012
		ret

loc_39012:
		mov	ax, word ptr [word_40C4D]
		and	ax, 3
		cmp	ax, 0
		jz	short loc_3901E
		ret

loc_3901E:
		mov	bl, byte ptr _levelmap[esi + 1]
		inc	bl
		cmp	bl, MAP_PORT_SPECIAL_UP_TO_DOWN
		jl	short loc_39034
		call	GenerateRandomNumber
		and	al, 3Fh
		add	al, 20h	; ' '
		neg	al
		mov	bl, al

loc_39034:
		mov	byte ptr _levelmap[esi + 1], bl
		cmp	bl, MAP_SPACE
		jge	short loc_3903E
		ret


loc_3903E:
		cmp	byte ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _levelmap[esi - 2], MAP_MURPHY	; -1
		jz	short loc_39078
		cmp	byte ptr _levelmap[esi + 2], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
		jz	short loc_39078
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_MURPHY
		jz	short loc_39078
		jmp	short loc_3907B


loc_39078:
		;call	SND_aaa

loc_3907B:
		xor	bh, bh
		;mov	di, word ptr word_456CB[esi]
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
		cmp	byte ptr _levelmap[esi], MAP_TERMINAL
		jz	short loc_390A7
		ret


loc_390A7:
		mov	bl, byte ptr _levelmap[esi + 1]
		inc	bl
		cmp	bl, 0
		jg	short loc_390B7
		mov	byte ptr _levelmap[esi + 1], bl
		ret


loc_390B7:
		call	GenerateRandomNumber
		and	al, byte ptr [byte_40C5A]
		neg	al
		mov	bl, al
		mov	byte ptr _levelmap[esi + 1], bl
		;mov	edi, word ptr word_456CB[esi]
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

initrandomnumbergenerator label byte
_initrandomnumbergenerator proc near

		mov cx, 01234h
		mov dx, 4321h ; FIXME
		xor	cx, dx
		mov	word ptr [RandomSeed], cx
		ret
_initrandomnumbergenerator endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


GenerateRandomNumber proc near
		mov	ax, word ptr [RandomSeed]
		mov	bx, 1509
		mul	bx
		add	ax, 49
		mov	word ptr [RandomSeed], ax
		shr	ax, 1
		ret
GenerateRandomNumber endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

findplayerpositiononlevelmap label byte
_findplayerpositiononlevelmap proc near

			push	ebp
			push	esi
			push	edi
			push 	ebx

			xor		ax, ax
			mov		word ptr [word_40C4F], ax
			mov		word ptr [word_40C51], ax
			mov		edi, offset _levelmap
			mov		ecx, MAP_NUM_CELLS
			mov		ax, MAP_MURPHY
			repne 	scasw
			dec		edi
			dec		edi
			mov		eax, MAP_WIDTH * MAP_HEIGHT - 1
			sub		eax, ecx
			mov		ecx, eax
			shl		ecx, 1
			mov		dword ptr [PlayerPosition_Ofs], ecx
			mov		bl, MAP_WIDTH
			div		bl
			mov		bl, ah
			xor		bh, bh
			mov		word ptr [PlayerPosition_MapX], bx
			xor		ah, ah
			mov		word ptr [PlayerPosition_MapY], ax
			mov		cl, 4
			shl		ax, cl
			shl		bx, cl
			mov		word ptr [PlayerPosition_PixelsX],	bx
			mov		word ptr [PlayerPosition_PixelsY],	ax
			call	sub_38F26

			pop		ebx
			pop		edi
			pop		esi
			pop		ebp

			ret

_findplayerpositiononlevelmap endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_392df label byte
_sub_392df	proc near

		push	ebp
		push	esi
		push	edi
		push 	ebx

		xor	ebx, ebx
		xor	edx, edx
		mov	ecx, MAP_WIDTH * MAP_HEIGHT
		mov	esi, offset _levelmap

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
		cmp	word ptr [esi - 2], MAP_SPACE
		jnz	short loc_3933F
		mov	byte ptr [esi + 1], 1
		jmp	short loc_39398


loc_3933F:
		cmp	word ptr [esi - 2 * MAP_WIDTH], 0
		jnz	short loc_39350
		mov	word ptr [esi - 2 * MAP_WIDTH], 1000h + MAP_SNIK_SNAK
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398


loc_39350:
		cmp	word ptr [esi+2], 0
		jnz	short loc_39398
		mov	word ptr [esi+2], 2800h + MAP_SNIK_SNAK
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
		mov	word ptr [esi-78h], 1000h + MAP_ELECTRON
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398

loc_3937E:
		cmp	word ptr [esi+2], 0
		jnz	short loc_39398
		mov	word ptr [esi+2], 2800h + MAP_ELECTRON
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
		cmp	byte ptr [byte_40326], 0
		jz	short loc_393AD
		mov	dl, byte ptr [byte_40326]

loc_393AD:
		mov	byte ptr [InfotronsLeftToCollect],	dl
		mov	byte ptr [TotalInfotronsInTheLevel], dl
		;call	sub_3EC01

		pop	ebx
		pop	edi
		pop	esi
		pop	ebp

		ret
_sub_392df	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

xxx_handleOrangeDisk proc near
		cmp	byte ptr _levelmap[esi], MAP_DISK_ORANGE
		jz	short loc_393D7
		ret

loc_393D7:
		mov	ax, word ptr _levelmap[esi]
		cmp	ax, 3000h + 8
		jge	short loc_39424
		cmp	ax, 2000h + 8
		jge	short loc_393FA
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
		cmp	ax, 0
		jz	short loc_393EF
		ret

loc_393EF:
		mov	byte ptr _levelmap[esi + 1], 20h
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 8
		ret

loc_393FA:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_39408
		mov	word ptr _levelmap[esi], 8
		ret

loc_39408:
		mov	bl, byte ptr _levelmap[esi + 1]
		inc	bl
		cmp	bl, 22h
		jnz	short loc_39415
		mov	bl, 30h

loc_39415:
		mov	byte ptr _levelmap[esi + 1], bl
		ret

		mov	byte ptr _levelmap[esi + 1], 30h
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 8

loc_39424:
		mov	bl, byte ptr _levelmap[esi + 1]
		inc	bl
		mov	al, bl
		and	al, 7
		jz	short loc_39466
		mov	byte ptr _levelmap[esi + 1], bl
		ret


loc_39466:
		mov	word ptr _levelmap[esi], 0
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 8
		add	esi, 78h
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_39487
		mov	byte ptr _levelmap[esi + 1], 30h ; '0'
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 8
		ret


loc_39487:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_RED_LAMP
		jnz	short loc_3948F
		ret


loc_3948F:
		call	CreateExplosion
		ret
xxx_handleOrangeDisk endp


loc_39493:
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3949B
		ret

loc_3949B:
		mov	ax, word ptr [word_40C4D]
		and	ax, 3
		cmp	ax, 0
		jz	short loc_394A7
		ret

loc_394A7:
		mov	bl, byte ptr _levelmap[esi + 1]
		test	bl, 80h
		jnz	short loc_394F0
		inc	bl
		mov	byte ptr _levelmap[esi + 1], bl
		cmp	bl, 8
		jnz	short locret_394EF
		mov	word ptr _levelmap[esi], 0
		mov	byte ptr [byte_403B0], 0

locret_394EF:
		ret


loc_394F0:
		inc	bl
		cmp	bl, 89h
		jnz	short loc_39503
		mov	word ptr _levelmap[esi], 4
		mov	byte ptr [byte_403B0], 0
		ret


loc_39503:
		mov	byte ptr _levelmap[esi + 1], bl
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
		call	CreateExplosion
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
		mov	word ptr _levelmap[esi], 0FF00h + MAP_ELECTRON
		call	CreateExplosion
		pop	ebx
		pop	ecx
		pop	esi
		jmp	short loc_39551
sub_39530	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


CreateExplosion	proc near
		cmp	byte ptr _levelmap[esi], MAP_GRAY_DENTED_PYRAMID
		jnz	short loc_39577
		ret


loc_39577:
		mov	byte ptr [byte_403B0], 1
		cmp	byte ptr _levelmap[esi], 3
		jnz	short loc_39589
		mov	word ptr [word_403C1], 1

loc_39589:
		cmp	byte ptr _levelmap[esi], MAP_ELECTRON
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
		mov	ax, word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)]
		cmp	al, 8
		jz	short loc_395D0
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_395D0
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_395D0
		cmp	al, 1
		jz	short loc_395EC
		cmp	al, 4
		jz	short loc_395E2
		cmp	al, MAP_ELECTRON
		jnz	short loc_395C6
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_395D0

loc_395C6:
		cmp	al, 3
		jnz	short loc_395D8
		mov	word ptr [word_403C1], 1

loc_395D0:
		cmp	al, 6
		jz	short loc_395D8
		mov	SomeLevelData[ebx - MAP_WIDTH - 1], dh

loc_395D8:
		cmp	al, 6
		jz	short loc_395E0
		mov	word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], cx

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
		mov	ax, word ptr _levelmap[esi - 2 * MAP_WIDTH]
		cmp	al, 8
		jz	short loc_39627
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39627
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39627
		cmp	al, 1
		jz	short loc_39643
		cmp	al, 4
		jz	short loc_39639
		cmp	al, MAP_ELECTRON
		jnz	short loc_3961D
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39627

loc_3961D:
		cmp	al, 3
		jnz	short loc_3962F
		mov	word ptr [word_403C1], 1

loc_39627:
		cmp	al, 6
		jz	short loc_3962F
		mov	SomeLevelData[ebx - MAP_WIDTH], dh

loc_3962F:
		cmp	al, 6
		jz	short loc_39637
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], cx

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
		mov	ax, word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)]
		cmp	al, 8
		jz	short loc_3967E
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_3967E
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_3967E
		cmp	al, 1
		jz	short loc_3969A
		cmp	al, 4
		jz	short loc_39690
		cmp	al, MAP_ELECTRON
		jnz	short loc_39674
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_3967E

loc_39674:
		cmp	al, 3
		jnz	short loc_39686
		mov	word ptr [word_403C1], 1

loc_3967E:
		cmp	al, 6
		jz	short loc_39686
		mov	SomeLevelData[ebx - (2 * MAP_WIDTH - 1)], dh

loc_39686:
		cmp	al, 6
		jz	short loc_3968E
		mov	word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], cx

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
		mov	ax, word ptr _levelmap[esi - 2]
		cmp	al, 8
		jz	short loc_396D5
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_396D5
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_396D5
		cmp	al, 1
		jz	short loc_396F1
		cmp	al, 4
		jz	short loc_396E7
		cmp	al, MAP_ELECTRON
		jnz	short loc_396CB
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_396D5


loc_396CB:
		cmp	al, 3
		jnz	short loc_396DD
		mov	word ptr [word_403C1], 1

loc_396D5:
		cmp	al, 6
		jz	short loc_396DD
		mov	SomeLevelData[ebx - 1], dh

loc_396DD:
		cmp	al, 6
		jz	short loc_396E5
		mov	word ptr _levelmap[esi - 2], cx

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
		mov	word ptr _levelmap[esi], cx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _levelmap[esi + 2]
		cmp	al, 8
		jz	short loc_39730
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39730
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39730
		cmp	al, 1
		jz	short loc_3974C
		cmp	al, 4
		jz	short loc_39742
		cmp	al, MAP_ELECTRON
		jnz	short loc_39726
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39730

loc_39726:
		cmp	al, 3
		jnz	short loc_39738
		mov	word ptr [word_403C1], 1

loc_39730:
		cmp	al, 6
		jz	short loc_39738
		mov	SomeLevelData[ebx + 1], dh

loc_39738:
		cmp	al, 6
		jz	short loc_39740
		mov	word ptr _levelmap[esi + 2], cx

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
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH - 2]
		cmp	al, 8
		jz	short loc_39787
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39787
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39787
		cmp	al, 1
		jz	short loc_397A3
		cmp	al, 4
		jz	short loc_39799
		cmp	al, MAP_ELECTRON
		jnz	short loc_3977D
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39787

loc_3977D:
		cmp	al, 3
		jnz	short loc_3978F
		mov	word ptr [word_403C1], 1

loc_39787:
		cmp	al, 6
		jz	short loc_3978F
		mov	byte ptr SomeLevelData[ebx + 2 * MAP_WIDTH - 1], dh

loc_3978F:
		cmp	al, 6
		jz	short loc_39797
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], cx

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
		add	esi, 76h
		call	sub_398F9
		pop	esi
		jmp	short loc_397AD

loc_397AD:
		pop	ecx
		mov	dh, dl
		push	ecx
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
		cmp	al, 8
		jz	short loc_397DE
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_397DE
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_397DE
		cmp	al, MAP_ZONK
		jz	short loc_397FA
		cmp	al, 4
		jz	short loc_397F0
		cmp	al, MAP_ELECTRON
		jnz	short loc_397D4
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_397DE

loc_397D4:
		cmp	al, 3
		jnz	short loc_397E6
		mov	word ptr [word_403C1], 1

loc_397DE:
		cmp	al, 6
		jz	short loc_397E6
		mov	SomeLevelData[ebx + MAP_WIDTH], dh

loc_397E6:
		cmp	al, 6
		jz	short loc_397EE
		mov word ptr _levelmap[esi + 2 * MAP_WIDTH], cx

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
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH + 2]
		cmp	al, 8
		jz	short loc_39835
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39835
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39835
		cmp	al, 1
		jz	short loc_39851
		cmp	al, 4
		jz	short loc_39847
		cmp	al, MAP_ELECTRON
		jnz	short loc_3982B
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39835


loc_3982B:
		cmp	al, 3
		jnz	short loc_3983D
		mov	word ptr [word_403C1], 1

loc_39835:
		cmp	al, 6
		jz	short loc_3983D
		mov	SomeLevelData[ebx + MAP_WIDTH + 1], dh

loc_3983D:
		cmp	al, 6
		jz	short loc_39845
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], cx

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
CreateExplosion	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

PlaceRedDisk	proc near
		mov		al, byte ptr [NumRedDisks]
		cmp		al, 1
		jle		short loc_398AA
		mov		esi, dword ptr [dword_403CC]
		cmp		word ptr _levelmap[esi], 0
		jnz		short loc_39882
		mov		word ptr _levelmap[esi], MAP_DISK_RED
loc_39882:
		inc	byte ptr [NumRedDisks]
		cmp	byte ptr [NumRedDisks], 40
		jl	short loc_398AA
		mov	esi, dword ptr [dword_403CC]
		call	CreateExplosion
		mov	byte ptr [NumRedDisks], 0

loc_398AA:
		ret
PlaceRedDisk	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_398F9	proc near
		mov	word ptr _levelmap[esi], cx
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
		cmp	word ptr _levelmap[esi], 9999h
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

		mov	word ptr _levelmap[esi], cx
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
		cmp	word ptr _levelmap[esi], 9999h
		jnz	short locret_399A9
		call	sub_399E9

locret_399A9:
		ret

loc_399AA:
		add	esi, 2
		call	sub_399E9
		add	esi, 76h
		cmp	word ptr _levelmap[esi], 9999h
		jnz	short locret_399BE
		call	sub_399E9

locret_399BE:
		ret

loc_399BF:
		sub	esi, 2
		call	sub_399E9
		add	esi, 7Ah
		cmp	word ptr _levelmap[esi], 9999h
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

		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jnz	short loc_399F1
		ret

loc_399F1:
		mov	word ptr _levelmap[esi], 0
		ret
sub_399E9	endp


loadlevelmap label byte
_loadlevelmap	proc near

		push	edi

		mov		edi, offset SomeLevelData
		xor		al, al
		mov		ecx, 1536
		cld
		rep 	stosb

		pop edi
		ret

_loadlevelmap	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

xxx_HandleMurphy proc near

		mov	word ptr [word_403BF], 1
		cmp	byte ptr _levelmap[esi], MAP_MURPHY
		jz	short loc_3CA6E
		ret

loc_3CA6E:
		mov	dword ptr [dword_403B7], esi
		mov	ax, word ptr _levelmap[esi]
		cmp	ax, MAP_MURPHY
		jz	short loc_3CA7E
		jmp	loc_3D84B

loc_3CA7E:
		mov	byte ptr [byte_403C8], 0
		cmp	byte ptr [byte_4030C], 0
		jz	short loc_3CAAB
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_PORT_DOWN_TO_UP
		jz	short loc_3CAAB
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_PORT_VERTICAL
		jz	short loc_3CAAB
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_PORT_CROSS
		jz	short loc_3CAAB
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
		jnz	short loc_3CAAB
		mov	byte ptr [byte_403C8], 1

loc_3CAAB:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 0
		jz	short loc_3CAB7
		jmp	loc_3CBEF

loc_3CAB7:
		mov	byte ptr [byte_403C3], 1
		cmp	byte ptr [byte_403C8], 0
		jz	short loc_3CAC6
		jmp	loc_3CF84

loc_3CAC6:
		mov	ax, word ptr [word_40C4D]
		and	ax, 3
		jz	short loc_3CACF
		ret

loc_3CACF:
		inc	word ptr [word_403BD]
		cmp	word ptr [word_403BD], 4
		jnz	short loc_3CAEE
		ret

loc_3CAEE:
		cmp	word ptr [word_403BD], 500
		jg	short loc_3CAF7
		ret

loc_3CAF7:
		cmp	word ptr [word_403BD], 522
		jg	short loc_3CB20
		ret

loc_3CB20:
		cmp	word ptr [word_403BD], 1000
		jg	short loc_3CB29
		ret

loc_3CB29:
		cmp	word ptr [word_403BD], 1022
		jg	short loc_3CB52
		ret

loc_3CB52:
		cmp	word ptr [word_403BD], 1600
		jg	short loc_3CB5B
		ret

loc_3CB5B:
		cmp	word ptr [word_403BD], 1622
		jg	short loc_3CB84
		ret

loc_3CB84:
		cmp	word ptr [word_403BD], 1654
		jg	short locret_3CBA0
		cmp	word ptr _levelmap[esi - 2], 0
		jnz	short loc_3CBA1
		cmp	word ptr _levelmap[esi + 2], 0
		jnz	short loc_3CBC8
		mov	word ptr [word_403BD], 24h

locret_3CBA0:
loc_3CBA1:
loc_3CBC8:
		ret

loc_3CBEF:
		cmp	byte ptr [byte_403C8], 0
		jz	short loc_3CC23
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3CC23
		cmp	bl, 1
		jnz	short loc_3CC09
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 2
		jz	short loc_3CC23

loc_3CC09:
		cmp	bl, 2
		jnz	short loc_3CC15
		cmp	word ptr _levelmap[esi - 2], 2
		jz	short loc_3CC23

loc_3CC15:
		cmp	bl, 4
		jnz	short loc_3CC21
		cmp	word ptr _levelmap[esi + 2], 2
		jz	short loc_3CC23

loc_3CC21:
		mov	bl, 3

loc_3CC23:
		cmp	bl, 1
		jnz	short loc_3CC2F
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CC98

loc_3CC2F:
		cmp	bl, 2
		jnz	short loc_3CC3C
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CCFA

loc_3CC3C:
		cmp	bl, 3
		jnz	short loc_3CC49
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CD74

loc_3CC49:
		cmp	bl, 4
		jnz	short loc_3CC56
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CDD6

loc_3CC56:
		cmp	bl, 5
		jnz	short loc_3CC63
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CE4E

loc_3CC63:
		cmp	bl, 6
		jnz	short loc_3CC70
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CE78

loc_3CC70:
		cmp	bl, MAP_EXIT
		jnz	short loc_3CC7D
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CEA8

loc_3CC7D:
		cmp	bl, 8
		jnz	short loc_3CC8A
		mov	byte ptr [byte_403C3], 0
		jmp	loc_3CED2

loc_3CC8A:
		cmp	bl, 9
		jnz	short loc_3CC92
		jmp	loc_3CF02

loc_3CC92:
		mov	byte ptr [byte_403C3], 0
		ret

loc_3CC98:
		mov	ax, word ptr _levelmap[esi - 2 * MAP_WIDTH]
		cmp	ax, 0
		jnz	short loc_3CCA4
		jmp	loc_3CF32

loc_3CCA4:
		cmp	ax, 2
		jnz	short loc_3CCAC
		jmp	loc_3CFF3

loc_3CCAC:
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


loc_3CCFA:
		mov	word ptr [word_403BB], 1
		mov	ax, word ptr _levelmap[esi - 2]
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
		cmp	ax, MAP_EXIT
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
		cmp	al, MAP_PORT_RIGHT_TO_LEFT
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
		cmp	ax, MAP_DISK_RED
		jnz	short loc_3CD57
		jmp	loc_3D5EE


loc_3CD57:				; CODE XREF: xxx_HandleMurphy+2F2j
		cmp	ax, MAP_DISK_YELLOW
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
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
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
		cmp	ax, MAP_EXIT
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
		cmp	al, MAP_DISK_RED
		jnz	short loc_3CDC2
		jmp	loc_3D617


loc_3CDC2:				; CODE XREF: xxx_HandleMurphy+35Dj
		cmp	al, MAP_DISK_YELLOW
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

loc_3CDD6:
		mov	word ptr [word_403BB], 0
		mov	ax, word ptr _levelmap[esi + 2]
		cmp	ax, 0
		jnz	short loc_3CDE8
		jmp	loc_3CFB9

loc_3CDE8:
		cmp	ax, 2
		jnz	short loc_3CDF0
		jmp	loc_3D0C2

loc_3CDF0:
		cmp	al, 19h
		jnz	short loc_3CDF7
		jmp	loc_3D0B1


loc_3CDF7:				; CODE XREF: xxx_HandleMurphy+392j
		cmp	ax, 4
		jnz	short loc_3CDFF
		jmp	loc_3D282


loc_3CDFF:				; CODE XREF: xxx_HandleMurphy+39Aj
		cmp	ax, MAP_EXIT
		jnz	short loc_3CE07
		jmp	loc_3D376


loc_3CE07:				; CODE XREF: xxx_HandleMurphy+3A2j
		cmp	ax, 1
		jnz	short loc_3CE0F
		jmp	loc_3D3F4


loc_3CE0F:
		cmp	al, 13h
		jnz	short loc_3CE16
		jmp	loc_3D4DA

loc_3CE16:
		cmp	al, 9
		jnz	short loc_3CE1D
		jmp	loc_3D5A2

loc_3CE1D:
		cmp	al, 16h
		jnz	short loc_3CE24
		jmp	loc_3D5A2

loc_3CE24:
		cmp	al, 17h
		jnz	short loc_3CE2B
		jmp	loc_3D5A2

loc_3CE2B:
		cmp	al, MAP_DISK_RED
		jnz	short loc_3CE32
		jmp	loc_3D63F

loc_3CE32:
		cmp	al, MAP_DISK_YELLOW
		jnz	short loc_3CE39
		jmp	loc_3D783

loc_3CE39:
		cmp	ax, 8
		jnz	short loc_3CE41
		jmp	loc_3D7F3

loc_3CE41:
		push	esi
		add	esi, 2
		call	sub_3E0D7
		pop	esi
		jb	short locret_3CE4D
		jmp	loc_3CDD6

locret_3CE4D:
		ret

loc_3CE4E:
		mov	ax, word ptr _levelmap[esi - 2 * MAP_WIDTH]
		cmp	ax, 2
		jnz	short loc_3CE5A
		jmp	loc_3D0FF


loc_3CE5A:
		cmp	al, 19h
		jnz	short loc_3CE61
		jmp	loc_3D0EE

loc_3CE61:
		cmp	ax, 4
		jnz	short loc_3CE69
		jmp	loc_3D2AE

loc_3CE69:
		cmp	al, 13h
		jnz	short loc_3CE70
		jmp	loc_3D437

loc_3CE70:
		cmp	al, MAP_DISK_RED
		jnz	short locret_3CE77
		jmp	loc_3D668

locret_3CE77:
		ret

loc_3CE78:
		mov	word ptr [word_403BB], 1
		mov	ax, word ptr _levelmap[esi - 2]
		cmp	ax, 2
		jnz	short loc_3CE8A
		jmp	loc_3D13D

loc_3CE8A:
		cmp	al, 19h
		jnz	short loc_3CE91
		jmp	loc_3D12C

loc_3CE91:
		cmp	ax, 4
		jnz	short loc_3CE99
		jmp	loc_3D2E0

loc_3CE99:
		cmp	al, 13h
		jnz	short loc_3CEA0
		jmp	loc_3D46E

loc_3CEA0:
		cmp	al, MAP_DISK_RED
		jnz	short locret_3CEA7
		jmp	loc_3D684

locret_3CEA7:
		ret

loc_3CEA8:
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
		cmp	ax, 2
		jnz	short loc_3CEB4
		jmp	loc_3D17B

loc_3CEB4:
		cmp	al, 19h
		jnz	short loc_3CEBB
		jmp	loc_3D16A

loc_3CEBB:
		cmp	ax, 4
		jnz	short loc_3CEC3
		jmp	loc_3D312

loc_3CEC3:
		cmp	al, 13h
		jnz	short loc_3CECA
		jmp	loc_3D4A4

loc_3CECA:
		cmp	al, MAP_DISK_RED
		jnz	short locret_3CED1
		jmp	loc_3D6A0

locret_3CED1:
		ret

loc_3CED2:
		mov	word ptr [word_403BB], 0
		mov	ax, word ptr _levelmap[esi + 2]
		cmp	ax, 2
		jnz	short loc_3CEE4
		jmp	loc_3D1B9

loc_3CEE4:
		cmp	al, 19h
		jnz	short loc_3CEEB
		jmp	loc_3D1A8

loc_3CEEB:
		cmp	ax, 4
		jnz	short loc_3CEF3
		jmp	loc_3D344

loc_3CEF3:
		cmp	al, 13h
		jnz	short loc_3CEFA
		jmp	loc_3D4DA

loc_3CEFA:
		cmp	al, MAP_DISK_RED
		jnz	short locret_3CF01
		jmp	loc_3D6BC

locret_3CF01:
		ret

loc_3CF02:
		cmp	byte ptr [byte_40C4C], 0
		jz	short locret_3CF31
		cmp	byte ptr [NumRedDisks], 0
		jnz	short locret_3CF31
		cmp	byte ptr [byte_403C3], 1
		jnz	short locret_3CF31
		mov	byte ptr _levelmap[esi + 1], 2Ah
		mov	word ptr [word_403DE], 40h
		mov	dx, 110Eh
		mov	byte ptr [NumRedDisks], 1
		mov	dword ptr [dword_403CC], esi
		jmp	loc_3D835

locret_3CF31:
		ret

loc_3CF32:
		cmp	word ptr [word_403BB], 0
		jz	short loc_3CF3E
		mov	dx, 0DFEh
		jmp	short loc_3CF41

loc_3CF3E:
		mov	dx, 0E0Eh

loc_3CF41:
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 1
		mov	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		sub	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3CF61:				; CODE XREF: xxx_HandleMurphy+2A9j
		mov	dx, 0E1Eh
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi - 1], 2
		mov	byte ptr _levelmap[esi - 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		sub	esi, 2
		jmp	loc_3D835


loc_3CF84:				; CODE XREF: xxx_HandleMurphy+63j
					; xxx_HandleMurphy+31Dj
		cmp	word ptr [word_403BB], 0
		jz	short loc_3CF90
		mov	dx, 0E2Eh
		jmp	short loc_3CF93


loc_3CF90:				; CODE XREF: xxx_HandleMurphy+529j
		mov	dx, 0E3Eh

loc_3CF93:				; CODE XREF: xxx_HandleMurphy+52Ej
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 3
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		add	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3CFB9:
		mov	dx, 0E4Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 3], 4
		mov	byte ptr _levelmap[esi + 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		add	esi, 2
		jmp	loc_3D835


loc_3CFE2:				; CODE XREF: xxx_HandleMurphy+250j
		cmp	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 0
		jl	short loc_3CFED
		call	CreateExplosion
		ret


loc_3CFED:
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 2

loc_3CFF3:
		;call	SND_bbb
		cmp	word ptr [word_403BB], 0
		jz	short loc_3D002
		mov	dx, 0E6Eh
		jmp	short loc_3D005


loc_3D002:				; CODE XREF: xxx_HandleMurphy+59Bj
		mov	dx, 0E7Eh

loc_3D005:				; CODE XREF: xxx_HandleMurphy+5A0j
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 5
		mov	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		sub	esi, 78h	; 'x'
		jmp	loc_3D835


loc_3D02B:				; CODE XREF: xxx_HandleMurphy+2B8j
		cmp	byte ptr _levelmap[esi - 1], 0
		jl	short loc_3D036
		call	CreateExplosion
		ret


loc_3D036:				; CODE XREF: xxx_HandleMurphy+5D0j
		mov	word ptr _levelmap[esi - 2], 2

loc_3D03C:				; CODE XREF: xxx_HandleMurphy+2B1j
		;call	SND_bbb
		mov	dx, 0E8Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi - 1], 2
		mov	byte ptr _levelmap[esi - 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		sub	esi, 2
		jmp	loc_3D835

loc_3D068:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 0
		jl	short loc_3D073
		call	CreateExplosion
		ret

loc_3D073:
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 2

loc_3D079:
		;call	SND_bbb
		cmp	word ptr [word_403BB], 0
		jz	short loc_3D088
		mov	dx, 0E9Eh
		jmp	short loc_3D08B

loc_3D088:
		mov	dx, 0EAEh

loc_3D08B:
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 7
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		add	esi, 78h
		jmp	loc_3D835

loc_3D0B1:
		cmp	byte ptr _levelmap[esi + 3], 0
		jl	short loc_3D0BC
		call	CreateExplosion
		ret

loc_3D0BC:
		mov	word ptr _levelmap[esi + 2], 2

loc_3D0C2:
		;call	SND_bbb
		mov	dx, 0EBEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 3], 8
		mov	byte ptr _levelmap[esi + 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		add	esi, 2
		jmp	loc_3D835

loc_3D0EE:
		cmp	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 0
		jl	short loc_3D0F9
		call	CreateExplosion
		ret

loc_3D0F9:
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 2

loc_3D0FF:
		;call	SND_bbb
		mov	dx, 0ECEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], 10h
		jmp	loc_3D835

loc_3D12C:
		cmp	byte ptr _levelmap[esi - 1], 0
		jl	short loc_3D137
		call	CreateExplosion
		ret

loc_3D137:
		mov	word ptr _levelmap[esi - 2], 2

loc_3D13D:
		;call	SND_bbb
		mov	dx, 0EDEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], MAP_SNIK_SNAK
		jmp	loc_3D835

loc_3D16A:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 0
		jl	short loc_3D175
		call	CreateExplosion
		ret

loc_3D175:
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 2

loc_3D17B:
		;call	SND_bbb
		mov	dx, 0EEEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], MAP_DISK_YELLOW
		jmp	loc_3D835


loc_3D1A8:				; CODE XREF: xxx_HandleMurphy+488j
		cmp	byte ptr _levelmap[esi + 3], 0
		jl	short loc_3D1B3
		call	CreateExplosion
		ret

loc_3D1B3:
		mov	word ptr _levelmap[esi + 2], 2

loc_3D1B9:
		;call	SND_bbb
		mov	dx, 0EFEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], 13h
		jmp	loc_3D835

loc_3D1E6:
		;call	SND_ppp
		cmp	word ptr [word_403BB], 0
		jz	short loc_3D1F5
		mov	dx, 0F0Eh
		jmp	short loc_3D1F8

loc_3D1F5:
		mov	dx, 0F1Eh

loc_3D1F8:
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 9
		mov	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		sub	esi, 78h	; 'x'
		jmp	loc_3D835

loc_3D21E:
		;call	SND_ppp
		mov	dx, 0F2Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi - 1], 0Ah
		mov	byte ptr _levelmap[esi - 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		sub	esi, 2
		jmp	loc_3D835

loc_3D24A:
		;call	SND_ppp
		cmp	word ptr [word_403BB], 0
		jz	short loc_3D259
		mov	dx, 0F3Eh
		jmp	short loc_3D25C

loc_3D259:
		mov	dx, 0F4Eh

loc_3D25C:
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], MAP_PORT_RIGHT_TO_LEFT
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		add	esi, 78h	; 'x'
		jmp	loc_3D835

loc_3D282:
		;call	SND_ppp
		mov	dx, 0F5Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 3], 0Ch
		mov	byte ptr _levelmap[esi + 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		add	esi, 2
		jmp	loc_3D835

loc_3D2AE:
		;call	SND_ppp
		mov	dx, 0F6Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], MAP_DISK_RED
		mov	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 0FFh
		jmp	loc_3D835

loc_3D2E0:
		;call	SND_ppp
		mov	dx, 0F7Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], 15h
		mov	byte ptr _levelmap[esi - 1], 0FFh
		jmp	loc_3D835

loc_3D312:
		;call	SND_ppp
		mov	dx, 0F8Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], 16h
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 0FFh
		jmp	loc_3D835

loc_3D344:
		;call	SND_ppp
		mov	dx, 0F9Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], 17h
		mov	byte ptr _levelmap[esi + 3], 0FFh
		jmp	loc_3D835

loc_3D376:
		cmp	byte ptr [InfotronsLeftToCollect],	0
		jnz	short locret_3D3BA
		;call	SND_ccc
		push	esi
		cmp	byte ptr [IsDemoRunning], 0
		jnz	short loc_3D397
		mov	byte ptr [byte_403AB], 1
		mov	byte ptr [byte_403AA], 0
		;call	sub_3BF39
		jmp	short loc_3D39C

loc_3D397:
		mov	byte ptr [byte_403AA], 0

loc_3D39C:
		mov	word ptr [TimeoutAfterWhichLeaveTheLevel], 40h
		pop	esi
		mov	dx, 0E5Eh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 0
		mov	byte ptr _levelmap[esi + 1], 0Dh
		jmp	loc_3D835

locret_3D3BA:
		ret

loc_3D3BB:
		mov	ax, word ptr _levelmap[esi - 4]
		cmp	ax, 0
		jz	short loc_3D3C5
		ret

loc_3D3C5:
		mov	byte ptr _levelmap[esi - 3], 1
		mov	dx, 0FAEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 8
		mov	byte ptr _levelmap[esi + 1], 0Eh
		jmp	loc_3D835

loc_3D3F4:
		mov	ax, word ptr _levelmap[esi + 4]
		cmp	ax, 0
		jz	short loc_3D3FE
		ret

loc_3D3FE:
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH + 2]
		cmp	ax, 0
		jnz	short loc_3D408
		ret

loc_3D408:
		mov	byte ptr _levelmap[esi + 5], 1
		mov	dx, 0FBEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 8
		mov	byte ptr _levelmap[esi + 1], 0Fh
		jmp	loc_3D835

loc_3D437:
		cmp	byte ptr [byte_40C5B], 0
		jz	short loc_3D458
		mov	word ptr [word_403BD], 0Ah
		ret

loc_3D458:
		jmp	loc_3D510

loc_3D46E:
		cmp	byte ptr [byte_40C5B], 0
		jz	short loc_3D48F
		mov	word ptr [word_403BD], 0Ah
		ret

loc_3D48F:
		jmp	loc_3D510

loc_3D4A4:
		cmp	byte ptr [byte_40C5B], 0
		jz	short loc_3D4C5
		mov	word ptr [word_403BD], 0Ah
		ret

loc_3D4C5:
		jmp	short loc_3D510

loc_3D4DA:
		cmp	byte ptr [byte_40C5B], 0
		jz	short loc_3D4FB
		mov	word ptr [word_403BD], 0Ah
		ret

loc_3D4FB:
		jmp	short loc_3D510

loc_3D510:
		mov	byte ptr [byte_40C5A], 7
		mov	byte ptr [byte_40C5B], 1
		push	esi
		mov	esi, 0
		mov	ecx, MAP_WIDTH * MAP_HEIGHT

loc_3D521:
		cmp	word ptr _levelmap[esi], MAP_DISK_YELLOW
		jnz	short loc_3D52F
		push	ecx
		push	esi
		call	CreateExplosion
		pop	esi
		pop	ecx

loc_3D52F:
		add	esi, 2
		loop	loc_3D521
		pop	esi
		ret

loc_3D536:
		cmp	word ptr _levelmap[esi - (4 * MAP_WIDTH)], 0
		jz	short loc_3D53E
		ret

loc_3D53E:
		mov	dx, 0FCEh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 1
		mov	byte ptr _levelmap[esi + 1], MAP_ELECTRON
		mov	byte ptr _levelmap[esi - (4 * MAP_WIDTH - 1)], 3
		jmp	loc_3D835

loc_3D55A:
		cmp	word ptr _levelmap[esi - 4], 0
		jz	short loc_3D562
		ret

loc_3D562:
		mov	dx, 0FDEh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 1
		mov	byte ptr _levelmap[esi + 1], 19h
		mov	byte ptr _levelmap[esi - 3], 3
		jmp	loc_3D835

loc_3D57E:
		cmp	word ptr _levelmap[esi + (4 * MAP_WIDTH)], 0
		jz	short loc_3D586
		ret

loc_3D586:
		mov	dx, 0FEEh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 1
		mov	byte ptr _levelmap[esi + 1], 1Ah
		mov	byte ptr _levelmap[esi + MAP_WIDTH * 4 + 1], 3
		jmp	loc_3D835

loc_3D5A2:
		cmp	word ptr _levelmap[esi + 4], 0
		jz	short loc_3D5AA
		ret

loc_3D5AA:
		mov	dx, 0FFEh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 1
		mov	byte ptr _levelmap[esi + 1], MAP_RAM_CHIP_RIGHT
		mov	byte ptr _levelmap[esi + 5], 3
		jmp	loc_3D835

loc_3D5C6:
		cmp	word ptr [word_403BB], 0
		jz	short loc_3D5D2
		mov	dx, 100Eh
		jmp	short loc_3D5D5

loc_3D5D2:
		mov	dx, 101Eh

loc_3D5D5:
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 1Ch
		mov	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 3
		jmp	loc_3D835

loc_3D5EE:
		mov	dx, 102Eh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi - 1], 1Dh
		mov	byte ptr _levelmap[esi - 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		sub	esi, 2
		jmp	loc_3D835

loc_3D617:
		cmp	word ptr [word_403BB], 0
		jz	short loc_3D623
		mov	dx, 103Eh
		jmp	short loc_3D626

loc_3D623:
		mov	dx, 104Eh

loc_3D626:
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 1Eh
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 3
		jmp	loc_3D835


loc_3D63F:
		mov	dx, 105Eh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 3], 1Fh
		mov	byte ptr _levelmap[esi + 2], 3
		mov	byte ptr _levelmap[esi + 1], 3
		mov	byte ptr _levelmap[esi], 0
		add	esi, 2
		jmp	loc_3D835


loc_3D668:
		mov	dx, 106Eh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 20h
		mov	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 3
		jmp	loc_3D835

loc_3D684:
		mov	dx, 107Eh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 21h
		mov	byte ptr _levelmap[esi - 1], 3
		jmp	loc_3D835

loc_3D6A0:
		mov	dx, 108Eh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 22h
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 3
		jmp	loc_3D835

loc_3D6BC:
		mov	dx, 109Eh
		mov	word ptr [word_403DE], 0
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 23h
		mov	byte ptr _levelmap[esi + 3], 3
		jmp	loc_3D835

loc_3D6D8:
		mov	ax, word ptr _levelmap[esi - (4 * MAP_WIDTH)]
		cmp	ax, 0
		jz	short loc_3D6E2
		ret

loc_3D6E2:
		mov	byte ptr _levelmap[esi - (4 * MAP_WIDTH - 1)], MAP_DISK_YELLOW
		mov	dx, 10AEh
		mov	word ptr [word_403DE], 8
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 24h
		jmp	loc_3D835

loc_3D711:
		mov	ax, word ptr _levelmap[esi - 4]
		cmp	ax, 0
		jz	short loc_3D71B
		ret

loc_3D71B:
		mov	byte ptr _levelmap[esi - 3], MAP_DISK_YELLOW
		mov	dx, 10BEh
		mov	word ptr [word_403DE], 8
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 25h
		jmp	loc_3D835

loc_3D74A:
		mov	ax, word ptr _levelmap[esi + (4 * MAP_WIDTH)]
		cmp	ax, 0
		jz	short loc_3D754
		ret

loc_3D754:
		mov	byte ptr _levelmap[esi + MAP_WIDTH * 4 + 1], MAP_DISK_YELLOW
		mov	dx, 10CEh
		mov	word ptr [word_403DE], 8
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 27h
		jmp	loc_3D835

loc_3D783:
		mov	ax, word ptr _levelmap[esi + 4]
		cmp	ax, 0
		jz	short loc_3D78D
		ret

loc_3D78D:
		mov	byte ptr _levelmap[esi + 5], MAP_DISK_YELLOW
		mov	dx, 10DEh
		mov	word ptr [word_403DE], 8
		mov	word ptr [word_403C9], 0
		mov	byte ptr _levelmap[esi + 1], 26h
		jmp	loc_3D835

loc_3D7BB:
		mov	ax, word ptr _levelmap[esi - 4]
		cmp	ax, 0
		jz	short loc_3D7C5
		ret

loc_3D7C5:
		mov	byte ptr _levelmap[esi - 3], 8
		mov	dx, 10EEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 8
		mov	byte ptr _levelmap[esi + 1], 28h
		jmp	short loc_3D835

loc_3D7F3:
		mov	ax, word ptr _levelmap[esi + 4]
		cmp	ax, 0
		jz	short loc_3D7FD
		ret

loc_3D7FD:
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH + 2]
		cmp	ax, 0
		jnz	short loc_3D807
		ret

loc_3D807:
		mov	byte ptr _levelmap[esi + 5], 1
		mov	dx, 10FEh
		mov	word ptr [word_403C9], 0
		mov	word ptr [word_403DE], 8
		mov	byte ptr _levelmap[esi + 1], 29h
		jmp	short loc_3D835

loc_3D835:
		jmp	short loc_3D84B

loc_3D84B:
		mov	word ptr [word_403BD], 0
		mov	ax, word ptr [word_403DE]
		cmp	ax, 0
		jz	short loc_3D8AF
		dec	ax
		mov	word ptr [word_403DE], ax
		jnz	short loc_3D862
		;call	SND_mmm

loc_3D862:
		mov	bl, byte ptr _levelmap[esi + 1]
		cmp	bl, 0Eh
		jnz	short loc_3D86E
		jmp	loc_3DB9E

loc_3D86E:
		cmp	bl, 0Fh
		jnz	short loc_3D876
		jmp	loc_3DBDC

loc_3D876:
		cmp	bl, 28h
		jnz	short loc_3D87E
		jmp	loc_3DC1A

loc_3D87E:
		cmp	bl, 29h
		jnz	short loc_3D886
		jmp	loc_3DC58


loc_3D886:
		cmp	bl, 24h
		jnz	short loc_3D88E
		jmp	loc_3DC96

loc_3D88E:
		cmp	bl, 25h
		jnz	short loc_3D896
		jmp	loc_3DCD4

loc_3D896:
		cmp	bl, 27h
		jnz	short loc_3D89E
		jmp	loc_3DD12

loc_3D89E:
		cmp	bl, 26h
		jnz	short loc_3D8A6
		jmp	loc_3DD50

loc_3D8A6:
		cmp	bl, 2Ah
		jnz	short locret_3D8AE
		jmp	loc_3DD8E

locret_3D8AE:
		ret

loc_3D8AF:
		mov	ax, word ptr [word_403EA]
		add	word ptr [PlayerPosition_PixelsX], ax
		mov	ax, word ptr [word_403EC]
		add	word ptr [PlayerPosition_PixelsY], ax
		;mov	di, [si+60CBh]
		;add	di, [word_403E0]
		;mov		esi, [dword_403E8]
		;mov	ax, [si]
		;add		esi, 2
		;mov		[dword_403E8], esi
		;mov	si, ax
		;mov		bx, [word_403E4]
		;mov		dx, [word_403E6]
		;push	ds
		;mov	ax, es
		;mov	ds, ax

;loc_3D8E3:
		;mov	cx, bx
		;rep movsb
		;add	si, 7Ah	; 'z'
		;sub	si, bx
		;add	di, 7Ah	; 'z'
		;sub	di, bx
		;cmp	si, 4D34h
		;jb	short loc_3D8FB
		;sub	si, 4D0Ch

;loc_3D8FB:
		;dec	dx
		;jnz	short loc_3D8E3
		;mov	ax, ds
		;pop	ds
		;cmp	[word_403C9], 0
		;jz	short loc_3D93E
		;sub	di, 7A0h
		;add	di, [word_403E2]
		;mov	esi, [dword_403E8]
		;add	esi, 10h
		;mov	esi, [esi]
		;mov	dx, [word_403E6]
		;push	ds
		;mov	ds, ax

;loc_3D920:
		;mov	cx, bx
		;rep movsb
		;add	si, 7Ah
		;sub	si, bx
		;add	di, 7Ah
		;sub	di, bx
		;cmp	si, 4D34h
		;jb	short loc_3D938
		;sub	si, 4D0Ch

;loc_3D938:
		;dec	dx
		;jnz	short loc_3D920
		;pop	ds
		;jmp	short loc_3D948

;loc_3D93E:
;		mov	ax, [word_403E0]
;		add	ax, [word_403E2]
;		mov	[word_403E0], ax

;loc_3D948:
;		mov	esi, [dword_403E8]
;		cmp	word ptr [esi], 0FFFFh
;		jz	short loc_3D955
;		pop	di
;		pop	si
;		ret

loc_3D955:
		mov	ax, word ptr [word_403EA]
		shr	ax, 1
		mov	bx, word ptr [word_403EC]
		shr	bx, 1
		add	word ptr [PlayerPosition_MapX], ax
		add	word ptr [PlayerPosition_MapY], bx
		mov	bl, byte ptr _levelmap[esi + 1]
		mov	byte ptr _levelmap[esi + 1], 0
		cmp	bl, 1
		jnz	short loc_3D97B
		jmp	loc_3DAE8

loc_3D97B:
		cmp	bl, 2
		jnz	short loc_3D983
		jmp	loc_3DB06

loc_3D983:
		cmp	bl, 3
		jnz	short loc_3D98B
		jmp	loc_3DB24

loc_3D98B:
		cmp	bl, 4
		jnz	short loc_3D993
		jmp	loc_3DDE4

loc_3D993:
		cmp	bl, 5
		jnz	short loc_3D99B
		jmp	loc_3DAE8

loc_3D99B:
		cmp	bl, 6
		jnz	short loc_3D9A3
		jmp	loc_3DB06

loc_3D9A3:
		cmp	bl, 7
		jnz	short loc_3D9AB
		jmp	loc_3DB24

loc_3D9AB:
		cmp	bl, 8
		jnz	short loc_3D9B3
		jmp	loc_3DDE4

loc_3D9B3:
		cmp	bl, 9
		jnz	short loc_3D9BB
		jmp	loc_3DADA

loc_3D9BB:
		cmp	bl, 0Ah
		jnz	short loc_3D9C3
		jmp	loc_3DAF8

loc_3D9C3:
		cmp	bl, MAP_PORT_RIGHT_TO_LEFT
		jnz	short loc_3D9CB
		jmp	@@InfotronCollected

loc_3D9CB:
		cmp	bl, 0Ch
		jnz	short loc_3D9D3
		jmp	loc_3DDD6

loc_3D9D3:
		cmp	bl, 0Eh
		jnz	short loc_3D9DB
		jmp	loc_3DB38

loc_3D9DB:
		cmp	bl, 0Fh
		jnz	short loc_3D9E3
		jmp	loc_3DB5B

loc_3D9E3:
		cmp	bl, 10h
		jnz	short loc_3D9EB
		jmp	loc_3DE02

loc_3D9EB:
		cmp	bl, MAP_SNIK_SNAK
		jnz	short loc_3D9F3
		jmp	loc_3DE1E

loc_3D9F3:
		cmp	bl, 13h
		jnz	short loc_3D9FB
		jmp	loc_3DE56

loc_3D9FB:
		cmp	bl, MAP_DISK_YELLOW
		jnz	short loc_3DA03
		jmp	loc_3DE3A

loc_3DA03:
		cmp	bl, 13h
		jnz	short loc_3DA0B
		jmp	loc_3DE56

loc_3DA0B:
		cmp	bl, MAP_DISK_RED
		jnz	short loc_3DA13
		jmp	loc_3DDF4

loc_3DA13:
		cmp	bl, 15h
		jnz	short loc_3DA1B
		jmp	loc_3DE10

loc_3DA1B:
		cmp	bl, 17h
		jnz	short loc_3DA23
		jmp	loc_3DE48

loc_3DA23:
		cmp	bl, 16h
		jnz	short loc_3DA2B
		jmp	loc_3DE2C

loc_3DA2B:
		cmp	bl, 17h
		jnz	short loc_3DA33
		jmp	loc_3DE48

loc_3DA33:
		cmp	bl, 0Dh
		jnz	short loc_3DA3B
		jmp	loc_3DB97

loc_3DA3B:
		cmp	bl, MAP_ELECTRON
		jnz	short loc_3DA43
		jmp	loc_3DE64

loc_3DA43:
		cmp	bl, 19h
		jnz	short loc_3DA4B
		jmp	loc_3DE92

loc_3DA4B:
		cmp	bl, 1Ah
		jnz	short loc_3DA53
		jmp	loc_3DEBF

loc_3DA53:
		cmp	bl, MAP_RAM_CHIP_RIGHT
		jnz	short loc_3DA5B
		jmp	loc_3DEED

loc_3DA5B:
		cmp	bl, 1Ch
		jnz	short loc_3DA63
		jmp	loc_3DF1A

loc_3DA63:
		cmp	bl, 1Dh
		jnz	short loc_3DA6B
		jmp	loc_3DF38

loc_3DA6B:
		cmp	bl, 1Eh
		jnz	short loc_3DA73
		jmp	loc_3DF53

loc_3DA73:
		cmp	bl, 1Fh
		jnz	short loc_3DA7B
		jmp	loc_3DF71

loc_3DA7B:
		cmp	bl, 20h
		jnz	short loc_3DA83
		jmp	loc_3DF8C

loc_3DA83:
		cmp	bl, 21h
		jnz	short loc_3DA8B
		jmp	loc_3DFA1

loc_3DA8B:
		cmp	bl, 22h
		jnz	short loc_3DA93
		jmp	loc_3DFB6

loc_3DA93:
		cmp	bl, 23h
		jnz	short loc_3DA9B
		jmp	loc_3DFCB

loc_3DA9B:
		cmp	bl, 24h
		jnz	short loc_3DAA3
		jmp	loc_3DFE0

loc_3DAA3:
		cmp	bl, 25h
		jnz	short loc_3DAAB
		jmp	loc_3DFFD

loc_3DAAB:
		cmp	bl, 27h
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
		mov	word ptr [word_40C64], 1
		ret


loc_3DADA:				; CODE XREF: xxx_HandleMurphy+F58j
		cmp	byte ptr [InfotronsLeftToCollect],	0
		jbe	short loc_3DAE5
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DAE5:				; CODE XREF: xxx_HandleMurphy+107Fj
		;call	sub_3EC01

loc_3DAE8:				; CODE XREF: xxx_HandleMurphy+F18j
					; xxx_HandleMurphy+F38j
		mov	word ptr _levelmap[esi], 3
		add	esi, 78h	; 'x'
		call	sub_38191
		sub	esi, 78h	; 'x'
		ret


loc_3DAF8:				; CODE XREF: xxx_HandleMurphy+F60j
		cmp	byte ptr [InfotronsLeftToCollect],	0
		jbe	short loc_3DB03
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DB03:				; CODE XREF: xxx_HandleMurphy+109Dj
		;call	sub_3EC01

loc_3DB06:				; CODE XREF: xxx_HandleMurphy+F20j
					; xxx_HandleMurphy+F40j
		mov	word ptr _levelmap[esi], 3
		add	esi, 2
		call	sub_38191
		sub	esi, 2
		ret


; __linkproc__ InfotronCollected
@@InfotronCollected:			; CODE XREF: xxx_HandleMurphy+F68j
		cmp	byte ptr [InfotronsLeftToCollect],	0
		jbe	short loc_3DB21
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DB21:				; CODE XREF: xxx_HandleMurphy+10BBj
		;call	sub_3EC01

loc_3DB24:				; CODE XREF: xxx_HandleMurphy+F28j
					; xxx_HandleMurphy+F48j
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short loc_3DB31
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0

loc_3DB31:				; CODE XREF: xxx_HandleMurphy+10C9j
		mov	word ptr _levelmap[esi], 3
		ret


loc_3DB38:				; CODE XREF: xxx_HandleMurphy+F78j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DB45
		mov	word ptr _levelmap[esi], 0

loc_3DB45:				; CODE XREF: xxx_HandleMurphy+10DDj
		mov	word ptr _levelmap[esi - 2], 3
		mov	word ptr _levelmap[esi - 4], 1
		sub	esi, 4
		call	sub_3DB7E
		add	esi, 2
		ret


loc_3DB5B:				; CODE XREF: xxx_HandleMurphy+F80j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DB68
		mov	word ptr _levelmap[esi], 0

loc_3DB68:				; CODE XREF: xxx_HandleMurphy+1100j
		mov	word ptr _levelmap[esi + 2], 3
		mov	word ptr _levelmap[esi + 4], 1
		add	esi, 4
		call	sub_3DB7E
		sub	esi, 2
		ret
xxx_HandleMurphy endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3DB7E	proc near

		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SNIK_SNAK
		jz	short loc_3DB8D
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 00BBh
		jz	short loc_3DB8D
		ret

loc_3DB8D:
		add	esi, 78h
		call	CreateExplosion
		sub	esi, 78h
		ret

sub_3DB7E	endp

loc_3DB97:
		mov	word ptr [word_40C64], 1
		ret

loc_3DB9E:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 2
		jnz	short loc_3DBAF
		cmp	word ptr _levelmap[esi - 2], 1
		jnz	short loc_3DBAF
		ret

loc_3DBAF:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi - 2], 1
		cmp	byte ptr _levelmap[esi - 4], MAP_RED_LAMP
		jz	short loc_3DBC8
		mov	word ptr _levelmap[esi - 4], 0

loc_3DBC8:
		ret

loc_3DBDC:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 4
		jnz	short loc_3DBED
		cmp	word ptr _levelmap[esi + 2], 1
		jnz	short loc_3DBED
		ret

loc_3DBED:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi + 2], 1
		cmp	byte ptr _levelmap[esi + 4], MAP_RED_LAMP
		jz	short loc_3DC06
		mov	word ptr _levelmap[esi + 4], 0

loc_3DC06:
		ret

loc_3DC1A:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 2
		jnz	short loc_3DC2B
		cmp	word ptr _levelmap[esi - 2], 8
		jnz	short loc_3DC2B
		ret

loc_3DC2B:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi - 2], 8
		cmp	byte ptr _levelmap[esi - 4], MAP_RED_LAMP
		jz	short loc_3DC44
		mov	word ptr _levelmap[esi - 4], 0

loc_3DC44:
		ret

loc_3DC58:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 4
		jnz	short loc_3DC69
		cmp	word ptr _levelmap[esi + 2], 8
		jnz	short loc_3DC69
		ret

loc_3DC69:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi + 2], 8
		cmp	byte ptr _levelmap[esi + 4], MAP_RED_LAMP
		jz	short loc_3DC82
		mov	word ptr _levelmap[esi + 4], 0

loc_3DC82:
		ret

loc_3DC96:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 1
		jnz	short loc_3DCA7
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_DISK_YELLOW
		jnz	short loc_3DCA7
		ret

loc_3DCA7:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_DISK_YELLOW
		cmp	byte ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_RED_LAMP
		jz	short loc_3DCC0
		mov	word ptr _levelmap[esi - (4 * MAP_WIDTH)], 0

loc_3DCC0:
		ret

loc_3DCD4:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 2
		jnz	short loc_3DCE5
		cmp	word ptr _levelmap[esi - 2], MAP_DISK_YELLOW
		jnz	short loc_3DCE5
		ret

loc_3DCE5:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi - 2], MAP_DISK_YELLOW
		cmp	byte ptr _levelmap[esi - 4], MAP_RED_LAMP
		jz	short loc_3DCFE
		mov	word ptr _levelmap[esi - 4], 0

loc_3DCFE:
		ret

loc_3DD12:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 3
		jnz	short loc_3DD23
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_YELLOW
		jnz	short loc_3DD23
		ret

loc_3DD23:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_YELLOW
		cmp	byte ptr _levelmap[esi + (4 * MAP_WIDTH)], MAP_RED_LAMP
		jz	short loc_3DD3C
		mov	word ptr _levelmap[esi + (4 * MAP_WIDTH)], 0

loc_3DD3C:
		ret

loc_3DD50:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 4
		jnz	short loc_3DD61
		cmp	word ptr _levelmap[esi + 2], MAP_DISK_YELLOW
		jnz	short loc_3DD61
		ret

loc_3DD61:
		mov	word ptr _levelmap[esi], 3
		mov	word ptr _levelmap[esi + 2], MAP_DISK_YELLOW
		cmp	byte ptr _levelmap[esi + 4], MAP_RED_LAMP
		jz	short loc_3DD7A
		mov	word ptr _levelmap[esi + 4], 0

loc_3DD7A:
		ret

loc_3DD8E:
		mov	bl, byte ptr [_joystickbuttons]
		cmp	bl, 9
		jnz	short loc_3DDB7
		cmp	word ptr [word_403DE], 20h
		jg	short locret_3DDB6
		mov	byte ptr [NumRedDisks], 1

locret_3DDB6:
		ret

loc_3DDB7:
		mov		word ptr _levelmap[esi], 3
		mov		byte ptr [NumRedDisks], 0
		ret

loc_3DDD6:
		cmp	byte ptr [InfotronsLeftToCollect],	0
		jbe	short loc_3DDE1
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DDE1:
		;call	sub_3EC01

loc_3DDE4:
		sub	esi, 2
		call	sub_38191
		add	esi, 2
		mov	word ptr _levelmap[esi], 3
		ret


loc_3DDF4:				; CODE XREF: xxx_HandleMurphy+FB0j
		cmp	byte ptr [InfotronsLeftToCollect],	0
		jbe	short loc_3DDFF
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DDFF:				; CODE XREF: xxx_HandleMurphy+1399j
		;call	sub_3EC01

loc_3DE02:				; CODE XREF: xxx_HandleMurphy+F88j
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short locret_3DE0F
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0

locret_3DE0F:				; CODE XREF: xxx_HandleMurphy+13A7j
		ret


loc_3DE10:				; CODE XREF: xxx_HandleMurphy+FB8j
		cmp	byte ptr [InfotronsLeftToCollect], 0
		jbe	short loc_3DE1B
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DE1B:				; CODE XREF: xxx_HandleMurphy+13B5j
		;call	sub_3EC01

loc_3DE1E:				; CODE XREF: xxx_HandleMurphy+F90j
		cmp	byte ptr _levelmap[esi - 2], MAP_RED_LAMP
		jz	short locret_3DE2B
		mov	word ptr _levelmap[esi - 2], 0

locret_3DE2B:				; CODE XREF: xxx_HandleMurphy+13C3j
		ret


loc_3DE2C:				; CODE XREF: xxx_HandleMurphy+FC8j
		cmp	byte ptr [InfotronsLeftToCollect], 0
		jbe	short loc_3DE37
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DE37:				; CODE XREF: xxx_HandleMurphy+13D1j
		;call	sub_3EC01

loc_3DE3A:				; CODE XREF: xxx_HandleMurphy+FA0j
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short locret_3DE47
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0

locret_3DE47:				; CODE XREF: xxx_HandleMurphy+13DFj
		ret


loc_3DE48:
		cmp	byte ptr [InfotronsLeftToCollect],0
		jbe	short loc_3DE53
		dec	byte ptr [InfotronsLeftToCollect]

loc_3DE53:
		;call	sub_3EC01

loc_3DE56:
		cmp	byte ptr _levelmap[esi + 2], MAP_RED_LAMP
		jz	short locret_3DE63
		mov	word ptr _levelmap[esi + 2], 0

locret_3DE63:				; CODE XREF: xxx_HandleMurphy+13FBj
		ret


loc_3DE64:				; CODE XREF: xxx_HandleMurphy+FE0j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DE71
		mov	word ptr _levelmap[esi], 0

loc_3DE71:				; CODE XREF: xxx_HandleMurphy+1409j
		mov	word ptr _levelmap[esi - (4 * MAP_WIDTH)], 3
		mov	word ptr [word_403C9], 0
		sub	esi, 0F0h
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 1
		jnz	short locret_3DE91
		add	esi, 78h
		call	sub_3E190
		sub	esi, 78h

locret_3DE91:
		ret

loc_3DE92:
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DE9F
		mov	word ptr _levelmap[esi], 0

loc_3DE9F:
		mov	word ptr _levelmap[esi - 4], 3
		mov	word ptr [word_403C9], 0
		sub	esi, 4
		cmp	byte ptr _levelmap[esi + 3], 1
		jnz	short locret_3DEBE
		add	esi, 2
		call	sub_3E190
		sub	esi, 2

locret_3DEBE:
		ret

loc_3DEBF:
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DECC
		mov	word ptr _levelmap[esi], 0

loc_3DECC:
		mov	word ptr _levelmap[esi + (4 * MAP_WIDTH)], 3
		mov	word ptr [word_403C9], 0
		add	esi, 0F0h
		cmp	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 1
		jnz	short locret_3DEEC
		sub	esi, 78h
		call	sub_3E190
		add	esi, 78h

locret_3DEEC:
		ret

loc_3DEED:
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DEFA
		mov	word ptr _levelmap[esi], 0

loc_3DEFA:
		mov	word ptr _levelmap[esi + 4], 3
		mov	word ptr [word_403C9], 0
		add	esi, 4
		cmp	byte ptr _levelmap[esi - 1], 1
		jnz	short locret_3DF19
		sub esi, 2
		call	sub_3E190
		add	esi, 2

locret_3DF19:				; CODE XREF: xxx_HandleMurphy+14AEj
		ret


loc_3DF1A:				; CODE XREF: xxx_HandleMurphy+1000j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DF27
		mov	word ptr _levelmap[esi], 0

loc_3DF27:				; CODE XREF: xxx_HandleMurphy+14BFj
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		sub	esi, 78h
		ret


loc_3DF38:				; CODE XREF: xxx_HandleMurphy+1008j
		cmp	byte ptr _levelmap[esi + 2], MAP_RED_LAMP
		jz	short loc_3DF45
		mov	word ptr _levelmap[esi + 2], 0

loc_3DF45:				; CODE XREF: xxx_HandleMurphy+14DDj
		mov	word ptr _levelmap[esi], 3
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		ret


loc_3DF53:				; CODE XREF: xxx_HandleMurphy+1010j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DF60
		mov	word ptr _levelmap[esi], 0

loc_3DF60:				; CODE XREF: xxx_HandleMurphy+14F8j
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		add	esi, 78h	; 'x'
		ret


loc_3DF71:				; CODE XREF: xxx_HandleMurphy+1018j
		cmp	byte ptr _levelmap[esi - 2], MAP_RED_LAMP
		jz	short loc_3DF7E
		mov	word ptr _levelmap[esi - 2], 0

loc_3DF7E:				; CODE XREF: xxx_HandleMurphy+1516j
		mov	word ptr _levelmap[esi], 3
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		ret


loc_3DF8C:				; CODE XREF: xxx_HandleMurphy+1020j
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short loc_3DF99
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0

loc_3DF99:				; CODE XREF: xxx_HandleMurphy+1531j
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		ret


loc_3DFA1:				; CODE XREF: xxx_HandleMurphy+1028j
		cmp	byte ptr _levelmap[esi - 2], MAP_RED_LAMP
		jz	short loc_3DFAE
		mov	word ptr _levelmap[esi - 2], 0

loc_3DFAE:				; CODE XREF: xxx_HandleMurphy+1546j
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		ret


loc_3DFB6:				; CODE XREF: xxx_HandleMurphy+1030j
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short loc_3DFC3
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0

loc_3DFC3:
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		ret


loc_3DFCB:
		cmp	byte ptr _levelmap[esi + 2], MAP_RED_LAMP
		jz	short loc_3DFD8
		mov	word ptr _levelmap[esi + 2], 0

loc_3DFD8:
		inc	byte ptr [byte_40C4C]
		;call	sub_3EC91
		ret

loc_3DFE0:
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3DFED
		mov	word ptr _levelmap[esi], 0

loc_3DFED:
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		mov	word ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_DISK_YELLOW
		sub	esi, 78h	; 'x'
		ret

loc_3DFFD:
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3E00A
		mov	word ptr _levelmap[esi], 0

loc_3E00A:				; CODE XREF: xxx_HandleMurphy+15A2j
		mov	word ptr _levelmap[esi - 2], 3
		mov	word ptr _levelmap[esi - 4], MAP_DISK_YELLOW
		sub	esi, 2
		ret


loc_3E01A:				; CODE XREF: xxx_HandleMurphy+1050j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3E027
		mov	word ptr _levelmap[esi], 0

loc_3E027:				; CODE XREF: xxx_HandleMurphy+15BFj
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		mov	word ptr _levelmap[esi + (4 * MAP_WIDTH)], MAP_DISK_YELLOW
		add	esi, 78h	; 'x'
		ret


loc_3E037:				; CODE XREF: xxx_HandleMurphy+1058j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3E044
		mov	word ptr _levelmap[esi], 0

loc_3E044:				; CODE XREF: xxx_HandleMurphy+15DCj
		mov	word ptr _levelmap[esi + 2], 3
		mov	word ptr _levelmap[esi + 4], MAP_DISK_YELLOW
		add	esi, 2
		ret


loc_3E054:				; CODE XREF: xxx_HandleMurphy+1060j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3E061
		mov	word ptr _levelmap[esi], 0

loc_3E061:				; CODE XREF: xxx_HandleMurphy+15F9j
		mov	word ptr _levelmap[esi - 2], 3
		mov	word ptr _levelmap[esi - 4], 8
		sub	esi, 2
		ret


loc_3E071:				; CODE XREF: xxx_HandleMurphy+1068j
		cmp	byte ptr _levelmap[esi], MAP_RED_LAMP
		jz	short loc_3E07E
		mov	word ptr _levelmap[esi], 0

loc_3E07E:				; CODE XREF: xxx_HandleMurphy+1616j
		mov	word ptr _levelmap[esi + 2], 3
		mov	word ptr _levelmap[esi + 4], 8
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH + 4], 0
		jnz	short loc_3E09B
		mov	byte ptr _levelmap[esi + 5], 20h ; ' '
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 5], 8

loc_3E09B:				; CODE XREF: xxx_HandleMurphy+162Fj
		add	esi, 2
		ret


loc_3E09F:				; CODE XREF: xxx_HandleMurphy+1070j
		mov	word ptr _levelmap[esi], 3
		mov	byte ptr [NumRedDisks], 2
		dec	byte ptr [byte_40C4C]
		;call	sub_3EC91
		;call	SND_mmm
		ret

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_3E0D7	proc near

		cmp	ax, 0FFFFh
		jz	short loc_3E14E
		cmp	ax, 0AAAAh
		jz	short loc_3E14E
		cmp	ah, 0
		jz	short loc_3E14E
		cmp	al, 1
		jz	short loc_3E107
		cmp	al, MAP_RED_LAMP
		jz	short loc_3E150
		cmp	al, 8
		jz	short loc_3E14E
		cmp	al, 9
		jz	short loc_3E14E
		cmp	al, 0Ah
		jz	short loc_3E14E
		cmp	al, MAP_PORT_RIGHT_TO_LEFT
		jz	short loc_3E14E
		cmp	al, 0Ch
		jz	short loc_3E14E
		call	CreateExplosion
		stc
		ret


loc_3E107:				; CODE XREF: sub_3E0D7+11j
		cmp	bl, 2
		jz	short loc_3E116
		cmp	bl, 4
		jz	short loc_3E132
		call	CreateExplosion
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
		call	CreateExplosion

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
		call	CreateExplosion

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
		call	CreateExplosion
		stc
		ret


loc_3E15F:				; CODE XREF: sub_3E0D7+81j
		mov	word ptr _levelmap[esi], 0
		clc
		ret
sub_3E0D7	endp


		cmp	bl, 2
		jz	short loc_3E176
		cmp	bl, 4
		jz	short loc_3E183
		call	CreateExplosion
		stc
		ret


loc_3E176:				; CODE XREF: CODE_SEG:754Aj
		and	ah, 0F0h
		cmp	ah, 20h	; ' '
		jz	short loc_3E181
		call	CreateExplosion

loc_3E181:				; CODE XREF: CODE_SEG:755Cj
		stc
		ret


loc_3E183:				; CODE XREF: CODE_SEG:754Fj
		and	ah, 0F0h
		cmp	ah, 20h	; ' '
		jz	short loc_3E18E
		call	CreateExplosion

loc_3E18E:				; CODE XREF: CODE_SEG:7569j
		stc
		ret

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_3E190	proc near		; CODE XREF: xxx_HandleMurphy+142Bp
					; xxx_HandleMurphy+1458p ...
		mov	cl, byte ptr [byte_40327]
		xor	ch, ch
		cmp	cx, 0
		jz	short locret_3E1AB
		mov	edi, offset byte_40328

loc_3E19E:
		mov	bx, [edi]
		xchg	bl, bh
		cmp	bx, si
		jz	short loc_3E1AC
		add	edi, 6
		loop	loc_3E19E

locret_3E1AB:
		ret

loc_3E1AC:
		mov	al, [edi+2]
		mov	byte ptr [byte_4030C], al
		mov	al, [edi+3]
		mov	byte ptr [byte_40325], al
		mov	al, [edi+4]
		mov	byte ptr [byte_403C7], al
		mov	ax, word ptr [word_4039C]
		xor	ax, word ptr [RandomSeed]
		mov	word ptr [word_4039C], ax
		ret
sub_3E190	endp



; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


handleSnikSnak	proc near		; DATA XREF: DATA_SEG:162Co
		cmp	byte ptr [byte_403C7], 1
		jz	short locret_3E1F1
		cmp	byte ptr _levelmap[esi], MAP_SNIK_SNAK
		jnz	short locret_3E1F1
		mov	bl, byte ptr _levelmap[esi + 1]
		movzx ebx, bl
		shl	ebx, 2
		mov	eax, SnikSnakHandlers[ebx]
		cmp	eax, 0FFFFh
		je	short locret_3E1F1
		call	eax

locret_3E1F1:
		ret
handleSnikSnak	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

SnikSnakHandler_1	proc near		
		mov	ax, word ptr [word_40C4D]
		and	ax, 3
		jz	short loc_3E200
		cmp	ax, 3
		jz	short loc_3E242
		ret

loc_3E200:
		shr	ebx, 2
		inc	ebx
		and	bl, 7
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E22A:
		cmp	ah, MAP_RAM_CHIP_RIGHT
		jz	short locret_3E241
		cmp	ah, 19h
		jz	short locret_3E241
		cmp	ah, MAP_ELECTRON
		jz	short locret_3E241
		cmp	ah, 1Ah
		jz	short locret_3E241
		call	CreateExplosion

locret_3E241:
		ret

loc_3E242:
		mov	bl, byte ptr _levelmap[esi + 1]
		cmp	bl, 0
		jz	short loc_3E25B
		cmp	bl, 2
		jz	short loc_3E27E
		cmp	bl, 4
		jz	short loc_3E2A1
		cmp	bl, 6
		jz	loc_3E2C7
		ret

loc_3E25B:
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E26E
		mov	ax, word ptr _levelmap[esi - 2 * MAP_WIDTH]
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
		jz	short loc_3E22A
		ret

loc_3E26E:
		mov	word ptr _levelmap[esi], 1000h + 0BBh
		sub	esi, 2 * MAP_WIDTH
		mov	word ptr _levelmap[esi], 1000h + MAP_SNIK_SNAK
		ret

loc_3E27E:
		cmp	word ptr _levelmap[esi - 2], 0
		jz	short loc_3E291
		mov	ax, word ptr _levelmap[esi - 2]
		cmp	byte ptr _levelmap[esi - 2], 3
		jz	loc_3E22A
		ret

loc_3E291:
		mov	word ptr _levelmap[esi], 2000h + 0BBh
		sub	esi, 2
		mov	word ptr _levelmap[esi], 1800h + MAP_SNIK_SNAK
		ret

loc_3E2A1:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E2B7
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jnz	short locret_3E2B6
		jmp	loc_3E22A

locret_3E2B6:
		ret

loc_3E2B7:
		mov	word ptr _levelmap[esi], 30BBh
		add	esi, 78h
		mov	word ptr _levelmap[esi], 2000h + MAP_SNIK_SNAK
		ret

loc_3E2C7:
		cmp	word ptr _levelmap[esi + 2], 0
		jz	short loc_3E2DD
		mov	ax, word ptr _levelmap[esi + 2]
		cmp	byte ptr _levelmap[esi + 2], 3
		jnz	short locret_3E2DC
		jmp	loc_3E22A

locret_3E2DC:
		ret

loc_3E2DD:
		mov	word ptr _levelmap[esi], 4000h + 0BBh
		add	esi, 2
		mov	word ptr _levelmap[esi], 2800h + MAP_SNIK_SNAK
		ret

SnikSnakHandler_1	endp


SnikSnakHandler_2 proc near
		mov	ax, word ptr [word_40C4D]
		and	ax, 3
		jz	short loc_3E2FB
		cmp	ax, 3
		jz	short loc_3E33F
		ret

loc_3E2FB:
		shr	ebx, 2
		inc	bl
		and	bl, 7
		or	bl, 8
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E327:
		cmp	ah, MAP_RAM_CHIP_RIGHT
		jz	short locret_3E33E
		cmp	ah, 19h
		jz	short locret_3E33E
		cmp	ah, MAP_ELECTRON
		jz	short locret_3E33E
		cmp	ah, 1Ah
		jz	short locret_3E33E
		call	CreateExplosion

locret_3E33E:
		ret

loc_3E33F:
		mov	bl, byte ptr _levelmap[esi + 1]
		cmp	bl, 8
		jz	short loc_3E358
		cmp	bl, 0Ah
		jz	loc_3E3C4
		cmp	bl, 0Ch
		jz	short loc_3E39E
		cmp	bl, 0Eh
		jz	short loc_3E37B
		ret

loc_3E358:
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E36B
		mov	ax, word ptr _levelmap[esi - 2 * MAP_WIDTH]
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		jz	short loc_3E327
		ret

loc_3E36B:
		mov	word ptr _levelmap[esi], 1000h + 0BBh
		sub	esi, 78h
		mov	word ptr _levelmap[esi], 1000h + MAP_SNIK_SNAK
		ret

loc_3E37B:
		cmp	word ptr _levelmap[esi - 2], 0
		jz	short loc_3E38E
		mov	ax, word ptr _levelmap[esi - 2]
		cmp	byte ptr _levelmap[esi - 2], 3
		jz	loc_3E327
		ret

loc_3E38E:
		mov	word ptr _levelmap[esi], 2000h + 0BBh
		sub	esi, 2
		mov	word ptr _levelmap[esi], 1800h + MAP_SNIK_SNAK
		ret


loc_3E39E:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E3B4
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jnz	short locret_3E3B3
		jmp	loc_3E327

locret_3E3B3:
		ret

loc_3E3B4:
		mov	word ptr _levelmap[esi], 30BBh
		add	esi, 78h
		mov	word ptr _levelmap[esi], 2000h + MAP_SNIK_SNAK
		ret

loc_3E3C4:
		cmp	word ptr _levelmap[esi + 2], 0
		jz	short loc_3E3DA
		mov	ax, word ptr _levelmap[esi + 2]
		cmp	byte ptr _levelmap[esi + 2], 3
		jnz	short locret_3E3D9
		jmp	loc_3E327

locret_3E3D9:
		ret

loc_3E3DA:
		mov	word ptr _levelmap[esi], 40BBh
		add	esi, 2
		mov	word ptr _levelmap[esi], 2800h + MAP_SNIK_SNAK
		ret

SnikSnakHandler_2 endp

SnikSnakHandler_3:
		shr ebx, 1
		sub ebx, 1Eh
		shr	ebx, 1
		cmp	bl, 7
		jnz	short loc_3E426
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short loc_3E426
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0

loc_3E426:
		cmp	bl, 8
		jge	short loc_3E433
		add	bl, 10h
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E433:
		mov	word ptr _levelmap[esi], MAP_SNIK_SNAK
		cmp	word ptr _levelmap[esi - 2], 0
		jnz	short loc_3E446
		mov	byte ptr _levelmap[esi + 1], 1
		ret

loc_3E446:
		cmp	byte ptr _levelmap[esi - 2], MAP_MURPHY
		jnz	short loc_3E453
		mov	byte ptr _levelmap[esi + 1], 1
		ret

loc_3E453:
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E46A
		mov	word ptr _levelmap[esi], 1000h + 0BBh
		sub	esi, 120
		mov	word ptr _levelmap[esi], 1000h + MAP_SNIK_SNAK
		ret

loc_3E46A:
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
		jnz	short loc_3E475
		call	CreateExplosion
		ret

loc_3E475:
		cmp	word ptr _levelmap[esi + 2], 0
		jnz	short loc_3E482
		mov	byte ptr _levelmap[esi + 1], 9
		ret

loc_3E482:
		cmp	byte ptr _levelmap[esi + 2], MAP_MURPHY
		jnz	short loc_3E48F
		mov	byte ptr _levelmap[esi + 1], 9
		ret

loc_3E48F:
		mov	byte ptr _levelmap[esi + 1], 1
		ret

SnikSnakHandler_4 proc near

		shr	ebx, 1
		sub ebx, 40h
		shr ebx, 1
		and	bl, 7
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E4CC
		cmp	byte ptr _levelmap[esi + 2], MAP_RED_LAMP
		jz	short loc_3E4CC
		mov	word ptr _levelmap[esi + 2], 0

loc_3E4CC:
		cmp	bl, 8
		jge	short loc_3E4D9
		add	bl, MAP_ELECTRON
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E4D9:
		mov	word ptr _levelmap[esi], MAP_SNIK_SNAK
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E4EC
		mov	byte ptr _levelmap[esi + 1], 3
		ret

loc_3E4EC:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E4F9
		mov	byte ptr _levelmap[esi + 1], 3
		ret

loc_3E4F9:
		cmp	word ptr _levelmap[esi - 2], 0
		jnz	short loc_3E510
		mov	word ptr _levelmap[esi], 20BBh
		sub	esi, 2
		mov	word ptr _levelmap[esi], 1800h + MAP_SNIK_SNAK
		ret

loc_3E510:
		cmp	byte ptr _levelmap[esi - 2], 3
		jnz	short loc_3E51B
		call	CreateExplosion
		ret

loc_3E51B:
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E528
		mov	byte ptr _levelmap[esi + 1], 0Fh
		ret

loc_3E528:
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E535
		mov	byte ptr _levelmap[esi + 1], 0Fh
		ret

loc_3E535:
		mov	byte ptr _levelmap[esi + 1], 3
		ret

SnikSnakHandler_4 endp

SnikSnakHandler_5 proc near
		shr	ebx, 1
		sub ebx, 40h
		shr ebx, 1
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E579
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short loc_3E579
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0

loc_3E579:
		cmp	bl, 8
		jge	short loc_3E586
		add	bl, 20h
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E586:
		mov	word ptr _levelmap[esi], MAP_SNIK_SNAK
		cmp	word ptr _levelmap[esi + 2], 0
		jnz	short loc_3E599
		mov	byte ptr _levelmap[esi + 1], 5
		ret

loc_3E599:
		cmp	byte ptr _levelmap[esi + 2], 3
		jnz	short loc_3E5A6
		mov	byte ptr _levelmap[esi + 1], 5
		ret

loc_3E5A6:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E5BD
		mov	word ptr _levelmap[esi], 30BBh
		add	esi, 78h
		mov	word ptr _levelmap[esi], 2000h + MAP_SNIK_SNAK
		ret

loc_3E5BD:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E5C8
		call	CreateExplosion
		ret

loc_3E5C8:
		cmp	word ptr _levelmap[esi - 2], 0
		jnz	short loc_3E5D5
		mov	byte ptr _levelmap[esi + 1], 0Dh
		ret


loc_3E5D5:
		cmp	byte ptr _levelmap[esi - 2], 3
		jnz	short loc_3E5E2
		mov	byte ptr _levelmap[esi + 1], 0Dh
		ret

loc_3E5E2:
		mov	byte ptr _levelmap[esi + 1], 5
		ret

SnikSnakHandler_5 endp

SnikSnakHandler_6 proc near

		shr	ebx, 2
		and	bl, 7
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E620
		cmp	byte ptr _levelmap[esi - 2], MAP_RED_LAMP
		jz	short loc_3E620
		mov	word ptr _levelmap[esi - 2], 0

loc_3E620:
		cmp	bl, 8
		jge	short loc_3E62D
		add	bl, 28h
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E62D:
		mov	word ptr _levelmap[esi], MAP_SNIK_SNAK
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E640
		mov	byte ptr _levelmap[esi + 1], 7
		ret

loc_3E640:
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E64D
		mov	byte ptr _levelmap[esi + 1], 7
		ret

loc_3E64D:
		cmp	word ptr _levelmap[esi + 2], 0
		jnz	short loc_3E664
		mov	word ptr _levelmap[esi], 40BBh
		add	esi, 2
		mov	word ptr _levelmap[esi], 2800h + MAP_SNIK_SNAK
		ret

loc_3E664:
		cmp	byte ptr _levelmap[esi + 2], 3
		jnz	short loc_3E66F
		call	CreateExplosion
		ret

loc_3E66F:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E67C
		mov	byte ptr _levelmap[esi + 1], MAP_PORT_RIGHT_TO_LEFT
		ret

loc_3E67C:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E689
		mov	byte ptr _levelmap[esi + 1], MAP_PORT_RIGHT_TO_LEFT
		ret

loc_3E689:
		mov	byte ptr _levelmap[esi + 1], 7
		ret

SnikSnakHandler_6 endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

handleElectron	proc near
		cmp	byte ptr [byte_403C7], 1
		jz	short locret_3E6B0
		cmp	byte ptr _levelmap[esi], MAP_ELECTRON
		jnz	short locret_3E6B0
		mov	bl, byte ptr _levelmap[esi + 1]
		movzx ebx, bl
		shl	ebx, 2
		mov	eax, dword ptr ElectronHandlers[ebx]
		cmp	eax, 0FFFFh
		jz	short locret_3E6B0
		call	eax

locret_3E6B0:
		ret
handleElectron	endp

ElectronHandler_1 proc near
		mov	ax, word ptr [word_40C4D]
		and	ax, 3
		jz	short loc_3E6BF
		cmp	ax, 3
		jz	short loc_3E6ED
		ret

loc_3E6BF:
		shr	ebx, 2
		inc	ebx
		and	bl, 7
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E6E9:
		call	CreateExplosion
		ret

loc_3E6ED:
		mov	bl, byte ptr _levelmap[esi + 1]
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
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E715
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		jz	short loc_3E6E9
		ret

loc_3E715:
		mov	word ptr _levelmap[esi], 100h + 0BBh
		sub	esi, 78h	; 'x'
		mov	word ptr _levelmap[esi], 1000h + MAP_ELECTRON
		ret

loc_3E725:
		cmp	word ptr _levelmap[esi - 2], 0
		jz	short loc_3E734
		cmp	byte ptr _levelmap[esi - 2], 3
		jz	short loc_3E6E9
		ret

loc_3E734:
		mov	word ptr _levelmap[esi], 200h + 0BBh
		sub	esi, 2
		mov	word ptr _levelmap[esi], 1800h + MAP_ELECTRON
		ret

loc_3E744:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E753
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jz	loc_3E6E9
		ret

loc_3E753:
		mov	word ptr _levelmap[esi], 300h + 0BBh
		add	esi, 78h
		mov	word ptr _levelmap[esi], 2000h + MAP_ELECTRON
		ret

loc_3E763:
		cmp	word ptr _levelmap[esi + 2], 0
		jz	short loc_3E775
		cmp	byte ptr _levelmap[esi + 2], 3
		jnz	short locret_3E774
		jmp	loc_3E6E9

locret_3E774:
		ret

loc_3E775:
		mov	word ptr _levelmap[esi], 400h + 0BBh
		add	esi, 2
		mov	word ptr _levelmap[esi], 2800h + MAP_ELECTRON
		ret

ElectronHandler_1 endp

ElectronHandler_2 proc near

		mov	ax, word ptr [word_40C4D]
		and	ax, 3
		jz	short loc_3E793
		cmp	ax, 3
		jz	short loc_3E7C3
		ret

loc_3E793:
		shr	ebx, 2
		inc	bl
		and	bl, 7
		or	bl, 8
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E7BF:
		call	CreateExplosion
		ret

loc_3E7C3:
		mov	bl, byte ptr _levelmap[esi + 1]
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
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jz	short loc_3E7EB
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		jz	short loc_3E7BF
		ret

loc_3E7EB:
		mov	word ptr _levelmap[esi], 100h + 0BBh
		sub	esi, 78h
		mov	word ptr _levelmap[esi], 1000h + MAP_ELECTRON
		ret

loc_3E7FB:
		cmp	word ptr _levelmap[esi - 2], 0
		jz	short loc_3E80A
		cmp	byte ptr _levelmap[esi - 2], 3
		jz	short loc_3E7BF
		ret

loc_3E80A:
		mov	word ptr _levelmap[esi], 200h + 0BBh
		sub	esi, 2
		mov	word ptr _levelmap[esi], 1800h + MAP_ELECTRON
		ret

loc_3E81A:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jz	short loc_3E829
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jz	loc_3E7BF
		ret

loc_3E829:
		mov	word ptr _levelmap[esi], 300h + 0BBh
		add	esi, 78h
		mov	word ptr _levelmap[esi], 2000h + MAP_ELECTRON
		ret

loc_3E839:
		cmp	word ptr _levelmap[esi + 2], MAP_SPACE
		jz	short loc_3E84B
		cmp	byte ptr _levelmap[esi + 2], MAP_MURPHY
		jnz	short locret_3E84A
		jmp	loc_3E7BF

locret_3E84A:
		ret

loc_3E84B:
		mov	word ptr _levelmap[esi], 400h + 0BBh
		add	esi, 2
		mov	word ptr _levelmap[esi], 2800h + MAP_ELECTRON
		ret

ElectronHandler_2 endp

ElectronHandler_3 proc near

		shr ebx, 1
		sub ebx, 01Eh
		shr	ebx, 1
		cmp	bl, 7
		jnz	short loc_3E897
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short loc_3E897
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0

loc_3E897:
		cmp	bl, 8
		jge	short loc_3E8A4
		add	bl, 10h
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E8A4:
		mov	word ptr _levelmap[esi], MAP_ELECTRON
		cmp	word ptr _levelmap[esi - 2], 0
		jnz	short loc_3E8B7
		mov	byte ptr _levelmap[esi + 1], 1
		ret

loc_3E8B7:
		cmp	byte ptr _levelmap[esi - 2], 3
		jnz	short loc_3E8C4
		mov	byte ptr _levelmap[esi + 1], 1
		ret

loc_3E8C4:
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E8DB
		mov	word ptr _levelmap[esi], 100h + 0BBh
		sub	esi, 78h
		mov	word ptr _levelmap[esi], 1000h + MAP_ELECTRON
		ret

loc_3E8DB:
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E8E6
		call	CreateExplosion
		ret

loc_3E8E6:
		cmp	word ptr _levelmap[esi + 2], 0
		jnz	short loc_3E8F3
		mov	byte ptr _levelmap[esi + 1], 9
		ret

loc_3E8F3:
		cmp	byte ptr _levelmap[esi + 2], 3
		jnz	short loc_3E900
		mov	byte ptr _levelmap[esi + 1], 9
		ret


loc_3E900:
		mov	byte ptr _levelmap[esi + 1], 1
		ret

ElectronHandler_3 endp

ElectronHandler_4 proc near

		shr	ebx, 2
		and	bl, 7
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E93D
		cmp	byte ptr _levelmap[esi + 2], MAP_RED_LAMP
		jz	short loc_3E93D
		mov	word ptr _levelmap[esi + 2], 0

loc_3E93D:
		cmp	bl, 8
		jge	short loc_3E94A
		add	bl, MAP_ELECTRON
		mov	byte ptr _levelmap[esi + 1], bl
		ret


loc_3E94A:
		mov	word ptr _levelmap[esi], MAP_ELECTRON
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3E95D
		mov	byte ptr _levelmap[esi + 1], 3
		ret


loc_3E95D:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3E96A
		mov	byte ptr _levelmap[esi + 1], 3
		ret


loc_3E96A:
		cmp	word ptr _levelmap[esi - 2], 0
		jnz	short loc_3E981
		mov	word ptr _levelmap[esi], 200h + 0BBh
		sub	esi, 2
		mov	word ptr _levelmap[esi], 1800h + MAP_ELECTRON
		ret


loc_3E981:
		cmp	byte ptr _levelmap[esi - 2], 3
		jnz	short loc_3E98C
		call	CreateExplosion
		ret


loc_3E98C:
		cmp	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0
		jnz	short loc_3E999
		mov	byte ptr _levelmap[esi + 1], 0Fh
		ret


loc_3E999:
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], 3
		jnz	short loc_3E9A6
		mov	byte ptr _levelmap[esi + 1], 0Fh
		ret


loc_3E9A6:
		mov	byte ptr _levelmap[esi + 1], 3
		ret

ElectronHandler_4 endp

; ===========================================================

ElectronHandler_5 proc near

		shr ebx, 1
		sub ebx, 40h
		shr	ebx, 1
		inc	bl
		cmp	bl, 7
		jnz	short loc_3E9EA
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_RED_LAMP
		jz	short loc_3E9EA
		mov	word ptr _levelmap[esi - 2 * MAP_WIDTH], 0

loc_3E9EA:
		cmp	bl, 8
		jge	short loc_3E9F7
		add	bl, 20h
		mov	byte ptr _levelmap[esi + 1], bl
		ret

loc_3E9F7:
		mov	word ptr _levelmap[esi], MAP_ELECTRON
		cmp	word ptr _levelmap[esi + 2], 0
		jnz	short loc_3EA0A
		mov	byte ptr _levelmap[esi + 1], 5
		ret

loc_3EA0A:
		cmp	byte ptr _levelmap[esi + 2], 3
		jnz	short loc_3EA17
		mov	byte ptr _levelmap[esi + 1], 5
		ret

loc_3EA17:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_3EA2E
		mov	word ptr _levelmap[esi], 300h + 0BBh
		add	esi, 78h
		mov	word ptr _levelmap[esi], 2000h + MAP_ELECTRON
		ret

loc_3EA2E:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], 3
		jnz	short loc_3EA39
		call	CreateExplosion
		ret

loc_3EA39:
		cmp	word ptr _levelmap[esi - 2], 0
		jnz	short loc_3EA46
		mov	byte ptr _levelmap[esi + 1], 0Dh
		ret

loc_3EA46:
		cmp	byte ptr _levelmap[esi - 2], 3
		jnz	short loc_3EA53
		mov	byte ptr _levelmap[esi + 1], 0Dh
		ret

loc_3EA53:
		mov	byte ptr _levelmap[esi + 1], 5
		ret

ElectronHandler_5 endp

; ===========================================================

ElectronHandler_6 proc near

			shr		ebx, 2
			and		bl, 7
			inc		bl
			cmp		bl, 7
			jne		short @@1
			cmp		byte ptr _levelmap[esi - 2], MAP_RED_LAMP
			je		short @@1
			mov		word ptr _levelmap[esi - 2], MAP_SPACE
@@1:		cmp		bl, 8
			jge		short @@2
			add		bl, 28h
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@2:		mov		word ptr _levelmap[esi], MAP_ELECTRON
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_SPACE
			jne		short @@3
			mov		byte ptr _levelmap[esi + 1], 7
			ret

@@3:		cmp		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
			jne		short @@4
			mov		byte ptr _levelmap[esi + 1], 7
			ret

@@4:		cmp		word ptr _levelmap[esi + 2], 0
			jne		short @@5
			mov		word ptr _levelmap[esi], 0400h + 0BBh
			add		esi, 2
			mov		word ptr _levelmap[esi], 2800h + MAP_ELECTRON
			ret

@@5:		cmp		byte ptr _levelmap[esi + 2], MAP_MURPHY
			jne		short @@6
			call	CreateExplosion
			ret

@@6:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			jne		short @@7
			mov		byte ptr _levelmap[esi + 1], 0Bh
			ret

@@7:		cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			jne		short @@8
			mov		byte ptr _levelmap[esi + 1], 0Bh
			ret

@@8:		mov		byte ptr _levelmap[esi + 1], 7
			ret

ElectronHandler_6 endp

.data

byte_4030C	db 0
byte_40325	db 0
byte_40326	db 0
byte_40327	db 0
byte_40328	db 100 dup(0)

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
dword_403B7	dd 0
PlayerPosition_Ofs dd 0
word_403BB	dw 0
word_403BD	dw 0
word_403BF	dw 0
word_403C1	dw 0
byte_403C3	db 0
byte_403C7	db 0
byte_403C8	db 0
word_403C9	dw 0
NumRedDisks	db 0
dword_403CC	dd 0
IsDemoRunning	db 0
PlayerPosition_PixelsX dw 0
PlayerPosition_PixelsY dw 0

word_403DE	dw 0
word_403E0	dw 0
word_403E2	dw 0
word_403E4	dw 0
word_403E6	dw 0
;dword_403E8	dd 0
word_403EA	dw 0
word_403EC	dw 0

SnikSnakHandlers label dword
		dd 8 dup (SnikSnakHandler_1)
		dd 8 dup (SnikSnakHandler_2)
		dd 8 dup (SnikSnakHandler_3)
		dd 8 dup (SnikSnakHandler_4)
		dd 8 dup (SnikSnakHandler_5)
		dd 8 dup (SnikSnakHandler_6)

ElectronHandlers label dword
		dd 8 dup (ElectronHandler_1)
		dd 8 dup (ElectronHandler_2)
		dd 8 dup (ElectronHandler_3)
		dd 8 dup (ElectronHandler_4)
		dd 8 dup (ElectronHandler_5)
		dd 8 dup (ElectronHandler_6)

MapHandlers_4		dd 0
		dd handleZonk
		dd 0
		dd 0
		dd handleInfotron
		dd 0
		dd 0
		dd 0
		dd xxx_handleOrangeDisk
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd handleSnikSnak
		dd 0
		dd xxx_HandleTerminal
		dd 0
		dd 0
		dd 0
		dd 0
		dd handleElectron
		dd xxx_HandleBug
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd loc_39493
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

levelmap label word
_levelmap	dw 1536 dup(0)
SomeLevelData	db 1536	dup(0)
RandomSeed	dw 0
word_4C47B	dw 0
word_4C47D	dw 0
word_4C47F	dw 2008 dup(0)
			db 0

joystickbuttons label byte
_joystickbuttons db 0

		end
