///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           ToolHelpWrp.hpp                                  
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
//  Creation Date:  26/11/2003                                  
//                                                                           
///////////////////////////////////////////////////////////////////////////////

#ifndef _TOOLHELPWRP_
#define _TOOLHELPWRP_

#include <windows.h>
#include <TlHelp32.h>
#include "IToolHelpProcessor.hpp"
#include "ToolHelpSnapeshot.hpp"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolHelpWrp
{
public:

    static BOOL ProcessProcessInfo (IN const CToolHelpSnapeshot &snapeshot,
                                    OUT IToolHelpProcessor &processor);
    static BOOL ProcessModuleInfo (IN const CToolHelpSnapeshot &snapeshot,
                                   OUT IToolHelpProcessor &processor);
    static BOOL ProcessHeapInfo (IN const CToolHelpSnapeshot &snapeshot,
                                 OUT IToolHelpProcessor &processor);
    static BOOL ProcessHeapBlockInfo (IN DWORD th32ProcessID,
                                      IN ULONG_PTR th32HeapID,
                                      OUT IToolHelpProcessor &processor);

    static BOOL ProcessThreadInfo (IN const CToolHelpSnapeshot &snapeshot,
                                   OUT IToolHelpProcessor &processor);
};

#endif //!defined _TOOLHELPWRP_
