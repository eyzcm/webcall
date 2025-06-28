#include "el_sessionservice.h"

el_sessionService::el_sessionService()
{

    m_sessionService = nullptr;
	m_LoginPageService = nullptr;
}


void el_sessionService::AddService( QString name, void * service )
{
    if( name == "el_sessionInterface")
    {
        if( m_sessionService ) m_sessionService->deleteLater();
        m_sessionService = (el_sessionInterface *)service;
    }
    if( name == "LoginTransitionsPageInterface")
    {
        if( m_LoginPageService ) delete m_LoginPageService;
        m_LoginPageService = (LoginTransitionsPageInterface *)service;
    }

}

el_sessionInterface * el_sessionService::GetSessionService()
{

    return m_sessionService;
}

LoginTransitionsPageInterface * el_sessionService::GetLoginTransitionsPageService()
{
    return m_LoginPageService;
}


el_sessionService *el_sessionService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new el_sessionService;
    }
    return m_instance;
}

el_sessionService *el_sessionService::m_instance = nullptr;

el_sessionService::Garbo::~Garbo()
{
    if (el_sessionService::m_instance)
    {
        delete el_sessionService::m_instance;
        el_sessionService::m_instance = NULL;
    }
}

