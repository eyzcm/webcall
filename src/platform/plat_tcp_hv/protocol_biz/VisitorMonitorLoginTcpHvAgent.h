#ifndef VisitorMonitorLoginTcpHvAgent_H
#define VisitorMonitorLoginTcpHvAgent_H

#include <QObject>

#include "utils/Plat_EnumDefine.h"

#include "protocol/tcpmessage.h"
#include "tcpimp/baseprocess/ITcpWork.h"
#include <QVector>

class VisitorMonitorLoginTcpHvAgent : public ITcpWork
{
    Q_OBJECT

public:
    static VisitorMonitorLoginTcpHvAgent* GetInstance();
    ~VisitorMonitorLoginTcpHvAgent();

private:
    VisitorMonitorLoginTcpHvAgent();
    static VisitorMonitorLoginTcpHvAgent* m_instance;

    int m_setRetryLogin;

    virtual void R_TcpConnect();
    virtual void R_Login();

public:

    void Init(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber);
    void Clear();
    void TcpReconnect(QString loginurl, QString port);

public:
    bool    isLoginMaxCount();
    QString Key() const;
    void    R_IsVMLogin(bool m_isLogin);
    void    DoExecVMLogin(QString userId, QString companyId);
    void    DoExecVMAuthLogin();
    void    DoTryVMLoginConnect();
    void    DoExecVMKeepAlive();
    void    DoExecVMGetVisitorList();
    void    DoExecVMAuthLoginCmdResult(TcpMessagePtr message);

public slots:
    void R_test1();
     void R_connected();
    void R_SetCurrentLoginStatus(ELOGINSTATUS oldState, ELOGINSTATUS newState);

signals:
    void S_test1();
    void S_connected();
    void S_NetWorkError(int errorCode, QString msg);
    void S_MessageParsingFailed();
    void S_VM_AuthFinished(bool isOk, int code);
    void S_VM_LoginFinished(QString body);
    //////////////////////////////////////////////////////////////////////////
    void S_VMGetVisitorListFinished(QString body);
    void S_VMVisitorChanged(QString body);
    void S_VMVisitorRemove(QString body);
    void S_VMVisitorTempRemove(QString body);
    void S_VMVisitorMonitor(QString body);
    //////////////////////////////////////////////////////////////////////////



private slots:
    void R_ReceivedMessage(TcpMessagePtr msg);
    void R_NetWorkError(int errorCode, QString msg);
    void R_DoExecVMLoginCmdResult(TcpMessagePtr message);
    void R_DoExecVMAuthLoginCmdResult(TcpMessagePtr message);

    void R_MessageParsingFailed();
    void ReTryVMLoginConnect();

private:
    void ParseAuth(TcpMessage* msg);

private:
    bool         m_isLastAddressInfo;
    QString      m_loginUrl;
    QString      m_port;
    int          m_logincount;
    QString      m_vmKey;
    bool         m_isVmLogined;
    bool         m_isConnect;
    bool         m_isLogin;
    QString      m_userName;
    QString      m_companyId;
    int          m_retryVMLoginCount;
    ELOGINSTATUS m_currentLoginStatus;
    QString      m_monitorvisitornumber;
};

#define gVisitorMonitorLoginTcpHvAgent VisitorMonitorLoginTcpHvAgent::GetInstance()

#endif // VisitorMonitorLoginTcpHvAgent_H
