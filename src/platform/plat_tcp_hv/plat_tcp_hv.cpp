#include "plat_tcp_hv.h"


#include "protocol/platprotocolservice.h"
#include "protocol_biz/protocol_login_Hvagent.h"
#include "protocol/TcpHvManager.h"
#include "protocol/VisitorMonitorTcpHvManager.h"


#include <protocol/PlatProtocolService.h>

Plat_tcp_hv * gInterface = new Plat_tcp_hv();
DllLoadInterface * GetDllInterface()
{
   return gInterface;

}


Plat_tcp_hv::Plat_tcp_hv()
{
   // m_main = new DllLoadInterface();



    /*
    if( gInterface == nullptr )  gInterface = new  DllLoadService();
    return (void *)gInterface;
    */


}

void Plat_tcp_hv::R_Init()
{
    PlatProtocolService * protocol  = PlatProtocolService::GetInstance();

    protocol->AddService( "Plat_TcpManagerInterface",TcpHvManager::Instance() );
    protocol->AddService( "Plat_MonitorTcpInterface",VisitorMonitorTcpHvManager::Instance() );
    protocol->AddService( "PlatLoginAgentInterface",IProtocolLoginHvAgent::GetInstance() );
}


QString Plat_tcp_hv::GetModuleName()
{
    return "Plat_tcp_hv";
};

void Plat_tcp_hv::Init( int tcount ,  char * argv[]  )
{
    PlatProtocolService * protocol  = PlatProtocolService::GetInstance();

    protocol->AddService( "Plat_TcpManagerInterface",TcpHvManager::Instance() );
    protocol->AddService( "Plat_MonitorTcpInterface",VisitorMonitorTcpHvManager::Instance() );
    protocol->AddService( "PlatLoginAgentInterface",IProtocolLoginHvAgent::GetInstance() );

}

bool Plat_tcp_hv::CheckVersion( QString version )
{
    return true;
}
