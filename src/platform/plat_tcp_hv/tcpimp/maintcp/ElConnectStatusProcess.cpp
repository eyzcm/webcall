#include "stdafx.h"
#include "ElConnectStatusProcess.h"
#include "utils/EL_Util_Time.h"
#include "ElConnectConnectedState.h"
#include "ElConnectConnectingState.h"
#include "ElConnectLoginState.h"
#include "tcp/ElConnectState.h"
#include "ElConnectSuccessState.h"
#include "protocol/tcpHvmanager.h"
#include "service/util_service.h"
#include "tcpimp/tcphvimp.h"
#include <QMetaObject>

ElConnectStatusProcess *ElConnectStatusProcess::m_connectProcess = nullptr;

ElConnectStatusProcess::ElConnectStatusProcess()
{
	m_sendTimer = NULL;
    m_reconnect  = false;
    m_currtcpImp = nullptr;

    m_init        = 0;
    m_maxTcpCount = 5;
    m_currentTcp  = m_maxTcpCount - 1;

    //connect(this, SIGNAL(S_LoginOk(int  )), this, SLOT(R_LoginOk(int  )));
	connect(this, SIGNAL(S_LoginOk()), this, SLOT(R_LoginOk()));

    m_states = (ElConnectState **)malloc(sizeof(ElConnectState *) * ELTCPlLOGINSTATUS::END);
    memset(m_states, 0, sizeof(ElConnectState *) * ELTCPlLOGINSTATUS::END);
    m_istate = NULL;

    m_states[ELTCPlLOGINSTATUS::CONNECT]    = new ElConnectConnectedState(this);
    m_states[ELTCPlLOGINSTATUS::CONNECTING] = new ElConnectConnectingState(this);
    m_states[ELTCPlLOGINSTATUS::LOGIN]      = new ElConnectLoginState(this);
    m_states[ELTCPlLOGINSTATUS::LOGINOK]    = new ElConnectSuccessState(this);
    m_currStatus                            = ELTCPlLOGINSTATUS::CONNECT;
    setStatus(ELTCPlLOGINSTATUS::CONNECT);
}

ElConnectStatusProcess::~ElConnectStatusProcess()
{
    for (int i = 0; i < ELTCPlLOGINSTATUS::END; i++)
    {
        if (m_states[i])
            delete m_states[i];
    }
    delete m_states;

    qDeleteAll(m_tcpimplist);
    m_tcpimplist.clear();
}

ElConnectStatusProcess *ElConnectStatusProcess::getInstance()
{
    if (m_connectProcess == nullptr)
        m_connectProcess = new ElConnectStatusProcess();
    return m_connectProcess;
}

//自动分配线程池连接
int ElConnectStatusProcess::AlloceTcpImp()
{
    if (m_currtcpImp != nullptr)
    {
        //GTESTLOG( "AlloceTcpImp disconnect");
        m_currtcpImp->ClearConnectHost();
        disconnect(CurrentTcpImp(), SIGNAL(S_connected()), this, SLOT(R_connected()));
        disconnect(CurrentTcpImp(), SIGNAL(S_disconnected()), this, SLOT(R_disconnected()));
        disconnect(CurrentTcpImp(), SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
        disconnect(CurrentTcpImp(), SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));

        //disconnect(gTcpHvManager, SIGNAL(S_SendTcpMessage(QByteArray)), CurrentTcpImp(), SIGNAL(S_SendMessage(QByteArray)));
		disconnect(gTcpHvManager, SIGNAL(S_SendTcpMessage(QByteArray)), this, SIGNAL(R_sendMessage(QByteArray)));
		
		//if(m_tcpimplist.size() > 2 )
			m_currtcpImp->ExitThread();

    }

    m_currentTcp++;
    m_currentTcp = m_currentTcp % m_maxTcpCount;

    int ret = 0;

	do {
		if (m_tcpimplist.size() > m_currentTcp)
		{
			int delflag = 0;
			for each (TcpHvImp * imp in m_tcpimplist)
			{
				if (!imp->Effective()) {
					delete imp;
					m_tcpimplist.removeOne( imp);
					delflag = 1;
				}

			}
			 
			if (delflag)
			{
				
				break;
			}

			

		}
		else
			break;
	} while (true);
	/*if (m_tcpimplist.size() > m_currentTcp)
    {
        m_currtcpImp = m_tcpimplist[m_currentTcp];
        ret          = 0;
    }
    else
	*/
    {
        TcpHvImp *tcpImp = new TcpHvImp(this);
        tcpImp->setThreadName("tcp");
        m_tcpimplist.append(tcpImp);
        m_currtcpImp = tcpImp;
        ret          = 1;
		tcpImp->start();
		
    }
    connect(CurrentTcpImp(), SIGNAL(S_connected()), this, SLOT(R_connected()));
    connect(CurrentTcpImp(), SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
	connect(CurrentTcpImp(), SIGNAL(S_disconnected()), this, SLOT(R_disconnected()));
    connect(CurrentTcpImp(), SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));
    connect(CurrentTcpImp(), SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()), Qt::QueuedConnection);

    //connect(gTcpHvManager, SIGNAL(S_SendTcpMessage(QByteArray)), CurrentTcpImp(), SIGNAL(S_SendMessage(QByteArray)), Qt::QueuedConnection);
	connect(gTcpHvManager, SIGNAL(S_SendTcpMessage(QByteArray)), this, SLOT(R_sendMessage(QByteArray)));
    //GTESTLOG( "AlloceTcpImp connect");

	if (m_sendTimer == nullptr)
	{
		m_sendTimer = new QTimer(this);
		m_sendTimer->setInterval(100);
		connect(m_sendTimer, SIGNAL(timeout()), this, SLOT(R_Timeout()));
		m_sendTimer->start();
	}


    return ret;
}

