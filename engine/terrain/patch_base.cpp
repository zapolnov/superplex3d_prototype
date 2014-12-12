#include "patch_base.h"
#include "terrain.h"

/* TerrainPatchBase methods */

//
// Constructor
//
TerrainPatchBase::TerrainPatchBase(Terrain * master, int gridX, int gridY)
	: m_VisFlags(0), m_Master(master), m_GridX(gridX), m_GridY(gridY), m_Active(false),
	  m_LowVertMethod2(false), m_CurrentError(-1.0f), m_NewSelfTesselation(5),
	  m_SelfTesselation(5), m_OldRealSelfTesselation(5), m_NewRealSelfTesselation(5),
	  m_RealSelfTesselation(5), m_LeftTesselation(5), m_RightTesselation(5),
	  m_TopTesselation(5), m_BottomTesselation(5), m_Scale(1.0f, 1.0f, 1.0f), m_MinY(-1.0f),
	  m_MaxY(-1.0f), m_Next(NULL), m_Prev(NULL), m_Left(NULL), m_Right(NULL), m_Top(NULL),
	  m_Bottom(NULL), m_IndexMap(NULL), m_Vertices(NULL), m_Indices(NULL)
{
}

//
// Initializes the patch
//
void TerrainPatchBase::init()
{
	m_NewSelfTesselation = 5;
	m_SelfTesselation = 5;
	m_LeftTesselation = 5;
	m_RightTesselation = 5;
	m_TopTesselation = 5;
	m_BottomTesselation = 5;
	m_OldRealSelfTesselation = 5;

	m_Vertices = new PatchVertex[MAX_VERTICES];
	m_Indices = new ushort[MAX_INDICES];
	m_IndexMap = new ushort[MAX_VERTICES];

	m_NumIndices = 0;
	m_NumVertices = 0;
	m_NumNewVertices = 0;
	m_NumNewIndices = 0;

	m_ForceBufferCreate = true;
	m_Active = true;

	addToActiveList();
}

//
// Frees used resources
//
void TerrainPatchBase::deinit()
{
	delete m_Vertices;
	m_Vertices = NULL;
	delete m_Indices;
	m_Indices = NULL;
	delete m_IndexMap;
	m_IndexMap = NULL;

	m_CurrentError = -1.0f;
	m_Active = false;

	removeFromActiveList();
}

//
// Renders the patch
//
int TerrainPatchBase::render()
{
	return 0;
}

//
// Updates the tesselation
//
bool TerrainPatchBase::updateTessellation()
{
	bool ret = false;

	int	lT = m_Left->getNewTessellationSafe();
	int rT = m_Right->getNewTessellationSafe();
	int bT = m_Bottom->getNewTessellationSafe();
	int tT = m_Top->getNewTessellationSafe();

	if (m_ForceBufferCreate || m_NewRealSelfTesselation != m_RealSelfTesselation ||
		m_NewSelfTesselation != m_SelfTesselation || m_LeftTesselation != lT ||
		m_RightTesselation != rT || m_BottomTesselation != bT || m_TopTesselation != tT)
	{
		m_RealSelfTesselation = m_NewRealSelfTesselation;
		m_SelfTesselation = m_NewSelfTesselation;

		m_LeftTesselation = lT;
		m_RightTesselation = rT;
		m_BottomTesselation = bT;
		m_TopTesselation = tT;

		Q_ASSERT(m_SelfTesselation - lT <= 1);
		Q_ASSERT(m_SelfTesselation - rT <= 1);
		Q_ASSERT(m_SelfTesselation - bT <= 1);
		Q_ASSERT(m_SelfTesselation - tT <= 1);

		createTessellation(m_SelfTesselation, m_LeftTesselation,
			m_RightTesselation, m_BottomTesselation, m_TopTesselation);
		m_ForceBufferCreate = false;

		ret = true;
	}

	float err0 = m_Errors[m_RealSelfTesselation].diff;
	float err1 = m_Errors[m_RealSelfTesselation + 1].diff;
	float maxerr = m_Master->getMaxError();

	if (m_RealSelfTesselation >= MAX_SUBDIV)
		err1 = 2.0f * err0;

	m_CurrentError = (maxerr - err0) / (err1 - err0);

	if (m_RealSelfTesselation >= 4 && m_CurrentError > 1.0f)
		m_CurrentError = 1.0f;

	Q_ASSERT(m_CurrentError <= 1.0f);

	return ret;
}

