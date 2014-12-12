#include <core/atom_table.h>
#include <core/atom.h>

// Hash function (c) Copyright 2004-2008 by Paul Hsieh 
// See http://www.azillionmonkeys.com/qed/hash.html

/* Macros */

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
 #define get16bits(d) (*((const uint16 *)(d)))
#endif

#if !defined (get16bits)
 #define get16bits(d) ((((uint32)(((const uint8 *)(d))[1])) << 8) \
                       + (uint32)(((const uint8 *)(d))[0]))
#endif


/* Private variables */

static AtomTable g_AtomTable;	/**< Instance of the atom table. */


/* AtomTable::HashCompare methods */

//
// Returns hash of the string
//
size_t AtomTable::HashCompare::hash(const QString & str)
{
	uint32 len = str.length(), hash = len * sizeof(QChar), tmp;
	const char * data = (const char *)str.constData();
	int rem;

	if (unlikely(len <= 0))
		return 0;

	rem = len & 3;
	len >>= 2;

	// Main loop
	for (; len > 0; len--)
	{
		hash += get16bits(data);
		tmp = (get16bits(data + 2) << 11) ^ hash;
		hash = (hash << 16) ^ tmp;
		data += 2 * sizeof (uint16);
		hash += hash >> 11;
	}

	// Handle end cases
	switch (rem)
	{
	case 3:
		hash += get16bits(data);
		hash ^= hash << 16;
		hash ^= data[sizeof(uint16)] << 18;
		hash += hash >> 11;
		break;

	case 2:
		hash += get16bits(data);
		hash ^= hash << 11;
		hash += hash >> 17;
		break;

	case 1:
		hash += *data;
		hash ^= hash << 10;
		hash += hash >> 1;
		break;
	}

	// Force "avalanching" of final 127 bits
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}


/* AtomTable methods */

//
// Constructor
//
AtomTable::AtomTable()
{
}

//
// Destructor
//
AtomTable::~AtomTable()
{
}

//
// Returns pointer to the instance of the atom table
//
const AtomTable * AtomTable::instance()
{
	return &g_AtomTable;
}

//
// Returns atom for the specified string
//
long AtomTable::getAtom(const QString & str) const
{
	AtomHashTable::accessor it;

	if (const_cast<AtomTable *>(this)->m_StringToAtom.insert(it, str))
	{
		AtomValueTable::iterator jt = const_cast<AtomTable *>(this)->m_AtomToString.push_back(str);
		it->second = jt - m_AtomToString.begin();
	}

	return it->second;
}

//
// Returns string for the specified atom
//
QString AtomTable::getAtomValue(long atom) const
{
	Q_ASSERT(atom >= 0);
	Q_ASSERT(atom < (long)m_AtomToString.size());

	return m_AtomToString[atom];
}
