#include <engine/render.h>
#include <engine/input.h>
#include <engine/system.h>
#include <engine/filesys.h>
#include <opengl/context.h>
#include <sound/functions.h>
#include "superplex_game.h"

GAME_MAIN(SuperplexGame)

/* SuperplexGame methods */

//
// Constructor
//
SuperplexGame::SuperplexGame()
	: m_PrevFrameTime(0.0), m_SkipThisFrame(true), m_InfotronFrame(0.0f), m_InfotronAngle(0.0f)
{
	FS_INIT_ZIP()

	m_Camera = new Camera;
	m_Camera->setNearFarPlanes(1.0f, 1000.0f);

	m_Meshes.resize(40);
	m_Meshes[MAP_ZONK] = R_LoadMesh("zonk.obj");
	m_Meshes[MAP_BASE] = R_LoadMesh("base.obj");
	m_Meshes[MAP_RAM_CHIP] = R_LoadMesh("ram_chip_square.obj");
	m_Meshes[MAP_GRAY_DENTED_PYRAMID] = R_LoadMesh("pyramid.obj");
	m_Meshes[MAP_EXIT] = R_LoadMesh("exit.obj");
	m_Meshes[MAP_DISK_ORANGE] = R_LoadMesh("orange_disk.obj");
	m_Meshes[MAP_PORT_LEFT_TO_RIGHT] = R_LoadMesh("port_right.obj");
	m_Meshes[MAP_PORT_RIGHT_TO_LEFT] = R_LoadMesh("port_left.obj");
	m_Meshes[MAP_PORT_UP_TO_DOWN] = R_LoadMesh("port_down.obj");
	m_Meshes[MAP_PORT_DOWN_TO_UP] = R_LoadMesh("port_up.obj");
	m_Meshes[MAP_DISK_YELLOW] = R_LoadMesh("yellow_disk.obj");
	m_Meshes[MAP_DISK_RED] = R_LoadMesh("red_disk.obj");
	m_Meshes[MAP_PORT_VERTICAL] = R_LoadMesh("port_vertical.obj");
	m_Meshes[MAP_PORT_HORIZONTAL] = R_LoadMesh("port_horizontal.obj");
	m_Meshes[MAP_PORT_CROSS] = R_LoadMesh("port_cross.obj");
	m_Meshes[MAP_RAM_CHIP_LEFT] = R_LoadMesh("ram_chip_top.obj");
	m_Meshes[MAP_RAM_CHIP_RIGHT] = R_LoadMesh("ram_chip_bottom.obj");
	m_Meshes[MAP_RAM_CHIP_TOP] = R_LoadMesh("ram_chip_top.obj");
	m_Meshes[MAP_RAM_CHIP_BOTTOM] = R_LoadMesh("ram_chip_bottom.obj");
	m_Meshes[MAP_HARDWARE_1] = R_LoadMesh("hardware1.obj");
	m_Meshes[MAP_HARDWARE_2] = R_LoadMesh("hardware2.obj");
	m_Meshes[MAP_HARDWARE_3] = R_LoadMesh("hardware3.obj");
	m_Meshes[MAP_HARDWARE_4] = R_LoadMesh("hardware4.obj");
	m_Meshes[MAP_HARDWARE_5] = R_LoadMesh("hardware5.obj");
	m_Meshes[MAP_HARDWARE_6] = R_LoadMesh("hardware6.obj");
	m_Meshes[MAP_HARDWARE_7] = R_LoadMesh("hardware7.obj");
	m_Meshes[MAP_HARDWARE_8] = R_LoadMesh("hardware8.obj");
	m_Meshes[MAP_HARDWARE_9] = R_LoadMesh("hardware9.obj");
	m_Meshes[MAP_HARDWARE_10] = R_LoadMesh("hardware9.obj");
	m_Meshes[MAP_TERMINAL] = R_LoadMesh("terminal_back.obj");

	for (int i = 1; i <= 29; i++)
	{
		char buf[256];

		snprintf(buf, sizeof(buf), "infotron_%02d.obj", i);
		m_Infotron.push_back(R_LoadMesh(buf));
	}	

	for (int i = 1; i <= 9; i++)
	{
		char buf[256];

		snprintf(buf, sizeof(buf), "sniksnak_%02d.obj", i);
		m_SnikSnak.push_back(R_LoadMesh(buf));
	}	

	for (int i = 1; i <= 9; i++)
	{
		char buf[256];

		if (i == 6)
			continue;
		snprintf(buf, sizeof(buf), "murphy_%02d.obj", i);
		m_Murphy.push_back(R_LoadMesh(buf));
	}
	m_Murphy.push_back(R_LoadMesh("murphy_shift.obj"));
	m_Murphy.push_back(R_LoadMesh("murphy_putdisk.obj"));

	m_Star = R_LoadMesh("star.obj");

	m_TerminalFront = R_LoadMesh("terminal_front.obj");
	m_TerminalPixelShader = R_CompileFragmentShader("shaders/terminal.ps");
	R_SetMeshFragmentShader(m_TerminalFront, m_TerminalPixelShader);

	m_Boom = R_LoadTexture("boom.jpg");

	int level = 1;
	FILE * f = fopen((Sys_GetAppPath() + "/level.cfg").toLocal8Bit().constData(), "r");
  #ifdef __APPLE__
	if (!f)
		f = fopen((Sys_GetAppPath() + "/../../../level.cfg").toLocal8Bit().constData(), "r");
  #endif
	if (f)
	{
		fscanf(f, "%d", &level);
		if (level < 1)
			level = 1;
		if (level > 111)
			level = 111;
		fclose(f);
	}	

	R_SetClearColor(0.0f, 0.0f, 0.0f);
	m_Lightning = R_CreateLightning(Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.5f, 0.5f, 0.0f), 0.4f, 0.54f, 5);

	SND_SetListenerOrientation(Vector3(0.0f, 0.0f, -1.0f));

	SND_PreloadSound("sounds/boom.wav");
	SND_PreloadSound("sounds/eat_base.wav");
	SND_PreloadSound("sounds/eat_infotron.wav");
	SND_PreloadSound("sounds/zonk_fall.wav");
	SND_PreloadSound("sounds/zonk_shift.wav");
	SND_PreloadSound("sounds/level_done.wav");
	SND_PreloadSound("sounds/bug.wav");

	SND_PlayMusic("sounds/music.ogg");

	m_Game.startGame(level - 1);
}

