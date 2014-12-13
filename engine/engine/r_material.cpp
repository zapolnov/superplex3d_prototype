#include <core/logger.h>
#include <core/exception.h>
#include <engine/render.h>
#include <engine/r_opengl.h>
#include <math/vector4.h>
#include <interfaces/file_system.h>
#include <tinyxml.h>
#include <GLFW/glfw3.h>

/* Structures */

/** Material. */
struct MaterialImpl : public Material
{
public:
	/** Constructor. */
	inline MaterialImpl()
	{
	}
};

/** Description of OpenGL constant. */
struct GLConstDesc
{
	const char * str;	/**< Name of the constant. */
	GLenum value;		/**< Value of the constant. */
};


/* Private variables */

/** Source blending factors. */
static GLConstDesc g_SrcBlendFactors[] =
{
	{ "GL_ZERO",				GL_ZERO					},
	{ "GL_ONE",					GL_ONE					},
	{ "GL_DST_COLOR",			GL_DST_COLOR			},
	{ "GL_ONE_MINUS_DST_COLOR",	GL_ONE_MINUS_DST_COLOR	},
	{ "GL_SRC_ALPHA",			GL_SRC_ALPHA			},
	{ "GL_ONE_MINUS_SRC_ALPHA",	GL_ONE_MINUS_SRC_ALPHA	},
	{ "GL_DST_ALPHA",			GL_DST_ALPHA			},
	{ "GL_ONE_MINUS_DST_ALPHA",	GL_ONE_MINUS_DST_ALPHA	},
	{ "GL_SRC_ALPHA_SATURATE",	GL_SRC_ALPHA_SATURATE	},
};

/** Destination blending factors. */
static GLConstDesc g_DstBlendFactors[] =
{
	{ "GL_ZERO",				GL_ZERO					},
	{ "GL_ONE",					GL_ONE					},
	{ "GL_SRC_COLOR",			GL_SRC_COLOR			},
	{ "GL_ONE_MINUS_SRC_COLOR",	GL_ONE_MINUS_SRC_COLOR	},
	{ "GL_SRC_ALPHA",			GL_SRC_ALPHA			},
	{ "GL_ONE_MINUS_SRC_ALPHA",	GL_ONE_MINUS_SRC_ALPHA	},
	{ "GL_DST_ALPHA",			GL_DST_ALPHA			},
	{ "GL_ONE_MINUS_DST_ALPHA",	GL_ONE_MINUS_DST_ALPHA	},
};

/** Culling modes. */
static GLConstDesc g_CullModes[] =
{
	{ "GL_NONE",				0,						},
	{ "GL_FRONT",				GL_FRONT				},
	{ "GL_BACK",				GL_BACK					},
	{ "GL_FRONT_AND_BACK",		GL_FRONT_AND_BACK		}
};

/** Texture environment modes. */
static GLConstDesc g_TexEnvModes[] =
{
	{ "GL_MODULATE",			GL_MODULATE				},
	{ "GL_REPLACE",				GL_REPLACE				},
	{ "GL_DECAL",				GL_DECAL				},
	{ "GL_BLEND",				GL_BLEND				}
};


/* Macros */

#define PARSE_BOOLEAN_VALUE(name, target) \
	if (tag == (name)) \
	{ \
		tagParsed = true; \
		const char * value = e->Attribute("enabled"); \
		if (unlikely(!value)) \
			logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << (name) << "enabled"; \
		else \
		{ \
			if (!strcmp(value, "false")) \
				(target) = false; \
			else if (!strcmp(value, "true")) \
				(target) = true; \
			else \
				logger << LOG_ERROR << "%s: expected boolean value in %s.%s" << file << (name) << "enabled"; \
		} \
		continue; \
	}

