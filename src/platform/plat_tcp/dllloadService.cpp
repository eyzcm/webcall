
#include "dllloadservice.h"
#include "protocol/platprotocolservice.h"
#include "protocol_biz/protocol_login_agent.h"
#include "protocol/TcpManager.h"

DllLoadService::DllLoadService()
{

}

QString DllLoadService::GetModuleName()
{
    return "plat_tcp";
}

void DllLoadService::Init( int /*argc*/ ,  char * argv[]  )
{

    PlatProtocolService::GetInstance()->AddService( "Plat_TcpManagerInterface",TcpManager::Instance() );
    PlatProtocolService::GetInstance()->AddService( "PlatLoginAgentInterface",IProtocolLoginAgent::GetInstance() );

}

bool DllLoadService::CheckVersion( QString /*version*/ )
{
    return true;
}

