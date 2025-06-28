#pragma once
#include "tcp/ElConnectState.h"

class ElConnectStatusProcess;

class ElConnectSuccessState : public ElConnectState
{

    ElConnectStatusProcess* m_process;

public:
    ElConnectSuccessState(ElConnectStatusProcess* process);
    ~ElConnectSuccessState(void);

    virtual bool Connect();

    virtual bool TcpLogin(QByteArray msg);

    virtual bool SendTcpMessage(QByteArray msg);

    virtual bool VMConnect();

    virtual bool VMLogin();

    virtual bool VMSendMessage();

    virtual bool ReConnect();

    virtual bool LoginOut();

    virtual bool InitState();
};
