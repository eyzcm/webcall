#ifndef TcpHvManager_H
#define TcpHvManager_H

#include <QObject>
#include <QTimer>
#include <protocol/plat_TcpManagerinterface.h>
#include <protocol/TcpMessage.h>
#include <protocol/tcpparser.h>

class ElConnectStatusProcess;

class  TcpManager : public  Plat_TcpManagerInterface
{
    Q_OBJECT

public:
    static TcpManager* Instance();
    ~TcpManager();

private:
    TcpManager();
    static TcpManager* m_instance;

public:
    void ProcessAllNoEndTcpMessage();
    void CreateTcpConnect(QString host, QString port);
    void CreateTimer();
    void CloseTimer();
    void DisconnectTcp();

    void ErrorLogin(QString msg);

    TcpMessage* CreateNewResquest();

    void    SendTcpMessage(TcpMessage* msg);
	void    SendTcpMessage(QByteArray msg);
	void    ReceiveOutdataTcpMessage(QByteArray msg);
    void    LoginOk();
    int     GetErrorCode();
    QString GetErrorMsg();


private slots:
    void R_OnTimer();
    void R_ReceivedMessage(TcpMessagePtr msg);

    void R_NetWorkError(int code, QString reason);

private:
    int         GetNextCallId();
    TcpMessage* FindTcpMessage(TcpMessage* msg);

private:
    QList<TcpMessage*> m_reqList;

    int m_callID;
    // TcpImp* m_tcpImp;
    ElConnectStatusProcess* m_tcpImp;

    QTimer* m_timer;
    int     m_currCode;
    QString m_currMsg;

	TcpParser*  m_poutSipParser;
};

#define gTcpHvManager TcpManager::Instance()

#endif // TcpHvManager_H
