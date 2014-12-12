///////////////////////////////////////////////////////////////////
//
// project:     Pluto Game Engine
// class:       TerrainPatchMorphSW
// creator:     Daniel Wagner
// description: 
//
///////////////////////////////////////////////////////////////////
// $Id: TerrainPatchMorphSW.h,v 1.9 2002/11/14 18:04:04 daniel Exp $
///////////////////////////////////////////////////////////////////


#ifndef __TERRAIN__PATCH_MORPH_SW_H__
#define __TERRAIN__PATCH_MORPH_SW_H__

/* Includes */

#include <core/common.h>
#include "patch_base.h"


/* Classes */

class TerrainPatchMorphSW : public TerrainPatchBase
{
public:
	TerrainPatchMorphSW(Terrain* nMaster, int nX, int nY);


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

	PatchVertex *verticesLow;
	float					*yMoveSelf, *yMoveLeft,*yMoveLeft2, *yMoveRight,*yMoveRight2, *yMoveBottom,*yMoveBottom2, *yMoveTop,*yMoveTop2;

	float					factorSelf, factorLeft,factorLeft2, factorRight,factorRight2, factorBottom,factorBottom2, factorTop,factorTop2;

	bool					newFollowsLeft,followsLeft, newFollowsRight,followsRight, newFollowsBottom,followsBottom, newFollowsTop,followsTop;
	bool					recalcBorderLeft, recalcBorderRight, recalcBorderBottom, recalcBorderTop;

	bool					forceRetessellation;

	virtual bool fillBuffers();
	bool checkBuffers();
	void fillMorphVertices();
	void checkBorderLeft();
	void checkBorderRight();
	void checkBorderBottom();
	void checkBorderTop();
};


#endif // __TERRAIN__PATCH_MORPH_SW_H__
