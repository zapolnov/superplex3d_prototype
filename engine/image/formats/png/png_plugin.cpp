#include <core/common.h>
#include "png_plugin.h"
#include "png_reader.h"

/* PNGPlugin methods */

//
// Creates new instance of the image format reader
//
ImageFormatReaderPtr PNGPlugin::createFileReader(const QString & ext, MemoryFile & file)
{
	return new PNGReader(ext, file);
}

//
// Creates new instance of the image format writer
//
ImageFormatWriterPtr PNGPlugin::createFileWriter(const QString & ext, QFile & file)
{
	return ImageFormatWriterPtr();
}
