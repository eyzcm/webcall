#ifndef ProtocolLoginWork_H
#define ProtocolLoginWork_H

#include <QList>
#include <QObject>
#include <QTimer>

#include <include/json/value.h>


#include "utils/baseConstructionData.h"
#include "utils/Plat_EnumDefine.h"
#include "thread/DispathThread.h"
#include "data/Im_Message.h"

#include "thread/CheckThread.h"


class IProtocolLoginAgentImp;

class ProtocolLoginWork : public QObject
{
    Q_OBJECT

public:
    ~ProtocolLoginWork();
    ProtocolLoginWork();

public:

    void         SetAiByContent(const QString &content);
    void         DoUpdateChat(Json::Value message);
    void         SaveCommonWordToLocal();
    CheckThread *checkThread()
    {
        return m_checkThread;
    }
    static void changeChatInfo(const QString &datatype, void *itemList, QString &param1, QString &param2, void *outlist);

    QString GetSystemConfigByKey(const QString &key, const QString &defaultString);
    void    Init();
    void    Protocol(IProtocolLoginAgentImp *imp);
    void    InitTimer();
    void    Clear();

    ECLIENTTYPE GetClientType() const;


    void SetUserState(EPRESENCESTATUS state);


signals:
	void S_ThreadInit();
    void S_ForceLogout();
    void S_ForceReLogin(int);

    void S_VM_LoginEnd();
    void S_AddMonitorVisitorList();

    void S_Received_KeepAlive_State(EPRESENCESTATUS);
    void S_HeartPaceTimeOut();

	void S_SendTcpMessage(QByteArray msg);


private slots:
	void  R_ThreadInit();
    void    onInitTimer();
	void onReceiveCmdTask(QString cmd);

private slots:
    void R_UploadDumpFile();
    void R_OnParentHeartPaceTimeOut();

  //  void R_GetInitDataFinished(QString code, QString reason, QString body, QString key);


private:

	void ReplyPrototcol(const QString& req, const QString &sender, const QString &receiver, int rescode);
	int  ParseProtocol(const QString &body, QString& req, QString& sender);

private:
    //客户端版本
    EPRESENCESTATUS m_curPresenceStatus;
    ELOGINSTATUS    m_currentLoginState;

    QTimer *m_parentHeartPaceTimer;
    QTimer *m_heartPaceTimer;


    IProtocolLoginAgentImp                 *m_protocol;
    ECLIENTTYPE                        m_clientType;
    DispathThread                     *m_dispathThread;
    CheckThread                       *m_checkThread;
     BaseConstructionData                  *m_factoryBaseData;

    bool m_firstGetCurrentChat;
    bool m_exitclose;
    int  m_offlinecount;
    bool m_currentChatEnd;


};

#endif // ProtocolLoginWork_H
