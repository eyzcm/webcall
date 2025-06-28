#ifndef PLAT_TCP_HV_H
#define PLAT_TCP_HV_H

#include "plat_tcp_hv_global.h"
#include "DllLoadInterface.h"

class PLAT_TCP_HV_EXPORT Plat_tcp_hv: public DllLoadInterface
{
   Q_OBJECT

public:
    Plat_tcp_hv();
    virtual QString GetModuleName();
    virtual void Init( int  ,  char * argv[]  );
    virtual bool CheckVersion( QString version );

   // DllLoadInterface * m_main;

private slots:
    void R_Init();

};

extern "C" PLAT_TCP_HV_EXPORT   DllLoadInterface  *   GetDllInterface();


#endif // PLAT_TCP_HV_H
