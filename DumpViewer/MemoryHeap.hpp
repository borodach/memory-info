///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           MemoryHeap.h
//
//  Facility:       
//
//
//  Abstract:
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  04/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <fstream.h>
#include <vector>

#include "MemoryBlock.hpp"

#ifndef _MEMORY_HEAP_
#define _MEMORY_HEAP_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMemoryHeap  
{
public:
    typedef std::vector <CMemoryBlock> BlockContainer;
	CMemoryHeap ();
    CMemoryHeap (const CMemoryHeap &other);
	virtual ~CMemoryHeap ();

    void Reset ();

    void Dump (ofstream &os);
	BOOL RestoreGuts (ifstream &is);
    static const char* GetTableHeader ();
	
    
    CMemoryHeap& operator = (const CMemoryHeap &other);
    int operator == (const CMemoryHeap &other) const;

    DWORD GetProcessID () const
    {
        return m_dwProcessID;
    }
    DWORD GetHeapID () const
    {
        return m_dwHeapID;
    }
    DWORD GetFlags () const
    {
        return m_dwFlags;
    }
    
protected:
    DWORD m_dwProcessID;
    DWORD m_dwHeapID;
    DWORD m_dwFlags;

    BlockContainer m_Blocks;
};

#endif // _MEMORY_HEAP_
