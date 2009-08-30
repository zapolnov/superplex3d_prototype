
.386

ifdef TASM
 locals @@
else
 .model flat
endif

.code

public PlayerExploded
public PlayerIsLookingLeft
public PlayerAnim_NumFrames
public PlayerAnim_ID
public PlayerPosition_Ofs
public PlayerPosition_PixelsX
public PlayerPosition_PixelsY
public PlayerIsInsideTeleport
public NumRedDisks
public RedDiskPlacementTimer
public RedDiskDetonateTimer
public RedDiskPosition
public SupaplexYawnTimeout
public levelmap
public loadlevelmap
public findplayerpositiononlevelmap
public initrandomnumbergenerator
public initplayerstate
public sub_392df
public beginlevel
public runthelevel
public joystickbuttons

public _PlayerExploded
public _PlayerIsLookingLeft
public _PlayerAnim_NumFrames
public _PlayerAnim_ID
public _PlayerPosition_Ofs
public _PlayerPosition_PixelsX
public _PlayerPosition_PixelsY
public _PlayerIsInsideTeleport
public _NumRedDisks
public _RedDiskPlacementTimer
public _RedDiskDetonateTimer
public _RedDiskPosition
public _SupaplexYawnTimeout
public _levelmap
public _loadlevelmap
public _findplayerpositiononlevelmap
public _initrandomnumbergenerator
public _initplayerstate
public _sub_392df
public _beginlevel
public _runthelevel
public _joystickbuttons

extrn _SND_PlayZonkFallSound : proc
extrn _SND_PlayBaseEatSound : proc
extrn _SND_PlayBugSound : proc
extrn _SND_PlayBoomSound : proc
extrn _SND_PlayExitSound : proc
extrn _SND_PlayObjectPushSound : proc
extrn _SND_PlayEatInfotronSound : proc

SPECIAL_PORT	struc ;	(sizeof=0X6)
coords		dw ?
gravity		db ?
freeze_zonks	db ?
freeze_enemies	db ?
unused		db ?
SPECIAL_PORT	ends

struc_2	struc ; (sizeof=0XE)
animID	dw ?
numFrames	dw	?
stepX	dw ?
stepY	dw ?
struc_2	ends

; enum PLAYER_ANIM
ANIM_MOVE_UP_L = 0
ANIM_MOVE_UP_R = 1
ANIM_MOVE_LEFT = 2
ANIM_MOVE_DOWN_L = 3
ANIM_MOVE_DOWN_R = 4
ANIM_MOVE_RIGHT = 5
ANIM_EXIT = 6
ANIM_EATBASE_UP_L = 7
ANIM_EATBASE_UP_R = 8
ANIM_EATBASE_LEFT = 9
ANIM_EATBASE_DOWN_L = 10
ANIM_EATBASE_DOWN_R = 11
ANIM_EATBASE_RIGHT = 12
ANIM_EATBASE_ATTOP = 13
ANIM_EATBASE_ATLEFT = 14
ANIM_EATBASE_ATBOTTOM = 15
ANIM_EATBASE_ATRIGHT = 16
ANIM_EATINFOTRON_UP_L = 17
ANIM_EATINFOTRON_UP_R = 18
ANIM_EATINFOTRON_LEFT = 19
ANIM_EATINFOTRON_DOWN_L = 20
ANIM_EATINFOTRON_DOWN_R = 21
ANIM_EATINFOTRON_RIGHT = 22
ANIM_EATINFOTRON_ATTOP = 23
ANIM_EATINFOTRON_ATLEFT = 24
ANIM_EATINFOTRON_ATBOTTOM = 25
ANIM_EATINFOTRON_ATRIGHT = 26
ANIM_SHIFTZONK_LEFT = 27
ANIM_SHIFTZONK_RIGHT = 28
ANIM_TELEPORT_UP = 29
ANIM_TELEPORT_LEFT = 30
ANIM_TELEPORT_DOWN = 31
ANIM_TELEPORT_RIGHT = 32
ANIM_EATDISK_UP_L = 33
ANIM_EATDISK_UP_R = 34
ANIM_EATDISK_LEFT = 35
ANIM_EATDISK_DOWN_L = 36
ANIM_EATDISK_DOWN_R = 37
ANIM_EATDISK_RIGHT = 38
ANIM_EATDISK_ATTOP = 39
ANIM_EATDISK_ATLEFT = 40
ANIM_EATDISK_ATBOTTOM = 41
ANIM_EATDISK_ATRIGHT = 42
ANIM_SHIFTYELLOWDISK_UP = 43
ANIM_SHIFTYELLOWDISK_LEFT = 44
ANIM_SHIFTYELLOWDISK_DOWN = 45
ANIM_SHIFTYELLOWDISK_RIGHT = 46
ANIM_SHIFTORANGEDISK_LEFT = 47
ANIM_SHIFTORANGEDISK_RIGHT = 48
ANIM_PLACEREDDISK = 49

; enum MAP_ELEMENT
MAP_SPACE						= 0
MAP_ZONK						= 1
MAP_BASE						= 2
MAP_MURPHY						= 3
MAP_INFOTRON					= 4
MAP_RAM_CHIP 					= 5
MAP_HARDWARE					= 6
MAP_EXIT						= 7
MAP_DISK_ORANGE					= 8
MAP_PORT_LEFT_TO_RIGHT			= 9
MAP_PORT_UP_TO_DOWN 			= 10
MAP_PORT_RIGHT_TO_LEFT			= 11
MAP_PORT_DOWN_TO_UP				= 12
MAP_PORT_SPECIAL_LEFT_TO_RIGHT	= 13
MAP_PORT_SPECIAL_UP_TO_DOWN		= 14
MAP_PORT_SPECIAL_RIGHT_TO_LEFT	= 15
MAP_PORT_SPECIAL_DOWN_TO_UP		= 16
MAP_SNIK_SNAK					= 17
MAP_DISK_YELLOW					= 18
MAP_TERMINAL					= 19
MAP_DISK_RED					= 20
MAP_PORT_VERTICAL				= 21
MAP_PORT_HORIZONTAL				= 22
MAP_PORT_CROSS					= 23
MAP_ELECTRON					= 24
MAP_BUG							= 25
MAP_RAM_CHIP_LEFT				= 26
MAP_RAM_CHIP_RIGHT				= 27
MAP_HARDWARE_1	 				= 28
MAP_HARDWARE_2					= 29
MAP_HARDWARE_3					= 30
MAP_HARDWARE_4					= 31
MAP_BOOM						= 31
MAP_HARDWARE_5					= 32
MAP_HARDWARE_6					= 33
MAP_HARDWARE_7					= 34
MAP_HARDWARE_8					= 35
MAP_HARDWARE_9					= 36
MAP_HARDWARE_10					= 37
MAP_RAM_CHIP_TOP				= 38
MAP_RAM_CHIP_BOTTOM				= 39
MAP_INVISIBLE_WALL				= 40

; enum MURPHY_ANIM
MURPHY_ANIM_MOVE_UP_EAT_SPACE	= 1
MURPHY_ANIM_MOVE_LEFT_EAT_SPACE	= 2
MURPHY_ANIM_MOVE_DOWN_EAT_SPACE	= 3
MURPHY_ANIM_MOVE_RIGHT_EAT_SPACE= 4
MURPHY_ANIM_MOVE_UP_EAT_BASE	= 5
MURPHY_ANIM_MOVE_LEFT_EAT_BASE	= 6
MURPHY_ANIM_MOVE_DOWN_EAT_BASE	= 7
MURPHY_ANIM_MOVE_RIGHT_EAT_BASE	= 8
MURPHY_ANIM_MOVE_UP_EAT_INFTR	= 9
MURPHY_ANIM_MOVE_LEFT_EAT_INFTR	= 10
MURPHY_ANIM_MOVE_DOWN_EAT_INFTR	= 11
MURPHY_ANIM_MOVE_RIGHT_EAT_INFTR= 12
MURPHY_ANIM_EXIT				= 13
MURPHY_ANIM_SHIFT_ZONK_LEFT		= 14
MURPHY_ANIM_SHIFT_ZONK_RIGHT	= 15
MURPHY_ANIM_EAT_BASE_TOP		= 16
MURPHY_ANIM_EAT_BASE_LEFT		= 17
MURPHY_ANIM_EAT_BASE_BOTTOM		= 18
MURPHY_ANIM_EAT_BASE_RIGHT		= 19
MURPHY_ANIM_EAT_INFOTRON_TOP	= 20
MURPHY_ANIM_EAT_INFOTRON_LEFT	= 21
MURPHY_ANIM_EAT_INFOTRON_BOTTOM	= 22
MURPHY_ANIM_EAT_INFOTRON_RIGHT	= 23
MURPHY_ANIM_TELEPORT_UP			= 24
MURPHY_ANIM_TELEPORT_LEFT		= 25
MURPHY_ANIM_TELEPORT_DOWN		= 26
MURPHY_ANIM_TELEPORT_RIGHT		= 27
MURPHY_ANIM_MOVE_UP_EAT_BOMB	= 28
MURPHY_ANIM_MOVE_LEFT_EAT_BOMB	= 29
MURPHY_ANIM_MOVE_DOWN_EAT_BOMB	= 30
MURPHY_ANIM_MOVE_RIGHT_EAT_BOMB	= 31
MURPHY_ANIM_EAT_BOMB_TOP		= 32
MURPHY_ANIM_EAT_BOMB_LEFT		= 33
MURPHY_ANIM_EAT_BOMB_BOTTOM		= 34
MURPHY_ANIM_EAT_BOMB_RIGHT		= 35
MURPHY_ANIM_MOVE_YELLOW_DISK_UP	= 36
MURPHY_ANIM_MOVE_YELLOW_DISK_LF	= 37
MURPHY_ANIM_MOVE_YELLOW_DISK_RT	= 38
MURPHY_ANIM_MOVE_YELLOW_DISK_DN	= 39
MURPHY_ANIM_SHIFT_ORANGE_DISK_LF= 40
MURPHY_ANIM_SHIFT_ORANGE_DISK_RT= 41
MURPHY_ANIM_PUT_BOMB			= 42

; enum JOYSTICK_BUTTON_CODE
JB_UP							= 1
JB_LEFT							= 2
JB_DOWN							= 3
JB_RIGHT						= 4
JB_SPACE_UP						= 5
JB_SPACE_LEFT					= 6
JB_SPACE_DOWN					= 7
JB_SPACE_RIGHT					= 8
JB_SPACE						= 9

; enum MAP_DIMENSIONS
MAP_HEIGHT	 = 24
MAP_WIDTH	 = 60
MAP_NUM_CELLS	 = 1440

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

HandleZonk	proc near

			cmp		byte ptr _levelmap[esi], MAP_ZONK
			jne		@@done
			mov		ax,	word ptr _levelmap[esi]
			cmp		ax, MAP_ZONK
			jne		@@7
			cmp		byte ptr [byte_40325], 2
			je		@@done
			mov		ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
			cmp		ax, MAP_SPACE
			je		short @@3
			cmp		ax, MAP_ZONK
			je		short @@1
			cmp		ax, MAP_INFOTRON
			je		short @@1
			cmp		ax, MAP_RAM_CHIP
			jne		@@done
@@1:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_SPACE
			je		short @@4
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short @@4
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short @@4
@@2:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_SPACE
			je		short @@5
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short @@5
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short @@5
			ret

@@3:		mov		byte ptr _levelmap[esi + 1], 40h
			jmp		short @@7

@@4:		cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			jne		short @@2
			mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			jmp		short @@7

@@5:		cmp		word ptr _levelmap[esi + 2], MAP_SPACE
			je		short @@6
			cmp		word ptr _levelmap[esi + 2], 9999h
			jne		@@done
			cmp		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], MAP_ZONK
			jne		@@done
@@6:		mov		byte ptr _levelmap[esi + 1], 60h
			mov		word ptr _levelmap[esi + 2], 8888h
@@7:		movzx	ebx, byte ptr _levelmap[esi + 1]
			mov		al, bl
			and		al, 0F0h
			cmp		al, 10h
			je		short @@8
			cmp		al, 20h
			je		@@22
			cmp		al, 30h
			je		@@26
			cmp		byte ptr [byte_40325], 2
			je		@@done
			cmp		al, 40h
			je		@@30
			cmp		al, 50h
			je		@@33
			cmp		al, 60h
			je		@@37
			cmp		al, 70h
			je		@@41
			ret

