///////////////////////////////////////////////////////////////////
//
// project:     Pluto Game Engine
// class:       TerrainPatchMorphSW
// creator:     Daniel Wagner
// description: 
//
///////////////////////////////////////////////////////////////////
// $Id: TerrainPatchMorphSW.cpp,v 1.13 2002/12/22 10:51:32 daniel Exp $
///////////////////////////////////////////////////////////////////

#include "patch_morph_sw.h"
#include "terrain.h"


TerrainPatchMorphSW::TerrainPatchMorphSW(Terrain* nMaster, int nX, int nY) : TerrainPatchBase(nMaster, nX,nY)
{
	/*NZ
	vBuffer = NULL;
	iBuffer = NULL;
	*/
	verticesLow = NULL;
	yMoveSelf = yMoveLeft = yMoveLeft2 = yMoveRight = yMoveRight2 = yMoveBottom = yMoveBottom2 = yMoveTop = yMoveTop2 = NULL;
	newFollowsLeft = followsLeft = newFollowsRight = followsRight = newFollowsBottom = followsBottom = newFollowsTop = followsTop = false;
	recalcBorderLeft = recalcBorderRight = recalcBorderBottom = recalcBorderTop = false;
	forceRetessellation = false;
}


void
TerrainPatchMorphSW::init()
{
	TerrainPatchBase::init();
	verticesLow = new PatchVertex[MAX_VERTICES];
	yMoveSelf = new float[MAX_VERTICES];
	yMoveLeft = new float[MAX_VERTICES];
	yMoveLeft2 = new float[MAX_VERTICES];
	yMoveRight = new float[MAX_VERTICES];
	yMoveRight2 = new float[MAX_VERTICES];
	yMoveTop = new float[MAX_VERTICES];
	yMoveTop2 = new float[MAX_VERTICES];
	yMoveBottom = new float[MAX_VERTICES];
	yMoveBottom2 = new float[MAX_VERTICES];
	newFollowsLeft = followsLeft = newFollowsRight = followsRight = newFollowsBottom = followsBottom = newFollowsTop = followsTop = false;
	recalcBorderLeft = recalcBorderRight = recalcBorderBottom = recalcBorderTop = false;
	forceRetessellation = false;
}


void
TerrainPatchMorphSW::deinit()
{
	TerrainPatchBase::deinit();

	if(verticesLow)
		delete verticesLow;
	verticesLow = NULL;

	if(yMoveSelf)
		delete yMoveSelf;
	yMoveSelf = NULL;

	if(yMoveLeft)
		delete yMoveLeft;
	yMoveLeft = NULL;

	if(yMoveLeft2)
		delete yMoveLeft2;
	yMoveLeft2 = NULL;

	if(yMoveRight)
		delete yMoveRight;
	yMoveRight = NULL;

	if(yMoveRight2)
		delete yMoveRight2;
	yMoveRight2 = NULL;

	if(yMoveBottom)
		delete yMoveBottom;
	yMoveBottom = NULL;

	if(yMoveBottom2)
		delete yMoveBottom2;
	yMoveBottom2 = NULL;

	if(yMoveTop)
		delete yMoveTop;
	yMoveTop = NULL;

	if(yMoveTop2)
		delete yMoveTop2;
	yMoveTop2 = NULL;

	deleteDeviceObjects();
}


void
TerrainPatchMorphSW::deleteDeviceObjects()
{
	/*NZ
	SAFE_RELEASE(iBuffer);
	SAFE_RELEASE(vBuffer);
	*/
}


bool
TerrainPatchMorphSW::restoreDeviceObjects()
{
	m_ForceBufferCreate = true;
	return true;
}


