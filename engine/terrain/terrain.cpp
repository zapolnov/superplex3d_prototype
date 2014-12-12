#include "terrain.h"
#include "patch.h"
#include "patch_morph_sw.h"
#include "patch_morph_hw.h"


inline uint32 FtoDW( float f ) { return *((uint32*)&f); }


/* Terrain methods */

//
// Constructor
//
Terrain::Terrain(HeightMapRenderMode mode)
	: m_RenderMode(mode), m_PVS(NULL), m_FogColor(1.0f, 1.0f, 1.0f, 1.0f), m_HeightMap(NULL),
	  m_IndexBuffer(NULL), m_VertexBuffer(NULL)
{
	for (int i = 0; i <= 4; i++)
		patchCount[i] = 0;
	bufferCreateCount = 0;
	mergeSmallPatches = true;


	patches = NULL;
	patchesX = patchesY = 0;
//NZ	heightmap = NULL;
	normals = NULL;
	maxError = 0.015f;
	desiredFPS = 0;
	activePatches = NULL;
	forceBufferCreate = true;
	skySpeedX = skySpeedY = 0.0f;

	m_FogNear = m_FogFar = 0.0f;

//NZ	treeMap = NULL;

//NZ	mipFilter = pgTextureStage::FILTER_LINEAR;

	numNewVertices = numNewIndices = numVertices = numIndices = 0;

	useUnitDistance = true;

	updateWorldMatrix();
}


void
Terrain::setPatchSize(float nX, float nY, float nZ)
{
	patchDX = nX;
	patchDY = nY;
	patchDZ = nZ;
}

//
// Sets the height map
//
void Terrain::setHeightMap(const ImageFilePtr & image)
{
	if (unlikely(!image))
		delete m_HeightMap;

	m_HeightMap = new byte[image->getWidth() * image->getHeight()];
	// FIXME
}


/*NZ
void
Terrain::setSkyMap(pgTexture* nSkyMap, float nX, float nY, float nSpeedX, float nSpeedY)
{
	fillPassInfo(&skyPass, nSkyMap, nX, nY, NULL);
	skyPosX = skyPosY = 0.0f;
	skySpeedX = nSpeedX;
	skySpeedY = nSpeedY;
}
*/


void
Terrain::updateSkyMatrix()
{
	/*NZ
	if(!skyPass.texColor)
		return;

	float ftime = pgITime::getLastFrameTime();

	skyPosX += ftime * skySpeedX;
	skyPosY += ftime * skySpeedY;

	D3DXMatrixScaling(&skyPass.matrix, skyPass.repeatX, skyPass.repeatY, 1.0f);
	skyPass.matrix._31 = skyPosX;
	skyPass.matrix._32 = skyPosY;
	*/
}


/*NZ
void
Terrain::fillPassInfo(MultiPassInfo* nPass, pgTexture* nTexture, float nRepeatX, float nRepeatY, pgTexture* nModTexture)
{
	nPass->texColor = nTexture;
	nPass->texMod = nModTexture;

	if(nRepeatX==0.0f)	nRepeatX = 1.0f;
	if(nRepeatY==0.0f)	nRepeatY = 1.0f;

	nPass->repeatX = nRepeatX;
	nPass->repeatY = nRepeatY;
	D3DXMatrixScaling(&nPass->matrix, nPass->repeatX, nPass->repeatY, 1.0f);
}
*/

/*NZ
void
Terrain::addPass(pgTexture* nColorTexture, float nRepeatX, float nRepeatY, pgTexture* nTransTexture)
{
	MultiPassInfo* pass = new MultiPassInfo();
	multiPassInfos.addTail(pass);

	fillPassInfo(pass, nColorTexture, nRepeatX, nRepeatY, nTransTexture);
}
*/


void
Terrain::calcPassVisibility(MultiPassInfo* nInfo, int nX, int nY)
{
	/*NZ
	int dx = nInfo->texMod->getWidth(),
		dy = nInfo->texMod->getHeight();
	int px = dx / patchesX,
		py = dy / patchesY;
	int	x0 = nX*px, x1 = nX*px+px,
		y0 = nY*py, y1 = nY*py+py,
		x,y;
	unsigned char	*data = nInfo->visData,
					min = 10, max = 245;
	int				pitch = nInfo->visPitch;

	nInfo->inVis = nInfo->fullVis = true;

	for(x=x0; x<x1; x++)
		for(y=y0; y<y1; y++)
		{
			int idx = 4*x + (dy-1-y)*pitch + 3;
			unsigned char d = data[idx];

			if(d>=min)
				nInfo->inVis = false;
			if(d<=max)
				nInfo->fullVis = false;
		}
	*/
}


