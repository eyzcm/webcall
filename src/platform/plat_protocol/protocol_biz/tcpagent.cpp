#include "tcpagent.h"
#include "utils/EL_Util_Time.h"
#include "service/dbuiservice.h"
#include "include/json/reader.h"
#include "include/json/value.h"
#include "utils/jsonhelper.h"

#include "protocol/AutoMonitor.h"
#include "protocol/CommChatMessageCmd.h"
#include "protocol/ConversationCmd.h"
#include "protocol/ConversationCmdEx.h"
#include "protocol/EventCustomCmd.h"
#include "protocol/VisitorEvaluationCmd.h"
#include "protocol/httpProxyReqCmd.h"
#include "protocol/httpcmd.h"
#include "protocol/offlinemessagecmd.h"
#include "protocol/sendmsgcmd.h"
#include "protocol/serviceaddressmanager.h"
#include "protocol/PlatProtocolService.h"
#include "protocol/UpdateLastMessageTime.h"
#include "service/util_service.h"
#include "session/el_sessionservice.h"

#include "data/UIVisitorUploadDataInfo.h"

#include <protocol/logincmd.h>
#include "protocol/PlatProtocolService.h"

TcpAgent::TcpAgent() : QObject()
{
    m_isTcpManager       = false;
    m_currentLoginStatus = E_LOGIN_NONE;
    m_retryLoginCount    = 0;
    m_isConnect          = false;
    m_isLogin            = false;


	//
    
}

TcpAgent *TcpAgent::m_instance = nullptr;

void TcpAgent::Init(QString loginurl, QString port)
{
    m_isLastAddressInfo = false;
    m_loginUrl          = loginurl;
    m_port              = port;
    CreateTcpConnect(loginurl, port);
}

void TcpAgent::CreateTcpConnect(QString loginurl, QString port)
{
	static int init = 0;
	if (init == 0)
	{
		init = 1;
		connect(gTcpManager, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));
        connect(gTcpManager, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
		connect(gTcpManager, SIGNAL(S_UpdataeRecordId(QString, QString, QString)), this, SIGNAL(S_UpdataeRecordId(QString, QString, QString)));
		connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
		m_currentLoginStatus = E_LOGIN_OK_MAINTCP;

	}


#if 0
    m_isConnect = false;
    m_loginUrl  = loginurl;
    m_port      = port;
    gTcpManager->DisconnectTcp();
    if (m_isTcpManager == false)
    {
        m_isTcpManager = true;
       // connect(gTcpManager, SIGNAL(S_connected()), this, SLOT(R_connected()));
        //connect(gTcpManager, SIGNAL(S_MessageParsingFailed()), this, SLOT(R_MessageParsingFailed()));
        connect(gTcpManager, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));
        connect(gTcpManager, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
        connect(gTcpManager, SIGNAL(S_UpdataeRecordId(QString, QString, QString)), this, SIGNAL(S_UpdataeRecordId(QString, QString, QString)));
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
        gTcpManager->CreateTcpConnect(info->m_host, info->m_port);
    }
    else
    {
        m_isLastAddressInfo = true;
        gTcpManager->CreateTcpConnect(loginurl, port);
    }
#endif
}

void TcpAgent::Clear()
{
    foreach (SendMsgCmd *obj, m_offlineMsgList)
    {
        if (obj != nullptr)
        {
            obj->deleteLater();
            obj = nullptr;
        }
    }
    m_offlineMsgList.clear();

    m_isLastAddressInfo = false;
    ServiceAddressManager::GetInstance()->ResetServiceList(false);
    this->disconnect();
}


void TcpAgent::DoExecCommChatMessageCmd(const QString &chatid, ECHATACTION action, QString vid)
{
    CommChatMessageCmd req ;
    //connect(req, SIGNAL(S_Finished(TcpMessage *)), this, SLOT(R_DoExecCommChatMessageCmdResult(TcpMessage *)));
    req.ChatAction(action);
    req.ChatId(chatid);
    req.VisitorId(vid);
    req.MakeCmd();
    req.SendRequestToServer();
}

