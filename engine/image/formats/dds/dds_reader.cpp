/****************************************************************************/
/* Portions Copyright (c) NVIDIA Corporation. All rights reserved.			*/
/* Author: Evan Hart														*/
/* Email: sdkfeedback@nvidia.com											*/
/****************************************************************************/
#include <core/common.h>
#include <core/exception.h>
#include <core/logger.h>
#include <core/memory_file.h>
#include "dds_reader.h"

/* Macros */

#ifndef MAKEFOURCC
 #define MAKEFOURCC(c0,c1,c2,c3) \
	((ulong)(byte)(c0)| \
	((ulong)(byte)(c1) << 8)| \
	((ulong)(byte)(c2) << 16)| \
	((ulong)(byte)(c3) << 24))
#endif

#define CHECK_BITMASK(flags, bits, r, g, b, a) \
	(ddsh.ddspf.dwFlags == (flags) && \
	 ddsh.ddspf.dwRGBBitCount == (bits) && \
	 ddsh.ddspf.dwRBitMask == (r) && \
	 ddsh.ddspf.dwGBitMask == (g) && \
	 ddsh.ddspf.dwBBitMask == (b) && \
	 ddsh.ddspf.dwABitMask == (a))


/* Constants */

/** Surface description flag. */
enum SurfaceDescriptionFlag
{
	DDSF_CAPS				= 0x00000001L,
	DDSF_HEIGHT				= 0x00000002L,
	DDSF_WIDTH				= 0x00000004L,
	DDSF_PITCH				= 0x00000008L,
	DDSF_PIXELFORMAT		= 0x00001000L,
	DDSF_MIPMAPCOUNT		= 0x00020000L,
	DDSF_LINEARSIZE			= 0x00080000L,
	DDSF_DEPTH				= 0x00800000L
};

/** Pixel format flag. */
enum PixelFormatFlag
{
	DDSF_ALPHAPIXELS		= 0x00000001L,
	DDSF_ALPHA				= 0x00000002L,
	DDSF_FOURCC				= 0x00000004L,
	DDSF_PALETTEINDEXED4	= 0x00000008L,
	DDSF_PALETTEINDEXEDTO8	= 0x00000010L,
	DDSF_PALETTEINDEXED8	= 0x00000020L,
	DDSF_RGB				= 0x00000040L,
	DDSF_RGBA				= 0x00000041L,
	DDSF_COMPRESSED			= 0x00000080L,
	DDSF_RGBTOYUV			= 0x00000100L,
	DDSF_YUV				= 0x00000200L,
	DDSF_ZBUFFER			= 0x00000400L,
	DDSF_PALETTEINDEXED1	= 0x00000800L,
	DDSF_PALETTEINDEXED2	= 0x00001000L,
	DDSF_ZPIXELS			= 0x00002000L,
	DDSF_STENCILBUFFER		= 0x00004000L,
	DDSF_ALPHAPREMULT		= 0x00008000L,
	DDSF_LUMINANCE			= 0x00020000L,
	DDSF_BUMPLUMINANCE		= 0x00040000L,
	DDSF_BUMPDUDV			= 0x00080000L,
};

/** dwCaps1 flag. */
enum CapsFlag
{
	DDSF_COMPLEX			= 0x00000008L,
	DDSF_TEXTURE			= 0x00001000L,
	DDSF_MIPMAP				= 0x00400000L
};

/** dwCaps2 flag. */
enum CapsFlag2
{
	DDSF_CUBEMAP			= 0x00000200L,
	DDSF_CUBEMAP_POSITIVEX	= 0x00000400L,
	DDSF_CUBEMAP_NEGATIVEX	= 0x00000800L,
	DDSF_CUBEMAP_POSITIVEY	= 0x00001000L,
	DDSF_CUBEMAP_NEGATIVEY	= 0x00002000L,
	DDSF_CUBEMAP_POSITIVEZ	= 0x00004000L,
	DDSF_CUBEMAP_NEGATIVEZ	= 0x00008000L,
	DDSF_CUBEMAP_ALL_FACES	= 0x0000FC00L,
	DDSF_VOLUME				= 0x00200000L
};

