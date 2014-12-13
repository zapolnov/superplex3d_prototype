#ifndef __CORE__STACK_TRACE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__STACK_TRACE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <vector>


/* Classes */

/** Stack trace generator. */
class StackTrace
{
public:
	/**
	 * Constructor.
	 * @param getNames If true, get function names instead of addresses.
	 */
	COREAPI StackTrace(bool getNames = false);

	/** Destructor. */
	COREAPI ~StackTrace();

	/** Gets names for functions. */
	COREAPI bool extractNames();

	/** Returns number of symbols in the backtrace. */
	inline int numSymbols() const
	{
		return m_Symbols.size();
	}

	/**
	 * Returns name of the specified symbol.
	 * @param idx Index of the symbol.
	 */
	inline QByteArray symbolName(uint idx) const
	{
		if (unlikely(!m_HaveNames || idx >= (uint)m_Symbols.size()))
			return QByteArray();
		return m_Symbols[idx].name;
	}

	/**
	 * Returns address of the specified symbol.
	 * @param idx Index of the symbol.
	 */
	inline void * symbolAddress(uint idx) const
	{
		if (unlikely(idx >= (uint)m_Symbols.size()))
			return NULL;
		return m_Symbols[idx].address;
	}

	/** Converts this stack trace to the STL string. */
	COREAPI std::string toStdString() const;

private:
	/** Symbol information. */
	struct Symbol
	{
		QByteArray name;			/**< Symbol name. */
		void * address;				/**< Symbol address. */
	};

	std::vector<Symbol> m_Symbols;	/**< Backtrace. */
	bool m_HaveNames;				/**< If true, symbol names are extracted. */

	/** Demangles function names. */
	void demangleNames();
};

#endif // __CORE__STACK_TRACE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
