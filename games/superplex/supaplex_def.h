#ifndef __SUPAPLEX_DEF_H__
#define __SUPAPLEX_DEF_H__

////////////////////////////////////////////////////////
// MAP DIMENSIONS

static const int MAP_WIDTH = 60;
static const int MAP_HEIGHT = 24;

////////////////////////////////////////////////////////
// MAP ELEMENTS

static const int MAP_SPACE = 0;
static const int MAP_ZONK = 1;
static const int MAP_BASE = 2;
static const int MAP_MURPHY = 3;
static const int MAP_INFOTRON = 4;
static const int MAP_RAM_CHIP = 5;
static const int MAP_GRAY_DENTED_PYRAMID = 6;
static const int MAP_HARDWARE = 6;
static const int MAP_EXIT = 7;
static const int MAP_DISK_ORANGE = 8;
static const int MAP_PORT_LEFT_TO_RIGHT = 9;
static const int MAP_PORT_UP_TO_DOWN  = 10;
static const int MAP_PORT_RIGHT_TO_LEFT = 11;
static const int MAP_PORT_DOWN_TO_UP = 12;
static const int MAP_PORT_SPECIAL_LEFT_TO_RIGHT = 13;
static const int MAP_PORT_SPECIAL_UP_TO_DOWN = 14;
static const int MAP_PORT_SPECIAL_RIGHT_TO_LEFT = 15;
static const int MAP_PORT_SPECIAL_DOWN_TO_UP = 16;
static const int MAP_SNIK_SNAK = 17;
static const int MAP_DISK_YELLOW = 18;
static const int MAP_TERMINAL = 19;
static const int MAP_DISK_RED = 20;
static const int MAP_PORT_VERTICAL = 21;
static const int MAP_PORT_HORIZONTAL = 22;
static const int MAP_PORT_CROSS = 23;
static const int MAP_ELECTRON = 24;
static const int MAP_BUG = 25;
static const int MAP_RAM_CHIP_LEFT = 26;
static const int MAP_RAM_CHIP_RIGHT = 27;
static const int MAP_HARDWARE_1 = 28;
static const int MAP_HARDWARE_2 = 29;
static const int MAP_HARDWARE_3 = 30;
static const int MAP_HARDWARE_4 = 31;
static const int MAP_BOOM = 31;
static const int MAP_HARDWARE_5 = 32;
static const int MAP_HARDWARE_6 = 33;
static const int MAP_HARDWARE_7 = 34;
static const int MAP_HARDWARE_8 = 35;
static const int MAP_HARDWARE_9 = 36;
static const int MAP_HARDWARE_10 = 37;
static const int MAP_RAM_CHIP_TOP = 38;
static const int MAP_RAM_CHIP_BOTTOM = 39;
static const int MAP_INVISIBLE_WALL = 40;

enum MURPHY_STATE
{
	MURPHY_STATE_MOVE_UP_EAT_SPACE = 1,
	MURPHY_STATE_MOVE_LEFT_EAT_SPACE = 2,
	MURPHY_STATE_MOVE_DOWN_EAT_SPACE = 3,
	MURPHY_STATE_MOVE_RIGHT_EAT_SPACE = 4,
	MURPHY_STATE_MOVE_UP_EAT_BASE = 5,
	MURPHY_STATE_MOVE_LEFT_EAT_BASE = 6,
	MURPHY_STATE_MOVE_DOWN_EAT_BASE = 7,
	MURPHY_STATE_MOVE_RIGHT_EAT_BASE = 8,
	MURPHY_STATE_MOVE_UP_EAT_INFTR = 9,
	MURPHY_STATE_MOVE_LEFT_EAT_INFTR = 10,
	MURPHY_STATE_MOVE_DOWN_EAT_INFTR = 11,
	MURPHY_STATE_MOVE_RIGHT_EAT_INFTR = 12,
	MURPHY_STATE_EXIT = 13,
	MURPHY_STATE_SHIFT_ZONK_LEFT = 14,
	MURPHY_STATE_SHIFT_ZONK_RIGHT = 15,
	MURPHY_STATE_EAT_BASE_TOP = 16,
	MURPHY_STATE_EAT_BASE_LEFT = 17,
	MURPHY_STATE_EAT_BASE_BOTTOM = 18,
	MURPHY_STATE_EAT_BASE_RIGHT = 19,
	MURPHY_STATE_EAT_INFOTRON_TOP = 20,
	MURPHY_STATE_EAT_INFOTRON_LEFT = 21,
	MURPHY_STATE_EAT_INFOTRON_BOTTOM = 22,
	MURPHY_STATE_EAT_INFOTRON_RIGHT = 23,
	MURPHY_STATE_TELEPORT_UP = 24,
	MURPHY_STATE_TELEPORT_LEFT = 25,
	MURPHY_STATE_TELEPORT_DOWN = 26,
	MURPHY_STATE_TELEPORT_RIGHT = 27,
	MURPHY_STATE_MOVE_UP_EAT_BOMB = 28,
	MURPHY_STATE_MOVE_LEFT_EAT_BOMB = 29,
	MURPHY_STATE_MOVE_DOWN_EAT_BOMB = 30,
	MURPHY_STATE_MOVE_RIGHT_EAT_BOMB = 31,
	MURPHY_STATE_EAT_BOMB_TOP = 32,
	MURPHY_STATE_EAT_BOMB_LEFT = 33,
	MURPHY_STATE_EAT_BOMB_BOTTOM = 34,
	MURPHY_STATE_EAT_BOMB_RIGHT = 35,
	MURPHY_STATE_MOVE_YELLOW_DISK_UP = 36,
	MURPHY_STATE_MOVE_YELLOW_DISK_LF = 37,
	MURPHY_STATE_MOVE_YELLOW_DISK_RT = 38,
	MURPHY_STATE_MOVE_YELLOW_DISK_DN = 39,
	MURPHY_STATE_SHIFT_ORANGE_DISK_LF = 40,
	MURPHY_STATE_SHIFT_ORANGE_DISK_RT = 41,
	MURPHY_STATE_PUT_BOMB = 42,
};

