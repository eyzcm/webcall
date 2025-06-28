#ifndef DbUiHistoryInterface_H
#define DbUiHistoryInterface_H


#include "datafactory/ConstructionData.h"
//#include "ui/common/CommonStruct.h"
#include "plat_interface_global.h"
#include "data/UIHistoryInfo.h"
#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <QObject>

class PLAT_INTERFACE_EXPORT DbUiHistoryInterface : public DbUiBaseService
{

    friend class DbUiWebcallService;
    friend class DbUiWebcallServer;

public:
    DbUiHistoryInterface(DbUiService* mainService){ };
    DbUiHistoryInterface(){ };

public:
    ~DbUiHistoryInterface(){ };

public:
    virtual QString      ServiceId( ){return "DbUiHistoryInterface";};
     virtual UIHistoryInfo* CreateInfo(UIHistoryInfo* pinfo = NULL){ return nullptr;};
     virtual void           FreeObj(UIHistoryInfo* obj){ };
     virtual void           AddHistoryMessage(const UIHistoryInfo& recode){ };
     virtual void           ClearChatByVid(const QString& vid){ };
     virtual int            GetHistoryListByChatId(const QString& chatId, const QString& datatype, ConstructionData* factory, void* outlist){ return 0;};
     virtual int            GetHistoryListByVid(const QString& vid, const QString& datatype, ConstructionData* factory, void* outlist){ return 0;};

    virtual bool HasRecordHistoryByVid(const QString& vid){ return false;};
    virtual bool HasRecordHistoryByChatId(const QString& chatid, const QString& vid){return false; };

private:


};

#endif // DbUiHistoryInterface_H