bool
TerrainPatchMorphSW::updateTessellation()
{
	TerrainPatchMorphSW* leftSW = (TerrainPatchMorphSW*)m_Left;
	TerrainPatchMorphSW* rightSW = (TerrainPatchMorphSW*)m_Right;
	TerrainPatchMorphSW* bottomSW = (TerrainPatchMorphSW*)m_Bottom;
	TerrainPatchMorphSW* topSW = (TerrainPatchMorphSW*)m_Top;

	if(leftSW && leftSW->m_OldRealSelfTesselation<5 && leftSW->m_NewRealSelfTesselation!=leftSW->m_OldRealSelfTesselation)
		recalcBorderLeft = true;

	if(rightSW && rightSW->m_OldRealSelfTesselation<5 && rightSW->m_NewRealSelfTesselation!=rightSW->m_OldRealSelfTesselation)
		recalcBorderRight = true;

	if(bottomSW && bottomSW->m_OldRealSelfTesselation<5 && bottomSW->m_NewRealSelfTesselation!=bottomSW->m_OldRealSelfTesselation)
		recalcBorderBottom = true;

	if(topSW && topSW->m_OldRealSelfTesselation<5 && topSW->m_NewRealSelfTesselation!=topSW->m_OldRealSelfTesselation)
		recalcBorderTop = true;

	if(TerrainPatchBase::updateTessellation())
		m_ForceBufferCreate = true;

	return false;
}


bool
TerrainPatchMorphSW::updateTessellation2()
{
	float	selfTesEx = getExactTessellation(),
			leftTesEx = (m_Left && m_Left->isActive()) ? m_Left->getExactTessellation() : -1.0f,
			rightTesEx = (m_Right && m_Right->isActive()) ? m_Right->getExactTessellation() : -1.0f,
			bottomTesEx = (m_Bottom && m_Bottom->isActive()) ? m_Bottom->getExactTessellation() : -1.0f,
			topTesEx = (m_Top && m_Top->isActive()) ? m_Top->getExactTessellation() : -1.0f;

	newFollowsLeft = (selfTesEx<leftTesEx) || (selfTesEx==leftTesEx&& !((TerrainPatchMorphSW*)m_Left)->newFollowsRight);
	newFollowsRight = (selfTesEx<rightTesEx) || (selfTesEx==rightTesEx && !((TerrainPatchMorphSW*)m_Right)->newFollowsLeft);
	newFollowsBottom = (selfTesEx<bottomTesEx) || (selfTesEx==bottomTesEx && !((TerrainPatchMorphSW*)m_Bottom)->newFollowsTop);
	newFollowsTop = (selfTesEx<topTesEx) || (selfTesEx==topTesEx && !((TerrainPatchMorphSW*)m_Top)->newFollowsBottom);


	// check if this patch does no longer follow a neighbor
	// if there was no retessellation due to some other reason, we have to do one NOW
	if(!m_ForceBufferCreate)
		if((!newFollowsLeft && followsLeft) || (!newFollowsRight && followsRight) ||
		   (!newFollowsBottom && followsBottom) || (!newFollowsTop && followsTop))
		{
			Q_ASSERT(m_SelfTesselation - m_LeftTesselation <= 1);
			Q_ASSERT(m_SelfTesselation - m_RightTesselation <= 1);
			Q_ASSERT(m_SelfTesselation - m_BottomTesselation <= 1);
			Q_ASSERT(m_SelfTesselation - m_TopTesselation <= 1);

			createTessellation(m_SelfTesselation, m_LeftTesselation, m_RightTesselation,
				m_BottomTesselation, m_TopTesselation);
			m_NumVertices = m_NumNewVertices;
			m_NumIndices = m_NumNewIndices;
			forceRetessellation = true;
		}

	fillMorphVertices();
	return false;
}


bool
TerrainPatchMorphSW::updateTessellation3()
{
	checkBorderLeft();
	checkBorderRight();
	checkBorderBottom();
	checkBorderTop();

	fillBuffers();

	followsLeft = newFollowsLeft;
	followsRight = newFollowsRight;
	followsBottom = newFollowsBottom;
	followsTop = newFollowsTop;

	recalcBorderLeft = false;
	recalcBorderRight = false;
	recalcBorderBottom = false;
	recalcBorderTop = false;
	forceRetessellation = false;

	return false;
}


int
TerrainPatchMorphSW::render()
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


	// draw the patch with one single D3D call
	pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, numVertices, 0, numIndices-2);
	*/
	return m_NumIndices-2;
}


void
TerrainPatchMorphSW::fillMorphVertices()
{
	if(m_ForceBufferCreate || forceRetessellation)
	{
		memcpy(verticesLow, m_Vertices, m_NumNewVertices*sizeof(PatchVertex));

		if(m_RealSelfTesselation<4)
		{
			reduceShapeTo(m_RealSelfTesselation+1, verticesLow);
			makeBordersSimpler(verticesLow);
		}

		//if(m_SelfTesselation==m_RealSelfTesselation+1)
		//	memcpy(verticesLow, vertices, numNewVertices*sizeof(PATCHVERTEX));

		for(int i=0; i<m_NumNewVertices; i++)
			yMoveSelf[i] = m_Vertices[i].pos.y() - verticesLow[i].pos.y();
	}
}


