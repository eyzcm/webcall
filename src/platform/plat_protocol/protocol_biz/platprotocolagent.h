#ifndef PLATPROTOCOLAGENT_H
#define PLATPROTOCOLAGENT_H

#include <QObject>


#include <protocol_biz/iplatprotocolinterface.h>
class PlatProtocolAgentImpl;

class  PlatProtocolAgent : public IPlatProtocolInterface
{
    Q_OBJECT
public:
    static PlatProtocolAgent* GetInstance();
    virtual ~PlatProtocolAgent();

    PlatProtocolAgent();


    virtual void InitTcpAgent(QString loginurl, QString port);
    virtual void DoExecVMGetVisitorList();
    //接口

    virtual void ClearTcpAgent();
    virtual void DoExecSendMonitorChatCmd(QString chatid);
    virtual void DoExecTakeoverChatCmd(QString chatid, QString groupId, QString customeId, int flag, QMap<QString, QString> map = QMap<QString, QString>()); //增加发送请求发起方，2.为机器人服务发起的 3.为客户端根据配置规则发送的, 6为多轮引导请求的
    virtual void DoExecSendMsgCmd(const QString& msgBody, const QString& fileName, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map = QMap<QString, QString>()) ;

    virtual void DoExecSendRichTextMsgCmdEx(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map);


    virtual void DoExecCommChatMessageCmd(const QString& chatid, ECHATACTION action, QString vid = "");
    virtual void DoVisitorUploadData(UIVisitorUploadDataInfo* info) ;

    virtual void DoExecInviteText(const QString& userid, const QString& viewId, const QString& InviteText);
    virtual void DoExecConversationCmd(const QString& userid, const QString& viewId, const QString& conversationType);

    virtual void DoExecOfflineMessageCmd();
    virtual void DoExecSetUserStateCmd(int state);
    virtual void DoExecSendAcceptActionCmd(QString groupId, QString vid);



      virtual void DoExecSendRichTextMsgCmd(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId);

    virtual void DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map);
    virtual void DoExecVisitorEvaluationCmd(const QString& chatId);
    virtual void DoExecRandomSendAcceptActionCmd();

    virtual void DoUploadOperatorLog(QString label) ;
    virtual void DoTransChat(const QString& data) ;
    virtual void DoAllocationChat(QString visitorId, QString groupId, QString customerId) ;
    virtual void DoSaveSummary(const QString& type, const QString& summary, const QString& chatId) ;
    virtual void DoSendShiver(QString chatId, QString vid);
    virtual void DoOpenAckChat(QString chatId, QString vid);
    virtual void DoJoinover(int type, QString chatId, int delaytime);
    virtual void DoUpdateLastMessageTime(QString chatId);

private:

    static PlatProtocolAgent* m_instance;
    PlatProtocolAgentImpl* m_pProtocolAgent;


public:


signals:

};

#endif // PLATPROTOCOLAGENT_H
