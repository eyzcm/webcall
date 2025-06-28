#ifndef TcpHvManager_H
#define TcpHvManager_H

#include <QObject>
#include <QTimer>
#include <protocol/plat_TcpManagerinterface.h>
#include <protocol/TcpMessage.h>
#include <protocol/tcpparser.h>

class ElConnectStatusProcess;

class  TcpHvManager : public  Plat_TcpManagerInterface
{
    Q_OBJECT

public:
    static TcpHvManager* Instance();
    ~TcpHvManager();

private:
    TcpHvManager();
    static TcpHvManager* m_instance;

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

signals:

    void S_connected();
    void S_NetWorkError(int, QString);
    void S_MessageParsingFailed();
    void S_ReceivedMessage(TcpMessagePtr msg);

    void S_SendTcpMessage(QByteArray msg);
    void S_UpdataeRecordId(QString chatId, QString callbackId, QString recordId);

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

#define gTcpHvManager TcpHvManager::Instance()

#endif // TcpHvManager_H
