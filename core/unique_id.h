#ifndef __CORE__UNIQUE_ID_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__UNIQUE_ID_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <tbb/concurrent_hash_map.h>
#include <tbb/atomic.h>


NAMESPACE_INTERNAL_BEGIN

/* Types */

/** Hash of unique IDs. */
typedef tbb::concurrent_hash_map<const char *, uint> IdentifierHash;


/* Variables */

/** Hash of unique IDs. */
COREAPI extern IdentifierHash g_IDs;

NAMESPACE_INTERNAL_END


/* Functions */

/** Generates unique ID. */
template <class TYPE> static inline uint allocUniqueID(TYPE * p)
{
	const char * name = typeid(p).name();

	Internal::IdentifierHash::accessor accessor;
	Internal::g_IDs.insert(accessor, name);

	uint id = accessor->second;
	accessor->second = id + 1;

	return id;
}

#endif // __CORE__UNIQUE_ID_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
