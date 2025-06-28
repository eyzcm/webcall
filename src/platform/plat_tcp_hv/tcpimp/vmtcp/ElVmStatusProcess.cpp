#include "ElVmStatusProcess.h"
#include "utils/EL_Util_Time.h"
#include "ElVmAuthState.h"
#include "ElVmConnectedState.h"
#include "ElVmConnectingState.h"
#include "ElVmLoginState.h"
#include "ElVmState.h"
#include "ElVmSuccessState.h"
#include "protocol_biz/VisitorMonitorLoginTcpHvAgent.h"
#include "protocol/visitormonitortcpHvmanager.h"
#include "tcpimp/tcphvimp.h"


ElVmStatusProcess *ElVmStatusProcess::m_connectProcess = nullptr;

ElVmStatusProcess::ElVmStatusProcess()
{
    m_currvmTcpImp = nullptr;
    m_init         = 0;
    m_maxTcpCount  = 5;
    m_loginCount   = 0;
    m_currentTcp   = m_maxTcpCount - 1;
    m_tcpwork      = gVisitorMonitorLoginTcpHvAgent;
    m_tcpwork->InitSignalConnect(nullptr);
    m_currStatus = ElVmStatus::VMCONNECT;
    connect(this, SIGNAL(S_LoginOk()), this, SLOT(R_LoginOk()));
    m_states = (ElVmState **)malloc(sizeof(ElVmState *) * ElVmStatus::VM_END);
    memset(m_states, 0, sizeof(ElVmState *) * ElVmStatus::VM_END);
    m_istate                           = NULL;
    m_states[ElVmStatus::VMCONNECT]    = new ElVmConnectedState(this);
    m_states[ElVmStatus::VMAUTH]       = new ElVmAuthState(this);
    m_states[ElVmStatus::VMCONNECTING] = new ElVmConnectingState(this);
    m_states[ElVmStatus::VMLOGIN]      = new ElVmLoginState(this);
    m_states[ElVmStatus::VMLOGINING]   = new ElVmConnectingState(this);
    m_states[ElVmStatus::VMLOGINOK]    = new ElVmSuccessState(this);
    setStatus(ElVmStatus::VMCONNECT);
    m_isUpdate = 0;
}

ElVmStatusProcess::~ElVmStatusProcess()
{
    for (int i = 0; i < ElVmStatus::VM_END; i++)
    {
        if (m_states[i])
            delete m_states[i];
    }
    delete m_states;
    qDeleteAll(m_vmtcpimplist);
    m_vmtcpimplist.clear();
}

ElVmStatusProcess *ElVmStatusProcess::getInstance()
{
    if (m_connectProcess == nullptr)
        m_connectProcess = new ElVmStatusProcess();
    return m_connectProcess;
}

int ElVmStatusProcess::AlloceTcpImp()
{
    if (m_currvmTcpImp != nullptr)
    {
        m_currvmTcpImp->ClearConnectHost();
        disconnect(CurrentTcpVMImp(), SIGNAL(S_connected()), this, SLOT(R_connected()));
        disconnect(CurrentTcpVMImp(), SIGNAL(S_disconnected()), this, SLOT(R_disconnected()));
        disconnect(CurrentTcpVMImp(), SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
        disconnect(CurrentTcpVMImp(), SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));
        disconnect(gVisitorMonitorTcpHvManager, SIGNAL(S_TCP_SendTcpMessage(QByteArray)), CurrentTcpVMImp(), SIGNAL(S_SendMessage(QByteArray)));
		m_currvmTcpImp->ExitThread();
    }
    m_currentTcp++;
    m_currentTcp = m_currentTcp % m_maxTcpCount;

    int ret = 0;
    do {
        if (m_vmtcpimplist.size() > m_currentTcp)
        {
			int delflag = 0;
            for each (TcpHvImp * imp in m_vmtcpimplist)
            {
                if (!imp->Effective()) {
                    //imp->deleteLater();
                    m_vmtcpimplist.removeOne( imp);
					delflag = 1;
                }

            }
			if ( delflag )break;

        }
        else
            break;
    } while (true);
	/*
    if (m_vmtcpimplist.size() > m_currentTcp)
    {
        m_currvmTcpImp = m_vmtcpimplist[m_currentTcp];
        ret            = 0;
    }
    else
	*/
    {
        TcpHvImp *tcpImp = new TcpHvImp(this);
        tcpImp->setThreadName("vm");
        m_vmtcpimplist.append(tcpImp);
        m_currvmTcpImp = tcpImp;
        ret            = 1;
    }
    connect(CurrentTcpVMImp(), SIGNAL(S_connected()), this, SLOT(R_connected()));
    connect(CurrentTcpVMImp(), SIGNAL(S_disconnected()), this, SLOT(R_disconnected()));
    connect(CurrentTcpVMImp(), SIGNAL(S_ReceivedMessage(TcpMessagePtr)), this, SLOT(R_ReceivedMessage(TcpMessagePtr)));
    connect(CurrentTcpVMImp(), SIGNAL(S_NetWorkError(int, QString)), this, SLOT(R_NetWorkError(int, QString)));
    connect(CurrentTcpVMImp(), SIGNAL(S_MessageParsingFailed()), this, SIGNAL(S_MessageParsingFailed()), Qt::QueuedConnection);
    connect(gVisitorMonitorTcpHvManager, SIGNAL(S_TCP_SendTcpMessage(QByteArray)), CurrentTcpVMImp(), SIGNAL(S_sendMessage(QByteArray)), Qt::QueuedConnection);
    return ret;
}