void
Terrain::build()
{
	/*NZ
	int x,y, pass, pass2;

	if(!heightmap)
		return;

	patches = new pgTerrainPatchBasePPtr[patchesY];
	for(y=0; y<patchesY; y++)
		patches[y] = new pgTerrainPatchBasePtr[patchesX];

	sizeX = patchDX * patchesX;
	sizeY = patchDY;
	sizeZ = patchDZ * patchesY;


	if(pvsName.getLength()>0)
	{
		pvs = new pgTerrainPVS();
		if(!pvs->load(pgISettings::tmpFullPath(pvsName+".pvs")))
		{
			delete pvs;
			pvs = NULL;
		}
	}


	for(pass=1; pass<multiPassInfos.getSize(); pass++)
	{
		MultiPassInfo* p = multiPassInfos[pass];
		bool ok = p->texMod->getData(p->visData, p->visPitch);
		assert(ok);
	}


	for(y=0; y<patchesY; y++)
		for(x=0; x<patchesX; x++)
		{
			TerrainPatchBase* patch = createPatch(x,y);
			patches[y][x] = patch;

			patch->setScale(Vector3(patchDX, patchDY, patchDZ));
			patch->setPosition(Vector3(x*patchDX, 0.0f, y*patchDZ));
			patch->setHeightMapPosition(x*(TerrainPatchBase::SOURCE_WIDTH-1),
										y*(TerrainPatchBase::SOURCE_HEIGHT-1));
			patch->calcErrors();
			patch->calcMinMaxY();

			// calculate for each pass if there is anything to draw
			// (check the alpha map)
			int visFlags = 0,
				numPasses = multiPassInfos.getSize();

			for(pass=1; pass<numPasses; pass++)
				calcPassVisibility(multiPassInfos[pass], x,y);

			visFlags = TerrainPatchBase::VIS_LAYERALL;

			for(pass=1; pass<numPasses; pass++)									// base pass checking
				if(multiPassInfos[pass]->fullVis)
					visFlags &= ~TerrainPatchBase::VIS_BASE;

			for(pass=1; pass<numPasses; pass++)									// additional passes
			{
				if(multiPassInfos[pass]->inVis)									// is this pass invisible ? (also no need to check on)
				{
					visFlags &= ~TerrainPatchBase::getAddLayerFlag(pass);
					continue;
				}

				for(pass2=pass+1; pass2<numPasses; pass2++)						// see if another pass completely covers this pass
					if(multiPassInfos[pass2]->fullVis)
					{
						visFlags &= ~TerrainPatchBase::getAddLayerFlag(pass);
						break;
					}
			}

			patch->setVisFlags(visFlags);
		}


	// tell each patch about its neighbors
	//
	for(y=0; y<patchesY; y++)
		for(x=0; x<patchesX; x++)
			patches[y][x]->setNeighbors(x>0 ? patches[y][x-1] : NULL,
										x<patchesX-1 ? patches[y][x+1] : NULL,
										y>0 ? patches[y-1][x] : NULL,
										y<patchesY-1 ? patches[y+1][x] : NULL);


	for(pass=1; pass<multiPassInfos.getSize(); pass++)
	{
		MultiPassInfo* p = multiPassInfos[pass];
		p->texMod->releaseData();
	}
	*/
}


