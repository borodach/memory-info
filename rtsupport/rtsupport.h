
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RTSUPPORT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RTSUPPORT_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RTSUPPORT_EXPORTS
#define RTSUPPORT_API __declspec(dllexport)
#else
#define RTSUPPORT_API __declspec(dllimport)
#endif

// This class is exported from the rtsupport.dll
class RTSUPPORT_API CRtsupport
{
public:
    CRtsupport (void);
    // TODO: add your methods here.
};

extern RTSUPPORT_API int nRtsupport;

RTSUPPORT_API int fnRtsupport (void);