@@8:		movzx	ebx, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 16h
			jnz		short @@9
			mov		byte ptr _levelmap[esi + 1], bl
			sub		esi, 2 * MAP_WIDTH
			call	sub_3826F
			add		esi, 2 * MAP_WIDTH
			ret

@@9:		cmp		bl, 18h
			jge		short @@10
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@10:		mov		byte ptr _levelmap[esi + 1], 0
			cmp		byte ptr [byte_40325], 2
			je		@@done
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			je		@@12
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			je		@@12
			cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			je		@@15
			cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SNIK_SNAK
			je		@@20
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 200h + 0BBh
			je		@@16
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 400h + 0BBh
			je		@@18
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ELECTRON
			je		@@20
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_ORANGE
			je		@@21
			call	_SND_PlayZonkFallSound
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ZONK
			je		short @@11
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_INFOTRON
			je		short @@11
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_RAM_CHIP
			je		short @@11
			ret

@@11:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_SPACE
			je		short @@13
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short @@13
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short @@13
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_SPACE
			je		short @@14
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short @@14
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short @@14
			ret

@@12:		mov		word ptr _levelmap[esi], 7000h + MAP_ZONK
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			ret

@@13:		cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			jne		@@2
			mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			ret

@@14:		cmp		word ptr _levelmap[esi + 2], MAP_SPACE
			jne		@@done
			mov		byte ptr _levelmap[esi + 1], 60h
			mov		word ptr _levelmap[esi + 2], 8888h
			ret

@@15:		mov		bl, byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1]
			cmp		bl, 0Eh
			je		@@done
			cmp		bl, 0Fh
			je		@@done
			cmp		bl, 28h
			je		@@done
			cmp		bl, 29h
			je		@@done
			cmp		bl, 25h
			je		@@done
			cmp		bl, 26h
			je		@@done
@@16:		cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_ELECTRON
			jne		short @@17
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ELECTRON
@@17:		cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_BOOM
			je		short @@20
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_SPACE
			jmp		short @@20

@@18:		cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_ELECTRON
			jne		short @@19
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ELECTRON
@@19:		cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_BOOM
			je		short @@20
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_SPACE
			jmp		short @@20

@@20:		add		esi, 2 * MAP_WIDTH
			call	CreateExplosion
			ret

@@21:		add		esi, 2 * MAP_WIDTH
			mov		ebx, esi
			shr		ebx, 1
			mov		byte ptr ActiveExplosions[ebx], 6
			ret

@@22:		movzx	ebx, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 24h
			jne		short @@23
			mov		word ptr _levelmap[esi + 2], 0AAAAh
@@23:		cmp		bl, 26h
			jnz		short @@24
			mov		byte ptr _levelmap[esi + 1], bl
			add		esi, 2
			call	sub_3826F
			sub		esi, 2
			ret
@@24:		cmp		bl, 28h
			jge		short @@25
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@25:		mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _levelmap[esi], 1000h + MAP_ZONK
			ret

@@26:		movzx	ebx, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 34h
			jne		short @@27
			mov		word ptr _levelmap[esi - 2], 0AAAAh
@@27:		cmp		bl, 36h
			jnz		short @@28
			mov		byte ptr _levelmap[esi + 1], bl
			sub		esi, 2
			call	sub_3826F
			add		esi, 2
			ret

@@28:		cmp		bl, 38h
			jge		short @@29
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@29:		mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, MAP_WIDTH * 2
			mov		word ptr _levelmap[esi], 1000h + MAP_ZONK
			ret

@@30:		inc		bl
			cmp		bl, 42h
			jge		short @@31
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@31:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			je		short @@32
			dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@32:		mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _levelmap[esi], 1000h + MAP_ZONK
			ret

@@33:		movzx	ebx, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 52h
			jge		short @@34
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@34:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_SPACE
			jne		short @@36
			cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			je		short @@35
			cmp		word ptr _levelmap[esi - 2], 8888h
			jne		short @@36
@@35:		mov		word ptr _levelmap[esi], 0FFFFh
			sub		esi, 2
			mov		word ptr _levelmap[esi], 2200h + MAP_ZONK
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0FFFFh
			ret

@@36:		dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@37:		movzx	ebx, byte ptr _levelmap[esi + 1]
			inc		bl
			cmp		bl, 62h
			jge		short @@38
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@38:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_SPACE
			jne		short @@40
			cmp		word ptr _levelmap[esi + 2], MAP_SPACE
			je		short @@39
			cmp		word ptr _levelmap[esi + 2], 8888h
			jne		short @@40
@@39:		mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2
			mov		word ptr _levelmap[esi], 3200h + MAP_ZONK
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 0FFFFh
			ret

@@40:		dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret

@@41:		cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			je		short @@42
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			jne		short @@done
@@42:		mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _levelmap[esi], 1000h + MAP_ZONK
			jmp		@@8

@@done:		ret

HandleZonk	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


handleInfotron	proc near

			cmp		byte ptr _levelmap[esi], MAP_INFOTRON
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
loc_37DC6:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_SPACE
			je		short loc_37DFC
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			je		short loc_37DFC
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			je		short loc_37DFC
loc_37DDD:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_SPACE
			je		short loc_37E12
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			je		short loc_37E12
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			je		short loc_37E12
			ret
loc_37DF5:	mov		byte ptr _levelmap[esi + 1], 40h
			jmp		short loc_37E27
loc_37DFC:	cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			je		short loc_37E05
			jmp		short loc_37DDD
loc_37E05:	mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			jmp		short loc_37E27
loc_37E12:	cmp		word ptr _levelmap[esi + 2], MAP_SPACE
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
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			jnz		short loc_37EBE
			jmp		loc_37F4D
loc_37EBE:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			jnz		short loc_37EC9
			jmp		loc_37F4D
loc_37EC9:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
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
loc_37EFB:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_ORANGE
			jnz		short loc_37F05
			jmp		loc_37FA6
loc_37F05:	call	_SND_PlayZonkFallSound
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_ZONK
			je		short loc_37F1E
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_INFOTRON
			je		short loc_37F1E
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_RAM_CHIP
			je		short loc_37F1E
			ret
loc_37F1E:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_SPACE
			jz		short loc_37F5A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 8888h
			jz		short loc_37F5A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], 0AAAAh
			jz		short loc_37F5A
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_SPACE
			jz		short loc_37F70
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 8888h
			jz		short loc_37F70
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], 0AAAAh
			jz		short loc_37F70
			ret
loc_37F4D:	mov		word ptr _levelmap[esi], 7000h + MAP_INFOTRON
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_37F5A:	cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			je		short loc_37F64
			jmp		loc_37DDD
loc_37F64:	mov		byte ptr _levelmap[esi + 1], 50h
			mov		word ptr _levelmap[esi - 2], 8888h
			ret
loc_37F70:	cmp		word ptr _levelmap[esi + 2], MAP_SPACE
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
loc_38002:	mov		word ptr _levelmap[esi], 7000h + MAP_INFOTRON
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
loc_3805D:	mov		word ptr _levelmap[esi], 7000h + MAP_INFOTRON
			ret
loc_38064:	inc		bl
			cmp		bl, 42h
			jge		short loc_38070
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_38070:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			jz		short loc_3807E
			dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_3807E:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		byte ptr _levelmap[esi], MAP_INFOTRON
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
loc_380C9:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_SPACE
			jnz		short loc_380F9
			cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			jz		short loc_380DF
			cmp		word ptr _levelmap[esi - 2], 8888h
			jnz		short loc_380F9
loc_380DF:	mov		word ptr _levelmap[esi], 0FFFFh
			sub		esi, 2
			mov		byte ptr _levelmap[esi], MAP_INFOTRON
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
loc_38137:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_SPACE
			jne		short loc_38167
			cmp		word ptr _levelmap[esi + 2], MAP_SPACE
			je		short loc_3814D
			cmp		word ptr _levelmap[esi + 2], 8888h
			jne		short loc_38167
loc_3814D:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2
			mov		byte ptr _levelmap[esi], MAP_INFOTRON
			mov		byte ptr _levelmap[esi + 1], 32h
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			ret
loc_38167:	dec		bl
			mov		byte ptr _levelmap[esi + 1], bl
			ret
loc_3816E:	cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			je		short loc_3817E
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], 9999h
			je		short loc_3817E
			ret
loc_3817E:	mov		word ptr _levelmap[esi], 0FFFFh
			add		esi, 2 * MAP_WIDTH
			mov		word ptr _levelmap[esi], 1000h + MAP_INFOTRON
			jmp		loc_37E62

handleInfotron	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_38191	proc near

			cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3819E
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3819E:	cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_SPACE
			je		short loc_381C8
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_381C8
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_ZONK
			je		short loc_381BC
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_INFOTRON
			je		short loc_381C2
			ret
loc_381BC:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 40h
			ret
loc_381C2:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 40h
			ret
loc_381C8:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], MAP_ZONK
			je		short loc_381E5
			cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], MAP_INFOTRON
			je		loc_3822A
loc_381D6:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], MAP_ZONK
			je		short loc_38208
			cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], MAP_INFOTRON
			je		loc_3824D
			ret
loc_381E5:	cmp		word ptr _levelmap[esi - 2], MAP_ZONK
			je		short loc_381FC
			cmp		word ptr _levelmap[esi - 2], MAP_INFOTRON
			je		short loc_381FC
			cmp		word ptr _levelmap[esi - 2], MAP_RAM_CHIP
			je		short loc_381FC
			jmp		short loc_381D6
loc_381FC:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH + 1)], 60h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_38208:	cmp		word ptr _levelmap[esi + 2], MAP_ZONK
			je		short loc_3821E
			cmp		word ptr _levelmap[esi + 2], MAP_INFOTRON
			je		short loc_3821E
			cmp		word ptr _levelmap[esi + 2], MAP_RAM_CHIP
			je		short loc_3821E
			ret
loc_3821E:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 3)], 50h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
loc_3822A:	cmp		word ptr _levelmap[esi - 2], MAP_ZONK
			je		short loc_38241
			cmp		word ptr _levelmap[esi - 2], MAP_INFOTRON
			je		short loc_38241
			cmp		word ptr _levelmap[esi - 2], MAP_RAM_CHIP
			je		short loc_38241
			jmp		loc_381D6
loc_38241:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH + 1)], 60h ; '`'
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_3824D:	cmp		word ptr _levelmap[esi + 2], MAP_ZONK
			je		short loc_38263
			cmp		word ptr _levelmap[esi + 2], MAP_INFOTRON
			je		short loc_38263
			cmp		word ptr _levelmap[esi + 2], MAP_RAM_CHIP
			je		short loc_38263
			ret
loc_38263:	mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 3)], 50h
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_38191	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_3826F	proc near

			cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3827C
			mov		word ptr _levelmap[esi], 0
loc_3827C:	cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_SPACE
			je		short loc_38294
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_3828C
			ret
loc_3828C:	cmp		byte ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_INFOTRON
			je		short loc_38294
			ret
loc_38294:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], MAP_ZONK
			je		short loc_382A3
loc_3829B:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], MAP_ZONK
			je		short loc_382C7
			ret
loc_382A3:	cmp		word ptr _levelmap[esi - 2], MAP_ZONK
			je		short loc_382BA
			cmp		word ptr _levelmap[esi - 2], MAP_INFOTRON
			je		short loc_382BA
			cmp		word ptr _levelmap[esi - 2], MAP_RAM_CHIP
			je		short loc_382BA
			jmp		short loc_3829B
loc_382BA:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 6000h + MAP_ZONK
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_382C7:	cmp		word ptr _levelmap[esi + 2], MAP_ZONK
			je		short loc_382DD
			cmp		word ptr _levelmap[esi + 2], MAP_INFOTRON
			je		short loc_382DD
			cmp		word ptr _levelmap[esi + 2], MAP_RAM_CHIP
			je		short loc_382DD
			ret
loc_382DD:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 5000h + MAP_ZONK
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_3826F	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


sub_382EA	proc near

			cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_382F7
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_382F7:	cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_SPACE
			je		short loc_3830F
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], 9999h
			je		short loc_38307
			ret
loc_38307:	cmp		byte ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_ZONK
			je		short loc_3830F
			ret
