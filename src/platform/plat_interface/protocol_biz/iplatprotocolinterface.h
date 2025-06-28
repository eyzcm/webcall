#ifndef IPLATPROTOCOLINTERFACE_H
#define IPLATPROTOCOLINTERFACE_H
#include "protocol_biz/IBaseProtocolAgent.h"
#include "utils/Plat_EnumDefine.h"
#include <QMap>
#include "data/CommInfo.h"
#include "data/UIVisitorUploadDataInfo.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT IPlatProtocolInterface  :  public QObject
{
    Q_OBJECT
public:
    IPlatProtocolInterface();

    virtual void DoExecVMGetVisitorList() = 0;
	virtual void InitTcpAgent(QString loginurl, QString port) = 0;
    virtual void DoExecSendMonitorChatCmd(QString chatid)= 0;
    virtual void DoExecTakeoverChatCmd(QString chatid, QString groupId, QString customeId, int flag, QMap<QString, QString> map = QMap<QString, QString>())=0; //增加发送请求发起方，2.为机器人服务发起的 3.为客户端根据配置规则发送的, 6为多轮引导请求的
    virtual void DoExecSendMsgCmd(const QString& msgBody, const QString& fileName, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map = QMap<QString, QString>()) = 0;

    virtual void DoExecSendRichTextMsgCmdEx(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map)= 0;
    

    virtual void DoExecCommChatMessageCmd(const QString& chatid, ECHATACTION action, QString vid = "")= 0;
    virtual void DoVisitorUploadData(UIVisitorUploadDataInfo* info) = 0;

    virtual void DoExecInviteText(const QString& userid, const QString& viewId, const QString& InviteText)= 0;
    virtual void DoExecConversationCmd(const QString& userid, const QString& viewId, const QString& conversationType)= 0;

    virtual void DoExecOfflineMessageCmd()= 0;
    virtual void DoExecSetUserStateCmd(int state)= 0;
    virtual void DoExecSendAcceptActionCmd(QString groupId, QString vid)= 0;



      virtual void DoExecSendRichTextMsgCmd(const QString& msgBody, const QString& msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId)= 0;

    virtual void DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map)= 0;
    virtual void DoExecVisitorEvaluationCmd(const QString& chatId)= 0;
    virtual void DoExecRandomSendAcceptActionCmd()= 0;

    virtual void DoUploadOperatorLog(QString label) = 0;
    virtual void DoTransChat(const QString& data) = 0;
    virtual void DoAllocationChat(QString visitorId, QString groupId, QString customerId) = 0;
    virtual void DoSaveSummary(const QString& type, const QString& summary, const QString& chatId) = 0;
    virtual void ClearTcpAgent() = 0;
    virtual void DoSendShiver(QString chatId, QString vid)= 0;
    virtual void DoOpenAckChat(QString chatId, QString vid)= 0;
    virtual void DoJoinover(int type, QString chatId, int delaytime) = 0;
    virtual void DoUpdateLastMessageTime(QString chatId) = 0;


signals:
    void S_MainTcpNetWorkError(int errorCode, QString msg);
    void S_MainTcpMessageParsingFailed();
    void S_Received_Im_Message(QString body);
    void S_Received_Im_Group_Message(QString body);
    void S_Received_Im_Group_Event(QString body);
    void S_Received_Im_Queue(QString body);
    void S_Received_Invite_Response(QString body);

    void S_Received_Im_AI_Message(QString body);
    void S_Received_Im_AI_Robot_Message(int type, QString chatId, QString customerId, int delaytime);
    void S_Received_KeepAlive_Event(QString body);
    void S_Received_Im_Data_VisitorInfo(QString body);
    void S_MainTcpLoginFinished(QString authType, QString errorCode, QString body);
    void S_VMConnected();
    void S_VMNetWorkError(int errorCode, QString msg);
    void S_VMMessageParsingFailed();
    void S_VM_AuthFinished(bool isOk, int code);
    void S_VM_LoginFinished(QString body);
    void S_VMGetVisitorListFinished(QString body);
    void S_VMVisitorChanged(QString body);
    void S_VMVisitorRemove(QString body);
    void S_VMVisitorTempRemove(QString body);
    void S_VMVisitorMonitor(QString body);
};

#endif // IPLATPROTOCOLINTERFACE_H
