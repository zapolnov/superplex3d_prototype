#ifndef __MESH__INTERFACES__MESH_FORMAT_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__INTERFACES__MESH_FORMAT_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>


/* Classes */

class MeshFile;

/** Interface to the mesh format reader. */
class IMeshFormatReader : public RefCounted
{
public:
	/** Returns true if this is a valid file. */
	virtual bool isValidFile() const = 0;

	/**
	 * Reads the mesh.
	 * @param mesh Pointer to the mesh file.
	 */
	virtual void read(MeshFile * mesh) = 0;
};

/** Pointer to the image format reader. */
typedef Pointer<IMeshFormatReader> MeshFormatReaderPtr;

#endif // __MESH__INTERFACES__MESH_FORMAT_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
