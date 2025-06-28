#include "plat_protocol.h"

#include <protocol/platprotocolservice.h>

#include <protocol_biz/platprotocolagent.h>

Plat_protocol::Plat_protocol()
{
}


Plat_protocol * gInterface = new Plat_protocol();

DllLoadInterface * GetDllInterface()
{
    return gInterface;
}
DllLoadInterface * Webcall_protocol_lib_GetDllInterface()
{
    return gInterface;
}



QString Plat_protocol::GetModuleName()
{
    return "plat_protocol";
};

void Plat_protocol::Init( int tcount ,  char * argv[]  )
{
    PlatProtocolService * protocol = PlatProtocolService::GetInstance();
    protocol->AddService( "IPlatProtocolInterface",PlatProtocolAgent::GetInstance());



}

bool Plat_protocol::CheckVersion( QString version )
{
    return true;
}
