/**
 * \file hash.c
 * Generic hash table. 
 *
 * Used for display lists, texture objects, vertex/fragment programs,
 * buffer objects, etc.  The hash functions are thread-safe.
 * 
 * \note key=0 is illegal.
 *
 * \author Brian Paul
 */

/*
 * Mesa 3-D graphics library
 * Version:  6.5.1
 *
 * Copyright (C) 1999-2006  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "glheader.h"
#include "imports.h"
#include "hash.h"


#define TABLE_SIZE 1023  /**< Size of lookup table/array */

#define HASH_FUNC(K)  ((K) % TABLE_SIZE)


/**
 * An entry in the hash table.  
 */
struct HashEntry {
   GLuint Key;             /**< the entry's key */
   void *Data;             /**< the entry's data */
   struct HashEntry *Next; /**< pointer to next entry */
};


/**
 * The hash table data structure.  
 */
struct _mesa_HashTable {
   struct HashEntry *Table[TABLE_SIZE];  /**< the lookup table */
   GLuint MaxKey;                        /**< highest key inserted so far */
   GLboolean InDeleteAll;                /**< Debug check */
};



/**
 * Create a new hash table.
 * 
 * \return pointer to a new, empty hash table.
 */
struct _mesa_HashTable *
_mesa_NewHashTable(void)
{
   struct _mesa_HashTable *table = CALLOC_STRUCT(_mesa_HashTable);
   return table;
}



/**
 * Delete a hash table.
 * Frees each entry on the hash table and then the hash table structure itself.
 * Note that the caller should have already traversed the table and deleted
 * the objects in the table (i.e. We don't free the entries' data pointer).
 *
 * \param table the hash table to delete.
 */
void
_mesa_DeleteHashTable(struct _mesa_HashTable *table)
{
   GLuint pos;
   assert(table);
   for (pos = 0; pos < TABLE_SIZE; pos++) {
      struct HashEntry *entry = table->Table[pos];
      while (entry) {
	 struct HashEntry *next = entry->Next;
         if (entry->Data) {
            _mesa_problem(NULL,
                          "In _mesa_DeleteHashTable, found non-freed data");
         }
	 _mesa_free(entry);
	 entry = next;
      }
   }
   _mesa_free(table);
}



/**
 * Lookup an entry in the hash table.
 * 
 * \param table the hash table.
 * \param key the key.
 * 
 * \return pointer to user's data or NULL if key not in table
 */
void *
_mesa_HashLookup(const struct _mesa_HashTable *table, GLuint key)
{
   GLuint pos;
   const struct HashEntry *entry;

   assert(table);
   assert(key);

   pos = HASH_FUNC(key);
   entry = table->Table[pos];
   while (entry) {
      if (entry->Key == key) {
	 return entry->Data;
      }
      entry = entry->Next;
   }
   return NULL;
}



/**
 * Insert a key/pointer pair into the hash table.  
 * If an entry with this key already exists we'll replace the existing entry.
 * 
 * \param table the hash table.
 * \param key the key (not zero).
 * \param data pointer to user data.
 */
void
_mesa_HashInsert(struct _mesa_HashTable *table, GLuint key, void *data)
{
   /* search for existing entry with this key */
   GLuint pos;
   struct HashEntry *entry;

   assert(table);
   assert(key);

   if (key > table->MaxKey)
      table->MaxKey = key;

   pos = HASH_FUNC(key);

   /* check if replacing an existing entry with same key */
   for (entry = table->Table[pos]; entry; entry = entry->Next) {
      if (entry->Key == key) {
         /* replace entry's data */
#if 0 /* not sure this check is always valid */
         if (entry->Data) {
            _mesa_problem(NULL, "Memory leak detected in _mesa_HashInsert");
         }
#endif
	 entry->Data = data;
	 return;
      }
   }

   /* alloc and insert new table entry */
   entry = MALLOC_STRUCT(HashEntry);
   entry->Key = key;
   entry->Data = data;
   entry->Next = table->Table[pos];
   table->Table[pos] = entry;
}



/**
 * Remove an entry from the hash table.
 * 
 * \param table the hash table.
 * \param key key of entry to remove.
 *
 * While holding the hash table's lock, searches the entry with the matching
 * key and unlinks it.
 */
