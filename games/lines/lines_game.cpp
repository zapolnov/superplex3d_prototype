#include <engine/render.h>
#include <engine/input.h>
#include <opengl/context.h>
#include <core/logger.h>
#include <engine/system.h>
#include <engine/filesys.h>
#include <engine/r_opengl.h>
#include <sound/functions.h>
#include "lines_game.h"
#include "ball.h"
#include "a_star.h"
#include <stdlib.h>

GAME_MAIN(LinesGame)

/* Constants */

const float g_CellWidth = 2.0f;
const float g_CellHeight = 2.0f;
const float g_CenterX = ((GAME_FIELD_WIDTH - 1) * g_CellWidth) / 2.0f;
const float g_CenterY = ((GAME_FIELD_HEIGHT - 1) * g_CellHeight) / 2.0f;


/* LinesGame methods */

//
// Constructor
//
LinesGame::LinesGame()
	: m_State(GameOver), m_SelectedBall(NULL), m_SelectedBallPos(-1, -1), m_Score(0),
	  m_ButtonClicked(-1), m_StartNewGameDialogShown(false), m_AboutDialogShown(false),
	  m_SettingsDialogShown(false)
{
	FS_INIT_ZIP();

	m_LargeFont = R_LoadFont("arial25.fnt");
	m_MenuFont = R_LoadFont("arial13.fnt");

	m_ScoreStr = L"Score: %lld";
	m_StartNewGameMsg = L"Do you really wish to start a new game?";
	m_StartNewGameMsgSize = R_CalcStringMetrics(m_LargeFont, m_StartNewGameMsg);
	m_YesMsg  = L"Yes";
	m_YesMsgSize = R_CalcStringMetrics(m_MenuFont, m_YesMsg);
	m_NoMsg  = L"No";
	m_NoMsgSize = R_CalcStringMetrics(m_MenuFont, m_NoMsg);
	m_OkMsg  = L"OK";
	m_OkMsgSize = R_CalcStringMetrics(m_MenuFont, m_OkMsg);
	m_CancelMsg  = L"Cancel";
	m_CancelMsgSize = R_CalcStringMetrics(m_MenuFont, m_CancelMsg);
	m_NextMsg = L"Next:";
	m_NextMsgSize = R_CalcStringMetrics(m_MenuFont, m_NextMsg);

	m_AboutMsg[0] = L"The Balls Game";
	m_AboutMsg[1] = L"Version 1.0";
	m_AboutMsg[2] = L"";
	m_AboutMsg[3] = L"Copyright \x00A9 2009 Nikolay Zapolnov";
	m_AboutMsg[4] = L"All rights reserved.";

	for (int i = 0; i < 5; i++)
		m_AboutMsgSize[i] = R_CalcStringMetrics(m_LargeFont, m_AboutMsg[i]);

	Q_ASSERT(NUM_BUTTONS == 3);
	m_ButtonTitles[0] = L"New game";
	m_ButtonTitles[1] = L"Settings";
	m_ButtonTitles[2] = L"About";

	for (int i = 0; i < NUM_BUTTONS; i++)
		m_ButtonTitleSizes[i] = R_CalcStringMetrics(m_MenuFont, m_ButtonTitles[i]);

	m_Camera = new Camera;
	m_Camera->setNearFarPlanes(1, 1000);
	m_Camera->lookAt(Vector3(0.0f, 16.0f, 20.0f), Vector3(0.0f, -4.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));

	m_FieldTexture = R_LoadTexture("marble_green.jpg");
	m_GameOver = R_LoadTexture("game_over_eng.dds");
	m_GameOverSize = R_GetTextureSize(m_GameOver);
	m_Button = R_LoadTexture("button.dds");
	m_ButtonPressed = R_LoadTexture("button_pressed.dds");
	m_ButtonSize = R_GetTextureSize(m_Button);

	memset(m_GameField, 0, sizeof(m_GameField));
	memset(m_NextBalls, 0, sizeof(m_NextBalls));

	SND_SetListenerPosition(Vector3(0.0f, 16.0f, 20.0f));
	SND_SetListenerOrientation(Vector3(0.0f, -1.0f, 0.0f));
	SND_PreloadSound("jump.wav");
	SND_PlayMusic("music.ogg");

	startGame();
}

