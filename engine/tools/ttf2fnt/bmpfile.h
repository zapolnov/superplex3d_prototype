#ifndef __BMPFILE_H__
#define __BMPFILE_H__

/* Includes */

#include <core/common.h>


/* Constants */

static const uint16 BmpFileMagic = 0x4D42;	// 'BM'


/* Structures */

#pragma pack(push, 1)

/** Header of the Windows BMP file. */
struct BmpFileHeader
{
	uint16 	bfType;				/**< Magic number. */
	uint32	bfSize;				/**< File size. */
	uint16	bfReserved1;		/**< Reserved field. */
	uint16	bfReserved2;   		 /**< Reserved field. */
	uint32	bfOffBits;			/**< Offset of start of image data from the beginning of the file. */
	uint32	biSize;				/**< sizeof(BmpFileHeader) - 14. */
	uint32	biWidth;			/**< Width of the image. */
	uint32	biHeight;			/**< Height of the image. */
	uint16	biPlanes;			/**< Number of bit planes in the image. */
	uint16	biBitCount;			/**< Number of bits per pixel. */
	uint32	biCompression;		/**< Compression method used. */
	uint32	biSizeImage;		/**< Image size. */
	uint32	biXPelsPerMeter;	/**< Image horizontal resolution. */
	uint32	biYPelsPerMeter;    /**< Image vertical resolution. */
	uint32	biClrUsed;			/**< Number of colors in the palette that are actually used. */
	uint32	biClrImportant;		/**< Number of important colors in the palette. */
};

#pragma pack(pop)

#endif // __IMAGE__FORMATS__BMPFILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
