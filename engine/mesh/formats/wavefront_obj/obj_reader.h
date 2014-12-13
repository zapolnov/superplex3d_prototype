#ifndef __MESH__FORMATS__WAVEFRONT_OBJ__OBJ_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__FORMATS__WAVEFRONT_OBJ__OBJ_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <mesh/interfaces/mesh_format_reader.h>
#include <mesh/helpers/mesh_format_reader_helper.h>


/* Classes */

/** OBJ mesh format reader. */
class OBJReader : public MeshFormatReaderHelper
{
public:
	/**
	 * Constructor.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 */
	OBJReader(const QString & ext, MemoryFile & file);

	/**
	 * Reads the mesh.
	 * @param mesh Pointer to the mesh file.
	 */
	void read(MeshFile * mesh);
};

#endif // __MESH__FORMATS__WAVEFRONT_OBJ__OBJ_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