//
// Destructor
//
SuperplexGame::~SuperplexGame()
{
}

//
// Renders a single frame
//
void SuperplexGame::renderFrame()
{
	static const float terminal[4] = { 0.5435f, 0.362f, 0.1805f, 0.0f };

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float light0_position[] = { 0.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_NORMALIZE);

	Vector2 playerPos = m_Game.getPlayerCoords();
	playerPos.y() = -playerPos.y();
	m_Camera->lookAt(Vector3(playerPos, 15), Vector3(playerPos, 0), Vector3(0, 1, 0));
	R_Set3D(m_Camera);

	static float listener_x, listener_y;
	static bool listener_init = false;
	if (listener_x != playerPos.x() || listener_y != playerPos.y() ||
		unlikely(!listener_init))
	{
		listener_init = true;
		listener_x = playerPos.x() * 0.1f;
		listener_y = playerPos.y() * 0.1f;
		SND_SetListenerPosition(Vector3(listener_x, listener_y, 0));
	}

	float murphy_x, murphy_y;
	MapItem murphy;
	for (int x = 0; x < 60; x++)
	{
		for (int y = 0; y < 24; y++)
		{
			MapItem extra, item = m_Game.getItem(x, y);
			bool dontPaint = false;
			unsigned anim;
			byte real;

			glPushMatrix();
			glTranslatef((float)x, -(float)y, 0);
			switch (item.id)
			{
			case MAP_MURPHY:
				murphy_x = (float)x;
				murphy_y = -(float)y;
				murphy = item;
				break;

			case MAP_BUG:
				R_RenderMesh(m_Meshes[MAP_BASE]);
				if (item.state < 0x80)
				{
					glPushMatrix();
					glTranslatef(0.0f, 0.0f, 0.11f);
					glScalef(0.65f, 0.65f, 0.65f);
					R_RenderLightning(m_Lightning, 5.0f, false);
					glPopMatrix();
				}
				break;

			case MAP_ZONK:
			case MAP_INFOTRON:
				switch (item.state & 0xF0)
				{
				case 0x10:	// Fall down
					glTranslatef(0.0f, 1.0f - (item.state & 0xF) * 2.0f / 15.0f, 0.0);
					break;

				case 0x20:	// Fall left
					glTranslatef(1.0f - ((item.state & 0x7) - 1) * 1.0f / 7.0f, 0.0, 0.0);
					glRotatef((float)((item.state & 0x7) - 2) * 60.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x30:	// Fall right
					glTranslatef(-1.0f + ((item.state & 0x7) - 1) * 1.0f / 7.0f, 0.0, 0.0);
					glRotatef((float)((item.state & 0x7) - 2) * -60.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x70:
					glTranslatef(0.0f, -0.05f, 0.0);
					break;

				default:
					if (item.id == MAP_ZONK)
					{
						extra = m_Game.getItem(x + 1, y);
						if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_SHIFT_ZONK_LEFT)
							dontPaint = true;
						extra = m_Game.getItem(x - 1, y);
						if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_SHIFT_ZONK_RIGHT)
							dontPaint = true;
					}
					else if (item.id == MAP_INFOTRON)
					{
						extra = m_Game.getItem(x, y + 1);
						if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_INFOTRON_TOP)
							dontPaint = true;
						extra = m_Game.getItem(x, y - 1);
						if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_INFOTRON_BOTTOM)
							dontPaint = true;
						extra = m_Game.getItem(x + 1, y);
						if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_INFOTRON_LEFT)
							dontPaint = true;
						extra = m_Game.getItem(x - 1, y);
						if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_INFOTRON_RIGHT)
							dontPaint = true;
					}
				}

				if (!dontPaint)
				{
					if (item.id == MAP_ZONK)
						R_RenderMesh(m_Meshes[item.id]);
					else
						renderInfotron();
				}	
				break;

			case MAP_BOOM:
				R_BindTexture(0, m_Boom);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glBegin(GL_QUADS);
					glTexCoord2f(0.125f * (item.state - 1), 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
					glTexCoord2f(0.125f * item.state, 0.0f); glVertex3f( 0.5f, -0.5f, 0.0f);
					glTexCoord2f(0.125f * item.state, 1.0f); glVertex3f( 0.5f,  0.5f, 0.0f);
					glTexCoord2f(0.125f * (item.state - 1), 1.0f); glVertex3f(-0.5f,  0.5f, 0.0f);
				glEnd();
				break;

			case MAP_DISK_ORANGE:
				if ((item.state & 0xF0) == 0x30)
					glTranslatef(0.0f, (item.state & 0xF) * -2.0f / 15.0f, 0.0f);
				else
				{
					extra = m_Game.getItem(x + 1, y);
					if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_SHIFT_ORANGE_DISK_LF)
						dontPaint = true;
					extra = m_Game.getItem(x - 1, y);
					if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_SHIFT_ORANGE_DISK_RT)
						dontPaint = true;
				}

				if (!dontPaint)
					R_RenderMesh(m_Meshes[MAP_DISK_ORANGE]);
				break;

			case MAP_DISK_YELLOW:
				extra = m_Game.getItem(x + 1, y);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_MOVE_YELLOW_DISK_LF)
					dontPaint = true;
				extra = m_Game.getItem(x - 1, y);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_MOVE_YELLOW_DISK_RT)
					dontPaint = true;
				extra = m_Game.getItem(x, y + 1);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_MOVE_YELLOW_DISK_UP)
					dontPaint = true;
				extra = m_Game.getItem(x, y - 1);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_MOVE_YELLOW_DISK_DN)
					dontPaint = true;

				if (!dontPaint)
					R_RenderMesh(m_Meshes[MAP_DISK_YELLOW]);
				break;

			case MAP_BASE:
				extra = m_Game.getItem(x, y + 1);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_BASE_TOP)
					dontPaint = true;
				extra = m_Game.getItem(x, y - 1);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_BASE_BOTTOM)
					dontPaint = true;
				extra = m_Game.getItem(x + 1, y);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_BASE_LEFT)
					dontPaint = true;
				extra = m_Game.getItem(x - 1, y);
				if (extra.id == MAP_MURPHY && extra.state == MURPHY_STATE_EAT_BASE_RIGHT)
					dontPaint = true;

				if (!dontPaint)
					R_RenderMesh(m_Meshes[MAP_BASE]);
				break;

			case MAP_DISK_RED:
				extra = m_Game.getItem(x, y + 1);
				if (extra.id == MAP_MURPHY &&
						(extra.state == MURPHY_STATE_EAT_BOMB_TOP || extra.state == MURPHY_STATE_MOVE_UP_EAT_BOMB))
					dontPaint = true;
				extra = m_Game.getItem(x, y - 1);
				if (extra.id == MAP_MURPHY &&
						(extra.state == MURPHY_STATE_EAT_BOMB_BOTTOM || extra.state == MURPHY_STATE_MOVE_DOWN_EAT_BOMB))
					dontPaint = true;
				extra = m_Game.getItem(x + 1, y);
				if (extra.id == MAP_MURPHY &&
						extra.state == MURPHY_STATE_EAT_BOMB_LEFT)
					dontPaint = true;
				extra = m_Game.getItem(x - 1, y);
				if (extra.id == MAP_MURPHY &&
						extra.state == MURPHY_STATE_EAT_BOMB_RIGHT)
					dontPaint = true;

				if (!dontPaint)
					R_RenderMesh(m_Meshes[MAP_DISK_RED]);
				break;

			case MAP_SNIK_SNAK:
				anim = 0;
				switch (item.state)
				{
				case 0x01:
				case 0x09:
					break;

				case 0x00:
				case 0x0A:
					glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x07:
				case 0x0B:
					glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x06:
				case 0x0C:
					glRotatef(-135.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x05:
				case 0x0D:
					glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x04:
				case 0x0E:
					glRotatef(135.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x03:
				case 0x0F:
					glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
					break;

				case 0x02:
				case 0x08:
					glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
					break;

				default:
					if (item.state >= 0x10 && item.state <= 0x17)
					{
						unsigned ofs = item.state & 7;
						glTranslatef(0.0f, -1.0f + ofs * 1.0f / 8.0f, 0.0f);
						anim = ofs + 1;
					}
					else if (item.state >= 0x18 && item.state <= 0x1F)
					{
						unsigned ofs = item.state & 7;
						glTranslatef(1.0f - ofs * 1.0f / 8.0f, 0.0f, 0.0f);
						glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
						anim = ofs + 1;
					}
					else if (item.state >= 0x28 && item.state <= 0x2F)
					{
						unsigned ofs = item.state & 7;
						glTranslatef(-1.0f + ofs * 1.0f / 8.0f, 0.0f, 0.0f);
						glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
						anim = ofs + 1;
					}
					else if (item.state >= 0x20 && item.state <= 0x27)
					{
						unsigned ofs = item.state & 7;
						glTranslatef(0.0f, 1.0f - ofs * 1.0f / 8.0f, 0.0f);
						glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
						anim = ofs + 1;
					}
				}
				R_RenderMesh(m_SnikSnak[anim]);
				glPopMatrix();
				break;

			case MAP_TERMINAL:
				R_SetShaderUniform(m_TerminalPixelShader, 1, terminal[(item.state & 0x7F) & 3]);
				R_RenderMesh(m_TerminalFront);
				R_RenderMesh(m_Meshes[MAP_TERMINAL]);
				break;

			case MAP_ELECTRON:
				if (item.state >= 0x10 && item.state <= 0x17)
				{
					unsigned ofs = item.state & 7;
					glTranslatef(0.0f, -1.0f + ofs * 1.0f / 8.0f, 0.0f);
				}
				else if (item.state >= 0x18 && item.state <= 0x1F)
				{
					unsigned ofs = item.state & 7;
					glTranslatef(1.0f - ofs * 1.0f / 8.0f, 0.0f, 0.0f);
				}
				else if (item.state >= 0x28 && item.state <= 0x2F)
				{
					unsigned ofs = item.state & 7;
					glTranslatef(-1.0f + ofs * 1.0f / 8.0f, 0.0f, 0.0f);
				}
				else if (item.state >= 0x20 && item.state <= 0x27)
				{
					unsigned ofs = item.state & 7;
					glTranslatef(0.0f, 1.0f - ofs * 1.0f / 8.0f, 0.0f);
				}
				renderElectron();
				break;

			default:
				real = item.id;
				if (real == MAP_RAM_CHIP || real == MAP_HARDWARE)
					real = m_Game.getRealItem(x, y);

				if (real == MAP_RAM_CHIP_RIGHT)
					glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				else if (real == MAP_RAM_CHIP_LEFT)
					glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

				if (real == MAP_RAM_CHIP_LEFT || real == MAP_RAM_CHIP_TOP)
				{
					glTranslatef(0.0f, -0.0110f, 0.0001f);
					glScalef(1.004f, 1.0f, 1.004f);
				}
				else if (real == MAP_HARDWARE_1)
					glTranslatef(0.0f, 0.0f, 0.2f);

				if (real < m_Meshes.size() && m_Meshes[real])
					R_RenderMesh(m_Meshes[real]);
			}
			glPopMatrix();
		}	
	}

	if (!m_Game.isPlayerDead())
	{
		float scale, dirX = 0.0f, dirY = 0.0f;
		unsigned off = 7, idx = 0;
		double equ[4];

		switch (murphy.state)
		{
		case MURPHY_STATE_MOVE_UP_EAT_BASE:
		case MURPHY_STATE_MOVE_UP_EAT_INFTR:
		case MURPHY_STATE_MOVE_UP_EAT_BOMB:
			dirY = 1.0f;
			goto _l0;
		case MURPHY_STATE_MOVE_DOWN_EAT_BASE:
		case MURPHY_STATE_MOVE_DOWN_EAT_INFTR:
		case MURPHY_STATE_MOVE_DOWN_EAT_BOMB:
			dirY = -1.0f;
			goto _l0;
		case MURPHY_STATE_MOVE_LEFT_EAT_BASE:
		case MURPHY_STATE_MOVE_LEFT_EAT_INFTR:
		case MURPHY_STATE_MOVE_LEFT_EAT_BOMB:
			dirX = -1.0f;
			goto _l0;
		case MURPHY_STATE_MOVE_RIGHT_EAT_BASE:
		case MURPHY_STATE_MOVE_RIGHT_EAT_INFTR:
		case MURPHY_STATE_MOVE_RIGHT_EAT_BOMB:
			dirX = 1.0f;
		_l0:
			off = (8 - m_Game.animFramesLeft());
			glPushMatrix();
			glTranslatef(playerPos.x(), playerPos.y(), 0.0f);
			equ[0] = dirX;
			equ[1] = dirY;
			equ[2] = 0.0f;
			equ[3] = -0.5f;
			glClipPlane(GL_CLIP_PLANE0, equ);
			glPopMatrix();
			glPushMatrix();
			glEnable(GL_CLIP_PLANE0);
			if (murphy.state == MURPHY_STATE_MOVE_DOWN_EAT_INFTR ||
					murphy.state == MURPHY_STATE_MOVE_UP_EAT_INFTR ||
					murphy.state == MURPHY_STATE_MOVE_LEFT_EAT_INFTR ||
					murphy.state == MURPHY_STATE_MOVE_RIGHT_EAT_INFTR)
			{
				glTranslatef(murphy_x, murphy_y, 0.0f);
				renderInfotron();
			}
			else if (murphy.state == MURPHY_STATE_MOVE_DOWN_EAT_BOMB ||
					murphy.state == MURPHY_STATE_MOVE_UP_EAT_BOMB)
			{
				glTranslatef(murphy_x, murphy_y + dirY, 0.0f);
				R_RenderMesh(m_Meshes[MAP_DISK_RED]);
			}
			else if (murphy.state == MURPHY_STATE_MOVE_LEFT_EAT_BOMB ||
					murphy.state == MURPHY_STATE_MOVE_RIGHT_EAT_BOMB)
			{
				glTranslatef(murphy_x, murphy_y, 0.0f);
				R_RenderMesh(m_Meshes[MAP_DISK_RED]);
			}
			else
			{
				glTranslatef(murphy_x, murphy_y, 0.0f);
				R_RenderMesh(m_Meshes[MAP_BASE]);
			}
			glDisable(GL_CLIP_PLANE0);
			glPopMatrix();
			break;

		case MURPHY_STATE_EAT_BASE_TOP:
		case MURPHY_STATE_EAT_INFOTRON_TOP:
		case MURPHY_STATE_EAT_BOMB_TOP:
			dirY = 1.0f;
			goto _l2;
		case MURPHY_STATE_EAT_BASE_LEFT:
		case MURPHY_STATE_EAT_INFOTRON_LEFT:
		case MURPHY_STATE_EAT_BOMB_LEFT:
			dirX = -1.0f;
			goto _l2;
		case MURPHY_STATE_EAT_BASE_RIGHT:
		case MURPHY_STATE_EAT_INFOTRON_RIGHT:
		case MURPHY_STATE_EAT_BOMB_RIGHT:
			dirX = 1.0f;
			goto _l2;
		case MURPHY_STATE_EAT_BASE_BOTTOM:
		case MURPHY_STATE_EAT_INFOTRON_BOTTOM:
		case MURPHY_STATE_EAT_BOMB_BOTTOM:
			dirY = -1.0f;
		_l2:
			off = (8 - m_Game.animFramesLeft());
			glPushMatrix();
			glTranslatef(murphy_x + dirX, murphy_y + dirY, 0.0f);
			scale = (float)(7 - (off & 0x7)) * 1.0f / 7.0f;
			glScalef(scale, scale, scale);
			if (murphy.state == MURPHY_STATE_EAT_INFOTRON_TOP ||
					murphy.state == MURPHY_STATE_EAT_INFOTRON_RIGHT ||
					murphy.state == MURPHY_STATE_EAT_INFOTRON_LEFT ||
					murphy.state == MURPHY_STATE_EAT_INFOTRON_BOTTOM)
				renderInfotron();
			else if (murphy.state == MURPHY_STATE_EAT_BOMB_TOP ||
					murphy.state == MURPHY_STATE_EAT_BOMB_RIGHT ||
					murphy.state == MURPHY_STATE_EAT_BOMB_LEFT ||
					murphy.state == MURPHY_STATE_EAT_BOMB_BOTTOM)
				R_RenderMesh(m_Meshes[MAP_DISK_RED]);
			else
				R_RenderMesh(m_Meshes[MAP_BASE]);
			glPopMatrix();
			break;

		case MURPHY_STATE_SHIFT_ZONK_LEFT:
		case MURPHY_STATE_SHIFT_ORANGE_DISK_LF:
		case MURPHY_STATE_MOVE_YELLOW_DISK_LF:
			off = (8 - m_Game.animFramesLeft());
			glPushMatrix();
			glTranslatef(playerPos.x() - 1.0f, playerPos.y(), 0.0f);
			if (murphy.state == MURPHY_STATE_SHIFT_ORANGE_DISK_LF)
				R_RenderMesh(m_Meshes[MAP_DISK_ORANGE]);
			else if (murphy.state == MURPHY_STATE_MOVE_YELLOW_DISK_LF)
				R_RenderMesh(m_Meshes[MAP_DISK_YELLOW]);
			else
			{
				glRotatef((float)((off & 0x7)) * 45.0f, 0.0f, 0.0f, 1.0f);
				R_RenderMesh(m_Meshes[MAP_ZONK]);
			}
			glPopMatrix();
			break;

		case MURPHY_STATE_SHIFT_ZONK_RIGHT:
		case MURPHY_STATE_SHIFT_ORANGE_DISK_RT:
		case MURPHY_STATE_MOVE_YELLOW_DISK_RT:
			off = (8 - m_Game.animFramesLeft());
			glPushMatrix();
			glTranslatef(playerPos.x() + 1.0f, playerPos.y(), 0.0f);
			if (murphy.state == MURPHY_STATE_SHIFT_ORANGE_DISK_RT)
				R_RenderMesh(m_Meshes[MAP_DISK_ORANGE]);
			else if (murphy.state == MURPHY_STATE_MOVE_YELLOW_DISK_RT)
				R_RenderMesh(m_Meshes[MAP_DISK_YELLOW]);
			else
			{
				glRotatef((float)(8 - (off & 0x7)) * 45.0f, 0.0f, 0.0f, 1.0f);
				R_RenderMesh(m_Meshes[MAP_ZONK]);
			}
			glPopMatrix();
			break;

		case MURPHY_STATE_MOVE_YELLOW_DISK_DN:
			glPushMatrix();
			glTranslatef(playerPos.x(), playerPos.y() - 1.0f, 0.0f);
			R_RenderMesh(m_Meshes[MAP_DISK_YELLOW]);
			glPopMatrix();
			break;

		case MURPHY_STATE_MOVE_YELLOW_DISK_UP:
			glPushMatrix();
			glTranslatef(playerPos.x(), playerPos.y() + 1.0f, 0.0f);
			R_RenderMesh(m_Meshes[MAP_DISK_YELLOW]);
			glPopMatrix();
			break;

		case MURPHY_STATE_TELEPORT_RIGHT:
		case MURPHY_STATE_TELEPORT_LEFT:
		case MURPHY_STATE_TELEPORT_UP:
		case MURPHY_STATE_TELEPORT_DOWN:
			off = (8 - m_Game.animFramesLeft());
			break;

		case MURPHY_STATE_EXIT:
			off = (41 - m_Game.animFramesLeft());
			break;
		}

		static byte murphyState = 0xFF, stateTimer = 0;
		if (!m_SkipThisFrame)
		{
			if (stateTimer > 0)
			{
				stateTimer--;
				if (stateTimer == 0)
					murphyState = murphy.state;
			}

			if (murphyState != murphy.state)
			{
				if (murphyState == 0xFF || murphy.state != 0)
					murphyState = murphy.state;
				else if (stateTimer == 0)
				{
					stateTimer = 5;
					if (murphyState == MURPHY_STATE_TELEPORT_RIGHT)
						murphyState = MURPHY_STATE_MOVE_RIGHT_EAT_SPACE;
					else if (murphyState == MURPHY_STATE_TELEPORT_LEFT)
						murphyState = MURPHY_STATE_MOVE_LEFT_EAT_SPACE;
					else if (murphyState == MURPHY_STATE_TELEPORT_UP)
						murphyState = MURPHY_STATE_MOVE_UP_EAT_SPACE;
					else if (murphyState == MURPHY_STATE_TELEPORT_DOWN)
						murphyState = MURPHY_STATE_MOVE_DOWN_EAT_SPACE;
				}
			}
		}

		glPushMatrix();
		glTranslatef(playerPos.x(), playerPos.y(), 0.0f);
		if (murphyState == MURPHY_STATE_MOVE_LEFT_EAT_BASE ||
			murphyState == MURPHY_STATE_MOVE_LEFT_EAT_INFTR ||
			murphyState == MURPHY_STATE_MOVE_LEFT_EAT_BOMB ||
			murphyState == MURPHY_STATE_MOVE_LEFT_EAT_SPACE ||
			murphyState == MURPHY_STATE_TELEPORT_LEFT)
		{
			idx = 7 - m_Game.animFramesLeft();
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_MOVE_RIGHT_EAT_BASE ||
			murphyState == MURPHY_STATE_MOVE_RIGHT_EAT_INFTR ||
			murphyState == MURPHY_STATE_MOVE_RIGHT_EAT_BOMB ||
			murphyState == MURPHY_STATE_MOVE_RIGHT_EAT_SPACE ||
			murphyState == MURPHY_STATE_TELEPORT_RIGHT)
		{
			idx = 7 - m_Game.animFramesLeft();
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_MOVE_UP_EAT_BASE ||
			murphyState == MURPHY_STATE_MOVE_UP_EAT_INFTR ||
			murphyState == MURPHY_STATE_MOVE_UP_EAT_BOMB ||
			murphyState == MURPHY_STATE_MOVE_UP_EAT_SPACE ||
			murphyState == MURPHY_STATE_TELEPORT_UP)
		{
			idx = 7 - m_Game.animFramesLeft();
			if (m_Game.playerLooksRight())
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_MOVE_DOWN_EAT_BASE ||
			murphyState == MURPHY_STATE_MOVE_DOWN_EAT_INFTR ||
			murphyState == MURPHY_STATE_MOVE_DOWN_EAT_BOMB ||
			murphyState == MURPHY_STATE_MOVE_DOWN_EAT_SPACE ||
			murphyState == MURPHY_STATE_TELEPORT_DOWN)
		{
			idx = 7 - m_Game.animFramesLeft();
			if (m_Game.playerLooksRight())
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_SHIFT_ORANGE_DISK_LF ||
				 murphyState == MURPHY_STATE_MOVE_YELLOW_DISK_LF ||
				 murphyState == MURPHY_STATE_SHIFT_ZONK_LEFT)
		{
			idx = 8;
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_SHIFT_ORANGE_DISK_RT ||
				 murphyState == MURPHY_STATE_MOVE_YELLOW_DISK_RT ||
				 murphyState == MURPHY_STATE_SHIFT_ZONK_RIGHT)
		{
			idx = 8;
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_MOVE_YELLOW_DISK_UP)
		{
			idx = 8;
			if (m_Game.playerLooksRight())
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_MOVE_YELLOW_DISK_DN)
		{
			idx = 8;
			if (m_Game.playerLooksRight())
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_EXIT)
		{
			Vector2 exit = m_Game.getExitRelativePos();

			float step = off / 35.0f;
			glTranslatef(step * exit.x(), step * -exit.y(), 0.0f);

			glRotatef(off * 360.0f / 20.0f, 0.0f, 1.0f, 0.0f);

			float scale = (41 - off) * 1.0f / 41.0f;
			glScalef(scale, scale, scale);
		}
		else if (murphyState == MURPHY_STATE_EAT_BASE_TOP || 
				 murphyState == MURPHY_STATE_EAT_INFOTRON_TOP ||
				 murphyState == MURPHY_STATE_EAT_BOMB_TOP)
		{
			glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_EAT_BASE_BOTTOM || 
				 murphyState == MURPHY_STATE_EAT_INFOTRON_BOTTOM ||
				 murphyState == MURPHY_STATE_EAT_BOMB_BOTTOM)
		{
			glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_EAT_BASE_LEFT || 
				 murphyState == MURPHY_STATE_EAT_INFOTRON_LEFT ||
				 murphyState == MURPHY_STATE_EAT_BOMB_LEFT)
		{
			glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (murphyState == MURPHY_STATE_EAT_BASE_RIGHT || 
				 murphyState == MURPHY_STATE_EAT_INFOTRON_RIGHT ||
				 murphyState == MURPHY_STATE_EAT_BOMB_RIGHT)
		{
			glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
		}
		else if (m_Game.getRedDiskDetonateTimer() != 0 && m_Game.getRedDiskPlacementTimer() <= 32)
		{
			idx = 9;
		}

		if (murphyState == MURPHY_STATE_TELEPORT_RIGHT || murphyState == MURPHY_STATE_TELEPORT_LEFT ||
			murphyState == MURPHY_STATE_TELEPORT_DOWN || murphyState == MURPHY_STATE_TELEPORT_UP)
		{
			float scale;

			if (m_Game.getTeleportType() == MAP_PORT_CROSS)
				scale = (off < 4 ? (8 - off) * 1.0f / 8.0f: off * 1.0f / 8.0f) * 0.8f;
			else
				scale = 0.2f + (off < 4 ? (8 - off) * 1.0f / 8.0f: off * 1.0f / 8.0f) * 0.8f;

			glScalef(scale, scale, scale);
			idx = 0;
		}

		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		R_RenderMesh(m_Murphy[idx]);
		glPopMatrix();
	}

	if (m_Game.getRedDiskDetonateTimer() > 1)
	{
		Vector2 pos = m_Game.getRedDiskPosition();

		glPushMatrix();
		glTranslatef(pos.x(), -pos.y(), 0.0f);
		R_RenderMesh(m_Meshes[MAP_DISK_RED]);
		glPopMatrix();
	}	
}