void
Terrain::update()
{
	/*NZ
	if(!patches)
		return;

	Vector3				cPos, lPos,rPos;
	float				fact = (float)tan(pgIDirectX::getFOVX()/2.0f),
						farP = pgIDirectX::getFarPlane();
	int					x=0,y=0;
	D3DXVECTOR3			pt0,pt1,pt2, tpt0,tpt1,tpt2;
	D3DXMATRIX			vMat, tMat;
	//pgTerrainPatchPtr	curPatch;
	pgTerrainPatchBasePtr	curPatch;
	pgPlane				planes[6];


	profileUpdate.beginSection();

	if(desiredFPS>0)
	{
		if(pgITime::getFPS()<desiredFPS*0.95f && maxError<0.5f)
			maxError *= 1.03f;
		else
		if(pgITime::getFPS()>desiredFPS*1.05f && maxError>0.005f)
			maxError *= 0.97f;
	}


	numPatchesProcessed = 0;


	updateSkyMatrix();
	updateWorldMatrix();

	// calculate which rectangle of the map
	// is visible from the clipping frustum
	lPos = cPos + Vector3(-farP*fact, 0.0f, farP);
	rPos = cPos + Vector3( farP*fact, 0.0f, farP);

	pgIDirectX::setD3DVecFromVec3(pt0, cPos);
	pgIDirectX::setD3DVecFromVec3(pt1, lPos);
	pgIDirectX::setD3DVecFromVec3(pt2, rPos);

	//D3DXMatrixInverse(&vMat, NULL, pgIDirectX::getViewMatrix());
	//D3DXMatrixTranspose(&tMat, &vMat);

	D3DXMatrixInverse(&vMat, NULL, pgIDirectX::getViewMatrix().getD3DMatrix());

	D3DXMatrixMultiply(&tMat, &worldMatrix, &vMat);
	D3DXVec3TransformCoord(&tpt0, &pt0, &tMat);
	D3DXVec3TransformCoord(&tpt1, &pt1, &tMat);
	D3DXVec3TransformCoord(&tpt2, &pt2, &tMat);

	int	x0 = (int)(tpt0.x / patchDX),	y0 = (int)(tpt0.z / patchDZ),
		x1 = (int)(tpt1.x / patchDX),	y1 = (int)(tpt1.z / patchDZ),
		x2 = (int)(tpt2.x / patchDX),	y2 = (int)(tpt2.z / patchDZ);

	maxX = -1;	maxY = -1;	minX = patchesX+1;	minY = patchesY+1;

	if(x0<minX)	minX = x0;
	if(y0<minY)	minY = y0;
	if(x0>maxX)	maxX = x0;
	if(y0>maxY)	maxY = y0;
	if(x1<minX)	minX = x1;
	if(y1<minY)	minY = y1;
	if(x1>maxX)	maxX = x1;
	if(y1>maxY)	maxY = y1;
	if(x2<minX)	minX = x2;
	if(y2<minY)	minY = y2;
	if(x2>maxX)	maxX = x2;
	if(y2>maxY)	maxY = y2;

	minX-=2;	minY-=2;	maxX+=2;	maxY+=2;

	if(minX<0)			minX = 0;
	if(minY<0)			minY = 0;
	if(maxX>=patchesX)	maxX = patchesX-1;
	if(maxY>=patchesY)	maxY = patchesY-1;

	//minX=12; maxX=14;
	//minY=3; maxY=4;


	if(pgIDirectX::getUpdateVisibility())
	{
		// mark all patches in the list as non active
		// (checkVisibility will set this to active, if
		// it is in the list and is active)
		curPatch = activePatches;
		while(curPatch)
		{
			curPatch->setFlagActive(false);
			curPatch = curPatch->getNextActive();
		}


		// create and transform the clipping planes
		// (we transform them here, so that checkVisibility()
		// doesn't need to transform any corner points)
		pgIDirectX::createClippingPlanes(planes);


		// first determine for each patch if
		// it will be visible during the next rendering
		int pvsX=-1, pvsY=-1, pvsZ=-1;

		if(pvs)
		{
			pvsX = tpt0.x>=0.0f ? (int)(tpt0.x / patchDX) : -1;
			pvsY = tpt0.z>=0.0f ? patchesY-1 - (int)(tpt0.z / patchDZ) : -1;
			pvsZ = (int)((tpt0.y / patchDY) * 255) / pvs->getDataDZ();
		}

		D3DXMATRIX	mat;
		D3DXMatrixMultiply(&mat, &worldMatrix, pgIDirectX::getViewMatrix().getD3DMatrix());


		for(y=minY; y<=maxY; y++)
			for(x=minX; x<=maxX; x++)
				if(patches[y][x])
				{
					if(patches[y][x]->checkVisibility(&mat, planes) && pvs)
					{
						// if the patch could not be frustum-culled we try pvs (if there is pvs...)
						//
						if(!pvs->isVisible(pvsX,pvsY,pvsZ, x,patchesY-1-y))
							patches[y][x]->setVisible(false);
					}

					numPatchesProcessed++;
				}


		// deinit all patches will are still in the
		// list, but where checkVisibility() has not been called
		curPatch = activePatches;
		while(curPatch)
		{
			if(!curPatch->getFlagActive())
				curPatch->deinit();
			curPatch = curPatch->getNextActive();
		}


		// next update the projected error
		// and tesselation stage
		D3DXMATRIX	mat2, matT;

		D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 1.2f*max(patchDY,max(patchDX,patchDZ)));

		D3DXMatrixMultiply(&mat2, &matT, pgIDirectX::getProjectionMatrix().getD3DMatrix());
		D3DXMatrixMultiply(&mat, pgIDirectX::getViewMatrix().getD3DMatrix(), &mat2);
		D3DXMatrixMultiply(&mat2, &worldMatrix, &mat);


		curPatch = activePatches;
		while(curPatch)
		{
			curPatch->updateProjectedErrors(&mat2);

			for(int i=TerrainPatchBase::MAX_SUBDIV; i>=0; i--)
				if(curPatch->getProjectedError(i)<maxError)
				{
					curPatch->setRealTessellation(i);
					curPatch->setTessellation(i);
					break;
				}

			curPatch = curPatch->getNextActive();
		}


		// next check that each neighbor has only a
		// tesselation stage difference at max. 1
		if(useUnitDistance)
		{
			bool changed;

			do
			{
				changed=false;
				curPatch = activePatches;
				while(curPatch)
				{
					int tes = curPatch->getNewTessellation(),
						lT = curPatch->getLeft()->getNewTessellationSafe(),
						rT = curPatch->getRight()->getNewTessellationSafe(),
						bT = curPatch->getBottom()->getNewTessellationSafe(),
						tT = curPatch->getTop()->getNewTessellationSafe();


					int sT = min(min(lT+1,rT+1) , min(bT+1,tT+1));

					if(tes>sT)
					{
						curPatch->setTessellation(sT);
						//curPatch->setRealTessellation(sT);
						changed = true;
					}

					curPatch = curPatch->getNextActive();
				}
			} while(changed);

		}


		// finally update tesselation to be correct to
		// all the neighbours' tesselation stages
		numNewVertices = numNewIndices = 0;
		curPatch = activePatches;
		while(curPatch)
		{
			int tes = curPatch->getNewTessellation(),
				lT = curPatch->getLeft()->getNewTessellationSafe(),
				rT = curPatch->getRight()->getNewTessellationSafe(),
				bT = curPatch->getBottom()->getNewTessellationSafe(),
				tT = curPatch->getTop()->getNewTessellationSafe();

			assert(tes-lT<=1);
			assert(tes-rT<=1);
			assert(tes-bT<=1);
			assert(tes-tT<=1);

			curPatch->updateTessellation();
			numNewVertices += curPatch->getNumVertices();
			numNewIndices += curPatch->getNumIndices() + 2;	// two more for degen-tris

			curPatch = curPatch->getNextActive();
		}

		// some patch types may need a second update method
		// (e.g.: to adapt to each other)
		curPatch = activePatches;
		while(curPatch)
		{
			curPatch->updateTessellation2();
			curPatch = curPatch->getNextActive();
		}

		// some patch types may need a third update method
		// (e.g.: to adapt to each other)
		curPatch = activePatches;
		while(curPatch)
		{
			curPatch->updateTessellation3();
			curPatch = curPatch->getNextActive();
		}
	}


	if(numNewIndices>0)
		numNewIndices -= 2;			// the last patch does not need degen-tris


	profileUpdate.endSection();
	*/
}


