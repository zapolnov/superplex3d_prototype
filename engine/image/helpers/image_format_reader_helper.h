#ifndef __IMAGE__HELPERS__IMAGE_FORMAT_READER_HELPER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __IMAGE__HELPERS__IMAGE_FORMAT_READER_HELPER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <image/interfaces/image_format_reader.h>
#include <image/image_file.h>


/* Classes */

class MemoryFile;
class QFile;

/** Helper class for image format readers. */
class GCC_PUBLIC ImageFormatReaderHelper : public IImageFormatReader
{
public:
	/**
	 * Constructor.
	 * @param file Reference to the file.
	 */
	IMAGEAPI ImageFormatReaderHelper(MemoryFile & file);

	/** Returns true if this is a valid file. */
	IMAGEAPI bool isValidFile() const;

protected:
	MemoryFile & m_File;	/**< Reference to the file. */
	bool m_IsValid;			/**< True if this is a valid file. */

	/**
	 * Sets depth of the specified image.
	 * @param image Pointer to the image file.
	 * @param depth Depth of the image.
	 */
	inline static void setImageDepth(ImageFile * image, int depth)
	{
		image->m_Depth = depth;
	}

	/**
	 * Sets size of the specified image.
	 * @param image Pointer to the image file.
	 * @param width Width of the image.
	 * @param height Height of the image.
	 */
	inline static void setImageSize(ImageFile * image, int width, int height)
	{
		image->m_Width = width;
		image->m_Height = height;
	}

	/**
	 * Sets number of mip-levels in the specified image.
	 * @param image Pointer to the image file.
	 * @param levels Number of mip-levels.
	 */
	inline static void setImageMipLevels(ImageFile * image, int levels)
	{
		image->m_LevelCount = levels;
	}

	/**
	 * Sets number of cube map faces in the specified image.
	 * @param image Pointer to the image file.
	 * @param faces Number of faces.
	 */
	inline static void setImageFaces(ImageFile * image, int faces)
	{
		image->m_Faces = faces;
	}

	/**
	 * Sets image format.
	 * @param image Pointer to the image file.
	 * @param format Format of the image.
	 */
	inline static void setImageFormat(ImageFile * image, ImageFormat format)
	{
		image->m_Format = format;
	}

	/**
	 * Sets number of bytes in the image element.
	 * @param image Pointer to the image file.
	 * @param size Size of the image element.
	 */
	inline static void setImageElementSize(ImageFile * image, int size)
	{
		image->m_ElementSize = size;
	}

	/**
	 * Appends image mip-level data.
	 * @param image Pointer to the image file.
	 * @param data Pointer to the mip-level data.
	 */
	inline static void appendImageLevelData(ImageFile * image, byte * data)
	{
		image->m_Data.push_back(data);
	}

	/**
	 * Reads data from file.
	 * @param ptr Pointer to the buffer.
	 * @param size Size of the buffer.
	 * @returns true on success, false if not enough data has been read.
	 */
	IMAGEAPI bool tryReadFile(void * ptr, uint size);

	/**
	 * Reads data from file.
	 * @param ptr Pointer to the buffer.
	 * @param size Size of the buffer.
	 */
	IMAGEAPI void readFile(void * ptr, uint size);
};

#endif // __IMAGE__HELPERS__IMAGE_FORMAT_READER_HELPER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
