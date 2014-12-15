#include <engine/filesys.h>
#include <engine/input.h>
#include <sound/functions.h>
#include "supaplex_game.h"
#include "supaplex_def.h"
#include <stdlib.h>

#ifdef WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
#endif

/* Private functions */

//
// Converts map offset into 3D position
//
static
Vector3 offsetTo3D(uint offset)
{
	return Vector3
	(
		(float)(offset % MAP_WIDTH) * 0.1f,
		-(float)(offset / MAP_WIDTH) * 0.1f,
		0.0f
	);
}


/* SupaplexGame methods */

//
// Constructor
//
SupaplexLogic::SupaplexLogic()
	: m_LevelComplete(false)
{
}

//
// Destructor
//
SupaplexLogic::~SupaplexLogic()
{
}

//
// Starts new game
//
void SupaplexLogic::startGame(int index)
{
	m_LevelComplete = false;
	m_CurrentFrameNumber = 0xF000;
	loadLevel(index);
	resetPlayerState();
	calcPlayerInitialPosition();
}	

//
// Handles movement of the player
//
void SupaplexLogic::handlePlayerMovement(JoystickButton button)
{
	MapItem murphy;

	// Check if murphy is present on the map
	murphy = m_GameMap[m_PlayerMapOffset];
	if (unlikely(murphy.id != MAP_MURPHY))
		return;

	// Handle player's movement
	m_PlayerPrevMapOffset = m_PlayerMapOffset;
	if (murphy.state == 0)
	{
		// Check for gravity
		bool shouldFallBecauseGravity = false;
		if (m_Gravity && m_GameMap[m_PlayerMapOffset - MAP_WIDTH].id != MAP_PORT_DOWN_TO_UP &&
			m_GameMap[m_PlayerMapOffset - MAP_WIDTH].id != MAP_PORT_VERTICAL &&
			m_GameMap[m_PlayerMapOffset - MAP_WIDTH].id != MAP_PORT_CROSS &&
			m_GameMap[m_PlayerMapOffset + MAP_WIDTH].full == MAP_SPACE)
		{
			shouldFallBecauseGravity = true;
		}

		// Did user press any key?
		if (button == JB_NONE)
		{
			byte_403C3 = true;
			if (shouldFallBecauseGravity)
				goto fall;
			return;
		}

		// Fall player in case of gravity
		if (shouldFallBecauseGravity && m_GameMap[m_PlayerMapOffset + MAP_WIDTH].full == MAP_SPACE)
		{
			switch (button)
			{
			case JB_UP:
				if (m_GameMap[m_PlayerMapOffset - MAP_WIDTH].full == MAP_BASE)
					break;
				goto _lx;

			case JB_LEFT:
				if (m_GameMap[m_PlayerMapOffset - 1].full == MAP_BASE)
					break;
				goto _lx;

			case JB_RIGHT:
				if (m_GameMap[m_PlayerMapOffset + 1].full == MAP_BASE)
					break;
				goto _lx;

			default:
			_lx:
				button = JB_DOWN;
			}	
		}

		// Handle the player's key press
		switch (button)
		{
		case JB_UP:
			byte_403C3 = false;
			if (!movePlayerUp())
				return;
			break;	

		case JB_LEFT:
			byte_403C3 = false;
			if (!movePlayerLeft())
				return;
			break;	

		case JB_DOWN:
		fall:
			byte_403C3 = false;
			if (!movePlayerDown())
				return;
			break;	

		case JB_RIGHT:
			byte_403C3 = false;
			if (!movePlayerRight())
				return;
			break;

		case JB_SPACE_UP:
			byte_403C3 = false;
			if (!eatAbove())
				return;
			break;

		case JB_SPACE_LEFT:
			byte_403C3 = false;
			if (!eatAtLeft())
				return;
			break;

		case JB_SPACE_DOWN:
			byte_403C3 = false;
			if (!eatBelow())
				return;
			break;

		case JB_SPACE_RIGHT:
			byte_403C3 = false;
			if (!eatAtRight())
				return;
			break;

		case JB_SPACE:
			if (m_NumRedDisks == 0)
				return;
			if (m_RedDiskDetonateTimer != 0)
				return;
			if (!byte_403C3)
				return;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_PUT_BOMB;
			m_RedDiskPlacementTimer = 64;
			setPlayerAnimation(ANIM_PLACEREDDISK, 2, 0, 0);
			m_RedDiskDetonateTimer = 1;
			m_RedDiskPosition = m_PlayerMapOffset;
			break;

		default:
			byte_403C3 = false;
			return;
		}
	}

	if (m_RedDiskPlacementTimer != 0)	
	{
		if (--m_RedDiskPlacementTimer == 0 &&
				m_GameMap[m_PlayerMapOffset].state != MURPHY_STATE_PUT_BOMB)
			SND_PlaySound("sounds/zonk_shift.wav", offsetTo3D(m_PlayerMapOffset));

		switch (m_GameMap[m_PlayerMapOffset].state)
		{
		case MURPHY_STATE_SHIFT_ZONK_LEFT:
			if (button != JB_LEFT || m_GameMap[m_PlayerMapOffset - 1].full != MAP_ZONK)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset - 1].full = MAP_ZONK;
				if (m_GameMap[m_PlayerMapOffset - 2].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset - 2].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_SHIFT_ZONK_RIGHT:
			if (button != JB_RIGHT || m_GameMap[m_PlayerMapOffset + 1].full != MAP_ZONK)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset + 1].full = MAP_ZONK;
				if (m_GameMap[m_PlayerMapOffset + 2].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset + 2].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_SHIFT_ORANGE_DISK_LF:
			if (button != JB_LEFT || m_GameMap[m_PlayerMapOffset - 1].full != MAP_DISK_ORANGE)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset - 1].full = MAP_DISK_ORANGE;
				if (m_GameMap[m_PlayerMapOffset - 2].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset - 2].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_SHIFT_ORANGE_DISK_RT:
			if (button != JB_RIGHT || m_GameMap[m_PlayerMapOffset + 1].full != MAP_DISK_ORANGE)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset + 1].full = MAP_DISK_ORANGE;
				if (m_GameMap[m_PlayerMapOffset + 2].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset + 2].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_MOVE_YELLOW_DISK_UP:
			if (button != JB_UP || m_GameMap[m_PlayerMapOffset - MAP_WIDTH].full != MAP_DISK_YELLOW)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset - MAP_WIDTH].full = MAP_DISK_YELLOW;
				if (m_GameMap[m_PlayerMapOffset - 2 * MAP_WIDTH].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset - 2 * MAP_WIDTH].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_MOVE_YELLOW_DISK_LF:
			if (button != JB_LEFT || m_GameMap[m_PlayerMapOffset - 1].full != MAP_DISK_YELLOW)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset - 1].full = MAP_DISK_YELLOW;
				if (m_GameMap[m_PlayerMapOffset - 2].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset - 2].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_MOVE_YELLOW_DISK_DN:
			if (button != JB_DOWN || m_GameMap[m_PlayerMapOffset + MAP_WIDTH].full != MAP_DISK_YELLOW)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset + MAP_WIDTH].full = MAP_DISK_YELLOW;
				if (m_GameMap[m_PlayerMapOffset + 2 * MAP_WIDTH].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset + 2 * MAP_WIDTH].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_MOVE_YELLOW_DISK_RT:
			if (button != JB_RIGHT || m_GameMap[m_PlayerMapOffset + 1].full != MAP_DISK_YELLOW)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset + 1].full = MAP_DISK_YELLOW;
				if (m_GameMap[m_PlayerMapOffset + 2].id != MAP_BOOM)
					m_GameMap[m_PlayerMapOffset + 2].full = MAP_SPACE;
			}
			break;

		case MURPHY_STATE_PUT_BOMB:
			if (button != JB_SPACE)
			{
				m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
				m_RedDiskDetonateTimer = 0;
			}
			else if ((char)m_RedDiskPlacementTimer <= 32)
				m_RedDiskDetonateTimer = 1;
			break;
		}

		return;
	}

	m_PlayerPositionPixelsX += m_PlayerAnim_StepX;
	m_PlayerPositionPixelsY += m_PlayerAnim_StepY;

	if (!m_PlayerAnim_NumFrames || --m_PlayerAnim_NumFrames != 0)
		return;

	m_PlayerAnim_StepX = 0;
	m_PlayerAnim_StepY = 0;

	byte state = m_GameMap[m_PlayerMapOffset].state;
	m_GameMap[m_PlayerMapOffset].state = 0;

	switch (state)
	{
	case MURPHY_STATE_MOVE_UP_EAT_INFTR:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_MOVE_UP_EAT_SPACE:
	case MURPHY_STATE_MOVE_UP_EAT_BASE:
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		sub_38191(m_PlayerMapOffset + MAP_WIDTH);
		break;

	case MURPHY_STATE_MOVE_LEFT_EAT_INFTR:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_MOVE_LEFT_EAT_SPACE:
	case MURPHY_STATE_MOVE_LEFT_EAT_BASE:
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		sub_38191(m_PlayerMapOffset + 1);
		break;

	case MURPHY_STATE_MOVE_DOWN_EAT_INFTR:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_MOVE_DOWN_EAT_SPACE:
	case MURPHY_STATE_MOVE_DOWN_EAT_BASE:
		if (m_GameMap[m_PlayerMapOffset - MAP_WIDTH].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset - MAP_WIDTH].full = MAP_SPACE;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		break;

	case MURPHY_STATE_MOVE_RIGHT_EAT_INFTR:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_MOVE_RIGHT_EAT_SPACE:
	case MURPHY_STATE_MOVE_RIGHT_EAT_BASE:
		sub_38191(m_PlayerMapOffset - 1);
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		break;

	case MURPHY_STATE_SHIFT_ZONK_LEFT:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_GameMap[m_PlayerMapOffset - 1].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset - 2].full = MAP_ZONK;
		sub_3DB7E(m_PlayerMapOffset - 2);
		m_PlayerMapOffset--;
		break;	

	case MURPHY_STATE_SHIFT_ZONK_RIGHT:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_GameMap[m_PlayerMapOffset + 1].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset + 2].full = MAP_ZONK;
		sub_3DB7E(m_PlayerMapOffset + 2);
		m_PlayerMapOffset++;
		break;

	case MURPHY_STATE_EAT_INFOTRON_TOP:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_EAT_BASE_TOP:
		if (m_GameMap[m_PlayerMapOffset - MAP_WIDTH].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset - MAP_WIDTH].full = MAP_SPACE;
		break;

	case MURPHY_STATE_EAT_INFOTRON_LEFT:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_EAT_BASE_LEFT:
		if (m_GameMap[m_PlayerMapOffset - 1].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset - 1].full = MAP_SPACE;
		break;

	case MURPHY_STATE_EAT_INFOTRON_RIGHT:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_EAT_BASE_RIGHT:
		if (m_GameMap[m_PlayerMapOffset + 1].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset + 1].full = MAP_SPACE;
	break;

	case MURPHY_STATE_EAT_INFOTRON_BOTTOM:
		if (m_InfotronsLeftToCollect > 0)
			--m_InfotronsLeftToCollect;
	case MURPHY_STATE_EAT_BASE_BOTTOM:
		if (m_GameMap[m_PlayerMapOffset + MAP_WIDTH].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset + MAP_WIDTH].full = MAP_SPACE;
		break;

	case MURPHY_STATE_EXIT:
		m_PlayerCompletedLevel = true;
		break;

	case MURPHY_STATE_TELEPORT_UP:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset -= 2 * MAP_WIDTH;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_PlayerIsInsideTeleport = 0;
		if (m_GameMap[m_PlayerMapOffset + MAP_WIDTH].state == 1)	// Is this a gravity switch port?
			handleGravityPort(m_PlayerMapOffset + MAP_WIDTH);
		break;

	case MURPHY_STATE_TELEPORT_LEFT:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset -= 2;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_PlayerIsInsideTeleport = 0;
		if (m_GameMap[m_PlayerMapOffset + 1].state == 1)			// Is this a gravity switch port?
			handleGravityPort(m_PlayerMapOffset + 1);
		break;

	case MURPHY_STATE_TELEPORT_DOWN:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset += 2 * MAP_WIDTH;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_PlayerIsInsideTeleport = 0;
		if (m_GameMap[m_PlayerMapOffset - MAP_WIDTH].state == 1)	// Is this a gravity switch port?
			handleGravityPort(m_PlayerMapOffset - MAP_WIDTH);
		break;

	case MURPHY_STATE_TELEPORT_RIGHT:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset += 2;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_PlayerIsInsideTeleport = 0;
		if (m_GameMap[m_PlayerMapOffset - 1].state == 1)			// Is this a gravity switch port?
			handleGravityPort(m_PlayerMapOffset - 1);
		break;

	case MURPHY_STATE_MOVE_UP_EAT_BOMB:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset -= MAP_WIDTH;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_MOVE_LEFT_EAT_BOMB:
		if (m_GameMap[m_PlayerMapOffset + 1].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset + 1].full = MAP_SPACE;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_MOVE_DOWN_EAT_BOMB:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset += MAP_WIDTH;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_MOVE_RIGHT_EAT_BOMB:
		if (m_GameMap[m_PlayerMapOffset - 1].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset - 1].full = MAP_SPACE;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_EAT_BOMB_TOP:
		if (m_GameMap[m_PlayerMapOffset - MAP_WIDTH].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset - MAP_WIDTH].full = MAP_SPACE;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_EAT_BOMB_LEFT:
		if (m_GameMap[m_PlayerMapOffset - 1].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset - 1].full = MAP_SPACE;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_EAT_BOMB_BOTTOM:
		if (m_GameMap[m_PlayerMapOffset + MAP_WIDTH].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset + MAP_WIDTH].full = MAP_SPACE;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_EAT_BOMB_RIGHT:
		if (m_GameMap[m_PlayerMapOffset + 1].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset + 1].full = MAP_SPACE;
		m_NumRedDisks++;
		break;

	case MURPHY_STATE_MOVE_YELLOW_DISK_UP:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset -= MAP_WIDTH;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset - MAP_WIDTH].full = MAP_DISK_YELLOW;
		break;

	case MURPHY_STATE_MOVE_YELLOW_DISK_LF:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset--;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset - 1].full = MAP_DISK_YELLOW;
		break;

	case MURPHY_STATE_MOVE_YELLOW_DISK_DN:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset += MAP_WIDTH;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset + MAP_WIDTH].full = MAP_DISK_YELLOW;
		break;

	case MURPHY_STATE_MOVE_YELLOW_DISK_RT:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset++;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset + 1].full = MAP_DISK_YELLOW;
		break;

	case MURPHY_STATE_SHIFT_ORANGE_DISK_LF:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset--;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset - 1].full = MAP_DISK_ORANGE;
		break;

	case MURPHY_STATE_SHIFT_ORANGE_DISK_RT:
		if (m_GameMap[m_PlayerMapOffset].id != MAP_BOOM)
			m_GameMap[m_PlayerMapOffset].full = MAP_SPACE;
		m_PlayerMapOffset++;
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_GameMap[m_PlayerMapOffset + 1].full = MAP_DISK_ORANGE;
		if (m_GameMap[m_PlayerMapOffset + MAP_WIDTH + 1].full == MAP_SPACE)
		{
			m_GameMap[m_PlayerMapOffset + 1].state = 0x20;
			m_GameMap[m_PlayerMapOffset + MAP_WIDTH + 1].state = MAP_DISK_ORANGE;
		}
		break;

	case MURPHY_STATE_PUT_BOMB:
		m_GameMap[m_PlayerMapOffset].full = MAP_MURPHY;
		m_RedDiskDetonateTimer = 2;
		--m_NumRedDisks;
		SND_PlaySound("sounds/zonk_shift.wav", offsetTo3D(m_PlayerMapOffset));
		break;

	default:
		m_PlayerCompletedLevel = true;
	}	
}

