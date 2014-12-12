///////////////////////////////////////////////////////////////////
//
// project:     Pluto Game Engine
// class:       TerrainPatchMorphHW
// creator:     Daniel Wagner
// description: 
//
///////////////////////////////////////////////////////////////////
// $Id: TerrainPatchMorphHW.h,v 1.5 2002/12/08 16:36:23 daniel Exp $
///////////////////////////////////////////////////////////////////


#ifndef __TERRAIN__PATCH_MORPH_HW_H__
#define __TERRAIN__PATCH_MORPH_HW_H__

/* Includes */

#include <core/common.h>
#include "patch_morph_sw.h"


/* Classes */

class TerrainPatchMorphHW : public TerrainPatchMorphSW
{
public:
	TerrainPatchMorphHW(Terrain* nMaster, int nX, int nY);


	virtual void init();

	virtual void deinit();

	virtual int render();

	//virtual bool updateTessellation3();

	virtual void deleteDeviceObjects();

	virtual bool restoreDeviceObjects();

	static bool setupVertexShader();

//NZ	static bool setupTextureMatrix(D3DXMATRIX& nTexMatrix);

protected:
	static uint32	morphShader;

	//PDIRECT3DVERTEXBUFFER8	mBuffer;

	virtual bool fillBuffers();
	bool checkBuffers();
	bool checkShader();
};


#endif // __TERRAIN__PATCH_MORPH_HW_H__