void
Terrain::setupTextureStages(MultiPassInfo* nPass, bool nBasePass, bool nForceNoHW)
{
	/*NZ
	bool morphHW = nForceNoHW ? false : renderMode==HardwareMorphRender;


	// texture stages setup
	//
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   nBasePass ? D3DTOP_DISABLE : D3DTOP_SELECTARG1);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, mipFilter);
	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	if(nPass->texMod)
	{
		pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   nBasePass ? D3DTOP_DISABLE : D3DTOP_SELECTARG1);
		pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		pd3dDevice->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		pd3dDevice->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		pd3dDevice->SetTextureStageState(1, D3DTSS_MIPFILTER, mipFilter);
		pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, morphHW ? 1 : 0);

		pd3dDevice->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		pd3dDevice->SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	}
	else
	{
		pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	}


	// zbuffer, filling & culling setup
	//
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, pgIDirectX::getFillFlag());
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, pgIDirectX::getCullFlag(D3DCULL_CCW));


	// fog setup
	//
	if(m_FogNear!=0.0f || m_FogFar!=0.0f)
	{
		float fogDens = 1.0f;

		pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_FOGCOLOR,  D3DCOLOR_COLORVALUE(m_FogColor[0], m_FogColor[1], m_FogColor[2], m_FogColor[3]));

		pd3dDevice->SetRenderState(D3DRS_FOGSTART, FtoDW(m_FogNear));
		pd3dDevice->SetRenderState(D3DRS_FOGEND, FtoDW(m_FogFar));
		pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, FtoDW(fogDens));
		pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		//pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	}
	else
		pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);


	// set textures
	//
	pd3dDevice->SetTexture(0, nPass->texColor->getD3DTexture());
	pd3dDevice->SetTexture(1, nPass->texMod->getD3DTexture());


	// setup lighting & blending
	//
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if(nBasePass)
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	else
	{
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}


	// setup texture matrix for stage 0
	//
	//if(renderMode!=HardwareMorphRender)
	{
		if(nPass->repeatX!=1.0f || nPass->repeatY!=1.0f)
		{
			pd3dDevice->SetTransform(D3DTS_TEXTURE0, &nPass->matrix);
			pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		}
		else
			pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	}
	//else
		pgTerrainPatchMorphHW::setupTextureMatrix(nPass->matrix);

	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	*/
}