enum PlayerAnimation
{
	ANIM_NONE = -1,
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

////////////////////////////////////////////////////////
// JOYSTICK BUTTON CODE

enum JoystickButton
{
	JB_NONE = 0,
	JB_UP = 1,
	JB_LEFT = 2,
	JB_DOWN = 3,
	JB_RIGHT = 4,
	JB_SPACE_UP = 5,
	JB_SPACE_LEFT = 6,
	JB_SPACE_DOWN = 7,
	JB_SPACE_RIGHT = 8,
	JB_SPACE = 9,
};	

////////////////////////////////////////////////////////
// DESCRIPTION OF THE GRAVITY SWITCH PORT

struct GravitySwitchPort
{
	word mapOffset;		/**< Offset of the port from beginning of the map. */
	byte gravity;		/**< 1 (turn on) or 0 (turn off) gravity. */
	byte freezeZonks;	/**< 2 (turn on) or 0 (turn off) freeze zonks. */
	byte freezeEnemies;	/**< 1 (turn on) or 0 (turn 0ff) freeze enemies. */
	byte unused;		/**< Not used. */
};

////////////////////////////////////////////////////////
// LEVEL DATA FORMAT

struct LevelData
{
	byte map[MAP_WIDTH * MAP_HEIGHT];	/**< Game map. */
	uint32 unused;						/**< Not used. */
	byte gravity;						/**< Gravity start value (0=off, 1=on). */
	byte version;						/**< Level data version. */
	char title[23];						/**< Level title. */
	byte freezeZonks;					/**< Freeze zonks start value (0=off, 2=on). */
	byte infotronsNeeded;				/**< Number of infotrons needed. */
	byte numberOfGravitySwitchPorts;	/**< Number of gravity switch ports (max 10). */
	GravitySwitchPort ports[10];		/**< Description of gravity switch ports. */
	uint32 unused2;						/**< Not used. */
};

/** Map item. */
union MapItem
{
	uint16		full;		/**< Full item data. */
	struct
	{
		uint8	id;			/**< Item ID. */
		uint8	state;		/**< Item state. */
	};
};

////////////////////////////////////////////////////////
// GAME STATE

struct GameState
{
	LevelData m_LevelData;						/**< Original level data. */
	MapItem m_GameMap[MAP_WIDTH * MAP_HEIGHT];	/**< Current state of the game. */
	byte m_Explosions[MAP_WIDTH * MAP_HEIGHT];	/**< Explosions state. */
	dword m_PlayerMapOffset;					/**< Offset of the player on the map. */
	dword m_PlayerPrevMapOffset;				/**< Offset of the player on the map in the previous game frame. */
 	bool m_PlayerIsLookingLeft;					/**< 1 if player is looking to the left, 0 if to the right. */
	bool m_PlayerDied;							/**< 1 if player has died, 0 if not. */
	bool m_PlayerExploded;						/**< 1 if player has exploded, 0 if not. */
	bool m_PlayerCompletedLevel;				/**< 1 if player has completed the level, 0 if not. */
	int m_PlayerPositionPixelsX;				/**< Current X coordinate of the player. */
	int m_PlayerPositionPixelsY;				/**< Current Y coordinate of the player. */
	word m_TimeoutToLeaveTheLevel;				/**< Number of ticks after which the level should be exited. */
	byte m_NumRedDisks;							/**< Number of red disks a player have. */
	byte m_RedDiskDetonateTimer;
	uint32 m_RedDiskPosition;
	word m_RedDiskPlacementTimer;
	bool m_TerminalsActivated;					/**< 1 if player has activated a terminal. */
	bool m_Gravity;								/**< 1 if gravity is turned on. */
	bool m_FreezeZonks;							/**< 2 if zonks are frozen. */
	bool m_FreezeEnemies;						/**< 1 if enemies are frozen. */
	word m_CurrentFrameNumber;					/**< Current game frame number. */
	byte m_InfotronsLeftToCollect;				/**< Number of infotrons left to complete the level. */
	byte m_TotalInfotronsInTheLevel;			/**< Total number of infotrons in the level. */
	byte m_PlayerIsInsideTeleport;
	PlayerAnimation m_PlayerAnim_ID;
	int m_PlayerAnim_StepX;
	int m_PlayerAnim_StepY;
	uint m_PlayerAnim_NumFrames;
	bool byte_403C3;
};

#endif // __SUPAPLEX_DEF_H__
