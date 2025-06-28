#pragma once


#include "datafactory/ConstructionData.h"
#include "plat_interface_global.h"
#include "data/UIChatInfo.h"
#include <QMap>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <QObject>

class PLAT_INTERFACE_EXPORT DbUiConversationInterface : public DbUiBaseService
{
public:
    DbUiConversationInterface(DbUiService *mainService){ };
     DbUiConversationInterface(){ };
    ~DbUiConversationInterface(void){ };

     virtual QString      ServiceId( ){return "DbUiConversationInterface";};
     virtual UIChatInfo *CreateInfo(UIChatInfo *info = NULL){return nullptr; };
     virtual void        FreeObj(UIChatInfo *obj){ };

     virtual void Init(QString userid){ };
     virtual void Clear(){ };
     virtual void ClearChatList(){ };
     virtual void ClearAllCacheChatList(){ };
     virtual void UpdateChatList(QList<UIChatInfo *> &uichatlist){ };
     virtual void TempCacheChatList(QList<UIChatInfo *> &uichatlist){ };
     virtual int  GetAllCacheChatList(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
    virtual bool FindChatInfoByChatId(QString chatid){ return false;};

     virtual QStringList RemoveAllChatByVisitorVId(const QString &vid,int type){ return QStringList();};

     virtual void  AddChat(UIChatInfo *info){ };
     virtual int   GetChatList(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual int   GetChatSize(){ return 0;};
     virtual int   GetALLCacheChatSize(){return 0; };
    virtual bool  CheckChat(const QString &chatid){return false; };
     virtual void *GetChatInfoByChatId(QString chatid, const QString &datatype, ConstructionData *factory){ return nullptr;};
     virtual void *GetChatByChatId(const QString &chatid, const QString &datatype, ConstructionData *factory){return nullptr; };
     virtual void *GetChatByVid(const QString &vid, const QString &datatype, ConstructionData *factory){return nullptr; };
     virtual int   GetChatByUserId(const QString &userid, const QString &datatype, ConstructionData *factory, void *outlist){return 0;};
     virtual int   GetChatByCustomerId(const QString &userid, const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual void  RemoveChatInfoByChatId(const QString &chatid, bool isTempDelet = true){return ; };

     virtual QString GetViewIdByChatId(const QString &chatid){ return "";};
     virtual QString GetChatIdByViewId(const QString &vid){ return "";};

private:
   
    
};
