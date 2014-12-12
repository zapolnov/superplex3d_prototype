#ifndef __TERRAIN__PATCH_BASE_H__
#define __TERRAIN__PATCH_BASE_H__

/* Includes */

#include <core/common.h>
#include <math/vector2.h>
#include <math/vector3.h>
#include <math/camera.h>


/* Classes */

class Terrain;

/** Base class for patches of the terrain */
class TerrainPatchBase
{
public:
	/** Visibility flags. */
	enum VisibilityFlags
	{
		VisBase = 0x01,
		VisLayer0 = 0x02,
		VisLayer1 = 0x04,
		VisLayer2 = 0x08,
		VisLayer3 = 0x10,
		VisLayer4 = 0x20,
		VisLayerAll = 0x3F
	};

	enum
	{
		MAX_SUBDIV = 4,
		SOURCE_WIDTH = 17,
		SOURCE_HEIGHT = 17,
		MAX_VERTICES = SOURCE_WIDTH * SOURCE_HEIGHT,
		MAX_INDICES = MAX_VERTICES * 3
	};

	/** Neighbours. */
	enum
	{
		LEFT = 1,
		RIGHT = 2,
		TOP = 4,
		BOTTOM = 8
	};

	/** Format of the vertex. */
	struct PatchVertex
	{
		Vector3 pos;
		Vector2 tex;
	};

	/**
	 * Constructor.
	 * @param master Pointer to the terrain that owns this patch.
	 * @param gridX
	 * @param gridY
	 */
	TerrainPatchBase(Terrain * nMaster, int gridX, int gridY);

	/** Initializes the patch. */
	virtual void init();

	/** Frees used resources. */
	virtual void deinit();

	/** Renders the patch. */
	virtual int render();

	/** Updates the tesselation. */
	virtual bool updateTessellation();

	/** Used in geomorphing. */
	virtual bool updateTessellation2();

	/** Used in geomorphing. */
	virtual bool updateTessellation3();

	/** Destroys any OpenGL-related objects. */
	virtual void deleteDeviceObjects();

	/** Restores any OpenGL-related objects. */
	virtual bool restoreDeviceObjects();

	/**
	 * Copies vertices into the specified buffer.
	 * @param dest Pointer to the destination buffer.
	 */
	virtual int fillBaseVertices(PatchVertex * dest);

	/**
	 * Sets the rendered flag.
	 * @param flag New value of the flag.
	 */
	inline void setRendered(bool flag)
	{
		m_Rendered = flag;
	}

	/** Returns state of the rendered flag. */
	inline bool getRendered() const
	{
		return m_Rendered;
	}

	/**
	 * Sets visibility flags.
	 * @param flags Visibility flags.
	 */
	inline void setVisFlags(int flags)
	{
		m_VisFlags = flags;
	}

	/** Returns the current visibility flags. */
	inline int getVisFlags() const
	{
		return m_VisFlags;
	}

	/**
	 * Sets pointer to the terrain.
	 * @param master Pointer to the terrain.
	 */
	inline void setMaster(Terrain * master)
	{
		m_Master = master;
	}

	/**
	 * Sets scale of the patch.
	 * @param scale Scale of the patch.
	 */
	inline void setScale(const Vector3 & scale)
	{
		m_Scale = scale;
	}

	/** Adds this patch to the list of active patches. */
	void addToActiveList();

	/** Removes this patch from the list of active patches. */
	void removeFromActiveList();

	/**
	 * Sets position of this patch on the height map.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 */
	inline void setHeightMapPosition(int x, int y)
	{
		m_HeightMapX = x;
		m_HeightMapY = y;
	}

	/** Returns current error. */
	inline float getCurrentError() const 
	{
		return m_CurrentError;
	}

	/** Returns current exact tesselation level of this patch. */
	float getExactTessellation() const;

	/** Returns current tesselation level of this patch. */
	inline int getCurrentTessellation() const
	{
		return m_SelfTesselation;
	}

	/** Returns new tesselation level of this patch. */
	inline int getNewTessellation() const
	{
		return m_NewSelfTesselation;
	}

	/**
	 * Returns new tesselation level of this patch.
	 * If this patch is not valid, some adequate value is returned.
	 */
	inline int getNewTessellationSafe() const
	{
		if (this == NULL)
			return MAX_SUBDIV;

		if (!isVisible())
			return m_SelfTesselation;

		return getNewTessellation();
	}

	/** Returns real tesselation level of this patch. */
	inline int getRealTessellation() const
	{
		return m_RealSelfTesselation;
	}

