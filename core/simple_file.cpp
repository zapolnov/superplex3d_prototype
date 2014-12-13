#include <core/simple_file.h>
#include <core/exception.h>

/* SimpleFile methods */

//
// Constructor
//
SimpleFile::SimpleFile(const QString & path)
	: ExtIODevice(path), m_File(path)
{
	if (unlikely(!m_File.open(QFile::ReadOnly)))
	{
		throw Exception(QString("Unable to open file \"%1\": %2")
			.arg(path).arg(m_File.errorString()));
	}
}

//
// Destructor
//
SimpleFile::~SimpleFile()
{
}

//
// Closes the file
//
void SimpleFile::close()
{
	m_File.close();
}

//
// Sets current position in the file
//
bool SimpleFile::seek(qint64 pos)
{
	return m_File.seek(pos);
}

//
// Returns current position in the file
//
qint64 SimpleFile::pos() const
{
	return m_File.pos();
}

//
// Returns size of the file
//
qint64 SimpleFile::size() const
{
	return m_File.size();
}

//
// Opens the file
//
bool SimpleFile::open(int mode)
{
	Q_ASSERT_MSG(false, "SimpleFile::open() should never be called.");
	return true;
}

//
// Reads data from the device
//
qint64 SimpleFile::readData(char * data, qint64 maxSize)
{
	return m_File.read(data, maxSize);
}

//
// Writes data from the device
//
qint64 SimpleFile::writeData(const char * data, qint64 maxSize)
{
	return -1;
}
