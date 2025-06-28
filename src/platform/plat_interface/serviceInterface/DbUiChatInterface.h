#pragma once

#include "./data/UIChatInfo.h"
#include "datafactory/ConstructionData.h"

#include "plat_interface_global.h"
#include <QHash>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <serviceinterface/sign/dbuichatsignal.h>

typedef enum{
    YL_AiChatFlag = 1,
    YL_TAKEOVERINFO,
    YL_REMOVECHAT,
    YL_IsAckLeave,
    YL_TakeoverFlag,
    YL_AiChatFlag_Ex,
    YL_location,
    YL_LOCALIMG,
    YL_Card_SaveType,
    //todo 需求5332/10855 敏感词屏蔽
    YL_SensitiveWords,
    YL_REMOVECHAT_Ex = 99

}UpdateChatInfoType;

typedef void (*fenumFun)(void *param1, const QString &keyid, const QList<UIChatInfo *> &varinfo);

class PLAT_INTERFACE_EXPORT DbUiChatInterface : public DbUiBaseService
{
    friend class DbUiWebcallService;

public:
    DbUiChatInterface(DbUiService *mainService){ };
    DbUiChatInterface(){ };

public:
    ~DbUiChatInterface(void){ };
    virtual QString      ServiceId( ){return "DbUiChatInterface";};

     virtual QString GetViewIdByChatId(const QString &chatid, int type){ return "";};
     virtual QString GetCurrentChatIds(){ return "";};
    virtual QString GetCurrentinvalidChatIds(){ return "";};
     virtual void    AddBatchId(){ };
     virtual int     GetBatchId(){ return 0;};
     virtual int     GetChatCount(){ return 0;};

    virtual int     GetChatCountByUserId(QString userId){return 0;};

     virtual void    AddOcpcData(QString chatId, QString jsonstr){ };
     virtual QString GetOcpcData(QString chatId){ return "";};
     virtual void    AddBaidubcpData(QString chatId, QString jsonstr){ };
     virtual QString GetBaidubcpData(QString chatId){ return "";};

     virtual UIChatInfo *CreateInfo(UIChatInfo *pinfo = NULL){ return nullptr;};
     virtual void        FreeObj(UIChatInfo *obj){ };

     virtual void UpdateList(UIChatInfo *pinfo){ };

     virtual void AddNewInfo(UIChatInfo *pinfo){ };
    //增量添加对话
     virtual void AddNewChatInfo(UIChatInfo *pinfo){ };
     virtual void UpdateNewChatInfo(UIChatInfo *pinfo){ };
     virtual void AddNewInfoEnd(){ };
     virtual void ClearNewInfo(int flag ){ };

     virtual void       *GetChatByChatId(const QString &chatid, const QString &datatype, ConstructionData *factory){return nullptr; };
     virtual int         GetCustomerNewChatListByCustomerId(QString id, const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual QStringList GetChatCustomerIdList(){ return QStringList();};
     virtual QStringList GetChatIdList(){ return QStringList();};

     virtual void DeleteChatByChatId(QString chatId, int delFlag, QMap<QString, QString> extendInfo = QMap<QString, QString>()){ };

     virtual void *GetOldChatInfoByChatId(const QString &chatid, const QString &datatype, ConstructionData *factory){return nullptr; };
     virtual int   GetChatListData(QString datatype, ConstructionData *factory, void *outlist){return 0; };
     virtual void  Enum(fenumFun fun, void *param){ };

     virtual void UpdateCardInfo(UICardInfo *info){ };
     virtual void UpdateChatInfo(UpdateChatInfoType flag, QString chatId, QString newValue){ };

    virtual DbUiChatSignal *Signal(){return nullptr; };
    virtual void ProcessSignData(){};

private:



};
