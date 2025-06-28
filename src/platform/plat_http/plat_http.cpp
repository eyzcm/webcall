#include "plat_http.h"
#include "protocol/platprotocolservice.h"

#include <protocol/httpService.h>

Plat_http * gInterface = new Plat_http();

DllLoadInterface * GetDllInterface()
{

    return gInterface;

}
DllLoadInterface * Plat_http_lib_GetDllInterface()
{

    return gInterface;

}


Plat_http::Plat_http()
{
}


QString Plat_http::GetModuleName()
{
    return "Plat_http";
};

void Plat_http::Init( int tcount ,  char * argv[]  )
{
    PlatProtocolService * service = PlatProtocolService::GetInstance();
    service->AddService("Plat_HttpManagerInterface", HttpService::GetInstance() );

}

bool Plat_http::CheckVersion( QString version )
{
    return true;
}
