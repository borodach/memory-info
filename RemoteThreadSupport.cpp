///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           RemoteThreadSupport.cpp
//
//  Facility:       Запуск потока в контексте другого процесса.
//
//
//  Abstract:       Модуль позволячет выполнить произвольный код в контексте
//                  заданного процесса
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  30/11/2003
//
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <Psapi.h>
#include <tchar.h>

#include "RemoteThreadSupport.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ShowError
//
//  Description:
//
//  Выводит описание и код ошибки в поток os.
//
///////////////////////////////////////////////////////////////////////////////

void ShowError (DWORD dwErrorCode, ostream &os)
{
    LPVOID lpMsgBuf;
    FormatMessageA (FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    dwErrorCode,
                    MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPSTR) & lpMsgBuf,
                    0,
                    NULL);
    os << "Error #" << dwErrorCode << ": " << (LPCSTR) lpMsgBuf << endl;
    LocalFree (lpMsgBuf);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RemoteThreadProc
//
//  Description:
//
//  Загружает библиотеку "rtsupport.dll" и выполняет в ней функцию
//  "EntryPoint". RemoteThreadProc выполняется в контексте другого процесса,
//   поэтому в ней не используется сегмент данных.
//
///////////////////////////////////////////////////////////////////////////////

DWORD WINAPI RemoteThreadProc (LPVOID lpParameter)
{
    PacketForRemoteThread *pPacket = (PacketForRemoteThread *) lpParameter;
    DWORD dwResult = - 1;

    //
    //Загружаем библиотеку и выполняем основную функцию.
    //

    HMODULE hLib = pPacket->pfnLoadLibraryA (pPacket->szLibrary);
    if (hLib != NULL)
    {
        fRemoteThreadStartPrtoc pRemoteThreadStartPrtoc;
        pRemoteThreadStartPrtoc = (fRemoteThreadStartPrtoc)
                                  pPacket->pfnGetProcAddress (hLib,
                                                              pPacket->szProcName);
        if (NULL != pRemoteThreadStartPrtoc)
        {
            dwResult = pRemoteThreadStartPrtoc (lpParameter);
        }

        pPacket->pfnFreeLibrary (hLib);
    }

    return dwResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetModuleOffset
//
//  Description:
//
//  Вычисляет смещение точки загрузки модуля strModuleName в процессе hProcess 
// относительно точки загрузки этого же модуля в текущем процессе.
//
///////////////////////////////////////////////////////////////////////////////

BOOL GetModuleOffset (HANDLE hProcess, LPCTSTR strModuleName, int &nOffset)
{
    nOffset = 0;
    HMODULE hKernel = GetModuleHandle (strModuleName);
    if (NULL == hKernel)
    {
        return FALSE;
    }
    MODULEINFO kernelMI;
    if (! GetModuleInformation (GetCurrentProcess (),
                                hKernel,
                                & kernelMI,
                                sizeof (kernelMI)))
    {
        return FALSE;
    }                                        

    HMODULE modules [4096];
    DWORD dwSize = 0;
    if (! EnumProcessModules (hProcess, modules, sizeof (modules), & dwSize))
    {
        return FALSE;
    }

    TCHAR buffer [1024];
    for (int nIdx = 0; dwSize > 0; dwSize -= sizeof (HMODULE), ++nIdx)
    {
        if (! GetModuleBaseName (hProcess,
                                 modules [nIdx],
                                 buffer,
                                 sizeof (buffer) / sizeof (buffer [0])))
        {
            continue;
        }
        if (_tcsicmp (strModuleName, buffer) != 0)
        {
            continue;
        }

        MODULEINFO mi;
        if (GetModuleInformation (hProcess, modules [nIdx], & mi, sizeof (mi)))
        {
            nOffset = (char *) mi.EntryPoint - (char *) kernelMI.EntryPoint;
            return TRUE;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       StartRemoteThreadAndWait
//
//  Description:
//
//  Создает в адресном пространстве заданного процесса копию функции 
//  RemoteThreadProc и выполняет ее в контексте этого процесса (создается
//  отдельный поток).
//
///////////////////////////////////////////////////////////////////////////////

BOOL StartRemoteThreadAndWait (HANDLE hProcess,
                               LPTHREAD_START_ROUTINE lpStartRoutine,
                               DWORD dwCodeSize,
                               const char *szLibraryPath,
                               const char *szFunctionName,
                               const char *szOutputFileName,
                               ostream &os)
{
    class ResourcesDeallocator
    {
    public:
        ResourcesDeallocator (HANDLE hProcess) : hThread (NULL),
                                                 m_hProcess (hProcess),
                                                 lpAddress (NULL)
        {
        }
        ~ResourcesDeallocator ()
        {
            if (NULL != hThread)
                CloseHandle (hThread);
            if (NULL != lpAddress)
                VirtualFreeEx (m_hProcess, lpAddress, 0, MEM_RELEASE);
        }

        HANDLE hThread;
        HANDLE m_hProcess;
        LPTHREAD_START_ROUTINE lpAddress;
    } rd (hProcess);

    if (NULL != hProcess)
    {
        LPVOID lpStart = lpStartRoutine;
        PacketForRemoteThread packet;

        int nOffset = 0;
        if (! GetModuleOffset (hProcess, TEXT ("kernel32.dll"), nOffset))
        {
            return FALSE;
        }
        HMODULE hKernel = GetModuleHandle (TEXT ("kernel32.dll"));
        if (NULL == hKernel)
        {
            os << "Can not get mogule handle for kernel32.dll. ";
            ShowError (GetLastError (), os);

            return FALSE;
        }
        packet.pfnLoadLibraryA = (fnLoadLibraryA)
                                 ((char *)
                                  GetProcAddress (hKernel, "LoadLibraryA") +
                                  nOffset);
        packet.pfnGetProcAddress = (fnGetProcAddress)
                                   ((char *)
                                    GetProcAddress (hKernel, "GetProcAddress") +
                                    nOffset);
        packet.pfnFreeLibrary = (fnFreeLibrary)
                                ((char *)
                                 GetProcAddress (hKernel, "FreeLibrary") +
                                 nOffset);
        strcpy (packet.szLibrary, szLibraryPath);
        strcpy (packet.szProcName, szFunctionName);
        strcpy (packet.szOutputFile, szOutputFileName);

        //
        //Выделяем память в другом процессе
        //

        rd.lpAddress = (LPTHREAD_START_ROUTINE)
                       VirtualAllocEx (hProcess,
                                       NULL,
                                       dwCodeSize +
                                       sizeof (PacketForRemoteThread),
                                       MEM_COMMIT,
                                       PAGE_EXECUTE_READWRITE);

        if (NULL != rd.lpAddress)
        {
            if (WriteProcessMemory (hProcess,
                                    rd.lpAddress,
                                    lpStart,
                                    dwCodeSize,
                                    NULL) &&
                WriteProcessMemory (hProcess,
                                    (char *)
                                                                 rd.lpAddress +
                                                                 dwCodeSize,
                                    & packet,
                                    sizeof (PacketForRemoteThread),
                                    NULL))
            {
                DWORD dwThreadID;
                rd.hThread = CreateRemoteThread (hProcess,
                                                 NULL,
                                                 0,
                                                 rd.lpAddress,
                                                 ((char *) rd.lpAddress) +
                                                 dwCodeSize,
                                                 0,
                                                 & dwThreadID);
                if (NULL != rd.hThread)
                {
                    BOOL bResult = (WAIT_OBJECT_0 ==
                                    WaitForSingleObject (rd.hThread,
                                                         INFINITE));
                    if (! bResult)
                    {
                        os <<
                        "Can not wait until the end of the remote thread. ";
                        ShowError (GetLastError (), os);
                    }

                    return bResult;
                } else
                {
                    os << "Can not create remote thread. ";
                    ShowError (GetLastError (), os);
                }
            } else
            {
                os << "Can not copy data into remote process memory. ";
                ShowError (GetLastError (), os);
            }
        } else
        {
            os <<
            "Can not allocate memory within the virtual address space of a specified process. ";
            ShowError (GetLastError (), os);
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       StartRemoteThreadAndWait
//
//  Description:
//
//  Создает в адресном пространстве заданного процесса копию функции 
//  RemoteThreadProc и выполняет ее в контексте этого процесса (создается
//  отдельный поток).
//
///////////////////////////////////////////////////////////////////////////////

BOOL StartRemoteThreadAndWait (DWORD dwProcessID,
                               LPTHREAD_START_ROUTINE lpStartRoutine,
                               DWORD dwCodeSize,
                               const char *szLibraryPath,
                               const char *szFunctionName,
                               const char *szOutputFileName,
                               ostream &os)
{
    class ResourcesDeallocator
    {
    public:
        ResourcesDeallocator () : hProcess (NULL)
        {
        }
        ~ResourcesDeallocator ()
        {
            if (NULL != hProcess)
                CloseHandle (hProcess);
        }

        HANDLE hProcess;
    } rd;


    //
    //Получаем дескриптор процесса
    //

    rd.hProcess = OpenProcess (PROCESS_CREATE_THREAD |
                               PROCESS_QUERY_INFORMATION |
                               PROCESS_VM_OPERATION |
                               PROCESS_VM_WRITE |
                               PROCESS_VM_READ,
                               FALSE,
                               dwProcessID);
    if (NULL != rd.hProcess)
    {
        return StartRemoteThreadAndWait (rd.hProcess,
                                         lpStartRoutine,
                                         dwCodeSize,
                                         szLibraryPath,
                                         szFunctionName,
                                         szOutputFileName,
                                         os);
    }
    os << "Can not get process handle. ";
    ShowError (GetLastError (), os);

    return FALSE;
}