#include <engine/render.h>
#include <engine/r_opengl.h>
#include <3rdparty/glsl_compiler/glsl_compiler.h>
#include <engine/filesys.h>
#include <core/logger.h>
#include <map>

/* Structures */

/** Shader. */
struct ShaderImpl : public Shader
{
	/**
	 * Constructor.
     * @param type Type of the shader.
	 */
	inline ShaderImpl(GLenum type)
		: m_Type(type)
	{
		Q_ASSERT(type == GL_VERTEX_PROGRAM_ARB || type == GL_FRAGMENT_PROGRAM_ARB);
		Q_ASSERT(pglGenPrograms);
		pglGenPrograms(1, &m_ShaderID);
	}

	/** Destructor. */
	inline ~ShaderImpl()
	{
		Q_ASSERT(pglDeletePrograms);
		pglDeletePrograms(1, &m_ShaderID);
	}

	/** Returns type of the shader. */
	inline GLenum type() const
	{
		return m_Type;
	}

	/** Binds the shader. */
	inline void bind() const
	{
		Q_ASSERT(pglBindProgram);
		pglBindProgram(m_Type, m_ShaderID);
	}

	/** Unbinds the shader. */
	inline void unbind() const
	{
		Q_ASSERT(isBound());
		pglBindProgram(m_Type, 0);
	}

	/** Returns true if shader is currently bound. */
	inline bool isBound() const
	{
		GLint binding;

		Q_ASSERT(pglGetProgramiv);
		pglGetProgramiv(m_Type, GL_PROGRAM_BINDING_ARB, &binding);
		return ((GLuint)binding == m_ShaderID);
	}

	/**
	 * Compiles the shader.
	 * @param src Shader source code.
	 */
	inline bool compile(const std::string & src)
	{
		Q_ASSERT(isBound());

		Q_ASSERT(pglProgramString);
		pglProgramString(m_Type, GL_PROGRAM_FORMAT_ASCII_ARB, src.length(), src.c_str());

		GLint errorPos;
		glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);
		if (unlikely(errorPos != -1))
		{
			logger << LOG_ERROR << "Shader compilation error:\n%s"
				<< (const char *)glGetString(GL_PROGRAM_ERROR_STRING_ARB);
			return false;
		}

		return true;
	}

private:
	GLenum m_Type;		/**< Type of the shader. */
	GLuint m_ShaderID;	/**< OpenGL ID of the shader. */
};


/* Private variables */

static std::map<std::string, ShaderPtr> g_VertexShaders;		/**< Map of vertex shaders. */
static std::map<std::string, ShaderPtr> g_FragmentShaders;		/**< Map of fragment shaders. */


/* Public functions */

//
// Creates a new vertex shader
//
ShaderPtr R_CompileVertexShader(const std::string & file)
{
	if (unlikely(!ARB_vertex_program))
		return ShaderPtr();

	// Check whether we have already loaded the shader
	std::map<std::string, ShaderPtr>::iterator it = g_VertexShaders.find(file);
	if (it != g_VertexShaders.end())
		return it->second;

	logger << LOG_INFO << "Compiling %s shader \"%s\"" << "vertex" << file;
	std::string source = GLSL_CompileVertexShader(FS_GetFileContents(file));

#ifdef _DEBUG
	logger << LOG_TRACE << "%s" << source;
#endif

	ShaderPtr shader = new ShaderImpl(GL_VERTEX_PROGRAM_ARB);
	ShaderImpl * s = shader.as<ShaderImpl>();

	s->bind();
	bool result = s->compile(source);
	s->unbind();

	// Store shader pointer
	ShaderPtr value = result ? shader : ShaderPtr();
	g_VertexShaders.insert(std::pair<std::string, ShaderPtr>(file, value));

	return value;
}

//
// Creates a new fragment shader
//
ShaderPtr R_CompileFragmentShader(const std::string & file)
{
	if (unlikely(!ARB_fragment_program))
		return ShaderPtr();

	// Check whether we have already loaded the shader
	std::map<std::string, ShaderPtr>::iterator it = g_FragmentShaders.find(file);
	if (it != g_FragmentShaders.end())
		return it->second;

	logger << LOG_INFO << "Compiling %s shader \"%s\"" << "fragment" << file;
	std::string source = GLSL_CompileFragmentShader(FS_GetFileContents(file));

#ifdef _DEBUG
	logger << LOG_TRACE << "%s" << source;
#endif

	ShaderPtr shader = new ShaderImpl(GL_FRAGMENT_PROGRAM_ARB);
	ShaderImpl * s = shader.as<ShaderImpl>();

	s->bind();
	bool result = s->compile(source);
	s->unbind();

	// Store shader pointer
	ShaderPtr value = result ? shader : ShaderPtr();
	g_FragmentShaders.insert(std::pair<std::string, ShaderPtr>(file, value));

	return value;
}

//
// Binds the specified vertex shader
//
void R_BindVertexShader(const ShaderPtr & shader)
{
	if (unlikely(!ARB_vertex_program))
		return;

	const ShaderImpl * s = shader.as<ShaderImpl>();

	Q_ASSERT(s->type() == GL_VERTEX_PROGRAM_ARB);
	s->bind();

	glEnable(GL_VERTEX_PROGRAM_ARB);
}

//
// Binds the specified fragment shader
//
void R_BindFragmentShader(const ShaderPtr & shader)
{
	if (unlikely(!ARB_fragment_program))
		return;

	const ShaderImpl * s = shader.as<ShaderImpl>();

	Q_ASSERT(s->type() == GL_FRAGMENT_PROGRAM_ARB);
	s->bind();

	glEnable(GL_FRAGMENT_PROGRAM_ARB);
}

//
// Unbinds any currently bound vertex shader
//
void R_UnbindVertexShader()
{
	if (unlikely(!ARB_vertex_program))
		return;

	Q_ASSERT(pglBindProgram);
	pglBindProgram(GL_VERTEX_PROGRAM_ARB, 0);
	glDisable(GL_VERTEX_PROGRAM_ARB);
}

//
// Unbinds any currently bound fragment shader
//
void R_UnbindFragmentShader()
{
	if (unlikely(!ARB_fragment_program))
		return;

	Q_ASSERT(pglBindProgram);
	pglBindProgram(GL_FRAGMENT_PROGRAM_ARB, 0);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
}

//
// Clears the cache of vertex shaders
//
void R_ClearVertexShaderCache()
{
	g_VertexShaders.clear();
}

//
// Clears the cache of fragment shaders
//
void R_ClearFragmentShaderCache()
{
	g_FragmentShaders.clear();
}

//
// Sets value of the specified uniform variable
//
void R_SetShaderUniform(ShaderPtr shader, uint idx, float value)
{
	if (unlikely(!shader))
		return;

	ShaderImpl * s = shader.as<ShaderImpl>();
	s->bind();

	if (s->type() == GL_VERTEX_PROGRAM_ARB)
	{
		if (unlikely(!ARB_vertex_program))
			return;
	}
	else if (s->type() == GL_FRAGMENT_PROGRAM_ARB)
	{
		if (unlikely(!ARB_fragment_program))
			return;
	}

	pglProgramEnvParameter4f(GL_FRAGMENT_PROGRAM_ARB, idx, value, 0.0f, 0.0f, 0.0f);
}
