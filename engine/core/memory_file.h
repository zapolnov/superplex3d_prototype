#ifndef __CORE__MEMORY_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__MEMORY_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>


/* Classes */

/** Descendant of QBuffer with support for filename storage. */
class MemoryFile : public QBuffer
{
public:
	/**
	 * Constructor.
	 * @param name Name of the file.
	 * @param data File data.
	 */
	inline MemoryFile(const QString & name, const QByteArray & data)
		: QBuffer(const_cast<QByteArray *>(&data)), m_FileName(name)
	{
	}

	/** Returns name of the file. */
	inline const QString & fileName() const
	{
		return m_FileName;
	}

private:
	QString m_FileName;		/**< Name of the file. */
};

#endif // __CORE__MEMORY_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