void
TerrainPatchMorphSW::checkBorderLeft()
{
	int i, y,yc, pow;

	if(m_ForceBufferCreate || forceRetessellation || followsLeft!=newFollowsLeft)
	{
		for(i=0; i<m_NumNewVertices; i++)
			yMoveLeft[i] = 0.0f;

		for(i=0; i<m_NumNewVertices; i++)
			yMoveLeft2[i] = 0.0f;
	}


	if(newFollowsLeft && (!followsLeft || m_ForceBufferCreate || forceRetessellation || recalcBorderLeft))
	{
		const TerrainPatchMorphSW* leftSW = reinterpret_cast<const TerrainPatchMorphSW*>(m_Left);

		// copy shared border column
		//
		Q_ASSERT(m_SelfTesselation==leftSW->m_RightTesselation);
		Q_ASSERT(m_LeftTesselation==leftSW->m_SelfTesselation);
		Q_ASSERT(leftSW->m_IndexMap);

		pow = getPowerTwo(Q_MIN(m_SelfTesselation,m_LeftTesselation));

		for(y=pow; y<SOURCE_HEIGHT-1; y+=pow)
		{
			int idx = m_IndexMap[0 + y * SOURCE_WIDTH],
				idxL = leftSW->m_IndexMap[SOURCE_WIDTH-1 + y * SOURCE_WIDTH];
			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxL!=0xffff);

			verticesLow[idx].pos.y() = leftSW->verticesLow[idxL].pos.y();
			m_Vertices[idx].pos.y() = leftSW->m_Vertices[idxL].pos.y();
			yMoveLeft[idx] = leftSW->yMoveSelf[idxL];
			yMoveSelf[idx] = 0.0f;
		}


		if(m_SelfTesselation>=3)
			return;

		// correct border neighbors
		//
		pow = getPowerTwo(m_SelfTesselation);

		for(y=pow,yc=1; y<SOURCE_HEIGHT-1; y+=pow,yc++)
		{
			float yLT=0.0f,yRB=0.0f;
			int idx, idxLT=0,idxRB=0;

			idx = calculateDiagonalVertices(pow, y, 1, yc, pow, verticesLow, yLT,idxLT, yRB,idxRB);
			Q_ASSERT(idx>=0 && idx!=0xffff);

			float moveLT = m_Vertices[idxLT].pos.y() - verticesLow[idxLT].pos.y();

			verticesLow[idx].pos.y() = (yLT + yRB) / 2.0f;

			// our vertex is moved 50% by the left (-top) neighbor
			// this garanties that this vertex is on the diagonal when currentError is 0.0f
			yMoveLeft2[idx] = moveLT * 0.5f;

			yMoveSelf[idx] = m_Vertices[idx].pos.y() - verticesLow[idx].pos.y();
		}


		// correct neighbors to lie on a diagonal
		//
		if(m_SelfTesselation!=m_RealSelfTesselation)
		{
			pow = getPowerTwo(m_SelfTesselation);

			for(y=pow,yc=1; y<SOURCE_HEIGHT-1; y+=pow,yc++)
			{
				float yLT=0.0f,yRB=0.0f;
				int idx, idxLT=0,idxRB=0;

				idx = calculateDiagonalVertices(pow, y, 1, yc, pow, verticesLow, yLT,idxLT, yRB,idxRB);
				Q_ASSERT(idx>=0 && idx!=0xffff);

				verticesLow[idx].pos.y() = (yLT + yRB) * 0.5f;
				m_Vertices[idx].pos.y() = (m_Vertices[idxLT].pos.y() + m_Vertices[idxRB].pos.y()) * 0.5f;

				// the real new part: this vertex always STAYS on the diagonal
				//
				yMoveLeft2[idx] = 0.0f;
				yMoveLeft[idx] = (m_Vertices[idxLT].pos.y() - verticesLow[idxLT].pos.y()) * 0.5f;
				yMoveSelf[idx] = (m_Vertices[idxRB].pos.y() - verticesLow[idxRB].pos.y()) * 0.5f;
			}

			// necessary to recorrect the shared right-top border neighbor
			//
			if(followsBottom)
				recalcBorderBottom = true;
		}
	}
}