//
// Used in geomorphing
//
bool TerrainPatchBase::updateTessellation2()
{
	// if we want to do geo-morphing we need to create a second
	// vertex array which contains the same amount of vertices, but
	// looking as if they where one tessellation stage higher
	// since some tessellation methods might need results of neighbors
	// this is done in updateTessellation2+3() 
	return false;
}

//
// Used in geomorphing
//
bool TerrainPatchBase::updateTessellation3()
{
	return false;
}

//
// Destroys any OpenGL-related objects
//
void TerrainPatchBase::deleteDeviceObjects()
{
}

//
// Restores any OpenGL-related objects
//
bool TerrainPatchBase::restoreDeviceObjects()
{
	m_ForceBufferCreate = true;
	return true;
}

//
// Copies vertices into the specified buffer
//
int TerrainPatchBase::fillBaseVertices(PatchVertex * dest)
{
	Q_ASSERT(m_Vertices);

	memcpy(dest, m_Vertices, m_NumVertices * sizeof(PatchVertex));
	return m_NumVertices;
}

//
// Adds this patch to the list of active patches
//
void TerrainPatchBase::addToActiveList()
{
	Q_ASSERT(m_Master);
	Q_ASSERT(!m_Prev);
	Q_ASSERT(!m_Next);

	m_Prev = NULL;
	m_Next = m_Master->getActiveList();

	if (m_Next)
		m_Next->m_Prev = this;

	m_Master->getActiveList() = this;
}

//
// Removes this patch from the list of active patches
//
void TerrainPatchBase::removeFromActiveList()
{
	if (m_Prev)
		m_Prev->m_Next = m_Next;
	if (m_Next)
		m_Next->m_Prev = m_Prev;

	if (m_Master->getActiveList() == this)
	{
		Q_ASSERT(!m_Prev);
		m_Master->getActiveList() = m_Next;
	}

	m_Prev = m_Next = NULL;
}

//
// Returns current exact tesselation level of this patch
//
float TerrainPatchBase::getExactTessellation() const
{
	if (m_RealSelfTesselation < 0)
		return -1.0f;

	return (m_CurrentError < 0.0f ? (float)m_RealSelfTesselation
		: (float)m_RealSelfTesselation + m_CurrentError);
}

//
// Checks whether the patch is visible on the screen (clipping)
//
bool TerrainPatchBase::checkVisibility(const CameraPtr & camera)
{
	updateVisibility(camera);
	setVisible(m_Visible);

	return m_Visible;
}