void TcpAgent::DoExecInviteText(const QString &userid, const QString &viewId, const QString &InviteText)
{
    QString tInviteText = InviteText.toUtf8().toPercentEncoding();

    ConversationCmdEx *req = new ConversationCmdEx;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecConversationCmdResult(TcpMessagePtr)));
    req->UserId(userid);
    req->ViewId(viewId);
    req->ConversationsType("invite");
    req->ExInfo()->SetValueByKey("inviteText", tInviteText);
    req->MakeCmd();
    req->SendRequestToServer();
}

void TcpAgent::DoExecConversationCmd(const QString &userid, const QString &viewId, const QString &conversationType)
{
    ConversationCmd *req = new ConversationCmd;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecConversationCmdResult(TcpMessagePtr)));
    req->UserId(userid);
    req->ViewId(viewId);
    req->ConversationsType(conversationType);
    req->MakeCmd();
    req->SendRequestToServer();
}

void TcpAgent::DoExecOfflineMessageCmd()
{
    OfflineMessageCmd *req = new OfflineMessageCmd;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecOfflineMessageCmdResult(TcpMessagePtr)));
    req->MakeCmd();
    req->SendRequestToServer();
}

void TcpAgent::DoExecSetUserStateCmd(int state)
{
    TcpCmd  * req = new TcpCmd();
    QString uri = QString("/im/status?s=%1").arg(state);
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoSetUserStateCmdResult(TcpMessagePtr))); //发送文字消息 完成 4
    req->Uri(uri);
	req->HasResponse(true);
    req->Body("setStatus");
    req->SendRequestToServer();
}

void TcpAgent::DoExecSendAcceptActionCmd(QString groupId, QString vid)
{
    TcpCmd  req;
    QString uri = QString("/im/control?gId=%1&type=%2&viewId=%3").arg(groupId).arg("acceptQueue").arg(vid);
    req.Uri(uri);
    req.SendRequestToServer();
}

void TcpAgent::DoExecSendMonitorChatCmd(QString chatid)
{
    TcpCmd  req;
    QString uri = QString("/im/control?type=%1&chatId=%2").arg("joinToMonitorChat").arg(chatid);
    req.Uri(uri);
    req.SendRequestToServer();
}

void TcpAgent::DoExecTakeoverChatCmd(QString chatid, QString groupId, QString customeId, int flag, QMap<QString, QString> map)
{
    QString cmdexe = "takeover";
    if (flag == CHAT_TAKEOVER_MUCHGUIDE)
        cmdexe = "takeoverMuchGuide";
    TcpCmd  req;
    QString uri = QString("/im/control?type=%1&chatId=%2&flag=%3").arg(cmdexe).arg(chatid).arg(flag);

    if (!groupId.isEmpty())
    {
        uri.append(QString("&toGroupId=%1").arg(groupId));
    }
    if (map.find("configId") != map.end())
    {
        uri.append(QString("&configId=%1").arg(map["configId"]));
    }
    if (map.find("configType") != map.end())
    {
        uri.append(QString("&configType=%1").arg(map["configType"]));
    }
    if (map.find("groupId") != map.end())
    {
        uri.append(QString("&groupId=%1").arg(map["groupId"]));
    }
    if (!customeId.isEmpty())
    {
        if (flag == CHAT_TAKEOVER_WHIRL)
        {
            uri.append(QString("&takeoverUserId=%1").arg(customeId));
        }
        else
        {
            uri.append(QString("&preCustomerId=%1").arg(customeId));
        }
    }
    else
    {
        if (flag == CHAT_TAKEOVER_WHIRL)
        {
            uri.append(QString("&takeoverUserId=%1").arg(gSessionService->UserInfo()->UserId()));
        }
    }
    req.Uri(uri);
    req.SendRequestToServer();
}

