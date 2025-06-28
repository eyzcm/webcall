#include "TcpHvImp.h"
#include <qDebug>
#include "utils/EL_Util_Time.h"
#include "TcpClient.h"

#include "ClientTcpImp.h"
#include "ThreadHv.h"
#include "service/util_service.h"

using namespace hv;

TcpHvImp::TcpHvImp(QObject* process) : ElThreadCheck()
{
	qsrand(time(NULL));
	qDebug() << "moveToThread TcpHvImp" << endl;
	moveToThread(this);

	m_isConnect = false;
	m_loaded = false;
	m_initReceivedsign = 0;
	m_imp = nullptr;

	m_pSipParser = new TcpParser;

	m_imp = new ClientTcpImp();
	m_effective = true;



}

void TcpHvImp::R_Timeout()
{
	return;
	/* 测试服务器通讯

	QMutexLocker locker(&m_mutex);
	if (m_bufdata.length() > 0)
	{
		for (;;)
		{
			int r = qrand() % 100+300;
			if (m_bufdata.length() <= r)
			{
				//Write(m_bufdata);
				//m_bufdata.clear();
				static int i = 0;
				i++;
				if (i % 5 == 0)
				{
					Write(m_bufdata);
					m_bufdata.clear();
				}
			}
			else
			{
				if ((r<=0)||(m_bufdata[r-1] == 'R'))
				{
					continue;
				}
				QByteArray tmp = m_bufdata.left(r);
				m_bufdata = m_bufdata.right(m_bufdata.length() - r);
				Write(tmp);
			}
			break;
		}
	}
	*/
}

TcpHvImp::~TcpHvImp()
{
	Clear();
}
void TcpHvImp::R_ClearConnectHost()
{
	InitSignal(false);
	m_isConnect = false;
}

void TcpHvImp::ClearConnectHost()
{

	m_initReceivedsign = 0;

	// disconnect()
	Q_EMIT S_ClearConnectHost();
}

void TcpHvImp::R_connectservice()
{
	InitSignal(true);
	connectserviceBase();
}
void TcpHvImp::connectserviceBase()
{

}

void TcpHvImp::Clear()
{
	m_isConnect = false;
	this->disconnect();
	if (m_pSipParser)
	{
		m_pSipParser->deleteLater();
		m_pSipParser = NULL;
	}
	if (m_imp)
	{
		m_imp->deleteLater();
		m_imp = NULL;
	}

}

bool TcpHvImp::Init(QString address, quint16 port, QString type)
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

bool TcpHvImp::IsConnect()
{
	return m_isConnect;
}

void TcpHvImp::SendMessageBase(QByteArray msg)
{
	QMutexLocker locker(&m_mutex);
	if (!msg.isEmpty())
	{
		//m_bufdata.append(msg);
		Write(msg);
	}

}

void TcpHvImp::SendTcpMessage(const QByteArray& msg)
{
	GTESTLOG("TcpHvImp::SendTcpMessage");
	SendMessageBase(msg);
	//Q_EMIT S_sendMessage(msg);
}

void TcpHvImp::R_connected()
{

	m_isConnect = true;
	Q_EMIT S_connected();

}

void TcpHvImp::R_readyRead()
{
	/*
		if (m_client && m_client->bytesAvailable())
		{
			if (m_pSipParser)
			{
				m_pSipParser->Parse(m_client->readAll());
			}
		}
		*/
}

void TcpHvImp::R_NetWorkError(int code, QString msg)
{
	Q_EMIT S_NetWorkError(code, msg);
}

void TcpHvImp::R_disconnected()
{
	ET("TcpHvImp::R_disconnected()");
	m_isConnect = false;
	// ClearConnectHost();
	Q_EMIT S_disconnected();
}

void TcpHvImp::R_SendMessage(QByteArray msg)
{
	ET("TcpHvImp::R_SendMessage");
	SendMessageBase(msg);
}

void TcpHvImp::R_Tcp_SendMessage(QByteArray msg)
{
	ET("TcpHvImp::R_Tcp_SendMessage");
	SendMessageBase(msg);
}

void TcpHvImp::Write(QByteArray buf)
{

	if (m_isConnect && m_imp)
	{

		m_imp->tcp_client_send(buf.data(), buf.length());

		GPROTOCOLLOG(QString("tcp send protocol:%1").arg(QString::fromUtf8(buf)), "TcpHvImp");
	}
	else
	{
		GERRORLOG(QString("tcp send protocol error！ 协议内容:%1").arg(QString::fromUtf8(buf)));
	}

}

void TcpHvImp::connectservice()
{
	Q_EMIT S_connectservice();
}

