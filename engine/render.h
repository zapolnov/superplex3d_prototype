#ifndef __ENGINE__RENDER_H__
#define __ENGINE__RENDER_H__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>
#include <opengl/context.h>
#include <math/camera.h>
#include <string>


/* Constants */

/** Vertex components. */
enum VertexComponent
{
	VF_TexCoords = 0x01,	/**< Texture coordinates (2 x float). */
	VF_Color = 0x02,		/**< Color (4 x unsigned byte). */
	VF_Normal = 0x04,		/**< Normal vector (3 x float). */
	VF_Position = 0x08,		/**< Vertex coordinates (3 x float). */
};

/** Buffer object access mode. */
enum AccessMode
{
	ReadAccess = 0,			/**< Read-only access. */
	WriteAccess,			/**< Write-only access. */
	ReadWriteAccess,		/**< Read/write access. */
};

/** Buffer object usage mode. */
enum UsageMode
{
	StaticDraw = 0,			/**< Buffer contents are never changed and it is used for drawing. */
	DynamicDraw,			/**< Buffer contents are changed infrequently and it is used for drawing. */
	StreamDraw,				/**< Buffer contents are changed at every usage and it is used for drawing. */
};

/** Heightmap rendering mode. */
enum HeightMapRenderMode
{
	UnifiedRender = 0,		/**< Renders the heightmap with one rendering call. */
	SplitRender,			/**< Renders the each patch seperated. */
	SoftwareMorphRender,	/**< Does vertex morphing between two tesselation stages to reduce pops. */
	HardwareMorphRender		/**< Does vertex morphing using hardware tweeming between two tesselation stages to reduce pops. */
};

/** Maximum number of textures the engine can handle. */
#define MAX_TEXTURES 4


/* Structures */

/** Texture. */
struct Texture : public RefCounted {};

/** Pointer to the texture. */
typedef Pointer<Texture> TexturePtr;

/** Shader. */
struct Shader : public RefCounted {};

/** Pointer to the shader. */
typedef Pointer<Shader> ShaderPtr;

/** Material. */
struct Material : public RefCounted
{
	/** Texture sampler description. */
	struct TextureSampler
	{
		TexturePtr texture;		/**< Texture bound to this unit. */
		GLenum env;				/**< Texture environment mode. */

		/** Constructor. */
		inline TextureSampler()
			: env(GL_MODULATE)
		{
		}
	};

	bool wireframe : 1;						/**< True for wireframe models. */
	bool blending : 1;						/**< True if blending should be enabled. */
	bool cullFace : 1;						/**< True if face culling should be enabled. */
	GLenum blendSrc;						/**< Source factor for blending. */
	GLenum blendDst;						/**< Destination factor for blending. */
	GLenum cullMode;						/**< Face culling mode. */
	TextureSampler textures[MAX_TEXTURES];	/**< Texture samplers. */
	ShaderPtr vertexShader;					/**< Pointer to the vertex shader. */
	ShaderPtr fragmentShader;				/**< Pointer to the fragment shader. */
	Vector4 color;							/**< Constant color. */

protected:
	/** Constructor. */
	inline Material()
		: wireframe(false), blending(false), cullFace(false),
		  blendSrc(GL_ONE), blendDst(GL_ZERO), cullMode(GL_BACK),
		  color(1.0, 1.0, 1.0, 1.0)
	{
	}
};

/** Pointer to the material. */
typedef Pointer<Material> MaterialPtr;

/** Buffer object. */
struct BufferObject : public RefCounted {};

/** Pointer to the buffer object. */
typedef Pointer<BufferObject> BufferObjectPtr;

/** Mesh. */
struct Mesh : public RefCounted {};

/** Pointer to the mesh. */
typedef Pointer<Mesh> MeshPtr;

/** Font. */
struct Font : public RefCounted {};

/** Pointer to the font. */
typedef Pointer<Font> FontPtr;

/** Lightning. */
struct Lightning : public RefCounted {};

/** Pointer to the lightning. */
typedef Pointer<Lightning> LightningPtr;


/* Functions */

EXTERN_C_BEGIN

// r_main.cpp

/** Initializes the renderer. */
void R_Init() throw(std::exception);

/** Shuts down the renderer. */
void R_Shutdown();

/**
 * Sets the screen clearing color.
 * @param r Red component of a color.
 * @param g Green component of a color.
 * @param b Blue component of a color.
 */
ENGINEAPI void R_SetClearColor(float r, float g, float b);

/** Begins rendering a frame. */
void R_BeginFrame();

/** Ends rendering a frame. */
void R_EndFrame();

/** Switches to the 2D rendering mode. */
void R_Set2D();

/**
 * Switches to the 3D rendering mode.
 * @param cam Camera to use.
 */
ENGINEAPI void R_Set3D(const CameraPtr & camera);

