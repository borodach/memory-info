///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           ToolHelpSnapeshot.hpp                                  
//
//  Facility:       Работа со снимком TOOLHELP 
//                                                                           
//                                                                           
//  Abstract:       Обертка для дескриптора снимка TOOLHELP
//                                                                           
//  Environment:    VC 6.0                                                   
//                                                                           
//  Author:         Зайцев С. А.                                                          
//                                                                           
//  Creation Date:  26/11/2003                                  
//                                                                           
///////////////////////////////////////////////////////////////////////////////


#ifndef _TOOLHELPSNAPESHOT_
#define _TOOLHELPSNAPESHOT_

#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolHelpSnapeshot
{
public:
    CToolHelpSnapeshot (IN DWORD dwFlags = 0, IN DWORD th32ProcessID = 0);
    CToolHelpSnapeshot (const CToolHelpSnapeshot &other)
    {
        m_hSnapeshotHandle = INVALID_HANDLE_VALUE;
    }

    CToolHelpSnapeshot & operator = (const CToolHelpSnapeshot &other)
    {
        Reset ();

        return *this;
    }

    virtual ~CToolHelpSnapeshot ();

    BOOL Reset ();
    BOOL Init (IN DWORD dwFlags, IN DWORD th32ProcessID);
    BOOL IsValid () const
    {
        return INVALID_HANDLE_VALUE != m_hSnapeshotHandle;
    }
    operator HANDLE () const
    {
        return m_hSnapeshotHandle;
    }
protected:
    HANDLE m_hSnapeshotHandle;
};

#endif // !defined _TOOLHELPSNAPESHOT_
