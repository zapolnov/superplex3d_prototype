#include <engine/render.h>
#include <engine/system.h>
#include <engine/r_opengl.h>
#include <core/logger.h>
#include <GL/glfw.h>
#include <limits>

/* Private functions */

//
// Unprojects the point
//
static
Vector3 unproject(float winx, float winy, float winz, const QSize & viewport, const CameraPtr & camera)
{
	Matrix modelMatrix = camera->combinedMatrix(viewport);
	Matrix invModelMatrix;
	if (!invModelMatrix.invert(modelMatrix))
	{
		float nan = std::numeric_limits<float>::quiet_NaN();
		return Vector3(nan, nan, nan);
	}

	float in[4] = { winx, winy, winz, 1.0f };

	// Map x and y from window coordinates
	in[0] /= (float)viewport.width();
	in[1] /= (float)viewport.height();

	// Map to range -1 to 1
	in[0] = in[0] * 2.0f - 1.0f;
	in[1] = in[1] * 2.0f - 1.0f;
	in[2] = in[2] * 2.0f - 1.0f;

	float out[4];
	for (int i = 0; i < 4; i++)
	{
		out[i] =
			in[0] * invModelMatrix(0, i) +
			in[1] * invModelMatrix(1, i) +
			in[2] * invModelMatrix(2, i) +
			in[3] * invModelMatrix(3, i);
	}

	if (out[3] == 0.0)
	{
		float nan = std::numeric_limits<float>::quiet_NaN();
		return Vector3(nan, nan, nan);
	}

    out[0] /= out[3];
	out[1] /= out[3];
	out[2] /= out[3];

	return Vector3(out[0], out[1], out[2]);
}


/* Public functions */

//
// Initializes the renderer
//
void R_Init() throw(std::exception)
{
	if (unlikely(!glfwOpenWindow(640, 480, 8, 8, 8, 8, 24, 0, GLFW_WINDOW)))
		throw std::runtime_error("glfwOpenWindow() failed.");

	GL_GetInfo();
	GL_InitExtensions();
	GL_CollectLimits();
	logger << LOG_INFO << " ";

	R_SetClearColor(0.1f, 0.3f, 0.5f);

	glShadeModel(GL_SMOOTH);

	glfwSwapInterval(0);
}

//
// Shuts down the renderer
//
void R_Shutdown()
{
	R_ClearVertexShaderCache();
	R_ClearFragmentShaderCache();
	R_ClearTextureCache();
	R_ClearMeshCache();

	glfwCloseWindow();
}

//
// Sets the screen clearing color
//
void R_SetClearColor(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
}

//
// Begins rendering a frame
//
void R_BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//
// Ends rendering a frame
//
void R_EndFrame()
{
}

//
// Switches to the 2D rendering mode
//
void R_Set2D()
{
	int width, height;
	glfwGetWindowSize(&width, &height);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float)width, (float)height, 0, -100.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (ARB_multitexture)
	{
		for (uint i = 1; i < Q_MAX(MAX_TEXTURES, (uint)GL_MaxTextureUnits); i++)
		{
			pglActiveTexture(GL_TEXTURE0_ARB + i);
			if (EXT_texture3D)
				glDisable(GL_TEXTURE_3D);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_1D);
		}
		pglActiveTexture(GL_TEXTURE0_ARB);
	}

	if (EXT_texture3D)
		glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_1D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	R_UnbindVertexShader();
	R_UnbindFragmentShader();
}

//
// Switches to the 3D rendering mode
//
void R_Set3D(const CameraPtr & camera)
{
	QSize size;
	glfwGetWindowSize(&size.width(), &size.height());

	glViewport(0, 0, size.width(), size.height());

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera->projectionMatrix(size).ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->transformationMatrix().ptr());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//
// Swaps the front and back buffers
//
bool R_SwapBuffers()
{
	glfwSwapBuffers();
	return glfwGetWindowParam(GLFW_OPENED) != 0;
}

//
// Unprojects the specified 3D point into the 2D space
//
Vector3 R_UnprojectPoint(const CameraPtr & camera, const QPoint & p)
{
	int width, height;
	glfwGetWindowSize(&width, &height);
	QSize viewport(width, height);

	int mouseX = p.x();
	int mouseY = viewport.height() - p.y();

	float z = 0.0f;
	glReadPixels(mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

	return unproject((float)mouseX, (float)mouseY, z, viewport, camera);
}
