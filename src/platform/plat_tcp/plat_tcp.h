#ifndef PLAT_TCP_H
#define PLAT_TCP_H
#include <qobject.h>

#include "plat_tcp_global.h"
#include "DllLoadInterface.h"

class PLAT_TCP_EXPORT Plat_tcp: public DllLoadInterface
{
   Q_OBJECT

public:
    Plat_tcp();
    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

   // DllLoadInterface * m_main;

private slots:
    void R_Init();

};

extern "C" PLAT_TCP_EXPORT   DllLoadInterface  *   GetDllInterface();
extern "C" PLAT_TCP_EXPORT   DllLoadInterface  *   Plat_tcp_lib_GetDllInterface();



#endif // PLAT_TCP_H
