///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           main.cpp
//
//  Facility:       
//
//
//  Abstract:       Главный модуль
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  01/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include <tchar.h>
#include <iostream.h>
#include <windows.h>
#include <Psapi.h>
#include "ToolhelpWrp.hpp"
#include "RemoteThreadSupport.h"


#define LIBRARY_NAME     "rtsupport.dll"
#define ENTRY_POINT_NAME "EntryPoint"

char szLibraryPath [MAX_PATH];
char szOutputFileName [MAX_PATH];
char szProgramName [MAX_PATH];
char *szUsage = " (<process image> | ID:<process ID>) <output file name>";

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator <<
//
//  Description:
//
//  Выводит в поток описание процесса.
//
///////////////////////////////////////////////////////////////////////////////

ostream & operator << (ostream &os, const PROCESSENTRY32 &process)
{
#ifdef _UNICODE   
    char buf [1024];
    wsprintfA (buf, "%S (%d)", process.szExeFile, process.th32ProcessID);
    os << buf; 
#else
    os << process.szExeFile << " (" << process.th32ProcessID << ")";
#endif

    return os;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ShowProcesses
//
//  Description:
//
//  Выводит в поток список описаний процессов.
//
///////////////////////////////////////////////////////////////////////////////

void ShowProcesses (ostream &os, const CToolHelpSnapeshot &processes)
{
    class CProcessViewer : public CToolHelpProcessorStub
    {
    public: CProcessViewer (ostream *pStream) : m_pStream (pStream), m_nIdx (0)
        {
        } 
        virtual BOOL Process (const PROCESSENTRY32 &entry)
        {
            if (NULL != m_pStream)
            {
                *m_pStream << m_nIdx + 1 << ". " << entry << endl;
                ++m_nIdx;

                return TRUE;
            }
            return FALSE;
        }

    protected: 
        ostream *m_pStream;
        int m_nIdx;
    } pv (&os);

    CToolHelpWrp::ProcessProcessInfo (processes, pv);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetLibraryPath
//
//  Description:
//
//  Записывает в szPath полный путь библиотеки szLibraryName
//
///////////////////////////////////////////////////////////////////////////////

BOOL GetLibraryPath (const char *szLibraryName, char *szPath, DWORD dwSize)
{
    HMODULE hModule = LoadLibraryA (szLibraryName);
    if (NULL == hModule)
    {
        return FALSE;
    }

    BOOL bResult = GetModuleFileNameA (hModule, szPath, dwSize);
    FreeLibrary (hModule);

    return bResult;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       _tmain
//
//  Description:
//
//  Точка входа.
//
///////////////////////////////////////////////////////////////////////////////

int _tmain (int argc, TCHAR **argv)
{
    szProgramName [0] = 0;
    if (!GetModuleBaseNameA (GetCurrentProcess (),
                             GetModuleHandle (NULL),
                             szProgramName,
                             sizeof (szProgramName)))
    {
        cout << "Can not get program name. ";
        ShowError (GetLastError (), cout);

        return 0;
    }     

    //
    //Получаем полный путь до библиотеки LIBRARY_NAME
    //

    if (! GetLibraryPath (LIBRARY_NAME, szLibraryPath, sizeof (szLibraryPath)))
    {
        cout << "Can not find library " << LIBRARY_NAME << ". ";
        ShowError (GetLastError (), cout);

        return 0;
    }

    CToolHelpSnapeshot processes (TH32CS_SNAPPROCESS);
    if (! processes.IsValid ())
    {
        cout << "Can not create snapeshot." << endl;

        return 0;
    }

    if (argc < 3)
    {
        cout <<
        "Program needs for two arguments - process image name and output file name."
             <<
        endl;
        cout << "Usage: " << szProgramName << szUsage << endl;
        cout << "Example1: " << szProgramName << " Explorer.exe explorer.dmp"
             << endl;
        cout << "Example2: " << szProgramName << " ID:1050 dump.dmp" << endl;
        ShowProcesses (cout, processes);

        return 0;
    }

    //
    //Получаем полный путь до файла
    //

    char *szFileName;
    char szArg [MAX_PATH];
    wsprintfA (szArg, "%S", argv [2]);
    if (! GetFullPathNameA (szArg,
                            sizeof (szOutputFileName),
                            szOutputFileName,
                            & szFileName))
    {
        cout << "Can not get full output file path . ";
        ShowError (GetLastError (), cout);

        return 0;
    }

    class CProcessProcessor : public CToolHelpProcessorStub
    {
    public:
        CProcessProcessor (LPCTSTR strImageName) : m_strImageName (strImageName),
                                                   m_bProcessFound (FALSE)
        {
        }
        virtual BOOL Process (const PROCESSENTRY32 &entry)
        {
            TCHAR strProcessIdString [16];
            wsprintf (strProcessIdString, TEXT ("ID:%d"), entry.th32ProcessID);
            if (_tcsicmp (entry.szExeFile, m_strImageName) ==
                0 ||
                _tcsicmp (m_strImageName,
                          strProcessIdString) ==
                0)
            {
                DWORD dwCodeSize = (byte *) GetModuleOffset -
                                   (byte *) RemoteThreadProc;
                StartRemoteThreadAndWait (entry.th32ProcessID,
                                          RemoteThreadProc,
                                          dwCodeSize,
                                          szLibraryPath,
                                          ENTRY_POINT_NAME,
                                          szOutputFileName,
                                          cout);
                m_bProcessFound = TRUE;

                return FALSE;
            }
            return TRUE;
        }

        BOOL ProcessFound () const
        {
            return m_bProcessFound;
        }
    protected:                      
        LPCTSTR m_strImageName;
        BOOL m_bProcessFound;
    } pp (argv [1]);
    CToolHelpWrp::ProcessProcessInfo (processes, pp);


    if (! pp.ProcessFound ())
    {
        cout << "Can not find process image \"" << argv [1] << "\"." << endl;
        ShowProcesses (cout, processes);
    }   

    return 1;
}
