#include "protocol_login_agentImp.h"
#include "qdebug.h"

#include "./protocol_biz/tcploginagent.h"
#include "./protocol_biz/VisitorMonitorLoginTcpAgent.h"


#include "protocol/serviceaddressmanager.h"
#include "protocol_biz/protocol_login_agent.h"
//#include "session/el_sessionservice.h"

#include "thread/protocolthread.h"
#include "utils/utilshelper.h"
#include "session/el_sessionService.h"

IProtocolLoginAgentImp *IProtocolLoginAgentImp::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new IProtocolLoginAgentImp;
    }
    return m_instance;
}

IProtocolLoginAgentImp::~IProtocolLoginAgentImp()
{
}

IProtocolLoginAgentImp::IProtocolLoginAgentImp()
{
    m_pProtocolwork      = nullptr;
   m_tcpAgent           = TcpLoginAgent::GetInstance();
    m_factoryBaseData    = new BaseConstructionData();
    m_currentChatType    = 1; //默认为增量方式
    m_ForceLogin         = 0;
}

IProtocolLoginAgentImp *IProtocolLoginAgentImp::m_instance = nullptr;

void IProtocolLoginAgentImp::Init()
{
    qDebug()<<"moveToThread IProtocolLoginAgentImp"<<endl;
    m_pProtocolwork = new ProtocolLoginWork();
    m_pProtocolwork->Protocol(this);
    //ServiceAddressManager::GetInstance()->Init(gSessionService->LoginData());
   gVisitorMonitorLoginTcpAgent->moveToThread(&m_protocolThread);

     connect(m_tcpAgent, SIGNAL(S_LoginFinished(QString, QString, QString)), this, SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)));
    connect(m_tcpAgent, SIGNAL(S_ForceLogout()), this, SIGNAL(S_ForceLogout()));
    connect(m_tcpAgent, SIGNAL(S_ForceReLogin(int)), this, SIGNAL(S_ForceReLogin(int)));
    connect(m_tcpAgent, SIGNAL(S_NetWorkError(int, QString)), this, SIGNAL(S_MainTcpNetWorkError(int, QString)));



    connect(gVisitorMonitorLoginTcpAgent, SIGNAL(S_connected()), this, SIGNAL(S_VMConnected()));
    connect(gVisitorMonitorLoginTcpAgent, SIGNAL(S_connected()), this, SLOT(R_VMConnected()));

    connect(gVisitorMonitorLoginTcpAgent, SIGNAL(S_VM_AuthFinished(bool, int)), this, SIGNAL(S_VM_AuthFinished(bool, int)));
    connect(gVisitorMonitorLoginTcpAgent, SIGNAL(S_VM_LoginFinished(QString)), this, SIGNAL(S_VM_LoginFinished(QString)));

    connect(gVisitorMonitorLoginTcpAgent, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_VMMessageParsingFailed()));
/*
    connect(gVisitorMonitorTcpAgent, SIGNAL(S_VMGetVisitorListFinished(QString)), this, SIGNAL(S_VMGetVisitorListFinished(QString)));
    connect(gVisitorMonitorTcpAgent, SIGNAL(S_VMVisitorChanged(QString)), this, SIGNAL(S_VMVisitorChanged(QString)));
    connect(gVisitorMonitorTcpAgent, SIGNAL(S_VMVisitorRemove(QString)), this, SIGNAL(S_VMVisitorRemove(QString)));
    connect(gVisitorMonitorTcpAgent, SIGNAL(S_VMVisitorTempRemove(QString)), this, SIGNAL(S_VMVisitorTempRemove(QString)));
    connect(gVisitorMonitorTcpAgent, SIGNAL(S_VMVisitorMonitor(QString)), this, SIGNAL(S_VMVisitorMonitor(QString)));
*/
  //  connect(this, SIGNAL(S_AdminAuthLogin()), gIProtocolLoginAgent, SIGNAL(S_AdminAuthLogin()));


    m_pProtocolwork->moveToThread(&m_protocolThread);


    m_pProtocolwork->Init();
	m_protocolThread.SetMainObject(m_pProtocolwork);
    m_protocolThread.start();
    
}

void IProtocolLoginAgentImp::InitTcpAgent(QString loginurl, QString port)
{
    m_tcpAgent->Init(loginurl, port);
}

void IProtocolLoginAgentImp::InitVisitorMonitorTcpAgent(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber)
{
    gVisitorMonitorLoginTcpAgent->Init(loginurl, port, userName, companyId, monitorvisitornumber);
}


void IProtocolLoginAgentImp::DoLoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState)
{
    m_tcpAgent->R_SetCurrentLoginStatus(oldState, newState);
    gVisitorMonitorLoginTcpAgent->R_SetCurrentLoginStatus(oldState, newState);
}

ECLIENTTYPE IProtocolLoginAgentImp::GetClientType()
{

    QString     key     = "6";// gUISystemConfigManager->GetSystemConfigByKey("ismedical", "0");
    ECLIENTTYPE retType = UtilsHelper::GetClientTypeByString(key);

    return retType;
}

void IProtocolLoginAgentImp::ClearTcpAgent()
{
    m_tcpAgent->Clear();
}

 ProtocolLoginWork* IProtocolLoginAgentImp::WorkObject()
 {
    return m_pProtocolwork;
 }


 void IProtocolLoginAgentImp::DoExecLoginCmd(const QString &userName, const QString &password, int flag,int isforce)
 {
     // m_protocolThread.Sign()->LoginCmd( userName,password,1);
     m_tcpAgent->DoExecLoginCmd(userName, password, flag,isforce);
 }

void IProtocolLoginAgentImp::DoUpdateLoginToken(const QString& token)
{
    m_tcpAgent->DoUpdateLoginToken(token);
}

void IProtocolLoginAgentImp::R_VMConnected()
{
    int i = 0;
}

void IProtocolLoginAgentImp::DoExecVMAuthLogin()
{
    gVisitorMonitorLoginTcpAgent->DoExecVMAuthLogin();

}

void IProtocolLoginAgentImp::DoExecVMLogin(QString userId, QString companyId)
{
    gVisitorMonitorLoginTcpAgent->DoExecVMLogin(userId, companyId);
}

