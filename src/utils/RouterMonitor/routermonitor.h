#ifndef ROUTERMONITOR_H
#define ROUTERMONITOR_H

#include "RouterMonitor_global.h"

#include <dllloadinterface.h>

extern "C" ROUTERMONITOR_EXPORT DllLoadInterface * GetDllInterface();
extern "C" ROUTERMONITOR_EXPORT DllLoadInterface * RouterMonitor_lib_GetDllInterface();



class ROUTERMONITOR_EXPORT RouterMonitor:public DllLoadInterface
{
public:
    RouterMonitor();
    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

};

#endif // ROUTERMONITOR_H
