#include <interfaces/file_system.h>

/* IFileSystem variables */

#ifndef _PG_ENABLE_QT4
IFileSystem * IFileSystem::m_Instance = NULL;
#endif


/* IFileSystem methods */

//
// Destructor
//
IFileSystem::~IFileSystem()
{
#ifndef _PG_ENABLE_QT4
	Q_ASSERT(m_Instance);
	m_Instance = NULL;
#endif // _PG_ENABLE_QT4
}