void ElConnectStatusProcess::R_Timeout()
{
	CurrentTcpImp()->R_Timeout();
}

bool ElConnectStatusProcess::reconnectservice()
{
    ET("ElConnectStatusProcess::reconnectservice");
    //	m_reconnect = true;

    m_currtcpImp->closeConnect();
    return false;
}

bool ElConnectStatusProcess::connectservice()
{
    /*	if( getStatus() >=  ElConnectStatus::CONNECTING )
    {
    return true;
    }
    */
    // if( m_init == 0 )

    //GTESTLOG("ElConnectStatusProcess::connectservice");

    if (m_currStatus >= ELTCPlLOGINSTATUS::LOGIN)
    {
        // return
        reconnectservice();
    }

    this->setStatus(ELTCPlLOGINSTATUS::CONNECT);

    AlloceTcpImp();

    CurrentTcpImp()->Init(m_loginaddress, m_loginport, m_logintype);
    if (CurrentTcpImp()->Loaded() == false)
    {

        this->setStatus(ELTCPlLOGINSTATUS::CONNECTING);
        CurrentTcpImp()->Loaded(true);
        CurrentTcpImp()->start();

        m_init = 1;
    }
    else
    {

        //调用状态类实现函数
        Connect();
        //将设成
        if (getStatus() == ELTCPlLOGINSTATUS::CONNECT)
        {
            this->setStatus(ELTCPlLOGINSTATUS::CONNECTING);
        }
    }

    return true;
}

void ElConnectStatusProcess::R_connected()
{
    if (CheckSender() == false)
        return;
    ET("Q_EMIT S_connected();");
    Q_EMIT S_connected();

	
    this->setStatus(ELTCPlLOGINSTATUS::LOGIN);
}

void ElConnectStatusProcess::R_disconnected()
{
    if (CheckSender() == false)
        return;
    //GTESTLOG( "ElConnectStatusProcess::R_disconnected" );
    ET("ElConnectStatusProcess::R_disconnected");
    if (m_reconnect)
    {
        this->setStatus(ELTCPlLOGINSTATUS::CONNECT);
        m_reconnect = false;
        connectservice();
    }
    else
        this->setStatus(ELTCPlLOGINSTATUS::CONNECT);
}

ELTCPlLOGINSTATUS ElConnectStatusProcess::getStatus()
{

    return m_currStatus;
}

void ElConnectStatusProcess::closeConnect()
{
    if (CurrentTcpImp())
    {
        //CurrentTcpImp()->closeConnect();
		CurrentTcpImp()->R_closeConnect();
        setStatus(ELTCPlLOGINSTATUS::CONNECT);
    }
}


void ElConnectStatusProcess::ForceClose()
{
	if (CurrentTcpImp())
	{
		CurrentTcpImp()->ForceClose();
		
	}
}

void ElConnectStatusProcess::setStatus(ELTCPlLOGINSTATUS value)
{

    if (value < ELTCPlLOGINSTATUS::END)
    {

        if (m_currStatus != value)
        {

            ET(QString("Q_EMIT S_MainTcpLoginStateChanged(%1,%2 )").arg(m_currStatus).arg(value));
            Q_EMIT S_MainTcpLoginStateChanged(m_currStatus, value);
        }
        m_currStatus = value;
        m_istate     = m_states[value];
        if (m_istate)
            m_istate->InitState();
    }
}

