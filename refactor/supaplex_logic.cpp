#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
 #include <windows.h>
 #include <mmsystem.h>
#endif

#ifdef USE_GLUT
 #include <GL/glut.h>
 #include <GL/gl.h>
 #include <time.h>
 #include "BMPLoader.h"
#endif

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

#ifdef USE_GLUT

static unsigned speed = 50;

static GLuint texFixed;
static float texFixedStepX;
static float texFixedStepY;
static GLuint texMoving;
static float texMovingStepX;
static float texMovingStepY;
static GLuint texFont;
static float texFontStepX;
static float texFontStepY;

#else

static HBITMAP hFixedBmp;
static HBITMAP hMovingBmp;

static HDC hPaintDC;
static HDC hMemPaintDC;
static HBITMAP hPaintOldBitmap;

#endif

static unsigned long long getTime()
{
#ifdef WIN32
	return timeGetTime();
#else
	struct timespec tv;
	clock_gettime(CLOCK_REALTIME, &tv);
	return (unsigned long long)tv.tv_nsec * 1000000ULL + (unsigned long long)tv.tv_sec;
#endif
}

static const int MAP_WIDTH = 60;
static const int MAP_HEIGHT = 24;
static const int MAP_NUM_CELLS = MAP_WIDTH * MAP_HEIGHT;

static unsigned long long prevTime = 0;

extern "C" {
extern unsigned short PlayerAnim_ID;
extern short PlayerPosition_PixelsX;
extern short PlayerPosition_PixelsY;
extern unsigned short PlayerIsLookingLeft;
extern unsigned short PlayerIsInsideTeleport;
extern unsigned char NumRedDisks;
extern unsigned short SupaplexYawnTimeout;
extern unsigned char RedDiskDetonateTimer;
extern unsigned short RedDiskPlacementTimer;
extern unsigned int RedDiskPosition;
extern unsigned short PlayerAnim_NumFrames;
extern short levelmap[MAP_NUM_CELLS];
extern char joystickbuttons;
extern void loadlevelmap();
extern void findplayerpositiononlevelmap();
extern void initrandomnumbergenerator();
extern void sub_392df();
extern void initplayerstate();
extern void beginlevel();
extern bool runthelevel();
}

void startLevel()
{
	loadlevelmap();
	//sub_3C12B();
	findplayerpositiononlevelmap();
	//sub_386E7();
	//VID_DrawBottomPanel();
	sub_392df();
	initplayerstate();
}

void paintChar(int xx, int yy, unsigned char ch)
{
	if (ch == 0xFF)
	{
		glDisable(GL_TEXTURE_2D);

		glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex2i(xx, yy);
			glVertex2i(xx + 8, yy);
			glVertex2i(xx + 8, yy + 16);
			glVertex2i(xx, yy + 16);
		glEnd();
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texFont);

		unsigned short offset = (unsigned short)ch * 8;
		float x1 = (float)offset * texFontStepX;
		float x2 = (float)(offset + 8) * texFontStepX;

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(x1, 1.0f); glVertex2i(xx, yy);
			glTexCoord2f(x2, 1.0f); glVertex2i(xx + 8, yy);
			glTexCoord2f(x2, 0.0f); glVertex2i(xx + 8, yy + 16);
			glTexCoord2f(x1, 0.0f); glVertex2i(xx, yy + 16);
		glEnd();
	}
}

void paintHex(int xx, int yy, unsigned char value)
{
	paintChar(xx, yy, value >> 4);
	paintChar(xx + 8, yy, value & 0xF);
}

