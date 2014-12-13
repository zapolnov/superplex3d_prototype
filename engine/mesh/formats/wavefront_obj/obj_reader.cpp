#include <core/exception.h>
#include <core/trolltech.h>
#include <core/logger.h>
#include "obj_reader.h"
#include "obj_loader.h"

/* OBJReader methods */

//
// Constructor
//
OBJReader::OBJReader(const QString & ext, MemoryFile & file)
	: MeshFormatReaderHelper(file)
{
	m_IsValid = (ext == "obj");
}

//
// Reads the mesh
//
void OBJReader::read(MeshFile * mesh)
{
	ModelOBJ model;

	// Load the model
	if (unlikely(!model.import(m_File)))
	{
		throw Exception(QObject::tr("Unable to load Wavefront OBJ model file \"%1\": %2")
			.arg(m_File.fileName()).arg(m_File.errorString()));
	}
	model.normalize();

	// Set the primitive topology
	setMeshPrimitiveTopology(mesh, TriangleList);

	// Setup the vertex format
	VertexFormat format;
	if (model.hasPositions())
		format.setAttributeFormat(AttributePosition, FormatFloat3);
	if (model.hasTextureCoords())
		format.setAttributeFormat(AttributeTexCoord, FormatFloat2);
	if (model.hasNormals())
		format.setAttributeFormat(AttributeNormal, FormatFloat3);
	setMeshVertexFormat(mesh, format);
	meshVertices(mesh).setVertexFormat(format);

	// Clear the vertex and index buffers
	meshVertices(mesh).resize(0);
	meshIndices(mesh).resize(0);

	// Preallocate memory
	meshVertices(mesh).reserve(model.getNumberOfVertices());
	meshIndices(mesh).reserve(model.getNumberOfIndices());

	// Setup the vertex buffer
	const ModelOBJ::Vertex * vertices = model.getVertexBuffer();
	for (int i = 0; i < model.getNumberOfVertices(); i++)
	{
		char data[64];
		char * p = data;

		// Copy vertex position
		if (format.isAttributePresent(AttributePosition))
		{
			memcpy(p, vertices[i].position, 3 * sizeof(float));
			p += 3 * sizeof(float);
		}

		// Copy vertex normal
		if (format.isAttributePresent(AttributeNormal))
		{
			memcpy(p, vertices[i].normal, 3 * sizeof(float));
			p += 3 * sizeof(float);
		}

		// Copy vertex texture coordinates
		if (format.isAttributePresent(AttributeTexCoord))
		{
			memcpy(p, vertices[i].texCoord, 2 * sizeof(float));
			p += 2 * sizeof(float);
		}

		meshVertices(mesh).addVertex(data);
	}

	// Setup the index buffer
	const int * indices = model.getIndexBuffer();
	for (int i = 0; i < model.getNumberOfIndices(); i++)
		meshIndices(mesh).addIndex(*indices++);

	// Setup materials information
	for (int i = 0; i < model.getNumberOfMeshes(); i++)
	{
		const ModelOBJ::Mesh & mat = model.getMesh(i);

		MeshFile::Submaterial m;
		m.firstIndex = mat.startIndex;
		m.numIndices = mat.triangleCount * 3;

		m.material = new MeshMaterial;
		m.material->setName(QString::fromLatin1(mat.pMaterial->name.c_str()));
		m.material->setDiffuseTexture(QString::fromLatin1(mat.pMaterial->colorMapFilename.c_str()));
		m.material->setNormalMap(QString::fromLatin1(mat.pMaterial->bumpMapFilename.c_str()));
		m.material->setAmbientColor(Vector4(mat.pMaterial->ambient[0], mat.pMaterial->ambient[1],
			mat.pMaterial->ambient[2], mat.pMaterial->ambient[3]));
		m.material->setDiffuseColor(Vector4(mat.pMaterial->diffuse[0], mat.pMaterial->diffuse[1],
			mat.pMaterial->diffuse[2], mat.pMaterial->diffuse[3]));
		m.material->setSpecularColor(Vector4(mat.pMaterial->specular[0], mat.pMaterial->specular[1],
			mat.pMaterial->specular[2], mat.pMaterial->specular[3]));
		m.material->setShininess(mat.pMaterial->shininess);

		addMeshSubmaterial(mesh, m);
	}

	// Calculate bounding volumes
	mesh->calcBoundingVolumes();

	// Log the info
	logger << LOG_INFO << "%s: Wavefront OBJ; %d vertices, %d indices, %d material%s" << m_File.fileName()
		<< model.getNumberOfVertices() << model.getNumberOfIndices()
		<< model.getNumberOfMeshes() << (model.getNumberOfMeshes() == 1 ? "" : "s");
}
