#include <engine/render.h>
#include <core/exception.h>
#include <core/logger.h>
#include <mesh/mesh_file.h>
#include <GLFW/glfw3.h>
#include <engine/r_opengl.h>
#include <map>
#include <vector>

/* Classes */

/** Buffer in the video memory. */
class BufferObjectImpl : public BufferObject
{
public:
	/**
	 * Constructor.
	 * @param usage Buffer object usage (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER).
	 */
	BufferObjectImpl(GLenum usage)
		: m_Target(usage)
	{
		Q_ASSERT(usage == GL_ARRAY_BUFFER || usage == GL_ELEMENT_ARRAY_BUFFER);
	}

	/**
	 * Binds the buffer.
	 * @returns pointer to be used in array binding functions.
	 */
	virtual void * bind() const = 0;

	/** Unbinds the buffer. */
	virtual void unbind() const = 0;

	/** Returns size of the buffer in bytes. */
	virtual uint size() const = 0;

	/**
	 * Maps the buffer and returns pointer to the data.
	 * @param access Access mode.
	 */
	virtual void * mapBuffer(GLenum access) = 0;

	/** Unmaps the buffer. */
	virtual void unmapBuffer() = 0;

	/**
	 * Sets buffer data.
	 * @param data Pointer to the data.
	 * @param size Size of the data in bytes.
	 * @param usage Data usage pattern.
	 */
	virtual void setData(const void * data, GLsizeiptrARB size, GLenum usage) = 0;

protected:
	GLenum m_Target;	/**< Buffer object usage. */
};

/** Vertex Arrays implementation. */
class VAR : public BufferObjectImpl
{
public:
	/**
	 * Constructor.
	 * @param usage Buffer object usage (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER).
	 */
	VAR(GLenum usage)
		: BufferObjectImpl(usage), m_Allocated(1), m_Size(0)
	{
		m_Data = new char[1];
	}

	/** Destructor. */
	~VAR()
	{
		delete[] m_Data;
	}

	/**
	 * Binds the buffer.
	 * @returns pointer to be used in array binding functions.
	 */
	void * bind() const
	{
		m_Current[m_Target == GL_ARRAY_BUFFER ? 0 : 1] = const_cast<VAR *>(this);
		return m_Data;
	}

	/** Unbinds the buffer. */
	void unbind() const
	{
		Q_ASSERT(isBound());
		m_Current[m_Target == GL_ARRAY_BUFFER ? 0 : 1] = NULL;
	}

	/** Returns true if this buffer is currently bound. */
	bool isBound() const
	{
		return m_Current[m_Target == GL_ARRAY_BUFFER ? 0 : 1] == this;
	}

	/** Returns size of the buffer in bytes. */
	uint size() const
	{
		return m_Size;
	}

	/**
	 * Maps the buffer and returns pointer to the data.
	 * @param access Access mode.
	 */
	void * mapBuffer(GLenum access)
	{
		Q_ASSERT(access == GL_READ_ONLY || access == GL_WRITE_ONLY || access == GL_READ_WRITE);
		Q_ASSERT(isBound());
		return m_Data;
	}

	/** Unmaps the buffer. */
	void unmapBuffer()
	{
		Q_ASSERT(isBound());
	}

	/**
	 * Sets buffer data.
	 * @param data Pointer to the data.
	 * @param size Size of the data in bytes.
	 * @param usage Data usage pattern.
	 */
	void setData(const void * data, GLsizeiptrARB size, GLenum usage)
	{
		Q_ASSERT(isBound());

		if (size > (GLsizeiptr)m_Allocated)
		{
			delete[] m_Data;
			m_Data = new char[size];
			m_Allocated = size;
		}

		if (data)
			memcpy(m_Data, data, size);

		m_Size = size;
	}

private:
	static VAR * m_Current[2];		/**< Pointer to the currently bound buffer. */
	char * m_Data;					/**< Pointer to the data. */
	uint m_Allocated;				/**< Size of the buffer. */
	uint m_Size;					/**< Size of the data in the buffer. */
};

VAR * VAR::m_Current[2] = { NULL, NULL };

/** VBO implementation. */
class VBO : public BufferObjectImpl
{
public:
	/**
	 * Constructor.
	 * @param usage Buffer object usage (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER).
	 */
	VBO(GLenum usage)
		: BufferObjectImpl(usage), m_Size(0)
	{
		pglGenBuffers(1, &m_BufferID);
	}

	/** Destructor. */
	~VBO()
	{
		pglDeleteBuffers(1, &m_BufferID);
	}

	/**
	 * Binds the buffer.
	 * @returns pointer to be used in array binding functions.
	 */
	void * bind() const
	{
		pglBindBuffer(m_Target, m_BufferID);
		return NULL;
	}