//
//
// visibility testing is done by checking all 8 corners of the bounding box
// against all six sides of the viewing frustum
void TerrainPatchBase::updateVisibility(const CameraPtr & camera)
{
	m_Visible = true;

	/*
	D3DXVECTOR3	pt0,pt1,pt2,pt3,pt4,pt5,pt6,pt7, tpt0,tpt1,tpt2,tpt3,tpt4,tpt5,tpt6,tpt7, tp0,tp1,tp2,tp3;
	const D3DXMATRIX&	mat = *nMatrix;

	//visible = true;
	//return;

	// create and transform the cubes corners vertices
	//
	pt0.x = pos.x;				pt0.y = pos.y + minY;		pt0.z = pos.z;
	pt1.x = pos.x + scale.x;	pt1.y = pos.y + minY;		pt1.z = pos.z;
	pt2.x = pos.x;				pt2.y = pos.y + minY;		pt2.z = pos.z + scale.z;
	pt3.x = pos.x + scale.x;	pt3.y = pos.y + minY;		pt3.z = pos.z + scale.z;
	pt4.x = pt0.x;				pt4.y = pt0.y + maxY;		pt4.z = pt0.z;
	pt5.x = pt1.x;				pt5.y = pt1.y + maxY;		pt5.z = pt1.z;
	pt6.x = pt2.x;				pt6.y = pt2.y + maxY;		pt6.z = pt2.z;
	pt7.x = pt3.x;				pt7.y = pt3.y + maxY;		pt7.z = pt3.z;

	D3DXVec3TransformCoord(&tpt0, &pt0, &mat);
	D3DXVec3TransformCoord(&tpt1, &pt1, &mat);
	D3DXVec3TransformCoord(&tpt2, &pt2, &mat);
	D3DXVec3TransformCoord(&tpt3, &pt3, &mat);
	D3DXVec3TransformCoord(&tpt4, &pt4, &mat);
	D3DXVec3TransformCoord(&tpt5, &pt5, &mat);
	D3DXVec3TransformCoord(&tpt6, &pt6, &mat);
	D3DXVec3TransformCoord(&tpt7, &pt7, &mat);


	// check against all six planes
	//
	visible = true;

	for(int i=0; i<6; i++)
	{
		const pgPlane& p = nPlanes[i];
		bool v = false;

		if(tpt0.x*p.x + tpt0.y*p.y + tpt0.z*p.z < p.d)
			v = true;
		else
		if(tpt1.x*p.x + tpt1.y*p.y + tpt1.z*p.z < p.d)
			v = true;
		else
		if(tpt2.x*p.x + tpt2.y*p.y + tpt2.z*p.z < p.d)
			v = true;
		else
		if(tpt3.x*p.x + tpt3.y*p.y + tpt3.z*p.z < p.d)
			v = true;
		else
		if(tpt4.x*p.x + tpt4.y*p.y + tpt4.z*p.z < p.d)
			v = true;
		else
		if(tpt5.x*p.x + tpt5.y*p.y + tpt5.z*p.z < p.d)
			v = true;
		else
		if(tpt6.x*p.x + tpt6.y*p.y + tpt6.z*p.z < p.d)
			v = true;
		else
		if(tpt7.x*p.x + tpt7.y*p.y + tpt7.z*p.z < p.d)
			v = true;

		if(v==false)
		{
			visible = false;
			return;
		}
	}
	*/
}

//
// Calculates errors for all tesselation levels
//
void TerrainPatchBase::calcErrors()
{
	for (int i = 0; i <= MAX_SUBDIV; i++)
		calcError(i);
}

//
// Sets visibility state of this patch
//
void TerrainPatchBase::setVisible(bool flag)
{
	m_Visible = flag;

	if (m_Visible)
	{
		m_FlagActive = true;
		if (!m_Active)
			init();
		return;
	}

	if (!m_Active)
		deinit();
}

//
// Updates projected errors	for all tesselation levels
//
void TerrainPatchBase::updateProjectedErrors(const CameraPtr & camera)
{
	Matrix matrix = camera->transformationMatrix();
	Vector3 corr = m_Errors[1].correct * matrix;

	Q_ASSERT(corr.z() > 0.0f);
	if (unlikely(corr.z() < 0.0f))
		corr.z() = 0.0f;

	for (int i = 1; i <= MAX_SUBDIV; i++)
	{
		Vector3 real = m_Errors[i].real * matrix;

		Q_ASSERT(real.z() > 0.0f);
		if (unlikely(real.z() < 0.0f))
			real.z() = 0.0f;

		Vector3 diff(corr - real);
		m_Errors[i].diff = diff.length();

		if (m_Errors[i].diff < m_Errors[i - 1].diff * 1.1f)
			m_Errors[i].diff = m_Errors[i - 1].diff * 1.1f;
	}
}

//
//  Creates vertices and indices for the patch
//
void TerrainPatchBase::createTessellation(int center, int left, int right, int bottom, int top)
{
	Q_ASSERT(m_IndexMap);
	Q_ASSERT(m_Vertices);
	Q_ASSERT(m_Indices);

	memset(m_IndexMap, 0xFF, MAX_VERTICES * sizeof(ushort));
	m_NumNewVertices = 0;
	m_NumNewIndices = 0;

	int pow = getPowerTwo(center);
	Q_ASSERT(center - left <= 1);
	Q_ASSERT(center - right <= 1);
	Q_ASSERT(center - bottom <= 1);
	Q_ASSERT(center - top <= 1);

	bool bLeft = left < center;
	bool bRight = right < center;
	bool bBottom = bottom < center;
	bool bTop = top < center;

	if (pow == 16)
		addLevel4PatchTriangles(bLeft, bRight, bBottom, bTop);
	else
	{
		for (int y = 0; y < SOURCE_HEIGHT - pow; y += pow)
			addTriangleRow(y, pow, bLeft, bRight, bBottom, bTop);
	}

	// Indices must always be an even number (we store quads as two triangles)
	Q_ASSERT((m_NumNewIndices & 1) == 0);

	if (m_RealSelfTesselation > m_SelfTesselation)
		reduceShapeTo(m_RealSelfTesselation, m_Vertices);

	makeBordersSimpler(m_Vertices);
}

