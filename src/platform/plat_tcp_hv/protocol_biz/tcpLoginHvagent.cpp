#include "tcploginHvagent.h"
#include "service/util_service.h"
#include "utils/EL_Util_Time.h"


#include "protocol/serviceaddressmanager.h"
#include "protocol/tcpHvmanager.h"
#include "session/el_sessionservice.h"

#include <protocol/logincmd.h>
#include <protocol/offlinemessagecmd.h>
#include "service/util_service.h"


TcpLoginHvAgent::TcpLoginHvAgent() : QObject()
{
    m_isTcpManager       = false;
    m_currentLoginStatus = E_LOGIN_NONE;
    m_retryLoginCount    = 0;
    m_isConnect          = false;
    m_isLogin            = false;
    m_ForceLogout        = false;
    m_setRetryLogin      = 0;
    m_loginType          = 0;
    m_reLoginFlag        = 0;
    m_reloginTimer       = nullptr;
    m_loginCount         = 0;
}

TcpLoginHvAgent *TcpLoginHvAgent::m_instance = nullptr;


TcpLoginHvAgent::~TcpLoginHvAgent()
{

}

void TcpLoginHvAgent::Init(QString loginurl, QString port)
{
    m_isLastAddressInfo = false;
    m_loginUrl          = loginurl;
    m_port              = port;
    CreateTcpConnect(loginurl, port);
}

void TcpLoginHvAgent::CreateTcpConnect(QString loginurl, QString port)
{
    m_isConnect = false;
    m_loginUrl  = loginurl;
    m_port      = port;
    gTcpHvManager->DisconnectTcp();
    if (m_isTcpManager == false)
    {
        m_isTcpManager = true;
        connect(gTcpHvManager, SIGNAL(S_connected()), this, SLOT(R_connected()));
        connect(gTcpHvManager, SIGNAL(S_MessageParsingFailed()), this, SLOT(R_MessageParsingFailed()));
        connect(gTcpHvManager, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));
        connect(gTcpHvManager, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
        connect(gTcpHvManager, SIGNAL(S_UpdataeRecordId(QString, QString, QString)), this, SIGNAL(S_UpdataeRecordId(QString, QString, QString)));
    }
    addressinfo *info = ServiceAddressManager::GetInstance()->GetAddressInfo();
    if (info == nullptr)
    {
        m_loginCount++;
        if (m_loginCount % 5 != 3)
        {
            ServiceAddressManager::GetInstance()->ResetServiceList(false);
            info = ServiceAddressManager::GetInstance()->GetAddressInfo();
        }
    }
    if (info)
    {
        gTcpHvManager->CreateTcpConnect(info->m_host, info->m_port);
    }
    else
    {
        m_isLastAddressInfo = true;
        gTcpHvManager->CreateTcpConnect(loginurl, port);
    }
}

void TcpLoginHvAgent::Clear()
{

}

void TcpLoginHvAgent::TcpReconnect()
{
    if (m_ForceLogout == false)
        CreateTcpConnect(m_loginUrl, m_port);
}


TcpLoginHvAgent *TcpLoginHvAgent::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new TcpLoginHvAgent;
    }
    return m_instance;
}


void TcpLoginHvAgent::R_SetCurrentLoginStatus(ELOGINSTATUS oldState, ELOGINSTATUS newState)
{
    m_currentLoginStatus = newState;

    switch (newState)
    {
        case E_LOGIN_DOING:
            break;
        case E_LOGIN_ALLOK:
            m_isLastAddressInfo = false;
            break;
        case E_LOGIN_RETRY_MAINTCP:
        {
            gTcpHvManager->CreateTimer();

            if (m_setRetryLogin == 0)
            {
                if (m_retryLoginCount > 5)
                {
                    m_setRetryLogin = 1;
                    QTimer::singleShot(60 * 1000, this, SLOT(ReTryLoginConnect()));
                }
                else if (m_retryLoginCount > 3 && m_retryLoginCount < 5)
                {
                    m_setRetryLogin = 1;
                    QTimer::singleShot(30 * 1000, this, SLOT(ReTryLoginConnect()));
                }
                else
                {
                    m_setRetryLogin = 1;
                    QTimer::singleShot(10 * 1000, this, SLOT(ReTryLoginConnect()));
                }

                m_retryLoginCount++;
            }
        }
        break;
        case E_LOGIN_OK_MAINTCP:
        {
            gTcpHvManager->CreateTimer();


        }
        break;
        default:

            break;
    }
}


