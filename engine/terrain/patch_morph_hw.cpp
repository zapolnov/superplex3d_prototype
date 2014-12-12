///////////////////////////////////////////////////////////////////
//
// project:     Pluto Game Engine
// class:       TerrainPatchMorphHW
// creator:     Daniel Wagner
// description: 
//
///////////////////////////////////////////////////////////////////
// $Id: TerrainPatchMorphHW.cpp,v 1.13 2003/03/09 18:15:24 daniel Exp $
///////////////////////////////////////////////////////////////////


#include "patch_morph_hw.h"
#include "terrain.h"


struct PATCHVERTEX_MORPH_HW
{
	float		posX,posZ;
	float		texX,texY;
	float		posY, yMoveSelf, v0,v1,
				yMoveLeft, yMoveLeft2, yMoveRight, yMoveRight2,
				yMoveBottom, yMoveBottom2, yMoveTop, yMoveTop2;
};

/*NZ
DWORD	dwMorphShaderDecl[] =
		{
			D3DVSD_STREAM(0),
			D3DVSD_REG(0, D3DVSDT_FLOAT4),
			D3DVSD_REG(1, D3DVSDT_FLOAT4),
			D3DVSD_REG(2, D3DVSDT_FLOAT4),
			D3DVSD_REG(3, D3DVSDT_FLOAT4),
			D3DVSD_END()
		};
*/


inline uint32 FtoDW( float f ) { return *((uint32*)&f); }

uint32	TerrainPatchMorphHW::morphShader = NULL;


TerrainPatchMorphHW::TerrainPatchMorphHW(Terrain* nMaster, int nX, int nY) : TerrainPatchMorphSW(nMaster, nX,nY)
{
}


void
TerrainPatchMorphHW::init()
{
	TerrainPatchMorphSW::init();
}


void
TerrainPatchMorphHW::deinit()
{
	TerrainPatchMorphSW::deinit();
}


int
TerrainPatchMorphHW::render()
{
	/*NZ
	if(!checkDevice("cannot render terrainpatch: no device set"))
		return 0;

	if(!isVisible())
		return 0;

	if(iBuffer==NULL || vBuffer==NULL)			// nothing to do ?
		return 0;


	float	factorLeft2=0.0f, factorRight2=0.0f, factorBottom2=0.0f, factorTop2=0.0f;

	Q_ASSERT(selfTes>=0 && selfTes<=MAX_SUBDIV);

	factorSelf = 1.0f-currentError;
	factorLeft = left ? 1.0f-left->getCurrentError() : 0.0f;
	factorRight = right ? 1.0f-right->getCurrentError() : 0.0f;
	factorBottom = bottom ? 1.0f-bottom->getCurrentError() : 0.0f;
	factorTop = top ? 1.0f-top->getCurrentError() : 0.0f;

	Q_ASSERT(factorSelf>=0.0f);
	Q_ASSERT(factorLeft>=0.0f);
	Q_ASSERT(factorRight>=0.0f);
	Q_ASSERT(factorBottom>=0.0f);
	Q_ASSERT(factorTop>=0.0f);

	// clamping for patches which use selfTes!=realSelfTes
	//
	if(factorSelf>=1.0f)
		factorSelf = 1.0f;
	if(factorLeft>=1.0f)
		factorLeft = 1.0f;
	if(factorRight>=1.0f)
		factorRight = 1.0f;
	if(factorBottom>=1.0f)
		factorBottom = 1.0f;
	if(factorTop>=1.0f)
		factorTop = 1.0f;

	factorLeft2 = (1.0f-factorSelf) * factorLeft;
	factorRight2 = (1.0f-factorSelf) * factorRight;
	factorBottom2 = (1.0f-factorSelf) * factorBottom;
	factorTop2 = (1.0f-factorSelf) * factorTop;


	D3DXVECTOR4 weights0(factorSelf, 0.0f, 0.5f, 1.0f),
				weights2(factorLeft, factorLeft2, factorRight, factorRight2),
				weights3(factorBottom, factorBottom2, factorTop, factorTop2);


	pd3dDevice->SetVertexShaderConstant(0, &weights0, 1);
	pd3dDevice->SetVertexShaderConstant(2, &weights2, 1);
	pd3dDevice->SetVertexShaderConstant(3, &weights3, 1);

	pd3dDevice->SetStreamSource(0, vBuffer, sizeof(PATCHVERTEX_MORPH_HW));
	pd3dDevice->SetIndices(iBuffer, 0);


	// draw the patch with one single D3D call
	HRESULT hr = pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, numVertices, 0, numIndices-2);

	if(hr==D3D_OK)
		return numIndices-2;
	else
		*/
		return 0;
}


void
TerrainPatchMorphHW::deleteDeviceObjects()
{
	TerrainPatchMorphSW::deleteDeviceObjects();
}


bool
TerrainPatchMorphHW::restoreDeviceObjects()
{
	return TerrainPatchMorphSW::restoreDeviceObjects();
}