	/** Returns number of indices in this patch. */
	inline int getNumIndices() const
	{
		return m_NumNewIndices;
	}

	/** Returns number of vertices in this patch. */
	inline int getNumVertices() const
	{
		return m_NumNewVertices;
	}

	/** Returns pointer to the list of vertices. */
	inline const PatchVertex * getVertices() const
	{
		return m_Vertices;
	}

	/** Returns pointer to the list of indices. */
	inline const ushort * getIndices() const
	{
		return m_Indices;
	}

	/**
	 * Checks whether the patch is visible on the screen.
	 * @param camera Camera.
	 */
	bool checkVisibility(const CameraPtr & camera);

	/**
	 * Calculates visibility of this patch.
	 * @param camera Camera.
	 */
	void updateVisibility(const CameraPtr & camera);

	/** Calculates errors for all tesselation levels. */
	void calcErrors();

	/** Returns true if this patch is visible on the screen. */
	inline bool isVisible() const
	{
		return m_Visible;
	}

	/** Returns true if this patch is active. */
	inline bool isActive() const
	{
		return m_Active;
	}

	/**
	 * Sets visibility state of this patch.
	 * @param flag New visibility state.
	 */
	void setVisible(bool flag);

	/**
	 * Updates projected errors for all tesselation levels.
	 * @param camera Pointer to the camera.
	 */
	void updateProjectedErrors(const CameraPtr & camera);

	/**
	 * Returns current projection error for the specified tesselation level.
	 * @param which Index of the level to return error for.
	 */
	float getProjectedError(int which)
	{
		return m_Errors[which].diff;
	}

	/**
	 * Sets real tesselation level of the patch.
	 * @param tess Value.
	 */
	inline void setRealTessellation(int tess)
	{
		m_OldRealSelfTesselation = m_RealSelfTesselation;
		m_NewRealSelfTesselation = tess;
	}

	/**
	 * Sets tesselation level of the patch.
	 * @param tess Value.
	 */
	inline void setTessellation(int tess)
	{
		m_NewSelfTesselation = tess;
	}

	/** Returns pointer to the next active patch. */
	inline TerrainPatchBase * getNextActive()
	{
		return m_Next;
	}

	/** */
	inline int getGridPosX() const
	{
		return m_GridX;
	}

	/** */
	inline int getGridPosY() const
	{
		return m_GridY;
	}

	/**
	 * Sets the active flag.
	 * @param flag New value of the flag.
	 */
	inline void setFlagActive(bool flag)
	{
		m_FlagActive = flag;
	}

	/** Returns current value of the active flag. */
	inline bool getFlagActive() const
	{
		return m_FlagActive;
	}

	/**
	 * Sets position of the patch.
	 * @param pos New position of the patch.
	 */
	inline void setPosition(const Vector3 & pos)
	{
		m_Pos = pos;
	}

	/**
	 * Sets neighbours of the patch.
	 * @param left Pointer to the left neighbour.
	 * @param right Pointer to the right neighbour.
	 * @param top Pointer to the top neighbour.
	 * @param bottom Pointer to the bottom neighbour.
	 */
	inline void setNeighbors(const TerrainPatchBase * left, const TerrainPatchBase * right,
		const TerrainPatchBase * top, const TerrainPatchBase * bottom)
	{
		m_Left = left;
		m_Right = right;
		m_Top = top;
		m_Bottom = bottom;
	}

	/**
	 * Returns flag for the specified layer.
	 * @param layer Index of the layer.
	 */
	static inline int getAddLayerFlag(int layer)
	{
		return 1 << layer;
	}

	/**
	 * Creates vertices and indices for the patch.
	 * @param center Center tesselation level.
	 * @param left Left tesselation level.
	 * @param right Right tesselation level.
	 * @param bottom Bottom tesselation level.
	 * @param top Top tesselation level.
	 */
	void createTessellation(int center, int left, int right, int bottom, int top);

	/** Returns pointer to the left neighbour. */
	inline const TerrainPatchBase * getLeft() const
	{
		return m_Left;
	}

	/** Returns pointer to the right neighbour. */
	inline const TerrainPatchBase * getRight() const
	{
		return m_Right;
	}

	/** Returns pointer to the bottom neighbour. */
	inline const TerrainPatchBase * getBottom() const
	{
		return m_Bottom;
	}

	/** Returns pointer to the top neighbour. */
	inline const TerrainPatchBase * getTop() const
	{
		return m_Top;
	}

