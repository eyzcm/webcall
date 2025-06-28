#include "ElVmConnectedState.h"
#include "ElVmStatusProcess.h"
#include "protocol/tcpimp.h"

ElVmConnectedState::ElVmConnectedState(ElVmStatusProcess* process)
{

    m_process = process;
}

ElVmConnectedState::~ElVmConnectedState()
{
}

bool ElVmConnectedState::Connect()
{
    // m_process->CurrentTcpVMImp()->connectservice();

    m_process->TcpWork()->TcpConnect();

    // m_process.setStatus( ElVmStatus.CONNECT );
    return true;
}

bool ElVmConnectedState::TcpLogin(QByteArray msg)
{

    return true;
}

bool ElVmConnectedState::SendTcpMessage(QByteArray msg)
{

    return false;
}

bool ElVmConnectedState::VMConnect()
{
    m_process->CurrentTcpVMImp()->connectservice();
    return true;
}

bool ElVmConnectedState::VMLogin()
{

    return false;
}

bool ElVmConnectedState::VMSendMessage()
{
    return false;
}

bool ElVmConnectedState::ReConnect()
{
    return false;
}

bool ElVmConnectedState::LoginOut()
{
    return false;
}

bool ElVmConnectedState::InitState()
{
    return false;
}

bool ElVmConnectedState::VMAuth(QByteArray msg)
{
    return false;
}