bool ElConnectStatusProcess::Connect()
{

    return m_istate->Connect();
}

bool ElConnectStatusProcess::TcpLogin(QByteArray msg)
{

    return m_istate->TcpLogin(msg);
}

bool ElConnectStatusProcess::SendTcpMessage(QByteArray msg)
{

    if ((CurrentTcpImp()->IsConnect() == false) && (getStatus() != ELTCPlLOGINSTATUS::CONNECT))
    {
        CurrentTcpImp()->ClearConnectHost();
        setStatus(ELTCPlLOGINSTATUS::CONNECT);
        Connect();
    }
    else if (getStatus() < ELTCPlLOGINSTATUS::LOGINOK)
    {
    }
    else
    {
        return m_istate->SendTcpMessage(msg);
    }
    return FALSE;
}

bool ElConnectStatusProcess::ReConnect()
{

    return m_istate->ReConnect();
}

bool ElConnectStatusProcess::LoginOut()
{

    return m_istate->LoginOut();
}

bool ElConnectStatusProcess::InitState()
{
    return m_istate->InitState();
}

TcpHvImp *ElConnectStatusProcess::CurrentTcpImp()
{
    return m_currtcpImp;
}

bool ElConnectStatusProcess::Init(QString address, quint16 port, QString type)
{
    if (address.isEmpty())
    {
        return false;
    }
    m_logintype    = type;
    m_loginaddress = address;
    m_loginport    = port;
    ////服务器地址或者需要发送的数据为空直接返回
    // client.connectToHost(address, port);//建立一个TCP连接，主机地址是m_httpAddress，端口号是httpPort
    // client.setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。
    return true;
}

void ElConnectStatusProcess::R_LoginOk()
{

    setStatus(ELTCPlLOGINSTATUS::LOGINOK);
}

void ElConnectStatusProcess::LoginOk()
{
    Q_EMIT S_LoginOk();
}
bool ElConnectStatusProcess::CheckSender()
{
    TcpHvImp *tcpimp = (TcpHvImp *)QObject::sender();
    if ((void *)tcpimp != (void *)CurrentTcpImp())
    {
        return false;
    }

    return true;
}

void ElConnectStatusProcess::R_NetWorkError(int code, QString reason)
{

    ET("ElConnectStatusProcess::R_NetWorkError");
    if (CheckSender() == false)
        return;
    // CurrentTcpImp()->ClearConnectHost();
    // CurrentTcpImp()->disconnect(  );

    setStatus(ELTCPlLOGINSTATUS::CONNECT);
    if (m_reconnect)
    {
        //GTESTLOG( QString("R_NetWorkError code=%1,reason%2").arg(code).arg(reason) );
        m_reconnect = false;
        connectservice();
    }
    else
    {
        //GTESTLOG( QString("S_NetWorkError code=%1,reason%2").arg(code).arg(reason) );
        Q_EMIT S_NetWorkError(code, reason);
    }
}

