#include "protocol_biz/protocol_login_Hvagent.h"

#include "protocol/serviceaddressmanager.h"



IProtocolLoginHvAgent* IProtocolLoginHvAgent::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new IProtocolLoginHvAgent();
    }
    return m_instance;
}

IProtocolLoginHvAgent::~IProtocolLoginHvAgent()
{
}

IProtocolLoginHvAgent::IProtocolLoginHvAgent()
{
    m_pProtocolAgent = IProtocolLoginHvAgentImp::GetInstance();
    m_pProtocolAgent->Init();

    connect(m_pProtocolAgent, SIGNAL(S_VMConnected()), this, SIGNAL(S_VMConnected()));
    connect(m_pProtocolAgent, SIGNAL(S_VM_AuthFinished(bool, int)), this, SIGNAL(S_VM_AuthFinished(bool, int)));
      connect(m_pProtocolAgent, SIGNAL(S_VM_LoginFinished(QString)), this, SIGNAL(S_VM_LoginFinished(QString)));

    connect(m_pProtocolAgent, SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)), this, SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)));
    connect(m_pProtocolAgent, SIGNAL(S_MainTcpNetWorkError(int, QString)), this, SIGNAL(S_MainTcpNetWorkError(int, QString)));

    connect(m_pProtocolAgent, SIGNAL(S_ForceLogout()), this, SIGNAL(S_ForceLogout()));

}

IProtocolLoginHvAgent* IProtocolLoginHvAgent::m_instance = nullptr;

void IProtocolLoginHvAgent::Init()
{
    m_pProtocolAgent->Init();

}


void IProtocolLoginHvAgent::InitTcpAgent(QString loginurl, QString port)
{
    m_pProtocolAgent->InitTcpAgent(loginurl, port);

}

void IProtocolLoginHvAgent::InitVisitorMonitorTcpAgent(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber)
{
    m_pProtocolAgent->InitVisitorMonitorTcpAgent(loginurl, port, userName, companyId, monitorvisitornumber);
}


void IProtocolLoginHvAgent::ClearTcpAgent()
{
    m_pProtocolAgent->ClearTcpAgent();
}

void IProtocolLoginHvAgent::Clear()
{
}


void IProtocolLoginHvAgent::DoTcpResultLog(QString msgtype, QString body)
{
    Q_EMIT S_TcpResultLog(msgtype, body);
}

void IProtocolLoginHvAgent::DoTcpReqLog(QString msgtype, QString body)
{
    Q_EMIT S_TcpReqLog(msgtype, body);
}

void IProtocolLoginHvAgent::DoExecLoginCmd(const QString& userName, const QString& password, int flag,int isforce)
{
    m_pProtocolAgent->DoExecLoginCmd(userName, password, flag,isforce);
}


void IProtocolLoginHvAgent::DoUpdateLoginToken(const QString& token)
{
    m_pProtocolAgent->DoUpdateLoginToken(token);
}



void IProtocolLoginHvAgent::DoGetSoftVersion()
{

}

void IProtocolLoginHvAgent::GetInitData(const QString& key, const QString& url)
{

}

void IProtocolLoginHvAgent::DoExecVMAuthLogin()
{
    m_pProtocolAgent->DoExecVMAuthLogin();
}

void IProtocolLoginHvAgent::DoExecVMLogin(QString userId, QString companyId)
{
    m_pProtocolAgent->DoExecVMLogin(userId,companyId);
}

void IProtocolLoginHvAgent::DoLoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState)
{
    m_pProtocolAgent->DoLoginStateChanged(oldState,  newState);
}