/** Swaps the front and back buffers. */
bool R_SwapBuffers();

/**
 * Unprojects the specified 3D point into the 2D space.
 * @param camera Current camera.
 * @param p Point to unproject.
 */
extern "C++" ENGINEAPI Vector3 R_UnprojectPoint(const CameraPtr & camera, const QPoint & p);

// r_texture

/**
 * Loads the specified texture.
 * @param name Name of the texture file.
 */
extern "C++" ENGINEAPI TexturePtr R_LoadTexture(const std::string & name) throw(std::exception);

/**
 * Binds texture to the specified texture unit.
 * @param unit Index of the texture unit.
 * @param texture Texture to bind.
 * @param env Texture environment mode.
 */
ENGINEAPI void R_BindTexture(uint unit, const TexturePtr & texture, GLenum env = GL_MODULATE);

/** Clears the texture cache. */
ENGINEAPI void R_ClearTextureCache();

/**
 * Renders the specified texture at the specified position.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param tex Texture to render.
 */
ENGINEAPI void R_RenderTexture2D(float x, float y, const TexturePtr & tex, GLenum env = GL_MODULATE);

/**
 * Returns size of the texture.
 * @param tex Texture to render.
 */
ENGINEAPI QSize R_GetTextureSize(const TexturePtr & tex);

// r_buffer

/** Creates the dynamic vertex buffer object. */
extern "C++" ENGINEAPI BufferObjectPtr R_CreateVertexBufferObject();

/** Creates the dynamic index buffer object. */
extern "C++" ENGINEAPI BufferObjectPtr R_CreateIndexBufferObject();

/**
 * Locks the specified buffer object.
 * @param obj Pointer to the buffer object.
 * @param access Access mode.
 */
ENGINEAPI void * R_LockBufferObject(const BufferObjectPtr & ptr, AccessMode access);

/**
 * Unlocks the specified buffer object.
 * @param obj Pointer to the buffer object.
 */
ENGINEAPI void R_UnlockBufferObject(const BufferObjectPtr & ptr);

/**
 * Binds the specified buffer object.
 * @param obj Pointer to the buffer object.
 */
ENGINEAPI void * R_BindBufferObject(const BufferObjectPtr & ptr);

/**
 * Unbinds the specified buffer object.
 * @param obj Pointer to the buffer object.
 */
ENGINEAPI void R_UnbindBufferObject(const BufferObjectPtr & ptr);

/**
 * Uploads data to the buffer object.
 * @param obj Pointer to the buffer object.
 * @param data Pointer to the data.
 * @param size Size of the data.
 * @param usage Buffer object usage.
 */
ENGINEAPI void R_SetBufferObjectData(const BufferObjectPtr & ptr, const void * data, size_t size, UsageMode usage);

// r_mesh

/**
 * Returns size of a single vertex for the specified vertex format.
 * @param format Vertex format (@sa VertexComponent).
 */
ENGINEAPI int R_CalcVertexSize(int format);

/**
 * Creates the box mesh.
 * @param material Pointer to the material.
 * @param format Vertex format (@sa VertexComponent).
 */
extern "C++" ENGINEAPI MeshPtr R_CreateBoxMesh(const MaterialPtr & material = MaterialPtr(), int format = VF_Position | VF_Normal);

/**
 * Loads mesh from the specified file.
 * @param name Name of the mesh file.
 */
extern "C++" ENGINEAPI MeshPtr R_LoadMesh(const std::string & name);

/**
 * Renders the specified mesh.
 * @param mesh Pointer to the mesh.
 */
ENGINEAPI void R_RenderMesh(const MeshPtr & mesh);

/**
 * Renders the specified mesh without applying the material.
 * @param mesh Pointer to the mesh.
 */
ENGINEAPI void R_RenderMeshWithoutMaterial(const MeshPtr & mesh);

/** Clears the cache of meshes. */
ENGINEAPI void R_ClearMeshCache();

/**
 * Overrides vertex shader for the mesh.
 * @param mesh Pointer to the mesh.
 * @param shader Vertex shader.
 */
ENGINEAPI void R_SetMeshVertexShader(MeshPtr mesh, const ShaderPtr & shader);

/**
 * Overrides fragment shader for the mesh.
 * @param mesh Pointer to the mesh.
 * @param shader Fragment shader.
 */
ENGINEAPI void R_SetMeshFragmentShader(MeshPtr mesh, const ShaderPtr & shader);

// r_font

/**
 * Loads the specified font file.
 * @param name Name of the font file.
 */
extern "C++" ENGINEAPI FontPtr R_LoadFont(const std::string & name) throw(std::exception);

/**
 * Draws string using the specified font.
 * @param font Pointer to the font to use.
 * @param posX X coordinate.
 * @param posY Y coordinate.
 * @param str String to draw.
 * @param argb Color to use.
 * @returns width and height of the text string.
 */
