#ifndef __TERRAIN__TERRAIN_H__
#define __TERRAIN__TERRAIN_H__

/* Includes */

#include <core/common.h>
#include <engine/render.h>
#include <math/matrix.h>
#include <image/image_file.h>
#include "patch_base.h"


/* Classes */

class TerrainPVS;

/**
 * Class used to render the terrain.
 * Geo-mipmapping is used in order to display large landscapes at a high framerate. An arbitrary number of
 * texture passes can be applied. An alpha map defines, where each texture is visible on the landscape and where not.
 */
class Terrain
{
public:
	/**
	 * Constructor.
	 * @param mode Terrain rendering mode.
	 */
	Terrain(HeightMapRenderMode mode);

	/// Sets a mipmap filter
	/**
	 *  Default filter is LINEAR.
	 */
//	void setMipmapFilter(pgTextureStage::FILTER nFilter)  {  mipFilter = nFilter;  }


	/// Sets the base passes texture repeat rate and lightmap
	/**
	 *  Sets a texture for the base pass. nRepeatX and nRepeatY define
	 *  how often the texture is repeated (wrapped) throughout the whole terrain.
	 *  (1.0 means to stretch the texture to cover the whole terrain
	 *  without repeating). The lightmap is not repeated.
	 */
	//void setBasePass(pgTexture* nColorTexture, float nRepeatX, float nRepeatY, pgTexture* nLightmap);

	/// Adds a pass to the terrain.
    /**
	 *  This method works same as setBasePass, except that the lightmap has
	 *  to have an alpha channel, which defines, where the colortexture is
	 *  visible and where not.
	 */
//	void addPass(pgTexture* nColorTexture, float nRepeatX, float nRepeatY, pgTexture* nLightmap);


	/**
	 * Sets the height map.
	 * @param image Pointer to the image.
	 */
	void setHeightMap(const ImageFilePtr & image);


	/// Defines the name of the pvs map file
	//
	void setPVSName(const QString & nName)  {  m_PVSName = nName;  }


	/// Sets a map which defines where trees are inserted
//	void setTreeMap(pgImage* nImage)  {  treeMap = nImage;  }


//	void addTreeType(pgBlendTree* nTree, int nIdx)  {  treeTypes.addTail(TreeType(nTree, nIdx));  }


	/// Sets a texture which will be painted on the terrain to show cloud shadows.
	/**
	 *  nRepeatX and nRepeatY define of often the texture is repeated (wrapped).
	 *  nSpeedX and nSpeedY define how much (in texture coordinates) the texture
	 *  moves in one second.
	 */
//	void setSkyMap(pgTexture* nSkyMap, float nRepeatX, float nRepeatY, float nSpeedX, float nSpeedY);


	/// Sets the size of the landscape in number of patches.
	/**
	 *  nX and nY must correlate to the image which was set as heightmap.
	 *  The heightmaps size must be (nX*16)+1 x (nY*16)+1.
	 */
	void setNumPatches(int nX, int nY)  {  patchesX = nX;  patchesY = nY;  }


	/// Sets how large one patch is.
	/**
	 *  The size of the complete landscape can be calculated by:
	 *  patchesX*nSizeX x nSizeY x patchesY*nSizeZ
	 */
	void setPatchSize(float nSizeX, float nSizeY, float nSizeZ);


	/// Sets the maximum error which is allowed for patch rendering
	/**
	 *  If the projected error of a patch on the render window exceeds
	 *  maxError, the next more detailed tesselation is used.
	 */
	void setMaxError(float nErr)  {  maxError = nErr;  }


	/// Returns the maximum error, which was set by setMaxError()
	float getMaxError() const  {  return maxError;  }


	/// Sets the fog's range
	/**
	 *  If this method is not called no fog is used
	 */
	void setFogRange(float nNear, float nFar)  {  m_FogNear = nNear;  m_FogFar = nFar;  }


