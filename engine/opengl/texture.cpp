#include "texture.h"

/* Public variables */

GLTextureFormat g_TextureFormatToGL[] =
{
	{ GL_ALPHA8, GL_ALPHA, GL_UNSIGNED_BYTE },
	{ GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT },							// EXT_texture_integer
	{ GL_RGBA32I_EXT, GL_RGBA_INTEGER_EXT, GL_INT },									// EXT_texture_integer
	{ GL_RGBA32F, GL_RGBA, GL_FLOAT },													// ARB_texture_float
	{ GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT },
	{ GL_RGBA16UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_SHORT },						// EXT_texture_integer
	{ GL_RGBA16I_EXT, GL_RGBA_INTEGER_EXT, GL_SHORT },									// EXT_texture_integer
	{ GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT_ARB },											// ARB_texture_float, ARB_half_float_pixel
	{ GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE },
	{ GL_RGBA8UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_BYTE },							// EXT_texture_integer
	{ GL_RGBA8I_EXT, GL_RGBA_INTEGER_EXT, GL_BYTE },									// EXT_texture_integer
	{ GL_RGB32UI_EXT, GL_RGB_INTEGER_EXT, GL_UNSIGNED_INT },							// EXT_texture_integer
	{ GL_RGB32I_EXT, GL_RGB_INTEGER_EXT, GL_INT },										// EXT_texture_integer
	{ GL_RGB32F_ARB, GL_RGB, GL_FLOAT },												// ARB_texture_float
	{ GL_R11F_G11F_B10F_EXT, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV_EXT },				// EXT_packed_float
	{ GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV },
	{ GL_RGB9_E5_EXT, GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV_EXT },						// EXT_texture_shared_exponent
	{ GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT },										// ARB_texture_rg
	{ GL_RG32I, GL_RG_INTEGER, GL_INT },												// ARB_texture_rg
	{ GL_RG32F, GL_RG, GL_FLOAT },														// ARB_texture_rg
	{ GL_RG16, GL_RG, GL_UNSIGNED_SHORT },												// ARB_texture_rg
	{ GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_SHORT },									// ARB_texture_rg
	{ GL_RG16I, GL_RG_INTEGER, GL_SHORT },												// ARB_texture_rg
	{ GL_RG16F, GL_RG, GL_HALF_FLOAT_ARB },												// ARB_texture_rg
	{ GL_RG8, GL_RG, GL_UNSIGNED_BYTE },												// ARB_texture_rg
	{ GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE },										// ARB_texture_rg
	{ GL_RG8I, GL_RG_INTEGER, GL_BYTE },												// ARB_texture_rg
	{ GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT },										// ARB_texture_rg
	{ GL_R32I, GL_RED_INTEGER, GL_INT },												// ARB_texture_rg
	{ GL_R32F, GL_RED, GL_FLOAT },														// ARB_texture_rg
	{ GL_R16, GL_RED, GL_UNSIGNED_SHORT },												// ARB_texture_rg
	{ GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT },									// ARB_texture_rg
	{ GL_R16I, GL_RED_INTEGER, GL_SHORT },												// ARB_texture_rg
	{ GL_R16F, GL_RED, GL_HALF_FLOAT_ARB },												// ARB_texture_rg
	{ GL_R8, GL_RED, GL_UNSIGNED_BYTE },												// ARB_texture_rg
	{ GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE },										// ARB_texture_rg
	{ GL_R8I, GL_RED_INTEGER, GL_BYTE },												// ARB_texture_rg
	{ GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 0, 0 },											// EXT_texture_compression_s3tc
	{ GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 0, 0 },											// EXT_texture_compression_s3tc
	{ GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0, 0 },											// EXT_texture_compression_s3tc
	{ GL_COMPRESSED_RED_RGTC1_EXT, 0, 0 },												// EXT_texture_compression_rgtc
	{ GL_COMPRESSED_SIGNED_RED_RGTC1_EXT, 0, 0 },										// EXT_texture_compression_rgtc
	{ GL_COMPRESSED_RED_GREEN_RGTC2_EXT, 0, 0 },										// EXT_texture_compression_rgtc
	{ GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT, 0, 0 },									// EXT_texture_compression_rgtc
	{ GL_DEPTH_COMPONENT16_ARB,	GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT },				// ARB_depth_texture
	{ GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT }, 							// ARB_depth_texture
	{ GL_DEPTH24_STENCIL8_EXT, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT },		// EXT_packed_depth_stencil
	{ GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL_EXT, GL_FLOAT_32_UNSIGNED_INT_24_8_REV },	// ARB_depth_buffer_float
};
