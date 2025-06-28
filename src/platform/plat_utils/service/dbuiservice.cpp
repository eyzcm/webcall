#include "DbUiService.h"
#include "./utils/EL_Util_Time.h"
#include "QHash.h"
#include <qdebug.h>

DbUiService *DbUiService::m_server = nullptr;

DbUiService::DbUiService()
{
    m_userInfoService         = nullptr;
    /*
    m_chatUserservice         = nullptr;
    m_chatService             = nullptr;
    m_visitorService          = nullptr;

    m_conversationService     = nullptr;
    m_commonWordService       = nullptr;
    m_emotionService          = nullptr;
    m_extendConfigService     = nullptr;
    m_colleagueService        = nullptr;
    m_historyService          = nullptr;
    m_personalConfigService   = nullptr;
    m_summarizeInfoService    = nullptr;
    m_systemConfigService     = nullptr;
    m_trackInfoService        = nullptr;
    m_visitorColService       = nullptr;
    m_waitVisitorService      = nullptr;
    m_otherService            = nullptr;
    m_chatRecordService       = nullptr;
    m_cacheService            = nullptr;
    m_menuConfigService       = nullptr;
    m_personalConfigYLService = nullptr;
    m_manRobotService         = nullptr;
    m_highAutoReplyService    = nullptr;
    */
}

DbUiService::~DbUiService()
{
}

DbUiService *DbUiService::GetInstance1()
{
    if (DbUiService::m_server == nullptr)
    {
        DbUiService::m_server = new DbUiService();
    }
    return DbUiService::m_server;
}

void DbUiService::Release()
{
    if (DbUiService::m_server)
    {
        delete DbUiService::m_server;
        DbUiService::m_server = nullptr;
    }
}


DbUiUserInfoInterface *DbUiService::UserInfoService()
{
    /*if (m_userInfoService == nullptr)
        m_userInfoService = new DbUiUserInfoInterface(this);
        */
    return m_userInfoService;
}


#if 0
DbUiChatUserService *DbUiService::ChatUserService()
{
    if (m_chatUserservice == nullptr)
    {
        m_chatUserservice = new DbUiChatUserService(this);
    }
    return m_chatUserservice;
}

DbUiChatService *DbUiService::ChatService()
{
    if (m_chatService == nullptr)
    {
        m_chatService = new DbUiChatService(this);
    }
    return m_chatService;
}

DbUiManRobotService *DbUiService::ManRobotService()
{
    if (m_manRobotService == nullptr)
    {
        m_manRobotService = new DbUiManRobotService(this);
    }
    return m_manRobotService;
}

DbUiVisitorService *DbUiService::VisitorService()
{
    if (m_visitorService == nullptr)
    {
        m_visitorService = new DbUiVisitorService(this);
    }
    return m_visitorService;
}

DbUiEmotionService *DbUiService::EmotionService()
{
    if (m_emotionService == nullptr)
        m_emotionService = new DbUiEmotionService(this);
    return m_emotionService;
}

DbUiExtendConfigService *DbUiService::ExtendConfigService()
{
    if (m_extendConfigService == nullptr)
        m_extendConfigService = new DbUiExtendConfigService(this);
    return m_extendConfigService;
}

DbUiColleagueService *DbUiService::ColleagueService()
{
    if (m_colleagueService == nullptr)
        m_colleagueService = new DbUiColleagueService(this);
    return m_colleagueService;
}

DbUiHistoryService *DbUiService::HistoryService()
{
    if (m_historyService == nullptr)
        m_historyService = new DbUiHistoryService(this);
    return m_historyService;
}

DbUiSummarizeInfoService *DbUiService::SummarizeInfoService()
{
    if (m_summarizeInfoService == nullptr)
        m_summarizeInfoService = new DbUiSummarizeInfoService(this);
    return m_summarizeInfoService;
}

DbUiSystemConfigService *DbUiService::SystemConfigService()
{
    if (m_systemConfigService == nullptr)
        m_systemConfigService = new DbUiSystemConfigService(this);
    return m_systemConfigService;
}

DbUiTrackInfoService *DbUiService::TrackInfoService()
{
    if (m_trackInfoService == nullptr)
        m_trackInfoService = new DbUiTrackInfoService(this);
    return m_trackInfoService;
}

DbUiVisitorColService *DbUiService::VisitorColService()
{
    if (m_visitorColService == nullptr)
        m_visitorColService = new DbUiVisitorColService(this);
    return m_visitorColService;
}

DbUiChatGroupService *DbUiService::ChatGroupService()
{
    if (m_chatGroupService == nullptr)
        m_chatGroupService = new DbUiChatGroupService(this);
    return m_chatGroupService;
}

UiData *DbUiService::UiData()
{
    return UiData::GetInstance();
}

DbUiConversationService *DbUiService::ConversationService()
{
    if (m_conversationService == nullptr)
        m_conversationService = new DbUiConversationService(this);

    return m_conversationService;
}

DbUiCommonWordService *DbUiService::CommonWordService()
{
    if (m_commonWordService == nullptr)
        m_commonWordService = new DbUiCommonWordService(this);

    return m_commonWordService;
}

DbUiWaitVisitorService *DbUiService::WaitVisitorInfoService()
{
    if (m_waitVisitorService == nullptr)
        m_waitVisitorService = new DbUiWaitVisitorService(this);

    return m_waitVisitorService;
}

DbUiOtherService *DbUiService::OtherService()
{
    if (m_otherService == nullptr)
        m_otherService = new DbUiOtherService(this);

    return m_otherService;
}

DbUiChatRecordService *DbUiService::ChatRecordService()
{
    if (m_chatRecordService == nullptr)
        m_chatRecordService = new DbUiChatRecordService(this);

    return m_chatRecordService;
}

DbUiCacheService *DbUiService::CacheService()
{
    if (m_cacheService == nullptr)
        m_cacheService = new DbUiCacheService(this);

    return m_cacheService;
}

DbUiPersonalConfigYLService *DbUiService::PersonalConfigYLService()
{
    if (m_personalConfigYLService == nullptr)
        m_personalConfigYLService = new DbUiPersonalConfigYLService(this);

    return m_personalConfigYLService;
}

DbUiMenuConfigService *DbUiService::MenuConfigService()
{
    if (m_menuConfigService == nullptr)
        m_menuConfigService = new DbUiMenuConfigService(this);
    return m_menuConfigService;
}

DbUiHighAutoReplyService *DbUiService::HighAutoReplyService()
{
    if (m_highAutoReplyService == nullptr)
        m_highAutoReplyService = new DbUiHighAutoReplyService(this);
    return m_highAutoReplyService;
}
#endif

void DbUiService::AddService(  DbUiBaseService * service  )
{

    if( service->ServiceId() == "DbUiUserInfoInterface")
    {

        m_userInfoService = (DbUiUserInfoInterface * )service;
    }

}

