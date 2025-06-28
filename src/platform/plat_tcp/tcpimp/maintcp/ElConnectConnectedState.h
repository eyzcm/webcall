#pragma once
#include "tcp/ElConnectState.h"

class ElConnectStatusProcess;

class ElConnectConnectedState : public ElConnectState
{

    ElConnectStatusProcess* m_process;

public:
    ElConnectConnectedState(ElConnectStatusProcess* process);
    ~ElConnectConnectedState(void);

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
