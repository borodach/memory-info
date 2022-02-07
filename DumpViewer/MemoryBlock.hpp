///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           MemoryBlock.hpp
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

#ifndef _MEMORY_BLOCK_
#define _MEMORY_BLOCK_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMemoryBlock  
{
public:
	void Dump (ofstream &os);
	BOOL RestoreGuts (ifstream &is);
    static const char* GetTableHeader ();
	CMemoryBlock();
    CMemoryBlock(const CMemoryBlock &other);
	virtual ~CMemoryBlock ();

    CMemoryBlock& operator = (const CMemoryBlock &other);
    int operator == (const CMemoryBlock &other) const;

    HANDLE GetHandle () const
    {
        return m_hHandle;
    }
    DWORD GetAddress () const
    {
        return m_dwAddress;
    }
    DWORD GetFlags () const
    {
        return m_dwFlags;
    }
    SIZE_T GetBlockSize () const
    {
        return m_dwBlockSize;
    }
    DWORD GetLockCount () const
    {
        return m_dwLockCount;
    }

protected:
    HANDLE m_hHandle;
    DWORD m_dwAddress;
    DWORD m_dwFlags;
    SIZE_T m_dwBlockSize;
    DWORD m_dwLockCount;
};

#endif // _MEMORY_BLOCK_
