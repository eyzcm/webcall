#include "VisitorMonitorLoginTcpAgent.h"
#include "service/util_service.h"

#include "protocol/serviceaddressmanager.h"
#include "protocol/visitormonitortcpcmd.h"
#include "protocol/visitormonitortcpmanager.h"
#include "protocol/vm_authcmd.h"
#include "protocol/vm_authlogincmd.h"
#include <QMap>
#include <QTimer>
#include <include/json/value.h>
#include <tcpimp/vmtcp/ElVMStatusProcess.h>
#include <utils/jsonhelper.h>

VisitorMonitorLoginTcpAgent::VisitorMonitorLoginTcpAgent() : ITcpWork()
{
    m_logincount           = 0;
    m_isVmLogined          = false;
    m_isConnect            = false;
    m_isLogin              = false;
    m_loginUrl             = "";
    m_port                 = "";
    m_vmKey                = "";
    m_currentLoginStatus   = E_LOGIN_NONE;
    m_retryVMLoginCount    = 0;
    m_monitorvisitornumber = "50";
    connect(this, SIGNAL(S_test1()), this, SLOT(R_test1()));


}

VisitorMonitorLoginTcpAgent* VisitorMonitorLoginTcpAgent::m_instance = nullptr;

void VisitorMonitorLoginTcpAgent::R_TcpConnect()
{
}

void VisitorMonitorLoginTcpAgent::R_Login()
{
    DoExecVMAuthLogin();
}

void VisitorMonitorLoginTcpAgent::Init(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber)
{
    m_isLastAddressInfo    = false;
    m_userName             = userName;
    m_companyId            = companyId;
    m_loginUrl             = loginurl;
    m_port                 = port;
    m_monitorvisitornumber = monitorvisitornumber;

    TcpReconnect(m_loginUrl, m_port);

}

void VisitorMonitorLoginTcpAgent::Clear()
{
    m_isLastAddressInfo = false;
    ServiceAddressManager::GetInstance()->ResetServiceList(true);
    gVisitorMonitorTcpHvManager->disconnect();
    this->disconnect();
    gVisitorMonitorTcpHvManager->DisconnectTcp();
}

void VisitorMonitorLoginTcpAgent::TcpReconnect(QString loginurl, QString port)
{
    m_isConnect   = false;
    m_isLogin     = false;
    m_isVmLogined = false;
    m_loginUrl    = loginurl;
    m_port        = port;
    gVisitorMonitorTcpHvManager->disconnect();
    gVisitorMonitorTcpHvManager->DisconnectTcp();
    gVisitorMonitorTcpHvManager->CloseTimer();
    gVisitorMonitorTcpHvManager->CreateTimer();

    connect(gVisitorMonitorTcpHvManager, SIGNAL(S_connected()), this, SLOT(R_connected()),Qt::DirectConnection);

    connect(gVisitorMonitorTcpHvManager, SIGNAL(S_MessageParsingFailed()), this, SLOT(R_MessageParsingFailed()),Qt::DirectConnection);
    connect(gVisitorMonitorTcpHvManager, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)),Qt::DirectConnection);
    connect(gVisitorMonitorTcpHvManager, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)),Qt::DirectConnection);

    addressinfo* info = ServiceAddressManager::GetInstance()->GetMonitorAddressInfo();
    if (info)
    {
        gVisitorMonitorTcpHvManager->CreateTcpConnect(info->m_host, info->m_port);
    }
    else
    {
        m_isLastAddressInfo = true;
        gVisitorMonitorTcpHvManager->CreateTcpConnect(loginurl, port);
    }
}

QString VisitorMonitorLoginTcpAgent::Key() const
{
    return m_vmKey;
}

void VisitorMonitorLoginTcpAgent::DoExecVMLogin(QString userId, QString companyId)
{
    m_isLogin   = true;
    m_userName  = userId;
    m_companyId = companyId;
    if (m_isVmLogined)
    {
        VM_AuthLoginCmd* req = new VM_AuthLoginCmd;
        connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecVMLoginCmdResult(TcpMessagePtr)),Qt::DirectConnection);
        req->HasResponse(true);
        req->CompanyId(companyId);
        req->UserId(userId);
        req->SetKey(m_vmKey);
        req->MakeCmd();
        req->SendRequestToServer();
    }
}

void VisitorMonitorLoginTcpAgent::DoExecVMAuthLogin()
{
    //	if (m_isConnect)
    {
        VM_AuthCmd* req = new VM_AuthCmd();
        connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecVMAuthLoginCmdResult(TcpMessagePtr)) );

        req->HasResponse(true);
        req->MakeCmd();
        req->SendRequestToServer();
    }
}

void VisitorMonitorLoginTcpAgent::DoExecVMKeepAlive()
{
    VisitorMonitorTcpCmd cmd;
    cmd.Uri("/keepalive/response");
    cmd.SendRequestToServer();
}

