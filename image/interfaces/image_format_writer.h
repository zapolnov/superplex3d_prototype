#ifndef __IMAGE__INTERFACES__IMAGE_FORMAT_WRITER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __IMAGE__INTERFACES__IMAGE_FORMAT_WRITER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>


/* Classes */

class ImageFile;

/** Interface to the image format writer. */
class IImageFormatWriter : public RefCounted
{
public:
	/**
	 * Saves the image.
	 * @param image Pointer to the image file.
	 */
	virtual void write(const ImageFile * image) = 0;
};

/** Pointer to the image format writer. */
typedef Pointer<IImageFormatWriter> ImageFormatWriterPtr;

#endif // __IMAGE__INTERFACES__IMAGE_FORMAT_WRITER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