#define PARSE_ENUM(name, attr, values, target) \
	if (tag == (name)) \
	{ \
		tagParsed = true; \
		const char * value = e->Attribute((attr)); \
		if (unlikely(!value)) \
			logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << (name) << (attr); \
		else \
		{ \
			uint i; \
			for (i = 0; i < (sizeof((values)) / sizeof((values)[0])); i++) \
			{ \
				if (!strcmp(value, (values)[i].str)) \
				{ \
					(target) = (values)[i].value; \
					break; \
				} \
			} \
			if (unlikely(i == (sizeof((values)) / sizeof((values)[0])))) \
			{ \
				logger << LOG_ERROR << "%s: invalid value of %s.%s: \"%s\"." \
					<< file << (name) << (attr) << value; \
			} \
		} \
		/* continue; // Don't place 'continue' here. */ \
	}

#define PARSE_OBJECT(name, target, constructor) \
	if (tag == (name)) \
	{ \
		tagParsed = true; \
		const char * value = e->Attribute("file"); \
		if (unlikely(!value)) \
			logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << (name) << "file"; \
		else \
			(target) = (constructor)(value); \
		continue; \
	}

#define PARSE_COLOR(name, target) \
	if (tag == (name)) \
	{ \
		tagParsed = true; \
		const char * R = e->Attribute("red"); \
		const char * G = e->Attribute("green"); \
		const char * B = e->Attribute("blue"); \
		const char * A = e->Attribute("alpha"); \
		if (unlikely(!R)) \
			logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << (name) << "red"; \
		else if (unlikely(!G)) \
			logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << (name) << "green"; \
		else if (unlikely(!B)) \
			logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << (name) << "blue"; \
		else if (unlikely(!A)) \
			logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << (name) << "alpha"; \
		else \
		{ \
			(target).set( \
				(Vector4::VALUE_TYPE)atof(R), \
				(Vector4::VALUE_TYPE)atof(G), \
				(Vector4::VALUE_TYPE)atof(B), \
				(Vector4::VALUE_TYPE)atof(A) \
			); \
		} \
		continue; \
	}


/* Private functions */

//
// Parses the texture parameters
//
static
void parseTexture(const std::string & file, MaterialImpl * m, TiXmlElement * n)
{
	const char * value = n->Attribute("unit");
	if (unlikely(!value))
	{
		logger << LOG_ERROR << "%s: missing attribute %s.%s" << file << "texture" << "unit";
		return;
	}

	int unit = atoi(value) - 1;
	if (unlikely(unit < 0 || unit >= MAX_TEXTURES))
	{
		logger << LOG_ERROR << "%s: invalid value of %s.%s: \"%s\"."
			<< file << "texture" << "unit" << value;
		return;
	}

	for (TiXmlNode * c = n->FirstChild(); c != 0; c = c->NextSibling())
	{
		if (c->Type() != TiXmlNode::ELEMENT)
			continue;

		TiXmlElement * e = c->ToElement();
		if (!e)
			continue;

		std::string tag = e->Value();
		bool tagParsed = false;

		PARSE_OBJECT("image", m->textures[unit].texture, R_LoadTexture);
		PARSE_ENUM("environment", "mode", g_TexEnvModes, m->textures[unit].env);

		if (unlikely(!tagParsed))
			logger << LOG_ERROR << "%s: unknown tag \"%s\"" << file << ("texture" + std::string(".") + tag);
	}
}

//
// Parses the material XML
//
static
void parseMaterial(const std::string & file, MaterialImpl * m, TiXmlElement * root)
{
	for (TiXmlNode * c = root->FirstChild(); c != 0; c = c->NextSibling())
	{
		if (c->Type() != TiXmlNode::ELEMENT)
			continue;

		TiXmlElement * e = c->ToElement();
		if (!e)
			continue;

		std::string tag = e->Value();
		bool tagParsed = false;

		if (tag == "texture")
		{
			parseTexture(file, m, e);
			continue;
		}

		PARSE_BOOLEAN_VALUE("wireframe", m->wireframe);
		PARSE_BOOLEAN_VALUE("blending", m->blending);
		PARSE_COLOR("color", m->color); 
		PARSE_OBJECT("vertex_shader", m->vertexShader, R_CompileVertexShader);
		PARSE_OBJECT("fragment_shader", m->fragmentShader, R_CompileFragmentShader);

		PARSE_ENUM("blend_func", "src", g_SrcBlendFactors, m->blendSrc);
		PARSE_ENUM("blend_func", "dst", g_DstBlendFactors, m->blendDst);
		PARSE_ENUM("culling", "face", g_CullModes, m->cullMode);
		m->cullFace = (m->cullMode != 0);

		if (unlikely(!tagParsed))
			logger << LOG_ERROR << "%s: unknown tag \"%s\"" << file << tag;
	}
}