void VisitorMonitorLoginTcpAgent::DoExecVMGetVisitorList()
{
    QString              numVal = m_monitorvisitornumber;
    QString              uri    = QString("/visitor/get?maxcount=").arg(numVal);
    VisitorMonitorTcpCmd cmd;
    cmd.Uri(uri);
    cmd.SendRequestToServer();
}

VisitorMonitorLoginTcpAgent* VisitorMonitorLoginTcpAgent::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new VisitorMonitorLoginTcpAgent;
    }
    return m_instance;
}

VisitorMonitorLoginTcpAgent::~VisitorMonitorLoginTcpAgent()
{
    m_isVmLogined = false;
    m_loginUrl    = "";
    m_port        = "";
    m_vmKey       = "";
}

void VisitorMonitorLoginTcpAgent::R_SetCurrentLoginStatus(ELOGINSTATUS oldState, ELOGINSTATUS newState)
{
    m_currentLoginStatus = newState;

    switch (newState)
    {
        case E_LOGIN_DOING:

            break;
        case E_LOGIN_ALLOK:
            m_isLastAddressInfo = false;

            ServiceAddressManager::GetInstance()->ResetServiceList(true);
            break;
        case E_LOGIN_ERROR_MONITORTCP:
        case E_LOGIN_RETRY_MONITORTCP:
        {
            if (m_setRetryLogin == 0)
            {

                m_setRetryLogin = 1;
                if (m_retryVMLoginCount > 5)
                {
                    QTimer::singleShot(60 * 1000, this, SLOT(ReTryVMLoginConnect()));
                }
                else if (m_retryVMLoginCount > 3 && m_retryVMLoginCount < 5)
                {
                    QTimer::singleShot(30 * 1000, this, SLOT(ReTryVMLoginConnect()));
                }
                else
                {
                    QTimer::singleShot(10 * 1000, this, SLOT(ReTryVMLoginConnect()));
                }
                m_retryVMLoginCount++;
            }
        }
        break;
        default:
            break;
    }
}

void VisitorMonitorLoginTcpAgent::R_ReceivedMessage(TcpMessagePtr pmsg)
{
	TcpMessage * msg = pmsg.data();
    if (msg == nullptr)
    {
        Q_ASSERT(false);
        return;
    }
    if (msg->Uri() == "/auth")
    {
        ParseAuth(msg);
    }
    else if (msg->Uri() == "/keepalive/response")
    {
    }
    else if (msg->Uri() == "/im/forceLogout")
    {
        //强制注销。TODO shizhenyu
    }
    else if (msg->Uri() == "/keepalive/request")
    {

        DoExecVMKeepAlive();
    }

    /*
    else if (msg->Uri() == "/visitor/get")
    {
        Q_EMIT S_VMGetVisitorListFinished(msg->Body());
    }
    else if (msg->Uri() == "/visitor/changed")
    {
        Q_EMIT S_VMVisitorChanged(msg->Body());
    }
    else if (msg->Uri() == "/visitor/remove")
    {
        Q_EMIT S_VMVisitorRemove(msg->Body());
    }
    else if (msg->Uri() == "/visitor/remove/temp")
    {
        // zcmtest add
        Q_EMIT S_VMVisitorTempRemove(msg->Body());

    }
    else if (msg->Uri() == "/visitor/monitor")
    {
        Q_EMIT S_VMVisitorMonitor(msg->Body());


    }
    else
    {
        Q_ASSERT(false);
    }
    if (msg)
    {
        msg->deleteLater();
        msg = nullptr;
    }
    */
}

void VisitorMonitorLoginTcpAgent::R_NetWorkError(int errorCode, QString msg)
{
    if (m_isLastAddressInfo)
    {
        m_isConnect   = false;
        m_isLogin     = false;
        m_isVmLogined = false;
        Q_EMIT S_NetWorkError(errorCode, msg);
        if (m_currentLoginStatus == E_LOGIN_RETRY_MONITORTCP)
        {
            if (m_setRetryLogin == 0)
            {

                m_setRetryLogin = 1;
                if (m_retryVMLoginCount > 5)
                {
                    QTimer::singleShot(60 * 1000, this, SLOT(ReTryVMLoginConnect()));
                }
                else if (m_retryVMLoginCount > 3 && m_retryVMLoginCount < 5)
                {
                    QTimer::singleShot(30 * 1000, this, SLOT(ReTryVMLoginConnect()));
                }
                else
                {
                    QTimer::singleShot(10 * 1000, this, SLOT(ReTryVMLoginConnect()));
                }
                m_retryVMLoginCount++;
            }
        }
    }
    else
    {
        m_isConnect   = false;
        m_isLogin     = false;
        m_isVmLogined = false;
        TcpReconnect(m_loginUrl, m_port);
    }
}

