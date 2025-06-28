#pragma once

#include "./data/UISiteInfo.h"
#include "plat_interface_global.h"


#include "datafactory/ConstructionData.h"

#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <data/UIChatInfo.h>

class PLAT_INTERFACE_EXPORT DbUiChatRecordInterface : public DbUiBaseService
{
public:
    DbUiChatRecordInterface(DbUiService *mainService){ };
    DbUiChatRecordInterface(){};
    ~DbUiChatRecordInterface(void){ };

    virtual QString      ServiceId( ){return "DbUiChatRecordInterface";};
     virtual int   GetChatRecord(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual void  ClearChatRecordInfo(){ };
     virtual int   AddChatRecordInfo(UIChatInfo *info){ return 0;};
     virtual void *findChatRecordById(QString id, const QString &datatype, ConstructionData *factory){return nullptr; };

private:
   
    
};