loc_3830F:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], MAP_INFOTRON
			je		short loc_3831E
loc_38316:	cmp		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], MAP_INFOTRON
			je		short loc_38342
			ret
loc_3831E:	cmp		word ptr _levelmap[esi - 2], MAP_ZONK
			je		short loc_38335
			cmp		word ptr _levelmap[esi - 2], MAP_INFOTRON
			je		short loc_38335
			cmp		word ptr _levelmap[esi - 2], MAP_RAM_CHIP
			je		short loc_38335
			jmp		short loc_38316
loc_38335:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], 6000h + MAP_INFOTRON
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret
loc_38342:	cmp		word ptr _levelmap[esi + 2], MAP_ZONK
			je		short loc_38358
			cmp		word ptr _levelmap[esi + 2], MAP_INFOTRON
			je		short loc_38358
			cmp		word ptr _levelmap[esi + 2], MAP_RAM_CHIP
			je		short loc_38358
			ret
loc_38358:	mov		word ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], 5000h + MAP_INFOTRON
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], 8888h
			ret

sub_382EA	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

initplayerstate label byte
_initplayerstate	proc near

			xor		ax, ax
			mov		word ptr [PlayerIsLookingLeft], ax
			mov		word ptr [PlayerDied], ax
			mov		word ptr [PlayerExploded], ax
			mov		word ptr [PlayerCompletedLevel], ax
			mov		word ptr [TimeoutAfterWhichLeaveTheLevel], ax
			mov		byte ptr [NumRedDisks], 0
			mov		byte ptr [byte_40C6C], 0
			mov		word ptr [word_4C47F], 0
			mov		word ptr [word_4C47B], 0
			mov		word ptr [word_4C47D], 0
			mov		word ptr [SupaplexYawnTimeout], 0
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
			mov		byte ptr [TerminalsActivated], 0
			mov		word ptr [CurrentFrameNumber], 0
			mov		byte ptr [byte_403B1], 1
			mov		byte ptr [byte_403B2], 0
			mov		byte ptr [byte_403C7], 0
			ret

_initplayerstate	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

beginlevel label byte
_beginlevel proc near

			cmp		byte ptr [IsDemoRunning], 0
			jz		short @@1
			mov		byte ptr [byte_403AA], 0
			jmp		short @@2
@@1:		mov		byte ptr [byte_403AA], 1
@@2:		;mov	word ptr [word_40392], 1
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
loc_3846B:	xor		ebx, ebx;call	PollMouse
			cmp		bx, 1
			jnz		short loc_38480
			cmp		word ptr [TimeoutAfterWhichLeaveTheLevel], 0
			jg		short loc_38480
			mov		word ptr [PlayerDied], 1
loc_38480:	cmp		bx, 2
			jnz		short loc_3848F
			cmp		word ptr [word_40C60], 0
			jz		short loc_3848F
			;call	VID_PaintGameMap
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
			call	CreateChainedExplosions
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
			call	_SND_PlayBaseEatSound
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
			je		short loc_38592
loc_38592:	inc		word ptr [CurrentFrameNumber]
			cmp		word ptr [PlayerCompletedLevel], 1
			je		short loc_385B0
			cmp		word ptr [TimeoutAfterWhichLeaveTheLevel], 0
			jne		short loc_385A7
			mov		eax, 1
			jmp short locret
loc_385A7:	dec		word ptr [TimeoutAfterWhichLeaveTheLevel]
			je		short loc_385B0
			mov		eax, 1
			jmp short locret
loc_385B0:	;mov		[word_40392], 0
			;cmp		[DemoIsBeingRecorded], 0
			;jz		short loc_385C0
			;call	EndDemoRecording
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
			call	HandlePlayerMovement
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
			cmp		dword ptr MapObjectHandlers[ebx], 0
			je		short loc_38EC9
			mov		eax, MapObjectHandlers[ebx]
			push	esi
			push	edi
			push	ecx
			call	eax
			pop		ecx
			pop		edi
			pop		esi
loc_38EC9:	add		esi, 2
			loop	loc_38EA2
loc_38EE8:	cmp		word ptr [PlayerDied], 1
			je		short loc_38F01
			cmp		word ptr [word_403BF], 0
			je		short loc_38F01
			ret
loc_38F01:	cmp		word ptr [TimeoutAfterWhichLeaveTheLevel], 0
			jne		short loc_38F1B
			mov		word ptr [PlayerDied], 0
			mov		word ptr [PlayerExploded], 1
			mov		esi, dword ptr [CurrentMurphyOfs]
			call	CreateExplosion
			mov		word ptr [TimeoutAfterWhichLeaveTheLevel], 64
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
		cmp	ax, 0A8h
		jle	short loc_38F98
		mov	ax, 0A8h

loc_38F98:
		add	bx, 0;word ptr [word_40C53]
		add	ax, 0;word ptr [word_40C55]
		mov	word ptr [word_40C4F], bx
		mov	word ptr [word_40C51], ax
		ret
sub_38F26	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

xxx_HandleBug	proc near

		cmp	byte ptr _levelmap[esi], MAP_BUG
		je	short loc_39012
		ret

loc_39012:
		mov	ax, word ptr [CurrentFrameNumber]
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
		add	al, 20h
		neg	al
		mov	bl, al

loc_39034:
		mov	byte ptr _levelmap[esi + 1], bl
		cmp	bl, 0
		jge	short loc_3903E
		ret

loc_3903E:
		cmp	byte ptr _levelmap[esi - (2 * MAP_WIDTH + 2)], MAP_MURPHY
		je	short loc_39078
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
		je	short loc_39078
		cmp	byte ptr _levelmap[esi - (2 * MAP_WIDTH - 2)], MAP_MURPHY
		je	short loc_39078
		cmp	byte ptr _levelmap[esi - 2], MAP_MURPHY
		je	short loc_39078
		cmp	byte ptr _levelmap[esi + 2], MAP_MURPHY
		je	short loc_39078
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH - 2], MAP_MURPHY
		je	short loc_39078
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
		je	short loc_39078
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 2], MAP_MURPHY
		je	short loc_39078
		jmp	short loc_3907B

loc_39078:
		call	_SND_PlayBugSound

loc_3907B:
		ret

xxx_HandleBug	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


xxx_HandleTerminal proc	near
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

			xor		eax, eax
			mov		word ptr [word_40C4F], ax
			mov		word ptr [word_40C51], ax
			mov		edi, offset _levelmap
			mov		ecx, MAP_NUM_CELLS
			mov		al, MAP_MURPHY
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
			xor		ah, ah
			shl		eax, 4
			shl		ebx, 4
			mov		word ptr [PlayerPosition_PixelsX], bx
			mov		word ptr [PlayerPosition_PixelsY], ax
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
		mov	word ptr [esi], MAP_RAM_CHIP
		jmp	loc_39398


loc_3932D:
		mov	word ptr [esi], MAP_HARDWARE
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
		cmp	word ptr [esi - 2], 0
		jnz	short loc_3936D
		mov	byte ptr [esi + 1], 1
		jmp	short loc_39398

loc_3936D:
		cmp	word ptr [esi - 2 * MAP_WIDTH], MAP_SPACE
		jnz	short loc_3937E
		mov	word ptr [esi - 2 * MAP_WIDTH], 1000h + MAP_ELECTRON
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398

loc_3937E:
		cmp	word ptr [esi + 2], MAP_SPACE
		jnz	short loc_39398
		mov	word ptr [esi + 2], 2800h + MAP_ELECTRON
		mov	word ptr [esi], 0FFFFh
		jmp	short loc_39398

loc_3938F:
		sub	byte ptr [esi], 4
		mov	byte ptr [esi + 1], 1
		jmp	short loc_39398

loc_39398:
		add		esi, 2
		inc		ebx
		dec		ecx
		jz		short loc_393A2
		jmp		loc_392E9

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
		cmp	ax, 3000h + MAP_DISK_ORANGE
		jge	short loc_39424
		cmp	ax, 2000h + MAP_DISK_ORANGE
		jge	short loc_393FA
		mov	ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
		cmp	ax, MAP_SPACE
		jz	short loc_393EF
		ret

loc_393EF:
		mov	byte ptr _levelmap[esi + 1], 20h
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 8
		ret

loc_393FA:
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], 0
		jnz	short loc_39408
		mov	word ptr _levelmap[esi], MAP_DISK_ORANGE
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
		mov	word ptr _levelmap[esi], MAP_SPACE
		mov	word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_ORANGE
		add	esi, 2 * MAP_WIDTH
		cmp	word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
		jne	short loc_39487
		mov	byte ptr _levelmap[esi + 1], 30h
		mov	byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 8
		ret

loc_39487:
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_BOOM
		jne	short loc_3948F
		ret

loc_3948F:
		call	CreateExplosion
		ret

xxx_handleOrangeDisk endp


loc_39493:
		cmp	byte ptr _levelmap[esi], MAP_BOOM
		jz	short loc_3949B
		ret

loc_3949B:
		mov	ax, word ptr [CurrentFrameNumber]
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
		mov	word ptr _levelmap[esi], MAP_INFOTRON
		mov	byte ptr [byte_403B0], 0
		ret

loc_39503:
		mov	byte ptr _levelmap[esi + 1], bl
		ret

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


CreateChainedExplosions	proc near

		mov	esi, 0
		mov	ebx, 0
		mov	ecx, MAP_WIDTH * MAP_HEIGHT

loc_39539:
		cmp	byte ptr ActiveExplosions[ebx], 0
		jz	short loc_39551
		jl	short loc_39558
		dec	byte ptr ActiveExplosions[ebx]
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
		inc	byte ptr ActiveExplosions[ebx]
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
CreateChainedExplosions	endp


; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл


CreateExplosion	proc near
		cmp	byte ptr _levelmap[esi], MAP_HARDWARE
		jnz	short loc_39577
		ret


loc_39577:
		mov	byte ptr [byte_403B0], 1
		cmp	byte ptr _levelmap[esi], MAP_MURPHY
		jnz	short loc_39589
		mov	word ptr [PlayerDied], 1

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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_395D0
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_395D0
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_395D0
		cmp	al, MAP_ZONK
		jz	short loc_395EC
		cmp	al, MAP_INFOTRON
		jz	short loc_395E2
		cmp	al, MAP_ELECTRON
		jnz	short loc_395C6
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_395D0

loc_395C6:
		cmp	al, MAP_MURPHY
		jnz	short loc_395D8
		mov	word ptr [PlayerDied], 1

loc_395D0:
		cmp	al, MAP_HARDWARE
		jz	short loc_395D8
		mov	ActiveExplosions[ebx - MAP_WIDTH - 1], dh

loc_395D8:
		cmp	al, MAP_HARDWARE
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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_39627
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39627
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39627
		cmp	al, MAP_ZONK
		jz	short loc_39643
		cmp	al, MAP_INFOTRON
		jz	short loc_39639
		cmp	al, MAP_ELECTRON
		jnz	short loc_3961D
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39627

loc_3961D:
		cmp	al, MAP_MURPHY
		jnz	short loc_3962F
		mov	word ptr [PlayerDied], 1

loc_39627:
		cmp	al, MAP_HARDWARE
		jz	short loc_3962F
		mov	ActiveExplosions[ebx - MAP_WIDTH], dh

loc_3962F:
		cmp	al, MAP_HARDWARE
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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_3967E
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_3967E
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_3967E
		cmp	al, MAP_ZONK
		jz	short loc_3969A
		cmp	al, MAP_INFOTRON
		jz	short loc_39690
		cmp	al, MAP_ELECTRON
		jnz	short loc_39674
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_3967E

loc_39674:
		cmp	al, MAP_MURPHY
		jnz	short loc_39686
		mov	word ptr [PlayerDied], 1

loc_3967E:
		cmp	al, MAP_HARDWARE
		jz	short loc_39686
		mov	ActiveExplosions[ebx - (2 * MAP_WIDTH - 1)], dh

loc_39686:
		cmp	al, MAP_HARDWARE
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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_396D5
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_396D5
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_396D5
		cmp	al, MAP_ZONK
		jz	short loc_396F1
		cmp	al, MAP_INFOTRON
		jz	short loc_396E7
		cmp	al, MAP_ELECTRON
		jnz	short loc_396CB
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_396D5


