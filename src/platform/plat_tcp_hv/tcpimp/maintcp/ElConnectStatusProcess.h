#pragma once
#include "tcp/ElConnectState.h"
#include "utils/Plat_EnumDefine.h"
#include <QObject>
#include <protocol/TcpMessage.h>

class TcpHvImp;

class ElConnectStatusProcess : public QObject, ElConnectState
{

    Q_OBJECT

public:
    ElConnectStatusProcess();
    ~ElConnectStatusProcess();

    static ElConnectStatusProcess* getInstance();
    bool                           Init(QString address, quint16 port, QString type);

    TcpHvImp* CurrentTcpImp();

    ELTCPlLOGINSTATUS getStatus();
    void              setStatus(ELTCPlLOGINSTATUS value);

    bool reconnectservice();
    bool connectservice();

	void ForceClose();
    void closeConnect();
    void LoginOk();

    //代理功能区
    bool Connect();
    bool TcpLogin(QByteArray msg);
    bool SendTcpMessage(QByteArray msg);

    bool ReConnect();

    bool LoginOut();
    bool InitState();
    bool command(QString command);

signals:
    void S_LoginOk();

    void S_connected();
    void S_NetWorkError(int, QString);
    void S_MessageParsingFailed();
    void S_ReceivedMessage(TcpMessagePtr msg);
    void S_MainTcpLoginStateChanged(ELTCPlLOGINSTATUS oldStatus, ELTCPlLOGINSTATUS newStatus);

public slots:
	void R_sendMessage(QByteArray body );
private slots:
    void R_connected();
    void R_disconnected();
    void R_NetWorkError(int, QString);
    void R_ReceivedMessage(TcpMessagePtr msg);
    void R_LoginOk();
	void R_Timeout();
	

private:
    bool                           CheckSender();
    int                            AlloceTcpImp();
    static ElConnectStatusProcess* m_connectProcess;
    ElConnectState**               m_states;
    ElConnectState*                m_istate;

    TcpHvImp*           m_currtcpImp;
    ELTCPlLOGINSTATUS m_currStatus;

    QList<TcpHvImp*> m_tcpimplist;
    bool           m_reconnect;
    int            m_init;
    int            m_maxTcpCount;
    int            m_currentTcp;

    QString m_loginaddress;
    QString m_logintype;
    quint16 m_loginport;


	QTimer   *  m_sendTimer;
};
