#ifndef __IMAGE__INTERFACES__IMAGE_FORMAT_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __IMAGE__INTERFACES__IMAGE_FORMAT_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <image/interfaces/image_format_reader.h>
#include <image/interfaces/image_format_writer.h>
#include <image/image_file.h>


/* Classes */

class QString;
class QFile;
class MemoryFile;

/** Interface to the image format plugin. */
class IImageFormatPlugin
{
public:
	/** Destructor. */
	virtual ~IImageFormatPlugin()
	{
		ImageFile::unregisterImageFormatPlugin(this);
	}

	/**
	 * Creates new instance of the image format reader.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the reader or NULL if this file cannot be parsed by this plugin.
	 */
	virtual ImageFormatReaderPtr createFileReader(const QString & ext, MemoryFile & file) = 0;

	/**
	 * Creates new instance of the image format writer.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the writer or NULL if this file cannot be parsed by this plugin.
	 */
	virtual ImageFormatWriterPtr createFileWriter(const QString & ext, QFile & file) = 0;
};

#endif // __IMAGE__INTERFACES__IMAGE_FORMAT_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
