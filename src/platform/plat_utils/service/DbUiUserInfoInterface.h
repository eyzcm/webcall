#pragma once



#include "data/UILoginData.h"
#include "data/UIUserInfo.h"

#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"


class  DbUiUserInfoInterface : public DbUiBaseService
{
    friend class DbUiService;

public:
    DbUiUserInfoInterface( ){};

public:
    ~DbUiUserInfoInterface(void){};

public:
    virtual void         Clear(){};
    virtual UIUserInfo*  CreateInfo(UIUserInfo* info = NULL);
	virtual UILoginData* CreateLoginInfo(UILoginData* info = NULL);
	virtual void         UserInfo(UIUserInfo* info);
	virtual void         LoginData(UILoginData* info);
	virtual UILoginData* LoginData() const
    {
        return m_service->LoginData();
    }
	virtual UIUserInfo* UserInfo();

    virtual QString LockPassword();
	virtual void    LockPassword(QString password);
	virtual void    SetNewToken(QString token);

private:
    
    DbUiUserInfoInterface* m_service;
};

//#define gUiUserInfoService  DbUiService::GetInstance()->UserInfoService()
