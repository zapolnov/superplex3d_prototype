#include <engine/render.h>
#include <core/exception.h>
#include <core/logger.h>
#include <mesh/mesh_file.h>
#include <GLFW/glfw3.h>
#include <engine/r_opengl.h>
#include <map>
#include <vector>

/* Structures */

/** Mesh. */
struct MeshImpl : public Mesh
{
public:
	/** Information about the material. */
	struct Material
	{
		MaterialPtr ptr;				/**< Pointer to the material. */
		uint32 firstIndex;				/**< First index. */
		uint32 numIndices;				/**< Number of indices. */
	};

	int vertexFormat;					/**< Vertex format. */
	int stride;							/**< Stride. */
	uint numIndices;					/**< Total number of indices. */
	uint numVertices;					/**< Total number of vertices. */
	GLenum primitive;					/**< Primitive type. */
	BufferObjectPtr vertexBuffer;		/**< Pointer to the vertex buffer. */
	BufferObjectPtr indexBuffer;		/**< Pointer to the index buffer. */
	std::vector<Material> materials;	/**< Set of materials this mesh uses. */

	/** Constructor. */
	MeshImpl()
	{
		vertexBuffer = R_CreateVertexBufferObject();
		indexBuffer = R_CreateIndexBufferObject();
	}

	/** Destructor. */
	~MeshImpl()
	{
		indexBuffer.release();
		vertexBuffer.release();
	}
};


/* Private variables */

static std::map<std::string, MeshPtr> g_Meshes;		/**< Map of meshes. */


/* Private functions */

//
// Binds vertex arrays for mesh rendering
//
static
ushort * R_SetupVertexPointers(const MeshImpl * m)
{
	float * offset = (float *)R_BindBufferObject(m->vertexBuffer);
	if (m->vertexFormat == (VF_Color | VF_Position))
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glInterleavedArrays(GL_C4UB_V3F, 0, offset);
	}
	else if (m->vertexFormat == (VF_Normal | VF_Position))
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glInterleavedArrays(GL_N3F_V3F, 0, offset);
	}
	else if (m->vertexFormat == (VF_TexCoords | VF_Position))
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glInterleavedArrays(GL_T2F_V3F, 0, offset);
	}
	else if (m->vertexFormat == (VF_TexCoords | VF_Color | VF_Position))
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glInterleavedArrays(GL_T2F_C4UB_V3F, 0, offset);
	}
	else if (m->vertexFormat == (VF_TexCoords | VF_Normal | VF_Position))
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glInterleavedArrays(GL_T2F_N3F_V3F, 0, offset);
	}
	else
	{
		if ((m->vertexFormat & VF_TexCoords) != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, m->stride, offset);
			offset += 2;
		}
		if ((m->vertexFormat & VF_Color) != 0)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, m->stride, offset);
			offset += 1;
		}
		if ((m->vertexFormat & VF_Normal) != 0)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, m->stride, offset);
			offset += 3;
		}
		if ((m->vertexFormat & VF_Position) != 0)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, m->stride, offset);
			offset += 3;
		}
	}

	return (ushort *)R_BindBufferObject(m->indexBuffer);
}

