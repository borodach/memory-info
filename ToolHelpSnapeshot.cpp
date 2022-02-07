///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           ToolHelpSnapeshot.cpp                                  
//
//  Facility:       ������ �� ������� TOOLHELP 
//                                                                           
//                                                                           
//  Abstract:       ������� ��� ����������� ������ TOOLHELP
//                                                                           
//  Environment:    VC 6.0                                                   
//                                                                           
//  Author:         ������ �. �.                                                          
//                                                                           
//  Creation Date:  26/11/2003                                  
//                                                                           
///////////////////////////////////////////////////////////////////////////////

#include "ToolHelpSnapeshot.hpp"
#include <TlHelp32.h>


///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:       CToolHelpSnapeshot
//                                                                           
//  Description:      
//                                                       
//  ����������� ������.                                                                         
//                                                                           
///////////////////////////////////////////////////////////////////////////////

CToolHelpSnapeshot::CToolHelpSnapeshot (IN DWORD dwFlags/* = 0*/,
                                        IN DWORD th32ProcessID/* = 0*/) : m_hSnapeshotHandle (INVALID_HANDLE_VALUE)
{
    Init (dwFlags, th32ProcessID);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:       ~CToolHelpSnapeshot                                                        
//                                                                           
//  Description:                                                             
// 
//  ����������.                                                                         
//                                                                           
///////////////////////////////////////////////////////////////////////////////

CToolHelpSnapeshot::~CToolHelpSnapeshot ()
{
    Reset ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Init
//
//  Description:
//
//  ��������� ������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpSnapeshot::Init (IN DWORD dwFlags, IN DWORD th32ProcessID)
{
    Reset ();
    m_hSnapeshotHandle = CreateToolhelp32Snapshot (dwFlags, th32ProcessID);

    return IsValid ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Reset
//
//  Description:
//
//  �������� ������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CToolHelpSnapeshot::Reset ()
{
    BOOL bResult = TRUE;
    if (IsValid ())
    {
        bResult = CloseHandle (m_hSnapeshotHandle);
        m_hSnapeshotHandle = NULL;
    }

    return bResult;
}
