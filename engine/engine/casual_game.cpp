#include <engine/casual_game.h>
#include <engine/system.h>
#include <engine/render.h>
#include <engine/filesys.h>
#include <engine/input.h>
#include <sound/functions.h>
#include <core/logger.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

static void errorCallback(int, const char* description)
{
	logger << LOG_ERROR << "GLFW error: %s" << description;
}

/* CasualGame methods */

//
// Constructor
//
CasualGame::CasualGame()
{
}

//
// Destructor
//
CasualGame::~CasualGame()
{
}

//
// Executes the game
//
int CasualGame::main(int argc, char ** argv, CasualGame * (* constructor)(void))
{
	CasualGame * ptr = NULL;

	if (!glfwInit())
		return 1;
	glfwSetErrorCallback(errorCallback);

	try
	{
		Sys_Init();
		FS_Init();
		R_Init();
		IN_Init();
		SND_Init();

		ptr = constructor();
		ptr->startupDialog();

		do
		{
			Sys_UpdateTimer();
			IN_Poll();

			ptr->runPlayer();
			ptr->runAI();

			R_BeginFrame();
			ptr->renderFrame();
			R_Set2D();
			ptr->renderGUI();
			R_EndFrame();
		}
		while (R_SwapBuffers());

		SND_Shutdown();
		IN_Shutdown();
		R_Shutdown();
		FS_Shutdown();
		Sys_Shutdown();
	}
	catch (std::exception & e)
	{
		if (ptr)
			delete ptr;

		SND_Shutdown();
		R_Shutdown();

		logger << LOG_ERROR << "%s" << e.what();
		Sys_ErrorMessage(e.what());

		FS_Shutdown();
		Sys_Shutdown();

		glfwTerminate();

		return 255;
	}

	glfwTerminate();

	return 0;
}

//
// Displays the startup dialog
//
void CasualGame::startupDialog()
{
}
