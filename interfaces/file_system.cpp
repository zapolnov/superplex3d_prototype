#include <interfaces/file_system.h>

/* IFileSystem variables */

IFileSystem * IFileSystem::m_Instance = NULL;


/* IFileSystem methods */

//
// Destructor
//
IFileSystem::~IFileSystem()
{
	Q_ASSERT(m_Instance);
	m_Instance = NULL;
}