	/// Sets the fog's color (default: (1.0, 1.0, 1.0, 1.0)
	void setFogColor(const Vector4& nCol)  {  m_FogColor = nCol;  }


	/// Sets the fps which shall be achieved
	/**
	 *  If the value is not zero, the terrain will automatically
	 *  been drawn coarser or finer in order to achieve the
	 *  desired fps. Pass 0 in order to turn of fps achievment.
	 */
	void setDesiredFPS(int nFPS)  {  desiredFPS = nFPS;  }


	/// Returns the desired fps set with setDesiredFPS()
	/**
	 *  By default the automatic fps feature is disabled and
	 *  getDesiredFPS() will return 0.
	 */
	int getDesiredFPS() const  {  return desiredFPS;  }


	void setMergePatches(bool nSet)  {  mergeSmallPatches = nSet;  }


	bool getMergePatches() const  {  return mergeSmallPatches;  }


	/// Build the landscape after all properties have been set.
	void build();


	/// Renders the landscape.
	void render();


	/// Updates the landscapes tesselation.
	/**
	 *  It's important that the camera's final position for
	 *  the current frame has already been set, since it is
	 *  used to calculate the tesselation depth for each
	 *  patch.
	 */
	void update();


	/// Does not yet work correctly
	bool intersectLine(const Vector3& nPos0, const Vector3& nPos1, float& nScalarPos) const;


	/// Calculates the position on landscape's surface below nPos
	/**
	 *  If nPos is not above the landscape false is returned.
	 */
	bool projectDown(const Vector3& nPos, Vector3& nProjected) const;


	/// Moves nPos to stay above the surface.
	/**
	 *  (nGlideFactor is not used yet)
	 */
	bool moveAboveSurface(Vector3& nPos, float nHeight, float nGlideFactor) const;


	/// Returns the number of patches which were rendered in the last frame
	int getNumPatchesRendered()  {  return numPatchesRendered;  }


	TerrainPatchBase * getPatch(int nX, int nY)  {  return patches[nY][nX];  }


	/// Fills a string with information about the last rendered terrain
//	void fillInfoString(pgString& nStr);
//	void fillInfoString2(pgString& nStr);


	virtual void setPosition(const Vector3 & nPos)  {  pos = nPos;  }
	virtual void setRotation(const Vector3 & nRot)  {  rot = nRot;  }

	virtual void deleteDeviceObjects();
	virtual bool restoreDeviceObjects();

	void addBufferCreateCount()  {  bufferCreateCount++;  }

//	static HeightMapRenderMode getRenderModeFromString(const pgString& nString);

protected:
	struct MultiPassInfo
	{
//		MultiPassInfo()  {  texColor = texMod = NULL;  repeatX = repeatY = 1.0f;  }

//		pgTexturePtr	texColor, texMod;
		float			repeatX, repeatY;
		Matrix			matrix;

		// these members are only used for searching invisible patches...
		//
		unsigned char*	visData;
		int				visPitch;
		bool			inVis,fullVis;
	};


/*	struct TreeType
	{
		TreeType()  {  tree = NULL;  idx = 0;  }
		TreeType(pgBlendTree* nTree, int nIdx)  {  tree = nTree;  idx = nIdx;  }

		friend bool																	// operator== needed for pgList
			operator==(const TreeType&, const TreeType&)  { return false; }

		pgBlendTree*	tree;
		int				idx;
	};*/


	HeightMapRenderMode m_RenderMode;				/**< Current rendering mode. */
	QString	m_PVSName;						/**< Name of the PVS file. */
	TerrainPVS * m_PVS;						/**< Pointer to the PVS data. */

	Vector3						pos, rot;

	float						sizeX, sizeY, sizeZ;
	float						patchDX, patchDY, patchDZ;
	int							minX, minY, maxX, maxY;

	float						maxError;
	int							desiredFPS;

	TerrainPatchBase *		**patches;
	TerrainPatchBase *		activePatches;