//
// Renders the GUI
//
void SuperplexGame::renderGUI()
{
}

//
// Runs the AI
//
void SuperplexGame::runAI()
{
	animateInfotron();
	animateElectron();

	if (!m_SkipThisFrame)
	{
		R_UpdateLightning(m_Lightning);
		m_Game.runAI();
	}	
}

//
// Handles the player's input
//
void SuperplexGame::runPlayer()
{
	double currentTime = Sys_GetCurrentTime();
	const double frameLength = 0.04;

	m_SkipThisFrame = (m_PrevFrameTime != 0.0f && currentTime - m_PrevFrameTime < frameLength);
	if (!m_SkipThisFrame)
		m_PrevFrameTime += frameLength;

	if (!m_SkipThisFrame)
	{
		JoystickButton button = JB_NONE;

		bool spacePressed = IN_IsKeyPressed(KeySpace);
		if (IN_IsKeyPressed(KeyEsc))	
			m_Game.killPlayer();
		else if (IN_IsKeyPressed(KeyUp))
			button = (spacePressed ? JB_SPACE_UP : JB_UP);
		else if (IN_IsKeyPressed(KeyDown))
			button = (spacePressed ? JB_SPACE_DOWN : JB_DOWN);
		else if (IN_IsKeyPressed(KeyLeft))
			button = (spacePressed ? JB_SPACE_LEFT : JB_LEFT);
		else if (IN_IsKeyPressed(KeyRight))
			button = (spacePressed ? JB_SPACE_RIGHT : JB_RIGHT);
		else if (spacePressed)
			button = JB_SPACE;

		m_Game.handlePlayerMovement(button);
	}

	if (m_Game.isLevelComplete())
		exit(1);
}

