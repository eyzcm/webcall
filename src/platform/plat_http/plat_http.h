#ifndef PLAT_HTTP_H
#define PLAT_HTTP_H

#include "plat_http_global.h"
#include "DllLoadInterface.h"

extern "C" PLAT_HTTP_EXPORT DllLoadInterface * GetDllInterface();
extern "C" PLAT_HTTP_EXPORT DllLoadInterface * Plat_http_lib_GetDllInterface();


class PLAT_HTTP_EXPORT Plat_http:public DllLoadInterface
{
public:
    Plat_http();
    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );
};

#endif // PLAT_HTTP_H
