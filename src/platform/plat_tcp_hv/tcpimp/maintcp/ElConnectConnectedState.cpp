#include "stdafx.h"
#include "ElConnectConnectedState.h"
#include "ElConnectStatusProcess.h"
#include "tcpimp/tcphvimp.h"

ElConnectConnectedState::ElConnectConnectedState(ElConnectStatusProcess* process)
{

    m_process = process;
}

ElConnectConnectedState::~ElConnectConnectedState()
{
}

bool ElConnectConnectedState::Connect()
{
    m_process->CurrentTcpImp()->connectservice();

    // m_process.setStatus( ElConnectStatus.CONNECT );
    return true;
}

bool ElConnectConnectedState::TcpLogin(QByteArray msg)
{

    return true;
}

bool ElConnectConnectedState::SendTcpMessage(QByteArray msg)
{

    return false;
}

bool ElConnectConnectedState::VMConnect()
{

    return false;
}

bool ElConnectConnectedState::VMLogin()
{

    return false;
}

bool ElConnectConnectedState::VMSendMessage()
{
    return false;
}

bool ElConnectConnectedState::ReConnect()
{
    return false;
}

bool ElConnectConnectedState::LoginOut()
{
    return false;
}

bool ElConnectConnectedState::InitState()
{
    return false;
}
