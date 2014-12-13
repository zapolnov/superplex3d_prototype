#ifndef __MESH_INTERFACES__MESH_FORMAT_WRITER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH_INTERFACES__MESH_FORMAT_WRITER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>


/* Classes */

class MeshFile;

/** Interface to the mesh format writer. */
class IMeshFormatWriter : public RefCounted
{
public:
	/**
	 * Saves the mesh.
	 * @param mesh Pointer to the mesh.
	 */
	virtual void write(const MeshFile * mesh) = 0;
};

/** Pointer to the mesh format writer. */
typedef Pointer<IMeshFormatWriter> MeshFormatWriterPtr;

#endif // __MESH_INTERFACES__MESH_FORMAT_WRITER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
