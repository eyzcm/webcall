#include "protocol_biz/protocol_login_agent.h"

#include "protocol/serviceaddressmanager.h"



IProtocolLoginAgent* IProtocolLoginAgent::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new IProtocolLoginAgent();
    }
    return m_instance;
}

IProtocolLoginAgent::~IProtocolLoginAgent()
{
}

IProtocolLoginAgent::IProtocolLoginAgent()
{
    m_pProtocolAgent = IProtocolLoginAgentImp::GetInstance();
    m_pProtocolAgent->Init();

    connect(m_pProtocolAgent, SIGNAL(S_VMConnected()), this, SIGNAL(S_VMConnected()));
    connect(m_pProtocolAgent, SIGNAL(S_VM_AuthFinished(bool, int)), this, SIGNAL(S_VM_AuthFinished(bool, int)));
      connect(m_pProtocolAgent, SIGNAL(S_VM_LoginFinished(QString)), this, SIGNAL(S_VM_LoginFinished(QString)));

    connect(m_pProtocolAgent, SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)), this, SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)));
    connect(m_pProtocolAgent, SIGNAL(S_MainTcpNetWorkError(int, QString)), this, SIGNAL(S_MainTcpNetWorkError(int, QString)));

    connect(m_pProtocolAgent, SIGNAL(S_ForceLogout()), this, SIGNAL(S_ForceLogout()));

}

IProtocolLoginAgent* IProtocolLoginAgent::m_instance = nullptr;

void IProtocolLoginAgent::Init()
{
    m_pProtocolAgent->Init();

}


void IProtocolLoginAgent::InitTcpAgent(QString loginurl, QString port)
{
    m_pProtocolAgent->InitTcpAgent(loginurl, port);

}

void IProtocolLoginAgent::InitVisitorMonitorTcpAgent(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber)
{
    m_pProtocolAgent->InitVisitorMonitorTcpAgent(loginurl, port, userName, companyId, monitorvisitornumber);
}


void IProtocolLoginAgent::ClearTcpAgent()
{
    m_pProtocolAgent->ClearTcpAgent();
}

void IProtocolLoginAgent::Clear()
{
}


void IProtocolLoginAgent::DoTcpResultLog(QString msgtype, QString body)
{
    Q_EMIT S_TcpResultLog(msgtype, body);
}

void IProtocolLoginAgent::DoTcpReqLog(QString msgtype, QString body)
{
    Q_EMIT S_TcpReqLog(msgtype, body);
}

void IProtocolLoginAgent::DoExecLoginCmd(const QString& userName, const QString& password, int flag,int isforce)
{
    m_pProtocolAgent->DoExecLoginCmd(userName, password, flag,isforce);
}


void IProtocolLoginAgent::DoUpdateLoginToken(const QString& token)
{
    m_pProtocolAgent->DoUpdateLoginToken(token);
}



void IProtocolLoginAgent::DoGetSoftVersion()
{

}

void IProtocolLoginAgent::GetInitData(const QString& key, const QString& url)
{

}

void IProtocolLoginAgent::DoExecVMAuthLogin()
{
    m_pProtocolAgent->DoExecVMAuthLogin();
}

void IProtocolLoginAgent::DoExecVMLogin(QString userId, QString companyId)
{
    m_pProtocolAgent->DoExecVMLogin(userId,companyId);
}

void IProtocolLoginAgent::DoLoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState)
{
    m_pProtocolAgent->DoLoginStateChanged(oldState,  newState);
}