void paintGameField()
{
	bool dont_overpaint[MAP_WIDTH * MAP_HEIGHT];

	char buf[64];
	sprintf(buf, "%02X %04X\n", RedDiskDetonateTimer, RedDiskPlacementTimer);
	OutputDebugString(buf);

	memset(dont_overpaint, 0, sizeof(dont_overpaint));
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			bool dont_paint = false;
			unsigned short ch = levelmap[y * MAP_WIDTH + x];
			unsigned char object = ch & 0xFF;
			unsigned char high = (ch >> 8) & 0xFF;

			float tx1 = 0.0f, tx2 = 0.0f, ty1 = 0.0f, ty2 = 0.0f;
			int x1 = x * 16;
			int y1 = y * 16;
			int x2 = x * 16 + 16;
			int y2 = y * 16 + 16;
			if (object == MAP_SNIK_SNAK && (high & 0xF8) == 0x10)
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned char ofs = (high & 0xF) + 1;

				tx1 = (float)(ofs * 16) * texMovingStepX;
				ty1 = (float)(462 - 424) * texMovingStepY;
				tx2 = (float)(ofs * 16 + 16) * texMovingStepX;
				ty2 = (float)(462 - 424 - 16) * texMovingStepY;

				unsigned char h = (high & 0xF) * 2;

				y1 = y1 + 16 - h;
				y2 = y2 + 16 - h;

				dont_overpaint[(y + 1) * MAP_WIDTH + x] = true;
			}
			else if (object == MAP_SNIK_SNAK && (high & 0xF8) == 0x18)
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned char ofs = (high & 0xF) - 8;
				int texX = ((high & 0xF) < 0xC ? 192 : -128);
				int texY = ((high & 0xF) < 0xC ? 228 : 244);

				tx1 = (float)(ofs * 32 + texX) * texMovingStepX;
				ty1 = (float)(462 - texY) * texMovingStepY;
				tx2 = (float)(ofs * 32 + texX + 32) * texMovingStepX;
				ty2 = (float)(462 - texY - 16) * texMovingStepY;

				x2 += 16;
				dont_overpaint[y * MAP_WIDTH + x + 1] = true;
			}
			else if (object == MAP_SNIK_SNAK && (high & 0xF8) == 0x20)
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned char ofs = (high & 0xF) + 1;

				tx1 = (float)(ofs * 16 + 144) * texMovingStepX;
				ty1 = (float)(462 - 424) * texMovingStepY;
				tx2 = (float)(ofs * 16 + 16 + 144) * texMovingStepX;
				ty2 = (float)(462 - 424 - 16) * texMovingStepY;

				unsigned char h = (high & 0xF) * 2;

				y1 = y1 - 16 + h;
				y2 = y2 - 16 + h;
			}
			else if (object == MAP_SNIK_SNAK && (high & 0xF8) == 0x28)
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned char ofs = (high & 0xF) - 8;
				int texX = ((high & 0xF) < 0xE ? 4 * 32 : -192);
				int texY = ((high & 0xF) < 0xE ? 244 : 260);

				tx1 = (float)(ofs * 32 + texX) * texMovingStepX;
				ty1 = (float)(462 - texY) * texMovingStepY;
				tx2 = (float)(ofs * 32 + texX + 32) * texMovingStepX;
				ty2 = (float)(462 - texY - 16) * texMovingStepY;

				x1 -= 16;
			}
			else if (object == MAP_SNIK_SNAK && (high & 0xF8) == 0)
			{
				static int ssx[] = { 112, 192,  64, 206, 80,  208, 96,   48, };
				static int ssy[] = { 260, 388, 260, 228, 260, 388, 260, 260, };

				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned idx = high & 0x7;

				tx1 = (float)(ssx[idx]) * texMovingStepX;
				ty1 = (float)(462 - ssy[idx]) * texMovingStepY;
				tx2 = (float)(ssx[idx] + 16) * texMovingStepX;
				ty2 = (float)(462 - ssy[idx] - 16) * texMovingStepY;
			}
			else if ((object == MAP_ZONK || object == MAP_INFOTRON) && ((high & 0xF0) == 0x10 || high == 0x70))	// Fall down
			{
				glBindTexture(GL_TEXTURE_2D, texFixed);
				glEnable(GL_TEXTURE_2D);

				tx1 = (float)(object * 16) * texFixedStepX;
				ty1 = (float)(16) * texFixedStepY;
				tx2 = (float)(object * 16 + 16) * texFixedStepX;
				ty2 = (float)(0) * texFixedStepY;

				unsigned char h = (high == 0x70 ? 17 : (high & 0xF) * 2);

				y1 = y1 - 15 + h;
				y2 = y2 - 15 + h;
			}
			else if ((object == MAP_ZONK || object == MAP_INFOTRON) && (high & 0xF0) == 0x30)		// Scroll right
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned char ofs = (high & 0x7);
				int texY = (object == MAP_ZONK ? 100 : 180);

				tx1 = (float)(ofs * 32) * texMovingStepX;
				ty1 = (float)(462 - texY) * texMovingStepY;
				tx2 = (float)(ofs * 32 + 32) * texMovingStepX;
				ty2 = (float)(462 - texY - 16) * texMovingStepY;

				x1 -= 16;
				x2 = x1 + 32;
			}
			else if ((object == MAP_ZONK || object == MAP_INFOTRON) && (high & 0xF0) == 0x20)		// Scroll left
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned char ofs = (high & 0x7);
				int texY = (object == MAP_ZONK ? 84 : 164);

				tx1 = (float)(ofs * 32) * texMovingStepX;
				ty1 = (float)(462 - texY) * texMovingStepY;
				tx2 = (float)(ofs * 32 + 32) * texMovingStepX;
				ty2 = (float)(462 - texY - 16) * texMovingStepY;

				x2 = x1 + 32;
				dont_overpaint[y * MAP_WIDTH + x + 1] = true;
			}
			else if (object == MAP_DISK_ORANGE && (high & 0xF0) == 0x30)	// Fall down
			{
				glBindTexture(GL_TEXTURE_2D, texFixed);
				glEnable(GL_TEXTURE_2D);
				tx1 = (float)(MAP_DISK_ORANGE * 16) * texFixedStepX;
				ty1 = (float)(16) * texFixedStepY;
				tx2 = (float)(MAP_DISK_ORANGE * 16 + 16) * texFixedStepX;
				ty2 = (float)(0) * texFixedStepY;

				y1 = y1 + (high & 0xF) * 2;
				y2 = y2 + (high & 0xF) * 2;
				dont_overpaint[(y + 1) * MAP_WIDTH + x] = true;
			}
			else if (object == MAP_RED_LAMP && (high >= 0 && high < 8))
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				int ttx, tty;
				ttx = high * 16;
				tty = 196;

				tx1 = (float)(ttx) * texMovingStepX;
				ty1 = (float)(462 - tty - 16) * texMovingStepY;
				tx2 = (float)(ttx + 16) * texMovingStepX;
				ty2 = (float)(462 - tty) * texMovingStepY;
			}
			else if (ch == 0xFFFF || ch == 0x8888 || ch == 0x9999 || ch == 0xAAAA || object == 0xBB)
				dont_paint = true;
			else if (object < 40)
			{
				if (object == MAP_SPACE && dont_overpaint[y * MAP_WIDTH + x])
					dont_paint = true;
				else
				{
					if (object == MAP_MURPHY)
						object = MAP_SPACE;

					glBindTexture(GL_TEXTURE_2D, texFixed);
					glEnable(GL_TEXTURE_2D);
					tx1 = (float)(object * 16) * texFixedStepX;
					ty1 = (float)(16) * texFixedStepY;
					tx2 = (float)(object * 16 + 16) * texFixedStepX;
					ty2 = (float)(0) * texFixedStepY;
				}
			}
			else
			{
				if (!dont_overpaint[y * MAP_WIDTH + x])
					paintHex(x * 16, y * 16, object);
				dont_paint = true;
			}

			if (!dont_paint)
			{
				glColor4f(1.0f,1.0f,1.0f,1.0f);
				glBegin(GL_QUADS);
					glTexCoord2f(tx1, ty1); glVertex2i(x1, y1);
					glTexCoord2f(tx2, ty1); glVertex2i(x2, y1);
					glTexCoord2f(tx2, ty2); glVertex2i(x2, y2);
					glTexCoord2f(tx1, ty2); glVertex2i(x1, y2);
				glEnd();
			}

			paintHex(x * 16, y * 16 + MAP_HEIGHT * 16, high);
		}
	}

	// Paint the player
	if (true)
	{
		glBindTexture(GL_TEXTURE_2D, texFixed);
		glEnable(GL_TEXTURE_2D);

		float tx1 = (float)(MAP_MURPHY * 16) * texFixedStepX;
		float ty1 = (float)(16) * texFixedStepY;
		float tx2 = (float)(MAP_MURPHY * 16 + 16) * texFixedStepX;
		float ty2 = (float)(0) * texFixedStepY;

		int x1 = PlayerPosition_PixelsX;
		int y1 = PlayerPosition_PixelsY;
		int x2 = x1 + 16;
		int y2 = y1 + 16;

		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(tx1, ty1); glVertex2i(x1, y1);
			glTexCoord2f(tx2, ty1); glVertex2i(x2, y1);
			glTexCoord2f(tx2, ty2); glVertex2i(x2, y2);
			glTexCoord2f(tx1, ty2); glVertex2i(x1, y2);
		glEnd();
	}
	else
	{
		/*glBindTexture(GL_TEXTURE_2D, texMoving);
		glEnable(GL_TEXTURE_2D);

		float tx1 = (float)(288) * texMovingStepX;
		float ty1 = (float)(462 - 132) * texMovingStepY;
		float tx2 = (float)(288 + 16) * texMovingStepX;
		float ty2 = (float)(462 - 132 - 16) * texMovingStepY;

		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(tx1, ty1); glVertex2i(x1, y1);
			glTexCoord2f(tx2, ty1); glVertex2i(x2, y1);
			glTexCoord2f(tx2, ty2); glVertex2i(x2, y2);
			glTexCoord2f(tx1, ty2); glVertex2i(x1, y2);
		glEnd();*/
	}

	// Paint the red diskette
	if (RedDiskDetonateTimer)
	{
		int x = (RedDiskPosition / 2) % MAP_WIDTH;
		int y = (RedDiskPosition / 2) / MAP_WIDTH;

		int x1 = x * 16;
		int y1 = y * 16;
		int x2 = x1 + 16;
		int y2 = y1 + 16;

		if (RedDiskPlacementTimer <= 32)
		{
			glBindTexture(GL_TEXTURE_2D, texMoving);
			glEnable(GL_TEXTURE_2D);

			float tx1 = (float)(288) * texMovingStepX;
			float ty1 = (float)(462 - 132) * texMovingStepY;
			float tx2 = (float)(288 + 16) * texMovingStepX;
			float ty2 = (float)(462 - 132 - 16) * texMovingStepY;

			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(tx1, ty1); glVertex2i(x1, y1);
				glTexCoord2f(tx2, ty1); glVertex2i(x2, y1);
				glTexCoord2f(tx2, ty2); glVertex2i(x2, y2);
				glTexCoord2f(tx1, ty2); glVertex2i(x1, y2);
			glEnd();
		}

		if (RedDiskDetonateTimer > 1)
		{
			glBindTexture(GL_TEXTURE_2D, texFixed);
			glEnable(GL_TEXTURE_2D);

			float tx1 = (float)(MAP_DISK_RED * 16) * texFixedStepX;
			float ty1 = (float)(16) * texFixedStepY;
			float tx2 = (float)(MAP_DISK_RED * 16 + 16) * texFixedStepX;
			float ty2 = (float)(0) * texFixedStepY;

			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(tx1, ty1); glVertex2i(x1, y1);
				glTexCoord2f(tx2, ty1); glVertex2i(x2, y1);
				glTexCoord2f(tx2, ty2); glVertex2i(x2, y2);
				glTexCoord2f(tx1, ty2); glVertex2i(x1, y2);
			glEnd();
		}
	}

	paintHex(0, 2 * MAP_HEIGHT * 16, NumRedDisks);

	sprintf(buf, "%d %d %d %04X %04X", PlayerPosition_PixelsX, PlayerPosition_PixelsY,
		PlayerAnim_NumFrames, PlayerIsLookingLeft, PlayerAnim_ID);
	int x = 4*16;
	for (int i = 0; buf[i]; i++, x += 8)
	{
		if (buf[i] >= '0' && buf[i] <= '9')
			paintChar(x, 2 * MAP_HEIGHT * 16, buf[i] - '0');
		else if (buf[i] >= 'A' && buf[i] <= 'F')
			paintChar(x, 2 * MAP_HEIGHT * 16, buf[i] - 'A' + 10);
		else if (buf[i] >= 'a' && buf[i] <= 'f')
			paintChar(x, 2 * MAP_HEIGHT * 16, buf[i] - 'a' + 10);
	}

