#ifndef __CORE__EXT_IO_DEVICE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__EXT_IO_DEVICE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <core/ref_counted.h>
#include <core/pointer.h>


/* Classes */

/** Descendant of QIODevice with support for filename storage. */
class ExtIODevice : public QIODevice, public RefCounted
{
public:
	/**
	 * Constructor.
	 * @param name Name of the file.
	 */
	inline ExtIODevice(const QString & name)
		: QIODevice(), m_FileName(name)
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

/** Pointer to the ExtIODevice. */
typedef Pointer<ExtIODevice> FilePtr;

#endif // __CORE__EXT_IO_DEVICE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