void
TerrainPatchMorphSW::checkBorderRight()
{
	int i, y,yc, pow, xcMax;

	if(m_ForceBufferCreate || forceRetessellation || followsRight!=newFollowsRight)
	{
		for(i=0; i<m_NumNewVertices; i++)
			yMoveRight[i] = 0.0f;

		for(i=0; i<m_NumNewVertices; i++)
			yMoveRight2[i] = 0.0f;
	}

	if(newFollowsRight && (!followsRight || m_ForceBufferCreate || forceRetessellation || recalcBorderRight))
	{
		const TerrainPatchMorphSW* rightSW = reinterpret_cast<const TerrainPatchMorphSW*>(m_Right);

		// copy shared border column
		//
		Q_ASSERT(m_SelfTesselation==rightSW->m_LeftTesselation);
		Q_ASSERT(m_RightTesselation==rightSW->m_SelfTesselation);
		Q_ASSERT(rightSW->m_IndexMap);

		pow = getPowerTwo(Q_MIN(m_SelfTesselation,m_RightTesselation));

		for(y=pow; y<SOURCE_HEIGHT-1; y+=pow)
		{
			int idx = m_IndexMap[SOURCE_WIDTH-1 + y * SOURCE_WIDTH],
				idxR = rightSW->m_IndexMap[0 + y * SOURCE_WIDTH];
			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxR!=0xffff);

			verticesLow[idx].pos.y() = rightSW->verticesLow[idxR].pos.y();
			m_Vertices[idx].pos.y() = rightSW->m_Vertices[idxR].pos.y();
			yMoveRight[idx] = rightSW->yMoveSelf[idxR];
			yMoveSelf[idx] = 0.0f;
		}


		// correct border neighbors
		//
		if(m_RealSelfTesselation<3)
		{
			pow = getPowerTwo(m_RealSelfTesselation);
			xcMax = (SOURCE_WIDTH-1-pow)/pow;

			for(y=pow,yc=1; y<SOURCE_HEIGHT-1; y+=pow,yc++)
			{
				float yLT=0.0f,yRB=0.0f;
				int idx, idxLT=0,idxRB=0;

				idx = calculateDiagonalVertices(SOURCE_WIDTH-1-pow, y, xcMax, yc, pow, verticesLow, yLT,idxLT, yRB,idxRB);
				Q_ASSERT(idx>=0 && idx!=0xffff);

				verticesLow[idx].pos.y() = (yLT + yRB) * 0.5f;

				// our vertex is moved 50% by the right (-bottom) neighbor
				// this garanties that this vertex is on the diagonal when currentError is 0.0f
				yMoveRight2[idx] = (m_Vertices[idxRB].pos.y() - verticesLow[idxRB].pos.y()) * 0.5f;

				yMoveSelf[idx] = m_Vertices[idx].pos.y() - verticesLow[idx].pos.y();
			}
		}


		// correct neighbors to lie on a diagonal
		//
		if(m_SelfTesselation!=m_RealSelfTesselation)
		{
			pow = getPowerTwo(m_SelfTesselation);
			xcMax = (SOURCE_WIDTH-1-pow)/pow;

			for(y=pow,yc=1; y<SOURCE_HEIGHT-1; y+=pow,yc++)
			{
				float yLT=0.0f,yRB=0.0f;
				int idx, idxLT=0,idxRB=0;

				idx = calculateDiagonalVertices(SOURCE_WIDTH-1-pow, y, xcMax, yc, pow, verticesLow, yLT,idxLT, yRB,idxRB);
				Q_ASSERT(idx>=0 && idx!=0xffff);

				verticesLow[idx].pos.y() = (yLT + yRB) * 0.5f;
				m_Vertices[idx].pos.y() = (m_Vertices[idxLT].pos.y() + m_Vertices[idxRB].pos.y()) * 0.5f;

				// the real new part: this vertex always STAYS on the diagonal
				//
				yMoveRight2[idx] = 0.0f;
				yMoveRight[idx] = (m_Vertices[idxRB].pos.y() - verticesLow[idxRB].pos.y()) * 0.5f;
				yMoveSelf[idx] = (m_Vertices[idxLT].pos.y() - verticesLow[idxLT].pos.y()) * 0.5f;
			}

			// necessary to recorrect the shared right-top border neighbor
			//
			if(followsTop)
				recalcBorderTop = true;
		}
	}
}