loc_396CB:
		cmp	al, MAP_MURPHY
		jnz	short loc_396DD
		mov	word ptr [PlayerDied], 1

loc_396D5:
		cmp	al, MAP_HARDWARE
		jz	short loc_396DD
		mov	ActiveExplosions[ebx - 1], dh

loc_396DD:
		cmp	al, MAP_HARDWARE
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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_39730
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39730
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39730
		cmp	al, MAP_ZONK
		jz	short loc_3974C
		cmp	al, MAP_INFOTRON
		jz	short loc_39742
		cmp	al, MAP_ELECTRON
		jnz	short loc_39726
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39730

loc_39726:
		cmp	al, MAP_MURPHY
		jnz	short loc_39738
		mov	word ptr [PlayerDied], 1

loc_39730:
		cmp	al, MAP_HARDWARE
		jz	short loc_39738
		mov	ActiveExplosions[ebx + 1], dh

loc_39738:
		cmp	al, MAP_HARDWARE
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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_39787
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39787
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39787
		cmp	al, MAP_ZONK
		jz	short loc_397A3
		cmp	al, MAP_INFOTRON
		jz	short loc_39799
		cmp	al, MAP_ELECTRON
		jnz	short loc_3977D
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39787

loc_3977D:
		cmp	al, MAP_MURPHY
		jnz	short loc_3978F
		mov	word ptr [PlayerDied], 1

loc_39787:
		cmp	al, MAP_HARDWARE
		jz	short loc_3978F
		mov	byte ptr ActiveExplosions[ebx + 2 * MAP_WIDTH - 1], dh

loc_3978F:
		cmp	al, MAP_HARDWARE
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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_397DE
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_397DE
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_397DE
		cmp	al, MAP_ZONK
		jz	short loc_397FA
		cmp	al, MAP_INFOTRON
		jz	short loc_397F0
		cmp	al, MAP_ELECTRON
		jnz	short loc_397D4
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_397DE

loc_397D4:
		cmp	al, MAP_MURPHY
		jnz	short loc_397E6
		mov	word ptr [PlayerDied], 1

loc_397DE:
		cmp	al, MAP_HARDWARE
		jz	short loc_397E6
		mov	ActiveExplosions[ebx + MAP_WIDTH], dh

loc_397E6:
		cmp	al, MAP_HARDWARE
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
		cmp	al, MAP_DISK_ORANGE
		jz	short loc_39835
		cmp	al, MAP_DISK_YELLOW
		jz	short loc_39835
		cmp	al, MAP_SNIK_SNAK
		jz	short loc_39835
		cmp	al, MAP_ZONK
		jz	short loc_39851
		cmp	al, MAP_INFOTRON
		jz	short loc_39847
		cmp	al, MAP_ELECTRON
		jnz	short loc_3982B
		neg	dh
		mov	ecx, 801Fh
		jmp	short loc_39835


loc_3982B:
		cmp	al, MAP_MURPHY
		jnz	short loc_3983D
		mov	word ptr [PlayerDied], 1

loc_39835:
		cmp	al, MAP_HARDWARE
		jz	short loc_3983D
		mov	ActiveExplosions[ebx + MAP_WIDTH + 1], dh

loc_3983D:
		cmp	al, MAP_HARDWARE
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
		call	_SND_PlayBoomSound
		ret
CreateExplosion	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

PlaceRedDisk	proc near
		mov		al, byte ptr [RedDiskDetonateTimer]
		cmp		al, 1
		jle		short loc_398AA
		mov		esi, dword ptr [RedDiskPosition]
		cmp		word ptr _levelmap[esi], MAP_SPACE
		jnz		short loc_39882
		mov		word ptr _levelmap[esi], MAP_DISK_RED
loc_39882:
		inc	byte ptr [RedDiskDetonateTimer]
		cmp	byte ptr [RedDiskDetonateTimer], 40
		jl	short loc_398AA
		mov	esi, dword ptr [RedDiskPosition]
		call	CreateExplosion
		mov	byte ptr [RedDiskDetonateTimer], 0

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

		cmp	byte ptr _levelmap[esi], MAP_BOOM
		jnz	short @@1
		ret

@@1:
		mov	word ptr _levelmap[esi], 0
		ret

sub_399E9	endp


loadlevelmap label byte
_loadlevelmap	proc near

		push	edi

		mov		edi, offset ActiveExplosions
		xor		al, al
		mov		ecx, 1536
		cld
		rep 	stosb

		pop edi
		ret

_loadlevelmap	endp

; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

HandlePlayerMovement proc near

			mov		[word_403BF], 1
			cmp		byte ptr _levelmap[esi], MAP_MURPHY
			jne		@@done
			mov		[CurrentMurphyOfs], esi
			mov		ax, word ptr _levelmap[esi]
			cmp		ax, MAP_MURPHY
			jne		loc_3D84B
			mov		[byte_403C8], 0
			cmp		[byte_4030C], 0
			je		short @@1
			cmp		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_PORT_DOWN_TO_UP
			je		short @@1
			cmp		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_PORT_VERTICAL
			je		short @@1
			cmp		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_PORT_CROSS
			je		short @@1
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			jne		short @@1
			mov		[byte_403C8], 1
@@1:		mov		bl, [_joystickbuttons]
			cmp		bl, 0
			jne		@@keypress
			mov		[byte_403C3], 1
			cmp		[byte_403C8], 0
			jne		@@d_space
			mov		ax, [CurrentFrameNumber]
			and		ax, 3
			jne		@@done
			inc		[SupaplexYawnTimeout]
			cmp		[SupaplexYawnTimeout], 4
			je		@@done
			cmp		[SupaplexYawnTimeout], 500
			jle		@@done
			cmp		[SupaplexYawnTimeout], 522
			jle		@@done
			cmp		[SupaplexYawnTimeout], 1000
			jle		@@done
			cmp		[SupaplexYawnTimeout], 1022
			jle		@@done
			cmp		[SupaplexYawnTimeout], 1600
			jle		@@done
			cmp		[SupaplexYawnTimeout], 1622
			jle		@@done
			cmp		[SupaplexYawnTimeout], 1654
			jg		@@done
			cmp		word ptr _levelmap[esi - 2], MAP_SPACE
			jne		@@done
			cmp		word ptr _levelmap[esi + 2], MAP_SPACE
			jne		@@done
			mov		[SupaplexYawnTimeout], 36
			ret

@@keypress:	cmp		[byte_403C8], 0
			je		short loc_3CC23
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
			jne		short loc_3CC23
			cmp		bl, JB_UP
			jne		short loc_3CC09
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BASE
			je		short loc_3CC23
loc_3CC09:	cmp		bl, JB_LEFT
			jne		short loc_3CC15
			cmp		word ptr _levelmap[esi - 2], MAP_BASE
			je		short loc_3CC23
loc_3CC15:	cmp		bl, JB_RIGHT
			jne		short loc_3CC21
			cmp		word ptr _levelmap[esi + 2], MAP_BASE
			je		short loc_3CC23
loc_3CC21:	mov		bl, JB_DOWN
loc_3CC23:	cmp		bl, JB_UP
			jne		short loc_3CC2F
			mov		[byte_403C3], 0
			jmp		@@moveup
loc_3CC2F:	cmp		bl, JB_LEFT
			jne		short loc_3CC3C
			mov		[byte_403C3], 0
			jmp		@@moveleft
loc_3CC3C:	cmp		bl, JB_DOWN
			jne		short loc_3CC49
			mov		[byte_403C3], 0
			jmp		@@movedown
loc_3CC49:	cmp		bl, JB_RIGHT
			jne		short loc_3CC56
			mov		[byte_403C3], 0
			jmp		@@moveright
loc_3CC56:	cmp		bl, JB_SPACE_UP
			jne		short loc_3CC63
			mov		[byte_403C3], 0
			jmp		@@eattop
loc_3CC63:	cmp		bl, JB_SPACE_LEFT
			jne		short loc_3CC70
			mov		[byte_403C3], 0
			jmp		@@eatleft
loc_3CC70:	cmp		bl, JB_SPACE_DOWN
			jne		short loc_3CC7D
			mov		[byte_403C3], 0
			jmp		@@eatbottom
loc_3CC7D:	cmp		bl, JB_SPACE_RIGHT
			jne		short loc_3CC8A
			mov		[byte_403C3], 0
			jmp		@@eatright
loc_3CC8A:	cmp		bl, JB_SPACE
			jne		short loc_3CC92
			jmp		@@putbomb
loc_3CC92:	mov		[byte_403C3], 0
			ret

@@moveup:	mov		ax, word ptr _levelmap[esi - 2 * MAP_WIDTH]
			cmp		ax, MAP_SPACE
			je		@@u_space
			cmp		ax, MAP_BASE
			je		@@u_base
			cmp		al, MAP_BUG
			je		@@u_bug
			cmp		ax, MAP_INFOTRON
			je		@@u_inftr
			cmp		ax, MAP_EXIT
			je		@@levelexit
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_PORT_DOWN_TO_UP
			je		@@u_port
			cmp		al, MAP_PORT_VERTICAL
			je		@@u_port
			cmp		al, MAP_PORT_CROSS
			je		@@u_port
			cmp		al, MAP_DISK_RED
			je		@@u_rdisk
			cmp		al, MAP_DISK_YELLOW
			je		@@u_ydisk
			push	esi
			sub		esi, 2 * MAP_WIDTH
			call	sub_3E0D7
			pop		esi
			jnc		@@moveup
			ret

@@moveleft:	mov		[PlayerIsLookingLeft], 1
			mov		ax, word ptr _levelmap[esi - 2]
			cmp		ax, MAP_SPACE
			je		@@l_space
			cmp		ax, MAP_BASE
			je		@@l_base
			cmp		al, MAP_BUG
			je		@@l_bug
			cmp		ax, MAP_INFOTRON
			je		@@l_inftr
			cmp		ax, MAP_EXIT
			je		@@levelexit
			cmp		ax, MAP_ZONK
			je		@@l_zonk
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_PORT_RIGHT_TO_LEFT
			je		@@l_port
			cmp		al, MAP_PORT_HORIZONTAL
			je		@@l_port
			cmp		al, MAP_PORT_CROSS
			je		@@l_port
			cmp		ax, MAP_DISK_RED
			je		@@l_rdisk
			cmp		ax, MAP_DISK_YELLOW
			je		@@l_ydisk
			cmp		ax, MAP_DISK_ORANGE
			je		@@l_odisk
			push	esi
			sub		esi, 2
			call	sub_3E0D7
			pop		esi
			jnc		@@moveleft
			ret

@@movedown:	mov		ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
			cmp		ax, MAP_SPACE
			je		@@d_space
			cmp		ax, MAP_BASE
			je		@@d_base
			cmp		al, MAP_BUG
			je		@@d_bug
			cmp		ax, MAP_INFOTRON
			je		@@d_inftr
			cmp		ax, MAP_EXIT
			je		@@levelexit
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_PORT_UP_TO_DOWN
			je		@@d_port
			cmp		al, MAP_PORT_VERTICAL
			je		@@d_port
			cmp		al, MAP_PORT_CROSS
			je		@@d_port
			cmp		al, MAP_DISK_RED
			je		@@d_rdisk
			cmp		al, MAP_DISK_YELLOW
			je		@@d_ydisk
			push	esi
			add		esi, 2 * MAP_WIDTH
			call	sub_3E0D7
			pop		esi
			jnc		@@movedown

@@moveright:mov		[PlayerIsLookingLeft], 0
			mov		ax, word ptr _levelmap[esi + 2]
			cmp		ax, MAP_SPACE
			je		@@r_space
			cmp		ax, MAP_BASE
			je		@@r_base
			cmp		al, MAP_BUG
			je		@@r_bug
			cmp		ax, MAP_INFOTRON
			je		@@r_inftr
			cmp		ax, MAP_EXIT
			je		@@levelexit
			cmp		ax, MAP_ZONK
			je		@@r_zonk
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_PORT_LEFT_TO_RIGHT
			je		@@r_port
			cmp		al, MAP_PORT_HORIZONTAL
			je		@@r_port
			cmp		al, MAP_PORT_CROSS
			je		@@r_port
			cmp		al, MAP_DISK_RED
			je		@@r_rdisk
			cmp		al, MAP_DISK_YELLOW
			je		@@r_ydisk
			cmp		ax, MAP_DISK_ORANGE
			je		@@r_odisk
			push	esi
			add		esi, 2
			call	sub_3E0D7
			pop		esi
			jnc		@@moveright
			ret

