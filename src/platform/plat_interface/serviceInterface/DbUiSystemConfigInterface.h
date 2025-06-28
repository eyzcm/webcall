#ifndef DbUiSystemConfigInterface_H
#define DbUiSystemConfigInterface_H
#include <QDateTime>


#include "datafactory/ConstructionData.h"
#include "data/CommInfo.h"
//#include "ui/common/CommonStruct.h"

#include "plat_interface_global.h"
#include "data/UIEmotion.h"
#include "data/UILoginData.h"
#include "data/UISystemTipData.h"
#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <QObject>
#include "data/UISkinInfo.h"
#include "data/UINewVersionInfo.h"

class PLAT_INTERFACE_EXPORT DbUiSystemConfigInterface : public DbUiBaseService
{

    friend class DbUiWebcallService;


public:
    DbUiSystemConfigInterface(DbUiService* mainService){ };
    DbUiSystemConfigInterface(){ };

public:
    ~DbUiSystemConfigInterface(){ };

    virtual QString      ServiceId( ){return "DbUiSystemConfigInterface";};
     virtual UISystemTipData* CreateInfo(UISystemTipData* pinfo = NULL){return nullptr; };
     virtual void             FreeObj(UISystemTipData* obj){ };

     virtual void      Init(){ };
     virtual void      LoadSystemConfig(){ };
    virtual CommInfo* SystemConfig(){ return nullptr;};
     virtual void      SystemConfig(const CommInfo& val){ };
     virtual QString   GetCurrentVersion(){ return "";};
    virtual bool      istestVersion(){return true;};
     virtual QString   GetSystemConfigByKey(const QString& key, const QString& defaultString) = 0;//{return ""; };



     virtual void LoadDefaultSystemTipSetting(){ };
     virtual void LoadEngineData(){ };


     virtual int   DefaultSystemTipList(const QString& datatype, ConstructionData* factory, void* outlist){return 0; };
     virtual void  DefaultSystemTipList(QList<UISystemTipData*> val){ };
     virtual void* GetSystemTipByKey(const QString& type, const QString& datatype, ConstructionData* factory){return nullptr; };

     virtual QString GetEngineNameById(const QString& id){ return "";};
    virtual QString GetEngineCNameById(const QString& id){ return "";};

     virtual int GetEngineList(const QString& datatype, ConstructionData* factory, void* outlist){return 0; };

     virtual int       Timediff(){return 0; };
     virtual void      Timediff(int val){ };
     virtual QDateTime GetCurrentServerTime(){return QDateTime(); };
     virtual UISkinInfo * CurrentSkin(){return nullptr;};
    virtual  void   CurrentSkin(UISkinInfo * info){};
     virtual UINewVersionInfo * NewVersion() = 0;
     virtual void  NewVersion(UINewVersionInfo * info ) = 0;
     virtual void  AutoRemindUpdate(int val) = 0;
     virtual int   AutoRemindUpdate() = 0;
     virtual QString LockPassword() = 0;
    virtual void LockPassword(QString password) = 0;
    virtual QMap<QString, UISkinInfo *> GetSkinMap() = 0;


private:
   

};

#endif // DbUiSystemConfigInterface_H
