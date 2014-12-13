/****************************************************************************/
/* Portions Copyright (c) NVIDIA Corporation. All rights reserved.			*/
/* Author: Evan Hart														*/
/* Email: sdkfeedback@nvidia.com											*/
/****************************************************************************/
#ifndef __IMAGE__IMAGE_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __IMAGE__IMAGE_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>
#include <core/trolltech.h>
#include <interfaces/loadable.h>
#include <tbb/spin_rw_mutex.h>
#include <set>
#include <vector>


/* Constants */

/** Image pixel formats. */
enum ImageFormat
{
	A8_UNORM = 0,					/**< 8-bit unsigned normalized ALPHA texture. */
	RGBA32_UINT,					/**< 4 x 32-bit unsigned RGBA texture. */
	RGBA32_SINT,					/**< 4 x 32-bit signed RGBA texture. */
	RGBA32_FLOAT,					/**< 4 x 32-bit floating point RGBA texture. */
	RGBA16_UNORM,					/**< 4 x 16-bit unsigned normalized RGBA texture. */
	RGBA16_UINT,					/**< 4 x 16-bit unsigned RGBA texture. */
	RGBA16_SINT,					/**< 4 x 16-bit signed RGBA texture. */
	RGBA16_FLOAT,					/**< 4 x 16-bit floating point RGBA texture. */
	RGBA8_UNORM,					/**< 4 x 8-bit unsigned normalized RGBA texture. */
	RGBA8_UINT,						/**< 4 x 8-bit unsigned RGBA texture. */
	RGBA8_SINT,						/**< 4 x 8-bit signed RGBA texture. */
	RGB32_UINT,						/**< 3 x 32-bit unsigned RGB texture. */
	RGB32_SINT,						/**< 3 x 32-bit signed RGB texture. */
	RGB32_FLOAT,					/**< 3 x 32-bit floating point RGB texture. */
	R11F_G11F_B10F,					/**< 2 x 11-bit Red/Green, 10-bit Blue texture. */
	RGB10_A2,						/**< 3 x 10-bit unsigned normalized RGB, 2-bit ALPHA texture. */
	R9G9B9E5_SHAREDEXP,				/**< 3 x 9-bit RGB, 5-bit shared exponent RGB texture. */
	RG32_UINT,						/**< 2 x 32-bit unsigned RED-GREEN texture. */
	RG32_SINT,						/**< 2 x 32-bit signed RED-GREEN texture. */
	RG32_FLOAT,						/**< 2 x 32-bit floating point RED-GREEN texture. */
	RG16_UNORM,						/**< 2 x 16-bit unsigned normalized RED-GREEN texture. */
	RG16_UINT,						/**< 2 x 16-bit unsigned RED-GREEN texture. */
	RG16_SINT,						/**< 2 x 16-bit signed RED-GREEN texture. */
	RG16_FLOAT,						/**< 2 x 16-bit floating point RED-GREEN texture. */
	RG8_UNORM,						/**< 2 x 8-bit unsigned normalized RED-GREEN texture. */
	RG8_UINT,						/**< 2 x 8-bit unsigned RED-GREEN texture. */
	RG8_SINT,						/**< 2 x 8-bit signed RED-GREEN texture. */
	R32_UINT,						/**< 32-bit unsigned RED texture. */
	R32_SINT,						/**< 32-bit signed RED texture. */
	R32_FLOAT,						/**< 32-bit floating-point RED texture. */
	R16_UNORM,						/**< 16-bit unsigned normalized RED texture. */
	R16_UINT,						/**< 16-bit unsigned RED texture. */
	R16_SINT,						/**< 16-bit signed RED texture. */
	R16_FLOAT,						/**< 16-bit floating-point RED texture. */
	R8_UNORM,						/**< 8-bit unsigned normalized RED texture. */
	R8_UINT,						/**< 8-bit unsigned RED texture. */
	R8_SINT,						/**< 8-bit signed RED texture. */
	BC1_UNORM,						/**< BC1 (S3TC DXT1) unsigned normalized compressed texture. */
	BC2_UNORM,						/**< BC2 (S3TC DXT3) unsigned normalized compressed texture. */
	BC3_UNORM,						/**< BC3 (S3TC DXT5) unsigned normalized compressed texture. */
	BC4_UNORM,						/**< BC4 (RGTC) unsigned normalized compressed texture. */
	BC4_SNORM,						/**< BC4 (RGTC) signed normalized compressed texture. */
	BC5_UNORM,						/**< BC5 (RGTC) unsigned normalized compressed texture. */
	BC5_SNORM,						/**< BC5 (RGTC) signed normalized compressed texture. */
	DEPTH16_UNORM,					/**< 16-bit unsigned normalized depth texture. */
	DEPTH32_FLOAT,					/**< 32-bit floating point depth texture. */
	DEPTH24_STENCIL8,				/**< 24-bit unsigned normalized depth, 8-bit unsigned stencil texture. */
	DEPTH32F_STENCIL8,				/**< 32-bit floating point depth, 8-bit unsigned stencil texture. */
};

/* Cubemap face index. */
enum CubeMapFace
{
	CubeMapPositiveX = 0,			/**< X+ side of the cube map. */
	CubeMapNegativeX,				/**< X- side of the cube map. */
	CubeMapPositiveY,				/**< Y+ side of the cube map. */
	CubeMapNegativeY,				/**< Y- side of the cube map. */
	CubeMapPositiveZ,				/**< Z+ side of the cube map. */
	CubeMapNegativeZ,				/**< Z- side of the cube map. */
};


/* Classes */

class IImageFormatPlugin;