bool
TerrainPatchMorphHW::fillBuffers()
{
	/*NZ
	if(master->getMergePatches() && selfTes==4)
	{
		numIndices = numNewIndices;
		numVertices = numNewVertices;
		forceBufferCreate = false;
		return true;
	}

	if(!forceBufferCreate && !forceRetessellation &&
		followsLeft==newFollowsLeft && !recalcBorderLeft &&
		followsRight==newFollowsRight && !recalcBorderRight &&
		followsBottom==newFollowsBottom && !recalcBorderBottom &&
		followsTop==newFollowsTop && !recalcBorderTop)
		return true;

	if(!checkDevice("cannot create terrainpatch: no device set"))
		return false;

	if(!checkBuffers())
		return false;

	if(numVertices==0 || numIndices==0)					// nothing to do ???
		return true;

	int i;

	//if(forceBufferCreate)
		master->addBufferCreateCount();

	// fill vertex buffer
	//
	PATCHVERTEX_MORPH_HW*	pVertices = NULL;
	vBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);

	for(i=0; i<numVertices; i++)
	{
		pVertices[i].posX = vertices[i].pos.x;
		pVertices[i].posZ = vertices[i].pos.z;
		pVertices[i].texX = vertices[i].tex.x;
		pVertices[i].texY = vertices[i].tex.y;

		pVertices[i].posY = verticesLow[i].pos.y;
		pVertices[i].yMoveSelf = yMoveSelf[i];
		pVertices[i].v0 = 0.0f,
		pVertices[i].v1 = 1.0f,
		pVertices[i].yMoveLeft = yMoveLeft[i];
		pVertices[i].yMoveLeft2 = yMoveLeft2[i];
		pVertices[i].yMoveRight = yMoveRight[i];
		pVertices[i].yMoveRight2 = yMoveRight2[i];
		pVertices[i].yMoveBottom = yMoveBottom[i];
		pVertices[i].yMoveBottom2 = yMoveBottom2[i];
		pVertices[i].yMoveTop = yMoveTop[i];
		pVertices[i].yMoveTop2 = yMoveTop2[i];
	}

	vBuffer->Unlock();


	// fill index buffer
	//
	WORD*			pIndices = NULL;

	iBuffer->Lock(0, 0, (BYTE**)&pIndices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);
	for(i=0; i<numIndices; i++)
	{
		pIndices[i] = indices[i];
		//pgLog::trace("%d: %d\n", i, indices[i]);
	}
	iBuffer->Unlock();


	//pgLog::trace("patch buffers (%d|%d) filled\n", gridX,gridY);
	*/
	return true;
}


bool
TerrainPatchMorphHW::checkBuffers()
{
	/*NZ
	// create new buffers if the size changed
	//
	if(numIndices<numNewIndices || forceBufferCreate)
	{
		numIndices = numNewIndices;
		SAFE_RELEASE(iBuffer)

		if(numIndices>0)
			if(FAILED(pd3dDevice->CreateIndexBuffer(numIndices * sizeof(WORD), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,
													D3DFMT_INDEX16, pgIDirectX::getPoolType(), &iBuffer)))
			{
				pgLog::error("creating index buffer for terrainpatch failed");
				return false;
			}

		//pgLog::trace("index-buffer (%d|%d) recreated\n", gridX,gridY);
	}


	if(m_NumVertices<m_NumNewVertices  || m_ForceBufferCreate)
	{
		m_NumVertices = m_NumNewVertices;
		NZ SAFE_RELEASE(vBuffer)

		if(m_NumVertices>0)
			if(FAILED(pd3dDevice->CreateVertexBuffer(numVertices * sizeof(PATCHVERTEX_MORPH_HW), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,
													 0, pgIDirectX::getPoolType(), &vBuffer)))
			{
				pgLog::error("creating base vertex buffer for terrainpatch failed");
				SAFE_RELEASE(iBuffer);
				return false;
			}
	}

	m_ForceBufferCreate = false;
	*/
	return true;
}


bool
TerrainPatchMorphHW::setupVertexShader()
{
	/*NZ
	LPDIRECT3DDEVICE8 pd3dDevice = pgIDirectX::getDevice();

	if(!pd3dDevice)
	{
		pgLog::error("can not setup terrain vertex shader: no D3D device");
		return false;
	}

	D3DXMATRIX mat, matTranspose;
	D3DXMatrixMultiply(&mat, pgIDirectX::getViewMatrix().getD3DMatrix(), pgIDirectX::getProjectionMatrix().getD3DMatrix());
	D3DXMatrixTranspose(&matTranspose, &mat);

	pd3dDevice->SetVertexShaderConstant(4, &matTranspose, 4);

	if(morphShader==NULL)
	{
		HRESULT hr;
		if(FAILED((hr=D3DUtil_CreateVertexShader(pd3dDevice, pgISettings::tmpFullPath("shader/terrainMorph.vsh"),
												 dwMorphShaderDecl, &morphShader))))
		{
			pgLog::error("failed to create vertex shader for terrain MORPH_HW");

			char szBuffer[512];
			D3DXGetErrorStringA(hr, szBuffer, sizeof(szBuffer));
			pgLog::error(szBuffer);
			pgLog::trace(szBuffer);

			return false;
		}
	}

	return(pd3dDevice->SetVertexShader(morphShader)==S_OK);
	*/
	/*NZ*/return 0;
}


/*NZ
bool
TerrainPatchMorphHW::setupTextureMatrix(D3DXMATRIX& nTexMatrix)
{
	D3DXMATRIX			matTranspose;
	LPDIRECT3DDEVICE8	pd3dDevice = pgIDirectX::getDevice();

	if(!pd3dDevice)
	{
		pgLog::error("can not setup terrain texture matrix: no D3D device");
		return false;
	}

	D3DXMatrixTranspose(&matTranspose, &nTexMatrix);

	pd3dDevice->SetVertexShaderConstant(8, &matTranspose, 4);

	return true;
}
*/

