#pragma once
#include <service/DbUiBaseService.h>
#include <data/UILoginData.h>
#include <data/UIUserInfo.h>


#include <QList>
#include <QMap>
#include <QObject>

class DbUiUserInfoServiceImpl : public DbUiBaseService
{
    friend class DbUiUserInfoService;

protected:
    DbUiUserInfoServiceImpl();

public:
    ~DbUiUserInfoServiceImpl(void);

public:
    void         Clear();
    UIUserInfo*  CreateInfo(UIUserInfo* info = NULL);
    UILoginData* CreateLoginInfo(UILoginData* info = NULL);
    void         UserInfo(UIUserInfo* info);
    void         LoginData(UILoginData* info);
    UILoginData* LoginData() const
    {
        return m_loginData;
    }
    UIUserInfo* UserInfo() const;
    QString     LockPassword();
    void        LockPassword(QString password);
    void        SetNewToken(QString token);

private:
    //登录数据
    UILoginData* m_loginData;
    //当前用户
    UIUserInfo* m_userInfo;
    QString     m_lockPassword;
};
