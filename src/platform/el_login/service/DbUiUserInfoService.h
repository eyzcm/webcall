#pragma once

#include "utils/baseConstructionData.h"

#include "data/UILoginData.h"
#include "data/UIUserInfo.h"
#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include "utils/UISingleton.h"
#include <QObject>
#include <service/DbUiUserInfoInterface.h>

class DbUiUserInfoServiceImpl;

class  DbUiUserInfoService :public QObject,public DbUiUserInfoInterface,public UISingleton<DbUiUserInfoService>
{
Q_OBJECT
    friend class UISingleton<DbUiUserInfoService>; //因为基类在getInstance()中 new T() 时会调用到A的构造函数
    friend class DbUiService;


private:
    DbUiUserInfoService(DbUiService* mainService=nullptr);

public:
    ~DbUiUserInfoService(void);

public:
    virtual QString      ServiceId();
    void         Clear();
    UIUserInfo*  CreateInfo(UIUserInfo* info = NULL);
    UILoginData* CreateLoginInfo(UILoginData* info = NULL);
    void         UserInfo(UIUserInfo* info);
    void         LoginData(UILoginData* info);
    UILoginData* LoginData() const;

    UIUserInfo* UserInfo();

    QString LockPassword();
    void    LockPassword(QString password);
    void    SetNewToken(QString token);

private:
    QMutex                   m_mutex;
    DbUiUserInfoServiceImpl* m_service;
};

#define gUiUserInfoService = gUIUserInfoManager
