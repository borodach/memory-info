///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           MemoryBlock.cpp
//
//  Facility:       
//
//
//  Abstract:
//
//  Environment:    VC 6.0
//
//  Author:         ������ �. �.
//
//  Creation Date:  04/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include "MemoryBlock.hpp"
#include <TlHelp32.h>

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CMemoryBlock
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

CMemoryBlock::CMemoryBlock ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CMemoryBlock
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

CMemoryBlock::CMemoryBlock (const CMemoryBlock &other)
{
    *this = other;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CMemoryBlock
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

CMemoryBlock::~CMemoryBlock ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator =
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

CMemoryBlock & CMemoryBlock::operator = (const CMemoryBlock &other)
{
    m_hHandle = other.m_hHandle;
    m_dwAddress = other.m_dwAddress;  
    m_dwFlags = other.m_dwFlags;  
    m_dwBlockSize = other.m_dwBlockSize;
    m_dwLockCount = other.m_dwLockCount;

    return *this;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator ==
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

int CMemoryBlock::operator == (const CMemoryBlock &other) const
{
    return (m_hHandle == other.m_hHandle) &&
           (m_dwAddress == other.m_dwAddress) &&
           (m_dwFlags == other.m_dwFlags) &&
           (m_dwBlockSize == other.m_dwBlockSize);
};

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RestoreGuts
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

BOOL CMemoryBlock::RestoreGuts (ifstream &is)
{
    if (! is)
    {
        return FALSE;
    }

    is.read ((char *) &m_hHandle, sizeof (m_hHandle));
    is.read ((char *) &m_dwAddress, sizeof (m_dwAddress));
    is.read ((char *) &m_dwFlags, sizeof (m_dwFlags));
    is.read ((char *) &m_dwBlockSize, sizeof (m_dwBlockSize));
    is.read ((char *) &m_dwLockCount, sizeof (m_dwLockCount));
    
    return (! is) == FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Dump
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

void CMemoryBlock::Dump (ofstream &os)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetTableHeader
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

const char * CMemoryBlock::GetTableHeader ()
{
    return NULL;
}