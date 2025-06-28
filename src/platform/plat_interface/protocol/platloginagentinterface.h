#ifndef PLATLOGINAGENTINTERFACE_H
#define PLATLOGINAGENTINTERFACE_H

#include <QObject>
#include <QString>
#include "utils/Plat_EnumDefine.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT PlatLoginAgentInterface: public QObject
{
    Q_OBJECT
public:
    PlatLoginAgentInterface();

    virtual void DoGetSoftVersion(){};

    //初始化
    virtual  void Init(){};
  //初始化tcp代理
    virtual void InitTcpAgent(QString loginurl, QString port){};
    //初始化访客监测代理
    virtual void InitVisitorMonitorTcpAgent(QString loginurl, QString port, QString userName, QString companyId, QString monitorvisitornumber){};
     virtual  void ClearTcpAgent(){};
    //清除
    virtual  void Clear(){};

    virtual void DoLoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState){};

    virtual void DoHttpReqLog(QString url,QString postdata);
    virtual void DoHttpResultFinishedLog(QString code, QString reason, QString body);
    virtual void DoTcpReqLog(QString msgtype, QString body);
    virtual void DoTcpResultLog(QString msgtype, QString body);



signals:
    //协议类日志显示
     void S_HttpResultFinishedLog(QString code, QString reason, QString body);
     void S_HttpReqLog(QString urlstr, QString body);
     void S_TcpReqLog(QString msgtype, QString body);
     void S_TcpResultLog(QString msgtype, QString body);


    void S_MainTcpConnected();
    void S_MainTcpNetWorkError(int, QString);
    void S_VMConnected();
    void S_VM_AuthFinished(bool, int);
    void S_VMMessageParsingFailed();
    void S_VM_LoginFinished(QString);

    void S_ForceLogout();
    void S_ForceReLogin(int);
    void S_VMNetWorkError(int, QString);
    void S_VM_LoginEnd();
    void S_AddMonitorVisitorList();
    void S_SetVisitorStatus(QString, EVISITORSTATUS, EVISITORSTATUS);
    void S_Received_KeepAlive_State(EPRESENCESTATUS);
    void S_HeartPaceTimeOut();
    void S_AdminAuthLogin();
    


    void S_MainTcpLoginFinished(QString, QString, QString);





    //路由
    void S_ReceiveCmdData(QString);

public:


    virtual void DoExecVMLogin(QString userId, QString companyId){};
    virtual void DoExecVMAuthLogin(){};

    virtual void GetInitData(const QString& key, const QString& url){};

    virtual void DoExecLoginCmd(const QString& userName, const QString& password, int flag,int isForce){};
    virtual void DoUpdateLoginToken(const QString& token){};
    virtual void DoTcpReconnect(){};

    //////////////////////////////////////////////////
/*
    virtual void PostHttpCardOrderExtendData(const QString& url, const QString& body, const QString& vid, const QString& staticId, const QString& chatId);
    virtual void PostHttpChatExtendData(const QString& url, const QString& body, const QString& vid, const QString& chatId);
    virtual void PostHttpOrderExtendData(const QString& url, const QString& body, const QString& vid, const QString& chatId);

    virtual void DoSaveSetting(const QString& body);
    virtual void DoGetSetting();
*/


};

#endif // PLATLOGINAGENTINTERFACE_H