//
// Runs AI and physics
//
void SupaplexLogic::runAI()
{
	updateZonksAndEnemies();
	updateRedDisk();
	createChainedExplosions();

	m_CurrentFrameNumber++;
	if (m_PlayerCompletedLevel)
		m_LevelComplete = true;
	else if (m_TimeoutToLeaveTheLevel > 1)
		m_TimeoutToLeaveTheLevel--;
	else if (m_TimeoutToLeaveTheLevel == 1)
	{
		m_TimeoutToLeaveTheLevel = 0;
		m_LevelComplete = true;
	}
}

//
// Returns position of the exit relative to the position of the player
//
Vector2 SupaplexLogic::getExitRelativePos() const
{
	if (m_GameMap[m_PlayerMapOffset - MAP_WIDTH].id == MAP_EXIT)
		return Vector2(0.0f, -1.0f);
	else if (m_GameMap[m_PlayerMapOffset + MAP_WIDTH].id == MAP_EXIT)
		return Vector2(0.0f, 1.0f);
	else if (m_GameMap[m_PlayerMapOffset - 1].id == MAP_EXIT)
		return Vector2(-1.0f, 0.0f);
	else if (m_GameMap[m_PlayerMapOffset + 1].id == MAP_EXIT)
		return Vector2(1.0f, 0.0f);

	return Vector2(0.0f, 0.0f);
}

//
// Loads the specified level
//
void SupaplexLogic::loadLevel(int index)
{
	std::string contents = FS_GetFileContents("levels.dat");

	// Get level data
	memcpy(&m_LevelData, contents.data() + index * sizeof(m_LevelData), sizeof(m_LevelData));

	// Fill the game map
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
		m_GameMap[i].full = m_LevelData.map[i];

	// Clear an array of active explosions
	memset(m_Explosions, 0, MAP_WIDTH * MAP_HEIGHT);

	m_Gravity = (m_LevelData.gravity == 1 ? true : false);
	m_FreezeZonks = (m_LevelData.freezeZonks == 2 ? true : false);
	m_FreezeEnemies = false;

	setupLevelMap();
}

//
// Resets current state of the player
//
void SupaplexLogic::resetPlayerState()
{
	m_PlayerIsLookingLeft = false;
	m_PlayerDied = false;
	m_PlayerExploded = false;
	m_PlayerCompletedLevel = false;
	m_TimeoutToLeaveTheLevel = 0;
	m_NumRedDisks = 0;
	m_TerminalsActivated = false;
	m_CurrentFrameNumber = 0;
	m_PlayerAnim_ID = ANIM_NONE;
	m_PlayerAnim_StepX = 0;
	m_PlayerAnim_StepY = 0;
	m_PlayerAnim_NumFrames = 0;
	m_RedDiskPosition = 0;
	m_RedDiskPlacementTimer = 0;
	m_RedDiskDetonateTimer = 0;
	byte_403C3 = false;
}

//
// Calculates initial position of the player
//
void SupaplexLogic::calcPlayerInitialPosition()
{
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
	{
		if (m_GameMap[i].full == MAP_MURPHY)
		{
			m_PlayerMapOffset = i;
			m_PlayerPositionPixelsX = (i % MAP_WIDTH) * 16;
			m_PlayerPositionPixelsY = (i / MAP_WIDTH) * 16;
			break;
		}
	}
}

//
// Setup the level map
//
void SupaplexLogic::setupLevelMap()
{
	int numInfotrons = 0;

	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
	{
		switch (m_GameMap[i].full)
		{
		case MAP_INFOTRON:
			numInfotrons++;
			break;

		case MAP_SNIK_SNAK:
		case MAP_ELECTRON:
			if (i > 0 && m_GameMap[i - 1].full == MAP_SPACE)
				m_GameMap[i].state = 0x01;
			else if (i >= MAP_WIDTH && m_GameMap[i - MAP_WIDTH].full == MAP_SPACE)
			{
				m_GameMap[i - MAP_WIDTH].state = 0x10;
				m_GameMap[i - MAP_WIDTH].id = m_GameMap[i].id;
				m_GameMap[i].full = 0xFFFF;
			}	
			else if (i < MAP_WIDTH * MAP_HEIGHT - 1 && m_GameMap[i + 1].full == MAP_SPACE)
			{
				m_GameMap[i + 1].state = 0x28;
				m_GameMap[i + 1].id = m_GameMap[i].id;
				m_GameMap[i].full = 0xFFFF;
			}
			break;

		case MAP_RAM_CHIP_LEFT:
		case MAP_RAM_CHIP_RIGHT:
		case MAP_RAM_CHIP_TOP:
		case MAP_RAM_CHIP_BOTTOM:
			m_GameMap[i].full = MAP_RAM_CHIP;
			break;

		case MAP_HARDWARE_1:
		case MAP_HARDWARE_2:
		case MAP_HARDWARE_3:
		case MAP_HARDWARE_4:
		case MAP_HARDWARE_5:
		case MAP_HARDWARE_6:
		case MAP_HARDWARE_7:
		case MAP_HARDWARE_8:
		case MAP_HARDWARE_9:
		case MAP_HARDWARE_10:
			m_GameMap[i].full = MAP_HARDWARE;
			break;

		case MAP_PORT_SPECIAL_LEFT_TO_RIGHT:
		case MAP_PORT_SPECIAL_UP_TO_DOWN:
		case MAP_PORT_SPECIAL_RIGHT_TO_LEFT:
		case MAP_PORT_SPECIAL_DOWN_TO_UP:
			m_GameMap[i].state = 0x01;
			m_GameMap[i].id -= 4;
			break;
		}	
	}

	if (m_LevelData.infotronsNeeded != 0)
		numInfotrons = m_LevelData.infotronsNeeded;

	m_InfotronsLeftToCollect = numInfotrons & 0xFF;
	m_TotalInfotronsInTheLevel = numInfotrons & 0xFF;
}

