/**
 * Heap manager class
 *
 * This source code is free and anyone can copy, pirate or distribute
 * the code without prior written or vocal permission.
 *
 * This software is provided "AS IS" and without any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose are disclaimed.
 *
 * Written By: Pradeep Chulliyan (chulliyan@hotmail.com)
 * Dated: June 07 2006
 *
 * http://www.codeproject.com/KB/cpp/Shared_Memory_Heap_Class.aspx
 */
#ifndef __CORE__HEAP_MANAGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__HEAP_MANAGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>


/* Classes */

/** Heap manager class for allocating and freeing blocks of memory from a pre-existing memory pool. */
class HeapManager
{
public:
	/**
	 * Constructor.
	 * @param size Size of the memory pool.
	 */
	COREAPI HeapManager(uint size);

	/** Destructor. */
	COREAPI ~HeapManager();

public:
	/**
	 * Allocates block of memory.
	 * @param len Length of the block to allocate.
	 */
	COREAPI void * alloc(size_t len);

	/**
	 * Frees previously allocated memory block.
	 * @param ptr Pointer to the block to free.
	 */
	COREAPI void free(void * ptr);

	/** Returns number of free bytes. */
	inline int getFreeSize() const
	{
		return m_Header->free;
	}	

	/** Return the allocated size. */
	inline int getAllocatedSize() const
	{
		return m_Header->alloc;
	}

	/**
	 * Returns true if specified pointer is a valid pointer within the heap.
	 * @param ptr Pointer to check validity of.
	 */
	inline bool isPointer(const void * ptr) const
	{
		byte * begin = m_Data + sizeof(HEAP_HEADER);
		byte * end = begin + m_Size;
		return ((byte *)ptr >= begin && (byte *)ptr < end);
	}

	/**
	 * Converts offset to pointer.
     * @param offset Offset of the block inside the heap.
     */
	inline void * toPointer(uint off) const
	{
		Q_ASSERT(off < m_Size);
		return (likely(off >= sizeof(HEAP_HEADER)) ? m_Data + off : NULL);
	}

	/**
     * Converts pointer to the offset.
     * @param ptr Pointer.
     */
	inline uint toOffset(const void * ptr) const
	{
		Q_ASSERT(isPointer(ptr));
		return (byte *)ptr - m_Data;
	}

private:
	/** Tree node. */
	struct TREE_NODE
	{
		uint key;				/**< Node key. */
		TREE_NODE * right;		/**< Right node. */
		TREE_NODE * left;		/**< Left node. */
	};

	/** Header of the heap. */
	struct HEAP_HEADER
	{
		TREE_NODE * len;		/**< 'Length' tree. */
		TREE_NODE * ptr;		/**< 'Pointer' tree. */
		int free;				/**< Free bytes. */
		int alloc;				/**< Allocated bytes. */
	};

	/** Header of the heap block. */
	struct HEAP_BLOCK
	{
		TREE_NODE len;			/**< Length tree node. */
		TREE_NODE ptr;			/**< Pointer tree node. */
		HEAP_BLOCK * next;		/**< Used by the 'length' tree. */
		HEAP_BLOCK * prev;		/**< Used by the 'length' tree. */
	};

	byte * m_Data;			/**< Heap memory pool. */
	size_t m_Size;			/**< Size of the memory pool. */
	HEAP_HEADER * m_Header;	/**< Heap header. */

	/**
	 * Adjust the tree (splays the heap) to get the best matching node at the top of the heap.
	 * @param root Pointer to the root of the tree.
	 * @param key Key.
	 */
	TREE_NODE * splay(TREE_NODE ** root, uint key);

	/**
	 * Inserts node into the 'length' tree.
	 * @param root Pointer to the root of the tree.
	 * @param node Pointer to the node to insert.
	 */
	void insertLenNode(TREE_NODE ** root, TREE_NODE * node);

	/**
	 * Inserts node into the tree.
	 * @param root Pointer to the root of the tree.
	 * @param node Pointer to the node to insert.
	 */
	void insertNode(TREE_NODE ** root, TREE_NODE * node);

	/**
	 * Inserts node into the tree.
	 * @param root Pointer to the root of the tree.
	 * @param node Pointer to the node to insert.
	 * @param t Pointer to the splay result.
	 */
	void insertNode(TREE_NODE ** root, TREE_NODE * node, TREE_NODE * t);

	/**
	 * Deletes node from the 'length' tree.
	 * @param root Pointer to the root of the tree.
	 * @param node Pointer to the node to delete.
	 * @param isSplayed If true, then the tree is assumed to be splayed with the correct key.
	 */
	void deleteLenNode(TREE_NODE ** root, TREE_NODE * node, bool isSplayed);

	/**
	 * Deletes node from the tree.
	 * @param root Pointer to the root of the tree.
	 * @param node Pointer to the node to delete.
	 * @param isSplayed If true, then the tree is assumed to be splayed with the correct key.
	 */
	void deleteNode(TREE_NODE ** root, TREE_NODE * node, bool isSplayed);
};

#endif // __CORE__HEAP_MANAGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