/** Compressed texture types. */
enum FourCC
{
	FOURCC_UNKNOWN			= 0,
	FOURCC_R8G8B8			= 20,
	FOURCC_A8R8G8B8			= 21,
	FOURCC_X8R8G8B8			= 22,
	FOURCC_R5G6B5			= 23,
	FOURCC_X1R5G5B5			= 24,
	FOURCC_A1R5G5B5			= 25,
	FOURCC_A4R4G4B4			= 26,
	FOURCC_R3G3B2			= 27,
	FOURCC_A8				= 28,
	FOURCC_A8R3G3B2			= 29,
	FOURCC_X4R4G4B4			= 30,
	FOURCC_A2B10G10R10		= 31,
	FOURCC_A8B8G8R8			= 32,
	FOURCC_X8B8G8R8			= 33,
	FOURCC_G16R16			= 34,
	FOURCC_A2R10G10B10		= 35,
	FOURCC_A16B16G16R16		= 36,
	FOURCC_L8				= 50,
	FOURCC_A8L8				= 51,
	FOURCC_A4L4				= 52,
	FOURCC_DXT1				= MAKEFOURCC('D','X','T','1'),
	FOURCC_DXT2				= MAKEFOURCC('D','X','T','2'),
	FOURCC_DXT3				= MAKEFOURCC('D','X','T','3'),
	FOURCC_DXT4				= MAKEFOURCC('D','X','T','4'),
	FOURCC_DXT5				= MAKEFOURCC('D','X','T','5'),
	FOURCC_ATI1				= MAKEFOURCC('A','T','I','1'),
	FOURCC_ATI2				= MAKEFOURCC('A','T','I','2'),
	FOURCC_D16_LOCKABLE		= 70,
	FOURCC_D32				= 71,
	FOURCC_D24X8			= 77,
	FOURCC_D16				= 80,
	FOURCC_D32F_LOCKABLE	= 82,
	FOURCC_L16				= 81,
	FOURCC_R16F				= 111,
	FOURCC_G16R16F			= 112,
	FOURCC_A16B16G16R16F	= 113,
	FOURCC_R32F				= 114,
	FOURCC_G32R32F			= 115,
	FOURCC_A32B32G32R32F	= 116
};


/* Structures */

/** DXT compression color block. */
struct DXTColBlock
{
	ushort col0;
	ushort col1;
	byte row[4];
};

/** DXT3 alpha block. */
struct DXT3AlphaBlock
{
	ushort row[4];
};

/** DXT5 alpha block. */
struct DXT5AlphaBlock
{
	byte alpha0;
	byte alpha1;
	byte row[6];
};

/** DDS pixel format description. */
struct DDS_PIXELFORMAT
{
	ulong dwSize;
	ulong dwFlags;
	ulong dwFourCC;
	ulong dwRGBBitCount;
	ulong dwRBitMask;
	ulong dwGBitMask;
	ulong dwBBitMask;
	ulong dwABitMask;
};

/** DDS file header. */
struct DDS_HEADER
{
	ulong dwSize;
	ulong dwFlags;
	ulong dwHeight;
	ulong dwWidth;
	ulong dwPitchOrLinearSize;
	ulong dwDepth;
	ulong dwMipMapCount;
	ulong dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	ulong dwCaps1;
	ulong dwCaps2;
	ulong dwReserved2[3];
};


/* DDSReader methods */

//
// Constructor
//
DDSReader::DDSReader(const QString & ext, MemoryFile & file)
	: ImageFormatReaderHelper(file)
{
	char marker[4];
	m_IsValid = tryReadFile(marker, sizeof(marker)) && !memcmp(marker, "DDS ", sizeof(marker));
}