//
// Called when player enters the gravity switch port
//
void SupaplexLogic::handleGravityPort(uint32 offset)
{
	word check = (((offset & 0xFF) << 8) | ((offset >> 8) & 0xFF)) << 1;

	for (int i = 0; i < m_LevelData.numberOfGravitySwitchPorts; i++)
	{
		if (check == m_LevelData.ports[i].mapOffset)
		{
			m_Gravity = m_LevelData.ports[i].gravity == 1 ? true : false;
			m_FreezeZonks = m_LevelData.ports[i].freezeZonks == 2 ? true : false;
			m_FreezeEnemies = m_LevelData.ports[i].freezeEnemies == 1 ? true : false;

			break;
		}
	}
}

//
// Moves player one cell up
//
bool SupaplexLogic::movePlayerUp()
{
	uint32 offset = m_PlayerMapOffset - MAP_WIDTH;
	MapItem item;

	do
	{
		item = m_GameMap[offset];
		switch (item.id)
		{
		case MAP_SPACE:
			if (item.state == 0)
			{
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_MOVE_UP_L : ANIM_MOVE_UP_R, 8, 0, -2);
				m_RedDiskPlacementTimer = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_UP_EAT_SPACE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_BUG:
			if ((char)item.state > 0)
			{
				createExplosion(m_PlayerMapOffset);
				return true;
			}
			m_GameMap[offset].full = MAP_BASE;
			/* pass-through to MAP_BASE */

		case MAP_BASE:
			if (item.state == 0 || item.id == MAP_BUG)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_EATBASE_UP_L : ANIM_EATBASE_UP_R, 8, 0, -2);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_UP_EAT_BASE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_INFOTRON:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_EATINFOTRON_UP_L : ANIM_EATINFOTRON_UP_R, 8, 0, -2);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_UP_EAT_INFTR;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_EXIT:
			return handleLevelExit(m_PlayerMapOffset);

		case MAP_TERMINAL:
			handleTerminal();
			return true;

		case MAP_PORT_DOWN_TO_UP:
		case MAP_PORT_VERTICAL:
		case MAP_PORT_CROSS:
			if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
			{
				setPlayerAnimation(ANIM_TELEPORT_UP, 8, 0, -4);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = item.id;
				m_GameMap[offset - MAP_WIDTH].state = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_TELEPORT_UP;
			}
			return true;

		case MAP_DISK_RED:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_EATDISK_UP_L : ANIM_EATDISK_UP_R, 8, 0, -2);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_MOVE_UP_EAT_BOMB;
				m_GameMap[offset].state = MAP_MURPHY;

				return true;
			}
			break;

		case MAP_DISK_YELLOW:
			if (item.state == 0)
			{
				if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTYELLOWDISK_UP, 8, 0, -2);
					m_RedDiskPlacementTimer = 8;
					m_PlayerIsInsideTeleport = 0;
					m_GameMap[offset - MAP_WIDTH].state = MAP_DISK_YELLOW;
					m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_MOVE_YELLOW_DISK_UP;
				}	

				return true;
			}
			break;
		}
	}
	while (!checkPlayerDied(offset, item, JB_UP));

	return false;
}

//
// Moves player one cell down
//
bool SupaplexLogic::movePlayerDown()
{
	uint32 offset = m_PlayerMapOffset + MAP_WIDTH;
	MapItem item;

	do
	{
		item = m_GameMap[offset];
		switch (item.id)
		{
		case MAP_SPACE:
			if (item.state == 0)
			{
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_MOVE_DOWN_L : ANIM_MOVE_DOWN_R, 8, 0, 2);
				m_RedDiskPlacementTimer = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_DOWN_EAT_SPACE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_BUG:
			if ((char)item.state > 0)
			{
				createExplosion(m_PlayerMapOffset);
				return true;
			}
			m_GameMap[offset].full = MAP_BASE;
			/* pass-through to MAP_BASE */

		case MAP_BASE:
			if (item.state == 0 || item.id == MAP_BUG)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_EATBASE_DOWN_L : ANIM_EATBASE_DOWN_R, 8, 0, 2);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_DOWN_EAT_BASE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_INFOTRON:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_EATINFOTRON_DOWN_L : ANIM_EATINFOTRON_DOWN_R, 8, 0, 2);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_DOWN_EAT_INFTR;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_EXIT:
			return handleLevelExit(m_PlayerMapOffset);

		case MAP_TERMINAL:
			handleTerminal();
			return true;

		case MAP_PORT_UP_TO_DOWN:
		case MAP_PORT_VERTICAL:
		case MAP_PORT_CROSS:
			if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
			{
				setPlayerAnimation(ANIM_TELEPORT_DOWN, 8, 0, 4);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = item.id;
				m_GameMap[offset + MAP_WIDTH].state = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_TELEPORT_DOWN;
			}
			return true;

		case MAP_DISK_RED:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(m_PlayerIsLookingLeft ? ANIM_EATDISK_DOWN_L : ANIM_EATDISK_DOWN_R, 8, 0, 2);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_MOVE_DOWN_EAT_BOMB;
				m_GameMap[offset].state = MAP_MURPHY;

				return true;
			}
			break;

		case MAP_DISK_YELLOW:
			if (item.state == 0)
			{
				if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTYELLOWDISK_DOWN, 8, 0, 2);
					m_RedDiskPlacementTimer = 8;
					m_PlayerIsInsideTeleport = 0;
					m_GameMap[offset - MAP_WIDTH].state = MAP_DISK_YELLOW;
					m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_MOVE_YELLOW_DISK_DN;
				}	
				return true;
			}
			break;
		}
	}
	while (!checkPlayerDied(offset, item, JB_DOWN));

	return false;
}