void TcpAgent::DoExecSendMsgCmd(const QString &msgBody, const QString &fileName, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map)
{
    SendMsgCmd *req = new SendMsgCmd;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecSendMsgCmdResult(TcpMessagePtr))); //发送文字消息 完成 4
    req->HasResponse(true);
    req->Robot("0");
    req->RecorderId(recordId);
    req->MsgTo(msgTo);
    req->Body(msgBody);
    req->MsgType(msgType);
    req->UserType(userType);
    req->FileName(fileName);
    req->CallbackId(callbackId);
    if (map.count() > 0)
       {
           req->ExtMap(map);
       }
    req->MakeCmd();
    if (m_currentLoginStatus == E_LOGIN_OK_MAINTCP || m_currentLoginStatus == E_LOGIN_ALLOK || m_currentLoginStatus == E_LOGIN_PRE_MONITORTCP || m_currentLoginStatus == E_LOGIN_RETRY_MONITORTCP)
    {
        req->SendRequestToServer();
    }
    else
    {
        m_offlineMsgList.push_back(req);
    }
}

void TcpAgent::DoExecVisitorEvaluationCmd(const QString &chatId)
{
    VisitorEvaluationCmd *req = new VisitorEvaluationCmd;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecVisitorEvaluationCmdResult(TcpMessagePtr)));
    req->ChatId(chatId);
    req->MakeCmd();
    req->SendRequestToServer();
}

void TcpAgent::R_DoExecCommChatMessageCmdResult(TcpMessagePtr message)
{
    CommChatMessageCmd *obj = (CommChatMessageCmd *)QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }
    if (message)
    {
        message->deleteLater();
        message = nullptr;
    }
}

void TcpAgent::R_DoExecConversationCmdResult(TcpMessagePtr message)
{
    ConversationCmd *obj = (ConversationCmd *)QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }
    if (message)
    {
        message->deleteLater();
        message = nullptr;
    }
}


void TcpAgent::R_DoExecLoginCmdResult(TcpMessagePtr msg)
{
    ET("TcpAgent::R_DoExecLoginCmdResult");
    LoginCmd *obj = (LoginCmd *)QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }
    
    QString servers        = msg->GetHeaderByName("Servers");
    QString monitorServers = msg->GetHeaderByName("Monitor-Servers");
    ServiceAddressManager::GetInstance()->SetServiceList(servers, 0);
    ServiceAddressManager::GetInstance()->SetServiceList(monitorServers, 1);
    QString errorCode = msg->GetHeaderByName("Error");
    QString authType  = msg->GetHeaderByName("Auth");
    QString msgBody   = msg->Body();
    Q_EMIT S_LoginFinished(authType, errorCode, msgBody);

    if (authType == "error")
    {
        gTcpManager->ErrorLogin(msgBody);
    }
    if (msg)
    {
        msg->deleteLater();
        msg = nullptr;
    }
}

void TcpAgent::R_DoExecOfflineMessageCmdResult(TcpMessagePtr message)
{
    OfflineMessageCmd *obj = (OfflineMessageCmd *)QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }
    if (message)
    {
        message->deleteLater();
        message = nullptr;
    }
}

