#ifndef LoginServiceIMPL_H
#define LoginServiceIMPL_H

#include "utils/Plat_EnumDefine.h"
#include "include/json/value.h"
#include "SessionServiceImpl.h"
#include "loginservice.h"
#include <QObject>
#include <QTimer>
#include "protocol/platprotocolservice.h"


//class IProtocolLoginAgentImp;
class SessionServiceImpl;
class LoginServerSignal;

class LoginServiceImpl : public LoginService
{
    Q_OBJECT

public:
    LoginServiceImpl(SessionServiceImpl *main);
    ~LoginServiceImpl();

    LoginServerSignal * LoginSignal();

signals:

    void S_AdminAuthLogin();
    void S_MainTcpLoginFinished(QString authType, QString errorCode, QString body);
    void S_VM_LoginEnd();
    void S_CustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus,CUSTOMER_LOGIN_TYPE loginType);
    void S_MainTcpConnected();
    void S_ForceLogout();
    void S_ForceReLogin(int force);


private slots:
    void R_OnReLoginTime();
    void R_MainTcpConnected();
    void R_MainTcpNetWorkError(int errorCode, QString msg);
    void R_MainTcpLoginFinished(QString authType, QString errorCode, QString body);
    void R_ForceLogout();
    void R_VMMessageParsingFailed();
    void R_VM_LoginFinished(QString body);
    void R_VM_AuthFinished(bool isOk, int code);
    void R_OnVisitorMonitorTimer();
    void R_VMConnected();
    void R_LoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState);

public:
    int          GetLoginErrorCount();
    void         DoLogin();
    void         CreateReloginTimer();
    void         Init();
    void         CurrentLoginState(ELOGINSTATUS val);
    ELOGINSTATUS CurrentLoginState() const;
    void         LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void         MainTcpNetWorkError(ELOGINSTATUS loginStatus, int errorCode, QString msg);
    void         MainTcpLoginFinished(QString authType, QString errorCode, QString body);
    void         VM_LoginEnd();
    void         MainTcpConnected();

private:
    void                CreateVisitorMonitorTimer();
    void                InitTimer();
    QString             GetAiUrl(QString companyId);

    void                ParseLoginResult(QString body);
    void                ParseLicenceData(Json::Value val);
    void                ParseTimeoutReplayData(Json::Value val);
    void                ParseGlobalConfigData(Json::Value val);
    
    QString             GetSystemConfigByKey(const QString &key, const QString &defaultString);
    void                ParseUserInfoData(Json::Value val);

    SessionServiceImpl *m_main;
    PlatLoginAgentInterface  *m_protocol;

    ECLIENTTYPE         m_clientType;
    ELOGINSTATUS        m_currentLoginState;
    QTimer             *m_visitorMonitorTimer;
    QTimer             *m_reLoginTimer;
    int                 m_vmLogined;
    int                 m_loginErrorCount;

    LoginServerSignal * m_logingServerSignal;

};

#endif // LoginServiceIMPL_H
