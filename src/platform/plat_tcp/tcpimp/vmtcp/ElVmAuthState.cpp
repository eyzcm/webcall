#include "ElVmAuthState.h"
#include "ElVmStatusProcess.h"
#include "protocol/tcpimp.h"

ElVmAuthState::ElVmAuthState(ElVmStatusProcess* process)
{

    m_process = process;
}

ElVmAuthState::~ElVmAuthState()
{
}

bool ElVmAuthState::VMConnect()
{
    // m_process->Connect()
    // m_process.setStatus( ElVmStatus.CONNECT );
    return true;
    ;
}

bool ElVmAuthState::VMLogin()
{
    m_process->TcpWork()->Login();
    return true;
}

bool ElVmAuthState::VMLogin(QByteArray msg)
{
    return false;
}

bool ElVmAuthState::VMSendMessage(QByteArray msg)
{

    return false;
}

bool ElVmAuthState::ReConnect()
{
    return false;
}

bool ElVmAuthState::LoginOut()
{
    return false;
}

bool ElVmAuthState::InitState()
{
    return false;
}

bool ElVmAuthState::VMAuth(QByteArray msg)
{
    m_process->CurrentTcpVMImp()->SendTcpMessage(msg);
    return true;
}
