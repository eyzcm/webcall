#include "stdafx.h"
#include "ElConnectConnectingState.h"
#include "ElConnectStatusProcess.h"
#include "tcpimp/tcphvimp.h"

ElConnectConnectingState::ElConnectConnectingState(ElConnectStatusProcess* process)
{
    m_process = process;
}

ElConnectConnectingState::~ElConnectConnectingState()
{
}

bool ElConnectConnectingState::Connect()
{
    m_process->CurrentTcpImp()->connectservice();

    // m_process.setStatus( ElConnectStatus.CONNECT );
    return true;
}

bool ElConnectConnectingState::TcpLogin(QByteArray msg)
{

    return true;
}

bool ElConnectConnectingState::SendTcpMessage(QByteArray msg)
{

    return false;
}

bool ElConnectConnectingState::VMConnect()
{

    return false;
}

bool ElConnectConnectingState::VMLogin()
{

    return false;
}

bool ElConnectConnectingState::VMSendMessage()
{
    return false;
}

bool ElConnectConnectingState::ReConnect()
{
    return false;
}

bool ElConnectConnectingState::LoginOut()
{
    return false;
}

bool ElConnectConnectingState::InitState()
{
    return false;
}
