#ifndef __ENGINE__CASUAL_GAME_H__
#define __ENGINE__CASUAL_GAME_H__

/* Includes */

#include <core/common.h>


/* Macros */

#ifdef WIN32

 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>

 #define GAME_MAIN(class) \
 	static CasualGame * constructor() \
 	{ \
 		 return new class; \
 	} \
	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) \
	{ \
		char * argv[] = { (char*)"", NULL }; \
		return CasualGame::main(1, argv, constructor); \
	}

#else // WIN32

 #define GAME_MAIN(class) \
 	static CasualGame * constructor() \
 	{ \
 		 return new class; \
 	} \
	int main(int argc, char ** argv) \
	{ \
		return CasualGame::main(argc, argv, constructor); \
	}

#endif // WIN32


/* Classes */

/** Base class for casual games. */
class CasualGame
{
public:
	/** Constructor. */
	CasualGame();

	/** Destructor. */
	virtual ~CasualGame();

	/** Renders a single frame. */
	virtual void renderFrame() = 0;

	/** Renders the GUI. */
	virtual void renderGUI() = 0;

	/** Runs the AI. */
	virtual void runAI() = 0;

	/** Handles the player's input. */
	virtual void runPlayer() = 0;

	/**
	 * Executes the game.
	 * @param argc Number of command-line arguments.
	 * @param argv Array of command-line arguments.
	 * @param constructor Pointer to the constructor of the game class.
	 */
	static int main(int argc, char ** argv, CasualGame * (* constructor)(void));

private:
	/** Displays the startup dialog. */
	void startupDialog();
};

#endif // __ENGINE__CASUAL_GAME_H__
