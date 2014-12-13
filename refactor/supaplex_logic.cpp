#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
 #include <windows.h>
 #include <mmsystem.h>
#else
 inline static void OutputDebugString(const char * msg)
 {
 	fprintf(stderr, "%s\n", msg);
 }
#endif

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <GL/glut.h>
#include <time.h>
#include "BMPLoader.h"

#ifndef _WIN32
enum {
	VK_SPACE = 0,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN
};
static bool keystate[9];
static int GetAsyncKeyState(int key)
{
	bool state = keystate[key];
	keystate[key] = false;
	return (state ? 0x8000 : 0);
}
#endif

enum PlayerAnim
{
	ANIM_MOVE_UP_L = 0,
	ANIM_MOVE_UP_R = 1,
	ANIM_MOVE_LEFT = 2,
	ANIM_MOVE_DOWN_L = 3,
	ANIM_MOVE_DOWN_R = 4,
	ANIM_MOVE_RIGHT = 5,
	ANIM_EXIT = 6,
	ANIM_EATBASE_UP_L = 7,
	ANIM_EATBASE_UP_R = 8,
	ANIM_EATBASE_LEFT = 9,
	ANIM_EATBASE_DOWN_L = 10,
	ANIM_EATBASE_DOWN_R = 11,
	ANIM_EATBASE_RIGHT = 12,
	ANIM_EATBASE_ATTOP = 13,
	ANIM_EATBASE_ATLEFT = 14,
	ANIM_EATBASE_ATBOTTOM = 15,
	ANIM_EATBASE_ATRIGHT = 16,
	ANIM_EATINFOTRON_UP_L = 17,
	ANIM_EATINFOTRON_UP_R = 18,
	ANIM_EATINFOTRON_LEFT = 19,
	ANIM_EATINFOTRON_DOWN_L = 20,
	ANIM_EATINFOTRON_DOWN_R = 21,
	ANIM_EATINFOTRON_RIGHT = 22,
	ANIM_EATINFOTRON_ATTOP = 23,
	ANIM_EATINFOTRON_ATLEFT = 24,
	ANIM_EATINFOTRON_ATBOTTOM = 25,
	ANIM_EATINFOTRON_ATRIGHT = 26,
	ANIM_SHIFTZONK_LEFT = 27,
	ANIM_SHIFTZONK_RIGHT = 28,
	ANIM_TELEPORT_UP = 29,
	ANIM_TELEPORT_LEFT = 30,
	ANIM_TELEPORT_DOWN = 31,
	ANIM_TELEPORT_RIGHT = 32,
	ANIM_EATDISK_UP_L = 33,
	ANIM_EATDISK_UP_R = 34,
	ANIM_EATDISK_LEFT = 35,
	ANIM_EATDISK_DOWN_L = 36,
	ANIM_EATDISK_DOWN_R = 37,
	ANIM_EATDISK_RIGHT = 38,
	ANIM_EATDISK_ATTOP = 39,
	ANIM_EATDISK_ATLEFT = 40,
	ANIM_EATDISK_ATBOTTOM = 41,
	ANIM_EATDISK_ATRIGHT = 42,
	ANIM_SHIFTYELLOWDISK_UP = 43,
	ANIM_SHIFTYELLOWDISK_LEFT = 44,
	ANIM_SHIFTYELLOWDISK_DOWN = 45,
	ANIM_SHIFTYELLOWDISK_RIGHT = 46,
	ANIM_SHIFTORANGEDISK_LEFT = 47,
	ANIM_SHIFTORANGEDISK_RIGHT = 48,
	ANIM_PLACEREDDISK = 49,
};