void
Terrain::setupTextureStagesForSky(bool nForceNoHW)
{
	/*NZ
	bool morphHW = nForceNoHW ? false : renderMode==HardwareMorphRender;


	// texture stages setup
	//
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, mipFilter);
	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);


	// zbuffer, filling & culling setup
	//
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, pgIDirectX::getFillFlag());
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, pgIDirectX::getCullFlag(D3DCULL_CCW));


	// fog setup
	//
	if(m_FogNear!=0.0f || m_FogFar!=0.0f)
	{
		float fogDens = 0.0f;

		pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_FOGCOLOR,  D3DCOLOR_COLORVALUE(m_FogColor[0], m_FogColor[1], m_FogColor[2], m_FogColor[3]));

		pd3dDevice->SetRenderState(D3DRS_FOGSTART, FtoDW(m_FogNear));
		pd3dDevice->SetRenderState(D3DRS_FOGEND, FtoDW(m_FogFar));
		pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, FtoDW(fogDens));
		pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	}
	else
		pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);


	// set texture
	//
	pd3dDevice->SetTexture(0, skyPass.texColor->getD3DTexture());


	// setup lighting & blending
	//
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	// setup texture matrix for stage 0
	//
	if(!morphHW)
	{
		if(skyPass.repeatX!=1.0f || skyPass.repeatY!=1.0f)
		{
			//D3DXMATRIX			matTranspose;
			//D3DXMatrixTranspose(&matTranspose, &skyPass.matrix);
			//pd3dDevice->SetTransform(D3DTS_TEXTURE0, &matTranspose);

			pd3dDevice->SetTransform(D3DTS_TEXTURE0, &skyPass.matrix);
			pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		}
		else
			pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	}
	else
		pgTerrainPatchMorphHW::setupTextureMatrix(skyPass.matrix);

	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	*/
}


void
Terrain::updateWorldMatrix()
{
	/*NZ
	D3DXMATRIX matScale, matMove, matRot, mat;
	Vector3 scale(1.0f, 1.0f, 1.0f);

	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matMove, pos.x,pos.y,pos.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.x,rot.y,rot.z);
	D3DXMatrixMultiply(&mat, &matRot, &matMove);
	D3DXMatrixMultiply(&worldMatrix, &matScale, &mat);
	*/
}


void
Terrain::checkBuffers()
{
	/*NZ
	if(renderMode==UnifiedRender || mergeSmallPatches)
	{
		assert(numNewIndices < 0xffff);

		if(numNewIndices > numIndices || forceBufferCreate)
		{
			SAFE_RELEASE(m_IndexBuffer)

			if(numNewIndices>0)
				if(FAILED(pd3dDevice->CreateIndexBuffer(numNewIndices * sizeof(WORD), D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
														D3DFMT_INDEX16, pgIDirectX::getPoolType(), &m_IndexBuffer)))
				{
					pgLog::error("creating index buffer for terrain failed");
					return;
				}
		}
		numIndices = numNewIndices;

		if(numNewVertices > numVertices || forceBufferCreate)
		{
			SAFE_RELEASE(m_VertexBuffer)

			if(numNewVertices>0)
			{
				if(FAILED(pd3dDevice->CreateVertexBuffer(numNewVertices * sizeof(TerrainPatchBase::PATCHVERTEX),
														 D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
														 D3DFVF_PATCHVERTEX, pgIDirectX::getPoolType(), &m_VertexBuffer)))
				{
					pgLog::error("creating vertex buffer for terrain failed");
					SAFE_RELEASE(m_IndexBuffer);
					return;
				}
			}
		}
		numVertices = numNewVertices;
	}

	forceBufferCreate = false;
	*/
}


void
Terrain::render()
{
	/*NZ
	// activate the morph shader for HardwareMorphRender mode
	//
	if(renderMode==HardwareMorphRender)
		if(!pgTerrainPatchMorphHW::setupVertexShader())
			return;

	profileRender.beginSection();

	switch(renderMode)
	{
	case UnifiedRender:
		renderUnified();
		break;

	case SplitRender:
	case SoftwareMorphRender:
	case HardwareMorphRender:
		renderSplit();
		break;

	default:
		break;
	}

	profileRender.endSection();
	*/
}


