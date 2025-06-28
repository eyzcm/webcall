#include "ElVmConnectingState.h"
#include "ElVmStatusProcess.h"
#include "protocol/tcpimp.h"

ElVmConnectingState::ElVmConnectingState(ElVmStatusProcess* process)
{
    m_process = process;
}

ElVmConnectingState::~ElVmConnectingState()
{
}

bool ElVmConnectingState::Connect()
{
    m_process->CurrentTcpVMImp()->connectservice();

    // m_process.setStatus( ElVmStatus.CONNECT );
    return true;
}

bool ElVmConnectingState::TcpLogin(QByteArray msg)
{

    return true;
}

bool ElVmConnectingState::SendTcpMessage(QByteArray msg)
{

    return false;
}

bool ElVmConnectingState::VMConnect()
{

    return false;
}

bool ElVmConnectingState::VMLogin()
{

    return false;
}

bool ElVmConnectingState::VMSendMessage()
{
    return false;
}

bool ElVmConnectingState::ReConnect()
{
    return false;
}

bool ElVmConnectingState::LoginOut()
{
    return false;
}

bool ElVmConnectingState::InitState()
{
    return false;
}

bool ElVmConnectingState::VMAuth(QByteArray msg)
{
    return false;
}