/** Image file. */
class GCC_PUBLIC ImageFile : public ILoadable
{
public:
	/** Constructor. */
	IMAGEAPI ImageFile();

	/** Destructor. */
	IMAGEAPI virtual ~ImageFile();

	/**
	 * Loads image from the specified file.
	 * @param file Path to the file.
	 */
	IMAGEAPI void loadFromFile(const QString & file);

	/**
	 * Loads image from the specified in-memory stream.
	 * @param stream Data stream.
	 */
	IMAGEAPI void loadFromFile(MemoryFile & stream);

	/**
	 * Saves image to the specified file
	 * @param file Path to the file.
	 */
	IMAGEAPI void saveToFile(const QString & file);

	/** Returns true if image is compressed. */
	IMAGEAPI bool isCompressed() const;

	/** Returns true if image represents a cubemap. */
	inline bool isCubeMap() const
	{
		return m_Faces > 0;
	}

	/** Returns true if image represents a volume. */
	inline bool isVolume() const
	{
		return m_Depth > 0;
	}

	/** Returns width of the image. */
	inline int getWidth() const
	{
		return m_Width;
	}

	/** Returns height of the image. */
	inline int getHeight() const
	{
		return m_Height;
	}

	/** Returns depth of the image (0 for images with no depth). */
	inline int getDepth() const
	{
		return m_Depth;
	}

	/** Returns number of mipmap levels available for the image. */
	inline int getMipLevels() const
	{
		return m_LevelCount;
	}

	/** Return number of cubemap faces available for the image (0 for non-cubemap images). */
	inline int getFaces() const
	{
		return m_Faces;
	}

	/** Returns format of the image data. */
	inline ImageFormat getFormat() const
	{
		return m_Format;
	}

	/**
	 * Returns size in bytes of a row of a specified level of the image.
	 * @param level Index of the level.
	 */
	IMAGEAPI int getImagePitch(int level = 0) const;

	/**
	 * Returns size in bytes of a specified level of the image.
	 * @param level Index of the level.
	 */
	IMAGEAPI int getImageSize(int level = 0) const;

	/**
	 * Returns pointer to the data of the specified level.
	 * @param level Index of the leve.
	 * @param face Index of the cubemap face.
	 */
	IMAGEAPI const void * getLevel(int level, CubeMapFace face = CubeMapPositiveX) const;

	/**
	 * Returns pointer to the data of the specified level.
	 * @param level Index of the leve.
	 * @param face Index of the cubemap face.
	 */
	IMAGEAPI void * getLevel(int level, CubeMapFace face = CubeMapPositiveX);

	/**
	 * Converts a suitable image from a cubemap cross to a cubemap.
	 * @returns true on success, false for unsuitable images.
	 */
//	bool convertCrossToCubemap();

	/**
	 * Registers the specified image format plugin.
	 * @param ptr Pointer to the image format plugin.
	 */
	IMAGEAPI static void registerImageFormatPlugin(IImageFormatPlugin * ptr);

	/**
	 * Unregisters the specified image format plugin.
	 * @param ptr Pointer to the image format plugin.
	 */
	IMAGEAPI static void unregisterImageFormatPlugin(IImageFormatPlugin * ptr);

private:
	int m_Width;									/**< Width of the image in pixels. */
	int m_Height;									/**< Height of the image in pixels. */
	int m_Depth;									/**< Depth of the image in pixels (0 for images that have no depth). */
	int m_LevelCount;								/**< Number of levels in the image. */
	int m_Faces;									/**< Number of cubemap faces (0 for images that are not a cubemap). */
	ImageFormat m_Format;							/**< Format of the image data. */
	int m_ElementSize;								/**< Number of bytes in the image element. */
	std::vector<byte *> m_Data;						/**< Pointers to the levels' data. */
	static std::set<IImageFormatPlugin *> m_Plugins;/**< Set of image format plugins. */
	static tbb::spin_rw_mutex m_PluginsLock;		/**< Lock for access to the list of image format plugins. */
	static bool m_StdPluginsRegistered;				/**< True if standard plugins has been registered. */

	/** Frees memory occupied by the image data. */
	void freeData();

#if 0
        NVSDKENTRY void flipSurface(GLubyte *surf, int width, int height, int depth);


        //
        // Static elements used to dispatch to proper sub-readers
        //
        //////////////////////////////////////////////////////////////
        struct FormatInfo {
            const char* extension;
            bool (*reader)( const char* file, Image& i);
            bool (*writer)( const char* file, Image& i);
        };

        static FormatInfo formatTable[]; 

        NVSDKENTRY static bool readPng( const char *file, Image& i);
        NVSDKENTRY static bool readDDS( const char *file, Image& i);
        NVSDKENTRY static bool readHdr( const char *file, Image& i);

        NVSDKENTRY static bool writePng( const char *file, Image& i);
        //NVSDKENTRY static bool writeDDS( const char *file, Image& i);
        //NVSDKENTRY static bool writeHdr( const char *file, Image& i);

        NVSDKENTRY static void flip_blocks_dxtc1(GLubyte *ptr, unsigned int numBlocks);
        NVSDKENTRY static void flip_blocks_dxtc3(GLubyte *ptr, unsigned int numBlocks);
        NVSDKENTRY static void flip_blocks_dxtc5(GLubyte *ptr, unsigned int numBlocks);
#endif

	Q_DISABLE_COPY(ImageFile)
	friend class ImageFormatReaderHelper;
};

/** Pointer to the image file. */
typedef Pointer<ImageFile> ImageFilePtr;

#endif // __IMAGE__IMAGE_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
