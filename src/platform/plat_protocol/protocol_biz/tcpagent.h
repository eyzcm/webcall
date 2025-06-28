#ifndef TCPAGENT_H
#define TCPAGENT_H

#include <QList>
#include <QObject>
#include <QVector>
#include "plat_protocol_global.h"
#include "utils/Plat_EnumDefine.h"
#include "QTimer"
#include "data/CommInfo.h"

#include "include/json/value.h"
#include "protocol/httpcmd.h"
#include "protocol/sendmsgcmd.h"
#include "protocol/tcpmessage.h"

class PLAT_PROTOCOL_EXPORT TcpAgent : public QObject
{
    Q_OBJECT

public:
    static TcpAgent *GetInstance();
    ~TcpAgent();

private:
    TcpAgent();
    static TcpAgent *m_instance;

    
public:

    void DoJoinover(int type, QString chatId, int delayTime);
    void DoUpdateLastMessageTime(QString chatId);
    void Init(QString loginurl, QString port);
    void CreateTcpConnect(QString loginurl, QString port);
    void Clear();
    
    void DoSendShiver(QString chatId, QString vid);
    void DoOpenAckChat(QString chatId, QString vid);
    void DoHttpProxyReqCmd(QMap<QString, QString> cookies, HttpCmd *cmd);
    void DoExecInviteText(const QString &userid, const QString &viewId, const QString &InviteText);
    void DoExecCommChatMessageCmd(const QString &chatid, ECHATACTION action, QString vid = "");
    void DoExecConversationCmd(const QString &userid, const QString &viewId, const QString &conversationType);
    
    void DoExecOfflineMessageCmd();
    void DoExecSetUserStateCmd(int state);
    void DoExecSendAcceptActionCmd(QString groupId, QString vid);
    void DoExecSendMonitorChatCmd(QString chatid);
    void DoExecTakeoverChatCmd(QString chatid, QString groupId, QString customeId, int flag, QMap<QString, QString> map);
        //void DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map);
        void DoExecSendMsgCmd(const QString &msgBody, const QString &fileName, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map);
     void DoExecSendRichTextMsgCmd(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId);
    void DoExecSendRichTextMsgCmdEx(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map);
    void DoExecVisitorEvaluationCmd(const QString &chatId);

private slots:
	
    void R_DoExecHttpProxyCmdResult(TcpMessagePtr message);
    void R_DoExecCommChatMessageCmdResult(TcpMessagePtr message);
    void R_DoExecConversationCmdResult(TcpMessagePtr message);
    void R_DoExecLoginCmdResult(TcpMessagePtr message);
    void R_DoExecOfflineMessageCmdResult(TcpMessagePtr message);
    void R_DoExecSendMsgCmdResult(TcpMessagePtr message);
    void R_DoExecVisitorEvaluationCmdResult(TcpMessagePtr message);
    void R_TokenError();
    void R_DoSetUserStateCmdResult(TcpMessagePtr message);

public slots:
    void R_SetCurrentLoginStatus(ELOGINSTATUS oldState, ELOGINSTATUS newState);

signals:
    void S_UpdataeRecordId(QString chatId, QString callbackId, QString recordId);
    
    void S_NetWorkError(int errorCode, QString msg);
    void S_MessageParsingFailed();
    void S_Received_Im_Message(QString body);
    void S_Received_Im_Group_Message(QString body);
    void S_Received_Im_Group_Event(QString body);
    void S_Received_Im_Queue(QString body);
    void S_LoginFinished(QString authType, QString errorCode, QString body);
    void S_Received_Invite_Response(QString body);
    void S_Received_Im_AI_Message(QString body);
    void S_Received_Im_AI_Robot_Message(int type, QString chatId, QString customerId, int delayTime);
    void S_Received_KeepAlive_Event(QString body);
    void S_Received_Im_Data_VisitorInfo(QString body);
    void S_Received_UserStateCmd(QString body);

private slots:
    
    void R_MessageParsingFailed();
    void R_ReceivedMessage(TcpMessagePtr msg);
    void R_NetWorkError(int errorCode, QString msg);
    
    

private:
    bool m_isTcpManager;
    bool m_isLastAddressInfo;

    QVector<QString>    m_messageReqList;
    QVector<QString>    m_groupMessageReqList;
    ELOGINSTATUS        m_currentLoginStatus;
    QList<SendMsgCmd *> m_offlineMsgList;
    QString             m_loginUrl;
    QString             m_port;

    int     m_retryLoginCount;
    bool    m_isConnect;
    bool    m_isLogin;



    
    
    
    
};

#define gTcpAgent TcpAgent::GetInstance()

#endif // TCPAGENT_H
