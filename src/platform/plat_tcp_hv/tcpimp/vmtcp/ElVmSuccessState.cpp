#include "ElVmSuccessState.h"
#include "ElVmStatusProcess.h"
#include "tcpimp/tcphvimp.h"

ElVmSuccessState::ElVmSuccessState(ElVmStatusProcess* process)
{

    m_process = process;
}

ElVmSuccessState::~ElVmSuccessState()
{
}

bool ElVmSuccessState::VMLogin(QByteArray msg)
{

    return true;
}

bool ElVmSuccessState::VMLogin()
{
    return false;
}

bool ElVmSuccessState::VMAuth(QByteArray msg)
{
    return false;
}

bool ElVmSuccessState::VMSendMessage(QByteArray msg)
{

    m_process->CurrentTcpVMImp()->SendTcpMessage(msg);

    return false;
}

bool ElVmSuccessState::VMConnect()
{

    return false;
}

bool ElVmSuccessState::ReConnect()
{
    return false;
}

bool ElVmSuccessState::LoginOut()
{
    return false;
}

bool ElVmSuccessState::InitState()
{
    return false;
}