//
// Reads the image
//
void DDSReader::read(ImageFile * image)
{
	if (unlikely(!m_IsValid))
		throw Exception(QObject::tr("\"%1\" is not a valid DDS file.").arg(m_File.fileName()));

	// Read in DDS header
	DDS_HEADER ddsh;
	readFile(&ddsh, sizeof(ddsh));

	// Check if image is a volume texture
	int depth = ((ddsh.dwCaps2 & DDSF_VOLUME) != 0 && ddsh.dwDepth > 0) ? ddsh.dwDepth : 0;
	setImageDepth(image, depth);

	// There are flags that are supposed to mark these fields as valid, but some dds files don't set them properly
	int width = ddsh.dwWidth;
	int height = ddsh.dwHeight;
	setImageSize(image, width, height);

	// Sets number of mip-levels in the specified image
	int levelCount = (ddsh.dwFlags & DDSF_MIPMAPCOUNT) ? ddsh.dwMipMapCount : 1;
	setImageMipLevels(image, levelCount);

	// Check cube-map faces
	int faces = 0;
	if ((ddsh.dwCaps2 & DDSF_CUBEMAP) == 0)
		setImageFaces(image, 0);
	else
	{
		// This is a cubemap, count the faces
		faces += (ddsh.dwCaps2 & DDSF_CUBEMAP_POSITIVEX) ? 1 : 0;
		faces += (ddsh.dwCaps2 & DDSF_CUBEMAP_NEGATIVEX) ? 1 : 0;
		faces += (ddsh.dwCaps2 & DDSF_CUBEMAP_POSITIVEY) ? 1 : 0;
		faces += (ddsh.dwCaps2 & DDSF_CUBEMAP_NEGATIVEY) ? 1 : 0;
		faces += (ddsh.dwCaps2 & DDSF_CUBEMAP_POSITIVEZ) ? 1 : 0;
		faces += (ddsh.dwCaps2 & DDSF_CUBEMAP_NEGATIVEZ) ? 1 : 0;
		setImageFaces(image, faces);

		// Check for a complete cubemap
		if (unlikely(faces != 6 || width != height))
			throw Exception(QObject::tr("\"%1\": Incomplete cubemap").arg(m_File.fileName()));
	}

	bool btcCompressed = false, setAlpha = false, addAlpha = false, swapRedBlue = false;
	int bytesPerElement = 0;

	// Figure out what the image format is
	ImageFormat format;
	if (ddsh.ddspf.dwFlags & DDSF_FOURCC) 
	{
		switch (ddsh.ddspf.dwFourCC)
		{
		case FOURCC_DXT1:
			logger << LOG_INFO << "%s: BC1_UNORM" << m_File.fileName();
			format = BC1_UNORM;
			bytesPerElement = 8;
			btcCompressed = true;
			break;

		case FOURCC_DXT2:
		case FOURCC_DXT3:
			logger << LOG_INFO << "%s: BC2_UNORM" << m_File.fileName();
			format = BC2_UNORM;
			bytesPerElement = 16;
			btcCompressed = true;
			break;

		case FOURCC_DXT4:
		case FOURCC_DXT5:
			logger << LOG_INFO << "%s: BC3_UNORM" << m_File.fileName();
			format = BC3_UNORM;
			bytesPerElement = 16;
			btcCompressed = true;
			break;

		case FOURCC_ATI1:
			logger << LOG_INFO << "%s: BC4_UNORM" << m_File.fileName();
			format = BC4_UNORM;
			bytesPerElement = 8;
			btcCompressed = true;
			break;

		case FOURCC_ATI2:
			logger << LOG_INFO << "%s: BC5_UNORM" << m_File.fileName();
			format = BC5_UNORM;
			bytesPerElement = 16;
			btcCompressed = true;
			break;

		case FOURCC_R16F:
			logger << LOG_INFO << "%s: R16_FLOAT" << m_File.fileName();
			format = R16_FLOAT;
			bytesPerElement = 2;
			break;

		case FOURCC_R32F:
			logger << LOG_INFO << "%s: R32_FLOAT" << m_File.fileName();
			format = R32_FLOAT;
			bytesPerElement = 4;
			break;

		case FOURCC_A16B16G16R16F:
			logger << LOG_INFO << "%s: RGBA16_FLOAT" << m_File.fileName();
			format = RGBA16_FLOAT;
			bytesPerElement = 8;
			break;

		case FOURCC_A32B32G32R32F:
			logger << LOG_INFO << "%s: RGBA32_FLOAT" << m_File.fileName();
			format = RGBA32_FLOAT;
			bytesPerElement = 16;
			break;

		case FOURCC_G16R16F:
			logger << LOG_INFO << "%s: RG16_FLOAT" << m_File.fileName();
			format = RG16_FLOAT;
			bytesPerElement = 4;
			break;

		case FOURCC_G32R32F:
			logger << LOG_INFO << "%s: RG32_FLOAT" << m_File.fileName();
			format = RG32_FLOAT;
			bytesPerElement = 8;
			break;

		case FOURCC_A16B16G16R16:
			logger << LOG_INFO << "%s: RGBA16_UNORM" << m_File.fileName();
			format = RGBA16_UNORM;
			bytesPerElement = 8;
			break;

		case FOURCC_UNKNOWN:
		case FOURCC_R8G8B8:
		case FOURCC_A8R8G8B8:
		case FOURCC_X8R8G8B8:
		case FOURCC_R5G6B5:
		case FOURCC_X8B8G8R8:
		case FOURCC_A2R10G10B10:
		case FOURCC_A2B10G10R10:
		case FOURCC_L8:
		case FOURCC_A8L8:
		case FOURCC_L16:
		case FOURCC_G16R16:
		case FOURCC_X1R5G5B5:
		case FOURCC_A1R5G5B5:
		case FOURCC_A4R4G4B4:
		case FOURCC_R3G3B2:
		case FOURCC_A8R3G3B2:
		case FOURCC_X4R4G4B4:
		case FOURCC_A4L4:
		case FOURCC_A8:
		case FOURCC_A8B8G8R8:
		case FOURCC_D16_LOCKABLE:
		case FOURCC_D32:
		case FOURCC_D24X8:
		case FOURCC_D16:
		case FOURCC_D32F_LOCKABLE:
		default:
			throw Exception(QObject::tr("File \"%1\" has unsupported FOURCC format.").arg(m_File.fileName()));
		}
	}
	else if (CHECK_BITMASK(DDSF_RGBA, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000))
	{
		logger << LOG_INFO << "%s: RGBA8_UNORM (converted from %s)" << m_File.fileName() << "BGRA8_UNORM";
		format = RGBA8_UNORM;
		swapRedBlue = true;
		bytesPerElement = 4;
	}
	else if (CHECK_BITMASK(DDSF_RGBA, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000))
	{
		logger << LOG_INFO << "%s: RGBA8_UNORM" << m_File.fileName();
		format = RGBA8_UNORM;
		bytesPerElement = 4;
	}
	else if (CHECK_BITMASK(DDSF_RGBA, 32, 0x3FF00000, 0x000FFC00, 0x000003FF, 0xC0000000))
	{
		logger << LOG_INFO << "%s: RGB10_A2" << m_File.fileName();
		format = RGB10_A2;
		bytesPerElement = 4;
	}
	else if (CHECK_BITMASK(DDSF_RGB, 32, 0x00FF0000, 0x0000FF00, 0x00000000FF, 0x00000000))
	{
		logger << LOG_INFO << "%s: RGBA8_UNORM (converted from %s)" << m_File.fileName() << "BGRX8_UNORM";
		format = RGBA8_UNORM;
		swapRedBlue = true;
		setAlpha = true;
		bytesPerElement = 4;
	}
	else if (CHECK_BITMASK(DDSF_RGB, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000))
	{
		logger << LOG_INFO << "%s: RGBA8_UNORM (converted from %s)" << m_File.fileName() << "RGBX8_UNORM";
		format = RGBA8_UNORM;
		setAlpha = true;
		bytesPerElement = 4;
	}
	else if (CHECK_BITMASK(DDSF_RGB, 32, 0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000))
	{
		logger << LOG_INFO << "%s: RG16_UNORM" << m_File.fileName();
		format = RG16_UNORM;
		bytesPerElement = 4;
	}
	else if (CHECK_BITMASK(DDSF_RGB, 24, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000))
	{
		logger << LOG_INFO << "%s: RGBA8_UNORM (converted from %s)" << m_File.fileName() << "BGR8_UNORM";
		format = RGBA8_UNORM;
		addAlpha = true;
		swapRedBlue = true;
        bytesPerElement = 4;
	}
	else if (CHECK_BITMASK(DDSF_RGB, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000))
	{
		logger << LOG_INFO << "%s: RGBA8_UNORM (converted from %s)" << m_File.fileName() << "RGB8_UNORM";
		format = RGBA8_UNORM;
		addAlpha = true;
		bytesPerElement = 4;
	}
	else
		throw Exception(QObject::tr("File \"%1\" has unsupported DDS surface format.").arg(m_File.fileName()));
	setImageFormat(image, format);

	setImageElementSize(image, bytesPerElement);
	for (int face = 0; face < (faces ? faces : 1); face++)
	{
		int w = width, h = height, d = (depth ? depth : 1);

		for (int level = 0; level < levelCount; level++)
		{
			int bw = btcCompressed ? (w + 3) / 4 : w;
			int bh = btcCompressed ? (h + 3) / 4 : h;
			int num_pixels = bw * bh * d;
			int size = num_pixels * bytesPerElement;
			byte * data = new byte[size];

			try
			{
				if (!addAlpha)
					readFile(data, size);
				else
				{
					int file_size = num_pixels * 3;
					readFile(data, file_size);

					for (int i = num_pixels - 1; i >=0; i--)
					{
						byte * src = data + i * 3;
						byte * dst = data + i * 4;

						dst[0] = src[0];
						dst[1] = src[1];
						dst[2] = src[2];
						dst[3] = 0xFF;
					}
				}

				if (swapRedBlue)
				{
					for (int i = 0; i < num_pixels * 4; i += 4)
					{
						byte tmp = data[i];
						data[i] = data[i + 2];
						data[i + 2] = tmp;
					}
				}

				if (setAlpha)
				{
					for (int i = 0; i < num_pixels * 4; i += 4)
						data[i + 3] = 0xFF;
				}

				appendImageLevelData(image, data);
			}
			catch (...)
			{
				delete[] data;
				throw;
			}

//			if (faces != 6)
//				i.flipSurface( data, w, h, d);

			// Reduce mip sizes
			w = (w > 1 ? w >> 1 : 1);
			h = (h > 1 ? h >> 1 : 1);
			d = (d > 1 ? d >> 1 : 1);
		}
	}

/*
    //reverse cube map y faces
    if (i._faces == 6) {
        for (int level = 0; level < i._levelCount; level++) {
            GLubyte *temp = i._data[2*i._levelCount + level];
            i._data[2*i._levelCount + level] = i._data[3*i._levelCount + level];
            i._data[3*i._levelCount + level] = temp;
        }
    }
  */
}

