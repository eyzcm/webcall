#include "VisitorMonitorTcpManager.h"
#include "utils/ConstStringHelper.h"
#include "utils/EL_Util_Time.h"
#include "protocol_biz/protocol_login_agent.h"
#include "protocol_biz/VisitorMonitorLoginTcpAgent.h"
#include "session/el_sessionservice.h"
#include "tcpimp/vmtcp/ElVMStatusProcess.h"
#include "vm_keeplivecmd.h"
#include "service/util_service.h"

VisitorMonitorTcpManager* VisitorMonitorTcpManager::m_instance = nullptr;
VisitorMonitorTcpManager::VisitorMonitorTcpManager()
{
    m_timer  = NULL;
    m_callID = 0;
    m_tcpImp = NULL;
}

VisitorMonitorTcpManager::~VisitorMonitorTcpManager()
{
    if (m_timer)
    {
        m_timer->deleteLater();
        m_timer = NULL;
    }
}

VisitorMonitorTcpManager* VisitorMonitorTcpManager::Instance()
{
    if (nullptr == m_instance)
    {
        m_instance = new VisitorMonitorTcpManager;
    }
    return m_instance;
}

void VisitorMonitorTcpManager::CreateTcpConnect(QString host, QString port)
{
    QMutexLocker locker(&m_mutex);
    if (m_tcpImp)
    {
        m_tcpImp->InitVM(host, port.toInt(), ConstStringHelper::m_cVisitorMonitorTcpManager);
        m_tcpImp->connectVMservice();
    }
    else
    {

        m_tcpImp = ElVmStatusProcess::getInstance();

        connect(m_tcpImp, SIGNAL(S_connected()), this, SIGNAL(S_connected()));

         connect(m_tcpImp, SIGNAL(S_connected()), this, SLOT(R_connected()));
        connect(m_tcpImp, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)), Qt::QueuedConnection);
        connect(m_tcpImp, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()), Qt::QueuedConnection);
        connect(m_tcpImp, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
        // m_tcpImp->start();
        m_tcpImp->InitVM(host, port.toInt(), ConstStringHelper::m_cVisitorMonitorTcpManager);

        m_tcpImp->connectVMservice();
    }
}

int VisitorMonitorTcpManager::GetNextCallId()
{
    return m_callID++;
}

TcpMessagePtr VisitorMonitorTcpManager::CreateNewResquest()
{
    TcpMessagePtr pMsg(new TcpMessage());
    QString     seq  = QString("seq_%1").arg(GetNextCallId());
    pMsg->Seq(seq);
    //添加进sip请求列表。
    m_reqList.append(pMsg);
    return pMsg;
}

void VisitorMonitorTcpManager::CreateTimer()
{

    if (m_timer == NULL)
    {
        m_timer = new QTimer;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(R_OnTimer()));
        m_timer->setInterval(65 * 1000);
        m_timer->start();
    }
}

void VisitorMonitorTcpManager::R_NetWorkError(int code, QString reason)
{
    Q_EMIT S_NetWorkError(code, reason);
}

void VisitorMonitorTcpManager::R_OnTimer()
{

    if (m_tcpImp->getStatus() != ElVmStatus::VMLOGINOK)
    {
        if (gVisitorMonitorLoginTcpAgent->isLoginMaxCount() == false)
        {
            GLOG(QString("testreconnect %1").arg(m_tcpImp->getStatus()));
            if (gSessionService->Status() >= E_LOGIN_OK_MAINTCP)
                m_tcpImp->connectVMservice();
        }
    }
    /*
    VM_KeepLiveCmd req;
    req.MakeCmd();
    req.SendRequestToServer();
    */
}

void VisitorMonitorTcpManager::CloseTimer()
{
    if (m_timer)
    {
        m_timer->stop();
        m_timer->deleteLater();
        m_timer = NULL;
    }
}

void VisitorMonitorTcpManager::DisconnectTcp()
{
    QMutexLocker locker(&m_mutex);
    //清除心跳
    CloseTimer();
    //断开连接。

    if (m_tcpImp)
    {

        m_tcpImp->closeVMservice();
    }
}

void VisitorMonitorTcpManager::SendTcpMessage(TcpMessagePtr msg)
{


    if (msg)
    {
        if (m_tcpImp == NULL)
        {
            ; // Q_ASSERT( false );
        }
        else
        {
            // m_tcpImp->SendMessage( msg );
            
			GPROTOCOLLOG(QString("tcp recv MONITOR protocol:%1").arg(QString::fromUtf8(msg->ToString())), "MONITOR");
            gIProtocolLoginAgent->DoTcpReqLog("MONITOR", QString::fromUtf8(msg->ToString()));
            if (msg->Uri() == "/auth")
            {
                if (msg->GetPramsByName("token").isEmpty())
                {
                    m_tcpImp->VMAuth(msg->ToString());
                }
                else
                    m_tcpImp->VMLogin(msg->ToString());
            }
            else
                m_tcpImp->VMSendTcpMessage(msg->ToString());

            if (!msg->HasResponse())
            {
                TcpMessagePtr obj = FindTcpMessage(msg);
                if (obj != nullptr)
                {
                    m_reqList.removeOne(obj);
                    obj->deleteLater();
                    obj = nullptr;
                }
            }
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

void VisitorMonitorTcpManager::R_ReceivedMessage(TcpMessagePtr pmsg)
{
	TcpMessage * msg = pmsg.data();
	GPROTOCOLLOG(QString("tcp recv MONITOR protocol:%1").arg(QString::fromUtf8(msg->ToString())), "MONITOR");
    //GPROTOCOLLOG(QString::fromUtf8(msg->ToString()), "MONITOR");
	
    gIProtocolLoginAgent->DoTcpResultLog("MONITOR", QString::fromUtf8(msg->ToString()));
    TcpMessagePtr obj = FindTcpMessage(pmsg);
    if (obj == nullptr)
    {
		DEFTCPMESSAGE(tmsg, msg);
        Q_EMIT S_ReceivedMessage(tmsg);
    }
    else
    {
        obj->SendMessageResponse(pmsg);
        m_reqList.removeOne(obj);
        obj->deleteLater();
        obj = nullptr;
    }
}

TcpMessagePtr VisitorMonitorTcpManager::FindTcpMessage(TcpMessagePtr msg)
{
    if (msg == NULL)
    {
        return nullptr;
    }
    foreach (TcpMessagePtr obj, m_reqList)
    {
        if (obj->Seq() == msg->Seq())
        {
            return obj;
        }
    }
    return nullptr;
}

void VisitorMonitorTcpManager::LoginOk()
{
    m_tcpImp->LoginOk();
}
void VisitorMonitorTcpManager::R_connected()
{
    Q_EMIT s_test();

}