void
TerrainPatchMorphSW::checkBorderBottom()
{
	int i, x,xc, pow;

	if(m_ForceBufferCreate || forceRetessellation || followsBottom!=newFollowsBottom)
	{
		for(i=0; i<m_NumNewVertices; i++)
			yMoveBottom[i] = 0.0f;

		for(i=0; i<m_NumNewVertices; i++)
			yMoveBottom2[i] = 0.0f;
	}


	if(newFollowsBottom && (!followsBottom || m_ForceBufferCreate || forceRetessellation || recalcBorderBottom))
	{
		const TerrainPatchMorphSW* bottomSW = reinterpret_cast<const TerrainPatchMorphSW*>(m_Bottom);

		// copy shared border column
		//
		Q_ASSERT(m_SelfTesselation==bottomSW->m_TopTesselation);
		Q_ASSERT(m_BottomTesselation==bottomSW->m_SelfTesselation);
		Q_ASSERT(bottomSW->m_IndexMap);

		pow = getPowerTwo(Q_MIN(m_SelfTesselation,m_BottomTesselation));

		for(x=pow; x<SOURCE_WIDTH-1; x+=pow)
		{
			int idx = m_IndexMap[x + 0 * SOURCE_WIDTH],
				idxB = bottomSW->m_IndexMap[x + (SOURCE_WIDTH-1) * SOURCE_WIDTH];
			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxB!=0xffff);

			verticesLow[idx].pos.y() = bottomSW->verticesLow[idxB].pos.y();
			m_Vertices[idx].pos.y() = bottomSW->m_Vertices[idxB].pos.y();
			yMoveBottom[idx] = bottomSW->yMoveSelf[idxB];
			yMoveSelf[idx] = 0.0f;
		}


		if(m_SelfTesselation>=3)
			return;

		// correct border neighbors
		//
		pow = getPowerTwo(m_SelfTesselation);

		for(x=pow,xc=1; x<SOURCE_WIDTH-1; x+=pow,xc++)
		{
			float yLT=0.0f,yRB=0.0f;
			int idx, idxLT=0,idxRB=0;

			idx = calculateDiagonalVertices(x, pow, xc, 1, pow, verticesLow, yLT,idxLT, yRB,idxRB);
			Q_ASSERT(idx>=0 && idx!=0xffff);

			float moveRB = m_Vertices[idxRB].pos.y() - verticesLow[idxRB].pos.y();

			verticesLow[idx].pos.y() = (yLT + yRB) / 2.0f;

			// our vertex is moved 50% by the right (-bottom) neighbor
			// this garanties that this vertex is on the diagonal when currentError is 0.0f
			yMoveBottom2[idx] = moveRB * 0.5f;

			yMoveSelf[idx] = m_Vertices[idx].pos.y() - verticesLow[idx].pos.y();
		}


		// correct neighbors to lie on a diagonal
		//
		if(m_SelfTesselation!=m_RealSelfTesselation)
		{
			pow = getPowerTwo(m_SelfTesselation);

			for(x=pow,xc=1; x<SOURCE_WIDTH-1; x+=pow,xc++)
			{
				float yLT=0.0f,yRB=0.0f;
				int idx, idxLT=0,idxRB=0;

				idx = calculateDiagonalVertices(x, pow, xc, 1, pow, verticesLow, yLT,idxLT, yRB,idxRB);
				Q_ASSERT(idx>=0 && idx!=0xffff);

				verticesLow[idx].pos.y() = (yLT + yRB) * 0.5f;
				m_Vertices[idx].pos.y() = (m_Vertices[idxLT].pos.y() + m_Vertices[idxRB].pos.y()) * 0.5f;

				// the real new part: this vertex always STAYS on the diagonal
				//
				yMoveBottom2[idx] = 0.0f;
				yMoveBottom[idx] = (m_Vertices[idxRB].pos.y() - verticesLow[idxRB].pos.y()) * 0.5f;

				if(yMoveLeft[idx]==0.0f)
					yMoveSelf[idx] = (m_Vertices[idxLT].pos.y() - verticesLow[idxLT].pos.y()) * 0.5f;
				else
					yMoveSelf[idx] = 0.0f;
			}
		}
	}
}


