#include "protocol_login_HvagentImp.h"
#include "qdebug.h"

#include "./protocol_biz/tcploginHvagent.h"
#include "./protocol_biz/VisitorMonitorLoginTcpHvAgent.h"


#include "protocol/serviceaddressmanager.h"
#include "protocol_biz/protocol_login_Hvagent.h"
//#include "session/el_sessionservice.h"

#include "thread/protocolthread.h"
#include "utils/utilshelper.h"
#include "session/el_sessionService.h"

IProtocolLoginHvAgentImp *IProtocolLoginHvAgentImp::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new IProtocolLoginHvAgentImp;
    }
    return m_instance;
}

IProtocolLoginHvAgentImp::~IProtocolLoginHvAgentImp()
{
}

IProtocolLoginHvAgentImp::IProtocolLoginHvAgentImp()
{
    m_pProtocolwork      = nullptr;
   m_tcpAgent           = TcpLoginHvAgent::GetInstance();
    m_factoryBaseData    = new BaseConstructionData();
    m_currentChatType    = 1; //默认为增量方式
    m_ForceLogin         = 0;
}

IProtocolLoginHvAgentImp *IProtocolLoginHvAgentImp::m_instance = nullptr;

void IProtocolLoginHvAgentImp::Init()
{
    qDebug()<<"moveToThread IProtocolLoginHvAgentImp"<<endl;
    m_pProtocolwork = new ProtocolLoginHvWork();
    m_pProtocolwork->Protocol(this);
    //ServiceAddressManager::GetInstance()->Init(gSessionService->LoginData());
   gVisitorMonitorLoginTcpHvAgent->moveToThread(&m_protocolThread);

     connect(m_tcpAgent, SIGNAL(S_LoginFinished(QString, QString, QString)), this, SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)));
    connect(m_tcpAgent, SIGNAL(S_ForceLogout()), this, SIGNAL(S_ForceLogout()));
    connect(m_tcpAgent, SIGNAL(S_ForceReLogin(int)), this, SIGNAL(S_ForceReLogin(int)));
    connect(m_tcpAgent, SIGNAL(S_NetWorkError(int, QString)), this, SIGNAL(S_MainTcpNetWorkError(int, QString)));



    connect(gVisitorMonitorLoginTcpHvAgent, SIGNAL(S_connected()), this, SIGNAL(S_VMConnected()));
    connect(gVisitorMonitorLoginTcpHvAgent, SIGNAL(S_connected()), this, SLOT(R_VMConnected()));

    connect(gVisitorMonitorLoginTcpHvAgent, SIGNAL(S_VM_AuthFinished(bool, int)), this, SIGNAL(S_VM_AuthFinished(bool, int)));
    connect(gVisitorMonitorLoginTcpHvAgent, SIGNAL(S_VM_LoginFinished(QString)), this, SIGNAL(S_VM_LoginFinished(QString)));

    connect(gVisitorMonitorLoginTcpHvAgent, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_VMMessageParsingFailed()));
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

void IProtocolLoginHvAgentImp::InitTcpAgent(QString loginurl, QString port)
{
    m_tcpAgent->Init(loginurl, port);
}

void IProtocolLoginHvAgentImp::InitVisitorMonitorTcpAgent(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber)
{
    gVisitorMonitorLoginTcpHvAgent->Init(loginurl, port, userName, companyId, monitorvisitornumber);
}


void IProtocolLoginHvAgentImp::DoLoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState)
{
    m_tcpAgent->R_SetCurrentLoginStatus(oldState, newState);
    gVisitorMonitorLoginTcpHvAgent->R_SetCurrentLoginStatus(oldState, newState);
}

ECLIENTTYPE IProtocolLoginHvAgentImp::GetClientType()
{

    QString     key     = "6";// gUISystemConfigManager->GetSystemConfigByKey("ismedical", "0");
    ECLIENTTYPE retType = UtilsHelper::GetClientTypeByString(key);

    return retType;
}

void IProtocolLoginHvAgentImp::ClearTcpAgent()
{
    m_tcpAgent->Clear();
}

 ProtocolLoginHvWork* IProtocolLoginHvAgentImp::WorkObject()
 {
    return m_pProtocolwork;
 }


 void IProtocolLoginHvAgentImp::DoExecLoginCmd(const QString &userName, const QString &password, int flag,int isforce)
 {
     // m_protocolThread.Sign()->LoginCmd( userName,password,1);
     m_tcpAgent->DoExecLoginCmd(userName, password, flag,isforce);
 }

void IProtocolLoginHvAgentImp::DoUpdateLoginToken(const QString& token)
{
    m_tcpAgent->DoUpdateLoginToken(token);
}

void IProtocolLoginHvAgentImp::R_VMConnected()
{
    int i = 0;
}

void IProtocolLoginHvAgentImp::DoExecVMAuthLogin()
{
    gVisitorMonitorLoginTcpHvAgent->DoExecVMAuthLogin();

}

void IProtocolLoginHvAgentImp::DoExecVMLogin(QString userId, QString companyId)
{
    gVisitorMonitorLoginTcpHvAgent->DoExecVMLogin(userId, companyId);
}