void TcpAgent::R_DoExecSendMsgCmdResult(TcpMessagePtr msg) //发送文字消息 完成 5
{
    SendMsgCmd *obj = (SendMsgCmd *)QObject::sender();
    if (msg == nullptr)
    {
        if (obj != nullptr)
        {
            obj->deleteLater();
            obj = nullptr;
        }
        Q_ASSERT(false);
        return;
    }
    QString errorCode = msg->GetHeaderByName("Error");
    if (errorCode == "600")
    {
        //重新登录
        if (obj != nullptr)
        {
            obj->deleteLater();
            obj = nullptr;

			//重新登录
            gIProtocolLoginAgent->DoTcpReconnect();
        }
        Q_ASSERT(false);
        return;
    }

    if (msg->Uri() == "/Message/success")
    {
        QMap<QString, QString> map = msg->GetHeaderList();
        if (map.find("callbackId") != map.end())
        {
            QString callbackId = map["callbackId"];
            QString recorderId = map["recorderId"];
            QString chatId     = map["chatId"];

            if ((!callbackId.isEmpty()) && (!recorderId.isEmpty()) && (!chatId.isEmpty()))
            {
                if (callbackId.toInt() > 5000)
                    Q_EMIT S_UpdataeRecordId(chatId, callbackId, recorderId); //发送文字消息 完成 6
            }
        }
        if (obj != nullptr)
        {
            obj->deleteLater();
            obj = nullptr;
        }

    }
    else if (msg->Uri() == "/im/group/message/success")
    {
        if (obj != nullptr)
        {
            obj->deleteLater();
            obj = nullptr;
        }
    }
    else
    {
        int count = obj->RetryCount();
        if (count > 5)
        {
            if (obj != nullptr)
            {
                obj->deleteLater();
                obj = nullptr;
            }
        }
        else
        {
            if ((!obj->ChatId().isEmpty()) && (obj->CallbackId() > 5000))
            {
                //发送失败
                Q_EMIT S_UpdataeRecordId(obj->ChatId(), QString("%1").arg(obj->CallbackId()), "-2");
            }
            else
            {
                obj->RetryCount(count++);
                obj->SendRequestToServer();
            }
        }
    }
    if (msg != nullptr)
    {
        msg->deleteLater();
        msg = nullptr;
    }
}

void TcpAgent::R_DoExecVisitorEvaluationCmdResult(TcpMessagePtr message)
{
    VisitorEvaluationCmd *obj = (VisitorEvaluationCmd *)QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }
    if (message)
    {
        message->deleteLater();
        message = nullptr;
    }
}

void TcpAgent::R_SetCurrentLoginStatus(ELOGINSTATUS oldState, ELOGINSTATUS newState)
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
        }
        break;
        case E_LOGIN_OK_MAINTCP:
        {
            foreach (SendMsgCmd *obj, m_offlineMsgList)
            {
                if (obj == nullptr)
                {
                    continue;
                }
                obj->MakeCmd();
                obj->SendRequestToServer();
            }

            foreach (SendMsgCmd *obj, m_offlineMsgList)
            {
                if (obj != nullptr)
                {
                    obj->deleteLater();
                    obj = nullptr;
                }
            }
            m_offlineMsgList.clear();
        }
        break;
        default:

            break;
    }
}

TcpAgent *TcpAgent::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new TcpAgent;
    }
    return m_instance;
}

TcpAgent::~TcpAgent()
{
    foreach (SendMsgCmd *obj, m_offlineMsgList)
    {
        if (obj != nullptr)
        {
            obj->deleteLater();
            obj = nullptr;
        }
    }
    m_offlineMsgList.clear();
}

void TcpAgent::R_MessageParsingFailed()
{
    Q_EMIT S_MessageParsingFailed();
}

void TcpAgent::R_ReceivedMessage(TcpMessagePtr msg)
{

    if (msg == nullptr)
    {
        Q_ASSERT(false);
        return;
    }
    QObject *obj = QObject::sender();


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
        QString chatId     = msg->GetPramsByName("chatId");
        QString customerId = msg->GetPramsByName("customerId");
        QString delayTime  = msg->GetPramsByName("delayTime");


        Q_EMIT S_Received_Im_AI_Robot_Message(1, chatId, customerId, delayTime.toInt());
    }
    else if (msg->Uri() == "/im/robot/start")
    {
        QString chatId     = msg->GetPramsByName("chatId");
        QString customerId = msg->GetPramsByName("customerId");
        QString delayTime  = msg->GetPramsByName("delayTime");


        Q_EMIT S_Received_Im_AI_Robot_Message(0, chatId, customerId, delayTime.toInt());

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


}

