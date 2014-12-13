#ifndef __IMAGE__FORMATS__DDS__DDS_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __IMAGE__FORMATS__DDS__DDS_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <image/interfaces/image_format_plugin.h>


/* Classes */

/** DDS image format plugin. */
class DDSPlugin : public IImageFormatPlugin
{
public:
	/**
	 * Creates new instance of the image format reader.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the reader or NULL if this file cannot be parsed by this plugin.
	 */
	ImageFormatReaderPtr createFileReader(const QString & ext, MemoryFile & file);

	/**
	 * Creates new instance of the image format writer.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 * @returns pointer to the instance of the writer or NULL if this file cannot be parsed by this plugin.
	 */
	ImageFormatWriterPtr createFileWriter(const QString & ext, QFile & file);
};

#endif // __IMAGE__FORMATS__DDS__DDS_PLUGIN_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
