#include <core/ref_counted.h>
#include <core/pointer.h>

/* RefCounted methods */

//
// Destructor
//
RefCounted::~RefCounted()
{
	Q_ASSERT_MSG(m_RefCount == 0, "RefCountedObject was destroyed while it was still owned by somebody");
}
