#include "patch.h"
#include "terrain.h"

/* TerrainPatch methods */

//
// Constructor
//
TerrainPatch::TerrainPatch(Terrain * master, int gridX, int gridY)
	: TerrainPatchBase(master, gridX, gridY)
{
	/*NZ
	vBuffer = NULL;
	iBuffer = NULL;
	*/
}


void
TerrainPatch::init()
{
	TerrainPatchBase::init();
}


void
TerrainPatch::deinit()
{
	TerrainPatchBase::deinit();
	deleteDeviceObjects();
}


void
TerrainPatch::deleteDeviceObjects()
{
	/*NZ
	SAFE_RELEASE(iBuffer);
	SAFE_RELEASE(vBuffer);
	*/
}


bool
TerrainPatch::restoreDeviceObjects()
{
	m_ForceBufferCreate = true;
	return true;
}


bool
TerrainPatch::updateTessellation()
{
	if(TerrainPatchBase::updateTessellation())
		m_ForceBufferCreate = true;

	return true;
}


bool
TerrainPatch::updateTessellation2()
{
	TerrainPatch* left = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Left));
	TerrainPatch* right = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Right));
	TerrainPatch* bottom = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Bottom));
	TerrainPatch* top = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Top));

	if(m_GridX==7 && m_GridY==5)
		m_GridX=7;

	if(m_ForceBufferCreate || (left && left->m_ForceBufferCreate)
						 || (right && right->m_ForceBufferCreate)
						 || (bottom && bottom->m_ForceBufferCreate)
						 || (top && top->m_ForceBufferCreate))
	{
		bool updated = checkBorders();

		if(m_ForceBufferCreate || updated)
		{
			bool fOld = m_ForceBufferCreate;
			m_ForceBufferCreate = false;
			fillBuffers();
			m_ForceBufferCreate = fOld;
		}
	}

	return true;
}


bool
TerrainPatch::updateTessellation3()
{
	m_ForceBufferCreate = false;
	return true;
}


int
TerrainPatch::render()
{
	/*NZ
	if(!checkDevice("cannot render terrainpatch: no device set"))
		return 0;

	if(!isVisible())
		return 0;

	if(iBuffer==NULL || vBuffer==NULL)			// nothing to do ?
		return 0;


	pd3dDevice->SetVertexShader(D3DFVF_PATCHVERTEX);
	pd3dDevice->SetStreamSource(0, vBuffer, sizeof(PATCHVERTEX));
	pd3dDevice->SetIndices(iBuffer, 0);


	// rendering is VERY fast: just a single
	// DrawIndexedPrimitive call for each patch
	//
	pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, numVertices, 0, numIndices-2);
	*/
	return m_NumIndices-2;
}


