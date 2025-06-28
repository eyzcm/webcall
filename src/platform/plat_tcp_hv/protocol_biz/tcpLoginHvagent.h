#ifndef TcpLoginHvAgent_H
#define TcpLoginHvAgent_H

#include <QList>
#include <QObject>
#include <QVector>

#include "utils/Plat_EnumDefine.h"
#include "QTimer"
#include "data/CommInfo.h"


#include "protocol/tcpmessage.h"


class TcpLoginHvAgent : public QObject
{
    Q_OBJECT

public:
    static TcpLoginHvAgent *GetInstance();
    ~TcpLoginHvAgent();

private:
    TcpLoginHvAgent();
    static TcpLoginHvAgent *m_instance;

    int m_setRetryLogin;

public:
    void Init(QString loginurl, QString port);
    void CreateTcpConnect(QString loginurl, QString port);
    void Clear();
    void TcpReconnect();
    void DoExecLoginCmd(const QString &userName, const QString &password, int flag,int isForce);
    void DoExecOfflineMessageCmd();
    void DoUpdateLoginToken(const QString& token);


private slots:
    void R_MessageParsingFailed();
    void R_ReceivedMessage(TcpMessagePtr msg);
    void R_NetWorkError(int errorCode, QString msg);
    void ReTryLoginConnect();
    void R_ReloginTimeOut();
    void R_DoExecLoginCmdResult(TcpMessagePtr msg);


public slots:
    void R_SetCurrentLoginStatus(ELOGINSTATUS oldState, ELOGINSTATUS newState);

signals:
    void S_connected();
    void S_NetWorkError(int errorCode, QString msg);
    void S_MessageParsingFailed();
  void S_LoginFinished(QString authType, QString errorCode, QString body);
    void S_ForceLogout();
    void S_ForceReLogin(int force);

private slots:
    void R_connected();
    void R_TokenError();


private:
    bool m_isTcpManager;
    bool m_isLastAddressInfo;

    QVector<QString>    m_messageReqList;
    QVector<QString>    m_groupMessageReqList;
    ELOGINSTATUS        m_currentLoginStatus;


    QString             m_loginUrl;
    QString             m_port;

    int     m_retryLoginCount;
    bool    m_isConnect;
    bool    m_isLogin;
    QString m_userName;
    QString m_password;
    bool    m_initConnected;
    bool    m_ForceLogout;
    bool    m_reLoginFlag;
    int     m_loginType;
    int     m_loginCount;
    QTimer *m_reloginTimer;
    int     m_ForceLogin;
};

#define gTcpLoginHvAgent TcpLoginHvAgent::GetInstance()

#endif // TcpLoginHvAgent_H