#if 0
//
// flip a DXT1 color block
////////////////////////////////////////////////////////////
void Image::flip_blocks_dxtc1(GLubyte *ptr, unsigned int numBlocks)
{
    DXTColBlock *curblock = (DXTColBlock*)ptr;
    GLubyte temp;

    for (unsigned int i = 0; i < numBlocks; i++) {
        temp = curblock->row[0];
        curblock->row[0] = curblock->row[3];
        curblock->row[3] = temp;
        temp = curblock->row[1];
        curblock->row[1] = curblock->row[2];
        curblock->row[2] = temp;

        curblock++;
    }
}

//
// flip a DXT3 color block
////////////////////////////////////////////////////////////
void Image::flip_blocks_dxtc3(GLubyte *ptr, unsigned int numBlocks)
{
    DXTColBlock *curblock = (DXTColBlock*)ptr;
    DXT3AlphaBlock *alphablock;
    GLushort tempS;
    GLubyte tempB;

    for (unsigned int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT3AlphaBlock*)curblock;

        tempS = alphablock->row[0];
        alphablock->row[0] = alphablock->row[3];
        alphablock->row[3] = tempS;
        tempS = alphablock->row[1];
        alphablock->row[1] = alphablock->row[2];
        alphablock->row[2] = tempS;

        curblock++;

        tempB = curblock->row[0];
        curblock->row[0] = curblock->row[3];
        curblock->row[3] = tempB;
        tempB = curblock->row[1];
        curblock->row[1] = curblock->row[2];
        curblock->row[2] = tempB;

        curblock++;
    }
}