@@eattop:	mov		ax, word ptr _levelmap[esi - 2 * MAP_WIDTH]
			cmp		ax, MAP_BASE
			je		@@et_base
			cmp		al, MAP_BUG
			je		@@et_bug
			cmp		ax, MAP_INFOTRON
			je		@@et_inftr
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_DISK_RED
			je		@@et_rdisk
			ret

@@eatleft:	mov		[PlayerIsLookingLeft], 1
			mov		ax, word ptr _levelmap[esi - 2]
			cmp		ax, MAP_BASE
			je		@@el_base
			cmp		al, MAP_BUG
			je		@@el_bug
			cmp		ax, MAP_INFOTRON
			je		@@el_inftr
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_DISK_RED
			je		@@el_rdisk
			ret

@@eatbottom:mov		ax, word ptr _levelmap[esi + 2 * MAP_WIDTH]
			cmp		ax, MAP_BASE
			je		@@eb_base
			cmp		al, MAP_BUG
			je		@@eb_bug
			cmp		ax, MAP_INFOTRON
			je		@@eb_inftr
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_DISK_RED
			je		@@eb_rdisk
			ret

@@eatright:	mov		[PlayerIsLookingLeft], 0
			mov		ax, word ptr _levelmap[esi + 2]
			cmp		ax, MAP_BASE
			je		@@er_base
			cmp		al, MAP_BUG
			je		@@er_bug
			cmp		ax,	MAP_INFOTRON
			je		@@er_inftr
			cmp		al, MAP_TERMINAL
			je		@@terminal
			cmp		al, MAP_DISK_RED
			je		@@er_rdisk
			ret

@@putbomb:	cmp		[NumRedDisks], 0
			je		@@done
			cmp		[RedDiskDetonateTimer], 0
			jne		@@done
			cmp		[byte_403C3], 1
			jne		@@done
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_PUT_BOMB
			mov		word ptr [RedDiskPlacementTimer], 64
			mov		edx, offset stru_4070E
			mov		[RedDiskDetonateTimer], 1
			mov		[RedDiskPosition], esi
			jmp		loc_3D835

@@u_space:	cmp		[PlayerIsLookingLeft], 0
			je		short loc_3CF3E
			mov		edx, offset stru_403FE
			jmp		short loc_3CF41
loc_3CF3E:	mov		edx, offset stru_4040E
loc_3CF41:	mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], MURPHY_ANIM_MOVE_UP_EAT_SPACE
			mov		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			sub		esi, 2 * MAP_WIDTH
			jmp		loc_3D835

@@l_space:	mov		edx, offset stru_4041E
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi - 1], MURPHY_ANIM_MOVE_LEFT_EAT_SPACE
			mov		byte ptr _levelmap[esi - 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			sub		esi, 2
			jmp		loc_3D835

@@d_space:	cmp		[PlayerIsLookingLeft], 0
			je		short loc_3CF90
			mov		edx, offset stru_4042E
			jmp		short loc_3CF93
loc_3CF90:	mov		edx, offset stru_4043E
loc_3CF93:  mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], MURPHY_ANIM_MOVE_DOWN_EAT_SPACE
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			add		esi, 2 * MAP_WIDTH
			jmp		loc_3D835

@@r_space:	mov		edx, offset stru_4044E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 3], MURPHY_ANIM_MOVE_RIGHT_EAT_SPACE
			mov		byte ptr _levelmap[esi + 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_MURPHY
			add		esi, 2
			jmp		loc_3D835

@@u_bug:	cmp		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 0
			jl		short @@u_bug_na
			call	CreateExplosion
			ret
@@u_bug_na:	mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BASE
@@u_base:	call	_SND_PlayBaseEatSound
			cmp		[PlayerIsLookingLeft], 0
			je		short loc_3D002
			mov		edx, offset stru_4046E
			jmp		short loc_3D005
loc_3D002:	mov		edx, offset stru_4047E
loc_3D005:	mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], MURPHY_ANIM_MOVE_UP_EAT_BASE
			mov		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			sub		esi, 2 * MAP_WIDTH
			jmp		loc_3D835

@@l_bug:	cmp		byte ptr _levelmap[esi - 1], 0
			jl		short @@l_bug_na
			call	CreateExplosion
			ret
@@l_bug_na:	mov		word ptr _levelmap[esi - 2], MAP_BASE
@@l_base:	call	_SND_PlayBaseEatSound
			mov		edx, offset stru_4048E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi - 1], MURPHY_ANIM_MOVE_LEFT_EAT_SPACE	; FIXME: MURPHY_ANIM_MOVE_LEFT_EAT_BASE ???
			mov		byte ptr _levelmap[esi - 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			sub		esi, 2
			jmp		loc_3D835

@@d_bug:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 0
			jl		short @@d_bug_na
			call	CreateExplosion
			ret
@@d_bug_na:	mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_BASE
@@d_base:	call	_SND_PlayBaseEatSound
			cmp		[PlayerIsLookingLeft], 0
			jz		short loc_3D088
			mov		edx, offset stru_4049E
			jmp		short loc_3D08B
loc_3D088:	mov		edx, offset stru_404AE
loc_3D08B:	mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], MURPHY_ANIM_MOVE_DOWN_EAT_BASE
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			add		esi, 2 * MAP_WIDTH
			jmp		loc_3D835

@@r_bug:	cmp		byte ptr _levelmap[esi + 3], 0
			jl		short @@r_bug_na
			call	CreateExplosion
			ret
@@r_bug_na:	mov		word ptr _levelmap[esi + 2], MAP_BASE
@@r_base:  	call	_SND_PlayBaseEatSound
			mov		edx, offset stru_404BE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 3], MURPHY_ANIM_MOVE_RIGHT_EAT_BASE
			mov		byte ptr _levelmap[esi + 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			add		esi, 2
			jmp		loc_3D835

@@et_bug:	cmp		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 0
			jl		short @@et_bug_na
			call	CreateExplosion
			ret
@@et_bug_na:mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BASE
@@et_base:	call	_SND_PlayBaseEatSound
			mov		edx, offset stru_404CE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BASE_TOP
			jmp		loc_3D835

@@el_bug:	cmp		byte ptr _levelmap[esi - 1], 0
			jl		short @@el_bug_na
			call	CreateExplosion
			ret
@@el_bug_na:mov		word ptr _levelmap[esi - 2], MAP_BASE
@@el_base:	call	_SND_PlayBaseEatSound
			mov		edx, offset stru_404DE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BASE_LEFT
			jmp		loc_3D835

@@eb_bug:	cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 0
			jl		short @@eb_bug_na
			call	CreateExplosion
			ret
@@eb_bug_na:mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_BASE
@@eb_base:	call	_SND_PlayBaseEatSound
			mov		edx, offset stru_404EE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BASE_BOTTOM
			jmp		loc_3D835

@@er_bug:	cmp		byte ptr _levelmap[esi + 3], 0
			jl		short @@er_bug_na
			call	CreateExplosion
			ret
@@er_bug_na:mov		word ptr _levelmap[esi + 2], MAP_BASE
@@er_base:	call	_SND_PlayBaseEatSound
			mov		edx, offset stru_404FE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BASE_RIGHT
			jmp		loc_3D835

@@u_inftr:	call	_SND_PlayEatInfotronSound
			cmp		[PlayerIsLookingLeft], 0
			je		short loc_3D1F5
			mov		edx, offset stru_4050E
			jmp		short loc_3D1F8
loc_3D1F5:	mov		edx, offset stru_4051E
loc_3D1F8:	mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], MURPHY_ANIM_MOVE_UP_EAT_INFTR
			mov		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			sub		esi, 2 * MAP_WIDTH
			jmp		loc_3D835

@@l_inftr:	call	_SND_PlayEatInfotronSound
			mov		edx, offset stru_4052E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi - 1], MURPHY_ANIM_MOVE_LEFT_EAT_INFTR
			mov		byte ptr _levelmap[esi - 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			sub		esi, 2
			jmp		loc_3D835

@@d_inftr:	call	_SND_PlayEatInfotronSound
			cmp		[PlayerIsLookingLeft], 0
			je		short loc_3D259
			mov		edx, offset stru_4053E
			jmp		short loc_3D25C
loc_3D259:	mov		edx, offset stru_4054E
loc_3D25C:	mov		word ptr [PlayerIsInsideTeleport], 0
			mov		word ptr [RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], MURPHY_ANIM_MOVE_DOWN_EAT_INFTR
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			add		esi, 2 * MAP_WIDTH
			jmp		loc_3D835

@@r_inftr:	call	_SND_PlayEatInfotronSound
			mov		edx, offset stru_4055E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 3], MURPHY_ANIM_MOVE_RIGHT_EAT_INFTR
			mov		byte ptr _levelmap[esi + 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			add		esi, 2
			jmp		loc_3D835

@@et_inftr:	call	_SND_PlayEatInfotronSound
			mov		edx, offset stru_4056E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_INFOTRON_TOP
			mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 0FFh
			jmp		loc_3D835

@@el_inftr:	call	_SND_PlayEatInfotronSound
			mov		edx, offset stru_4057E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_INFOTRON_LEFT
			mov		byte ptr _levelmap[esi - 1], 0FFh
			jmp		loc_3D835

@@eb_inftr:	call	_SND_PlayEatInfotronSound
			mov		edx, offset stru_4058E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_INFOTRON_BOTTOM
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 0FFh
			jmp		loc_3D835

@@er_inftr:	call	_SND_PlayEatInfotronSound
			mov		edx, offset stru_4059E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_INFOTRON_RIGHT
			mov		byte ptr _levelmap[esi + 3], 0FFh
			jmp		loc_3D835

@@levelexit:cmp		[InfotronsLeftToCollect], 0
			jne		@@done
			call	_SND_PlayExitSound
			push	esi
;			cmp		[IsDemoRunning], 0
;			jne		short loc_3D397
			mov		[byte_403AB], 1
			mov		[byte_403AA], 0
;			call	sub_3BF39
;			jmp		short loc_3D39C
;loc_3D397:	mov		byte ptr [byte_403AA], 0
;loc_3D39C:	
			mov		word ptr [TimeoutAfterWhichLeaveTheLevel], 64
			pop		esi
			mov		edx, offset stru_4045E
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EXIT
			jmp		loc_3D835

@@l_zonk:	mov		ax, word ptr _levelmap[esi - 4]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		byte ptr _levelmap[esi - 3], MAP_ZONK
			mov		edx, offset stru_405AE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 8
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_SHIFT_ZONK_LEFT
			jmp		loc_3D835

@@r_zonk:	mov		ax, word ptr _levelmap[esi + 4]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		ax, word ptr _levelmap[esi + 2 * MAP_WIDTH + 2]
			cmp		ax, MAP_SPACE
			je		@@done
			mov		byte ptr _levelmap[esi + 5], MAP_ZONK
			mov		edx, offset stru_405BE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 8
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_SHIFT_ZONK_RIGHT
			jmp		loc_3D835

@@terminal:	cmp		byte ptr [TerminalsActivated], 0
			je		@@term_act
			mov		word ptr [SupaplexYawnTimeout], 10
			ret
@@term_act:	mov		byte ptr [byte_40C5A], 7
			mov		byte ptr [TerminalsActivated], 1
			push	esi
			xor		esi, esi
			mov		ecx, MAP_WIDTH * MAP_HEIGHT
@@t_chk_dsk:cmp		word ptr _levelmap[esi], MAP_DISK_YELLOW
			jne		short @@t_next
			push	ecx
			push	esi
			call	CreateExplosion
			pop		esi
			pop		ecx
@@t_next:	add		esi, 2
			loop	@@t_chk_dsk
			pop		esi
			ret

@@u_port:	cmp		word ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_SPACE
			jne		@@done
			mov		edx, offset stru_405CE
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 1
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_TELEPORT_UP
			mov		byte ptr _levelmap[esi - (4 * MAP_WIDTH - 1)], MAP_MURPHY
			jmp		loc_3D835

@@l_port:	cmp		word ptr _levelmap[esi - 4], MAP_SPACE
			jne		@@done
			mov		edx, offset stru_405DE
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 1
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_TELEPORT_LEFT
			mov		byte ptr _levelmap[esi - 3], MAP_MURPHY
			jmp		loc_3D835

@@d_port:	cmp		word ptr _levelmap[esi + (4 * MAP_WIDTH)], 0
			jne		@@done
			mov		edx, offset stru_405EE
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 1
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_TELEPORT_DOWN
			mov		byte ptr _levelmap[esi + MAP_WIDTH * 4 + 1], MAP_MURPHY
			jmp		loc_3D835

@@r_port:	cmp		word ptr _levelmap[esi + 4], 0
			jne		@@done
			mov		edx, offset stru_405FE
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 1
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_TELEPORT_RIGHT
			mov		byte ptr _levelmap[esi + 5], MAP_MURPHY
			jmp		loc_3D835

@@u_rdisk:	cmp		[PlayerIsLookingLeft], 0
			je		short loc_3D5D2
			mov		edx, offset stru_4060E
			jmp		short loc_3D5D5
loc_3D5D2:	mov		edx, offset stru_4061E
loc_3D5D5:	mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_MOVE_UP_EAT_BOMB
			mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], MAP_MURPHY
			jmp		loc_3D835

