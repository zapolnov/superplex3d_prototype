#ifndef __LINES_GAME_H__
#define __LINES_GAME_H__

/* Includes */

#include <core/common.h>
#include <math/camera.h>
#include <engine/casual_game.h>
#include <engine/render.h>


/* Constants */

#define GAME_FIELD_WIDTH	9
#define GAME_FIELD_HEIGHT	9

#define MIN_LINE_LENGTH		5		// Minimum number of balls required to build a line

#define NUM_BUTTONS			3		// Number of buttons in the menu


/* Classes */

class Ball;

/** Game field. */
typedef Ball * GameField[GAME_FIELD_WIDTH][GAME_FIELD_HEIGHT];

/** The lines game. */
class LinesGame : public CasualGame
{
public:
	/** Constructor. */
	LinesGame();

	/** Destructor. */
	~LinesGame();

	/** Renders a single frame. */
	void renderFrame();

	/** Renders the GUI. */
	void renderGUI();

	/** Runs the AI. */
	void runAI();

	/** Handles the player's input. */
	void runPlayer();

private:
	/** Game state. */
	enum GameState
	{
		PlayerInput,							/**< Waiting for player input. */
		MovingBall,								/**< We're currently moving the ball. */
		CheckCompleteLines,						/**< We're checking, whether complete line has been built. */
		BallsFalling,							/**< Balls are falling. */
		GameOver,								/**< Game is over. */
	};

	GameState m_State;							/**< Current state of the game. */
	CameraPtr m_Camera;							/**< Pointer to the camera. */
	GameField m_GameField;						/**< Game field. */
	Ball * m_SelectedBall;						/**< Pointer to the selected ball. */
	QPoint m_SelectedBallPos;					/**< Coordinates of the selected ball. */
	Vector3 m_MousePos;							/**< Current 3D position of the mouse. */
	std::vector<QPoint> m_BallPath;				/**< Path of the current ball. */
	double m_LastBallMoveTime;					/**< Last time when the ball was moved. */
	FontPtr m_LargeFont;						/**< Pointer to the font. */
	FontPtr m_MenuFont;							/**< Pointer to the font for the menu. */
	TexturePtr m_FieldTexture;					/**< Texture for the gaming field. */
	Ball * m_NextBalls[3];						/**< Balls that will appear next. */
	long long m_Score;							/**< Game score. */
	TexturePtr m_Button;						/**< Button texture. */
	TexturePtr m_ButtonPressed;					/**< Texture of a pressed button. */
	TexturePtr m_GameOver;						/**< Game over image. */
	QSize m_GameOverSize;						/**< Size of the 'game over' image. */
	QSize m_ButtonSize;							/**< Dimensions of the button. */
	std::wstring m_ScoreStr;					/**< String for user's score display. */
	std::wstring m_ButtonTitles[NUM_BUTTONS];	/**< Titles of the menu buttons. */
	int m_ButtonClicked;						/**< Index of a button that is currently clicked. */
	QSize m_ButtonTitleSizes[NUM_BUTTONS];		/**< Sizes of the button titles. */
	bool m_StartNewGameDialogShown : 1;			/**< If true, we're showing a 'Start a new game' dialog. */
	bool m_AboutDialogShown : 1;				/**< If true, we're showing an about dialog. */
	bool m_SettingsDialogShown : 1;				/**< If true, we're showing a settings dialog. */
	std::wstring m_StartNewGameMsg;				/**< New game confirmation string. */
	std::wstring m_YesMsg;						/**< 'Yes' message. */
	std::wstring m_NoMsg;						/**< 'No' message. */
	std::wstring m_OkMsg;						/**< 'OK' message. */
	std::wstring m_CancelMsg;					/**< 'Cancel' message. */
	QSize m_StartNewGameMsgSize;				/**< Dimensions of the new game confirmation string. */
	QSize m_YesMsgSize;							/**< Dimensions of the 'Yes' message. */
	QSize m_NoMsgSize;							/**< Dimensions of the 'No' message. */
	QSize m_OkMsgSize;							/**< Dimensions of the 'OK' message. */
	QSize m_CancelMsgSize;						/**< Dimensions of the 'Cancel' message. */
	std::wstring m_AboutMsg[5];					/**< About strings. */
	QSize m_AboutMsgSize[5];					/**< Sizes of about strings. */
	std::wstring m_NextMsg;						/**< 'Next:' message. */
	QSize m_NextMsgSize;						/**< Size of the 'Next:' message. */

