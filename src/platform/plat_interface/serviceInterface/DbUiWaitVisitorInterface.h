#pragma once
#include "./data/UIWaitVisitorInfo.h"

#include "datafactory/ConstructionData.h"
#include "plat_interface_global.h"
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <qmap.h>

class PLAT_INTERFACE_EXPORT DbUiWaitVisitorInterface : public DbUiBaseService
{
public:
    DbUiWaitVisitorInterface(DbUiService *mainService){ };
    DbUiWaitVisitorInterface(){ };
    ~DbUiWaitVisitorInterface(void){ };
     virtual UIWaitVisitorInfo *CreateInfo(UIWaitVisitorInfo *info = NULL){ return nullptr;};

    virtual QString      ServiceId( ){return "DbUiWaitVisitorInterface";};
     virtual void  FreeObj(UIWaitVisitorInfo *obj){ };
     virtual void  Clear(){ };
     virtual void  AddQueueVisitor(UIWaitVisitorInfo *info){ };
     virtual void  RemoveQueueVisitorByVid(const QString &vid){ };
     virtual void  ClearQueueList(){ };
     virtual int   GetWaitQueueList(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual void *GetWaitQueueInfoById(const QString &id, const QString &datatype, ConstructionData *factory){ return nullptr;};
     virtual int   GetWaitQueueInfoByUserId(const QString &userid, const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual int   GetQueueVisitorCount(){return 0; };
     virtual void *GetWaitQueueInfoByRandom(const QString &datatype, ConstructionData *factory){return nullptr; };

private:
   
    
};