//
// Animates the infotron
//
void SuperplexGame::animateInfotron()
{
	double delta = Sys_GetLastFrameTime();

 	m_InfotronFrame += delta * 40.0f;
	m_InfotronAngle += delta * 180.0f;
	while (unlikely(m_InfotronFrame >= (double)m_Infotron.size()))
		m_InfotronFrame -= (double)m_Infotron.size();
	while (unlikely(m_InfotronAngle >= 360.0))
		m_InfotronAngle -= 360.0;
}

//
// Renders the infotron
//
void SuperplexGame::renderInfotron()
{
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	glRotatef((float)m_InfotronAngle, 0.0f, 0.0f, 1.0f);
	glRotatef(-(float)m_InfotronAngle, 0.0f, 1.0f, 0.0f);
	R_RenderMesh(m_Infotron[(int)m_InfotronFrame]);
}

static float scale[9] = { 0.0f, 0.1f, 0.8f, 0.5f, 0.3f, 0.2f, 0.9f, 0.4f, 1.0f };
//static float step[9] = { 1.0f, 1.2f, 1.4f, 1.6f, 1.8f, 2.0f, 1.7f, 0.6f, 0.8f };
static float step[9] = { 1.0f, 1.2f, 1.4f, 1.6f, 1.8f, 2.0f, 1.6f, 1.4f, 1.0f };