//
// Moves player one cell left
//
bool SupaplexLogic::movePlayerLeft()
{
	uint32 offset = m_PlayerMapOffset - 1;
	MapItem item;

	m_PlayerIsLookingLeft = true;

	do
	{
		item = m_GameMap[offset];
		switch (item.id)
		{
		case MAP_SPACE:
			if (item.state == 0)
			{
				setPlayerAnimation(ANIM_MOVE_LEFT, 8, -2, 0);
				m_RedDiskPlacementTimer = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_LEFT_EAT_SPACE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_BUG:
			if ((char)item.state > 0)
			{
				createExplosion(m_PlayerMapOffset);
				return true;
			}
			m_GameMap[offset].full = MAP_BASE;
			/* pass-through to MAP_BASE */

		case MAP_BASE:
			if (item.state == 0 || item.id == MAP_BUG)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(ANIM_EATBASE_LEFT, 8, -2, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_LEFT_EAT_BASE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_INFOTRON:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(ANIM_EATINFOTRON_LEFT, 8, -2, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_LEFT_EAT_INFTR;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_ZONK:
			if (item.state == 0)
			{
			 	if (m_GameMap[offset - 1].full == MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTZONK_LEFT, 8, -2, 0);
					m_GameMap[offset - 1].state = MAP_ZONK;
					m_PlayerIsInsideTeleport = 0;
					m_RedDiskPlacementTimer = 8;
					m_GameMap[offset + 1].state = MURPHY_STATE_SHIFT_ZONK_LEFT;
				}	

				return true;
			}
			break;

		case MAP_EXIT:
			return handleLevelExit(m_PlayerMapOffset);

		case MAP_TERMINAL:
			handleTerminal();
			return true;

		case MAP_PORT_RIGHT_TO_LEFT:
		case MAP_PORT_HORIZONTAL:
		case MAP_PORT_CROSS:
			if (m_GameMap[offset - 1].full == MAP_SPACE)
			{
				setPlayerAnimation(ANIM_TELEPORT_LEFT, 8, -4, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = item.id;
				m_GameMap[offset - 1].state = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_TELEPORT_LEFT;
			}
			return true;

		case MAP_DISK_RED:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(ANIM_EATDISK_LEFT, 8, -2, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_LEFT_EAT_BOMB;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_PlayerMapOffset--;

				return true;
			}
			break;

		case MAP_DISK_YELLOW:
			if (item.state == 0)
			{
				if (m_GameMap[offset - 1].full == MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTYELLOWDISK_LEFT, 8, -2, 0);
					m_RedDiskPlacementTimer = 8;
					m_PlayerIsInsideTeleport = 0;
					m_GameMap[offset - 1].state = MAP_DISK_YELLOW;
					m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_MOVE_YELLOW_DISK_LF;
				}	
				return true;
			}
			break;

		case MAP_DISK_ORANGE:
			if (item.state == 0)
			{
				if (m_GameMap[offset - 1].full == MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTORANGEDISK_LEFT, 8, -2, 0);
					m_RedDiskPlacementTimer = 8;
					m_PlayerIsInsideTeleport = 0;
					m_GameMap[offset - 1].state = MAP_DISK_ORANGE;
					m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_SHIFT_ORANGE_DISK_LF;
				}	
				return true;
			}
			break;
		}
	}
	while (!checkPlayerDied(offset, item, JB_LEFT));

	return false;
}

//
// Moves player one cell right
//
bool SupaplexLogic::movePlayerRight()
{
	uint32 offset = m_PlayerMapOffset + 1;
	MapItem item;

	m_PlayerIsLookingLeft = false;

	do
	{
		item = m_GameMap[offset];
		switch (item.id)
		{
		case MAP_SPACE:
			if (item.state == 0)
			{
				setPlayerAnimation(ANIM_MOVE_RIGHT, 8, 2, 0);
				m_RedDiskPlacementTimer = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_RIGHT_EAT_SPACE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_BUG:
			if ((char)item.state > 0)
			{
				createExplosion(m_PlayerMapOffset);
				return true;
			}
			m_GameMap[offset].full = MAP_BASE;
			/* pass-through to MAP_BASE */

		case MAP_BASE:
			if (item.state == 0 || item.id == MAP_BUG)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(ANIM_EATBASE_RIGHT, 8, 2, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_RIGHT_EAT_BASE;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_INFOTRON:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(ANIM_EATINFOTRON_RIGHT, 8, 2, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_RIGHT_EAT_INFTR;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_PlayerMapOffset = offset;

				return true;
			}
			break;

		case MAP_ZONK:
			if (item.state == 0)
			{
			 	if (m_GameMap[offset + 1].full == MAP_SPACE &&
			 			m_GameMap[offset + MAP_WIDTH].full != MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTZONK_RIGHT, 8, 2, 0);
					m_GameMap[offset + 1].state = MAP_ZONK;
					m_PlayerIsInsideTeleport = 0;
					m_RedDiskPlacementTimer = 8;
					m_GameMap[offset - 1].state = MURPHY_STATE_SHIFT_ZONK_RIGHT;
				}	

				return true;
			}
			break;

		case MAP_EXIT:
			return handleLevelExit(m_PlayerMapOffset);

		case MAP_TERMINAL:
			handleTerminal();
			return true;

		case MAP_PORT_LEFT_TO_RIGHT:
		case MAP_PORT_HORIZONTAL:
		case MAP_PORT_CROSS:
			if (m_GameMap[offset + 1].full == MAP_SPACE)
			{
				setPlayerAnimation(ANIM_TELEPORT_RIGHT, 8, 4, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = item.id;
				m_GameMap[offset + 1].state = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_TELEPORT_RIGHT;
			}
			return true;

		case MAP_DISK_RED:
			if (item.state == 0)
			{
				SND_PlaySound("sounds/eat_base.wav", offsetTo3D(m_PlayerMapOffset));
				setPlayerAnimation(ANIM_EATDISK_RIGHT, 8, 2, 0);
				m_RedDiskPlacementTimer = 0;
				m_PlayerIsInsideTeleport = 0;
				m_GameMap[offset].state = MURPHY_STATE_MOVE_RIGHT_EAT_BOMB;
				m_GameMap[offset].id = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].state = MAP_MURPHY;
				m_GameMap[m_PlayerMapOffset].id = MAP_SPACE;
				m_PlayerMapOffset++;

				return true;
			}
			break;

		case MAP_DISK_YELLOW:
			if (item.state == 0)
			{
				if (m_GameMap[offset + 1].full == MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTYELLOWDISK_RIGHT, 8, 2, 0);
					m_RedDiskPlacementTimer = 8;
					m_PlayerIsInsideTeleport = 0;
					m_GameMap[offset - 1].state = MAP_DISK_YELLOW;
					m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_MOVE_YELLOW_DISK_RT;
				}	
				return true;
			}
			break;

		case MAP_DISK_ORANGE:
			if (item.state == 0)
			{
				if (m_GameMap[offset + 1].full == MAP_SPACE &&
					m_GameMap[offset + MAP_WIDTH].full != MAP_SPACE)
				{
					setPlayerAnimation(ANIM_SHIFTORANGEDISK_RIGHT, 8, 2, 0);
					m_RedDiskPlacementTimer = 8;
					m_PlayerIsInsideTeleport = 0;
					m_GameMap[offset - 1].state = MAP_DISK_ORANGE;
					m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_SHIFT_ORANGE_DISK_RT;
				}	
				return true;
			}
			break;
		}
	}
	while (!checkPlayerDied(offset, item, JB_RIGHT));

	return false;
}

//
// Eats item at left of the player
//
bool SupaplexLogic::eatAtLeft()
{
	uint32 offset = m_PlayerMapOffset - 1;
	MapItem item = m_GameMap[offset];

	m_PlayerIsLookingLeft = true;

	switch (item.id)
	{
	case MAP_BUG:
		if ((char)item.state > 0)
		{
			createExplosion(m_PlayerMapOffset);
			return true;
		}
		m_GameMap[offset].full = MAP_BASE;
		/* pass-through to MAP_BASE */

	case MAP_BASE:
		if (item.state == 0 || item.id == MAP_BUG)
		{
			SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATBASE_ATLEFT, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BASE_LEFT;
			return true;
		}
		break;

	case MAP_INFOTRON:
		if (item.state == 0)
		{
			SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATINFOTRON_ATLEFT, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_INFOTRON_LEFT;
			m_GameMap[offset].state = 0xFF; 
			return true;
		}
		break;

	case MAP_TERMINAL:
		handleTerminal();
		return true;

	case MAP_DISK_RED:
		SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
		setPlayerAnimation(ANIM_EATDISK_ATLEFT, 8, 0, 0);
		m_RedDiskPlacementTimer = 0;
		m_PlayerIsInsideTeleport = 0;
		m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BOMB_LEFT;
		m_GameMap[offset].state = MAP_MURPHY;
		return true;
	}

	return false;
}

//
// Eats item at right of the player
//
bool SupaplexLogic::eatAtRight()
{
	uint32 offset = m_PlayerMapOffset + 1;
	MapItem item = m_GameMap[offset];

	m_PlayerIsLookingLeft = false;

	switch (item.id)
	{
	case MAP_BUG:
		if ((char)item.state > 0)
		{
			createExplosion(m_PlayerMapOffset);
			return true;
		}
		m_GameMap[offset].full = MAP_BASE;
		/* pass-through to MAP_BASE */

	case MAP_BASE:
		if (item.state == 0 || item.id == MAP_BUG)
		{
			SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATBASE_ATRIGHT, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BASE_RIGHT;
			return true;
		}
		break;

	case MAP_INFOTRON:
		if (item.state == 0)
		{
			SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATINFOTRON_ATRIGHT, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_INFOTRON_RIGHT;
			m_GameMap[offset].state = 0xFF; 
			return true;
		}
		break;

	case MAP_TERMINAL:
		handleTerminal();
		return true;

	case MAP_DISK_RED:
		SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
		setPlayerAnimation(ANIM_EATDISK_ATRIGHT, 8, 0, 0);
		m_RedDiskPlacementTimer = 0;
		m_PlayerIsInsideTeleport = 0;
		m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BOMB_RIGHT;
		m_GameMap[offset].state = MAP_MURPHY;
		return true;
	}

	return false;
}

//
// Eats item above the player
//
bool SupaplexLogic::eatAbove()
{
	uint32 offset = m_PlayerMapOffset - MAP_WIDTH;
	MapItem item = m_GameMap[offset];

	switch (item.id)
	{
	case MAP_BUG:
		if ((char)item.state > 0)
		{
			createExplosion(m_PlayerMapOffset);
			return true;
		}
		m_GameMap[offset].full = MAP_BASE;
		/* pass-through to MAP_BASE */

	case MAP_BASE:
		if (item.state == 0 || item.id == MAP_BUG)
		{
			SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATBASE_ATTOP, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BASE_TOP;
			return true;
		}
		break;

	case MAP_INFOTRON:
		if (item.state == 0)
		{
			SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATINFOTRON_ATTOP, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_INFOTRON_TOP;
			m_GameMap[offset].state = 0xFF; 
			return true;
		}
		break;

	case MAP_TERMINAL:
		handleTerminal();
		return true;

	case MAP_DISK_RED:
		SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
		setPlayerAnimation(ANIM_EATDISK_ATTOP, 8, 0, 0);
		m_RedDiskPlacementTimer = 0;
		m_PlayerIsInsideTeleport = 0;
		m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BOMB_TOP;
		m_GameMap[offset].state = MAP_MURPHY;
		return true;
	}

	return false;
}

//
// Eats item below the player
//
bool SupaplexLogic::eatBelow()
{
	uint32 offset = m_PlayerMapOffset + MAP_WIDTH;
	MapItem item = m_GameMap[offset];

	switch (item.id)
	{
	case MAP_BUG:
		if ((char)item.state > 0)
		{
			createExplosion(m_PlayerMapOffset);
			return true;
		}
		m_GameMap[offset].full = MAP_BASE;
		/* pass-through to MAP_BASE */

	case MAP_BASE:
		if (item.state == 0 || item.id == MAP_BUG)
		{
			SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATBASE_ATBOTTOM, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BASE_BOTTOM;
			return true;
		}
		break;

	case MAP_INFOTRON:
		if (item.state == 0)
		{
			SND_PlaySound("sounds/eat_infotron.wav", offsetTo3D(offset));
			setPlayerAnimation(ANIM_EATINFOTRON_ATBOTTOM, 8, 0, 0);
			m_PlayerIsInsideTeleport = 0;
			m_RedDiskPlacementTimer = 0;
			m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_INFOTRON_BOTTOM;
			m_GameMap[offset].state = 0xFF; 
			return true;
		}
		break;

	case MAP_TERMINAL:
		handleTerminal();
		return true;

	case MAP_DISK_RED:
		SND_PlaySound("sounds/eat_base.wav", offsetTo3D(offset));
		setPlayerAnimation(ANIM_EATDISK_ATBOTTOM, 8, 0, 0);
		m_RedDiskPlacementTimer = 0;
		m_PlayerIsInsideTeleport = 0;
		m_GameMap[m_PlayerMapOffset].state = MURPHY_STATE_EAT_BOMB_BOTTOM;
		m_GameMap[offset].state = MAP_MURPHY;
		return true;
	}

	return false;
}

//
// Called when user activates the exit
//
bool SupaplexLogic::handleLevelExit(uint32 offset)
{
	if (m_InfotronsLeftToCollect != 0)
		return false;

	SND_PlaySound("sounds/level_done.wav", offsetTo3D(m_PlayerMapOffset));

	m_TimeoutToLeaveTheLevel = 64;
	setPlayerAnimation(ANIM_EXIT, 41, 0, 0);
	m_PlayerIsInsideTeleport = 0;
	m_RedDiskPlacementTimer = 0;

	m_GameMap[offset].state = MURPHY_STATE_EXIT;

	return true;
}	

//
// Called when user activates the terminal
//
void SupaplexLogic::handleTerminal()
{
	if (m_TerminalsActivated)
		return;

	m_TerminalsActivated = true;
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
	{
		if (m_GameMap[i].full == MAP_DISK_YELLOW)
			createExplosion(i);
	}
}

//
// Checks whether player have died due to it's movement
//
bool SupaplexLogic::checkPlayerDied(uint32 offset, MapItem item, JoystickButton button)
{
	if (item.full == 0xFFFF || item.full == 0xAAAA || item.state == 0)
		return true;

	switch (item.id)
	{
	case MAP_ZONK:
		if (button == JB_LEFT)
		{
			byte mode = item.state;

			mode &= 0xF0;
			if (mode != 0x20 && mode != 0x40 && mode != 0x50 && mode != 0x70)
				createExplosion(offset);

			return true;
		}
		else if (button == JB_RIGHT)
		{
			byte mode = item.state;

			mode &= 0xF0;
			if (mode != 0x30 && mode != 0x40 && mode != 0x60 && mode != 0x70)
				createExplosion(offset);

			return true;
		}

		createExplosion(offset);
		return true;

	case MAP_BOOM:
		if ((item.state & 0x80) != 0 || (char)item.state < 4)
		{
			createExplosion(offset);
			return true;
		}	

		m_GameMap[offset].full = MAP_SPACE;
		return false;

	case MAP_DISK_ORANGE:
	case MAP_PORT_LEFT_TO_RIGHT:
	case MAP_PORT_UP_TO_DOWN:
	case MAP_PORT_RIGHT_TO_LEFT:
	case MAP_PORT_DOWN_TO_UP:
	    return true;
	}	

	createExplosion(offset);

	return true;
}

//
// Sets current animation for the player
//
void SupaplexLogic::setPlayerAnimation(PlayerAnimation animID, uint numFrames, int stepX, int stepY)
{
	m_PlayerAnim_ID = animID;
	m_PlayerAnim_StepX = stepX;
	m_PlayerAnim_StepY = stepY;
	m_PlayerAnim_NumFrames = numFrames;
}

//
// Creates explosion at the specified position
//
void SupaplexLogic::createExplosion(uint32 offset)
{
	if (m_GameMap[offset].id == MAP_HARDWARE)
		return;

	if (m_GameMap[offset].id == MAP_MURPHY)
		m_PlayerDied = true;

	char dl;
	word cx;
	if (m_GameMap[offset].id != MAP_ELECTRON)
	{
		dl = 13;
		cx = MAP_BOOM;
	}
	else
	{
		dl = -13;
		cx = 0x8000 | MAP_BOOM;
	}

	uint32 offsets[] =
	{
		offset - MAP_WIDTH - 1,
		offset - MAP_WIDTH,
		offset - MAP_WIDTH + 1,
		offset - 1,
		offset + 1,
		offset + MAP_WIDTH - 1,
		offset + MAP_WIDTH,
		offset + MAP_WIDTH + 1
	};

	m_GameMap[offset].full = cx;
	for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++)
	{
		MapItem item = m_GameMap[offsets[i]];
		char dh = dl;
	
		switch (item.id)
		{
		case MAP_HARDWARE:
			break;

		case MAP_ZONK:
			sub_398F9(offsets[i], item, cx);
			break;

		case MAP_INFOTRON:
			sub_39969(offsets[i], item, cx);
			break;

		case MAP_MURPHY:
			m_PlayerDied = true;
		case MAP_ELECTRON:
		case MAP_DISK_ORANGE:
		case MAP_DISK_YELLOW:
		case MAP_SNIK_SNAK:
			if (item.id == MAP_ELECTRON)
			{
				dh = -dl;
				cx = 0x8000 | MAP_BOOM;
			}
			m_Explosions[offsets[i]] = dh;

		default:
			m_GameMap[offsets[i]].full = cx;
			break;
		}
	}

	SND_PlaySound("sounds/boom.wav", offsetTo3D(offset));
}