//
// Calculates minimum and maximum Y coordinate
//
void TerrainPatchBase::calcMinMaxY()
{
	Vector3 p;

	m_MinY = m_Scale.y();
	m_MaxY = 0.0f;

	for (int x = 0; x < SOURCE_WIDTH; x++)
	{
		for (int y = 0; y < SOURCE_HEIGHT; y++)
		{
			getVertex(x, y, p);

			if (p.y() < m_MinY)
				m_MinY = p.y();
			if (p.y() > m_MaxY)
				m_MaxY = p.y();
		}
	}
}

//
// Calculates power of two for the specified value
//
int TerrainPatchBase::getPowerTwo(int val)
{
	if (unlikely(val < 0 || val > 6))
		return 1;
	return 1 << val;
}


















void
TerrainPatchBase::calcError(int nTes)
{
	float	sumError = 0.0f;
	int		numErrors = 0;

	if(nTes!=0)
	{
		int pow = getPowerTwo(nTes),
			x0,y0, xi,yi;

		for(y0=0; y0<SOURCE_HEIGHT-pow; y0+=pow)
			for(x0=0; x0<SOURCE_WIDTH-pow; x0+=pow)
				for(yi=1; yi<pow; yi++)
					for(xi=1; xi<pow; xi++)
					{
						int		x = x0+xi,
								y = y0+yi;
						float	fx0 = (float)xi/(float)pow, fx1 = 1.0f-fx0,
								fy0 = (float)yi/(float)pow, fy1 = 1.0f-fy0;

						float	height00 = getHeight(x0,y0),
								height10 = getHeight(x0+pow,y0),
								height01 = getHeight(x0,y0+pow),
								height11 = getHeight(x0+pow,y0+pow);

						float	paintHeight =	fx1*fy1 * height00 +
												fx0*fy1 * height10 +
												fx1*fy0 * height01 +
												fx0*fy0 * height11,
								correctHeight =  getHeight(x,y);

						float	er = (float)fabs(correctHeight - paintHeight);

						numErrors++;
						sumError += er;
					}

		float error = sumError / numErrors;
		getVertex(SOURCE_WIDTH / 2, SOURCE_HEIGHT / 2, m_Errors[nTes].correct);
		m_Errors[nTes].real = m_Errors[nTes].correct;
		m_Errors[nTes].real.y() += error;
		m_Errors[nTes].diff = error;
	}
}








/****************************************************************************************\
*                                                                                        *
*                                                                                        *
*                                Tessellation Sub Methods                                 *
*                                                                                        *
*                                                                                        *
\****************************************************************************************/


// calls makeSimpler() sequentially to make a mesh with
// selfTes<nTes to look like being created with nTes
void TerrainPatchBase::reduceShapeTo(int nTes, PatchVertex *nData)
{
	for (int t = nTes; t > 0; t--)
	{
		if (m_SelfTesselation < t)
			makeSimpler(t, nData);
	}
}


