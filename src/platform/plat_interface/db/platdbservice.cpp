#include "platdbservice.h"


PlatDbService::PlatDbService()
{
    m_cacheDb = nullptr;

}

PlatDbService *PlatDbService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new PlatDbService;
    }
    return m_instance;
}

PlatDbService *PlatDbService::m_instance = nullptr;

PlatDbService::Garbo::~Garbo()
{
    if (PlatDbService::m_instance)
    {
        delete PlatDbService::m_instance;
        PlatDbService::m_instance = NULL;
    }
}


void PlatDbService::AddCacheDbServer(CacheDbInterface * service,int type )
{
    switch( type)
    {
    case 0:
        if( service)  m_cacheDb = service;
        break;
    case 1:
        if( service)  m_webcacheDb = service;
        break;
    }

}

CacheDbInterface * PlatDbService::GetCacheDbServer( )
{
    return m_cacheDb;
}

CacheDbInterface * PlatDbService::GetWebCacheDbServer( )
{
    return m_webcacheDb;
}



