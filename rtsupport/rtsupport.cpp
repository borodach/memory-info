// rtsupport.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "rtsupport.h"

#include "ToolhelpSnapeshot.hpp"
#include "RemoteThreadSupport.h"
#include "ToolhelpWrp.hpp"
#include "HeapProcessor.hpp"
#include <fstream.h>  

BOOL APIENTRY DllMain (HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// This is the constructor of a class that has been exported.
// see rtsupport.h for the class definition
CRtsupport::CRtsupport ()
{
    return;
}


RTSUPPORT_API DWORD WINAPI EntryPoint (LPVOID lpParameter)
{
    PacketForRemoteThread *pPacket = (PacketForRemoteThread *) lpParameter; 
    ofstream os (pPacket->szOutputFile, ios::binary | ios::trunc | ios::out);
    if (! os.is_open ())
    {
        return -1;
    }

    CToolHelpSnapeshot heaps (TH32CS_SNAPHEAPLIST);
    if (! heaps.IsValid ())
    {
        return -1;
    }                        

    CHeapProcessor processor (os);
    CToolHelpWrp::ProcessHeapInfo (heaps, processor);
    DWORD dwSize = 0;
    os.write ((char *) &dwSize, sizeof (dwSize));

    return 1;
}