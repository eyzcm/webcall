#pragma once

#include "./data/UICardInfo.h"
#include "./data/UIVisitorInfo.h"

#include "datafactory/ConstructionData.h"
#include "plat_interface_global.h"
#include "data/UITrackInfo.h"
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <QObject>
#include <QStringList>
#include <qmap>
#include <serviceinterface/sign/DbUiVisitorSignal.h>

class PLAT_INTERFACE_EXPORT DbUiVisitorInterface : public DbUiBaseService
{
    friend class DbUiWebcallService;

public:
    DbUiVisitorInterface(DbUiService *mainService){ };
    DbUiVisitorInterface(){ };

public:
    ~DbUiVisitorInterface(void){ };

    virtual QString      ServiceId( ){return "DbUiVisitorInterface";};
     virtual UIVisitorInfo     *CreateInfo(UIVisitorInfo *info = NULL){ return nullptr;};
     virtual void               FreeObj(UIVisitorInfo *obj){ };
     virtual UICardInfo        *CreateCardInfo(UICardInfo *info = NULL){ return nullptr;};
     virtual void               FreeCardObj(UICardInfo *obj){ };
     virtual void               Clear(){ };
     virtual void               ClearNew(){ };
     virtual void               AddVisitorInfo(const UIVisitorInfo &data){ };
     virtual void               AddVisitorInfoEnd(){ };
     virtual QString            GetVisitorType(const QString &vid){ return ""; };
     virtual void               UpdateVisitorInfo(const UIVisitorInfo &data){ };
     virtual void               MergerVisitorInfoList(QList<UIVisitorInfo> &list){ };
     virtual void               SetVMStatus(const QString &vid, EVISITORSTATUS status){ };
   virtual EVISITORSTATUS     GetVMStatus(const QString &vid){return E_VSTATUS_DEFAULT; };
     virtual int                GetCurrentVisitorInfo(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual void               ClearCurrentVisitorData(){ };
     virtual void              *GetVisitorInfoById(const QString &id, const QString &datatype, ConstructionData *factory){return nullptr; };
     virtual int                GetVisitorInfoByUserId(const QString &userid, const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual void              *GetVisitorCardByUserId(const QString &userId, const QString &datatype, ConstructionData *factory){return nullptr; };
     virtual void               UpdateVisitorCard(UICardInfo *info){ };
     virtual int                GetVisitorInfoCount(){return 0; };
     virtual void               RemoveCachLeaveVisitorByID(const QString &id){ };
     virtual void               RemoveLeaveVisitorByID(const QString &id){ };
     virtual QStringList        TempIdList(){ return QStringList(); };
     virtual void               AddTempIdList(QStringList idList){ };
     virtual QStringList        DeleteIdList(){ return QStringList();};
     virtual void               AddDeleteIdList(QStringList val){ };
     virtual void               SetVisitorType(QString id, QString strType){ };
     virtual void               UpdateVisitorWebpage(UITrackInfo *trackInfo){ };
    virtual DbUiVisitorSignal *Signal(){ return nullptr;};

private:
    //离开的访客vid列表
   
    
};
