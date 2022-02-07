///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           ToolHelpWrp.cpp
//
//  Facility:       ��������� ���������� � ���������, �������, ������ � �������                                                         
//                                                                           
//                                                                           
//  Abstract:       �����-������� ��� ������ � TOOLHELP API
//
//  Environment:    VC 6.0
//
//  Author:         ������ �. �.
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
//  �������� � storage ���������� � ���������.
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
//  �������� � storage ���������� � ������� ���������. ��� �������� ������
//  snapeshot ������ ���� ������ � ������ TH32CS_SNAPMODULE.
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
//  �������� � storage ���������� � ����� ���������. ��� �������� ������
//  snapeshot ������ ���� ������ � ������ TH32CS_SNAPHEAPLIST.
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
//  �������� � storage ���������� � ������ ������ � ����� ��������. ��� 
//  �������� ������ snapeshot ������ ���� ������ � ������ TH32CS_SNAPHEAPLIST
//  th32ProcessID - toolhelp ������������� �������� (�� ���������)
//  th32HeapID - ������������� ���� (�� HEAPLIST32).
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
//  �������� � storage ���������� � �������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpWrp::ProcessThreadInfo (IN const CToolHelpSnapeshot &snapeshot,
                                      OUT IToolHelpProcessor &processor)
{
    SNAPESHOT_COPY_TEMPLATE (THREADENTRY32, Thread32First, Thread32Next)
}
