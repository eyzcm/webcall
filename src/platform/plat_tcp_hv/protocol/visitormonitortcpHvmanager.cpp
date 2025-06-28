#include "VisitorMonitorTcpHvManager.h"
#include "utils/ConstStringHelper.h"
#include "utils/EL_Util_Time.h"
#include "protocol_biz/protocol_login_Hvagent.h"
#include "protocol_biz/VisitorMonitorLoginTcpHvAgent.h"
#include "session/el_sessionservice.h"
#include "tcpimp/vmtcp/ElVMStatusProcess.h"
#include "vm_keeplivecmd.h"
#include "service/util_service.h"

VisitorMonitorTcpHvManager* VisitorMonitorTcpHvManager::m_instance = nullptr;
VisitorMonitorTcpHvManager::VisitorMonitorTcpHvManager()
{
    m_timer  = NULL;
    m_callID = 0;
    m_tcpImp = NULL;
}

VisitorMonitorTcpHvManager::~VisitorMonitorTcpHvManager()
{
    if (m_timer)
    {
        m_timer->deleteLater();
        m_timer = NULL;
    }
}

VisitorMonitorTcpHvManager* VisitorMonitorTcpHvManager::Instance()
{
    if (nullptr == m_instance)
    {
        m_instance = new VisitorMonitorTcpHvManager;
    }
    return m_instance;
}

void VisitorMonitorTcpHvManager::CreateTcpConnect(QString host, QString port)
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

int VisitorMonitorTcpHvManager::GetNextCallId()
{
    return m_callID++;
}

TcpMessagePtr VisitorMonitorTcpHvManager::CreateNewResquest()
{
    TcpMessagePtr pMsg(new TcpMessage());
    QString     seq  = QString("seq_%1").arg(GetNextCallId());
    pMsg->Seq(seq);
    //添加进sip请求列表。
    m_reqList.append(pMsg);
    return pMsg;
}

void VisitorMonitorTcpHvManager::CreateTimer()
{

    if (m_timer == NULL)
    {
        m_timer = new QTimer;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(R_OnTimer()));
        m_timer->setInterval(65 * 1000);
        m_timer->start();
    }
}

void VisitorMonitorTcpHvManager::R_NetWorkError(int code, QString reason)
{
    Q_EMIT S_NetWorkError(code, reason);
}

void VisitorMonitorTcpHvManager::R_OnTimer()
{

    if (m_tcpImp->getStatus() != ElVmStatus::VMLOGINOK)
    {
        if (gVisitorMonitorLoginTcpHvAgent->isLoginMaxCount() == false)
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

void VisitorMonitorTcpHvManager::CloseTimer()
{
    if (m_timer)
    {
        m_timer->stop();
        m_timer->deleteLater();
        m_timer = NULL;
    }
}

void VisitorMonitorTcpHvManager::DisconnectTcp()
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

void VisitorMonitorTcpHvManager::SendTcpMessage(TcpMessagePtr msg)
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
            gIProtocolLoginHvAgent->DoTcpReqLog("MONITOR", QString::fromUtf8(msg->ToString()));
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

void VisitorMonitorTcpHvManager::R_ReceivedMessage(TcpMessagePtr pmsg)
{
	TcpMessage * msg = pmsg.data();
	GPROTOCOLLOG(QString("tcp recv MONITOR protocol:%1").arg(QString::fromUtf8(msg->ToString())), "MONITOR");
    //GPROTOCOLLOG(QString::fromUtf8(msg->ToString()), "MONITOR");
	
    gIProtocolLoginHvAgent->DoTcpResultLog("MONITOR", QString::fromUtf8(msg->ToString()));
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

TcpMessagePtr VisitorMonitorTcpHvManager::FindTcpMessage(TcpMessagePtr msg)
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

void VisitorMonitorTcpHvManager::LoginOk()
{
    m_tcpImp->LoginOk();
}
void VisitorMonitorTcpHvManager::R_connected()
{
    Q_EMIT s_test();

}