//
// flip a DXT5 alpha block
////////////////////////////////////////////////////////////
void flip_dxt5_alpha(DXT5AlphaBlock *block)
{
    GLubyte gBits[4][4];

    const unsigned long mask = 0x00000007;          // bits = 00 00 01 11
    unsigned long bits = 0;
    memcpy(&bits, &block->row[0], sizeof(unsigned char) * 3);

    gBits[0][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[0][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[0][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[0][3] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][3] = (GLubyte)(bits & mask);

    bits = 0;
    memcpy(&bits, &block->row[3], sizeof(GLubyte) * 3);

    gBits[2][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[2][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[2][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[2][3] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][3] = (GLubyte)(bits & mask);

    // clear existing alpha bits
    memset(block->row, 0, sizeof(GLubyte) * 6);

    unsigned long *pBits = ((unsigned long*) &(block->row[0]));

    *pBits = *pBits | (gBits[3][0] << 0);
    *pBits = *pBits | (gBits[3][1] << 3);
    *pBits = *pBits | (gBits[3][2] << 6);
    *pBits = *pBits | (gBits[3][3] << 9);

    *pBits = *pBits | (gBits[2][0] << 12);
    *pBits = *pBits | (gBits[2][1] << 15);
    *pBits = *pBits | (gBits[2][2] << 18);
    *pBits = *pBits | (gBits[2][3] << 21);

    pBits = ((unsigned long*) &(block->row[3]));

    *pBits = *pBits | (gBits[1][0] << 0);
    *pBits = *pBits | (gBits[1][1] << 3);
    *pBits = *pBits | (gBits[1][2] << 6);
    *pBits = *pBits | (gBits[1][3] << 9);

    *pBits = *pBits | (gBits[0][0] << 12);
    *pBits = *pBits | (gBits[0][1] << 15);
    *pBits = *pBits | (gBits[0][2] << 18);
    *pBits = *pBits | (gBits[0][3] << 21);
}

//
// flip a DXT5 color block
////////////////////////////////////////////////////////////
void Image::flip_blocks_dxtc5(GLubyte *ptr, unsigned int numBlocks)
{
    DXTColBlock *curblock = (DXTColBlock*)ptr;
    DXT5AlphaBlock *alphablock;
    GLubyte temp;

    for (unsigned int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT5AlphaBlock*)curblock;

        flip_dxt5_alpha(alphablock);

        curblock++;

        temp = curblock->row[0];
        curblock->row[0] = curblock->row[3];
        curblock->row[3] = temp;
        temp = curblock->row[1];
        curblock->row[1] = curblock->row[2];
        curblock->row[2] = temp;

        curblock++;
    }
}

#endif