void
Terrain::renderSplit()
{
	/*NZ
	if(!checkDevice("cannot render terrain: no device set"))
		return;

	TerrainPatchBase*					patch;
	TerrainPatchBase::PATCHVERTEX*	pVertices = NULL;
	WORD*								pIndices = NULL;
	int									i, pass, numTris=0, numPasses = multiPassInfos.getSize(), numPassesP;

	for(i=0; i<=TerrainPatchBase::MAX_SUBDIV; i++)
		patchCount[i] = 0;
	numPatchesRendered = numPatchesPassesRendered = 0;

	if(mergeSmallPatches)
	{
		numNewVertices = 0;
		numNewIndices = 0;
		patch = activePatches;
		while(patch)
		{
			if(patch->getNewTessellation()==4)
			{
				numNewVertices += patch->getNumVertices();
				numNewIndices += patch->getNumIndices()+2;
			}
			patch = patch->getNextActive();
		}

		checkBuffers();
	}

	numPassesP = skyPass.texColor ? numPasses+1 : numPasses;

	for(pass=0; pass<numPassesP; pass++)
	{
		int				numV=0, numI=0;

		if(pass<numPasses)
			setupTextureStages(multiPassInfos[pass], false);
		else
			setupTextureStagesForSky();

		if(mergeSmallPatches && m_VertexBuffer && m_IndexBuffer)
		{
			if(renderMode==HardwareMorphRender)
				if(!pgTerrainPatchMorphHW::setupVertexShader())
					return;

			m_VertexBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);
			m_IndexBuffer->Lock(0, 0, (BYTE**)&pIndices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);
		}


		patch = activePatches;
		while(patch)
		{
			int tes = patch->getNewTessellation();

			assert(tes>=0 && tes<=TerrainPatchBase::MAX_SUBDIV);

			if(pass==0)
			{
				numPatchesRendered++;
				patchCount[tes]++;
				patch->setRendered(false);
			}

			if(pass<numPasses || patch->getVisFlags()&TerrainPatchBase::getAddLayerFlag(pass))
			{
				if(mergeSmallPatches && tes==4)
				{
					// copy the vertex data
					//
					int numNewV = patch->fillBaseVertices(pVertices+numV);

					// then copy the indices, which must be updated to
					// reflect the moved vertex data (index shifts)
					//
					const WORD*	indices = patch->getIndices();
					int			n = patch->getNumIndices();

					if(numI>0)				// add degen-tris to concenate strips
					{
						pIndices[numI++] = pIndices[numI-1];
						pIndices[numI++] = (unsigned short)(indices[0] + numV);
					}

					for(i=0; i<n; i++)
						pIndices[numI++] = (unsigned short)(indices[i] + numV);

					numV += numNewV;
				}
				else
				{
					numTris = patch->render();
				}

				if(!patch->getRendered())
					pgIDirectX::addTerrainTris(numTris);

				patch->setRendered(true);
				numPatchesPassesRendered++;
			}

			patch = patch->getNextActive();
		}

		if(mergeSmallPatches && m_VertexBuffer && m_IndexBuffer)
		{
			if(pass<numPasses)
				setupTextureStages(multiPassInfos[pass], false, true);
			else
				setupTextureStagesForSky(true);

			m_IndexBuffer->Unlock();
			m_VertexBuffer->Unlock();

			assert(numV<=numNewVertices);
			assert(numI<=numNewIndices);

			pd3dDevice->SetVertexShader(D3DFVF_PATCHVERTEX);
			pd3dDevice->SetStreamSource(0, m_VertexBuffer, sizeof(TerrainPatchBase::PATCHVERTEX));
			pd3dDevice->SetIndices(m_IndexBuffer, 0);

			//pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, numV, 0, numI-2);
			pgIDirectX::addTerrainTris(numV);
		}
	}
	*/


	/*if(skyPass.texColor)
	{
		setupTextureStagesForSky();
		patch = activePatches;
		while(patch)
		{
			patch->render();
			numPatchesPassesRendered++;
			patch = patch->getNextActive();
		}
	}*/
}


void
Terrain::renderUnified()
{
	/*NZ
	TerrainPatchBase::PATCHVERTEX*	pVertices = NULL;
	WORD*							pIndices = NULL;
	int								i, numV=0, numI=0;
	unsigned short					lastIndex = 0xffff;

	if(!patches || !checkDevice("cannot render terrain: no device set"))
		return;

	checkBuffers();
	pd3dDevice->SetTransform(D3DTS_WORLD, &worldMatrix);

	numPatchesRendered = numPatchesPassesRendered = 0;

	if(m_VertexBuffer==NULL || m_IndexBuffer==NULL || numVertices==0 || numIndices==0)
		return;


	// lock only as much of the buffers
	// as we are really going to update
	//
	m_VertexBuffer->Lock(0, numVertices*sizeof(TerrainPatchBase::PATCHVERTEX), (BYTE**)&pVertices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);
	m_IndexBuffer->Lock(0, numIndices*sizeof(WORD), (BYTE**)&pIndices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);

	//pgTerrainPatch* patch = activePatches;
	TerrainPatchBase* patch = activePatches;
	while(patch)
	{
		// copy the vertex data
		//
		memcpy(pVertices+numV, patch->getVertices(), patch->getNumVertices()*sizeof(TerrainPatchBase::PATCHVERTEX));

		// then copy the indices, which must be updated to
		// reflect the moved vertex data (index shifts)
		//
		const WORD*	indices = patch->getIndices();
		int			n = patch->getNumIndices();

		if(lastIndex!=0xffff)				// add degen-tris to concenate strips
		{
			pIndices[numI++] = lastIndex;
			pIndices[numI++] = (unsigned short)(indices[0] + numV);
		}

		for(i=0; i<n; i++)
			pIndices[numI++] = (unsigned short)(indices[i] + numV);

		lastIndex = pIndices[numI-1];
		numV += patch->getNumVertices();
		numPatchesRendered++;

		patch = patch->getNextActive();
	}

	assert(numV == numVertices);
	assert(numI == numIndices);

	m_IndexBuffer->Unlock();
	m_VertexBuffer->Unlock();


	// finally draw the complete landscape with one single
	// DrawIndexedPrimitive() call !!!
	//
	pd3dDevice->SetVertexShader(D3DFVF_PATCHVERTEX);
	pd3dDevice->SetStreamSource(0, m_VertexBuffer, sizeof(TerrainPatchBase::PATCHVERTEX));
	pd3dDevice->SetIndices(m_IndexBuffer, 0);

	// draw the base texture with a lightmap (without alphablending)
	//
	//setupTextureStages(&basePassInfo, true);
	//pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, numVertices, 0, numIndices-2);

	// draw all additional passes
	//
	for(i=0; i<multiPassInfos.getSize(); i++)
	{
		setupTextureStages(multiPassInfos[i], false);
		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, numVertices, 0, numIndices-2);
	}

	if(skyPass.texColor)
	{
		setupTextureStagesForSky();
		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, numVertices, 0, numIndices-2);
	}

	pgIDirectX::addTerrainTris(numIndices-2);

	numPatchesPassesRendered = numPatchesRendered * (multiPassInfos.getSize()+1);
	*/
}