//
// Animates the electron
//
void SuperplexGame::animateElectron()
{
	double delta = Sys_GetLastFrameTime();

	for (int i = 0; i < 9; i++)
	{
		scale[i] += (float)(delta * step[i]);
		if (scale[i] > 1.0f)
		{
			scale[i] = 1.0f;
			step[i] = -step[i];
		}	
		if (scale[i] < 0.0f)
		{
			scale[i] = 0.0f;
			step[i] = -step[i];
		}	
	}
}	

//
// Renders the electron
//
void SuperplexGame::renderElectron()
{
/*	if (scale[0] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(-0.25f, -0.25f, -0.25f);
		glScalef(scale[0] * 0.3f, scale[0] * 0.3f, scale[0] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}

	if (scale[1] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(0.25f, -0.25f, -0.25f);
		glScalef(scale[1] * 0.3f, scale[1] * 0.3f, scale[1] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}	

	if (scale[2] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(0.25f, 0.25f, -0.25f);
		glScalef(scale[2] * 0.3f, scale[2] * 0.3f, scale[2] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}	

	if (scale[3] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(-0.25f, 0.25f, -0.25f);
		glScalef(scale[3] * 0.3f, scale[3] * 0.3f, scale[3] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}*/

	if (scale[4] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(-0.25f, -0.25f, 0.0f);
		glScalef(scale[4] * 0.3f, scale[4] * 0.3f, scale[4] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}

	if (scale[5] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(0.25f, -0.25f, 0.0f);
		glScalef(scale[5] * 0.3f, scale[5] * 0.3f, scale[5] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}

	if (scale[6] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(0.25f, 0.25f, 0.0f);
		glScalef(scale[6] * 0.3f, scale[6] * 0.3f, scale[6] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}

	if (scale[7] > 0.0f)
	{
		glPushMatrix();
		glTranslatef(-0.25f, 0.25f, 0.0f);
		glScalef(scale[7] * 0.3f, scale[7] * 0.3f, scale[7] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}

	if (scale[8] > 0.0f)
	{
		glPushMatrix();
		glScalef(scale[8] * 0.3f, scale[8] * 0.3f, scale[8] * 0.3f);
		R_RenderMesh(m_Star);
		glPopMatrix();
	}
}