void TcpLoginHvAgent::DoExecLoginCmd(const QString &userName, const QString &password, int flag,int isForce)
{
    m_loginType = flag;
    m_ForceLogin = isForce;
    m_isLogin   = true;
    m_userName  = userName;
    m_password   = password;

    if (m_isConnect)
    {
        LoginCmd *req = new LoginCmd;

        connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecLoginCmdResult(TcpMessagePtr)));
        EPRESENCESTATUS ePresence = E_ONLINE;
        req->Version(gSessionService->Version());
        if ((gSessionService->LoginData()) && (!gSessionService->LoginData()->UserName().isEmpty()))
        {
            int state = gSessionService->LoginData()->UserState();
            if (state == 0)
            {
                ePresence = E_ONLINE;
            }
            else if (state == 1)
            {
                ePresence = E_BUSY;
            }
            else if (state == 2)
            {
                ePresence = E_LEAVE;
            }
            else
            {
                ePresence = E_ONLINE;
            }
        }
        req->Init(userName, m_password, m_loginUrl, m_port);
        req->LoginType(flag);
        req->IsForce(isForce );
        m_loginType = flag;
        m_ForceLogin = m_loginType;

        req->LoginStatus(ePresence);
        req->IsFriendData(1);
        req->HasResponse(true);
        req->MakeCmd();
        req->SendRequestToServer();
    }
}

void TcpLoginHvAgent::DoExecOfflineMessageCmd()
{
    OfflineMessageCmd *req = new OfflineMessageCmd;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecOfflineMessageCmdResult(TcpMessagePtr)));
    req->MakeCmd();
    req->SendRequestToServer();
}

void TcpLoginHvAgent::DoUpdateLoginToken(const QString& token)
{

}


void TcpLoginHvAgent::R_connected()
{
    ET("TcpAgent::R_connected();");
    m_isConnect       = true;
    m_retryLoginCount = 0;
    DoExecLoginCmd(m_userName, m_password, m_loginType,m_ForceLogin);
    Q_EMIT S_connected();
}

void TcpLoginHvAgent::R_MessageParsingFailed()
{
    Q_EMIT S_MessageParsingFailed();
}

void TcpLoginHvAgent::R_ReceivedMessage(TcpMessagePtr msg)
{
    

    if (msg == nullptr)
    {
        Q_ASSERT(false);
        return;
    }
    QObject *obj = QObject::sender();



    if (msg->Uri() == "/im/reLogin")
        {
            QString forcestr = msg->GetPramsByName("force");
            int     force    = 1;
            if (forcestr == "false")
                force = 0;
            if (force == 1)
            {
                TcpReconnect();
            }
            else
            {
                if (m_reloginTimer == nullptr)
                {
                    m_reloginTimer = new QTimer;

                    int ltime = 60 + (GetTickCount() % (9 * 60));
                    m_reloginTimer->setInterval(ltime * 1000);
                    connect(m_reloginTimer, SIGNAL(timeout()), SLOT(R_ReloginTimeOut()));
                    m_reloginTimer->start();
                }
                m_reLoginFlag = 1;
            }
	
			
      }
        else if (msg->Uri() == "/im/forceLogout")
        {
			GLOG( "R_ReceivedMessage /im/forceLogout" );
            m_ForceLogout = true;
            Q_EMIT S_ForceLogout();
        }
		else if (msg->Uri() == "/im/keepAlive")
		{
			TcpReconnect();

		}
   #if 0
    //收到消息
    // todo shizhenyu
    if (msg->Uri() == "/im/message")
    {
        if (m_messageReqList.indexOf(msg->Seq()) == -1)
        {
            //收到消息
            QString body = msg->Body();
            if (msg->GetHeaderByName("Content-Type") == "json")
            {
                //使用json解析。
                Q_EMIT S_Received_Im_Message(body);
            }
            else
            {
                Q_ASSERT(false);
            }
            m_messageReqList.push_back(msg->Seq());
        }
        if (m_messageReqList.size() > 100)
        {
            m_messageReqList.remove(0);
        }
    }
    else if (msg->Uri() == "/im/queue")
    {
        QString body = msg->Body();
        if (msg->GetHeaderByName("Content-Type") == "json")
        {
            //使用json解析。
            Q_EMIT S_Received_Im_Queue(body);
        }
        else
        {
            Q_ASSERT(false);
        }
    }
    else if (msg->Uri() == "/im/control")
    {
        QString body = msg->Body();
        Q_EMIT S_Received_Invite_Response(body);
    }
    else
    else if (msg->Uri() == "/im/group/message")
    {
        if (m_groupMessageReqList.indexOf(msg->Seq()) == -1)
        {
            //群消息
            QString body = msg->Body();
            if (msg->GetHeaderByName("Content-Type") == "json")
            {
                //使用json解析。
                Q_EMIT S_Received_Im_Group_Message(body);
            }
            else
            {
                Q_ASSERT(false);
            }
            m_groupMessageReqList.push_back(msg->Seq());
        }
        if (m_groupMessageReqList.size() > 100)
        {
            m_messageReqList.remove(0);
        }
    }
    else if (msg->Uri() == "/im/group/event")
    {
        // 6.5	删除群成员
        QString body = msg->Body();
        if (msg->GetHeaderByName("Content-Type") == "json")
        {
            //使用json解析。
            Q_EMIT S_Received_Im_Group_Event(body);
        }
        else
        {
            Q_ASSERT(false);
        }
    }
    else if (msg->Uri() == "/im/ai")
    {
        QString body = msg->Body();
        Q_EMIT S_Received_Im_AI_Message(body);
    }
    else if (msg->Uri() == "/im/robot/stop")
    {
        QString chatId = msg->GetPramsByName("chatId");

        Q_EMIT S_Received_Im_AI_Robot_Message(chatId);
    }
    else if (msg->Uri() == "/im/keepAlive/success")
    {
        QString body   = msg->Body();
        QString header = msg->GetHeaderByName("Error");
        Q_EMIT S_Received_KeepAlive_Event(body);
    }
    else if (msg->Uri() == "/im/data/visitorInfo")
    {
        QString body   = msg->Body();
        QString header = msg->GetHeaderByName("Error");

        Q_EMIT S_Received_Im_Data_VisitorInfo(body);
    }

    if (msg != nullptr)
    {
        msg->deleteLater();
        msg = nullptr;
    }
   #endif
}