//
// Disables vertex arrays after mesh rendering
//
static
void R_DisableVertexPointers(const MeshImpl * m)
{
	R_UnbindBufferObject(m->indexBuffer);
	R_UnbindBufferObject(m->vertexBuffer);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


/* Public functions */

//
// Returns size of a single vertex for the specified vertex format
//
int R_CalcVertexSize(int format)
{
	int size = 0;

	if ((format & VF_TexCoords) != 0)
		size += 2 * sizeof(float);
	if ((format & VF_Color) != 0)
		size += 4;
	if ((format & VF_Normal) != 0)
		size += 3 * sizeof(float);
	if ((format & VF_Position) != 0)
		size += 3 * sizeof(float);

	return size;
}

//
// Creates the box mesh
//
MeshPtr R_CreateBoxMesh(const MaterialPtr & material, int format)
{
	static const float tX[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	static const float tY[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	static const float x[] = { -1.0f,  1.0f, -1.0f, 1.0f };
	static const float y[] = { -1.0f, -1.0f,  1.0f, 1.0f };
	static const int iX[] = { 0, 0, 2 };
	static const int iY[] = { 1, 2, 1 };
	static const int iZ[] = { 2, 1, 0 };

	MeshPtr mesh = new MeshImpl;
	MeshImpl * m = mesh.as<MeshImpl>();

	MeshImpl::Material mat;
	mat.ptr = material;
	mat.firstIndex = 0;
	mat.numIndices = 24;
	m->materials.push_back(mat);

	R_BindBufferObject(m->vertexBuffer);
	R_BindBufferObject(m->indexBuffer);

	m->vertexFormat = format;
	m->stride = R_CalcVertexSize(format);
	m->primitive = GL_TRIANGLE_STRIP;
	m->numVertices = 24;
	m->numIndices = 24;
	R_SetBufferObjectData(m->vertexBuffer, NULL, 24 * m->stride, StaticDraw);
	R_SetBufferObjectData(m->indexBuffer, NULL, 24 * 2, StaticDraw);

	BufferLocker vb(m->vertexBuffer, WriteAccess);
	BufferLocker ib(m->indexBuffer, WriteAccess);
	ushort * index = (ushort *)ib.ptr();
	float * vertex = (float *)vb.ptr();

	int curidx = 0;
	for (int i = 0; i < 6; i++)
	{
		// Calculate normal
		const int idx = i >> 1;
		const float sign = ((i & 1) == 0 ? 1.0f : -1.0f);
		const float nx = (idx == 2 ? sign : 0.0f);
		const float ny = (idx == 1 ? sign : 0.0f);
		const float nz = (idx == 0 ? sign : 0.0f);

		// Generate points for the plane
		for (int j = 0; j < 4; j++)
		{
			// Write texture coordinates
			if ((format & VF_TexCoords) != 0)
			{
				*vertex++ = tX[j];
				*vertex++ = tY[j];
			}

			// Write color
			if ((format & VF_Color) != 0)
			{
				*((uint32 *)vertex) = 0xFFFFFFFF;
				vertex++;
			}

			// Write normal
			if ((format & VF_Normal) != 0)
			{
				*vertex++ = nx;
				*vertex++ = ny;
				*vertex++ = nz;
			}

			// Write position
			if ((format & VF_Position) != 0)
			{
				vertex[iX[idx]] = x[j];
				vertex[iY[idx]] = y[j];
				vertex[iZ[idx]] = sign;
				vertex += 3;
			}

			// Write indexes
			*index++ = curidx++;
		}
	}

	return mesh;
}

/**
 * Loads mesh from the specified file.
 * @param name Name of the mesh file.
 */
MeshPtr R_LoadMesh(const std::string & name)
{
	uint opos = 0, onormal = 0, otexcoord = 0, ocolor = 0;
	AttributeFormat fpos, ftexcoord, fcolor;

	// Check whether we have already loaded the mesh
	std::map<std::string, MeshPtr>::iterator it = g_Meshes.find(name);
	if (it != g_Meshes.end())
		return it->second;

	// Load the mesh
	MeshFile file;
	file.loadFromFile(name);

	// Parse mesh format
	int format = 0;
	if (file.format().isAttributePresent(AttributePosition))
	{
		format |= VF_Position;
		fpos = file.format().attributeFormat(AttributePosition);
		opos = file.format().attributeOffset(AttributePosition);
	}
	if (file.format().isAttributePresent(AttributeNormal))
	{
		if (unlikely(file.format().attributeFormat(AttributeNormal) != FormatFloat3))
			throw Exception(QString("%1: Invalid format of normal vector.").arg(name));
		format |= VF_Normal;
		onormal = file.format().attributeOffset(AttributeNormal);
	}
	if (file.format().isAttributePresent(AttributeTexCoord))
	{
		format |= VF_TexCoords;
		ftexcoord = file.format().attributeFormat(AttributeTexCoord);
		otexcoord = file.format().attributeOffset(AttributeTexCoord);
	}
	if (file.format().isAttributePresent(AttributeColor))
	{
		format |= VF_Color;
		fcolor = file.format().attributeFormat(AttributeColor);
		ocolor = file.format().attributeOffset(AttributeColor);
	}
	if (file.format().isAttributePresent(AttributeSecondaryTexCoord))
		logger << LOG_WARN << "%s: Secondary texture coordinates are ignored." << name;
	if (file.format().isAttributePresent(AttributeSecondaryColor))
		logger << LOG_WARN << "%s: Secondary color is ignored." << name;
	if (file.format().isAttributePresent(AttributeTangent))
		logger << LOG_WARN << "%s: Tangent vector is ignored." << name;

	// Parse primitive type
	GLenum primitiveType;
	switch (file.primitiveTopology())
	{
	case PointList:
		primitiveType = GL_POINTS;
		break;
	case LineList:
		primitiveType = GL_LINES;
		break;
	case LineStrip:
		primitiveType = GL_LINE_STRIP;
		break;
	case TriangleList:
		primitiveType = GL_TRIANGLES;
		break;
	case TriangleStrip:
		primitiveType = GL_TRIANGLE_STRIP;
		break;
	default:
		throw Exception(QString("File \"%1\" has unsupported primitive type.").arg(name));
	}

	// Get number of vertices and indices
	const uint numVertices = file.vertices().count();
	const uint numIndices = file.indices().count();

	// Create the mesh object
	MeshPtr mesh = new MeshImpl;
	MeshImpl * m = mesh.as<MeshImpl>();

	// Bind buffers
	R_BindBufferObject(m->vertexBuffer);
	R_BindBufferObject(m->indexBuffer);

	// Setup mesh data
	m->numVertices = numVertices;
	m->numIndices = numIndices;
	try
	{
		// Store model parameters and allocate memory for data
		m->vertexFormat = format;
		m->stride = R_CalcVertexSize(format);
		m->primitive = primitiveType;
		R_SetBufferObjectData(m->vertexBuffer, NULL, numVertices * m->stride, StaticDraw);
		R_SetBufferObjectData(m->indexBuffer, NULL, numIndices * 2, StaticDraw);

		// Lock the buffers
		BufferLocker vb(m->vertexBuffer, WriteAccess);
		BufferLocker ib(m->indexBuffer, WriteAccess);
		ushort * index = (ushort *)ib.ptr();
		float * vertex = (float *)vb.ptr();

		// Process vertices
		for (uint i = 0; i < numVertices; i++)
		{
			const char * src = (const char *)file.vertices()[i];

			// Write texture coordinates
			if ((format & VF_TexCoords) != 0)
			{
				float x = 0.0f, y = 0.0f;

				switch (ftexcoord)
				{
				case FormatFloat2:
				case FormatFloat3:
				case FormatFloat4:
					y = ((float *)(src + otexcoord))[1];
				case FormatFloat:
					x = ((float *)(src + otexcoord))[0];
					break;
				default:
					throw Exception(QString("%1: invalid format of texture coordinates."));
				}

				*vertex++ = x;
				*vertex++ = y;
			}

			// Write color
			if ((format & VF_Color) != 0)
			{
				float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

				switch (fcolor)
				{
				case FormatFloat4:
					a = ((float *)(src + ocolor))[3];
				case FormatFloat3:
					b = ((float *)(src + ocolor))[2];
				case FormatFloat2:
					g = ((float *)(src + ocolor))[1];
				case FormatFloat:
					r = ((float *)(src + ocolor))[0];
					break;
				default:
					throw Exception(QString("%1: invalid format of color."));
				}

				uint32 color = 0;
				color |= ((uint)(r * 255) & 0xFF) << 0;
				color |= ((uint)(g * 255) & 0xFF) << 8;
				color |= ((uint)(b * 255) & 0xFF) << 16;
				color |= ((uint)(a * 255) & 0xFF) << 24;

				*((uint32 *)vertex) = color;
				vertex++;
			}

			// Write normal
			if ((format & VF_Normal) != 0)
			{
				*vertex++ = ((float *)(src + onormal))[0];
				*vertex++ = ((float *)(src + onormal))[1];
				*vertex++ = ((float *)(src + onormal))[2];
			}

			// Write position
			if ((format & VF_Position) != 0)
			{
				float x = 0.0f, y = 0.0f, z = 0.0f;

				switch (fpos)
				{
				case FormatFloat4:
				case FormatFloat3:
					z = ((float *)(src + opos))[2];
				case FormatFloat2:
					y = ((float *)(src + opos))[1];
				case FormatFloat:
					x = ((float *)(src + opos))[0];
					break;
				default:
					throw Exception(QString("%1: invalid format of vertex."));
				}

				*vertex++ = x;
				*vertex++ = y;
				*vertex++ = z;
			}
		}

		// Process indices
		memcpy(index, file.indices().data(), numIndices * 2);
	}
	catch (...)
	{
		R_UnbindBufferObject(m->indexBuffer);
		R_UnbindBufferObject(m->vertexBuffer);
		throw;
	}

	// Process materials
	for (uint i = 0; i < file.numSubmaterials(); i++)
	{
		const MeshFile::Submaterial & sm = file.submaterial(i);

		MeshImpl::Material mat;
		mat.firstIndex = sm.firstIndex;
		mat.numIndices = sm.numIndices;

		mat.ptr = R_CreateMaterial();
		mat.ptr->color = sm.material->diffuseColor();
		if (sm.material->diffuseTexture().length() > 0)
		{
			mat.ptr->textures[0].texture = R_LoadTexture(sm.material->diffuseTexture().toLocal8Bit());
			mat.ptr->textures[0].env = GL_REPLACE;
		}

		m->materials.push_back(mat);
	}

	// Store mesh pointer
	g_Meshes.insert(std::pair<std::string, MeshPtr>(name, mesh));

	return mesh;
}

//
// Renders the specified mesh
//
void R_RenderMesh(const MeshPtr & mesh)
{
	const MeshImpl * m = mesh.as<MeshImpl>();

#ifdef DONT_USE_VBO_AND_VAR
	float * offset = (float *)R_BindBufferObject(m->vertexBuffer);
	ushort * ofs = (ushort *)R_BindBufferObject(m->indexBuffer);
#else
	ushort * ofs = R_SetupVertexPointers(m);
#endif
	for (std::vector<MeshImpl::Material>::const_iterator it = m->materials.begin();
		it != m->materials.end(); ++it)
	{
		R_SetMaterial(it->ptr);
	#ifndef DONT_USE_VBO_AND_VAR
		if (EXT_draw_range_elements)
			pglDrawRangeElements(m->primitive, 0, m->numIndices, it->numIndices, GL_UNSIGNED_SHORT, ofs + it->firstIndex);
		else
			glDrawElements(m->primitive, it->numIndices, GL_UNSIGNED_SHORT, ofs + it->firstIndex);
	#else
		glBegin(m->primitive);
		for (uint i = 0; i < it->numIndices; i++)
		{
			unsigned short index = ofs[i + it->firstIndex];

			float * v = (float *)((byte *)offset + index * m->stride);
			if ((m->vertexFormat & VF_TexCoords) != 0)
			{
				glTexCoord2fv(v);
				v += 2;
			}	
			if ((m->vertexFormat & VF_Color) != 0)
			{
				glColor4ubv((byte *)v);
				v += 1;
			}	
			if ((m->vertexFormat & VF_Normal) != 0)
			{
				glNormal3fv(v);
				v += 3;
			}
			if ((m->vertexFormat & VF_Position) != 0)
			{
				glVertex3fv(v);
				v += 3;
			}	
		}
		glEnd();
	#endif
	}
#ifndef DONT_USE_VBO_AND_VAR
	R_DisableVertexPointers(m);
#endif
}

//
// Renders the specified mesh without applying the material
//
void R_RenderMeshWithoutMaterial(const MeshPtr & mesh)
{
	const MeshImpl * m = mesh.as<MeshImpl>();

	ushort * ofs = R_SetupVertexPointers(m);
	for (std::vector<MeshImpl::Material>::const_iterator it = m->materials.begin();
		it != m->materials.end(); ++it)
	{
		glDrawElements(m->primitive, it->numIndices, GL_UNSIGNED_SHORT, ofs + it->firstIndex);
	}
	R_DisableVertexPointers(m);
}

//
// Clears the cache of meshes
//
void R_ClearMeshCache()
{
	g_Meshes.clear();
}

//
// Overrides vertex shader for the mesh
//
void R_SetMeshVertexShader(MeshPtr mesh, const ShaderPtr & shader)
{
	MeshImpl * m = mesh.as<MeshImpl>();

	for (std::vector<MeshImpl::Material>::iterator it = m->materials.begin();
		it != m->materials.end(); ++it)
	{
		it->ptr->vertexShader = shader;
	}
}

/**
 * Overrides fragment shader for the mesh.
 * @param mesh Pointer to the mesh.
 * @param shader Fragment shader.
 */
void R_SetMeshFragmentShader(MeshPtr mesh, const ShaderPtr & shader)
{
	MeshImpl * m = mesh.as<MeshImpl>();

	for (std::vector<MeshImpl::Material>::iterator it = m->materials.begin();
		it != m->materials.end(); ++it)
	{
		it->ptr->fragmentShader = shader;
	}
}
