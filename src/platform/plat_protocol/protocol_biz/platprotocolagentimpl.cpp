#include "platprotocolagentimpl.h"

#include <data/UIWaitVisitorInfo.h>
#include "serviceInterface/dbuiplatservice.h"
#include "httpclient/httpplatclientapiagent.h"
#include "platprotocolagent.h"

PlatProtocolAgentImpl::PlatProtocolAgentImpl(QObject *parent) : QObject(parent)
{
    m_factoryBaseData    = new ConstructionData();
    m_tcpAgent           = TcpAgent::GetInstance();
    m_httpPlatAgent      = HttpPlatClientApiAgent::GetInstance();

    QTimer::singleShot( 10, this, [=]() {
        Init();
    });

}


PlatProtocolAgentImpl *PlatProtocolAgentImpl::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new PlatProtocolAgentImpl;
    }
    return m_instance;
}

PlatProtocolAgentImpl::~PlatProtocolAgentImpl()
{
}



PlatProtocolAgentImpl *PlatProtocolAgentImpl::m_instance = nullptr;


void PlatProtocolAgentImpl::Init()
{

    //connect(this, &ProtocolWork::S_GetCusotmerInfoResult, gIProtocolAgent, &IProtocolInterface::S_GetCusotmerInfoResult);
    connect(m_tcpAgent, &TcpAgent::S_NetWorkError, PlatProtocolAgent::GetInstance(), &IPlatProtocolInterface::S_MainTcpNetWorkError);
    //connect(m_tcpAgent, SIGNAL(S_NetWorkError(int, QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_MainTcpNetWorkError(int, QString)));
    connect(m_tcpAgent, SIGNAL(S_MessageParsingFailed()), PlatProtocolAgent::GetInstance(), SIGNAL(S_MainTcpMessageParsingFailed()));
    connect(m_tcpAgent, &TcpAgent::S_Received_Im_Message, PlatProtocolAgent::GetInstance(),  &IPlatProtocolInterface::S_Received_Im_Message);
    //connect(m_tcpAgent, SIGNAL(S_Received_Im_Message(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Im_Message(QString)));
    connect(m_tcpAgent, SIGNAL(S_Received_Im_Group_Message(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Im_Group_Message(QString)));
    connect(m_tcpAgent, SIGNAL(S_Received_Im_Group_Event(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Im_Group_Event(QString)));
    connect(m_tcpAgent, SIGNAL(S_Received_Im_Queue(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Im_Queue(QString)));
    connect(m_tcpAgent, SIGNAL(S_Received_Invite_Response(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Invite_Response(QString)));
    connect(m_tcpAgent, SIGNAL(S_LoginFinished(QString, QString, QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)));

    connect(m_tcpAgent, SIGNAL(S_Received_Im_AI_Message(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Im_AI_Message(QString)));
    connect(m_tcpAgent, SIGNAL(S_Received_UserStateCmd(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_UserStateCmd(QString)));

    connect(m_tcpAgent, SIGNAL(S_Received_Im_AI_Robot_Message(int, QString, QString, int)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Im_AI_Robot_Message(int, QString, QString, int)));
    connect(m_tcpAgent, SIGNAL(S_Received_KeepAlive_Event(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_KeepAlive_Event(QString)));
    connect(m_tcpAgent, SIGNAL(S_Received_Im_Data_VisitorInfo(QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_Received_Im_Data_VisitorInfo(QString)));


    connect(m_tcpAgent, SIGNAL(S_UpdataeRecordId(QString, QString, QString)), PlatProtocolAgent::GetInstance(), SIGNAL(S_UpdataeRecordId(QString, QString, QString))); //发送文字消息 完成 7

}

void PlatProtocolAgentImpl::DoExecOfflineMessageCmd()
{
    m_tcpAgent->DoExecOfflineMessageCmd();
}

void PlatProtocolAgentImpl::DoExecSetUserStateCmd(int state, int reTry)
{
    //更新状态

    m_tcpAgent->DoExecSetUserStateCmd(state);
}

void PlatProtocolAgentImpl::DoExecSendAcceptActionCmd(QString groupId, QString vid)
{
    m_tcpAgent->DoExecSendAcceptActionCmd(groupId, vid);
}

void PlatProtocolAgentImpl::ClearTcpAgent()
{
    m_tcpAgent->Clear();
}
void PlatProtocolAgentImpl::DoExecRandomSendAcceptActionCmd()
{
    UIWaitVisitorInfo *info = (UIWaitVisitorInfo *)gUIWaitVisitorInfoManager->GetWaitQueueInfoByRandom("UIContactInfo", m_factoryBaseData);
    if (info)
        m_tcpAgent->DoExecSendAcceptActionCmd(info->GroupId(), info->ViewVid());
}

void PlatProtocolAgentImpl::DoExecTakeoverChatCmd(QString chatid, QString groupId, QString customeId, int flag, QMap<QString, QString> map)
{
    if (flag == CHAT_TAKEOVER_AITOMAN)
        m_tokenChatId.append(chatid);
    m_tcpAgent->DoExecTakeoverChatCmd(chatid, groupId, customeId, flag, map);
}

void PlatProtocolAgentImpl::DoExecSendMonitorChatCmd(QString chatid)
{
    m_tcpAgent->DoExecSendMonitorChatCmd(chatid);
}

void PlatProtocolAgentImpl::DoExecSendMsgCmd(const QString &msgBody, const QString &fileName, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map)
{
    m_tcpAgent->DoExecSendMsgCmd(msgBody, fileName, msgTo, msgType, userType, recordId, callbackId, map);
}


void PlatProtocolAgentImpl::DoExecVisitorEvaluationCmd(const QString &chatId)
{
    m_tcpAgent->DoExecVisitorEvaluationCmd(chatId);
}



void PlatProtocolAgentImpl::DoExecCommChatMessageCmd(const QString &chatid, ECHATACTION action, QString vid)
{
    m_tcpAgent->DoExecCommChatMessageCmd(chatid, action, vid);
    // WorkObject()->RemoveChat( chatid,vid);
}

void PlatProtocolAgentImpl::DoExecInviteText(const QString &userid, const QString &viewId, const QString &InviteText)
{
    m_tcpAgent->DoExecInviteText(userid, viewId, InviteText);
}

void PlatProtocolAgentImpl::DoExecConversationCmd(const QString &userid, const QString &viewId, const QString &conversationType)
{
    m_tcpAgent->DoExecConversationCmd(userid, viewId, conversationType);
}


void PlatProtocolAgentImpl::DoSendShiver(QString chatId, QString vid)
{
    m_tcpAgent->DoSendShiver(chatId, vid);
}

void PlatProtocolAgentImpl::DoOpenAckChat(QString chatId, QString vid)
{
    m_tcpAgent->DoOpenAckChat(chatId, vid);
}


void PlatProtocolAgentImpl::DoJoinover(int type, QString chatId, int delaytime)
{
    m_tcpAgent->DoJoinover(type, chatId, delaytime);
}


void PlatProtocolAgentImpl::DoUpdateLastMessageTime(QString chatId)
{
    m_tcpAgent->DoUpdateLastMessageTime(chatId);
}





void PlatProtocolAgentImpl::DoExecSendRichTextMsgCmd(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId)
{
    m_tcpAgent->DoExecSendRichTextMsgCmd(msgBody, msgTo, msgType, userType, recordId, aiOtherInfo, actionId, callbackId);
}
void PlatProtocolAgentImpl::DoExecSendRichTextMsgCmdEx(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map)
{
    m_tcpAgent->DoExecSendRichTextMsgCmdEx(msgBody, msgTo, msgType, userType, recordId, aiOtherInfo, actionId, callbackId, map); //发送文字消息 8
}




void PlatProtocolAgentImpl::DoAllocationChat(QString visitorId, QString groupId, QString customerId)
{
    m_httpPlatAgent->DoAllocationChat(visitorId, groupId, customerId);


}


void PlatProtocolAgentImpl::InitTcpAgent(QString loginurl, QString port)
{
    m_tcpAgent->Init(loginurl, port);
}


//todo 9999

void PlatProtocolAgentImpl::DoExecVMGetVisitorList()
{
    //gVisitorMonitorTcpAgent->DoExecVMGetVisitorList();
}
void PlatProtocolAgentImpl::DoTransChat(const QString &data)
{
    //m_httpAgent->DoTransChat(data);
}
void PlatProtocolAgentImpl::DoExecSetUserStateCmd(int state)
{

}
void PlatProtocolAgentImpl::DoSaveSummary(const QString &type, const QString &summary, const QString &chatId)
{

}

void PlatProtocolAgentImpl::DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map)
{
	//m_tcpAgent->DoExecSendAiControlCmd(chatid, map);
}

void PlatProtocolAgentImpl::DoUploadOperatorLog(QString label)
{
	//m_httpAgent->DoUploadOperatorLog(info);
}

void PlatProtocolAgentImpl::DoVisitorUploadData(UIVisitorUploadDataInfo *info)
{
    //m_httpAgent->DoVisitorUploadData(info);
}

