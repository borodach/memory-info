///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           IToolHelpProcessor
//
//  Facility:       Обработка результатов перечисления потоков, процессов
//                  модулей, куч и блоков памяти. 
//
//
//  Abstract:
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  01/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <TlHelp32.h>

#ifndef _ITOOLHELPPROCESSOR_
#define _ITOOLHELPPROCESSOR_

class IToolHelpProcessor
{
public:
    virtual BOOL Process (const HEAPENTRY32 &entry) = 0;
    virtual BOOL Process (const HEAPLIST32 &entry) = 0;
    virtual BOOL Process (const MODULEENTRY32 &entry) = 0;
    virtual BOOL Process (const PROCESSENTRY32 &entry) = 0;
    virtual BOOL Process (const THREADENTRY32 &entry) = 0;
};

class CToolHelpProcessorStub : public IToolHelpProcessor
{
public:
    virtual BOOL Process (const HEAPENTRY32 &entry)
    {
        return FALSE;
    }
    virtual BOOL Process (const HEAPLIST32 &entry)
    {
        return FALSE;
    }
    virtual BOOL Process (const MODULEENTRY32 &entry)
    {
        return FALSE;
    }
    virtual BOOL Process (const PROCESSENTRY32 &entry)
    {
        return FALSE;
    }
    virtual BOOL Process (const THREADENTRY32 &entry)
    {
        return FALSE;
    }
};

#endif//_ITOOLHELPPROCESSOR_