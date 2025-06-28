#include "ElVmLoginState.h"
#include "ElVmStatusProcess.h"
#include "tcpimp/tcphvimp.h"

ElVmLoginState::ElVmLoginState(ElVmStatusProcess* process)
{

    m_process = process;
}

ElVmLoginState::~ElVmLoginState()
{
}

bool ElVmLoginState::VMConnect()
{
    // m_process->Connect()
    // m_process.setStatus( ElVmStatus.CONNECT );
    return true;
    ;
}

bool ElVmLoginState::VMLogin()
{
    m_process->TcpWork()->Login();
    return true;
}

bool ElVmLoginState::VMLogin(QByteArray msg)
{
    m_process->CurrentTcpVMImp()->SendTcpMessage(msg);
    return true;
}

bool ElVmLoginState::VMSendMessage(QByteArray msg)
{

    return false;
}

bool ElVmLoginState::ReConnect()
{
    return false;
}

bool ElVmLoginState::LoginOut()
{
    return false;
}

bool ElVmLoginState::InitState()
{
    return false;
}

bool ElVmLoginState::VMAuth(QByteArray msg)
{
    return false;
}