void
_mesa_HashRemove(struct _mesa_HashTable *table, GLuint key)
{
   GLuint pos;
   struct HashEntry *entry, *prev;

   assert(table);
   assert(key);

   /* have to check this outside of mutex lock */
   if (table->InDeleteAll) {
      _mesa_problem(NULL, "_mesa_HashRemove illegally called from "
                    "_mesa_HashDeleteAll callback function");
      return;
   }

   pos = HASH_FUNC(key);
   prev = NULL;
   entry = table->Table[pos];
   while (entry) {
      if (entry->Key == key) {
         /* found it! */
         if (prev) {
            prev->Next = entry->Next;
         }
         else {
            table->Table[pos] = entry->Next;
         }
         _mesa_free(entry);
	 return;
      }
      prev = entry;
      entry = entry->Next;
   }

}



/**
 * Delete all entries in a hash table, but don't delete the table itself.
 * Invoke the given callback function for each table entry.
 *
 * \param table  the hash table to delete
 * \param callback  the callback function
 * \param userData  arbitrary pointer to pass along to the callback
 *                  (this is typically a GLcontext pointer)
 */
void
_mesa_HashDeleteAll(struct _mesa_HashTable *table,
                    void (*callback)(GLuint key, void *data, void *userData),
                    void *userData)
{
   GLuint pos;
   ASSERT(table);
   ASSERT(callback);
   table->InDeleteAll = GL_TRUE;
   for (pos = 0; pos < TABLE_SIZE; pos++) {
      struct HashEntry *entry, *next;
      for (entry = table->Table[pos]; entry; entry = next) {
         callback(entry->Key, entry->Data, userData);
         next = entry->Next;
         _mesa_free(entry);
      }
      table->Table[pos] = NULL;
   }
   table->InDeleteAll = GL_FALSE;
}


/**
 * Walk over all entries in a hash table, calling callback function for each.
 * Note: we use a separate mutex in this function to avoid a recursive
 * locking deadlock (in case the callback calls _mesa_HashRemove()) and to
 * prevent multiple threads/contexts from getting tangled up.
 * A lock-less version of this function could be used when the table will
 * not be modified.
 * \param table  the hash table to walk
 * \param callback  the callback function
 * \param userData  arbitrary pointer to pass along to the callback
 *                  (this is typically a GLcontext pointer)
 */
void
_mesa_HashWalk(const struct _mesa_HashTable *table,
               void (*callback)(GLuint key, void *data, void *userData),
               void *userData)
{
   /* cast-away const */
//   struct _mesa_HashTable *table2 = (struct _mesa_HashTable *) table;
   GLuint pos;
   ASSERT(table);
   ASSERT(callback);
   for (pos = 0; pos < TABLE_SIZE; pos++) {
      struct HashEntry *entry, *next;
      for (entry = table->Table[pos]; entry; entry = next) {
         /* save 'next' pointer now in case the callback deletes the entry */
         next = entry->Next;
         callback(entry->Key, entry->Data, userData);
      }
   }
}


/**
 * Find a block of adjacent unused hash keys.
 * 
 * \param table the hash table.
 * \param numKeys number of keys needed.
 * 
 * \return Starting key of free block or 0 if failure.
 *
 * If there are enough free keys between the maximum key existing in the table
 * (_mesa_HashTable::MaxKey) and the maximum key possible, then simply return
 * the adjacent key. Otherwise do a full search for a free key block in the
 * allowable key range.
 */
GLuint
_mesa_HashFindFreeKeyBlock(struct _mesa_HashTable *table, GLuint numKeys)
{
   const GLuint maxKey = ~((GLuint) 0);
   if (maxKey - numKeys > table->MaxKey) {
      /* the quick solution */
      return table->MaxKey + 1;
   }
   else {
      /* the slow solution */
      GLuint freeCount = 0;
      GLuint freeStart = 1;
      GLuint key;
      for (key = 1; key != maxKey; key++) {
	 if (_mesa_HashLookup(table, key)) {
	    /* darn, this key is already in use */
	    freeCount = 0;
	    freeStart = key+1;
	 }
	 else {
	    /* this key not in use, check if we've found enough */
	    freeCount++;
	    if (freeCount == numKeys) {
	       return freeStart;
	    }
	 }
      }
      /* cannot allocate a block of numKeys consecutive keys */
      return 0;
   }
}
