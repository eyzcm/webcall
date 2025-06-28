#ifndef TcpHvImp_H
#define TcpHvImp_H

#include <thread/ElThreadCheck.h>

#include <protocol/tcpmessage.h>
#include <protocol/tcpparser.h>
#include <QMutex>
#include<QTimer>

class ClientTcpImp;


class TcpHvImp: public ElThreadCheck
{
    Q_OBJECT

public:
    TcpHvImp(QObject* process);
    ~TcpHvImp();

	void ExitThread();
    void ClearConnectHost();
    bool Init(QString address, quint16 port, QString type);
    bool IsConnect();

    void closeConnect();
	void ForceClose();

    void connectservice();
    void SendTcpMessage(const QByteArray& msg);
    bool Loaded()
    {
        return m_loaded;
    };
	bool Effective()
	{
		return m_effective;
	}
    void Loaded(bool loader)
    {
        m_loaded = loader;
    };
    void InitSignal(bool isLoad);

	void DoTimeout();


	
signals:
	void S_Timeout();
    void S_NetWorkError(int, QString);
    void S_MessageParsingFailed();
    void S_ReceivedMessage(TcpMessagePtr msg);
    void S_connected();
    void S_disconnected();

    void S_ClearConnectHost();

    //信号
    void S_SendMessage(QByteArray msg);
    void S_connectservice();
    void S_closeConnect();
	void S_ForceClose();

public slots:
	void R_Timeout();
	
	void R_SendMessage(QByteArray msg);
    void R_closeConnect();
private slots:
	void R_message(void * buf, int len);

    void R_connectservice();
    void R_connected();
    void R_readyRead();
    void R_disconnected();
    void R_NetWorkError(int code , QString msg);


    
    void R_Tcp_SendMessage(QByteArray msg);
    void R_ClearConnectHost();
	void R_ForceClose();
	

private:
    bool m_loaded;
    void Clear();
    void SendMessageBase(QByteArray msg);
    void Write(QByteArray buf);
    void connectserviceBase();

    virtual void run();

private:

    bool        m_isConnect;
    int         m_initReceivedsign;
    TcpParser*  m_pSipParser;
    QString     m_host;
    quint16     m_port;
    QString     m_type;

	ClientTcpImp * m_imp;

	QMutex m_mutex;
	
	QByteArray m_bufdata;

	bool       m_effective;

	
};

#endif // TcpHvImp_H
