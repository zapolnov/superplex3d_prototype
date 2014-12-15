#include <core/exception.h>
#include <core/logger.h>
#include <core/memory_file.h>
#include <png.h>
#include "png_reader.h"
#include <stdlib.h>

/* Private functions */

//
// Reads data from the input file
//
static
void pngReadFunc(png_struct * pngPtr, png_byte * data, png_size_t bytes)
{
	MemoryFile * file = (MemoryFile *)pngPtr->io_ptr;
	Q_ASSERT(file);

	int64 result = file->read((char *)data, bytes);
	if (unlikely(result != (int64)bytes))
		png_error(pngPtr, "I/O error");
}

//
// Error handling function for libpng
//
static
void pngErrorFunc(png_struct *, const char * message)
{
	logger << LOG_ERROR << "%s" << message;
	abort();
}

//
// Warning handling function for libpng
//
static
void pngWarningFunc(png_struct *, const char * message)
{
	logger << LOG_WARN << "%s" << message;
}


/* PNGReader methods */

//
// Constructor
//
PNGReader::PNGReader(const QString & ext, MemoryFile & file)
	: ImageFormatReaderHelper(file)
{
	if (file.size() < 8)
		return;

	byte buf[8];
	if (!tryReadFile(buf, sizeof(buf)))
		return;

	if (png_check_sig((png_bytep)buf, sizeof(buf)))
		m_IsValid = true;
}

//
// Reads the image
//
void PNGReader::read(ImageFile * image)
{
	volatile unsigned char ** volatile rowPointers = NULL;
	png_struct * pngPtr = NULL;
	png_info * infoPtr = NULL;
	byte * data = NULL;

	try
	{
		int colorType, bitDepth;
		bool hasAlpha = false;
		uint bytesPerElement;
		ImageFormat format;
		unsigned long w, h;
		unsigned rowbytes;

		// Initialize the PNG library
		pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngErrorFunc, pngWarningFunc);
		if (unlikely(!pngPtr))
			throw Exception(QString("%1: png_create_read_struct() failed").arg(m_File.fileName()));

		// Initialize the PNG info struct
		infoPtr = png_create_info_struct(pngPtr);
		if (unlikely(!infoPtr))
			throw Exception(QString("%1: png_create_info_struct() failed").arg(m_File.fileName()));

		// Setup error handler
		if (unlikely(setjmp(png_jmpbuf(pngPtr)) != 0))
			throw Exception(QString("%1: PNG decompression failed").arg(m_File.fileName()));

		// Setup file reading
		png_set_read_fn(pngPtr, &m_File, pngReadFunc);
		png_set_sig_bytes(pngPtr, 8);
		png_read_info(pngPtr, infoPtr);
		png_get_IHDR(pngPtr, infoPtr, &w, &h, &bitDepth, &colorType, NULL, NULL, NULL);

		// Setup 16 bit -> 8 bit conversion
		if (bitDepth > 8) 
			png_set_strip_16(pngPtr);

		// Translate transparency to alpha channel
  		if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(pngPtr);
			hasAlpha = true;
		}

		// Setup gamma correction
		double gamma;
		if (png_get_gAMA(pngPtr, infoPtr, &gamma))
			png_set_gamma(pngPtr, 2.2, gamma);
		else
			png_set_gamma(pngPtr, 2.2, 0.45455);

		// We want at least 8-bit
		if (bitDepth < 8)
		{
			if ((colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA))
				png_set_gray_1_2_4_to_8(pngPtr);
			else
				png_set_packing(pngPtr);
		}

		// Convert paletted images to RGB
		if (colorType == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(pngPtr);
			colorType = hasAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB;
		}

		// Convert gray/alpha images to RGB
		if (colorType == PNG_COLOR_TYPE_GRAY_ALPHA ||
			(colorType == PNG_COLOR_TYPE_GRAY && hasAlpha))
		{
			png_set_gray_to_rgb(pngPtr);
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
		}

		// Add dummy alpha channel for RGB images
		if (colorType == PNG_COLOR_TYPE_RGB && !hasAlpha)
		{
			png_set_filler(pngPtr, 0xFF, PNG_FILLER_BEFORE);
			hasAlpha = true;
		}
		else if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
			hasAlpha = true;

		// Select pixel format
		switch (colorType)
		{
		case PNG_COLOR_TYPE_PALETTE:
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			Q_ASSERT(false);	// Should be handled in the code above
			break;

		case PNG_COLOR_TYPE_GRAY:
			bytesPerElement = 1;
			format = R8_UNORM;
			break;

		default:
			Q_ASSERT(hasAlpha);	// Should be handled in the code above
			bytesPerElement = 4;
			format = RGBA8_UNORM;
			break;
		}

		// Re-configure PNG reading to include configured conversions
		png_read_update_info(pngPtr, infoPtr);
		rowbytes = png_get_rowbytes(pngPtr, infoPtr);

		// Set basic image info
		setImageSize(image, w, h);
		setImageDepth(image, 0);
		setImageMipLevels(image, 1);
		setImageFaces(image, 0);
		setImageFormat(image, format);
		setImageElementSize(image, bytesPerElement);

		// Allocate memory for image data
		data = new byte[w * h * bytesPerElement];
		if (unlikely(!data))
			throw Exception("Out of memory.");

		// Allocate array of row pointers
		rowPointers = new volatile unsigned char * [h];
		for (unsigned j = 0; j < h; j++)
			rowPointers[j] = data + j * rowbytes;

		// Read the image
		png_read_image(pngPtr, (png_byte **)rowPointers);

		// Store image information
		appendImageLevelData(image, data);
		data = NULL;

		// Free memory
		png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
		pngPtr = NULL;
	}
	catch (...)
	{
		delete[] rowPointers;
		delete[] data;

		if (pngPtr)
		{
			if (infoPtr)
				png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
			else
				png_destroy_read_struct(&pngPtr, NULL, NULL);
		}	

		throw;
	}

	delete[] rowPointers;
}
