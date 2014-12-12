#include <core/logger.h>
#include <engine/r_opengl.h>
#include <opengl/glext.h>
#include <engine/system.h>

/* Public variables. */

//GLint GL_MajorVer;
//GLint GL_MinorVer;
//GLint GL_Revision;

GLint GL_MaxClientAttribStackDepth;
GLint GL_MaxAttribStackDepth;
GLint GL_MaxClipPlanes;
GLint GL_MaxElementIndices;
GLint GL_MaxElementVertices;
GLint GL_MaxLights = 0;
GLint GL_MaxModelViewStackDepth;
GLint GL_MaxProjectionStackDepth;
GLint GL_MaxTextureSize = 1;
GLint GL_MaxElementsVertices = 0;
GLint GL_MaxElementsIndices = 0;

bool SGIS_generate_mipmap = false;

bool EXT_draw_range_elements = false;
PFNGLDRAWRANGEELEMENTSEXTPROC pglDrawRangeElements = NULL;

bool EXT_packed_depth_stencil = false;

bool EXT_packed_float = false;

bool EXT_texture_integer = false;

bool EXT_texture_shared_exponent = false;

bool EXT_texture_edge_clamp = false;

bool EXT_texture_filter_anisotropic = false;
GLint GL_MaxTextureAnisotropy = 1;

bool EXT_texture_compression_s3tc = false;

bool EXT_texture_compression_rgtc = false;

bool ARB_texture_cube_map = false;
GLint GL_MaxCubeMapSize = 1;

bool ARB_texture_rg = false;

bool ARB_texture_float = false;

bool ARB_depth_texture = false;

bool ARB_depth_buffer_float = false;

bool ARB_half_float_pixel = false;

bool ARB_multitexture = false;
GLint GL_MaxTextureUnits = 1;
PFNGLCLIENTACTIVETEXTUREARBPROC pglClientActiveTexture = NULL;
PFNGLACTIVETEXTUREARBPROC pglActiveTexture = NULL;
PFNGLMULTITEXCOORD1SARBPROC pglMultiTexCoord1s = NULL;
PFNGLMULTITEXCOORD1IARBPROC pglMultiTexCoord1i = NULL;
PFNGLMULTITEXCOORD1FARBPROC pglMultiTexCoord1f = NULL;
PFNGLMULTITEXCOORD1DARBPROC pglMultiTexCoord1d = NULL;
PFNGLMULTITEXCOORD2SARBPROC pglMultiTexCoord2s = NULL;
PFNGLMULTITEXCOORD2IARBPROC pglMultiTexCoord2i = NULL;
PFNGLMULTITEXCOORD2FARBPROC pglMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2DARBPROC pglMultiTexCoord2d = NULL;
PFNGLMULTITEXCOORD3SARBPROC pglMultiTexCoord3s = NULL;
PFNGLMULTITEXCOORD3IARBPROC pglMultiTexCoord3i = NULL;
PFNGLMULTITEXCOORD3FARBPROC pglMultiTexCoord3f = NULL;
PFNGLMULTITEXCOORD3DARBPROC pglMultiTexCoord3d = NULL;
PFNGLMULTITEXCOORD4SARBPROC pglMultiTexCoord4s = NULL;
PFNGLMULTITEXCOORD4IARBPROC pglMultiTexCoord4i = NULL;
PFNGLMULTITEXCOORD4FARBPROC pglMultiTexCoord4f = NULL;
PFNGLMULTITEXCOORD4DARBPROC pglMultiTexCoord4d = NULL;
PFNGLMULTITEXCOORD1SVARBPROC pglMultiTexCoord1sv = NULL;
PFNGLMULTITEXCOORD1IVARBPROC pglMultiTexCoord1iv = NULL;
PFNGLMULTITEXCOORD1FVARBPROC pglMultiTexCoord1fv = NULL;
PFNGLMULTITEXCOORD1DVARBPROC pglMultiTexCoord1dv = NULL;
PFNGLMULTITEXCOORD2SVARBPROC pglMultiTexCoord2sv = NULL;
PFNGLMULTITEXCOORD2IVARBPROC pglMultiTexCoord2iv = NULL;
PFNGLMULTITEXCOORD2FVARBPROC pglMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2DVARBPROC pglMultiTexCoord2dv = NULL;
PFNGLMULTITEXCOORD3SVARBPROC pglMultiTexCoord3sv = NULL;
PFNGLMULTITEXCOORD3IVARBPROC pglMultiTexCoord3iv = NULL;
PFNGLMULTITEXCOORD3FVARBPROC pglMultiTexCoord3fv = NULL;
PFNGLMULTITEXCOORD3DVARBPROC pglMultiTexCoord3dv = NULL;
PFNGLMULTITEXCOORD4SVARBPROC pglMultiTexCoord4sv = NULL;
PFNGLMULTITEXCOORD4IVARBPROC pglMultiTexCoord4iv = NULL;
PFNGLMULTITEXCOORD4FVARBPROC pglMultiTexCoord4fv = NULL;
PFNGLMULTITEXCOORD4DVARBPROC pglMultiTexCoord4dv = NULL;