void TcpAgent::R_NetWorkError(int errorCode, QString msg)
{
    if (m_currentLoginStatus == E_LOGIN_DOING)
    {
       
    }
    else if (m_currentLoginStatus == E_LOGIN_RETRY_MAINTCP)
    {
       
    }
    else if (m_currentLoginStatus == E_LOGIN_ALLOK)
    {
        
        
    }
    else if (m_currentLoginStatus == E_LOGIN_OK_MAINTCP)
    {
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
        }

        
    }
    else
    {
        
    }
}

void TcpAgent::R_TokenError()
{
    Q_EMIT S_NetWorkError(400, "token error");
}

void TcpAgent::DoExecSendRichTextMsgCmd(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId)
{
    SendMsgCmd *req = new SendMsgCmd;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecSendMsgCmdResult(TcpMessagePtr)));
    req->HasResponse(true);
    req->Body(msgBody);
    req->MsgTo(msgTo);
    req->RecorderId(recordId);
    req->Robot(actionId);
    req->RichText(true);
    req->MsgType(msgType);
    req->UserType(userType);
    req->CallbackId(callbackId);
    req->RobotLogId(aiOtherInfo.GetValueByKey("robotLogId"));
    req->MakeCmd();
    if (m_currentLoginStatus == E_LOGIN_OK_MAINTCP || m_currentLoginStatus == E_LOGIN_ALLOK || m_currentLoginStatus == E_LOGIN_PRE_MONITORTCP || m_currentLoginStatus == E_LOGIN_RETRY_MONITORTCP)
    {
        req->SendRequestToServer();
    }
    else
    {
        m_offlineMsgList.push_back(req);
        if (msgType == E_MSGTYPE_MESSAGE)
        {
            Q_EMIT S_UpdataeRecordId(msgTo, QString("%1").arg(callbackId), "-2");
        }
    }
}

void TcpAgent::DoExecSendRichTextMsgCmdEx(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map)
{
    SendMsgCmd *req = new SendMsgCmd;
    connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecSendMsgCmdResult(TcpMessagePtr)));
    req->HasResponse(true);
    req->Body(msgBody);
    req->MsgTo(msgTo);
    req->RecorderId(recordId);
    req->Robot(actionId);
    req->RichText(true);
    req->MsgType(msgType);
    req->UserType(userType);
    req->CallbackId(callbackId);
    req->RobotLogId(aiOtherInfo.GetValueByKey("robotLogId"));
    req->ExtMap(map);
    req->MakeCmd();
    if (m_currentLoginStatus == E_LOGIN_OK_MAINTCP || m_currentLoginStatus == E_LOGIN_ALLOK || m_currentLoginStatus == E_LOGIN_PRE_MONITORTCP || m_currentLoginStatus == E_LOGIN_RETRY_MONITORTCP)
    {
        req->SendRequestToServer(); //发送文字消息 9
    }
    else
    {
        m_offlineMsgList.push_back(req);
        if (msgType == E_MSGTYPE_MESSAGE)
        {
            Q_EMIT S_UpdataeRecordId(msgTo, QString("%1").arg(callbackId), "-2");
        }
    }
}
/*
void TcpAgent::DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map)
{
    AiControlCmd *req = new AiControlCmd;

    req->ChatId(chatid);
    req->SetExtMap(map);
    req->MakeCmd();
    req->SendRequestToServer();
}
*/

