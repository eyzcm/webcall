#ifndef JSWXLOGINHELPER_H
#define JSWXLOGINHELPER_H

#include <qobject.h>
#include "BaseJsLoginHelper.h"
#include "utils/Plat_EnumDefine.h"

class JsWxLoginHelper : public BaseJsLoginHelper
{
    Q_OBJECT

public:
    JsWxLoginHelper();
    ~JsWxLoginHelper();

public:
    virtual void LoadFinished();
    virtual void SetUrl(const QString &url);
    void         SetInitData();
    void         SetAccountWayData(QString userId, QString password);
    void         HideWxLogin();
    void         SetLoginType(int type);
    QString      CheckUrl();
    int          IsCheck();
    int          IsPwdSafed();

signals:
    void S_Login(QString uid, QString token, QString serverip);
    void S_AuthorizationLoginDenied();
    void S_BackAccountLogin();
    void S_UpdateToken(QString token);
    void S_JsLoginexception(int i);

public slots:
    void    Login(QString uid, QString token);
    void    LoginError(QString code, QString msg);
    void    JsSetClientParam(QString key, QString val);
    QString JsGetClientParam(QString key);
    void    JsLoginexception(QString error); //登录异常提示
    void    JsLoginjump(int i);              //登录框跳转

private:
    QString m_checkUrl;
    int     m_isPwdSafed;
    int     m_isCheck;
};

#endif // JSWXLOGINHELPER_H
