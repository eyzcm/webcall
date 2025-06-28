#ifndef DbUiColleagueInterface1_H
#define DbUiColleagueInterface1_H


#include "datafactory/ConstructionData.h"
#include "utils/Plat_EnumDefine.h"
#include "plat_interface_global.h"
#include "data/UIContactGroupInfo.h"
#include "data/UIContactInfo.h"
#include "serviceinterface/dbuiFriendsignal.h"
#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <QObject>
#include "utils/Plat_EnumDefine.h"
//#include <service/sign/dbuiFriendsignal.h>

class PLAT_INTERFACE_EXPORT DbUiColleagueInterface : public DbUiBaseService
{
    friend class DbUiWebcallService;
    friend class DbUiWebcallServer;

public:
    DbUiColleagueInterface(DbUiService *mainService){ };
    DbUiColleagueInterface(){ };

public:

    virtual QString      ServiceId( ){return "DbUiColleagueInterface";};
    ~DbUiColleagueInterface(){ };
     virtual void               *GetFriendByID(const QString &id, const QString &datatype, ConstructionData *factory){ return nullptr;};
     virtual void                AddFriendEnd(){ };
    virtual DbUiFriendSignal   *Signal(){return nullptr; };
     virtual void                AddFriendMember(UIContactInfo *member){ };
     virtual UIContactGroupInfo *CreateInfo(UIContactGroupInfo *pinfo = NULL){return nullptr; };
     virtual UIContactInfo      *CreateContactInfo(UIContactInfo *pinfo = NULL){return nullptr; };
     virtual void                FreeObj(UIContactGroupInfo *obj){ };
     virtual void                FreeContactObj(UIContactInfo *obj){ };

     virtual void Init(QString userId, QString departmentId){ };
     virtual void Clear(){ };
     virtual void ClearFriend(){ };
     virtual void ClearCommonwordUsers(){ };
    virtual void ClearCommonwordUsers1(){ };
    virtual bool HasColleagueList(){ return false;};
     virtual void AddColleagueGroup(UIContactGroupInfo *group){ };
     virtual void AddColleagueMember(UIContactInfo *member){ };
    virtual void  LoadColleagueListEnd(){};
    //检查权限
    virtual bool    CheckPermissionsByRangeCodes(int permvalue, QString userId){return false; };
    virtual bool    CheckPermissions(EPERMISSIONS ptype, QString userId){ return false;};
     virtual void    AddPermissions(EPERMISSIONS ptype, int value){ };
    virtual void SetPresenceStatus(QString id, QString status,QString loginTypeStr ){};
    virtual  void SetPresenceStatus(QString id, EPRESENCESTATUS status,CUSTOMER_LOGIN_TYPE loginType){};

     virtual QString GetOnlineColleagueCount(){ return "";};
     virtual int     GetColleagueCount(){ return 0;};
     virtual void    AddCommonwordUser(UIContactInfo *member){ };
     virtual void    AddCommonwordEnd(){ };
     virtual int     GetCommonwordUser(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual int     GetFriendList(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};
     virtual int     GetColleagueMemberList(const QString &datatype, ConstructionData *factory, void *outlist){return 0; };
     virtual int     GetColleagueGroupList(const QString &datatype, ConstructionData *factory, void *outlist){return 0; };
     virtual int     GetColleagueMemberListByGroupID(const QString &id, const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};

    virtual EPRESENCESTATUS GetPresenceStatusByID(const QString &id){ return E_NONE;};

     virtual void *GetColleagueMemberByID(const QString &id, const QString &datatype, ConstructionData *factory){return nullptr; };
    virtual bool  FindFriendByID(const QString &id){ return false;};
    virtual bool  FindColleagueMemberByID(const QString &id){return false; };
     virtual void *GetColleagueGroupByID(const QString &id, const QString &datatype, ConstructionData *factory){ return nullptr;};

    virtual void  CustomerPresenceChanged(const QString& id, const QString& status, const QString& runningStatus,const QString & loginTypeStr){};
    virtual void  CustomerPresenceChanged(const QString& id, EPRESENCESTATUS eStatus,CUSTOMER_LOGIN_TYPE loginType){};

    //如果没有被过滤，那么返回true， 被过滤了，返回false
    virtual bool IsFilterByUserId(const QString &id){return false; };
     virtual void SetFilter(const QString &id, bool isFilter){ };
     virtual void SaveContactFilter(){ };
     virtual void LoadContactFilter(){ };
    //如果返回true，那么curItem排序在上面
    //如果返回false，那么curItem排序在下面
    virtual bool IsCompareMember(UIContactInfo *curItem, UIContactInfo *other){return false; };

private:
   
    
};

#endif // DbUiColleagueInterface_H
