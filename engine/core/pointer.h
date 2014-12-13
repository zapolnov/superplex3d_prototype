#ifndef __CORE__POINTER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__POINTER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <tbb/atomic.h>


/* Classes */

/** Shared pointer to the reference-counted object. */
template <class T> class Pointer
{
public:
	/** Constructor. */
	inline Pointer()
	{
		m_Object = NULL;
	}

	/**
	 * Constructor from the given pointer.
	 * @param obj Pointer to the object.
	 */
	inline Pointer(T * obj)
	{
		m_Object = NULL;
		assign(obj);
	}

	/**
	 * Copy constructor.
	 * @param src Reference to the source handle.
	 */
	inline Pointer(const Pointer & src)
	{
		m_Object = NULL;
		assign(src.m_Object);
	}

	/** Destructor. */
	inline ~Pointer()
	{
		assign(NULL);
	}

	/**
	 * Assignment operator.
	 * @param src Reference to the source handle.
	 */
	inline Pointer & operator=(const Pointer & src)
	{
		assign(src.m_Object);
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param obj Pointer to the object.
	 */
	inline Pointer & operator=(T * obj)
	{
		assign(obj);
		return *this;
	}

	/** Releases the holded object. */
	inline void release()
	{
		assign(NULL);
	}

	/** Provides access to the stored pointer. */
	inline T * get()
	{
		return this->as<T>();
	}

	/** Provides access to the stored pointer. */
	inline const T * get() const
	{
		return this->as<T>();
	}

	/** Provides access to the stored pointer. */
	inline T * operator->()
	{
		Q_ASSERT(m_Object);
		return m_Object;
	}

	/** Provides access to the stored pointer. */
	inline const T * operator->() const
	{
		Q_ASSERT(m_Object);
		return m_Object;
	}

	/** Casts stored pointer to the specified type. */
	template <class T2> inline T2 * as()
	{
		return safe_cast<T2 *>((RefCounted *)m_Object);
	}

	/** Casts stored pointer to the specified type. */
	template <class T2> inline const T2 * as() const
	{
		return safe_cast<const T2 *>((const RefCounted *)m_Object);
	}

	/** Casts stored pointer to the specified type. */
	template <class T2> inline T2 * dynamicCast()
	{
		return dynamic_cast<T2 *>((RefCounted *)m_Object);
	}

	/** Casts stored pointer to the specified type. */
	template <class T2> inline const T2 * dynamicCast() const
	{
		return dynamic_cast<const T2 *>((const RefCounted *)m_Object);
	}

	/** Returns true if pointer is NULL. */
	inline bool operator!() const
	{
		return !m_Object;
	}

	/** Returns true if pointer is not NULL. */
	inline operator bool() const
	{
		return m_Object != NULL;
	}

	/**
	 * Returns true if stored pointer is equivalent to the specified one.
	 * @param b pointer to compare with.
	 */
	inline bool operator==(const Pointer & b) const
	{
		return m_Object == b.m_Object;
	}

	/**
	 * Returns true if stored pointer is equivalent to the specified one.
	 * @param b pointer to compare with.
	 */
	inline bool operator==(const T * b) const
	{
		return m_Object == b;
	}

	/**
	 * Returns true if stored pointer is not equivalent to the specified one.
	 * @param b pointer to compare with.
	 */
	inline bool operator!=(const Pointer & b) const
	{
		return m_Object != b.m_Object;
	}

	/**
	 * Returns true if stored pointer is not equivalent to the specified one.
	 * @param b pointer to compare with.
	 */
	inline bool operator!=(const T * b) const
	{
		return m_Object != b;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator<(const Pointer & b) const
	{
		return m_Object < b.m_Object;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator<(const T * b) const
	{
		return m_Object < b;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator<=(const Pointer & b) const
	{
		return m_Object <= b.m_Object;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator<=(const T * b) const
	{
		return m_Object <= b;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator>(const Pointer & b) const
	{
		return m_Object > b.m_Object;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator>(const T * b) const
	{
		return m_Object > b;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator>=(const Pointer & b) const
	{
		return m_Object >= b.m_Object;
	}

	/**
	 * Comparison operator.
	 * @param b pointer to compare with.
	 */
	inline bool operator>=(const T * b) const
	{
		return m_Object >= b;
	}

private:
	/** Pointer to the object. */
	tbb::atomic<T *> m_Object;

	/**
	 * Assigns new pointer.
	 * @param ptr New pointer.
	 */
	void assign(T * ptr)
	{
		if (ptr)
			ptr->addRef();

		T * old = m_Object.fetch_and_store(ptr);

		if (old)
			old->release();
	}
};

#endif // __CORE__POINTER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