@@l_rdisk:	mov		edx, offset stru_4062E
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi - 1], MURPHY_ANIM_MOVE_LEFT_EAT_BOMB
			mov		byte ptr _levelmap[esi - 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			sub		esi, 2
			jmp		loc_3D835

@@d_rdisk:	cmp		[PlayerIsLookingLeft], 0
			je		short loc_3D623
			mov		edx, offset stru_4063E
			jmp		short loc_3D626
loc_3D623:	mov		edx, offset stru_4064E
loc_3D626:	mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_MOVE_DOWN_EAT_BOMB
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], MAP_MURPHY
			jmp		loc_3D835

@@r_rdisk:	mov		edx, offset stru_4065E
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 3], MURPHY_ANIM_MOVE_RIGHT_EAT_BOMB
			mov		byte ptr _levelmap[esi + 2], MAP_MURPHY
			mov		byte ptr _levelmap[esi + 1], MAP_MURPHY
			mov		byte ptr _levelmap[esi], MAP_SPACE
			add		esi, 2
			jmp		loc_3D835

@@et_rdisk:	mov		edx, offset stru_4066E
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BOMB_TOP
			mov		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], MAP_MURPHY
			jmp		loc_3D835

@@el_rdisk:	mov		edx, offset stru_4067E
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BOMB_LEFT
			mov		byte ptr _levelmap[esi - 1], MAP_MURPHY
			jmp		loc_3D835

@@eb_rdisk:	mov		edx, offset stru_4068E
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BOMB_BOTTOM
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], MAP_MURPHY
			jmp		loc_3D835

@@er_rdisk:	mov		edx, offset stru_4069E
			mov		[RedDiskPlacementTimer], 0
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_EAT_BOMB_RIGHT
			mov		byte ptr _levelmap[esi + 3], MAP_MURPHY
			jmp		loc_3D835

@@u_ydisk:	mov		ax, word ptr _levelmap[esi - (4 * MAP_WIDTH)]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		byte ptr _levelmap[esi - (4 * MAP_WIDTH - 1)], MAP_DISK_YELLOW
			mov		edx, offset stru_406AE
			mov		[RedDiskPlacementTimer], 8
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_MOVE_YELLOW_DISK_UP
			jmp		loc_3D835

@@l_ydisk:	mov		ax, word ptr _levelmap[esi - 4]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		byte ptr _levelmap[esi - 3], MAP_DISK_YELLOW
			mov		edx, offset stru_406BE
			mov		[RedDiskPlacementTimer], 8
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_MOVE_YELLOW_DISK_LF
			jmp		loc_3D835

@@d_ydisk:	mov		ax, word ptr _levelmap[esi + (4 * MAP_WIDTH)]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		byte ptr _levelmap[esi + MAP_WIDTH * 4 + 1], MAP_DISK_YELLOW
			mov		edx, offset stru_406CE
			mov		[RedDiskPlacementTimer], 8
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_MOVE_YELLOW_DISK_DN
			jmp		loc_3D835

@@r_ydisk:	mov		ax, word ptr _levelmap[esi + 4]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		byte ptr _levelmap[esi + 5], MAP_DISK_YELLOW
			mov		edx, offset stru_406DE
			mov		[RedDiskPlacementTimer], 8
			mov		[PlayerIsInsideTeleport], 0
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_MOVE_YELLOW_DISK_RT
			jmp		loc_3D835

@@l_odisk:	mov		ax, word ptr _levelmap[esi - 4]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		byte ptr _levelmap[esi - 3], MAP_DISK_ORANGE
			mov		edx, offset stru_406EE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 8
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_SHIFT_ORANGE_DISK_LF
			jmp		short loc_3D835

@@r_odisk:	mov		ax, word ptr _levelmap[esi + 4]
			cmp		ax, MAP_SPACE
			jne		@@done
			mov		ax, word ptr _levelmap[esi + 2 * MAP_WIDTH + 2]
			cmp		ax, MAP_SPACE
			je		@@done
			mov		byte ptr _levelmap[esi + 5], MAP_ZONK		; FIXME: MAP_DISK_ORANGE ???
			mov		edx, offset stru_406FE
			mov		[PlayerIsInsideTeleport], 0
			mov		[RedDiskPlacementTimer], 8
			mov		byte ptr _levelmap[esi + 1], MURPHY_ANIM_SHIFT_ORANGE_DISK_RT
			jmp		short loc_3D835

loc_3D835:	mov 	ax, [edx + struc_2.animID]
			mov 	[PlayerAnim_ID], ax
			mov 	ax, [edx + struc_2.stepX]
			mov 	[PlayerAnim_StepX], ax
			mov 	ax, [edx + struc_2.stepY]
			mov 	[PlayerAnim_StepY], ax
			mov 	ax, [edx + struc_2.numFrames]
			mov 	[PlayerAnim_NumFrames], ax
loc_3D84B:	mov		word ptr [SupaplexYawnTimeout], 0
			mov		ax, word ptr [RedDiskPlacementTimer]
			cmp		ax, 0
			je		loc_3D8AF
			dec		ax
			mov		word ptr [RedDiskPlacementTimer], ax
			jnz		short loc_3D862
			call	_SND_PlayObjectPushSound
loc_3D862:	mov		bl, byte ptr _levelmap[esi + 1]
			cmp		bl, MURPHY_ANIM_SHIFT_ZONK_LEFT
			je		@@do_l_zonk
			cmp		bl, MURPHY_ANIM_SHIFT_ZONK_RIGHT
			je		@@do_r_zonk
			cmp		bl, MURPHY_ANIM_SHIFT_ORANGE_DISK_LF
			je		@@do_l_odsk
			cmp		bl, MURPHY_ANIM_SHIFT_ORANGE_DISK_RT
			je		@@do_r_odsk
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_UP
			je		@@do_u_ydsk
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_LF
			je		@@do_l_ydsk
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_DN
			je		@@do_d_ydsk
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_RT
			je		@@do_r_ydsk
			cmp		bl, MURPHY_ANIM_PUT_BOMB
			je		@@do_ptbomb
			ret

@@do_l_zonk:mov		bl, [_joystickbuttons]
			cmp		bl, JB_LEFT
			jne		short @@lz_cancel
			cmp		word ptr _levelmap[esi - 2], MAP_ZONK
			je		short locret_FFF
@@lz_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		word ptr _levelmap[esi - 2], MAP_ZONK
			cmp		byte ptr _levelmap[esi - 4], MAP_BOOM
			je		short locret_FFF
			mov		word ptr _levelmap[esi - 4], MAP_SPACE
locret_FFF:	ret

@@do_r_zonk:mov		bl, [_joystickbuttons]
			cmp		bl, JB_RIGHT
			jne		short @@rz_cancel
			cmp		word ptr _levelmap[esi + 2], MAP_ZONK
			je		short locret_GGG
@@rz_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		word ptr _levelmap[esi + 2], MAP_ZONK
			cmp		byte ptr _levelmap[esi + 4], MAP_BOOM
			je		short locret_GGG
			mov		word ptr _levelmap[esi + 4], MAP_SPACE
locret_GGG:	ret

@@do_l_odsk:mov		bl, [_joystickbuttons]
			cmp		bl, JB_LEFT
			jne		short @@lo_cancel
			cmp		word ptr _levelmap[esi - 2], MAP_DISK_ORANGE
			je		short locret_HHH
@@lo_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		word ptr _levelmap[esi - 2], MAP_DISK_ORANGE
			cmp		byte ptr _levelmap[esi - 4], MAP_BOOM
			je		short locret_HHH
			mov		word ptr _levelmap[esi - 4], MAP_SPACE
locret_HHH:	ret

@@do_r_odsk:mov		bl, [_joystickbuttons]
			cmp		bl, JB_RIGHT
			jne		short @@ro_cancel
			cmp		word ptr _levelmap[esi + 2], MAP_DISK_ORANGE
			je		short locret_III
@@ro_cancel:mov		word ptr _levelmap[esi], 3
			mov		word ptr _levelmap[esi + 2], MAP_DISK_ORANGE
			cmp		byte ptr _levelmap[esi + 4], MAP_BOOM
			jz		short locret_III
			mov		word ptr _levelmap[esi + 4], MAP_SPACE
locret_III:	ret

@@do_u_ydsk:mov		bl, [_joystickbuttons]
			cmp		bl, JB_UP
			jne		short @@uy_cancel
			cmp		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_DISK_YELLOW
			je		short locret_JJJ
@@uy_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_DISK_YELLOW
			cmp		byte ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_BOOM
			je		short locret_JJJ
			mov		word ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_SPACE
locret_JJJ:	ret

@@do_l_ydsk:mov		bl, [_joystickbuttons]
			cmp		bl, JB_LEFT
			jne		short @@ly_cancel
			cmp		word ptr _levelmap[esi - 2], MAP_DISK_YELLOW
			je		short locret_KKK
@@ly_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		word ptr _levelmap[esi - 2], MAP_DISK_YELLOW
			cmp		byte ptr _levelmap[esi - 4], MAP_BOOM
			je		short locret_KKK
			mov		word ptr _levelmap[esi - 4], MAP_SPACE
locret_KKK:	ret

@@do_d_ydsk:mov		bl, [_joystickbuttons]
			cmp		bl, 3
			jne		short @@dy_cancel
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_YELLOW
			je		short locret_LLL
@@dy_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_DISK_YELLOW
			cmp		byte ptr _levelmap[esi + (4 * MAP_WIDTH)], MAP_BOOM
			je		short locret_LLL
			mov		word ptr _levelmap[esi + (4 * MAP_WIDTH)], MAP_SPACE
locret_LLL:	ret

@@do_r_ydsk:mov		bl, [_joystickbuttons]
			cmp		bl, JB_RIGHT
			jne		short @@ry_cancel
			cmp		word ptr _levelmap[esi + 2], MAP_DISK_YELLOW
			je		short locret_MMM
@@ry_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		word ptr _levelmap[esi + 2], MAP_DISK_YELLOW
			cmp		byte ptr _levelmap[esi + 4], MAP_BOOM
			je		short locret_MMM
			mov		word ptr _levelmap[esi + 4], MAP_SPACE
locret_MMM: ret

@@do_ptbomb:mov		bl, [_joystickbuttons]
			cmp		bl, JB_SPACE
			jne		short @@pb_cancel
			cmp		[RedDiskPlacementTimer], 32
			jg		short locret_NNN
			mov		[RedDiskDetonateTimer], 1
locret_NNN:	ret
@@pb_cancel:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		[RedDiskDetonateTimer], 0
			ret

