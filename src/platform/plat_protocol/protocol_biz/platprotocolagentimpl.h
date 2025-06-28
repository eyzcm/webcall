#ifndef PLATPROTOCOLAGENTIMPL_H
#define PLATPROTOCOLAGENTIMPL_H

#include "tcpagent.h"

#include <QObject>
#include <datafactory/ConstructionData.h>
#include <protocol_biz/iplatprotocolinterface.h>
class HttpPlatClientApiAgent;

class PlatProtocolAgentImpl : public QObject
{
    Q_OBJECT

public:
    static PlatProtocolAgentImpl* GetInstance();
    ~PlatProtocolAgentImpl();

    explicit PlatProtocolAgentImpl(QObject *parent = nullptr);

    void Init();
	void DoExecSetUserStateCmd(int state, int reTry);
    void ClearTcpAgent();
    void DoExecSendMonitorChatCmd(QString chatid);
    void DoExecTakeoverChatCmd(QString chatid, QString groupId, QString customeId, int flag, QMap<QString, QString> map = QMap<QString, QString>()); //增加发送请求发起方，2.为机器人服务发起的 3.为客户端根据配置规则发送的, 6为多轮引导请求的
    void DoExecSendMsgCmd(const QString& msgBody, const QString& fileName, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map = QMap<QString, QString>()) ;

    void DoExecSendRichTextMsgCmdEx(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map);


    void DoExecCommChatMessageCmd(const QString& chatid, ECHATACTION action, QString vid = "");
    void DoVisitorUploadData(UIVisitorUploadDataInfo* info) ;

    void DoExecInviteText(const QString& userid, const QString& viewId, const QString& InviteText);
    void DoExecConversationCmd(const QString& userid, const QString& viewId, const QString& conversationType);

    void DoExecOfflineMessageCmd();
    void DoExecSetUserStateCmd(int state);
    void DoExecSendAcceptActionCmd(QString groupId, QString vid);



    void DoExecSendRichTextMsgCmd(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId);

    void DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map);
    void DoExecVisitorEvaluationCmd(const QString& chatId);
    void DoExecRandomSendAcceptActionCmd();

    void DoUploadOperatorLog(QString label) ;
    void DoTransChat(const QString& data) ;
    void DoAllocationChat(QString visitorId, QString groupId, QString customerId) ;
    void DoSaveSummary(const QString& type, const QString& summary, const QString& chatId) ;
    void DoSendShiver(QString chatId, QString vid);
    void DoOpenAckChat(QString chatId, QString vid);
    void DoJoinover(int type, QString chatId, int delaytime);
    void DoUpdateLastMessageTime(QString chatId);

    void DoExecVMGetVisitorList();
    void InitTcpAgent(QString loginurl, QString port);

private:
    static PlatProtocolAgentImpl* m_instance;

    TcpAgent*           m_tcpAgent;


    QStringList         m_tokenChatId;
    ConstructionData*   m_factoryBaseData;
    HttpPlatClientApiAgent*          m_httpPlatAgent;


signals:



};

#endif // PLATPROTOCOLAGENTIMPL_H
