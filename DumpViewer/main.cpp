///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           main.cpp
//
//  Facility:       
//
//
//  Abstract:
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  04/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <Psapi.h>
#include <fstream.h>

char szProgramName [MAX_PATH];
char *szUsage = " <dump file 1> [<dump file 2>]";

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

int ShowDump (const char *szDumpFile, ostream &os);
int CompareDumps (const char *szBaseDumpFile,
                  const char *szDumpFile,
                  ostream &os);

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       main
//
//  Description:
//
//  Точка входа
//
///////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv [])
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

    if (argc < 2)
    {
        cout << szProgramName << szUsage << endl;

        return 0;
    }

    if (argc == 2)
    {
        return ShowDump (argv [1], cout);
    }
    else
    {
        return CompareDumps (argv [1], argv [2], cout);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ShowDump
//
//  Description:
//
//  Выводит содержимое дампа szDumpFile в поток os
//
///////////////////////////////////////////////////////////////////////////////

int ShowDump (const char *szDumpFile, ostream &os)
{
    ifstream dumpStream (szDumpFile, ios::binary | ios::in | ios::nocreate);
    if (! dumpStream.is_open ())
    {
        os << "Can not open file \"" << szDumpFile << "\"" << endl;

        return 0;
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CompareDumps
//
//  Description:
//
//  Выводит в os различия дампов szBaseDumpFile и szDumpFile
//
///////////////////////////////////////////////////////////////////////////////

int CompareDumps (const char *szBaseDumpFile,
                  const char *szDumpFile,
                  ostream &os)
{
    ifstream baseDumpStream (szBaseDumpFile,
                             ios::binary | ios::in | ios::nocreate);
    if (! baseDumpStream.is_open ())
    {
        os << "Can not open file \"" << szBaseDumpFile << "\"" << endl;

        return 0;
    }

    ifstream dumpStream (szDumpFile, ios::binary | ios::in | ios::nocreate);
    if (! dumpStream.is_open ())
    {
        os << "Can not open file \"" << szDumpFile << "\"" << endl;

        return 0;
    }

    return 1;
}