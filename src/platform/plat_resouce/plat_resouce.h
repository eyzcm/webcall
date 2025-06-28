#ifndef EL_RESOUCE_H
#define EL_RESOUCE_H

#include "plat_resouce_global.h"
#include <dllloadinterface.h>

extern "C" PLAT_RESOUCE_EXPORT DllLoadInterface * GetDllInterface();

class PLAT_RESOUCE_EXPORT Plat_resouce :public DllLoadInterface
{

public:
    Plat_resouce() ;
    ~Plat_resouce();

    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );
};

#endif // EL_RESOUCE_H
