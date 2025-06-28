#ifndef TCPIMP_H
#define TCPIMP_H

#include "./thread/elthreadcheck.h"

#include "tcpmessage.h"
#include "tcpparser.h"
#include "tcprequest.h"
#include "tcpresponse.h"
#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include "platdata_global.h"
class ElConnectStatusProcess;

class PLATDTA_EXPORT TcpImp : public ElThreadCheck
{
    Q_OBJECT

public:
    TcpImp(QObject* process);
    ~TcpImp();


    void ClearConnectHost();
    bool Init(QString address, quint16 port, QString type);
    bool IsConnect();

    void closeConnect();

    void connectservice();
    void SendTcpMessage(const QByteArray& msg);
    bool Loaded()
    {
        return m_loaded;
    };
    void Loaded(bool loader)
    {
        m_loaded = loader;
    };
    void InitSignal(bool isLoad);

signals:
    void S_NetWorkError(int, QString);
    void S_MessageParsingFailed();
    void S_ReceivedMessage(TcpMessagePtr msg);
	
    void S_connected();
    void S_disconnected();

    void S_ClearConnectHost();

    //信号
    void S_sendMessage(QByteArray msg);
    void S_connectservice();
    void S_closeConnect();

private slots:
    void R_closeConnect();
    void R_connectservice();
    void R_connected();
    void R_readyRead();
    void R_disconnected();
    void R_error(QAbstractSocket::SocketError socketError);
    void R_stateChanged(QAbstractSocket::SocketState socketState);

    void R_SendMessage(QByteArray msg);
    void R_Tcp_SendMessage(QByteArray msg);
    void R_ClearConnectHost();

private:
    bool m_loaded;
    void Clear();
    void SendMessageBase(QByteArray msg);
    void Write(QByteArray buf);
    void connectserviceBase();

    virtual void run();

private:
    QTcpSocket* m_client;
    bool        m_isConnect;
    int         m_initReceivedsign;
    TcpParser*  m_pSipParser;
    QString     m_host;
    quint16     m_port;
    QString     m_type;
};

#endif // TCPIMP_H
