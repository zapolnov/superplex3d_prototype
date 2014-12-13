#include <core/exception.h>
#include <core/trolltech.h>
#include "riff.h"

/* RiffFile methods */

//
// Constructor
//
RiffFile::RiffFile(const QString & name, QIODevice * device)
	: m_FileName(name), m_Device(device)
{
	// Read the RIFF chunk
	if (m_Device->read((char *)&m_CurrentChunk, sizeof(m_CurrentChunk)) != sizeof(m_CurrentChunk))
		throw Exception(QString("%1: Not a RIFF file.").arg(m_FileName));
	if (m_CurrentChunk.id != FOURCC_RIFF)
		throw Exception(QString("%1: Not a RIFF file.").arg(m_FileName));

	// Check file size
	if (m_CurrentChunk.size < m_Device->size() - m_Device->pos())
		throw Exception(QString("%1: Corrupt RIFF file.").arg(m_FileName));

	// Read file format
	uint32 format;
	if (m_CurrentChunk.size < sizeof(uint32))
		throw Exception(QString("%1: RIFF chunk is too small.").arg(m_FileName));
	if (device->read((char *)&format, sizeof(format)) != sizeof(format))
		throw Exception(QString("%1: I/O error.").arg(m_FileName));
	m_Format = (RiffFormat)format;

	// Store the starting offset
	m_RiffChunkBegin = m_Device->pos();
	if (unlikely(m_Device->pos() < 0))
		throw Exception(QString("%1: I/O error.").arg(m_FileName));
	m_RiffChunkSize = m_CurrentChunk.size;
}

//
// Destructor
//
RiffFile::~RiffFile()
{
}

//
// Moves file pointer to the beginning of the RIFF chunk
//
void RiffFile::seekToStart()
{
	if (unlikely(!m_Device->seek(m_RiffChunkBegin)))
		throw Exception(QString("%1: Seek failed.").arg(m_FileName));
}

//
// Moves file pointer to the next chunk with the specified ID
//
bool RiffFile::seekToChunk(uint32 fourCC)
{
	RiffChunk chunk;

	seekToStart();
	while (m_Device->pos() < (int64)m_RiffChunkBegin + (int64)m_RiffChunkSize - (int64)sizeof(RiffChunk))
	{
		if (m_Device->read((char *)&chunk, sizeof(chunk)) != sizeof(chunk))
			throw Exception(QString("%1: I/O error.").arg(m_FileName));

		if (chunk.id == fourCC)
		{
			m_CurrentChunk = chunk;
			return true;
		}

		int64 pos = m_Device->pos();
		if (unlikely(pos < 0))
			throw Exception(QString("%1: I/O error.").arg(m_FileName));
		pos += chunk.size + (chunk.size & 1);
		if (unlikely(!m_Device->seek(pos)))
			throw Exception(QString("%1: Seek failed.").arg(m_FileName));
	}

	m_CurrentChunk.id = 0;
	m_CurrentChunk.size = 0;

	return false;
}
