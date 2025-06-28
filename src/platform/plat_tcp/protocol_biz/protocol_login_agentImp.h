#ifndef IProtocolLoginAgentImp_H
#define IProtocolLoginAgentImp_H

#include "./protocol_biz/protocolLoginwork.h"
#include "utils/BaseConstructionData.h"
#include "utils/Plat_EnumDefine.h"

#include "protocol_biz/IBaseProtocolAgent.h"
#include "data/CommInfo.h"


#include "thread/protocolthread.h"
#include <QObject>


class TcpLoginAgent;

class  IProtocolLoginAgentImp : public QObject
{
    Q_OBJECT
public:
    static IProtocolLoginAgentImp* GetInstance();
    ~IProtocolLoginAgentImp();

    ProtocolThread* ProtoThread()
    {
        return &m_protocolThread;
    }

private:
    IProtocolLoginAgentImp();
    static IProtocolLoginAgentImp* m_instance;
    QString                   m_currentUserId;

public:

    void DoLoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState);
    void DoExecVMLogin(QString userId, QString companyId);

    void DoExecVMAuthLogin();
    void Init();
  void InitTcpAgent(QString loginurl, QString port);
    void ClearTcpAgent();
    void InitVisitorMonitorTcpAgent(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber);

    void DoExecLoginCmd(const QString &userName, const QString &password, int flag,int isforce);
    ProtocolLoginWork* WorkObject();

    void DoUpdateLoginToken(const QString& token);

    QString CurrentUserId()
    {
        return m_currentUserId;
    }
    void CurrentUserId(QString CurrentUserId)
    {
        m_currentUserId = CurrentUserId;
    }

public slots:

    void R_VMConnected();

public:


signals:
    void S_VM_LoginEnd();
    void S_VMConnected();
    void S_VM_AuthFinished(bool, int);
    void S_VM_LoginFinished(QString body);
    void S_VMMessageParsingFailed();

    void S_Received_KeepAlive_State(EPRESENCESTATUS);
    void S_MainTcpLoginFinished(QString, QString, QString);
    void S_MainTcpNetWorkError(int errorCode, QString msg);

    void S_ForceLogout();
    void S_ForceReLogin(int force);
    void S_HeartPaceTimeOut();

    void S_AddMonitorVisitorList();


private:
    ECLIENTTYPE GetClientType();

private:
    ProtocolThread      m_protocolThread;
    ProtocolLoginWork*       m_pProtocolwork;

    TcpLoginAgent*           m_tcpAgent;
    BaseConstructionData*   m_factoryBaseData;
    int                 m_currentChatType; // 0 老方式 1.增量方式
    QStringList         m_tokenChatId;
    int    m_ForceLogin;
};

#endif // IProtocolLoginAgentImp_H