void TcpAgent::DoHttpProxyReqCmd(QMap<QString, QString> cookies, HttpCmd *cmd)
{
    if (cmd)
    {
        httpProxyReqCmd *req = new httpProxyReqCmd;
        req->SetHttpCmd(cmd);
        connect(req, SIGNAL(S_Finished(TcpMessagePtr)), this, SLOT(R_DoExecHttpProxyCmdResult(TcpMessagePtr)));
        req->HasResponse(true);
        Json::Value bodyval;
        bodyval["url"] = cmd->GetUriWithBody().toStdString();
        if (cookies.size() > 0)
        {
            Json::Value cookiesval;

            QMap<QString, QString>::Iterator it = cookies.begin();
            while (it != cookies.end())
            {
                QString key                           = it.key();
                QString value                         = it.value();
                cookiesval[key.toStdString().c_str()] = value.toStdString();
                it++;
            }
            bodyval["cookies"] = cookiesval;
        }

        req->Body(QString::fromUtf8(bodyval.toStyledString().c_str()));
        req->MakeCmd();
        req->SendRequestToServer();
    }
}


void TcpAgent::R_DoExecHttpProxyCmdResult(TcpMessagePtr message)
{
    httpProxyReqCmd *obj = (httpProxyReqCmd *)QObject::sender();
    if (obj)
    {
        HttpCmd *cmd = obj->GetHttpCmd();
        if (cmd)
        {
            Json::Value json;
            bool        isOk = JsonHelper::GetJsonValueFromBody(message->Body(), json);
            if (isOk)
            {
                QString status = JsonHelper::GetValueByKey("status", json);
                QString body   = JsonHelper::GetValueByKey("body", json);
                if ((status == "0") || (status == "-1"))
                {
                    cmd->DoFinished("711", "", "");
                }
                else
                {
                    cmd->DoFinished(status, "", body);
                }
            }
            else
                cmd->DoFinished("712", "", "");
        }
        obj->deleteLater();
        obj = nullptr;
    }
    int i = 0;
}


void TcpAgent::DoOpenAckChat(QString chatId, QString vid)
{
    EventCustomCmd cmd (chatId);
	
    cmd.SubType(QString("%1").arg(E_SUBEVENT_ACKCHAT));
    cmd.AddMap("liveType", "open");
    cmd.MsgTo(vid);
    cmd.MakeCmd();
    cmd.SendRequestToServer();
}

void TcpAgent::DoSendShiver(QString chatId, QString vid)
{
    EventCustomCmd cmd(chatId);
	
    cmd.SubType(QString("%1").arg(E_SUBMSGTYPE_SHAKE));
    cmd.MsgTo(vid);
    cmd.MakeCmd();
    cmd.SendRequestToServer();
}


void TcpAgent::DoJoinover(int type, QString chatId, int delayTime)
{
    TcpCmd  req;
    QString cmd = "stop";
    if ((type != CHAT_STATUS_JOININ) &&(type != CHAT_STATUS_ROBOT) && (type != CHAT_STATUS_JOINOVER))
    {
        return;
    }
    if ((type == CHAT_STATUS_JOININ )||(type == CHAT_STATUS_ROBOT))
    {
        cmd = "start";
    }
    QString uri = QString("/im/robot/%1?chatId=%2&customerId=%3&delaytime=%4").arg(cmd).arg(chatId).arg(gSessionService->UserInfo()->UserId()).arg(delayTime);
    req.Uri(uri);
    req.SendRequestToServer();
}

void TcpAgent::DoUpdateLastMessageTime(QString chatId)
{
    UpdateLastMessageTime *cmd = new UpdateLastMessageTime(chatId);
    cmd->IsJsontype(false);
    cmd->Body();
    cmd->MakeCmd();
    cmd->SendRequestToServer();
}


void TcpAgent::R_DoSetUserStateCmdResult(TcpMessagePtr msg)
{
    ET("TcpAgent::R_DoSetUserStateCmdResult");
    LoginCmd *obj = (LoginCmd *)QObject::sender();
    if (obj)
    {
        obj->deleteLater();
        obj = nullptr;
    }
    QString msgBody   = msg->Body();
    Q_EMIT S_Received_UserStateCmd(msgBody);
}
