#include "PlatProtocolService.h"


PlatProtocolService::PlatProtocolService()
{
    m_tcpService = nullptr;
    m_loginAgentService = nullptr;
    m_monitorTcpService = nullptr;
    m_httpService = nullptr;
    m_platProtocolService = nullptr;
    m_commonModelInterface = nullptr;
    m_commonSignalService = nullptr;

}


void PlatProtocolService::AddService( QString servername ,  void  * service )
{
    if( servername == "Plat_TcpManagerInterface" )
    {

        m_tcpService = ( Plat_TcpManagerInterface *)service;
        m_tcpServicelist.append( m_tcpService);
    }
    else if( servername == "PlatLoginAgentInterface" )
    {
        m_loginAgentService = ( PlatLoginAgentInterface *)service;
        m_loginAgentServicelist.append( m_loginAgentService);
    }
    else if( servername == "Plat_MonitorTcpInterface" )
    {
        m_monitorTcpService = ( Plat_MonitorTcpInterface *)service;
        m_monitorTcpServicelist.append( m_monitorTcpService);
    }
    else if( servername == "Plat_HttpManagerInterface" )
    {
        m_httpService = ( Plat_HttpManagerInterface *)service;
        m_httpServicelist.append( m_httpService);
    }else if( servername == "IPlatProtocolInterface" )
    {
        m_platProtocolService = ( IPlatProtocolInterface *)service;
    }else if( servername == "ICommonModelInterface" )
    {
        m_commonModelInterface = ( ICommonModelInterface *)service;
    }else if( servername == "ICommonSignalInterface" )
    {
        m_commonSignalService = ( ICommonSignalInterface *)service;
	}

}

Plat_TcpManagerInterface * PlatProtocolService::GetSessionService(int index)
{
    return m_tcpServicelist.size()>index?m_tcpServicelist.at(index):nullptr;
}

PlatLoginAgentInterface  * PlatProtocolService::GetLoginAgentService(int index)
{
    return m_loginAgentServicelist.size()>index?m_loginAgentServicelist.at(index):nullptr;

}

Plat_MonitorTcpInterface * PlatProtocolService::GetMonitorTcpService(int index)
{
    return m_monitorTcpServicelist.size()>index?m_monitorTcpServicelist.at(index):nullptr;

}

Plat_HttpManagerInterface * PlatProtocolService::GetHttpService(int index)
{
    return m_httpServicelist.size()>index?m_httpServicelist.at(index):nullptr;

}

ICommonModelInterface  * PlatProtocolService::GetCommonModelService()
{
    return m_commonModelInterface;
}

ICommonSignalInterface  * PlatProtocolService::GetCommonSignalService()
{
    return m_commonSignalService;
}

PlatProtocolService *PlatProtocolService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new PlatProtocolService;
    }
    return m_instance;
}

PlatProtocolService *PlatProtocolService::m_instance = nullptr;

PlatProtocolService::Garbo::~Garbo()
{
    if (PlatProtocolService::m_instance)
    {
        delete PlatProtocolService::m_instance;
        PlatProtocolService::m_instance = NULL;
    }
}

IPlatProtocolInterface * PlatProtocolService::GetPlatProtolService()
{
    return m_platProtocolService;
}

