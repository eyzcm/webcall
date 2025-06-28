#include "dbuiplatservice.h"

DbUiPlatService::DbUiPlatService()
{
    m_visitorService          = nullptr;
    m_waitVisitorService      = nullptr;
    m_historyService        = nullptr;
    m_personalConfigService        = nullptr;
    m_conversationService        = nullptr;
    m_colleagueService        = nullptr;
    m_chatService        = nullptr;
    m_cacheService        = nullptr;
    m_constructionData        = nullptr;
    m_systemConfigService = nullptr;

}

DbUiVisitorInterface  *DbUiPlatService::VisitorService()
{
    if (m_visitorService == nullptr)
    {
        m_visitorService = new DbUiVisitorInterface(this);
    }
    return m_visitorService;
}


DbUiWaitVisitorInterface  *DbUiPlatService::WaitVisitorInfoService()
{
    if (m_waitVisitorService == nullptr)
        m_waitVisitorService = new DbUiWaitVisitorInterface(this);

    return m_waitVisitorService;
}

DbUiCacheInterface            *DbUiPlatService::CacheService()
{
    if (m_cacheService == nullptr)
        m_cacheService = new DbUiCacheInterface(this);

    return m_cacheService;
}


void DbUiPlatService::AddService(  DbUiBaseService * service  )
{

    if( service->ServiceId() == "DbUiVisitorInterface")
        {

            m_visitorService = (DbUiVisitorInterface * )service;
    }
    if( service->ServiceId() == "DbUiWaitVisitorInterface")
        {

            m_waitVisitorService = (DbUiWaitVisitorInterface * )service;
        }

    if( service->ServiceId() == "DbUiSystemConfigInterface")
        {

            m_systemConfigService = (DbUiSystemConfigInterface * )service;
        }
    if( service->ServiceId() == "DbUiHistoryInterface")
        {

            m_historyService = (DbUiHistoryInterface * )service;
        }
    if( service->ServiceId() == "DbUiPersonalConfigInterface")
        {

            m_personalConfigService = (DbUiPersonalConfigInterface * )service;
        }
    if( service->ServiceId() == "DbUiConversationInterface")
        {

            m_conversationService = (DbUiConversationInterface * )service;
        }
    if( service->ServiceId() == "DbUiColleagueInterface")
        {

            m_colleagueService = (DbUiColleagueInterface * )service;
        }
    if( service->ServiceId() == "DbUiChatGroupInterface")
        {

            m_chatGroupService = (DbUiChatGroupInterface * )service;
        }
    if( service->ServiceId() == "DbUiChatInterface")
        {

            m_chatService = (DbUiChatInterface * )service;
        }
    if( service->ServiceId() == "DbUiCacheInterface")
        {

            m_cacheService = (DbUiCacheInterface * )service;
        }
    if( service->ServiceId() == "ConstructionDataInterface")
        {

            m_constructionData = (ConstructionDataInterface * )service;
        }
    if( service->ServiceId() == "DbUiChatRecordInterface")
        {

            m_chatRecordService = (DbUiChatRecordInterface * )service;
        }

}


DbUiPlatService *DbUiPlatService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new DbUiPlatService;
    }
    return m_instance;
}

DbUiPlatService *DbUiPlatService::m_instance = nullptr;

DbUiPlatService::Garbo::~Garbo()
{
    if (DbUiPlatService::m_instance)
    {
        delete DbUiPlatService::m_instance;
        DbUiPlatService::m_instance = NULL;
    }
}

DbUiSystemConfigInterface     *DbUiPlatService::SystemConfigService()
{


    return m_systemConfigService;
}

DbUiHistoryInterface  *DbUiPlatService::HistoryService()
{
    if (m_historyService == nullptr)
        m_historyService = new DbUiHistoryInterface(this);
    return m_historyService;
}


DbUiChatRecordInterface  *DbUiPlatService::ChatRecordService()
{
    if (m_chatRecordService == nullptr)
        m_chatRecordService = new DbUiChatRecordInterface(this);

    return m_chatRecordService;
}

DbUiColleagueInterface        *DbUiPlatService::ColleagueService()
{
    if (m_colleagueService == nullptr)
        m_colleagueService = new DbUiColleagueInterface(this);

    return m_colleagueService;
}

DbUiChatInterface             *DbUiPlatService::ChatService()
{
    if (m_chatService == nullptr)
        m_chatService = new DbUiChatInterface(this);

    return m_chatService;
}


DbUiConversationInterface     *DbUiPlatService::ConversationService()
{
    if (m_conversationService == nullptr)
        m_conversationService = new DbUiConversationInterface(this);

    return m_conversationService;
}


DbUiChatGroupInterface  *DbUiPlatService::ChatGroupService()
{
    if (m_chatGroupService == nullptr)
        m_chatGroupService = new DbUiChatGroupInterface(this);
    return m_chatGroupService;
}