bool EXT_texture3D = false;
GLint GL_MaxTextureSize3D = 1;
PFNGLTEXIMAGE3DEXTPROC pglTexImage3D = NULL;

bool ARB_vertex_buffer_object = false;
PFNGLBINDBUFFERARBPROC pglBindBuffer = NULL;
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffers = NULL;
PFNGLGENBUFFERSARBPROC pglGenBuffers = NULL;
PFNGLISBUFFERARBPROC pglIsBuffer = NULL;
PFNGLBUFFERDATAARBPROC pglBufferData = NULL;
PFNGLBUFFERSUBDATAARBPROC pglBufferSubData = NULL;
PFNGLGETBUFFERSUBDATAARBPROC pglGetBufferSubData = NULL;
PFNGLMAPBUFFERARBPROC pglMapBuffer = NULL;
PFNGLUNMAPBUFFERARBPROC pglUnmapBuffer = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVARBPROC pglGetBufferPointerv = NULL;

bool EXT_compiled_vertex_array = false;
PFNGLLOCKARRAYSEXTPROC pglLockArrays = NULL;
PFNGLUNLOCKARRAYSEXTPROC pglUnlockArrays = NULL;

bool ARB_vertex_program = false;
bool ARB_fragment_program = false;
GLint GL_MaxVertexTemporaries = 0;
GLint GL_MaxFragmentTemporaries = 0;
PFNGLVERTEXATTRIB1SARBPROC pglVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1FARBPROC pglVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1DARBPROC pglVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB2SARBPROC pglVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2FARBPROC pglVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2DARBPROC pglVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB3SARBPROC pglVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3FARBPROC pglVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3DARBPROC pglVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB4SARBPROC pglVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4FARBPROC pglVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4DARBPROC pglVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4NUBARBPROC pglVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB1SVARBPROC pglVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB1FVARBPROC pglVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1DVARBPROC pglVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB2SVARBPROC pglVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB2FVARBPROC pglVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2DVARBPROC pglVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB3SVARBPROC pglVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB3FVARBPROC pglVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3DVARBPROC pglVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB4BVARBPROC pglVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4SVARBPROC pglVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4IVARBPROC pglVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4UBVARBPROC pglVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4USVARBPROC pglVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIB4UIVARBPROC pglVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4FVARBPROC pglVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4DVARBPROC pglVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4NBVARBPROC pglVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NSVARBPROC pglVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NIVARBPROC pglVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NUBVARBPROC pglVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUSVARBPROC pglVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4NUIVARBPROC pglVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC pglVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC pglEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC pglDisableVertexAttribArray = NULL;
PFNGLPROGRAMSTRINGARBPROC pglProgramString = NULL;
PFNGLBINDPROGRAMARBPROC pglBindProgram = NULL;
PFNGLDELETEPROGRAMSARBPROC pglDeletePrograms = NULL;
PFNGLGENPROGRAMSARBPROC pglGenPrograms = NULL;
PFNGLPROGRAMENVPARAMETER4DARBPROC pglProgramEnvParameter4d = NULL;
PFNGLPROGRAMENVPARAMETER4DVARBPROC pglProgramEnvParameter4dv = NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC pglProgramEnvParameter4f = NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC pglProgramEnvParameter4fv = NULL;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC pglProgramLocalParameter4d = NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC pglProgramLocalParameter4dv = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC pglProgramLocalParameter4f = NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC pglProgramLocalParameter4fv = NULL;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC pglGetProgramEnvParameterdv = NULL;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC pglGetProgramEnvParameterfv = NULL;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC pglGetProgramLocalParameterdv = NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC pglGetProgramLocalParameterfv = NULL;
PFNGLGETPROGRAMIVARBPROC pglGetProgramiv = NULL;
PFNGLGETPROGRAMSTRINGARBPROC pglGetProgramString = NULL;
PFNGLGETVERTEXATTRIBDVARBPROC pglGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFVARBPROC pglGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIVARBPROC pglGetVertexAttribiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC pglGetVertexAttribPointerv = NULL;
PFNGLISPROGRAMARBPROC pglIsProgram = NULL;


/* Private functions */

//
// Notifies user that specified extension does not have any of entrypoints
//
static
void badEntryPoints(const char * extension)
{
	Sys_ErrorMessage(
		(QObject::tr("OpenGL advertises %1, but required entry points does not exist.")
		.arg(extension)).toLocal8Bit()
	);
}


/* Public functions */

