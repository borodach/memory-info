///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           RemoteThreadSupport.h
//
//  Facility:       Поддержка работы с удаленными потоками
//
//
//  Abstract:       Объявление структуры данных, используемой для иницирования
//                  работы удаленного потока.
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  01/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <iostream.h>

#ifndef _REMOTE_THRED_SUPPORT_
#define _REMOTE_THRED_SUPPORT_

typedef DWORD (WINAPI *fRemoteThreadStartPrtoc) (LPVOID lpParameter);
typedef HMODULE (WINAPI *fnLoadLibraryA) (IN char *szLibFileName);
typedef FARPROC (WINAPI *fnGetProcAddress) (IN HMODULE hModule,
                                            IN LPCSTR lpProcName);
typedef BOOL (WINAPI *fnFreeLibrary) (IN OUT HMODULE hLibModule);

struct PacketForRemoteThread
{
    fnLoadLibraryA pfnLoadLibraryA;
    fnGetProcAddress pfnGetProcAddress;
    fnFreeLibrary pfnFreeLibrary;
    char szLibrary [512];
    char szProcName [128];
    char szOutputFile [512];

    DWORD dwExtraParam;
};

BOOL StartRemoteThreadAndWait (DWORD dwProcessID,
                               LPTHREAD_START_ROUTINE lpStartRoutine,
                               DWORD dwCodeSize,
                               const char *szLibraryPath,
                               const char *szFunctionName,
                               const char *szOutputFileName,
                               ostream &os);
BOOL StartRemoteThreadAndWait (HANDLE hProcess,
                               LPTHREAD_START_ROUTINE lpStartRoutine,
                               DWORD dwCodeSize,
                               const char *szLibraryPath,
                               const char *szFunctionName,
                               const char *szOutputFileName,
                               ostream &os);

DWORD WINAPI RemoteThreadProc (LPVOID lpParameter);
BOOL GetModuleOffset (HANDLE hProcess, LPCTSTR strModuleName, int &nOffset);
void ShowError (DWORD dwErrorCode, ostream &os);

#endif //#ifndef _REMOTE_THRED_SUPPORT_