void
TerrainPatchMorphSW::checkBorderTop()
{
	int i, x,xc, pow, ycMax;

	if(m_ForceBufferCreate || forceRetessellation || followsTop!=newFollowsTop)
	{
		for(i=0; i<m_NumNewVertices; i++)
			yMoveTop[i] = 0.0f;

		for(i=0; i<m_NumNewVertices; i++)
			yMoveTop2[i] = 0.0f;
	}

	if(newFollowsTop && (!followsTop || m_ForceBufferCreate || forceRetessellation || recalcBorderTop))
	{
		const TerrainPatchMorphSW* topSW = reinterpret_cast<const TerrainPatchMorphSW*>(m_Top);

		// copy shared border column
		//
		Q_ASSERT(m_SelfTesselation==topSW->m_BottomTesselation);
		Q_ASSERT(m_TopTesselation==topSW->m_SelfTesselation);
		Q_ASSERT(topSW->m_IndexMap);

		pow = getPowerTwo(Q_MIN(m_SelfTesselation,m_TopTesselation));

		for(x=pow; x<SOURCE_WIDTH-1; x+=pow)
		{
			int idx = m_IndexMap[x + (SOURCE_HEIGHT-1) * SOURCE_WIDTH],
				idxT = topSW->m_IndexMap[x + 0 * SOURCE_WIDTH];
			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idxT!=0xffff);

			verticesLow[idx].pos.y() = topSW->verticesLow[idxT].pos.y();
			m_Vertices[idx].pos.y() = topSW->m_Vertices[idxT].pos.y();
			yMoveTop[idx] = topSW->yMoveSelf[idxT];
			yMoveSelf[idx] = 0.0f;
		}


		if(m_SelfTesselation>=3)
			return;

		// correct border neighbors
		//
		pow = getPowerTwo(m_SelfTesselation);
		ycMax = (SOURCE_WIDTH-1-pow)/pow;

		for(x=pow,xc=1; x<SOURCE_WIDTH-1; x+=pow,xc++)
		{
			float yLT=0.0f,yRB=0.0f;
			int idx, idxLT=0,idxRB=0;

			idx = calculateDiagonalVertices(x, SOURCE_HEIGHT-1-pow, xc, ycMax, pow, verticesLow, yLT,idxLT, yRB,idxRB);
			Q_ASSERT(idx>=0 && idx!=0xffff);

			float moveLT = m_Vertices[idxLT].pos.y() - verticesLow[idxLT].pos.y();

			verticesLow[idx].pos.y() = (yLT + yRB) / 2.0f;

			// our vertex is moved 50% by the left (-top) neighbor
			// this garanties that this vertex is on the diagonal when currentError is 0.0f
			yMoveTop2[idx] = moveLT * 0.5f;

			yMoveSelf[idx] = m_Vertices[idx].pos.y() - verticesLow[idx].pos.y();
		}


		// correct neighbors to lie on a diagonal
		//
		if(m_SelfTesselation!=m_RealSelfTesselation)
		{
			pow = getPowerTwo(m_SelfTesselation);
			ycMax = (SOURCE_WIDTH-1-pow)/pow;

			for(x=pow,xc=1; x<SOURCE_WIDTH-1; x+=pow,xc++)
			{
				float yLT=0.0f,yRB=0.0f;
				int idx, idxLT=0,idxRB=0;

				idx = calculateDiagonalVertices(x, SOURCE_HEIGHT-1-pow, xc, ycMax, pow, verticesLow, yLT,idxLT, yRB,idxRB);
				Q_ASSERT(idx>=0 && idx!=0xffff);

				verticesLow[idx].pos.y() = (yLT + yRB) * 0.5f;
				m_Vertices[idx].pos.y() = (m_Vertices[idxLT].pos.y() + m_Vertices[idxRB].pos.y()) * 0.5f;

				// the real new part: this vertex always STAYS on the diagonal
				//
				yMoveTop2[idx] = 0.0f;
				yMoveTop[idx] = (m_Vertices[idxLT].pos.y() - verticesLow[idxLT].pos.y()) * 0.5f;

				if(yMoveRight[idx]==0.0f)
					yMoveSelf[idx] = (m_Vertices[idxRB].pos.y() - verticesLow[idxRB].pos.y()) * 0.5f;
				else
					yMoveSelf[idx] = 0.0f;

			}
		}
	}
}


