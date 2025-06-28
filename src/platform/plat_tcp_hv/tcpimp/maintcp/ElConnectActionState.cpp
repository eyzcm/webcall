#include "ElConnectActionState.h"
#include "ElConnectStatusProcess.h"
#include <QByteArray>



ElConnectActionState::ElConnectActionState(ElConnectStatusProcess * process)
{
	
	
}

ElConnectActionState::~ElConnectActionState()
{
	
	
}

bool ElConnectActionState::Connect()
{
	m_process->Connect();
	//m_process.setStatus( ElConnectStatus.CONNECT );
	return true;
	;
}


bool ElConnectActionState::TcpLogin(QByteArray msg)
{
	m_process->TcpLogin( msg);
	return true;
}


bool ElConnectActionState::SendTcpMessage()
{
	m_process->SendTcpMessage();
	return false;
}


bool ElConnectActionState::VMConnect()
{
	m_process->VMConnect();
	return false;
}


bool ElConnectActionState::VMLogin()
{
	m_process->VMLogin();
	return false;
}


bool ElConnectActionState::VMSendMessage()
{
	return false;
}


bool ElConnectActionState::ReConnect()
{
	return false;
}


bool ElConnectActionState::LoginOut()
{
	return false;
}

bool ElConnectActionState::InitState()
{
	return false;
}