void ElConnectStatusProcess::R_ReceivedMessage(TcpMessagePtr pmsg)
{

	TcpMessage * msg = pmsg.data();
    if (CheckSender() == false)
        return;
	DEFTCPMESSAGE(tmsg, msg);
    Q_EMIT S_ReceivedMessage(tmsg);
	GLOG("ElConnectStatusProcess::R_ReceivedMessage");
	if (getStatus() == ELTCPlLOGINSTATUS::LOGINOK)
	{
	/*
		if (msg->ContentLength() < 30)
		{
			static int ii = 0;
			ii++;

			if ((ii % 3) == 0)

			{
				QString data = QString("R /Message 1202 1207991787\r\n") +
					QString("Content-Type:json\r\n") +
					QString("\r\n") +
					QString("{\"msg\":\"<div class=\\\"form-content\\\" data-flag=\\\"form-info\\\">\\r\\n<div class=\\\"form-title\\\">表单</div>\\r\\n<form class=\\\"form-main\\\">\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"mobile\\\" placeholder=\\\"手机号码\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"extColumn1\\\" placeholder=\\\"维修产品\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"extColumn15\\\" placeholder=\\\"故障现象\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"extColumn6\\\" placeholder=\\\"详细地址（市+行政区+小区名）\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n</form>\\r\\n<div class=\\\"btn\\\">\\r\\n<span class=\\\"btnClick\\\" >提交表单</span>\\r\\n</div>\\r\\n</div>\",\"chatId\":\"433327019401487360\",\"dst\":\"\",\"fromUserId\":\"AI-xingyu\",\"msgId\":\"433328278485408768\",\"recorderId\":433328278485408768,\"type\":\"message\",\"richText\":\"1\",\"robot\":2,\"viewUserId\":null,\"extBodyJson\":\"\",\"colleagueMessage\":false,\"subType\":0,\"exts\":{\"robot\":2,\"node\":\"trust6\",\"robotLogId\":\"433327019778966531\",\"system_monitor_message\":true,\"system_monitor_users\":[\"18996173092\",\"15826484685\"]},\"timestamp\":\"1675851112482\"}");

				SendTcpMessage(data.toUtf8());
			}

			else if ((ii % 3) == 1)

			{

				QString data = QString("R /Message?callbackId=50002&chatId=447836470946826240&questioId=447836471060072448&richText=1&robot=2&robotLogId=447836471877961729&type=message 42 seq_12\r\n") +
					QString("\r\n") +
					QString("测试AI延迟为0时，消息是否发送");
				SendTcpMessage(data.toUtf8());
			}


			else if ((ii % 3) == 2)
			{

				QString data = QString("R /Message?callbackId=50002&chatId=447836470946826240&questioId=447836471060072448&richText=1&robot=2&robotLogId=447836471877961729&type=message 42 seq_12\r\n") +
					QString("\r\n") +
					QString("测试AI延迟为0时，消息是否发送") +
					QString("R /Message 1202 1207991787\r\n") +
					QString("Content-Type:json\r\n") +
					QString("\r\n") +
					QString("{\"msg\":\"<div class=\\\"form-content\\\" data-flag=\\\"form-info\\\">\\r\\n<div class=\\\"form-title\\\">表单</div>\\r\\n<form class=\\\"form-main\\\">\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"mobile\\\" placeholder=\\\"手机号码\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"extColumn1\\\" placeholder=\\\"维修产品\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"extColumn15\\\" placeholder=\\\"故障现象\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n<div>\\r\\n<input class=\\\"input\\\" type=\\\"text\\\" name=\\\"extColumn6\\\" placeholder=\\\"详细地址（市+行政区+小区名）\\\" autocomplete=\\\"off\\\">\\r\\n</div>\\r\\n</form>\\r\\n<div class=\\\"btn\\\">\\r\\n<span class=\\\"btnClick\\\" >提交表单</span>\\r\\n</div>\\r\\n</div>\",\"chatId\":\"433327019401487360\",\"dst\":\"\",\"fromUserId\":\"AI-xingyu\",\"msgId\":\"433328278485408768\",\"recorderId\":433328278485408768,\"type\":\"message\",\"richText\":\"1\",\"robot\":2,\"viewUserId\":null,\"extBodyJson\":\"\",\"colleagueMessage\":false,\"subType\":0,\"exts\":{\"robot\":2,\"node\":\"trust6\",\"robotLogId\":\"433327019778966531\",\"system_monitor_message\":true,\"system_monitor_users\":[\"18996173092\",\"15826484685\"]},\"timestamp\":\"1675851112482\"}");


				SendTcpMessage(data.toUtf8());
			}
		}
		*/
		
		
	}

    return;
    {

        TcpHvImp *sendobj = (TcpHvImp *)QObject::sender();
        if (sendobj == m_currtcpImp)
        {
            if (getStatus() == ELTCPlLOGINSTATUS::LOGIN)
            {
                if (msg->Uri() == "/Auth")
                {
					DEFTCPMESSAGE(tmsg, msg);
                    Q_EMIT S_ReceivedMessage(tmsg);
                }
            }
            else if (getStatus() == ELTCPlLOGINSTATUS::LOGINOK)
            {

				QString data = QString("R /Message?callbackId=50002&chatId=447836470946826240&questioId=447836471060072448&richText=1&robot=2&robotLogId=447836471877961729&type=message 42 seq_12\r\n") +
					QString("\r\n") +
					tr("测试AI延迟为0时，消息是否发送");
				SendTcpMessage(data.toUtf8());

				DEFTCPMESSAGE(tmsg, msg);
                Q_EMIT S_ReceivedMessage(tmsg);
            }
        }
    }
}
void ElConnectStatusProcess::R_sendMessage(QByteArray body)
{
    CurrentTcpImp()->R_SendMessage(body);
/*
	QMetaObject::invokeMethod(CurrentTcpImp(), "R_Timeout1", Qt::QueuedConnection);
	QMetaObject::invokeMethod(CurrentTcpImp(), "R_SendMessage", Qt::QueuedConnection, 
		 Q_ARG(QByteArray , body));
*/
		 

	
}
