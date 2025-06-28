#ifndef IProtocolLoginHvAgent_H
#define IProtocolLoginHvAgent_H

#include "./protocol_biz/protocol_login_agentImp.h"
#include "utils/Plat_EnumDefine.h"
#include "./protocol_biz/IBaseProtocolAgent.h"
#include "data/CommInfo.h"
#include "data/Im_Message.h"

#include <QObject>

#include <protocol/platloginagentinterface.h>

class  IProtocolLoginAgent : public PlatLoginAgentInterface
{
    Q_OBJECT
public:
    static IProtocolLoginAgent* GetInstance();
    ~IProtocolLoginAgent();

private:
    IProtocolLoginAgent();
    static IProtocolLoginAgent* m_instance;

public:
    void DoLoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState);
    void DoExecVMLogin(QString userId, QString companyId);
    void DoExecVMAuthLogin();
    void DoGetSoftVersion();

    //初始化
    void Init();
  //初始化tcp代理
    void InitTcpAgent(QString loginurl, QString port);
    //初始化访客监测代理
    void InitVisitorMonitorTcpAgent(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber);
      void ClearTcpAgent();
    //清除
    void Clear();


signals:


public:

    void GetInitData(const QString& key, const QString& url);

    void DoExecLoginCmd(const QString& userName, const QString& password, int flag,int isForce);
    void DoUpdateLoginToken(const QString& token);
    void DoTcpReqLog(QString msgtype, QString body);
    void DoTcpResultLog(QString msgtype, QString body);

    //////////////////////////////////////////////////
/*
    virtual void PostHttpCardOrderExtendData(const QString& url, const QString& body, const QString& vid, const QString& staticId, const QString& chatId);
    virtual void PostHttpChatExtendData(const QString& url, const QString& body, const QString& vid, const QString& chatId);
    virtual void PostHttpOrderExtendData(const QString& url, const QString& body, const QString& vid, const QString& chatId);

    virtual void DoSaveSetting(const QString& body);
    virtual void DoGetSetting();
*/

private:
    IProtocolLoginAgentImp* m_pProtocolAgent;
};

#define gIProtocolLoginAgent IProtocolLoginAgent::GetInstance()
#endif // IProtocolLoginHvAgent_H
