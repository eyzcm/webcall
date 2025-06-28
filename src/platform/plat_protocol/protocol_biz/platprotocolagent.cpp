#include "platprotocolagent.h"
#include "platprotocolagentimpl.h"

PlatProtocolAgent::PlatProtocolAgent()
{
    m_pProtocolAgent = PlatProtocolAgentImpl::GetInstance();
}




PlatProtocolAgent *PlatProtocolAgent::GetInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new PlatProtocolAgent();
    }
    return m_instance;
}

PlatProtocolAgent::~PlatProtocolAgent()
{
}



PlatProtocolAgent *PlatProtocolAgent::m_instance = nullptr;


void PlatProtocolAgent::DoExecSendMonitorChatCmd(QString chatid)
{

    return m_pProtocolAgent->DoExecSendMonitorChatCmd(chatid);
}
void PlatProtocolAgent::DoExecTakeoverChatCmd(QString chatid, QString groupId, QString customeId, int flag, QMap<QString, QString> map )
{
    return m_pProtocolAgent->DoExecTakeoverChatCmd(chatid,groupId,customeId,flag,map);
}

void PlatProtocolAgent::DoExecSendMsgCmd(const QString& msgBody, const QString& fileName, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map )
{
    return m_pProtocolAgent->DoExecSendMsgCmd(msgBody, fileName,msgTo,msgType,userType,recordId,callbackId,map);
}

void PlatProtocolAgent::DoExecSendRichTextMsgCmdEx(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map)
{
    return m_pProtocolAgent->DoExecSendRichTextMsgCmdEx(msgBody,msgTo,msgType,userType, recordId,aiOtherInfo,actionId, callbackId, map);
}


void PlatProtocolAgent::DoExecCommChatMessageCmd(const QString& chatid, ECHATACTION action, QString vid )
{
    return m_pProtocolAgent->DoExecCommChatMessageCmd(chatid,action,vid);
}

void PlatProtocolAgent::DoVisitorUploadData(UIVisitorUploadDataInfo* info)
{
    return m_pProtocolAgent->DoVisitorUploadData(info);
}

void PlatProtocolAgent::DoExecInviteText(const QString& userid, const QString& viewId, const QString& InviteText)
{
    return m_pProtocolAgent->DoExecInviteText(userid, viewId,InviteText);
}

void PlatProtocolAgent::DoExecConversationCmd(const QString& userid, const QString& viewId, const QString& conversationType)
{
    return m_pProtocolAgent->DoExecConversationCmd(userid,viewId,conversationType);
}

void PlatProtocolAgent::DoExecOfflineMessageCmd()
{
    return m_pProtocolAgent->DoExecOfflineMessageCmd();
}

void PlatProtocolAgent::DoExecSetUserStateCmd(int state)
{
    return m_pProtocolAgent->DoExecSetUserStateCmd(state);
}

void PlatProtocolAgent::DoExecSendAcceptActionCmd(QString groupId, QString vid)
{
    return m_pProtocolAgent->DoExecSendAcceptActionCmd(groupId,vid);
}


void PlatProtocolAgent::DoExecSendRichTextMsgCmd(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId)
{
    return m_pProtocolAgent->DoExecSendRichTextMsgCmd(msgBody,msgTo,msgType,userType,recordId,aiOtherInfo,actionId,callbackId);
}

void PlatProtocolAgent::DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map)
{
	return m_pProtocolAgent->DoExecSendAiControlCmd(chatid, map);
}

void PlatProtocolAgent::DoExecVisitorEvaluationCmd(const QString& chatId)
{
    return m_pProtocolAgent->DoExecVisitorEvaluationCmd(chatId);
}

void PlatProtocolAgent::DoExecRandomSendAcceptActionCmd()
{
    return m_pProtocolAgent->DoExecRandomSendAcceptActionCmd();
}

void PlatProtocolAgent::DoUploadOperatorLog(QString label)
{
    return m_pProtocolAgent->DoUploadOperatorLog(label);
}

void PlatProtocolAgent::DoTransChat(const QString& data)
{
    return m_pProtocolAgent->DoTransChat(data);
}

void PlatProtocolAgent::DoAllocationChat(QString visitorId, QString groupId, QString customerId)
{
    return m_pProtocolAgent->DoAllocationChat(visitorId,groupId,customerId);
}

void PlatProtocolAgent::DoSaveSummary(const QString& type, const QString& summary, const QString& chatId)
{
    return m_pProtocolAgent->DoSaveSummary(type,summary,chatId);
}

void PlatProtocolAgent::ClearTcpAgent()
{
    return m_pProtocolAgent->ClearTcpAgent();
}

void PlatProtocolAgent::DoSendShiver(QString chatId, QString vid)
{
    return m_pProtocolAgent->DoSendShiver(chatId,vid);
}

void PlatProtocolAgent::DoOpenAckChat(QString chatId, QString vid)
{
    return m_pProtocolAgent->DoOpenAckChat(chatId,vid);
}



void PlatProtocolAgent::DoJoinover(int type, QString chatId, int delaytime)
{
    return m_pProtocolAgent->DoJoinover(type,chatId, delaytime);
}

void PlatProtocolAgent::DoUpdateLastMessageTime(QString chatId)
{
    return m_pProtocolAgent->DoUpdateLastMessageTime(chatId);
}


void PlatProtocolAgent::DoExecVMGetVisitorList()
{
    m_pProtocolAgent->DoExecVMGetVisitorList();
}

void PlatProtocolAgent::InitTcpAgent(QString loginurl, QString port)
{
    m_pProtocolAgent->InitTcpAgent(loginurl, port);
}