	int							patchesX, patchesY;
	int							numPatchesRendered, numPatchesPassesRendered, numPatchesProcessed;

	Matrix						worldMatrix;

//	pgPtrList<MultiPassInfo>	multiPassInfos;


	MultiPassInfo				skyPass;
	float						skySpeedX,skySpeedY,
								skyPosX,skyPosY;

	float						m_FogNear;
	float						m_FogFar;
	Vector4						m_FogColor;

	byte *						m_HeightMap;						/**< Pointer to the height map data. */
	uint						m_HeightMapWidth;					/**< Width of the height map. */
	uint						m_HeightMapHeight;					/**< Height of the height map. */

//	pgImagePtr					treeMap;
//	pgList<TreeType>			treeTypes;

	Vector3 *					normals;

	int							numNewVertices, numNewIndices,
								numVertices, numIndices;

	bool						forceBufferCreate;
	BufferObjectPtr				m_IndexBuffer;						/**< Pointer to the index buffer. */
	BufferObjectPtr				m_VertexBuffer;						/**< Pointer to the vertex buffer. */

//	pgTextureStage::FILTER		mipFilter;

	bool						useUnitDistance, mergeSmallPatches;
	int							patchCount[TerrainPatchBase::MAX_SUBDIV+1], bufferCreateCount;

	TerrainPatchBase * createPatch(int gridX, int gridY);




	void updateSkyMatrix();

	void renderUnified();
	void renderSplit();

//	void fillPassInfo(MultiPassInfo* nPass, pgTexture* nTexture, float nX, float nY, pgTexture* nLightmap);
	void calcPassVisibility(MultiPassInfo* nInfo, int nX, int nY);

	void setupTextureStages(MultiPassInfo* nPass, bool nBasePass, bool nForceNoHW=false);
	void setupTextureStagesForSky(bool nForceNoHW=false);
	void updateWorldMatrix();
	void checkBuffers();

	TerrainPatchBase *& getActiveList()  {  return activePatches;  }

	inline float getHeight(int nX, int nY) const;
	inline void getNormal(int nX, int nY, Vector3& nNormal) const;
	inline void getTexCoord(int nX, int nY, Vector2& nTCoord) const;

	float getHeightDif(int nX0, int nY0, int nX1, int nY1);
	void createNormals();




	friend class TerrainPatchBase;
};

#define FLIP_Y

inline float
Terrain::getHeight(int nX, int nY) const
{
	Q_ASSERT(nX >= 0 && nX < (int)m_HeightMapWidth);
	Q_ASSERT(nY >= 0 && nY < (int)m_HeightMapHeight);

#ifdef FLIP_Y
	return ((float)m_HeightMap[nX + (m_HeightMapHeight-1-nY)*m_HeightMapWidth])/255.0f;
#else
	return ((float)m_HeightMap[nX + nY*m_HeightMapWidth])/255.0f;
#endif // FLIP_Y
}


inline void
Terrain::getNormal(int nX, int nY, Vector3& nNormal) const
{
	Q_ASSERT(nX >= 0 && nX < (int)m_HeightMapWidth);
	Q_ASSERT(nY >= 0 && nY < (int)m_HeightMapHeight);

//#ifdef FLIP_Y
//	nNormal = normals[nX + (m_HeightMapHeight-1-nY)*m_HeightMapWidth];
//#else
	nNormal = normals[nX + nY*m_HeightMapWidth];
//#endif // FLIP_Y
}


inline void
Terrain::getTexCoord(int nX, int nY, Vector2& nTCoord) const
{
	nTCoord.x() = (float)nX/(float)(m_HeightMapWidth-1);

#ifdef FLIP_Y
	nTCoord.y() = (float)(m_HeightMapHeight-1-nY)/(float)(m_HeightMapHeight-1);
#else
	nTCoord.y() = (float)nY/(float)(m_HeightMapHeight-1);
#endif // FLIP_Y
}


#endif // __TERRAIN__TERRAIN_H__

