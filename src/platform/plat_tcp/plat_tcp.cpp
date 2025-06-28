#include "protocol/platprotocolservice.h"
#include "protocol_biz/protocol_login_agent.h"
#include "protocol/TcpManager.h"
#include "protocol/VisitorMonitorTcpManager.h"
#include "plat_tcp.h"

#include <protocol/PlatProtocolService.h>

Plat_tcp * gInterface = new Plat_tcp();

DllLoadInterface * GetDllInterface()
{
   return gInterface;

}

DllLoadInterface * Plat_tcp_lib_GetDllInterface()
{
   return gInterface;

}



Plat_tcp::Plat_tcp()
{
   // m_main = new DllLoadInterface();



    /*
    if( gInterface == nullptr )  gInterface = new  DllLoadService();
    return (void *)gInterface;
    */


}

void Plat_tcp::R_Init()
{
    PlatProtocolService * protocol  = PlatProtocolService::GetInstance();

    protocol->AddService( "Plat_TcpManagerInterface",TcpManager::Instance() );
    protocol->AddService( "Plat_MonitorTcpInterface",VisitorMonitorTcpManager::Instance() );
    protocol->AddService( "PlatLoginAgentInterface",IProtocolLoginAgent::GetInstance() );
}


QString Plat_tcp::GetModuleName()
{
    return "Plat_tcp";
};

void Plat_tcp::Init( int tcount ,  char * argv[]  )
{
    PlatProtocolService * protocol  = PlatProtocolService::GetInstance();

    protocol->AddService( "Plat_TcpManagerInterface",TcpManager::Instance() );
    protocol->AddService( "Plat_MonitorTcpInterface",VisitorMonitorTcpManager::Instance() );
    protocol->AddService( "PlatLoginAgentInterface",IProtocolLoginAgent::GetInstance() );


}

bool Plat_tcp::CheckVersion( QString version )
{
    return true;
}
