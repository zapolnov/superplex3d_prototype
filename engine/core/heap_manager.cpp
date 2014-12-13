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
 */
#include <core/heap_manager.h>
#include <string.h>

/* HeapManager methods */

//
// Constructor
//
HeapManager::HeapManager(uint size)
    : m_Size(size + sizeof(HEAP_HEADER) + sizeof(HEAP_BLOCK))
{
	m_Data = new byte[m_Size];
	m_Header = reinterpret_cast<HEAP_HEADER *>(m_Data);

	memset(m_Header, 0, sizeof(HEAP_HEADER));
	m_Header->free = size;

	HEAP_BLOCK * blk = reinterpret_cast<HEAP_BLOCK *>(&m_Header[1]);
	blk->ptr.key = toOffset(blk);
	blk->len.key = (uint)m_Header->free;

	insertLenNode(&m_Header->len, &blk->len);
	insertNode(&m_Header->ptr, &blk->ptr);
}

//
// Destructor
//
HeapManager::~HeapManager()
{
}

//
// Allocates block of memory
//
void * HeapManager::alloc(size_t len)
{
	if (unlikely(len == 0))
		return 0;

	if (len > m_Header->free - sizeof(HEAP_BLOCK))
		return NULL;

	// Splay the 'length' tree to obtain the best match
	TREE_NODE * node = splay(&m_Header->len, (uint)len);
	bool doSplay = true;
	if (node && node->key < (uint)len)
	{
		doSplay = false;
		node = node->right;
		if (node != NULL)
		{
			while (node->left)
				node = node->left;
		}
	}

	// If we cannot retrieve any tree node, return '0' to indicate error.
	if (!node)
		return NULL;

	// 'len' node is the first attribute of the HEAP_BLOCK, therefore every 'len' node is also a HEAP_BLOCK.
	HEAP_BLOCK * blk = reinterpret_cast<HEAP_BLOCK *>(node);
	int tln = len + sizeof(HEAP_BLOCK);
	int rem = (int)node->key - tln;
	byte * ptr = (byte *)&blk[1];

	// Remove current block from the trees
	deleteLenNode(&m_Header->len, node, doSplay);
	deleteNode(&m_Header->ptr, &blk->ptr, false);

	// If the block is larger than the requested size, split the block and insert the second block into the trees
	if (rem == 0)
		tln = (int)node->key;
	else
	{
		HEAP_BLOCK * sec = reinterpret_cast<HEAP_BLOCK *>(&ptr[len]);

		blk->len.key = (uint)len;
		sec->len.key = (uint)rem;
		sec->ptr.key = toOffset(sec);

		// Insert the second block into the trees
		insertLenNode(&m_Header->len, &sec->len);
		insertNode(&m_Header->ptr, &sec->ptr);
	}

	// Adjust the allocation size
	m_Header->free -= tln;
	m_Header->alloc += tln;

	// Clear attributes and return the pointer
	blk->len.right = blk->ptr.right = NULL;
	blk->len.left = blk->ptr.left = NULL;
	blk->next = blk->prev = NULL;

	// Clear trailing 'pad'.
	ptr[len - 1] = 0;

	return ptr;
}

//
// Frees previously allocated memory block
//
void HeapManager::free(void * ptr)
{
	return;
	Q_ASSERT(ptr);
	Q_ASSERT(isPointer(ptr));

	HEAP_BLOCK * blk = static_cast<HEAP_BLOCK *>(ptr) - 1;
	HEAP_BLOCK * next = reinterpret_cast<HEAP_BLOCK *>((byte *)&blk[1] + blk->len.key);

    // Check for memory corruption
#ifdef _DEBUG
	byte * tpad = ((byte *)ptr + blk->len.key - 1);
	Q_ASSERT_MSG(!*tpad, "Heap corruption detected");
#endif

	// Check if the pointer has already been freed or not
#ifdef _DEBUG
	TREE_NODE * tmp = splay(&m_Header->ptr, blk->ptr.key);
	Q_ASSERT_MSG(!tmp || tmp->key != blk->ptr.key, "Double free detected");
#endif

	// Adjust the free bytes
	m_Header->free += blk->len.key;
	m_Header->alloc -= blk->len.key;

	TREE_NODE * node = NULL, * pred = NULL;
	if (isPointer(next))
		node = splay(&m_Header->ptr, next->ptr.key);

	if (node)
	{
		// Find a predecessor
		if (node->key < next->ptr.key)
            pred = node;
		else
		{
			pred = node->left;
			if (pred > 0)
			{
				while (pred->right)
					pred = pred->right;
			}
		}

		// See if the block can be merged with the sucessor
		if (node->key == next->ptr.key)
		{
			// Remove the successor from both trees
			deleteLenNode(&m_Header->len, &next->len, false);
			deleteNode(&m_Header->ptr, &next->ptr, true); 

			// Adjust the size
			blk->len.key += (next->len.key + sizeof(HEAP_BLOCK));
			m_Header->free += sizeof(HEAP_BLOCK);
		}

		// See if the block can be merged with the predecessor
		if (pred)
		{
			HEAP_BLOCK * b = static_cast<HEAP_BLOCK *>(toPointer(pred->key));
			HEAP_BLOCK * t = reinterpret_cast<HEAP_BLOCK *>((byte *)&b[1] + b->len.key);

			// Merge with the predecessor
			if (t->ptr.key == blk->ptr.key)
			{
				// Remove the predecessor from the 'len' tree
				deleteLenNode(&m_Header->len, &b->len, false);

				// Adjust the size
				b->len.key += (blk->len.key + sizeof(HEAP_BLOCK));
				m_Header->free += (int)sizeof(HEAP_BLOCK);
				m_Header->alloc -= (int)sizeof(HEAP_BLOCK);

				// Re-insert the node in 'len' tree
				insertLenNode(&m_Header->len, &b->len);

				blk = NULL; // We don't have to insert the node
			}
		}
	}

	// Insert the node
	if (blk)
	{
		insertLenNode(&m_Header->len, &blk->len);
		insertNode(&m_Header->ptr, &blk->ptr);
	}
}