//
// Activates chained explosions
//
void SupaplexLogic::createChainedExplosions()
{
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
	{
		if ((char)m_Explosions[i] < 0)
		{
			if (++m_Explosions[i] == 0)
			{
				m_GameMap[i].state = 0xFF;
				m_GameMap[i].id = MAP_ELECTRON;
				createExplosion(i);
			}
		}
		else if ((char)m_Explosions[i] > 0)
		{
			if (--m_Explosions[i] == 0)
				createExplosion(i);
		}
	}
}	

void SupaplexLogic::sub_398F9(uint32 offset, MapItem item, word cx)
{
	m_GameMap[offset].full = cx;

	switch (item.state & 0xF0)
	{
	case 0x10:
	case 0x70:
		cancelAnimation(offset - MAP_WIDTH);
		offset += MAP_WIDTH;
		if (m_GameMap[offset].full == 0x9999)
			cancelAnimation(offset);
		break;

	case 0x20:
		cancelAnimation(offset + 1);
		cancelAnimation(offset + MAP_WIDTH);
		break;

	case 0x30:
		cancelAnimation(offset - 1);
		cancelAnimation(offset + MAP_WIDTH);
		break;

	case 0x50:
		cancelAnimation(offset - 1);
		break;

	case 0x60:
		cancelAnimation(offset + 1);
		break;

	//case 0x70:
	//	cancelAnimation(offset + MAP_WIDTH);
	//	break;
	}
}

void SupaplexLogic::sub_39969(uint32 offset, MapItem item, word cx)
{
	m_GameMap[offset].full = cx;

	switch (item.state & 0xF0)
	{
	case 0x10:
	case 0x70:
		cancelAnimation(offset - MAP_WIDTH);
		offset += MAP_WIDTH;
		if (m_GameMap[offset].full == 0x9999)
			cancelAnimation(offset);
		break;

	case 0x20:
		cancelAnimation(offset + 1);
		cancelAnimation(offset + MAP_WIDTH);
		if (m_GameMap[offset].full == 0x9999)
			cancelAnimation(offset);
		break;

	case 0x30:
		cancelAnimation(offset - 1);
		cancelAnimation(offset + MAP_WIDTH);
		if (m_GameMap[offset].full == 0x9999)
			cancelAnimation(offset);
		break;

	case 0x50:
		cancelAnimation(offset - 1);
		break;

	case 0x60:
		cancelAnimation(offset + 1);
		break;

	//case 0x70:
	//	cancelAnimation(offset + MAP_WIDTH);
	//	break;
	}
}

//
// Cancels any animation at the specified position
//
void SupaplexLogic::cancelAnimation(uint32 offset)
{
	if (m_GameMap[offset].id != MAP_BOOM)
		m_GameMap[offset].full = MAP_SPACE;
}

//
// Updates state of zonks and enemies
//
void SupaplexLogic::updateZonksAndEnemies()
{
	// handlePlayerMovement()
	uint32 offset = MAP_WIDTH + 1;
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT - MAP_WIDTH - 1; i++, offset++)
	{
		switch (m_GameMap[offset].id)
		{
		case MAP_ZONK:
			updateZonk(offset);
			break;

		case MAP_INFOTRON:
			updateInfotron(offset);
			break;

		case MAP_DISK_ORANGE:
			updateOrangeDisk(offset);
			break;

		case MAP_SNIK_SNAK:
			updateSnikSnak(offset);
			break;

		case MAP_TERMINAL:
			updateTerminal(offset);
			break;

		case MAP_ELECTRON:	
			updateElectron(offset);
			break;

		case MAP_BUG:
			updateBug(offset);
			break;

		case MAP_BOOM:
			updateBoom(offset);
			break;
		}
	}

	if (m_PlayerDied && m_TimeoutToLeaveTheLevel == 0)
	{
		m_PlayerDied = false;
		m_PlayerExploded = true;
		createExplosion(m_PlayerPrevMapOffset);
		m_TimeoutToLeaveTheLevel = 64;
	}	
}

//
// Places red disk if required
//
void SupaplexLogic::updateRedDisk()
{
	if ((char)m_RedDiskDetonateTimer <= 1)
		return;

	if (m_GameMap[m_RedDiskPosition].full == MAP_SPACE)
		m_GameMap[m_RedDiskPosition].full = MAP_DISK_RED;

	m_RedDiskDetonateTimer++;
	if ((char)m_RedDiskDetonateTimer < 40)
		return;

	createExplosion(m_RedDiskPosition);
	m_RedDiskDetonateTimer = 0;
}

void SupaplexLogic::sub_38191(uint32 offset)
{
	if (m_GameMap[offset].id != MAP_BOOM)
		m_GameMap[offset].full = MAP_SPACE;

	if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE ||
			m_GameMap[offset - MAP_WIDTH].full == 0x9999)
	{
		if (m_GameMap[offset - MAP_WIDTH - 1].full == MAP_ZONK)
		{
			if (m_GameMap[offset - 1].full == MAP_ZONK ||
					m_GameMap[offset - 1].full == MAP_INFOTRON ||
					m_GameMap[offset - 1].full == MAP_RAM_CHIP)
			{
				m_GameMap[offset - MAP_WIDTH - 1].state = 0x60;
				m_GameMap[offset - MAP_WIDTH].full = 0x8888;
				return;
			}

			goto loc_381D6;
		}

		if (m_GameMap[offset - MAP_WIDTH - 1].full != MAP_INFOTRON)
		{
	loc_381D6:	
			if (m_GameMap[offset - MAP_WIDTH + 1].full == MAP_ZONK)
			{
				if (m_GameMap[offset + 1].full != MAP_ZONK &&
						m_GameMap[offset + 1].full != MAP_INFOTRON &&
						m_GameMap[offset + 1].full != MAP_RAM_CHIP)
					return;
				m_GameMap[offset - MAP_WIDTH + 1].state = 0x50;
				m_GameMap[offset - MAP_WIDTH].full = 0x8888;
			}
			else if (m_GameMap[offset - MAP_WIDTH + 1].full == MAP_INFOTRON)
			{
				if (m_GameMap[offset + 1].full == MAP_ZONK ||
						m_GameMap[offset + 1].full == MAP_INFOTRON ||
						m_GameMap[offset + 1].full == MAP_RAM_CHIP)
				{
					m_GameMap[offset - MAP_WIDTH + 1].state = 0x50;
					m_GameMap[offset - MAP_WIDTH].full = 0x8888;
				}
				return;
			}
			else
				return;
		}

		if (m_GameMap[offset - 1].full == MAP_ZONK ||
				m_GameMap[offset - 1].full == MAP_INFOTRON ||
				m_GameMap[offset - 1].full == MAP_RAM_CHIP)
		{
			m_GameMap[offset - MAP_WIDTH - 1].state = 0x60;
			m_GameMap[offset - MAP_WIDTH].full = 0x8888;
			return;
		}
		else
			goto loc_381D6;

		return;
	}

	if (m_GameMap[offset - MAP_WIDTH].full == MAP_ZONK ||
			m_GameMap[offset - MAP_WIDTH].full == MAP_INFOTRON)
	{
		m_GameMap[offset - MAP_WIDTH].state = 0x40;
		return;
	}

	return;	
}