void VisitorMonitorLoginTcpAgent::R_DoExecVMLoginCmdResult(TcpMessagePtr msg)
{
    VM_AuthLoginCmd* obj = (VM_AuthLoginCmd*)QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }

    Json::Value val;
    bool        isOK = JsonHelper::GetJsonValueFromBody(msg->Body(), val);
    if (isOK)
    {
        QMap<QString, QString> tempMap;
        bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
        if (isParse)
        {
            if (tempMap.find("success") != tempMap.end())
            {
                if (tempMap["success"] == "1")
                {
                    Q_EMIT S_VM_LoginFinished(msg->Body());
                }
                else
                {
                    QString strKey = tempMap["key"];
                    m_vmKey        = strKey;
                    // code为1代表token验证错误， 为2代表系统错误， 如数据库连接失败之类的错误
                    //每次失败都会生成新的key
                    QString strCode = tempMap["code"];
                    if (strCode == "1")
                    {
                        DoExecVMLogin(m_userName, m_companyId);
                    }
                    else if (strCode == "2")
                    {
                        Q_EMIT S_VM_LoginFinished(msg->Body());
                    }
                    else
                    {
                        //直接退出还是如何：
                        Q_EMIT S_VM_LoginFinished(msg->Body());
                    }
                }
            }
        }
    }
    if (msg)
    {
        msg->deleteLater();
        msg = nullptr;
    }
}

void VisitorMonitorLoginTcpAgent::R_DoExecVMAuthLoginCmdResult(TcpMessagePtr msg)
{
    Json::Value val;
    bool        isOK = JsonHelper::GetJsonValueFromBody(msg->Body(), val);
    if (isOK)
    {
        QMap<QString, QString> tempMap;
        bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
        if (isParse)
        {
            QString strKey = tempMap["key"];
            m_isVmLogined  = true;
            m_vmKey        = strKey;
            if (m_isLogin)
            {
                DoExecVMLogin(m_userName, m_companyId);
            }
            Q_EMIT S_VM_AuthFinished(true, 200);
        }
    }
    else
    {
        Q_EMIT S_VM_AuthFinished(false, 500);
    }
    if (msg)
    {
        msg->deleteLater();
        msg = nullptr;
    }
}

void VisitorMonitorLoginTcpAgent::R_connected()
{
    m_retryVMLoginCount = 0;
    m_isConnect         = true;
    if (m_isVmLogined)
    {
        DoExecVMAuthLogin();
    }
    Q_EMIT S_connected();
}

void VisitorMonitorLoginTcpAgent::R_MessageParsingFailed()
{
    Q_EMIT S_MessageParsingFailed();
}

void VisitorMonitorLoginTcpAgent::ReTryVMLoginConnect()
{
    m_setRetryLogin = 0;
    if (m_currentLoginStatus == E_LOGIN_RETRY_MONITORTCP || m_currentLoginStatus == E_LOGIN_ERROR_MONITORTCP)
    {
        TcpReconnect(m_loginUrl, m_port);
    }
}

void VisitorMonitorLoginTcpAgent::DoTryVMLoginConnect()
{
    TcpReconnect(m_loginUrl, m_port);
}

void VisitorMonitorLoginTcpAgent::ParseAuth(TcpMessage* msg)
{
    if (m_isVmLogined)
    {
        //已经获取到key
        Json::Value val;
        bool        isOK = JsonHelper::GetJsonValueFromBody(msg->Body(), val);
        if (isOK)
        {
            QMap<QString, QString> tempMap;
            bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
            if (isParse)
            {
                if (tempMap.find("success") != tempMap.end())
                {
                    if (tempMap["success"] == "1")
                    {
                        Q_EMIT S_VM_LoginFinished(msg->Body());
                    }
                    else
                    {
                        if (m_logincount <= 10)
                        {
                            QString strKey = tempMap["key"];
                            m_vmKey        = strKey;
                            m_isVmLogined  = true;
                            DoExecVMLogin(m_userName, m_companyId);
                            m_logincount++;
                        }
                    }
                }
            }
        }
    }
    else
    {
        bool isError = true;
        do
        {
            Json::Value val;
            bool        isOK = JsonHelper::GetJsonValueFromBody(msg->Body(), val);
            if (isOK)
            {
                QMap<QString, QString> tempMap;
                bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
                if (isParse)
                {
                    QString strKey = tempMap["key"];
                    m_vmKey        = strKey;
                    m_isVmLogined  = true;
                    DoExecVMLogin(m_userName, m_companyId);
                    isError = false;
                    // Q_EMIT S_VM_AuthFinished(true, 200);
                }
            }
        } while (false);

        if (isError)
        {
            Q_EMIT S_VM_AuthFinished(false, 500);
        }
    }
}

void VisitorMonitorLoginTcpAgent::DoExecVMAuthLoginCmdResult(TcpMessagePtr message)
{
    return R_DoExecVMAuthLoginCmdResult(message);
}

bool VisitorMonitorLoginTcpAgent::isLoginMaxCount()
{
    return (m_logincount >= 10);
}

void VisitorMonitorLoginTcpAgent::R_test1()
{
    qDebug()<<"r_test1";
    int i = 0;
}


