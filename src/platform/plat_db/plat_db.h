#ifndef PLAT_DB_H
#define PLAT_DB_H
#include <dllloadinterface.h>
#include "plat_db_global.h"

extern "C" PLAT_DB_EXPORT DllLoadInterface * GetDllInterface();
extern "C" PLAT_DB_EXPORT DllLoadInterface * plat_db_lib_GetDllInterface();


class PLAT_DB_EXPORT Plat_db:public DllLoadInterface
{
public:
    Plat_db();
    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

};

#endif // PLAT_DB_H