//
// Updates state of the zonk
//
void SupaplexLogic::updateZonk(uint32 offset)
{
	byte b;

	if (unlikely(m_GameMap[offset].id != MAP_ZONK))
		return;

	if (m_GameMap[offset].state == 0)
	{
		if (m_FreezeZonks)
			return;

		switch (m_GameMap[offset + MAP_WIDTH].id)
		{
		case MAP_SPACE:
			m_GameMap[offset].state = 0x40;
			break;

		case MAP_RAM_CHIP:
		case MAP_ZONK:
		case MAP_INFOTRON:
			if ((m_GameMap[offset + MAP_WIDTH - 1].full == MAP_SPACE ||
					m_GameMap[offset + MAP_WIDTH - 1].full == 0x8888 ||
					m_GameMap[offset + MAP_WIDTH - 1].full == 0xAAAA) &&
					m_GameMap[offset - 1].full == MAP_SPACE)
			{
				m_GameMap[offset].state = 0x50;
				m_GameMap[offset - 1].full = 0x8888;
				break;
			}
		_l2:
			if (m_GameMap[offset + MAP_WIDTH + 1].full == MAP_SPACE ||
					m_GameMap[offset + MAP_WIDTH + 1].full == 0x8888 ||
					m_GameMap[offset + MAP_WIDTH + 1].full == 0xAAAA)
			{
				if (m_GameMap[offset + 1].full != MAP_SPACE)
				{
					if (m_GameMap[offset + 1].full != 0x9999)
						return;
					if (m_GameMap[offset - MAP_WIDTH + 1].id != MAP_ZONK)
						return;
				}
				m_GameMap[offset].state = 0x60;
				m_GameMap[offset + 1].full = 0x8888;
				break;
			}
			return;

		default:
			return;
		}
	}

	char val = m_GameMap[offset].state;
	switch (val & 0xF0)
	{
	case 0x10:
	_l8:
		val = m_GameMap[offset].state + 1;
		if (val == 0x16)
		{
			m_GameMap[offset].state = val;
			sub_3826F(offset - MAP_WIDTH, MAP_ZONK, MAP_INFOTRON);
			return;
		}
		if (val < 0x18)
		{
			m_GameMap[offset].state = val;
			return;
		}

		m_GameMap[offset].state = 0;
		if (m_FreezeZonks)
			return;

		switch (m_GameMap[offset + MAP_WIDTH].id)
		{
		case MAP_SPACE:
			if (m_GameMap[offset + MAP_WIDTH].state != 0)
				break;
			goto _l_a0;
		case 0x99:
			if (m_GameMap[offset + MAP_WIDTH].state != 0x99)
				break;
		_l_a0:
			m_GameMap[offset].state = 0x70;
			m_GameMap[offset].id = MAP_ZONK;
			m_GameMap[offset + MAP_WIDTH].full = 0x9999;
			return;

		case MAP_MURPHY:
			b = m_GameMap[offset + MAP_WIDTH].state;
			if (b == 0x0E || b == 0x0F || b == 0x28 || b == 0x29 || b == 0x25 || b == 0x26)
				return;
			goto _l16;

		case MAP_SNIK_SNAK:
			createExplosion(offset + MAP_WIDTH);
			return;

		case 0xBB:
			if (m_GameMap[offset + MAP_WIDTH].state == 0x02)
			{
			_l16:
				if (m_GameMap[offset + MAP_WIDTH - 1].id == MAP_ELECTRON)
					m_GameMap[offset + MAP_WIDTH].full = MAP_ELECTRON;
				if (m_GameMap[offset + MAP_WIDTH - 1].id != MAP_BOOM)
					m_GameMap[offset + MAP_WIDTH - 1].full = MAP_SPACE;
					createExplosion(offset + MAP_WIDTH);
				return;
			}
			else if (m_GameMap[offset + MAP_WIDTH].state == 0x04)
			{
				if (m_GameMap[offset + MAP_WIDTH + 1].id == MAP_ELECTRON)
					m_GameMap[offset + MAP_WIDTH].full = MAP_ELECTRON;
				if (m_GameMap[offset + MAP_WIDTH + 1].id != MAP_BOOM)
					m_GameMap[offset + MAP_WIDTH + 1].full = MAP_SPACE;
				createExplosion(offset + MAP_WIDTH);
				return;
			}
			break;

		case MAP_ELECTRON:
			createExplosion(offset + MAP_WIDTH);
			return;

		case MAP_DISK_ORANGE:
			m_Explosions[offset + MAP_WIDTH] = 6;
			return;
		}

		SND_PlaySound("sounds/zonk_fall.wav", offsetTo3D(offset));

		if (m_GameMap[offset + MAP_WIDTH].full == MAP_ZONK ||
			m_GameMap[offset + MAP_WIDTH].full == MAP_INFOTRON ||
			m_GameMap[offset + MAP_WIDTH].full == MAP_RAM_CHIP)
		{
			if (m_GameMap[offset + MAP_WIDTH - 1].full == MAP_SPACE ||
				m_GameMap[offset + MAP_WIDTH - 1].full == 0x8888 ||
				m_GameMap[offset + MAP_WIDTH - 1].full == 0xAAAA)
			{
				if (m_GameMap[offset - 1].full != MAP_SPACE)
					goto _l2;
				m_GameMap[offset].state = 0x50;
				m_GameMap[offset - 1].full = 0x8888;
				return;
			}
			if (m_GameMap[offset + MAP_WIDTH + 1].full == MAP_SPACE ||
				m_GameMap[offset + MAP_WIDTH + 1].full == 0x8888 ||
				m_GameMap[offset + MAP_WIDTH + 1].full == 0xAAAA)
			{
				if (m_GameMap[offset + 1].full != MAP_SPACE)
					return;
				m_GameMap[offset].state = 0x60;
				m_GameMap[offset + 1].full = 0x8888;
				return;
			}
			return;
		}

		return;

	case 0x20:
		if (++val == 0x24)
			m_GameMap[offset + 1].full = 0xAAAA;
		if (val == 0x26)
		{
			m_GameMap[offset].state = val;
			sub_3826F(offset + 1, MAP_ZONK, MAP_INFOTRON);
		}
		else if (val < 0x28)
			m_GameMap[offset].state = val;
		else
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + MAP_WIDTH].state = 0x10;
			m_GameMap[offset + MAP_WIDTH].id = MAP_ZONK;
		}
		return;

	case 0x30:
		if (++val == 0x34)
			m_GameMap[offset - 1].full = 0xAAAA;
		if (val == 0x36)
		{
			m_GameMap[offset].state = val;
			sub_3826F(offset - 1, MAP_ZONK, MAP_INFOTRON);
		}
		else if (val < 0x38)
			m_GameMap[offset].state = val;
		else
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + MAP_WIDTH].state = 0x10;
			m_GameMap[offset + MAP_WIDTH].id = MAP_ZONK;
		}
		return;
	}

	if (m_FreezeZonks)
		return;

	switch (val & 0xF0)
	{
	case 0x40:
		if (++val < 0x42)
			m_GameMap[offset].state = val;
		else if (m_GameMap[offset + MAP_WIDTH].full != MAP_SPACE)
			m_GameMap[offset].state = val - 1;
		else
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + MAP_WIDTH].state = 0x10;
			m_GameMap[offset + MAP_WIDTH].id = MAP_ZONK;
		}
		break;

	case 0x50:
		if (++val < 0x52)
			m_GameMap[offset].state = val;
		else if (m_GameMap[offset + MAP_WIDTH - 1].full == MAP_SPACE &&
				 (m_GameMap[offset - 1].full == MAP_SPACE || m_GameMap[offset - 1].full == 0x8888))
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset - 1].state = 0x22;
			m_GameMap[offset - 1].id = MAP_ZONK;
			m_GameMap[offset + MAP_WIDTH - 1].full = 0xFFFF;
		}
		else
		{
			--val;
			m_GameMap[offset].state = val;
		}
		break;

	case 0x60:
		if (++val < 0x62)
			m_GameMap[offset].state = val;
		else if (m_GameMap[offset + MAP_WIDTH + 1].full == MAP_SPACE &&
				 (m_GameMap[offset + 1].full == MAP_SPACE || m_GameMap[offset + 1].full == 0x8888))
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + 1].state = 0x32;
			m_GameMap[offset + 1].id = MAP_ZONK;
			m_GameMap[offset + MAP_WIDTH + 1].full = 0xFFFF;
		}
		else
		{
			--val;
			m_GameMap[offset].state = val;
		}
		break;

	case 0x70:
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset + MAP_WIDTH].full == 0x9999)
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + MAP_WIDTH].state = 0x10;
			m_GameMap[offset + MAP_WIDTH].id = MAP_ZONK;
			goto _l8;
		}
		break;
	}
}

//
// Updates state of the infotron
//
void SupaplexLogic::updateInfotron(uint32 offset)
{
	byte b;

	if (unlikely(m_GameMap[offset].id != MAP_INFOTRON))
		return;

	if (m_GameMap[offset].state == 0)
	{
		switch (m_GameMap[offset + MAP_WIDTH].id)
		{
		case MAP_SPACE:
			m_GameMap[offset].state = 0x40;
			break;

		case MAP_RAM_CHIP:
		case MAP_ZONK:
		case MAP_INFOTRON:
			if ((m_GameMap[offset + MAP_WIDTH - 1].full == MAP_SPACE ||
					m_GameMap[offset + MAP_WIDTH - 1].full == 0x8888 ||
					m_GameMap[offset + MAP_WIDTH - 1].full == 0xAAAA) &&
					m_GameMap[offset - 1].full == MAP_SPACE)
			{
				m_GameMap[offset].state = 0x50;
				m_GameMap[offset - 1].full = 0x8888;
				break;
			}
		_l2:
			if (m_GameMap[offset + MAP_WIDTH + 1].full == MAP_SPACE ||
					m_GameMap[offset + MAP_WIDTH + 1].full == 0x8888 ||
					m_GameMap[offset + MAP_WIDTH + 1].full == 0xAAAA)
			{
				if (m_GameMap[offset + 1].full != MAP_SPACE)
					return;
				m_GameMap[offset].state = 0x60;
				m_GameMap[offset + 1].full = 0x8888;
				break;
			}
			return;

		default:
			return;
		}
	}

	char val = m_GameMap[offset].state;
	switch (val & 0xF0)
	{
	case 0x10:
	_l8:
		val = m_GameMap[offset].state + 1;
		if (val == 0x16)
		{
			m_GameMap[offset].state = val;
			sub_3826F(offset - MAP_WIDTH, MAP_INFOTRON, MAP_ZONK);
			return;
		}
		if (val < 0x18)
		{
			m_GameMap[offset].state = val;
			return;
		}

		m_GameMap[offset].state = 0;

		switch (m_GameMap[offset + MAP_WIDTH].id)
		{
		case MAP_SPACE:
			if (m_GameMap[offset + MAP_WIDTH].state != 0)
				break;
			goto _l_a0;
		case 0x99:
			if (m_GameMap[offset + MAP_WIDTH].state != 0x99)
				break;
		_l_a0:
			m_GameMap[offset].state = 0x70;
			m_GameMap[offset].id = MAP_INFOTRON;
			m_GameMap[offset + MAP_WIDTH].full = 0x9999;
			return;

		case MAP_MURPHY:
			b = m_GameMap[offset + MAP_WIDTH].state;
			if (b == 0x0E || b == 0x0F || b == 0x28 || b == 0x29 || b == 0x25 || b == 0x26)
				return;
			createExplosion(offset + MAP_WIDTH);
			return;

		case MAP_DISK_RED:
		case MAP_DISK_YELLOW:
		case MAP_DISK_ORANGE:
			if (m_GameMap[offset + MAP_WIDTH].state != 0)
				break;
		case MAP_SNIK_SNAK:
		case MAP_ELECTRON:
			createExplosion(offset + MAP_WIDTH);
			return;
		}

		SND_PlaySound("sounds/zonk_fall.wav", offsetTo3D(offset));

		if (m_GameMap[offset + MAP_WIDTH].full == MAP_ZONK ||
			m_GameMap[offset + MAP_WIDTH].full == MAP_INFOTRON ||
			m_GameMap[offset + MAP_WIDTH].full == MAP_RAM_CHIP)
		{
			if (m_GameMap[offset + MAP_WIDTH - 1].full == MAP_SPACE ||
				m_GameMap[offset + MAP_WIDTH - 1].full == 0x8888 ||
				m_GameMap[offset + MAP_WIDTH - 1].full == 0xAAAA)
			{
				if (m_GameMap[offset - 1].full != MAP_SPACE)
					goto _l2;
				m_GameMap[offset].state = 0x50;
				m_GameMap[offset - 1].full = 0x8888;
				return;
			}
			if (m_GameMap[offset + MAP_WIDTH + 1].full == MAP_SPACE ||
				m_GameMap[offset + MAP_WIDTH + 1].full == 0x8888 ||
				m_GameMap[offset + MAP_WIDTH + 1].full == 0xAAAA)
			{
				if (m_GameMap[offset + 1].full != MAP_SPACE)
					return;
				m_GameMap[offset].state = 0x60;
				m_GameMap[offset + 1].full = 0x8888;
				return;
			}
			return;
		}

		return;

	case 0x20:
		if (++val == 0x24)
			m_GameMap[offset + 1].full = 0xAAAA;
		if (val == 0x26)
		{
			m_GameMap[offset].state = val;
			sub_3826F(offset + 1, MAP_INFOTRON, MAP_ZONK);
		}
		else if (val < 0x28)
			m_GameMap[offset].state = val;
		else
		{
			m_GameMap[offset].state = 0x70;
			m_GameMap[offset].id = MAP_INFOTRON;
		}
		return;

	case 0x30:
		if (++val == 0x34)
			m_GameMap[offset - 1].full = 0xAAAA;
		if (val == 0x36)
		{
			m_GameMap[offset].state = val;
			sub_3826F(offset - 1, MAP_INFOTRON, MAP_ZONK);
		}
		else if (val < 0x38)
			m_GameMap[offset].state = val;
		else
		{
			m_GameMap[offset].state = 0x70;
			m_GameMap[offset].id = MAP_INFOTRON;
		}
		return;

	case 0x40:
		if (++val < 0x42)
			m_GameMap[offset].state = val;
		else if (m_GameMap[offset + MAP_WIDTH].full != MAP_SPACE)
			m_GameMap[offset].state = val - 1;
		else
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + MAP_WIDTH].state = 0x10;
			m_GameMap[offset + MAP_WIDTH].id = MAP_INFOTRON;
		}
		break;

	case 0x50:
		if (++val < 0x52)
			m_GameMap[offset].state = val;
		else if (m_GameMap[offset + MAP_WIDTH - 1].full == MAP_SPACE &&
				 (m_GameMap[offset - 1].full == MAP_SPACE || m_GameMap[offset - 1].full == 0x8888))
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset - 1].state = 0x22;
			m_GameMap[offset - 1].id = MAP_INFOTRON;
			m_GameMap[offset + MAP_WIDTH - 1].full = 0x9999;
		}
		else
		{
			--val;
			m_GameMap[offset].state = val;
		}
		break;

	case 0x60:
		if (++val < 0x62)
			m_GameMap[offset].state = val;
		else if (m_GameMap[offset + MAP_WIDTH + 1].full == MAP_SPACE &&
				 (m_GameMap[offset + 1].full == MAP_SPACE || m_GameMap[offset + 1].full == 0x8888))
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + 1].state = 0x32;
			m_GameMap[offset + 1].id = MAP_INFOTRON;
			m_GameMap[offset + MAP_WIDTH + 1].full = 0x9999;
		}
		else
		{
			--val;
			m_GameMap[offset].state = val;
		}
		break;

	case 0x70:
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset + MAP_WIDTH].full == 0x9999)
		{
			m_GameMap[offset].full = 0xFFFF;
			m_GameMap[offset + MAP_WIDTH].state = 0x10;
			m_GameMap[offset + MAP_WIDTH].id = MAP_INFOTRON;
			goto _l8;
		}
		break;
	}
}