loc_3D8AF:	mov		ax, word ptr [PlayerAnim_StepX]
			add		word ptr [PlayerPosition_PixelsX], ax
			mov		ax, word ptr [PlayerAnim_StepY]
			add		word ptr [PlayerPosition_PixelsY], ax
			dec		word ptr [PlayerAnim_NumFrames]
			cmp		word ptr [PlayerAnim_NumFrames], 0
			jne		@@done
			xor		bl, bl
			xchg	bl, byte ptr _levelmap[esi + 1]
			cmp		bl, MURPHY_ANIM_MOVE_UP_EAT_SPACE
			je		@@d_u_move
			cmp		bl, MURPHY_ANIM_MOVE_LEFT_EAT_SPACE
			je		@@d_l_move
			cmp		bl, MURPHY_ANIM_MOVE_DOWN_EAT_SPACE
			je		@@d_d_move
			cmp		bl, MURPHY_ANIM_MOVE_RIGHT_EAT_SPACE
			je		@@d_r_move
			cmp		bl, MURPHY_ANIM_MOVE_UP_EAT_BASE
			je		@@d_u_move
			cmp		bl, MURPHY_ANIM_MOVE_LEFT_EAT_BASE
			je		@@d_l_move
			cmp		bl, MURPHY_ANIM_MOVE_DOWN_EAT_BASE
			je		@@d_d_move
			cmp		bl, MURPHY_ANIM_MOVE_RIGHT_EAT_BASE
			je		@@d_r_move
			cmp		bl, MURPHY_ANIM_MOVE_UP_EAT_INFTR
			je		@@d_u_inftr
			cmp		bl, MURPHY_ANIM_MOVE_LEFT_EAT_INFTR
			je		@@d_l_inftr
			cmp		bl, MURPHY_ANIM_MOVE_DOWN_EAT_INFTR
			je		@@d_d_inftr
			cmp		bl, MURPHY_ANIM_MOVE_RIGHT_EAT_INFTR
			je		@@d_r_inftr
			cmp		bl, MURPHY_ANIM_SHIFT_ZONK_LEFT
			je		@@d_l_zonk
			cmp		bl, MURPHY_ANIM_SHIFT_ZONK_RIGHT
			je		@@d_r_zonk
			cmp		bl, MURPHY_ANIM_EAT_BASE_TOP
			je		@@d_et_base
			cmp		bl, MURPHY_ANIM_EAT_BASE_LEFT
			je		@@d_el_base
			cmp		bl, MURPHY_ANIM_EAT_BASE_RIGHT
			je		@@d_er_base
			cmp		bl, MURPHY_ANIM_EAT_BASE_BOTTOM
			je		@@d_eb_base
			cmp		bl, MURPHY_ANIM_EAT_INFOTRON_TOP
			je		@@d_et_inft
			cmp		bl, MURPHY_ANIM_EAT_INFOTRON_LEFT
			je		@@d_el_inft
			cmp		bl, MURPHY_ANIM_EAT_INFOTRON_RIGHT
			je		@@d_er_inft
			cmp		bl, MURPHY_ANIM_EAT_INFOTRON_BOTTOM
			je		@@d_eb_inft
			cmp		bl, MURPHY_ANIM_EXIT
			je		@@d_levexit
			cmp		bl, MURPHY_ANIM_TELEPORT_UP
			je		@@d_u_port
			cmp		bl, MURPHY_ANIM_TELEPORT_LEFT
			je		@@d_l_port
			cmp		bl, MURPHY_ANIM_TELEPORT_DOWN
			je		@@d_d_port
			cmp		bl, MURPHY_ANIM_TELEPORT_RIGHT
			je		@@d_r_port
			cmp		bl, MURPHY_ANIM_MOVE_UP_EAT_BOMB
			je		@@d_u_bomb
			cmp		bl, MURPHY_ANIM_MOVE_LEFT_EAT_BOMB
			je		@@d_l_bomb
			cmp		bl, MURPHY_ANIM_MOVE_DOWN_EAT_BOMB
			je		@@d_d_bomb
			cmp		bl, MURPHY_ANIM_MOVE_RIGHT_EAT_BOMB
			je		@@d_r_bomb
			cmp		bl, MURPHY_ANIM_EAT_BOMB_TOP
			je		@@d_et_bomb
			cmp		bl, MURPHY_ANIM_EAT_BOMB_LEFT
			je		@@d_el_bomb
			cmp		bl, MURPHY_ANIM_EAT_BOMB_BOTTOM
			je		@@d_eb_bomb
			cmp		bl, MURPHY_ANIM_EAT_BOMB_RIGHT
			je		@@d_er_bomb
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_UP
			je		@@d_u_ydisk
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_LF
			je		@@d_l_ydisk
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_DN
			je		@@d_d_ydisk
			cmp		bl, MURPHY_ANIM_MOVE_YELLOW_DISK_RT
			je		@@d_r_ydisk
			cmp		bl, MURPHY_ANIM_SHIFT_ORANGE_DISK_LF
			je		@@d_l_odisk
			cmp		bl, MURPHY_ANIM_SHIFT_ORANGE_DISK_RT
			je		@@d_r_odisk
			cmp		bl, MURPHY_ANIM_PUT_BOMB	
			je		@@d_putbomb
			mov		word ptr [PlayerCompletedLevel], 1
			ret

@@d_u_inftr:cmp		[InfotronsLeftToCollect],	0
			jbe		short @@d_u_move
			dec		[InfotronsLeftToCollect]
@@d_u_move:	mov		word ptr _levelmap[esi], MAP_MURPHY
			add		esi, 2 * MAP_WIDTH
			call	sub_38191
			sub		esi, 2 * MAP_WIDTH
			ret

@@d_l_inftr:cmp		[InfotronsLeftToCollect],	0
			jbe		short @@d_l_move
			dec		[InfotronsLeftToCollect]
@@d_l_move:	mov		word ptr _levelmap[esi], MAP_MURPHY
			add		esi, 2
			call	sub_38191
			sub		esi, 2
			ret

@@d_d_inftr:cmp		[InfotronsLeftToCollect],	0
			jbe		short @@d_d_move
			dec		[InfotronsLeftToCollect]
@@d_d_move:	cmp		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BOOM
			je		short loc_3DB31
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_SPACE
loc_3DB31:	mov		word ptr _levelmap[esi], MAP_MURPHY
			ret

@@d_l_zonk:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DB45
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DB45:	mov		word ptr _levelmap[esi - 2], MAP_MURPHY
			mov		word ptr _levelmap[esi - 4], MAP_ZONK
			sub		esi, 4
			call	sub_3DB7E
			add		esi, 2
			ret

@@d_r_zonk:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DB68
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DB68:	mov		word ptr _levelmap[esi + 2], MAP_MURPHY
			mov		word ptr _levelmap[esi + 4], MAP_ZONK
			add		esi, 4
			call	sub_3DB7E
			sub		esi, 2
			ret

@@d_r_inftr:cmp		[InfotronsLeftToCollect], 0
			jbe		short @@d_r_move
			dec		[InfotronsLeftToCollect]
@@d_r_move:	sub		esi, 2
			call	sub_38191
			add		esi, 2
			mov		word ptr _levelmap[esi], MAP_MURPHY
			ret

@@d_et_inft:cmp		[InfotronsLeftToCollect],	0
			jbe		short @@d_et_base
			dec		[InfotronsLeftToCollect]
@@d_et_base:cmp		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BOOM
			je		short locret_XXX
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_SPACE
locret_XXX:	ret


@@d_el_inft:cmp		[InfotronsLeftToCollect], 0
			jbe		short @@d_el_base
			dec		[InfotronsLeftToCollect]
@@d_el_base:cmp		byte ptr _levelmap[esi - 2], MAP_BOOM
			je		short locret_YYY
			mov		word ptr _levelmap[esi - 2], MAP_SPACE
locret_YYY:	ret

@@d_eb_inft:cmp		[InfotronsLeftToCollect], 0
			jbe		short @@d_eb_base
			dec		[InfotronsLeftToCollect]
@@d_eb_base:cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_BOOM
			je		short locret_ZZZ
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
locret_ZZZ:	ret

@@d_er_inft:cmp		[InfotronsLeftToCollect],0
			jbe		short @@d_er_base
			dec		[InfotronsLeftToCollect]
@@d_er_base:cmp		byte ptr _levelmap[esi + 2], MAP_BOOM
			je		short locret_AAA
			mov		word ptr _levelmap[esi + 2], MAP_SPACE
locret_AAA:	ret

@@d_u_port:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DE71
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DE71:	mov		word ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_MURPHY
			mov		[PlayerIsInsideTeleport], 0
			sub		esi, 4 * MAP_WIDTH
			cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 1], 1
			jne		short locret_BBB
			add		esi, 2 * MAP_WIDTH
			call	sub_3E190
			sub		esi, 2 * MAP_WIDTH
locret_BBB:	ret

@@d_l_port:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DE9F
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DE9F:	mov		word ptr _levelmap[esi - 4], MAP_MURPHY
			mov		[PlayerIsInsideTeleport], 0
			sub		esi, 4
			cmp		byte ptr _levelmap[esi + 3], 1
			jne		short locret_CCC
			add		esi, 2
			call	sub_3E190
			sub		esi, 2
locret_CCC:	ret

@@d_d_port:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DECC
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DECC:	mov		word ptr _levelmap[esi + (4 * MAP_WIDTH)], MAP_MURPHY
			mov		[PlayerIsInsideTeleport], 0
			add		esi, 4 * MAP_WIDTH
			cmp		byte ptr _levelmap[esi - (2 * MAP_WIDTH - 1)], 1
			jne		short locret_DDD
			sub		esi, 2 * MAP_WIDTH
			call	sub_3E190
			add		esi, 2 * MAP_WIDTH
locret_DDD:	ret

@@d_r_port:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DEFA
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DEFA:	mov		word ptr _levelmap[esi + 4], 3
			mov		[PlayerIsInsideTeleport], 0
			add		esi, 4
			cmp		byte ptr _levelmap[esi - 1], 1
			jne		short locret_EEE
			sub 	esi, 2
			call	sub_3E190
			add		esi, 2
locret_EEE:	ret

@@d_u_bomb:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DF27
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DF27:	mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
			inc		[NumRedDisks]
			sub		esi, 2 * MAP_WIDTH
			ret

@@d_l_bomb:	cmp		byte ptr _levelmap[esi + 2], MAP_BOOM
			je		short loc_3DF45
			mov		word ptr _levelmap[esi + 2], MAP_SPACE
loc_3DF45:	mov		word ptr _levelmap[esi], MAP_MURPHY
			inc		[NumRedDisks]
			ret

@@d_d_bomb:	cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DF60
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DF60:	mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			inc		[NumRedDisks]
			add		esi, 2 * MAP_WIDTH
			ret

@@d_r_bomb:	cmp		byte ptr _levelmap[esi - 2], MAP_BOOM
			je		short loc_3DF7E
			mov		word ptr _levelmap[esi - 2], MAP_SPACE
loc_3DF7E:	mov		word ptr _levelmap[esi], MAP_MURPHY
			inc		[NumRedDisks]
			ret

@@d_et_bomb:cmp		byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BOOM
			je		short loc_3DF99
			mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_SPACE
loc_3DF99:	inc		[NumRedDisks]
			ret

@@d_el_bomb:cmp		byte ptr _levelmap[esi - 2], MAP_BOOM
			je		short loc_3DFAE
			mov		word ptr _levelmap[esi - 2], MAP_SPACE
loc_3DFAE:	inc		[NumRedDisks]
			ret

@@d_eb_bomb:cmp		byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_BOOM
			je		short loc_3DFC3
			mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_SPACE
loc_3DFC3:	inc		[NumRedDisks]
			ret

@@d_er_bomb:cmp		byte ptr _levelmap[esi + 2], MAP_BOOM
			je		short loc_3DFD8
			mov		word ptr _levelmap[esi + 2], MAP_SPACE
loc_3DFD8:	inc		[NumRedDisks]
			ret

@@d_u_ydisk:cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3DFED
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3DFED:	mov		word ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_MURPHY
			mov		word ptr _levelmap[esi - (4 * MAP_WIDTH)], MAP_DISK_YELLOW
			sub		esi, 2 * MAP_WIDTH
			ret

@@d_l_ydisk:cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3E00A
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3E00A:	mov		word ptr _levelmap[esi - 2], MAP_MURPHY
			mov		word ptr _levelmap[esi - 4], MAP_DISK_YELLOW
			sub		esi, 2
			ret

@@d_d_ydisk:cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3E027
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3E027:	mov		word ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_MURPHY
			mov		word ptr _levelmap[esi + (4 * MAP_WIDTH)], MAP_DISK_YELLOW
			add		esi, 2 * MAP_WIDTH
			ret

