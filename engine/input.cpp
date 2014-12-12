#include <engine/input.h>
#include <GL/glfw.h>

/* Private variables */

static int g_MouseX;				/**< X coordinate of the mouse. */
static int g_MouseY;				/**< Y coordinate of the mouse. */

static bool g_ButtonState[3];		/**< Current state of mouse buttons. */
static bool g_OldButtonState[3];	/**< State of mouse buttons in previous frame. */

static int g_Keys[] =				/**< Engine to GLFW key mapping. */
{
	GLFW_KEY_UP,
	GLFW_KEY_DOWN,
	GLFW_KEY_LEFT,
	GLFW_KEY_RIGHT,
	GLFW_KEY_ESC,
	GLFW_KEY_SPACE,
};


/* Public functions */

//
// Initializes the input manager
//
void IN_Init()
{
}

//
// Shuts down the input manager
//
void IN_Shutdown()
{
}

//
// Polls the input devices
//
void IN_Poll()
{
	memcpy(g_OldButtonState, g_ButtonState, sizeof(g_ButtonState));

	glfwGetMousePos(&g_MouseX, &g_MouseY);

	g_ButtonState[0] = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	g_ButtonState[1] = glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	g_ButtonState[2] = glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
}

//
// Returns current 2D position of the mouse
//
QPoint IN_GetMousePos()
{
	return QPoint(g_MouseX, g_MouseY);
}

//
// Returns true if the specified mouse button has been clicked in this frame
//
bool IN_IsMouseClicked(MouseButton button)
{
	return (g_OldButtonState[button] != g_ButtonState[button] && g_ButtonState[button]);
}

//
// Returns true if the specified mouse button has been released in this frame
//
bool IN_IsMouseReleased(MouseButton button)
{
	return (g_OldButtonState[button] != g_ButtonState[button] && !g_ButtonState[button]);
}

//
// Returns true if the specified key is currently pressed
//
bool IN_IsKeyPressed(KeyboardButton key)
{
	Q_ASSERT(key >= 0 && key < sizeof(g_Keys) / sizeof(g_Keys[0]));
	return glfwGetKey(g_Keys[key]) == GLFW_PRESS;
}
