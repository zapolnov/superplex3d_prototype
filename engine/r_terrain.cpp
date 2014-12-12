#include <engine/render.h>
#include "terrain/terrain.h"

/* Private variables */

static Terrain * g_Terrain = NULL;
static std::string g_HeightMapName;
static HeightMapRenderMode g_RenderMode = UnifiedRender;


/* Public functions */

//
// Loads the specified height map
//
void R_LoadHeightMap(const std::string & map)
{
	Q_ASSERT(map.length() > 0);

	R_FreeHeightMap();

	g_HeightMapName = map;
	g_Terrain = new Terrain(g_RenderMode);
}

//
// Destroys any currently loaded height map
//
void R_FreeHeightMap()
{
	g_HeightMapName.clear();
	delete g_Terrain;
	g_Terrain = NULL;
}

/**
 * Renders the height map.
 * @param camera Position of the camera.
 */
void R_RenderHeightMap(const CameraPtr & camera)
{
	Q_ASSERT(g_Terrain);
	g_Terrain->render();
}

//
// Sets the rendering mode of the terrain
//
void R_SetHeightMapRenderMode(HeightMapRenderMode mode)
{
	if (mode != g_RenderMode)
	{
		g_RenderMode = mode;
		if (g_HeightMapName.length() > 0)
			R_LoadHeightMap(g_HeightMapName);
	}	
}