//
// Updates state of the boom
//
void SupaplexLogic::updateBoom(uint32 offset)
{
	if (unlikely(m_GameMap[offset].id != MAP_BOOM))
		return;

	if ((m_CurrentFrameNumber & 3) != 0)
		return;

	byte val = m_GameMap[offset].state;
	if ((val & 0x80) != 0)
	{
		if (++val != 0x89)
			m_GameMap[offset].state = val;
		else
			m_GameMap[offset].full = MAP_INFOTRON;
	}
	else
	{
		val++;
		m_GameMap[offset].state = val;

		if (val == 8)
			m_GameMap[offset].full = MAP_SPACE;
	}
}

//
// Updates state of the orange disk
//
void SupaplexLogic::updateOrangeDisk(uint32 offset)
{
	if (unlikely(m_GameMap[offset].id != MAP_DISK_ORANGE))
		return;

	MapItem item = m_GameMap[offset];
	if ((char)item.state >= 0x30)
	{
		byte val = item.state + 1;
		if ((val & 0x7) != 0)
			m_GameMap[offset].state = val;
		else
		{
			m_GameMap[offset].full = MAP_SPACE;
			m_GameMap[offset + MAP_WIDTH].full = MAP_DISK_ORANGE;
			if (m_GameMap[offset + 2 * MAP_WIDTH].full == MAP_SPACE)
			{
				m_GameMap[offset + MAP_WIDTH].state = 0x30;
				m_GameMap[offset + 2 * MAP_WIDTH].state = MAP_DISK_ORANGE;
			}
			else if (m_GameMap[offset + 2 * MAP_WIDTH].id != MAP_BOOM)
				createExplosion(offset + MAP_WIDTH);
		}
	}
	else if ((char)item.state >= 0x20)
	{
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
			m_GameMap[offset].full = MAP_DISK_ORANGE;
		else
		{
			byte val = item.state + 1;
			if (val == 0x22)
				val = 0x30;
			m_GameMap[offset].state = val;
		}	
	}
	else
	{
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x20;
			m_GameMap[offset + MAP_WIDTH].state = MAP_DISK_ORANGE;
		}	
	}	
}

//
// Updates state of the bug
//
void SupaplexLogic::updateBug(uint32 offset)
{
	if (unlikely(m_GameMap[offset].id != MAP_BUG))
		return;

	if ((m_CurrentFrameNumber & 3) != 0)
		return;

	if (m_GameMap[offset].state == 0)
		SND_PlaySound("sounds/bug.wav", offsetTo3D(offset));

	char state = m_GameMap[offset].state + 1;
	if (state >= 14)
		state = -((rand() & 0x3F) + 32);
	m_GameMap[offset].state = state;

	/*
	if (state >= 0)
	{
		if (m_GameMap[offset - MAP_WIDTH - 1].id == MAP_MURPHY ||
			m_GameMap[offset - MAP_WIDTH].id == MAP_MURPHY ||
			m_GameMap[offset - MAP_WIDTH + 1].id == MAP_MURPHY ||
			m_GameMap[offset - 1].id == MAP_MURPHY ||
			m_GameMap[offset + 1].id == MAP_MURPHY ||
			m_GameMap[offset + MAP_WIDTH - 1].id == MAP_MURPHY ||
			m_GameMap[offset + MAP_WIDTH].id == MAP_MURPHY ||
			m_GameMap[offset + MAP_WIDTH + 1].id == MAP_MURPHY)
		{
			SND_PlaySound("sounds/bug.wav");
		}
	}
	*/
}	

//
// Updates state of the terminal
//
void SupaplexLogic::updateTerminal(uint32 offset)
{
	if (unlikely(m_GameMap[offset].id != MAP_TERMINAL))
		return;

	if (m_TerminalsActivated && (m_CurrentFrameNumber & 3) != 0)
		return;

	if (!m_TerminalsActivated && (m_CurrentFrameNumber & 15) != 0)
		return;

	byte val = m_GameMap[offset].state + 1;
	if (val < 4)
	{
		m_GameMap[offset].state = val;
		return;
	}

	m_GameMap[offset].state = 0;
}

//
// Updates state of the snik-snak
//
void SupaplexLogic::updateSnikSnak(uint32 offset)
{
	if (m_FreezeEnemies)
		return;

	if (unlikely(m_GameMap[offset].id != MAP_SNIK_SNAK))
		return;

	switch (m_GameMap[offset].state >> 3)
	{
	case 0:
		snikSnakHandler_1(offset);
		break;

	case 1:
		snikSnakHandler_2(offset);
		break;

	case 2:
		snikSnakHandler_3(offset);
		break;

	case 3:
		snikSnakHandler_4(offset);
		break;

	case 4:
		snikSnakHandler_5(offset);
		break;

	case 5:
		snikSnakHandler_6(offset);
		break;
	}
}

//
// Updates state of the snik-snak
//
void SupaplexLogic::snikSnakHandler_1(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	if ((m_CurrentFrameNumber & 3) == 0)
	{
		m_GameMap[offset].state = (item.state + 1) & 7;
		return;
	}

	if ((m_CurrentFrameNumber & 3) != 3)
		return;

	switch (item.state)
	{
	case 0:
		if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x10;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - MAP_WIDTH].state = 0x10;
			m_GameMap[offset - MAP_WIDTH].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset - MAP_WIDTH].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset - MAP_WIDTH];
    	break;

	case 2:
		if (m_GameMap[offset - 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x20;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - 1].state = 0x18;
			m_GameMap[offset - 1].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset - 1].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset - 1];
    	break;

	case 4:
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x30;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + MAP_WIDTH].state = 0x20;
			m_GameMap[offset + MAP_WIDTH].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset + MAP_WIDTH].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset + MAP_WIDTH];
    	break;

	case 6:
		if (m_GameMap[offset + 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x40;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + 1].state = 0x28;
			m_GameMap[offset + 1].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset + 1].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset + 1];
    	break;

	default:
		return;
	}

	if (item.state != MURPHY_STATE_TELEPORT_RIGHT && item.state != MURPHY_STATE_TELEPORT_LEFT &&
			item.state != MURPHY_STATE_TELEPORT_UP && item.state != MURPHY_STATE_TELEPORT_DOWN)
		createExplosion(offset);
}

//
// Updates state of the snik-snak
//
void SupaplexLogic::snikSnakHandler_2(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	if ((m_CurrentFrameNumber & 3) == 0)
	{
		m_GameMap[offset].state = ((item.state + 1) & 7) | 0x08;
		return;
	}

	if ((m_CurrentFrameNumber & 3) != 3)
		return;

	switch (item.state)
	{
	case 8:
		if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x10;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - MAP_WIDTH].state = 0x10;
			m_GameMap[offset - MAP_WIDTH].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset - MAP_WIDTH].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset - MAP_WIDTH];
    	break;

	case 10:
		if (m_GameMap[offset + 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x40;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + 1].state = 0x28;
			m_GameMap[offset + 1].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset + 1].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset + 1];
    	break;

	case 12:	
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x30;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + MAP_WIDTH].state = 0x20;
			m_GameMap[offset + MAP_WIDTH].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset + MAP_WIDTH].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset + MAP_WIDTH];
    	break;

	case 14:
		if (m_GameMap[offset - 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x20;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - 1].state = 0x18;
			m_GameMap[offset - 1].id = MAP_SNIK_SNAK;
			return;
		}

		if (m_GameMap[offset - 1].id != MAP_MURPHY)
			return;

		item = m_GameMap[offset - 1];
	   	break;

	default:
		return;
	}	

	if (item.state != MURPHY_STATE_TELEPORT_RIGHT && item.state != MURPHY_STATE_TELEPORT_LEFT &&
			item.state != MURPHY_STATE_TELEPORT_UP && item.state != MURPHY_STATE_TELEPORT_DOWN)
		createExplosion(offset);
}

//
// Updates state of the snik-snak
//
void SupaplexLogic::snikSnakHandler_3(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = item.state - 15;
	if (state == 7 && m_GameMap[offset + MAP_WIDTH].id != MAP_BOOM)
		m_GameMap[offset + MAP_WIDTH].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x10;
		return;
	}	

	m_GameMap[offset].full = MAP_SNIK_SNAK;

	if (m_GameMap[offset - 1].full == MAP_SPACE || m_GameMap[offset - 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x01;
	else if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x10;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset - MAP_WIDTH].state = 0x10;
		m_GameMap[offset - MAP_WIDTH].id = MAP_SNIK_SNAK;
	}	
	else if (m_GameMap[offset - MAP_WIDTH].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset + 1].full == MAP_SPACE || m_GameMap[offset + 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x09;
	else
		m_GameMap[offset].state = 0x01;
}

