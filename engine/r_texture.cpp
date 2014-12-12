#include <core/exception.h>
#include <engine/render.h>
#include <engine/filesys.h>
#include <engine/r_opengl.h>
#include <image/image_file.h>
#include <opengl/texture.h>
#include <map>
#include <GLFW/glfw3.h>
#include <opengl/glext.h>

/* Structures */

/** Texture. */
struct TextureImpl : public Texture
{
	GLuint textureID;		/**< OpenGL ID of the texture. */
	GLenum target;			/**< Texture binding target. */
	GLenum targetGet;		/**< Constant for glGetIntegerv to get currently bound texture. */
	int width;				/**< Width of the texture. */
	int height;				/**< Height of the texture. */

	/** Constructor. */
	inline TextureImpl()
	{
		glGenTextures(1, &textureID);
	}

	/** Destructor. */
	inline ~TextureImpl()
	{
		glDeleteTextures(1, &textureID);
	}

	/**
	 * Binds the texture.
	 * @param unit Texture unit to bind texture to.
	 */
	inline void bind(uint unit = 0)
	{
		if (unit >= (uint)GL_MaxTextureUnits)
			return;

		if (ARB_multitexture)
			pglActiveTexture(GL_TEXTURE0_ARB + unit);
		glBindTexture(target, textureID);
	}

	/**
	 * Unbinds the texture.
	 * @param unit Texture unit to unbind texture from.
	 */
	inline void unbind(uint unit = 0)
	{
		if (unit >= (uint)GL_MaxTextureUnits)
			return;

		Q_ASSERT(isBound(unit));

		if (ARB_multitexture)
			pglActiveTexture(GL_TEXTURE0_ARB + unit);
		glBindTexture(target, 0);
	}

	/**
	 * Returns true if this texture is currently bound to the specified unit.
	 * @param unit Texture unit.
	 */
	inline bool isBound(uint unit = 0) const
	{
		if (unit >= (uint)GL_MaxTextureUnits)
			return false;

		GLint texID;
		if (ARB_multitexture)
			pglActiveTexture(GL_TEXTURE0_ARB + unit);
		glGetIntegerv(targetGet, &texID);

		return textureID == (GLuint)texID;
	}
};


/* Private variables */

static std::map<std::string, TexturePtr> g_Textures;		/**< Map of textures. */


/* Public functions */

//
// Loads the specified texture
//
TexturePtr R_LoadTexture(const std::string & name) throw(std::exception)
{
	// Check whether we have already loaded the texture
	std::map<std::string, TexturePtr>::iterator it = g_Textures.find(name);
	if (it != g_Textures.end())
		return it->second;

	// Load texture file
	ImageFile image;
	image.loadFromFile(name);

	// Get image info
	int width = image.getWidth();
	int height = image.getHeight();
	int depth = image.getDepth();
	int numLevels = image.getMipLevels();
	ImageFormat format = image.getFormat();
	Q_ASSERT(width > 0 && height > 0);
	Q_ASSERT(numLevels > 0);

	// Create the texture object
	TexturePtr texture = new TextureImpl;
	TextureImpl * tex = texture.as<TextureImpl>();
	tex->width = width;
	tex->height = height;

	// Choose texture type
	Q_ASSERT(!image.isCubeMap());
	if (image.isVolume())
	{
		if (!EXT_texture3D)
			throw Exception(QObject::tr("3D textures are not supported by the hardware."));
		tex->target = GL_TEXTURE_3D;
		tex->targetGet = GL_TEXTURE_BINDING_3D;
	}
	else
	{
		Q_ASSERT(depth == 0);
		if (height == 1)
		{
			tex->target = GL_TEXTURE_1D;
			tex->targetGet = GL_TEXTURE_BINDING_1D;
		}
		else
		{
			tex->target = GL_TEXTURE_2D;
			tex->targetGet = GL_TEXTURE_BINDING_2D;
		}
	}

	// Convert texture format to OpenGL
	GLenum internalFormat = textureFormatToInternalFormatGL(format);
	GLenum sourceFormat = textureFormatToDataFormatGL(format);
	GLenum type = textureFormatToPixelFormatGL(format);

	// Upload texture data
	tex->bind();
	try
	{
		for (int i = 0; i < numLevels; i++)
		{
			const void * data = image.getLevel(i);

			switch (tex->target)
			{
			case GL_TEXTURE_1D:
				glTexImage1D(tex->target, i, internalFormat, width, 0, sourceFormat, type, data);
				break;

			case GL_TEXTURE_2D:
				glTexImage2D(tex->target, i, internalFormat, width, height, 0, sourceFormat, type, data);
				break;

			case GL_TEXTURE_3D:
				pglTexImage3D(tex->target, i, internalFormat, width, height, depth, 0, sourceFormat, type, data);
				break;

			default:
				Q_ASSERT(false);
			}

			width = (width > 1 ? width >> 1 : width);
			height = (height > 1 ? height >> 1 : height);
			depth = (depth > 1 ? depth >> 1 : depth);
		}

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, numLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (EXT_texture3D)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}
	catch (...)
	{
		tex->unbind();
		throw;
	}

	tex->unbind();

	// Store texture pointer
	g_Textures.insert(std::pair<std::string, TexturePtr>(name, texture));

	return texture;
}

//
// Binds texture to the specified texture unit
//
void R_BindTexture(uint unit, const TexturePtr & texture, GLenum env)
{
	if (unit >= (uint)GL_MaxTextureUnits)
		return;

	const TextureImpl * tex = texture.as<TextureImpl>();

	if (ARB_multitexture)
		pglActiveTexture(GL_TEXTURE0_ARB + unit);

	if (!texture)
	{
		if (EXT_texture3D)
			glDisable(GL_TEXTURE_3D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_1D);
		return;
	}

	// Enable or disable 3D texture
	if (EXT_texture3D)
	{
		if (tex->target == GL_TEXTURE_3D)
			glEnable(GL_TEXTURE_3D);
		else
			glDisable(GL_TEXTURE_3D);
	}

	// Enable or disable 2D texture
	if (tex->target == GL_TEXTURE_2D)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	// Enable or disable 1D texture
	if (tex->target == GL_TEXTURE_1D)
		glEnable(GL_TEXTURE_1D);
	else
		glDisable(GL_TEXTURE_1D);

	// Bind the texture	
	glBindTexture(tex->target, tex->textureID);

	// Set texture environment mode
	Q_ASSERT(env == GL_MODULATE || env == GL_REPLACE || env == GL_DECAL || env == GL_BLEND);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, env);
}

//
// Clears the texture cache
//
void R_ClearTextureCache()
{
	g_Textures.clear();
}

//
// Renders the specified texture at the specified position
//
void R_RenderTexture2D(float x, float y, const TexturePtr & tex, GLenum env)
{
	const TextureImpl * t = tex.as<TextureImpl>();

	if (ARB_multitexture)
		pglActiveTexture(GL_TEXTURE0_ARB);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t->textureID);

	// Set texture environment mode
	Q_ASSERT(env == GL_MODULATE || env == GL_REPLACE || env == GL_DECAL || env == GL_BLEND);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, env);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(x + (float)t->width - 1, y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(x + (float)t->width - 1, y + (float)t->height - 1);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + (float)t->height - 1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

//
// Returns size of the texture
//
QSize R_GetTextureSize(const TexturePtr & tex)
{
	const TextureImpl * t = tex.as<TextureImpl>();
	return QSize(t->width, t->height);
}
