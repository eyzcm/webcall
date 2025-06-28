#ifndef DBUIPLATSERVICE_H
#define DBUIPLATSERVICE_H

#include <QObject>
#include <serviceInterface/dbuihistoryinterface.h>
#include "plat_interface_global.h"
#include "service/dbuiservice.h"

class DbUiPersonalConfigInterface;
class DbUiConversationInterface;
class DbUiColleagueInterface;
class DbUiChatInterface;
class DbUiChatGroupInterface;
class DbUiCacheInterface;
class DbUiVisitorInterface;
class DbUiWaitVisitorInterface;
class ConstructionDataInterface;
class DbUiSystemConfigInterface;
class DbUiHistoryInterface;
class DbUiChatRecordInterface;

class PLAT_INTERFACE_EXPORT DbUiPlatService:public DbUiService
{

public:
    ~DbUiPlatService(){};
    static DbUiPlatService *GetInstance();

private:
    DbUiPlatService();
    class Garbo
    {
    public:
        ~Garbo();
    };
    static DbUiPlatService *m_instance;
public:


    void   AddService(  DbUiBaseService * service  );

    DbUiVisitorInterface          *VisitorService();
    DbUiHistoryInterface    *HistoryService();

    DbUiPersonalConfigInterface   *PersonalConfigService();
    DbUiConversationInterface     *ConversationService();
    DbUiColleagueInterface        *ColleagueService();
    DbUiChatGroupInterface        *ChatGroupService();
    DbUiChatInterface             *ChatService();
    DbUiCacheInterface            *CacheService();



    DbUiWaitVisitorInterface      *WaitVisitorInfoService();
    DbUiSystemConfigInterface     *SystemConfigService();
    DbUiChatRecordInterface       *ChatRecordService();




/*
      DbUiChatUserInterface         *ChatUserService();
      DbUiChatInterface             *ChatService();
      DbUiVisitorInterface          *VisitorService();

      DbUiConversationInterface     *ConversationService();
      */

private:
    DbUiSystemConfigInterface     *m_systemConfigService;
    DbUiVisitorInterface          *m_visitorService;
    DbUiHistoryInterface          *m_historyService;
    DbUiPersonalConfigInterface   *m_personalConfigService;
    DbUiConversationInterface     *m_conversationService;
    DbUiColleagueInterface        *m_colleagueService;
    DbUiChatGroupInterface        *m_chatGroupService;
    DbUiChatInterface             *m_chatService;
    DbUiCacheInterface            *m_cacheService;

    DbUiWaitVisitorInterface      *m_waitVisitorService;
    ConstructionDataInterface     *m_constructionData;
    DbUiChatRecordInterface       *m_chatRecordService;



};

#include "serviceinterface/dbuihistoryinterface.h"
#include "serviceinterface/DbUiConversationInterface.h"
#include "serviceinterface/DbUiChatInterface.h"
#include "serviceinterface/DbUiColleagueInterface.h"
#include "serviceinterface/DbUiChatGroupInterface.h"
#include "serviceinterface/DbUiVisitorInterface.h"
#include "serviceinterface/DbUiWaitVisitorInterface.h"

#include "mem/DbUiCacheInterface.h"
//#include "serviceinterface/DbUiPersonalConfigInterface.h"
#include "serviceinterface/DbUiSystemConfigInterface.h"
#include "serviceinterface/DbUiChatRecordInterface.h"



#define gUISystemConfigManager DbUiPlatService::GetInstance()->SystemConfigService()
#define gUIHistoryManager DbUiPlatService::GetInstance()->HistoryService()
//#define gUIPersonalConfigManager DbUiPlatService::GetInstance()->PersonalConfigService()
#define gUIConversationManager DbUiPlatService::GetInstance()->ConversationService()
#define gUIColleagueManager DbUiPlatService::GetInstance()->ColleagueService()
#define gUIChatGroupManager DbUiPlatService::GetInstance()->ChatGroupService()
#define gUIChatServiceManager DbUiPlatService::GetInstance()->ChatService()
#define gUICacheService DbUiPlatService::GetInstance()->CacheService()
#define gUIVisitorInfoManager DbUiPlatService::GetInstance()->VisitorService()
#define gUIWaitVisitorInfoManager DbUiPlatService::GetInstance()->WaitVisitorInfoService()
#define gUIChatRecordServiceManager DbUiPlatService::GetInstance()->ChatRecordService()



#endif // DBUIPLATSERVICE_H