bool
TerrainPatch::checkBorders()
{
	// border correction is only necessary if the neighboring patch
	// looks coarser than it is (m_RealSelfTesselation>m_SelfTesselation)

	TerrainPatch* left = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Left));
	TerrainPatch* right = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Right));
	TerrainPatch* bottom = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Bottom));
	TerrainPatch* top = const_cast<TerrainPatch *>(safe_cast<const TerrainPatch*>(m_Top));
	bool			ret = false;

	if(left && left->isActive() && (left->m_SelfTesselation!=left->m_RealSelfTesselation ||
		left->m_RealSelfTesselation!=left->m_OldRealSelfTesselation))
	{
		int y,pow = getPowerTwo(Q_MIN(m_LeftTesselation,m_SelfTesselation));

		for(y=pow; y<SOURCE_HEIGHT-1; y+=pow)
		{
			int	idx = m_IndexMap[0 + y*SOURCE_WIDTH],
				idxL = left->m_IndexMap[SOURCE_WIDTH-1 + y*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxL!=0xffff);

			m_Vertices[idx].pos.y() = left->m_Vertices[idxL].pos.y();
		}

		ret = true;
	}

	if(right && right->isActive() && (right->m_SelfTesselation!=right->m_RealSelfTesselation || right->m_RealSelfTesselation!=right->m_OldRealSelfTesselation))
	{
		int y,pow = getPowerTwo(Q_MIN(m_RightTesselation,m_SelfTesselation));

		for(y=pow; y<SOURCE_HEIGHT-1; y+=pow)
		{
			int	idx = m_IndexMap[SOURCE_WIDTH-1 + y*SOURCE_WIDTH],
				idxR = right->m_IndexMap[0 + y*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxR!=0xffff);

			m_Vertices[idx].pos.y() = right->m_Vertices[idxR].pos.y();
		}

		ret = true;
	}

	if(bottom && bottom->isActive() && (bottom->m_SelfTesselation!=bottom->m_RealSelfTesselation || bottom->m_RealSelfTesselation!=bottom->m_OldRealSelfTesselation))
	{
		int x,pow = getPowerTwo(Q_MIN(m_BottomTesselation,m_SelfTesselation));

		for(x=pow; x<SOURCE_WIDTH-1; x+=pow)
		{
			int	idx = m_IndexMap[x + 0*SOURCE_WIDTH],
				idxB = bottom->m_IndexMap[x + (SOURCE_HEIGHT-1)*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxB!=0xffff);

			m_Vertices[idx].pos.y() = bottom->m_Vertices[idxB].pos.y();
		}

		ret = true;
	}

	if(top && top->isActive() && (top->m_SelfTesselation!=top->m_RealSelfTesselation ||
		top->m_RealSelfTesselation!=top->m_OldRealSelfTesselation))
	{
		int x,pow = getPowerTwo(Q_MIN(m_TopTesselation,m_SelfTesselation));

		for(x=pow; x<SOURCE_HEIGHT-1; x+=pow)
		{
			int	idx = m_IndexMap[x + (SOURCE_HEIGHT-1)*SOURCE_WIDTH],
				idxT = top->m_IndexMap[x + 0*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxT!=0xffff);

			m_Vertices[idx].pos.y() = top->m_Vertices[idxT].pos.y();
		}

		ret = true;
	}

	return ret;
}


bool
TerrainPatch::fillBuffers()
{
	/*NZ
	if(!checkDevice("cannot create terrainpatch: no device set"))
		return false;

	if(!checkBuffers())
		return false;

	if(numVertices==0 || numIndices==0)					// nothing to do ???
		return true;


	// fill vertex buffer
	//
	PATCHVERTEX*	pVertices = NULL;

	vBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_NOSYSLOCK);
	memcpy(pVertices, vertices, numVertices*sizeof(PATCHVERTEX));
	vBuffer->Unlock();


	// fill index buffer
	//
	WORD*			pIndices = NULL;

	iBuffer->Lock(0, 0, (BYTE**)&pIndices, D3DLOCK_NOSYSLOCK);
	memcpy(pIndices, indices, numIndices*sizeof(unsigned short));
	iBuffer->Unlock();
	*/
	return true;
}


bool
TerrainPatch::checkBuffers()
{
	/*NZ
	// create new buffers if the size changed
	//
	if(numIndices!=numNewIndices || forceBufferCreate || iBuffer==NULL)
	{
		numIndices = numNewIndices;
		SAFE_RELEASE(iBuffer)

		if(numIndices>0)
		{
			if(FAILED(pd3dDevice->CreateIndexBuffer(numIndices * sizeof(WORD), D3DUSAGE_WRITEONLY,
													D3DFMT_INDEX16, pgIDirectX::getPoolType(), &iBuffer)))
			{
				pgLog::error("creating index buffer for terrainpatch failed");
				return false;
			}

			pgLog::trace("index-buffer (%d|%d) recreated\n", gridX,gridY);
		}
	}


	if(numVertices!=numNewVertices  || forceBufferCreate || vBuffer==NULL)
	{
		numVertices = numNewVertices;
		SAFE_RELEASE(vBuffer)

		if(numVertices>0)
		{
			if(FAILED(pd3dDevice->CreateVertexBuffer(numVertices * sizeof(PATCHVERTEX), D3DUSAGE_WRITEONLY,
													 D3DFVF_PATCHVERTEX, pgIDirectX::getPoolType(), &vBuffer)))
			{
				pgLog::error("creating vertex buffer for terrainpatch failed");
				SAFE_RELEASE(iBuffer);
				return false;
			}

			pgLog::trace("vertex-buffer (%d|%d) recreated\n", gridX,gridY);
		}
	}
	*/

	return true;
}

