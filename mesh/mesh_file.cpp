#ifdef _PG_ENABLE_QT4
 #include <core/application.h>
#endif
#include <core/exception.h>
#include <mesh/mesh_file.h>
#include <mesh/formats/wavefront_obj/obj_plugin.h>
#include <interfaces/file_system.h>
#include <core/trolltech.h>

/* MeshFile variables */

std::set<IMeshFormatPlugin *> MeshFile::m_Plugins;
tbb::spin_rw_mutex MeshFile::m_PluginsLock;
bool MeshFile::m_StdPluginsRegistered;


/* MeshFile methods */

//
// Constructor
//
MeshFile::MeshFile()
	: m_Topology(TriangleList)
{
	if (unlikely(!m_StdPluginsRegistered))
	{
		tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, true);

		if (m_StdPluginsRegistered)
			return;

		m_StdPluginsRegistered = true;
		m_Plugins.insert(new OBJPlugin);
	}
}

//
// Loads mesh from the specified file
//
void MeshFile::loadFromFile(const QString & file)
{
	QByteArray data = fileSystem()->getFileContents(file);

	MemoryFile f(file, data);
	f.open(MemoryFile::ReadOnly);

	loadFromFile(f);
}

//
// Loads mesh from the specified file.
//
void MeshFile::loadFromFile(MemoryFile & file)
{
	m_Vertices.resize(0);
	m_Indices.resize(0);

	// Get file extension
	QFileInfo info(file.fileName());
	QString ext = info.suffix();

	// Find suitable format reader
	MeshFormatReaderPtr reader;
	{
		tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, false);

		Q_ASSERT(m_StdPluginsRegistered);
		for (std::set<IMeshFormatPlugin *>::const_iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			reader = (*it)->createFileReader(ext, file);
			if (reader && reader->isValidFile())
				break;
			reader = NULL;
			if (unlikely(!file.seek(0)))
			{
				throw Exception(QObject::tr("Seek failed in file \"%1\": %2")
					.arg(file.fileName()).arg(file.errorString()));
			}
		}
	}

	// Did we find a format reader?
	if (unlikely(!reader))
		throw Exception(QObject::tr("File \"%1\" has unsupported mesh format.").arg(file.fileName()));

	reader->read(this);
}

//
// Saves mesh to the specified file
//
void MeshFile::saveToFile(const QString & file)
{
	// Get file extension
	QFileInfo info(file);
	QString ext = info.suffix();

	// Try to create the file
	QFile f(file);
	if (unlikely(!f.open(QFile::WriteOnly)))
		throw Exception(QObject::tr("Unable to create file \"%1\": %2").arg(file).arg(f.errorString()));

	// Find suitable format writer
	MeshFormatWriterPtr writer;
	{
		tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, false);

		Q_ASSERT(m_StdPluginsRegistered);
		for (std::set<IMeshFormatPlugin *>::const_iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			writer = (*it)->createFileWriter(ext, f);
			if (writer)
				break;
		}
	}

	// Did we find a format writer?
	if (unlikely(!writer))
		throw Exception(QObject::tr("Unable to write file \"%1\": unknown mesh format.").arg(file));

	writer->write(this);
}

//
// Calculates bounding sphere and bounding box for the mesh
//
void MeshFile::calcBoundingVolumes()
{
	if (m_Vertices.count() == 0 ||
		!m_Vertices.format().isAttributePresent(AttributePosition))
	{
		m_BoundingBox = AABB();
		m_BoundingSphere = Sphere();
		return;
	}

	Q_ASSERT(m_Vertices.format().attributeFormat(AttributePosition) == FormatFloat3);

	// Calculate bounding box
	Vector3 minCoord, maxCoord;
	for (int i = 0; i < (int)m_Vertices.count(); i++)
	{
		const Vector3 * coord = (const Vector3 *)m_Vertices[i];

		if (i == 0)
		{
			minCoord = *coord;
			maxCoord = *coord;
		}
		else
		{
			if (coord->x() < minCoord.x())
				minCoord.x() = coord->x();
			if (coord->y() < minCoord.y())
				minCoord.y() = coord->y();
			if (coord->z() < minCoord.z())
				minCoord.z() = coord->z();

			if (coord->x() > maxCoord.x())
				maxCoord.x() = coord->x();
			if (coord->y() > maxCoord.y())
				maxCoord.y() = coord->y();
			if (coord->z() > maxCoord.z())
				maxCoord.z() = coord->z();
		}
	}
	m_BoundingBox = AABB(minCoord, maxCoord);

	// Calculate bounding sphere
	Vector3 center = (maxCoord - minCoord) / 2.0f;
	float radius = 0.0f;
	for (int i = 0; i < (int)m_Vertices.count(); i++)
	{
		const Vector3 * coord = (const Vector3 *)m_Vertices[i];

		Vector3 v = *coord - center;
		float len = v.length2();

		if (len > radius)
			radius = len;
	}
	m_BoundingSphere = Sphere(center, sqrtf(radius));
}

//
// Registers the specified mesh format plugin
//
void MeshFile::registerMeshFormatPlugin(IMeshFormatPlugin * ptr)
{
	tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, true);
	m_Plugins.insert(ptr);
}

//
// Unregisters the specified mesh format plugin
//
void MeshFile::unregisterMeshFormatPlugin(IMeshFormatPlugin * ptr)
{
	tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, true);
	m_Plugins.erase(ptr);
}
