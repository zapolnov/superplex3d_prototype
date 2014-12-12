#ifndef __OPENGL__TEXTURE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __OPENGL__TEXTURE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <image/image_file.h>
#include <opengl/context.h>


/* Structures */

/** OpenGL texture format information. */
struct GLTextureFormat
{
	GLenum internalFormat;		/**< Internal format of the texture. */
	GLenum format;				/**< Format of the input data. */
	GLenum type;				/**< Type of the components of the input data. */
};


/* Variables */

/** Texture format conversion array. */
extern GLTextureFormat g_TextureFormatToGL[];


/* Functions */

/**
 * Converts texture format from engine representation to OpenGL internal format representation.
 * @param format Texture format.
 */
static inline
GLenum textureFormatToInternalFormatGL(ImageFormat format)
{
	Q_ASSERT(g_TextureFormatToGL[A8_UNORM].internalFormat == GL_ALPHA8);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_UINT].internalFormat == GL_RGBA32UI_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_SINT].internalFormat == GL_RGBA32I_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_FLOAT].internalFormat == GL_RGBA32F);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_UNORM].internalFormat == GL_RGBA16);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_UINT].internalFormat == GL_RGBA16UI_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_SINT].internalFormat == GL_RGBA16I_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_FLOAT].internalFormat == GL_RGBA16F);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_UNORM].internalFormat == GL_RGBA8);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_UINT].internalFormat == GL_RGBA8UI_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_SINT].internalFormat == GL_RGBA8I_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_UINT].internalFormat == GL_RGB32UI_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_SINT].internalFormat == GL_RGB32I_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_FLOAT].internalFormat == GL_RGB32F_ARB);
	Q_ASSERT(g_TextureFormatToGL[R11F_G11F_B10F].internalFormat == GL_R11F_G11F_B10F_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB10_A2].internalFormat == GL_RGB10_A2);
	Q_ASSERT(g_TextureFormatToGL[R9G9B9E5_SHAREDEXP].internalFormat == GL_RGB9_E5_EXT);
	Q_ASSERT(g_TextureFormatToGL[RG32_UINT].internalFormat == GL_RG32UI);
	Q_ASSERT(g_TextureFormatToGL[RG32_SINT].internalFormat == GL_RG32I);
	Q_ASSERT(g_TextureFormatToGL[RG32_FLOAT].internalFormat == GL_RG32F);
	Q_ASSERT(g_TextureFormatToGL[RG16_UNORM].internalFormat == GL_RG16);
	Q_ASSERT(g_TextureFormatToGL[RG16_UINT].internalFormat == GL_RG16UI);
	Q_ASSERT(g_TextureFormatToGL[RG16_SINT].internalFormat == GL_RG16I);
	Q_ASSERT(g_TextureFormatToGL[RG16_FLOAT].internalFormat == GL_RG16F);
	Q_ASSERT(g_TextureFormatToGL[RG8_UNORM].internalFormat == GL_RG8);
	Q_ASSERT(g_TextureFormatToGL[RG8_UINT].internalFormat == GL_RG8UI);
	Q_ASSERT(g_TextureFormatToGL[RG8_SINT].internalFormat == GL_RG8I);
	Q_ASSERT(g_TextureFormatToGL[R32_UINT].internalFormat == GL_R32UI);
	Q_ASSERT(g_TextureFormatToGL[R32_SINT].internalFormat == GL_R32I);
	Q_ASSERT(g_TextureFormatToGL[R32_FLOAT].internalFormat == GL_R32F);
	Q_ASSERT(g_TextureFormatToGL[R16_UNORM].internalFormat == GL_R16);
	Q_ASSERT(g_TextureFormatToGL[R16_UINT].internalFormat == GL_R16UI);
	Q_ASSERT(g_TextureFormatToGL[R16_SINT].internalFormat == GL_R16I);
	Q_ASSERT(g_TextureFormatToGL[R16_FLOAT].internalFormat == GL_R16F);
	Q_ASSERT(g_TextureFormatToGL[R8_UNORM].internalFormat == GL_R8);
	Q_ASSERT(g_TextureFormatToGL[R8_UINT].internalFormat == GL_R8UI);
	Q_ASSERT(g_TextureFormatToGL[R8_SINT].internalFormat == GL_R8I);
	Q_ASSERT(g_TextureFormatToGL[BC1_UNORM].internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT);
	Q_ASSERT(g_TextureFormatToGL[BC2_UNORM].internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
	Q_ASSERT(g_TextureFormatToGL[BC3_UNORM].internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
	Q_ASSERT(g_TextureFormatToGL[BC4_UNORM].internalFormat == GL_COMPRESSED_RED_RGTC1_EXT);
	Q_ASSERT(g_TextureFormatToGL[BC4_SNORM].internalFormat == GL_COMPRESSED_SIGNED_RED_RGTC1_EXT);
	Q_ASSERT(g_TextureFormatToGL[BC5_UNORM].internalFormat == GL_COMPRESSED_RED_GREEN_RGTC2_EXT);
	Q_ASSERT(g_TextureFormatToGL[BC5_SNORM].internalFormat == GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH16_UNORM].internalFormat == GL_DEPTH_COMPONENT16_ARB);
	Q_ASSERT(g_TextureFormatToGL[DEPTH32_FLOAT].internalFormat == GL_DEPTH_COMPONENT32F);
	Q_ASSERT(g_TextureFormatToGL[DEPTH24_STENCIL8].internalFormat == GL_DEPTH24_STENCIL8_EXT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH32F_STENCIL8].internalFormat == GL_DEPTH32F_STENCIL8);

	return g_TextureFormatToGL[format].internalFormat;
}

