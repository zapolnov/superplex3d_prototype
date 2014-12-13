#ifndef __SUPAPLEX_GAME_H__
#define __SUPAPLEX_GAME_H__

/* Includes */

#include <core/common.h>
#include <math/vector2.h>
#include "supaplex_def.h"


/* Classes */

/** Supaplex game logic. */
class SupaplexLogic : public GameState
{
public:
	/** Constructor. */
	SupaplexLogic();

	/** Destructor. */
	~SupaplexLogic();

	/**
	 * Starts new game.
	 * @param index Index of the level.
	 */
	void startGame(int index);

	/**
	 * Handles movement of the player.
	 * @param button Joystick button code.
	 */
	void handlePlayerMovement(JoystickButton button);

	/** Runs AI and physics. */
	void runAI();

	/**
	 * Returns item at the specified position of the map.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 */
	inline MapItem getItem(int x, int y) const
	{
		Q_ASSERT(x >= 0 && x < MAP_WIDTH);
		Q_ASSERT(y >= 0 && y < MAP_HEIGHT);

		return m_GameMap[y * MAP_WIDTH + x];
	}

	/**
	 * Returns real (not simplified) item at the specified position of the map.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 */
	inline byte getRealItem(int x, int y) const
	{
		Q_ASSERT(x >= 0 && x < MAP_WIDTH);
		Q_ASSERT(y >= 0 && y < MAP_HEIGHT);

		return m_LevelData.map[y * MAP_WIDTH + x];
	}

	/** Returns current coordinates of the player on the map. */
	inline Vector2 getPlayerCoords() const
	{
		return Vector2((float)m_PlayerPositionPixelsX / 16.0f, (float)m_PlayerPositionPixelsY / 16.0f);
	}

	/** Returns true if player is dead. */
	inline bool isPlayerDead() const
	{
		return m_PlayerExploded || m_PlayerDied || m_LevelComplete;
	}

	/** Returns true if level has been completed. */
	inline bool isLevelComplete() const
	{
		return m_LevelComplete;
	}

	/** Returns number of animation frames left. */
	inline unsigned animFramesLeft() const
	{
		return m_PlayerAnim_NumFrames;
	}

	/** Returns position of the exit relative to the position of the player. */
	Vector2 getExitRelativePos() const;

	/** Returns type of the teleport the player is currently passing through. */
	inline byte getTeleportType() const
	{
		return m_PlayerIsInsideTeleport;
	}

	/** Returns current in-game frame. */
	inline uint getCurrentFrameNumber() const
	{
		return m_CurrentFrameNumber;
	}

	/** Returns true if player looks to the right. */
	inline bool playerLooksRight() const
	{
		return !m_PlayerIsLookingLeft;
	}

	/** Returns value of the red disk detonate timer. */
	inline uint getRedDiskDetonateTimer() const
	{
		return m_RedDiskDetonateTimer;
	}

	/** Returns value of the red disk placement timer. */
	inline uint getRedDiskPlacementTimer() const
	{
		return m_RedDiskPlacementTimer;
	}

	/** Returns position of the red disk. */
	inline Vector2 getRedDiskPosition() const
	{
		return Vector2(
			(float)(m_RedDiskPosition % MAP_WIDTH),
			(float)(m_RedDiskPosition / MAP_WIDTH)
		);
	}

	/** Kills the player. */
	inline void killPlayer()
	{
		if (!isPlayerDead())
			createExplosion(m_PlayerMapOffset);
	}

private:
	bool m_LevelComplete;		/**< If true, level has been completed. */

	/**
	 * Loads the specified level.
	 * @param index Index of the level.
	 */
	void loadLevel(int index);

	/** Resets current state of the player. */
	void resetPlayerState();

	/** Calculates initial position of the player. */
	void calcPlayerInitialPosition();

	/** Setup the level map. */
	void setupLevelMap();

	/**
	 * Called when player enters the gravity switch port.
	 * @param offset Offset of the port on the map.
	 */
	void handleGravityPort(uint32 offset);

	/** Moves player one cell up. */
	bool movePlayerUp();

	/** Moves player one cell down. */
	bool movePlayerDown();

	/** Moves player one cell left. */
	bool movePlayerLeft();

	/** Moves player one cell right. */
	bool movePlayerRight();

	/** Eats item at left of the player. */
	bool eatAtLeft();

	/** Eats item at right of the player. */
	bool eatAtRight();

	/** Eats item above the player. */
	bool eatAbove();

	/** Eats item below the player. */
	bool eatBelow();

	/**
	 * Called when user activates the exit.
	 * @param offset Offset on the map.
	 */
	bool handleLevelExit(uint32 offset);

