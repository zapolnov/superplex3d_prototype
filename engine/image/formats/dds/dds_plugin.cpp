#include <core/common.h>
#include "dds_plugin.h"
#include "dds_reader.h"

/* DDSPlugin methods */

//
// Creates new instance of the image format reader
//
ImageFormatReaderPtr DDSPlugin::createFileReader(const QString & ext, MemoryFile & file)
{
	return new DDSReader(ext, file);
}

//
// Creates new instance of the image format writer
//
ImageFormatWriterPtr DDSPlugin::createFileWriter(const QString & ext, QFile & file)
{
	return ImageFormatWriterPtr();
}
