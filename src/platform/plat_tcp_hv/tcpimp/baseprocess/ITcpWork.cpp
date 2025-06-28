#include "ITcpWork.h"
#include "protocol_biz/VisitorMonitorLoginTcpHvAgent.h"

ITcpWork::ITcpWork(void)
{
}

ITcpWork::~ITcpWork(void)
{
}

void ITcpWork::Init()
{
    connect(this, SIGNAL(S_TcpConnect()), this, SLOT(R_TcpConnect()));
    connect(this, SIGNAL(S_Login()), this, SLOT(R_Login()));
}
void ITcpWork::InitSignalConnect(QObject *obj)
{
    if (obj)
    {
    }
    else
    {
        Init();
    }
}
void ITcpWork::TcpConnect()
{
    Q_EMIT S_TcpConnect();
}

void ITcpWork::Login()
{
    Q_EMIT S_Login();
}

void ITcpWork::R_TcpConnect()
{

    gVisitorMonitorLoginTcpHvAgent->DoTryVMLoginConnect();
}

void ITcpWork::R_Login()
{

    gVisitorMonitorLoginTcpHvAgent->DoExecVMAuthLogin();
}

void ITcpWork::R_DispConnect()
{
}

void ITcpWork::DispConnect()
{
    Q_EMIT S_DispConnect();
}

void ITcpWork::R_connected()
{
    gVisitorMonitorLoginTcpHvAgent->DoTryVMLoginConnect();
}

void ITcpWork::SetStatus(int status)
{
}
