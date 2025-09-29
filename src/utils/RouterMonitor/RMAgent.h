#ifndef RMAGENT_H
#define RMAGENT_H

#include "WorkThread.h"
#include "logconsolework.h"
#include "OutdataConsoleWork.h"

#include <QCoreApplication>
#include <QDomElement>
#include <QMap>
#include <QTimer>


class IAgent;
class IRequest;
class RMAgent : public QObject
{
    Q_OBJECT

public:
    RMAgent();
    ~RMAgent();

    static RMAgent *instance();
    void            start(QString cmd);

signals:
    void S_LogInfo(QString body);
	void S_OutdataInfo(QString body);

private slots:
    void KeepAlive();
    void SubKeepAlive();
    void DoReceive(QString, QString, QString);
    void DoExit(int ncode, bool isCrash);


private:
	bool    CreateOutDataAgent(bool isRetry /*= false*/);
    bool    CreateConsoleAgent(bool isRetry /*= false*/);
    bool    CreateDataCenterAgent(bool isRetry = false, QString cmd = "");
    bool    CreateDataCenterAgentLoginOff();
    void    CreateTimer();
    void    PraseProtocol(const QString &sender, const QString &body);
    void    Do_Response_Protocol(const QString &body);
    void    Do_SetLoginInfo(const QString &body);
    void    Do_LogoutInfo(const QString &body);
    void    Do_KeepAlive(const QString &body);
    void    Do_LogInfo(const QString &body);

    QString GetProtocolType(const QString &body);
    void    Do_LogoffClient_Protocol();
    void    RunExit(int ncode, bool isForce = false);

    QMap<QString, QString> Convert2Map(const QDomElement &ele);

private:
    IAgent                   *m_dataCenterAgent;
    IAgent                   *m_logConsoleAgent;
	IAgent                   *m_outdataAgent;

    int                       m_req;
    QMap<QString, IRequest *> m_requestMap;
    QTimer                   *m_timer;
    QTimer                   *m_subtimer;
    QString                   m_password;
    QString                   m_username;
    QString                   m_servers;
    QString                   m_port;
    int                       m_nCount;


    WorkThread               m_workThread;
    LogConsoleWork           m_logConsoleWork;
	OutdataConsoleWork           m_outdataConsoleWork;

};
#endif // RMAGENT_H
