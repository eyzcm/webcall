#include "stdafx.h"
#include "ElConnectSuccessState.h"
#include "ElConnectStatusProcess.h"
#include "protocol/tcpimp.h"

ElConnectSuccessState::ElConnectSuccessState(ElConnectStatusProcess* process)
{

    m_process = process;
}

ElConnectSuccessState::~ElConnectSuccessState()
{
}

bool ElConnectSuccessState::Connect()
{
    // m_process->Connect();
    // m_process.setStatus( ElConnectStatus.CONNECT );
    return true;
    ;
}

bool ElConnectSuccessState::TcpLogin(QByteArray msg)
{

    return true;
}

bool ElConnectSuccessState::SendTcpMessage(QByteArray msg)
{

    m_process->CurrentTcpImp()->SendTcpMessage(msg);

    return false;
}

bool ElConnectSuccessState::VMConnect()
{

    return false;
}

bool ElConnectSuccessState::VMLogin()
{

    return false;
}

bool ElConnectSuccessState::VMSendMessage()
{
    return false;
}

bool ElConnectSuccessState::ReConnect()
{
    return false;
}

bool ElConnectSuccessState::LoginOut()
{
    return false;
}

bool ElConnectSuccessState::InitState()
{
    return false;
}
