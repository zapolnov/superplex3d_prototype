#ifndef __CORE__ATOM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__ATOM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/atom_table.h>
#include <core/trolltech.h>


/* Classes */

/** String atom. */
class Atom
{
public:
	/** Default constructor. */
	inline Atom()
		: m_Value(-1)
	{
	}

	/**
	 * Constructor.
	 * @param str String.
	 */
	inline Atom(const QString & str)
		: m_Value(AtomTable::instance()->getAtom(str))
	{
	}

	/**
	 * Copy constructor.
	 * @param src Reference to the source atom.
	 */
	inline Atom(const Atom & src)
		: m_Value(src.m_Value)
	{
	}

	/**
	 * Assignment operator.
	 * @param src Reference to the source atom.
	 */
	inline Atom & operator=(const Atom & src)
	{
		m_Value = src.m_Value;
		return *this;
	}

	/**
	 * Compares this atom with the specified one.
	 * @param atom Atom to compare with.
	 */
	inline bool operator==(const Atom & atom) const
	{
		return m_Value == atom.m_Value;
	}

	/**
	 * Compares this atom with the specified one.
	 * @param atom Atom to compare with.
	 */
	inline bool operator!=(const Atom & atom) const
	{
		return m_Value != atom.m_Value;
	}

	/**
	 * Compares this atom with the specified one.
	 * @param atom Atom to compare with.
	 */
	inline bool operator<(const Atom & atom) const
	{
		return m_Value < atom.m_Value;
	}

	/**
	 * Compares this atom with the specified one.
	 * @param atom Atom to compare with.
	 */
	inline bool operator<=(const Atom & atom) const
	{
		return m_Value <= atom.m_Value;
	}

	/**
	 * Compares this atom with the specified one.
	 * @param atom Atom to compare with.
	 */
	inline bool operator>(const Atom & atom) const
	{
		return m_Value > atom.m_Value;
	}

	/**
	 * Compares this atom with the specified one.
	 * @param atom Atom to compare with.
	 */
	inline bool operator>=(const Atom & atom) const
	{
		return m_Value >= atom.m_Value;
	}

	/** Returns integer value of the atom. */
	inline operator long () const
	{
		Q_ASSERT(isValid());
		return m_Value;
	}

	/** Returns string value of the atom. */
	inline QString toString() const
	{
		Q_ASSERT(isValid());
		return AtomTable::instance()->getAtomValue(m_Value);
	}

	/** Returns string value of the atom. */
	inline operator QString () const
	{
		return toString();
	}

	/** Returns true if this atom is valid. */
	inline bool isValid() const
	{
		return m_Value >= 0;
	}

private:
	long m_Value;			/**< Value of the atom. */
};

#endif // __CORE__ATOM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