	/** Unbinds the buffer. */
	void unbind() const
	{
		Q_ASSERT(isBound());
		pglBindBuffer(m_Target, 0);
	}

	/** Returns true if this buffer is currently bound. */
	bool isBound() const
	{
		GLint bufferID;

		switch (m_Target)
		{
		case GL_ARRAY_BUFFER:
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bufferID);
			break;
		case GL_ELEMENT_ARRAY_BUFFER:
			glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bufferID);
			break;
		default:
			Q_ASSERT_MSG(false, "Unsupported target");
			return false;
		}

		return (GLuint)bufferID == m_BufferID;
	}

	/** Returns size of the buffer in bytes. */
	uint size() const
	{
		return m_Size;
	}

	/**
	 * Maps the buffer and returns pointer to the data.
	 * @param access Access mode.
	 */
	void * mapBuffer(GLenum access)
	{
		Q_ASSERT(access == GL_READ_ONLY || access == GL_WRITE_ONLY || access == GL_READ_WRITE);
		Q_ASSERT(isBound());
		return pglMapBuffer(m_Target, access);
	}

	/** Unmaps the buffer. */
	void unmapBuffer()
	{
		Q_ASSERT(isBound());
		pglUnmapBuffer(m_Target);
	}

	/**
	 * Sets buffer data.
	 * @param data Pointer to the data.
	 * @param size Size of the data.
	 * @param usage Data usage pattern.
	 */
	void setData(const void * data, GLsizeiptrARB size, GLenum usage)
	{
		Q_ASSERT(isBound());
		pglBufferData(m_Target, size, data, usage);
		m_Size = size;
	}

private:
	GLuint m_BufferID;		/**< ID of the VBO. */
	uint m_Size;			/**< Current size of the buffer in bytes. */
};


/* Public functions */

//
// Creates the dynamic vertex buffer object
//
BufferObjectPtr R_CreateVertexBufferObject()
{
#ifndef DONT_USE_VBO_AND_VAR
	if (likely(ARB_vertex_buffer_object))
		return new VBO(GL_ARRAY_BUFFER);
	else
#endif
		return new VAR(GL_ARRAY_BUFFER);
}

//
// Creates the dynamic index buffer object
//
BufferObjectPtr R_CreateIndexBufferObject()
{
#ifndef DONT_USE_VBO_AND_VAR
	if (likely(ARB_vertex_buffer_object))
		return new VBO(GL_ELEMENT_ARRAY_BUFFER);
	else
#endif
		return new VAR(GL_ELEMENT_ARRAY_BUFFER);
}

//
// Locks the specified buffer object
//
void * R_LockBufferObject(const BufferObjectPtr & ptr, AccessMode access)
{
	Q_ASSERT(ptr);

	GLenum mode;
	switch (access)
	{
	case ReadAccess:
		mode = GL_READ_ONLY;
		break;
	case WriteAccess:
		mode = GL_WRITE_ONLY;
		break;
	case ReadWriteAccess:
		mode = GL_READ_WRITE;
		break;
	default:
		Q_ASSERT(false);
		return NULL;
	}

	const BufferObjectImpl * impl = ptr.as<BufferObjectImpl>();
	impl->bind();

	return const_cast<BufferObjectImpl *>(impl)->mapBuffer(mode);
}

//
// Unlocks the specified buffer object
//
void R_UnlockBufferObject(const BufferObjectPtr & ptr)
{
	Q_ASSERT(ptr);

	const BufferObjectImpl * impl = ptr.as<BufferObjectImpl>();
	impl->bind();
	const_cast<BufferObjectImpl *>(impl)->unmapBuffer();
}

//
// Binds the specified buffer object
//
void * R_BindBufferObject(const BufferObjectPtr & ptr)
{
	Q_ASSERT(ptr);
	return ptr.as<BufferObjectImpl>()->bind();
}

//
// Unbinds the specified buffer object
//
void R_UnbindBufferObject(const BufferObjectPtr & ptr)
{
	Q_ASSERT(ptr);
	ptr.as<BufferObjectImpl>()->unbind();
}

//
// Uploads data to the buffer object
//
void R_SetBufferObjectData(const BufferObjectPtr & ptr, const void * data, size_t size, UsageMode usage)
{
	Q_ASSERT(ptr);

	GLenum mode;
	switch (usage)
	{
	case StaticDraw:
		mode = GL_STATIC_DRAW;
		break;
	case DynamicDraw:
		mode = GL_DYNAMIC_DRAW;
		break;
	case StreamDraw:
		mode = GL_STREAM_DRAW;
		break;
	default:
		Q_ASSERT(false);
		return;
	}

	const BufferObjectImpl * impl = ptr.as<BufferObjectImpl>();
	const_cast<BufferObjectImpl *>(impl)->setData(data, size, mode);
}
