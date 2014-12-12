#ifndef __CORE__ATOM_TABLE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__ATOM_TABLE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_vector.h>


/* Classes */

/** Table of string values. */
class AtomTable
{
public:
	/** Constructor. */
	AtomTable();

	/** Destructor. */
	~AtomTable();

	/** Returns pointer to the instance of the atom table. */
	COREAPI static const AtomTable * instance();

	/**
	 * Returns atom for the specified string.
	 * @param str String to return atom of.
	 */
	COREAPI long getAtom(const QString & str) const;

	/**
	 * Returns string for the specified atom.
	 * @param atom Atom to return string value of.
	 */
	COREAPI QString getAtomValue(long atom) const;

private:
	/** Hashing and comparison operators for hash tables. */
	class HashCompare
	{
	public:
		/**
		 * Returns hash of the string.
		 * @param str String to calculate hash of.
		 */
		COREAPI static size_t hash(const QString & str);

		/**
		 * Returns true if strings are equal.
		 * @param str1 First string.
		 * @param str2 Second string.
		 */
		static inline bool equal(const QString & str1, const QString & str2)
		{
			return str1 == str2;
		}
	};

	/** Hash table used for string to atom conversion. */
	typedef tbb::concurrent_hash_map<QString, long, HashCompare> AtomHashTable;
	/** Vector used to store atom values. */
	typedef tbb::concurrent_vector<QString> AtomValueTable;

	AtomHashTable m_StringToAtom;				/**< Table for string -> atom conversion. */
	AtomValueTable m_AtomToString;				/**< Table for atom -> string conversion. */
};

#endif // __CORE__ATOM_TABLE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