void TerrainPatchBase::makeBordersSimpler(PatchVertex *nData)
{
	int sT = m_SelfTesselation;
	int x,y, idx,idx0,idx1, pow,
		lDiff = sT - m_LeftTesselation,
		rDiff = sT - m_RightTesselation,
		bDiff = sT - m_BottomTesselation,
		tDiff = sT - m_TopTesselation;


	if((lDiff==1 || lDiff==-1) && m_Left)
	{
		pow = getPowerTwo(Q_MIN(m_LeftTesselation, sT));

		for(y=pow; y<SOURCE_HEIGHT-1; y+=2*pow)
		{
			idx = m_IndexMap[0 + y*SOURCE_WIDTH];
			idx0 = m_IndexMap[0 + (y-pow)*SOURCE_WIDTH];
			idx1 = m_IndexMap[0 + (y+pow)*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idx0!=0xffff);
			Q_ASSERT(idx1!=0xffff);

			nData[idx].pos.y() = (nData[idx0].pos.y() + nData[idx1].pos.y()) / 2.0f;
		}
	}

	if((rDiff==1 || rDiff==-1) && m_Right)
	{
		pow = getPowerTwo(Q_MIN(m_RightTesselation,sT));

		for(y=pow; y<SOURCE_HEIGHT-1; y+=2*pow)
		{
			idx = m_IndexMap[SOURCE_WIDTH-1 + y*SOURCE_WIDTH];
			idx0 = m_IndexMap[SOURCE_WIDTH-1 + (y-pow)*SOURCE_WIDTH];
			idx1 = m_IndexMap[SOURCE_WIDTH-1 + (y+pow)*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idx0!=0xffff);
			Q_ASSERT(idx1!=0xffff);

			nData[idx].pos.y() = (nData[idx0].pos.y() + nData[idx1].pos.y()) / 2.0f;
		}
	}

	if((bDiff==1 || bDiff==-1) && m_Bottom)
	{
		pow = getPowerTwo(Q_MIN(m_BottomTesselation, sT));

		for(x=pow; x<SOURCE_WIDTH-1; x+=2*pow)
		{
			idx = m_IndexMap[x + 0*SOURCE_WIDTH];
			idx0 = m_IndexMap[x-pow + 0*SOURCE_WIDTH];
			idx1 = m_IndexMap[x+pow + 0*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idx0!=0xffff);
			Q_ASSERT(idx1!=0xffff);

			nData[idx].pos.y() = (nData[idx0].pos.y() + nData[idx1].pos.y()) / 2.0f;
		}
	}

	if((tDiff==1 || tDiff==-1) && m_Top)
	{
		pow = getPowerTwo(Q_MIN(m_TopTesselation, sT));

		for(x=pow; x<SOURCE_WIDTH-1; x+=2*pow)
		{
			idx = m_IndexMap[x + (SOURCE_HEIGHT-1)*SOURCE_WIDTH];
			idx0 = m_IndexMap[x-pow + (SOURCE_HEIGHT-1)*SOURCE_WIDTH];
			idx1 = m_IndexMap[x+pow + (SOURCE_HEIGHT-1)*SOURCE_WIDTH];

			Q_ASSERT(idx!=0xffff);
			Q_ASSERT(idx0!=0xffff);
			Q_ASSERT(idx1!=0xffff);

			nData[idx].pos.y() = (nData[idx0].pos.y() + nData[idx1].pos.y()) / 2.0f;
		}
	}
}


// this methods makes the mesh looking simpler by
// putting vertices into position inbetween its neighbors
// this method can do only for one step at a time
// (to make a tes_1 mesh look like tes_3 first call
// makeSimpler(3,...) then makeSimpler(2,...)
// Call reduceShapeTo() which cares about those constraints
// automatically
void TerrainPatchBase::makeSimpler(int nTes, PatchVertex *nData)
{
	Q_ASSERT(m_SelfTesselation < nTes);
	Q_ASSERT(nTes>0);

	int pow = getPowerTwo(nTes-1),
		x,y, xc,yc;

	for(yc=y=0; y<SOURCE_HEIGHT; y+=pow,yc++)
	{
		for(xc=x=0; x<SOURCE_WIDTH; x+=pow,xc++)
			calculateInbetweenVertex(x, y, xc, yc, pow, nData);
	}
}


void TerrainPatchBase::calculateInbetweenVertex(int x, int y, int xc, int yc, int pow, PatchVertex *nData)
{
	float	heightLeftTop=0.0f, heightRightBottom=0.0f;
	int		idx, idxLT,idxRB;

	idx = calculateDiagonalVertices(x, y, xc, yc, pow, nData, heightLeftTop, idxLT, heightRightBottom, idxRB);

	if(idx>=0)
		nData[idx].pos.y() = (heightLeftTop + heightRightBottom) / 2.0f;
}


