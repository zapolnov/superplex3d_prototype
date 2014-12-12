/*
    Copyright 2005-2009 Intel Corporation.  All Rights Reserved.

    This file is part of Threading Building Blocks.

    Threading Building Blocks is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.

    Threading Building Blocks is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Threading Building Blocks; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
*/

//We works on windows only
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE 1

#include <windows.h>
#include <new>
#include "tbb_function_replacement.h"

inline UINT_PTR Ptr2Addrint(LPVOID ptr)
{
    Int2Ptr i2p;
    i2p.lpv = ptr;
    return i2p.uip;
}

inline LPVOID Addrint2Ptr(UINT_PTR ptr)
{
    Int2Ptr i2p;
    i2p.uip = ptr;
    return i2p.lpv;
}

// We need to remember which pointer was allocated from myHeap and which
// was allocated before replacing malloc (therefore allocated from the 
// CRT heap).
// Keep page database, since entire pages belong to the same heap, just
// remember the pages.
#define PAGE_SIZE 0x1000

// Use this value as the maximum size the trampoline region
const int MAX_PROBE_SIZE = 16;

// The size of a jump relative instruction "e9 00 00 00 00"
const int SIZE_OF_RELJUMP = 5;

// The size of jump RIP relative indirect "ff 25 00 00 00 00"
const int SIZE_OF_INDJUMP = 6;

// The size of address we put in the location (in Intel64)
const int SIZE_OF_ADDRESS = 8;

// The max distance covered in 32 bits
const __int64 MAX_DISTANCE = (((__int64)1 << 31) - 1);

// The maximum number of distinct buffers in memory
const int MAX_NUM_BUFFERS = 256;

// Is the distance between addr1 and addr2 smaller than dist
inline BOOL IsInDistance(UINT_PTR addr1, UINT_PTR addr2, __int64 dist)
{
    __int64 diff = addr1 - addr2;
    if (diff < 0) diff = -diff;
    return (diff < dist);
}

/*
 * When inserting a probe in 64 bits process the distance between the insertion
 * point and the target may be bigger than 2^32. In this case we are using 
 * indirect jump through memory where the offset to this memory location
 * is smaller than 2^32 and it contains the absolute address (8 bytes).
 *
 * This class is used to hold the pages used for the above trampolines.
 * Since this utility will be used to replace malloc functions this implementation
 * doesn't allocate memory dynamically.
 *
 * The struct MemoryBuffer holds the data about a page in the memory used for
 * replacing functions in Intel64 where the target is too far to be replaced
 * with a short jump. All the calculations of m_base and m_next are in a multiple
 * of SIZE_OF_ADDRESS (which is 8 in Win64).
 */
class MemoryProvider {
private:
    struct MemoryBuffer {
        UINT_PTR m_base;    // base address of the buffer
        UINT_PTR m_next;    // next free location in the buffer
        DWORD    m_size;    // size of buffer

        // Default constructor
        MemoryBuffer() : m_base(0), m_next(0), m_size(0) {}

