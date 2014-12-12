#include <image/helpers/image_format_reader_helper.h>
#include <core/exception.h>
#include <core/memory_file.h>

/* ImageFormatReaderHelper methods */

//
// Constructor
//
ImageFormatReaderHelper::ImageFormatReaderHelper(MemoryFile & file)
	: m_File(file), m_IsValid(false)
{
}

//
// Returns true if this is a valid file
//
bool ImageFormatReaderHelper::isValidFile() const
{
	return m_IsValid;
}

//
// Reads data from file
//
bool ImageFormatReaderHelper::tryReadFile(void * ptr, uint size)
{
	qint64 result = m_File.read((char *)ptr, size);

	if (unlikely(result < 0))
		throw Exception(QObject::tr("I/O error in file \"%1\": %2").arg(m_File.fileName()).arg(m_File.errorString()));

	return result == (qint64)size;
}

//
// Reads data from file
//
void ImageFormatReaderHelper::readFile(void * ptr, uint size)
{
	if (unlikely(!tryReadFile(ptr, size)))
		throw Exception(QObject::tr("Unexpected end of file \"%1\"").arg(m_File.fileName()));
}