//
// Collects global OpenGL info
//
void GL_GetInfo()
{
	logger << LOG_INFO << "\nOpenGL info\n-----------";

	//glfwGetGLVersion(&GL_MajorVer, &GL_MinorVer, &GL_Revision);

	logger << LOG_INFO << "Vendor: %s" << (const char *)glGetString(GL_VENDOR);
	logger << LOG_INFO << "Renderer: %s" << (const char *)glGetString(GL_RENDERER);
//	logger << LOG_INFO << "Version: %s (parsed as %d.%d.%d)" <<
//		(const char *)glGetString(GL_VERSION) << GL_MajorVer << GL_MinorVer << GL_Revision;
	logger << LOG_INFO << "Version: %s" << (const char *)glGetString(GL_VERSION);
}

//
// Initializes OpenGL extensions
//
void GL_InitExtensions()
{
	logger << LOG_INFO << "\nOpenGL extensions\n-----------------";

	// Check for automatic mipmap generation support
	SGIS_generate_mipmap = glfwExtensionSupported("GL_SGIS_generate_mipmap") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (SGIS_generate_mipmap ? '+' : ' ') << "GL_SGIS_generate_mipmap";

	// Check for CVAR
	EXT_compiled_vertex_array = glfwExtensionSupported("GL_EXT_compiled_vertex_array") ? true : false;
	if (EXT_compiled_vertex_array)
	{
		pglLockArrays = (PFNGLLOCKARRAYSEXTPROC)glfwGetProcAddress("glLockArraysEXT");
		pglUnlockArrays = (PFNGLUNLOCKARRAYSEXTPROC)glfwGetProcAddress("glUnlockArraysEXT");

		if (unlikely(!pglLockArrays || !pglUnlockArrays))
		{
			badEntryPoints("GL_EXT_compiled_vertex_array");
			EXT_compiled_vertex_array = false;
		}
	}
	logger << LOG_INFO << "(%c) %s" << (EXT_compiled_vertex_array ? '+' : ' ') << "GL_EXT_compiled_vertex_array";

	// Check for draw_range_elements
	EXT_draw_range_elements = glfwExtensionSupported("GL_EXT_draw_range_elements") ? true : false;
	if (EXT_draw_range_elements)
	{
		pglDrawRangeElements = (PFNGLDRAWRANGEELEMENTSEXTPROC)glfwGetProcAddress("glDrawRangeElementsEXT");

		if (unlikely(!pglDrawRangeElements))
		{
			badEntryPoints("GL_EXT_draw_range_elements");
			EXT_draw_range_elements = false;
		}
	}

	// Check for 3D textures
	EXT_texture3D = glfwExtensionSupported("GL_EXT_texture3D") ? true : false;
	if (EXT_texture3D)
	{
		pglTexImage3D = (PFNGLTEXIMAGE3DEXTPROC)glfwGetProcAddress("glTexImage3DEXT");

		if (unlikely(!pglTexImage3D))
		{
			badEntryPoints("GL_EXT_texture3D");
			EXT_texture3D = false;
		}
	}	
	logger << LOG_INFO << "(%c) %s" << (EXT_texture3D ? '+' : ' ') << "GL_EXT_texture3D";

	// Check for depth-stencil textures support
	EXT_packed_depth_stencil = glfwExtensionSupported("GL_EXT_packed_depth_stencil") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_packed_depth_stencil ? '+' : ' ') << "GL_EXT_packed_depth_stencil";

	// Check for packed float textures support
	EXT_packed_float = glfwExtensionSupported("GL_EXT_packed_float") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_packed_float ? '+' : ' ') << "GL_EXT_packed_float";

	// Check for integer textures support
	EXT_texture_integer = glfwExtensionSupported("GL_EXT_texture_integer") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_texture_integer ? '+' : ' ') << "GL_EXT_texture_integer";

	// Check for shared exponent textures support
	EXT_texture_shared_exponent = glfwExtensionSupported("GL_EXT_texture_shared_exponent") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_texture_shared_exponent ? '+' : ' ') << "GL_EXT_texture_shared_exponent";

	// Check for CLAMP_TO_EDGE support
	EXT_texture_edge_clamp = glfwExtensionSupported("GL_EXT_texture_edge_clamp") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_texture_edge_clamp ? '+' : ' ') << "GL_EXT_texture_edge_clamp";

	// Check for anisotropic filtering
	EXT_texture_filter_anisotropic = glfwExtensionSupported("GL_EXT_texture_filter_anisotropic") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_texture_filter_anisotropic ? '+' : ' ') << "GL_EXT_texture_filter_anisotropic";

	// Check for S3TC texture compression support
	EXT_texture_compression_s3tc = glfwExtensionSupported("GL_EXT_texture_compression_s3tc") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_texture_compression_s3tc ? '+' : ' ') << "GL_EXT_texture_compression_s3tc";

	// Check for RGTC texture compression support
	EXT_texture_compression_rgtc = glfwExtensionSupported("GL_EXT_texture_compression_rgtc") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (EXT_texture_compression_rgtc ? '+' : ' ') << "GL_EXT_texture_compression_rgtc";

	// Check for cube maps
	ARB_texture_cube_map = glfwExtensionSupported("GL_ARB_texture_cube_map") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (ARB_texture_cube_map ? '+' : ' ') << "GL_ARB_texture_cube_map";

	// Check for red-green textures support
	ARB_texture_rg = glfwExtensionSupported("GL_ARB_texture_rg") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (ARB_texture_rg ? '+' : ' ') << "GL_ARB_texture_rg";

	// Check for floating-point textures support
	ARB_texture_float = glfwExtensionSupported("GL_ARB_texture_float") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (ARB_texture_float ? '+' : ' ') << "GL_ARB_texture_float";

	// Check for depth textures support
	ARB_depth_texture = glfwExtensionSupported("GL_ARB_depth_texture") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (ARB_depth_texture ? '+' : ' ') << "GL_ARB_depth_texture";

	// Check for floating-point depth buffer support
	ARB_depth_buffer_float = glfwExtensionSupported("GL_ARB_depth_buffer_float") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (ARB_depth_buffer_float ? '+' : ' ') << "GL_ARB_depth_buffer_float";

	// Check for support of half-float pixel format
	ARB_half_float_pixel = glfwExtensionSupported("GL_ARB_half_float_pixel") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (ARB_half_float_pixel ? '+' : ' ') << "GL_ARB_half_float_pixel";

	// Check for VBO
	ARB_vertex_buffer_object = glfwExtensionSupported("GL_ARB_vertex_buffer_object") ? true : false;
	if (ARB_vertex_buffer_object)
	{
		pglBindBuffer = (PFNGLBINDBUFFERARBPROC)glfwGetProcAddress("glBindBufferARB");
		pglDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)glfwGetProcAddress("glDeleteBuffersARB");
		pglGenBuffers = (PFNGLGENBUFFERSARBPROC)glfwGetProcAddress("glGenBuffersARB");
		pglIsBuffer = (PFNGLISBUFFERARBPROC)glfwGetProcAddress("glIsBufferARB");
		pglBufferData = (PFNGLBUFFERDATAARBPROC)glfwGetProcAddress("glBufferDataARB");
		pglBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)glfwGetProcAddress("glBufferSubDataARB");
		pglGetBufferSubData = (PFNGLGETBUFFERSUBDATAARBPROC)glfwGetProcAddress("glGetBufferSubDataARB");
		pglMapBuffer = (PFNGLMAPBUFFERARBPROC)glfwGetProcAddress("glMapBufferARB");
		pglUnmapBuffer = (PFNGLUNMAPBUFFERARBPROC)glfwGetProcAddress("glUnmapBufferARB");
		pglGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVARBPROC)glfwGetProcAddress("glGetBufferParameterivARB");
		pglGetBufferPointerv = (PFNGLGETBUFFERPOINTERVARBPROC)glfwGetProcAddress("glGetBufferPointervARB");

		if (unlikely(!pglBindBuffer || !pglDeleteBuffers || !pglGenBuffers ||
			!pglIsBuffer || !pglBufferData || !pglBufferSubData || !pglGetBufferSubData ||
			!pglMapBuffer || !pglUnmapBuffer || !pglGetBufferParameteriv ||
			!pglGetBufferPointerv))
		{
			badEntryPoints("GL_ARB_vertex_buffer_object");
			ARB_vertex_buffer_object = false;
		}
	}
	logger << LOG_INFO << "(%c) %s" << (ARB_vertex_buffer_object ? '+' : ' ') << "GL_ARB_vertex_buffer_object";

	// Check for multitexturing
	ARB_multitexture = glfwExtensionSupported("GL_ARB_multitexture") ? true : false;
	logger << LOG_INFO << "(%c) %s" << (ARB_multitexture ? '+' : ' ') << "GL_ARB_multitexture";
	if (ARB_multitexture)
	{
		pglClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC)glfwGetProcAddress("glClientActiveTextureARB");
		pglActiveTexture = (PFNGLACTIVETEXTUREARBPROC)glfwGetProcAddress("glActiveTextureARB");
		pglMultiTexCoord1s = (PFNGLMULTITEXCOORD1SARBPROC)glfwGetProcAddress("glMultiTexCoord1sARB");
		pglMultiTexCoord1i = (PFNGLMULTITEXCOORD1IARBPROC)glfwGetProcAddress("glMultiTexCoord1iARB");
		pglMultiTexCoord1f = (PFNGLMULTITEXCOORD1FARBPROC)glfwGetProcAddress("glMultiTexCoord1fARB");
		pglMultiTexCoord1d = (PFNGLMULTITEXCOORD1DARBPROC)glfwGetProcAddress("glMultiTexCoord1dARB");
		pglMultiTexCoord2s = (PFNGLMULTITEXCOORD2SARBPROC)glfwGetProcAddress("glMultiTexCoord2sARB");
		pglMultiTexCoord2i = (PFNGLMULTITEXCOORD2IARBPROC)glfwGetProcAddress("glMultiTexCoord2iARB");
		pglMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)glfwGetProcAddress("glMultiTexCoord2fARB");
		pglMultiTexCoord2d = (PFNGLMULTITEXCOORD2DARBPROC)glfwGetProcAddress("glMultiTexCoord2dARB");
		pglMultiTexCoord3s = (PFNGLMULTITEXCOORD3SARBPROC)glfwGetProcAddress("glMultiTexCoord3sARB");
		pglMultiTexCoord3i = (PFNGLMULTITEXCOORD3IARBPROC)glfwGetProcAddress("glMultiTexCoord3iARB");
		pglMultiTexCoord3f = (PFNGLMULTITEXCOORD3FARBPROC)glfwGetProcAddress("glMultiTexCoord3fARB");
		pglMultiTexCoord3d = (PFNGLMULTITEXCOORD3DARBPROC)glfwGetProcAddress("glMultiTexCoord3dARB");
		pglMultiTexCoord4s = (PFNGLMULTITEXCOORD4SARBPROC)glfwGetProcAddress("glMultiTexCoord4sARB");
		pglMultiTexCoord4i = (PFNGLMULTITEXCOORD4IARBPROC)glfwGetProcAddress("glMultiTexCoord4iARB");
		pglMultiTexCoord4f = (PFNGLMULTITEXCOORD4FARBPROC)glfwGetProcAddress("glMultiTexCoord4fARB");
		pglMultiTexCoord4d = (PFNGLMULTITEXCOORD4DARBPROC)glfwGetProcAddress("glMultiTexCoord4dARB");
		pglMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVARBPROC)glfwGetProcAddress("glMultiTexCoord1svARB");
		pglMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVARBPROC)glfwGetProcAddress("glMultiTexCoord1ivARB");
		pglMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVARBPROC)glfwGetProcAddress("glMultiTexCoord1fvARB");
		pglMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVARBPROC)glfwGetProcAddress("glMultiTexCoord1dvARB");
		pglMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVARBPROC)glfwGetProcAddress("glMultiTexCoord2svARB");
		pglMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVARBPROC)glfwGetProcAddress("glMultiTexCoord2ivARB");
		pglMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVARBPROC)glfwGetProcAddress("glMultiTexCoord2fvARB");
		pglMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVARBPROC)glfwGetProcAddress("glMultiTexCoord2dvARB");
		pglMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVARBPROC)glfwGetProcAddress("glMultiTexCoord3svARB");
		pglMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVARBPROC)glfwGetProcAddress("glMultiTexCoord3ivARB");
		pglMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVARBPROC)glfwGetProcAddress("glMultiTexCoord3fvARB");
		pglMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVARBPROC)glfwGetProcAddress("glMultiTexCoord3dvARB");
		pglMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVARBPROC)glfwGetProcAddress("glMultiTexCoord4svARB");
		pglMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVARBPROC)glfwGetProcAddress("glMultiTexCoord4ivARB");
		pglMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVARBPROC)glfwGetProcAddress("glMultiTexCoord4fvARB");
		pglMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVARBPROC)glfwGetProcAddress("glMultiTexCoord4dvARB");

		if (unlikely(!pglClientActiveTexture || !pglActiveTexture ||
			!pglMultiTexCoord1s || !pglMultiTexCoord1i || !pglMultiTexCoord1f || !pglMultiTexCoord1d ||
			!pglMultiTexCoord2s || !pglMultiTexCoord2i || !pglMultiTexCoord2f || !pglMultiTexCoord2d ||
			!pglMultiTexCoord3s || !pglMultiTexCoord3i || !pglMultiTexCoord3f || !pglMultiTexCoord3d ||
			!pglMultiTexCoord4s || !pglMultiTexCoord4i || !pglMultiTexCoord4f || !pglMultiTexCoord4d ||
			!pglMultiTexCoord1sv || !pglMultiTexCoord1iv || !pglMultiTexCoord1fv || !pglMultiTexCoord1dv ||
			!pglMultiTexCoord2sv || !pglMultiTexCoord2iv || !pglMultiTexCoord2fv || !pglMultiTexCoord2dv ||
			!pglMultiTexCoord3sv || !pglMultiTexCoord3iv || !pglMultiTexCoord3fv || !pglMultiTexCoord3dv ||
			!pglMultiTexCoord4sv || !pglMultiTexCoord4iv || !pglMultiTexCoord4fv || !pglMultiTexCoord4dv))
		{
			badEntryPoints("GL_ARB_multitexture");
			ARB_multitexture = false;
		}
	}

	// Check for shaders
	ARB_vertex_program = glfwExtensionSupported("GL_ARB_vertex_program") ? true : false;
	if (ARB_vertex_program)
	{
		pglVertexAttrib1s = (PFNGLVERTEXATTRIB1SARBPROC)glfwGetProcAddress("glVertexAttrib1sARB");
		pglVertexAttrib1f = (PFNGLVERTEXATTRIB1FARBPROC)glfwGetProcAddress("glVertexAttrib1fARB");
		pglVertexAttrib1d = (PFNGLVERTEXATTRIB1DARBPROC)glfwGetProcAddress("glVertexAttrib1dARB");
		pglVertexAttrib2s = (PFNGLVERTEXATTRIB2SARBPROC)glfwGetProcAddress("glVertexAttrib2sARB");
		pglVertexAttrib2f = (PFNGLVERTEXATTRIB2FARBPROC)glfwGetProcAddress("glVertexAttrib2fARB");
		pglVertexAttrib2d = (PFNGLVERTEXATTRIB2DARBPROC)glfwGetProcAddress("glVertexAttrib2dARB");
		pglVertexAttrib3s = (PFNGLVERTEXATTRIB3SARBPROC)glfwGetProcAddress("glVertexAttrib3sARB");
		pglVertexAttrib3f = (PFNGLVERTEXATTRIB3FARBPROC)glfwGetProcAddress("glVertexAttrib3fARB");
		pglVertexAttrib3d = (PFNGLVERTEXATTRIB3DARBPROC)glfwGetProcAddress("glVertexAttrib3dARB");
		pglVertexAttrib4s = (PFNGLVERTEXATTRIB4SARBPROC)glfwGetProcAddress("glVertexAttrib4sARB");
		pglVertexAttrib4f = (PFNGLVERTEXATTRIB4FARBPROC)glfwGetProcAddress("glVertexAttrib4fARB");
		pglVertexAttrib4d = (PFNGLVERTEXATTRIB4DARBPROC)glfwGetProcAddress("glVertexAttrib4dARB");
		pglVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBARBPROC)glfwGetProcAddress("glVertexAttrib4NubARB");
		pglVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVARBPROC)glfwGetProcAddress("glVertexAttrib1svARB");
		pglVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVARBPROC)glfwGetProcAddress("glVertexAttrib1fvARB");
		pglVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVARBPROC)glfwGetProcAddress("glVertexAttrib1dvARB");
		pglVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVARBPROC)glfwGetProcAddress("glVertexAttrib2svARB");
		pglVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVARBPROC)glfwGetProcAddress("glVertexAttrib2fvARB");
		pglVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVARBPROC)glfwGetProcAddress("glVertexAttrib2dvARB");
		pglVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVARBPROC)glfwGetProcAddress("glVertexAttrib3svARB");
		pglVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVARBPROC)glfwGetProcAddress("glVertexAttrib3fvARB");
		pglVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVARBPROC)glfwGetProcAddress("glVertexAttrib3dvARB");
		pglVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVARBPROC)glfwGetProcAddress("glVertexAttrib4bvARB");
		pglVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVARBPROC)glfwGetProcAddress("glVertexAttrib4svARB");
		pglVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVARBPROC)glfwGetProcAddress("glVertexAttrib4ivARB");
		pglVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVARBPROC)glfwGetProcAddress("glVertexAttrib4ubvARB");
		pglVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVARBPROC)glfwGetProcAddress("glVertexAttrib4usvARB");
		pglVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVARBPROC)glfwGetProcAddress("glVertexAttrib4uivARB");
		pglVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVARBPROC)glfwGetProcAddress("glVertexAttrib4fvARB");
		pglVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVARBPROC)glfwGetProcAddress("glVertexAttrib4dvARB");
		pglVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVARBPROC)glfwGetProcAddress("glVertexAttrib4NbvARB");
		pglVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVARBPROC)glfwGetProcAddress("glVertexAttrib4NsvARB");
		pglVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVARBPROC)glfwGetProcAddress("glVertexAttrib4NivARB");
		pglVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVARBPROC)glfwGetProcAddress("glVertexAttrib4NubvARB");
		pglVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVARBPROC)glfwGetProcAddress("glVertexAttrib4NusvARB");
		pglVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVARBPROC)glfwGetProcAddress("glVertexAttrib4NuivARB");
		pglVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)glfwGetProcAddress("glVertexAttribPointerARB");
		pglEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)glfwGetProcAddress("glEnableVertexAttribArrayARB");
		pglDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)glfwGetProcAddress("glDisableVertexAttribArrayARB");
		pglGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVARBPROC)glfwGetProcAddress("glGetVertexAttribdvARB");
		pglGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVARBPROC)glfwGetProcAddress("glGetVertexAttribfvARB");
		pglGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVARBPROC)glfwGetProcAddress("glGetVertexAttribivARB");
		pglGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC)glfwGetProcAddress("glGetVertexAttribPointervARB");

		if (unlikely(!pglVertexAttrib1s || !pglVertexAttrib1f || !pglVertexAttrib1d ||
			!pglVertexAttrib2s || !pglVertexAttrib2f || !pglVertexAttrib2d ||
			!pglVertexAttrib3s || !pglVertexAttrib3f || !pglVertexAttrib3d ||
			!pglVertexAttrib4s || !pglVertexAttrib4f || !pglVertexAttrib4d ||
			!pglVertexAttrib4Nub ||
			!pglVertexAttrib1sv || !pglVertexAttrib1fv || !pglVertexAttrib1dv ||
			!pglVertexAttrib2sv || !pglVertexAttrib2fv || !pglVertexAttrib2dv ||
			!pglVertexAttrib3sv || !pglVertexAttrib3fv || !pglVertexAttrib3dv ||
			!pglVertexAttrib4bv || !pglVertexAttrib4sv || !pglVertexAttrib4iv ||
			!pglVertexAttrib4ubv || !pglVertexAttrib4usv || !pglVertexAttrib4uiv ||
			!pglVertexAttrib4fv || !pglVertexAttrib4dv ||
			!pglVertexAttrib4Nbv || !pglVertexAttrib4Nsv || !pglVertexAttrib4Niv ||
			!pglVertexAttrib4Nubv || !pglVertexAttrib4Nusv || !pglVertexAttrib4Nuiv ||
			!pglVertexAttribPointer || !pglEnableVertexAttribArray ||
			!pglDisableVertexAttribArray ||
			!pglGetVertexAttribdv || !pglGetVertexAttribfv || !pglGetVertexAttribiv ||
			!pglGetVertexAttribPointerv))
		{
			badEntryPoints("GL_ARB_vertex_program");
			ARB_vertex_program = false;
		}
	}
	logger << LOG_INFO << "(%c) %s" << (ARB_vertex_program ? '+' : ' ') << "GL_ARB_vertex_program";

	ARB_fragment_program = glfwExtensionSupported("GL_ARB_fragment_program") ? true : false;
	if (ARB_vertex_program || ARB_fragment_program)
	{
		pglProgramString = (PFNGLPROGRAMSTRINGARBPROC)glfwGetProcAddress("glProgramStringARB");
		pglBindProgram = (PFNGLBINDPROGRAMARBPROC)glfwGetProcAddress("glBindProgramARB");
		pglDeletePrograms = (PFNGLDELETEPROGRAMSARBPROC)glfwGetProcAddress("glDeleteProgramsARB");
		pglGenPrograms = (PFNGLGENPROGRAMSARBPROC)glfwGetProcAddress("glGenProgramsARB");
		pglProgramEnvParameter4d = (PFNGLPROGRAMENVPARAMETER4DARBPROC)glfwGetProcAddress("glProgramEnvParameter4dARB");
		pglProgramEnvParameter4dv = (PFNGLPROGRAMENVPARAMETER4DVARBPROC)glfwGetProcAddress("glProgramEnvParameter4dvARB");
		pglProgramEnvParameter4f = (PFNGLPROGRAMENVPARAMETER4FARBPROC)glfwGetProcAddress("glProgramEnvParameter4fARB");
		pglProgramEnvParameter4fv = (PFNGLPROGRAMENVPARAMETER4FVARBPROC)glfwGetProcAddress("glProgramEnvParameter4fvARB");
		pglProgramLocalParameter4d = (PFNGLPROGRAMLOCALPARAMETER4DARBPROC)glfwGetProcAddress("glProgramLocalParameter4dARB");
		pglProgramLocalParameter4dv = (PFNGLPROGRAMLOCALPARAMETER4DVARBPROC)glfwGetProcAddress("glProgramLocalParameter4dvARB");
		pglProgramLocalParameter4f = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC)glfwGetProcAddress("glProgramLocalParameter4fARB");
		pglProgramLocalParameter4fv = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)glfwGetProcAddress("glProgramLocalParameter4fvARB");
		pglGetProgramEnvParameterdv = (PFNGLGETPROGRAMENVPARAMETERDVARBPROC)glfwGetProcAddress("glGetProgramEnvParameterdvARB");
		pglGetProgramEnvParameterfv = (PFNGLGETPROGRAMENVPARAMETERFVARBPROC)glfwGetProcAddress("glGetProgramEnvParameterfvARB");
		pglGetProgramLocalParameterdv = (PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC)glfwGetProcAddress("glGetProgramLocalParameterdvARB");
		pglGetProgramLocalParameterfv = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)glfwGetProcAddress("glGetProgramLocalParameterfvARB");
		pglGetProgramiv = (PFNGLGETPROGRAMIVARBPROC)glfwGetProcAddress("glGetProgramivARB");
		pglGetProgramString = (PFNGLGETPROGRAMSTRINGARBPROC)glfwGetProcAddress("glGetProgramStringARB");
		pglIsProgram = (PFNGLISPROGRAMARBPROC)glfwGetProcAddress("glIsProgramARB");

		if (unlikely(!pglProgramString || !pglBindProgram || !pglDeletePrograms ||
			!pglGenPrograms || !pglProgramEnvParameter4d || !pglProgramEnvParameter4dv ||
			!pglProgramEnvParameter4f || !pglProgramEnvParameter4fv ||
			!pglProgramLocalParameter4d || !pglProgramLocalParameter4dv ||
			!pglProgramLocalParameter4f || !pglProgramLocalParameter4fv ||
			!pglGetProgramEnvParameterdv || !pglGetProgramEnvParameterfv ||
			!pglGetProgramLocalParameterdv || !pglGetProgramLocalParameterfv ||
			!pglGetProgramiv || !pglGetProgramString || !pglIsProgram))
		{
			if (ARB_vertex_program)
				badEntryPoints("GL_ARB_vertex_program");
			if (ARB_fragment_program)
				badEntryPoints("GL_ARB_fragment_program");
			ARB_vertex_program = false;
			ARB_fragment_program = false;
		}
	}
	logger << LOG_INFO << "(%c) %s" << (ARB_fragment_program ? '+' : ' ') << "GL_ARB_fragment_program";
}

