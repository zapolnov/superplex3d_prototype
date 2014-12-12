#ifndef __TERRAIN__PATCH_H__
#define __TERRAIN__PATCH_H__

/* Includes */

#include <core/common.h>
#include "patch_base.h"


/* Classes */

class Terrain;

class TerrainPatch : public TerrainPatchBase
{
public:
	/**
	 * Constructor.
	 * @param master Pointer to the terrain that owns this patch.
	 * @param gridX
	 * @param gridY
	 */
	TerrainPatch(Terrain * master, int gridX, int gridY);


	virtual void init();

	virtual void deinit();

	virtual int render();

	virtual bool updateTessellation();

	virtual bool updateTessellation2();

	virtual bool updateTessellation3();

	virtual void deleteDeviceObjects();

	virtual bool restoreDeviceObjects();

protected:
//NZ	PDIRECT3DINDEXBUFFER8	iBuffer;
//NZ	PDIRECT3DVERTEXBUFFER8	vBuffer;


	bool checkBorders();
	bool fillBuffers();
	bool checkBuffers();
};


#endif //__TERRAINPATCH_HEADERFILE__
