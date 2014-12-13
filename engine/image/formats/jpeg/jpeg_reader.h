#ifndef __IMAGE__FORMATS__JPEG__JPEG_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __IMAGE__FORMATS__JPEG__JPEG_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <image/interfaces/image_format_reader.h>
#include <image/helpers/image_format_reader_helper.h>


/* Classes */

class MemoryFile;

/** JPEG image format reader. */
class JPEGReader : public ImageFormatReaderHelper
{
public:
	/**
	 * Constructor.
	 * @param ext File extension.
	 * @param file Reference to the file.
	 */
	JPEGReader(const QString & ext, MemoryFile & file);

	/**
	 * Reads the image.
	 * @param image Pointer to the image file.
	 */
	void read(ImageFile * image);
};

#endif // __IMAGE__FORMATS__JPEG__JPEG_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
