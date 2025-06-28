#include "stdafx.h"
#include "tcpHvmanager.h"
#include "./utils/ConstStringHelper.h"
#include "protocol/ACKLiveCmd.h"
#include "utils/EL_Util_Time.h"
#include "protocol/keeplivecmd.h"
#include "tcpimp/maintcp/ElConnectStatusProcess.h"
#include "protocol_biz/protocol_login_Hvagent.h"
#include "session/el_sessionservice.h"

#include <qobject.h>
#include <QThread>
#include "service/util_service.h"

TcpHvManager *TcpHvManager::m_instance = nullptr;
TcpHvManager::TcpHvManager()
{
    m_timer  = NULL;
    m_callID = 0;
    m_tcpImp = NULL;
	m_poutSipParser = new TcpParser();
}

TcpHvManager::~TcpHvManager()
{
	if (m_poutSipParser)
	{
		m_poutSipParser->deleteLater();
		m_poutSipParser = nullptr;
	}
    if (m_timer)
    {
        m_timer->deleteLater();
        m_timer = NULL;
    }
    if (m_tcpImp)
    {
        // m_tcpImp->quit();
        // m_tcpImp->deleteLater();
        delete m_tcpImp;
        m_tcpImp = NULL;
    }
}

TcpHvManager *TcpHvManager::Instance()
{
    if (nullptr == m_instance)
    {
        m_instance = new TcpHvManager;
    }
    return m_instance;
}

void TcpHvManager::CreateTcpConnect(QString host, QString port)
{
    m_currCode = 0;
    m_currMsg  = "";
    ET("TcpHvManager::CreateTcpConnect");

    if (m_tcpImp)
    {
        m_tcpImp->Init(host, port.toInt(), ConstStringHelper::m_cTcpManager);
        m_tcpImp->connectservice();
    }
    else
    {
        m_tcpImp = ElConnectStatusProcess::getInstance();
        // m_tcpImp->setThreadName("tcp");
        connect(m_tcpImp, SIGNAL(S_connected()), this, SIGNAL(S_connected()), Qt::QueuedConnection);
        connect(m_tcpImp, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)), Qt::QueuedConnection);
        connect(m_tcpImp, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()), Qt::QueuedConnection);
        connect(m_tcpImp, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)), Qt::QueuedConnection);
        connect(m_tcpImp, SIGNAL(S_MainTcpLoginStateChanged(ELTCPlLOGINSTATUS, ELTCPlLOGINSTATUS)), gSessionService, SIGNAL(S_MainTcpLoginStateChanged(ELTCPlLOGINSTATUS, ELTCPlLOGINSTATUS)), Qt::QueuedConnection);
		connect(m_poutSipParser, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()));

		connect(m_poutSipParser, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
		

        m_tcpImp->Init(host, port.toInt(), ConstStringHelper::m_cTcpManager);
        m_tcpImp->connectservice();
        //	m_tcpImp->start();
    }
}

int TcpHvManager::GetNextCallId()
{
    return m_callID++;
}

TcpMessage *TcpHvManager::CreateNewResquest()
{
    TcpMessage *pMsg = new TcpMessage();
    QString     seq  = QString("seq_%1").arg(GetNextCallId());
    pMsg->Seq(seq);
    //添加进sip请求列表。
    m_reqList.append(pMsg);
    return pMsg;
}

void TcpHvManager::CreateTimer()
{
    if (m_timer == NULL)
    {
        m_timer = new QTimer;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(R_OnTimer()));
        m_timer->setInterval(30 * 1000);
        m_timer->start();
    }
    else
    {
        m_timer->start();
    }
}

void TcpHvManager::R_OnTimer()
{
    KeepLiveCmd req;
    req.MakeCmd();
    req.SendRequestToServer();
}

void TcpHvManager::CloseTimer()
{
    if (m_timer)
    {
        m_timer->stop();
        m_timer->deleteLater();
        m_timer = NULL;
    }
}

void TcpHvManager::ErrorLogin(QString msg)
{
    m_currCode = 109;
    m_currMsg  = msg;
    if (m_tcpImp)
        m_tcpImp->closeConnect();
}

void TcpHvManager::DisconnectTcp()
{
    // this->disconnect();
    //清除心跳
    // CloseTimer();
    //断开连接。
    /*
        if(m_tcpImp)
        {
                m_tcpImp->ClearConnectHost();
        }
    */
    if (m_tcpImp)
        m_tcpImp->closeConnect();
}

void    TcpHvManager::SendTcpMessage(QByteArray msg)
{
	if (m_tcpImp == NULL)
	{
		Q_ASSERT(false);
	}
	else
	{
		m_tcpImp->SendTcpMessage(msg);
	}
}

