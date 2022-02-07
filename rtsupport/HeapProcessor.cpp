///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           HeapProcessor.hpp
//
//  Facility:       Обработка снимка куч процесса.
//
//
//  Abstract:       Класс, не используя динамическую память, сбрасывает
//                  описание куч в поток
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  01/12/2003
//
///////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "HeapProcessor.hpp"
#include "ToolHelpWrp.hpp" 
#include <Psapi.h>


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CHeapProcessor
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

CHeapProcessor::CHeapProcessor (ofstream &os) : m_pStream (&os),
                                                m_nBlockCount (0),
                                                m_nHeapCount (0),
                                                m_nBasePos (0)
{
    if (NULL != m_pStream)
    {
        char szBuf [MAX_PATH];
        szBuf [0] = 0;
        if (! GetModuleBaseNameA (GetCurrentProcess (),
                                  GetModuleHandle (NULL),
                                  szBuf,
                                  sizeof (szBuf)))
        {
            szBuf [0] = 0;
        }

        DWORD dwSize = strlen (szBuf);
        m_pStream->write ((char *) &dwSize, sizeof (dwSize));
        if (0 != dwSize)
            m_pStream->write (szBuf, dwSize);

        m_nBasePos = m_pStream->tellp ();
        m_pStream->write ((char *) &m_nHeapCount, sizeof (m_nHeapCount));
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CHeapProcessor
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

CHeapProcessor::~CHeapProcessor ()
{
    if (NULL != m_pStream)
    {
        streampos nPos = m_pStream->tellp ();
        m_pStream->seekp (m_nBasePos);
        m_pStream->write ((char *) &m_nHeapCount, sizeof (m_nHeapCount));
        m_pStream->seekp (nPos);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Process
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

BOOL CHeapProcessor::Process (const HEAPENTRY32 &entry)
{
    if (NULL == m_pStream)
        return FALSE;
    m_pStream->write ((char *) &entry.hHandle, sizeof (entry.hHandle));
    m_pStream->write ((char *) &entry.dwAddress, sizeof (entry.dwAddress));
    m_pStream->write ((char *) &entry.dwFlags, sizeof (entry.dwFlags));
    m_pStream->write ((char *) &entry.dwBlockSize, sizeof (entry.dwBlockSize));
    m_pStream->write ((char *) &entry.dwLockCount, sizeof (entry.dwLockCount));

    ++m_nBlockCount;

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Process
//
//  Description:
//
//  
//
///////////////////////////////////////////////////////////////////////////////

BOOL CHeapProcessor::Process (const HEAPLIST32 &entry)
{
    if (NULL == m_pStream)
        return FALSE;

    m_pStream->write ((char *) &entry.th32ProcessID,
                      sizeof (entry.th32ProcessID));
    m_pStream->write ((char *) &entry.th32HeapID, sizeof (entry.th32HeapID));
    m_pStream->write ((char *) &entry.dwFlags, sizeof (entry.dwFlags));
    ++m_nHeapCount;

    streampos nPos0 = m_pStream->tellp ();
    m_nBlockCount = 0;
    m_pStream->write ((char *) &m_nBlockCount, sizeof (m_nBlockCount));
    CToolHelpWrp::ProcessHeapBlockInfo (entry.th32ProcessID,
                                        entry.th32HeapID,
                                        * this);
    streampos nPos1 = m_pStream->tellp ();
    m_pStream->seekp (nPos0);
    m_pStream->write ((char *) &m_nBlockCount, sizeof (m_nBlockCount));
    m_pStream->seekp (nPos1);

    return TRUE;
}