int TerrainPatchBase::calculateDiagonalVertices(int x, int y, int xc, int yc, int pow, PatchVertex *nData,
											  float& nHeightLeftTop, int& idxLT, float& nHeightRightBottom, int& idxRB)
{
	int		c = (xc&1) + (yc&1)*2,
			idx = m_IndexMap[x + y*SOURCE_WIDTH];

	Q_ASSERT(idx!=0xffff);

	switch(c)
	{
	case 0:			// even-x & even-y
		// nothing to do...
		return -1;

	case 1:			// odd-x & even-y
		Q_ASSERT(x>0 && x<SOURCE_WIDTH-1);

		idxLT = m_IndexMap[x-pow + y*SOURCE_WIDTH];
		idxRB = m_IndexMap[x+pow + y*SOURCE_WIDTH];

		Q_ASSERT(idxLT!=0xffff);
		Q_ASSERT(idxRB!=0xffff);

		nHeightLeftTop = nData[idxLT].pos.y();
		nHeightRightBottom = nData[idxRB].pos.y();
		return idx;

	case 2:			// even-x & odd-y
		Q_ASSERT(y>0 && y<SOURCE_HEIGHT-1);

		idxLT = m_IndexMap[x + (y+pow)*SOURCE_WIDTH];
		idxRB = m_IndexMap[x + (y-pow)*SOURCE_WIDTH];

		Q_ASSERT(idxLT!=0xffff);
		Q_ASSERT(idxRB!=0xffff);

		nHeightLeftTop = nData[idxLT].pos.y();
		nHeightRightBottom = nData[idxRB].pos.y();
		return idx;

	case 3:			// odd-x & odd-y
		Q_ASSERT(x>0 && x<SOURCE_WIDTH-1);
		Q_ASSERT(y>0 && y<SOURCE_HEIGHT-1);

		idxLT = m_IndexMap[x-pow + (y+pow)*SOURCE_WIDTH];
		idxRB = m_IndexMap[x+pow + (y-pow)*SOURCE_WIDTH];

		Q_ASSERT(idxLT!=0xffff);
		Q_ASSERT(idxRB!=0xffff);

		nHeightLeftTop = nData[idxLT].pos.y();
		nHeightRightBottom = nData[idxRB].pos.y();

		return idx;

	default:
		Q_ASSERT(false);	// should never come here...
		return -1;
	}

	//nData[idx].pos.y = (y0+y1)/2.0f;
}


void TerrainPatchBase::addTriangleRow(int nY, int nPow, bool nLeft, bool nRight, bool nBottom, bool nTop)
{
	int powH = nPow/2;


	// special case: bottom line with nBottom
	//
	if(nY==0 && nBottom)
	{
		addTriangleBottomRow(nPow, nLeft, nRight);
		return;
	}

	if(nY!=0)
	{
		addLastIndexAgain();
		addIndex(getIndex(0,nY));
	}

	// special case: top line with nTop
	//
	if(nY>=SOURCE_HEIGHT-2*nPow && nTop)
	{
		addTriangleTopRow(nPow, nLeft, nRight);
		return;
	}


	// all other cases
	//
	int x, x0=0, x1=SOURCE_WIDTH;

	if(nLeft)
	{
		Q_ASSERT(powH>=1);

		addIndex(getIndex(0,nY));
		addIndex(getIndex(0,nY+powH));
		addIndex(getIndex(nPow,nY));
		addIndex(getIndex(0,nY+nPow));
		addIndex(getIndex(nPow,nY+nPow));
		addLastIndexAgain();							// finalize sub-strip
		x0 = nPow;
	}

	if(nRight)
		x1 -= nPow;

	for(x=x0; x<x1; x+=nPow)
	{
		addIndex(getIndex(x,nY));
		addIndex(getIndex(x,nY+nPow));
	}

	if(nRight)
	{
		Q_ASSERT(powH>=1);

		addIndex(getIndex(SOURCE_WIDTH-1-nPow,nY));
		addLastIndexAgain();							// reverse oriented triangles
		addIndex(getIndex(SOURCE_WIDTH-1,nY));
		addIndex(getIndex(SOURCE_WIDTH-1-nPow,nY+nPow));
		addIndex(getIndex(SOURCE_WIDTH-1,nY+powH));
		addIndex(getIndex(SOURCE_WIDTH-1,nY+nPow));
	}
}


