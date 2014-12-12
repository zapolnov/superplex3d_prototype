#ifndef __MESH__FORMATS__WAVEFRONT_OBJ__DDS_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__FORMATS__WAVEFRONT_OBJ__DDS_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <mesh/interfaces/mesh_format_plugin.h>


/* Classes */

/** Wavefront OBJ mesh format plugin. */
class OBJPlugin : public IMeshFormatPlugin
{
public:
	/**
	 * Creates new instance of the mesh format reader.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the reader or NULL if this file cannot be parsed by this plugin.
	 */
	MeshFormatReaderPtr createFileReader(const QString & ext, MemoryFile & file);

	/**
	 * Creates new instance of the mesh format writer.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the writer or NULL if this file cannot be parsed by this plugin.
	 */
	MeshFormatWriterPtr createFileWriter(const QString & ext, QFile & file);
};

#endif // __MESH__FORMATS__WAVEFRONT_OBJ__DDS_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
