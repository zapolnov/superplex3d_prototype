#include <core/exception.h>
#include <core/trolltech.h>
#include <mesh/helpers/mesh_format_reader_helper.h>

/* MeshFormatReaderHelper methods */

//
// Constructor
//
MeshFormatReaderHelper::MeshFormatReaderHelper(MemoryFile & file)
	: m_File(file), m_IsValid(false)
{
}

//
// Returns true if this is a valid file
//
bool MeshFormatReaderHelper::isValidFile() const
{
	return m_IsValid;
}

//
// Reads data from file
//
bool MeshFormatReaderHelper::tryReadFile(void * ptr, uint size)
{
	qint64 result = m_File.read((char *)ptr, size);

	if (unlikely(result < 0))
		throw Exception(QObject::tr("I/O error in file \"%1\": %2").arg(m_File.fileName()).arg(m_File.errorString()));

	return result == (qint64)size;
}

//
// Reads data from file
//
void MeshFormatReaderHelper::readFile(void * ptr, uint size)
{
	if (unlikely(!tryReadFile(ptr, size)))
		throw Exception(QObject::tr("Unexpected end of file \"%1\"").arg(m_File.fileName()));
}
