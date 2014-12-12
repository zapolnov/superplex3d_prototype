#include <core/common.h>
#include "obj_plugin.h"
#include "obj_reader.h"

/* OBJPlugin methods */

//
// Creates new instance of the mesh format reader
//
MeshFormatReaderPtr OBJPlugin::createFileReader(const QString & ext, MemoryFile & file)
{
	return new OBJReader(ext, file);
}

//
// Creates new instance of the mesh format writer
//
MeshFormatWriterPtr OBJPlugin::createFileWriter(const QString & ext, QFile & file)
{
	return MeshFormatWriterPtr();
}
