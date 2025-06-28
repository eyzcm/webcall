#pragma once
#include "tcp/ElConnectState.h"

class ElConnectStatusProcess;

class ElConnectActionState:public ElConnectState   {

	ElConnectStatusProcess * m_process;
public:
	 ElConnectActionState( ElConnectStatusProcess * process );
	~ElConnectActionState(void);
	
	

	virtual bool Connect();

	virtual bool TcpLogin(QByteArray msg);

	virtual bool SendTcpMessage();

	virtual bool VMConnect();

	virtual bool VMLogin();

	virtual bool VMSendMessage();

	virtual bool ReConnect();

	virtual bool LoginOut();

	virtual bool InitState(); 

};