void TcpHvManager::SendTcpMessage(TcpMessage *msg)
{

    if (msg)
    {
        if (m_tcpImp == NULL)
        {
            Q_ASSERT(false);
        }
        else
        {
            // m_tcpImp->SendMessage( msg );

            //GPROTOCOLLOG(QString::fromUtf8(msg->ToString()), "CHAT");
            gIProtocolLoginHvAgent->DoTcpReqLog("CHAT", QString::fromUtf8(msg->ToString()));
            if (msg->Uri() == "/Auth")
            {
                m_tcpImp->TcpLogin(msg->ToString());
            }
            else
                m_tcpImp->SendTcpMessage(msg->ToString());
            // Q_EMIT S_SendTcpMessage(msg->ToString());
            if (!msg->HasResponse())
            {
                TcpMessage *obj = FindTcpMessage(msg);
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

void TcpHvManager::R_ReceivedMessage(TcpMessagePtr pmsg)
{
    
	TcpMessage * msg = pmsg.data();
    if (msg == nullptr)
    {
        return;
    }

    //GPROTOCOLLOG(QString::fromUtf8(msg->ToString()), "CHAT");
	GPROTOCOLLOG(QString("tcp recv protocol:%1").arg(QString::fromUtf8(msg->ToString())), "CHAT");

    gIProtocolLoginHvAgent->DoTcpResultLog("CHAT", QString::fromUtf8(msg->ToString()));

    TcpMessage *obj = FindTcpMessage(msg);
    if (obj == nullptr)
    {
        //是否需要回执
        if (msg->Uri() == "/im/message")
        {
            TcpMessage resp;
            resp.Seq(msg->Seq());
            resp.Uri("/im/message/success");
            resp.Body("OK");
            resp.HasResponse(false);
            Q_EMIT S_SendTcpMessage(resp.ToString());
        }
        else if (msg->Uri() == "/im/group/message" || msg->Uri() == "/im/group/event")
        {
            TcpMessage resp;
            resp.Seq(msg->Seq());
            resp.Uri("/im/group/message/success");
            resp.Body("OK");
            resp.HasResponse(false);
			
            Q_EMIT S_SendTcpMessage(resp.ToString());
        }
        else if ((msg->Uri() == "/im/data/visitorInfo") || (msg->Uri() == "/im/robot/stop") || (msg->Uri() == "/im/robot/start"))
        {
            TcpMessage resp;
            resp.Seq(msg->Seq());
            resp.Uri("/im/message/success");
            resp.Body("OK");
            resp.HasResponse(false);
            Q_EMIT S_SendTcpMessage(resp.ToString());
		}
		else   if (msg->Uri() == "/im/forceLogout")
		{

			if (m_tcpImp)
				m_tcpImp->ForceClose();
			//强制退出
			
		}
        
        {
    
			DEFTCPMESSAGE(tmsg, msg);
            Q_EMIT S_ReceivedMessage(tmsg);
        }
    }
    else
    {
        obj->SendMessageResponse(pmsg);
        m_reqList.removeOne(obj);
        obj->deleteLater();
        obj = nullptr;
    }
}

void TcpHvManager::R_NetWorkError(int code, QString reason)
{
    m_currCode = code;
    m_currMsg  = reason;
    if (code == 0)
    {
        return;
    }
    ET("TcpHvManager::R_NetWorkError");
    /*if (m_tcpImp)
        {
                m_tcpImp->deleteLater();
                m_tcpImp->quit();
    }*/

    ElConnectStatusProcess *sendObj = (ElConnectStatusProcess *)QObject::sender();
    if (sendObj == m_tcpImp)
    {
        GTESTLOG(QString("S_NetWorkError,%1,%2").arg(code).arg(reason));
        Q_EMIT S_NetWorkError(code, reason);
    }
}

TcpMessage *TcpHvManager::FindTcpMessage(TcpMessage *msg)
{
    if (msg == NULL)
    {
        return nullptr;
    }
    foreach (TcpMessage *obj, m_reqList)
    {
        if (obj->Seq() == msg->Seq())
        {
            return obj;
        }
    }
    return nullptr;
}

void TcpHvManager::ProcessAllNoEndTcpMessage()
{
    foreach (TcpMessage *obj, m_reqList)
    {
        if (obj->Uri() == "/Message")
        {
            QString chatId     = obj->GetPramsByName("chatId");
            QString callbackId = obj->GetPramsByName("callbackId");
            if (!chatId.isEmpty() && (callbackId.toInt() > 5000))
            {
                Q_EMIT S_UpdataeRecordId(chatId, callbackId, "-2");
            }
        }
    }
}

void TcpHvManager::LoginOk()
{
    m_tcpImp->LoginOk();
}

int TcpHvManager::GetErrorCode()
{
    return m_currCode;
}
QString TcpHvManager::GetErrorMsg()
{
    return m_currMsg;
}


void    TcpHvManager::ReceiveOutdataTcpMessage(QByteArray msg)
{
	if (m_poutSipParser)
		m_poutSipParser->Parse(msg);
}