bool
TerrainPatchMorphSW::fillBuffers()
{
	/*NZ
	if(!checkDevice("cannot create terrainpatch: no device set"))
		return false;

	if(!checkBuffers())
		return false;


	if(m_NumVertices==0 || m_NumIndices==0)					// nothing to do ???
		return true;


	// fill vertex buffer
	//
	PatchVertex *	pVertices = NULL;
	vBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);

	if(m_SelfTesselation>0 || m_SelfTesselation<MAX_SUBDIV)
	{
		int		i;

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

		// clamping for patches which use m_SelfTesselation!=m_RealSelfTesselation
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

		float	factorLeft2 = (1.0f-factorSelf) * factorLeft,
				factorRight2 = (1.0f-factorSelf) * factorRight,
				factorBottom2 = (1.0f-factorSelf) * factorBottom,
				factorTop2 = (1.0f-factorSelf) * factorTop;


		// either this patch has to follow its neighbors or the other way around
		//
		Q_ASSERT(!(left && left->isActive()) || (newFollowsLeft!=reinterpret_cast<const TerrainPatchMorphSW*>(left)->newFollowsRight));
		Q_ASSERT(!(right && right->isActive()) || (newFollowsRight!=reinterpret_cast<const TerrainPatchMorphSW*>(right)->newFollowsLeft));
		Q_ASSERT(!(bottom && bottom->isActive()) || (newFollowsBottom!=reinterpret_cast<const TerrainPatchMorphSW*>(bottom)->newFollowsTop));
		Q_ASSERT(!(top && top->isActive()) || (newFollowsTop!=reinterpret_cast<const TerrainPatchMorphSW*>(top)->newFollowsBottom));


		for(i=0; i<numVertices; i++)
		{
			pVertices[i].tex = verticesLow[i].tex;
			pVertices[i].pos.x = verticesLow[i].pos.x;
			pVertices[i].pos.z = verticesLow[i].pos.z;

			pVertices[i].pos.y = verticesLow[i].pos.y + factorSelf * yMoveSelf[i]
							   + factorLeft * yMoveLeft[i] + factorLeft2 * yMoveLeft2[i]
							   + factorRight * yMoveRight[i] + factorRight2 * yMoveRight2[i]
							   + factorBottom * yMoveBottom[i] + factorBottom2 * yMoveBottom2[i]
							   + factorTop * yMoveTop[i] + factorTop2 * yMoveTop2[i];
		}

	}
	else
		memcpy(pVertices, vertices, numVertices*sizeof(PATCHVERTEX));

	vBuffer->Unlock();


	// fill index buffer
	//
	WORD*			pIndices = NULL;

	iBuffer->Lock(0, 0, (BYTE**)&pIndices, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);
	memcpy(pIndices, indices, numIndices*sizeof(unsigned short));
	//for(int i=0; i<numIndices; i++)
	//	pgLog::trace("%d: %d\n", i, indices[i]);
	iBuffer->Unlock();
	*/
	return true;
}


bool
TerrainPatchMorphSW::checkBuffers()
{
	/*NZ
	// create new buffers if the size changed
	//
	if(numIndices!=numNewIndices || m_ForceBufferCreate)
	{
		numIndices = numNewIndices;
		SAFE_RELEASE(iBuffer)

		if(numIndices>0)
			if(FAILED(pd3dDevice->CreateIndexBuffer(numIndices * sizeof(WORD), D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
													D3DFMT_INDEX16, pgIDirectX::getPoolType(), &iBuffer)))
			{
				pgLog::error("creating index buffer for terrainpatch failed");
				return false;
			}
	}


	if(numVertices!=numNewVertices  || m_ForceBufferCreate)
	{
		numVertices = numNewVertices;
		SAFE_RELEASE(vBuffer)

		if(numVertices>0)
			if(FAILED(pd3dDevice->CreateVertexBuffer(numVertices * sizeof(PATCHVERTEX), D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
													 D3DFVF_PATCHVERTEX, pgIDirectX::getPoolType(), &vBuffer)))
			{
				pgLog::error("creating vertex buffer for terrainpatch failed");
				SAFE_RELEASE(iBuffer);
				return false;
			}
	}
	*/

	m_ForceBufferCreate = false;
	return true;
}


