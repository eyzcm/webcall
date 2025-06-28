#include "stdafx.h"
#include "ElConnectLoginState.h"
#include "ElConnectStatusProcess.h"
#include "tcpimp/tcphvimp.h"


ElConnectLoginState::ElConnectLoginState(ElConnectStatusProcess* process)
{

    m_process = process;
}

ElConnectLoginState::~ElConnectLoginState()
{
}

bool ElConnectLoginState::Connect()
{
    // m_process->Connect();
    // m_process.setStatus( ElConnectStatus.CONNECT );
    return true;
    ;
}

bool ElConnectLoginState::TcpLogin(QByteArray msg)
{
    m_process->CurrentTcpImp()->SendTcpMessage(msg);
    return true;
}

bool ElConnectLoginState::SendTcpMessage(QByteArray msg)
{

    return false;
}

bool ElConnectLoginState::VMConnect()
{

    return false;
}

bool ElConnectLoginState::VMLogin()
{

    return false;
}

bool ElConnectLoginState::VMSendMessage()
{
    return false;
}

bool ElConnectLoginState::ReConnect()
{
    return false;
}

bool ElConnectLoginState::LoginOut()
{
    return false;
}

bool ElConnectLoginState::InitState()
{
    return false;
}
