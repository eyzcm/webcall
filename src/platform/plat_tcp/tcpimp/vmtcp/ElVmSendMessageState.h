#pragma once
#include "ElVmState.h"

class ElVmStatusProcess;

class ElVmActionState:public ElVmState   {

	ElVmStatusProcess * m_process;
public:
	 ElVmActionState( ElVmStatusProcess * process );
	~ElVmActionState(void);
	
	

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