float
Terrain::getHeightDif(int nX0, int nY0, int nX1, int nY1)
{
	return getHeight(nX0,nY0) - getHeight(nX1,nY1);
}


void
Terrain::createNormals()
{
	/*NZ
	int						x,y, w=heightmap->getWidth(), h=heightmap->getHeight();

	if(normals)
		delete normals;

	normals = new Vector3[w*h];

	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
		{
			int		idx = y*w+x;
			float	dxp=0.0f,dxm=0.0f, dyp=0.0f,dym=0.0f;

			if(x>0 && x<w-1 && y>0 && y<w-1)
			{
				dxp =  getHeightDif(x,y, x+1,y);
				dxm =  getHeightDif(x,y, x-1,y);
				dyp =  getHeightDif(x,y, x,y+1);
				dym =  getHeightDif(x,y, x,y-1);
			}

			normals[idx].x() = ((dxp - dxm)/2.0f) * patchDX * TerrainPatchBase::SOURCE_WIDTH;
			normals[idx].y() = patchDY;
			normals[idx].z() = ((dyp - dym)/2.0f) * patchDZ * TerrainPatchBase::SOURCE_HEIGHT;
			normals[idx].normalize();
		}

	normals[0] = normals[1+w];
	normals[w-1] = normals[w+w-2];

	normals[(h-1)*w] = normals[(h-2)*w+1];
	normals[(h-1)*w+w-1] = normals[(h-2)*w+w-2];

	for(x=1; x<w-1; x++)
	{
		normals[x] = normals[x+w];
		normals[x+(y-1)*w] = normals[x+(y-2)*w];
	}

	for(y=1; y<h-1; y++)
	{
		normals[y*w] = normals[y*w+1];
		normals[y*w+w-1] = normals[y*w+w-2];
	}
	*/
}


void Terrain::deleteDeviceObjects()
{
	m_IndexBuffer.release();
	m_VertexBuffer.release();

	if (!patches)
		return;

	for (int y = 0; y < patchesY; y++)
	{
		for (int x = 0; x < patchesX; x++)
		{
			if (patches[y][x])
				patches[y][x]->deleteDeviceObjects();
		}
	}
}

bool Terrain::restoreDeviceObjects()
{
	forceBufferCreate = true;

	if (!patches)
		return true;

	for (int y = 0; y < patchesY; y++)
	{
		for(int x = 0; x < patchesX; x++)
		{
			if (patches[y][x])
				patches[y][x]->restoreDeviceObjects();
		}
	}

	return true;
}

/*NZ
void
Terrain::fillInfoString(pgString& nStr)
{
	float factor = (float)numPatchesPassesRendered/(float)numPatchesRendered;
	nStr.set("Terr: %d processed  %d (%d passes -> x%.2f) rendered", numPatchesProcessed, numPatchesRendered, numPatchesPassesRendered, factor);
}

void
Terrain::fillInfoString2(pgString& nStr)
{
	nStr.set("%d %d %d %d %d | %d", patchCount[0], patchCount[1], patchCount[2], patchCount[3], patchCount[4], bufferCreateCount);
	bufferCreateCount = 0;
}*/