//
// Collects OpenGL limits
//
void GL_CollectLimits()
{
	logger << LOG_INFO << "\nOpenGL limits\n-------------";

	glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, &GL_MaxClientAttribStackDepth);
	logger << LOG_INFO << "GL_MAX_CLIENT_ATTRIB_STACK_DEPTH = %d" << GL_MaxClientAttribStackDepth;
	glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &GL_MaxAttribStackDepth);
	logger << LOG_INFO << "GL_MAX_ATTRIB_STACK_DEPTH = %d" << GL_MaxAttribStackDepth;
	glGetIntegerv(GL_MAX_CLIP_PLANES, &GL_MaxClipPlanes);
	logger << LOG_INFO << "GL_MAX_CLIP_PLANES = %d" << GL_MaxClipPlanes;
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &GL_MaxElementIndices);
	logger << LOG_INFO << "GL_MAX_ELEMENTS_INDICES = %d" << GL_MaxElementIndices;
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &GL_MaxElementVertices);
	logger << LOG_INFO << "GL_MAX_ELEMENTS_VERTICES = %d" << GL_MaxElementVertices;
	glGetIntegerv(GL_MAX_LIGHTS, &GL_MaxLights);
	logger << LOG_INFO << "GL_MAX_LIGHTS = %d" << GL_MaxLights;
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &GL_MaxModelViewStackDepth);
	logger << LOG_INFO << "GL_MAX_MODELVIEW_STACK_DEPTH = %d" << GL_MaxModelViewStackDepth;
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &GL_MaxProjectionStackDepth);
	logger << LOG_INFO << "GL_MAX_PROJECTION_STACK_DEPTH = %d" << GL_MaxProjectionStackDepth;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &GL_MaxTextureSize);
	logger << LOG_INFO << "GL_MAX_TEXTURE_SIZE = %d" << GL_MaxTextureSize;

	if (EXT_draw_range_elements)
	{
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES_EXT, &GL_MaxElementsVertices);
		logger << LOG_INFO << "GL_MAX_ELEMENTS_VERTICES = %d" << GL_MaxElementsVertices;
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES_EXT, &GL_MaxElementsIndices);
		logger << LOG_INFO << "GL_MAX_ELEMENTS_INDICES = %d" << GL_MaxElementsIndices;
	}

	if (ARB_texture_cube_map)
	{
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, &GL_MaxCubeMapSize);
		logger << LOG_INFO << "GL_MAX_CUBE_MAP_TEXTURE_SIZE = %d" << GL_MaxCubeMapSize;
	}

	if (EXT_texture3D)
	{
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &GL_MaxTextureSize3D);
		logger << LOG_INFO << "GL_MAX_3D_TEXTURE_SIZE = %d" << GL_MaxTextureSize3D;
	}

	if (ARB_multitexture)
	{
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &GL_MaxTextureUnits);
		logger << LOG_INFO << "GL_MAX_TEXTURE_UNITS = %d" << GL_MaxTextureUnits;
	}

	if (EXT_texture_filter_anisotropic)
	{
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &GL_MaxTextureAnisotropy);
		logger << LOG_INFO << "GL_MAX_TEXTURE_MAX_ANISOTROPY = %d" << GL_MaxTextureAnisotropy;
	}

	if (ARB_vertex_program)
	{
		pglGetProgramiv(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &GL_MaxVertexTemporaries);
		logger << LOG_INFO << "GL_VERTEX_PROGRAM.GL_MAX_PROGRAM_TEMPORARIES = %d" << GL_MaxVertexTemporaries;
	}

	if (ARB_fragment_program)
	{
		pglGetProgramiv(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &GL_MaxFragmentTemporaries);
		logger << LOG_INFO << "GL_FRAGMENT_PROGRAM.GL_MAX_PROGRAM_TEMPORARIES = %d" << GL_MaxFragmentTemporaries;
	}
}