bool ElVmStatusProcess::reconnectVMservice()
{
    CurrentTcpVMImp()->ClearConnectHost();
    setStatus(ElVmStatus::VMCONNECT);
    return false;
}

bool ElVmStatusProcess::connectVMservice()
{
    ET(QString("ElVmStatusProcess::connectVMservice[%1]").arg(getStatus()));
    if (m_currStatus >= ElVmStatus::VMAUTH)
    {
        reconnectVMservice();
    }
    if (m_isUpdate == 0)
    {
        if (getStatus() == ElVmStatus::VMCONNECTING)
        {
            setStatus(ElVmStatus::VMCONNECT);
        }
    }
    else
        m_isUpdate = 0;
    if (getStatus() >= ElVmStatus::VMCONNECTING)
    {
        return true;
    }
    AlloceTcpImp();
    if (m_currvmTcpImp->Loaded() == false)
    {
        CurrentTcpVMImp()->Init(m_vmaddress, m_vmport, m_vmtype);
        this->setStatus(ElVmStatus::VMCONNECTING);
        CurrentTcpVMImp()->Loaded(true);
        CurrentTcpVMImp()->start();

        m_init = 1;
    }
    else
    {
        //调用状态类实现函数
        VMConnect();
        //将设成
        if (getStatus() == ElVmStatus::VMCONNECT)
        {
            this->setStatus(ElVmStatus::VMCONNECTING);
        }
    }
    return true;
}

void ElVmStatusProcess::R_connected()
{
    if (!CheckSender())
        return;
    ET("ElVmStatusProcess::R_connected()");

    this->setStatus(ElVmStatus::VMAUTH);
    Q_EMIT S_connected();
}

ElVmStatus ElVmStatusProcess::getStatus()
{
    return m_currStatus;
}

void ElVmStatusProcess::setStatus(ElVmStatus value)
{
    if (value < ElVmStatus::VM_END)
    {
        m_currStatus = value;
        if (m_istate != m_states[value])
        {
            m_loginCount = 0;
            m_isUpdate   = 1;
        }
        m_istate = m_states[value];
        if (m_istate)
            m_istate->InitState();
    }
}

bool ElVmStatusProcess::VMConnect()
{
    return m_istate->VMConnect();
}

bool ElVmStatusProcess::VMLogin()
{
    return m_istate->VMLogin();
}

bool ElVmStatusProcess::VMLogin(QByteArray msg)
{
    bool bret = false;
    ET("ElVmStatusProcess::VMLogin");
    m_loginCount++;
    if (m_loginCount < 4)
    {
        bret = m_istate->VMLogin(msg);
        if (getStatus() == ElVmStatus::VMLOGIN)
            setStatus(ElVmStatus::VMLOGINING);
        return bret;
    }
    return false;
}

bool ElVmStatusProcess::VMSendTcpMessage(QByteArray msg)
{
 
    if (CurrentTcpVMImp()->IsConnect() == false)
    {
        CurrentTcpVMImp()->ClearConnectHost();
        setStatus(ElVmStatus::VMCONNECT);
        VMConnect();
    }
    else if (getStatus() < ElVmStatus::VMLOGINOK)
    {
    }
    else
        return m_istate->VMSendMessage(msg);

    return true;
}

bool ElVmStatusProcess::ReConnect()
{
    return m_istate->ReConnect();
}

bool ElVmStatusProcess::LoginOut()
{
    return m_istate->LoginOut();
}

bool ElVmStatusProcess::InitState()
{
    return m_istate->InitState();
}

bool ElVmStatusProcess::VMAuth(QByteArray msg)
{
    ET("ElVmStatusProcess::VMAuth");
    m_loginCount++;
    if (m_loginCount < 4)
    {
        bool ret = m_istate->VMAuth(msg);
        setStatus(ElVmStatus::VMLOGIN);
        return ret;
    }
    return false;
}

TcpHvImp *ElVmStatusProcess::CurrentTcpVMImp()
{
    return m_currvmTcpImp;
}

bool ElVmStatusProcess::InitVM(QString address, quint16 port, QString type)
{
    if (address.isEmpty())
    {
        return false;
    }
    m_vmtype    = type;
    m_vmaddress = address;
    m_vmport    = port;
    if (m_currvmTcpImp)
    {
        m_currvmTcpImp->ClearConnectHost();
    }
    return true;
}

bool ElVmStatusProcess::CheckSender()
{
    TcpHvImp *tcpimp = (TcpHvImp *)QObject::sender();
    if (tcpimp != CurrentTcpVMImp())
    {
        return false;
    }
    return true;
}

void ElVmStatusProcess::R_NetWorkError(int code, QString reason)
{
    if (!CheckSender())
        return;
    ET(QString("ElVmStatusProcess::R_NetWorkError[%1]").arg(reason));
}

void ElVmStatusProcess::R_ReceivedMessage(TcpMessagePtr msg)
{
	DEFTCPMESSAGE(tmsg, msg.data());
    Q_EMIT S_ReceivedMessage(tmsg);
}

void ElVmStatusProcess::R_LoginOk()
{
    ET("ElVmStatusProcess::R_LoginOk()");
    setStatus(ElVmStatus::VMLOGINOK);
}

void ElVmStatusProcess::LoginOk()
{
    Q_EMIT S_LoginOk();
}

void ElVmStatusProcess::R_disconnected()
{
    if (!CheckSender())
        return;
    ET("ElVmStatusProcess::R_disconnected()");
    setStatus(ElVmStatus::VMCONNECT);
}

void ElVmStatusProcess::closeVMservice()
{
    if (m_currvmTcpImp)
    {
        m_currvmTcpImp->ClearConnectHost();
    }
    setStatus(ElVmStatus::VMCONNECT);
}
