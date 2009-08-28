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

extern "C" {
extern short LevelMap[MAP_NUM_CELLS];
extern char JoystickButtons;
extern void LoadLevelMap();
extern void FindPlayerPositionOnLevelMap();
extern void InitRandomNumberGenerator();
extern void sub_392DF();
extern void InitPlayerState();
extern void BeginLevel();
extern bool RunTheLevel();
}

void startLevel()
{
	LoadLevelMap();
	//sub_3C12B();
	FindPlayerPositionOnLevelMap();
	//sub_386E7();
	//VID_DrawBottomPanel();
	sub_392DF();
	InitPlayerState();
}

void paintGameField(HDC hDC)
{
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hFixedBmp);

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			short ch = LevelMap[y * MAP_WIDTH + x];
			if (ch >= 0 && ch < 40)
				StretchBlt(hDC, x * 20, y * 20, 20, 20, hMemDC, ch * 16, 0, 16, 16, SRCCOPY);
			else
			{
				char buf[10];

				sprintf(buf, "%d", ch);
				BitBlt(hDC, x * 20, y * 20, 20, 20, NULL, 0, 0, WHITENESS);
				TextOut(hDC, x * 20, y * 20, buf, strlen(buf));
			}
		}
	}

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool spacePressed = false;
	bool pressed = false;
	PAINTSTRUCT ps;
	HDC hDC;

	switch (msg)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintGameField(hDC);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{
			pressed = true;
			JoystickButtons = 1;
		}	
		if (wParam == VK_LEFT)
		{
			pressed = true;
			JoystickButtons = 2;
		}
		if (wParam == VK_DOWN)
		{
			pressed = true;
			JoystickButtons = 3;
		}
		if (wParam == VK_RIGHT)
		{
			pressed = true;
			JoystickButtons = 4;
		}
		if (wParam == VK_SPACE)
		{
			spacePressed = true;
		}

		if (spacePressed)
		{
			if (pressed)
				JoystickButtons += 4;
			else
				JoystickButtons = 9;
		}
		break;

	case WM_KEYUP:
		if (wParam == VK_SPACE)
			spacePressed = false;
		break;

	case WM_TIMER:
		if (!RunTheLevel())
			PostQuitMessage(0);
		JoystickButtons = 0;
		InvalidateRect(hWnd, NULL, FALSE);
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
	int levelNumber = 11;//27;	
	fseek(f, levelNumber * 1536, SEEK_SET);
	char buf[1536];
	fread(buf, 1536, 1, f);
	for (int i = 0; i < 1536; i++)
		*(unsigned short *)(&LevelMap[i]) = *(unsigned char *)(&buf[i]);
	fclose(f);

	InitPlayerState();
	startLevel();
	BeginLevel();

	hFixedBmp = (HBITMAP)LoadImage(hInstance, "BMP_FIXED", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION);
	hMovingBmp = (HBITMAP)LoadImage(hInstance, "BMP_MOVING", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION);

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

	SetTimer(hWnd, 1, 10, NULL);

	for (;;)
	{
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