enum MapElements
{
	MAP_SPACE = 0,
	MAP_ZONK = 1,
	MAP_BASE = 2,
	MAP_MURPHY = 3,
	MAP_INFOTRON = 4,
	MAP_RAM_CHIP,					// 5
	MAP_HARDWARE,
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
	MAP_HARDWARE_2,
	MAP_HARDWARE_3,					// 30
	MAP_HARDWARE_4 = 31,
	MAP_BOOM = 31,
	MAP_HARDWARE_5,
	MAP_HARDWARE_6,
	MAP_HARDWARE_7,
	MAP_HARDWARE_8,					// 35
	MAP_HARDWARE_9,
	MAP_HARDWARE_10,
	MAP_RAM_CHIP_TOP,
	MAP_RAM_CHIP_BOTTOM,
	MAP_INVISIBLE_WALL,				// 40
};

static unsigned speed = 25;

static GLuint texFixed;
static float texFixedStepX;
static float texFixedStepY;
static GLuint texMoving;
static float texMovingStepX;
static float texMovingStepY;
static GLuint texFont;
static float texFontStepX;
static float texFontStepY;

static unsigned long long getTime()
{
#ifdef WIN32
	return timeGetTime();
#elif defined(__APPLE__)
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	return (unsigned long long)mts.tv_nsec * 1000000000ULL + (unsigned long long)mts.tv_sec;
#else
	struct timespec tv;
	clock_gettime(CLOCK_MONOTONIC, &tv);
	return (unsigned long long)tv.tv_nsec * 1000000000ULL + (unsigned long long)tv.tv_sec;
#endif
}

static const int MAP_WIDTH = 60;
static const int MAP_HEIGHT = 24;
static const int MAP_NUM_CELLS = MAP_WIDTH * MAP_HEIGHT;

static unsigned long long prevTime = 0;

extern "C" {
extern unsigned short PlayerExploded;
extern unsigned short PlayerAnim_ID;
extern short PlayerPosition_PixelsX;
extern short PlayerPosition_PixelsY;
extern unsigned int PlayerPosition_Ofs;
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

unsigned char OriginalLevelData[1536];

void SND_PlayZonkFallSound()
{
	OutputDebugString("SND_PlayZonkFallSound\n");
}

void SND_PlayObjectPushSound()
{
	OutputDebugString("SND_PlayObjectPushSound\n");
}

void SND_PlayBoomSound()
{
	OutputDebugString("SND_PlayBoomSound\n");
}

void SND_PlayBugSound()
{
	OutputDebugString("SND_PlayBugSound\n");
}

void SND_PlayExitSound()
{
	OutputDebugString("SND_PlayExitSound\n");
}

void SND_PlayBaseEatSound()
{
	OutputDebugString("SND_PlayBaseEatSound\n");
}

void SND_PlayEatInfotronSound()
{
	OutputDebugString("SND_PlayEatInfotronSound\n");
}

} // extern "C"


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

void paintChar(int xx, int yy, unsigned char ch, float r = 1.0f, float g = 1.0f, float b = 1.0f)
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

		glColor4f(r, g, b, 1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(x1, 1.0f); glVertex2i(xx, yy);
			glTexCoord2f(x2, 1.0f); glVertex2i(xx + 8, yy);
			glTexCoord2f(x2, 0.0f); glVertex2i(xx + 8, yy + 16);
			glTexCoord2f(x1, 0.0f); glVertex2i(xx, yy + 16);
		glEnd();
	}
}

void paintHex(int xx, int yy, unsigned char value, float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
	paintChar(xx, yy, value >> 4, r, g, b);
	paintChar(xx + 8, yy, value & 0xF, r, g, b);
}

