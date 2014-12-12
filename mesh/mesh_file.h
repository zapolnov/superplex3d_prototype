#ifndef __MESH__MESH_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__MESH_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>
#include <interfaces/loadable.h>
#include <mesh/vertex_format.h>
#include <mesh/index_buffer.h>
#include <mesh/vertex_buffer.h>
#include <mesh/mesh_material.h>
#include <core/memory_file.h>
#include <math/aabb.h>
#include <math/sphere.h>
#include <tbb/spin_rw_mutex.h>
#include <set>
#include <vector>


/* Constants */

/** Primitive topologies. */
enum PrimitiveTopology
{
	PointList = 0,					/**< Primitive is a set of points. */
	LineList,						/**< Primitive is a set of lines. */
	LineStrip,						/**< Primitive is a strip of lines. */
	TriangleList,					/**< Primitive is a set of triangles. */
	TriangleStrip,					/**< Primitive is a strip of triangles. */
	LineListAdj,					/**< Primitive is a set of lines with adjacency information. */
	LineStripAdj,					/**< Primitive is a strip of lines with adjacency information. */
	TriangleListAdj,				/**< Primitive is a set of triangles with adjacency information. */
	TriangleStripAdj				/**< Primitive is a strip of triangles with adjacency information. */
};


/* Classes */

class IMeshFormatPlugin;

/** 3D mesh file. */
class GCC_PUBLIC MeshFile : public ILoadable
{
public:
	/** Sub-material information. */
	struct Submaterial
	{
		uint firstIndex;			/**< Index of the first index that uses this material. */
		uint numIndices;			/**< Number of indices that use this material. */
		MeshMaterialPtr material;	/**< Information about the material. */
	};

	/** Constructs an empty mesh. */
	MESHAPI MeshFile();

	/**
	 * Loads mesh from the specified file.
	 * @param file Path to the file.
	 */
	MESHAPI void loadFromFile(const QString & file);

	/**
	 * Loads mesh from the specified file.
	 * @param file File data.
	 */
	MESHAPI void loadFromFile(MemoryFile & file);

	/**
	 * Saves mesh to the specified file
	 * @param file Path to the file.
	 */
	MESHAPI void saveToFile(const QString & file);

	/** Returns vertex format of the mesh. */
	inline const VertexFormat & format() const
	{
		return m_Format;
	}

	/** Returns vertex data of the mesh. */
	inline const VertexBuffer & vertices() const
	{
		return m_Vertices;
	}

	/** Returns index data of the mesh. */
	inline const IndexBuffer & indices() const
	{
		return m_Indices;
	}

	/** Returns number of submaterials in the mesh. */
	inline uint numSubmaterials() const
	{
		return m_Submaterials.size();
	}

	/**
	 * Returns the specified submaterial.
	 * @param idx Index of the submaterial.
	 */
	inline const Submaterial & submaterial(uint idx) const
	{
		Q_ASSERT(idx < m_Submaterials.size());
		return m_Submaterials[idx];
	}

	/** Returns primitive topology of the mesh. */
	inline PrimitiveTopology primitiveTopology() const
	{
		return m_Topology;
	}

	/** Returns bounding box for the mesh. */
	inline const AABB & boundingBox() const
	{
		return m_BoundingBox;
	}

	/** Returns bounding sphere for the mesh. */
	inline const Sphere & boundingSphere() const
	{
		return m_BoundingSphere;
	}

	/** Calculates bounding sphere and bounding box for the mesh. */
	MESHAPI void calcBoundingVolumes();

	/**
	 * Registers the specified mesh format plugin.
	 * @param ptr Pointer to the mesh format plugin.
	 */
	MESHAPI static void registerMeshFormatPlugin(IMeshFormatPlugin * ptr);

	/**
	 * Unregisters the specified mesh format plugin.
	 * @param ptr Pointer to the mesh format plugin.
	 */
	MESHAPI static void unregisterMeshFormatPlugin(IMeshFormatPlugin * ptr);

private:
	VertexFormat m_Format;							/**< Vertex format. */
	VertexBuffer m_Vertices;						/**< Vertex data. */
	IndexBuffer m_Indices;							/**< Index data. */
	std::vector<Submaterial> m_Submaterials;		/**< List of submaterials. */
	PrimitiveTopology m_Topology;					/**< Primitive topology. */
	AABB m_BoundingBox;								/**< Bounding box for the mesh. */
	Sphere m_BoundingSphere;						/**< Bounding sphere for the mesh */
	static std::set<IMeshFormatPlugin *> m_Plugins;	/**< Set of mesh format plugins. */
	static tbb::spin_rw_mutex m_PluginsLock;		/**< Lock for access to the list of mesh format plugins. */
	static bool m_StdPluginsRegistered;				/**< True if standard plugins has been registered. */

	Q_DISABLE_COPY(MeshFile)
	friend class MeshFormatReaderHelper;
};

/** Pointer to the mesh file. */
typedef Pointer<MeshFile> MeshFilePtr;

#endif // __MESH__MESH_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
