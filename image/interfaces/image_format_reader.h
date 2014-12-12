#ifndef __IMAGE__INTERFACES__IMAGE_FORMAT_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __IMAGE__INTERFACES__IMAGE_FORMAT_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>


/* Classes */

class ImageFile;

/** Interface to the image format reader. */
class IImageFormatReader : public RefCounted
{
public:
	/** Returns true if this is a valid file. */
	virtual bool isValidFile() const = 0;

	/**
	 * Reads the image.
	 * @param image Pointer to the image file.
	 */
	virtual void read(ImageFile * image) = 0;
};

/** Pointer to the image format reader. */
typedef Pointer<IImageFormatReader> ImageFormatReaderPtr;

#endif // __IMAGE__INTERFACES__IMAGE_FORMAT_READER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