void paintGameField()
{
	bool dont_overpaint[MAP_WIDTH * MAP_HEIGHT];
	unsigned char murphy_mode = 0;

	memset(dont_overpaint, 0, sizeof(dont_overpaint));
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			bool dont_paint = false;
			unsigned short ch = levelmap[y * MAP_WIDTH + x];
			unsigned char object = ch & 0xFF;
			unsigned char high = (ch >> 8) & 0xFF;

			if (object == MAP_MURPHY)
				murphy_mode = high;

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
			else if (object == MAP_BUG && high < 0x80)
			{
				int sx[] = { 304, 256, 272, 288, 304, 288, 272, 288, 304, 288, 272, 256, 304, };
				int sy[] = { 100, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 100, };

				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				unsigned char ofs = high % 13;

				tx1 = (float)(sx[ofs]) * texMovingStepX;
				ty1 = (float)(462 - sy[ofs]) * texMovingStepY;
				tx2 = (float)(sx[ofs] + 16) * texMovingStepX;
				ty2 = (float)(462 - sy[ofs] - 16) * texMovingStepY;
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
			else if (object == MAP_BOOM && (high >= 0 && high < 8))
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				int ttx, tty;
				ttx = high * 16;
				tty = 196;

				tx1 = (float)(ttx) * texMovingStepX;
				ty1 = (float)(462 - tty) * texMovingStepY;
				tx2 = (float)(ttx + 16) * texMovingStepX;
				ty2 = (float)(462 - tty - 16) * texMovingStepY;
			}
			else if (object == MAP_BOOM && high >= 0x80)
			{
				glBindTexture(GL_TEXTURE_2D, texMoving);
				glEnable(GL_TEXTURE_2D);

				if (high > 0x87)
					high = 0x87;

				int ttx, tty;
				ttx = 128 + (high - 0x80) * 16;
				tty = 196;

				tx1 = (float)(ttx) * texMovingStepX;
				ty1 = (float)(462 - tty) * texMovingStepY;
				tx2 = (float)(ttx + 16) * texMovingStepX;
				ty2 = (float)(462 - tty - 16) * texMovingStepY;
			}
			else if (ch == 0xFFFF || ch == 0x8888 || ch == 0x9999 || ch == 0xAAAA || object == 0xBB)
				dont_paint = true;
			else if (object < 40)
			{
				if (object == MAP_SPACE && dont_overpaint[y * MAP_WIDTH + x])
					dont_paint = true;
				else if (object != MAP_MURPHY)
				{
					if (object == MAP_HARDWARE || object == MAP_RAM_CHIP)
						object = OriginalLevelData[y * MAP_WIDTH + x];

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

			if (object == MAP_MURPHY)
				paintHex(x * 16, y * 16 + MAP_HEIGHT * 16, high, 1, 1, 0);
			else
				paintHex(x * 16, y * 16 + MAP_HEIGHT * 16, high);
		}
	}

	// Paint the player
	if (!PlayerExploded)
	{
		int paintAbove = -1, paintBelow = -1, paintInPlace = -1, paintOver = -1, paintOverShift = 0;
		int secondTexX = -1, secondTexY = -1, secondX, secondY;
		int texX, texY, texW = 16, texH = 16;
		bool use_default = true;

		int x1 = PlayerPosition_PixelsX;
		int y1 = PlayerPosition_PixelsY;

		if (PlayerAnim_ID == ANIM_EXIT)
		{
			unsigned idx = (41 - PlayerAnim_NumFrames) >> 2;
			if (idx == 0)
			{
				texX = 192;
				texY = 16;
			}
			else if (idx == 1)
			{
				texX = 176;
				texY = 16;
			}
			else if (idx >= 9)
			{
				texX = 302;
				texY = 16;
			}
			else
			{
				texX = 192 + (idx - 2) * 16;
				texY = 64;
			}

			use_default = false;
		}
		else if (PlayerAnim_NumFrames > 0 && PlayerAnim_NumFrames < 8)
		{
			const int shift[] = { 0, 16, 16, 32, 32, 16, 16, 0 };
			int idx = (7 - PlayerAnim_NumFrames);

			use_default = false;

			switch (PlayerAnim_ID)
			{
			case ANIM_EATBASE_LEFT:
				texY = 0;
				texX = idx * 32;
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
				break;

			case ANIM_EATBASE_RIGHT:
				if (idx < 2)
				{
					texX = 256 + idx * 32;
					texY = 0;
				}
				else
				{
					texX = (idx - 2) * 32;
					texY = 16;
				}
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 1) * 16;
				break;

			case ANIM_MOVE_LEFT:
				texY = 32;
				texX = 32 + idx * 32;
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
				break;

			case ANIM_MOVE_RIGHT:
				if (idx == 0)
				{
					texX = 288;
					texY = 32;
				}
				else
				{
					texY = 48;
					texX = (idx - 1) * 32;
				}
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 1) * 16;
				break;

			case ANIM_EATINFOTRON_LEFT:
				texY = 212;
				texX = idx * 32;
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
				break;

			case ANIM_EATINFOTRON_RIGHT:
				if (idx < 2)
				{
					texX = 256 + idx * 32;
					texY = 212;
				}
				else
				{
					texY = 228;
					texX = (idx - 2) * 32;
				}
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 1) * 16;
				break;

			case ANIM_EATDISK_LEFT:
				if (idx < 5)
				{
					texY = 260;
					texX = 128 + idx * 32;
				}
				else
				{
					texY = 260 + (idx - 5) * 32;
					texX = 288;
				}
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
				break;

			case ANIM_EATDISK_RIGHT:
				if (idx < 4)
				{
					texX = 192 + idx * 32;
					texY = 308;
				}
				else if (idx == 4)
				{
					texY = 324;
					texX = 288;
				}
				else if (idx == 5)
				{
					texY = 340;
					texX = 288;
				}
				else if (idx == 6)
				{
					texY = 356;
					texX = 192;
				}
				else
				{
					texY = 356;
					texX = 224;
				}
				texW = 32;
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 1) * 16;
				break;

			case ANIM_MOVE_UP_L:
				texY = 66;
				texX = shift[idx & 7];
				break;

			case ANIM_MOVE_UP_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				break;

			case ANIM_MOVE_DOWN_L:
				texY = 66;
				texX = shift[idx & 7];
				break;

			case ANIM_MOVE_DOWN_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				break;

			case ANIM_EATBASE_UP_L:
				texY = 66;
				texX = shift[idx & 7];
				paintInPlace = MAP_BASE;
				break;

			case ANIM_EATBASE_UP_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				paintInPlace = MAP_BASE;
				break;

			case ANIM_EATBASE_DOWN_L:
				texY = 66;
				texX = shift[idx & 7];
				paintInPlace = MAP_BASE;
				break;

			case ANIM_EATBASE_DOWN_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				paintInPlace = MAP_BASE;
				break;

			case ANIM_EATINFOTRON_UP_L:
				texY = 66;
				texX = shift[idx & 7];
				paintInPlace = MAP_INFOTRON;
				break;

			case ANIM_EATINFOTRON_UP_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				paintInPlace = MAP_INFOTRON;
				break;

			case ANIM_EATINFOTRON_DOWN_L:
				texY = 66;
				texX = shift[idx & 7];
				paintInPlace = MAP_INFOTRON;
				break;

			case ANIM_EATINFOTRON_DOWN_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				paintInPlace = MAP_INFOTRON;
				break;

			case ANIM_EATDISK_UP_L:
				texY = 66;
				texX = shift[idx & 7];
				paintAbove = MAP_DISK_RED;
				break;

			case ANIM_EATDISK_UP_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				paintAbove = MAP_DISK_RED;
				break;

			case ANIM_EATDISK_DOWN_L:
				texY = 66;
				texX = shift[idx & 7];
				paintBelow = MAP_DISK_RED;
				break;

			case ANIM_EATDISK_DOWN_R:
				texY = 66;
				texX = 48 + shift[idx & 7];
				paintBelow = MAP_DISK_RED;
				break;

			case ANIM_SHIFTZONK_LEFT:
				if (idx < 6)
				{
					texY = 116;
					texX = idx * 48;
					texW = 48;
				}
				else
				{
					texY = 132;
					texX = (idx - 6) * 48;
					texW = 48;
				}
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 2) * 16;
				break;

			case ANIM_SHIFTZONK_RIGHT:
				if (idx < 4)
				{
					texY = 132;
					texX = 96 + idx * 48;
					texW = 48;
				}
				else
				{
					texY = 148;
					texX = (idx - 4) * 48;
					texW = 48;
				}
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
				break;

			case ANIM_SHIFTORANGEDISK_LEFT:
				if (idx < 6)
				{
					texY = 276;
					texX = idx * 48;
					texW = 48;
				}
				else
				{
					texY = 292;
					texX = (idx - 6) * 48;
					texW = 48;
				}
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 2) * 16;
				break;

			case ANIM_SHIFTORANGEDISK_RIGHT:
				if (idx < 4)
				{
					texY = 292;
					texX = 96 + idx * 48;
					texW = 48;
				}
				else
				{
					texY = 308;
					texX = (idx - 4) * 48;
					texW = 48;
				}
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
				break;

			case ANIM_SHIFTYELLOWDISK_LEFT:
				if (idx < 6)
				{
					texY = 324;
					texX = idx * 48;
					texW = 48;
				}
				else
				{
					texY = 340;
					texX = (idx - 6) * 48;
					texW = 48;
				}
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 2) * 16;
				break;

			case ANIM_SHIFTYELLOWDISK_RIGHT:
				if (idx < 4)
				{
					texY = 340;
					texX = 96 + idx * 48;
					texW = 48;
				}
				else
				{
					texY = 356;
					texX = (idx - 4) * 48;
					texW = 48;
				}
				x1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
				break;

			case ANIM_SHIFTYELLOWDISK_UP:
				texY = 422;
				texX = 304;
				texH = 16;
				if (PlayerIsLookingLeft)
				{
					texX += 16;
					texW = -16;
				}
				paintOver = MAP_DISK_YELLOW;
				paintOverShift = y1 - ((PlayerPosition_Ofs >> 1) / MAP_WIDTH + 1) * 16;
				break;

			case ANIM_SHIFTYELLOWDISK_DOWN:
				texY = 408;
				texX = 288;
				texH = 16;
				if (PlayerIsLookingLeft)
				{
					texX += 16;
					texW = -16;
				}
				paintOver = MAP_DISK_YELLOW;
				paintOverShift = y1 - ((PlayerPosition_Ofs >> 1) / MAP_WIDTH - 1) * 16;
				break;

			case ANIM_EATBASE_ATTOP:
			case ANIM_EATBASE_ATLEFT:
			case ANIM_EATBASE_ATBOTTOM:
			case ANIM_EATBASE_ATRIGHT:
				if (idx < 4)
				{
					secondTexX = 256 + idx * 16;
					secondTexY = 84;
				}
				else
				{
					secondTexX = 256 + (idx - 4) * 16;
					secondTexY = 100;
				}
        		switch (PlayerAnim_ID)
				{
				case ANIM_EATBASE_ATTOP:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH - 1) * 16;
					break;
				case ANIM_EATBASE_ATLEFT:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 1) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16;
					break;
				case ANIM_EATBASE_ATBOTTOM:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH + 1) * 16;
					break;
				case ANIM_EATBASE_ATRIGHT:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH + 1) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16;
					break;
				}
				use_default = true;
				break;

			case ANIM_EATINFOTRON_ATTOP:
			case ANIM_EATINFOTRON_ATLEFT:
			case ANIM_EATINFOTRON_ATBOTTOM:
			case ANIM_EATINFOTRON_ATRIGHT:
				secondTexX = 192 + idx * 16;
				secondTexY = 148;
        		switch (PlayerAnim_ID)
				{
				case ANIM_EATINFOTRON_ATTOP:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH - 1) * 16;
					break;
				case ANIM_EATINFOTRON_ATLEFT:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 1) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16;
					break;
				case ANIM_EATINFOTRON_ATBOTTOM:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH + 1) * 16;
					break;
				case ANIM_EATINFOTRON_ATRIGHT:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH + 1) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16;
					break;
				}
				use_default = true;
				break;

			case ANIM_EATDISK_ATTOP:
			case ANIM_EATDISK_ATLEFT:
			case ANIM_EATDISK_ATBOTTOM:
			case ANIM_EATDISK_ATRIGHT:
				if (idx < 4)
				{
					secondTexX = 256 + idx * 16;
					secondTexY = 164;
				}
				else
				{
					secondTexX = 256 + (idx - 4) * 16;
					secondTexY = 180;
				}
        		switch (PlayerAnim_ID)
				{
				case ANIM_EATDISK_ATTOP:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH - 1) * 16;
					break;
				case ANIM_EATDISK_ATLEFT:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH - 1) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16;
					break;
				case ANIM_EATDISK_ATBOTTOM:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH + 1) * 16;
					break;
				case ANIM_EATDISK_ATRIGHT:
					secondX = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH + 1) * 16;
					secondY = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16;
					break;
				}
				use_default = true;
				break;

			case ANIM_PLACEREDDISK:
				use_default = true;
				break;
			}
		}

		static unsigned char prevMurphyMode = 0;

		unsigned char origMode = murphy_mode;
		if (!murphy_mode && (prevMurphyMode == 0x0E || prevMurphyMode == 0x0F ||
				prevMurphyMode == 0x28 || prevMurphyMode == 0x29 ||
				prevMurphyMode == 0x24 || prevMurphyMode == 0x27 ||
				prevMurphyMode == 0x25 || prevMurphyMode == 0x26 ||
				prevMurphyMode == 0x10 || prevMurphyMode == 0x11 ||
				prevMurphyMode == 0x12 || prevMurphyMode == 0x13 ||
				prevMurphyMode == 0x03 || prevMurphyMode == 0x04 ||
				prevMurphyMode == 0x05 || prevMurphyMode == 0x07 ||
				prevMurphyMode == 0x06 || prevMurphyMode == 0x08 ||
				prevMurphyMode == 0x09 || prevMurphyMode == 0x0A ||
				prevMurphyMode == 0x0B || prevMurphyMode == 0x0C ||
				prevMurphyMode == 0x1C || prevMurphyMode == 0x1D ||
				prevMurphyMode == 0x1E || prevMurphyMode == 0x1F ||
				prevMurphyMode == 0x14 || prevMurphyMode == 0x15 ||
				prevMurphyMode == 0x16 || prevMurphyMode == 0x17 ||
				prevMurphyMode == 0x20 || prevMurphyMode == 0x21 ||
				prevMurphyMode == 0x22 || prevMurphyMode == 0x23 ||
				prevMurphyMode == 0x01 || prevMurphyMode == 0x02))
			murphy_mode = prevMurphyMode;

		if (use_default)
		{
			if (murphy_mode == 0x27 || murphy_mode == 0x24)
			{
				if (PlayerIsLookingLeft)
				{
					texX = 240;
					texY = 16;
				}
				else
				{
					texX = 224;
					texY = 16;
				}
			}
			else if (murphy_mode == 0x0E || murphy_mode == 0x28 || murphy_mode == 0x25)
			{
				texX = 240;
				texY = 16;
			}
			else if (murphy_mode == 0x0F || murphy_mode == 0x29 || murphy_mode == 0x26)
			{
				texX = 224;
				texY = 16;
			}
			else if (murphy_mode == 0x10 || murphy_mode == 0x14 || murphy_mode == 0x20)
			{
				texX = 160;
				texY = 64;
			}
			else if (murphy_mode == 0x11 || murphy_mode == 0x15 || murphy_mode == 0x21)
			{
				texX = 208;
				texY = 16;
			}
			else if (murphy_mode == 0x12 || murphy_mode == 0x16 || murphy_mode == 0x22)
			{
				texX = 176;
				texY = 64;
			}
			else if (murphy_mode == 0x13 || murphy_mode == 0x17 || murphy_mode == 0x23)
			{
				texX = 192;
				texY = 16;
			}
			else if (murphy_mode == 0x01 || murphy_mode == 0x02 || murphy_mode == 0x03 || murphy_mode == 0x04 ||
				murphy_mode == 0x05 || murphy_mode == 0x06 || murphy_mode == 0x07 || murphy_mode == 0x08 ||
				murphy_mode == 0x09 ||murphy_mode == 0x0A || murphy_mode == 0x0B || murphy_mode == 0x0C ||
				murphy_mode == 0x1C ||murphy_mode == 0x1D || murphy_mode == 0x1E || murphy_mode == 0x1F)
			{
				if (PlayerIsLookingLeft)
				{
					texX = 0;
					texY = 66;
				}
				else
				{
					texX = 48;
					texY = 66;
				}
			}	
			else
			{
				texX = 304;
				texY = 132;
			}
		}

		prevMurphyMode = origMode;

		if (paintAbove != -1)
		{
			glBindTexture(GL_TEXTURE_2D, texFixed);
			glEnable(GL_TEXTURE_2D);

			float tx1 = (float)(paintAbove * 16) * texFixedStepX;
			float ty1 = (float)(16) * texFixedStepY;
			float tx2 = (float)(paintAbove * 16 + 16) * texFixedStepX;
			float ty2 = (float)(0) * texFixedStepY;

			int xx1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
			int yy1 = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH - 1) * 16;

			int xx2 = xx1 + 16;
			int yy2 = yy1 + 16;

			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(tx1, ty1); glVertex2i(xx1, yy1);
				glTexCoord2f(tx2, ty1); glVertex2i(xx2, yy1);
				glTexCoord2f(tx2, ty2); glVertex2i(xx2, yy2);
				glTexCoord2f(tx1, ty2); glVertex2i(xx1, yy2);
			glEnd();
		}

		if (paintBelow != -1)
		{
			glBindTexture(GL_TEXTURE_2D, texFixed);
			glEnable(GL_TEXTURE_2D);

			float tx1 = (float)(paintBelow * 16) * texFixedStepX;
			float ty1 = (float)(16) * texFixedStepY;
			float tx2 = (float)(paintBelow * 16 + 16) * texFixedStepX;
			float ty2 = (float)(0) * texFixedStepY;

			int xx1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
			int yy1 = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH + 1) * 16;

			int xx2 = xx1 + 16;
			int yy2 = yy1 + 16;

			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(tx1, ty1); glVertex2i(xx1, yy1);
				glTexCoord2f(tx2, ty1); glVertex2i(xx2, yy1);
				glTexCoord2f(tx2, ty2); glVertex2i(xx2, yy2);
				glTexCoord2f(tx1, ty2); glVertex2i(xx1, yy2);
			glEnd();
		}

		if (paintInPlace != -1)
		{
			glBindTexture(GL_TEXTURE_2D, texFixed);
			glEnable(GL_TEXTURE_2D);

			float tx1 = (float)(paintInPlace * 16) * texFixedStepX;
			float ty1 = (float)(16) * texFixedStepY;
			float tx2 = (float)(paintInPlace * 16 + 16) * texFixedStepX;
			float ty2 = (float)(0) * texFixedStepY;

			int xx1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
			int yy1 = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16;

			int xx2 = xx1 + 16;
			int yy2 = yy1 + 16;

			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(tx1, ty1); glVertex2i(xx1, yy1);
				glTexCoord2f(tx2, ty1); glVertex2i(xx2, yy1);
				glTexCoord2f(tx2, ty2); glVertex2i(xx2, yy2);
				glTexCoord2f(tx1, ty2); glVertex2i(xx1, yy2);
			glEnd();
		}

		glBindTexture(GL_TEXTURE_2D, texMoving);
		glEnable(GL_TEXTURE_2D);

		float tx1 = (float)(texX) * texMovingStepX;
		float ty1 = (float)(462 - texY) * texMovingStepY;
		float tx2 = (float)(texX + texW) * texMovingStepX;
		float ty2 = (float)(462 - texY - texH) * texMovingStepY;

		int x2 = x1 + abs(texW);
		int y2 = y1 + abs(texH);

		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(tx1, ty1); glVertex2i(x1, y1);
			glTexCoord2f(tx2, ty1); glVertex2i(x2, y1);
			glTexCoord2f(tx2, ty2); glVertex2i(x2, y2);
			glTexCoord2f(tx1, ty2); glVertex2i(x1, y2);
		glEnd();

		if (secondTexX != -1 && secondTexY != -1)
		{
			glBindTexture(GL_TEXTURE_2D, texMoving);
			glEnable(GL_TEXTURE_2D);

			float tx1 = (float)(secondTexX) * texMovingStepX;
			float ty1 = (float)(462 - secondTexY) * texMovingStepY;
			float tx2 = (float)(secondTexX + 16) * texMovingStepX;
			float ty2 = (float)(462 - secondTexY - 16) * texMovingStepY;

			int x1 = secondX;
			int y1 = secondY;
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

		if (paintOver != -1)
		{
			glBindTexture(GL_TEXTURE_2D, texFixed);
			glEnable(GL_TEXTURE_2D);

			float tx1 = (float)(paintOver * 16) * texFixedStepX;
			float ty1 = (float)(16) * texFixedStepY;
			float tx2 = (float)(paintOver * 16 + 16) * texFixedStepX;
			float ty2 = (float)(0) * texFixedStepY;

			int xx1 = ((PlayerPosition_Ofs >> 1) % MAP_WIDTH) * 16;
			int yy1 = ((PlayerPosition_Ofs >> 1) / MAP_WIDTH) * 16 + paintOverShift;

			int xx2 = xx1 + 16;
			int yy2 = yy1 + 16;

			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(tx1, ty1); glVertex2i(xx1, yy1);
				glTexCoord2f(tx2, ty1); glVertex2i(xx2, yy1);
				glTexCoord2f(tx2, ty2); glVertex2i(xx2, yy2);
				glTexCoord2f(tx1, ty2); glVertex2i(xx1, yy2);
			glEnd();
		}
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

	char buf[64];
	sprintf(buf, "%d %d %d %04X %04X %02X %04X", PlayerPosition_PixelsX, PlayerPosition_PixelsY,
		PlayerAnim_NumFrames, PlayerIsLookingLeft, PlayerAnim_ID, murphy_mode, PlayerExploded);
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
			speed = 25;
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
	glOrtho(0, MAP_WIDTH * 16, MAP_HEIGHT * 16 * 2 + 16, 0, -100.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

#ifndef _WIN32
static void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: keystate[VK_LEFT] = true; break;
	case GLUT_KEY_DOWN: keystate[VK_DOWN] = true; break;
	case GLUT_KEY_RIGHT: keystate[VK_RIGHT] = true; break;
	case GLUT_KEY_UP: keystate[VK_UP] = true; break;
	case ' ': keystate[VK_SPACE] = true; break;
	case GLUT_KEY_F1: keystate[VK_F1] = true; break;
	case GLUT_KEY_F2: keystate[VK_F2] = true; break;
	case GLUT_KEY_F3: keystate[VK_F3] = true; break;
	case GLUT_KEY_F4: keystate[VK_F4] = true; break;
	}
}
#endif

int main(int argc, char ** argv)
{
	FILE * f = fopen("levels.dat", "rb");
	if (!f)
		return 1;
		//64=gravity
		//61=red diskettes
		//104=yellow diskettes
		//8=bugs
		//29=electrons
	int levelNumber = 29;//29;//61;//63;//15;//30;//11;//27;
	fseek(f, levelNumber * 1536, SEEK_SET);
	fread(OriginalLevelData, 1536, 1, f);
	for (int i = 0; i < 1536; i++)
	{
		*(unsigned short *)(&levelmap[i]) = *(unsigned char *)(&OriginalLevelData[i]);
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
#ifndef _WIN32
	glutKeyboardFunc(keyboard);
#endif

	{
		glGenTextures(1, &texFixed);
		glBindTexture(GL_TEXTURE_2D, texFixed);
		BMPClass bmp;
		BMPLoad("fixed.bmp", bmp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.bytes);
		texFontStepX = 1.0f / (float)bmp.width;
		texFontStepY = 1.0f / (float)bmp.height;
	}

	glEnable(GL_TEXTURE_2D);

	glutMainLoop();
}
