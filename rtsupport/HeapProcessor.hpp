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

#include "IToolHelpProcessor.hpp"
#include <fstream.h>

#ifndef _HEAPPROCESSOR_
#define _HEAPPROCESSOR_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHeapProcessor : public CToolHelpProcessorStub
{
public:
    CHeapProcessor (ofstream &os);
    virtual ~CHeapProcessor ();

    virtual BOOL Process (const HEAPENTRY32 &entry);
    virtual BOOL Process (const HEAPLIST32 &entry);
protected:
    ofstream *m_pStream;
    int m_nBlockCount;
    int m_nHeapCount;
    streampos m_nBasePos;
};

#endif // _HEAPPROCESSOR_