	/** Starts a new game. */
	void startGame();

	/** Spawns three balls in random fields of the map. */
	void spawnThreeBalls();

	/** Returns true if there is at least one free cell on the game field. */
	bool gameStillPossible();

	/**
	 * Sets the game state.
	 * @param state New game state.
	 */
	void setGameState(GameState state);

	/** Generates three random balls. */
	void generateNextBalls();

	/**
	 * Clears all complete lines on the field.
	 * @returns true If at least one line has been removed.
	 */
	bool clearAllLines();

	/**
	 * Counts number of similar balls in a line.
	 * @param startX X coordinate of the starting cell.
	 * @param startY Y coordinate of the starting cell.
	 * @param stepX X step.
	 * @param stepY Y step.
	 * @param colorIndex Index of the ball color to search for.
	 * @returns Number of similar balls in a line.
	 */
	int countSimilarBalls(int startX, int startY, int stepX, int stepY, int colorIndex);

	/**
	 * Clears a line of balls.
	 * @param startX X coordinate of the starting cell.
	 * @param startY Y coordinate of the starting cell.
	 * @param stepX X step.
	 * @param stepY Y step.
	 * @param count Number of balls to remove in the given direction.
	 */
	void clearLine(int startX, int startY, int stepX, int stepY, int count);

	/**
	 * Draws button at the specified position on the screen.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @param text Button text.
	 * @param textSize Size of the text.
	 * @param pressed If true, button is pressed.
	 */
	void drawButton(float x, float y, const std::wstring & text, const QSize & textSize, bool pressed);

	/**
	 * Called when user clicks a button.
	 * @param idx Index of a button.
	 */
	void buttonClicked(int idx);

	/**
	 * Paints the 'Start a new game' dialog.
	 * @param screenWidth Width of the screen.
	 * @param screenHeight Height of the screen.
	 */
	void paintStartNewGameDialog(int screenWidth, int screenHeight);

	/**
	 * Handles input for the 'Start a new game' dialog.
     * @param mousePos2D Mouse coordinates.
	 * @param screenWidth Width of the screen.
	 * @param screenHeight Height of the screen.
	 */
	void runStartNewGameDialog(const QPoint & mousePos2D, int screenWidth, int screenHeight);

	/**
	 * Paints the 'About' dialog.
	 * @param screenWidth Width of the screen.
	 * @param screenHeight Height of the screen.
	 */
	void paintAboutDialog(int screenWidth, int screenHeight);

	/**
	 * Handles input for the 'About' dialog.
     * @param mousePos2D Mouse coordinates.
	 * @param screenWidth Width of the screen.
	 * @param screenHeight Height of the screen.
	 */
	void runAboutDialog(const QPoint & mousePos2D, int screenWidth, int screenHeight);

	/**
	 * Paints the 'Settings' dialog.
	 * @param screenWidth Width of the screen.
	 * @param screenHeight Height of the screen.
	 */
	void paintSettingsDialog(int screenWidth, int screenHeight);

	/**
	 * Handles input for the 'Settings' dialog.
     * @param mousePos2D Mouse coordinates.
	 * @param screenWidth Width of the screen.
	 * @param screenHeight Height of the screen.
	 */
	void runSettingsDialog(const QPoint & mousePos2D, int screenWidth, int screenHeight);
};

#endif // __LINES_GAME_H__