ENGINEAPI QSize R_DrawString(const FontPtr & font, int posX, int posY, const std::wstring & str, ulong argb = 0xFFFFFFFFUL);

/**
 * Calculates width and height of the specified text string.
 * @param font Pointer to the font to use.
 * @param str String to calculate metrics of.
 */
ENGINEAPI QSize R_CalcStringMetrics(const FontPtr & font, const std::wstring & str);

// r_shader

/**
 * Creates a new vertex shader.
 * @param file Name of the file.
 */
extern "C++" ENGINEAPI ShaderPtr R_CompileVertexShader(const std::string & file);

/**
 * Creates a new fragment shader.
 * @param file Name of the file.
 */
extern "C++" ENGINEAPI ShaderPtr R_CompileFragmentShader(const std::string & file);

/**
 * Binds the specified vertex shader.
 * @param shader Shader to bind.
 */
ENGINEAPI void R_BindVertexShader(const ShaderPtr & shader);

/**
 * Binds the specified fragment shader.
 * @param shader Shader to bind.
 */
ENGINEAPI void R_BindFragmentShader(const ShaderPtr & shader);

/** Unbinds any currently bound vertex shader. */
ENGINEAPI void R_UnbindVertexShader();

/** Unbinds any currently bound fragment shader. */
ENGINEAPI void R_UnbindFragmentShader();

/** Clears the cache of vertex shaders. */
ENGINEAPI void R_ClearVertexShaderCache();

/** Clears the cache of fragment shaders. */
ENGINEAPI void R_ClearFragmentShaderCache();

/**
 * Sets value of the specified uniform variable.
 * @param shader Pointer to the shader.
 * @param idx Index of the uniform.
 * @param value Value to set.
 */
ENGINEAPI void R_SetShaderUniform(ShaderPtr shader, uint idx, float value);

// r_material

/** Creates new material. */
extern "C++" ENGINEAPI MaterialPtr R_CreateMaterial();

/** 
 * Loads material from the specified file.
 * @param file XML file with material definition.
 */
extern "C++" ENGINEAPI MaterialPtr R_LoadMaterial(const std::string & file);

/**
 * Binds the specified material.
 * @param material Pointer to the material.
 */
ENGINEAPI void R_SetMaterial(const MaterialPtr & material);

// r_terrain

/**
 * Loads the specified height map.
 * @param map Path to the height map.
 */
ENGINEAPI void R_LoadHeightMap(const std::string & map);

/** Destroys any currently loaded height map. */
ENGINEAPI void R_FreeHeightMap();

/**
 * Renders the height map.
 * @param camera Position of the camera.
 */
ENGINEAPI void R_RenderHeightMap(const CameraPtr & camera);

/**
 * Sets the rendering mode of the terrain.
 * @param mode Rendering mode.
 */
ENGINEAPI void R_SetHeightMapRenderMode(HeightMapRenderMode mode);

// r_lightning

/**
 * Creates new lightning.
 * @param from Source point of the lightning.
 * @param to Destination point of the lightning.
 * @param fracLevel Fractalisation level.
 * @param fracDec Fractalisation decrease.
 * @param maxFracDepth Maximum depth of fractalisation.
 */
extern "C++" ENGINEAPI LightningPtr R_CreateLightning(const Vector3 & from,
	const Vector3 & to, float fracLevel, float fracDec, uint maxFracDepth);

/**
 * Renders the lightning.
 * @param ptr Pointer to the lightning.
 * @param width Width of the lightning.
 * @param is3d If true, render lightning in three dimensions.
 */
ENGINEAPI void R_RenderLightning(const LightningPtr & ptr, float width, bool is3d = true);

/**
 * Updates the lightning.
 * @param ptr Pointer to the lightning
 */
ENGINEAPI void R_UpdateLightning(LightningPtr ptr);

EXTERN_C_END


/* Classes */

/** Temporary object used to lock the buffer object. */
class BufferLocker
{
public:
	/**
	 * Constructor.
	 * @param buf Pointer to the buffer object to lock.
	 * @param access Access mode.
	 */
	BufferLocker(BufferObjectPtr & buf, AccessMode access)
		: m_Buffer(buf)
	{
		m_Pointer = R_LockBufferObject(m_Buffer, access);
	}

	/** Destructor. */
	~BufferLocker()
	{
		R_UnlockBufferObject(m_Buffer);
	}

	/** Returns pointer to the data. */
	inline void * ptr()
	{
		return m_Pointer;
	}

private:
	BufferObjectPtr & m_Buffer;	/**< Pointer to the buffer object. */
	void * m_Pointer;			/**< Pointer to the data. */
};

#endif // __ENGINE__RENDER_H__