//
// Updates state of the snik-snak
//
void SupaplexLogic::snikSnakHandler_4(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = ((item.state - 32) & 7) + 1;
	if (state == 7 && m_GameMap[offset + 1].id != MAP_BOOM)
		m_GameMap[offset + 1].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x18;
		return;
	}	

	m_GameMap[offset].full = MAP_SNIK_SNAK;

	if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset + MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x03;
	else if (m_GameMap[offset - 1].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x20;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset - 1].state = 0x18;
		m_GameMap[offset - 1].id = MAP_SNIK_SNAK;
	}	
	else if (m_GameMap[offset - 1].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset - MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x0F;
	else
		m_GameMap[offset].state = 0x03;
}

//
// Updates state of the snik-snak
//
void SupaplexLogic::snikSnakHandler_5(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = item.state - 31;
	if (state == 7 && m_GameMap[offset - MAP_WIDTH].id != MAP_BOOM)
		m_GameMap[offset - MAP_WIDTH].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x20;
		return;
	}	

	m_GameMap[offset].full = MAP_SNIK_SNAK;

	if (m_GameMap[offset + 1].full == MAP_SPACE || m_GameMap[offset + 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x05;
	else if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x30;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset + MAP_WIDTH].state = 0x20;
		m_GameMap[offset + MAP_WIDTH].id = MAP_SNIK_SNAK;
	}	
	else if (m_GameMap[offset + MAP_WIDTH].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset - 1].full == MAP_SPACE || m_GameMap[offset - 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x0D;
	else
		m_GameMap[offset].state = 0x05;
}

//
// Updates state of the snik-snak
//
void SupaplexLogic::snikSnakHandler_6(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = (item.state & 7) + 1;
	if (state == 7 && m_GameMap[offset - 1].id != MAP_BOOM)
		m_GameMap[offset - 1].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x28;
		return;
	}	

	m_GameMap[offset].full = MAP_SNIK_SNAK;

	if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset - MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x07;
	else if (m_GameMap[offset + 1].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x40;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset + 1].state = 0x28;
		m_GameMap[offset + 1].id = MAP_SNIK_SNAK;
	}	
	else if (m_GameMap[offset + 1].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset + MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x0B;
	else
		m_GameMap[offset].state = 0x07;
}

//
// Updates state of the electron
//
void SupaplexLogic::updateElectron(uint32 offset)
{
	if (m_FreezeEnemies)
		return;

	if (unlikely(m_GameMap[offset].id != MAP_ELECTRON))
		return;

	switch (m_GameMap[offset].state >> 3)
	{
	case 0:
		electronHandler_1(offset);
		break;

	case 1:
		electronHandler_2(offset);
		break;

	case 2:
		electronHandler_3(offset);
		break;

	case 3:
		electronHandler_4(offset);
		break;

	case 4:
		electronHandler_5(offset);
		break;

	case 5:
		electronHandler_6(offset);
		break;
	}
}

//
// Updates state of the electron
//
void SupaplexLogic::electronHandler_1(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	if ((m_CurrentFrameNumber & 3) == 0)
	{
		m_GameMap[offset].state = (item.state + 1) & 7;
		return;
	}

	if ((m_CurrentFrameNumber & 3) != 3)
		return;

	switch (item.state)
	{
	case 0:
		if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x01;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - MAP_WIDTH].state = 0x10;
			m_GameMap[offset - MAP_WIDTH].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset - MAP_WIDTH].id != MAP_MURPHY)
			return;

    	break;

	case 2:
		if (m_GameMap[offset - 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x02;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - 1].state = 0x18;
			m_GameMap[offset - 1].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset - 1].id != MAP_MURPHY)
			return;

    	break;

	case 4:
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x03;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + MAP_WIDTH].state = 0x20;
			m_GameMap[offset + MAP_WIDTH].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset + MAP_WIDTH].id != MAP_MURPHY)
			return;

    	break;

	case 6:
		if (m_GameMap[offset + 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x04;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + 1].state = 0x28;
			m_GameMap[offset + 1].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset + 1].id != MAP_MURPHY)
			return;

    	break;

	default:
		return;
	}

	createExplosion(offset);
}

//
// Updates state of the electron
//
void SupaplexLogic::electronHandler_2(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	if ((m_CurrentFrameNumber & 3) == 0)
	{
		m_GameMap[offset].state = ((item.state + 1) & 7) | 0x08;
		return;
	}

	if ((m_CurrentFrameNumber & 3) != 3)
		return;

	switch (item.state)
	{
	case 8:
		if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x01;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - MAP_WIDTH].state = 0x10;
			m_GameMap[offset - MAP_WIDTH].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset - MAP_WIDTH].id != MAP_MURPHY)
			return;

    	break;

	case 10:
		if (m_GameMap[offset + 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x04;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + 1].state = 0x28;
			m_GameMap[offset + 1].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset + 1].id != MAP_MURPHY)
			return;

    	break;

	case 12:	
		if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x03;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset + MAP_WIDTH].state = 0x20;
			m_GameMap[offset + MAP_WIDTH].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset + MAP_WIDTH].id != MAP_MURPHY)
			return;

    	break;

	case 14:
		if (m_GameMap[offset - 1].full == MAP_SPACE)
		{
			m_GameMap[offset].state = 0x02;
			m_GameMap[offset].id = 0xBB;
			m_GameMap[offset - 1].state = 0x18;
			m_GameMap[offset - 1].id = MAP_ELECTRON;
			return;
		}

		if (m_GameMap[offset - 1].id != MAP_MURPHY)
			return;

	   	break;

	default:
		return;
	}	

	createExplosion(offset);
}

//
// Updates state of the electron
//
void SupaplexLogic::electronHandler_3(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = item.state - 15;
	if (state == 7 && m_GameMap[offset + MAP_WIDTH].id != MAP_BOOM)
		m_GameMap[offset + MAP_WIDTH].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x10;
		return;
	}	

	m_GameMap[offset].full = MAP_ELECTRON;

	if (m_GameMap[offset - 1].full == MAP_SPACE || m_GameMap[offset - 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x01;
	else if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x01;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset - MAP_WIDTH].state = 0x10;
		m_GameMap[offset - MAP_WIDTH].id = MAP_ELECTRON;
	}	
	else if (m_GameMap[offset - MAP_WIDTH].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset + 1].full == MAP_SPACE || m_GameMap[offset + 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x09;
	else
		m_GameMap[offset].state = 0x01;
}

//
// Updates state of the electron
//
void SupaplexLogic::electronHandler_4(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = (item.state & 7) + 1;
	if (state == 7 && m_GameMap[offset + 1].id != MAP_BOOM)
		m_GameMap[offset + 1].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x18;
		return;
	}	

	m_GameMap[offset].full = MAP_ELECTRON;

	if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset + MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x03;
	else if (m_GameMap[offset - 1].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x02;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset - 1].state = 0x18;
		m_GameMap[offset - 1].id = MAP_ELECTRON;
	}	
	else if (m_GameMap[offset - 1].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset - MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x0F;
	else
		m_GameMap[offset].state = 0x03;
}

//
// Updates state of the electron
//
void SupaplexLogic::electronHandler_5(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = item.state - 31;
	if (state == 7 && m_GameMap[offset - MAP_WIDTH].id != MAP_BOOM)
		m_GameMap[offset - MAP_WIDTH].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x20;
		return;
	}	

	m_GameMap[offset].full = MAP_ELECTRON;

	if (m_GameMap[offset + 1].full == MAP_SPACE || m_GameMap[offset + 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x05;
	else if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x03;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset + MAP_WIDTH].state = 0x20;
		m_GameMap[offset + MAP_WIDTH].id = MAP_ELECTRON;
	}	
	else if (m_GameMap[offset + MAP_WIDTH].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset - 1].full == MAP_SPACE || m_GameMap[offset - 1].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x0D;
	else
		m_GameMap[offset].state = 0x05;
}

//
// Updates state of the electron
//
void SupaplexLogic::electronHandler_6(uint32 offset)
{
	MapItem item = m_GameMap[offset];

	char state = (item.state & 7) + 1;
	if (state == 7 && m_GameMap[offset - 1].id != MAP_BOOM)
		m_GameMap[offset - 1].full = MAP_SPACE;

	if (state < 8)
	{
		m_GameMap[offset].state = state + 0x28;
		return;
	}	

	m_GameMap[offset].full = MAP_ELECTRON;

	if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset - MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x07;
	else if (m_GameMap[offset + 1].full == MAP_SPACE)
	{
		m_GameMap[offset].state = 0x04;
		m_GameMap[offset].id = 0xBB;
		m_GameMap[offset + 1].state = 0x28;
		m_GameMap[offset + 1].id = MAP_ELECTRON;
	}	
	else if (m_GameMap[offset + 1].id == MAP_MURPHY)
		createExplosion(offset);
	else if (m_GameMap[offset + MAP_WIDTH].full == MAP_SPACE || m_GameMap[offset + MAP_WIDTH].id == MAP_MURPHY)
		m_GameMap[offset].state = 0x0B;
	else
		m_GameMap[offset].state = 0x07;
}

void SupaplexLogic::sub_3826F(uint32 offset, byte baseMapItem, byte secondMapItem)
{
	if (m_GameMap[offset].id != MAP_BOOM)
		m_GameMap[offset].full = 0;

	if (m_GameMap[offset - MAP_WIDTH].full == MAP_SPACE)
		goto _l1;
	if (m_GameMap[offset - MAP_WIDTH].full != 0x9999)
		return;
	if (m_GameMap[offset - MAP_WIDTH * 2].id != secondMapItem)
		return;

_l1:
	if (m_GameMap[offset - MAP_WIDTH - 1].full == baseMapItem)
		goto _l3;
_l2:
	if (m_GameMap[offset - MAP_WIDTH + 1].full == baseMapItem)
		goto _l4;
	return;
_l3:
	if (m_GameMap[offset - 1].full != MAP_ZONK && m_GameMap[offset - 1].full != MAP_INFOTRON &&
			m_GameMap[offset - 1].full != MAP_RAM_CHIP)
		goto _l2;
	m_GameMap[offset - MAP_WIDTH - 1].state = 0x60;
	m_GameMap[offset - MAP_WIDTH - 1].id = baseMapItem;
	m_GameMap[offset - MAP_WIDTH].full = 0x8888;
	return;

_l4:
	if (m_GameMap[offset + 1].full != MAP_ZONK && m_GameMap[offset + 1].full != MAP_INFOTRON &&
			m_GameMap[offset + 1].full != MAP_RAM_CHIP)
		return;

	m_GameMap[offset - MAP_WIDTH + 1].state = 0x50;
	m_GameMap[offset - MAP_WIDTH + 1].id = baseMapItem;
	m_GameMap[offset - MAP_WIDTH].full = 0x8888;
}

void SupaplexLogic::sub_3DB7E(uint32 offset)
{
	offset += MAP_WIDTH;
	if (m_GameMap[offset].id == MAP_SNIK_SNAK || m_GameMap[offset].id == 0xBB)
		createExplosion(offset);
}