//	paintHex(4 * 16, 2 * MAP_HEIGHT * 16, word_403C9 >> 8);
//	paintHex(4 * 16 + 2 * 8, 2 * MAP_HEIGHT * 16, word_403C9 & 0xFF);
}

static
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	paintGameField();

	glutSwapBuffers();
}

static
void idle()
{
	unsigned long long curTime = getTime();
	if (prevTime == 0 || prevTime > curTime || curTime >= prevTime + speed)
	{
		bool spacePressed = GetAsyncKeyState(VK_SPACE) & 0x8000;

		if (GetAsyncKeyState(VK_UP) & 0x8000)
			joystickbuttons = spacePressed ? 5 : 1;
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			joystickbuttons = spacePressed ? 6 : 2;
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			joystickbuttons = spacePressed ? 7 : 3;
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			joystickbuttons = spacePressed ? 8 : 4;
		else if (spacePressed)
			joystickbuttons = 9;

		if (GetAsyncKeyState(VK_F1) & 0x8000)
			speed = 50;
		else if (GetAsyncKeyState(VK_F2) & 0x8000)
			speed = 100;
		else if (GetAsyncKeyState(VK_F3) & 0x8000)
			speed = 1000;
		else if (GetAsyncKeyState(VK_F4) & 0x8000)
			speed = 5000;

		if (!runthelevel())
			exit(0);

		joystickbuttons = 0;
		spacePressed = false;
		glutPostRedisplay();
		prevTime = curTime;
	}
}