/**
 * Converts texture format from engine representation to OpenGL data format representation.
 * @param format Texture format.
 */
static inline
GLenum textureFormatToDataFormatGL(ImageFormat format)
{
	Q_ASSERT(g_TextureFormatToGL[A8_UNORM].format == GL_ALPHA);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_UINT].format == GL_RGBA_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_SINT].format == GL_RGBA_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_FLOAT].format == GL_RGBA);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_UNORM].format == GL_RGBA);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_UINT].format == GL_RGBA_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_SINT].format == GL_RGBA_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_FLOAT].format == GL_RGBA);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_UNORM].format == GL_RGBA);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_UINT].format == GL_RGBA_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_SINT].format == GL_RGBA_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_UINT].format == GL_RGB_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_SINT].format == GL_RGB_INTEGER_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_FLOAT].format == GL_RGB);
	Q_ASSERT(g_TextureFormatToGL[R11F_G11F_B10F].format == GL_RGB);
	Q_ASSERT(g_TextureFormatToGL[RGB10_A2].format == GL_RGBA);
	Q_ASSERT(g_TextureFormatToGL[R9G9B9E5_SHAREDEXP].format == GL_RGB);
	Q_ASSERT(g_TextureFormatToGL[RG32_UINT].format == GL_RG_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[RG32_SINT].format == GL_RG_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[RG32_FLOAT].format == GL_RG);
	Q_ASSERT(g_TextureFormatToGL[RG16_UNORM].format == GL_RG);
	Q_ASSERT(g_TextureFormatToGL[RG16_UINT].format == GL_RG_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[RG16_SINT].format == GL_RG_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[RG16_FLOAT].format == GL_RG);
	Q_ASSERT(g_TextureFormatToGL[RG8_UNORM].format == GL_RG);
	Q_ASSERT(g_TextureFormatToGL[RG8_UINT].format == GL_RG_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[RG8_SINT].format == GL_RG_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[R32_UINT].format == GL_RED_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[R32_SINT].format == GL_RED_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[R32_FLOAT].format == GL_RED);
	Q_ASSERT(g_TextureFormatToGL[R16_UNORM].format == GL_RED);
	Q_ASSERT(g_TextureFormatToGL[R16_UINT].format == GL_RED_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[R16_SINT].format == GL_RED_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[R16_FLOAT].format == GL_RED);
	Q_ASSERT(g_TextureFormatToGL[R8_UNORM].format == GL_RED);
	Q_ASSERT(g_TextureFormatToGL[R8_UINT].format == GL_RED_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[R8_SINT].format == GL_RED_INTEGER);
	Q_ASSERT(g_TextureFormatToGL[BC1_UNORM].format == 0);
	Q_ASSERT(g_TextureFormatToGL[BC2_UNORM].format == 0);
	Q_ASSERT(g_TextureFormatToGL[BC3_UNORM].format == 0);
	Q_ASSERT(g_TextureFormatToGL[BC4_UNORM].format == 0);
	Q_ASSERT(g_TextureFormatToGL[BC4_SNORM].format == 0);
	Q_ASSERT(g_TextureFormatToGL[BC5_UNORM].format == 0);
	Q_ASSERT(g_TextureFormatToGL[BC5_SNORM].format == 0);
	Q_ASSERT(g_TextureFormatToGL[DEPTH16_UNORM].format == GL_DEPTH_COMPONENT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH32_FLOAT].format == GL_DEPTH_COMPONENT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH24_STENCIL8].format == GL_DEPTH_STENCIL_EXT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH32F_STENCIL8].format == GL_DEPTH_STENCIL_EXT);

	return g_TextureFormatToGL[format].format;
}

/**
 * Converts texture format from engine representation to OpenGL pixel format representation.
 * @param format Texture format.
 */