@@d_r_ydisk:cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3E044
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3E044:	mov		word ptr _levelmap[esi + 2], MAP_MURPHY
			mov		word ptr _levelmap[esi + 4], MAP_DISK_YELLOW
			add		esi, 2
			ret

@@d_l_odisk:cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3E061
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3E061:	mov		word ptr _levelmap[esi - 2], MAP_MURPHY
			mov		word ptr _levelmap[esi - 4], MAP_DISK_ORANGE
			sub		esi, 2
			ret

@@d_r_odisk:cmp		byte ptr _levelmap[esi], MAP_BOOM
			je		short loc_3E07E
			mov		word ptr _levelmap[esi], MAP_SPACE
loc_3E07E:	mov		word ptr _levelmap[esi + 2], MAP_MURPHY
			mov		word ptr _levelmap[esi + 4], MAP_DISK_ORANGE
			cmp		word ptr _levelmap[esi + 2 * MAP_WIDTH + 4], MAP_SPACE
			jne		short loc_3E09B
			mov		byte ptr _levelmap[esi + 5], 20h
			mov		byte ptr _levelmap[esi + 2 * MAP_WIDTH + 5], MAP_DISK_ORANGE
loc_3E09B:	add		esi, 2
			ret

@@d_levexit:mov		[PlayerCompletedLevel], 1
			ret

@@d_putbomb:mov		word ptr _levelmap[esi], MAP_MURPHY
			mov		[RedDiskDetonateTimer], 2
			dec		[NumRedDisks]
			call	_SND_PlayObjectPushSound
			ret

@@done:		ret

HandlePlayerMovement endp

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



; ллллллллллллллл S U B	R O U T	I N E ллллллллллллллллллллллллллллллллллллллл

sub_3E0D7	proc near

		cmp	ax, 0FFFFh
		je	short loc_3E14E
		cmp	ax, 0AAAAh
		je	short loc_3E14E
		cmp	ah, 0
		je	short loc_3E14E
		cmp	al, MAP_ZONK
		je	short loc_3E107
		cmp	al, MAP_BOOM
		je	short loc_3E150
		cmp	al, MAP_DISK_ORANGE
		je	short loc_3E14E
		cmp	al, MAP_PORT_LEFT_TO_RIGHT
		je	short loc_3E14E
		cmp	al, MAP_PORT_UP_TO_DOWN
		je	short loc_3E14E
		cmp	al, MAP_PORT_RIGHT_TO_LEFT
		je	short loc_3E14E
		cmp	al, MAP_PORT_DOWN_TO_UP
		je	short loc_3E14E
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


sub_3E190	proc near

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
		mov	ax, word ptr [CurrentFrameNumber]
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
		mov	ax, word ptr [CurrentFrameNumber]
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
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_BOOM
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
		cmp	byte ptr _levelmap[esi + 2], MAP_BOOM
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
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BOOM
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
		cmp	word ptr _levelmap[esi - 2], MAP_SPACE
		jnz	short loc_3E5D5
		mov	byte ptr _levelmap[esi + 1], 0Dh
		ret


loc_3E5D5:
		cmp	byte ptr _levelmap[esi - 2], MAP_MURPHY
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
		cmp	byte ptr _levelmap[esi - 2], MAP_BOOM
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
		mov	ax, word ptr [CurrentFrameNumber]
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

		mov	ax, word ptr [CurrentFrameNumber]
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
		cmp	byte ptr _levelmap[esi + 2 * MAP_WIDTH], MAP_BOOM
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
		cmp	byte ptr _levelmap[esi + 2], MAP_BOOM
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
		cmp	byte ptr _levelmap[esi - 2 * MAP_WIDTH], MAP_BOOM
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
			cmp		byte ptr _levelmap[esi - 2], MAP_BOOM
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
byte_403B0	db 0
byte_403B1	db 0
byte_403B2	db 0

CurrentMurphyOfs	dd 0

_PlayerPosition_Ofs label dword
PlayerPosition_Ofs dd 0

_PlayerIsLookingLeft label word
PlayerIsLookingLeft	dw 0

_SupaplexYawnTimeout label word
SupaplexYawnTimeout	dw 0

word_403BF	dw 0

_PlayerExploded label word
PlayerExploded	dw 0

PlayerDied	dw 0

byte_403C3	db 0
byte_403C7	db 0
byte_403C8	db 0

_PlayerIsInsideTeleport label word
PlayerIsInsideTeleport	dw 0

_RedDiskDetonateTimer label byte
RedDiskDetonateTimer	db 0

_RedDiskPosition label dword
RedDiskPosition	dd 0

IsDemoRunning	db 0

_PlayerPosition_PixelsX label word
PlayerPosition_PixelsX dw 0

_PlayerPosition_PixelsY label word
PlayerPosition_PixelsY dw 0

_RedDiskPlacementTimer label word
RedDiskPlacementTimer	dw 0

PlayerAnim_NumFrames label word
_PlayerAnim_NumFrames dw 0

PlayerAnim_StepX label word
_PlayerAnimStepX dw 0

PlayerAnim_StepY label word
_PlayerAnimStepY dw 0

PlayerAnim_ID label word
_PlayerAnim_ID dw 0

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

MapObjectHandlers label dword
		dd 0
		dd HandleZonk
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

_NumRedDisks label byte
NumRedDisks	db 0

CurrentFrameNumber	dw 0F000h
word_40C4F	dw 0
word_40C51	dw 0
byte_40C5A	db 0
TerminalsActivated	db 0
word_40C60	dw 0
PlayerCompletedLevel	dw 0
TimeoutAfterWhichLeaveTheLevel	dw 0
byte_40C6C	db 0

word_40CF3	dw 0
word_40CF7	dw 1

levelmap label word
_levelmap	dw 1536 dup(0)

ActiveExplosions label byte
_ActiveExplosions	db 1536	dup(0)

RandomSeed	dw 0
word_4C47B	dw 0
word_4C47D	dw 0
word_4C47F	dw 2008 dup(0)
			db 0

joystickbuttons label byte
_joystickbuttons db 0

				; ! Move up (looking left), eat nothing
stru_403FE      dw ANIM_MOVE_UP_L, 8, 0, -2
                ; ! Move up (looking right), eat nothing
stru_4040E      dw ANIM_MOVE_UP_R, 8, 0, -2
                ; ! Move left, eat nothing
stru_4041E      dw ANIM_MOVE_LEFT, 8, -2, 0
                ; ! Move down (looking left), eat nothing
stru_4042E      dw ANIM_MOVE_DOWN_L, 8, 0, 2
                ; ! Move down (looking right), eat nothing
stru_4043E      dw ANIM_MOVE_DOWN_R, 8, 0, 2
                ; ! Move right, eat nothing
stru_4044E      dw ANIM_MOVE_RIGHT, 8, 2, 0
                ; ! Exit the level
stru_4045E      dw ANIM_EXIT, 41, 0, 0
                ; ! Move up (looking left), eat base
stru_4046E      dw ANIM_EATBASE_UP_L, 8, 0, -2
                ; ! Move up (looking right), eat base
stru_4047E      dw ANIM_EATBASE_UP_R, 8, 0, -2
                ; ! Move left, eat base
stru_4048E      dw ANIM_EATBASE_LEFT, 8, -2, 0
                ; ! Move down (looking left), eat base
stru_4049E      dw ANIM_EATBASE_DOWN_L, 8, 0, 2
                ; ! Move down (looking right), eat base
stru_404AE      dw ANIM_EATBASE_DOWN_R, 8, 0, 2
				; ! Move right, eat base
stru_404BE      dw ANIM_EATBASE_RIGHT, 8, 2, 0
                ; ! Eat base at top
stru_404CE      dw ANIM_EATBASE_ATTOP, 8, 0, 0
                ; ! Eat base at left
stru_404DE      dw ANIM_EATBASE_ATLEFT, 8, 0, 0
                ; ! Eat base at bottom
stru_404EE      dw ANIM_EATBASE_ATBOTTOM, 8, 0, 0
                ; ! Eat base at right
stru_404FE      dw ANIM_EATBASE_ATRIGHT, 8, 0, 0
                ; ! Move up (looking left), eat infotron
stru_4050E      dw ANIM_EATINFOTRON_UP_L, 8, 0, -2
                ; ! Move up (looking right), eat infotron
stru_4051E      dw ANIM_EATINFOTRON_UP_R, 8, 0, -2
                ; ! Move left, eat infotron
stru_4052E      dw ANIM_EATINFOTRON_LEFT, 8, -2, 0
                ; ! Move down (looking left), eat infotron
stru_4053E      dw ANIM_EATINFOTRON_DOWN_L, 8, 0, 2
                ; ! Move down (looking right), eat infotron
stru_4054E      dw ANIM_EATINFOTRON_DOWN_R, 8, 0, 2
                ; ! Move right, eat infotron
stru_4055E      dw ANIM_EATINFOTRON_RIGHT, 8, 2, 0
                ; ! Eat infotron at top
stru_4056E      dw ANIM_EATINFOTRON_ATTOP, 8, 0, 0
                ; ! Eat infotron at left
stru_4057E      dw ANIM_EATINFOTRON_ATLEFT, 8, 0, 0
                ; ! Eat infotron at bottom
stru_4058E      dw ANIM_EATINFOTRON_ATBOTTOM, 8, 0, 0
                ; ! Eat infotron at right
stru_4059E      dw ANIM_EATINFOTRON_ATRIGHT, 8, 0, 0
                ; ! Shift zonk left
stru_405AE      dw ANIM_SHIFTZONK_LEFT, 8, -2, 0
                ; ! Shift zonk right
stru_405BE      dw ANIM_SHIFTZONK_RIGHT, 8, 2, 0
                ; + Move up through teleport
stru_405CE      dw ANIM_TELEPORT_UP, 8, 0, -4
                ; + Move left through teleport
stru_405DE      dw ANIM_TELEPORT_LEFT, 8, -4, 0
                ; + Move down through teleport
stru_405EE      dw ANIM_TELEPORT_DOWN, 8, 0, 4
				; + Move right through teleport
stru_405FE      dw ANIM_TELEPORT_RIGHT, 8, 4, 0
                ; ! Move up (looking left), eat red disk
stru_4060E      dw ANIM_EATDISK_UP_L, 8, 0, -2
                ; ! Move up (looking right), eat red disk
stru_4061E      dw ANIM_EATDISK_UP_R, 8, 0, -2
                ; ! Move left, eat red disk
stru_4062E      dw ANIM_EATDISK_LEFT, 8, -2, 0
                ; ! Move down (looking left), eat red disk
stru_4063E      dw ANIM_EATDISK_DOWN_L, 8, 0, 2
                ; ! Move down (looking right), eat red disk
stru_4064E      dw ANIM_EATDISK_DOWN_R, 8, 0, 2
                ; ! Move right, eat red disk
stru_4065E      dw ANIM_EATDISK_RIGHT, 8, 2, 0
                ; ! Eat red disk at top
stru_4066E      dw ANIM_EATDISK_ATTOP, 8, 0, 0 
                ; ! Eat red disk at left
stru_4067E      dw ANIM_EATDISK_ATLEFT, 8, 0, 0
                ; ! Eat red disk at bottom
stru_4068E      dw ANIM_EATDISK_ATBOTTOM, 8, 0, 0
                ; ! Eat red disk at right
stru_4069E      dw ANIM_EATDISK_ATRIGHT, 8, 0, 0
                ; ! Shift yellow disk up
stru_406AE      dw ANIM_SHIFTYELLOWDISK_UP, 8, 0, -2
                ; ! Shift yellow disk left
stru_406BE      dw ANIM_SHIFTYELLOWDISK_LEFT, 8, -2, 0
                ; ! Shift yellow disk down
stru_406CE      dw ANIM_SHIFTYELLOWDISK_DOWN, 8, 0, 2
                ; ! Shift yellow disk right
stru_406DE      dw ANIM_SHIFTYELLOWDISK_RIGHT, 8, 2, 0
                ; ! Shift orange disk left
stru_406EE      dw ANIM_SHIFTORANGEDISK_LEFT, 8, -2, 0
                ; ! Shift orange disk right
stru_406FE      dw ANIM_SHIFTORANGEDISK_RIGHT, 8, 2, 0
                ; ! Place red disk
stru_4070E      dw ANIM_PLACEREDDISK, 2, 0, 0

		end
