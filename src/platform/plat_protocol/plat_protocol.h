#ifndef PLAT_PROTOCOL_H
#define PLAT_PROTOCOL_H

#include "plat_protocol_global.h"
#include "dllloadinterface.h"


extern "C" PLAT_PROTOCOL_EXPORT DllLoadInterface * GetDllInterface();
extern "C" PLAT_PROTOCOL_EXPORT DllLoadInterface * Plat_protocol_lib_GetDllInterface();

class PLAT_PROTOCOL_EXPORT Plat_protocol: public DllLoadInterface
{
public:
    Plat_protocol();

    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

};

#endif // PLAT_PROTOCOL_H
