///////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File:           utils.cpp
//
//  Facility:       Поддержка работы со снимком toolhelp
//
//
//  Abstract:       Реализация методов для вывода данных Toolhelp.
//
//  Environment:    VC 6.0
//
//  Author:         Зайцев С. А.
//
//  Creation Date:  01/12/2003
//
///////////////////////////////////////////////////////////////////////////////

#include "utils.h"



///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator <<
//
//  Description:
//
//  Выводит в поток описание кучи.
//
///////////////////////////////////////////////////////////////////////////////

ostream& operator << (ostream& os, const HEAPLIST32 & heap)
{
    if (heap.dwFlags & HF32_DEFAULT) {
        os << "process deafault heap: ";
    }
    else {
        os << "dynamic heap: ";
    }
    os << "heapID = " << heap.th32HeapID 
       << ", processID = " << heap.th32ProcessID;

    return os;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator <<
//
//  Description:
//
//  Выводит в поток описание блоков кучи.
//
///////////////////////////////////////////////////////////////////////////////

ostream& operator << (ostream& os, const HEAPENTRY32 & block)
{
    char buf [1024];
    sprintf (buf, "0x%08X\t%10d\t%4d\t", block.dwAddress, 
                                       block.dwBlockSize, 
                                       block.dwLockCount);
    os << buf;
    if (block.dwFlags & LF32_FREE) os << "free ";
    if (block.dwFlags & LF32_FIXED) os << "fixed ";
    if (block.dwFlags & LF32_MOVEABLE) os << "movable";

    return os;
}              

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ShowHeapInfo
//
//  Description:
//
//  Выводит в поток описание кучи.
//
///////////////////////////////////////////////////////////////////////////////

//void ShowHeapInfo (ostream &os, const CToolHelpWrp::HEAPENTRY32_STORAGE &blocks) 
//{
//    size_t nLength = blocks.size ();
//    if (0 == nLength) {
//        os << "Heap is empty.\n";
//        return;
//    }
//    os << "Heap has " << nLength << " blocks\n";
//    {
//        char buf [1024];
//        sprintf (buf, "%10s\t%10s\t%4s\t%s", "address", "size", "lock", "type");
//        os << buf << "\n";
//    }
//    for (size_t nIdx = 0; nIdx < nLength; ++nIdx) {
//        os << /*nIdx + 1 << ". " << */blocks [nIdx] << "\n";
//    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ShowProcessHeaps
//
//  Description:
//
//  Выводит в поток описание куч. 
//
///////////////////////////////////////////////////////////////////////////////
//
//void ShowProcessHeaps (const CToolHelpWrp::HEAPLIST32_STORAGE &heaps, 
//                       ostream& os)
//{
//    CToolHelpWrp::HEAPENTRY32_STORAGE blocks;
//    size_t nLength = heaps.size ();
//    if (0 == nLength) {
//        os << "Process has not heap." << endl;
//        return;
//    }
//    os << "Process  has " << nLength << " heap(s)" << endl;
//    for (size_t nIdx = 0; nIdx < nLength; ++nIdx) {
//        os << nIdx + 1 << ". " << heaps [nIdx] << "\n";
//
//        CToolHelpWrp::ExtractHeapBlockInfo (heaps [nIdx].th32ProcessID,
//                                            heaps [nIdx].th32HeapID,                                         
//                                            blocks);
//        ShowHeapInfo (os, blocks);
//        os << endl;
//    }
}