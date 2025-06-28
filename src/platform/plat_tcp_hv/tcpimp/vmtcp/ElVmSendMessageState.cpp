
#include "ElVmStatusProcess.h"



ElVmActionState::ElVmActionState(ElVmStatusProcess * process)
{
	
	
}

ElVmActionState::~ElVmActionState()
{
	
	
}

bool ElVmActionState::Connect()
{
	m_process->Connect();
	//m_process.setStatus( ElVmStatus.CONNECT );
	return true;
	;
}


bool ElVmActionState::TcpLogin(QByteArray msg)
{
	m_process->TcpLogin( msg);
	return true;
}


bool ElVmActionState::SendTcpMessage()
{
	m_process->SendTcpMessage();
	return false;
}


bool ElVmActionState::VMConnect()
{
	m_process->VMConnect();
	return false;
}


bool ElVmActionState::VMLogin()
{
	m_process->VMLogin();
	return false;
}


bool ElVmActionState::VMSendMessage()
{
	return false;
}


bool ElVmActionState::ReConnect()
{
	return false;
}


bool ElVmActionState::LoginOut()
{
	return false;
}

bool ElVmActionState::InitState()
{
	return false;
}
