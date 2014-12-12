#ifndef __INPUT_H__
#define __INPUT_H__

/* Includes */

#include <core/common.h>
#include <math/vector2.h>
#include <math/vector3.h>
#include <math/camera.h>


/* Constants */

/** Keyboard buttons */
enum KeyboardButton
{
	KeyUp = 0,			/**< Up arrow. */
	KeyDown,			/**< Down arrow. */
	KeyLeft,			/**< Left arrow. */
	KeyRight,			/**< Right arrow. */
	KeyEsc,				/**< Escape. */
	KeySpace,			/**< Space bar. */
};

/** Mouse buttons */
enum MouseButton
{
	LeftButton = 0,		/**< Left button. */
	RightButton,		/**< Right button. */
	MiddleButton		/**< Middle button. */
};


/* Functions. */

/** Initializes the input manager. */
void IN_Init();

/** Shuts down the input manager. */
void IN_Shutdown();

/** Polls the input devices. */
void IN_Poll();

/** Returns current 2D position of the mouse. */
extern "C++" ENGINEAPI QPoint IN_GetMousePos();

/**
 * Returns true if the specified mouse button has been clicked in this frame.
 * @param button Mouse button to check.
 */
ENGINEAPI bool IN_IsMouseClicked(MouseButton button);

/**
 * Returns true if the specified mouse button has been released in this frame.
 * @param button Mouse button to check.
 */
ENGINEAPI bool IN_IsMouseReleased(MouseButton button);

/**
 * Returns true if the specified key is currently pressed.
 * @param key Key code.
 */
ENGINEAPI bool IN_IsKeyPressed(KeyboardButton key);

#endif // __INPUT_H__