        // Constructor
        MemoryBuffer(void *base, DWORD size)
        {
            m_base = Ptr2Addrint(base);
            m_next = m_base;
            m_size = size;
        }
    };

MemoryBuffer *CreateBuffer(UINT_PTR addr)
    {
        // No more room in the pages database
        if (m_lastBuffer - m_pages == MAX_NUM_BUFFERS)
            return 0;

        void *newAddr = Addrint2Ptr(addr);
        // Get information for the region which the given address belongs to
        MEMORY_BASIC_INFORMATION memInfo;
        if (VirtualQuery(newAddr, &memInfo, sizeof(memInfo)) != sizeof(memInfo))
            return 0;

        for(;;) {
            // The new address to check is beyond the current region and aligned to allocation size
            newAddr = Addrint2Ptr( (Ptr2Addrint(memInfo.BaseAddress) + memInfo.RegionSize + m_allocSize) & ~(UINT_PTR)(m_allocSize-1) );

            // Check that the address is in the right distance.
            // VirtualAlloc can only round the address down; so it will remain in the right distance
            if (!IsInDistance(addr, Ptr2Addrint(newAddr), MAX_DISTANCE))
                break;

            if (VirtualQuery(newAddr, &memInfo, sizeof(memInfo)) != sizeof(memInfo))
                break;

            if (memInfo.State == MEM_FREE && memInfo.RegionSize >= m_allocSize)
            {
                // Found a free region, try to allocate a page in this region
                void *newPage = VirtualAlloc(newAddr, m_allocSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
                if (!newPage)
                    break;

                // Add the new page to the pages database
                MemoryBuffer *pBuff = new (m_lastBuffer) MemoryBuffer(newPage, m_allocSize);
                ++m_lastBuffer;
                return pBuff;
            }
        }

        // Failed to find a buffer in the distance
        return 0;
    }

public:
    MemoryProvider() 
    { 
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        m_allocSize = sysInfo.dwAllocationGranularity; 
        m_lastBuffer = &m_pages[0];
    }

    // We can't free the pages in the destructor because the trampolines
    // are using these memory locations and a replaced function might be called
    // after the destructor was called.
    ~MemoryProvider() 
    {
    }

    // Return a memory location in distance less than 2^31 from input address 
    UINT_PTR GetLocation(UINT_PTR addr)
    {
        MemoryBuffer *pBuff = m_pages;
        for (; pBuff<m_lastBuffer && IsInDistance(pBuff->m_base, addr, MAX_DISTANCE); ++pBuff)
        {
            if (pBuff->m_next < pBuff->m_base + pBuff->m_size)
            {
                UINT_PTR loc = pBuff->m_next;
                pBuff->m_next += SIZE_OF_ADDRESS;
                return loc;
            }
        }

        pBuff = CreateBuffer(addr);
        if(!pBuff)
            return 0;

        UINT_PTR loc = pBuff->m_next;
        pBuff->m_next += SIZE_OF_ADDRESS;
        return loc;
    }

private:
    MemoryBuffer m_pages[MAX_NUM_BUFFERS];
    MemoryBuffer *m_lastBuffer;
    DWORD m_allocSize;
};

static MemoryProvider memProvider;

// Insert jump relative instruction to the input address
// RETURN: the size of the trampoline or 0 on failure
static DWORD InsertTrampoline32(void *inpAddr, void *targetAddr)
{
    UINT_PTR srcAddr = Ptr2Addrint(inpAddr);
    UINT_PTR tgtAddr = Ptr2Addrint(targetAddr);
    UINT_PTR offset = tgtAddr - srcAddr - SIZE_OF_RELJUMP;
    UINT offset32 = (UINT)(offset & 0xFFFFFFFF);
    UCHAR *codePtr = (UCHAR *)inpAddr;

    // Check that the target fits in 32 bits
    if (!IsInDistance(srcAddr, tgtAddr, MAX_DISTANCE))
        return 0;

    // Fill the buffer
    *codePtr++ = 0xE9;
    memcpy(codePtr, &offset32, sizeof(offset32));
    return SIZE_OF_RELJUMP;
}


// This function is called when the offset doesn't fit in 32 bits
// 1  Find and allocate a page in the small distance (<2^31) from input address
// 2  Put jump RIP relative indirect through the address in the close page
// 3  Put the absolute address of the target in the allocated location
// RETURN: the size of the trampoline or 0 on failure
static DWORD InsertTrampoline64(void *inpAddr, void *targetAddr)
{
    UINT_PTR srcAddr = Ptr2Addrint(inpAddr);
    UINT_PTR tgtAddr = Ptr2Addrint(targetAddr);

    // Get a location close to the source address
    UINT_PTR location = memProvider.GetLocation(srcAddr);
    if (!location)
        return 0;

    UINT_PTR offset = location - srcAddr - SIZE_OF_INDJUMP;
    UINT offset32 = (INT)(offset & 0xFFFFFFFF);

    // Fill the location
    UINT_PTR *locPtr = (UINT_PTR *)Addrint2Ptr(location);
    *locPtr = tgtAddr;

    // Fill the buffer
    UCHAR *codePtr = (UCHAR *)inpAddr;
    *codePtr++ = 0xFF;
    *codePtr++ = 0x25;
    memcpy(codePtr, &offset32, sizeof(offset32));
    return SIZE_OF_INDJUMP;
}


// Insert a jump instruction in the inpAddr to the targetAddr
// 1. Get the memory protection of the page containing the input address
// 2. Change the memory protection to writable
// 3. Call InsertTrampoline32 or InsertTrampoline64
// 4. Restore memory protection
// RETURN: FALSE on failure, TRUE on success
static BOOL InsertTrampoline(void *inpAddr, void *targetAddr)
{
    DWORD probeSize;

    // Change page protection to EXECUTE+WRITE
    DWORD origProt = 0;
    if (!VirtualProtect(inpAddr, MAX_PROBE_SIZE, PAGE_EXECUTE_WRITECOPY, &origProt))
        return FALSE;

    probeSize = InsertTrampoline32(inpAddr, targetAddr);
    if (!probeSize)
        probeSize = InsertTrampoline64(inpAddr, targetAddr);

    // Restore original protection
    VirtualProtect(inpAddr, MAX_PROBE_SIZE, origProt, &origProt);

    if (!probeSize)
        return FALSE;

    FlushInstructionCache(GetCurrentProcess(), inpAddr, probeSize);
    return TRUE;
}

FRR_TYPE ReplaceFunctionA(const char *dllName, const char *funcName, FUNCPTR newFunc)
{
    // Cache the results of the last search for the module
    // Assume that there was no DLL unload between 
    static char cachedName[MAX_PATH+1];
    static HMODULE cachedHM = 0;

    if (!dllName || !*dllName)
        return FRR_NODLL;

    if (!cachedHM || strncmp(dllName, cachedName, MAX_PATH) != 0)
    {
        // Find the module handle for the input dll
        HMODULE hModule = GetModuleHandleA(dllName);
        if (hModule == 0)
        {
            // Couldn't find the module with the input name
            cachedHM = 0;
            return FRR_NODLL;
        }

        cachedHM = hModule;
        strncpy(cachedName, dllName, MAX_PATH);
    }

    FARPROC inpFunc = GetProcAddress(cachedHM, funcName);
    if (inpFunc == 0)
    {
        // Function was not found
        return FRR_NOFUNC;
    }

    if (!InsertTrampoline((void*)inpFunc, (void*)newFunc))
    {
        // Failed to insert the trampoline to the target address
        return FRR_FAILED;
    }

    return FRR_OK;
}

FRR_TYPE ReplaceFunctionW(const wchar_t *dllName, const char *funcName, FUNCPTR newFunc)
{
    // Cache the results of the last search for the module
    // Assume that there was no DLL unload between 
    static wchar_t cachedName[MAX_PATH+1];
    static HMODULE cachedHM = 0;

    if (!dllName || !*dllName)
        return FRR_NODLL;

    if (!cachedHM || wcsncmp(dllName, cachedName, MAX_PATH) != 0)
    {
        // Find the module handle for the input dll
        HMODULE hModule = GetModuleHandleW(dllName);
        if (hModule == 0)
        {
            // Couldn't find the module with the input name
            cachedHM = 0;
            return FRR_NODLL;
        }

        cachedHM = hModule;
        wcsncpy(cachedName, dllName, MAX_PATH);
    }

    FARPROC inpFunc = GetProcAddress(cachedHM, funcName);
    if (inpFunc == 0)
    {
        // Function was not found
        return FRR_NOFUNC;
    }

    if (!InsertTrampoline((void*)inpFunc, (void*)newFunc))
    {
        // Failed to insert the trampoline to the target address
        return FRR_FAILED;
    }

    return FRR_OK;
}

#endif //_WIN32
