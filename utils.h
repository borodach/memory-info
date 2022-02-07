///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           utils.h
//
//  Facility:       Поддержка раьботы со снимком toolhelp
//
//
//  Abstract:       Объявление методов для вывода данных Toolhelp.
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  01/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include "ToolhelpWrp.hpp"
#include <iostream.h>

#ifndef _TOOLHELP_UTRILS_
#define _TOOLHELP_UTRILS_

ostream &operator << (ostream &os, const PROCESSENTRY32 &process);
ostream &operator << (ostream &os, const HEAPLIST32 &heap);
ostream &operator << (ostream &os, const HEAPENTRY32 &block);

#endif //_TOOLHELP_UTRILS_