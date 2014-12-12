#ifndef __MESH__INTERFACES__MESH_FORMAT_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__INTERFACES__MESH_FORMAT_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <mesh/interfaces/mesh_format_reader.h>
#include <mesh/interfaces/mesh_format_writer.h>
#include <core/memory_file.h>
#include <mesh/mesh_file.h>


/* Classes */

class QString;
class QFile;

/** Interface to the mesh format plugin. */
class IMeshFormatPlugin
{
public:
	/** Destructor. */
	virtual ~IMeshFormatPlugin()
	{
		MeshFile::unregisterMeshFormatPlugin(this);
	}

	/**
	 * Creates new instance of the mesh format reader.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the reader or NULL if this file cannot be parsed by this plugin.
	 */
	virtual MeshFormatReaderPtr createFileReader(const QString & ext, MemoryFile & file) = 0;

	/**
	 * Creates new instance of the mesh format writer.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the writer or NULL if this file cannot be parsed by this plugin.
	 */
	virtual MeshFormatWriterPtr createFileWriter(const QString & ext, QFile & file) = 0;
};

#endif // __MESH__INTERFACES__MESH_FORMAT_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