static inline
GLenum textureFormatToPixelFormatGL(ImageFormat format)
{
	Q_ASSERT(g_TextureFormatToGL[A8_UNORM].type == GL_UNSIGNED_BYTE);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_UINT].type == GL_UNSIGNED_INT);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_SINT].type == GL_INT);
	Q_ASSERT(g_TextureFormatToGL[RGBA32_FLOAT].type == GL_FLOAT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_UNORM].type == GL_UNSIGNED_SHORT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_UINT].type == GL_UNSIGNED_SHORT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_SINT].type == GL_SHORT);
	Q_ASSERT(g_TextureFormatToGL[RGBA16_FLOAT].type == GL_HALF_FLOAT_ARB);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_UNORM].type == GL_UNSIGNED_BYTE);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_UINT].type == GL_UNSIGNED_BYTE);
	Q_ASSERT(g_TextureFormatToGL[RGBA8_SINT].type == GL_BYTE);
	Q_ASSERT(g_TextureFormatToGL[RGB32_UINT].type == GL_UNSIGNED_INT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_SINT].type == GL_INT);
	Q_ASSERT(g_TextureFormatToGL[RGB32_FLOAT].type == GL_FLOAT);
	Q_ASSERT(g_TextureFormatToGL[R11F_G11F_B10F].type == GL_UNSIGNED_INT_10F_11F_11F_REV_EXT);
	Q_ASSERT(g_TextureFormatToGL[RGB10_A2].type == GL_UNSIGNED_INT_2_10_10_10_REV);
	Q_ASSERT(g_TextureFormatToGL[R9G9B9E5_SHAREDEXP].type == GL_UNSIGNED_INT_5_9_9_9_REV_EXT);
	Q_ASSERT(g_TextureFormatToGL[RG32_UINT].type == GL_UNSIGNED_INT);
	Q_ASSERT(g_TextureFormatToGL[RG32_SINT].type == GL_INT);
	Q_ASSERT(g_TextureFormatToGL[RG32_FLOAT].type == GL_FLOAT);
	Q_ASSERT(g_TextureFormatToGL[RG16_UNORM].type == GL_UNSIGNED_SHORT);
	Q_ASSERT(g_TextureFormatToGL[RG16_UINT].type == GL_UNSIGNED_SHORT);
	Q_ASSERT(g_TextureFormatToGL[RG16_SINT].type == GL_SHORT);
	Q_ASSERT(g_TextureFormatToGL[RG16_FLOAT].type == GL_HALF_FLOAT_ARB);
	Q_ASSERT(g_TextureFormatToGL[RG8_UNORM].type == GL_UNSIGNED_BYTE);
	Q_ASSERT(g_TextureFormatToGL[RG8_UINT].type == GL_UNSIGNED_BYTE);
	Q_ASSERT(g_TextureFormatToGL[RG8_SINT].type == GL_BYTE);
	Q_ASSERT(g_TextureFormatToGL[R32_UINT].type == GL_UNSIGNED_INT);
	Q_ASSERT(g_TextureFormatToGL[R32_SINT].type == GL_INT);
	Q_ASSERT(g_TextureFormatToGL[R32_FLOAT].type == GL_FLOAT);
	Q_ASSERT(g_TextureFormatToGL[R16_UNORM].type == GL_UNSIGNED_SHORT);
	Q_ASSERT(g_TextureFormatToGL[R16_UINT].type == GL_UNSIGNED_SHORT);
	Q_ASSERT(g_TextureFormatToGL[R16_SINT].type == GL_SHORT);
	Q_ASSERT(g_TextureFormatToGL[R16_FLOAT].type == GL_HALF_FLOAT_ARB);
	Q_ASSERT(g_TextureFormatToGL[R8_UNORM].type == GL_UNSIGNED_BYTE);
	Q_ASSERT(g_TextureFormatToGL[R8_UINT].type == GL_UNSIGNED_BYTE);
	Q_ASSERT(g_TextureFormatToGL[R8_SINT].type == GL_BYTE);
	Q_ASSERT(g_TextureFormatToGL[BC1_UNORM].type == 0);
	Q_ASSERT(g_TextureFormatToGL[BC2_UNORM].type == 0);
	Q_ASSERT(g_TextureFormatToGL[BC3_UNORM].type == 0);
	Q_ASSERT(g_TextureFormatToGL[BC4_UNORM].type == 0);
	Q_ASSERT(g_TextureFormatToGL[BC4_SNORM].type == 0);
	Q_ASSERT(g_TextureFormatToGL[BC5_UNORM].type == 0);
	Q_ASSERT(g_TextureFormatToGL[BC5_SNORM].type == 0);
	Q_ASSERT(g_TextureFormatToGL[DEPTH16_UNORM].type == GL_UNSIGNED_SHORT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH32_FLOAT].type == GL_FLOAT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH24_STENCIL8].type == GL_UNSIGNED_INT_24_8_EXT);
	Q_ASSERT(g_TextureFormatToGL[DEPTH32F_STENCIL8].type == GL_FLOAT_32_UNSIGNED_INT_24_8_REV);

	return g_TextureFormatToGL[format].type;
}

#endif // __OPENGL__TEXTURE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
