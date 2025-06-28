#ifndef DbUiCHATGROUPInterface_H
#define DbUiCHATGROUPInterface_H


#include "datafactory/ConstructionData.h"
#include "plat_interface_global.h"
#include "data/UIGroupInfo.h"
#include <QList>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <QObject>

class PLAT_INTERFACE_EXPORT DbUiChatGroupInterface : public DbUiBaseService
{

    friend class DbUiWebcallService;
    friend class DbUiWebcallServer;

public:
    DbUiChatGroupInterface(DbUiService* mainService){ };
    DbUiChatGroupInterface(){ };

public:
    ~DbUiChatGroupInterface(){ };

public:
    virtual QString      ServiceId( ){return "DbUiChatGroupInterface";};
     virtual UIGroupInfo* CreateInfo(UIGroupInfo* pinfo = NULL){return nullptr; };
     virtual void         FreeObj(UIGroupInfo* obj){ };

     virtual void  Clear(){ };
     virtual void  AddChatGroup(UIGroupInfo* info){ };
     virtual void  RemoveChatGroupById(const QString& id){ };
    virtual bool  FindChatGroupById(const QString& id){ return false;};
     virtual void* GetChatGroupById(const QString& id, const QString& datatype, ConstructionData* factory){ return nullptr;};
     virtual int   GetChatGroupList(const QString& datatype, ConstructionData* factory, void* outlist){ return 0;};

private:
   

};

#endif // DbUiCHATGROUPInterface_H
