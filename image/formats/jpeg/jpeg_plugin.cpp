#include <core/common.h>
#include "jpeg_plugin.h"
#include "jpeg_reader.h"

/* JPEGPlugin methods */

//
// Creates new instance of the image format reader
//
ImageFormatReaderPtr JPEGPlugin::createFileReader(const QString & ext, MemoryFile & file)
{
	return new JPEGReader(ext, file);
}

//
// Creates new instance of the image format writer
//
ImageFormatWriterPtr JPEGPlugin::createFileWriter(const QString & ext, QFile & file)
{
	return ImageFormatWriterPtr();
}