	/** Called when user activates the terminal. */
	void handleTerminal();

	/**
	 * Checks whether player have due to it's movement
	 * @param offset Offset of the target position on the map.
	 * @param item Map item at the target position.
	 * @param button Joystick button 
	 */
	bool checkPlayerDied(uint32 offset, MapItem item, JoystickButton button);

	/**
	 * Sets current animation for the player.
	 * @param anim Player animation ID.
	 * @param numFrames Number of frames in the animation.
	 * @param stepX X step.
	 * @param stepY Y step.
	 */
	void setPlayerAnimation(PlayerAnimation animID, uint numFrames, int stepX, int stepY);

	/**
	 * Creates explosion at the specified position.
	 * @param offset Center of the explosion.
	 */
	void createExplosion(uint32 offset);

	/** Activates chained explosions. */
	void createChainedExplosions();

	void sub_398F9(uint32 offset, MapItem item, word cx);

	void sub_39969(uint32 offset, MapItem item, word cx);

	/**
	 * Cancels any animation at the specified position.
	 * @param offset Offset on the map.
	 */
	void cancelAnimation(uint32 offset);

	/** Updates state of zonks and enemies. */
	void updateZonksAndEnemies();

	/** Updates status of any placed red disk. */
	void updateRedDisk();

	void sub_38191(uint32 offset);

	/**
	 * Updates state of the zonk.
	 * @param offset Position of the zonk on the map.
	 */
	void updateZonk(uint32 offset);

	/**
	 * Updates state of the infotron.
	 * @param offset Position of the infotron on the map.
	 */
	void updateInfotron(uint32 offset);

	/**
	 * Updates state of the boom.
	 * @param offset Position of the boom on the map.
	 */
	void updateBoom(uint32 offset);

	/**
	 * Updates state of the orange disk.
	 * @param offset Position of the orange disk on the map.
	 */
	void updateOrangeDisk(uint32 offset);

	/**
	 * Updates state of the bug.
	 * @param offset Position of the bug on the map.
	 */
	void updateBug(uint32 offset);

	/**
	 * Updates state of the terminal.
	 * @param offset Position of the terminal on the map.
	 */
	void updateTerminal(uint32 offset);

	/**
	 * Updates state of the snik-snak.
	 * @param offset Position of the snik-snak on the map.
	 */
	void updateSnikSnak(uint32 offset);

	/**
	 * Updates state of the snik-snak.
	 * @param offset Position of the snik-snak on the map.
	 */
	void snikSnakHandler_1(uint32 offset);

	/**
	 * Updates state of the snik-snak.
	 * @param offset Position of the snik-snak on the map.
	 */
	void snikSnakHandler_2(uint32 offset);

	/**
	 * Updates state of the snik-snak.
	 * @param offset Position of the snik-snak on the map.
	 */
	void snikSnakHandler_3(uint32 offset);

	/**
	 * Updates state of the snik-snak.
	 * @param offset Position of the snik-snak on the map.
	 */
	void snikSnakHandler_4(uint32 offset);

	/**
	 * Updates state of the snik-snak.
	 * @param offset Position of the snik-snak on the map.
	 */
	void snikSnakHandler_5(uint32 offset);

	/**
	 * Updates state of the snik-snak.
	 * @param offset Position of the snik-snak on the map.
	 */
	void snikSnakHandler_6(uint32 offset);

	/**
	 * Updates state of the electron.
	 * @param offset Position of the electron on the map.
	 */
	void updateElectron(uint32 offset);

	/**
	 * Updates state of the electron.
	 * @param offset Position of the electron on the map.
	 */
	void electronHandler_1(uint32 offset);

	/**
	 * Updates state of the electron.
	 * @param offset Position of the electron on the map.
	 */
	void electronHandler_2(uint32 offset);

	/**
	 * Updates state of the electron.
	 * @param offset Position of the electron on the map.
	 */
	void electronHandler_3(uint32 offset);

	/**
	 * Updates state of the electron.
	 * @param offset Position of the electron on the map.
	 */
	void electronHandler_4(uint32 offset);

	/**
	 * Updates state of the electron.
	 * @param offset Position of the electron on the map.
	 */
	void electronHandler_5(uint32 offset);

	/**
	 * Updates state of the electron.
	 * @param offset Position of the electron on the map.
	 */
	void electronHandler_6(uint32 offset);

	void sub_3826F(uint32 offset, byte baseMapItem, byte secondMapItem);

	void sub_3DB7E(uint32 offset);
};

#endif // __SUPAPLEX_GAME_H__
