#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <windows.h>

enum MapElements
{
	MAP_SPACE = 0,
	MAP_ZONK,
	MAP_BASE,
	MAP_MURPHY,
	MAP_INFOTRON,
	MAP_RAM_CHIP_SQUARE,			// 5
	MAP_GRAY_DENTED_PYRAMID,
	MAP_EXIT,
	MAP_DISK_ORANGE,
	MAP_PORT_LEFT_TO_RIGHT,
	MAP_PORT_UP_TO_DOWN,			// 10
	MAP_PORT_RIGHT_TO_LEFT,
	MAP_PORT_DOWN_TO_UP,
	MAP_PORT_SPECIAL_LEFT_TO_RIGHT,
	MAP_PORT_SPECIAL_UP_TO_DOWN,
	MAP_PORT_SPECIAL_RIGHT_TO_LEFT,	// 15
	MAP_PORT_SPECIAL_DOWN_TO_UP,
	MAP_SNIK_SNAK,
	MAP_DISK_YELLOW,
	MAP_TERMINAL,
	MAP_DISK_RED,					// 20
	MAP_PORT_VERTICAL,
	MAP_PORT_HORIZONTAL,
	MAP_PORT_CROSS,
	MAP_ELECTRON,
	MAP_BUG,						// 25
	MAP_RAM_CHIP_LEFT,
	MAP_RAM_CHIP_RIGHT,
	MAP_HARDWARE_1,
	MAP_GREEN_LAMP,
	MAP_BLUE_LAMP,					// 30
	MAP_RED_LAMP,
	MAP_YELLOW_BLACK_DIAGONAL,
	MAP_HARDWARE_2,
	MAP_HARDWARE_3,
	MAP_HARDWARE_4,					// 35
	MAP_HARDWARE_5,
	MAP_HARDWARE_6,
	MAP_RAM_CHIP_TOP,
	MAP_RAM_CHIP_BOTTOM,
	MAP_INVISIBLE_WALL,				// 40
};

static HBITMAP hFixedBmp;
static HBITMAP hMovingBmp;

static const int MAP_WIDTH = 60;
static const int MAP_HEIGHT = 24;
static const int MAP_NUM_CELLS = MAP_WIDTH * MAP_HEIGHT;

static char LevelMap[MAP_NUM_CELLS];


void handleSnikSnak(int ofs)
{
//	if (byte_403C7 == 1)
//		return;

	if (LevelMap[ofs] != MAP_SNIK_SNAK)
		return;

	// MapHandlers_1
	//char value = LevelMap[ofs + 1];
	//if (value >= 0 && value <= 7)
	//	sub_3E1F2();
}

void moveObjectsInTheLevel()
{
//	mov	si, PlayerPosition_Ofs
//	call xxx_HandleMurphy
//	mov	PlayerPosition_Ofs, si

	for (int start = 2 * MAP_WIDTH + 2; start < MAP_NUM_CELLS; start++)
	{
		char value = LevelMap[start];
		switch (value)
		{
		case MAP_ZONK:
			//handleZonk(start);
			break;
		case MAP_INFOTRON:
			//handleInfotron(start);
			break;
		case MAP_DISK_ORANGE:
			//xxx_handleOrangeDisk(start);
			break;
		case MAP_SNIK_SNAK:
			handleSnikSnak(start);
			break;
		case MAP_TERMINAL:
			//xxx_HandleTerminal(start);
			break;
		case MAP_ELECTRON:
			//handleElectron(start);
			break;
		case MAP_BUG:
			//xxx_HandleBug(start);
			break;
		case MAP_RED_LAMP:
			//loc_39493(start);
			break;
		}
	}
}

#if 0			
			cmp	word_403C1, 1
		jz	short loc_38F01
		cmp	word_403BF, 0
		jz	short loc_38F01
		retn
; 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

loc_38F01:				; CODE XREF: MoveObjectsInTheLevel+B4j
					; MoveObjectsInTheLevel+BBj
		cmp	word_40C68, 0
		jnz	short loc_38F1B
		mov	word_403C1, 0
		mov	si, word_403B7
		call	sub_3956F
		mov	word_40C68, 40h	; '@'

loc_38F1B:				; CODE XREF: MoveObjectsInTheLevel+C3j
		mov	dx, 3CEh
		mov	al, 5
		out	dx, al		; EGA: graph 1 and 2 addr reg:
					; mode register.Data bits:
					; 0-1: Write mode 0-2
					; 2: test condition
					; 3: read mode:	1=color	compare, 0=direct
					; 4: 1=use odd/even RAM	addressing
					; 5: 1=use CGA mid-res map (2-bits/pixel)
		inc	dx
		mov	al, 1
		out	dx, al		; EGA port: graphics controller	data register
		retn
MoveObjectsInTheLevel endp


PlayerPosition_Ofs dw 0			; DATA XREF: MoveObjectsInTheLevel+Ar
LevelMap db 1440 dup(0)

CODE_SEG ENDS

		end

#endif

void paintGameField(HDC hDC)
{
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hFixedBmp);

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			char ch = LevelMap[y * MAP_WIDTH + x];
			if (ch >= 0 && ch < 40)
				StretchBlt(hDC, x * 20, y * 20, 20, 20, hMemDC, ch * 16, 0, 16, 16, SRCCOPY);
		}
	}

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (msg)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintGameField(hDC);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	FILE * f = fopen("C:/SUPAPLEX/1/levels.dat", "rb");
	if (!f)
		return 1;
//	fseek(f, 2 * 1538, SEEK_SET);
	fread(LevelMap, MAP_NUM_CELLS, 1, f);
	fclose(f);

	hFixedBmp = (HBITMAP)LoadImage(hInstance, "BMP_FIXED", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION);
//	hMovingBmp = (HBITMAP)LoadImage(hInstance, "BMP_MOVING", IMAGE_BITMAP, 0, 0,
//		LR_CREATEDIBSECTION);

	WNDCLASS wc;

	memset(&wc, 0, sizeof(wc));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = MainWndProc;
	wc.lpszClassName = "Supaplex";
	if (!RegisterClass(&wc))
		return 1;

	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = WS_POPUP | WS_CAPTION | WS_BORDER | WS_SYSMENU;

	RECT rc;
	rc.left = 200;
	rc.top = 200;
	rc.right = rc.left + MAP_WIDTH * 20;
	rc.bottom = rc.top + MAP_HEIGHT * 20;
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

	HWND hWnd = CreateWindowEx(dwExStyle, wc.lpszClassName, "Supaplex", dwStyle,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return 1;

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	for (;;)
	{
		moveObjectsInTheLevel();
//		Sleep(10);

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				return 0;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
