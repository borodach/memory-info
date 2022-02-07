///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           ToolHelpWrp.cpp
//
//  Facility:       Получение информации о процессах, потоках, памяти и модулях                                                         
//                                                                           
//                                                                           
//  Abstract:       Класс-обертка для работы с TOOLHELP API
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  27/11/2003
//
///////////////////////////////////////////////////////////////////////////////

#include "ToolHelpWrp.hpp"

#define SNAPESHOT_COPY_TEMPLATE(ENTRTYPE, GetFirst_, GetNext_)\
    ENTRTYPE entry;                                           \
    memset (&entry, 0, sizeof (entry));                       \
    entry.dwSize = sizeof (ENTRTYPE);                         \
                                                              \
    if ( ! GetFirst_ (snapeshot, &entry)  &&                  \
        (GetLastError () == ERROR_NO_MORE_FILES)) {           \
                                                              \
        return TRUE;                                          \
    }                                                         \
    if ( !processor.Process (entry) ) {                       \
        return TRUE;                                          \
    }                                                         \
    entry.dwSize = sizeof (ENTRTYPE);                         \
    while (GetNext_ (snapeshot, &entry))                      \
    {                                                         \
        if ( !processor.Process (entry) ) {                   \
            return TRUE;                                      \
        }                                                     \
        entry.dwSize = sizeof (ENTRTYPE);                     \
    }                                                         \
                                                              \
    return GetLastError () == ERROR_NO_MORE_FILES;


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ProcessProcessInfo
//
//  Description:
//
//  Копирует в storage информацию о процессах.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpWrp::ProcessProcessInfo (IN const CToolHelpSnapeshot &snapeshot,
                                       OUT IToolHelpProcessor &processor)
{
    SNAPESHOT_COPY_TEMPLATE (PROCESSENTRY32, Process32First, Process32Next)
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ProcessProcessModuleInfo
//
//  Description:
//
//  Копирует в storage информацию о модулях процессов. Для успешной работы
//  snapeshot должен быть создан с флагом TH32CS_SNAPMODULE.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpWrp::ProcessModuleInfo (IN const CToolHelpSnapeshot &snapeshot,
                                      OUT IToolHelpProcessor &processor)
{
    SNAPESHOT_COPY_TEMPLATE (MODULEENTRY32, Module32First, Module32Next)
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ProcessProcessHeapInfo
//
//  Description:
//
//  Копирует в storage информацию о кучах процессов. Для успешной работы
//  snapeshot должен быть создан с флагом TH32CS_SNAPHEAPLIST.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpWrp::ProcessHeapInfo (IN const CToolHelpSnapeshot &snapeshot,
                                    OUT IToolHelpProcessor &processor)
{
    SNAPESHOT_COPY_TEMPLATE (HEAPLIST32, Heap32ListFirst, Heap32ListNext)
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ProcessHeapBlockInfo
//
//  Description:
//
//  Копирует в storage информацию о блоках памяти в кучах процесса. Для 
//  успешной работы snapeshot должен быть создан с флагом TH32CS_SNAPHEAPLIST
//  th32ProcessID - toolhelp идентификатор процесса (не системный)
//  th32HeapID - идентификатор кучи (из HEAPLIST32).
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpWrp::ProcessHeapBlockInfo (IN DWORD th32ProcessID,
                                         IN ULONG_PTR th32HeapID,
                                         OUT IToolHelpProcessor &processor)
{
    HEAPENTRY32 entry;                                          
    memset (&entry, 0, sizeof (entry));                      
    entry.dwSize = sizeof (HEAPENTRY32);                        

    if (! Heap32First (&entry, th32ProcessID, th32HeapID))
    {
        return  GetLastError () == ERROR_NO_MORE_FILES;
    }

    if (!processor.Process (entry))
    {
        return TRUE;
    }

    entry.dwSize = sizeof (HEAPENTRY32);                        
    while (TRUE == Heap32Next (&entry))
    {
        if (!processor.Process (entry))
        {
            return TRUE;
        }
        entry.dwSize = sizeof (HEAPENTRY32);
    }  

    return GetLastError () == ERROR_NO_MORE_FILES;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ProcessThreadInfo
//
//  Description:
//
//  Копирует в storage информацию о потоках.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpWrp::ProcessThreadInfo (IN const CToolHelpSnapeshot &snapeshot,
                                      OUT IToolHelpProcessor &processor)
{
    SNAPESHOT_COPY_TEMPLATE (THREADENTRY32, Thread32First, Thread32Next)
}