	/** Calculates minimum and maximum Y coordinate. */
	void calcMinMaxY();

protected:
	/** Error information. */
	struct Error
	{
		Vector3 correct;		/**< Correct value. */
		Vector3 real;			/**< Real value. */
		float diff;				/**< Difference between correct and real values. */

		/** Constructor. */
		inline Error()
			: diff(0.0f)
		{
		}
	};

	int							m_VisFlags;						/**< Visibility flags of the patch. */
	Terrain *					m_Master;						/**< Pointer to the terrain. */
	int							m_GridX;
	int							m_GridY;
	bool						m_Active : 1;					/**< True if patch is active. */
	bool						m_Visible : 1;					/**< True if patch is visible on the screen. */
	bool						m_ForceBufferCreate : 1;
	bool						m_Rendered : 1;
	bool						m_LowVertMethod2 : 1;
	Error						m_Errors[MAX_SUBDIV + 2];		/**< List of errors for each tesselation level. */
	float						m_CurrentError;
	int							m_NewSelfTesselation;
	int							m_SelfTesselation;
	int							m_OldRealSelfTesselation;
	int							m_NewRealSelfTesselation;
	int							m_RealSelfTesselation;
	int							m_LeftTesselation;
	int							m_RightTesselation;
	int							m_TopTesselation;
	int							m_BottomTesselation;
	Vector3						m_Pos;
	Vector3						m_Scale;
	float						m_MinY;
	float						m_MaxY;
	int							m_HeightMapX;					/**< X coordinate of this patch on the height map. */
	int							m_HeightMapY;					/**< Y coordinate of this patch on the height map. */
	int							m_NumVertices;					/**< Number of vertices in the patch. */
	int							m_NumIndices;					/**< Number of indices in the patch. */
	int							m_NumNewVertices;
	int							m_NumNewIndices;
	int							m_FrameIndex;
	TerrainPatchBase *			m_Next;							/**< Pointer to the next active patch. */
	TerrainPatchBase *			m_Prev;							/**< Pointer to the previous active patch. */
	bool						m_FlagActive;
	const TerrainPatchBase *	m_Left;							/**< Pointer to the left neighbour. */
	const TerrainPatchBase *	m_Right;						/**< Pointer to the right neighbour. */
	const TerrainPatchBase *	m_Top;							/**< Pointer to the top neighbour. */
	const TerrainPatchBase *	m_Bottom;						/**< Pointer to the bottom neighbour. */
	ushort *					m_IndexMap;
	PatchVertex	*				m_Vertices;						/**< Array of vertex coordinates. */
	ushort *					m_Indices;						/**< Array of indices. */
	unsigned char				m_UseNeightbors;

	/** Calculates power of two for the specified value. */
	static int getPowerTwo(int val);


	unsigned short getIndex(int nX, int nY);

	float getHeight(int nX, int nY) const;
	void getVertex(int nX, int nY, Vector3 & nPoint) const;

	void getTexCoord(int nX, int nY, Vector2 & nTCoord) const;
	void getNormal(int nX, int nY, Vector3 & nNormal) const;

	void calcError(int nTes);

	/**
	 * Adds index to the indices array.
	 * @param idx Value of the index.
	 */
	inline void addIndex(ushort idx)
	{
		m_Indices[m_NumNewIndices++] = idx;
	}

	/** Duplicates the previously added index in the indices array. */
	inline void addLastIndexAgain()
	{
		if (m_NumNewIndices > 0)
			m_Indices[m_NumNewIndices++] = m_Indices[m_NumNewIndices - 1];
	}

	void reduceShapeTo(int nTes, PatchVertex *nData);
	void makeBordersSimpler(PatchVertex *nData);
	void makeSimpler(int nTes, PatchVertex *nData);
	void calculateInbetweenVertex(int x, int y, int xc, int yc, int pow, PatchVertex *nData);
	int calculateDiagonalVertices(int x, int y, int xc, int yc, int pow, PatchVertex *nData,
								  float& nHeightLeftTop, int& idxLT, float& nHeightRightBottom, int& idxRB);

	void addLevel4PatchTriangles(bool nLeft, bool nRight, bool nBottom, bool nTop);
	void addTriangleRow(int nY, int nPow, bool nLeft, bool nRight, bool nBottom, bool nTop);
	void addTriangleBottomRow(int nPow, bool nLeft, bool nRight);
	void addTriangleTopRow(int nPow, bool nLeft, bool nRight);
};

#endif // __TERRAIN__PATCH_BASE_H__