//
// Adjust the tree (splays the heap) to get the best matching node at the top of the heap
//
HeapManager::TREE_NODE * HeapManager::splay(TREE_NODE ** root, uint key)
{
	TREE_NODE * t = *root;

	if (!t)
		return 0;

	TREE_NODE n = { 0 };
	TREE_NODE * l = &n;
	TREE_NODE * r = l;
	TREE_NODE * y = 0;

	for (;;)
	{
		if (key < t->key)
		{
			if (!t->left)
				break;

			// Rotate right
			if (key < t->left->key)
			{
				y = t->left;
				t->left = y->right;
				y->right = t;
				t = y;

				if (!t->left)
					break;
			}

			// Link right
			r->left = t;
			r = t;
			t = t->left;
		}
		else if (key > t->key)
		{
			if (!t->right) 
				break;

			// Rotate left
			if (key > t->right->key)
			{
				y = t->right;
				t->right = y->left;
				y->left = t;
				t = y;

				if (!t->right)
					break;
			}

			// Link left
			l->right = t;
			l = t;
			t = t->right;
		}
		else
			break;
	}

	// Re-create the node
	l->right = r->left = NULL;
	l->right = t->left;
	r->left = t->right;
	t->left = n.right;
	t->right = n.left;

	*root = t;

	return *root;
}

//
// Inserts node into the 'length' tree
//
void HeapManager::insertLenNode(TREE_NODE ** root, TREE_NODE * node)
{
	// There can be many 'len' nodes with the same key. Therefore, we have to 
	// build a linked list of 'len' nodes if a node already exists in the tree.
	// 'len' TREE_NODE is the first entry in the 'HEAP_BLOCK'. Therefore,
	// we can safely typecast TREE_NODE to a HEAP_BLOCK.
	HEAP_BLOCK * t = reinterpret_cast<HEAP_BLOCK *>(splay(root, node->key));
	HEAP_BLOCK * p = reinterpret_cast<HEAP_BLOCK *>(node);

	if (!t || t->len.key != node->key)
	{
		// Insert into the tree
		p->prev = p->next = NULL;
		insertNode(root, node, reinterpret_cast<TREE_NODE *>(t));
	}
	else
	{
		// Link to the existing tree node
		p->next = t->next;
		p->prev = t;
		t->next = p;

		if (p->next)
			static_cast<HEAP_BLOCK *>(p->next)->prev = p;
	}
}

//
// Inserts node into the tree
//
void HeapManager::insertNode(TREE_NODE ** root, TREE_NODE * node)
{
	insertNode(root, node, splay(root, node->key));
}

//
// Inserts node into the tree
//
void HeapManager::insertNode(TREE_NODE ** root, TREE_NODE * node, TREE_NODE * t)
{
	if (!t)
	{
		node->left = NULL;
		node->right = NULL;
	}
	else if (node->key < t->key)
	{
		node->left = t->left;
		node->right = t;
		t->left = NULL;
	}
	else
	{
		node->right = t->right;
		node->left = t;
		t->right = NULL;
	}

	*root = node;
}

//
// Deletes node from the 'length' tree
//
void HeapManager::deleteLenNode(TREE_NODE ** root, TREE_NODE * node, bool isSplayed)
{
	// There can be more than one length node with the same key (or memory
	// blocks with the same length). This method will look for the matching node
	// in the tree and later walk the linked list to delete the correct node.
	HEAP_BLOCK * p = reinterpret_cast<HEAP_BLOCK *>(node);

	// If the node 'node' is not the first node in the linked list,
	// simply de-link 'node' from the linked list.
	if (p->prev)
	{
		static_cast<HEAP_BLOCK *>(p->prev)->next = p->next;
		if (p->next)
			static_cast<HEAP_BLOCK *>(p->next)->prev = p->prev;
	}
	else
	{
		// If the node 'node' is the first node of the linked list, then
		// remove it from the tree and add the next node from the linked
		// list to the tree.
		if (p->next)
		{
			HEAP_BLOCK * t = static_cast<HEAP_BLOCK *>(p->next);
			TREE_NODE * x = *root;

			x = isSplayed ? node : splay(&x, node->key);
			if (x && x->key == node->key)
			{
				t->len.left = x->left;
				t->len.right = x->right;
				t->prev = NULL;
				*root = reinterpret_cast<TREE_NODE *>(p->next);
			}
		}
		else
			deleteNode(root, node, false);
	}
}

//
// Deletes node from the tree.
//
void HeapManager::deleteNode(TREE_NODE ** root, TREE_NODE * node, bool isSplayed)
{
	TREE_NODE * x = *root;

	x = isSplayed ? node : splay(&x, node->key);
	if (x->key == node->key)
	{
		if (!node->left)
            x = node->right;
		else
		{
			x = splay(&node->left, node->key);
			x->right = node->right;
		}
	}
	*root = x;
}