void TerrainPatchBase::addTriangleBottomRow(int nPow, bool nLeft, bool nRight)
{
	int powH = nPow/2;
	int x0=0, x1=SOURCE_WIDTH-1;

	Q_ASSERT(powH>=1);

	if(nLeft)
	{
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,nPow));
		addIndex(getIndex(nPow,0));
		addIndex(getIndex(nPow,nPow));
		x0 = nPow;
	}

	Q_ASSERT((m_NumNewIndices & 1) == 0);

	if(nRight)
		x1 -= nPow;

	for(int x=x0; x<x1; x+=nPow)
	{
		addIndex(getIndex(x,0));
		addLastIndexAgain();						// reverse oriented triangles
		addIndex(getIndex(x+powH,0));
		addIndex(getIndex(x,nPow));
		addIndex(getIndex(x+nPow,0));
		addIndex(getIndex(x+nPow,nPow));
	}

	Q_ASSERT((m_NumNewIndices & 1) == 0);

	if(nRight)
	{
		addIndex(getIndex(SOURCE_WIDTH-1-nPow,0));
		addIndex(getIndex(SOURCE_WIDTH-1-nPow,nPow));
		addIndex(getIndex(SOURCE_WIDTH-1-powH,0));
		addIndex(getIndex(SOURCE_WIDTH-1,0));
		addLastIndexAgain();							// finalize sub-strip
		addIndex(getIndex(SOURCE_WIDTH-1-nPow,nPow));
		addIndex(getIndex(SOURCE_WIDTH-1,powH));
		addIndex(getIndex(SOURCE_WIDTH-1,nPow));
	}


	Q_ASSERT((m_NumNewIndices & 1) == 0);
}


void TerrainPatchBase::addTriangleTopRow(int nPow, bool nLeft, bool nRight)
{
	int powH = nPow/2;
	int x0=0, x1=SOURCE_WIDTH-1;

	Q_ASSERT(powH>=1);

	if(nLeft)
	{
		addIndex(getIndex(0,SOURCE_HEIGHT-1-nPow));
		addIndex(getIndex(0,SOURCE_HEIGHT-1-powH));
		addIndex(getIndex(nPow,SOURCE_HEIGHT-1-nPow));
		addIndex(getIndex(0,SOURCE_HEIGHT-1));
		addIndex(getIndex(powH,SOURCE_HEIGHT-1));
		addLastIndexAgain();						// reverse oriented triangles
		addIndex(getIndex(nPow,SOURCE_HEIGHT-1-nPow));
		addIndex(getIndex(nPow,SOURCE_HEIGHT-1));
		x0 = nPow;
	}

	Q_ASSERT((m_NumNewIndices & 1) == 0);

	if(nRight)
		x1 -= nPow;

	int x;
	for(x=x0; x<x1; x+=nPow)
	{
		addIndex(getIndex(x,SOURCE_HEIGHT-1-nPow));
		addIndex(getIndex(x,SOURCE_HEIGHT-1));
		addIndex(getIndex(x+nPow,SOURCE_HEIGHT-1-nPow));
		addIndex(getIndex(x+powH,SOURCE_HEIGHT-1));
		addIndex(getIndex(x+nPow,SOURCE_HEIGHT-1));
		addLastIndexAgain();						// reverse oriented triangles
	}

	if(nRight)
	{
		addIndex(getIndex(x,SOURCE_HEIGHT-1-nPow));
		addIndex(getIndex(x,SOURCE_HEIGHT-1));
		addIndex(getIndex(x+nPow,SOURCE_HEIGHT-1-nPow));
		addIndex(getIndex(x+powH,SOURCE_HEIGHT-1));
		addIndex(getIndex(x+nPow,SOURCE_HEIGHT-1-powH));
		addIndex(getIndex(x+nPow,SOURCE_HEIGHT-1));
	}
}


