#ifndef __MESH__HELPERS__MESH_FORMAT_READER_HELPER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__HELPERS__MESH_FORMAT_READER_HELPER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <mesh/interfaces/mesh_format_reader.h>
#include <mesh/mesh_file.h>
#include <core/memory_file.h>


/* Classes */

class QFile;

/** Helper class for mesh format readers. */
class MeshFormatReaderHelper : public IMeshFormatReader
{
public:
	/**
	 * Constructor.
	 * @param file Reference to the file.
	 */
	MESHAPI MeshFormatReaderHelper(MemoryFile & file);

	/** Returns true if this is a valid file. */
	MESHAPI bool isValidFile() const;

protected:
	MemoryFile & m_File;	/**< Reference to the file. */
	bool m_IsValid;			/**< True if this is a valid file. */

	/**
	 * Sets vertex format of the mesh.
	 * @param mesh Pointer to the mesh file.
	 * @param format Vertex format of the mesh.
	 */
	inline void setMeshVertexFormat(MeshFile * mesh, const VertexFormat & format)
	{
		mesh->m_Format = format;
	}

	/**
	 * Provides access to the vertex data of the mesh.
	 * @param mesh Pointer to the mesh file.
	 */
	inline VertexBuffer & meshVertices(MeshFile * mesh)
	{
		return mesh->m_Vertices;
	}

	/**
	 * Provides access to the index data of the mesh.
	 * @param mesh Pointer to the mesh file.
	 */
	inline IndexBuffer & meshIndices(MeshFile * mesh)
	{
		return mesh->m_Indices;
	}

	/**
	 * Adds submaterial to the mesh.
	 * @param mesh Pointer to the mesh file.
	 * @param mat Submaterial to add.
	 */
	inline void addMeshSubmaterial(MeshFile * mesh, const MeshFile::Submaterial & mat)
	{
		mesh->m_Submaterials.push_back(mat);
	}

	/**
	 * Sets primitive topology of the mesh.
	 * @param mesh Pointer to the mesh file.
	 * @param topology Primitive topology.
	 */
	inline void setMeshPrimitiveTopology(MeshFile * mesh, PrimitiveTopology topology)
	{
		mesh->m_Topology = topology;
	}

	/**
	 * Sets mesh bounding sphere.
	 * @param mesh Pointer to the mesh file.
	 * @param sphere Bounding sphere.
	 */
	inline void setMeshBoundingSphere(MeshFile * mesh, const Sphere & sphere)
	{
		mesh->m_BoundingSphere = sphere;
	}

	/**
	 * Sets mesh bounding box.
	 * @param mesh Pointer to the mesh file.
	 * @param box Bounding box.
	 */
	inline void setMeshBoundingBox(MeshFile * mesh, const AABB & box)
	{
		mesh->m_BoundingBox = box;
	}

	/**
	 * Reads data from file.
	 * @param ptr Pointer to the buffer.
	 * @param size Size of the buffer.
	 * @returns true on success, false if not enough data has been read.
	 */
	MESHAPI bool tryReadFile(void * ptr, uint size);

	/**
	 * Reads data from file.
	 * @param ptr Pointer to the buffer.
	 * @param size Size of the buffer.
	 */
	MESHAPI void readFile(void * ptr, uint size);
};

#endif // __MESH__HELPERS__MESH_FORMAT_READER_HELPER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