void TcpLoginHvAgent::R_NetWorkError(int errorCode, QString msg)
{
    if (m_currentLoginStatus == E_LOGIN_DOING)
    {
        if (m_isLastAddressInfo)
        {
            Q_EMIT S_NetWorkError(errorCode, msg);
            if (m_currentLoginStatus == E_LOGIN_RETRY_MAINTCP)
            {
                if (m_setRetryLogin == 0)
                {
                    m_setRetryLogin = 1;
                    if (m_retryLoginCount > 5)
                    {
                        QTimer::singleShot(60 * 1000, this, SLOT(ReTryLoginConnect()));
                    }
                    else if (m_retryLoginCount > 3 && m_retryLoginCount < 5)
                    {
                        QTimer::singleShot(30 * 1000, this, SLOT(ReTryLoginConnect()));
                    }
                    else
                    {
                        QTimer::singleShot(10 * 1000, this, SLOT(ReTryLoginConnect()));
                    }
                }
                m_retryLoginCount++;
            }
        }
        else
        {
            CreateTcpConnect(m_loginUrl, m_port);
        }
    }
    else if (m_currentLoginStatus == E_LOGIN_RETRY_MAINTCP)
    {
        if (m_isLastAddressInfo)
        {
            Q_EMIT S_NetWorkError(errorCode, msg);

            if (m_setRetryLogin == 0)
            {
                m_setRetryLogin = 1;
                if (m_retryLoginCount > 5)
                {
                    QTimer::singleShot(60 * 1000, this, SLOT(ReTryLoginConnect()));
                }
                else if (m_retryLoginCount > 3 && m_retryLoginCount < 5)
                {
                    QTimer::singleShot(30 * 1000, this, SLOT(ReTryLoginConnect()));
                }
                else
                {
                    QTimer::singleShot(10 * 1000, this, SLOT(ReTryLoginConnect()));
                }
                m_retryLoginCount++;
            }
        }
        else
        {
            CreateTcpConnect(m_loginUrl, m_port);
        }
    }
    else if (m_currentLoginStatus == E_LOGIN_ALLOK)
    {
        gTcpHvManager->ProcessAllNoEndTcpMessage();

        Q_EMIT S_NetWorkError(errorCode, msg);
    }
    else if (m_currentLoginStatus == E_LOGIN_OK_MAINTCP)
    {
        /*
        foreach (SendMsgCmd *obj, m_offlineMsgList)
        {
            if (obj == nullptr)
            {
                continue;
            }
            if ((!obj->ChatId().isEmpty()) && (obj->CallbackId() > 5000))
            {
                //发送失败
                Q_EMIT S_UpdataeRecordId(obj->ChatId(), QString("%1").arg(obj->CallbackId()), "-2");
            }
        }*/

        Q_EMIT S_NetWorkError(errorCode, msg);
    }
    else
    {
        Q_EMIT S_NetWorkError(errorCode, msg);
    }
}

void TcpLoginHvAgent::ReTryLoginConnect()
{
    m_setRetryLogin = 0;
    if (m_ForceLogout == false)
        CreateTcpConnect(m_loginUrl, m_port);
}

void TcpLoginHvAgent::R_TokenError()
{
    Q_EMIT S_NetWorkError(400, "token error");
}


void TcpLoginHvAgent::R_ReloginTimeOut()
{
    if (m_reLoginFlag)
    {
        m_reLoginFlag = 0;
     //   if (gUIChatServiceManager->GetChatCount() == 0)
        {
            TcpReconnect();
        }
    }
}


void TcpLoginHvAgent::R_DoExecLoginCmdResult(TcpMessagePtr msg)
{

    ET("TcpAgent::R_DoExecLoginCmdResult");
    LoginCmd* obj = (LoginCmd*) QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }

    m_reLoginFlag = 0;
    QString servers = msg->GetHeaderByName("Servers");
    QString monitorServers = msg->GetHeaderByName("Monitor-Servers");

    ServiceAddressManager::GetInstance()->SetServiceList(servers, 0);
    ServiceAddressManager::GetInstance()->SetServiceList(monitorServers, 1);
    QString errorCode = msg->GetHeaderByName("Error");
    QString authType = msg->GetHeaderByName("Auth");
    QString msgBody = msg->Body();
    if( authType == "error")
    {
        gTcpHvManager->ErrorLogin( msgBody );

    }
    Q_EMIT S_LoginFinished(authType, errorCode, msgBody);


    if (msg)
    {
        msg->deleteLater();
        msg = nullptr;
    }
}