void TerrainPatchBase::addLevel4PatchTriangles(bool nLeft, bool nRight, bool nBottom, bool nTop)
{
	const int	powH = 8, pow = 16;
	int			code = (nLeft ? 1 : 0) | (nRight ? 2 : 0) | (nBottom ? 4 : 0) | (nTop ? 8 : 0);

	// use the created code for fast selection of the case to build...
	//
	switch(code)
	{
	case 0:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(pow,pow));
		break;

	case 1:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,pow));
		addLastIndexAgain();
		break;

	case 2:
		addIndex(getIndex(pow,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,0));
		addLastIndexAgain();
		break;

	case 3:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(pow,pow));
		break;

	case 4:
		addIndex(getIndex(0,0));
		addLastIndexAgain();
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(pow,pow));
		break;

	case 5:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(pow,pow));
		break;

	case 6:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(powH,0));
		addIndex(getIndex(pow,0));
		addLastIndexAgain();
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(pow,pow));
		break;

	case 7:
		addIndex(getIndex(pow,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,0));
		addLastIndexAgain();
		break;

	case 8:
		addIndex(getIndex(pow,pow));
		addLastIndexAgain();
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(0,0));
		break;

	case 9:
		addIndex(getIndex(pow,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(0,pow));
		addLastIndexAgain();
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(0,0));
		break;

	case 10:
		addIndex(getIndex(pow,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(0,0));
		break;

	case 11:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(pow,pow));
		addLastIndexAgain();
		break;

	case 12:
		addIndex(getIndex(0,0));
		addLastIndexAgain();
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(pow,pow));
		addLastIndexAgain();
		break;

	case 13:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(pow,pow));
		addLastIndexAgain();
		break;

	case 14:
		addIndex(getIndex(pow,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,0));
		addLastIndexAgain();
		break;

	case 15:
		addIndex(getIndex(0,0));
		addIndex(getIndex(0,powH));
		addIndex(getIndex(powH,0));
		addIndex(getIndex(0,pow));
		addIndex(getIndex(pow,0));
		addIndex(getIndex(powH,pow));
		addIndex(getIndex(pow,powH));
		addIndex(getIndex(pow,pow));
		break;
	}

	Q_ASSERT((m_NumNewIndices & 1) == 0);		// indices must always be an even number (we store quads as two triangles)
}


inline float TerrainPatchBase::getHeight(int nX, int nY) const
{
	Q_ASSERT(nX>=0 && nX<SOURCE_WIDTH);
	Q_ASSERT(nY>=0 && nY<SOURCE_HEIGHT);
	Q_ASSERT(m_Master);

	return m_Pos.y() + m_Scale.y() * m_Master->getHeight(m_HeightMapX + nX, m_HeightMapY + nY); // - scale.y*0.5f;
}


inline void	TerrainPatchBase::getVertex(int nX, int nY, Vector3 & nPoint) const
{
	Q_ASSERT(nX>=0 && nX<SOURCE_WIDTH);
	Q_ASSERT(nY>=0 && nY<SOURCE_HEIGHT);

	nPoint.x() = m_Pos.x() + m_Scale.x() * nX / (float)(SOURCE_WIDTH - 1);
	nPoint.y() = getHeight(nX, nY);
	nPoint.z() = m_Pos.z() + m_Scale.z() * nY / (float)(SOURCE_HEIGHT - 1);
}


inline void TerrainPatchBase::getTexCoord(int nX, int nY, Vector2 & nTCoord) const
{
	Q_ASSERT(nX>=0 && nX<SOURCE_WIDTH);
	Q_ASSERT(nY>=0 && nY<SOURCE_HEIGHT);
	Q_ASSERT(m_Master);

	m_Master->getTexCoord(m_HeightMapX + nX, m_HeightMapY + nY, nTCoord);
}


inline void TerrainPatchBase::getNormal(int nX, int nY, Vector3& nNormal) const
{
	Q_ASSERT(nX>=0 && nX<SOURCE_WIDTH);
	Q_ASSERT(nY>=0 && nY<SOURCE_HEIGHT);
	Q_ASSERT(m_Master);

	m_Master->getNormal(m_HeightMapX + nX, m_HeightMapY + nY, nNormal);
}


ushort TerrainPatchBase::getIndex(int nX, int nY)
{
	int idx = nX + nY * SOURCE_WIDTH;

	Q_ASSERT(nX >= 0 && nX < SOURCE_WIDTH);
	Q_ASSERT(nY >= 0 && nY < SOURCE_HEIGHT);
	Q_ASSERT(idx < MAX_VERTICES);

	if (m_IndexMap[idx]==0xffff)
	{
		m_IndexMap[idx] = (ushort)m_NumNewVertices;

		getVertex(nX, nY, m_Vertices[m_NumNewVertices].pos);
		getTexCoord(nX, nY, m_Vertices[m_NumNewVertices].tex);
		m_NumNewVertices++;
	}

	return m_IndexMap[idx];
}
