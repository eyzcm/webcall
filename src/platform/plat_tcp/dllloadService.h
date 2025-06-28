#ifndef DllLoadService_h
#define DllLoadService_H
#include "plat_tcp_global.h"
#include <qobject.h>
#include "DllLoadInterface.h"

class PLAT_TCP_EXPORT DllLoadService : public DllLoadInterface
{
public:
    DllLoadService();

    virtual QString GetModuleName();
    virtual void Init( int argc ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

};




#endif // DLLLOADINTERFACE_H