void TcpHvImp::InitSignal(bool isLoad)
{
	if (isLoad)
	{
		ET("void TcpHvImp::InitSignal(true)");
		connect(m_pSipParser, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()));

		connect(m_pSipParser, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SIGNAL(S_ReceivedMessage(TcpMessagePtr)));

		connect(m_imp, SIGNAL(S_connected()), this, SLOT(R_connected()));
		connect(m_imp, SIGNAL(S_disconnected()), this, SLOT(R_disconnected()));
		connect(m_imp, SIGNAL(S_message(void *, int)), this, SLOT(R_message(void *, int)));

		connect(m_imp, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));


		/*
		connect(m_imp, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(R_error(QAbstractSocket::SocketError)));
		connect(m_imp, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(R_stateChanged(QAbstractSocket::SocketState)));
		*/

	}
	else
	{
		ET("void TcpHvImp::InitSignal(false)");

		disconnect(m_pSipParser, SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()));
		disconnect(m_pSipParser, SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SIGNAL(S_ReceivedMessage(TcpMessagePtr)));
		disconnect(m_imp, SIGNAL(S_connected()), this, SLOT(R_connected()));
		disconnect(m_imp, SIGNAL(S_disconnected()), this, SLOT(R_disconnected()));
		disconnect(m_imp, SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));
		disconnect(m_imp, SIGNAL(S_message()), this, SLOT(R_message()));
		/*disconnect(m_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(R_error(QAbstractSocket::SocketError)));
		disconnect(m_client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(R_stateChanged(QAbstractSocket::SocketState)));
		*/

	}
}
#if 0
void TcpHvImp::run()
{

	TcpClient cli;
	int connfd = cli.createsocket(m_port, m_host.toStdString().c_str());
	if (connfd < 0) {
		return;
	}
	TcpHvImp * imp = this;
	printf("client connect to port %d, connfd=%d ...\n", m_port, connfd);
	cli.onConnection = [&cli](const SocketChannelPtr& channel) {

		std::string peeraddr = channel->peeraddr();
		if (channel->isConnected()) {
			printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());

			// send(time) every 3s
			setInterval(3000, [channel](TimerID timerID) {
				if (channel->isConnected()) {
					if (channel->isWriteComplete()) {
						char str[DATETIME_FMT_BUFLEN] = { 0 };
						datetime_t dt = datetime_now();
						datetime_fmt(&dt, str);
						//channel->write(str);
					}
				}
				else {
					;// killTimer(timerID);
				}
			});
		}
		else {
			printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
		}
		if (cli.isReconnect()) {
			printf("reconnect cnt=%d, delay=%d\n", cli.reconn_setting->cur_retry_cnt, cli.reconn_setting->cur_delay);
		}
	};
	cli.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
		printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
	};

#if TEST_RECONNECT
	// reconnect: 1,2,4,8,10,10,10...
	reconn_setting_t reconn;
	reconn_setting_init(&reconn);
	reconn.min_delay = 1000;
	reconn.max_delay = 10000;
	reconn.delay_policy = 2;
	cli.setReconnect(&reconn);
#endif

#if TEST_TLS
	cli.withTLS();
#endif

	cli.start();


}

#else

void TcpHvImp::run()
{

	m_imp->tcp_client_new(NULL, m_host.toStdString().c_str(), m_port);
	m_initReceivedsign = 1;
	m_imp->tcp_client_connect(m_host.toStdString().c_str(), m_port, 0);
	// connect(this,SIGNAL(S_SendTcpMessage(QByteArray)),this,SLOT(R_SendMessage(QByteArray)), Qt::QueuedConnection);
	connect(this, SIGNAL(S_SendMessage(QByteArray)), this, SLOT(R_SendMessage(QByteArray)), Qt::QueuedConnection);
	connect(this, SIGNAL(S_connectservice()), this, SLOT(R_connectservice()), Qt::QueuedConnection);
	connect(this, SIGNAL(S_ClearConnectHost()), this, SLOT(R_ClearConnectHost()), Qt::QueuedConnection);
	connect(this, SIGNAL(S_closeConnect()), this, SLOT(R_closeConnect()), Qt::QueuedConnection);
	connect(this, SIGNAL(S_ForceClose()), this, SLOT(R_ForceClose()), Qt::QueuedConnection);
	//connect(this, SIGNAL(S_Timeout()), this, SLOT(R_Timeout()), Qt::QueuedConnection);



	InitSignal(true);

	m_imp->tcp_client_run();
    //m_imp->tcp_client_free();
	/*
	ThreadHv * hv = new ThreadHv(m_imp );
	hv->start();
	*/
    //InitConnect();
	m_effective = false;
	//connectserviceBase();

   //exec();
}
#endif


void TcpHvImp::closeConnect()
{
	Q_EMIT S_closeConnect();
}

void TcpHvImp::R_closeConnect()
{

}


void TcpHvImp::ForceClose()
{
	ClientTcpImp::m_forceClose = true;
	Q_EMIT S_ForceClose();
}

void TcpHvImp::R_ForceClose()
{

}
void TcpHvImp::R_message(void * buf, int len)
{
	QMutexLocker locker(&m_mutex);
	char * msg = (char *)malloc(len + 1);
	memcpy(msg, buf, len);
	msg[len] = '\0';
    GTESTLOG(QString("tcp recv src protocol:%1").arg((char *)msg));
	QByteArray tt;
	tt.append((char *)buf, len);
	if (m_pSipParser)
	{
		m_pSipParser->Parse(tt);
	}
	delete msg;

}

void TcpHvImp::DoTimeout()
{
	Q_EMIT S_Timeout();
}

void TcpHvImp::ExitThread()
{
	m_imp->tcp_client_stop();
}