/* Public functions */

//
// Creates new material
//
MaterialPtr R_CreateMaterial()
{
	return new MaterialImpl;
}

//
// Loads material from the specified file
//
MaterialPtr R_LoadMaterial(const std::string & file)
{
	QByteArray xml = fileSystem()->getFileContents(file);

	TiXmlDocument doc;
	if (!doc.Parse(xml.constData()))
		throw Exception(QObject::tr("Unable to parse XML file \"%1\"").arg(file));

	TiXmlElement * root = doc.RootElement();
	if (root->Value() != std::string("material"))
		throw Exception(QObject::tr("Invalid material file \"%1\"").arg(file));

	MaterialPtr material = new MaterialImpl;
	parseMaterial(file, material.as<MaterialImpl>(), root);

	return material;
}

//
// Binds the specified material
//
void R_SetMaterial(const MaterialPtr & material)
{
	const MaterialImpl * m = material.as<MaterialImpl>();

	// Bind textures
	for (uint i = 0; i < Q_MIN(MAX_TEXTURES, (uint)GL_MaxTextureUnits); i++)
		R_BindTexture(i, m->textures[i].texture, m->textures[i].env);

	// Bind the vertex shader
	if (m && m->vertexShader)
		R_BindVertexShader(m->vertexShader);
	else
		R_UnbindVertexShader();

	// Bind the fragment shader
	if (m && m->fragmentShader)
		R_BindFragmentShader(m->fragmentShader);
	else
		R_UnbindFragmentShader();

	// Enable/disable wireframe
	if (m && m->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Setup face culling
	if (!m || !m->cullFace)
		glDisable(GL_CULL_FACE);
	else
	{
		Q_ASSERT(m->cullMode == GL_FRONT || m->cullMode == GL_BACK || m->cullMode == GL_FRONT_AND_BACK);

		glEnable(GL_CULL_FACE);
		glCullFace(m->cullMode);
	}

	// Setup blending
	if (!m || !m->blending)
		glDisable(GL_BLEND);
	else
	{
		Q_ASSERT(m->blendSrc == GL_ZERO || m->blendSrc == GL_ONE || m->blendSrc == GL_DST_COLOR ||
				 m->blendSrc == GL_ONE_MINUS_DST_COLOR || m->blendSrc == GL_SRC_ALPHA ||
				 m->blendSrc == GL_ONE_MINUS_SRC_ALPHA || m->blendSrc == GL_DST_ALPHA ||
				 m->blendSrc == GL_ONE_MINUS_DST_ALPHA || m->blendSrc == GL_SRC_ALPHA_SATURATE);
		Q_ASSERT(m->blendDst == GL_ZERO || m->blendDst == GL_ONE || m->blendDst == GL_SRC_COLOR ||
				 m->blendDst == GL_ONE_MINUS_SRC_COLOR || m->blendDst == GL_SRC_ALPHA ||
				 m->blendDst == GL_ONE_MINUS_SRC_ALPHA || m->blendDst == GL_DST_ALPHA ||
				 m->blendDst == GL_ONE_MINUS_DST_ALPHA);
		glEnable(GL_BLEND);
		glBlendFunc(m->blendSrc, m->blendDst);
	}

	// Setup constant color
	glColor4fv(m->color.ptr());
}
