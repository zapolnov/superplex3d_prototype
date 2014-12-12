#ifndef __OPENGL__DISPLAY_LIST_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __OPENGL__DISPLAY_LIST_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <opengl/context.h>


/* Classes */

/** Display list object. */
class GLDisplayList
{
public:
	/** Constructor. */
	inline GLDisplayList()
		: m_Created(false), m_Handle(0)
	{
	}

	/** Destructor. */
	inline ~GLDisplayList()
	{
		if (m_Created)
			glDeleteLists(m_Handle, 1);
	}

	/**
	 * Creates the display list if it is not created yet.
	 * @returns true if list was created or false it it was already created at the time this function
	 * was called.
	 */
	inline bool createDisplayList()
	{
		if (m_Created)
			return false;
	}

	/**
	 * Begins compilation of the list.
	 * @param exec If true, also execute the commands.
     */
	inline void beginCompileDisplayList(bool exec = true)
	{
		glNewList(m_Handle, exec ? GL_COMPILE_AND_EXECUTE : GL_COMPILE);
	}

	/** Ends compilation of the list. */
	inline void endCompileDisplayList()
	{
		glEndList();
	}

	/**
	 * Executes the display list.
	 * @returns true on success, false if display list needs to be created.
	 */
	inline bool executeDisplayList()
	{
		if (unlikely(!m_Created))
		{
			m_Handle = glGenLists(1);
			m_Created = true;
			return false;
		}

		glCallList(m_Handle);
		return true;
	}

private:
	bool m_Created;			/**< True if display list has been created. */
	GLuint m_Handle;		/**< Handle of the display list. */
};

#endif // __OPENGL__DISPLAY_LIST_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
