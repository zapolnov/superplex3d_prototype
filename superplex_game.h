#ifndef __TANKS_GAME_H__
#define __TANKS_GAME_H__

/* Includes */

#include <core/common.h>
#include <math/camera.h>
#include <engine/casual_game.h>
#include "supaplex_game.h"


/* Classes */

/** The superplex3d game. */
class SuperplexGame : public CasualGame
{
public:
	/** Constructor. */
	SuperplexGame();

	/** Destructor. */
	~SuperplexGame();

	/** Renders a single frame. */
	void renderFrame();

	/** Renders the GUI. */
	void renderGUI();

	/** Runs the AI. */
	void runAI();

	/** Handles the player's input. */
	void runPlayer();

private:
	CameraPtr m_Camera;					/**< Pointer to the camera. */
	std::vector<MeshPtr> m_Meshes;		/**< Array of meshes. */
	std::vector<MeshPtr> m_Infotron;	/**< Infotron meshes. */
	std::vector<MeshPtr> m_Murphy;		/**< Murphy meshes. */
	std::vector<MeshPtr> m_SnikSnak;	/**< Snik-snak meshes. */
	MeshPtr m_Star;						/**< Star. */
	MeshPtr m_TerminalFront;			/**< Front face of the terminal. */
	ShaderPtr m_TerminalPixelShader;	/**< Pixel shader for the terminal. */
	LightningPtr m_Lightning;			/**< Pointer to the lightning. */
	SupaplexLogic m_Game;				/**< Game logic. */
	TexturePtr m_Boom;					/**< Boom texture. */
	double m_PrevFrameTime;				/**< Time when previous frame has been executed. */
	bool m_SkipThisFrame;				/**< If true, this frame should be skipped. */
	double m_InfotronFrame;				/**< Current frame of the infotron animation. */
	double m_InfotronAngle;				/**< Current rotation angle of the infotron. */

	/** Animates the infotron. */
	void animateInfotron();

	/** Renders the infotron. */
	void renderInfotron();

	/** Animates the electron. */
	void animateElectron();

	/** Renders the electron. */
	void renderElectron();
};

#endif // __TANKS_GAME_H__
