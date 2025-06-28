#include "tcpimp.h"
#include "utils/ConstStringHelper.h"
#include "utils/EL_Util_Time.h"

#include "service/util_service.h"
//#include "visitormonitortcpmanager.h"

TcpImp::TcpImp(QObject* process) : ElThreadCheck()
{

    qDebug()<<"moveToThread TcpImp"<<endl;
	moveToThread(this);

    m_isConnect        = false;
    m_loaded           = false;
    m_initReceivedsign = 0;
}

TcpImp::~TcpImp()
{
    Clear();
}
void TcpImp::R_ClearConnectHost()
{
    InitSignal(false);
    // m_isConnect = false;
    if (m_client->isOpen())
        m_client->disconnectFromHost();
    else
        m_isConnect = false;
}

void TcpImp::ClearConnectHost()
{

    m_initReceivedsign = 0;

    // disconnect()
    Q_EMIT S_ClearConnectHost();
}

void TcpImp::R_connectservice()
{
    InitSignal(true);
    connectserviceBase();
}
void TcpImp::connectserviceBase()
{
    int isvalid = m_client->isValid();

    if (m_client)
    {
        m_client->disconnectFromHost();
        m_client->close();
        m_client->connectToHost(m_host, m_port); //建立一个TCP连接，主机地址是m_httpAddress，端口号是httpPort
    }
}

void TcpImp::Clear()
{
    m_isConnect = false;
    this->disconnect();
    if (m_pSipParser)
    {
        m_pSipParser->deleteLater();
        m_pSipParser = NULL;
    }
    if (m_client)
    {
        // m_client->close();
        m_client->deleteLater();
        m_client = nullptr;
    }
}

bool TcpImp::Init(QString address, quint16 port, QString type)
{
    if (address.isEmpty())
    {
        return false;
    }
    m_type = type;
    m_host = address;
    m_port = port;
    ////服务器地址或者需要发送的数据为空直接返回
    // client.connectToHost(address, port);//建立一个TCP连接，主机地址是m_httpAddress，端口号是httpPort
    // client.setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。
    return true;
}

bool TcpImp::IsConnect()
{
    return m_isConnect;
}

void TcpImp::SendMessageBase(QByteArray msg)
{
    if (!msg.isEmpty())
    {
        Write(msg);
    }
}

void TcpImp::SendTcpMessage(const QByteArray& msg)
{
    Q_EMIT S_sendMessage(msg);
}

void TcpImp::R_connected()
{

    m_isConnect = true;
    Q_EMIT S_connected();
}

void TcpImp::R_readyRead()
{

    if (m_client && m_client->bytesAvailable())
    {
        if (m_pSipParser)
        {
            m_pSipParser->Parse(m_client->readAll());
        }
    }
}

void TcpImp::R_disconnected()
{
    ET("TcpImp::R_disconnected()");
    m_isConnect = false;
    // ClearConnectHost();
    Q_EMIT S_disconnected();
}

void TcpImp::R_error(QAbstractSocket::SocketError socketError)
{

    {
        // tcp长连接错误。
        if (m_client)
        {
            GTESTLOG( QString("%1,%2").arg(socketError).arg( m_client->errorString()) );
            Q_EMIT S_NetWorkError(socketError, m_client->errorString());
        }
        else
        {
            GTESTLOG( QString("%1,unknow error!").arg(socketError) );
            Q_EMIT S_NetWorkError(socketError, "unknow error!");
        }
    }
}

void TcpImp::R_stateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        m_isConnect = false;
    }else
    {
        if (socketState ==  QAbstractSocket::ConnectedState )
            m_isConnect = true;
    }


    /*
    if (socketState == QAbstractSocket::ConnectedState)
    {
        m_isConnect = true;
    }
    else
    {

        m_isConnect = false;
    }
    */
}

void TcpImp::R_SendMessage(QByteArray msg)
{
    ET("TcpImp::R_SendMessage");
    SendMessageBase(msg);
}

void TcpImp::R_Tcp_SendMessage(QByteArray msg)
{
    ET("TcpImp::R_Tcp_SendMessage");
    SendMessageBase(msg);
}

void TcpImp::Write(QByteArray buf)
{
    if (m_isConnect && m_client)
    {

        m_client->write(buf);
        m_client->flush();
        GPROTOCOLLOG(QString("tcp send protocol:%1").arg(QString::fromUtf8(buf)), "TCPIMP");
    }
    else
    {
        GERRORLOG(QString("tcp send protocol error！ 协议内容:%1").arg(QString::fromUtf8(buf)));
    }
}

void TcpImp::connectservice()
{

    Q_EMIT S_connectservice();
}

void TcpImp::InitSignal(bool isLoad)
{
    if (isLoad)
    {
        ET("void TcpImp::InitSignal(true)");
        connect(m_pSipParser, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()));

        connect(m_pSipParser, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SIGNAL(S_ReceivedMessage(TcpMessagePtr)));
        connect(m_client, SIGNAL(connected()), this, SLOT(R_connected()));
        connect(m_client, SIGNAL(disconnected()), this, SLOT(R_disconnected()));
        connect(m_client, SIGNAL(readyRead()), this, SLOT(R_readyRead()));
        connect(m_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(R_error(QAbstractSocket::SocketError)));
        connect(m_client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(R_stateChanged(QAbstractSocket::SocketState)));
    }
    else
    {
        ET("void TcpImp::InitSignal(false)");
        disconnect(m_pSipParser, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()));
        disconnect(m_pSipParser, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SIGNAL(S_ReceivedMessage(TcpMessagePtr)));
        disconnect(m_client, SIGNAL(connected()), this, SLOT(R_connected()));
        disconnect(m_client, SIGNAL(disconnected()), this, SLOT(R_disconnected()));
        disconnect(m_client, SIGNAL(readyRead()), this, SLOT(R_readyRead()));
        disconnect(m_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(R_error(QAbstractSocket::SocketError)));
        disconnect(m_client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(R_stateChanged(QAbstractSocket::SocketState)));
    }
}

void TcpImp::run()
{
    m_pSipParser = new TcpParser;

    m_initReceivedsign = 1;
    m_client           = new QTcpSocket();

    // connect(this,SIGNAL(S_SendTcpMessage(QByteArray)),this,SLOT(R_SendMessage(QByteArray)), Qt::QueuedConnection);
    connect(this, SIGNAL(S_sendMessage(QByteArray)), this, SLOT(R_SendMessage(QByteArray)), Qt::QueuedConnection);
    connect(this, SIGNAL(S_connectservice()), this, SLOT(R_connectservice()), Qt::QueuedConnection);
    connect(this, SIGNAL(S_ClearConnectHost()), this, SLOT(R_ClearConnectHost()), Qt::QueuedConnection);
    connect(this, SIGNAL(S_closeConnect()), this, SLOT(R_closeConnect()), Qt::QueuedConnection);

    InitSignal(true);
    InitConnect();
    connectserviceBase();

    exec();
}

void TcpImp::closeConnect()
{
    Q_EMIT S_closeConnect();
}

void TcpImp::R_closeConnect()
{
    if (m_client->isOpen())
        m_client->disconnectFromHost();
}