static
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -100.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	FILE * f = fopen("levels.dat", "rb");
	if (!f)
		return 1;
		//64=gravity
		//61=red diskettes
	int levelNumber = 63;//15;//30;//11;//27;
	fseek(f, levelNumber * 1536, SEEK_SET);
	char buf[1536];
	fread(buf, 1536, 1, f);
	for (int i = 0; i < 1536; i++)
	{
		*(unsigned short *)(&levelmap[i]) = *(unsigned char *)(&buf[i]);
	}
	fclose(f);

	initplayerstate();
	startLevel();
	beginlevel();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(16 * MAP_WIDTH, 16 * MAP_HEIGHT * 2 + 16);
	glutCreateWindow("Supaplex");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	{
		glGenTextures(1, &texFixed);
		glBindTexture(GL_TEXTURE_2D, texFixed);
		BMPClass bmp;
		BMPLoad("fixed.bmp", bmp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.bytes);
		texFixedStepX = 1.0f / (float)bmp.width;
		texFixedStepY = 1.0f / (float)bmp.height;
	}

	{
		glGenTextures(1, &texMoving);
		glBindTexture(GL_TEXTURE_2D, texMoving);
		BMPClass bmp;
		BMPLoad("moving.bmp", bmp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.bytes);
		texMovingStepX = 1.0f / (float)bmp.width;
		texMovingStepY = 1.0f / (float)bmp.height;
	}

	{
		glGenTextures(1, &texFont);
		glBindTexture(GL_TEXTURE_2D, texFont);
		BMPClass bmp;
		BMPLoad("font.bmp", bmp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.bytes);
		texFontStepX = 1.0f / (float)bmp.width;
		texFontStepY = 1.0f / (float)bmp.height;
	}

	glEnable(GL_TEXTURE_2D);

	glutMainLoop();
}