//
// Destructor
//
LinesGame::~LinesGame()
{
	for (int i = 0; i < 3; i++)
		delete m_NextBalls[i];

	for (int x = 0; x < GAME_FIELD_WIDTH; x++)
		for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
			delete m_GameField[x][y];
}

//
// Renders a single frame
//
void LinesGame::renderFrame()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float light0_position[] = { 0.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_NORMALIZE);

	R_Set3D(m_Camera);

	glDisable(GL_LIGHTING);

	float mouseX = (m_MousePos.x() + g_CenterX) / g_CellWidth;
	float mouseY = (m_MousePos.z() + g_CenterY) / g_CellHeight;
	int cellX = (int)round(mouseX);
	int cellY = (int)round(mouseY);

	R_BindTexture(0, m_FieldTexture);
	float px = -g_CenterX - g_CellWidth / 2.0f;
	float py = -g_CenterY - g_CellHeight / 2.0f;
	const float tx_step = 1.0f / (float)GAME_FIELD_WIDTH;
	const float ty_step = 1.0f / (float)GAME_FIELD_WIDTH;
	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	glBegin(GL_QUADS);
	for (int x = 0; x < GAME_FIELD_WIDTH; x++)
	{
		for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
		{
			float x1 = px + g_CellWidth * (float)x;
			float y1 = py + g_CellHeight * (float)y;
			float x2 = px + g_CellWidth * (float)(x + 1);
			float y2 = py + g_CellHeight * (float)(y + 1);

			float tx1 = tx_step * (float)x;
			float ty1 = ty_step * (float)y;
			float tx2 = tx_step * (float)(x + 1);
			float ty2 = ty_step * (float)(y + 1);

			if (m_State == PlayerInput && !m_StartNewGameDialogShown && !m_AboutDialogShown &&
					!m_SettingsDialogShown && cellX == x && cellY == y)
				glColor4f(1.0f, 0.7f, 1.0f, 1.0f);
			else
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glTexCoord2f(tx1, ty1); glVertex3f(x1, -0.5f, y1);
			glTexCoord2f(tx2, ty1); glVertex3f(x2, -0.5f, y1);
			glTexCoord2f(tx2, ty2); glVertex3f(x2, -0.5f, y2);
			glTexCoord2f(tx1, ty2); glVertex3f(x1, -0.5f, y2);
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(2.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glBegin(GL_LINES);
	for (int x = 0; x <= GAME_FIELD_WIDTH; x++)
	{
		float x1 = px + g_CellWidth * (float)x;
		float y2 = py + GAME_FIELD_HEIGHT * g_CellHeight;

		glVertex3f(x1, -0.5f, py);
		glVertex3f(x1, -0.5f, y2);
	}
	for (int y = 0; y <= GAME_FIELD_HEIGHT; y++)
	{
		float x2 = px + GAME_FIELD_WIDTH * g_CellWidth;
		float y1 = py + g_CellHeight * (float)y;

		glVertex3f(px, -0.5f, y1);
		glVertex3f(x2, -0.5f, y1);
	}
	glEnd();

	glLineWidth(1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	for (int x = 0; x < GAME_FIELD_WIDTH; x++)
	{
		for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
		{
			glPushMatrix();
			glTranslatef((float)x * g_CellWidth - g_CenterX, 0.0f, (float)y * g_CellHeight - g_CenterY);
			glScalef(1.5f, 1.5f, 1.5f);
			if (m_GameField[x][y])
			{
				m_GameField[x][y]->update();
				m_GameField[x][y]->render();
			}
			glPopMatrix();
		}
	}

	glDisable(GL_LIGHTING);
}

//
// Renders the GUI
//
void LinesGame::renderGUI()
{
	wchar score[32];
	swprintf(score, wsizeof(score), m_ScoreStr.c_str(), m_Score);
	R_DrawString(m_LargeFont, 10, 10, score);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float x = (float)viewport[2] - 125.0f;

	glEnable(GL_LIGHT0);
	float light0_position[] = { 0.0, 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	R_DrawString(m_LargeFont, (int)x - m_NextMsgSize.width() - 35, 10, m_NextMsg);

	for (int i = 0; i < 3; i++)
	{
		if (unlikely(!m_NextBalls[i]))
			continue;

		glPushMatrix();
		glTranslatef(x + (float)i * 40.0f, 3.0f, 0.0f);

/*		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex2i( 0, 10);
			glVertex2i(30, 10);
			glVertex2i(30, 40);
			glVertex2i( 0, 40);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); */

		glEnable(GL_LIGHTING);
		glTranslatef(15.0f, 25.0f, 0.0f);
		glScalef(25.0f, 25.0f, 25.0f);
		m_NextBalls[i]->renderStatic();
		glDisable(GL_LIGHTING);

		glPopMatrix();
	}

	if (m_State == GameOver)
	{
		float imageX = (float)(viewport[2] - m_GameOverSize.width()) / 2.0f;
		float imageY = (float)(viewport[3] - m_GameOverSize.height()) / 2.0f;

		R_RenderTexture2D(imageX, imageY, m_GameOver);
	}

	float width = (float)viewport[2] / (float)NUM_BUTTONS;
	float buttonY = (float)(viewport[3] - m_ButtonSize.height() - 10);
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		float buttonX = (float)i * width + (width - (float)m_ButtonSize.width()) / 2.0f;
		drawButton(buttonX, buttonY, m_ButtonTitles[i], m_ButtonTitleSizes[i],
			!m_StartNewGameDialogShown && !m_AboutDialogShown && !m_SettingsDialogShown && m_ButtonClicked == i);
	}

	if (m_StartNewGameDialogShown)
		paintStartNewGameDialog(viewport[2], viewport[3]);
	else if (m_AboutDialogShown)
		paintAboutDialog(viewport[2], viewport[3]);
	else if (m_SettingsDialogShown)
		paintSettingsDialog(viewport[2], viewport[3]);
}

//
// Runs the AI
//
void LinesGame::runAI()
{
	if (m_State == GameOver)
		return;

	if (unlikely(!gameStillPossible()))
	{
		setGameState(GameOver);
		return;
	}

	switch (m_State)
	{
	case BallsFalling:
		for (int x = 0; x < GAME_FIELD_WIDTH; x++)
		{
			for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
			{
				if (m_GameField[x][y] && m_GameField[x][y]->isFalling())
					return;
			}
		}
		clearAllLines();
		setGameState(PlayerInput);
		break;

	case MovingBall:
		Q_ASSERT(m_SelectedBall);
		if (m_SelectedBall->isJumping())
			break;
		else if (Sys_GetCurrentTime() >= m_LastBallMoveTime + 0.1f)
		{
			if (m_BallPath.size() == 0)
			{
				m_SelectedBall = NULL;
				m_SelectedBallPos = QPoint(-1, -1);
				setGameState(CheckCompleteLines);
			}
			else
			{
				QPoint pos = m_BallPath[0];
				m_BallPath.erase(m_BallPath.begin());

				int oldX = m_SelectedBallPos.x();
				int oldY = m_SelectedBallPos.y();

				Q_ASSERT(!m_GameField[pos.x()][pos.y()]);
				Q_ASSERT(m_GameField[oldX][oldY] == m_SelectedBall);

				m_GameField[oldX][oldY] = NULL;
				m_GameField[pos.x()][pos.y()] = m_SelectedBall;

				m_SelectedBallPos = pos;
				m_LastBallMoveTime = Sys_GetCurrentTime();
			}
		}
		break;

	case CheckCompleteLines:
		if (!clearAllLines())
			spawnThreeBalls();
		else
			setGameState(PlayerInput);
		break;

	default:
		break;
	}
}

//
// Handles the player's input
//
void LinesGame::runPlayer()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	QPoint mousePos2D = IN_GetMousePos();
	m_MousePos = R_UnprojectPoint(m_Camera, mousePos2D);

	if (m_StartNewGameDialogShown)
	{
		runStartNewGameDialog(mousePos2D, viewport[2], viewport[3]);
		return;
	}
	else if (m_AboutDialogShown)
	{
		runAboutDialog(mousePos2D, viewport[2], viewport[3]);
		return;
	}
	else if (m_SettingsDialogShown)
	{
		runSettingsDialog(mousePos2D, viewport[2], viewport[3]);
		return;
	}

	if (IN_IsMouseReleased(LeftButton))
	{
		int button = m_ButtonClicked;
		m_ButtonClicked = -1;

		// Did user click on a button?
		float bwidth = (float)viewport[2] / (float)NUM_BUTTONS;
		int buttonY = viewport[3] - m_ButtonSize.height() - 10;
		for (int i = 0; i < NUM_BUTTONS; i++)
		{
			int buttonX = (int)((float)i * bwidth + (bwidth - (float)m_ButtonSize.width()) / 2.0f);
			if (mousePos2D.x() >= buttonX && mousePos2D.y() >= buttonY &&
				mousePos2D.x() < buttonX + m_ButtonSize.width() &&
				mousePos2D.y() < buttonY + m_ButtonSize.height())
			{
				if (button == i)
					buttonClicked(i);
				return;
			}	
		}
	}
	else if (IN_IsMouseClicked(LeftButton))
	{
		// Did user click on a button?
		float bwidth = (float)viewport[2] / (float)NUM_BUTTONS;
		int buttonY = viewport[3] - m_ButtonSize.height() - 10;
		for (int i = 0; i < NUM_BUTTONS; i++)
		{
			int buttonX = (int)((float)i * bwidth + (bwidth - (float)m_ButtonSize.width()) / 2.0f);
			if (mousePos2D.x() >= buttonX && mousePos2D.y() >= buttonY &&
				mousePos2D.x() < buttonX + m_ButtonSize.width() &&
				mousePos2D.y() < buttonY + m_ButtonSize.height())
			{
				m_ButtonClicked = i;
				return;
			}	
		}

		if (m_State != PlayerInput)
			return;
		if (m_MousePos.isNaN())
			return;

		float x = (m_MousePos.x() + g_CenterX) / g_CellWidth;
		float y = (m_MousePos.z() + g_CenterY) / g_CellHeight;

		int cx = (int)round(x);
		int cy = (int)round(y);

		if (cx >= 0 && cx < GAME_FIELD_WIDTH &&
			cy >= 0 && cy < GAME_FIELD_HEIGHT)
		{
			if (m_GameField[cx][cy])
			{
				if (m_SelectedBall)
					m_SelectedBall->setJumping(false);

				if (m_SelectedBall == m_GameField[cx][cy])
				{
					m_SelectedBall = NULL;
					m_SelectedBallPos = QPoint(-1, -1);
				}
				else
				{
					m_SelectedBall = m_GameField[cx][cy];
					m_SelectedBallPos = QPoint(cx, cy);
					if (m_SelectedBall)
						m_SelectedBall->setJumping(true);
				}
			}
			else if (m_SelectedBall)
			{
				AStar::A_Star pathFinder;
				int numStates;

				AStar::State state(&m_GameField, AStar::Location(m_SelectedBallPos.x(), m_SelectedBallPos.y()),
					AStar::Location(cx, cy));
				AStar::State * sol = pathFinder.solve(&state, numStates);
				if (sol && numStates > 1)
				{
					Q_ASSERT(sol[0].location().x() == m_SelectedBallPos.x());
					Q_ASSERT(sol[0].location().y() == m_SelectedBallPos.y());

				    for (int i = 1; i < numStates; i++)
						m_BallPath.push_back(sol[i].location());
					delete[] sol;

					m_SelectedBall->setJumping(false);
					m_LastBallMoveTime = Sys_GetCurrentTime();
					setGameState(MovingBall);
				}
			}
		}
	}
}

//
// Starts a new game
//
void LinesGame::startGame()
{
	for (int i = 0; i < 3; i++)
	{
		delete m_NextBalls[i];
		m_NextBalls[i] = NULL;
	}

	for (int x = 0; x < GAME_FIELD_WIDTH; x++)
	{
		for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
		{
			delete m_GameField[x][y];
			m_GameField[x][y] = NULL;
		}
	}

	m_SelectedBall = NULL;
	m_SelectedBallPos = QPoint(-1, -1);
	m_Score = 0;
	m_BallPath.clear();
	m_LastBallMoveTime = 0.0;
	m_ButtonClicked = -1;

	generateNextBalls();
	spawnThreeBalls();
}

//
// Spawns three balls in random fields of the map
//
void LinesGame::spawnThreeBalls()
{
	setGameState(BallsFalling);

	if (m_SelectedBall)
	{
		m_SelectedBall->setJumping(false);
		m_SelectedBall = NULL;
		m_SelectedBallPos = QPoint(-1, -1);
	}

	for (int i = 0; i < 3; i++)
	{
		if (!gameStillPossible())
			break;

		int cx, cy;
		do
		{
			cx = rand() % GAME_FIELD_WIDTH;
			cy = rand() % GAME_FIELD_HEIGHT;
		}
		while (m_GameField[cx][cy] != NULL);

		Q_ASSERT(m_NextBalls[i]);
		m_GameField[cx][cy] = m_NextBalls[i];
		m_NextBalls[i] = NULL;
	}

	for (int i = 0; i < 3; i++)
	{
		delete m_NextBalls[i];
		m_NextBalls[i] = NULL;
	}	

	generateNextBalls();
}

//
// Returns true if there is at least one free cell on the game field
//
bool LinesGame::gameStillPossible()
{
	for (int x = 0; x < GAME_FIELD_WIDTH; x++)
	{
		for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
		{
			if (m_GameField[x][y] == NULL)
				return true;
		}
	}

	return false;
}

//
// Sets the game state
//
void LinesGame::setGameState(GameState state)
{
	if (m_State == state)
		return;

	m_State = state;

	switch (state)
	{
	case PlayerInput:
		logger << LOG_TRACE << "Switch to PlayerInput";
		break;

	case MovingBall:
		logger << LOG_TRACE << "Switch to MovingBall";
		break;

	case BallsFalling:
		logger << LOG_TRACE << "Switch to BallsFalling";
		break;

	case CheckCompleteLines:
		logger << LOG_TRACE << "Switch to CheckCompleteLines";
		break;

	case GameOver:
		logger << LOG_TRACE << "Switch to GameOver";
		break;

	default:
		Q_ASSERT(false);
	}
}

//
// Generates three random balls
//
void LinesGame::generateNextBalls()
{
	Q_ASSERT(m_NextBalls[0] == NULL);
	Q_ASSERT(m_NextBalls[1] == NULL);
	Q_ASSERT(m_NextBalls[2] == NULL);

	m_NextBalls[0] = new Ball;
	m_NextBalls[1] = new Ball;
	m_NextBalls[2] = new Ball;
}

//
// Clears all complete lines on the field
//
bool LinesGame::clearAllLines()
{
	bool result = false;

	for (int x = 0; x < GAME_FIELD_WIDTH; x++)
	{
		for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
		{
			if (!m_GameField[x][y])
				continue;

			int colorIndex = m_GameField[x][y]->colorIndex();

			int hcount = countSimilarBalls(x, y, 1, 0, colorIndex);
			int vcount = countSimilarBalls(x, y, 0, 1, colorIndex);
			int dcount1 = countSimilarBalls(x, y, 1, 1, colorIndex);
			int dcount2 = countSimilarBalls(x, y, -1, 1, colorIndex);

			bool hcomp = (hcount >= MIN_LINE_LENGTH);
			bool vcomp = (vcount >= MIN_LINE_LENGTH);
			bool dcomp1 = (dcount1 >= MIN_LINE_LENGTH);
			bool dcomp2 = (dcount2 >= MIN_LINE_LENGTH);

			if (hcomp || vcomp || dcomp1 || dcomp2)
			{
				int numDirections = 0;
				int longestRow = 0;
				int totalBalls = 0;

				if (hcomp)
				{
					clearLine(x, y, 1, 0, hcount);
					totalBalls += hcount;
					if (hcount > longestRow)
						longestRow = hcount;
					numDirections++;
				}

				if (vcomp)
				{
					clearLine(x, y, 0, 1, vcount);
					totalBalls += vcount;
					if (vcount > longestRow)
						longestRow = vcount;
					numDirections++;
				}

				if (dcomp1)
				{
					clearLine(x, y, 1, 1, dcount1);
					totalBalls += dcount1;
					if (dcount1 > longestRow)
						longestRow = dcount1;
					numDirections++;
				}

				if (dcomp2)
				{
					clearLine(x, y, -1, 1, dcount2);
					totalBalls += dcount2;
					if (dcount2 > longestRow)
						longestRow = dcount2;
					numDirections++;
				}

				Q_ASSERT(longestRow >= MIN_LINE_LENGTH);
				Q_ASSERT(totalBalls >= MIN_LINE_LENGTH);
				Q_ASSERT(numDirections > 0);

				m_Score += numDirections * 10;
				m_Score += totalBalls * 12;
				m_Score += (longestRow - MIN_LINE_LENGTH) * totalBalls * 5;

				result = true;	
			}
		}
	}

	return result;
}

//
// Counts number of similar balls in a line
//
int LinesGame::countSimilarBalls(int startX, int startY, int stepX, int stepY, int colorIndex)
{
	int count;

	int x = startX + stepX;
	int y = startY + stepY;

	for (count = 1; x >= 0 && y >= 0 && x < GAME_FIELD_WIDTH && y < GAME_FIELD_HEIGHT;
		count++, x += stepX, y += stepY)
	{
		if (!m_GameField[x][y] || m_GameField[x][y]->colorIndex() != colorIndex)
			break;
	}

	return count;
}

//
// Draws button at the specified position on the screen
//
void LinesGame::drawButton(float x, float y, const std::wstring & text, const QSize & textSize, bool pressed)
{
	R_RenderTexture2D(x, y, pressed ? m_ButtonPressed : m_Button);

	int tx = (int)(x + ((float)m_ButtonSize.width() - (float)textSize.width()) / 2.0f);
	int ty = (int)(y + ((float)m_ButtonSize.height() - (float)textSize.height()) / 2.0f) - 2;
	R_DrawString(m_MenuFont, tx + 1, ty + 1, text);
	R_DrawString(m_MenuFont, tx, ty, text, 0x000000FF);
}

//
// Clears the line of balls
//
void LinesGame::clearLine(int startX, int startY, int stepX, int stepY, int count)
{
	int x = startX;
	int y = startY;

	for (int i = 0; i < count; i++, x += stepX, y += stepY)
	{
		Q_ASSERT(m_GameField[x][y] != m_SelectedBall);
		delete m_GameField[x][y];
		m_GameField[x][y] = NULL;
	}
}

//
// Called when user clicks a button
//
void LinesGame::buttonClicked(int idx)
{
	switch (idx)
	{
	case 0:
		logger << LOG_TRACE << "User clicked 'New Game'";
		if (m_State == GameOver)
			startGame();
		else
			m_StartNewGameDialogShown = true;
		break;

	case 1:
		logger << LOG_TRACE << "User clicked 'Settings'";
		m_SettingsDialogShown = true;
		break;

	case 2:
		logger << LOG_TRACE << "User clicked 'About'";
		m_AboutDialogShown = true;
		break;

	default:
		Q_ASSERT(false);
	}	
}

//
// Paints the 'Start a new game' dialog
//
void LinesGame::paintStartNewGameDialog(int screenWidth, int screenHeight)
{
	const int dialogW = 500;
	const int dialogH = 150;

	int dialogX = (screenWidth - dialogW) / 2;
	int dialogY = (screenHeight - dialogH) / 2;

	glDisable(GL_TEXTURE_2D);

	glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
	glBegin(GL_QUADS);
		glVertex2i(dialogX, dialogY);
		glVertex2i(dialogX + dialogW, dialogY);
		glVertex2i(dialogX + dialogW, dialogY + dialogH);
		glVertex2i(dialogX, dialogY + dialogH);
	glEnd();

	int msgX = (dialogW - m_StartNewGameMsgSize.width()) / 2;
	R_DrawString(m_LargeFont, dialogX + msgX, dialogY + 40, m_StartNewGameMsg);

	int width = m_ButtonSize.width() * 2 + 10;
	float buttonX = (float)(dialogX + (dialogW - width) / 2.0f);
	float buttonY = (float)(dialogY + dialogH - m_ButtonSize.height() - 10);
	drawButton(buttonX, buttonY, m_YesMsg, m_YesMsgSize, m_ButtonClicked == 0);
	buttonX += (float)m_ButtonSize.width() + 10.0f;
	drawButton(buttonX, buttonY, m_NoMsg, m_NoMsgSize, m_ButtonClicked == 1);
}

//
// Handles input for the 'Start a new game' dialog
//
void LinesGame::runStartNewGameDialog(const QPoint & mousePos2D, int screenWidth, int screenHeight)
{
	const int dialogW = 500;
	const int dialogH = 150;

	int dialogX = (screenWidth - dialogW) / 2;
	int dialogY = (screenHeight - dialogH) / 2;

	int width = m_ButtonSize.width() * 2 + 10;
	int buttonX = dialogX + (dialogW - width) / 2;
	int buttonY = dialogY + dialogH - m_ButtonSize.height() - 10;

	int buttonID = -1;
	if (mousePos2D.x() >= buttonX && mousePos2D.x() < buttonX + m_ButtonSize.width() &&
			mousePos2D.y() >= buttonY && mousePos2D.y() < buttonY + m_ButtonSize.height())
		buttonID = 0;
	else
	{
		buttonX += m_ButtonSize.width() + 10;
		if (mousePos2D.x() >= buttonX && mousePos2D.x() < buttonX + m_ButtonSize.width() &&
				mousePos2D.y() >= buttonY && mousePos2D.y() < buttonY + m_ButtonSize.height())
			buttonID = 1;
	}

	if (IN_IsMouseClicked(LeftButton))
		m_ButtonClicked = buttonID;
	else if (IN_IsMouseReleased(LeftButton) && m_ButtonClicked != -1)
	{
		if (buttonID == m_ButtonClicked)
		{
		 	if (buttonID == 0)
				startGame();
			m_StartNewGameDialogShown = false;
		}	
		m_ButtonClicked = -1;
		return;
	}
}

//
// Paints the 'About' dialog
//
void LinesGame::paintAboutDialog(int screenWidth, int screenHeight)
{
	const int dialogW = 500;
	const int dialogH = 250;

	int dialogX = (screenWidth - dialogW) / 2;
	int dialogY = (screenHeight - dialogH) / 2;

	glDisable(GL_TEXTURE_2D);

	glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
	glBegin(GL_QUADS);
		glVertex2i(dialogX, dialogY);
		glVertex2i(dialogX + dialogW, dialogY);
		glVertex2i(dialogX + dialogW, dialogY + dialogH);
		glVertex2i(dialogX, dialogY + dialogH);
	glEnd();

	int msgY = dialogY + 40;
	for (int i = 0; i < 5; i++)
	{
		int msgX = (dialogW - m_AboutMsgSize[i].width()) / 2;
		R_DrawString(m_LargeFont, dialogX + msgX, msgY, m_AboutMsg[i]);
		msgY += m_AboutMsgSize[i].height();
	}

	float buttonX = (float)(dialogX + (dialogW - m_ButtonSize.width()) / 2);
	float buttonY = (float)(dialogY + dialogH - m_ButtonSize.height() - 10);

	drawButton(buttonX, buttonY, m_OkMsg, m_OkMsgSize, m_ButtonClicked == 0);
}

//
// Handles input for the 'About' dialog
//
void LinesGame::runAboutDialog(const QPoint & mousePos2D, int screenWidth, int screenHeight)
{
	const int dialogW = 500;
	const int dialogH = 250;

	int dialogX = (screenWidth - dialogW) / 2;
	int dialogY = (screenHeight - dialogH) / 2;

	int buttonX = dialogX + (dialogW - m_ButtonSize.width()) / 2;
	int buttonY = dialogY + dialogH - m_ButtonSize.height() - 10;

	int buttonID = -1;
	if (mousePos2D.x() >= buttonX && mousePos2D.x() < buttonX + m_ButtonSize.width() &&
			mousePos2D.y() >= buttonY && mousePos2D.y() < buttonY + m_ButtonSize.height())
		buttonID = 0;

	if (IN_IsMouseClicked(LeftButton))
		m_ButtonClicked = buttonID;
	else if (IN_IsMouseReleased(LeftButton) && m_ButtonClicked != -1)
	{
		if (buttonID == m_ButtonClicked)
			m_AboutDialogShown = false;
		m_ButtonClicked = -1;
	}
}

//
// Paints the 'Settings' dialog
//
void LinesGame::paintSettingsDialog(int screenWidth, int screenHeight)
{
	const int dialogW = 500;
	const int dialogH = 350;

	int dialogX = (screenWidth - dialogW) / 2;
	int dialogY = (screenHeight - dialogH) / 2;

	glDisable(GL_TEXTURE_2D);

	glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
	glBegin(GL_QUADS);
		glVertex2i(dialogX, dialogY);
		glVertex2i(dialogX + dialogW, dialogY);
		glVertex2i(dialogX + dialogW, dialogY + dialogH);
		glVertex2i(dialogX, dialogY + dialogH);
	glEnd();

//	int msgX = (dialogW - m_StartNewGameMsgSize.width()) / 2;
//	R_DrawString(m_LargeFont, dialogX + msgX, dialogY + 40, m_StartNewGameMsg);

	int width = m_ButtonSize.width() * 2 + 10;
	float buttonX = (float)(dialogX + (dialogW - width) / 2.0f);
	float buttonY = (float)(dialogY + dialogH - m_ButtonSize.height() - 10);
	drawButton(buttonX, buttonY, m_OkMsg, m_OkMsgSize, m_ButtonClicked == 0);
	buttonX += (float)m_ButtonSize.width() + 10.0f;
	drawButton(buttonX, buttonY, m_CancelMsg, m_CancelMsgSize, m_ButtonClicked == 1);
}

//
// Handles input for the 'Settings' dialog
//
void LinesGame::runSettingsDialog(const QPoint & mousePos2D, int screenWidth, int screenHeight)
{
	const int dialogW = 500;
	const int dialogH = 350;

	int dialogX = (screenWidth - dialogW) / 2;
	int dialogY = (screenHeight - dialogH) / 2;

	int width = m_ButtonSize.width() * 2 + 10;
	int buttonX = dialogX + (dialogW - width) / 2;
	int buttonY = dialogY + dialogH - m_ButtonSize.height() - 10;

	int buttonID = -1;
	if (mousePos2D.x() >= buttonX && mousePos2D.x() < buttonX + m_ButtonSize.width() &&
			mousePos2D.y() >= buttonY && mousePos2D.y() < buttonY + m_ButtonSize.height())
		buttonID = 0;
	else
	{
		buttonX += m_ButtonSize.width() + 10;
		if (mousePos2D.x() >= buttonX && mousePos2D.x() < buttonX + m_ButtonSize.width() &&
				mousePos2D.y() >= buttonY && mousePos2D.y() < buttonY + m_ButtonSize.height())
			buttonID = 1;
	}

	if (IN_IsMouseClicked(LeftButton))
		m_ButtonClicked = buttonID;
	else if (IN_IsMouseReleased(LeftButton) && m_ButtonClicked != -1)
	{
		if (buttonID == m_ButtonClicked)
		{
		 	if (buttonID == 0)
		 	{
		 		// FIXME: TODO
		 	}
			m_SettingsDialogShown = false;
		}	
		m_ButtonClicked = -1;
		return;
	}
}