// this method is not yet working correctly...
//
bool
Terrain::intersectLine(const Vector3& nPos0, const Vector3& nPos1, float& nScalarPos) const
{
	/*NZ
	D3DXMATRIX	mat;
	D3DXVECTOR3	p0,p1, tp0,tp1, ds, p;
	float		len, flatLen, f = 0.0f, h;
	bool		startAbove=false, curAbove=false, startAboveInit=false;

	D3DXMatrixInverse(&mat, NULL, &worldMatrix);

	pgIDirectX::setD3DVecFromVec3(p0, nPos0);
	pgIDirectX::setD3DVecFromVec3(p1, nPos1);

	D3DXVec3TransformCoord(&tp0, &p0, &mat);
	D3DXVec3TransformCoord(&tp1, &p1, &mat);

	tp0.x /= sizeX;		tp0.y /= sizeY;		tp0.z /= sizeZ;
	tp1.x /= sizeX;		tp1.y /= sizeY;		tp1.z /= sizeZ;

	if((tp0.x<0.0f && tp1.x<0.0f) || (tp0.x>1.0f && tp1.x>1.0f) ||
	   (tp0.z<0.0f && tp1.z<0.0f) || (tp0.z>1.0f && tp1.z>1.0f))
	   return false;

	ds = tp1-tp0;
	//ds.x *= 255.0f;
	//ds.z *= 255.0f;
	flatLen = (float)(sqrt(ds.x*ds.x + ds.z*ds.z));
	if(flatLen>0.0f)
	{
		ds.x /= flatLen;
		ds.y /= (255.0f*flatLen);
		ds.z /= flatLen;
	}
	len = (float)(sqrt(ds.x*ds.x + ds.y*ds.y + ds.z*ds.z));
	p = tp0;

	do
	{
		if(p.x>=0.0f && p.x<=hmWidth && p.z>=0.0f && p.z<=hmHeight)
		{
			if(!startAboveInit)
			{
				startAboveInit = true;
				startAbove = getHeight((int)p.x, (int)p.z) < p.y;
			}

			p += ds;
			f += 1.0f;

			h = getHeight((int)p.x, (int)p.z);
			curAbove = h < p.y;

			if(startAboveInit && curAbove!=startAbove)
			{
				//nScalarPos = (p-tp0).GetLength() / (tp1-tp0).length();
				nScalarPos = 0.0f;
				return true;
			}
		}
		else
		{
			p += ds;
			f += 1.0f;
		}
	} while(f<flatLen);
	*/
	return false;
}


bool
Terrain::projectDown(const Vector3& nPos, Vector3& nProjected) const
{
	/*NZ
	D3DXMATRIX	mat;
	D3DXVECTOR3	p, tp;

	D3DXMatrixInverse(&mat, NULL, &worldMatrix);

	pgIDirectX::setD3DVecFromVec3(p, nPos);

	D3DXVec3TransformCoord(&tp, &p, &mat);

	tp.x /= sizeX;		tp.y /= sizeY;		tp.z /= sizeZ;

	float	x = hmWidth*tp.x,
			y = hmHeight*tp.z;

	x -= 0.5f;
	y -= 0.5f;

	int		ix = (int)x,
			iy = (int)y;
	float	u = x - ix,
			v = y - iy;

	if(ix<0 || iy<0 || ix>=hmWidth-1 || iy>=hmHeight-1)
		return false;

	float	h00 = getHeight(ix+0, iy+0),
			h10 = getHeight(ix+1, iy+0),
			h01 = getHeight(ix+0, iy+1),
			h11 = getHeight(ix+1, iy+1);

	tp.y = (1.0f-v) * ((1.0f-u)*h00 + u*h10) + 
				 v  * ((1.0f-u)*h01 + u*h11);

	tp.x *= sizeX;		tp.y *= sizeY;		tp.z *= sizeZ;

	D3DXVec3TransformCoord(&p, &tp, &worldMatrix);
	pgIDirectX::setVec3FromVecD3D(nProjected, p);
	*/
	return true;
}


bool
Terrain::moveAboveSurface(Vector3& nPos, float nHeight, float /*nGlideFactor*/) const
{
	Vector3 downPos;

	if(projectDown(nPos, downPos))
	{
		if(nPos.y() < downPos.y() + nHeight)
		{
			nPos.y() = downPos.y() + nHeight;
			return true;
		}
	}

	return false;
}

/*NZ
Terrain::HeightMapRenderMode
Terrain::getRenderModeFromString(const pgString& nString)
{
	if(nString=="UnifiedRender")
		return UnifiedRender;
	if(nString=="SplitRender")
		return SplitRender;
	if(nString=="SoftwareMorphRender")
		return SoftwareMorphRender;
	if(nString=="HardwareMorphRender")
		return HardwareMorphRender;

	return SplitRender;	// default value
}
*/


TerrainPatchBase * Terrain::createPatch(int gridX, int gridY)
{
	switch (m_RenderMode)
	{
	case UnifiedRender:
		return new TerrainPatchBase(this, gridX, gridY);

	case SoftwareMorphRender:
		return new TerrainPatchMorphSW(this, gridX, gridY);

	case HardwareMorphRender:
		return new TerrainPatchMorphHW(this, gridX, gridY);

	case SplitRender:
	default:
		return new TerrainPatch(this, gridX, gridY);
	}
}
